/*
 * vsys_switch.c
 *
 * vsys switch driver
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
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/pm_wakeup.h>
#include <linux/notifier.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <chipset_common/hwpower/power_common.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/huawei_battery_temp.h>
#include <huawei_platform/power/vsys_switch/vsys_switch.h>
#include <chipset_common/hwpower/power_sysfs.h>
#include <chipset_common/hwpower/power_dts.h>

#ifdef CONFIG_HUAWEI_BATTERY_VOLTAGE
#include <huawei_platform/power/battery_voltage.h>
#endif

#ifdef CONFIG_HISI_BCI_BATTERY
#include <linux/power/hisi/hisi_bci_battery.h>
#endif

#ifdef CONFIG_HISI_COUL
#include <linux/power/hisi/coul/hisi_coul_drv.h>
#endif

#ifdef CONFIG_HUAWEI_POWER_DEBUG
#include <chipset_common/hwpower/power_debug.h>
#endif

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG vsys_switch
HWLOG_REGIST();

static struct vsys_buck_device_ops *g_buck_ops;
static struct vsys_sc_device_ops *g_sc_ops;
static struct vsys_ovp_switch_device_ops *g_ovp_switch_ops;
struct device *vsys_switch_dev;
static struct vsys_switch_device_info *g_vsys_switch_di;
static int vbatt_samples[MAF_BUFFER_LEN];
static int ibatt_samples[MAF_BUFFER_LEN];
static int rbatt_samples[MAF_BUFFER_LEN];

ATOMIC_NOTIFIER_HEAD(vsys_sc_event_nh);

int vsys_buck_ops_register(struct vsys_buck_device_ops *ops)
{
	int ret = 0;

	if (ops) {
		g_buck_ops = ops;
		hwlog_info("%s buck ops register ok\n", ops->chip_name);
	} else {
		hwlog_err("buck ops register fail\n");
		ret = -EPERM;
	}

	return ret;
}

int vsys_sc_ops_register(struct vsys_sc_device_ops *ops)
{
	int ret = 0;

	if (ops) {
		g_sc_ops = ops;
		hwlog_info("%s sc ops register ok\n", ops->chip_name);
	} else {
		hwlog_err("sc ops register fail\n");
		ret = -EPERM;
	}

	return ret;
}

int vsys_ovp_switch_ops_register(struct vsys_ovp_switch_device_ops *ops)
{
	int ret = 0;

	if (ops) {
		g_ovp_switch_ops = ops;
		hwlog_info("%s ovpsw ops register ok\n", ops->chip_name);
	} else {
		hwlog_err("ovpsw ops register fail\n");
		ret = -EPERM;
	}

	return ret;
}

int vsys_switch_set_sc_frequency_mode(u8 mode)
{
	struct vsys_sc_device_ops *di = g_sc_ops;

	if (!di || !di->set_frequency_mode) {
		hwlog_err("di or ops is null\n");
		return -1;
	}

	return di->set_frequency_mode(mode);
}

static void vsys_switch_calc_avg_vbatt(struct vsys_switch_device_info *di,
	int vbatt_sample)
{
	int index;

	index = di->batt_para.vbatt_maf_count % MAF_BUFFER_LEN;

	di->batt_para.vbatt_sum -= vbatt_samples[index];
	vbatt_samples[index] = vbatt_sample;
	di->batt_para.vbatt_sum += vbatt_samples[index];

	if (++di->batt_para.vbatt_maf_count >= MAF_BUFFER_LEN)
		di->batt_para.vbatt_maf_count = 0;

	di->batt_para.vbatt_avg = di->batt_para.vbatt_sum / MAF_BUFFER_LEN;
}

static void vsys_switch_calc_avg_ibatt(struct vsys_switch_device_info *di,
	int ibatt_sample)
{
	int index;

	index = di->batt_para.ibatt_maf_count % MAF_BUFFER_LEN;

	di->batt_para.ibatt_sum -= ibatt_samples[index];
	ibatt_samples[index] = ibatt_sample;
	di->batt_para.ibatt_sum += ibatt_samples[index];

	if (++di->batt_para.ibatt_maf_count >= MAF_BUFFER_LEN)
		di->batt_para.ibatt_maf_count = 0;

	di->batt_para.ibatt_avg = di->batt_para.ibatt_sum / MAF_BUFFER_LEN;
}

static void vsys_switch_calc_avg_rbatt(struct vsys_switch_device_info *di,
	int rbatt_sample)
{
	int index;

	index = di->batt_para.rbatt_maf_count % MAF_BUFFER_LEN;

	di->batt_para.rbatt_sum -= rbatt_samples[index];
	rbatt_samples[index] = rbatt_sample;
	di->batt_para.rbatt_sum += rbatt_samples[index];

	if (++di->batt_para.rbatt_maf_count >= MAF_BUFFER_LEN)
		di->batt_para.rbatt_maf_count = 0;

	di->batt_para.rbatt_avg = di->batt_para.rbatt_sum / MAF_BUFFER_LEN;
}

static void vsys_switch_reset_avg_vbatt(struct vsys_switch_device_info *di)
{
	int i;

	for (i = 0; i < MAF_BUFFER_LEN; i++)
		vbatt_samples[i] = VBATT_VAL_MIN;

	di->batt_para.vbatt_avg = VBATT_VAL_MIN;
	di->batt_para.vbatt_sum = VBATT_VAL_MIN * MAF_BUFFER_LEN;
	di->batt_para.vbatt_maf_count = 0;
}

static void vsys_switch_reset_avg_ibatt(struct vsys_switch_device_info *di)
{
	int i;

	for (i = 0; i < MAF_BUFFER_LEN; i++)
		ibatt_samples[i] = IBATT_VAL_MAX;

	di->batt_para.ibatt_avg = IBATT_VAL_MAX;
	di->batt_para.ibatt_sum = IBATT_VAL_MAX * MAF_BUFFER_LEN;
	di->batt_para.ibatt_maf_count = 0;
}

static void vsys_switch_reset_avg_rbatt(struct vsys_switch_device_info *di)
{
	int i;

	for (i = 0; i < MAF_BUFFER_LEN; i++)
		rbatt_samples[i] = RBATT_VAL_MAX;

	di->batt_para.rbatt_avg = RBATT_VAL_MAX;
	di->batt_para.rbatt_sum = RBATT_VAL_MAX * MAF_BUFFER_LEN;
	di->batt_para.rbatt_maf_count = 0;
}

static int vsys_switch_calc_rbatt(struct vsys_switch_device_info *di,
	int batt_temp, int batt_soc, int batt_cycle)
{
	int i;
	int j;
	int k;

	for (i = 0; i < di->rbatt_sf_lut->rows - 1; i++) {
		if (batt_temp >= di->rbatt_sf_lut->temp[i])
			break;
	}

	for (j = di->rbatt_sf_lut->cols - 1; j > 0; j--) {
		if (batt_soc >= di->rbatt_sf_lut->soc[j])
			break;
	}

	for (k = 0; k < di->rbatt_aging_data.total_level - 1; k++) {
		if (batt_cycle >=
			di->rbatt_aging_data.rbatt_aging_para[k].cycle)
			break;
	}

	return di->rbatt_sf_lut->sf[i][j] *
		di->rbatt_aging_data.rbatt_aging_para[k].coefficient;
}

static int vsys_switch_calc_series_rbatt(struct vsys_switch_device_info *di)
{
	int temp0;
	int temp1;
	int soc;
	int cycle;
	int series_rbatt;

	huawei_battery_temp(BAT_TEMP_0, &temp0);
	huawei_battery_temp(BAT_TEMP_1, &temp1);
	soc = hisi_battery_capacity();
	cycle = hisi_battery_cycle_count();

	series_rbatt = vsys_switch_calc_rbatt(di, temp0, soc, cycle) +
		vsys_switch_calc_rbatt(di, temp1, soc, cycle);

	return series_rbatt;
}

static void vsys_switch_calc_avg_batt_para(struct vsys_switch_device_info *di)
{
	int vbatt_sample;
	int ibatt_sample;
	int rbatt_sample;

	if (di->cancel_ctrl_work_flag)
		return;

	vbatt_sample = hw_battery_voltage(BAT_ID_ALL);
	ibatt_sample = hisi_battery_current();
	rbatt_sample = vsys_switch_calc_series_rbatt(di);

	vsys_switch_calc_avg_vbatt(di, vbatt_sample);
	vsys_switch_calc_avg_ibatt(di, ibatt_sample);
	vsys_switch_calc_avg_rbatt(di, rbatt_sample);
	hwlog_info("vbatt_avg = %d, ibatt_avg = %d, rbatt_avg = %d\n",
		di->batt_para.vbatt_avg, di->batt_para.ibatt_avg, di->batt_para.rbatt_avg);
}

static void vsys_switch_get_vsys_volt(struct vsys_switch_device_info *di)
{
	if (di->state_info.curr_vsys_chnl == VSYS_BUCK_OPEN) {
		if (di->buck_ops->get_vout() < 0) {
			hwlog_err("get buck vout failed\n");
			return;
		}
		di->state_info.vsys_volt = di->buck_ops->get_vout();
	} else if (di->state_info.curr_vsys_chnl == VSYS_SC_OPEN) {
		di->state_info.vsys_volt = hw_battery_voltage(BAT_ID_ALL) / 2;
	}
}

static void vsys_switch_wake_lock(void)
{
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (!di->wakelock.active) {
		__pm_stay_awake(&di->wakelock);
		hwlog_info("wake_lock\n");
	}
}

static void vsys_switch_wake_unlock(void)
{
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (di->wakelock.active) {
		__pm_relax(&di->wakelock);
		hwlog_info("wake_unlock\n");
	}
}

static int vsys_switch_enable_sc_channel(struct vsys_switch_device_info *di)
{
	int ret;

	ret = di->sc_ops->set_state(SC_CHNL_ENABLE);
	if (ret) {
		hwlog_err("enable_sc_chnl: enable sc fail\n");
		if ((ret == VSYS_SC_SET_TEST_REGS_FAIL) ||
			(ret == VSYS_SC_NOT_PG))
			goto enable_sc_fail_1;
		else
			goto enable_sc_fail_0;
	}

	/* wait for 10ms to enable ovp switch */
	usleep_range(10000, 11000);

	ret = di->ovp_ops->set_state(SC_CHNL_ENABLE);
	if (ret) {
		hwlog_err("enable_sc_chnl: enable ovp switch fail\n");
		goto enable_sc_fail_1;
	}

	hwlog_info("enable sc channel succeed\n");
	return 0;

enable_sc_fail_1:
	di->sc_ops->set_state(SC_CHNL_DISABLE);
enable_sc_fail_0:
	return ret;
}

static int vsys_switch_enable_buck_channel(struct vsys_switch_device_info *di)
{
	int ret;

	ret = di->buck_ops->set_vout(BUCK_VOUT_LOW);
	if (ret) {
		hwlog_err("enable_buck_chnl: set buck vout 4300 failed\n");
		goto enable_buck_fail_0;
	}

	hwlog_info("enable buck channel succeed\n");
	return 0;

enable_buck_fail_0:
	return ret;
}

static int vsys_switch_disable_sc_channel(struct vsys_switch_device_info *di)
{
	int ret;

	ret = di->ovp_ops->set_state(SC_CHNL_DISABLE);
	if (ret) {
		hwlog_err("disable_sc_chnl: disable ovp switch failed\n");
		goto disable_sc_fail_0;
	}

	/* wait for 20ms */
	msleep(20);

	ret = di->sc_ops->set_state(SC_CHNL_DISABLE);
	if (ret) {
		hwlog_err("disable_sc_chnl: disable sc failed\n");
		if (ret == VSYS_SC_SET_MODE_CTRL_REG_FAIL)
			goto disable_sc_fail_2;
		else
			goto disable_sc_fail_1;
	}

	hwlog_info("disable sc channel succeed\n");
	return 0;

disable_sc_fail_2:
	di->sc_ops->set_state(SC_CHNL_ENABLE);
	/* wait for 10ms to enable ovp switch */
	usleep_range(10000, 11000);
disable_sc_fail_1:
	di->ovp_ops->set_state(SC_CHNL_ENABLE);
disable_sc_fail_0:
	return ret;
}

static int vsys_switch_disable_buck_channel(struct vsys_switch_device_info *di)
{
	int ret;

	ret = di->buck_ops->set_vout(BUCK_VOUT_LOW);
	if (ret) {
		hwlog_err("disable_buck_chnl: set buck vout 3500 failed\n");
		goto disable_buck_fail_0;
	}

	hwlog_info("disable buck channel succeed\n");
	return 0;

disable_buck_fail_0:
	return ret;
}

static int vsys_switch_enable_sc_channel_only(
	struct vsys_switch_device_info *di)
{
	int ret;

	vsys_switch_wake_lock();

	ret = di->buck_ops->set_vout(BUCK_VOUT_HIGH);
	if (ret) {
		hwlog_err("set buck vout 4200 failed\n");
		goto buck_hi_fail;
	}
	/* wait 10ms for voltage ready */
	usleep_range(10000, 11000);

	ret = vsys_switch_enable_sc_channel(di);
	if (ret) {
		hwlog_err("enable sc channel failed\n");
		goto enable_sc_only_fail_0;
	}

	/* wait for 50ms */
	usleep_range(50000, 51000);

	ret = di->buck_ops->set_vout(BUCK_VOUT_LOW);
	if (ret) {
		hwlog_err("set buck vout 3500 failed\n");
		goto enable_sc_only_fail_1;
	}

	di->state_info.curr_vsys_chnl = VSYS_SC_OPEN;
	hwlog_info("enable_sc_channel_only succeed\n");
	vsys_switch_wake_unlock();

	return 0;

enable_sc_only_fail_1:
	di->ovp_ops->set_state(SC_CHNL_DISABLE);
	/* wait for 20ms */
	msleep(20);
	di->sc_ops->set_state(SC_CHNL_DISABLE);
enable_sc_only_fail_0:
	di->buck_ops->set_vout(BUCK_VOUT_LOW);
buck_hi_fail:
	vsys_switch_wake_unlock();
	return -1;
}

static int vsys_switch_enable_buck_channel_only(
	struct vsys_switch_device_info *di)
{
	int ret;

	vsys_switch_wake_lock();

	ret = di->buck_ops->set_vout(BUCK_VOUT_HIGH);
	if (ret) {
		hwlog_err("set buck vout 4200 failed\n");
		goto enable_buck_only_fail_0;
	}

	/* wait 10ms for voltage ready */
	usleep_range(10000, 11000);

	ret = vsys_switch_disable_sc_channel(di);
	if (ret) {
		hwlog_err("disable sc channel failed\n");
		goto enable_buck_only_fail_1;
	}

	/* wait for 20ms */
	usleep_range(20000, 21000);

	ret = vsys_switch_enable_buck_channel(di);
	if (ret) {
		hwlog_err("enable buck channel failed\n");
		goto enable_buck_only_fail_0;
	}

	di->state_info.curr_vsys_chnl = VSYS_BUCK_OPEN;
	hwlog_info("enable_buck_channel_only succeed\n");
	vsys_switch_wake_unlock();

	return 0;

enable_buck_only_fail_1:
	/* wait for 50ms */
	usleep_range(50000, 51000);
	di->buck_ops->set_vout(BUCK_VOUT_LOW);
enable_buck_only_fail_0:
	vsys_switch_wake_unlock();
	return -1;
}

static int vsys_switch_set_sc_channel(struct vsys_switch_device_info *di)
{
	int ret = 0;

	if (di->ctrl_para.enable_sc == SC_CHNL_ENABLE) {
		ret = vsys_switch_enable_sc_channel(di);
		if (ret)
			hwlog_err("set_sc_chnl: enable sc chnl failed\n");
	} else if (di->ctrl_para.enable_sc == SC_CHNL_DISABLE) {
		ret = vsys_switch_disable_sc_channel(di);
		if (ret)
			hwlog_err("set_sc_chnl: disable sc chnl failed\n");
	}

	return ret;
}

static int vsys_switch_set_buck_channel(struct vsys_switch_device_info *di)
{
	int ret = 0;

	if (di->ctrl_para.enable_buck == BUCK_CHNL_ENABLE) {
		ret = vsys_switch_enable_buck_channel(di);
		if (ret)
			hwlog_err("set_buck_chnl: enable buck chnl fail\n");
	} else if (di->ctrl_para.enable_buck == BUCK_CHNL_DISABLE) {
		ret = vsys_switch_disable_buck_channel(di);
		if (ret)
			hwlog_err("set_buck_chnl: disable buck chnl fail\n");
	}

	return ret;
}

static bool vsys_switch_can_open_buck_chnl(
	struct vsys_switch_device_info *di)
{
	bool ret = false;
	int calc_val;

	di->batt_para.ibatt_avg = 0;

	calc_val = di->batt_para.vbatt_avg +
		(di->batt_para.ibatt_avg - di->ctrl_para.dischg_curr_max) *
		(di->batt_para.comp_res * TEN + di->batt_para.rbatt_avg) /
		TEN / THOUSAND;

	if (calc_val < di->ctrl_para.vbatt_base)
		ret = true;

	return ret;
}

static bool vsys_switch_can_open_sc_chnl(
	struct vsys_switch_device_info *di)
{
	bool ret = false;
	int calc_val;

	di->batt_para.ibatt_avg = 0;

	calc_val = di->batt_para.vbatt_avg +
		(di->batt_para.ibatt_avg - di->ctrl_para.dischg_curr_max) *
		(di->batt_para.comp_res * TEN + di->batt_para.rbatt_avg) /
		TEN / THOUSAND;

	if (calc_val > (di->ctrl_para.vbatt_base + di->ctrl_para.vbatt_gap))
		ret = true;

	return ret;
}

static int vsys_switch_set_vsys_channel(struct vsys_switch_device_info *di)
{
	int ret = 0;

	if (di->cancel_ctrl_work_flag) {
		hwlog_info("cancel control work\n");
		return 0;
	}

	if (!di->support_sc_chnl) {
		hwlog_info("not support sc channel\n");
		return 0;
	}

	if (di->state_info.sc_not_pg) {
		ret = vsys_switch_enable_buck_channel_only(di);
		if (!ret)
			di->state_info.sc_not_pg = false;

		return ret;
	}

	hwlog_info("vsys_chnl %d,  curr_vsys %d\n",
		di->ctrl_para.switch_vsys_chnl, di->state_info.curr_vsys_chnl);

	if (di->ctrl_para.switch_vsys_chnl == SWITCH_VSYS_AUTO) {
		if (di->switch_sc_retry_num < SWITCH_SC_RETRY_MAX) {
			if (vsys_switch_can_open_sc_chnl(di) &&
				di->state_info.curr_vsys_chnl != VSYS_SC_OPEN) {
				hwlog_info("sc vbat=%d ibatt=%d rbatt=%d\n",
					di->batt_para.vbatt_avg,
					di->batt_para.ibatt_avg,
					di->batt_para.rbatt_avg/TEN);
				ret = vsys_switch_enable_sc_channel_only(di);
				if (ret)
					di->switch_sc_retry_num++;
				return ret;
			}
		} else if (di->switch_sc_retry_num == SWITCH_SC_RETRY_MAX) {
			hwlog_err("switch sc exceed retry max, stop retry\n");
			di->switch_sc_retry_num++;
		}

		if (vsys_switch_can_open_buck_chnl(di)) {
			if (di->state_info.curr_vsys_chnl != VSYS_BUCK_OPEN) {
				hwlog_info("buck vbat=%d ibatt=%d rbatt=%d\n",
					di->batt_para.vbatt_avg,
					di->batt_para.ibatt_avg,
					di->batt_para.rbatt_avg/TEN);
				return vsys_switch_enable_buck_channel_only(di);
			}
		}
	} else if (di->ctrl_para.switch_vsys_chnl == SWITCH_VSYS_SC) {
		if (di->switch_sc_retry_num < SWITCH_SC_RETRY_MAX) {
			if ((hw_battery_voltage(BAT_ID_ALL) <
				di->ctrl_para.vbatt_th) &&
				(di->state_info.curr_vsys_chnl !=
				VSYS_SC_OPEN)) {
				hwlog_info("vbatt is low, can't switch sc\n");
				return ret;
			}
			if (di->state_info.curr_vsys_chnl != VSYS_SC_OPEN) {
				hwlog_err("enable sc channel only\n");
				ret = vsys_switch_enable_sc_channel_only(di);
				if (ret)
					di->switch_sc_retry_num++;
				return ret;
			}
		} else if (di->switch_sc_retry_num == SWITCH_SC_RETRY_MAX) {
			hwlog_err("switch sc exceed retry max, stop retry\n");
			di->switch_sc_retry_num++;
		}
	} else if (di->ctrl_para.switch_vsys_chnl == SWITCH_VSYS_BUCK) {
		if (di->state_info.curr_vsys_chnl != VSYS_BUCK_OPEN) {
			hwlog_err("enable buck channel only\n");
			return vsys_switch_enable_buck_channel_only(di);
		}
	}

	return ret;
}

static void vsys_switch_control_work(struct work_struct *work)
{
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (di->cancel_ctrl_work_flag)
		return;

	vsys_switch_calc_avg_batt_para(di);
	vsys_switch_set_vsys_channel(di);

	if (di->cancel_ctrl_work_flag)
		return;

	schedule_delayed_work(&di->ctrl_work,
		msecs_to_jiffies(di->ctrl_work_interval));
}

static void vsys_switch_sc_event_work(struct work_struct *work)
{
	struct vsys_switch_device_info *di = NULL;

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct vsys_switch_device_info, sc_event_work);
	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (di->sc_event_type == VSYS_SC_INT_NOT_PG)
		di->state_info.sc_not_pg = true;
	else
		di->state_info.sc_not_pg = false;
}

static int vsys_switch_sc_event_notifier_call(
	struct notifier_block *nb, unsigned long event, void *data)
{
	struct vsys_switch_device_info *di = NULL;

	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	di = container_of(nb, struct vsys_switch_device_info, sc_event_nb);
	if (!di) {
		hwlog_err("di is null\n");
		return NOTIFY_OK;
	}

	di->sc_event_type = (enum vsys_sc_int_type)event;
	schedule_work(&di->sc_event_work);
	return NOTIFY_OK;
}

static int vsys_switch_check_support_sc_chnl(
	struct vsys_switch_device_info *di)
{
	int device_id;
	int ret;

	di->ovp_ops->set_state(SC_CHNL_ENABLE);

	/* wait for 50ms */
	usleep_range(50000, 51000);

	ret = di->ovp_ops->get_id(&device_id);
	if (ret) {
		di->support_sc_chnl = false;
		di->ovp_ops->set_state(SC_CHNL_DISABLE);
		return ret;
	}

	di->ovp_ops->set_state(SC_CHNL_DISABLE);

	if (device_id != VSYS_OVP_SWITCH_FPF2283) {
		di->support_sc_chnl = false;
		hwlog_info("support_sc_chnl false\n");
	}

	return ret;
}

static void vsys_switch_para_reset(struct vsys_switch_device_info *di)
{
	int i;
	int vbat_sample;

	di->ctrl_para.enable_sc = 0;
	di->ctrl_para.enable_buck = 0;
	di->cancel_ctrl_work_flag = false;
	vsys_switch_reset_avg_ibatt(di);
	vsys_switch_reset_avg_rbatt(di);

	vbat_sample = hw_battery_voltage(BAT_ID_ALL);

	for (i = 0; i < MAF_BUFFER_LEN; i++)
		vbatt_samples[i] = vbat_sample;

	di->batt_para.vbatt_avg = vbat_sample;
	di->batt_para.vbatt_sum = vbat_sample * MAF_BUFFER_LEN;
	di->batt_para.vbatt_maf_count = 0;
}

static void vsys_switch_para_init(struct vsys_switch_device_info *di)
{
	di->ctrl_work_interval = CONTROL_INTERVAL;
	di->state_info.curr_vsys_chnl = VSYS_BUCK_OPEN;
	di->state_info.sc_not_pg = false;
	di->ctrl_para.enable_sc = 0;
	di->ctrl_para.enable_buck = 0;
	di->ctrl_para.switch_vsys_chnl = SWITCH_VSYS_AUTO;
	di->cancel_ctrl_work_flag = false;
	di->support_sc_chnl = true;
	di->switch_sc_retry_num = 0;
	vsys_switch_reset_avg_vbatt(di);
	vsys_switch_reset_avg_ibatt(di);
	vsys_switch_reset_avg_rbatt(di);
}

static int vsys_switch_check_ops(struct vsys_switch_device_info *di)
{
	int ret = 0;

	if ((!di->buck_ops) || (!di->sc_ops) || (!di->ovp_ops) ||
		(!di->buck_ops->get_state) ||
		(!di->buck_ops->set_state) ||
		(!di->buck_ops->get_vout) ||
		(!di->buck_ops->set_vout) ||
		(!di->sc_ops->get_state) ||
		(!di->sc_ops->set_state) ||
		(!di->sc_ops->get_id) ||
		(!di->sc_ops->set_frequency_mode) ||
		(!di->ovp_ops->set_state) ||
		(!di->ovp_ops->get_state) ||
		(!di->ovp_ops->get_id)) {
		hwlog_err("vsys_switch ops is null\n");
		ret = -EINVAL;
	}

	return ret;
}

#ifdef CONFIG_HUAWEI_POWER_DEBUG
static ssize_t vsys_switch_dbg_show_switch_vsys(
	void *dev_data, char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "buf or dev_p or di is null\n");
	}

	return scnprintf(buf, size, "switch_vsys_chnl = %d\n",
		di->ctrl_para.switch_vsys_chnl);
}

static ssize_t vsys_switch_dbg_store_switch_vsys(
	const void *dev_data, char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return -EINVAL;
	}

	if ((kstrtoint(buf, 0, &val) < 0) || (val < 0) || (val > 2)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	di->ctrl_para.switch_vsys_chnl = val;

	hwlog_info("set switch_vsys = %d\n",
		di->ctrl_para.switch_vsys_chnl);

	return size;
}

static ssize_t vsys_switch_dbg_show_enable_sc(void *dev_data,
	char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "dev_p or buf or di is null\n");
	}

	return scnprintf(buf, size, "enable_sc = %d\n",
		di->ctrl_para.enable_sc);
}

static ssize_t vsys_switch_dbg_store_enable_sc(void *dev_data,
	const char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return -EINVAL;
	}

	if ((kstrtoint(buf, 0, &val) < 0) || (val < 0) || (val > 1)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	di->ctrl_para.enable_sc = val;
	vsys_switch_set_sc_channel(di);

	hwlog_info("set enable_sc = %d\n", di->ctrl_para.enable_sc);

	return size;
}

static ssize_t vsys_switch_dbg_show_enable_buck(void *dev_data,
	char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "dev_p or buf or di is null\n");
	}

	return scnprintf(buf, size, "enable_buck = %d\n",
		di->ctrl_para.enable_buck);
}

static ssize_t vsys_switch_dbg_store_enable_buck(void *dev_data,
	const char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return -EINVAL;
	}

	if ((kstrtoint(buf, 0, &val) < 0) || (val < 0) || (val > 1)) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	di->ctrl_para.enable_buck = val;
	vsys_switch_set_buck_channel(di);

	hwlog_info("set enable_buck = %d\n",
		di->ctrl_para.enable_buck);

	return size;
}

static ssize_t vsys_switch_dbg_show_vbatt_threshold(void *dev_data,
	char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "dev_p or buf or di is null\n");
	}

	return scnprintf(buf, size, "vbatt_threshold = %d\n",
		di->ctrl_para.vbatt_th);
}

static ssize_t vsys_switch_dbg_store_vbatt_threshold(void *dev_data,
	const char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return -EINVAL;
	}

	if (kstrtoint(buf, 0, &val) < 0) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	if (val < VBATT_TH_MIN)
		di->ctrl_para.vbatt_th = VBATT_TH_MIN;
	else if (val > VBATT_TH_MAX)
		di->ctrl_para.vbatt_th = VBATT_TH_MAX;
	else
		di->ctrl_para.vbatt_th = val;

	hwlog_info("set vbatt_threshold = %d\n",
		di->ctrl_para.vbatt_th);

	return size;
}

static ssize_t vsys_switch_dbg_show_vsys_volt(void *dev_data,
	char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "dev_p or buf or di is null\n");
	}

	vsys_switch_get_vsys_volt(di);
	return scnprintf(buf, size, "vsys voltage is %d\n",
		di->state_info.vsys_volt);
}

static ssize_t vsys_switch_dbg_store_vsys_volt(void *dev_data,
	const char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p) {
		hwlog_err("buf or dev_p is null\n");
		return -EINVAL;
	}

	if (kstrtoint(buf, 0, &val) < 0) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	hwlog_info("vsys_volt is read only\n");

	return size;
}

static ssize_t vsys_switch_dbg_show_vsys_chnl(void *dev_data,
	char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p || !di) {
		hwlog_err("buf or dev_p or di is null\n");
		return scnprintf(buf, size, "buf or dev_p or di is null\n");
	}

	return scnprintf(buf, size, "current vsys_chnl is %d\n",
		di->state_info.curr_vsys_chnl);
}

static ssize_t vsys_switch_dbg_store_vsys_chnl(void *dev_data,
	const char *buf, size_t size)
{
	struct vsys_switch_device_info *dev_p = NULL;
	int val = 0;

	dev_p = (struct vsys_switch_device_info *)dev_data;
	if (!buf || !dev_p) {
		hwlog_err("buf or dev_p is null\n");
		return -EINVAL;
	}

	if (kstrtoint(buf, 0, &val) < 0) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	hwlog_info("vsys_chnl is read only\n");

	return size;
}
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

#ifdef CONFIG_SYSFS
static ssize_t vsys_switch_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);
static ssize_t vsys_switch_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);

static struct power_sysfs_attr_info vsys_switch_sysfs_field_tbl[] = {
	power_sysfs_attr_rw(vsys_switch, 0640, VSYS_SWITCH_SYSFS_SWITCH_VSYS, switch_vsys),
	power_sysfs_attr_rw(vsys_switch, 0640, VSYS_SWITCH_SYSFS_ENABLE_SC, enable_sc),
	power_sysfs_attr_rw(vsys_switch, 0640, VSYS_SWITCH_SYSFS_ENABLE_BUCK, enable_buck),
	power_sysfs_attr_rw(vsys_switch, 0640, VSYS_SWITCH_SYSFS_VBATT_TH, vbatt_th),
	power_sysfs_attr_ro(vsys_switch, 0440, VSYS_SWITCH_SYSFS_VSYS_VOLT, vsys_volt),
	power_sysfs_attr_ro(vsys_switch, 0440, VSYS_SWITCH_SYSFS_VSYS_CHNL, vsys_chnl),
};

#define VSYS_SWITCH_SYSFS_ATTRS_SIZE  ARRAY_SIZE(vsys_switch_sysfs_field_tbl)

static struct attribute *vsys_switch_sysfs_attrs[VSYS_SWITCH_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group vsys_switch_sysfs_attr_group = {
	.attrs = vsys_switch_sysfs_attrs,
};

static ssize_t vsys_switch_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	int len = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		vsys_switch_sysfs_field_tbl, VSYS_SWITCH_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case VSYS_SWITCH_SYSFS_SWITCH_VSYS:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->ctrl_para.switch_vsys_chnl);
		break;
	case VSYS_SWITCH_SYSFS_ENABLE_SC:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->ctrl_para.enable_sc);
		break;
	case VSYS_SWITCH_SYSFS_ENABLE_BUCK:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->ctrl_para.enable_buck);
		break;
	case VSYS_SWITCH_SYSFS_VBATT_TH:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->ctrl_para.vbatt_th);
		break;
	case VSYS_SWITCH_SYSFS_VSYS_VOLT:
		vsys_switch_get_vsys_volt(di);
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->state_info.vsys_volt);
		break;
	case VSYS_SWITCH_SYSFS_VSYS_CHNL:
		len = snprintf(buf, PAGE_SIZE, "%d\n",
			di->state_info.curr_vsys_chnl);
		break;
	default:
		break;
	}

	return len;
}

static ssize_t vsys_switch_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct power_sysfs_attr_info *info = NULL;
	struct vsys_switch_device_info *di = g_vsys_switch_di;
	long val = 0;

	info = power_sysfs_lookup_attr(attr->attr.name,
		vsys_switch_sysfs_field_tbl, VSYS_SWITCH_SYSFS_ATTRS_SIZE);
	if (!info || !di)
		return -EINVAL;

	if (kstrtol(buf, POWER_BASE_DEC, &val) < 0) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	switch (info->name) {
	case VSYS_SWITCH_SYSFS_SWITCH_VSYS:
		di->ctrl_para.switch_vsys_chnl = val;
		hwlog_info("set switch_vsys = %d\n",
			di->ctrl_para.switch_vsys_chnl);
		break;
	case VSYS_SWITCH_SYSFS_ENABLE_SC:
		di->ctrl_para.enable_sc = val;
		vsys_switch_set_sc_channel(di);
		hwlog_info("set enable_sc = %d\n", di->ctrl_para.enable_sc);
		break;
	case VSYS_SWITCH_SYSFS_ENABLE_BUCK:
		di->ctrl_para.enable_buck = val;
		vsys_switch_set_buck_channel(di);
		hwlog_info("set enable_buck = %d\n", di->ctrl_para.enable_buck);
		break;
	case VSYS_SWITCH_SYSFS_VBATT_TH:
		di->ctrl_para.vbatt_th = val;
		hwlog_info("set vbatt_th = %d\n", di->ctrl_para.vbatt_th);
		break;
	default:
		break;
	}

	return count;
}

static void vsys_switch_sysfs_create_group(struct device *dev)
{
	power_sysfs_init_attrs(vsys_switch_sysfs_attrs,
		vsys_switch_sysfs_field_tbl, VSYS_SWITCH_SYSFS_ATTRS_SIZE);
	power_sysfs_create_link_group("hw_power", "vsys_switch",
		"vsys_switch_data", dev, &vsys_switch_sysfs_attr_group);
}

static void vsys_switch_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "vsys_switch",
		"vsys_switch_data", dev, &vsys_switch_sysfs_attr_group);
}
#else
static inline void vsys_switch_sysfs_create_group(struct device *dev)
{
}

static inline void vsys_switch_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int vsys_switch_parse_rbatt_sf_lut(struct device_node *np,
	struct vsys_switch_device_info *di)
{
	int ret;
	int i;
	int j;
	long idata = 0;
	const char *data_string = NULL;

	di->rbatt_sf_lut = kzalloc(sizeof(*(di->rbatt_sf_lut)), GFP_KERNEL);
	if (!di->rbatt_sf_lut)
		return -ENOMEM;

	ret = of_property_read_u32(np, "rbatt_sf_rows",
		(unsigned int *)(&(di->rbatt_sf_lut->rows)));
	if (ret) {
		hwlog_err("rbatt_sf_rows dts read failed\n");
		return -EINVAL;
	}
	hwlog_info("rbatt_sf_rows=%d\n", di->rbatt_sf_lut->rows);

	ret = of_property_read_u32(np, "rbatt_sf_cols",
		(unsigned int *)(&(di->rbatt_sf_lut->cols)));
	if (ret) {
		hwlog_err("rbatt_sf_cols dts read failed\n");
		return -EINVAL;
	}
	hwlog_info("rbatt_sf_cols=%d\n", di->rbatt_sf_lut->cols);

	for (i = 0; i < di->rbatt_sf_lut->cols; i++) {
		ret = of_property_read_u32_index(np, "rbatt_sf_soc", i,
			(unsigned int *)(&(di->rbatt_sf_lut->soc[i])));
		if (ret) {
			hwlog_err("rbatt_sf_soc[%d] dts read failed\n", i);
			return -EINVAL;
		}
		hwlog_info("rbatt_sf_soc[%d]=%d\n", i,
			di->rbatt_sf_lut->soc[i]);
	}

	for (i = 0; i < di->rbatt_sf_lut->rows; i++) {
		if (power_dts_read_string_index(power_dts_tag(HWLOG_TAG), np,
			"rbatt_sf_temp", i, &data_string))
			return -EINVAL;

		if (kstrtol(data_string, POWER_BASE_DEC, &idata) < 0) {
			hwlog_err("unable to parse input:%s\n", data_string);
			return -EINVAL;
		}
		di->rbatt_sf_lut->temp[i] = idata;
		hwlog_info("rbatt_sf_temp[%d]=%d\n", i,
			di->rbatt_sf_lut->temp[i]);
	}

	for (i = 0; i < di->rbatt_sf_lut->cols; i++) {
		for (j = 0; j < di->rbatt_sf_lut->rows; j++) {
			ret = of_property_read_u32_index(np, "rbatt_sf_sf",
				j * di->rbatt_sf_lut->cols + i,
			(unsigned int *)(&(di->rbatt_sf_lut->sf[j][i])));
			if (ret) {
				hwlog_err("rbatt_sf_sf[%d][%d] dts read failed\n",
					j, i);
				return -EINVAL;
			}
			hwlog_info("rbatt_sf_sf[%d][%d]=%d\n", j, i,
				di->rbatt_sf_lut->sf[j][i]);
		}
	}

	return ret;
}

static int vsys_switch_parse_rbatt_aging_para(struct device_node *np,
	struct vsys_switch_device_info *di)
{
	int i;
	int string_len;
	long idata = 0;
	const char *data_string = NULL;

	string_len = power_dts_read_count_strings(power_dts_tag(HWLOG_TAG), np,
		"rbatt_aging_para", RBATT_AGING_PARA_LEVEL,
		RBATT_AGING_PARA_INFO_TOTAL);
	if (string_len < 0) {
		di->rbatt_aging_data.total_level = 0;
		return -EINVAL;
	}

	di->rbatt_aging_data.total_level = string_len /
		RBATT_AGING_PARA_INFO_TOTAL;
	di->rbatt_aging_data.rbatt_aging_para = kcalloc(
		di->rbatt_aging_data.total_level,
		sizeof(*(di->rbatt_aging_data.rbatt_aging_para)),
		GFP_KERNEL);
	if (!di->rbatt_aging_data.rbatt_aging_para) {
		di->rbatt_aging_data.total_level = 0;
		hwlog_err("alloc rbatt_aging_para failed\n");
		return -EINVAL;
	}

	for (i = 0; i < string_len; i++) {
		if (power_dts_read_string_index(power_dts_tag(HWLOG_TAG), np,
			"rbatt_aging_para", i, &data_string)) {
			di->rbatt_aging_data.total_level = 0;
			return -EINVAL;
		}

		if (kstrtol(data_string, POWER_BASE_DEC, &idata) < 0) {
			hwlog_err("unable to parse input:%s\n", data_string);
			return -EINVAL;
		}
		switch (i % RBATT_AGING_PARA_INFO_TOTAL) {
		case RBATT_AGING_PARA_INFO_CYCLE:
			di->rbatt_aging_data.rbatt_aging_para[i /
				(RBATT_AGING_PARA_INFO_TOTAL)].cycle =
				(int)idata;
				break;
		case RBATT_AGING_PARA_INFO_COEFFICIENT:
			di->rbatt_aging_data.rbatt_aging_para[i /
				(RBATT_AGING_PARA_INFO_TOTAL)].coefficient =
				(int)idata;
			break;
		default:
			hwlog_err("rbatt_aging_para dts read failed\n");
			break;
		}
	}

	for (i = 0; i < di->rbatt_aging_data.total_level; i++)
		hwlog_info("rbatt_aging_para[%d] cycle=%-4d coeff=%-3d\n",
			i, di->rbatt_aging_data.rbatt_aging_para[i].cycle,
			di->rbatt_aging_data.rbatt_aging_para[i].coefficient);

	return 0;
}

static int vsys_switch_parse_dts(struct device_node *np,
	struct vsys_switch_device_info *di)
{
	int ret;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"vbatt_th", &di->ctrl_para.vbatt_th, VBATT_TH_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"vbatt_base", &di->ctrl_para.vbatt_base, VBATT_BASE_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"vbatt_gap", &di->ctrl_para.vbatt_gap, VBATT_GAP_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"compensate_res", &di->batt_para.comp_res, COMP_RES_DEFAULT);
	(void)power_dts_read_str2int(power_dts_tag(HWLOG_TAG), np,
		"dischg_curr_max", &di->ctrl_para.dischg_curr_max, 0);

	ret = vsys_switch_parse_rbatt_sf_lut(np, di);
	if (ret)
		return -EINVAL;

	ret = vsys_switch_parse_rbatt_aging_para(np, di);
	if (ret)
		return -EINVAL;

	return ret;
}

static void vsys_switch_device_info_free(struct vsys_switch_device_info *di)
{
	if (di) {
		kfree(di->rbatt_sf_lut);
		di->rbatt_sf_lut = NULL;
		kfree(di->rbatt_aging_data.rbatt_aging_para);
		di->rbatt_aging_data.rbatt_aging_para = NULL;
	}
}

static int vsys_switch_probe(struct platform_device *pdev)
{
	int ret;
	struct vsys_switch_device_info *di = NULL;
	struct device_node *np = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_vsys_switch_di = di;
	di->dev = &pdev->dev;
	np = di->dev->of_node;
	di->buck_ops = g_buck_ops;
	di->sc_ops = g_sc_ops;
	di->ovp_ops = g_ovp_switch_ops;
	platform_set_drvdata(pdev, di);

	wakeup_source_init(&di->wakelock,
		"vsys_switch_wakelock");

	ret = vsys_switch_check_ops(di);
	if (ret)
		goto vsys_switch_fail_0;

	ret = vsys_switch_parse_dts(np, di);
	if (ret)
		goto vsys_switch_fail_0;

	INIT_WORK(&di->sc_event_work, vsys_switch_sc_event_work);
	INIT_DELAYED_WORK(&di->ctrl_work, vsys_switch_control_work);
	di->sc_event_nb.notifier_call = vsys_switch_sc_event_notifier_call;
	ret = atomic_notifier_chain_register(&vsys_sc_event_nh,
		&di->sc_event_nb);
	if (ret < 0) {
		hwlog_err("register sc_event notifier failed\n");
		goto vsys_switch_fail_0;
	}

	if (di->sc_ops->chip_init)
		di->sc_ops->chip_init();

	vsys_switch_para_init(di);
	vsys_switch_check_support_sc_chnl(di);
	schedule_delayed_work(&di->ctrl_work, msecs_to_jiffies(0));

	vsys_switch_sysfs_create_group(di->dev);

#ifdef CONFIG_HUAWEI_POWER_DEBUG
	power_dbg_ops_register("vsys_switch", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_switch_vsys,
		(power_dbg_store)vsys_switch_dbg_store_switch_vsys);
	power_dbg_ops_register("vsys_en_sc", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_enable_sc,
		(power_dbg_store)vsys_switch_dbg_store_enable_sc);
	power_dbg_ops_register("vsys_en_buck", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_enable_buck,
		(power_dbg_store)vsys_switch_dbg_store_enable_buck);
	power_dbg_ops_register("vbatt_th", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_vbatt_threshold,
		(power_dbg_store)vsys_switch_dbg_store_vbatt_threshold);
	power_dbg_ops_register("vsys_vol", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_vsys_volt,
		(power_dbg_store)vsys_switch_dbg_store_vsys_volt);
	power_dbg_ops_register("vsys_chnl", platform_get_drvdata(pdev),
		(power_dbg_show)vsys_switch_dbg_show_vsys_chnl,
		(power_dbg_store)vsys_switch_dbg_store_vsys_chnl);
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

	return 0;

vsys_switch_fail_0:
	di->buck_ops = NULL;
	di->sc_ops = NULL;
	di->ovp_ops = NULL;
	vsys_switch_device_info_free(di);
	wakeup_source_trash(&di->wakelock);
	devm_kfree(&pdev->dev, di);
	g_vsys_switch_di = NULL;
	platform_set_drvdata(pdev, NULL);

	return ret;
}

static int vsys_switch_remove(struct platform_device *pdev)
{
	struct vsys_switch_device_info *di = platform_get_drvdata(pdev);

	if (!di)
		return -ENODEV;

	atomic_notifier_chain_unregister(&vsys_sc_event_nh, &di->sc_event_nb);
	cancel_delayed_work_sync(&di->ctrl_work);
	vsys_switch_sysfs_remove_group(di->dev);

	if (!di->buck_ops) {
		di->buck_ops = NULL;
		g_buck_ops = NULL;
	}

	if (!di->sc_ops) {
		di->sc_ops = NULL;
		g_sc_ops = NULL;
	}

	if (!di->ovp_ops) {
		di->ovp_ops = NULL;
		g_ovp_switch_ops = NULL;
	}

	vsys_switch_device_info_free(di);
	wakeup_source_trash(&di->wakelock);
	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, di);
	g_vsys_switch_di = NULL;

	return 0;
}

static void vsys_switch_shutdown(struct platform_device *pdev)
{
	struct vsys_switch_device_info *di = platform_get_drvdata(pdev);

	hwlog_info("shutdown begin\n");

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	atomic_notifier_chain_unregister(&vsys_sc_event_nh, &di->sc_event_nb);
	di->ctrl_para.switch_vsys_chnl = SWITCH_VSYS_BUCK;
	di->cancel_ctrl_work_flag = true;
	cancel_delayed_work_sync(&di->ctrl_work);

	if (di->state_info.curr_vsys_chnl != VSYS_BUCK_OPEN)
		vsys_switch_enable_buck_channel_only(di);

	hwlog_info("shutdown end\n");
}

#ifdef CONFIG_PM
static int vsys_switch_suspend(struct platform_device *pdev,
	pm_message_t state)
{
	struct vsys_switch_device_info *di = platform_get_drvdata(pdev);

	hwlog_info("suspend begin\n");

	if (!di)
		return 0;

	cancel_delayed_work_sync(&di->ctrl_work);

	hwlog_info("suspend end\n");
	return 0;
}

static int vsys_switch_resume(struct platform_device *pdev)
{
	struct vsys_switch_device_info *di = platform_get_drvdata(pdev);

	hwlog_info("resume begin\n");

	if (!di)
		return 0;

	vsys_switch_para_reset(di);
	schedule_delayed_work(&di->ctrl_work, msecs_to_jiffies(0));

	hwlog_info("resume end\n");
	return 0;
}
#endif /* CONFIG_PM */

static const struct of_device_id vsys_switch_match_table[] = {
	{
		.compatible = "huawei,vsys_switch",
		.data = NULL,
	},
	{},
};

static struct platform_driver vsys_switch_driver = {
	.probe = vsys_switch_probe,
	.remove = vsys_switch_remove,
#ifdef CONFIG_PM
	.suspend = vsys_switch_suspend,
	.resume = vsys_switch_resume,
#endif /* CONFIG_PM */
	.shutdown = vsys_switch_shutdown,
	.driver = {
		.name = "huawei,vsys_switch",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(vsys_switch_match_table),
	},
};

static int __init vsys_switch_init(void)
{
	return platform_driver_register(&vsys_switch_driver);
}

static void __exit vsys_switch_exit(void)
{
	platform_driver_unregister(&vsys_switch_driver);
}

late_initcall(vsys_switch_init);
module_exit(vsys_switch_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("vsys switch module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
