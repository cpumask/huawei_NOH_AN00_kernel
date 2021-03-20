/*
 * tps63810.h
 *
 * tps63810 header
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

#ifndef _TPS63810_H_
#define _TPS63810_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/i2c.h>

#define TPS_I2C_RETRY_CNT            3

/* register status */
#define TPS63810_STATUS_ADDR         0x02
#define TPS63810_STATUS_PG_MASK      BIT(0)
#define TPS63810_STATUS_PG_SHIFT     0

/* register vout1 */
#define TPS63810_VOUT1_ADDR          0x04
#define TPS63810_DEFAULT_VOUT1       3300
#define TPS63810_MIN_VOUT1           1825
#define TPS63810_MAX_VOUT1           5000
#define TPS63810_VOUT1_STEP          25

/* register vout2 */
#define TPS63810_VOUT2_ADDR          0x05
#define TPS63810_DEFAULT_VOUT2       3450
#define TPS63810_MIN_VOUT2           1825
#define TPS63810_MAX_VOUT2           5000
#define TPS63810_VOUT2_STEP          25

struct tps63810_device_info {
	struct i2c_client *client;
	struct device *dev;
	u32 vsel_state;
};

#endif /* _TPS63810_H_ */
