/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:  hisi block dump interface
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

#ifndef __HISI_BLK_DUMP_INTERFACE_H__
#define __HISI_BLK_DUMP_INTERFACE_H__
#include <linux/kernel.h>
#include <linux/blkdev.h>

extern int __cfi_hisi_blk_panic_notify(
	struct notifier_block *nb, unsigned long event, void *buf);
extern int hisi_blk_panic_notify(
	struct notifier_block *nb, unsigned long event, void *buf);

#endif /* __HISI_BLK_DUMP_INTERFACE_H__ */
