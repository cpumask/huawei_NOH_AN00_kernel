/*
 * huawei_battery_temp.c
 *
 * battery temp driver
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
#include <linux/err.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/device.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/battery_voltage.h>
#include <chipset_common/hwpower/power_dts.h>
#include <huawei_platform/power/huawei_battery_temp.h>

#define HWLOG_TAG huawei_batt_temp
HWLOG_REGIST();

struct batt_temp_info {
	struct device *dev;
	char name[BATT_TEMP_NAME_MAX];
	struct batt_temp_ops *ops;
};

static struct batt_temp_info *g_temp_info;

enum batt_temp_type {
	SYSFS_BATT_TEMP_0 = 0,
	SYSFS_BATT_TEMP_1,
	SYSFS_BATT_TEMP_MIXED,
	SYSFS_BATT_TEMP_0_NOW,
	SYSFS_BATT_TEMP_1_NOW,
	SYSFS_BATT_TEMP_MIXED_NOW,
	SYSFS_BATT_TEMP_NAME,
};

#ifdef CONFIG_SYSFS
static ssize_t batt_temp_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);

static struct power_sysfs_attr_info batt_temp_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_0, batt_0),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_1, batt_1),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_MIXED, batt_mixed),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_0_NOW, batt_0_now),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_1_NOW, batt_1_now),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_MIXED_NOW, batt_mixed_now),
	power_sysfs_attr_ro(batt_temp, 0440,
		SYSFS_BATT_TEMP_NAME, batt_temp_name),
};

#define BATT_TEMP_SYSFS_ATTRS_SIZE  ARRAY_SIZE(batt_temp_sysfs_field_tbl)

static struct attribute *batt_temp_sysfs_attrs[BATT_TEMP_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group batt_temp_sysfs_attr_group = {
	.attrs = batt_temp_sysfs_attrs,
};

static void batt_temp_sysfs_create_group(struct device *dev)
{
	power_sysfs_init_attrs(batt_temp_sysfs_attrs,
		batt_temp_sysfs_field_tbl, BATT_TEMP_SYSFS_ATTRS_SIZE);
	power_sysfs_create_link_group("hw_power", "charger", "hw_batt_temp",
		dev, &batt_temp_sysfs_attr_group);
}

static void batt_temp_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "charger", "hw_batt_temp",
		dev, &batt_temp_sysfs_attr_group);
}
#else
static inline void batt_temp_sysfs_create_group(struct device *dev)
{
}

static inline void batt_temp_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static ssize_t batt_temp_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct batt_temp_info *di = g_temp_info;
	int batt_temp = 0;
	int len = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		batt_temp_sysfs_field_tbl, BATT_TEMP_SYSFS_ATTRS_SIZE);
	if (!info || !di)
		return -EINVAL;

	switch (info->name) {
	case SYSFS_BATT_TEMP_0:
	case SYSFS_BATT_TEMP_1:
	case SYSFS_BATT_TEMP_MIXED:
		huawei_battery_temp(info->name - SYSFS_BATT_TEMP_0, &batt_temp);
		len = snprintf(buf, PAGE_SIZE, "%d\n", batt_temp);
		break;
	case SYSFS_BATT_TEMP_0_NOW:
	case SYSFS_BATT_TEMP_1_NOW:
	case SYSFS_BATT_TEMP_MIXED_NOW:
		huawei_battery_temp_now(info->name - SYSFS_BATT_TEMP_0_NOW,
			&batt_temp);
		len = snprintf(buf, PAGE_SIZE, "%d\n", batt_temp);
		break;
	case SYSFS_BATT_TEMP_NAME:
		len = snprintf(buf, PAGE_SIZE, "%s\n", di->name);
		break;
	default:
		break;
	}

	return len;
}

int huawei_battery_temp(enum batt_temp_id id, int *temp)
{
	struct batt_temp_info *di = g_temp_info;

	if (!temp) {
		hwlog_err("temp is null\n");
		return -EINVAL;
	}

	if (!is_hisi_battery_exist())
		hwlog_err("battery not exist\n");

	if (!di || !di->ops || !di->ops->get_temp) {
		*temp = hisi_battery_temperature();
		hwlog_info("default temp api: temp %d\n", *temp);
		return 0;
	}

	return di->ops->get_temp(id, temp);
}

int huawei_battery_temp_now(enum batt_temp_id id, int *temp)
{
	struct batt_temp_info *di = g_temp_info;

	if (!temp) {
		hwlog_err("temp is null\n");
		return -EINVAL;
	}

	if (!is_hisi_battery_exist())
		hwlog_err("battery not exist\n");

	if (!di || !di->ops || !di->ops->get_temp_sync) {
		*temp = hisi_battery_temperature_for_charger();
		hwlog_info("default temp now api: temp %d\n", *temp);
		return 0;
	}

	return di->ops->get_temp_sync(id, temp);
}

int huawei_battery_temp_register(const char *name, struct batt_temp_ops *ops)
{
	if (!name || !ops || !ops->get_temp || !ops->get_temp_sync) {
		hwlog_err("input arg err\n");
		return -EINVAL;
	}

	if (!g_temp_info) {
		hwlog_err("temp info ptr is null\n");
		return -ENODEV;
	}

	if (strlen(name) >= BATT_TEMP_NAME_MAX) {
		hwlog_err("name is err\n");
		return -EINVAL;
	}

	strlcpy(g_temp_info->name, name, sizeof(g_temp_info->name));
	g_temp_info->ops = ops;
	return 0;
}

static int batt_temp_probe(struct platform_device *pdev)
{
	struct batt_temp_info *di = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_temp_info = di;
	di->dev = &pdev->dev;
	platform_set_drvdata(pdev, di);

	batt_temp_sysfs_create_group(di->dev);

	return 0;
}

static int batt_temp_remove(struct platform_device *pdev)
{
	struct batt_temp_info *di = platform_get_drvdata(pdev);

	if (!di)
		return -ENODEV;

	batt_temp_sysfs_remove_group(di->dev);
	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, di);
	g_temp_info = NULL;

	return 0;
}

static const struct of_device_id batt_temp_match_table[] = {
	{
		.compatible = "huawei,battery_temp",
		.data = NULL,
	},
	{},
};

static struct platform_driver batt_temp_driver = {
	.probe = batt_temp_probe,
	.remove = batt_temp_remove,
	.driver = {
		.name = "huawei,battery_temp",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(batt_temp_match_table),
	},
};

int __init batt_temp_init(void)
{
	return platform_driver_register(&batt_temp_driver);
}

void __exit batt_temp_exit(void)
{
	platform_driver_unregister(&batt_temp_driver);
}

fs_initcall(batt_temp_init);
module_exit(batt_temp_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("huawei battery temp module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
