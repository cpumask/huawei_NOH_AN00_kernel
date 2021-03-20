/*
 * direct_charger_cable.c
 *
 * direct charger cable driver
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

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG direct_charge_cable
HWLOG_REGIST();

static struct resist_data g_resist_base_data;

/* for second resistance check */
#define SEC_RESIST_IBUS_TH        4000
#define CTC_EMARK_CURR_5A         5000

/* stdandard cable: double 56k cable */
static void direct_charge_detect_std_cable(void)
{
	int ret;
	bool cc_moisture_status = false;
	enum cur_cap c_cap;
	int adapter_type = direct_charge_get_adapter_type();
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di || l_di->cc_cable_detect_ok)
		return;

	/* 10v2a and 10v2.25a no need check cable */
	if ((adapter_type == ADAPTER_TYPE_10V2A) ||
		(adapter_type == ADAPTER_TYPE_10V2P25A)) {
		l_di->cc_cable_detect_ok = 1; /* detect success flag */
		l_di->full_path_res_thld = l_di->std_cable_full_path_res_max;
		direct_charge_send_icon_uevent();
		return;
	}

	if (!l_di->cd_ops || !l_di->cd_ops->cable_detect) {
		hwlog_err("cd_ops or cable_detect is null\n");
		direct_charge_set_stage_status(DC_STAGE_DEFAULT);
		return;
	}

	ret = l_di->cd_ops->cable_detect();
	cc_moisture_status = pd_dpm_get_cc_moisture_status();
	c_cap = pd_dpm_get_cvdo_cur_cap();
	if (!ret || (c_cap >= PD_DPM_CURR_5A))
		l_di->orig_cable_type = DC_STD_CABLE;
	if (ret && !cc_moisture_status && (c_cap < PD_DPM_CURR_5A)) {
		hwlog_info("stdandard cable detect fail\n");
		l_di->cc_cable_detect_ok = 0;
		if (pd_dpm_get_ctc_cable_flag()) {
			l_di->full_path_res_thld =
				l_di->ctc_cable_full_path_res_max;
			l_di->cable_type = DC_STD_CABLE;
			direct_charge_send_icon_uevent();
		} else {
			l_di->full_path_res_thld =
				l_di->nonstd_cable_full_path_res_max;
			l_di->cable_type = DC_NONSTD_CABLE;
			if (l_di->is_show_ico_first)
				direct_charge_send_icon_uevent();
		}
	} else {
		hwlog_info("stdandard cable detect ok\n");
		l_di->cable_type = DC_STD_CABLE;
		l_di->cc_cable_detect_ok = 1;
		l_di->full_path_res_thld = l_di->std_cable_full_path_res_max;
		direct_charge_send_icon_uevent();
	}
}

static int direct_charge_get_emark_cable_max_current(void)
{
	enum cur_cap c_cap = pd_dpm_get_cvdo_cur_cap();

	switch (c_cap) {
	case PD_DPM_CURR_5A:
		return CTC_EMARK_CURR_5A;
	default:
		return 0;
	}
}

int direct_charge_get_cable_max_current(void)
{
	int cable_limit = 0;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (l_di->cc_cable_detect_ok) {
		if (pd_dpm_get_ctc_cable_flag())
			cable_limit = direct_charge_get_emark_cable_max_current() * l_di->dc_volt_ratio;
	} else {
		if (pd_dpm_get_ctc_cable_flag())
			cable_limit = l_di->max_current_for_ctc_cable;
		else
			cable_limit = l_di->max_current_for_nonstd_cable;
	}
	return cable_limit;
}

void direct_charge_detect_cable(void)
{
	direct_charge_detect_std_cable();
	direct_charge_set_stage_status(DC_STAGE_SWITCH_DETECT);

	direct_charge_send_soc_decimal_uevent();
	direct_charge_send_cable_type_uevent();
	direct_charge_send_max_power_uevent();
}

int direct_charge_calculate_path_resistance(int *rpath)
{
	struct resist_data tmp;
	int sum = 0;
	int i;
	int retry = 3; /* 3 : retry times */

	g_resist_base_data.iadapt = 0;
	g_resist_base_data.vadapt = 0;
	g_resist_base_data.ibus = 0;
	g_resist_base_data.vbus = 0;
	for (i = 0; i < retry; ++i) {
		if (direct_charge_get_device_ibus(&tmp.ibus))
			return -1;

		if (direct_charge_get_device_vbus(&tmp.vbus))
			return -1;

		if (direct_charge_get_adapter_voltage(&tmp.vadapt))
			return -1;

		if (direct_charge_get_adapter_current(&tmp.iadapt))
			return -1;

		if (tmp.ibus == 0) {
			hwlog_err("ibus is zero\n");
			return -1;
		}

		/* r = v/i, 1000mohm */
		*rpath = (tmp.vadapt - tmp.vbus) * 1000 / tmp.ibus;
		sum += *rpath;

		g_resist_base_data.ibus += tmp.ibus;
		g_resist_base_data.vbus += tmp.vbus;
		g_resist_base_data.vadapt += tmp.vadapt;

		hwlog_info("[%d]: Rpath=%d, Vapt=%d, Vbus=%d, Iapt=%d, Ibus=%d\n",
			i, *rpath, tmp.vadapt, tmp.vbus, tmp.iadapt, tmp.ibus);
	}

	g_resist_base_data.ibus /= retry;
	g_resist_base_data.vadapt /= retry;
	g_resist_base_data.vbus /= retry;

	*rpath = sum / retry;
	*rpath = *rpath > 0 ? *rpath : -*rpath;

	hwlog_info("Rpath=%d, Vapt=%d, Vbus=%d, Ibus=%d\n",
		*rpath, g_resist_base_data.vadapt, g_resist_base_data.vbus, g_resist_base_data.ibus);

	return 0;
}

int direct_charge_calculate_path_resistance_second(void)
{
	struct resist_data tmp;
	struct resist_data resist = { 0 };
	int i;
	int rpath;
	int retry = 5; /* 5 : retry times */
	struct direct_charge_device *l_di = direct_charge_get_di();
	char tmp_buf[ERR_NO_STRING_SIZE] = { 0 };

	if (!l_di || (l_di->ls_ibus < SEC_RESIST_IBUS_TH))
		return -1;

	for (i = 0; i < retry; ++i) {
		if (direct_charge_get_device_ibus(&tmp.ibus))
			return -1;

		if (direct_charge_get_device_vbus(&tmp.vbus))
			return -1;

		if (direct_charge_get_adapter_voltage(&tmp.vadapt))
			return -1;

		if (direct_charge_get_adapter_current(&tmp.iadapt))
			return -1;

		if (tmp.ibus == 0) {
			hwlog_err("ibus is zero\n");
			return -1;
		}

		resist.ibus += tmp.ibus;
		resist.vbus += tmp.vbus;
		resist.vadapt += tmp.vadapt;

		hwlog_info("[%d]: Vapt=%d, Vbus=%d, Iapt=%d, Ibus=%d\n",
			i, tmp.vadapt, tmp.vbus, tmp.iadapt, tmp.ibus);
	}

	resist.ibus = resist.ibus / retry;
	resist.vadapt = resist.vadapt / retry;
	resist.vbus = resist.vbus / retry;

	if (resist.ibus - g_resist_base_data.ibus == 0)
		return -1;

	/* r = v/i, 1000mohm */
	rpath = ((resist.vadapt - g_resist_base_data.vadapt) - (resist.vbus - g_resist_base_data.vbus)) * 1000 /
		(resist.ibus - g_resist_base_data.ibus);
	rpath = rpath > 0 ? rpath : -rpath;

	snprintf(tmp_buf, sizeof(tmp_buf),
		"f_Rpath=%d, S_Rpath=%d, Vapt=%d, Vbus=%d, Ibus=%d,\t"
		"Vapt0=%d, Vbus0=%d, Ibus0=%d, adp_type=%d, ctc_cable=%d\n",
		l_di->full_path_resistance, rpath, resist.vadapt, resist.vbus,
		resist.ibus, g_resist_base_data.vadapt,
		g_resist_base_data.vbus, g_resist_base_data.ibus,
		direct_charge_get_adapter_type(), pd_dpm_get_ctc_cable_flag());

	hwlog_info("%s\n", tmp_buf);

	if (rpath >= l_di->second_path_res_report_th)
		power_dsm_dmd_report(POWER_DSM_BATTERY,
			DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE_2ND, tmp_buf);

	l_di->second_path_resistance = rpath;
	l_di->second_resist_check_ok = true;

	return 0;
}

/* get the maximum current allowed by direct charging at specified resist */
int direct_charge_resist_handler(int mode, int value)
{
	int i;
	struct direct_charge_device *l_di = direct_charge_get_di_by_mode(mode);
	struct direct_charge_resist_para *para = NULL;

	if (!l_di)
		return 0;

	if (pd_dpm_get_ctc_cable_flag())
		para = l_di->ctc_resist_para;
	else if (l_di->cc_cable_detect_ok == 0)
		para = l_di->nonstd_resist_para;
	else if (l_di->cc_cable_detect_ok == 1)
		para = l_di->std_resist_para;
	else
		return 0;

	for (i = 0; i < DC_RESIST_LEVEL; ++i) {
		if ((value >= para[i].resist_min) &&
			(value < para[i].resist_max))
			return para[i].resist_cur_max;
	}

	hwlog_err("current resist is illegal, cable=%d, resist=%d\n",
		l_di->cc_cable_detect_ok, value);
	return 0;
}

/* get the maximum current allowed by direct charging at specified secondary resist */
int direct_charge_second_resist_handler(void)
{
	int i;
	int path_resist;
	struct direct_charge_device *l_di = direct_charge_get_di();
	struct direct_charge_resist_para *para = NULL;

	if (!l_di || (l_di->second_resist_check_ok == false))
		return 0;

	if (pd_dpm_get_ctc_cable_flag())
		para = l_di->ctc_second_resist_para;
	else
		para = l_di->second_resist_para;

	path_resist = l_di->second_path_resistance;
	for (i = 0; i < DC_RESIST_LEVEL; ++i) {
		if ((path_resist >= para[i].resist_min) &&
			(path_resist < para[i].resist_max))
			return para[i].resist_cur_max;
	}

	hwlog_err("second path resist is illegal, resist=%d\n", path_resist);
	return 0;
}
