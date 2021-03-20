/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  this file implements ioctl for node hisi-ivp
 * Author:  chenweiyu
 * Create:  2019-07-30
 */

#include "ivp_ioctl.h"
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/dma-buf.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/ion.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/hisi-iommu.h>
#include <linux/syscalls.h>
#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include <linux/firmware.h>
#include <linux/hisi/hisi_efuse.h>
#include "securec.h"
#include "ivp_manager.h"
#include "ivp.h"
#include "ivp_log.h"
#include "ivp_reg.h"
#include "ivp_sec.h"
#include "ivp_map.h"
#ifdef CONFIG_IVP_SMMU_V3
#include "ivp_smmuv3.h"
#else
#include "ivp_smmu.h"
#endif

#ifdef MULTIPLE_ALGO
#include "elf.h"

#define IVP_ALGO_MAX_SIZE            0x00500000
#define IVP_IRAM_TEXT_SEGMENT_ADDR   0xE9102000
#define IVP_DRAM0_DATA_SEGMENT_ADDR  0xE9000000
#define IVP_DRAM0_BSS_SEGMENT_ADDR   0xE9003C00

#define ALGO_DDR_RECORD_MAX          200
#define ALGO_IRAM_RECORD_MAX         96
#define ALGO_DRAM_DATA_RECORD_MAX    60
#define ALGO_DRAM_BSS_RECORD_MAX     36

#define ALGO_DDR_NODE_SIZE           (25 * 1024)
#define ALGO_MEM_NODE_SIZE           256

#define EFUSE_PARTIAL_PASSP2_START   2276
#define EFUSE_IVP_BUFFER_NUM         3
#define EFUSE_IVP_CORE0_POS          2308
#define EFUSE_IVP_CORE1_POS          2307
#define EFUSE_IVP_ALL_AVAILABLE      0x03
#define EFUSE_BUF_POS(x)  (((x) - EFUSE_PARTIAL_PASSP2_START) / 32)
#define EFUSE_BIT_POS(x)  (((x) - EFUSE_PARTIAL_PASSP2_START) % 32)


/* record for algo nodes */
bool algo_ddr_record[ALGO_DDR_RECORD_MAX];
bool algo_core0_iram_record[ALGO_IRAM_RECORD_MAX];
bool algo_core1_iram_record[ALGO_IRAM_RECORD_MAX];
bool algo_core0_dram_data_record[ALGO_DRAM_DATA_RECORD_MAX];
bool algo_core1_dram_data_record[ALGO_DRAM_DATA_RECORD_MAX];
bool algo_core0_dram_bss_record[ALGO_DRAM_BSS_RECORD_MAX];
bool algo_core1_dram_bss_record[ALGO_DRAM_BSS_RECORD_MAX];

/* two cores use same ddr-zone for text sections */
bool dyn_core_loaded[2];

struct record_node {
	bool *mem_pool;
	unsigned int pool_size;
	unsigned int node_size;
	unsigned int start_zone;
	unsigned int size_zone;
};

static void init_algo_mem_info(struct ivp_device *pdev,
		const unsigned int algo_id)
{
	pdev->algo_mem_info[algo_id].algo_start_addr = 0;
	pdev->algo_mem_info[algo_id].algo_rel_addr   = 0;
	pdev->algo_mem_info[algo_id].algo_rel_count  = 0;
	pdev->algo_mem_info[algo_id].algo_ddr_addr   = 0;
	pdev->algo_mem_info[algo_id].algo_ddr_vaddr  = 0;
	pdev->algo_mem_info[algo_id].algo_ddr_size   = 0;
	pdev->algo_mem_info[algo_id].algo_func_addr  = 0;
	pdev->algo_mem_info[algo_id].algo_func_vaddr = 0;
	pdev->algo_mem_info[algo_id].algo_func_size  = 0;
	pdev->algo_mem_info[algo_id].algo_data_addr  = 0;
	pdev->algo_mem_info[algo_id].algo_data_vaddr = 0;
	pdev->algo_mem_info[algo_id].algo_data_size  = 0;
	pdev->algo_mem_info[algo_id].algo_bss_addr   = 0;
	pdev->algo_mem_info[algo_id].algo_bss_vaddr  = 0;
	pdev->algo_mem_info[algo_id].algo_bss_size   = 0;
}

static int ivp_algo_mem_init(struct ivp_device *pdev)
{
	int i;

	pdev->core_status = FREE;
	pdev->algo_mem_info = (struct ivp_algo_mem_info *)kzalloc(
		sizeof(struct ivp_algo_mem_info) * IVP_ALGO_NODE_MAX, GFP_KERNEL);
	if (!pdev->algo_mem_info) {
		ivp_err("kmalloc ivp algo_mem fail");
		return -ENOMEM;
	}

	for (i = 0; i < IVP_ALGO_NODE_MAX; i++) {
		pdev->algo_mem_info[i].occupied = FREE;
		init_algo_mem_info(pdev, i);
	}
	return EOK;
}

static int ivp_base_record_info(const struct ivp_device *pdev,
		const unsigned int segment_type, struct record_node *node)
{
	if (segment_type == SEGMENT_DDR_TEXT) {
		node->mem_pool = algo_ddr_record;
		node->pool_size = ALGO_DDR_RECORD_MAX;
		node->node_size = ALGO_DDR_NODE_SIZE;
	} else if (segment_type == SEGMENT_IRAM_TEXT) {
		if (pdev->core_id == IVP_CORE0_ID)
			node->mem_pool = algo_core0_iram_record;
		else
			node->mem_pool = algo_core1_iram_record;
		node->pool_size = ALGO_IRAM_RECORD_MAX;
		node->node_size = ALGO_MEM_NODE_SIZE;
	} else if (segment_type == SEGMENT_DRAM0_DATA) {
		if (pdev->core_id == IVP_CORE0_ID)
			node->mem_pool = algo_core0_dram_data_record;
		else
			node->mem_pool = algo_core1_dram_data_record;
		node->pool_size = ALGO_DRAM_DATA_RECORD_MAX;
		node->node_size = ALGO_MEM_NODE_SIZE;
	} else if (segment_type == SEGMENT_DRAM0_BSS) {
		if (pdev->core_id == IVP_CORE0_ID)
			node->mem_pool = algo_core0_dram_bss_record;
		else
			node->mem_pool = algo_core1_dram_bss_record;
		node->pool_size = ALGO_DRAM_BSS_RECORD_MAX;
		node->node_size = ALGO_MEM_NODE_SIZE;
	} else {
		ivp_err("invalid segment type");
		return -EINVAL;
	}
	return 0;
}

static int ivp_alloc_record_info(const struct ivp_device *pdev,
		const unsigned int algo_id, const unsigned int segment_type,
		struct record_node *node)
{
	if (ivp_base_record_info(pdev, segment_type, node) != 0) {
		return -EINVAL;
	}

	if (segment_type == SEGMENT_DDR_TEXT) {
		node->size_zone = pdev->algo_mem_info[algo_id].algo_ddr_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_ddr_size % node->node_size != 0)
			node->size_zone += 1;
	} else if (segment_type == SEGMENT_IRAM_TEXT) {
		node->size_zone = pdev->algo_mem_info[algo_id].algo_func_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_func_size % node->node_size != 0)
			node->size_zone += 1;
	} else if (segment_type == SEGMENT_DRAM0_DATA) {
		node->size_zone = pdev->algo_mem_info[algo_id].algo_data_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_data_size % node->node_size != 0)
			node->size_zone += 1;
	} else if (segment_type == SEGMENT_DRAM0_BSS) {
		node->size_zone = pdev->algo_mem_info[algo_id].algo_bss_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_bss_size % node->node_size != 0)
			node->size_zone += 1;
	}
	return 0;
}

static int ivp_free_record_info(struct ivp_device *pdev,
		const unsigned int algo_id, const unsigned int segment_type,
		struct record_node *node)
{
	if (ivp_base_record_info(pdev, segment_type, node) != 0) {
		return -EINVAL;
	}

	if (segment_type == SEGMENT_DDR_TEXT) {
		node->start_zone = (pdev->algo_mem_info[algo_id].algo_ddr_addr -
			(pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr + SIZE_1M)) /
			node->node_size;
		node->size_zone = pdev->algo_mem_info[algo_id].algo_ddr_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_ddr_size % node->node_size != 0)
			node->size_zone += 1;
		pdev->algo_mem_info[algo_id].algo_ddr_addr = 0;
	} else if (segment_type == SEGMENT_IRAM_TEXT) {
		node->start_zone = (pdev->algo_mem_info[algo_id].algo_func_addr -
			IVP_IRAM_TEXT_SEGMENT_ADDR) / node->node_size;
		node->size_zone = pdev->algo_mem_info[algo_id].algo_func_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_func_size % node->node_size != 0)
			node->size_zone += 1;
		pdev->algo_mem_info[algo_id].algo_func_addr = 0;
	} else if (segment_type == SEGMENT_DRAM0_DATA) {
		node->start_zone = (pdev->algo_mem_info[algo_id].algo_data_addr -
			IVP_DRAM0_DATA_SEGMENT_ADDR) / node->node_size;
		node->size_zone = pdev->algo_mem_info[algo_id].algo_data_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_data_size % node->node_size != 0)
			node->size_zone += 1;
		pdev->algo_mem_info[algo_id].algo_data_addr = 0;
	} else if (segment_type == SEGMENT_DRAM0_BSS) {
		node->start_zone = (pdev->algo_mem_info[algo_id].algo_bss_addr -
			IVP_DRAM0_BSS_SEGMENT_ADDR) / node->node_size;
		node->size_zone = pdev->algo_mem_info[algo_id].algo_bss_size /
			node->node_size;
		if (pdev->algo_mem_info[algo_id].algo_bss_size % node->node_size != 0)
			node->size_zone += 1;
		pdev->algo_mem_info[algo_id].algo_bss_addr = 0;
	}
	return 0;
}

static int ivp_alloc_algo_node(struct ivp_device *pdev,
		const unsigned int algo_id, const unsigned int segment_type)
{
	unsigned int best;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k;
	struct record_node node;
	(void)memset_s(&node, sizeof(node), 0, sizeof(node));

	if (ivp_alloc_record_info(pdev, algo_id, segment_type, &node) != 0) {
		ivp_err("get alloc record info failed");
		return -EINVAL;
	}
	best = node.pool_size + 1;
	while (i < node.pool_size) {
		while (j < node.pool_size && node.mem_pool[j]) {
			i++;
			j++;
		}
		while (j < node.pool_size && !node.mem_pool[j])
			j++;
		if ((j - i >= node.size_zone) && (j - i < best)) {
			best = j - i;
			node.start_zone = i;
		}
		i = j;
	}

	if (best <= node.pool_size) {
		for (k = node.start_zone; k < node.start_zone + node.size_zone; k++)
			node.mem_pool[k] = true;
		if (segment_type == SEGMENT_DDR_TEXT)
			pdev->algo_mem_info[algo_id].algo_ddr_addr =
				(pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr + SIZE_1M) +
				node.start_zone * node.node_size;
		else if (segment_type == SEGMENT_IRAM_TEXT)
			pdev->algo_mem_info[algo_id].algo_func_addr =
				IVP_IRAM_TEXT_SEGMENT_ADDR + node.start_zone * node.node_size;
		else if (segment_type == SEGMENT_DRAM0_DATA)
			pdev->algo_mem_info[algo_id].algo_data_addr =
				IVP_DRAM0_DATA_SEGMENT_ADDR + node.start_zone * node.node_size;
		else if (segment_type == SEGMENT_DRAM0_BSS)
			pdev->algo_mem_info[algo_id].algo_bss_addr =
				IVP_DRAM0_BSS_SEGMENT_ADDR + node.start_zone * node.node_size;
		ivp_info("core[%u]-algo[%u] alloc memory pool_size[%u] node_size[%u] \
			start_zone[%u] size_zone[%u]", pdev->core_id, algo_id,
			node.pool_size, node.node_size, node.start_zone, node.size_zone);
		return 0;
	}
	return -ENOMEM;
}

static int ivp_free_algo_zone(struct ivp_device *pdev,
		unsigned int algo_id, unsigned int segment_type)
{
	unsigned int k;
	struct record_node node;
	if (ivp_free_record_info(pdev, algo_id, segment_type, &node) != 0) {
		ivp_err("get free record info failed");
		return -EINVAL;
	}

	if (node.pool_size <= node.start_zone + node.size_zone) {
		ivp_warn("invalid start[%x] size[%x]", node.start_zone, node.size_zone);
		return -EINVAL;
	}

	for (k = node.start_zone; k < node.start_zone + node.size_zone; k++)
		node.mem_pool[k] = false;
	ivp_info("core[%u]-algo[%u] free memory pool_size[%u] node_size[%u] \
		start_zone[%u] size_zone[%u]", pdev->core_id, algo_id,
		node.pool_size, node.node_size, node.start_zone, node.size_zone);

	return 0;
}

static int ivp_alloc_algo_mem(struct ivp_device *pdev,
		const XT_Elf32_Phdr header, const unsigned int algo_id)
{
	if (header.p_vaddr < IVP_ALGO_MAX_SIZE) {
		pdev->algo_mem_info[algo_id].algo_ddr_vaddr = header.p_vaddr;
		pdev->algo_mem_info[algo_id].algo_ddr_size = header.p_memsz;
		if (ivp_alloc_algo_node(pdev, algo_id, SEGMENT_DDR_TEXT) != 0)
			goto err_ivp_alloc_mem;
	} else if (header.p_vaddr == IVP_IRAM_TEXT_SEGMENT_ADDR) {
		pdev->algo_mem_info[algo_id].algo_func_vaddr = header.p_vaddr;
		pdev->algo_mem_info[algo_id].algo_func_size = header.p_memsz;
		if (ivp_alloc_algo_node(pdev, algo_id, SEGMENT_IRAM_TEXT) != 0)
			goto err_ivp_alloc_mem;
	} else if (header.p_vaddr == IVP_DRAM0_DATA_SEGMENT_ADDR) {
		pdev->algo_mem_info[algo_id].algo_data_vaddr = header.p_vaddr;
		pdev->algo_mem_info[algo_id].algo_data_size = header.p_memsz;
		if (ivp_alloc_algo_node(pdev, algo_id, SEGMENT_DRAM0_DATA) != 0)
			goto err_ivp_alloc_mem;
	} else if (header.p_vaddr == IVP_DRAM0_BSS_SEGMENT_ADDR) {
		pdev->algo_mem_info[algo_id].algo_bss_vaddr = header.p_vaddr;
		pdev->algo_mem_info[algo_id].algo_bss_size = header.p_memsz;
		if (ivp_alloc_algo_node(pdev, algo_id, SEGMENT_DRAM0_BSS) != 0)
			goto err_ivp_alloc_mem;
	} else {
		ivp_err("invalid segment address");
		goto err_ivp_alloc_mem;
	}
	return 0;

err_ivp_alloc_mem:
	ivp_err("alloc memory fail");
	if (pdev->algo_mem_info[algo_id].algo_ddr_addr != 0)
		ivp_free_algo_zone(pdev, algo_id, SEGMENT_DDR_TEXT);
	if (pdev->algo_mem_info[algo_id].algo_func_addr != 0)
		ivp_free_algo_zone(pdev, algo_id, SEGMENT_IRAM_TEXT);
	if (pdev->algo_mem_info[algo_id].algo_data_addr != 0)
		ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_DATA);
	if (pdev->algo_mem_info[algo_id].algo_bss_addr != 0)
		ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_BSS);
	return -EINVAL;
}
#endif

static int ivp_poweron_remap(struct ivp_device *ivp_devp)
{
	int ret;
	unsigned int section_index;

#ifdef SEC_IVP_ENABLE
	if (ivp_devp->ivp_comm.ivp_secmode == SECURE_MODE) {
		ret = ivp_sec_poweron_remap(ivp_devp);
	} else
#endif
	{
#ifdef MULTIPLE_ALGO
		section_index = BASE_SECT_INDEX;
#else
		section_index = DDR_SECTION_INDEX;
#endif
		ret = ivp_remap_addr_ivp2ddr(ivp_devp,
			ivp_devp->ivp_comm.sects[section_index].ivp_addr,
			ivp_devp->ivp_comm.ivp_meminddr_len,
			ivp_devp->ivp_comm.sects[section_index].acpu_addr << IVP_MMAP_SHIFT);
	}
	if (ret)
		ivp_err("remap addr failed %d", ret);

	return ret;
}

static int ivp_dev_poweron(struct ivp_device *pdev)
{
	int ret;

	mutex_lock(&pdev->ivp_comm.ivp_wake_lock_mutex);
	if (!pdev->ivp_comm.ivp_power_wakelock.active) {
		__pm_stay_awake(&pdev->ivp_comm.ivp_power_wakelock);
		ivp_info("ivp power on enter, wake lock");
	}
	mutex_unlock(&pdev->ivp_comm.ivp_wake_lock_mutex); /*lint !e456*/

	ret = ivp_poweron_pri(pdev);
	if (ret) {
		ivp_err("power on pri setting failed [%d]", ret);
		goto err_ivp_poweron_pri;
	}

	/* set auto gate clk etc. */
	if (pdev->ivp_comm.ivp_secmode == SECURE_MODE)
		ivp_dev_set_dynamic_clk(pdev, IVP_DISABLE);
	else
		ivp_dev_set_dynamic_clk(pdev, IVP_ENABLE);

	ret = ivp_poweron_remap(pdev);
	if (ret) {
		ivp_err("power on remap setting failed [%d]", ret);
		goto err_ivp_poweron_remap;
	}

	/* After reset, enter running mode */
	ivp_hw_set_ocdhalt_on_reset(pdev, 0);

	/* Put ivp in stall mode */
	ivp_hw_runstall(pdev, IVP_RUNSTALL_STALL);
	/* Reset ivp core */
	ivp_hw_enable_reset(pdev);

	/* Boot from IRAM. */
	ivp_hw_set_bootmode(pdev, IVP_BOOT_FROM_IRAM);

	/* Disable system reset, let ivp core leave from reset */
	ivp_hw_disable_reset(pdev);

#ifdef MULTIPLE_ALGO
	ret = ivp_algo_mem_init(pdev);
#endif
	return ret; /*lint !e454*/

err_ivp_poweron_remap:
	ivp_poweroff_pri(pdev);

err_ivp_poweron_pri:
	mutex_lock(&pdev->ivp_comm.ivp_wake_lock_mutex);
	if (pdev->ivp_comm.ivp_power_wakelock.active) {
		__pm_relax(&pdev->ivp_comm.ivp_power_wakelock);
		ivp_err("ivp power on failed, wake unlock");
	}
	mutex_unlock(&pdev->ivp_comm.ivp_wake_lock_mutex); /*lint !e456*/

	return ret; /*lint !e454*/
}

void ivp_dev_poweroff(struct ivp_device *pdev)
{
	int ret;
#ifdef MULTIPLE_ALGO
	int i;
#endif

	if (!pdev) {
		ivp_err("invalid input param pdev");
		return;
	}
	ret = ivp_poweroff_pri(pdev);
	if (ret)
		ivp_err("power on private setting failed:%d", ret);

	mutex_lock(&pdev->ivp_comm.ivp_wake_lock_mutex);
	if (pdev->ivp_comm.ivp_power_wakelock.active) {
		__pm_relax(&pdev->ivp_comm.ivp_power_wakelock);
		ivp_info("ivp power off, wake unlock");
	}

#ifdef MULTIPLE_ALGO
	pdev->core_status = (unsigned int)INVALID;
	dyn_core_loaded[pdev->core_id] = false;
	for (i = 0; i < IVP_ALGO_NODE_MAX; i++) {
		if (pdev->algo_mem_info[i].occupied == WORK) {
			if (pdev->algo_mem_info[i].algo_ddr_addr)
				ivp_free_algo_zone(pdev, i, SEGMENT_DDR_TEXT);
			if (pdev->algo_mem_info[i].algo_func_addr)
				ivp_free_algo_zone(pdev, i, SEGMENT_IRAM_TEXT);
			if (pdev->algo_mem_info[i].algo_data_addr)
				ivp_free_algo_zone(pdev, i, SEGMENT_DRAM0_DATA);
			if (pdev->algo_mem_info[i].algo_bss_addr)
				ivp_free_algo_zone(pdev, i, SEGMENT_DRAM0_BSS);
		}
		pdev->algo_mem_info[i].occupied = INVALID;
		init_algo_mem_info(pdev, i);
	}
	kfree(pdev->algo_mem_info);
	pdev->algo_mem_info = NULL;
#endif
	mutex_unlock(&pdev->ivp_comm.ivp_wake_lock_mutex); /*lint !e456*/
}

void ivp_poweroff(struct ivp_device *pdev)
{
	if (!pdev) {
		ivp_err("invalid input param pdev");
		return;
	}

	if (atomic_read(&pdev->ivp_comm.poweron_success) != 0) {
		ivp_err("maybe ivp dev not poweron success");
		return;
	}

	ivp_deinit_resethandler(pdev);

	ivp_hw_runstall(pdev, IVP_RUNSTALL_STALL);
	if (ivp_hw_query_runstall(pdev) != IVP_RUNSTALL_STALL)
		ivp_err("failed to stall ivp");
	ivp_hw_clr_wdg_irq(pdev);

	disable_irq(pdev->ivp_comm.wdg_irq);
	free_irq(pdev->ivp_comm.wdg_irq, pdev);

	if (pdev->ivp_comm.ivp_secmode == NOSEC_MODE) {
		disable_irq(pdev->ivp_comm.dwaxi_dlock_irq);
		free_irq(pdev->ivp_comm.dwaxi_dlock_irq, pdev);
		ivp_dev_smmu_deinit(pdev);
#ifdef SEC_IVP_V300
#ifdef SEC_IVP_ENABLE
	} else {
		ivp_clear_map_info();
		ivp_dev_smmu_deinit(pdev);
#endif
#endif
	}

	ivp_dev_poweroff(pdev);
#ifdef SEC_IVP_ENABLE
	if (pdev->ivp_comm.ivp_sec_support && pdev->ivp_comm.ivp_secmode) {
		if (ivp_destroy_secimage_thread(pdev))
			ivp_err("ivp_destroy_secimage_thread failed!");
	}
#endif

	pdev->ivp_comm.ivp_secmode = NOSEC_MODE;
	atomic_inc(&pdev->ivp_comm.poweron_access);
	atomic_inc(&pdev->ivp_comm.poweron_success);
}

static int ivp_poweron(
		struct ivp_device *pdev, const struct ivp_power_up_info *pu_info)
{
	int ret = -1;

	if (atomic_read(&pdev->ivp_comm.poweron_access) == 0) {
		ivp_err("maybe ivp dev has power on");
		return -EBUSY;
	}
	atomic_dec(&pdev->ivp_comm.poweron_access);
	atomic_set(&pdev->ivp_comm.wdg_sleep, 0);
	sema_init(&pdev->ivp_comm.wdg_sem, 0);

	if (pu_info->sec_mode == SECURE_MODE && pdev->ivp_comm.ivp_sec_support == 0) {
		ivp_err("ivp don't support secure mode");
		goto err_ivp_invalid_param;
	}
	pdev->ivp_comm.ivp_secmode = (unsigned int)pu_info->sec_mode;
	pdev->ivp_comm.ivp_sec_buff_fd = pu_info->sec_buff_fd;
#ifdef SEC_IVP_ENABLE
	if (pdev->ivp_comm.ivp_secmode) {
#ifdef SEC_IVP_V300
		ret = ivp_init_map_info();
		if (ret) {
			ivp_err("failed to init map info ret:%d", ret);
			goto ivp_create_sec_thread;
		}
#endif
		ret = ivp_create_secimage_thread(pdev);
		if (ret) {
			ivp_err("create sec ivp thread failed, ret:%d", ret);
			goto ivp_create_sec_thread;
		}
	}
#endif
	ret = ivp_dev_poweron(pdev);
	if (ret < 0) {
		ivp_err("failed to power on ivp");
		goto err_ivp_dev_poweron;
	}

	if (pdev->ivp_comm.ivp_secmode == NOSEC_MODE) {
		ret = ivp_dev_smmu_init(pdev);
		if (ret) {
			ivp_err("failed to init smmu");
			goto err_ivp_dev_smmu_init;
		}
		ret = request_irq(pdev->ivp_comm.dwaxi_dlock_irq, ivp_dwaxi_irq_handler,
			0, "ivp_dwaxi_irq", (void *)pdev);
		if (ret) {
			ivp_err("failed to request dwaxi irq.%d", ret);
			goto err_request_irq_dwaxi;
		}
#ifdef SEC_IVP_V300
#ifdef SEC_IVP_ENABLE
	} else {
		ret = ivp_dev_smmu_init(pdev);
		if (ret) {
			ivp_err("failed to init smmu");
			goto err_ivp_dev_smmu_init;
		}
#endif
#endif
	}

	ret = request_irq(pdev->ivp_comm.wdg_irq, ivp_wdg_irq_handler,
		0, "ivp_wdg_irq", (void *)pdev);
	if (ret) {
		ivp_err("failed to request wdg irq.%d", ret);
		goto err_request_irq_wdg;
	}

	ret = ivp_init_resethandler(pdev);
	if (ret) {
		ivp_err("failed to init reset handler");
		goto err_ivp_init_resethandler;
	}

#ifdef IVP_DUAL_CORE
	ivp_info("open ivp device success, core_id: %u", pdev->core_id);
#else
	ivp_info("open ivp device success");
#endif
	atomic_dec(&pdev->ivp_comm.poweron_success);

	return ret;

err_ivp_init_resethandler:
	free_irq(pdev->ivp_comm.wdg_irq, pdev);
err_request_irq_wdg:
	if (pdev->ivp_comm.ivp_secmode == NOSEC_MODE)
		free_irq(pdev->ivp_comm.dwaxi_dlock_irq, pdev);
err_request_irq_dwaxi:
	if (pdev->ivp_comm.ivp_secmode == NOSEC_MODE)
		ivp_dev_smmu_deinit(pdev);
err_ivp_dev_smmu_init:
	ivp_dev_poweroff(pdev);
err_ivp_dev_poweron:
#ifdef SEC_IVP_ENABLE
	ivp_destroy_secimage_thread(pdev);
ivp_create_sec_thread:
#endif
	pdev->ivp_comm.ivp_secmode = NOSEC_MODE;
err_ivp_invalid_param:
	atomic_inc(&pdev->ivp_comm.poweron_access);
	ivp_dsm_error_notify(DSM_IVP_OPEN_ERROR_NO);

	ivp_info("poweron ivp device fail");
	return ret;
}

static long ioctl_power_up(struct file *fd, unsigned long args)
{
	long ret = 0;
	struct ivp_power_up_info info = {0};
	struct ivp_device *pdev = NULL;
	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = (struct ivp_device *)fd->private_data;

	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	mutex_lock(&pdev->ivp_comm.ivp_power_up_off_mutex);
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		goto power_up_err;
	}

	if (info.sec_mode != SECURE_MODE && info.sec_mode != NOSEC_MODE) {
		ivp_err("invalid input secMode value:%d", info.sec_mode);
		goto power_up_err;
	}
	if (info.sec_mode == SECURE_MODE && info.sec_buff_fd < 0) {
		ivp_err("invalid sec buffer fd value:%d", info.sec_buff_fd);
		goto power_up_err;
	}

	ret = ivp_poweron(pdev, &info);
	mutex_unlock(&pdev->ivp_comm.ivp_power_up_off_mutex);

	return ret;

power_up_err:
	mutex_unlock(&pdev->ivp_comm.ivp_power_up_off_mutex);
	return -EINVAL;
}

static long ioctl_power_down(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = NULL;
	if (!fd || !fd->private_data) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = (struct ivp_device *)fd->private_data;
	mutex_lock(&pdev->ivp_comm.ivp_power_up_off_mutex);
#ifdef IVP_DUAL_CORE
	ivp_info("power down ivp %u", pdev->core_id);
#endif
	ivp_poweroff(pdev);
	mutex_unlock(&pdev->ivp_comm.ivp_power_up_off_mutex);
	return EOK;
}

static long ioctl_sect_count(struct file *fd, unsigned long args)
{
	long ret;
	struct ivp_device *pdev = NULL;
	unsigned int sect_count;
	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or odev");
		return -EINVAL;
	}
	sect_count = (unsigned int)pdev->ivp_comm.sect_count;

	ivp_info("get img sect num:%#x", sect_count);
	ret = copy_to_user((void *)(uintptr_t)args,
		&sect_count, sizeof(sect_count));

	return ret;
}

static long ioctl_hidl_map(struct file *fd, unsigned long args)
{
	struct ivp_map_info info;
	struct ivp_device *pdev = NULL;
	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}
	if (pdev->ivp_comm.ivp_secmode == SECURE_MODE) {
		return -EINVAL;
	}

	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	if (ivp_map_hidl_fd(&pdev->ivp_comm.ivp_pdev->dev, &info)) {
		ivp_err("ivp_map_hidl_fd fail");
		return -EPERM;
	}

	if (copy_to_user((void *)(uintptr_t)args, &info, sizeof(info))) {
		ivp_err("copy to user failed");
		return -ENOMEM;
	}

	return EOK;
}

static long ioctl_hidl_unmap(struct file *fd, unsigned long args)
{
	struct ivp_map_info info;
	struct ivp_device *pdev = NULL;
	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}
	if (pdev->ivp_comm.ivp_secmode == SECURE_MODE) {
		return -EINVAL;
	}

	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	if (ivp_unmap_hidl_fd(&pdev->ivp_comm.ivp_pdev->dev, &info)) {
		ivp_err("ivp_unmap_hidl_fd fail");
		return -EPERM;
	}

	if (copy_to_user((void *)(uintptr_t)args, &info, sizeof(info))) {
		ivp_err("copy to user failed");
		return -ENOMEM;
	}

	return EOK;
}

static long ioctl_fd_info(struct file *fd, unsigned long args)
{
#ifdef SEC_IVP_V300
	struct ivp_fd_info info;
	struct ivp_device *pdev = NULL;

	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}
	if (pdev->ivp_comm.ivp_secmode != SECURE_MODE) {
		return -EINVAL;
	}
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	if (info.mapped == 0) {
		if (ivp_init_sec_fd(&info)) {
			ivp_err("ivp init sec fd fail");
			return -ENOMEM;
		}
	} else {
		if (ivp_deinit_sec_fd(&info)) {
			ivp_err("ivp deinit sec fd fail");
			return -ENOMEM;
		}
	}

	if (copy_to_user((void *)(uintptr_t)args, &info, sizeof(info))) {
		ivp_err("copy to user failed");
		return -ENOMEM;
	}
#else
	if (!fd || !args) {
		ivp_err("invalid input args or fd");
		return -EINVAL;
	}
#endif

	return EOK;
}

static long ioctl_sect_info(struct file *fd, unsigned long args)
{
	long ret;
	struct ivp_sect_info info;
	struct ivp_device *pdev = NULL;
	unsigned int sect_count;
	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	sect_count = (unsigned int)pdev->ivp_comm.sect_count;
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	if (info.index >= sect_count) {
		ivp_err("index is out of range.index:%u, sec_count:%u",
			info.index, sect_count);
		return -EINVAL;
	}

	if ((pdev->ivp_comm.ivp_secmode == SECURE_MODE) ||
		(info.info_type == SECURE_MODE)) {
		ret = copy_to_user((void *)(uintptr_t)args,
			&pdev->ivp_comm.sec_sects[info.index],
			sizeof(struct ivp_sect_info));
		ivp_dbg("name:%s, ivp_addr:0x%x, acpu_addr:0x%lx",
			pdev->ivp_comm.sec_sects[info.index].name,
			pdev->ivp_comm.sec_sects[info.index].ivp_addr,
			pdev->ivp_comm.sec_sects[info.index].acpu_addr);
	} else {
		ret = copy_to_user((void *)(uintptr_t)args,
			&pdev->ivp_comm.sects[info.index],
			sizeof(struct ivp_sect_info));
	}

	return ret;
}

static int ivp_load_section(struct ivp_device *pdev,
		struct image_section_header image_sect, const struct firmware *fw)
{
	unsigned int iova = 0;
	unsigned int offset = 0;
	unsigned int size;
	unsigned int i;
	unsigned int *source = NULL;
	unsigned char type = 0;
	unsigned long ivp_ddr_addr = 0;
	unsigned int *mem_addr = NULL;
	void *mem = NULL;
	bool ddr_flag = false;
	errno_t ret;

	iova = image_sect.vaddr;
	size = (unsigned int)image_sect.size;

	source = (unsigned int *)(fw->data + image_sect.offset);
	type = image_sect.type;
	if ((image_sect.vaddr >= pdev->ivp_comm.sects[SECT_START_NUM].ivp_addr) &&
		(image_sect.vaddr <= (pdev->ivp_comm.sects[SECT_START_NUM].ivp_addr +
			pdev->ivp_comm.sects[SECT_START_NUM].len))) {
		ddr_flag = true;
	} else {
		ddr_flag = false;
	}
	switch (type) {
	case IMAGE_SECTION_TYPE_EXEC:
	case IMAGE_SECTION_TYPE_DATA: {
		if (ddr_flag == true) {
			ivp_ddr_addr = (pdev->ivp_comm.sects[SECT_START_NUM].acpu_addr <<
				IVP_MMAP_SHIFT) + iova -
				pdev->ivp_comm.sects[SECT_START_NUM].ivp_addr;
			mem = ivp_vmap(ivp_ddr_addr, image_sect.size, &offset);
		} else {
			mem = ioremap_nocache(image_sect.vaddr,
					image_sect.size);
		}

		if (!mem) {
			ivp_err("can't map base address");
			return -EINVAL;
		}
		mem_addr = mem;
		if (ddr_flag == true) {
			ret = memcpy_s(mem_addr, image_sect.size,
					source, image_sect.size);
			if (ret != EOK) {
				ivp_err("memcpy_s fail, ret [%d]", ret);
				vunmap(mem - offset);
				return -EINVAL;
			}
		} else {
			for (i = 0; i < image_sect.size / 4; i++) /* 4 is sizeof xt-int */
				*(mem_addr + i) = *(source + i);
		}
	}
	break;
	case IMAGE_SECTION_TYPE_BSS:
	break;
	default: {
		ivp_err("unsupported section type %d", type);
		return -EINVAL;
	}
	}
	if (mem) {
		if (ddr_flag == true)
			vunmap(mem - offset);
		else
			iounmap(mem);
	}
	return 0;
}

static int ivp_get_validate_section_info(struct ivp_device *pdev,
		const struct firmware *fw,
		struct image_section_header *psect_header,
		unsigned int index)
{
	int offset;
	errno_t ret;

	if (!psect_header) {
		ivp_err("input para is invalid");
		return -EINVAL;
	}
	offset = sizeof(struct file_header) + sizeof(*psect_header) * index;
	if ((offset + sizeof(*psect_header)) > fw->size) {
		ivp_err("image index is err");
		return -EINVAL;
	}
	ret = memcpy_s(psect_header, sizeof(*psect_header),
		fw->data + offset, sizeof(*psect_header));
	if (ret != EOK) {
		ivp_err("memcpy_s fail, ret [%d]", ret);
		return -EINVAL;
	}

	if ((psect_header->offset + psect_header->size) > fw->size) {
		ivp_err("get invalid offset 0x%x", psect_header->offset);
		return -EINVAL;
	}
#ifdef IVP_DUAL_CORE
	if ((psect_header->vaddr < pdev->ivp_comm.sects[SECT_START_NUM].ivp_addr) ||
		(psect_header->vaddr > (pdev->ivp_comm.sects[SECT_START_NUM].ivp_addr +
			pdev->ivp_comm.sects[SECT_START_NUM].len))) {
		psect_header->vaddr += pdev->base_offset;
	}
#endif
	/* 0,1,2,3 represent array index */
	if (((psect_header->vaddr >= pdev->ivp_comm.sects[0].ivp_addr) &&
		(psect_header->vaddr < (pdev->ivp_comm.sects[0].ivp_addr +
			pdev->ivp_comm.sects[0].len))) ||
		((psect_header->vaddr >= pdev->ivp_comm.sects[1].ivp_addr) &&
		(psect_header->vaddr < (pdev->ivp_comm.sects[1].ivp_addr +
			pdev->ivp_comm.sects[1].len))) ||
		((psect_header->vaddr >= pdev->ivp_comm.sects[2].ivp_addr) &&
		(psect_header->vaddr < (pdev->ivp_comm.sects[2].ivp_addr +
			pdev->ivp_comm.sects[2].len))) ||
		((psect_header->vaddr >= pdev->ivp_comm.sects[3].ivp_addr) &&
		(psect_header->vaddr < (pdev->ivp_comm.sects[3].ivp_addr +
			pdev->ivp_comm.sects[3].len)))) {
		return EOK;
	}

	ivp_err("get invalid addr");
	return -EINVAL;
}

static int ivp_load_firmware(struct ivp_device *pdev,
		const char *filename, const struct firmware *fw)
{
	unsigned int idx;
	struct file_header mheader;
	errno_t ret;

	ret = memcpy_s(&mheader, sizeof(mheader), fw->data, sizeof(mheader));
	if (ret != EOK) {
		ivp_err("memcpy_s fail, ret [%d]", ret);
		return -EINVAL;
	}
	ivp_info("start loading image %s, section counts 0x%x",
		filename, mheader.sect_count);
	for (idx = 0; idx < mheader.sect_count; idx++) {
		struct image_section_header sect;

		if (ivp_get_validate_section_info(pdev, fw, &sect, idx)) {
			ivp_err("get section %d fails", idx);
			return -EINVAL;
		}
		if (ivp_load_section(pdev, sect, fw)) {
			ivp_err("load section %d fails", idx);
			return -EINVAL;
		}
	}
	ivp_info("finish loading image %s", filename);
	return 0;
}

static int ivp_check_image(const struct firmware *fw)
{
	errno_t ret;
	struct file_header mheader;

	if (sizeof(mheader) > fw->size) {
		ivp_err("image file mheader is err");
		return -EINVAL;
	}
	ret = memcpy_s(&mheader, sizeof(mheader), fw->data, sizeof(mheader));
	if (ret != EOK) {
		ivp_err("memcpy_s fail, ret [%d]", ret);
		return -EINVAL;
	}

	if (strncmp(mheader.name, "IVP:", 4) != 0) {
		ivp_err("image file header is not for IVP");
		return -EINVAL;
	}

	if (fw->size != mheader.image_size) {
		ivp_err("request_firmware size 0x%zx mheader size 0x%zx",
			fw->size, mheader.image_size);
		return -EINVAL;
	}
	return 0;
}

#ifdef MULTIPLE_ALGO
static int ivp_segment_type(const XT_Elf32_Phdr program_header)
{
	if (program_header.p_vaddr < IVP_ALGO_MAX_SIZE)
		return SEGMENT_DDR_TEXT;
	else if (program_header.p_vaddr == IVP_IRAM_TEXT_SEGMENT_ADDR)
		return SEGMENT_IRAM_TEXT;
	else if (program_header.p_vaddr == IVP_DRAM0_DATA_SEGMENT_ADDR)
		return SEGMENT_DRAM0_DATA;
	else if (program_header.p_vaddr == IVP_DRAM0_BSS_SEGMENT_ADDR)
		return SEGMENT_DRAM0_BSS;
	ivp_info("abnormal segment type");
	return -EINVAL;
}

static void *ivp_ioremap(const struct ivp_device *pdev,
		const XT_Elf32_Phdr program_header, const unsigned int algo_id,
		const int segment_type)
{
	if (segment_type == SEGMENT_IRAM_TEXT) {
		return ioremap_nocache(pdev->algo_mem_info[algo_id].algo_func_addr +
			pdev->base_offset, program_header.p_memsz);
	} else if (segment_type == SEGMENT_DRAM0_DATA) {
		return ioremap_nocache(pdev->algo_mem_info[algo_id].algo_data_addr +
			pdev->base_offset, program_header.p_memsz);
	} else if (segment_type == SEGMENT_DRAM0_BSS) {
		return ioremap_nocache(pdev->algo_mem_info[algo_id].algo_bss_addr +
			pdev->base_offset, program_header.p_memsz);
	}
	ivp_info("abnormal segment type");
	return NULL;
}

static int ivp_load_algo_segment(struct ivp_device *pdev,
		const struct firmware *fw, const XT_Elf32_Phdr program_header,
		const unsigned int algo_id)
{
	unsigned long ivp_ddr_addr = 0;
	void *mem = NULL;
	unsigned int offset = 0;
	unsigned int *mem_addr = NULL;
	unsigned int i;
	int segment_type = ivp_segment_type(program_header);
	unsigned int *source = (unsigned int *)(fw->data + program_header.p_offset);

	if (segment_type == SEGMENT_DDR_TEXT) {
		ivp_ddr_addr = pdev->algo_mem_info[algo_id].algo_ddr_addr -
			(pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr + SIZE_1M) +
			((pdev->ivp_comm.sects[BASE_SECT_INDEX].acpu_addr << IVP_MMAP_SHIFT) +
				pdev->ivp_comm.sects[BASE_SECT_INDEX].len);
		ivp_info("vaddr[%#x]  memsz[%x]", program_header.p_vaddr,
			program_header.p_memsz);
		mem = ivp_vmap(ivp_ddr_addr, program_header.p_memsz, &offset);
	} else {
		mem = ivp_ioremap(pdev, program_header, algo_id, segment_type);
	}

	if (!mem) {
		ivp_err("can't map base address");
		return -EINVAL;
	}
	mem_addr = mem;
	if (segment_type == SEGMENT_DDR_TEXT) {
		if (memcpy_s(mem_addr, program_header.p_memsz, source,
			program_header.p_memsz) != EOK) {
			ivp_err("memcpy_s fail");
			vunmap(mem - offset);
			return -EINVAL;
		}
	} else {
		/* 4 stand for byte wide*/
		for (i = 0; i < program_header.p_memsz / 4; i++)
			*(mem_addr + i) = *(source + i);
	}

	if (mem) {
		if (segment_type == SEGMENT_DDR_TEXT)
			vunmap(mem - offset);
		else
			iounmap(mem);
	}

	return 0;
}

static unsigned int ivp_get_algo_start(struct ivp_device *pdev,
		XT_Elf32_Ehdr elf_head, unsigned int algo_id)
{
	return elf_head.e_entry +
		pdev->algo_mem_info[algo_id].algo_ddr_addr -
		pdev->algo_mem_info[algo_id].algo_ddr_vaddr;
}

static int ivp_get_dyn_info(struct ivp_device *pdev,
		XT_Elf32_Phdr dynamic_header, const struct firmware *fw,
		unsigned int algo_id)
{
	errno_t ret;
	int index = 0;
	unsigned int offset;
	XT_Elf32_Dyn dyn_entry;

	do {
		offset = dynamic_header.p_offset + sizeof(XT_Elf32_Dyn) * index;
		ret = memcpy_s(&dyn_entry, sizeof(XT_Elf32_Dyn),
			fw->data + offset, sizeof(XT_Elf32_Dyn));
		if (ret != EOK) {
			ivp_err("memcpy_s dyn_entry failed");
			return -EINVAL;
		}
		switch ((Elf32_Word)dyn_entry.d_tag)
		{
		case DT_RELA:
			pdev->algo_mem_info[algo_id].algo_rel_addr =
				dyn_entry.d_un.d_ptr +
				pdev->algo_mem_info[algo_id].algo_ddr_addr -
				pdev->algo_mem_info[algo_id].algo_ddr_vaddr;
			break;
		case DT_RELASZ:
			pdev->algo_mem_info[algo_id].algo_rel_count =
				dyn_entry.d_un.d_val / sizeof(XT_Elf32_Rela);
			break;
		default:
			break;
		}
		index++;
	} while (dyn_entry.d_tag != DT_NULL);

	return 0;
}

/* get avaliable algo node and apply memory */
static int ivp_get_algo_node(struct ivp_device *pdev,
		const struct firmware *fw, XT_Elf32_Ehdr elf_head)
{
	int ret;
	unsigned int i;
	unsigned int j;
	int offset;

	for (i = 0; i < IVP_ALGO_NODE_MAX; i++) {
		if (pdev->algo_mem_info[i].occupied == FREE) {
			XT_Elf32_Phdr header;
			init_algo_mem_info(pdev, i);
			for (j = 0; j < elf_head.e_phnum; j++) {
				offset = elf_head.e_phoff + elf_head.e_phentsize * j;
				ret = memcpy_s(&header, sizeof(header),
					fw->data + offset, elf_head.e_phentsize);
				if (ret != EOK) {
					ivp_err("memcpy_s program header failed[%x]", ret);
					return -EINVAL;
				}
				if (header.p_type != PT_LOAD || header.p_memsz == 0) {
					continue; /* support dynamic header */
				}
				ret = ivp_alloc_algo_mem(pdev, header, i);
				if (ret != 0) {
					ivp_err("alloc algo memory fail[%x]", ret);
					return ret;
				}
			}
			ivp_info("find available algo node [%u][%u]",
				pdev->core_id, i);
			return i;
		}
	}
	ivp_err("fail found available algo node");
	for (i = 0; i < IVP_ALGO_NODE_MAX; i++)
		ivp_err("[%u][%u] status[%u]", pdev->core_id, i,
			pdev->algo_mem_info[i].occupied);

	return -EINVAL;
}

static int ivp_load_foreach_segment(struct ivp_device *pdev,
		const struct firmware *fw, const XT_Elf32_Ehdr elf_head,
		const int algo_id)
{
	errno_t ret;
	unsigned int i;
	XT_Elf32_Phdr program_header;
	int offset;

	for (i = 0; i < elf_head.e_phnum; i++) {
		offset = elf_head.e_phoff + elf_head.e_phentsize * i;
		ret = memcpy_s(&program_header, sizeof(program_header),
			fw->data + offset, elf_head.e_phentsize);
		if (ret != EOK) {
			ivp_err("memcpy_s program_header failed[%x]", ret);
			return -EINVAL;
		}
		if ((program_header.p_offset + program_header.p_memsz) > fw->size) {
			ivp_err("get invalid offset 0x%x", program_header.p_offset);
			return -EINVAL;
		}
		if (program_header.p_type == PT_LOAD && program_header.p_memsz > 0) {
			ret = ivp_load_algo_segment(pdev, fw, program_header, algo_id);
			if (ret) {
				ivp_err("ivp_load_algo_segment failed");
				return -EINVAL;
			}
		} else if (program_header.p_type == PT_DYNAMIC) {
			ivp_get_dyn_info(pdev, program_header, fw, algo_id);
		}
	}
	return EOK;
}

static int ivp_load_algo_firmware(struct ivp_device *pdev,
		const struct firmware *fw)
{
	int algo_id;
	errno_t ret;
	XT_Elf32_Ehdr elf_head;

	ivp_info("start load ivp algo");
	ret = memset_s((char *)&elf_head, sizeof(elf_head), 0, sizeof(elf_head));
	if (ret != EOK) {
		ivp_err("memset_s elf_head failed[%x]", ret);
		return ret;
	}
	ret = memcpy_s(&elf_head, sizeof(elf_head), fw->data, sizeof(elf_head));
	if (ret != EOK) {
		ivp_err("memcpy_s elf_head failed[%x]", ret);
		return ret;
	}

	algo_id = ivp_get_algo_node(pdev, fw, elf_head);
	if (algo_id < 0) {
		ivp_err("get algo node failed");
		return algo_id;
	}

	ret = ivp_load_foreach_segment(pdev, fw, elf_head, algo_id);
	if (ret != EOK) {
		ivp_err("load foreach segments failed");
		if (pdev->algo_mem_info[algo_id].algo_ddr_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DDR_TEXT);
		if (pdev->algo_mem_info[algo_id].algo_func_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_IRAM_TEXT);
		if (pdev->algo_mem_info[algo_id].algo_data_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_DATA);
		if (pdev->algo_mem_info[algo_id].algo_bss_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_BSS);
		return -EINVAL;
	}

	pdev->algo_mem_info[algo_id].algo_start_addr = ivp_get_algo_start(pdev,
		elf_head, algo_id);
	pdev->algo_mem_info[algo_id].occupied = WORK;
	ivp_info("set algo[%x] status work", algo_id);

	return algo_id;
}

static int ivp_load_algo_image(struct ivp_device *pdev, const char *name)
{
	unsigned int ret;
	const struct firmware *firmware = NULL;
	struct device *dev = NULL;

	if (!name) {
		ivp_err("ivp image file name is invalid in function");
		return -EINVAL;
	}
	ivp_info("load algo name[%s]", name);
	dev = pdev->ivp_comm.device.this_device;
	if (!dev) {
		ivp_err("ivp miscdevice element struce device is null");
		return -EINVAL;
	}
	ret = request_firmware(&firmware, name, dev);
	if (ret) {
		ivp_err("request_firmware return error value %d for file (%s)",
			ret, name);
		return ret;
	}

	ret = ivp_load_algo_firmware(pdev, firmware);
	release_firmware(firmware);
	return ret;
}

static long ioctl_algo_info(struct file *fd, unsigned long args)
{
	long ret;
	struct ivp_algo_info info;
	struct ivp_device *pdev = NULL;

	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}
	ivp_info("current running core_id: %u", pdev->core_id);
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	if (info.algo_id >= IVP_ALGO_NODE_MAX) {
		ivp_err("invalid algo id %d", info.algo_id);
		return -EINVAL;
	}

	info.algo_start = pdev->algo_mem_info[info.algo_id].algo_start_addr;
	info.rel_addr = pdev->algo_mem_info[info.algo_id].algo_rel_addr;
	info.rel_count = pdev->algo_mem_info[info.algo_id].algo_rel_count;
	info.ddr_addr = pdev->algo_mem_info[info.algo_id].algo_ddr_addr;
	info.ddr_vaddr = pdev->algo_mem_info[info.algo_id].algo_ddr_vaddr;
	info.ddr_size = pdev->algo_mem_info[info.algo_id].algo_ddr_size;
	info.func_addr = pdev->algo_mem_info[info.algo_id].algo_func_addr;
	info.func_vaddr = pdev->algo_mem_info[info.algo_id].algo_func_vaddr;
	info.func_size = pdev->algo_mem_info[info.algo_id].algo_func_size;
	info.data_addr = pdev->algo_mem_info[info.algo_id].algo_data_addr;
	info.data_vaddr = pdev->algo_mem_info[info.algo_id].algo_data_vaddr;
	info.data_size = pdev->algo_mem_info[info.algo_id].algo_data_size;
	info.bss_addr = pdev->algo_mem_info[info.algo_id].algo_bss_addr;
	info.bss_vaddr = pdev->algo_mem_info[info.algo_id].algo_bss_vaddr;
	info.bss_size = pdev->algo_mem_info[info.algo_id].algo_bss_size;

	ret = copy_to_user((void *)(uintptr_t)args,
		&info, sizeof(struct ivp_algo_info));
	return ret;
}

static long ioctl_unload_algo(struct file *fd, unsigned long args)
{
	unsigned int algo_id;
	struct ivp_device *pdev = NULL;

	if (!fd || !args || !fd->private_data) {
		ivp_err("invalid input para");
		return -EINVAL;
	}

	pdev = fd->private_data;
	if ((pdev->core_id != IVP_CORE0_ID) &&
		(pdev->core_id != IVP_CORE1_ID)) {
		ivp_err("invalid input core id");
		return -EINVAL;
	}
	if (copy_from_user(&algo_id, (void *)(uintptr_t)args,
			sizeof(unsigned int)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}
	if (pdev->core_status != WORK ||
		algo_id >= IVP_ALGO_NODE_MAX) {
		ivp_err("invalid core[%x] algo[%x]",
			pdev->core_status, algo_id);
		return -EINVAL;
	}

	if (pdev->algo_mem_info[algo_id].occupied == WORK) {
		pdev->algo_mem_info[algo_id].occupied = FREE;
		if (pdev->algo_mem_info[algo_id].algo_ddr_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DDR_TEXT);
		if (pdev->algo_mem_info[algo_id].algo_func_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_IRAM_TEXT);
		if (pdev->algo_mem_info[algo_id].algo_data_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_DATA);
		if (pdev->algo_mem_info[algo_id].algo_bss_addr)
			ivp_free_algo_zone(pdev, algo_id, SEGMENT_DRAM0_BSS);

		init_algo_mem_info(pdev, algo_id);
		ivp_info("finish algo[%u][%u] unload", pdev->core_id, algo_id);
		return 0;
	}
	ivp_err("algo[%u][%u] is not work status", pdev->core_id, algo_id);
	return -EINVAL;
}

static long ioctl_load_algo(struct file *fd, unsigned long args)
{
	struct ivp_image_info info;
	struct ivp_device *pdev = NULL;

	if (!fd || !args || !fd->private_data) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}

	pdev = fd->private_data;
	mutex_lock(&pdev->ivp_comm.ivp_load_image_mutex);
	if (memset_s((char *)&info, sizeof(info), 0, sizeof(info)) != EOK) {
		ivp_err("memset_s fail");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if (copy_from_user(&info, (void __user *)(uintptr_t)args,
		sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	info.name[sizeof(info.name) - 1] = '\0';
	if ((info.length > (sizeof(info.name) - 1)) ||
		info.length != strlen(info.name)) {
		ivp_err("image file:but pass param length:%u", info.length);
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if ((pdev->core_id != IVP_CORE0_ID) &&
		(pdev->core_id != IVP_CORE1_ID)) {
		ivp_err("invalid input core id");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if (pdev->core_status != WORK) {
		ivp_err("invalid core status");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	info.algo_id = ivp_load_algo_image(pdev, info.name);
	mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);

	if (info.algo_id < 0 || info.algo_id >= IVP_ALGO_NODE_MAX) {
		ivp_err("invalid algo id %d", info.algo_id);
		return -EINVAL;
	}

	if (copy_to_user((void *)(uintptr_t)args, &info, sizeof(info))) {
		ivp_err("copy to user failed");
		return -ENOMEM;
	}

	ivp_info("finish algo [%u][%d] load", pdev->core_id, info.algo_id);
	return EOK;
}

static int ivp_core_loaded_check(const struct ivp_device *pdev)
{
	/* two cores use same ddr-zone for text sections */
	if ((dyn_core_loaded[IVP_CORE0_ID] &&
			pdev->core_id == IVP_CORE1_ID) ||
			(dyn_core_loaded[IVP_CORE1_ID] &&
			pdev->core_id == IVP_CORE0_ID)) {
		ivp_info("core rodata sections had loaded");
		return 0;
	}
	return -1;
}

static void *ivp_ddr_vmap(struct ivp_device *pdev,
		XT_Elf32_Phdr program_header, unsigned int *offset)
{
	unsigned long ivp_ddr_addr;
	ivp_ddr_addr = (pdev->ivp_comm.sects[BASE_SECT_INDEX].acpu_addr <<
		IVP_MMAP_SHIFT) + program_header.p_vaddr -
		pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr;
	ivp_info("vaddr[%#x]  memsz[%x]", program_header.p_vaddr,
		program_header.p_memsz);
	return ivp_vmap(ivp_ddr_addr, program_header.p_memsz, offset);
}

static int ivp_load_core_segment(struct ivp_device *pdev,
		const struct firmware *fw, const XT_Elf32_Phdr program_header)
{
	unsigned int *mem_addr = NULL;
	void *mem = NULL;
	bool ddr_flag = false;
	unsigned int offset = 0;
	unsigned int i;
	unsigned int *source = (unsigned int *)(fw->data + program_header.p_offset);

	if (program_header.p_vaddr >= pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr &&
		(program_header.p_vaddr <= (pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr +
		pdev->ivp_comm.sects[BASE_SECT_INDEX].len)))
		ddr_flag = true;

	if (ddr_flag) {
		if (!ivp_core_loaded_check(pdev)) {
			return 0;
		}
		mem = ivp_ddr_vmap(pdev, program_header, &offset);
	} else {
		mem = ioremap_nocache(program_header.p_vaddr +
			pdev->base_offset, program_header.p_memsz);
		ivp_info("load xtensa memory at offset %#x", program_header.p_vaddr +
			pdev->base_offset);
	}

	if (!mem) {
		ivp_err("can't map base address");
		return -EINVAL;
	}
	mem_addr = mem;
	if (ddr_flag) {
		if (memcpy_s(mem_addr, program_header.p_memsz, source,
			program_header.p_memsz) != EOK) {
			ivp_err("memcpy_s fail");
			vunmap(mem - offset);
			return -EINVAL;
		}
	} else {
		/* 4 stand for byte wide*/
		for (i = 0; i < program_header.p_memsz / 4; i++)
			*(mem_addr + i) = *(source + i);
	}

	if (mem) {
		if (ddr_flag)
			vunmap(mem - offset);
		else
			iounmap(mem);
	}

	return 0;
}

static int ivp_get_core_segment(struct ivp_device *pdev,
		const struct firmware *fw, XT_Elf32_Phdr program_header)
{
	unsigned int p_vaddr = program_header.p_vaddr + pdev->base_offset;

	if (((program_header.p_offset + program_header.p_memsz) > fw->size) ||
		program_header.p_type != PT_LOAD || program_header.p_memsz == 0) {
		ivp_err("program_header is abnormal");
		return -EINVAL;
	}
	if ((p_vaddr >= pdev->ivp_comm.sects[DRAM0_SECT_INDEX].ivp_addr &&
			(p_vaddr < (pdev->ivp_comm.sects[DRAM0_SECT_INDEX].ivp_addr +
			pdev->ivp_comm.sects[DRAM0_SECT_INDEX].len))) ||
		(p_vaddr >= pdev->ivp_comm.sects[DRAM1_SECT_INDEX].ivp_addr &&
			(p_vaddr < (pdev->ivp_comm.sects[DRAM1_SECT_INDEX].ivp_addr +
			pdev->ivp_comm.sects[DRAM1_SECT_INDEX].len))) ||
		(p_vaddr >= pdev->ivp_comm.sects[IRAM_SECT_INDEX].ivp_addr &&
			(p_vaddr < (pdev->ivp_comm.sects[IRAM_SECT_INDEX].ivp_addr +
			pdev->ivp_comm.sects[IRAM_SECT_INDEX].len))) ||
		(program_header.p_vaddr >= pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr &&
			(program_header.p_vaddr <
			(pdev->ivp_comm.sects[BASE_SECT_INDEX].ivp_addr +
			pdev->ivp_comm.sects[BASE_SECT_INDEX].len))))
		return EOK;

	ivp_err("get invalid addr");
	return -EINVAL;
}

static int ivp_load_core_firmware(struct ivp_device *pdev,
		const struct firmware *fw)
{
	errno_t ret;
	unsigned int i;
	XT_Elf32_Ehdr elf_head;

	ivp_info("start analyze ivp core[%u]", pdev->core_id);
	ret = memset_s((char *)&elf_head, sizeof(elf_head), 0, sizeof(elf_head));
	if (ret != EOK) {
		ivp_err("memset_s failed %d", ret);
		return ret;
	}
	ret = memcpy_s(&elf_head, sizeof(elf_head), fw->data, sizeof(elf_head));
	if (ret != EOK) {
		ivp_err("memcpy_s failed %d", ret);
		return ret;
	}

	/* Standard ELF head magic is :0x7f E L F */
	if (elf_head.e_ident[ELF_MAGIC0] != 0x7F ||
		elf_head.e_ident[ELF_MAGIC1] != 'E' ||
		elf_head.e_ident[ELF_MAGIC2] != 'L' ||
		elf_head.e_ident[ELF_MAGIC3] != 'F') {
		ivp_err("check ivp core elf head fail");
		return -EINVAL;
	}

	for (i = 0; i < elf_head.e_phnum; i++) {
		XT_Elf32_Phdr program_header;
		int offset = elf_head.e_phoff + elf_head.e_phentsize * i;
		ret = memcpy_s(&program_header, sizeof(program_header),
			fw->data + offset, elf_head.e_phentsize);
		if (ret != EOK) {
			ivp_err("memcpy_s failed %d", ret);
			return -EINVAL;
		}
		if (program_header.p_memsz == 0) {
			continue;
		}

		if (ivp_get_core_segment(pdev, fw, program_header) != EOK) {
			ivp_err("get segment %u fails", i);
			return -EINVAL;
		}
		ret = ivp_load_core_segment(pdev, fw, program_header);
		if (ret) {
			ivp_err("load core segment failed");
			return ret;
		}
	}
	return EOK;
}

static int ivp_load_core_image(struct ivp_device *pdev,
		struct ivp_image_info info)
{
	int ret;
	const struct firmware *firmware = NULL;
	struct device *dev = NULL;
	dev = pdev->ivp_comm.device.this_device;
	if (!dev) {
		ivp_err("ivp miscdevice element struce device is null");
		return -EINVAL;
	}
	ret = request_firmware(&firmware, info.name, dev);
	if (ret) {
		ivp_err("request_firmware return error value %d for file (%s)",
			ret, info.name);
		return ret;
	}
	ret = ivp_load_core_firmware(pdev, firmware);
	release_firmware(firmware);
	return ret;
}

static long ioctl_load_core(struct file *fd, unsigned long args)
{
	long ret;
	errno_t rc;
	struct ivp_image_info info;
	struct ivp_device *pdev = NULL;

	if (!fd || !args || !fd->private_data) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if ((pdev->core_id != IVP_CORE0_ID) &&
		(pdev->core_id != IVP_CORE1_ID)) {
		ivp_err("invalid input core id");
		return -EINVAL;
	}
	if (pdev->core_status != FREE) {
		ivp_err("invalid core status %u", pdev->core_status);
		return -EINVAL;
	}
	if (ivp_hw_query_runstall(pdev) == IVP_RUNSTALL_RUN) {
		ivp_err("invalid ivp status:ivp alredy run");
		return -EINVAL;
	}

	mutex_lock(&pdev->ivp_comm.ivp_load_image_mutex);
	rc = memset_s((char *)&info, sizeof(info), 0, sizeof(info));
	if (rc != EOK) {
		ivp_err("memset_s fail, rc:%d", rc);
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return rc;
	}
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	info.name[sizeof(info.name) - 1] = '\0';
	if (strcmp((const char *)&info.name, IVP_DYNAMIC_CORE_NAME)) {
		ivp_err("image name is not %s", info.name);
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	ret = ivp_load_core_image(pdev, info);
	if (ret == EOK) {
		pdev->core_status = WORK;
		dyn_core_loaded[pdev->core_id] = true;
		ivp_info("load core[%u] success", pdev->core_id);
	}
	mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
	return ret;
}
#endif

#ifdef IVP_DUAL_CORE
static long ioctl_available_core(struct file *fd, unsigned long args)
{
	long ret;
	struct ivp_device *pdev = NULL;
	unsigned int buffer[EFUSE_IVP_BUFFER_NUM] = {0};
	struct device_node *np = NULL;
	const char *dts_str = NULL;

	if (!fd) {
		ivp_err("invalid input fd");
		return -EINVAL;
	}
	pdev = fd->private_data;
	if (!args || !pdev) {
		ivp_err("invalid input args or odev");
		return -EINVAL;
	}

	if (pdev->available_core == 0) {
		np = of_find_compatible_node(NULL, NULL, "hisilicon, soc_spec");
		if (!np) {
			ivp_info("no soc_spec node!");
			pdev->available_core = EFUSE_IVP_ALL_AVAILABLE;
			goto copy_back_to_user;
		}
		if (of_property_read_string(np, "soc_spec_set", &dts_str)) {
			ivp_info("no soc_spec_set!");
			of_node_put(np);
			pdev->available_core = EFUSE_IVP_ALL_AVAILABLE;
			goto copy_back_to_user;
		}
		of_node_put(np);
		ivp_info("soc_spec_set is %s", dts_str);

		if (!strcmp(dts_str, "normal")) {
			pdev->available_core = EFUSE_IVP_ALL_AVAILABLE;
		} else if (!strcmp(dts_str, "lite-normal") || !strcmp(dts_str, "wifi-only-normal")) {
			pdev->available_core = 0x01;
		} else if (!strcmp(dts_str, "lite") || !strcmp(dts_str, "wifi-only")) {
			/* from efuse bit 2276~2371 buffer[0]:bit31~2307 buffer[1]:bit0~2308 */
			ret = hisi_efuse_read_value(buffer, EFUSE_IVP_BUFFER_NUM,
				EFUSE_FN_RD_PARTIAL_PASSP2);
			if (ret != EOK) {
				ivp_err("hisi_efuse_read_value fail, ret:%lx", ret);
				pdev->available_core = EFUSE_IVP_ALL_AVAILABLE;
				goto copy_back_to_user;
			}
			pdev->available_core |=
				(buffer[EFUSE_BUF_POS(EFUSE_IVP_CORE0_POS)] >>
				EFUSE_BIT_POS(EFUSE_IVP_CORE0_POS)) & 0x01;
			if (pdev->available_core == 0) {
				pdev->available_core |=
					((buffer[EFUSE_BUF_POS(EFUSE_IVP_CORE1_POS)] >>
					EFUSE_BIT_POS(EFUSE_IVP_CORE1_POS)) & 0x01) << 1;
			}
		} else {
			pdev->available_core = 0;
		}
	}

copy_back_to_user:
	/* bit 0: core0, bit1: core1; 0 fail, 1 pass */
	ivp_info("get available_core:%#x", pdev->available_core);
	ret = copy_to_user((void *)(uintptr_t)args,
		&pdev->available_core, sizeof(pdev->available_core));

	return ret;
}
#endif

static int ivp_load_image(struct ivp_device *pdev, const char *name)
{
	int ret;
	const struct firmware *firmware = NULL;
	struct device *dev = NULL;

	if (!name) {
		ivp_err("ivp image file is invalid");
		return -EINVAL;
	}

	dev = pdev->ivp_comm.device.this_device;
	if (!dev) {
		ivp_err("ivp miscdevice is invalid");
		return -EINVAL;
	}
	ret = request_firmware(&firmware, name, dev);
	if (ret) {
		ivp_err("return error:%d for file:%s", ret, name);
		return ret;
	}

	ret = ivp_check_image(firmware);
	if (ret != 0) {
		release_firmware(firmware);
		ivp_err("check ivp image %s fail value 0x%x ", name, ret);
		return ret;
	}

	ret = ivp_load_firmware(pdev, name, firmware);
	release_firmware(firmware);
	return ret;
}

static long ioctl_load_firmware(struct file *fd, unsigned long args)
{
	long ret;
	struct ivp_image_info info;
	errno_t rc;
	struct ivp_device *pdev = fd->private_data;

	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	if (ivp_hw_query_runstall(pdev) == IVP_RUNSTALL_RUN) {
		ivp_err("invalid ivp status:ivp alredy run");
		return -EINVAL;
	}
	mutex_lock(&pdev->ivp_comm.ivp_load_image_mutex);
	rc = memset_s((char *)&info, sizeof(info), 0, sizeof(info));
	if (rc != EOK) {
		ivp_err("memcpy_s fail, rc:%d", rc);
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if (copy_from_user(&info, (void *)(uintptr_t)args, sizeof(info)) != 0) {
		ivp_err("invalid input param size");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	info.name[sizeof(info.name) - 1] = '\0';
	if ((info.length > (sizeof(info.name) - 1)) ||
		info.length <= IVP_IMAGE_SUFFIX_LENGTH ||
		info.length != strlen(info.name)) {
		ivp_err("image file:but pass param length:%d", info.length);
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if (strcmp((const char *)&info.name[info.length - IVP_IMAGE_SUFFIX_LENGTH],
		IVP_IMAGE_SUFFIX)) {
		ivp_err("image is not bin file");
		mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
		return -EINVAL;
	}
	if (pdev->ivp_comm.ivp_secmode == SECURE_MODE)
		ret = ivp_sec_loadimage(pdev);
	else
		ret = ivp_load_image(pdev, info.name);

	mutex_unlock(&pdev->ivp_comm.ivp_load_image_mutex);
	return ret;
}

static long ioctl_dsp_run(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}
	if (ivp_check_resethandler(pdev) == 1)
		ivp_dev_run(pdev);
	else
		ivp_err("ivp image not upload.");

	return EOK;
}

static long ioctl_dsp_suspend(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	long ret;
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ret = ivp_dev_suspend(pdev);

	return ret;
}

static long ioctl_dsp_resume(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	if (ivp_check_resethandler(pdev) == 1) {
		ivp_dev_resume(pdev);
		return EOK;
	} else {
		ivp_err("ivp image not upload");
		return -ENODEV;
	}
}

static long ioctl_dsp_stop(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ivp_dev_stop(pdev);

	return EOK;
}

static long ioctl_query_runstall(struct file *fd, unsigned long args)
{
	long ret;
	unsigned int runstall = 0;
	struct ivp_device *pdev = fd->private_data;

	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	runstall = (u32)ivp_hw_query_runstall(pdev);
	ret = copy_to_user((void *)(uintptr_t)args,
			&runstall, sizeof(runstall));

	return ret;
}

static long ioctl_query_waiti(struct file *fd, unsigned long args)
{
	long ret;
	unsigned int waiti;
	struct ivp_device *pdev = fd->private_data;

	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	waiti = (u32)ivp_hw_query_waitmode(pdev);
	ret = copy_to_user((void *)(uintptr_t)args, &waiti, sizeof(waiti));

	return ret;
}

static long ioctl_trigger_nmi(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ivp_hw_trigger_NMI(pdev);

	return EOK;
}

static long ioctl_watchdog(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	long ret;
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ret = ivp_dev_keep_on_wdg(pdev);

	return ret;
}

static long ioctl_watchdog_sleep(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ivp_dev_sleep_wdg(pdev);

	return EOK;
}

static long ioctl_smmu_invalidate_tlb(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	return 0;
}

static long ioctl_bm_init(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ivp_dev_hwa_enable(pdev);
	return EOK;
}

static long ioctl_clk_level(struct file *fd, unsigned long args)
{
	unsigned int level = 0;
	struct ivp_device *pdev = fd->private_data;

	if (!args || !pdev) {
		ivp_err("invalid input args or pdev");
		return -EINVAL;
	}

	if (copy_from_user(&level, (void *)(uintptr_t)args,
				sizeof(unsigned int)) != 0) {
		ivp_err("invalid input param size");
		return -EINVAL;
	}

	ivp_change_clk(pdev, level);
	return EOK;
}

static long ioctl_dump_dsp_status(struct file *fd,
		unsigned long args __attribute__((unused)))
{
	struct ivp_device *pdev = fd->private_data;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	ivp_dump_status(pdev);

	return EOK;
}


typedef long (*ivp_ioctl_func_t)(struct file *fd, unsigned long args);
struct ivp_ioctl_ops {
	unsigned int cmd;
	ivp_ioctl_func_t func;
};

#define IVP_IOCTL_OPS(_cmd, _func) {.cmd = _cmd, .func = _func}

/* ioctl_normal_ops do not depend on ivp powered up */
static struct ivp_ioctl_ops ioctl_normal_ops[] = {
	IVP_IOCTL_OPS(IVP_IOCTL_SECTINFO, ioctl_sect_info),
	IVP_IOCTL_OPS(IVP_IOCTL_SECTCOUNT, ioctl_sect_count),
	IVP_IOCTL_OPS(IVP_IOCTL_POWER_UP, ioctl_power_up),
	IVP_IOCTL_OPS(IVP_IOCTL_HIDL_MAP, ioctl_hidl_map),
	IVP_IOCTL_OPS(IVP_IOCTL_HIDL_UNMAP, ioctl_hidl_unmap),
#ifdef IVP_DUAL_CORE
	IVP_IOCTL_OPS(IVP_IOCTL_AVAILABLE_CORE, ioctl_available_core),
#endif
};

/* ioctl_actived_ops must be executed after powered up */
static struct ivp_ioctl_ops ioctl_actived_ops[] = {
	IVP_IOCTL_OPS(IVP_IOCTL_POWER_DOWN, ioctl_power_down),
	IVP_IOCTL_OPS(IVP_IOCTL_LOAD_FIRMWARE, ioctl_load_firmware),
#ifdef MULTIPLE_ALGO
	IVP_IOCTL_OPS(IVP_IOCTL_LOAD_CORE, ioctl_load_core),
	IVP_IOCTL_OPS(IVP_IOCTL_LOAD_ALGO, ioctl_load_algo),
	IVP_IOCTL_OPS(IVP_IOCTL_UNLOAD_ALGO, ioctl_unload_algo),
	IVP_IOCTL_OPS(IVP_IOCTL_ALGOINFO, ioctl_algo_info),
#endif
	IVP_IOCTL_OPS(IVP_IOCTL_DSP_RUN, ioctl_dsp_run),
	IVP_IOCTL_OPS(IVP_IOCTL_DSP_SUSPEND, ioctl_dsp_suspend),
	IVP_IOCTL_OPS(IVP_IOCTL_DSP_RESUME, ioctl_dsp_resume),
	IVP_IOCTL_OPS(IVP_IOCTL_DSP_STOP, ioctl_dsp_stop),
	IVP_IOCTL_OPS(IVP_IOCTL_QUERY_RUNSTALL, ioctl_query_runstall),
	IVP_IOCTL_OPS(IVP_IOCTL_QUERY_WAITI, ioctl_query_waiti),
	IVP_IOCTL_OPS(IVP_IOCTL_TRIGGER_NMI, ioctl_trigger_nmi),
	IVP_IOCTL_OPS(IVP_IOCTL_WATCHDOG, ioctl_watchdog),
	IVP_IOCTL_OPS(IVP_IOCTL_WATCHDOG_SLEEP, ioctl_watchdog_sleep),
	IVP_IOCTL_OPS(IVP_IOCTL_BM_INIT, ioctl_bm_init),
	IVP_IOCTL_OPS(IVP_IOCTL_CLK_LEVEL, ioctl_clk_level),
	IVP_IOCTL_OPS(IVP_IOCTL_DUMP_DSP_STATUS, ioctl_dump_dsp_status),
	IVP_IOCTL_OPS(IVP_IOCTL_SMMU_INVALIDATE_TLB, ioctl_smmu_invalidate_tlb),
	IVP_IOCTL_OPS(IVP_IOCTL_FDINFO, ioctl_fd_info),
};

static ivp_ioctl_func_t ivp_get_ioctl_func(unsigned int cmd,
		struct ivp_ioctl_ops *ops_tbl, uint32_t tbl_size)
{
	uint32_t idx;
	for (idx = 0; idx < tbl_size; idx++) {
		if (cmd == ops_tbl[idx].cmd) {
			return ops_tbl[idx].func;
		}
	}
	return NULL;
}

long ivp_ioctl(struct file *fd, unsigned int cmd, unsigned long args)
{
	ivp_ioctl_func_t func = NULL;
	struct ivp_device *pdev = NULL;
	if (!fd) {
		ivp_err("invalid input param fd");
		return -EINVAL;
	}
	ivp_info("received ioctl command(0x%08x)", cmd);

	func = ivp_get_ioctl_func(cmd, ioctl_normal_ops,
		ARRAY_SIZE(ioctl_normal_ops));
	if (func) {
		return func(fd, args);
	}
	pdev = fd->private_data;
	if (!pdev) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}
	if (atomic_read(&pdev->ivp_comm.poweron_success) != 0) {
		ivp_err("ioctl cmd is error %u since ivp not power", cmd);
		return -EINVAL;
	}
	func = ivp_get_ioctl_func(cmd, ioctl_actived_ops,
		ARRAY_SIZE(ioctl_actived_ops));
	if (func) {
		return func(fd, args);
	}

	ivp_err("invalid ioctl command(0x%08x) received", cmd);
	return -EINVAL;
}
