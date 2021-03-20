/*
 * npu_dpm.c
 *
 * about npu dpm
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#include "npu_dpm.h"

#include <dpm_hwmon.h>
#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/semaphore.h>
#include <linux/version.h>
#include <linux/clk.h>
#include <linux/time.h>

#include "npu_platform_register.h"
#include "npu_log.h"
#include "npu_common.h"
#include "hisi_npu_pm.h"
#include "npu_adapter.h"

unsigned int dpm_npu_states = 0;
static DEFINE_MUTEX(dpm_npu_lock);
#if   defined(CONFIG_HISI_DPM_HWMON_V2)
int npu_dpm_update_energy(void);
#endif

struct dpm_hwmon_ops npu_dpm_ops = {
	.dpm_module_id = DPM_NPU_ID,
	.dpm_type = DPM_NPU_MODULE,
	.hi_dpm_update_counter = npu_dpm_update_counter,
#if   defined(CONFIG_HISI_DPM_HWMON_V2)
	.dpm_cnt_len = DEVDRV_NPU_DPM_NUM * NPU_DPMONITOR_BUSY_CNT_REG_NUM,
	.dpm_power_len = DPM_ENERGY_NUM * DEVDRV_NPU_DPM_NUM,
	.hi_dpm_update_power = npu_dpm_update_energy,
#endif
};

#if   defined(CONFIG_HISI_DPM_HWMON_V2)

struct devdrv_npu_dpm_addr g_devdrv_npu_dpm_addr[DEVDRV_NPU_DPM_NUM] = {
	{SOC_ACPU_lite_dpm0_BASE_ADDR, NULL},
	{SOC_ACPU_lite_dpm1_BASE_ADDR, NULL},
	{SOC_ACPU_lite_dpm2_BASE_ADDR, NULL},
	{SOC_ACPU_lite_npu_dpm3_BASE_ADDR, NULL}
};

u32 npu_dpm_para0[NPU_DPM_PARA_NUM] = {
	0xB26, 0x393B, 0xEBF, 0x4BCB, 0, 0x1365, 0xD67, 0,
	0x6CA4, 0, 0xAA6, 0x1C25, 0x28C6, 0, 0, 0x21C9, /* POWER PARA */
	0, /* const power para */
};
u32 npu_dpm_para1[NPU_DPM_PARA_NUM] = {
	0x6916, 0, 0x232, 0, 0, 0, 0, 0,
	0x1643D, 0x13F84, 0x1225, 0xE62, 0, 0, 0x4189, 0, /* POWER PARA */
	0, /* const power para */
};

/* npu pcr related */
void __iomem *acpu_peri_crg_base_vir_addr = NULL;
void __iomem *acpu_npu_crg_base_vir_addr = NULL;
void __iomem *acpu_npu_sctrl_base_vir_addr = NULL;
void __iomem *acpu_npu_pcr_base_vir_addr = NULL;


int npu_dpm_update_counter(void)
{
	unsigned int i;
	unsigned int j;
	unsigned int val;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;
	struct devdrv_npu_dpm_addr *dpm_addr = NULL;
	unsigned int count_reg_num_updated = 0;
	unsigned int energy_reg_num_updated = 0;

	/* dubai disable npu dpm, return 0 */
	if (npu_dpm_ops.module_enabled == false)
		return 0;

	if ((npu_dpm_ops.dpm_counter_table == NULL) ||
		(npu_dpm_ops.dpm_power_table == NULL))
		return -1;

	cur_dev_ctx = get_dev_ctx_by_id(0);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("get current device failed");
		return -1;
	}

	if (atomic_read(&cur_dev_ctx->power_access) == 0) {
		mutex_lock(&dpm_npu_lock);
		for (i = 0; i < DEVDRV_NPU_DPM_NUM; i++) {
			dpm_addr = &g_devdrv_npu_dpm_addr[i];
			if (dpm_addr->vir_addr != NULL) {
				/* step14 */
				writel(0x1, SOC_DPMONITOR_SOFT_PULSE_ADDR(dpm_addr->vir_addr));
				udelay(NPU_DPM_PULSE_SLEEP);
				/* step15 */
				for (j = 0; j < NPU_DPMONITOR_BUSY_CNT_REG_NUM; j++) {
					val = readl(SOC_DPMONITOR_BUSY_CNT0_ADDR(dpm_addr->vir_addr) +
						DPM_REG_BASE_OFFSET * j);
					npu_dpm_ops.dpm_counter_table[count_reg_num_updated++] += val;
				}
				/* step16 */
				val = readl(SOC_DPMONITOR_ACC_ENERGY_ADDR(dpm_addr->vir_addr));
				npu_dpm_ops.dpm_power_table[energy_reg_num_updated++] += val;
			}
		}
		mutex_unlock(&dpm_npu_lock);
	}

	return (count_reg_num_updated + energy_reg_num_updated);
}

int npu_dpm_update_energy(void)
{
	u32 index;
	u32 val;
	int reg_num_updated = 0;
	struct devdrv_npu_dpm_addr *dpm_addr = NULL;
	struct devdrv_dev_ctx *cur_dev_ctx = NULL;

	/* dubai disable npu dpm, return 0 */
	if (npu_dpm_ops.module_enabled == false)
		return 0;
	
	if (npu_dpm_ops.dpm_power_table == NULL)
		return -1;

	cur_dev_ctx = get_dev_ctx_by_id(0);
	if (cur_dev_ctx == NULL) {
		NPU_DRV_ERR("get current device failed");
		return -1;
	}

	if (atomic_read(&cur_dev_ctx->power_access) == 0) {
		mutex_lock(&dpm_npu_lock);
		for (index = 0; index < DEVDRV_NPU_DPM_NUM; index++) {
			dpm_addr = &g_devdrv_npu_dpm_addr[index];
			if (dpm_addr->vir_addr != NULL) {
				/* step14 */
				writel(0x1, SOC_DPMONITOR_SOFT_PULSE_ADDR(dpm_addr->vir_addr));
				udelay(NPU_DPM_PULSE_SLEEP);
				/* step16 */
				val = readl(SOC_DPMONITOR_ACC_ENERGY_ADDR(dpm_addr->vir_addr));
				npu_dpm_ops.dpm_power_table[reg_num_updated++] += val;
			} else {
				NPU_DRV_DEBUG("g_devdrv_npu_dpm_addr %d vir_addr is null\n", index);
			}
		}
		mutex_unlock(&dpm_npu_lock);
	}

	return reg_num_updated;
}

static void npu_dpm_disable_and_iounmap(int index)
{
	int i;
	struct devdrv_npu_dpm_addr *dpm_addr = NULL;
	int num = (index > DEVDRV_NPU_DPM_NUM) ? DEVDRV_NPU_DPM_NUM : index;

	for (i = 0; i < num; i++) {
		dpm_addr = &g_devdrv_npu_dpm_addr[i];
		if (dpm_addr->vir_addr != NULL) {
			dpm_monitor_disable(dpm_addr->vir_addr);
			iounmap(dpm_addr->vir_addr);
			dpm_addr->vir_addr = NULL;
		}
	}
}

static void npu_pcr_remap(void)
{
	if (acpu_npu_crg_base_vir_addr == NULL) {
		acpu_npu_crg_base_vir_addr = ioremap(SOC_ACPU_npu_crg_BASE_ADDR, SOC_NPU_CRG_SIZE);
		if (acpu_npu_crg_base_vir_addr == NULL) {
			NPU_DRV_ERR("ioremap SOC_ACPU_NPU_CRG_BASE fail\n");
			return;
		}
	}

	if (acpu_npu_sctrl_base_vir_addr == NULL) {
		acpu_npu_sctrl_base_vir_addr = ioremap(SOC_ACPU_npu_sysctrl_BASE_ADDR, SOC_NPU_SCTRL_SIZE);
		if (acpu_npu_sctrl_base_vir_addr == NULL) {
			NPU_DRV_ERR("ioremap SOC_ACPU_npu_sysctrl_BASE_ADDR fail\n");
			goto npu_sctrl_fail;
		}
	}

	if (acpu_peri_crg_base_vir_addr == NULL) {
		acpu_peri_crg_base_vir_addr = ioremap(SOC_ACPU_PERI_CRG_BASE_ADDR, SOC_ACPU_PERI_CRG_SIZE);
		if (acpu_peri_crg_base_vir_addr == NULL) {
			NPU_DRV_ERR("ioremap SOC_ACPU_PERI_CRG_BASE_ADDR fail\n");
			goto peri_crg_fail;
		}
	}

	if (acpu_npu_pcr_base_vir_addr == NULL) {
		acpu_npu_pcr_base_vir_addr = ioremap(SOC_ACPU_npu_pcr_BASE_ADDR, SOC_NPU_PCR_SIZE);
		if (acpu_npu_pcr_base_vir_addr == NULL) {
			NPU_DRV_ERR("ioremap SOC_ACPU_npu_pcr_BASE_ADDR fail\n");
			goto npu_pcr_fail;
		}
	}

	return;
npu_pcr_fail:
	if (acpu_peri_crg_base_vir_addr != NULL) {
		iounmap(acpu_peri_crg_base_vir_addr);
		acpu_peri_crg_base_vir_addr = NULL;
	}
peri_crg_fail:
	if (acpu_npu_sctrl_base_vir_addr != NULL) {
		iounmap(acpu_npu_sctrl_base_vir_addr);
		acpu_npu_sctrl_base_vir_addr = NULL;
	}
npu_sctrl_fail:
	if (acpu_npu_crg_base_vir_addr != NULL) {
		iounmap(acpu_npu_crg_base_vir_addr);
		acpu_npu_crg_base_vir_addr = NULL;
	}
	NPU_DRV_ERR("npu_dpm_pre_enable fail leave\n");
}

static void npu_pcr_unremap(void)
{
	if (acpu_npu_pcr_base_vir_addr != NULL) {
		iounmap(acpu_npu_pcr_base_vir_addr);
		acpu_npu_pcr_base_vir_addr = NULL;
	}

	if (acpu_peri_crg_base_vir_addr != NULL) {
		iounmap(acpu_peri_crg_base_vir_addr);
		acpu_peri_crg_base_vir_addr = NULL;
	}

	if (acpu_npu_sctrl_base_vir_addr != NULL) {
		iounmap(acpu_npu_sctrl_base_vir_addr);
		acpu_npu_sctrl_base_vir_addr = NULL;
	}

	if (acpu_npu_crg_base_vir_addr != NULL) {
		iounmap(acpu_npu_crg_base_vir_addr);
		acpu_npu_crg_base_vir_addr = NULL;
	}
}

static void npu_pcr_enable(void)
{
	COND_RETURN_VOID(((acpu_npu_pcr_base_vir_addr == NULL) ||
		(acpu_peri_crg_base_vir_addr == NULL) ||
		(acpu_npu_sctrl_base_vir_addr == NULL) ||
		(acpu_npu_crg_base_vir_addr == NULL)), "addr pointer is null\n");

	/* close gt_clk_npu_pcr */
	writel((u32)BIT(SOC_CRGPERIPH_PERDIS7_gt_clk_npu_pcr_START),
		SOC_CRGPERIPH_PERDIS7_ADDR(acpu_peri_crg_base_vir_addr));

	/* un-reset ip_arst_npupcr */
	writel((u32)BIT(SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_pcr_START),
		(SOC_NPUCRG_PERRSTDIS2_ADDR(acpu_npu_crg_base_vir_addr)));

	/* un-reset ip_prst_npu_pcr */
	writel((u32)BIT(SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_pcr_START),
		(SOC_NPUCRG_PERRSTDIS1_ADDR(acpu_npu_crg_base_vir_addr)));

	/* open gt_clk_npu_pcr */
	writel((u32)BIT(SOC_CRGPERIPH_PEREN7_gt_clk_npu_pcr_START),
		(SOC_CRGPERIPH_PEREN7_ADDR(acpu_peri_crg_base_vir_addr)));

	/* open gt_pclk_npu_pcr */
	writel((u32)BIT(SOC_NPUCRG_PEREN1_gt_pclk_npu_pcr_START),
		(SOC_NPUCRG_PEREN1_ADDR(acpu_npu_crg_base_vir_addr)));

	writel(0x00000850, (SOC_NPU_SCTRL_npu_ctrl11_ADDR(acpu_npu_sctrl_base_vir_addr)));

	writel(0x01434000, (SOC_PCR_CFG_PERIOD0_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0x5, (SOC_PCR_CFG_PERIOD1_ADDR(acpu_npu_pcr_base_vir_addr)));

	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_BUDGET0_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_BUDGET1_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_BUDGET2_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_BUDGET3_ADDR(acpu_npu_pcr_base_vir_addr)));

	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_DIDT0_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_DIDT1_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_DIDT2_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0xFFFFFFFF, (SOC_PCR_THRESHOLD_DIDT3_ADDR(acpu_npu_pcr_base_vir_addr)));

	writel(0x00000080, (SOC_PCR_THRESHOLD_BUDGET_SVFD0_ADDR(acpu_npu_pcr_base_vir_addr)));
	writel(0x000099ff, (SOC_PCR_THRESHOLD_BUDGET_SVFD1_ADDR(acpu_npu_pcr_base_vir_addr)));

	/* step2 */
	/* enable pcr */
	writel(0x00000010, (SOC_PCR_CTRL_ADDR(acpu_npu_pcr_base_vir_addr)));
	/* no bypass, open svfd, and close pcr_cg */
	writel(0x4, (SOC_PCR_CFG_BYPASS_ADDR(acpu_npu_pcr_base_vir_addr)));
	NPU_DRV_WARN("%s [%d]\n", __func__, __LINE__);

	return;
}

static void npu_pcr_disable(void)
{
	NPU_DRV_WARN("%s [%d]\n", __func__, __LINE__);
	if ((acpu_npu_pcr_base_vir_addr == NULL) ||
		(acpu_peri_crg_base_vir_addr == NULL) ||
		(acpu_npu_sctrl_base_vir_addr == NULL) ||
		(acpu_npu_crg_base_vir_addr == NULL)) {
		NPU_DRV_WARN("addr pointer is null\n");
		return;
	}

	/* pcr deinit */
	/* bypass pcr CG */
	writel((u32)0x00010001,
		(SOC_NPUCRG_PERI_CTRL0_ADDR(acpu_npu_crg_base_vir_addr)));
	/* bypass pcr output */
	writel((u32)0x0,
		(SOC_PCR_CFG_BYPASS_ADDR(acpu_npu_pcr_base_vir_addr)));
	/* disable pcr */
	writel((u32)0x0,
		(SOC_PCR_CTRL_ADDR(acpu_npu_pcr_base_vir_addr)));
	/* pcr shutdown mode */
	writel((u32)0x00000854,
		(SOC_NPU_SCTRL_npu_ctrl11_ADDR(acpu_npu_sctrl_base_vir_addr)));

	/* un-reset ip_arst_npupcr */
	writel((u32)BIT(SOC_NPUCRG_PERRSTEN2_ip_rst_npu_pcr_START),
		(SOC_NPUCRG_PERRSTEN2_ADDR(acpu_npu_crg_base_vir_addr)));

	/* un-reset ip_prst_npu_pcr */
	writel((u32)BIT(SOC_NPUCRG_PERRSTEN1_ip_prst_npu_pcr_START),
		(SOC_NPUCRG_PERRSTEN1_ADDR(acpu_npu_crg_base_vir_addr)));

	/* close gt_clk_npu_pcr */
	writel((u32)BIT(SOC_CRGPERIPH_PERDIS7_gt_clk_npu_pcr_START),
		(SOC_CRGPERIPH_PERDIS7_ADDR(acpu_peri_crg_base_vir_addr)));

	/* close gt_pclk_npu_pcr */
	writel((u32)BIT(SOC_NPUCRG_PERDIS1_gt_pclk_npu_pcr_START),
		(SOC_NPUCRG_PERDIS1_ADDR(acpu_npu_crg_base_vir_addr)));
}

static void npu_dpm_enable(void)
{
	int index;
	int freq_index;
	struct devdrv_npu_dpm_addr *dpm_addr = NULL;

	/* dubai disable npu dpm, return */
	if (npu_dpm_ops.module_enabled == false)
		return;

	npu_pcr_remap();
	npu_pcr_enable();

	mutex_lock(&dpm_npu_lock);
	for (index = 0; index < DEVDRV_NPU_DPM_NUM; index++) {
		dpm_addr = &g_devdrv_npu_dpm_addr[index];
		if (dpm_addr->vir_addr == NULL) {
			dpm_addr->vir_addr = ioremap(dpm_addr->phy_addr, SOC_NPU_DPM_SIZE);
			if (dpm_addr->vir_addr == NULL) {
				npu_dpm_disable_and_iounmap(index);
				NPU_DRV_ERR("ioremap g_devdrv_npu_dpm_addr %d fail\n", index);
				mutex_unlock(&dpm_npu_lock);
				return;
			}
		}
		NPU_DRV_DEBUG(" [%d] phy_addr:0x%llx vir_addr:0x%pK\n", index, dpm_addr->phy_addr, dpm_addr->vir_addr);
		if ((index == LITE_DPM0) || (index == LITE_DPM2))
			dpm_monitor_enable(dpm_addr->vir_addr, SIGNAL_LEVEL_MODE0, npu_dpm_para0, NPU_DPM_PARA_NUM);
		else
			dpm_monitor_enable(dpm_addr->vir_addr, SIGNAL_LEVEL_MODE1, npu_dpm_para1, NPU_DPM_PARA_NUM);

		for (freq_index = 0; freq_index < NPU_DPMONITOR_FREQ_PARAM_REG_NUM; freq_index++)
			writel(FREQ_PARAM_DEFAULT_VALUE,
				SOC_DPMONITOR_FREQ_PARAM0_ADDR(dpm_addr->vir_addr) +
				freq_index * DPM_REG_BASE_OFFSET);
	}
	mutex_unlock(&dpm_npu_lock);
	NPU_DRV_WARN("%s [%d]\n", __func__, __LINE__);
	return;
}

static void npu_dpm_disable(void)
{
	npu_pcr_disable();
	npu_pcr_unremap();
	mutex_lock(&dpm_npu_lock);
	npu_dpm_disable_and_iounmap(DEVDRV_NPU_DPM_NUM);
	mutex_unlock(&dpm_npu_lock);
	return;
}

bool npu_dpm_enable_flag(void)
{
	return npu_dpm_ops.module_enabled;
}
#endif

// npu powerup
void npu_dpm_init(void)
{
	npu_dpm_enable();

	dpm_npu_states = 1;
	NPU_DRV_INFO("dpm init success\n");
}

// npu powerdown
void npu_dpm_exit(void)
{

	dpm_npu_states = 0;

	npu_dpm_disable();
	NPU_DRV_WARN("[dpm_npu]dpm_npu is successfully deinitialized\n");
}

static int __init dpm_npu_init(void)
{
	return dpm_hwmon_register(&npu_dpm_ops);
}
module_init(dpm_npu_init);

static void __exit dpm_npu_exit(void)
{
	dpm_hwmon_unregister(&npu_dpm_ops);
}
module_exit(dpm_npu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei Tech. Co., Ltd");
MODULE_DESCRIPTION("DPM");
MODULE_VERSION("V1.0");

