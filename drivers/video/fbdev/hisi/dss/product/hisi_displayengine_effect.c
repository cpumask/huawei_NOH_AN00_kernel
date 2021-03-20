/* Copyright (c) 2020-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "product/hisi_displayengine_effect.h"
#include <linux/hisi/hw_cmdline_parse.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

extern uint8_t* share_mem_virt;
extern phys_addr_t share_mem_phy;

static display_engine_param_t de_param_for_panels[DISPLAY_ENGINE_PANEL_NUM];

/* Common tool functions */
static inline struct hisi_fb_data_type *display_engine_check_input_and_get_hisifd(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;

	if (NULL == info) {
		HISI_FB_ERR("[effect] info is NULL\n");
		return NULL;
	}

	if (NULL == argp) {
		HISI_FB_ERR("[effect] argp is NULL\n");
		return NULL;
	}

	hisifd = (struct hisi_fb_data_type *)info->par;
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return NULL;
	}

	return hisifd;
}

struct hisi_fb_data_type *display_engine_check_param(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = display_engine_check_input_and_get_hisifd(info, argp);
	if (NULL == hisifd) {
		return NULL;
	}

	if (!hisifd->de_info.is_ready) {
		HISI_FB_ERR("[effect] display engine has not been initialized!\n");
		return NULL;
	}

	return hisifd;
}

static int display_engine_param_get_begin(struct fb_info *info, void __user *argp, struct hisi_fb_data_type **hisifd,
	display_engine_param_t **de_param, size_t *size)
{
	int ret = 0;

	if (info == NULL || argp == NULL || hisifd == NULL || de_param == NULL || size == NULL) {
		HISI_FB_ERR("[effect] Invalid input: some pointer is null!\n");
		return -EINVAL;
	}

	*hisifd = display_engine_check_param(info, argp);
	if (*hisifd == NULL) {
		HISI_FB_ERR("[effect] display_engine_check_param return error!\n");
		return -EINVAL;
	}

	if ((*hisifd)->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE)
		*size = sizeof(display_engine_param_t) * DISPLAY_ENGINE_PANEL_NUM;
	else
		*size = sizeof(display_engine_param_t);
	*de_param = de_param_for_panels;

	ret = (int)copy_from_user(*de_param, argp, *size);
	if (ret) {
		HISI_FB_ERR("[effect] copy_from_user(param,argp,%zu) failed! ret=%d.\n", *size, ret);
		return ret;
	}
	return 0;
}

static int displaye_engine_param_get_end(void __user *argp, display_engine_param_t *de_param, size_t size)
{
	if (argp == NULL || de_param == NULL || size == 0) {
		HISI_FB_ERR("[effect] Invalid input: argp == NULL || de_param == NULL || size == %zu!\n", size);
		return -EINVAL;
	}

	int ret = (int)copy_to_user(argp, de_param, size);
	if (ret)
		HISI_FB_ERR("[effect] copy_to_user(argp,param,%zu) failed! ret=%d.\n", size, ret);
	return ret;
}

/* Specific getting functions for features or modules */
static int display_engine_blc_param_get(struct hisi_fb_data_type *hisifd, display_engine_blc_param_t *param)
{
	if (NULL == hisifd) {
		HISI_FB_ERR("[effect] hisifd is NULL\n");
		return -EINVAL;
	}

	if (NULL == param) {
		HISI_FB_ERR("[effect] param is NULL\n");
		return -EINVAL;
	}

	param->enable = hisifd->de_info.blc_enable ? 1 : 0;
	param->delta = hisifd->de_info.blc_delta;
	return 0;
}

int display_engine_color_rectify_param_get(struct hisi_fb_data_type *hisifd, display_engine_color_rectify_param_t *param)
{
	int ret = 0;
	struct hisi_fb_panel_data *pdata = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	if (hisifd->pdev == NULL) {
		HISI_FB_ERR("[effect] pdev is NULL Pointer\n");
		return -1;
	}

	memset(&(hisifd->de_info.lcd_color_oeminfo), 0, sizeof(struct disp_lcdbrightnesscoloroeminfo));

	pdata = dev_get_platdata(&hisifd->pdev->dev);
	if (NULL == pdata) {
		HISI_FB_ERR("[effect] pdata is NULL Pointer\n");
		return -1;
	}

	down(&hisifd->blank_sem);

	if (!hisifd->panel_power_on) {
		HISI_FB_DEBUG("fb%d, panel power off!\n", hisifd->index);
		ret = -1;
		goto err_out;
	}

	if (pdata->lcd_color_param_get_func != NULL) {
		hisifb_activate_vsync(hisifd);
		pdata->lcd_color_param_get_func(hisifd);
		hisifb_deactivate_vsync(hisifd);
	}
	param->lcd_color_oeminfo = hisifd->de_info.lcd_color_oeminfo;
	HISI_FB_DEBUG("[effect] display_engine_color_rectify_param_get params\n");
err_out:
	up(&hisifd->blank_sem);
	return ret;
}

int display_engine_share_mem_get(struct hisi_fb_data_type *hisifd, display_engine_share_memory_t *param)
{
	if (NULL == param) {
		HISI_FB_ERR("[shmmap] params is null\n");
		return -1;
	}
	param->addr_virt = (uint64_t)(uintptr_t)share_mem_virt;
	param->addr_phy = (uint64_t)(uintptr_t)share_mem_phy;
	return 0;
}

static void display_engine_foldable_print(display_engine_foldable_info_t param)
{
	HISI_FB_INFO("[effect] get dbv primary:%u slave:%u folding:%u\n",
		param.dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
	HISI_FB_INFO("[effect] get origin dbv primary:%u slave:%u folding:%u\n",
		param.original_dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.original_dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.original_dbv_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
	HISI_FB_INFO("[effect] get screen on duration primary:%u slave:%u folding:%u\n",
		param.screen_on_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.screen_on_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.screen_on_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
	HISI_FB_INFO("[effect] get screen on duration(hiace on) primary:%u slave:%u folding:%u\n",
		param.screen_on_duration_with_hiace_enable[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.screen_on_duration_with_hiace_enable[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.screen_on_duration_with_hiace_enable[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
	HISI_FB_INFO("[effect] get hbm acc duration primary:%u slave:%u folding:%u \n",
		param.hbm_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.hbm_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.hbm_acc[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
	HISI_FB_INFO("[effect] get hbm duration:%u slave:%u folding:%u \n",
		param.hbm_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_PRIMARY],
		param.hbm_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_SLAVE],
		param.hbm_duration[DISPLAY_ENGINE_FOLDABLE_PANEL_FOLDING]);
}

static int display_engine_foldable_info_get(struct hisi_fb_data_type *hisifd, display_engine_foldable_info_t *param) {
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

#ifdef CONFIG_LCD_KIT_DRIVER
	struct display_engine_duration duration_param;
	duration_param.dbv_acc = param->dbv_acc;
	duration_param.size_dbv_acc = sizeof(param->dbv_acc);
	duration_param.original_dbv_acc = param->original_dbv_acc;
	duration_param.size_original_dbv_acc =
		sizeof(param->original_dbv_acc);
	duration_param.screen_on_duration = param->screen_on_duration;
	duration_param.size_screen_on_duration = sizeof(param->screen_on_duration);
	duration_param.hbm_acc = param->hbm_acc;
	duration_param.size_hbm_acc = sizeof(param->hbm_acc);
	duration_param.hbm_duration = param->hbm_duration;
	duration_param.size_hbm_duration = sizeof(param->hbm_duration);

	int ret = lcd_get_dbv_stat(hisifd, duration_param);

	if (ret != 0) {
		HISI_FB_ERR("[effect] lcd_get_dbv_stat() failed:%d\n", ret);
		return -1;
	}
#endif
	hisifb_panel_get_hiace_display_time(hisifd, param->screen_on_duration_with_hiace_enable);
	display_engine_foldable_print(*param);
	return 0;
}

int display_engine_ddic_cabc_param_get(struct hisi_fb_data_type *hisifd, display_engine_ddic_cabc_param_t *param)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}
	param->ddic_cabc_mode = hisifd->de_info.ddic_cabc_mode;
	return 0;
}

int display_engine_ddic_color_param_get(struct hisi_fb_data_type *hisifd, display_engine_ddic_color_param_t *param)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	param->ddic_color_mode = hisifd->de_info.ddic_color_mode;

	return 0;
}

int display_engine_ddic_rgbw_param_get(struct hisi_fb_data_type *hisifd, display_engine_ddic_rgbw_param_t *param)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	param->ddic_panel_id = hisifd->de_info.ddic_panel_id;
	param->ddic_rgbw_mode = hisifd->de_info.ddic_rgbw_mode;
	param->ddic_rgbw_backlight = hisifd->de_info.ddic_rgbw_backlight;
	param->rgbw_saturation_control = hisifd->de_info.rgbw_saturation_control;
	param->frame_gain_limit = hisifd->de_info.frame_gain_limit;
	param->frame_gain_speed = hisifd->de_info.frame_gain_speed;
	param->color_distortion_allowance = hisifd->de_info.color_distortion_allowance;
	param->pixel_gain_limit = hisifd->de_info.pixel_gain_limit;
	param->pixel_gain_speed = hisifd->de_info.pixel_gain_speed;
	param->pwm_duty_gain = hisifd->de_info.pwm_duty_gain;
	HISI_FB_DEBUG("[effect] display_engine_ddic_rgbw_param_get params\n");
	return 0;
}

static void display_engine_panel_info_get_from_pinfo(display_engine_panel_info_param_t *param,
	struct hisi_panel_info *pinfo)
{
	if (param == NULL || pinfo == NULL) {
		HISI_FB_ERR("[effect] Invalid input: param == NULL || pinfo == NULL!\n");
		return;
	}
	param->width = pinfo->xres;
	param->height = pinfo->yres;
	param->maxluminance = pinfo->hiace_param.iMaxLcdLuminance;
	param->minluminance = pinfo->hiace_param.iMinLcdLuminance;
	param->maxbacklight = pinfo->bl_max;
	param->minbacklight = pinfo->bl_min;
	if (pinfo->panel_name != NULL) {
		strncpy(param->panel_name, pinfo->panel_name, PANEL_NAME_LENGTH - 1);
		param->panel_name[PANEL_NAME_LENGTH - 1] = '\0';
	} else {
		HISI_FB_WARNING("[effect] pinfo->panel_name is NULL Pointer\n");
	}
	memcpy(param->lcd_panel_version, pinfo->lcd_panel_version, sizeof(pinfo->lcd_panel_version));
	param->factory_runmode = runmode_is_factory();
	param->board_version = pinfo->board_version; //version id related with real hardware boardid
	param->sn_code_length = pinfo->sn_code_length;
	if (pinfo->sn_code_length > 0 && pinfo->sn_code_length <= SN_CODE_LENGTH_MAX)
		memcpy(param->sn_code, pinfo->sn_code, pinfo->sn_code_length);

	if (pinfo->gamma_lut_table_len && g_factory_gamma_enable) {
		uint32_t i = 0;
		uint16_t *u16_gm_r = param->factory_gamma;
		uint16_t *u16_gm_g = u16_gm_r + pinfo->gamma_lut_table_len;
		uint16_t *u16_gm_b = u16_gm_g + pinfo->gamma_lut_table_len;

		for (i = 0; i < pinfo->gamma_lut_table_len; i++) {
			u16_gm_r[i] = (uint16_t)pinfo->gamma_lut_table_R[i];
			u16_gm_g[i] = (uint16_t)pinfo->gamma_lut_table_G[i];
			u16_gm_b[i] = (uint16_t)pinfo->gamma_lut_table_B[i];
		}
		param->factory_gamma_enable = g_factory_gamma_enable;
	} else {
		param->factory_gamma_enable = 0;
	}
}

static int display_engine_panel_info_get_with_id(struct hisi_fb_data_type *hisifd,
	display_engine_panel_info_param_t *param, uint32_t panel_id)
{
	struct hisi_panel_info *pinfo = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	get_lcd_panel_info(hisifd, &pinfo, panel_id);
	display_engine_panel_info_get_from_pinfo(param, pinfo);
	return 0;
}

static int display_engine_panel_info_get(struct hisi_fb_data_type *hisifd, display_engine_panel_info_param_t *param)
{
	struct hisi_panel_info *pinfo = NULL;

	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	if (param == NULL) {
		HISI_FB_ERR("[effect] param is NULL Pointer\n");
		return -1;
	}

	pinfo = &(hisifd->panel_info);
	display_engine_panel_info_get_from_pinfo(param, pinfo);
	return 0;
}

static int display_engine_common_panel_info_get(struct hisi_fb_data_type *hisifd,
	display_engine_common_panel_info_param_t *param)
{
	if (hisifd == NULL) {
		HISI_FB_ERR("[effect] hisifd is NULL Pointer\n");
		return -1;
	}

	param->current_panel_id = hisifd->panel_info.disp_panel_id;
	HISI_FB_INFO("[effect] current_panel_id:%d\n", param->current_panel_id);
	return 0;
}

/* Combined functions for features or modules classification */
static int display_engine_param_get_default(struct hisi_fb_data_type *hisifd, display_engine_param_t *de_param)
{
	int ret = 0;

	if (de_param->modules & DISPLAY_ENGINE_BLC) {
		ret = display_engine_blc_param_get(hisifd, &de_param->blc);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get BLC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_COLOR_RECTIFY) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_COLOR_RECTIFY.\n");
		ret = display_engine_color_rectify_param_get(hisifd, &de_param->color_param);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get color param, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_SHAREMEM) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_SHAREMEM.\n");
		ret = display_engine_share_mem_get(hisifd, &de_param->share_mem);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get share memory param, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_FOLDABLE_INFO) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_FOLDABLE_INFO\n");
		ret = display_engine_foldable_info_get(hisifd, &de_param->foldable_info);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get foldable info, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_COMMON_PANL_INFO) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_COMMON_PANL_INFO\n");
		ret = display_engine_common_panel_info_get(hisifd, &de_param->common_panel_info);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get common panel info, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

ERR_OUT:
	return ret;
}

static int display_engine_param_get_ddic(struct hisi_fb_data_type *hisifd, display_engine_param_t *de_param)
{
	int ret = 0;

	if (de_param->modules & DISPLAY_ENGINE_DDIC_CABC) {
		ret = display_engine_ddic_cabc_param_get(hisifd, &de_param->ddic_cabc);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get BLC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_DDIC_COLOR) {
		ret = display_engine_ddic_color_param_get(hisifd, &de_param->ddic_color);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get BLC, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

	if (de_param->modules & DISPLAY_ENGINE_DDIC_RGBW) {
		ret = display_engine_ddic_rgbw_param_get(hisifd, &de_param->ddic_rgbw);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get rgbw, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

ERR_OUT:
	return ret;
}

static int display_engine_param_get_with_id(struct hisi_fb_data_type *hisifd, display_engine_param_t *de_param,
	uint32_t panel_id)
{
	int ret = 0;

	if (de_param->modules & DISPLAY_ENGINE_PANEL_INFO) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_PANEL_INFO id=%d.\n", panel_id);
		ret = display_engine_panel_info_get_with_id(hisifd, &de_param->panel_info, panel_id);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get panel info id=%d, ret=%d\n", panel_id, ret);
			goto ERR_OUT;
		}
		HISI_FB_DEBUG("[effect] id=%d panel_name:%s res(x:%d, y:%d) lum(min:%d, max:%d) bl(min:%d, max:%d)\n", panel_id,
			de_param->panel_info.panel_name,
			de_param->panel_info.width,
			de_param->panel_info.height,
			de_param->panel_info.minluminance,
			de_param->panel_info.maxluminance,
			de_param->panel_info.minbacklight,
			de_param->panel_info.maxbacklight);
	}

ERR_OUT:
	return ret;
}

static int display_engine_param_get_without_id(struct hisi_fb_data_type *hisifd, display_engine_param_t *de_param)
{
	int ret = 0;

	if (de_param->modules & DISPLAY_ENGINE_PANEL_INFO) {
		HISI_FB_INFO("[effect] DISPLAY_ENGINE_PANEL_INFO.\n");
		ret = display_engine_panel_info_get(hisifd, &de_param->panel_info);
		if (ret) {
			HISI_FB_ERR("[effect] failed to get panel info, ret=%d\n", ret);
			goto ERR_OUT;
		}
	}

ERR_OUT:
	return ret;
}

/* External funcions for kernel driver's calling */
int display_engine_param_get(struct fb_info *info, void __user *argp)
{
	struct hisi_fb_data_type *hisifd = NULL;
	display_engine_param_t *de_param = NULL;
	size_t size = 0;
	int ret = display_engine_param_get_begin(info, argp, &hisifd, &de_param, &size);
	if (ret)
		goto ERR_OUT;

	ret = display_engine_param_get_default(hisifd, de_param);
	if (ret)
		goto ERR_OUT;
	ret = display_engine_param_get_ddic(hisifd, de_param);
	if (ret)
		goto ERR_OUT;

	if (hisifd->panel_info.product_type & PANEL_SUPPORT_TWO_PANEL_DISPLAY_TYPE) {
		ret = display_engine_param_get_with_id(hisifd, &de_param[DISPLAY_ENGINE_PANEL_MAIN], DISPLAY_OUTER_PANEL_ID);
		if (ret)
			goto ERR_OUT;
		ret = display_engine_param_get_with_id(hisifd, &de_param[DISPLAY_ENGINE_PANEL_FULL], DISPLAY_INNEL_PANEL_ID);
		if (ret)
			goto ERR_OUT;
	} else {
		ret = display_engine_param_get_without_id(hisifd, de_param);
		if (ret)
			goto ERR_OUT;
	}

	ret = displaye_engine_param_get_end(argp, de_param, size);

ERR_OUT:
	return ret;
}

#pragma GCC diagnostic pop
