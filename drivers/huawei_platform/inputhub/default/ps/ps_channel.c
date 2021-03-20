/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ps channel source file
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
#include "ps_channel.h"
#ifdef CONFIG_CONTEXTHUB_SHMEM
#include "shmem.h"
#endif
#include <securec.h>

int send_ps_calibrate_data_to_mcu(void)
{
	struct ps_device_info *dev_info = NULL;

	dev_info = ps_get_device_info(TAG_PS);
	if (dev_info == NULL)
		return -1;
	if (read_calibrate_data_from_nv(PS_CALIDATA_NV_NUM,
		PS_CALIDATA_NV_SIZE, "PSENSOR"))
		return -1;

	dev_info->ps_first_start_flag = 1;

	if (memcpy_s(ps_sensor_offset, sizeof(ps_sensor_offset),
		user_info.nv_data, sizeof(ps_sensor_offset)) != EOK)
		return -1;
	hwlog_info("nve_direct_access read ps_offset %d,%d,%d, %d,%d,%d\n",
		ps_sensor_offset[0], ps_sensor_offset[1], ps_sensor_offset[2],
		ps_sensor_offset[3], ps_sensor_offset[4], ps_sensor_offset[5]);
	if (memcpy_s(&ps_sensor_calibrate_data,
		sizeof(ps_sensor_calibrate_data),
		ps_sensor_offset,
		(sizeof(ps_sensor_calibrate_data) < PS_CALIDATA_NV_SIZE) ?
		sizeof(ps_sensor_calibrate_data) : PS_CALIDATA_NV_SIZE) != EOK)
		return -1;

	if (send_calibrate_data_to_mcu(TAG_PS, SUB_CMD_SET_OFFSET_REQ,
		ps_sensor_offset, PS_CALIDATA_NV_SIZE, false))
		return -1;

	return 0;
}

void reset_ps_calibrate_data(void)
{
	struct ps_device_info *dev_info = NULL;

	dev_info = ps_get_device_info(TAG_PS);
	if (dev_info == NULL)
		return;
	if (dev_info->chip_type != PS_CHIP_NONE ||
		dev_info->ps_cali_supported == 1)
		send_calibrate_data_to_mcu(TAG_PS, SUB_CMD_SET_OFFSET_REQ,
			ps_sensor_calibrate_data, PS_CALIDATA_NV_SIZE, true);
}

