/*
 * frame_info.c
 *
 * Frame-based load tracking for rt_frame and hisi_rtg
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

#include "frame_info.h"

#include <linux/cpufreq.h>
#include <linux/hisi_rtg.h>
#include <linux/sched.h>
#include <trace/events/sched.h>
#include <../kernel/sched/sched.h>

#include "hwrtg/set_rtg.h"

/* FPS value : [1, 120] */
#define DEFAULT_FRAME_RATE 60
#define MIN_FRAME_RATE 1
#define MAX_FRAME_RATE 120

/* MARGIN value : [-100, 100] */
#define DEFAULT_VLOAD_MARGIN 16
#define MIN_VLOAD_MARGIN (-100)
#define MAX_VLOAD_MARGIN 0xffff

#define FRAME_MAX_VLOAD SCHED_CAPACITY_SCALE
#define FRAME_MAX_LOAD SCHED_CAPACITY_SCALE
#define FRAME_UTIL_INVALID_FACTOR 4
#define FRAME_DEFAULT_MAX_UTIL SCHED_CAPACITY_SCALE

#define INVALID_PREFERRED_CLUSTER 10

static atomic_t g_start_frame = ATOMIC_INIT(0);
static struct task_struct *g_pid_task;
static struct task_struct *g_tid_task;
static struct frame_info g_frame_info;

/*lint -save -e508 -e712 -e732 -e571 -e737 -e563*/
bool is_frame_task(struct task_struct *task)
{
	struct related_thread_group *grp = NULL;

	if (unlikely(!task))
		return false;

	rcu_read_lock();
	grp = task_related_thread_group(task);
	rcu_read_unlock();

	return (grp && (grp->id == DEFAULT_RT_FRAME_ID));
}
EXPORT_SYMBOL_GPL(is_frame_task);

static inline struct related_thread_group *frame_rtg(void)
{
	return lookup_related_thread_group(DEFAULT_RT_FRAME_ID);
}

static inline struct frame_info *rtg_frame_info_inner(
	const struct related_thread_group *grp)
{
	return (struct frame_info *)grp->private_data;
}

struct frame_info *rtg_frame_info(void)
{
	struct frame_info *frame_info = NULL;
	struct related_thread_group *grp = frame_rtg();

	if (unlikely(!grp))
		return NULL;

	frame_info = rtg_frame_info_inner(grp);
	return frame_info;
}

int set_frame_rate(int rate)
{
	struct frame_info *frame_info = NULL;

	if ((rate < MIN_FRAME_RATE) || (rate > MAX_FRAME_RATE)) {
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid QOS(rate) value",
			__func__);
		return -EINVAL;
	}

	frame_info = rtg_frame_info();
	if (unlikely(!frame_info))
		return -EINVAL;

	frame_info->qos_frame = rate;
	frame_info->qos_frame_time = NSEC_PER_SEC / rate;
	frame_info->max_vload_time =
		frame_info->qos_frame_time / NSEC_PER_MSEC +
		frame_info->vload_margin;
	trace_rtg_frame_sched("FRAME_QOS", rate);
	trace_rtg_frame_sched("FRAME_MAX_TIME", frame_info->max_vload_time);
	return 0;
}
EXPORT_SYMBOL_GPL(set_frame_rate);

int set_frame_margin(int margin)
{
	struct frame_info *frame_info = NULL;

	if ((margin < MIN_VLOAD_MARGIN) || (margin > MAX_VLOAD_MARGIN)) {
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid MARGIN value",
			__func__);
		return -EINVAL;
	}

	frame_info = rtg_frame_info();
	if (unlikely(!frame_info))
		return -EINVAL;

	frame_info->vload_margin = margin;
	frame_info->max_vload_time =
		frame_info->qos_frame_time / NSEC_PER_MSEC +
		frame_info->vload_margin;
	trace_rtg_frame_sched("FRAME_MARGIN", margin);
	trace_rtg_frame_sched("FRAME_MAX_TIME", frame_info->max_vload_time);
	return 0;
}
EXPORT_SYMBOL_GPL(set_frame_margin);

int set_frame_max_util(int max_util)
{
	struct frame_info *frame_info = NULL;

	if ((max_util < 0) || (max_util > SCHED_CAPACITY_SCALE)) {
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid max_util value",
			__func__);
		return -EINVAL;
	}

	frame_info = rtg_frame_info();
	if (unlikely(!frame_info))
		return -EINVAL;

	frame_info->frame_max_util = max_util;
	trace_rtg_frame_sched("FRAME_MAX_UTIL", frame_info->frame_max_util);
	return 0;
}
EXPORT_SYMBOL_GPL(set_frame_max_util);

void set_frame_sched_state(bool enable)
{
	struct related_thread_group *grp = NULL;
	struct frame_info *frame_info = NULL;

	grp = frame_rtg();
	if (unlikely(!grp))
		return;

	frame_info = rtg_frame_info_inner(grp);
	if (unlikely(!frame_info))
		return;

	if (enable) {
		if (atomic_read(&g_start_frame) == 1)
			return;
		atomic_set(&g_start_frame, 1);
		trace_rtg_frame_sched("FRAME_SCHED_ENABLE", 1);

		frame_info->prev_fake_load_util = 0;
		frame_info->prev_frame_load_util = 0;
		frame_info->frame_vload = 0;
		frame_info_rtg_load(frame_info)->curr_window_load = 0;
	} else {
		if (atomic_read(&g_start_frame) == 0)
			return;
		atomic_set(&g_start_frame, 0);
		trace_rtg_frame_sched("FRAME_SCHED_ENABLE", 0);

		(void)sched_set_group_normalized_util(DEFAULT_RT_FRAME_ID,
			0, FRAME_NORMAL_UPDATE);
		frame_info->status = FRAME_END;

		trace_rtg_frame_sched("preferred_cluster",
			INVALID_PREFERRED_CLUSTER);
	}

	if (g_pid_task)
		set_frame_rtg_thread(g_pid_task, enable);
	if (g_tid_task)
		set_frame_rtg_thread(g_tid_task, enable);

	trace_rtg_frame_sched("FRAME_STATUS", frame_info->status);
}
EXPORT_SYMBOL_GPL(set_frame_sched_state);

static struct task_struct *do_update_thread(int pid,
	struct task_struct *old_task)
{
	struct task_struct *task = NULL;

	if (pid > 0) {
		if (old_task && (pid == old_task->pid))
			return old_task;

		rcu_read_lock();
		task = find_task_by_vpid(pid);
		if (task)
			get_task_struct(task);
		rcu_read_unlock();
	}

	if (atomic_read(&g_start_frame) == 1) {
		set_frame_rtg_thread(old_task, false);
		set_frame_rtg_thread(task, true);
	}

	if (old_task)
		put_task_struct(old_task);

	return task;
}

void update_frame_thread(int pid, int tid)
{
	g_pid_task = do_update_thread(pid, g_pid_task);
	g_tid_task = do_update_thread(tid, g_tid_task);
}
EXPORT_SYMBOL_GPL(update_frame_thread);

int set_frame_timestamp(unsigned long timestamp)
{
	int ret;

	if (atomic_read(&g_start_frame) == 0)
		return -EINVAL;

	ret = sched_set_group_window_rollover(DEFAULT_RT_FRAME_ID);
	if (!ret)
		ret = set_frame_status(timestamp);

	return ret;
}
EXPORT_SYMBOL_GPL(set_frame_timestamp);

/*
 * frame_vload [0~1024]
 * vtime: now - timestamp
 * max_time: frame_info->qos_frame_time + vload_margin
 * load = F(vtime)
 *      = vtime ^ 2 - vtime * max_time + FRAME_MAX_VLOAD * vtime / max_time;
 *      = vtime * (vtime + FRAME_MAX_VLOAD / max_time - max_time);
 * [0, 0] -=> [max_time, FRAME_MAX_VLOAD]
 *
 */
static u64 calc_frame_vload(const struct frame_info *frame_info, u64 timeline)
{
	u64 vload;
	int vtime = timeline / NSEC_PER_MSEC;
	int max_time = frame_info->max_vload_time;
	int factor;

	if ((max_time <= 0) || (vtime > max_time))
		return FRAME_MAX_VLOAD;

	factor = vtime + FRAME_MAX_VLOAD / max_time;
	/* margin maybe negative */
	if ((vtime <= 0) || (factor <= max_time))
		return 0;

	vload = (u64)vtime * (u64)(factor - max_time);
	return vload;
}

static inline void frame_boost(struct frame_info *frame_info)
{
	if (frame_info->frame_util < frame_info->frame_boost_min_util)
		frame_info->frame_util = frame_info->frame_boost_min_util;
}

/*
 * frame_load : caculate frame load using exec util
 */
static inline u64 calc_frame_exec(const struct frame_info *frame_info)
{
	if (frame_info->qos_frame_time > 0)
		return (frame_info_rtg_load(frame_info)->curr_window_exec <<
			SCHED_CAPACITY_SHIFT) / frame_info->qos_frame_time;
	else
		return 0;
}

/*
 * frame_load: vload for FRMAE_END and FRAME_INVALID
 */
static inline u64 calc_frame_load(const struct frame_info *frame_info)
{
	return (frame_info_rtg_load(frame_info)->curr_window_load <<
		SCHED_CAPACITY_SHIFT) / frame_info->qos_frame_time;
}

/*
 * real_util:
 * max(last_util, virtual_util, boost_util, phase_util, frame_min_util)
 */
static u64 calc_frame_util(const struct frame_info *frame_info, bool fake)
{
	unsigned long load_util;

	if (fake)
		load_util = frame_info->prev_fake_load_util;
	else
		load_util = frame_info->prev_frame_load_util;
	load_util = max_t(unsigned long, load_util, frame_info->frame_min_util);
	return min_t(unsigned long, max_t(unsigned long, load_util,
			frame_info->frame_vload),
			READ_ONCE(frame_info->frame_max_util));
}

static u64 calc_prev_frame_load_util(const struct frame_info *frame_info)
{
	u64 prev_frame_load = frame_info->prev_frame_load;
	u64 frame_util = 0;

	if (prev_frame_load >= frame_info->qos_frame_time)
		frame_util = FRAME_MAX_LOAD;
	else
		frame_util = (prev_frame_load << SCHED_CAPACITY_SHIFT) /
			frame_info->qos_frame_time;
	return frame_util;
}

static u64 calc_prev_fake_load_util(const struct frame_info *frame_info)
{
	u64 prev_frame_load = frame_info->prev_frame_load;
	u64 prev_frame_time = max_t(unsigned long, frame_info->prev_frame_time,
		frame_info->qos_frame_time);
	u64 frame_util = 0;

	if (prev_frame_time > 0)
		frame_util = (prev_frame_load << SCHED_CAPACITY_SHIFT) /
			prev_frame_time;

	if (frame_util > FRAME_MAX_LOAD)
		frame_util = FRAME_MAX_LOAD;
	return frame_util;
}

/* last frame load tracking */
static void update_frame_prev_load(struct frame_info *frame_info, bool fake)
{
	/* last frame load tracking */
	frame_info->prev_frame_exec =
		frame_info_rtg_load(frame_info)->prev_window_exec;
	frame_info->prev_frame_time =
		frame_info_rtg(frame_info)->prev_window_time;
	frame_info->prev_frame_load =
		frame_info_rtg_load(frame_info)->prev_window_load;

	if (fake)
		frame_info->prev_fake_load_util =
			calc_prev_fake_load_util(frame_info);
	else
		frame_info->prev_frame_load_util =
			calc_prev_frame_load_util(frame_info);
}

static inline bool check_frame_util_invalid(const struct frame_info *frame_info,
	u64 timeline)
{
	return ((frame_info_rtg(frame_info)->util_invalid_interval <= timeline) &&
		(frame_info_rtg_load(frame_info)->curr_window_exec *
		FRAME_UTIL_INVALID_FACTOR <= timeline));
}

static void set_frame_start(struct frame_info *frame_info)
{
	if (likely(frame_info->status == FRAME_START)) {
		/*
		 * START -=> START -=> ......
		 * FRMAE_START is
		 *	the end of last frame
		 *	the start of the current frame
		 */
		update_frame_prev_load(frame_info, false);
	} else if ((frame_info->status == FRAME_END) ||
		(frame_info->status == FRAME_INVALID)) {
		/* START -=> END -=> [START]
		 *  FRAME_START is
		 *	only the start of current frame
		 * we shoudn't tracking the last rtg-window
		 * [FRAME_END, FRAME_START]
		 * it's not an available frame window
		 */
		update_frame_prev_load(frame_info, true);
		frame_info->status = FRAME_START;
	}

	trace_rtg_frame_sched("FRAME_STATUS", frame_info->status);
	trace_rtg_frame_sched("frame_last_task_time",
		frame_info->prev_frame_exec);
	trace_rtg_frame_sched("frame_last_time", frame_info->prev_frame_time);
	trace_rtg_frame_sched("frame_last_load", frame_info->prev_frame_load);
	trace_rtg_frame_sched("frame_last_load_util",
		frame_info->prev_frame_load_util);

	/* new_frame_start */
	frame_info->frame_vload = 0;
	if (!frame_info->margin_imme) {
		unsigned long util = max_t(unsigned long,
			frame_info->prev_frame_load_util,
			frame_info->frame_min_util);
		frame_info->frame_util = min_t(unsigned long,
			frame_info->frame_max_util, util);
	} else {
		frame_info->frame_vload = calc_frame_vload(frame_info, 0);
		frame_info->frame_util = calc_frame_util(frame_info, false);
	}

	trace_rtg_frame_sched("frame_vload", frame_info->frame_vload);
}

static void do_frame_end(struct frame_info *frame_info)
{
	frame_info->status = FRAME_END;
	trace_rtg_frame_sched("FRAME_STATUS", frame_info->status);

	/* last frame load tracking */
	update_frame_prev_load(frame_info, false);

	/* reset frame_info */
	frame_info->frame_vload = 0;

	frame_info->frame_util = min_t(unsigned long,
		frame_info->frame_max_util,
		frame_info->prev_fake_load_util);

	trace_rtg_frame_sched("frame_last_task_time",
		frame_info->prev_frame_exec);
	trace_rtg_frame_sched("frame_last_time", frame_info->prev_frame_time);
	trace_rtg_frame_sched("frame_last_load", frame_info->prev_frame_load);
	trace_rtg_frame_sched("frame_last_load_util",
		frame_info->prev_frame_load_util);
	trace_rtg_frame_sched("frame_vload", frame_info->frame_vload);
}

static void set_frame_end(struct frame_info *frame_info, bool fake)
{
	unsigned long prev_util;

	frame_info->status = FRAME_END;
	trace_rtg_frame_sched("FRAME_STATUS", frame_info->status);

	/* last frame load tracking */
	update_frame_prev_load(frame_info, fake);

	/* reset frame_info */
	frame_info->frame_vload = 0;

	if (fake)
		prev_util = frame_info->prev_fake_load_util;
	else
		prev_util = frame_info->prev_frame_load_util;
	frame_info->frame_min_util = 0;
	frame_info->frame_util = min_t(unsigned long,
		frame_info->frame_max_util,
		prev_util);

	trace_rtg_frame_sched("frame_last_task_time",
		frame_info->prev_frame_exec);
	trace_rtg_frame_sched("frame_last_time", frame_info->prev_frame_time);
	trace_rtg_frame_sched("frame_last_load", frame_info->prev_frame_load);
	trace_rtg_frame_sched("frame_last_load_util",
		frame_info->prev_frame_load_util);
	trace_rtg_frame_sched("frame_util", frame_info->frame_util);
	trace_rtg_frame_sched("frame_vload", frame_info->frame_vload);
}

static int update_frame_timestamp(unsigned long status,
	struct frame_info *frame_info, const struct related_thread_group *grp)
{
	/* SCHED_FRAME timestamp */
	switch (status) {
	case FRAME_START:
		/* collect frame_info when frame_end timestamp comming */
		set_frame_start(frame_info);
		break;
	case FRAME_END:
		/* FRAME_END should only set and update freq once */
		if (unlikely(frame_info->status == FRAME_END))
			return 0;
		do_frame_end(frame_info);
		break;
	default:
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid timestamp(status)\n",
			__func__);
		return -EINVAL;
	}

	frame_boost(frame_info);
	trace_rtg_frame_sched("frame_util", frame_info->frame_util);
	/* update cpufreq force when frame_stop */
	sched_set_group_normalized_util(DEFAULT_RT_FRAME_ID,
		frame_info->frame_util, FRAME_FORCE_UPDATE);

	if (grp->preferred_cluster)
		trace_rtg_frame_sched("preferred_cluster",
			grp->preferred_cluster->id);

	return 0;
}

int set_frame_status(unsigned long status)
{
	struct related_thread_group *grp = NULL;
	struct frame_info *frame_info = NULL;

	if (atomic_read(&g_start_frame) == 0)
		return -EINVAL;

	if (!(status & FRAME_SETTIME) ||
		(status == (unsigned long)FRAME_SETTIME_PARAM)) {
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid timetsamp(status)\n",
			__func__);
		return -EINVAL;
	}

	grp = frame_rtg();
	if (unlikely(!grp))
		return -EINVAL;

	frame_info = rtg_frame_info_inner(grp);
	if (unlikely(!frame_info))
		return -EINVAL;

	if (status & FRAME_TIMESTAMP_SKIP_START) {
		frame_info->timestamp_skipped = true;
		status &= ~FRAME_TIMESTAMP_SKIP_START;
	} else if (status & FRAME_TIMESTAMP_SKIP_END) {
		frame_info->timestamp_skipped = false;
		status &= ~FRAME_TIMESTAMP_SKIP_END;
	} else if (frame_info->timestamp_skipped) {
		/*
		 * skip the following timestamp until
		 * FRAME_TIMESTAMP_SKIPPED reset
		 */
		return 0;
	}
	trace_rtg_frame_sched("FRAME_TIMESTAMP_SKIPPED",
		frame_info->timestamp_skipped);
	trace_rtg_frame_sched("FRAME_MAX_UTIL", frame_info->frame_max_util);

	if (status & FRAME_USE_MARGIN_IMME) {
		frame_info->margin_imme = true;
		status &= ~FRAME_USE_MARGIN_IMME;
	} else {
		frame_info->margin_imme = false;
	}
	trace_rtg_frame_sched("FRAME_MARGIN_IMME", frame_info->margin_imme);
	trace_rtg_frame_sched("FRAME_TIMESTAMP", status);

	return update_frame_timestamp(status, frame_info, grp);
}
EXPORT_SYMBOL_GPL(set_frame_status);

static void update_frame_vload_statistic(struct frame_info *frame_info,
	u64 timeline)
{
	u64 curr_load = 0;

	frame_info->frame_vload = calc_frame_vload(frame_info, timeline);
	if (frame_info->max_vload_time >= frame_info->qos_frame_time) {
		curr_load = calc_frame_load(frame_info);
		if (curr_load <= (frame_info->frame_vload >> 1))
			frame_info->frame_vload = curr_load;
	}
}

static int update_frame_info_tick_inner(struct frame_info *frame_info,
	u64 timeline)
{
	switch (frame_info->status) {
	case FRAME_INVALID:
	case FRAME_END:
		if (timeline >= frame_info->qos_frame_time) {
			/*
			 * fake FRAME_END here to rollover frame_window.
			 */
			sched_set_group_window_rollover(DEFAULT_RT_FRAME_ID);
			set_frame_end(frame_info, true);
		} else {
			frame_info->frame_vload = calc_frame_exec(frame_info);
			frame_info->frame_util =
				calc_frame_util(frame_info, true);
		}

		/* when not in boost, start tick timer */
		break;
	case FRAME_START:
		/* check frame_util invalid */
		if (!check_frame_util_invalid(frame_info, timeline)) {
			/* frame_vload statistic */
			update_frame_vload_statistic(frame_info, timeline);
			/* frame_util statistic */
			frame_info->frame_util =
				calc_frame_util(frame_info, false);
		} else {
			frame_info->status = FRAME_INVALID;
			trace_rtg_frame_sched("FRAME_STATUS",
				frame_info->status);
			trace_rtg_frame_sched("frame_status",
				frame_info->status);

			/*
			 * trigger FRAME_END to rollover frame_window,
			 * we treat FRAME_INVALID as FRAME_END.
			 */
			sched_set_group_window_rollover(DEFAULT_RT_FRAME_ID);
			set_frame_end(frame_info, false);
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

/*
 * update CPUFREQ and PLACEMENT when frame task running (in tick) and migration
 */
static void update_frame_info_tick(struct related_thread_group *grp)
{
	u64 window_start;
	u64 wallclock;
	u64 timeline;
	struct frame_info *frame_info = NULL;

	if (atomic_read(&g_start_frame) == 0)
		return;

	rcu_read_lock();
	frame_info = rtg_frame_info_inner(grp);
	window_start = grp->window_start;
	rcu_read_unlock();
	if (unlikely(!frame_info))
		return;

	trace_rtg_frame_sched("frame_status", frame_info->status);

	wallclock = ktime_get_ns();
	timeline = wallclock - window_start;

	trace_rtg_frame_sched("update_curr_pid", current->pid);
	trace_rtg_frame_sched("frame_timeline", timeline / NSEC_PER_MSEC);

	if (update_frame_info_tick_inner(frame_info, timeline) == -EINVAL)
		return;

	frame_boost(frame_info);
	trace_rtg_frame_sched("frame_vload", frame_info->frame_vload);
	trace_rtg_frame_sched("frame_util", frame_info->frame_util);
	sched_set_group_normalized_util(DEFAULT_RT_FRAME_ID,
		frame_info->frame_util, FRAME_NORMAL_UPDATE);

	if (grp->preferred_cluster)
		trace_rtg_frame_sched("preferred_cluster",
			grp->preferred_cluster->id);
}

int update_frame_isolation(void)
{
	struct related_thread_group *grp = current->grp;

	if (unlikely(!grp || !grp->preferred_cluster))
		return -EINVAL;

	if (unlikely((grp->id != DEFAULT_RT_FRAME_ID) &&
		(grp->id != DEFAULT_AUX_ID)))
		return -EINVAL;

	return !(grp->preferred_cluster == max_cap_cluster());
}

int set_frame_min_util(int min_util, bool is_boost)
{
	struct frame_info *frame_info = NULL;

	if (unlikely((min_util < 0) || (min_util > SCHED_CAPACITY_SCALE))) {
		pr_err("[%s] [IPROVISION-FRAME_INFO] invalid min_util value",
			__func__);
		return -EINVAL;
	}

	frame_info = rtg_frame_info();
	if (unlikely(!frame_info))
		return -EINVAL;

	if (is_boost) {
		frame_info->frame_boost_min_util = min_util;
		trace_rtg_frame_sched("FRAME_BOOST_MIN_UTIL", min_util);
	} else {
		frame_info->frame_min_util = min_util;

		frame_info->frame_util = calc_frame_util(frame_info, false);
		trace_rtg_frame_sched("frame_util", frame_info->frame_util);
		sched_set_group_normalized_util(DEFAULT_RT_FRAME_ID,
			frame_info->frame_util, FRAME_FORCE_UPDATE);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(set_frame_min_util);

int set_frame_min_util_and_margin(int min_util, int margin)
{
	int ret = set_frame_margin(margin);
	if (ret != 0)
		return ret;

	ret = set_frame_min_util(min_util, false);
	return ret;
}
EXPORT_SYMBOL_GPL(set_frame_min_util_and_margin);

const struct rtg_class frame_rtg_class = {
	.sched_update_rtg_tick = update_frame_info_tick,
};

static int __init init_frame_info(void)
{
	struct related_thread_group *grp = NULL;
	struct frame_info *frame_info = NULL;
	unsigned long flags;

	g_pid_task = NULL;
	g_tid_task = NULL;

	frame_info = &g_frame_info;
	memset(frame_info, 0, sizeof(*frame_info));

	frame_info->qos_frame = DEFAULT_FRAME_RATE;
	frame_info->qos_frame_time = NSEC_PER_SEC / frame_info->qos_frame;
	frame_info->vload_margin = DEFAULT_VLOAD_MARGIN;
	frame_info->max_vload_time =
		frame_info->qos_frame_time / NSEC_PER_MSEC +
		frame_info->vload_margin;
	frame_info->frame_max_util = FRAME_DEFAULT_MAX_UTIL;
	frame_info->frame_min_util = 0;
	frame_info->margin_imme = false;
	frame_info->timestamp_skipped = false;
	frame_info->status = FRAME_END;

	grp = frame_rtg();

	raw_spin_lock_irqsave(&grp->lock, flags);
	grp->private_data = frame_info;
	grp->rtg_class = &frame_rtg_class;
	raw_spin_unlock_irqrestore(&grp->lock, flags);

	frame_info->rtg = grp;

	return 0;
}
late_initcall(init_frame_info);
/*lint -restore*/
