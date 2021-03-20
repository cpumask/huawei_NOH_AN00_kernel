/*
 * direct_charger_path_switch.h
 *
 * path switch for direct charger
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

#ifndef _DIRECT_CHARGER_PATH_SWITCH_H_
#define _DIRECT_CHARGER_PATH_SWITCH_H_

/*
 * define charging path with direct charge
 * DC is simplified identifier with direct-charge
 */
enum direct_charge_charging_path_type {
	PATH_BEGIN,
	PATH_NORMAL = PATH_BEGIN,
	PATH_LVC,
	PATH_SC,
	PATH_END,
};

#ifdef CONFIG_DIRECT_CHARGER
int direct_charge_switch_charging_path(unsigned int path);
void direct_charge_open_aux_wired_channel(void);
void direct_charge_close_aux_wired_channel(void);
#else
static inline int direct_charge_switch_charging_path(unsigned int path)
{
	return -1;
}

static inline void direct_charge_open_aux_wired_channel(void)
{
}

static inline void direct_charge_close_aux_wired_channel(void)
{
}
#endif /* CONFIG_DIRECT_CHARGER */

#endif /* _DIRECT_CHARGER_PATH_SWITCH_H_ */
