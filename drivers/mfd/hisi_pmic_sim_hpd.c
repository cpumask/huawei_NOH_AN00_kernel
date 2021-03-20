/*
 * hisi_pmic_sim_hpd.c
 *
 * pmic sim hpd irq process
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hisi_pmic_sim_hpd.h"
#include <linux/mfd/hisi_pmic.h>

#if defined(HISI_PMIC_SIM_HPD_EN)

static void hisi_pmic_ldo_en_clear(u32 ldo_en_reg)
{
	unsigned int reg_tmp;
	unsigned int value = LDO_DISABLE;
	unsigned int mask = LDO_ONOFF_MASK;

	reg_tmp = hisi_pmic_reg_read(ldo_en_reg);
	reg_tmp &= ~mask;
	reg_tmp |= value;
	hisi_pmic_reg_write(ldo_en_reg, reg_tmp);
}

static void hisi_pmic_sim0_hpd_ctrl(const unsigned int pending,
	const unsigned int ctrl0, const unsigned int ctrl1)
{
	if (pending & SIM0_HPD_R_MASK) {
		if (ctrl0 & SIM0_HPD_R_LDO11_PD) {
			hisi_pmic_ldo_en_clear(LDO11_ONOFF_ADDR);

			if (ctrl0 & SIM0_HPD_LDO12_PD)
				hisi_pmic_ldo_en_clear(LDO12_ONOFF_ADDR);
		}

		if (ctrl1 & SIM0_HPD_R_LDO16_PD)
			hisi_pmic_ldo_en_clear(LDO16_ONOFF_ADDR);
	}

	if (pending & SIM0_HPD_F_MASK) {
		if (ctrl0 & SIM0_HPD_F_LDO11_PD) {
			hisi_pmic_ldo_en_clear(LDO11_ONOFF_ADDR);

			if (ctrl0 & SIM0_HPD_LDO12_PD)
				hisi_pmic_ldo_en_clear(LDO12_ONOFF_ADDR);
		}

		if (ctrl1 & SIM0_HPD_F_LDO16_PD)
			hisi_pmic_ldo_en_clear(LDO16_ONOFF_ADDR);
	}
}

static void hisi_pmic_sim1_hpd_ctrl(const unsigned int pending,
	const unsigned int ctrl0, const unsigned int ctrl1)
{
	if (pending & SIM1_HPD_R_MASK) {
		if (ctrl0 & SIM1_HPD_R_LDO12_PD)
			hisi_pmic_ldo_en_clear(LDO12_ONOFF_ADDR);

		if (ctrl1 & SIM1_HPD_R_LDO16_PD)
			hisi_pmic_ldo_en_clear(LDO16_ONOFF_ADDR);
	}

	if (pending & SIM1_HPD_F_MASK) {
		if (ctrl0 & SIM1_HPD_F_LDO12_PD)
			hisi_pmic_ldo_en_clear(LDO12_ONOFF_ADDR);

		if (ctrl1 & SIM1_HPD_F_LDO16_PD)
			hisi_pmic_ldo_en_clear(LDO16_ONOFF_ADDR);
	}
}

void hisi_pmic_sim_hpd_proc(void)
{
	unsigned int pending;
	unsigned int sim_ctr0, sim_ctr1;

	pending = hisi_pmic_reg_read(SIM_IRQ_ADDR);
	if (!(pending & SIM_HPD_PENDING))
		return;

	sim_ctr0 = hisi_pmic_reg_read(SIM_CTRL0_ADDR);
	sim_ctr1 = hisi_pmic_reg_read(SIM_CTRL1_ADDR);

	hisi_pmic_sim0_hpd_ctrl(pending, sim_ctr0, sim_ctr1);
	hisi_pmic_sim1_hpd_ctrl(pending, sim_ctr0, sim_ctr1);
}
#endif

#if defined(HISI_SUB_PMIC_SIM_HPD_EN)

static void hisi_sub_pmic_ldo_en_clear(u32 ldo_en_reg)
{
	unsigned int reg_tmp;
	unsigned int value = LDO_DISABLE;
	unsigned int mask = LDO_ONOFF_MASK;

	reg_tmp = hisi_sub_pmic_reg_read(ldo_en_reg);
	reg_tmp &= ~mask;
	reg_tmp |= value;
	hisi_sub_pmic_reg_write(ldo_en_reg, reg_tmp);
}

static void hisi_sub_pmic_level_bbrp_clear(u32 level_bbrp_reg)
{
	unsigned int reg_tmp;
	unsigned int value = SIM_ENABLE;
	unsigned int mask = SIM_ONOFF_MASK;

	reg_tmp = hisi_sub_pmic_reg_read(level_bbrp_reg);
	reg_tmp &= ~mask;
	reg_tmp |= value;
	hisi_sub_pmic_reg_write(level_bbrp_reg, reg_tmp);
}

static void hisi_sub_pmic_sim_ldo_clear(u32 ldo_en_reg, u32 level_bbrp_reg)
{
	hisi_sub_pmic_level_bbrp_clear(level_bbrp_reg);
	hisi_sub_pmic_ldo_en_clear(ldo_en_reg);
}

static void hisi_sub_pmic_sim_hpd_r_proc(unsigned int sub_sim_ctr)
{
	if (sub_sim_ctr & SUB_SIM_HPD_R_PD_EN) {
		if (sub_sim_ctr & SUB_SIM_HPD_PD_LDO53)
			hisi_sub_pmic_sim_ldo_clear(
				LDO53_ONOFF_ADDR, SUB_SIM_CFG_0_ADDR);

		if (sub_sim_ctr & SUB_SIM_HPD_PD_LDO54)
			hisi_sub_pmic_sim_ldo_clear(
				LDO54_ONOFF_ADDR, SUB_SIM_CFG_2_ADDR);
	}
}

static void hisi_sub_pmic_sim_hpd_f_proc(unsigned int sub_sim_ctr)
{
	if (sub_sim_ctr & SUB_SIM_HPD_F_PD_EN) {
		if (sub_sim_ctr & SUB_SIM_HPD_PD_LDO53)
			hisi_sub_pmic_sim_ldo_clear(
				LDO53_ONOFF_ADDR, SUB_SIM_CFG_0_ADDR);

		if (sub_sim_ctr & SUB_SIM_HPD_PD_LDO54)
			hisi_sub_pmic_sim_ldo_clear(
				LDO54_ONOFF_ADDR, SUB_SIM_CFG_2_ADDR);
	}
}

void hisi_sub_pmic_sim_hpd_proc(void)
{
	unsigned int pending;
	unsigned int sub_sim_ctr0;

	pending = hisi_sub_pmic_reg_read(SUB_SIM_IRQ_ADDR);
	if (!(pending & SUB_SIM_HPD_PENDING))
		return;

	sub_sim_ctr0 = hisi_sub_pmic_reg_read(SUB_SIM_CTRL0_ADDR);

	if (pending & SUB_SIM_HPD_R_MASK)
		hisi_sub_pmic_sim_hpd_r_proc(sub_sim_ctr0);

	if (pending & SUB_SIM_HPD_F_MASK)
		hisi_sub_pmic_sim_hpd_f_proc(sub_sim_ctr0);
}
#endif
