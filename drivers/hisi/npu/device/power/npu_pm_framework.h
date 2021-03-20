/*
 * npu_pm_framework.h
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#ifndef __NPU_PM_FRAMEWORK_H
#define __NPU_PM_FRAMEWORK_H

#include "npu_proc_ctx.h"
#include "npu_platform_pm.h"

#define npu_bitmap_init(max) (~((0xFFFFFFFF >> (unsigned)(max)) << (unsigned)(max)))
#define npu_bitmap_clear(current, type) ((current) & (~(1U << (unsigned)(type))))
#define npu_bitmap_set(current, type) ((current) | ((1 << (unsigned)(type))))
#define NPU_SUBIP_ALL (npu_bitmap_init(NPU_SUBIP_MAX))

enum devdrv_power_stage {
	DEVDRV_PM_DOWN,
	DEVDRV_PM_NPUCPU,
	DEVDRV_PM_SMMU,
	DEVDRV_PM_TS,
	DEVDRV_PM_UP
};

enum devdrv_work_status {
	WORK_IDLE,
	WORK_ADDING,
	WORK_CANCELING,
	WORK_ADDED
};

int npu_sync_ts_time(void);

int devdrv_npu_ctrl_core(u32 dev_id, u32 core_num);

int npu_pm_enter_workmode(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u32 workmode);

int npu_pm_exit_workmode(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u32 workmode);

int npu_pm_vote(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u32 workmode);

int npu_pm_unvote(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u32 workmode);

int npu_pm_reboot(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx);

int npu_powerdown(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx);

int npu_pm_resource_init(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx);

int npu_pm_get_delta_subip_set(struct npu_power_manage *handle, int work_mode, int pm_ops);

void npu_pm_delete_idle_timer(struct devdrv_dev_ctx *dev_ctx);

void npu_pm_add_idle_timer(struct devdrv_dev_ctx *dev_ctx);

int npu_interframe_enable(struct devdrv_proc_ctx *proc_ctx, uint32_t flag);

void npu_pm_adapt_init(struct devdrv_dev_ctx *dev_ctx);
#endif
