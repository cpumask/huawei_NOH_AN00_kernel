/*
 * npu_ts_sqe.h
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
#ifndef _NPU_TS_SQE_H_
#define _NPU_TS_SQE_H_

#include <linux/types.h>

#define DEVDRV_MAX_LABEL_ID	      1024

/**
* @ingroup devdrv
* @brief the type defination of task
*/
typedef enum devdrv_ts_task_type {
	DEVDRV_TS_SQE_KERNEL_AICORE = 0,	    /* AI core task */
	DEVDRV_TS_SQE_KERNEL_AICPU = 1,	   /* AI cpu task */
	DEVDRV_TS_SQE_EVENT_RECORD = 2,	   /* event record task */
	DEVDRV_TS_SQE_STREAM_WAIT_EVENT = 3,	  /* stream wait event task */
	DEVDRV_TS_SQE_FUSION_ISSUE = 4,	     /* fusion issue task */
	DEVDRV_TS_SQE_MEMCPY = 5,	          /* memory copy task */
	DEVDRV_TS_SQE_MAINTENANCE = 6,	      /* such as destroy the event or stream */
	DEVDRV_TS_SQE_CREATE_STREAM = 7,	   /* create stream task */
	DEVDRV_TS_SQE_REMOTE_EVENT_WAIT = 9,	  /* wait for event on another device */
	DEVDRV_TS_SQE_PCTRACE_ENABLE = 10,
	DEVDRV_TS_SQE_CREATE_L2_ADDR = 11,	  /* create L2 addr info for aicpu kernel */
	DEVDRV_TS_SQE_MODEL_MAINTAINCE = 12,
	DEVDRV_TS_SQE_MODEL_EXECUTE = 13,
	DEVDRV_TS_SQE_RDMA_SEND = 16,	     /* hccl rdma send task */
	DEVDRV_TS_SQE_L2_SDMA_TASK_MEMCPY = 17,	 /* test l2 task memory copy task */
	DEVDRV_TS_SQE_STREAM_SWITCH = 18,
	DEVDRV_TS_SQE_STREAM_ACTIVE = 19,
	DEVDRV_TS_SQE_LABEL_SET = 20,	  /* set label for control flow ops */
	DEVDRV_TS_SQE_LABEL_SWITCH = 21,	  /* switch label for control flow ops */
	DEVDRV_TS_SQE_LABEL_GOTO = 22,	  /* goto label for control flow ops */
	DEVDRV_TS_SQE_PROFILING_ENABLE = 0x40,
	DEVDRV_TS_SQE_PROFILING_DISABLE = 0x41,
	DEVDRV_TS_SQE_RESERVED = 0x42,
} devdrv_ts_task_type_t;

typedef enum devdrv_ts_task_state {
	DEVDRV_TS_TASK_STATE_INIT = 0,
	DEVDRV_TS_TASK_STATE_WAIT = 1,
	DEVDRV_TS_TASK_STATE_RUN = 2,
	DEVDRV_TS_TASK_STATE_COMPLETE = 3,
	DEVDRV_TS_TASK_STATE_PENDING = 4,
	DEVDRV_TS_TASK_STATE_SDMA_PROCESS_FAILED = 5,
	DEVDRV_TS_TASK_STATE_SDMA_PROCESS_SUCCESS = 6,
} devdrv_ts_task_state_t;

typedef enum devdrv_ts_report_type {
	DEVDRV_TS_REPORT_TYPE_TASK = 0,                 /* task command report */
	DEVDRV_TS_REPORT_TYPE_PROFILE_TIMELINE = 1,     /* timeline profile data */
	DEVDRV_TS_REPORT_TYPE_PROFILE_TASKBASED = 2,    /* task based profile data */
	DEVDRV_TS_REPORT_TYPE_RECYCLE_SQ_FINISHED = 3,  /* recycle sq report */
	DEVDRV_TS_REPORT_TYPE_RESERVED,
} devdrv_ts_report_type_t;

enum devdrv_ts_mmt_stream_type {
	DEVDRV_TS_MMT_HEAD_STREAM = 0,               /* model first stream */
	DEVDRV_TS_MMT_WAIT_ACTIVE_STREAM = 1,        /* model non-first stream */
};

typedef struct devdrv_ts_event_sqe {
	u16 event_id;
	u8 reserved[46];
} devdrv_ts_event_sqe_t;

enum devdrv_ts_fusion_flag {
	DEVDRV_TS_FUSION_BEGIN = 0,
	DEVDRV_TS_FUSION_END = 1,
};

typedef struct devdrv_ts_fusion_sqe {
	u16 flag;
	u8 reserved[46];
} devdrv_ts_fusion_sqe_t;

typedef struct devdrv_ts_memcpy_sqe {
	u64 src_addr;
	u64 dst_addr;
	u64 length;
	u16 memcpy_type;
	u8  dir;
	u8  reserved[21];
} devdrv_ts_memcpy_sqe_t;

enum devdrv_ts_mmt_type {
	DEVDRV_TS_MT_STREAM_DESTROY = 0,
	DEVDRV_TS_MT_EVENT_DESTROY,
};

typedef struct devdrv_ts_maintenance_sqe {
	u16 goal;                      /**< destory 0:stream, 1:event;*/
	u16 target_id;
	u8  reserved[44];
} devdrv_ts_maintenance_sqe_t;

typedef struct devdrv_ts_create_stream_sqe {
	u64 pid;
	u64 l2_base_vaddr;
	u64 asid_baddr;
	u64 tcr;
	u32 thread_id;
	u16 asid;
	u16 smmu_svm_ssid; /* sub_stream_id */
	u16 sq_id;
	u8  priority;
	u8  reserved[5];
} devdrv_ts_create_stream_sqe_t;

typedef enum devdrv_ts_mmt_operation_type {
	DEVDRV_TS_MMT_STREAM_BIND = 0,       /* model stream bind */
	DEVDRV_TS_MMT_STREAM_UNBIND = 1,     /* model stream unbind */
	DEVDRV_TS_MMT_MODEL_CREATE = 2,      /* model create by task pool */
	DEVDRV_TS_MMT_MODEL_DESTROY = 3,     /* model destroy */
	DEVDRV_TS_MMT_MODEL_PRE_PROC = 4,
	DEVDRV_TS_MMT_RESERVED
} devdrv_ts_mmt_operation_type_t;

typedef struct devdrv_ts_model_maintenance_sqe {
	u16 model_id;
	u16 stream_id;
	u16 operation_type;
	u16 stream_type;
	u16 first_task_id;
	u8  reserved[38];
} devdrv_ts_model_maintenance_sqe_t;

typedef struct devdrv_ts_model_execute_sqe {
	u16 model_id;
	u16 first_task_id;
	u8  res0[4];
	u64 asid_baddr;
	u64 tcr;
	u16 asid;
	u16 smmu_svm_ssid; /* sub_stream_id */
	u32 pid;
	u32 execute_pid;
	u32 sequence;
	u8 priority;
	u8 reserved[7];
} devdrv_ts_model_execute_sqe_t;

/**
* @brief TS SQE 64B (same to Runtime non_sink Task 64B)
*/
typedef struct devdrv_ts_comm_sqe {
	/* 16 bytes */
	u16 stream_id;
	u16 task_id;
	u16 next_task_id;
	u16 type;
	u16 next_stream_id;
	u16 task_state;
	u8  task_prof_en;
	u8  reserved[3];
	/* 48 bytes */
	union {
		devdrv_ts_event_sqe_t event_sqe; // type 2,3
		devdrv_ts_fusion_sqe_t fusion_sqe; // type 4
		devdrv_ts_memcpy_sqe_t memcpy_sqe; // type 5
		devdrv_ts_maintenance_sqe_t maintenance_sqe; // type 6
		devdrv_ts_create_stream_sqe_t create_stream_sqe; // type 7
		devdrv_ts_model_maintenance_sqe_t model_maintenance_sqe; // type 12
		devdrv_ts_model_execute_sqe_t model_execute_sqe; // type 13
	} u;
} devdrv_ts_comm_sqe_t;

/**
* @ response receive control para
*/
struct devdrv_receive_response_info {
	u64 response_addr;
	u32 response_count;
	int wait_result;
	int timeout;
};

/**
* @ingroup devdrv
* @brief the struct define of report msg when task is completed
*/
typedef struct devdrv_ts_task_report_msg {
	volatile u16 sop : 1; /* start of packet, indicates this is the first 32bit return payload */
	volatile u16 mop : 1; /* middle of packet, indicates the payload is a continuation of previous task return payload */
	volatile u16 eop : 1; /* end of packet, indicates this is the last 32bit return payload.
	sop & eop can appear in the same packet, mop & eop can also appear on the same packet */
	volatile u16 report_type : 3;
	volatile u16 stream_id : 10;
	volatile u16 task_id;
	volatile u32 pay_load;
	volatile u16 sq_id : 9;
	volatile u16 reserved : 6;
	volatile u16 phase : 1;
	volatile u16 sq_head;
} devdrv_ts_task_report_msg_t;

int devdrv_verify_ts_sqe(void *ts_task);
int devdrv_format_ts_sqe(void *ts_sqe_addr, devdrv_ts_comm_sqe_t *ts_task, u32 index);

#endif
