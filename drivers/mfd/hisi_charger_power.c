
/*
 * hisi_charger_power.c
 *
 * Device driver for SCHARGER POWER DRIVER
 *
 * Copyright (c) 2015-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * either version 2 of that License or (at your option) any later version.
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/of.h>
const static struct of_device_id of_hisi_scharger_power_child_match_tbl[] = {
	/* regulators */
	{
		.compatible = "hisilicon,hisi-regulator-scharger",
	},
	{ /* end */ }
};

const static struct of_device_id of_hisi_scharger_power_match_tbl[] = {
	{
		.compatible = "hisilicon,hisi-scharger-driver",
	},
	{ /* end */ }
};
static int hisi_scharger_power_probe(struct platform_device *pdev)
{
	struct device *dev = NULL;
	struct device_node *np = NULL;

	if (pdev == NULL) {
		printk(KERN_ERR"[%s] Pdev is null!\n", __func__);
		return -1;
	}
	dev = &pdev->dev;

	if (dev == NULL) {
		printk(KERN_ERR"[%s] dev is null!\n", __func__);
		return -1;
	}
	np = dev->of_node;

	/*
	 * populate sub nodes,system init only adapt father node.
	 */
	of_platform_populate(np, of_hisi_scharger_power_child_match_tbl,
		NULL, dev);

	return 0;
}

const static struct platform_driver scharger_power_driver = {
	.driver = {
		.name = "hisi_charger_power",
		.owner = THIS_MODULE,
		.of_match_table = of_hisi_scharger_power_match_tbl,
	},
	.probe = hisi_scharger_power_probe,
};

static int __init hisi_charger_power_init(void)
{
	return platform_driver_register(&scharger_power_driver);
}

static void __exit hisi_charger_power_exit(void)
{
	platform_driver_unregister(&scharger_power_driver);
}

fs_initcall(hisi_charger_power_init);
module_exit(hisi_charger_power_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("scharger power interface driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
