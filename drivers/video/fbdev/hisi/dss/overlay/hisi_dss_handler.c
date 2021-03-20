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

#include "hisi_overlay_utils.h"
#include "../hisi_display_effect.h"
#include "../hisi_dpe_utils.h"
#include "../hisi_ovl_online_wb.h"
#include "../hisi_mmbuf_manager.h"
#include "../hisi_spr_dsc.h"
#include "../hisi_frame_rate_ctrl.h"

#define OV_COMPOSE_PRE 0
#define OV_COMPOSE_RDMA_IN 1
#define OV_COMPOSE_RDMA_OUT 2
#define OV_COMPOSE_RDFC_OUT 3
#define OV_COMPOSE_SCF_OUT 4

#ifdef SUPPORT_DSI_VER_2_0
static void unflow_mipi_ldi_int_handler(char __iomem *mipi_ldi_base, bool unmask)
{
	uint32_t temp;

	temp = inp32(mipi_ldi_base + MIPI_LDI_CPU_ITF_INT_MSK);  //lint !e529
	if (unmask)
		temp &= ~BIT_LDI_UNFLOW;
	else
		temp |= BIT_LDI_UNFLOW;

	outp32(mipi_ldi_base + MIPI_LDI_CPU_ITF_INT_MSK, temp);
}

void hisi_dss_unflow_handler(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, bool unmask)
{
	uint32_t temp = 0;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is null\n");
	hisi_check_and_no_retval(!pov_req, ERR, "pov_req is null\n");

	if (pov_req->ovl_idx == DSS_OVL0) {
		unflow_mipi_ldi_int_handler(hisifd->mipi_dsi0_base, unmask);
		if (is_dual_mipi_panel(hisifd))
			unflow_mipi_ldi_int_handler(hisifd->mipi_dsi1_base, unmask);
	} else if (pov_req->ovl_idx == DSS_OVL1) {
		if (is_dp_panel(hisifd)) {
			temp = inp32(hisifd->dss_base + DSS_LDI_DP_OFFSET + LDI_CPU_ITF_INT_MSK);  //lint !e529
			if (unmask)
				temp &= ~BIT_LDI_UNFLOW;
			else
				temp |= BIT_LDI_UNFLOW;

			outp32(hisifd->dss_base + DSS_LDI_DP_OFFSET + LDI_CPU_ITF_INT_MSK, temp);

			temp = inp32(hisifd->dss_base + DSS_LDI_DP1_OFFSET + LDI_CPU_ITF_INT_MSK);  //lint !e529
			if (unmask)
				temp &= ~BIT_LDI_UNFLOW;
			else
				temp |= BIT_LDI_UNFLOW;

			outp32(hisifd->dss_base + DSS_LDI_DP1_OFFSET + LDI_CPU_ITF_INT_MSK, temp);
		} else {
			unflow_mipi_ldi_int_handler(hisifd->mipi_dsi1_base, unmask);
		}
	} else {
		;  /* do nothing */
	}
}
#else
void hisi_dss_unflow_handler(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, bool unmask)
{
	uint32_t tmp = 0;
	char __iomem *ldi_base = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is null\n");
	hisi_check_and_no_retval(!pov_req, ERR, "pov_req is null\n");

	ldi_base = hisifd->dss_base + DSS_LDI1_OFFSET;

	if (pov_req->ovl_idx == DSS_OVL0) {
		tmp = inp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INT_MSK);
		if (unmask)
			tmp &= ~BIT_LDI_UNFLOW;
		else
			tmp |= BIT_LDI_UNFLOW;

		outp32(hisifd->dss_base + DSS_LDI0_OFFSET + LDI_CPU_ITF_INT_MSK, tmp);

	} else if (pov_req->ovl_idx == DSS_OVL1) {
		tmp = inp32(ldi_base + LDI_CPU_ITF_INT_MSK);
		if (unmask)
			tmp &= ~BIT_LDI_UNFLOW;
		else
			tmp |= BIT_LDI_UNFLOW;

		outp32(ldi_base + LDI_CPU_ITF_INT_MSK, tmp);
	} else {
		; /* do nothing */
	}
}
#endif

static bool is_base_layer(dss_layer_t *layer, dss_overlay_block_t *pov_h_block, bool *has_base)
{
	if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR)) {
		if (layer->need_cap & CAP_BASE)
			*has_base = true;

		return true;
	}

	if (layer->dst_rect.y < pov_h_block->ov_block_rect.y) {
		if (g_debug_ovl_block_composer)
			HISI_FB_INFO("%s=%d, %s=%d, %s=%d, %s=%d!!!!\n",
				"layer->dst_rect.y", layer->dst_rect.y,
				"pov_h_block->ov_block_rect.y", pov_h_block->ov_block_rect.y,
				"layer->chn_idx", layer->chn_idx,
				"layer->layer_idx", layer->layer_idx);

		return true;
	}

	return false;
}

static int hisi_ov_compose_base_config(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	dss_layer_t *layer, dss_rect_t *wb_ov_block_rect, bool *has_base)
{
	int ret;

	ret = hisi_dss_ovl_layer_config(hisifd, pov_req, layer, wb_ov_block_rect, *has_base);
	if (ret != 0) {
		HISI_FB_ERR("hisi_dss_ovl_config failed! need_cap=0x%x, ret=%d\n", layer->need_cap, ret);
		return ret;
	}

	ret = hisi_dss_mctl_ch_config(hisifd, layer, NULL, wb_ov_block_rect, *has_base);
	if (ret != 0) {
		HISI_FB_ERR("hisi_dss_mctl_ch_config failed! ret = %d\n", ret);
		return ret;
	}

	return ret;
}

static void ov_compose_print(uint8_t type, struct hisi_fb_data_type *hisifd, dss_layer_t *layer,
	dss_rect_ltrb_t *clip_rect, dss_rect_t *aligned_rect)
{
	if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer) {
		switch (type) {
		case OV_COMPOSE_RDMA_IN:
			HISI_FB_INFO("fb%d, rdma input, %s:%d,%d,%d,%d, %s%d,%d,%d,%d, %s:%d,%d,%d,%d\n",
				hisifd->index,
				"src_rect", layer->src_rect.x, layer->src_rect.y, layer->src_rect.w, layer->src_rect.h,
				"src_rect_mask", layer->src_rect_mask.x, layer->src_rect_mask.y,
				layer->src_rect_mask.w, layer->src_rect_mask.h,
				"dst_rect", layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h);

			break;
		case OV_COMPOSE_RDMA_OUT:
			HISI_FB_INFO("fb%d, rdma output, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d\n",
				hisifd->index,
				"clip_rect", clip_rect->left, clip_rect->right, clip_rect->top, clip_rect->bottom,
				"aligned_rect", aligned_rect->x, aligned_rect->y, aligned_rect->w, aligned_rect->h,
				"dst_rect", layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h);

			break;
		case OV_COMPOSE_RDFC_OUT:
			HISI_FB_INFO("fb%d, rdfc output, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d\n",
				hisifd->index,
				"clip_rect", clip_rect->left, clip_rect->right, clip_rect->top, clip_rect->bottom,
				"aligned_rect", aligned_rect->x, aligned_rect->y, aligned_rect->w, aligned_rect->h,
				"dst_rect", layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h);

			break;
		case OV_COMPOSE_SCF_OUT:
			HISI_FB_INFO("fb%d, scf output, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d, %s:%d,%d,%d,%d\n",
				hisifd->index,
				"clip_rect", clip_rect->left, clip_rect->right, clip_rect->top, clip_rect->bottom,
				"aligned_rect", aligned_rect->x, aligned_rect->y, aligned_rect->w, aligned_rect->h,
				"dst_rect", layer->dst_rect.x, layer->dst_rect.y, layer->dst_rect.w, layer->dst_rect.h);

			break;
		default:
			break;
		}
	}
}

static int hisi_ov_composer_spec_config(struct hisi_fb_data_type *hisifd,
	dss_layer_t *layer,
	struct hisi_ov_compose_rect *ov_compose_rect,
	struct hisi_ov_compose_flag *ov_compose_flag)
{
	int ret;
	dss_overlay_t *pov_req = &(hisifd->ov_req);

	ret = hisi_dss_smmu_ch_config(hisifd, layer, NULL);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_smmu_ch_config failed! ret = %d\n", ret);

	ov_compose_print(OV_COMPOSE_RDMA_IN, hisifd, layer, ov_compose_rect->clip_rect, ov_compose_rect->aligned_rect);
	ret = hisi_dss_rdma_config(hisifd, layer, ov_compose_rect, ov_compose_flag);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_rdma_config failed! ret = %d\n", ret);
	ov_compose_print(OV_COMPOSE_RDMA_OUT, hisifd, layer, ov_compose_rect->clip_rect, ov_compose_rect->aligned_rect);

	ret = hisi_dss_aif_ch_config(hisifd, pov_req, layer, ov_compose_rect->wb_dst_rect, NULL);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_aif_ch_config failed! ret = %d\n", ret);

	ret = hisi_dss_aif1_ch_config(hisifd, pov_req, layer, NULL, pov_req->ovl_idx);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_aif1_ch_config failed! ret = %d\n", ret);

	ret = hisi_dss_mif_config(hisifd, layer, NULL, ov_compose_flag->rdma_stretch_enable);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_mif_config failed! ret = %d\n", ret);

#ifdef SUPPORT_RCH_CLD
	ret = hisi_dss_cld_config(hisifd, layer);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_cld_config failed! ret = %d\n", ret);
#endif

	ret = hisi_dss_rdfc_config(hisifd, layer, ov_compose_rect->aligned_rect, *ov_compose_rect->clip_rect);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_rdfc_config failed! ret = %d\n", ret);
	ov_compose_print(OV_COMPOSE_RDFC_OUT, hisifd, layer, ov_compose_rect->clip_rect, ov_compose_rect->aligned_rect);

	ret = hisi_dss_scl_config(hisifd, layer, ov_compose_rect->aligned_rect, ov_compose_flag->rdma_stretch_enable);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_scl_config failed! ret = %d\n", ret);

	ret = hisi_dss_arsr2p_config(hisifd, layer, ov_compose_rect->aligned_rect,
		ov_compose_flag->rdma_stretch_enable);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_arsr2p_config failed! ret = %d\n", ret);

	ret = hisi_dss_post_clip_config(hisifd, layer);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_post_clip_config failed! ret = %d\n", ret);

	ov_compose_print(OV_COMPOSE_SCF_OUT, hisifd, layer, ov_compose_rect->clip_rect, ov_compose_rect->aligned_rect);

	if (ov_compose_flag->csc_needed) {
		ret = hisi_dss_csc_config(hisifd, layer, NULL);
		hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_csc_config failed! ret = %d\n", ret);
	}

	return ret;
}

int hisi_ov_compose_handler(struct hisi_fb_data_type *hisifd,
	dss_overlay_block_t *pov_h_block,
	dss_layer_t *layer,
	struct hisi_ov_compose_rect *ov_compose_rect,
	struct hisi_ov_compose_flag *ov_compose_flag)
{
	int ret;
	int32_t mctl_idx;
	dss_overlay_t *pov_req = &(hisifd->ov_req);

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");
	hisi_check_and_return(!pov_h_block, -EINVAL, ERR, "pov_h_block is NULL\n");
	hisi_check_and_return(!layer, -EINVAL, ERR, "layer is NULL\n");
	hisi_check_and_return(!ov_compose_rect, -EINVAL, ERR, "ov_compose_rect is NULL\n");
	hisi_check_and_return((!ov_compose_rect->aligned_rect || !ov_compose_rect->clip_rect),
		-EINVAL, ERR, "aligned_rect or clip_rect is NULL\n");
	hisi_check_and_return(!ov_compose_flag, -EINVAL, ERR, "ov_compose_flag is NULL\n");

	if ((hisifd->index == MEDIACOMMON_PANEL_IDX) && g_enable_ovl_async_composer) {
		;
	} else {
		ret = hisifd->fb_pan_display ? 0 : hisi_dss_check_layer_par(hisifd, layer);
		hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_check_layer_par failed! ret = %d\n", ret);
	}

	/* assume next frame use online play. */
	hisifd->fb_pan_display = false;
	if (g_debug_ovl_block_composer)
		HISI_FB_INFO("%s=%d, %s=%d, %s=%d, %s=%d\n",
			"layer->dst_rect.y", layer->dst_rect.y,
			"pov_h_block->ov_block_rect.y", pov_h_block->ov_block_rect.y,
			"layer->chn_idx", layer->chn_idx,
			"layer->layer_idx", layer->layer_idx);

	ov_compose_print(OV_COMPOSE_PRE, hisifd, layer, ov_compose_rect->clip_rect, ov_compose_rect->aligned_rect);

	if (is_base_layer(layer, pov_h_block, &ov_compose_flag->has_base) == true) {
		ret = hisi_ov_compose_base_config(hisifd, pov_req, layer, ov_compose_rect->wb_ov_block_rect,
			&ov_compose_flag->has_base);
		return ret;
	}

	ret = hisi_ov_composer_spec_config(hisifd, layer, ov_compose_rect, ov_compose_flag);
	if (ret != 0)
		return ret;

	ret = hisi_ov_compose_base_config(hisifd, pov_req, layer, ov_compose_rect->wb_ov_block_rect,
		&ov_compose_flag->has_base);
	if (ret != 0)
		return ret;

	mctl_idx = pov_req->ovl_idx;
	ret = hisi_dss_ch_module_set_regs(hisifd, mctl_idx, layer->chn_idx, 0, ov_compose_flag->enable_cmdlist);
	hisi_check_and_return((ret != 0), ret, ERR, "fb%d, hisi_dss_ch_module_set_regs failed! ret = %d\n",
		hisifd->index, ret);

	return 0;
}

int hisi_wb_compose_handler(struct hisi_fb_data_type *hisifd,
	dss_wb_layer_t *wb_layer,
	dss_rect_t *wb_ov_block_rect,
	bool last_block,
	struct hisi_ov_compose_flag ov_compose_flag)
{
	int ret = 0;
	dss_rect_t aligned_rect;
	dss_overlay_t *pov_req = NULL;

	memset(&aligned_rect, 0, sizeof(aligned_rect));

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL Point!\n");
	hisi_check_and_return(!wb_layer, -EINVAL, ERR, "wb_layer is NULL Point!\n");

	pov_req = &(hisifd->ov_req);

	if (ov_compose_flag.csc_needed) {
		ret = hisi_dss_csc_config(hisifd, NULL, wb_layer);
		hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_csc_config failed! ret = %d\n", ret);
	}

	if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer)
		HISI_FB_INFO("fb%d, wdfc input, src_rect:%d,%d,%d,%d, dst_rect:%d,%d,%d,%d.\n",
			hisifd->index,
			wb_layer->src_rect.x, wb_layer->src_rect.y, wb_layer->src_rect.w, wb_layer->src_rect.h,
			wb_layer->dst_rect.x, wb_layer->dst_rect.y, wb_layer->dst_rect.w, wb_layer->dst_rect.h);

	ret = hisi_dss_wb_scl_config(hisifd, wb_layer);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_wb_scl_config failed, ret = %d\n", ret);

	ret = hisi_dss_wdfc_config(hisifd, wb_layer, &aligned_rect, wb_ov_block_rect);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_wdfc_config failed, ret = %d\n", ret);

	if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer)
		HISI_FB_INFO("fb%d, wdfc output, aligned_rect:%d,%d,%d,%d, dst_rect:%d,%d,%d,%d.\n",
			hisifd->index, aligned_rect.x, aligned_rect.y, aligned_rect.w, aligned_rect.h,
			wb_layer->dst_rect.x, wb_layer->dst_rect.y, wb_layer->dst_rect.w, wb_layer->dst_rect.h);

	ret = hisi_dss_wdma_config(hisifd, wb_layer, aligned_rect, wb_ov_block_rect, last_block);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_wdma_config failed, ret = %d\n", ret);

	if (g_debug_ovl_online_composer || g_debug_ovl_offline_composer)
		HISI_FB_INFO("fb%d, wdma output, aligned_rect:%d,%d,%d,%d, dst_rect:%d,%d,%d,%d.\n",
			hisifd->index, aligned_rect.x, aligned_rect.y, aligned_rect.w, aligned_rect.h,
			wb_layer->dst_rect.x, wb_layer->dst_rect.y, wb_layer->dst_rect.w, wb_layer->dst_rect.h);

	ret = hisi_dss_mif_config(hisifd, NULL, wb_layer, false);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_mif_config failed! ret = %d\n", ret);

	ret = hisi_dss_aif1_ch_config(hisifd, pov_req, NULL, wb_layer, pov_req->ovl_idx);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_aif1_ch_config failed! ret = %d\n", ret);

	ret = hisi_dss_aif_ch_config(hisifd, pov_req, NULL, NULL, wb_layer);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_aif_ch_config failed! ret = %d\n", ret);

	ret = hisi_dss_smmu_ch_config(hisifd, NULL, wb_layer);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_smmu_ch_config failed! ret = %d\n", ret);

	ret = hisi_dss_mctl_ch_config(hisifd, NULL, wb_layer, wb_ov_block_rect, 0);
	hisi_check_and_return((ret != 0), ret, ERR, "hisi_dss_mctl_ch_config failed! ret = %d\n", ret);

	return 0;
}

int hisi_wb_ch_module_set_regs(struct hisi_fb_data_type *hisifd, uint32_t wb_type,
	dss_wb_layer_t *wb_layer, bool enable_cmdlist)
{
	int32_t mctl_idx;
	int ret;
	dss_overlay_t *pov_req_h_v = NULL;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL Point!\n");

	pov_req_h_v = &(hisifd->ov_req);

	/* chicago copybit */
	mctl_idx = (wb_layer->chn_idx == DSS_WCHN_W2) ? DSS_MCTL5 : pov_req_h_v->ovl_idx;
	ret = hisi_dss_ch_module_set_regs(hisifd, mctl_idx, wb_layer->chn_idx, wb_type, enable_cmdlist);
	hisi_check_and_return(ret != 0, ret, ERR, "fb%d, set reg fail, ret = %d\n", hisifd->index, ret);

	return 0;
}

static void hisi_vactive0_start_disable_ldi(struct hisi_fb_data_type *hisifd) {
	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE)
		disable_ldi(hisifd);
	else if ((hisifd->secure_ctrl.secure_status == hisifd->secure_ctrl.secure_event) &&
		(hisifd->secure_ctrl.secure_status == DSS_SEC_IDLE))
		disable_ldi(hisifd);
}

void hisi_vactive0_start_isr_handler(struct hisi_fb_data_type *hisifd)
{
	struct hisifb_vsync *vsync_ctrl = NULL;
	ktime_t pre_vactive_timestamp;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (is_mipi_cmd_panel(hisifd) && (hisifd->frame_update_flag == 0)) {
		hisifd->vactive0_start_flag = 1;

		hisi_vactive0_start_disable_ldi(hisifd);
	} else {
		hisifd->vactive0_start_flag++;
	}

	wake_up_interruptible_all(&hisifd->vactive0_start_wq);

	if (hisi_dss_is_async_play(hisifd)) {
		hisifd->asynchronous_vactive0_start_flag = 1;
		wake_up_interruptible_all(&hisifd->asynchronous_play_wq);
	}

	if (g_debug_online_vactive) {
		vsync_ctrl = &(hisifd->vsync_ctrl);

		pre_vactive_timestamp = vsync_ctrl->vactive_timestamp;
		vsync_ctrl->vactive_timestamp = ktime_get();

		HISI_FB_INFO("fb%d, VACTIVE =%llu, time_diff=%llu.\n", hisifd->index,
			ktime_to_ns(vsync_ctrl->vactive_timestamp),
			(ktime_to_ns(vsync_ctrl->vactive_timestamp) - ktime_to_ns(pre_vactive_timestamp)));
	}
}

void hisi_vactive0_end_isr_handler(struct hisi_fb_data_type *hisifd)
{
	uint32_t delaycount = 0;
	uint32_t task_name = 0;
	bool timeout = false;

	if ((hisifd == NULL) || (hisifd->index != PRIMARY_PANEL_IDX))
		return;

	if (hisifd->panel_info.mode_switch_state == PARA_UPDT_DOING)
		task_name = BIT(1);

	if (task_name) {
		while ((delaycount++) <= 5000) {  /* wait timers */
			if (task_name & BIT(1)) {
				if (hisifd->panel_info.mode_switch_state == PARA_UPDT_END)
					break;
			}
			udelay(10);
		}

		if (delaycount > 5000)  /* wait timers */
			timeout = true;
	}

	if (timeout)
		HISI_FB_ERR("wait task[0x%x] execution end timeout.\n", task_name);
}

static void hisi_set_ov_async_play(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	int ret;
	uint32_t timediff;
	struct timeval tv0;
	struct timeval tv1;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL Pointer\n");
		return;
	}

	if (hisifd->online_play_count < ONLINE_PLAY_LOG_PRINTF) {
		HISI_FB_INFO("[online_play_count = %d] fb%d online_play_count\n",
			hisifd->online_play_count, hisifd->index);
	}
	hisifd->online_play_count++;

	if (hisifd->ov_asynchronous_play != NULL) {
		if (g_debug_ovl_online_composer_timediff & 0x1)
			hisifb_get_timestamp(&tv0);

		down(&hisifd->blank_sem);
		ret = hisifd->ov_asynchronous_play(hisifd, argp);
		if (ret != 0)
			HISI_FB_ERR("fb%d ov_asynchronous_play failed!\n", hisifd->index);

		up(&hisifd->blank_sem);

		if (g_debug_ovl_online_composer_timediff & 0x1) {
			hisifb_get_timestamp(&tv1);
			timediff = hisifb_timestamp_diff(&tv0, &tv1);
			if (timediff >= g_debug_ovl_online_composer_time_threshold)
				HISI_FB_ERR("ONLING_IOCTL_TIMEDIFF is %u us!\n", timediff);
		}

		if (ret == 0) {
			if (hisifd->bl_update != NULL)
				hisifd->bl_update(hisifd);
		}
	}
}

static int hisifb_ov_online_play(struct hisi_fb_data_type *hisifd, void __user *argp)
{
	uint32_t timediff;
	struct timeval tv0;
	struct timeval tv1;
	int ret = 0;

	if (hisifd->online_play_count < ONLINE_PLAY_LOG_PRINTF)
		HISI_FB_INFO("[online_play_count = %d] fb%d HISIFB_OV_ONLINE_PLAY!\n",
			hisifd->online_play_count, hisifd->index);

	hisifd->online_play_count++;

	if (hisifd->ov_online_play != NULL) {
		if (g_debug_ovl_online_composer_timediff & 0x1)
			hisifb_get_timestamp(&tv0);

		down(&hisifd->blank_sem);
		ret = hisifd->ov_online_play(hisifd, argp);
		if (ret != 0)
			HISI_FB_ERR("fb%d ov_online_play failed!\n", hisifd->index);
		up(&hisifd->blank_sem);

		if (g_debug_ovl_online_composer_timediff & 0x1) {
			hisifb_get_timestamp(&tv1);
			timediff = hisifb_timestamp_diff(&tv0, &tv1);
			if (timediff >= g_debug_ovl_online_composer_time_threshold) //lint !e737 !e574
				HISI_FB_ERR("ONLING_IOCTL_TIMEDIFF is %u us!\n", timediff);
		}

		if (ret == 0) {
			if (hisifd->bl_update != NULL)
				hisifd->bl_update(hisifd);
		}
	}
	return ret;
}

int hisi_overlay_ioctl_handler(struct hisi_fb_data_type *hisifd,
	uint32_t cmd, void __user *argp)
{
	int ret = 0;
	struct hisi_panel_info *pinfo = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("NULL Pointer\n");
		return -EINVAL;
	}
	pinfo = &(hisifd->panel_info);

	switch (cmd) {
	case HISIFB_OV_ONLINE_PLAY:
		ret = hisifb_ov_online_play(hisifd, argp);
		break;
	case HISIFB_OV_OFFLINE_PLAY:
		if (hisifd->ov_offline_play != NULL) {
			ret = hisifd->ov_offline_play(hisifd, argp);
			if (ret < 0)
				HISI_FB_ERR("fb%d ov_offline_play failed!\n", hisifd->index);
		}
		break;
	case HISIFB_ONLINE_PLAY_BYPASS:
		ret = hisi_online_play_bypass(hisifd, argp);
		if (ret != 0)
			HISI_FB_ERR("fb%d online_play_bypass failed!\n", hisifd->index);
		break;
	case HISIFB_GET_RELEASE_AND_RETIRE_FENCE:
		ret = hisi_get_release_and_retire_fence(hisifd, argp);
		if (ret != 0)
			HISI_FB_INFO("fb%d get_release_and_retire_fence exit!\n", hisifd->index);
		break;
	case HISIFB_OV_ASYNC_PLAY:
		hisi_set_ov_async_play(hisifd, argp);
		break;
	default:
		break;
	}

	return ret;
}
