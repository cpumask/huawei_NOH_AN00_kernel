// SPDX-License-Identifier: GPL-2.0
/*
 * Register config for USB on HiSilicon platform
 *
 * Copyright (C) 2017-2018 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#ifndef __HISI_USB_REG_CFG_H__
#define __HISI_USB_REG_CFG_H__

#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

enum reg_cfg_type {
	WRITE_ONLY,
	BIT_MASK,
	READ_WRITE,
	TEST_READ,
	REG_CFG_TYPE_MAX
};

struct hisi_usb_reg_cfg {
	struct regmap *regmap;
	enum reg_cfg_type cfg_type;
	unsigned int offset;
	unsigned int value;
	unsigned int mask;
};

extern struct hisi_usb_reg_cfg *of_get_hisi_usb_reg_cfg(
		struct device_node *np,
		const char *prop_name);
extern int get_hisi_usb_reg_cfg_array(struct device *dev, const char *prop_name,
		struct hisi_usb_reg_cfg ***reg_cfgs, int *num_cfgs);
extern void of_remove_hisi_usb_reg_cfg(struct hisi_usb_reg_cfg *reg_cfg);
extern void free_hisi_usb_reg_cfg_array(struct hisi_usb_reg_cfg **reg_cfgs,
					int num_cfgs);
extern int hisi_usb_reg_write(struct hisi_usb_reg_cfg *reg_cfg);
extern int hisi_usb_reg_test_cfg(struct hisi_usb_reg_cfg *reg_cfg);
extern int hisi_usb_reg_write_array(struct hisi_usb_reg_cfg **reg_cfgs,
				    int num_cfgs);

#endif /* __HISI_USB_REG_CFG_H__ */
