/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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

#ifndef __HUAWEI_TOUCHSCREEN_ALGO_H_
#define __HUAWEI_TOUCHSCREEN_ALGO_H_
#include "huawei_touchscreen_chips.h"

#define TP_FINGER 1
#define TP_STYLUS 2
#define TP_GLOVE 6
#define FILTER_GLOVE_NUMBER	4


extern struct ts_data g_ts_data;
int ts_register_algo_func(struct ts_device_data *chip_data);
#endif
