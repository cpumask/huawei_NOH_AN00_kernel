/*
 * hw_pogopin_otg_id.c
 *
 * pogopin id driver
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
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/of_gpio.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/mfd/hisi_pmic.h>
#include <linux/hisi/hisi_adc.h>
#include <pmic_interface.h>
#include <chipset_common/hwpower/power_dts.h>
#include <huawei_platform/usb/hw_pogopin.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/usb/hw_pogopin_otg_id.h>
#include <chipset_common/hwpower/power_cmdline.h>

/* include platform head-file */
#if defined(CONFIG_DEC_USB)
#include "dwc_otg_dec.h"
#include "dwc_otg_cil.h"
#endif

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif

#define HWLOG_TAG hw_pogopin_id
HWLOG_REGIST();

static struct pogopin_otg_id_dev *g_pogopin_otg_id_di;
static int g_startup_otg_sample;

static int pogopin_event_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	int pogopin_current_status = pogopin_get_interface_status();

	hwlog_info("current_int_status=%d\n", pogopin_current_status);

	switch (event) {
	case CHARGER_TYPE_NONE:
		hwlog_info("off mode\n");
		if ((pogopin_current_status == TYPEC_INTERFACE) ||
			(pogopin_current_status == POGOPIN_AND_TYPEC))
			pogopin_5pin_completion_devoff();
		msleep(SLEEP_50MS);
		break;
	case CHARGER_TYPE_SDP:
	case CHARGER_TYPE_CDP:
	case CHARGER_TYPE_DCP:
	case CHARGER_TYPE_UNKNOWN:
		hwlog_info("charger mode\n");
		break;
	case PLEASE_PROVIDE_POWER:
		hwlog_info("otg mode\n");
		break;
	default:
		hwlog_info("ignore other types %ld\n", event);
		break;
	}

	return NOTIFY_OK;
}

static int pogopin_otg_status_check_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct pogopin_otg_id_dev *di = g_pogopin_otg_id_di;

	if (!di) {
		hwlog_err("di is null\n");
		return NOTIFY_OK;
	}

	switch (event) {
	case POGOPIN_CHARGER_OUT_COMPLETE:
		if (gpio_get_value(di->gpio) == LOW)
			schedule_work(&di->otg_intb_work);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int pogopin_otg_id_adc_sampling(struct pogopin_otg_id_dev *di)
{
	int avgvalue;
	int vol_value;
	int sum = 0;
	int i;
	int sample_cnt = 0;

	if (di->otg_adc_channel == OTG_INVALID_ADC) {
		hwlog_info("get otg_adc_channel fail\n");
		return FAIL;
	}

	for (i = 0; i < VBATT_AVR_MAX_COUNT; i++) {
		vol_value = hisi_adc_get_value(di->otg_adc_channel);
		/* add some interval */
		if ((di->sampling_time_optimize == SAMPLING_OPTIMIZE_FLAG) &&
			(g_startup_otg_sample == SAMPLE_DOING))
			udelay(SMAPLING_OPTIMIZE);
		else
			msleep(SAMPLING_INTERVAL);

		if (vol_value < 0) {
			hwlog_info("the value from adc is error\n");
			continue;
		}

		sum += vol_value;
		sample_cnt++;
	}

	if (sample_cnt == 0) {
		/* ESD cause the sample is always negative */
		avgvalue = ADC_VOLTAGE_NEGATIVE;
	} else {
		avgvalue = sum / sample_cnt;
	}

	hwlog_info("the average voltage of adc is %d\n", avgvalue);

	return avgvalue;
}

static void pogopin_otg_in_handle(struct pogopin_otg_id_dev *di)
{
	int pogopin_current_int_status = pogopin_get_interface_status();
	unsigned long timeout;

	hwlog_info("pogopin otg insert cutoff cc,wait data and power switch\n");

	/* disable cc to remove typec devices */
	if (!pogopin_typec_chg_ana_audio_suport() ||
		(pogopin_typec_chg_ana_audio_suport() &&
		!pogopin_5pin_get_ana_audio_status()))
		pogopin_5pin_typec_detect_disable(TRUE);
	pogopin_5pin_reinit_completion_devoff();
	if (pogopin_current_int_status == TYPEC_INTERFACE)
		timeout = pogopin_5pin_wait_for_completion(TIMEOUT_1000MS);
	else
		timeout = pogopin_5pin_wait_for_completion(TIMEOUT_200MS);
	pogopin_5pin_set_pogo_status(POGO_OTG);
	hwlog_info("start pogopin otg,wait timeout =%ld\n", timeout);
	/* enable cc to detect typec pd and ana audio event */
	if (pogopin_typec_chg_ana_audio_suport()) {
		if (!pogopin_5pin_get_ana_audio_status())
			pogopin_5pin_typec_detect_disable(false);
	}
	pogopin_5pin_otg_in_switch_from_typec();
}

static void pogopin_otg_out_handle(struct pogopin_otg_id_dev *di)
{
	unsigned long timeout;

	hwlog_info("pogopin otg out,switch to typec, wait\n");
	pogopin_5pin_remove_switch_to_typec();
	pogopin_5pin_set_pogo_status(POGO_NONE);
	/* disable and enable cc to detect typec event */
	if (pogopin_typec_chg_ana_audio_suport()) {
		if (!pogopin_5pin_get_ana_audio_status()) {
			/* and set charger type remove when pogo remove otg */
			if ((pogopin_get_interface_status() ==
				TYPEC_INTERFACE) ||
				(pogopin_get_interface_status() ==
				POGOPIN_AND_TYPEC))
				pogopin_event_notify(POGOPIN_OTG_AND_CHG_STOP);
			pogopin_5pin_typec_detect_disable(true);
			msleep(POGOPIN_TIME_DELAYE_300MS);
		}
	}
	timeout = pogopin_5pin_wait_for_completion(TIMEOUT_800MS);

	hwlog_info("enable typec detect,wait timeout = %ld\n", timeout);
	if (!pogopin_typec_chg_ana_audio_suport() ||
		(pogopin_typec_chg_ana_audio_suport() &&
		!pogopin_5pin_get_ana_audio_status()))
		pogopin_5pin_typec_detect_disable(FALSE);
}

static void pogopin_otg_id_intb_work(struct work_struct *work)
{
	int gpio_value;
	int avgvalue;
	struct pogopin_otg_id_dev *di = NULL;

	if (!work) {
		hwlog_err("work is null\n");
		return;
	}

	di = container_of(work, struct pogopin_otg_id_dev, otg_intb_work);
	if (!di || (pogopin_is_charging() &&
		pogopin_typec_chg_ana_audio_suport())) {
		hwlog_err("di is null, or pogo charging ignore otg event\n");
		return;
	}

	hwlog_info("pogopin otg int work\n");

	gpio_value = gpio_get_value(di->gpio);
	if (gpio_value == LOW) {
		avgvalue = pogopin_otg_id_adc_sampling(di);
		if ((avgvalue >= 0) && (avgvalue <= ADC_VOLTAGE_LIMIT)) {
			pogopin_otg_in_handle(di);
			hisi_usb_otg_event(ID_FALL_EVENT);
			if (pogopin_typec_chg_ana_audio_suport())
				pogopin_otg_status_change_process(POGO_OTG);
		} else {
			hwlog_info("avgvalue is %d\n", avgvalue);
		}
		pogopin_5pin_reinit_completion_devoff();
	} else {
		hisi_usb_otg_event(ID_RISE_EVENT);
		pogopin_otg_out_handle(di);
		pogopin_5pin_reinit_completion_devoff();
		pogopin_get_interface_status();
		if (pogopin_typec_chg_ana_audio_suport())
			pogopin_otg_status_change_process(POGO_NONE);
	}
}

static irqreturn_t pogopin_otg_id_irq_handle(int irq, void *dev_id)
{
	int gpio_value;
	struct pogopin_otg_id_dev *di = g_pogopin_otg_id_di;

	if (!di) {
		hwlog_err("di is null\n");
		return IRQ_HANDLED;
	}

	disable_irq_nosync(di->irq);
	pogopin_5pin_int_irq_disable(TRUE);
	gpio_value = gpio_get_value(di->gpio);

	if (gpio_value == LOW)
		irq_set_irq_type(di->irq, IRQF_TRIGGER_HIGH | IRQF_NO_SUSPEND);
	else
		irq_set_irq_type(di->irq, IRQF_TRIGGER_LOW | IRQF_NO_SUSPEND);

	pogopin_5pin_int_irq_disable(FALSE);
	enable_irq(di->irq);
	schedule_work(&di->otg_intb_work);

	return IRQ_HANDLED;
}

static int pogopin_otg_id_parse_dts(struct pogopin_otg_id_dev *di,
	struct device_node *np)
{
	di->gpio = of_get_named_gpio(np, "otg-gpio", 0);
	hwlog_info("otg_gpio=%d\n", di->gpio);

	if (!gpio_is_valid(di->gpio)) {
		hwlog_err("gpio is not valid\n");
		return -EINVAL;
	}

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"sampling_time_optimize", &di->sampling_time_optimize, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"fpga_flag", &di->fpga_flag, 0);
	if (power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"otg_adc_channel", &di->otg_adc_channel, 0))
		return -EINVAL;

	return 0;
}

static int pogopin_otg_id_irq_init(struct pogopin_otg_id_dev *di)
{
	int ret = -EINVAL;

	di->irq = gpio_to_irq(di->gpio);
	if (di->irq < 0) {
		hwlog_err("gpio map to irq fail\n");
		return ret;
	}

	ret = request_irq(di->irq, pogopin_otg_id_irq_handle,
		IRQF_TRIGGER_LOW | IRQF_NO_SUSPEND | IRQF_ONESHOT,
		"otg_gpio_irq", NULL);
	if (ret < 0)
		hwlog_err("gpio irq request fail\n");
	else
		di->otg_irq_enabled = TRUE;

	return ret;
}

static int pogopin_otg_id_probe(struct platform_device *pdev)
{
	int ret;
	int avgvalue;
	struct device_node *np = NULL;
	struct device *dev = NULL;
	struct pogopin_otg_id_dev *di = NULL;

	if (power_cmdline_is_powerdown_charging_mode()) {
		hwlog_err("in charger mode, exit pogopin otg probe\n");
		return -EINVAL;
	}

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	g_pogopin_otg_id_di = di;
	platform_set_drvdata(pdev, di);
	di->pdev = pdev;
	dev = &pdev->dev;
	np = dev->of_node;

	di->pogopin_nb.notifier_call = pogopin_event_notifier_call;
	ret = hisi_charger_type_notifier_register(&di->pogopin_nb);
	if (ret < 0) {
		hwlog_err("charger_type_notifier register failed\n");
		goto fail_register_charge_notifier;
	}

	di->pogopin_otg_status_check_nb.notifier_call =
		pogopin_otg_status_check_notifier_call;
	ret = pogopin_event_notifier_register(&di->pogopin_otg_status_check_nb);
	if (ret < 0) {
		hwlog_err("pogopin otg_notifier register failed\n");
		goto fail_register_otg_check_notifier;
	}

	ret = pogopin_otg_id_parse_dts(di, np);
	if (ret != 0)
		goto fail_parse_dts;

	/* init otg gpio process */
	ret = gpio_request(di->gpio, "otg_gpio_irq");
	if (ret < 0) {
		hwlog_err("gpio request fail\n");
		goto fail_request_gpio;
	}

	avgvalue = pogopin_otg_id_adc_sampling(di);
	g_startup_otg_sample = SAMPLE_DONE;
	if ((avgvalue > ADC_VOLTAGE_LIMIT) && (avgvalue <= ADC_VOLTAGE_MAX)) {
		hwlog_info("set gpio output, avgvalue is %d\n", avgvalue);
		ret = gpio_direction_output(di->gpio, HIGH);
	} else {
		ret = gpio_direction_input(di->gpio);
		if (ret < 0) {
			hwlog_err("gpio set input fail\n");
			goto fail_set_gpio_direction;
		}
	}

	ret = pogopin_otg_id_irq_init(di);
	if (ret != 0)
		goto fail_request_irq;

	/* init otg intr handle work function */
	INIT_WORK(&di->otg_intb_work, pogopin_otg_id_intb_work);

	/* check the otg status when the phone poweron */
	/* call work function to handle irq */
	ret = gpio_get_value(di->gpio);
	if (ret == 0)
		schedule_work(&di->otg_intb_work);

	return 0;

fail_request_irq:
fail_set_gpio_direction:
	gpio_free(di->gpio);
fail_request_gpio:
fail_parse_dts:
	pogopin_event_notifier_unregister(&di->pogopin_otg_status_check_nb);
fail_register_otg_check_notifier:
	hisi_charger_type_notifier_unregister(&di->pogopin_nb);
fail_register_charge_notifier:
	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, di);
	g_pogopin_otg_id_di = NULL;

	return ret;
}

static int pogopin_otg_id_remove(struct platform_device *pdev)
{
	struct pogopin_otg_id_dev *di = g_pogopin_otg_id_di;

	if (!di)
		return -ENODEV;

	hisi_charger_type_notifier_unregister(&di->pogopin_nb);
	free_irq(di->irq, pdev);
	gpio_free(di->gpio);
	platform_set_drvdata(pdev, NULL);
	devm_kfree(&pdev->dev, di);
	g_pogopin_otg_id_di = NULL;

	return 0;
}

static const struct of_device_id pogopin_otg_id_of_match[] = {
	{
		.compatible = "huawei,pogopin-otg-by-id",
	},
	{},
};

static struct platform_driver pogopin_otg_id_drv = {
	.probe = pogopin_otg_id_probe,
	.remove = pogopin_otg_id_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "pogo_otg_id",
		.of_match_table = pogopin_otg_id_of_match,
	},
};

static int __init pogopin_otg_id_init(void)
{
	return platform_driver_register(&pogopin_otg_id_drv);
}

static void __exit pogopin_otg_id_exit(void)
{
	platform_driver_unregister(&pogopin_otg_id_drv);
}

late_initcall(pogopin_otg_id_init);
module_exit(pogopin_otg_id_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("POGOPIN OTG connection/disconnection driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
