/*
 * power_log.c
 *
 * log for power module
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
#include <chipset_common/hwpower/power_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG power_log
HWLOG_REGIST();

struct power_log_device_info *g_power_log_info;

static const char * const g_power_log_device_id_table[] = {
	[POWER_LOG_DEVICE_ID_SERIES_BATT] = "series_batt",
	[POWER_LOG_DEVICE_ID_MULTI_BTB] = "multi_btb",
	[POWER_LOG_DEVICE_ID_BD99954] = "bd99954",
	[POWER_LOG_DEVICE_ID_BQ2419X] = "bq2419x",
	[POWER_LOG_DEVICE_ID_BQ2429X] = "bq2429x",
	[POWER_LOG_DEVICE_ID_BQ2560X] = "bq2560x",
	[POWER_LOG_DEVICE_ID_BQ25882] = "bq25882",
	[POWER_LOG_DEVICE_ID_BQ25892] = "bq25892",
	[POWER_LOG_DEVICE_ID_BQ25970] = "bq25970",
	[POWER_LOG_DEVICE_ID_HL7019] = "hl7019",
	[POWER_LOG_DEVICE_ID_RT9466] = "rt9466",
	[POWER_LOG_DEVICE_ID_RT9471] = "rt9471",
	[POWER_LOG_DEVICE_ID_RT9759] = "rt9759",
	[POWER_LOG_DEVICE_ID_HI6522] = "hi6522",
	[POWER_LOG_DEVICE_ID_HI6523] = "hi6523",
	[POWER_LOG_DEVICE_ID_HI6526] = "hi6526",
};

static struct power_log_device_info *power_log_get_info(void)
{
	if (!g_power_log_info) {
		hwlog_err("g_power_log_info is null\n");
		return NULL;
	}

	return g_power_log_info;
}

static int power_log_get_device_id(const char *str)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_power_log_device_id_table); i++) {
		if (!strncmp(str, g_power_log_device_id_table[i], strlen(str)))
			return i;
	}

	return -1;
}

static int power_log_check_type(int type)
{
	if ((type >= POWER_LOG_TYPE_BEGIN) && (type < POWER_LOG_TYPE_END))
		return 0;

	hwlog_err("invalid type=%d\n", type);
	return -1;
}

static int power_log_operate_ops(struct power_log_ops *ops,
	int type, char *buf, int size)
{
	int ret = POWER_LOG_INVAID_OP;

	switch (type) {
	case POWER_LOG_DUMP_LOG_HEAD:
		if (!ops->dump_log_head)
			return ret;
		memset(buf, 0, size);
		return ops->dump_log_head(buf, size, ops->dev_data);
	case POWER_LOG_DUMP_LOG_CONTENT:
		if (!ops->dump_log_content)
			return ret;
		memset(buf, 0, size);
		return ops->dump_log_content(buf, size, ops->dev_data);
	default:
		return ret;
	}
}

int power_log_common_operate(int type, char *buf, int size)
{
	int i, ret;
	int buf_size;
	int used = 0;
	int unused;
	struct power_log_device_info *info = NULL;

	if (!buf)
		return -EINVAL;

	ret = power_log_check_type(type);
	if (ret)
		return -EINVAL;

	info = power_log_get_info();
	if (!info)
		return -EINVAL;

	mutex_lock(&info->log_lock);

	for (i = 0; i < POWER_LOG_DEVICE_ID_END; i++) {
		if (!info->ops[i])
			continue;

		ret = power_log_operate_ops(info->ops[i], type,
			info->log_buf, POWER_LOG_MAX_SIZE - 1);
		if (ret) {
			/* if op is invalid, must be skip output */
			if (ret == POWER_LOG_INVAID_OP)
				continue;
			hwlog_err("error type=%d, i=%d, ret=%d\n", type, i, ret);
			break;
		}

		unused = size - POWER_LOG_RESERVED_SIZE - used;
		buf_size = strlen(info->log_buf);
		if (unused > buf_size) {
			strncat(buf, info->log_buf, buf_size);
			used += buf_size;
		} else {
			strncat(buf, info->log_buf, unused);
			used += unused;
			break;
		}
	}
	buf_size = strlen("\n\0");
	strncat(buf, "\n\0", buf_size);

	mutex_unlock(&info->log_lock);
	return used + buf_size;
}

int power_log_ops_register(struct power_log_ops *ops)
{
	struct power_log_device_info *info = g_power_log_info;
	int dev_id;

	if (!info || !ops || !ops->dev_name) {
		hwlog_err("info or ops is null\n");
		return -EINVAL;
	}

	dev_id = power_log_get_device_id(ops->dev_name);
	if (dev_id < 0) {
		hwlog_err("%s ops register fail\n", ops->dev_name);
		return -EINVAL;
	}

	info->ops[dev_id] = ops;
	info->total_ops++;

	hwlog_info("total_ops=%d %d:%s ops register ok\n",
		info->total_ops, dev_id, ops->dev_name);
	return 0;
}

static int __init power_log_init(void)
{
	struct power_log_device_info *info = NULL;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->total_ops = 0;
	mutex_init(&info->log_lock);
	g_power_log_info = info;
	return 0;
}

static void __exit power_log_exit(void)
{
	struct power_log_device_info *info = g_power_log_info;

	if (!info)
		return;

	mutex_destroy(&info->log_lock);
	kfree(info);
	g_power_log_info = NULL;
}

subsys_initcall(power_log_init);
module_exit(power_log_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("power log module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
