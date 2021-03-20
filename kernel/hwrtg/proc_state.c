/*
 * proc_state.c
 *
 * proc state manager
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

#include "include/proc_state.h"

#include <linux/atomic.h>
#include <linux/hisi_rtg.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/cred.h>
#include <linux/sched/task.h>
#include <linux/string.h>
#include <trace/events/sched.h>

#include <linux/sched/frame.h>
#include "include/set_rtg.h"
#include "include/frame_timer.h"
#include "include/aux.h"
#include "include/iaware_rtg.h"

#ifdef CONFIG_HW_QOS_THREAD
#include <chipset_common/hwqos/hwqos_common.h>
#endif

#define FRAME_START (1 << 0)
#define FRAME_END (1 << 1)
#define FRAME_USE_MARGIN_IMME (1 << 4)
#define FRAME_TIMESTAMP_SKIP_START (1 << 5)
#define FRAME_TIMESTAMP_SKIP_END (1 << 6)

#define MIN_RTG_QOS 2
#define DEFAULT_FREQ_CYCLE 4
#define MIN_FREQ_CYCLE 4
#define MAX_FREQ_CYCLE 16
#define DEFAULT_INVALID_INTERVAL 50

#define INIT_VALUE (-1)

static struct list_head g_margin_list_head;
static rwlock_t g_state_margin_lock;

static atomic_t g_rtg_uid;
static atomic_t g_rtg_pid;
static atomic_t g_rtg_tid;

static atomic_t g_boost_thread_pid = ATOMIC_INIT(INIT_VALUE);
static atomic_t g_boost_thread_tid = ATOMIC_INIT(INIT_VALUE);
static atomic_t g_boost_thread_min_util = ATOMIC_INIT(0);

static atomic_t g_act_state;
static atomic_t g_frame_state;
static atomic_t g_frame_freq = ATOMIC_INIT(0);

static int g_frame_max_util = DEFAULT_MAX_UTIL;
static int g_act_max_util = DEFAULT_MAX_UTIL;

/*lint -save -e446 -e666 -e734*/
static void set_frame(int margin, int frame_type)
{
	atomic_set(&g_frame_state, frame_type);
	if (set_frame_margin(margin) == SUCC)
		set_frame_timestamp(FRAME_START);
}

static void reset_frame(void)
{
	if (atomic_read(&g_frame_state) == FRAME_END_STATE) {
		pr_debug("[AWARE_RTG]: Frame state is already reset\n");
		return;
	}
	atomic_set(&g_frame_state, FRAME_END_STATE);
	set_frame_timestamp(FRAME_END);
}

static void set_activity(int margin)
{
	pr_debug("[AWARE_RTG]: %s margin=%d\n", __func__, margin);
	atomic_set(&g_act_state, ACTIVITY_BEGIN);
	if (set_frame_margin(margin) == SUCC) {
		set_frame_max_util(g_act_max_util);
		set_frame_timestamp(FRAME_START | FRAME_USE_MARGIN_IMME |
			FRAME_TIMESTAMP_SKIP_START);
	}
}

static void reset_activity(void)
{
	if (atomic_read(&g_act_state) == ACTIVITY_END) {
		pr_debug("[AWARE_RTG]: Activity state is already reset\n");
		return;
	}
	pr_debug("[AWARE_RTG]: %s\n", __func__);
	atomic_set(&g_act_state, ACTIVITY_END);
	set_frame_max_util(g_frame_max_util);
	set_frame_timestamp(FRAME_END | FRAME_TIMESTAMP_SKIP_END);
}

static int get_proc_state(const char *state_str)
{
	int state;

	pr_debug("[AWARE_RTG,str]: %s\n", state_str);
	if (!strcmp("activity", state_str))
		state = ACTIVITY_BEGIN;
	else if (!strcmp("frame0", state_str))
		state = FRAME_BUFFER_0;
	else if (!strcmp("frame1", state_str))
		state = FRAME_BUFFER_1;
	else if (!strcmp("frame2", state_str))
		state = FRAME_BUFFER_2;
	else if (!strcmp("click", state_str))
		state = FRAME_CLICK;
	else
		state = STATE_MAX;
	return state;
}

/*lint -e429*/
static int put_state_margin(const char *state_str, const char *margin_str)
{
	struct state_margin_list *temp = NULL;
	int state;
	int margin;

	if ((state_str == NULL) || (margin_str == NULL))
		return -1;

	if (kstrtoint((const char *)margin_str, DECIMAL, &margin))
		return -1;

	state = get_proc_state(state_str);
	if ((state <= STATE_MIN) || (state >= STATE_MAX))
		return -1;

	temp = kmalloc(sizeof(*temp), GFP_ATOMIC);
	if (temp == NULL)
		return -1;

	temp->state = state;
	temp->margin = margin;
	pr_info("[AWARE_RTG]: %s state=%d, margin=%d", __func__, state, margin);

	write_lock(&g_state_margin_lock);
	list_add_tail(&(temp->list), &g_margin_list_head);
	write_unlock(&g_state_margin_lock);
	return 0;
}
/*lint +e429*/

static int get_rme_margin(int state)
{
	int margin = INVALID_VALUE;
	if ((state >= FRAME_BUFFER_3) && (state <= FRAME_BUFFER_18))
		margin = -state;
	return margin;
}

static int get_state_margin(int state)
{
	struct state_margin_list *temp = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	int margin = INVALID_VALUE;

	read_lock(&g_state_margin_lock);

	list_for_each_safe(pos, n, &g_margin_list_head) {
		temp = list_entry(pos, struct state_margin_list, list);
		if (temp->state == state) {
			margin = temp->margin;
			break;
		}
	}

	if (margin == INVALID_VALUE)
		margin = get_rme_margin(state);

	read_unlock(&g_state_margin_lock);
	return margin;
}

#ifdef CONFIG_HW_QOS_THREAD
static int get_qos(pid_t tid)
{
	int qos = -1;
	struct task_struct *tsk = NULL;

	if (atomic_read(&g_rtg_uid) != current_uid().val)
		return qos;

	rcu_read_lock();
	if (tid > 0) {
		tsk = find_task_by_vpid(tid);
		if (tsk == NULL) {
			rcu_read_unlock();
			return qos;
		}
		qos = get_task_qos(tsk);
	}
	rcu_read_unlock();
	pr_debug("[AWARE_RTG]: %s tid=%d, qos=%d", __func__, tid, qos);
	return qos;
}
#elif defined CONFIG_HW_VIP_THREAD
static int get_vip(pid_t tid)
{
	int vip = 0;
	struct task_struct *tsk = NULL;

	if (atomic_read(&g_rtg_uid) != current_uid().val)
		return vip;

	rcu_read_lock();
	if (tid > 0) {
		tsk = find_task_by_vpid(tid);
		if (tsk == NULL) {
			rcu_read_unlock();
			return vip;
		}

		get_task_struct(tsk);
		vip = tsk->static_vip;
		put_task_struct(tsk);
	}
	rcu_read_unlock();
	pr_debug("[AWARE_RTG]: %s tid=%d, vip=%d", __func__, tid, vip);
	return vip;
}
#endif

static void update_rtg_frame_thread(const struct rtg_data_head *proc_info)
{
	int uid = proc_info->uid;
	int pid = proc_info->pid;
	int tid = proc_info->tid;

	if ((uid == -1) || (pid == -1) || (tid == -1)) {
		atomic_set(&g_rtg_uid, -1);
		atomic_set(&g_rtg_pid, -1);
		atomic_set(&g_rtg_tid, -1);
		update_frame_thread(-1, -1);
		return;
	}

	if (uid != atomic_read(&g_rtg_uid))
		atomic_set(&g_rtg_uid, uid);
	if (pid != atomic_read(&g_rtg_pid))
		atomic_set(&g_rtg_pid, pid);
	if (tid != atomic_read(&g_rtg_tid))
		atomic_set(&g_rtg_tid, tid);

	update_frame_thread(pid, tid);
	pr_debug("[AWARE_RTG] %s uid=%d, pid=%d, tid=%d\n",
			__func__, atomic_read(&g_rtg_uid),
			atomic_read(&g_rtg_pid), atomic_read(&g_rtg_tid));
}

static void free_margin_list(void)
{
	struct state_margin_list *temp = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;

	write_lock(&g_state_margin_lock);
	list_for_each_safe(pos, n, &g_margin_list_head) {
		temp = list_entry(pos, struct state_margin_list, list);
		list_del(&temp->list);
		kfree(temp);
	}
	write_unlock(&g_state_margin_lock);
}

static void start_frame_freq(void)
{
	if (atomic_read(&g_frame_freq) == 0) {
		atomic_set(&g_frame_freq, 1);
		set_frame_sched_state(true);
	}
}

int init_proc_state(const int *config, int len)
{
	struct rtg_load_mode mode;

	if ((config == NULL) || (len != RTG_CONFIG_NUM))
		return -INVALID_ARG;
	rwlock_init(&g_state_margin_lock);

	write_lock(&g_state_margin_lock);

	INIT_LIST_HEAD(&g_margin_list_head);
	write_unlock(&g_state_margin_lock);

	atomic_set(&g_rtg_uid, -1);
	atomic_set(&g_rtg_pid, -1);
	atomic_set(&g_rtg_tid, -1);
	atomic_set(&g_act_state, -1);
	atomic_set(&g_frame_state, -1);

	if (config[RTG_LOAD_FREQ] != INVALID_VALUE) {
		mode.grp_id = DEFAULT_RT_FRAME_ID;
		mode.freq_enabled = !config[RTG_LOAD_FREQ];
		mode.util_enabled = 1;
		sched_set_group_load_mode(&mode);
	}

	if ((config[RTG_FREQ_CYCLE] >= MIN_FREQ_CYCLE) &&
		(config[RTG_FREQ_CYCLE] <= MAX_FREQ_CYCLE))
		sched_set_group_freq_update_interval(DEFAULT_RT_FRAME_ID,
				(unsigned int)config[RTG_FREQ_CYCLE]);
	else
		sched_set_group_freq_update_interval(DEFAULT_RT_FRAME_ID,
				DEFAULT_FREQ_CYCLE);

	if (config[RTG_INVALID_INTERVAL] != INVALID_VALUE)
		sched_set_group_util_invalid_interval(DEFAULT_RT_FRAME_ID,
				config[RTG_INVALID_INTERVAL]);
	else
		sched_set_group_util_invalid_interval(DEFAULT_RT_FRAME_ID,
				DEFAULT_INVALID_INTERVAL);

	if ((config[RTG_FRAME_MAX_UTIL] > 0) &&
		(config[RTG_FRAME_MAX_UTIL] < DEFAULT_MAX_UTIL))
		g_frame_max_util = config[RTG_FRAME_MAX_UTIL];
	if ((config[RTG_ACT_MAX_UTIL] > 0) &&
		(config[RTG_ACT_MAX_UTIL] < DEFAULT_MAX_UTIL))
		g_act_max_util = config[RTG_ACT_MAX_UTIL];

	set_frame_max_util(g_frame_max_util);

	return SUCC;
}

void deinit_proc_state(void)
{
	free_margin_list();
	atomic_set(&g_rtg_uid, -1);
	atomic_set(&g_rtg_pid, -1);
	atomic_set(&g_rtg_tid, -1);
	atomic_set(&g_act_state, -1);
	atomic_set(&g_frame_state, -1);
}

int parse_config(const struct rtg_str_data *rs_data)
{
	int len;
	char *p = NULL;
	char *tmp = NULL;
	char *data = NULL;

	if (rs_data == NULL)
		return -INVALID_ARG;
	data = rs_data->data;
	len = rs_data->len;
	if ((data == NULL) || (strlen(data) != len)) //lint !e737
		return -INVALID_ARG;

	// eg: margin0:-1;margin1:0;margin2:1;margin3:2;margin4:3
	p = strsep(&data, ";");
	while (p != NULL) {
		tmp = strsep(&p, ":");
		if (put_state_margin(tmp, p)) {
			pr_err("[AWARE_RTG] parse config failed\n");
			return -INVALID_ARG;
		}
		p = strsep(&data, ";");
	}
	return SUCC;
}

int parse_aux_thread(const struct rtg_str_data *rs_data)
{
	char *p = NULL;
	char *tmp = NULL;
	char *data = NULL;
	int id;
	int tid = 0;
	int enable = 0;
	struct aux_info info;

	if (rs_data == NULL)
		return -INVALID_ARG;
	data = rs_data->data;
	if ((data == NULL) || (rs_data->len <= 0) ||
		(rs_data->len > MAX_DATA_LEN))
		return -INVALID_ARG;

	// eg: auxTid:10000;enable:1;minUtil:100;rtgPrio:1
	for (p = strsep(&data, ";"); p != NULL; p = strsep(&data, ";")) {
		tmp = strsep(&p, ":");
		if ((tmp == NULL) || (p == NULL))
			continue;
		if (kstrtoint((const char *)p, DECIMAL, &id))
			return -INVALID_ARG;
		if (!strcmp(tmp, "auxTid")) {
			tid = id;
		} else if (!strcmp(tmp, "enable")) {
			enable = id;
		} else if (!strcmp(tmp, "minUtil")) {
			info.min_util = id;
		} else if (!strcmp(tmp, "rtgPrio")) {
			info.prio = id;
		} else {
			pr_err("[AWARE_RTG] parse config failed!\n");
			return -INVALID_ARG;
		}
	}

	return sched_rtg_aux(tid, enable, &info);
}

#define AUX_KEY_COMM_MAX 12
struct st_aux_comms {
	int count;
	char comms[AUX_KEY_COMM_MAX][TASK_COMM_LEN];
};

static struct st_aux_comms s_aux_comms = {
	.count = 0,
	.comms = {{0}}
};

int parse_aux_comm_config(const struct rtg_str_data *rs_data)
{
	char *tmp = NULL;
	char *p = NULL;
	int len;

	if (rs_data == NULL)
		return -INVALID_ARG;

	p = rs_data->data;
	len = rs_data->len;
	if ((p == NULL) || (strlen(p) != len)) //lint !e737
		return -INVALID_ARG;

	s_aux_comms.count = 0;

	// eg: keyThreads:Chrome_InProcRe,CrRendererMain
	tmp = strsep(&p, ":");
	while (p != NULL) {
		if (s_aux_comms.count >= AUX_KEY_COMM_MAX)
			break;

		tmp = strsep(&p, ",");
		if (tmp && *tmp != '\0') {
			strncpy(s_aux_comms.comms[s_aux_comms.count],
				tmp, TASK_COMM_LEN - 1);
			s_aux_comms.count++;
		}
	}

	return SUCC;
}

bool is_key_aux_comm(const struct task_struct *task, const char *comm)
{
	int i;

	if (!comm || !task || !is_rtg_enable() ||
		(s_aux_comms.count == 0))
		return false;

	for (i = 0; i < s_aux_comms.count; ++i) {
		if (strstr(comm, s_aux_comms.comms[i]))
			return true;
	}

	return false;
}

static void set_min_util_detail(int tid, int min_util)
{
	struct task_struct *task = NULL;

	rcu_read_lock();
	task = find_task_by_vpid(tid);
	if (!task) {
		rcu_read_unlock();
		return;
	}
	get_task_struct(task);
	rcu_read_unlock();

	(void)set_task_min_util(task, min_util);
	put_task_struct(task);
}

void set_boost_thread_min_util(int min_util)
{
	pid_t pid;
	pid_t tid;

	if ((min_util > DEFAULT_MAX_UTIL) || (min_util < 0))
		return;

	pid = atomic_read(&g_boost_thread_pid);
	tid = atomic_read(&g_boost_thread_tid);
	if ((pid == INIT_VALUE) || (tid == INIT_VALUE))
		return;

	trace_rtg_frame_sched("boost_util", min_util);
	atomic_set(&g_boost_thread_min_util, min_util);

	set_min_util_detail(pid, min_util);
	set_min_util_detail(tid, min_util);
}

static void update_boost_thread(int pid, int tid)
{
	int min_util = 0;

	if ((pid == INIT_VALUE) || (tid == INIT_VALUE)) {
		set_boost_thread_min_util(0);
		atomic_set(&g_boost_thread_pid, INIT_VALUE);
		atomic_set(&g_boost_thread_tid, INIT_VALUE);
		return;
	}

	if ((pid != atomic_read(&g_boost_thread_pid)) ||
		(tid != atomic_read(&g_boost_thread_tid))) {
		min_util = atomic_read(&g_boost_thread_min_util);
		set_boost_thread_min_util(0);
	}

	atomic_set(&g_boost_thread_pid, pid);
	atomic_set(&g_boost_thread_tid, tid);
	if (min_util != 0)
		set_boost_thread_min_util(min_util);
}

int parse_boost_thread(const struct rtg_str_data *rs_data)
{
	char *p = NULL;
	char *tmp = NULL;
	char *data = NULL;
	pid_t pid = INIT_VALUE;
	pid_t tid = INIT_VALUE;
	int id;

	if (rs_data == NULL)
		return -INVALID_ARG;
	data = rs_data->data;
	if ((data == NULL) || (rs_data->len <= 0) || (rs_data->len > MAX_DATA_LEN))
		return -INVALID_ARG;

	for (p = strsep(&data, ";"); p != NULL; p = strsep(&data, ";")) {
		tmp = strsep(&p, ":");
		if (tmp == NULL || p == NULL)
			continue;
		if (kstrtoint((const char *)p, DECIMAL, &id))
			return -INVALID_ARG;

		if (!strcmp(tmp, "boostUid")) {
			continue;
		} else if (!strcmp(tmp, "utid")) {
			pid = id;
		} else if (!strcmp(tmp, "rtid")) {
			tid = id;
		} else {
			pr_err("[AWARE_RTG] parse thread boost config failed!\n");
			return -INVALID_ARG;
		}
	}

	update_boost_thread(pid, tid);
	return SUCC;
}

int parse_frame_thread(const struct rtg_str_data *rs_data)
{
	char *p = NULL;
	char *tmp = NULL;
	char *data = NULL;
	int id;
	struct rtg_data_head proc_info;

	if (rs_data == NULL)
		return -INVALID_ARG;
	data = rs_data->data;
	if ((data == NULL) || (rs_data->len <= 0) ||
		(rs_data->len > MAX_DATA_LEN))
		return -INVALID_ARG;

	proc_info.uid = 0;
	proc_info.pid = 0;
	proc_info.tid = 0;
	// eg: fgUid:10000;uiTid:10000;renderTid:10000
	for (p = strsep(&data, ";"); p != NULL; p = strsep(&data, ";")) {
		tmp = strsep(&p, ":");
		if (tmp == NULL || p == NULL)
			continue;
		if (kstrtoint((const char *)p, DECIMAL, &id))
			return -INVALID_ARG;

		if (!strcmp(tmp, "fgUid")) {
			proc_info.uid = id;
		} else if (!strcmp(tmp, "uiTid")) {
			proc_info.pid = id;
		} else if (!strcmp(tmp, "renderTid")) {
			proc_info.tid = id;
		} else {
			pr_err("[AWARE_RTG] parse config failed!\n");
			return -INVALID_ARG;
		}
	}

	update_rtg_frame_thread(&proc_info);
	reset_activity();
	return SUCC;
}

int update_frame_state(const struct rtg_data_head *rtg_head,
	int frame_type, bool in_frame)
{
	int margin;

	if ((rtg_head == NULL) || (rtg_head->tid != current->pid))
		return -FRAME_ERR_PID;

	if (atomic_read(&g_act_state) == ACTIVITY_BEGIN)
		return -FRAME_IS_ACT;

	if ((rtg_head->tid != atomic_read(&g_rtg_pid)) &&
		(rtg_head->tid != atomic_read(&g_rtg_tid)))
		return -FRAME_ERR_TID;

	if (in_frame) {
		start_frame_freq();
		margin = get_state_margin(frame_type);
		if (margin != INVALID_VALUE)
			set_frame(margin, frame_type);
	} else {
		reset_frame();
	}
	return SUCC;
}

int set_min_util(const struct rtg_data_head *rtg_head, int min_util)
{
	if ((rtg_head == NULL) || (rtg_head->tid != current->pid))
		return -FRAME_ERR_PID;

	if (atomic_read(&g_act_state) == ACTIVITY_BEGIN)
		return -FRAME_IS_ACT;

	set_frame_min_util(min_util, false);
	return SUCC;
}

int set_margin(const struct rtg_data_head *rtg_head, int margin)
{
	if ((rtg_head == NULL) || (rtg_head->tid != current->pid))
		return -FRAME_ERR_PID;

	if (atomic_read(&g_act_state) == ACTIVITY_BEGIN)
		return -FRAME_IS_ACT;

	set_frame_margin(margin);
	return SUCC;
}

int set_min_util_and_margin(const struct rtg_data_head *rtg_head,
			int min_util, int margin)
{
	if ((rtg_head == NULL) || (rtg_head->tid != current->pid))
		return -FRAME_ERR_PID;

	if (atomic_read(&g_act_state) == ACTIVITY_BEGIN)
		return -FRAME_IS_ACT;

	set_frame_min_util_and_margin(min_util, margin);
	return SUCC;
}

int check_rme_head(const struct rtg_data_head *rtg_head)
{
	if ((rtg_head == NULL) || (rtg_head->tid != current->pid))
		return -FRAME_ERR_PID;

	if ((rtg_head->tid != atomic_read(&g_rtg_pid)) &&
		(rtg_head->tid != atomic_read(&g_rtg_tid)))
		return -FRAME_ERR_TID;
	return SUCC;
}

int update_act_state(const struct rtg_data_head *rtg_head, bool in_activity)
{
#if ((!defined(CONFIG_HW_QOS_THREAD)) && (!defined(CONFIG_HW_VIP_THREAD)))
	return -ACT_ERR_QOS;
#endif
	int margin;

	if ((rtg_head == NULL) || (rtg_head->uid != current_uid().val) ||
		(rtg_head->uid != atomic_read(&g_rtg_uid)))
		return -ACT_ERR_UID;

#ifdef CONFIG_HW_QOS_THREAD
	if (get_qos(rtg_head->tid) < MIN_RTG_QOS)
		return -ACT_ERR_QOS;
#elif defined CONFIG_HW_VIP_THREAD
	if (get_vip(rtg_head->tid) <= 0)
		return -ACT_ERR_QOS;
#endif

	if (in_activity) {
		start_frame_freq();
		margin = get_state_margin(ACTIVITY_BEGIN);
		if (margin != INVALID_VALUE)
			set_activity(margin);
	} else {
		reset_activity();
	}
	return SUCC;
}

int is_cur_frame(void)
{
	if (atomic_read(&g_rtg_uid) != current_uid().val)
		return 0;
	if ((current->pid == atomic_read(&g_rtg_pid)) ||
		(current->pid == atomic_read(&g_rtg_tid)))
		return 1;
	return 0;
}

int stop_frame_freq(const struct rtg_data_head *rtg_head)
{
	if (rtg_head == NULL)
		return -INVALID_ARG;

	if (rtg_head->tid != atomic_read(&g_rtg_pid))
		return -FRAME_ERR_PID;

	atomic_set(&g_frame_freq, 0);
	set_frame_sched_state(false);
	return 0;
}

void start_rtg_boost(void)
{
	set_frame_sched_state(true);
}

bool is_rtg_sched_enable(void)
{
	return atomic_read(&g_frame_freq) == 1;
}

bool is_frame_start(void)
{
	if ((atomic_read(&g_frame_state) == FRAME_END_STATE) &&
		(atomic_read(&g_act_state) == ACTIVITY_END))
		return false;
	return true;
}

/*lint -restore*/
