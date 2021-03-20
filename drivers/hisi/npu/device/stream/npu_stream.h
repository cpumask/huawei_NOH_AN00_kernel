/*
 * npu_stream.h
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
#ifndef __NPU_STREAM_H
#define __NPU_STREAM_H

#include <linux/list.h>
#include "npu_id_allocator.h"

struct devdrv_stream_sub_extra_info {
	wait_queue_head_t sync_report_wait;
};

int devdrv_stream_list_init(struct devdrv_dev_ctx *dev_ctx);

int devdrv_alloc_stream_id(u8 dev_id);

int devdrv_free_stream_id(u8 dev_id, u32 stream_id);

int devdrv_bind_stream_with_sq(u8 dev_id, u32 stream_id, u32 sq_id);

int devdrv_bind_stream_with_cq(u8 dev_id, u32 stream_id, u32 cq_id);

struct npu_id_entity *devdrv_get_non_sink_stream_sub_addr(struct devdrv_dev_ctx *dev_ctx, u32 stream_id);

#endif
