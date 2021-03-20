/*
  * hisi volume key notifier code
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

#include <linux/export.h>
#include <linux/hisi/hisi_volumekey_event.h>
#include <linux/notifier.h>

static ATOMIC_NOTIFIER_HEAD(hisi_volumekey_notifier_list);

int hisi_volumekey_register_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&hisi_volumekey_notifier_list, nb);
}

int hisi_volumekey_unregister_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(
		&hisi_volumekey_notifier_list, nb);
}

int hisi_call_volumekey_notifiers(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&hisi_volumekey_notifier_list,
		val, v);
}
