/*
 * wireless_tx_cap.c
 *
 * tx capability for wireless reverse charging
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

#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/wireless_charger.h>
#include <huawei_platform/power/wireless/wireless_protocol.h>
#include <huawei_platform/power/wireless_transmitter.h>

#define HWLOG_TAG wireless_tx_cap
HWLOG_REGIST();

u8 wltx_get_tx_cap_type(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return -EPERM;

	return di->tx_cap.cap_para[di->tx_cap.exp_id].type;
}

static void wltx_cap_set_tx_type(u8 *tx_cap, u8 type)
{
	int wired_ch_state = wireless_charge_get_wired_channel_state();

	if ((type != WIRELESS_OTG_A) && (type != WIRELESS_OTG_B)) {
		tx_cap[WLC_TX_CAP_TYPE] = type;
	} else {
		if (wired_ch_state == WIRED_CHANNEL_ON)
			tx_cap[WLC_TX_CAP_TYPE] = WIRELESS_OTG_B;
		else
			tx_cap[WLC_TX_CAP_TYPE] = WIRELESS_OTG_A;
	}

	hwlog_info("[tx_cap_type] type:0x%x\n", tx_cap[WLC_TX_CAP_TYPE]);
}

static void wltx_cap_set_tx_vmax(u8 *tx_cap, int vmax)
{
	/* 100mV in unit according to private qi protocol */
	tx_cap[WLC_TX_CAP_VMAX] = (u8)(vmax / 100);

	hwlog_info("[tx_cap_vmax] vmax:%dmV\n", vmax);
}

static void wltx_cap_set_tx_imax(u8 *tx_cap, int imax)
{
	enum wltx_pwr_type pwr_type;
	const char *src_name = NULL;

	pwr_type = wltx_get_pwr_type();
	src_name = wltx_get_pwr_src_name(wltx_get_cur_pwr_src());
	if ((pwr_type == WL_TX_PWR_5VBST_VBUS_OTG_CP) &&
		!strstr(src_name, "CP"))
		imax = 500; /* 500mA imax for 5vbst/vbus/otg pwr_src */

	/* 100mA in unit according to private qi protocol */
	tx_cap[WLC_TX_CAP_IMAX] = (u8)(imax / 100);

	hwlog_info("[tx_cap_imax] imax:%dmA\n", imax);
}

static void wltx_cap_set_tx_attr(u8 *tx_cap, u8 attr)
{
	tx_cap[WLC_TX_CAP_ATTR] = attr;

	hwlog_info("[tx_cap_attr] attr:0x%x\n", attr);
}

static void wltx_revise_exp_tx_cap_id(struct wireless_tx_device_info *di)
{
	struct wltx_pwr_ctrl_info *info = wltx_get_pwr_ctrl_info();

	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return;
	if (info && (info->charger_type != WLTX_SC_HI_PWR_CHARGER))
		di->tx_cap.exp_id = WLTX_DFLT_CAP;
}

static int wltx_set_tx_cap(u8 *tx_cap, int len)
{
	int cap_id;
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return -EPERM;

	wltx_revise_exp_tx_cap_id(di);
	cap_id = di->tx_cap.exp_id;
	if ((cap_id < 0) || (cap_id >= WLTX_TOTAL_CAP))
		return -EINVAL;

	wltx_cap_set_tx_type(tx_cap, di->tx_cap.cap_para[cap_id].type);
	wltx_cap_set_tx_vmax(tx_cap, di->tx_cap.cap_para[cap_id].vout);
	wltx_cap_set_tx_imax(tx_cap, di->tx_cap.cap_para[cap_id].iout);
	wltx_cap_set_tx_attr(tx_cap, di->tx_cap.cap_para[cap_id].attr);

	return 0;
}

void wltx_send_tx_cap(void)
{
	u8 tx_cap[WLC_TX_CAP_TOTAL] = { WIRELESS_OTG_A, 0 };

	if (wltx_set_tx_cap(tx_cap, WLC_TX_CAP_TOTAL))
		return;

	hwlog_info("[send_tx_cap] type=0x%x vmax=0x%x imax=0x%x attr=0x%x\n",
		tx_cap[WLC_TX_CAP_TYPE], tx_cap[WLC_TX_CAP_VMAX],
		tx_cap[WLC_TX_CAP_IMAX], tx_cap[WLC_TX_CAP_ATTR]);
	(void)wireless_send_tx_capability(WIRELESS_PROTOCOL_QI,
		tx_cap, WLC_TX_CAP_TOTAL);
}

static bool wltx_support_high_pwr_cap(struct wireless_tx_device_info *di)
{
	int soc;
	struct wltx_pwr_ctrl_info *info = NULL;

	soc = hisi_battery_capacity();
	if (soc < WL_TX_HI_PWR_SOC_MIN)
		return false;

	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return true;

	info = wltx_get_pwr_ctrl_info();
	if (info && (info->charger_type == WLTX_SC_HI_PWR_CHARGER))
		return true;

	return false;
}

void wltx_reset_exp_tx_cap_id(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	if (wltx_is_tx_open_by_hall()) {
		di->tx_cap.exp_id = WLTX_DFLT_CAP;
		return;
	}

	if (di->tx_cap.cap_level <= 1) { /* only default cap, 5v */
		di->tx_cap.exp_id = WLTX_DFLT_CAP;
		return;
	}

	if (wltx_support_high_pwr_cap(di)) {
		di->tx_cap.exp_id = WLTX_HIGH_PWR_CAP;
		return;
	}

	di->tx_cap.exp_id = WLTX_DFLT_CAP;
}
