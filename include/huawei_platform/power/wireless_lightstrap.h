/*
 * wireless_lightstrap.h
 *
 * wireless lightstrap driver
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

#ifndef _WIRELESS_LIGHTSTRAP_H_
#define _WIRELESS_LIGHTSTRAP_H_

#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <huawei_platform/power/wireless/wireless_tx/wireless_tx_pwr_src.h>

struct lightstrap_di {
	struct device *dev;
	struct notifier_block event_nb;
	struct blocking_notifier_head event_nh;
	struct work_struct event_work;
	struct delayed_work check_work;
	u8 product_type;
	unsigned long event_type;
	bool is_opened_by_hall;
};

enum lightstrap_event_type {
	LIGHTSTRAP_EVENT_ON,
	LIGHTSTRAP_EVENT_OFF,
	LIGHTSTRAP_EVENT_GET_PRODUCT_TYPE,
	LIGHTSTRAP_EVENT_EPT,
};

enum wireless_lightstrap_sysfs_type {
	LIGHTSTRAP_SYSFS_DEV_PRODUCT_TYPE,
};

#ifdef CONFIG_WIRELESS_ACCESSORY
void lightstrap_event_notify(unsigned long e, void *v);
bool lightstrap_need_specify_pwr_src(void);
enum wltx_pwr_src lightstrap_specify_pwr_src(void);
#else
static inline void lightstrap_event_notify(unsigned long e, void *v) {}

static inline bool lightstrap_need_specify_pwr_src(void)
{
	return false;
}

static inline enum wltx_pwr_src lightstrap_specify_pwr_src(void)
{
	return PWR_SRC_NULL;
}
#endif /* CONFIG_WIRELESS_ACCESSORY */

#endif /* _WIRELESS_LIGHTSTRAP_H_ */
