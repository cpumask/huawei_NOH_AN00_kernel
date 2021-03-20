/*
 * pcie2usb.c
 *
 * pcie2usb driver
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/usb.h>
#include <linux/hisi/usb/tca.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/hisi/pcie-kirin-api.h>
#include "pcie2usb.h"

static int g_wake_gpio = 0;

enum {
	PCIE_DEVICE_NONE = 0,
	PCIE_DEVICE_UPD720202 = 1,
};

enum {
	PCIE0 = 0,
	PCIE1 = 1,
};



static irqreturn_t pcie2usb_wakeup_ep_handler(int irq, void *dev_id)
{
	pcie2usb_info("PCIe wakeup endpoint rc");
	return IRQ_HANDLED;
}

int pcie2usb_register_wakeup_gpio(struct platform_device *pdev)
{
	int ret;
	int irqn;

	g_wake_gpio = of_get_named_gpio(pdev->dev.of_node, "wake-gpio", 0);
	if (g_wake_gpio < 0) {
		pcie2usb_err("no wake gpio exist");
		ret = g_wake_gpio;
		return ret;
	}
	ret = gpio_request(g_wake_gpio, "pcie_wake");
	if (ret) {
		pcie2usb_err("Failed to request gpio-%d", g_wake_gpio);
		return ret;
	}
	gpio_direction_input(g_wake_gpio);
	gpio_to_irq(0);
	irqn = gpio_to_irq(g_wake_gpio);
	if (irqn < 0) {
		ret = irqn;
		gpio_free(g_wake_gpio);
		return ret;
	}

	pcie2usb_info("requesting IRQ %d with IRQF_NO_SUSPEND\n", irqn);

	ret = request_irq(irqn, pcie2usb_wakeup_ep_handler,
	IRQF_TRIGGER_FALLING|IRQF_NO_SUSPEND, "PCIE_WAKE", NULL);
	if (ret) {
		pcie2usb_err("request_irq failed\n");
		return ret;
	}
	enable_irq_wake(irqn);

	pcie2usb_info("pcie2usb_register_wakeup_gpio success");
	return ret;
}


static int pcie2usb_probe(struct platform_device *pdev)
{
	int ret;
	int pcie1_device_type = 0;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	pcie2usb_info("pcie2usb probe ++\n");

	ret = of_property_read_u32(np, "pcie1_device_type", (u32 *)&pcie1_device_type);
	if (ret) {
		pcie2usb_err("get pcie1_device_type failed\n");
	}
	if (pcie1_device_type == PCIE_DEVICE_UPD720202) {
		kirin_pcie_enumerate(PCIE1);
	}

	pcie2usb_register_wakeup_gpio(pdev);

	pcie2usb_info("pcie2usb probe --\n");

	return ret;
}

static int pcie2usb_remove(struct platform_device *pdev)
{
	pcie2usb_info("pcie2usb remove\n");
	gpio_free(g_wake_gpio);

	return 0;
}

static const struct of_device_id pcie2usb_match[] = {
	{ .compatible = "huawei,pcie2usb" },
	{},
};


static struct platform_driver pcie2usb_driver = {
	.probe          = pcie2usb_probe,
	.remove         = pcie2usb_remove,
	.driver         = {
		.name   = "pcie2usb",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(pcie2usb_match),
		},
};

static int __init pcie2usb_init(void)
{
        return platform_driver_register(&pcie2usb_driver);
}

static void __exit pcie2usb_exit(void)
{
        platform_driver_unregister(&pcie2usb_driver);
}

late_initcall(pcie2usb_init);
module_exit(pcie2usb_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("huawei pcie2usb misc control module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
