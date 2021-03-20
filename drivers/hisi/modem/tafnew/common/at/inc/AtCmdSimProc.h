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

#ifndef __ATCMDSIMPROC_H__
#define __ATCMDSIMPROC_H__

#include "si_app_emat.h"
#include "AtCtx.h"
#include "AtParse.h"
#include "ATCmdProc.h"
#include "si_app_pih.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_SIM_FILE_PATH_MASK 0xFF00
#define AT_SIM_FILE_PATH_LEN_2 2
#define AT_SIM_FILE_PATH_LEN_3 3
#define AT_SIM_FILE_PATH_INDEX_1 1
#define AT_SIM_FILE_PATH_INDEX_2 2


enum AT_CARDAPP {
    AT_PREFER_APP_CDMA = 0x00000000,
    AT_PREFER_APP_GUTL = 0x00000001,
    AT_PREFER_APP_BUTT = 0x00000002,
};
typedef TAF_UINT32 AT_CARDAPP_Uint32;


typedef struct {
    VOS_UINT32 bitCardNum : 3;
    VOS_UINT32 bitReserved0 : 5;
    VOS_UINT32 bitCard0 : 3;
    VOS_UINT32 bitCard1 : 3;
    VOS_UINT32 bitCard2 : 3;
    VOS_UINT32 bitReserved1 : 15;
} TAF_NvSciCfg;

extern VOS_UINT32 At_SetHvsstPara(VOS_UINT8 indexNum);
extern VOS_UINT32 At_QryHvsstPara(VOS_UINT8 indexNum);

extern VOS_UINT32 At_HvsstQueryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_SetSciChgPara(VOS_UINT8 indexNum);
extern VOS_UINT32 At_QrySciChgPara(VOS_UINT8 indexNum);

extern VOS_UINT32 At_SciCfgQueryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_SetBwtPara(VOS_UINT8 indexNum);

VOS_UINT32 At_SetPassThrough(VOS_UINT8 indexNum);

VOS_UINT32 At_SetPassThroughCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_QryPassThrough(VOS_UINT8 indexNum);

VOS_UINT32 At_QryPassThroughCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
VOS_UINT32 At_SetSingleModemDualSlot(VOS_UINT8 indexNum);

VOS_UINT32 At_QrySingleModemDualSlot(VOS_UINT8 indexNum);

VOS_UINT32 At_SetSingleModemDualSlotCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_QrySingleModemDualSlotCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);
#endif

extern VOS_UINT32 At_ProcPihFndBndCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihGenericAccessCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihIsdbAccessCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCchoSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCchpSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_ProcPihPrivateCchoSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_ProcPihPrivateCchpSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCchcSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihSciCfgSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihBwtSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihHvsstSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCglaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCardAtrQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCardTypeQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCardTypeExQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCardVoltageQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihPrivateCglaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCrsmSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCrlaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihSessionQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCimiQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 At_ProcPihCcimiQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

extern VOS_UINT32 AT_UiccAuthCnf(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);
extern VOS_UINT32 AT_UiccAccessFileCnf(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);
extern VOS_UINT32 At_SetCrlaPara(VOS_UINT8 indexNum);
extern VOS_UINT32 At_QryCardSession(VOS_UINT8 indexNum);
extern TAF_UINT32 At_QryCardVoltagePara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetPrivateCglaPara(TAF_UINT8 indexNum);
extern TAF_UINT16 At_PrintPrivateCglaResult(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
extern TAF_UINT32 At_SetPrfAppPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_QryPrfAppPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_TestPrfAppPara(TAF_UINT8 indexNum);

extern TAF_UINT32 At_SetUiccPrfAppPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_QryUiccPrfAppPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_TestUiccPrfAppPara(TAF_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
extern TAF_UINT32 At_SetCCimiPara(TAF_UINT8 indexNum);
#endif
extern TAF_UINT16 At_CardErrorInfoInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);

extern TAF_UINT16 At_CardIccidInfoInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);

TAF_UINT16 At_CardStatusInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);

#if ((FEATURE_VSIM == FEATURE_ON) && (FEATURE_VSIM_ICC_SEC_CHANNEL == FEATURE_ON))
extern VOS_UINT32 At_QryHvCheckCardPara(VOS_UINT8 indexNum);

VOS_UINT32 At_QryIccVsimVer(VOS_UINT8 indexNum);

#endif

TAF_UINT16 At_SimHotPlugStatusInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);

TAF_UINT16 At_SWCheckStatusInd(SI_PIH_EventInfo *event);

TAF_UINT16 At_UpdateFileAtInd(SI_PIH_EventInfo *event);

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
TAF_UINT16 AT_SimRefreshInd(SI_PIH_EventInfo *event);
VOS_UINT32 AT_ProcNoCardSetCnf(VOS_UINT8 index, SI_PIH_EventInfo *event, VOS_UINT16 *length);
#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdSimProc.h */
