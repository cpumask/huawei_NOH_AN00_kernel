/*
 * hisi_ip_core.h
 *
 * driver for hisi ip regulator core
 *
 * Copyright (c) 2016-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mfd/hisi_ip_core.h>

static DEFINE_MUTEX(reg_lock_mutex);

static const struct of_device_id of_hisi_regulator_ip_core_child_match_tbl[] = {
	/* regulators */
	{
		.compatible = "ip-regulator-atf",
	},
	{
		.compatible = "ip-regulator-lpm",
	},
	{ /* end */ }
};

static const struct of_device_id of_hisi_regulator_ip_core_match_tbl[] = {
	{
		.compatible = "hisilicon,hisi_regulator_ip_atf_core",
	},
	{ /* end */ }
};

static int hisi_regulator_ip_core_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct hisi_regulator_ip_core *pmic = NULL;

	pmic = devm_kzalloc(dev, sizeof(*pmic), GFP_KERNEL);
	if (!pmic) {
		dev_err(dev, "cannot allocate hisi_regulator_ip_core device info\n");
		return -ENOMEM;
	}

	/* get resources */
	pmic->res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!pmic->res) {
		dev_err(dev, "platform_get_resource err !\n");
		return -ENOENT;
	}

	if (!devm_request_mem_region(dev, pmic->res->start,
		resource_size(pmic->res), pdev->name)) {
		dev_err(dev, "cannot claim register memory\n");
		return -ENOMEM;
	}

	pmic->regs = devm_ioremap(dev, pmic->res->start,
		resource_size(pmic->res));
	if (!pmic->regs) {
		dev_err(dev, "cannot map register memory\n");
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, pmic);

	of_platform_populate(np, of_hisi_regulator_ip_core_child_match_tbl,
		NULL, dev);

	return 0;

}

static int hisi_regulator_ip_core_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct hisi_regulator_ip_core *pmic = platform_get_drvdata(pdev);

	devm_iounmap(dev, pmic->regs);
	devm_release_mem_region(dev, pmic->res->start,
		resource_size(pmic->res));
	devm_kfree(dev, pmic);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

static struct platform_driver hisi_regulator_ip_core_driver = {
	.driver = {
		.name	= "hisi_regulator_ip_core",
		.owner  = THIS_MODULE,
		.of_match_table = of_hisi_regulator_ip_core_match_tbl,
	},
	.probe	= hisi_regulator_ip_core_probe,
	.remove	= hisi_regulator_ip_core_remove,
};

static int __init hisi_regulator_ip_core_init(void)
{
	return platform_driver_register(&hisi_regulator_ip_core_driver);
}
fs_initcall(hisi_regulator_ip_core_init);

static void __exit hisi_regulator_ip_core_exit(void)
{
	platform_driver_unregister(&hisi_regulator_ip_core_driver);
}
module_exit(hisi_regulator_ip_core_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hisi regulator ip driver");
MODULE_AUTHOR("Zhiliang Xue <xuezhiliang@hisilicon.com>");
