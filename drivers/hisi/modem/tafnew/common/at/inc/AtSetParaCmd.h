/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
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
 * *    documentation and/or other materials provided with the distribution
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
#ifndef __ATSETPARACMD_H__
#define __ATSETPARACMD_H__

#include "vos.h"
#include "taf_ps_api.h"
#include "pppa_pppc_at_type.h"
#include "taf_ps_type_def.h"

#include "si_app_pb.h"
#include "at_mta_interface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#define AT_SIMLOCKDATAWRITE_PARA_LEN 1032 /* 锁网锁卡改制命令参数结构体长度 */
#define AT_BITS_OF_ONE_BYTE 8
#define AT_OOS_SRCH_STGY_TIMER_LEN_PARA_LEN 2

typedef VOS_UINT8  AT_SYSCFG_RatPrioUint8;
typedef VOS_UINT32 AT_PhynumTypeUint32;
typedef VOS_UINT8  AT_SyscfgexRatTypeUint8;


typedef struct {
    VOS_UINT8               ratOrderNum;                          /* syscfgex中设置的acqoder中的指示接入技术的个数 */
    AT_SyscfgexRatTypeUint8 ratOrder[TAF_PH_MAX_SUPPORT_RAT_NUM]; /* at^syscfgex中设置的acqoder接入优先级 */
    VOS_UINT8               rsv;
} AT_SyscfgexRatOrder;


typedef struct {
    /* 结构体码流, 包含该锁网锁卡类型的包括状态, 锁网号段, CK, UK等所有信息(未做half-byte解析) */
    VOS_UINT8 categoryData[AT_SIMLOCKDATAWRITE_PARA_LEN];
} AT_SIMLOCKDATAWRITE_SetReq;


typedef struct {
    VOS_UINT8 *phynumTypeData;
    VOS_UINT8 *phonePhynumValue;
    VOS_UINT8 *hmacData;
    VOS_UINT16 phynumTypeLen;
    VOS_UINT16 phonePhynumValueLen;
    VOS_UINT16 hmacLen;
    VOS_UINT8  rsv[2];
} AT_PhonePhynumPara;

PPP_AuthTypeUint8 AT_CtrlGetPDPAuthType(VOS_UINT32 value, VOS_UINT16 totalLen);

TAF_PDP_AuthTypeUint8 AT_ClGetPdpAuthType(VOS_UINT32 value, VOS_UINT16 totalLen);

TAF_UINT32 At_AsciiNum2HexString(TAF_UINT8 *src, TAF_UINT16 *srcLen);
TAF_UINT32 At_AsciiString2HexSimple(TAF_UINT8 *textStr, TAF_UINT8 *src, TAF_UINT16 srcLen);

TAF_UINT32 AT_HexToAsciiString(TAF_UINT8 *dstStr, TAF_UINT32 dstLen, TAF_UINT8 *srcStr, TAF_UINT32 srcLen);

TAF_UINT32 At_AsciiNum2BcdNum(TAF_UINT8 *dst, TAF_UINT8 *src, TAF_UINT16 srcLen);
TAF_UINT32 At_CheckNumLen(TAF_UINT16 max, TAF_UINT16 len);

TAF_UINT32 At_CheckUssdNumLen(VOS_UINT8 indexNum, TAF_SS_DATA_CODING_SCHEME dcs, TAF_UINT16 len);

TAF_UINT32 At_UnicodeTransferTo81CalcLength(const TAF_UINT8 *data, TAF_UINT16 len, TAF_UINT16 *baseCode,
                                            TAF_UINT16 *dstLen);

TAF_UINT32 At_UnicodeTransferTo82CalcLength(const TAF_UINT8 *data, TAF_UINT16 len, TAF_UINT16 *baseCode,
                                            TAF_UINT16 *dstLen);

TAF_UINT32 At_UnicodeTransferTo81(const TAF_UINT8 *data, TAF_UINT16 len, TAF_UINT16 codeBase, TAF_UINT8 *dest);

TAF_UINT32 At_UnicodeTransferTo82(const TAF_UINT8 *data, TAF_UINT16 len, TAF_UINT16 codeBase, TAF_UINT8 *dest);

TAF_UINT32 At_UnicodePrint2Unicode(TAF_UINT8 *data, TAF_UINT16 *len);
TAF_UINT32 At_UnicodePrintToAscii(TAF_UINT8 *data, TAF_UINT16 *len);
VOS_VOID   AT_PhSendRestoreFactParm(VOS_VOID);
VOS_VOID   AT_PhSendRestoreFactParmNoReset(VOS_VOID);

TAF_UINT32 At_SetNumTypePara(TAF_UINT8 *dst, TAF_UINT32 dstLen, TAF_UINT8 *src, TAF_UINT16 srcLen);

#if (FEATURE_RSTRIGGER_SMS == FEATURE_ON)
VOS_UINT32 AT_ReadActiveMessage(ModemIdUint16 modemId, MN_MSG_ActiveMessage *activeMessage);

TAF_UINT32 AT_SetRstriggerPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetCsmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgsmsPara(TAF_UINT8 indexNum);

VOS_UINT32 At_GetSmsStorage(VOS_UINT8 indexNum, MN_MSG_MemStoreUint8 memReadorDelete,
                            MN_MSG_MemStoreUint8 memSendorWrite, MN_MSG_MemStoreUint8 memRcv);

TAF_UINT32 At_SetCpmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgfPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCscaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsmpPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsdhPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCnmiPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmglPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCnmaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmssPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgwPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgdPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmstPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetCmStubPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCmmsPara(TAF_UINT8 indexNum);

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
TAF_UINT32 At_SetCscbPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetClipPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetClirPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetColpPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCscsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmeePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCeerPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCregPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgregPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetCsdfPara(VOS_UINT8 indexNum);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_SetCeregPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetC5gregPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetClccPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCpasPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCstaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCcugPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCssnPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetChldPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetChldExPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetChupPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetVtsPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetDtmfPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_AbortVtsPara(VOS_UINT8 index);
VOS_UINT32 AT_AbortDtmfPara(VOS_UINT8 index);
TAF_UINT32 At_SetCrcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCbstPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmodPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetCmmiPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_CheckApnFormat(VOS_UINT8 *apn, VOS_UINT16 apnLen, VOS_UINT16 clientId);

VOS_UINT32 AT_SetCgdcontPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCgdscontPara(VOS_UINT8 indexNum);
TAF_UINT32 AT_SetCgeqreqPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgeqminPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgpaddrPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgeqnegPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgcmodPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgactPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgansPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgtftPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgdnsPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCgautoPara(TAF_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetC5gqosPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetC5gqosRdpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetC5gPNssaiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetC5gNssaiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetC5gNssaaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetC5gNssaiRdpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCsUePolicy(VOS_UINT8 clientId);
VOS_UINT32 AT_SetLadnRptPara(VOS_UINT8 idx);
VOS_UINT32 AT_SetImsUrsp(VOS_UINT8 clientId);
#endif

VOS_UINT32 AT_SetMcsPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRrcStatPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetHfreqInfoPara(VOS_UINT8 indexNum);

VOS_UINT32 atSetCgeqosPara(VOS_UINT8 indexNum);
VOS_UINT32 atQryCgeqosPara(VOS_UINT8 indexNum);
VOS_UINT32 atSetCgeqosrdpPara(VOS_UINT8 indexNum);
VOS_UINT32 atSetCgcontrdpPara(VOS_UINT8 indexNum);
VOS_UINT32 atSetCgscontrdpPara(VOS_UINT8 indexNum);
VOS_UINT32 atSetCgtftrdpPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCsndPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCgdataPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgattPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgcattPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_DigitString2Hex(VOS_UINT8 *digitStr, VOS_UINT16 len, VOS_UINT32 *destHex);

#if (FEATURE_CSG == FEATURE_ON)
VOS_UINT32 AT_SetCsgIdSearchPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetCopsPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetCesqPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgclassPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCfunPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgmiPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgmmPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgmrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCgsnPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCimiPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsqPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetCpdwPara(VOS_UINT8 indexNum);
#endif

TAF_UINT32 At_SetSnPara(TAF_UINT8 indexNum);
VOS_UINT32 At_WriteSnPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetHwVerPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetFHVerPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetPfverPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCLACPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCpbsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCpbr2Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCpbrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCpbwPara(TAF_UINT8 indexNum);

TAF_UINT32 At_OneUnicodeToGsm(TAF_UINT16 src, TAF_UINT8 *dst, TAF_UINT32 dstBufLen, TAF_UINT16 *dstLen);

TAF_UINT32 At_SetCpbfPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetModemStatusPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCpbwPara2(TAF_UINT8 indexNum);
TAF_UINT32 At_SetScpbwPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetScpbrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCnumPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsimPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCchoPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCchpPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetPrivateCchoPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetPrivateCchpPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCchcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCglaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCardATRPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetUsimStub(VOS_UINT8 indexNum);
VOS_UINT32 At_SetRefreshStub(VOS_UINT8 indexNum);
VOS_UINT32 At_SetAutoReselStub(VOS_UINT8 indexNum);
#endif

#if (VOS_OS_VER == VOS_WIN32)
VOS_UINT32 At_SetDelayBgStub(VOS_UINT8 indexNum);
#endif

TAF_UINT32 At_SetCrsmPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCpinPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCpin2Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetIccidPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetPNNPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCPNNPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetOPLPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCardlockPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_FillSsBsService(TAF_SS_ErasessReq *sSPara, AT_ParseParaType *atPara);

VOS_UINT32 AT_FillSsNoRepCondTime(VOS_UINT8 ssCode, AT_ParseParaType *atPara, TAF_SS_RegisterssReq *sSPara);

TAF_UINT32 At_SetCcwaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCcfcPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetCmlckPara(TAF_UINT8 indexNum);
#endif

AT_MTA_PersCategoryUint8 AT_GetSimlockUnlockCategoryFromClck(VOS_VOID);

TAF_UINT32 At_SetClckPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCtfrPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetCusdPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetCpwdPara(TAF_UINT8 indexNum);

TAF_UINT32 At_SetDialGprsPara(TAF_UINT8 indexNum, TAF_UINT8 cid, TAF_GPRS_ActiveTypeUint8 activeType);

VOS_UINT32 AT_FillCalledNumPara(VOS_UINT8 *atPara, VOS_UINT16 len, MN_CALL_CalledNum *calledNum);

VOS_UINT32 At_DialNumByIndexFromPb(VOS_UINT8 indexNum, SI_PB_EventInfo *event);

TAF_UINT32 At_SetDPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetAPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_ModemHangupCall(VOS_UINT8 indexNum);
VOS_UINT32 At_SetHPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetEPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetVPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetTPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetPPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetXPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetZPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetQPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetAmpCPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetAmpDPara(VOS_UINT8 indexNum);

#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_UINT32 At_SetAmpSPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetLPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetS0Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetS3Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetS4Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetS5Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetS6Para(TAF_UINT8 indexNum);
TAF_UINT32 At_SetS7Para(TAF_UINT8 indexNum);

VOS_VOID AT_SetDefaultRatPrioList(ModemIdUint16 modemId, TAF_MMA_MultimodeRatCfg *sysCfgRatOrder,
                                  VOS_UINT8 userSptLteFlag, VOS_UINT8 userSptNrFlag);

AT_SYSCFG_RatPrioUint8 AT_GetGuAccessPrioNvim(ModemIdUint16 modemId);

VOS_UINT32 AT_IsSupportGMode(TAF_MMA_MultimodeRatCfg *ratOrder);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_IsSupport1XMode(TAF_MMA_MultimodeRatCfg *ratOrder);
VOS_UINT32 AT_IsSupportHrpdMode(TAF_MMA_MultimodeRatCfg *ratOrder);
#endif

VOS_UINT32 AT_IsSupportWMode(TAF_MMA_MultimodeRatCfg *ratOrder);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetCpamPara(VOS_UINT8 indexNum);

TAF_UINT32 At_SetTestPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCwasPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCGasPara(TAF_UINT8 indexNum);

TAF_UINT32 At_SetCmmPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_SetCccPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetStsfPara(TAF_UINT8 indexNum);
TAF_UINT32 At_ChangeSATCmdType(TAF_UINT32 cmdNo, TAF_UINT32 *cmdType);

#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
TAF_UINT32 At_SetStgiPara(TAF_UINT8 indexNum);
#endif

VOS_UINT32 At_SetClvlPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetVMSETPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetAppDialModePara(VOS_UINT8 indexNum);

VOS_VOID AT_ShowCccRst(NAS_CC_StateInfo *ccState, VOS_UINT16 *length);

TAF_UINT32 At_SetSPNPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCardModePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetDialModePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCurcPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetBOOTPara(TAF_UINT8 indexNum);
TAF_UINT32 At_DelWCellEntity(TAF_UINT8 indexNum);
TAF_UINT32 At_GetMeanRpt(TAF_UINT8 indexNum);
TAF_UINT32 At_SetRRCVersion(TAF_UINT8 indexNum);
TAF_UINT32 At_SetFreqLock(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetCellInfoPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCSNR(TAF_UINT8 indexNum);

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
VOS_UINT32 At_SetM2MFreqLock(VOS_UINT8 indexNum);
#endif

TAF_UINT32 At_SetParaCmd(TAF_UINT8 indexNum);

#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
TAF_UINT32 At_SetStgrPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetCstrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsinPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsenPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCsmnPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCstcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetImsichgPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_PhyNumIsNull(ModemIdUint16 modemId, AT_PhynumTypeUint32 setType, VOS_BOOL *pbPhyNumIsNull);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetImeiPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetFPlmnPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetQuickStart(TAF_UINT8 indexNum);
TAF_UINT32 At_SetAutoAttach(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetSystemInfo(TAF_UINT8 indexNum);
#endif

TAF_MMA_ServiceDomainUint8 At_ConvertDetachTypeToServiceDomain(VOS_UINT32 cgcattMode);

TAF_MMA_AttachTypeUint8 At_ConvertCgcattModeToAttachType(VOS_UINT32 cgcattMode);

VOS_UINT32 At_CheckRepeatedRatOrder(AT_SyscfgexRatOrder *syscfgExRatOrder);

VOS_UINT32 At_CheckSysCfgExRatTypeValid(VOS_UINT8 ratOrder);

VOS_UINT32 AT_ConvertSysCfgExGuBandPara(VOS_UINT8 *para, VOS_UINT16 len, TAF_UserSetPrefBand64 *prefBandPara);

VOS_UINT32 AT_ConvertSysCfgExLteBandPara(VOS_UINT8 *para, VOS_UINT16 len, TAF_USER_SetLtePrefBandInfo *prefBandPara);

TAF_MMA_UserSetPrioRatUint8 AT_GetSysCfgPrioRat(TAF_MMA_SysCfgPara *sysCfgExSetPara);

VOS_UINT32 AT_SetSysCfgExPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNetScanPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_AbortNetScanPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetSysCfgPara(VOS_UINT8 indexNum);
#if (FEATURE_UE_MODE_G == FEATURE_ON)
TAF_UINT32 At_SetTrigPara(TAF_UINT8 indexNum);
#endif
TAF_UINT32 At_SetGcfIndPara(TAF_UINT8 indexNum);
#endif

TAF_UINT32 At_SetCcinPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetGcapPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetMsIdInfo(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetDsFlowClrPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetDsFlowQryPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetDsFlowRptPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetVTFlowRptPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetSrvstPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetRssiPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetCerssiPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCnmrPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetTimePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCtzrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetModePara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetSimstPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetHSPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetPhyNumPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetGTimerPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetU2DiagPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetPort(VOS_UINT8 indexNum);
VOS_UINT32 AT_OpenSpecificPort(VOS_UINT8 port);
VOS_UINT32 AT_CloseSpecificPort(VOS_UINT8 port);
TAF_UINT32 At_SetPcscInfo(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCellSearch(TAF_UINT8 indexNum);
TAF_UINT32 At_SetGetportmodePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmsrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmgiPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCmmtPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetUssdModePara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetAdcTempPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetDwinsPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetCommDebugPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetYjcxPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetPhyInitPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDockPara(VOS_UINT8 indexNum);
#endif

TAF_UINT32 At_SetAt2OmPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_GetOperatorNameFromParam(VOS_UINT16 *operNameLen, VOS_CHAR *operName, VOS_UINT16 bufSize,
                                       VOS_UINT32 formatType);

TAF_UINT32 At_SetCpolPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCplsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetpidPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetOpwordParaForApModem(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetOpwordPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCpwordPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDislogPara(VOS_UINT8 indexNum);

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
VOS_UINT32 AT_SetSpwordPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetShellPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_SetRsrpCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRscpCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEcioCfgPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_DSDS == FEATURE_ON)
VOS_UINT32 At_SetPsProtectModePara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetWifiGlobalMacPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCsqlvlPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCsqlvlExtPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetOpenportPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_CheckIpv6Capability(VOS_UINT8 pdpType);
VOS_UINT32 AT_SetNdisdupPara(VOS_UINT8 indexNum);
TAF_UINT32 At_SetAuthdataPara(TAF_UINT8 indexNum);
TAF_UINT32 At_SetCrpnPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetAlsPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SaveRxDivPara(VOS_UINT16 setDivBands, VOS_UINT8 rxDivCfg);
TAF_UINT32 At_SetTModePara(TAF_UINT8 indexNum);

#if (FEATURE_LTEV == FEATURE_ON)
TAF_UINT32 At_SetVModePara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_LTE == FEATURE_ON)
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetSfm(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_SetFrStatus(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetBsn(VOS_UINT8 indexNum);
VOS_UINT32 At_SetQosPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetSDomainPara(VOS_UINT8 indexNum);
VOS_UINT32 At_WriteDpaCatToNV(VOS_UINT8 dpaRate);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetDpaCat(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_WriteRrcVerToNV(VOS_UINT8 srcWcdmaRRC);
VOS_UINT32 At_SetDataLock(VOS_UINT8 indexNum);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 At_SetInfoRRS(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetGPIOPL(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCuus1Para(VOS_UINT8 indexNum);
VOS_UINT32 At_SetGlastErrPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetMDatePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFacInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCallSrvPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCsdfltPara(VOS_UINT8 indexNum);
VOS_UINT32 At_GetParaCnt(VOS_UINT8 *data, VOS_UINT16 len);

VOS_UINT32 At_CovertAtParaToSimlockPlmnInfo(VOS_UINT32 paraCnt, AT_ParseParaType *paralist,
                                            TAF_CUSTOM_SimLockPlmnInfo *simLockPlmnInfo);

VOS_UINT32 At_SetSimLockPlmnInfo(VOS_UINT8 indexNum, VOS_UINT32 paraCnt, AT_ParseParaType *paralist);

VOS_UINT32 At_SetMaxLockTimes(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetVertime(VOS_UINT8 indexNum);

#if (VOS_OS_VER == VOS_WIN32)
VOS_UINT32 AT_ResetNplmn(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNplmn(VOS_UINT8 indexNum);
VOS_UINT32 At_SetReadUsimStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNvimPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPidReinitPara(VOS_UINT8 indexNum);
TAF_UINT32 AT_SetUSIMPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetSIMPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RefreshUSIMPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetSystemInfoEx(TAF_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 At_Set5gOptionPara(TAF_UINT8 indexNum);
VOS_UINT32 At_Qry5gOptionPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryNrSsbIdPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetTmmiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetTestScreenPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCdurPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetWebPwdPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetSdloadPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetProdNamePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetApRptSrvUrlPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetApXmlInfoTypePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetApXmlRptFlagPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFastDormPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetMemInfoPara(VOS_UINT8 indexNum);
#endif

#if (VOS_OS_VER == VOS_WIN32)
VOS_UINT32 AT_SetRplmnStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCsgStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDamParaStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEonsParaStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetWildCardParaStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCsUnAvailPlmnStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDisableRatPlmnStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetForbRoamTaStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetTinTypeStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPsBearIsrFlgStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsRatStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsCapabilityStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDomainStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCmdImsRoamingStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCmdRedailStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsVoiceInterSysLauEnableStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsVoiceMMEnableStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCmdImsUssdStub(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetDnsQueryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMemStatusPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetCdmaMemStatusPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetApRptPortSelectPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetUsbSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetSARReduction(VOS_UINT8 indexNum);
TAF_UINT32 At_SetScidPara(TAF_UINT8 indexNum);

VOS_VOID AtBase64Encode(const VOS_UINT8 *pdata, const VOS_UINT32 dataSize, VOS_UINT8 *outPcode);

unsigned int AtBase64Decode(const VOS_UINT8 *pcode, const unsigned int codeSize, VOS_UINT8 *outPdata,
                            const unsigned int dataSize, unsigned int *outPwritedDataSize);
VOS_UINT32   AT_SetRsfrPara(VOS_UINT8 indexNum);
VOS_VOID     AT_SetRsfrVersionCnf(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT32 len);
VOS_UINT32   AT_SetRsfwPara(VOS_UINT8 indexNum);
VOS_UINT32   AT_SetHukPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetFacAuthPubkeyExPara(VOS_UINT8 indexNum, VOS_UINT32 currIndex, VOS_UINT32 total, VOS_UINT32 paraLen,
                                     VOS_UINT8 *pubKeyData);

VOS_UINT32 AT_SetIdentifyStartPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetIdentifyEndPara(VOS_UINT8 indexNum, VOS_UINT8 *rsaData, VOS_UINT32 rsaLen, VOS_UINT8 otaRsaFlag);

VOS_UINT32 AT_SetSimlockDataWritePara(VOS_UINT8 indexNum, AT_SIMLOCKDATAWRITE_SetReq *simlockDataWrite);

VOS_UINT32 AT_SetPhonePhynumPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetApSimStPara(VOS_UINT8 indexNum);
#endif

VOS_VOID AT_ConvertMncToNasType(VOS_UINT32 mnc, VOS_UINT32 mncLen, VOS_UINT32 *nasMnc);

VOS_VOID AT_ConvertMccToNasType(VOS_UINT32 mcc, VOS_UINT32 *nasMcc);

VOS_UINT32 AT_SetNvmEccNumPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCposPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCposrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetXcposrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetXcposrRptPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetGpsInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLogCfgParaEx(VOS_UINT32 paraValue);
VOS_UINT32 AT_SetLogCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLogCpsParaEx(VOS_UINT32 logPort, VOS_UINT32 paraValue);
VOS_UINT32 AT_SetLogCpsPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLogNvePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLogSavePara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetCISAPara(VOS_UINT8 indexNum, VOS_UINT32 lengthValue, VOS_UINT8 *command, VOS_UINT16 commandLength);

VOS_UINT32 AT_SetCmutPara(VOS_UINT8 indexNum);

#if (FEATURE_PHONE_SC == FEATURE_ON)
VOS_UINT32 AT_SetSimlockUnlockPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetApdsPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetClprPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetCLteRoamAllowPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetWlthresholdcfgPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetSwverPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCbgPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SndMtaBodySarPara(VOS_UINT8 indexNum, AT_MTA_BodySarStateUint16 bodySarState,
                                MTA_BodySarPara *bodySarPara);

VOS_UINT32 AT_SetBodySarOnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetBodySarWcdmaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_GetGsmBandCapa(VOS_UINT32 *gBand);
VOS_UINT32 AT_SetBodySarGsmPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCopnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetHistoryFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCommBoosterPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNvLoadPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNCellMonitorPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetSimInsertPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetModemLoopPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetReleaseRrc(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRefclkfreqPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetDieSNPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetHandleDect(VOS_UINT8 indexNum);

#if (FEATURE_IMS == FEATURE_ON)
TAF_UINT32 AT_SetCiregPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCirepPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetVolteimpuPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCacmimsPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_SetCcwaiPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetUiccAuthPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetURSMPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetKsNafAuthPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRoamImsServicePara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetUserCfgOPlmnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEcidPara(VOS_UINT8 indexNum);

#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_VOID   AT_UpdateUartCfgNV(VOS_VOID);
VOS_UINT32 AT_SetIprPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetIcfPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetIfcPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetOPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetSwitchUart(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetRATCombinePara(VOS_UINT8 indexNum);

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_SetCallModifyInitPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCallModifyAnsPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEconfDialPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsSwitchPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_InputValueTransToVoiceDomain(VOS_UINT32 value, TAF_MMA_VoiceDomainUint32 *voiceDomain);

VOS_UINT32 AT_SetCevdpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsctrlmsgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsDomainCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCallEncryptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFusionCallRawPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_HUAWEI_VP == FEATURE_ON)
VOS_UINT32 AT_SetVoicePreferPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetCrmPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetCcmgsPara(VOS_UINT8 indexNum, VOS_UINT32 lengthValue, VOS_UINT8 *command, VOS_UINT16 commandLength);

VOS_UINT32 AT_SetCcmgwPara(VOS_UINT8 indexNum, VOS_UINT32 lengthValue, VOS_UINT32 statValue, VOS_UINT8 *command,
                           VOS_UINT16 commandLength);

VOS_UINT32 AT_SetCcmgdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_CheckCFreqLockEnablePara(VOS_VOID);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetCFreqLockPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetCdmaCsqPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetTTYModePara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 At_SetCustomDial(VOS_UINT8 indexNum);

VOS_UINT32 AT_IsPlatformSupportHrpdMode(PLATAFORM_RatCapability *platform);

VOS_UINT32 AT_IsPlatformSupport1XMode(PLATAFORM_RatCapability *platform);

VOS_UINT32 AT_SwitchCdmaMode(VOS_UINT8 indexNum, VOS_UINT32 modemId, PLATAFORM_RatCapability *modem0Platform,
                             PLATAFORM_RatCapability *modem1Platform, PLATAFORM_RatCapability *modem2Platform);

VOS_UINT32 At_SetCdmaModemSwitch(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCdmaModemSwitch(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCtaPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetFemCtrl(VOS_UINT8 indexNum);
VOS_UINT32 At_SetRatRfSwitch(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetQuitCallBack(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_Set1xChanPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetCSidEnable(VOS_UINT8 indexNum);
VOS_UINT32 AT_ConvertCSidListMcc(VOS_UINT8 *para, VOS_UINT16 len, VOS_UINT32 *mcc);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetCSidList(VOS_UINT8 indexNum);
#endif

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON)
VOS_UINT32 AT_SetEncryptCall(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEncryptCallRemoteCtrl(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEncryptCallCap(VOS_UINT8 indexNum);

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
VOS_UINT32 AT_SetEncryptCallKmc(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEccTestMode(VOS_UINT8 indexNum);
#endif
#endif

VOS_UINT32 AT_SetClocinfoEnable(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPrivacyModePreferred(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCtOosCount(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCtRoamInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNoCardMode(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_CheckMccFreqPara(VOS_VOID);
VOS_UINT32 AT_SetMccFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCloudDataPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_CheckBlockCellListBsPara(VOS_VOID);
VOS_UINT32 AT_CheckBlockCellListPara(VOS_VOID);
VOS_UINT32 AT_SetBlockCellListPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetBorderInfoPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_ParseDplmnLengthIllegal(VOS_UINT16 subStrLen);

VOS_UINT32 AT_ParseDplmnStringList(VOS_UINT8 indexNum, VOS_UINT8 *dplmnString, TAF_MMA_DplmnInfoSet *dplmnInfoSet);

VOS_UINT32 AT_ParseEhplmnStringList(VOS_UINT32 paraLen, VOS_UINT8 *para, VOS_UINT8 *ehPlmnNum, TAF_PLMN_Id *ehplmnList);

VOS_UINT32 AT_SetDplmnListPara(VOS_UINT8 indexNum);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
VOS_UINT32 AT_ExchangeModemInfo(VOS_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetCdmaDormantTimer(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetUECenterPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetQuickCardSwitchPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetHdrCsqPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetSensorPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetScreenPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPcuiCtrlConcurrentFlagForTest(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFclassPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetGciPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetBestFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_CheckPseucellInfoParaLen(VOS_VOID);

VOS_UINT32 AT_ParsePseucellInfoMccMnc(VOS_UINT8 *para, VOS_UINT16 paraLen, VOS_UINT32 *mcc, VOS_UINT32 *mnc);

VOS_UINT32 AT_ParsePseucellInfoLac(VOS_UINT8 *para, VOS_UINT16 paraLen, VOS_UINT32 *lac);

VOS_UINT32 AT_ParsePseucellInfoCellId(VOS_UINT8 *para, VOS_UINT16 paraLen, VOS_UINT32 *cellId);

VOS_UINT32 AT_SetPseucellInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetChrAlarmRlatCnfInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMtReattachPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetCemodePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCvhuPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCvhuPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetParaRspSimsqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryParaRspSimsqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPlmnSrchPara(TAF_UINT8 indexNum);
VOS_UINT32 At_SetDetectPlmnRsrp(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_SetLteSarStubPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 At_SetHifiReset(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetOosSrchStgyPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetSinglePdnSwitch(VOS_UINT8 indexNum);
#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
VOS_UINT32 AT_SetNoCard(VOS_UINT8 indexNum);
#endif
#endif

#pragma pack(pop)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtSetParaCmd.h */
