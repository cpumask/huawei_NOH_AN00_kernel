/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: venc dpm
 * Create: 2020/3/28
 */

#ifndef __DRV_VENC_DPM_H__
#define __DRV_VENC_DPM_H__

#include "venc_regulator.h"

void venc_dpm_init(struct venc_config *venc_config);
void venc_dpm_deinit(void);
void venc_dpm_freq_select(venc_clk_t clk_type);

#endif

