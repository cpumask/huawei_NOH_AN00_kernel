/*
 * direct_charger_uevent.c
 *
 * uevent handle for direct charger
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

#include <huawei_platform/power/direct_charger/direct_charger.h>
#include <huawei_platform/power/battery_voltage.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/power_common.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG direct_charge_uevent
HWLOG_REGIST();

static int direct_charge_get_max_current(void)
{
	int adp_limit;
	int cable_limit;
	int bat_limit;
	int max_cur;
	int bat_vol = hw_battery_get_series_num() * BAT_RATED_VOLT;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return 0;

	/* get max current by adapter */
	adp_limit = direct_charge_get_adapter_max_current(
		bat_vol * l_di->dc_volt_ratio);
	adp_limit *= l_di->dc_volt_ratio;

	/* get max current by battery */
	if (l_di->product_max_pwr)
		bat_limit = l_di->product_max_pwr * THOUSAND_UNIT / bat_vol;
	else
		bat_limit = direct_charge_get_battery_max_current();

	/* get max current by cable */
	cable_limit = direct_charge_get_cable_max_current();
	if (l_di->cc_cable_detect_enable && cable_limit)
		max_cur = (bat_limit < cable_limit) ? bat_limit : cable_limit;
	else
		max_cur = bat_limit;

	if (adp_limit)
		max_cur = (max_cur < adp_limit) ? max_cur : adp_limit;

	l_di->max_pwr = max_cur * bat_vol;
	l_di->max_pwr /= THOUSAND_UNIT;

	hwlog_info("l_adp=%d, l_cable=%d, l_bat=%d, m_cur=%d, m_pwr=%d\n",
		adp_limit, cable_limit, bat_limit, max_cur, l_di->max_pwr);

	return max_cur;
}

int direct_charge_get_super_charging_flag(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return 0;

	return l_di->super_charge_flag;
}

void direct_charge_send_normal_charging_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return;

	/* clear quick_charging and super_charging flag */
	l_di->quick_charge_flag = 0;
	l_di->super_charge_flag = 0;

	wired_connect_send_icon_uevent(ICON_TYPE_NORMAL);
}

void direct_charge_send_quick_charging_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return;

	/* clear super_charging flag and set quick_charging flag */
	l_di->quick_charge_flag = 1;
	l_di->super_charge_flag = 0;

	wired_connect_send_icon_uevent(ICON_TYPE_QUICK);
}

void direct_charge_send_super_charging_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return;

	/* clear quick_charging flag and set super_charging flag */
	l_di->quick_charge_flag = 0;
	l_di->super_charge_flag = 1;

	wired_connect_send_icon_uevent(ICON_TYPE_SUPER);
}

void direct_charge_send_icon_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();
	int max_cur = direct_charge_get_max_current();

	if (!l_di)
		return;

	if (max_cur >= l_di->super_ico_current)
		direct_charge_send_super_charging_uevent();
	else
		direct_charge_send_quick_charging_uevent();
}

void direct_charge_send_max_power_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di || l_di->ui_max_pwr <= 0)
		return;

	if (l_di->max_pwr >= l_di->ui_max_pwr)
		power_ui_event_notify(POWER_UI_NE_MAX_POWER, &l_di->max_pwr);
}

void direct_charge_send_soc_decimal_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return;

	power_event_notify(POWER_EVENT_NE_SOC_DECIMAL_DC, &l_di->max_pwr);
}

void direct_charge_send_cable_type_uevent(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di || l_di->is_send_cable_type == 0)
		return;

	power_ui_event_notify(POWER_UI_NE_CABLE_TYPE, &l_di->cable_type);
}
