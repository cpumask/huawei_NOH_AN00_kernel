/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: therm detect source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include "sensor_feima.h"
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "protocol.h"
#include <sensor_sysfs.h>
#include <sensor_config.h>
#include <sensor_detect.h>
#include "therm_channel.h"
#include "therm_detect.h"
#include "contexthub_debug.h"
#include "vibrator_channel.h"
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/switch.h>
#include <linux/hisi/hw_cmdline_parse.h>
#include "huawei_thp_attr.h"
#include <huawei_platform/inputhub/sensorhub.h>
#ifdef CONFIG_HUAWEI_SHB_THP
#include <huawei_platform/inputhub/thphub.h>
#endif
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <huawei_platform/devdetect/hw_dev_dec.h>
#endif
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#ifdef CONFIG_CONTEXTHUB_SHMEM
#include "shmem.h"
#endif
#include "securec.h"
#ifdef DSS_ALSC_NOISE
#include "display_effect_alsc_interface.h"
#endif

#define THERM_DEVICE_ID_0 0
#define THERM_SENSOR_DEFAULT_ID "65574"
#define THERM_DEFAULT_CALIBRATE_INDEX "1 2"

static char *therm_sensor_id = THERM_SENSOR_DEFAULT_ID;
static char *therm_calibrate_index = THERM_DEFAULT_CALIBRATE_INDEX;

static struct therm_platform_data therm_data[THERM_DEV_COUNT_MAX] = {
	{
	.cfg = {
		.bus_type = TAG_I2C,
		.bus_num = 0,
		.disable_sample_thread = 0,
		{ .i2c_address = 0x3A },
	},
	.blackbody_count = 2,
	}
};
static struct therm_device_info g_therm_dev_info[THERM_DEV_COUNT_MAX];

struct therm_platform_data *therm_get_platform_data(int32_t tag)
{
	if (tag == TAG_THERMOMETER)
		return (&(therm_data[THERM_DEVICE_ID_0]));

	hwlog_info("%s error, please check tag %d\n", __func__, tag);
	return NULL;
}

struct therm_device_info *therm_get_device_info(int32_t tag)
{
	if (tag == TAG_THERMOMETER)
		return (&(g_therm_dev_info[THERM_DEVICE_ID_0]));

	hwlog_info("%s error, please check tag %d\n", __func__, tag);
	return NULL;
}

char *therm_get_sensors_id_string(void)
{
	return therm_sensor_id;
}

void therm_get_sensors_id_from_dts(struct device_node *dn)
{
	if (dn == NULL)
		return;
	if (of_property_read_string(dn,
		"therm_sensor_id", (const char **)&therm_sensor_id)) {
		therm_sensor_id = THERM_SENSOR_DEFAULT_ID;
		hwlog_err("therm_sensor_id err\n");
	}
	hwlog_info("therm_sensor_id:%s\n", therm_sensor_id);
}

char *therm_get_calibrate_index_string(void)
{
	return therm_calibrate_index;
}

static void read_thermometer_distance_algo_para(struct device_node *dn)
{
	struct therm_platform_data *pf_data = &therm_data[THERM_DEVICE_ID_0];
	int temp = 0;

	if (of_property_read_u32(dn, "blackbody_count", &temp))
		hwlog_err("blackbody_count err\n");
	else
		pf_data->blackbody_count = (uint8_t)temp;
	hwlog_info("therm blackbody_count:%d\n", temp);

	if (of_property_read_u32(dn, "ar_mode", &temp))
		hwlog_err("ar_mode err\n");
	else
		pf_data->ar_mode = (uint8_t)temp;
	hwlog_info("therm ar_mode:%d\n", temp);

	if (of_property_read_u32(dn, "k1", &temp))
		hwlog_err("therm k1 err\n");
	else
		pf_data->k1 = temp;

	if (of_property_read_u32(dn, "k2", &temp))
		hwlog_err("therm k2 err\n");
	else
		pf_data->k2 = temp;

	if (of_property_read_u32(dn, "c1", &temp))
		hwlog_err("therm c1 err\n");
	else
		pf_data->c1 = temp;

	if (of_property_read_u32(dn, "c2", &temp))
		hwlog_err("therm c2 err\n");
	else
		pf_data->c2 = temp;

	if (of_property_read_u32(dn, "c3", &temp))
		hwlog_err("therm c3 err\n");
	else
		pf_data->c3 = temp;

	if (of_property_read_u32(dn, "c4", &temp))
		hwlog_err("therm c3 err\n");
	else
		pf_data->c4 = temp;

	if (of_property_read_u32(dn, "c5", &temp))
		hwlog_err("therm c3 err\n");
	else
		pf_data->c5 = temp;
}

void read_thermometer_data_from_dts(struct device_node *dn)
{
	int temp = 0;

	read_chip_info(dn, THERMOMETER);

	if (of_property_read_string(dn, "calibrate_index", (const char **)&therm_calibrate_index)) {
		therm_calibrate_index = THERM_DEFAULT_CALIBRATE_INDEX;
		hwlog_err("therm_calibrate_index err\n");
	}
	hwlog_info("therm_calibrate_index:%s\n", therm_calibrate_index);

	read_thermometer_distance_algo_para(dn);

	if (of_property_read_u32(dn, "file_id", &temp))
		hwlog_err("%s: read thermometer file_id fail\n", __func__);
	else
		read_dyn_file_list((uint16_t)temp);

	if (of_property_read_u32(dn, "sensor_list_info_id", &temp))
		hwlog_err("%s:read thermometer sensor_list_info_id fail\n", __func__);
	else
		sensorlist[++sensorlist[0]] = (uint16_t)temp;
}

int thermometer_data_from_mcu(const pkt_header_t *head)
{
	struct therm_device_info *dev_info =
		&g_therm_dev_info[THERM_DEVICE_ID_0];
	int value;
	char buf[SELFTEST_RESULT_MAXLEN];

	switch (((pkt_thermometer_data_req_t *)head)->subcmd) {
	case SUB_CMD_SELFCALI_REQ:
		value = ((pkt_thermometer_data_req_t *)head)->return_data;
		if (value == 0)
			dev_info->therm_calibration_res = SUC;
		else
			dev_info->therm_calibration_res = EXEC_FAIL;
		hwlog_info("therm calibrate cmd %d, value %d, result %d\n",
			((pkt_thermometer_data_req_t *)head)->subcmd,
			value, dev_info->therm_calibration_res);
		calibrate_processing = 0;
		break;
	case SUB_CMD_SELFTEST_REQ:
		value = ((pkt_thermometer_data_req_t *)head)->return_data;
		hwlog_info("therm selftest cmd %d, result %d\n",
			((pkt_thermometer_data_req_t *)head)->subcmd, value);
		if (value == 0) {
			if (memcpy_s(sensor_status.selftest_result[TAG_THERMOMETER],
				SELFTEST_RESULT_MAXLEN, "0", 2) != EOK)
				hwlog_err("%s: memcpy_s error\n", __func__);
		} else {
			snprintf_s(buf, SELFTEST_RESULT_MAXLEN, SELFTEST_RESULT_MAXLEN - 1, "%d\n", value);
			if (memcpy_s(sensor_status.selftest_result[TAG_THERMOMETER],
				SELFTEST_RESULT_MAXLEN, buf, SELFTEST_RESULT_MAXLEN) != EOK)
				hwlog_err("%s: memcpy_s error\n", __func__);
		}
		break;
	default:
		hwlog_err("uncorrect subcmd 0x%x\n", ((pkt_thermometer_data_req_t *)head)->subcmd);
		break;
	}
	return 0;
}

void therm_detect_init(struct sensor_detect_manager *sm, uint32_t len)
{
	struct sensor_detect_manager *p = NULL;

	if (len <= THERMOMETER) {
		hwlog_err("%s:len=%d\n", __func__, len);
		return;
	}
	g_therm_dev_info[THERM_DEVICE_ID_0].obj_tag = TAG_THERMOMETER;
	g_therm_dev_info[THERM_DEVICE_ID_0].detect_list_id = THERMOMETER;
	g_therm_dev_info[THERM_DEVICE_ID_0].therm_dev_index = 0;
	p = sm + THERMOMETER;
	p->spara = (const void *)&therm_data[THERM_DEVICE_ID_0];
	p->cfg_data_length = sizeof(therm_data[THERM_DEVICE_ID_0]);
}

