/*
 * power_platform.h
 *
 * differentiated interface related to chip platform
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

#ifndef _POWER_PLATFORM_H_
#define _POWER_PLATFORM_H_

#include <linux/hisi/hisi_adc.h>
#include <linux/power/hisi/coul/hisi_coul_drv.h>

static inline int power_platform_adc_get_adc(int adc_channel)
{
	return hisi_adc_get_adc(adc_channel);
}

static inline int power_platform_adc_get_value(int adc_channel)
{
	return hisi_adc_get_value(adc_channel);
}

static inline int power_platform_get_battery_current_avg(void)
{
	return hisi_battery_current_avg();
}

#endif /* _POWER_PLATFORM_H_ */
