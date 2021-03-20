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

#ifndef __ATCMDMISCPROC_H__
#define __ATCMDMISCPROC_H__

#include "AtCtx.h"
#include "AtParse.h"
#include "at_mta_interface.h"
#include "at_lte_common.h"

#include "css_at_interface.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)


#define AT_EFUSE_OK 0     /* efuse返回成功 */
#define AT_EFUSE_REPEAT 1 /* efuse返回重复设置 */

#define AT_MODEM_YTD_LEN 10  /* 字符串年月日的总长度 */
#define AT_MODEM_TIME_LEN 8  /* 字符串小时、分钟、秒的总长度 */
#define AT_MODEM_YEAR_LEN 4  /* 字符串年份的长度 */
#define AT_MODEM_MONTH_LEN 2 /* 字符串月份的长度 */
#define AT_MODEM_DATE_LEN 2  /* 字符串日期的长度 */
#define AT_MODEM_HOUR_LEN 2  /* 字符串小时的长度 */
#define AT_MODEM_MIN_LEN 2   /* 字符串分钟的长度 */
#define AT_MODEM_SEC_LEN 2   /* 字符串秒的长度 */
#define AT_MODEM_ZONE_LEN 3  /* 字符串时区的长度 */
#define AT_GET_MODEM_TIME_BUFF_LEN 5

#define AT_MODEM_DEFALUT_AUX_MODE_YEAR_LEN 2      /* "yy"总长度 */
#define AT_MODEM_OTHER_AUX_MODE_YEAR_LEN 4        /* "yyyy"总长度 */
#define AT_MODEM_AUX_MODE_EXCEPT_YEAR_TIME_LEN 18 /* "/mm/dd,hh:mm:ss+/-zz"总长度 */

#define AT_MODEM_DEFALUT_AUX_MODE_YEAR_MAX 99 /* "yy"可设置年最大值 */
#define AT_MODEM_DEFALUT_AUX_MODE_YEAR_MIN 0  /* "yy"可设置年最小值 */

#define AT_MODEM_TIME_ZONE_MAX 48    /* 可设置时区最大值 */
#define AT_MODEM_TIME_ZONE_MIN (-48) /* 可设置时区最大值 */

#define AT_MODEM_YEAR_MAX 2050 /* 可设置年最大值 */
#define AT_MODEM_YEAR_MIN 1970 /* 可设置年最小值 */
#define AT_MODEM_MONTH_MAX 12  /* 可设置月最大值 */
#define AT_MODEM_MONTH_MIN 1   /* 可设置月最小值 */
#define AT_MODEM_DAY_MAX 31    /* 可设置天最大值 */
#define AT_MODEM_DAY_MIN 1     /* 可设置天最小值 */

#define AT_MODEM_HOUR_MAX 23 /* 可设置小时最大值 */
#define AT_MODEM_HOUR_MIN 0  /* 可设置小时最小值 */
#define AT_MODEM_MIN_MAX 59  /* 可设置分钟最大值 */
#define AT_MODEM_MIN_MIN 0   /* 可设置分钟最小值 */
#define AT_MODEM_SEC_MAX 59  /* 可设置秒钟最大值 */
#define AT_MODEM_SEC_MIN 0   /* 可设置秒钟最小值 */

#define AT_MODEM_ZONE_MAX 12    /* 可设置时区最大值 */
#define AT_MODEM_ZONE_MIN (-12) /* 可设置时区最小值 */

#define AT_TEMP_ZONE_DEFAULT 31 /* 温区默认值 二进制11111 */

#define AT_GMT_TIME_DEFAULT 70 /* 判断标准时间是1900还是1970 */

#define AT_SIM_LOCK_DATA_WRITEEX_MAX_TOTAL 42 /* 对total进行限制，不能超过42，防止超过OSA规格60k */

#define AT_FINE_TIME_DEFAULT_NUM 2

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#define AT_NRPWRCTRL_MAX_DELETE_CA_NUM (-8)
#define AT_NRPWRCTRL_MAX_ADD_CA_NUM 8

#define AT_NRPWRCTRL_DISABLE_REDUCE_RI 0
#define AT_NRPWRCTRL_ENABLE_REDUCE_RI 1

#define AT_NRPWRCTRL_MIN_SUPPORT_BSR_NUM 0
#define AT_NRPWRCTRL_MAX_SUPPORT_BSR_NUM 63
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
enum AT_NrpwrctrlModeType {
    AT_NRPWRCTRL_MODE_CC_NUM_CTR  = 0,
    AT_NRPWRCTRL_MODE_RI_NUM_CTR  = 1,
    AT_NRPWRCTRL_MODE_BSR_NUM_CTR = 2,
    AT_NRPWRCTRL_MODE_BUTT        = 3,
};
typedef VOS_UINT32 AT_NrpwrctrlModeTypeUint32;

#endif

typedef VOS_VOID (*AT_SpecialCmdProcFunc)(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
            const VOS_CHAR *specialCmdName);

typedef struct {
    const VOS_CHAR        *specialCmdName; /* 特殊命令名 */
    AT_SpecialCmdProcFunc  specialCmdFunc; /* 特殊命令处理函数 */
} AT_SpecialCmdFuncTbl;

VOS_UINT32 AT_SetActiveModem(VOS_UINT8 indexNum);
VOS_VOID   AT_FillOverHeatingReqParam(AT_MTA_OverHeatingSetReq *overHeatingReq);
VOS_UINT32 AT_FillUeCapPara(AT_MTA_UeCapParamSet *setUeCap, VOS_UINT32 bsLen);
VOS_UINT32 AT_CheckUeCapPara(VOS_VOID);
VOS_UINT32 AT_CheckOverHeatingPara(VOS_VOID);
VOS_BOOL   AT_CheckModem0(VOS_UINT8 indexNum);

#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)
VOS_UINT32 AT_SetMBMSServiceOptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMBMSServiceStatePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMBMSPreferencePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMBMSSib16NetworkTimePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMBMSBssiSignalLevelPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMBMSNetworkInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMBMSModemStatusPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMBMSCMDPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMBMSEVPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMBMSInterestListPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMBMSCmdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMBMSServiceOptSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSServiceStateSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSPreferenceSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSSib16NetworkTimeQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSBssiSignalLevelQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSNetworkInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSModemStatusQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSEVSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSServiceEventInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSInterestListSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaMBMSCmdQryCnf(struct MsgCB *msg);
VOS_VOID   AT_ReportMBMSCmdQryCnf(MTA_AT_MbmsAvlServiceListQryCnf *lrrcCnf, VOS_UINT8 indexNum);
VOS_UINT32 At_TestMBMSCMDPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_RcvMtaLteLowPowerSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetLteLowPowerPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_CheckIsmCoexParaValue(VOS_INT32 val, VOS_UINT32 paraNum);
VOS_UINT32 AT_SetIsmCoexPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryIsmCoexPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaIsmCoexSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaIsmCoexQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetLL2ComCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLL2ComQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaLL2ComCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLL2ComCfgQryCnf(struct MsgCB *msg);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetNL2ComCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNL2ComCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetNL2ComQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNL2ComCfgQryCnf(struct MsgCB *msg);
#ifndef MODEM_FUSION_VERSION
VOS_UINT32 AT_SetNPdcpSleepThresCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNPdcpSleepThresCfgCnf(struct MsgCB *msg);
#endif
#endif

VOS_UINT32 AT_SetLCaCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaLteCaCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryLCaCellExPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaLteCaCellExQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLteCaCellExInfoNtf(struct MsgCB *msg);
VOS_UINT32 AT_SetLcaCellRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLcaCellRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaCACellSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaCACellQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetFineTimeReqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaFineTimeSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSibFineTimeNtf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLppFineTimeNtf(struct MsgCB *msg);
VOS_UINT32 AT_SetConnectRecovery(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetOverHeatingCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetOverHeatingQryParam(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetLrrcUeCapPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaLrrcUeCapSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaOverHeatingSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaOverHeatingQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLrrcUeCapNtf(struct MsgCB *msg);

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_QryNrCaCellPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNrCaCellRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNrCaCellRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNrCaCellInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrCaCellInfoRptCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrCaCellInfoRptCfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrCaCellInfoInd(struct MsgCB *msg);
VOS_UINT32 AT_SetNrNwCapRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNrNwCapInfoReportInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrNwCapInfoRptCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetNrNwCapQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNrNwCapRptQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNrNwCapInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrNwCapInfoRptCfgQryCnf(struct MsgCB *msg);
#endif

VOS_UINT32 AT_SetLogEnablePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLogEnable(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetActPdpStubPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetNVCHKPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_RcvDrvAgentSimlockWriteExSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSimlockDataReadExReadCnf(struct MsgCB *msg);
#if (FEATURE_PHONE_SC == FEATURE_ON)
VOS_UINT32 AT_SimLockDataReadExPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_SaveSimlockDataIntoCtx(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                     VOS_UINT8 netWorkFlg);
VOS_UINT32 AT_SetSimlockDataWriteExPara(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                        AT_SimlockTypeUint8 netWorkFlg);
VOS_UINT32 AT_ProcSimlockWriteExData(VOS_UINT8 *simLockData, VOS_UINT16 paraLen);

VOS_UINT32 AT_ProcPhonePhynumCmdCtx(AT_PhonePhynumPara *phonePhynumSetPara);

VOS_UINT32 AT_RcvMtaAfcClkInfoCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetPhyComCfgPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#ifndef MODEM_FUSION_VERSION
VOS_UINT32 AT_SetNrphyComCfgPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_RcvMtaNrPhyComCfgSetCnf(struct MsgCB *msg);
#endif
#endif

VOS_UINT32 AT_RcvMtaPhyComCfgSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetSamplePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaSetSampleCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
extern VOS_UINT32 AT_SetEvdoSysEvent(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetDoSigMask(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_RcvMtaEvdoSysEventSetCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaEvdoSigMaskSetCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaEvdoRevARLinkInfoInd(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaEvdoSigExEventInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaNoCardModeSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaNoCardModeQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetCdmaModemCapPara(PLATAFORM_RatCapability *sourceModemPlatform,
                                  PLATAFORM_RatCapability *destinationModemPlatform, ModemIdUint16 sourceModemId,
                                  ModemIdUint16 destinationModemId, VOS_UINT8 indexNum);
VOS_UINT32 At_SetCdmaCapResume(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFactoryCdmaCap(VOS_UINT8 indexNum);

#endif

VOS_VOID AT_ReadPlatFormPrint(ModemIdUint16 modemId, PLATAFORM_RatCapability platRat);

VOS_UINT32 AT_SetFratIgnitionPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetModemTimePara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetRxTestModePara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_RcvMtaSetRxTestModeCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetMipiWrParaEx(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMipiWrEXCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetMipiRdParaEx(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaMipiRdEXCnf(struct MsgCB *msg);

#if (FEATURE_DSDS == FEATURE_ON)
VOS_UINT32 AT_RcvMmaDsdsStateSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaDsdsStateNotify(struct MsgCB *msg);
#endif

extern VOS_UINT32 AT_SetCrrconnPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_QryCrrconnPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_RcvMtaSetCrrconnCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaQryCrrconnCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaCrrconnStatusInd(struct MsgCB *msg);

extern VOS_UINT32 AT_SetVtrlqualrptPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_RcvMtaSetVtrlqualrptCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaRlQualityInfoInd(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaVideoDiagInfoRpt(struct MsgCB *msg);

VOS_UINT32 AT_SetEccCfgPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetSmsDomainPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaSmsDomainSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QrySmsDomainPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaSmsDomainQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetWs46Para(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryWs46Para(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestWs46Para(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetGameModePara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetGpsLocSetPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaGpsLocSetCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetLendcPara(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_SetNrFreqLockPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_GetNrFreqLockPara(VOS_UINT8 clientId, AT_MTA_SetNrfreqlockReq *nrFreqLockInfo);
#endif

#endif

extern VOS_VOID AT_PrintCclkTime(VOS_UINT8 indexNum, TIME_ZONE_Time *timeZoneTime, ModemIdUint16 modemId);
VOS_UINT32      AT_RcvMtaCclkQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaTempProtectInd(struct MsgCB *msg);

VOS_UINT32 AT_SetNvRefreshPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNvRefreshSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetPseudBtsPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetSubClfsParamPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QrySubClfsParamPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetClDbDomainStatusPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetUlFreqRptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryUlFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaUlFreqRptSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaUlFreqChangeInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaUlFreqRptQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaPseudBtsIdentInd(struct MsgCB *msg);

VOS_UINT32 At_SetPsScenePara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryPsScenePara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaPsSceneSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaPsSceneQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetSmsAntiAttackCapQrypara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvSmsAntiAttackCapQryCnf(struct MsgCB *rcvMsg);
VOS_UINT32 AT_SetSmsAntiAttackState(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaSmsAntiAttackSetCnf(struct MsgCB *rcvMsg);
VOS_UINT32 AT_QrySmsNasCapPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaSmsNasCapQryCnf(struct MsgCB *rcvMsg);
VOS_UINT32 AT_RcvMmaPseudBtsIdentInd(struct MsgCB *rcvMsg);
VOS_UINT32 AT_SetForceSyncPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaForceSyncSetCnf(struct MsgCB *msg);

VOS_UINT32 At_SetGnssNtyPara(VOS_UINT8 indexNum);

VOS_UINT32 At_SetLowPowerModePara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetNrrcCapCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNrrcCapQryPara(VOS_UINT8 indexNum);
#ifndef MODEM_FUSION_VERSION
VOS_UINT32 AT_SetNrPwrCtrlPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestNrPwrCtrlPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_SetNrrcUeCapPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNrrcUeCapSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrrcUeCapNtf(struct MsgCB *msg);

#endif

VOS_UINT32 AT_RcvMtaTxPowerQryCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#ifndef MODEM_FUSION_VERSION
VOS_UINT32 AT_RcvMtaNtxPowerQryCnf(struct MsgCB *msg);
#endif
#endif

VOS_UINT32 AT_RcvMtaMcsSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaHfreqinfoQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaHfreqinfoInd(struct MsgCB *msg);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvMtaFastReturn5gEndcInd(struct MsgCB *msg);
#endif
VOS_UINT32 AT_RcvMmaRrcStatQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRrcStatInd(struct MsgCB *msg);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvMta5gOptionSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMta5gOptionQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetNrBandBlockListWith3Para(TAF_MMA_NrBandBlockListSetPara *nrBandBlockListSetStru);
VOS_UINT32 AT_SetNrBandBlockListPara(VOS_UINT8 atIndex);

VOS_UINT32 AT_RcvMmaNrBandBlockListSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaNrBandBlockListQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetNwDeploymentPara(VOS_UINT8 atIndex);

VOS_UINT32 AT_RcvCssNwDeploymentSetCnf(struct MsgCB *msg);
#endif

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetUartTest(VOS_UINT8 indexId);
VOS_UINT32 AT_QryUartTest(VOS_UINT8 indexId);
VOS_UINT32 AT_SetI2sTest(VOS_UINT8 indexId);
VOS_UINT32 AT_QryI2sTest(VOS_UINT8 indexId);
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMtaLteSarStubSetCnf(struct MsgCB *msg);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_SetNrPowerSavingCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetNrPowerSavingQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNrPowerSavingCfgCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrPowerSavingQryCnf(struct MsgCB *msg);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdMiscProc.h */
