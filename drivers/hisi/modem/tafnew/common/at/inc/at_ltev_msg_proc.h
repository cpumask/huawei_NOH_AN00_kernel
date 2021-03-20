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

#ifndef _AT_LTEV_MSG_PROC_H_
#define _AT_LTEV_MSG_PROC_H_

#include "taf_v2x_api.h"
#if (FEATURE_LTEV == FEATURE_ON)
#if (FEATURE_LTEV_WL == FEATURE_ON)
#include "at_mta_interface.h"
#endif
#endif
#include "AtParse.h"
#include "at_mt_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#if (FEATURE_LTEV == FEATURE_ON)
/* AT与VRRC模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_VRRC_MsgProcFunc)(MN_AT_IndEvt *msg);

/* AT与VMAC模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_VMAC_MsgProcFunc)(MN_AT_IndEvt *msg);

/* AT与VTC模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_VTC_MsgProcFunc)(VTC_AT_MsgCnf *msg);

#define AT_SET_PHYR_TX_POWER_MAX 23
#define AT_SET_PHYR_TX_POWER_MIN (-30)

#define AT_UINT_BYTES_LEN 8
#define AT_UINT_DISPLACE_MAX 7

#define AT_RPPCFG_BITMAP_LEN_MIN_VALUE 16
#define AT_RPPCFG_BITMAP_LEN_MID_VALUE 20
#define AT_RPPCFG_BITMAP_LEN_MAX_VALUE 100

#define AT_RPPCFG_GROUP_RESPOOL_NUM 8
#define AT_RPPCFG_GROUP_RESPOOL_NUM_SUM 16

#define AT_TX_POWER_PARA_LEN_MAX 7
#define AT_SET_TX_POWER_MAX 240
#define AT_SET_TX_POWER_MIN (-496)
/* AT与VSYNC模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_VSYNC_MsgProcFunc)(MN_AT_IndEvt *msg);
/* AT与VPDCP模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_VPDCP_MsgProcFunc)(MN_AT_IndEvt *msg);

/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_VRRC_MsgTypeUint16 msgName;
    VOS_UINT16            reserved;
    AT_VRRC_MsgProcFunc   procMsgFunc;
} AT_VRRC_ProcMsgTbl;

typedef struct {
    AT_VMAC_MsgTypeUint16 msgName;
    VOS_UINT16            reserved;
    AT_VMAC_MsgProcFunc   procMsgFunc;
} AT_VMAC_ProcMsgTbl;

typedef struct {
    AT_VRRC_MsgTypeUint16 msgName;
    VOS_UINT16            reserved;
    AT_VSYNC_MsgProcFunc  procMsgFunc;
} AT_VSYNC_ProcMsgTbl;

typedef struct {
    AT_VPDCP_MsgTypeUint16 msgName;
    VOS_UINT16             reserved;
    AT_VPDCP_MsgProcFunc   procMsgFunc;
} AT_VPDCP_ProcMsgTbl;

typedef struct {
    AT_VTC_MsgTypeUint32 msgName;
    AT_VTC_MsgProcFunc   procMsgFunc;
} AT_VTC_ProcMsgTbl;
/*lint +e958 +e959 ;cause:64bit*/
typedef struct {
    VTC_AT_ResultCodeUint32 vtcResultCode;
    AT_RreturnCodeUint32    atReturnCode;
} AT_ConvertVtcResultCodeTbl;

typedef struct {
    VRRC_AT_ResultCodeUint32 vrrcResultCode;
    AT_RreturnCodeUint32     atReturnCode;
} AT_ConvertVrrcResultCodeTbl;

VOS_VOID   AT_ProcMsgFromVrrc(struct MsgCB *msg);
VOS_VOID   AT_ProcMsgFromVmac(struct MsgCB *msg);
VOS_VOID   AT_ProcMsgFromVtc(struct MsgCB *msg);
VOS_VOID   AT_ProcMsgFromVpdcp(struct MsgCB *msg);
VOS_UINT32 AT_RcvTestModeActiveStateSetCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvTestModeActiveStateQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvTestModeECloseStateSetCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvTestModeECloseStateQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvResetUtcTimeSetCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvSendingDataActionSetCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvSendingDataActionQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvSidelinkPacketCounterQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvVtcCbrQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvVmacCbrQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvCbrRptInd(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRssiRptInd(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncSourceQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncSourceRptSetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncSourceRptQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncSourceRptInd(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncModeQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvSyncModeSetCnf(MN_AT_IndEvt *msg);

VOS_UINT32 AT_RcvVrrcMsgSetSLInfoCnfProc(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvVrrcMsgQrySLInfoCnfProc(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuPhyrSetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuPhyrQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvPtrRptSetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvPtrRptQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvVrrcMsgSetRppCfgCnfProc(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvGnssTestStartCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvGnssInfoQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvGnssDebugInfoGetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvVrrcMsgQryRppCfgCnfProc(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvV2xResPoolCfgState(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuVphyStatQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuVphyStatClrSetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuVsnrRsrpQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuV2xRssiQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvVrssiQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvPc5PktSndCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvVrrcMsgSetTxPowerCnfProc(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvPc5PktSndRcvState(MN_AT_IndEvt *msg);
VOS_VOID   AT_InitBitMap(VOS_UINT8 *bitmap, VOS_UINT8 bitmapLen, VOS_UINT8 *scBitmap, VOS_UINT8 scLen);

VOS_UINT32 VRRC_SetGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_QryGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_QrySideLinkInfo(MN_CLIENT_ID_T clientID, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_SetSideLinkInfo(VOS_UINT32 senderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                const AT_VRRC_Slinfo *setParams);
VOS_UINT32 VRRC_QryRsuPhyr(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_SetRsuPhyr(VOS_UINT32 senderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                           const AT_VRRC_PhyrSet *setParams);
VOS_UINT32 VRRC_QryRsuVphyStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_SetRsuVPhyStatClr(VOS_UINT32 senderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                  const AT_VRRC_VphystatClrSet *setParams);
VOS_UINT32 VRRC_QryRsuVSnrRsrp(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_QryRsuV2xRssi(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VMAC_QryVRssi(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_SetV2xResPoolPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VRRC_RppcfgSet *setParams);
VOS_UINT32 VRRC_QryV2xResPoolPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VRRC_RppcfgQry *qryParams);
VOS_UINT32 VPDCP_QryPtrRpt(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VPDCP_SetPtrRpt(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_GetGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 AT_RcvTestModeModifyL2IdSetCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 AT_RcvTestModeModifyL2IdQryCnf(VTC_AT_MsgCnf *msg);
VOS_UINT32 VRRC_SetTxPower(VOS_UINT32 ulSenderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                           const AT_VRRC_Set_Tx_PowerPara *setParams);

#if (FEATURE_LTEV_WL == FEATURE_ON)
VOS_VOID   AT_ProcMsgFromVsync(struct MsgCB *msg);
VOS_UINT32 AT_RcvPc5SyncSetCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvPc5SyncQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuMcsCfgCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvMtaFileWrCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMtaFileRdCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetLedTestRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetGpioTestRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentQryAntTestRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetHkadcTestRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentGpioQryRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentGpioSetRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentBootModeSetRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetGpioHeatSetRsp(struct MsgCB *msg);
extern int stmmac_phy_loop_test(void);
extern int stmmac_ge_loop_test(void);
extern int stmmac_phy_medium_autodetect(int flag);
extern int stmmac_sfp_loop_test_result(void);
VOS_UINT32 AT_RcvRsuSyncStatQryCnf(MN_AT_IndEvt *msg);
VOS_UINT32 AT_RcvRsuGnssSyncStatQryCnf(MN_AT_IndEvt *msg);

VOS_UINT32 VRRC_QryRsuSyncStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VRRC_QryGnssSyncStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_UINT32 VMAC_SetMcgCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VMAC_McgCfgSet *setParams);
VOS_UINT32 VMAC_SetDstIdTMPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VMAC_DstidtmSet *setParams);
VOS_UINT32 VRRC_SYNC_SetCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VRRC_Pc5syncSet *setParams);
VOS_UINT32 VRRC_SYNC_QryCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_VOID AT_GetStrAfterchr(VOS_INT8 *src, VOS_INT8 c, VOS_INT8 *dst);
VOS_VOID AT_GetStrBeforechr(VOS_INT8 *src, VOS_INT8 c, VOS_INT8 *dst);
VOS_VOID AT_GetIndexNum(VOS_UINT8 *data, VOS_UINT32 len, VOS_UINT32 *indexMin, VOS_UINT32 *indexMax);
VOS_UINT32 TAF_GetFileSize(FILE *fp, VOS_UINT32 *fileSize);
VOS_UINT32 AT_IndexRdProc(AT_MTA_FileRdReq *data, VOS_UINT8 *rec, VOS_UINT8 recSize);
VOS_INT32 IsDigit(VOS_INT8 ch);
#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
