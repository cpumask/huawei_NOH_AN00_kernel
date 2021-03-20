/*
 * npu_hwts_cq.c
 *
 * about npu hwts cq
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#include "npu_hwts_cq.h"

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

#include "devdrv_user_common.h"
#include "npu_iova.h"
#include "npu_common.h"
#include "npu_shm.h"
#include "npu_log.h"
#include "npu_pool.h"

// iova way
static int devdrv_hwts_cq_pool_alloc(u32 len, vir_addr_t *virt_addr, unsigned long *iova)
{
	int ret = 0;

	if (virt_addr == NULL || iova == NULL) {
		NPU_DRV_ERR("virt_addr or iova is null\n");
		return -1;
	}

	ret = devdrv_hisi_npu_iova_alloc(virt_addr, len);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_alloc fail, ret = %d\n", ret);
		return ret;
	}
	ret = devdrv_hisi_npu_iova_map(*virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_map fail, ret = %d\n", ret);
		devdrv_hisi_npu_iova_free(*virt_addr);
	}
	return ret;
}
static void devdrv_hwts_cq_pool_free(vir_addr_t virt_addr, unsigned long iova)
{
	int ret = 0;

	ret = devdrv_hisi_npu_iova_unmap(virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_unmap fail, ret = %d\n", ret);
		return;
	}

	devdrv_hisi_npu_iova_free(virt_addr);
}

int devdrv_hwts_cq_init(u8 dev_id)
{
	struct devdrv_hwts_cq_info *hwts_cq_info = NULL;
	int ret;
	int i;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	ret = devdrv_pool_regist(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_CQ, 0, DEVDRV_MAX_HWTS_CQ_NUM,
		DEVDRV_MAX_HWTS_CQ_DEPTH * DEVDRV_HWTS_CQ_SLOT_SIZE, DEVDRV_MAX_HWTS_CQ_POOL_NUM,
		devdrv_hwts_cq_pool_alloc, devdrv_hwts_cq_pool_free);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hwts_sq_init=>devdrv_pool_regist error! ret = %d, \n", ret);
		return -1;
	}

	for (i = 0; i < DEVDRV_MAX_HWTS_CQ_NUM; i++) {
		hwts_cq_info = devdrv_calc_hwts_cq_info(dev_id, i);
		hwts_cq_info->head = 0;
		hwts_cq_info->tail = 0;
		hwts_cq_info->index = (u32)i;
		hwts_cq_info->count_report = 0;
		hwts_cq_info->vir_addr = 0;
		hwts_cq_info->pid = DEVDRV_INVALID_FD_OR_NUM;
		hwts_cq_info->slot_size = DEVDRV_HWTS_CQ_SLOT_SIZE;
		hwts_cq_info->stream_num = 0;
		hwts_cq_info->receive_count = 0;
		hwts_cq_info->phase = 1;
	}
	return 0;
}

int devdrv_hwts_cq_destroy(u8 dev_id)
{
	return devdrv_pool_unregist(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_CQ);
}

int devdrv_alloc_hwts_cq(u8 dev_id)
{
	struct devdrv_entity_info *entity_info = NULL;
	struct devdrv_hwts_cq_info *hwts_cq_info = NULL;

	entity_info = devdrv_alloc_entity(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_CQ);
	if (entity_info == NULL) {
		NPU_DRV_ERR("devdrv_alloc_hwts_sq=>devdrv_alloc_entity fail\n");
		return -1;
	}

	hwts_cq_info = devdrv_calc_hwts_cq_info(dev_id, entity_info->index);
	if (hwts_cq_info == NULL) {
		NPU_DRV_ERR("devdrv_alloc_hwts_sq=>devdrv_calc_hwts_sq_info fail, index=%d\n", entity_info->index);
		devdrv_free_entity(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_CQ, entity_info->index);
		return -1;
	}

	hwts_cq_info->hwts_cq_sub = (u64)(uintptr_t)entity_info;
	hwts_cq_info->iova_addr = entity_info->iova;
	return entity_info->index;
}

int devdrv_free_hwts_cq(u8 dev_id, u32 hwts_cq_id)
{
	struct devdrv_hwts_cq_info *hwts_cq_info = NULL;
	int ret;

	ret = devdrv_free_entity(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_CQ, hwts_cq_id);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_free_hwts_cq=>devdrv_free_entity error!hwts_cq_id=%d, ret=%d", hwts_cq_id, ret);
		return ret;
	}

	hwts_cq_info = devdrv_calc_hwts_cq_info(dev_id, hwts_cq_id);
	if (hwts_cq_info == NULL) {
		NPU_DRV_DEBUG("hwts cq info is null\n");
		return -1;
	}
	hwts_cq_info->head = 0;
	hwts_cq_info->tail = 0;
	hwts_cq_info->count_report = 0;
	hwts_cq_info->stream_num = 0;
	hwts_cq_info->receive_count = 0;
	hwts_cq_info->slot_size = DEVDRV_HWTS_CQ_SLOT_SIZE;
	return 0;
}


