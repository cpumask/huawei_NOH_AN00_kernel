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

struct hisi_dss_dirty_region {
	uint32_t h_porch_pading;
	uint32_t v_porch_pading;
	uint32_t max_latency;
	uint32_t bits_per_pixel;
	uint32_t h_front_porch_max;
	uint32_t h_front_porch;
	uint32_t h_back_porch;
};

void hisi_dss_dirty_region_dbuf_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *dss_base, dirty_region_updt_t *s_dirty_region_updt)
{
	hisi_check_and_no_retval(!hisifd, DEBUG, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!dss_base, DEBUG, "dss_base is NULL!\n");
	hisi_check_and_no_retval(!s_dirty_region_updt, DEBUG, "s_dirty_region_updt is NULL!\n");

	hisifd->set_reg(hisifd, dss_base + DSS_DBUF0_OFFSET + DBUF_FRM_SIZE,
		s_dirty_region_updt->dbuf_frm_size, 32, 0);
	hisifd->set_reg(hisifd, dss_base + DSS_DBUF0_OFFSET + DBUF_FRM_HSIZE,
		s_dirty_region_updt->dbuf_frm_hsize, 32, 0);

#ifdef SUPPORT_DISP_CHANNEL
	hisifd->set_reg(hisifd, dss_base + DSS_DISP_CH0_OFFSET + IMG_SIZE_BEF_SR,
		s_dirty_region_updt->dpp_img_size_bef_sr, 32, 0);
	hisifd->set_reg(hisifd, dss_base + DSS_DISP_CH0_OFFSET + IMG_SIZE_AFT_SR,
		s_dirty_region_updt->dpp_img_size_aft_sr, 32, 0);
	hisifd->set_reg(hisifd, dss_base + DSS_DISP_CH0_OFFSET + IMG_SIZE_AFT_IFBCSW,
		s_dirty_region_updt->dpp_img_size_aft_ifbcsw, 32, 0);
#else
	hisifd->set_reg(hisifd, dss_base + DSS_DPP_OFFSET + DPP_IMG_SIZE_BEF_SR,
		s_dirty_region_updt->dpp_img_size_bef_sr, 32, 0);
	hisifd->set_reg(hisifd, dss_base + DSS_DPP_OFFSET + DPP_IMG_SIZE_AFT_SR,
		s_dirty_region_updt->dpp_img_size_aft_sr, 32, 0);
#endif

#if defined(CONFIG_HISI_FB_V600)
	hisifd->set_reg(hisifd, dss_base + DSS_HI_ACE_OFFSET + DPE_IMAGE_INFO,
		s_dirty_region_updt->hiace_img_size, 32, 0);

	if (hisifd->panel_info.ifbc_type != IFBC_TYPE_NONE)
		hisifd->set_reg(hisifd, dss_base + DSS_DSC_OFFSET + DSC_PIC_SIZE,
			s_dirty_region_updt->ifbc_size, 32, 0);
#endif
}

static bool is_dirty_region_empty(dss_overlay_t *pov_req)
{
	return ((!pov_req) || (pov_req->dirty_rect.x == 0 && pov_req->dirty_rect.y == 0 &&
		pov_req->dirty_rect.w == 0 && pov_req->dirty_rect.h == 0));
}

static void get_hisi_dss_dirty_rect(struct hisi_fb_data_type *hisifd,
	struct dss_rect *dirty,
	dirty_region_updt_t *dirty_region_updt,
	dss_overlay_t *pov_req)
{
	struct hisi_panel_info *pinfo = NULL;

	pinfo = &(hisifd->panel_info);

	if (is_dirty_region_empty(pov_req)) {
		dirty->x = 0;
		dirty->y = 0;
		dirty->w = hisifd->panel_info.xres;
		dirty->h = hisifd->panel_info.yres;
	} else {
		dirty->x = pov_req->dirty_rect.x;
		dirty->y = pov_req->dirty_rect.y;
		dirty->w = pov_req->dirty_rect.w;
		dirty->h = pov_req->dirty_rect.h;

		/* rog scale, just consider (x,y) is (0,0) now */
		if (pinfo->cascadeic_support && (pov_req->rog_width > 0 && pov_req->rog_height > 0)) {
			dirty->w = dirty->w * hisifd->panel_info.xres / pov_req->rog_width;
			dirty->h = dirty->h * hisifd->panel_info.yres / pov_req->rog_height;
			if ((uint32_t)dirty->w > hisifd->panel_info.xres)
				dirty->w = hisifd->panel_info.xres;
			if ((uint32_t)dirty->h > hisifd->panel_info.yres)
				dirty->h = hisifd->panel_info.yres;
		}
	}

	if (pinfo->cascadeic_support && pov_req) {
		HISI_FB_DEBUG("dirty_rect: %d,%d / dirty: %d,%d\n",
			pov_req->dirty_rect.w, pov_req->dirty_rect.h, dirty->w, dirty->h);
		if (!dirty_region_updt)
			HISI_FB_DEBUG("dirty_region_updt: %d,%d\n", hisifd->dirty_region_updt.w,
				hisifd->dirty_region_updt.h);
	}
}

static bool is_dss_dirty_region(struct hisi_fb_data_type *hisifd, struct dss_rect *dirty)
{
	if ((dirty->x == hisifd->dirty_region_updt.x) && (dirty->y == hisifd->dirty_region_updt.y) &&
		(dirty->w == hisifd->dirty_region_updt.w) && (dirty->h == hisifd->dirty_region_updt.h))
		return false;

	return true;
}

static void hisi_dss_set_ifbc_size(dirty_region_updt_t *dirty_region_updt,
	struct hisi_panel_info *pinfo, struct dss_rect *dirty)
{
#ifdef SUPPORT_DSC_PIC_WIDTH
	if (pinfo->ifbc_type != IFBC_TYPE_NONE)  /* set ifbc size */
		dirty_region_updt->ifbc_size = set_bits32(dirty_region_updt->ifbc_size,
			((DSS_WIDTH((uint32_t)dirty->w) << 16) | DSS_HEIGHT((uint32_t)dirty->h)), 32, 0);
#else
	/* set ifbc size */
	if (pinfo->ifbc_type != IFBC_TYPE_NONE) {
		if ((pinfo->ifbc_type == IFBC_TYPE_VESA2X_DUAL) || (pinfo->ifbc_type == IFBC_TYPE_VESA3X_DUAL))
			dirty_region_updt->ifbc_size = set_bits32(dirty_region_updt->ifbc_size,
				((DSS_WIDTH((uint32_t)dirty->w / 2) << 16) | DSS_HEIGHT((uint32_t)dirty->h)), 32, 0);
		else
			dirty_region_updt->ifbc_size = set_bits32(dirty_region_updt->ifbc_size,
				((DSS_WIDTH((uint32_t)dirty->w) << 16) | DSS_HEIGHT((uint32_t)dirty->h)), 32, 0);
	}
#endif
}

#ifdef SUPPORT_DSC_PIC_WIDTH
int hisi_dss_dirty_region_dbuf_config(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req)
{
	struct hisi_panel_info *pinfo = NULL;
	dirty_region_updt_t *dirty_region_updt = NULL;
	struct dss_rect dirty = {0};
	dss_rect_t rect = {0};

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");
	pinfo = &(hisifd->panel_info);

	if ((hisifd->index != PRIMARY_PANEL_IDX) || !pinfo->dirty_region_updt_support)
		return 0;

	get_hisi_dss_dirty_rect(hisifd, &dirty, NULL, pov_req);
	if (!is_dss_dirty_region(hisifd, &dirty))
		return 0;

	dirty_region_updt = &(hisifd->dss_module.dirty_region_updt);
	hisifd->dss_module.dirty_region_updt_used = 1;

	/* horizontal dirty update must fix arsr post bug */
	dirty_region_updt->dpp_img_size_bef_sr = set_bits32(dirty_region_updt->dpp_img_size_bef_sr,
		(DSS_WIDTH((uint32_t)dirty.h) << 16) | DSS_WIDTH((uint32_t)dirty.w), 32, 0);
	dirty_region_updt->dpp_img_size_aft_sr = set_bits32(dirty_region_updt->dpp_img_size_aft_sr,
		(DSS_WIDTH((uint32_t)dirty.h) << 16) | DSS_WIDTH((uint32_t)dirty.w), 32, 0);
	dirty_region_updt->dpp_img_size_aft_ifbcsw = set_bits32(dirty_region_updt->dpp_img_size_aft_ifbcsw,
		(DSS_WIDTH((uint32_t)dirty.h) << 16) | DSS_WIDTH((uint32_t)dirty.w), 32, 0);

	rect.x = 0;
	rect.y = 0;
	rect.w = dirty.w;
	rect.h = dirty.h;
	mipi_ifbc_get_rect(hisifd, &rect);

#if defined(CONFIG_HISI_FB_V600)
	hisi_dss_set_ifbc_size(dirty_region_updt, pinfo, &dirty);
	dirty_region_updt->hiace_img_size = ((uint32_t)dirty.h << 16) | (uint32_t)dirty.w;
#endif
	dirty_region_updt->dbuf_frm_size = set_bits32(dirty_region_updt->dbuf_frm_size, rect.w * dirty.h, 27, 0);
	dirty_region_updt->dbuf_frm_hsize = set_bits32(dirty_region_updt->dbuf_frm_hsize, DSS_WIDTH(rect.w), 13, 0);

	return 0;
}
#else
int hisi_dss_dirty_region_dbuf_config(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req)
{
	struct hisi_panel_info *pinfo = NULL;
	dirty_region_updt_t *dirty_region_updt = NULL;
	struct dss_rect dirty = {0};

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");
	pinfo = &(hisifd->panel_info);

	if ((hisifd->index != PRIMARY_PANEL_IDX) || !pinfo->dirty_region_updt_support)
		return 0;

	get_hisi_dss_dirty_rect(hisifd, &dirty, NULL, pov_req);
	if (!is_dss_dirty_region(hisifd, &dirty))
		return 0;

	dirty_region_updt = &(hisifd->dss_module.dirty_region_updt);
	hisifd->dss_module.dirty_region_updt_used = 1;

	dirty_region_updt->dpp_img_size_bef_sr = set_bits32(dirty_region_updt->dpp_img_size_bef_sr,
		(DSS_WIDTH((uint32_t)dirty.h) << 16) | DSS_WIDTH((uint32_t)dirty.w), 32, 0);
	dirty_region_updt->dpp_img_size_aft_sr = set_bits32(dirty_region_updt->dpp_img_size_aft_sr,
		(DSS_WIDTH((uint32_t)dirty.h) << 16) | DSS_WIDTH((uint32_t)dirty.w), 32, 0);

	dirty_region_updt->dbuf_frm_size = set_bits32(dirty_region_updt->dbuf_frm_size, dirty.w * dirty.h, 27, 0);
	dirty_region_updt->dbuf_frm_hsize = set_bits32(dirty_region_updt->dbuf_frm_hsize, DSS_WIDTH(dirty.w), 13, 0);

	return 0;
}
#endif

/*******************************************************************************
 * dirty_region_updt
 */
#ifdef SUPPORT_DSI_VER_2_0
static void hisi_dss_dirty_region_updt_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *dss_base, dirty_region_updt_t *s_dirty_region_updt)
{
	uint32_t ldi_vrt_ctrl0;
	uint32_t vsync_delay_cnt;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!dss_base, ERR, "dss_base is NULL!\n");
	hisi_check_and_no_retval(!s_dirty_region_updt, ERR, "s_dirty_region_updt is NULL!\n");

	if (s_dirty_region_updt->ldi_vrt_ctrl0 > V_FRONT_PORCH_MAX) {
		ldi_vrt_ctrl0 = V_FRONT_PORCH_MAX;
		if (is_lcd_dfr_support(hisifd))
			vsync_delay_cnt = (s_dirty_region_updt->ldi_vrt_ctrl0 - V_FRONT_PORCH_MAX) *
				hisifd->panel_info.frm_rate_ctrl.current_hline_time;
		else
			vsync_delay_cnt = (s_dirty_region_updt->ldi_vrt_ctrl0 - V_FRONT_PORCH_MAX) *
				hisifd->panel_info.mipi.hline_time;
	} else {
		ldi_vrt_ctrl0 = s_dirty_region_updt->ldi_vrt_ctrl0;
		vsync_delay_cnt = VSYNC_DELAY_TIME_DEFAULT;
	}

#ifndef CONFIG_HISI_FB_V600
	set_reg(dss_base + DSS_HI_ACE_OFFSET + DPE_IMAGE_INFO, s_dirty_region_updt->hiace_img_size, 32, 0);
#endif

	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPI_LDI_DPI0_HRZ_CTRL2,
		s_dirty_region_updt->ldi_dpi0_hrz_ctrl2, 13, 0);
	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPI_LDI_VRT_CTRL2, s_dirty_region_updt->ldi_vrt_ctrl2, 13, 0);

	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPIDSI_VID_VFP_LINES_OFFSET, ldi_vrt_ctrl0, 10, 0);
	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPI_VSYNC_DELAY_TIME, vsync_delay_cnt, 32, 0);

	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPIDSI_VID_VACTIVE_LINES_OFFSET,
		s_dirty_region_updt->ldi_vrt_ctrl2 + 1, 14, 0);
	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPIDSI_EDPI_CMD_SIZE_OFFSET,
		s_dirty_region_updt->edpi_cmd_size, 32, 0);

	if (is_dual_mipi_panel(hisifd)) {
		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPI_LDI_DPI0_HRZ_CTRL2,
			s_dirty_region_updt->ldi_dpi0_hrz_ctrl2, 13, 0);
		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPI_LDI_VRT_CTRL2,
			s_dirty_region_updt->ldi_vrt_ctrl2, 13, 0);

		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPIDSI_VID_VFP_LINES_OFFSET, ldi_vrt_ctrl0, 10, 0);
		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPI_VSYNC_DELAY_TIME, vsync_delay_cnt, 32, 0);

		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPIDSI_VID_VACTIVE_LINES_OFFSET,
			s_dirty_region_updt->ldi_vrt_ctrl2 + 1, 14, 0);
		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPIDSI_EDPI_CMD_SIZE_OFFSET,
			s_dirty_region_updt->edpi_cmd_size, 32, 0);
	}

#ifndef CONFIG_HISI_FB_V600
	if (hisifd->panel_info.ifbc_type != IFBC_TYPE_NONE)
		set_reg(dss_base + DSS_DSC_OFFSET + DSC_PIC_SIZE, s_dirty_region_updt->ifbc_size, 32, 0);
#endif
}

#else
static void hisi_dss_dirty_region_updt_set_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *dss_base, dirty_region_updt_t *s_dirty_region_updt)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!dss_base, ERR, "dss_base is NULL!\n");
	hisi_check_and_no_retval(!s_dirty_region_updt, ERR, "s_dirty_region_updt is NULL!\n");

	set_reg(dss_base + DSS_MIPI_DSI0_OFFSET + MIPIDSI_EDPI_CMD_SIZE_OFFSET,
		s_dirty_region_updt->edpi_cmd_size, 32, 0);
	if (is_dual_mipi_panel(hisifd)) {
		set_reg(dss_base + DSS_MIPI_DSI1_OFFSET + MIPIDSI_EDPI_CMD_SIZE_OFFSET,
			s_dirty_region_updt->edpi_cmd_size, 32, 0);
	}

	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI0_HRZ_CTRL0,
		s_dirty_region_updt->ldi_dpi0_hrz_ctrl0, 29, 0);
	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI0_HRZ_CTRL1,
		s_dirty_region_updt->ldi_dpi0_hrz_ctrl1, 13, 0);
	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI0_HRZ_CTRL2,
		s_dirty_region_updt->ldi_dpi0_hrz_ctrl2, 13, 0);
	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_VRT_CTRL0,
		s_dirty_region_updt->ldi_vrt_ctrl0, 29, 0);
	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_VRT_CTRL1,
		s_dirty_region_updt->ldi_vrt_ctrl1, 13, 0);
	set_reg(dss_base + DSS_LDI0_OFFSET + LDI_VRT_CTRL2,
		s_dirty_region_updt->ldi_vrt_ctrl2, 13, 0);

	if (is_dual_mipi_panel(hisifd)) {
		set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI1_HRZ_CTRL0,
			s_dirty_region_updt->ldi_dpi1_hrz_ctrl0, 29, 0);
		set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI1_HRZ_CTRL1,
			s_dirty_region_updt->ldi_dpi1_hrz_ctrl1, 13, 0);
		set_reg(dss_base + DSS_LDI0_OFFSET + LDI_DPI1_HRZ_CTRL2,
			s_dirty_region_updt->ldi_dpi1_hrz_ctrl2, 13, 0);
	}

	if (hisifd->panel_info.ifbc_type != IFBC_TYPE_NONE) {
		if (!is_ifbc_vesa_panel(hisifd))
			set_reg(dss_base + DSS_IFBC_OFFSET + IFBC_SIZE, s_dirty_region_updt->ifbc_size, 32, 0);
		else
			set_reg(dss_base + DSS_DSC_OFFSET + DSC_PIC_SIZE, s_dirty_region_updt->ifbc_size, 32, 0);
	}

	set_reg(dss_base + DSS_HI_ACE_OFFSET + DPE_IMAGE_INFO, s_dirty_region_updt->hiace_img_size, 32, 0);
}
#endif

static void get_hisi_dss_dirty_region(struct hisi_fb_data_type *hisifd, struct dss_rect *dirty,
	dss_rect_t *rect, struct hisi_dss_dirty_region *dirty_region)
{
	struct hisi_panel_info *pinfo = NULL;

	pinfo = &hisifd->panel_info;
	if ((hisifd->panel_info.xres) >= (uint32_t)dirty->w)
		dirty_region->h_porch_pading = hisifd->panel_info.xres - dirty->w;

	if (hisifd->panel_info.yres >= (uint32_t)dirty->h)
		dirty_region->v_porch_pading = hisifd->panel_info.yres - dirty->h;

	rect->x = 0;
	rect->y = 0;
	rect->w = dirty->w;
	rect->h = dirty->h;
	mipi_ifbc_get_rect(hisifd, rect);

	dirty_region->h_front_porch = pinfo->ldi.h_front_porch;
	dirty_region->h_back_porch = pinfo->ldi.h_back_porch;

	dirty_region->h_porch_pading = dirty_region->h_porch_pading * rect->w / dirty->w;
	dirty_region->v_porch_pading = dirty_region->v_porch_pading * rect->h / dirty->h;

	/* max_latency = data_lp2hs_time + (EDPI_CMD_SIZE * bytes_per_pixel + 1 + 6) / number_of_lanes
	 * h_front_porch <= max_latency * byte_clk_cycle / pixel_clk_cycle
	 */
	if (pinfo->bpp == LCD_RGB888)
		dirty_region->bits_per_pixel = 24;  /* bpp:LCD_RGB888(8+8+8=24) */
	else if (pinfo->bpp == LCD_RGB565)
		dirty_region->bits_per_pixel = 16;  /* bpp:LCD_RGB565(5+6+5=16) */
	else
		dirty_region->bits_per_pixel = 24;

	if (pinfo->pxl_clk_rate_div == 0)
		pinfo->pxl_clk_rate_div = 1;

	dirty_region->max_latency = (rect->w * dirty_region->bits_per_pixel / 8 + 1 + 6) / (pinfo->mipi.lane_nums + 1);
	if (pinfo->mipi.phy_mode == DPHY_MODE) {
		if (pinfo->dsi_phy_ctrl.lane_byte_clk != 0)
			dirty_region->h_front_porch_max = dirty_region->max_latency *
				(pinfo->pxl_clk_rate / pinfo->pxl_clk_rate_div) / pinfo->dsi_phy_ctrl.lane_byte_clk;
	} else if (pinfo->mipi.phy_mode == CPHY_MODE) {
		if (pinfo->dsi_phy_ctrl.lane_word_clk != 0)
			dirty_region->h_front_porch_max = dirty_region->max_latency *
				(pinfo->pxl_clk_rate / pinfo->pxl_clk_rate_div) / pinfo->dsi_phy_ctrl.lane_word_clk;
	}

	HISI_FB_DEBUG("bits_per_pixel = %d\n data_lane_lp2hs_time = %d\n max_latency = %d\n"
		"pxl_clk_rate = %lld\n pxl_clk_rate_div = %d\n dsi_phy_ctrl.lane_byte_clk = %lld\n"
		"h_front_porch_max = %d\n", dirty_region->bits_per_pixel, pinfo->dsi_phy_ctrl.data_lane_lp2hs_time,
		dirty_region->max_latency, pinfo->pxl_clk_rate, pinfo->pxl_clk_rate_div,
		pinfo->dsi_phy_ctrl.lane_byte_clk, dirty_region->h_front_porch_max);

	if (dirty_region->h_front_porch > dirty_region->h_front_porch_max) {
		dirty_region->h_back_porch += (dirty_region->h_front_porch - dirty_region->h_front_porch_max);
		dirty_region->h_front_porch = dirty_region->h_front_porch_max;
	}
}

static void hisi_dss_set_hrz_ctr(struct hisi_fb_data_type *hisifd,
	dirty_region_updt_t *dirty_region_updt, struct hisi_panel_info *pinfo,
	dss_rect_t *rect, struct hisi_dss_dirty_region *dirty_region)
{
	dirty_region_updt->ldi_dpi0_hrz_ctrl0 = set_bits32(dirty_region_updt->ldi_dpi0_hrz_ctrl0,
		(dirty_region->h_front_porch) |
		((dirty_region->h_back_porch + dirty_region->h_porch_pading) << 16), 29, 0);
	dirty_region_updt->ldi_dpi0_hrz_ctrl1 = set_bits32(dirty_region_updt->ldi_dpi0_hrz_ctrl1,
		DSS_WIDTH(pinfo->ldi.h_pulse_width), 13, 0);
	dirty_region_updt->ldi_dpi0_hrz_ctrl2 = set_bits32(dirty_region_updt->ldi_dpi0_hrz_ctrl2,
		DSS_WIDTH(rect->w), 13, 0);

	if (is_dual_mipi_panel(hisifd)) {
		dirty_region_updt->ldi_dpi1_hrz_ctrl0 = set_bits32(dirty_region_updt->ldi_dpi1_hrz_ctrl0,
			(dirty_region->h_back_porch + dirty_region->h_porch_pading) << 16, 29, 0);
		dirty_region_updt->ldi_dpi1_hrz_ctrl1 = set_bits32(dirty_region_updt->ldi_dpi1_hrz_ctrl1,
			DSS_WIDTH(pinfo->ldi.h_pulse_width), 13, 0);
		dirty_region_updt->ldi_dpi1_hrz_ctrl2 = set_bits32(dirty_region_updt->ldi_dpi1_hrz_ctrl2,
			DSS_WIDTH(rect->w), 13, 0);
	}
}

static void hisi_dss_set_vrt_ctr(struct hisi_fb_data_type *hisifd,
	dirty_region_updt_t *dirty_region_updt, struct hisi_panel_info *pinfo,
	dss_rect_t *rect, struct hisi_dss_dirty_region *dirty_region)
{
#ifdef SUPPORT_DSI_VER_2_0
	if (is_lcd_dfr_support(hisifd)) {
		dirty_region_updt->ldi_vrt_ctrl0 = set_bits32(dirty_region_updt->ldi_vrt_ctrl0,
			(hisifd->panel_info.frm_rate_ctrl.current_vfp + dirty_region->v_porch_pading), 32, 0);
		if ((dirty_region->v_porch_pading > 0) && (hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_NEED_DOING))
			HISI_FB_ERR("dirty region update conflict with dfr\n");
	} else {
		dirty_region_updt->ldi_vrt_ctrl0 = set_bits32(dirty_region_updt->ldi_vrt_ctrl0,
			(pinfo->mipi.vfp + dirty_region->v_porch_pading), 32, 0);
	}
#else
	dirty_region_updt->ldi_vrt_ctrl0 = set_bits32(dirty_region_updt->ldi_vrt_ctrl0,
		(pinfo->ldi.v_front_porch + dirty_region->v_porch_pading) | ((pinfo->ldi.v_back_porch) << 16), 29, 0);
#endif

	dirty_region_updt->ldi_vrt_ctrl1 = set_bits32(dirty_region_updt->ldi_vrt_ctrl1,
		DSS_HEIGHT(pinfo->ldi.v_pulse_width), 13, 0);
	dirty_region_updt->ldi_vrt_ctrl2 = set_bits32(dirty_region_updt->ldi_vrt_ctrl2,
		DSS_HEIGHT(rect->h), 13, 0);
}

static void hisi_dss_set_display_dirty_region(struct hisi_fb_data_type *hisifd,
	struct dss_rect *dirty_updt, struct dss_rect *dirty, dss_rect_t rect)
{
	struct hisi_fb_panel_data *pdata = dev_get_platdata(&hisifd->pdev->dev);
	struct hisi_panel_info *pinfo = &(hisifd->panel_info);

	hisi_check_and_no_retval((!pdata || !pdata->set_display_region), INFO,
		"pdata or set_display_region is NULL\n");

	if (is_dual_mipi_panel(hisifd)) {
		dirty_updt->x /= 2;
		dirty_updt->w = rect.w + pinfo->ldi.dpi0_overlap_size;
		pdata->set_display_region(hisifd->pdev, dirty_updt);

	} else {
		pdata->set_display_region(hisifd->pdev, dirty);
	}
}

void hisi_dss_dirty_region_updt_config(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req)
{
	struct hisi_panel_info *pinfo = NULL;
	dirty_region_updt_t *dirty_region_updt = NULL;
	struct dss_rect dirty = {0};
	struct dss_rect dirty_updt = {0};
	dss_rect_t rect = {0};
	struct hisi_dss_dirty_region dirty_region = {0};

	hisi_check_and_no_retval((hisifd == NULL), ERR, "hisifd is NULL\n");

	pinfo = &(hisifd->panel_info);

	if ((hisifd->index != PRIMARY_PANEL_IDX) || !pinfo->dirty_region_updt_support)
		return;

	get_hisi_dss_dirty_rect(hisifd, &dirty, dirty_region_updt, pov_req);
	if (!is_dss_dirty_region(hisifd, &dirty))
		return;

	dirty_updt = dirty;
	get_hisi_dss_dirty_region(hisifd, &dirty, &rect, &dirty_region);

	dirty_region_updt = &(hisifd->dss_module.dirty_region_updt);

	hisi_dss_set_hrz_ctr(hisifd, dirty_region_updt, pinfo, &rect, &dirty_region);
	hisi_dss_set_vrt_ctr(hisifd, dirty_region_updt, pinfo, &rect, &dirty_region);

	/* set dsi size */
	if (is_dual_mipi_panel(hisifd))
		dirty_region_updt->edpi_cmd_size = set_bits32(dirty_region_updt->edpi_cmd_size,
			rect.w + pinfo->ldi.dpi0_overlap_size, 16, 0);
	else
		dirty_region_updt->edpi_cmd_size = set_bits32(dirty_region_updt->edpi_cmd_size, rect.w, 16, 0);

#ifndef CONFIG_HISI_FB_V600
	hisi_dss_set_ifbc_size(dirty_region_updt, pinfo, &dirty);

	dirty_region_updt->hiace_img_size = ((uint32_t)dirty.h << 16) | (uint32_t)dirty.w;
#endif

	hisi_dss_set_display_dirty_region(hisifd, &dirty_updt, &dirty, rect);
	HISI_FB_DEBUG("dirty_updt[%d,%d, %d,%d], h_porch_pading=%d, v_porch_pading=%d.\n",
		dirty_updt.x, dirty_updt.y, dirty_updt.w, dirty_updt.h,
		dirty_region.h_porch_pading, dirty_region.v_porch_pading);

	hisifd->dirty_region_updt = dirty;

	hisi_dss_dirty_region_updt_set_reg(hisifd, hisifd->dss_base, &(hisifd->dss_module.dirty_region_updt));


	HISI_FB_DEBUG("dirty[%d,%d, %d,%d], h_porch_pading=%d, v_porch_pading=%d.\n",
		dirty.x, dirty.y, dirty.w, dirty.h, dirty_region.h_porch_pading, dirty_region.v_porch_pading);

}
