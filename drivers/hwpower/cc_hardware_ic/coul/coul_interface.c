/*
 * coul_interface.c
 *
 * interface for coul driver
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
#include <chipset_common/hwpower/coul_interface.h>
#include <chipset_common/hwpower/coul_test.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG coul_interface
HWLOG_REGIST();

static struct coul_interface_dev *g_coul_interface_dev;

static const char * const g_coul_interface_type_table[] = {
	[COUL_TYPE_MAIN] = "main",
	[COUL_TYPE_AUX] = "aux",
};

static int coul_interface_check_type(unsigned int type)
{
	if ((type >= COUL_TYPE_BEGIN) && (type < COUL_TYPE_END))
		return 0;

	hwlog_err("invalid coul_type=%u\n", type);
	return -1;
}

static int coul_interface_get_type(const char *str)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_coul_interface_type_table); i++) {
		if (!strncmp(str, g_coul_interface_type_table[i], strlen(str)))
			return i;
	}

	hwlog_err("invalid coul_type_str=%s\n", str);
	return -1;
}

static struct coul_interface_ops *coul_interface_get_ops(int type)
{
	if (coul_interface_check_type(type))
		return NULL;

	if (!g_coul_interface_dev || !g_coul_interface_dev->p_ops[type]) {
		hwlog_err("g_coul_interface_dev or p_ops is null\n");
		return NULL;
	}

	return g_coul_interface_dev->p_ops[type];
}

int coul_interface_is_coul_ready(int type)
{
	struct coul_interface_ops *l_ops = coul_interface_get_ops(type);

	if (l_ops && l_ops->is_coul_ready)
		return l_ops->is_coul_ready();

	return 0;
}

int coul_interface_is_battery_exist(int type)
{
	struct coul_interface_ops *l_ops = coul_interface_get_ops(type);
	struct coul_test_info *l_info = coul_test_get_info_data();

	if (l_info && (l_info->flag & COUL_TEST_BAT_EXIST)) {
		hwlog_info("battery_exist is %d\n", l_info->bat_exist);
		return l_info->bat_exist;
	}

	if (l_ops && l_ops->is_battery_exist)
		return l_ops->is_battery_exist();

	return 0;
}

int coul_interface_get_battery_capacity(int type)
{
	struct coul_interface_ops *l_ops = coul_interface_get_ops(type);
	struct coul_test_info *l_info = coul_test_get_info_data();

	if (l_info && (l_info->flag & COUL_TEST_BAT_CAPACITY)) {
		hwlog_info("battery_cap is %d\n", l_info->bat_capacity);
		return l_info->bat_capacity;
	}

	if (l_ops && l_ops->get_battery_capacity)
		return l_ops->get_battery_capacity();

	return -EPERM;
}

int coul_interface_ops_register(struct coul_interface_ops *ops)
{
	int type;

	if (!g_coul_interface_dev || !ops || !ops->type_name) {
		hwlog_err("g_coul_interface_dev or ops or type_name is null\n");
		return -1;
	}

	type = coul_interface_get_type(ops->type_name);
	if (type < 0) {
		hwlog_err("%s ops register fail\n", ops->type_name);
		return -1;
	}

	g_coul_interface_dev->p_ops[type] = ops;
	g_coul_interface_dev->total_ops++;

	hwlog_info("total_ops=%d type=%d:%s ops register ok\n",
		g_coul_interface_dev->total_ops, type, ops->type_name);
	return 0;
}

static int __init coul_interface_init(void)
{
	struct coul_interface_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_coul_interface_dev = l_dev;
	return 0;
}

static void __exit coul_interface_exit(void)
{
	if (!g_coul_interface_dev)
		return;

	kfree(g_coul_interface_dev);
	g_coul_interface_dev = NULL;
}

subsys_initcall_sync(coul_interface_init);
module_exit(coul_interface_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("interface for coul driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
