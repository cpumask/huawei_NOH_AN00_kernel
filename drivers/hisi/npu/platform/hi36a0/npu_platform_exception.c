/*
 * npu_platform_exception.c
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
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

#include "npu_platform_exception.h"
#include "npu_shm.h"
#include "npu_hwts_sqe.h"
#include "npu_hwts_plat.h"
#include "npu_aicore_plat.h"
#include "npu_sdma_plat.h"
#include "npu_pm_framework.h"
#include "npu_pool.h"

u64 npu_exception_calc_sqe_addr(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id)
{
	u64 hwts_base = 0;
	u16 sq_head = 0;
	u64 sqe_addr = 0;
	int ret = 0;
	struct devdrv_hwts_sq_info *sq_info = NULL;
	struct devdrv_entity_info *hwts_sq_sub_info = NULL;

	COND_RETURN_ERROR(dev_ctx == NULL, 0, "dev_ctx is null\n");
	sq_info = devdrv_calc_hwts_sq_info(dev_ctx->devid, hwts_sq_id);
	COND_RETURN_ERROR(sq_info == NULL, 0,
		"hwts_sq_id= %u, hwts_sq_info is null\n", hwts_sq_id);

	hwts_sq_sub_info = (struct devdrv_entity_info *)(uintptr_t)sq_info->hwts_sq_sub;
	COND_RETURN_ERROR(hwts_sq_sub_info == NULL, 0,
		"hwts_sq_id= %u, hwts_sq_sub is null\n", hwts_sq_id);
	COND_RETURN_ERROR(hwts_sq_sub_info->vir_addr == 0, 0, "vir_addr is null\n");

	hwts_base = npu_hwts_get_base_addr();
	COND_RETURN_ERROR(hwts_base == 0, 0, "hwts_base is NULL\n");
	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_sq_head(channel_id, &sq_head, 1), ret);
	COND_RETURN_ERROR(ret != 0, 0, "can't get sq_head\n");

	sqe_addr = hwts_sq_sub_info->vir_addr + sq_head * sizeof(devdrv_hwts_kernel_sqe_t);
	NPU_DRV_DEBUG("sqe_addr = %016llx\n", sqe_addr);
	return sqe_addr;
}

void npu_exception_pool_conflict_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	u8 i;
	u8 status = 0;
	(void)hwts_sq_id;
	(void)channel_id;

	for (i = 0; i < NPU_HWTS_MAX_AICORE_POOL_NUM; i++)
		NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
			(void)npu_hwts_query_aicore_pool_status(i, 0, &status, do_print));
	for (i = 0; i < NPU_HWTS_MAX_SDMA_POOL_NUM; i++)
		NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
			(void)npu_hwts_query_sdma_pool_status(i, 0, &status, do_print));
}

void npu_exception_bus_error_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	u8 status;
	(void)hwts_sq_id;
	(void)channel_id;

	NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
		(void)npu_hwts_query_bus_error_type(&status, do_print));
	NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
		(void)npu_hwts_query_bus_error_id(&status, do_print));
}

void npu_exception_sqe_error_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	u64 sqe_head_addr = npu_exception_calc_sqe_addr(dev_ctx, hwts_sq_id, channel_id);

	(void)npu_hwts_query_sqe_info(sqe_head_addr);
}

void npu_exception_sw_status_error_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	u32 status = 0;

	NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
		(void)npu_hwts_query_sw_status(channel_id, &status, do_print));
	npu_exception_sqe_error_proc(dev_ctx, hwts_sq_id, channel_id, do_print);
}

void npu_exception_aicore_exception_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 channel_id, u8 do_print)
{
	int ret = 0;
	u8 aic_own_bitmap = 0;
	u8 aic_exception_bitmap = 0;
	struct aicore_exception_info aicore_info = {0};
	struct aicore_exception_error_info aicore_error_info = {0};
	struct aicore_exception_dbg_info aicore_dbg_info = {0};

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_aic_own_bitmap(
		channel_id, &aic_own_bitmap, do_print),
		ret);
	COND_RETURN_VOID(ret != 0, "cannot get aic_own_bitmap\n");

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_aic_exception_bitmap(
		channel_id, &aic_exception_bitmap, do_print),
		ret);
	COND_RETURN_VOID(ret != 0, "cannot get aic_exception_bitmap\n");

	if (aic_exception_bitmap == 0) {
		NPU_DRV_WARN("no aicore get exception\n");
		return;
	}
	if (aic_exception_bitmap & (0x1 << 0)) {
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_info(0, &aicore_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_error_info(
			0, &aicore_error_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_dbg_info(
			0, &aicore_dbg_info, do_print));
	}
	if (aic_exception_bitmap & (0x1 << 1)) {
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_info(1, &aicore_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_error_info(
			1, &aicore_error_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_dbg_info(
			1, &aicore_dbg_info, do_print));
	}
}

void npu_exception_aicore_trap_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 channel_id, u8 do_print)
{
	int ret = 0;
	u8 aic_own_bitmap = 0;
	u8 aic_trap_bitmap = 0;
	struct aicore_exception_info aicore_info = {0};
	struct aicore_exception_error_info aicore_error_info = {0};
	struct aicore_exception_dbg_info aicore_dbg_info = {0};

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_aic_own_bitmap(
		channel_id, &aic_own_bitmap, do_print),
		ret);
	COND_RETURN_VOID(ret != 0, "cannot get aic_own_bitmap\n");

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_aic_trap_bitmap(
		channel_id, &aic_trap_bitmap, do_print),
		ret);
	COND_RETURN_VOID(ret != 0, "cannot get aic_trap_bitmap\n");

	COND_RETURN_VOID(aic_trap_bitmap == 0, "no aicore get trap\n");
	if (aic_trap_bitmap & (0x1 << 0)) {
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_info(0, &aicore_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_error_info(
			0, &aicore_error_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE0,
			(void)npu_aicore_query_exception_dbg_info(
			0, &aicore_dbg_info, do_print));
	}
	if (aic_trap_bitmap & (0x1 << 1)) {
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_info(1, &aicore_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_error_info(
			1, &aicore_error_info, do_print));
		NPU_PM_SAFE_CALL(dev_ctx, NPU_AICORE1,
			(void)npu_aicore_query_exception_dbg_info(
			1, &aicore_dbg_info, do_print));
	}
}

void npu_exception_task_trap_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	npu_exception_aicore_trap_proc(dev_ctx, channel_id, do_print);
	npu_exception_sqe_error_proc(dev_ctx, hwts_sq_id, channel_id, do_print);
}

void npu_exception_sdma_exception_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 channel_id, u8 do_print)
{
	int ret = 0;
	u8 sdma_own_state = 0;
	u8 sdma_exception_id = 0;
	struct sdma_exception_info sdma_info = {0};

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_sdma_own_state(channel_id, &sdma_own_state, do_print), ret);
	COND_RETURN_VOID(ret != 0, "cannot get sdma_own_state\n");
	for (sdma_exception_id = 0; sdma_exception_id < NPU_HWTS_SDMA_CHANNEL_NUM;
		sdma_exception_id++)
		if (sdma_own_state & (1 << sdma_exception_id))
			break;
	if (sdma_exception_id == NPU_HWTS_SDMA_CHANNEL_NUM) {
		NPU_DRV_WARN("no sdma channel error\n");
		return;
	}

	NPU_PM_SAFE_CALL(dev_ctx, NPU_SUBSYS,
		(void)npu_sdma_query_exception_info(sdma_exception_id,
			&sdma_info, do_print));
}

void npu_exception_task_exception_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	int ret = 0;
	struct hwts_interrupt_info interrupt_info = {0};
	struct sq_exception_info sq_info = {0};
	(void)hwts_sq_id;

	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_interrupt_info(&interrupt_info, do_print), ret);
	NPU_PM_SAFE_CALL_WITH_RETURN(dev_ctx, NPU_SUBSYS,
		npu_hwts_query_sq_info(channel_id, &sq_info, do_print), ret);
	npu_exception_aicore_exception_proc(dev_ctx, channel_id, do_print);
	npu_exception_sdma_exception_proc(dev_ctx, channel_id, do_print);
}

void npu_exception_task_timeout_proc(
	struct devdrv_dev_ctx *dev_ctx, u16 hwts_sq_id, u16 channel_id, u8 do_print)
{
	npu_exception_task_exception_proc(dev_ctx, hwts_sq_id, channel_id, do_print);
}

void npu_exception_report_proc(
	struct devdrv_dev_ctx *dev_ctx, struct hwts_exception_report_info *report)
{
	static void (*exception_func[NPU_EXCEPTION_TYPE_MAX])
		(struct devdrv_dev_ctx *, u16, u16, u8) = {
		npu_exception_task_exception_proc,
		npu_exception_task_timeout_proc,
		npu_exception_task_trap_proc,
		npu_exception_sqe_error_proc,
		npu_exception_sw_status_error_proc,
		npu_exception_bus_error_proc,
		npu_exception_pool_conflict_proc
	};

	COND_RETURN_VOID(report == NULL, "report is null\n");
	COND_RETURN_VOID(report->exception_type >= NPU_EXCEPTION_TYPE_MAX,
		"unknown exception type\n");

	NPU_DRV_WARN("hwts_exception_report_info:model_id = %u, stream_id = %u, "
		"channel_id = %u, hwts_sq_id = %u, task_id = %u, exception_type = %u\n",
		report->model_id, report->stream_id, report->channel_id,
		report->hwts_sq_id, report->task_id, report->exception_type);

	(exception_func[report->exception_type])(dev_ctx,
		report->hwts_sq_id, report->channel_id, 1);
}
