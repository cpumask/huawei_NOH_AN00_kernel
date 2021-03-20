/*
 * tpm driver for GP
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include "tpm.h"
#include "tpm-huawei.h"

int tpm_reset(struct spi_device *dev)
{
	int rst_gpio;
	struct device_node *np = dev->dev.of_node;
	if (!np) {
		dev_err(&dev->dev, "no dt node defined\n");
		return -EINVAL;
	}

	rst_gpio = of_get_named_gpio(np, "rst_gpio", 0);
	if (rst_gpio < 0) {
		dev_err(&dev->dev, "get rst_gpio failed\n");
		return -EINVAL;
	}

	if (!gpio_is_valid(rst_gpio)) {
		dev_err(&dev->dev, "rst_gpio is not valid\n");
		return -EINVAL;
	}

	if (devm_gpio_request(&dev->dev, rst_gpio, "rst_gpio")) {
		dev_err(&dev->dev, "Could not request %d gpio\n", rst_gpio);
		return -EINVAL;
	} else if (gpio_direction_output(rst_gpio, GPIO_LOW)) {
		dev_err(&dev->dev, "Could not set gpio %d as output\n", rst_gpio);
		return -EINVAL;
	}

	gpio_set_value(rst_gpio, GPIO_LOW);
	tpm_msleep(5);
	gpio_set_value(rst_gpio, GPIO_HIGH);
	tpm_msleep(2);

	return 0;
}
