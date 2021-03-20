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

#ifdef SUPPORT_RCH_CLD
void hisi_dss_cld_init(const char __iomem *cld_base, struct dss_cld *s_cld)
{
	if (cld_base == NULL) {
		HISI_FB_ERR("cld_base is NULL!\n");
		return;
	}
	if (s_cld == NULL) {
		HISI_FB_ERR("s_cld is NULL!\n");
		return;
	}

	memset(s_cld, 0, sizeof(*s_cld));
	s_cld->size_vrt = inp32(cld_base + CLD_SIZE_VRT);
	s_cld->size_hrz = inp32(cld_base + CLD_SIZE_HRZ);
	s_cld->cld_rgb = inp32(cld_base + CLD_RGB);
	s_cld->cld_clk_en = inp32(cld_base + CH_CLK_EN);
	s_cld->cld_clk_sel = inp32(cld_base + CH_CLK_SEL);
	s_cld->cld_en = inp32(cld_base + CLD_EN);
}

void hisi_dss_cld_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *cld_base, struct dss_cld *s_cld, int channel)
{
	if (hisifd == NULL) {
		HISI_FB_DEBUG("hisifd is NULL\n");
		return;
	}
	if (cld_base == NULL) {
		HISI_FB_DEBUG("cld_base is NULL\n");
		return;
	}
	if (s_cld == NULL) {
		HISI_FB_DEBUG("s_cld is NULL\n");
		return;
	}

	if (channel != DSS_RCHN_D0)
		return;

	hisifd->set_reg(hisifd, cld_base + CLD_SIZE_VRT, s_cld->size_vrt, 32, 0);
	hisifd->set_reg(hisifd, cld_base + CLD_SIZE_HRZ, s_cld->size_hrz, 32, 0);
	hisifd->set_reg(hisifd, cld_base + CLD_RGB, s_cld->cld_rgb, 32, 0);
	hisifd->set_reg(hisifd, cld_base + CLD_EN, s_cld->cld_en, 32, 0);
	hisifd->set_reg(hisifd, cld_base + CH_CLK_EN, s_cld->cld_clk_en, 1, 10);
	hisifd->set_reg(hisifd, cld_base + CH_CLK_SEL, s_cld->cld_clk_sel, 1, 10);
}

int hisi_dss_cld_config(struct hisi_fb_data_type *hisifd, dss_layer_t *layer)
{
	struct dss_cld *cld = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}
	if (layer == NULL) {
		HISI_FB_ERR("layer is NULL\n");
		return -EINVAL;
	}

	if (layer->chn_idx != DSS_RCHN_D0)
		return 0;

	cld = &(hisifd->dss_module.cld);
	hisifd->dss_module.cld_used = 1;

	if (layer->is_cld_layer == 1) {
		cld->size_vrt = layer->src_rect.h - 1;  /* CLD height is rdma height minus 1. */
		cld->size_hrz = layer->src_rect.w - 1;  /*  CLD width is rdma width minus 1. */
		cld->cld_rgb  = layer->color;  /* Can be assigned if is necessary. */
		cld->cld_en   = 1;
	} else {
		cld->size_vrt = 0;  /* CLD height is default */
		cld->size_hrz = 0;  /* CLD width is default */
		cld->cld_rgb  = 0x000000;  /* CLD RGB is default */
		cld->cld_en   = 0;
	}

	return 0;
}

#endif
