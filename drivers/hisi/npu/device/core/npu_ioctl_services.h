/*
 * npu_ioctl_services.h
 *
 * about npu ioctl services
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
#ifndef __NPU_IOCTL_SERVICE_H
#define __NPU_IOCTL_SERVICE_H
#include <linux/cdev.h>

#include "npu_proc_ctx.h"

#define DEVDRV_ID_MAGIC    'D'
#define MAX_NODE_NUM        4
#define MAX_NODE_NUM_OF_FRAME    8

#define DEVDRV_ALLOC_STREAM_ID _IO(DEVDRV_ID_MAGIC, 1)
#define DEVDRV_FREE_STREAM_ID _IO(DEVDRV_ID_MAGIC, 2)
#define DEVDRV_ALLOC_EVENT_ID _IO(DEVDRV_ID_MAGIC, 3)
#define DEVDRV_FREE_EVENT_ID _IO(DEVDRV_ID_MAGIC, 4)
#define DEVDRV_REPORT_WAIT _IO(DEVDRV_ID_MAGIC, 5)
#define DEVDRV_MAILBOX_SEND _IO(DEVDRV_ID_MAGIC, 7)
#define DEVDRV_ALLOC_MODEL_ID _IO(DEVDRV_ID_MAGIC, 11)
#define DEVDRV_FREE_MODEL_ID _IO(DEVDRV_ID_MAGIC, 12)
#define DEVDRV_REQUEST_SEND _IO(DEVDRV_ID_MAGIC, 13)
#define DEVDRV_RESPONSE_RECEIVE _IO(DEVDRV_ID_MAGIC, 14)
#define DEVDRV_REPORT_SYNC_WAIT     _IO(DEVDRV_ID_MAGIC, 15)
#define DEVDRV_ENTER_WORKMODE       _IO(DEVDRV_ID_MAGIC, 16)
#define DEVDRV_EXIT_WORKMODE        _IO(DEVDRV_ID_MAGIC, 17)
#define DEVDRV_SET_LIMIT            _IO(DEVDRV_ID_MAGIC, 18)
#define DEVDRV_ENABLE_FEATURE       _IO(DEVDRV_ID_MAGIC, 19)
#define DEVDRV_DISABLE_FEATURE      _IO(DEVDRV_ID_MAGIC, 20)

#define DEVDRV_GET_OCCUPY_STREAM_ID _IO(DEVDRV_ID_MAGIC, 25)
#define DEVDRV_ALLOC_TASK_ID        _IO(DEVDRV_ID_MAGIC, 27)
#define DEVDRV_FREE_TASK_ID         _IO(DEVDRV_ID_MAGIC, 28)
#define DEVDRV_GET_TS_TIMEOUT_ID    _IO(DEVDRV_ID_MAGIC, 30)
#define DEVDRV_ATTACH_SYSCACHE      _IO(DEVDRV_ID_MAGIC, 31)

#define DEVDRV_SVM_BIND_PID             _IO(DEVDRV_ID_MAGIC, 35)
#define DEVDRV_SVM_UNBIND_PID           _IO(DEVDRV_ID_MAGIC, 36)
#define DEVDRV_CUSTOM_IOCTL         _IO(DEVDRV_ID_MAGIC, 64)

#define DEVDRV_MAX_CMD 65

#define DEVDRV_INTERFRAME_FEATURE_ID     0
#define DEVDRV_MAX_FEATURE_ID            10

long devdrv_npu_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

int devdrv_ioctl_alloc_stream(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_get_occupy_stream_id(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_alloc_event(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_alloc_model(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_alloc_task(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_free_stream(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_free_event(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_free_model(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_send_request(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_receive_response(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_free_task(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_report_wait(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_report_sync_wait(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_custom(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_get_ts_timeout(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_attach_syscache(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_enter_workwode(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_exit_workwode(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_set_limit(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int npu_ioctl_check_ion(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_svm_bind_pid(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int devdrv_ioctl_svm_unbind_pid(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

extern int devdrv_proc_npu_ioctl_call(struct devdrv_proc_ctx *proc_ctx,
                                      unsigned int cmd, unsigned long arg);

extern int npu_feature_enable(struct devdrv_proc_ctx *proc_ctx,
                        uint32_t feature_id, uint32_t enable);

int npu_ioctl_enable_feature(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

int npu_ioctl_disable_feature(struct devdrv_proc_ctx *proc_ctx, unsigned long arg);

#endif /* __DEVDRV_MANAGER_H */
