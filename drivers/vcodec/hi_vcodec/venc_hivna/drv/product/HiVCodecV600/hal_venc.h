/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: venc register config
 * Create: 2019-08-30
 */

#ifndef __HAL_VENC_H__
#define __HAL_VENC_H__

#include "hi_type.h"
#include "drv_venc_ioctl.h"
#include "smmu.h"
#include "hal_cmdlist.h"
#include "hal_common.h"

#define VENC_COMPATIBLE       "hisi,HiVCodecV600-venc"
#define VENC_ES_SUPPORT
#define VENC_FPGAFLAG_ES      "fpga_es"
#define VENC_PCTRL_PERI_ES    0xFEC3E0BC
#define VENC_EXISTBIT_ES      0x38

#define VENC_CS_SUPPORT
#define VENC_FPGAFLAG_CS      "fpga_cs"
#define VENC_PCTRL_PERI_CS    0xFEC3E0BC
#define VENC_EXISTBIT_CS      0x38
#define VENC_EXIST_TRUE       0x8

HI_VOID venc_hal_clr_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg);
HI_VOID venc_hal_start_encode(S_HEVC_AVC_REGS_TYPE *vedu_reg);
HI_VOID venc_hal_get_reg_venc(struct stream_info *stream_info, HI_U32 *reg_base);
HI_VOID venc_hal_get_reg_stream_len(struct stream_info *stream_info, HI_U32 *reg_base);
HI_VOID venc_hal_cfg_curld_osd01(struct encode_info *channel_cfg, HI_U32 *reg_base);
HI_VOID venc_hal_disable_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg);
HI_VOID venc_hal_get_slice_reg(struct stream_info *stream_info, HI_U32 *reg_base);

#endif
