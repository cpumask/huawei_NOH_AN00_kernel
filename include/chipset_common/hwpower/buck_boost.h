/*
 * buck_boost.h
 *
 * buck_boost macro, interface etc.
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

#ifndef _BUCK_BOOST_H_
#define _BUCK_BOOST_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#define BBST_DEFAULT_VOUT      3450

struct buck_boost_ops {
	bool (*device_check)(void);
	bool (*pwr_good)(void);
	int (*set_vout)(unsigned int);
	int (*set_pwm_enable)(unsigned int);
};

struct buck_boost_dev {
	struct device *dev;
	struct buck_boost_ops *ops;
};

#ifdef CONFIG_BUCKBOOST
extern int buck_boost_ops_register(struct buck_boost_ops *ops);
extern int buck_boost_set_pwm_enable(unsigned int enable);
extern int buck_boost_set_vol(unsigned int vol);
extern bool buck_boost_pwr_good(void);
#else
static inline int buck_boost_ops_register(struct buck_boost_ops *ops)
{
	return 0;
}

static inline int buck_boost_set_pwm_enable(unsigned int enable)
{
	return 0;
}

static inline int buck_boost_set_vol(unsigned int vol)
{
	return 0;
}

static inline bool buck_boost_pwr_good(void)
{
	return true;
}
#endif /* CONFIG_BUCKBOOST */

#endif  /* _BUCK_BOOST_H_ */
