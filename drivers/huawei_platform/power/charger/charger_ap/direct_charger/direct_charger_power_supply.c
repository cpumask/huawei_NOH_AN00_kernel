/*
 * direct_charger_power_supply.c
 *
 * power supply for direct charger
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/delay.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/huawei_charger.h>
#ifdef CONFIG_BOOST_5V
#include <huawei_platform/power/boost_5v.h>
#endif
#ifdef CONFIG_USB_AUDIO_POWER
#include <huawei_platform/audio/usb_audio_power.h>
#endif
#ifdef CONFIG_DIG_HS_POWER
#include <huawei_platform/audio/dig_hs_power.h>
#endif

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG direct_charge_ps
HWLOG_REGIST();

static u32 scp_ps_by_5vboost;
static u32 scp_ps_by_charger;

static u32 is_need_bst_ctrl;
static int bst_ctrl;
static u32 bst_ctrl_use_common_gpio;

int direct_charge_set_bst_ctrl(int enable)
{
	int ret = 0;

	if (is_need_bst_ctrl) {
		if (!bst_ctrl_use_common_gpio) {
			ret |= gpio_direction_output(bst_ctrl, enable);
		} else {
#ifdef CONFIG_USB_AUDIO_POWER
			ret |= bst_ctrl_enable(enable, VBOOST_CONTROL_PM);
#endif /* CONFIG_USB_AUDIO_POWER */
#ifdef CONFIG_DIG_HS_POWER
			ret |= dig_hs_bst_ctrl_enable(enable, DIG_HS_VBOOST_CONTROL_PM);
#endif /* CONFIG_DIG_HS_POWER */
		}
	}

	return ret;
}

static int scp_ps_enable_by_5vboost(int enable)
{
	int ret = 0;

	hwlog_info("by_5vboost=%d,%d,%d, enable=%d\n",
		scp_ps_by_5vboost, is_need_bst_ctrl, bst_ctrl_use_common_gpio,
		enable);

	if (scp_ps_by_5vboost) {
#ifdef CONFIG_BOOST_5V
		ret |= boost_5v_enable(enable, BOOST_CTRL_DC);
#endif /* CONFIG_BOOST_5V */

		ret |= direct_charge_set_bst_ctrl(enable);
	}

	return ret;
}

static int scp_ps_enable_by_charger(int enable)
{
	int ret = 0;

	hwlog_info("by_charger=%d, enable=%d\n", scp_ps_by_charger, enable);

	if (scp_ps_by_charger) {
#ifdef CONFIG_HUAWEI_CHARGER_AP
		ret |= charge_otg_mode_enable(enable, VBUS_CH_USER_DC);
#endif /* CONFIG_HUAWEI_CHARGER_AP */
	}

	return ret;
}

static int scp_ps_enable_by_dummy(int enable)
{
	hwlog_info("by_dummy=dummy, enable=%d\n", enable);

	return 0;
}

static struct direct_charge_pps_ops scp_ps_dummy_ops = {
	.power_supply_enable = scp_ps_enable_by_dummy,
};

static struct direct_charge_pps_ops scp_ps_5vboost_ops = {
	.power_supply_enable = scp_ps_enable_by_5vboost,
};

static struct direct_charge_pps_ops scp_ps_charger_ops = {
	.power_supply_enable = scp_ps_enable_by_charger,
};

int direct_charge_ps_probe(struct platform_device *pdev)
{
	int ret;
	struct device_node *np = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	np = pdev->dev.of_node;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"boost_5v_support_scp_power", &scp_ps_by_5vboost, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"huawei_charger_support_scp_power", &scp_ps_by_charger, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"is_need_bst_ctrl", &is_need_bst_ctrl, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"bst_ctrl_use_common_gpio", &bst_ctrl_use_common_gpio, 0);

	if ((is_need_bst_ctrl) && (!bst_ctrl_use_common_gpio)) {
		if (power_gpio_request(np, "bst_ctrl", "bst_ctrl", &bst_ctrl))
			return -1;
	}

	/* regisger dummy protocol power supply ops */
	ret = direct_charge_pps_ops_register(&scp_ps_dummy_ops);
	if (ret) {
		hwlog_err("(dummy)scp power supply ops register fail\n");
		goto fail_register_ops;
	} else {
		hwlog_info("(dummy)scp power supply ops register ok\n");
	}

	/* regisger 5vboost protocol power supply ops */
	if (scp_ps_by_5vboost) {
		ret = direct_charge_pps_ops_register(&scp_ps_5vboost_ops);
		if (ret) {
			hwlog_err("(5vboost)scp power supply ops register fail\n");
			goto fail_register_ops;
		} else {
			hwlog_info("(5vboost)scp power supply ops register ok\n");
		}
	}

	/* regisger charger protocol power supply ops */
	if (scp_ps_by_charger) {
		ret = direct_charge_pps_ops_register(&scp_ps_charger_ops);
		if (ret) {
			hwlog_err("(charger)scp power supply ops register fail\n");
			goto fail_register_ops;
		} else {
			hwlog_info("(charger)scp power supply ops register ok\n");
		}
	}

	return 0;

fail_register_ops:
	if ((is_need_bst_ctrl) && (!bst_ctrl_use_common_gpio))
		gpio_free(bst_ctrl);

	return ret;
}

static int direct_charge_ps_remove(struct platform_device *pdev)
{
	if ((is_need_bst_ctrl) && (!bst_ctrl_use_common_gpio))
		gpio_free(bst_ctrl);

	return 0;
}

static const struct of_device_id direct_charge_ps_match_table[] = {
	{
		.compatible = "huawei,direct_charge_ps",
		.data = NULL,
	},
	{},
};

static struct platform_driver direct_charge_ps_driver = {
	.probe = direct_charge_ps_probe,
	.remove = direct_charge_ps_remove,
	.driver = {
		.name = "huawei,direct_charge_ps",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(direct_charge_ps_match_table),
	},
};

static int __init direct_charge_ps_init(void)
{
	return platform_driver_register(&direct_charge_ps_driver);
}

static void __exit direct_charge_ps_exit(void)
{
	platform_driver_unregister(&direct_charge_ps_driver);
}

device_initcall_sync(direct_charge_ps_init);
module_exit(direct_charge_ps_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("direct charge power supply module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
