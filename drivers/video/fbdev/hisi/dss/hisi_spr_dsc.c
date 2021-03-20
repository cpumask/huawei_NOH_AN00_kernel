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
 */

#include "hisi_fb.h"
#include "hisi_spr_dsc.h"

#define SPR_LUT_LEN 65
#define SPR_LUT_INDEX 6

#ifdef CONFIG_HISI_FB_V600
const uint32_t spr_lut_addr[SPR_LUT_INDEX] = {
	SPR_GAMMA_LUT_R,
	SPR_GAMMA_LUT_G,
	SPR_GAMMA_LUT_B,
	SPR_DEGAMMA_LUT_R,
	SPR_DEGAMMA_LUT_G,
	SPR_DEGAMMA_LUT_B,
};
#endif

static void spr_coef_offset_config(char __iomem *spr_coef_base,
	uint32_t offset, uint32_t coef[])
{
	/* set one group coefficients using 3 registers */
	outp32(spr_coef_base + offset, coef[0]);
	outp32(spr_coef_base + offset + 4, coef[1]);
	outp32(spr_coef_base + offset + 8, coef[2]);
}

static void spr_init_coeff(char __iomem *spr_base, struct spr_dsc_panel_para *spr)
{
	uint32_t offset = 0;

	/* set Red's v0h0 v0h1 v1h0 v1h1 coefficients */
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_r_v0h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_r_v0h1_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_r_v1h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_r_v1h1_coef);

	/* set Green's v0h0 v0h1 v1h0 v1h1 coefficients */
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_g_v0h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_g_v0h1_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_g_v1h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_g_v1h1_coef);

	/* set Blue's v0h0 v0h1 v1h0 v1h1 coefficients */
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_b_v0h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_b_v0h1_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_b_v1h0_coef);
	offset += 0xC;
	spr_coef_offset_config(spr_base + SPR_COEFF_OFFSET, offset, spr->spr_b_v1h1_coef);

}

static void spr_init(char __iomem *spr_base, struct spr_dsc_panel_para *spr)
{
	outp32(spr_base + SPR_PIX_EVEN_COEF_SEL, spr->spr_coeffsel_even);
	outp32(spr_base + SPR_PIX_ODD_COEF_SEL, spr->spr_coeffsel_odd);
	outp32(spr_base + SPR_PIX_PANEL_ARRANGE_SEL, spr->pix_panel_arrange_sel);

	spr_init_coeff(spr_base, spr);

	/* set RGB's border detect gain parameters */
	outp32(spr_base + SPR_BORDERLR_REG, spr->spr_borderlr_detect_r);
	outp32(spr_base + SPR_BORDERLR_REG + 0x4, spr->spr_bordertb_detect_r);
	outp32(spr_base + SPR_BORDERLR_REG + 0x8, spr->spr_borderlr_detect_g);
	outp32(spr_base + SPR_BORDERLR_REG + 0xC, spr->spr_bordertb_detect_g);
	outp32(spr_base + SPR_BORDERLR_REG + 0x10, spr->spr_borderlr_detect_b);
	outp32(spr_base + SPR_BORDERLR_REG + 0x14, spr->spr_bordertb_detect_b);

	/* set RGB's final gain parameters */
	outp32(spr_base + SPR_PIXGAIN_REG, (spr->spr_pixgain >> 32) & 0xFFFFFFFF);
	outp32(spr_base + SPR_PIXGAIN_REG1, spr->spr_pixgain & 0xFFFFFFFF);

	/* set boarder region */
	outp32(spr_base + SPR_BORDER_POSITION0, spr->spr_borderl_position);
	outp32(spr_base + SPR_BORDER_POSITION0 + 0x4, spr->spr_borderr_position);
	outp32(spr_base + SPR_BORDER_POSITION0 + 0x8, spr->spr_bordert_position);
	outp32(spr_base + SPR_BORDER_POSITION0 + 0xC, spr->spr_borderb_position);

	/* set RGB's diff ,weight regs */
	outp32(spr_base + SPR_R_DIFF_REG, spr->spr_r_diff);
	outp32(spr_base + SPR_R_WEIGHT_REG0, (spr->spr_r_weight >> 32) & 0xFFFFFFFF);
	outp32(spr_base + SPR_R_WEIGHT_REG1, spr->spr_r_weight & 0xFFFFFFFF);
	outp32(spr_base + SPR_R_DIFF_REG + 0xC, spr->spr_g_diff);
	outp32(spr_base + SPR_R_WEIGHT_REG0 + 0xC, (spr->spr_g_weight >> 32) & 0xFFFFFFFF);
	outp32(spr_base + SPR_R_WEIGHT_REG1 + 0xC, spr->spr_g_weight & 0xFFFFFFFF);
	outp32(spr_base + SPR_R_DIFF_REG + 0xC + 0xC, spr->spr_b_diff);
	outp32(spr_base + SPR_R_WEIGHT_REG0 + 0xC + 0xC, (spr->spr_b_weight >> 32) & 0xFFFFFFFF);
	outp32(spr_base + SPR_R_WEIGHT_REG1 + 0xC + 0xC, spr->spr_b_weight & 0xFFFFFFFF);

	outp32(spr_base + SPR_CSC_COEFF0, spr->spr_rgbg2uyvy_coeff[0]);
	outp32(spr_base + SPR_CSC_COEFF1, spr->spr_rgbg2uyvy_coeff[1]);
	outp32(spr_base + SPR_CSC_OFFSET0, spr->spr_rgbg2uyvy_coeff[2]);
	outp32(spr_base + SPR_CSC_OFFSET1, spr->spr_rgbg2uyvy_coeff[3]);
	outp32(spr_base + SPR_RESO, spr->input_reso);
}

static void spr_lut_config(char __iomem *spr_base, struct spr_dsc_panel_para *pinfo)
{
	int idx;
	int lut_idx;
	uint32_t low_part_index;
	uint32_t hight_part_index;
	uint32_t lut;

	if ((!pinfo) || (!pinfo->spr_lut_table))
		return;

	for (idx = 0; idx < SPR_LUT_LEN; idx += 2) {
		for (lut_idx = 0; lut_idx < SPR_LUT_INDEX; lut_idx++) {
			low_part_index = idx + SPR_LUT_LEN * lut_idx;

			if (idx + 1 == SPR_LUT_LEN)
				hight_part_index = low_part_index;
			else
				hight_part_index = idx + 1 + SPR_LUT_LEN * lut_idx;

			if ((low_part_index > (SPR_LUT_LEN * SPR_LUT_INDEX - 1)) ||
				(hight_part_index > (SPR_LUT_LEN * SPR_LUT_INDEX - 1))) {
				HISI_FB_ERR("LUT CONFIG ERROR low_part_index = %d, hight_part_index = %d\n",
					low_part_index, hight_part_index);
				return;
			}

			lut = (pinfo->spr_lut_table[hight_part_index] << 16) | pinfo->spr_lut_table[low_part_index];

			outp32(spr_base + spr_lut_addr[lut_idx] + 0x4 * (idx / 2), lut);
		}
	}
}

uint32_t get_hsize_after_spr_dsc(struct hisi_fb_data_type *hisifd, uint32_t rect_width)
{
	struct hisi_panel_info *pinfo = NULL;
	uint32_t hsize = rect_width;
	uint32_t bits_per_pixel;
	uint32_t bits_per_component;

	if (!hisifd) {
		HISI_FB_ERR("null hisifd\n");
		return hsize;
	}

	pinfo = &(hisifd->panel_info);

	if (pinfo->spr_dsc_mode == SPR_DSC_MODE_NONE)
		return hsize;

	if (pinfo->vesa_dsc.bits_per_pixel == DSC_0BPP) {
		/* DSC new mode */
		bits_per_pixel = pinfo->panel_dsc_info.dsc_info.dsc_bpp;
		bits_per_component = pinfo->panel_dsc_info.dsc_info.dsc_bpc;
	} else {
		bits_per_pixel = pinfo->vesa_dsc.bits_per_pixel;
		bits_per_component = pinfo->vesa_dsc.bits_per_component;
	}

	if (bits_per_component == 0) {
		HISI_FB_ERR("bpp value is error\n");
		return hsize;
	}

	/*
	 * bits_per_component * 3: 3 means GPR888 have 3 component
	 * pinfo->vesa_dsc.bits_per_pixel / 2: 2 means YUV422 BPP nead plus 2 config.
	 */
	if (pinfo->spr_dsc_mode == SPR_DSC_MODE_SPR_AND_DSC)
		hsize = (rect_width * (bits_per_pixel / 2)) / (bits_per_component * 3);
	else if (pinfo->spr_dsc_mode == SPR_DSC_MODE_SPR_ONLY)
		hsize = rect_width * 2 / 3;
	else if (pinfo->spr_dsc_mode == SPR_DSC_MODE_DSC_ONLY)
		hsize = (rect_width * bits_per_pixel) / (bits_per_component * 3);

	return hsize;
}

void spr_dsc_init(struct hisi_fb_data_type *hisifd, bool fastboot_enable)
{
	char __iomem *spr_base = NULL;
	struct hisi_panel_info *pinfo = NULL;
	struct spr_dsc_panel_para *spr = NULL;
	uint32_t spr_ctrl_value;

	if (!hisifd)
		return;

	pinfo = &(hisifd->panel_info);
	spr_base = hisifd->dss_base + SPR_OFFSET;
	if (!pinfo || (pinfo->spr_dsc_mode == SPR_DSC_MODE_NONE)) {
		if (pinfo && (pinfo->product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE))
			outp32(spr_base + SPR_CTRL, 0);
		return;
	}

	if (fastboot_enable)
		return;

	HISI_FB_INFO("+\n");
	spr = &(pinfo->spr);

	/* init spr ctrl reg */
	spr_ctrl_value = (spr->spr_rgbg2uyvy_8biten << 17) |
		(spr->spr_hpartial_mode << 15) |
		(spr->spr_partial_mode << 13) |
		(spr->spr_rgbg2uyvy_en << 11) |
		(spr->spr_horzborderdect << 10) |
		(spr->spr_linebuf_1dmode << 9) |
		(spr->spr_bordertb_dummymode << 8) |
		(spr->spr_borderlr_dummymode << 6) |
		(spr->spr_pattern_mode << 4) |
		(spr->spr_pattern_en << 3) |
		(spr->spr_subpxl_layout << 2) |
		(spr->ck_gt_spr_en << 1) |
		spr->spr_en;

	HISI_FB_INFO("spr_ctrl_value = 0x%x\n", spr_ctrl_value);
	outp32(spr_base + SPR_CTRL, spr_ctrl_value);

	if (spr->spr_pattern_en) {
		/* spr pattern generate config */
		outp32(spr_base + SPR_PATTERNGEN_POSITION, (pinfo->xres << 12) | 0x0);
		outp32(spr_base + SPR_PATTERNGEN_POSITION1, (pinfo->yres << 12) | 0x0);
		outp32(spr_base + SPR_PATTERNGEN_PIX0, (0x3fc << 20) | (0x3fc << 10) | 0x3fc);
		outp32(spr_base + SPR_PATTERNGEN_PIX1, (0x200 << 20) | (0x200 << 10) | 0x200);
	}

	spr_init(spr_base, spr);
	spr_lut_config(spr_base, spr);
	HISI_FB_INFO("-\n");
}

void spr_get_real_dirty_region(struct hisi_fb_data_type *hisifd, struct dss_rect *dirty,
	dss_overlay_t *pov_req)
{
	if (hisifd->panel_info.spr_dsc_mode == SPR_DSC_MODE_NONE)
		return;

	switch (pov_req->spr_overlap_type) {
	case SPR_OVERLAP_TOP:
		dirty->y += 1;
		dirty->h -= 1;
		break;
	case SPR_OVERLAP_BOTTOM:
		dirty->h -= 1;
		break;
	case SPR_OVERLAP_TOP_BOTTOM:
		dirty->y += 1;
		dirty->h -= 2;
		break;
	default:
		return;
	}
}

void spr_dsc_partial_updt_config(struct hisi_fb_data_type *hisifd, struct dss_rect dirty,
	dss_overlay_t *pov_req)
{
	uint32_t overlap_value;

	if (hisifd->panel_info.spr_dsc_mode == SPR_DSC_MODE_NONE)
		return;

	/* The follow code from chip protocol, It contains lots of fixed numbers */
	switch (pov_req->spr_overlap_type) {
	case SPR_OVERLAP_TOP:
		overlap_value = 1;
		break;
	case SPR_OVERLAP_BOTTOM:
		overlap_value = 2;
		break;
	case SPR_OVERLAP_TOP_BOTTOM:
		overlap_value = 3;
		break;
	default:
		overlap_value = 0;
		break;
	}

	HISI_FB_ERR("spr_overlap_type = %d\n", pov_req->spr_overlap_type);

	set_reg(hisifd->dss_base + SPR_OFFSET + SPR_CTRL, overlap_value, 2, 15);
	outp32(hisifd->dss_base + SPR_OFFSET + SPR_RESO, ((dirty.h - 1) << 16) | (dirty.w - 1));
}

