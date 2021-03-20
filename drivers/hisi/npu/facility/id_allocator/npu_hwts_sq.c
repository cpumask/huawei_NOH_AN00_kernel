/*
 * npu_hwts_sq.c
 *
 * about npu hwts sq
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
#include "npu_hwts_sq.h"

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/vmalloc.h>

#include "devdrv_user_common.h"
#include "npu_common.h"
#include "npu_shm.h"
#include "npu_log.h"
#include "npu_iova.h"
#include "npu_pool.h"

// iova way
static int devdrv_hwts_sq_pool_alloc(u32 len, vir_addr_t *virt_addr, unsigned long *iova)
{
	int ret;

	if (virt_addr == NULL || iova == NULL) {
		NPU_DRV_ERR("virt_add or iova is null\n");
		return -1;
	}

	ret = devdrv_hisi_npu_iova_alloc(virt_addr, len);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_alloc fail, ret=%d\n", ret);
		return ret;
	}
	ret = devdrv_hisi_npu_iova_map(*virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_map fail, ret=%d\n", ret);
		devdrv_hisi_npu_iova_free(*virt_addr);
	}
	return ret;
}

static void devdrv_hwts_sq_pool_free(vir_addr_t virt_addr, unsigned long iova)
{
	int ret = 0;

	ret = devdrv_hisi_npu_iova_unmap(virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_unmap fail, ret=%d\n", ret);
		return;
	}

	devdrv_hisi_npu_iova_free(virt_addr);
}

int devdrv_hwts_sq_init(u8 dev_id)
{
	struct devdrv_hwts_sq_info *hwts_sq_info = NULL;
	int ret;
	int i;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	ret = devdrv_pool_regist(
		dev_id,
		DEVDRV_MEM_POOL_TYPE_HWTS_SQ,
		DEVDRV_MAX_LONG_HWTS_SQ_NUM,
		DEVDRV_MAX_HWTS_SQ_NUM - DEVDRV_MAX_LONG_HWTS_SQ_NUM,
		DEVDRV_MAX_HWTS_SQ_DEPTH * DEVDRV_HWTS_SQ_SLOT_SIZE,
		DEVDRV_MAX_HWTS_SQ_POOL_NUM,
		devdrv_hwts_sq_pool_alloc,
		devdrv_hwts_sq_pool_free
	);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_pool_regist error! ret = %d", ret);
		return -1;
	}

	ret = devdrv_pool_regist(
		dev_id,
		DEVDRV_MEM_POOL_TYPE_LONG_HWTS_SQ,
		0,
		DEVDRV_MAX_LONG_HWTS_SQ_NUM,
		DEVDRV_MAX_LONG_HWTS_SQ_DEPTH * DEVDRV_HWTS_SQ_SLOT_SIZE,
		DEVDRV_MAX_LONG_HWTS_SQ_POOL_NUM,
		devdrv_hwts_sq_pool_alloc,
		devdrv_hwts_sq_pool_free
	);
	if (ret != 0) {
		devdrv_pool_unregist(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_SQ);
		NPU_DRV_ERR("devdrv_pool_regist error! ret = %d", ret);
		return -1;
	}

	for (i = 0; i < DEVDRV_MAX_HWTS_SQ_NUM; i++) {
		hwts_sq_info = devdrv_calc_hwts_sq_info(dev_id, i);
		hwts_sq_info->head = 0;
		hwts_sq_info->tail = 0;
		hwts_sq_info->credit = i < DEVDRV_MAX_LONG_HWTS_SQ_NUM ?
			(DEVDRV_MAX_LONG_HWTS_SQ_DEPTH - 1) : (DEVDRV_MAX_HWTS_SQ_DEPTH - 1);
		hwts_sq_info->index = (u32)i;
		hwts_sq_info->vir_addr = 0;
		hwts_sq_info->pid = DEVDRV_INVALID_FD_OR_NUM;
		hwts_sq_info->length = hwts_sq_info->credit + 1;
		hwts_sq_info->stream_num = 0;
		hwts_sq_info->send_count = 0;
	}
	return 0;
}

int devdrv_hwts_sq_destroy(u8 dev_id)
{
	int ret;
	ret = devdrv_pool_unregist(dev_id, DEVDRV_MEM_POOL_TYPE_HWTS_SQ);
	ret += devdrv_pool_unregist(dev_id, DEVDRV_MEM_POOL_TYPE_LONG_HWTS_SQ);
	return ret;
}

int devdrv_alloc_hwts_sq(u8 dev_id, u8 is_long)
{
	struct devdrv_entity_info *entity_info = NULL;
	struct devdrv_hwts_sq_info *hwts_sq_info = NULL;

	entity_info = devdrv_alloc_entity(dev_id, is_long ? DEVDRV_MEM_POOL_TYPE_LONG_HWTS_SQ : DEVDRV_MEM_POOL_TYPE_HWTS_SQ);
	if (entity_info == NULL) {
		NPU_DRV_ERR("devdrv_alloc_entry fail\n");
		return -1;
	}

	hwts_sq_info = devdrv_calc_hwts_sq_info(dev_id, entity_info->index);
	if (hwts_sq_info == NULL) {
		NPU_DRV_ERR("devdrv_calc_hwts_sq_info fail, index = %u\n", entity_info->index);
		devdrv_free_entity(dev_id, is_long ? DEVDRV_MEM_POOL_TYPE_LONG_HWTS_SQ : DEVDRV_MEM_POOL_TYPE_HWTS_SQ,
			entity_info->index);
		return -1;
	}

	hwts_sq_info->hwts_sq_sub = (u64)(uintptr_t)entity_info;
	hwts_sq_info->iova_addr = entity_info->iova;
	NPU_DRV_DEBUG("index = %u, virtaddr = %llx, iova = %llx\n", entity_info->index,
		entity_info->vir_addr, entity_info->iova);
	return entity_info->index;
}

int devdrv_free_hwts_sq(u8 dev_id, u32 hwts_sq_id)
{
	struct devdrv_hwts_sq_info *hwts_sq_info = NULL;
	int ret;

	ret = devdrv_free_entity(dev_id
		, hwts_sq_id >= DEVDRV_MAX_LONG_HWTS_SQ_NUM ? DEVDRV_MEM_POOL_TYPE_HWTS_SQ : DEVDRV_MEM_POOL_TYPE_LONG_HWTS_SQ
		, hwts_sq_id);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_free_entity error!hwts_sq_id = %u, ret = %d", hwts_sq_id, ret);
		return ret;
	}

	hwts_sq_info = devdrv_calc_hwts_sq_info(dev_id, hwts_sq_id);
	if (hwts_sq_info == NULL) {
		NPU_DRV_DEBUG("sq_sub is null\n");
		return -1;
	}
	hwts_sq_info->vir_addr = 0;
	hwts_sq_info->pid = DEVDRV_INVALID_FD_OR_NUM;
	hwts_sq_info->iova_addr = 0;
	return 0;
}

