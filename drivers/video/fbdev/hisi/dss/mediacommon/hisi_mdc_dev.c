/* Copyright (c) 2019-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <uapi/linux/sched/types.h>

#include "hisi_fb.h"
#include "hisi_mdc_overlay.h"

static void hisi_mdc_aif_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_mif_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_rch_mctl_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_chn_data_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_chn_yuv_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_wch_mctl_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
static void hisi_mdc_wch_init(dss_module_reg_t *mdc_module, char __iomem *addr_base, int32_t chn_idx);
#ifdef SUPPORT_RCH_CLD
extern void hisi_dss_cld_init(const char __iomem *cld_base, struct dss_cld *s_cld);
#endif

static hisi_mdc_chn_res_t g_mdc_chn_res[] = {
	{
		DSS_RCHN_D2, DSS_RCH_D0_DMA_OFFSET, /* rch0 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_rch_mctl_init,
		hisi_mdc_chn_data_init,
	},
	{
		DSS_RCHN_D3, DSS_RCH_D1_DMA_OFFSET, /* rch1 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_rch_mctl_init,
		hisi_mdc_chn_data_init,
	},
	{
		DSS_RCHN_V0, DSS_RCH_VG0_DMA_OFFSET, /* rch2 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_rch_mctl_init,
		hisi_mdc_chn_yuv_init,
	},
	{
		DSS_RCHN_D0, DSS_RCH_D2_DMA_OFFSET, /* rch6 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_rch_mctl_init,
		hisi_mdc_chn_data_init,
	},
	{
		DSS_WCHN_W0, WCH0_OFFSET, /* MDC_WCHN_W0 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_wch_mctl_init,
		hisi_mdc_wch_init,
	},
	{
		DSS_WCHN_W1, DSS_WCH1_DMA_OFFSET, /* DSS_WCHN_W1 */
		hisi_mdc_aif_init,
		hisi_mdc_mif_init,
		hisi_mdc_wch_mctl_init,
		hisi_mdc_wch_init,
	},
};

int mdc_clk_regulator_enable(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	struct clk *clk_tmp = NULL;
	struct dss_vote_cmd dss_vote_cmd = {0};

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is nullptr!\n");

	clk_tmp = hisifd->dss_aclk_media_common_clk;
	if (!IS_ERR(clk_tmp)) {
		ret = clk_prepare_enable(clk_tmp);
		if (ret) {
			HISI_FB_ERR("fb%u aclk_mdc_clk clk_prepare_enable failed, error=%d!\n", hisifd->index, ret);
			return -EINVAL;
		}
	}

	clk_tmp = hisifd->dss_clk_media_common_clk;
	if (!IS_ERR(clk_tmp)) {
		ret = clk_prepare_enable(clk_tmp);
		if (ret) {
			HISI_FB_ERR("fb%u clk_mdc_clk clk_prepare_enable failed, error=%d!\n", hisifd->index, ret);
			clk_disable_unprepare(hisifd->dss_aclk_media_common_clk);
			return -EINVAL;
		}
	}

	if (!IS_ERR(hisifd->mdc_regulator)) {
		ret = regulator_enable(hisifd->mdc_regulator);
		if (ret) {
			HISI_FB_ERR("fb%u mdc_regulator enable failed, error=%d!\n", hisifd->index, ret);
			clk_disable_unprepare(hisifd->dss_clk_media_common_clk);
			clk_disable_unprepare(hisifd->dss_aclk_media_common_clk);
			return -EINVAL;
		}
	}

	/* use dss max perf level */
	hisifb_get_other_fb_votelevel(hisifd, &dss_vote_cmd.dss_voltage_level);
	if (dss_vote_cmd.dss_voltage_level < PERI_VOLTAGE_LEVEL1)
		dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL1;

	set_mdc_core_clk(hisifd, dss_vote_cmd);

	return ret;
}

void mdc_clk_regulator_disable(struct hisi_fb_data_type *hisifd)
{
	struct clk *clk_tmp = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	hisifd->dss_vote_cmd.dss_voltage_level = PERI_VOLTAGE_LEVEL0;
	(void)set_mdc_core_clk(hisifd, hisifd->dss_vote_cmd);
	(void)clk_set_rate(hisifd->dss_clk_media_common_clk, DEFAULT_DSS_CORE_CLK_RATE_POWER_OFF);

	if (!IS_ERR(hisifd->mdc_regulator))
		(void)regulator_disable(hisifd->mdc_regulator);

	clk_tmp = hisifd->dss_clk_media_common_clk;
	if (!IS_ERR(clk_tmp))
		(void)clk_disable_unprepare(clk_tmp);

	clk_tmp = hisifd->dss_aclk_media_common_clk;
	if (!IS_ERR(clk_tmp))
		(void)clk_disable_unprepare(clk_tmp);
}

void hisi_mdc_interrupt_config(struct hisi_fb_data_type *hisifd)
{
	uint32_t value;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	value = ~0;
	outp32(hisifd->media_common_base + GLB_CPU_OFF_INT_MSK, value);
	outp32(hisifd->media_common_base + GLB_CPU_OFF_INTS, value);

	if (hisifd->dpe_irq) {
		enable_irq(hisifd->dpe_irq);
		HISI_FB_DEBUG("enable irq %u\n", hisifd->dpe_irq);
	}

	value &= ~(BIT_OFF_WCH0_INTS | BIT_OFF_WCH1_INTS);

	outp32(hisifd->media_common_base + GLB_CPU_OFF_INT_MSK, value);
}

void hisi_mdc_interrupt_deconfig(struct hisi_fb_data_type *hisifd)
{
	uint32_t value;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	value = ~0;
	outp32(hisifd->media_common_base + GLB_CPU_OFF_INT_MSK, value);

	if (hisifd->dpe_irq) {
		disable_irq(hisifd->dpe_irq);
		HISI_FB_DEBUG("enable irq %u\n", hisifd->dpe_irq);
	}
}

/*lint -e529*/
static void hisi_mdc_aif_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	uint32_t reg_step = chn_idx * 0x20;

	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");

	mdc_module->aif_ch_base[chn_idx] =
		SOC_MDC_AIF_CH_CTL_0_ADDR(addr_base) + reg_step;

	mdc_module->aif[chn_idx].aif_ch_ctl =
		inp32(mdc_module->aif_ch_base[chn_idx] + AIF_CH_CTL);

#ifdef CONFIG_DSS_SMMU_V3
	/* aif select srt_axi0 */
	mdc_module->aif[chn_idx].aif_ch_ctl =
		set_bits32(mdc_module->aif[chn_idx].aif_ch_ctl, 0x1, 1, 0);
#endif

	mdc_module->aif[chn_idx].aif_ch_hs =
		inp32(mdc_module->aif_ch_base[chn_idx] + AIF_CH_HS);

	mdc_module->aif[chn_idx].aif_ch_ls =
		inp32(mdc_module->aif_ch_base[chn_idx] + AIF_CH_LS);

}

/*lint -e676*/
static void hisi_mdc_mif_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	uint32_t reg_step = chn_idx * 0x10;

	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX)), ERR, "chn_idx=%d!\n", chn_idx);
	if (chn_idx >= DSS_WCHN_W0)
		reg_step += 0x10;

	mdc_module->mif_ch_base[chn_idx] =
		SOC_MDC_MIF_CTRL_D0_ADDR(addr_base) + reg_step;

	hisi_dss_mif_init(mdc_module->mif_ch_base[chn_idx],
		&(mdc_module->mif[chn_idx]), chn_idx);
}
/*lint +e676*/

void hisi_mdc_rch_mctl_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	uint32_t reg_step = chn_idx * 0x4;

	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");
	mdc_module->mctl_ch_base[chn_idx].chn_mutex_base =
		SOC_MDC_CTL_MUTEX_RCH0_ADDR(addr_base) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_ov_en_base =
		addr_base + SOC_MDC_RCH0_OV_OEN_ADDR(MCTL_SYS_OFFSET) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_flush_en_base =
		addr_base + SOC_MDC_RCH0_FLUSH_EN_ADDR(MCTL_SYS_OFFSET) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_mod_dbg_base =
		addr_base + SOC_MDC_MOD0_DBG_ADDR(MCTL_SYS_OFFSET) + reg_step;

	hisi_dss_mctl_ch_mod_dbg_init(
		mdc_module->mctl_ch_base[chn_idx].chn_mod_dbg_base,
		&(mdc_module->mctl_ch[chn_idx]));
}

static void hisi_mdc_chn_data_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");
	mdc_module->dma_base[chn_idx] = SOC_MDC_DMA_OFT_X0_ADDR(addr_base);
	hisi_dss_rdma_init(mdc_module->dma_base[chn_idx],
		&(mdc_module->rdma[chn_idx]));

	mdc_module->dfc_base[chn_idx] = SOC_MDC_DFC_DISP_SIZE_ADDR(addr_base);
	hisi_dss_dfc_init(mdc_module->dfc_base[chn_idx],
		&(mdc_module->dfc[chn_idx]));
}

static void hisi_mdc_chn_yuv_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");
	hisi_mdc_chn_data_init(mdc_module, addr_base, chn_idx);

	hisi_dss_rdma_u_init(mdc_module->dma_base[chn_idx], &(mdc_module->rdma[chn_idx]));

	hisi_dss_rdma_v_init(mdc_module->dma_base[chn_idx], &(mdc_module->rdma[chn_idx]));
}

static void hisi_mdc_wch_mctl_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	uint32_t reg_step = 0;

	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");
	if (chn_idx == DSS_WCHN_W1)
		reg_step = 0x4;

	mdc_module->mctl_ch_base[chn_idx].chn_mutex_base =
		SOC_MDC_CTL_MUTEX_WCH0_ADDR(addr_base) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_ov_en_base =
		addr_base + SOC_MDC_WCH0_OV_IEN_ADDR(MCTL_SYS_OFFSET) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_flush_en_base =
		addr_base + SOC_MDC_WCH0_FLUSH_EN_ADDR(MCTL_SYS_OFFSET) + reg_step;

	mdc_module->mctl_ch_base[chn_idx].chn_mod_dbg_base =
		addr_base + SOC_MDC_MOD8_DBG_ADDR(MCTL_SYS_OFFSET) + reg_step;

	hisi_dss_mctl_ch_mod_dbg_init(
		mdc_module->mctl_ch_base[chn_idx].chn_mod_dbg_base, &(mdc_module->mctl_ch[chn_idx]));
}

static void hisi_mdc_wch_init(dss_module_reg_t *mdc_module,
	char __iomem *addr_base, int32_t chn_idx)
{
	hisi_check_and_no_retval(((chn_idx < 0) || (chn_idx >= DSS_CHN_MAX_DEFINE)), ERR, "array idx out of range!\n");
	mdc_module->dma_base[chn_idx] = SOC_MDC_DMA_OFT_X0_ADDR(addr_base);
	hisi_dss_wdma_init(mdc_module->dma_base[chn_idx],
		&(mdc_module->wdma[chn_idx]));

	mdc_module->dfc_base[chn_idx] = SOC_MDC_DFC_DISP_SIZE_ADDR(addr_base);
	hisi_dss_dfc_init(mdc_module->dfc_base[chn_idx],
		&(mdc_module->dfc[chn_idx]));

	mdc_module->scl_base[chn_idx] = SOC_MDC_SCF_EN_HSCL_STR_ADDR(addr_base);
	hisi_dss_scl_init(mdc_module->scl_base[chn_idx],
		&(mdc_module->scl[chn_idx]));

	mdc_module->pcsc_base[chn_idx] = SOC_MDC_PCSC_IDC0_ADDR(addr_base);
	hisi_dss_csc_init(mdc_module->pcsc_base[chn_idx],
		&(mdc_module->pcsc[chn_idx]));

	mdc_module->post_clip_base[chn_idx] =
		SOC_MDC_POST_CLIP_DISP_SIZE_ADDR(addr_base);
	hisi_dss_post_clip_init(mdc_module->post_clip_base[chn_idx],
		&(mdc_module->post_clip[chn_idx]));

	mdc_module->csc_base[chn_idx] = SOC_MDC_CSC_IDC0_ADDR(addr_base);
	hisi_dss_csc_init(mdc_module->csc_base[chn_idx],
		&(mdc_module->csc[chn_idx]));
}

static void hisi_mdc_mctl_init(dss_module_reg_t *mdc_module,
	char __iomem *mdc_base)
{
	mdc_module->ov_base[DSS_OVL2] = mdc_base + DSS_OVL2_OFFSET;
	hisi_dss_ovl_init(mdc_module->ov_base[DSS_OVL2],
		&(mdc_module->ov[DSS_OVL2]), DSS_OVL2);

	mdc_module->mctl_sys_base = mdc_base + MCTL_SYS_OFFSET;
	hisi_dss_mctl_sys_init(mdc_module->mctl_sys_base,
		&(mdc_module->mctl_sys));

	mdc_module->mctl_base[DSS_MCTL0] = mdc_base + DSS_MCTRL_CTL0_OFFSET;
	hisi_dss_mctl_init(mdc_module->mctl_base[DSS_MCTL0],
		&(mdc_module->mctl[DSS_MCTL0]));

	/* ov2 for offline compose, so reuse dss driver by set DSS_MCTL2 */
	mdc_module->mctl_base[DSS_MCTL2] = mdc_base + DSS_MCTRL_CTL1_OFFSET;
	hisi_dss_mctl_init(mdc_module->mctl_base[DSS_MCTL2],
		&(mdc_module->mctl[DSS_MCTL2]));

	mdc_module->smmu_base = mdc_base + VBIF0_SMMU_OFFSET;
	hisi_dss_smmu_init(mdc_module->smmu_base, &(mdc_module->smmu));
}

void hisi_mdc_module_default(struct hisi_fb_data_type *hisifd)
{
	uint32_t i = 0;
	dss_module_reg_t *dss_mdc_module = NULL;
	hisi_mdc_chn_res_t *resource = NULL;
	char __iomem *mdc_base = NULL;
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");

	(void)hisi_dss_module_init(hisifd);
	dss_mdc_module = &hisifd->dss_mdc_module_default;
	mdc_base = hisifd->media_common_base;

	hisi_check_and_no_retval(!mdc_base, ERR, "nullptr!\n");
	HISI_FB_DEBUG("fb%u +!\n", hisifd->index);
	for (; i < ARRAY_SIZE(g_mdc_chn_res); i++) {
		resource = &(g_mdc_chn_res[i]);
		if (resource->aif_init)
			resource->aif_init(dss_mdc_module, mdc_base + DSS_VBIF0_AIF, resource->chn_idx);

		if (resource->mif_init)
			resource->mif_init(dss_mdc_module, mdc_base + DSS_MIF_OFFSET, resource->chn_idx);

		if (resource->mctl_init)
			resource->mctl_init(dss_mdc_module, mdc_base, resource->chn_idx);

		if (resource->chn_init)
			resource->chn_init(dss_mdc_module, mdc_base + resource->chn_addr_base, resource->chn_idx);
	}
	hisi_mdc_mctl_init(dss_mdc_module, mdc_base);
#ifdef SUPPORT_RCH_CLD
	dss_mdc_module->cld_base = mdc_base + CLD_OFFSET;
	hisi_dss_cld_init(dss_mdc_module->cld_base, &(dss_mdc_module->cld));
#endif

	kthread_init_work(&hisifd->preoverlay_work, hisi_mdc_preoverlay_async_play);
	kthread_init_worker(&hisifd->preoverlay_worker);
	hisifd->preoverlay_thread = kthread_create(kthread_worker_fn, &hisifd->preoverlay_worker, "preoverlay");
	if (IS_ERR_OR_NULL(hisifd->preoverlay_thread)) {
		HISI_FB_WARNING("fb%u failed to create preoverlay_thread!\n", hisifd->index);
		return;
	}

	sched_setscheduler_nocheck(hisifd->preoverlay_thread, SCHED_FIFO, &param);

	wake_up_process(hisifd->preoverlay_thread);

	HISI_FB_DEBUG("fb%u -!\n", hisifd->index);
}

void hisi_mdc_mif_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *mif_base = NULL;
	char __iomem *addr_base = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is nullptr!\n");
	/* config mdc select SRT_AXI0 */
	addr_base = hisifd->media_common_base + VBIF0_AIF_OFFSET;
	set_reg(SOC_MDC_AIF_CH_CTL_0_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_1_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_2_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_6_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_8_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_9_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_10_ADDR(addr_base), 0xF001, 32, 0);
	set_reg(SOC_MDC_AIF_CH_CTL_CMD_ADDR(addr_base), 0x1, 32, 0);
	set_reg(SOC_MDC_MMU_ID_ATTR_NS_13_ADDR(addr_base), 0x3F, 32, 0);

	hisi_check_and_no_retval(!hisifd->media_common_base, ERR, "nullptr!\n");
	mif_base = hisifd->media_common_base + VBIF0_MIF_OFFSET;
	set_reg(mif_base + MIF_ENABLE, 0x1, 1, 0);

#ifdef CONFIG_HISI_FB_V600
	/* disable mdc pref */
	set_reg(hisifd->media_common_base + MIF_CH0_OFFSET + MIF_CTRL0, 0x0, 2, 0);
	set_reg(hisifd->media_common_base + MIF_CH1_OFFSET + MIF_CTRL0, 0x0, 2, 0);
	set_reg(hisifd->media_common_base + MIF_CH2_OFFSET + MIF_CTRL0, 0x0, 2, 0);
	set_reg(hisifd->media_common_base + MIF_CH6_OFFSET + MIF_CTRL0, 0x0, 2, 0);
	set_reg(hisifd->media_common_base + MIF_CH8_OFFSET + MIF_CTRL0, 0x0, 2, 0);
	set_reg(hisifd->media_common_base + MIF_CH9_OFFSET + MIF_CTRL0, 0x0, 2, 0);
#else
	set_reg(hisifd->media_common_base + MIF_CH2_OFFSET + MIF_CTRL0, 0x1, 1, 0);
	set_reg(hisifd->media_common_base + MIF_CH9_OFFSET + MIF_CTRL0, 0x1, 1, 0);
#endif
	set_reg(mif_base + AIF_CMD_RELOAD, 0x1, 1, 0);
}

void hisi_mdc_scl_coef_on(struct hisi_fb_data_type *hisifd, int coef_lut_idx)
{
	char __iomem *module_base = NULL;
	char __iomem *h0_y_addr = NULL;
	char __iomem *y_addr = NULL;
	char __iomem *uv_addr = NULL;
	int ret;
	struct coef_lut_tap lut_tap_addr = { PHASE_NUM, 0 };

	hisi_check_and_no_retval((!hisifd || !hisifd->media_common_base), ERR, "null pointer\n");
	hisi_check_and_no_retval(((coef_lut_idx < 0) || (coef_lut_idx >= SCL_COEF_IDX_MAX)), ERR,
		"array index out of bound!\n");
	module_base = hisifd->media_common_base + DSS_WCH1_DMA_OFFSET + WCH_SCL_LUT_OFFSET;

	h0_y_addr = module_base + DSS_SCF_H0_Y_COEF_OFFSET;
	y_addr = module_base + DSS_SCF_Y_COEF_OFFSET;
	uv_addr = module_base + DSS_SCF_UV_COEF_OFFSET;

	lut_tap_addr.col = TAP6;
	ret = hisi_dss_scl_write_coefs(hisifd, false, h0_y_addr,
		(const int **)coef_lut_tap6[coef_lut_idx], lut_tap_addr);
	if (ret < 0)
		HISI_FB_ERR("Error to write H0_Y_COEF coefficients.\n");

	lut_tap_addr.col = TAP5;
	ret = hisi_dss_scl_write_coefs(hisifd, false, y_addr,
		(const int **)coef_lut_tap5[coef_lut_idx], lut_tap_addr);
	if (ret < 0)
		HISI_FB_ERR("Error to write Y_COEF coefficients.\n");

	lut_tap_addr.col = TAP4;
	ret = hisi_dss_scl_write_coefs(hisifd, false, uv_addr,
		(const int **)coef_lut_tap4[coef_lut_idx], lut_tap_addr);
	if (ret < 0)
		HISI_FB_ERR("Error to write UV_COEF coefficients.\n");
}

/* config value was reference chip manual */
void hisi_mdc_mctl_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *mctl_base = NULL;
	char __iomem *mctl_sys_base = NULL;

	hisi_check_and_no_retval((!hisifd || !hisifd->media_common_base), ERR, "null pointer\n");

	mctl_base = hisifd->media_common_base + DSS_MCTRL_CTL0_OFFSET;
	mctl_sys_base = hisifd->media_common_base + MCTL_SYS_OFFSET;

	set_reg(mctl_base + MCTL_CTL_EN, 0x1, 32, 0);

	/* clear ISP configuration */
	set_reg(mctl_base + MCTL_MUTEX_ISPIF, 0x0, 32, 0);
	set_reg(mctl_sys_base + MCTL_SYS_ISP_WCH_SEL, 0x0, 32, 0);
	set_reg(SOC_MDC_MOD0_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD1_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD2_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD6_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD8_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD9_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(SOC_MDC_MOD12_DBG_ADDR(mctl_sys_base), 0xA8000, 32, 0);
	set_reg(mctl_base + MCTL_CTL_TOP, 0x1, 32, 0);

	mctl_base = hisifd->media_common_base + DSS_MCTRL_CTL1_OFFSET;
	set_reg(mctl_base + MCTL_CTL_EN, 0x1, 32, 0);
	set_reg(mctl_base + MCTL_MUTEX_ISPIF, 0x0, 32, 0);
	set_reg(mctl_base + MCTL_CTL_TOP, 0x1, 32, 0);
}

static int hisi_mdc_power_on(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	int prev_refcount;

	HISI_FB_DEBUG("fb%u, +\n", hisifd->index);

	down(&hisifd->media_common_sr_sem);
	prev_refcount = hisifd->media_common_composer_sr_refcount++;
	if (prev_refcount == 0) {
		if (!hisifd->panel_power_on) {
			ret = hisifd->on_fnc(hisifd);
			if (ret == 0)
				hisifd->panel_power_on = true;
		}
	}
	up(&hisifd->media_common_sr_sem);

	HISI_FB_DEBUG("fb%u, ret=%d sr_refcount=%d -\n",
		hisifd->index, ret, hisifd->media_common_composer_sr_refcount);

	return ret;
}

int hisi_mdc_power_off(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	int new_refcount;

	down(&hisifd->media_common_sr_sem);
	new_refcount = --hisifd->media_common_composer_sr_refcount;
	if (new_refcount < 0)
		HISI_FB_ERR("dss new_refcount err");

	if (new_refcount == 0) {
		if (hisifd->panel_power_on) {
			hisifd->panel_power_on = false;
			ret = hisifd->off_fnc(hisifd);
			if (ret != 0)
				hisifd->panel_power_on = true;
		}
	}
	up(&hisifd->media_common_sr_sem);

	HISI_FB_DEBUG("fb%u, ret=%d sr_refcount=%d -\n",
		hisifd->index, ret, hisifd->media_common_composer_sr_refcount);

	return ret;
}

static int hisi_mdc_power_enable(struct hisi_fb_data_type *hisifd,
	const void __user *argp)
{
	int ret;
	int cmd = MDC_POWER_INVALD_CMD;

	hisi_check_and_return(!argp, -EINVAL, ERR, "argp is null!\n");

	ret = copy_from_user(&cmd, argp, sizeof(cmd));
	if (ret) {
		HISI_FB_ERR("fb%u, copy for user failed! ret=%d\n",
			hisifd->index, ret);
		return -EINVAL;
	}

	switch (cmd) {
	case MDC_POWER_UP_REQUEST:
		ret = hisi_mdc_power_on(hisifd);
		break;
	case MDC_POWER_DOWN_REQUEST:
		ret = hisi_mdc_power_off(hisifd);
		break;
	default:
		HISI_FB_INFO("fb%u input cmd=%d is invalid\n", hisifd->index, cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

void hisi_mdc_cmdlist_config_reset(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, uint32_t cmdlist_idxs)
{
	uint32_t i = 0;
	hisi_mdc_chn_res_t *resource = NULL;
	char __iomem *addr_base = NULL;
	uint32_t mctl_status;
	int delay_count = 0;

	hisi_check_and_no_retval((!hisifd || !hisifd->media_common_base), ERR, "null pointer\n");
	addr_base = hisifd->media_common_base + DSS_CMDLIST_OFFSET;
	set_reg(SOC_MDC_CMDLIST_SWRST_ADDR(addr_base), cmdlist_idxs, 32, 0);

	addr_base = hisifd->media_common_base + DSS_MCTRL_CTL0_OFFSET;
	if (pov_req->wb_compose_type == DSS_WB_COMPOSE_MEDIACOMMON)
		addr_base = hisifd->media_common_base + DSS_MCTRL_CTL1_OFFSET;

	set_reg(SOC_MDC_CTL_CLEAR_ADDR(addr_base), 0x1, 1, 0);
	do {
		mctl_status = inp32(SOC_MDC_CTL_STATUS_ADDR(addr_base));
		udelay(1);
		++delay_count;
	} while (((mctl_status & 0x10) != 0x0) && (delay_count <= DSS_COMPOSER_TIMEOUT_THRESHOLD_ASIC));

	if (delay_count > DSS_COMPOSER_TIMEOUT_THRESHOLD_ASIC) {
		HISI_FB_ERR("clear_ack_timeout, mctl_status=0x%x !\n",
			inp32(SOC_MDC_CTL_STATUS_ADDR(addr_base)));
		for (; i < ARRAY_SIZE(g_mdc_chn_res); i++) {
			resource = &(g_mdc_chn_res[i]);
			HISI_FB_ERR("chn%d: [DMA_BUF_DBG0]=0x%x [DMA_BUF_DBG1]=0x%x !\n",
				resource->chn_idx,
				inp32(SOC_MDC_DMA_BUF_DBG0_ADDR(hisifd->media_common_base + resource->chn_addr_base)),
				inp32(SOC_MDC_DMA_BUF_DBG1_ADDR(hisifd->media_common_base + resource->chn_addr_base)));
		}
	}
}
/*lint +e529*/

int hisi_mediacommon_ioctl_handler(struct hisi_fb_data_type *hisifd,
	uint32_t cmd, const void __user *argp)
{
	int ret = -ENOSYS;

	hisi_check_and_return((!hisifd || !argp), -EINVAL, ERR, "nullptr!\n");

	switch (cmd) {
	case HISIFB_MDC_POWER_DOWNUP_CTRL:
		ret = hisi_mdc_power_enable(hisifd, argp);
		break;
	case HISIFB_OV_MEDIA_COMMON_PLAY:
		ret = hisi_mdc_power_on(hisifd);
		if (ret != 0)
			break;
		if (hisifd->ov_media_common_play)
			ret = hisifd->ov_media_common_play(hisifd, argp);
		if (!g_enable_ovl_async_composer)
			hisi_mdc_power_off(hisifd);
		break;
	default:
		break;
	}

	if (ret == -ENOSYS)
		HISI_FB_ERR("fb%u unsupported ioctl=0x%x\n", hisifd->index, cmd);

	return ret;
}

