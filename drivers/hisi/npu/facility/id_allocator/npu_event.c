/*
 * npu_event.c
 *
 * about npu event
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
#include "npu_event.h"
#include <linux/errno.h>
#include "npu_log.h"
#include "npu_common.h"
#include "npu_pm_framework.h"
#include "devdrv_user_common.h"

struct npu_id_entity *devdrv_alloc_event(u8 dev_id)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct npu_id_allocator *id_allocator = NULL;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, NULL, "invalid device id, dev_id = %u\n", dev_id);
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, NULL, "cur_dev_ctx %u is null\n", dev_id);
	id_allocator = &(cur_dev_ctx->id_allocator[NPU_ID_TYPE_EVENT]);

	return npu_id_allocator_alloc(id_allocator);
}

int devdrv_free_event_id(u8 dev_id, u32 event_id)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct npu_id_allocator *id_allocator = NULL;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -EINVAL, "invalid device id, dev_id = %u\n", dev_id);
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -ENODATA, "cur_dev_ctx %u is null\n", dev_id);
	id_allocator = &(cur_dev_ctx->id_allocator[NPU_ID_TYPE_EVENT]);

	return npu_id_allocator_free(id_allocator, event_id);
}
