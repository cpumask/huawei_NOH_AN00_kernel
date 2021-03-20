/*
 * lcd_kit_utils.c
 *
 * lcdkit utils function for lcd driver
 *
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
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

#include "lcd_kit_utils.h"
#include <linux/time.h>
#include "lcd_kit_disp.h"
#include "lcd_kit_common.h"
#include "lcd_kit_power.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_core.h"
#include "lcd_kit_sysfs_mtk.h"
#ifdef LCD_FACTORY_MODE
#include "lcd_kit_factory.h"
#endif
#ifdef CONFIG_DRM_MEDIATEK
#include "lcd_kit_drm_panel.h"
#else
#include "lcm_drv.h"
#endif

#ifndef CONFIG_DRM_MEDIATEK
extern struct LCM_DRIVER lcdkit_mtk_common_panel;
#endif
#define MAX_DELAY_TIME 200

bool lcd_kit_support(void)
{
	struct device_node *lcdkit_np = NULL;
	const char *support_type = NULL;
	ssize_t ret;

	lcdkit_np = of_find_compatible_node(NULL, NULL,
		DTS_COMP_LCD_KIT_PANEL_TYPE);
	if (!lcdkit_np) {
		LCD_KIT_ERR("NOT FOUND device node!\n");
		return false;
	}
	ret = of_property_read_string(lcdkit_np, "support_lcd_type",
		&support_type);
	if (ret) {
		LCD_KIT_ERR("failed to get support_type\n");
		return false;
	}
	if (!strncmp(support_type, "LCD_KIT", strlen("LCD_KIT"))) {
		LCD_KIT_INFO("lcd_kit is support!\n");
		return true;
	}
	LCD_KIT_INFO("lcd_kit is not support!\n");
	return false;
}

int lcd_kit_lread_reg(void *pdata, uint32_t *out,
	struct lcd_kit_dsi_cmd_desc *cmds, uint32_t len)
{
	return LCD_KIT_OK;
}

static int lcd_kit_check_project_id(void)
{
	int i;

	for (i = 0; i < PROJECTID_LEN; i++) {
		if (isalnum((disp_info->project_id.id)[i]) == 0 &&
			isdigit((disp_info->project_id.id)[i]) == 0)
			return LCD_KIT_FAIL;
	}

	return LCD_KIT_OK;
}

int lcd_kit_read_project_id(void)
{
	int ret;
	if (!disp_info) {
		LCD_KIT_ERR("disp_info is null\n");
		return LCD_KIT_FAIL;
	}

	if (disp_info->project_id.support) {
		memset(disp_info->project_id.id, 0,
			sizeof(disp_info->project_id.id));
#ifdef CONFIG_DRM_MEDIATEK
		ret = lcd_kit_dsi_cmds_extern_rx(disp_info->project_id.id,
			&disp_info->project_id.cmds, LCD_DDIC_INFO_LEN - 1);
#else
		ret = lcd_kit_dsi_cmds_extern_rx_v1(disp_info->project_id.id,
			&disp_info->project_id.cmds);
#endif
		if (ret) {
			LCD_KIT_ERR("read project id fail\n");
			return LCD_KIT_FAIL;
		}

		if (lcd_kit_check_project_id() == LCD_KIT_OK) {
			LCD_KIT_ERR("project id check fail\n");
			return LCD_KIT_FAIL;
		}

		LCD_KIT_INFO("read project id is %s\n",
			disp_info->project_id.id);
		return LCD_KIT_OK;
	} else {
		LCD_KIT_ERR("project id is not support!\n");
		return LCD_KIT_FAIL;
	}
}

int lcd_kit_updt_fps(struct platform_device *pdev)
{
	return LCD_KIT_OK;
}

int lcd_kit_updt_fps_scence(struct platform_device *pdev, uint32_t scence)
{
	return LCD_KIT_OK;
}

void lcd_kit_disp_on_check_delay(void)
{
	long delta_time_bl_to_panel_on;
	unsigned int delay_margin;
	struct timeval tv = {0};
	int max_delay_margin = MAX_DELAY_TIME;

	if (disp_info == NULL) {
		LCD_KIT_INFO("disp_info is NULL\n");
		return;
	}
	memset(&tv, 0, sizeof(struct timeval));
	do_gettimeofday(&tv);
	LCD_KIT_INFO("set backlight at %lu seconds %lu mil seconds\n",
				 tv.tv_sec, tv.tv_usec);
	/* change s to us */
	delta_time_bl_to_panel_on = (tv.tv_sec - disp_info->quickly_sleep_out.panel_on_record_tv.tv_sec) *
		1000000 + tv.tv_usec - disp_info->quickly_sleep_out.panel_on_record_tv.tv_usec;
	/* change us to ms */
	delta_time_bl_to_panel_on /= 1000;
	if (delta_time_bl_to_panel_on >= disp_info->quickly_sleep_out.interval) {
		LCD_KIT_INFO("%lu > %d, no need delay\n",
			delta_time_bl_to_panel_on,
			disp_info->quickly_sleep_out.interval);
		disp_info->quickly_sleep_out.panel_on_tag = false;
		return;
	}
	delay_margin = disp_info->quickly_sleep_out.interval -
		delta_time_bl_to_panel_on;
	if (delay_margin > max_delay_margin) {
		LCD_KIT_INFO("something maybe error");
		disp_info->quickly_sleep_out.panel_on_tag = false;
		return;
	}
	msleep(delay_margin);
	disp_info->quickly_sleep_out.panel_on_tag = false;
}

void lcd_kit_disp_on_record_time(void)
{
	if (disp_info == NULL) {
		LCD_KIT_INFO("disp_info is NULL\n");
		return;
	}
	do_gettimeofday(&disp_info->quickly_sleep_out.panel_on_record_tv);
	LCD_KIT_INFO("display on at %lu seconds %lu mil seconds\n",
		disp_info->quickly_sleep_out.panel_on_record_tv.tv_sec,
		disp_info->quickly_sleep_out.panel_on_record_tv.tv_usec);
	disp_info->quickly_sleep_out.panel_on_tag = true;
}

void lcd_kit_pinfo_init(struct device_node *np, struct mtk_panel_info *pinfo)
{
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return;
	}
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-xres",
		&pinfo->xres, 1440);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-yres",
		&pinfo->yres, 2560);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-width",
		&pinfo->width, 73);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-height",
		&pinfo->height, 130);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-orientation",
		&pinfo->orientation, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bpp",
		&pinfo->bpp, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bgr-fmt",
		&pinfo->bgr_fmt, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-type",
		&pinfo->bl_set_type, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-min",
		&pinfo->bl_min, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-max",
		&pinfo->bl_max, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-def",
		&pinfo->bl_default, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-cmd-type",
		&pinfo->type, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,panel-frc-enable",
		&pinfo->frc_enable, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,panel-esd-skip-mipi-check",
		&pinfo->esd_skip_mipi_check, 1);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ifbc-type",
		&pinfo->ifbc_type, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-ic-ctrl-type",
		&pinfo->bl_ic_ctrl_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-gpio-offset",
		&pinfo->gpio_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-pwm-out-div-value",
		&pinfo->blpwm_out_div_value, 0);
	OF_PROPERTY_READ_U64_RETURN(np, "lcd-kit,panel-pxl-clk",
		&pinfo->pxl_clk_rate);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-pxl-clk-div",
		&pinfo->pxl_clk_rate_div, 1);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-data-rate",
		&pinfo->data_rate, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-vsyn-ctr-type",
		&pinfo->vsync_ctrl_type, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-bl-pwm-preci-type",
		&pinfo->blpwm_precision_type, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,esd-recovery-bl-support",
		&pinfo->esd_recovery_bl_support, 0);

	/* effect info */
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,sbl-support",
	&pinfo->sbl_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,gamma-support",
		&pinfo->gamma_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,gmp-support",
		&pinfo->gmp_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,color-temp-support",
		&pinfo->color_temperature_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,color-temp-rectify-support",
		&pinfo->color_temp_rectify_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,comform-mode-support",
		&pinfo->comform_mode_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,cinema-mode-support",
		&pinfo->cinema_mode_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,xcc-support",
		&pinfo->xcc_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,hiace-support",
		&pinfo->hiace_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,panel-ce-support",
		&pinfo->panel_effect_support, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,arsr1p-sharpness-support",
		&pinfo->arsr1p_sharpness_support, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,imax-lcd-luminance",
		&pinfo->maxluminance, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,imin-lcd-luminance",
		&pinfo->minluminance, 0);

	/* ldi info */
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,h-back-porch",
	&pinfo->ldi.h_back_porch);
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,h-front-porch",
		&pinfo->ldi.h_front_porch);
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,h-pulse-width",
		&pinfo->ldi.h_pulse_width);
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,v-back-porch",
		&pinfo->ldi.v_back_porch);
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,v-front-porch",
		&pinfo->ldi.v_front_porch);
	OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,v-pulse-width",
		&pinfo->ldi.v_pulse_width);
	OF_PROPERTY_READ_U8_RETURN(np, "lcd-kit,ldi-hsync-plr",
		&pinfo->ldi.hsync_plr);
	OF_PROPERTY_READ_U8_RETURN(np, "lcd-kit,ldi-vsync-plr",
		&pinfo->ldi.vsync_plr);
	OF_PROPERTY_READ_U8_RETURN(np, "lcd-kit,ldi-pixel-clk-plr",
		&pinfo->ldi.pixelclk_plr);
	OF_PROPERTY_READ_U8_RETURN(np, "lcd-kit,ldi-data-en-plr",
		&pinfo->ldi.data_en_plr);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,ldi-dpi0-overlap-size",
		&pinfo->ldi.dpi0_overlap_size, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,ldi-dpi1-overlap-size",
		&pinfo->ldi.dpi1_overlap_size, 0);

	/* mipi hopping info */
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-hop-support",
		&pinfo->mipi_hopping.switch_en, 0);
	if (pinfo->mipi_hopping.switch_en != 0) {
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-h-back-porch",
			&pinfo->mipi_hopping.h_back_porch);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-h-front-porch",
			&pinfo->mipi_hopping.h_front_porch);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-h-pulse-width",
			&pinfo->mipi_hopping.h_pulse_width);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-v-back-porch",
			&pinfo->mipi_hopping.v_back_porch);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-v-front-porch",
			&pinfo->mipi_hopping.v_front_porch);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-v-pulse-width",
			&pinfo->mipi_hopping.v_pulse_width);
		OF_PROPERTY_READ_U32_RETURN(np, "lcd-kit,mipi-hop-lp-v-front-porch",
			&pinfo->mipi_hopping.vfp_lp_dyn);
		OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-hop-data-rate",
			&pinfo->mipi_hopping.data_rate, 0);
	}

	/* mipi info */
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,mipi-lane-nums",
	&pinfo->mipi.lane_nums, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,mipi-color-mode",
		&pinfo->mipi.color_mode, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,mipi-vc", &pinfo->mipi.vc, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-burst-mode",
		&pinfo->mipi.burst_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk",
		&pinfo->mipi.dsi_bit_clk, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-max-tx-esc-clk",
		&pinfo->mipi.max_tx_esc_clk, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk-val1",
		&pinfo->mipi.dsi_bit_clk_val1, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk-val2",
		&pinfo->mipi.dsi_bit_clk_val2, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk-val3",
		&pinfo->mipi.dsi_bit_clk_val3, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk-val4",
		&pinfo->mipi.dsi_bit_clk_val4, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-dsi-bit-clk-val5",
		&pinfo->mipi.dsi_bit_clk_val5, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,mipi-non-continue-enable",
		&pinfo->mipi.non_continue_en, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-post-adjust",
		&pinfo->mipi.clk_post_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-pre-adjust",
		&pinfo->mipi.clk_pre_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-t-hs-prepare-adjust",
		&pinfo->mipi.clk_t_hs_prepare_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-t-lpx-adjust",
		&pinfo->mipi.clk_t_lpx_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-t-hs-trail-adjust",
		&pinfo->mipi.clk_t_hs_trial_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-t-hs-exit-adjust",
		&pinfo->mipi.clk_t_hs_exit_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-clk-t-hs-zero-adjust",
		&pinfo->mipi.clk_t_hs_zero_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-data-t-hs-trail-adjust",
		&pinfo->mipi.data_t_hs_trial_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-rg-vrefsel-vcm-adjust",
		&pinfo->mipi.rg_vrefsel_vcm_adjust, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-phy-mode",
		&pinfo->mipi.phy_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-lp11_flag",
		&pinfo->mipi.lp11_flag, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-hs-wr-to-time",
		&pinfo->mipi.hs_wr_to_time, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,mipi-phy-update",
		&pinfo->mipi.phy_m_n_count_update, 0);
	OF_PROPERTY_READ_U8_DEFAULT(np, "lcd-kit,mipi-dsi-upt-support",
		&pinfo->dsi_bit_clk_upt_support, 0);

	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-lcm-type",
		&pinfo->panel_lcm_type, 0);
	LCD_KIT_ERR("pinfo->panel_lcm_type [%d]\n", pinfo->panel_lcm_type);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ldi-dsi-mode",
		&pinfo->panel_dsi_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-dsi-switch-mode",
		&pinfo->panel_dsi_switch_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ldi-trans-seq",
		&pinfo->panel_trans_seq, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ldi-data-padding",
		&pinfo->panel_data_padding, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ldi-packet-size",
		&pinfo->panel_packtet_size, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-ldi-ps",
		&pinfo->panel_ps, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-vfp-lp",
		&pinfo->ldi.v_front_porch_forlp, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-fbk-div",
		&pinfo->pxl_fbk_div, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,panel-density",
		&pinfo->panel_density, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,ssc-disable",
		&pinfo->ssc_disable, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,ssc-range",
		&pinfo->ssc_range, 0);
	/* frame rate */
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,vetical-refresh",
		&pinfo->vrefresh, 60);
	/* project id */
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,project-id-support",
		&disp_info->project_id.support, 0);
	if (disp_info->project_id.support) {
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,project-id-cmds",
			"lcd-kit,project-id-cmds-state",
			&disp_info->project_id.cmds);

		disp_info->project_id.default_project_id = (char*)of_get_property(np,
			"lcd-kit,default-project-id", NULL);
	}

	/* oem information */
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,oem-info-support",
		&disp_info->oeminfo.support, 0);

	if (disp_info->oeminfo.support) {
		OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,oem-barcode-2d-support",
			&disp_info->oeminfo.barcode_2d.support, 0);
		OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,oem-barcode-2d-num-offset",
			&disp_info->oeminfo.barcode_2d.number_offset, 0);
		if (disp_info->oeminfo.barcode_2d.support)
			lcd_kit_parse_dcs_cmds(np, "lcd-kit,barcode-2d-cmds",
				"lcd-kit,barcode-2d-cmds-state",
				&disp_info->oeminfo.barcode_2d.cmds);
	}

	if (common_info->esd.support)
		pinfo->esd_enable = 1;
}

void lcd_kit_parse_running(struct device_node *np)
{
}

void lcd_kit_parse_effect(struct device_node *np)
{
	/* rgbw */
	lcd_kit_parse_u32(np, "lcd-kit,rgbw-support",
		&disp_info->rgbw.support, 0);
	if (disp_info->rgbw.support) {
		lcd_kit_parse_u32(np, "lcd-kit,rgbw-bl-max",
			&disp_info->rgbw.rgbw_bl_max, 0);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-mode1-cmds",
			"lcd-kit,rgbw-mode1-cmds-state",
			&disp_info->rgbw.mode1_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-mode2-cmds",
			"lcd-kit,rgbw-mode2-cmds-state",
			&disp_info->rgbw.mode2_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-mode3-cmds",
			"lcd-kit,rgbw-mode3-cmds-state",
			&disp_info->rgbw.mode3_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-mode4-cmds",
			"lcd-kit,rgbw-mode4-cmds-state",
			&disp_info->rgbw.mode4_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-backlight-cmds",
			"lcd-kit,rgbw-backlight-cmds-state",
			&disp_info->rgbw.backlight_cmds);
		lcd_kit_parse_dcs_cmds(np, "lcd-kit,rgbw-pixel-gain-limit-cmds",
			"lcd-kit,rgbw-pixel-gain-limit-cmds-state",
			&disp_info->rgbw.pixel_gain_limit_cmds);
	}
}

void lcd_kit_parse_fps(struct device_node *np)
{
	int index;
	int i;

	lcd_kit_parse_array_data(np, "lcd-kit,panel-support-fps-list",
		&disp_info->fps.panel_support_fps_list);

	if (disp_info->fps.panel_support_fps_list.buf == NULL ||
		disp_info->fps.panel_support_fps_list.cnt == 0) {
		LCD_KIT_ERR("support list is null\n");
		return;
	}

	/* hopping */
	lcd_kit_parse_u32(np, "lcd-kit,mipi-hop-support",
		&disp_info->fps.hop_support, 0);

	for (i = 0; i < disp_info->fps.panel_support_fps_list.cnt; i++) {
		index = disp_info->fps.panel_support_fps_list.buf[i];
		if (index == LCD_FPS_SCENCE_60) {
			lcd_kit_parse_dcs_cmds(np, "lcd-kit,fps-to-normal-60-cmds",
				"lcd-kit,fps-to-normal-60-cmds-state",
				&disp_info->fps.fps_to_cmds[index]);
			lcd_kit_parse_array_data(np, "lcd-kit,fps-normal-60-dsi-timming",
				&disp_info->fps.fps_dsi_timming[index]);
			/* hopping */
			if (disp_info->fps.hop_support)
				lcd_kit_parse_array_data(np, "lcd-kit,fps-normal-60-mipi-hopping",
					&disp_info->fps.hop_info[index]);
		} else if (index == LCD_FPS_SCENCE_H60) {
			lcd_kit_parse_dcs_cmds(np, "lcd-kit,fps-to-high-60-cmds",
				"lcd-kit,fps-to-high-60-cmds-state",
				&disp_info->fps.fps_to_cmds[index]);
			lcd_kit_parse_array_data(np, "lcd-kit,fps-high-60-dsi-timming",
				&disp_info->fps.fps_dsi_timming[index]);
			/* hopping */
			if (disp_info->fps.hop_support)
				lcd_kit_parse_array_data(np, "lcd-kit,fps-high-60-mipi-hopping",
					&disp_info->fps.hop_info[index]);
		} else if (index == LCD_FPS_SCENCE_90) {
			lcd_kit_parse_dcs_cmds(np, "lcd-kit,fps-to-90-cmds",
				"lcd-kit,fps-to-90-cmds-state",
				&disp_info->fps.fps_to_cmds[index]);
			lcd_kit_parse_array_data(np, "lcd-kit,fps-90-dsi-timming",
				&disp_info->fps.fps_dsi_timming[index]);
			/* hopping */
			if (disp_info->fps.hop_support)
				lcd_kit_parse_array_data(np, "lcd-kit,fps-90-mipi-hopping",
					&disp_info->fps.hop_info[index]);
		} else if (index == LCD_FPS_SCENCE_120) {
			lcd_kit_parse_dcs_cmds(np, "lcd-kit,fps-to-120-cmds",
				"lcd-kit,fps-to-120-cmds-state",
				&disp_info->fps.fps_to_cmds[index]);
			lcd_kit_parse_array_data(np, "lcd-kit,fps-120-dsi-timming",
				&disp_info->fps.fps_dsi_timming[index]);
			/* hopping */
			if (disp_info->fps.hop_support)
				lcd_kit_parse_array_data(np, "lcd-kit,fps-120-mipi-hopping",
					&disp_info->fps.hop_info[index]);
		}
	}
}

void lcd_kit_parse_dsc(struct device_node *np) {
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-ver",
		&disp_info->dsc_params.ver, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-slice-mode",
		&disp_info->dsc_params.slice_mode, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rgb-swap",
		&disp_info->dsc_params.rgb_swap, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-cfg",
		&disp_info->dsc_params.dsc_cfg, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rct-on",
		&disp_info->dsc_params.rct_on, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-bit-per-channel",
		&disp_info->dsc_params.bit_per_channel, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-line-buf-depth",
		&disp_info->dsc_params.dsc_line_buf_depth, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-bp-enable",
		&disp_info->dsc_params.bp_enable, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-bit-per-pixel",
		&disp_info->dsc_params.bit_per_pixel, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-pic-height",
		&disp_info->dsc_params.pic_height, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-pic-width",
		&disp_info->dsc_params.pic_width, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-slice-height",
		&disp_info->dsc_params.slice_height, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-slice-width",
		&disp_info->dsc_params.slice_width, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-chunk-size",
		&disp_info->dsc_params.chunk_size, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-xmit-delay",
		&disp_info->dsc_params.xmit_delay, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-dec-delay",
		&disp_info->dsc_params.dec_delay, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-scale-value",
		&disp_info->dsc_params.scale_value, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-increment-interval",
		&disp_info->dsc_params.increment_interval, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-decrement-interval",
		&disp_info->dsc_params.decrement_interval, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-line-bpg-offset",
		&disp_info->dsc_params.line_bpg_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-nfl-bpg-offset",
		&disp_info->dsc_params.nfl_bpg_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-slice-bpg-offset",
		&disp_info->dsc_params.slice_bpg_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-initial-offset",
		&disp_info->dsc_params.initial_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-final-offset",
		&disp_info->dsc_params.final_offset, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-flatness-minqp",
		&disp_info->dsc_params.flatness_minqp, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-flatness-maxqp",
		&disp_info->dsc_params.flatness_maxqp, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-model-size",
		&disp_info->dsc_params.rc_model_size, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-edge-factor",
		&disp_info->dsc_params.rc_edge_factor, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-quant-incr-limit0",
		&disp_info->dsc_params.rc_quant_incr_limit0, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-quant-incr-limit1",
		&disp_info->dsc_params.rc_quant_incr_limit1, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-tgt-offset-hi",
		&disp_info->dsc_params.rc_tgt_offset_hi, 0);
	OF_PROPERTY_READ_U32_DEFAULT(np, "lcd-kit,dsc-rc-tgt-offset-lo",
		&disp_info->dsc_params.rc_tgt_offset_lo, 0);
}

void lcd_kit_parse_util(struct device_node *np)
{
	if (np == NULL) {
		LCD_KIT_ERR("np is null\n");
		return;
	}
	/* quickly sleep out */
	lcd_kit_parse_u32(np, "lcd-kit,quickly-sleep-out-support",
		&disp_info->quickly_sleep_out.support, 0);
	if (disp_info->quickly_sleep_out.support)
		lcd_kit_parse_u32(np,
			"lcd-kit,quickly-sleep-out-interval",
			&disp_info->quickly_sleep_out.interval, 0);

	/* fps */
	lcd_kit_parse_u32(np, "lcd-kit,fps-support",
		&disp_info->fps.support, 0);
	if (disp_info->fps.support)
		lcd_kit_parse_fps(np);

	/* dsc */
	lcd_kit_parse_u32(np, "lcd-kit,dsc-enable",
		&disp_info->dsc_enable, 0);
	if (disp_info->dsc_enable)
		lcd_kit_parse_dsc(np);
}

void lcd_kit_parse_dt(struct device_node *np)
{
	if (!np) {
		LCD_KIT_ERR("np is null\n");
		return;
	}
	/* parse running test */
	lcd_kit_parse_running(np);
	/* parse effect info */
	lcd_kit_parse_effect(np);
	/* parse normal function */
	lcd_kit_parse_util(np);
}

int lcd_kit_get_bl_max_nit_from_dts(void)
{
	int ret;
	struct device_node *np = NULL;

	if (common_info->blmaxnit.get_blmaxnit_type == GET_BLMAXNIT_FROM_DDIC) {
		np = of_find_compatible_node(NULL, NULL,
			DTS_COMP_LCD_KIT_PANEL_TYPE);
		if (!np) {
			LCD_KIT_ERR("NOT FOUND device node %s!\n",
				DTS_COMP_LCD_KIT_PANEL_TYPE);
			ret = -1;
			return ret;
		}
		OF_PROPERTY_READ_U32_RETURN(np, "panel_ddic_max_brightness",
			&common_info->actual_bl_max_nit);
		LCD_KIT_INFO("max nit is %d\n", common_info->actual_bl_max_nit);
	}
	return LCD_KIT_OK;
}

static int lcd_kit_get_project_id(char *buff)
{
	if (buff == NULL) {
		LCD_KIT_ERR("buff is null\n");
		return LCD_KIT_FAIL;
	}

	if (!disp_info) {
		LCD_KIT_ERR("disp_info is null\n");
		return LCD_KIT_FAIL;
	}

	/* use read project id */
	if (disp_info->project_id.support && disp_info->project_id.id &&
		(strlen(disp_info->project_id.id) > 0)) {
		strncpy(buff, disp_info->project_id.id,
			strlen(disp_info->project_id.id));
		LCD_KIT_INFO("use read project id is %s\n",
			disp_info->project_id.id);
		return LCD_KIT_OK;
	}

	/* use default project id */
	if (disp_info->project_id.support &&
		disp_info->project_id.default_project_id) {
		strncpy(buff, disp_info->project_id.default_project_id,
			PROJECTID_LEN);
		LCD_KIT_INFO("use default project id:%s\n",
			disp_info->project_id.default_project_id);
		return LCD_KIT_OK;
	}

	LCD_KIT_ERR("not support get project id\n");
	return LCD_KIT_FAIL;
}

int lcd_kit_get_online_status(void)
{
	int status = LCD_ONLINE;

	if (!strncmp(disp_info->compatible, LCD_KIT_DEFAULT_PANEL,
		strlen(disp_info->compatible)))
		/* panel is online */
		status = LCD_OFFLINE;
	LCD_KIT_INFO("status = %d\n", status);
	return status;
}

static int lcd_get_2d_barcode(char *buff)
{
	int ret;

	if (!buff) {
		LCD_KIT_ERR("buff is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!disp_info) {
		LCD_KIT_ERR("disp_info is null\n");
		return LCD_KIT_FAIL;
	}

	if (!disp_info->oeminfo.support) {
		LCD_KIT_ERR("oem info is not support\n");
		return LCD_KIT_FAIL;
	}
#ifdef CONFIG_DRM_MEDIATEK
	ret = lcd_kit_dsi_cmds_extern_rx(buff,
		&disp_info->oeminfo.barcode_2d.cmds, LCD_KIT_SN_CODE_LENGTH);
#else
	ret = lcd_kit_dsi_cmds_extern_rx_v1(buff,
		&disp_info->oeminfo.barcode_2d.cmds);
#endif
	if (ret) {
		LCD_KIT_ERR("get 2d barcode fail\n");
		return LCD_KIT_FAIL;
	} else {
		LCD_KIT_INFO("get 2d barcode success! %s\n", buff);
		return LCD_KIT_OK;
	}
}

#ifdef CONFIG_DRM_MEDIATEK
int lcd_kit_rgbw_set_mode(int mode)
{
	int ret = LCD_KIT_OK;
	static int old_rgbw_mode;
	int rgbw_mode;

	if (disp_info == NULL) {
		LCD_KIT_ERR("disp_info is null\n");
		return LCD_KIT_FAIL;
	}
	rgbw_mode = disp_info->ddic_rgbw_param.ddic_rgbw_mode;
	if (rgbw_mode != old_rgbw_mode) {
		switch (mode) {
		case RGBW_SET1_MODE:
			ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.mode1_cmds);
			break;
		case RGBW_SET2_MODE:
			ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.mode2_cmds);
			break;
		case RGBW_SET3_MODE:
			ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.mode3_cmds);
			break;
		case RGBW_SET4_MODE:
			ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.mode4_cmds);
			break;
		default:
			LCD_KIT_ERR("mode err: %d\n", disp_info->ddic_rgbw_param.ddic_rgbw_mode);
			ret = LCD_KIT_FAIL;
			break;
		}
	}
	LCD_KIT_DEBUG("rgbw_mode=%d,rgbw_mode_old=%d!\n", rgbw_mode, old_rgbw_mode);
	old_rgbw_mode = rgbw_mode;
	return ret;
}

int lcd_kit_rgbw_set_backlight(int bl_level)
{
	int ret;
	unsigned int level;

	if (bl_level < 0)
		bl_level = 0;
	level = (unsigned int)bl_level;
	/* change bl level to dsi cmds */
	disp_info->rgbw.backlight_cmds.cmds[0].payload[1] = (level >> 8) & 0xff;
	disp_info->rgbw.backlight_cmds.cmds[0].payload[2] = level & 0xff;
	ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.backlight_cmds);
	return ret;
}

static int lcd_kit_rgbw_pix_gain(void)
{
	unsigned int pix_gain;
	static unsigned int pix_gain_old;
	int rgbw_mode;
	int ret = LCD_KIT_OK;

	if (disp_info->rgbw.pixel_gain_limit_cmds.cmds == NULL) {
		LCD_KIT_INFO("RGBW not support pixel_gain_limit\n");
		return ret;
	}
	rgbw_mode = disp_info->ddic_rgbw_param.ddic_rgbw_mode;
	pix_gain = (unsigned int)disp_info->ddic_rgbw_param.pixel_gain_limit;
	if ((pix_gain != pix_gain_old) && (rgbw_mode == RGBW_SET4_MODE)) {
		disp_info->rgbw.pixel_gain_limit_cmds.cmds[0].payload[1] = pix_gain;
		ret = lcd_kit_dsi_cmds_extern_tx(&disp_info->rgbw.pixel_gain_limit_cmds);
		LCD_KIT_DEBUG("RGBW pixel_gain=%u,pix_gain_old=%u\n",
			pix_gain, pix_gain_old);
		pix_gain_old = pix_gain;
	}
	return ret;
}

int lcd_kit_rgbw_set_handle(void)
{
	int ret;
	static int old_rgbw_backlight;
	int rgbw_backlight;
	int rgbw_bl_level;
	struct mtk_panel_info *panel_info = NULL;

	if (disp_info == NULL) {
		LCD_KIT_ERR("disp_info is null\n");
		return LCD_KIT_FAIL;
	}
	panel_info = lcm_get_panel_info();
	if (panel_info == NULL) {
		LCD_KIT_ERR("panel_info is NULL\n");
		return LCD_KIT_FAIL;
	}
	/* set mode */
	ret = lcd_kit_rgbw_set_mode(disp_info->ddic_rgbw_param.ddic_rgbw_mode);
	if (ret < 0) {
		LCD_KIT_ERR("RGBW set mode fail\n");
		return LCD_KIT_FAIL;
	}

	/* set backlight */
	rgbw_backlight = disp_info->ddic_rgbw_param.ddic_rgbw_backlight;
	if (disp_info->rgbw.backlight_cmds.cmds &&
		(panel_info->bl_current != 0) &&
		(rgbw_backlight != old_rgbw_backlight)) {
		rgbw_bl_level = rgbw_backlight * disp_info->rgbw.rgbw_bl_max /
			panel_info->bl_max;
		ret = lcd_kit_rgbw_set_backlight(rgbw_bl_level);
		if (ret < 0) {
			LCD_KIT_ERR("RGBW set backlight fail\n");
			return LCD_KIT_FAIL;
		}
	}
	old_rgbw_backlight = rgbw_backlight;

	/* set gain */
	ret = lcd_kit_rgbw_pix_gain();
	if (ret) {
		LCD_KIT_INFO("RGBW set pix_gain fail\n");
		return LCD_KIT_FAIL;
	}
	return ret;
}
#endif

int lcd_kit_get_status_by_type(int type, int *status)
{
	int ret;

	if (status == NULL) {
		LCD_KIT_ERR("status is null\n");
		return LCD_KIT_FAIL;
	}
	switch (type) {
	case LCD_ONLINE_TYPE:
		*status = lcd_kit_get_online_status();
		ret = LCD_KIT_OK;
		break;
	case PT_STATION_TYPE:
#ifdef LCD_FACTORY_MODE
		*status = lcd_kit_get_pt_station_status();
#endif
		ret = LCD_KIT_OK;
		break;
	default:
		LCD_KIT_ERR("not support type\n");
		ret = LCD_KIT_FAIL;
		break;
	}
	return ret;
}

static int lcd_create_sysfs(struct kobject *obj)
{
	int rc = LCD_KIT_FAIL;

	if (obj == NULL) {
		LCD_KIT_ERR("create sysfs fail, obj is NULL\n");
		return rc;
	}

	rc = lcd_kit_create_sysfs(obj);
	if (rc) {
		LCD_KIT_ERR("create sysfs fail\n");
		return rc;
	}
#ifdef LCD_FACTORY_MODE
	rc = lcd_create_fact_sysfs(obj);
	if (rc) {
		LCD_KIT_ERR("create fact sysfs fail\n");
		return rc;
	}
#endif
	return rc;
}

static int lcd_kit_proximity_power_off(void)
{
	LCD_KIT_INFO("[Proximity_feature] lcd_kit_proximity_power_off enter!\n");
	if (!common_info->thp_proximity.support) {
		LCD_KIT_INFO("[Proximity_feature] thp_proximity not support exit!\n");
		return LCD_KIT_FAIL;
	}
	if (lcd_kit_get_pt_mode()) {
		LCD_KIT_INFO("[Proximity_feature] pt test mode exit!\n");
		return LCD_KIT_FAIL;
	}
	down(&disp_info->thp_second_poweroff_sem);
	if (common_info->thp_proximity.panel_power_state == POWER_ON) {
		LCD_KIT_INFO("[Proximity_feature] power state is on exit!\n");
		up(&disp_info->thp_second_poweroff_sem);
		return LCD_KIT_FAIL;
	}
	if (common_info->thp_proximity.panel_power_state == POWER_TS_SUSPEND) {
		LCD_KIT_INFO("[Proximity_feature] power off suspend state exit!\n");
		up(&disp_info->thp_second_poweroff_sem);
		return LCD_KIT_OK;
	}
	if (common_info->thp_proximity.work_status == TP_PROXMITY_DISABLE) {
		LCD_KIT_INFO("[Proximity_feature] thp_proximity has been disabled exit!\n");
		up(&disp_info->thp_second_poweroff_sem);
		return LCD_KIT_FAIL;
	}
	common_info->thp_proximity.work_status = TP_PROXMITY_DISABLE;
	if (common_ops->panel_only_power_off)
		common_ops->panel_only_power_off(NULL);
	up(&disp_info->thp_second_poweroff_sem);
	LCD_KIT_INFO("[Proximity_feature] lcd_kit_proximity_power_off exit!\n");
	return LCD_KIT_OK;
}

static int lcd_kit_get_sn_code(void)
{
	int ret;
	struct mtk_panel_info *pinfo = NULL;
	struct lcd_kit_panel_ops *panel_ops = NULL;
	char read_value[OEM_INFO_SIZE_MAX + 1] = {0};

#ifdef CONFIG_DRM_MEDIATEK
	pinfo = lcm_get_panel_info();
#else
	pinfo = (struct mtk_panel_info *)lcdkit_mtk_common_panel.panel_info;
#endif
	if (pinfo == NULL) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	if (common_info && common_info->sn_code.support) {
		panel_ops = lcd_kit_panel_get_ops();
		if (panel_ops && panel_ops->lcd_get_2d_barcode) {
			ret = panel_ops->lcd_get_2d_barcode(read_value);
			if (ret != 0) {
				LCD_KIT_ERR("get sn_code error!\n");
				return LCD_KIT_FAIL;
			}
			memcpy(pinfo->sn_code, read_value + 2, LCD_KIT_SN_CODE_LENGTH);
			pinfo->sn_code_length = LCD_KIT_SN_CODE_LENGTH;
			return LCD_KIT_OK;
		}
		if (disp_info && disp_info->oeminfo.barcode_2d.support) {
#ifdef CONFIG_DRM_MEDIATEK
			ret = lcd_kit_dsi_cmds_extern_rx(read_value,
				&disp_info->oeminfo.barcode_2d.cmds,
				LCD_KIT_SN_CODE_LENGTH);
#else
			ret = lcd_kit_dsi_cmds_extern_rx_v1(read_value,
				&disp_info->oeminfo.barcode_2d.cmds);
#endif
			if (ret != 0) {
				LCD_KIT_ERR("get sn_code error!\n");
				return LCD_KIT_FAIL;
			}
			memcpy(pinfo->sn_code, read_value, LCD_KIT_SN_CODE_LENGTH);
			pinfo->sn_code_length = LCD_KIT_SN_CODE_LENGTH;
			return LCD_KIT_OK;
		}
	}
	return LCD_KIT_OK;
}

struct lcd_kit_ops g_lcd_ops = {
	.lcd_kit_support = lcd_kit_support,
	.get_project_id = lcd_kit_get_project_id,
	.create_sysfs = lcd_create_sysfs,
	.read_project_id = lcd_kit_read_project_id,
	.get_2d_barcode = lcd_get_2d_barcode,
	.get_status_by_type = lcd_kit_get_status_by_type,
	.proximity_power_off = lcd_kit_proximity_power_off,
#ifdef LCD_FACTORY_MODE
	.get_pt_station_status = lcd_kit_get_pt_station_status,
#endif
	.get_sn_code = lcd_kit_get_sn_code,
};

int lcd_kit_utils_init(struct device_node *np, struct mtk_panel_info *pinfo)
{
	/* init sem */
	sema_init(&disp_info->lcd_kit_sem, 1);
	sema_init(&disp_info->thp_second_poweroff_sem, 1);
	/* init mipi lock */
	mutex_init(&disp_info->mipi_lock);
	/* parse display dts */
	lcd_kit_parse_dt(np);
#ifdef LCD_FACTORY_MODE
	lcd_kit_fact_init(np);
#endif
	/*init hisi pinfo*/
	lcd_kit_pinfo_init(np, pinfo);
	lcd_kit_ops_register(&g_lcd_ops);
	return LCD_KIT_OK;
}
