/*
 * wlc_wp.c
 *
 * wireless charge wp test
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

#include <linux/module.h>
#include <linux/device.h>
#include <linux/notifier.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/power_dts.h>
#include <chipset_common/hwpower/power_test.h>
#include <huawei_platform/power/wireless_charger.h>
#include <huawei_platform/power/wireless_direct_charger.h>

#include "wlc_wp.h"

#define HWLOG_TAG wireless_charge_wp
HWLOG_REGIST();

static struct wlc_wp_data *g_wlc_wp_di;

void wlc_wp_send_charge_mode(struct wlc_wp_data *di)
{
	int ret;

	hwlog_info("[%s] mode=%d\n", __func__, di->dc_type);
	ret = wireless_send_charge_mode(WIRELESS_PROTOCOL_QI, (u8)di->dc_type);
	if (ret)
		hwlog_err("%s: fail\n", __func__);
}

static void wldc_wp_chk_work(struct work_struct *work)
{
	int cp_avg_iout;
	struct wlc_wp_data *di = g_wlc_wp_di;

	if (!di)
		return;

	if (wldc_is_stop_charging_complete())
		return;

	cp_avg_iout = wldc_get_cp_avg_iout();
	if (cp_avg_iout > di->dts_para.cp_iout_th) {
		wlc_wp_send_charge_mode(di);
		return;
	}

	schedule_delayed_work(&di->wldc_wp_chk_work,
		msecs_to_jiffies(WLDC_WP_CHK_INTERVAL));
}

static void wlc_wp_start_test(struct wlc_wp_data *di)
{
	u8 tx_type = 0;

	wireless_charge_get_tx_adaptor_type(&tx_type);
	if ((tx_type < QI_PARA_TX_TYPE_FAC_BASE) ||
		(tx_type > QI_PARA_TX_TYPE_FAC_MAX))
		return;

	hwlog_info("[wp_start_test] tx_type=%u\n", tx_type);
	schedule_delayed_work(&di->wldc_wp_chk_work,
		msecs_to_jiffies(0));
}

static int wlc_wp_notifier_call(struct notifier_block *wlc_mmi_nb,
	unsigned long evt, void *data)
{
	struct wlc_wp_data *di = g_wlc_wp_di;

	if (!di)
		return NOTIFY_OK;

	switch (evt) {
	case WLC_NOTIFY_DC_START_CHARGING:
		if (data)
			di->dc_type = *((int *)data);
		wlc_wp_start_test(di);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int wlc_wp_parse_dts(struct device_node *np, struct wlc_wp_data *di)
{
	int ret;
	u32 tmp_para[WLC_WP_PARA_MAX] = { 0 };

	ret = power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"wlc_wp_para", tmp_para, WLC_WP_PARA_MAX);
	if (ret)
		tmp_para[WLC_WP_PARA_CP_IOUT_TH] = WLDC_WP_DEFLT_CP_IOUT_TH;

	di->dts_para.cp_iout_th = (int)tmp_para[WLC_WP_PARA_CP_IOUT_TH];
	hwlog_info("[wlc_wp_para] cp_iout_th:%dma\n", di->dts_para.cp_iout_th);
	return 0;
}

static void wlc_wp_init(struct device *dev)
{
	int ret;
	struct wlc_wp_data *di = NULL;

	if (!dev || !dev->of_node)
		return;

	di = devm_kzalloc(dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return;

	g_wlc_wp_di = di;
	ret = wlc_wp_parse_dts(dev->of_node, di);
	if (ret) {
		hwlog_err("init: parse dts failed\n");
		goto free_mem;
	}

	INIT_DELAYED_WORK(&di->wldc_wp_chk_work, wldc_wp_chk_work);
	di->wlc_wp_nb.notifier_call = wlc_wp_notifier_call;
	ret = register_wireless_charge_notifier(&di->wlc_wp_nb);
	if (ret) {
		hwlog_err("init: register notifier failed\n");
		goto free_mem;
	}

	hwlog_info("init succ\n");
	return;

free_mem:
	devm_kfree(dev, di);
	g_wlc_wp_di = NULL;
}

static void wlc_wp_exit(struct device *dev)
{
	if (!g_wlc_wp_di || !dev)
		return;

	cancel_delayed_work_sync(&g_wlc_wp_di->wldc_wp_chk_work);
	(void)unregister_wireless_charge_notifier(&g_wlc_wp_di->wlc_wp_nb);
	devm_kfree(dev, g_wlc_wp_di);
	g_wlc_wp_di = NULL;
}

static struct power_test_ops g_wlc_wp_ops = {
	.name = "wlc_wp",
	.init = wlc_wp_init,
	.exit = wlc_wp_exit,
};

static int __init wlc_wp_module_init(void)
{
	return power_test_ops_register(&g_wlc_wp_ops);
}

module_init(wlc_wp_module_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wlc_mmi module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
