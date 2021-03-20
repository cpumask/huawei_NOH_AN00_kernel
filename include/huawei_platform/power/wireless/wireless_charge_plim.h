/*
 * wireless_charge_plim.h
 *
 * wireless charger power limit module
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

#ifndef _WIRELESS_CHARGE_PLIM_H_
#define _WIRELESS_CHARGE_PLIM_H_

#define PLIM_SRC_NAME_LEN    32
#define NORMAL_FOP_MAX             150
#define VOUT_9V_STAGE_MAX          10500
#define VOUT_9V_STAGE_MIN          7500

enum wlc_plim_src {
	WLC_PLIM_SRC_BEGIN = 0,
	WLC_PLIM_SRC_OTG = WLC_PLIM_SRC_BEGIN,
	WLC_PLIM_SRC_RPP,
	WLC_PLIM_SRC_FAN,
	WLC_PLIM_SRC_VOUT_ERR,
	WLC_PLIM_TX_ALARM,
	WLC_PLIM_TX_BST_ERR,
	WLC_PLIM_KB,
	WLC_PLIM_THERMAL,
	WLC_PLIM_FIX_FOP,
	WLC_PLIM_SRC_END,
};

struct wlc_plim_para {
	enum wlc_plim_src src_id;
	char src_name[PLIM_SRC_NAME_LEN];
	bool need_rst; /* reset limit para when wireless para init */
	int tx_vout; /* mV */
	int rx_vout; /* mV */
	int rx_iout; /* mA */
};

struct wlc_plim {
	unsigned long src;
};

#ifdef CONFIG_WIRELESS_CHARGER
int wlc_get_plimit_src(void);
void wlc_set_plimit_src(int src_id);
void wlc_clear_plimit_src(int src_id);
void wlc_reset_plimit(void);
void wlc_update_plimit_para(void);
#else
static inline int wlc_get_plimit_src(void) { }
static inline void wlc_set_plimit_src(int src_id) { }
static inline void wlc_clear_plimit_src(int src_id) { }
static inline void wlc_reset_plimit(void) { }
static inline void wlc_update_plimit_para(void) { }
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_CHARGE_PLIM_H_ */
