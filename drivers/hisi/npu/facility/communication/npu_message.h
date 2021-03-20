/*
 * npu_message.h
 *
 * about npu sq/cq msg
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __NPU_MESSAGE_H
#define __NPU_MESSAGE_H

#include "npu_proc_ctx.h"
#include "npu_ts_sqe.h"

int npu_send_request(struct devdrv_proc_ctx *proc_ctx, devdrv_ts_comm_sqe_t *comm_task);
int npu_receive_response(struct devdrv_proc_ctx *proc_ctx, struct devdrv_receive_response_info *report_info);
int npu_report_wait(struct devdrv_proc_ctx *proc_ctx, int *timeout_out, int timeout_in);
int npu_report_sync_wait(struct devdrv_proc_ctx *proc_ctx, struct devdrv_report_sync_wait_info *wait_info);
void npu_delete_idle_timer(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx);

#endif