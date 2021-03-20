/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description:  hisi block function test
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

#ifndef __HISI_BLK_FT_H__
#define __HISI_BLK_FT_H__

bool hisi_blk_ft_mq_queue_rq_redirection(
	struct request *rq, struct request_queue *q);
bool hisi_blk_ft_mq_complete_rq_redirection(struct request *rq, bool succ_done);
extern bool hisi_blk_ft_mq_rq_timeout_redirection(
	struct request *rq, enum blk_eh_timer_return *ret);

#endif /* __HISI_BLK_FT_H__ */
