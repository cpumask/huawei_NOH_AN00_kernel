/*
 * hisi-clk-intr.c
 *
 * PLL unlock intr driver support
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
 */
#include "clk.h"
#include <linux/version.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/mod_devicetable.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kern_levels.h>
#include <linux/hisi/rdr_hisi_platform.h>
#include <soc_crgperiph_interface.h>
#include <linux/hisi/hisi_bbox_diaginfo.h>

#define hipll_err(fmt, args ...) pr_err("[PLL_INTR]"fmt, ##args)

#define hipll_info(fmt, args ...) pr_info("[PLL_INTR]"fmt, ##args)

#define hipll_debug(fmt, args ...) pr_debug("[PLL_INTR]"fmt, ##args)

/* used for the register that high 16bit is mask */
#define HM_EN(n) (0x10001U << (n))
#define HM_DIS(n) (0x10000U << (n))

struct hipll_device {
	u32 intr_clear;
	u32 intr_bypass;
	u32 intr_stat;
	u32 pll_irq;
	u32 en_bit;
	u32 clr_bit;
	u32 status;
	u32 intr_val;
	u32 base_id;
	struct platform_device *pdev;
	void __iomem *base_addr;
};

static irqreturn_t hisi_pll_unlock_interrupt(int irq, void *p)
{
	u32 pll_lock_stat;
	struct hipll_device *hisi_pll = (struct hipll_device *)p;

	pll_lock_stat = (unsigned int)readl(hisi_pll->intr_stat + hisi_pll->base_addr);
	hipll_err("%s Pll UnLock Status : 0x%x!\n", __func__, pll_lock_stat);

	disable_irq_nosync(hisi_pll->pll_irq);

	/* user verison use DMD; debug version panic directly */
#ifdef CONFIG_HISI_CLK_DEBUG
	rdr_syserr_process_for_ap(MODID_AP_S_PANIC_PLL_UNLOCK, 0ULL, 0ULL);
#else
	bbox_diaginfo_record(CLOCK_PLL_AP, NULL, "pll unlock status : 0x%x", pll_lock_stat);
#endif
	return IRQ_HANDLED;
}

static void pll_unlock_en(struct hipll_device *hisi_pll)
{
	writel(HM_DIS(hisi_pll->en_bit),
		hisi_pll->intr_clear + hisi_pll->base_addr);

	writel(hisi_pll->intr_val,
		hisi_pll->intr_bypass + hisi_pll->base_addr);

	writel(HM_EN(hisi_pll->clr_bit),
		hisi_pll->intr_clear + hisi_pll->base_addr);

	udelay(2);

	writel(HM_DIS(hisi_pll->clr_bit),
		hisi_pll->intr_clear + hisi_pll->base_addr);

	writel(HM_EN(hisi_pll->en_bit),
		hisi_pll->intr_clear + hisi_pll->base_addr);
}

static int hisipll_init(struct device_node *node, struct hipll_device *hisi_pll)
{
	int ret;

	ret = of_property_read_u32(node, "pll_unlock_detect_status", (u32 *)&hisi_pll->status);
	if (ret) {
		hipll_err("no pll_unlock_detect_status resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_clear", (u32 *)&hisi_pll->intr_clear);
	if (ret) {
		hipll_err("no intr_clear resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_en_bit", (u32 *)&hisi_pll->en_bit);
	if (ret) {
		hipll_err("no en_bit resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_clr_bit", (u32 *)&hisi_pll->clr_bit);
	if (ret) {
		hipll_err("no clr_bit resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_bypass_acpu", (u32 *)&hisi_pll->intr_bypass);
	if (ret) {
		hipll_err("no intr_clear resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_stat", (u32 *)&hisi_pll->intr_stat);
	if (ret) {
		hipll_err("no intr_stat resources\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_intr_val", (u32 *)&hisi_pll->intr_val);
	if (ret) {
		hipll_err("no intr_val resources\n");
		return -ENODEV;
	}

	return 0;
}

static int hisi_pll_unlock_probe(struct platform_device *pdev)
{
	struct hipll_device *hisi_pll = NULL;
	struct device *dev = &pdev->dev;
	struct device_node *node = pdev->dev.of_node;
	int ret;

	if (node == NULL) {
		hipll_err("dts[%s] node not found\n", "hisilicon,pll_unlock_intr");
		return -ENODEV;
	}

	hisi_pll = devm_kzalloc(dev, sizeof(struct hipll_device), GFP_KERNEL);
	if (hisi_pll == NULL) {
		hipll_err("no mem for hipll device\n");
		return -ENOMEM;
	}

	ret = hisipll_init(node, hisi_pll);
	if (ret) {
		hipll_err("hisi_pll_init fail\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(node, "pll_unlock_detect_base_id", (u32 *)&hisi_pll->base_id);
	if (ret) {
		hipll_err("no pll_unlock_detect_base_id resources\n");
		return -ENODEV;
	}

	hisi_pll->base_addr = hs_clk_base(hisi_pll->base_id);
	if (hisi_pll->base_addr == NULL) {
		pr_err("[%s] fail to get reg_base!\n", __func__);
		return -ENODEV;
	}

	hisi_pll->pll_irq = irq_of_parse_and_map(node, 0);
	hipll_debug("pll_irq is [%u]\n", hisi_pll->pll_irq);

	platform_set_drvdata(pdev, hisi_pll);

	if (hisi_pll->status == 0)
		goto out;

	pll_unlock_en(hisi_pll);

	ret = request_irq(hisi_pll->pll_irq, hisi_pll_unlock_interrupt, 0,
		"hisi_pll_intr", (void *)hisi_pll);
	if (ret) {
		hipll_err("request pll irq failed, ret = %d\n", ret);
		return ret;
	}
out:
	hipll_info("[%s]hipll module init ok!\n", __func__);
	return ret;
}

static int hisi_pll_unlock_remove(struct platform_device *pdev)
{
	hipll_debug("%s ++\n", __func__);
	platform_set_drvdata(pdev, NULL);
	hipll_debug("%s --\n", __func__);
	return 0;
}

static const struct of_device_id hisi_pll_unlock_of_match[] = {
	{ .compatible = "hisilicon,pll_unlock_intr", },
	{},
};

MODULE_DEVICE_TABLE(of, hisi_pll_unlock_of_match);


static struct platform_driver hisi_pll_unlock_driver = {
	.probe = hisi_pll_unlock_probe,
	.remove = hisi_pll_unlock_remove,
	.driver = {
		   .name = "hisi-pll-unlock-intr",
		   .owner = THIS_MODULE,
		   .of_match_table = of_match_ptr(hisi_pll_unlock_of_match),
	},
};

module_platform_driver(hisi_pll_unlock_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("PLL Unlock driver support");
MODULE_AUTHOR("Shaobo Zheng <zhengshaobo1@huawei.com>");
