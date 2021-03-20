/*
 * wifi_audio_ptp.h
 *
 * wifi audio ptp time sync kernel module implementation
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

#ifndef _WIFI_AUDIO_PTP_H_
#define _WIFI_AUDIO_PTP_H_

#include <linux/netdevice.h>
#include "wifi_audio.h"

int netlink_msg_ptp_handle(const struct netlink_data *msg_rev);
int wifi_audio_time_sync_init(void);
void wifi_audio_time_sync_uninit(void);
void ptp_clear_by_dev(const struct net_device *dev);
#endif /* _WIFI_AUDIO_PTP_H_ */

