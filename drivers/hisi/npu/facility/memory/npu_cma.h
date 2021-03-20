/*
 * npu_cma.h
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
#ifndef __NPU_CMA_H
#define __NPU_CMA_H

#include <linux/mm.h>
#include <linux/dma-mapping.h>

#include "npu_common.h"
#include "npu_proc_ctx.h"

#define MEM_INFO_VALID       0xa5a5a5a5
#define MEM_TOTAL_SIZE       0x800000
#define MEM_INIT_SIZE        0x200000
#define MAP_MAX_SIZE         0x200000
#define SHARE_NUM_OCCUPIED   0
#define L2_MAP_ALIGN         2

struct devdrv_cm_msg {
	u32 share_num;
	u64 size;
	u64 user_va;
};

int devdrv_continuous_mem_init(u8 dev_id);

int devdrv_map_cm(const struct devdrv_proc_ctx *proc_ctx, struct vm_area_struct *vma, u32 share_num, u8 dev_id);

int devdrv_save_cm_info(struct devdrv_dev_ctx *dev_ctx, struct devdrv_cm_info *info, u32 *share_num);

int devdrv_save_cm_info_occupied(struct devdrv_dev_ctx *dev_ctx, u32 *share_num);

int devdrv_delete_cm_info(struct devdrv_dev_ctx *dev_ctx, u32 share_num);

int devdrv_delete_cm_info_occupied(struct devdrv_dev_ctx *dev_ctx);

int devdrv_is_cm_available(const struct devdrv_dev_ctx *dev_ctx, u64 need_size);

int devdrv_inc_cm_total_size(struct devdrv_dev_ctx *dev_ctx, u64 size);

int devdrv_dec_cm_total_size(struct devdrv_dev_ctx *dev_ctx, u64 size);

int devdrv_is_cm_valid(const struct devdrv_dev_ctx *dev_ctx, u32 share_num);

int devdrv_cm_resource_recycle(struct devdrv_dev_ctx *dev_ctx);

#endif
