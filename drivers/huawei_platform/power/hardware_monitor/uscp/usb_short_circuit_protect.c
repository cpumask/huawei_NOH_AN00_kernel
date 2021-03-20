/*
 * usb_short_circuit_protect.c
 *
 * usb port short circuit protect (control by ap) monitor driver
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

#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/notifier.h>
#include <linux/pm_wakeup.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/hisi/hisi_adc.h>
#include <linux/delay.h>
#ifdef CONFIG_HISI_COUL
#include <linux/power/hisi/coul/hisi_coul_drv.h>
#endif
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/usb_short_circuit_protect.h>
#include <huawei_platform/power/huawei_battery_temp.h>

#define HWLOG_TAG uscp_ap
HWLOG_REGIST();

static bool uscp_enable = true;
/* uscp enable parameter 0640 */
module_param(uscp_enable, bool, USCP_ENABLE_PAR);

static int usb_temp = USB_TEMP_NUM;
/* usb temperature parameter 0440 */
module_param(usb_temp, int, USB_TEMP_PAR);

static bool uscp_enforce;
/* uscp enforce parameter 0640 */
module_param(uscp_enforce, bool, USCP_ENFORCE_PAR);

struct uscp_device_info {
	struct device *dev;
	struct workqueue_struct *uscp_wq;
	struct work_struct uscp_check_wk;
	struct notifier_block usb_nb;
	struct notifier_block tcpc_nb;
	struct hrtimer timer;
	int gpio_uscp;
	int uscp_threshold_tusb;
	int open_mosfet_temp;
	int open_hiz_temp;
	int close_mosfet_temp;
	int interval_switch_temp;
	int check_interval;
	int keep_check_cnt;
	u32 dmd_hiz_enable;
};

static int protect_enable;
static int protect_dmd_notify_enable = 1; /* enable */
static int protect_dmd_notify_enable_hiz = 1; /* enable */
static int is_uscp_mode;
static int is_hiz_mode = NOT_HIZ_MODE;
static int is_rt_uscp_mode;
static unsigned int first_in = 1;
#ifdef CONFIG_DIRECT_CHARGER
static int is_scp_charger;
#endif /* CONFIG_DIRECT_CHARGER */

static struct uscp_device_info *g_uscp_di;
static struct wakeup_source uscp_wakelock;

#ifdef CONFIG_HUAWEI_POWER_DEBUG
static ssize_t uscp_dbg_show(void *dev_data, char *buf, size_t size)
{
	struct uscp_device_info *dev_p = (struct uscp_device_info *)dev_data;

	if (!buf || !dev_p) {
		hwlog_err("buf or dev_p is null\n");
		return scnprintf(buf, size, "buf or dev_p is null\n");
	}

	return scnprintf(buf, size,
		"uscp_threshold_tusb=%d\n"
		"open_mosfet_temp=%d\n"
		"close_mosfet_temp=%d\n"
		"interval_switch_temp=%d\n",
		dev_p->uscp_threshold_tusb,
		dev_p->open_mosfet_temp,
		dev_p->close_mosfet_temp,
		dev_p->interval_switch_temp);
}

static ssize_t uscp_dbg_store(void *dev_data, const char *buf, size_t size)
{
	struct uscp_device_info *dev_p = (struct uscp_device_info *)dev_data;
	unsigned int uscp_tusb = 0;
	unsigned int open_temp = 0;
	unsigned int close_temp = 0;
	unsigned int switch_temp = 0;

	if (!buf || !dev_p) {
		hwlog_err("buf or dev_p is null\n");
		return -EINVAL;
	}

	/* 4: four parameters */
	if (sscanf(buf, "%d %d %d %d",
		&uscp_tusb, &open_temp, &close_temp, &switch_temp) != 4) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	dev_p->uscp_threshold_tusb = uscp_tusb;
	dev_p->open_mosfet_temp = open_temp;
	dev_p->close_mosfet_temp = close_temp;
	dev_p->interval_switch_temp = switch_temp;

	hwlog_info("uscp_threshold_tusb=%d\n", dev_p->interval_switch_temp);
	hwlog_info("open_mosfet_temp=%d\n", dev_p->open_mosfet_temp);
	hwlog_info("close_mosfet_temp=%d\n", dev_p->close_mosfet_temp);
	hwlog_info("interval_switch_temp=%d\n", dev_p->interval_switch_temp);

	return size;
}
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

static void uscp_wake_lock(void)
{
	if (!uscp_wakelock.active) {
		hwlog_info("wake lock\n");
		__pm_stay_awake(&uscp_wakelock);
	}
}

static void uscp_wake_unlock(void)
{
	if (uscp_wakelock.active) {
		hwlog_info("wake unlock\n");
		__pm_relax(&uscp_wakelock);
	}
}

static void uscp_charge_type_handler(struct uscp_device_info *di,
	enum hisi_charger_type type)
{
	int interval;

	if (!protect_enable)
		return;

	if ((type == CHARGER_TYPE_DCP) || (type == CHARGER_TYPE_UNKNOWN) ||
		(type == CHARGER_TYPE_SDP) || (type == CHARGER_TYPE_CDP)) {
		if (hrtimer_active(&(di->timer))) {
			hwlog_info("timer already armed, do nothing\n");
		} else {
			hwlog_info("start uscp check\n");
			interval = 0;
			first_in = 1;
			/*
			 * record 30 seconds after the charger just insert
			 * 30s = (1100 - 1001 + 1) * 300ms
			 * keep check count init number 1100
			 */
			di->keep_check_cnt = CHECK_CNT_INIT;
			hrtimer_start(&di->timer,
				ktime_set(interval / MSEC_PER_SEC,
				(interval % MSEC_PER_SEC) * USEC_PER_SEC),
				HRTIMER_MODE_REL);
		}
	} else {
		hwlog_info("charge_type=%d, do nothing\n", type);
	}
}

static int uscp_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct uscp_device_info *di = NULL;
	enum hisi_charger_type type = (enum hisi_charger_type)event;

	if (!nb) {
		hwlog_err("nb is null\n");
		return NOTIFY_OK;
	}

	di = container_of(nb, struct uscp_device_info, usb_nb);
	if (!di) {
		hwlog_err("di is null\n");
		return NOTIFY_OK;
	}

	uscp_charge_type_handler(di, type);

	return NOTIFY_OK;
}

#ifdef CONFIG_TCPC_CLASS
static int uscp_pd_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	return uscp_notifier_call(nb, event, data);
}
#endif /* CONFIG_TCPC_CLASS */

static int uscp_usb_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	return uscp_notifier_call(nb, event, data);
}

static void uscp_set_interval(struct uscp_device_info *di, int tdiff)
{
	hwlog_info("tdiff=%d, switch_temp=%d, interval=%d, cnt=%d\n",
		tdiff, di->interval_switch_temp,
		di->check_interval, di->keep_check_cnt);

	if (tdiff > di->interval_switch_temp) {
		di->check_interval = CHECK_INTERVAL_FAST;
		di->keep_check_cnt = 0;
		return;
	}

	if (di->keep_check_cnt > CHECK_CNT_LIMIT) {
		/*
		 * check the temperature per 0.3 second for 100 times,
		 * when the charger just insert.
		 */
		di->keep_check_cnt -= 1;
		di->check_interval = CHECK_INTERVAL_FAST;
		is_uscp_mode = 0;
	} else if (di->keep_check_cnt == CHECK_CNT_LIMIT) {
		/* reset the flag when the temperature status is stable */
		di->keep_check_cnt = -1;
		/* set the check interval 10000ms */
		di->check_interval = CHECK_INTERVAL_SLOW;
		is_uscp_mode = 0;
		uscp_wake_unlock();
	} else if (di->keep_check_cnt >= 0) {
		di->keep_check_cnt = di->keep_check_cnt + 1;
		/* set the check interval 300ms */
		di->check_interval = CHECK_INTERVAL_FAST;
	} else {
		/* set the check interval 10000ms */
		di->check_interval = CHECK_INTERVAL_SLOW;
		is_uscp_mode = 0;
	}
}

static void uscp_protection_process(struct uscp_device_info *di,
	int tbatt, int tusb)
{
#ifdef CONFIG_DIRECT_CHARGER
	int ret;
	int state;
#endif /* CONFIG_DIRECT_CHARGER */
	int tdiff;

	if (!uscp_enable) {
		hwlog_info("current uscp is disabled\n");
		return;
	}

	tdiff = tusb - tbatt;

	/* uscp happened: enable charge hiz */
	if ((tusb >= di->uscp_threshold_tusb) && (tdiff >= di->open_hiz_temp)) {
		is_hiz_mode = HIZ_MODE;
		hwlog_err("enable charge hiz\n");
		charge_set_hiz_enable(HIZ_MODE_ENABLE);
	}

	/* uscp happened: pull up mosfet */
	if (((tusb >= di->uscp_threshold_tusb) &&
		(tdiff >= di->open_mosfet_temp)) || uscp_enforce) {
		uscp_wake_lock();
		is_uscp_mode = 1;
		is_rt_uscp_mode = 1;
		if (uscp_enforce)
			hwlog_err("force uscp work\n");

#ifdef CONFIG_DIRECT_CHARGER
		direct_charge_set_stop_charging_flag(1);

		/* wait until direct charger stop complete */
		while (1) {
			state = direct_charge_get_stage_status();
			if (direct_charge_get_stop_charging_complete_flag() &&
				((state == DC_STAGE_DEFAULT) ||
				(state == DC_STAGE_CHARGE_DONE)))
				break;
		}

		direct_charge_set_stop_charging_flag(0);

		if (first_in) {
			if (state == DC_STAGE_DEFAULT) {
				if (direct_charge_detect_adapter_again())
					is_scp_charger = 0;
				else
					is_scp_charger = 1;
			} else if (state == DC_STAGE_CHARGE_DONE) {
				is_scp_charger = 1;
			}
			first_in = 0;
		}

		if (is_scp_charger) {
			/* close charging path: disable direct charger */
			ret = direct_charge_set_adapter_output_enable(0);
			if (!ret) {
				hwlog_err("disable adapter output success\n");
				is_scp_charger = 0;
				msleep(SLEEP_200MS);
			} else {
				hwlog_err("disable adapter output fail\n");
			}
		}
#endif /* CONFIG_DIRECT_CHARGER */

		/* close charging path: open mosfet */
		msleep(SLEEP_10MS);
		gpio_set_value(di->gpio_uscp, 1);
		hwlog_err("pull up mosfet\n");
	} else if (tdiff <= di->close_mosfet_temp) {
		if (is_uscp_mode) {
			/* open charging path: close mosfet and disable hiz */
			gpio_set_value(di->gpio_uscp, 0);
			is_rt_uscp_mode = 0;
			msleep(SLEEP_10MS);
			charge_set_hiz_enable(HIZ_MODE_DISABLE);
			is_hiz_mode = NOT_HIZ_MODE;
			hwlog_info("disable charge hiz and pull down mosfet\n");
		}

		if (is_hiz_mode) {
			/* open charging path: disable hiz */
			charge_set_hiz_enable(HIZ_MODE_DISABLE);
			is_hiz_mode = NOT_HIZ_MODE;
			hwlog_info("disable charge hiz\n");
		}
	}
}

static void uscp_check_temperature(struct uscp_device_info *di)
{
	int tusb;
	int tbatt = 0;
	int tdiff;
	int batt_id;
	char buf1[POWER_DSM_BUF_SIZE_0128] = { 0 };
	char buf2[POWER_DSM_BUF_SIZE_0128] = { 0 };

	tusb = power_temp_get_average_value(POWER_TEMP_USB_PORT) / POWER_TEMP_UNIT;
	usb_temp = tusb;
	huawei_battery_temp(BAT_TEMP_MIXED, &tbatt);
	tdiff = tusb - tbatt;
	batt_id = hisi_battery_id_voltage();

	hwlog_info("tusb=%d, tbatt=%d, tdiff=%d\n", tusb, tbatt, tdiff);

	if ((tusb >= di->uscp_threshold_tusb) &&
		(tdiff >= di->open_hiz_temp)) {
		if (di->dmd_hiz_enable && protect_dmd_notify_enable_hiz) {
			snprintf(buf1, POWER_DSM_BUF_SIZE_0128 - 1,
				"uscp happened(ap), open hiz, tusb=%d,tbatt=%d\n",
				tusb, tbatt);
			hwlog_info("%s\n", buf1);

			if (power_dsm_dmd_report(POWER_DSM_USCP,
				ERROR_NO_USB_SHORT_PROTECT_HIZ, buf1) == 0)
				protect_dmd_notify_enable_hiz = 0; /* disable */
		}
	}

	if ((tusb >= di->uscp_threshold_tusb) &&
		(tdiff >= di->open_mosfet_temp)) {
		is_rt_uscp_mode = 1;
		if (protect_dmd_notify_enable) {
			snprintf(buf2, POWER_DSM_BUF_SIZE_0128 - 1,
				"uscp happened(ap), tusb=%d,tbatt=%d,batt_id=%d\n",
				tusb, tbatt, batt_id);
			hwlog_info("%s\n", buf2);

			if (power_dsm_dmd_report(POWER_DSM_USCP,
				ERROR_NO_USB_SHORT_PROTECT, buf2) == 0)
				protect_dmd_notify_enable = 0; /* disable */
		}
	}

	uscp_set_interval(di, tdiff);

	uscp_protection_process(di, tbatt, tusb);
}

int is_in_uscp_mode(void)
{
	return is_uscp_mode;
}

int is_uscp_hiz_mode(void)
{
	return is_hiz_mode;
}

int is_in_rt_uscp_mode(void)
{
	return is_rt_uscp_mode;
}

static void uscp_check_work(struct work_struct *work)
{
	struct uscp_device_info *di = NULL;
	int t;
	enum hisi_charger_type type = hisi_get_charger_type();

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct uscp_device_info, uscp_check_wk);
	if (!di) {
		hwlog_err("di is null\n");
		return;
	}

#ifdef CONFIG_DIRECT_CHARGER
	if ((di->keep_check_cnt == -1) && (type == CHARGER_TYPE_NONE) &&
		(direct_charge_get_stage_status() == DC_STAGE_DEFAULT))
#else
	if ((di->keep_check_cnt == -1) && (type == CHARGER_TYPE_NONE))
#endif /* CONFIG_DIRECT_CHARGER */
	{
		protect_dmd_notify_enable = 1; /* enable dmd notify */
		gpio_set_value(di->gpio_uscp, 0); /* close mosfet */
		di->keep_check_cnt = -1;
		di->check_interval = CHECK_INTERVAL_SLOW;
		is_uscp_mode = 0;
		di->keep_check_cnt = CHECK_CNT_INIT; /* check count init 1100 */
		first_in = 1;
#ifdef CONFIG_DIRECT_CHARGER
		is_scp_charger = 0;
#endif /* CONFIG_DIRECT_CHARGER */

		hwlog_info("charger removed, stop uscp checking\n");
		return;
	}

	uscp_check_temperature(di);

	t = di->check_interval;
	hrtimer_start(&di->timer,
		ktime_set(t / MSEC_PER_SEC, (t % MSEC_PER_SEC) * USEC_PER_SEC),
		HRTIMER_MODE_REL);
}

static enum hrtimer_restart uscp_timer_func(struct hrtimer *timer)
{
	struct uscp_device_info *di = NULL;

	if (!timer) {
		hwlog_err("timer is null\n");
		return HRTIMER_NORESTART;
	}

	di = container_of(timer, struct uscp_device_info, timer);
	if (!di) {
		hwlog_err("di is null\n");
		return HRTIMER_NORESTART;
	}

	queue_work(di->uscp_wq, &di->uscp_check_wk);

	return HRTIMER_NORESTART;
}

static void uscp_check_ntc(void)
{
	int temp;
#ifndef CONFIG_HLTHERM_RUNTEST
	int tbatt = INVALID_TEMP_VAL;
	int batt_id;
	char buf[POWER_DSM_BUF_SIZE_0128] = { 0 };
#endif /* CONFIG_HLTHERM_RUNTEST */

	temp = power_temp_get_average_value(POWER_TEMP_USB_PORT) / POWER_TEMP_UNIT;

	if (temp > CHECK_NTC_TEMP_MAX || temp < CHECK_NTC_TEMP_MIN) {
#ifndef CONFIG_HLTHERM_RUNTEST
		huawei_battery_temp(BAT_TEMP_MIXED, &tbatt);
		batt_id = hisi_battery_id_voltage();

		snprintf(buf, POWER_DSM_BUF_SIZE_0128 - 1,
			"usb ntc error happened, tusb=%d,tbatt=%d,batt_id=%d\n",
			temp, tbatt, batt_id);

		power_dsm_dmd_report(POWER_DSM_USCP,
			ERROR_NO_USB_SHORT_PROTECT_NTC, buf);
#endif /* CONFIG_HLTHERM_RUNTEST */

		hwlog_err("disable uscp when temp is invalid\n");
		protect_enable = 0;
	} else {
		hwlog_info("enable uscp\n");
		protect_enable = 1;
	}
}

static void uscp_check_battery(void)
{
	if (!is_hisi_battery_exist()) {
		hwlog_err("disable uscp when battery is not exist\n");
		protect_enable = 0;
	}
}

static int uscp_parse_dts(struct device_node *np,
	struct uscp_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"uscp_threshold_tusb", (u32 *)&di->uscp_threshold_tusb,
		DEFAULT_TUSB_THRESHOLD);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"open_mosfet_temp", &di->open_mosfet_temp, 0))
		return -EINVAL;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"open_hiz_temp", (u32 *)&di->open_hiz_temp,
		di->open_mosfet_temp);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"dmd_hiz_enable", (u32 *)&di->dmd_hiz_enable, DMD_HIZ_DISABLE);

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"close_mosfet_temp", (u32 *)&di->close_mosfet_temp, 0))
		return -EINVAL;

	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"interval_switch_temp", (u32 *)&di->interval_switch_temp, 0))
		return -EINVAL;

	if (power_gpio_config_output(np,
		"gpio_usb_short_circuit_protect", "uscp_ap", &di->gpio_uscp, 0))
		return -EINVAL;

	return 0;
}

static int uscp_probe(struct platform_device *pdev)
{
	struct device_node *np = NULL;
	struct uscp_device_info *di = NULL;
	enum hisi_charger_type type = hisi_get_charger_type();
	int ret;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_uscp_di = di;
	np = pdev->dev.of_node;

	ret = uscp_parse_dts(np, di);
	if (ret)
		goto fail_free_mem;

	is_uscp_mode = 0;
	is_rt_uscp_mode = 0;
	di->keep_check_cnt = CHECK_CNT_INIT;

	/* first: check ntc */
	uscp_check_ntc();

	/* second: check battery */
	uscp_check_battery();

	/* third: disable uscp when ntc and battery are invalid */
	if (!protect_enable)
		goto fail_free_mem;

	wakeup_source_init(&uscp_wakelock, "uscp_wakelock");
	di->uscp_wq = create_singlethread_workqueue("uscp_wq");
	INIT_WORK(&di->uscp_check_wk, uscp_check_work);
	hrtimer_init(&di->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	di->timer.function = uscp_timer_func;

#ifdef CONFIG_TCPC_CLASS
	if (is_pd_supported()) {
		di->tcpc_nb.notifier_call = uscp_pd_notifier_call;
		ret = register_pd_dpm_notifier(&di->tcpc_nb);
	} else {
		di->usb_nb.notifier_call = uscp_usb_notifier_call;
		ret = hisi_charger_type_notifier_register(&di->usb_nb);
	}
#else
	di->usb_nb.notifier_call = uscp_usb_notifier_call;
	ret = hisi_charger_type_notifier_register(&di->usb_nb);
#endif /* CONFIG_TCPC_CLASS */

	if (ret < 0) {
		hwlog_err("charger_type_notifier register failed\n");
		goto fail_free_wakelock;
	}

	uscp_charge_type_handler(di, type);

	platform_set_drvdata(pdev, di);

#ifdef CONFIG_HUAWEI_POWER_DEBUG
	power_dbg_ops_register("uscp_para", platform_get_drvdata(pdev),
		(power_dbg_show)uscp_dbg_show, (power_dbg_store)uscp_dbg_store);
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

	return 0;

fail_free_wakelock:
	wakeup_source_trash(&uscp_wakelock);
fail_free_mem:
	kfree(di);
	g_uscp_di = NULL;
	return ret;
}

#ifdef CONFIG_PM
static int uscp_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct uscp_device_info *di = platform_get_drvdata(pdev);

	if (!di)
		return 0;

	cancel_work_sync(&di->uscp_check_wk);
	hrtimer_cancel(&di->timer);

	return 0;
}

static int uscp_resume(struct platform_device *pdev)
{
	enum hisi_charger_type type = hisi_get_charger_type();
	struct uscp_device_info *di = platform_get_drvdata(pdev);

	if (!di)
		return 0;

	hwlog_info("charger_type=%d\n", type);
	if (type == CHARGER_TYPE_NONE)
		return 0;

	queue_work(di->uscp_wq, &di->uscp_check_wk);

	return 0;
}
#endif /* CONFIG_PM */

static const struct of_device_id uscp_match_table[] = {
	{
		.compatible = "huawei,usb_short_circuit_protect",
		.data = NULL,
	},
	{},
};

static struct platform_driver uscp_driver = {
	.probe = uscp_probe,
#ifdef CONFIG_PM
	/*
	 * depend on IPC driver
	 * so we set SR suspend/resume and IPC is suspend_late/early_resume
	 */
	.suspend = uscp_suspend,
	.resume = uscp_resume,
#endif /* CONFIG_PM */
	.driver = {
		.name = "huawei,usb_short_circuit_protect",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(uscp_match_table),
	},
};

static int __init uscp_init(void)
{
	return platform_driver_register(&uscp_driver);
}

static void __exit uscp_exit(void)
{
	platform_driver_unregister(&uscp_driver);
}

device_initcall_sync(uscp_init);
module_exit(uscp_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("usb port short circuit protect module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
