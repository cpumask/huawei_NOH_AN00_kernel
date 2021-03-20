/*
 * drivers/inputhub/sensor_feima.c
 *
 * sensors feima header file
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

#ifndef __SENSOR_FEIMA_H__
#define __SENSOR_FEIMA_H__

#include <linux/mtd/hisi_nve_interface.h>
#include "sensor_config.h"

#define ALS_DBG_PARA_SIZE 8
#define BUF_SIZE 128
#define ALS_MCU_HAL_CONVER 10
#define ACC_CONVERT_COFF 1000
#define ELE_PHONE_TYPE 47
#define VOGUE_PHONE_TYPE 48

extern struct hisi_nve_info_user user_info;

struct sensor_cookie {
	int tag;
	const char *name;
	const struct attribute_group *attrs_group;
	struct device *dev;
};

typedef struct {
	uint16_t sub_cmd;
	uint16_t sar_info;
} rpc_ioctl_t;

enum {
	CALL_START = 0xa2,
	CALL_STOP,
};

extern void send_lcd_freq_to_sensorhub(uint32_t lcd_freq);
extern void save_light_to_sensorhub(uint32_t mipi_level, uint32_t bl_level);
int posture_sensor_enable(void);
void report_fold_status_when_poweroff_charging(int status);
#endif /* __SENSOR_FEIMA_H__ */
