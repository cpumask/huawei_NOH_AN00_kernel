/*
 * npu_message.c
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
#include <securec.h>
#include <linux/timer.h>

#include "npu_common.h"
#include "npu_proc_ctx.h"
#include "npu_calc_channel.h"
#include "npu_calc_cq.h"
#include "npu_stream.h"
#include "npu_shm.h"
#include "npu_log.h"
#include "npu_mailbox.h"
#include "npu_event.h"
#include "npu_model.h"
#include "npu_task.h"
#include "npu_pm_framework.h"
#include "npu_heart_beat.h"
#include "npu_adapter.h"
#include "npu_ts_sqe.h"
#include "npu_calc_sq.h"
#include "npu_hwts_sqe.h"
#include "npu_pool.h"
#include "npu_iova.h"
#include "npu_doorbell.h"
#include "npu_platform.h"
#include "npu_svm.h"

enum npu_refcnt_updata {
	OPS_ADD = 0,
	OPS_SUB = 1,
	OPS_BUTT
};

const u32 dummy = 0xFFFFFFFF;

static struct devdrv_report *__devdrv_get_report(phys_addr_t base_addr,
	u32 slot_size, u32 slot_id)
{
	return (struct devdrv_report *)(uintptr_t)(base_addr + slot_size * slot_id);
}

static inline u32 devdrv_report_get_phase(struct devdrv_report *report)
{
	COND_RETURN_ERROR(report == NULL, dummy, "devdrv_report_get_phase failed\n");
	return (report->phase);
}

static inline u32 devdrv_report_get_sq_index(struct devdrv_report *report)
{
	COND_RETURN_ERROR(report == NULL, dummy, "devdrv_report_get_sq_index failed\n");
	return (report->sq_id);
}

static inline u32 devdrv_report_get_sq_head(struct devdrv_report *report)
{
	COND_RETURN_ERROR(report == NULL, dummy, "devdrv_report_get_sq_head failed\n");
	return (report->sq_head);
}

int devdrv_proc_get_calc_sq_info(struct devdrv_proc_ctx *proc_ctx, u32 sq_index, devdrv_ts_sq_info_t **sq_info,
	struct devdrv_sq_sub_info **sq_sub_info)
{
	struct devdrv_dev_ctx *cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %u is null\n", proc_ctx->devid);
		return -1;
	}

	/* outer function should make sure paras are not null */
	*sq_info = devdrv_calc_sq_info(proc_ctx->devid, sq_index);
	if (*sq_info == NULL) {
		NPU_DRV_ERR("sq_index= %u, sq_info is null\n", sq_index);
		return -1;
	}

	*sq_sub_info = npu_get_sq_sub_addr(cur_dev_ctx, (*sq_info)->index);
	if (*sq_sub_info == NULL) {
		NPU_DRV_ERR("sq_index= %u sq_sub is null\n", sq_index);
		return -1;
	}

	return 0;
}

/* according to DevdrvUpdateCqTailAndSqHead() */
void devdrv_update_cq_tail_sq_head(
	struct devdrv_proc_ctx *proc_ctx, struct devdrv_cq_sub_info *cq_sub_info, struct devdrv_ts_cq_info *cq_info)
{
	devdrv_ts_sq_info_t *sq_info = NULL;
	struct devdrv_report *report = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	u8 dev_id = 0;
	u32 next_tail;
	u32 sq_head;

	NPU_DRV_DEBUG("devdrv_update_cq_tail_sq_head enter\n");

	cur_dev_ctx = get_dev_ctx_by_id(dev_id);
	COND_RETURN_VOID(cur_dev_ctx == NULL, "cur dev ctx is null\n");
	report = __devdrv_get_report(cq_sub_info->virt_addr, cq_info->slot_size, cq_info->tail);

	NPU_DRV_DEBUG("cq_tail=%u, cq_base_addr=%p, valid_phase=%u \n", cq_info->tail, cq_sub_info->virt_addr,
		cq_info->phase);
	/* update cq_info->tail */
	while (devdrv_report_get_phase(report) == cq_info->phase) {
		NPU_DRV_DEBUG("cq_tail=%u\n", cq_info->tail);
		next_tail = (cq_info->tail + 1) % DEVDRV_MAX_CQ_DEPTH;
		/* cq_info->tail must not cover register->cq_head */
		/* use one slot to keep tail not cover the head, which may have report */
		if (next_tail == cq_info->head)
			break;

		sq_info = devdrv_calc_sq_info(proc_ctx->devid,
			devdrv_report_get_sq_index(report));
		COND_RETURN_VOID(sq_info == NULL, "devdrv_calc_sq_info error, sq_index = %d\n", devdrv_report_get_sq_index(report));
		sq_head = devdrv_report_get_sq_head(report); /* handle to which one (sq slot) */
		if (sq_head >= DEVDRV_MAX_SQ_DEPTH) {
			NPU_DRV_ERR("wrong sq head from cq, sqHead = %u\n", sq_head);
			break;
		}
		spin_lock(&cur_dev_ctx->spinlock);
		sq_info->head = sq_head;
		NPU_DRV_DEBUG("update sqinfo[%d]: head:%d, tail:%d, credit:%d\n", sq_info->index, sq_info->head,
			sq_info->tail, sq_info->credit);

		next_tail = cq_info->tail + 1;
		if (next_tail > (DEVDRV_MAX_CQ_DEPTH - 1)) {
			cq_info->phase = (cq_info->phase == 0) ? 1 : 0;
			cq_info->tail = 0;
		} else {
			cq_info->tail++;
		}
		report = __devdrv_get_report(cq_sub_info->virt_addr, cq_info->slot_size, cq_info->tail);
		spin_unlock(&cur_dev_ctx->spinlock);
	}
	NPU_DRV_DEBUG("cq_tail=%u\n", cq_info->tail);
}

static int devdrv_proc_get_report_wait_timeout(
	struct devdrv_proc_ctx *proc_ctx, struct devdrv_ts_cq_info *cq_info, int timeout_in)
{
	int result = 0;
	// get report from cq_tail and compare it with cq_info->phase
	if (devdrv_proc_is_valid_report_received(proc_ctx, cq_info) == true) {
		NPU_DRV_DEBUG("get valid report now without sleep\n");
		result = 1;
	} else { // will block cuurent thread
		NPU_DRV_DEBUG("get invalid report and will sleep\n");
		result = devdrv_proc_report_wait(proc_ctx, cq_info, timeout_in);
	}

	return result;
}

int npu_report_wait(struct devdrv_proc_ctx *proc_ctx, int *timeout_out, int timeout_in)
{
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_ts_cq_info *cq_info = NULL;

	if (list_empty_careful(&proc_ctx->cq_list)) {
		NPU_DRV_ERR("cq_info is null");
		return -ENODEV;
	}

	cq_sub_info = list_first_entry(&proc_ctx->cq_list, struct devdrv_cq_sub_info, list);
	cq_info = devdrv_calc_cq_info(proc_ctx->devid, cq_sub_info->index);
	if (cq_info == NULL) {
		NPU_DRV_DEBUG("proc ctx first cq_info is null");
		return -ENODEV;
	}

	*timeout_out = devdrv_proc_get_report_wait_timeout(proc_ctx, cq_info, timeout_in);

	return 0;
}

int devdrv_proc_send_request_occupy(devdrv_ts_sq_info_t *sq_info)
{
	u32 cmd_count = 1; /* need to be sent */
	u32 available_count;
	u32 sq_head = sq_info->head;
	u32 sq_tail = sq_info->tail;
	/* if credit is less than cmdCount, update credit */
	if (sq_info->credit < cmd_count)
		sq_info->credit =
			(sq_tail >= sq_head) ? (DEVDRV_MAX_SQ_DEPTH - (sq_tail - sq_head + 1)) : (sq_head - sq_tail - 1);

	if (sq_info->credit == 0) {
		NPU_DRV_WARN(
			"no available sq slot, sq id= %u, sq head= %u, sq tail= %u\n", sq_info->index, sq_head, sq_tail);
		return -1;
	}

	if ((sq_tail >= sq_head) && (DEVDRV_MAX_SQ_DEPTH - sq_tail <= cmd_count)) {
		if (sq_head == 0)
			available_count = DEVDRV_MAX_SQ_DEPTH - sq_tail - 1;
		else
			available_count = DEVDRV_MAX_SQ_DEPTH - sq_tail;

		if (available_count < cmd_count) {
			NPU_DRV_ERR("occupy fail available_count %d cmd_count %d\n", available_count, cmd_count);
			return -1;
		} else {
			return 0;
		}
	}

	if (sq_info->credit < cmd_count) {
		NPU_DRV_ERR("sq_info credit is not enough\n");
		return -1;
	}
	return 0;
}

int devdrv_proc_send_request_send(
	struct devdrv_dev_ctx *cur_dev_ctx, devdrv_ts_sq_info_t *sq_info, u32 sq_index, u32 cq_index, u32 cmd_count)
{
	struct devdrv_ts_cq_info *cq_info = NULL;
	u32 report_count = 1;
	u32 new_sq_tail;

	NPU_DRV_DEBUG("devdrv_proc_send_request_send enter\n");

	cq_info = devdrv_calc_cq_info(cur_dev_ctx->devid, cq_index);
	COND_RETURN_ERROR(cq_info == NULL, -1, "get cq info failed\n");

	/* if credit(available cmd slots) is less than cmdCount, it must be an error */
	COND_RETURN_ERROR(sq_info->credit < cmd_count, -EINVAL, "sqid= %u, credit= %u, cmd_count= %d is too much\n",
		sq_info->index, sq_info->credit, cmd_count);

	spin_lock(&cur_dev_ctx->spinlock);
	new_sq_tail = (sq_info->tail + cmd_count) % DEVDRV_MAX_SQ_DEPTH;
	if (new_sq_tail == sq_info->head)
		NPU_DRV_ERR("too much cmd, count= %d, credit= %u, sq_id= %d, head= %d, tail= %d\n", cmd_count,
			sq_info->credit, sq_info->index, sq_info->head, sq_info->tail);

	sq_info->tail = new_sq_tail;
	sq_info->credit -= cmd_count;
	sq_info->send_count += cmd_count;
	NPU_DRV_DEBUG("update sqinfo, id= %d, head= %d, tail= %d, credit= %d\n", sq_info->index, sq_info->head,
		sq_info->tail, sq_info->credit);
	/* use atomic add */
	__sync_add_and_fetch(&cq_info->count_report, report_count);
	spin_unlock(&cur_dev_ctx->spinlock);

	/* update doorbell */
	mutex_lock(&cur_dev_ctx->npu_power_up_off_mutex);
	if (cur_dev_ctx->power_stage == DEVDRV_PM_UP)
		(void)devdrv_write_doorbell_val(DOORBELL_RES_CAL_SQ, sq_index, sq_info->tail);
	else
		NPU_DRV_ERR("npu is powered off, power_stage[%u]\n",
			cur_dev_ctx->power_stage);
	mutex_unlock(&cur_dev_ctx->npu_power_up_off_mutex);
	return 0;
}

int devdrv_proc_receive_response_wait(struct devdrv_proc_ctx *proc_ctx, struct devdrv_cq_sub_info *cq_sub_info,
	struct devdrv_ts_cq_info *cq_info, struct devdrv_receive_response_info *report_info)
{
	int result;

	COND_RETURN_ERROR(cq_info == NULL, -1, "cq_info is null\n");
	devdrv_update_cq_tail_sq_head(proc_ctx, cq_sub_info, cq_info);
	if (cq_info->head != cq_info->tail) {
		/* must set 1 for valid report receive */
		report_info->wait_result = 1;
		return 0;
	}

	result = devdrv_proc_get_report_wait_timeout(proc_ctx, cq_info, report_info->timeout);
	report_info->wait_result = result;

	NPU_DRV_DEBUG("devid: %d, irq wait get report, cq id: %d, head: %d, tail: %d, phase: %d, result:%d\n",
		proc_ctx->devid, cq_info->index, cq_info->head, cq_info->tail, cq_info->phase, result);

	COND_RETURN_ERROR(result <= 0, 0, "DRV_WAIT_TIMEOUT\n");

	devdrv_update_cq_tail_sq_head(proc_ctx, cq_sub_info, cq_info);

	return 0;
}

int devdrv_get_report(struct devdrv_proc_ctx *proc_ctx, struct devdrv_ts_cq_info *cq_info,
	struct devdrv_cq_sub_info *cq_sub_info, struct devdrv_receive_response_info *report_info)
{
	int ret;
	u32 count;
	struct devdrv_report *report = NULL;

	if ((proc_ctx == NULL) || (cq_info == NULL) || (cq_sub_info == NULL)) {
		NPU_DRV_ERR("invalid pointer\n");
		return -1;
	}
	devdrv_update_cq_tail_sq_head(proc_ctx, cq_sub_info, cq_info);
	if (cq_info->head == cq_info->tail) {
		NPU_DRV_ERR("head is equal to tail, no report, head=%d, tail=%d, phase=%d\n"
			, cq_info->head, cq_info->tail, cq_info->phase);
		return -1;
	}
	report = __devdrv_get_report(cq_sub_info->virt_addr, cq_info->slot_size, cq_info->head);

	if (cq_info->tail > cq_info->head) {
		count = cq_info->tail - cq_info->head;
		if (count > cq_info->count_report)
			NPU_DRV_ERR("devid: %d, too much report, cq id: %d, cq head: %d, cq tail: %d, "
					"count: %d, count_report: %d\n",
				proc_ctx->devid, cq_info->index, cq_info->head, cq_info->tail, count,
				cq_info->count_report);
		/* use atomic sub */
		__sync_sub_and_fetch(&cq_info->count_report, count);
	} else {
		count = DEVDRV_MAX_CQ_DEPTH - cq_info->head;
		if (count > cq_info->count_report)
			NPU_DRV_ERR("devid: %d, too much report, cq id: %d, cq head: %d, cq tail: %d, "
					"count: %d, count_report: %d\n",
				proc_ctx->devid, cq_info->index, cq_info->head, cq_info->tail, count,
				cq_info->count_report);
		/* use atomic sub */
		__sync_sub_and_fetch(&cq_info->count_report, count);
	}

	ret = copy_to_user_safe(
		(void *)(uintptr_t)report_info->response_addr, report, count * sizeof(struct devdrv_report));
	if (ret != 0) {
		NPU_DRV_ERR("copy report to user fail ret %d \n", ret);
		return ret;
	}

	report_info->response_count = count;

	return 0;
}

int devdrv_release_report(struct devdrv_proc_ctx *proc_ctx, struct devdrv_ts_cq_info *cq_info, u32 count)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	u32 head;

	COND_RETURN_ERROR(cq_info == NULL, -1, "cq_info is null\n");

	cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("cur_dev_ctx %d is null\n", proc_ctx->devid);
		return -1;
	}

	spin_lock(&cur_dev_ctx->spinlock);
	cq_info->receive_count += count;
	head = (cq_info->head + count) % DEVDRV_MAX_CQ_DEPTH;
	cq_info->head = head;
	spin_unlock(&cur_dev_ctx->spinlock);

	mutex_lock(&cur_dev_ctx->npu_power_up_off_mutex);
	if (cur_dev_ctx->power_stage == DEVDRV_PM_UP)
		(void)devdrv_write_doorbell_val(DOORBELL_RES_CAL_CQ, cq_info->index, head);
	else
		NPU_DRV_ERR("npu is powered off, power_stage[%u]\n",
			cur_dev_ctx->power_stage);

	NPU_DRV_DEBUG("receive report, cq_id= %u, cq_tail= %u, cq_head= %u, receive_count= %lld\n",
		cq_info->index, cq_info->tail, cq_info->head, cq_info->receive_count);
	mutex_unlock(&cur_dev_ctx->npu_power_up_off_mutex);
	return 0;
}

void npu_task_cnt_update(struct devdrv_dev_ctx *dev_ctx, u32 cnt, u32 flag)
{
	int ref_cnt;

	if (flag == OPS_ADD) {
		atomic_add(cnt, &dev_ctx->pm.task_ref_cnt);
		NPU_DRV_DEBUG("add , task ref cnt = %d, cnt = %d\n", atomic_read(&dev_ctx->pm.task_ref_cnt), cnt);
	} else {
		ref_cnt = atomic_sub_return(cnt, &dev_ctx->pm.task_ref_cnt);
		NPU_DRV_DEBUG("sub , task ref cnt = %d, cnt = %d\n", atomic_read(&dev_ctx->pm.task_ref_cnt), cnt);
		if (ref_cnt == 0) {
			npu_pm_add_idle_timer(dev_ctx);
			NPU_DRV_DEBUG("idle timer add, ref_cnt = %d\n", cnt);
		}
	}
}

static devdrv_complete_ts_sqe(u8 devid, devdrv_ts_comm_sqe_t *comm_task)
{
	uint64_t ttbr;
	uint16_t ssid;
	uint64_t tcr;
	int ret;
	COND_RETURN_ERROR(comm_task == NULL, -1, "comm_task is null\n");
	if (comm_task->type == 13 && comm_task->u.model_execute_sqe.asid == 0 &&
		comm_task->u.model_execute_sqe.asid_baddr == 0) {
		// v200:new procedure
		ret = devdrv_get_ssid_bypid(devid, current->tgid, comm_task->u.model_execute_sqe.execute_pid, &ssid, &ttbr, &tcr);
		COND_RETURN_ERROR(ret != 0, -1, "devdrv_get_ssid_bypid fail, ret= %d\n", ret);
		NPU_DRV_INFO("ttbr=0x%llx, tcr=%llx", ttbr, tcr);
		comm_task->u.model_execute_sqe.asid = ttbr >> 48;
		comm_task->u.model_execute_sqe.asid_baddr = ttbr & (0x0000FFFFFFFFFFFFu);
		comm_task->u.model_execute_sqe.smmu_svm_ssid = ssid;
		comm_task->u.model_execute_sqe.tcr = tcr;
	}
	return 0;
}

int npu_send_request(struct devdrv_proc_ctx *proc_ctx, devdrv_ts_comm_sqe_t *comm_task)
{
	devdrv_stream_info_t *stream_info = NULL;
	devdrv_ts_sq_info_t *sq_info = NULL;
	struct devdrv_sq_sub_info *sq_sub_info = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	u32 cmd_count = 1; /* send one by one time */
	int ret = 0;

	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "cur_dev_ctx %d is null\n", proc_ctx->devid);
	npu_task_cnt_update(cur_dev_ctx, cmd_count, OPS_ADD);
	ret = npu_pm_enter_workmode(proc_ctx, cur_dev_ctx, NPU_NONSEC);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret,
				"first task : power up fail, ret= %d\n", ret);

	/* verify ts_sqe */
	ret = devdrv_verify_ts_sqe(comm_task);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret,
				"devdrv_verify_ts_sqe fail, ret= %d\n", ret);

	/* complete ts_sqe*/
	ret = devdrv_complete_ts_sqe(proc_ctx->devid, comm_task);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret, "devdrv_complete_ts_sqe fail, ret= %d\n", ret);

	/* save ts_sqe */
	stream_info = devdrv_calc_stream_info(proc_ctx->devid, comm_task->stream_id);
	COND_GOTO_ERROR(stream_info == NULL, SEND_FAIL, ret, -1,
		"devdrv_calc_stream_info fail, ret= %d\n", -1);
	ret = devdrv_proc_get_calc_sq_info(proc_ctx, stream_info->sq_index, &sq_info, &sq_sub_info);
	COND_GOTO_ERROR(((ret != 0 || sq_info == NULL || sq_sub_info == NULL)),	SEND_FAIL, ret, ret,
		"devdrv_proc_send_request_occypy failed, sq_info= %pK, sq_sub_info= %pK\n", sq_info, sq_sub_info);

	NPU_DRV_DEBUG("sq_sub_info, index= %u, ref_by_streams= %u, phy_addr= 0x%llx, virt_addr= 0x%llx\n",
		sq_sub_info->index, sq_sub_info->ref_by_streams, sq_sub_info->phy_addr, sq_sub_info->virt_addr);

	ret = devdrv_proc_send_request_occupy(sq_info);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret,
		"devdrv_proc_send_request_occypy failed, sq_id= %u, sq_head= %u sq_tail= %u\n",
		sq_info->index, sq_info->head, sq_info->tail);

	ret = devdrv_format_ts_sqe((void *)(uintptr_t)sq_sub_info->virt_addr, comm_task, sq_info->tail);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret,
		"format sqe failed, ret= %d, virt_addr= 0x%llx, phy_addr= 0x%llx, tail= %u\n",
		ret, sq_sub_info->virt_addr, sq_sub_info->phy_addr, sq_info->tail);

	ret = devdrv_proc_send_request_send(cur_dev_ctx, sq_info, stream_info->sq_index, stream_info->cq_index, cmd_count);
	COND_GOTO_ERROR(ret != 0, SEND_FAIL, ret, ret,
		"devdrv_proc_send_request_occypy failed, sq_id= %u, sq_head= %u sq_tail= %u\n",
		sq_info->index, sq_info->head, sq_info->tail);

	NPU_DRV_DEBUG("communication stream= %d, sq_indx= %u, sq_tail= %u, send_count= %lld\n", stream_info->id,
		stream_info->sq_index, sq_info->tail, sq_info->send_count);

	return 0;

SEND_FAIL:
	npu_task_cnt_update(cur_dev_ctx, cmd_count, OPS_SUB);
	return ret;
}

int npu_receive_response(struct devdrv_proc_ctx *proc_ctx, struct devdrv_receive_response_info *report_info)
{
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct devdrv_cq_sub_info *cq_sub_info = NULL;
	struct devdrv_ts_cq_info *cq_info = NULL;
	int ret = 0;
	u32 cq_id = 0xFF;

	COND_RETURN_ERROR(proc_ctx == NULL, -1, "invalid proc ctx\n");

	cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "invalid dev ctx\n");
	ret = npu_proc_get_cq_id(proc_ctx, &cq_id);
	COND_RETURN_ERROR((ret != 0), -1, "get cq fail\n");

	cq_info = devdrv_calc_cq_info(proc_ctx->devid, cq_id);
	COND_RETURN_ERROR(cq_info == NULL, -1, "get cq info failed\n");

	cq_sub_info = npu_get_cq_sub_addr(cur_dev_ctx, cq_info->index);
	COND_RETURN_ERROR(cq_sub_info == NULL, -1, "invalid para cq info index %u\n",
		cq_info->index);

	report_info->timeout = cur_dev_ctx->pm.npu_task_time_out;
	ret = devdrv_proc_receive_response_wait(proc_ctx, cq_sub_info, cq_info, report_info);
	if (report_info->wait_result <= 0) {
		NPU_DRV_ERR("drv receive response wait timeout, need reboot!!!\n");
		(void)npu_pm_reboot(proc_ctx, cur_dev_ctx);
		return ret;
	}
	COND_RETURN_ERROR((ret != 0), ret, "receive response wait ret %d result %d \n", ret, report_info->wait_result);

	ret = devdrv_get_report(proc_ctx, cq_info, cq_sub_info, report_info);
	COND_RETURN_ERROR(ret != 0, ret, "fail to get report ret= %d\n", ret);

	ret = devdrv_release_report(proc_ctx, cq_info, report_info->response_count);
	COND_RETURN_ERROR(ret != 0, ret, "fail to release report ret= %d\n", ret);

	npu_task_cnt_update(cur_dev_ctx, report_info->response_count, OPS_SUB);

	return 0;
}

int npu_report_sync_wait(struct devdrv_proc_ctx *proc_ctx, struct devdrv_report_sync_wait_info *wait_info)
{
	int ret = 0;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	cur_dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	COND_RETURN_ERROR(cur_dev_ctx == NULL, -1, "invalid dev ctx\n");

	wait_info->timeout = cur_dev_ctx->pm.npu_task_time_out;
	ret = devdrv_proc_report_sync_wait(proc_ctx, wait_info);

	if (ret > 0) {
		npu_task_cnt_update(cur_dev_ctx, 1, OPS_SUB);
	} else {
		NPU_DRV_ERR("drv receive sync response wait timeout, need reboot!!!\n");
		(void)npu_pm_reboot(proc_ctx, cur_dev_ctx);
	}

	return ret;
}
