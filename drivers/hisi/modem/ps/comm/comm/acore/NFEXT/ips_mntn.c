/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019.All rights reserved.
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

#include "ips_mntn.h"
#include "ips_mntn_mini.h"
#include "ips_traffic_statistic.h"
#include "securec.h"
#include "ttf_ip_comm.h"
#include "ttf_util.h"
#include "nv_stru_gucnas.h"
#include "netfilter_ex.h"
#include "netfilter_ex_ctrl.h"

#define THIS_FILE_ID PS_FILE_ID_ACPU_IPS_MNTN_C

#define IPS_IFNAMSIZ 16 /* �����������Ƴ��� */
#define IPS_END_NAME 16
#define IPS_MNTN_TRACE_MAX_BYTE_LEN 1514 /* ����TCP/IPЭ��ջ��������� */
#define MAC_HEADER_LENGTH 14
#define NFEXT_STATS_ALLOC_MEM_FAIL 4

/* TCP/IPЭ��ջ���Ĳ�����Ϣ */
typedef struct {
    VOS_UINT16 primId;
    VOS_UINT16 toolId;
    VOS_UINT8  netIfName[IPS_IFNAMSIZ]; /* ������������ */
    VOS_UINT16 len;
    VOS_UINT8  data[2];
} IpsMntnPktInfo;

/* ������ת���Ĳ�����Ϣ */
typedef struct {
    VOS_UINT16 primId;
    VOS_UINT16 toolId;
    VOS_UINT8  srcPort[IPS_END_NAME];  /* ��������������Ϣ */
    VOS_UINT8  destPort[IPS_END_NAME]; /* ���������������� */
    VOS_UINT16 len;
    VOS_UINT8  data[2];
} IpsMntnBridgePktInfo;

/* APP�����������Ϣ */
typedef struct {
    VOS_UINT16 len;
    VOS_UINT8  data[2];
} IpsMntnAppCmdInfo;

/* ������Ϣ */
typedef struct {
    VOS_UINT16 primId;
    VOS_UINT16 toolId;
    VOS_UINT32 fcType;
} IpsMntnFlowCtrlInfo;

IPS_MntnTraceCfgReq g_ipsTraceMsgCfg; /* TCP/IPЭ��ջ��ά�ɲ�������Ϣ */

/* ��OM����͸����Ϣ, �Դ������񹴰���ɺ��ͷ�͸��ָ��; ���ڿ��ܵ������򣬲���֧�ַ��ж�������ֱ�ӹ��� */
STATIC VOS_VOID IPSMNTN_TransMsg(VOS_UINT8* traceData, VOS_UINT32 contentLen, TTF_MntnMsgTypeUint16 msgName)
{
    NfExtDataRingBuf       bufData     = {0};
    mdrv_diag_trans_ind_s* diagRptData = (mdrv_diag_trans_ind_s*)traceData;

    /* �����Ϣ */
    diagRptData->ulPid    = ACPU_PID_NFEXT;
    diagRptData->ulMsgId  = msgName;
    diagRptData->ulModule = MDRV_DIAG_GEN_MODULE(MODEM_ID_0, DIAG_MODE_COMM);
    diagRptData->ulLength = contentLen;
    diagRptData->pData    = traceData + sizeof(mdrv_diag_trans_ind_s);

    bufData.data = traceData;

    if (NFEXT_AddDataToRingBuf(&bufData) != VOS_OK) {
        NF_EXT_MEM_FREE(ACPU_PID_NFEXT, traceData);
    }
}

/* ������Ϣ������Ϣ����ȡ�����ĳ��� */
STATIC VOS_UINT32 IPSMNTN_GetPktLenByTraceCfg(const IPS_MntnTraceCfg* traceCfg, VOS_UINT16 pktLen, VOS_UINT8* pktData,
    VOS_UINT32* dataLen)
{
    IPS_MntnTraceChosenUint32 choice       = traceCfg->choice;
    VOS_UINT32                traceDataLen = 0;
    *dataLen                               = 0;

    choice = PS_MIN(choice, IPS_MNTN_TRACE_MSG_HEADER_CHOSEN);

    /* �ж϶�ʱ���Ƿ����� */
    switch (choice) {
        case IPS_MNTN_TRACE_NULL_CHOSEN:
            return VOS_ERR;
        case IPS_MNTN_TRACE_MSG_HEADER_CHOSEN:
            traceDataLen = TTF_GetIpDataTraceLen(ACPU_PID_NFEXT, pktData, pktLen);
            *dataLen     = TTF_MIN(traceDataLen, pktLen);
            break;
        case IPS_MNTN_TRACE_CONFIGURABLE_LEN_CHOSEN:
            *dataLen = TTF_MIN(traceCfg->traceDataLen, pktLen);
            break;
        case IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN:
            *dataLen = pktLen;
            break;
        default:
            return VOS_ERR;
    }

    return VOS_OK;
}

STATIC IPS_MntnTraceCfg* IPSMNTN_MatchPktTraceCfg(TTF_MntnMsgTypeUint16 msgType)
{
    /* ������Ϣ����ѡ���Ӧ������Ϣ */
    if (msgType == ID_IPS_TRACE_INPUT_DATA_INFO) {
        return &(g_ipsTraceMsgCfg.preRoutingTraceCfg);
    } else if (msgType == ID_IPS_TRACE_OUTPUT_DATA_INFO) {
        return &(g_ipsTraceMsgCfg.postRoutingTraceCfg);
    } else if ((msgType >= ID_IPS_TRACE_BRIDGE_DATA_INFO) && (msgType <= ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_STOP)) {
        return &(g_ipsTraceMsgCfg.localTraceCfg);
    } else {
        return VOS_NULL_PTR;
    }
}

#define IPS_MNTN_TRACE_GRO_MAX_BYTE_LEN 64000 /* ����TCP/IPЭ��ջ��������� */

/* ����TCP/IPЭ��ջ���պͷ��ͱ��Ļص����� */
VOS_VOID IPSMNTN_PktInfoCB(const VOS_CHAR* netIfName, VOS_UINT32 nameLen, const struct sk_buff* skb,
    TTF_MntnMsgTypeUint16 msgType)
{
    VOS_UINT32              ipDataLen = 0;
    VOS_UINT32              captureLen;
    VOS_UINT8*              traceData = VOS_NULL_PTR;
    IpsMntnPktInfo*         traceMsg  = VOS_NULL_PTR;
    const IPS_MntnTraceCfg* traceCfg  = VOS_NULL_PTR;
    VOS_UINT8*              ipHeader  = VOS_NULL_PTR;

    /* HIDSδ���� */
    if (mdrv_diag_get_conn_state() == VOS_NO) {
        return;
    }

    traceCfg = IPSMNTN_MatchPktTraceCfg(msgType);
    if (traceCfg == VOS_NULL_PTR) {
        TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "msgType is not Expectes!\n");
        return;
    }

    ipHeader = skb_network_header(skb);
    /* ������Ϣ������Ϣ����ȡ������ĳ��� */
    if (IPSMNTN_GetPktLenByTraceCfg(traceCfg, skb->len, ipHeader, &ipDataLen) == VOS_ERR) {
        /* ������ñ��� */
        return;
    }

    ipDataLen = PS_MIN(IPS_MNTN_TRACE_GRO_MAX_BYTE_LEN, ipDataLen);
    TTF_GET_OFFSET(captureLen, IpsMntnPktInfo, data);
    /* IP���ݳ��ȼ�MACͷ���ȶ�ӦWireShark���������е�Capture Length�������񳤶� */
    captureLen += ipDataLen;
    captureLen += MAC_HEADER_LENGTH;

    if (IPSMNTN_PktIsOverFlow(captureLen) == VOS_TRUE) {
        return;
    }

    /* ��Linux�ں������ڴ� */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(mdrv_diag_trans_ind_s));
    if (traceData == VOS_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }
    /* ����͸����Ϣ */
    traceMsg = (IpsMntnPktInfo*)(traceData + sizeof(mdrv_diag_trans_ind_s));
    /* traceMsg->len��ӦWireShark���������е�Frame Length����֡���� */
    traceMsg->len    = skb->len + MAC_HEADER_LENGTH;
    traceMsg->primId = msgType;
    TTF_ACORE_SF_CHK(memcpy_s(traceMsg->netIfName, IPS_IFNAMSIZ, netIfName, nameLen));

    /* ��ĳЩ�����(������ traceCfg->ulTraceDataLen Ϊ0)��ipDataLenΪ0������������쳣��������б��� */
    /*lint -e{416,426,669} */
    if (ipDataLen > 0) {
        if (skb_copy_bits(skb, 0, traceMsg->data + MAC_HEADER_LENGTH, ipDataLen) < 0) {
            TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "Call skb_copy_bits fail!\n");
            NF_EXT_MEM_FREE(ACPU_PID_NFEXT, traceData);
            return;
        }
    }

    /* user�汾������IP��ַ */
    TTF_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data + MAC_HEADER_LENGTH, (VOS_UINT16)ipDataLen);

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

STATIC IPS_MntnTraceCfg* IPSMNTN_MatchBridgePktTraceCfg(TTF_MntnMsgTypeUint16 msgType)
{
    if ((msgType >= ID_IPS_TRACE_BRIDGE_DATA_INFO) && (msgType <= ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_STOP)) {
        return &(g_ipsTraceMsgCfg.bridgeArpTraceCfg);
    }
    return VOS_NULL_PTR;
}

/* ����������ת���Ļص����� */
VOS_VOID IPSMNTN_BridgePktInfoCB(const VOS_CHAR* srcPort, const VOS_CHAR* destPort, VOS_UINT8* pktData, VOS_UINT16 pktLen,
    TTF_MntnMsgTypeUint16 msgType)
{
    VOS_UINT32              ipDataLen = 0;
    VOS_UINT32              captureLen;
    IpsMntnBridgePktInfo*   traceMsg  = VOS_NULL_PTR;
    VOS_UINT8*              traceData = VOS_NULL_PTR;
    const IPS_MntnTraceCfg* traceCfg  = VOS_NULL_PTR;

    if (pktData == VOS_NULL_PTR) {
        TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "pktData is NULL!\n");
        return;
    }

    if (mdrv_diag_get_conn_state() == VOS_NO) {
        return;
    }
    /* ������Ϣ����ѡ���Ӧ������Ϣ */
    traceCfg = IPSMNTN_MatchBridgePktTraceCfg(msgType);
    if (traceCfg == VOS_NULL_PTR) {
        TTF_LOG1(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "msgType is not Expectes!\n", msgType);
        return;
    }
    /* ���IP���ĳ��� */
    if (IPSMNTN_GetPktLenByTraceCfg(traceCfg, pktLen, pktData, &ipDataLen) == VOS_ERR) {
        /* ������ñ��� */
        return;
    }

    ipDataLen = PS_MIN(IPS_MNTN_TRACE_MAX_BYTE_LEN, ipDataLen);
    TTF_GET_OFFSET(captureLen, IpsMntnBridgePktInfo, data);
    /* IP���ݳ��ȼ�MACͷ���ȶ�ӦWireShark���������е�Capture Length�������񳤶� */
    captureLen += ipDataLen;
    captureLen += MAC_HEADER_LENGTH;

    /* ��Linux�ں������ڴ� */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(mdrv_diag_trans_ind_s));
    if (traceData == VOS_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }

    /* ����͸����Ϣ */
    traceMsg = (IpsMntnBridgePktInfo*)(traceData + sizeof(mdrv_diag_trans_ind_s));
    /* traceMsg->len��ӦWireShark���������е�Frame Length����֡���� */
    traceMsg->len    = pktLen + MAC_HEADER_LENGTH;
    traceMsg->primId = msgType;

    if (srcPort != VOS_NULL_PTR) {
        TTF_ACORE_SF_CHK(memcpy_s(traceMsg->srcPort, IPS_END_NAME, srcPort, IPS_END_NAME));
    }
    if (destPort != VOS_NULL_PTR) {
        TTF_ACORE_SF_CHK(memcpy_s(traceMsg->destPort, IPS_END_NAME, destPort, IPS_END_NAME));
    }
    if (ipDataLen > 0) {
        TTF_ACORE_SF_CHK(memcpy_s(traceMsg->data + MAC_HEADER_LENGTH, ipDataLen, pktData, ipDataLen));
    }

    /* user�汾������IP��ַ */
    TTF_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data + MAC_HEADER_LENGTH, (VOS_UINT16)ipDataLen);

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

/* ����TCP/IPЭ��ջ���Ʊ��Ļص����� */
VOS_VOID IPSMNTN_CtrlPktInfoCB(const VOS_CHAR* netIfName, VOS_UINT32 nameLen, const VOS_UINT8* pktData, VOS_UINT16 pktLen,
    TTF_MntnMsgTypeUint16 msgType)
{
    VOS_UINT8*      traceData = VOS_NULL_PTR;
    IpsMntnPktInfo* traceMsg  = VOS_NULL_PTR;
    VOS_UINT32      ipDataLen;
    VOS_UINT32      captureLen = 0;

    if (pktData == VOS_NULL_PTR) {
        TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "pucPktData is NULL!\n");
        return;
    }

    /* HIDSδ���� */
    if (mdrv_diag_get_conn_state() == VOS_NO) {
        return;
    }
    ipDataLen = PS_MIN(IPS_MNTN_TRACE_MAX_BYTE_LEN, pktLen);
    TTF_GET_OFFSET(captureLen, IpsMntnPktInfo, data);
    /* ��ӦWireShark���������е�Capture Length�������񳤶� */
    captureLen += ipDataLen;
    /* ��Linux�ں������ڴ� */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(mdrv_diag_trans_ind_s));
    if (traceData == VOS_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }

    /* ����͸����Ϣ */
    traceMsg = (IpsMntnPktInfo*)(traceData + sizeof(mdrv_diag_trans_ind_s));
    /* traceMsg->len��ӦWireShark���������е�Frame Length����֡���� */
    traceMsg->len    = pktLen;
    traceMsg->primId = msgType;
    TTF_ACORE_SF_CHK(memcpy_s(traceMsg->netIfName, IPS_IFNAMSIZ, netIfName, nameLen));
    if (ipDataLen > 0) {
        TTF_ACORE_SF_CHK(memcpy_s(traceMsg->data, ipDataLen, pktData, ipDataLen));
    }

    /* user�汾������IP��ַ */
    TTF_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data, (VOS_UINT16)ipDataLen);

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

#define MNTN_FLOW_CTRL_INFO_LEN ((VOS_UINT32)sizeof(IpsMntnFlowCtrlInfo))
/* Э��ջ���ؿ�ά�ɲ��ϱ� */
VOS_VOID IPSMNTN_FlowCtrl(VOS_UINT32 fcType, TTF_MntnMsgTypeUint16 msgType)
{
    mdrv_diag_trans_ind_s transData   = {0};
    IpsMntnFlowCtrlInfo   flowCtrlMsg = {0};

    flowCtrlMsg.fcType = fcType;
    flowCtrlMsg.primId = msgType;

    /* �����Ϣ */
    transData.ulPid    = ACPU_PID_NFEXT;
    transData.ulMsgId  = msgType;
    transData.ulModule = MDRV_DIAG_GEN_MODULE(MODEM_ID_0, DIAG_MODE_COMM);
    transData.ulLength = MNTN_FLOW_CTRL_INFO_LEN;
    transData.pData    = &flowCtrlMsg;

    if (mdrv_diag_trans_report(&transData) != VOS_OK) {
        TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "Call mdrv_diag_trans_report fail!\n");
    }
}

/* ��ȡ����NV���ã���������������־ */
STATIC VOS_BOOL IPSMNTN_GetPrivacyFilterNvCfg(VOS_VOID)
{
    VOS_UINT8 logFilterFlag = VOS_TRUE;

    return logFilterFlag;
}


/* �����������ã������������� */
STATIC VOS_VOID IPSMNTN_TraceSensitiveAdjustSwitch(VOS_VOID)
{
    VOS_UINT8 adjustFlag = PS_FALSE;

    /* ������״̬�����κθ��� */
    if (IPSMNTN_GetPrivacyFilterNvCfg() != VOS_TRUE) {
        return;
    }

    /* ����У������״̬ */
    if (g_ipsTraceMsgCfg.bridgeArpTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.bridgeArpTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = PS_TRUE;
    }

    if (g_ipsTraceMsgCfg.preRoutingTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.preRoutingTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = PS_TRUE;
    }

    if (g_ipsTraceMsgCfg.postRoutingTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.postRoutingTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = PS_TRUE;
    }

    if (g_ipsTraceMsgCfg.localTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.localTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = PS_TRUE;
    }

    if (adjustFlag == PS_TRUE) {
        TTF_LOG(ACPU_PID_NFEXT, DIAG_MODE_COMM, PS_PRINT_ERROR, "In Sensitive status, Some config has been modify!\n");
    }
}

VOS_VOID IPSMNTN_SetIpsTraceMsgCfg(const IPS_MntnTraceCfgReq* cfgReq)
{
    /* �������ò��� */
    g_ipsTraceMsgCfg = *cfgReq;

    IPSMNTN_TraceSensitiveAdjustSwitch();
}

