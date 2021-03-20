/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: airpress sysfs header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __AIRPRESS_SYSFS_H__
#define __AIRPRESS_SYSFS_H__

read_info_t send_airpress_calibrate_cmd(uint8_t tag, unsigned long val,
	RET_TYPE *rtype);
ssize_t show_sensor_read_airpress_common(struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t show_sensor_read_airpress(struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t show_airpress_set_calidata(struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t store_airpress_set_calidata(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size);
#ifdef SENSOR_DATA_ACQUISITION
void airpress_touch_data_acquisition(struct airpress_touch_calibrate_data *cd);
#endif

#endif
