/*
 * npu_calc_sq.c
 *
 * about npu calc sq
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
#include "npu_calc_sq.h"

#include <linux/io.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/vmalloc.h>

#include "npu_shm.h"
#include "npu_log.h"

int devdrv_sq_list_init(u8 dev_id)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	struct devdrv_ts_sq_info *sq_info = NULL;
	unsigned long size;
	u32 num_sq = DEVDRV_MAX_SQ_NUM;  // need get from platform
	u32 i;

	COND_RETURN_ERROR(dev_id >= NPU_DEV_NUM, -1, "illegal npu dev id\n");

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "cur_dev_ctx %d is null\n", dev_id);

	INIT_LIST_HEAD(&cur_dev_ctx->sq_available_list);
	if (!list_empty_careful(&cur_dev_ctx->sq_available_list)) {
		NPU_DRV_ERR("sq_available_list is not empty\n");
		return -1;
	}

	cur_dev_ctx->sq_num = 0;
	size = (long)(unsigned)sizeof(struct devdrv_sq_sub_info) * num_sq;
	sq_sub_info = vmalloc(size);
	COND_RETURN_ERROR(sq_sub_info == NULL, -ENOMEM, "no mem to alloc sq sub info list\n");

	cur_dev_ctx->sq_sub_addr = (void *)sq_sub_info;

	for (i = 0; i < num_sq; i++) {
		sq_info = devdrv_calc_sq_info(dev_id, i);
		COND_RETURN_ERROR(sq_info == NULL, -ENOMEM, "no mem to alloc sq info\n");
		sq_info->head = 0;
		sq_info->tail = 0;
		sq_info->credit = DEVDRV_MAX_SQ_DEPTH - 1;
		sq_info->index = i;
		sq_info->vir_addr = NULL;
		sq_info->pid = DEVDRV_INVALID_FD_OR_NUM;
		sq_info->stream_num = 0;
		sq_info->send_count = 0;
		sq_sub_info[i].index = sq_info->index;
		sq_sub_info[i].ref_by_streams = 0;
		list_add_tail(&sq_sub_info[i].list, &cur_dev_ctx->sq_available_list);
		cur_dev_ctx->sq_num++;
	}
	NPU_DRV_DEBUG("cur dev %d own %d calc sq \n", dev_id, cur_dev_ctx->sq_num);

	return 0;
}

struct devdrv_sq_sub_info *npu_get_sq_sub_addr(struct devdrv_dev_ctx *cur_dev_ctx, u32 sq_index)
{
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	if ((sq_index >= DEVDRV_MAX_SQ_NUM) || (cur_dev_ctx->sq_sub_addr == NULL)) {
		NPU_DRV_ERR("sq_index %u is invalid or sq_sub_addr is null\n", sq_index);
		return NULL;
	}

	sq_sub_info = (struct devdrv_sq_sub_info *)cur_dev_ctx->sq_sub_addr;
	sq_sub_info = sq_sub_info + sq_index;
	if (sq_sub_info->index != sq_index) {
		NPU_DRV_ERR("sq_sub_info index %u sq index %u is not match\n",
			sq_sub_info->index, sq_index);
		return NULL;
	}
	return sq_sub_info;
}

int devdrv_alloc_sq_id(u8 dev_id)
{
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	NPU_DRV_INFO("test dev_id = %u\n", dev_id);
	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	NPU_DRV_INFO("&cur_dev_ctx->sq_available_list= %pK, dev id = %u\n", &cur_dev_ctx->sq_available_list, dev_id);
	if (list_empty_careful(&cur_dev_ctx->sq_available_list)) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_WARN("cur dev %d available sq list empty,"
			"left sq_num = %d !!!\n", dev_id, cur_dev_ctx->sq_num);
		return -1;
	}
	sq_sub = list_first_entry(&cur_dev_ctx->sq_available_list, struct devdrv_sq_sub_info, list);
	NPU_DRV_INFO("sq_sub= %pk\n", sq_sub);
	if (sq_sub == NULL || (((u64)(uintptr_t)sq_sub >> 32) == (u64)0xdead0000)) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub is NULL\n");
		return -1;
	}
	NPU_DRV_INFO("sq_sub->list next= %pk, prev= %pk\n", sq_sub->list.next, sq_sub->list.prev);
	list_del(&sq_sub->list);
	cur_dev_ctx->sq_num--;
	spin_unlock(&cur_dev_ctx->spinlock);
	NPU_DRV_DEBUG("cur dev %d alloc:%d, left %d sq\n", dev_id, sq_sub->index, cur_dev_ctx->sq_num);

	return sq_sub->index;
}

int devdrv_alloc_sq_mem(u8 dev_id, u32 sq_id, int pid)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	phys_addr_t phy_addr;
	u8 *virt_addr = NULL;
	u32 map_size;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	phy_addr = (unsigned long long)(g_sq_desc->base + (sq_id * DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE));
	map_size = DEVDRV_MAX_SQ_DEPTH * DEVDRV_RT_TASK_SIZE;
	virt_addr =(u8 *)ioremap_wc(phy_addr, map_size);
	if (virt_addr == 0) {
		NPU_DRV_ERR("cur_dev_ctx %d calc cq ioremap_wc failed \n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		iounmap((void *)(uintptr_t)virt_addr);
		NPU_DRV_ERR("sq_info get failed, sq_id:%u\n", sq_id);
		return -1;
	}
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		iounmap((void *)(uintptr_t)virt_addr);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}
	sq_sub->phy_addr = phy_addr;
	sq_sub->virt_addr = (vir_addr_t)(uintptr_t)virt_addr;
#ifdef NPU_ARCH_V200
	sq_info->vir_addr = (u8 *)virt_addr;
#else
	sq_info->vir_addr = NULL;
#endif
	spin_unlock(&cur_dev_ctx->spinlock);

	NPU_DRV_DEBUG("dev %d cur sq %d phy_addr = %pK \n", dev_id, sq_id, (void *)(uintptr_t) phy_addr);

	return 0;
}

int devdrv_get_sq_send_count(u8 dev_id, u32 sq_id, u32 *send_count)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	// struct devdrv_sq_sub_info *sq_sub;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	if (send_count == NULL) {
		NPU_DRV_ERR("send_count is null ptr\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		NPU_DRV_ERR("sq_info get failed, sq_id:%u\n", sq_id);
		return -1;
	}
	*send_count = sq_info->send_count;

	return 0;
}

int devdrv_get_sq_ref_by_stream(u8 dev_id, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info  = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		NPU_DRV_ERR("sq_info is null\n");
		return -1;
	}

	NPU_DRV_DEBUG("sq calc channel %d for ref_by_streams = %d\n",
		dev_id, sq_id, sq_info->stream_num);
	return sq_info->stream_num;
}

// get sq_id sq`s sq_addr from dev_id(must called after alloc_sq_mem)
int devdrv_get_sq_phy_addr(u8 dev_id, u32 sq_id, phys_addr_t *phy_addr)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	if (phy_addr == NULL) {
		NPU_DRV_ERR("phy_addr is null\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_info get failed, sq_id:%u\n", sq_id);
		return -1;
	}
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}
	*phy_addr = sq_sub->phy_addr;
	spin_unlock(&cur_dev_ctx->spinlock);

	NPU_DRV_DEBUG("dev %d cur sq %d phy_addr = %pK \n", dev_id, sq_id, (void *)(uintptr_t)(*phy_addr));

	return 0;
}

// sq_sub->ref_by_streams-- excute by service layer
int devdrv_free_sq_id(u8 dev_id, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_info get failed, sq_id:%u\n", sq_id);
		return -1;
	}
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}

	if (sq_sub->ref_by_streams != 0) {
		NPU_DRV_DEBUG("can't release cur_dev_ctx %d "
			"sq calc channel %d for ref_by_streams = %d\n",
			dev_id, sq_id, sq_sub->ref_by_streams);
		spin_unlock(&cur_dev_ctx->spinlock);
		return -1;
	}
	list_add(&sq_sub->list, &cur_dev_ctx->sq_available_list);
	// no stream use it
	sq_sub->ref_by_streams = 0;
	cur_dev_ctx->sq_num++;
	sq_info->head = 0;
	sq_info->tail = 0;
	sq_info->credit = DEVDRV_MAX_SQ_DEPTH - 1;
	sq_info->stream_num = 0;
	sq_info->send_count = 0;
	spin_unlock(&cur_dev_ctx->spinlock);
	NPU_DRV_DEBUG("cur dev %d own %d sq\n", dev_id, cur_dev_ctx->sq_num);
	NPU_DRV_DEBUG("update sqinfo[%d]: head:%d, tail:%d, credit:%d\n",
		sq_info->index, sq_info->head, sq_info->tail, sq_info->credit);

	return 0;
}

int devdrv_free_sq_mem(u8 dev_id, u32 sq_id)
{
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	vir_addr_t sq_virt_addr = 0;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	if (sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("illegal npu sq id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_info get failed, sq_id:%u\n", sq_id);
		return -1;
	}
	sq_info->vir_addr = NULL;
	sq_info->pid = DEVDRV_INVALID_FD_OR_NUM;
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}
	sq_sub->phy_addr = 0;
	sq_virt_addr = sq_sub->virt_addr;
	sq_sub->virt_addr = 0;
	spin_unlock(&cur_dev_ctx->spinlock);
	iounmap((void *)(uintptr_t)sq_virt_addr);

	return 0;
}

// called by alloc stream in service layer
int devdrv_inc_sq_ref_by_stream(u8 dev_id, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_info is null, sq_index = %u\n", sq_id);
		return -1;
	}
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}
	sq_info->stream_num++;  // should do it here or user driver
	sq_sub->ref_by_streams++;
	spin_unlock(&cur_dev_ctx->spinlock);

	return 0;
}

// called by free stream in service layer
int devdrv_dec_sq_ref_by_stream(u8 dev_id, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_info is null, sq_index = %u\n", sq_id);
		return -1;
	}
	sq_sub = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	if (sq_sub == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("sq_sub %u is null\n", sq_info->index);
		return -1;
	}
	sq_info->stream_num--;   // should do it here or user driver
	sq_sub->ref_by_streams--;
	spin_unlock(&cur_dev_ctx->spinlock);

	return 0;
}

int devdrv_sq_list_destroy(u8 dev_id)
{
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	if (!list_empty_careful(&cur_dev_ctx->sq_available_list)) {
		list_for_each_safe(pos, n, &cur_dev_ctx->sq_available_list) {
			cur_dev_ctx->sq_num--;
			sq_sub_info = list_entry(pos, struct devdrv_sq_sub_info, list);
			list_del(pos);
		}
	}

	vfree(cur_dev_ctx->sq_sub_addr);
	cur_dev_ctx->sq_sub_addr = NULL;

	return 0;
}

// called at 4 cases:
//   1. alloc
//   2. free
//   3. power up
//   4. power down
int npu_clear_sq_info(u8 dev_id, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	if (dev_id >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", dev_id);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	sq_info = devdrv_calc_sq_info(dev_id, sq_id);
	if (sq_info == NULL) {
		spin_unlock(&cur_dev_ctx->spinlock);
		NPU_DRV_ERR("devdrv_calc_sq_info error, sq_index = %u\n", sq_id);
		return -1;
	}
	sq_info->head = 0;
	sq_info->tail = 0;
	sq_info->credit = DEVDRV_MAX_SQ_DEPTH - 1;
	sq_info->send_count = 0;
	NPU_DRV_DEBUG("sq_info->head = 0x%x, sq_info->tail = 0x%x\n", sq_info->head, sq_info->tail);
	spin_unlock(&cur_dev_ctx->spinlock);
	NPU_DRV_DEBUG("update sqinfo[%d]: head:%d, tail:%d, credit:%d\n",
		sq_info->index, sq_info->head, sq_info->tail, sq_info->credit);
	return 0;
}

int npu_alloc_sq(u8 dev_id, int pid)
{
	int ret   = 0;
	int sq_id = devdrv_alloc_sq_id(dev_id);
	if (sq_id < 0) {
		/* V200: sq is reused between streams, if alloc failed, with warning log
		   V100: caller function prints err log if alloc sq failed */
		NPU_DRV_WARN("sq_id %d is invalid\n", sq_id);
		return -1;
	}

	ret = devdrv_alloc_sq_mem(dev_id, sq_id, pid);
	if (ret != 0) {
		devdrv_free_sq_id(dev_id, sq_id);
		NPU_DRV_ERR("alloc sq mem failed, ret:%d\n", ret);
		return -1;
	}

	ret = npu_clear_sq_info(dev_id, sq_id);
	if (ret != 0) {
		devdrv_free_sq_mem(dev_id, sq_id);
		devdrv_free_sq_id(dev_id, sq_id);
		NPU_DRV_ERR("alloc sq mem failed, ret:%d\n", ret);
		return -1;
	}

	return sq_id;
}

int npu_free_sq(u8 dev_id, u32 sq_id)
{
	int ret   = 0;
	ret = devdrv_free_sq_id(dev_id, sq_id);
	COND_RETURN_ERROR(ret != 0, -1, "free sq id failed, ret:%d\n", ret);

	ret = npu_clear_sq_info(dev_id, sq_id);
	COND_RETURN_ERROR(ret != 0, -1, "clear sq info failed, ret:%d\n", ret);

	ret = devdrv_free_sq_mem(dev_id, sq_id);
	COND_RETURN_ERROR(ret != 0, -1, "free sq mem failed, ret:%d\n", ret);

	return 0;
}
