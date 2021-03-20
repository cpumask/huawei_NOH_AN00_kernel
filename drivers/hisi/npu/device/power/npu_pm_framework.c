/*
 * npu_pm_framework.c
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
#include "npu_pm_framework.h"

#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/hisi/rdr_pub.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#include "bbox/npu_dfx_black_box.h"
#include "npu_proc_ctx.h"
#include "npu_common.h"
#include "npu_log.h"
#include "npu_platform.h"
#include "npu_firmware.h"
#include "npu_shm.h"
#include "npu_adapter.h"
#include "npu_autofs.h"
#include "npu_powercapping.h"
#include "npu_dpm.h"
#include "npu_message.h"
#include "npu_heart_beat.h"
#include "npu_hwts_plat.h"
#include "npu_platform_exception.h"
#include "dts/npu_reg.h"
#include <dsm/dsm_pub.h>

static void npu_pm_idle_work(struct work_struct *npu_idle_work);
static void npu_pm_interframe_idle_work(struct work_struct *interframe_idle_work);
struct npu_pm_work_strategy *npu_pm_get_work_strategy(
			struct npu_power_manage *handle, int work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;

	strategy = (struct npu_pm_work_strategy *)(uintptr_t)(handle->strategy_table_addr
		+ sizeof(struct npu_pm_work_strategy) * (work_mode));

	return strategy;
}

struct npu_pm_subip_action *npu_pm_get_subip_action(
			struct npu_power_manage *handle, int subip)
{
	struct npu_pm_subip_action *action = NULL;

	action = (struct npu_pm_subip_action *)(uintptr_t)(handle->action_table_addr
		+ sizeof(struct npu_pm_subip_action) * (subip));

	return action;
}

static struct npu_pm_work_strategy *npu_pm_interframe_idle_get_work_strategy(
					struct npu_power_manage *handle, int work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;
	struct npu_interframe_idle_manager *interframe_idle_manager =
				&handle->interframe_idle_manager;

	strategy = (struct npu_pm_work_strategy *)(uintptr_t)(interframe_idle_manager->strategy_table_addr
			+ sizeof(struct npu_pm_work_strategy) * (work_mode));

	return strategy;
}

static u32 npu_pm_interframe_idle_get_powerup_delta_subip(
				struct npu_power_manage *handle, int work_mode)
{
	struct npu_pm_work_strategy *strategy;
	u32 delta_subip;
	u32 cur_subip;

	strategy = npu_pm_interframe_idle_get_work_strategy(handle, work_mode);
	delta_subip = strategy->subip_set.data;
	cur_subip = handle->cur_subip_set;
	delta_subip &= (~cur_subip);

	return delta_subip;
}

static u32 npu_pm_interframe_idle_get_powerdown_delta_subip(
				struct npu_power_manage *handle, int work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;
	u32 mode_idx;
	u32 idle_subip;
	u32 delta_subip;
	u32 used_subip = 0;

	for (mode_idx = 0; mode_idx < NPU_WORKMODE_MAX; mode_idx++) {
		if (mode_idx == work_mode)
			continue;
		if (BITMAP_GET(handle->work_mode, mode_idx) == 0x1) {
			strategy = npu_pm_interframe_idle_get_work_strategy(handle, mode_idx);
			used_subip |= strategy->subip_set.data;
		}
	}
	idle_subip = handle->interframe_idle_manager.idle_subip;
	delta_subip = idle_subip & (~used_subip);

	return delta_subip;
}

static u32 npu_pm_interframe_idle_get_delta_subip(
				struct npu_power_manage *handle, int work_mode, int pm_ops)
{
	u32 delta_subip;

	if (pm_ops == POWER_UP)
		delta_subip = npu_pm_interframe_idle_get_powerup_delta_subip(handle, work_mode);
	else
		delta_subip = npu_pm_interframe_idle_get_powerdown_delta_subip(handle, work_mode);

	return delta_subip;
}

void npu_close_idle_power_down(struct devdrv_dev_ctx *dev_ctx)
{
	int work_state;

	if (!dev_ctx) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return;
	}
	work_state = atomic_cmpxchg(&dev_ctx->pm.idle_wq_processing, WORK_ADDED, WORK_CANCELING);
	if (work_state == WORK_ADDED) {
		cancel_delayed_work_sync(&dev_ctx->pm.idle_work);
		atomic_set(&dev_ctx->pm.idle_wq_processing, WORK_IDLE);
	} else if (work_state != WORK_IDLE) {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}
}

void npu_close_interframe_power_down(struct devdrv_dev_ctx *dev_ctx)
{
	int work_state;
	atomic_t *wq_processing = NULL;

	if (!dev_ctx) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return;
	}

	wq_processing = &dev_ctx->pm.interframe_idle_manager.wq_processing;
	work_state = atomic_cmpxchg(wq_processing, WORK_ADDED, WORK_CANCELING);
	if (work_state == WORK_ADDED) {
		cancel_delayed_work_sync(&dev_ctx->pm.interframe_idle_manager.work);
		atomic_set(wq_processing, WORK_IDLE);
	} else if (work_state != WORK_IDLE) {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}
}

void npu_pm_delete_idle_timer(struct devdrv_dev_ctx *dev_ctx)
{
	struct devdrv_platform_info *plat_info = dev_ctx->plat_info;

	if (!plat_info) {
		NPU_DRV_ERR("plat info is null\n");
		return;
	}

	npu_close_idle_power_down(dev_ctx);

	if (devdrv_plat_get_feature_switch(plat_info, DEVDRV_FEATURE_INTERFRAME_IDLE_POWER_DOWN))
		npu_close_interframe_power_down(dev_ctx);
}

void npu_open_idle_power_down(struct devdrv_dev_ctx *dev_ctx)
{
	int work_state;

	if (!dev_ctx) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return;
	}
	work_state = atomic_cmpxchg(&dev_ctx->pm.idle_wq_processing, WORK_IDLE, WORK_ADDING);
	if (work_state == WORK_IDLE) {
		schedule_delayed_work(&dev_ctx->pm.idle_work, msecs_to_jiffies(dev_ctx->pm.npu_idle_time_out));
		atomic_set(&dev_ctx->pm.idle_wq_processing, WORK_ADDED);
	} else if (work_state != WORK_ADDED) {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}
}

void npu_open_interframe_power_down(struct devdrv_dev_ctx *dev_ctx)
{
	int work_state;
	atomic_t *wq_processing = NULL;
	struct delayed_work *interframe_work = NULL;
	if (!dev_ctx) {
		NPU_DRV_ERR("dev_ctx is null\n");
		return;
	}

	wq_processing = &dev_ctx->pm.interframe_idle_manager.wq_processing;
	interframe_work = &dev_ctx->pm.interframe_idle_manager.work;
	work_state = atomic_cmpxchg(wq_processing, WORK_IDLE, WORK_ADDING);
	if (work_state == WORK_IDLE) {
		INIT_DELAYED_WORK(interframe_work, npu_pm_interframe_idle_work);
		schedule_delayed_work(interframe_work,
			msecs_to_jiffies(dev_ctx->pm.interframe_idle_manager.idle_time_out));
		atomic_set(wq_processing, WORK_ADDED);
	} else if (work_state != WORK_ADDED) {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}
}

void npu_pm_add_idle_timer(struct devdrv_dev_ctx *dev_ctx)
{
	struct devdrv_platform_info *plat_info = dev_ctx->plat_info;
	if (!plat_info) {
		NPU_DRV_ERR("plat info is null\n");
		return;
	}

	if ((BITMAP_GET(dev_ctx->pm.work_mode, NPU_NONSEC) == 0) || (dev_ctx->power_stage != DEVDRV_PM_UP)) {
		NPU_DRV_WARN("Can not add idle timer, for work mode: %d, power stage: %d!\n",
			dev_ctx->pm.work_mode, dev_ctx->power_stage);
		return;
	}

	npu_open_idle_power_down(dev_ctx);

	if (devdrv_plat_get_feature_switch(plat_info, DEVDRV_FEATURE_INTERFRAME_IDLE_POWER_DOWN) &&
		(atomic_read(&dev_ctx->pm.interframe_idle_manager.enable) == 1))
			npu_open_interframe_power_down(dev_ctx);
}

int npu_pm_powerdown_proc(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, int work_mode, u32 delta_subip)
{
	int ret = 0;
	int fail_flag = false;
	int subip;
	int subip_idx;
	u32 curr_subip_state;
	struct npu_pm_subip_action *subip_action = NULL;

	curr_subip_state = dev_ctx->pm.cur_subip_set;
	NPU_DRV_INFO("delta_subip : 0x%x, curr_subip_state: 0x%x\n", delta_subip, curr_subip_state);
	for (subip_idx = 0; subip_idx < NPU_SUBIP_MAX; subip_idx++) {
		subip = g_powerdown_order[subip_idx];
		if (BITMAP_GET(delta_subip, subip) && (BITMAP_GET(curr_subip_state, subip) == 1)) {
			subip_action = npu_pm_get_subip_action(&dev_ctx->pm, subip);
			ret = subip_action->power_down(work_mode, delta_subip, (void *)proc_ctx);
			if (ret != 0) {
				fail_flag = true;
				NPU_DRV_ERR("subip power down fail : subip %d ret = %d\n", subip, ret);
			}
			curr_subip_state &= (~(1U << (unsigned)subip));
			if ((subip == NPU_AICORE0) && (BITMAP_GET(delta_subip, NPU_AICORE1)))
				curr_subip_state &= (~(1 << NPU_AICORE1));
			dev_ctx->pm.cur_subip_set = curr_subip_state;
			NPU_DRV_INFO("subip_%d power down succ, state : 0x%x\n", subip, curr_subip_state);
		}
	}
	if (fail_flag == true) {
		if (npu_rdr_exception_is_count_exceeding((u32)RDR_EXC_TYPE_NPU_POWERDOWN_FAIL) == 0)
			rdr_system_error((u32)RDR_EXC_TYPE_NPU_POWERDOWN_FAIL, 0, 0);
	}
	return ret;
}

int npu_pm_interframe_idle_powerdown(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	u32 delta_subip;

	delta_subip = npu_pm_interframe_idle_get_delta_subip(&dev_ctx->pm, work_mode, POWER_DOWN);
	ret = npu_pm_powerdown_proc(proc_ctx, dev_ctx, work_mode, delta_subip);

	return ret;
}

static void npu_pm_interframe_idle_work(struct work_struct *interframe_idle_work)
{
	struct npu_interframe_idle_manager *idle_manager = NULL;
	struct devdrv_dev_ctx *dev_ctx = NULL;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	int work_state;

	NPU_DRV_INFO("interframe idle timer work enter\n");
	COND_RETURN_VOID(interframe_idle_work == NULL, "interframe_idle_work is null\n");
	idle_manager = container_of(interframe_idle_work,
					struct npu_interframe_idle_manager, work.work);
	proc_ctx  = idle_manager->private_data;
	COND_RETURN_VOID(proc_ctx == NULL, "proc_ctx is null\n");
	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_VOID(dev_ctx == NULL, "proc_ctx is null\n");

	if (atomic_read(&dev_ctx->pm.task_ref_cnt) != 0) {
		atomic_cmpxchg(&idle_manager->wq_processing, WORK_ADDED, WORK_IDLE);
		return;
	}
	work_state = atomic_read(&idle_manager->wq_processing);
	if (work_state == WORK_ADDED) {
		mutex_lock(&dev_ctx->npu_power_up_off_mutex);
		(void)npu_pm_interframe_idle_powerdown(proc_ctx, dev_ctx, NPU_NONSEC);

		if ((DEVDRV_PLAT_GET_FEAUTRE_SWITCH(dev_ctx->plat_info, DEVDRV_FEATURE_NPU_AUTOFS) == 1) &&
			(BITMAP_GET(dev_ctx->pm.work_mode, NPU_ISPNN_SEPARATED) ==0) &&
			(BITMAP_GET(dev_ctx->pm.work_mode, NPU_ISPNN_SHARED) ==0) &&
			(dev_ctx->power_stage == DEVDRV_PM_UP))
			npu_autofs_enable();
		mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
		atomic_set(&idle_manager->wq_processing, WORK_IDLE);
	} else {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}
}

static int npu_pm_interframe_check_need_powerup(struct devdrv_dev_ctx *dev_ctx, u32 work_mode)
{
	u32 cur_subip;
	u32 workmode_subip;
	struct npu_power_manage *power_manage = &dev_ctx->pm;
	struct npu_pm_work_strategy *work_strategy = NULL;

	work_strategy = npu_pm_interframe_idle_get_work_strategy(power_manage, work_mode);
	workmode_subip = work_strategy->subip_set.data;
	cur_subip = power_manage->cur_subip_set;
	NPU_DRV_DEBUG("cur_subip_set = 0x%x, subip_set = 0x%x\n", cur_subip, workmode_subip);
	if ((workmode_subip & cur_subip) != workmode_subip)
		return 1;
	else
		return 0;
}

int npu_pm_powerup_proc(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, int work_mode, u32 delta_subip)
{
	int ret = 0;
	int subip;
	int subip_idx;
	u32 curr_subip_set;
	struct npu_pm_subip_action *subip_action = NULL;

	curr_subip_set = dev_ctx->pm.cur_subip_set;
	NPU_DRV_INFO("delta_subip : 0x%x, curr_subip_state: 0x%x\n", delta_subip, curr_subip_set);
	for (subip_idx = 0; subip_idx < NPU_SUBIP_MAX; subip_idx++) {
		subip = g_powerup_order[subip_idx];
		if (BITMAP_GET(delta_subip, subip) && (BITMAP_GET(curr_subip_set, subip) == 0)) {
			subip_action = npu_pm_get_subip_action(&dev_ctx->pm, subip);
			ret = subip_action->power_up(work_mode, delta_subip, (void **)proc_ctx);
			COND_GOTO_ERROR(
				ret != 0, POWER_UP_FAIL, ret, ret, "subip power up fail : subip %d ret = %d\n", subip, ret);
			curr_subip_set |= (1 << (unsigned)subip);
			if ((subip == NPU_AICORE0) && (BITMAP_GET(delta_subip, NPU_AICORE1)))
				curr_subip_set |= (1 << NPU_AICORE1);
			dev_ctx->pm.cur_subip_set = curr_subip_set;
			NPU_DRV_INFO("subip_%d power up succ, state : 0x%x\n", subip, dev_ctx->pm.cur_subip_set);
		}
	}

	return ret;
POWER_UP_FAIL:
	(void)npu_pm_powerdown_proc(proc_ctx, dev_ctx, work_mode, delta_subip);
	if (npu_rdr_exception_is_count_exceeding((u32)RDR_EXC_TYPE_NPU_POWERUP_FAIL) == 0 &&
		ret != -NOSUPPORT)
		rdr_system_error((u32)RDR_EXC_TYPE_NPU_POWERUP_FAIL, 0, 0);
	if (!dsm_client_ocuppy(davinci_dsm_client)) {
		dsm_client_record(davinci_dsm_client, "npu power up failed\n");
		dsm_client_notify(davinci_dsm_client, DSM_AI_KERN_POWER_UP_ERR_NO);
		NPU_DRV_ERR("[I/DSM] %s dmd report\n", davinci_dsm_client->client_name);
	}
	return ret;
}

static int npu_pm_interframe_idle_powerup(struct devdrv_proc_ctx *proc_ctx,
				struct devdrv_dev_ctx *dev_ctx, u32 workmode)
{
	int ret;
	u32 delta_subip;

	delta_subip = npu_pm_interframe_idle_get_delta_subip(&dev_ctx->pm, workmode, POWER_UP);
	ret = npu_pm_powerup_proc(proc_ctx, dev_ctx, workmode, delta_subip);
	COND_RETURN_ERROR(ret != 0, ret, "power down fail: ret = %d\n", ret);

	NPU_DRV_INFO("interframe power up\n");

	return ret;
}

int npu_pm_check_and_interframe_powerup(struct devdrv_proc_ctx *proc_ctx,
			struct devdrv_dev_ctx *dev_ctx, u32 workmode)
{
	int ret = 0;

	if (devdrv_plat_get_feature_switch(dev_ctx->plat_info, DEVDRV_FEATURE_INTERFRAME_IDLE_POWER_DOWN)) {
		if (workmode == NPU_NONSEC) {
			if (npu_pm_interframe_check_need_powerup(dev_ctx, workmode)) {
				if ((DEVDRV_PLAT_GET_FEAUTRE_SWITCH(dev_ctx->plat_info, DEVDRV_FEATURE_NPU_AUTOFS) == 1) &&
					(dev_ctx->power_stage == DEVDRV_PM_UP))
					npu_autofs_disable();
				ret = npu_pm_interframe_idle_powerup(proc_ctx, dev_ctx, workmode);
			}
		}
	}

	return ret;
}

int npu_pm_enter_workmode(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, u32 work_mode)
{
	int ret;
	u32 curr_work_mode_set;
	struct npu_pm_work_strategy *work_strategy = NULL;
	struct devdrv_platform_info *plat_info = dev_ctx->plat_info;

	if (plat_info == NULL) {
		NPU_DRV_ERR("plat info is null, enter workmode %d failed\n", work_mode);
		return -1;
	}

	mutex_lock(&dev_ctx->npu_power_up_off_mutex);
	curr_work_mode_set = dev_ctx->pm.work_mode;
	if (curr_work_mode_set == 0)
		npu_pm_resource_init(proc_ctx, dev_ctx);
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);

	npu_rdr_exception_init();
	if (BITMAP_GET(curr_work_mode_set, NPU_NONSEC) && (work_mode == NPU_SEC)) {
		ret = npu_pm_exit_workmode(proc_ctx, dev_ctx, NPU_NONSEC);
		COND_RETURN_ERROR(ret != 0, ret, "exit NONSEC failed ret:%d\n", ret);
	}

	if (work_mode == NPU_NONSEC)
		npu_pm_delete_idle_timer(dev_ctx);

	mutex_lock(&dev_ctx->npu_power_up_off_mutex);
	curr_work_mode_set = dev_ctx->pm.work_mode;
	if (BITMAP_GET(curr_work_mode_set, work_mode) != 0) {
		NPU_DRV_INFO("workmode already enabled, workmode : %d, curr_work : 0x%x\n", work_mode, curr_work_mode_set);
		ret = npu_pm_check_and_interframe_powerup(proc_ctx, dev_ctx, work_mode);
		COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "npu interframe powerup failed\n");
		mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
		return 0;
	}

	work_strategy = npu_pm_get_work_strategy(&dev_ctx->pm, work_mode);
	if ((curr_work_mode_set & work_strategy->work_mode_set.data) != 0) {
		NPU_DRV_INFO(
			"exc_work_mode already enabled, workmode : %d, curr_work : %d\n", work_mode, curr_work_mode_set);
		ret = -1;
		goto FAIL;
	}

	ret = npu_pm_vote(proc_ctx, dev_ctx, work_mode);
	COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "fail to enter workwode: %d, ret = %d\n", work_mode, ret);

	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);

	return 0;

FAIL:
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
	return ret;
}

int npu_pm_exit_workmode(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u32 workmode)
{
	int ret;
	u32 curr_work_mode_set;

	if (workmode == NPU_NONSEC)
		npu_pm_delete_idle_timer(dev_ctx);

	mutex_lock(&dev_ctx->npu_power_up_off_mutex);

	curr_work_mode_set = dev_ctx->pm.work_mode;
	if (BITMAP_GET(curr_work_mode_set, workmode) == 0) {
		NPU_DRV_WARN("workmode already exited, workmode : %d, curr_work : 0x%x\n", workmode, curr_work_mode_set);
		mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
		return 0;
	}

	ret = npu_pm_unvote(proc_ctx, dev_ctx, workmode);
	if (ret != 0)
		NPU_DRV_ERR("fail to exit workwode: %d, ret = %d\n", workmode, ret);

	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);

	if (workmode == NPU_NONSEC) {
		atomic_set(&dev_ctx->pm.task_ref_cnt, 0);
	}
	return ret;
}

int npu_pm_l2_swap_in(struct devdrv_proc_ctx *proc_ctx,
	struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret = 0;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	struct npu_vma_mmapping *npu_vma_map = NULL;

	if ((!list_empty_careful(&proc_ctx->l2_vma_list)) &&
		(!BITMAP_GET(dev_ctx->pm.work_mode, NPU_SEC)) &&
		(work_mode == NPU_INIT)) {
		list_for_each_safe(pos, n, &proc_ctx->l2_vma_list) {
			npu_vma_map = list_entry(pos, struct npu_vma_mmapping, list);
			ret = l2_mem_swapin(npu_vma_map->vma);
		}
	} else {
		NPU_DRV_WARN("l2_vma_list is empty or work_mode 0x%x is not init,"
			" l2_mem_swapin is not necessary", work_mode);
	}

	return ret;
}

int npu_pm_l2_swap_out(struct devdrv_proc_ctx *proc_ctx,
	struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret = 0;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	struct npu_vma_mmapping *npu_vma_map = NULL;

	if ((!list_empty_careful(&proc_ctx->l2_vma_list)) &&
		(!BITMAP_GET(dev_ctx->pm.work_mode, NPU_SEC)) &&
		(work_mode == NPU_INIT)) {
		list_for_each_safe(pos, n, &proc_ctx->l2_vma_list) {
			npu_vma_map = list_entry(pos, struct npu_vma_mmapping, list);
			ret = l2_mem_swapout(npu_vma_map->vma, dev_ctx->devid);
		}
	} else {
		NPU_DRV_WARN("l2_vma_list is empty or work_mode 0x%x is not init,"
			" l2_mem_swapout is not necessary", work_mode);
	}

	return ret;
}

int npu_pm_powerdown_pre_proc(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	npu_work_mode_info_t work_mode_info = {0};
	u32 goal_work_mode_set = (dev_ctx->pm.work_mode & (~(1U << (unsigned)work_mode)));

	ret = npu_pm_l2_swap_out(proc_ctx, dev_ctx, work_mode);
	if (ret != 0)
		NPU_DRV_ERR("l2 swap out fail, ret = %d\n", ret);

	/* hwts aicore pool switch back */
	work_mode_info.work_mode = work_mode;
	work_mode_info.flags = dev_ctx->pm.work_mode_flags;
	ret = npu_plat_switch_hwts_aicore_pool(dev_ctx, &work_mode_info, POWER_OFF);
	if (ret != 0)
		NPU_DRV_ERR("hwts return aicore to pool fail, ret = %d\n", ret);

	if ((goal_work_mode_set == 0) && (work_mode != NPU_SEC)) {
		/* bbox heart beat exit */
		devdrv_heart_beat_exit(dev_ctx);
#if defined (CONFIG_HISI_DPM_HWMON) && defined (CONFIG_NPU_DPM_ENABLED)
		npu_dpm_exit();
#endif
		npu_powercapping_disable();
	}

	return 0;
}

int npu_pm_powerdown_post_proc(struct devdrv_proc_ctx *proc_ctx,
			struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int aicore_result;
	u32 *ts_status = NULL;
	u32 goal_work_mode_set = (dev_ctx->pm.work_mode & (~(1U << (unsigned)(work_mode))));

	aicore_result = dev_ctx->pm.cur_subip_set >> NPU_AICORE0;
	NPU_DRV_WARN("aicore power up status : 0x%x\n", aicore_result);

	if (goal_work_mode_set == 0) {
		dev_ctx->ts_work_status = 0;
		ts_status = devdrv_get_ts_work_status(dev_ctx->devid);
		if (ts_status != NULL)
			*ts_status = DEVDRV_TS_DOWN;

		mutex_lock(&dev_ctx->npu_wake_lock_mutex);
		__pm_relax(&dev_ctx->wakelock);
		mutex_unlock(&dev_ctx->npu_wake_lock_mutex);
	}
	return 0;
}

int npu_pm_powerup_pre_proc(struct devdrv_proc_ctx *proc_ctx,
			struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	struct devdrv_platform_info *plat_info = NULL;

	plat_info = dev_ctx->plat_info;
	COND_RETURN_ERROR(plat_info == NULL, -1, "get plat infofailed\n");
	NPU_DRV_INFO("firmware is loading");

	if ((DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_KERNEL_LOAD_IMG) == 1)
		&& (dev_ctx->pm.work_mode == 0)) {
		ret = devdrv_load_cpu_fw();
		if (ret != 0) {
			atomic_set(&dev_ctx->power_access, 1);
			NPU_DRV_ERR("load cpu fw failed ret %d", ret);
			return ret;
		}
	} else {
		NPU_DRV_WARN("no need to load firmware, for npu/aicpu firmware is loaded in fastboot");
	}

	atomic_set(&dev_ctx->power_access, 0);

	return 0;
}

int npu_pm_powerup_post_proc(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	int aicore_result;
	unsigned long flags;
	struct devdrv_platform_info *plat_info = NULL;
	u32 *ts_status = NULL;
	npu_work_mode_info_t work_mode_info = {0};

	aicore_result = dev_ctx->pm.cur_subip_set >> NPU_AICORE0;
	NPU_DRV_WARN("aicore power up status : 0x%x\n", aicore_result);

	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "get plat infofailed\n");
	dev_ctx->inuse.devid = dev_ctx->devid;
	spin_lock_irqsave(&dev_ctx->ts_spinlock, flags);
	dev_ctx->inuse.ai_core_num = DEVDRV_PLAT_GET_AICORE_MAX(plat_info);
	dev_ctx->inuse.ai_core_error_bitmap = 0;
	dev_ctx->inuse.ai_cpu_num = DEVDRV_PLAT_GET_AICPU_MAX(plat_info);
	dev_ctx->inuse.ai_cpu_error_bitmap = 0;
	spin_unlock_irqrestore(&dev_ctx->ts_spinlock, flags);
	dev_ctx->power_stage = DEVDRV_PM_UP;

	ts_status = devdrv_get_ts_work_status(dev_ctx->devid);
	if (ts_status != NULL)
		*ts_status = (u32)(work_mode != NPU_SEC ? DEVDRV_TS_WORK : DEVDRV_TS_SEC_WORK);
	NPU_DRV_WARN("npu dev %u hardware powerup successfully!\n", dev_ctx->devid);

	if ((work_mode != NPU_SEC) && (dev_ctx->pm.work_mode == 0)) {
		dev_ctx->heart_beat.hwts_exception_callback = npu_exception_report_proc;
		/* bbox heart beat init in non_secure mode */
		ret = devdrv_heart_beat_init(dev_ctx);
		if (ret != 0)
			NPU_DRV_ERR("devdrv_heart_beat_init failed, ret = %d\n", ret);

		ret = npu_sync_ts_time();
		if (ret != 0)
			NPU_DRV_WARN("npu_sync_ts_time fail. ret = %d\n", ret);

		npu_powercapping_enable();
#if defined (CONFIG_HISI_DPM_HWMON) && defined (CONFIG_NPU_DPM_ENABLED)
		npu_dpm_init();
#endif
		/* set hwts log&profiling gobal config when hwts init ok */
		hwts_profiling_init(dev_ctx->devid);
	}

	mutex_lock(&dev_ctx->npu_wake_lock_mutex);
	__pm_stay_awake(&dev_ctx->wakelock);
	mutex_unlock(&dev_ctx->npu_wake_lock_mutex);

	/* hwts aicore pool switch */
	work_mode_info.work_mode = work_mode;
	work_mode_info.flags = dev_ctx->pm.work_mode_flags;
	ret = npu_plat_switch_hwts_aicore_pool(dev_ctx, &work_mode_info, POWER_ON);
	COND_RETURN_ERROR(ret != 0, ret,
		"hwts pull aicore from pool fail, ret = %d\n", ret);

	if (work_mode == NPU_NONSEC) {
		ret = devdrv_proc_send_alloc_stream_mailbox(proc_ctx);
		if (ret) {
			NPU_DRV_ERR("devdrv send stream mailbox failed\n");
			return ret;
		}
	} else {
		NPU_DRV_WARN("secure or ispnn npu power up,no need send mbx to tscpu,return directly\n");
	}

	ret = npu_pm_l2_swap_in(proc_ctx, dev_ctx, work_mode);
	if (ret != 0)
		NPU_DRV_ERR("l2 swap in fail, ret = %d\n", ret);

	dev_ctx->ts_work_status = 1;

	return 0;
}

static inline void npu_pm_set_power_on(int work_mode)
{
	if (work_mode == NPU_SEC)
		devdrv_plat_set_npu_power_status(DRV_NPU_POWER_ON_SEC_FLAG);
	else
		devdrv_plat_set_npu_power_status(DRV_NPU_POWER_ON_FLAG);
}

static inline void npu_pm_set_power_off(void)
{
	devdrv_plat_set_npu_power_status(DRV_NPU_POWER_OFF_FLAG);
}

int npu_pm_powerdown(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	u32 delta_subip;

	NPU_DRV_WARN("enter, mode = %d\n", work_mode);
	npu_pm_set_power_off();
	ret = npu_pm_powerdown_pre_proc(proc_ctx, dev_ctx, work_mode);
	if (ret != 0)
		NPU_DRV_ERR("power down pre_porc fail: ret = %d\n", ret);

	delta_subip = npu_pm_get_delta_subip_set(&dev_ctx->pm, work_mode, POWER_DOWN);
	ret = npu_pm_powerdown_proc(proc_ctx, dev_ctx, work_mode, delta_subip);
	if (ret != 0)
		NPU_DRV_ERR("power down fail: ret = %d\n", ret);

	ret = npu_pm_powerdown_post_proc(proc_ctx, dev_ctx, work_mode);
	if (ret != 0)
		NPU_DRV_ERR("power down post_porc fail : ret = %d\n", ret);

	NPU_DRV_WARN("npu dev %u powerdown end! delta_subip = 0x%x\n", dev_ctx->devid, delta_subip);
	return ret;
}

int npu_pm_powerup(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, int work_mode)
{
	int ret;
	u32 delta_subip;

	NPU_DRV_WARN("enter, mode = %d\n", work_mode);

	ret = npu_pm_powerup_pre_proc(proc_ctx, dev_ctx, work_mode);
	COND_RETURN_ERROR(ret != 0, ret, "power up pre_porc fail: ret = %d\n", ret);

	delta_subip = npu_pm_get_delta_subip_set(&dev_ctx->pm, work_mode, POWER_UP);
	ret = npu_pm_powerup_proc(proc_ctx, dev_ctx, work_mode, delta_subip);
	COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "power up fail: ret = %d\n", ret);

	ret = npu_pm_powerup_post_proc(proc_ctx, dev_ctx, work_mode);
	COND_GOTO_ERROR(ret != 0, POST_FAIL, ret, ret, "power up post_porc fail: ret = %d\n", ret);

	npu_pm_set_power_on(work_mode);

	NPU_DRV_WARN("npu dev %u powerup succ! delta_subip = 0x%x\n", dev_ctx->devid, delta_subip);
	return ret;

POST_FAIL:
	(void)npu_pm_powerdown(proc_ctx, dev_ctx, work_mode);
FAIL:
	if (dev_ctx->pm.work_mode == 0) {
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
		atomic_set(&dev_ctx->power_access, 1);
	}
	return ret;
}

int npu_powerdown(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx)
{
	int ret = 0;
	int workmode_idx;
	u32 goal_work_mode_set;
	u32 curr_work_mode_set;

	NPU_DRV_WARN("all workmode power down enter");

	curr_work_mode_set = dev_ctx->pm.work_mode;
	goal_work_mode_set = curr_work_mode_set;
	for (workmode_idx = 0; workmode_idx < NPU_WORKMODE_MAX; workmode_idx++) {
		if (BITMAP_GET(curr_work_mode_set, workmode_idx) != 0) {
			goal_work_mode_set &= (~(1U << (unsigned)workmode_idx));
			ret = npu_pm_powerdown(proc_ctx, dev_ctx, workmode_idx);
			COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "npu power down fail : ret = %d, in workmode = %d\n",
				ret, workmode_idx);

			dev_ctx->pm.work_mode = goal_work_mode_set;
		}
	}

	if (dev_ctx->pm.work_mode == 0)
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
	NPU_DRV_WARN("succ, work_mode_set = 0x%x\n", dev_ctx->pm.work_mode);
	return ret;
FAIL:
	dev_ctx->pm.work_mode = goal_work_mode_set;
	if (dev_ctx->pm.work_mode == 0)
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
	return ret;
}

int npu_pm_vote(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, u32 workmode)
{
	int ret;
	u32 goal_work_mode_set;
	u32 curr_work_mode_set;

	curr_work_mode_set = dev_ctx->pm.work_mode;
	goal_work_mode_set = curr_work_mode_set | (1 << workmode);
	ret = npu_pm_powerup(proc_ctx, dev_ctx, workmode);
	COND_RETURN_ERROR(ret != 0, ret, "vote fail in power up : ret = %d\n", ret);

	dev_ctx->pm.work_mode = goal_work_mode_set;
	NPU_DRV_WARN("NPU vote SUCC, workmode:%d, curr_workmode_set = 0x%x\n", workmode, goal_work_mode_set);

	if (dev_ctx->pm.work_mode != 0)
		dev_ctx->power_stage = DEVDRV_PM_UP;
	return ret;
}

int npu_pm_unvote(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx, u32 workmode)
{
	int ret;
	u32 goal_work_mode_set;
	u32 curr_work_mode_set;

	curr_work_mode_set = dev_ctx->pm.work_mode;
	goal_work_mode_set = curr_work_mode_set & (~(1U << workmode));
	ret = npu_pm_powerdown(proc_ctx, dev_ctx, workmode);
	COND_RETURN_ERROR(ret != 0, ret, "unvote fail in power up : ret = %d\n", ret);

	dev_ctx->pm.work_mode = goal_work_mode_set;
	NPU_DRV_WARN("NPU unvote SUCC, workmode:%d, curr_workmode_set = 0x%x\n", workmode, goal_work_mode_set);

	if (dev_ctx->pm.work_mode == 0) {
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
		atomic_set(&dev_ctx->power_access, 1);
	}
	return ret;
}

int npu_pm_reboot(struct devdrv_proc_ctx *proc_ctx,
		struct devdrv_dev_ctx *dev_ctx)
{
	int ret = 0;
	int workmode_idx;
	u32 goal_work_mode_set;
	u32 curr_work_mode_set;

	NPU_DRV_WARN("reboot enter");

	curr_work_mode_set = dev_ctx->pm.work_mode;
	npu_pm_delete_idle_timer(dev_ctx);

	atomic_set(&dev_ctx->pm.task_ref_cnt, 0);

	mutex_lock(&dev_ctx->npu_power_up_off_mutex);
	curr_work_mode_set = dev_ctx->pm.work_mode;
	goal_work_mode_set = curr_work_mode_set;
	for (workmode_idx = 0; workmode_idx < NPU_WORKMODE_MAX; workmode_idx++) {
		if (BITMAP_GET(curr_work_mode_set, workmode_idx) != 0) {
			goal_work_mode_set &= (~(1U << (unsigned)workmode_idx));
			ret = npu_pm_powerdown(proc_ctx, dev_ctx, workmode_idx);
			COND_GOTO_ERROR(ret != 0, FAIL, ret, ret,
				"reboot fail in power down : ret = %d, workmode = %d\n", ret, workmode_idx);

			dev_ctx->pm.work_mode = goal_work_mode_set;
		}
	}

	for (workmode_idx = 0; workmode_idx < NPU_WORKMODE_MAX; workmode_idx++) {
		if (BITMAP_GET(curr_work_mode_set, workmode_idx) != 0) {
			ret = npu_pm_powerup(proc_ctx, dev_ctx, workmode_idx);
			COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "reboot fail in power up : ret = %d, workmode = %d\n",
				ret, workmode_idx);

			goal_work_mode_set |= (1 << (unsigned)workmode_idx);
			dev_ctx->pm.work_mode = goal_work_mode_set;
		}
	}

	if (dev_ctx->pm.work_mode == 0)
		dev_ctx->power_stage = DEVDRV_PM_DOWN;

	NPU_DRV_WARN("reboot succ, work_mode_set = 0x%x\n", dev_ctx->pm.work_mode);
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
	return ret;
FAIL:
	dev_ctx->pm.work_mode = goal_work_mode_set;
	if (dev_ctx->pm.work_mode == 0) {
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
		atomic_set(&dev_ctx->power_access, 1);
	}
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
	return ret;
}

static void npu_pm_idle_work(struct work_struct *npu_idle_work)
{
	int ret;
	struct npu_power_manage *power_manager = NULL;
	struct devdrv_proc_ctx *proc_ctx = NULL;
	struct devdrv_dev_ctx *dev_ctx = NULL;
	u32 goal_work_mode_set;
	u32 curr_work_mode_set;
	int work_state;

	NPU_DRV_WARN("idle timer work enter\n");
	COND_RETURN_VOID(npu_idle_work == NULL, "idle_work is null\n");
	power_manager = container_of(npu_idle_work, struct npu_power_manage, idle_work.work);
	COND_RETURN_VOID(power_manager == NULL, "power_manager is null\n");
	dev_ctx  = (struct devdrv_dev_ctx *)power_manager->private_data[0];
	COND_RETURN_VOID(dev_ctx == NULL, "dev_ctx is null\n");
	proc_ctx = (struct devdrv_proc_ctx *)power_manager->private_data[1];
	COND_RETURN_VOID(proc_ctx == NULL, "proc_ctx is null\n");

	if (atomic_read(&dev_ctx->pm.task_ref_cnt) != 0) {
		atomic_cmpxchg(&power_manager->idle_wq_processing, WORK_ADDED, WORK_IDLE);
		return;
	}

	work_state = atomic_read(&power_manager->idle_wq_processing);
	if (work_state == WORK_ADDED) {
		mutex_lock(&dev_ctx->npu_power_up_off_mutex);
		curr_work_mode_set = dev_ctx->pm.work_mode;
		if (BITMAP_GET(curr_work_mode_set, NPU_NONSEC) != 0) {
			goal_work_mode_set = curr_work_mode_set & (~(1 << NPU_NONSEC));
			ret = npu_pm_powerdown(proc_ctx, dev_ctx, NPU_NONSEC);
			COND_GOTO_ERROR(ret != 0, FAIL, ret, ret, "fail in power down : ret = %d\n", ret);

			dev_ctx->pm.work_mode = goal_work_mode_set;
			NPU_DRV_WARN("npu nonsec exit succ, curr_workmode_set = 0x%x\n", goal_work_mode_set);
		} else {
			NPU_DRV_WARN("npu nonsec already exit, curr_workmode_set = 0x%x\n", dev_ctx->pm.work_mode);
		}

		if (dev_ctx->pm.work_mode == 0)
			dev_ctx->power_stage = DEVDRV_PM_DOWN;
		mutex_unlock(&dev_ctx->npu_power_up_off_mutex);

		atomic_set(&power_manager->idle_wq_processing, WORK_IDLE);
	} else {
		NPU_DRV_WARN("work state = %d\n", work_state);
	}

	return;
FAIL:
	dev_ctx->pm.work_mode = goal_work_mode_set;
	if (dev_ctx->pm.work_mode == 0)
		dev_ctx->power_stage = DEVDRV_PM_DOWN;
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);
	atomic_set(&power_manager->idle_wq_processing, WORK_IDLE);
}

int npu_pm_resource_init(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx)
{
	int ret;
	struct devdrv_platform_info *plat_info = NULL;
	struct npu_power_manage *power_manager = &dev_ctx->pm;
	struct npu_interframe_idle_manager *idle_manager = NULL;

	plat_info = dev_ctx->plat_info;
	COND_RETURN_ERROR(plat_info == NULL, -1, "plat_info is null\n");

	ret = npu_pm_init(dev_ctx);
	COND_RETURN_ERROR(ret != 0, -1, "npu power manager init failed\n");

	power_manager->private_data[0] = dev_ctx;
	power_manager->private_data[1] = proc_ctx;
	if (devdrv_plat_get_feature_switch(plat_info, DEVDRV_FEATURE_INTERFRAME_IDLE_POWER_DOWN)) {
		idle_manager = &dev_ctx->pm.interframe_idle_manager;
		idle_manager->private_data = proc_ctx;
	}

	return 0;
}

void npu_pm_delay_work_init(struct devdrv_dev_ctx *dev_ctx)
{
	struct npu_power_manage *power_manager = &dev_ctx->pm;
	struct npu_interframe_idle_manager *idle_manager = NULL;
	struct devdrv_platform_info *plat_info = NULL;

	plat_info = dev_ctx->plat_info;
	COND_RETURN_VOID(plat_info == NULL, "plat_info is null\n");

	atomic_set(&power_manager->idle_wq_processing, WORK_IDLE);
	INIT_DELAYED_WORK(&power_manager->idle_work, npu_pm_idle_work);

	if (devdrv_plat_get_feature_switch(plat_info, DEVDRV_FEATURE_INTERFRAME_IDLE_POWER_DOWN)) {
		idle_manager = &dev_ctx->pm.interframe_idle_manager;
		idle_manager->idle_time_out = NPU_INTERFRAME_IDLE_TIME_OUT_DEFAULT_VALUE;
		atomic_set(&idle_manager->wq_processing, WORK_IDLE);
		INIT_DELAYED_WORK(&idle_manager->work, npu_pm_interframe_idle_work);
	}
	return;
}

void npu_pm_adapt_init(struct devdrv_dev_ctx *dev_ctx)
{
	npu_pm_delay_work_init(dev_ctx);
	return ;
}

int npu_pm_get_delta_subip_set(struct npu_power_manage *handle, int work_mode, int pm_ops)
{
	u32 mode_idx;
	u32 delta_subip_set;
	u32 old_subip_set;
	u32 new_subip_set = 0;
	struct npu_pm_work_strategy *work_strategy = NULL;

	work_strategy = npu_pm_get_work_strategy(handle, work_mode);
	delta_subip_set = work_strategy->subip_set.data;
	old_subip_set = handle->cur_subip_set;

	if (pm_ops == POWER_UP) {
		delta_subip_set &= (~old_subip_set);
	} else {
		for (mode_idx = 0; mode_idx < NPU_WORKMODE_MAX; mode_idx++) {
			if (BITMAP_GET(handle->work_mode, mode_idx) && (mode_idx != work_mode)) {
				work_strategy = npu_pm_get_work_strategy(handle, mode_idx);
				new_subip_set |= work_strategy->subip_set.data;
			}
		}
		delta_subip_set = (old_subip_set & (~new_subip_set));
	}

	return delta_subip_set;
}

int devdrv_npu_ctrl_core(u32 dev_id, u32 core_num)
{
	int ret;
	struct devdrv_platform_info *plat_info = NULL;
	struct devdrv_dev_ctx *dev_ctx = NULL;

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("get plat_ops failed\n");
		return -EINVAL;
	}

	if (dev_id > NPU_DEV_NUM) {
		NPU_DRV_ERR("invalid device id %d\n", dev_id);
		return -EINVAL;
	}

	if ((core_num == 0) || (core_num > DEVDRV_PLAT_GET_AICORE_MAX(plat_info))) {
		NPU_DRV_ERR("invalid core num %d\n", core_num);
		return -EINVAL;
	}

	if (DEVDRV_PLAT_GET_RES_CTRL_CORE(plat_info) == NULL) {
		NPU_DRV_ERR("do not support ctrl core num %d\n", core_num);
		return -EINVAL;
	}

	dev_ctx = get_dev_ctx_by_id(dev_id);
	if (dev_ctx == NULL) {
		NPU_DRV_ERR("get device %d ctx fail\n", dev_id);
		return -EINVAL;
	}

	mutex_lock(&dev_ctx->npu_power_up_off_mutex);
	ret = DEVDRV_PLAT_GET_RES_CTRL_CORE(plat_info)(dev_ctx, core_num);
	mutex_unlock(&dev_ctx->npu_power_up_off_mutex);

	if (ret != 0)
		NPU_DRV_ERR("ctrl device %d core num %d fail ret %d\n", dev_id, core_num, ret);
	else
		NPU_DRV_WARN("ctrl device %d core num %d success\n", dev_id, core_num);

	return ret;
}

int npu_interframe_enable(struct devdrv_proc_ctx *proc_ctx, uint32_t enable)
{
	struct devdrv_dev_ctx *dev_ctx = NULL;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -1, "invalid dev ctx\n");

	atomic_set(&dev_ctx->pm.interframe_idle_manager.enable, enable);
	NPU_DRV_DEBUG("interframe enable = %u", atomic_read(&dev_ctx->pm.interframe_idle_manager.enable));

	return 0;
}

