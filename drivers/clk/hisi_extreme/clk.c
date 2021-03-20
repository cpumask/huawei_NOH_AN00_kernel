/*
 * clk.c
 *
 * Hisilicon common clock driver
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

struct hs_clk hs_clk = {
	.lock = __SPIN_LOCK_UNLOCKED(hs_clk.lock),
};

#ifdef CONFIG_HISI_CLK
int is_fpga(void)
{
	static int flag_diff_fpga_asic = -1;

	if (flag_diff_fpga_asic == -1) {
		if (of_find_node_by_name(NULL, "fpga"))
			flag_diff_fpga_asic = 1;
		else
			flag_diff_fpga_asic = 0;
	}
	return flag_diff_fpga_asic;
}
EXPORT_SYMBOL_GPL(is_fpga);
#endif

unsigned int mul_valid_cal(unsigned long freq, unsigned long freq_conversion)
{
	unsigned long freq_update = freq * freq_conversion;

	return (!freq) || (freq_update / freq == freq_conversion);
}

void __iomem __init *hs_clk_base(u32 ctrl)
{
	struct device_node *np = NULL;
	void __iomem *ret = NULL;

	switch (ctrl) {
	case HS_SYSCTRL:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
		break;
	case HS_CRGCTRL:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,crgctrl");
		break;
	case HS_PMCTRL:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,pmctrl");
		break;
	case HS_MEDIACRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,mediactrl");
		break;
	case HS_MEDIA1CRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,media1ctrl");
		break;
	case HS_MEDIA2CRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,media2ctrl");
		break;
	case HS_MMC1CRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,mmc1_sysctrl");
		break;
	case HS_HSDTCRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt_crg");
		break;
	case HS_MMC0CRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,mmc0crg");
		break;
	case HS_HSDT1CRG:
		np = of_find_compatible_node(NULL, NULL, "hisilicon,hsdt1_crg");
		break;
	default:
		pr_err("[%s] ctrl err!\n", __func__);
		goto out;
	}
	if (!np) {
		pr_err("[%s] node doesn't have node!\n", __func__);
		goto out;
	}
	ret = of_iomap(np, 0);
	WARN_ON(!ret);
out:
	return ret;
}

#ifdef CONFIG_HISI_CLK_DEBUG
char *hs_base_addr_transfer(long unsigned int base_addr)
{
	if(!base_addr) {
		pr_err("[%s] base_addr doesn't exist!\n", __func__);
		return NULL;
	}

	if(base_addr == (uintptr_t)hs_clk.crgctrl)
		return "PERICRG";
	else if(base_addr == (uintptr_t)hs_clk.sctrl)
		return "SCTRL";
	else if(base_addr == (uintptr_t)hs_clk.pmctrl)
		return "PMCTRL";
	else if(base_addr == (uintptr_t)hs_clk.pctrl)
		return "PCTRL";
	else if(base_addr == (uintptr_t)hs_clk.media1crg)
		return "MEDIA1CRG";
	else if(base_addr == (uintptr_t)hs_clk.media2crg)
		return "MEDIA2CRG";
	else if(base_addr == (uintptr_t)hs_clk.mmc1crg)
		return "MMC1CRG";
	else if(base_addr == (uintptr_t)hs_clk.mmc0crg)
		return "MMC0CRG";
	else if(base_addr == (uintptr_t)hs_clk.hsdtcrg)
		return "HSDCRG";
	else if (base_addr == (uintptr_t)hs_clk.hsdt1crg)
		return "HSD1CRG";
	else if(base_addr == (uintptr_t)hs_clk.iomcucrg)
		return "IOMCUCRG";
	else
		return "NONE";

}
#endif

struct hisi_clock_data *hisi_clk_init(struct device_node *np,
	int nr_clks)
{
	struct hisi_clock_data *clk_data = NULL;
	struct clk **clk_table = NULL;
	void __iomem *base = NULL;

	base = of_iomap(np, 0);
	if (!base) {
		pr_err("%s: failed to map clock registers\n", __func__);
		goto err;
	}

	clk_data = kzalloc(sizeof(*clk_data), GFP_KERNEL);
	if (!clk_data) {
		pr_err("%s: could not allocate clock data\n", __func__);
		goto err_map;
	}
	clk_data->base = base;

	clk_table = kzalloc(sizeof(struct clk *) * nr_clks, GFP_KERNEL);
	if (!clk_table) {
		pr_err("%s: could not allocate clock lookup table\n", __func__);
		goto err_data;
	}

	clk_data->clk_data.clks = clk_table;
	clk_data->clk_data.clk_num = nr_clks;

	of_clk_add_provider(np, of_clk_src_onecell_get, &clk_data->clk_data);

	return clk_data;
err_data:
	kfree(clk_data);
err_map:
	iounmap(base);
err:
	return NULL;
}
EXPORT_SYMBOL_GPL(hisi_clk_init);

void hisi_clk_data_init(struct clk *clk, const char *alias,
	u32 id, struct hisi_clock_data *data)
{
	int ret;

	if (alias) {
		ret = clk_register_clkdev(clk, alias, NULL);
		if (ret)
			pr_err("%s: failed to register clock lookup for %s!\n",
				__func__, alias);
	}

	if (id < data->clk_data.clk_num)
		data->clk_data.clks[id] = clk;
	else
		pr_err("%s: clk id override, clk num = %d, clk alias = %s!\n",
			__func__, data->clk_data.clk_num, alias);
}
EXPORT_SYMBOL_GPL(hisi_clk_data_init);

void hisi_clk_register_fixed_rate(const struct hisi_fixed_rate_clock *clks,
		int nums, struct hisi_clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = clk_register_fixed_rate(NULL, clks[i].name,
			clks[i].parent_name, clks[i].flags, clks[i].fixed_rate);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		hisi_clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}
EXPORT_SYMBOL_GPL(hisi_clk_register_fixed_rate);

void hisi_clk_register_fixed_factor(const struct hisi_fixed_factor_clock *clks,
		int nums, struct hisi_clock_data *data)
{
	struct clk *clk = NULL;
	int i;

	for (i = 0; i < nums; i++) {
		clk = clk_register_fixed_factor(NULL, clks[i].name,
			clks[i].parent_name, clks[i].flags, clks[i].mult, clks[i].div);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		hisi_clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}
EXPORT_SYMBOL_GPL(hisi_clk_register_fixed_factor);

void hisi_clk_register_mux(const struct hisi_mux_clock *clks,
	int nums, struct hisi_clock_data *data)
{
	struct clk *clk = NULL;
	int i;
	void __iomem *base = data->base;

	for (i = 0; i < nums; i++) {
		u32 shift = ffs(clks[i].mux_mask) - 1;
		u32 width = fls(clks[i].mux_mask) - ffs(clks[i].mux_mask) + 1;
		u32 mask = BIT(width) - 1;

		clk = clk_register_mux_table(NULL, clks[i].name,
				clks[i].parent_names,
				clks[i].num_parents, CLK_SET_RATE_PARENT,
				base + clks[i].offset, shift,
				mask, clks[i].mux_flags,
				NULL, &hs_clk.lock);
		if (IS_ERR_OR_NULL(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

		clk_log_dbg("clks id %d, nums %d, clks name = %s!\n",
			clks[i].id, nums, clks[i].name);

		hisi_clk_data_init(clk, clks[i].alias, clks[i].id, data);
	}
}
EXPORT_SYMBOL_GPL(hisi_clk_register_mux);
