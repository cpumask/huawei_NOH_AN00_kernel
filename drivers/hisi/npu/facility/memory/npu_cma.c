/*
 * npu_cma.c
 *
 * about npu cma
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
#include "npu_cma.h"
#include <asm/tlbflush.h>

#include "npu_log.h"
#include "hisi_svm.h"
#include "npu_platform.h"

static struct devdrv_continuous_mem g_continuous_mem[NPU_DEV_NUM];

int devdrv_continuous_mem_init(u8 dev_id)
{
	struct devdrv_dev_ctx *dev_ctx = NULL;
	void *cpu_addr = NULL;
	struct devdrv_platform_info *plat_info = NULL;
	dma_addr_t dma_handle = 0;
	u64 size = MEM_INIT_SIZE;

	spin_lock_init(&g_continuous_mem[dev_id].cm_spinlock);
	spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
	g_continuous_mem[dev_id].total_size = 0;
	spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);

	dev_ctx = get_dev_ctx_by_id(dev_id);
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("npu dev %d `s dev_ctx is null\n", dev_id);
		return -1;
	}
	dev_ctx->cm = &g_continuous_mem[dev_id];

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -1;
	}

	cpu_addr = dma_alloc_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size, &dma_handle, GFP_KERNEL);
	if (cpu_addr != NULL) {
		NPU_DRV_WARN("succ to malloc cma mem size 0x%llx\n", size);
		spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
		g_continuous_mem[dev_id].mem_info[SHARE_NUM_OCCUPIED].dma_handle = dma_handle;
		g_continuous_mem[dev_id].mem_info[SHARE_NUM_OCCUPIED].cpu_addr = cpu_addr;
		g_continuous_mem[dev_id].mem_info[SHARE_NUM_OCCUPIED].size = size;
		g_continuous_mem[dev_id].mem_info[SHARE_NUM_OCCUPIED].valid = 0;
		spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);
	} else {
		NPU_DRV_ERR("fail to malloc cma mem size 0x%llx\n", size);
		return -1;
	}

	return 0;
}

int devdrv_save_cm_info(struct devdrv_dev_ctx *dev_ctx, struct devdrv_cm_info *info, u32 *share_num)
{
	int i;
	u8 dev_id;

	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	if (info == NULL) {
		NPU_DRV_ERR("cm info is null\n");
		return -EINVAL;
	}

	if (share_num == NULL) {
		NPU_DRV_ERR("share_num addr is null\n");
		return -EINVAL;
	}

	dev_id = dev_ctx->devid;
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d\n", dev_id);
		return -EINVAL;
	}

	for (i = 0; i < MAX_MEM_INFO_NUM; i++) {
		if (dev_ctx->cm->mem_info[i].valid != MEM_INFO_VALID) {
			spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
			dev_ctx->cm->mem_info[i].dma_handle = info->dma_handle;
			dev_ctx->cm->mem_info[i].cpu_addr = info->cpu_addr;
			dev_ctx->cm->mem_info[i].size = info->size;
			dev_ctx->cm->mem_info[i].valid = MEM_INFO_VALID;
			*share_num = (u32)i;
			spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);
			return 0;
		} else if (info->dma_handle == dev_ctx->cm->mem_info[i].dma_handle) {
			NPU_DRV_ERR("repeat insert npu cm l2 ctrl addr\n");
			return -EINVAL;
		}
	}

	return -EINVAL;
}

int devdrv_save_cm_info_occupied(struct devdrv_dev_ctx *dev_ctx, u32 *share_num)
{
	u8 dev_id;

	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	if (share_num == NULL) {
		NPU_DRV_ERR("share_num addr is null\n");
		return -EINVAL;
	}

	dev_id = dev_ctx->devid;
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d\n", dev_id);
		return -EINVAL;
	}

	spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
	dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].valid = MEM_INFO_VALID;
	*share_num = SHARE_NUM_OCCUPIED;
	spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);
	return 0;
}

int devdrv_delete_cm_info(struct devdrv_dev_ctx *dev_ctx, u32 share_num)
{
	u8 dev_id;

	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	dev_id = dev_ctx->devid;
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d\n", dev_id);
		return -EINVAL;
	}

	if (share_num >= MAX_MEM_INFO_NUM) {
		NPU_DRV_ERR("invalid share_num %d\n", share_num);
		return -EINVAL;
	}

	if (dev_ctx->cm->mem_info[share_num].valid != MEM_INFO_VALID) {
		NPU_DRV_ERR("invalid info ,no need to delete %d\n", dev_ctx->cm->mem_info[share_num].valid);
		return -EINVAL;
	}

	spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
	dev_ctx->cm->mem_info[share_num].dma_handle = 0;
	dev_ctx->cm->mem_info[share_num].cpu_addr = 0;
	dev_ctx->cm->mem_info[share_num].size = 0;
	dev_ctx->cm->mem_info[share_num].valid = 0;
	dev_ctx->cm->mem_info[share_num].user_va = 0;
	spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);

	return 0;
}

int devdrv_delete_cm_info_occupied(struct devdrv_dev_ctx *dev_ctx)
{
	u8 dev_id;

	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	dev_id = dev_ctx->devid;
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id = %d\n", dev_id);
		return -EINVAL;
	}

	if (dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].valid != MEM_INFO_VALID) {
		NPU_DRV_ERR("invalid info ,no need to delete %d\n", dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].valid);
		return -EINVAL;
	}

	spin_lock(&g_continuous_mem[dev_id].cm_spinlock);
	dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].valid = 0;
	spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);

	return 0;
}

int devdrv_inc_cm_total_size(struct devdrv_dev_ctx *dev_ctx, u64 size)
{
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	spin_lock(&dev_ctx->cm->cm_spinlock);
	dev_ctx->cm->total_size += size;
	spin_unlock(&dev_ctx->cm->cm_spinlock);

	NPU_DRV_DEBUG("total ocuppied cm size = %llx after request now\n", dev_ctx->cm->total_size);

	return 0;
}

static int devdrv_get_cm_info(u8 dev_id, u32 share_num, struct devdrv_cm_info *info)
{
	spin_lock(&g_continuous_mem[dev_id].cm_spinlock);

	info->dma_handle = g_continuous_mem[dev_id].mem_info[share_num].dma_handle;
	info->cpu_addr = g_continuous_mem[dev_id].mem_info[share_num].cpu_addr;
	info->size = g_continuous_mem[dev_id].mem_info[share_num].size;
	spin_unlock(&g_continuous_mem[dev_id].cm_spinlock);

	return 0;
}

int devdrv_dec_cm_total_size(struct devdrv_dev_ctx *dev_ctx, u64 size)
{
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	spin_lock(&dev_ctx->cm->cm_spinlock);
	dev_ctx->cm->total_size -= size;
	spin_unlock(&dev_ctx->cm->cm_spinlock);

	NPU_DRV_DEBUG("total ocuppied cm size = 0x%llx after free now\n", dev_ctx->cm->total_size);

	return 0;
}

int devdrv_is_cm_available(const struct devdrv_dev_ctx *dev_ctx, u64 req_size)
{
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	if (dev_ctx->cm->total_size >= MEM_TOTAL_SIZE || req_size > MEM_TOTAL_SIZE || req_size == 0) {
		NPU_DRV_ERR("total size = 0x%llx, req_size = 0x%llx\n", dev_ctx->cm->total_size, req_size);
		return -EINVAL;
	}

	NPU_DRV_DEBUG("current cm occupied total size = 0x%llx, "
		"request_size = 0x%llx\n dev mem total size = 0x%x \n",
		dev_ctx->cm->total_size, req_size, MEM_TOTAL_SIZE);

	if (req_size > MEM_TOTAL_SIZE) {
		NPU_DRV_ERR("req_size = 0x%llx is oversize\n", req_size);
		return -EINVAL;
	}

	if (dev_ctx->cm->total_size > (MEM_TOTAL_SIZE - req_size)) {
		NPU_DRV_ERR("left cm size is no enough now\n");
		return -EINVAL;
	}

	return 0;
}

int devdrv_is_cm_valid(const struct devdrv_dev_ctx *dev_ctx, u32 share_num)
{
	u64 total_size;
	u64 size;

	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	if (share_num >= MAX_MEM_INFO_NUM) {
		NPU_DRV_ERR("invalid share_num %d\n", share_num);
		return -EINVAL;
	}

	if (dev_ctx->cm->mem_info[share_num].valid != MEM_INFO_VALID) {
		NPU_DRV_ERR("invalid cm info valid_val =  %d share_num = %d\n",
			dev_ctx->cm->mem_info[share_num].valid, share_num);
		return -EINVAL;
	}

	size = dev_ctx->cm->mem_info[share_num].size;
	total_size = dev_ctx->cm->total_size;
	if (size > total_size) {
		NPU_DRV_ERR("share_num = %d invalid free_size 0x%llx "
			"total size 0x%llx\n", share_num, size, total_size);
		return -EINVAL;
	}

	return 0;
}

// for l2ctrl using currently
int devdrv_map_cm(const struct devdrv_proc_ctx *proc_ctx, struct vm_area_struct *vma,
		  u32 share_num, u8 dev_id)
{
	struct devdrv_cm_info info = { 0 };
	unsigned long pfn;
	unsigned long size;
	int ret;
	struct mm_struct *mm = NULL;
	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -EFAULT, "illegal npu dev id %d\n", dev_id);

	COND_RETURN_ERROR(share_num >= MAX_MEM_INFO_NUM, -EFAULT, "illegal share_num %d\n", share_num);

	COND_RETURN_ERROR(proc_ctx == NULL, -EFAULT, "proc_ctx is NULL\n");

	COND_RETURN_ERROR(proc_ctx->map_ops_flag != true, -EFAULT, "map_ops_flag is false. cannot map\n");
	size = vma->vm_end - vma->vm_start;

	ret = devdrv_get_cm_info(dev_id, share_num, &info);
	COND_RETURN_ERROR(ret != 0, -EFAULT, "get continue mem info fail ret %d\n", ret);

	pfn = info.dma_handle >> PAGE_SHIFT;
	/* we do not want to have this area swapped out, lock it */
	vma->vm_flags |= VM_LOCKED;

	COND_RETURN_ERROR((size != info.size) || (size > MAP_MAX_SIZE), -ENOMEM, "invalid cm size = %lu\n", size);

	NPU_DRV_INFO("vma_cm=%p vm_next=%p vm_start=0x%lx, vm_end =0x%lx\n",
		vma, vma->vm_next, vma->vm_start, vma->vm_end);

	vma->vm_page_prot = __pgprot_modify(vma->vm_page_prot, 0, PTE_DIRTY);
	ret = remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
	COND_RETURN_ERROR(ret != 0, -ENXIO, "remap_pfn_range cm failed ret = %d\n", ret);

	mm = current->mm;
	COND_RETURN_ERROR(mm == NULL, -ENXIO);
	COND_RETURN_ERROR(hisi_svm_flush_cache(mm, vma->vm_start, size), -ENXIO);

	return ret;
}

int devdrv_cm_resource_recycle(struct devdrv_dev_ctx *dev_ctx)
{
	struct devdrv_platform_info *plat_info = NULL;
	int i = 0;
	u64 size = 0;
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("invalid dev_ctx value\n");
		return -EINVAL;
	}

	if (dev_ctx->cm == NULL) {
		NPU_DRV_ERR("invalid cm value\n");
		return -EINVAL;
	}

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -EINVAL;
	}

	for (i = 0; i < MAX_MEM_INFO_NUM; i++) {
		if (dev_ctx->cm->mem_info[i].valid != MEM_INFO_VALID) {
			continue;
		}
		size = dev_ctx->cm->mem_info[i].size;
		if ((dev_ctx->cm->total_size) < size) {
			NPU_DRV_WARN("invalid size 0x%llx total size 0x%llx\n", size, dev_ctx->cm->total_size);
			return -EINVAL;
		}
		NPU_DRV_WARN("recycle cm mem %d size 0x%llx \n", i, size);
		dma_free_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size, dev_ctx->cm->mem_info[i].cpu_addr,
			dev_ctx->cm->mem_info[i].dma_handle);

		(void)devdrv_delete_cm_info(dev_ctx, i);
		spin_lock(&dev_ctx->cm->cm_spinlock);
		dev_ctx->cm->total_size -= size;
		spin_unlock(&dev_ctx->cm->cm_spinlock);
		NPU_DRV_WARN("recycle cm mem %d size 0x%llx " "cm->total_size(occupied) = 0x%llx \n",
			i, size, dev_ctx->cm->total_size);
	}

	return 0;
}
