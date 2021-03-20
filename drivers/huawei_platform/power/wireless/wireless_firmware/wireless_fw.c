/*
 * wireless_fw.c
 *
 * wireless firmware driver
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

#include <linux/init.h>
#include <linux/device.h>

#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/power_cmdline.h>
#include <chipset_common/hwpower/power_sysfs.h>
#include <chipset_common/hwpower/power_common.h>
#include <huawei_platform/power/wireless/wireless_fw.h>

#define HWLOG_TAG wireless_fw
HWLOG_REGIST();

static struct wireless_fw_dev *g_wireless_fw_di;

int wireless_fw_ops_register(struct wireless_fw_ops *ops)
{
	if (!ops || !g_wireless_fw_di || g_wireless_fw_di->ops) {
		hwlog_err("%s: di null or already registered\n", __func__);
		return -EPERM;
	}

	g_wireless_fw_di->ops = ops;
	return 0;
}

static void wireless_fw_program_fw_work(struct work_struct *work)
{
	int ret;
	static bool first_in = true;
	struct wireless_fw_dev *di = g_wireless_fw_di;

	if (!di)
		return;

	if (!di->ops || !di->ops->is_fw_exist || !di->ops->program_fw) {
		di->program_fw_flag = false;
		return;
	}

	hwlog_info("[%s] ++\n", __func__);

	ret = di->ops->is_fw_exist();
	if (ret == WIRELESS_FW_PROGRAMED) {
		di->program_fw_flag = false;
		return;
	}

	if (first_in) {
		first_in = false;
		(void)di->ops->program_fw(WIRELESS_PROGRAM_FW);
	} else {
		(void)di->ops->program_fw(WIRELESS_RECOVER_FW);
	}

	di->program_fw_flag = false;
	hwlog_info("[%s] --\n", __func__);
}

static void wireless_fw_sysfs_program_fw(void)
{
	struct wireless_fw_dev *di = g_wireless_fw_di;

	if (!di)
		return;

	if (!power_cmdline_is_factory_mode())
		return;

	if (!di->program_fw_flag) {
		di->program_fw_flag = true;
		schedule_work(&di->program_fw_work);
	}
}

static int wireless_fw_sysfs_check_fw(char *buf, int len)
{
	struct wireless_fw_dev *di = g_wireless_fw_di;

	if (!di || !di->ops || !di->ops->check_fw)
		return -EINVAL;

	if (!power_cmdline_is_factory_mode())
		return 0;

	if (di->program_fw_flag)
		return -EBUSY;

	return snprintf(buf, len, "%s\n", di->ops->check_fw() ?
		"0: otp is bad" : "1: otp is good");
}

static int wireless_fw_sysfs_check_fw_exist(char *buf, int len)
{
	struct wireless_fw_dev *di = g_wireless_fw_di;

	if (!di || !di->ops || !di->ops->is_fw_exist)
		return -EINVAL;

	if (!power_cmdline_is_factory_mode())
		return 0;

	if (di->program_fw_flag)
		return -EBUSY;

	return snprintf(buf, len, "%d\n", di->ops->is_fw_exist());
}

#ifdef CONFIG_SYSFS
static ssize_t wireless_fw_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);
static ssize_t wireless_fw_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);

static struct power_sysfs_attr_info wireless_fw_sysfs_field_tbl[] = {
	power_sysfs_attr_rw(wireless_fw, 0644, WLFW_SYSFS_PROGRAM_FW, program_fw),
	power_sysfs_attr_ro(wireless_fw, 0444, WLFW_SYSFS_CHK_FW, check_fw),
};

#define WIRELESS_FW_SYSFS_ATTRS_SIZE  ARRAY_SIZE(wireless_fw_sysfs_field_tbl)

static struct attribute *wireless_fw_sysfs_attrs[WIRELESS_FW_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group wireless_fw_sysfs_attr_group = {
	.attrs = wireless_fw_sysfs_attrs,
};

static ssize_t wireless_fw_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;

	info = power_sysfs_lookup_attr(attr->attr.name,
		wireless_fw_sysfs_field_tbl, WIRELESS_FW_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case WLFW_SYSFS_PROGRAM_FW:
		return wireless_fw_sysfs_check_fw_exist(buf, PAGE_SIZE);
	case WLFW_SYSFS_CHK_FW:
		return wireless_fw_sysfs_check_fw(buf, PAGE_SIZE);
	default:
		return 0;
	}
}

static ssize_t wireless_fw_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct power_sysfs_attr_info *info = NULL;
	long val = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		wireless_fw_sysfs_field_tbl, WIRELESS_FW_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case WLFW_SYSFS_PROGRAM_FW:
		/* 10: decimal 1: program_otp */
		if ((kstrtol(buf, POWER_BASE_DEC, &val) < 0) || (val != 1)) {
			hwlog_err("%s: input not valid\n", __func__);
			return -EINVAL;
		}
		wireless_fw_sysfs_program_fw();
		break;
	default:
		break;
	}
	return count;
}

static struct device *wireless_fw_sysfs_create_group(void)
{
	power_sysfs_init_attrs(wireless_fw_sysfs_attrs,
		wireless_fw_sysfs_field_tbl, WIRELESS_FW_SYSFS_ATTRS_SIZE);
	return power_sysfs_create_group("hw_power", "wireless_fw",
		&wireless_fw_sysfs_attr_group);
}

static void wireless_fw_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &wireless_fw_sysfs_attr_group);
}
#else
static inline struct device *wireless_fw_sysfs_create_group(void)
{
	return NULL;
}

static inline void wireless_fw_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int __init wireless_fw_init(void)
{
	struct wireless_fw_dev *di = NULL;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_wireless_fw_di = di;

	INIT_WORK(&di->program_fw_work, wireless_fw_program_fw_work);
	di->dev = wireless_fw_sysfs_create_group();

	return 0;
}

static void __exit wireless_fw_exit(void)
{
	if (!g_wireless_fw_di)
		return;

	wireless_fw_sysfs_remove_group(g_wireless_fw_di->dev);
	kfree(g_wireless_fw_di);
	g_wireless_fw_di = NULL;
}

subsys_initcall_sync(wireless_fw_init);
module_exit(wireless_fw_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wireless firmware driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
