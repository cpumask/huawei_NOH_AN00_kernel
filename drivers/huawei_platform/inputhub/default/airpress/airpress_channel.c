/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: airpress channel source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/rtc.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/hisi/usb/hisi_usb.h>
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#ifdef CONFIG_HW_TOUCH_KEY
#include <huawei_platform/sensor/huawei_key.h>
#endif
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "contexthub_recovery.h"
#include "protocol.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_sysfs.h"
#include "airpress_channel.h"
#ifdef CONFIG_CONTEXTHUB_SHMEM
#include "shmem.h"
#endif
#include <securec.h>

int send_airpress_calibrate_data_to_mcu(void)
{
	struct airpress_platform_data *pf_data = NULL;

	pf_data = airpress_get_platform_data(TAG_PRESSURE);
	if (pf_data == NULL)
		return -1;

	if (read_calibrate_data_from_nv(AIRPRESS_CALIDATA_NV_NUM,
		AIRPRESS_CALIDATA_NV_SIZE, "AIRDATA"))
		return -1;

	/* send to mcu */
	if (memcpy_s(&pf_data->offset, sizeof(pf_data->offset),
		user_info.nv_data, sizeof(pf_data->offset)) != EOK)
		return -1;
	hwlog_info("airpress offset data=%d\n", pf_data->offset);

	if (send_calibrate_data_to_mcu(TAG_PRESSURE, SUB_CMD_SET_OFFSET_REQ,
		&pf_data->offset, sizeof(pf_data->offset), false))
		return -1;

	return 0;
}

void reset_airpress_calibrate_data(void)
{
	if (strlen(sensor_chip_info[AIRPRESS])) {
		struct airpress_platform_data *pf_data = NULL;

		pf_data = airpress_get_platform_data(TAG_PRESSURE);
		if (pf_data == NULL)
			return;

		hwlog_info("airpress offset reset data=%d\n", pf_data->offset);
		send_calibrate_data_to_mcu(TAG_PRESSURE, SUB_CMD_SET_OFFSET_REQ,
			&pf_data->offset, sizeof(pf_data->offset), true);
	}
}

