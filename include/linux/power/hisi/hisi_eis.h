/*
 * hisi_eis.h
 *
 * hisi eis driver.
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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

#ifndef HISI_EIS_H
#define HISI_EIS_H

#include <linux/notifier.h>

#define EIS_FREQ_CYCLE_S            (20 * 60)
#define LEN_T_FREQ                  16
#define SAME_CYC_MAX_TRIAL          3

enum hisi_eis_event {
	EVT_EIS_BEGIN = 0,
	EVT_EIS_FREQ_UPDATE = EVT_EIS_BEGIN,
	EVT_EIS_FREQ_TIME_UPDATE,
	EVT_EIS_FREQ_WAVE_UPDATE,
	EVT_EIS_END
};

struct eis_freq_info {
	bool sample_valid;
	int temp;
	int cur;
	int soc;
	int cycle;
	int freq;
	long long real;
	long long imag;
};

struct eis_freq_infos {
	int num_of_info;
	struct eis_freq_info freq_infos[LEN_T_FREQ];
};

#ifdef CONFIG_HISI_EIS
void hisi_eis_freq_clear_flash_data(void);
int get_freq_work_flag(void);

int hisi_eis_register_blocking_notifier(struct notifier_block *nb);
int hisi_eis_unregister_blocking_notifier(struct notifier_block *nb);
int hisi_call_eis_blocking_notifiers(int val, void *v);

#else
static inline void hisi_eis_freq_clear_flash_data(void){}
static inline int get_freq_work_flag(void) { return 0; }

static inline int hisi_eis_register_blocking_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int hisi_eis_unregister_blocking_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int hisi_call_eis_blocking_notifiers(int val, void *v)
{
	return 0;
}

#endif /* CONFIG_HISI_EIS */

#endif /* HISI_EIS_H */
