/*
 * hisi_cpufreq_dt.h
 *
 * hisi cpufreq device tree head file
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

#ifndef _HISI_CPUFREQ_DT_H
#define _HISI_CPUFREQ_DT_H

#include <linux/cpufreq.h>
#include <linux/version.h>
#ifdef CONFIG_HISI_HW_VOTE_CPU_FREQ
#include <linux/hisi/hisi_hw_vote.h>
#endif
#ifdef CONFIG_HISI_FREQ_STATS_COUNTING_IDLE
#include <linux/cpumask.h>
#endif


#ifdef CONFIG_HISI_CPUFREQ_DT
struct opp_table *hisi_cpufreq_set_supported_hw(struct device *cpu_dev);
void hisi_cpufreq_put_supported_hw(struct opp_table *opp_table);

void hisi_cpufreq_get_suspend_freq(struct cpufreq_policy *policy);
int hisi_cpufreq_init(void);

#ifdef CONFIG_HISI_HW_VOTE_CPU_FREQ

#ifdef CONFIG_HISI_L2_DYNAMIC_RETENTION
void l2_dynamic_retention_ctrl(struct cpufreq_policy *policy, unsigned int freq);
#endif

struct hvdev *hisi_cpufreq_hv_init(struct device *cpu_dev);
void hisi_cpufreq_hv_exit(struct hvdev *cpu_hvdev, unsigned int cpu);
int hisi_cpufreq_set(struct hvdev *cpu_hvdev, unsigned int freq);
unsigned int hisi_cpufreq_get(unsigned int cpu);
void hisi_cpufreq_policy_cur_init(struct hvdev *cpu_hvdev,
				  struct cpufreq_policy *policy);
#endif

#endif

#ifdef CONFIG_HISI_FREQ_STATS_COUNTING_IDLE
void time_in_state_update_freq(struct cpumask *cpus, unsigned int new_freq_index);
void time_in_state_update_idle(int cpu, unsigned int new_idle_index);
ssize_t hisi_time_in_state_show(int cpu, char *buf);
int hisi_time_in_freq_get(int cpu, u64 *freqs, int freqs_len);
#endif

#endif /* _HISI_CPUFREQ_DT_H */
