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

#include "AtCmdImsProc.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "taf_type_def.h"
#include "AtDataProc.h"
#include "AtDeviceCmd.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_IMS_PROC_C

#define AT_VIDEO_CALL_CAP_SWITCH 0
#define AT_VIDEO_CALL_CAP_CCWA 1

#define AT_IMSSMSCFG_PARA_NUM 4
#define AT_IMSSMSCFG_UTRAN_ENABLE 2
#define AT_IMSSMSCFG_GSM_ENABLE 3

#define AT_DMRCSCFG_PARA_NUM 2
#define AT_DMRCSCFG_FEATURETAG 0
#define AT_DMRCSCFG_DEVCFG 1

#define AT_USERAGENTCFG_PARA_NUM 6
#define AT_USERAGENTCFG_PARA1 0
#define AT_USERAGENTCFG_PARA2 1
#define AT_USERAGENTCFG_PARA3 2
#define AT_USERAGENTCFG_PARA4 3
#define AT_USERAGENTCFG_PARA5 4
#define AT_USERAGENTCFG_PARA6 5

#define AT_EFLOCIINFO_PARA_NUM 5
#define AT_EFLOCIINFO_PLMN 1
#define AT_EFLOCIINFO_TMSI 0
#define AT_EFLOCIINFO_LAC 2
#define AT_EFLOCIINFO_LOCATION_UPDATE_STATUS 3
#define AT_EFLOCIINFO_RFU 4
#define AT_EFLOCIINFO_PLMN_THIRD_PARA 3

#define AT_EFPSLOCIINFO_PARA_NUM 6
#define AT_EFPSLOCIINFO_PLMN 2
#define AT_EFPSLOCIINFO_LAC 3
#define AT_EFPSLOCIINFO_RAC 4
#define AT_EFPSLOCIINFO_ROUTING_AREA_UPDATE_STATUS 5
#define AT_EFPSLOCIINFO_PLMN_THIRD_PARA 3

#define AT_CACDC_ENAPPINDICATION 2

#define AT_ERRCCAPCFG_MAX_PARA_NUM 4
#define AT_ERRCCAPCFG_MIN_PARA_NUM 2
#define AT_ERRCCAPCFG_CONTROL_CAPA_3_PARA_NUM 3 /* ERRCCAPCFG命令控制能力为三个时的参数个数 */
#define AT_ERRCCAPCFG_PARA2 2
#define AT_ERRCCAPCFG_PARA3 3

#define AT_NICKNAME_NICK_NAME 0

#define AT_DMDYN_PARA_NUM 15
#define AT_DMDYN_PHONECONTEXT 13

#define AT_IMSPCSCF_PARA_NUM 7
#define AT_DMTIMER_PARA_NUM 14
#define AT_IMSVTCAPCFG_PARA_NUM 2
#define AT_RTTMODIFY_PARA_NUM 2
#define AT_IMSIPCAPCFG_PARA_NUM 2

#define AT_IMSPCSCF_IPV6_ADDRESS1_INDEX 1
#define AT_IMSPCSCF_IPV6_ADDRESS2_INDEX 2
#define AT_IMSPCSCF_IPV6_ADDRESS3_INDEX 3
#define AT_IMSPCSCF_IPV4_ADDRESS1_INDEX 4
#define AT_IMSPCSCF_IPV4_ADDRESS2_INDEX 5
#define AT_IMSPCSCF_IPV4_ADDRESS3_INDEX 6

#define AT_DMDYN_AWR_WB_OCTET_ALIGNED_INDEX 0
#define AT_DMDYN_AWR_WB_BANDWIDTH_ECT_INDEX 1
#define AT_DMDYN_AWR_OCTET_ALIGNED_INDEX 2
#define AT_DMDYN_AWR_BANDWIDTH_ECT_INDEX 3
#define AT_DMDYN_AWR_WB_MODE_INDEX 4
#define AT_DMDYN_DTMF_WB_INDEX 5
#define AT_DMDYN_DTMF_NB_INDEX 6
#define AT_DMDYN_SPEECH_START_INDEX 7
#define AT_DMDYN_SPEECH_END_INDEX 8
#define AT_DMDYN_VIDEO_START_INDEX 9
#define AT_DMDYN_VIDEO_END_INDEX 10
#define AT_DMDYN_REGRETRYBASETIME_INDEX 11
#define AT_DMDYN_REGRETRYMAXTIME_INDEX 12
#define AT_DMDYN_PHONECONTEXT_INDEX 13
#define AT_DMDYN_PUBLIC_USER_ID_INDEX 14

#define AT_DMTIMER_TIMER_T1_INDEX 0
#define AT_DMTIMER_TIMER_T2_INDEX 1
#define AT_DMTIMER_TIMER_T4_INDEX 2
#define AT_DMTIMER_TIMER_TA_INDEX 3
#define AT_DMTIMER_TIMER_TB_INDEX 4
#define AT_DMTIMER_TIMER_TC_INDEX 5
#define AT_DMTIMER_TIMER_TD_INDEX 6
#define AT_DMTIMER_TIMER_TE_INDEX 7
#define AT_DMTIMER_TIMER_TF_INDEX 8
#define AT_DMTIMER_TIMER_TG_INDEX 9
#define AT_DMTIMER_TIMER_TH_INDEX 10
#define AT_DMTIMER_TIMER_TI_INDEX 11
#define AT_DMTIMER_TIMER_TJ_INDEX 12
#define AT_DMTIMER_TIMER_TK_INDEX 13

#define AT_MCC_STR_LEN 3
#define AT_PLMN_MCC_CLASS_MASK 0xFFFFF000
#define AT_HALF_BYTE_TO_BITS_LENS 4

#if (FEATURE_IMS == FEATURE_ON)

/* AT与IMSA模块间消息处理函数指针 */
static const AT_IMSA_MsgProFunc g_atImsaMsgTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { ID_IMSA_AT_CIREG_SET_CNF, AT_RcvImsaCiregSetCnf },
    { ID_IMSA_AT_CIREG_QRY_CNF, AT_RcvImsaCiregQryCnf },
    { ID_IMSA_AT_CIREP_SET_CNF, AT_RcvImsaCirepSetCnf },
    { ID_IMSA_AT_CIREP_QRY_CNF, AT_RcvImsaCirepQryCnf },
    { ID_IMSA_AT_VOLTEIMPU_QRY_CNF, AT_RcvImsaImpuSetCnf },
    { ID_IMSA_AT_CIREGU_IND, AT_RcvImsaCireguInd },
    { ID_IMSA_AT_CIREPH_IND, AT_RcvImsaCirephInd },
    { ID_IMSA_AT_CIREPI_IND, AT_RcvImsaCirepiInd },

    { ID_IMSA_AT_MT_STATES_IND, AT_RcvImsaMtStateInd },
    { ID_IMSA_AT_IMS_CTRL_MSG, AT_RcvImsaImsCtrlMsg },
    { ID_IMSA_AT_FUSIONCALL_CTRL_MSG, AT_RcvImsaFusionCallCtrlMsg },
    { ID_IMSA_AT_IMS_REG_DOMAIN_QRY_CNF, AT_RcvImsaImsRegDomainQryCnf },
    { ID_IMSA_AT_CALL_ENCRYPT_SET_CNF, AT_RcvImsaCallEncryptSetCnf },
    { ID_IMSA_AT_ROAMING_IMS_QRY_CNF, AT_RcvImsaRoamImsServiceQryCnf },

    { ID_IMSA_AT_IMS_RAT_HANDOVER_IND, AT_RcvImsaRatHandoverInd },
    { ID_IMSA_AT_IMS_SRV_STATUS_UPDATE_IND, AT_RcvSrvStatusUpdateInd },

    { ID_IMSA_AT_PCSCF_SET_CNF, AT_RcvImsaPcscfSetCnf },
    { ID_IMSA_AT_PCSCF_QRY_CNF, AT_RcvImsaPcscfQryCnf },

    { ID_IMSA_AT_DMDYN_SET_CNF, AT_RcvImsaDmDynSetCnf },
    { ID_IMSA_AT_DMDYN_QRY_CNF, AT_RcvImsaDmDynQryCnf },

    { ID_IMSA_AT_DMCN_IND, AT_RcvImsaDmcnInd },

    { ID_IMSA_AT_IMSTIMER_QRY_CNF, AT_RcvImsaImsTimerQryCnf },
    { ID_IMSA_AT_IMSTIMER_SET_CNF, AT_RcvImsaImsTimerSetCnf },
    { ID_IMSA_AT_SMSPSI_SET_CNF, AT_RcvImsaImsPsiSetCnf },
    { ID_IMSA_AT_SMSPSI_QRY_CNF, AT_RcvImsaImsPsiQryCnf },
    { ID_IMSA_AT_DMUSER_QRY_CNF, AT_RcvImsaDmUserQryCnf },

    { ID_IMSA_AT_NICKNAME_SET_CNF, AT_RcvImsaNickNameSetCnf },
    { ID_IMSA_AT_NICKNAME_QRY_CNF, AT_RcvImsaNickNameQryCnf },
    { ID_IMSA_AT_REGFAIL_IND, AT_RcvImsaImsRegFailInd },
    { ID_IMSA_AT_BATTERYINFO_SET_CNF, AT_RcvImsaBatteryInfoSetCnf },

    { ID_IMSA_AT_VOLTEIMPI_QRY_CNF, AT_RcvImsaVolteImpiQryCnf },
    { ID_IMSA_AT_VOLTEDOMAIN_QRY_CNF, AT_RcvImsaVolteDomainQryCnf },
    { ID_IMSA_AT_REGERR_REPORT_SET_CNF, AT_RcvImsaRegErrRptSetCnf },
    { ID_IMSA_AT_REGERR_REPORT_QRY_CNF, AT_RcvImsaRegErrRptQryCnf },
    { ID_IMSA_AT_REGERR_REPORT_IND, AT_RcvImsaRegErrRptInd },

    { ID_IMSA_AT_IMS_IP_CAP_SET_CNF, AT_RcvImsaImsIpCapSetCnf },
    { ID_IMSA_AT_IMS_IP_CAP_QRY_CNF, AT_RcvImsaImsIpCapQryCnf },

    { ID_IMSA_AT_EMC_PDN_ACTIVATE_IND, AT_RcvImsaEmcPdnActivateInd },
    { ID_IMSA_AT_EMC_PDN_DEACTIVATE_IND, AT_RcvImsaEmcPdnDeactivateInd },

    { ID_IMSA_AT_IMS_SRV_STAT_RPT_SET_CNF, AT_RcvImsaImsSrvStatRptSetCnf },
    { ID_IMSA_AT_IMS_SRV_STAT_RPT_QRY_CNF, AT_RcvImsaImsSrvStatRptQryCnf },
    { ID_IMSA_AT_IMS_SERVICE_STATUS_QRY_CNF, AT_RcvImsaImsSrvStatusQryCnf },
    { ID_IMSA_AT_EMERGENCY_AID_SET_CNF, AT_RcvImsaEmcAIdSetCnf },
    { ID_IMSA_AT_CALL_ALT_SRV_IND, AT_RcvImsaCallAltSrvInd },
    { ID_IMSA_AT_DM_RCS_CFG_SET_CNF, AT_RcvImsaDmRcsCfgSetCnf },

    { ID_IMSA_AT_USER_AGENT_CFG_SET_CNF, AT_RcvImsaUserAgentSetCnf },

    { ID_IMSA_AT_IMPU_TYPE_IND, AT_RcvImsaImpuInd },

    { ID_IMSA_AT_VICECFG_SET_CNF, AT_RcvImsaViceCfgSetCnf },
    { ID_IMSA_AT_VICECFG_QRY_CNF, AT_RcvImsaViceCfgQryCnf },
    { ID_IMSA_AT_DIALOG_NOTIFY, AT_RcvImsaDialogNtfInd },
    { ID_IMSA_AT_RTTCFG_SET_CNF, AT_RcvImsaRttCfgSetCnf },
    { ID_IMSA_AT_RTT_MODIFY_SET_CNF, AT_RcvImsaRttModifySetCnf },
    { ID_IMSA_AT_RTT_EVENT_IND, AT_RcvImsaRttEventInd },
    { ID_IMSA_AT_RTT_ERROR_IND, AT_RcvImsaRttErrorInd },
    { ID_IMSA_AT_TRANSPORT_TYPE_SET_CNF, AT_RcvImsaSipPortSetCnf },
    { ID_IMSA_AT_TRANSPORT_TYPE_QRY_CNF, AT_RcvImsaSipPortQryCnf },
#if (FEATURE_ECALL == FEATURE_ON)
    { ID_IMSA_AT_ECALL_ECONTENT_TYPE_SET_CNF, AT_RcvImsaEcallContentTypeSetCnf},
    { ID_IMSA_AT_ECALL_ECONTENT_TYPE_QRY_CNF, AT_RcvImsaEcallContentTypeQryCnf},
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_IMSA_AT_IMS_URSP_SET_CNF, AT_RcvImsaImsUrspSetCnf},
#endif
#if (FEATURE_IMS == FEATURE_ON)
    {ID_IMSA_AT_EMC_PDN_ACTIVATE_CNF, AT_RcvImsaEmcPdnActivateCnf},
    {ID_IMSA_AT_EMC_PDN_DEACTIVATE_CNF, AT_RcvImsaEmcPdnDeactivateCnf},
#endif
};


VOS_VOID AT_ProcImsaMsg(struct MsgCB *msg)
{
    AT_IMSA_Msg *msgTemp = VOS_NULL_PTR;
    VOS_UINT32   i;
    VOS_UINT32   msgCnt;
    VOS_UINT32   msgId;
    VOS_UINT32   rst;

    /* 从g_astAtProcMsgFromImsaTab中获取消息个数 */
    msgCnt  = sizeof(g_atImsaMsgTab) / sizeof(AT_IMSA_MsgProFunc);
    msgTemp = (AT_IMSA_Msg *)msg;

    /* 从消息包中获取MSG ID */
    msgId = msgTemp->msgId;

    /* g_astAtProcMsgFromImsaTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atImsaMsgTab[i].msgId == msgId) {
            rst = g_atImsaMsgTab[i].procMsgFunc((struct MsgCB *)msgTemp);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcImsaMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcImsaMsg: Msg Id is invalid!");
    }

    return;
}


VOS_UINT32 AT_RcvImsaCiregSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CiregSetCnf *ciregCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    ciregCnf = (IMSA_AT_CiregSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(ciregCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIREG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIREG_SET) {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:Not AT_CMD_CIREG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (ciregCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCiregQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CiregQryCnf *ciregCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;
    VOS_UINT16           length;

    /* 初始化消息变量 */
    indexNum = 0;
    length   = 0;
    ciregCnf = (IMSA_AT_CiregQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(ciregCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIREG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIREG_QRY) {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:Not AT_CMD_CIREG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (ciregCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName, ciregCnf->cireg);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", ciregCnf->regInfo);

        /* 如果IMS未注册，<ext_info>参数无意义，且不输出，详见3GPP 27007 v11 8.71 */
        if ((ciregCnf->regInfo != VOS_FALSE) && (ciregCnf->opExtInfo != VOS_FALSE)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", ciregCnf->extInfo);
        }

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirepSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CirepSetCnf *cirepCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    cirepCnf = (IMSA_AT_CirepSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cirepCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIREP_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIREP_SET) {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:Not AT_CMD_CIREP_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (cirepCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirepQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CirepQryCnf *cirepCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    cirepCnf = (IMSA_AT_CirepQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cirepCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIREP_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIREP_QRY) {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:Not AT_CMD_CIREP_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (cirepCnf->result == VOS_OK) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            cirepCnf->report, cirepCnf->imsVops);

        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImpuSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_VolteimpuQryCnf *impuCnf = VOS_NULL_PTR;
    VOS_CHAR                 acString[AT_IMSA_IMPU_MAX_LENGTH + 1];
    VOS_UINT32               result;
    errno_t                  memResult;
    VOS_UINT16               length;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    length   = 0;
    impuCnf  = (IMSA_AT_VolteimpuQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(impuCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMPU_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMPU_SET) {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:Not AT_CMD_IMPU_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    (VOS_VOID)memset_s(acString, sizeof(acString), 0x00, sizeof(acString));
    if (impuCnf->impuLen > 0) {
        memResult = memcpy_s(acString, sizeof(acString), impuCnf->impu,
                             TAF_MIN(AT_IMSA_IMPU_MAX_LENGTH, impuCnf->impuLen));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acString), TAF_MIN(AT_IMSA_IMPU_MAX_LENGTH, impuCnf->impuLen));
    }

    /* 判断查询操作是否成功 */
    if (impuCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, acString);

        if ((impuCnf->impuLenVirtual != 0) && (impuCnf->impuLenVirtual < AT_IMSA_IMPU_MAX_LENGTH)) {
            impuCnf->impuVirtual[impuCnf->impuLenVirtual] = '\0';
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", impuCnf->impuVirtual);
        }

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetVolteImpiPara(TAF_UINT8 indexNum)
{
    AT_IMSA_VolteimpiQryReq impi;
    VOS_UINT32              result;

    (VOS_VOID)memset_s(&impi, sizeof(impi), 0x00, sizeof(impi));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送^VOLTEIMPI设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_VOLTEIMPI_QRY_REQ, impi.content,
                                    sizeof(impi.content), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VOLTEIMPI_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetVolteDomainPara(TAF_UINT8 indexNum)
{
    AT_IMSA_VoltedomainQryReq domain;
    VOS_UINT32                result;

    (VOS_VOID)memset_s(&domain, sizeof(domain), 0x00, sizeof(domain));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送^VOLTEDOMAIN设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_VOLTEDOMAIN_QRY_REQ, domain.content,
                                    sizeof(domain.content), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VOLTEDOMAIN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaVolteImpiQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_VolteimpiQryCnf *impiCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    errno_t                  memResult;
    VOS_UINT16               length;
    VOS_CHAR                 acString[AT_IMSA_IMPI_MAX_LENGTH + 1];
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    length   = 0;
    impiCnf  = (IMSA_AT_VolteimpiQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(impiCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaVolteImpiSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaVolteImpiSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMPI_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VOLTEIMPI_SET) {
        AT_WARN_LOG("AT_RcvImsaVolteImpiSetCnf: WARNING:Not AT_CMD_IMPI_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    (VOS_VOID)memset_s(acString, sizeof(acString), 0x00, sizeof(acString));
    impiCnf->impiLen = AT_MIN(impiCnf->impiLen, AT_IMSA_IMPI_MAX_LENGTH);

    if (impiCnf->impiLen > 0) {
        memResult = memcpy_s(acString, sizeof(acString), impiCnf->impi, impiCnf->impiLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acString), impiCnf->impiLen);
    }

    /* 判断查询操作是否成功 */
    if (impiCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, acString);

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaVolteDomainQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_VoltedomainQryCnf *domainCnf = VOS_NULL_PTR;
    VOS_UINT32                 result;
    errno_t                    memResult;
    VOS_UINT16                 length;
    VOS_CHAR                   acString[AT_IMSA_DOMAIN_MAX_LENGTH + 1];
    VOS_UINT8                  indexNum;

    /* 初始化消息变量 */
    indexNum  = 0;
    length    = 0;
    domainCnf = (IMSA_AT_VoltedomainQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(domainCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaVolteDomainSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaVolteDomainSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VOLTEDOMAIN_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VOLTEDOMAIN_SET) {
        AT_WARN_LOG("AT_RcvImsaVolteDomainSetCnf: WARNING:Not AT_CMD_IMPI_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    (VOS_VOID)memset_s(acString, sizeof(acString), 0x00, sizeof(acString));
    domainCnf->domainLen = AT_MIN(domainCnf->domainLen, AT_IMSA_DOMAIN_MAX_LENGTH);

    if (domainCnf->domainLen > 0) {
        memResult = memcpy_s(acString, sizeof(acString), domainCnf->domain, domainCnf->domainLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acString), domainCnf->domainLen);
    }

    /* 判断查询操作是否成功 */
    if (domainCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, acString);

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirephInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CirephInd *cirephInd = VOS_NULL_PTR;
    VOS_UINT8          indexNum;

    /* 初始化消息变量 */
    indexNum  = 0;
    cirephInd = (IMSA_AT_CirephInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cirephInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCirephInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d%s", g_atCrLf, g_atStringTab[AT_STRING_CIREPH].text,
        cirephInd->handover, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirepiInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CirepiInd *cirepiInd = VOS_NULL_PTR;
    VOS_UINT8          indexNum;

    /* 初始化消息变量 */
    indexNum  = 0;
    cirepiInd = (IMSA_AT_CirepiInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cirepiInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCirepiInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d%s", g_atCrLf, g_atStringTab[AT_STRING_CIREPI].text,
        cirepiInd->imsvops, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCireguInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CireguInd *cireguInd = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT16         length;

    /* 初始化消息变量 */
    indexNum  = 0;
    length    = 0;
    cireguInd = (IMSA_AT_CireguInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cireguInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCireguInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d", g_atCrLf, g_atStringTab[AT_STRING_CIREGU].text,
        cireguInd->regInfo);

    /* 如果IMS未注册，<ext_info>参数无意义 */
    if ((cireguInd->regInfo != VOS_FALSE) && (cireguInd->opExtInfo != VOS_FALSE)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cireguInd->extInfo);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    g_atSendDataBuff.bufLen = length;
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_VOID AT_ReportImsEmcStatResult(VOS_UINT8 indexNum, AT_PDP_StatusUint32 status)
{
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^IMSEMCSTAT:%d%s", g_atCrLf, status, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return;
}


VOS_UINT32 AT_RcvImsaEmcPdnActivateInd(struct MsgCB *msg)
{
    IMSA_AT_EmcPdnActivateInd *pdnActivateInd = VOS_NULL_PTR;
    AT_IMS_EmcRdp             *imsEmcRdp      = VOS_NULL_PTR;
    errno_t                    memResult;
    VOS_UINT8                  indexNum = 0;

    pdnActivateInd = (IMSA_AT_EmcPdnActivateInd *)msg;

    /* 转换ClientId */
    if (At_ClientIdToUserId(pdnActivateInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnActivateInd: ClientId is invalid.");
        return VOS_ERR;
    }

    /* 获取IMS EMC RDP */
    imsEmcRdp = AT_GetImsEmcRdpByClientId(indexNum);
    if (imsEmcRdp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnActivateInd: ImsEmcRdp not found.");
        return VOS_ERR;
    }

    /* 清除IMS EMC信息 */
    (VOS_VOID)memset_s(imsEmcRdp, sizeof(AT_IMS_EmcRdp), 0x00, sizeof(AT_IMS_EmcRdp));

    /* 更新IPv4 PDN信息 */
    if ((pdnActivateInd->pdpAddr.pdpType == TAF_PDP_IPV4) || (pdnActivateInd->pdpAddr.pdpType == TAF_PDP_IPV4V6)) {
        imsEmcRdp->opIPv4PdnInfo = VOS_TRUE;

        memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.ipAddr, sizeof(imsEmcRdp->iPv4PdnInfo.ipAddr),
                             pdnActivateInd->pdpAddr.ipv4Addr, sizeof(pdnActivateInd->pdpAddr.ipv4Addr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.ipAddr), sizeof(pdnActivateInd->pdpAddr.ipv4Addr));

        if (pdnActivateInd->ipv4Dns.bitOpPrimDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr, sizeof(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr),
                                 pdnActivateInd->ipv4Dns.primDnsAddr, sizeof(pdnActivateInd->ipv4Dns.primDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr),
                                sizeof(pdnActivateInd->ipv4Dns.primDnsAddr));
        }

        if (pdnActivateInd->ipv4Dns.bitOpSecDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.dnsSecAddr, sizeof(imsEmcRdp->iPv4PdnInfo.dnsSecAddr),
                                 pdnActivateInd->ipv4Dns.secDnsAddr, sizeof(pdnActivateInd->ipv4Dns.secDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.dnsSecAddr),
                                sizeof(pdnActivateInd->ipv4Dns.secDnsAddr));
        }

        imsEmcRdp->iPv4PdnInfo.mtu = pdnActivateInd->mtu;
    }

    /* 更新IPv6 PDN信息 */
    if ((pdnActivateInd->pdpAddr.pdpType == TAF_PDP_IPV6) || (pdnActivateInd->pdpAddr.pdpType == TAF_PDP_IPV4V6)) {
        imsEmcRdp->opIPv6PdnInfo = VOS_TRUE;

        memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.ipAddr, sizeof(imsEmcRdp->iPv6PdnInfo.ipAddr),
                             pdnActivateInd->pdpAddr.ipv6Addr, sizeof(pdnActivateInd->pdpAddr.ipv6Addr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.ipAddr), sizeof(pdnActivateInd->pdpAddr.ipv6Addr));

        if (pdnActivateInd->ipv6Dns.bitOpPrimDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr, sizeof(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr),
                                 pdnActivateInd->ipv6Dns.primDnsAddr, sizeof(pdnActivateInd->ipv6Dns.primDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr),
                                sizeof(pdnActivateInd->ipv6Dns.primDnsAddr));
        }

        if (pdnActivateInd->ipv6Dns.bitOpSecDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.dnsSecAddr, sizeof(imsEmcRdp->iPv6PdnInfo.dnsSecAddr),
                                 pdnActivateInd->ipv6Dns.secDnsAddr, sizeof(pdnActivateInd->ipv6Dns.secDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.dnsSecAddr),
                                sizeof(pdnActivateInd->ipv6Dns.secDnsAddr));
        }

        imsEmcRdp->iPv6PdnInfo.mtu = pdnActivateInd->mtu;
    }

    /* 上报连接状态 */
    AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_ACT);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaEmcPdnDeactivateInd(struct MsgCB *msg)
{
    IMSA_AT_EmcPdnDeactivateInd *pdnDeactivateInd = VOS_NULL_PTR;
    AT_IMS_EmcRdp               *imsEmcRdp        = VOS_NULL_PTR;
    VOS_UINT8                    indexNum         = 0;

    pdnDeactivateInd = (IMSA_AT_EmcPdnDeactivateInd *)msg;

    /* 检查ClientId */
    if (At_ClientIdToUserId(pdnDeactivateInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateInd: ClientId is invalid.");
        return VOS_ERR;
    }

    /* 获取IMS EMC上下文 */
    imsEmcRdp = AT_GetImsEmcRdpByClientId(indexNum);
    if (imsEmcRdp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateInd: ImsEmcRdp not found.");
        return VOS_ERR;
    }

    /* 检查IMS EMC状态 */
    if ((imsEmcRdp->opIPv4PdnInfo != VOS_TRUE) && (imsEmcRdp->opIPv6PdnInfo != VOS_TRUE)) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateInd: IMS EMC PDN not active.");
        return VOS_ERR;
    }

    /* 清除IMS EMC信息 */
    (VOS_VOID)memset_s(imsEmcRdp, sizeof(AT_IMS_EmcRdp), 0x00, sizeof(AT_IMS_EmcRdp));

    /* 上报连接状态 */
    AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_DEACT);

    return VOS_OK;
}


LOCAL VOS_VOID AT_UpdateIpv4Ipv6PdnInfo(AT_IMS_EmcRdp *imsEmcRdp, const IMSA_AT_EmcPdnActivateCnf *pdnActivateCnf)
{
    errno_t memResult;

    /* 更新IPv4 PDN信息 */
    if ((pdnActivateCnf->pdpAddr.pdpType == TAF_PDP_IPV4) || (pdnActivateCnf->pdpAddr.pdpType == TAF_PDP_IPV4V6)) {
        imsEmcRdp->opIPv4PdnInfo = VOS_TRUE;

        memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.ipAddr, sizeof(imsEmcRdp->iPv4PdnInfo.ipAddr),
            pdnActivateCnf->pdpAddr.ipv4Addr, sizeof(pdnActivateCnf->pdpAddr.ipv4Addr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.ipAddr), sizeof(pdnActivateCnf->pdpAddr.ipv4Addr));

        if (pdnActivateCnf->ipv4Dns.bitOpPrimDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr, sizeof(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr),
                pdnActivateCnf->ipv4Dns.primDnsAddr, sizeof(pdnActivateCnf->ipv4Dns.primDnsAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.dnsPrimAddr),
            sizeof(pdnActivateCnf->ipv4Dns.primDnsAddr));
        }

        if (pdnActivateCnf->ipv4Dns.bitOpSecDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv4PdnInfo.dnsSecAddr, sizeof(imsEmcRdp->iPv4PdnInfo.dnsSecAddr),
                pdnActivateCnf->ipv4Dns.secDnsAddr, sizeof(pdnActivateCnf->ipv4Dns.secDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv4PdnInfo.dnsSecAddr),
                sizeof(pdnActivateCnf->ipv4Dns.secDnsAddr));
        }

        imsEmcRdp->iPv4PdnInfo.mtu = pdnActivateCnf->mtu;
    }
    /* 更新IPv6 PDN信息 */
    if ((pdnActivateCnf->pdpAddr.pdpType == TAF_PDP_IPV6) || (pdnActivateCnf->pdpAddr.pdpType == TAF_PDP_IPV4V6)) {
        imsEmcRdp->opIPv6PdnInfo = VOS_TRUE;

        memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.ipAddr, sizeof(imsEmcRdp->iPv6PdnInfo.ipAddr),
            pdnActivateCnf->pdpAddr.ipv6Addr, sizeof(pdnActivateCnf->pdpAddr.ipv6Addr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.ipAddr), sizeof(pdnActivateCnf->pdpAddr.ipv6Addr));

        if (pdnActivateCnf->ipv6Dns.bitOpPrimDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr, sizeof(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr),
                pdnActivateCnf->ipv6Dns.primDnsAddr, sizeof(pdnActivateCnf->ipv6Dns.primDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.dnsPrimAddr),
            sizeof(pdnActivateCnf->ipv6Dns.primDnsAddr));
        }

        if (pdnActivateCnf->ipv6Dns.bitOpSecDnsAddr == VOS_TRUE) {
            memResult = memcpy_s(imsEmcRdp->iPv6PdnInfo.dnsSecAddr, sizeof(imsEmcRdp->iPv6PdnInfo.dnsSecAddr),
                pdnActivateCnf->ipv6Dns.secDnsAddr, sizeof(pdnActivateCnf->ipv6Dns.secDnsAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(imsEmcRdp->iPv6PdnInfo.dnsSecAddr),
                sizeof(pdnActivateCnf->ipv6Dns.secDnsAddr));
        }

        imsEmcRdp->iPv6PdnInfo.mtu = pdnActivateCnf->mtu;
    }
}


VOS_UINT32 AT_RcvImsaEmcPdnActivateCnf(struct MsgCB *msg)
{
    IMSA_AT_EmcPdnActivateCnf  *pdnActivateCnf = VOS_NULL_PTR;
    AT_IMS_EmcRdp              *imsEmcRdp      = VOS_NULL_PTR;
    VOS_UINT8                   indexNum       = 0;

    pdnActivateCnf = (IMSA_AT_EmcPdnActivateCnf *)msg;

    /* 转换ClientId */
    if (At_ClientIdToUserId(pdnActivateCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnActivateCnf: ClientId is invalid.");
        return VOS_ERR;
    }

    /* 建立失败，直接回复结果 */
    if (pdnActivateCnf->result == VOS_ERR) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnActivateCnf: EmcPdn Active fail.");
        AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_DEACT);
        return VOS_OK;
    }

    /* 获取IMS EMC RDP */
    imsEmcRdp = AT_GetImsEmcRdpByClientId(indexNum);
    if (imsEmcRdp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnActivateInd: ImsEmcRdp not found.");
        return VOS_ERR;
    }

    /* 清除IMS EMC信息 */
    (VOS_VOID)memset_s(imsEmcRdp, sizeof(AT_IMS_EmcRdp), 0x00, sizeof(AT_IMS_EmcRdp));

    AT_UpdateIpv4Ipv6PdnInfo(imsEmcRdp, pdnActivateCnf);

    /* 上报连接状态 */
    AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_ACT);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaEmcPdnDeactivateCnf(struct MsgCB *msg)
{
    IMSA_AT_EmcPdnDeactivateCnf *pstPdnDeactivateCnf = VOS_NULL_PTR;
    AT_IMS_EmcRdp               *imsEmcRdp           = VOS_NULL_PTR;
    VOS_UINT8                    indexNum            = 0;

    pstPdnDeactivateCnf = (IMSA_AT_EmcPdnDeactivateCnf *)msg;

    /* 检查ClientId */
    if (At_ClientIdToUserId(pstPdnDeactivateCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateCnf: ClientId is invalid.");
        return VOS_ERR;
    }

    if (pstPdnDeactivateCnf->result == VOS_OK) {
        /* 上报状态 */
        AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_DEACT);

        /* 获取IMS EMC上下文 */
        imsEmcRdp = AT_GetImsEmcRdpByClientId(indexNum);
        if (imsEmcRdp == VOS_NULL_PTR) {
            AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateCnf: ImsEmcRdp not found.");
            return VOS_ERR;
        }

        /* 检查IMS EMC状态 */
        if ((imsEmcRdp->opIPv4PdnInfo != VOS_TRUE) && (imsEmcRdp->opIPv6PdnInfo != VOS_TRUE)) {
            AT_WARN_LOG("AT_RcvImsaEmcPdnDeactivateCnf: IMS EMC PDN not active.");
            return VOS_ERR;
        }

        /* 清除IMS EMC信息 */
        (VOS_VOID)memset_s(imsEmcRdp, sizeof(AT_IMS_EmcRdp), 0x00, sizeof(AT_IMS_EmcRdp));
     } else {
        /* 没有释放成功，给上报hold */
        AT_ReportImsEmcStatResult(indexNum, AT_PDP_STATUS_HOLD);
     }

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaMtStateInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_MtStatesInd *mtStatusInd = VOS_NULL_PTR;
    VOS_CHAR             acString[AT_IMSA_CALL_ASCII_NUM_MAX_LENGTH + 1];
    errno_t              memResult;
    VOS_UINT8            indexNum;

    /* 初始化消息变量 */
    indexNum    = 0;
    mtStatusInd = (IMSA_AT_MtStatesInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtStatusInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(acString, sizeof(acString), 0x00, sizeof(acString));
    memResult = memcpy_s(acString, sizeof(acString), mtStatusInd->asciiCallNum, AT_IMSA_CALL_ASCII_NUM_MAX_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acString), AT_IMSA_CALL_ASCII_NUM_MAX_LENGTH);

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^IMSMTRPT: %s,%d,%d%s", g_atCrLf, acString, mtStatusInd->mtStatus,
        mtStatusInd->causeCode, g_atCrLf);
    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaImsRegDomainQryCnf(struct MsgCB *msg)
{
    /* 局部变量 */
    IMSA_AT_ImsRegDomainQryCnf *imsRegDomainCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;
    VOS_UINT32                  result;

    /* 初始化变量 */
    indexNum        = 0;
    result          = 0;
    imsRegDomainCnf = (IMSA_AT_ImsRegDomainQryCnf *)msg;

    /* 获取ucIndex */
    if (At_ClientIdToUserId(imsRegDomainCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsRegDomainQryCnf: WARNING: AT INDEX NOT FOUND1");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsRegDomainQryCnf: WARNING: AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作是否为AT_CMD_IMSREGDOMAIN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSREGDOMAIN_QRY) {
        AT_WARN_LOG("AT_RcvImsaImsRegDomainQryCnf: WARNING: Not AT_CMD_IMSREGDOMAIN_QRY!");
        return VOS_ERR;
    }

    /* 复位状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 无效值修改为255后，删除返回值是否合法判断 */
    g_atSendDataBuff.bufLen +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            imsRegDomainCnf->imsRegDomain);
    result = AT_OK;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImsCtrlMsg(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsCtrlMsg              *imsCtrlMsgInd = VOS_NULL_PTR;
    AT_IMS_CtrlMsgReceiveModuleUint8 module;
    VOS_UINT8                        indexNum;

    /* 初始化消息变量 */
    module        = AT_IMS_CTRL_MSG_RECEIVE_MODULE_IMSA;
    indexNum      = 0;
    imsCtrlMsgInd = (IMSA_AT_ImsCtrlMsg *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsCtrlMsgInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsCtrlMsg: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_PS_ReportImsCtrlMsgu(indexNum, module, imsCtrlMsgInd->wifiMsgLen, imsCtrlMsgInd->wifiMsg);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaFusionCallCtrlMsg(struct MsgCB *msg)
{
    IMSA_AT_FusionCallCtrlMsg *imsFusionCall = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;

    indexNum      = 0;
    imsFusionCall = (IMSA_AT_FusionCallCtrlMsg *)msg;
    imsFusionCall->msgLen = AT_MIN(imsFusionCall->msgLen, AT_IMSA_FUSIONCALL_DATA_MAX_LEN);

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsFusionCall->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaFusionCallCtrlMsg: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_PS_ReportImsaFusioncCallCtrlMsgu(indexNum, imsFusionCall->srcId, imsFusionCall->msgLen, imsFusionCall->msgData);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaCallEncryptSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CallEncryptSetCnf *callEncryptCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;
    VOS_UINT32                 result;

    /* 初始化消息变量 */
    indexNum       = 0;
    callEncryptCnf = (IMSA_AT_CallEncryptSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(callEncryptCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCallEncryptSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaCallEncryptSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CALLENCRYPT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CALLENCRYPT_SET) {
        AT_WARN_LOG("AT_RcvImsaCallEncryptSetCnf: WARNING:Not AT_CMD_CALLENCRYPT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (callEncryptCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRoamImsServiceQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RoamingImsQryCnf *roamImsServiceCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;
    VOS_UINT16                length;

    /* 初始化消息变量 */
    indexNum          = 0;
    length            = 0;
    roamImsServiceCnf = (IMSA_AT_RoamingImsQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(roamImsServiceCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRoamImsServiceQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaRoamImsServiceQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ROAM_IMS_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ROAM_IMS_QRY) {
        AT_WARN_LOG("AT_RcvImsaRoamImsServiceQryCnf: WARNING:Not AT_CMD_ROAM_IMS_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (roamImsServiceCnf->result == VOS_OK) {
        length +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                ((roamImsServiceCnf->roamingImsSupportFlag == AT_IMSA_ROAMING_IMS_SUPPORT) ? 1 : 0));

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRatHandoverInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsRatHandoverInd *handoverInd = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;

    /* 初始化消息变量 */
    indexNum    = 0;
    handoverInd = (IMSA_AT_ImsRatHandoverInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(handoverInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRatHandoverInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^IMSRATHO: %d,%d,%d,%d%s", g_atCrLf, handoverInd->hoStatus,
        handoverInd->srcRat, handoverInd->dstRat, handoverInd->cause, g_atCrLf);
    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvSrvStatusUpdateInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsSrvStatusUpdateInd *srvUpdateInd = VOS_NULL_PTR;
    VOS_UINT8                      indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    srvUpdateInd = (IMSA_AT_ImsSrvStatusUpdateInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(srvUpdateInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvSrvStatusUpdateInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^IMSSRVSTATUS: %d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            srvUpdateInd->imsSrvStatInfo.smsSrvStatus, srvUpdateInd->imsSrvStatInfo.smsSrvRat,
            srvUpdateInd->imsSrvStatInfo.voIpSrvStatus, srvUpdateInd->imsSrvStatInfo.voIpSrvRat,
            srvUpdateInd->imsSrvStatInfo.vtSrvStatus, srvUpdateInd->imsSrvStatInfo.vtSrvRat,
            srvUpdateInd->imsSrvStatInfo.vsSrvStatus, srvUpdateInd->imsSrvStatInfo.vsSrvRat, g_atCrLf);
    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_VOID At_FillDmdynNumParaInCmd(VOS_UINT16 *length, VOS_UINT32 value, VOS_UINT32 valueValidFlg)
{
    /* 数据有效 */
    if (valueValidFlg == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", value);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID At_FillDmdynStrParaInCmd(VOS_UINT16 *length, VOS_CHAR *pcValue, VOS_UINT32 valueValidFlg,
                                  VOS_UINT32 lastParaFlg)
{
    /* 数据有效 */
    if (valueValidFlg == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "\"%s\"", pcValue);
    }

    /* 不是最后一个参数需要在后面添加逗号 */
    if (lastParaFlg == VOS_FALSE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID At_FillIpv6AddrInCmd(VOS_UINT16 *length, VOS_UINT8 *addr, VOS_UINT32 addrValidFlg, VOS_UINT32 sipPort,
                              VOS_UINT32 portValidFlg)
{
    VOS_UINT8 iPv6Str[TAF_MAX_IPV6_ADDR_DOT_STR_LEN];

    (VOS_VOID)memset_s(iPv6Str, TAF_MAX_IPV6_ADDR_DOT_STR_LEN, 0, TAF_MAX_IPV6_ADDR_DOT_STR_LEN);

    /* IPV6地址有效 */
    if (addrValidFlg == VOS_TRUE) {
        /* 将IPV6地址从num转换为str */
        AT_Ipv6AddrToStr(iPv6Str, addr, AT_IPV6_STR_TYPE_HEX);

        /* 是否存在端口号 */
        if (portValidFlg == VOS_TRUE) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"[%s]:%u\"", iPv6Str, sipPort);
        } else {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", iPv6Str);
        }
    }
    /* IPV6地址无效 */
    else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID At_FillIpv4AddrInCmd(VOS_UINT16 *length, VOS_UINT8 *addr, VOS_UINT32 addrValidFlg, VOS_UINT32 sipPort,
                              VOS_UINT32 portValidFlg)
{
    VOS_CHAR iPv4Str[TAF_MAX_IPV4_ADDR_STR_LEN + 1];

    (VOS_VOID)memset_s(iPv4Str, TAF_MAX_IPV4_ADDR_STR_LEN + 1, 0, TAF_MAX_IPV4_ADDR_STR_LEN + 1);

    /* IPV4地址有效 */
    if (addrValidFlg == VOS_TRUE) {
        /* 将IPV4地址从num转换为str */
        AT_PcscfIpv4Addr2Str(iPv4Str, sizeof(iPv4Str), addr);

        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s", iPv4Str);

        /* 是否存在端口号 */
        if (portValidFlg == VOS_TRUE) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ":%u\"", sipPort);
        } else {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "\"");
        }
    }
    /* IPV4地址无效 */
    else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_UINT32 AT_RcvImsaPcscfSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_PcscfSetCnf *pcscfCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    pcscfCnf = (IMSA_AT_PcscfSetCnf *)msg;

    AT_INFO_LOG("AT_RcvImsaPcscfSetCnf entered!");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(pcscfCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaDmPcscfSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaDmPcscfSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PCSCF_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PCSCF_SET) {
        AT_ERR_LOG("AT_RcvImsaDmPcscfSetCnf: WARNING:Not AT_CMD_PCSCF_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (pcscfCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaPcscfQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_PcscfQryCnf *pcscfCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;
    VOS_UINT16           length;

    /* 初始化变量 */
    pcscfCnf = (IMSA_AT_PcscfQryCnf *)msg;
    indexNum = 0;
    length   = 0;
    result   = AT_OK;

    AT_INFO_LOG("AT_RcvImsaPcscfQryCnf entered!");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(pcscfCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaPcscfQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaPcscfQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PCSCF_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PCSCF_QRY) {
        AT_ERR_LOG("AT_RcvImsaPcscfQryCnf: WARNING:Not AT_CMD_PCSCF_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (pcscfCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            pcscfCnf->atPcscf.src);

        /* <PrimIpv6Pcscf> */
        At_FillIpv6AddrInCmd(&length, pcscfCnf->atPcscf.ipv6Pcscf.primPcscfAddr,
                             pcscfCnf->atPcscf.ipv6Pcscf.opPrimPcscfAddr, pcscfCnf->atPcscf.ipv6Pcscf.primPcscfSipPort,
                             pcscfCnf->atPcscf.ipv6Pcscf.opPrimPcscfSipPort);

        /* <SecIpv6Pcscf> */
        At_FillIpv6AddrInCmd(&length, pcscfCnf->atPcscf.ipv6Pcscf.secPcscfAddr,
                             pcscfCnf->atPcscf.ipv6Pcscf.opSecPcscfAddr, pcscfCnf->atPcscf.ipv6Pcscf.secPcscfSipPort,
                             pcscfCnf->atPcscf.ipv6Pcscf.opSecPcscfSipPort);

        /* <ThiIpv6Pcscf> */
        At_FillIpv6AddrInCmd(&length, pcscfCnf->atPcscf.ipv6Pcscf.thiPcscfAddr,
                             pcscfCnf->atPcscf.ipv6Pcscf.opThiPcscfAddr, pcscfCnf->atPcscf.ipv6Pcscf.thiPcscfSipPort,
                             pcscfCnf->atPcscf.ipv6Pcscf.opThiPcscfSipPort);

        /* <PrimIpv4Pcscf> */
        At_FillIpv4AddrInCmd(&length, pcscfCnf->atPcscf.ipv4Pcscf.primPcscfAddr,
                             pcscfCnf->atPcscf.ipv4Pcscf.opPrimPcscfAddr, pcscfCnf->atPcscf.ipv4Pcscf.primPcscfSipPort,
                             pcscfCnf->atPcscf.ipv4Pcscf.opPrimPcscfSipPort);

        /* <SecIpv4Pcscf> */
        At_FillIpv4AddrInCmd(&length, pcscfCnf->atPcscf.ipv4Pcscf.secPcscfAddr,
                             pcscfCnf->atPcscf.ipv4Pcscf.opSecPcscfAddr, pcscfCnf->atPcscf.ipv4Pcscf.secPcscfSipPort,
                             pcscfCnf->atPcscf.ipv4Pcscf.opSecPcscfSipPort);

        /* <ThiIpv4Pcscf> */
        At_FillIpv4AddrInCmd(&length, pcscfCnf->atPcscf.ipv4Pcscf.thiPcscfAddr,
                             pcscfCnf->atPcscf.ipv4Pcscf.opThiPcscfAddr, pcscfCnf->atPcscf.ipv4Pcscf.thiPcscfSipPort,
                             pcscfCnf->atPcscf.ipv4Pcscf.opThiPcscfSipPort);

        g_atSendDataBuff.bufLen = length;

    }
    /* 消息携带结果不是OK时，直接返回ERROR */
    else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaDmDynSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DmdynSetCnf *dmdynCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    dmdynCnf = (IMSA_AT_DmdynSetCnf *)msg;

    AT_INFO_LOG("AT_RcvImsaDmDynSetCnf entered!");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dmdynCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaDmDynSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaDmDynSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMDYN_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMDYN_SET) {
        AT_ERR_LOG("AT_RcvImsaDmDynSetCnf: WARNING:Not AT_CMD_DMDYN_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (dmdynCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaDmDynQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DmdynQryCnf *dmDynCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;
    VOS_UINT16           length;

    /* 初始化消息变量 */
    indexNum = 0;
    length   = 0;
    dmDynCnf = (IMSA_AT_DmdynQryCnf *)msg;

    AT_INFO_LOG("AT_RcvImsaDmDynQryCnf entered!");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dmDynCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaDmDynQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaDmDynQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMDYN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMDYN_QRY) {
        AT_ERR_LOG("AT_RcvImsaDmDynQryCnf: WARNING:Not AT_CMD_DMDYN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (dmDynCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <ulAmrWbOctetAcigned> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.amrWbOctetAcigned,
                                 (VOS_UINT32)(dmDynCnf->dmdyn.opAmrWbOctetAcigned));

        /* <ulAmrWbBandWidthEfficient> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.amrWbBandWidthEfficient,
                                 (VOS_UINT32)(dmDynCnf->dmdyn.opAmrWbBandWidthEfficient));

        /* <ulAmrOctetAcigned> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.amrOctetAcigned,
                                 (VOS_UINT32)(dmDynCnf->dmdyn.opAmrOctetAcigned));

        /* <ulAmrBandWidthEfficient> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.amrBandWidthEfficient,
                                 (VOS_UINT32)(dmDynCnf->dmdyn.opAmrBandWidthEfficient));

        /* <ulAmrWbMode> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.amrWbMode, (VOS_UINT32)(dmDynCnf->dmdyn.opAmrWbMode));

        /* <ulDtmfWb> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.dtmfWb, (VOS_UINT32)(dmDynCnf->dmdyn.opDtmfWb));

        /* <ulDtmfNb> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.dtmfNb, (VOS_UINT32)(dmDynCnf->dmdyn.opDtmfNb));

        /* <ulSpeechStart> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.speechStart, (VOS_UINT32)(dmDynCnf->dmdyn.opSpeechStart));

        /* <ulSpeechEnd> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.speechEnd, (VOS_UINT32)(dmDynCnf->dmdyn.opSpeechEnd));

        /* <ulSpeechStart> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.videoStart, (VOS_UINT32)(dmDynCnf->dmdyn.opVideoStart));

        /* <ulSpeechEnd> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.videoEnd, (VOS_UINT32)(dmDynCnf->dmdyn.opVideoEnd));

        /* <ulRetryBaseTime> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.retryBaseTime, (VOS_UINT32)(dmDynCnf->dmdyn.opRetryBaseTime));

        /* <ulRetryMaxTime> */
        At_FillDmdynNumParaInCmd(&length, dmDynCnf->dmdyn.retryMaxTime, (VOS_UINT32)(dmDynCnf->dmdyn.opRetryMaxTime));

        /* <acPhoneContext> */
        dmDynCnf->dmdyn.phoneContext[AT_IMSA_PHONECONTEXT_MAX_LENGTH] = '\0';
        At_FillDmdynStrParaInCmd(&length, dmDynCnf->dmdyn.phoneContext, (VOS_UINT32)(dmDynCnf->dmdyn.opPhoneContext),
                                 VOS_FALSE);

        /* <acPhoneContextImpu> */
        dmDynCnf->dmdyn.phoneContextImpu[AT_IMSA_PUBLICEUSERID_MAX_LENGTH] = '\0';
        At_FillDmdynStrParaInCmd(&length, dmDynCnf->dmdyn.phoneContextImpu,
                                 (VOS_UINT32)(dmDynCnf->dmdyn.opPhoneContextImpu), VOS_TRUE);

        g_atSendDataBuff.bufLen = length;

        result = AT_OK;
    }
    /* 消息携带结果不是OK时，直接返回ERROR */
    else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaDmcnInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DmcnInd *dmcnInd = VOS_NULL_PTR;
    VOS_UINT8        indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    dmcnInd  = (IMSA_AT_DmcnInd *)msg;

    AT_INFO_LOG("AT_RcvImsaDmcnInd entered!");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dmcnInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaDmcnInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 构造AT主动上报^DMCN */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_DMCN].text, g_atCrLf);

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_ParseIpv6PcscfData(VOS_UINT32 *addrExistFlg, VOS_UINT8 *ipAddr, VOS_UINT32 *portExistFlg,
                                 VOS_UINT32 *portNum, VOS_UINT32 indexNum)
{
    /* IPV6地址长度超过限制 */
    if ((AT_PARA_MAX_LEN + 1) < g_atParaList[indexNum].paraLen) {
        *portExistFlg = VOS_FALSE;
        *addrExistFlg = VOS_FALSE;
        AT_ERR_LOG("AT_FillIpv6PcscfData: PCSCF IPV6 address length OUT OF RANGE");
        return VOS_ERR;
    }

    /* AT参数为空，代表地址不存在 */
    if (g_atParaList[indexNum].paraLen == 0) {
        AT_NORM_LOG("AT_ParseIpv6PcscfData: PCSCF IPV6 address is NULL");
        *portExistFlg = VOS_FALSE;
        *addrExistFlg = VOS_FALSE;
        return VOS_OK;
    }

    /* 解析IPV6地址和端口号 */
    if (AT_Ipv6PcscfDataToAddr(g_atParaList[indexNum].para, ipAddr, portExistFlg, portNum) != VOS_OK) {
        *addrExistFlg = VOS_FALSE;
        AT_ERR_LOG("AT_ParseIpv6PcscfData: PCSCF IPV6 address decode ERROR");
        return VOS_ERR;
    } else {
        AT_NORM_LOG("AT_ParseIpv6PcscfData: PCSCF IPV6 address decode SUCC");
        *addrExistFlg = VOS_TRUE;
        return VOS_OK;
    }
}


VOS_UINT32 AT_FillIpv6PcscfData(AT_IMSA_PcscfSetReq *pcscf)
{
    VOS_UINT32 result;
    VOS_UINT32 indexNum;
    VOS_UINT32 addrExistsFlg;
    VOS_UINT32 portExistsFlg;

    /*
     * 解析AT，组装发给IMSA的消息
     * ^ IMSPCSCF =<Source>,
     *             <IPV6Address1>,
     *             <IPV6Address2>，
     *             <IPV6Address3>,
     *             <IPV4Address1>,
     *             <IPV4Address2>,
     *             <IPV4Address3>
     */

    /* 解析<IPV6Address1>，将索引设置为AT命令的第2个参数 */
    indexNum      = AT_IMSPCSCF_IPV6_ADDRESS1_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv6PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv6Pcscf.primPcscfAddr, &portExistsFlg,
                                   &(pcscf->atPcscf.ipv6Pcscf.primPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv6Pcscf.opPrimPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv6Pcscf.opPrimPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv6PcscfData: Primary PCSCF IPV6 address decode ERROR");
        return result;
    }

    /* 解析<IPV6Address2>,将索引设置为AT命令的第3个参数 */
    indexNum      = AT_IMSPCSCF_IPV6_ADDRESS2_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv6PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv6Pcscf.secPcscfAddr, &portExistsFlg,
                                   &(pcscf->atPcscf.ipv6Pcscf.secPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv6Pcscf.opSecPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv6Pcscf.opSecPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv6PcscfData: Secondary PCSCF IPV6 address decode ERROR");
        return result;
    }

    /* 解析<IPV6Address3>,将索引设置为AT命令的第4个参数 */
    indexNum      = AT_IMSPCSCF_IPV6_ADDRESS3_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv6PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv6Pcscf.thiPcscfAddr, &portExistsFlg,
                                   &(pcscf->atPcscf.ipv6Pcscf.thiPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv6Pcscf.opThiPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv6Pcscf.opThiPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv6PcscfData: Third PCSCF IPV6 address decode ERROR");
        return result;
    }

    return result;
}


#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
VOS_UINT32 AT_ParseIpv4PcscfData(VOS_UINT32 *addrExistFlg, VOS_UINT8 *ipAddr, VOS_UINT32 ipBufLen,
    VOS_UINT32 *portExistFlg, VOS_UINT32 *portNum, VOS_UINT32 indexNum)
{
    VOS_CHAR  *pcPortStr = VOS_NULL_PTR;
    VOS_UINT8 *tmpIpAddr;
    VOS_UINT32 strLen = 0;
    errno_t    memResult;

    tmpIpAddr = (TAF_UINT8 *)VOS_MemAlloc(WUEPS_PID_AT, DYNAMIC_MEM_PT, AT_PARA_MAX_LEN + 1);

    if (tmpIpAddr == NULL) {
        AT_ERR_LOG("AT_ParseIpv4PcscfData: Fail to malloc mem.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(tmpIpAddr, AT_PARA_MAX_LEN + 1, 0, AT_PARA_MAX_LEN + 1);

    /* IPV4地址长度超过限制 */
    if (AT_PARA_MAX_LEN + 1 < g_atParaList[indexNum].paraLen) {
        AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address length OUT OF RANGE");
        VOS_MemFree(WUEPS_PID_AT, tmpIpAddr);
        return VOS_ERR;
    }

    /* 参数为空，代表此地址不存在 */
    if (g_atParaList[indexNum].paraLen == 0) {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address is NULL");
        *portExistFlg = VOS_FALSE;
        *addrExistFlg = VOS_FALSE;
        VOS_MemFree(WUEPS_PID_AT, tmpIpAddr);
        return VOS_OK;
    }

    pcPortStr = VOS_StrStr((VOS_CHAR *)(g_atParaList[indexNum].para), ":");

    /* 检查是否有端口号 */
    if (pcPortStr != VOS_NULL_PTR) {
        if (AT_PortAtoI(pcPortStr + 1, portNum) == VOS_OK) {
            AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 port num decode SUCC");
            *portExistFlg = VOS_TRUE;
            strLen        = (VOS_UINT32)(pcPortStr - (VOS_CHAR *)(g_atParaList[indexNum].para));
        } else { /* 解析端口号失败 */
            *portExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 port num decode ERROR");
            VOS_MemFree(WUEPS_PID_AT, tmpIpAddr);
            return VOS_ERR;
        }
    } else {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: No port in PCSCF IPV4 addr");
        *portExistFlg = VOS_FALSE;
        strLen        = VOS_StrLen((VOS_CHAR *)(g_atParaList[indexNum].para));
    }

    if (strLen > 0) {
        memResult = memcpy_s(tmpIpAddr, AT_PARA_MAX_LEN + 1, g_atParaList[indexNum].para,
                             AT_MIN(strLen, AT_PARA_MAX_LEN + 1));
        TAF_MEM_CHK_RTN_VAL(memResult, AT_PARA_MAX_LEN + 1, AT_MIN(strLen, AT_PARA_MAX_LEN + 1));
    }

    /* 解析IPV4地址 */
    if (AT_Ipv4AddrAtoi((VOS_CHAR *)tmpIpAddr, ipAddr, ipBufLen) != VOS_OK) {
        *addrExistFlg = VOS_FALSE;
        AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address decode ERROR");
        VOS_MemFree(WUEPS_PID_AT, tmpIpAddr);
        return VOS_ERR;
    } else {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address decode SUCC");
        *addrExistFlg = VOS_TRUE;
    }
    VOS_MemFree(WUEPS_PID_AT, tmpIpAddr);
    return VOS_OK;
}
#else
VOS_UINT32 AT_ParseIpv4PcscfData(VOS_UINT32 *addrExistFlg, VOS_UINT8 *ipAddr, VOS_UINT32 ipBufLen,
                                 VOS_UINT32 *portExistFlg, VOS_UINT32 *portNum, VOS_UINT32 indexNum)
{
    VOS_CHAR  *pcPortStr = VOS_NULL_PTR;
    VOS_UINT8  tmpIpAddr[AT_PARA_MAX_LEN + 1];
    VOS_UINT32 strLen = 0;
    errno_t    memResult;

    (VOS_VOID)memset_s(tmpIpAddr, AT_PARA_MAX_LEN + 1, 0, AT_PARA_MAX_LEN + 1);

    /* IPV4地址长度超过限制 */
    if ((AT_PARA_MAX_LEN + 1) < g_atParaList[indexNum].paraLen) {
        AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address length OUT OF RANGE");
        return VOS_ERR;
    }

    /* 参数为空，代表此地址不存在 */
    if (g_atParaList[indexNum].paraLen == 0) {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address is NULL");
        *portExistFlg = VOS_FALSE;
        *addrExistFlg = VOS_FALSE;
        return VOS_OK;
    }

    pcPortStr = VOS_StrStr((VOS_CHAR *)(g_atParaList[indexNum].para), ":");

    /* 检查是否有端口号 */
    if (pcPortStr != VOS_NULL_PTR) {
        if (AT_PortAtoI(pcPortStr + 1, portNum) == VOS_OK) {
            AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 port num decode SUCC");
            *portExistFlg = VOS_TRUE;
            strLen        = (VOS_UINT32)(pcPortStr - (VOS_CHAR *)(g_atParaList[indexNum].para));
        }
        /* 解析端口号失败 */
        else {
            *portExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 port num decode ERROR");
            return VOS_ERR;
        }
    } else {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: No port in PCSCF IPV4 addr");
        *portExistFlg = VOS_FALSE;
        strLen        = VOS_StrLen((VOS_CHAR *)(g_atParaList[indexNum].para));
    }

    if (strLen > 0) {
        memResult = memcpy_s(tmpIpAddr, AT_PARA_MAX_LEN + 1, g_atParaList[indexNum].para,
                             AT_MIN(strLen, AT_PARA_MAX_LEN + 1));
        TAF_MEM_CHK_RTN_VAL(memResult, AT_PARA_MAX_LEN + 1, AT_MIN(strLen, AT_PARA_MAX_LEN + 1));
    }

    /* 解析IPV4地址 */
    if (AT_Ipv4AddrAtoi((VOS_CHAR *)tmpIpAddr, ipAddr, ipBufLen) != VOS_OK) {
        *addrExistFlg = VOS_FALSE;
        AT_ERR_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address decode ERROR");
        return VOS_ERR;
    } else {
        AT_NORM_LOG("AT_ParseIpv4PcscfData: PCSCF IPV4 address decode SUCC");
        *addrExistFlg = VOS_TRUE;
    }

    return VOS_OK;
}
#endif


VOS_UINT32 AT_FillIpv4PcscfData(AT_IMSA_PcscfSetReq *pcscf)
{
    VOS_UINT32 result;
    VOS_UINT32 indexNum;
    VOS_UINT32 addrExistsFlg;
    VOS_UINT32 portExistsFlg;

    /*
     * 解析AT，组装发给IMSA的消息
     * ^ IMSPCSCF =<Source>,
     *             <IPV6Address1>,
     *             <IPV6Address2>，
     *             <IPV6Address3>,
     *             <IPV4Address1>,
     *             <IPV4Address2>,
     *             <IPV4Address3>
     */

    /* 解析Primary IPV4，将索引设置为AT命令的第5个参数 */
    indexNum      = AT_IMSPCSCF_IPV4_ADDRESS1_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv4PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv4Pcscf.primPcscfAddr, TAF_IPV4_ADDR_LEN,
                                   &portExistsFlg, &(pcscf->atPcscf.ipv4Pcscf.primPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv4Pcscf.opPrimPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv4Pcscf.opPrimPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv4PcscfData: Primary PCSCF IPV4 address Decode ERROR");
        return result;
    }

    /* 解析Secondary IPV4，将索引设置为AT命令的第6个参数 */
    indexNum      = AT_IMSPCSCF_IPV4_ADDRESS2_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv4PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv4Pcscf.secPcscfAddr, TAF_IPV4_ADDR_LEN,
                                   &portExistsFlg, &(pcscf->atPcscf.ipv4Pcscf.secPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv4Pcscf.opSecPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv4Pcscf.opSecPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv4PcscfData: Secondary PCSCF IPV4 address Decode ERROR");
        return result;
    }

    /* 解析Third IPV4，将索引设置为AT命令的第7个参数 */
    indexNum      = AT_IMSPCSCF_IPV4_ADDRESS3_INDEX;
    addrExistsFlg = VOS_FALSE;
    portExistsFlg = VOS_FALSE;

    result = AT_ParseIpv4PcscfData(&addrExistsFlg, pcscf->atPcscf.ipv4Pcscf.thiPcscfAddr, TAF_IPV4_ADDR_LEN,
                                   &portExistsFlg, &(pcscf->atPcscf.ipv4Pcscf.thiPcscfSipPort), indexNum);

    pcscf->atPcscf.ipv4Pcscf.opThiPcscfAddr    = addrExistsFlg;
    pcscf->atPcscf.ipv4Pcscf.opThiPcscfSipPort = portExistsFlg;

    /* 解码错误直接返回 */
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_FillIpv4PcscfData: Third PCSCF IPV4 address Decode ERROR");
        return result;
    }

    return result;
}


VOS_UINT32 AT_FillDataToPcscf(AT_IMSA_PcscfSetReq *pcscf)
{
    VOS_UINT32 result;

    if (pcscf == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_FillDataToPcscf: pstPcscf is NULL, return ERROR");
        return VOS_ERR;
    }

    /* 没有填写<Source>或者超出范围 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[0].paraValue >= IMSA_AT_PCSCF_SRC_BUTT)) {
        AT_ERR_LOG("AT_FillDataToPcscf: No <source> parameter or out of range, return ERROR");
        return VOS_ERR;
    }

    /* <Source> */
    pcscf->atPcscf.src = g_atParaList[0].paraValue;

    result = AT_FillIpv6PcscfData(pcscf);

    /* IPV6和IPV4都解析成功才返回OK */
    if ((AT_FillIpv4PcscfData(pcscf) == VOS_OK) && (result == VOS_OK)) {
        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_UINT32 AT_SetImsPcscfPara(VOS_UINT8 indexNum)
{
    AT_IMSA_PcscfSetReq pcscf;
    VOS_UINT32          result;

    AT_INFO_LOG("AT_SetImsPcscfPara Entered");

    (VOS_VOID)memset_s(&pcscf, sizeof(AT_IMSA_PcscfSetReq), 0x00, sizeof(AT_IMSA_PcscfSetReq));

    /* 不是设置命令则出错 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetImsPcscfPara: NOT SET CMD, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*
     * 解析AT，组装发给IMSA的消息
     * ^IMSPCSCF =<Source>,
     *            <IPV6Address1>,
     *            <IPV6Address2>，
     *            <IPV6Address3>,
     *            <IPV4Address1>,
     *            <IPV4Address2>,
     *            <IPV4Address3>
     */

    /* ^IMSPCSCF携带7个参数，否则出错 */
    if (g_atParaIndex != AT_IMSPCSCF_PARA_NUM) {
        AT_ERR_LOG("AT_SetImsPcscfPara: Para number incorrect, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 组装发给IMSA的消息 */
    if (AT_FillDataToPcscf(&pcscf) != VOS_OK) {
        AT_ERR_LOG("AT_SetImsPcscfPara: AT_FillDataToPcscf FAIL, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_PCSCF_SET_REQ, (VOS_UINT8 *)&pcscf.atPcscf,
                                    (VOS_UINT32)sizeof(IMSA_AT_Pcscf), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetImsPcscfPara: AT_FillAndSndAppReqMsg FAIL");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PCSCF_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_FillImsPayloadTypePara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum)
{
    /* 初始化 */
    *valueValidFlg = VOS_FALSE;

    /* IMS要求PAYLOAD TYPE的参数范围只能使用动态解码范围0X60-0X7F */
    if (g_atParaList[indexNum].paraLen != 0) {
        if ((g_atParaList[indexNum].paraValue < AT_IMS_PAYLOAD_TYPE_RANGE_MIN) ||
            (g_atParaList[indexNum].paraValue > AT_IMS_PAYLOAD_TYPE_RANGE_MAX)) {
            AT_ERR_LOG("At_FillImsPayloadTypePara: IMS Payload type para OUT OF RANGE");
            return VOS_ERR;
        }

        *value         = g_atParaList[indexNum].paraValue;
        *valueValidFlg = VOS_TRUE;
    } else {
        AT_NORM_LOG("At_FillImsPayloadTypePara: IMS Payload type para length is 0");
        *valueValidFlg = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 At_FillImsAmrWbModePara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum)
{
    /* 初始化 */
    *valueValidFlg = VOS_FALSE;

    /* IMS要求AMR WB MODE的参数取值范围为0-8 */
    if (g_atParaList[indexNum].paraLen != 0) {
        if (g_atParaList[indexNum].paraValue > AT_IMS_AMR_WB_MODE_MAX) {
            AT_ERR_LOG("At_FillImsPayloadTypePara: IMS AMR WB MODE OUT OF RANGE");
            return VOS_ERR;
        }

        *value         = g_atParaList[indexNum].paraValue;
        *valueValidFlg = VOS_TRUE;
    } else {
        AT_NORM_LOG("At_FillImsPayloadTypePara: IMS AMR WB MODE para length is 0");
        *valueValidFlg = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 At_FillImsRtpPortPara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum)
{
    /* 初始化 */
    *valueValidFlg = VOS_FALSE;

    /* IMS要求RTP PORT的参数只能使用偶数且不能为0 */
    if (g_atParaList[indexNum].paraLen != 0) {
        if (((g_atParaList[indexNum].paraValue) % 2 != 0) || (g_atParaList[indexNum].paraValue == 0)) {
            AT_ERR_LOG("At_FillImsPayloadTypePara: IMS RTP Port para incorrect");
            return VOS_ERR;
        }

        *value         = g_atParaList[indexNum].paraValue;
        *valueValidFlg = VOS_TRUE;
    } else {
        AT_NORM_LOG("At_FillImsPayloadTypePara: IMS RTP Port para length is 0");
        *valueValidFlg = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 At_FillImsaNumericPara(VOS_UINT32 *value, VOS_UINT32 *valueValidFlg, VOS_UINT32 indexNum)
{
    /* 初始化 */
    *valueValidFlg = VOS_FALSE;

    if (g_atParaList[indexNum].paraLen != 0) {
        *value         = g_atParaList[indexNum].paraValue;
        *valueValidFlg = VOS_TRUE;
    } else {
        AT_NORM_LOG("At_FillImsaNumericPara: IMSA numeric para length is 0");
        *valueValidFlg = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 At_FillImsaStrPara(VOS_CHAR *str, VOS_UINT32 *strValidFlg, VOS_UINT32 maxLen, VOS_UINT32 indexNum)
{
    errno_t memResult;
    /* 初始化 */
    *strValidFlg = VOS_FALSE;

    if (g_atParaList[indexNum].paraLen != 0) {
        if (maxLen < g_atParaList[AT_DMDYN_PHONECONTEXT].paraLen) {
            AT_ERR_LOG("At_FillImsPayloadTypePara: IMS string para out of range");
            return VOS_ERR;
        }

        if (maxLen > 0) {
            memResult = memcpy_s(str, maxLen, (VOS_CHAR *)g_atParaList[indexNum].para,
                                 AT_MIN(maxLen, g_atParaList[indexNum].paraLen));
            TAF_MEM_CHK_RTN_VAL(memResult, maxLen, AT_MIN(maxLen, g_atParaList[indexNum].paraLen));
        }
        *strValidFlg = VOS_TRUE;
    } else {
        AT_NORM_LOG("At_FillImsaNumericPara: IMSA string para length is 0");
        *strValidFlg = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetDmDynPara(TAF_UINT8 indexNum)
{
    AT_IMSA_DmdynSetReq dmdynSetReq;
    VOS_UINT32          result;
    VOS_UINT32          tmpResult;
    VOS_UINT32          valueValidFlg;

    AT_INFO_LOG("AT_SetDmDynPara Entered");

    (VOS_VOID)memset_s(&dmdynSetReq, sizeof(AT_IMSA_DmdynSetReq), 0x00, sizeof(AT_IMSA_DmdynSetReq));
    result = AT_SUCCESS;

    /* 不是设置命令则出错 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetDmDynPara: NOT SET CMD, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ^DMDYN携带15个参数，否则出错 */
    if (g_atParaIndex != AT_DMDYN_PARA_NUM) {
        AT_ERR_LOG("AT_SetDmDynPara: Para number incorrect, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*
     * 解析AT，组装发给IMSA的消息
     * ^DMDYN=<AMR_WB_octet_aligned>,
     *        <AMR_WB_bandwidth_efficient>,
     *        <AMR_octet_aligned>,
     *        <AMR_bandwidth_efficient>,
     *        <AMR_WB_mode>,
     *        <DTMF_WB>,
     *        <DTMF_NB>,
     *        <Speech_start>,
     *        <Speech_end>,
     *        <Video_start>,
     *        <Video_end>,
     *        <RegRetryBaseTime>,
     *        <RegRetryMaxTime>,
     *        <PhoneContext>,
     *        <Public_user_identity>
     */

    /* <AMR_WB_octet_aligned> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.amrWbOctetAcigned),
                                          &valueValidFlg, AT_DMDYN_AWR_WB_OCTET_ALIGNED_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opAmrWbOctetAcigned = valueValidFlg;

    /* <AMR_WB_bandwidth_efficient> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.amrWbBandWidthEfficient),
                                          &valueValidFlg, AT_DMDYN_AWR_WB_BANDWIDTH_ECT_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opAmrWbBandWidthEfficient = valueValidFlg;

    /* <AMR_octet_aligned> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.amrOctetAcigned),
                                          &valueValidFlg, AT_DMDYN_AWR_OCTET_ALIGNED_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opAmrOctetAcigned = valueValidFlg;

    /* <AMR_bandwidth_efficient> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.amrBandWidthEfficient),
                                          &valueValidFlg, AT_DMDYN_AWR_BANDWIDTH_ECT_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opAmrBandWidthEfficient = valueValidFlg;

    /* <AMR_WB_mode> */
    tmpResult = At_FillImsAmrWbModePara(&(dmdynSetReq.dmdyn.amrWbMode), &valueValidFlg, AT_DMDYN_AWR_WB_MODE_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opAmrWbMode = valueValidFlg;

    /* <DTMF_WB> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.dtmfWb), &valueValidFlg, AT_DMDYN_DTMF_WB_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opDtmfWb = valueValidFlg;

    /* <DTMF_NB> */
    tmpResult = At_FillImsPayloadTypePara(&(dmdynSetReq.dmdyn.dtmfNb), &valueValidFlg, AT_DMDYN_DTMF_NB_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opDtmfNb = valueValidFlg;

    /* <Speech_start> */
    tmpResult = At_FillImsRtpPortPara(&(dmdynSetReq.dmdyn.speechStart), &valueValidFlg, AT_DMDYN_SPEECH_START_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opSpeechStart = valueValidFlg;

    /* <Speech_end> */
    tmpResult = At_FillImsRtpPortPara(&(dmdynSetReq.dmdyn.speechEnd), &valueValidFlg, AT_DMDYN_SPEECH_END_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opSpeechEnd = valueValidFlg;

    /* <Video_start> */
    tmpResult = At_FillImsRtpPortPara(&(dmdynSetReq.dmdyn.videoStart), &valueValidFlg, AT_DMDYN_VIDEO_START_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opVideoStart = valueValidFlg;

    /* <Video_end> */
    tmpResult = At_FillImsRtpPortPara(&(dmdynSetReq.dmdyn.videoEnd), &valueValidFlg, AT_DMDYN_VIDEO_END_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opVideoEnd = valueValidFlg;

    /* <RegRetryBaseTime> */
    tmpResult = At_FillImsaNumericPara(&(dmdynSetReq.dmdyn.retryBaseTime),
                                       &valueValidFlg, AT_DMDYN_REGRETRYBASETIME_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opRetryBaseTime = valueValidFlg;

    /* <RegRetryMaxTime> */
    tmpResult = At_FillImsaNumericPara(&(dmdynSetReq.dmdyn.retryMaxTime),
                                       &valueValidFlg, AT_DMDYN_REGRETRYMAXTIME_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opRetryMaxTime = valueValidFlg;

    /* 因IMSA要求Base Time的时间不能超过Max Time */
    if (dmdynSetReq.dmdyn.retryBaseTime > dmdynSetReq.dmdyn.retryMaxTime) {
        AT_ERR_LOG("AT_SetDmDynPara: ulRetryBaseTime is larger than ulRetryMaxTime, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* <PhoneContext> */
    tmpResult = At_FillImsaStrPara(dmdynSetReq.dmdyn.phoneContext, &valueValidFlg, AT_IMSA_PHONECONTEXT_MAX_LENGTH,
                                   AT_DMDYN_PHONECONTEXT_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opPhoneContext = valueValidFlg;

    /* <Public_user_identity> */
    tmpResult = At_FillImsaStrPara(dmdynSetReq.dmdyn.phoneContextImpu, &valueValidFlg, AT_IMSA_PUBLICEUSERID_MAX_LENGTH,
                                   AT_DMDYN_PUBLIC_USER_ID_INDEX);
    result    = result | tmpResult;

    dmdynSetReq.dmdyn.opPhoneContextImpu = valueValidFlg;

    if (result != VOS_OK) {
        AT_ERR_LOG("AT_SetDmDynPara: There have out of range para in setting command");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_DMDYN_SET_REQ, (VOS_UINT8 *)&dmdynSetReq.dmdyn,
                                    (VOS_UINT32)sizeof(AT_IMSA_Dmdyn), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetDmDynPara: AT_FillAndSndAppReqMsg FAIL");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMDYN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryImsPcscfPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    AT_INFO_LOG("AT_QryImsPcscfPara Entered");

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_PCSCF_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryImsPcscfPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PCSCF_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDmDynPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_DMDYN_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryDmdynPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMDYN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_FillImsTimerReqBitAndPara(VOS_UINT32 *value, VOS_UINT32 *bitOpValue, VOS_UINT32 indexNum)
{
    /* 初始化 */
    *bitOpValue = VOS_FALSE;

    /* 判断AT命令参数长度是否为0,定时器时长范围是0-128000ms */
    if (g_atParaList[indexNum].paraLen != 0) {
        if (g_atParaList[indexNum].paraValue > AT_IMS_TIMER_DATA_RANGE_MAX) {
            AT_ERR_LOG("AT_FillImsTimerReqBitAndPara: IMS Payload type para OUT OF RANGE");
            return VOS_ERR;
        }

        *value      = g_atParaList[indexNum].paraValue;
        *bitOpValue = VOS_TRUE;
    } else {
        AT_ERR_LOG("AT_FillImsTimerReqBitAndPara: IMS Timer para length is 0");
        *value      = 0;
        *bitOpValue = VOS_FALSE;
    }

    return VOS_OK;
}


VOS_UINT32 AT_FillImsTimerReqData(AT_IMSA_ImstimerSetReq *imsTimer)
{
    VOS_UINT32 bitOpValueFlg;

    /* 初始化 */
    (VOS_VOID)memset_s(imsTimer, sizeof(AT_IMSA_ImstimerSetReq), 0x00, sizeof(AT_IMSA_ImstimerSetReq));

    /* 填入AT命令参数<Timer_T1> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timer1Value),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_T1_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimer1Value = bitOpValueFlg;

    /* <Timer_T2> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timer2Value),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_T2_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimer2Value = bitOpValueFlg;

    /* <Timer_T4> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timer4Value),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_T4_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimer4Value = bitOpValueFlg;

    /* <Timer_TA> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerAValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TA_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerAValue = bitOpValueFlg;

    /* <Timer_TB> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerBValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TB_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerBValue = bitOpValueFlg;

    /* <Timer_TC> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerCValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TC_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerCValue = bitOpValueFlg;

    /* <Timer_TD> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerDValue),\
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TD_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerDValue = bitOpValueFlg;

    /* <Timer_TE> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerEValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TE_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerEValue = bitOpValueFlg;

    /* <Timer_TF> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerFValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TF_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerFValue = bitOpValueFlg;

    /* <Timer_TG> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerGValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TG_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerGValue = bitOpValueFlg;

    /* <Timer_TH> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerHValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TH_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerHValue = bitOpValueFlg;

    /* <Timer_TI> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerIValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TI_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerIValue = bitOpValueFlg;

    /* <Timer_TJ> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerJValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TJ_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerJValue = bitOpValueFlg;

    /* <Timer_TK> */
    if (AT_FillImsTimerReqBitAndPara(&(imsTimer->imsTimer.timerKValue),
                                     &(bitOpValueFlg), AT_DMTIMER_TIMER_TK_INDEX) != VOS_OK) {
        AT_ERR_LOG("AT_FillImsTimerReqData: IMS Timer para is invalid");
        return VOS_ERR;
    }

    imsTimer->imsTimer.opTimerKValue = bitOpValueFlg;

    return VOS_OK;
}


VOS_VOID At_FillImsaTimerParaInCmd(VOS_UINT16 *length, VOS_UINT32 value, VOS_UINT32 valueValidFlg,
                                   VOS_UINT32 lastParaFlg)
{
    /* 数据有效 */
    if (valueValidFlg == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", value);
    }
    if (lastParaFlg == VOS_FALSE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_UINT32 AT_SetImsTimerPara(VOS_UINT8 indexNum)
{
    AT_IMSA_ImstimerSetReq timer;

    VOS_UINT32 result;

    /* 不是设置命令则出错 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetImsTimerPara():ucCmdOptType is not AT_CMD_OPT_SET_PARA_CMD");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 14个参数 */
    if (g_atParaIndex != AT_DMTIMER_PARA_NUM) {
        AT_ERR_LOG("AT_SetImsTimerPara():incorrect parameters");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 组装发给IMSA的消息 */
    if (AT_FillImsTimerReqData(&timer) != VOS_OK) {
        AT_ERR_LOG("AT_SetImsTimerPara():incorrect parameters");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_IMSTIMER_SET_REQ, (VOS_UINT8 *)&timer.imsTimer,
                                    (VOS_UINT32)sizeof(IMSA_AT_ImsTimer), PS_PID_IMSA);
    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetImsTimerPara():AT_FillAndSndAppReqMsg fail");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMTIMER_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetImsSmsPsiPara(VOS_UINT8 indexNum)
{
    AT_IMSA_SmspsiSetReq smsPsi;
    VOS_UINT32           result;
    errno_t              memResult;

    /* 清空 */
    (VOS_VOID)memset_s(&smsPsi, sizeof(AT_IMSA_SmspsiSetReq), 0x00, sizeof(AT_IMSA_SmspsiSetReq));

    /* 不是设置命令则出错 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetImsSmsPsiPara():WARNING:ucCmdOptType is not AT_CMD_OPT_SET_PARA_CMD");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 判断参数个数 */
    if (g_atParaIndex != 1) {
        AT_ERR_LOG("AT_SetImsSmsPsiPara: incorrect parameter, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }


    if (g_atParaList[0].paraLen > AT_IMSA_MAX_SMSPSI_LEN) {
        AT_ERR_LOG("AT_SetImsSmsPsiPara: <SMS_PSI> parameter over boundary , return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 组装发给IMSA的消息 */
    if (g_atParaList[0].paraLen != 0) {
        memResult = memcpy_s(smsPsi.smsPsi.smsPsi, sizeof(smsPsi.smsPsi.smsPsi), g_atParaList[0].para,
                             g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsPsi.smsPsi.smsPsi), g_atParaList[0].paraLen);
    }

    /* 给IMSA发送设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_SMSPSI_SET_REQ, (VOS_UINT8 *)&smsPsi.smsPsi,
                                    (VOS_UINT32)sizeof(IMSA_SMS_Psi), PS_PID_IMSA);
    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetImsSmsPsiPara():WARNING:AT_FillAndSndAppReqMsg fail");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSPSI_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryImsTimerPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG1("AT_QryImsTimerPara: cmt type is not AT_CMD_OPT_READ_CMD", g_atParseCmd.cmdOptType);
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_IMSTIMER_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_QryImsTimerPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMTIMER_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryImsSmsPsiPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_SMSPSI_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_QryImsSmsPsiPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSPSI_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDmUserPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_DMUSER_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_QryDmUserPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMUSER_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaImsTimerSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImstimerSetCnf *cmTimerCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    indexNum   = 0;
    cmTimerCnf = (IMSA_AT_ImstimerSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cmTimerCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaImsTimerSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaImsTimerSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMTIMER_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMTIMER_SET) {
        AT_ERR_LOG("AT_RcvImsaImsTimerSetCnf: WARNING:Not AT_CMD_DMTIMER_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (cmTimerCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImsTimerQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImstimerQryCnf *imsTimerCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;
    VOS_UINT16              length;

    /* 初始化消息变量 */
    indexNum    = 0;
    length      = 0;
    imsTimerCnf = (IMSA_AT_ImstimerQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsTimerCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaImsTimerQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaImsTimerQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMTIMER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMTIMER_QRY) {
        AT_ERR_LOG("AT_RcvImsaImsTimerQryCnf: WARNING:Not AT_CMD_DMTIMER_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (imsTimerCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <Timer_T1> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timer1Value, imsTimerCnf->imsTimer.opTimer1Value,
                                  VOS_FALSE);

        /* <Timer_T2> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timer2Value, imsTimerCnf->imsTimer.opTimer2Value,
                                  VOS_FALSE);

        /* <Timer_T4> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timer4Value, imsTimerCnf->imsTimer.opTimer4Value,
                                  VOS_FALSE);

        /* <Timer_TA> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerAValue, imsTimerCnf->imsTimer.opTimerAValue,
                                  VOS_FALSE);

        /* <Timer_TB> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerBValue, imsTimerCnf->imsTimer.opTimerBValue,
                                  VOS_FALSE);

        /* <Timer_TC> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerCValue, imsTimerCnf->imsTimer.opTimerCValue,
                                  VOS_FALSE);

        /* <Timer_TD> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerDValue, imsTimerCnf->imsTimer.opTimerDValue,
                                  VOS_FALSE);

        /* <Timer_TE> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerEValue, imsTimerCnf->imsTimer.opTimerEValue,
                                  VOS_FALSE);

        /* <Timer_TF> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerFValue, imsTimerCnf->imsTimer.opTimerFValue,
                                  VOS_FALSE);

        /* <Timer_TG> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerGValue, imsTimerCnf->imsTimer.opTimerGValue,
                                  VOS_FALSE);

        /* <Timer_TH> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerHValue, imsTimerCnf->imsTimer.opTimerHValue,
                                  VOS_FALSE);

        /* <Timer_TI> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerIValue, imsTimerCnf->imsTimer.opTimerIValue,
                                  VOS_FALSE);

        /* <Timer_TJ> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerJValue, imsTimerCnf->imsTimer.opTimerJValue,
                                  VOS_FALSE);

        /* <Timer_TK> */
        At_FillImsaTimerParaInCmd(&length, imsTimerCnf->imsTimer.timerKValue, imsTimerCnf->imsTimer.opTimerKValue,
                                  VOS_TRUE);

        g_atSendDataBuff.bufLen = length;

        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaImsPsiSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_SmspsiSetCnf *smsPsiCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    /* 初始化消息变量 */
    indexNum  = 0;
    smsPsiCnf = (IMSA_AT_SmspsiSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(smsPsiCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaImsPsiSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaImsPsiSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型AT_CMD_IMSPSI_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSPSI_SET) {
        AT_ERR_LOG("AT_RcvImsaImsPsiSetCnf: WARNING:Not AT_CMD_IMSPSI_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (smsPsiCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImsPsiQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_SmspsiQryCnf *smsPsiCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    /* 变量初始化 */
    indexNum  = 0;
    smsPsiCnf = (IMSA_AT_SmspsiQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(smsPsiCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaImsPsiQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaImsPsiQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作是否是AT_CMD_IMSPSI_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSPSI_QRY) {
        AT_ERR_LOG("AT_RcvImsaImsPsiQryCnf: WARNING:Not AT_CMD_IMSPSI_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (smsPsiCnf->result == VOS_OK) {
        smsPsiCnf->smsPsi.smsPsi[AT_IMSA_MAX_SMSPSI_LEN] = '\0';
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: \"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
            smsPsiCnf->smsPsi.smsPsi);

        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaDmUserQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DmuserQryCnf *dmUserCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;
    TAF_NVIM_SmsDomain    nvSmsDomain;

    /* 初始化消息变量 */
    indexNum  = 0;
    dmUserCnf = (IMSA_AT_DmuserQryCnf *)msg;

    (VOS_VOID)memset_s(&nvSmsDomain, sizeof(TAF_NVIM_SmsDomain), 0x00, sizeof(TAF_NVIM_SmsDomain));

    /* 读NV项NV_ITEM_SMS_DOMAIN，失败，直接返回 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SMS_DOMAIN, &nvSmsDomain, (VOS_UINT32)sizeof(TAF_NVIM_SmsDomain)) !=
        NV_OK) {
        AT_ERR_LOG("AT_RcvImsaDmUserQryCnf():WARNING: read NV_ITEM_SMS_DOMAIN Error");

        return VOS_ERR;
    }

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dmUserCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvImsaDmUserQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvImsaDmUserQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMUSER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMUSER_QRY) {
        AT_ERR_LOG("AT_RcvImsaDmUserQryCnf: WARNING:Not AT_CMD_DMUSER_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (dmUserCnf->result == VOS_OK) {
        dmUserCnf->dmUser.impi[AT_IMSA_IMPI_MAX_LENGTH]             = '\0';
        dmUserCnf->dmUser.impu[AT_IMSA_IMPU_MAX_LENGTH]             = '\0';
        dmUserCnf->dmUser.homeNetWorkDomain[IMSA_IMS_STRING_LENGTH] = '\0';
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: \"%s\",\"%s\",\"%s\",%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, dmUserCnf->dmUser.impi, dmUserCnf->dmUser.impu,
                dmUserCnf->dmUser.homeNetWorkDomain, dmUserCnf->dmUser.voiceDomain, nvSmsDomain.smsDomain,
                dmUserCnf->dmUser.ipsecEnable);

        result = AT_OK;

    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetNickNamePara(TAF_UINT8 indexNum)
{
    AT_IMSA_NicknameSetReq nickname;
    VOS_UINT32             result;
    errno_t                memResult;
    VOS_UINT16             maxNickNameLen;

    (VOS_VOID)memset_s(&nickname, sizeof(nickname), 0x00, sizeof(nickname));
    maxNickNameLen = 2 * (MN_CALL_DISPLAY_NAME_STRING_SZ - 1); /* UTF8编码格式，最大长度为256个字符 */

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNickNamePara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetNickNamePara: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[AT_NICKNAME_NICK_NAME].paraLen == 0) ||
        (maxNickNameLen < g_atParaList[AT_NICKNAME_NICK_NAME].paraLen) ||
        /* 取值只能为偶数 */
        (g_atParaList[AT_NICKNAME_NICK_NAME].paraLen % 2 != 0)) {
        AT_WARN_LOG("AT_SetNickNamePara: para len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (At_AsciiNum2HexString(g_atParaList[AT_NICKNAME_NICK_NAME].para,
        &g_atParaList[AT_NICKNAME_NICK_NAME].paraLen) != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetNickNamePara: At_AsciiNum2HexString is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    nickname.nickName.nickNameLen = (VOS_UINT8)g_atParaList[AT_NICKNAME_NICK_NAME].paraLen;
    if (nickname.nickName.nickNameLen > 0) {
        memResult = memcpy_s(nickname.nickName.nickName, nickname.nickName.nickNameLen,
                             g_atParaList[AT_NICKNAME_NICK_NAME].para, g_atParaList[AT_NICKNAME_NICK_NAME].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, nickname.nickName.nickNameLen, g_atParaList[AT_NICKNAME_NICK_NAME].paraLen);
    }
    nickname.nickName.nickName[nickname.nickName.nickNameLen] = '\0';

    /* 给IMSA发送^NICKNAME设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_NICKNAME_SET_REQ,
                                    (VOS_UINT8 *)&(nickname.nickName), (VOS_UINT32)sizeof(IMSA_AT_NicknameInfo),
                                    PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNickNamePara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NICKNAME_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryNickNamePara(TAF_UINT8 indexNum)
{
    AT_IMSA_NicknameQryReq nickNameQryReq;
    VOS_UINT32             result;

    (VOS_VOID)memset_s(&nickNameQryReq, sizeof(nickNameQryReq), 0x00, sizeof(nickNameQryReq));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送^NICKNAME查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_NICKNAME_QRY_REQ, VOS_NULL_PTR, 0,
                                    PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNickNamePara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NICKNAME_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaNickNameSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_NicknameSetCnf *nickNameSetCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化消息变量 */
    indexNum       = 0;
    nickNameSetCnf = (IMSA_AT_NicknameSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(nickNameSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaNickNameSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaNickNameSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NICKNAME_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NICKNAME_SET) {
        AT_WARN_LOG("AT_RcvImsaNickNameSetCnf: WARNING:Not AT_CMD_NICKNAME_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (nickNameSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaNickNameQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_NicknameQryCnf *nickNameQryCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT16              length;
    VOS_UINT8               indexNum;
    VOS_UINT8               i;

    /* 初始化消息变量 */
    indexNum       = 0;
    length         = 0;
    i              = 0;
    nickNameQryCnf = (IMSA_AT_NicknameQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(nickNameQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaNickNameQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaNickNameQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NICKNAME_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NICKNAME_QRY) {
        AT_WARN_LOG("AT_RcvImsaNickNameQryCnf: WARNING:Not AT_CMD_NICKNAME_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if ((nickNameQryCnf->result == VOS_OK) && (nickNameQryCnf->nickName.nickNameLen < MN_CALL_DISPLAY_NAME_STRING_SZ)) {
        if (nickNameQryCnf->nickName.nickNameLen != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            for (i = 0; i < nickNameQryCnf->nickName.nickNameLen; i++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%X", (VOS_UINT8)nickNameQryCnf->nickName.nickName[i]);
            }
        } else {
            AT_WARN_LOG("AT_RcvImsaNickNameQryCnf: WARNING: ucNickNameLen is 0!");
        }

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImsRegFailInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RegFailInd *regFailInd = VOS_NULL_PTR;
    VOS_UINT8           indexNum;

    /* 初始化消息变量 */
    indexNum   = 0;
    regFailInd = (IMSA_AT_RegFailInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(regFailInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsRegFailInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_IMS_REG_FAIL].text,
        regFailInd->regFailInfo.failCode, g_atCrLf);
    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetBatteryInfoPara(TAF_UINT8 indexNum)
{
    AT_ModemImsContext       *localBatteryInfo = VOS_NULL_PTR;
    AT_IMSA_BatteryInfoSetReq batteryInfoSetReq;
    VOS_UINT32                result;

    (VOS_VOID)memset_s(&batteryInfoSetReq, sizeof(batteryInfoSetReq), 0x00, sizeof(batteryInfoSetReq));
    localBatteryInfo = AT_GetModemImsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetBatteryInfoPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetBatteryInfoPara: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetBatteryInfoPara: para len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    batteryInfoSetReq.batteryStatusInfo.batteryStatus = (AT_IMSA_BatteryStatusUint8)g_atParaList[0].paraValue;

    /* 给IMSA发送^BATTERYINFO设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_BATTERYINFO_SET_REQ,
                                    (VOS_UINT8 *)&(batteryInfoSetReq.batteryStatusInfo),
                                    (VOS_UINT32)sizeof(AT_IMSA_BatteryStatusInfo), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetBatteryInfoPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    localBatteryInfo->batteryInfo.tempBatteryInfo = batteryInfoSetReq.batteryStatusInfo.batteryStatus;

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BATTERYINFO_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaBatteryInfoSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_BatteryInfoSetCnf *batteryInfoSetCnf = VOS_NULL_PTR;
    AT_ModemImsContext        *localBatteryInfo  = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;

    /* 初始化消息变量 */
    indexNum          = 0;
    localBatteryInfo  = AT_GetModemImsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);
    batteryInfoSetCnf = (IMSA_AT_BatteryInfoSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(batteryInfoSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaBatteryInfoSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaBatteryInfoSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_BATTERYINFO_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BATTERYINFO_SET) {
        AT_WARN_LOG("AT_RcvImsaBatteryInfoSetCnf: WARNING:Not AT_CMD_BATTERYINFO_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (batteryInfoSetCnf->result == VOS_OK) {
        localBatteryInfo->batteryInfo.currBatteryInfo = localBatteryInfo->batteryInfo.tempBatteryInfo;
        localBatteryInfo->batteryInfo.tempBatteryInfo = AT_IMSA_BATTERY_STATUS_BUTT;
        AT_NORM_LOG1("AT_RcvImsaBatteryInfoSetCnf: Local enBatteryInfo is ",
                     localBatteryInfo->batteryInfo.currBatteryInfo);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryBatteryInfoPara(TAF_UINT8 indexNum)
{
    AT_ModemImsContext *localBatteryInfo = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    localBatteryInfo = AT_GetModemImsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);

    if (localBatteryInfo->batteryInfo.currBatteryInfo < AT_IMSA_BATTERY_STATUS_BUTT) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            localBatteryInfo->batteryInfo.currBatteryInfo);

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetVolteRegPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送^VOLTEREG查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_VOLTEREG_NTF, VOS_NULL_PTR, 0,
                                    PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetVolteRegPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetImsVideoCallCancelPara(TAF_UINT8 indexNum)
{
    TAF_Ctrl                  ctrl;
    VOS_UINT32                callId;
    VOS_UINT32                result;
    ModemIdUint16             modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetImsVideoCallCancelPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetImsVideoCallCancelPara: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    callId        = g_atParaList[0].paraValue;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 给CCM发送^IMSVIDEOCALLCANCEL查询请求 */
    result = TAF_CCM_CallCommonReq(&ctrl, &callId, ID_TAF_CCM_CANCEL_ADD_VIDEO_REQ, sizeof(callId), modemId);

    if (result != VOS_OK) {
        AT_WARN_LOG("AT_SetImsVideoCallCancelPara: TAF_CCM_CallCommonReq is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSVIDEOCALLCANCEL_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_ParseImsvtcapcfgPara(TAF_MMA_ImsVideoCallCap *imsVtCap)
{
    switch (g_atParaList[0].paraValue) {
        case AT_VIDEO_CALL_CAP_SWITCH:
            imsVtCap->videoCallCapType  = TAF_MMA_IMS_VIDEO_CALL_CAP_SWITCH;
            imsVtCap->videoCallCapValue = g_atParaList[1].paraValue;
            return VOS_TRUE;

        case AT_VIDEO_CALL_CAP_CCWA:
            imsVtCap->videoCallCapType  = TAF_MMA_IMS_VIDEO_CALL_CAP_CCWA;
            imsVtCap->videoCallCapValue = g_atParaList[1].paraValue;
            return VOS_TRUE;

        default:
            return VOS_FALSE;
    }
}


VOS_UINT32 AT_SetImsVtCapCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32              rst;
    TAF_MMA_ImsVideoCallCap imsVtCap;

    /* 初始化结构体 */
    (VOS_VOID)memset_s(&imsVtCap, sizeof(imsVtCap), 0x00, sizeof(imsVtCap));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_IMSVTCAPCFG_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数值不对 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (At_ParseImsvtcapcfgPara(&imsVtCap) != VOS_TRUE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    rst = TAF_MMA_SetImsVtCapCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &imsVtCap);

    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSVTCAPCFG_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaImsVideoCallCapSetCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsVideoCallCapSetCnf *imsVtCapSetCnf = VOS_NULL_PTR;
    VOS_UINT32                     result;
    VOS_UINT8                      indexNum;

    indexNum = 0;
    imsVtCapSetCnf = (TAF_MMA_ImsVideoCallCapSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(imsVtCapSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsVideoCallCapSetCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsVideoCallCapSetCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSSMSCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSVTCAPCFG_SET) {
        AT_WARN_LOG("AT_RcvMmaImsVideoCallCapSetCnf : WARNING:Not AT_CmdImsSmsCfg_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (imsVtCapSetCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, imsVtCapSetCnf->result);
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetImsSmsCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32        rst;
    TAF_MMA_ImsSmsCfg imsSmsCfg;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex != AT_IMSSMSCFG_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0) ||
        (g_atParaList[AT_IMSSMSCFG_UTRAN_ENABLE].paraLen == 0) ||
        (g_atParaList[AT_IMSSMSCFG_GSM_ENABLE].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&imsSmsCfg, sizeof(imsSmsCfg), 0, sizeof(imsSmsCfg));

    imsSmsCfg.wifiEnable  = (VOS_TRUE == g_atParaList[0].paraValue) ? VOS_TRUE : VOS_FALSE;
    imsSmsCfg.lteEnable   = (VOS_TRUE == g_atParaList[1].paraValue) ? VOS_TRUE : VOS_FALSE;
    imsSmsCfg.utranEnable = (VOS_TRUE == g_atParaList[AT_IMSSMSCFG_UTRAN_ENABLE].paraValue) ? VOS_TRUE : VOS_FALSE;
    imsSmsCfg.gsmEnable   = (VOS_TRUE == g_atParaList[AT_IMSSMSCFG_GSM_ENABLE].paraValue) ? VOS_TRUE : VOS_FALSE;

    /* 执行命令操作 */
    rst = TAF_MMA_SetImsSmsCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &imsSmsCfg);

    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSSMSCFG_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryImsSmsCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MMA 发送查询请求消息 */
    result = TAF_MMA_QryImsSmsCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);

    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSSMSCFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaImsSmsCfgSetCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsSmsCfgSetCnf *imsSmsCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum = 0;

    imsSmsCfgSetCnf = (TAF_MMA_ImsSmsCfgSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(imsSmsCfgSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgSetCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgSetCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSSMSCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSSMSCFG_SET) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgSetCnf : WARNING:Not AT_CmdImsSmsCfg_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (imsSmsCfgSetCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, imsSmsCfgSetCnf->result);
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaImsSmsCfgQryCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsSmsCfgQryCnf *imsSmsCfgQryCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum        = 0;
    VOS_UINT32               result;

    imsSmsCfgQryCnf = (TAF_MMA_ImsSmsCfgQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(imsSmsCfgQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgQryCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSSMSCFG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSSMSCFG_QRY) {
        AT_WARN_LOG("AT_RcvMmaImsSmsCfgQryCnf : WARNING:Not AT_CMD_IMSSMSCFG_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (imsSmsCfgQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                imsSmsCfgQryCnf->imsSmsCfg.wifiEnable, imsSmsCfgQryCnf->imsSmsCfg.lteEnable,
                imsSmsCfgQryCnf->imsSmsCfg.utranEnable, imsSmsCfgQryCnf->imsSmsCfg.gsmEnable);

        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, imsSmsCfgQryCnf->result);
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_SetImsRegErrRpt(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;
    VOS_UINT8  reportFlag;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 当前只能使能或不使能ims注册错误原因值上报，故使用VOS_TRUE和VOS_FALSE标示 */
    reportFlag = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 给IMSA发送^IMSREGERRRPT设置请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_REGERR_REPORT_SET_REQ, &(reportFlag), (VOS_UINT32)sizeof(reportFlag),
                                 PS_PID_IMSA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSREGERRRPT_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaRegErrRptSetCnf(struct MsgCB *msg)
{
    IMSA_AT_RegerrReportSetCnf *errRptSetCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;

    indexNum     = 0;
    errRptSetCnf = (IMSA_AT_RegerrReportSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(errRptSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptSetCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptSetCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSREGERRRPT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSREGERRRPT_SET) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptSetCnf : WARNING:Not AT_CMD_IMSREGERRRPT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (errRptSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryImsRegErrRpt(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG1("AT_QryImsRegErrRpt: cmt type is not AT_CMD_OPT_READ_CMD", g_atParseCmd.cmdOptType);
        return AT_ERROR;
    }

    /* 给IMSA发送^IMSREGERRRPT查询请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_REGERR_REPORT_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSREGERRRPT_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaRegErrRptQryCnf(struct MsgCB *msg)
{
    IMSA_AT_RegerrReportQryCnf *errRptQryCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;
    VOS_UINT32                  result;

    indexNum     = 0;
    errRptQryCnf = (IMSA_AT_RegerrReportQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(errRptQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptQryCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSREGERRRPT_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSREGERRRPT_QRY) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptQryCnf : WARNING:Not AT_CMD_IMSREGERRRPT_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (errRptQryCnf->result == VOS_OK) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                errRptQryCnf->reportFlag);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRegErrRptInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RegerrReportInd *regErrInd = VOS_NULL_PTR;
    VOS_UINT32               failStage;
    VOS_UINT32               failCause;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    regErrInd = (IMSA_AT_RegerrReportInd *)msg;
    failStage = 0;
    failCause = 0;
    indexNum  = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(regErrInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRegErrRptInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 添加异常原因值检查 */
    if (AT_IsImsRegErrRptParaValid(msg) != VOS_TRUE) {
        return VOS_ERR;
    }

    if (regErrInd->imsaRegErrType == IMSA_AT_REG_ERR_TYPE_PDN_FAIL) {
        failStage = regErrInd->imsaRegErrType;
        failCause = regErrInd->imsaPdnFailCause;
    }

    if (regErrInd->imsaRegErrType == IMSA_AT_REG_ERR_TYPE_IMS_REG_FAIL) {
        failStage = regErrInd->imsaRegErrType;
        failCause = regErrInd->imsaRegFailCause;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s %d,%u,%u,\"%s\"%s", g_atCrLf, g_atStringTab[AT_STRING_IMS_REG_ERR].text,
        regErrInd->imsaRegDomain, failStage, failCause, regErrInd->imsRegFailReasonCtx, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_IsImsRegErrRptParaValid(struct MsgCB *msg)
{
    IMSA_AT_RegerrReportInd *regErrInd = VOS_NULL_PTR;
    VOS_UINT32               strLen;

    /* 初始化消息变量 */
    regErrInd = (IMSA_AT_RegerrReportInd *)msg;
    strLen    = 0;

    /* 如果注册域为非wifi和非lte，认为上报异常 */
    if (regErrInd->imsaRegDomain >= IMSA_AT_IMS_REG_DOMAIN_TYPE_UNKNOWN) {
        return VOS_FALSE;
    }

    /* 如果注册失败类型为非pdn和非reg，认为上报异常 */
    if (regErrInd->imsaRegErrType >= IMSA_AT_REG_ERR_TYPE_BUTT) {
        return VOS_FALSE;
    }

    /* PDN类型失败，但失败原因值大于pdn失败原因值的最大值，则认为上报异常 */
    if ((regErrInd->imsaRegErrType == IMSA_AT_REG_ERR_TYPE_PDN_FAIL) &&
        (regErrInd->imsaPdnFailCause >= IMSA_AT_PDN_FAIL_CAUSE_BUTT)) {
        return VOS_FALSE;
    }

    /* reg类型失败，但失败原因值大于reg失败原因值的最大值，则认为上报异常 */
    if ((regErrInd->imsaRegErrType == IMSA_AT_REG_ERR_TYPE_IMS_REG_FAIL) &&
        (regErrInd->imsaRegFailCause >= IMSA_AT_REG_FAIL_CAUSE_BUTT)) {
        return VOS_FALSE;
    }

    strLen = VOS_StrNLen(regErrInd->imsRegFailReasonCtx, IMSA_AT_REG_FAIL_CAUSE_STR_MAX_LEN);

    /* 失败字符串约定最大长度为255，大于255，认为上报异常 */
    if (strLen == IMSA_AT_REG_FAIL_CAUSE_STR_MAX_LEN) {
        AT_ERR_LOG1("AT_IsImsRegErrRptParaValid: str len beyond IMSA_AT_REG_FAIL_CAUSE_STR_MAX_LEN!", strLen);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_SetImsSrvStatRptCfgPara(TAF_UINT8 indexNum)
{
    AT_IMSA_ImsSrvStatReportSetReq imsSrvStatRptSetReq;
    VOS_UINT32                     result;

    (VOS_VOID)memset_s(&imsSrvStatRptSetReq, sizeof(imsSrvStatRptSetReq), 0x00, sizeof(imsSrvStatRptSetReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetImsSrvStatRpt: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetImsSrvStatRpt: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetImsSrvStatRpt: para len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    imsSrvStatRptSetReq.imsSrvStatRpt = (AT_IMSA_ImsSrvStatReportUint32)g_atParaList[0].paraValue;

    /* 给IMSA发送^IMSSRVSTATRPT设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_IMS_SRV_STAT_RPT_SET_REQ,
                                    (VOS_UINT8 *)&(imsSrvStatRptSetReq.imsSrvStatRpt),
                                    (VOS_UINT32)sizeof(AT_IMSA_ImsSrvStatReportUint32), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetImsSrvStatRpt: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMS_SRV_STAT_RPT_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaImsSrvStatRptSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsSrvStatReportSetCnf *imsSrvStatRptSetCnf = VOS_NULL_PTR;
    VOS_UINT32                      result;
    VOS_UINT8                       indexNum;

    /* 初始化消息变量 */
    indexNum            = 0;
    imsSrvStatRptSetCnf = (IMSA_AT_ImsSrvStatReportSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsSrvStatRptSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMS_SRV_STAT_RPT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMS_SRV_STAT_RPT_SET) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptSetCnf: WARNING:Not AT_CMD_IMS_SRV_STAT_RPT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsSrvStatRptSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryImsSrvStatRptCfgPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG1("AT_QryImsSrvStatRptCfgPara: cmt type is not AT_CMD_OPT_READ_CMD", g_atParseCmd.cmdOptType);
        return AT_ERROR;
    }

    /* 给IMSA发送^IMSSRVSTATRPT查询请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_IMS_SRV_STAT_RPT_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMS_SRV_STAT_RPT_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaImsSrvStatRptQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsSrvStatReportQryCnf *imsSrvStatRptQryCnf = VOS_NULL_PTR;
    VOS_UINT32                      result;
    VOS_UINT16                      length;
    VOS_UINT8                       indexNum;

    /* 初始化消息变量 */
    indexNum            = 0;
    length              = 0;
    imsSrvStatRptQryCnf = (IMSA_AT_ImsSrvStatReportQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsSrvStatRptQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMS_SRV_STAT_RPT_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMS_SRV_STAT_RPT_QRY) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatRptQryCnf: WARNING:Not AT_CMD_IMS_SRV_STAT_RPT_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (imsSrvStatRptQryCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_UINT32)(imsSrvStatRptQryCnf->imsSrvStatRpt));

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryImsSrvStatusPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG1("AT_QryImsSrvStatusPara: cmt type is not AT_CMD_OPT_READ_CMD", g_atParseCmd.cmdOptType);
        return AT_ERROR;
    }

    /* 给IMSA发送^IMSSRVSTATUS查询请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_IMS_SERVICE_STATUS_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMS_SERVICE_STATUS_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaImsSrvStatusQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsServiceStatusQryCnf *imsSrvStatusQryCnf = VOS_NULL_PTR;
    VOS_UINT32                      result;
    VOS_UINT16                      length;
    VOS_UINT8                       indexNum;

    /* 初始化消息变量 */
    indexNum           = 0;
    length             = 0;
    imsSrvStatusQryCnf = (IMSA_AT_ImsServiceStatusQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsSrvStatusQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatusQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatusQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMS_SERVICE_STATUS_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMS_SERVICE_STATUS_QRY) {
        AT_WARN_LOG("AT_RcvImsaImsSrvStatusQryCnf: WARNING:Not AT_CMD_IMS_SERVICE_STATUS_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (imsSrvStatusQryCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, imsSrvStatusQryCnf->imsSrvStatInfo.smsSrvStatus,
            imsSrvStatusQryCnf->imsSrvStatInfo.smsSrvRat, imsSrvStatusQryCnf->imsSrvStatInfo.voIpSrvStatus,
            imsSrvStatusQryCnf->imsSrvStatInfo.voIpSrvRat, imsSrvStatusQryCnf->imsSrvStatInfo.vtSrvStatus,
            imsSrvStatusQryCnf->imsSrvStatInfo.vtSrvRat, imsSrvStatusQryCnf->imsSrvStatInfo.vsSrvStatus,
            imsSrvStatusQryCnf->imsSrvStatInfo.vsSrvRat);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetEmcAIdPara(VOS_UINT8 indexNum)
{
    AT_IMSA_EmergencyAidInfo emcAIdInfo;
    VOS_UINT32               result;
    errno_t                  memResult;

    (VOS_VOID)memset_s(&emcAIdInfo, sizeof(emcAIdInfo), 0x00, sizeof(emcAIdInfo));

    /* 参数类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数错误 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if (g_atParaList[0].paraLen > AT_IMSA_MAX_EMERGENCY_AID_LEN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    emcAIdInfo.addressIdLen = g_atParaList[0].paraLen;

    if (emcAIdInfo.addressIdLen != 0) {
        memResult = memcpy_s(emcAIdInfo.addressId, sizeof(emcAIdInfo.addressId), g_atParaList[0].para,
                             g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(emcAIdInfo.addressId), g_atParaList[0].paraLen);
    }

    /* 给IMSA发送^WIEMCAID设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_IMSA_EMERGENCY_AID_SET_REQ,
                                    (VOS_UINT8 *)&emcAIdInfo, (VOS_UINT32)sizeof(AT_IMSA_EmergencyAidInfo),
                                    PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetImsSrvStatRpt: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_WIEMCAID_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaEmcAIdSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_EmergencyAidSetCnf *imsEmcAidSetCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;

    /* 初始化消息变量 */
    indexNum        = 0;
    imsEmcAidSetCnf = (IMSA_AT_EmergencyAidSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsEmcAidSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEmcAIdSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaEmcAIdSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_WIEMCAID_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_WIEMCAID_SET) {
        AT_WARN_LOG("AT_RcvImsaEmcAIdSetCnf: WARNING:Not AT_CMD_WIEMCAID_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsEmcAidSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCallAltSrvInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_CallAltSrvInd *callAltSrvInd = VOS_NULL_PTR;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum      = 0;
    callAltSrvInd = (IMSA_AT_CallAltSrvInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(callAltSrvInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaCallAltSrvInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_CALL_ALT_SRV].text, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetDmRcsCfgPara(VOS_UINT8 indexNum)
{
    AT_IMSA_DmRcsCfgSetPara dmRcsCfgPara;
    VOS_UINT32              result;

    /* 清空 */
    (VOS_VOID)memset_s(&dmRcsCfgPara, sizeof(AT_IMSA_DmRcsCfgSetPara), 0x00, sizeof(AT_IMSA_DmRcsCfgSetPara));

    /* 不是设置命令则出错 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetDmRcsCfgPara():WARNING:ucCmdOptType is not AT_CMD_OPT_SET_PARA_CMD");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 判断参数是否有效 */
    if ((g_atParaIndex != AT_DMRCSCFG_PARA_NUM) || (g_atParaList[AT_DMRCSCFG_FEATURETAG].paraLen == 0) ||
        (g_atParaList[AT_DMRCSCFG_DEVCFG].paraLen == 0)) {
        AT_ERR_LOG("AT_SetDmRcsCfgPara: incorrect parameter, return ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    dmRcsCfgPara.featureTag = g_atParaList[AT_DMRCSCFG_FEATURETAG].paraValue;
    dmRcsCfgPara.devCfg     = g_atParaList[AT_DMRCSCFG_DEVCFG].paraValue;

    /* 给IMSA发送设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_DM_RCS_CFG_SET_REQ, (VOS_UINT8 *)&dmRcsCfgPara,
                                    (VOS_UINT32)sizeof(AT_IMSA_DmRcsCfgSetPara), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetDmRcsCfgPara():WARNING:AT_FillAndSndAppReqMsg fail");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DMRCSCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetRcsSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32            rst;
    TAF_MMA_ImsSwitchInfo imsSwitchInfo;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != 1) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&imsSwitchInfo, sizeof(imsSwitchInfo), 0, sizeof(imsSwitchInfo));

    imsSwitchInfo.bitOpRcsSwitch = VOS_TRUE;
    imsSwitchInfo.rcsSwitch      = g_atParaList[0].paraValue;

    rst = TAF_MMA_SetImsSwitchReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                  &imsSwitchInfo);
    if (rst != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RCSSWITCH_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryRcsSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MMA 发送查询请求消息 */
    rst = TAF_MMA_QryRcsSwitchReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RCSSWITCH_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaDmRcsCfgSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DmRcsCfgSetCnf *dmRcsCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化消息变量 */
    indexNum       = 0;
    dmRcsCfgSetCnf = (IMSA_AT_DmRcsCfgSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dmRcsCfgSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaDmRcsCfgSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaDmRcsCfgSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DMRCSCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DMRCSCFG_SET) {
        AT_WARN_LOG("AT_RcvImsaDmRcsCfgSetCnf: WARNING:Not AT_CMD_DMRCSCFG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (dmRcsCfgSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvMmaRcsSwitchQryCnf(struct MsgCB *msg)
{
    TAF_MMA_RcsSwitchQryCnf *rcsSwitchQryCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum        = 0;
    VOS_UINT32               result;

    rcsSwitchQryCnf = (TAF_MMA_RcsSwitchQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcsSwitchQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRcsSwitchQryCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRcsSwitchQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_RCSSWITCH_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RCSSWITCH_QRY) {
        AT_WARN_LOG("AT_RcvMmaRcsSwitchQryCnf : WARNING:Not AT_CMD_RCSSWITCH_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (rcsSwitchQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                rcsSwitchQryCnf->rcsSwitch);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_VOID AT_ProcUserAgentPara(AT_IMSA_UserAgentCfg *userAgentCfg)
{
    errno_t memResult;

    /* 给参数赋值 */
    if (g_atParaList[0].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para1, sizeof(userAgentCfg->para1), g_atParaList[0].para,
                             g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para1), g_atParaList[0].paraLen);
    }

    if (g_atParaList[1].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para2, sizeof(userAgentCfg->para2), g_atParaList[1].para,
                             g_atParaList[1].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para2), g_atParaList[1].paraLen);
    }

    if (g_atParaList[AT_USERAGENTCFG_PARA3].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para3, sizeof(userAgentCfg->para3),
                             g_atParaList[AT_USERAGENTCFG_PARA3].para,
                             g_atParaList[AT_USERAGENTCFG_PARA3].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para3), g_atParaList[AT_USERAGENTCFG_PARA3].paraLen);
    }

    if (g_atParaList[AT_USERAGENTCFG_PARA4].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para4, sizeof(userAgentCfg->para4),
                             g_atParaList[AT_USERAGENTCFG_PARA4].para,
                             g_atParaList[AT_USERAGENTCFG_PARA4].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para4), g_atParaList[AT_USERAGENTCFG_PARA4].paraLen);
    }

    if (g_atParaList[AT_USERAGENTCFG_PARA5].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para5, sizeof(userAgentCfg->para5),
                             g_atParaList[AT_USERAGENTCFG_PARA5].para,
                             g_atParaList[AT_USERAGENTCFG_PARA5].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para5), g_atParaList[AT_USERAGENTCFG_PARA5].paraLen);
    }

    if (g_atParaList[AT_USERAGENTCFG_PARA6].paraLen != 0) {
        memResult = memcpy_s(userAgentCfg->para6, sizeof(userAgentCfg->para6),
                             g_atParaList[AT_USERAGENTCFG_PARA6].para,
                             g_atParaList[AT_USERAGENTCFG_PARA6].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(userAgentCfg->para6), g_atParaList[AT_USERAGENTCFG_PARA6].paraLen);
    }

    return;
}


VOS_UINT32 AT_SetUserAgentCfgPara(VOS_UINT8 indexNum)
{
    AT_IMSA_UserAgentCfg userAgentCfg;
    VOS_UINT32           result;

    (VOS_VOID)memset_s(&userAgentCfg, sizeof(userAgentCfg), 0x00, sizeof(userAgentCfg));

    /* 参数类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数错误 */
    if (g_atParaIndex != AT_USERAGENTCFG_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数长度错误 */
    if ((g_atParaList[AT_USERAGENTCFG_PARA1].paraLen > AT_IMSA_USER_AGENT_STR_LEN) ||
        (g_atParaList[AT_USERAGENTCFG_PARA2].paraLen > AT_IMSA_USER_AGENT_STR_LEN) ||
        (g_atParaList[AT_USERAGENTCFG_PARA3].paraLen > AT_IMSA_USER_AGENT_STR_LEN) ||
        (g_atParaList[AT_USERAGENTCFG_PARA4].paraLen > AT_IMSA_USER_AGENT_STR_LEN) ||
        (g_atParaList[AT_USERAGENTCFG_PARA5].paraLen > AT_IMSA_USER_AGENT_STR_LEN) ||
        (g_atParaList[AT_USERAGENTCFG_PARA6].paraLen > AT_IMSA_USER_AGENT_STR_LEN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给参数赋值 */
    AT_ProcUserAgentPara(&userAgentCfg);

    /* 给IMSA发送^USERAGENTCFG设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_USER_AGENT_CFG_SET_REQ, (VOS_UINT8 *)&userAgentCfg,
                                    (VOS_UINT32)sizeof(AT_IMSA_UserAgentCfg), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetUserAgentCfgPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_USERAGENTCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaUserAgentSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_UserAgentCfgSetCnf *imsUserAgentSetCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;

    /* 初始化消息变量 */
    indexNum           = 0;
    imsUserAgentSetCnf = (IMSA_AT_UserAgentCfgSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsUserAgentSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaUserAgentSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaUserAgentSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_USERAGENTCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_USERAGENTCFG_SET) {
        AT_WARN_LOG("AT_RcvImsaUserAgentSetCnf: WARNING:Not AT_CMD_USERAGENTCFG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsUserAgentSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaImpuInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImpuTypeInd *impuInd = VOS_NULL_PTR;
    VOS_UINT8            indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    impuInd  = (IMSA_AT_ImpuTypeInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(impuInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImpuInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_IMPU].text, impuInd->impuType,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetViceCfgPara(VOS_UINT8 indexNum)
{
    AT_IMSA_VicecfgSetReq atViceCfgSetReq;
    VOS_UINT32            result;

    result = 0;
    (VOS_VOID)memset_s(&atViceCfgSetReq, sizeof(atViceCfgSetReq), 0x00, sizeof(atViceCfgSetReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetViceCfgPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetViceCfgPara: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atViceCfgSetReq.viceCfg = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 给IMSA发送^VICECFG设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_VICECFG_SET_REQ, &(atViceCfgSetReq.viceCfg),
                                    (VOS_UINT32)sizeof(atViceCfgSetReq.viceCfg), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetViceCfgPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VICECFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaViceCfgSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_VicecfgSetCnf *imsViceCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum         = 0;
    imsViceCfgSetCnf = (IMSA_AT_VicecfgSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsViceCfgSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaViceCfgSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaViceCfgSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VICECFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VICECFG_SET) {
        AT_WARN_LOG("AT_RcvImsaViceCfgSetCnf: WARNING:Not AT_CMD_VICECFG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsViceCfgSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryViceCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;
    rst = 0;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 给IMSA发送^VICECFG查询请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_VICECFG_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VICECFG_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvImsaViceCfgQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_VicecfgQryCnf *imsViceCfgQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum         = 0;
    length           = 0;
    imsViceCfgQryCnf = (IMSA_AT_VicecfgQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsViceCfgQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaViceCfgQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaViceCfgQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VICECFG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VICECFG_QRY) {
        AT_WARN_LOG("AT_RcvImsaViceCfgQryCnf: WARNING:Not AT_CMD_VICECFG_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (imsViceCfgQryCnf->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            imsViceCfgQryCnf->viceCfg);
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }
    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaDialogNtfInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_DialogNotify *dialogNtfInd = VOS_NULL_PTR;
    VOS_UINT32            loop;
    VOS_UINT16            length;
    VOS_UINT8             indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    length       = 0;
    loop         = 0;
    dialogNtfInd = (IMSA_AT_DialogNotify *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dialogNtfInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaDialogNtfInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    dialogNtfInd->txtLength = TAF_MIN(dialogNtfInd->txtLength, IMSA_AT_DIALOG_NOTIFY_MAX_LEN);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: \"", g_atStringTab[AT_STRING_DIALOGNTF].text);

    for (loop = 0; loop < dialogNtfInd->txtLength; loop++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", dialogNtfInd->notify[loop]);
    }
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetRttCfgPara(VOS_UINT8 indexNum)
{
    AT_IMSA_RttcfgSetReq atRttCfgSetReq;
    VOS_UINT32           result;

    result = 0;
    (VOS_VOID)memset_s(&atRttCfgSetReq, sizeof(atRttCfgSetReq), 0x00, sizeof(atRttCfgSetReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetRttCfgPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetRttCfgPara: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    atRttCfgSetReq.rttCfg = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 给IMSA发送^RTTCFG设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_RTTCFG_SET_REQ, &(atRttCfgSetReq.rttCfg),
                                    (VOS_UINT32)sizeof(atRttCfgSetReq.rttCfg), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetRttCfgPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RTTCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetRttModifyPara(VOS_UINT8 indexNum)
{
    VOS_UINT32              result;
    AT_IMSA_RttModifySetReq atRttModifySetReq;

    result = 0;
    (VOS_VOID)memset_s(&atRttModifySetReq, sizeof(atRttModifySetReq), 0x00, sizeof(atRttModifySetReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetRttModifyPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_RTTMODIFY_PARA_NUM) {
        AT_WARN_LOG("AT_SetRttModifyPara: para num is not equal 2!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atRttModifySetReq.rttModify.callId    = (VOS_UINT8)g_atParaList[0].paraValue;
    atRttModifySetReq.rttModify.operation = (VOS_UINT8)g_atParaList[1].paraValue;

    /* 给IMSA发送^RTTMODIFY设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_RTT_MODIFY_SET_REQ, (VOS_UINT8 *)&(atRttModifySetReq.rttModify),
                                    (VOS_UINT32)sizeof(atRttModifySetReq.rttModify), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetRttModifyPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RTTMODIFY_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaRttCfgSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RttcfgSetCnf *imsRttCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化消息变量 */
    indexNum        = 0;
    result          = 0;
    imsRttCfgSetCnf = (IMSA_AT_RttcfgSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsRttCfgSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRttCfgSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaRttCfgSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为 AT_CMD_RTTCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RTTCFG_SET) {
        AT_WARN_LOG("AT_RcvImsaRttCfgSetCnf: WARNING:Not AT_CMD_RTTCFG_SET!");
        return VOS_ERR;
    }
    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsRttCfgSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRttModifySetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RttModifySetCnf *imsRttModifySetCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum           = 0;
    result             = 0;
    imsRttModifySetCnf = (IMSA_AT_RttModifySetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsRttModifySetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRttModifySetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaRttModifySetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为 AT_CMD_RTTMODIFY_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RTTMODIFY_SET) {
        AT_WARN_LOG("AT_RcvImsaRttModifySetCnf: WARNING:Not AT_CMD_RTTMODIFY_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (imsRttModifySetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRttEventInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RttEventInd *rttEvtInd = VOS_NULL_PTR;
    VOS_UINT8            indexNum;

    /* 初始化消息变量 */
    indexNum  = 0;
    rttEvtInd = (IMSA_AT_RttEventInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rttEvtInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRttEventInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRINT_RTTEVENT].text,
        rttEvtInd->rttEvent.callId, rttEvtInd->rttEvent.event, rttEvtInd->rttEvent.evtReason, g_atCrLf);

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaRttErrorInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_RttErrorInd *rttErrInd = VOS_NULL;
    VOS_CHAR             acRttErrTxt[IMSA_AT_RTT_REASON_TEXT_MAX_LEN + 1];
    errno_t              memResult;
    VOS_UINT8            indexNum;

    /* 初始化消息变量 */
    indexNum  = 0;
    rttErrInd = (IMSA_AT_RttErrorInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rttErrInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaRttErrorInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(acRttErrTxt, sizeof(acRttErrTxt), 0x00, sizeof(acRttErrTxt));
    memResult = memcpy_s(acRttErrTxt, sizeof(acRttErrTxt), rttErrInd->rttError.reasonText,
                         IMSA_AT_RTT_REASON_TEXT_MAX_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acRttErrTxt), IMSA_AT_RTT_REASON_TEXT_MAX_LEN);

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d,%d,%d,\"%s\"%s", g_atCrLf, g_atStringTab[AT_STRINT_RTTERROR].text,
        rttErrInd->rttError.callId, rttErrInd->rttError.operation, rttErrInd->rttError.causeCode, acRttErrTxt,
        g_atCrLf);

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaSipPortSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_TransportTypeSetCnf *sipTransPort = VOS_NULL_PTR;
    VOS_UINT8                    indexNum;
    VOS_UINT32                   result;

    /* 初始化消息变量 */
    indexNum     = 0;
    sipTransPort = (IMSA_AT_TransportTypeSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(sipTransPort->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaSipPortSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaSipPortSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIREG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIPPORT_SET) {
        AT_WARN_LOG("AT_RcvImsaSipPortSetCnf: WARNING:Not AT_CMD_SIPPORT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (sipTransPort->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaSipPortQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_TransportTypeQryCnf *sipTransPort = VOS_NULL_PTR;
    VOS_UINT8                    indexNum;
    VOS_UINT32                   result;
    VOS_UINT16                   length;

    /* 初始化消息变量 */
    indexNum     = 0;
    length       = 0;
    sipTransPort = (IMSA_AT_TransportTypeQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(sipTransPort->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaSipPortQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaSipPortQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIPPORT_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIPPORT_QRY) {
        AT_WARN_LOG("AT_RcvImsaSipPortQryCnf: WARNING:Not AT_CMD_SIPPORT_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (sipTransPort->result == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            sipTransPort->tcpThreshold);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


TAF_UINT32 At_SetSipTransPort(TAF_UINT8 indexNum)
{
    AT_IMSA_TransportTypeSetReq sipTransPort;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&sipTransPort, sizeof(sipTransPort), 0x00, sizeof(sipTransPort));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数不为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    sipTransPort.tcpThreshold     = g_atParaList[0].paraValue;
    sipTransPort.appCtrl.clientId = g_atClientTab[indexNum].clientId;
    sipTransPort.appCtrl.opId     = g_atClientTab[indexNum].opId;

    /* 给IMSA发送+SIPPORT设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_TRANSPORT_TYPE_SET_REQ, (VOS_UINT8 *)&(sipTransPort.tcpThreshold),
                                    sizeof(sipTransPort.tcpThreshold), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIPPORT_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QrySipTransPort(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_TRANSPORT_TYPE_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QrySipTransPort: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIPPORT_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_ECALL == FEATURE_ON)

VOS_UINT32 AT_RcvImsaEcallContentTypeSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_EcallContentTypeSetCnf      *contentTypeSetCnf = VOS_NULL_PTR;
    VOS_UINT8                            indexNum;
    VOS_UINT32                           result;

    /* 初始化消息变量 */
    indexNum          = 0;
    contentTypeSetCnf = (IMSA_AT_EcallContentTypeSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(contentTypeSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ECLIMSCFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLIMSCFG_SET) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeSetCnf: WARNING:Not AT_CMD_ECLIMSCFG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (contentTypeSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvImsaEcallContentTypeQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_EcallContentTypeQryCnf      *contentTypeQryCnf = VOS_NULL_PTR;
    VOS_UINT8                            indexNum;
    VOS_UINT32                           result;

    indexNum          = 0;
    contentTypeQryCnf = (IMSA_AT_EcallContentTypeQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(contentTypeQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ECLIMSCFG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLIMSCFG_QRY) {
        AT_WARN_LOG("AT_RcvImsaEcallContentTypeQryCnf: WARNING:Not AT_CMD_ECLIMSCFG_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (contentTypeQryCnf->result == VOS_ERR) {
        result = AT_ERROR;
    } else {
        /* 如果CONTENT TYPE为ECALL 客户定制，则返回CONTENT TYOE设置模式及内容，否则只返回设置模式 */
        if (contentTypeQryCnf->contentTypeMode == AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_CUSTOM) {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,\"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
                contentTypeQryCnf->contentTypeMode, contentTypeQryCnf->contentTypeContext);
        } else {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                contentTypeQryCnf->contentTypeMode);
        }
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif
#endif


VOS_UINT32 At_SetEflociInfoPara(VOS_UINT8 indexNum)
{
    TAF_MMA_Eflociinfo efLociInfo;
    VOS_UINT32         plmnHex;

    /* 初始化结构体 */
    (VOS_VOID)memset_s(&efLociInfo, sizeof(efLociInfo), 0x00, sizeof(efLociInfo));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetEflociInfoPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_EFLOCIINFO_PARA_NUM) {
        AT_WARN_LOG("At_SetEflociInfoPara: PARA NUM IS NOT EQUAL 5!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 转换Tmsi */
    if (g_atParaList[AT_EFLOCIINFO_TMSI].paraLen == 0) {
        AT_WARN_LOG("At_SetEflociInfoPara: Tmsi LEN IS 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    efLociInfo.tmsi = g_atParaList[AT_EFLOCIINFO_TMSI].paraValue;

    /* 转换PLMN */
    if ((g_atParaList[AT_EFLOCIINFO_PLMN].paraLen != AT_PLMN_STR_MAX_LEN) &&
        (g_atParaList[AT_EFLOCIINFO_PLMN].paraLen != AT_PLMN_STR_MIN_LEN)) {
        AT_WARN_LOG("At_SetEflociInfoPara: PLMN LEN IS ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    plmnHex = 0;

    if (AT_String2Hex(g_atParaList[AT_EFLOCIINFO_PLMN].para,
        g_atParaList[AT_EFLOCIINFO_PLMN].paraLen, &plmnHex) != AT_SUCCESS) {
        AT_WARN_LOG("At_SetEflociInfoPara: String2Hex error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (plmnHex == 0xffffff) {
        efLociInfo.plmnId.mcc = 0xFFFFFFFF;
        efLociInfo.plmnId.mnc = 0xFFFFFFFF;
    } else {
        if (AT_DigitString2Hex(g_atParaList[AT_EFLOCIINFO_PLMN].para,
            AT_MCC_STR_LEN, &efLociInfo.plmnId.mcc) != VOS_TRUE) {
            AT_WARN_LOG("At_SetEflociInfoPara: Mcc IS ERROR!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if (AT_DigitString2Hex(&(g_atParaList[AT_EFLOCIINFO_PLMN].para[AT_EFLOCIINFO_PLMN_THIRD_PARA]),
                               g_atParaList[AT_EFLOCIINFO_PLMN].paraLen - AT_MCC_STR_LEN,
                               &(efLociInfo.plmnId.mnc)) != VOS_TRUE) {
            AT_WARN_LOG("At_SetEflociInfoPara: Mnc IS ERROR!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        efLociInfo.plmnId.mcc |= AT_PLMN_MCC_CLASS_MASK;
        /* mnc一个数字只占半个字节，也就是4bit */
        efLociInfo.plmnId.mnc |= (0xFFFFFFFF << ((g_atParaList[AT_EFLOCIINFO_PLMN].paraLen - AT_MCC_STR_LEN) *
        AT_HALF_BYTE_TO_BITS_LENS));
    }

    /* 转换LAC */
    if (g_atParaList[AT_EFLOCIINFO_LAC].paraLen == 0) {
        AT_WARN_LOG("At_SetEflociInfoPara: LAC LEN IS 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    efLociInfo.lac = (VOS_UINT16)g_atParaList[AT_EFLOCIINFO_LAC].paraValue;

    /* 转换location_update_Status */
    if (g_atParaList[AT_EFLOCIINFO_LOCATION_UPDATE_STATUS].paraLen == 0) {
        AT_WARN_LOG("At_SetEflociInfoPara: location_update_Status LEN IS 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    efLociInfo.locationUpdateStatus = (VOS_UINT8)g_atParaList[AT_EFLOCIINFO_LOCATION_UPDATE_STATUS].paraValue;

    /* 转换rfu */
    if (g_atParaList[AT_EFLOCIINFO_RFU].paraLen == 0) {
        AT_WARN_LOG("At_SetEflociInfoPara: rfu LEN IS 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    efLociInfo.rfu = (VOS_UINT8)g_atParaList[AT_EFLOCIINFO_RFU].paraValue;

    /* 执行操作命令 */
    if (TAF_MMA_SetEflociInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &efLociInfo) != VOS_TRUE) {
        AT_WARN_LOG("At_SetEflociInfoPara: TAF_MMA_SetEflociInfo return is not VOS_TRUE !");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EFLOCIINFO_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryEflociInfoPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_MMA_QryEflociInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EFLOCIINFO_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetPsEflociInfoPara(VOS_UINT8 indexNum)
{
    TAF_MMA_Efpslociinfo psefLociInfo;
    VOS_UINT32           plmnHex;

    /* 初始化结构体 */
    (VOS_VOID)memset_s(&psefLociInfo, sizeof(psefLociInfo), 0x00, sizeof(psefLociInfo));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_EFPSLOCIINFO_PARA_NUM) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: Para num is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 转换PTMSI */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: PTMSI len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    psefLociInfo.pTmsi = g_atParaList[0].paraValue;

    /* 转换PtmsiSignature */
    if (g_atParaList[1].paraLen == 0) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: PtmsiSignature len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    psefLociInfo.pTmsiSignature = g_atParaList[1].paraValue;

    /* 转换PLMN */
    if ((g_atParaList[AT_EFPSLOCIINFO_PLMN].paraLen != AT_PLMN_STR_MAX_LEN) &&
        (g_atParaList[AT_EFPSLOCIINFO_PLMN].paraLen != AT_PLMN_STR_MIN_LEN)) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: PLMN len is Error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    plmnHex = 0;

    if (AT_String2Hex(g_atParaList[AT_EFPSLOCIINFO_PLMN].para,
        g_atParaList[AT_EFPSLOCIINFO_PLMN].paraLen, &plmnHex) != AT_SUCCESS) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: String2Hex error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (plmnHex == 0xffffff) {
        psefLociInfo.plmnId.mcc = 0xFFFFFFFF;
        psefLociInfo.plmnId.mnc = 0xFFFFFFFF;
    } else {
        if (AT_DigitString2Hex(g_atParaList[AT_EFPSLOCIINFO_PLMN].para, AT_MCC_STR_LEN,
                               &psefLociInfo.plmnId.mcc) != VOS_TRUE) {
            AT_WARN_LOG("At_SetPsEflociInfoPara: Mcc num is Error!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if (AT_DigitString2Hex(&(g_atParaList[AT_EFPSLOCIINFO_PLMN].para[AT_EFPSLOCIINFO_PLMN_THIRD_PARA]),
                               g_atParaList[AT_EFPSLOCIINFO_PLMN].paraLen - AT_MCC_STR_LEN,
                               &(psefLociInfo.plmnId.mnc)) != VOS_TRUE) {
            AT_WARN_LOG("At_SetPsEflociInfoPara: Mnc num is Error!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        psefLociInfo.plmnId.mcc |= AT_PLMN_MCC_CLASS_MASK;
        psefLociInfo.plmnId.mnc |= (0xFFFFFFFF << ((g_atParaList[AT_EFPSLOCIINFO_PLMN].paraLen -
        AT_MCC_STR_LEN) * AT_HALF_BYTE_TO_BITS_LENS));
    }

    /* 转换LAC */
    if (g_atParaList[AT_EFPSLOCIINFO_LAC].paraLen == 0) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: LAC len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    psefLociInfo.lac = (VOS_UINT16)g_atParaList[AT_EFPSLOCIINFO_LAC].paraValue;

    /* 转换RAC */
    if (g_atParaList[AT_EFPSLOCIINFO_RAC].paraLen == 0) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: RAC len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    psefLociInfo.rac = (VOS_UINT8)g_atParaList[AT_EFPSLOCIINFO_RAC].paraValue;

    /* 转换location_update_Status */
    if (g_atParaList[AT_EFPSLOCIINFO_ROUTING_AREA_UPDATE_STATUS].paraLen == 0) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: location_update_Status len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    psefLociInfo.psLocationUpdateStatus = (VOS_UINT8)g_atParaList[AT_EFPSLOCIINFO_ROUTING_AREA_UPDATE_STATUS].paraValue;

    /* 执行操作命令 */
    if (TAF_MMA_SetPsEflociInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &psefLociInfo) != VOS_TRUE) {
        AT_WARN_LOG("At_SetPsEflociInfoPara: TAF_MMA_SetPsEflociInfo return is not VOS_TRUE !");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EFPSLOCIINFO_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryPsEflociInfoPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_MMA_QryPsEflociInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EFPSLOCIINFO_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetImsIpCapPara(VOS_UINT8 indexNum)
{
    AT_IMSA_ImsIpCapSetInfo imsIpCapSetInfo;
    VOS_UINT32              result;

    (VOS_VOID)memset_s(&imsIpCapSetInfo, sizeof(imsIpCapSetInfo), 0x00, sizeof(imsIpCapSetInfo));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetImsIpCapPara: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_IMSIPCAPCFG_PARA_NUM) {
        AT_WARN_LOG("AT_SetImsIpCapPara: para num is not equal 2!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen != 0) {
        imsIpCapSetInfo.opIpsecFlag = VOS_TRUE;
        imsIpCapSetInfo.ipsecFlag   = g_atParaList[0].paraValue;
    }

    if (g_atParaList[1].paraLen != 0) {
        imsIpCapSetInfo.opKeepAliveFlag = VOS_TRUE;
        imsIpCapSetInfo.keepAliveFlag   = g_atParaList[1].paraValue;
    }

    /* 给IMSA发送^IMSIPCAPCFG设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_IMS_IP_CAP_SET_REQ, (VOS_UINT8 *)&(imsIpCapSetInfo),
                                    (VOS_SIZE_T)sizeof(AT_IMSA_ImsIpCapSetInfo), PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetImsIpCapPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSIPCAPCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaImsIpCapSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsIpCapSetCnf *imsIpCapSetCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化消息变量 */
    indexNum       = 0;
    imsIpCapSetCnf = (IMSA_AT_ImsIpCapSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsIpCapSetCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSUECAP_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSIPCAPCFG_SET) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapSetCnf: WARNING:Not AT_CMD_IMSIPCAP_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (imsIpCapSetCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryImsIpCapPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 给IMSA发送^IMSIPCAPCFG查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_IMS_IP_CAP_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryImsIpCapPara: AT_FillAndSndAppReqMsg is error!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSIPCAPCFG_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvImsaImsIpCapQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsIpCapQryCnf *imsIpCapQryCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化消息变量 */
    indexNum = 0;

    imsIpCapQryCnf = (IMSA_AT_ImsIpCapQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsIpCapQryCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSIPCAP_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSIPCAPCFG_QRY) {
        AT_WARN_LOG("AT_RcvImsaImsIpCapQryCnf: WARNING:Not AT_CMD_IMSIPCAPCFG_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (imsIpCapQryCnf->result == VOS_OK) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            imsIpCapQryCnf->ipsecFlag, imsIpCapQryCnf->keepAliveFlag);

        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

LOCAL VOS_UINT32 AT_CheckCacdcPara(VOS_VOID)
{
    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_CheckCacdcPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不为TAF_AT_EOPLMN_PARA_NUM，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != TAF_MMA_ACDC_PARA_NUM) {
        AT_WARN_LOG("AT_CheckCacdcPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数检查，长度不等于TAF_MMA_OSID_ORIGINAL_LEN，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[0].paraLen != TAF_MMA_OSID_ORIGINAL_LEN) {
        AT_WARN_LOG1("AT_CheckCacdcPara: OsId Error.", g_atParaList[0].paraLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第2个参数检查，长度大于0小于等于TAF_MMA_MAX_APPID_LEN，返回AT_CME_INCORRECT_PARAMETERS */
    if ((g_atParaList[1].paraLen == 0) || (g_atParaList[1].paraLen > TAF_MMA_MAX_APPID_LEN)) {
        AT_WARN_LOG1("AT_CheckCacdcPara: AppId Error.", g_atParaList[1].paraLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetCacdcPara(VOS_UINT8 indexNum)
{
    TAF_MMA_AcdcAppInfo acdcAppInfo;
    VOS_UINT8           osId[TAF_MMA_OSID_ORIGINAL_LEN];
    VOS_UINT16          len;
    VOS_UINT32          result;
    errno_t             memResult;

    len = TAF_MMA_OSID_ORIGINAL_LEN;

    (VOS_VOID)memset_s(&acdcAppInfo, sizeof(acdcAppInfo), 0x00, sizeof(acdcAppInfo));
    (VOS_VOID)memset_s(osId, sizeof(osId), 0x00, sizeof(osId));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    result = AT_CheckCacdcPara();
    if (result != AT_SUCCESS) {
        return result;
    }

    memResult = memcpy_s(osId, TAF_MMA_OSID_ORIGINAL_LEN, g_atParaList[0].para, TAF_MMA_OSID_ORIGINAL_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, TAF_MMA_OSID_ORIGINAL_LEN, TAF_MMA_OSID_ORIGINAL_LEN);

    /* 填写消息结构 */
    /* 填写OSID */
    result = At_AsciiNum2HexString(osId, &len);
    if ((result != AT_SUCCESS) || (len != TAF_MMA_OSID_LEN)) {
        AT_ERR_LOG1("AT_SetCacdcPara: Ascii to Hex Error", len);
        return AT_ERROR;
    }

    memResult = memcpy_s(acdcAppInfo.osId, TAF_MMA_OSID_LEN, osId, TAF_MMA_OSID_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, TAF_MMA_OSID_LEN, TAF_MMA_OSID_LEN);

    /* 填写APPID */
    memResult = strncpy_s((VOS_CHAR *)acdcAppInfo.appId, TAF_MMA_MAX_APPID_LEN + 1, (VOS_CHAR *)g_atParaList[1].para,
                          g_atParaList[1].paraLen);
    TAF_STRCPY_CHK_RTN_VAL_CONTINUE(memResult, TAF_MMA_MAX_APPID_LEN + 1, g_atParaList[1].paraLen);

    /* 填写enAppIndication */
    acdcAppInfo.appIndication = g_atParaList[AT_CACDC_ENAPPINDICATION].paraValue;

    /* 通过ID_TAF_MMA_ACDC_APP_NOTIFY消息来设置ACDC */
    result = TAF_MMA_AcdcAppNotify(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, At_GetOpId(), &acdcAppInfo);

    if (result == VOS_TRUE) {
        return AT_OK;
    }

    return AT_ERROR;
}


LOCAL VOS_UINT32 AT_CheckErrcCapCfgPara(VOS_VOID)
{
    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为2-4个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if ((g_atParaIndex < AT_ERRCCAPCFG_MIN_PARA_NUM) || (g_atParaIndex > AT_ERRCCAPCFG_MAX_PARA_NUM)) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数长度不为0，否则返回AT_CME_INCORR ECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: Length = 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetErrcCapCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_ErrccapCfgReq setErrcCapCfgReq;
    VOS_UINT32           result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setErrcCapCfgReq, sizeof(setErrcCapCfgReq), 0x00, sizeof(AT_MTA_ErrccapCfgReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    result = AT_CheckErrcCapCfgPara();
    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充结构体 */
    setErrcCapCfgReq.errcCapType = g_atParaList[0].paraValue;
    setErrcCapCfgReq.para1       = g_atParaList[1].paraValue;

    if (g_atParaIndex == AT_ERRCCAPCFG_CONTROL_CAPA_3_PARA_NUM) {
        setErrcCapCfgReq.para2 = g_atParaList[AT_ERRCCAPCFG_PARA2].paraValue;
    }

    if (g_atParaIndex == AT_ERRCCAPCFG_MAX_PARA_NUM) {
        setErrcCapCfgReq.para2 = g_atParaList[AT_ERRCCAPCFG_PARA2].paraValue;
        setErrcCapCfgReq.para3 = g_atParaList[AT_ERRCCAPCFG_PARA3].paraValue;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_ERRCCAP_CFG_SET_REQ, (VOS_UINT8 *)&setErrcCapCfgReq,
                                    sizeof(AT_MTA_ErrccapCfgReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ERRCCAPCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_SetErrcCapQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_ErrccapQryReq setErrcCapQryReq;
    VOS_UINT32           result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setErrcCapQryReq, sizeof(setErrcCapQryReq), 0x00, sizeof(setErrcCapQryReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetErrcCapQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetErrcCapQryPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数长度不能为0，否则返回AT_CME_INCORR ECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetErrcCapQryPara: Length is 0.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    setErrcCapQryReq.errcCapType = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_ERRCCAP_QRY_SET_REQ, (VOS_UINT8 *)&setErrcCapQryReq,
                                    sizeof(AT_MTA_ErrccapQryReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ERRCCAPQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvImsaImsUrspSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    IMSA_AT_ImsUrspSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum = 0;
    VOS_UINT8              broadCastIdx;

    /* 初始化消息变量 */
    setCnf = (IMSA_AT_ImsUrspSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(setCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvImsaImsUrspSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvImsaImsUrspSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSURSP_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSURSP_SET) {
        AT_WARN_LOG("AT_RcvImsaImsUrspSetCnf: WARNING:Not AT_CMD_IMSURSP_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (setCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    if (setCnf->result != VOS_OK) {
        if (At_ClientIdToUserBroadCastId(setCnf->appCtrl.clientId, &broadCastIdx) != AT_SUCCESS) {
            AT_WARN_LOG("AT_RcvImsaImsUrspSetCnf: At_ClientIdToUserBroadCastId is err!");
            return VOS_ERR;
        }

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^IMSURSPVERSION: %d%s", g_atCrLf, setCnf->version, g_atCrLf);

        At_SendResultData(broadCastIdx, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}
#endif

