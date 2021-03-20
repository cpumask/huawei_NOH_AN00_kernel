/*
 * npu_dfx_profiling.h
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
#ifndef _PROF_DRV_DEV_H_
#define _PROF_DRV_DEV_H_

#include <linux/cdev.h>
#include "devdrv_user_common.h"

#ifdef NPU_ARCH_V100
#include "npu_dfx_profiling_bs_adapter.h"
#else
#include "npu_dfx_profiling_hwts_adapter.h"
#endif

#ifndef STATIC
#define STATIC
#endif

#define CHAR_DRIVER_NAME  "prof_drv"
#define DRV_MODE_VERSION  "DAVINCI 1.01"

#define PROF_SETTING_INFO_SIZE          256

#define PROF_OK                         0
#define PROF_ERROR                      (-1)

struct prof_read_config{
	char __user *dest_buff;
	uint32_t dest_size;
	uint32_t src_size;
	uint32_t channel;
};

/* profiling command */
enum prof_cmd_type {
	PROF_SETTING = 201,
	PROF_START,
	PROF_READ,
	PROF_STOP,
	PROF_CMD_MAX
};

typedef struct char_device {
	struct class *dev_class;
	struct cdev   cdev;
	dev_t devno;
	struct device *device;
} char_device_t;

char *get_prof_channel_data_addr (struct prof_buff_desc *prof_buff, u32 channel);
u32 get_prof_channel_data_size(struct prof_buff_desc *prof_buff ,u32 channel);

int prof_buffer_init (struct prof_buff_desc *prof_buff);
int prof_buffer_release (struct prof_buff_desc *prof_buff);

#endif /* _PROF_DRV_DEV_H_ */
