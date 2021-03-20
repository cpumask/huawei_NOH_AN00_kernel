/*
 * vbus_check.c
 *
 * vbus abnormal monitor driver
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

#include "vbus_check.h"
#include <huawei_platform/power/direct_charger/direct_charger.h>
#include <chipset_common/hwpower/power_event_ne.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/mfd/hisi_pmic.h>

#define HWLOG_TAG vbus_check
HWLOG_REGIST();

struct vbus_check_dev *g_vbus_check_dev;

static struct vbus_check_dev *vbus_check_get_dev(void)
{
	if (!g_vbus_check_dev) {
		hwlog_err("g_vbus_check_dev is null\n");
		return NULL;
	}

	return g_vbus_check_dev;
}

static void vbus_check_monitor(struct vbus_check_dev *l_dev)
{
	struct power_event_notify_data n_data;

#ifdef CONFIG_DIRECT_CHARGER
	if (direct_charge_in_charging_stage() == DC_IN_CHARGING_STAGE) {
		l_dev->absent_cnt = 0;
		return;
	}
#endif /* CONFIG_DIRECT_CHARGER */

	if (hisi_pmic_get_vbus_status() == 0) {
		l_dev->state = VBUS_STATE_ABSENT;
		l_dev->absent_cnt++;
	} else {
		l_dev->state = VBUS_STATE_PRESENT;
		l_dev->absent_cnt = 0;
	}

	hwlog_info("state=%d, cnt=%d\n", l_dev->state, l_dev->absent_cnt);
	if (l_dev->absent_cnt < VBUS_ABSENT_MAX_CNTS)
		return;

	n_data.event = "VBUS_ABSENT=";
	n_data.event_len = 12; /* length of VBUS_ABSENT= */
	power_event_notify(POWER_EVENT_NE_VBUS_ABSENT, &n_data);
}

static void vbus_check_monitor_work(struct work_struct *work)
{
	struct vbus_check_dev *l_dev = vbus_check_get_dev();

	if (!l_dev)
		return;

	if (!power_cmdline_is_powerdown_charging_mode())
		return;

	vbus_check_monitor(l_dev);
	schedule_delayed_work(&l_dev->monitor_work,
		msecs_to_jiffies(VBUS_MONITOR_CHECK_TIME));
}

static int __init vbus_check_init(void)
{
	struct vbus_check_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_vbus_check_dev = l_dev;
	INIT_DELAYED_WORK(&l_dev->monitor_work, vbus_check_monitor_work);
	schedule_delayed_work(&l_dev->monitor_work,
		msecs_to_jiffies(VBUS_MONITOR_INIT_TIME));
	return 0;
}

static void __exit vbus_check_exit(void)
{
	struct vbus_check_dev *l_dev = g_vbus_check_dev;

	if (!l_dev)
		return;

	cancel_delayed_work(&l_dev->monitor_work);
	kfree(l_dev);
	g_vbus_check_dev = NULL;
}

fs_initcall_sync(vbus_check_init);
module_exit(vbus_check_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("vbus abnormal monitor module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
