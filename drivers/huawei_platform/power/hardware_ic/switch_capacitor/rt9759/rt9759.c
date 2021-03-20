/*
 * rt9759.c
 *
 * rt9759 driver
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/raid/pq.h>
#include <chipset_common/hwpower/power_i2c.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/direct_charger/direct_charger.h>
#include "rt9759.h"
#include <chipset_common/hwpower/power_thermalzone.h>
#include <linux/hisi/hisi_adc.h>
#include <chipset_common/hwpower/power_algorithm.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG rt9759
HWLOG_REGIST();

static struct rt9759_device_info *g_rt9759_dev;

static int g_get_id_time = RT9759_NOT_USED;
static int g_init_finish_flag = RT9759_NOT_INIT;
static int g_int_notify_enable_flag = RT9759_DISABLE_INT_NOTIFY;

static int rt9759_write_byte(u8 reg, u8 value)
{
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!di || di->chip_already_init == 0) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_write_byte(di->client, reg, value);
}

static int rt9759_read_byte(u8 reg, u8 *value)
{
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!di || di->chip_already_init == 0) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_read_byte(di->client, reg, value);
}

static int rt9759_read_word(u8 reg, s16 *value)
{
	struct rt9759_device_info *di = g_rt9759_dev;
	u16 data = 0;

	if (!di || di->chip_already_init == 0) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	if (power_i2c_u8_read_word(di->client, reg, &data, true))
		return -1;

	*value = (s16)data;
	return 0;
}

static int rt9759_write_mask(u8 reg, u8 mask, u8 shift, u8 value)
{
	int ret;
	u8 val = 0;

	ret = rt9759_read_byte(reg, &val);
	if (ret < 0)
		return ret;

	val &= ~mask;
	val |= ((value << shift) & mask);

	return rt9759_write_byte(reg, val);
}

static void rt9759_dump_register(void)
{
	u8 i;
	int ret;
	u8 val = 0;

	for (i = 0; i < RT9759_DEGLITCH_REG; ++i) {
		ret = rt9759_read_byte(i, &val);
		if (ret)
			hwlog_err("dump_register read fail\n");

		hwlog_info("reg [%x]=0x%x\n", i, val);
	}
}

static int rt9759_reg_reset(void)
{
	int ret;
	u8 reg = 0;

	ret = rt9759_write_mask(RT9759_CONTROL_REG,
		RT9759_REG_RST_MASK, RT9759_REG_RST_SHIFT,
		RT9759_REG_RST_ENABLE);
	if (ret)
		return -1;

	ret = rt9759_read_byte(RT9759_CONTROL_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("reg_reset [%x]=0x%x\n", RT9759_CONTROL_REG, reg);
	return 0;
}

static int rt9759_fault_clear(void)
{
	int ret;
	u8 reg = 0;

	ret = rt9759_read_byte(RT9759_FLT_FLAG_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("fault_flag [%x]=0x%x\n", RT9759_FLT_FLAG_REG, reg);
	return 0;
}

static int rt9759_charge_enable(int enable)
{
	int ret;
	u8 reg = 0;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9759_write_mask(RT9759_CHRG_CTL_REG,
		RT9759_CHARGE_EN_MASK, RT9759_CHARGE_EN_SHIFT,
		value);
	if (ret)
		return -1;

	ret = rt9759_read_byte(RT9759_CHRG_CTL_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("charge_enable [%x]=0x%x\n", RT9759_CHRG_CTL_REG, reg);
	return 0;
}

static int rt9759_adc_enable(int enable)
{
	int ret;
	u8 reg = 0;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9759_write_mask(RT9759_ADC_CTRL_REG,
		RT9759_ADC_CTRL_EN_MASK, RT9759_ADC_CTRL_EN_SHIFT,
		value);
	if (ret)
		return -1;

	ret = rt9759_read_byte(RT9759_ADC_CTRL_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("adc_enable [%x]=0x%x\n", RT9759_ADC_CTRL_REG, reg);
	return 0;
}

static int rt9759_discharge(int enable)
{
	int ret;
	u8 reg = 0;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9759_write_mask(RT9759_BUS_OVP_REG,
		RT9759_VBUS_PD_EN_MASK, RT9759_VBUS_PD_EN_SHIFT, value);
	if (ret)
		return -1;

	ret = rt9759_read_byte(RT9759_BUS_OVP_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("discharge [%x]=0x%x\n", RT9759_BUS_OVP_REG, reg);
	return 0;
}

static int rt9759_is_device_close(void)
{
	u8 reg = 0;
	int ret;

	ret = rt9759_read_byte(RT9759_CHRG_CTL_REG, &reg);
	if (ret)
		return 1;

	if (reg & RT9759_CHARGE_EN_MASK)
		return 0;

	return 1;
}

static bool rt9759_is_adc_disabled(void)
{
	int ret;
	u8 reg = 0;

	ret = rt9759_read_byte(RT9759_ADC_CTRL_REG, &reg);
	if (ret || !(reg & RT9759_ADC_CTRL_EN_MASK))
		return true;

	return false;
}

static int rt9759_get_device_id(void)
{
	u8 part_info = 0;
	int ret;
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	if (g_get_id_time == RT9759_USED)
		return di->device_id;

	g_get_id_time = RT9759_USED;
	ret = rt9759_read_byte(RT9759_PART_INFO_REG, &part_info);
	if (ret) {
		g_get_id_time = RT9759_NOT_USED;
		hwlog_err("get_device_id read fail\n");
		return -1;
	}
	hwlog_info("get_device_id [%x]=0x%x\n",
		RT9759_PART_INFO_REG, part_info);

	part_info = part_info & RT9759_DEVICE_ID_MASK;
	switch (part_info) {
	case RT9759_DEVICE_ID_RT9759:
		di->device_id = SWITCHCAP_RT9759;
		break;
	default:
		di->device_id = -1;
		hwlog_err("dev_id not match\n");
		break;
	}

	return di->device_id;
}

static int rt9759_get_vbat_mv(void)
{
	s16 data = 0;
	int ret;
	int vbat;
	int vbat2;

	if (rt9759_is_adc_disabled())
		return 0;

	ret = rt9759_read_word(RT9759_VBAT_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("VBAT_ADC=0x%x\n", data);

	vbat = (int)(data);

	ret = rt9759_read_word(RT9759_VBAT_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("VBAT_ADC=0x%x\n", data);

	vbat2 = (int)(data);

	return vbat < vbat2 ? vbat : vbat2;
}

static int rt9759_get_ibat_ma(int *ibat)
{
	int ret;
	s16 data = 0;
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!ibat || !di)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*ibat = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_IBAT_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("IBAT_ADC=0x%x\n", data);

	*ibat = ((int)data) * SENSE_R_2_MOHM;
	*ibat /= di->sense_r_mohm;

	return 0;
}

static int rt9759_get_ibus_ma(int *ibus)
{
	s16 data = 0;
	int ret;

	if (!ibus)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*ibus = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_IBUS_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("IBUS_ADC=0x%x\n", data);

	*ibus = (int)(data);

	return 0;
}

static int rt9759_get_vbus_mv(int *vbus)
{
	int ret;
	s16 data = 0;

	if (!vbus)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*vbus = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_VBUS_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("VBUS_ADC=0x%x\n", data);

	*vbus = (int)(data);

	return 0;
}

static int rt9759_is_tsbat_disabled(void)
{
	u8 reg = 0;
	int ret;

	ret = rt9759_read_byte(RT9759_CHRG_CTL_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("is_tsbat_disabled [%x]=0x%x\n", RT9759_CHRG_CTL_REG, reg);

	if (reg & RT9759_TSBAT_DIS_MASK)
		return 0;

	return -1;
}

static int rt9759_get_device_temp(int *temp)
{
	s16 data = 0;
	s16 temperature;
	int ret;

	if (!temp)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*temp = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_TDIE_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("TDIE_ADC=0x%x\n", data);

	temperature = data & ((RT9759_TDIE_ADC1_MASK <<
		RT9759_LENTH_OF_BYTE) | RT9759_LOW_BYTE_INIT);
	*temp = (int)(temperature / RT9759_TDIE_SCALE);

	return 0;
}

static int rt9759_get_vusb_mv(int *vusb)
{
	int ret;
	s16 data = 0;

	if (!vusb)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*vusb = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_VAC_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("VAC_ADC=0x%x\n", data);

	*vusb = (int)(data);

	return 0;
}

static int rt9759_get_vout_mv(int *vout)
{
	int ret;
	s16 data = 0;

	if (!vout)
		return -1;

	if (rt9759_is_adc_disabled()) {
		*vout = 0;
		return 0;
	}

	ret = rt9759_read_word(RT9759_VOUT_ADC1_REG, &data);
	if (ret)
		return -1;

	hwlog_info("VOUT_ADC=0x%x\n", data);

	*vout = (int)(data);

	return 0;
}

static int rt9759_get_register_head(char *buffer, int size, void *dev_data)
{
	if (!buffer)
		return -1;

	snprintf(buffer, size,
		"   Ibus1  Vbus1  Ibat1  Vusb1  Vout1  Vbat1  Temp1");
	return 0;
}

static int rt9759_value_dump(char *buffer, int size, void *dev_data)
{
	int ibus = 0;
	int vbus = 0;
	int ibat = 0;
	int vusb = 0;
	int vout = 0;
	int temp = 0;

	if (!buffer)
		return -1;

	rt9759_get_ibus_ma(&ibus);
	rt9759_get_vbus_mv(&vbus);
	rt9759_get_ibat_ma(&ibat);
	rt9759_get_vusb_mv(&vusb);
	rt9759_get_vout_mv(&vout);
	rt9759_get_device_temp(&temp);

	snprintf(buffer, size,
		"%-7d%-7d%-7d%-7d%-7d%-7d%-7d  ",
		ibus, vbus, ibat, vusb, vout, rt9759_get_vbat_mv(), temp);
	return 0;
}

static int rt9759_config_watchdog_ms(int time)
{
	u8 val;
	u8 reg;
	int ret;

	if (time >= RT9759_WTD_CONFIG_TIMING_30000MS)
		val = RT9759_WTD_SET_30000MS;
	else if (time >= RT9759_WTD_CONFIG_TIMING_5000MS)
		val = RT9759_WTD_SET_30000MS;
	else if (time >= RT9759_WTD_CONFIG_TIMING_1000MS)
		val = RT9759_WTD_SET_30000MS;
	else
		val = RT9759_WTD_SET_30000MS;

	ret = rt9759_write_mask(RT9759_CONTROL_REG,
		RT9759_WATCHDOG_CONFIG_MASK, RT9759_WATCHDOG_CONFIG_SHIFT,
		val);
	if (ret)
		return -1;

	ret = rt9759_read_byte(RT9759_CONTROL_REG, &reg);
	if (ret)
		return -1;

	hwlog_info("config_watchdog_ms [%x]=0x%x\n", RT9759_CONTROL_REG, reg);

	return 0;
}

static int rt9759_config_vbat_ovp_threshold_mv(int ovp_threshold)
{
	u8 value;
	int ret;

	if (ovp_threshold < RT9759_BAT_OVP_BASE_3500MV)
		ovp_threshold = RT9759_BAT_OVP_BASE_3500MV;

	if (ovp_threshold > RT9759_BAT_OVP_MAX_5075MV)
		ovp_threshold = RT9759_BAT_OVP_MAX_5075MV;

	value = (u8)((ovp_threshold - RT9759_BAT_OVP_BASE_3500MV) /
		RT9759_BAT_OVP_STEP);
	ret = rt9759_write_mask(RT9759_BAT_OVP_REG,
		RT9759_BAT_OVP_MASK, RT9759_BAT_OVP_SHIFT,
		value);
	if (ret)
		return -1;

	hwlog_info("config_vbat_ovp_threshold_mv [%x]=0x%x\n",
		RT9759_BAT_OVP_REG, value);

	return 0;
}

static int rt9759_config_ibat_ocp_threshold_ma(int ocp_threshold)
{
	u8 value;
	int ret;

	if (ocp_threshold < RT9759_BAT_OCP_BASE_2000MA)
		ocp_threshold = RT9759_BAT_OCP_BASE_2000MA;

	if (ocp_threshold > RT9759_BAT_OCP_MAX_10000MA)
		ocp_threshold = RT9759_BAT_OCP_MAX_10000MA;

	value = (u8)((ocp_threshold - RT9759_BAT_OCP_BASE_2000MA) /
		RT9759_BAT_OCP_STEP);
	ret = rt9759_write_mask(RT9759_BAT_OCP_REG,
		RT9759_BAT_OCP_MASK, RT9759_BAT_OCP_SHIFT,
		value);
	if (ret)
		return -1;

	hwlog_info("config_ibat_ocp_threshold_ma [%x]=0x%x\n",
		RT9759_BAT_OCP_REG, value);

	return 0;
}

static int rt9759_config_ac_ovp_threshold_mv(int ovp_threshold)
{
	u8 value;
	int ret;

	if (ovp_threshold < RT9759_AC_OVP_BASE_11000MV)
		ovp_threshold = RT9759_AC_OVP_BASE_11000MV;

	if (ovp_threshold > RT9759_AC_OVP_MAX_18000MV)
		ovp_threshold = RT9759_AC_OVP_MAX_18000MV;

	value = (u8)((ovp_threshold - RT9759_AC_OVP_BASE_11000MV) /
		RT9759_AC_OVP_STEP);
	ret = rt9759_write_mask(RT9759_AC_OVP_REG,
		RT9759_AC_OVP_MASK, RT9759_AC_OVP_SHIFT,
		value);
	if (ret)
		return -1;

	hwlog_info("config_ac_ovp_threshold_mv [%x]=0x%x\n",
		RT9759_AC_OVP_REG, value);

	return 0;
}

static int rt9759_config_vbus_ovp_threshold_mv(int ovp_threshold)
{
	u8 value;
	int ret;

	if (ovp_threshold < RT9759_BUS_OVP_BASE_6000MV)
		ovp_threshold = RT9759_BUS_OVP_BASE_6000MV;

	if (ovp_threshold > RT9759_BUS_OVP_MAX_12350MV)
		ovp_threshold = RT9759_BUS_OVP_MAX_12350MV;

	value = (u8)((ovp_threshold - RT9759_BUS_OVP_BASE_6000MV) /
		RT9759_BUS_OVP_STEP);
	ret = rt9759_write_mask(RT9759_BUS_OVP_REG,
		RT9759_BUS_OVP_MASK, RT9759_BUS_OVP_SHIFT,
		value);
	if (ret)
		return -1;

	hwlog_info("config_vbus_ovp_threshold_mv [%x]=0x%x\n",
		RT9759_BUS_OVP_REG, value);

	return 0;
}

static int rt9759_config_ibus_ocp_threshold_ma(int ocp_threshold)
{
	u8 value;
	int ret;

	if (ocp_threshold < RT9759_BUS_OCP_BASE_1000MA)
		ocp_threshold = RT9759_BUS_OCP_BASE_1000MA;

	if (ocp_threshold > RT9759_BUS_OCP_MAX_4750MA)
		ocp_threshold = RT9759_BUS_OCP_MAX_4750MA;

	value = (u8)((ocp_threshold - RT9759_BUS_OCP_BASE_1000MA) /
		RT9759_BUS_OCP_STEP);
	ret = rt9759_write_mask(RT9759_BUS_OCP_UCP_REG,
		RT9759_BUS_OCP_MASK, RT9759_BUS_OCP_SHIFT,
		value);
	if (ret)
		return -1;

	hwlog_info("config_ibus_ocp_threshold_ma [%x]=0x%x\n",
		RT9759_BUS_OCP_UCP_REG, value);

	return 0;
}

static int rt9759_config_switching_frequency(int data)
{
	int freq;
	int freq_shift;
	int ret;

	switch (data) {
	case RT9759_SW_FREQ_450KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_500KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_M_P10;
		break;
	case RT9759_SW_FREQ_500KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_500KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_NORMAL;
		break;
	case RT9759_SW_FREQ_550KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_500KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_P_P10;
		break;
	case RT9759_SW_FREQ_675KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_750KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_M_P10;
		break;
	case RT9759_SW_FREQ_750KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_750KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_NORMAL;
		break;
	case RT9759_SW_FREQ_825KHZ:
		freq = RT9759_FSW_SET_SW_FREQ_750KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_P_P10;
		break;
	default:
		freq = RT9759_FSW_SET_SW_FREQ_500KHZ;
		freq_shift = RT9759_SW_FREQ_SHIFT_P_P10;
		break;
	}

	ret = rt9759_write_mask(RT9759_CONTROL_REG,
		RT9759_FSW_SET_MASK, RT9759_FSW_SET_SHIFT,
		freq);
	if (ret)
		return -1;

	ret = rt9759_write_mask(RT9759_CHRG_CTL_REG,
		RT9759_FREQ_SHIFT_MASK, RT9759_FREQ_SHIFT_SHIFT,
		freq_shift);
	if (ret)
		return -1;

	hwlog_info("config_switching_frequency [%x]=0x%x\n",
		RT9759_CONTROL_REG, freq);
	hwlog_info("config_switching_frequency [%x]=0x%x\n",
		RT9759_CHRG_CTL_REG, freq_shift);

	return 0;
}

static int rt9759_reg_init(void)
{
	int ret;

	ret = rt9759_write_byte(RT9759_CONTROL_REG,
		RT9759_CONTROL_REG_INIT);
	ret |= rt9759_write_byte(RT9759_CHRG_CTL_REG,
		g_rt9759_dev->chrg_ctl_reg_init);
	ret |= rt9759_write_byte(RT9759_INT_MASK_REG,
		RT9759_INT_MASK_REG_INIT);
	ret |= rt9759_write_byte(RT9759_FLT_MASK_REG,
		RT9759_FLT_MASK_REG_INIT);
	ret |= rt9759_write_byte(RT9759_ADC_FN_DIS_REG,
		g_rt9759_dev->adc_fn_reg_init);
	ret |= rt9759_write_mask(RT9759_BAT_OVP_ALM_REG,
		RT9759_BAT_OVP_ALM_DIS_MASK, RT9759_BAT_OVP_ALM_DIS_SHIFT,
		RT9759_ALM_DISABLE);
	ret |= rt9759_write_mask(RT9759_BAT_OCP_ALM_REG,
		RT9759_BAT_OCP_ALM_DIS_MASK, RT9759_BAT_OCP_ALM_DIS_SHIFT,
		RT9759_ALM_DISABLE);
	ret |= rt9759_write_mask(RT9759_BAT_UCP_ALM_REG,
		RT9759_BAT_UCP_ALM_DIS_MASK, RT9759_BAT_UCP_ALM_DIS_SHIFT,
		RT9759_ALM_DISABLE);
	ret |= rt9759_write_mask(RT9759_BUS_OVP_REG,
		RT9759_VBUS_PD_EN_MASK, RT9759_VBUS_PD_EN_SHIFT,
		RT9759_VBUS_PD_EN_DISABLE);
	ret |= rt9759_write_mask(RT9759_BUS_OVP_ALM_REG,
		RT9759_BUS_OVP_ALM_DIS_MASK, RT9759_BUS_OVP_ALM_DIS_SHIFT,
		RT9759_ALM_DISABLE);
	ret |= rt9759_write_mask(RT9759_BUS_OCP_ALM_REG,
		RT9759_BUS_OCP_ALM_DIS_MASK, RT9759_BUS_OCP_ALM_DIS_SHIFT,
		RT9759_ALM_DISABLE);
	ret |= rt9759_config_vbat_ovp_threshold_mv(
		RT9759_VBAT_OVP_THRESHOLD_INIT);
	ret |= rt9759_config_ibat_ocp_threshold_ma(
		RT9759_IBAT_OCP_THRESHOLD_INIT);
	ret |= rt9759_config_ac_ovp_threshold_mv(
		RT9759_AC_OVP_THRESHOLD_INIT);
	ret |= rt9759_config_vbus_ovp_threshold_mv(
		RT9759_VBUS_OVP_THRESHOLD_INIT);
	ret |= rt9759_config_ibus_ocp_threshold_ma(
		RT9759_IBUS_OCP_THRESHOLD_INIT);
	ret |= rt9759_config_switching_frequency(
		g_rt9759_dev->switching_frequency);
	if (ret) {
		hwlog_err("reg_init fail\n");
		return -1;
	}

	return 0;
}

static int rt9759_charge_init(void)
{
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	if (rt9759_reg_init())
		return -1;

	di->device_id = rt9759_get_device_id();
	if (di->device_id == -1)
		return -1;

	hwlog_info("device id is %d\n", di->device_id);

	g_init_finish_flag = RT9759_INIT_FINISH;
	return 0;
}

static int rt9759_charge_exit(void)
{
	int ret;
	struct rt9759_device_info *di = g_rt9759_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	ret = rt9759_charge_enable(RT9759_SWITCHCAP_DISABLE);

	g_init_finish_flag = RT9759_NOT_INIT;
	g_int_notify_enable_flag = RT9759_DISABLE_INT_NOTIFY;

	usleep_range(10000, 11000); /* sleep 10ms */

	return ret;
}

static int rt9759_batinfo_exit(void)
{
	return 0;
}

static int rt9759_batinfo_init(void)
{
	return 0;
}

static void rt9759_fault_event_notify(unsigned long event, void *data)
{
	struct atomic_notifier_head *fault_notifier_list = NULL;

	sc_get_fault_notifier(&fault_notifier_list);
	atomic_notifier_call_chain(fault_notifier_list, event, data);
}

static void rt9759_fault_handle(struct nty_data *data)
{
	int val = 0;
	u8 fault_flag = data->event1;
	u8 ac_protection = data->event2;
	u8 converter_state = data->event3;

	if (ac_protection & RT9759_AC_OVP_FLAG_MASK) {
		hwlog_info("AC OVP happened\n");
		rt9759_fault_event_notify(DC_FAULT_AC_OVP, data);
	} else if (fault_flag & RT9759_BAT_OVP_FLT_FLAG_MASK) {
		val = rt9759_get_vbat_mv();
		hwlog_info("BAT OVP happened, vbat=%d mv\n", val);
		if (val >= RT9759_VBAT_OVP_THRESHOLD_INIT)
			rt9759_fault_event_notify(DC_FAULT_VBAT_OVP, data);
	} else if (fault_flag & RT9759_BAT_OCP_FLT_FLAG_MASK) {
		rt9759_get_ibat_ma(&val);
		hwlog_info("BAT OCP happened, ibat=%d ma\n", val);
		if (val >= RT9759_IBAT_OCP_THRESHOLD_INIT)
			rt9759_fault_event_notify(DC_FAULT_IBAT_OCP, data);
	} else if (fault_flag & RT9759_BUS_OVP_FLT_FLAG_MASK) {
		rt9759_get_vbus_mv(&val);
		hwlog_info("BUS OVP happened, vbus=%d mv\n", val);
		if (val >= RT9759_VBUS_OVP_THRESHOLD_INIT)
			rt9759_fault_event_notify(DC_FAULT_VBUS_OVP, data);
	} else if (fault_flag & RT9759_BUS_OCP_FLT_FLAG_MASK) {
		rt9759_get_ibus_ma(&val);
		hwlog_info("BUS OCP happened, ibus=%d ma\n", val);
		if (val >= RT9759_IBUS_OCP_THRESHOLD_INIT)
			rt9759_fault_event_notify(DC_FAULT_IBUS_OCP, data);
	} else if (fault_flag & RT9759_TSBAT_FLT_FLAG_MASK) {
		hwlog_info("BAT TEMP OTP happened\n");
		rt9759_fault_event_notify(DC_FAULT_TSBAT_OTP, data);
	} else if (fault_flag & RT9759_TSBUS_FLT_FLAG_MASK) {
		hwlog_info("BUS TEMP OTP happened\n");
		rt9759_fault_event_notify(DC_FAULT_TSBUS_OTP, data);
	} else if (fault_flag & RT9759_TDIE_ALM_FLAG_MASK) {
		hwlog_info("DIE TEMP OTP happened\n");
	}

	if (converter_state & RT9759_CONV_OCP_FLAG_MASK) {
		hwlog_info("CONV OCP happened\n");
		rt9759_fault_event_notify(DC_FAULT_CONV_OCP, data);
	}
}

static void rt9759_interrupt_work(struct work_struct *work)
{
	struct rt9759_device_info *di = NULL;
	struct nty_data *data = NULL;
	u8 converter_state = 0;
	u8 fault_flag = 0;
	u8 ac_protection = 0;
	u8 ibus_ucp = 0;
	int ret;

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct rt9759_device_info, irq_work);
	if (!di || !di->client) {
		hwlog_err("di is null\n");
		return;
	}

	data = &(di->nty_data);

	ret = rt9759_read_byte(RT9759_AC_OVP_REG, &ac_protection);
	ret |= rt9759_read_byte(RT9759_BUS_OCP_UCP_REG, &ibus_ucp);
	ret |= rt9759_read_byte(RT9759_FLT_FLAG_REG, &fault_flag);
	ret |= rt9759_read_byte(RT9759_CONVERTER_STATE_REG, &converter_state);
	if (ret)
		hwlog_err("irq_work read fail\n");

	data->event1 = fault_flag;
	data->event2 = ac_protection;
	data->event3 = converter_state;
	data->addr = di->client->addr;

	if (g_int_notify_enable_flag == RT9759_ENABLE_INT_NOTIFY) {
		rt9759_fault_handle(data);
		rt9759_dump_register();
	}

	hwlog_info("ac_ovp_reg [%x]=0x%x\n", RT9759_AC_OVP_REG, ac_protection);
	hwlog_info("bus_ocp_ucp_reg [%x]=0x%x\n", RT9759_BUS_OCP_UCP_REG, ibus_ucp);
	hwlog_info("flt_flag_reg [%x]=0x%x\n", RT9759_FLT_FLAG_REG, fault_flag);
	hwlog_info("converter_state_reg [%x]=0x%x\n", RT9759_CONVERTER_STATE_REG, converter_state);

	/* clear irq */
	enable_irq(di->irq_int);
}

static irqreturn_t rt9759_interrupt(int irq, void *_di)
{
	struct rt9759_device_info *di = _di;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	if (di->chip_already_init == 0)
		hwlog_err("chip not init\n");

	if (g_init_finish_flag == RT9759_INIT_FINISH)
		g_int_notify_enable_flag = RT9759_ENABLE_INT_NOTIFY;

	hwlog_info("int happened\n");
	disable_irq_nosync(di->irq_int);
	schedule_work(&di->irq_work);
	return IRQ_HANDLED;
}

static int rt9759_irq_init(struct rt9759_device_info *di, struct device_node *np)
{
	int ret;

	ret = power_gpio_config_interrupt(np,
		"gpio_int", "rt9759_gpio_int", &di->gpio_int, &di->irq_int);
	if (ret)
		return ret;

	ret = request_irq(di->irq_int, rt9759_interrupt,
		IRQF_TRIGGER_FALLING, "rt9759_int_irq", di);
	if (ret) {
		hwlog_err("gpio irq request fail\n");
		di->irq_int = -1;
		gpio_free(di->gpio_int);
		return ret;
	}

	enable_irq_wake(di->irq_int);
	INIT_WORK(&di->irq_work, rt9759_interrupt_work);
	return 0;
}

static int rt9759_reg_reset_and_init(void)
{
	int ret;

	ret = rt9759_reg_reset();
	if (ret) {
		hwlog_err("reg reset fail\n");
		return ret;
	}

	ret = rt9759_fault_clear();
	if (ret) {
		hwlog_err("fault clear fail\n");
		return ret;
	}

	ret = rt9759_reg_init();
	if (ret) {
		hwlog_err("reg init fail\n");
		return ret;
	}

	return 0;
}

static int rt9759_get_raw_data(int adc_channel, long *data, void *dev_data)
{
	int adc_value;
	struct rt9759_device_info *di = g_rt9759_dev;
	struct adc_comp_data comp_data = { 0 };

	if (!di || !data)
		return -1;

	adc_value = hisi_adc_get_adc(adc_channel);
	if (adc_value < 0)
		return -1;

	comp_data.adc_accuracy = di->adc_accuracy;
	comp_data.adc_v_ref = di->adc_v_ref;
	comp_data.v_pullup = di->v_pullup;
	comp_data.r_pullup = di->r_pullup;
	comp_data.r_comp = di->r_comp;

	*data = (long)power_get_adc_compensation_value(adc_value, &comp_data);
	if (*data < 0)
		return -1;

	return 0;
}
static void rt9759_parse_dts(struct device_node *np,
	struct rt9759_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tsbus_high_r_kohm", &di->tsbus_high_r_kohm,
		RT9759_RESISTORS_100KOHM);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tsbus_low_r_kohm", &di->tsbus_low_r_kohm,
		RT9759_RESISTORS_100KOHM);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"switching_frequency", &di->switching_frequency,
		RT9759_SW_FREQ_500KHZ);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ic_role", &di->ic_role, CHARGE_IC_TYPE_MAIN);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"sense_r_mohm", &di->sense_r_mohm, SENSE_R_2_MOHM);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"chrg_ctl_reg_init", &di->chrg_ctl_reg_init,
		RT9759_CHRG_CTL_REG_INIT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"adc_fn_reg_init", &di->adc_fn_reg_init,
		RT9759_ADC_FN_DIS_REG_INIT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"adc_accuracy", &di->adc_accuracy, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"adc_v_ref", &di->adc_v_ref, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"v_pullup", &di->v_pullup, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"r_pullup", &di->r_pullup, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"r_comp", &di->r_comp, 0);
}

static struct direct_charge_ic_ops rt9759_sysinfo_ops = {
	.dev_name = "rt9759",
	.ic_init = rt9759_charge_init,
	.ic_exit = rt9759_charge_exit,
	.ic_enable = rt9759_charge_enable,
	.ic_adc_enable = rt9759_adc_enable,
	.ic_discharge = rt9759_discharge,
	.is_ic_close = rt9759_is_device_close,
	.get_ic_id = rt9759_get_device_id,
	.config_ic_watchdog = rt9759_config_watchdog_ms,
	.get_ic_status = rt9759_is_tsbat_disabled,
};

static struct direct_charge_batinfo_ops rt9759_batinfo_ops = {
	.init = rt9759_batinfo_init,
	.exit = rt9759_batinfo_exit,
	.get_bat_btb_voltage = rt9759_get_vbat_mv,
	.get_bat_package_voltage = rt9759_get_vbat_mv,
	.get_vbus_voltage = rt9759_get_vbus_mv,
	.get_bat_current = rt9759_get_ibat_ma,
	.get_ic_ibus = rt9759_get_ibus_ma,
	.get_ic_temp = rt9759_get_device_temp,
	.get_ic_vusb = rt9759_get_vusb_mv,
	.get_ic_vout = rt9759_get_vout_mv,
};

static struct power_log_ops rt9759_log_ops = {
	.dev_name = "rt9759",
	.dump_log_head = rt9759_get_register_head,
	.dump_log_content = rt9759_value_dump,
};

static struct power_tz_ops rt9759_temp_sensing_ops = {
	.get_raw_data = rt9759_get_raw_data,
};

static int rt9759_ops_register(struct rt9759_device_info *di)
{
	int ret;

	ret = dc_sc_ops_register(di->ic_role, &rt9759_sysinfo_ops);
	if (ret) {
		hwlog_err("sysinfo ops register fail\n");
		return ret;
	}

	ret = dc_batinfo_ops_register(SC_MODE, di->ic_role, &rt9759_batinfo_ops);
	if (ret) {
		hwlog_err("batinfo ops register fail\n");
		return ret;
	}

	rt9759_temp_sensing_ops.dev_data = (void *)di;
	ret = power_tz_ops_register(&rt9759_temp_sensing_ops, "bq25970");
	if (ret)
		hwlog_err("thermalzone ops register fail\n");

	rt9759_log_ops.dev_data = (void *)di;
	ret = power_log_ops_register(&rt9759_log_ops);
	if (ret)
		hwlog_err("power log register fail\n");

	return 0;
}

static int rt9759_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct rt9759_device_info *di = NULL;
	struct device_node *np = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_rt9759_dev = di;
	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;
	i2c_set_clientdata(client, di);

	di->chip_already_init = 1;

	ret = rt9759_get_device_id();
	if (ret < 0)
		goto rt9759_fail_0;

	rt9759_parse_dts(np, di);

	ret = rt9759_irq_init(di, np);
	if (ret)
		goto rt9759_fail_0;

	ret = rt9759_ops_register(di);
	if (ret)
		goto rt9759_fail_1;

	ret = rt9759_reg_reset_and_init();
	if (ret)
		goto rt9759_fail_1;

	ret = rt9759_write_byte(RT9759_ADC_CTRL_REG,
		RT9759_ADC_CTRL_REG_INIT);
	if (ret)
		goto rt9759_fail_1;

	return 0;

rt9759_fail_1:
	free_irq(di->irq_int, di);
	gpio_free(di->gpio_int);
rt9759_fail_0:
	di->chip_already_init = 0;
	g_rt9759_dev = NULL;
	devm_kfree(&client->dev, di);

	return ret;
}

static int rt9759_remove(struct i2c_client *client)
{
	struct rt9759_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	if (di->irq_int)
		free_irq(di->irq_int, di);

	if (di->gpio_int)
		gpio_free(di->gpio_int);

	return 0;
}

static void rt9759_shutdown(struct i2c_client *client)
{
	rt9759_reg_reset();
}

MODULE_DEVICE_TABLE(i2c, rt9759);
static const struct of_device_id rt9759_of_match[] = {
	{
		.compatible = "bq25970",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id rt9759_i2c_id[] = {
	{ "bq25970", 0 },
	{}
};

static struct i2c_driver rt9759_driver = {
	.probe = rt9759_probe,
	.remove = rt9759_remove,
	.shutdown = rt9759_shutdown,
	.id_table = rt9759_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "rt9759",
		.of_match_table = of_match_ptr(rt9759_of_match),
	},
};

static int __init rt9759_init(void)
{
	return i2c_add_driver(&rt9759_driver);
}

static void __exit rt9759_exit(void)
{
	i2c_del_driver(&rt9759_driver);
}

module_init(rt9759_init);
module_exit(rt9759_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("rt9759 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
