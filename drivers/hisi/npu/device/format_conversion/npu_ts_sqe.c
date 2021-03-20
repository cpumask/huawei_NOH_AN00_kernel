/*
 * npu_ts_sqe.c
 *
 * about npu ts sqe
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
#include "npu_ts_sqe.h"

#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <securec.h>

#include "npu_common.h"
#include "npu_log.h"
#include "devdrv_user_common.h"

typedef int (*verify_ts_task_func)(devdrv_ts_comm_sqe_t *ts_task);
typedef void (*format_ts_sqe_func)(void *ts_sqe_addr, devdrv_ts_comm_sqe_t *ts_task);

#define TS_SQE_ENTRY(ts_sq_addr, offset) ((ts_sq_addr) + (offset) * DEVDRV_RT_TASK_SIZE)

int verify_sqe_comm_field(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->type >= DEVDRV_TS_SQE_RESERVED) {
		NPU_DRV_ERR("invalid task type:%u, task_id:%u\n", ts_task->type, ts_task->task_id);
		return -1;
	}
	if (ts_task->stream_id > DEVDRV_MAX_NON_SINK_STREAM_ID) {
		NPU_DRV_ERR("invalid task stream_id:%u, task_id:%u\n", ts_task->stream_id, ts_task->task_id);
		return -1;
	}
	if (ts_task->task_id < DEVDRV_MAX_TASK_START_ID || ts_task->task_id > UINT16_MAX) {
		NPU_DRV_ERR("invalid task_id:%u\n", ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_evt_rec_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.event_sqe.event_id >= DEVDRV_MAX_EVENT_ID) {
		NPU_DRV_ERR("invalid event_id:%u, task_id:%u\n",
			ts_task->u.event_sqe.event_id, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_wait_evt_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.event_sqe.event_id >= DEVDRV_MAX_EVENT_ID) {
		NPU_DRV_ERR("invalid event_id:%u, task_id:%u\n",
			ts_task->u.event_sqe.event_id, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_fusion_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.fusion_sqe.flag > DEVDRV_TS_FUSION_END) {
		NPU_DRV_ERR("invalid fusion_sqe.flag:%u, task_id:%u\n",
			ts_task->u.fusion_sqe.flag, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_memcpy_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.memcpy_sqe.src_addr & NPU_BIT_MASK(4) ||
		ts_task->u.memcpy_sqe.src_addr == 0) {
		NPU_DRV_ERR("invalid src_addr not 16B align or is 0, sdma task_id:%u\n",
			ts_task->task_id);
		return -1;
	}
	if (ts_task->u.memcpy_sqe.dst_addr & NPU_BIT_MASK(4) ||
		ts_task->u.memcpy_sqe.dst_addr == 0) {
		NPU_DRV_ERR("invalid dst_addr not 16B align or is 0, sdma task_id:%u\n",
			ts_task->task_id);
		return -1;
	}
	if (ts_task->u.memcpy_sqe.length & NPU_BIT_MASK(4) ||
		ts_task->u.memcpy_sqe.length == 0) {
		NPU_DRV_ERR("invalid length:%llu not 16B align or is 0, sdma task_id:%u\n",
			ts_task->u.memcpy_sqe.length, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_maintenance_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.maintenance_sqe.goal > DEVDRV_TS_MT_EVENT_DESTROY) {
		NPU_DRV_ERR("invalid maintenance_sqe.goal:%u, task_id:%u\n",
			ts_task->u.maintenance_sqe.goal, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_create_stream_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.create_stream_sqe.sq_id >= DEVDRV_MAX_SQ_NUM) {
		NPU_DRV_ERR("invalid create_stream_sqe.sq_id:%u, task_id:%u\n",
			ts_task->u.create_stream_sqe.sq_id, ts_task->task_id);
		return -1;
	}
	if (ts_task->u.create_stream_sqe.priority > DEVDRV_MAX_STREAM_PRIORITY) {
		NPU_DRV_ERR("invalid create_stream_sqe.priority:%u, task_id:%u\n",
			ts_task->u.create_stream_sqe.priority, ts_task->task_id);
		return -1;
	}
	return 0;
}

int verify_model_maintenance_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.model_maintenance_sqe.model_id >= DEVDRV_MAX_MODEL_ID) {
		NPU_DRV_ERR("invalid model_maintenance_sqe.model_id:%u, task_id:%u\n",
			ts_task->u.model_maintenance_sqe.model_id, ts_task->task_id);
		return -1;
	}
	if (ts_task->u.model_maintenance_sqe.operation_type >= DEVDRV_TS_MMT_RESERVED) {
		NPU_DRV_ERR("invalid model_maintenance_sqe.operation_type:%u, task_id:%u\n",
			ts_task->u.model_maintenance_sqe.operation_type, ts_task->task_id);
		return -1;
	}
	/* no need to verify ts_task->u.model_maintenance_sqe.first_task_id, since it's not used after the first edition */
	return 0;
}

int verify_model_execute_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	if (ts_task->u.model_execute_sqe.model_id >= DEVDRV_MAX_MODEL_ID) {
		NPU_DRV_ERR("invalid model_execute_sqe.model_id:%u, task_id:%u\n",
			ts_task->u.model_execute_sqe.model_id, ts_task->task_id);
		return -1;
	}
	// verify pid
	if (ts_task->u.model_execute_sqe.pid == 0)
		NPU_DRV_ERR("invalid model_execute_sqe.pid:%u, task_id:%u\n",
			ts_task->u.model_execute_sqe.pid, ts_task->task_id);

	/* no need to verify ts_task->u.model_maintenance_sqe.first_task_id, since it's not used after the first edition */
	return 0;
}

int verify_profiling_enable_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	return 0;
}

int verify_profiling_disable_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	return 0;
}

int verify_bypass_task_sqe(devdrv_ts_comm_sqe_t *ts_task)
{
	return 0;
}

verify_ts_task_func verify_ts_sqe_map[] = {
/*	0								1								2						3			*/
	NULL,							NULL,							verify_evt_rec_sqe,		verify_wait_evt_sqe,
/*	4								5								6						7			*/
	NULL,							verify_memcpy_sqe,				verify_maintenance_sqe,	verify_create_stream_sqe,
/*	8								9								10						11			*/
	NULL,							NULL,							NULL,					NULL,
/*	12								13								14						15			*/
	verify_model_maintenance_sqe,	verify_model_execute_sqe,		NULL,					NULL,
/*	16								17								18						19			*/
	NULL,							NULL,							NULL,					NULL,
/*	20								21								22						23			*/
	NULL,							NULL,							NULL,					verify_bypass_task_sqe,
/*	24								25								26						27			*/
	verify_bypass_task_sqe,			NULL,							NULL,					NULL,
/*	28								29								30						31			*/
	NULL,							NULL,							NULL,					NULL,
/*	32								33								34						35			*/
	NULL,							NULL,							NULL,					NULL,
/*	36								37								38						39			*/
	NULL,							NULL,							NULL,					NULL,
/*	40								41								42						43			*/
	NULL,							NULL,							NULL,					NULL,
/*	44								45								46						47			*/
	NULL,							NULL,							NULL,					NULL,
/*	48								49								50						51			*/
	NULL,							NULL,							NULL,					NULL,
/*	52								53								54						55			*/
	NULL,							NULL,							NULL,					NULL,
/*	56								57								58						59			*/
	NULL,							NULL,							NULL,					NULL,
/*	60								61								62						63			*/
	NULL,							NULL,							NULL,					NULL,
/*	64								65								66						67			*/
	verify_profiling_enable_sqe,	verify_profiling_disable_sqe,	NULL,					NULL,
};

int devdrv_verify_ts_sqe(void *ts_task)
{
	verify_ts_task_func verify_func = NULL;
	int ret;
	devdrv_ts_comm_sqe_t *ts_sqe = (devdrv_ts_comm_sqe_t *)ts_task;
	NPU_DRV_DEBUG("ts_task_addr:%pK", ts_task);

	ret = verify_sqe_comm_field(ts_sqe);
	if (ret != 0) {
		NPU_DRV_ERR("verify_sqe_comm_field failed, ret:%d, task_id:%u, type:%u\n",
			ret, ts_sqe->task_id, ts_sqe->type);
		return -1;
	}

	verify_func = verify_ts_sqe_map[ts_sqe->type];
	if (verify_func == NULL) {
		NPU_DRV_ERR("invalid task_id:%u, type:%u\n", ts_sqe->task_id, ts_sqe->type);
		return -1;
	}
	ret = verify_func(ts_sqe);
	if (ret != 0) {
		NPU_DRV_ERR("verify_ts_sqe failed, ret:%d, task_id:%u, type:%u\n",
			ret, ts_sqe->task_id, ts_sqe->type);
		return -1;
	}

	NPU_DRV_DEBUG("devdrv_verify_ts_sqe end\n");
	return 0;
}

int devdrv_format_ts_sqe(void *ts_sqe_addr, devdrv_ts_comm_sqe_t *ts_task, u32 index)
{
	devdrv_ts_comm_sqe_t *comm_sqe_addr = NULL;
	if (ts_sqe_addr == NULL) {
		NPU_DRV_ERR("ts_sqe_addr is null\n");
		return -1;
	}
	COND_RETURN_ERROR(index >= DEVDRV_MAX_SQ_DEPTH, -1, "illegal sq index:%u\n", index);
	/* runtime task structure same as comm ts sqe, add to tail sqe */
	comm_sqe_addr = (devdrv_ts_comm_sqe_t *)TS_SQE_ENTRY(ts_sqe_addr, index);
	NPU_DRV_DEBUG("ts_sqe_addr= %pK, comm_sqe_addr= %pK, index= %u, size= %u, type=%d\n",
		ts_sqe_addr, comm_sqe_addr, index, DEVDRV_RT_TASK_SIZE, comm_sqe_addr->type);
	if (memcpy_s(comm_sqe_addr, DEVDRV_RT_TASK_SIZE, ts_task, DEVDRV_RT_TASK_SIZE) != 0) {
		NPU_DRV_ERR("memcpy_s failed\n");
		return -1;
	}
	return 0;
}
