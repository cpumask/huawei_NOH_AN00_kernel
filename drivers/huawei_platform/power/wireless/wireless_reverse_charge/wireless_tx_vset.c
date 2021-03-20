/*
 * wireless_tx_vset.c
 *
 * set tx_vin for wireless reverse charging
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
#include <huawei_platform/power/hardware_ic/charge_pump.h>
#include <huawei_platform/power/wireless_transmitter.h>
#include <huawei_platform/power/wireless_power_supply.h>
#include <huawei_platform/power/wireless/wireless_tx/wireless_tx_pwr_src.h>

#define HWLOG_TAG wireless_tx_vset
HWLOG_REGIST();

static void wltx_set_tx_fod_coef(struct wireless_tx_device_info *di)
{
	int ret;

	if (!di || !di->tx_ops || !di->tx_ops->set_tx_fod_coef)
		return;

	if ((di->tx_vset.para[di->tx_vset.cur].pl_th <= 0) ||
		(di->tx_vset.para[di->tx_vset.cur].pl_cnt <= 0))
		return;

	ret = di->tx_ops->set_tx_fod_coef(
		di->tx_vset.para[di->tx_vset.cur].pl_th,
		di->tx_vset.para[di->tx_vset.cur].pl_cnt);
	if (ret) {
		hwlog_err("set_tx_fod_coef: failed\n");
		return;
	}

	hwlog_info("[set_tx_fod_coef] succ\n");
}

static const char *wltx_bridge_name(enum wltx_bridge_type type)
{
	static const char *const bridge_name[] = {
		[WLTX_PING_FULL_BRIDGE] = "ping in full bridge mode",
		[WLTX_PING_HALF_BRIDGE] = "ping in half bridge mode",
		[WLTX_PT_FULL_BRIDGE] = "power transfer in full bridge mode",
		[WLTX_PT_HALF_BRIDGE] = "power transfer in half bridge mode"
	};

	if ((type < WLTX_BRIDGE_BEGIN) || (type >= WLTX_BRIDGE_END))
		return "invalid bridge mode";

	return bridge_name[type];
}

static int wltx_set_bridge(enum wltx_bridge_type type)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di || !di->tx_ops || !di->tx_ops->set_bridge)
		return -EPERM;

	hwlog_info("[set_tx_bridge] %s\n", wltx_bridge_name(type));
	return di->tx_ops->set_bridge(type);
}

static bool wltx_is_half_volt(int vin, int vout)
{
	return vout * 100 < vin * 75; /* true: vout/vin<75%, otherwise false */
}

static enum wltx_bridge_type wltx_select_ping_bridge(int cur_vset, int vout)
{
	if (wltx_is_half_volt(cur_vset, vout))
		return WLTX_PING_HALF_BRIDGE;

	return WLTX_PING_FULL_BRIDGE;
}

static enum wltx_bridge_type wltx_select_pt_bridge(int cur_vset, int vout)
{
	if (wltx_is_half_volt(cur_vset, vout))
		return WLTX_PT_HALF_BRIDGE;

	return WLTX_PT_FULL_BRIDGE;
}

static enum wltx_bridge_type wltx_select_bridge(int cur_vset,
	enum wltx_stage stage, int vout)
{
	if (stage == WL_TX_STAGE_PING_RX)
		return wltx_select_ping_bridge(cur_vset, vout);

	return wltx_select_pt_bridge(cur_vset, vout);
}

static void wltx_set_tx_vout(struct wireless_tx_device_info *di,
	enum wltx_stage stage, int vout, bool force)
{
	int cur_vset;
	enum wltx_bridge_type type;

	cur_vset = di->tx_vset.para[di->tx_vset.cur].vset;
	type = wltx_select_bridge(cur_vset, stage, vout);
	if (!force && (type == di->tx_vset.bridge))
		return;

	if (wltx_set_bridge(type))
		return;

	di->tx_vset.bridge = type;
}

static int wltx_recalc_tx_vset(struct wireless_tx_device_info *di, int tx_vset)
{
	const char *src = NULL;

	if (wltx_is_tx_open_by_hall())
		return HALL_TX_VOUT;

	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return tx_vset;

	src = wltx_get_pwr_src_name(di->cur_pwr_src);
	if (strstr(src, "CP"))
		return tx_vset;

	tx_vset /= CP_CP_RATIO;
	hwlog_info("[recalc_tx_vset] tx_vset=%dmV\n", tx_vset);

	return tx_vset;
}

static void wltx_set_tx_vset_by_id(struct wireless_tx_device_info *di,
	int id, bool force_flag)
{
	int tx_vset;

	if ((id < 0) || (id >= di->tx_vset.total))
		return;

	if (di->pwr_type == WL_TX_PWR_5VBST_VBUS_OTG_CP) {
		di->tx_vset.cur = id;
		return;
	}

	tx_vset = di->tx_vset.para[id].vset;
	if (!force_flag && (tx_vset == di->tx_vset.para[di->tx_vset.cur].vset))
		return;
	if (wlps_tx_mode_vset(tx_vset))
		return;
	if (di->tx_vset.para[id].ext_hdl == WLTX_EXT_HDL_BP2CP)
		wltx_set_pwr_src_output(true, PWR_SRC_BP2CP);

	di->tx_vset.cur = id;
}

static void wltx_set_tx_vset(struct wireless_tx_device_info *di,
	int tx_vset, bool force_flag)
{
	int i;

	if (di->tx_vset.cur < 0)
		return;
	if (di->tx_vset.cur >= WLTX_TX_VSET_TYPE_MAX)
		return;

	tx_vset = wltx_recalc_tx_vset(di, tx_vset);
	for (i = 0; i < di->tx_vset.total; i++) {
		if ((tx_vset >= di->tx_vset.para[i].rx_vmin) &&
			(tx_vset < di->tx_vset.para[i].rx_vmax))
			break;
	}
	if (i >= di->tx_vset.total) {
		hwlog_err("set_tx_vset: tx_vset=%dmV mismatch\n", tx_vset);
		return;
	}

	wltx_set_tx_vset_by_id(di, i, force_flag);
	wltx_set_tx_fod_coef(di);
}

void wltx_set_tx_volt(enum wltx_stage stage, int vtx, bool force)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	if (di->pwr_type != WL_TX_PWR_5VBST_VBUS_OTG_CP)
		return wltx_set_tx_vset(di, vtx, force);

	if (stage == WL_TX_STAGE_POWER_SUPPLY)
		return wltx_set_tx_vset(di, vtx, force);

	if (stage >= WL_TX_STAGE_PING_RX)
		return wltx_set_tx_vout(di, stage, vtx, force);
}

void wltx_reset_vset_para(void)
{
	struct wireless_tx_device_info *di = wltx_get_dev_info();

	if (!di)
		return;

	if (di->rx_disc_pwr_on) {
		di->rx_disc_pwr_on = false;
		return;
	}

	di->tx_vset.cur = 0;
	di->tx_vset.bridge = -1;
}
