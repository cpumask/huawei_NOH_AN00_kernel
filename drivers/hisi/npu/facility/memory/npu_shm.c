/*
 * npu_shm.c
 *
 * about npu shm
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "npu_shm.h"

#include <linux/idr.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <asm/uaccess.h>
#include <linux/gfp.h>
#include <asm/tlbflush.h>

#include "hisi_svm.h"
#include "npu_cma.h"

struct devdrv_mem_desc *g_sq_desc = NULL;
struct devdrv_mem_info g_shm_desc[NPU_DEV_NUM][DEVDRV_MAX_MEM];

u8 *devdrv_calc_sq_slot(u8 dev_id, u32 index)
{
	u64 addr = g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr;
	u8 *sq = (u8 *)(uintptr_t)(addr + index * DEVDRV_SQ_SLOT_SIZE);

	NPU_DRV_DEBUG("sq_info base vaddr = 0x%llx, id= %u, addr= %p\n", addr, index, sq);
	return sq;
}

u8 *devdrv_calc_cq_slot(u8 dev_id, u32 index)
{
	u64 addr = g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr + DEVDRV_SQ_OCCUPY_SIZE;
	u8 *cq = (u8 *)(uintptr_t)(addr + index * DEVDRV_CQ_SLOT_SIZE);

	NPU_DRV_DEBUG("cq_info base vaddr = 0x%llx, id= %u, addr= %p\n", addr, index, cq);
	return cq;
}

struct devdrv_ts_sq_info *devdrv_calc_sq_info(u8 devid, u32 index)
{
	struct devdrv_ts_sq_info *sq = NULL;
	u64 addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	if (addr == 0) {
		NPU_DRV_ERR("invalid share mem addr\n");
		return NULL;
	}
	COND_RETURN_ERROR(index >= DEVDRV_MAX_SQ_NUM, NULL, "illegal sq index: %u\n", index);
	NPU_DRV_DEBUG("sq_info base vaddr = 0x%llx\n", addr);

	sq = (struct devdrv_ts_sq_info *)(uintptr_t) (addr +
		(long)(unsigned)sizeof(struct devdrv_ts_sq_info) * (index));

	NPU_DRV_DEBUG("sq->index = 0x%x, sq->head = 0x%x, "
		"sq->tail = 0x%x, sq->credit = 0x%x, "
		"sq->vir_addr = 0x%llx, " "\n",
		sq->index, sq->head, sq->tail, sq->credit,
		sq->vir_addr);
	return sq;
}

struct devdrv_ts_cq_info *devdrv_calc_cq_info(u8 devid, u32 index)
{
	struct devdrv_ts_cq_info *cq = NULL;
	u64 addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	if (addr == 0) {
		NPU_DRV_ERR("invalid share mem addr\n");
		return NULL;
	}
	COND_RETURN_ERROR(index >= DEVDRV_MAX_CQ_NUM, NULL, "illegal cq index: %u\n", index);
	NPU_DRV_DEBUG("cq_info base vaddr = 0x%llx\n", addr);
	NPU_DRV_DEBUG("DEVDRV_SQ_INFO_OCCUPY_SIZE:%d", DEVDRV_SQ_INFO_OCCUPY_SIZE);

	cq = (struct devdrv_ts_cq_info *)(uintptr_t) (addr + DEVDRV_SQ_INFO_OCCUPY_SIZE +
		(long)(unsigned)sizeof(struct devdrv_ts_cq_info) * (index));

	NPU_DRV_DEBUG("cq->index = 0x%x, cq->head = 0x%x, "
		"cq->tail = 0x%x, cq->phase = 0x%x, "
		"cq->int_flag = 0x%x, cq->slot_size = 0x%x, "
		"cq->vir_addr = 0x%llx\n",
		cq->index, cq->head, cq->tail, cq->phase,
		cq->int_flag, cq->slot_size, cq->vir_addr);

	return cq;
}

struct devdrv_stream_info *devdrv_calc_stream_info(u8 devid, u32 index)
{
	struct devdrv_stream_info *stream_info = NULL;
	u64 addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	COND_RETURN_ERROR(index >= DEVDRV_MAX_STREAM_ID, NULL, "illegal npu stream index\n");
	stream_info = (struct devdrv_stream_info *)(uintptr_t) (addr +
		DEVDRV_SQ_INFO_OCCUPY_SIZE + DEVDRV_CQ_INFO_OCCUPY_SIZE +
		(long)(unsigned)sizeof(struct devdrv_stream_info) * (index));
	return stream_info;
}

u32 *devdrv_get_ts_work_status(u8 devid)
{
	u64 addr;
	u32 *ts_status = NULL;

	addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	ts_status = (u32 *)(uintptr_t) (addr + DEVDRV_INFO_OCCUPY_SIZE);
	NPU_DRV_INFO("ts_status offset:0x%lx", DEVDRV_INFO_OCCUPY_SIZE);
	return ts_status;
}

int devdrv_shm_init(u8 dev_id)
{
	gfp_t gfp_flags = GFP_KERNEL | __GFP_COMP | __GFP_ZERO;
	char *tmp = NULL;
	struct devdrv_mem_desc *persistent_task_buf_desc = NULL;
	u32 *ts_status = NULL;
	struct devdrv_platform_info *plat_info = NULL;
	u32 order = 0;
	u32 doorbell_size;
	phys_addr_t doorbell_base ;

	NPU_DRV_DEBUG("dev_id = %u\n", dev_id);
	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "illegal npu dev id = %d\n", dev_id);

	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info failed\n");

	order = DEVDRV_MAX_INFO_ORDER;
	doorbell_base = DEVDRV_PLAT_GET_REG_DESC(plat_info, DEVDRV_REG_TS_DOORBELL).base;
	doorbell_size = DEVDRV_PLAT_GET_REG_DESC(plat_info, DEVDRV_REG_TS_DOORBELL).len;
	persistent_task_buf_desc = DEVDRV_PLAT_GET_PERSISTENT_TASK_BUF(plat_info);
	g_sq_desc = DEVDRV_PLAT_GET_SQCQ_BUF(plat_info);

	tmp = (char *)(uintptr_t) __get_free_pages(gfp_flags, order);
	COND_RETURN_ERROR(tmp == NULL, -ENOMEM, "alloc share mem descriptor memory failed !\n");

	g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr = g_sq_desc->base + CHIP_BASEADDR_PA_OFFSET * dev_id;
	g_shm_desc[dev_id][DEVDRV_SQ_MEM].size = g_sq_desc->len;
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr = virt_to_phys(tmp);
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr = (vir_addr_t) (uintptr_t) tmp;
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].size = (long)(unsigned)(1 << order) * PAGE_SIZE;
	g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].phy_addr = doorbell_base;
	g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].size = doorbell_size;
	g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].phy_addr = persistent_task_buf_desc->base;
	g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].size = persistent_task_buf_desc->len;

	ts_status = devdrv_get_ts_work_status(dev_id);
	*ts_status = DEVDRV_TS_DOWN;

	gfp_flags = GFP_KERNEL | __GFP_ZERO;
	tmp = (char *)(uintptr_t) __get_free_pages(gfp_flags, 0);
	COND_RETURN_ERROR(tmp == NULL, -ENOMEM, "alloc share mem pad memory failed\n");

	g_shm_desc[dev_id][DEVDRV_PAD_MEM].phy_addr = virt_to_phys(tmp);
	g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr = (vir_addr_t)(uintptr_t)tmp;
	g_shm_desc[dev_id][DEVDRV_PAD_MEM].size = PAGE_SIZE;

	NPU_DRV_DEBUG("sq mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].size);

	NPU_DRV_DEBUG("info mem: virt_addr = 0x%llx, order = %u, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr,
		order, g_shm_desc[dev_id][DEVDRV_INFO_MEM].size);

	NPU_DRV_DEBUG("doorbell mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].size);

	NPU_DRV_DEBUG("persistent task buf mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].phy_addr,
		g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].size);

	// init continuous memory resource
	return devdrv_continuous_mem_init(dev_id);
}

void devdrv_shm_destroy(u8 dev_id)
{
	unsigned long addr;
	u32 order;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id %d\n", dev_id);
		return;
	}

	order = DEVDRV_MAX_INFO_ORDER;
	addr = (unsigned long)g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr;
	if (addr != 0) {
		free_pages(addr, order);
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr = 0;
	}

	addr = (unsigned long)g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr;
	if (addr != 0) {
		free_pages(addr, 0);
		g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr = 0;
	}
}

struct devdrv_hwts_sq_info *devdrv_calc_hwts_sq_info(u8 devid, u32 index)
{
	struct devdrv_hwts_sq_info *hwts_sq_info = NULL;
	u64 addr;

	if (index >= DEVDRV_MAX_HWTS_SQ_INFO_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d, index=%d\n", devid, index);
		return NULL;
	}

	addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	hwts_sq_info = (struct devdrv_hwts_sq_info *)(uintptr_t) (addr + DEVDRV_HWTS_SQ_INFO_OFFSET +
		(long)(unsigned)sizeof(struct devdrv_hwts_sq_info) * (index));

	return hwts_sq_info;
}

struct devdrv_hwts_cq_info *devdrv_calc_hwts_cq_info(u8 devid, u32 index)
{
	struct devdrv_hwts_cq_info *hwts_cq_info = NULL;
	u64 addr;

	if (index >= DEVDRV_MAX_HWTS_CQ_INFO_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d, index=%d\n", devid, index);
		return NULL;
	}

	addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	hwts_cq_info = (struct devdrv_hwts_cq_info *)(uintptr_t) (addr + DEVDRV_HWTS_CQ_INFO_OFFSET +
		(long)(unsigned)sizeof(struct devdrv_hwts_cq_info) * (index));

	return hwts_cq_info;
}

struct devdrv_model_desc_info *devdrv_calc_model_desc_info(u8 devid, u32 index)
{
	struct devdrv_model_desc_info *model_desc_info = NULL;
	u64 addr;

	if (index >= DEVDRV_MAX_MODEL_ID) {
		NPU_DRV_ERR("illegal npu dev id= %u, model_id= %u\n", devid, index);
		return NULL;
	}

	addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	model_desc_info = (struct devdrv_model_desc_info *)(uintptr_t) (addr + DEVDRV_MODEL_INFO_OFFSET +
		(long)(unsigned)sizeof(struct devdrv_model_desc_info) * (index));

	NPU_DRV_DEBUG("npu devid= %u, model_id= %u, index= %u, addr= %pK\n",
		devid, index, index, model_desc_info);
	return model_desc_info;
}

struct devdrv_prof_info *devdrv_calc_profiling_info(u8 devid)
{
	u64 addr;
	struct devdrv_prof_info *profiling_info = NULL;

	addr = g_shm_desc[devid][DEVDRV_INFO_MEM].virt_addr;
	profiling_info = (struct devdrv_prof_info *)(uintptr_t) (addr + DEVDRV_PROFILINGL_INFO_OFFSET);

	return profiling_info;
}

int devdrv_shm_v200_init_sq_mem(struct devdrv_platform_info *plat_info, u8 dev_id)
{
	vir_addr_t virt_addr;
	g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr = g_sq_desc->base + CHIP_BASEADDR_PA_OFFSET * dev_id;
	g_shm_desc[dev_id][DEVDRV_SQ_MEM].size = g_sq_desc->len;
	virt_addr = (vir_addr_t)(uintptr_t)ioremap_wc(
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr, g_shm_desc[dev_id][DEVDRV_SQ_MEM].size);
	if (virt_addr == 0) {
		NPU_DRV_ERR("ioremap_wc failed!");
		return -1;
	}
	g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr = virt_addr;

	NPU_DRV_DEBUG("sqcq mem: phy_addr = 0x%llx, size = %lu, virt_addr = 0x%llx, "
		"sq phy_addr = 0x%llx, cq phy_addr = 0x%llx, "
		"dfx sq phy_addr = 0x%llx, dfx cq phy_addr = 0x%llx \n",
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].size,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr + DEVDRV_SQ_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr + DEVDRV_SQ_OCCUPY_SIZE + DEVDRV_CQ_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr + DEVDRV_SQ_OCCUPY_SIZE +
			DEVDRV_CQ_OCCUPY_SIZE + DEVDRV_DFX_SQ_OCCUPY_SIZE);

	return 0;
}

int devdrv_shm_v200_init_info_mem(struct devdrv_platform_info *plat_info, u8 dev_id)
{
	vir_addr_t virt_addr;
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr = DEVDRV_PLAT_GET_INFO_BUF(plat_info)->base;
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].size = DEVDRV_PLAT_GET_INFO_BUF(plat_info)->len;
	virt_addr = (vir_addr_t)(uintptr_t)ioremap_wc(
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr, g_shm_desc[dev_id][DEVDRV_INFO_MEM].size);
	g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr = virt_addr;
	if (virt_addr == 0) {
		NPU_DRV_ERR("ioremap_wc failed!");
		return -1;
	}

	NPU_DRV_DEBUG("info mem: phy_addr = 0x%llx, size = %lu, "
		"sqinfo phy_addr = 0x%llx, cqinfo phy_addr = 0x%llx, "
		"streaminfo phy_addr = 0x%llx, hwtssqinfo phy_addr = 0x%llx, "
		"hwtscqinfo phy_addr = 0x%llx, modelinfo phy_addr = 0x%llx \n",
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].size,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr + DEVDRV_SQ_INFO_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr + DEVDRV_SQ_INFO_OCCUPY_SIZE + DEVDRV_CQ_INFO_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr + DEVDRV_SQ_INFO_OCCUPY_SIZE +
			DEVDRV_CQ_INFO_OCCUPY_SIZE + DEVDRV_STREAM_INFO_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr + DEVDRV_SQ_INFO_OCCUPY_SIZE +
			DEVDRV_CQ_INFO_OCCUPY_SIZE + DEVDRV_STREAM_INFO_OCCUPY_SIZE + DEVDRV_HWTS_SQ_INFO_OCCUPY_SIZE,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr + DEVDRV_SQ_INFO_OCCUPY_SIZE +
			DEVDRV_CQ_INFO_OCCUPY_SIZE + DEVDRV_STREAM_INFO_OCCUPY_SIZE +
			DEVDRV_HWTS_SQ_INFO_OCCUPY_SIZE + DEVDRV_HWTS_CQ_INFO_OCCUPY_SIZE);

	return 0;
}

int devdrv_shm_v200_init_pad_mem(u8 dev_id)
{
	gfp_t gfp_flags;
	char *tmp = NULL;

	gfp_flags = GFP_KERNEL | __GFP_ZERO;
	tmp = (char *)(uintptr_t) __get_free_pages(gfp_flags, 0);
	if (tmp == NULL) {
		NPU_DRV_ERR("alloc share mem pad memory failed.n");
		return -ENOMEM;
	}
	g_shm_desc[dev_id][DEVDRV_PAD_MEM].phy_addr = virt_to_phys(tmp);
	g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr = (vir_addr_t)(uintptr_t) tmp;
	g_shm_desc[dev_id][DEVDRV_PAD_MEM].size = PAGE_SIZE;

	NPU_DRV_DEBUG("sq mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].size);

	NPU_DRV_DEBUG("info mem: virt_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr,
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].size);

	NPU_DRV_DEBUG("doorbell mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].size);

	NPU_DRV_DEBUG("pad buf mem: phy_addr = 0x%llx, size = %lu\n",
		g_shm_desc[dev_id][DEVDRV_PAD_MEM].phy_addr,
		g_shm_desc[dev_id][DEVDRV_PAD_MEM].size);

	return 0;
}

int devdrv_shm_v200_init(u8 dev_id)
{
	struct devdrv_platform_info *plat_info = NULL;
	phys_addr_t doorbell_base;
	u32 doorbell_size;
	struct devdrv_mem_desc *persistent_task_buf_desc = NULL;
	u32 *ts_status = NULL;
	int ret;

	NPU_DRV_DEBUG("dev_id = %u\n", dev_id);
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d\n", dev_id);
		return -1;
	}

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -1;
	}

	doorbell_base = DEVDRV_PLAT_GET_REG_DESC(plat_info, DEVDRV_REG_TS_DOORBELL).base;
	doorbell_size = DEVDRV_PLAT_GET_REG_DESC(plat_info, DEVDRV_REG_TS_DOORBELL).len;
	persistent_task_buf_desc = DEVDRV_PLAT_GET_PERSISTENT_TASK_BUF(plat_info);
	g_sq_desc = DEVDRV_PLAT_GET_SQCQ_BUF(plat_info);

	ret = devdrv_shm_v200_init_sq_mem(plat_info, dev_id);
	if (ret != 0)
		return ret;

	ret = devdrv_shm_v200_init_info_mem(plat_info, dev_id);
	if (ret != 0)
		return ret;

	g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].phy_addr = doorbell_base;
	g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].size = doorbell_size;

	ts_status = devdrv_get_ts_work_status(dev_id);
	*ts_status = DEVDRV_TS_DOWN;

	return devdrv_shm_v200_init_pad_mem(dev_id);
}

void devdrv_shm_v200_destroy(u8 dev_id)
{
	unsigned long addr;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id %d\n", dev_id);
		return;
	}

	if (g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr != 0) {
		iounmap((void *)(
			uintptr_t)g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr);
		g_shm_desc[dev_id][DEVDRV_SQ_MEM].virt_addr = 0;
	}

	if (g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr != 0) {
		iounmap((void *)(
			uintptr_t)g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr);
		g_shm_desc[dev_id][DEVDRV_INFO_MEM].virt_addr = 0;
	}

	addr = (unsigned long)g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr;
	if (addr != 0) {
		free_pages(addr, 0);
		g_shm_desc[dev_id][DEVDRV_PAD_MEM].virt_addr = 0;
	}
}

int devdrv_devmem_swapin(struct vm_area_struct *vma, unsigned long devmem_base,
                         unsigned long size, unsigned long align_size)
{
	int err = 0;
	struct mm_struct *mm = NULL;
	unsigned long vma_start_aligned;
	COND_RETURN_ERROR (vma == NULL, -EFAULT, "vma is NULL\n");

	vma_start_aligned = ALIGN_UP(vma->vm_start, align_size);
	NPU_DRV_DEBUG(
		"zap_vma_ptes.devmem_base=0x%lx, vma_start_aligned=0x%lx, size=0x%lx, align_size=0x%lx, err=%d\n",
		devmem_base, vma_start_aligned, size, align_size, err);
	NPU_DRV_DEBUG("vm_start = 0x%lx vm_end = 0x%lx vm_flags=0x%lx vm_page_prot=0x%llx",
		vma->vm_start, vma->vm_end, vma->vm_flags, (u64)vma->vm_page_prot.pgprot);

	err = zap_vma_ptes(vma, vma_start_aligned, size);
	if (err) {
		NPU_DRV_ERR(
			"zap_vma_ptes failed. size=0x%lx,align_size=0x%lx, err=%d\n",
			size, align_size, err);
		NPU_DRV_DEBUG("vm_start = 0x%lx vm_end = 0x%lx vm_flags=0x%lx vm_page_prot=0x%lx",
			vma->vm_start, vma->vm_end, vma->vm_flags, vma->vm_page_prot.pgprot);
		return -EFAULT;
	}
	COND_RETURN_ERROR(size <= 0, -ENOMEM," size = %lu\n", size);

	if (remap_pfn_range(vma, vma_start_aligned, __phys_to_pfn(devmem_base), size, vma->vm_page_prot)) {
		NPU_DRV_ERR("fail to map body mem. align_size=0x%lx\n", align_size);
		return -ENXIO;
	}

	mm = current->mm;
	COND_RETURN_ERROR(mm == NULL, -ENXIO);
	COND_RETURN_ERROR(hisi_svm_flush_cache(mm, vma->vm_start, size), -ENXIO);

	return 0;
}

int devdrv_devmem_swapout(struct vm_area_struct *vma, unsigned long pad_base,
                          unsigned long size, unsigned long align_size)
{
	int err = 0;
	struct mm_struct *mm = NULL;
	unsigned long vma_start_aligned;
	unsigned long pad_start;
	unsigned long vma_tmp;

	vma_start_aligned = ALIGN_UP(vma->vm_start, align_size);

	NPU_DRV_DEBUG("vma_start_aligned=0x%lx, size=0x%lx, align_size=0x%lx, err=%d\n",
		vma_start_aligned, size, align_size, err);
	NPU_DRV_DEBUG("vm_start = 0x%lx vm_end = 0x%lx vm_flags=0x%lx vm_page_prot=0x%llx",
		vma->vm_start, vma->vm_end, vma->vm_flags, (u64)vma->vm_page_prot.pgprot);

	err = zap_vma_ptes(vma, vma_start_aligned, size);
	if (err) {
		NPU_DRV_ERR("zap_vma_ptes failed. size=0x%lx, align_size=0x%lx, err=%d\n",
			size, align_size, err);
		NPU_DRV_DEBUG("vm_start = 0x%lx vm_end = 0x%lx vm_flags=0x%lx vm_page_prot=0x%lx",
			vma->vm_start, vma->vm_end, vma->vm_flags, vma->vm_page_prot.pgprot);
		return -EFAULT;
	}

	vma_tmp = vma_start_aligned + size;
	COND_RETURN_ERROR(size <= 0, -ENOMEM," size = %lu\n", size);
	for (pad_start = vma_start_aligned; pad_start < vma_tmp; pad_start += PAGE_SIZE) {
		if (remap_pfn_range(vma, pad_start, __phys_to_pfn(pad_base), PAGE_SIZE, vma->vm_page_prot)) {
			NPU_DRV_ERR("fail to map pad mem\n");
			return -ENXIO;
		}
	}

	mm = current->mm;
	COND_RETURN_ERROR(mm == NULL, -ENXIO);
	COND_RETURN_ERROR(hisi_svm_flush_cache(mm, vma->vm_start, size), -ENXIO);

	return 0;
}

bool devdrv_vma_valid(const struct vm_area_struct *vma)
{
	struct mm_struct *vm_mm = NULL;
	struct vm_area_struct *vma_node = NULL;
	int i;
	int map_count;

	if (vma == NULL) {
		NPU_DRV_ERR("vma is null\n");
		return false;
	}

	vm_mm = current->mm;
	if (vm_mm == NULL) {
		NPU_DRV_ERR("vm_mm is null\n");
		return false;
	}

	vma_node = vm_mm->mmap; /* list of VMAs, first node */
	map_count = vm_mm->map_count;

	for (i = 0; i < map_count && vma_node != NULL; i++) {
		if (vma_node == vma) {
			NPU_DRV_DEBUG("VALID.%d vma=%pK vm_mm=%pK vm_next=%pK vm_start=0x%lx, vm_end =0x%lx, map_count=%d\n",
				i, vma, vma->vm_mm, vma->vm_next, vma->vm_start, vma->vm_end, map_count);
			return true;
		}
		vma_node = vma_node->vm_next;
	}
	NPU_DRV_DEBUG("UNVALID.%d vma=%pK vm_mm=%pK vm_next=%pK vm_start=0x%lx, vm_end =0x%lx, map_count=%d\n",
		i, vma, vma->vm_mm, vma->vm_next, vma->vm_start, vma->vm_end, map_count);

	return false;
}

static int l2_mem_check_vma(struct vm_area_struct *vma, unsigned long *base, unsigned long *len)
{
	struct devdrv_mem_desc *l2_desc = NULL;
	unsigned long l2_len;
	unsigned long vma_len;

	l2_desc = devdrv_plat_get_reg_desc(DEVDRV_REG_L2BUF_BASE);
	if (l2_desc == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_reg_desc failed\n");
		return -1;
	}
	if (vma->vm_end < vma->vm_start) {
		NPU_DRV_ERR("invalid vma\n");
		return -1;
	}
	l2_len = l2_desc->len + 1; // becasue of dts will minus 1
	NPU_DRV_WARN("l2_len %lu \n", l2_len);
	vma_len = vma->vm_end - vma->vm_start;
	if (vma_len != l2_len * L2_MAP_ALIGN) {
		NPU_DRV_ERR("vma len %lu l2_len %lu l2 map align %d is not match\n", vma_len, l2_len, L2_MAP_ALIGN);
		return -EFAULT;
	}
	*base = l2_desc->base;
	*len = l2_len;
	return 0;
}

int l2_mem_swapin(struct vm_area_struct *vma)
{
	unsigned long l2_base = 0;
	unsigned long l2_len = 0;
	int err;

	err = l2_mem_check_vma(vma, &l2_base, &l2_len);
	if (err) {
		NPU_DRV_ERR("l2 check vma fail err %d\n", err);
		return -1;
	}
	err = devdrv_devmem_swapin(vma, l2_base, l2_len, l2_len);
	if (err) {
		NPU_DRV_ERR("devdrv_devmem_swapin failed. l2_len=0x%lx\n", l2_len);
		return -1;
	}
	NPU_DRV_WARN("l2_mem_swapin exit");
	return 0;
}

int l2_mem_swapout(struct vm_area_struct *vma, u8 dev_id)
{
	unsigned long l2_base = 0;
	unsigned long l2_len = 0;
	unsigned long pad_base;
	int err;
	NPU_DRV_DEBUG(" enter");

	pad_base = g_shm_desc[dev_id][DEVDRV_PAD_MEM].phy_addr;
	if (pad_base == 0) {
		NPU_DRV_ERR("invalid pad_base\n");
		return -EFAULT;
	}

	err = l2_mem_check_vma(vma, &l2_base, &l2_len);
	if (err) {
		NPU_DRV_ERR("l2 check vma fail dev id %u err %d\n", dev_id, err);
		return -1;
	}

	err = devdrv_devmem_swapout(vma, pad_base, l2_len, l2_len);
	if (err) {
		NPU_DRV_ERR("devdrv_devmem_swapout failed. l2_len=0x%lx\n", l2_len);
		return -1;
	}
	NPU_DRV_WARN("l2_mem_swapout exit");
	return 0;
}

int devdrv_map_l2_buff(const struct file *filp, struct vm_area_struct *vma, u8 dev_id)
{
	unsigned long pad_base, pad_start;
	unsigned long l2_base;
	unsigned long l2_len;
	int err;

	NPU_DRV_DEBUG("map l2 buff enter");

	if ((vma == NULL) || (filp == NULL) || (dev_id >= NPU_DEV_NUM)) {
		NPU_DRV_ERR("invalid para\n");
		return -EFAULT;
	}

	err = l2_mem_check_vma(vma, &l2_base, &l2_len);
	if (err) {
		NPU_DRV_ERR("l2 check vma fail dev id %u err %d\n", dev_id, err);
		return -EFAULT;
	}

	NPU_DRV_DEBUG("vma=%pK vm_mm=%pK vm_next=%pK vm_start=0x%lx, vm_end =0x%lx\n",
		vma, vma->vm_mm, vma->vm_next, vma->vm_start, vma->vm_end);

	/* we do not want to have this area swapped out, lock it */
	vma->vm_flags |= VM_LOCKED;

	pad_base = g_shm_desc[dev_id][DEVDRV_PAD_MEM].phy_addr;
	if (pad_base == 0) {
		NPU_DRV_ERR("invalid mem base\n");
		return -EFAULT;
	}

	/**
	 * map head with the pad page
	 */
	for (pad_start = vma->vm_start; pad_start < vma->vm_end; pad_start += PAGE_SIZE) {
		if (remap_pfn_range(vma, pad_start, __phys_to_pfn(pad_base), PAGE_SIZE, vma->vm_page_prot)) {
			NPU_DRV_ERR("fail to map pad mem\n");
			return -ENXIO;
		}
	}

	NPU_DRV_DEBUG("map l2 buff success");
	return 0;
}

// map dev_id npu's info a€?sq and cq to user space
int devdrv_info_sq_cq_mmap(u8 dev_id, const struct file *filep, struct vm_area_struct *vma)
{
	int err;
	size_t size;
	unsigned long start_addr = 0;
	phys_addr_t phy_addr;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();

	COND_RETURN_ERROR(plat_info == NULL, -EINVAL, "devdrv_plat_get_info\n");
	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "illegal npu dev id %d\n", dev_id);
	COND_RETURN_ERROR(vma == NULL, -1, "vma is null\n");

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 0) {
		NPU_DRV_DEBUG("vma->vm_end = %pK  vma->vm_start = %pK \n",
			(void *)(uintptr_t) vma->vm_end, (void *)(uintptr_t) vma->vm_start);

		COND_RETURN_ERROR((vma->vm_end < vma->vm_start) || ((vma->vm_end - vma->vm_start) != LITE_NPU_SHARE_MEM_MAP_SIZE),
			-EINVAL, "invalid vma length\n");

		vma->vm_flags |= VM_LOCKED;
		vma->vm_flags |= VM_DONTEXPAND;
		vma->vm_flags |= VM_PFNMAP;
		vma->vm_flags |= VM_WRITE;

		/*
		   |___SQ(32MB)___|____INFO(32MB)_____|__DOORBELL(32MB)___|___CQ(32MB)___|(32M vitural address space respectively)
		 */
		start_addr = vma->vm_start;
		phy_addr = g_sq_desc->base;
		NPU_DRV_DEBUG("npu dev %d sq mem: user_virt_addr=0x%lx, phys_addr=0x%llx\n", dev_id, start_addr, phy_addr);

		/* remap sq cq pfn range for user space */
		err = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT,
			DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_NUM, vma->vm_page_prot);
		COND_RETURN_ERROR(err != 0, -EINVAL, "npu dev_id %d sq memory mmap failed\n", dev_id);

		/* remap info pfn range for user space */
		phy_addr = g_shm_desc[dev_id][DEVDRV_INFO_MEM].phy_addr;
		size = g_shm_desc[dev_id][DEVDRV_INFO_MEM].size;
		start_addr += DEVDRV_VM_BLOCK_OFFSET;  // gap to reduce memory override probability
		COND_RETURN_ERROR(size <= 0, -ENOMEM, "npu dev %d illegal info mem size = %lu\n", dev_id, size);

		NPU_DRV_DEBUG("npu dev %d info mem:user_virt_addr = 0x%lx, "
			"phys_addr = 0x%llx, size = %lu\n", dev_id, start_addr, phy_addr, size);
		if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1)
			vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
		err = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT, size, vma->vm_page_prot);
		if (err) {
			NPU_DRV_ERR("npu dev_id %d info memory mmap failed\n", dev_id);
			return -EINVAL;
		}
		/* remap doorbell pfn range for user space temporarily,will delete later */
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		start_addr += DEVDRV_VM_BLOCK_OFFSET;
		phy_addr = g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].phy_addr;
		size = g_shm_desc[dev_id][DEVDRV_DOORBELL_MEM].size;
		COND_RETURN_ERROR(size <= 0, -ENOMEM, "npu dev %d illegal doorbell cfg size = %lu\n", dev_id, size);
		NPU_DRV_DEBUG("npu dev %d doorbell mem:user_virt_addr = 0x%lx, "
			"phys_addr = 0x%llx, size = %lu\n", dev_id, start_addr, phy_addr, size);

		err = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT, size, vma->vm_page_prot);
		COND_RETURN_ERROR(err != 0, -EINVAL, "npu dev_id %d doobell register mmap failed\n", dev_id);

		// cq non cache solution
		start_addr += DEVDRV_VM_BLOCK_OFFSET;
		phy_addr = (unsigned long long)(g_sq_desc->base +
			DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_NUM);
		NPU_DRV_DEBUG("npu dev %d cq mem:user_virt_addr=0x%lx, phys_addr=0x%llx\n", dev_id, start_addr, phy_addr);

		err = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT,
			DEVDRV_MAX_CQ_DEPTH * DEVDRV_CQ_SLOT_SIZE, vma->vm_page_prot);
		COND_RETURN_ERROR(err != 0, -EINVAL, "npu dev_id %d cq mem mmap failed\n", dev_id);

		// persistent task buff
		start_addr += DEVDRV_VM_BLOCK_OFFSET;
		phy_addr =  g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].phy_addr;
		size =  g_shm_desc[dev_id][DEVDRV_PERSISTENT_TASK_BUFF].size;
		COND_RETURN_ERROR(size <= 0, -ENOMEM, "npu dev %d illegal info mem size = %lu\n", dev_id, size);

		NPU_DRV_DEBUG("npu dev %d persistent task buff mem:user_virt_addr = 0x%lx, "
			"phys_addr = 0x%llx, size = %lu\n", dev_id, start_addr, phy_addr, size);
		err = remap_pfn_range(vma, start_addr, phy_addr >> PAGE_SHIFT, size, pgprot_writecombine(PAGE_SHARED));
		if (err) {
			NPU_DRV_ERR("npu dev_id %d persistent task buff mem mmap failed\n", dev_id);
			return -EINVAL;
		}
	}

	return 0;
}
