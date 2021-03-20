/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: therm route source file
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
#include "contexthub_debug.h"
#include "sensor_config.h"
#include "sensor_feima.h"
#include "sensor_sysfs.h"
#include "sensor_detect.h"
#include "therm_channel.h"
#include "therm_route.h"
#include <securec.h>

ssize_t therm_store_set_mode(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	read_info_t pkg_mcu = { 0 };
	write_info_t pkg_ap = { 0 };
	int ret;
	int arg;
	int argc = 0;
	therm_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;

	if (strlen(sensor_chip_info[THERMOMETER]) == 0) {
		hwlog_err("%s: no thermometer sensor\n", __func__);
		return -EINVAL;
	}

	/* parse cmd buffer */
	for (; (buf = get_str_begin(buf)) != NULL; buf = get_str_end(buf)) {
		if (get_arg(buf, &arg)) {
			if (argc < THERM_PARA_LEN)
				cpkt.para[argc++]= arg;
			else
				hwlog_err("too many args, %d will be ignored\n", arg);
		}
	}

	pkg_ap.tag = TAG_THERMOMETER;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	cpkt.sub_cmd = SUB_CMD_THERM_START_MEASURE;
	pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
	pkg_ap.wr_buf = &hd[1];
	hwlog_info("%s therm start measure mode %d tof %d emmisivity %d sent\n",
			__func__, cpkt.para[0], cpkt.para[1], cpkt.para[2]);
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret != 0) {
		hwlog_err("%s therm start measure mode failed, ret = %d\n",
			__func__, ret);
		return -1;
	}
	if (pkg_mcu.errno != 0) {
		hwlog_err("%s therm start measure mode, errno = %d\n",
			__func__, pkg_mcu.errno);
		return -1;
	}

	hwlog_info("%s therm start measure mode success\n", __func__);
	return size;
}

