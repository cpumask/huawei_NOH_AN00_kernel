/*
 * npu_stream.c
 *
 * about npu stream
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
#include "npu_stream.h"

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#include "devdrv_user_common.h"
#include "npu_shm.h"
#include "npu_log.h"
#include "npu_platform.h"

int devdrv_stream_list_init(struct devdrv_dev_ctx *dev_ctx)
{
	int ret;
	struct npu_id_entity *stream_sub = NULL;
	struct devdrv_stream_info *stream_info = NULL;
	struct npu_id_allocator *id_allocator = NULL;
	struct devdrv_platform_info* plat_info = NULL;
	u32 i;

	COND_RETURN_ERROR(dev_ctx == NULL, -ENODATA, "dev_ctx is null\n");

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info\n");
		return -EINVAL;
	}

	id_allocator = &(dev_ctx->id_allocator[NPU_ID_TYPE_STREAM]);
	ret = npu_id_allocator_create(id_allocator, 0, DEVDRV_MAX_NON_SINK_STREAM_ID, sizeof(struct devdrv_stream_sub_extra_info));
	if (ret != 0) {
		NPU_DRV_ERR("stream id regist failed \n");
		return -1;
	}

	for (i = 0; i < DEVDRV_MAX_NON_SINK_STREAM_ID; i++) {
		stream_sub = (struct npu_id_entity *)((uintptr_t)id_allocator->id_entity_base_addr +
			id_allocator->entity_size * i);
		stream_info = devdrv_calc_stream_info(dev_ctx->devid, i);
		if (stream_info == NULL) {
			NPU_DRV_ERR("devdrv_calc_stream_info is null, failed\n");
			return -EINVAL;
		}
		stream_info->id = i;
		stream_info->devid = dev_ctx->devid;
		stream_info->cq_index = (u32) DEVDRV_CQSQ_INVALID_INDEX;
		stream_info->sq_index = (u32) DEVDRV_CQSQ_INVALID_INDEX;

		if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
			stream_info->smmu_substream_id = 0;
			stream_info->priority = 0;
		}
		init_waitqueue_head(&(((struct devdrv_stream_sub_extra_info*)(stream_sub->data))->sync_report_wait));
	}

	return 0;
}

struct npu_id_entity *devdrv_get_non_sink_stream_sub_addr(struct devdrv_dev_ctx *dev_ctx, u32 stream_id)
{
	struct npu_id_entity *stream_sub = NULL;
	struct npu_id_allocator *id_allocator = NULL;

	if (stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID) {
		NPU_DRV_ERR("stream id %u is invalid\n", stream_id);
		return NULL;
	}
	id_allocator = &(dev_ctx->id_allocator[NPU_ID_TYPE_STREAM]);
	stream_sub = (struct npu_id_entity *)(
		(uintptr_t)id_allocator->id_entity_base_addr +
		id_allocator->entity_size * stream_id);

	if (stream_sub->id != stream_id) {
		NPU_DRV_ERR("stream_sub_info id %u stream_id %u is not match\n",
			stream_sub->id, stream_id);
		return NULL;
	}

	return stream_sub;
}

int devdrv_alloc_stream_id(u8 dev_id)
{
	struct npu_id_entity *stream_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct npu_id_allocator *id_allocator = NULL;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "invalid device id, dev_id = %u\n", dev_id);
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "cur_dev_ctx %u is null\n", dev_id);
	id_allocator = &(cur_dev_ctx->id_allocator[NPU_ID_TYPE_STREAM]);

	stream_sub_info = npu_id_allocator_alloc(id_allocator);
	if (stream_sub_info == NULL) {
		NPU_DRV_ERR("alloc stream id failed\n");
		return -1;
	}

	return stream_sub_info->id;
}

int devdrv_free_stream_id(u8 dev_id, u32 stream_id)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct npu_id_allocator *id_allocator = NULL;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -EINVAL, "invalid device id, dev_id = %u\n", dev_id);
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -ENODATA, "cur_dev_ctx %u is null\n", dev_id);
	id_allocator = &(cur_dev_ctx->id_allocator[NPU_ID_TYPE_STREAM]);

	return npu_id_allocator_free(id_allocator, stream_id);
}

int devdrv_bind_stream_with_sq(u8 dev_id, u32 stream_id, u32 sq_id)
{
	struct devdrv_stream_info *stream_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID) {
		NPU_DRV_ERR("illegal npu stream id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal sq id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}
	spin_lock(&cur_dev_ctx->spinlock);
	stream_info = devdrv_calc_stream_info(dev_id, stream_id);
	if (stream_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("stream_info is null, stream_id :%d\n", stream_id);
		return -1;
	}
	stream_info->sq_index = sq_id;
	spin_unlock(&cur_dev_ctx->spinlock);

	return 0;
}

// called by alloc complete stream in service layer
int devdrv_bind_stream_with_cq(u8 dev_id, u32 stream_id, u32 cq_id)
{
	struct devdrv_stream_info *stream_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID) {
		NPU_DRV_ERR("illegal npu stream id\n");
		return -1;
	}

	if (cq_id >= DEVDRV_MAX_CQ_NUM) {
		NPU_DRV_ERR("illegal cq id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}
	spin_lock(&cur_dev_ctx->spinlock);
	stream_info = devdrv_calc_stream_info(dev_id, stream_id);
	if (stream_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("stream_info is null, stream_id :%d\n", stream_id);
		return -1;
	}
	stream_info->cq_index = cq_id;
	/* sync_cq_index = cq_num + dev_num */
	stream_info->sync_cq_index = DEVDRV_MAX_CQ_NUM + dev_id;
	spin_unlock(&cur_dev_ctx->spinlock);

	return 0;
}
