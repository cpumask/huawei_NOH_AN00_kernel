/*
 * npu_hwts_sqe.h
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
#ifndef _NPU_HWTS_SQE_H_
#define _NPU_HWTS_SQE_H_

#include <linux/types.h>
#include "devdrv_user_common.h"

typedef enum devdrv_hwts_sqe_type {
	DEVDRV_HWTS_SQE_AICORE = 0, // rt 0
	DEVDRV_HWTS_SQE_AICPU = 1,
	DEVDRV_HWTS_SQE_VECTOR_CORE = 2,
	DEVDRV_HWTS_SQE_PLACE_HOLDER = 3, // rt 20 21 22
	DEVDRV_HWTS_SQE_EVENT_RECORD = 4, // rt 2
	DEVDRV_HWTS_SQE_EVENT_WAIT = 5, // rt 3
	DEVDRV_HWTS_SQE_NOTIFY_RECORD = 6, // rt 15
	DEVDRV_HWTS_SQE_NOTIFY_WAIT = 7, // rt 14
	DEVDRV_HWTS_SQE_WRITE_VALUE = 8,
	DEVDRV_HWTS_SQE_MEMCPY = 9, // rt 5
	DEVDRV_HWTS_SQE_TYPE_RESV,
} devdrv_hwts_sqe_type_t;

typedef enum devdrv_hwts_ph_task_type {
	DEVDRV_HWTS_PH_SQE_NORMAL = 0, // rt 20
	DEVDRV_HWTS_PH_SQE_LABEL_SWITCH = 1, // rt 21
	DEVDRV_HWTS_PH_SQE_LABEL_GOTO = 2, // rt 21
	DEVDRV_HWTS_PH_SQE_TYPE_RESV,
} devdrv_hwts_ph_task_type_t;

typedef struct devdrv_hwts_sqe_head {
	u8  type;
	u8  ie : 1;
	u8  pre_p : 1;
	u8  post_p : 1;
	u8  wr_cqe : 1;
	u8  rd_cond : 1;
	u8  res0 : 1;
	u8  l2_lock : 1;
	u8  l2_unlock : 1;
	u16 block_dim;
	u16 stream_id;
	u16 task_id;
} devdrv_hwts_sqe_head_t;

// HWTS_SQE 64B
typedef struct devdrv_hwts_kernel_sqe {
	u32 type:8;
	u32 ie:1;
	u32 pre_p:1;
	u32 post_p:1;
	u32 wr_cqe:1;
	u32 rd_cond:1;
	u32 res0:1;
	u32 l2_lock:1;
	u32 l2_unlock:1;
	u32 block_dim:16;
	u32 stream_id:16;
	u32 task_id:16;
	// 8 Bytes
	u32 pc_addr_low;
	u32 pc_addr_high:16;
	u32 kernel_credit:8;
	u32 res1:3;
	u32 icache_prefetch_cnt:5;
	u32 param_addr_low;
	u32 param_addr_high:16;
	u32 l2_in_main:8;
	u32 res2:8;
	u32 literal_addr_low;
	u32 literal_addr_high:16;
	u32 res3:16;
	u32 literal_base_ub;
	u32 res4;
	u32 literal_buff_len;
	u32 res5;
	u32 l2_ctrl_addr_low;
	u32 l2_ctrl_addr_high:16;
	u32 res6:16;

	u8  res7[8];
} devdrv_hwts_kernel_sqe_t;

typedef struct devdrv_hwts_label_switch_sqe
{
	u64 right;
	u16 true_label_idx;
	u8  condition;
	u8  res0[5];
} devdrv_hwts_label_switch_sqe_t;

typedef struct devdrv_hwts_label_goto_sqe
{
	u16 label_idx;
	u16 res0[7];
} devdrv_hwts_label_goto_sqe_t;

typedef struct devdrv_hwts_ph_sqe
{
	u8  type;
	u8  ie:1;
	u8  pre_p:1;
	u8  post_p:1;
	u8  wr_cqe:1;
	u8  res0:2;
	u8  l2_lock:1;
	u8  l2_unlock:1;
	u16 task_type;
	u16 stream_id;
	u16 task_id;
	// 16 Bytes
	union {
		devdrv_hwts_label_switch_sqe_t label_switch;
		devdrv_hwts_label_goto_sqe_t   label_goto;
	} u;
	u32 res2[10];
} devdrv_hwts_ph_sqe_t;

typedef struct devdrv_hwts_event_sqe
{
	u8  type;
	u8  ie:1;
	u8  pre_p:1;
	u8  post_p:1;
	u8  wr_cqe:1;
	u8  reserved:2;
	u8  l2_lock:1;
	u8  l2_unlock:1;
	u16 res0;
	u16 stream_id;
	u16 task_id;
	// 8 Bytes
	u32 event_id:10;
	u32 res1:22;
	u32 res2:16;
	u32 kernel_credit:8; /* type = 5 only! task timeout limit = kernel_credit * 2^task_timeout_exp*/
	u32 res3:8;
	u32 res4[12];
} devdrv_hwts_event_sqe_t;

typedef struct devdrv_hwts_notify_sqe
{
	u8  type;
	u8  ie:1;
	u8  pre_p:1;
	u8  post_p:1;
	u8  wr_cqe:1;
	u8  reserved:2;
	u8  l2_lock:1;
	u8  l2_unlock:1;
	u16 res0;
	u16 stream_id;
	u16 task_id;
	// 8 Bytes
	u32 notify_id:10;
	u32 res1:22;
	u32 res2:16;
	u32 kernel_credit:8; /* type = 7 only! task timeout limit = kernel_credit * 2^task_timeout_exp*/
	u32 res3:8;
	u32 res4[12];
} devdrv_hwts_notify_sqe_t;

typedef struct devdrv_hwts_write_val_sqe
{
	u8  type;
	u8  ie:1;
	u8  pre_p:1;
	u8  post_p:1;
	u8  wr_cqe:1;
	u8  reserved:2;
	u8  l2_lock:1;
	u8  l2_unlock:1;
	u16 res0;
	u16 stream_id;
	u16 task_id;
	// 8 Bytes
	u32 reg_addr_low;
	u16 reg_addr_high;
	u16 awsize:3; /* 0:8 bits.1:16bits 2:32bits 3:64 bits.4-7:reserved*/
	u16 snoop:1;
	u16 res1:4;
	u16 awcache:4; /* cache property of destnation*/
	u16 awprot:3; /* secure setting,should be setted 2*/
	u16 va:1;
	u32 write_value_low;
	u32 write_value_high;
	u32 res2[10];
} devdrv_hwts_write_val_sqe_t;

typedef struct devdrv_hwts_memcpy_sqe
{
	// 16B head
	u8  type;
	u8  ie:1;
	u8  pre_p:1;
	u8  post_p:1;
	u8  wr_cqe:1;
	u8  reserved:2;
	u8  l2_lock:1;
	u8  l2_unlock:1;
	u16 res0;
	u16 stream_id;
	u16 task_id;
	// 8 Bytes
	u32 res1;
	u32 res2:16;
	u32 kernel_credit:8;
	u32 res3:8;

	// 32B payload;
	u32 ie_dma:1;
	u32 mode:3;
	u32 res4:4;
	u32 w_pattern:8;
	u32 res5:4;
	u32 message0:12;
	u32 src_streamid:8;
	u32 src_substreamid:8;
	u32 dst_streamid:8;
	u32 dst_substreamid:8;
	u32 res6;
	u32 length;
	u32 src_addr_low;
	u32 src_addr_high:16;
	u32 res7:15;
	u32 src_addr_high_p:1;
	u32 dst_addr_low;
	u32 dst_addr_high:16;
	u32 res8:15;
	u32 dst_addr_high_p:1;

	// 16B reserved
	u32 res10[4];
} devdrv_hwts_memcpy_sqe_t;

// HWTS_SQE 16B
typedef struct devdrv_hwts_cqe {
	volatile u16 p : 1;
	volatile u16 w : 1;
	volatile u16 evt : 1;
	volatile u16 res0 : 1;
	volatile u16 sq_id : 10;
	volatile u16 res1 : 2;
	volatile u16 sq_head;
	volatile u16 stream_id;
	volatile u16 task_id;
	volatile u32 syscnt_low; // status
	volatile u32 syscnt_high; // res0
} devdrv_hwts_cqe_t;

/* ----------------------  Runtime sink Task  ---------------------- */
typedef struct devdrv_rt_hwts_kernel_task {
	u64 pc_start;
	u64 param_base;
	u64 l2_preload_ctrl;
	u64 literal_src_addr;
	u32 literal_dst_base;
	u32 literal_size;
	u16 block_dim;
	u8  l2_size;
	u8  priority;
	u8  ai_core_alloc_hint_bw;
	u8  ai_core_alloc_hint_l2bw;
	u8  rd_cond;
	u8  reserved;
} devdrv_rt_hwts_kernel_task_t;

typedef struct devdrv_rt_hwts_label_switch_task {
	u64 left_addr;
	u64 right;
	u16 true_label_idx;
	u16 true_label_id;
	u8  condition;
	u8  reserved[27];
} devdrv_rt_hwts_label_switch_task_t;

typedef struct devdrv_rt_hwts_label_goto_task {
	u16 label_idx;
	u16 label_id;
	u8 reserved[44];
} devdrv_rt_hwts_label_goto_task_t;

typedef struct devdrv_rt_hwts_event_task {
	u16 event_id;
	u8  reserved[46];
} devdrv_rt_hwts_event_task_t;

typedef struct devdrv_rt_hwts_notify_task {
	u16 notify_id;
	u8 reserved[46];
} devdrv_rt_hwts_notify_task_t;

typedef struct devdrv_rt_hwts_memcpy_task {
	u64 src_addr;
	u64 dst_addr;
	u64 length;
	u16 memcpy_type;
	u8  dir;
	u8  reserved[21];
} devdrv_rt_hwts_memcpy_task_t;

/**
* @brief Runtime sink Task 64B (9 kinds of hwts task)
*/
typedef struct devdrv_rt_hwts_task {
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
		devdrv_rt_hwts_kernel_task_t kernel_task;
		// Place holder task
		devdrv_rt_hwts_label_switch_task_t label_switch_task;
		devdrv_rt_hwts_label_goto_task_t label_goto_task;
		devdrv_rt_hwts_event_task_t event_task;
		devdrv_rt_hwts_notify_task_t notify_task;
		// write value task
		devdrv_rt_hwts_memcpy_task_t memcpy_task;
	} u;
} devdrv_rt_hwts_task_t;

int devdrv_verify_hwts_sqe(void *stream_buf_addr, u64 ts_sq_len);
int devdrv_format_hwts_sqe(void *hwts_sq_addr, void *stream_buf_addr,
	u64 ts_sq_len, devdrv_model_desc_info_t *model_desc_info);

int npu_hwts_query_sqe_info(u64 sqe);

#endif
