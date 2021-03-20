/*
 * wireless_lightstrap.c
 *
 * wireless lightstrap driver
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
 *
 */

#include <huawei_platform/power/wireless_lightstrap.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/power/hisi/hisi_bci_battery.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/wireless_transmitter.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG lightstrap
HWLOG_REGIST();
#define LIGHTSTRAP_MAX_RX_SIZE           5
#define LIGHTSTRAP_PRODUCT_TYPE          7
#define LIGHTSTRAP_OFF                   0
#define LIGHTSTRAP_TIMEOUT               3600
#define LIGHTSTRAP_DELAY                 500

static struct lightstrap_di *g_lightstrap_di;

#ifdef CONFIG_SYSFS
static ssize_t wireless_lightstrap_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);
static struct power_sysfs_attr_info lightstrap_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(wireless_lightstrap, 0444,
		LIGHTSTRAP_SYSFS_DEV_PRODUCT_TYPE, rx_product_type),
};
static struct attribute *lightstrap_sysfs_attrs[ARRAY_SIZE(lightstrap_sysfs_field_tbl) + 1];
static const struct attribute_group lightstrap_sysfs_attr_group = {
	.attrs = lightstrap_sysfs_attrs,
};

static ssize_t wireless_lightstrap_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct lightstrap_di *di = g_lightstrap_di;

	info = power_sysfs_lookup_attr(attr->attr.name,
		lightstrap_sysfs_field_tbl,
		ARRAY_SIZE(lightstrap_sysfs_field_tbl));
	if (!info || !di)
		return -EINVAL;

	switch (info->name) {
	case LIGHTSTRAP_SYSFS_DEV_PRODUCT_TYPE:
		return snprintf(buf, PAGE_SIZE, "%u\n", di->product_type);
	default:
		return 0;
	}
}

static void lightstrap_sysfs_create_group(struct device *dev)
{
	power_sysfs_init_attrs(lightstrap_sysfs_attrs,
		lightstrap_sysfs_field_tbl,
		ARRAY_SIZE(lightstrap_sysfs_field_tbl));
	power_sysfs_create_link_group("hw_power", "charger",
		"wireless_lightstrap", dev, &lightstrap_sysfs_attr_group);
}

static void lightstrap_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "charger",
		"wireless_lightstrap", dev, &lightstrap_sysfs_attr_group);
}
#else
static inline void lightstrap_sysfs_create_group(struct device *dev)
{
}

static inline void lightstrap_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

bool lightstrap_need_specify_pwr_src(void)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (!di) {
		hwlog_err("di is null\n");
		return false;
	}

	return di->product_type == LIGHTSTRAP_PRODUCT_TYPE;
}

enum wltx_pwr_src lightstrap_specify_pwr_src(void)
{
	return PWR_SRC_5VBST;
}

static bool can_soc_do_reverse_charging(void)
{
	int soc = hisi_bci_show_capacity();

	if ((wireless_charge_get_wired_channel_state() == WIRED_CHANNEL_OFF) &&
		(soc <= WL_TX_SOC_MIN) && lightstrap_need_specify_pwr_src()) {
		hwlog_info("[%s] capacity is out of range\n", __func__);
		wireless_tx_set_tx_status(WL_TX_STATUS_TX_CLOSE);
		return false;
	}

	return true;
}

static bool lightstrap_can_do_reverse_charging(void)
{
	if (!can_soc_do_reverse_charging())
		return false;

	return true;
}

static void lightstrap_send_on_uevent(struct lightstrap_di *di)
{
	char *envp[LIGHTSTRAP_MAX_RX_SIZE] = { "LIGHTSTRAPCASE=ON", "RXID=07",
		"MODELID=296", "SUBMODELID=0", NULL };

	if (!di)
		return;

	kobject_uevent_env(&di->dev->kobj, KOBJ_CHANGE, envp);
	hwlog_info("lightstrap send case=on uevent\n");
}

static void lightstrap_send_off_uevent(struct lightstrap_di *di)
{
	char *envp[LIGHTSTRAP_MAX_RX_SIZE] = { "LIGHTSTRAPCASE=OFF", "RXID=00",
		"MODELID=296", "SUBMODELID=0", NULL };

	if (!di)
		return;

	kobject_uevent_env(&di->dev->kobj, KOBJ_CHANGE, envp);
	hwlog_info("lightstrap send case=off uevent\n");
}

static void lightstrap_check_work(struct work_struct *work)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (di->is_opened_by_hall && !lightstrap_need_specify_pwr_src()) {
		wltx_open_tx(WLTX_OPEN_BY_LIGHTSTRAP, false);
		di->is_opened_by_hall = false;
	}
}

static void lightstrap_close_wltx(void)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	if (di->is_opened_by_hall) {
		cancel_delayed_work(&di->check_work);
		wltx_open_tx(WLTX_OPEN_BY_LIGHTSTRAP, false);
		msleep(LIGHTSTRAP_DELAY);
		di->is_opened_by_hall = false;
	}
}

static void lightstrap_event_work(struct work_struct *work)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

	hwlog_info("lightstrap event type = %d\n", di->event_type);
	switch (di->event_type) {
	case LIGHTSTRAP_EVENT_ON:
		if (wireless_tx_get_tx_status() == WL_TX_STATUS_IN_CHARGING)
			return;

		di->is_opened_by_hall = true;
		wltx_open_tx(WLTX_OPEN_BY_LIGHTSTRAP, true);
		schedule_delayed_work(&di->check_work, msecs_to_jiffies(LIGHTSTRAP_TIMEOUT));
		break;
	case LIGHTSTRAP_EVENT_OFF:
		if (lightstrap_need_specify_pwr_src()) {
			di->product_type = LIGHTSTRAP_OFF;
			lightstrap_send_off_uevent(di);
		}

		lightstrap_close_wltx();
		break;
	case LIGHTSTRAP_EVENT_GET_PRODUCT_TYPE:
		if (di->is_opened_by_hall) {
			lightstrap_close_wltx();
			lightstrap_send_on_uevent(di);
		}

		break;
	case LIGHTSTRAP_EVENT_EPT:
		wltx_open_tx(WLTX_OPEN_BY_LIGHTSTRAP, false);
		break;
	default:
		hwlog_err("invalid lightstrap_event_type\n");
		break;
	}
}

static int lightstrap_event_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	u8 product_type;
	struct lightstrap_di *di = container_of(nb, struct lightstrap_di, event_nb);

	if (!di) {
		hwlog_err("di is null\n");
		return NOTIFY_OK;
	}

	di->event_type = event;
	if (di->event_type == LIGHTSTRAP_EVENT_GET_PRODUCT_TYPE) {
		if (!data)
			return NOTIFY_OK;

		product_type = *(u8 *)data;
		if (product_type != LIGHTSTRAP_PRODUCT_TYPE)
			return NOTIFY_OK;

		di->product_type = product_type;
	}
	schedule_work(&di->event_work);

	return NOTIFY_OK;
}

static int lightstrap_event_notifier_chain_register(struct notifier_block *nb)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (di && nb)
		return blocking_notifier_chain_register(&di->event_nh, nb);

	return NOTIFY_OK;
}

static int lightstrap_event_notifier_chain_unregister(struct notifier_block *nb)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (di && nb)
		return blocking_notifier_chain_unregister(&di->event_nh, nb);

	return NOTIFY_OK;
}

void lightstrap_event_notify(unsigned long e, void *v)
{
	struct lightstrap_di *di = g_lightstrap_di;

	if (di)
		blocking_notifier_call_chain(&di->event_nh, e, v);
}

static struct wltx_logic_ops g_lightstrap_logic_ops = {
	.type                 = WLTX_OPEN_BY_LIGHTSTRAP,
	.can_rev_charge_check = lightstrap_can_do_reverse_charging,
	.need_specify_pwr_src = lightstrap_need_specify_pwr_src,
	.specify_pwr_src      = lightstrap_specify_pwr_src,
	.need_reinit_tx_para  = lightstrap_need_specify_pwr_src,
};

static int lightstrap_probe(struct platform_device *pdev)
{
	int ret;
	struct lightstrap_di *di = NULL;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_lightstrap_di = di;
	di->dev = &pdev->dev;

	BLOCKING_INIT_NOTIFIER_HEAD(&di->event_nh);
	INIT_WORK(&di->event_work, lightstrap_event_work);
	INIT_DELAYED_WORK(&di->check_work, lightstrap_check_work);
	platform_set_drvdata(pdev, di);
	di->event_nb.notifier_call = lightstrap_event_notifier_call;
	ret = lightstrap_event_notifier_chain_register(&di->event_nb);
	if (ret)
		goto notifier_regist_fail;

	ret = wireless_tx_logic_ops_register(&g_lightstrap_logic_ops);
	if (ret)
		goto logic_ops_regist_fail;

	lightstrap_sysfs_create_group(di->dev);

	return 0;

logic_ops_regist_fail:
	(void)lightstrap_event_notifier_chain_unregister(&di->event_nb);
notifier_regist_fail:
	kfree(di);
	di = NULL;
	g_lightstrap_di = NULL;
	platform_set_drvdata(pdev, NULL);

	return ret;
}

static int lightstrap_remove(struct platform_device *pdev)
{
	struct lightstrap_di *di = platform_get_drvdata(pdev);

	if (!di) {
		hwlog_err("di is null\n");
		return 0;
	}

	lightstrap_sysfs_remove_group(di->dev);
	lightstrap_event_notifier_chain_unregister(&di->event_nb);
	kfree(di);
	g_lightstrap_di = NULL;
	platform_set_drvdata(pdev, NULL);

	return 0;
}

static const struct of_device_id lightstrap_match_table[] = {
	{
		.compatible = "huawei,lightstrap",
		.data = NULL,
	},
	{},
};

static struct platform_driver lightstrap_driver = {
	.probe = lightstrap_probe,
	.remove = lightstrap_remove,
	.driver = {
		.name = "huawei,lightstrap",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lightstrap_match_table),
	},
};

static int __init lightstrap_init(void)
{
	return platform_driver_register(&lightstrap_driver);
}

static void __exit lightstrap_exit(void)
{
	platform_driver_unregister(&lightstrap_driver);
}

device_initcall_sync(lightstrap_init);
module_exit(lightstrap_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wireless lightstrap module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
