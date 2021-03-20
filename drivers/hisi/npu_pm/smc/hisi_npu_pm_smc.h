/*
 * hisi_npu_pm_smc.h
 *
 * hisilicon npu pm smc interface
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
#ifndef __HISI_NPU_PM_SMC_H__
#define __HISI_NPU_PM_SMC_H__

#ifdef CONFIG_HISI_NPUFREQ
int hisi_npu_pm_pwron_smc_request(unsigned long freq, int temp);
int hisi_npu_pm_pwroff_smc_request(int temp);
int hisi_npu_pm_dvfs_request_smc_request(unsigned long tar_freq, int temp);
int hisi_npu_pm_set_voltage_smc_request(unsigned int voltage);
int hisi_npu_pm_get_voltage_smc_request(void);
void hisi_npu_pm_dbg_smc_request(unsigned int enable);
#else
static inline int hisi_npu_pm_pwron_request(unsigned long freq, int temp)
{
	(void)freq;
	(void)temp;

	return 0;
}

static inline int hisi_npu_pm_pwroff_smc_request(int temp)
{
	(void)temp;

	return 0;
}

static inline int hisi_npu_pm_dvfs_request_smc_request(unsigned long tar_freq,
						       int temp)
{
	(void)tar_freq;
	(void)temp;

	return 0;
}

static inline int hisi_npu_pm_set_voltage_smc_request(unsigned int voltage)
{
	(void)voltage;

	return 0;
}

static inline int hisi_npu_pm_get_voltage_smc_request(void)
{
	return 0;
}
#endif

#endif /* __HISI_NPU_PM_SMC_H__ */
