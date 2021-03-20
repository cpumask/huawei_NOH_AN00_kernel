/*
 * huawei_hung_task.h
 *
 * Detect Hung Task header file
 *
 * Copyright (c) 2017-2019 Huawei Technologies Co., Ltd.
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

#ifndef _HUAWEI_HUNG_TASK_H_
#define _HUAWEI_HUNG_TASK_H_

#ifndef HEARTBEAT_TIME
#define HEARTBEAT_TIME 3
#endif

extern unsigned long __read_mostly sysctl_hung_task_timeout_secs;
extern unsigned int __read_mostly sysctl_hung_task_panic;

void check_hung_tasks_proposal(unsigned long timeout);
void fetch_hung_task_panic(int new_did_panic);
void fetch_task_timeout_secs(unsigned long new_sysctl_hung_task_timeout_secs);
int create_sysfs_hungtask(void);
void hwhungtask_show_state_filter(unsigned long state_filter);
#ifdef CONFIG_HW_ZEROHUNG
int hwhungtask_get_backlight(void);
#endif

#endif
