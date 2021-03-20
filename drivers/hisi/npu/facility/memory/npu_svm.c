/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description:
 */
#include "npu_svm.h"

#include <linux/err.h>
#include <linux/iommu.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/hisi-iommu.h>
#include <linux/pid.h>
#include <linux/hisi/hisi_svm.h>
#include <securec.h>

#include "npu_log.h"
#include "npu_adapter.h"
#include "hisi_smmu.h"

struct t_svm_pid_info {
	pid_t manager_pid; // the pid of the process that opened the manager device
	pid_t svm_pid;
	struct task_struct *task;
	struct hisi_svm *hisi_svm;
};
#define DRVDEV_APP_MAX_NUM 20
struct t_svm_manager {
	struct mutex lock;
	uint32_t item_num;
	struct t_svm_pid_info svm_pid_info[DRVDEV_APP_MAX_NUM];
};

int devdrv_svm_manager_init(uint32_t devid)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}

	svm_manager = kmalloc(sizeof(struct t_svm_manager), GFP_KERNEL);
	if (svm_manager == NULL) {
		NPU_DRV_ERR("no sys mem to alloc svm manager\n");
		return -ENOMEM;
	}
	memset_s(svm_manager, sizeof(struct t_svm_manager), 0x0, sizeof(struct t_svm_manager));
	cur_dev_ctx->svm_manager = (void *)svm_manager;
	mutex_init(&svm_manager->lock);
	return 0;
}

static int devdrv_get_item_bypid(struct t_svm_manager *svm_manager, pid_t svm_pid)
{
	// if find the item, then return its index, otherwise return the first empty item.
	int i;
	int j = -1;
	for (i = 0; i < DRVDEV_APP_MAX_NUM; i++) {
		if (svm_manager->svm_pid_info[i].svm_pid == svm_pid)
			break;
		if (svm_manager->svm_pid_info[i].manager_pid == 0 && j < 0)
			j = i;
	}
	return i < DRVDEV_APP_MAX_NUM ? i : j;
}
static int devdrv_check_pid_task_match(pid_t pid, struct task_struct *task)
{
	struct pid *pid_struct = NULL;
	struct task_struct *task_tmp = NULL;
	pid_struct = find_get_pid(pid);
	if (pid_struct != NULL)
		task_tmp = pid_task(pid_struct, PIDTYPE_PID);
	if (task_tmp != NULL && task_tmp == task && task->mm != NULL)
		return 0;
	NPU_DRV_ERR("the pid is invalid, pid=%d", pid);
	NPU_DRV_DEBUG("task=%pK, task_tmp=%pK\n", task, task_tmp);
	return -1;
}
// for current and app process
// 0. item should exist 1.[bind svm] 2.get ssid
int devdrv_get_ssid_bypid(uint32_t devid, pid_t manager_pid, pid_t svm_pid, uint16_t *ssid, uint64_t *ttbr, uint64_t *tcr)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;
	int i;
	int ret = 0;

	if (manager_pid <= 0 || svm_pid <= 0) {
		NPU_DRV_ERR("illegal pid\n");
		return -1;
	}

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}
	svm_manager = (struct t_svm_manager *)cur_dev_ctx->svm_manager;

	mutex_lock(&svm_manager->lock);
	i = devdrv_get_item_bypid(svm_manager, svm_pid);
	if (unlikely(i < 0)) {
		// table is full
		NPU_DRV_ERR("svm manager table is full\n");
		ret = -1;
		goto Complete;
	}
	if (unlikely(svm_manager->svm_pid_info[i].manager_pid == 0)) {
		NPU_DRV_ERR("the item isnot existed, manager_pid=%d, svm_pid=%d\n", manager_pid, svm_pid);
		ret = -1;
		goto Complete;
	}
	if (unlikely(svm_manager->svm_pid_info[i].manager_pid != manager_pid)) {
		NPU_DRV_ERR("the item has inserted by other process=%d, manager_pid=%d, svm_pid=%d\n",
			svm_manager->svm_pid_info[i].manager_pid, manager_pid, svm_pid);
		ret = -1;
		goto Complete;
	}
	if (svm_manager->svm_pid_info[i].hisi_svm == NULL) {
		if (devdrv_check_pid_task_match(svm_pid, svm_manager->svm_pid_info[i].task) != 0) {
			NPU_DRV_ERR("pid is invalid, svm_pid=%d\n", svm_pid);
			ret = -1;
			goto Complete;
		}
		devdrv_plat_svm_bind(
			cur_dev_ctx, svm_manager->svm_pid_info[i].task, (void **)(&(svm_manager->svm_pid_info[i].hisi_svm)));
	}
	ret = hisi_svm_get_ssid(svm_manager->svm_pid_info[i].hisi_svm, ssid, ttbr, tcr);
	if (ret != 0) {
		NPU_DRV_ERR("fail to get ssid, ret = %d\n", ret);
		goto Complete;
	}
Complete:
	mutex_unlock(&svm_manager->lock);
	return ret;
}
// app process:called by CreateProcess(Runtime)
// HiAiServer:called by open manager device
// 1. item  should not exist
// 2. insert a item, and set flag
int devdrv_insert_item_bypid(uint32_t devid, pid_t manager_pid, pid_t svm_pid)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;
	int i;
	int ret = 0;
	struct pid *pid_struct = NULL;
	struct task_struct *task = NULL;

	if (manager_pid <= 0 || svm_pid <= 0) {
		NPU_DRV_ERR("illegal pid\n");
		return -1;
	}

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}
	svm_manager = (struct t_svm_manager *)cur_dev_ctx->svm_manager;

	mutex_lock(&svm_manager->lock);
	i = devdrv_get_item_bypid(svm_manager, svm_pid);
	if (i < 0) {
		// table is full
		NPU_DRV_ERR("svm manager table is full\n");
		ret = -1;
		goto Complete;
	}
	if (svm_manager->svm_pid_info[i].manager_pid == 0) {
		// create a new item
		pid_struct = find_get_pid(svm_pid);
		if (pid_struct != NULL)
			task = pid_task(pid_struct, PIDTYPE_PID);
		if (task == NULL) {
			NPU_DRV_ERR("hisi_svm_flag_set failed\n");
			ret = -1;
			goto Complete;
		}
		ret = hisi_svm_flag_set(task, 1);
		if (ret != 0) {
			NPU_DRV_ERR("hisi_svm_flag_set failed\n");
			goto Complete;
		}
		svm_manager->svm_pid_info[i].manager_pid = manager_pid;
		svm_manager->svm_pid_info[i].svm_pid = svm_pid;
		svm_manager->svm_pid_info[i].task = task;
		svm_manager->item_num++;
		NPU_DRV_WARN("insert a new item, manager_pid=%d, svm_pid=%d, item_num=%u", manager_pid, svm_pid, svm_manager->item_num);
	} else {
		NPU_DRV_ERR("the pid is existed, exe_p_pid=%d, manager_pid=%d, svm_pid=%d\n",
			svm_manager->svm_pid_info[i].manager_pid, manager_pid, svm_pid);
		ret = -1;
		goto Complete;
	}
Complete:
	mutex_unlock(&svm_manager->lock);
	return ret;
}
// for app process
// 1. item should exist
// 2. unbind + unsetflag + delete item
int devdrv_release_item_bypid(uint32_t devid, pid_t manager_pid, pid_t svm_pid)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;
	int ret = 0;
	int i;

	if (manager_pid <= 0 || svm_pid <= 0) {
		NPU_DRV_ERR("illegal manager_pid=%d, svm_pid=%d\n", manager_pid, svm_pid);
		return -1;
	}

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}
	svm_manager = (struct t_svm_manager *)cur_dev_ctx->svm_manager;

	mutex_lock(&svm_manager->lock);
	i = devdrv_get_item_bypid(svm_manager, svm_pid);
	if (i < 0 || svm_manager->svm_pid_info[i].manager_pid != manager_pid || svm_manager->svm_pid_info[i].task == NULL) {
		NPU_DRV_ERR("the manager_pid=%d, svm_pid=%d is error!\n", manager_pid, svm_pid);
		goto Complete;
	}
	// unbind the svm
	if (svm_manager->svm_pid_info[i].hisi_svm) {
		hisi_svm_unbind_task(svm_manager->svm_pid_info[i].hisi_svm);
		svm_manager->svm_pid_info[i].hisi_svm = NULL;
	}
	// unsetflag
	hisi_svm_flag_set(svm_manager->svm_pid_info[i].task, 0);
	svm_manager->svm_pid_info[i].task = NULL;
	// delete the item
	svm_manager->svm_pid_info[i].manager_pid = 0;
	svm_manager->svm_pid_info[i].svm_pid = 0;
	svm_manager->item_num--;
	NPU_DRV_WARN("delete a new item, manager_pid=%d, item_num=%u", manager_pid, svm_manager->item_num);
Complete:
	mutex_unlock(&svm_manager->lock);
	return ret;
}
// for current and app process
// if called by powerdown(flag=0):unbind
// if called by close device(flag=1):unbind + (unsetflag + delete item)(exept current->tgid)
int devdrv_clear_pid_ssid_table(uint32_t devid, int flag)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;
	int i;

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}
	svm_manager = (struct t_svm_manager *)cur_dev_ctx->svm_manager;

	mutex_lock(&svm_manager->lock);
	for (i = 0; i < DRVDEV_APP_MAX_NUM; i++) {
		if (svm_manager->svm_pid_info[i].manager_pid == current->tgid) {
			// 1. unbind
			if (svm_manager->svm_pid_info[i].hisi_svm != NULL) {
				hisi_svm_unbind_task(svm_manager->svm_pid_info[i].hisi_svm);
				NPU_DRV_INFO("delete a new item, svm_pid=%d", svm_manager->svm_pid_info[i].svm_pid);
				svm_manager->svm_pid_info[i].hisi_svm = NULL;
			}
			// 2. unset flag + delete item
			if (flag && svm_manager->svm_pid_info[i].svm_pid != current->tgid) {
				if (svm_manager->svm_pid_info[i].task != NULL) {
					hisi_svm_flag_set(svm_manager->svm_pid_info[i].task, 0);
					svm_manager->svm_pid_info[i].task = NULL;
				}
				svm_manager->svm_pid_info[i].manager_pid = 0;
				svm_manager->svm_pid_info[i].svm_pid = 0;
				svm_manager->item_num--;
			}
		}
	}
	mutex_unlock(&svm_manager->lock);
	NPU_DRV_WARN("clear pid ssid table when %s", flag == 0 ? "powerdown" : "close npu device");
	return 0;
}

int devdrv_svm_manager_destroy(uint32_t devid)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct t_svm_manager *svm_manager = NULL;

	if (devid >= NPU_DEV_NUM) {
		NPU_DRV_ERR("illegal npu dev id\n");
		return -1;
	}

	cur_dev_ctx = get_dev_ctx_by_id(devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", devid);
		return -1;
	}

	devdrv_clear_pid_ssid_table(devid, 1);

	svm_manager = (struct t_svm_manager *)cur_dev_ctx->svm_manager;
	mutex_destroy(&svm_manager->lock);
	kfree(svm_manager);
	cur_dev_ctx->svm_manager = NULL;
	return 0;
}
