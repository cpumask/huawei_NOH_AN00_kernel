/*
 * trans_rtg.c
 *
 * trans rtg thread
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

#include "include/trans_rtg.h"

#include <linux/atomic.h>
#include <linux/printk.h>
#include <../kernel/sched/sched.h>

#include <linux/sched/frame.h>
#include "include/set_rtg.h"
#include "include/aux.h"

static int g_trans_depth = DEFAULT_TRANS_DEPTH;
static int g_max_thread_num = DEFAULT_MAX_THREADS;
static atomic_t g_rtg_thread_num = ATOMIC_INIT(0);

void set_trans_config(int depth, int max_threads)
{
	if ((depth != INVALID_VALUE) && (depth >= 0))
		g_trans_depth = depth;
	if ((max_threads != INVALID_VALUE) && (max_threads >= 0))
		g_max_thread_num = max_threads;
}

static bool is_depth_valid(struct task_struct *from)
{
	if (g_trans_depth != DEFAULT_TRANS_DEPTH && g_trans_depth <= 0)
		return false;

	if ((g_trans_depth > 0) && (from->rtg_depth != STATIC_RTG_DEPTH) &&
		(from->rtg_depth >= g_trans_depth))
		return false;

	return true;
}

void add_trans_thread(struct task_struct *target, struct task_struct *from)
{
	int ret;
	unsigned int grpid;
	int prio;

	if ((target == NULL) || (from == NULL))
		return;

	if (!is_depth_valid(from))
		return;

	grpid = sched_get_group_id(target);
	if ((grpid == DEFAULT_RT_FRAME_ID) || (grpid == DEFAULT_AUX_ID))
		return;

	grpid = sched_get_group_id(from);
	if (grpid == DEFAULT_RT_FRAME_ID) {
		if (g_max_thread_num != DEFAULT_MAX_THREADS &&
			atomic_read(&g_rtg_thread_num) >= g_max_thread_num)
			return;
		prio = DEFAULT_RT_PRIO;
	} else if (grpid == DEFAULT_AUX_ID) {
		prio = (from->prio < MAX_RT_PRIO ? from->prio : NOT_RT_PRIO);
	} else {
		return;
	}

	get_task_struct(target);
	ret = set_rtg_sched(target, true, grpid, prio);
	if (ret < 0) {
		put_task_struct(target);
		return;
	}
	if (from->rtg_depth == STATIC_RTG_DEPTH)
		target->rtg_depth = 1;
	else
		target->rtg_depth = from->rtg_depth + 1;

	if (grpid == DEFAULT_RT_FRAME_ID)
		atomic_inc(&g_rtg_thread_num);
	pr_debug("[AWARE_RTG] %s pid=%d, depth=%d", __func__,
			target->pid, target->rtg_depth);
	put_task_struct(target);
}

void remove_trans_thread(struct task_struct *target)
{
	int ret;
	unsigned int grpid;

	if (target == NULL)
		return;

	grpid = sched_get_group_id(target);
	if ((grpid != DEFAULT_RT_FRAME_ID) && (grpid != DEFAULT_AUX_ID))
		return;

	get_task_struct(target);
	if (target->rtg_depth == STATIC_RTG_DEPTH) {
		put_task_struct(target);
		return;
	}

	ret = set_rtg_sched(target, false, DEFAULT_RT_FRAME_ID,
		DEFAULT_RT_PRIO);
	if (ret < 0) {
		put_task_struct(target);
		return;
	}
	target->rtg_depth = 0;
	if ((grpid == DEFAULT_RT_FRAME_ID) &&
		(atomic_read(&g_rtg_thread_num) > 0))
		atomic_dec(&g_rtg_thread_num);
	pr_debug("[AWARE_RTG] %s pid=%d", __func__, target->pid);
	put_task_struct(target);
}
