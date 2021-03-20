/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hisi block io latency interface
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

#include <linux/kernel.h>
#include <linux/blkdev.h>

#include "hisi-blk-latency-interface.h"

void __hisi_blk_latency_check_timer_expire(unsigned long data)
{
	hisi_blk_latency_check_timer_expire(data);
}

ssize_t __hisi_queue_io_latency_warning_threshold_store(
	struct request_queue *q, const char *page, size_t count)
{
	return hisi_queue_io_latency_warning_threshold_store(q, page, count);
}

