/*
 * bq25892_charger.c
 *
 * bq25892 driver
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include <linux/pm_wakeup.h>
#include <linux/usb/otg.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/power_supply.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/raid/pq.h>
#include <linux/hisi/hisi_adc.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <huawei_platform/log/hw_log.h>
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <huawei_platform/devdetect/hw_dev_dec.h>
#endif
#include <huawei_platform/power/huawei_charger.h>
#ifdef CONFIG_HISI_BCI_BATTERY
#include <linux/power/hisi/hisi_bci_battery.h>
#endif
#include <chipset_common/hwpower/power_i2c.h>
#include "bq25892_charger.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG bq25892_charger
HWLOG_REGIST();

struct bq25892_device_info *g_bq25892_dev;

/* configured in dts based on the real value of the iin limit resistance */
static unsigned int rilim = BQ25892_RILIM_124_OHM;
/* configured in dts based on the real adc channel number */
static unsigned int adc_channel_iin = BQ25892_ADC_CHANNEL_IIN_10;
static unsigned int adc_channel_vbat_sys = BQ25892_ADC_CHANNEL_VBAT_SYS;
static u32 is_board_type; /* 0:sft 1:udp 2:asic */
static int ico_current_mode;

static int iin_set = IINLIM_MIN_100;

#define BUF_LEN                      26

static int bq25892_write_byte(u8 reg, u8 value)
{
	struct bq25892_device_info *di = g_bq25892_dev;

	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_write_byte(di->client, reg, value);
}

static int bq25892_read_byte(u8 reg, u8 *value)
{
	struct bq25892_device_info *di = g_bq25892_dev;

	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_read_byte(di->client, reg, value);
}

static int bq25892_write_mask(u8 reg, u8 mask, u8 shift, u8 value)
{
	int ret;
	u8 val = 0;

	ret = bq25892_read_byte(reg, &val);
	if (ret < 0)
		return ret;

	val &= ~mask;
	val |= ((value << shift) & mask);

	return bq25892_write_byte(reg, val);
}

static int bq25892_read_mask(u8 reg, u8 mask, u8 shift, u8 *value)
{
	int ret;
	u8 val = 0;

	ret = bq25892_read_byte(reg, &val);
	if (ret < 0)
		return ret;

	val &= mask;
	val >>= shift;
	*value = val;

	return 0;
}

static int bq25892_device_check(void)
{
	int ret;
	u8 reg = 0xff;

	ret = bq25892_read_byte(BQ25892_REG_14, &reg);
	if (ret)
		return CHARGE_IC_BAD;

	hwlog_info("device_check [%x]=0x%x\n", BQ25892_REG_14, reg);

	if ((reg & CHIP_VERSION_MASK) == BQ25892 &&
		(reg & CHIP_REVISION_MASK) == CHIP_REVISION) {
		hwlog_info("bq25892 is good\n");
		return CHARGE_IC_GOOD;
	}

	hwlog_err("bq25892 is bad\n");
	return CHARGE_IC_BAD;
}

static int bq25892_set_bat_comp(int value)
{
	int bat_comp;
	u8 bat_comp_reg;

	bat_comp = value;

	if (bat_comp < BAT_COMP_MIN_0) {
		hwlog_err("set bat_comp %d, out of range:%d\n",
			value, BAT_COMP_MIN_0);
		bat_comp = BAT_COMP_MIN_0;
	} else if (bat_comp > BAT_COMP_MAX_140) {
		hwlog_err("set bat_comp %d, out of range:%d\n",
			value, BAT_COMP_MAX_140);
		bat_comp = BAT_COMP_MAX_140;
	}

	bat_comp_reg = (bat_comp - BAT_COMP_MIN_0) / BAT_COMP_STEP_20;

	hwlog_info("set_bat_comp [%x]=0x%x\n", BQ25892_REG_08, bat_comp_reg);

	return bq25892_write_mask(BQ25892_REG_08,
		BQ25892_REG_08_BAT_COMP_MASK,
		BQ25892_REG_08_BAT_COMP_SHIFT,
		bat_comp_reg);
}

static int bq25892_set_vclamp(int value)
{
	int vclamp;
	u8 vclamp_reg;

	vclamp = value;

	if (vclamp < VCLAMP_MIN_0) {
		hwlog_err("set vclamp %d, out of range:%d\n",
			value, VCLAMP_MIN_0);
		vclamp = VCLAMP_MIN_0;
	} else if (vclamp > VCLAMP_MAX_224) {
		hwlog_err("set vclamp %d, out of range:%d\n",
			value, VCLAMP_MAX_224);
		vclamp = VCLAMP_MAX_224;
	}

	vclamp_reg = (vclamp - VCLAMP_MIN_0) / VCLAMP_STEP_32;

	hwlog_info("set_vclamp [%x]=0x%x\n", BQ25892_REG_08, vclamp_reg);

	return bq25892_write_mask(BQ25892_REG_08,
		BQ25892_REG_08_VCLAMP_MASK,
		BQ25892_REG_08_VCLAMP_SHIFT,
		vclamp_reg);
}

static int bq25892_set_covn_start(int enable)
{
	int ret;
	u8 reg = 0;
	int i;

	ret = bq25892_read_byte(BQ25892_REG_0B, &reg);
	if (ret)
		return -1;

	if (!(reg & BQ25892_NOT_PG_STAT)) {
		hwlog_err("bq25892 PG NOT GOOD, can not set covn,reg:%x\n",
			reg);
		return -1;
	}

	ret = bq25892_write_mask(BQ25892_REG_02,
		BQ25892_REG_02_CONV_START_MASK,
		BQ25892_REG_02_CONV_START_SHIFT,
		enable);
	if (ret)
		return -1;

	/* The conversion result is ready after tCONV, max (10*100)ms */
	for (i = 0; i < 10; i++) {
		ret = bq25892_read_mask(BQ25892_REG_02,
			BQ25892_REG_02_CONV_START_MASK,
			BQ25892_REG_02_CONV_START_SHIFT,
			&reg);
		if (ret)
			return -1;

		/* if ADC Conversion finished, CONV_RATE bit will be 0 */
		if (reg == 0)
			break;

		msleep(100); /* sleep 100ms */
	}

	hwlog_info("one-shot covn start is set %d\n", enable);
	return 0;
}

static int bq25892_5v_chip_init(struct bq25892_device_info *di)
{
	int ret = 0;

	/* enable Start 1s Continuous Conversion, others as default */
	ret |= bq25892_write_byte(BQ25892_REG_02, 0x1D); /* adc off */

	/* WD_RST 1,CHG_CONFIG 0,SYS_MIN 3.5 */
	ret |= bq25892_write_byte(BQ25892_REG_03, 0x4A);

	/* Fast Charge Current Limit 2048mA */
	ret |= bq25892_write_byte(BQ25892_REG_04, 0x20);

	/* Precharge Current Limit 256mA,Termination Current Limit 256mA */
	ret |= bq25892_write_byte(BQ25892_REG_05, 0x33);

	/*
	 * Charge Voltage Limit 4.4,
	 * Battery Precharge to Fast Charge Threshold 3, Battery Recharge 100mV
	 */
	ret |= bq25892_write_byte(BQ25892_REG_06, 0x8e);

	/*
	 * EN_TERM 0, Watchdog Timer 80s, EN_TIMER 1, Charge Timer 20h,
	 * JEITA Low Temperature Current Setting 1
	 */
	ret |= bq25892_write_byte(BQ25892_REG_07, 0x2f);

	/*
	 * IR compensation voatge clamp = 224mV,
	 * IR compensation resistor setting = 80mohm
	 */
	ret |= bq25892_set_bat_comp(di->param_dts.bat_comp);
	ret |= bq25892_set_vclamp(di->param_dts.vclamp);

	/* boost mode current limit = 500mA,boostv 4.998v */
	ret |= bq25892_write_byte(BQ25892_REG_0A, 0x70);

	/* enable charging */
	gpio_set_value(di->gpio_cd, 0);

	return ret;
}

static int bq25892_set_adc_conv_rate(int mode)
{
	if (mode)
		mode = 1;
	hwlog_info("adc conversion rate mode is set to %d\n", mode);

	return bq25892_write_mask(BQ25892_REG_02,
		BQ25892_REG_02_CONV_RATE_MASK,
		BQ25892_REG_02_CONV_RATE_SHIFT,
		mode);
}

/*
 * ONLY TI bq25892 charge chip has a bug.
 * When charger is in charging done state and under PFM mode,
 * there is risk to slowly drive Q4 to open when unplug charger.
 * The result is VSYS drops to 2.0V and reach milliseconds interval.
 * The VSYS drop can be captured by hi6421 that result in SMPL.
 * The solution that TI suggest: when charger chip is bq25892,
 * and under charging done state, set IINLIM(reg00 bit0-bit5) to 400mA or
 * below to force chargeIC to exit PFM
 */
static int bq25892_exit_pfm_when_cd(unsigned int limit_default)
{
	u8 reg0b = 0;
	u8 reg14 = 0;
	int ret = 0;

	ret |= bq25892_read_byte(BQ25892_REG_0B, &reg0b);
	ret |= bq25892_read_byte(BQ25892_REG_14, &reg14);
	if (ret)
		return limit_default;

	if (((reg14 & BQ25892_REG_14_REG_PN_MASK) ==
		BQ25892_REG_14_REG_PN_IS_25892) && /* bq25892 */
		(limit_default > IINLIM_FOR_BQ25892_EXIT_PFM)) {
		if ((reg0b & BQ25892_REG_0B_CHRG_STAT_MASK) ==
			BQ25892_CHGR_STAT_CHARGE_DONE) { /* charging done */
			limit_default = IINLIM_FOR_BQ25892_EXIT_PFM;
			hwlog_err("limit input current to %dmA to force exit PFM\n",
				limit_default);
		}
	}

	return limit_default;
}

static int bq25892_set_input_current(int value)
{
	int limit_current;
	u8 iin_limit;

	limit_current = value;

	if (limit_current < IINLIM_MIN_100) {
		hwlog_err("set input_current %d, out of range:%d\n",
			value, IINLIM_MIN_100);
		limit_current = IINLIM_MIN_100;
	} else if (limit_current > IINLIM_MAX_3250) {
		hwlog_err("set input_current %d, out of range:%d\n",
			value, IINLIM_MAX_3250);
		limit_current = IINLIM_MAX_3250;
	}

	hwlog_info("input current is set %dmA\n", limit_current);

	/*
	 * in order to avoid smpl because bq25892 bug,
	 * set iinlim to 400mA if under charging done and chip ic is bq25892,
	 * otherwise keep it change
	 */
	if (!power_cmdline_is_powerdown_charging_mode())
		limit_current = bq25892_exit_pfm_when_cd(limit_current);

	hwlog_info("input current is set %dmA\n", limit_current);

	iin_limit = (limit_current - IINLIM_MIN_100) / IINLIM_STEP_50;
	iin_set = iin_limit * IINLIM_STEP_50 + IINLIM_MIN_100;

	return bq25892_write_mask(BQ25892_REG_00,
		BQ25892_REG_00_IINLIM_MASK,
		BQ25892_REG_00_IINLIM_SHIFT,
		iin_limit);
}

static int bq25892_get_input_current_set(void)
{
	return iin_set;
}

static int bq25892_set_charge_current(int value)
{
	int currentma;
	u8 ichg;

	currentma = value;

	if (currentma < 0) {
		currentma = 0;
	} else if (currentma > ICHG_MAX_5056) {
		hwlog_err("set charge_current %d, out of range:%d\n",
			value, ICHG_MAX_5056);
		currentma = ICHG_MAX_5056;
	}

	hwlog_info("charge current is set %dmA\n", currentma);

	ichg = currentma / ICHG_STEP_64;

	return bq25892_write_mask(BQ25892_REG_04,
		BQ25892_REG_04_ICHG_MASK,
		BQ25892_REG_04_ICHG_SHIFT,
		ichg);
}

static int bq25892_set_terminal_voltage(int value)
{
	int voltagemv;
	u8 voreg;

	voltagemv = value;

	if (voltagemv < VCHARGE_MIN_3840) {
		hwlog_err("set terminal_voltage %d, out of range:%d\n",
			value, VCHARGE_MIN_3840);
		voltagemv = VCHARGE_MIN_3840;
	} else if (voltagemv > VCHARGE_MAX_4496) {
		hwlog_err("set terminal_voltage %d, out of range:%d\n",
			value, VCHARGE_MAX_4496);
		voltagemv = VCHARGE_MAX_4496;
	}

	hwlog_info("terminal voltage is set %dmV\n", voltagemv);

	voreg = (voltagemv - VCHARGE_MIN_3840) / VCHARGE_STEP_16;

	return bq25892_write_mask(BQ25892_REG_06,
		BQ25892_REG_06_VREG_MASK,
		BQ25892_REG_06_VREG_SHIFT,
		voreg);
}

static int bq25892_set_dpm_voltage(int value)
{
	int vindpm_voltage;
	u8 vindpm;
	u8 reg0d;

	vindpm_voltage = value;

	if (vindpm_voltage > VINDPM_MAX_15300) {
		hwlog_err("set dpm_voltage %d, out of range:%d\n",
			value, VINDPM_MAX_15300);
		vindpm_voltage = VINDPM_MAX_15300;
	} else if (vindpm_voltage < VINDPM_MIN_3900) {
		hwlog_err("set dpm_voltage %d, out of range:%d\n",
			value, VINDPM_MIN_3900);
		vindpm_voltage = VINDPM_MIN_3900;
	}

	hwlog_info("vindpm absolute voltage is set %dmV\n", vindpm_voltage);

	vindpm = (vindpm_voltage - VINDPM_OFFSET_2600) / VINDPM_STEP_100;

	/* 1 absolute dpm */
	reg0d = (1 << BQ25892_REG_0D_FORCE_VINDPM_SHIFT) | vindpm;

	return bq25892_write_byte(BQ25892_REG_0D, reg0d);
}

static int bq25892_set_terminal_current(int value)
{
	int term_currentma;
	u8 iterm_reg;

	term_currentma = value;

	if (term_currentma < ITERM_MIN_64) {
		hwlog_err("set terminal_current %d, out of range:%d\n",
			value, ITERM_MIN_64);
		term_currentma = ITERM_MIN_64;
	} else if (term_currentma > ITERM_MAX_1024) {
		hwlog_err("set terminal_current %d, out of range:%d\n",
			value, ITERM_MAX_1024);
		term_currentma = ITERM_MAX_1024;
	}

	hwlog_info("term current is set %dmA\n", term_currentma);

	iterm_reg = term_currentma / ITERM_STEP_64;

	return bq25892_write_mask(BQ25892_REG_05,
		BQ25892_REG_05_ITERM_MASK,
		BQ25892_REG_05_ITERM_SHIFT,
		iterm_reg);
}

static int bq25892_set_charge_enable(int enable)
{
	struct bq25892_device_info *di = g_bq25892_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	gpio_set_value(di->gpio_cd, !enable);

	return bq25892_write_mask(BQ25892_REG_03,
		BQ25892_REG_03_CHG_CONFIG_MASK,
		BQ25892_REG_03_CHG_CONFIG_SHIFT,
		enable);
}

static int bq25892_set_otg_enable(int enable)
{
	u8 val;
	struct bq25892_device_info *di = g_bq25892_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	gpio_set_value(di->gpio_cd, !enable);

	if (enable)
		val = 1 << 1; /* 1 left shift bit1 */
	else
		val = 0 << 1; /* 0 left shift bit1 */

	return bq25892_write_mask(BQ25892_REG_03,
		BQ25892_REG_03_CHG_CONFIG_MASK,
		BQ25892_REG_03_CHG_CONFIG_SHIFT,
		val);
}

static int bq25892_set_otg_current(int value)
{
	int temp_currentma;
	u8 reg;

	temp_currentma = value;

	if (temp_currentma < BOOST_LIM_MIN_500 ||
		temp_currentma > BOOST_LIM_MAX_2450)
		hwlog_err("set otg_current %d, out of range\n", value);

	if (temp_currentma < BOOST_LIM_750)
		reg = 0;
	else if (temp_currentma >= BOOST_LIM_750 &&
		temp_currentma < BOOST_LIM_1200)
		reg = 1;
	else if (temp_currentma >= BOOST_LIM_1200 &&
		temp_currentma < BOOST_LIM_1400)
		reg = 2;
	else if (temp_currentma >= BOOST_LIM_1400 &&
		temp_currentma < BOOST_LIM_1650)
		reg = 3;
	else if (temp_currentma >= BOOST_LIM_1650 &&
		temp_currentma < BOOST_LIM_1875)
		reg = 4;
	else if (temp_currentma >= BOOST_LIM_1875 &&
		temp_currentma < BOOST_LIM_2150)
		reg = 5;
	else if (temp_currentma >= BOOST_LIM_2150 &&
		temp_currentma < BOOST_LIM_MAX_2450)
		reg = 6;
	else
		reg = 7;

	hwlog_info("otg current is set %dmA\n", temp_currentma);

	return bq25892_write_mask(BQ25892_REG_0A,
		BQ25892_REG_0A_BOOST_LIM_MASK,
		BQ25892_REG_0A_BOOST_LIM_SHIFT,
		reg);
}

static int bq25892_set_term_enable(int enable)
{
	return bq25892_write_mask(BQ25892_REG_07,
		BQ25892_REG_07_EN_TERM_MASK,
		BQ25892_REG_07_EN_TERM_SHIFT,
		enable);
}

static int bq25892_get_charge_state(unsigned int *state)
{
	u8 reg = 0;
	int ret;

	if (!state)
		return -1;

	ret = bq25892_read_byte(BQ25892_REG_0B, &reg);

	hwlog_info("get_charge_state [%x]=0x%x\n", BQ25892_REG_0B, reg);

	if (!(reg & BQ25892_NOT_PG_STAT))
		*state |= CHAGRE_STATE_NOT_PG;

	if ((reg & BQ25892_CHGR_STAT_CHARGE_DONE) ==
		BQ25892_CHGR_STAT_CHARGE_DONE)
		*state |= CHAGRE_STATE_CHRG_DONE;

	ret |= bq25892_read_byte(BQ25892_REG_0C, &reg);
	ret |= bq25892_read_byte(BQ25892_REG_0C, &reg);

	hwlog_info("get_charge_state [%x]=0x%x\n", BQ25892_REG_0C, reg);

	if ((reg & BQ25892_WATCHDOG_FAULT) == BQ25892_WATCHDOG_FAULT)
		*state |= CHAGRE_STATE_WDT_FAULT;

	if ((reg & BQ25892_POWER_SUPPLY_OVP_MASK) == BQ25892_POWER_SUPPLY_OVP)
		*state |= CHAGRE_STATE_VBUS_OVP;

	if ((reg & BQ25892_BAT_FAULT_OVP) == BQ25892_BAT_FAULT_OVP)
		*state |= CHAGRE_STATE_BATT_OVP;

	if ((reg & BQ25892_NTC_FAULT) > BQ25892_NTC_NORMAL)
		*state |= CHAGRE_STATE_NTC_FAULT;

	if (reg != 0)
		hwlog_err("state is not normal, BQ25892 REG[0x0C] is %x\n",
			reg);

	return ret;
}

static int bq25892_get_vbus_mv(unsigned int *vbus_mv)
{
	u8 reg = 0;
	int ret;

	if (!vbus_mv)
		return -1;

	ret = bq25892_read_byte(BQ25892_REG_11, &reg);

	reg = reg & BQ25892_REG_11_VBUSV_MASK;
	*vbus_mv = (unsigned int)reg * BQ25892_REG_11_VBUSV_STEP_MV +
		BQ25892_REG_11_VBUSV_OFFSET_MV;

	hwlog_info("vbus mv is %dmV\n", *vbus_mv);

	return ret;
}

static int bq25892_reset_watchdog_timer(void)
{
	return bq25892_write_mask(BQ25892_REG_03,
		BQ25892_REG_03_WDT_RESET_MASK,
		BQ25892_REG_03_WDT_RESET_SHIFT,
		0x01);
}

static int bq25892_get_vilim_sample(void)
{
	int i;
	int retry_times = 3; /* retry 3 times */
	int v_sample;

	for (i = 0; i < retry_times; ++i) {
		v_sample = hisi_adc_get_value(adc_channel_iin);
		if (v_sample < 0)
			hwlog_err("adc read channel 10 fail\n");
		else
			break;
	}

	hwlog_info("get vilim_sample=%d\n", v_sample);
	return v_sample;
}

static int bq25892_get_ilim(void)
{
	int i;
	int cnt = 0;
	int v_temp;
	int delay_times = 100; /* 100ms */
	int sample_num = 5; /* use 5 samples to get an average value */
	int sum = 0;
	int kilim = 355; /* based bq25892 spec */

	for (i = 0; i < sample_num; ++i) {
		v_temp = bq25892_get_vilim_sample();
		if (v_temp >= 0) {
			sum += v_temp;
			++cnt;
		} else {
			hwlog_err("bq25892 get v_temp fail\n");
		}

		msleep(delay_times);
	}

	if (cnt > 0) {
		 /* I =(K* Vlim)/(rilim*0.8) new spec from TI */
		return ((10 * sum * kilim) / (8 * (int)(rilim) * cnt));
	}

	hwlog_err("use 0 as default vilim\n");
	return 0;
}

static int bq25892_vbat_sys(void)
{
	int i;
	int retry_times = 3; /* retry 3 times */
	int v_sample;

	for (i = 0; i < retry_times; ++i) {
		v_sample = hisi_adc_get_value(adc_channel_vbat_sys);
		if (v_sample < 0)
			hwlog_err("adc read channel 15 fail\n");
		else
			break;
	}

	hwlog_info("get vbat_sys=%d\n", v_sample);
	return v_sample;
}

static int bq25892_get_vbat_sys(void)
{
	int i;
	int cnt = 0;
	int v_temp;
	int delay_times = 100; /* 100ms */
	int sample_num = 5; /* use 5 samples to get an average value */
	int sum = 0;

	for (i = 0; i < sample_num; ++i) {
		v_temp = bq25892_vbat_sys();
		if (v_temp >= 0) {
			sum += v_temp;
			++cnt;
		} else {
			hwlog_err("bq25892 get v_temp fail\n");
		}

		msleep(delay_times);
	}

	if (cnt > 0)
		return (3 * 1000 * sum / cnt); /* vbat_sys_step is 3*1000 */

	hwlog_err("use 0 as default vvlim\n");
	return 0;
}

static int bq25892_dump_register(char *reg_value, int size, void *dev_data)
{
	u8 reg[BQ25892_REG_TOTAL_NUM] = {0};
	char buff[BUF_LEN] = {0};
	int i;
	int vbus = 0;
	int ret;

	if (!reg_value)
		return 0;

	memset(reg_value, 0, size);
	snprintf(buff, BUF_LEN, "%-8.2d", bq25892_get_ilim());
	strncat(reg_value, buff, strlen(buff));

	ret = bq25892_get_vbus_mv((unsigned int *)&vbus);
	if (ret)
		hwlog_err("bq25892_get_vbus_mv failed\n");

	snprintf(buff, BUF_LEN, "%-8.2d", vbus);
	strncat(reg_value, buff, strlen(buff));

	for (i = 0; i < BQ25892_REG_TOTAL_NUM; i++) {
		ret = bq25892_read_byte(i, &reg[i]);
		if (ret)
			hwlog_err("dump_register read fail\n");

		snprintf(buff, BUF_LEN, "0x%-8x", reg[i]);
		strncat(reg_value, buff, strlen(buff));
	}

	return 0;
}

static int bq25892_get_register_head(char *reg_head, int size, void *dev_data)
{
	char buff[BUF_LEN] = {0};
	int i;

	if (!reg_head)
		return 0;

	memset(reg_head, 0, size);
	snprintf(buff, BUF_LEN, "Ibus    ");
	strncat(reg_head, buff, strlen(buff));

	snprintf(buff, BUF_LEN, "Vbus    ");
	strncat(reg_head, buff, strlen(buff));

	for (i = 0; i < BQ25892_REG_TOTAL_NUM; i++) {
		snprintf(buff, BUF_LEN, "Reg[0x%2x] ", i);
		strncat(reg_head, buff, strlen(buff));
	}

	return 0;
}

static int bq25892_set_batfet_disable(int disable)
{
	return bq25892_write_mask(BQ25892_REG_09,
		BQ25892_REG_09_BATFET_DISABLE_MASK,
		BQ25892_REG_09_BATFET_DISABLE_SHIFT,
		disable);
}

static int bq25892_set_watchdog_timer(int value)
{
	u8 val;
	u8 dog_time = value;

	if (dog_time >= WATCHDOG_TIMER_160_S)
		val = BQ25892_REG_07_WATCHDOG_160;
	else if (dog_time >= WATCHDOG_TIMER_80_S)
		val = BQ25892_REG_07_WATCHDOG_80;
	else if (dog_time >= WATCHDOG_TIMER_40_S)
		val = BQ25892_REG_07_WATCHDOG_40;
	else
		val = BQ25892_REG_07_WATCHDOG_DIS;

	hwlog_info("watch dog timer is %d ,the register value is set %d\n",
		dog_time, val);

	return bq25892_write_mask(BQ25892_REG_07,
		BQ25892_REG_07_WATCHDOG_MASK,
		BQ25892_REG_07_WATCHDOG_SHIFT,
		val);
}

static int bq25892_set_charger_hiz(int enable)
{
	int ret;

	if (enable > 0)
		ret = bq25892_write_mask(BQ25892_REG_00,
			BQ25892_REG_00_EN_HIZ_MASK,
			BQ25892_REG_00_EN_HIZ_SHIFT,
			TRUE);
	else
		ret = bq25892_write_mask(BQ25892_REG_00,
			BQ25892_REG_00_EN_HIZ_MASK,
			BQ25892_REG_00_EN_HIZ_SHIFT,
			FALSE);

	return ret;
}

static int bq25892_get_ico_current(struct ico_output *output)
{
	int ret;
	u8 reg = 0;
	int check_ico_optimized_times = 10;
	int idpm_limit;

	ret = bq25892_read_byte(BQ25892_REG_09, &reg);
	if (ret)
		return ret;

	ret = bq25892_write_mask(BQ25892_REG_09,
		BQ25892_REG_09_FORCE_ICO_MASK,
		BQ25892_REG_09_FORCE_ICO_SHIFT,
		TRUE);
	if (ret)
		return ret;

	while (check_ico_optimized_times--) {
		ret = bq25892_read_byte(BQ25892_REG_14, &reg);
		if (ret)
			return ret;

		if (reg & BQ25892_REG_14_ICO_OPTIMIZED_MASK) {
			ret = bq25892_read_mask(BQ25892_REG_13,
				BQ25892_REG_13_IDPM_LIM_MASK,
				BQ25892_REG_13_IDPM_LIM_SHIFT,
				&reg);
			if (ret)
				return ret;

			idpm_limit = reg * BQ25892_REG_13_IDPM_STEP_MV +
				BQ25892_REG_13_IDPM_OFFSET_MV;
			hwlog_info("ico result: reg is 0x%x, current is %dmA\n",
				reg, idpm_limit);

			switch (ico_current_mode) {
			case 0:
				output->input_current =
				(idpm_limit > (int)(output->input_current) ?
				idpm_limit : (int)(output->input_current));
				output->charge_current =
				(idpm_limit > (int)(output->charge_current) ?
				idpm_limit : (int)(output->input_current));
				break;
			case 1:
				output->input_current = idpm_limit;
				output->charge_current = idpm_limit;
				break;
			default:
				break;
			}

			return ret;
		}

		hwlog_info("bq25892_start_force_ico: msleep 200\n");
		msleep(200);
	}

	return -1;
}

static int bq25892_turn_on_ico(struct ico_input *input,
	struct ico_output *output)
{
	int ret;

	ret = bq25892_set_charge_enable(TRUE);
	if (ret)
		return -1;

	ret = bq25892_set_input_current(input->iin_max);
	if (ret)
		return -1;

	ret = bq25892_set_charge_current(input->ichg_max);
	if (ret)
		return -1;

	ret = bq25892_set_terminal_voltage(input->vterm);
	if (ret)
		return -1;

	ret = bq25892_get_ico_current(output);

	return ret;
}

static int bq25892_9v_chip_init(struct bq25892_device_info *di)
{
	int ret = 0;

	/* enable Start 1s Continuous Conversion, others as default */
	ret |= bq25892_write_byte(BQ25892_REG_02, 0x1D); /* adc off */

	/* WD_RST 1,CHG_CONFIG 0,SYS_MIN 3.5 */
	ret |= bq25892_write_byte(BQ25892_REG_03, 0x4A);

	/* Fast Charge Current Limit 2048mA */
	ret |= bq25892_write_byte(BQ25892_REG_04, 0x20);

	/* Precharge Current Limit 256mA,Termination Current Limit 256mA */
	ret |= bq25892_write_byte(BQ25892_REG_05, 0x33);

	/*
	 * Charge Voltage Limit 4.4,
	 * Battery Precharge to Fast Charge Threshold 3,Battery Recharge 100mV
	 */
	ret |= bq25892_write_byte(BQ25892_REG_06, 0x8e);

	/*
	 * EN_TERM 1, Watchdog Timer 80s, EN_TIMER 1,
	 * Charge Timer 20h,JEITA Low Temperature Current Setting 1
	 */
	ret |= bq25892_write_byte(BQ25892_REG_07, 0x2f);

	/*
	 * IR compensation voatge clamp = 224mV,
	 * IR compensation resistor setting = 80mohm
	 */
	ret |= bq25892_set_bat_comp(di->param_dts.bat_comp);
	ret |= bq25892_set_vclamp(di->param_dts.vclamp);

	/*
	 * FORCE_ICO 0, TMR2X_EN 1, BATFET_DIS 0,
	 * JEITA_VSET 0, BATFET_RST_EN 1
	 */
	ret |= bq25892_write_byte(BQ25892_REG_09, 0x44);

	/* boost mode current limit = 500mA,boostv 4.998v */
	ret |= bq25892_write_byte(BQ25892_REG_0A, 0x70);

	/*
	 * set dpm voltage as 4700mv instead of 7600mv
	 * because chargerIC cannot reset dpm after watchdog time out
	 */
	ret |= bq25892_set_dpm_voltage(4700);

	/* enable charging */
	gpio_set_value(di->gpio_cd, 0);

	return ret;
}

static int bq25892_chip_init(struct chip_init_crit *init_crit)
{
	int ret = -1;
	struct bq25892_device_info *di = g_bq25892_dev;

	if (!di || !init_crit) {
		hwlog_err("di or init_crit is null\n");
		return -ENOMEM;
	}

	switch (init_crit->vbus) {
	case ADAPTER_5V:
		ret = bq25892_5v_chip_init(di);
		break;
	case ADAPTER_9V:
		ret = bq25892_9v_chip_init(di);
		break;
	default:
		hwlog_err("invaid init_crit vbus mode\n");
		break;
	}

	return ret;
}

static int bq25892_check_input_vdpm_state(void)
{
	u8 reg = 0;
	int ret;

	ret = bq25892_read_byte(BQ25892_REG_13, &reg);
	if (ret < 0)
		return FALSE;

	hwlog_info("check_input_vdpm_state [%x]=0x%x\n", BQ25892_REG_13, reg);

	if (reg & BQ25892_REG_13_VDPM_STAT_MASK)
		return TRUE;

	return FALSE;
}

static int bq25892_check_input_idpm_state(void)
{
	u8 reg = 0;
	int ret;

	ret = bq25892_read_byte(BQ25892_REG_13, &reg);
	if (ret < 0)
		return FALSE;

	hwlog_info("check_input_idpm_state [%x]=0x%x\n", BQ25892_REG_13, reg);

	if (reg & BQ25892_REG_13_IDPM_STAT_MASK)
		return TRUE;

	return FALSE;
}

static int bq25892_check_input_dpm_state(void)
{
	u8 reg = 0;
	int ret;

	ret = bq25892_read_byte(BQ25892_REG_13, &reg);
	if (ret < 0)
		return FALSE;

	if ((reg & BQ25892_REG_13_VDPM_STAT_MASK) ||
		(reg & BQ25892_REG_13_IDPM_STAT_MASK))
		return TRUE;

	return FALSE;
}

static int bq25892_stop_charge_config(void)
{
	/*
	 * as vindpm of bq25892 won't be reset after watchdog timer out,
	 * if vindpm is higher than 5v, IC will not supply power with USB/AC
	 */
	return bq25892_set_dpm_voltage(CHARGE_VOLTAGE_4520_MV);
}

static int bq25892_force_set_term_enable(int enable)
{
	return 0;
}

static int bq25892_get_charger_state(void)
{
	return 0;
}

static struct charge_device_ops bq25892_ops = {
	.chip_init = bq25892_chip_init,
	.dev_check = bq25892_device_check,
	.set_adc_conv_rate = bq25892_set_adc_conv_rate,
	.set_input_current = bq25892_set_input_current,
	.set_charge_current = bq25892_set_charge_current,
	.set_terminal_voltage = bq25892_set_terminal_voltage,
	.set_dpm_voltage = bq25892_set_dpm_voltage,
	.set_terminal_current = bq25892_set_terminal_current,
	.set_charge_enable = bq25892_set_charge_enable,
	.set_otg_enable = bq25892_set_otg_enable,
	.set_term_enable = bq25892_set_term_enable,
	.get_charge_state = bq25892_get_charge_state,
	.reset_watchdog_timer = bq25892_reset_watchdog_timer,
	.set_watchdog_timer = bq25892_set_watchdog_timer,
	.set_batfet_disable = bq25892_set_batfet_disable,
	.get_ibus = bq25892_get_ilim,
	.get_vbus = bq25892_get_vbus_mv,
	.get_vbat_sys = bq25892_get_vbat_sys,
	.set_covn_start = bq25892_set_covn_start,
	.set_charger_hiz = bq25892_set_charger_hiz,
	.check_input_dpm_state = bq25892_check_input_dpm_state,
	.check_input_vdpm_state = bq25892_check_input_vdpm_state,
	.check_input_idpm_state = bq25892_check_input_idpm_state,
	.set_otg_current = bq25892_set_otg_current,
	.stop_charge_config = bq25892_stop_charge_config,
	.turn_on_ico = bq25892_turn_on_ico,
	.set_force_term_enable = bq25892_force_set_term_enable,
	.get_charger_state = bq25892_get_charger_state,
	.soft_vbatt_ovp_protect = NULL,
	.rboost_buck_limit = NULL,
	.get_charge_current = NULL,
	.get_iin_set = bq25892_get_input_current_set,
};

static struct power_log_ops bq25892_log_ops = {
	.dev_name = "bq25892",
	.dump_log_head = bq25892_get_register_head,
	.dump_log_content = bq25892_dump_register,
};

static struct charger_otg_device_ops bq25892_otg_ops = {
	.dev_name = "bq25892",
	.otg_set_charger_enable = bq25892_set_charge_enable,
	.otg_set_enable = bq25892_set_otg_enable,
	.otg_set_current = bq25892_set_otg_current,
	.otg_set_watchdog_timer = bq25892_set_watchdog_timer,
	.otg_reset_watchdog_timer = bq25892_reset_watchdog_timer,
};

static void bq25892_irq_work(struct work_struct *work)
{
	struct bq25892_device_info *di = NULL;
	u8 reg = 0;
	u8 reg1 = 0;
	int ret;

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct bq25892_device_info, irq_work);
	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	msleep(100); /* sleep 100ms */

	ret = bq25892_read_byte(BQ25892_REG_0B, &reg1);
	ret |= bq25892_read_byte(BQ25892_REG_0C, &reg);
	ret |= bq25892_read_byte(BQ25892_REG_0C, &reg);
	if (ret)
		hwlog_err("irq_work read fail\n");

	if (reg & BQ25892_REG_0C_BOOST) {
		hwlog_info("CHARGE_FAULT_BOOST_OCP happened\n");
		atomic_notifier_call_chain(&fault_notifier_list,
			CHARGE_FAULT_BOOST_OCP, NULL);
	}

	if (di->irq_active == 0) {
		di->irq_active = 1;
		enable_irq(di->irq_int);
	}
}

static irqreturn_t bq25892_interrupt(int irq, void *_di)
{
	struct bq25892_device_info *di = _di;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	hwlog_info("bq25892 int happened\n");

	if (di->irq_active == 1) {
		di->irq_active = 0;
		disable_irq_nosync(di->irq_int);
		schedule_work(&di->irq_work);
	} else {
		hwlog_info("the irq is not enable,do nothing\n");
	}

	return IRQ_HANDLED;
}

static void bq25892_prase_dts(struct device_node *np,
	struct bq25892_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"bat_comp", &(di->param_dts.bat_comp),
		80); /* default is 80ma */
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"vclamp", &(di->param_dts.vclamp),
		224); /* default is 224mv */
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ico_current_mode", &ico_current_mode, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rilim", &rilim, BQ25892_RILIM_124_OHM);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"adc_channel_iin", &adc_channel_iin,
		BQ25892_ADC_CHANNEL_IIN_10);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"adc_channel_vbat_sys", &adc_channel_vbat_sys,
		BQ25892_ADC_CHANNEL_VBAT_SYS);
	(void)power_dts_read_u32_compatible(power_dts_tag(HWLOG_TAG),
		"huawei,hisi_bci_battery", "battery_board_type",
		&is_board_type, BAT_BOARD_ASIC);
}

static int bq25892_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct bq25892_device_info *di = NULL;
	struct device_node *np = NULL;
	struct power_devices_info_data *power_dev_info = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_bq25892_dev = di;
	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;
	i2c_set_clientdata(client, di);

	INIT_WORK(&di->irq_work, bq25892_irq_work);

	bq25892_prase_dts(np, di);

	/* set gpio to control CD pin to disable/enable bq25892 IC */
	ret = power_gpio_config_output(np,
		"gpio_cd", "charger_cd", &di->gpio_cd, 0);
	if (ret)
		goto bq25892_fail_0;

	ret = power_gpio_config_interrupt(np,
		"gpio_int", "charger_int", &di->gpio_int, &di->irq_int);
	if (ret)
		goto bq25892_fail_1;

	ret = request_irq(di->irq_int, bq25892_interrupt,
		IRQF_TRIGGER_FALLING, "charger_int_irq", di);
	if (ret) {
		hwlog_err("gpio irq request fail\n");
		di->irq_int = -1;
		goto bq25892_fail_2;
	}
	di->irq_active = 1;

	ret = charge_ops_register(&bq25892_ops);
	if (ret) {
		hwlog_err("bq25892 charge ops register fail\n");
		goto bq25892_fail_3;
	}

	ret = charger_otg_ops_register(&bq25892_otg_ops);
	if (ret) {
		hwlog_err("bq25892 charger_otg ops register fail\n");
		goto bq25892_fail_3;
	}

	power_dev_info = power_devices_info_register();
	if (power_dev_info) {
		power_dev_info->dev_name = di->dev->driver->name;
		power_dev_info->dev_id = 0;
		power_dev_info->ver_id = 0;
	}

	bq25892_log_ops.dev_data = (void *)di;
	power_log_ops_register(&bq25892_log_ops);
	return 0;

bq25892_fail_3:
	free_irq(di->irq_int, di);
bq25892_fail_2:
	gpio_free(di->gpio_int);
bq25892_fail_1:
	gpio_free(di->gpio_cd);
bq25892_fail_0:
	g_bq25892_dev = NULL;

	return ret;
}

static int bq25892_remove(struct i2c_client *client)
{
	struct bq25892_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	gpio_set_value(di->gpio_cd, 1);

	if (di->gpio_cd)
		gpio_free(di->gpio_cd);

	if (di->irq_int)
		free_irq(di->irq_int, di);

	if (di->gpio_int)
		gpio_free(di->gpio_int);

	g_bq25892_dev = NULL;

	return 0;
}

MODULE_DEVICE_TABLE(i2c, bq25892);
static const struct of_device_id bq25892_of_match[] = {
	{
		.compatible = "huawei,bq25892_charger",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id bq25892_i2c_id[] = {
	{ "bq25892_charger", 0 }, {}
};

static struct i2c_driver bq25892_driver = {
	.probe = bq25892_probe,
	.remove = bq25892_remove,
	.id_table = bq25892_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "bq25892_charger",
		.of_match_table = of_match_ptr(bq25892_of_match),
	},
};

static int __init bq25892_init(void)
{
	return i2c_add_driver(&bq25892_driver);
}

static void __exit bq25892_exit(void)
{
	i2c_del_driver(&bq25892_driver);
}

module_init(bq25892_init);
module_exit(bq25892_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("bq25892 charger module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
