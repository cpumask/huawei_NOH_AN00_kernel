// SPDX-License-Identifier: GPL-2.0
/*
 * ComboPHY Function Module on HiSilicon platform
 *
 * Copyright (C) 2020-2022 Hilisicon Electronics Co., Ltd.
 *		http://www.huawei.com
 *
 */

#include <linux/hisi/usb/tca.h>
#include <huawei_platform/dp_source/dp_dsm.h>
#include <huawei_platform/usb/hw_pd_dev.h>
#include <linux/hisi/hisi_dss_dp.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include "combophy_common.h"
#include "combophy_core.h"

#undef pr_fmt
#define pr_fmt(fmt) "[COMBOPHY_FUNC]%s: " fmt, __func__

#define MAX_EVENT_COUNT 8
#define INVALID_REG_VALUE 0xFFFFFFFFu
#define COMBOPHY_EVENT_WAIT_TIMEOUT msecs_to_jiffies(10500)

struct combophy_manager {
	TCPC_MUX_CTRL_TYPE curr_mode;
	TYPEC_PLUG_ORIEN_E curr_orien;
	struct timer_list event_timer;
};

static struct combophy_manager *g_combophy_manager;

static void combophy_event_complete(struct hisi_usb_event *event)
{
	struct combophy_manager *manager = event->content;
	TCPC_MUX_CTRL_TYPE mode_type = (TCPC_MUX_CTRL_TYPE)event->param1;
	TYPEC_PLUG_ORIEN_E typec_orien = (TYPEC_PLUG_ORIEN_E)event->param2;

	pr_info("event %s, mode %s, orien %d\n",
		dev_type_string((TCA_DEV_TYPE_E)event->type),
		mode_type_string(mode_type),
		typec_orien);

	/* update mode type and orien after event complete */
	manager->curr_mode = mode_type;
	manager->curr_orien = typec_orien;

	del_timer_sync(&manager->event_timer);
	dp_dfp_u_notify_dp_configuration_done(mode_type, 0);
}

static void combophy_handle_dp_event(struct hisi_usb_event *event)
{
	struct combophy_manager *manager = event->content;
	TCA_IRQ_TYPE_E irq_type = (TCA_IRQ_TYPE_E)event->param1;
	int ret = -EFAULT;

	pr_info("irq %s\n", irq_type_string(irq_type));

	if (!manager) {
		pr_err("event content is null, invalid parameter\n");
		return;
	}
	if (manager->curr_mode != TCPC_DP &&
	    manager->curr_mode != TCPC_USB31_AND_DP_2LINE) {
		pr_err("mode %s event %d mismatch\n",
		       mode_type_string(manager->curr_mode),
		       event->type);
		goto out;
	}

	ret = hisi_dptx_hpd_trigger(irq_type,
				    manager->curr_mode,
				    manager->curr_orien);
	if (ret)
		pr_err("hisi_dptx_hpd_trigger err[%d][%d]\n",
		       __LINE__, ret);

out:
	del_timer_sync(&manager->event_timer);
	dp_dfp_u_notify_dp_configuration_done(manager->curr_mode, ret);
}

static int combophy_event_handler(struct combophy_manager *manager,
				   TCA_IRQ_TYPE_E irq_type,
				   TCPC_MUX_CTRL_TYPE mode_type,
				   TCA_DEV_TYPE_E dev_type,
				   TYPEC_PLUG_ORIEN_E typec_orien)

{
	struct hisi_usb_event usb_event = {0};
	int ret;

	usb_event.type = (enum otg_dev_event_type)dev_type;
	usb_event.content = manager;
	switch (dev_type) {
	case TCA_DP_IN:
	case TCA_DP_OUT:
		usb_event.param1 = (uint32_t)irq_type;
		usb_event.flags = EVENT_CB_AT_HANDLE;
		usb_event.callback = combophy_handle_dp_event;
		break;
	case TCA_ID_RISE_EVENT:
	case TCA_CHARGER_DISCONNECT_EVENT:
	case TCA_ID_FALL_EVENT:
	case TCA_CHARGER_CONNECT_EVENT:
		usb_event.param1 = (uint32_t)mode_type;
		usb_event.param2 = (uint32_t)typec_orien;
		usb_event.flags = PD_EVENT | EVENT_CB_AT_COMPLETE;
		usb_event.callback = combophy_event_complete;
		break;
	default:
		return -EINVAL;
	}

	ret = hisi_usb_queue_event(&usb_event);
	if (ret) {
		pr_err("queue event failed %d\n", ret);
		return ret;
	}

	manager->event_timer.expires = jiffies + COMBOPHY_EVENT_WAIT_TIMEOUT;
	manager->event_timer.data = (TIMER_DATA_TYPE)mode_type;
	mod_timer(&manager->event_timer, manager->event_timer.expires);

	return 0;
}

int pd_event_notify(TCA_IRQ_TYPE_E irq_type, TCPC_MUX_CTRL_TYPE mode_type,
		    TCA_DEV_TYPE_E dev_type, TYPEC_PLUG_ORIEN_E typec_orien)
{
	struct combophy_manager *manager = NULL;
	int ret;

	manager = g_combophy_manager;
	if (!manager) {
		pr_err("combophy func not probed\n");
		ret = hisi_usb_otg_event((enum otg_dev_event_type)dev_type);
		if (ret)
			pr_err("hisi_usb_otg_event ret %d\n", ret);
		dp_dfp_u_notify_dp_configuration_done(mode_type, ret);
		return ret;
	}

	pr_info("IRQ[%s]MODEcur[%s]new[%s]DEV[%s]ORIEN[%d]\n",
		irq_type_string(irq_type),
		mode_type_string(manager->curr_mode),
		mode_type_string(mode_type),
		dev_type_string(dev_type),
		typec_orien);
	dp_imonitor_set_pd_event(irq_type, manager->curr_mode, mode_type,
				 dev_type, typec_orien);

	if (irq_type >= TCA_IRQ_MAX_NUM || mode_type >= TCPC_MUX_MODE_MAX ||
	    dev_type >= TCA_DEV_MAX || typec_orien >= TYPEC_ORIEN_MAX) {
		ret = -EINVAL;
		pr_err("invalid pd event\n");
		goto err_out;
	}

	ret = combophy_event_handler(manager, irq_type, mode_type,
				     dev_type, typec_orien);
	if (ret)
		goto err_out;

	pr_info("-\n");
	return 0;
err_out:
	pr_err("err out %d\n", ret);
	dp_dfp_u_notify_dp_configuration_done(mode_type, ret);
	return ret;
}

static void combophy_event_watchdog(TIMER_DATA_TYPE arg)
{
	TCPC_MUX_CTRL_TYPE mode_type = (TCPC_MUX_CTRL_TYPE)arg;

	pr_err("timeout for handle event\n");
	dp_dfp_u_notify_dp_configuration_done(mode_type, -ETIMEDOUT);
}

static int combophy_func_probe(struct platform_device *pdev)
{
	struct combophy_manager *manager = NULL;
	struct device *dev = &pdev->dev;
	int ret;

	pr_info("+\n");
	manager = devm_kzalloc(dev, sizeof(*manager), GFP_KERNEL);
	if (!manager)
		return -ENOMEM;

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_no_callbacks(dev);
	ret = pm_runtime_get_sync(dev);
	if (ret < 0) {
		pr_err("pm_runtime_get_sync failed\n");
		goto err_pm_put;
	}

	pm_runtime_forbid(dev);

	manager->curr_mode = TCPC_NC;
	manager->curr_orien = TYPEC_ORIEN_POSITIVE;
	setup_timer(&manager->event_timer, combophy_event_watchdog, 0);

	g_combophy_manager = manager;

	pr_info("-\n");

	return 0;
err_pm_put:
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);

	pr_err("ret %d\n", ret);
	return ret;
}

static int combophy_func_remove(struct platform_device *pdev)
{
	struct combophy_manager *manager = g_combophy_manager;

	del_timer_sync(&manager->event_timer);
	pm_runtime_allow(&pdev->dev);
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	g_combophy_manager = NULL;

	return 0;
}

static const struct of_device_id combophy_func_of_match[] = {
	{.compatible = "hisilicon,combophy-func",},
	{ }
};
MODULE_DEVICE_TABLE(of, combophy_func_of_match);

static struct platform_driver combophy_func_driver = {
	.probe	= combophy_func_probe,
	.remove = combophy_func_remove,
	.driver = {
		.name	= "hisi-combophy-func",
		.of_match_table	= combophy_func_of_match,
	}
};
module_platform_driver(combophy_func_driver);

MODULE_AUTHOR("Yu Chen <chenyu56@huawei.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hilisicon ComboPHY Function Driver");
