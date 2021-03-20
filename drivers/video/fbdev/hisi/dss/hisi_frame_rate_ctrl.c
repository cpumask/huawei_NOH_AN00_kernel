/*
 * hisi_frame_rate_ctrl.c
 *
 * The driver of dynamic frame rate
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
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
#include "hisi_frame_rate_ctrl.h"
#include "hisi_mipi_dsi.h"
#ifdef CONFIG_HUAWEI_DUBAI
#include <huawei_platform/log/hwlog_kernel.h>
#endif

#define PORCH_RATIO_UPPER_LIMIT 90
#define PORCH_RATIO_LOWER_LIMIT 10
#define MAX_DELAY_COUNT 10000

/* notify hwc to change core clk */
static void hwc_uevent_frame_update_notify(struct hisi_fb_data_type *hisifd,
	int frm_rate, uint32_t porch_ratio, uint32_t notify_type)
{
	char buf[64] = {0};  /* dfr notify print info */
	char *envp[2] = {NULL};  /* environment variable */
	uint32_t value;

	value = (notify_type << 16) | (porch_ratio << 8) | (uint32_t)frm_rate;
	snprintf(buf, sizeof(buf) - 1, "dfrNotify=%u", value);

	envp[0] = buf;
	envp[1] = NULL;

	kobject_uevent_env(&(hisifd->fbi->dev->kobj), KOBJ_CHANGE, envp);

	HISI_FB_INFO("fb%d, %s, %d, %u, %u\n", hisifd->index, buf, frm_rate, porch_ratio, notify_type);
}

void hisi_dfr_notice_handle_func(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = NULL;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	hisifd = container_of(work, struct hisi_fb_data_type, dfr_notice_work);
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return;
	}

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	hwc_uevent_frame_update_notify(hisifd, frm_rate_ctrl->target_frame_rate,
		frm_rate_ctrl->porch_ratio, frm_rate_ctrl->notify_type);
}

static void set_disrupt_vsync_delay(struct hisi_fb_data_type *hisifd, uint32_t dis)
{
	if (is_mipi_video_panel(hisifd))
		set_reg(hisifd->mipi_dsi0_base + MIPI_VSYNC_DELAY_CTRL, dis, 1, 0);
}

static void check_disrupt_vsync_delay(struct hisi_fb_data_type *hisifd)
{
	uint32_t vstate = 0;

	if (is_mipi_video_panel(hisifd)) {
		vstate = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE);  /*lint !e529*/
		if (vstate == LDI_VSTATE_V_WAIT_GPU) {
			udelay(2);
			vstate = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE);  /*lint !e529*/
			if (vstate == LDI_VSTATE_V_WAIT_GPU)
				HISI_FB_ERR("disrupt vsync delay fail\n");
		}
	}
}

static void hisi_dfr_set_end_state(struct hisi_fb_data_type *hisifd)
{
	unsigned long flags = 0;

	struct frame_rate_ctrl *frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	spin_lock_irqsave(&hisifd->mipi_resource_lock, flags);
	hisifd->panel_info.fps = frm_rate_ctrl->target_frame_rate;
	frm_rate_ctrl->current_frame_rate = frm_rate_ctrl->target_frame_rate;
	frm_rate_ctrl->current_dsi_bit_clk = frm_rate_ctrl->target_dsi_bit_clk;
	frm_rate_ctrl->current_dsc_en = frm_rate_ctrl->target_dsc_en;
	frm_rate_ctrl->current_lane_byte_clk = (hisifd->panel_info.mipi.phy_mode == DPHY_MODE) ?
		frm_rate_ctrl->phy_ctrl.lane_byte_clk : frm_rate_ctrl->phy_ctrl.lane_word_clk;

	frm_rate_ctrl->status = FRM_UPDT_DONE;
	spin_unlock_irqrestore(&hisifd->mipi_resource_lock, flags);
}

static void hisi_dfr_delay_work(struct work_struct *work)
{
	uint32_t delay_range = 200;  /* us */
	struct hisi_fb_data_type *hisifd = NULL;
	uint32_t vstate = 0;
	uint32_t timeout = 0;
	uint32_t wait_vstate = 0;

	hisifd = container_of(work, struct hisi_fb_data_type, dfr_delay_work);
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return;
	}

	if (is_mipi_video_panel(hisifd)) {
		vstate = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE);  /*lint !e529*/
		wait_vstate = LDI_VSTATE_VFP | LDI_VSTATE_IDLE | LDI_VSTATE_VSW |
			LDI_VSTATE_VBP | LDI_VSTATE_V_WAIT_GPU;
		while ((vstate & wait_vstate) && ((timeout++) < MAX_DELAY_COUNT)) {
			udelay(2);  /* 2us */
			vstate = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE);  /*lint !e529*/
		}
	} else {
		usleep_range(hisifd->panel_info.dfr_delay_time, hisifd->panel_info.dfr_delay_time + delay_range);
	}

	hisi_dfr_set_end_state(hisifd);

	HISI_FB_INFO("timeout=%d, vstate=0x%x, delay %d us to release dfr status\n",
		timeout, vstate, hisifd->panel_info.dfr_delay_time);
}

static void hisi_dfr_end_process(struct hisi_fb_data_type *hisifd, bool dfr_succ)
{
	struct hisi_panel_info *pinfo = NULL;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	pinfo = &(hisifd->panel_info);
	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	if (dfr_succ) {
		if (is_mipi_video_panel(hisifd)) {
			queue_work(hisifd->dfr_delay_wq, &hisifd->dfr_delay_work);
		} else {
			if (frm_rate_ctrl->target_frame_rate == FPS_60HZ &&
				hisifd->panel_info.dfr_delay_time > 0) {
				queue_work(hisifd->dfr_delay_wq, &hisifd->dfr_delay_work);
			} else {
				hisi_dfr_set_end_state(hisifd);
			}
		}
	}
}

struct mipi_panel_info *get_mipi_ctrl(struct hisi_fb_data_type *hisifd)
{
	struct mipi_panel_info *mipi = NULL;

	if (!is_lcd_dfr_support(hisifd))
		return &(hisifd->panel_info.mipi);

	if (hisifd->panel_info.frm_rate_ctrl.target_frame_rate == FPS_60HZ)
		mipi = &(hisifd->panel_info.mipi);
	else
		mipi = &(hisifd->panel_info.mipi_updt);

	return mipi;
}

static uint32_t get_compression_ratio(struct hisi_fb_data_type *hisifd)
{
	uint32_t ifbc_type = hisifd->panel_info.ifbc_type;
	int mipi_idx = is_dual_mipi_panel(hisifd) ? 1 : 0;

	if (ifbc_type >= IFBC_TYPE_MAX) {
		HISI_FB_ERR("ifbc_type is invalid\n");
		return 0;
	}

	return g_mipi_ifbc_division[mipi_idx][ifbc_type].xres_div;
}

static uint32_t get_porch_ratio(struct hisi_fb_data_type *hisifd)
{
	uint32_t porch_ratio = 10;  /* default porch ratio 10% */
	uint32_t cmp_ratio;
	uint32_t active_dpi_hsize;
	uint32_t bpp = 24;  /* BIT PER PIEXL, RGB888 */
	uint32_t real_area;
	uint32_t total_area;
	struct mipi_panel_info *mipi = get_mipi_ctrl(hisifd);

	if (hisifd->panel_info.frm_rate_ctrl.ignore_hporch) {
		real_area = hisifd->panel_info.yres;
		total_area = hisifd->panel_info.yres + mipi->vsa + mipi->vbp + mipi->vfp;
		porch_ratio = (total_area - real_area) * 100 / total_area;
		HISI_FB_INFO("real_area=%u, total_area=%u, porch_ratio=%u", real_area, total_area, porch_ratio);
		return porch_ratio;
	}

	if (hisifd->panel_info.frm_rate_ctrl.target_frame_rate == FPS_60HZ &&
		(hisifd->panel_info.dfr_method == DFR_METHOD_HPORCH))
		return porch_ratio;

	cmp_ratio = get_compression_ratio(hisifd);
	if (cmp_ratio == 0)
		return porch_ratio;

	/* (xres/dsc*bpp/8+6)/lane_num, 8 - byte/bit, 6 - headerSize */
	active_dpi_hsize = (hisifd->panel_info.xres / cmp_ratio * bpp / 8 + 6) /
		(hisifd->panel_info.mipi.lane_nums + 1);

	real_area = active_dpi_hsize * hisifd->panel_info.yres;
	total_area = mipi->hline_time * (mipi->vsa + mipi->vbp + mipi->vfp + hisifd->panel_info.yres);
	porch_ratio = (total_area - real_area) * 100 / total_area;  /* % */

	if (porch_ratio < PORCH_RATIO_LOWER_LIMIT || porch_ratio > PORCH_RATIO_UPPER_LIMIT)
		HISI_FB_WARNING("porch_ratio %u could be wrong\n", porch_ratio);

	HISI_FB_INFO("c_ratio=%u, dpiSize=%u, result%u\n", cmp_ratio, active_dpi_hsize, porch_ratio);
	return porch_ratio;
}

static void get_dsi_phy_ctrl(struct hisi_fb_data_type *hisifd,
	struct mipi_dsi_phy_ctrl *phy_ctrl)
{
	if (hisifd->panel_info.mipi.phy_mode == DPHY_MODE)
		get_dsi_dphy_ctrl(hisifd, phy_ctrl);
	else
		get_dsi_cphy_ctrl(hisifd, phy_ctrl);
}

static bool need_skip_dirty_region_frame(struct hisi_fb_data_type *hisifd)
{
	uint32_t vrt_lines;
	uint32_t vactive_lines;
	char __iomem *mipi_dsi_base = NULL;

	mipi_dsi_base = get_mipi_dsi_base(hisifd);
	vrt_lines = inp32(mipi_dsi_base + MIPI_LDI_VRT_CTRL2) & 0x1FFF; /*lint !e529*/
	vactive_lines = inp32(mipi_dsi_base + MIPIDSI_VID_VACTIVE_LINES_OFFSET); /*lint !e529*/

	if (vrt_lines < (hisifd->panel_info.yres - 1) || vactive_lines < hisifd->panel_info.yres) {
		HISI_FB_INFO("skip, vrt_lines = 0x%x, vactive_lines = 0x%x, yres = 0x%x\n",
			vrt_lines, vactive_lines, hisifd->panel_info.yres);
		return true;
	}

	return false;
}

static bool need_send_dfr_cmds_in_vactive(struct hisi_fb_data_type *hisifd)
{
	bool ret = false;

	if (is_mipi_cmd_panel(hisifd) &&
		hisifd->panel_info.send_dfr_cmd_in_vactive &&
		hisifd->panel_info.frm_rate_ctrl.target_frame_rate == FPS_60HZ)
		ret = true;

	return ret;
}

void dfr_status_convert_on_isr_vstart(struct hisi_fb_data_type *hisifd)
{
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	if (!hisifd || hisifd->index != PRIMARY_PANEL_IDX || !is_lcd_dfr_support(hisifd))
		return;

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	if (frm_rate_ctrl->status == FRM_UPDT_CONFIG_DONE) {
		if (need_skip_dirty_region_frame(hisifd))
			return;

		frm_rate_ctrl->status = FRM_UPDT_NEED_DOING;

		if (need_send_dfr_cmds_in_vactive(hisifd))
			hisifd->fps_upt_isr_handler(hisifd);  /* sending fps change cmds to ddic */

		if (is_mipi_video_panel(hisifd))
			disable_ldi(hisifd);
	}
}

bool need_update_frame_rate(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd)
		return false;

	/* video panel should wait a vsync not before first frame displayed */
	if (is_mipi_video_panel(hisifd) && (hisifd->online_play_count < 2))
		return false;

	if (is_lcd_dfr_support(hisifd) &&
		(hisifd->esd_check_is_doing == 0) &&
		hisifd->panel_info.fps_updt != hisifd->panel_info.fps &&
		hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_DONE) {
		HISI_FB_INFO("fps_updt %d, fps %d\n", hisifd->panel_info.fps_updt, hisifd->panel_info.fps);
		return true;
	}

	return false;
}

static bool check_dfr_core_rate(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *panel_info = NULL;
	uint64_t need_core_rate;
	uint64_t current_core_rate;
	uint32_t porch_ratio;
	uint32_t fps;

	panel_info = &(hisifd->panel_info);

	porch_ratio = panel_info->frm_rate_ctrl.porch_ratio;
	fps = panel_info->frm_rate_ctrl.target_frame_rate;

	need_core_rate = calc_core_rate(hisifd, porch_ratio, fps);
	current_core_rate = hisifd->dss_vote_cmd.dss_pri_clk_rate;

	HISI_FB_DEBUG("dfr core rate need[%llu] get[%llu]\n",
		need_core_rate, current_core_rate);

	if (current_core_rate < need_core_rate) {
		HISI_FB_ERR("need_core_rate [%llu], current_core_rate [%llu], not enough!\n",
			need_core_rate, current_core_rate);
		return false;
	}

	return true;
}

static bool ocean_product(struct hisi_fb_data_type *hisifd)
{
	if (hisifd->panel_info.diff_product_type != 0) // Ocean M536:1, Ocean bal lite:2, others:0
		return true;

	return false;
}

/* low fps switch to high fps, we need to check the first high fps frame should be prepared by target fps.
 * high fps switch to low fps, we don't need to check it.
 */
static bool check_target_fps(struct hisi_fb_data_type *hisifd)
{
	int target_frame_rate;
	int current_frame_rate;
	struct hisi_panel_info *panel_info = NULL;
	panel_info = &(hisifd->panel_info);

	target_frame_rate = panel_info->frm_rate_ctrl.target_frame_rate;
	current_frame_rate = panel_info->frm_rate_ctrl.current_frame_rate;

	HISI_FB_INFO("dfr target fps:(%d), frameFps(%d)\n", target_frame_rate, hisifd->ov_req_prev.frameFps);

	if (((current_frame_rate < target_frame_rate) && (target_frame_rate == hisifd->ov_req_prev.frameFps)) ||
		(current_frame_rate > target_frame_rate))
		return true;

	return false;
}

bool need_config_frame_rate_timing(struct hisi_fb_data_type *hisifd)
{
	bool ret = false;

	if (!hisifd)
		return ret;

	if (is_lcd_dfr_support(hisifd) &&
		(hisifd->panel_info.frm_rate_ctrl.status == FRM_UPDT_NEED_DOING)) {
		/* If esd is doning at this time, the dfr set_reg will be delayed. But the ldi need enable here,
		 * responsible for the waitting vactive satrt timeout for video panel
		 */
		if (hisifd->esd_check_is_doing == 1 ||
			(ocean_product(hisifd) && !(check_dfr_core_rate(hisifd) && check_target_fps(hisifd)))) {
			if (is_mipi_video_panel(hisifd))
				enable_ldi(hisifd);
			hisifd->panel_info.frm_rate_ctrl.status = FRM_UPDT_CONFIG_DONE;
			ret = false;
		} else {
			HISI_FB_INFO("status=%d\n", hisifd->panel_info.frm_rate_ctrl.status);
			ret = true;
		}
	}

	return ret;
}

static void frm_rate_update_dsi_timing_config(struct hisi_fb_data_type *hisifd)
{
	struct mipi_panel_info *mipi = NULL;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);
	if (!frm_rate_ctrl) {
		HISI_FB_ERR("frm_rate_ctrl is NULL\n");
		return;
	}
	mipi = get_mipi_ctrl(hisifd);

	frm_rate_ctrl->timing.dpi_hsize = mipi->dpi_hsize;
	frm_rate_ctrl->timing.hsa = mipi->hsa;
	frm_rate_ctrl->timing.hbp = mipi->hbp;
	frm_rate_ctrl->timing.hline_time = mipi->hline_time;
	frm_rate_ctrl->timing.width = mipi->width;

	frm_rate_ctrl->timing.vsa = mipi->vsa;
	frm_rate_ctrl->timing.vbp = mipi->vbp;
	frm_rate_ctrl->timing.vfp = mipi->vfp;
	frm_rate_ctrl->timing.vactive_line = mipi->vactive_line;

	frm_rate_ctrl->target_dsi_bit_clk = mipi->dsi_bit_clk_upt;

	HISI_FB_INFO("get paramters:dpi_hsize = %d,hsa = %d,hbp = %d,hline_time = %d, "
		"vsa = %d,vbp = %d,vfp = %d,vactive_line = %d,target_dsi_bit_clk = %d\n",
		mipi->dpi_hsize, mipi->hsa, mipi->hbp, mipi->hline_time,
		mipi->vsa, mipi->vbp, mipi->vfp, mipi->vactive_line, mipi->dsi_bit_clk_upt);
}

static uint32_t get_rect_width(struct hisi_fb_data_type *hisifd)
{
	struct hisi_panel_info *pinfo = NULL;
	uint32_t rectwidth;
	uint32_t ifbc_type;
	uint32_t mipi_idx;
	uint32_t xres_div;

	pinfo = &(hisifd->panel_info);
	mipi_idx = 0;
	ifbc_type = pinfo->ifbc_type;
	if (ifbc_type >= IFBC_TYPE_MAX) {
		HISI_FB_ERR("ifbc_type not correct\n");
		return 0;
	}

	xres_div = g_mipi_ifbc_division[mipi_idx][ifbc_type].xres_div;
	if (xres_div == 0) {
		HISI_FB_ERR("xres_div is 0\n");
		return 0;
	}

	if ((ifbc_type == IFBC_TYPE_VESA3X_DUAL) &&
		(pinfo->current_mode == MODE_10BIT_VIDEO_3X)) {
		/* data size of 10bit vesa3x is larger than 10bit vesa3.75x */
		rectwidth = pinfo->xres * 30 / 24 / xres_div;
	} else {
		rectwidth = pinfo->xres / xres_div;
	}

#ifdef SUPPORT_SPR_DSC
	rectwidth = get_hsize_after_spr_dsc(hisifd, rectwidth);
#endif

	HISI_FB_DEBUG("xres_div=%d, rectwidth=%d\n", xres_div, rectwidth);

	return rectwidth;
}

static void frm_rate_update_dsc_dynamic_config(struct hisi_fb_data_type *hisifd)
{
	struct mipi_panel_info *mipi = NULL;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;
	struct hisi_panel_info *pinfo = NULL;
	uint32_t rectwidth;

	pinfo = &(hisifd->panel_info);
	rectwidth = hisifd->panel_info.xres;

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);
	if (!frm_rate_ctrl) {
		HISI_FB_ERR("frm_rate_ctrl is NULL\n");
		return;
	}

	if (!pinfo->dynamic_dsc_support)
		return;

	mipi = get_mipi_ctrl(hisifd);

	if (frm_rate_ctrl->target_dsc_en == mipi->dynamic_dsc_en)
		return;

	rectwidth = get_rect_width(hisifd);
	if (rectwidth == 0) {
		HISI_FB_ERR("rectwidth not correct\n");
		return;
	}

	frm_rate_ctrl->target_dsc_en = mipi->dynamic_dsc_en;
	frm_rate_ctrl->dbuf_hsize = rectwidth;
	frm_rate_ctrl->dbuf_size = rectwidth * pinfo->yres;
	frm_rate_ctrl->timing.width = rectwidth;
	if (is_dual_mipi_panel(hisifd))
		frm_rate_ctrl->dmipi_hsize = (DSS_WIDTH(rectwidth * 2) << 16) | DSS_WIDTH(rectwidth);

	HISI_FB_INFO("get paramters:dynamic_dsc_en = %d,target_dsc_en = %d,current_dsc_en = %d, "
		"rectwidth = %d,dbuf_hsize = %d,dbuf_size = %d,dmipi_hsize = %d, width = %d\n",
		mipi->dynamic_dsc_en, frm_rate_ctrl->target_dsc_en, frm_rate_ctrl->current_dsc_en,
		rectwidth, frm_rate_ctrl->dbuf_hsize, frm_rate_ctrl->dbuf_size,
		frm_rate_ctrl->dmipi_hsize, frm_rate_ctrl->timing.width);
}

static int mipi_dsi_frm_rate_para_config(struct hisi_fb_data_type *hisifd)
{
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	frm_rate_update_dsi_timing_config(hisifd);
	frm_rate_update_dsc_dynamic_config(hisifd);

	HISI_FB_INFO("current_dsi_bit_clk = %llu, target_dsi_bit_clk = %llu\n",
		frm_rate_ctrl->current_dsi_bit_clk, frm_rate_ctrl->target_dsi_bit_clk);

	if (frm_rate_ctrl->current_dsi_bit_clk != frm_rate_ctrl->target_dsi_bit_clk) {
		memset(&(frm_rate_ctrl->phy_ctrl), 0, sizeof(frm_rate_ctrl->phy_ctrl));
		get_dsi_phy_ctrl(hisifd, &(frm_rate_ctrl->phy_ctrl));
	}

	return 0;
}

static void frm_update_set_dsi_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *mipi_dsi_base, struct frame_rate_ctrl *frm_rate_ctrl)
{
	uint32_t ldi_vrt_ctrl0;
	uint32_t vsync_delay_cnt;
	uint32_t tmp_hline_time;
	struct hisi_panel_info *pinfo = NULL;

	pinfo = &(hisifd->panel_info);

	tmp_hline_time =
		get_mipi_timing_hline_time(hisifd, frm_rate_ctrl->timing.hline_time);

	set_reg(mipi_dsi_base + MIPIDSI_VID_HLINE_TIME_OFFSET, tmp_hline_time, 15, 0);
	if (pinfo->dynamic_dsc_support) {  /* do not consider dsi_bit_clk_upt !! */
		set_reg(mipi_dsi_base + MIPIDSI_VID_HBP_TIME_OFFSET, frm_rate_ctrl->timing.hbp, 12, 0);
		set_reg(mipi_dsi_base + MIPIDSI_VID_HSA_TIME_OFFSET, frm_rate_ctrl->timing.hsa, 12, 0);
	}
	set_reg(mipi_dsi_base + MIPIDSI_VID_VBP_LINES_OFFSET, frm_rate_ctrl->timing.vbp, 10, 0);

	if (frm_rate_ctrl->timing.vfp > V_FRONT_PORCH_MAX) {
		ldi_vrt_ctrl0 = V_FRONT_PORCH_MAX;
		vsync_delay_cnt = (frm_rate_ctrl->timing.vfp - V_FRONT_PORCH_MAX) * tmp_hline_time;
	} else {
		ldi_vrt_ctrl0 = frm_rate_ctrl->timing.vfp;
		/* If vsync delay is enable, the vsync_delay_count can not be set to 0.
		 * It is recommended to set to a small value to avoid the abnormality of vsync_delay function
		 */
		vsync_delay_cnt = 5;
	}
	set_reg(mipi_dsi_base + MIPIDSI_VID_VFP_LINES_OFFSET, ldi_vrt_ctrl0, 10, 0);
	set_reg(mipi_dsi_base + MIPI_VSYNC_DELAY_TIME, vsync_delay_cnt, 32, 0);

	frm_rate_ctrl->current_hline_time = tmp_hline_time;
	frm_rate_ctrl->current_vfp = frm_rate_ctrl->timing.vfp;

	HISI_FB_INFO("set paramters:dpi_hsize = %d,hsa = %d,hbp = %d,hline_time = %d,"
		"vsa=%d,vbp=%d,vfp=%d,vactive_line=%d,vsync_delay_cnt=%d,ldi_vrt_ctrl0=%d\n",
		frm_rate_ctrl->timing.dpi_hsize, frm_rate_ctrl->timing.hsa,
		frm_rate_ctrl->timing.hbp, frm_rate_ctrl->timing.hline_time,
		frm_rate_ctrl->timing.vsa, frm_rate_ctrl->timing.vbp,
		frm_rate_ctrl->timing.vfp, frm_rate_ctrl->timing.vactive_line,
		vsync_delay_cnt, ldi_vrt_ctrl0);
}

static void frm_update_set_dynamic_dsc_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *mipi_dsi_base, struct frame_rate_ctrl *frm_rate_ctrl)
{
	char __iomem *dsc_base = NULL;
	char __iomem *dbuf_base = NULL;
	struct hisi_panel_info *pinfo = NULL;

	pinfo = &(hisifd->panel_info);
	if (!pinfo->dynamic_dsc_support)
		return;

	dsc_base = hisifd->dss_base + DSS_DSC_OFFSET;
	dbuf_base = hisifd->dss_base + DSS_DBUF0_OFFSET;

	if (frm_rate_ctrl->current_dsc_en == frm_rate_ctrl->target_dsc_en)
		return;

	/* dsc param */
	set_reg(dsc_base + DSC_EN, frm_rate_ctrl->target_dsc_en, 1, 0);

	/* dbuf param */
	set_reg(dbuf_base + DBUF_FRM_HSIZE, DSS_WIDTH(frm_rate_ctrl->dbuf_hsize), 32, 0);
	set_reg(dbuf_base + DBUF_FRM_SIZE, frm_rate_ctrl->dbuf_size, 32, 0);

	/* dsi param */
	if (is_dual_mipi_panel(hisifd))
		set_reg(hisifd->dss_base + DSS_DISP_GLB_OFFSET + DUAL_MIPI_HSIZE,
			frm_rate_ctrl->dmipi_hsize, 32, 0);

	set_reg(mipi_dsi_base + MIPI_LDI_DPI0_HRZ_CTRL3,
		DSS_WIDTH(frm_rate_ctrl->timing.dpi_hsize), 13, 0);
	set_reg(mipi_dsi_base + MIPI_LDI_DPI0_HRZ_CTRL2,
		DSS_WIDTH(frm_rate_ctrl->timing.width), 13, 0);
	set_reg(mipi_dsi_base + MIPIDSI_VID_PKT_SIZE_OFFSET,
		frm_rate_ctrl->timing.width, 14, 0);

	HISI_FB_INFO("set paramters:target_dsc_en = %d,current_dsc_en = %d, "
		"dbuf_hsize = %d,dbuf_size = %d,dmipi_hsize = %d, width = %d, ifbc_type = %d\n",
		frm_rate_ctrl->target_dsc_en, frm_rate_ctrl->current_dsc_en,
		frm_rate_ctrl->dbuf_hsize,  frm_rate_ctrl->dbuf_size,
		frm_rate_ctrl->dmipi_hsize, frm_rate_ctrl->timing.width, pinfo->ifbc_type);
}

static void phy_pll_reset(char __iomem *mipi_dsi_base)
{
	mipi_config_phy_test_code(mipi_dsi_base, 0x00010031, 0x8);
	mipi_config_phy_test_code(mipi_dsi_base, 0x0001003C, 0x0);
	udelay(1);  /* 1us */
	mipi_config_phy_test_code(mipi_dsi_base, 0x0001003C, 0x1);
	mipi_config_phy_test_code(mipi_dsi_base, 0x00010031, 0x0);
}

static void frm_update_set_phy_reg(struct hisi_fb_data_type *hisifd,
	char __iomem *mipi_dsi_base, const struct mipi_dsi_phy_ctrl *phy_ctrl)
{
	unsigned long dw_jiffies;
	uint32_t temp;
	bool is_ready = false;
	uint32_t status_phy_lock = 0x1;

	mipi_config_phy_test_code(mipi_dsi_base, 0x00010049,
		(phy_ctrl->rg_pll_posdiv << 4) | phy_ctrl->rg_pll_prediv);

	mipi_config_phy_test_code(mipi_dsi_base, 0x0001004A, phy_ctrl->rg_pll_fbkdiv);
	HISI_FB_INFO("set mipi pll:rg_pll_prediv=%d, rg_pll_posdiv=%d, rg_pll_fbkdiv=%d\n",
			phy_ctrl->rg_pll_prediv, phy_ctrl->rg_pll_posdiv,
			phy_ctrl->rg_pll_fbkdiv);

	mipi_config_phy_test_code(mipi_dsi_base, 0x0001004B, 0x1);

	if (hisifd->panel_info.mipi.phy_mode == CPHY_MODE)
		mipi_config_cphy_spec1v0_parameter(mipi_dsi_base, &(hisifd->panel_info), phy_ctrl);
	else
		mipi_config_dphy_spec1v2_parameter(mipi_dsi_base, &(hisifd->panel_info), phy_ctrl);

	phy_pll_reset(mipi_dsi_base);

	dw_jiffies = jiffies + HZ / 2;  /* HZ/2 = 0.5s */
	do {
		temp = inp32(mipi_dsi_base + MIPIDSI_PHY_STATUS_OFFSET);  /*lint !e529*/
		if ((temp & status_phy_lock) == status_phy_lock) {
			is_ready = true;
			break;
		}
	} while (time_after(dw_jiffies, jiffies));

	if (!is_ready) {
		HISI_FB_ERR("fb%d, phylock is not ready!MIPIDSI_PHY_STATUS_OFFSET=0x%x.\n",
			hisifd->index, temp);
	}

	set_reg(mipi_dsi_base + MIPIDSI_PHY_IF_CFG_OFFSET, phy_ctrl->phy_stop_wait_time, 8, 8);

	outp32(mipi_dsi_base + MIPIDSI_PHY_TMR_LPCLK_CFG_OFFSET,
		(phy_ctrl->clk_lane_lp2hs_time + (phy_ctrl->clk_lane_hs2lp_time << 16)));

	outp32(mipi_dsi_base + MIPIDSI_PHY_TMR_CFG_OFFSET,
		(phy_ctrl->data_lane_lp2hs_time + (phy_ctrl->data_lane_hs2lp_time << 16)));

	/* escape clock dividor */
	set_reg(mipi_dsi_base + MIPIDSI_CLKMGR_CFG_OFFSET,
		(phy_ctrl->clk_division + (phy_ctrl->clk_division << 8)), 16, 0);
}

static uint32_t get_ldi_vstate_idle_vfp_time(struct hisi_fb_data_type *hisifd, uint64_t lane_byte_clk)
{
	uint32_t vfp_line;
	uint32_t hline_time;
	uint32_t vsync_delay_cnt;
	uint32_t vfp_time;

	hline_time = inp32(hisifd->mipi_dsi0_base + MIPIDSI_VID_HLINE_TIME_OFFSET) & 0x7FFF;  /*lint !e529*/
	vfp_line = inp32(hisifd->mipi_dsi0_base + MIPIDSI_VID_VFP_LINES_OFFSET) & 0x3FF;  /*lint !e529*/
	vfp_line += 10; /* vfp margin 10 lines */
	vsync_delay_cnt = inp32(hisifd->mipi_dsi0_base + MIPI_VSYNC_DELAY_TIME);  /*lint !e529*/

	vfp_time = (vfp_line * hline_time + vsync_delay_cnt) /
		((uint32_t)(lane_byte_clk / 1000000UL)); /* unit convert to us from s */

	return vfp_time;
}

static bool wait_ldi_vstate_idle(struct hisi_fb_data_type *hisifd, struct timeval *tv0)
{
	bool ret = false;
	uint32_t ldi_vstate;
	uint64_t lane_byte_clk;
	uint32_t vfp_time;
	struct timeval tv1;
	uint32_t timediff = 0;
	uint32_t state_idle = 0x1;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	lane_byte_clk = frm_rate_ctrl->current_lane_byte_clk;

	vfp_time = get_ldi_vstate_idle_vfp_time(hisifd, lane_byte_clk);

	while (timediff < vfp_time) {
		udelay(5);  /* 5us */
		hisifb_get_timestamp(&tv1);
		timediff = hisifb_timestamp_diff(tv0, &tv1);
		ldi_vstate = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE) & 0xFFFF;  /*lint !e529*/
		if ((ldi_vstate & state_idle) == state_idle) {
			ret = true;
			break;
		}
	}

	HISI_FB_INFO("timediff = %d us, vfp_time = %d us, ldi_vstate = 0x%x, lane_byte_clk = %llu\n",
		timediff, vfp_time, ldi_vstate, lane_byte_clk);

	return ret;
}

static int mipi_dsi_frm_rate_set_reg(struct hisi_fb_data_type *hisifd,
	struct frame_rate_ctrl *frm_rate_ctrl, struct timeval *tv0)
{
	char __iomem *mipi_dsi_base = NULL;
	mipi_dsi_base = get_mipi_dsi_base(hisifd);

	if (frm_rate_ctrl->current_dsi_bit_clk == frm_rate_ctrl->target_dsi_bit_clk) {
		if (!need_send_dfr_cmds_in_vactive(hisifd))
			hisifd->fps_upt_isr_handler(hisifd);  /* sending fps change cmds to ddic */

		frm_update_set_dsi_reg(hisifd, mipi_dsi_base, frm_rate_ctrl);
		frm_update_set_dynamic_dsc_reg(hisifd, mipi_dsi_base, frm_rate_ctrl);
		if (is_dual_mipi_panel(hisifd)) {
			frm_update_set_dsi_reg(hisifd, hisifd->mipi_dsi1_base, frm_rate_ctrl);
			frm_update_set_dynamic_dsc_reg(hisifd, hisifd->mipi_dsi1_base, frm_rate_ctrl);
		}
	} else {
		if (!wait_ldi_vstate_idle(hisifd, tv0)) {
			frm_rate_ctrl->status = FRM_UPDT_CONFIG_DONE;
			HISI_FB_INFO("wait vstatus idle timeout");
			return -EINVAL;
		}

		if (!need_send_dfr_cmds_in_vactive(hisifd))
			hisifd->fps_upt_isr_handler(hisifd);  /* sending fps change cmds to ddic */

		frm_update_set_dsi_reg(hisifd, mipi_dsi_base, frm_rate_ctrl);
		frm_update_set_dynamic_dsc_reg(hisifd, mipi_dsi_base, frm_rate_ctrl);
		frm_update_set_phy_reg(hisifd, mipi_dsi_base, &frm_rate_ctrl->phy_ctrl);
		if (is_dual_mipi_panel(hisifd)) {
			frm_update_set_dsi_reg(hisifd, hisifd->mipi_dsi1_base, frm_rate_ctrl);
			frm_update_set_dynamic_dsc_reg(hisifd, hisifd->mipi_dsi1_base, frm_rate_ctrl);
			frm_update_set_phy_reg(hisifd, hisifd->mipi_dsi1_base, &frm_rate_ctrl->phy_ctrl);
		}
	}

	set_disrupt_vsync_delay(hisifd, 1);

	if ((frm_rate_ctrl->target_frame_rate == FPS_60HZ) ||
		(frm_rate_ctrl->target_frame_rate == FPS_HIGH_60HZ)) {
		frm_rate_ctrl->notify_type = TYPE_NOTIFY_PERF_ONLY;
		frm_rate_ctrl->porch_ratio = get_porch_ratio(hisifd);
		queue_work(hisifd->dfr_notice_wq, &hisifd->dfr_notice_work);
	}

	check_disrupt_vsync_delay(hisifd);

	set_disrupt_vsync_delay(hisifd, 0);

	return 0;
}

/* vactive end interrupt call this to set frm rate para */
int mipi_dsi_frm_rate_para_set_reg(struct hisi_fb_data_type *hisifd)
{
	uint8_t esd_enable;
	struct timeval tv0;
	struct hisi_panel_info *pinfo = NULL;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;
	int ret = 0;
	bool dfr_succ = false;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	pinfo = &(hisifd->panel_info);
	hisi_check_and_return((!is_lcd_dfr_support(hisifd)), -EINVAL, ERR, "dfr not support\n");

	esd_enable = pinfo->esd_enable;
	if (is_mipi_video_panel(hisifd)) {
		pinfo->esd_enable = 0;
		disable_ldi(hisifd);
	}

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);
	hisifb_get_timestamp(&tv0);

	spin_lock(&hisifd->mipi_resource_lock);

	/* if frm update has higher priority than online play, change status to FRM_UPDT_DOING at vactive start int */
	if (frm_rate_ctrl->status != FRM_UPDT_NEED_DOING) {
		HISI_FB_ERR("wrong status %d", frm_rate_ctrl->status);
		spin_unlock(&hisifd->mipi_resource_lock);
		goto exit;
	}
	frm_rate_ctrl->status = FRM_UPDT_DOING;

	spin_unlock(&hisifd->mipi_resource_lock);

	HISI_FB_DEBUG("current_dsi_bit_clk = %llu, target_dsi_bit_clk = %llu\n",
		frm_rate_ctrl->current_dsi_bit_clk, frm_rate_ctrl->target_dsi_bit_clk);

	if (hisifd->underflow_flag > 0) {
		frm_rate_ctrl->status = FRM_UPDT_CONFIG_DONE;
		HISI_FB_INFO("skip this frame, flag %d\n", hisifd->underflow_flag);
		goto exit;
	}

	ret = mipi_dsi_frm_rate_set_reg(hisifd, frm_rate_ctrl, &tv0);
	if (ret < 0)
		goto exit;

	/*lint -e529*/
	HISI_FB_INFO("fps successfully changed from %d to %d, vstate=0x%x\n",
		pinfo->fps, frm_rate_ctrl->target_frame_rate, inp32(hisifd->mipi_dsi0_base + MIPI_LDI_VSTATE));
	/*lint +e529*/
#ifdef CONFIG_HUAWEI_DUBAI
	/* report when lcd fresh rate change */
	HWDUBAI_LOGE("DUBAI_TAG_EPS_LCD_FREQ", "sourcerate=%d targetrate=%d",
		pinfo->fps, frm_rate_ctrl->target_frame_rate);
#endif

	dfr_succ = true;

exit:
	if (is_mipi_video_panel(hisifd)) {
		pinfo->esd_enable = esd_enable;
		enable_ldi(hisifd);
	}
	hisi_dfr_end_process(hisifd, dfr_succ);

	return ret;
}

/* frame rate update entrance */
int mipi_dsi_frm_rate_ctrl(struct hisi_fb_data_type *hisifd, int frm_rate)
{
	int ret;
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	hisi_check_and_return(!hisifd, 0, ERR, "hisifd is NULL\n");

	hisi_check_and_return((hisifd->index != PRIMARY_PANEL_IDX), 0,
		INFO, "hisifd->index is not PRIMARY_PANEL_IDX!\n");

	hisi_check_and_return((!hisifd->panel_power_on), 0, INFO,
		"fb%d, panel is power off\n", hisifd->index);

	hisi_check_and_return((!is_lcd_dfr_support(hisifd)), 0,
		INFO, "dfr is not support\n");

	hisi_check_and_return((g_dss_dfr_debug & BIT(0)), 0, INFO,
		"g_dss_dfr_debug = 0x%x, return\n", g_dss_dfr_debug);

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	spin_lock(&hisifd->mipi_resource_lock);
	if (frm_rate_ctrl->status != FRM_UPDT_DONE) {
		HISI_FB_INFO("last frame change has not been completed, status = %d\n",
			frm_rate_ctrl->status);
		spin_unlock(&hisifd->mipi_resource_lock);
		return 0;
	}
	if (frm_rate == frm_rate_ctrl->current_frame_rate) {
		HISI_FB_INFO("frm_rate %d does not change\n", frm_rate);
		spin_unlock(&hisifd->mipi_resource_lock);
		return 0;
	}
	frm_rate_ctrl->status = FRM_UPDT_CONFIG;
	frm_rate_ctrl->target_frame_rate = frm_rate;
	spin_unlock(&hisifd->mipi_resource_lock);

	if ((frm_rate_ctrl->target_frame_rate == FPS_60HZ) ||
		(frm_rate_ctrl->target_frame_rate == FPS_HIGH_60HZ))
		frm_rate_ctrl->notify_type = TYPE_NOTIFY_EFFECT_ONLY;
	else
		frm_rate_ctrl->notify_type = TYPE_NOTIFY_PERF_EFFECT;

	frm_rate_ctrl->porch_ratio = get_porch_ratio(hisifd);
	queue_work(hisifd->dfr_notice_wq, &hisifd->dfr_notice_work);

	ret = mipi_dsi_frm_rate_para_config(hisifd);

	spin_lock(&hisifd->mipi_resource_lock);
	frm_rate_ctrl->status = FRM_UPDT_CONFIG_DONE;
	spin_unlock(&hisifd->mipi_resource_lock);

	HISI_FB_INFO("frm_rate_para_config has been calculated\n");

	return ret;
}

void dfr_power_on_notification(struct hisi_fb_data_type *hisifd)
{
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	if (hisifd->index != PRIMARY_PANEL_IDX) {
		HISI_FB_DEBUG("fb%d is not primary panel\n", hisifd->index);
		return;
	}

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	if (!is_lcd_dfr_support(hisifd) || frm_rate_ctrl->registered == 0)
		return;

	if (hisifd->panel_info.dfr_method == DFR_METHOD_VPORCH) {
		frm_rate_ctrl->notify_type = TYPE_NOTIFY_PERF_ONLY;
		frm_rate_ctrl->porch_ratio = get_porch_ratio(hisifd);
		queue_work(hisifd->dfr_notice_wq, &hisifd->dfr_notice_work);
	}

	HISI_FB_INFO("notify hwc, dfr_method 0x%x\n", hisifd->panel_info.dfr_method);
}

static void hisi_create_dfr_thread(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->panel_info.fps_updt = FPS_60HZ;
	hisifd->dfr_notice_wq = create_singlethread_workqueue("dfr_notice_work");
	hisi_check_and_no_retval(!hisifd->dfr_notice_wq, ERR, "fb%d, create dfr notice workqueue failed!\n",
		hisifd->index);
	INIT_WORK(&hisifd->dfr_notice_work, hisi_dfr_notice_handle_func);

	hisifd->dfr_delay_wq = create_singlethread_workqueue("dfr_delay_work");
	hisi_check_and_no_retval(!hisifd->dfr_delay_wq, ERR, "fb%d, create dfr delay workqueue failed!\n",
		hisifd->index);
	INIT_WORK(&hisifd->dfr_delay_work, hisi_dfr_delay_work);

	HISI_FB_INFO("init succ\n");
}

/* panel off, set default frame rate */
void mipi_dsi_frm_rate_ctrl_init(struct hisi_fb_data_type *hisifd)
{
	struct frame_rate_ctrl *frm_rate_ctrl = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");
	hisi_check_and_no_retval((hisifd->index != PRIMARY_PANEL_IDX), DEBUG, "fb%d is not primary panel\n",
		hisifd->index);

	frm_rate_ctrl = &(hisifd->panel_info.frm_rate_ctrl);

	frm_rate_ctrl->target_frame_rate = FPS_60HZ;
	frm_rate_ctrl->current_frame_rate = FPS_60HZ;
	frm_rate_ctrl->status = FRM_UPDT_DONE;
	hisifd->panel_info.fps = FPS_60HZ;
	frm_rate_ctrl->current_hline_time = hisifd->panel_info.mipi.hline_time;
	frm_rate_ctrl->current_vfp = hisifd->panel_info.mipi.vfp;
	frm_rate_ctrl->current_dsi_bit_clk = hisifd->panel_info.mipi.dsi_bit_clk;
	frm_rate_ctrl->target_dsi_bit_clk = frm_rate_ctrl->current_dsi_bit_clk;
	frm_rate_ctrl->current_dsc_en = hisifd->panel_info.dynamic_dsc_en;
	frm_rate_ctrl->target_dsc_en = frm_rate_ctrl->current_dsc_en;
#ifdef CONFIG_HISI_FB_V600
	frm_rate_ctrl->ignore_hporch = true;
#else
	frm_rate_ctrl->ignore_hporch = hisifd->panel_info.mipi.ignore_hporch ? true : false;
#endif
	memset(&(frm_rate_ctrl->phy_ctrl), 0, sizeof(frm_rate_ctrl->phy_ctrl));

	if ((hisifd->panel_info.frm_rate_ctrl.registered == 0) &&
		((hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) == 0)) {
		hisifd->panel_info.frm_rate_ctrl.registered = 1;
		hisifd->panel_info.fps_updt = FPS_60HZ;
		hisifd->dfr_notice_wq = create_singlethread_workqueue("dfr_notice_work");
		hisi_check_and_no_retval(!hisifd->dfr_notice_wq, ERR, "fb%d, create dfr notice workqueue failed!\n",
			hisifd->index);
		INIT_WORK(&hisifd->dfr_notice_work, hisi_dfr_notice_handle_func);

		hisifd->dfr_delay_wq = create_singlethread_workqueue("dfr_delay_work");
		hisi_check_and_no_retval(!hisifd->dfr_delay_wq, ERR, "fb%d, create dfr delay workqueue failed!\n",
			hisifd->index);
		INIT_WORK(&hisifd->dfr_delay_work, hisi_dfr_delay_work);

		HISI_FB_INFO("init succ\n");
		return;
	}

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		if (hisifd->panel_info.frm_rate_ctrl.registered == 0)
			hisifd->panel_info.frm_rate_ctrl.registered = 1;

		if (hisifd->dfr_create_singlethread == false) {
			hisifd->dfr_create_singlethread = true;
			hisi_create_dfr_thread(hisifd);
			return;
		}

		if (hisifd->panel_info.fps_updt == 0) {
			hisifd->panel_info.fps_updt = FPS_60HZ;
			return;
		}
	}

	get_dsi_phy_ctrl(hisifd, &(frm_rate_ctrl->phy_ctrl));
	frm_rate_ctrl->current_lane_byte_clk = frm_rate_ctrl->phy_ctrl.lane_byte_clk_default;

#ifdef CONFIG_HUAWEI_DUBAI
	/* mipi fps rate control init */
	HWDUBAI_LOGE("DUBAI_TAG_EPS_LCD_FREQ", "sourcerate=%d targetrate=%d",
		frm_rate_ctrl->current_frame_rate, frm_rate_ctrl->target_frame_rate);
#endif

	HISI_FB_INFO("fps_updt:%u, fps:%u, target_frame_rate:%d, current_frame_rate:%d, current_lane_byte_clk:%u\n",
		hisifd->panel_info.fps_updt, hisifd->panel_info.fps, frm_rate_ctrl->target_frame_rate,
		frm_rate_ctrl->current_frame_rate, frm_rate_ctrl->current_lane_byte_clk);
}

