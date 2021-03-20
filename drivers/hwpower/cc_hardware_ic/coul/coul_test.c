/*
 * coul_test.c
 *
 * test for coul driver
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
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/coul_test.h>
#include <chipset_common/hwpower/power_debug.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG coul_test
HWLOG_REGIST();

static struct coul_test_dev *g_coul_test_dev;

static ssize_t coul_test_dbg_flag_store(void *dev_data,
	const char *buf, size_t size)
{
	unsigned long data = 0;
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return -EINVAL;

	if (kstrtoul(buf, 0, &data)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	l_dev->info.flag = data;
	return size;
}

static ssize_t coul_test_dbg_bat_exist_store(void *dev_data,
	const char *buf, size_t size)
{
	int data = 0;
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return -EINVAL;

	if (kstrtoint(buf, 0, &data)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	l_dev->info.bat_exist = data;
	return size;
}

static ssize_t coul_test_dbg_bat_capacity_store(void *dev_data,
	const char *buf, size_t size)
{
	int data = 0;
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return -EINVAL;

	if (kstrtoint(buf, 0, &data)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	l_dev->info.bat_capacity = data;
	return size;
}

static ssize_t coul_test_dbg_flag_show(void *dev_data,
	char *buf, size_t size)
{
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return scnprintf(buf, size, "not support\n");

	return scnprintf(buf, size, "ct_flag is %d\n",
		l_dev->info.flag);
}

static ssize_t coul_test_dbg_bat_exist_show(void *dev_data,
	char *buf, size_t size)
{
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return scnprintf(buf, size, "not support\n");

	return scnprintf(buf, size, "ct_bat_exist is %d\n",
		l_dev->info.bat_exist);
}

static ssize_t coul_test_dbg_bat_capacity_show(void *dev_data,
	char *buf, size_t size)
{
	struct coul_test_dev *l_dev = (struct coul_test_dev *)dev_data;

	if (!l_dev)
		return scnprintf(buf, size, "not support\n");

	return scnprintf(buf, size, "ct_bat_capacity is %d\n",
		l_dev->info.bat_capacity);
}

static void coul_test_dbg_register(struct coul_test_dev *dev)
{
	power_dbg_ops_register("ct_flag", (void *)dev,
		(power_dbg_show)coul_test_dbg_flag_show,
		(power_dbg_store)coul_test_dbg_flag_store);
	power_dbg_ops_register("ct_bat_exist", (void *)dev,
		(power_dbg_show)coul_test_dbg_bat_exist_show,
		(power_dbg_store)coul_test_dbg_bat_exist_store);
	power_dbg_ops_register("ct_bat_capacity", (void *)dev,
		(power_dbg_show)coul_test_dbg_bat_capacity_show,
		(power_dbg_store)coul_test_dbg_bat_capacity_store);
}

struct coul_test_info *coul_test_get_info_data(void)
{
	if (!g_coul_test_dev) {
		hwlog_err("g_coul_test_dev is null\n");
		return NULL;
	}

	return &g_coul_test_dev->info;
}

static int __init coul_test_init(void)
{
	struct coul_test_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	coul_test_dbg_register(l_dev);
	g_coul_test_dev = l_dev;
	return 0;
}

static void __exit coul_test_exit(void)
{
	if (!g_coul_test_dev)
		return;

	kfree(g_coul_test_dev);
	g_coul_test_dev = NULL;
}

subsys_initcall_sync(coul_test_init);
module_exit(coul_test_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("test for coul driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
