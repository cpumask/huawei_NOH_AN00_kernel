/*
 * direct_charger_check.h
 *
 * direct charger check driver
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

#ifndef _DIRECT_CHARGER_CHECK_H_
#define _DIRECT_CHARGER_CHECK_H_

enum direct_charge_check_notify_type {
	DC_CHECK_NOTIFY_BEGIN = 0,
	DC_CHECK_NOTIFY_START = DC_CHECK_NOTIFY_BEGIN,
	DC_CHECK_NOTIFY_CHG_SUCC,
	DC_CHECK_NOTIFY_END,
};

#ifdef CONFIG_DIRECT_CHARGER
bool direct_charge_get_can_enter_status(void);
bool direct_charge_in_mode_check(void);
void direct_charge_check(void);
int direct_charge_pre_check(void);
int direct_charge_check_notifier_register(struct notifier_block *nb);
int direct_charge_check_notifier_unregister(struct notifier_block *nb);
bool direct_charge_check_charge_done(void);
#else
static inline bool direct_charge_get_can_enter_status(void)
{
	return false;
}

static inline bool direct_charge_in_mode_check(void)
{
	return false;
}

static inline void direct_charge_check(void)
{
}

static inline int direct_charge_pre_check(void)
{
	return -1;
}

static inline int direct_charge_check_notifier_register(struct notifier_block *nb)
{
	return 0;
}

static inline int direct_charge_check_notifier_unregister(struct notifier_block *nb)
{
	return 0;
}

static inline bool direct_charge_check_charge_done(void)
{
	return true;
}
#endif /* CONFIG_DIRECT_CHARGER */

#endif /* _DIRECT_CHARGER_CHECK_H_ */
