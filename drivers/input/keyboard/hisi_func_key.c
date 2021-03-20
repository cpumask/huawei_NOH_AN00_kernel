/*
 * func key driver
 *
 * Copyright (C) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "hisi_gpio_key.h"

#define HISI_FUNC_KEY_NAME_STR "hisi_func_key"
#define HISI_FUNC_KEY_NODE_NAME_STR "gpio-keyfunc,gpio-irq"
#define HISI_FUNC_KEY_WAKE_NAME_STR "func_key_wake_lock"
#define HISI_FUNC_KEY_GPIO_TO_IRQ_NAME_STR "gpio_func"

static void report_key(struct hisi_key_devices *hisi_key_dev,
	struct hisi_key_common_data *g_key_common_data,
	unsigned int report_action)
{
#ifdef CONFIG_KEY_MASK
	/* make sure report in pair */
	if (g_key_common_data->key_mask_ctl == KEY_DISABLE_MASK)
		g_key_common_data->key_mask_flag = KEY_DISABLE_MASK;
	else if (g_key_common_data->key_mask_ctl == KEY_ENABLE_MASK)
		g_key_common_data->key_mask_flag = KEY_ENABLE_MASK;

	if (g_key_common_data->key_mask_flag == KEY_ENABLE_MASK) {
#endif
		pr_info("[funckey]func key %d action %u\n",
			KEY_F26, report_action);
		input_report_key(hisi_key_dev->input_dev,
			KEY_F26, report_action);
		input_sync(hisi_key_dev->input_dev);
#ifdef CONFIG_KEY_MASK
	} else {
		pr_info("[funckey]%s key is disabled %u\n",
			hisi_key_dev->devices_names, report_action);
	}
#endif
}

static void hisi_func_key_work(struct work_struct *work)
{
	struct hisi_key_devices *hisi_key_dev =
		container_of(work, struct hisi_key_devices, gpio_key_work.work);
	struct hisi_key_common_data *g_key_common_data = NULL;
	unsigned int key_func_value;
	unsigned int report_action = GPIO_KEY_RELEASE;

	key_func_value = gpio_get_value(
		(unsigned int)hisi_key_dev->hisi_key_num);
	/* judge key is pressed or released. */
	if (key_func_value == GPIO_LOW_VOLTAGE) {
		report_action = GPIO_KEY_PRESS;
		__pm_stay_awake(&hisi_key_dev->gpio_key_lock);
	} else if (key_func_value == GPIO_HIGH_VOLTAGE) {
		report_action = GPIO_KEY_RELEASE;
	} else {
		pr_err("[funckey][%s]invalid gpio key_func\n", __func__);
		return;
	}

	g_key_common_data = hisi_get_common_data();

	report_key(hisi_key_dev, g_key_common_data, report_action);

	if (key_func_value == GPIO_HIGH_VOLTAGE)
		__pm_relax(&hisi_key_dev->gpio_key_lock);

}

static irqreturn_t hisi_gpio_func_key_irq_handler(int irq, void *dev_id)
{
	struct hisi_key_devices *hisi_key_dev =
		(struct hisi_key_devices *)dev_id;

	__pm_stay_awake(&hisi_key_dev->gpio_key_lock);
	queue_delayed_work(system_power_efficient_wq,
		&hisi_key_dev->gpio_key_work, TIMER_DEBOUNCE);

	return IRQ_HANDLED;
}

static void hisi_func_key_set_bit(struct hisi_key_devices *hisi_key_dev)
{
	set_bit(KEY_F26, hisi_key_dev->input_dev->keybit);
}

static int hisi_func_key_req_irq(struct hisi_key_devices *hisi_key_dev)
{
	int err;

	err = request_irq(hisi_key_dev->hisi_key_irq,
		hisi_gpio_func_key_irq_handler,
		IRQF_NO_SUSPEND |
		IRQF_TRIGGER_RISING |
		IRQF_TRIGGER_FALLING,
		hisi_key_dev->pdev->name, hisi_key_dev);
	if (err)
		return err;

	return 0;
}

static const struct of_device_id hs_func_key_match[] = {
	{.compatible = "hisilicon,func-key"}, {},
};
MODULE_DEVICE_TABLE(of, hs_func_key_match);

static int hisi_func_key_probe(struct platform_device *pdev)
{
	struct hisi_key_devices *hisi_func_key = NULL;
	int ret;

	dev_info(&pdev->dev, "%s\n", __func__);

	ret = hisi_gpio_key_get_of_node(hs_func_key_match, pdev);
	if (ret) {
		pr_err("hisi_gpio__volume_key_get_of_node fail\n");
		return -EINVAL;
	}

	hisi_func_key = devm_kzalloc(&pdev->dev,
		sizeof(*hisi_func_key), GFP_KERNEL);
	if (!hisi_func_key)
		return -ENOMEM;

	hisi_func_key->devices_names = HISI_FUNC_KEY_NAME_STR;
	hisi_func_key->key_node_name = HISI_FUNC_KEY_NODE_NAME_STR;
	hisi_func_key->work_init_name = HISI_FUNC_KEY_WAKE_NAME_STR;
	hisi_func_key->gpio_to_irq_name = HISI_FUNC_KEY_GPIO_TO_IRQ_NAME_STR;

	hisi_func_key->key_set_bit = hisi_func_key_set_bit;
	hisi_func_key->key_work = hisi_func_key_work;
	hisi_func_key->hisi_key_req_irq = hisi_func_key_req_irq;

	hisi_func_key->pdev = pdev;

	ret = hisi_key_register_dev(hisi_func_key);
	if (ret) {
		pr_err("%s register fail\n", hisi_func_key->devices_names);
		return -EINVAL;
	}

	platform_set_drvdata(pdev, hisi_func_key);
	dev_info(&pdev->dev, "%s success\n", __func__);
	return 0;
}

static int hisi_func_key_remove(struct platform_device *pdev)
{
	struct hisi_key_devices *hisi_func_key = platform_get_drvdata(pdev);

	if (!hisi_func_key) {
		pr_err("%s:failed to get drvdata\n", __func__);
		return -EINVAL;
	}

	hisi_key_unregister_dev(hisi_func_key);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int hisi_gpio_func_key_suspend(
	struct platform_device *pdev, pm_message_t state)
{
	dev_info(&pdev->dev, "%s: suspend +\n", __func__);
	dev_info(&pdev->dev, "%s: suspend -\n", __func__);

	return 0;
}

static int hisi_gpio_func_key_resume(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "%s: resume +\n", __func__);
	dev_info(&pdev->dev, "%s: resume -\n", __func__);

	return 0;
}
#endif

struct platform_driver hisi_func_key_driver = {
	.probe = hisi_func_key_probe,
	.remove = hisi_func_key_remove,
	.driver = {
		.name = "hisi_func_key",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hs_func_key_match),
	},
#ifdef CONFIG_PM
	.suspend = hisi_gpio_func_key_suspend,
	.resume = hisi_gpio_func_key_resume,
#endif
};

module_platform_driver(hisi_func_key_driver);

MODULE_DESCRIPTION("Hisilicon func key platform driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
