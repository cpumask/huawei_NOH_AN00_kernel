/*
 * npu_hwts_sqe.c
 *
 * about npu hwts sqe
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
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include "npu_hwts_sqe.h"
#include "npu_log.h"
#include "npu_ts_sqe.h"

typedef int (*verify_hwts_task_func)(devdrv_rt_hwts_task_t *hwts_task);
typedef void (*format_hwts_sqe_func)(void *hwts_sqe_addr,
	devdrv_rt_hwts_task_t *hwts_task, devdrv_model_desc_info_t *model_desc_info);

#define RT_TASK_ENTRY(stream_buf_addr, offset) ((stream_buf_addr) + (offset) * DEVDRV_RT_TASK_SIZE)
#define HWTS_SQE_ENTRY(hwts_sq_addr, offset) ((hwts_sq_addr) + (offset) * DEVDRV_HWTS_SQ_SLOT_SIZE)

int verify_task_comm_field(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->type >= DEVDRV_TS_SQE_RESERVED) {
		NPU_DRV_ERR("invalid task type:%u, task_id:%u\n", hwts_task->type, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID || hwts_task->stream_id >= DEVDRV_MAX_STREAM_ID) {
		NPU_DRV_ERR("invalid task stream_id:%u, task_id:%u\n", hwts_task->stream_id, hwts_task->task_id);
		return -1;
	}
	if ((hwts_task->stream_id < DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_SINK_LONG_STREAM_ID
		&& hwts_task->task_id >= DEVDRV_MAX_SINK_LONG_TASK_ID) ||
		(hwts_task->stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_SINK_LONG_STREAM_ID
		&& hwts_task->task_id >= DEVDRV_MAX_SINK_TASK_ID)) {
		NPU_DRV_ERR("invalid task_id:%u, stream id:%u\n", hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

int verify_aicore_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.kernel_task.block_dim == 0 || hwts_task->u.kernel_task.block_dim > UINT16_MAX) {
		NPU_DRV_ERR("invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.pc_start == 0) {
		NPU_DRV_ERR("invalid pc_start is 0, task_id:%u\n", hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.param_base == 0) {
		NPU_DRV_ERR("invalid param_base is 0, task_id:%u\n", hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.rd_cond == 1 && hwts_task->u.kernel_task.block_dim != 1) {
		NPU_DRV_ERR("rd_cond is 1, invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_ph_task(devdrv_rt_hwts_task_t *hwts_task)
{
	return 0;
}

int verify_label_switch_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->stream_id >= (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_switch_task.true_label_idx >= DEVDRV_MAX_HWTS_SQ_DEPTH) {
		NPU_DRV_ERR("invalid true_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_switch_task.true_label_idx, hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	if (hwts_task->stream_id < (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_switch_task.true_label_idx >= DEVDRV_MAX_LONG_HWTS_SQ_DEPTH) {
		NPU_DRV_ERR("invalid true_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_switch_task.true_label_idx, hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

int verify_label_goto_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->stream_id >= (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_goto_task.label_idx >= DEVDRV_MAX_HWTS_SQ_DEPTH) {
		NPU_DRV_ERR("invalid goto_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_goto_task.label_idx, hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	if (hwts_task->stream_id < (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_goto_task.label_idx >= DEVDRV_MAX_LONG_HWTS_SQ_DEPTH) {
		NPU_DRV_ERR("invalid goto_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_goto_task.label_idx, hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

int verify_evt_rec_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.event_task.event_id >= DEVDRV_MAX_HWTS_EVENT_ID) {
		NPU_DRV_ERR("invalid event_id:%u, task_id:%u\n",
			hwts_task->u.event_task.event_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_wait_evt_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.event_task.event_id >= DEVDRV_MAX_HWTS_EVENT_ID) {
		NPU_DRV_ERR("invalid event_id:%u, task_id:%u\n",
			hwts_task->u.event_task.event_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_notify_rec_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.notify_task.notify_id >= DEVDRV_MAX_HWTS_NOTIFY_ID) {
		NPU_DRV_ERR("invalid notify_id:%u, task_id:%u\n",
			hwts_task->u.notify_task.notify_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_wait_notify_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.notify_task.notify_id >= DEVDRV_MAX_HWTS_NOTIFY_ID) {
		NPU_DRV_ERR("invalid notify_id:%u, task_id:%u\n",
			hwts_task->u.notify_task.notify_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_wite_value_task(devdrv_rt_hwts_task_t *hwts_task)
{
	return 0;
}

int verify_memcpy_task(devdrv_rt_hwts_task_t *hwts_task)
{
	if (hwts_task->u.memcpy_task.src_addr & NPU_BIT_MASK(4)) {
		NPU_DRV_INFO("invalid src_addr:0x%llx not 16B align\n", hwts_task->u.memcpy_task.src_addr);
		NPU_DRV_ERR("invalid sdma task_id:%u\n", hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.memcpy_task.dst_addr & NPU_BIT_MASK(4)) {
		NPU_DRV_INFO("invalid dst_addr:0x%llx not 16B align\n", hwts_task->u.memcpy_task.dst_addr);
		NPU_DRV_ERR("invalid sdma task_id:%u\n", hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.memcpy_task.length & NPU_BIT_MASK(4)) {
		NPU_DRV_ERR("invalid length:%llu not 16B align, sdma task_id:%u\n",
			hwts_task->u.memcpy_task.length, hwts_task->task_id);
		return -1;
	}
	return 0;
}

verify_hwts_task_func verify_hwts_task_map[] = {
/*	0								1								2							3			*/
	verify_aicore_task,				NULL,							verify_evt_rec_task,		verify_wait_evt_task,
/*	4								5								6							7			*/
	verify_ph_task,					verify_memcpy_task,				NULL,						NULL,
/*	8								9								10							11			*/
	NULL,							NULL,							NULL,						NULL,
/*	12								13								14							15			*/
	NULL,							NULL,							verify_wait_notify_task,	verify_notify_rec_task,
/*	16								17								18							19			*/
	NULL,							NULL,							NULL,						NULL,
/*	20								21								22							23			*/
	verify_ph_task,					verify_label_switch_task,		verify_label_goto_task,		NULL,
/*	24								25								26							27			*/
	NULL,							NULL,							NULL,						NULL,
/*	28								29								30							31			*/
	NULL,							NULL,							NULL,						NULL,
/*	32								33								34							35			*/
	NULL,							NULL,							NULL,						NULL,
/*	36								37								38							39			*/
	NULL,							NULL,							NULL,						NULL,
/*	40								41								42							43			*/
	NULL,							NULL,							NULL,						NULL,
/*	44								45								46							47			*/
	NULL,							NULL,							NULL,						NULL,
/*	48								49								50							51			*/
	NULL,							NULL,							NULL,						NULL,
/*	52								53								54							55			*/
	NULL,							NULL,							NULL,						NULL,
/*	56								57								58							59			*/
	NULL,							NULL,							NULL,						NULL,
/*	60								61								62							63			*/
	NULL,							NULL,							NULL,						NULL,
/*	64								65								66							67			*/
	NULL,							NULL,							NULL,						NULL,
};

int devdrv_verify_hwts_sqe(void *stream_buf_addr, u64 ts_sq_len)
{
	u64 ts_sqe_num = ts_sq_len / DEVDRV_RT_TASK_SIZE;
	verify_hwts_task_func verify_func = NULL;
	u8 *stream_buf_addr_base = stream_buf_addr;
	devdrv_rt_hwts_task_t *hwts_task = NULL;
	int ret = 0;
	u32 i;

	if (stream_buf_addr == NULL) {
		NPU_DRV_ERR("stream_buf_addr is null\n");
		return -1;
	}

	NPU_DRV_DEBUG("stream_buf_addr:0x%llx, ts_sq_len:%llu, ts_sqe_num:%llu",
		(uintptr_t)stream_buf_addr, ts_sq_len, ts_sqe_num);
	for (i = 0; i < ts_sqe_num; i++) {
		hwts_task = (devdrv_rt_hwts_task_t *)RT_TASK_ENTRY(stream_buf_addr_base, i);

		ret = verify_task_comm_field(hwts_task);
		if (ret != 0) {
			NPU_DRV_ERR("verify_task_comm_field failed, ret:%d, task_id:%u, type:%u\n",
				ret, hwts_task->task_id, hwts_task->type);
			return -1;
		}

		if (hwts_task->type >= DEVDRV_TS_SQE_RESERVED) {
			NPU_DRV_ERR("invalid task type:%u, task_id:%u\n",
				hwts_task->type, hwts_task->task_id);
			return -1;
		}
		verify_func = verify_hwts_task_map[hwts_task->type];
		if (verify_func == NULL) {
			NPU_DRV_ERR("invalid stream_id:%u, task_id:%u, type:%u, total:%llu, index:%u.\n",
				hwts_task->stream_id, hwts_task->task_id, hwts_task->type, ts_sqe_num, i);
			return -1;
		}
		ret = verify_func(hwts_task);
		if (ret != 0) {
			NPU_DRV_ERR("verify_hwts_sqe failed, ret:%d, task_id:%u, type:%u\n",
				ret, hwts_task->task_id, hwts_task->type);
			return -1;
		}
	}

	NPU_DRV_DEBUG("devdrv_verify_hwts_sqe end\n");
	return 0;
}

void format_aicore_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_kernel_sqe_t *kernel_sqe = (devdrv_hwts_kernel_sqe_t *)hwts_sqe_addr;
	kernel_sqe->type = DEVDRV_HWTS_SQE_AICORE;
	kernel_sqe->ie = 0;
	kernel_sqe->pre_p = 0;
	kernel_sqe->post_p = 0;
	kernel_sqe->wr_cqe = 0;
	kernel_sqe->rd_cond = hwts_task->u.kernel_task.rd_cond;
	kernel_sqe->res0 = 0;
	kernel_sqe->l2_lock = 0;
	kernel_sqe->l2_unlock = 0;
	kernel_sqe->block_dim = hwts_task->u.kernel_task.block_dim;
	kernel_sqe->stream_id = hwts_task->stream_id;
	kernel_sqe->task_id = hwts_task->task_id;

	kernel_sqe->pc_addr_low = (u32)(hwts_task->u.kernel_task.pc_start);
	kernel_sqe->pc_addr_high = (u16)((hwts_task->u.kernel_task.pc_start) >> 32);
	kernel_sqe->kernel_credit = 2;
	kernel_sqe->res2 = 0;
	kernel_sqe->icache_prefetch_cnt = 1;

	kernel_sqe->param_addr_low = (u32)(hwts_task->u.kernel_task.param_base);
	kernel_sqe->param_addr_high = (u16)((hwts_task->u.kernel_task.param_base) >> 32);
	kernel_sqe->l2_in_main = 0xFF; // dirty_code
	kernel_sqe->res3 = 0;

	kernel_sqe->literal_addr_low = (u32)(hwts_task->u.kernel_task.literal_src_addr);
	kernel_sqe->literal_addr_high = (u16)(hwts_task->u.kernel_task.literal_src_addr >> 32);
	kernel_sqe->res4 = 0;

	kernel_sqe->literal_base_ub = hwts_task->u.kernel_task.literal_dst_base;
	kernel_sqe->res5 = 0;

	kernel_sqe->literal_buff_len = hwts_task->u.kernel_task.literal_size;
	kernel_sqe->res6 = 0;

	NPU_DRV_DEBUG("kernel_sqe= %pK, struct size= %d, stream_id= %u, task_id= %u, pc_start= 0x%llx, param_base= 0x%llx, rd_cond= %u\n",
		kernel_sqe,
		sizeof(devdrv_hwts_kernel_sqe_t),
		hwts_task->stream_id,
		hwts_task->task_id,
		hwts_task->u.kernel_task.pc_start,
		hwts_task->u.kernel_task.param_base,
		hwts_task->u.kernel_task.rd_cond);
	return;
}

void format_ph_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_ph_sqe_t *ph_sqe = (devdrv_hwts_ph_sqe_t *)hwts_sqe_addr;

	ph_sqe->type = DEVDRV_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 0;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = DEVDRV_HWTS_PH_SQE_NORMAL;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	NPU_DRV_DEBUG("ph_sqe->task_id= %u\n", ph_sqe->task_id);
	return;
}

void format_label_switch_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_ph_sqe_t *ph_sqe = (devdrv_hwts_ph_sqe_t *)hwts_sqe_addr;

	ph_sqe->type = DEVDRV_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 1;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = DEVDRV_HWTS_PH_SQE_LABEL_SWITCH;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	ph_sqe->u.label_switch.condition = hwts_task->u.label_switch_task.condition;
	ph_sqe->u.label_switch.right = hwts_task->u.label_switch_task.right;
	ph_sqe->u.label_switch.true_label_idx = hwts_task->u.label_switch_task.true_label_idx;

	NPU_DRV_DEBUG("ph_sqe->task_id= %u, label_idx= %u, right= %llu, condition= %u\n",
		ph_sqe->task_id,
		hwts_task->u.label_switch_task.true_label_idx,
		hwts_task->u.label_switch_task.right,
		hwts_task->u.label_switch_task.condition);
	return;
}

void format_label_goto_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_ph_sqe_t *ph_sqe = (devdrv_hwts_ph_sqe_t *)hwts_sqe_addr;

	ph_sqe->type = DEVDRV_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 1;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = DEVDRV_HWTS_PH_SQE_LABEL_GOTO;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	ph_sqe->u.label_goto.label_idx = hwts_task->u.label_goto_task.label_idx;

	NPU_DRV_DEBUG("ph_sqe->task_id= %u, label_idx= %u\n", ph_sqe->task_id, hwts_task->u.label_goto_task.label_idx);
	return;
}

void add_model_event_id(devdrv_model_desc_info_t *model_desc_info, u16 event_id)
{
	// uint8_t bit map
	u16 group = event_id / 8;
	u8  offset = event_id % 8;

	uint8_t event_map = (uint8_t)0x1 << offset;
	model_desc_info->event_info[group] |= event_map;
	return;
}

void format_evt_rec_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_event_sqe_t *evt_rec_sqe = (devdrv_hwts_event_sqe_t *)hwts_sqe_addr;

	evt_rec_sqe->type = DEVDRV_HWTS_SQE_EVENT_RECORD;
	evt_rec_sqe->ie = 0;
	evt_rec_sqe->pre_p = 0;
	evt_rec_sqe->post_p = 0;
	evt_rec_sqe->wr_cqe = 0;
	evt_rec_sqe->res0 = 0;
	evt_rec_sqe->res1 = 0;

	evt_rec_sqe->stream_id = hwts_task->stream_id;
	evt_rec_sqe->task_id = hwts_task->task_id;
	evt_rec_sqe->event_id = hwts_task->u.event_task.event_id;
	evt_rec_sqe->res2 = 0;
	evt_rec_sqe->kernel_credit = 255;

	add_model_event_id(model_desc_info, evt_rec_sqe->event_id);
	NPU_DRV_DEBUG("evt_rec_sqe->task_id= %u, event_id= %u\n",
		evt_rec_sqe->task_id, evt_rec_sqe->event_id);
	return;
}

void format_wait_evt_sqe(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_event_sqe_t *evt_wait_sqe = (devdrv_hwts_event_sqe_t *)hwts_sqe_addr;

	evt_wait_sqe->type = DEVDRV_HWTS_SQE_EVENT_WAIT;
	evt_wait_sqe->ie = 0;
	evt_wait_sqe->pre_p = 0;
	evt_wait_sqe->post_p = 0;
	evt_wait_sqe->wr_cqe = 0;
	evt_wait_sqe->res0 = 0;
	evt_wait_sqe->res1 = 0;

	evt_wait_sqe->stream_id = hwts_task->stream_id;
	evt_wait_sqe->task_id = hwts_task->task_id;
	evt_wait_sqe->event_id = hwts_task->u.event_task.event_id;
	evt_wait_sqe->res2 = 0;
	evt_wait_sqe->kernel_credit = 255;

	NPU_DRV_DEBUG("evt_wait_sqe->task_id= %u, event_id= %u\n",
		evt_wait_sqe->task_id, evt_wait_sqe->event_id);
	return;
}

void format_notify_rec_task(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_notify_sqe_t *notify_rec_sqe = (devdrv_hwts_notify_sqe_t *)hwts_sqe_addr;

	notify_rec_sqe->type = DEVDRV_HWTS_SQE_NOTIFY_RECORD;
	notify_rec_sqe->ie = 0;
	notify_rec_sqe->pre_p = 0;
	notify_rec_sqe->post_p = 0;
	notify_rec_sqe->wr_cqe = 0;
	notify_rec_sqe->l2_lock = 0;
	notify_rec_sqe->l2_unlock = 0;

	notify_rec_sqe->stream_id = hwts_task->stream_id;
	notify_rec_sqe->task_id = hwts_task->task_id;
	notify_rec_sqe->notify_id = hwts_task->u.notify_task.notify_id;
	notify_rec_sqe->kernel_credit = 255;

	NPU_DRV_DEBUG("notify_rec_sqe->task_id= %u, notify_id= %u\n",
		notify_rec_sqe->task_id, notify_rec_sqe->notify_id);
	return;
}

void format_wait_notify_task(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_notify_sqe_t *notify_wait_sqe = (devdrv_hwts_notify_sqe_t *)hwts_sqe_addr;

	notify_wait_sqe->type = DEVDRV_HWTS_SQE_NOTIFY_WAIT;
	notify_wait_sqe->ie = 0;
	notify_wait_sqe->pre_p = 0;
	notify_wait_sqe->post_p = 0;
	notify_wait_sqe->wr_cqe = 0;
	notify_wait_sqe->l2_lock = 0;
	notify_wait_sqe->l2_unlock = 0;

	notify_wait_sqe->stream_id = hwts_task->stream_id;
	notify_wait_sqe->task_id = hwts_task->task_id;
	notify_wait_sqe->notify_id = hwts_task->u.notify_task.notify_id;
	notify_wait_sqe->kernel_credit = 255;

	NPU_DRV_DEBUG("notify_wait_sqe->task_id= %u, notify_id= %u\n",
		notify_wait_sqe->task_id, notify_wait_sqe->notify_id);
	return;
}

void format_memcpy_task(void *hwts_sqe_addr, devdrv_rt_hwts_task_t *hwts_task,
	devdrv_model_desc_info_t *model_desc_info)
{
	devdrv_hwts_memcpy_sqe_t *memcpy_sqe = (devdrv_hwts_memcpy_sqe_t *)hwts_sqe_addr;

	//part 1
	memcpy_sqe->type = DEVDRV_HWTS_SQE_MEMCPY;
	memcpy_sqe->ie = 0;
	memcpy_sqe->pre_p = 0;
	memcpy_sqe->post_p = 0;
	memcpy_sqe->wr_cqe = 0;
	memcpy_sqe->l2_lock = 0;
	memcpy_sqe->l2_unlock = 0;
	memcpy_sqe->stream_id = hwts_task->stream_id;
	memcpy_sqe->task_id = hwts_task->task_id;
	memcpy_sqe->kernel_credit = 255;

	//part 2
	memcpy_sqe->ie_dma = 0;
	memcpy_sqe->mode = 0;
	memcpy_sqe->w_pattern = 0;
	memcpy_sqe->src_streamid = 1; // need svm sid
	memcpy_sqe->dst_streamid = 1; // need svm sid
	memcpy_sqe->src_substreamid = 1; // need svm ssid
	memcpy_sqe->dst_substreamid = 1; // need svm ssid

	memcpy_sqe->src_addr_low = (u32)(hwts_task->u.memcpy_task.src_addr & NPU_BIT_MASK(32));
	memcpy_sqe->src_addr_high = (u32)((hwts_task->u.memcpy_task.src_addr >> 32) & NPU_BIT_MASK(32));
	memcpy_sqe->src_addr_high_p = 0; // 0-va, 1-pa
	memcpy_sqe->dst_addr_low = (u32)(hwts_task->u.memcpy_task.dst_addr & NPU_BIT_MASK(32));
	memcpy_sqe->dst_addr_high = (u32)((hwts_task->u.memcpy_task.dst_addr >> 32) & NPU_BIT_MASK(32));
	memcpy_sqe->dst_addr_high_p = 0; // 0-va, 1-pa
	memcpy_sqe->length = hwts_task->u.memcpy_task.length;

	NPU_DRV_DEBUG("memcpy_sqe->task_id= %u\n", memcpy_sqe->task_id);
	return;
}

format_hwts_sqe_func format_hwts_sqe_map[] = {
/*	0								1								2							3			*/
	format_aicore_sqe,				NULL,							format_evt_rec_sqe,			format_wait_evt_sqe,
/*	4								5								6							7			*/
	format_ph_sqe,					format_memcpy_task,				NULL,						NULL,
/*	8								9								10							11			*/
	NULL,							NULL,							NULL,						NULL,
/*	12								13								14							15			*/
	NULL,							NULL,							format_wait_notify_task,	format_notify_rec_task,
/*	16								17								18							19			*/
	NULL,							NULL,							NULL,						NULL,
/*	20								21								22							23			*/
	format_ph_sqe,					format_label_switch_sqe,		format_label_goto_sqe,		NULL,
/*	24								25								26							27			*/
	NULL,							NULL,							NULL,						NULL,
/*	28								29								30							31			*/
	NULL,							NULL,							NULL,						NULL,
/*	32								33								34							35			*/
	NULL,							NULL,							NULL,						NULL,
/*	36								37								38							39			*/
	NULL,							NULL,							NULL,						NULL,
/*	40								41								42							43			*/
	NULL,							NULL,							NULL,						NULL,
/*	44								45								46							47			*/
	NULL,							NULL,							NULL,						NULL,
/*	48								49								50							51			*/
	NULL,							NULL,							NULL,						NULL,
/*	52								53								54							55			*/
	NULL,							NULL,							NULL,						NULL,
/*	56								57								58							59			*/
	NULL,							NULL,							NULL,						NULL,
/*	60								61								62							63			*/
	NULL,							NULL,							NULL,						NULL,
/*	64								65								66							67			*/
	NULL,							NULL,							NULL,						NULL,
};

static inline void devdrv_format_head_sqe(devdrv_hwts_sqe_head_t *hwts_sqe_head)
{
	hwts_sqe_head->l2_lock = 1;
	return;
}

static inline void devdrv_format_tail_sqe(devdrv_hwts_sqe_head_t *hwts_sqe_head)
{
	hwts_sqe_head->l2_unlock = 1;
	hwts_sqe_head->wr_cqe = 1;
	return;
}

int devdrv_format_hwts_sqe(void *hwts_sq_addr, void *stream_buf_addr,
	u64 ts_sq_len, devdrv_model_desc_info_t *model_desc_info)
{
	u64 ts_sqe_num = ts_sq_len / DEVDRV_RT_TASK_SIZE;
	format_hwts_sqe_func format_func = NULL;
	u8 *hwts_sq_base = hwts_sq_addr;
	u8 *stream_buf_addr_base = stream_buf_addr;
	u8 *hwts_sqe = NULL;
	devdrv_rt_hwts_task_t *hwts_task = NULL;
	u32 i;

	if (hwts_sq_addr == NULL) {
		NPU_DRV_ERR("hwts_sq_addr is null\n");
		return -1;
	}
	if (stream_buf_addr == NULL) {
		NPU_DRV_ERR("stream_buf_addr is null\n");
		return -1;
	}

	NPU_DRV_DEBUG("hwts_sq_addr:0x%llx, stream_buf_addr:0x%llx, ts_sq_len:%llu, ts_sqe_num:%llu",
		(uintptr_t)hwts_sq_addr, (uintptr_t)stream_buf_addr, ts_sq_len, ts_sqe_num);
	for (i = 0; i < ts_sqe_num; i++) {
		hwts_task = (devdrv_rt_hwts_task_t *)RT_TASK_ENTRY(stream_buf_addr_base, i);
		hwts_sqe = HWTS_SQE_ENTRY(hwts_sq_base, i);

		format_func = format_hwts_sqe_map[hwts_task->type];
		if (format_func == NULL) {
			NPU_DRV_ERR("invalid task_id:%u, type:%u\n", hwts_task->task_id, hwts_task->type);
			return -1;
		}
		format_func((void *)hwts_sqe, hwts_task, model_desc_info);
	}

	devdrv_format_head_sqe((devdrv_hwts_sqe_head_t *)HWTS_SQE_ENTRY(hwts_sq_base, 0));
	devdrv_format_tail_sqe((devdrv_hwts_sqe_head_t *)HWTS_SQE_ENTRY(hwts_sq_base, ts_sqe_num - 1));
	NPU_DRV_DEBUG("devdrv_format_hwts_sqe end\n");
	return ts_sqe_num;
}

int npu_hwts_query_sqe_info(u64 sqe)
{
	u32 i;
	u32 *usqe = NULL;
	devdrv_hwts_sqe_head_t *sqe_head =
		(devdrv_hwts_sqe_head_t *)(uintptr_t)(sqe);

	COND_RETURN_ERROR(sqe_head == NULL, -1, "sqe_head is null\n");
	NPU_DRV_WARN("sqe.type = %u\n", sqe_head->type);
	NPU_DRV_WARN("sqe.ie = %u\n", sqe_head->ie);
	NPU_DRV_WARN("sqe.pre_p = %u\n", sqe_head->pre_p);
	NPU_DRV_WARN("sqe.post_p = %u\n", sqe_head->post_p);
	NPU_DRV_WARN("sqe.wr_cqe = %u\n", sqe_head->wr_cqe);
	NPU_DRV_WARN("sqe.rd_cond = %u\n", sqe_head->rd_cond);
	NPU_DRV_WARN("sqe.l2_lock = %u\n", sqe_head->l2_lock);
	NPU_DRV_WARN("sqe.l2_unlock = %u\n", sqe_head->l2_unlock);
	NPU_DRV_WARN("sqe.block_dim = %u\n", sqe_head->block_dim);
	NPU_DRV_WARN("sqe.stream_id = %u\n", sqe_head->stream_id);
	NPU_DRV_WARN("sqe.task_id = %u\n", sqe_head->task_id);
	usqe = (u32 *)(uintptr_t)(sqe);
	NPU_DRV_WARN("sqe in Hex:\n");
	for (i = 0; i < sizeof(struct devdrv_hwts_kernel_sqe) / sizeof(u32); i++)
		NPU_DRV_WARN("0x%08x\n", usqe[i]);

	return 0;
}
