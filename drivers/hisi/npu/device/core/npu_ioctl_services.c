/*
 * npu_ioctl_service.c
 *
 * about npu ioctl service
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
#include "npu_ioctl_services.h"

#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/swap.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/hisi/rdr_pub.h>
#include <dsm/dsm_pub.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched.h>
#include <securec.h>
#include <linux/hisi/hisi_svm.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0))
#include <linux/sched/mm.h>
#endif

#include "npu_common.h"
#include "npu_calc_channel.h"
#include "npu_calc_cq.h"
#include "npu_stream.h"
#include "npu_shm.h"
#include "npu_cma.h"
#include "npu_log.h"
#include "npu_mailbox.h"
#include "npu_event.h"
#include "npu_hwts_event.h"
#include "npu_model.h"
#include "npu_task.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_heart_beat.h"
#include "npu_adapter.h"
#include "npu_ts_sqe.h"
#include "npu_calc_sq.h"
#include "npu_hwts_sqe.h"
#include "npu_pool.h"
#include "npu_iova.h"
#include "npu_svm.h"
#include "npu_doorbell.h"
#include "npu_cache.h"
#include "npu_message.h"
#include "npu_pm_framework.h"

int devdrv_ioctl_alloc_stream(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	devdrv_stream_alloc_ioctl_info_t *para = (devdrv_stream_alloc_ioctl_info_t *)((uintptr_t)arg);
	devdrv_stream_alloc_ioctl_info_t para_1 = {0};
	struct devdrv_stream_info* stream_info = NULL;

	NPU_DRV_DEBUG("enter\n");
	mutex_lock(&proc_ctx->stream_mutex);

	if (copy_from_user_safe(&para_1, (void __user *)(uintptr_t)arg,
		sizeof(devdrv_stream_alloc_ioctl_info_t))) {
		NPU_DRV_ERR("copy from user safe error\n");
		mutex_unlock(&proc_ctx->stream_mutex);
		return -EFAULT;
	}

	ret = devdrv_proc_alloc_stream(proc_ctx, &stream_info, para_1.strategy, para_1.priority);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_alloc_stream failed\n");
		mutex_unlock(&proc_ctx->stream_mutex);
		return -ENOKEY;
	}

	if (copy_to_user_safe((void __user *)(&(para->stream_id)), &stream_info->id, sizeof(int))) {
		NPU_DRV_ERR("copy to user safe stream_id = %d error\n", stream_info->id);
		ret = devdrv_proc_free_stream(proc_ctx, stream_info->id);
		if (ret != 0)
			NPU_DRV_ERR("devdrv_ioctl_free_stream_id = %d error\n", stream_info->id);
		mutex_unlock(&proc_ctx->stream_mutex);
		return -EFAULT;
	}
	bitmap_set(proc_ctx->stream_bitmap, stream_info->id, 1);
	mutex_unlock(&proc_ctx->stream_mutex);

	NPU_DRV_DEBUG("devdrv_ioctl_alloc_stream end\n");
	return 0;
}

int devdrv_proc_get_occupy_stream_id(struct devdrv_proc_ctx *proc_ctx,
	struct devdrv_occupy_stream_id *stream_id, unsigned long arg)
{
	int ret;
	struct npu_id_entity *stream_sub_info = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;

	stream_id->count = 0;

	mutex_lock(&proc_ctx->stream_mutex);
	list_for_each_safe(pos, n, &proc_ctx->sink_stream_list) {
		stream_sub_info = list_entry(pos, struct npu_id_entity, list);
		if (stream_id->count >= DEVDRV_MAX_STREAM_ID) {
			NPU_DRV_ERR("stream_id->count: %u out of range\n", stream_id->count);
			mutex_unlock(&proc_ctx->stream_mutex);
			return -1;
		}
		stream_id->id[stream_id->count] = stream_sub_info->id;
		stream_id->count++;
	}

	list_for_each_safe(pos, n, &proc_ctx->stream_list) {
		stream_sub_info = list_entry(pos, struct npu_id_entity, list);
		if (stream_id->count >= DEVDRV_MAX_STREAM_ID) {
			NPU_DRV_ERR("stream_id->count: %u out of range\n", stream_id->count);
			mutex_unlock(&proc_ctx->stream_mutex);
			return -1;
		}
		stream_id->id[stream_id->count] = stream_sub_info->id;
		stream_id->count++;
	}

	ret = copy_to_user_safe((void *)(uintptr_t)arg, stream_id, sizeof(struct devdrv_occupy_stream_id));
	if (ret != 0) {
		NPU_DRV_ERR("copy_to_user_safe fail\n");
		mutex_unlock(&proc_ctx->stream_mutex);
		return -1;
	}

	mutex_unlock(&proc_ctx->stream_mutex);
	return 0;
}

int devdrv_ioctl_get_occupy_stream_id(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	struct devdrv_occupy_stream_id stream_id ;

	NPU_DRV_DEBUG("enter\n");

	stream_id.sqcq_strategy = 0;

	ret = copy_from_user_safe(&stream_id, (void*)(uintptr_t)arg, sizeof(struct devdrv_occupy_stream_id));
	COND_RETURN_ERROR(ret != 0, ret, "copy from user failed, ret = %d\n", ret);
	NPU_DRV_DEBUG("sqcq_strategy = %d\n",  stream_id.sqcq_strategy);

	ret = devdrv_proc_get_occupy_stream_id(proc_ctx, &stream_id, arg);
	if (ret != 0) {
		NPU_DRV_ERR("get occupy stream id fail ret %d\n", ret);
		return ret;
	}

	return 0;
}

int devdrv_ioctl_alloc_event(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	u32 event_id = 0;
	u32 max_event_id = 0;
	int ret;
	u16 strategy;
	npu_event_alloc_ioctl_info_t para = {0};
	npu_event_alloc_ioctl_info_t *para_1 = (npu_event_alloc_ioctl_info_t *)((uintptr_t)arg);
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -EINVAL, "devdrv_plat_get_info\n");

	mutex_lock(&proc_ctx->event_mutex);
	if (copy_from_user_safe(&para, (void __user *)(uintptr_t)arg,
		sizeof(npu_event_alloc_ioctl_info_t))) {
		NPU_DRV_ERR("copy from user safe error\n");
		mutex_unlock(&proc_ctx->event_mutex);
		return -1;
	}

	if (para.strategy >= EVENT_STRATEGY_TS) {
		NPU_DRV_ERR("proc alloc event failed, invalid input strategy: %u\n", para.strategy);
		mutex_unlock(&proc_ctx->event_mutex);
		return -EINVAL;
	} else if (para.strategy == EVENT_STRATEGY_SINK &&
		DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
		strategy = EVENT_STRATEGY_HWTS;
		max_event_id = DEVDRV_MAX_HWTS_EVENT_ID;
	} else {
		strategy = EVENT_STRATEGY_TS;
		max_event_id = DEVDRV_MAX_EVENT_ID;
	}

	ret = devdrv_proc_alloc_event(proc_ctx, &event_id, strategy);
	if (ret != 0) {
		NPU_DRV_ERR("proc alloc event failed, event id: %u\n", event_id);
		mutex_unlock(&proc_ctx->event_mutex);
		return -1;
	}

	if (copy_to_user_safe((void __user *)(&(para_1->event_id)), &event_id, sizeof(int))) {
		NPU_DRV_ERR("copy to user safe event_id = %u error\n", event_id);
		if (event_id != max_event_id) {
			ret = devdrv_proc_free_event(proc_ctx, event_id, strategy);
			if (ret != 0) {
				NPU_DRV_ERR("proc free event id failed, event id: %u\n", event_id);
				mutex_unlock(&proc_ctx->event_mutex);
				return -1;
			}
			mutex_unlock(&proc_ctx->event_mutex);
			return -1;
		}
	}

	if (strategy == EVENT_STRATEGY_HWTS)
		bitmap_set(proc_ctx->hwts_event_bitmap, event_id, 1);
	else
		bitmap_set(proc_ctx->event_bitmap, event_id, 1);

	mutex_unlock(&proc_ctx->event_mutex);
	return 0;
}

int devdrv_ioctl_alloc_model(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	u32 model_id = 0;
	int ret;

	mutex_lock(&proc_ctx->model_mutex);
	ret = devdrv_proc_alloc_model(proc_ctx, &model_id);
	if (ret != 0) {
		NPU_DRV_ERR("proc alloc model failed, model id: %d\n", model_id);
		mutex_unlock(&proc_ctx->model_mutex);
		return -EFAULT;
	}

	if (copy_to_user_safe((void *)(uintptr_t)arg, &model_id, sizeof(int))) {
		NPU_DRV_ERR("copy to user safe model_id = %d error\n", model_id);
		if (model_id != DEVDRV_MAX_MODEL_ID) {
			ret = devdrv_proc_free_model(proc_ctx, model_id);
			if (ret != 0) {
				NPU_DRV_ERR("proc free model id failed, model id: %d\n", model_id);
				mutex_unlock(&proc_ctx->model_mutex);
				return -EFAULT;
			}
			mutex_unlock(&proc_ctx->model_mutex);
			return -EFAULT;
		}
	}

	bitmap_set(proc_ctx->model_bitmap, model_id, 1);
	mutex_unlock(&proc_ctx->model_mutex);
	return 0;
}

int devdrv_ioctl_alloc_task(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	u32 task_id = 0;
	int ret;

	mutex_lock(&proc_ctx->task_mutex);
	ret = devdrv_proc_alloc_task(proc_ctx, &task_id);
	if (ret != 0) {
		NPU_DRV_ERR("proc alloc task failed, task id: %d\n", task_id);
		mutex_unlock(&proc_ctx->task_mutex);
		return -EFAULT;
	}

	if (copy_to_user_safe((void *)(uintptr_t)arg, &task_id, sizeof(int))) {
		NPU_DRV_ERR("copy to user safe task_id = %d error\n", task_id);
		if (task_id != DEVDRV_MAX_SINK_TASK_ID) {
			ret = devdrv_proc_free_task(proc_ctx, task_id);
			if (ret != 0) {
				NPU_DRV_ERR("proc free task id failed, task id: %d\n", task_id);
				mutex_unlock(&proc_ctx->task_mutex);
				return -EFAULT;
			}
			mutex_unlock(&proc_ctx->task_mutex);
			return -EFAULT;
		}
	}

	bitmap_set(proc_ctx->task_bitmap, task_id, 1);
	mutex_unlock(&proc_ctx->task_mutex);
	return 0;
}

static int devdrv_check_ioctl_free_stream_para(struct devdrv_proc_ctx *proc_ctx, u32 stream_id)
{
	DECLARE_BITMAP(tmp_bitmap, DEVDRV_MAX_STREAM_ID);
	mutex_lock(&proc_ctx->stream_mutex);

	if (stream_id >= DEVDRV_MAX_STREAM_ID) {
		mutex_unlock(&proc_ctx->stream_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_stream_para free_stream_id %d\n", stream_id);
		return -EFAULT;
	}

	bitmap_copy(tmp_bitmap, proc_ctx->stream_bitmap, DEVDRV_MAX_STREAM_ID);
	bitmap_set(tmp_bitmap, stream_id, 1);
	// if tmp_bitmap == proc_ctx->stream_bitmap ,bitmap_equal return 1, or return 0
	if (bitmap_equal(tmp_bitmap, proc_ctx->stream_bitmap, DEVDRV_MAX_STREAM_ID) == 0) {
		mutex_unlock(&proc_ctx->stream_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_stream_para invalidate id %d\n", stream_id);
		return -EFAULT;
	}
	mutex_unlock(&proc_ctx->stream_mutex);

	return 0;
}

static int devdrv_check_ioctl_free_event_para(struct devdrv_proc_ctx *proc_ctx, u32 event_id, u16 input_strategy)
{
	u32 max_event_id = 0;
	u16 strategy;
	DECLARE_BITMAP(tmp_bitmap, DEVDRV_MAX_HWTS_EVENT_ID);
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -EINVAL, "devdrv_plat_get_info\n");

	mutex_lock(&proc_ctx->event_mutex);

	if (input_strategy >= EVENT_STRATEGY_TS) {
		NPU_DRV_ERR("devdrv_check_ioctl_free_event_para strategy %u\n", input_strategy);
		mutex_unlock(&proc_ctx->event_mutex);
		return -EINVAL;
	} else if (input_strategy == EVENT_STRATEGY_SINK &&
		DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1) {
		strategy = EVENT_STRATEGY_HWTS;
		max_event_id = DEVDRV_MAX_HWTS_EVENT_ID;
	} else {
		strategy = EVENT_STRATEGY_TS;
		max_event_id = DEVDRV_MAX_EVENT_ID;
	}

	if (event_id >= max_event_id) {
		mutex_unlock(&proc_ctx->event_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_event_para free_event_id %d\n", event_id);
		return -1;
	}

	if (strategy == EVENT_STRATEGY_TS) {
		bitmap_copy(tmp_bitmap, proc_ctx->event_bitmap, max_event_id);
		bitmap_set(tmp_bitmap, event_id, 1);
		if (bitmap_equal(tmp_bitmap, proc_ctx->event_bitmap, max_event_id) == 0) {
			mutex_unlock(&proc_ctx->event_mutex);
			NPU_DRV_ERR("devdrv_check_ioctl_free_event_para invalidate id %u\n", event_id);
			return -1;
		}
	} else {
		bitmap_copy(tmp_bitmap, proc_ctx->hwts_event_bitmap, max_event_id);
		bitmap_set(tmp_bitmap, event_id, 1);
		if (bitmap_equal(tmp_bitmap, proc_ctx->hwts_event_bitmap, max_event_id) == 0) {
			mutex_unlock(&proc_ctx->event_mutex);
			NPU_DRV_ERR("devdrv_check_ioctl_free_hwts_event_para invalidate id %u\n", event_id);
			return -1;
		}
	}

	mutex_unlock(&proc_ctx->event_mutex);

	return 0;
}

static int devdrv_check_ioctl_free_model_para(struct devdrv_proc_ctx *proc_ctx, unsigned long arg, u32* model_id)
{
	DECLARE_BITMAP(tmp_bitmap, DEVDRV_MAX_MODEL_ID);
	mutex_lock(&proc_ctx->model_mutex);

	if (copy_from_user_safe(model_id, (void *)(uintptr_t)arg, sizeof(int))) {
		mutex_unlock(&proc_ctx->model_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_model_para error\n");
		return -EFAULT;
	}

	if (*model_id >= DEVDRV_MAX_MODEL_ID) {
		mutex_unlock(&proc_ctx->model_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_model_para free_stream_id %d\n", *model_id);
		return -EFAULT;
	}

	bitmap_copy(tmp_bitmap, proc_ctx->model_bitmap, DEVDRV_MAX_MODEL_ID);
	bitmap_set(tmp_bitmap, *model_id, 1);
	// if tmp_bitmap == proc_ctx->model_bitmap ,bitmap_equal return 1, or return 0
	if (bitmap_equal(tmp_bitmap, proc_ctx->model_bitmap, DEVDRV_MAX_MODEL_ID) == 0) {
		mutex_unlock(&proc_ctx->model_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_model_para invalidate model id %u\n", *model_id);
		return -EFAULT;
	}
	mutex_unlock(&proc_ctx->model_mutex);

	return 0;
}

static int devdrv_check_ioctl_free_task_para(struct devdrv_proc_ctx *proc_ctx, unsigned long arg, u32 *task_id)
{
	DECLARE_BITMAP(tmp_bitmap, DEVDRV_MAX_SINK_TASK_ID);
	mutex_lock(&proc_ctx->task_mutex);
	if (copy_from_user_safe(task_id, (void *)(uintptr_t)arg, sizeof(int))) {
		mutex_unlock(&proc_ctx->task_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_task_para error\n");
		return -EFAULT;
	}

	if (*task_id >= DEVDRV_MAX_SINK_TASK_ID) {
		mutex_unlock(&proc_ctx->task_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_task_para free_task_id %d\n",
		*task_id);
		return -EFAULT;
	}

	bitmap_copy(tmp_bitmap, proc_ctx->task_bitmap, DEVDRV_MAX_SINK_TASK_ID);
	bitmap_set(tmp_bitmap, *task_id, 1);
	if (bitmap_equal(tmp_bitmap, proc_ctx->task_bitmap, DEVDRV_MAX_SINK_TASK_ID) == 0) {
		mutex_unlock(&proc_ctx->task_mutex);
		NPU_DRV_ERR("devdrv_check_ioctl_free_task_para invalidate id %d\n", *task_id);
		return -EFAULT;
	}
	mutex_unlock(&proc_ctx->task_mutex);

	return 0;
}


static int devdrv_proc_ioctl_free_stream(struct devdrv_proc_ctx *proc_ctx,
	struct devdrv_stream_free_ioctl_info *para)
{
	int ret;

	NPU_DRV_DEBUG("enter sqcq_strategy %d", para->sqcq_strategy);

	mutex_lock(&proc_ctx->stream_mutex);
	ret = devdrv_proc_free_stream(proc_ctx, para->stream_id);
	mutex_unlock(&proc_ctx->stream_mutex);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_free_stream_id = %d error\n", para->stream_id);
		return -EINVAL;
	}

	return ret;
}

static int devdrv_proc_ioctl_free_event(struct devdrv_proc_ctx *proc_ctx, u32 free_event_id, u16 input_strategy)
{
	int ret;
	u16 strategy;
	struct devdrv_platform_info* plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -EINVAL, "devdrv_plat_get_info\n");

	mutex_lock(&proc_ctx->event_mutex);
	if (input_strategy == EVENT_STRATEGY_SINK &&
		DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1)
		strategy = EVENT_STRATEGY_HWTS;
	else
		strategy = EVENT_STRATEGY_TS;

	ret = devdrv_proc_free_event(proc_ctx, free_event_id, strategy);
	mutex_unlock(&proc_ctx->event_mutex);
	if (ret != 0) {
		NPU_DRV_ERR("free event id = %d error\n", free_event_id);
		ret = -EINVAL;
	}

	return ret;
}

static int devdrv_proc_ioctl_free_model(struct devdrv_proc_ctx *proc_ctx, u32 free_model_id)
{
	int ret;

	mutex_lock(&proc_ctx->model_mutex);
	ret = devdrv_proc_free_model(proc_ctx, free_model_id);
	mutex_unlock(&proc_ctx->model_mutex);
	if (ret != 0) {
		NPU_DRV_ERR("free model id = %d error\n", free_model_id);
		ret = -EINVAL;
	}

	return ret;
}

static int devdrv_proc_ioctl_free_task(struct devdrv_proc_ctx *proc_ctx, u32 free_task_id)
{
	int ret;

	mutex_lock(&proc_ctx->task_mutex);
	ret = devdrv_proc_free_task(proc_ctx, free_task_id);
	mutex_unlock(&proc_ctx->task_mutex);
	if (ret != 0) {
		NPU_DRV_ERR("free task id = %d error\n", free_task_id);
		ret = -EINVAL;
	}

	return ret;
}

int devdrv_ioctl_free_stream(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	struct devdrv_stream_free_ioctl_info para = {0};

	if (copy_from_user_safe(&para, (void *)(uintptr_t)arg, sizeof(para))) {
		NPU_DRV_ERR("devdrv_check_ioctl_free_stream_para error\n");
		return -EFAULT;
	}

	ret = devdrv_check_ioctl_free_stream_para(proc_ctx, para.stream_id);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_free_stream check para fail\n");
		return -EFAULT;
	}

	return devdrv_proc_ioctl_free_stream(proc_ctx, &para);
}

int devdrv_ioctl_free_event(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	npu_event_free_ioctl_info_t para = {0};

	if (copy_from_user_safe(&para, (void *)(uintptr_t)arg, sizeof(para))) {
		NPU_DRV_ERR("copy from user safe error\n");
		return -1;
	}

	ret = devdrv_check_ioctl_free_event_para(proc_ctx, para.event_id, para.strategy);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_free_event check para fail\n");
		return -EFAULT;
	}

	return devdrv_proc_ioctl_free_event(proc_ctx, para.event_id, para.strategy);
}

int devdrv_ioctl_free_model(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	int model_id = 0;

	ret = devdrv_check_ioctl_free_model_para(proc_ctx, arg, &model_id);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_free_model check para fail\n");
		return -EFAULT;
	}

	return devdrv_proc_ioctl_free_model(proc_ctx, model_id);
}

int devdrv_ioctl_free_task(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	int task_id = 0;

	ret = devdrv_check_ioctl_free_task_para(proc_ctx, arg, &task_id);
	if (ret) {
		NPU_DRV_ERR("devdrv_ioctl_free_task check para fail\n");
		return -EFAULT;
	}

	return devdrv_proc_ioctl_free_task(proc_ctx, task_id);
}

static int davinci_va_to_pa(struct davinci_area_info *params)
{
	struct mm_struct *mm = current->mm;
	unsigned long addr = params->va;

	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;

	pgd = pgd_offset(mm, addr);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		return -EINVAL;

	pud = pud_offset(pgd, addr);
	if (pud_none(*pud) || pud_bad(*pud))
		return -EINVAL;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		return -EINVAL;

	pte = pte_offset_map(pmd, addr);
	if (pte_none(*pte) || !pte_present(*pte))
		return -EINVAL;

	params->pa = PFN_PHYS(pte_pfn(*pte));

	return 0;
}

static int devdrv_ioctl_davinci_va_to_pa(u64 arg)
{
	int ret;
	struct davinci_area_info params;

	params.va = 0;
	params.pa = 0;
	params.len = 0;

	ret = copy_from_user_safe(&params, (void __user *)(uintptr_t)arg, sizeof(params));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy davinci_area_info params, ret = %d\n", ret);
		return -EINVAL;
	}
	ret = davinci_va_to_pa(&params);
	if (ret != 0) {
		NPU_DRV_ERR("fail to change the virtual pte, ret = %d\n", ret);
		return ret;
	}
	ret = copy_to_user_safe((void __user *)(uintptr_t)arg, &params, sizeof(params));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy phys params to user space, ret = %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

static int devdrv_ioctl_get_chip_info(u64 arg)
{
	int ret;
	struct devdrv_chip_info info = {0};
	struct devdrv_mem_desc *l2_desc = devdrv_plat_get_reg_desc(DEVDRV_REG_L2BUF_BASE);

	NPU_DRV_DEBUG("get devdrv_ioctl_get_chip_info arg = 0x%llx\n", arg);

	if (l2_desc == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_reg_desc failed\n");
		return -EFAULT;
	}

	info.l2_size = l2_desc->len + 1; // becasue of dts will minus 1

	ret = copy_to_user_safe((void __user *)(uintptr_t)arg, &info, sizeof(info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy chip_info params to " "user space,ret = %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

static int hisi_svm_get_ttbr(u64 *ttbr, u64 *tcr)
{
	unsigned long asid;
	struct mm_struct *mm = NULL;
	if ((ttbr == NULL) || (tcr == NULL)) {
		NPU_DRV_ERR("invalid ttbr or tcr\n");
		return -EINVAL;
	}
	mm = get_task_mm(current);
	if (mm == NULL) {
		NPU_DRV_ERR("get mm is null\n");
		return -EINVAL;
	}
	// flush cache? ion_flush_all_cpus_caches
	asid = ASID(mm);
	*ttbr = virt_to_phys(mm->pgd) | (asid << 48);
	*tcr  = read_sysreg(tcr_el1);

	NPU_DRV_DEBUG("pgdaddr:0x:%pK, context:0x%pK, pa:0x%pK\n", mm->pgd, (u64 *)(mm->pgd),
		(void *)(uintptr_t)virt_to_phys(mm->pgd));

	mmput(mm);

	NPU_DRV_DEBUG("asid:%lu ,ttbr:0x%pK, tcr:0x%pK\n", asid,
		(void *)(uintptr_t)*ttbr, (void *)(uintptr_t)*tcr);

	return 0;
}


static int devdrv_ioctl_get_svm_ssid(struct devdrv_dev_ctx *dev_ctx, u64 arg)
{
	int ret;
	struct process_info info = {0};
	u16 ssid  = 0;

	ret = copy_from_user_safe(&info, (void __user *)(uintptr_t)arg, sizeof(info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy process_info params, ret = %d\n",
			ret);
		return -EINVAL;
	}

	ret = devdrv_get_ssid_bypid(dev_ctx->devid, current->tgid, current->tgid, &ssid, &info.ttbr, &info.tcr);
	if (ret != 0) {
		NPU_DRV_ERR("fail to get ssid, ret = %d\n", ret);
		return ret;
	}

	info.pasid = ssid;
	ret = hisi_svm_get_ttbr(&info.ttbr, &info.tcr);
	if (ret != 0) {
		NPU_DRV_ERR("fail to get process info, ret = %d\n", ret);
		return ret;
	}

	NPU_DRV_DEBUG("pid=%d get ssid 0x%x ttbr 0x%llx tcr 0x%llx \n", current->pid, info.pasid, info.ttbr, info.tcr);

	ret = copy_to_user_safe((void __user *)(uintptr_t)arg, &info, sizeof(info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy process info params to user space," "ret = %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

int devdrv_ioctl_alloc_cm(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u64 arg)
{
	struct devdrv_cm_msg msg = {0};
	struct devdrv_cm_info info = {0};
	struct devdrv_platform_info *plat_info = NULL;
	void* cpu_addr = NULL;
	dma_addr_t dma_handle = 0;
	u32 share_num = 0;
	u64 size = 0;
	int ret;
	u32 pgoff;
	u64 cm_va;
	COND_RETURN_ERROR(proc_ctx == NULL, -EINVAL, "proc_ctx is null\n");
	COND_RETURN_ERROR(dev_ctx == NULL, -EINVAL, "dev_ctx is null\n");

	mutex_lock(&dev_ctx->cm_mutex_t);
	ret = copy_from_user_safe(&msg, (void __user *)(uintptr_t)arg, sizeof(msg));
	COND_GOTO_ERROR(ret != 0, fail, ret, -EINVAL, "fail to copy npu cm msg params, ret = %d\n", ret);

	size = msg.size;
	COND_GOTO_ERROR(devdrv_is_cm_available(dev_ctx, size), fail, ret, -ENOMEM,
		"exceed the limit, npu cm is not enough\n");

	plat_info = devdrv_plat_get_info();
	COND_GOTO_ERROR(plat_info == NULL, fail, ret, -EINVAL, "devdrv_plat_get_info failed\n");

	if (dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].valid == 0 &&
		dev_ctx->cm->mem_info[SHARE_NUM_OCCUPIED].size != 0) {
		NPU_DRV_INFO("devdrv_ioctl_alloc_cm SHARE_NUM_OCCUPIED enter\n");
		ret = devdrv_save_cm_info_occupied(dev_ctx, &share_num);
		COND_GOTO_ERROR(ret != 0, fail, ret, -ENOMEM, "fail to save continue mem info ret %d\n", ret);

		msg.share_num = share_num;
	} else {
		cpu_addr = dma_alloc_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size, &dma_handle, GFP_KERNEL);
		COND_GOTO_ERROR(cpu_addr == NULL, fail, ret, -ENOMEM, "fail to malloc cma mem size 0x%llx\n", size);

		info.cpu_addr = cpu_addr;
		info.dma_handle = dma_handle;
		info.size = size;

		ret = devdrv_save_cm_info(dev_ctx, &info, &share_num);
		if (ret != 0) {
			mutex_unlock(&dev_ctx->cm_mutex_t);
			NPU_DRV_ERR("fail to save continue mem info ret %d\n", ret);
			dma_free_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size, cpu_addr, dma_handle);
			return -ENOMEM;
		}

		msg.share_num = share_num;
	}
	proc_ctx->map_ops_flag = true; /* for forbidding the mmap in user space */

	pgoff = MAP_COMBINE(MAP_CONTIGUOUS_MEM, share_num);

	cm_va = vm_mmap(proc_ctx->filep, 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, pgoff * PAGE_SIZE);
	if (IS_ERR_VALUE((uintptr_t)cm_va)) {
		NPU_DRV_ERR("vm_mmap failed\n");
		proc_ctx->map_ops_flag = false;
		goto error_free;
	}
	proc_ctx->map_ops_flag = false;
	NPU_DRV_INFO("vm_mmap cm_va:0x%llx, share_num=%d, pgoff=0x%x\n", cm_va, share_num, pgoff);
	msg.user_va = cm_va;
	dev_ctx->cm->mem_info[share_num].user_va = cm_va;
	ret = copy_to_user_safe((void __user *)(uintptr_t)arg, &msg, sizeof(msg));
	COND_GOTO_ERROR(ret != 0, error_free, ret, ret, "copy_to_user_safe failed, ret = %d\n", ret);

	(void)devdrv_inc_cm_total_size(dev_ctx, size);
	mutex_unlock(&dev_ctx->cm_mutex_t);

	return 0;

error_free:
	if (share_num != SHARE_NUM_OCCUPIED) {
		dma_free_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size, cpu_addr, dma_handle);
		(void)devdrv_delete_cm_info(dev_ctx, share_num);
	}
	mutex_unlock(&dev_ctx->cm_mutex_t);
	NPU_DRV_ERR("fail to copy cm msg params to " "user space, ret = %d\n", ret);
	return -EINVAL;
fail:
	mutex_unlock(&dev_ctx->cm_mutex_t);
	return ret;
}

int devdrv_ioctl_free_cm(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u64 arg)
{
	int ret;
	u32 share_num = 0 ;
	u64 size;
	struct devdrv_platform_info* plat_info = NULL;
	struct devdrv_cm_info *cm_info = NULL;
	COND_RETURN_ERROR(dev_ctx == NULL, -EINVAL, "dev_ctx is null\n");

	mutex_lock(&dev_ctx->cm_mutex_t);
	ret = copy_from_user_safe(&share_num, (void __user *)(uintptr_t)arg, sizeof(share_num));
	COND_GOTO_ERROR(ret != 0, fail, ret, -EINVAL, "fail to copy share_num params, ret = %d\n", ret);

	NPU_DRV_INFO("from user msg: share_num = %d \n", share_num);

	ret = devdrv_is_cm_valid(dev_ctx, share_num);
	COND_GOTO_ERROR(ret != 0, fail, ret, -EINVAL, "invalid share_num , size or cm.valid info \n");

	cm_info = &dev_ctx->cm->mem_info[share_num];

	plat_info = devdrv_plat_get_info();
	COND_GOTO_ERROR(plat_info == NULL, fail, ret, -EINVAL, "devdrv_plat_get_info failed\n");

	size = cm_info->size;
	COND_GOTO_ERROR((dev_ctx->cm->total_size) < size, fail, ret, -EINVAL, "invalid size 0x%llx total size 0x%llx\n",
		size, dev_ctx->cm->total_size);

	NPU_DRV_INFO("valid size 0x%llx total size 0x%llx\n", size, dev_ctx->cm->total_size);

	ret = vm_munmap(cm_info->user_va, cm_info->size);
	NPU_DRV_DEBUG("vm_munmap end.ret=%d, user_va=0x%lx, size=0x%lx\n", ret, cm_info->user_va, cm_info->size);
	if (ret < 0)
		NPU_DRV_ERR("vm_munmap failed. ret=%d, size=0x%lx\n",
			ret, cm_info->size);

	if (share_num == SHARE_NUM_OCCUPIED) {
		NPU_DRV_INFO("devdrv_ioctl_free_cm SHARE_NUM_OCCUPIED enter\n");
		(void)devdrv_delete_cm_info_occupied(dev_ctx);
	} else {
		dma_free_coherent(DEVDRV_PLAT_GET_PDEV(plat_info), size,
			dev_ctx->cm->mem_info[share_num].cpu_addr,
			dev_ctx->cm->mem_info[share_num].dma_handle);

		(void)devdrv_delete_cm_info(dev_ctx, share_num);
	}

	(void)devdrv_dec_cm_total_size(dev_ctx, size);
	mutex_unlock(&dev_ctx->cm_mutex_t);

	return 0;
fail:
	mutex_unlock(&dev_ctx->cm_mutex_t);
	return ret;
}

int devdrv_ioctl_attach_syscache(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret = 0;
	struct devdrv_attach_sc msg = {0};

	ret = copy_from_user_safe(&msg, (void __user *)(uintptr_t)arg, sizeof(msg));
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to copy npu sc attach params, ret = %d\n", ret);

	/* syscahce attach interface with offset */
	ret = devdrv_plat_attach_sc(msg.fd, msg.offset, (size_t)msg.size);
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to devdrV_plat_attach_sc, ret = %d\n", ret);

	return ret;
}

int devdrv_ioctl_enter_workwode(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	npu_work_mode_info_t work_mode_info = {0};
	struct devdrv_dev_ctx *dev_ctx = NULL;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -1, "invalid dev ctx\n");

	ret = copy_from_user_safe(&work_mode_info, (void __user *)(uintptr_t)arg, sizeof(npu_work_mode_info_t));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy sec_mode_info params, ret = %d\n", ret);
		return -EINVAL;
	}
	COND_RETURN_ERROR(work_mode_info.work_mode >= NPU_WORKMODE_MAX, -EINVAL,
		"invalid work_mode = %u\n", work_mode_info.work_mode);

	dev_ctx->pm.work_mode_flags = work_mode_info.flags;
	NPU_DRV_DEBUG("workmode %u flags 0x%x\n", work_mode_info.work_mode, work_mode_info.flags);
	ret = npu_pm_enter_workmode(proc_ctx, dev_ctx, work_mode_info.work_mode);
	if (ret != 0) {
		NPU_DRV_ERR("fail to enter workwode: %u, ret = %d\n", work_mode_info.work_mode, ret);
		return -EINVAL;
	}

	return ret;
}

int devdrv_ioctl_exit_workwode(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	uint32_t workmode;
	struct devdrv_power_down_info power_down_info = {{0}, 0};
	struct devdrv_dev_ctx *dev_ctx = NULL;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -1, "invalid dev ctx\n");

	ret = copy_from_user_safe(&power_down_info, (void __user *)(uintptr_t)arg, sizeof(power_down_info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy sec_mode_info params, ret = %d\n", ret);
		return -EINVAL;
	}

	workmode = power_down_info.secure_info.work_mode;
	NPU_DRV_DEBUG("workmode = %d\n", workmode);
	COND_RETURN_ERROR(workmode >= NPU_WORKMODE_MAX, -EINVAL, "invalid workmode = %u\n", workmode);

	dev_ctx->pm.work_mode_flags = 0; /* clear flags */

	ret = npu_pm_exit_workmode(proc_ctx, dev_ctx, workmode);
	if (ret != 0) {
		NPU_DRV_ERR("fail to exit workwode: %d, ret = %d\n", workmode, ret);
		return ret;
	}
	return ret;
}

int devdrv_ioctl_set_limit(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	u32 value;
	struct devdrv_limit_time_info limit_time_info = {0, 0};
	struct devdrv_dev_ctx *dev_ctx = NULL;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(dev_ctx == NULL, -1, "invalid dev ctx\n");

	ret = copy_from_user_safe(&limit_time_info, (void __user *)(uintptr_t)arg, sizeof(limit_time_info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy sec_mode_info params, ret = %d\n", ret);
		return -EINVAL;
	}

	value = limit_time_info.time_out;
	if (limit_time_info.type == NPU_LOW_POWER_TIMEOUT) {
		COND_RETURN_ERROR(value < NPU_IDLE_TIME_OUT_MIN_VALUE, -EINVAL, "value :%u is too small\n", value);
		dev_ctx->pm.npu_idle_time_out = value;
	} else if (limit_time_info.type == NPU_STREAM_SYNC_TIMEOUT) {
		COND_RETURN_ERROR(value < NPU_TASK_TIME_OUT_MIN_VALUE, -EINVAL, "value :%u is too small\n", value);
		COND_RETURN_ERROR(value > NPU_TASK_TIME_OUT_MAX_VALUE, -EINVAL, "value :%u is too big\n", value);
		dev_ctx->pm.npu_task_time_out = value;
	} else {
		ret = -1;
		NPU_DRV_ERR("limit type wrong!, type = %u\n", limit_time_info.type);
	}

	return ret;
}

int npu_ioctl_enable_feature(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	uint32_t feature_id;

	ret = copy_from_user_safe(&feature_id, (void __user *)(uintptr_t)arg, sizeof(uint32_t));
	if (ret != 0) {
		NPU_DRV_ERR("copy_from_user_safe error\n");
		return -EINVAL;
	}
	ret = npu_feature_enable(proc_ctx, feature_id, 1);

	return ret;
}

int npu_ioctl_disable_feature(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	uint32_t feature_id;

	ret = copy_from_user_safe(&feature_id, (void __user *)(uintptr_t)arg, sizeof(uint32_t));
	if (ret != 0) {
		NPU_DRV_ERR("copy_from_user_safe error\n");
		return -EINVAL;
	}

	ret = npu_feature_enable(proc_ctx, feature_id, 0);
	return ret;
}

int devdrv_ioctl_reboot(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u64 arg)
{
	UNUSED(proc_ctx);
	UNUSED(dev_ctx);
	UNUSED(arg);
	return 0;
}

int devdrv_verify_model_desc(devdrv_model_desc_t model_desc)
{
	u8 idx;

	if (model_desc.model_id >= DEVDRV_MAX_MODEL_ID) {
		NPU_DRV_ERR("user model_id= %u invalid\n", model_desc.model_id);
		return -1;
	}
	if (model_desc.stream_cnt > DEVDRV_MODEL_STREAM_NUM ||
		model_desc.stream_cnt == 0) {
		NPU_DRV_ERR("user stream_cnt= %u invalid\n", model_desc.stream_cnt);
		return -1;
	}

	for (idx = 0; idx < model_desc.stream_cnt; idx++) {
		if (model_desc.stream_id[idx] < DEVDRV_MAX_NON_SINK_STREAM_ID ||
			model_desc.stream_id[idx] >= DEVDRV_MAX_STREAM_ID) {
			NPU_DRV_ERR("user sink stream_id[%u]= %u invalid\n",
				idx, model_desc.stream_id[idx]);
			return -1;
		}
		if ((model_desc.stream_id[idx] < (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM)) &&
			(model_desc.stream_tasks[idx] > DEVDRV_MAX_LONG_HWTS_SQ_DEPTH ||
			model_desc.stream_tasks[idx] == 0)) {
			NPU_DRV_ERR("user sink long stream_tasks[%u]= %u invalid\n",
				idx, model_desc.stream_tasks[idx]);
			return -1;
		}
		if ((model_desc.stream_id[idx] >= (DEVDRV_MAX_NON_SINK_STREAM_ID + DEVDRV_MAX_LONG_HWTS_SQ_NUM)) &&
			(model_desc.stream_tasks[idx] > DEVDRV_MAX_HWTS_SQ_DEPTH ||
			model_desc.stream_tasks[idx] == 0)) {
			NPU_DRV_ERR("user sink stream_tasks[%u]= %u invalid\n",
				idx, model_desc.stream_tasks[idx]);
			return -1;
		}
		if (model_desc.stream_addr[idx] == NULL) {
			NPU_DRV_ERR("user sink stream_addr[%u] is NULL invalid\n", idx);
			return -1;
		}
	}
	return 0;
}

static int devdrv_load_hwts_sqe(devdrv_model_desc_t *model_desc, int stream_idx,
	devdrv_model_desc_info_t *model_desc_info, struct devdrv_entity_info *hwts_sq_sub_info)
{
	void *stream_buf_addr = NULL;
	int stream_len, ret, sqe_num;

	COND_RETURN_ERROR(model_desc == NULL, -EINVAL, "model_desc is invalid\n");
	COND_RETURN_ERROR(stream_idx < 0 || stream_idx >= DEVDRV_MODEL_STREAM_NUM,
		-EINVAL, "stream_idx(%d) is invalid\n", stream_idx);
	COND_RETURN_ERROR(model_desc_info == NULL, -EINVAL, "model_desc_info is invalid\n");
	COND_RETURN_ERROR(hwts_sq_sub_info == NULL, -EINVAL, "hwts_sq_sub_info is invalid\n");
	// copy tasks of one stream to temp buff
	stream_len = model_desc->stream_tasks[stream_idx] * DEVDRV_RT_TASK_SIZE;
	stream_buf_addr = vmalloc(stream_len);
	COND_RETURN_ERROR(stream_buf_addr == NULL, -ENOMEM,
		"vmalloc stream_buf memory size= %d failed\n", stream_len);

	ret = copy_from_user_safe(stream_buf_addr,
		(void __user *)(uintptr_t)model_desc->stream_addr[stream_idx], stream_len);
	if (ret != 0) {
		vfree(stream_buf_addr);
		NPU_DRV_ERR("fail to copy stream_buf, ret= %d\n", ret);
		return -EINVAL;
	}

	// verify hwts_sqe
	ret = devdrv_verify_hwts_sqe(stream_buf_addr, stream_len);
	if (ret != 0) {
		vfree(stream_buf_addr);
		NPU_DRV_ERR("devdrv_verify_hwts_sqe fail, length= %d, ret= %d\n",
			stream_len, ret);
		return -1;
	}

	// save hwts_sqe
	NPU_DRV_DEBUG("hwts_sq_vir_addr:0x%lx, stream_buf_addr:%pK, stream_id:%u, stream_len:%d\n",
		hwts_sq_sub_info->vir_addr, stream_buf_addr, model_desc->stream_id[stream_idx], stream_len);
	sqe_num = devdrv_format_hwts_sqe((void *)(uintptr_t)hwts_sq_sub_info->vir_addr,
		stream_buf_addr, stream_len, model_desc_info);

	vfree(stream_buf_addr);
	return sqe_num;
}

int devdrv_ioctl_load_model(struct devdrv_proc_ctx *proc_ctx, struct devdrv_dev_ctx *dev_ctx, u64 arg)
{
	int ret, idx, sqe_num;
	devdrv_model_desc_t model_desc = {0};
	struct devdrv_stream_info *stream_info = NULL;
	struct devdrv_hwts_sq_info *hwts_sq_info = NULL;
	devdrv_model_desc_info_t *model_desc_info = NULL;
	struct devdrv_entity_info *hwts_sq_sub_info = NULL;

	NPU_DRV_DEBUG("devdrv_ioctl_load_model enter\n");
	ret = copy_from_user_safe(&model_desc, (void __user *)(uintptr_t)arg, sizeof(devdrv_model_desc_t));
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to copy model_desc, ret= %d\n", ret);

	NPU_DRV_DEBUG("model_id= %u, stream_cnt= %u\n", model_desc.model_id, model_desc.stream_cnt);
	ret = devdrv_verify_model_desc(model_desc);
	COND_RETURN_ERROR(ret != 0, -1, "devdrv_verify_model_desc fail, ret= %d\n", ret);

	model_desc_info = devdrv_calc_model_desc_info(proc_ctx->devid, model_desc.model_id);
	COND_RETURN_ERROR(model_desc_info == NULL, -1, "model_desc_info is NULL\n");
	ret = memset_s(model_desc_info, sizeof(devdrv_model_desc_info_t), 0, sizeof(devdrv_model_desc_info_t));
	COND_RETURN_ERROR(ret != 0, -1, "memset_s model_desc_info fail, ret = %d\n", ret);

	model_desc_info->model_id = model_desc.model_id;
	model_desc_info->stream_cnt = model_desc.stream_cnt;

	for (idx = 0; idx < model_desc.stream_cnt; idx++) {
		stream_info = devdrv_calc_stream_info(proc_ctx->devid, model_desc.stream_id[idx]);
		COND_RETURN_ERROR(stream_info == NULL, -1,
			"stream_id= %d stream_info is null\n", model_desc.stream_id[idx]);

		stream_info->smmu_substream_id = 1; // V200 useless field, should use modelExecute task field

		hwts_sq_info = devdrv_calc_hwts_sq_info(proc_ctx->devid, stream_info->sq_index);
		COND_RETURN_ERROR(hwts_sq_info == NULL, -1, "stream= %d, sq_index= %u, hwts_sq_info is null\n",
			stream_info->id, stream_info->sq_index);

		hwts_sq_sub_info = (struct devdrv_entity_info *)(uintptr_t)hwts_sq_info->hwts_sq_sub;
		COND_RETURN_ERROR(hwts_sq_sub_info == NULL, -1, "stream_id= %d sq_sub is null\n", stream_info->id);

		sqe_num = devdrv_load_hwts_sqe(&model_desc, idx, model_desc_info, hwts_sq_sub_info);
		if (sqe_num <= 0) {
			NPU_DRV_ERR("formate hwts_sq failed, stream:%d\n", stream_info->id);
			return -1;
		}

		model_desc_info->stream_id[idx] = model_desc.stream_id[idx];
		hwts_sq_info->head = 0;
		hwts_sq_info->tail = sqe_num;
	}

	NPU_DRV_DEBUG("devdrv_ioctl_load_model end\n");
	return 0;
}

static int devdrv_check_ioctl_report_wait_para(unsigned long arg, int* time_out)
{
	if (copy_from_user_safe(time_out, (void*)(uintptr_t)arg, sizeof(int))) {
		NPU_DRV_ERR("copy cq report time_out para from user error\n");
		return -EFAULT;
	}

	return 0;
}

int devdrv_ioctl_report_wait(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	int timeout_in = 0;
	int timeout_out = 0;

	ret = devdrv_check_ioctl_report_wait_para(arg, &timeout_in);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_report_wait fail\n");
		return -ENODEV;
	}

	ret = npu_report_wait(proc_ctx, &timeout_out, timeout_in);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_report_wait timeout = %d\n", timeout_out);
		return -ENODEV;
	}

	if (copy_to_user_safe((void *)(uintptr_t)arg, &timeout_out, sizeof(int))) {
		NPU_DRV_ERR("REPORT_POLLING copy_to_user_safe error\n");
		return -ENODEV;
	}

	return 0;
}

int devdrv_ioctl_report_sync_wait(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	struct devdrv_report_sync_wait_info wait_info;

	ret = copy_from_user_safe(&wait_info, (void __user *)(uintptr_t)arg, \
		sizeof(struct devdrv_report_sync_wait_info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy wait_info, ret= %d\n", ret);
		return -EINVAL;
	}

	if ((wait_info.stream_id >= DEVDRV_MAX_NON_SINK_STREAM_ID) ||
		(wait_info.report_addr == (u64)NULL)) {
		NPU_DRV_ERR("failed to check para, stream_id = %d\n", wait_info.stream_id);
		return -ENODEV;
	}

	ret = npu_report_sync_wait(proc_ctx, &wait_info);
	if (copy_to_user_safe((void *)(uintptr_t)&(((struct devdrv_report_sync_wait_info *)(uintptr_t)arg)->result), &ret, sizeof(int))) {
		NPU_DRV_ERR("failed to copy_to_user_safe error\n");
		return -ENODEV;
	}

	return 0;
}

int devdrv_check_ioctl_custom_para(struct devdrv_proc_ctx *proc_ctx, unsigned long arg,
	devdrv_custom_para_t* custom_para, struct devdrv_dev_ctx** dev_ctx)
{
	int ret;
	ret = copy_from_user_safe(custom_para, (void __user *)(uintptr_t)arg, sizeof(devdrv_custom_para_t));
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_check_ioctl_custom_para,ret = %d\n", ret);
		return -EINVAL;
	}

	if (custom_para->arg == 0) {
		NPU_DRV_ERR("devdrv_check_ioctl_custom_para invalid arg\n");
		return -EINVAL;
	}

	*dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	if ((*dev_ctx) == NULL) {
		NPU_DRV_ERR("devdrv_proc_ioctl_custom %d of npu process %d is null\n",
			proc_ctx->devid, proc_ctx->pid);
		return -1;
	}

	return ret;
}

int devdrv_ioctl_send_request(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	devdrv_ts_comm_sqe_t comm_task = {0};
	int ret = 0;

	ret = copy_from_user_safe(&comm_task, (void __user *)(uintptr_t)arg, sizeof(devdrv_ts_comm_sqe_t));
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to copy comm_task, ret= %d\n", ret);

	return npu_send_request(proc_ctx, &comm_task);
}

int devdrv_ioctl_receive_response(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	struct devdrv_receive_response_info report_info = {0};
	int ret = 0;
	NPU_DRV_DEBUG("devdrv_ioctl_receive_report enter\n");

	ret = copy_from_user_safe(&report_info, (void __user *)(uintptr_t)arg, sizeof(struct devdrv_receive_response_info));
	if (ret != 0) {
		NPU_DRV_ERR("fail to copy comm_report, ret= %d\n", ret);
		return -EINVAL;
	}

	ret = npu_receive_response(proc_ctx, &report_info);
	if(ret != 0)
		NPU_DRV_ERR("fail to receive_response ret= %d\n", ret);

	if(copy_to_user_safe((void *)(uintptr_t)arg, &report_info, sizeof(report_info))) {
		NPU_DRV_ERR("ioctl_receive_response copy_to_user_safe error\n");
		return -EINVAL;
	}

	NPU_DRV_DEBUG("devdrv_ioctl_receive_report exit\n");

	return ret;
}

static int devdrv_proc_ioctl_custom(struct devdrv_proc_ctx *proc_ctx, const devdrv_custom_para_t* custom_para,
	struct devdrv_dev_ctx *dev_ctx)
{
	int ret;
	switch (custom_para->cmd) {
	case DEVDRV_IOC_VA_TO_PA:
		ret = devdrv_ioctl_davinci_va_to_pa(custom_para->arg);
		break;
	case DEVDRV_IOC_GET_SVM_SSID:
		ret = devdrv_ioctl_get_svm_ssid(dev_ctx, custom_para->arg);
		break;
	case DEVDRV_IOC_GET_CHIP_INFO:
		ret = devdrv_ioctl_get_chip_info(custom_para->arg);
		break;
	case DEVDRV_IOC_ALLOC_CONTIGUOUS_MEM:
		// need lock because cm_info global resoure
		ret = devdrv_ioctl_alloc_cm(proc_ctx, dev_ctx, custom_para->arg);
		break;
	case DEVDRV_IOC_FREE_CONTIGUOUS_MEM:
		ret = devdrv_ioctl_free_cm(proc_ctx, dev_ctx, custom_para->arg);
		break;
	case DEVDRV_IOC_REBOOT:
		ret = devdrv_ioctl_reboot(proc_ctx, dev_ctx, custom_para->arg);
		break;
	case DEVDRV_IOC_LOAD_MODEL_BUFF: // for sink stream(v200)
		ret = devdrv_ioctl_load_model(proc_ctx, dev_ctx, custom_para->arg);
		break;
	default:
		NPU_DRV_ERR("invalid custom cmd 0x%x \n", custom_para->cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

int devdrv_ioctl_custom(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	devdrv_custom_para_t custom_para = {0};
	struct devdrv_dev_ctx* dev_ctx = NULL;

	ret = devdrv_check_ioctl_custom_para(proc_ctx, arg, &custom_para, &dev_ctx);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_custom,ret = %d\n", ret);
		return -EINVAL;
	}

	ret = devdrv_proc_ioctl_custom(proc_ctx, &custom_para, dev_ctx);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_ioctl_custom call devdrv_proc_ioctl_custom, ret = %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

/*
 * new add for TS timeout function
 */
int devdrv_ioctl_get_ts_timeout(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	uint64_t exception_code = 0;
	int ret = 0;
	NPU_DRV_DEBUG("enter\n");

	if (copy_from_user_safe(&exception_code, (void *)(uintptr_t)arg, sizeof(uint64_t))) {
		NPU_DRV_ERR("copy_from_user_safe error\n");
		return -EFAULT;
	}

	if (exception_code < (uint64_t)MODID_NPU_START || exception_code > (uint64_t)MODID_NPU_EXC_END) {
		NPU_DRV_ERR("expection code %llu out of npu range\n", exception_code);
		return -1;
	}

	if (npu_rdr_exception_is_count_exceeding(exception_code) == 0) {
		/* receive TS exception */
		NPU_DRV_WARN("call rdr_system_error\n");
		rdr_system_error(exception_code, 0, 0);
	}

	return ret;
}

/* ION memory map*/
int devdrv_ioctl_svm_bind_pid(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	pid_t pid;

	ret = copy_from_user_safe(&pid, (void __user *)(uintptr_t)arg, sizeof(pid));
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to copy svm params, ret = %d\n", ret);

	ret = devdrv_insert_item_bypid(proc_ctx->devid, current->tgid, pid);
	COND_RETURN_ERROR(ret != 0, -EINVAL, "devdrv_insert_item_bypid fail, ret = %d\n", ret);

	return ret;
}

/* ION memory map */
int devdrv_ioctl_svm_unbind_pid(struct devdrv_proc_ctx *proc_ctx, unsigned long arg)
{
	int ret;
	pid_t pid;

	ret = copy_from_user_safe(&pid, (void __user *)(uintptr_t)arg, sizeof(pid));
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to copy svm params, ret = %d\n", ret);

	ret = devdrv_release_item_bypid(proc_ctx->devid, current->tgid, pid);
	COND_RETURN_ERROR(ret != 0, -EINVAL, "devdrv_release_item_bypid fail, ret = %d\n", ret);

	return ret;
}

long devdrv_npu_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	struct devdrv_proc_ctx *proc_ctx = NULL;
	int ret;

	proc_ctx = (struct devdrv_proc_ctx *)filep->private_data;
	if (proc_ctx == NULL || arg == 0) {
		NPU_DRV_ERR("invalid devdrv_npu_ioctl parameter,arg = 0x%lx,cmd = %d\n", arg, cmd);
		return -EINVAL;
	}

	ret = devdrv_proc_npu_ioctl_call(proc_ctx, cmd, arg);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_npu_ioctl process failed,arg = %d\n", cmd);
		return -EINVAL;
	}

	return ret;
}

