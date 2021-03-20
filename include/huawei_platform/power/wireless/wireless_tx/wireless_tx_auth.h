/*
 * wireless_tx_auth.h
 *
 * wireless tx auth headfile for wireless reverse charging
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

#ifndef _WIRELESS_TX_AUTH_H_
#define _WIRELESS_TX_AUTH_H_

/* tx rx length */
#define WLTX_RX_RANDOM_LEN              8
#define WLTX_TX_HASH_LEN                8
#define WLTX_AF_WAIT_CT_TIMEOUT         2000 /* ms */

enum wireless_tx_auth_sysfs_type {
	WLTX_AUTH_SYSFS_AF,
};

struct wireless_tx_auth_info {
	struct device *dev;
	struct completion wltx_auth_af_completion;
};

int wireless_tx_auth_get_tx_hash(u8 *rx_random, u8 rx_random_len);

#endif /* _WIRELESS_TX_AUTH_H_ */
