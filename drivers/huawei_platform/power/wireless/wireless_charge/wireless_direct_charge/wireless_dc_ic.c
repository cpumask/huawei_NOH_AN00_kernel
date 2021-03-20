/*
 * wireless_dc_ic.c
 *
 * direct charge ic interface for wireless direct charge
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

#include <linux/delay.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/wireless_charger.h>
#include <huawei_platform/power/wireless_power_supply.h>
#include <huawei_platform/power/wireless_direct_charger.h>
#include <chipset_common/hwpower/power_dts.h>
#include <huawei_platform/power/direct_charger/direct_charge_ic_manager.h>

#define HWLOG_TAG wireless_dc_ic
HWLOG_REGIST();

static int wldc_ic_enable_multi_ic(struct wldc_dev_info *di, int type, int enable)
{
	if (enable && dc_ic_init(SC_MODE, type)) {
		di->ic_data.multi_ic_err_cnt++;
		return -1;
	}
	if (enable && dc_batinfo_init(SC_MODE, type)) {
		di->ic_data.multi_ic_err_cnt++;
		return -1;
	}

	if (dc_ic_enable(SC_MODE, type, enable)) {
		di->ic_data.multi_ic_err_cnt++;
		return -1;
	}
	if (di->wldc_stage == WLDC_STAGE_STOP_CHARGING)
		return -1;

	return 0;
}

static int wldc_ic_check_multi_ic_current(struct wldc_dev_info *di)
{
	int main_ibus = 0;
	int aux_ibus = 0;
	int count = WLDC_IC_IBUS_CHECK_CNT;

	while (count) {
		if (di->wldc_stage == WLDC_STAGE_STOP_CHARGING)
			return -1;
		if (dc_get_ic_ibus(SC_MODE, CHARGE_IC_MAIN, &main_ibus))
			return -1;
		if (dc_get_ic_ibus(SC_MODE, CHARGE_IC_AUX, &aux_ibus))
			return -1;
		hwlog_info("check multi current, main ibus=%d, aux_ibus=%d\n",
			main_ibus, aux_ibus);

		if ((main_ibus > WLDC_OPEN_PATH_IOUT_MIN) &&
			(aux_ibus > WLDC_OPEN_PATH_IOUT_MIN))
			return 0;
		if (wldc_msleep(500)) /* adc cycle 500ms */
			return -1;
		count--;
	}

	return -1;
}

static void wldc_ic_switch_to_multipath(struct wldc_dev_info *di)
{
	int type;

	wlps_control(WLPS_SC_SW2, WLPS_CTRL_ON);
	usleep_range(9500, 10500); /* 10ms for power stability */

	di->ic_data.ibus_para.sw_cnt = 0;

	if (di->ic_data.cur_type == CHARGE_IC_AUX)
		type = CHARGE_IC_MAIN;
	else
		type = CHARGE_IC_AUX;

	if (wldc_ic_enable_multi_ic(di, type, DC_IC_ENABLE)) {
		wlps_control(WLPS_SC_SW2, WLPS_CTRL_OFF);
		di->ic_data.multi_ic_err_cnt++;
		return;
	}

	if (wldc_msleep(250)) { /* need 200+ms to wait for sc open */
		wlps_control(WLPS_SC_SW2, WLPS_CTRL_OFF);
		return;
	}
	if (wldc_ic_check_multi_ic_current(di)) {
		wlps_control(WLPS_SC_SW2, WLPS_CTRL_OFF);
		wldc_ic_enable_multi_ic(di, type, DC_IC_DISABLE);
		multi_ic_check_set_ic_error_flag(type, &di->ic_mode_para);
		return;
	}

	di->ic_data.cur_type = CHARGE_MULTI_IC;
	di->ic_check_info.multi_ic_start_time = current_kernel_time().tv_sec;
	hwlog_info("switch to multi path succ\n");
}

static bool wldc_ic_is_need_multipath(struct wldc_dev_info *di)
{
	int ibus = 0;
	int count;

	if (di->ic_data.cur_type == CHARGE_MULTI_IC)
		return false;

	if (dc_get_ic_ibus(SC_MODE, CHARGE_IC_MAIN, &ibus))
		return false;

	if (ibus < di->ic_data.ibus_para.hth) {
		di->ic_data.ibus_para.hth_cnt = 0;
		return false;
	}

	if (di->wldc_stage == WLDC_STAGE_STOP_CHARGING)
		return false;

	di->ic_data.ibus_para.hth_cnt++;
	if (!di->ctrl_interval)
		return false;
	count = di->ic_data.ibus_para.h_time / di->ctrl_interval;
	if (di->ic_data.ibus_para.hth_cnt < count)
		return false;

	if (multi_ic_check_ic_status(&di->ic_mode_para))
		return false;

	return true;
}

static void wldc_ic_switch_to_singlepath(struct wldc_dev_info *di)
{
	if (dc_ic_enable(SC_MODE, CHARGE_IC_AUX, DC_IC_DISABLE)) {
		di->stop_flag_error = true;
		return;
	}

	(void)dc_ic_discharge(SC_MODE, CHARGE_IC_AUX, DC_IC_ENABLE);
	(void)dc_ic_discharge(SC_MODE, CHARGE_IC_AUX, DC_IC_DISABLE);

	di->ic_data.cur_type = CHARGE_IC_MAIN;
	di->ic_data.ibus_para.sw_cnt = WLDC_IC_SC_SW2_CNT;
	hwlog_info("switch to single path succ\n");
}

static bool wldc_ic_is_need_singlepath(struct wldc_dev_info *di)
{
	int count;
	int ibus = 0;

	if (di->ic_data.cur_type != CHARGE_MULTI_IC)
		return false;

	if (dc_get_total_ibus(SC_MODE, &ibus))
		return false;

	if (ibus > di->ic_data.ibus_para.lth) {
		di->ic_data.ibus_para.lth_cnt = 0;
		return false;
	}

	if (di->wldc_stage == WLDC_STAGE_STOP_CHARGING)
		return false;

	di->ic_data.ibus_para.lth_cnt++;
	if (!di->ctrl_interval)
		return false;
	count = di->ic_data.ibus_para.l_time / di->ctrl_interval;
	if (di->ic_data.ibus_para.lth_cnt < count)
		return false;

	return true;
}

void wldc_ic_gpio_sc_sw2_check(struct wldc_dev_info *di)
{
	if (di->ic_data.ibus_para.sw_cnt) {
		di->ic_data.ibus_para.sw_cnt--;
		if (di->ic_data.ibus_para.sw_cnt)
			return;
		wlps_control(WLPS_SC_SW2, WLPS_CTRL_OFF);
	}
}

void wldc_ic_select_charge_path(struct wldc_dev_info *di)
{
	if (!di || !di->ic_mode_para.support_multi_ic ||
		(di->ic_data.multi_ic_err_cnt > WLDC_IC_ERR_CNT))
		return;

	if (di->wldc_stage == WLDC_STAGE_STOP_CHARGING)
		return;

	if (wldc_ic_is_need_multipath(di))
		wldc_ic_switch_to_multipath(di);

	if (wldc_ic_is_need_singlepath(di))
		wldc_ic_switch_to_singlepath(di);

	wldc_ic_gpio_sc_sw2_check(di);
}

void wldc_ic_para_reset(struct wldc_dev_info *di)
{
	if (!di || !di->ic_mode_para.support_multi_ic)
		return;

	di->ic_data.multi_ic_err_cnt = 0;
	di->ic_data.ibus_para.sw_cnt = 0;
	di->ic_data.ibus_para.hth_cnt = 0;
	di->ic_data.ibus_para.lth_cnt = 0;
	di->ic_data.cur_type = CHARGE_IC_MAIN;
	di->ic_check_info.limit_current = WLDC_DFT_MAX_CP_IOUT;
	memset(di->ic_check_info.report_info, 0,
		sizeof(di->ic_check_info.report_info));
	memset(di->ic_check_info.ibus_error_num, 0,
		sizeof(di->ic_check_info.ibus_error_num));
	memset(di->ic_check_info.vbat_error_num, 0,
		sizeof(di->ic_check_info.vbat_error_num));
	memset(di->ic_check_info.tbat_error_num, 0,
		sizeof(di->ic_check_info.tbat_error_num));
	memset(di->ic_mode_para.ic_error_cnt, 0,
		sizeof(di->ic_mode_para.ic_error_cnt));
}
