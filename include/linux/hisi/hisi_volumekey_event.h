/*
  * hisi volume key notifier head file
  *
  * Copyright (C) Huawei Technologies Co., Ltd. 2016. All rights reserved.
  *
  * This software is licensed under the terms of the GNU General Public
  * License version 2, as published by the Free Software Foundation, and
  * may be copied, distributed, and modified under those terms.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  */

#ifndef _HISI_VOLUMEKEY_EVENT_H
#define _HISI_VOLUMEKEY_EVENT_H
#include  <linux/notifier.h>


typedef enum {
	HISI_VOLUMEKEY_DOWN_PRESS = 0,
	HISI_VOLUMEKEY_DOWN_RELEASE,
	HISI_VOLUMEKEY_UP_PRESS,
	HISI_VOLUMEKEY_UP_RELEASE,
	HISI_PVOLUMEKEY_MAX
} hisi_volumekey_event_t;

#ifdef  CONFIG_KEYBOARD_HISI_PMIC_GPIO_KEY
int hisi_volumekey_register_notifier(struct notifier_block *nb);
int hisi_volumekey_unregister_notifier(struct notifier_block *nb);
int hisi_call_volumekey_notifiers(unsigned long val,void *v);
#endif

#endif
