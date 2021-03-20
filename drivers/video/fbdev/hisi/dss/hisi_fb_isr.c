/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
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

#include "hisi_fb.h"
#include "hisi_mipi_dsi.h"
#include "hisi_frame_rate_ctrl.h"
#include "hisi_fb_debug.h"

/*******************************************************************************
 * handle isr
 */
static bool need_panel_mode_swtich(struct hisi_fb_data_type *hisifd, uint32_t isr_s2)
{

	if (hisifd->panel_mode_switch_isr_handler && hisifd->panel_info.panel_mode_swtich_support &&
		(hisifd->panel_info.mode_switch_to != hisifd->panel_info.current_mode)) {
		if (!(isr_s2 & BIT_LDI_UNFLOW))
			return true;
	}

	return false;
}

static int get_dfr_constraint(struct hisi_fb_data_type *hisifd)
{
	if (!is_lcd_dfr_support(hisifd))
		return 0;

	if (hisifd->panel_info.frm_rate_ctrl.status != FRM_UPDT_DONE) {
		HISI_FB_DEBUG("dfr is doing, delay mipi clk upt\n");
		return -1;
	}

	/* if the mipi clk update is not supported in the hight fps,
	 * then after the fps is switched to high fps,
	 * the previous mipi clk upt requirements need to be cancelled.
	 * At this time, dsi_bit_clk_upt and dsi_bit_clk need to be changed to the same.
	 */
	if ((hisifd->panel_info.dfr_constraint & DFR_SUPPORT_MIPI_CLK_UPT_ONLY_60HZ) &&
		hisifd->panel_info.fps != FPS_60HZ) {
		HISI_FB_DEBUG("fps=%d,cancel mipi clk upt,dsi_bit_clk_upt=%d,dsi_bit_clk=%d\n",
			hisifd->panel_info.fps, hisifd->panel_info.mipi.dsi_bit_clk_upt,
			hisifd->panel_info.mipi.dsi_bit_clk);
		hisifd->panel_info.mipi.dsi_bit_clk_upt = hisifd->panel_info.mipi.dsi_bit_clk;
		return -1;
	}

	return 0;
}

static bool need_dsi_bit_clk_upt(struct hisi_fb_data_type *hisifd)
{
	if (get_dfr_constraint(hisifd))
		return false;

	if (hisifd->mipi_dsi_bit_clk_upt_isr_handler && hisifd->panel_info.dsi_bit_clk_upt_support &&
		hisifd->panel_info.mipi.dsi_bit_clk_upt &&
		(hisifd->panel_info.mipi.dsi_bit_clk_upt != hisifd->panel_info.mipi.dsi_bit_clk))
		return true;

	return false;
}

static void hisifb_display_effect_flags_config(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->color_temperature_flag > 0)
		hisifd->color_temperature_flag--;

	if (hisifd->display_effect_flag > 0)
		hisifd->display_effect_flag--;

	if (hisifd->panel_info.xcc_set_in_isr_support && hisifd->xcc_coef_set == 1) {
		dpe_set_xcc_csc_value(hisifd);
		hisifd->xcc_coef_set = 0;
	}
}

static bool need_fps_upt(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->panel_info.fps_updt_support && hisifd->fps_upt_isr_handler &&
		(hisifd->panel_info.fps_updt != hisifd->panel_info.fps ||
		hisifd->panel_info.fps_updt_force_update) && !is_lcd_dfr_support(hisifd))
		return true;

	return false;
}

static void dss_pdp_isr_vactive0_end_handle(struct hisi_fb_data_type *hisifd,
	uint32_t isr_s2, struct hisifb_secure *secure_ctrl)
{
	hisifd->vactive0_end_flag = 1;

	hisifb_display_effect_flags_config(hisifd);

	if (hisifd->underflow_flag) {
		HISI_FB_ISR_INFO("mipi_dsi_reset_underflow_clear +\n");
		mipi_dsi_reset_underflow_clear(hisifd);
	} else {
#ifdef CONFIG_HISI_FB_ALSC
		hisi_alsc_store_data(hisifd, BIT_VACTIVE0_END);
#endif
		if (need_config_frame_rate_timing(hisifd)) {
			(void)mipi_dsi_frm_rate_para_set_reg(hisifd);
		} else if (hisifd->pipe_clk_ctrl.pipe_clk_updt_state == PARA_UPDT_DOING) {
			;
		} else if (need_panel_mode_swtich(hisifd, isr_s2)) {
			if (((uint32_t)inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CTRL) & 0x1) == 0)
				hisifd->panel_mode_switch_isr_handler(hisifd, hisifd->panel_info.mode_switch_to);

			hisifd->panel_info.mode_switch_state = PARA_UPDT_END;
		} else if (need_dsi_bit_clk_upt(hisifd)) {
			if (!(isr_s2 & BIT_VACTIVE0_START))
				hisifd->mipi_dsi_bit_clk_upt_isr_handler(hisifd);
		} else if (need_fps_upt(hisifd)) {
			hisifd->fps_upt_isr_handler(hisifd);
		}
	}
#ifdef CONFIG_DSS_SMMU_V3
	hisi_dss_mmu_lock_clear(hisifd);
#endif
	if (secure_ctrl->notify_secure_switch != NULL)
		secure_ctrl->notify_secure_switch(hisifd);

	if ((g_debug_ldi_underflow == 0) && (g_err_status & DSS_PDP_LDI_UNDERFLOW))
		g_err_status &= ~DSS_PDP_LDI_UNDERFLOW;
}

static void dss_pdp_isr_vactive0_start_handle(struct hisi_fb_data_type *hisifd, uint32_t isr_s2)
{
	if (hisifd->underflow_flag)
		HISI_FB_ISR_INFO("vactive end int of the underflow frame didnot come\n");

	if (hisifd->ov_vactive0_start_isr_handler != NULL)
		hisifd->ov_vactive0_start_isr_handler(hisifd);

#ifdef CONFIG_HISI_FB_ALSC
	hisi_alsc_store_data(hisifd, BIT_VACTIVE0_START);
#endif


	dfr_status_convert_on_isr_vstart(hisifd);


	if (need_update_frame_rate(hisifd))
		mipi_dsi_frm_rate_ctrl(hisifd, hisifd->panel_info.fps_updt);

	if ((hisifd->pipe_clk_ctrl.pipe_clk_updt_state == PARA_UPDT_NEED) ||
		(hisifd->pipe_clk_ctrl.pipe_clk_updt_state == PARA_UPDT_DOING)) {
		disable_ldi(hisifd);
		hisifd->pipe_clk_ctrl.pipe_clk_updt_state = PARA_UPDT_DOING;

		if (hisifd->pipe_clk_updt_isr_handler != NULL)
			hisifd->pipe_clk_updt_isr_handler(hisifd);
	}

	if (need_panel_mode_swtich(hisifd, isr_s2)) {
		disable_ldi(hisifd);
		hisifd->panel_info.mode_switch_state = PARA_UPDT_DOING;
	}
}

static void dss_debug_underflow_and_clear_base(struct hisi_fb_data_type *hisifd)
{
	if (g_debug_ldi_underflow_clear) {
		if (is_mipi_cmd_panel(hisifd)) {
			if (g_ldi_data_gate_en == 0) {
				if (hisifd->ldi_underflow_wq) {
					disable_ldi(hisifd);
					queue_work(hisifd->ldi_underflow_wq, &hisifd->ldi_underflow_work);
				}
			}
		} else {
			if (hisifd->ldi_underflow_wq) {
				disable_ldi(hisifd);
				queue_work(hisifd->ldi_underflow_wq, &hisifd->ldi_underflow_work);

#if defined(CONFIG_HUAWEI_DSM)
				if (hisifd->dss_underflow_debug_workqueue)
					queue_work(hisifd->dss_underflow_debug_workqueue,
						&hisifd->dss_underflow_debug_work);
#endif
			}
		}
	}

	if (g_debug_ldi_underflow) {
		if (g_debug_ovl_online_composer) {
			if (hisifd->dss_debug_wq)
				queue_work(hisifd->dss_debug_wq, &hisifd->dss_debug_work);
		}
	}
}

static void dss_te_vsync_isr_handle_base(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->vsync_isr_handler)
		hisifd->vsync_isr_handler(hisifd);

	if (hisifd->buf_sync_signal)
		hisifd->buf_sync_signal(hisifd);
}

#ifdef SUPPORT_DSI_VER_2_0
static void start_dss_clearing_work(struct hisi_fb_data_type *hisifd)
{
	if (g_debug_ldi_underflow_clear == 0)
		return;

	if (is_mipi_cmd_panel(hisifd) && g_ldi_data_gate_en)
		return;

	hisifd->underflow_flag = 1;
	disable_ldi(hisifd);
	hisi_dump_current_info(hisifd);

	if (hisifd->ldi_underflow_wq)
		queue_work(hisifd->ldi_underflow_wq, &hisifd->ldi_underflow_work);
}

static void dss_isr_underflow_handle(struct hisi_fb_data_type *hisifd, int dsi_idx)
{
	static uint32_t frame_no[2] = {0};  /* dsi[0,1] */

	if (!spin_trylock(&hisifd->underflow_lock)) {
		HISI_FB_ISR_INFO("dsi%d get underflow_lock fail\n", dsi_idx);
		return;
	}
	frame_no[dsi_idx] = hisifd->ov_req.frame_no;
	if (frame_no[0] != 0 && frame_no[0] == frame_no[1]) {
		HISI_FB_ISR_INFO("same frame, dsi%d return\n", dsi_idx);
		spin_unlock(&hisifd->underflow_lock);
		return;
	}
	spin_unlock(&hisifd->underflow_lock);

	HISI_FB_ISR_INFO("dsi%d underflow handle +\n", dsi_idx);
#ifdef CONFIG_DSS_SMMU_V3
	hisi_dss_mmu_lock_clear(hisifd);
#endif
	start_dss_clearing_work(hisifd);

	if (g_debug_ldi_underflow && g_debug_ovl_online_composer && hisifd->dss_debug_wq)
		queue_work(hisifd->dss_debug_wq, &hisifd->dss_debug_work);

	g_err_status |= DSS_PDP_LDI_UNDERFLOW;

#if defined(CONFIG_HUAWEI_DSM)
	if ((!g_fake_lcd_flag) && (hisifd->ldi_data_gate_en == 0) && hisifd->dss_underflow_debug_workqueue)
		queue_work(hisifd->dss_underflow_debug_workqueue, &hisifd->dss_underflow_debug_work);
#endif
}

static void dss_dsi0_isr_for_te_vactive0_start(struct hisi_fb_data_type *hisifd, uint32_t isr_s2)
{
	uint32_t isr_te_vsync;

	if (is_mipi_cmd_panel(hisifd))
		isr_te_vsync = BIT_LCD_TE0_PIN;
	else
		isr_te_vsync = BIT_VSYNC;

	if (hisi_dss_is_async_play(hisifd)) {
		if (isr_s2 & isr_te_vsync) {
			hisifd->te_timestamp = ktime_get();
			dss_te_vsync_isr_handle_base(hisifd);
		}

		if (isr_s2 & BIT_VACTIVE0_START) {
			dss_pdp_isr_vactive0_start_handle(hisifd, isr_s2);
		}
	} else {
		if (isr_s2 & BIT_VACTIVE0_START) {
			dss_pdp_isr_vactive0_start_handle(hisifd, isr_s2);
		}

		if (isr_s2 & isr_te_vsync) {
			hisifd->te_timestamp = ktime_get();
			dss_te_vsync_isr_handle_base(hisifd);
		}
	}
}

irqreturn_t dss_dsi0_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s2;
	uint32_t mask;
	struct hisifb_secure *secure_ctrl = NULL;
	char __iomem *mipi_dsi_base = NULL;

	hisifd = (struct hisi_fb_data_type *)ptr;
	if (!hisifd) {
		HISI_FB_ISR_INFO("hisifd is NULL\n");
		return IRQ_NONE;
	}

	secure_ctrl = &(hisifd->secure_ctrl);

	mipi_dsi_base = get_mipi_dsi_base(hisifd);
	isr_s2 = inp32(mipi_dsi_base + MIPI_LDI_CPU_ITF_INTS);
	outp32(mipi_dsi_base + MIPI_LDI_CPU_ITF_INTS, isr_s2);
	isr_s2 &= ~((uint32_t)inp32(mipi_dsi_base + MIPI_LDI_CPU_ITF_INT_MSK));

	if (isr_s2 & BIT_VACTIVE0_END)
		dss_pdp_isr_vactive0_end_handle(hisifd, isr_s2, secure_ctrl);

	dss_dsi0_isr_for_te_vactive0_start(hisifd, isr_s2);


	if (isr_s2 & BIT_LDI_UNFLOW) {
		mask = inp32(mipi_dsi_base + MIPI_LDI_CPU_ITF_INT_MSK);
		mask |= BIT_LDI_UNFLOW;
		outp32(mipi_dsi_base + MIPI_LDI_CPU_ITF_INT_MSK, mask);
		if (is_dual_mipi_panel(hisifd))
			outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK, mask);

		HISI_FB_ISR_INFO("dsi0 underflow frame_no %d, isr 0x%x, dpp_dbg 0x%x, online_fill_level 0x%x\n",
			hisifd->ov_req.frame_no, isr_s2,
			inp32(hisifd->dss_base + DSS_DISP_CH0_OFFSET + DISP_CH_DBG_CNT),
			inp32(hisifd->dss_base + DSS_DBUF0_OFFSET + DBUF_ONLINE_FILL_LEVEL));
		dss_isr_underflow_handle(hisifd, 0);
	}

	return IRQ_HANDLED;
}

irqreturn_t dss_dsi1_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct hisifb_secure *secure_ctrl = NULL;
	uint32_t isr_s2;
	uint32_t mask;
	uint32_t isr_te_vsync = BIT_VSYNC;

	hisifd = (struct hisi_fb_data_type *)ptr;
	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL!\n");

	isr_s2 = inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INTS);
	outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INTS, isr_s2);
	isr_s2 &= ~((uint32_t)inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK));

	HISI_FB_DEBUG("isr_s2 0x%x, mask 0x%x", isr_s2,
		inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK));

	if ((hisifd->index == PRIMARY_PANEL_IDX) && is_dual_mipi_panel(hisifd)) {
		if (isr_s2 & BIT_LDI_UNFLOW) {
			mask = inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK);
			mask |= BIT_LDI_UNFLOW;
			outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK, mask);
			HISI_FB_ISR_INFO("dsi1 underflow frame_no %d, isr 0x%x, dpp_dbg 0x%x, online_fill_level 0x%x\n",
				hisifd->ov_req.frame_no, isr_s2,
				inp32(hisifd->dss_base + DSS_DISP_CH0_OFFSET + DISP_CH_DBG_CNT),
				inp32(hisifd->dss_base + DSS_DBUF0_OFFSET + DBUF_ONLINE_FILL_LEVEL));
			dss_isr_underflow_handle(hisifd, 1);
		}
		return IRQ_HANDLED;
	}

	secure_ctrl = &(hisifd->secure_ctrl);
	if (is_mipi_cmd_panel(hisifd))
		isr_te_vsync = BIT_LCD_TE0_PIN;

	if (hisifd->panel_info.mipi.dsi_te_type == DSI1_TE1_TYPE)
		isr_te_vsync = BIT_LCD_TE1_PIN;

	if (isr_s2 & BIT_VACTIVE0_END)
		dss_pdp_isr_vactive0_end_handle(hisifd, isr_s2, secure_ctrl);

	if (isr_s2 & BIT_VACTIVE0_START)
		dss_pdp_isr_vactive0_start_handle(hisifd, isr_s2);

	if (isr_s2 & isr_te_vsync)
		dss_te_vsync_isr_handle_base(hisifd);

	if (isr_s2 & BIT_LDI_UNFLOW) {
		mask = inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK);
		mask |= BIT_LDI_UNFLOW;
		outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK, mask);

		dss_debug_underflow_and_clear_base(hisifd);
		g_err_status |= DSS_SDP_LDI_UNDERFLOW;

#ifdef CONFIG_HISI_FB_V600
		dss_isr_underflow_handle(hisifd, 1);
#endif

		HISI_FB_ISR_INFO("ldi1 underflow! frame_no = %d!\n", hisifd->ov_req.frame_no);
	}

	return IRQ_HANDLED;
}

irqreturn_t dss_sdp_isr_mipi_panel(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2_smmu;

	hisifd = (struct hisi_fb_data_type *)ptr;
	if (!hisifd) {
		HISI_FB_ISR_INFO("hisifd is NULL\n");
		return IRQ_NONE;
	}

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_SDP_INTS);

	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);

	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(hisifd->dss_base + GLB_CPU_SDP_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_SDP_INT_MSK));

	return IRQ_HANDLED;
}

irqreturn_t dss_sdp_isr_dp(int irq, void *ptr)
{
	char __iomem *ldi_base = NULL;
	char __iomem *ldi_base1 = NULL;
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2;
	uint32_t isr_s3;
	uint32_t isr_s2_smmu;
	uint32_t mask;
	uint32_t temp;

	hisifd = (struct hisi_fb_data_type *)ptr;
	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL!\n");

	ldi_base = hisifd->dss_base + DSS_LDI_DP_OFFSET;
	ldi_base1 = hisifd->dss_base + DSS_LDI_DP1_OFFSET;

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_DP_INTS);
	isr_s2 = inp32(ldi_base + LDI_CPU_ITF_INTS);
	isr_s3 = inp32(ldi_base1 + LDI_CPU_ITF_INTS);
	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);

	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(ldi_base + LDI_CPU_ITF_INTS, isr_s2);
	outp32(ldi_base1 + LDI_CPU_ITF_INTS, isr_s3);
	outp32(hisifd->dss_base + GLB_CPU_DP_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_DP_INT_MSK));
	isr_s2 &= ~((uint32_t)inp32(ldi_base + LDI_CPU_ITF_INT_MSK));

	if (isr_s2 & BIT_VACTIVE0_END)
		hisifd->vactive0_end_flag = 1;

	if (isr_s2 & BIT_VACTIVE0_START) {
		if (hisifd->ov_vactive0_start_isr_handler)
			hisifd->ov_vactive0_start_isr_handler(hisifd);
	}

	if (isr_s2 & BIT_VSYNC)
		dss_te_vsync_isr_handle_base(hisifd);

	if (isr_s2 & BIT_LDI_UNFLOW) {
		mask = inp32(ldi_base + LDI_CPU_ITF_INT_MSK);
		mask |= BIT_LDI_UNFLOW;
		outp32(ldi_base + LDI_CPU_ITF_INT_MSK, mask);

		dss_debug_underflow_and_clear_base(hisifd);

		g_err_status |= DSS_SDP_LDI_UNDERFLOW;
		temp = inp32(hisifd->dss_base + DSS_DPP1_OFFSET + DPP_DBG_CNT);

		HISI_FB_ISR_INFO("ldi1 underflow! frame_no = %d, dpp_dbg = 0x%x\n", hisifd->ov_req.frame_no, temp);
		hisi_dump_current_info(hisifd);
	}

	return IRQ_HANDLED;
}

irqreturn_t dss_pdp_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2_dpp;
	uint32_t isr_s2_smmu;
	uint32_t isr_s2_wb;
	uint32_t mask;

	struct hisifb_video_idle_ctrl *video_idle_ctrl = NULL;

	hisifd = (struct hisi_fb_data_type *)ptr;
	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL!\n");

	video_idle_ctrl = &(hisifd->video_idle_ctrl);

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_PDP_INTS);
	isr_s2_dpp = inp32(hisifd->dss_base + DSS_DISP_CH0_OFFSET + DPP_INTS);

	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);
	isr_s2_wb = inp32(hisifd->dss_base + DSS_WB_OFFSET + WB_ONLINE_ERR_INTS);

	outp32(hisifd->dss_base + DSS_WB_OFFSET + WB_ONLINE_ERR_INTS, isr_s2_wb);
	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(hisifd->dss_base + DSS_DISP_CH0_OFFSET + DPP_INTS, isr_s2_dpp);

	outp32(hisifd->dss_base + GLB_CPU_PDP_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_PDP_INT_MSK));
	isr_s2_dpp &= ~((uint32_t)inp32(hisifd->dss_base + DSS_DISP_CH0_OFFSET + DPP_INT_MSK));

	if (isr_s1 & BIT_DSS_WB_ERR_INTS) {
		mask = inp32(hisifd->dss_base + GLB_CPU_PDP_INT_MSK);
		mask |= BIT_DSS_WB_ERR_INTS;
		outp32(hisifd->dss_base + GLB_CPU_PDP_INT_MSK, mask);
		HISI_FB_ISR_INFO("isr_s1 = 0x%x, isr_s2_wb = 0x%x\n", isr_s1, isr_s2_wb);
		queue_work(video_idle_ctrl->idle_wb_err_wq, &video_idle_ctrl->idle_wb_err_work);
	}

	if (isr_s2_dpp & BIT_HIACE_IND) {
		if (hisifd->panel_info.hiace_support && hisifd->hiace_end_wq)
			hisi_effect_hiace_trigger_wq(hisifd);
	}

	return IRQ_HANDLED;
}

#else
static void dss_te_vsync_isr_handle(struct hisi_fb_data_type *hisifd)
{
	uint32_t temp;
	struct timeval tv;
	uint32_t isr_s2 = 0;

	hisifd->te_timestamp = ktime_get();

	if (hisifd->panel_info.delayed_cmd_queue_support && hisifd->delayed_cmd_queue_wq &&
		mipi_dsi_check_delayed_cmd_queue_working()) {
		mipi_dsi_set_timestamp();
		queue_work(hisifd->delayed_cmd_queue_wq, &hisifd->delayed_cmd_queue_work);
	}

	dss_te_vsync_isr_handle_base(hisifd);

	if (g_err_status & (DSS_PDP_LDI_UNDERFLOW | DSS_PDP_SMMU_ERR | DSS_SDP_SMMU_ERR)) {
		temp = inp32(hisifd->dss_base + DSS_DPP_OFFSET + DPP_DBG_CNT);
		g_err_status &= ~DSS_PDP_LDI_UNDERFLOW;
		HISI_FB_ISR_INFO("isr_te_vsync:frame_no = %d,dpp_dbg = 0x%x!\n", hisifd->ov_req.frame_no, temp);
	}

	if (g_debug_ldi_underflow) {
		hisifb_get_timestamp(&tv);
		HISI_FB_ISR_INFO("isr_te_vsync:frame_no = %d,isr_s2 = 0x%x\n", hisifd->ov_req.frame_no, isr_s2);
	}
}

static void dss_debug_underflow_and_clear(struct hisi_fb_data_type *hisifd)
{
	if (g_debug_ldi_underflow_clear) {
		if (is_mipi_cmd_panel(hisifd)) {
			if (g_ldi_data_gate_en == 0) {
				if (hisifd->ldi_underflow_wq != NULL) {
					hisifb_pipe_clk_set_underflow_flag(hisifd, true);
					disable_ldi(hisifd);
					queue_work(hisifd->ldi_underflow_wq, &hisifd->ldi_underflow_work);
				}
			}
		} else {
			if (hisifd->ldi_underflow_wq != NULL) {
				hisifb_pipe_clk_set_underflow_flag(hisifd, true);
				disable_ldi(hisifd);
				queue_work(hisifd->ldi_underflow_wq, &hisifd->ldi_underflow_work);
			}
		}
	}

	if (g_debug_ldi_underflow) {
		if (g_debug_ovl_online_composer) {
			if (hisifd->dss_debug_wq != NULL)
				queue_work(hisifd->dss_debug_wq, &hisifd->dss_debug_work);
		}
	}
}

static void dss_hiace_ind_handle(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->panel_info.hiace_support && hisifd->hiace_end_wq) {
		/* check if reading fna in isr first, if yes, no need queue work */
#ifdef HIACE_SINGLE_MODE_SUPPORT
		if (!hisi_hiace_single_mode_handle_isr(hisifd))
			hisi_display_effect_hiace_trigger_wq(hisifd, hisifd->is_idle_display);
#else
		hisi_display_effect_hiace_trigger_wq(hisifd, hisifd->is_idle_display);
#endif
	}
}

static void dss_display_effect_handle(struct hisi_fb_data_type *hisifd, uint32_t isr_s2, uint32_t isr_s2_dpp)
{
	if (isr_s2_dpp & BIT_HIACE_IND)
		dss_hiace_ind_handle(hisifd);

	if (isr_s2_dpp & BIT_CE_END_IND) {
		if (hisifd->panel_info.acm_ce_support && hisifd->dpp_ce_end_wq)
			queue_work(hisifd->dpp_ce_end_wq, &hisifd->dpp_ce_end_work);
	}
}

static void dss_pdp_te_vsync_isr_handle(struct hisi_fb_data_type *hisifd, uint32_t isr_s2)
{
	uint32_t isr_te_vsync;

	if (is_mipi_cmd_panel(hisifd))
		isr_te_vsync = BIT_LCD_TE0_PIN;
	else
		isr_te_vsync = BIT_VSYNC;

	if (isr_s2 & isr_te_vsync)
		dss_te_vsync_isr_handle(hisifd);
}

irqreturn_t dss_pdp_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2;
	uint32_t isr_s2_dpp;
	uint32_t isr_s2_smmu;
	uint32_t mask;
	uint32_t temp;

	hisifd = (struct hisi_fb_data_type *)ptr;
	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL!\n");

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_PDP_INTS);
	isr_s2 = inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INTS);
	isr_s2_dpp = inp32(hisifd->dss_base + DSS_DPP_OFFSET + DPP_INTS);
	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);

	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(hisifd->dss_base + DSS_DPP_OFFSET + DPP_INTS, isr_s2_dpp);
	outp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INTS, isr_s2);
	outp32(hisifd->dss_base + GLB_CPU_PDP_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_PDP_INT_MSK));
	isr_s2 &= ~((uint32_t)inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INT_MSK));
	isr_s2_dpp &= ~((uint32_t)inp32(hisifd->dss_base + DSS_DPP_OFFSET + DPP_INT_MSK));

	if (isr_s2 & BIT_VACTIVE0_END)
		dss_pdp_isr_vactive0_end_handle(hisifd, isr_s2, &(hisifd->secure_ctrl));

	if (isr_s2 & BIT_VACTIVE0_START)
		dss_pdp_isr_vactive0_start_handle(hisifd, isr_s2);

	dss_pdp_te_vsync_isr_handle(hisifd, isr_s2);

	if (isr_s2 & BIT_LDI_UNFLOW) {
		mask = inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INT_MSK);
		mask |= BIT_LDI_UNFLOW;
		outp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INT_MSK, mask);

		hisi_dump_current_info(hisifd);

		dss_debug_underflow_and_clear(hisifd);

		g_err_status |= DSS_PDP_LDI_UNDERFLOW;

		if (hisifd->ldi_data_gate_en == 0) {
			temp = inp32(hisifd->dss_base + DSS_DPP_OFFSET + DPP_DBG_CNT);
			HISI_FB_ISR_INFO("ldi underflow! frame_no = %d,dpp_dbg = 0x%x, vactive0_start_flag = %d!\n",
				hisifd->ov_req.frame_no, temp, hisifd->vactive0_start_flag);

#if defined(CONFIG_HUAWEI_DSM)
			if (hisifd->dss_underflow_debug_workqueue && !g_fake_lcd_flag)
				queue_work(hisifd->dss_underflow_debug_workqueue, &hisifd->dss_underflow_debug_work);
#endif
		}
	}

	dss_display_effect_handle(hisifd, isr_s2, isr_s2_dpp);

	return IRQ_HANDLED;
}
#endif

static void dss_sdp_handle_isr_s2(struct hisi_fb_data_type *hisifd, char __iomem *ldi_base, uint32_t isr_s2)
{
	uint32_t mask;

	if (isr_s2 & BIT_VACTIVE0_END)
		hisifd->vactive0_end_flag = 1;

	if (isr_s2 & BIT_VACTIVE0_START) {
		if (hisifd->ov_vactive0_start_isr_handler != NULL)
			hisifd->ov_vactive0_start_isr_handler(hisifd);
	}

	if (isr_s2 & BIT_VSYNC)
		dss_te_vsync_isr_handle_base(hisifd);

	if (isr_s2 & BIT_LDI_UNFLOW) {
		mask = inp32(ldi_base + LDI_CPU_ITF_INT_MSK);
		mask |= BIT_LDI_UNFLOW;
		outp32(ldi_base + LDI_CPU_ITF_INT_MSK, mask);

		dss_debug_underflow_and_clear_base(hisifd);

		g_err_status |= DSS_SDP_LDI_UNDERFLOW;

		HISI_FB_ISR_INFO("ldi1 underflow! frame_no = %d!\n", hisifd->ov_req.frame_no);
	}
}

irqreturn_t dss_sdp_isr(int irq, void *ptr)
{
	char __iomem *ldi_base = NULL;
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2;
	uint32_t isr_s2_smmu;

	hisifd = (struct hisi_fb_data_type *)ptr;

	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL\n");

	ldi_base = hisifd->dss_base + DSS_LDI1_OFFSET;

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_SDP_INTS);
	isr_s2 = inp32(ldi_base + LDI_CPU_ITF_INTS);
	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);

	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(ldi_base + LDI_CPU_ITF_INTS, isr_s2);
	outp32(hisifd->dss_base + GLB_CPU_SDP_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_SDP_INT_MSK));
	isr_s2 &= ~((uint32_t)inp32(ldi_base + LDI_CPU_ITF_INT_MSK));

	dss_sdp_handle_isr_s2(hisifd, ldi_base, isr_s2);

	return IRQ_HANDLED;
}

static void adp_isr_wch_ints(struct hisi_fb_data_type *hisifd, uint32_t isr_s1, uint32_t isr_s3_copybit)
{
	if (isr_s1 & BIT_OFF_WCH0_INTS) {
		if (hisifd->cmdlist_info->cmdlist_wb_flag[WB_TYPE_WCH0] == 1) {
			hisifd->cmdlist_info->cmdlist_wb_done[WB_TYPE_WCH0] = 1;
			wake_up_interruptible_all(&(hisifd->cmdlist_info->cmdlist_wb_wq[WB_TYPE_WCH0]));
		}
	}

	if (isr_s1 & BIT_OFF_WCH1_INTS) {
		if (hisifd->cmdlist_info->cmdlist_wb_flag[WB_TYPE_WCH1] == 1) {
			hisifd->cmdlist_info->cmdlist_wb_done[WB_TYPE_WCH1] = 1;
			wake_up_interruptible_all(&(hisifd->cmdlist_info->cmdlist_wb_wq[WB_TYPE_WCH1]));
		}
	}

	if (isr_s1 & BIT_OFF_WCH0_WCH1_FRM_END_INT) {
		if (hisifd->cmdlist_info->cmdlist_wb_flag[WB_TYPE_WCH0_WCH1] == 1) {
			hisifd->cmdlist_info->cmdlist_wb_done[WB_TYPE_WCH0_WCH1] = 1;
			wake_up_interruptible_all(&(hisifd->cmdlist_info->cmdlist_wb_wq[WB_TYPE_WCH0_WCH1]));
		}
	}

#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	if (isr_s3_copybit & BIT_OFF_CAM_WCH2_FRMEND_INTS) {
		if (hisifd->copybit_info->copybit_flag == 1) {
			hisifd->copybit_info->copybit_done = 1;
			wake_up_interruptible_all(&(hisifd->copybit_info->copybit_wq));
		}

		if (hisifd->cmdlist_info->cmdlist_wb_flag[WB_TYPE_WCH2] == 1) {
			hisifd->cmdlist_info->cmdlist_wb_done[WB_TYPE_WCH2] = 1;
			wake_up_interruptible_all(&(hisifd->cmdlist_info->cmdlist_wb_wq[WB_TYPE_WCH2]));
		}
	}
#else
	(void)isr_s3_copybit;
#endif
}

irqreturn_t dss_adp_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;
	uint32_t isr_s2_smmu;
	uint32_t isr_s3_copybit = 0;

	hisifd = (struct hisi_fb_data_type *)ptr;
	hisi_check_and_return(!hisifd, IRQ_NONE, ISR_INFO, "hisifd is NULL\n");

	isr_s1 = inp32(hisifd->dss_base + GLB_CPU_OFF_INTS);
	isr_s2_smmu = inp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTSTAT_NS);

	outp32(hisifd->dss_base + DSS_SMMU_OFFSET + SMMU_INTCLR_NS, isr_s2_smmu);
	outp32(hisifd->dss_base + GLB_CPU_OFF_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_OFF_INT_MSK));
#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	isr_s3_copybit = inp32(hisifd->dss_base + GLB_CPU_OFF_CAM_INTS);
	outp32(hisifd->dss_base + GLB_CPU_OFF_CAM_INTS, isr_s3_copybit);
	isr_s3_copybit &= ~((uint32_t)inp32(hisifd->dss_base + GLB_CPU_OFF_CAM_INT_MSK));
#endif


	adp_isr_wch_ints(hisifd, isr_s1, isr_s3_copybit);

	return IRQ_HANDLED;
}

irqreturn_t dss_mdc_isr(int irq, void *ptr)
{
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t isr_s1;

	hisifd = (struct hisi_fb_data_type *)ptr;
	if (!hisifd) {
		HISI_FB_ISR_INFO("hisifd is NULL\n");
		return IRQ_NONE;
	}

	isr_s1 = inp32(hisifd->media_common_base + GLB_CPU_OFF_INTS);
	outp32(hisifd->media_common_base + GLB_CPU_OFF_INTS, isr_s1);

	isr_s1 &= ~((uint32_t)inp32(hisifd->media_common_base + GLB_CPU_OFF_INT_MSK));
	if (isr_s1 & (BIT_OFF_WCH1_INTS | BIT_OFF_WCH0_INTS)) {
		if (hisifd->media_common_info->mdc_flag == 1) {
			hisifd->media_common_info->mdc_done = 1;
			wake_up_interruptible_all(&(hisifd->media_common_info->mdc_wq));
		}
	}

	return IRQ_HANDLED;
}

