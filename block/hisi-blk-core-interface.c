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

#include "hisi-blk-core-interface.h"

void _cfi_hisi_blk_queue_usr_ctrl_recovery_timer_expire(unsigned long data)
{
	hisi_blk_queue_usr_ctrl_recovery_timer_expire(data);
}

ssize_t __cfi_hisi_queue_status_show(struct request_queue *q, char *page)
{
	return hisi_queue_status_show(q, page, PAGE_SIZE);
}

#if defined(CONFIG_HISI_DEBUG_FS) || defined(CONFIG_HISI_BLK_DEBUG)
ssize_t __cfi_hisi_queue_io_prio_sim_show(struct request_queue *q, char *page)
{
	return hisi_queue_io_prio_sim_show(q, page);
}

ssize_t __cfi_hisi_queue_io_prio_sim_store(
	struct request_queue *q, const char *page, size_t count)
{
	return hisi_queue_io_prio_sim_store(q, page, count);
}
#endif
