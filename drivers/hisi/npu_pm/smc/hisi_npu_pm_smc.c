/*
 * hisi_npu_pm_smc.c
 *
 * hisilicon npu power management
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
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/devfreq.h>
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/pm.h>
#include <linux/pm_opp.h>
#include <linux/pm_qos.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <hisi_npu_pm.h>
#include "hisi_npu_pm_private.h"

#define HISI_NPU_PM_SMC_FID	0xc500dea0U /* smc service id */

enum {
	HISI_NPU_PM_SMC_PWRON_REQ,
	HISI_NPU_PM_SMC_PWROFF_REQ,
	HISI_NPU_PM_SMC_DVFS_REQ,
	HISI_NPU_PM_SMC_DBG_REQ,
	HISI_NPU_PM_SMC_SET_VOLT_REQ,
	HISI_NPU_PM_SMC_GET_VOLT_REQ,
};

struct npu_pm_device *g_npu_pm_dev;
DEFINE_MUTEX(power_mutex);

/*
 * brief smc caller for npu frequency scale
 *
 * param _arg0 param0 pass to atf
 * param _arg1 param1 pass to atf
 * param _arg2 param2 pass to atf
 * return atf execute result
 */
noinline int hisi_npu_pm_smc_request(u64 _type, u64 _arg0, u64 _arg1, u64 _arg2)
{
	register u64 fid asm("x0") = HISI_NPU_PM_SMC_FID;
	register u64 type asm("x1") = _type;
	register u64 arg0 asm("x2") = _arg0;
	register u64 arg1 asm("x3") = _arg1;
	register u64 arg2 asm("x4") = _arg2;

	asm volatile(
			__asmeq("%0", "x0")
			__asmeq("%1", "x1")
			__asmeq("%2", "x2")
			__asmeq("%3", "x3")
			__asmeq("%4", "x4")
			"smc    #0\n"
			: "+r" (fid)
			: "r" (type), "r" (arg0), "r" (arg1), "r" (arg2));

	return (int)fid;
}

int hisi_npu_pm_pwron_smc_request(unsigned long freq, int temp)
{
	return hisi_npu_pm_smc_request(HISI_NPU_PM_SMC_PWRON_REQ,
				       freq,
				       (u64)temp,
				       0);
}

int hisi_npu_pm_pwroff_smc_request(int temp)
{
	return hisi_npu_pm_smc_request(HISI_NPU_PM_SMC_PWROFF_REQ,
				       (u64)temp,
				       0,
				       0);
}

int hisi_npu_pm_dvfs_request_smc_request(u64 tar_freq, int temp)
{
	return hisi_npu_pm_smc_request(HISI_NPU_PM_SMC_DVFS_REQ,
				       tar_freq,
				       (u64)temp,
				       0);
}

void hisi_npu_pm_dbg_smc_request(unsigned int enable)
{
	(void)enable;
}

int hisi_npu_pm_set_voltage_smc_request(unsigned int voltage)
{
	(void)voltage;

	return -ENODEV;
}

int hisi_npu_pm_get_voltage_smc_request(void)
{
	return -ENODEV;
}

static inline void hisi_npu_devfreq_lock(struct devfreq *df)
{
	if (!IS_ERR_OR_NULL(df))
		mutex_lock(&df->lock);
}

static inline void hisi_npu_devfreq_unlock(struct devfreq *df)
{
	if (!IS_ERR_OR_NULL(df))
		mutex_unlock(&df->lock);
}

int hisi_npu_power_on(void)
{
	struct devfreq *devfreq = NULL;
	ktime_t in_ktime;
	unsigned long delta_time;
	unsigned long last_freq;
	int ret;

	if (IS_ERR_OR_NULL(g_npu_pm_dev)) {
		pr_err("[npupm] npu pm device not exist\n");
		return -ENODEV;
	}
	devfreq = g_npu_pm_dev->devfreq;
	if (IS_ERR_OR_NULL(devfreq)) {
		pr_err("%s npu pm devfreq devices not exist\n", __func__);
		return -ENODEV;
	}
	g_npu_pm_dev->power_on_count++;

	mutex_lock(&power_mutex);

	in_ktime = ktime_get();

	if (g_npu_pm_dev->power_on) {
		mutex_unlock(&power_mutex);
		return 0;
	}

	last_freq = devfreq->previous_freq;
	g_npu_pm_dev->last_freq = last_freq;
	g_npu_pm_dev->target_freq = last_freq;

	hisi_npu_devfreq_lock(devfreq);

	ret = hisi_npu_pm_pwr_on(last_freq);
	if (ret != 0) {
		pr_err("[npupm] Failed to enable\n");
		goto err_power_on;
	}

	g_npu_pm_dev->power_on = true;

	if (!IS_ERR_OR_NULL(g_npu_pm_dev->dvfs_data))
		g_npu_pm_dev->dvfs_data->dvfs_enable = true;

	hisi_npu_devfreq_unlock(devfreq);

	/* must out of devfreq lock */
	ret = hisi_npu_devfreq_resume(devfreq);
	if (ret != 0)
		pr_err("Resume device failed, ret=%d!\n", ret);

	delta_time = ktime_to_ns(ktime_sub(ktime_get(), in_ktime));
	if (delta_time > g_npu_pm_dev->max_pwron_time)
		g_npu_pm_dev->max_pwron_time = delta_time;

	mutex_unlock(&power_mutex);

	return 0;

err_power_on:
	hisi_npu_devfreq_unlock(devfreq);
	mutex_unlock(&power_mutex);

	return ret;
}

int hisi_npu_power_off(void)
{
	struct devfreq *devfreq = NULL;
	ktime_t in_ktime;
	unsigned long delta_time;
	int ret;

	if (IS_ERR_OR_NULL(g_npu_pm_dev)) {
		pr_err("[npupm] npu pm device not exist\n");
		return -ENODEV;
	}
	devfreq = g_npu_pm_dev->devfreq;
	if (IS_ERR_OR_NULL(devfreq)) {
		pr_err("%s npu pm devfreq devices not exist\n", __func__);
		return -ENODEV;
	}
	g_npu_pm_dev->power_off_count++;

	mutex_lock(&power_mutex);

	if (!g_npu_pm_dev->power_on) {
		mutex_unlock(&power_mutex);
		return 0;
	}

	in_ktime = ktime_get();

	/* out of devfreq lock */
	ret = hisi_npu_devfreq_suspend(devfreq);
	if (ret != 0)
		pr_err("Suspend device failed, ret=%d!\n", ret);

	hisi_npu_devfreq_lock(devfreq);

	if (!IS_ERR_OR_NULL(g_npu_pm_dev->dvfs_data))
		g_npu_pm_dev->dvfs_data->dvfs_enable = false;

	ret = hisi_npu_pm_pwr_off();
	if (ret != 0) {
		pr_err("[npupm] Failed to disable\n");
		goto err_power_off;
	}

	g_npu_pm_dev->power_on = false;

err_power_off:
	hisi_npu_devfreq_unlock(devfreq);

	delta_time = ktime_to_ns(ktime_sub(ktime_get(), in_ktime));
	if (delta_time > g_npu_pm_dev->max_pwroff_time)
		g_npu_pm_dev->max_pwroff_time = delta_time;

	mutex_unlock(&power_mutex);

	return ret;
}

static int hisi_npu_pm_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	unsigned int init_freq = 0;
	int ret;

	g_npu_pm_dev = devm_kzalloc(dev, sizeof(*g_npu_pm_dev), GFP_KERNEL);
	if (g_npu_pm_dev == NULL) {
		ret = -ENOMEM;
		goto err_out;
	}

	mutex_init(&g_npu_pm_dev->mutex);

	ret = of_property_read_u32(dev->of_node, "initial_freq", &init_freq);
	if (ret != 0) {
		dev_err(dev, "parse npu initial frequency fail%d\n", ret);
		ret = -EINVAL;
		goto err_out;
	}
	g_npu_pm_dev->last_freq = (unsigned long)init_freq * KHZ;
	g_npu_pm_dev->target_freq = g_npu_pm_dev->last_freq;

	g_npu_pm_dev->dev = dev;
	g_npu_pm_dev->pm_qos_class = PM_QOS_HISI_NPU_FREQ_DNLIMIT;
	g_npu_pm_dev->power_on = false;

	ret = hisi_npu_dvfs_init(g_npu_pm_dev);
	if (ret != 0) {
		dev_err(dev, "npu dvfs init fail%d\n", ret);
		ret = -EINVAL;
		goto err_out;
	}

	ret = hisi_npu_devfreq_init(g_npu_pm_dev);
	if (ret != 0)
		dev_err(dev, "npu devfreq init fail%d\n", ret);

	hisi_npu_pm_debugfs_init(g_npu_pm_dev);

err_out:
	of_node_put(dev->of_node);

	return ret;
}

#ifdef CONFIG_OF
static const struct of_device_id hisi_npu_pm_of_match[] = {
	{.compatible = "hisi,npu-pm",},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_npu_pm_of_match);
#endif

static struct platform_driver hisi_npu_pm_driver = {
	.probe  = hisi_npu_pm_probe,
	.driver = {
			.name = "hisi-npu-pm",
			.owner = THIS_MODULE,
			.of_match_table = of_match_ptr(hisi_npu_pm_of_match),
		},
};

static int __init hisi_npu_pm_init(void)
{
	return platform_driver_register(&hisi_npu_pm_driver);
}

device_initcall(hisi_npu_pm_init);

static void __exit hisi_npu_pm_exit(void)
{
	hisi_npu_pm_debugfs_exit();

	hisi_npu_devfreq_term(g_npu_pm_dev);

	platform_driver_unregister(&hisi_npu_pm_driver);
}

module_exit(hisi_npu_pm_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("gpu governor of devfreq framework");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
