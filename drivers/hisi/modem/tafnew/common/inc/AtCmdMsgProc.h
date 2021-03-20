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

#ifndef __ATCMDMSGPROC_H__
#define __ATCMDMSGPROC_H__

#include "vos.h"
#include "AtTypeDef.h"
#include "AtCtx.h"
#include "AcpuReset.h"
#include "taf_drv_agent.h"
#include "at_mta_interface.h"
#include "AtInternalMsg.h"
#if (FEATURE_IMS == FEATURE_ON)
#include "at_imsa_interface.h"
#endif

#include "taf_app_mma.h"

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#include "at_xpds_interface.h"
#endif

#include "taf_ccm_api.h"

#include "at_lte_ct_proc.h"
#include "at_lte_eventreport.h"
#include "AtSetParaCmd.h"
#include "dms_port_def.h"
#include "taf_msg_chk_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define TAF_AT_INVALID_MCC 0xffffffff

/* CESQ命令Rxlev参数相关宏定义 */
#define AT_CMD_CESQ_RXLEV_MIN_VALUE 0
#define AT_CMD_CESQ_RXLEV_MAX_VALUE 63
#define AT_CMD_CESQ_RXLEV_INVALID_VALUE 99
#define AT_CMD_CESQ_RXLEV_CONVERT_BASE_VALUE 111
#define AT_CMD_CESQ_RXLEV_LOWER_BOUNDARY_VALUE (-110)
#define AT_CMD_CESQ_RXLEV_UPPER_BOUNDARY_VALUE (-48)

/* CESQ命令Ber参数相关宏定义 */
#define AT_CMD_CESQ_BER_INVALID_VALUE 99

/* CESQ命令Rscp参数相关宏定义 */
#define AT_CMD_CESQ_RSCP_MIN_VALUE 0
#define AT_CMD_CESQ_RSCP_MAX_VALUE 96
#define AT_CMD_CESQ_RSCP_INVALID_VALUE 255
#define AT_CMD_CESQ_RSCP_CONVERT_BASE_VALUE 121
#define AT_CMD_CESQ_RSCP_LOWER_BOUNDARY_VALUE (-120)
#define AT_CMD_CESQ_RSCP_UPPER_BOUNDARY_VALUE (-25)

/* CESQ命令Ecno参数相关宏定义 */
#define AT_CMD_CESQ_ECNO_MIN_VALUE 0
#define AT_CMD_CESQ_ECNO_MAX_VALUE 49
#define AT_CMD_CESQ_ECNO_INVALID_VALUE 255
#define AT_CMD_CESQ_ECNO_CONVERT_BASE_VALUE 49
#define AT_CMD_CESQ_ECNO_LOWER_BOUNDARY_VALUE (-48)
#define AT_CMD_CESQ_ECNO_UPPER_BOUNDARY_VALUE 0

/* CESQ命令Rsrq参数相关宏定义 */
#define AT_CMD_CESQ_RSRQ_MIN_VALUE 0
#define AT_CMD_CESQ_RSRQ_MAX_VALUE 34
#define AT_CMD_CESQ_RSRQ_INVALID_VALUE 255
#define AT_CMD_CESQ_RSRQ_CONVERT_BASE_VALUE 40
#define AT_CMD_CESQ_RSRQ_LOWER_BOUNDARY_VALUE (-39)
#define AT_CMD_CESQ_RSRQ_UPPER_BOUNDARY_VALUE (-6)

/* CESQ命令Rsrp参数相关宏定义 */
#define AT_CMD_CESQ_RSRP_MIN_VALUE 0
#define AT_CMD_CESQ_RSRP_MAX_VALUE 97
#define AT_CMD_CESQ_RSRP_INVALID_VALUE 255
#define AT_CMD_CESQ_RSRP_CONVERT_BASE_VALUE 141
#define AT_CMD_CESQ_RSRP_LOWER_BOUNDARY_VALUE (-140)
#define AT_CMD_CESQ_RSRP_UPPER_BOUNDARY_VALUE (-44)

#define AT_SIGNAL_DIVISOR_TWO 2
#define AT_SIGNAL_DIVISOR_THREE 3
#define AT_SIGNAL_DIVISOR_FOUR  4
#define AT_SIGNAL_DIVISOR_EIGHT 8

#define AT_CMD_TIMEQRY_YEAR_DEFAULT_VALUE 90
#define AT_CMD_TIMEQRY_MONTH_DEFAULT_VALUE 1
#define AT_CMD_TIMEQRY_DAY_DEFAULT_VALUE 6
#define AT_CMD_TIMEQRY_HOUR_DEFAULT_VALUE 8

#define AT_SIGNAL_INVALID_VALUE 99

#define AT_PSEUD_BTS_PARAM_ENABLE 1 /* 查询伪基站是否支持 */
#define AT_PSEUD_BTS_PARAM_TIMES 2  /* 查询伪基站拦截次数 */

#define AT_FEATURE_BAND_STR_LEN_MAX 512

#define AT_PB_DECODE_LEN_MAX 500

#define AT_RX_RSRPR0 0  /* 天线 0 rsrp， 单位 dB */
#define AT_RX_RSRPR1 1  /* 天线 1 rsrp， 单位 dB */
#define AT_RX_RSRPR2 2  /* 天线 2 rsrp， 单位 dB，仅 4收时有效 */
#define AT_RX_RSRPR3 3  /* 天线 3 rsrp， 单位 dB，仅 4收时有效 */

#define AT_RX_SINRR0 0  /* 天线 0 sinr */
#define AT_RX_SINRR1 1  /* 天线 1 sinr */
#define AT_RX_SINRR2 2  /* 天线 2 sinr */
#define AT_RX_SINRR3 3  /* 天线 3 sinr */

extern VOS_INT8 g_ate5DissdPwd[AT_DISSD_PWD_LEN + 1];

/* 消息处理函数指针 */
typedef VOS_UINT32 (*pAtProcMsgFromDrvAgentFunc)(struct MsgCB *pMsg);

/* AT与MTA模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_MTA_MSG_PROC_FUNC)(struct MsgCB *pMsg);

/* AT与MMA模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_MMA_MSG_PROC_FUNC)(struct MsgCB *pMsg);

typedef VOS_UINT32 (*AT_XCALL_MSG_PROC_FUNC)(struct MsgCB *pMsg);

typedef VOS_UINT32 (*AT_CCM_MSG_PROC_FUNC)(struct MsgCB *pMsg);

typedef VOS_VOID (*AT_MSG_PROC_FUNC)(struct MsgCB *pMsg);

typedef VOS_UINT32 (*AT_VNAS_ProcFunc)(struct MsgCB *msg);

typedef VOS_UINT32 (*AT_DMS_MSG_PROC_FUNC)(struct MsgCB *msg);

typedef VOS_UINT32 (*AT_DMS_EVENT_PROC_FUNC)(struct MsgCB *msg);

/*
 * 结构说明: 消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    DRV_AGENT_MsgTypeUint32    msgType;
    pAtProcMsgFromDrvAgentFunc procMsgFunc;
} AT_PROC_MsgFromDrvAgent;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构说明: 消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32       sndPid;
    AT_MSG_PROC_FUNC procMsgFunc;
} AT_MSG_Proc;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * Structure: NAS_AT_OUTSIDE_RUNNING_CONTEXT_PART_ST
 * Description: PC回放工程，存储所有AT相关的全局变量，目前仅有短信相关全局变量
 * Message origin:
 * Note:
 */
typedef struct {
    VOS_UINT8         used;     /* 指示当前索引是否已被使用 */
    AT_USER_TYPE      userType; /* 指示当前用户类型 */
    AT_IND_MODE_TYPE  indMode;  /* 指示当前命令模式，只针对MUX和APP */
    VOS_UINT8         reserv1[1];
    VOS_UINT16        clientId; /* 指示当前用户的 */
    MN_OPERATION_ID_T opId;     /* Operation ID, 标识本次操作             */
    VOS_UINT8         reserv2[1];
} NAS_AT_ClientManageSimple;

/*
 * Structure: NAS_AT_SdtAtClientTable
 * Description: PC回放工程，所有AT相关的全局变量通过以下消息结构发送
 * Message origin:
 */
typedef struct {
    VOS_MSG_HEADER
    AT_InterMsgIdUint32       msgID;
    VOS_UINT8                 type;
    VOS_UINT8                 reserved[3]; /* 在PACK(1)到PACK(4)调整中定义的保留字节 */
    NAS_AT_ClientManageSimple atClientTab[AT_MAX_CLIENT_NUM];
} NAS_AT_SdtAtClientTable;

typedef struct {
    AT_CSCS_TYPE               atCscsType;
    AT_CSDH_TYPE               atCsdhType;
    MN_OPERATION_ID_T          opId;
    MN_MSG_CsmsMsgVersionUint8 atCsmsMsgVersion;
    AT_CnmiType                atCnmiType;
    AT_CmgfMsgFormatUint8      atCmgfMsgFormat;
    VOS_UINT8                  reserved[3]; /* 在PACK(1)到PACK(4)调整中定义的保留字节 */
    AT_CSCA_CsmpInfo           atCscaCsmpInfo;
    AT_MSG_Cpms                atCpmsInfo;
} NAS_AT_OUTSIDE_RUNNING_CONTEXT_PART_ST;

/*
 * Structure: NAS_AT_SDT_AT_PART_ST
 * Description: PC回放工程，所有AT相关的全局变量通过以下消息结构发送
 * Message origin:
 */
typedef struct {
    VOS_MSG_HEADER
    AT_InterMsgIdUint32                    msgID;       /* 匹配AT_MSG_STRU消息中的ulMsgID  */
    VOS_UINT8                              type;        /* 之前是ucMsgIDs */
    VOS_UINT8                              reserved[3]; /* 在PACK(1)到PACK(4)调整中定义的保留字节 */
    NAS_AT_OUTSIDE_RUNNING_CONTEXT_PART_ST outsideCtx[MODEM_ID_BUTT];
} NAS_AT_SDT_AT_PART_ST;

/*
 * 结构说明: AT与MTA消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_MTA_MsgTypeUint32 msgType;
    AT_MTA_MSG_PROC_FUNC procMsgFunc;
} AT_PROC_MsgFromMta;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构说明: AT与MTA消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32           msgName;
    AT_MMA_MSG_PROC_FUNC procMsgFunc;
} AT_PROC_MsgFromMma;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构名    : AT_PROC_MSG_FROM_CALL_STRU
 * 结构说明  : AT与XCALL消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构说明: AT与CCM消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    TAF_CCM_MsgTypeUint32 msgName;
    AT_CCM_MSG_PROC_FUNC  procMsgFunc;
} AT_PROC_MsgFromCcm;
/*lint +e958 +e959 ;cause:64bit*/

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))
/*
 * 结构说明: AT与XPDS消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_XPDS_MsgTypeUint32 msgType;
    AT_MMA_MSG_PROC_FUNC  procMsgFunc;
} AT_PROC_MsgFromXpds;
/*lint +e958 +e959 ;cause:64bit*/
#endif

/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32       msgType;
    AT_VNAS_ProcFunc procMsgFunc;
}AT_VNAS_ProcMsgTbl;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构说明: AT与DMS消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32           msgName;
    AT_DMS_MSG_PROC_FUNC procMsgFunc;
} AT_PROC_MsgFromDms;
/*lint +e958 +e959 ;cause:64bit*/

/*
 * 结构说明: AT与DMS消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    DMS_SubscripEventIdUint32 eventId;
    VOS_UINT32                fixdedPartLen;
    TAF_ChkMsgLenFunc         chkFunc;
    AT_DMS_EVENT_PROC_FUNC    procEventFunc;
} AT_PROC_EventFromDms;
/*lint +e958 +e959 ;cause:64bit*/

#if (FEATURE_CALL_WAKELOCK == FEATURE_ON && FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_VOID AT_SetCsCallStateWakeLock(TAF_CCM_MsgTypeUint32 msgName);
#endif

VOS_UINT32 AT_FormatAtiCmdQryString(ModemIdUint16 modemId, DRV_AGENT_MsidQryCnf *drvAgentMsidQryCnf);
VOS_UINT32 AT_RcvDrvAgentMsidQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentVertimeQryRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentYjcxSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentYjcxQryCnf(struct MsgCB *msg);

VOS_UINT32 At_RcvAtCcMsgStateQryCnfProc(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentHardwareQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentFullHardwareQryRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetSimlockCnf(struct MsgCB *msg);

VOS_VOID   At_QryEonsUcs2RspProc(VOS_UINT8 indexNum, VOS_UINT8 opId, VOS_UINT32 result,
                                 TAF_MMA_EonsUcs2PlmnName *eonsUcs2PlmnName, TAF_MMA_EonsUcs2HnbName *eonsUcs2HNBName);
VOS_UINT32 AT_RcvMmaEonsUcs2Cnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCmmSetCmdRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvAtMmaUsimStatusInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentSetNvRestoreCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentQryNvRestoreRstCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentNvRestoreManuDefaultRsp(struct MsgCB *msg);

VOS_UINT32 AT_GetImeiValue(ModemIdUint16 modemId, VOS_UINT8 imei[TAF_PH_IMEI_LEN + 1], VOS_UINT8 emeiMaxLength);
VOS_BOOL   AT_IsSimLockPlmnInfoValid(VOS_VOID);

VOS_UINT32 AT_RcvDrvAgentSetGpioplRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQryGpioplRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentSetDatalockRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQryTbatvoltRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQryVersionRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQrySecuBootRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQrySfeatureRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQryProdtypeRsp(struct MsgCB *msg);

extern VOS_VOID At_CmdMsgDistr(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvDrvAgentSetAdcRsp(struct MsgCB *msg);

extern VOS_BOOL AT_E5CheckRight(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

VOS_UINT32 AT_RcvDrvAgentHkAdcGetRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentQryTbatRsp(struct MsgCB *msg);

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
VOS_UINT32 AT_RcvDrvAgentSetSpwordRsp(struct MsgCB *msg);
#endif
VOS_UINT32 AT_RcvDrvAgentSetSecuBootRsp(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaCipherInfoQueryCnf(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMmaLocInfoQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentNvBackupStatQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentNandBadBlockQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentNandDevInfoQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentChipTempQryRsp(struct MsgCB *msg);


VOS_UINT32 AT_RcvDrvAgentSetMaxLockTmsRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentSetApSimstRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentHukSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentFacAuthPubkeySetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentIdentifyStartSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentIdentifyEndSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSimlockDataWriteSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentPhoneSimlockInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSimlockDataReadQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentPhonePhynumSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentGetSimlockEncryptIdQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentOpwordSetCnf(struct MsgCB *msg);

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_VOID   At_RcvVcI2sTestCnfProc(MN_AT_IndEvt *event);
VOS_UINT32 AT_RcvVcI2sTestRsltIndProc(struct MsgCB *msg);
#endif

extern VOS_UINT32 AT_RcvMtaCposSetCnf(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMtaCposrInd(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMtaXcposrRptInd(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMtaCgpsClockSetCnf(struct MsgCB *msg);
extern VOS_VOID   AT_ProcImsaMsg(struct MsgCB *msg);

extern VOS_VOID AT_Rpt_NV_Read(VOS_VOID);

extern VOS_UINT32 AT_RcvMtaSimlockUnlockSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQryNmrCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaWrrAutotestQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrCellinfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrMeanrptQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrCellSrhSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrCellSrhQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrFreqLockSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrFreqLockQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrRrcVersionSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaWrrRrcVersionQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaGrrAutotestQryCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMtaDelCellEntityCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaAcInfoQueryCnf(struct MsgCB *msg);


VOS_UINT32 AT_RcvMtaBodySarSetCnf(struct MsgCB *msg);

extern VOS_VOID   AT_ReportResetCmd(AT_ResetReportCauseUint32 cause);
extern VOS_VOID   AT_StopAllTimer(VOS_VOID);
extern VOS_VOID   AT_ResetParseCtx(VOS_VOID);
extern VOS_VOID   AT_ResetClientTab(VOS_VOID);
extern VOS_VOID   AT_ResetOtherCtx(VOS_VOID);
extern VOS_UINT32 AT_RcvCcpuResetStartInd(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvCcpuResetEndInd(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvHifiResetBeginInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvHifiResetEndInd(struct MsgCB *msg);

#if (VOS_WIN32 == VOS_OS_VER)
extern VOS_UINT32 At_PidInit(enum VOS_InitPhaseDefine phase);
#endif

VOS_UINT32 AT_RcvMtaQryCurcCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSetUnsolicitedRptCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaQryUnsolicitedRptCnf(struct MsgCB *msg);
VOS_UINT32 AT_ProcMtaUnsolicitedRptQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg);

VOS_UINT32 AT_ProcCerssiInfoQuery(struct MsgCB *msg);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_ProcCserssiInfoQuery(struct MsgCB *msg);
#endif

VOS_INT16  AT_ConvertCerssiRssiToCesqRxlev(VOS_INT16 rssiValue);
VOS_INT16  AT_ConvertCerssiRscpToCesqRscp(VOS_INT16 rscpValue);
VOS_INT8   AT_ConvertCerssiEcnoToCesqEcno(VOS_INT8 ecno);
VOS_INT16  AT_ConvertCerssiRsrqToCesqRsrq(VOS_INT16 rsrqValue);
VOS_INT16  AT_ConvertCerssiRsrpToCesqRsrp(VOS_INT16 rsrpValue);
VOS_UINT32 AT_ProcCesqInfoSet(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCerssiInfoQueryCnf(struct MsgCB *msg);

/*
 * 功能描述: 收到IMEI校验查询的处理
 */
VOS_UINT32 AT_RcvMtaImeiVerifyQryCnf(struct MsgCB *msg);
/*
 * 功能描述: 收到UE信息上报的处理
 */
VOS_UINT32 AT_RcvMtaCgsnQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCopnInfoQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetNCellMonitorCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaQryNCellMonitorCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNCellMonitorInd(struct MsgCB *msg);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMmaSimInsertRsp(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMtaRefclkfreqSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaRefclkfreqQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaRefclkfreqInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaNwSliceAuthCmdInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaNwSliceAuthRsltInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaHandleDectSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaHandleDectQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaPsTransferInd(struct MsgCB *msg);

#if (FEATURE_DSDS == FEATURE_ON)
VOS_UINT32 AT_RcvMtaPsProtectSetCnf(struct MsgCB *msg);
#endif
VOS_UINT32 AT_RcvMtaPhyInitCnf(struct MsgCB *msg);

#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_VOID AT_ProcDmsEscSeqInPppDataMode(AT_ClientIdUint16 clientId);
#endif

VOS_UINT32 AT_RcvMtaEcidSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaMipiInfoCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaMipiInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaSysCfgSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPhoneModeSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaDetachCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaAttachCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaAttachStatusQryCnf(struct MsgCB *msg);

#if (FEATURE_ECALL == FEATURE_ON)
VOS_UINT32 AT_RcvVcMsgSetMsdCnfProc(MN_AT_IndEvt *data);

VOS_UINT32 AT_RcvVcMsgQryMsdCnfProc(MN_AT_IndEvt *data);

VOS_UINT32 AT_RcvVcMsgQryEcallCfgCnfProc(MN_AT_IndEvt *data);

#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvMmaCdmaCsqSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaCdmaCsqQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaCdmaCsqInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaCLModInd(struct MsgCB *msg);


VOS_UINT32 AT_RcvMmaCFreqLockSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCFreqLockQueryCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaCTimeInd(struct MsgCB *msg);
VOS_UINT32        AT_RcvMmaClDbDomainStatusSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaClDbdomainStatusReportInd(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMtaXpassInfoInd(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaCFPlmnSetCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaCFPlmnQueryCnf(struct MsgCB *msg);

VOS_VOID AT_ReportQryPrefPlmnCmdPara(TAF_MMA_PrefPlmnQueryCnf *cpolQryCnf, AT_ModemNetCtx *netCtx, VOS_UINT16 *length,
                                     VOS_UINT32 indexNum, VOS_UINT32 loop);

VOS_VOID AT_ReportQryPrefPlmnCmd(TAF_MMA_PrefPlmnQueryCnf *cpolQryCnf, VOS_UINT32 *validPlmnNum, AT_ModemNetCtx *netCtx,
                                 VOS_UINT16 *length, VOS_UINT32 indexNum);

VOS_UINT32 AT_RcvMmaPrefPlmnSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPrefPlmnQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPrefPlmnTestCnf(struct MsgCB *msg);

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_RcvMmaImsSwitchSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaImsSwitchQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaVoiceDomainSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_VoiceDomainTransToOutputValue(TAF_MMA_VoiceDomainUint32 voiceDoman, VOS_UINT32 *value);

VOS_UINT32 AT_RcvMmaVoiceDomainQryCnf(struct MsgCB *msg);
#endif

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_RcvMmaImsDomainCfgSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaImsDomainCfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaRoamImsSupportSetCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvMmaCLImsCfgInfoInd(struct MsgCB *msg);
#endif
#endif

VOS_UINT32 AT_RcvMtaSetFemctrlCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCerssiSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPlmnSearchCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaPrefPlmnTypeSetCnf(struct MsgCB *msg);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern VOS_UINT32 AT_RcvMmaPowerDownCnf(struct MsgCB *msg);
#endif
extern VOS_UINT32 AT_RcvMmaPlmnListAbortCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaPhoneModeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaAcInfoChangeInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaEOPlmnSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaEOPlmnQryCnf(struct MsgCB *msg);

VOS_VOID AT_ReportNetScanInfo(VOS_UINT8 indexNum, VOS_UINT8 freqNum, TAF_MMA_NetScanInfo *netScanInfo);

VOS_UINT32 AT_RcvMmaNetScanCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaUserSrvStateQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPwrOnAndRegTimeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaSpnQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaMMPlmnInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaPlmnQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafMmaQuickStartSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafMmaAutoAttachSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafMmaSyscfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaSyscfgTestCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaQuickStartQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaCsnrQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaCsqQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaCsqlvlQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaTimeChangeInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaModeChangeInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaPlmnChangeInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafMmaCrpnQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCbcQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaHsQryCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvMmaHdrCsqInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaHdrCsqSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaHdrCsqQryCnf(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMmaAccessModeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCopsQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRegStateQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaAutoAttachQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaSysInfoQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaAnqueryQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaCsnrQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaCsqlvlQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaEHplmnInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaApHplmnQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaHplmnQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaSrvStatusInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRssiInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRegRejInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRegStatusInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPlmnSelectInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaImsiRefreshInd(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvMma1xChanSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMma1xChanQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCVerQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaStateQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCHverQueryCnf(struct MsgCB *msg);
#endif
#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_RcvMtaSetFrCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSib16TimeUpdateInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaAccessStratumRelInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaRsInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaEccStatusInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaEccCfgCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaErrcCapCfgCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaErrcCapQryCnf(struct MsgCB *msg);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvMtaLendcQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLendcInfoInd(struct MsgCB *msg);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMtaNrFreqLockSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrFreqLockQryCnf(struct MsgCB *msg);
#endif

#endif

VOS_UINT32 AT_RcvMtaPseucellInfoSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaChrAlarmRlatCfgSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaClearHistoryFreqCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaQuitCallBackCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaEmcCallBackNtf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaQryEmcCallBackCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvMmaCLocInfoQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCSidInd(struct MsgCB *msg);

/* 移出CDMA编译开关 */
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMmaSetCSidListCnf(struct MsgCB *msg);
#endif

/* 移出CDMA编译开关 */

VOS_UINT32 AT_RcvMtaMeidSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaMeidQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaQryCurrSidNidCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRoamingModeSwitchInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaCombinedModeSwitchInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaRatCombinedModeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaIccAppTypeSwitchInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCtRoamInfoCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCtRoamingInfoChgInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCtOosCountCnf(struct MsgCB *msg);

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON)
VOS_UINT32 AT_RcvTafCcmEncryptCallCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEccCapQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEncryptCallInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEccRemoteCtrlInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmRemoteCtrlAnsCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEccCapSetCnf(struct MsgCB *msg);
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
VOS_UINT32 AT_RcvTafCcmSetEccTestModeCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryEccTestModeCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryEccRandomCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmQryEccKmcCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmSetEccKmcCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmEncryptedVoiceDataInd(struct MsgCB *msg);
#endif
#endif
VOS_UINT32 AT_RcvMmaClocinfoInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmPrivacyModeSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmPrivacyModeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmPrivacyModeInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCtRoamRptCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaPrlIdQryCnf(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMmaDplmnSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaDplmnQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaExchangeModemInfoCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaBorderInfoSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaBorderInfoQryCnf(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvMmaSrchedPlmnInfoInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaTransModeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaUECenterQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaUECenterSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQuickCardSwitchSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaInitLocInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPsEflociInfoQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPsEflociInfoSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaEflociInfoQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaEflociInfoSetCnf(struct MsgCB *msg);

#if (FEATURE_LTEV == FEATURE_ON)
VOS_UINT32 AT_RcvMtaVModeSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaVModeQryCnf(struct MsgCB *msg);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_VOID   AT_ProcMsgFromVnas(struct MsgCB *msg);
VOS_UINT32 AT_RcvVnasSrcidSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvVnasSrcidQryCnf(struct MsgCB *msg);
#endif
#endif

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
VOS_UINT32 AT_RcvMtaSetM2MFreqLockCnf(struct MsgCB *pmsg);
VOS_UINT32 AT_RcvMtaQryM2MFreqLockCnf(struct MsgCB *pmsg);
#endif

VOS_UINT32 AT_RcvMtaSetXCposrCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQryXcposrCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetXcposrRptCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQryXcposrRptCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetSensorCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetScreenCnf(struct MsgCB *msg);

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMtaUartTestCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaUartTestRsltInd(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvFratIgnitionQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvFratIgnitionSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetModemTimeCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaAfcClkUnlockCauseInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetBestFreqCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaBestFreqInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaQryBestFreqCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaModemCapUpdateCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaPacspQryCnf(struct MsgCB *msg);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_RcvMtaLteCategoryInfoInd(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMtaPhyCommAckInd(struct MsgCB *msg);

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)
VOS_UINT32 AT_RcvMtaRcmDcxoSampleQryCnf(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvMtaCommBoosterInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaCommBoosterSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaCommBoosterQueryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvGameModeSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaNvLoadSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaMtReattachInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCindSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMmaCemodeSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaCemodeQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaPseudBtsSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSubClfSparamSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSubClfSparamQryCnf(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_RcvCdmaModemSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvCdmaCapResumeSetCnf(struct MsgCB *msg);

#endif
VOS_UINT32 AT_RcvMmaRejinfoQryCnf(struct MsgCB *msg);
VOS_UINT16 AT_PrintRejinfo(VOS_UINT16 length, TAF_PH_RegRejInfo *rejinfo);

VOS_UINT32 AT_RcvMmaElevatorStateInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmCnapQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmCSChannelInfoQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmChannelInfoInd(struct MsgCB *evtInfo);

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 At_RcvTafCcmCallModifyCnf(struct MsgCB *msg);

VOS_UINT32 At_RcvTafCcmCallAnswerRemoteModifyCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmQryEconfCalledInfoCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvTafCcmCallModifyStatusInd(struct MsgCB *msg);
#endif

VOS_VOID At_TafAndDmsMsgProc(struct MsgCB *msg);
TAF_VOID At_PppMsgProc(struct MsgCB *msg);
VOS_VOID AT_RcvNdisMsg(struct MsgCB *msg);
VOS_VOID At_HPAMsgProc(struct MsgCB *msg);
VOS_VOID At_GHPAMsgProc(struct MsgCB *msg);
TAF_VOID At_MmaMsgProc(struct MsgCB *msg);
VOS_VOID At_ProcMsgFromDrvAgent(struct MsgCB *msg);
VOS_VOID At_ProcMsgFromVc(struct MsgCB *msg);
VOS_VOID At_ProcMsgFromCc(struct MsgCB *msg);
VOS_VOID At_RcvRnicMsg(struct MsgCB *msg);
TAF_VOID AT_RabmMsgProc(struct MsgCB *msg);
VOS_VOID At_ProcMtaMsg(struct MsgCB *msg);
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_VOID At_CHPAMsgProc(struct MsgCB *msg);
#endif

VOS_VOID At_CcmMsgProc(struct MsgCB *msg);

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
extern VOS_UINT32 AT_GetTdsFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
extern VOS_VOID AT_GetNrFeatureInfo(MTA_NRRC_BandInfo nrBandInfo, AT_FEATURE_SUPPORT_ST *feATure);

VOS_UINT32 AT_RcvMta5gNssaiSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMta5gNssaiQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaLadnRptSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLadnInfoQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaLadnInfoInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaPendingNssaiInd(struct MsgCB *msg);
VOS_UINT16 AT_PrintLadnDnn(VOS_UINT8 dnnNum, VOS_UINT8 dnnList[][PS_MAX_APN_LEN], VOS_UINT16 length);
VOS_VOID   AT_ConvertNwDnnToString(VOS_UINT8 *inputDnn, VOS_UINT8 inputLen, VOS_UINT8 *outputDnn, VOS_UINT8 outputLen);
#endif

#if ((FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON))
VOS_UINT32 AT_RcvMtaHsrcellInfoInd(struct MsgCB *msg);
#endif /* (FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON) */
#if (FEATURE_LTEV == FEATURE_ON)
VOS_VOID AT_GetLtevFeatureInfo(AT_FEATURE_SUPPORT_ST *featureSupport, MTA_AT_SfeatureQryCnf *featureQryCnf);
#endif

VOS_VOID AT_ProcDmsMsg(struct MsgCB *msg);
VOS_UINT32 AT_ChkDmsSubscripEventNotify(const MSG_Header *msg);
VOS_UINT32 AT_RcvDmsSubscripEventNotify(struct MsgCB *msg);
VOS_UINT32 AT_RcvDmsLowWaterMarkEvent(struct MsgCB *msg);
VOS_UINT32 AT_RcvDmsNcmConnBreakEvent(struct MsgCB *msg);
VOS_UINT32 AT_RcvDmsUsbDisconnectEvent(struct MsgCB *msg);
VOS_UINT32 AT_RcvDmsEscapeSequenceEvent(struct MsgCB *msg);
VOS_UINT32 AT_RcvDmsDtrDeassertedEvent(struct MsgCB *msg);

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
VOS_UINT32 AT_RcvDmsUartInitEvent(struct MsgCB *msg);
#endif
VOS_UINT32 AT_RcvMtaNvRefreshNtf(struct MsgCB *msg);
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdMsgProc.h */
