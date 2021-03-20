/*
 * tz_spi_notify.h
 *
 * exported funcs for spi interrupt actions
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
 */
#ifndef _TZ_SPI_NOTIFY_H_
#define _TZ_SPI_NOTIFY_H_
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include "teek_ns_client.h"

int tz_spi_init(struct device *class_dev, struct device_node *np);
void tz_spi_exit(void);
int tc_ns_tst_cmd(struct tc_ns_dev_file *dev_id, void *argp);
#endif
