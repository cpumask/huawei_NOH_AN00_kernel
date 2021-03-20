/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  this file implements api for node hisi-ivp to register, device
 * init, remove, etc
 * Author: chenweiyu
 * Create: 2019-07-30
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/pm_wakeup.h>
#include <linux/ion.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/hisi-iommu.h>
#include <linux/syscalls.h>
#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include <linux/firmware.h>
#include "securec.h"
#include "ivp.h"
#include "ivp_log.h"
#include "ivp_sec.h"
#include "ivp_manager.h"
#include "ivp_ioctl.h"
#ifdef CONFIG_IVP_SMMU_V3
#include "ivp_smmuv3.h"
#else
#include "ivp_smmu.h"
#endif
#include "ivp_reg.h"
/*lint -save -e750 -e753 -specific(-e753) -e750 -e528*/
/*-specific(-e528) -e144 -e82 -e64 -e785 -e715 -e712 -e40*/
/*lint -save -e63 -e732 -e42 -e550 -e438 -e834 -e648 -e747 -e778 -e50*/
/*lint -save -e749 -e84 -e866 -e514 -e846 -e737 -e574*/

static void ivp_mutex_init(struct ivp_device *pdev)
{
	mutex_init(&pdev->ivp_comm.ivp_wake_lock_mutex);
	mutex_init(&pdev->ivp_comm.ivp_load_image_mutex);
	mutex_init(&pdev->ivp_comm.ivp_power_up_off_mutex);
	mutex_init(&pdev->ivp_comm.ivp_open_release_mutex);
}

static void ivp_mutex_deinit(struct ivp_device *pdev)
{
	mutex_destroy(&pdev->ivp_comm.ivp_open_release_mutex);
	mutex_destroy(&pdev->ivp_comm.ivp_power_up_off_mutex);
	mutex_destroy(&pdev->ivp_comm.ivp_load_image_mutex);
	mutex_destroy(&pdev->ivp_comm.ivp_wake_lock_mutex);
}

static int ivp_open(struct inode *inode, struct file *fd)
{
	struct ivp_device *pdev = NULL;
	struct miscdevice *pmiscdev =  NULL;

	if (!fd || !fd->private_data) {
		ivp_err("invalid param fd");
		return -ENODEV;
	}

	pmiscdev =  (struct miscdevice *)fd->private_data;
	pdev = container_of(pmiscdev, struct ivp_device, ivp_comm.device);

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

#ifdef CONFIG_IVP_SMMU_V3
	if (pdev->ivp_comm.ivp_pdev->dev.iommu_fwspec->num_ids == IVP_SMMU_ID_MAX) {
		pdev->sid =
			pdev->ivp_comm.ivp_pdev->dev.iommu_fwspec->ids[IVP_SMMU_SID];
		pdev->ssid =
			pdev->ivp_comm.ivp_pdev->dev.iommu_fwspec->ids[IVP_SMMU_SSID];
	} else {
		ivp_err("invalid sid ssid param number");
		return -EINVAL;
	}
#endif

	mutex_lock(&pdev->ivp_comm.ivp_open_release_mutex);

	if (atomic_read(&pdev->ivp_comm.accessible) == 0) {
		mutex_unlock(&pdev->ivp_comm.ivp_open_release_mutex);
		ivp_err("maybe ivp dev has been opened!");
		return -EBUSY;
	}

	atomic_dec(&pdev->ivp_comm.accessible);

	fd->private_data = (void *)pdev;
	mutex_unlock(&pdev->ivp_comm.ivp_open_release_mutex);
	ivp_info("open node %s success", pdev->ivp_comm.device.name);
	return 0;
}

static int ivp_release(struct inode *inode, struct file *fd)
{
	struct ivp_device *pdev = NULL;
	int ret = 0;

	if (!fd || !fd->private_data) {
		ivp_err("invalid param fd");
		return -ENODEV;
	}

	pdev = fd->private_data;

	mutex_lock(&pdev->ivp_comm.ivp_open_release_mutex);
	if (atomic_read(&pdev->ivp_comm.accessible) != 0) {
		mutex_unlock(&pdev->ivp_comm.ivp_open_release_mutex);
		ivp_err("maybe ivp dev not opened!");
		return -1;
	}

	mutex_lock(&pdev->ivp_comm.ivp_power_up_off_mutex);
	ivp_poweroff(pdev);
	mutex_unlock(&pdev->ivp_comm.ivp_power_up_off_mutex);

#ifdef IVP_DUAL_CORE
	ivp_info("ivp%d device closed", pdev->core_id);
#else
	ivp_info("ivp device closed");
#endif

	atomic_inc(&pdev->ivp_comm.accessible);
	mutex_unlock(&pdev->ivp_comm.ivp_open_release_mutex);
	return ret;
}

#ifdef CONFIG_COMPAT
static long ivp_ioctl32(struct file *fd, unsigned int cmd, unsigned long arg)
{
	void *ptr_user = compat_ptr(arg);
	return ivp_ioctl(fd, cmd, (uintptr_t)ptr_user);
}
#endif

static bool is_mem_in_range(unsigned long addr, unsigned long size,
		struct ivp_sect_info *mem_sect, int sect_count)
{
	int i;
	for (i = 0; i < sect_count; i++) {
		if (addr >= (mem_sect[i].acpu_addr << IVP_MMAP_SHIFT) &&
			(addr <= ((mem_sect[i].acpu_addr << IVP_MMAP_SHIFT) +
				mem_sect[i].len)) &&
			(addr + size) <= ((mem_sect[i].acpu_addr << IVP_MMAP_SHIFT) +
				mem_sect[i].len)) {
			ivp_info("Valid section %d for target.", i);
			return true;
		}
	}
	return false;
}

static int ivp_mmap(struct file *fd, struct vm_area_struct *vma)
{
	int ret;
	unsigned long size;
	unsigned long mm_pgoff;
	unsigned long phy_addr;
	struct ivp_device *pdev = NULL;

	if (!fd || !vma || !fd->private_data) {
		ivp_err("invalid input param");
		return -EINVAL;
	}

	mm_pgoff = (vma->vm_pgoff << IVP_MMAP_SHIFT);
	phy_addr = vma->vm_pgoff << (PAGE_SHIFT + IVP_MMAP_SHIFT);
	pdev = fd->private_data;

	size = vma->vm_end - vma->vm_start;
	if (size > LISTENTRY_SIZE) {
		ivp_err("Invalid size = 0x%08x", size);
		return -EINVAL;
	}

	if (!is_mem_in_range(phy_addr, size, pdev->ivp_comm.sects,
			pdev->ivp_comm.sect_count) &&
		!is_mem_in_range(phy_addr, size, pdev->ivp_comm.sec_sects,
			pdev->ivp_comm.sect_count)) {
		ivp_err("mmap addr invalid 0x%lx, size 0x%lx", phy_addr, size);
		return -EINVAL;
	}

	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	ret = remap_pfn_range(vma, vma->vm_start,
			mm_pgoff, size, vma->vm_page_prot);
	if (ret < 0) {
		ivp_err("Failed to map address space. Error code is %d", ret);
		return ret;
	}
	ivp_dbg("nocached success, ret:%#x", ret);

	return 0;
}

static const struct file_operations ivp_fops = {
	.owner = THIS_MODULE,
	.open = ivp_open,
	.release = ivp_release,
	.unlocked_ioctl = ivp_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = ivp_ioctl32,
#endif
	.mmap = ivp_mmap,
};

static int ivp_remove(struct platform_device *plat_devp)
{
	struct ivp_device *pdev =
		(struct ivp_device *)platform_get_drvdata(plat_devp);

	if (!pdev || !pdev->ivp_comm.sects) {
		ivp_err("invalid param pdev");
		return -ENODEV;
	}

	misc_deregister(&pdev->ivp_comm.device);

	ivp_deinit_pri(pdev);

	kfree(pdev->ivp_comm.sects);
	kfree(pdev->ivp_comm.sec_sects);
#ifdef SEC_IVP_ENABLE
	kfree(pdev->sec_dev);
	pdev->sec_dev = NULL;
#endif
	pdev->ivp_comm.sects = NULL;
	pdev->ivp_comm.sec_sects = NULL;
	pdev->ivp_comm.sect_count = 0;
	ivp_release_iores(plat_devp);

	pdev->ivp_comm.smmu_dev = NULL;
	wakeup_source_trash(&pdev->ivp_comm.ivp_power_wakelock);
	ivp_mutex_deinit(pdev);
	return 0;
}

static int ivp_device_init(struct platform_device *plat_dev,
		struct ivp_device *pdev)
{
	int ret;

	atomic_set(&pdev->ivp_comm.accessible, 1);
	atomic_set(&pdev->ivp_comm.poweron_access, 1);
	atomic_set(&pdev->ivp_comm.poweron_success, 1);
	ivp_mutex_init(pdev);

	/*lint -save -e838*/
	ret = misc_register(&pdev->ivp_comm.device);
	if (ret)
		goto err_out_misc;

	ret = ivp_setup_smmu_dev(pdev);
	if (ret)
		goto err_out_smmu;

	ret = ivp_setup_onchipmem_sections(plat_dev, pdev);
	if (ret)
		goto err_out_onchipmem;

	ret = ivp_setup_irq(plat_dev, pdev);
	if (ret)
		goto err_out_irq;

	ret = ivp_init_reg_res(plat_dev, pdev);
	if (ret)
		goto err_out_reg_res;

	ret = ivp_init_pri(plat_dev, pdev);
	if (ret)
		goto err_out_init_pri;

#ifdef SEC_IVP_ENABLE
	if (pdev->ivp_comm.ivp_sec_support) {
		pdev->sec_dev = (struct ivp_sec_device *)kzalloc(
				sizeof(struct ivp_sec_device), GFP_KERNEL);
		if (!pdev->sec_dev) {
			ivp_err("kmalloc ivp sec_dev fail");
			ret = -ENOMEM;
			goto err_out_sec_dev;
		}
	}
#endif

	ivp_info("device:%s init success", pdev->ivp_comm.device.name);
	return ret;
	/*lint -restore*/
#ifdef SEC_IVP_ENABLE
err_out_sec_dev:
	ivp_deinit_pri(pdev);
#endif

err_out_init_pri:
	ivp_release_iores(plat_dev);
err_out_reg_res:
err_out_irq:
	kfree(pdev->ivp_comm.sects);
	kfree(pdev->ivp_comm.sec_sects);
	pdev->ivp_comm.sects = NULL;
	pdev->ivp_comm.sec_sects = NULL;
	pdev->ivp_comm.sect_count = 0;
err_out_onchipmem:
	pdev->ivp_comm.smmu_dev = NULL;
err_out_smmu:
	misc_deregister(&pdev->ivp_comm.device);
err_out_misc:
	ivp_mutex_deinit(pdev);
	wakeup_source_trash(&pdev->ivp_comm.ivp_power_wakelock);

	ivp_err("device:%s init error", pdev->ivp_comm.device.name);
	return ret;
}

static int ivp_probe(struct platform_device *plat_dev)
{
	struct ivp_device *pdev = NULL;
	int ret = 0;

	if (!plat_dev) {
		ivp_err("plat_dev is NULL");
		return -EINVAL;
	}

	pdev = kzalloc(sizeof(struct ivp_device), GFP_KERNEL);
	if (!pdev) {
		ivp_err("ivp device kzalloc failed!");
		return -ENOMEM;
	}

	pdev->ivp_comm.ivp_pdev = plat_dev;
	pdev->ivp_comm.device.minor = MISC_DYNAMIC_MINOR,
	pdev->ivp_comm.device.name = IVP_MODULE_NAME,
	pdev->ivp_comm.device.fops = &ivp_fops,
#ifdef IVP_DUAL_CORE
	pdev->core_id = IVP_CORE0_ID;
	pdev->base_offset = 0;
#endif

	platform_set_drvdata(plat_dev, pdev);
	wakeup_source_init(&pdev->ivp_comm.ivp_power_wakelock, "ivp_power_wakelock");

	ret = ivp_device_init(plat_dev, pdev);
	if (ret) {
		ivp_err("ivp_device_init failed %d", ret);
		return ret;
	}
	ivp_info("ivp_device %s probe success", pdev->ivp_comm.device.name);
	return ret;
}

/*lint -save -e785 -e64*/
#ifdef CONFIG_OF
static const struct of_device_id ivp_of_id[] = {
	{ .compatible = "hisilicon,hisi-ivp", },
	{}
};
#endif

static struct platform_driver ivp_platform_driver = {
	.probe = ivp_probe,
	.remove = ivp_remove,
	.driver = {
		.name = IVP_MODULE_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = of_match_ptr(ivp_of_id),
#endif
	},
};

module_platform_driver(ivp_platform_driver);
/*lint -restore*/

#ifdef IVP_DUAL_CORE
static int ivp1_probe(struct platform_device *plat_dev)
{
	int ret = 0;
	struct ivp_device *pdev = NULL;

	if (!plat_dev) {
		ivp_err("plat_dev is NULL");
		return -EINVAL;
	}

	pdev = kzalloc(sizeof(struct ivp_device), GFP_KERNEL);
	if (!pdev) {
		ivp_err("ivp device kzalloc failed");
		return -ENOMEM;
	}

	pdev->ivp_comm.ivp_pdev = plat_dev;
	pdev->ivp_comm.device.minor = MISC_DYNAMIC_MINOR,
	pdev->ivp_comm.device.name = IVP1_MODULE_NAME,
	pdev->ivp_comm.device.fops = &ivp_fops,
	pdev->core_id = IVP_CORE1_ID;
	pdev->base_offset = IVP_BASE_REMAP_OFFSET;

	platform_set_drvdata(plat_dev, pdev);
	wakeup_source_init(&pdev->ivp_comm.ivp_power_wakelock, "ivp1_power_wakelock");

	ret = ivp_device_init(plat_dev, pdev);
	if (ret) {
		ivp_err("ivp_device_init failed %d", ret);
		return ret;
	}

	ivp_info("ivp device %s probe success", pdev->ivp_comm.device.name);
	return ret;
}
#ifdef CONFIG_OF
static const struct of_device_id ivp1_of_id[] = {
	{ .compatible = "hisilicon,hisi-ivp1", },
	{}
};
#endif
static struct platform_driver ivp1_platform_driver = {
	.probe = ivp1_probe,
	.remove = ivp_remove,
	.driver = {
		.name = IVP1_MODULE_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = of_match_ptr(ivp1_of_id),
#endif
	},
};
module_platform_driver(ivp1_platform_driver);
#endif

MODULE_DESCRIPTION("hisilicon_ivp driver");
MODULE_LICENSE("GPL");
//lint -restore
