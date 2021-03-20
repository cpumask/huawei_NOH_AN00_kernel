/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: smmu description
 * Author: zhangjianshun   zhangjianshun@huawei.com
 * Create: 2017-03-27
 */

#ifndef __HIVDEC_SMMU_H__
#define __HIVDEC_SMMU_H__

#include "hi_type.h"
#include "drv_venc_ioctl.h"

#define SMMU_OK     0
#define SMMU_ERR    -1

#define SECURE_ON    1
#define SECURE_OFF   0
#define SMMU_ON      1
#define SMMU_OFF     0

HI_S32 venc_smmu_init(HI_BOOL is_protected, HI_S32 core_id);
HI_S32 venc_smmu_cfg(struct encode_info *channelcfg, HI_U32 *reg_base);
HI_S32 venc_smmu_init_tbu(HI_S32 core_id);
HI_VOID venc_smmu_deinit(HI_VOID);
HI_VOID venc_smmu_debug(HI_U32 *reg_base, HI_BOOL first_cfg_flag);

#endif
