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

#ifndef __ATCMDIMSPROC_H__
#define __ATCMDIMSPROC_H__

#include "AtCtx.h"
#include "AtParse.h"
#include "at_imsa_interface.h"
#include "AtSetParaCmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#if (FEATURE_IMS == FEATURE_ON)
#define AT_IMS_PAYLOAD_TYPE_RANGE_MAX 0x7f
#define AT_IMS_PAYLOAD_TYPE_RANGE_MIN 0x60
#define AT_IMS_AMR_WB_MODE_MAX 8
#define AT_IMS_TIMER_DATA_RANGE_MAX 128000 /* 定时器最大时长:单位ms */

/* AT与IMSA模块间消息处理函数指针 */
typedef VOS_UINT32 (*AT_IMSA_MSG_PROC_FUNC)(struct MsgCB *pMsg);

/*
 * 结构说明: AT与IMSA消息与对应处理函数的结构
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_IMSA_MsgTypeUint32 msgId;
    AT_IMSA_MSG_PROC_FUNC procMsgFunc;
} AT_IMSA_MsgProFunc;
/*lint +e958 +e959 ;cause:64bit*/


VOS_UINT32 AT_RcvImsaCiregSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCiregQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCirepSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCirepQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaImpuSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetVolteImpiPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_SetVolteDomainPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_RcvImsaVolteImpiQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaVolteDomainQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCirephInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCirepiInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCireguInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCcwaiSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaEmcPdnActivateInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaEmcPdnDeactivateInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaEmcPdnActivateCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaEmcPdnDeactivateCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaMtStateInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaImsCtrlMsg(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaFusionCallCtrlMsg(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaImsRegDomainQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaCallEncryptSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaRoamImsServiceQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaRatHandoverInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvSrvStatusUpdateInd(struct MsgCB *msg);

VOS_VOID At_FillDmdynNumParaInCmd(VOS_UINT16 *length, VOS_UINT32 value, VOS_UINT32 valueValidFlg);

VOS_VOID At_FillDmdynStrParaInCmd(VOS_UINT16 *length, VOS_CHAR *pcValue, VOS_UINT32 valueValidFlg,
                                  VOS_UINT32 lastParaFlg);

VOS_VOID At_FillIpv6AddrInCmd(VOS_UINT16 *length, VOS_UINT8 *addr, VOS_UINT32 addrValidFlg, VOS_UINT32 sipPort,
                              VOS_UINT32 portValidFlg);

VOS_VOID At_FillIpv4AddrInCmd(VOS_UINT16 *length, VOS_UINT8 *addr, VOS_UINT32 addrValidFlg, VOS_UINT32 sipPort,
                              VOS_UINT32 portValidFlg);

VOS_UINT32 AT_RcvImsaPcscfSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaPcscfQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaDmDynSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaDmDynQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaDmcnInd(struct MsgCB *msg);

VOS_UINT32 AT_ParseIpv6PcscfData(VOS_UINT32 *addrExistFlg, VOS_UINT8 *ipAddr, VOS_UINT32 *portExistFlg,
                                 VOS_UINT32 *portNum, VOS_UINT32 indexNum);

VOS_UINT32 AT_FillIpv6PcscfData(AT_IMSA_PcscfSetReq *pcscf);

VOS_UINT32 AT_ParseIpv4PcscfData(VOS_UINT32 *addrExistFlg, VOS_UINT8 *ipAddr, VOS_UINT32 ipBufLen,
                                 VOS_UINT32 *portExistFlg, VOS_UINT32 *portNum, VOS_UINT32 indexNum);

VOS_UINT32 AT_FillIpv4PcscfData(AT_IMSA_PcscfSetReq *pcscf);

VOS_UINT32 AT_FillDataToPcscf(AT_IMSA_PcscfSetReq *pcscf);

VOS_UINT32 AT_SetImsPcscfPara(VOS_UINT8 indexNum);

VOS_UINT32 At_FillImsPayloadTypePara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum);

VOS_UINT32 At_FillImsAmrWbModePara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum);

VOS_UINT32 At_FillImsRtpPortPara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum);

VOS_UINT32 At_FillImsaNumericPara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum);

VOS_UINT32 At_FillImsaStrPara(VOS_CHAR *str, VOS_UINT32 *strValidFlg, VOS_UINT32 maxLen, VOS_UINT32 indexNum);

VOS_UINT32 AT_SetDmDynPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_QryImsPcscfPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryDmDynPara(VOS_UINT8 indexNum);

VOS_VOID   At_FillImsaTimerParaInCmd(VOS_UINT16 *length, VOS_UINT32 value, VOS_UINT32 valueValidFlg,
                                     VOS_UINT32 lastParaFlg);
VOS_UINT32 AT_FillImsTimerReqBitAndPara(VOS_UINT32 *value, VOS_UINT32 *bitOpValue, VOS_UINT32 indexNum);
VOS_UINT32 AT_FillImsTimerReqData(AT_IMSA_ImstimerSetReq *imsTimer);


VOS_UINT32 AT_RcvImsaImsTimerSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaImsTimerQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaImsPsiSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaImsPsiQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaDmUserQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetNickNamePara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryNickNamePara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaNickNameSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaNickNameQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaImsRegFailInd(struct MsgCB *msg);
VOS_UINT32 AT_SetBatteryInfoPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaBatteryInfoSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryBatteryInfoPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetVolteRegPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_SetImsVtCapCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaImsVideoCallCapSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetImsSmsCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsSmsCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMmaImsSmsCfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaImsSmsCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_SetImsVideoCallCancelPara(TAF_UINT8 indexNum);

VOS_UINT32 AT_SetImsRegErrRpt(TAF_UINT8 indexNum);

VOS_UINT32 AT_RcvImsaRegErrRptSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_QryImsRegErrRpt(TAF_UINT8 indexNum);

VOS_UINT32 AT_RcvImsaRegErrRptQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaRegErrRptInd(struct MsgCB *msg);

VOS_UINT32 AT_IsImsRegErrRptParaValid(struct MsgCB *msg);

VOS_UINT32 AT_SetImsSrvStatRptCfgPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaImsSrvStatRptSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryImsSrvStatRptCfgPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaImsSrvStatRptQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryImsSrvStatusPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaImsSrvStatusQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetEmcAIdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaEmcAIdSetCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaDmRcsCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaCallAltSrvInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvMmaRcsSwitchQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetUserAgentCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaUserAgentSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaImpuInd(struct MsgCB *msg);

VOS_UINT32 AT_SetViceCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaViceCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryViceCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaViceCfgQryCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaDialogNtfInd(struct MsgCB *msg);
VOS_UINT32 AT_SetRttCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRttModifyPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaRttCfgSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaRttModifySetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaRttEventInd(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaRttErrorInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvImsaSipPortSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaSipPortQryCnf(struct MsgCB *msg);
TAF_UINT32 At_SetSipTransPort(TAF_UINT8 indexNum);
VOS_UINT32 At_QrySipTransPort(VOS_UINT8 indexNum);
#if (FEATURE_ECALL == FEATURE_ON)
VOS_UINT32 AT_RcvImsaEcallContentTypeSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_RcvImsaEcallContentTypeQryCnf(struct MsgCB *msg);
#endif
#endif /* end of FEATURE_IMS */

VOS_UINT32 At_SetEflociInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryEflociInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetPsEflociInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryPsEflociInfoPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetImsIpCapPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaImsIpCapSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryImsIpCapPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvImsaImsIpCapQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetCacdcPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetErrcCapCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetErrcCapQryPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_RcvImsaImsUrspSetCnf(struct MsgCB *msg);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdImsProc.h */
