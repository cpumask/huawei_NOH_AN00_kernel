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
#ifndef _AT_MT_MSG_PROC_H_
#define _AT_MT_MSG_PROC_H_

#include "at_mt_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
VOS_VOID At_WTxCltIndProc(WPHY_AT_TxCltInd *msg);

VOS_UINT32 AT_RcvMtaRficSsiRdQryCnf(struct MsgCB *msg);

VOS_UINT32 At_MipiRdCnfProc(HPA_AT_MipiRdCnf *msg);
VOS_UINT32 At_MipiWrCnfProc(HPA_AT_MipiWrCnf *msg);
VOS_UINT32 At_SsiWrCnfProc(HPA_AT_SsiWrCnf *msg);
VOS_UINT32 At_SsiRdCnfProc(HPA_AT_SsiRdCnf *msg);

extern VOS_UINT32 At_PdmCtrlCnfProc(HPA_AT_PdmCtrlCnf *msg);

VOS_UINT32 At_SendContinuesWaveOnToHPA(VOS_UINT16 power, VOS_UINT8 indexNum);
VOS_VOID   At_RfCfgCnfReturnSuccProc(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetFchanRspErr(DRV_AGENT_FchanSetErrorUint32 result);

VOS_UINT32 At_RfCfgCnfReturnErrProc(VOS_UINT8 indexNum);

VOS_UINT32 AT_RcvDrvAgentSetFchanRsp(struct MsgCB *msg);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 At_SendContinuesWaveOnToCHPA(VOS_UINT8 ctrlType, VOS_UINT16 power);
#endif

VOS_UINT32 At_SendTxOnOffToCHPA(VOS_UINT8 switchStatus);

VOS_UINT32 At_SendRxOnOffToCHPA(VOS_UINT32 rxSwitch);

VOS_VOID At_HpaRfCfgCnfProc(HPA_AT_RfCfgCnf *msg);

VOS_VOID At_RfRssiIndProc(HPA_AT_RfRxRssiInd *msg);

VOS_VOID   At_CHpaRfCfgCnfProc(CHPA_AT_RfCfgCnf *msg);
VOS_UINT32 AT_RcvMtaPowerDetQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentTseLrfSetRsp(struct MsgCB *msg);
VOS_UINT32 AT_RcvDrvAgentSetFchanRsp(struct MsgCB *msg);

#else
VOS_UINT32 At_SndRxOnOffReq(VOS_VOID);
VOS_UINT32 At_SndTxOnOffReq(VOS_UINT16 powerDetFlg);
VOS_UINT32 At_LoadPhy(VOS_VOID);
VOS_VOID   AT_ProcCbtMsg(struct MsgCB *msg);
VOS_VOID   AT_ProcUeCbtMsg(struct MsgCB *msg);
VOS_VOID   AT_ProcDspIdleMsg(struct MsgCB *msg);
VOS_VOID   AT_ProcBbicMsg(struct MsgCB *msg);
VOS_UINT32 AT_ProcSetWorkModeCnf(struct MsgCB *msg);
VOS_UINT32 AT_ProcTxCnf(struct MsgCB *msg);
VOS_UINT32 AT_ProcRxCnf(struct MsgCB *msg);
VOS_UINT32 AT_ProcPowerDetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SndBbicCalMipiReadReq(VOS_UINT32 mipiPortSel, VOS_UINT32 secondaryId, VOS_UINT32 regAddr,
                                    VOS_UINT32 byteCnt, VOS_UINT32 readSpeed);
VOS_UINT32 AT_SndBbicCalMipiWriteReq(VOS_UINT32 mipiPortSel, VOS_UINT32 secondaryId, VOS_UINT32 regAddr,
                                     VOS_UINT32 byteCnt, VOS_UINT32 value);
VOS_UINT32 AT_RcvBbicCalMipiRedCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvBbicCalMipiWriteCnf(struct MsgCB *msg);
VOS_UINT32 AT_SndBbicPllStatusReq(VOS_VOID);
VOS_UINT32 At_RcvBbicPllStatusCnf(struct MsgCB *msg);
VOS_UINT32 AT_SndBbicRssiReq(VOS_VOID);
VOS_UINT32 At_RcvBbicRssiInd(struct MsgCB *msg);
VOS_UINT32 AT_SndBbicCalSetDpdtReq(BBIC_DPDT_OPERTYPE_ENUM_UINT16 operType, VOS_UINT32 value, VOS_UINT32 workType);
VOS_UINT32 AT_RcvBbicCalSetDpdtCnf(struct MsgCB *msg);
VOS_UINT32 AT_SndBbicCalQryFtemprptReq(INT16 channelNum);
VOS_UINT32 AT_RcvBbicCalQryFtemprptCnf(struct MsgCB *msg);
VOS_UINT32 At_SndDcxoReq(VOS_VOID);
VOS_UINT32 At_RcvBbicCalDcxoCnf(struct MsgCB *msg);

VOS_UINT32 At_SndGsmTxOnOffReq_ModulatedWave(VOS_UINT16 powerDetFlg);
VOS_UINT32 AT_ProcPowerDetCnf_ModulateWave(struct MsgCB *msg);
VOS_UINT32 At_SndUeCbtRfIcMemTestReq(VOS_VOID);
VOS_UINT32 At_ProcUeCbtRfIcMemTestCnf(struct MsgCB *msg);
VOS_UINT32 At_SndDspIdleSerdesRtReq(VOS_VOID);
VOS_UINT32 At_ProcDspIdleSerdesRtCnf(struct MsgCB *msg);
VOS_UINT32 At_SndDspIdleSltTestReq(VOS_VOID);
VOS_UINT32 AT_ProcDspIdleSltTestCnf(struct MsgCB *msg);
VOS_UINT32 At_SndBbicCalSetTrxTasReq(VOS_UINT16 trxTasValue);
VOS_UINT32 AT_RcvBbicCalSetTrxTasCnf(struct MsgCB *msg);
VOS_UINT32 At_RcvUeCbtRfIcIdExQryCnf(struct MsgCB *msg);
VOS_UINT32 At_SndUeCbtRfIcIdExQryReq(VOS_VOID);
VOS_UINT32 AT_ProcGsmTxCnf_ModulateWave(struct MsgCB *msg);
VOS_UINT32 At_SndDspIdleSerdesTestAsyncReq(VOS_VOID);
VOS_UINT32 At_ProcDspIdleSerdesAsyncTestCnf(struct MsgCB *rcvMsg);

#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
