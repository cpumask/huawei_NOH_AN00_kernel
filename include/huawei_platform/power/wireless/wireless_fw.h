/*
 * wireless_fw.h
 *
 * wireless firmware header file
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

#ifndef _WIRELESS_FW_H_
#define _WIRELESS_FW_H_

#define WIRELESS_FW_PROGRAMED                   1
#define WIRELESS_FW_NON_PROGRAMED               0
#define WIRELESS_FW_ERR_PROGRAMED               2
#define WIRELESS_FW_WORK_DELAYED_TIME           500 /* ms */

#define WIRELESS_PROGRAM_FW                     0
#define WIRELESS_RECOVER_FW                     1

enum wireless_fw_sysfs_type {
	WLFW_SYSFS_BEGIN = 0,
	WLFW_SYSFS_PROGRAM_FW = WLFW_SYSFS_BEGIN,
	WLFW_SYSFS_CHK_FW,
	WLFW_SYSFS_END,
};

struct wireless_fw_ops {
	int (*program_fw)(int);
	int (*is_fw_exist)(void);
	int (*check_fw)(void);
};

struct wireless_fw_dev {
	struct device *dev;
	struct work_struct program_fw_work;
	struct wireless_fw_ops *ops;
	bool program_fw_flag;
};

#ifdef CONFIG_WIRELESS_CHARGER
int wireless_fw_ops_register(struct wireless_fw_ops *ops);
#else
static inline int wireless_fw_ops_register(struct wireless_fw_ops *ops)
{
	return 0;
}
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_FW_H_ */
