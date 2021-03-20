/*
 * hal_common.h
 *
 * common config
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

#ifndef HAL_COMMON_H
#define HAL_COMMON_H

#include "hi_type.h"
#include "drv_venc_ioctl.h"

#define LCU_SIZE_H265 64
#define LCU_SIZE_H264 16
#define BASE_BLOCK_SIZE 8
#define DIST_PROTOCOL(protocol, value1, value2) ((protocol == VEDU_H265) ? (value1) : (value2))
#define VENC_MASK 0x000000cf
#define CMDLIST_MASK 0x00000602

void vedu_hal_request_bus_idle(HI_U32 *reg_base);
void vedu_hal_cfg_reg(struct encode_info *channel_cfg, HI_U32 core_id);
void vedu_hal_cfg_reg_simple(struct encode_info *channel_cfg, HI_U32 core_id);

#endif
