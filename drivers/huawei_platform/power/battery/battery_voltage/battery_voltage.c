/*
 * battery_voltage.c
 *
 * battery voltage interface for power module
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include <linux/hisi/hisi_adc.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/battery_voltage.h>

#define HWLOG_TAG hw_batt_vol
HWLOG_REGIST();

static struct hw_batt_vol_info *g_hw_batt_di;

int hw_battery_get_series_num(void)
{
	if (!g_hw_batt_di)
		return HW_ONE_BAT;

	return g_hw_batt_di->batt_series_num;
}

int hw_battery_voltage(enum hw_batt_id batt_id)
{
	int i;
	int max = -1;
	int min = MAX_VOL_MV;
	int vol = 0;

	if (!g_hw_batt_di)
		return hisi_battery_voltage();

	if (g_hw_batt_di->total_vol == HW_BATT_VOL_SINGLE_BATTERY) {
		return g_hw_batt_di->vol_buff[0].get_batt_vol();
	} else if (g_hw_batt_di->total_vol == 0) {
		hwlog_err("use default hisi_battery_voltage\n");
		return hisi_battery_voltage();
	}

	switch (batt_id) {
	case BAT_ID_0:
	case BAT_ID_1:
	case BAT_ID_ALL:
		for (i = 0; i < g_hw_batt_di->total_vol; i++) {
			if (g_hw_batt_di->vol_buff[i].batt_id != batt_id)
				continue;

			if (g_hw_batt_di->vol_buff[i].get_batt_vol) {
				vol = g_hw_batt_di->vol_buff[i].get_batt_vol();
			} else {
				hwlog_err("vol_buff[%d].get_batt_vol is null\n",
					batt_id);
				return -1;
			}

			hwlog_info("get hw_batt_vol[%d]=%dmv\n", batt_id, vol);
			return vol;
		}
		break;
	case BAT_ID_MAX:
		for (i = 0; i < g_hw_batt_di->total_vol; i++) {
			if (g_hw_batt_di->vol_buff[i].batt_id == BAT_ID_ALL)
				continue;

			if (g_hw_batt_di->vol_buff[i].get_batt_vol) {
				vol = g_hw_batt_di->vol_buff[i].get_batt_vol();
			} else {
				hwlog_err("vol_buff[%d].get_batt_vol is null\n",
					batt_id);
				return -1;
			}

			if (vol < 0) {
				hwlog_err("get_batt_vol fail, batt_id=%d\n",
					batt_id);
				return -1;
			}

			if (vol > max)
				max = vol;
		}

		hwlog_info("get hw_batt_vol_max[%d]=%dmv\n", batt_id, max);
		return max;
	case BAT_ID_MIN:
		for (i = 0; i < g_hw_batt_di->total_vol; i++) {
			if (g_hw_batt_di->vol_buff[i].batt_id == BAT_ID_ALL)
				continue;

			if (g_hw_batt_di->vol_buff[i].get_batt_vol) {
				vol = g_hw_batt_di->vol_buff[i].get_batt_vol();
			} else {
				hwlog_err("vol_buff[%d].get_batt_vol is null\n",
					batt_id);
				return -1;
			}

			if (vol < 0) {
				hwlog_err("get_batt_vol fail, batt_id=%d\n",
					batt_id);
				return -1;
			}

			if (vol < min)
				min = vol;
		}

		if (min < MAX_VOL_MV) {
			hwlog_info("get hw_batt_vol_min[%d]=%dmv\n",
				batt_id, min);
			return min;
		} else {
			return -1;
		}
		break;
	default:
		hwlog_err("invalid batt_id:%d\n", batt_id);
		break;
	}

	return -1;
}

int hw_battery_voltage_ops_register(struct hw_batt_vol_ops *ops,
	char *ops_name)
{
	int i;

	if (!g_hw_batt_di || !ops) {
		hwlog_err("g_hw_batt_di or ops is null\n");
		return -EPERM;
	}

	for (i = 0; i < g_hw_batt_di->total_vol; i++) {
		if (!strncmp(ops_name, g_hw_batt_di->vol_buff[i].ops_name,
			strlen(g_hw_batt_di->vol_buff[i].ops_name))) {
			g_hw_batt_di->vol_buff[i].get_batt_vol =
				ops->get_batt_vol;
			break;
		}
	}

	if (i >= g_hw_batt_di->total_vol) {
		hwlog_err("%s ops register fail\n", ops_name);
		return -EPERM;
	}

	hwlog_info("%s ops register ok\n", ops_name);
	return 0;
}

#ifdef CONFIG_SYSFS
static ssize_t hw_batt_vol_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);

static struct power_sysfs_attr_info hw_batt_vol_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(hw_batt_vol, 0440, HW_BATT_VOL_SYSFS_BAT_ID_0, bat_id_0),
	power_sysfs_attr_ro(hw_batt_vol, 0440, HW_BATT_VOL_SYSFS_BAT_ID_1, bat_id_1),
	power_sysfs_attr_ro(hw_batt_vol, 0440, HW_BATT_VOL_SYSFS_BAT_ID_ALL, bat_id_all),
	power_sysfs_attr_ro(hw_batt_vol, 0440, HW_BATT_VOL_SYSFS_BAT_ID_MAX, bat_id_max),
	power_sysfs_attr_ro(hw_batt_vol, 0440, HW_BATT_VOL_SYSFS_BAT_ID_MIN, bat_id_min),
};

#define HW_BATT_VOL_SYSFS_ATTRS_SIZE  ARRAY_SIZE(hw_batt_vol_sysfs_field_tbl)

static struct attribute *hw_batt_vol_sysfs_attrs[HW_BATT_VOL_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group hw_batt_vol_sysfs_attr_group = {
	.attrs = hw_batt_vol_sysfs_attrs,
};

static ssize_t hw_batt_vol_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	int len = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		hw_batt_vol_sysfs_field_tbl, HW_BATT_VOL_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case HW_BATT_VOL_SYSFS_BAT_ID_0:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			hw_battery_voltage(BAT_ID_0));
		break;
	case HW_BATT_VOL_SYSFS_BAT_ID_1:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			hw_battery_voltage(BAT_ID_1));
		break;
	case HW_BATT_VOL_SYSFS_BAT_ID_ALL:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			hw_battery_voltage(BAT_ID_ALL));
		break;
	case HW_BATT_VOL_SYSFS_BAT_ID_MAX:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			hw_battery_voltage(BAT_ID_MAX));
		break;
	case HW_BATT_VOL_SYSFS_BAT_ID_MIN:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			hw_battery_voltage(BAT_ID_MIN));
		break;
	default:
		break;
	}

	return len;
}

static void hw_batt_vol_sysfs_create_group(struct device *dev)
{
	power_sysfs_init_attrs(hw_batt_vol_sysfs_attrs,
		hw_batt_vol_sysfs_field_tbl, HW_BATT_VOL_SYSFS_ATTRS_SIZE);
	power_sysfs_create_link_group("hw_power", "charger", "hw_batt_vol",
		dev, &hw_batt_vol_sysfs_attr_group);
}

static void hw_batt_vol_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "charger", "hw_batt_vol",
		dev, &hw_batt_vol_sysfs_attr_group);
}
#else
static inline void hw_batt_vol_sysfs_create_group(struct device *dev)
{
}

static inline void hw_batt_vol_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int hw_batt_vol_parse_dts(struct device_node *np,
	struct hw_batt_vol_info *di)
{
	int i = 0;
	const char *ops_name = NULL;
	struct device_node *child_node = NULL;

	di->total_vol = of_get_child_count(np);
	if (di->total_vol <= 0) {
		hwlog_err("total_vol dts read failed\n");
		return -EINVAL;
	}

	for_each_child_of_node(np, child_node) {
		if (power_dts_read_u32(power_dts_tag(HWLOG_TAG),
			child_node, "batt_id", &di->vol_buff[i].batt_id, 0))
			return -EINVAL;

		if (power_dts_read_string(power_dts_tag(HWLOG_TAG),
			child_node, "ops_name", &ops_name))
			return -EINVAL;

		if (!strncmp(ops_name, HW_BATT_HISI_COUL,
			strlen(HW_BATT_HISI_COUL)))
			di->vol_buff[i].get_batt_vol = hisi_battery_voltage;

		strncpy(di->vol_buff[i].ops_name, ops_name,
			(HW_BATT_VOL_STR_MAX_LEM - 1));

		i++;
	}

	for (i = 0; i < di->total_vol; i++)
		hwlog_info("para[%d]: ops_name:%s, batt_id:%d\n",
			i, di->vol_buff[i].ops_name, di->vol_buff[i].batt_id);

	return 0;
}

static int hw_batt_vol_probe(struct platform_device *pdev)
{
	struct hw_batt_vol_info *di = NULL;
	struct device_node *np = NULL;
	struct device_node *dev_node = NULL;
	int ret = -1;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_hw_batt_di = di;
	di->pdev = pdev;
	di->dev = &pdev->dev;
	np = pdev->dev.of_node;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"batt_series_num", &di->batt_series_num, 1);

	dev_node = of_find_node_by_name(np, "batt_vol");
	if (!dev_node) {
		hwlog_err("batt_vol dts read failed\n");
		goto fail_free_mem;
	}

	ret = hw_batt_vol_parse_dts(dev_node, di);
	if (ret)
		goto fail_parse_dts;

	hw_batt_vol_sysfs_create_group(di->dev);
	platform_set_drvdata(pdev, di);

	return 0;

fail_parse_dts:
	of_node_put(dev_node);
fail_free_mem:
	devm_kfree(&pdev->dev, di);
	g_hw_batt_di = NULL;

	return ret;
}

static int hw_batt_vol_remove(struct platform_device *pdev)
{
	struct hw_batt_vol_info *info = platform_get_drvdata(pdev);

	if (!info)
		return -ENODEV;

	hw_batt_vol_sysfs_remove_group(info->dev);
	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, info);
	g_hw_batt_di = NULL;

	return 0;
}

static const struct of_device_id hw_batt_vol_match_table[] = {
	{
		.compatible = "huawei,battery_voltage",
		.data = NULL,
	},
	{},
};

static struct platform_driver hw_batt_vol_driver = {
	.probe = hw_batt_vol_probe,
	.remove = hw_batt_vol_remove,
	.driver = {
		.name = "huawei,battery_voltage",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hw_batt_vol_match_table),
	},
};

static int __init hw_batt_vol_init(void)
{
	return platform_driver_register(&hw_batt_vol_driver);
}

static void __exit hw_batt_vol_exit(void)
{
	platform_driver_unregister(&hw_batt_vol_driver);
}

fs_initcall(hw_batt_vol_init);
module_exit(hw_batt_vol_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("huawei battery voltage module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
