/*
* ntc_thermal.c
*
* thermal for ntc module
*
* Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/thermal.h>
#include <securec.h>
#include <chipset_common/hwpower/power_dts.h>
#include <huawei_platform/log/hw_log.h>
#include "ext_sensorhub_api.h"

#define POWER_TZ_STR_MAX_LEN			   16

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG watch_ntc
HWLOG_REGIST();

#define NTC_SERVICE_ID	  0x01
#define NTC_COMMAND_ID	  0x82
#define NTC_TEMP_RSP_TYPE   0x02
#define NTC_BYTES_RSP_LEN   6
#define NTC_TEMP_DEFAULT	30
#define NTC_RSP_LEN		 4

struct watch_ntc_tz_sensor {
	char sensor_name[POWER_TZ_STR_MAX_LEN];
	char ops_name[POWER_TZ_STR_MAX_LEN];
	struct thermal_zone_device *tz_dev;
};

struct ntc_thermal_priv {
	struct device *dev;
	struct watch_ntc_tz_sensor sensor;
};

static struct ntc_thermal_priv *g_power_tz_info;

static int parse_ntc_temp(unsigned char *data)
{
	hwlog_err("begin parse_ntc_temp\n");
	int ntc_temp;
	int ret;
	ret = memcpy_s(&ntc_temp, NTC_RSP_LEN, data, NTC_RSP_LEN);
	if (ret) {
		hwlog_err("[%s]:cpy ntc temp failed!\n", __func__);
		return -1;
	}
	ntc_temp = ntohl(ntc_temp);
	hwlog_err("[%s]:ntc temp is %d\n", __func__, ntc_temp);
	return ntc_temp;
}

static int watch_query_ntc_info(int *raw_temp)
{
	hwlog_err("begin watch_query_ntc_info\n");
	int ret;
	int payload = 0;
	unsigned char send_cmd[3] = {0x01, 0x01, 0x01};
	struct command cmd = {
		.service_id = NTC_SERVICE_ID,
		.command_id = NTC_COMMAND_ID,
		.send_buffer = &send_cmd[0],
		.send_buffer_len = sizeof(send_cmd),
	};

	struct cmd_resp mcu_rsp;
	mcu_rsp.receive_buffer_len = NTC_BYTES_RSP_LEN;
	mcu_rsp.receive_buffer = (unsigned char *)kmalloc(mcu_rsp.receive_buffer_len, GFP_KERNEL);

	ret = send_command(CHARGE_CHANNEL, &cmd, true, &mcu_rsp);
	hwlog_err("send_command ret %d\n", ret);
	if (ret < 0) {
		hwlog_err("get ntc temp failed\n");
		goto err;
	}

	if (mcu_rsp.receive_buffer[payload++] != NTC_TEMP_RSP_TYPE) {
		hwlog_err("receive buff is %02x\n", mcu_rsp.receive_buffer[payload]);
		ret = -1;
		goto err;
	}

	int payload_len = mcu_rsp.receive_buffer[payload++];
	hwlog_err("payload len is %d\n", payload_len);
	if (payload_len != 4) {
		ret = -1;
		hwlog_err("%s get ntc temp payloadlen:%d\n", __func__, payload_len);
		goto err;
	}

	*raw_temp = parse_ntc_temp(&mcu_rsp.receive_buffer[payload]);

	if (raw_temp < 0) {
		ret = -1;
		hwlog_err("parse_ntc_temp failed\n");
		goto err;
	}

	return 0;

err:
	*raw_temp = NTC_TEMP_DEFAULT;
	return ret;
}

static int ntc_thermal_get_temp(
	struct thermal_zone_device *thermal, int *temp)
{
	hwlog_err("begin get ntc_temp\n");
	int raw_temp;

	if (thermal == NULL || temp == NULL)
		return -EINVAL;
	watch_query_ntc_info(&raw_temp);
	*temp = raw_temp;
	return 0;
}


static struct thermal_zone_device_ops ntc_thermal_ops = {
	.get_temp = ntc_thermal_get_temp,
};

static int power_tz_parse_dts(struct device_node *dev_node,
							  struct ntc_thermal_priv *info)
{
	const char *sensor_name = NULL;
	const char *ops_name = NULL;

	if (power_dts_read_string(power_dts_tag(HWLOG_TAG), dev_node,
							  "sensor_name", &sensor_name))
		return -EINVAL;

	if (power_dts_read_string(power_dts_tag(HWLOG_TAG), dev_node,
							  "ops_name", &ops_name))
		return -EINVAL;

	strncpy_s(info->sensor.sensor_name, sizeof(info->sensor.sensor_name),
			sensor_name, (POWER_TZ_STR_MAX_LEN - 1));
	strncpy_s(info->sensor.ops_name, sizeof(info->sensor.ops_name),
			ops_name, (POWER_TZ_STR_MAX_LEN - 1));

	hwlog_info("para:sensor:%s,ops:%s\n",
			   info->sensor.sensor_name,
			   info->sensor.ops_name);

	return 0;
}

static int ntc_thermal_probe(struct platform_device *pdev)
{
	hwlog_err("begin ntc_thermal_probe\n");
	struct ntc_thermal_priv *priv = NULL;
	int ret;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	priv = devm_kzalloc(&pdev->dev, sizeof(struct ntc_thermal_priv) +
						sizeof(struct watch_ntc_tz_sensor), GFP_KERNEL);
	if (!priv) {
		hwlog_err("priv is null\n");
		return -ENOMEM;
	}

	g_power_tz_info = priv;
	priv->dev = &pdev->dev;

	ret = power_tz_parse_dts(priv->dev->of_node, priv);
	if (ret < 0) {
		hwlog_err("power_tz_parse_dts failed\n");
		goto fail_parse_dts;
	}

	priv->sensor.tz_dev = thermal_zone_device_register(
							  priv->sensor.sensor_name,
							  0, 0, priv, &ntc_thermal_ops, NULL, 0, 0);
	if (IS_ERR(priv->sensor.tz_dev)) {
		hwlog_err("ntc thermal zone register fail\n");
		ret = -ENODEV;
		goto fail_register_tz;
	}

	platform_set_drvdata(pdev, priv);

	return 0;

fail_register_tz:
	thermal_zone_device_unregister(priv->sensor.tz_dev);

fail_parse_dts:
	of_node_put(priv->dev->of_node);

fail_free_mem:
	devm_kfree(&pdev->dev, priv);
	g_power_tz_info = NULL;

	return ret;
}

static int ntc_thermal_remove(struct platform_device *pdev)
{
	struct ntc_thermal_priv *priv = platform_get_drvdata(pdev);

	if (!priv)
		return -ENODEV;

	thermal_zone_device_unregister(priv->sensor.tz_dev);

	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, priv);
	g_power_tz_info = NULL;

	return 0;
}

static const struct of_device_id ntc_thermal_match[] = {
	{
		.compatible = "hisilicon,ntc_thermal",
		.data = NULL,
	},
	{},
};

static struct platform_driver ntc_thermal_driver = {
	.probe = ntc_thermal_probe,
	.remove = ntc_thermal_remove,
	.driver =
	{
		.name = "ntc thermal",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(ntc_thermal_match),
	},
};

static int __init hisi_ntc_thermal_init(void)
{
	return platform_driver_register(&ntc_thermal_driver);
}

static void __exit hisi_ntc_thermal_exit(void)
{
	platform_driver_unregister(&ntc_thermal_driver);
}

module_init(hisi_ntc_thermal_init);
module_exit(hisi_ntc_thermal_exit);

MODULE_DESCRIPTION("ntc Thermal driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_LICENSE("GPL v2");