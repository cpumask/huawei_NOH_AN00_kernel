/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __FC_ACORE_H__
#define __FC_ACORE_H__

#include "vos.h"
#include "fc.h"
#include "acore_nv_stru_gucttf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define FC_ACORE_GetEnableMask(val, mask) ((val & (0x01 << mask)) >> mask)

enum FC_ACORE_EnableMask {
    FC_ACORE_MEM_ENABLE_MASK = 0,
    FC_ACORE_CPUA_ENABLE_MASK,
    FC_ACORE_CDS_ENABLE_MASK,
    FC_ACORE_CST_ENABLE_MASK,
    FC_ACORE_ENABLE_MASK_BUTT
};
typedef VOS_UINT8 FC_ACORE_EnableMaskUint8;

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    FC_MntnEventTypeUint16     msgName; /* _H2ASN_Skip */
    VOS_UINT8                  rsv[2];
    FC_DrvAssemPara            drvAssemPara;
} FC_MntnDrvAssemPara;

typedef struct {
    VOS_UINT32          lev;
    VOS_UINT8           rsv[4];
    FC_CpuDrvAssemPara *cpuDrvAssemPara;
} FC_CpuDrvAssemParaLev;

typedef struct {
    VOS_UINT8                   smoothCntUp;
    VOS_UINT8                   smoothCntDown;
    VOS_UINT8                   setDrvFailCnt;
    VOS_UINT8                   rsv;
    VOS_UINT32                  curLev;
    VOS_UINT8                   rsv1[4];
    FC_CpuDrvAssemPara          curAssemPara;
    FC_AcoreDrvAssembleParaFunc drvSetAssemParaFuncUe; /* _H2ASN_Replace VOS_UINT32  pDrvSetAssemParaFuncUe; */
    FC_AcoreDrvAssembleParaFunc drvSetAssemParaFuncPc; /* _H2ASN_Replace VOS_UINT32  pDrvSetAssemParaFuncPc; */
} FC_CpuDrvAssemParaEntity;

typedef struct {
    VOS_UINT8          enableMask;
    VOS_UINT8          smoothCntUpLev;
    VOS_UINT8          smoothCntDownLev;
    VOS_UINT8          rsv;
    FC_CpuDrvAssemPara cpuDrvAssemPara[FC_ACPU_DRV_ASSEM_LEV_BUTT];
} FC_CpuDrvAssemConfigPara;

#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
