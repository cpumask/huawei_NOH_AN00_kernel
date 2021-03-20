/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: cap prox channel source file
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
#include "cap_prox_channel.h"
#ifdef CONFIG_CONTEXTHUB_SHMEM
#include "shmem.h"
#endif
#include <securec.h>

struct sar_calibrate_data sar_calibrate_datas;
struct sar_calibrate_data sar1_calibrate_datas;

int send_cap_prox1_calibrate_data_to_mcu(void)
{
	if (strlen(sensor_chip_info[CAP_PROX1]) == 0) {
		hwlog_info("is not overseas phone,cap_prox sensor is not in board\n");
		return 0;
	}

	if (read_calibrate_data_from_nv(CAP_PROX1_CALIDATA_NV_NUM,
		CAP_PROX1_CALIDATA_NV_SIZE, CAP_PROX1_NV_NAME))
		return -1;

	/* send to mcu */
	if (memcpy_s(&sar1_calibrate_datas,
		sizeof(sar1_calibrate_datas),
		user_info.nv_data, sizeof(sar1_calibrate_datas)) != EOK)
		return -1;
	if (send_calibrate_data_to_mcu(TAG_CAP_PROX1, SUB_CMD_SET_OFFSET_REQ,
		(const void *)&sar1_calibrate_datas,
		sizeof(sar1_calibrate_datas), false))
		return -1;
	return 0;
}

int send_cap_prox_calibrate_data_to_mcu(void)
{
	if (strlen(sensor_chip_info[CAP_PROX]) == 0) {
		hwlog_info("is not overseas phone,cap_prox sensor is not in board\n");
		return 0;
	}

	if (read_calibrate_data_from_nv(CAP_PROX_CALIDATA_NV_NUM,
		CAP_PROX_CALIDATA_NV_SIZE, "Csensor"))
		return -1;

	if (memcpy_s(&sar_calibrate_datas,
		sizeof(sar_calibrate_datas),
		user_info.nv_data, sizeof(sar_calibrate_datas)) != EOK)
		return -1;

	hwlog_info("cap_prox :offset1=%d offset2=%d diff1=%d diff2=%d len:%ld\n",
		sar_calibrate_datas.offset[CAP_PROX_PHASE0],
		sar_calibrate_datas.offset[CAP_PROX_PHASE1],
		sar_calibrate_datas.diff[CAP_PROX_PHASE0],
		sar_calibrate_datas.diff[CAP_PROX_PHASE1],
		sizeof(sar_calibrate_datas));

	if (send_calibrate_data_to_mcu(TAG_CAP_PROX, SUB_CMD_SET_OFFSET_REQ,
		(const void *)&sar_calibrate_datas,
		sizeof(sar_calibrate_datas), false))
		return -1;
	return 0;
}

void reset_cap_prox_calibrate_data(void)
{
	if (strlen(sensor_chip_info[CAP_PROX]))
		send_calibrate_data_to_mcu(TAG_CAP_PROX,
			SUB_CMD_SET_OFFSET_REQ, &sar_calibrate_datas,
			sizeof(sar_calibrate_datas), true);
}

void reset_cap_prox1_calibrate_data(void)
{
	if (strlen(sensor_chip_info[CAP_PROX1]))
		send_calibrate_data_to_mcu(TAG_CAP_PROX1,
			SUB_CMD_SET_OFFSET_REQ, &sar_calibrate_datas,
			sizeof(sar_calibrate_datas), true);
}

