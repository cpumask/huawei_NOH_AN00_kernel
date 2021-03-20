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

#ifndef __ATEVENTREPORT_H__
#define __ATEVENTREPORT_H__

#include "si_app_emat.h"
#include "taf_ps_api.h"
#include "si_app_stk.h"
#include "mn_call_api.h"
#include "AtParse.h"
#include "AtCtx.h"
#include "si_app_pb.h"

#include "ATCmdProc.h"

#include "taf_ccm_api.h"
#include "taf_call_comm.h"
#include "AtSetParaCmd.h"
#include "taf_msg_chk_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_UNKNOWN_CLCK_CLASS 0

#define AT_MINUTES_OF_ONE_QUARTER 15
#define AT_SECONDS_OF_ONE_MINUTE 60
#define AT_LOCAL_TIME_ZONE_ADD_MAX_VALUE 56
#define AT_LOCAL_TIME_ZONE_DEL_MAX_VALUE 48

#define AT_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_PTR() (&g_atCurcRptCmdTable[0])
#define AT_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_SIZE() (sizeof(g_atCurcRptCmdTable) / sizeof(AT_RptCmdIndexUint8))

#define AT_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_PTR() (&g_atUnsolicitedRptCmdTable[0])
#define AT_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_SIZE() \
    (sizeof(g_atUnsolicitedRptCmdTable) / sizeof(AT_RptCmdIndexUint8))

#define AT_GET_CME_CALL_ERR_CODE_MAP_TBL_PTR() (g_atCmeCallErrCodeMapTbl)
#define AT_GET_CME_CALL_ERR_CODE_MAP_TBL_SIZE() (sizeof(g_atCmeCallErrCodeMapTbl) / sizeof(AT_CmeCallErrCodeMap))

#define AT_GET_CMS_SMS_ERR_CODE_MAP_TBL_PTR() (g_atCmsSmsErrCodeMapTbl)
#define AT_GET_CMS_SMS_ERR_CODE_MAP_TBL_SIZE() (sizeof(g_atCmsSmsErrCodeMapTbl) / sizeof(AT_CMS_SmsErrCodeMap))

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))
#define AT_GET_ENC_VOICE_ERR_CODE_MAP_TBL_PTR() (g_atEncVoiceErrCodeMapTbl)
#define AT_GET_ENC_VOICE_ERR_CODE_MAP_TBL_SIZE() (sizeof(g_atEncVoiceErrCodeMapTbl) / sizeof(AT_EncryptVoiceErrCodeMap))
#endif

#define AT_EVT_IS_VIDEO_CALL(enCallType)                                                   \
    (((enCallType) == MN_CALL_TYPE_VIDEO_TX) || ((enCallType) == MN_CALL_TYPE_VIDEO_RX) || \
     ((enCallType) == MN_CALL_TYPE_VIDEO))

#define AT_EVT_IS_PS_VIDEO_CALL(enCallType, enVoiceDomain)                               \
    ((((enCallType) == MN_CALL_TYPE_VIDEO) || ((enCallType) == MN_CALL_TYPE_VIDEO_TX) || \
      ((enCallType) == MN_CALL_TYPE_VIDEO_RX)) &&                                        \
     ((enVoiceDomain) == TAF_CALL_VOICE_DOMAIN_IMS))

#define AT_EVT_REL_IS_NEED_CLR_TIMER_STATUS_CMD(CmdCurrentOpt)                                            \
    (((CmdCurrentOpt) == AT_CMD_D_CS_VOICE_CALL_SET) || ((CmdCurrentOpt) == AT_CMD_D_CS_DATA_CALL_SET) || \
     ((CmdCurrentOpt) == AT_CMD_APDS_SET) || ((CmdCurrentOpt) == AT_CMD_CHLD_EX_SET) ||                   \
     ((CmdCurrentOpt) == AT_CMD_A_SET) || ((CmdCurrentOpt) == AT_CMD_END_SET))

#define AT_DEC_MAX_NUM 9
#define AT_DEC_SIGNED_MIN_NUM (-9)


enum AT_IMS_CallDomain {
    AT_IMS_CALL_DOMAIN_LTE  = 0,
    AT_IMS_CALL_DOMAIN_WIFI = 1,
    AT_IMS_CALL_DOMAIN_NR   = 2,

    AT_IMS_CALL_DOMAIN_BUTT = 255
};
typedef VOS_UINT8 AT_IMS_CallDomainUint8;


enum AT_CS_CallState {
    AT_CS_CALL_STATE_ORIG = 0,  /* originate a MO Call */
    AT_CS_CALL_STATE_CALL_PROC, /* Call is Proceeding */
    AT_CS_CALL_STATE_ALERTING,  /* Alerting,MO Call */
    AT_CS_CALL_STATE_CONNECT,   /* Call Connect */
    AT_CS_CALL_STATE_RELEASED,  /* Call Released */
    AT_CS_CALL_STATE_INCOMMING, /* Incoming Call */
    AT_CS_CALL_STATE_WAITING,   /* Waiting Call */
    AT_CS_CALL_STATE_HOLD,      /* Hold Call */
    AT_CS_CALL_STATE_RETRIEVE,  /* Call Retrieved */

    AT_CS_CALL_STATE_BUTT
};
typedef VOS_UINT8 AT_CS_CallStateUint8;

enum AT_ECALL_Type {
    AT_ECALL_TYPE_TEST = 0,     /* test ecall */
    AT_ECALL_TYPE_RECFGURATION, /* reconfiguration call */
    AT_ECALL_TYPE_MIEC,         /* manually initiated ecall */
    AT_ECALL_TYPE_AIEC,         /* automatic initiated ecall */

    AT_ECALL_TYPE_BUTT
};
typedef VOS_UINT8 AT_ECALL_TypeUint8;

extern VOS_UINT32 g_atAppDialModeCnf;

extern VOS_UINT32 g_lcStartTime;

typedef TAF_VOID (*AT_SMS_RSP_PROC_FUN)(TAF_UINT8 ucIndex, MN_MSG_EventInfo *pstEvent);

typedef TAF_VOID (*AT_QRY_PARA_PROC_FUNC)(TAF_UINT8 ucIndex, TAF_UINT8 OpId, TAF_UINT8 *pPara);

/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32            queryType;         /* 查询类型 */
    AT_QRY_PARA_PROC_FUNC atQryParaProcFunc; /* 查询类型对应的处理函数 */
} AT_QueryTypeFunc;
/*lint +e958 +e959 ;cause:64bit*/


typedef struct {
    TAF_MSG_ErrorUint32  msgErrorCode;
    AT_RreturnCodeUint32 atErrorCode;
} AT_SMS_ErrorCodeMap;


typedef struct {
    VOS_UINT32         cmeCode;
    TAF_CS_CauseUint32 csCause;

} AT_CmeCallErrCodeMap;


typedef struct {
    VOS_UINT32 cmsCode;
    VOS_UINT32 smsCause;

} AT_CMS_SmsErrCodeMap;

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))

typedef struct {
    AT_EncryptVoiceErrorUint32      atEncErr;
    TAF_CALL_EncryptVoiceTypeUint32 tafEncErr;

} AT_EncryptVoiceErrCodeMap;
#endif


typedef struct {
    TAF_ERROR_CodeUint32 tafErrCode;
    AT_RreturnCodeUint32 atReturnCode;

} AT_ChgTafErrCodeTbl;



typedef VOS_UINT32 (*AT_PIH_RSP_PROC_FUNC)(TAF_UINT8 ucIndex, SI_PIH_EventInfo *pstEvent, VOS_UINT16 *pusLength);

typedef struct {
    SI_PIH_EventUint32   eventType;
    VOS_UINT32           fixdedPartLen; /* 通过消息关联的结构可以直接计算出来消息长度的，直接标记消息有效长度 */
    TAF_ChkMsgLenFunc    chkFunc;       /* 消息结构中存在长度变量指示字段的，需要定义函数计算有效长度 */
    AT_PIH_RSP_PROC_FUNC atPihRspProcFunc;  //lint !e958
} AT_PIH_RspProcFunc;


typedef VOS_UINT32 (*AT_EMAT_RSP_PROC_FUNC)(TAF_UINT8 ucIndex, SI_EMAT_EventInfo *pstEvent, VOS_UINT16 *pusLength);

typedef struct {
    SI_EMAT_EventUint32   eventType;
    VOS_UINT32            fixdedPartLen; /* 通过消息关联的结构可以直接计算出来消息长度的，直接标记消息有效长度 */
    TAF_ChkMsgLenFunc     chkFunc;       /* 消息结构中存在长度变量指示字段的，需要定义函数计算有效长度 */
    AT_EMAT_RSP_PROC_FUNC atEMATRspProcFunc;  //lint !e958
} AT_EmatRspProcFunc;


typedef struct {
    MTA_AT_ResultUint32  mtaErrCode;
    AT_RreturnCodeUint32 atReturnCode;

} AT_ChgMtaErrCodeTbl;

extern TAF_UINT32 At_ChangeSTKCmdNo(TAF_UINT32 cmdType, TAF_UINT8 *cmdNo);

VOS_UINT32 AT_RcvTafCcmUus1InfoInd(struct MsgCB *msg);
VOS_UINT32 AT_ChkEmatAtEventCnfMsgLen(const MSG_Header *msgHeader);

/*
 * 功能描述: LOG打印消息处理
 */
VOS_VOID AT_LogPrintMsgProc(TAF_MntnLogPrint *msg);

/*
 * 功能描述: PS域事件处理函数
 */
VOS_VOID AT_RcvTafPsEvt(TAF_PS_Evt *evt);

VOS_UINT32 AT_RcvTafPsCallEvtPdpErrorInd(VOS_UINT8 indexNum, struct MsgCB *evtInfo);

/*
 * 功能描述: A核收到ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF事件的处理
 */
VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: A核收到ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ事件的处理
 */
VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateRej(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtPdpManageInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtPdpModifyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtPdpModifyRej(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtPdpModifiedInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: A核收到ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_CNF事件的处理
 */
VOS_UINT32 AT_RcvTafPsCallEvtPdpDeactivateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: A核收到ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND事件的处理
 */
VOS_UINT32 AT_RcvTafPsCallEvtPdpDeactivatedInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: A核收到ID_EVT_TAF_PS_CALL_END_CNF事件的处理
 */
VOS_UINT32 AT_RcvTafPsCallEvtCallEndCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtCallModifyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtCallAnswerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsCallEvtCallHangupCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetPrimPdpContextInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtSetSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetSecPdpContextInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetTftInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtSetUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetUmtsQosInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetUmtsQosMinInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetUmtsQosMinInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetUmtsQosMinInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGetDynamicUmtsQosInfoCnfMsgLen(const MSG_Header *msg);

VOS_UINT32 AT_RcvTafPsEvtGetDynamicUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetPdpStateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: Cgact查询命令返回
 */
VOS_UINT32 AT_RcvTafPsEvtCgactQryCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: Cgeqneg测试命令返回
 */
VOS_UINT32 AT_RcvTafPsEvtCgeqnegTestCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetPdpStateCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetPdpStateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetPdpIpAddrInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetPdpIpAddrInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetAnsModeInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetAnsModeInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetDynamicPrimPdpContextInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetDynamicPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetDynamicSecPdpContextInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetDynamicSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetDynamicTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetDynamicTftInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtSetEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetEpsQosInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGetDynamicEpsQosInfoCnfMsgLen(const MSG_Header *msg);

VOS_UINT32 AT_RcvTafPsEvtGetDynamicEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetDsFlowInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtClearDsFlowInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtConfigDsFlowRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtReportDsFlowInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtReportVTFlowInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtConfigVTFlowRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetApDsFlowRptCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetApDsFlowRptCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtApDsFlowReportInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetDsFlowNvWriteCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetDsFlowNvWriteCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetPdpDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetPdpDnsInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetPdpDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetAuthDataInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGetAuthDataInfoCnfMsgLen(const MSG_Header *msg);

VOS_UINT32 AT_RcvTafPsEvtGetAuthDataInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: ID_EVT_TAF_PS_GET_D_GPRS_ACTIVE_TYPE_CNF事件处理函数, 用于PPP拨号
 */
VOS_UINT32 AT_RcvTafPsEvtGetGprsActiveTypeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: ID_EVT_TAF_PS_PPP_DIAL_ORIG_CNF事件处理函数, 用于PPP拨号
 */
VOS_UINT32 AT_RcvTafPsEvtPppDialOrigCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 At_RcvTafPsEvtSetDialModeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtCgmtuValueChgInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtPdpDisconnectInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_VOID AT_QryParaRspCeregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_VOID AT_QryParaRspC5gregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para);
#endif

VOS_UINT32 AT_RcvTafPsEvtGetDynamicDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

TAF_VOID At_StkCsinIndPrint(TAF_UINT8 indexNum, SI_STK_EventInfo *event);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 atReadLtecsCnfProc(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 atReadCemodeCnfProc(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

/*
 * 功能描述: ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_CNF事件处理函数
 */
VOS_UINT32 AT_RcvTafGetLteAttachInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#endif

VOS_VOID AT_ConvertNasMccToBcdType(VOS_UINT32 nasMcc, VOS_UINT32 *mcc);

VOS_VOID AT_RcvMmaNsmStatusInd(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event);

VOS_UINT32 AT_CheckRptCmdStatus(VOS_UINT8 *rptCfg, AT_CmdRptCtrlTypeUint8 rptCtrlType, AT_RptCmdIndexUint8 rptCmdIndex);

VOS_VOID AT_ReportCCallstateResult(ModemIdUint16 modemId, VOS_UINT8 callId, VOS_UINT8 *rptCfg,
                                   AT_CS_CallStateUint8 callState, TAF_CALL_VoiceDomainUint8 voiceDomain);

VOS_VOID AT_ReportCendResult(VOS_UINT8 indexNum, TAF_CCM_CallReleasedInd *callReleaseInd);

VOS_VOID AT_CSCallStateReportProc(MN_AT_IndEvt *data);

VOS_VOID At_StkCcinIndPrint(TAF_UINT8 indexNum, SI_STK_EventInfo *event);

VOS_VOID At_StkHvsmrIndPrint(VOS_UINT8 indexNum, SI_STK_EventInfo *event);

VOS_VOID AT_RcvMmaRssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd);

TAF_UINT32 AT_ProcOperModeWhenLteOn(VOS_UINT8 indexNum);

/*
 * 功能描述: 从SS Event中获取AT命令错误码
 */
VOS_UINT32 AT_GetSsEventErrorCode(VOS_UINT8 indexNum, TAF_SS_CallIndependentEvent *event);

VOS_UINT32 At_QryParaPlmnListProc(struct MsgCB *msg);

#if (FEATURE_CSG == FEATURE_ON)
VOS_UINT32 AT_RcvMmaCsgListSearchCnfProc(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCsgListAbortCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCsgSpecSearchCnfProc(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaQryCampCsgIdInfoCnfProc(struct MsgCB *msg);

#endif

extern TAF_UINT8 At_GetClckClassFromBsCode(TAF_SS_BasicService *bs);

VOS_UINT32 AT_RcvTafCcmStartDtmfCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmStopDtmfCnf(struct MsgCB *msg);

VOS_VOID AT_RcvTafCallStartDtmfRslt(MN_AT_IndEvt *data);

VOS_VOID AT_RcvTafCallStopDtmfRslt(MN_AT_IndEvt *data);

VOS_VOID   AT_FlushSmsIndication(VOS_VOID);
VOS_UINT32 AT_IsClientBlock(VOS_VOID);

VOS_UINT32 At_RcvTafCcmCallOrigCnf(struct MsgCB *msg);
VOS_UINT32 At_RcvTafCcmCallSupsCmdCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallSupsCmdRsltInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallConnectInd(struct MsgCB *msg);
VOS_UINT32 At_RcvTafCcmCallIncomingInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallReleaseInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallAllReleasedInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallProcInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallAlertingInd(struct MsgCB *msg);
VOS_UINT32 AT_ReportCCallstateHoldList(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallRetrieveInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallHoldInd(struct MsgCB *msg);

/*
 * 功能描述: ID_MSG_TAF_GET_CID_SDF_CNF事件处理函数
 */
VOS_UINT32 AT_RcvTafPsEvtGetCidSdfInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ConvertCallError(TAF_CS_CauseUint32 cause);

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))
AT_EncryptVoiceErrorUint32 AT_MapEncVoiceErr(TAF_CALL_EncryptVoiceStatusUint32 tafEncVoiceErr);
#endif
VOS_VOID At_ReportClccDisplayName(MN_CALL_DisplayName *displayName, VOS_UINT16 *length);
VOS_VOID At_ProcQryClccResult(VOS_UINT8 numOfCalls, TAF_CCM_QryCallInfoCnf *qryCallInfoCnf, VOS_UINT8 indexNum);

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 At_ProcQryClccEconfResult(TAF_CCM_QryEconfCalledInfoCnf *callInfos, VOS_UINT8 indexNum);

VOS_UINT32 AT_RcvTafCcmEconfDialCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmEconfNotifyInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmCancelAddVideoCnf(struct MsgCB *msg);
#endif
VOS_UINT32 AT_RcvTafCcmQryCallInfoCnf(struct MsgCB *msg);

#if (FEATURE_ECALL == FEATURE_ON)
VOS_UINT32 At_ProcVcReportEcallStateEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo);

VOS_UINT32 At_ProcVcSetEcallCfgEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo);
#endif

VOS_VOID At_ChangeEcallTypeToCallType(MN_CALL_TypeUint8 ecallType, MN_CALL_TypeUint8 *callType);

PS_BOOL_ENUM_UINT8 At_CheckReportCendCallType(MN_CALL_TypeUint8 callType);

PS_BOOL_ENUM_UINT8 At_CheckReportOrigCallType(MN_CALL_TypeUint8 callType);

PS_BOOL_ENUM_UINT8 At_CheckReportConfCallType(MN_CALL_TypeUint8 callType);

PS_BOOL_ENUM_UINT8 At_CheckUartRingTeCallType(MN_CALL_TypeUint8 callType);


VOS_VOID AT_ReportSmMeFull(VOS_UINT8 indexNum, MN_MSG_MemStoreUint8 memStore);

TAF_UINT32 At_HexText2AsciiStringSimple(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT32 len,
                                        TAF_UINT8 *str);

PS_BOOL_ENUM_UINT8 At_CheckOrigCnfCallType(MN_CALL_TypeUint8 callType, VOS_UINT8 indexNum);
VOS_UINT32         At_IsCmdCurrentOptSendedOrigReq(AT_CmdCurrentOpt cmdCurrentOpt);

VOS_VOID AT_PB_ReadContinueProc(AT_ClientIdUint16 indexNum);
VOS_VOID AT_PB_ReadRspProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event);

VOS_UINT32 AT_RcvTafPsEvtSetPktCdataInactivityTimeLenCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetPktCdataInactivityTimeLenCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);


VOS_VOID AT_ReportSysCfgQryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum);

VOS_VOID AT_ReportSysCfgExQryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum);

VOS_VOID AT_ProcRegStatusInfoInd(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInfo);

VOS_UINT32 AT_RcvTafPsEvtSetImsPdpCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSet1xDormTimerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGet1xDormTimerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
VOS_UINT32 At_PrintSilentPinInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);
#endif
VOS_UINT32 At_PrintSetEsimSwitchInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_PrintQryEsimSwitchInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_PrintEsimCleanProfileInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_PrintEsimCheckProfileInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_PrintGetEsimEidInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32 At_PrintGetEsimPKIDInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length);

VOS_UINT32         AT_RcvTafPsCallEvtLimitPdpActInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
PS_BOOL_ENUM_UINT8 AT_CheckCurrentOptType_SupsCmdSuccess(VOS_UINT8 ataReportOkAsyncFlag, TAF_UINT8 indexNum);

PS_BOOL_ENUM_UINT8 AT_CheckCurrentOptType_SupsCmdOthers(TAF_UINT8 indexNum);

VOS_VOID AT_ConvertNasMncToBcdType(VOS_UINT32 nasMnc, VOS_UINT32 *mnc);

VOS_UINT32 AT_RcvTafPsEvtSetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtGetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtGetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ReportWs46QryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvTafPsEvtSet5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGet5gQosInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGet5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_ChkTafPsEvtGetDynamic5gQosInfoCnfMsgLen(const MSG_Header *msg);
VOS_UINT32 AT_RcvTafPsEvtGetDynamic5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtUePolicyRptInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtSetUePolicyRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_ChkTafPsEvtGetUePolicyCnfMsgLen(const MSG_Header *msg);

VOS_UINT32 AT_RcvTafPsEvtGetUePolicyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafPsEvtUePolicyRspCheckRsltInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_VOID AT_ConvertMultiSNssaiToString(VOS_UINT8 sNssaiNum, PS_S_NSSAI_STRU *sNssai, VOS_CHAR *pcStrNssai,
                                       VOS_UINT32 srcNssaiLength, VOS_UINT32 *dsrLength);

VOS_VOID AT_ConvertMultiRejectSNssaiToString(VOS_UINT8 sNssaiNum, PS_REJECTED_SNssai *sNssai, VOS_CHAR *pcStrNssai,
                                             VOS_UINT32 srcNssaiLength, VOS_UINT32 *dsrLength);

VOS_UINT32 AT_RcvTafPsEtherSessCapInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

#endif

VOS_UINT32 At_PlmnDetectIndProc(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafPsEvtSetRoamPdpTypeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);

VOS_UINT32 AT_RcvTafCcmCcwaiSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryXlemaCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmSetAlsCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmSetUus1InfoCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryUus1InfoCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryAlsCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEccNumInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryClprCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmSetCssnCnf(struct MsgCB *msg);

VOS_UINT32 AT_Hex2AsciiStrLowHalfFirst(VOS_UINT32 maxLength, VOS_INT8 *pcHeadaddr, VOS_UINT8 *dst, VOS_UINT8 *src,
                                       VOS_UINT16 srcLen);

VOS_UINT32 AT_RcvTafCcmCallOrigInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafPsEvtGetSinglePdnSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
VOS_UINT32 AT_RcvTafPsEvtSetSinglePdnSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo);
#pragma pack(pop)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
