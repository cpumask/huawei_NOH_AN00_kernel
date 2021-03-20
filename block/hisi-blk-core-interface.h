/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hisi block core interface
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

#ifndef __HISI_BLK_CORE_INTERFACE_H__
#define __HISI_BLK_CORE_INTERFACE_H__
#include <linux/blkdev.h>

extern void _cfi_hisi_blk_queue_usr_ctrl_recovery_timer_expire(
	unsigned long data);
extern void hisi_blk_queue_usr_ctrl_recovery_timer_expire(unsigned long data);
extern ssize_t __cfi_hisi_queue_status_show(
	struct request_queue *q, char *page);
extern ssize_t hisi_queue_status_show(
	const struct request_queue *q, char *page, unsigned long len);
#if defined(CONFIG_HISI_DEBUG_FS) || defined(CONFIG_HISI_BLK_DEBUG)
extern ssize_t __cfi_hisi_queue_io_prio_sim_show(
	struct request_queue *q, char *page);
extern ssize_t hisi_queue_io_prio_sim_show(
	const struct request_queue *q, char *page);
extern ssize_t __cfi_hisi_queue_io_prio_sim_store(
	struct request_queue *q, const char *page, size_t count);
extern ssize_t hisi_queue_io_prio_sim_store(
	struct request_queue *q, const char *page, size_t count);
#endif
#endif
