/*
 * direct_charger_path_switch.c
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

#include <huawei_platform/power/direct_charger/direct_charger.h>
#include <huawei_platform/power/wired_channel_switch.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG direct_charge_path
HWLOG_REGIST();

static const char * const g_dc_charging_path[PATH_END] = {
	[PATH_NORMAL] = "path_normal",
	[PATH_LVC] = "path_lvc",
	[PATH_SC] = "path_sc",
};

static const char *direct_charge_get_charging_path_string(unsigned int path)
{
	if ((path >= PATH_BEGIN) && (path < PATH_END))
		return g_dc_charging_path[path];

	return "illegal charging_path";
}

static int direct_charge_open_wired_channel(struct direct_charge_device *di)
{
	if (di->need_wired_sw_off)
		return wired_chsw_set_wired_channel(WIRED_CHANNEL_MAIN, WIRED_CHANNEL_RESTORE);

	return 0;
}

static int direct_charge_close_wired_channel(struct direct_charge_device *di)
{
	if (di->need_wired_sw_off)
		return wired_chsw_set_wired_channel(WIRED_CHANNEL_ALL, WIRED_CHANNEL_CUTOFF);

	return 0;
}

/* switch charging path to normal charging path */
static int direct_charge_switch_path_to_normal_charging(void)
{
	int ret;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	msleep(WAIT_LS_DISCHARGE); /* need to wait device discharge */

	/* no need to check the return value in here */
	hw_usb_ldo_supply_disable(HW_USB_LDO_CTRL_DIRECT_CHARGE);

	ret = direct_charge_open_wired_channel(l_di);

	if (pmic_vbus_irq_is_enabled())
		restore_pluggin_pluggout_interrupt();

	return ret;
}

/* switch charging path to lvc or sc charging path */
static int direct_charge_switch_path_to_dc_charging(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (pmic_vbus_irq_is_enabled()) {
		l_di->cutoff_normal_flag = 1;
		ignore_pluggin_and_pluggout_interrupt();
	}

	hw_usb_ldo_supply_enable(HW_USB_LDO_CTRL_DIRECT_CHARGE);

	if (l_di->scp_work_on_charger) {
		direct_charge_adapter_protocol_power_supply(ENABLE);
		charge_set_hiz_enable_by_direct_charge(HIZ_MODE_ENABLE);
	}

	msleep(100); /* delay 100ms */
	return direct_charge_close_wired_channel(l_di);
}

int direct_charge_switch_charging_path(unsigned int path)
{
	hwlog_info("switch to %d,%s charging path\n",
		path, direct_charge_get_charging_path_string(path));

	switch (path) {
	case PATH_NORMAL:
		return direct_charge_switch_path_to_normal_charging();
	case PATH_LVC:
	case PATH_SC:
		return direct_charge_switch_path_to_dc_charging();
	default:
		return -1;
	}
}

void direct_charge_open_aux_wired_channel(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (l_di->multi_ic_mode_para.support_multi_ic &&
		(l_di->multi_ic_error_cnt < MULTI_IC_CHECK_ERR_CNT_MAX)) {
		wired_chsw_set_wired_channel(WIRED_CHANNEL_AUX, WIRED_CHANNEL_RESTORE);
		msleep(5); /* delay 5ms, gennerally 0.1ms */
	}
}

void direct_charge_close_aux_wired_channel(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (l_di->multi_ic_mode_para.support_multi_ic)
		wired_chsw_set_wired_channel(WIRED_CHANNEL_AUX, WIRED_CHANNEL_CUTOFF);
}
