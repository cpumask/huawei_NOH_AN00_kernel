/*
 * wireless_tx_cap.h
 *
 * tx capability headfile for wireless reverse charging
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

#ifndef _WIRELESS_TX_CAP_H_
#define _WIRELESS_TX_CAP_H_

enum wltx_cap_info {
	WLC_TX_CAP_TYPE = 0,
	WLC_TX_CAP_VMAX,
	WLC_TX_CAP_IMAX,
	WLC_TX_CAP_ATTR,
	WLC_TX_CAP_TOTAL,
};

struct wltx_cap {
	u8 type;
	int vout;
	int iout;
	u8 attr;
};

enum wltx_cap_level {
	WLTX_DFLT_CAP = 0,
	WLTX_HIGH_PWR_CAP,
	WLTX_TOTAL_CAP,
};

struct wltx_cap_data {
	int exp_id; /* expect cap index */
	int cur_id;
	int cap_level;
	struct wltx_cap cap_para[WLTX_TOTAL_CAP];
};

#ifdef CONFIG_WIRELESS_CHARGER
void wltx_send_tx_cap(void);
void wltx_reset_exp_tx_cap_id(void);
u8 wltx_get_tx_cap_type(void);
#else
static inline void wltx_send_tx_cap(void) {}
static inline void wltx_reset_exp_tx_cap_id(void) {}

static inline u8 wltx_get_tx_cap_type(void)
{
	return 0;
}
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_TX_CAP_H_ */
