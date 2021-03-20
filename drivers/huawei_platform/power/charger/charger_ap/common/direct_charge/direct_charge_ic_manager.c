/*
 * direct_charge_ic_manager.c
 *
 * direct charge ic management interface
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

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/math64.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/power_common.h>
#include <huawei_platform/power/direct_charger/direct_charge_ic_manager.h>

#define HWLOG_TAG direct_charge_ic_manager
HWLOG_REGIST();

static struct direct_charge_batinfo_ops *g_sc_batinfo_ops[CHARGE_IC_MAX_NUM];
static struct direct_charge_batinfo_ops *g_lvc_batinfo_ops[CHARGE_IC_MAX_NUM];
static struct direct_charge_ic_ops *g_loadswitch_ops[CHARGE_IC_MAX_NUM];
static struct direct_charge_ic_ops *g_switchcap_ops[CHARGE_IC_MAX_NUM];
static long g_c_offset_a;
static long g_c_offset_b;

static int __init early_parse_direct_charge_cmdline(char *p)
{
	char *token = NULL;

	if (!p) {
		hwlog_err("cmdline is null\n");
		return -1;
	}

	token = strsep(&p, ",");
	if (token) {
		if (kstrtoul(token, POWER_BASE_DEC, &g_c_offset_a) != 0)
			return -1;
	}

	token = strsep(&p, ",");
	if (token) {
		if (kstrtoul(token, POWER_BASE_DEC, &g_c_offset_b) != 0)
			return -1;
	}

	if ((g_c_offset_a < C_OFFSET_A_MIN) || (g_c_offset_a > C_OFFSET_A_MAX)) {
		g_c_offset_a = 0;
		hwlog_err("c_offset_a invalid\n");
		return -1;
	}

	return 0;
}

early_param("direct_charger", early_parse_direct_charge_cmdline);

static struct direct_charge_ic_ops *dc_get_ic_ops(int mode,
	unsigned int type)
{
	if (type >= CHARGE_IC_TYPE_MAX)
		return NULL;

	switch (mode) {
	case LVC_MODE:
		return g_loadswitch_ops[type];
	case SC_MODE:
		return g_switchcap_ops[type];
	default:
		return NULL;
	}
}

static struct direct_charge_batinfo_ops *dc_get_battinfo_ops(int mode,
	unsigned int type)
{
	if (type >= CHARGE_IC_TYPE_MAX)
		return NULL;

	switch (mode) {
	case LVC_MODE:
		return g_lvc_batinfo_ops[type];
	case SC_MODE:
		return g_sc_batinfo_ops[type];
	default:
		return NULL;
	}
}

static inline int dc_check_ic_type(int type)
{
	if (!(type & CHARGE_MULTI_IC))
		return -1;

	return 0;
}

int dc_sc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops)
{
	if (!ops || type >= CHARGE_IC_TYPE_MAX)
		return -1;

	g_switchcap_ops[type] = ops;
	hwlog_info("type %u sc register ok\n", type);

	return 0;
}

int dc_lvc_ops_register(unsigned int type, struct direct_charge_ic_ops *ops)
{
	if (!ops || type >= CHARGE_IC_TYPE_MAX)
		return -1;

	g_loadswitch_ops[type] = ops;
	hwlog_info("type %u lvc register ok\n", type);

	return 0;
}

int dc_batinfo_ops_register(int mode, unsigned int type, struct direct_charge_batinfo_ops *ops)
{
	if (!ops || type >= CHARGE_IC_TYPE_MAX)
		return -1;

	switch (mode) {
	case LVC_MODE:
		g_lvc_batinfo_ops[type] = ops;
		break;
	case SC_MODE:
		g_sc_batinfo_ops[type] = ops;
		break;
	default:
		hwlog_info("mode is error\n");
		return -1;
	}

	hwlog_info("mode %d, type %u batinfo register ok\n", mode, type);
	return 0;
}

int dc_ic_init(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	hwlog_info("ic init type %d\n", type);

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_init)
			ret = temp_ops->ic_init();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_init)
			ret |= temp_ops->ic_init();
	}

	return ret;
}

int dc_ic_exit(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	hwlog_info("ic exit type %d\n", type);

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_exit)
			ret = temp_ops->ic_exit();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_exit)
			ret |= temp_ops->ic_exit();
	}

	return ret;
}

int dc_ic_enable(int mode, int type, int enable)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	hwlog_info("ic enable type %d, enable %d\n", type, enable);

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_enable)
			ret = temp_ops->ic_enable(enable);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_enable)
			ret |= temp_ops->ic_enable(enable);
	}

	return ret;
}

int dc_ic_adc_enable(int mode, int type, int enable)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_adc_enable)
			ret = temp_ops->ic_adc_enable(enable);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_adc_enable)
			ret |= temp_ops->ic_adc_enable(enable);
	}

	hwlog_info("ic adc enable type %d, enable %d\n", type, enable);
	return ret;
}

int dc_ic_discharge(int mode, int type, int enable)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_discharge)
			ret = temp_ops->ic_discharge(enable);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_discharge)
			ret |= temp_ops->ic_discharge(enable);
	}

	return ret;
}

int dc_is_ic_close(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->is_ic_close)
			ret = temp_ops->is_ic_close();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->is_ic_close)
			ret |= temp_ops->is_ic_close();
	}

	return ret;
}

bool dc_is_ic_support_prepare(int mode, int type)
{
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return false;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_enable_prepare)
			return true;
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_enable_prepare)
			return true;
	}

	return false;
}

int dc_ic_enable_prepare(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->ic_enable_prepare)
			ret = temp_ops->ic_enable_prepare();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->ic_enable_prepare)
			ret |= temp_ops->ic_enable_prepare();
	}

	return ret;
}

int dc_config_ic_watchdog(int mode, int type, int time)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->config_ic_watchdog)
			ret = temp_ops->config_ic_watchdog(time);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->config_ic_watchdog)
			ret |= temp_ops->config_ic_watchdog(time);
	}

	return ret;
}

int dc_kick_ic_watchdog(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->kick_ic_watchdog)
			ret = temp_ops->kick_ic_watchdog();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->kick_ic_watchdog)
			ret |= temp_ops->kick_ic_watchdog();
	}

	return ret;
}

int dc_get_ic_id(int mode, int type)
{
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_id)
			return temp_ops->get_ic_id();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_id)
			return temp_ops->get_ic_id();
	}

	return -1;
}

int dc_get_ic_status(int mode, int type)
{
	int ret = 0;
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_status)
			ret = temp_ops->get_ic_status();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_status)
			ret |= temp_ops->get_ic_status();
	}

	return ret;
}

const char *dc_get_ic_name(int mode, int type)
{
	struct direct_charge_ic_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return "invalid ic";

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->dev_name)
			return temp_ops->dev_name;
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_ic_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->dev_name)
			return temp_ops->dev_name;
	}

	return "invalid ic";
}

int dc_batinfo_init(int mode, int type)
{
	int ret = 0;
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->init)
			ret = temp_ops->init();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->init)
			ret |= temp_ops->init();
	}

	return ret;
}

int dc_batinfo_exit(int mode, int type)
{
	int ret = 0;
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->exit)
			ret = temp_ops->exit();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->exit)
			ret |= temp_ops->exit();
	}

	return ret;
}

int dc_get_bat_btb_voltage(int mode, int type)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_bat_btb_voltage)
			return temp_ops->get_bat_btb_voltage();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_bat_btb_voltage)
			return temp_ops->get_bat_btb_voltage();
	}

	return -1;
}

int dc_get_bat_btb_voltage_with_comp(int mode, int type, const int *vbat_comp)
{
	int vbat;
	int comp;

	if (!vbat_comp)
		return -1;

	/* vbat_comp[0] : main ic comp, vbat_comp[1] : aux ic comp */
	switch (type) {
	case CHARGE_MULTI_IC:
	case CHARGE_IC_MAIN:
		comp = vbat_comp[0];
		break;
	case CHARGE_IC_AUX:
		comp = vbat_comp[1];
		break;
	default:
		comp = 0;
		break;
	}

	vbat = dc_get_bat_btb_voltage(mode, type);
	if (vbat < 0)
		return -1;
	else if (vbat == 0)
		return vbat;

	hwlog_info("vbat = %d, vbat_with_comp = %d\n", vbat, vbat + comp);
	return vbat + comp;
}

int dc_get_bat_package_voltage(int mode, int type)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_bat_package_voltage)
			return temp_ops->get_bat_package_voltage();
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_bat_package_voltage)
			return temp_ops->get_bat_package_voltage();
	}

	return -1;
}

int dc_get_vbus_voltage(int mode, int type, int *vbus)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!vbus)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_vbus_voltage)
			return temp_ops->get_vbus_voltage(vbus);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_vbus_voltage)
			return temp_ops->get_vbus_voltage(vbus);
	}

	return -1;
}

int dc_get_bat_current(int mode, int type, int *ibat)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!ibat)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_bat_current)
			return temp_ops->get_bat_current(ibat);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_bat_current)
			return temp_ops->get_bat_current(ibat);
	}

	return -1;
}

int dc_get_bat_current_with_comp(int mode, int type, int *ibat)
{
	s64 temp;

	if (dc_get_bat_current(mode, type, ibat))
		return -1;

	hwlog_info("cali_b: bat_cur=%d\n", *ibat);
	if (g_c_offset_a) {
		temp = (*ibat * (s64)g_c_offset_a) + g_c_offset_b;
		*ibat = (int)div_s64(temp, 1000000); /* base 1000000 */
		hwlog_info("cali_a: bat_cur=%d,c_offset_a=%ld,c_offset_b=%ld\n",
			*ibat, g_c_offset_a, g_c_offset_b);
	}
	return 0;
}

int dc_get_ic_ibus(int mode, int type, int *ibus)
{
	int main_ibus = 0;
	int aux_ibus = 0;
	int ic_count = 0;
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!ibus)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_ibus &&
			!temp_ops->get_ic_ibus(&main_ibus))
			ic_count++;
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_ibus &&
			!temp_ops->get_ic_ibus(&aux_ibus))
			ic_count++;
	}

	if (!ic_count)
		return -1;

	*ibus = main_ibus + aux_ibus;
	if (type == CHARGE_MULTI_IC)
		hwlog_info("main_ibus = %d, aux_ibus = %d\n", main_ibus, aux_ibus);

	return 0;
}

int dc_get_ic_temp(int mode, int type, int *temp)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!temp)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_temp)
			return temp_ops->get_ic_temp(temp);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_temp)
			return temp_ops->get_ic_temp(temp);
	}

	return 0;
}

int dc_get_ic_vusb(int mode, int type, int *vusb)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!vusb)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_vusb)
			return temp_ops->get_ic_vusb(vusb);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_vusb)
			return temp_ops->get_ic_vusb(vusb);
	}

	return -1;
}

int dc_get_ic_vout(int mode, int type, int *vout)
{
	struct direct_charge_batinfo_ops *temp_ops = NULL;

	if (!vout)
		return -1;

	if (dc_check_ic_type(type))
		return -1;

	if (type & CHARGE_IC_MAIN) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_MAIN);
		if (temp_ops && temp_ops->get_ic_vout)
			return temp_ops->get_ic_vout(vout);
	}
	if (type & CHARGE_IC_AUX) {
		temp_ops = dc_get_battinfo_ops(mode, CHARGE_IC_TYPE_AUX);
		if (temp_ops && temp_ops->get_ic_vout)
			return temp_ops->get_ic_vout(vout);
	}

	return -1;
}

int dc_get_ic_max_temp(int mode, int *temp)
{
	int temp1 = 0;
	int temp2 = 0;
	int ret1;
	int ret2;

	if (!temp)
		return -1;

	ret1 = dc_get_ic_temp(mode, CHARGE_IC_MAIN, &temp1);
	ret2 = dc_get_ic_temp(mode, CHARGE_IC_AUX, &temp2);

	*temp = (temp1 > temp2 ? temp1 : temp2);

	return (ret1 && ret2);
}

int dc_get_total_ibus(int mode, int *ibus)
{
	int main_ibus = 0;
	int aux_ibus = 0;
	int ic_count = 0;

	if (!ibus)
		return -1;

	if (!dc_get_ic_ibus(mode, CHARGE_IC_MAIN, &main_ibus))
		ic_count++;
	if (!dc_get_ic_ibus(mode, CHARGE_IC_AUX, &aux_ibus))
		ic_count++;

	hwlog_info("main ibus %d, aux ibus %d\n", main_ibus, aux_ibus);

	if (!ic_count)
		return -1;

	*ibus = main_ibus + aux_ibus;

	return 0;
}

int dc_get_total_bat_current(int mode, int *ibat)
{
	int main_ibat = 0;
	int aux_ibat = 0;
	int ic_count = 0;

	if (!ibat)
		return -1;

	if (!dc_get_bat_current(mode, CHARGE_IC_MAIN, &main_ibat))
		ic_count++;
	if (!dc_get_bat_current(mode, CHARGE_IC_AUX, &aux_ibat))
		ic_count++;

	hwlog_info("main ibat %d, aux ibat %d\n", main_ibat, aux_ibat);

	if (!ic_count)
		return -1;

	*ibat = main_ibat + aux_ibat;

	return 0;
}

int dc_get_bat_btb_avg_voltage(int mode, int *vbat)
{
	int main_vbat;
	int aux_vbat;
	int ic_count = 0;

	if (!vbat)
		return -1;

	main_vbat = dc_get_bat_btb_voltage(mode, CHARGE_IC_MAIN);
	if (main_vbat > 0)
		ic_count++;

	aux_vbat = dc_get_bat_btb_voltage(mode, CHARGE_IC_AUX);
	if (aux_vbat > 0)
		ic_count++;

	if (!ic_count)
		return -1;

	*vbat = (main_vbat + aux_vbat) / ic_count;

	return 0;
}

int dc_get_bat_package_avg_voltage(int mode, int *vbat)
{
	int main_vbat;
	int aux_vbat;
	int ic_count = 0;

	if (!vbat)
		return -1;

	main_vbat = dc_get_bat_package_voltage(mode, CHARGE_IC_MAIN);
	if (main_vbat > 0)
		ic_count++;

	aux_vbat = dc_get_bat_package_voltage(mode, CHARGE_IC_AUX);
	if (aux_vbat > 0)
		ic_count++;

	if (!ic_count)
		return -1;

	*vbat = (main_vbat + aux_vbat) / ic_count;

	return 0;
}

int dc_get_vbus_avg_voltage(int mode, int *vbus)
{
	int main_vbus = 0;
	int aux_vbus = 0;
	int ic_count = 0;

	if (!vbus)
		return -1;

	if (!dc_get_vbus_voltage(mode, CHARGE_IC_MAIN, &main_vbus))
		ic_count++;
	if (!dc_get_vbus_voltage(mode, CHARGE_IC_AUX, &aux_vbus))
		ic_count++;

	if (!ic_count)
		return -1;

	*vbus = (main_vbus + aux_vbus) / ic_count;

	return 0;
}
