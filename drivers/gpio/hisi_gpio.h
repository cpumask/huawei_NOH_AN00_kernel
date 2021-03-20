/*
 * hisi_gpio.h
 *
 * gpio header file
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/hwspinlock.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/of_address.h>
#include <linux/version.h>

#ifdef CONFIG_HISI_TUI_PL061
#include "../hisi/tzdriver/tui.h"
#endif
#define GPIODATA 0x3fc
#define	GPIO_HWLOCK_ID	1
#define	LOCK_TIMEOUT	1000
#ifdef CONFIG_PM

struct pl061_context_save_regs {
	u8 gpio_data;
	u8 gpio_dir;
	u8 gpio_is;
	u8 gpio_ibe;
	u8 gpio_iev;
	u8 gpio_ie;
};
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0))
struct pl061 {
	raw_spinlock_t	lock;
	int		sec_status;
	void __iomem	*base;
	struct gpio_chip	gc;
	int                     parent_irq;
	bool		uses_pinctrl;
#ifdef CONFIG_PM
	struct pl061_context_save_regs csave_regs;
#endif
	struct amba_device *adev;
};

unsigned int get_gpio_hwspinlock_status(void);

int pl061_check_security_status(struct pl061 *chip);
#else
struct pl061_gpio {
	spinlock_t	lock;
	int		sec_status;
	void __iomem	*base;
	struct gpio_chip	gc;
	bool		uses_pinctrl;
#ifdef CONFIG_PM
	struct pl061_context_save_regs csave_regs;
#endif
	struct amba_device *adev;
};

int pl061_check_security_status(struct pl061_gpio *chip);
#endif
int pl061_parse_gpio_base(struct device *dev);
extern int gpiochip_hi6502_request(struct gpio_chip *chip, unsigned int offset);
extern void gpiochip_hi6502_free(struct gpio_chip *chip, unsigned int offset);
#ifdef CONFIG_HISI_TUI_PL061
int pl061_tui_request(struct device *dev);
int pl061_tui_release(struct device *dev);
int pl061_tui_switch_func(void *pdata, int secure);
void pl061_register_TUI_driver(struct device_node *np, struct device *dev);
#endif
