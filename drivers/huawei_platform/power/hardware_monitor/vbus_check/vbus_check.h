/*
 * vbus_check.h
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

#ifndef _VBUS_CHECK_H_
#define _VBUS_CHECK_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>

#define VBUS_MONITOR_INIT_TIME    4000 /* 4s */
#define VBUS_MONITOR_CHECK_TIME   2000 /* 2s */
#define VBUS_ABSENT_MAX_CNTS      5

enum vbus_check_state {
	VBUS_STATE_PRESENT = 0,
	VBUS_STATE_ABSENT = 1,
};

struct vbus_check_dev {
	struct delayed_work monitor_work;
	int absent_cnt;
	int state;
};

#endif /* _VBUS_CHECK_H_ */
