/*
 * wireless_charge_plim.c
 *
 * wireless charge driver, function as power limit
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
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/wireless_charger.h>

#define HWLOG_TAG wireless_plim
HWLOG_REGIST();

static struct wlc_plim g_plim;
static struct wlc_plim_para const g_plim_tbl[WLC_PLIM_SRC_END] = {
	{ WLC_PLIM_SRC_OTG,      "otg",        false, 5000,  5500,  1000 },
	{ WLC_PLIM_SRC_RPP,      "rpp",        true,  12000, 12000, 1300 },
	{ WLC_PLIM_SRC_FAN,      "fan",        true,  9000,  9900,  1250 },
	{ WLC_PLIM_SRC_VOUT_ERR, "vout_err",   true,  9000,  9900,  1250 },
	{ WLC_PLIM_TX_ALARM,     "tx_alarm",   true,  12000, 12000, 1300 },
	{ WLC_PLIM_TX_BST_ERR,   "tx_bst_err", true,  5000,  5500,  1000 },
	{ WLC_PLIM_KB,           "keyboard",   true,  9000,  9900,  1100 },
	{ WLC_PLIM_THERMAL,      "thermal",    false, 9000,  9900,  1250 },
};

int wlc_get_plimit_src(void)
{
	return g_plim.src;
}

void wlc_set_plimit_src(int src_id)
{
	if ((src_id < WLC_PLIM_SRC_BEGIN) || (src_id >= WLC_PLIM_SRC_END))
		return;

	if (test_bit(src_id, &g_plim.src))
		return;
	set_bit(src_id, &g_plim.src);
	if (src_id != g_plim_tbl[src_id].src_id)
		return;
	hwlog_info("set_plimit_src: %s\n", g_plim_tbl[src_id].src_name);
}

void wlc_clear_plimit_src(int src_id)
{
	if ((src_id < WLC_PLIM_SRC_BEGIN) || (src_id >= WLC_PLIM_SRC_END))
		return;

	if (!test_bit(src_id, &g_plim.src))
		return;
	clear_bit(src_id, &g_plim.src);
	if (src_id != g_plim_tbl[src_id].src_id)
		return;
	hwlog_info("clear_plimit_src: %s\n", g_plim_tbl[src_id].src_name);
}

void wlc_reset_plimit(void)
{
	int i;

	for (i = WLC_PLIM_SRC_BEGIN; i < WLC_PLIM_SRC_END; i++) {
		if (g_plim_tbl[i].need_rst)
			clear_bit(i, &g_plim.src);
	}
}

void wlc_update_plimit_para(void)
{
	int i;
	struct wireless_charge_device_info *di = wlc_get_dev_info();

	if (!di)
		return;

	for (i = WLC_PLIM_SRC_BEGIN; i < WLC_PLIM_SRC_END; i++) {
		if (!test_bit(i, &g_plim.src))
			continue;
		di->tx_vout_max = min(di->tx_vout_max, g_plim_tbl[i].tx_vout);
		di->rx_vout_max = min(di->rx_vout_max, g_plim_tbl[i].rx_vout);
		di->rx_iout_max = min(di->rx_iout_max, g_plim_tbl[i].rx_iout);
	}
}
