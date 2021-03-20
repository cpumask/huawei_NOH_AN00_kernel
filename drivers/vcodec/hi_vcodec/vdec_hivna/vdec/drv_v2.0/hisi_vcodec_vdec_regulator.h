/*
 * hisi_vcodec_vdec_refulator.h
 *
 * This is for vdec regulator
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef HISI_VCODEC_VDEC_REGULATOR_H
#define HISI_VCODEC_VDEC_REGULATOR_H

#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include "hi_type.h"
#include "vfmw_ext.h"

enum {
	MEDIA_REGULATOR,
	VDEC_REGULATOR,
	SMMU_TCU_REGULATOR,
	MAX_REGULATOR,
};

enum {
	CLK_LEVEL_FHD_60FPS,
	CLK_LEVEL_UHD_30FPS,
	CLK_LEVEL_UHD_60FPS,
	CLK_LEVEL_EXTREME,
	CLK_LEVEL_MAX,
};

typedef enum {
	VDEC_CLK_RATE_LOWER = 0,
	VDEC_CLK_RATE_LOW,
	VDEC_CLK_RATE_NORMAL,
	VDEC_CLK_RATE_HIGH,
	VDEC_CLK_RATE_HEIF,
	VDEC_CLK_RATE_BUTT,
} clk_rate_e;

typedef struct {
	struct clk *clk_vdec;
	struct regulator *regulators[MAX_REGULATOR];
	hi_u32 clk_values[CLK_LEVEL_MAX];
	hi_u32 transi_clk_rate;
	hi_u32 default_clk_rate;
} vdec_regulator;

typedef struct {
	struct mutex vdec_plat_mutex;
	vdec_regulator regulator_info;
	vdec_dts dts_info;
	hi_s32 power_flag;
	hi_s32 plt_init;
	clk_rate_e resume_clk;
	struct device *dev;
} vdec_plat;

vdec_plat *vdec_plat_get_entry(void);

hi_s32 vdec_plat_init(struct platform_device *plt_dev);
hi_s32 vdec_plat_deinit(void);

hi_s32 vdec_plat_regulator_enable(void);
void vdec_plat_regulator_disable(void);

void vdec_plat_regulator_get_clk_rate(clk_rate_e *clk_rate);
hi_s32 vdec_plat_regulator_set_clk_rate(clk_rate_e clk_level);

#endif

