/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: thermometer detect header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __THERM_DETECT_H__
#define __THERM_DETECT_H__

#define THERM_DEV_COUNT_MAX 1

struct therm_platform_data {
	struct sensor_combo_cfg cfg;
	uint8_t blackbody_count;
	uint8_t ar_mode;
	int k1;
	int k2;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
};

struct therm_device_info {
	int32_t obj_tag;
	uint32_t detect_list_id;
	uint8_t therm_dev_index;
	RET_TYPE therm_calibration_res;
};

struct therm_platform_data *therm_get_platform_data(int32_t tag);
struct therm_device_info *therm_get_device_info(int32_t tag);
char *therm_get_sensors_id_string(void);
void therm_get_sensors_id_from_dts(struct device_node *dn);
char *therm_get_calibrate_index_string(void);
void read_thermometer_data_from_dts(struct device_node *dn);
int thermometer_data_from_mcu(const pkt_header_t *head);
void therm_detect_init(struct sensor_detect_manager *sm, uint32_t len);

#endif
