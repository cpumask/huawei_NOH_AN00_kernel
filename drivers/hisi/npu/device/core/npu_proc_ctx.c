/*
 * npu_proc_ctx.c
 *
 * about npu proc ctx
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
#include "npu_proc_ctx.h"

#include <asm/barrier.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <securec.h>

#include "npu_shm.h"
#include "npu_calc_channel.h"
#include "npu_calc_cq.h"
#include "npu_stream.h"
#include "npu_cache.h"
#include "npu_log.h"
#include "npu_event.h"
#include "npu_hwts_event.h"
#include "npu_model.h"
#include "npu_task.h"
#include "npu_platform.h"
#include "npu_calc_sq.h"
#include "npu_hwts.h"
#include "npu_adapter.h"
#include "npu_pm_framework.h"
#include "npu_sink_stream.h"

static struct devdrv_cq_report_int_ctx g_cq_int_ctx;
static u64 g_recv_cq_int_num = 0; // use for debug
static u64 g_find_cq_index_called_num = 0; // use for debug

int devdrv_proc_ctx_init(struct devdrv_proc_ctx *proc_ctx)
{
	INIT_LIST_HEAD(&proc_ctx->cq_list);
	INIT_LIST_HEAD(&proc_ctx->sq_list);
	INIT_LIST_HEAD(&proc_ctx->sink_stream_list);
	INIT_LIST_HEAD(&proc_ctx->stream_list);
	INIT_LIST_HEAD(&proc_ctx->event_list);
	INIT_LIST_HEAD(&proc_ctx->hwts_event_list);
	INIT_LIST_HEAD(&proc_ctx->model_list);
	INIT_LIST_HEAD(&proc_ctx->task_list);
	INIT_LIST_HEAD(&proc_ctx->dev_ctx_list);
	INIT_LIST_HEAD(&proc_ctx->message_list_header);
	INIT_LIST_HEAD(&proc_ctx->ipc_msg_send_head);
	INIT_LIST_HEAD(&proc_ctx->ipc_msg_return_head);
	INIT_LIST_HEAD(&proc_ctx->l2_vma_list);
	INIT_LIST_HEAD(&proc_ctx->cma_vma_list);
	INIT_LIST_HEAD(&proc_ctx->sqcq_vma_list);

	proc_ctx->pid = current->tgid;
	proc_ctx->sink_stream_num = 0;
	proc_ctx->stream_num = 0;
	proc_ctx->event_num = 0;
	proc_ctx->hwts_event_num = 0;
	proc_ctx->cq_num = 0;
	proc_ctx->sq_num = 0;
	proc_ctx->model_num = 0;
	proc_ctx->task_num = 0;
	proc_ctx->send_count = 0;
	proc_ctx->receive_count = 0;
	proc_ctx->proc_ctx_sub = NULL;
	proc_ctx->ipc_port = -1;
	proc_ctx->cq_tail_updated = 0;
	proc_ctx->should_stop_thread = 0;
	proc_ctx->mailbox_message_count.counter = 0;
	proc_ctx->map_ops_flag = false;
	proc_ctx->filep = NULL;
	proc_ctx->sq_round_index = -1;

	init_waitqueue_head(&proc_ctx->report_wait);
	init_waitqueue_head(&proc_ctx->ipc_wait);
	spin_lock_init(&proc_ctx->mailbox_wait_spinlock);
	sema_init(&proc_ctx->mailbox_wait, 0);
	mutex_init(&proc_ctx->stream_mutex);
	mutex_init(&proc_ctx->event_mutex);
	mutex_init(&proc_ctx->model_mutex);
	mutex_init(&proc_ctx->task_mutex);
	mutex_init(&proc_ctx->map_mutex);
	bitmap_zero(proc_ctx->stream_bitmap, DEVDRV_MAX_STREAM_ID);
	bitmap_zero(proc_ctx->event_bitmap, DEVDRV_MAX_EVENT_ID);
	bitmap_zero(proc_ctx->hwts_event_bitmap, DEVDRV_MAX_HWTS_EVENT_ID);
	bitmap_zero(proc_ctx->model_bitmap, DEVDRV_MAX_MODEL_ID);
	bitmap_zero(proc_ctx->task_bitmap, DEVDRV_MAX_SINK_TASK_ID);

	return devdrv_proc_ctx_sub_init(proc_ctx);
}

void devdrv_proc_ctx_destroy(struct devdrv_proc_ctx **proc_ctx_ptr)
{
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info\n");
		return;
	}

	if (proc_ctx_ptr == NULL) {
		NPU_DRV_ERR("proc_ctx_ptr is NULL\n");
		return;
	}

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1)
		devdrv_proc_ctx_sub_destroy(*proc_ctx_ptr);

	kfree(*proc_ctx_ptr);
	*proc_ctx_ptr = NULL;
	return;
}

struct devdrv_ts_cq_info *devdrv_proc_alloc_cq(struct devdrv_proc_ctx *proc_ctx)
{
	struct devdrv_ts_cq_info *cq_info = NULL;
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	u8 dev_id;

	if (proc_ctx == NULL) {
		NPU_DRV_ERR("proc_ctx is null\n");
		return NULL;
	}
	dev_id = proc_ctx->devid;
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, NULL, "invalid dev id %u\n", dev_id);
	cq_info = npu_alloc_cq(dev_id, proc_ctx->pid);
	if (cq_info == NULL) {
		NPU_DRV_ERR("npu dev %d cq_info is null\n", dev_id);
		return NULL;
	}

	cq_sub_info = npu_get_cq_sub_addr(cur_dev_ctx, cq_info->index);
	COND_RETURN_ERROR(cq_sub_info == NULL, NULL, "invalid para cq info index %u\n",
		cq_info->index);
	list_add(&cq_sub_info->list, &proc_ctx->cq_list);
	cq_sub_info->proc_ctx = proc_ctx;
	proc_ctx->cq_num++;

	return cq_info;
}

int npu_proc_get_cq_id(struct devdrv_proc_ctx *proc_ctx, u32 *cq_id)
{
	struct devdrv_cq_sub_info *cq_sub = NULL;

	if (list_empty_careful(&proc_ctx->cq_list)) {
		NPU_DRV_ERR("cur process %d available cq list empty,"
			"left cq_num = %d !!!\n", proc_ctx->pid, proc_ctx->cq_num);
		return -1;
	}

	cq_sub = list_first_entry(&proc_ctx->cq_list, struct devdrv_cq_sub_info, list);
	COND_RETURN_ERROR(cq_sub == NULL, -1, "invalid cq_sub\n");

	if (cq_sub->index >= DEVDRV_MAX_CQ_NUM) {
		NPU_DRV_ERR("invalid cq id = %u\n", cq_sub->index);
		return -1;
	}
	*cq_id = cq_sub->index;
	return 0;
}

int devdrv_proc_send_alloc_stream_mailbox(struct devdrv_proc_ctx *proc_ctx)
{
	struct npu_id_entity *stream_sub = NULL;
	struct devdrv_stream_info *stream_info = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	u8 cur_dev_id = 0; // get from platform
	int ret;

	if (list_empty_careful(&proc_ctx->stream_list)) {
		NPU_DRV_ERR("proc context stream list is empty\n");
		return -1;
	}

	list_for_each_safe(pos, n, &proc_ctx->stream_list) {
		stream_sub = list_entry(pos, struct npu_id_entity, list);
		if (stream_sub == NULL) {
			NPU_DRV_ERR("stream sub is null\n");
			return -1;
		}

		stream_info = devdrv_calc_stream_info(cur_dev_id, stream_sub->id);
		COND_RETURN_ERROR(stream_info == NULL, -1, "illegal npu stream info is null\n");
		if (stream_info->strategy & STREAM_STRATEGY_SINK) {
			NPU_DRV_DEBUG("send no mailbox for sink stream\n");
			continue;
		}

		ret = devdrv_send_alloc_stream_mailbox(cur_dev_id, stream_sub->id, stream_info->cq_index);
		if (ret) {
			NPU_DRV_ERR("send alloc stream %d mailbox failed\n", stream_info->id);
			return ret;
		}
	}

	return 0;
}

int npu_proc_clear_sqcq_info(struct devdrv_proc_ctx *proc_ctx)
{
	struct npu_id_entity *stream_sub = NULL;
	struct devdrv_stream_info *stream_info = NULL;
	struct devdrv_cq_sub_info *cq_sub = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	u8 cur_dev_id = 0; // get from platform

	if (list_empty_careful(&proc_ctx->cq_list)) {
		NPU_DRV_DEBUG("proc context cq list is empty, no need clear\n");
		return 0;
	}

	list_for_each_safe(pos, n, &proc_ctx->cq_list) {
		cq_sub = list_entry(pos, struct devdrv_cq_sub_info, list);
		if (cq_sub != NULL)
			(void)npu_clear_cq(cur_dev_id, cq_sub->index);
	}

	if (list_empty_careful(&proc_ctx->stream_list)) {
		NPU_DRV_DEBUG("proc context sq list is empty, no need clear\n");
		return 0;
	}

	list_for_each_safe(pos, n, &proc_ctx->stream_list) {
		stream_sub = list_entry(pos, struct npu_id_entity, list);
		if (stream_sub != NULL) {
			stream_info = devdrv_calc_stream_info(cur_dev_id, stream_sub->id);
			COND_RETURN_ERROR(stream_info == NULL, -1, "illegal npu stream_info is null\n");
			if (stream_info->strategy & STREAM_STRATEGY_SINK) {
				NPU_DRV_DEBUG("send no mailbox for sink stream\n");
				continue;
			}
			NPU_DRV_DEBUG("stream_info->sq_index = 0x%x\n", stream_info->sq_index);
			(void)npu_clear_sq_info(cur_dev_id, stream_info->sq_index);
		}
	}

	return 0;
}

void devdrv_proc_clr_synccq_info(struct devdrv_sync_cq_info *sync_cq_info, uint16_t stream_id)
{
	struct devdrv_report *report = (struct devdrv_report*)(uintptr_t)(sync_cq_info->sync_cq_vaddr +
		(long)(unsigned)(sync_cq_info->slot_size * stream_id));

	NPU_DRV_DEBUG("clear sync cq info by stream:stream_id=%u task_id=%u phase=%u result=%llx\n",
		report->stream_id, report->task_id, report->phase, report->pay_load);
	memset_s(report, sizeof(struct devdrv_report), 0, sizeof(struct devdrv_report));

	return;
}

static inline int devdrv_proc_aval_sq_push(struct devdrv_proc_ctx *proc_ctx, u32 sq_id)
{
	struct devdrv_ts_sq_info *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	COND_RETURN_ERROR(proc_ctx == NULL, -1, "proc_ctx ptr is NULL\n");
	cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "invalid dev id %u\n", proc_ctx->devid);
	sq_info = devdrv_calc_sq_info(proc_ctx->devid, sq_id);
	COND_RETURN_ERROR(sq_info == NULL, -1, "sq_info ptr is NULL\n");
	sq_sub_info = npu_get_sq_sub_addr(cur_dev_ctx, sq_info->index);
	COND_RETURN_ERROR(sq_sub_info == NULL, -1, "invalid para sq info index %u\n", sq_info->index);

	list_add_tail(&sq_sub_info->list, &proc_ctx->sq_list);
	proc_ctx->sq_num++;

	NPU_DRV_DEBUG("proc_ctx->sq_num:%u sq_id:%u\n", proc_ctx->sq_num, sq_id);
	return 0;
}

static inline int devdrv_proc_aval_sq_pop(struct devdrv_proc_ctx *proc_ctx, u32 sq_id)
{
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	struct list_head *pos  = NULL;

	COND_RETURN_ERROR(proc_ctx == NULL, -1, "proc_ctx ptr is NULL\n");
	list_for_each(pos, &proc_ctx->sq_list) {
		sq_sub_info = container_of(pos, struct devdrv_sq_sub_info, list);
		if (sq_sub_info->index == sq_id) {
			break;
		}
	}

	list_del(&sq_sub_info->list);
	proc_ctx->sq_num--;
	NPU_DRV_DEBUG("proc_ctx->sq_num:%d sq_id:%d\n", proc_ctx->sq_num, sq_id);
	return 0;
}

static inline int devdrv_proc_alloc_aval_sq_round(struct devdrv_proc_ctx *proc_ctx)
{
	int i;
	struct list_head *pos  = NULL;
	struct devdrv_sq_sub_info *sq_sub = NULL;

	COND_RETURN_ERROR(proc_ctx == NULL, -1, "proc_ctx ptr is NULL\n");
	if (list_empty_careful(&proc_ctx->sq_list)) {
		NPU_DRV_ERR("sq list in curr proc is NULL\n");
		return -1;
	}

	COND_RETURN_ERROR(proc_ctx->sq_num == 0, -1, "npu sq_num is illegal\n");
	proc_ctx->sq_round_index = (proc_ctx->sq_round_index + 1) % proc_ctx->sq_num;

	NPU_DRV_DEBUG("proc_ctx->sq_round_index:%d \n", proc_ctx->sq_round_index);
	for (i = 0, pos = &(proc_ctx->sq_list); i < proc_ctx->sq_round_index; i++, pos = pos->next);

	sq_sub = list_first_entry(pos, struct devdrv_sq_sub_info, list);

	NPU_DRV_DEBUG("sq_sub->index:%d \n", sq_sub->index);
	return sq_sub->index;
}

static inline int devdrv_proc_free_aval_sq_round(struct devdrv_proc_ctx *proc_ctx)
{
	COND_RETURN_ERROR(proc_ctx->sq_round_index <= -1, -1, "npu sq_round_index:%d is illegal\n",
		proc_ctx->sq_round_index);
	proc_ctx->sq_round_index = proc_ctx->sq_round_index - 1;
	return 0;
}

/*
 * v200:transmit hwts_cq_id to sink stream, and cq_id to non-sink stream
 * v100:transmit cq_id to nonsink_stream and sink_stream
 * [caustion] stream cq id is proc wide used without resource allocin or freein,
              and there is no relase func corresponding to this one
 */
static inline int devdrv_proc_get_stream_cq_id(struct devdrv_proc_ctx *proc_ctx, u32 strategy, u32 *stream_cq_id)
{
	int ret;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "plat_info ptr is NULL\n");

	COND_RETURN_ERROR(stream_cq_id == NULL, -1, "stream_cq_id ptr is NULL\n");
	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1 && (strategy & STREAM_STRATEGY_SINK))
		ret = devdrv_proc_get_hwts_cq_id(proc_ctx, stream_cq_id);
	else
		ret = npu_proc_get_cq_id(proc_ctx, stream_cq_id);
	if (ret != 0) {
		NPU_DRV_ERR("get cq_id from proc_ctx cq_list failed, strategy:%u\n", strategy);
		return -1;
	}
	return 0;
}


int devdrv_proc_get_stream_sq_id(struct devdrv_proc_ctx *proc_ctx, u32 strategy,
		u32 *stream_sq_id, u32 *stream_sq_used_round)
{
	int sq_id      = 0;
	int hwts_sq_id = 0;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "plat_info ptr is NULL\n");

	COND_RETURN_ERROR(stream_sq_id == NULL, -1, "stream_sq_id ptr is NULL\n");
	COND_RETURN_ERROR(stream_sq_used_round == NULL, -1, "stream_sq_used_round ptr is NULL\n");

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
		if (strategy & STREAM_STRATEGY_SINK) {
			hwts_sq_id = devdrv_alloc_hwts_sq(proc_ctx->devid, strategy & STREAM_STRATEGY_LONG);
			COND_RETURN_ERROR(hwts_sq_id < 0, -1, "alloc hwts_sq from dev %d failed\n", proc_ctx->devid);
			*stream_sq_id = (u32)hwts_sq_id;
			return 0;
		}

		sq_id = npu_alloc_sq(proc_ctx->devid, proc_ctx->pid);
		if (sq_id >= 0) { /* alloc sq from dev */
			*stream_sq_id = (u32)sq_id;
			(void)devdrv_proc_aval_sq_push(proc_ctx, *stream_sq_id);
			return 0;
		}

		sq_id = devdrv_proc_alloc_aval_sq_round(proc_ctx); /* alloc sq from proc */
		COND_RETURN_ERROR(sq_id < 0, -1, "alloc sq from dev %d failed\n", proc_ctx->devid);

		*stream_sq_used_round = 1;
		*stream_sq_id = (u32)sq_id;
		NPU_DRV_WARN("sq list in curr dev has been exhausted, stream sq_id:%d is used round\n", *stream_sq_id);
	}
	else {
		if (strategy & STREAM_STRATEGY_SINK)
			return 0;
		sq_id = npu_alloc_sq(proc_ctx->devid, proc_ctx->pid); /* alloc sq */
		COND_RETURN_ERROR(sq_id < 0, -1, "alloc sq from dev %d failed\n", proc_ctx->devid);
		*stream_sq_id = (u32)sq_id;
	}
	return 0;
}

int devdrv_proc_revert_stream_sq_id(struct devdrv_proc_ctx *proc_ctx, u32 strategy,
		u32 stream_sq_id, u32 stream_sq_used_round)
{
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "plat_info ptr is NULL\n");

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
		if (strategy & STREAM_STRATEGY_SINK) { /* hwtssq is bind to sink-stream */
			devdrv_free_hwts_sq(proc_ctx->devid, stream_sq_id);
			return 0;
		}
		if (stream_sq_used_round == 0) {
			devdrv_proc_aval_sq_pop(proc_ctx, stream_sq_id);
			npu_free_sq(proc_ctx->devid, stream_sq_id);
		} else {
			devdrv_proc_free_aval_sq_round(proc_ctx);
		}
	} else {
		if (strategy & STREAM_STRATEGY_SINK)
			return 0;
		npu_free_sq(proc_ctx->devid, stream_sq_id);
	}
	return 0;
}

int devdrv_proc_release_stream_sq_id(struct devdrv_proc_ctx *proc_ctx, u32 strategy,
		u32 stream_sq_id)
{
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "plat_info ptr is NULL\n");

	if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
		if (strategy & STREAM_STRATEGY_SINK) {/* hwtssq is bind to sink-stream */
			devdrv_free_hwts_sq(proc_ctx->devid, stream_sq_id);
			return 0;
		}
		if (devdrv_get_sq_ref_by_stream(proc_ctx->devid, stream_sq_id) == 0) {
			devdrv_proc_aval_sq_pop(proc_ctx, stream_sq_id);
			npu_free_sq(proc_ctx->devid, stream_sq_id);
		}
	} else {
		if (strategy & STREAM_STRATEGY_SINK)
			return 0;
		npu_free_sq(proc_ctx->devid, stream_sq_id);
	}
	return 0;
}

// protect by proc_ctx->stream_mutex when called
int devdrv_proc_alloc_stream(struct devdrv_proc_ctx *proc_ctx, struct devdrv_stream_info **stream_info,
	u16 strategy, u16 priority)
{
	int ret;
	u32 stream_cq_id = 0;
	u32 stream_sq_id = 0;
	u32 stream_sq_used_round = 0;
	struct devdrv_dev_ctx *dev_ctx = NULL;
	struct npu_id_entity *stream_sub_info = NULL;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();

	COND_RETURN_ERROR(plat_info == NULL, -EINVAL, "devdrv_plat_get_info is NULL\n");
	COND_RETURN_ERROR(proc_ctx == NULL, -1, "proc_ctx ptr is NULL\n");
	COND_RETURN_ERROR(stream_info == NULL, -1, "stream_info pptr is NULL\n");

	ret = devdrv_proc_get_stream_cq_id(proc_ctx, strategy, &stream_cq_id);
	COND_RETURN_ERROR(ret != 0, -1, "get stream_cq_id failed, strategy:%d\n", strategy);
	ret = devdrv_proc_get_stream_sq_id(proc_ctx, strategy, &stream_sq_id, &stream_sq_used_round);
	COND_RETURN_ERROR(ret != 0, -1, "get stream_sq_id failed, strategy:%d\n", strategy);

	NPU_DRV_DEBUG("stream sq_id:%d cq_id:%d, strategy:%d\n", stream_sq_id, stream_cq_id, strategy);

	*stream_info = devdrv_alloc_stream(strategy, stream_sq_id, stream_cq_id);
	if (*stream_info == NULL) {
		devdrv_proc_revert_stream_sq_id(proc_ctx, strategy, stream_sq_id, stream_sq_used_round);
		NPU_DRV_ERR("get stream_info through cq %d failed \n", stream_cq_id);
		return -1;
	}
	(*stream_info)->pid = proc_ctx->pid;
	(*stream_info)->create_tid = current->pid;
	(*stream_info)->priority = (priority > DEVDRV_MAX_STREAM_PRIORITY) ? DEVDRV_MAX_STREAM_PRIORITY : priority;
	NPU_DRV_DEBUG("alloc stream success stream_id= %d, strategy= %u, stream_sq_id= %d, stream_cq_id= %d\n",
		(*stream_info)->id, strategy, (*stream_info)->sq_index, (*stream_info)->cq_index);


	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -1, "invalid dev_ctx\n");
	if (strategy & STREAM_STRATEGY_SINK) {
		stream_sub_info = devdrv_get_sink_stream_sub_addr(dev_ctx, (*stream_info)->id);
		COND_RETURN_ERROR(stream_sub_info == NULL, -1, "invalid stream_sub_info id %d\n", (*stream_info)->id);
		list_add(&stream_sub_info->list, &proc_ctx->sink_stream_list);
		proc_ctx->sink_stream_num++;
	} else {
		stream_sub_info = devdrv_get_non_sink_stream_sub_addr(dev_ctx, (*stream_info)->id);
		COND_RETURN_ERROR(stream_sub_info == NULL, -1, "invalid stream_sub_info id %d\n", (*stream_info)->id);
		list_add(&stream_sub_info->list, &proc_ctx->stream_list);
		proc_ctx->stream_num++;

		devdrv_proc_clr_synccq_info((struct devdrv_sync_cq_info *)dev_ctx->sync_cq, (uint16_t)(*stream_info)->id);
	}

	NPU_DRV_DEBUG("npu process_id = %d thread_id %d own sink stream num = %d, non sink stream num = %d now \n",
		proc_ctx->pid, current->pid, proc_ctx->sink_stream_num, proc_ctx->stream_num);

	return 0;
}

static int npu_proc_alloc_ts_event(struct devdrv_proc_ctx *proc_ctx, u32 *event_id_ptr)
{
	struct npu_id_entity *event_info = devdrv_alloc_event(proc_ctx->devid);
	if (event_info == NULL) {
		NPU_DRV_ERR("event info is null\n");
		*event_id_ptr = DEVDRV_MAX_EVENT_ID;
		return -ENODATA;
	}

	list_add(&event_info->list, &proc_ctx->event_list);
	proc_ctx->event_num++;
	NPU_DRV_DEBUG("npu process %d own event id = %u num = %d now \n", current->pid, event_info->id, proc_ctx->event_num);

	*event_id_ptr = event_info->id;
	return 0;
}

static int npu_proc_alloc_hwts_event(struct devdrv_proc_ctx *proc_ctx, u32 *event_id_ptr)
{
	struct npu_id_entity *event_info = npu_alloc_hwts_event(proc_ctx->devid);
	if (event_info == NULL) {
		NPU_DRV_ERR("event info is null\n");
		*event_id_ptr = DEVDRV_MAX_HWTS_EVENT_ID;
		return -ENODATA;
	}

	list_add(&event_info->list, &proc_ctx->hwts_event_list);
	proc_ctx->hwts_event_num++;
	NPU_DRV_DEBUG("npu process %d own hwts event id = %u num = %d now \n", current->pid, event_info->id, proc_ctx->hwts_event_num);

	*event_id_ptr = event_info->id;
	return 0;
}

static int npu_proc_free_ts_event(struct devdrv_proc_ctx *proc_ctx, u32 event_id)
{
	int ret;

	if (event_id >= DEVDRV_MAX_EVENT_ID || proc_ctx->event_num == 0) {
		NPU_DRV_ERR("invalid event_id is %u or event_num is %u\n", event_id, proc_ctx->event_num);
		return -EINVAL;
	}

	ret = devdrv_free_event_id(proc_ctx->devid, event_id);
	if (ret != 0) {
		NPU_DRV_ERR("free event id failed\n");
		return -ENODATA;
	}

	proc_ctx->event_num--;
	bitmap_clear(proc_ctx->event_bitmap, event_id, 1);
	NPU_DRV_DEBUG("npu process %d free event id = %u, left num = %d\n", current->pid, event_id, proc_ctx->event_num);
	return 0;
}

static int npu_proc_free_hwts_event(struct devdrv_proc_ctx *proc_ctx, u32 event_id)
{
	int ret;

	if (event_id >= DEVDRV_MAX_HWTS_EVENT_ID || proc_ctx->hwts_event_num == 0) {
		NPU_DRV_ERR("invalid hwts event_id is %u or hwts_event_num is %u\n", event_id, proc_ctx->hwts_event_num);
		return -EINVAL;
	}

	ret = npu_free_hwts_event_id(proc_ctx->devid, event_id);
	if (ret != 0) {
		NPU_DRV_ERR("free hwts event id failed\n");
		return -ENODATA;
	}

	proc_ctx->hwts_event_num--;
	bitmap_clear(proc_ctx->hwts_event_bitmap, event_id, 1);
	NPU_DRV_DEBUG("npu process %d free hwts event id = %u, left num = %d\n", current->pid, event_id, proc_ctx->hwts_event_num);
	return 0;
}

int devdrv_proc_alloc_event(struct devdrv_proc_ctx *proc_ctx, u32* event_id_ptr, u16 strategy)
{
	int ret;
	if (proc_ctx == NULL || event_id_ptr == NULL) {
		NPU_DRV_ERR("proc_ctx ptr or event id ptr is null \n");
		return -EINVAL;
	}

	if (strategy == EVENT_STRATEGY_TS) {
		NPU_DRV_DEBUG("alloc v100 event, v200 nonsink event \n");
		ret = npu_proc_alloc_ts_event(proc_ctx, event_id_ptr);
	} else {
		ret = npu_proc_alloc_hwts_event(proc_ctx, event_id_ptr);
	}

	return ret;
}

int devdrv_proc_free_event(struct devdrv_proc_ctx *proc_ctx, u32 event_id, u16 strategy)
{
	int ret;
	if (proc_ctx == NULL) {
		NPU_DRV_ERR("proc_ctx ptr is null\n");
		return -EINVAL;
	}

	if (strategy == EVENT_STRATEGY_TS)
		ret = npu_proc_free_ts_event(proc_ctx, event_id);
	 else
		ret = npu_proc_free_hwts_event(proc_ctx, event_id);

	return ret;
}

int devdrv_proc_alloc_model(struct devdrv_proc_ctx *proc_ctx, u32* model_id_ptr)
{
	struct npu_id_entity *model_info = NULL;

	if (proc_ctx == NULL || model_id_ptr == NULL) {
		NPU_DRV_ERR("proc_ctx ptr or model id ptr is null \n");
		return -EINVAL;
	}

	model_info = devdrv_alloc_model(proc_ctx->devid);
	if (model_info == NULL) {
		NPU_DRV_ERR("model info is null\n");
		*model_id_ptr = DEVDRV_MAX_MODEL_ID;
		return -ENODATA;
	}

	list_add(&model_info->list, &proc_ctx->model_list);
	proc_ctx->model_num++;
	NPU_DRV_DEBUG("npu process %d own model id = %u num = %d now \n", current->pid, model_info->id, proc_ctx->model_num);

	*model_id_ptr = model_info->id;
	return 0;
}

int devdrv_proc_free_model(struct devdrv_proc_ctx *proc_ctx, u32 model_id)
{
	int ret;

	if (proc_ctx == NULL || model_id >= DEVDRV_MAX_MODEL_ID) {
		NPU_DRV_ERR("proc_ctx ptr or model id ptr is null \n");
		return -EINVAL;
	}

	if (proc_ctx->model_num == 0) {
		NPU_DRV_ERR("model_num is 0 \n");
		return -EINVAL;
	}

	ret = devdrv_free_model_id(proc_ctx->devid, model_id);
	if (ret != 0) {
		NPU_DRV_ERR("free model id failed\n");
		return -ENODATA;
	}

	proc_ctx->model_num--;
	bitmap_clear(proc_ctx->model_bitmap, model_id, 1);
	NPU_DRV_DEBUG("npu process %d free model id = %u, left num = %d", current->pid, model_id, proc_ctx->model_num);

	return 0;
}

int devdrv_proc_alloc_task(struct devdrv_proc_ctx *proc_ctx, u32* task_id_ptr)
{
	struct npu_id_entity *task_info = NULL;

	if (proc_ctx == NULL || task_id_ptr == NULL) {
		NPU_DRV_ERR("proc_ctx ptr or task id ptr is null \n");
		return -EINVAL;
	}

	task_info = devdrv_alloc_task(proc_ctx->devid);
	if (task_info == NULL) {
		NPU_DRV_ERR("task info is null\n");
		*task_id_ptr = DEVDRV_MAX_SINK_TASK_ID;
		return -ENODATA;
	}

	list_add(&task_info->list, &proc_ctx->task_list);
	proc_ctx->task_num++;
	NPU_DRV_DEBUG("npu process %d own task id = %u num = %d now \n", current->pid, task_info->id, proc_ctx->task_num);

	*task_id_ptr = task_info->id;
	return 0;
}

int devdrv_proc_free_task(struct devdrv_proc_ctx *proc_ctx, u32 task_id)
{
	int ret;

	if (proc_ctx == NULL || task_id >= DEVDRV_MAX_SINK_TASK_ID) {
		NPU_DRV_ERR("proc_ctx ptr or task id ptr is null \n");
		return -EINVAL;
	}

	if (proc_ctx->task_num == 0) {
		NPU_DRV_ERR("task_num id 0\n");
		return -ENODATA;
	}

	ret = devdrv_free_task_id(proc_ctx->devid, task_id);
	if (ret != 0) {
		NPU_DRV_ERR("free task id failed\n");
		return -ENODATA;
	}
	proc_ctx->task_num--;
	bitmap_clear(proc_ctx->task_bitmap, task_id, 1);

	NPU_DRV_DEBUG("npu process %d free task id = %u, left num = %d", current->pid, task_id, proc_ctx->task_num);

	return 0;
}

struct npu_id_entity *npu_proc_get_stream_sub_addr(struct devdrv_dev_ctx *cur_dev_ctx, int stream_id)
{
	struct npu_id_entity *stream_sub_info = NULL;
	if (stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID)
		stream_sub_info = devdrv_get_non_sink_stream_sub_addr(cur_dev_ctx, stream_id);
	else
		stream_sub_info = devdrv_get_sink_stream_sub_addr(cur_dev_ctx, stream_id);

	return stream_sub_info;
}

int devdrv_proc_free_stream(struct devdrv_proc_ctx* proc_ctx, u32 stream_id)
{
	struct devdrv_stream_info* stream_info = NULL;
	u32 sq_send_count = 0;
	u8 dev_id;
	int ret;

	if (proc_ctx == NULL || stream_id >= DEVDRV_MAX_STREAM_ID) {
		NPU_DRV_ERR("proc_ctx ptr is null or illegal npu stream id. stream_id=%d\n", stream_id);
		return -1;
	}

	if (stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID && proc_ctx->stream_num == 0) {
		NPU_DRV_ERR("stream_num is 0. stream_id=%d \n", stream_id);
		return -1;
	}

	if (stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID && proc_ctx->sink_stream_num == 0) {
		NPU_DRV_ERR("sink stream_num is 0 stream_id=%d \n", stream_id);
		return -1;
	}

	dev_id = proc_ctx->devid;
	stream_info = devdrv_calc_stream_info(dev_id, stream_id);
	if (stream_info == NULL) {
		NPU_DRV_ERR("stream_info is NULL. stream_id=%d\n", stream_id);
		return -1;
	}

	if (test_bit(stream_id, proc_ctx->stream_bitmap) == 0) {
		NPU_DRV_ERR(" has already been freed! stream_id=%d \n", stream_id);
		return -1;
	}

	ret = devdrv_free_stream(proc_ctx->devid, stream_id, &sq_send_count);
	if (ret != 0) {
		NPU_DRV_ERR("npu process %d free stream_id %d failed \n", current->pid, stream_id);
		return -1;
	}
	proc_ctx->send_count += sq_send_count;
	(void)devdrv_proc_release_stream_sq_id(proc_ctx, stream_info->strategy, stream_info->sq_index);

	bitmap_clear(proc_ctx->stream_bitmap, stream_id, 1);
	if (stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID) {
		proc_ctx->stream_num--;
	} else {
		proc_ctx->sink_stream_num--;
	}

	NPU_DRV_DEBUG("npu process %d left stream num = %d sq_send_count = %d "
		"(if stream'sq has been released) now\n",
		current->pid, proc_ctx->stream_num, sq_send_count);
	return 0;
}

static int devdrv_proc_free_single_cq(struct devdrv_proc_ctx *proc_ctx, u32 cq_id)
{
	struct devdrv_ts_cq_info *cq_info = NULL;
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	u8 dev_id;

	if (proc_ctx == NULL) {
		NPU_DRV_ERR("proc_ctx is null\n");
		return -1;
	}

	if (cq_id >= DEVDRV_MAX_CQ_NUM) {
		NPU_DRV_ERR("illegal npu cq id = %d\n", cq_id);
		return -1;
	}

	if (proc_ctx->cq_num == 0) {
		NPU_DRV_ERR("cq_num is 0\n");
		return -1;
	}

	dev_id = proc_ctx->devid;
	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "invalid cur_dev_ctx\n");
	cq_info = devdrv_calc_cq_info(dev_id, cq_id);
	if (cq_info == NULL) {
		NPU_DRV_ERR("cq_info is NULL\n");
		return -1;
	}

	cq_sub_info = npu_get_cq_sub_addr(cur_dev_ctx, cq_info->index);
	COND_RETURN_ERROR(cq_sub_info == NULL, -1, "invalid cq_sub index %u\n", cq_info->index);
	proc_ctx->receive_count += cq_info->receive_count;

	// del from proc_ctx->cq_list
	list_del(&cq_sub_info->list);
	proc_ctx->cq_num--;
	// add to dev_ctx->cq_available_list
	(void)npu_free_cq(dev_id, cq_id);

	NPU_DRV_DEBUG("proc_ctx pid %d cq_id %d total receive report"
		" count = %d proc current left cq num = %d \n", proc_ctx->pid,
		cq_id, proc_ctx->receive_count, proc_ctx->cq_num);
	return 0;
}

int devdrv_proc_free_cq(struct devdrv_proc_ctx *proc_ctx)
{
	struct devdrv_cq_sub_info *cq_sub = NULL;
	u32 cq_id;

	if (proc_ctx == NULL) {
		NPU_DRV_ERR("proc_ctx is null\n");
		return -1;
	}

	if (list_empty_careful(&proc_ctx->cq_list)) {
		NPU_DRV_ERR("cur process %d available cq list empty,"
			"left cq_num = %d !!!\n", proc_ctx->pid, proc_ctx->cq_num);
		return -1;
	}

	cq_sub = list_first_entry(&proc_ctx->cq_list, struct devdrv_cq_sub_info, list);
	cq_id = cq_sub->index;

	return devdrv_proc_free_single_cq(proc_ctx, cq_id);
}

inline u16 devdrv_get_phase_from_report(struct devdrv_report *report)
{
	return report->phase;
}

inline void devdrv_set_phase_to_report(struct devdrv_report *report, u16 val)
{
	report->phase = val;
}

// get phase of report from cq head or cq tail depending on report_pos
static u32 __devdrv_get_report_phase_from_cq_info(
	u8 dev_id, struct devdrv_ts_cq_info *cq_info, cq_report_pos_t report_pos)
{
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_report *report = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	u32 phase = 0;

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, CQ_INVALID_PHASE, "invalid dev id %u\n", dev_id);

	cq_sub_info = npu_get_cq_sub_addr(cur_dev_ctx, cq_info->index);
	COND_RETURN_ERROR(cq_sub_info == NULL, CQ_INVALID_PHASE, "invalid cq_sub_info index %u\n", cq_info->index);

	if (report_pos == RREPORT_FROM_CQ_TAIL)
		report = (struct devdrv_report*)(uintptr_t)(cq_sub_info->virt_addr +
			(long)(unsigned)(cq_info->slot_size * cq_info->tail));
	if (report_pos == RREPORT_FROM_CQ_HEAD)
		report = (struct devdrv_report*)(uintptr_t)(cq_sub_info->virt_addr +
			(long)(unsigned)(cq_info->slot_size * cq_info->head));
	if (report != NULL)
		phase = devdrv_get_phase_from_report(report);

	return phase;
}

static u32 __devdrv_get_report_phase(struct devdrv_proc_ctx *proc_ctx, cq_report_pos_t report_pos)
{
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_ts_cq_info *cq_info = NULL;
	u32 phase = 0;

	if (list_empty_careful(&proc_ctx->cq_list)) {
		NPU_DRV_ERR("proc ctx cq list is empty");
		return phase;
	}

	cq_sub_info = list_first_entry(&proc_ctx->cq_list, struct devdrv_cq_sub_info, list);
	cq_info = devdrv_calc_cq_info(proc_ctx->devid, cq_sub_info->index);
	if (cq_info == NULL) {
		NPU_DRV_ERR("proc ctx first cq_info is null");
		return phase;
	}

	phase = __devdrv_get_report_phase_from_cq_info(proc_ctx->devid, cq_info, report_pos);
	if (phase == CQ_INVALID_PHASE) {
		NPU_DRV_ERR("invalid phase dev id %u report_pos %d cq index %u\n", proc_ctx->devid, report_pos, cq_info->index);
		return 0;
	}

	return phase;
}

bool devdrv_sync_cq_report_valid(struct devdrv_sync_cq_info *sync_cq_info, u16 stream_id)
{
	struct devdrv_report *report = NULL;

	report = (struct devdrv_report*)(uintptr_t)(sync_cq_info->sync_cq_vaddr +
		(long)(unsigned)(sync_cq_info->slot_size * stream_id));

	return (devdrv_get_phase_from_report(report) ? true : false);
}

u32 devdrv_proc_get_cq_tail_report_phase(struct devdrv_proc_ctx *proc_ctx)
{
	return __devdrv_get_report_phase(proc_ctx, RREPORT_FROM_CQ_TAIL);
}

u32 devdrv_proc_get_cq_head_report_phase(struct devdrv_proc_ctx *proc_ctx)
{
	return __devdrv_get_report_phase(proc_ctx, RREPORT_FROM_CQ_HEAD);
}

bool devdrv_proc_is_valid_report_received(struct devdrv_proc_ctx *proc_ctx,
	struct devdrv_ts_cq_info *cq_info)
{
	int report_phase;

	report_phase = __devdrv_get_report_phase_from_cq_info(proc_ctx->devid, cq_info, RREPORT_FROM_CQ_TAIL);
	if (report_phase == CQ_INVALID_PHASE) {
		NPU_DRV_ERR("invalid phase dev id %u cq index %u\n", proc_ctx->devid, cq_info->index);
		return false;
	}

	NPU_DRV_DEBUG("cqid = %u cqtail = %u cq_info phase = %u report phase = %u\n",
		cq_info->index, cq_info->tail, cq_info->phase, report_phase);

	if (cq_info->phase == report_phase)
		return true;

	return false;
}

int devdrv_proc_report_wait(struct devdrv_proc_ctx *proc_ctx, struct devdrv_ts_cq_info *cq_info, int timeout)
{
	int ret;
	unsigned long tmp;
	int wait_time = timeout;
	int wait_event_once = 1000; /* 1s */

	proc_ctx->cq_tail_updated = CQ_HEAD_INITIAL_UPDATE_FLAG;
	if (wait_time == -1) {
		while (1) {
			wait_event(proc_ctx->report_wait, proc_ctx->cq_tail_updated == CQ_HEAD_UPDATED_FLAG);
			if (devdrv_proc_is_valid_report_received(proc_ctx, cq_info) == false) {
				proc_ctx->cq_tail_updated = CQ_HEAD_INITIAL_UPDATE_FLAG;
				continue;
			}
			return 1; /* get one valid cq report */
		}
	}

	while (wait_time >= wait_event_once) {
		wait_time -= wait_event_once;
		tmp = msecs_to_jiffies(wait_event_once);
		ret = wait_event_timeout(proc_ctx->report_wait,
			proc_ctx->cq_tail_updated == CQ_HEAD_UPDATED_FLAG, tmp);
		if (ret > 0) {
			if (devdrv_proc_is_valid_report_received(proc_ctx, cq_info) == false) {
				wait_time += wait_event_once;
				proc_ctx->cq_tail_updated = CQ_HEAD_INITIAL_UPDATE_FLAG;
				continue;
			}
			return 1; /* condition became true, get one valid cq report */
		}

		if (devdrv_proc_is_valid_report_received(proc_ctx, cq_info) == true) {
			return 1;
		} else {
			NPU_DRV_DEBUG("devid: %d, wait cq timeout once, time_out: %d, left time: %d, ret :%d\n",
				proc_ctx->devid, timeout, wait_time, ret);
		}
	}

	if (wait_time > 0) {
		tmp = msecs_to_jiffies(wait_time);
		ret = wait_event_timeout(proc_ctx->report_wait,
			proc_ctx->cq_tail_updated == CQ_HEAD_UPDATED_FLAG, tmp);
		if (ret > 0) {
			return 1; /* condition became true, get one valid cq report */
		}

		if (devdrv_proc_is_valid_report_received(proc_ctx, cq_info) == true) {
			return 1;
		} else {
			NPU_DRV_DEBUG("devid: %d, wait cq timeout once, time_out: %d, left time: %d, ret :%d\n",
				proc_ctx->devid, timeout, wait_time, ret);
		}
	}

	return 0;
}

bool npu_sync_wait_condition(struct devdrv_sync_cq_info *sync_cq_info,
	struct devdrv_report_sync_wait_info *wait_info, uint32_t *sq_head)
{
	struct devdrv_report *report = NULL;
	unsigned long flags;

	NPU_DRV_DEBUG("wait_info:stream_id=%u task_id=%u\n", wait_info->stream_id, wait_info->task_id);

	report = (struct devdrv_report*)(uintptr_t)(sync_cq_info->sync_cq_vaddr +
		(long)(unsigned)(sync_cq_info->slot_size * wait_info->stream_id));

	NPU_DRV_DEBUG("report:stream_id=%u task_id=%u phase=%u result=%llx\n", report->stream_id,
		report->task_id, report->phase, report->pay_load);

	if ((report->stream_id == wait_info->stream_id) &&
		(report->task_id == wait_info->task_id) &&
		(report->phase == 1)) {
		NPU_DRV_DEBUG("stream_id=%u task_id=%u phase=%u\n", wait_info->stream_id, wait_info->task_id, report->phase);
		if (copy_to_user_safe((void *)(uintptr_t)(wait_info->report_addr), report, sync_cq_info->slot_size) != 0)
			NPU_DRV_ERR("copy_to_user_safe error, report_addr=0x%llx, slot_size=%u\n", wait_info->report_addr, sync_cq_info->slot_size);
		*sq_head = report->sq_head;
		memset_s(report, sizeof(struct devdrv_report), 0, sizeof(struct devdrv_report));
		mb();

		return true;
	} else {
		/* cq space is cacheable in ts, entry maybe write by cache flush */
		spin_lock_irqsave(&sync_cq_info->spinlock, flags);
		bitmap_set(sync_cq_info->sync_stream_bitmap, wait_info->stream_id, 1);
		spin_unlock_irqrestore(&sync_cq_info->spinlock, flags);
		NPU_DRV_DEBUG("failed to check sync report, wait_stream_id=%u wait_task_id=%u stream_id=%u task_id=%u phase=%u\n",
			wait_info->stream_id, wait_info->task_id, report->stream_id, report->task_id, report->phase);

		return false;
	}
}

int npu_sync_wait(struct npu_id_entity *stream_sub, struct devdrv_sync_cq_info *sync_cq_info, struct devdrv_report_sync_wait_info *wait_info, uint32_t *sq_head, int timeout)
{
	unsigned long tmp;
	int ret = -EINVAL;
	int wait_time = timeout;
	int wait_event_once = 1000; /* 1s */

	while (wait_time >= wait_event_once) {
		wait_time -= wait_event_once;
		tmp = msecs_to_jiffies(wait_event_once);
		if (wait_event_interruptible_timeout(((struct devdrv_stream_sub_extra_info*)(stream_sub->data))->sync_report_wait,
		(true == npu_sync_wait_condition(sync_cq_info, wait_info, sq_head)), tmp)) {
			ret = 1; /* condition became true */
			return ret;
		} else {
			ret = 0; /* timeout with condition false */
			NPU_DRV_DEBUG("wait cq timeout once, time_out: %d, left time: %d\n", timeout, wait_time);
		}
	}

	if (wait_time > 0) {
		tmp = msecs_to_jiffies(wait_time);
		if (wait_event_interruptible_timeout(((struct devdrv_stream_sub_extra_info*)(stream_sub->data))->sync_report_wait,
		(true == npu_sync_wait_condition(sync_cq_info, wait_info, sq_head)), tmp)) {
			ret = 1; /* condition became true */
			return ret;
		} else {
			ret = 0; /* timeout with condition false */
			NPU_DRV_DEBUG("wait cq timeout once, time_out: %d, left time: %d\n", timeout, 0);
		}
	}

	return ret;
}

int devdrv_proc_report_sync_wait(struct devdrv_proc_ctx *proc_ctx, struct devdrv_report_sync_wait_info *wait_info)
{
	struct devdrv_dev_ctx *dev_ctx;
	struct devdrv_sync_cq_info *sync_cq_info = NULL;
	struct devdrv_stream_info *stream_info = NULL;
	struct npu_id_entity *stream_sub = NULL;
	struct devdrv_ts_sq_info *sq_info = NULL;

	long wait_ret;
	uint32_t sq_head = 0;
	unsigned long flags;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -ENODATA, "dev_ctx %u is null\n", proc_ctx->devid);
	sync_cq_info = (struct devdrv_sync_cq_info *)dev_ctx->sync_cq;
	if (sync_cq_info == NULL) {
		NPU_DRV_ERR("sync_cq_info is null\n");
		return -1;
	}
	NPU_DRV_DEBUG("wait_info:report_addr=%llx stream_id=%u task_id=%u timeout=%u\n",
		wait_info->report_addr, wait_info->stream_id, wait_info->task_id, wait_info->timeout);
	NPU_DRV_DEBUG("sync_cq_info:sync_cq_vaddr=%llx sync_cq_paddr=%llx slot_size=%u\n",
		sync_cq_info->sync_cq_vaddr, sync_cq_info->sync_cq_paddr, sync_cq_info->slot_size);

	spin_lock_irqsave(&sync_cq_info->spinlock, flags);
	bitmap_set(sync_cq_info->sync_stream_bitmap, wait_info->stream_id, 1);
	spin_unlock_irqrestore(&sync_cq_info->spinlock, flags);

	stream_info = devdrv_calc_stream_info(proc_ctx->devid, wait_info->stream_id);
	COND_RETURN_ERROR(stream_info == NULL, -ENODATA, "stream info is null\n");
	stream_sub = npu_proc_get_stream_sub_addr(dev_ctx, stream_info->id);
	COND_RETURN_ERROR(stream_sub == NULL, -ENODATA, "stream sub is null\n");
	COND_RETURN_ERROR(stream_info->sq_index >= DEVDRV_MAX_SQ_NUM, -ENODATA, "stream_info->sq_index  is invalid\n");

	wait_ret = npu_sync_wait(stream_sub, sync_cq_info, wait_info, &sq_head, wait_info->timeout);
	if (wait_ret > 0) {
		NPU_DRV_INFO("success to wait report\n");
		sq_info = devdrv_calc_sq_info(proc_ctx->devid, stream_info->sq_index);
		COND_RETURN_ERROR(sq_info == NULL, -ENODATA, "sq_info is null\n");
		spin_lock(&dev_ctx->spinlock);
		sq_info->head = sq_head;
		spin_unlock(&dev_ctx->spinlock);
	} else {
		NPU_DRV_WARN("wait report timeout, stream_id = %u task_id = %u, time_out = %u, wait_ret = %ld\n",
			wait_info->stream_id, wait_info->task_id, wait_info->timeout, wait_ret);
	}

	/*
	*  1. wait_ret > 0, report already exist, need to clear stream bit
	*  2. wait_ret <=0, timeout or signal, need to clear stream bit
	*/
	spin_lock_irqsave(&sync_cq_info->spinlock, flags);
	bitmap_clear(sync_cq_info->sync_stream_bitmap, wait_info->stream_id, 1);
	spin_unlock_irqrestore(&sync_cq_info->spinlock, flags);

	return (int)wait_ret;
}

static void devdrv_walk_sync_cq(unsigned long data);

static void devdrv_find_cq_index(unsigned long data)
{
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_ts_cq_info *cq_info = NULL;
	struct devdrv_cq_report_int_ctx *int_context = NULL;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	unsigned long flags;
	int cq_index;
	u32 phase;
	int end;
	int i = 0;

	devdrv_walk_sync_cq(data);

	g_find_cq_index_called_num++; // user for debug,compare with ts side
	int_context = (struct devdrv_cq_report_int_ctx *)(uintptr_t)data;
	proc_ctx = int_context->proc_ctx;
	if (proc_ctx == NULL) {
		NPU_DRV_ERR("cq report int_context`s proc ctx is null ");
		return;
	}

	cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_VOID(cur_dev_ctx == NULL, "invalid dev_id %u\n", proc_ctx->devid);

	end = int_context->first_cq_index + DEVDRV_CQ_PER_IRQ;
	for (i = int_context->first_cq_index; i < end; i++) {
		cq_index = i;
		cq_info = devdrv_calc_cq_info(proc_ctx->devid, cq_index);
		COND_RETURN_VOID(cq_info == NULL, "cq info is null\n");
		if (cq_index != cq_info->index) {
			NPU_DRV_ERR("cq_index != cq_info->index, cq_index: %d, "
				"cq_info->index: %d\n", cq_index, cq_info->index);
			continue;
		}

		cq_sub_info = npu_get_cq_sub_addr(cur_dev_ctx, cq_info->index);
		if (cq_sub_info == NULL)
			continue;

		spin_lock_irqsave(&cq_sub_info->spinlock, flags);
		phase = devdrv_proc_get_cq_tail_report_phase(proc_ctx);
		spin_unlock_irqrestore(&cq_sub_info->spinlock, flags);

		/* for checking a thread is waiting for wake up */
		if (proc_ctx->cq_tail_updated != CQ_HEAD_INITIAL_UPDATE_FLAG) { /* condition is true, continue */
			NPU_DRV_DEBUG("receive report irq, cq id: %u, "
				"no runtime thread is waiting, not judge\n", cq_info->index);
			continue;
		}
		if (phase == CQ_INVALID_PHASE) {
			continue;
		}

		if (phase == cq_info->phase) {
			NPU_DRV_DEBUG("receive report irq, cq id: %u, cq_head = %d "
				"cq_tail = %d report is valid, wake up runtime thread\n",
				cq_info->index, cq_info->head, cq_info->tail);
			proc_ctx->cq_tail_updated = CQ_HEAD_UPDATED_FLAG;
			wake_up(&proc_ctx->report_wait);
		}
	}
}

static void devdrv_walk_sync_cq(unsigned long data)
{
	struct devdrv_sync_cq_info *sync_cq_info = NULL;
	struct devdrv_cq_report_int_ctx *int_context = NULL;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	struct devdrv_dev_ctx *dev_ctx = NULL;
	struct npu_id_entity *stream_sub_info = NULL;
	struct devdrv_stream_info *stream_info = NULL;
	u16 stream_id = 0;
	unsigned long flags;

	int_context = (struct devdrv_cq_report_int_ctx *)(uintptr_t)data;
	proc_ctx = int_context->proc_ctx;
	if (proc_ctx == NULL) {
		NPU_DRV_ERR("cq report int_context`s proc ctx is null ");
		return;
	}

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("dev_ctx %d is null\n", proc_ctx->devid);
		return;
	}

	sync_cq_info = (struct devdrv_sync_cq_info *)dev_ctx->sync_cq;
	if (sync_cq_info == NULL)
	{
		NPU_DRV_ERR("sync_cq_info is null\n");
		return;
	}

	NPU_DRV_DEBUG("walk sync_cq\n");

	for (stream_id = 0;
		stream_id = find_next_bit(sync_cq_info->sync_stream_bitmap, DEVDRV_MAX_NON_SINK_STREAM_ID, stream_id), \
		stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID;
		stream_id++) {
		if (false == devdrv_sync_cq_report_valid(sync_cq_info, stream_id))
			continue;

		spin_lock_irqsave(&sync_cq_info->spinlock, flags);
		bitmap_clear(sync_cq_info->sync_stream_bitmap, stream_id, 1);
		spin_unlock_irqrestore(&sync_cq_info->spinlock, flags);

		stream_info = devdrv_calc_stream_info(proc_ctx->devid, stream_id);
		COND_RETURN_VOID(stream_info == NULL, "stream info is null\n");
		stream_sub_info = npu_proc_get_stream_sub_addr(dev_ctx, stream_info->id);
		COND_RETURN_VOID(stream_sub_info == NULL, "stream sub is null\n");
		NPU_DRV_DEBUG("wakeup thread, stream_id=%u\n", stream_id);

		wake_up(&(((struct devdrv_stream_sub_extra_info*)(stream_sub_info->data))->sync_report_wait));
	}

	return;
}


static irqreturn_t devdrv_irq_handler(int irq, void *data)
{
	struct devdrv_cq_report_int_ctx *int_context = NULL;
	unsigned long flags;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	int_context = (struct devdrv_cq_report_int_ctx *)data;
	if (int_context->proc_ctx == NULL)
		return IRQ_NONE;

	cur_dev_ctx = get_dev_ctx_by_id(int_context->proc_ctx->devid);
	if (cur_dev_ctx == NULL)
		return IRQ_NONE;

	if (cur_dev_ctx->power_stage != DEVDRV_PM_UP)
		return IRQ_NONE;

	local_irq_save(flags);
	g_recv_cq_int_num++; // user for debug,compare with ts side
	devdrv_plat_handle_irq_tophalf(DEVDRV_IRQ_CALC_CQ_UPDATE0);

	tasklet_schedule(&int_context->find_cq_task);

	local_irq_restore(flags);

	return IRQ_HANDLED;
}

// just use for debug when exception happened
void show_cq_report_int_info(void)
{
	NPU_DRV_ERR("g_recv_cq_int_num = %llu, g_find_cq_index_called_num: %llu\n",
		g_recv_cq_int_num, g_find_cq_index_called_num);
}

static int __devdrv_request_cq_report_irq_bh(struct devdrv_cq_report_int_ctx *cq_int_ctx)
{
	int ret;
	unsigned int cq_irq;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info\n");
		return -EINVAL;
	}

	if (cq_int_ctx == NULL) {
		NPU_DRV_ERR("cq report int_context is null ");
		return -1;
	}

	cq_int_ctx->first_cq_index = 0;
	tasklet_init(&cq_int_ctx->find_cq_task, devdrv_find_cq_index, (uintptr_t)cq_int_ctx);

	cq_irq = (unsigned int)DEVDRV_PLAT_GET_CQ_UPDATE_IRQ(plat_info);

	ret = request_irq(cq_irq, devdrv_irq_handler, IRQF_TRIGGER_NONE, "npu_cq_report_handler", cq_int_ctx);
	if (ret != 0) {
		NPU_DRV_ERR("request cq report irq failed\n");
		goto request_failed0;
	}
	NPU_DRV_DEBUG("request cq report irq %d success\n", cq_irq);

	return ret;

request_failed0:
	tasklet_kill(&cq_int_ctx->find_cq_task);
	return ret;
}

int devdrv_request_cq_report_irq_bh(void)
{
	return __devdrv_request_cq_report_irq_bh(&g_cq_int_ctx);
}

static int __devdrv_free_cq_report_irq_bh(struct devdrv_cq_report_int_ctx *cq_int_ctx)
{
	unsigned int cq_irq;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info\n");
		return -EINVAL;
	}

	if (cq_int_ctx == NULL) {
		NPU_DRV_ERR("cq report int_context is null ");
		return -EINVAL;
	}

	cq_irq = (unsigned int)DEVDRV_PLAT_GET_CQ_UPDATE_IRQ(plat_info);
	free_irq(cq_irq, cq_int_ctx);
	tasklet_kill(&cq_int_ctx->find_cq_task);
	return 0;
}

int devdrv_free_cq_report_irq_bh(void)
{
	return __devdrv_free_cq_report_irq_bh(&g_cq_int_ctx);
}


void devdrv_bind_proc_ctx_with_cq_int_ctx(struct devdrv_proc_ctx *proc_ctx)
{
	g_cq_int_ctx.proc_ctx = proc_ctx;
}

void devdrv_unbind_proc_ctx_with_cq_int_ctx(const struct devdrv_proc_ctx *proc_ctx)
{
	if (proc_ctx != NULL)
		g_cq_int_ctx.proc_ctx = NULL;
}

