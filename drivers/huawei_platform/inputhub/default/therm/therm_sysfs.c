/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: therm sysfs source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/of.h>
#include <linux/rtc.h>
#include <linux/of_device.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>
#include <linux/completion.h>
#include <linux/hwspinlock.h>
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#include "contexthub_route.h"
#include "protocol.h"
#include "sensor_sysfs.h"
#include "therm_channel.h"
#include "therm_sysfs.h"
#include "contexthub_pm.h"
#include "contexthub_recovery.h"
#include "securec.h"

ssize_t attr_thermometer_calibrate_write(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct therm_device_info *dev_info = NULL;
	RET_TYPE rtype = RET_INIT;
	unsigned long val;
	int calibrate_index;
	read_info_t pkg_mcu = { 0 };

	if (strlen(sensor_chip_info[THERMOMETER]) == 0) {
		hwlog_err("%s: no thermometer sensor\n", __func__);
		return -EINVAL;
	}
	dev_info = therm_get_device_info(TAG_THERMOMETER);
	if (dev_info == NULL)
		return -1;
	if (kstrtoul(buf, TO_DECIMALISM, &val))
		return -EINVAL;

	calibrate_index = val;
	pkg_mcu = send_calibrate_cmd(TAG_THERMOMETER, val, &rtype);
	dev_info->therm_calibration_res = rtype;
	if (dev_info->therm_calibration_res == COMMU_FAIL ||
		pkg_mcu.errno != 0) {
		dev_info->therm_calibration_res = 0;
		hwlog_err("%s: calibrate fail errno = %d\n", __func__,
			pkg_mcu.errno);
		return count;
	} else if (pkg_mcu.errno == 0) {
		dev_info->therm_calibration_res = SUC;
		hwlog_info("%s: commu succ\n", __func__);
	}

	return count;
}

ssize_t attr_get_therm_sensor_id(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	char *id_string = NULL;

	id_string = therm_get_sensors_id_string();
	if (id_string == NULL)
		return -1;

	hwlog_info("%s is %s\n", __func__, id_string);
	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%s\n",
		id_string);
}

ssize_t attr_get_therm_calibrate_index(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	char *index_string = NULL;

	index_string = therm_get_calibrate_index_string();
	if (index_string == NULL)
		return -1;

	hwlog_info("%s is %s\n", __func__, index_string);
	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%s\n",
		index_string);
}

ssize_t sensors_calibrate_show_therm(int tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct therm_device_info *dev_info = NULL;

	dev_info = therm_get_device_info(tag);
	if (dev_info == NULL)
		return -1;
	if (tag == TAG_THERMOMETER)
		return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n",
			(int32_t)(dev_info->therm_calibration_res != SUC));

	return -1;
}

