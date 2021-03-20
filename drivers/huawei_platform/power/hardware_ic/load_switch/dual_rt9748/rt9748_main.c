/*
 * rt9748_main.c
 *
 * rt9748 driver
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
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <huawei_platform/devdetect/hw_dev_dec.h>
#endif
#include "../dual_loadswitch.h"
#include "rt9748.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG rt9748_main
HWLOG_REGIST();

static struct rt9748_device_info *g_rt9748_dev;

static int g_get_id_time;

static int rt9748_write_byte(u8 reg, u8 value)
{
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di || di->chip_already_init == 0) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_write_byte(di->client, reg, value);
}

static int rt9748_read_byte(u8 reg, u8 *value)
{
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di || di->chip_already_init == 0) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	return power_i2c_u8_read_byte(di->client, reg, value);
}

static int rt9748_write_mask(u8 reg, u8 mask, u8 shift, u8 value)
{
	int ret;
	u8 val = 0;

	ret = rt9748_read_byte(reg, &val);
	if (ret < 0)
		return ret;

	val &= ~mask;
	val |= ((value << shift) & mask);

	return rt9748_write_byte(reg, val);
}

static int rt9748_reg_init(void)
{
	int ret;
	u8 val;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		ret = rt9748_write_byte(RT9748_EVENT_1_MASK,
			RT9748_EVENT_1_MASK_INIT);
		ret |= rt9748_write_byte(RT9748_EVENT_2_MASK,
			RT9748_EVENT_2_MASK_INIT);
		ret |= rt9748_write_byte(RT9748_EVENT_1_EN,
			RT9748_EVENT_1_EN_INIT);
		ret |= rt9748_write_byte(RT9748_CONTROL,
			RT9748_CONTROL_INIT);
		ret |= rt9748_write_byte(RT9748_ADC_CTRL,
			RT9748_ADC_CTRL_INIT);
		ret |= rt9748_write_byte(RT9748_SAMPLE_EN,
			RT9748_SAMPLE_EN_INIT);
		ret |= rt9748_write_mask(RT9748_PROT_DLY_OCP,
			RT9748_REG_INIT_MASK, RT9748_REG_INIT_SHIFT,
			RT9748_PROT_DLY_OCP_INIT);
		ret |= rt9748_write_byte(RT9748_REV_CURRENT_SELECT,
			RT9748_REV_CURRENT_SELECT_INIT);
		ret |= rt9748_read_byte(RT9748_REV_CURRENT_SELECT, &val);
		if (ret)
			return -1;

		if (val != RT9748_REV_CURRENT_SELECT_INIT)
			hwlog_err("rt9748 write 0x26 fail\n");

		break;
	case loadswitch_bq25870:
		ret = rt9748_write_byte(BQ25870_EVENT_1_MASK,
			BQ25870_EVENT_1_MASK_INIT);
		ret |= rt9748_write_byte(BQ25870_EVENT_2_MASK,
			BQ25870_EVENT_2_MASK_INIT);
		ret |= rt9748_write_byte(BQ25870_EVENT_1_EN,
			BQ25870_EVENT_1_EN_INIT);
		ret |= rt9748_write_byte(BQ25870_CONTROL,
			BQ25870_CONTROL_INIT);
		ret |= rt9748_write_byte(BQ25870_ADC_CTRL,
			BQ25870_ADC_CTRL_INIT);
		ret |= rt9748_write_byte(BQ25870_SAMPLE_EN,
			BQ25870_SAMPLE_EN_INIT);
		ret |= rt9748_write_mask(BQ25870_PROT_DLY_OCP,
			BQ25870_REG_INIT_MASK, BQ25870_REG_INIT_SHIFT,
			BQ25870_PROT_DLY_OCP_INIT);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_adc_enable(int enable)
{
	int ret;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9748_write_mask(RT9748_ADC_CTRL,
		RT9748_ADC_EN_MASK, RT9748_ADC_EN_SHIFT,
		value);
	if (ret)
		return -1;

	return 0;
}

static int rt9748_charge_enable(int enable)
{
	u8 reg = 0;
	int ret;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9748_write_mask(RT9748_CONTROL,
		RT9748_CHARGE_EN_MASK, RT9748_CHARGE_EN_SHIFT,
		value);
	if (ret)
		return -1;

	ret = rt9748_read_byte(RT9748_CONTROL, &reg);
	if (ret)
		return -1;

	hwlog_info("charge_enable [%x]=0x%x\n", RT9748_CONTROL, reg);
	return 0;
}

static int rt9748_discharge(int enable)
{
	u8 reg = 0;
	int ret;
	u8 value = enable ? 0x1 : 0x0;

	ret = rt9748_write_mask(RT9748_EVENT_1_EN,
		RT9748_PD_EN_MASK, RT9748_PD_EN_SHIFT,
		value);
	if (ret)
		return -1;

	ret = rt9748_read_byte(RT9748_EVENT_1_EN, &reg);
	if (ret)
		return -1;

	hwlog_info("event1_en reg = 0x%x\n", reg);
	return 0;
}

static int rt9748_config_ioc_ocp_threshold_ma(int ocp_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (ocp_threshold < RT9748_IOC_OCP_MIN_0_MA)
			ocp_threshold = RT9748_IOC_OCP_MIN_0_MA;

		if (ocp_threshold > RT9748_IOC_OCP_MAX_6500_MA)
			ocp_threshold = RT9748_IOC_OCP_MAX_6500_MA;

		value = (u8)((ocp_threshold -
			RT9748_IOC_OCP_OFFSET_0_MA) / RT9748_IOC_OCP_STEP);
		hwlog_info("rt9748 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(RT9748_PROT_DLY_OCP,
			RT9748_IOC_OCP_MASK, RT9748_IOC_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (ocp_threshold < BQ25870_IOC_OCP_MIN_0_MA)
			ocp_threshold = BQ25870_IOC_OCP_MIN_0_MA;

		if (ocp_threshold > BQ25870_IOC_OCP_MAX_7500_MA)
			ocp_threshold = BQ25870_IOC_OCP_MAX_7500_MA;

		value = (u8)((ocp_threshold -
			BQ25870_IOC_OCP_OFFSET_0_MA) /
			BQ25870_IOC_OCP_STEP);
		hwlog_info("bq25870 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(BQ25870_PROT_DLY_OCP,
			BQ25870_IOC_OCP_MASK, BQ25870_IOC_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_config_vbus_ovp_threshold_mv(int ovp_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (ovp_threshold < RT9748_VBUS_OVP_MIN_4200_MV)
			ovp_threshold = RT9748_VBUS_OVP_MIN_4200_MV;

		if (ovp_threshold > RT9748_VBUS_OVP_MAX_6500_MV)
			ovp_threshold = RT9748_VBUS_OVP_MAX_6500_MV;

		value = (u8)((ovp_threshold -
			RT9748_VBUS_OVP_OFFSET_4200_MV) /
			RT9748_VBUS_OVP_STEP);
		hwlog_info("rt9748 ovp_threshold=%d, value=0x%x\n",
			ovp_threshold, value);
		ret = rt9748_write_mask(RT9748_VBUS_OVP,
			RT9748_VBUS_OVP_MASK, RT9748_VBUS_OVP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (ovp_threshold < BQ25870_VBUS_OVP_MIN_4200_MV)
			ovp_threshold = BQ25870_VBUS_OVP_MIN_4200_MV;

		if (ovp_threshold > BQ25870_VBUS_OVP_MAX_6510_MV)
			ovp_threshold = BQ25870_VBUS_OVP_MAX_6510_MV;

		value = (u8)((ovp_threshold -
			BQ25870_VBUS_OVP_OFFSET_4200_MV) /
			BQ25870_VBUS_OVP_STEP);
		hwlog_info("bq25870 ovp_threshold=%d, value=0x%x\n",
			ovp_threshold, value);
		ret = rt9748_write_mask(BQ25870_VBUS_OVP,
			BQ25870_VBUS_OVP_MASK, BQ25870_VBUS_OVP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_config_vout_reg_threshold_mv(int vout_reg_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (vout_reg_threshold < RT9748_VOUT_REG_MIN_4200_MV)
			vout_reg_threshold = RT9748_VOUT_REG_MIN_4200_MV;

		if (vout_reg_threshold > RT9748_VOUT_REG_MAX_5000_MV)
			vout_reg_threshold = RT9748_VOUT_REG_MAX_5000_MV;

		value = (u8)((vout_reg_threshold -
			RT9748_VOUT_REG_OFFSET_4200_MV) /
			RT9748_VOUT_REG_STEP);
		hwlog_info("rt9748 vout_reg_threshold=%d, value=0x%x\n",
			vout_reg_threshold, value);
		ret = rt9748_write_mask(RT9748_VOUT_REG,
			RT9748_VOUT_REG_MASK, RT9748_VOUT_REG_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (vout_reg_threshold < BQ25870_VOUT_REG_MIN_4200_MV)
			vout_reg_threshold = BQ25870_VOUT_REG_MIN_4200_MV;

		if (vout_reg_threshold > BQ25870_VOUT_REG_MAX_4975_MV)
			vout_reg_threshold = BQ25870_VOUT_REG_MAX_4975_MV;

		value = (u8)((vout_reg_threshold -
			BQ25870_VOUT_REG_OFFSET_4200_MV) /
			BQ25870_VOUT_REG_STEP);
		hwlog_info("bq25870 vout_reg_threshold=%d, value=0x%x\n",
			vout_reg_threshold, value);
		ret = rt9748_write_mask(BQ25870_VOUT_REG,
			BQ25870_VOUT_REG_MASK, BQ25870_VOUT_REG_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_config_vdrop_ovp_reg_threshold_mv(int vdrop_ovp_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (vdrop_ovp_threshold < RT9748_VDROP_OVP_MIN_0_MV)
			vdrop_ovp_threshold = RT9748_VDROP_OVP_MIN_0_MV;

		if (vdrop_ovp_threshold > RT9748_VDROP_OVP_MAX_1000_MV)
			vdrop_ovp_threshold = RT9748_VDROP_OVP_MAX_1000_MV;

		value = (u8)((vdrop_ovp_threshold -
			RT9748_VDROP_OVP_OFFSET_0_MV) /
			RT9748_VDROP_OVP_STEP);
		hwlog_info("rt9748 vdrop_ovp_threshold=%d, value=0x%x\n",
			vdrop_ovp_threshold, value);
		ret = rt9748_write_mask(RT9748_VDROP_OVP,
			RT9748_VDROP_OVP_MASK, RT9748_VDROP_OVP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (vdrop_ovp_threshold < BQ25870_VDROP_OVP_MIN_0_MV)
			vdrop_ovp_threshold = BQ25870_VDROP_OVP_MIN_0_MV;

		if (vdrop_ovp_threshold > BQ25870_VDROP_OVP_MAX_1000_MV)
			vdrop_ovp_threshold = BQ25870_VDROP_OVP_MAX_1000_MV;

		value = (u8)((vdrop_ovp_threshold -
			BQ25870_VDROP_OVP_OFFSET_0_MV) /
			BQ25870_VDROP_OVP_STEP);
		hwlog_info("bq25870 vdrop_ovp_threshold=%d, value=0x%x\n",
			vdrop_ovp_threshold, value);
		ret = rt9748_write_mask(BQ25870_VDROP_OVP,
			BQ25870_VDROP_OVP_MASK, BQ25870_VDROP_OVP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_config_vdrop_alm_reg_threshold_mv(int vdrop_alm_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (vdrop_alm_threshold < RT9748_VDROP_ALM_MIN_0_MV)
			vdrop_alm_threshold = RT9748_VDROP_ALM_MIN_0_MV;

		if (vdrop_alm_threshold > RT9748_VDROP_ALM_MAX_1000_MV)
			vdrop_alm_threshold = RT9748_VDROP_ALM_MAX_1000_MV;

		value = (u8)((vdrop_alm_threshold -
			RT9748_VDROP_ALM_OFFSET_0_MV) /
			RT9748_VDROP_ALM_STEP);
		hwlog_info("rt9748 vdrop_alm_threshold=%d, value=0x%x\n",
			vdrop_alm_threshold, value);
		ret = rt9748_write_mask(RT9748_VDROP_ALM,
			RT9748_VDROP_ALM_MASK, RT9748_VDROP_ALM_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (vdrop_alm_threshold < BQ25870_VDROP_ALM_MIN_0_MV)
			vdrop_alm_threshold = BQ25870_VDROP_ALM_MIN_0_MV;

		if (vdrop_alm_threshold > BQ25870_VDROP_ALM_MAX_1000_MV)
			vdrop_alm_threshold = BQ25870_VDROP_ALM_MAX_1000_MV;

		value = (u8)((vdrop_alm_threshold -
			BQ25870_VDROP_ALM_OFFSET_0_MV) /
			BQ25870_VDROP_ALM_STEP);
		hwlog_info("bq25870 vdrop_alm_threshold=%d, value=0x%x\n",
			vdrop_alm_threshold, value);
		ret = rt9748_write_mask(BQ25870_VDROP_ALM,
			BQ25870_VDROP_ALM_MASK, BQ25870_VDROP_ALM_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_config_vbat_reg_threshold_mv(int vbat_reg_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (vbat_reg_threshold < RT9748_VBAT_REG_MIN_4200_MV)
			vbat_reg_threshold = RT9748_VBAT_REG_MIN_4200_MV;

		if (vbat_reg_threshold > RT9748_VBAT_REG_MAX_5000_MV)
			vbat_reg_threshold = RT9748_VBAT_REG_MAX_5000_MV;

		value = (u8)((vbat_reg_threshold -
			RT9748_VBAT_REG_OFFSET_4200_MV) /
			RT9748_VBAT_REG_STEP);
		hwlog_info("rt9748 vbat_reg_threshold=%d, value=0x%x\n",
			vbat_reg_threshold, value);
		ret = rt9748_write_mask(RT9748_VBAT_REG,
			RT9748_VBAT_REG_MASK, RT9748_VBAT_REG_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (vbat_reg_threshold < BQ25870_VBAT_REG_MIN_4200_MV)
			vbat_reg_threshold = BQ25870_VBAT_REG_MIN_4200_MV;

		if (vbat_reg_threshold > BQ25870_VBAT_REG_MAX_4975_MV)
			vbat_reg_threshold = BQ25870_VBAT_REG_MAX_4975_MV;

		value = (u8)((vbat_reg_threshold -
			BQ25870_VBAT_REG_OFFSET_4200_MV) /
			BQ25870_VBAT_REG_STEP);
		hwlog_info("bq25870 vbat_reg_threshold=%d, value=0x%x\n",
			vbat_reg_threshold, value);
		ret = rt9748_write_mask(BQ25870_VBAT_REG,
			BQ25870_VBAT_REG_MASK, BQ25870_VBAT_REG_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return ret;
}

static int rt9748_config_ibat_ocp_threshold_ma(int ocp_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (ocp_threshold < RT9748_IBAT_OCP_MIN_400_MA)
			ocp_threshold = RT9748_IBAT_OCP_MIN_400_MA;

		if (ocp_threshold > RT9748_IBAT_OCP_MAX_6350_MA)
			ocp_threshold = RT9748_IBAT_OCP_MAX_6350_MA;

		value = (u8)((ocp_threshold -
			RT9748_IBAT_OCP_OFFSET_0_MA) / RT9748_IBAT_OCP_STEP);
		hwlog_info("rt9748 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(RT9748_IBAT_OCP,
			RT9748_IBAT_OCP_MASK, RT9748_IBAT_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (ocp_threshold < BQ25870_IBAT_OCP_MIN_0_MA)
			ocp_threshold = BQ25870_IBAT_OCP_MIN_0_MA;

		if (ocp_threshold > BQ25870_IBAT_OCP_MAX_6350_MA)
			ocp_threshold = BQ25870_IBAT_OCP_MAX_6350_MA;

		value = (u8)((ocp_threshold -
			BQ25870_IBAT_OCP_OFFSET_0_MA) / BQ25870_IBAT_OCP_STEP);
		hwlog_info("bq25870 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(BQ25870_IBAT_OCP,
			BQ25870_IBAT_OCP_MASK, BQ25870_IBAT_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return ret;
}

static int rt9748_config_ibus_ocp_threshold_ma(int ocp_threshold)
{
	u8 value;
	int ret;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		if (ocp_threshold < RT9748_IBUS_OCP_MIN_400_MA)
			ocp_threshold = RT9748_IBUS_OCP_MIN_400_MA;

		if (ocp_threshold > RT9748_IBUS_OCP_MAX_6350_MA)
			ocp_threshold = RT9748_IBUS_OCP_MAX_6350_MA;

		value = (u8)((ocp_threshold -
			RT9748_IBUS_OCP_OFFSET_0_MA) / RT9748_IBUS_OCP_STEP);
		hwlog_info("rt9748 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(RT9748_IBUS_OCP,
			RT9748_IBUS_OCP_MASK, RT9748_IBUS_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	case loadswitch_bq25870:
		if (ocp_threshold < BQ25870_IBUS_OCP_MIN_0_MA)
			ocp_threshold = BQ25870_IBUS_OCP_MIN_0_MA;

		if (ocp_threshold > BQ25870_IBUS_OCP_MAX_6300_MA)
			ocp_threshold = BQ25870_IBUS_OCP_MAX_6300_MA;

		value = (u8)((ocp_threshold -
			BQ25870_IBUS_OCP_OFFSET_0_MA) / BQ25870_IBUS_OCP_STEP);
		hwlog_info("bq25870 ocp_threshold=%d, value=0x%x\n",
			ocp_threshold, value);
		ret = rt9748_write_mask(BQ25870_IBUS_OCP,
			BQ25870_IBUS_OCP_MASK, BQ25870_IBUS_OCP_SHIFT,
			value);
		if (ret)
			return -1;

		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return ret;
}

static int rt9748_get_vbus_voltage_mv(int *vbus)
{
	int ret;
	u8 reg_high = 0;
	u8 reg_low = 0;
	int polarity;

	if (!vbus)
		return -1;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		ret = rt9748_read_byte(RT9748_VBUS_ADC2, &reg_high);
		ret |= rt9748_read_byte(RT9748_VBUS_ADC1, &reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & RT9748_VBUS_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*vbus = (reg_high & RT9748_VBUS_ADC_MASK) *
			RT9748_VBUS_HIGH_LSB;
		*vbus += reg_low * RT9748_VBUS_LOW_LSB;
		if (polarity == 1)
			*vbus *= -1;
		break;
	case loadswitch_bq25870:
		ret = rt9748_read_byte(BQ25870_VBUS_ADC2, &reg_high);
		ret |= rt9748_read_byte(BQ25870_VBUS_ADC1, &reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & BQ25870_VBUS_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*vbus = (reg_high & BQ25870_VBUS_ADC_MASK) *
			BQ25870_VBUS_HIGH_LSB;
		*vbus += reg_low * BQ25870_VBUS_LOW_LSB;
		if (polarity == 1)
			*vbus *= -1;
		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_get_bat_voltage_mv(void)
{
	int ret;
	int polarity;
	u8 reg_high = 0;
	u8 reg_low = 0;
	int vbat = 0;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		ret = rt9748_read_byte(RT9748_VBAT_ADC2, &reg_high);
		ret |= rt9748_read_byte(RT9748_VBAT_ADC1, &reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & RT9748_VBAT_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		vbat = (reg_high & RT9748_VBAT_ADC_MASK) *
			RT9748_VBAT_HIGH_LSB;
		vbat += reg_low * RT9748_VBAT_LOW_LSB;
		if (polarity == 1)
			vbat *= -1;
		break;
	case loadswitch_bq25870:
		ret = rt9748_read_byte(BQ25870_VBAT_ADC2, &reg_high);
		ret |= rt9748_read_byte(BQ25870_VBAT_ADC1, &reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & BQ25870_VBAT_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		vbat = (reg_high & BQ25870_VBAT_ADC_MASK) *
			BQ25870_VBAT_HIGH_LSB;
		vbat += reg_low * BQ25870_VBAT_LOW_LSB;
		if (polarity == 1)
			vbat *= -1;
		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return vbat;
}

static int rt9748_get_bat_current_ma(int *ibat)
{
	int ret;
	u8 reg_high = 0;
	u8 reg_low = 0;
	int polarity;

	if (!ibat)
		return -1;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		ret = rt9748_read_byte(RT9748_IBAT_ADC2, &reg_high);
		ret |= rt9748_read_byte(RT9748_IBAT_ADC1, &reg_low);
		hwlog_info("rt9748 IBAT_ADC2 = 0x%x\n", reg_high);
		hwlog_info("rt9748 IBAT_ADC1 = 0x%x\n", reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & RT9748_IBAT_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*ibat = (reg_high & RT9748_IBAT_ADC_MASK) *
			RT9748_IBAT_HIGH_LSB;
		*ibat += reg_low * RT9748_IBAT_LOW_LSB;
		if (polarity == 1)
			*ibat *= -1;
		break;
	case loadswitch_bq25870:
		ret = rt9748_read_byte(BQ25870_IBAT_ADC2, &reg_high);
		ret |= rt9748_read_byte(BQ25870_IBAT_ADC1, &reg_low);
		hwlog_info("bq25870 IBAT_ADC2 = 0x%x\n", reg_high);
		hwlog_info("bq25870 IBAT_ADC1 = 0x%x\n", reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & BQ25870_IBAT_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*ibat = (reg_high & BQ25870_IBAT_ADC_MASK) *
			BQ25870_IBAT_HIGH_LSB;
		*ibat += reg_low * BQ25870_IBAT_LOW_LSB;
		if (polarity == 1)
			*ibat *= -1;
		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int rt9748_get_ls_temp(int *temp)
{
	u8 reg;
	int ret;

	if (!temp)
		return -1;

	ret = rt9748_read_byte(RT9748_TDIE_ADC1, &reg);
	if (ret)
		return -1;

	*temp = (int)reg;
	return 0;
}

static int rt9748_get_ls_ibus(int *ibus)
{
	u8 reg_high = 0;
	u8 reg_low = 0;
	int polarity;
	int ret;

	if (!ibus)
		return -1;

	switch (g_rt9748_dev->device_id) {
	case loadswitch_rt9748:
		ret = rt9748_read_byte(RT9748_IBUS_ADC2, &reg_high);
		ret |= rt9748_read_byte(RT9748_IBUS_ADC1, &reg_low);
		hwlog_info("rt9748 IBUS_ADC2 = 0x%x\n", reg_high);
		hwlog_info("rt9748 IBUS_ADC1 = 0x%x\n", reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & RT9748_IBUS_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*ibus = (reg_high & RT9748_IBUS_ADC_MASK) *
			RT9748_IBUS_HIGH_LSB;
		*ibus += reg_low * RT9748_IBUS_LOW_LSB;
		if (polarity == 1)
			*ibus *= -1;
		break;
	case loadswitch_bq25870:
		ret = rt9748_read_byte(BQ25870_IBUS_ADC2, &reg_high);
		ret |= rt9748_read_byte(BQ25870_IBUS_ADC1, &reg_low);
		hwlog_info("bq25870 IBUS_ADC2 = 0x%x\n", reg_high);
		hwlog_info("bq25870 IBUS_ADC1 = 0x%x\n", reg_low);
		if (ret)
			return -1;

		polarity = (reg_high & BQ25870_IBUS_POLARITY_MASK) >>
			(LENTH_OF_BYTE - 1);
		*ibus = (reg_high & BQ25870_IBUS_ADC_MASK) *
			BQ25870_IBUS_HIGH_LSB;
		*ibus += reg_low * BQ25870_IBUS_LOW_LSB;
		if (polarity == 1)
			*ibus *= -1;
		break;
	default:
		hwlog_err("device_id is not found\n");
		return -1;
	}

	return 0;
}

static int loadswitch_get_device_id(void)
{
	u8 reg = 0;
	int ret;
	int bit3;
	int dev_id;
	struct rt9748_device_info *di = g_rt9748_dev;

	if (g_get_id_time == 0) {
		g_get_id_time = 1;
		ret = rt9748_read_byte(RT9748_ADC_CTRL, &reg);
		if (ret)
			return -1;

		hwlog_info("reg07=%x\n", reg);

		bit3 = (reg >> LS_GET_DEV_ID_SHIFT) & 1;
		if (bit3 == 1) {
			dev_id = loadswitch_rt9748;
			hwlog_info("dev_id=%d\n", dev_id);
			return dev_id;
		}

		ret = rt9748_read_byte(LOADSWITCH_DEV_INFO_REG0, &reg);
		if (ret)
			return -1;

		hwlog_info("reg00=%x\n", reg);

		switch (reg) {
		case 0:
			dev_id = loadswitch_rt9748;
			break;
		case 1:
			dev_id = loadswitch_bq25870;
			break;
		case 2:
			dev_id = loadswitch_fair_child;
			break;
		case 3:
			dev_id = loadswitch_nxp;
			break;
		default:
			dev_id = -1;
			break;
		}

		hwlog_info("dev_id=%d\n", dev_id);
		return dev_id;
	}

	hwlog_info("dev_id=%d\n", di->device_id);
	return di->device_id;
}

static int chip_init(void)
{
	int ret;
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	if (di->chip_already_init == 1)
		return 0;

	ret = gpio_direction_output(di->gpio_en, 1);
	if (ret) {
		hwlog_err("gpio_en fail\n");
		return -1;
	}

	msleep(200); /* delay 200ms */
	di->chip_already_init = 1;
	return 0;
}

static int rt9748_charge_status(void)
{
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	if (di->chip_already_init == 1)
		return 0;

	hwlog_err("charge_status=%d\n", di->chip_already_init);
	return -1;
}

static int rt9748_charge_init(void)
{
	int ret;
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	ret = chip_init();

	di->device_id = loadswitch_get_device_id();
	if (di->device_id == -1)
		return -1;

	hwlog_info("loadswitch device id is %d\n", di->device_id);

	ret |= rt9748_reg_init();
	ret |= rt9748_config_ioc_ocp_threshold_ma(6000);
	ret |= rt9748_config_vbus_ovp_threshold_mv(5800);
	ret |= rt9748_config_vout_reg_threshold_mv(4740);
	ret |= rt9748_config_vdrop_alm_reg_threshold_mv(1000);
	ret |= rt9748_config_vdrop_ovp_reg_threshold_mv(1000);
	ret |= rt9748_config_vbat_reg_threshold_mv(4450);
	ret |= rt9748_config_ibat_ocp_threshold_ma(5800);
	ret |= rt9748_config_ibus_ocp_threshold_ma(5800);
	if (ret)
		return -1;

	return 0;
}

static int batinfo_init(void)
{
	int ret;

	ret = chip_init();
	ret |= rt9748_adc_enable(1);
	if (ret) {
		hwlog_err("adc_enable fail\n");
		return -1;
	}

	return 0;
}

static int rt9748_charge_exit(void)
{
	int ret;
	struct rt9748_device_info *di = g_rt9748_dev;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	di->chip_already_init = 0;

	ret = rt9748_charge_enable(0);
	if (ret)
		hwlog_err("close fail\n");

	/* pull down reset pin to reset fan54151 */
	ret = gpio_direction_output(di->gpio_en, 0);
	if (ret) {
		hwlog_err("reset pull down fail\n");
		return -1;
	}

	usleep_range(10000, 11000); /* wait for 10ms */
	return ret;
}

static int rt9748_batinfo_exit(void)
{
	return 0;
}

static int rt9748_is_ls_close(void)
{
	u8 reg = 0;
	int ret;

	ret = rt9748_read_byte(RT9748_CONTROL, &reg);
	if (ret)
		return 1;

	if (reg & RT9748_CHARGE_EN_MASK)
		return 0;

	return 1;
}

static struct direct_charge_ic_ops rt9748_sysinfo_ops = {
	.dev_name = "rt9748_main",
	.ic_init = rt9748_charge_init,
	.ic_exit = rt9748_charge_exit,
	.ic_enable = rt9748_charge_enable,
	.ic_discharge = rt9748_discharge,
	.is_ic_close = rt9748_is_ls_close,
	.get_ic_id = loadswitch_get_device_id,
	.get_ic_status = rt9748_charge_status,
};

static struct direct_charge_batinfo_ops rt9748_batinfo_ops = {
	.init = batinfo_init,
	.exit = rt9748_batinfo_exit,
	.get_bat_btb_voltage = rt9748_get_bat_voltage_mv,
	.get_bat_package_voltage = rt9748_get_bat_voltage_mv,
	.get_vbus_voltage = rt9748_get_vbus_voltage_mv,
	.get_bat_current = rt9748_get_bat_current_ma,
	.get_ic_ibus = rt9748_get_ls_ibus,
	.get_ic_temp = rt9748_get_ls_temp,
};

static void rt9748_irq_work(struct work_struct *work)
{
	struct rt9748_device_info *di = NULL;
	struct nty_data *data = NULL;
	struct atomic_notifier_head *fault_notifier_list = NULL;
	u8 event1 = 0;
	u8 event2 = 0;
	u8 status = 0;
	int ret;

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct rt9748_device_info, irq_work);
	if (!di || !di->client) {
		hwlog_err("di is null\n");
		return;
	}

	data = &(di->nty_data);
	lvc_get_fault_notifier(&fault_notifier_list);

	ret = rt9748_read_byte(RT9748_EVENT_1, &event1);
	ret |= rt9748_read_byte(RT9748_EVENT_2, &event2);
	ret |= rt9748_read_byte(RT9748_EVENT_STATUS, &status);
	if (ret)
		hwlog_err("irq_work read fail\n");

	data->event1 = event1;
	data->event2 = event2;
	data->addr = di->client->addr;

	if (event1 & RT9748_VBUS_OVP_FLT) {
		hwlog_err("vbus ovp happened\n");
		atomic_notifier_call_chain(fault_notifier_list,
			DC_FAULT_VBUS_OVP, data);
	} else if (event1 & RT9748_IBUS_REVERSE_OCP_FLT) {
		hwlog_err("ibus reverse ocp happened\n");
		atomic_notifier_call_chain(fault_notifier_list,
			DC_FAULT_REVERSE_OCP, data);
	} else if (event2 & RT9748_OTP_FLT) {
		hwlog_err("otp happened\n");
		atomic_notifier_call_chain(fault_notifier_list,
			DC_FAULT_OTP, data);
	} else if (event2 & RT9748_INPUT_OCP_FLT) {
		hwlog_err("input ocp happened\n");
		atomic_notifier_call_chain(fault_notifier_list,
			DC_FAULT_INPUT_OCP, data);
	} else if (event2 & RT9748_VDROP_OVP_FLT) {
		hwlog_err("vdrop ovp happened\n");
		atomic_notifier_call_chain(fault_notifier_list,
			DC_FAULT_VDROP_OVP, data);
	}

	hwlog_info("event1 [%x]=0x%x\n", RT9748_EVENT_1, event1);
	hwlog_info("event2 [%x]=0x%x\n", RT9748_EVENT_2, event2);
	hwlog_info("status [%x]=0x%x\n", RT9748_EVENT_STATUS, status);

	/* clear irq */
	enable_irq(di->irq_int);
}

static irqreturn_t rt9748_interrupt(int irq, void *_di)
{
	struct rt9748_device_info *di = _di;

	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	hwlog_info("rt9748 int happened\n");

	disable_irq_nosync(di->irq_int);
	schedule_work(&di->irq_work);

	return IRQ_HANDLED;
}

static void rt9748_parse_dts(struct device_node *np,
	struct rt9748_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ic_role", &di->ic_role, CHARGE_IC_TYPE_MAIN);
}

static int rt9748_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct rt9748_device_info *di = NULL;
	struct device_node *np = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_rt9748_dev = di;
	di->chip_already_init = 0;
	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;
	i2c_set_clientdata(client, di);
	INIT_WORK(&di->irq_work, rt9748_irq_work);

	rt9748_parse_dts(np, di);

	ret = power_gpio_config_interrupt(np,
		"loadswitch_int", "loadswitch_int",
		&di->gpio_int, &di->irq_int);
	if (ret)
		goto rt9748_fail_0;

	ret = power_gpio_config_output(np,
		"loadswitch_en", "loadswitch_en", &di->gpio_en, 0);
	if (ret)
		goto rt9748_fail_1;

	ret = request_irq(di->irq_int, rt9748_interrupt,
		IRQF_TRIGGER_FALLING, "loadswitch_int_irq", di);
	if (ret) {
		hwlog_err("gpio irq request fail\n");
		di->irq_int = -1;
		goto rt9748_fail_2;
	}

	ret = loadswitch_main_ops_register(&rt9748_sysinfo_ops);
	if (ret) {
		hwlog_err("rt9748 sysinfo ops register fail\n");
		goto rt9748_fail_3;
	}

	ret = dc_batinfo_ops_register(LVC_MODE, di->ic_role, &rt9748_batinfo_ops);
	if (ret) {
		hwlog_err("rt9748 batinfo ops register fail\n");
		goto rt9748_fail_3;
	}

	return 0;

rt9748_fail_3:
	free_irq(di->irq_int, di);
rt9748_fail_2:
	gpio_free(di->gpio_en);
rt9748_fail_1:
	gpio_free(di->gpio_int);
rt9748_fail_0:
	devm_kfree(&client->dev, di);
	g_rt9748_dev = NULL;

	return ret;
}

static int rt9748_remove(struct i2c_client *client)
{
	struct rt9748_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	/* reset rt9748 */
	gpio_set_value(di->gpio_en, 0);

	if (di->gpio_en)
		gpio_free(di->gpio_en);

	if (di->irq_int)
		free_irq(di->irq_int, di);

	if (di->gpio_int)
		gpio_free(di->gpio_int);

	return 0;
}

MODULE_DEVICE_TABLE(i2c, rt9748);
static const struct of_device_id rt9748_of_match[] = {
	{
		.compatible = "rt9748_main",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id rt9748_i2c_id[] = {
	{ "rt9748_main", 0 }, {}
};

static struct i2c_driver rt9748_driver = {
	.probe = rt9748_probe,
	.remove = rt9748_remove,
	.id_table = rt9748_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "rt9748_main",
		.of_match_table = of_match_ptr(rt9748_of_match),
	},
};

static int __init rt9748_init(void)
{
	return i2c_add_driver(&rt9748_driver);
}

static void __exit rt9748_exit(void)
{
	i2c_del_driver(&rt9748_driver);
}

module_init(rt9748_init);
module_exit(rt9748_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("rt9748 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
