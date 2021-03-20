/*
 * power_event.c
 *
 * event for power module
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

#include "power_event.h"
#include <chipset_common/hwpower/power_event_ne.h>
#include <chipset_common/hwpower/power_sysfs.h>
#include <chipset_common/hwpower/power_interface.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG power_event
HWLOG_REGIST();

struct power_event_dev *g_power_event_dev;
static BLOCKING_NOTIFIER_HEAD(g_power_event_nh);

static const char * const g_power_event_ne_table[POWER_EVENT_NE_END] = {
	/* section: for send uevent form kernel to user */
	[POWER_EVENT_NE_USB_DISCONNECT] = "usb_disconnect",
	[POWER_EVENT_NE_USB_CONNECT] = "usb_connect",
	[POWER_EVENT_NE_WIRELESS_DISCONNECT] = "wireless_disconnect",
	[POWER_EVENT_NE_WIRELESS_CONNECT] = "wireless_connect",
	[POWER_EVENT_NE_WIRELESS_REVERSE_START] = "wireless_reverse_start",
	[POWER_EVENT_NE_WIRELESS_REVERSE_STOP] = "wireless_reverse_stop",
	[POWER_EVENT_NE_VBUS_ABSENT] = "vbus_absent",
	[POWER_EVENT_NE_AUTH_DC_SC] = "auth_dc_sc",
	[POWER_EVENT_NE_AUTH_WL_SC] = "auth_wl_sc",
	[POWER_EVENT_NE_AUTH_WL_SC_UVDM] = "auth_wl_sc_uvdm",
	[POWER_EVENT_NE_AUTH_WLTX] = "auth_wltx",
	[POWER_EVENT_NE_FW_UPDATE_WIRELESS] = "fw_update_wireless",
	[POWER_EVENT_NE_FW_UPDATE_ADAPTER] = "fw_update_adapter",
	[POWER_EVENT_NE_UI_CABLE_TYPE] = "ui_cable_type",
	[POWER_EVENT_NE_UI_MAX_POWER] = "ui_max_power",
	[POWER_EVENT_NE_UI_WL_OFF_POS] = "ui_wl_off_pos",
	[POWER_EVENT_NE_UI_WL_FAN_STATUS] = "ui_wl_fan_status",
	[POWER_EVENT_NE_UI_WL_COVER_STATUS] = "ui_wl_cover_status",
	[POWER_EVENT_NE_UI_WATER_STATUS] = "ui_water_status",
	[POWER_EVENT_NE_UI_HEATING_STATUS] = "ui_heating_status",
	[POWER_EVENT_NE_UI_ICON_TYPE] = "ui_icon_type",
	[POWER_EVENT_NE_HEATING_START] = "heating_start",
	[POWER_EVENT_NE_HEATING_STOP] = "heating_stop",
	/* section: for send start & stop charging */
	[POWER_EVENT_NE_START_CHARGING] = "start_charging",
	[POWER_EVENT_NE_STOP_CHARGING] = "stop_charging",
	/* section: for soc decimal */
	[POWER_EVENT_NE_SOC_DECIMAL_DC] = "soc_decimal_dc",
	[POWER_EVENT_NE_SOC_DECIMAL_WL_DC] = "soc_decimal_wl_dc",
	/* section: for bsoh uevent */
	[POWER_EVENT_NE_BSOH_TYPE] = "bsoh_notify",
};

static const char *power_event_get_ne_name(unsigned int event)
{
	if ((event >= POWER_EVENT_NE_BEGIN) && (event < POWER_EVENT_NE_END))
		return g_power_event_ne_table[event];

	return "illegal ne";
}

static struct power_event_dev *power_event_get_dev(void)
{
	if (!g_power_event_dev) {
		hwlog_err("g_power_event_dev is null\n");
		return NULL;
	}

	return g_power_event_dev;
}

static void power_event_notify_uevent(struct power_event_dev *l_dev, void *data)
{
	char uevent_buf[POWER_EVENT_NOTIFY_SIZE] = { 0 };
	char *envp[POWER_EVENT_NOTIFY_NUM] = { uevent_buf, NULL };
	struct power_event_notify_data *n_data = NULL;
	int i, ret;

	if (!l_dev || !l_dev->sysfs_ne)
		return;

	n_data = (struct power_event_notify_data *)data;
	if (!n_data || !n_data->event) {
		hwlog_err("n_data or event is null\n");
		return;
	}

	if (n_data->event_len >= POWER_EVENT_NOTIFY_SIZE) {
		hwlog_err("event_len is invalid\n");
		return;
	}

	for (i = 0; i < n_data->event_len; i++)
		uevent_buf[i] = n_data->event[i];
	hwlog_info("receive uevent_buf %s\n", uevent_buf);

	ret = kobject_uevent_env(l_dev->sysfs_ne, KOBJ_CHANGE, envp);
	if (ret < 0)
		hwlog_err("notify uevent fail, ret=%d\n", ret);
}

static void power_event_vbus_connect(struct power_event_dev *l_dev)
{
	struct power_event_notify_data n_data;

	/* ignore repeat event */
	if (l_dev->connect_state == POWER_EVENT_CONNECT)
		return;
	l_dev->connect_state = POWER_EVENT_CONNECT;

	n_data.event = "VBUS_CONNECT=";
	n_data.event_len = 13; /* length of VBUS_CONNECT= */
	power_event_notify_uevent(l_dev, &n_data);
}

static void power_event_vbus_disconnect(struct power_event_dev *l_dev)
{
	struct power_event_notify_data n_data;

	/* ignore repeat event */
	if (l_dev->connect_state == POWER_EVENT_DISCONNECT)
		return;
	l_dev->connect_state = POWER_EVENT_DISCONNECT;

	n_data.event = "VBUS_DISCONNECT=";
	n_data.event_len = 16; /* length of VBUS_DISCONNECT= */
	power_event_notify_uevent(l_dev, &n_data);

	power_if_kernel_sysfs_set(POWER_IF_OP_TYPE_ALL, POWER_IF_SYSFS_READY, 0);
}

static void power_event_wireless_reverse_start(void)
{
	struct power_event_notify_data n_data;
	struct power_event_dev *l_dev = power_event_get_dev();

	if (!l_dev)
		return;

	n_data.event = "TX_OPEN=";
	n_data.event_len = 8; /* length of TX_OPEN= */
	power_event_notify_uevent(l_dev, &n_data);
}

static void power_event_wireless_reverse_stop(void)
{
	struct power_event_notify_data n_data;
	struct power_event_dev *l_dev = power_event_get_dev();

	if (!l_dev)
		return;

	n_data.event = "TX_CLOSE=";
	n_data.event_len = 9; /* length of TX_CLOSE= */
	power_event_notify_uevent(l_dev, &n_data);
}

static int power_event_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct power_event_dev *l_dev = power_event_get_dev();

	if (!l_dev)
		return NOTIFY_OK;

	hwlog_info("receive event %s\n", power_event_get_ne_name(event));

	switch (event) {
	case POWER_EVENT_NE_USB_DISCONNECT:
	case POWER_EVENT_NE_WIRELESS_DISCONNECT:
		power_event_vbus_disconnect(l_dev);
		break;
	case POWER_EVENT_NE_USB_CONNECT:
	case POWER_EVENT_NE_WIRELESS_CONNECT:
		power_event_vbus_connect(l_dev);
		break;
	case POWER_EVENT_NE_WIRELESS_REVERSE_START:
		power_event_wireless_reverse_start();
		break;
	case POWER_EVENT_NE_WIRELESS_REVERSE_STOP:
		power_event_wireless_reverse_stop();
		break;
	case POWER_EVENT_NE_VBUS_ABSENT:
	case POWER_EVENT_NE_AUTH_DC_SC:
	case POWER_EVENT_NE_AUTH_WL_SC:
	case POWER_EVENT_NE_AUTH_WL_SC_UVDM:
	case POWER_EVENT_NE_AUTH_WLTX:
	case POWER_EVENT_NE_FW_UPDATE_WIRELESS:
	case POWER_EVENT_NE_FW_UPDATE_ADAPTER:
	case POWER_EVENT_NE_UI_CABLE_TYPE:
	case POWER_EVENT_NE_UI_MAX_POWER:
	case POWER_EVENT_NE_UI_WL_OFF_POS:
	case POWER_EVENT_NE_UI_WL_FAN_STATUS:
	case POWER_EVENT_NE_UI_WL_COVER_STATUS:
	case POWER_EVENT_NE_UI_WATER_STATUS:
	case POWER_EVENT_NE_UI_HEATING_STATUS:
	case POWER_EVENT_NE_UI_ICON_TYPE:
	case POWER_EVENT_NE_HEATING_START:
	case POWER_EVENT_NE_HEATING_STOP:
	case POWER_EVENT_NE_BSOH_TYPE:
		power_event_notify_uevent(l_dev, data);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

int power_event_notifier_chain_register(struct notifier_block *nb)
{
	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	return blocking_notifier_chain_register(&g_power_event_nh, nb);
}

int power_event_notifier_chain_unregister(struct notifier_block *nb)
{
	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	return blocking_notifier_chain_unregister(&g_power_event_nh, nb);
}

void power_event_notify(unsigned long event, void *data)
{
	blocking_notifier_call_chain(&g_power_event_nh, event, data);
}

#ifdef CONFIG_SYSFS
static ssize_t power_event_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);

static struct power_sysfs_attr_info power_event_sysfs_field_tbl[] = {
	power_sysfs_attr_ro(power_event, 0440, POWER_EVENT_SYSFS_CONNECT_STATE, connect_state),
};

#define POWER_EVENT_SYSFS_ATTRS_SIZE  ARRAY_SIZE(power_event_sysfs_field_tbl)

static struct attribute *power_event_sysfs_attrs[POWER_EVENT_SYSFS_ATTRS_SIZE + 1];

static const struct attribute_group power_event_sysfs_attr_group = {
	.attrs = power_event_sysfs_attrs,
};

static ssize_t power_event_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct power_event_dev *l_dev = power_event_get_dev();

	if (!l_dev)
		return -EINVAL;

	info = power_sysfs_lookup_attr(attr->attr.name,
		power_event_sysfs_field_tbl, POWER_EVENT_SYSFS_ATTRS_SIZE);
	if (!info)
		return -EINVAL;

	switch (info->name) {
	case POWER_EVENT_SYSFS_CONNECT_STATE:
		return scnprintf(buf, PAGE_SIZE, "%d\n", l_dev->connect_state);
	default:
		return 0;
	}
}

static struct device *power_event_sysfs_create_group(void)
{
	power_sysfs_init_attrs(power_event_sysfs_attrs,
		power_event_sysfs_field_tbl, POWER_EVENT_SYSFS_ATTRS_SIZE);
	return power_sysfs_create_group("hw_power", "power_event",
		&power_event_sysfs_attr_group);
}

static void power_event_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &power_event_sysfs_attr_group);
}
#else
static inline struct device *power_event_sysfs_create_group(void)
{
	return NULL;
}

static inline void power_event_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int __init power_event_init(void)
{
	int ret;
	struct power_event_dev *l_dev = NULL;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_power_event_dev = l_dev;
	l_dev->nb.notifier_call = power_event_notifier_call;
	ret = power_event_notifier_chain_register(&l_dev->nb);
	if (ret)
		goto fail_free_mem;

	l_dev->dev = power_event_sysfs_create_group();
	if (l_dev->dev)
		l_dev->sysfs_ne = &l_dev->dev->kobj;
	l_dev->connect_state = POWER_EVENT_INVAID;

	return 0;

fail_free_mem:
	kfree(l_dev);
	g_power_event_dev = NULL;
	return ret;
}

static void __exit power_event_exit(void)
{
	struct power_event_dev *l_dev = g_power_event_dev;

	if (!l_dev)
		return;

	power_event_notifier_chain_unregister(&l_dev->nb);
	power_event_sysfs_remove_group(l_dev->dev);
	kfree(l_dev);
	g_power_event_dev = NULL;
}

fs_initcall_sync(power_event_init);
module_exit(power_event_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("power event module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
