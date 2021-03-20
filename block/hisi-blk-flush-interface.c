/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  hisi block flush reduce interface
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hisi-blk-flush-interface.h"

void __cfi_hisi_blk_flush_work_fn(struct work_struct *work)
{
	hisi_blk_flush_work_fn(work);
}

int __cfi_hisi_blk_poweroff_flush_notifier_call(
	struct notifier_block *powerkey_nb, unsigned long event, void *data)
{
	return hisi_blk_poweroff_flush_notifier_call(powerkey_nb, event, data);
}

int __init __cfi_hisi_blk_flush_init(void)
{
	return hisi_blk_flush_init();
}

