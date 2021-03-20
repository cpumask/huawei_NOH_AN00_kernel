/*
 * huawei_battery_temp.h
 *
 * huawei battery temp driver
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

#ifndef _HUAWEI_BATTERY_TEMP_H_
#define _HUAWEI_BATTERY_TEMP_H_

#ifdef CONFIG_HISI_COUL
#include <linux/power/hisi/coul/hisi_coul_drv.h>
#endif

#define BATT_TEMP_NAME_MAX 64

enum batt_temp_id {
	BAT_TEMP_0 = 0,
	BAT_TEMP_1,
	BAT_TEMP_MIXED,
	BTB_TEMP_0 = 0,
	BTB_TEMP_1,
	BTB_TEMP_MIXED,
};

/*
 * get_temp_sync return current temperature.
 * get_temp return statistical temperature.
 */
struct batt_temp_ops {
	int (*get_temp_sync)(enum batt_temp_id id, int *temp);
	int (*get_temp)(enum batt_temp_id id, int *temp);
};

#ifdef CONFIG_HUAWEI_BATTERY_TEMP
int huawei_battery_temp(enum batt_temp_id id, int *temp);
int huawei_battery_temp_now(enum batt_temp_id id, int *temp);
int huawei_battery_temp_register(const char *name, struct batt_temp_ops *ops);
#else
static inline int huawei_battery_temp(enum batt_temp_id id, int *temp)
{
	if (!temp)
		return -1;

	*temp = hisi_battery_temperature();
	return 0;
}

static inline int huawei_battery_temp_now(enum batt_temp_id id, int *temp)
{
	if (!temp)
		return -1;

	*temp = hisi_battery_temperature_for_charger();
	return 0;
}

static inline int huawei_battery_temp_register(const char *name,
	struct batt_temp_ops *ops)
{
	return -ENODEV;
}
#endif /* CONFIG_HUAWEI_BATTERY_TEMP */

#endif /* _HUAWEI_BATTERY_TEMP_H_ */
