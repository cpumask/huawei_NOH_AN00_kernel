/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mag sysfs source file
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
#include "mag_channel.h"
#include "mag_sysfs.h"
#include "contexthub_pm.h"
#include "contexthub_recovery.h"
#include "securec.h"

ssize_t attr_get_mag_sensor_id(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	char *id_string = NULL;

	id_string = mag_get_sensors_id_string();
	if (id_string == NULL)
		return -1;

	hwlog_info("%s is %s\n", __func__, id_string);
	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%s\n",
		id_string);
}

