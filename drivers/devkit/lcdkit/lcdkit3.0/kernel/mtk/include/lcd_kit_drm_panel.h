/*
 * lcd_kit_drm_panel.h
 *
 * lcdkit display function head file for lcd driver
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

#ifndef __LCD_KIT_DRM_PANEL_H_
#define __LCD_KIT_DRM_PANEL_H_
#include <linux/backlight.h>
#include <drm/drmP.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>
#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>
#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include "lcd_kit_disp.h"
#include "lcd_kit_common.h"
#include "lcd_kit_power.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_core.h"
#include "bias_bl_utils.h"

enum LCM_DRM_DSI_MODE {
	DSI_CMD_MODE = 0,
	DSI_SYNC_PULSE_VDO_MODE = 1,
	DSI_SYNC_EVENT_VDO_MODE = 2,
	DSI_BURST_VDO_MODE = 3
};

enum LCM_DSI_PIXEL_FORMAT {
	LCM_PIXEL_16BIT_RGB565 = 0,
	LCM_LOOSELY_PIXEL_18BIT_RGB666 = 1,
	LCM_PIXEL_24BIT_RGB888 = 2,
	LCM_PACKED_PIXEL_18BIT_RGB666 = 3
};

struct lcm {
	struct device *dev;
	struct drm_panel panel;
	struct backlight_device *backlight;
	struct gpio_desc *reset_gpio;

	bool prepared;
	bool enabled;

	int error;
};

struct mtk_panel_info *lcm_get_panel_info(void);
void  lcd_kit_bl_ic_set_backlight(unsigned int bl_level);
int lcd_kit_init(void);
void lcd_kit_disp_on_check_delay(void);
int lcm_rgbw_mode_set_param(struct drm_device *dev, void *data,
	struct drm_file *file_priv);
int lcm_rgbw_mode_get_param(struct drm_device *dev, void *data,
	struct drm_file *file_priv);
int lcm_display_engine_get_panel_info(struct drm_device *dev, void *data,
	struct drm_file *file_priv);
int lcm_display_engine_init(struct drm_device *dev, void *data,
	struct drm_file *file_priv);
#endif
