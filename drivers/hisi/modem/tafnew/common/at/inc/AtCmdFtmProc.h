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

#ifndef __ATCMDFTMPROC_H__
#define __ATCMDFTMPROC_H__

#include "AtCtx.h"
#include "AtParse.h"

#include "at_mta_interface.h"
#include "taf_drv_agent.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* LOG3.5�Ķ˿ڶ���: USB */
#define AT_LOG_PORT_USB 0

/* LOG3.5�Ķ˿ڶ���: VCOM */
#define AT_LOG_PORT_VCOM 1

#define AT_JAM_DETECT_GSM_THRESHOLD_MAX 70
#define AT_JAM_DETECT_GSM_FREQNUM_MAX 255
#define AT_JAM_DETECT_WCDMA_RSSI_THRESHOLD_MAX 70
#define AT_JAM_DETECT_WCDMA_RSSI_PERCENT_MAX 100
#define AT_JAM_DETECT_WCDMA_PSCH_THRESHOLD_MAX 65535
#define AT_JAM_DETECT_WCDMA_PSCH_PERCENT_MAX 100
#define AT_JAM_DETECT_LTE_RSSI_THRESHOLD_MAX 120
#define AT_JAM_DETECT_LTE_RSSI_PERCENT_MAX 100
#define AT_JAM_DETECT_LTE_PSSRATIO_THRESHOLD_MAX 1000
#define AT_JAM_DETECT_LTE_PSSRATIO_PERCENT_MAX 100
#define AT_JAM_DETECT_GSM_PARA_NUM 3
#define AT_JAM_DETECT_WL_PARA_NUM 5

#define AT_HCSQ_LEVEL_MIN 0
#define AT_HCSQ_VALUE_INVALID 255
#define AT_HCSQ_RSSI_VALUE_MIN (-120)
#define AT_HCSQ_RSSI_VALUE_MAX (-25)
#define AT_HCSQ_RSSI_LEVEL_MAX 96

#define AT_NETMON_PLMN_STRING_MAX_LENGTH 10
#define AT_NETMON_GSM_RX_QUALITY_INVALID_VALUE 99
#define AT_NETMON_GSM_RSSI_INVALID_VALUE (-500)
#define AT_NETMON_UTRAN_FDD_RSCP_INVALID_VALUE 0
#define AT_NETMON_UTRAN_FDD_RSSI_INVALID_VALUE 0
#define AT_NETMON_UTRAN_FDD_ECN0_INVALID_VALUE 0

VOS_UINT32 At_SetLogPortPara(VOS_UINT8 indexNum);

VOS_UINT32 At_QryLogPortPara(VOS_UINT8 indexNum);

VOS_UINT32 At_QryLogCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryLogCpsPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetDpdtTestFlagPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaSetDpdtTestFlagCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSetDpdtValueCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaQryDpdtValueCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetJDCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryJDCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestJDCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetJDSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryJDSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaSetJamDetectCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaQryJamDetectCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaJamDetectInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetGFreqLockCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetGFreqLock(VOS_UINT8 indexNum);

TAF_UINT32 AT_QryGFreqLock(TAF_UINT8 indexNum);

VOS_UINT32 AT_RcvMtaGFreqLockQryCnf(struct MsgCB *msg);

VOS_UINT32 At_SetNetMonSCellPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetNetMonNCellPara(VOS_UINT8 indexNum);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 At_SetNetMonSSCellPara(VOS_UINT8 atIndex);

VOS_UINT32 AT_RcvMtaSetNetMonSSCellCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaNrSsbIdQryCnf(struct MsgCB *msg);

#endif
VOS_UINT32 AT_RcvMtaSetNetMonSCellCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaSetNetMonNCellCnf(struct MsgCB *msg);
VOS_VOID   AT_NetMonFmtGsmSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length);
VOS_VOID   AT_NetMonFmtUtranFddSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length);
VOS_VOID   AT_NetMonFmtGsmNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen);
VOS_VOID   AT_NetMonFmtUtranFddNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen);
VOS_VOID   AT_NetMonFmtPlmnId(VOS_UINT32 mcc, VOS_UINT32 mnc, VOS_CHAR *pstrPlmn, VOS_UINT16 *length);

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
VOS_VOID AT_NetMonFmtUtranTddSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length);
VOS_VOID AT_NetMonFmtUtranTddNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen);
#endif

#if (FEATURE_LTE == FEATURE_ON)
VOS_VOID AT_NetMonFmtEutranSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length);
VOS_VOID AT_NetMonFmtEutranNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen);
VOS_VOID AT_FormatRsrp(VOS_UINT8 indexNum, MTA_AT_RsInfoQryCnf *rsInfoQryCnf);
VOS_VOID AT_FormatRsrq(VOS_UINT8 indexNum, MTA_AT_RsInfoQryCnf *rsInfoQryCnf);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_VOID AT_NetMonFmtNrSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length);

VOS_VOID AT_NetMonFmtNrSSCellData(MTA_AT_NetmonCellInfo *cellInfo, VOS_UINT16 *outLen);

VOS_VOID AT_NetMonFmtNrNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen);

VOS_VOID AT_FmtNrSsbIdData(MTA_AT_NrSsbIdQryCnf *ssbIdInfo, VOS_UINT16 *outLen);
#endif

VOS_VOID AT_FormatGasAtCmdRslt(MTA_AT_GasAutotestQryRslt *atCmdRslt);

VOS_UINT32 AT_SetEmRssiCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEmRssiCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetEmRssiRptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEmRssiRptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaEmRssiCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaEmRssiCfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaEmRssiRptSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaEmRssiRptQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaEmRssiRptInd(struct MsgCB *msg);

VOS_UINT32 AT_SetLtePwrDissPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestLtePwrDissPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaAtLtePwrDissSetCnf(struct MsgCB *msg);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern VOS_UINT32 AT_RcvMtaLowPwrModeSetCnf(struct MsgCB *msg);
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
extern VOS_UINT32 AT_RcvMtaNrrcCapCfgCnf(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMtaNrrcCapQryCnf(struct MsgCB *msg);

#ifndef MODEM_FUSION_VERSION
extern VOS_UINT32 AT_RcvMtaNrPwrCtrlSetCnf(struct MsgCB *msg);
#endif
extern VOS_UINT32 AT_RcvMtaSetTrxTasCnf(struct MsgCB *msg);
extern VOS_UINT32 AT_RcvMtaQryTrxTasCnf(struct MsgCB *msg);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdFtmProc.h */
