/*
 * power_temp.c
 *
 * temperature interface for power module
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

#include <chipset_common/hwpower/power_temp.h>
#include <chipset_common/hwpower/power_sysfs.h>
#include <chipset_common/hwpower/power_algorithm.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG power_temp
HWLOG_REGIST();

static struct power_temp_dev *g_power_temp_dev;

static int power_temp_get_tz_temp(const char *sensor_name, int *temp)
{
	struct thermal_zone_device *tz = NULL;
	int ret;

	if (!sensor_name || !temp) {
		hwlog_err("sensor_name or temp is null\n");
		return -1;
	}

	tz = thermal_zone_get_zone_by_name(sensor_name);
	if (IS_ERR(tz)) {
		hwlog_err("get %s tz fail\n", sensor_name);
		return -1;
	}

	ret = thermal_zone_get_temp(tz, temp);
	if (ret) {
		hwlog_err("get %s tz temp fail\n", sensor_name);
		return -1;
	}

	return 0;
}

int power_temp_get_raw_value(const char *sensor_name)
{
	int temp = POWER_TEMP_INVALID_TEMP;

	if (power_temp_get_tz_temp(sensor_name, &temp))
		return POWER_TEMP_INVALID_TEMP / POWER_TEMP_UNIT;

	return temp / POWER_TEMP_UNIT;
}

int power_temp_get_average_value(const char *sensor_name)
{
	int samples[POWER_TEMP_SAMPLES] = { 0 };
	int retrys = POWER_TEMP_RETRYS;
	int i, max_temp, min_temp;
	int invalid_flag;

	while (retrys--) {
		invalid_flag = POWER_TEMP_VALID;

		for (i = 0; i < POWER_TEMP_SAMPLES; ++i) {
			if (power_temp_get_tz_temp(sensor_name, &samples[i])) {
				invalid_flag = POWER_TEMP_INVALID;
				break;
			}
			hwlog_info("sensor:%s, samples[%d]:%d\n",
				sensor_name, i, samples[i]);
		}

		if (invalid_flag == POWER_TEMP_INVALID)
			continue;

		/* check sample value is valid */
		max_temp = power_get_max_value(samples, POWER_TEMP_SAMPLES);
		min_temp = power_get_min_value(samples, POWER_TEMP_SAMPLES);
		if (max_temp - min_temp > POWER_TEMP_DIFF_THLD) {
			hwlog_err("invalid temp: max=%d min=%d\n",
				max_temp, min_temp);
			invalid_flag = POWER_TEMP_INVALID;
		}

		if (invalid_flag == POWER_TEMP_VALID)
			break;
	}

	if (invalid_flag == POWER_TEMP_INVALID) {
		hwlog_err("use default temp %d\n", POWER_TEMP_INVALID_TEMP);
		return POWER_TEMP_INVALID_TEMP;
	}

	return power_get_average_value(samples, POWER_TEMP_SAMPLES);
}

#ifdef CONFIG_SYSFS
static ssize_t power_temp_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);

static struct power_sysfs_attr_info power_temp_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(power_temp, 0440, POWER_TEMP_SYSFS_USB_PORT, usb_port),
	power_sysfs_attr_ro(power_temp, 0440, POWER_TEMP_SYSFS_SERIAL_BAT_0, serial_bat_0),
	power_sysfs_attr_ro(power_temp, 0440, POWER_TEMP_SYSFS_SERIAL_BAT_1, serial_bat_1),
	power_sysfs_attr_ro(power_temp, 0440, POWER_TEMP_SYSFS_BTB_NTC_AUX, btb_ntc_aux),
};

#define POWER_TEMP_SYSFS_ATTRS_SIZE  ARRAY_SIZE(power_temp_sysfs_field_tbl)

static struct attribute *power_temp_sysfs_attrs[POWER_TEMP_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group power_temp_sysfs_attr_group = {
	.attrs = power_temp_sysfs_attrs,
};

static ssize_t power_temp_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;

	info = power_sysfs_lookup_attr(attr->attr.name,
		power_temp_sysfs_field_tbl, POWER_TEMP_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case POWER_TEMP_SYSFS_USB_PORT:
		return scnprintf(buf, POWER_TEMP_RD_BUF_SIZE, "%d\n",
			power_temp_get_raw_value(POWER_TEMP_USB_PORT));
	case POWER_TEMP_SYSFS_SERIAL_BAT_0:
		return scnprintf(buf, POWER_TEMP_RD_BUF_SIZE, "%d\n",
			power_temp_get_raw_value(POWER_TEMP_SERIAL_BAT_0));
	case POWER_TEMP_SYSFS_SERIAL_BAT_1:
		return scnprintf(buf, POWER_TEMP_RD_BUF_SIZE, "%d\n",
			power_temp_get_raw_value(POWER_TEMP_SERIAL_BAT_1));
	case POWER_TEMP_SYSFS_BTB_NTC_AUX:
		return scnprintf(buf, POWER_TEMP_RD_BUF_SIZE, "%d\n",
			power_temp_get_raw_value(POWER_TEMP_BTB_NTC_AUX));
	default:
		return POWER_TEMP_INVALID_TEMP;
	}
}

static struct device *power_temp_sysfs_create_group(void)
{
	power_sysfs_init_attrs(power_temp_sysfs_attrs,
		power_temp_sysfs_field_tbl, POWER_TEMP_SYSFS_ATTRS_SIZE);
	return power_sysfs_create_group("hw_power", "power_temp",
		&power_temp_sysfs_attr_group);
}

static void power_temp_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &power_temp_sysfs_attr_group);
}
#else
static inline struct device *power_temp_sysfs_create_group(void)
{
	return NULL;
}

static inline void power_temp_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int __init power_temp_init(void)
{
	struct power_temp_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_power_temp_dev = l_dev;
	l_dev->dev = power_temp_sysfs_create_group();

	return 0;
}

static void __exit power_temp_exit(void)
{
	if (!g_power_temp_dev)
		return;

	power_temp_sysfs_remove_group(g_power_temp_dev->dev);
	kfree(g_power_temp_dev);
	g_power_temp_dev = NULL;
}

subsys_initcall_sync(power_temp_init);
module_exit(power_temp_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("temp for power module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
