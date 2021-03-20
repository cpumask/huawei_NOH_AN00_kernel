/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __AT_MBB_COMMON_H__
#define __AT_MBB_COMMON_H__

#include "vos.h"
#include "AtParse.h"
#include "AtSetParaCmd.h"
#include "taf_api.h"
#include "AtCtx.h"
#include "taf_app_mma.h"
#include "taf_iface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)

#define TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT 10
#define TAF_MIN_LTE_APN_IMSI_PREFIX_SUPPORT 5

#define LTE_PROFILE_PDP_TYPE 2
#define LTE_PROFILE_IMSIPREFIX 3

VOS_UINT32 AT_AbortCcwaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_AbortClipPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_AbortClirPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_AbortCmgsPara(VOS_UINT8 indexNum);
#endif

VOS_VOID AT_ReadMbbCustomizeNv(VOS_VOID);
VOS_VOID AT_GetMbbUsrDialPdpType(TAF_IFACE_DialParam *usrDialParam);

#if (FEATURE_MBB_CUST == FEATURE_ON)
VOS_UINT32 AT_SetCustProfilePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestCustProfilePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCustProfileAttachSwitch(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCustProfileAttachSwitch(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvTafPsEvtSetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCountCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSuccAttachProfileIndexInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __AT_MBB_COMMON_H__ */

