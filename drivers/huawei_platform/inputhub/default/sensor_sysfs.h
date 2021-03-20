/*
 * drivers/inputhub/sensor_sysfs.h
 *
 * sensors sysfs header
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

#ifndef __SENSOR_SYSFS_H
#define __SENSOR_SYSFS_H

#include "sensor_config.h"
#include "contexthub_route.h"
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif

#define SENSOR_LIST_NUM 50
#define DEBUG_DATA_LENGTH 10

#define CALIBRATE_DATA_LENGTH 15
#define TO_DECIMALISM 10
#define BITE_MOVE_FLAG 48
#define BITE_MOVE_BUS_NUM 40
#define BITE_MOVE_ADDR 32
#define BITE_MOVE_BUF_TEMP 24
#define BITE_MOVE_LEN 16
#define BITE_MOVE_RW 8
#define I2C_REG_TW 2
#define I2C_REG_THR 3
#define I2C_REG_FO 4

struct pdr_start_config {
	unsigned int report_interval;
	unsigned int report_precise;
	unsigned int report_count;
	unsigned int report_times;
};

struct i2c_i3c_rw_info {
	uint8_t i2c_i3c;
	uint8_t bus_num;
	uint8_t i2c_address;
	uint8_t len;
	uint8_t rw;
	uint8_t buf_temp[DEBUG_DATA_LENGTH];
};

typedef struct {
	unsigned int sub_cmd;
	union {
		struct pdr_start_config start_param;
		unsigned int stop_param;
	};
} pdr_ioctl_t;

enum semteck_thredhold {
	OFFSET_MIN_THREDHOLD,
	OFFSET_MAX_THREDHOLD,
	DIFF_MIN_THREDHOLD,
	DIFF_MAX_THREDHOLD
};

enum ps_tof_calibrate_order {
	PS_XTALK_CALIBRATE = 0x01,
	PS_5CM_CALIBRATE,    /* 2 */
	PS_3CM_CALIBRATE,    /* 3 */
	TOF_ZERO_CALIBRATE,  /* 4 */
	TOF_6CM_CALIBRATE,   /* 5 */
	TOF_10CM_CALIBRATE,  /* 6 */
	TOF_60CM_CALIBRATE,  /* 7 */
	PS_MINPDATA_MODE,    /* 8 */
	PS_SAVE_MINPDATA,    /* 9 */
	TOF_CLOSE = 10,
	TOF_OPEN = 11,
	TOF_CALI_FOR_FIX = 12,
	PS_SCREEN_ON_XTALK_CALIBRATE = 13,
	PS_SCREEN_ON_5CM_CALIBRATE, /* 14 */
	PS_SCREEN_ON_3CM_CALIBRATE, /* 15 */
};

extern struct hisi_nve_info_user user_info;

extern struct tof_platform_data tof_data;

extern sys_status_t iom3_sr_status;
extern volatile int vibrator_shake;

#ifdef CONFIG_HUAWEI_DSM
#ifdef CONFIG_HUAWEI_DATA_ACQUISITION
#define SENSOR_DATA_ACQUISITION
#endif
#endif

#ifdef SENSOR_DATA_ACQUISITION
#define NA                          "NA"
#define sensor_cal_result(x) (((x) == SUC) ? "pass" : "fail")

#define MAX_COPY_EVENT_SIZE          32
#define MAX_CAL_TEST_NAME_LEN        32
#define MAX_CAL_RESULT_LEN           8

struct sensor_eng_cal_test {
	int first_item;
	int32_t *cal_value;
	int value_num;
	int32_t *min_threshold;
	int32_t *max_threshold;
	int threshold_num;
	char name[MAX_CAL_TEST_NAME_LEN];
	char result[MAX_CAL_RESULT_LEN];
	char *test_name[MAX_COPY_EVENT_SIZE];
};
#endif

extern uint8_t tof_register_value;
extern int hall_sen_type;
extern int sensor_tof_flag;
extern u8 tplcd_manufacture;
extern u8 phone_color;
extern int get_airpress_data;
extern int get_temperature_data;
extern bool fingersense_data_intrans;
extern bool fingersense_data_ready;
extern uint8_t gyro_cali_way;
extern uint8_t acc_cali_way;
extern RET_TYPE handpress_calibration_res;
extern uint16_t sensorlist[SENSOR_LIST_NUM];
extern int calibrate_processing;
extern char *sensor_in_board_status;

extern void enable_motions_when_recovery_iom3(void);
extern void disable_motions_when_sysreboot(void);

#ifdef SENSOR_DATA_ACQUISITION
int enq_msg_data_in_sensorhub_single(struct event events);
void enq_notify_work_sensor(struct sensor_eng_cal_test sensor_test);
#endif
void do_tof_calibrate(unsigned long val);
read_info_t send_calibrate_cmd(uint8_t tag, unsigned long val,
	RET_TYPE *rtype);
int read_calibrate_data_from_nv(int nv_number, int nv_size,
	const char *nv_name);
int fingersense_commu(unsigned int cmd, unsigned int pare,
	unsigned int responsed, bool is_subcmd);
int fingersense_enable(unsigned int enable);
ssize_t sensors_calibrate_show(int tag, struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t show_get_sensors_id(int tag, struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t sensors_calibrate_store(int tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);
int ois_commu(int tag, unsigned int cmd, unsigned int pare,
	unsigned int responsed, bool is_subcmd);
ssize_t attr_handpress_calibrate_write(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);
int write_tof_offset_to_nv(uint8_t *temp, uint16_t length);

#endif /* __SENSOR_SYSFS_H */
