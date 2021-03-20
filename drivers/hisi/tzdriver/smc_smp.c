/*
 * smc_smp.c
 *
 * function for sending smc cmd
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
 */
#include "smc_smp.h"
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/semaphore.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include <linux/cpu.h>
#include <linux/regulator/consumer.h>
#include <linux/spi/spi.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/clk-provider.h>
#include <linux/clk.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/version.h>
#include <linux/cpumask.h>
#include <linux/err.h>

#ifdef CONFIG_HISI_SECS_CTRL
#include <linux/hisi/secs_power_ctrl.h>
#endif

#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <securec.h>
#include <asm/compiler.h>
#include <asm/cacheflush.h>

#ifdef CONFIG_TEE_AUDIT
#include <chipset_common/security/hw_kernel_stp_interface.h>
#endif

#ifdef CONFIG_LOG_EXCEPTION
#include <huawei_platform/log/imonitor.h>
#define IMONITOR_TA_CRASH_EVENT_ID           901002003
#endif

#include "tc_ns_log.h"
#include "teek_client_constants.h"
#include "tc_ns_client.h"
#include "agent.h"
#include "teek_ns_client.h"
#include "mailbox_mempool.h"
#include "cmdmonitor.h"
#include "tui.h"
#include "security_auth_enhance.h"
#include "tlogger.h"
#include "ko_adapt.h"
#include "log_cfg_api.h"

#ifndef CONFIG_ARM64
#error "TEE SMP has ARM64 support only"
#endif

#define SECS_SUSPEND_STATUS      0xA5A5
#define PREEMPT_COUNT            10000
#define HZ_COUNT                 10
#define IDLED_COUNT              100

#define CPU_ZERO    0
#define CPU_ONE     1
#define CPU_FOUR    4
#define CPU_FIVE    5
#define CPU_SIX     6
#define CPU_SEVEN   7
#define LOW_BYTE    0xF


/* Current state of the system */
static uint8_t g_sys_crash;

struct wo_pm_params {
	int *n_idled;
	u64 *ret;
	u64 *exit_reason;
	u64 *ta;
	u64 *target;
};

struct shadow_work {
	struct kthread_work kthwork;
	struct work_struct work;
	uint64_t target;
};

unsigned long g_shadow_thread_id = 0;
static struct task_struct *g_siq_thread = NULL;
static struct task_struct *g_smc_svc_thread = NULL;
static struct task_struct *g_ipi_helper_thread = NULL;
static DEFINE_KTHREAD_WORKER(g_ipi_helper_worker);

#if CONFIG_CPU_AFF_NR
static struct cpumask g_cpu_mask;
static int g_mask_flag = 0;
#endif

#ifdef CONFIG_DRM_ADAPT
static struct cpumask g_drm_cpu_mask;
static int g_drm_mask_flag = 0;
#endif

struct tc_ns_smc_queue *g_cmd_data = NULL;
phys_addr_t g_cmd_phys;

static struct list_head g_pending_head;
static spinlock_t g_pend_lock;

struct tc_ns_smc_queue * get_cmd_data_buffer(void)
{
	return g_cmd_data;
}

static inline void acquire_smc_buf_lock(smc_buf_lock_t *lock)
{
	int ret;

	preempt_disable();
	do {
		ret = cmpxchg(lock, 0, 1);
	} while (ret);
}

static inline void release_smc_buf_lock(smc_buf_lock_t *lock)
{
	(void)cmpxchg(lock, 1, 0);
	preempt_enable();
}

static int occupy_free_smc_in_entry(const struct tc_ns_smc_cmd *cmd)
{
	int idx = -1;
	int i;

	if (cmd == NULL) {
		tloge("Bad parameters! cmd is NULL.\n");
		return -1;
	}
	/* Note:
	 * acquire_smc_buf_lock will disable preempt and kernel will forbid
	 * call mutex_lock in preempt disabled scenes.
	 * To avoid such case(update_timestamp and update_chksum will call
	 * mutex_lock), only cmd copy is done when preempt is disable,
	 * then do update_timestamp and update_chksum.
	 * As soon as this idx of in_bitmap is set, gtask will see this
	 * cmd_in, but the cmd_in is not ready that lack of update_xxx,
	 * so we make a tricky here, set doing_bitmap and in_bitmap both
	 * at first, after update_xxx is done, clear doing_bitmap.
	 */
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	for (i = 0; i < MAX_SMC_CMD; i++) {
		if (test_bit(i, g_cmd_data->in_bitmap))
			continue;
		if (memcpy_s(&g_cmd_data->in[i], sizeof(g_cmd_data->in[i]),
			cmd, sizeof(*cmd)) != EOK) {
			tloge("memcpy_s failed,%s line:%d", __func__, __LINE__);
			break;
		}
		g_cmd_data->in[i].event_nr = i;
		isb();
		wmb();
		set_bit(i, g_cmd_data->in_bitmap);
		set_bit(i, g_cmd_data->doing_bitmap);
		idx = i;
		break;
	}
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	if (idx == -1) {
		tloge("can't get any free smc entry\n");
		return -1;
	}

	if (update_timestamp(&g_cmd_data->in[idx])) {
		tloge("update_timestamp failed !\n");
		goto clean;
	}
	if (update_chksum(&g_cmd_data->in[idx])) {
		tloge("update_chksum failed.\n");
		goto clean;
	}

	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	isb();
	wmb();
	clear_bit(idx, g_cmd_data->doing_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	return idx;

clean:
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	clear_bit(i, g_cmd_data->in_bitmap);
	clear_bit(i, g_cmd_data->doing_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	return -1;
}

static int reuse_smc_in_entry(uint32_t idx)
{
	int rc = 0;

	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	if (!(test_bit(idx, g_cmd_data->in_bitmap) &&
		test_bit(idx, g_cmd_data->doing_bitmap))) {
		tloge("invalid cmd to reuse\n");
		rc = -1;
		goto out;
	}
	if (memcpy_s(&g_cmd_data->in[idx], sizeof(g_cmd_data->in[idx]),
		&g_cmd_data->out[idx], sizeof(g_cmd_data->out[idx])) != EOK) {
		tloge("memcpy_s failed,%s line:%d", __func__, __LINE__);
		rc = -1;
		goto out;
	}
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	if (update_timestamp(&g_cmd_data->in[idx])) {
		tloge("update_timestamp failed !\n");
		return -1;
	}
	if (update_chksum(&g_cmd_data->in[idx])) {
		tloge("update_chksum failed.\n");
		return -1;
	}

	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	isb();
	wmb();
	clear_bit(idx, g_cmd_data->doing_bitmap);
out:
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	return rc;
}

enum CMD_REUSE {
	CLEAR,      /* clear this cmd index */
	RESEND,     /* use this cmd index resend */
};

static int copy_smc_out_entry(uint32_t idx, struct tc_ns_smc_cmd *copy,
	enum CMD_REUSE *usage)
{
	bool param_check = false;

	param_check = (copy == NULL || usage == NULL);
	if (param_check == true) {
		tloge("Bad parameters!\n");
		return -1;
	}
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	if (!test_bit(idx, g_cmd_data->out_bitmap)) {
		tloge("cmd out %u is not ready\n", idx);
		show_cmd_bitmap();
		release_smc_buf_lock(&g_cmd_data->smc_lock);
		return -1;
	}
	if (memcpy_s(copy, sizeof(*copy), &g_cmd_data->out[idx],
		sizeof(g_cmd_data->out[idx]))) {
		tloge("copy smc out failed\n");
		release_smc_buf_lock(&g_cmd_data->smc_lock);
		return -1;
	}
	isb();
	wmb();
	if (g_cmd_data->out[idx].ret_val == TEEC_PENDING2 ||
		g_cmd_data->out[idx].ret_val == TEEC_PENDING) {
		*usage = RESEND;
	} else {
		clear_bit(idx, g_cmd_data->in_bitmap);
		clear_bit(idx, g_cmd_data->doing_bitmap);
		*usage = CLEAR;
	}
	clear_bit(idx, g_cmd_data->out_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	return 0;
}

static inline void clear_smc_in_entry(uint32_t idx)
{
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	clear_bit(idx, g_cmd_data->in_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
}

static inline void release_smc_entry(uint32_t idx)
{
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	clear_bit(idx, g_cmd_data->in_bitmap);
	clear_bit(idx, g_cmd_data->doing_bitmap);
	clear_bit(idx, g_cmd_data->out_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
}

static inline int is_cmd_working_done(uint32_t idx)
{
	bool ret = false;

	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	if (test_bit(idx, g_cmd_data->out_bitmap))
		ret = true;
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	return ret;
}

static void show_in_bitmap(int *cmd_in, uint32_t len)
{
	uint32_t idx;
	uint32_t in = 0;
	char bitmap[MAX_SMC_CMD + 1];
	bool check_value = (len != MAX_SMC_CMD ||
		g_cmd_data == NULL);

	if (check_value == true)
		return;
	for (idx = 0; idx < MAX_SMC_CMD; idx++) {
		if (test_bit(idx, g_cmd_data->in_bitmap)) {
			bitmap[idx] = '1';
			cmd_in[in++] = idx;
		} else {
			bitmap[idx] = '0';
		}
	}
	bitmap[MAX_SMC_CMD] = '\0';
	tloge("in_bitmap: %s\n", bitmap);
}

static void show_out_bitmap(int *cmd_out, uint32_t len)
{
	uint32_t idx;
	uint32_t out = 0;
	char bitmap[MAX_SMC_CMD + 1];
	bool check_value = (len != MAX_SMC_CMD ||
		g_cmd_data == NULL);

	if (check_value == true)
		return;
	for (idx = 0; idx < MAX_SMC_CMD; idx++) {
		if (test_bit(idx, g_cmd_data->out_bitmap)) {
			bitmap[idx] = '1';
			cmd_out[out++] = idx;
		} else {
			bitmap[idx] = '0';
		}
	}
	bitmap[MAX_SMC_CMD] = '\0';
	tloge("out_bitmap: %s\n", bitmap);
}

static void show_doing_bitmap(void)
{
	uint32_t idx;
	char bitmap[MAX_SMC_CMD + 1];

	if (g_cmd_data == NULL)
		return;
	for (idx = 0; idx < MAX_SMC_CMD; idx++) {
		if (test_bit(idx, g_cmd_data->doing_bitmap))
			bitmap[idx] = '1';
		else
			bitmap[idx] = '0';
	}
	bitmap[MAX_SMC_CMD] = '\0';
	tloge("doing_bitmap: %s\n", bitmap);
}

void show_cmd_bitmap_with_lock(void)
{
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	show_cmd_bitmap();
	release_smc_buf_lock(&g_cmd_data->smc_lock);
}

void show_cmd_bitmap(void)
{
	uint32_t idx;
	int cmd_in[MAX_SMC_CMD];
	int cmd_out[MAX_SMC_CMD];
	bool check_value = false;

	check_value = memset_s(cmd_in, sizeof(cmd_in), -1, sizeof(cmd_in)) ||
		memset_s(cmd_out, sizeof(cmd_out), -1, sizeof(cmd_out));
	if (check_value == true) {
		tloge("memset failed\n");
		return;
	}
	show_in_bitmap(cmd_in, MAX_SMC_CMD);
	show_doing_bitmap();
	show_out_bitmap(cmd_out, MAX_SMC_CMD);

	tloge("cmd_in value:\n");
	for (idx = 0; idx < MAX_SMC_CMD; idx++) {
		if (cmd_in[idx] == -1)
			break;
		tloge("cmd[%d]: cmd_id=%u, ca_pid=%u, dev_id = 0x%x, event_nr=%u, ret_val=0x%x\n",
			cmd_in[idx],
			g_cmd_data->in[cmd_in[idx]].cmd_id,
			g_cmd_data->in[cmd_in[idx]].ca_pid,
			g_cmd_data->in[cmd_in[idx]].dev_file_id,
			g_cmd_data->in[cmd_in[idx]].event_nr,
			g_cmd_data->in[cmd_in[idx]].ret_val);
	}

	tloge("cmd_out value:\n");
	for (idx = 0; idx < MAX_SMC_CMD; idx++) {
		if (cmd_out[idx] == -1)
			break;
		tloge("cmd[%d]: cmd_id=%u, ca_pid=%u, dev_id = 0x%x, event_nr=%u, ret_val=0x%x\n",
			cmd_out[idx],
			g_cmd_data->out[cmd_out[idx]].cmd_id,
			g_cmd_data->out[cmd_out[idx]].ca_pid,
			g_cmd_data->out[cmd_out[idx]].dev_file_id,
			g_cmd_data->out[cmd_out[idx]].event_nr,
			g_cmd_data->out[cmd_out[idx]].ret_val);
	}
}

static struct pending_entry *init_pending_entry(pid_t pid)
{
	struct pending_entry *pe = NULL;

	pe = kmalloc(sizeof(*pe), GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)pe)) {
		tloge("alloc pe failed\n");
		return NULL;
	}
	if (memset_s(pe, sizeof(*pe), 0, sizeof(*pe)) != EOK) {
		tloge("memset pe failed!.\n");
		kfree(pe);
		return NULL;
	}
	atomic_set(&pe->users, 1); // init pe->users to 1
	pe->pid = pid;
	init_waitqueue_head(&pe->wq);
	atomic_set(&pe->run, 0);
	INIT_LIST_HEAD(&pe->list);
	spin_lock(&g_pend_lock);
	list_add_tail(&pe->list, &g_pending_head);
	spin_unlock(&g_pend_lock);
	return pe;
}

struct pending_entry *find_pending_entry(pid_t pid)
{
	struct pending_entry *pe = NULL;

	spin_lock(&g_pend_lock);
	list_for_each_entry(pe, &g_pending_head, list) {
		if (pe->pid == pid) {
			atomic_inc(&pe->users);
			spin_unlock(&g_pend_lock);
			return pe;
		}
	}
	spin_unlock(&g_pend_lock);
	return NULL;
}

void foreach_pending_entry(void (*func)(struct pending_entry *))
{
	struct pending_entry *pe = NULL;

	if (func == NULL)
		return;
	spin_lock(&g_pend_lock);
	list_for_each_entry(pe, &g_pending_head, list) {
		func(pe);
	}
	spin_unlock(&g_pend_lock);
}

void put_pending_entry(struct pending_entry *pe)
{
	if (pe != NULL) {
		if (atomic_dec_and_test(&pe->users))
			kfree(pe);
	}
}

static void release_pending_entry(struct pending_entry *pe)
{
	spin_lock(&g_pend_lock);
	list_del(&pe->list);
	spin_unlock(&g_pend_lock);
	put_pending_entry(pe);
}

static DECLARE_WAIT_QUEUE_HEAD(siq_th_wait);
static DECLARE_WAIT_QUEUE_HEAD(ipi_th_wait);
static atomic_t g_siq_th_run;

enum {
	TYPE_CRASH_TA   = 1,
	TYPE_CRASH_TEE = 2,
};

enum SMC_OPS_EXIT {
	SMC_OPS_NORMAL   = 0x0,
	SMC_OPS_SCHEDTO  = 0x1,
	SMC_OPS_START_SHADOW    = 0x2,
	SMC_OPS_START_FIQSHD    = 0x3,
	SMC_OPS_PROBE_ALIVE     = 0x4,
	SMC_OPS_ABORT_TASK      = 0x5,
	SMC_EXIT_NORMAL         = 0x0,
	SMC_EXIT_PREEMPTED      = 0x1,
	SMC_EXIT_SHADOW         = 0x2,
	SMC_EXIT_ABORT          = 0x3,
	SMC_EXIT_MAX            = 0x4,
};

#define SHADOW_EXIT_RUN             0x1234dead
#define SMC_EXIT_TARGET_SHADOW_EXIT 0x1

static inline bool is_shadow_exit(uint64_t target)
{
	return target & SMC_EXIT_TARGET_SHADOW_EXIT;
}

struct smc_cmd_ret {
	u64 exit;
	u64 ta;
	u64 target;
};

static void secret_fill(struct smc_cmd_ret *ret, u64 exit, u64 ta, u64 target)
{
	if (ret != NULL) {
		ret->exit = exit;
		ret->ta = ta;
		ret->target = target;
	}
}

int sigkill_pending(struct task_struct *tsk)
{
	if (tsk == NULL) {
		tloge("tsk is null!\n");
		return 0;
	}
	return sigismember(&tsk->pending.signal, SIGKILL) ||
		sigismember(&tsk->pending.signal, SIGUSR1) ||
		sigismember(&tsk->signal->shared_pending.signal, SIGKILL);
}

enum CMD_STATE {
	START,
	KILLING,
	KILLED,
};

#if CONFIG_CPU_AFF_NR
static void set_cpu_strategy(struct cpumask *old_mask)
{
	unsigned int i;

	if (!g_mask_flag) {
		cpumask_clear(&g_cpu_mask);
		for (i = 0; i < CONFIG_CPU_AFF_NR; i++)
			cpumask_set_cpu(i, &g_cpu_mask);
		g_mask_flag = 1;
	}
	cpumask_copy(old_mask, &current->cpus_allowed);
	if (raw_smp_processor_id() >= CONFIG_CPU_AFF_NR) {
		set_cpus_allowed_ptr(current, &g_cpu_mask);
		schedule();
	}
}
#endif

#if CONFIG_CPU_AFF_NR
static void restore_cpu(struct cpumask *old_mask)
{
	/* current equal old means no set cpu affinity, no need to restore */
	if (cpumask_equal(&current->cpus_allowed, old_mask))
		return;

	set_cpus_allowed_ptr(current, old_mask);
	schedule();
}
#endif

static noinline int smp_smc_send(uint32_t cmd, u64 ops, u64 ca,
	struct smc_cmd_ret *secret, bool need_kill)
{
	u64 x0 = cmd;
	u64 x1 = ops;
	u64 x2 = ca;
	u64 x3 = 0;
	u64 x4 = 0;
	u64 ret = 0;
	u64 exit_reason = 0;
	u64 ta = 0;
	u64 target = 0;
	bool check_value = false;
#if CONFIG_CPU_AFF_NR
	struct cpumask old_mask;
#endif

retry:
#if CONFIG_CPU_AFF_NR
	set_cpu_strategy(&old_mask);
#endif
	if (secret != NULL && secret->exit == SMC_EXIT_PREEMPTED) {
		x0 = cmd;
		x1 = SMC_OPS_SCHEDTO;
		x2 = ca;
		x3 = secret->ta;
		x4 = secret->target;
	}
	check_value = ops == SMC_OPS_SCHEDTO || ops == SMC_OPS_START_FIQSHD;
	if (secret != NULL && check_value)
		x4 = secret->target;
	tlogd("[cpu %d] start to send x0=%llx x1=%llx x2=%llx x3=%llx x4=%llx\n",
		raw_smp_processor_id(), x0, x1, x2, x3, x4);
	isb();
	wmb();
	do {
		asm volatile(
			"mov x0, %[fid]\n"
			"mov x1, %[a1]\n"
			"mov x2, %[a2]\n"
			"mov x3, %[a3]\n"
			"mov x4, %[a4]\n"
			"smc #0\n"
			"str x0, [%[re0]]\n"
			"str x1, [%[re1]]\n"
			"str x2, [%[re2]]\n"
			"str x3, [%[re3]]\n"
			:[fid] "+r"(x0), [a1] "+r"(x1), [a2] "+r"(x2),
			[a3] "+r"(x3), [a4] "+r"(x4)
			:[re0] "r"(&ret), [re1] "r"(&exit_reason),
			[re2] "r"(&ta), [re3] "r"(&target)
			: "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
			"x8", "x9", "x10", "x11", "x12", "x13",
			"x14", "x15", "x16", "x17");
	} while (0);

	isb();
	wmb();
	tlogd("[cpu %d] return val %llx exit_reason %llx ta %llx targ %llx\n",
		raw_smp_processor_id(), ret, exit_reason, ta, target);
	if (secret == NULL)
		return ret;
	secret_fill(secret, exit_reason, ta, target);
	if (exit_reason == SMC_EXIT_PREEMPTED) {
		/* There's 2 ways to send a terminate cmd to kill a running TA,
		 * in current context or another. If send terminate in another
		 * context, may encounter concurrency problem, as terminate cmd
		 * is send but not process, the original cmd has finished.
		 * So we send the terminate cmd in current context.
		 */
		check_value = need_kill && sigkill_pending(current)
			&& is_thread_reported(current->pid);
		if (check_value == true) {
			secret->exit = SMC_EXIT_ABORT;
			tloge("receive kill signal\n");
		} else {
#ifndef CONFIG_PREEMPT
			/* yield cpu to avoid soft lockup */
			cond_resched();
#endif
			goto retry;
		}
	}
#if CONFIG_CPU_AFF_NR
	restore_cpu(&old_mask);
#endif
	return ret;
}

static uint64_t send_smc_cmd(uint32_t cmd, phys_addr_t cmd_addr,
	uint32_t cmd_type, uint8_t wait)
{
	register u64 x0 asm("x0") = cmd;
	register u64 x1 asm("x1") = cmd_addr;
	register u64 x2 asm("x2") = cmd_type;
	register u64 x3 asm("x3") = cmd_addr >> ADDR_TRANS_NUM;
	do {
		asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x0")
			__asmeq("%2", "x1")
			__asmeq("%3", "x2")
			__asmeq("%4", "x3")
			"smc #0\n"
			: "+r"(x0)
			: "r"(x0), "r"(x1), "r"(x2), "r"(x3));
	} while (x0 == TSP_REQUEST && wait);
	return x0;
}

int raw_smc_send(uint32_t cmd, phys_addr_t cmd_addr,
	uint32_t cmd_type, uint8_t wait)
{
	uint64_t x0;

#if (CONFIG_CPU_AFF_NR != 0)
	struct cpumask old_mask;
	set_cpu_strategy(&old_mask);
#endif
	x0 = send_smc_cmd(cmd, cmd_addr, cmd_type, wait);
#if (CONFIG_CPU_AFF_NR != 0)
	restore_cpu(&old_mask);
#endif
	return x0;
}

void siq_dump(phys_addr_t mode)
{
	raw_smc_send(TSP_REE_SIQ, mode, 0, false);
	tz_log_write();
	do_cmd_need_archivelog();
}

static int siq_thread_fn(void *arg)
{
	int ret;

	while (1) {
		ret = wait_event_interruptible(siq_th_wait,
			atomic_read(&g_siq_th_run));
		if (ret) {
			tloge("wait_event_interruptible failed!\n");
			return -EINTR;
		}
		atomic_set(&g_siq_th_run, 0);
		siq_dump((phys_addr_t)(1)); // set this addr to 1
	}
}

#ifdef CONFIG_TEE_AUDIT
#define MAX_UPLOAD_INFO_LEN	 4
static void upload_audit_event(unsigned int eventindex)
{
#ifdef CONFIG_HW_KERNEL_STP
	struct stp_item item;
	int ret;
	char att_info[MAX_UPLOAD_INFO_LEN + 1] = {0};

	att_info[0] = (unsigned char)(eventindex >> 24); // eventindex is separated into four part
	att_info[1] = (unsigned char)(eventindex >> 16); // each part is one char
	att_info[2] = (unsigned char)(eventindex >> 8);
	att_info[3] = (unsigned char)eventindex;
	att_info[MAX_UPLOAD_INFO_LEN] = '\0';
	item.id = item_info[ITRUSTEE].id; // 0x00000185;
	item.status = STP_RISK;
	item.credible = STP_REFERENCE; // STP_CREDIBLE;
	item.version = 0;
	ret = strcpy_s(item.name, STP_ITEM_NAME_LEN, STP_NAME_ITRUSTEE);
	if (ret != EOK) {
		tloge("strncpy_s failed  %x .\n", ret);
		return;
	}
	tlogd("stp get size %lx succ \n", sizeof(item_info[ITRUSTEE].name));
	ret = kernel_stp_upload(item, att_info);
	if (ret != 0) {
		tloge("stp %x event upload failed\n", eventindex);
	} else {
		tloge("stp %x event upload succ\n", eventindex);
	}
#else
	(void)eventindex;
#endif
}
#endif

static void cmd_result_check(struct tc_ns_smc_cmd *cmd)
{
	bool check_value = false;

	check_value = (cmd->ret_val == TEEC_SUCCESS) &&
		(verify_chksum(cmd) != EOK);
	if (check_value) {
		cmd->ret_val = TEEC_ERROR_GENERIC;
		tloge("verify_chksum failed.\n");
	}

	check_value = cmd->ret_val == TEEC_PENDING ||
		cmd->ret_val == TEEC_PENDING2;
	if (check_value)
		tlogd("wakeup command %u\n", cmd->event_nr);
	if (cmd->ret_val == TEE_ERROR_TAGET_DEAD) {
		tloge("error smc call: ret = %x and cmd.err_origin=%x\n",
			cmd->ret_val, cmd->err_origin);
		cmd_monitor_ta_crash(TYPE_CRASH_TA);
		ta_crash_report_log();
	} else if (cmd->ret_val == TEEC_ERROR_TUI_NOT_AVAILABLE) {
		do_ns_tui_release();
	} else if (cmd->ret_val == TEE_ERROR_AUDIT_FAIL) {
		tloge("error smc call: ret = %x and cmd.err_origin=%x\n",
			cmd->ret_val, cmd->err_origin);
#ifdef CONFIG_TEE_AUDIT
		tloge("error smc call: status = %x and cmd.err_origin=%x\n",
			cmd->eventindex, cmd->err_origin);
		upload_audit_event(cmd->eventindex);
#endif
	}
}

static int shadow_wo_pm(const void *arg, const struct wo_pm_params *params)
{
	u64 x0 = TSP_REQUEST;
	u64 x1 = SMC_OPS_START_SHADOW;
	u64 x2 = current->pid;
	u64 x3 = 0;
	u64 x4 = *(u64 *)arg;

	if (*(params->exit_reason) == SMC_EXIT_PREEMPTED) {
		x0 = TSP_REQUEST;
		x1 = SMC_OPS_SCHEDTO;
		x2 = current->pid;
		x3 = *(params->ta);
		x4 = *(params->target);
	} else if (*(params->exit_reason) == SMC_EXIT_NORMAL) {
		x0 = TSP_REQUEST;
		x1 = SMC_OPS_SCHEDTO;
		x2 = current->pid;
		x3 = 0;
		x4 = 0;
		if (*(params->n_idled) > IDLED_COUNT) {
			*(params->n_idled) = 0;
			x1 = SMC_OPS_PROBE_ALIVE;
		}
	}
	isb();
	wmb();
	tlogd("%s: [cpu %d] x0=%llx x1=%llx x2=%llx x3=%llx x4=%llx\n", __func__,
		raw_smp_processor_id(), x0, x1, x2, x3, x4);
	do {
		asm volatile(
			"mov x0, %[fid]\n"
			"mov x1, %[a1]\n"
			"mov x2, %[a2]\n"
			"mov x3, %[a3]\n"
			"mov x4, %[a4]\n"
			"smc #0\n"
			"str x0, [%[re0]]\n"
			"str x1, [%[re1]]\n"
			"str x2, [%[re2]]\n"
			"str x3, [%[re3]]\n"
			:[fid] "+r"(x0), [a1] "+r"(x1), [a2] "+r"(x2),
			[a3] "+r"(x3), [a4] "+r"(x4)
			:[re0] "r"(params->ret), [re1] "r"(params->exit_reason),
			[re2] "r"(params->ta), [re3] "r"(params->target)
			: "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
			"x8", "x9", "x10", "x11", "x12", "x13",
			"x14", "x15", "x16", "x17");
	} while (0);

	isb();
	wmb();

	return 0;

}

static int shadow_thread_fn(void *arg)
{
	u64 ret = 0;
	u64 exit_reason = SMC_EXIT_MAX;
	u64 ta = 0;
	u64 target = 0;
	int n_preempted = 0;
	int n_idled = 0;
	int ret_val;
	struct pending_entry *pe = NULL;
	int rc;
	long long timeout;
	struct wo_pm_params params = { &n_idled, &ret, &exit_reason, &ta, &target };
	set_freezable();
	pe = init_pending_entry(current->pid);

	if (pe == NULL) {
		tloge("init pending entry failed\n");
		kfree(arg);
		return -ENOMEM;
	}
	isb();
	wmb();

retry:
#ifdef CONFIG_HISI_SECS_CTRL
	if (hisi_secs_power_on()) {
		tloge("hisi_secs_power_on failed\n");
		kfree(arg);
		release_pending_entry(pe);
		return -EINVAL;
	}
#endif
retry_wo_pm:
	ret_val = shadow_wo_pm(arg, &params);
	if (ret_val == -1)
		goto clean;
	tlogd("shadow thread return %lld\n", exit_reason);
	if (exit_reason == SMC_EXIT_PREEMPTED) {
		n_idled = 0;
		if (++n_preempted > PREEMPT_COUNT) {
			tlogi("%s: retry 10K times on CPU%d\n", __func__,
				smp_processor_id());
			n_preempted = 0;
		}
#ifndef CONFIG_PREEMPT
		/* yield cpu to avoid soft lockup */
		cond_resched();
#endif
		goto retry_wo_pm;
	} else if (exit_reason == SMC_EXIT_NORMAL) {
#ifdef CONFIG_HISI_SECS_CTRL
		if (hisi_secs_power_down()) {
			tloge("hisi_secs_power_down failed\n");
			ret_val = -1;
			goto clean_wo_pm;
		}
#endif
		n_preempted = 0;
		timeout = HZ * (long)(HZ_COUNT + ((uint8_t)current->pid & LOW_BYTE));
		rc = wait_event_freezable_timeout(pe->wq,
			atomic_read(&pe->run), (long)timeout);
		if (!rc)
			n_idled++;
		if (atomic_read(&pe->run) == SHADOW_EXIT_RUN) {
			tlogd("shadow thread work quit, be killed\n");
			goto clean_wo_pm;
		} else {
			atomic_set(&pe->run, 0);
			goto retry;
		}
	} else if (exit_reason == SMC_EXIT_SHADOW) {
		tlogd("shadow thread exit, it self\n");
	} else {
		tlogd("shadow thread exit with unknown code %ld\n", (long)exit_reason);
	}
clean:
#ifdef CONFIG_HISI_SECS_CTRL
	if (hisi_secs_power_down()) {
		tloge("hisi_secs_power_down failed\n");
		ret_val = -1;
	}
#endif
clean_wo_pm:
	kfree(arg);
	release_pending_entry(pe);
	return ret_val;
}

static void shadow_work_func(struct kthread_work *work)
{
	struct task_struct *shadow_thread = NULL;
	struct shadow_work *s_work =
		container_of(work, struct shadow_work, kthwork);
	uint64_t *target_arg = kmalloc(sizeof(uint64_t), GFP_KERNEL);

	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)target_arg)) {
		tloge("%s: kmalloc(8 bytes) failed\n", __func__);
		return;
	}
	if (memset_s(target_arg, sizeof(uint64_t),
		0, sizeof(uint64_t)) != EOK) {
		tloge("memset target_arg failed!.\n");
		kfree(target_arg);
		return;
	}
	*target_arg = s_work->target;
	shadow_thread = kthread_create(shadow_thread_fn, (void *)(uintptr_t)target_arg,
		"shadow_th/%lu", g_shadow_thread_id++);
	if (IS_ERR_OR_NULL(shadow_thread)) {
		kfree(target_arg);
		tloge("couldn't create shadow_thread %ld\n",
			PTR_ERR(shadow_thread));
		return;
	}
	tlogd("%s: create shadow thread %lu for target %llx\n",
		__func__, g_shadow_thread_id, *target_arg);
	wake_up_process(shadow_thread);
}

static int proc_smc_wakeup_ca(pid_t ca, int which)
{
	if (ca == 0) {
		tlogw("wakeup for ca = 0\n");
	} else {
		struct pending_entry *pe = find_pending_entry(ca);

		if (pe == NULL) {
			tlogd("invalid ca pid=%d for pending entry\n", (int)ca);
			return -1;
		}
		atomic_set(&pe->run, which);
		wake_up(&pe->wq);
		tlogd("wakeup pending thread %ld\n", (long)ca);
		put_pending_entry(pe);
	}
	return 0;
}

void wakeup_pe(struct pending_entry *pe)
{
	if (pe != NULL) {
		atomic_set(&pe->run, 1);
		wake_up(&pe->wq);
	}
}

int smc_wakeup_broadcast(void)
{
	foreach_pending_entry(wakeup_pe);
	return 0;
}

int smc_wakeup_ca(pid_t ca)
{
	return proc_smc_wakeup_ca(ca, 1); // set pe->run to 1
}

int smc_shadow_exit(pid_t ca)
{
	return proc_smc_wakeup_ca(ca, SHADOW_EXIT_RUN);
}

void fiq_shadow_work_func(uint64_t target)
{
	struct smc_cmd_ret secret = { SMC_EXIT_MAX, 0, target };

#ifdef CONFIG_HISI_SECS_CTRL
	if (g_secs_suspend_status == SECS_SUSPEND_STATUS)
		tloge("WARNING irq during suspend! No = %lld\n", target);
	if (hisi_secs_power_on()) {
		tloge("hisi_secs_power_on failed\n");
		return;
	}
#endif
	smp_smc_send(TSP_REQUEST, SMC_OPS_START_FIQSHD, current->pid,
		&secret, false);
#ifdef CONFIG_HISI_SECS_CTRL
	if (hisi_secs_power_down())
		tloge("hisi_secs_power_down failed\n");
#endif
	return;
}

int smc_queue_shadow_worker(uint64_t target)
{
	struct shadow_work work = {
		KTHREAD_WORK_INIT(work.kthwork, shadow_work_func),
		.target = target,
	};

	if (!kthread_queue_work(&g_ipi_helper_worker, &work.kthwork))
	{
		tloge("ipi helper work did't queue successfully, was already pending.\n");
		return -1;
	}
	kthread_flush_work(&work.kthwork);
	return 0;
}

#ifdef CONFIG_DRM_ADAPT
static void set_drm_strategy(void)
{
	if (!g_drm_mask_flag) {
		cpumask_clear(&g_drm_cpu_mask);
		cpumask_set_cpu(CPU_FOUR, &g_drm_cpu_mask);
		cpumask_set_cpu(CPU_FIVE, &g_drm_cpu_mask);
		cpumask_set_cpu(CPU_SIX, &g_drm_cpu_mask);
		cpumask_set_cpu(CPU_SEVEN, &g_drm_cpu_mask);
		g_drm_mask_flag = 1;
	}

	if (current->group_leader &&
		strstr(current->group_leader->comm, "drm@1.")) {
		set_cpus_allowed_ptr(current, &g_drm_cpu_mask);
		set_user_nice(current, -5); // set user prior to -5
	}
}
#endif

static int smc_ops_normal(enum CMD_REUSE *cmd_usage, int *cmd_index,
	int *last_index, struct pending_entry *pe, const struct tc_ns_smc_cmd *cmd, u64 ops)
{
	if (ops == SMC_OPS_NORMAL) {
		if (*cmd_usage == RESEND) {
			if (reuse_smc_in_entry(*cmd_index)) {
				tloge("reuse smc entry failed\n");
				release_smc_entry(*cmd_index);
				release_pending_entry(pe);
				return -1;
			}
		} else {
			*cmd_index = occupy_free_smc_in_entry(cmd);
			if (*cmd_index == -1) {
				tloge("there's no more smc entry\n");
				release_pending_entry(pe);
				return -1;
			}
		}
		if (*cmd_usage != CLEAR) {
			*cmd_index = *last_index;
			*cmd_usage = CLEAR;
		} else {
			*last_index = *cmd_index;
		}
		tlogd("submit new cmd: cmd.ca=%u cmd-id=%x ev-nr=%u cmd-index=%u last-index=%d\n",
			cmd->ca_pid, cmd->cmd_id,
			g_cmd_data->in[*cmd_index].event_nr, *cmd_index,
			*last_index);
	}
	return 0;
}

static int smp_smc_send_cmd_done(int cmd_index, struct tc_ns_smc_cmd *cmd,
	struct tc_ns_smc_cmd *in)
{
	cmd_result_check(cmd);
	switch (cmd->ret_val) {
	case TEEC_PENDING2: {
		unsigned int agent_id = cmd->agent_id;
		/* If the agent does not exist post
		 * the answer right back to the TEE
		 */
		if (agent_process_work(cmd, agent_id) != TEEC_SUCCESS)
			tloge("agent process work failed\n");
		return -1;
	}
	case TEE_ERROR_TAGET_DEAD:
	case TEEC_PENDING:
	/* just copy out, and let out to proceed */
	default:
		if (memcpy_s(in, sizeof(*in),
			cmd, sizeof(*cmd)) != EOK) {
			tloge("memcpy_s failed,%s line:%d", __func__, __LINE__);
			cmd->ret_val = -1;
		}

		break;
	}

	return 0;

}

#define SYM_NAME_LEN_MAX 16
#define SYM_NAME_LEN_1 7
#define SYM_NAME_LEN_2 4
#define CRASH_REG_NUM  3
#define LOW_FOUR_BITE  4

union crash_inf {
	uint64_t crash_reg[CRASH_REG_NUM];
	struct {
		uint8_t halt_reason:LOW_FOUR_BITE;
		uint8_t app:LOW_FOUR_BITE;
		char sym_name[SYM_NAME_LEN_1];
		uint16_t off;
		uint16_t size;
		uint32_t far;
		uint32_t fault;
		union {
			char sym_name_append[SYM_NAME_LEN_2];
			uint32_t elr;
		};
	} crash_msg;
};

static void print_crash_msg(union crash_inf *crash_info)
{
	static const char *tee_critical_app[] = {
		"gtask",
		"teesmcmgr",
		"hmsysmgr",
		"hmfilemgr",
		"platdrv",
		"vltmm_service",
		"kernel"
	};
	int app_num = sizeof(tee_critical_app) / sizeof(tee_critical_app[0]);
	const char *crash_app_name = "NULL";
	uint16_t off = crash_info->crash_msg.off;
	int app_index = crash_info->crash_msg.app & LOW_BYTE;
	int halt_reason = crash_info->crash_msg.halt_reason;
	int sret;

	crash_info->crash_msg.off = 0; // for end of sym_name

	if (app_index >= 0 && app_index < app_num)
		crash_app_name = tee_critical_app[app_index];
	else
		tloge("index error: %x\n", crash_info->crash_msg.app);

	// kernel
	if (app_index == (app_num - 1)) {
		tloge("====crash app:%s user_sym:%s kernel crash off/size: <0x%x/0x%x>\n",
		      crash_app_name, crash_info->crash_msg.sym_name,
		      off, crash_info->crash_msg.size);
		tloge("====crash halt_reason: 0x%x far:0x%x fault:0x%x elr:0x%x (ret_ip: 0x%llx)\n",
		      halt_reason, crash_info->crash_msg.far,
		      crash_info->crash_msg.fault, crash_info->crash_msg.elr,
		      crash_info->crash_reg[2]);
	} else { // user app
		char syms[SYM_NAME_LEN_MAX] = {0};

		sret = memcpy_s(syms, SYM_NAME_LEN_MAX,
			crash_info->crash_msg.sym_name, SYM_NAME_LEN_1);
		if (sret != EOK)
			tloge("memcpy sym_name failed!\n");
		sret = memcpy_s(syms + SYM_NAME_LEN_1,
			SYM_NAME_LEN_MAX - SYM_NAME_LEN_1,
			crash_info->crash_msg.sym_name_append, SYM_NAME_LEN_2);
		if (sret != EOK)
			tloge("memcpy sym_name_append failed!\n");
		tloge("====crash app:%s user_sym:%s + <0x%x/0x%x>\n",
		      crash_app_name, syms, off, crash_info->crash_msg.size);
		tloge("====crash far:0x%x fault:%x\n",
		      crash_info->crash_msg.far, crash_info->crash_msg.fault);
	}
}

static int smp_smc_send_process(struct tc_ns_smc_cmd *cmd, u64 ops,
	struct smc_cmd_ret *cmd_ret, int cmd_index)
{
	int ret;

#ifdef CONFIG_HISI_SECS_CTRL
	if (hisi_secs_power_on()) {
		tloge("hisi_secs_power_on failed\n");
		cmd->ret_val = -1;
		return -1;
	}
#endif

	ret = smp_smc_send(TSP_REQUEST, ops, current->pid, cmd_ret, ops != SMC_OPS_ABORT_TASK);

#ifdef CONFIG_HISI_SECS_CTRL
	if (hisi_secs_power_down()) {
		tloge("hisi_secs_power_down failed\n");
		cmd->ret_val = -1;
		return -1;
	}
#endif
	tlogd("smp_smc_send ret = %x, cmd_ret.exit=%ld, cmd_index=%d\n",
		ret, (long)cmd_ret->exit, cmd_index);
	isb();
	wmb();
	if (ret == (int)TSP_CRASH) {
		union crash_inf crash_info;
		crash_info.crash_reg[0] = cmd_ret->exit;
		crash_info.crash_reg[1] = cmd_ret->ta;
		crash_info.crash_reg[2] = cmd_ret->target;

		tloge("TEEOS has crashed!\n");
		print_crash_msg(&crash_info);

		g_sys_crash = 1;
		cmd_monitor_ta_crash(TYPE_CRASH_TEE);

		report_log_system_error();

		cmd->ret_val = -1;
		return -1;
	}

	return 0;
}

static int init_for_smc_send(struct tc_ns_smc_cmd *in, struct pending_entry **pe,
	struct tc_ns_smc_cmd *cmd, bool reuse)
{
	if (in == NULL) {
		tloge("Bad params\n");
		return -1;
	}

#ifdef CONFIG_DRM_ADAPT
	set_drm_strategy();
#endif
	*pe = init_pending_entry(current->pid);
	if (*pe == NULL) {
		tloge("init pending entry failed\n");
		return -ENOMEM;
	}
	in->ca_pid = current->pid;
	if (!reuse) {
		if (memcpy_s(cmd, sizeof(*cmd), in, sizeof(*in))) {
			tloge("memcpy_s failed,%s line:%d", __func__, __LINE__);
			release_pending_entry(*pe);
			return -1;
		}

	}
	return 0;
}

#define GOTO_RESLEEP 1
#define GOTO_RETRY_WITH_CMD 2

static int check_is_ca_killed(int cmd_index)
{
	/* if CA has not been killed */
	if (sigkill_pending(current) == 0) {
		if (!is_cmd_working_done(cmd_index)) {
			return GOTO_RESLEEP;
		} else {
			tloge("cmd done, may miss a spi!\n");
			show_cmd_bitmap_with_lock();
		}
	} else {
		/* signal pending, send abort cmd */
		tloge("wait event timeout and find pending signal\n");
		return GOTO_RETRY_WITH_CMD;
	}
	return 0;
}

static void clean_smc_resrc(enum CMD_REUSE cmd_usage, struct tc_ns_smc_cmd *cmd,
	struct pending_entry *pe, int cmd_index)
{
	if (cmd_usage != CLEAR && cmd->ret_val != TEEC_PENDING)
		release_smc_entry(cmd_index);
	release_pending_entry(pe);
}

static int set_abort_cmd(int index)
{
	acquire_smc_buf_lock(&g_cmd_data->smc_lock);
	if (!test_bit(index, g_cmd_data->doing_bitmap)) {
		release_smc_buf_lock(&g_cmd_data->smc_lock);
		tloge("can't abort an unprocess cmd\n");
		return -1;
	}

	if (g_cmd_data->in[index].global_cmd) {
		release_smc_buf_lock(&g_cmd_data->smc_lock);
		tloge("can't abort a global cmd\n");
		return -1;
	}

	g_cmd_data->in[index].cmd_id = GLOBAL_CMD_ID_KILL_TASK;
	g_cmd_data->in[index].global_cmd = true;
	clear_bit(index, g_cmd_data->doing_bitmap);
	release_smc_buf_lock(&g_cmd_data->smc_lock);
	tloge("set abort cmd success\n");

	return 0;
}

static enum SMC_OPS_EXIT process_abort_cmd(int index)
{
	if (set_abort_cmd(index) == 0)
		return SMC_OPS_ABORT_TASK;
	return SMC_OPS_SCHEDTO;
}

static int smp_smc_send_func(struct tc_ns_smc_cmd *in, uint32_t cmd_type,
	bool reuse)
{
	int cmd_index = 0;
	int last_index = 0;
	struct smc_cmd_ret cmd_ret = {0};
	struct tc_ns_smc_cmd cmd = { {0}, 0 };
	struct pending_entry *pe = NULL;
	u64 ops;
	enum CMD_REUSE cmd_usage = CLEAR;
	int ret;

	if (init_for_smc_send(in, &pe, &cmd, reuse) != 0)
		return -1;
	if (reuse) {
		last_index = cmd_index = in->event_nr;
		cmd_usage = RESEND;
	}
	ops = SMC_OPS_NORMAL;
retry:
	if (smc_ops_normal(&cmd_usage, &cmd_index,
		&last_index, pe, &cmd, ops) == -1)
		return -1;
retry_with_filled_cmd:
	tlogd("smp_smc_send start cmd_id = %u, ca = %u\n", cmd.cmd_id, cmd.ca_pid);

	if (smp_smc_send_process(&cmd, ops, &cmd_ret, cmd_index) == -1)
		goto clean;
	if (!is_cmd_working_done(cmd_index)) {
		if (cmd_ret.exit == SMC_EXIT_NORMAL) {
			/* task pending exit */
			tlogd("goto sleep, exit_reason=%lld\n", cmd_ret.exit);
resleep:
			if (wait_event_timeout(pe->wq, atomic_read(&pe->run),
				(long)(RESLEEP_TIMEOUT * HZ)) == 0) {
				tlogd("CA wait event for %d s\n", RESLEEP_TIMEOUT);
				ret = check_is_ca_killed(cmd_index);
				if (ret == GOTO_RESLEEP) {
					goto resleep;
				} else if (ret == GOTO_RETRY_WITH_CMD) {
					ops = (u64)process_abort_cmd(cmd_index);
					goto retry_with_filled_cmd;
				}
			}
			atomic_set(&pe->run, 0);

			if (is_cmd_working_done(cmd_index)) {
				tlogd("cmd is done\n");
				goto cmd_done;
			}
			ops = SMC_OPS_SCHEDTO;
			goto retry_with_filled_cmd;
		} else if (cmd_ret.exit == SMC_EXIT_ABORT) {
			ops = (u64)process_abort_cmd(cmd_index);
			goto retry_with_filled_cmd;
		} else {
			tloge("invalid cmd work state\n");
			cmd.ret_val = -1;
			goto clean;
		}
	} else {
cmd_done:
		if (copy_smc_out_entry(cmd_index, &cmd, &cmd_usage)) {
			cmd.ret_val = -1;
			goto clean;
		}
		if (smp_smc_send_cmd_done(cmd_index, &cmd, in) == -1) {
			ops = SMC_OPS_NORMAL;
			/* cmd will be reused */
			goto retry;
		}
	}
clean:
	clean_smc_resrc(cmd_usage, &cmd, pe, cmd_index);
	return cmd.ret_val;
}

static int smc_svc_thread_fn(void *arg)
{
	while (!kthread_should_stop()) {
		struct tc_ns_smc_cmd smc_cmd = { {0}, 0 };
		int ret;

		smc_cmd.global_cmd = true;
		smc_cmd.cmd_id = GLOBAL_CMD_ID_SET_SERVE_CMD;
		ret = smp_smc_send_func(&smc_cmd,
			TC_NS_CMD_TYPE_NS_TO_SECURE, false);
		tlogd("smc svc return 0x%x\n", ret);
	}
	tloge("smc_svc_thread stop ...\n");
	return 0;
}

static const char *g_hungtask_monitor_list[] = {
	"system_server", "fingerprintd", "atcmdserver", "keystore", "gatekeeperd",
	"volisnotd", "secure_storage", "secure_storage_s", "mediaserver",
	"vold",
	"IFAAPluginThrea"
};

bool is_tee_hungtask(struct task_struct *t)
{
	uint32_t i;
	uint32_t hungtask_num = sizeof(g_hungtask_monitor_list) / sizeof(g_hungtask_monitor_list[0]);

	if (t == NULL)
		return false;
	for (i = 0; i < hungtask_num; i++) {
		if (!strcmp(t->comm, g_hungtask_monitor_list[i])) {
			tloge("tee_hungtask detected:the hungtask is %s\n",
				t->comm);
			return true;
		}
	}
	return false;

}

void wakeup_tc_siq(void)
{
	atomic_set(&g_siq_th_run, 1); // init g_siq_th_run to 1
	wake_up_interruptible(&siq_th_wait);
}
/*
 * This function first power on crypto cell, then send smc cmd to trustedcore.
 * After finished, power off crypto cell.
 */
static int proc_tc_ns_smc(struct tc_ns_smc_cmd *cmd, bool reuse)
{
	int ret;
	struct cmd_monitor *item = NULL;

	if (g_sys_crash) {
		tloge("ERROR: sys crash happened!!!\n");
		return TEEC_ERROR_GENERIC;
	}
	if (cmd == NULL) {
		tloge("invalid cmd\n");
		return TEEC_ERROR_GENERIC;
	}
	tlogd(KERN_INFO "***smc call start on cpu %d ***\n",
		raw_smp_processor_id());
	item = cmd_monitor_log(cmd);
	ret = smp_smc_send_func(cmd, TC_NS_CMD_TYPE_NS_TO_SECURE, reuse);
	cmd_monitor_logend(item);
	return ret;
}

int tc_ns_smc(struct tc_ns_smc_cmd *cmd)
{
	return proc_tc_ns_smc(cmd, false);
}

int tc_ns_smc_with_no_nr(struct tc_ns_smc_cmd *cmd)
{
	return proc_tc_ns_smc(cmd, true);
}

static void smc_work_no_wait(uint32_t type)
{
	raw_smc_send(TSP_REQUEST, g_cmd_phys, type, true);
}

static void smc_work_set_cmd_buffer(struct work_struct *work)
{
	(void)work;
	smc_work_no_wait(TC_NS_CMD_TYPE_SECURE_CONFIG);
}

static void smc_work_init_secondary_cpus(struct work_struct *work)
{
	(void)work;
	smc_work_no_wait(TC_NS_CMD_TYPE_NS_TO_SECURE);
}

static void smc_set_cmd_buffer(void)
{
	struct work_struct work;

	INIT_WORK_ONSTACK(&work, smc_work_set_cmd_buffer);
	/* Run work on CPU 0 */
	schedule_work_on(0, &work);
	flush_work(&work);
	tlogd("smc set cmd buffer done\n");
}

static int smc_init_secondary_cpus(void)
{
	unsigned int i = 0;
	struct work_struct work;

	INIT_WORK_ONSTACK(&work, smc_work_init_secondary_cpus);
	/* Run work on all secondary cpus */
	get_online_cpus();
	for_each_online_cpu(i) {
		if (i != 0) {
#if CONFIG_CPU_AFF_NR
			if (i >= CONFIG_CPU_AFF_NR)
				break;
#endif
			schedule_work_on((int)i, &work);
			flush_work(&work);
			tlogd("init secondary cpu %u done\n", i);
		}
	}
	put_online_cpus();
	return 0;
}

#define compile_time_assert(cond, msg) typedef char ASSERT_##msg[(cond) ? 1 : -1] // cond is 1 or -1

compile_time_assert(sizeof(struct tc_ns_smc_queue) <= PAGE_SIZE,
	size_of_tc_ns_smc_queue_too_large);

int smc_init_data(const struct device *class_dev)
{
	int ret = 0;
	struct cpumask new_mask;

	if (class_dev == NULL || IS_ERR_OR_NULL(class_dev))
		return -ENOMEM;
	g_cmd_data = (struct tc_ns_smc_queue *)(uintptr_t)__get_free_page(
		GFP_KERNEL | __GFP_ZERO);
	if (g_cmd_data == NULL)
		return -ENOMEM;

	g_cmd_phys = virt_to_phys(g_cmd_data);
	/* Send the allocated buffer to TrustedCore for init */
	smc_set_cmd_buffer();
	if (smc_init_secondary_cpus()) {
		ret = -EINVAL;
		goto free_mem;
	}

	if (get_session_root_key()) {
		ret = -EFAULT;
		goto free_mem;
	}

	g_siq_thread = kthread_create(siq_thread_fn, NULL, "siqthread/%d", 0);
	if (unlikely(IS_ERR_OR_NULL(g_siq_thread))) {
		dev_err(class_dev, "couldn't create siqthread %ld\n",
			PTR_ERR(g_siq_thread));
		ret = (int)PTR_ERR(g_siq_thread);
		goto free_mem;
	}

	/*
	 * TEE Dump will disable IRQ/FIQ for about 500 ms, it's not
	 * a good choice to ask CPU0/CPU1 to do the dump.
	 * So, bind this kernel thread to other CPUs rather than CPU0/CPU1.
	 */
	cpumask_setall(&new_mask);
	cpumask_clear_cpu(CPU_ZERO, &new_mask);
	cpumask_clear_cpu(CPU_ONE, &new_mask);
	koadpt_kthread_bind_mask(g_siq_thread, &new_mask);
	g_ipi_helper_thread = kthread_create(kthread_worker_fn,
		&g_ipi_helper_worker, "ipihelper");
	if (IS_ERR_OR_NULL(g_ipi_helper_thread)) {
		dev_err(class_dev, "couldn't create ipi_helper_threads %ld\n",
			PTR_ERR(g_ipi_helper_thread));
		ret = (int)PTR_ERR(g_ipi_helper_thread);
		goto free_siq_worker;
	}
	wake_up_process(g_ipi_helper_thread);
	wake_up_process(g_siq_thread);
	init_cmd_monitor();
	INIT_LIST_HEAD(&g_pending_head);
	spin_lock_init(&g_pend_lock);
	return 0;
free_siq_worker:
	kthread_stop(g_siq_thread);
	g_siq_thread = NULL;
free_mem:
	free_page((unsigned long)(uintptr_t)g_cmd_data);
	g_cmd_data = NULL;
	free_root_key();
	return ret;
}

int init_smc_svc_thread(void)
{
	g_smc_svc_thread = kthread_create(smc_svc_thread_fn, NULL,
		"smc_svc_thread");
	if (unlikely(IS_ERR_OR_NULL(g_smc_svc_thread))) {
		tloge("couldn't create smc_svc_thread %ld\n",
			PTR_ERR(g_smc_svc_thread));
		return PTR_ERR(g_smc_svc_thread);
	}
	wake_up_process(g_smc_svc_thread);
	return 0;
}

int teeos_log_exception_archive(unsigned int eventid,
	const char *exceptioninfo)
{
#ifdef CONFIG_LOG_EXCEPTION
	int ret;
	struct imonitor_eventobj *teeos_obj = NULL;

	teeos_obj = imonitor_create_eventobj(eventid);
	if (exceptioninfo != NULL) {
		ret = imonitor_set_param(teeos_obj, 0,
			(long)(uintptr_t)exceptioninfo);
	} else {
		ret = imonitor_set_param(teeos_obj, 0,
			(long)(uintptr_t)"teeos something crash");
	}
	if (ret != 0) {
		tloge("imonitor_set_param failed\n");
		imonitor_destroy_eventobj(teeos_obj);
		return ret;
	}
	ret = imonitor_add_dynamic_path(teeos_obj,
		"/data/vendor/log/hisi_logs/tee");
	if (ret != 0) {
		tloge("add path failed\n");
		imonitor_destroy_eventobj(teeos_obj);
		return ret;
	}
	ret = imonitor_add_dynamic_path(teeos_obj, "/data/log/tee");
	if (ret != 0) {
		tloge("add path  failed\n");
		imonitor_destroy_eventobj(teeos_obj);
		return ret;
	}
	ret = imonitor_send_event(teeos_obj);
	imonitor_destroy_eventobj(teeos_obj);
	return ret;
#else
	return 0;
#endif
}

void smc_free_data(void)
{
	free_page((unsigned long)(uintptr_t)g_cmd_data);
	if (!IS_ERR_OR_NULL(g_smc_svc_thread)) {
		kthread_stop(g_smc_svc_thread);
		g_smc_svc_thread = NULL;
	}

	free_root_key();
}
