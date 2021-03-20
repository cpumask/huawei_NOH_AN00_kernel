/*
 * npu_calc_cq.h
 *
 * about npu calc cq
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
#ifndef _NPU_CALC_CQ_H
#define _NPU_CALC_CQ_H
#include <linux/list.h>
#include <linux/spinlock.h>

#include "devdrv_user_common.h"
#include "npu_common.h"

struct devdrv_cq_sub_info {
	u32 index;
	struct list_head list;
	void *proc_ctx;  // struct devdrv_proc_ctx
	spinlock_t spinlock; /*
	                      * use for avoid the problem:
	                      * tasklet(devdrv_find_cq_index) may access cq's uio mem,
	                      * there is a delay time, between set cq's uio invalid and accessing cq's uio mem by tasklet.
	                      */
	phys_addr_t virt_addr;
	phys_addr_t phy_addr;
};

struct devdrv_sync_cq_info {
	u64 sync_cq_vaddr;
	u64 sync_cq_paddr;
	spinlock_t spinlock;
	u8 slot_size;

	DECLARE_BITMAP(sync_stream_bitmap, DEVDRV_MAX_NON_SINK_STREAM_ID);
};


int devdrv_cq_list_init(u8 dev_id);

int devdrv_cq_list_destroy(u8 dev_id);

int devdrv_inc_cq_ref_by_stream(u8 dev_id, u32 cq_id);

int devdrv_inc_cq_ref_by_communication_stream(u8 dev_id, u32 cq_id);

int devdrv_dec_cq_ref_by_stream(u8 dev_id, u32 cq_id);

int devdrv_dec_cq_ref_by_communication_stream(u8 dev_id, u32 cq_id);

int devdrv_get_cq_ref_by_stream(u8 dev_id, u32 cq_id);

int devdrv_get_cq_ref_by_communication_stream(u8 dev_id, u32 cq_id);

int devdrv_get_cq_phy_addr(u8 dev_id, u32 cq_id, phys_addr_t *phy_addr);

int npu_clear_cq(u8 dev_id, u32 cq_id);

struct devdrv_ts_cq_info *npu_alloc_cq(u8 dev_id, int pid);

int npu_free_cq(u8 dev_id, u32 cq_id);

struct devdrv_cq_sub_info *npu_get_cq_sub_addr(struct devdrv_dev_ctx *cur_dev_ctx, u32 cq_index);

#endif
