/*
 * direct_charger_adapter.c
 *
 * adapter operate for direct charger
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

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG direct_charge_adapter
HWLOG_REGIST();

int direct_charge_init_adapter(void)
{
	struct adapter_init_data aid;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	aid.scp_mode_enable = 1;
	aid.vset_boundary = l_di->max_adapter_vset;
	aid.iset_boundary = l_di->max_adapter_iset;
	aid.init_voltage = l_di->init_adapter_vset;
	aid.watchdog_timer = 3; /* 3seconds */

	return adapter_set_init_data(ADAPTER_PROTOCOL_SCP, &aid);
}

int direct_charge_reset_operate(int type)
{
	switch (type) {
	case DC_RESET_ADAPTER:
		hwlog_info("soft reset adapter\n");
		return adapter_soft_reset_slave(ADAPTER_PROTOCOL_SCP);
	case DC_RESET_MASTER:
		hwlog_info("soft reset master\n");
		return adapter_soft_reset_master(ADAPTER_PROTOCOL_SCP);
	default:
		hwlog_info("reset operate invalid\n");
		return -1;
	}
}

int direct_charge_get_adapter_type(void)
{
	int adapter_type = ADAPTER_TYPE_UNKNOWN;

	adapter_get_adp_type(ADAPTER_PROTOCOL_SCP, &adapter_type);

	return adapter_type;
}

int direct_charge_get_adapter_support_mode(void)
{
	int adapter_mode = ADAPTER_SUPPORT_UNDEFINED;

	adapter_get_support_mode(ADAPTER_PROTOCOL_SCP, &adapter_mode);

	return adapter_mode;
}

int direct_charge_get_adapter_port_leakage_current(void)
{
	int leak_current = 0;

	adapter_get_port_leakage_current_flag(ADAPTER_PROTOCOL_SCP,
		&leak_current);

	return leak_current;
}

int direct_charge_get_adapter_voltage(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (adapter_get_output_voltage(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_VAPT, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter output voltage fail\n");
		return -1;
	}

	return 0;
}

int direct_charge_get_adapter_max_voltage(int *value)
{
	return adapter_get_max_voltage(ADAPTER_PROTOCOL_SCP, value);
}

int direct_charge_get_adapter_min_voltage(int *value)
{
	return adapter_get_min_voltage(ADAPTER_PROTOCOL_SCP, value);
}

int direct_charge_get_adapter_current(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (l_di->adaptor_vendor_id == ADAPTER_CHIP_IWATT)
		return direct_charge_get_device_ibus(value);

	if (adapter_get_output_current(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_IAPT, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter output current fail\n");
		return -1;
	}

	return 0;
}

int direct_charge_get_adapter_current_set(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (adapter_get_output_current_set(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_APT_CURR_SET, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter setting current fail\n");
		return -1;
	}

	return 0;
}

/* get the maximum current allowed by adapter at specified voltage */
static int direct_charge_adapter_volt_handler(int val,
	struct direct_charge_adp_cur_para *para)
{
	int i;

	if (!para)
		return 0;

	for (i = 0; i < DC_ADP_CUR_LEVEL; ++i) {
		if ((val > para[i].vol_min) && (val <= para[i].vol_max))
			return para[i].cur_th;
	}

	return 0;
}

static int direct_charge_get_adapter_max_cur_by_config(int val,
	struct direct_charge_device *di)
{
	int adapter_type = direct_charge_get_adapter_type();

	switch (adapter_type) {
	case ADAPTER_TYPE_10V2P25A:
		return direct_charge_adapter_volt_handler(val,
			di->adp_10v2p25a);
	case ADAPTER_TYPE_10V2P25A_CAR:
		return direct_charge_adapter_volt_handler(val,
			di->adp_10v2p25a_car);
	case ADAPTER_TYPE_QTR_A_10V2P25A:
		return direct_charge_adapter_volt_handler(val,
			di->adp_qtr_a_10v2p25a);
	case ADAPTER_TYPE_QTR_C_20V3A:
		return direct_charge_adapter_volt_handler(val,
			di->adp_qtr_c_20v3a);
	case ADAPTER_TYPE_10V4A:
		return direct_charge_adapter_volt_handler(val,
			di->adp_10v4a);
	default:
		break;
	}

	return 0;
}

static int direct_charge_get_adapter_max_cur_by_power_curve(int val,
	struct direct_charge_device *di)
{
	int i;

	for (i = 0; i < DC_ADP_PC_LEVEL; i++) {
		if (val <= di->adp_pwr_curve[i].volt)
			return di->adp_pwr_curve[i].cur;
	}

	return 0;
}

static int direct_charge_get_adapter_max_cur_by_reg(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (adapter_get_max_current(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_APT_MAX_CURR, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter max current fail\n");
		return -1;
	}
	return 0;
}

int direct_charge_get_power_drop_current(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (adapter_get_power_drop_current(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_APT_PWR_DROP_CURR, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter power drop current fail\n");
		return -1;
	}

	return 0;
}

void direct_charge_reset_adapter_power_curve(void *p)
{
	struct direct_charge_device *l_di = (struct direct_charge_device *)p;

	if (!l_di)
		return;

	l_di->adapter_power_curve_flag = false;
	memset(l_di->adp_pwr_curve, 0,
		sizeof(struct direct_charge_adp_pc_para) * DC_ADP_PC_LEVEL);
}

static int direct_charge_get_power_curve_num(int *num)
{
	if (!num)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	return adapter_get_power_curve_num(ADAPTER_PROTOCOL_SCP, num);
}

static int direct_charge_get_power_curve(int *val, unsigned int size)
{
	if (!val)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	return adapter_get_power_curve(ADAPTER_PROTOCOL_SCP, val, size);
}

int direct_charge_get_adapter_power_curve(void)
{
	int i;
	int num = 0;
	int value[DC_ADP_PC_LEVEL * DC_ADP_PC_PARA_SIZE] = { 0 };
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	direct_charge_reset_adapter_power_curve(l_di);
	if (direct_charge_get_power_curve_num(&num))
		return -1;

	if ((num <= 0) || (num > DC_ADP_PC_LEVEL))
		return -1;

	if (direct_charge_get_power_curve(value, num * DC_ADP_PC_PARA_SIZE))
		return -1;

	for (i = 0; i < num; i++) {
		l_di->adp_pwr_curve[i].volt = value[i * DC_ADP_PC_PARA_SIZE];
		l_di->adp_pwr_curve[i].cur = value[i * DC_ADP_PC_PARA_SIZE + 1];
	}

	l_di->adapter_power_curve_flag = true;
	for (i = 0; i < DC_ADP_PC_LEVEL; i++)
		hwlog_info("adp_pwr_curve[%d].volt=%d, adp_pwr_curve[%d].cur=%d\n",
			i, l_di->adp_pwr_curve[i].volt,
			i, l_di->adp_pwr_curve[i].cur);
	return 0;
}

int direct_charge_get_adapter_max_current(int value)
{
	int max_cur = 0;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return 0;

	if (l_di->adapter_power_curve_flag)
		max_cur = direct_charge_get_adapter_max_cur_by_power_curve(value, l_di);
	if (max_cur <= 0)
		max_cur = direct_charge_get_adapter_max_cur_by_config(value, l_di);
	if (max_cur <= 0)
		direct_charge_get_adapter_max_cur_by_reg(&max_cur);

	return max_cur;
}

int direct_charge_get_adapter_temp(int *value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	if (adapter_get_inside_temp(ADAPTER_PROTOCOL_SCP, value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_GET_APT_TEMP, NULL);
		direct_charge_set_stop_charging_flag(1);
		*value = 0;
		hwlog_err("get adapter temp fail\n");
		return -1;
	}

	return 0;
}

int direct_charge_get_protocol_register_state(void)
{
	if (adapter_get_protocol_register_state(ADAPTER_PROTOCOL_SCP)) {
		hwlog_err("adapter protocol not ready\n");
		return -1;
	}

	direct_charge_set_stage_status(DC_STAGE_ADAPTER_DETECT);
	return 0;
}

int direct_charge_set_adapter_voltage(int value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();
	int l_value = value;

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	hwlog_info("set adapter_volt=%d,max_volt=%d\n",
		l_value, l_di->max_adapter_vset);

	if (l_value > l_di->max_adapter_vset) {
		l_value = l_di->max_adapter_vset;
		l_di->adaptor_vset = l_di->max_adapter_vset;
	}

	if (adapter_set_output_voltage(ADAPTER_PROTOCOL_SCP, l_value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_SET_APT_VOLT, NULL);
		direct_charge_set_stop_charging_flag(1);
		hwlog_err("set adapter voltage fail\n");
		return -1;
	}

	return 0;
}

int direct_charge_set_adapter_current(int value)
{
	struct direct_charge_device *l_di = direct_charge_get_di();
	int l_value = value;

	if (!l_di)
		return -1;

	if (direct_charge_get_stop_charging_flag())
		return -1;

	hwlog_info("set adapter_cur=%d,max_cur=%d\n",
		l_value, l_di->max_adapter_iset);

	if (l_value > l_di->max_adapter_iset)
		l_value = l_di->max_adapter_iset;

	if (adapter_set_output_current(ADAPTER_PROTOCOL_SCP, l_value)) {
		direct_charge_fill_eh_buf(l_di->dsm_buff,
			sizeof(l_di->dsm_buff),
			DC_EH_SET_APT_CURR, NULL);
		direct_charge_set_stop_charging_flag(1);
		hwlog_err("set adapter current fail\n");
		return -1;
	}

	return 0;
}

int direct_charge_set_adapter_output_enable(int enable)
{
	int ret, i;
	int retry = 3; /* retry 3 times */
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	hwlog_info("set adapter_output_enable=%d\n", enable);

	for (i = 0; i < retry; i++) {
		ret = adapter_set_output_enable(ADAPTER_PROTOCOL_SCP, enable);
		if (!ret)
			break;
	}

	return ret;
}

void direct_charge_set_adapter_default_param(void)
{
	adapter_set_default_param(ADAPTER_PROTOCOL_SCP);
}

int direct_charge_set_adapter_default(void)
{
	return adapter_set_default_state(ADAPTER_PROTOCOL_SCP);
}

int direct_charge_update_adapter_info(void)
{
	int ret;
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	ret = adapter_get_device_info(ADAPTER_PROTOCOL_SCP);
	if (ret) {
		hwlog_err("get adapter info fail\n");
		return -1;
	}
	adapter_show_device_info(ADAPTER_PROTOCOL_SCP);

	ret = adapter_get_chip_vendor_id(ADAPTER_PROTOCOL_SCP,
		&l_di->adaptor_vendor_id);
	if (ret) {
		hwlog_err("get adapter vendor id fail\n");
		return -1;
	}

	return 0;
}

unsigned int direct_charge_update_adapter_support_mode(void)
{
	unsigned int adp_mode = ADAPTER_SUPPORT_SCP_B_LVC +
		ADAPTER_SUPPORT_SCP_B_SC;

	adapter_update_adapter_support_mode(ADAPTER_PROTOCOL_SCP, &adp_mode);

	return adp_mode;
}

int direct_charge_check_adapter_support_mode(int *mode)
{
	return adapter_detect_adapter_support_mode(ADAPTER_PROTOCOL_SCP, mode);
}

static int direct_charge_wait_adapter_antifake(void)
{
	struct direct_charge_device *l_di = direct_charge_get_di();

	if (!l_di)
		return -1;

	/*
	 * if timeout happend, we assume the serivce is dead,
	 * return hash calculate ok anyway
	 */
	if (!wait_for_completion_timeout(&l_di->dc_af_completion,
		DC_AF_WAIT_CT_TIMEOUT)) {
		hwlog_err("bms_auth service wait timeout\n");
		return 0;
	}

	/*
	 * if not timeout,
	 * return the antifake result base on the hash calc result
	 */
	if (l_di->dc_antifake_result == 0) {
		hwlog_err("bms_auth hash calculate fail\n");
		return -1;
	}

	hwlog_info("bms_auth hash calculate ok\n");
	return 0;
}

int direct_charge_check_adapter_antifake(void)
{
	int ret;
	int max_cur;
	int max_pwr;
	int bat_vol = hw_battery_get_series_num() * BAT_RATED_VOLT;
	struct direct_charge_device *l_di = direct_charge_get_di();
	struct power_event_notify_data n_data;

	if (!l_di)
		return -1;

	if (!l_di->adp_antifake_enable)
		return 0;

	if (direct_charge_is_priority_inversion()) {
		hwlog_info("antifake already checked\n");
		return 0;
	}

	max_cur = direct_charge_get_adapter_max_current(
		bat_vol * l_di->dc_volt_ratio);
	if (max_cur == 0)
		return -1;
	max_pwr = max_cur * BAT_RATED_VOLT * l_di->dc_volt_ratio *
		hw_battery_get_series_num() / 1000; /* unit: 1000mw */
	hwlog_info("max_cur=%d, max_pwr=%d\n", max_cur, max_pwr);
	if (max_pwr <= POWER_TH_IGNORE_ANTIFAKE)
		return 0;

	memset(l_di->dc_af_key, 0x00, DC_AF_KEY_LEN);

	ret = adapter_auth_encrypt_start(ADAPTER_PROTOCOL_SCP,
		l_di->adp_antifake_key_index, l_di->dc_af_key, DC_AF_KEY_LEN);
	if (ret)
		goto fail_check;

	l_di->dc_antifake_result = 0;
	reinit_completion(&l_di->dc_af_completion);

	if (l_di->working_mode == LVC_MODE)
		n_data.event = "AUTH_DCCT=1";
	else
		n_data.event = "AUTH_DCCT=2";

	n_data.event_len = 11; /* length of AUTH_DCCT=2 */
	power_event_notify(POWER_EVENT_NE_AUTH_DC_SC, &n_data);
	ret = direct_charge_wait_adapter_antifake();

fail_check:
	ret += adapter_auth_encrypt_release(ADAPTER_PROTOCOL_SCP);
	return ret;
}
