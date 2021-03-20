/*
 * hisi_vcodec_vdec_dpm.h
 *
 * This is for dpm vdec clk enable
 *
 * Copyright (c) 2009-2020 Huawei Technologies CO., Ltd.
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

#ifndef HISI_VCODEC_VDEC_DPM_H
#define HISI_VCODEC_VDEC_DPM_H

#include "hisi_vcodec_vdec_regulator.h"
#include "hi_type.h"

void vdec_dpm_init(void);
void vdec_dpm_deinit(void);
void vdec_dpm_freq_select(clk_rate_e clk_type);

#endif
