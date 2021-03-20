/*
 * wlc_wp.h
 *
 * common interface, varibles, definition etc for wireless charge test
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

#ifndef _WLC_WP_H_
#define _WLC_WP_H_

#define WLDC_WP_CHK_INTERVAL        100   /* ms */
#define WLDC_WP_DEFLT_CP_IOUT_TH    0

enum wlc_wp_test_para {
	WLC_WP_PARA_BEGIN = 0,
	WLC_WP_PARA_CP_IOUT_TH = WLC_WP_PARA_BEGIN,
	WLC_WP_PARA_MAX,
};

struct wlc_wp_para {
	int cp_iout_th;
};

struct wlc_wp_data {
	struct notifier_block wlc_wp_nb;
	struct delayed_work wldc_wp_chk_work;
	struct wlc_wp_para dts_para;
	int dc_type;
};

#endif /* _WLC_WP_H_ */
