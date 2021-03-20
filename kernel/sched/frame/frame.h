/*
 * frame.h
 *
 * Frame declaration
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

#ifndef FRAME_EXTERN_H
#define FRAME_EXTERN_H

#ifdef CONFIG_FRAME_RTG
bool is_frame_task(struct task_struct *task);
int set_frame_rate(int rate);
int set_frame_margin(int margin);
int set_frame_status(unsigned long status);
int set_frame_max_util(int max_util);
void set_frame_sched_state(bool enable);
int set_frame_timestamp(unsigned long timestamp);
int set_frame_min_util(int min_util, bool is_boost);
int set_frame_min_util_and_margin(int min_util, int margin);
void update_frame_thread(int pid, int tid);
int update_frame_isolation(void);
#else
static inline bool is_frame_task(struct task_struct *task)
{
	return false;
}

static inline int set_frame_rate(int rate)
{
	return 0;
}

static inline int set_frame_margin(int margin)
{
	return 0;
}

static inline int set_frame_status(unsigned long status)
{
	return 0;
}

static inline int set_frame_max_util(int max_util)
{
	return 0;
}

static inline void set_frame_sched_state(bool enable) {}

static inline int set_frame_timestamp(unsigned long timestamp)
{
	return 0;
}

static inline int set_frame_min_util(int min_util, bool is_boost)
{
	return 0;
}

static inline int set_frame_min_util_and_margin(int min_util, int margin)
{
	return 0;
}

static inline void update_frame_thread(int pid, int tid) {}

static inline int update_frame_isolation(void)
{
	return 1;
}
#endif

#endif // FRAME_EXTERN_H
