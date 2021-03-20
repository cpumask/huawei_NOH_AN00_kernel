/*
 * npu_autofs.c
 *
 * about npu autofs
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

#include <asm/io.h>

#include "npu_platform_register.h"
#include "npu_log.h"
#include "npu_common.h"
#include "npu_platform.h"
#include "hisi_npu_pm.h"
#include "npu_autofs.h"


#define npu_autofs_reg_update(addr, value, mask) do { \
	uint32_t reg_value = readl(addr); \
	reg_value = (reg_value & (~mask)) | (value & mask); \
	writel(reg_value, addr); \
} while (0)

void __iomem *g_npu_autofs_crg_vaddr = NULL;

void npu_autofs_npubus_on()
{
	writel(0x0, (SOC_NPUCRG_PERI_AUTODIV0_ADDR(g_npu_autofs_crg_vaddr)));
	writel(0x07E0283F, (SOC_NPUCRG_PERI_AUTODIV2_ADDR(g_npu_autofs_crg_vaddr)));
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV2_ADDR(g_npu_autofs_crg_vaddr)), 0x0, 0x1);
	npu_autofs_reg_update((SOC_NPUCRG_PEREN1_ADDR(g_npu_autofs_crg_vaddr)), 0x100, 0x100);
	return;
}

void npu_autofs_npubus_off()
{
	npu_autofs_reg_update((SOC_NPUCRG_PERDIS1_ADDR(g_npu_autofs_crg_vaddr)), 0x100, 0x100);
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV2_ADDR(g_npu_autofs_crg_vaddr)), 0x1, 0x1);
	return;
}

void npu_autofs_npubuscfg_on()
{
	writel(0x00001800, (SOC_NPUCRG_PERI_AUTODIV3_ADDR(g_npu_autofs_crg_vaddr)));
	writel(0x0060283F, (SOC_NPUCRG_PERI_AUTODIV1_ADDR(g_npu_autofs_crg_vaddr)));
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV1_ADDR(g_npu_autofs_crg_vaddr)), 0x0, 0x1);
	npu_autofs_reg_update((SOC_NPUCRG_PEREN1_ADDR(g_npu_autofs_crg_vaddr)), 0x200, 0x200);
	return;
}

void npu_autofs_npubuscfg_off()
{
	npu_autofs_reg_update((SOC_NPUCRG_PERDIS1_ADDR(g_npu_autofs_crg_vaddr)), 0x200, 0x200);
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV1_ADDR(g_npu_autofs_crg_vaddr)), 0x1, 0x1);
	return;
}

void npu_autofs_npubus_aicore_on()
{
	writel(0x00000001, (SOC_NPUCRG_PERI_AUTODIV4_ADDR(g_npu_autofs_crg_vaddr)));
	writel(0x00602FFF, (SOC_NPUCRG_PERI_AUTODIV5_ADDR(g_npu_autofs_crg_vaddr)));
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV5_ADDR(g_npu_autofs_crg_vaddr)), 0x0, 0x1);
	npu_autofs_reg_update((SOC_NPUCRG_PEREN1_ADDR(g_npu_autofs_crg_vaddr)), 0x800, 0x800);
	return;
}

void npu_autofs_npubus_aicore_off()
{
	npu_autofs_reg_update((SOC_NPUCRG_PERDIS1_ADDR(g_npu_autofs_crg_vaddr)), 0x800, 0x800);
	npu_autofs_reg_update((SOC_NPUCRG_PERI_AUTODIV5_ADDR(g_npu_autofs_crg_vaddr)), 0x1, 0x1);
	return;
}

void npu_autofs_enable()
{
	g_npu_autofs_crg_vaddr = (g_npu_autofs_crg_vaddr == NULL) ?
		ioremap(SOC_ACPU_npu_crg_BASE_ADDR, SOC_ACPU_CRG_NPU_SIZE) : g_npu_autofs_crg_vaddr;
	if (g_npu_autofs_crg_vaddr == NULL) {
		NPU_DRV_ERR("ioremap SOC_ACPU_npu_pcr_BASE_ADDR fail\n");
		return;
	}
	npu_autofs_npubus_on();
	npu_autofs_npubuscfg_on();
	npu_autofs_npubus_aicore_on();
	NPU_DRV_INFO("npu autofs enable\n");
	return;
}


void npu_autofs_disable()
{
	if (g_npu_autofs_crg_vaddr == NULL) {
		NPU_DRV_WARN("g_npu_autofs_crg_vaddr is NULL\n");
		return;
	}
	npu_autofs_npubus_aicore_off();
	npu_autofs_npubuscfg_off();
	npu_autofs_npubus_off();

	iounmap(g_npu_autofs_crg_vaddr);
	g_npu_autofs_crg_vaddr = NULL;
	NPU_DRV_INFO("npu autofs disable\n");
	return;
}
