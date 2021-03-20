/*
 * hisilicon driver, hisi_isp_rproc.c
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>
#include <linux/amba/bus.h>
#include <linux/dma-mapping.h>
#include <linux/remoteproc.h>
#include <linux/platform_data/remoteproc_hisi.h>
#include <linux/hisi/hisi_mailbox.h>
#include <linux/hisi/hisi_rproc.h>
#include <linux/hisi/hisi_mailbox_dev.h>
#include <linux/delay.h>
#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/rproc_share.h>
#include <linux/sched/rt.h>
#include <linux/kthread.h>
#include <global_ddr_map.h>
#include <linux/iommu.h>
#include <linux/hisi-iommu.h>
#include <linux/miscdevice.h>
#include "hisp_internel.h"
#include "isp_ddr_map.h"
#include <linux/hisi/rdr_hisi_platform.h>
#include <securec.h>

#define ISP_MEM_SIZE    0x10000
#define CRGPERIPH_PERPWRSTAT_ADDR       (0x158)

static struct hisi_isp_nsec nsec_rproc_dev;

void dump_hisi_isp_boot(struct rproc *rproc, unsigned int size)
{
}

static void set_isp_nonsec(void)
{
	pr_alert("[%s] +\n", __func__);
	atfisp_set_nonsec();
	pr_alert("[%s] -\n", __func__);
}

int hisp_nsec_jpeg_powerup(void)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	int ret = 0;

	pr_info("[%s] +\n", __func__);
	mutex_lock(&dev->pwrlock);
	ret = hisp_subsys_powerup();
	if (ret != 0) {
		pr_err("[%s] Failed : ispup.%d\n", __func__, ret);
		mutex_unlock(&dev->pwrlock);
		return ret;
	}

	ret = ispcpu_qos_cfg();
	if (ret != 0) {
		pr_err("[%s] Failed : ispcpu_qos_cfg.%d\n",
				__func__, ret);
		goto isp_down;
	}
	set_isp_nonsec();

	ret = hisp_pwr_core_nsec_init();
	if (ret != 0) {
		pr_err("[%s] hisp_pwr_core_nsec_init.%d\n", __func__, ret);
		goto isp_down;
	}
	mutex_unlock(&dev->pwrlock);
	pr_info("[%s] -\n", __func__);

	return 0;

isp_down:
	if ((hisp_subsys_powerdn()) != 0)
		pr_err("[%s] Failed : ispdn\n", __func__);

	mutex_unlock(&dev->pwrlock);
	pr_info("[%s] -\n", __func__);

	return ret;
}
EXPORT_SYMBOL(hisp_nsec_jpeg_powerup);

/*lint -save -e631 -e613*/
int hisp_nsec_jpeg_powerdn(void)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	int ret;

	pr_info("%s: +\n", __func__);
	mutex_lock(&dev->pwrlock);
	ret = hisp_pwr_core_nsec_exit();
	if (ret != 0)
		pr_err("%s: hisp_pwr_core_nsec_exit failed, ret.%d\n",
			__func__, ret);

	ret = hisp_subsys_powerdn();
	if (ret != 0)
		pr_err("%s: hisp_subsys_powerdn failed, ret.%d\n",
			__func__, ret);

	mutex_unlock(&dev->pwrlock);
	pr_info("%s: -\n", __func__);

	return 0;
}
EXPORT_SYMBOL(hisp_nsec_jpeg_powerdn);
/*lint -restore */

int nonsec_isp_device_enable(void)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	int ret;

	mutex_lock(&dev->pwrlock);
	ret = hisp_subsys_powerup();
	if (ret != 0) {
		pr_err("[%s] Failed : hisp_subsys_powerup.%d\n", __func__, ret);
		mutex_unlock(&dev->pwrlock);
		return ret;
	}

	ret = ispcpu_qos_cfg();
	if (ret != 0) {
		pr_err("[%s] Failed : ispcpu_qos_cfg.%d\n", __func__, ret);
		goto isp_down;
	}
	set_isp_nonsec();

	ret = hisp_pwr_core_nsec_init();
	if (ret != 0) {
		pr_err("[%s] Failed : hisp_pwr_core_nsec_init.%d\n",
			__func__, ret);
		goto isp_down;
	}

	ret = hisp_pwr_cpu_nsec_dst(dev->remap_addr);
	if (ret != 0) {
		pr_err("[%s] Failed : hisp_pwr_cpu_nsec_dst.%d\n",
			__func__, ret);
		goto isp_exit;
	}
	mutex_unlock(&dev->pwrlock);
#ifdef CONFIG_HISI_ISP_DPM
	hisi_isp_dpm_init();
#endif

	return 0;

isp_exit:
	if ((hisp_pwr_core_nsec_exit()) != 0)
		pr_err("[%s] Failed : hisp_pwr_core_nsec_exit\n", __func__);
isp_down:
	if ((hisp_subsys_powerdn()) != 0)
		pr_err("[%s] Failed : hisp_subsys_powerdn\n", __func__);

	mutex_unlock(&dev->pwrlock);

	return ret;
}
EXPORT_SYMBOL(nonsec_isp_device_enable);

int nonsec_isp_device_disable(void)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	int ret;
#ifdef CONFIG_HISI_ISP_DPM
	hisi_isp_dpm_deinit();
#endif
	mutex_lock(&dev->pwrlock);
	ret = hisp_pwr_cpu_nsec_rst();
	if (ret != 0)
		pr_err("[%s] hisp_pwr_cpu_nsec_rst failed, ret.%d\n",
			__func__, ret);

	ret = hisp_pwr_core_nsec_exit();
	if (ret != 0)
		pr_err("[%s] hisp_pwr_core_nsec_exit failed, ret.%d\n",
			__func__, ret);

	ret = hisp_subsys_powerdn();
	if (ret != 0)
		pr_err("[%s] hisp_subsys_powerdn failed, ret.%d\n",
			__func__, ret);
	mutex_unlock(&dev->pwrlock);

	return 0;
}
EXPORT_SYMBOL(nonsec_isp_device_disable);

int hisp_get_pgd_base(u64 *pgd_base)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;

	*pgd_base = hisi_domain_get_ttbr(dev->device);
	pr_info("[%s] -\n", __func__);
	return 0;
}

static int set_nonsec_pgd(struct rproc *rproc)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	struct rproc_shared_para *param = NULL;
	int ret;

	ret = hisp_get_pgd_base(&dev->pgd_base);
	if (ret < 0) {
		pr_err("[%s] Failed : hisp_get_pgd_base.%d\n", __func__, ret);
		return ret;
	}

	ret = hisi_isp_set_pgd();
	if (ret < 0) {
		pr_err("[%s] Failed : hisi_isp_set_pgd.%d\n", __func__, ret);
		return ret;
	}

	hisp_lock_sharedbuf();
	param = rproc_get_share_para();
	if (param == NULL) {
		pr_err("[%s] Failed : param.%pK\n", __func__, param);
		hisp_unlock_sharedbuf();
		return -EINVAL;
	}
	param->dynamic_pgtable_base = dev->pgd_base;
	hisp_unlock_sharedbuf();

	return 0;
}

int hisi_isp_rproc_pgd_set(struct rproc *rproc)
{
	int err = 0;

	if (use_nonsec_isp()) {
		pr_info("[%s] +\n", __func__);
		err = set_nonsec_pgd(rproc);
		if (err != 0) {
			pr_err("[%s] Failed : set_nonsec_pgd.%d\n",
					__func__, err);
			return err;
		}
	} else {
		struct rproc_shared_para *param = NULL;

		hisp_lock_sharedbuf();
		param = rproc_get_share_para();
		if (param != NULL)
			param->dynamic_pgtable_base = get_nonsec_pgd();
		hisp_unlock_sharedbuf();
	}

	return 0;
}

static int isp_remap_rsc(struct hisi_isp_nsec *dev)
{
	dev->isp_dma_va = dma_alloc_coherent(dev->device, ISP_MEM_SIZE,
					&dev->isp_dma, GFP_KERNEL);
	if (dev->isp_dma_va == NULL) {
		pr_err("[%s] isp_dma_va failed\n", __func__);
		return -ENOMEM;
	}
	pr_info("[%s] isp_dma_va.%pK\n", __func__, dev->isp_dma_va);

	return 0;
}

static void isp_unmap_rsc(struct hisi_isp_nsec *dev)
{
	if (dev->isp_dma_va != NULL)
		dma_free_coherent(dev->device, ISP_MEM_SIZE,
				dev->isp_dma_va, dev->isp_dma);

	dev->isp_dma_va = NULL;
}

int hisi_isp_nsec_probe(struct platform_device *pdev)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;
	int ret;

	pr_alert("[%s] +\n", __func__);

	dev->device = &pdev->dev;
	dev->isp_pdev = pdev;

	ret = isp_remap_rsc(dev);
	if (ret != 0) {
		pr_err("[%s] failed, isp_remap_src.%d\n", __func__, ret);
		return ret;
	}

	ret = hisp_nsec_getdts(pdev, dev);
	if (ret != 0) {
		pr_err("[%s] Failed : hisi_isp_nsec_getdts.%d.\n",
				__func__, ret);
		goto hisi_isp_nsec_getdts_fail;
	}

	mutex_init(&dev->pwrlock);
	pr_alert("[%s] -\n", __func__);

	return 0;
hisi_isp_nsec_getdts_fail:
	isp_unmap_rsc(dev);

	return ret;
}

int hisi_isp_nsec_remove(struct platform_device *pdev)
{
	struct hisi_isp_nsec *dev = &nsec_rproc_dev;

	mutex_destroy(&dev->pwrlock);
	isp_unmap_rsc(dev);
	return 0;
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("HiStar V150 rproc driver");

