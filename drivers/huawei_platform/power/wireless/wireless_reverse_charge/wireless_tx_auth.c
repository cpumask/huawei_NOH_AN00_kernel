/*
 * wireless_tx_auth.c
 *
 * wireless tx auth for wireless reverse charging
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/power_mesg.h>
#include <chipset_common/hwpower/power_sysfs.h>
#include <huawei_platform/power/wireless/wireless_protocol.h>
#include <chipset_common/hwpower/power_event_ne.h>
#include <huawei_platform/power/wireless/wireless_tx/wireless_tx_auth.h>

#define HWLOG_TAG wireless_tx_auth
HWLOG_REGIST();

static struct wireless_tx_auth_info *g_wireless_tx_auth_di;
static bool g_wltx_auth_antifake_result;
static u8 g_rx_random[WLTX_RX_RANDOM_LEN];

int wireless_tx_auth_get_tx_hash(u8 *rx_random, u8 rx_random_len)
{
	int i;
	struct power_event_notify_data n_data;
	struct wireless_tx_auth_info *di = g_wireless_tx_auth_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return -1;
	}

	if (rx_random_len != WLTX_RX_RANDOM_LEN) {
		hwlog_err("%s: rx random len error\n", __func__);
		return -1;
	}

	for (i = 0; i < WLTX_RX_RANDOM_LEN; i++) {
		g_rx_random[i] = rx_random[i];
		rx_random[i] = 0;
		hwlog_info("[%s] g_rx_random[%d] = 0x%x\n",
			__func__, i, g_rx_random[i]);
	}

	g_wltx_auth_antifake_result = false;
	n_data.event = "AUTH_WTCT=";
	n_data.event_len = 10; /* length of "AUTH_WTCT=" */
	power_event_notify(POWER_EVENT_NE_AUTH_WLTX, &n_data);

	if (!wait_for_completion_timeout(&di->wltx_auth_af_completion,
		WLTX_AF_WAIT_CT_TIMEOUT)) {
		hwlog_err("bms_auth wait wltx_auth_af_completion timeout\n");
		return -1;
	}

	/*
	 * if not timeout,
	 * return the antifake result base on the hash calc result
	 */
	if (!g_wltx_auth_antifake_result) {
		hwlog_err("bms_auth hash calculate fail\n");
		return -1;
	}

	hwlog_info("bms_auth hash calculate ok\n");
	return 0;
}

#ifdef CONFIG_SYSFS
static ssize_t wireless_tx_auth_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf);
static ssize_t wireless_tx_auth_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count);

static struct power_sysfs_attr_info wireless_tx_auth_sysfs_field_tbl[] = {
	power_sysfs_attr_rw(wireless_tx_auth, 0644, WLTX_AUTH_SYSFS_AF, af),
};

static struct attribute *wireless_tx_auth_sysfs_attrs[ARRAY_SIZE(
	wireless_tx_auth_sysfs_field_tbl) + 1];

static const struct attribute_group wireless_tx_auth_sysfs_attr_group = {
	.attrs = wireless_tx_auth_sysfs_attrs,
};

static void wireless_tx_auth_sysfs_init_attrs(void)
{
	int i;
	int limit = ARRAY_SIZE(wireless_tx_auth_sysfs_field_tbl);

	for (i = 0; i < limit; i++)
		wireless_tx_auth_sysfs_attrs[i] =
			&wireless_tx_auth_sysfs_field_tbl[i].attr.attr;

	wireless_tx_auth_sysfs_attrs[limit] = NULL;
}

static struct power_sysfs_attr_info *wireless_tx_auth_sysfs_field_lookup(
	const char *name)
{
	int i;
	int limit = ARRAY_SIZE(wireless_tx_auth_sysfs_field_tbl);

	for (i = 0; i < limit; i++) {
		if (!strncmp(name,
			wireless_tx_auth_sysfs_field_tbl[i].attr.attr.name,
			strlen(name)))
			break;
	}
	if (i >= limit)
		return NULL;

	return &wireless_tx_auth_sysfs_field_tbl[i];
}

static void wireless_tx_auth_sysfs_create_group(struct device *dev)
{
	wireless_tx_auth_sysfs_init_attrs();
	power_sysfs_create_link_group("hw_power", "charger", "wireless_tx_auth",
		dev, &wireless_tx_auth_sysfs_attr_group);
}

static void wireless_tx_auth_sysfs_remove_group(struct device *dev)
{
	power_sysfs_remove_link_group("hw_power", "charger", "wireless_tx_auth",
		dev, &wireless_tx_auth_sysfs_attr_group);
}

static int wireless_tx_auth_get_rx_random(char *buf, unsigned int len)
{
	int i;
	struct wireless_tx_auth_info *di = g_wireless_tx_auth_di;

	if (!di) {
		hwlog_err("di is null\n");
		return 0;
	}

	for (i = 0; i < len; i++) {
		buf[i] = g_rx_random[i];
		g_rx_random[i] = 0;
	}

	return len;
}

static void wireless_tx_auth_set_tx_cipherkey(struct wireless_tx_auth_info *di,
	u8 *cipherkey, u8 cipherkey_len)
{
	g_wltx_auth_antifake_result = true;
	complete(&di->wltx_auth_af_completion);

	wireless_set_tx_cipherkey(WIRELESS_PROTOCOL_QI,
		cipherkey, cipherkey_len);
}

static ssize_t wireless_tx_auth_sysfs_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct power_sysfs_attr_info *info = NULL;
	struct wireless_tx_auth_info *di = g_wireless_tx_auth_di;

	info = wireless_tx_auth_sysfs_field_lookup(attr->attr.name);
	if (!info || !di)
		return -EINVAL;

	switch (info->name) {
	case WLTX_AUTH_SYSFS_AF:
		return wireless_tx_auth_get_rx_random(buf, WLTX_RX_RANDOM_LEN);
	default:
		break;
	}
	return 0;
}

static ssize_t wireless_tx_auth_sysfs_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	u8 hash[WLTX_TX_HASH_LEN] = { 0 };
	struct wireless_tx_auth_info *di = g_wireless_tx_auth_di;
	struct power_sysfs_attr_info *info =
		wireless_tx_auth_sysfs_field_lookup(attr->attr.name);

	if (!info || !di)
		return -EINVAL;

	switch (info->name) {
	case WLTX_AUTH_SYSFS_AF:
		memcpy(hash, buf, WLTX_TX_HASH_LEN);
		wireless_tx_auth_set_tx_cipherkey(di, hash, WLTX_TX_HASH_LEN);
		break;
	default:
		break;
	}
	return count;
}
#else
static inline void wireless_tx_auth_sysfs_create_group(struct device *dev)
{
}

static inline void wireless_tx_auth_sysfs_remove_group(struct device *dev)
{
}
#endif /* CONFIG_SYSFS */

static int wireless_tx_auth_probe(struct platform_device *pdev)
{
	struct wireless_tx_auth_info *di = NULL;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_wireless_tx_auth_di = di;
	di->dev = &pdev->dev;
	wireless_tx_auth_sysfs_create_group(di->dev);
	init_completion(&di->wltx_auth_af_completion);
	return 0;
}

static int wireless_tx_auth_remove(struct platform_device *pdev)
{
	struct wireless_tx_auth_info *di = platform_get_drvdata(pdev);

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return 0;
	}

	wireless_tx_auth_sysfs_remove_group(di->dev);
	return 0;
}

static const struct of_device_id wireless_tx_auth_match_table[] = {
	{
		.compatible = "huawei, wireless_tx_auth",
		.data = NULL,
	},
	{},
};

static struct platform_driver wireless_tx_auth_driver = {
	.probe = wireless_tx_auth_probe,
	.remove = wireless_tx_auth_remove,
	.driver = {
		.name = "huawei, wireless_tx_auth",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(wireless_tx_auth_match_table),
	},
};

static int __init wireless_tx_auth_init(void)
{
	return platform_driver_register(&wireless_tx_auth_driver);
}

static void __exit wireless_tx_auth_exit(void)
{
	platform_driver_unregister(&wireless_tx_auth_driver);
}

device_initcall_sync(wireless_tx_auth_init);
module_exit(wireless_tx_auth_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wireless tx auth driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
