/*
 * hisi_core_ctrl.h
 *
 * head file for core_ctrl.
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

#ifndef __HISI_CORE_CTL_H
#define __HISI_CORE_CTL_H
#include <linux/cpumask.h>

#ifdef CONFIG_HISI_CORE_CTRL
void core_ctl_check(void);
void core_ctl_set_boost(unsigned int timeout);
void core_ctl_spread_affinity(cpumask_t *allowed_mask);
#else
static inline void core_ctl_check(void) {}
static inline void core_ctl_set_boost(unsigned int timeout) {}
static inline void core_ctl_spread_affinity(cpumask_t *allowed_mask) {}
#endif
#endif
