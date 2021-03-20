/*
 * l3cache_extension.c
 *
 * l3cache extension
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

#include <linux/of_platform.h>
#include <linux/cpu_pm.h>
#include <linux/ktime.h>
#include <securec.h>
#include "l3cache_common.h"
#include "hisi_lb.h"
#define CREATE_TRACE_POINTS
#include <trace/events/l3cache_extension.h>

#define CACHE_EVICT_CONTROL		14

enum ai_type {
	AI_EXIT = 0,
	AI_ENTRY,
};


struct l3extension {
	struct platform_device *pdev;
	bool fcm_idle;
	bool enable_flag;
	bool mode_flag;
	/* to protect operation of fcm idle race */
	spinlock_t data_lock;
	/* to protect extension enable flag */
	spinlock_t enable_lock;
	struct notifier_block idle_nb;
};


noinline u64 atfd_l3extension_smc(u64 _function_id,
				  u64 _arg0,
				  u64 _arg1,
				  u64 _arg2)
{
	register u64 function_id asm("x0") = _function_id;
	register u64 arg0 asm("x1") = _arg0;
	register u64 arg1 asm("x2") = _arg1;
	register u64 arg2 asm("x3") = _arg2;

	asm volatile (
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")
		"smc #0\n"
		: "+r" (function_id)
		: "r" (arg0), "r" (arg1), "r" (arg2));

	return (u64)function_id;
}


static void enable_write_evict(void)
{
	(void)atfd_l3extension_smc((u64)L3_EXTENSION_SVC,
				   (u64)CMD_ENABLE_WE,
				   (u64)0,
				   (u64)0);
}

#define L3EXTENSION_ATTR_RW(_name) \
	static DEVICE_ATTR(_name, 0660, show_##_name, store_##_name)


/* mode */
static ssize_t store_mode(struct device *dev,
			  struct device_attribute *attr,
			  const char *buf, size_t count)
{
	struct l3extension *l3e = dev_get_drvdata(dev);
	int val = 0;
	int ret;

	ret = kstrtoint(buf, 10, &val);
	if (ret != 0) {
		dev_err(dev, "mode is not valid, val = %d\n", val);
		return -EINVAL;
	}
	val = !!val;

	switch (val) {
	case AI_ENTRY:
		spin_lock(&l3e->enable_lock);
		if (!l3e->mode_flag) {
			l3e->mode_flag = true;
			l3e->enable_flag = true;
			lb_gid_enable(0);
			trace_l3extension_switch("enable forcedly");
		}
		spin_unlock(&l3e->enable_lock);
		break;
	case AI_EXIT:
		/* disable forcedly, restart work */
		spin_lock(&l3e->enable_lock);
		if (l3e->mode_flag) {
			l3e->mode_flag = false;
			trace_l3extension_switch("disable forcedly");
		}
		spin_unlock(&l3e->enable_lock);
		break;
	default:
		pr_err("invalid mode value, val = %d\n", val);
		return -EINVAL;
	}

	return count;
}

static ssize_t show_mode(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	struct l3extension *l3e = dev_get_drvdata(dev);
	ssize_t ret;

	spin_lock(&l3e->enable_lock);
	ret = scnprintf(buf, PAGE_SIZE, "mode = %d\n", l3e->mode_flag);
	spin_unlock(&l3e->enable_lock);

	return ret;
}

L3EXTENSION_ATTR_RW(mode);

static struct attribute *dev_entries[] = {
	&dev_attr_mode.attr,
	NULL,
};

static struct attribute_group dev_attr_group = {
	.name	= "control",
	.attrs	= dev_entries,
};

static int check_write_evict_enabled(void)
{
	unsigned int val = 0;

	asm volatile("mrs %0, s3_0_c15_c3_4" : "=r" (val));
	if ((val & BIT(CACHE_EVICT_CONTROL)) == 0)
		return 0;
	else
		return 1;
}

static int l3ex_fcm_idle_notif(struct notifier_block *nb,
			       unsigned long action, void *data)
{
	bool fcm_pwrdn = true;
	struct l3extension *l3e = container_of(nb,
					       struct l3extension,
					       idle_nb);

	switch (action) {
	case CPU_PM_ENTER:
		spin_lock(&l3e->data_lock);
		fcm_pwrdn = hisi_fcm_cluster_pwrdn();
		if (fcm_pwrdn && !l3e->fcm_idle) {
			l3e->fcm_idle = true;
		}
		spin_unlock(&l3e->data_lock);
		break;
	case CPU_PM_ENTER_FAILED:
	case CPU_PM_EXIT:
		spin_lock(&l3e->data_lock);
		if (l3e->fcm_idle) {
			l3e->fcm_idle = false;

			spin_lock(&l3e->enable_lock);
			if (check_write_evict_enabled() == 0 &&
			    l3e->enable_flag)
				enable_write_evict();
			spin_unlock(&l3e->enable_lock);
		}
		spin_unlock(&l3e->data_lock);
		break;
	default:
		return NOTIFY_DONE;
	}
	return NOTIFY_OK;
}


static int l3extension_suspend(struct device *dev)
{
	struct l3extension *l3e = dev_get_drvdata(dev);

	spin_lock(&l3e->enable_lock);
	if (l3e->enable_flag) {
		l3e->enable_flag = false;
		dev_err(dev, "lb_gid_bypass\n");
		lb_gid_bypass(0);
	}
	spin_unlock(&l3e->enable_lock);

	return 0;
}

static int l3extension_resume(struct device *dev)
{
	struct l3extension *l3e = dev_get_drvdata(dev);

	spin_lock(&l3e->enable_lock);
	if (!l3e->enable_flag) {
		l3e->enable_flag = true;
		dev_err(dev, "lb_gid_enable\n");
		lb_gid_enable(0);
	}
	spin_unlock(&l3e->enable_lock);
	return 0;
}

static SIMPLE_DEV_PM_OPS(l3extension_pm,
			 l3extension_suspend,
			 l3extension_resume);

static const struct of_device_id l3extension_id[] = {
	{.compatible = "hisi,l3extension"},
	{}
};

static int l3extension_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct l3extension *l3e = NULL;
	int ret;

	dev_err(dev, "l3extension enter\n");
	if (is_lb_available() == 0) {
		dev_err(dev, "lb is not available\n");
		return -ENODEV;
	}

	l3e = devm_kzalloc(dev, sizeof(struct l3extension), GFP_KERNEL);
	if (l3e == NULL)
		return -ENOMEM;

	dev_set_name(dev, "l3extension");
	platform_set_drvdata(pdev, l3e);
	l3e->pdev = pdev;
	dev_set_drvdata(&pdev->dev, l3e);


	spin_lock_init(&l3e->data_lock);
	spin_lock_init(&l3e->enable_lock);
	l3e->fcm_idle = false;
	l3e->mode_flag = false;

	l3e->idle_nb.notifier_call = l3ex_fcm_idle_notif;
	ret = cpu_pm_register_notifier(&l3e->idle_nb);
	if (ret != 0) {
		dev_err(dev, "cpu pm register failed\n");
		goto out;
	}

	ret = sysfs_create_group(&pdev->dev.kobj, &dev_attr_group);
	if (ret != 0) {
		dev_err(&pdev->dev, "sysfs create err %d\n", ret);
		goto err_sysfs;
	}

	spin_lock(&l3e->enable_lock);
	l3e->enable_flag = true;
	lb_gid_enable(0);
	spin_unlock(&l3e->enable_lock);

	dev_err(dev, "l3extension exit\n");
	return 0;

err_sysfs:
	cpu_pm_unregister_notifier(&l3e->idle_nb);
out:
	return ret;
}

static int l3extension_remove(struct platform_device *pdev)
{
	struct l3extension *l3e = platform_get_drvdata(pdev);

	spin_lock(&l3e->enable_lock);
	l3e->enable_flag = false;
	lb_gid_bypass(0);
	spin_unlock(&l3e->enable_lock);
	sysfs_remove_group(&pdev->dev.kobj, &dev_attr_group);

	cpu_pm_unregister_notifier(&l3e->idle_nb);

	return 0;
}

MODULE_DEVICE_TABLE(of, l3extension_id);
#define MODULE_NAME	"l3extension"
static struct platform_driver l3extension_driver = {
	.probe	= l3extension_probe,
	.remove = l3extension_remove,
	.driver = {
		.name = MODULE_NAME,
		.of_match_table = l3extension_id,
		.pm = &l3extension_pm,
		.owner = THIS_MODULE,
	},
};

static int __init l3extension_init(void)
{
	return platform_driver_register(&l3extension_driver);
}

static void __exit l3extension_exit(void)
{
	platform_driver_unregister(&l3extension_driver);
}

module_init(l3extension_init);
module_exit(l3extension_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("L3Cache extension driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
