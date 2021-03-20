/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: hisi block busy idle interface
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

#include "hisi-blk-busy-idle-interface.h"

void __cfi_hisi_blk_busyidle_handler_latency_check_timer_expire(
	unsigned long data)
{
	hisi_blk_busyidle_handler_latency_check_timer_expire(data);
}

int __cfi_hisi_blk_busyidle_notify_handler(
	struct notifier_block *nb, unsigned long val, void *v)
{
	return hisi_blk_busyidle_notify_handler(nb, val, v);
}

void __cfi_hisi_blk_idle_notify_work(struct work_struct *work)
{
	hisi_blk_idle_notify_work(work);
}

void __cfi_hisi_blk_busyidle_end_rq(struct request *rq, blk_status_t error)
{
	hisi_blk_busyidle_end_rq(rq, error);
}

#if defined(CONFIG_HISI_DEBUG_FS) || defined(CONFIG_HISI_BLK_DEBUG)
ssize_t __cfi_hisi_queue_busyidle_enable_store(
	struct request_queue *q, const char *page, size_t count)
{
	return hisi_queue_busyidle_enable_store(q, page, count);
}

ssize_t __cfi_hisi_queue_busyidle_statistic_reset_store(
	struct request_queue *q, const char *page, size_t count)
{
	return hisi_queue_busyidle_statistic_reset_store(q, page, count);
}

ssize_t __cfi_hisi_queue_busyidle_statistic_show(
	struct request_queue *q, char *page)
{
	return hisi_queue_busyidle_statistic_show(q, page, PAGE_SIZE);
}

ssize_t __cfi_hisi_queue_hw_idle_enable_show(
	struct request_queue *q, char *page)
{
	return hisi_queue_hw_idle_enable_show(q, page, PAGE_SIZE);
}

ssize_t __cfi_hisi_queue_idle_state_show(struct request_queue *q, char *page)
{
	return hisi_queue_idle_state_show(q, page, PAGE_SIZE);
}
#endif /* CONFIG_HISI_BLK_DEBUG */

