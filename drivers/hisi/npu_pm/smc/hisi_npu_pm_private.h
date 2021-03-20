/*
 * hisi_npu_pm_debug_smc.c
 *
 * hisi npu debugfs
 *
 * Copyright (C) 2017-2020 Huawei Technologies Co., Ltd.
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
#ifndef __HISI_NPU_PM_PRIVATE_H__
#define __HISI_NPU_PM_PRIVATE_H__

#define KHZ	1000U
#define MAX_MODULE_NAME_LEN	10

struct hisi_npu_dvfs_data {
	bool dvfs_enable;
	unsigned int current_freq;
	struct thermal_zone_device *tz_dev;
	int *tzone;
	int tz_num;
	int tz_cur;
	unsigned int test;
	int temp_test;
	unsigned long max_dvfs_time;
};

enum {
	NPU_LOW_TEMP_DEBUG,
	NPU_LOW_TEMP_STATE
};

struct npu_pm_device {
	struct mutex mutex;
	struct device *dev;
	struct devfreq *devfreq;
	unsigned long last_freq;
	unsigned long target_freq;
	int pm_qos_class;
	bool power_on;
	unsigned int power_on_count;
	unsigned int power_off_count;
	struct hisi_npu_dvfs_data *dvfs_data;
#ifdef CONFIG_DEVFREQ_THERMAL
	struct thermal_cooling_device *devfreq_cooling;
	unsigned long cur_state;
#endif
	unsigned long max_pwron_time;
	unsigned long max_pwroff_time;
};

#ifdef CONFIG_HISI_NPUFREQ
int hisi_npu_devfreq_init(struct npu_pm_device *pmdev);
void hisi_npu_devfreq_term(struct npu_pm_device *pmdev);
int hisi_npu_devfreq_suspend(struct devfreq *devfreq);
int hisi_npu_devfreq_resume(struct devfreq *devfreq);
int hisi_npu_dvfs_hal(unsigned long target_freq);
int hisi_npu_dvfs_init(struct npu_pm_device *pmdev);
int hisi_npu_pm_pwr_on(unsigned long target_freq);
int hisi_npu_pm_pwr_off(void);
#else
static inline int hisi_npu_devfreq_init(struct npu_pm_device *pmdev)
{
	(void)pmdev;

	return 0;
}

static inline void hisi_npu_devfreq_term(struct npu_pm_device *pmdev)
{
	(void)pmdev;
}

static inline int hisi_npu_devfreq_suspend(struct devfreq *devfreq)
{
	(void)devfreq;

	return 0;
}

static inline int hisi_npu_devfreq_resume(struct devfreq *devfreq)
{
	(void)devfreq;

	return 0;
}

static inline int hisi_npu_dvfs_hal(unsigned long target_freq)
{
	(void)target_freq;

	return 0;
}

static inline int hisi_npu_dvfs_init(struct npu_pm_device *pmdev)
{
	(void)pmdev;

	return 0;
}

static inline int hisi_npu_pm_pwr_on(unsigned long target_freq)
{
	(void)target_freq;

	return 0;
}

static inline int hisi_npu_pm_pwr_off(void)
{
	return 0;
}
#endif

#ifdef CONFIG_HISI_NPU_PM_DEBUG
void hisi_npu_pm_debugfs_init(struct npu_pm_device *pmdev);
void hisi_npu_pm_debugfs_exit(void);
#else
static inline void hisi_npu_pm_debugfs_init(struct npu_pm_device *pmdev)
{
	(void)pmdev;
}

static inline void hisi_npu_pm_debugfs_exit(void)
{
}
#endif

#endif /* __HISI_NPU_PM_PRIVATE_H__ */
