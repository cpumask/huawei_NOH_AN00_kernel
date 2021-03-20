/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: usb misc ctrl ops
 * Author: Hisilicon
 * Create: 2019-09-30
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#include <linux/hisi/usb/hisi_usb_reg_cfg.h>
#include <linux/hisi/usb/hisi_usb_helper.h>
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#define MAX_CLKS	5

#define MISC_CTRL_INFO(format, arg...) \
	pr_info("[USB_MISC_CTRL][I][%s]"format, __func__, ##arg)

#define MISC_CTRL_ERR(format, arg...) \
	pr_err("[USB_MISC_CTRL][E][%s]"format, __func__, ##arg)

struct hisi_usb_misc_ctrl {
	struct device *dev;
	struct clk **clks;
	int num_clocks;
	int init_count;
	struct hisi_usb_reg_cfg *misc_ctrl_reset;
	struct hisi_usb_reg_cfg *misc_ctrl_unreset;
	struct hisi_usb_reg_cfg *misc_ctrl_is_unreset;
	struct hisi_usb_reg_cfg *exit_noc_power_idle;
	struct hisi_usb_reg_cfg *exit_noc_power_idleack;
	struct hisi_usb_reg_cfg *exit_noc_power_idlestate;
	struct hisi_usb_reg_cfg *enter_noc_power_idle;
	struct hisi_usb_reg_cfg *enter_noc_power_idleack;
	struct hisi_usb_reg_cfg *enter_noc_power_idlestate;
	struct hisi_usb_reg_cfg **init_regcfgs;
	int num_init_regcfg;
};

static int enter_noc_power_idle(struct hisi_usb_misc_ctrl *misc_ctrl);

static DEFINE_MUTEX(usb_misc_ctrl_lock);
static struct hisi_usb_misc_ctrl *usb_misc_ctrl;

static int reset_misc_ctrl(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	int ret;

	ret = hisi_usb_reg_write(misc_ctrl->misc_ctrl_reset);
	if (ret)
		MISC_CTRL_ERR("config failed\n");

	return ret;
}

static int unreset_misc_ctrl(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	int ret;

	ret = hisi_usb_reg_write(misc_ctrl->misc_ctrl_unreset);
	if (ret)
		MISC_CTRL_ERR("config failed\n");

	return ret;
}

bool misc_ctrl_is_ready(void)
{
	mutex_lock(&usb_misc_ctrl_lock);
	if (!usb_misc_ctrl) {
		mutex_unlock(&usb_misc_ctrl_lock);
		return false;
	}
	mutex_unlock(&usb_misc_ctrl_lock);

	return true;
}
EXPORT_SYMBOL_GPL(misc_ctrl_is_ready);

bool misc_ctrl_is_unreset(void)
{
	int ret;

	mutex_lock(&usb_misc_ctrl_lock);
	if (!usb_misc_ctrl) {
		ret = -ENODEV;
		goto out;
	}

	ret = hisi_usb_reg_test_cfg(usb_misc_ctrl->misc_ctrl_is_unreset);
	if (ret < 0)
		MISC_CTRL_ERR("config failed\n");

out:
	mutex_unlock(&usb_misc_ctrl_lock);

	return ret < 0 ? false : ret;
}
EXPORT_SYMBOL_GPL(misc_ctrl_is_unreset);

static bool test_exit_noc_power_idle(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	return (hisi_usb_reg_test_cfg(misc_ctrl->exit_noc_power_idleack) == 1)
		&& (hisi_usb_reg_test_cfg(misc_ctrl->exit_noc_power_idlestate)
				== 1);
}

static int exit_noc_power_idle(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	if (misc_ctrl->exit_noc_power_idle) {
		int ret;
		int retrys = 10;

		ret =  hisi_usb_reg_write(misc_ctrl->exit_noc_power_idle);
		if (ret < 0) {
			MISC_CTRL_ERR("config failed\n");
			return ret;
		}

		while (retrys--) {
			if (test_exit_noc_power_idle(usb_misc_ctrl))
				break;
			/* according to noc pw idle exit process */
			udelay(10);
		}

		if (retrys <= 0) {
			MISC_CTRL_ERR("wait noc power idle state timeout\n");
			return -ETIMEDOUT;
		}
	}

	return 0;
}

static int usb_misc_ctrl_init_cfg(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	if (misc_ctrl->init_regcfgs)
		return hisi_usb_reg_write_array(misc_ctrl->init_regcfgs,
					       misc_ctrl->num_init_regcfg);

	return 0;
}

int misc_ctrl_init(void)
{
	int ret;

	mutex_lock(&usb_misc_ctrl_lock);
	if (!usb_misc_ctrl) {
		ret = -ENODEV;
		goto out;
	}

	if (usb_misc_ctrl->init_count > 0) {
		usb_misc_ctrl->init_count++;
		ret = 0;
		goto out;
	}

	MISC_CTRL_INFO("+\n");
	ret = hisi_usb_init_clks(usb_misc_ctrl->clks,
				 usb_misc_ctrl->num_clocks);
	if (ret)
		goto out;

	ret = exit_noc_power_idle(usb_misc_ctrl);
	if (ret) {
		MISC_CTRL_ERR("exit_noc_power_idle failed ret %d\n", ret);
		goto shutdown_clks;
	}

	ret = usb_misc_ctrl_init_cfg(usb_misc_ctrl);
	if (ret) {
		MISC_CTRL_ERR("config init_regcfgs failed\n");
		goto enter_power_idle;
	}
	usb_misc_ctrl->init_count = 1;
	MISC_CTRL_INFO("-\n");
	mutex_unlock(&usb_misc_ctrl_lock);

	return 0;

enter_power_idle:
	if (enter_noc_power_idle(usb_misc_ctrl))
		MISC_CTRL_ERR("enter_noc_power_idle failed\n");
shutdown_clks:
	hisi_usb_shutdown_clks(usb_misc_ctrl->clks, usb_misc_ctrl->num_clocks);
out:
	mutex_unlock(&usb_misc_ctrl_lock);

	return ret;
}
EXPORT_SYMBOL_GPL(misc_ctrl_init);

static bool test_enter_noc_power_idle(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	return (hisi_usb_reg_test_cfg(misc_ctrl->enter_noc_power_idleack) == 1)
		&& (hisi_usb_reg_test_cfg(misc_ctrl->enter_noc_power_idlestate)
				== 1);
}

static int enter_noc_power_idle(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	if (misc_ctrl->exit_noc_power_idle) {
		int ret;
		int retrys = 10;

		ret =  hisi_usb_reg_write(misc_ctrl->enter_noc_power_idle);
		if (ret < 0) {
			MISC_CTRL_ERR("config failed\n");
			return ret;
		}

		while (retrys--) {
			if (test_enter_noc_power_idle(misc_ctrl))
				break;
			/* according to noc pw idle enter process */
			udelay(10);
		}

		if (retrys <= 0) {
			MISC_CTRL_ERR("wait noc power idle state timeout\n");
			return -ETIMEDOUT;
		}
	}

	return 0;
}

void misc_ctrl_exit(void)
{
	int ret;

	mutex_lock(&usb_misc_ctrl_lock);
	if (!usb_misc_ctrl)
		goto out;

	if (usb_misc_ctrl->init_count <= 0) {
		MISC_CTRL_ERR("call not balance\n");
		goto out;
	}

	if (--usb_misc_ctrl->init_count > 0)
		goto out;

	MISC_CTRL_INFO("+\n");
	ret = enter_noc_power_idle(usb_misc_ctrl);
	if (ret) {
		MISC_CTRL_ERR("enter_noc_power_idle failed ret %d\n", ret);
		goto out;
	}

	hisi_usb_shutdown_clks(usb_misc_ctrl->clks, usb_misc_ctrl->num_clocks);

	/* reset usb ctrl config */
	if (reset_misc_ctrl(usb_misc_ctrl))
		MISC_CTRL_ERR("reset_misc_ctrl failed\n");
	/* keep usb ctrl unreset */
	if (unreset_misc_ctrl(usb_misc_ctrl))
		MISC_CTRL_ERR("unreset_misc_ctrl failed\n");

	MISC_CTRL_INFO("-\n");

out:
	mutex_unlock(&usb_misc_ctrl_lock);
}
EXPORT_SYMBOL_GPL(misc_ctrl_exit);

static int get_clks(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	struct device *dev = misc_ctrl->dev;

	return hisi_usb_get_clks(dev, &misc_ctrl->clks, &misc_ctrl->num_clocks);
}

#define get_reg_cfg(name) (misc_ctrl->name = \
		of_get_hisi_usb_reg_cfg(np, #name))

static int get_reg_cfgs(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	struct device *dev = misc_ctrl->dev;
	struct device_node *np = dev->of_node;

	get_reg_cfg(misc_ctrl_reset);
	if (!misc_ctrl->misc_ctrl_reset)
		return -EINVAL;

	get_reg_cfg(misc_ctrl_unreset);
	if (!misc_ctrl->misc_ctrl_unreset)
		return -EINVAL;

	get_reg_cfg(misc_ctrl_is_unreset);
	if (!misc_ctrl->misc_ctrl_is_unreset)
		return -EINVAL;

	/* optional configs */
	get_reg_cfg(exit_noc_power_idle);
	get_reg_cfg(exit_noc_power_idleack);
	get_reg_cfg(exit_noc_power_idlestate);
	get_reg_cfg(enter_noc_power_idle);
	get_reg_cfg(enter_noc_power_idleack);
	get_reg_cfg(enter_noc_power_idlestate);

	if (misc_ctrl->exit_noc_power_idle &&
			(!misc_ctrl->exit_noc_power_idleack ||
			 !misc_ctrl->exit_noc_power_idlestate ||
			 !misc_ctrl->enter_noc_power_idle ||
			 !misc_ctrl->enter_noc_power_idleack ||
			 !misc_ctrl->enter_noc_power_idlestate))
		return -EINVAL;

	if (get_hisi_usb_reg_cfg_array(dev, "init_regcfgs",
				       &misc_ctrl->init_regcfgs,
				       &misc_ctrl->num_init_regcfg)) {
		MISC_CTRL_INFO("no init_regcfgs\n");
		misc_ctrl->init_regcfgs = NULL;
		misc_ctrl->num_init_regcfg = 0;
	}

	return 0;
}

static void put_reg_cfgs(struct hisi_usb_misc_ctrl *misc_ctrl)
{
	of_remove_hisi_usb_reg_cfg(misc_ctrl->misc_ctrl_reset);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->misc_ctrl_unreset);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->misc_ctrl_is_unreset);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->exit_noc_power_idle);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->exit_noc_power_idleack);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->exit_noc_power_idlestate);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->enter_noc_power_idle);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->enter_noc_power_idleack);
	of_remove_hisi_usb_reg_cfg(misc_ctrl->enter_noc_power_idlestate);
	if (misc_ctrl->init_regcfgs)
		free_hisi_usb_reg_cfg_array(misc_ctrl->init_regcfgs,
					    misc_ctrl->num_init_regcfg);
}

static int hisi_usb_misc_ctrl_probe(struct platform_device *pdev)
{
	struct hisi_usb_misc_ctrl *misc_ctrl = NULL;
	struct device *dev = &pdev->dev;
	int ret;

	MISC_CTRL_INFO("+\n");

	misc_ctrl = devm_kzalloc(dev, sizeof(*misc_ctrl), GFP_KERNEL);
	if (!misc_ctrl)
		return -ENOMEM;

	platform_set_drvdata(pdev, misc_ctrl);
	misc_ctrl->dev = dev;

	ret = get_clks(misc_ctrl);
	if (ret) {
		MISC_CTRL_ERR("get clks failed\n");
		return ret;
	}

	ret = get_reg_cfgs(misc_ctrl);
	if (ret) {
		put_reg_cfgs(misc_ctrl);
		MISC_CTRL_ERR("get reg cfgs failed\n");
		return ret;
	}

	ret = unreset_misc_ctrl(misc_ctrl);
	if (ret) {
		put_reg_cfgs(misc_ctrl);
		MISC_CTRL_ERR("unreset_misc_ctrl failed\n");
		return ret;
	}

	mutex_lock(&usb_misc_ctrl_lock);
	usb_misc_ctrl = misc_ctrl;
	mutex_unlock(&usb_misc_ctrl_lock);

	MISC_CTRL_INFO("+\n");

	return 0;
}

static int hisi_usb_misc_ctrl_remove(struct platform_device *pdev)
{
	struct hisi_usb_misc_ctrl *misc_ctrl = platform_get_drvdata(pdev);

	mutex_lock(&usb_misc_ctrl_lock);
	if (misc_ctrl->init_count) {
		mutex_unlock(&usb_misc_ctrl_lock);
		return -EBUSY;
	}

	if (reset_misc_ctrl(misc_ctrl))
		MISC_CTRL_ERR("reset_misc_ctrl failed\n");
	put_reg_cfgs(misc_ctrl);
	usb_misc_ctrl = NULL;
	mutex_unlock(&usb_misc_ctrl_lock);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int hisi_usb_misc_ctrl_resume_early(struct device *dev)
{
	struct hisi_usb_misc_ctrl *misc_ctrl = dev_get_drvdata(dev);
	int ret;

	MISC_CTRL_INFO("+\n");
	if (misc_ctrl) {
		ret = unreset_misc_ctrl(misc_ctrl);
		if (ret) {
			MISC_CTRL_ERR("unreset_misc_ctrl failed\n");
			return ret;
		}
	}
	MISC_CTRL_INFO("-\n");

	return 0;
}
#endif

static const struct dev_pm_ops hisi_usb_misc_ctrl_pm_ops = {
	SET_LATE_SYSTEM_SLEEP_PM_OPS(NULL, hisi_usb_misc_ctrl_resume_early)
};

static const struct of_device_id hisi_usb_misc_ctrl_of_match[] = {
	{ .compatible = "hisilicon,usb-misc-ctrl", },
	{ }
};
MODULE_DEVICE_TABLE(of, hisi_usb_misc_ctrl_of_match);

static struct platform_driver hisi_usb_misc_ctrl_driver = {
	.probe = hisi_usb_misc_ctrl_probe,
	.remove = hisi_usb_misc_ctrl_remove,
	.driver = {
		.name = "hisi-usb-misc-ctrl",
		.of_match_table = hisi_usb_misc_ctrl_of_match,
		.pm = &hisi_usb_misc_ctrl_pm_ops,
	}
};

static int __init hisi_usb_misc_ctrl_init(void)
{
	return platform_driver_register(&hisi_usb_misc_ctrl_driver);
}
subsys_initcall(hisi_usb_misc_ctrl_init);

static void __exit hisi_usb_misc_ctrl_exit(void)
{
	platform_driver_unregister(&hisi_usb_misc_ctrl_driver);
}
module_exit(hisi_usb_misc_ctrl_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hilisicon USB Misc Ctrl Driver");
