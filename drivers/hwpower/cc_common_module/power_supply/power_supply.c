/*
 * power_supply.c
 *
 * power supply interface for power module
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

#include <chipset_common/hwpower/power_supply.h>
#include <chipset_common/hwpower/power_dts.h>
#include <huawei_platform/log/hw_log.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG power_psy
HWLOG_REGIST();

static struct power_supply_dev *g_power_supply_dev;
static BLOCKING_NOTIFIER_HEAD(g_power_supply_nh);

static const char * const g_power_supply_prop_table[] = {
	[POWER_SUPPLY_PROP_BAT] = "bat",
	[POWER_SUPPLY_PROP_MAINS] = "mains",
	[POWER_SUPPLY_PROP_USB] = "usb",
	[POWER_SUPPLY_PROP_WIRELESS] = "wireless",
};

static const char * const g_power_supply_ne_table[POWER_SUPPLY_NE_END] = {
	[POWER_SUPPLY_NE_UPDATE_BATTERY] = "update_battery",
	[POWER_SUPPLY_NE_UPDATE_MAINS] = "update_mains",
	[POWER_SUPPLY_NE_UPDATE_USB] = "update_usb",
	[POWER_SUPPLY_NE_UPDATE_WIRELESS] = "update_wireless",
};

static const char *power_supply_get_ne_name(unsigned int event)
{
	if ((event >= POWER_SUPPLY_NE_BEGIN) && (event < POWER_SUPPLY_NE_END))
		return g_power_supply_ne_table[event];

	return "illegal ne";
}

static int power_supply_get_prop_type(const char *str)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(g_power_supply_prop_table); i++) {
		if (!strncmp(str, g_power_supply_prop_table[i], strlen(str)))
			return i;
	}

	hwlog_err("invalid prop_type_str=%s\n", str);
	return -1;
}

static int power_supply_check_prop_type(unsigned int type)
{
	if ((type >= POWER_SUPPLY_PROP_BEGIN) && (type < POWER_SUPPLY_PROP_END))
		return 0;

	hwlog_err("invalid prop_type=%d\n", type);
	return -1;
}

static struct power_supply_dev *power_supply_get_dev(void)
{
	if (!g_power_supply_dev) {
		hwlog_err("g_power_supply_dev is null\n");
		return NULL;
	}

	return g_power_supply_dev;
}

static struct power_supply_ops *power_supply_get_ops(unsigned int type)
{
	if (power_supply_check_prop_type(type))
		return NULL;

	if (!g_power_supply_dev || !g_power_supply_dev->p_ops[type]) {
		hwlog_err("g_power_supply_dev or p_ops is null\n");
		return NULL;
	}

	return g_power_supply_dev->p_ops[type];
}

int power_supply_ops_register(struct power_supply_ops *ops)
{
	int type;

	if (!g_power_supply_dev || !ops || !ops->type_name) {
		hwlog_err("g_power_supply_dev or ops or type_name is null\n");
		return -1;
	}

	type = power_supply_get_prop_type(ops->type_name);
	if (type < 0) {
		hwlog_err("%s ops register fail\n", ops->type_name);
		return -1;
	}

	g_power_supply_dev->p_ops[type] = ops;
	g_power_supply_dev->total_ops++;

	hwlog_info("total_ops=%d type=%d:%s ops register ok\n",
		g_power_supply_dev->total_ops, type, ops->type_name);
	return 0;
}

static int power_supply_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct power_supply_dev *l_dev = power_supply_get_dev();

	if (!l_dev)
		return NOTIFY_OK;

	hwlog_info("receive event %s\n", power_supply_get_ne_name(event));

	switch (event) {
	case POWER_SUPPLY_NE_UPDATE_BATTERY:
		if (l_dev->support_bat)
			power_supply_changed(l_dev->bat);
		break;
	case POWER_SUPPLY_NE_UPDATE_MAINS:
		if (l_dev->support_mains)
			power_supply_changed(l_dev->mains);
		break;
	case POWER_SUPPLY_NE_UPDATE_USB:
		if (l_dev->support_usb)
			power_supply_changed(l_dev->usb);
		break;
	case POWER_SUPPLY_NE_UPDATE_WIRELESS:
		if (l_dev->support_wireless)
			power_supply_changed(l_dev->wireless);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int power_supply_notifier_chain_register(struct notifier_block *nb)
{
	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	return blocking_notifier_chain_register(&g_power_supply_nh, nb);
}

static int power_supply_notifier_chain_unregister(struct notifier_block *nb)
{
	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	return blocking_notifier_chain_unregister(&g_power_supply_nh, nb);
}

void power_supply_event_notify(unsigned long event, void *data)
{
	blocking_notifier_call_chain(&g_power_supply_nh, event, data);
}

static int power_supply_get_status_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_status_prop)
		return POWER_SUPPLY_DEFAULT_STATUS;

	return l_ops->get_status_prop();
}

static int power_supply_get_health_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_health_prop)
		return POWER_SUPPLY_DEFAULT_HEALTH;

	return l_ops->get_health_prop();
}

static int power_supply_get_present_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_present_prop)
		return POWER_SUPPLY_DEFAULT_PRESENT;

	return l_ops->get_present_prop();
}

static int power_supply_get_online_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_online_prop)
		return POWER_SUPPLY_DEFAULT_ONLINE;

	return l_ops->get_online_prop();
}

static int power_supply_get_technology_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_technology_prop)
		return POWER_SUPPLY_DEFAULT_TECHNOLOGY;

	return l_ops->get_technology_prop();
}

static int power_supply_get_temp_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_temp_prop)
		return POWER_SUPPLY_DEFAULT_TEMP;

	return l_ops->get_temp_prop();
}

static int power_supply_get_cycle_count_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_cycle_count_prop)
		return POWER_SUPPLY_DEFAULT_CYCLE_COUNT;

	return l_ops->get_cycle_count_prop();
}

static int power_supply_get_limit_fcc_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_limit_fcc_prop)
		return POWER_SUPPLY_DEFAULT_LIMIT_FCC;

	return l_ops->get_limit_fcc_prop();
}

static int power_supply_get_voltage_now_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_voltage_now_prop)
		return POWER_SUPPLY_DEFAULT_VOLTAGE_NOW;

	return l_ops->get_voltage_now_prop();
}

static int power_supply_get_voltage_max_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_voltage_max_prop)
		return POWER_SUPPLY_DEFAULT_VOLTAGE_MAX;

	return l_ops->get_voltage_max_prop();
}

static int power_supply_get_current_now_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_current_now_prop)
		return POWER_SUPPLY_DEFAULT_CURRENT_NOW;

	return l_ops->get_current_now_prop();
}

static int power_supply_get_capacity_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_capacity_prop)
		return POWER_SUPPLY_DEFAULT_CAPACITY;

	return l_ops->get_capacity_prop();
}

static int power_supply_get_capacity_level_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_capacity_level_prop)
		return POWER_SUPPLY_DEFAULT_CAPACITY_LEVEL;

	return l_ops->get_capacity_level_prop();
}

static int power_supply_get_capacity_fcc_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_capacity_fcc_prop)
		return POWER_SUPPLY_DEFAULT_CAPACITY_FCC;

	return l_ops->get_capacity_fcc_prop();
}

static int power_supply_get_capacity_rm_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_capacity_rm_prop)
		return POWER_SUPPLY_DEFAULT_CAPACITY_RM;

	return l_ops->get_capacity_rm_prop();
}

static int power_supply_get_charge_full_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_charge_full_prop)
		return POWER_SUPPLY_DEFAULT_CHARGE_FULL;

	return l_ops->get_charge_full_prop();
}

static int power_supply_get_charge_now_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_charge_now_prop)
		return POWER_SUPPLY_DEFAULT_CHARGE_NOW;

	return l_ops->get_charge_now_prop();
}

static int power_supply_get_batid_property(type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_batid_prop)
		return POWER_SUPPLY_DEFAULT_BATID;

	return l_ops->get_batid_prop();
}

static const char *power_supply_get_brand_property(int type)
{
	struct power_supply_ops *l_ops = power_supply_get_ops(type);

	if (!l_ops || !l_ops->get_brand_prop)
		return POWER_SUPPLY_DEFAULT_BRAND;

	return l_ops->get_brand_prop();
}

static enum power_supply_property power_supply_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_CYCLE_COUNT,
	POWER_SUPPLY_PROP_LIMIT_FCC,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_CAPACITY_LEVEL,
	POWER_SUPPLY_PROP_CAPACITY_RM,
	POWER_SUPPLY_PROP_CAPACITY_FCC,
	POWER_SUPPLY_PROP_CHARGE_FULL,
	POWER_SUPPLY_PROP_CHARGE_NOW,
	POWER_SUPPLY_PROP_ID_VOLTAGE,
	POWER_SUPPLY_PROP_BRAND,
};

static enum power_supply_property power_supply_mains_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_HEALTH,
};

static enum power_supply_property power_supply_usb_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property power_supply_wireless_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static int power_supply_battery_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	int type = POWER_SUPPLY_PROP_BAT;

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = power_supply_get_status_property(type);
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = power_supply_get_health_property(type);
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = power_supply_get_present_property(type);
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = power_supply_get_online_property(type);
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = power_supply_get_technology_property(type);
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = power_supply_get_temp_property(type);
		break;
	case POWER_SUPPLY_PROP_CYCLE_COUNT:
		val->intval = power_supply_get_cycle_count_property(type);
		break;
	case POWER_SUPPLY_PROP_LIMIT_FCC:
		val->intval = power_supply_get_limit_fcc_property(type);
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = power_supply_get_voltage_now_property(type);
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		val->intval = power_supply_get_voltage_max_property(type);
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		val->intval = power_supply_get_current_now_property(type);
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = power_supply_get_capacity_property(type);
		break;
	case POWER_SUPPLY_PROP_CAPACITY_LEVEL:
		val->intval = power_supply_get_capacity_level_property(type);
		break;
	case POWER_SUPPLY_PROP_CAPACITY_RM:
		val->intval = power_supply_get_capacity_rm_property(type);
		break;
	case POWER_SUPPLY_PROP_CAPACITY_FCC:
		val->intval = power_supply_get_capacity_fcc_property(type);
		break;
	case POWER_SUPPLY_PROP_CHARGE_FULL:
		val->intval = power_supply_get_charge_full_property(type);
		break;
	case POWER_SUPPLY_PROP_CHARGE_NOW:
		val->intval = power_supply_get_charge_now_property(type);
		break;
	case POWER_SUPPLY_PROP_ID_VOLTAGE:
		val->intval = power_supply_get_batid_property(type);
		break;
	case POWER_SUPPLY_PROP_BRAND:
		val->strval = power_supply_get_brand_property(type);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int power_supply_mains_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	int type = POWER_SUPPLY_PROP_MAINS;

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = power_supply_get_status_property(type);
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = power_supply_get_health_property(type);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int power_supply_usb_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	int type = POWER_SUPPLY_PROP_USB;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = power_supply_get_online_property(type);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int power_supply_wireless_get_property(struct power_supply *psy,
	enum power_supply_property psp, union power_supply_propval *val)
{
	int type = POWER_SUPPLY_PROP_WIRELESS;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = power_supply_get_online_property(type);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static const struct power_supply_desc power_supply_battery_desc = {
	.name = "Battery",
	.type = POWER_SUPPLY_TYPE_BATTERY,
	.properties = power_supply_battery_props,
	.num_properties = ARRAY_SIZE(power_supply_battery_props),
	.get_property = power_supply_battery_get_property,
};

static const struct power_supply_desc power_supply_mains_desc = {
	.name = "Mains",
	.type = POWER_SUPPLY_TYPE_MAINS,
	.properties = power_supply_mains_props,
	.num_properties = ARRAY_SIZE(power_supply_mains_props),
	.get_property = power_supply_mains_get_property,
};

static const struct power_supply_desc power_supply_usb_desc = {
	.name = "USB",
	.type = POWER_SUPPLY_TYPE_USB,
	.properties = power_supply_usb_props,
	.num_properties = ARRAY_SIZE(power_supply_usb_props),
	.get_property = power_supply_usb_get_property,
};

static const struct power_supply_desc power_supply_wireless_desc = {
	.name = "Wireless",
	.type = POWER_SUPPLY_TYPE_WIRELESS,
	.properties = power_supply_wireless_props,
	.num_properties = ARRAY_SIZE(power_supply_wireless_props),
	.get_property = power_supply_wireless_get_property,
};

static int power_supply_register_power_supply(struct power_supply_dev *l_dev)
{
	if (l_dev->support_bat) {
		l_dev->bat = power_supply_register(l_dev->dev,
			&power_supply_battery_desc, NULL);
		if (IS_ERR(l_dev->bat))
			goto fail_register_battery;
	}

	if (l_dev->support_mains) {
		l_dev->mains = power_supply_register(l_dev->dev,
			&power_supply_mains_desc, NULL);
		if (IS_ERR(l_dev->mains))
			goto fail_register_mains;
	}

	if (l_dev->support_usb) {
		l_dev->usb = power_supply_register(l_dev->dev,
			&power_supply_usb_desc, NULL);
		if (IS_ERR(l_dev->usb))
			goto fail_register_usb;
	}

	if (l_dev->support_wireless) {
		l_dev->wireless = power_supply_register(l_dev->dev,
			&power_supply_wireless_desc, NULL);
		if (IS_ERR(l_dev->wireless))
			goto fail_register_wireless;
	}

	return 0;

fail_register_wireless:
	power_supply_unregister(l_dev->usb);
fail_register_usb:
	power_supply_unregister(l_dev->mains);
fail_register_mains:
	power_supply_unregister(l_dev->bat);
fail_register_battery:
	return -EINVAL;
}

static void power_supply_unregister_power_supply(struct power_supply_dev *l_dev)
{
	if (l_dev->support_wireless)
		power_supply_unregister(l_dev->wireless);

	if (l_dev->support_usb)
		power_supply_unregister(l_dev->usb);

	if (l_dev->support_mains)
		power_supply_unregister(l_dev->mains);

	if (l_dev->support_bat)
		power_supply_unregister(l_dev->bat);
}

static int power_supply_parse_dts(struct power_supply_dev *l_dev)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), l_dev->np,
		"support_bat", &l_dev->support_bat, 1);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), l_dev->np,
		"support_mains", &l_dev->support_mains, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), l_dev->np,
		"support_usb", &l_dev->support_usb, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), l_dev->np,
		"support_wireless", &l_dev->support_wireless, 0);

	return 0;
}

static int power_supply_probe(struct platform_device *pdev)
{
	int ret;
	struct power_supply_dev *l_dev = NULL;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	l_dev = kzalloc(sizeof(*l_dev), GFP_KERNEL);
	if (!l_dev)
		return -ENOMEM;

	g_power_supply_dev = l_dev;
	l_dev->dev = &pdev->dev;
	l_dev->np = pdev->dev.of_node;

	ret = power_supply_parse_dts(l_dev);
	if (ret)
		goto fail_free_mem;

	ret = power_supply_register_power_supply(l_dev);
	if (ret)
		goto fail_free_mem;

	l_dev->nb.notifier_call = power_supply_notifier_call;
	power_supply_notifier_chain_register(&l_dev->nb);
	platform_set_drvdata(pdev, l_dev);
	return 0;

fail_free_mem:
	kfree(l_dev);
	g_power_supply_dev = NULL;
	return ret;
}

static int power_supply_remove(struct platform_device *pdev)
{
	struct power_supply_dev *l_dev = platform_get_drvdata(pdev);

	if (!l_dev)
		return -ENODEV;

	power_supply_notifier_chain_unregister(&l_dev->nb);
	power_supply_unregister_power_supply(l_dev);
	platform_set_drvdata(pdev, NULL);
	kfree(l_dev);
	g_power_supply_dev = NULL;

	return 0;
}

static const struct of_device_id power_supply_match_table[] = {
	{
		.compatible = "huawei,power_supply",
		.data = NULL,
	},
	{},
};

static struct platform_driver power_supply_driver = {
	.probe = power_supply_probe,
	.remove = power_supply_remove,
	.driver = {
		.name = "huawei,power_supply",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(power_supply_match_table),
	},
};

static int __init power_supply_init(void)
{
	return platform_driver_register(&power_supply_driver);
}

static void __exit power_supply_exit(void)
{
	platform_driver_unregister(&power_supply_driver);
}

fs_initcall_sync(power_supply_init);
module_exit(power_supply_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("power supply interface driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
