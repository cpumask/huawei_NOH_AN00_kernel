/*
 * devdrv_user_common_model.h
 *
 * about devdrv user model
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
#ifndef __DEVDRV_USER_COMMON_MODEL_H
#define __DEVDRV_USER_COMMON_MODEL_H

#include <linux/types.h>

#define DEVDRV_HWTS_SQ_SLOT_SIZE	64
#define DEVDRV_HWTS_CQ_SLOT_SIZE	16
#define DEVDRV_MAX_HWTS_SQ_DEPTH	1024
#define DEVDRV_MAX_LONG_HWTS_SQ_DEPTH	8192
#define DEVDRV_MAX_HWTS_CQ_DEPTH	1024
#define DEVDRV_MAX_HWTS_SQ_NUM		384
#define DEVDRV_MAX_LONG_HWTS_SQ_NUM      32
#define DEVDRV_MAX_HWTS_CQ_NUM   	20
#define DEVDRV_MAX_HWTS_SQ_POOL_NUM  4  // dont consider the case where the pool size is not the same
#define DEVDRV_MAX_LONG_HWTS_SQ_POOL_NUM  1
#define DEVDRV_MAX_HWTS_CQ_POOL_NUM  1

#define DEVDRV_MAX_HWTS_EVENT_ID		1024
#define DEVDRV_MAX_HWTS_NOTIFY_ID		1024

#define DEVDRV_MAX_MODEL_INFO_NUM   (DEVDRV_MAX_MODEL_ID)
#define DEVDRV_MAX_STREAM_INFO_NUM   (DEVDRV_MAX_NON_SINK_STREAM_ID)
#define DEVDRV_MAX_SINK_STREAM_INFO_NUM   (DEVDRV_MAX_SINK_STREAM_ID)

#define DEVDRV_MAX_SQ_INFO_NUM   DEVDRV_MAX_SQ_NUM
#define DEVDRV_MAX_CQ_INFO_NUM   DEVDRV_MAX_CQ_NUM
#define DEVDRV_MAX_HWTS_SQ_INFO_NUM     DEVDRV_MAX_HWTS_SQ_NUM
#define DEVDRV_MAX_HWTS_CQ_INFO_NUM  DEVDRV_MAX_HWTS_CQ_NUM

#define DEVDRV_MODEL_STREAM_NUM	2
#define DEVDRV_MODEL_EVENT_NUM	128

#define PROF_HEAD_SIZE          0x1000
#define PROF_SETTING_SIZE       256
#define AI_CPU_MAX_PMU_NUM      8
#define AI_CORE_MAX_PMU_NUM     8

typedef struct devdrv_model_desc {
	u16 model_id;
	u16 stream_cnt;
	u16 stream_id[DEVDRV_MODEL_STREAM_NUM];
	u16 stream_tasks[DEVDRV_MODEL_STREAM_NUM];
	void *stream_addr[DEVDRV_MODEL_STREAM_NUM];
} devdrv_model_desc_t;

// temp num, need change to 2, every model include max stream num is 2
#define MODEL_STREAM_MAX_NUM  32
typedef struct devdrv_model_desc_info {
	uint16_t stream_id[MODEL_STREAM_MAX_NUM];
	uint8_t  event_info[DEVDRV_MODEL_EVENT_NUM];
	uint32_t model_id;
	uint32_t stream_cnt;
	uint8_t  reserve0[56];    // 64 bytes cache line align
	uint64_t to_schedule_times;
	uint32_t report_num;
	uint32_t appraisement;
	uint32_t in_schedule;
	uint32_t result;
	uint8_t  reserve1[40];
} devdrv_model_desc_info_t;

struct devdrv_hwts_sq_info {
	u32 head;      // head sqe
	u32 tail;      // tail sqe
	u32 credit;    // resv num
	u32 index;     // sq index
	s32 pid;
	u32 length;    // 1024/8192
	u64 vir_addr;
	u32 stream_num;
	u32 rsd;
	u64 send_count;
	u64 hwts_sq_sub;
	u64 iova_addr;
};

struct devdrv_hwts_cq_info {
	u32 head;
	u32 tail;
	volatile u32 count_report;
	u32 index;
	u32 phase;
	u32 int_flag;
	s32 pid;
	u32 resv;
	u64 vir_addr;
	u64 receive_count;
	u64 hwts_cq_sub;
	u64 iova_addr;

	u32 stream_num;
	u8 slot_size;
	u8 rsd[3];
};

enum profiling_channel{
	PROF_TSCPU_CHANNEL = 0,
	PROF_HWTS_LOG_CHANNEL,
	PROF_HWTS_PROFILING_CHANNEL,
	PROF_CHANNEL_MAX_NUM,
};

struct profiling_ts_cpu_config {
    uint32_t en;              /* enable=1 or disable=0 */
    uint32_t taskStateSwitch; /* 1 << TsTaskState means report */
    uint32_t taskTypeSwitch;  /* 1 << TsTaskType means report */
};

struct profiling_ai_cpu_config {
    uint32_t en;
    uint32_t eventNum;
    uint32_t event[AI_CPU_MAX_PMU_NUM];
};

struct profiling_ai_core_config {
    uint32_t en; /* en = 1 when open hwts log */
    uint32_t eventNum; /* eventNum > 0 when open hwts profiling */
    uint32_t event[AI_CORE_MAX_PMU_NUM];
};

struct profiling_setting_info {
    struct profiling_ts_cpu_config tscpu;
    struct profiling_ai_cpu_config aicpu; /* onetrack for profiling driver */
    struct profiling_ai_core_config aicore;
};

struct buff_ring_manager {
	volatile u32 read;
	volatile u32 length;
	volatile u64 base_addr;
	u32 rsd0[12]; /* because cache line size  is 64 bytes */
	volatile u32 write;
	volatile u32 iova_addr;
	u32 rsd1[14];
};

struct prof_buff_manager_info {
    union {
        struct profiling_setting_info info;
        char data[PROF_SETTING_SIZE];
    } cfg;
    struct buff_ring_manager ring_buff[PROF_CHANNEL_MAX_NUM];
    u32 start_flag;
};

struct devdrv_prof_info {
	union {
		struct prof_buff_manager_info manager;
		char data[PROF_HEAD_SIZE];
	} head;
};

/* The info memory of reserved memory order: 1.SQ_INFO, 2.CQ_INFO, 3.STREAM_INFO */
/* 4.HWTS_SQ_INFO, 5.HWTS_CQ_INFO, 6.MODEL_INFO 7.PROFILINGL_INFO 8.TS_STAUS_INFO */

/* reserved memory info size */
#define DEVDRV_HWTS_SQ_INFO_SIZE	sizeof(struct devdrv_hwts_sq_info)    // 64
#define DEVDRV_HWTS_CQ_INFO_SIZE	sizeof(struct devdrv_hwts_cq_info)    // 16
#define DEVDRV_MODEL_INFO_SIZE	sizeof(struct devdrv_model_desc_info)    // 1536

#define DEVDRV_HWTS_SQ_INFO_OCCUPY_SIZE \
	(DEVDRV_HWTS_SQ_INFO_SIZE * DEVDRV_MAX_HWTS_SQ_NUM)
#define DEVDRV_HWTS_CQ_INFO_OCCUPY_SIZE \
	(DEVDRV_HWTS_CQ_INFO_SIZE * DEVDRV_MAX_HWTS_CQ_NUM)
#define DEVDRV_MODEL_INFO_OCCUPY_SIZE   \
	(DEVDRV_MODEL_INFO_SIZE * DEVDRV_MAX_MODEL_INFO_NUM)
#define DEVDRV_PROFILINGL_INFO_OCCUPY_SIZE  (PROF_HEAD_SIZE)

/* reserved memory info offset */
#define DEVDRV_HWTS_SQ_INFO_OFFSET (DEVDRV_SQ_INFO_OCCUPY_SIZE + DEVDRV_CQ_INFO_OCCUPY_SIZE + \
	DEVDRV_STREAM_INFO_OCCUPY_SIZE)
#define DEVDRV_HWTS_CQ_INFO_OFFSET (DEVDRV_HWTS_SQ_INFO_OFFSET + DEVDRV_HWTS_SQ_INFO_OCCUPY_SIZE)
#define DEVDRV_MODEL_INFO_OFFSET (DEVDRV_HWTS_CQ_INFO_OFFSET + DEVDRV_HWTS_CQ_INFO_OCCUPY_SIZE)
#define DEVDRV_PROFILINGL_INFO_OFFSET (DEVDRV_MODEL_INFO_OFFSET + DEVDRV_MODEL_INFO_OCCUPY_SIZE)

/* reserved memory data size */
#define DEVDRV_SQ_OCCUPY_SIZE \
	(DEVDRV_MAX_SQ_NUM * DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE)
#define DEVDRV_CQ_OCCUPY_SIZE \
	(DEVDRV_MAX_CQ_NUM * DEVDRV_MAX_CQ_DEPTH * DEVDRV_CQ_SLOT_SIZE)

/* reserved memory DFX data size */
#define DEVDRV_DFX_SQ_OCCUPY_SIZE    \
	(DEVDRV_MAX_DFX_SQ_NUM * DEVDRV_MAX_SQ_DEPTH * DEVDRV_SQ_SLOT_SIZE)
#define DEVDRV_DFX_CQ_OCCUPY_SIZE    \
	(DEVDRV_MAX_DFX_CQ_NUM * DEVDRV_MAX_CQ_DEPTH * DEVDRV_CQ_SLOT_SIZE)
#define DEVDRV_DFX_SQ_INFO_OCCUPY_SIZE    \
	(DEVDRV_MAX_DFX_SQ_NUM * DEVDRV_SQ_INFO_SIZE)
#define DEVDRV_DFX_CQ_INFO_OCCUPY_SIZE    \
	(DEVDRV_MAX_DFX_CQ_NUM * DEVDRV_CQ_INFO_SIZE)
#define DEVDRV_DFX_OCCUPY_SIZE    \
	((DEVDRV_DFX_SQ_OCCUPY_SIZE) + (DEVDRV_DFX_CQ_OCCUPY_SIZE) + \
		(DEVDRV_DFX_SQ_INFO_OCCUPY_SIZE) + (DEVDRV_DFX_CQ_INFO_OCCUPY_SIZE))

#define DEVDRV_TS_LOG_OCCUPY_SIZE 0x80000  // 512k
#define DEVDRV_BLACKBOX_OCCUPY_SIZE 0x40000 // 256k

/* sysdma related */
#define NPU_SDMA_CHANNEL_SHIFT 8
#define NPU_HWTS_SDMA_CHANNEL_NUM 4 /* 0-3:HWTS, 4-7:reserved */
#define NPU_HWTS_CFG_BASE 0xE4300000
#define NPU_HWTS_SDMA_CQ_CFG_OFFSET 0xA0000 /* 0xE43A0000 */
/* Each 4KB is reserved for each SDMA CQ to write back to HWTS
 * in order to notify a completion status, The size is allowed
 * to 16B in this region, as there's no actual nManager registers
 */
#define NPU_HWTS_SDMA_CQ_OFFSET 0x2000
#define SDMA_CHN_QOS 3 /* 4 level: 3>2>1>0 */
#define SDMA_SQ_SIZE 10 /* sdma sq len: 2^SDMA_SQ_SIZE max len 65536-1*/
#define SDMA_CQ_SIZE 9 /* sdma cq len: 2^SDMA_CQ_SIZE, max len 512-1*/
#define SDMA_CQ_ENTRIES (1 << SDMA_CQ_SIZE)
#define SDMA_SQ_ENTRIES (1 << SDMA_SQ_SIZE)
#define SDMA_SQE_SIZE 32
#define SMMU_BYPASS_SID 63

#endif
