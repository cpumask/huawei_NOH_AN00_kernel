/*
 * smmu.c
 *
 * This is for smmu driver.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#ifdef ENV_ARMLINUX_KERNEL
#include <asm/memory.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/delay.h>
#endif
#include <linux/slab.h>
#include <linux/hisi-iommu.h>
#include "dbg.h"
#include "smmu.h"
#include "smmu_regs.h"
#include "vfmw_ext.h"
#include "hisi_vcodec_vdec_regulator.h"
#include "vfmw_osal.h"

#define TBU_ACK_TIMEOUT 1000

smmu_entry g_smmu_entry;

smmu_entry *smmu_get_entry(void)
{
	return &g_smmu_entry;
}

#define tbu_reg_vir() (smmu_get_entry()->reg_info.smmu_tbu_reg_vir)
#define sid_reg_vir() (smmu_get_entry()->reg_info.smmu_sid_reg_vir)

#define rd_smmu_tbu_vreg(reg, dat) (dat = *((volatile hi_u32 *)(tbu_reg_vir() + (reg))))
#define wr_smmu_tbu_vreg(reg, dat) (*((volatile hi_u32 *)(tbu_reg_vir() + (reg))) = dat)

#define rd_smmu_sid_vreg(reg, dat) (dat = *((volatile hi_u32 *)(sid_reg_vir() + (reg))))
#define wr_smmu_sid_vreg(reg, dat) (*((volatile hi_u32 *)(sid_reg_vir() + (reg))) = dat)

static void set_tbu_reg(hi_s32 addr, hi_u32 val, hi_u32 bw, hi_u32 bs)
{
	hi_u32 mask = (1UL << bw) - 1UL;
	hi_u32 tmp = 0;

	rd_smmu_tbu_vreg(addr, tmp);
	tmp &= ~(mask << bs);
	wr_smmu_tbu_vreg(addr, tmp | ((val & mask) << bs));
}

static void set_sid_reg(hi_s32 addr, hi_u32 val, hi_u32 bw, hi_u32 bs)
{
	hi_u32 mask = (1UL << bw) - 1UL;
	hi_u32 tmp = 0;

	rd_smmu_sid_vreg(addr, tmp);
	tmp &= ~(mask << bs);
	wr_smmu_sid_vreg(addr, tmp | ((val & mask) << bs));
}

void smmu_cfg_sid(void)
{
	set_sid_reg(SMMU_NORM_RSID, 9, 8, 0);
	set_sid_reg(SMMU_NORM_WSID, 9, 8, 0);

	set_sid_reg(SMMU_NORM_RSSID, 0, 1, 0);
	set_sid_reg(SMMU_NORM_WSSID, 0, 1, 0);

	set_sid_reg(SMMU_NORM_RSSIDV, 1, 1, 0);
	set_sid_reg(SMMU_NORM_WSSIDV, 1, 1, 0);
}

hi_s32 smmu_power_on_tcu(void)
{
	struct regulator *tcu_regulator =
		vdec_plat_get_entry()->regulator_info.regulators[SMMU_TCU_REGULATOR];

	return regulator_enable(tcu_regulator);
}

hi_void smmu_power_off_tcu(void)
{
	hi_s32 ret;
	struct regulator *tcu_regulator =
		vdec_plat_get_entry()->regulator_info.regulators[SMMU_TCU_REGULATOR];

	ret = regulator_disable(tcu_regulator);
	if (ret)
		dprint(PRN_ERROR, "power off tcu failed\n");
}

hi_s32 smmu_map_reg(void)
{
	UADDR reg_phy;
	hi_u8 *reg_vaddr = HI_NULL;
	vdec_dts *dts_info = &(vdec_plat_get_entry()->dts_info);
	smmu_entry *entry = smmu_get_entry();

	(void)memset_s(entry, sizeof(*entry), 0, sizeof(*entry));

	reg_phy = dts_info->module_reg[MMU_SID_MOUDLE].reg_phy_addr;
	reg_vaddr = (hi_u8 *)ioremap_nocache(reg_phy, dts_info->module_reg[MMU_SID_MOUDLE].reg_range);
	if (!reg_vaddr) {
		dprint(PRN_ERROR, "map mmu sid reg failed\n");
		return SMMU_ERR;
	}

	entry->reg_info.smmu_sid_reg_vir = reg_vaddr;

	reg_phy = dts_info->module_reg[MMU_TBU_MODULE].reg_phy_addr;
	reg_vaddr = (hi_u8 *)ioremap_nocache(reg_phy, dts_info->module_reg[MMU_TBU_MODULE].reg_range);
	if (!reg_vaddr) {
		dprint(PRN_ERROR, "map mmu tbu reg failed\n");
		iounmap(entry->reg_info.smmu_sid_reg_vir);
		entry->reg_info.smmu_sid_reg_vir = HI_NULL;
		return SMMU_ERR;
	}

	entry->reg_info.smmu_tbu_reg_vir = reg_vaddr;


	return SMMU_OK;
}

void smmu_unmap_reg(void)
{
	smmu_entry *entry = smmu_get_entry();

	if (entry->reg_info.smmu_tbu_reg_vir) {
		iounmap(entry->reg_info.smmu_tbu_reg_vir);
		entry->reg_info.smmu_tbu_reg_vir = HI_NULL;
	}

	if (entry->reg_info.smmu_sid_reg_vir) {
		iounmap(entry->reg_info.smmu_sid_reg_vir);
		entry->reg_info.smmu_sid_reg_vir = HI_NULL;
	}

}

hi_s32 smmu_init(void)
{
	hi_s32 i;
	hi_u32 rel;
	hi_u32 rel_cr;
	smmu_entry *entry = smmu_get_entry();

	if (smmu_power_on_tcu()) {
		dprint(PRN_FATAL, "power on tcu failed\n");
		return SMMU_ERR;
	}

	set_tbu_reg(SMMU_TBU_SCR, 0x1, 1, 0);
	set_tbu_reg(SMMU_TBU_CR, 0x17, 8, 8);
	set_tbu_reg(SMMU_TBU_CR, 0x1, 1, 0);

	for (i = 0; i < TBU_ACK_TIMEOUT; i++) {
		udelay(1);
		rd_smmu_tbu_vreg(SMMU_TBU_CRACK, rel);
		if ((rel & 0x3) == 0x3)
			break;
	}

	if (i == TBU_ACK_TIMEOUT) {
		dprint(PRN_FATAL, "tbu and tcu connect failed\n");
		goto error1;
	}

	dprint(PRN_CTRL, "tbu and tcu connect success\n");

	rd_smmu_tbu_vreg(SMMU_TBU_CR, rel_cr);
	if ((rel & 0xff00) < (rel_cr & 0xff00)) {
		dprint(PRN_FATAL, "check tok_trans_gnt failed\n");
		goto error1;
	}

	entry->smmu_init = 1;
	smmu_cfg_sid();
	return SMMU_OK;

error1:
	smmu_power_off_tcu();

	return SMMU_ERR;
}

void smmu_deinit(void)
{
	hi_s32 i;
	hi_u32 rel;
	smmu_entry *entry = smmu_get_entry();

	if (entry->smmu_init != 1) {
		dprint(PRN_ERROR, "smmu not init\n");
		return;
	}

	set_tbu_reg(SMMU_TBU_CR, 0x0, 1, 0);

	for (i = 0; i < TBU_ACK_TIMEOUT; i++) {
		udelay(1);
		rd_smmu_tbu_vreg(SMMU_TBU_CRACK, rel);
		if ((rel & 0x3) == 0x1)
			break;
	}

	if (i == TBU_ACK_TIMEOUT)
		dprint(PRN_ERROR, "tbu and tcu disconnect failed\n");

	dprint(PRN_CTRL, "tbu and tcu disconnect success\n");

	smmu_power_off_tcu();
	entry->smmu_init = 0;
}

