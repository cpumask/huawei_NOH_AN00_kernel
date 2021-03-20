/*
 * power_event_ne.h
 *
 * notifier event for power module
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

#ifndef _POWER_EVENT_NE_H_
#define _POWER_EVENT_NE_H_

#define POWER_EVENT_NOTIFY_SIZE 1024
#define POWER_EVENT_NOTIFY_NUM  2

/*
 * define notifier event for power module
 * NE is simplified identifier with notifier event
 */
enum power_event_ne_list {
	POWER_EVENT_NE_BEGIN = 0,
	/* section: for send uevent form kernel to user */
	POWER_EVENT_NE_USB_DISCONNECT = POWER_EVENT_NE_BEGIN,
	POWER_EVENT_NE_USB_CONNECT,
	POWER_EVENT_NE_WIRELESS_DISCONNECT,
	POWER_EVENT_NE_WIRELESS_CONNECT,
	POWER_EVENT_NE_WIRELESS_REVERSE_START,
	POWER_EVENT_NE_WIRELESS_REVERSE_STOP,
	POWER_EVENT_NE_VBUS_ABSENT,
	POWER_EVENT_NE_AUTH_DC_SC,
	POWER_EVENT_NE_AUTH_WL_SC,
	POWER_EVENT_NE_AUTH_WL_SC_UVDM,
	POWER_EVENT_NE_AUTH_WLTX,
	POWER_EVENT_NE_FW_UPDATE_WIRELESS,
	POWER_EVENT_NE_FW_UPDATE_ADAPTER,
	POWER_EVENT_NE_UI_CABLE_TYPE,
	POWER_EVENT_NE_UI_MAX_POWER,
	POWER_EVENT_NE_UI_WL_OFF_POS,
	POWER_EVENT_NE_UI_WL_FAN_STATUS,
	POWER_EVENT_NE_UI_WL_COVER_STATUS,
	POWER_EVENT_NE_UI_WATER_STATUS,
	POWER_EVENT_NE_UI_HEATING_STATUS,
	POWER_EVENT_NE_UI_ICON_TYPE,
	POWER_EVENT_NE_HEATING_START,
	POWER_EVENT_NE_HEATING_STOP,
	/* section: for start & stop charging */
	POWER_EVENT_NE_START_CHARGING,
	POWER_EVENT_NE_STOP_CHARGING,
	/* section: for soc decimal */
	POWER_EVENT_NE_SOC_DECIMAL_DC,
	POWER_EVENT_NE_SOC_DECIMAL_WL_DC,
	/* section: for bsoh uevent */
	POWER_EVENT_NE_BSOH_TYPE,
	POWER_EVENT_NE_END,
};

struct power_event_notify_data {
	const char *event;
	int event_len;
};

int power_event_notifier_chain_register(struct notifier_block *nb);
int power_event_notifier_chain_unregister(struct notifier_block *nb);
void power_event_notify(unsigned long event, void *data);

#endif /* _POWER_EVENT_NE_H_ */
