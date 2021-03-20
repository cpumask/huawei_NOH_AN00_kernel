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

#ifndef __HISI_BLK_LATENCY_INTERFACE_H__
#define __HISI_BLK_LATENCY_INTERFACE_H__
#include <linux/blk-mq.h>

#include "blk.h"
#include "hisi-blk-latency-interface.h"

extern const struct bio_delay_stage_config bio_stage_cfg[BIO_PROC_STAGE_MAX];
extern const struct req_delay_stage_config req_stage_cfg[REQ_PROC_STAGE_MAX];

extern void hisi_blk_latency_check_timer_expire(unsigned long data);
extern void __hisi_blk_latency_check_timer_expire(unsigned long data);
extern ssize_t hisi_queue_io_latency_warning_threshold_store(
	const struct request_queue *q, const char *page, size_t count);
extern ssize_t __hisi_queue_io_latency_warning_threshold_store(
	struct request_queue *q, const char *page, size_t count);
ssize_t hisi_queue_tz_write_bytes_show(struct request_queue *q, char *page);

#endif /* __HISI_BLK_LATENCY_INTERFACE_H__ */
