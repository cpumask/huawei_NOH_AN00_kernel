/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mag route source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <asm/memory.h>
#include <linux/time64.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <asm/cacheflush.h>
#include <linux/semaphore.h>
#include <linux/freezer.h>
#include <linux/of.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "sensor_config.h"
#include "sensor_feima.h"
#include "sensor_sysfs.h"
#include "sensor_detect.h"
#include "mag_channel.h"
#include "mag_route.h"
#include <securec.h>

ssize_t show_mag_calibrate_method(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct compass_platform_data *pf_data = NULL;

	pf_data = mag_get_platform_data(TAG_MAG);
	if (pf_data == NULL)
		return -1;

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%d\n",
		pf_data->calibrate_method);
}

ssize_t show_mag_sensorlist_info(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	if (memcpy_s(buf, MAX_STR_SIZE, &sensorlist_info[MAG],
		sizeof(struct sensorlist_info)) != EOK)
		return -1;
	return sizeof(struct sensorlist_info);
}

ssize_t calibrate_threshold_from_mag_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mag_device_info *dev_info = NULL;

	dev_info = mag_get_device_info(TAG_MAG);
	if (dev_info == NULL)
		return -1;
	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n",
		dev_info->mag_threshold_for_als_calibrate);
}

