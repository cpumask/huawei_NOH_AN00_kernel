/*
 * npu_dfx_profiling_hwts_adapter.h
 *
 * about npu dfx profiling
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
#ifndef _PROF_DRV_DEV_HWTS_ADAPT_H_
#define _PROF_DRV_DEV_HWTS_ADAPT_H_

#include <linux/cdev.h>
#include "devdrv_user_common.h"
#include "devdrv_user_common_model.h"
#include "npu_platform.h"
#include "npu_iova.h"

#define AI_CPU_MAX_PMU_NUM      8 /*stub*/
#define AI_CORE_MAX_PMU_NUM     8

/* profiling reserved memory size is 4k;
* head size = 4K; TSCPU DATA size  256k;
* AICPU DATA SIZE:2M
* AICORE DATA SIZE 2M
*/
#define PROF_HEAD_MANAGER_SIZE          0x1000
#define PROF_TSCPU_DATA_SIZE            0x40000
#define PROF_HWTS_LOG_SIZE            0x200000
#define PROF_HWTS_PROFILING_SIZE           0x200000

#define PROF_SETTING_INFO_SIZE          256
#define PROF_HWTS_LOG_FORMAT_SIZE       64
#define PROF_HWTS_PROFILING_FORMAT_SIZE       128

enum prof_channel{
	PROF_CHANNEL_TSCPU = 0,
	PROF_CHANNEL_AICPU,
	PROF_CHANNEL_HWTS_LOG = PROF_CHANNEL_AICPU,
	PROF_CHANNEL_AICORE,
	PROF_CHANNEL_HWTS_PROFILING = PROF_CHANNEL_AICORE,
	PROF_CHANNEL_MAX,
};

struct prof_ts_cpu_config {
	uint32_t en; /* enable=1 or disable=0 */
	uint32_t task_state_switch; /* 1 << TsTaskState means report */
	uint32_t task_type_switch; /* 1 << TsTaskType means report */
};

struct prof_ai_cpu_config {
	uint32_t en;
	uint32_t event_num;
	uint32_t event[AI_CPU_MAX_PMU_NUM];
};

struct prof_ai_core_config {
	uint32_t en;
	uint32_t event_num;
	uint32_t event[AI_CORE_MAX_PMU_NUM];
};

struct prof_setting_info {
	struct prof_ts_cpu_config tscpu;
	struct prof_ai_cpu_config aicpu;
	struct prof_ai_core_config aicore;
};

struct ring_buff_manager {
	volatile uint32_t read;
	volatile uint32_t length;
	volatile uint64_t base_addr;
	uint32_t reserved0[12];  /* because cache line size  is 64 bytes*/
	volatile uint32_t write;
	volatile uint32_t iova_addr;
	uint32_t reserved1[14];
};

struct prof_buff_manager {
	union {
		struct prof_setting_info info;
		char data[PROF_SETTING_INFO_SIZE];
	}cfg;
	struct ring_buff_manager ring_buff[PROF_CHANNEL_MAX];
	u32 start_flag;
};

struct prof_buff_desc {
	union {
		struct prof_buff_manager manager;
		char data[PROF_HEAD_MANAGER_SIZE];
	} head;
};

#endif /* _PROF_DRV_DEV_H_ */
