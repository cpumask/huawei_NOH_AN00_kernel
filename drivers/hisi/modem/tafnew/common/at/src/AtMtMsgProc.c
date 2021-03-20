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
#include "AtMtMsgProc.h"
#include "securec.h"
#include "AtTypeDef.h"
#include "mn_client.h"
#include "ATCmdProc.h"
#include "AtMtCommFun.h"
#include "mn_comm_api.h"
#include "at_phy_interface.h"
#include "taf_phy_pid_def.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_MT_MSG_PROC_C
#define AT_FSERDESRT_TEST_NUM_INDEX 0
#define AT_FSERDESRT_SYNC_LEN_INDEX 1
#define AT_FSERDESRT_BURST_TIME_INDEX 2
#define AT_FSERDESRT_STALL_TIME_INDEX 3
#define AT_FSERDESRT_DL_SYNC_LEN_INDEX 4
#define AT_FSERDESRT_DL_BURST_TIME_INDEX 5
#define AT_FSERDESRT_DL_STALL_TIME_INDEX 6
#define AT_FSERDESRT_LS_TEST_INDEX 7
#define AT_FSERDESRT_FWD_CLK_AMP_INDEX 8
#define AT_CAL_MIPIDEV_BYTE_CNT_VALUE_2 2
#define AT_CAL_MIPIDEV_BYTE_CNT_VALUE_3 3
#define AT_PHY_SERDES_TEST_PARALIST_PARA1 2
#define AT_PHY_SERDES_TEST_PARALIST_PARA2 3
#define AT_PHY_SERDES_TEST_PARALIST_PARA3 4
#define AT_PHY_SERDES_TEST_PARALIST_PARA4 5
#define AT_PHY_SERDES_TEST_PARALIST_PARA5 6
#define AT_PHY_SERDES_TEST_PARALIST_PARA6 7
#define AT_PHY_SERDES_TEST_PARALIST_PARA7 8
#define AT_PHY_SERDES_TEST_PARALIST_PARA8 9
#define AT_PHY_SERDES_TEST_PARALIST_PARA9 10
#define AT_PHY_SERDES_TEST_PARALIST_PARA10 11
#define AT_PHY_SERDES_TEST_PARALIST_PARA11 12
#define AT_PHY_SERDES_TEST_PARALIST_PARA12 13
#define AT_PHY_SERDES_TEST_PARALIST_PARA13 14
#define AT_PHY_SERDES_TEST_PARALIST_PARA14 15
#define AT_PHY_SERDES_TEST_PARALIST_PARA15 16
#define AT_PHY_SERDES_TEST_PARALIST_PARA16 17
#define AT_PHY_SERDES_TEST_PARALIST_PARA17 18
#define AT_PHY_SERDES_TEST_PARALIST_PARA18 19
#define AT_PHY_SERDES_TEST_PARALIST_PARA19 20
#define AT_PHY_SERDES_TEST_PARALIST_PARA20 21

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
/* 填写消息头 */
#if (VOS_OS_VER == VOS_WIN32)
#define AT_CFG_MT_MSG_HDR(msg, ulRecvPid, usSndMsgId) do { \
    ((AT_MT_MsgHeader *)(msg))->ulSenderCpuId   = VOS_LOCAL_CPUID; \
    ((AT_MT_MsgHeader *)(msg))->ulSenderPid     = WUEPS_PID_AT;    \
    ((AT_MT_MsgHeader *)(msg))->ulReceiverCpuId = VOS_LOCAL_CPUID; \
    ((AT_MT_MsgHeader *)(msg))->ulReceiverPid   = (ulRecvPid);     \
    ((AT_MT_MsgHeader *)(msg))->usMsgId         = (usSndMsgId);    \
} while (0)

#else
#define AT_CFG_MT_MSG_HDR(pstMsg, ulRecvPid, usSndMsgId) do { \
    VOS_SET_SENDER_ID(pstMsg, WUEPS_PID_AT);              \
    VOS_SET_RECEIVER_ID(pstMsg, (ulRecvPid));             \
    ((AT_MT_MsgHeader *)(pstMsg))->usMsgId = (usSndMsgId); \
} while (0)
#endif
/* 获取消息内容开始地址 */
#define AT_MT_GET_MSG_ENTITY(msg) ((VOS_VOID *)&(((AT_MT_MsgHeader *)(msg))->usMsgId))

/* 获取消息长度 */
#define AT_MT_GET_MSG_LENGTH(msg) VOS_GET_MSG_LEN(msg)

/* 封装消息初始化消息内容接口 */
VOS_VOID AT_MT_ClearMsgEntity(AT_MT_MsgHeader *msg);
#define AT_MT_CLR_MSG_ENTITY(msg) AT_MT_ClearMsgEntity((AT_MT_MsgHeader *)msg)
#if (FEATURE_LTEV == FEATURE_ON)
#define AT_COMPONENTTYPE_RAT_LTEV 0x7
#endif
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
static const AT_PROC_BbicMsg g_atProcBbicMsgTab[] = {
    { ID_BBIC_CAL_AT_MT_TX_CNF, AT_ProcTxCnf },
    { ID_BBIC_CAL_AT_MT_RX_CNF, AT_ProcRxCnf },
    { ID_BBIC_AT_CAL_MSG_CNF, AT_ProcGsmTxCnf_ModulateWave },
    { ID_BBIC_AT_GSM_TX_PD_CNF, AT_ProcPowerDetCnf_ModulateWave },
    { ID_BBIC_CAL_AT_MT_TX_PD_IND, AT_ProcPowerDetCnf },
    { ID_BBIC_AT_MIPI_READ_CNF, AT_RcvBbicCalMipiRedCnf },
    { ID_BBIC_AT_MIPI_WRITE_CNF, AT_RcvBbicCalMipiWriteCnf },
    { ID_BBIC_AT_PLL_QRY_CNF, At_RcvBbicPllStatusCnf },
    { ID_BBIC_CAL_AT_MT_RX_RSSI_IND, At_RcvBbicRssiInd },
    { ID_BBIC_AT_DPDT_CNF, AT_RcvBbicCalSetDpdtCnf },
    { ID_BBIC_AT_TEMP_QRY_CNF, AT_RcvBbicCalQryFtemprptCnf },
    { ID_BBIC_AT_DCXO_CNF, At_RcvBbicCalDcxoCnf },
    { ID_BBIC_AT_TRX_TAS_CNF, AT_RcvBbicCalSetTrxTasCnf }
};

static const AT_PROC_CbtMsg g_atProcCbtMsgTab[] = {
    { ID_CCBT_AT_SET_WORK_MODE_CNF, AT_ProcSetWorkModeCnf },
};

static const AT_PROC_UecbtMsg g_atProcUeCbtMsgTab[] = {
    { ID_UECBT_AT_RFIC_MEM_TEST_CNF, At_ProcUeCbtRfIcMemTestCnf },
    { ID_UECBT_AT_RFIC_DIE_IE_QUERY_CNF, At_RcvUeCbtRfIcIdExQryCnf },
};

static const AT_PROC_DspIdleMsg g_atProcDspIdleMsgTab[] = {
    { ID_PHY_AT_SERDES_AGING_TEST_CNF, At_ProcDspIdleSerdesRtCnf },
    { ID_PHY_AT_SERDES_TEST_ASYNC_CNF, At_ProcDspIdleSerdesAsyncTestCnf },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_PHY_AT_SLT_TEST_CNF, AT_ProcDspIdleSltTestCnf },
#endif
};

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)


VOS_UINT32 At_SendContinuesWaveOnToHPA(VOS_UINT16 power, VOS_UINT8 indexNum)
{
    AT_HPA_RfCfgReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;
    VOS_UINT8          ctrlType;

    ctrlType     = WDSP_CTRL_TX_ONE_TONE;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 打开调制 */
    if (atDevCmdCtrl->cltEnableFlg == AT_DSP_CLT_ENABLE) {
        ctrlType = WDSP_CTRL_TX_THREE_TONE;
    }

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_HPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendContinuesWaveOnToHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);

    /* 填写消息体 */
    msg->msgId = ID_AT_HPA_RF_CFG_REQ;

    /* Tx Cfg */
    mask                   = W_RF_MASK_TX_TXONOFF | W_RF_MASK_TX_POWER;
    msg->rfCfgPara.txPower = (VOS_INT16)power;

    /* 按位标识配置类型 */
    msg->rfCfgPara.mask      = mask;
    msg->rfCfgPara.txAfcInit = W_AFC_INIT_VALUE;

    /* 打开单音信号 */
    msg->rfCfgPara.txEnable = ctrlType;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendContinuesWaveOnToHPA: Send msg fail!");
        return AT_FAILURE;
    }
    return AT_SUCCESS;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 At_SendContinuesWaveOnToCHPA(VOS_UINT8 ctrlType, VOS_UINT16 power)
{
    AT_CHPA_RfCfgReq *msg = VOS_NULL_PTR;
    VOS_UINT32        length;
    VOS_UINT16        mask;

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_CHPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_CHPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendContinuesWaveOnToCHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, UPHY_PID_CSDR_1X_CM, length);

    /* 填写消息体 */
    msg->msgId = ID_AT_CHPA_RF_CFG_REQ;

    /* Tx Cfg */
    mask                   = W_RF_MASK_TX_TXONOFF | W_RF_MASK_TX_POWER;
    msg->rfCfgPara.txPower = (VOS_INT16)power;

    /* 按位标识配置类型 */
    msg->rfCfgPara.mask      = mask;
    msg->rfCfgPara.txAfcInit = W_AFC_INIT_VALUE;

    /* 打开单音信号 */
    msg->rfCfgPara.txEnable = ctrlType;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendContinuesWaveOnToCHPA: Send msg fail!");
        return AT_FAILURE;
    }
    return AT_SUCCESS;
}


VOS_UINT32 At_SendTxOnOffToCHPA(VOS_UINT8 switchStatus)
{
    AT_CHPA_RfCfgReq  *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_CHPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_CHPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendTxOnOffToCHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, UPHY_PID_CSDR_1X_CM, length);

    /* 填写消息体 */
    msg->msgId = ID_AT_CHPA_RF_CFG_REQ;

    /* Tx Cfg */
    mask = W_RF_MASK_AFC | W_RF_MASK_TX_ARFCN | W_RF_MASK_TX_TXONOFF | W_RF_MASK_TX_PAMODE | W_RF_MASK_TX_POWDET;

    mask                   = mask | W_RF_MASK_TX_POWER;
    msg->rfCfgPara.txPower = (VOS_INT16)atDevCmdCtrl->power;

    /* Tx Cfg */
    msg->rfCfgPara.mask      = mask;             /* 按位标识配置类型 */
    msg->rfCfgPara.txAfcInit = W_AFC_INIT_VALUE; /* AFC */
    /* atDevCmdCtrl->stDspBandArfcn.usDspBand;     1,2,3...,协议中的band编号,注意不是BandId */
    msg->rfCfgPara.txBand    = 0;
    msg->rfCfgPara.txFreqNum = atDevCmdCtrl->dspBandArfcn.ulArfcn; /* Arfcn */
    msg->rfCfgPara.txPaMode  = atDevCmdCtrl->paLevel;

    if (switchStatus == AT_DSP_RF_SWITCH_ON) {
        msg->rfCfgPara.txEnable = WDSP_CTRL_TX_ON; /* 打开发射TX */
    } else {
        msg->rfCfgPara.txEnable = WDSP_CTRL_TX_OFF; /* 关闭发射TX */
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendTxOnOffToCHPA: Send msg fail!");
        return AT_FAILURE;
    }
    return AT_SUCCESS;
}


VOS_UINT32 At_SendRxOnOffToCHPA(VOS_UINT32 rxSwitch)
{
    AT_CHPA_RfCfgReq  *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_CHPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_CHPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendRxOnOffToCHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, UPHY_PID_CSDR_1X_CM, length);

    /* 填写消息体 */
    msg->msgId = ID_AT_CHPA_RF_CFG_REQ;

    mask = W_RF_MASK_RX_ARFCN | W_RF_MASK_RX_ANTSEL | W_RF_MASK_RX_RXONOFF;

    /* 按位标识配置类型 */
    msg->rfCfgPara.mask = mask;

    /* 1,2,3...,协议中的band编号 */
    msg->rfCfgPara.rxBand    = 0; /* atDevCmdCtrl->stDspBandArfcn.usDspBand; */
    msg->rfCfgPara.rxFreqNum = atDevCmdCtrl->dspBandArfcn.ulArfcn;

    msg->rfCfgPara.rxAntSel = ANT_ONE;

    if (rxSwitch == AT_DSP_RF_SWITCH_ON) {
        /* CPU控制打开射频芯片,天线1 */
        msg->rfCfgPara.rxEnable = DSP_CTRL_RX_ANT1_ON;
    } else {
        /* 关闭接收RX */
        msg->rfCfgPara.rxEnable = DSP_CTRL_RX_OFF;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendRxOnOffToCHPA: Send msg fail!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

#endif


TAF_UINT32 At_SendTxOnOffToHPA(TAF_UINT8 ucSwitch, TAF_UINT8 indexNum)
{
    AT_HPA_RfCfgReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_HPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    mask   = 0;

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendTxOnOffToHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);

    /* 填写消息体 */
    msg->msgId = ID_AT_HPA_RF_CFG_REQ;

    /* Tx Cfg */
    mask = W_RF_MASK_AFC | W_RF_MASK_TX_TXONOFF | W_RF_MASK_TX_PAMODE | W_RF_MASK_TX_POWDET;

    /* AT^FDAC设置的FDAC值和AT^FWAVE设置的power值表示的含义相同，取后设置的值 */
    if (atDevCmdCtrl->fdacFlag == VOS_TRUE) {
        mask                 = mask | W_RF_MASK_TX_AGC;
        msg->rfCfgPara.txAgc = atDevCmdCtrl->fdac;
    } else {
        mask                   = mask | W_RF_MASK_TX_POWER;
        msg->rfCfgPara.txPower = (VOS_INT16)atDevCmdCtrl->power;
    }

    /* Tx Cfg */
    msg->rfCfgPara.mask      = mask;             /* 按位标识配置类型 */
    msg->rfCfgPara.txAfcInit = W_AFC_INIT_VALUE; /* AFC */
    /* 1,2,3...,协议中的band编号,注意不是BandId */
    msg->rfCfgPara.txBand    = atDevCmdCtrl->dspBandArfcn.dspBand;
    msg->rfCfgPara.txFreqNum = atDevCmdCtrl->dspBandArfcn.ulArfcn; /* Arfcn */
    msg->rfCfgPara.txPaMode  = atDevCmdCtrl->paLevel;

    if (ucSwitch == AT_DSP_RF_SWITCH_OFF) {
        msg->rfCfgPara.txEnable = WDSP_CTRL_TX_OFF; /* 关闭发射TX */
    } else {
        msg->rfCfgPara.txEnable = WDSP_CTRL_TX_ON; /* 打开发射TX */
        msg->rfCfgPara.mask |= W_RF_MASK_TX_ARFCN;

        /* 只有在^FTXON=1 时才下发 */
        if (atDevCmdCtrl->cltEnableFlg == AT_DSP_CLT_ENABLE) {
            msg->rfCfgPara.mask |= W_RF_MASK_TX_CLT;
            msg->rfCfgPara.mask &= ~(W_RF_MASK_TX_POWDET);
        }
        if (atDevCmdCtrl->dcxoTempCompEnableFlg == AT_DCXOTEMPCOMP_ENABLE) {
            msg->rfCfgPara.mask |= W_RF_MASK_TX_DCXOTEMPCOMP;
            msg->rfCfgPara.mask &= ~(W_RF_MASK_AFC);
        }
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendTxOnOffToHPA: Send msg fail!");
        return AT_FAILURE;
    }
    return AT_SUCCESS;
}


VOS_UINT32 At_SendTxOnOffToGHPA(VOS_UINT8 indexNum, VOS_UINT8 ucSwitch)
{
    AT_GHPA_RfTxCfgReq *txMsg        = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl  *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32          length;
    VOS_UINT16          mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_GHPA_RfTxCfgReq消息 */
    length = sizeof(AT_GHPA_RfTxCfgReq) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e830 */
    txMsg = (AT_GHPA_RfTxCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    /*lint -restore */
    if (txMsg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendRfCfgReqToGHPA: alloc Tx msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)txMsg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);

    /* Tx Cfg */

    /* 填写消息体 */
    txMsg->msgId = ID_AT_GHPA_RF_TX_CFG_REQ;
    mask         = G_RF_MASK_TX_AFC;

    /* AT^FDAC设置的FDAC值和AT^FWAVE设置的power值表示的含义相同，取后设置的值 */
    /*
     * 发射控制方式：
     * 0：GMSK电压控制,此方式下usTxVpa要配置；
     * 1：功率控制,此方式下usTxPower要配置；
     * 2：8PSK PaVbias电压&DBB Atten&RF Atten控制，
     * usPAVbias和usRfAtten,sDbbAtten三个参数都要配置；
     */
    if (atDevCmdCtrl->fdacFlag == VOS_TRUE) {
        txMsg->ctrlMode = AT_GHPA_RF_CTRLMODE_TYPE_GMSK;
        txMsg->txVpa    = atDevCmdCtrl->fdac; /* DAC设置 */
    } else {
        txMsg->ctrlMode = AT_GHPA_RF_CTRLMODE_TYPE_TXPOWER;
        txMsg->txPower  = atDevCmdCtrl->power;
    }

    /* Tx Cfg */
    txMsg->mask    = mask; /* 按位标识配置类型 */
    txMsg->afc     = 0;    /* AFC */
    txMsg->freqNum = (VOS_UINT16)((atDevCmdCtrl->dspBandArfcn.dspBand << 12) | atDevCmdCtrl->dspBandArfcn.ulArfcn);
    txMsg->txMode  = atDevCmdCtrl->txMode; /* 0:burst发送; 1:连续发送 */
    txMsg->modType = G_MOD_TYPE_GMSK;      /* 发射调制方式:0表示GMSK调制;1表示8PSK调制方式 */

    if (ucSwitch == AT_DSP_RF_SWITCH_ON) {
        txMsg->txEnable = GDSP_CTRL_TX_ON;
    } else {
        txMsg->txEnable = GDSP_CTRL_TX_OFF;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, txMsg) != VOS_OK) {
        AT_WARN_LOG("At_SendTxOnOffToGHPA: Send msg fail!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 At_SendRxOnOffToGHPA(VOS_UINT8 indexNum, VOS_UINT32 rxSwitch)
{
    AT_GHPA_RfRxCfgReq *rxMsg        = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl  *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32          length;
    VOS_UINT16          mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_GHPA_RfRxCfgReq消息 */
    length = sizeof(AT_GHPA_RfRxCfgReq) - VOS_MSG_HEAD_LENGTH;
    rxMsg  = (AT_GHPA_RfRxCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (rxMsg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendRxOnOffToGHPA: alloc Rx msg fail!");
        return AT_FAILURE;
    }

    /* Rx Cfg */
    TAF_CfgMsgHdr((MsgBlock *)rxMsg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_GPHY), length);

    rxMsg->msgId = ID_AT_GHPA_RF_RX_CFG_REQ;
    rxMsg->rsv   = 0;
    rxMsg->rsv2  = 0;

    mask = G_RF_MASK_RX_ARFCN | G_RF_MASK_RX_AGCMODE | G_RF_MASK_RX_AGCGAIN | G_RF_MASK_RX_MODE;

    /* Reference MASK_CAL_RF_G_RX_* section */
    rxMsg->mask = mask;

    rxMsg->freqNum = (VOS_UINT16)((atDevCmdCtrl->dspBandArfcn.dspBand << 12) | atDevCmdCtrl->dspBandArfcn.ulArfcn);

    /* 0:停止; 1:连续接收; */
    if (rxSwitch == TRUE) {
        rxMsg->rxMode = AT_GDSP_RX_MODE_CONTINOUS_BURST;
    } else {
        rxMsg->rxMode = AT_GDSP_RX_MODE_STOP;
    }

    /* 1:Fast AGC,0:Slow AGC */
    rxMsg->agcMode = AT_GDSP_RX_SLOW_AGC_MODE;

    /* AGC档位，共四档,取值为0-3 */
    rxMsg->agcGain = atDevCmdCtrl->lnaLevel;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, rxMsg) != VOS_OK) {
        AT_WARN_LOG("At_SendRxOnOffToGHPA: Send msg fail!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 At_SendRxOnOffToHPA(VOS_UINT32 rxSwitch, VOS_UINT8 indexNum)
{
    AT_HPA_RfCfgReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_HPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_HPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendRxOnOffToHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);

    /* 填写消息体 */
    msg->msgId = ID_AT_HPA_RF_CFG_REQ;

    mask = W_RF_MASK_RX_ARFCN | W_RF_MASK_RX_ANTSEL | W_RF_MASK_RX_RXONOFF;

    /* 按位标识配置类型 */
    msg->rfCfgPara.mask = mask;

    /* 1,2,3...,协议中的band编号 */
    msg->rfCfgPara.rxBand    = atDevCmdCtrl->dspBandArfcn.dspBand;
    msg->rfCfgPara.rxFreqNum = atDevCmdCtrl->dspBandArfcn.dlArfcn;

    /*
     * DSP频段Band1-Band9格式转换为
     *    W Rf support
     *         bit8       bit7      bit6     bit5    bit4     bit3      bit2     bit1
     * WCDMA   900(VIII)  2600(VII) 800(VI)  850(V)  1700(IV) 1800(III) 1900(II) 2100(I) oct1
     *         spare      spare     spare    spare   spare    spare     spare   J1700(IX)oct2
     */

    if (atDevCmdCtrl->priOrDiv == AT_RX_DIV_ON) {
        msg->rfCfgPara.rxAntSel = ANT_TWO;
    } else {
        msg->rfCfgPara.rxAntSel = ANT_ONE;
    }

    /* LNA模式控制 */
    msg->rfCfgPara.rxLnaGainMode = atDevCmdCtrl->lnaLevel;

    if (rxSwitch == AT_DSP_RF_SWITCH_ON) {
        if (msg->rfCfgPara.rxAntSel == ANT_TWO) {
            /* CPU控制打开射频芯片,天线1和2 */
            msg->rfCfgPara.rxEnable = DSP_CTRL_RX_ALL_ANT_ON;
        } else {
            /* CPU控制打开射频芯片,天线1 */
            msg->rfCfgPara.rxEnable = DSP_CTRL_RX_ANT1_ON;
        }
    } else {
        /* 关闭接收RX */
        msg->rfCfgPara.rxEnable = DSP_CTRL_RX_OFF;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendRxOnOffToHPA: Send msg fail!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 At_SendRfCfgAntSelToHPA(VOS_UINT8 divOrPriOn, VOS_UINT8 indexNum)
{
    AT_HPA_RfCfgReq   *msg          = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         length;
    VOS_UINT16         mask;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 申请AT_HPA_RfCfgReq消息 */
    length = sizeof(AT_HPA_RfCfgReq) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e830 */
    msg = (AT_HPA_RfCfgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendRfCfgAntSelToHPA: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, AT_GetDestPid(indexNum, I0_DSP_PID_WPHY), length);

    /* 填写消息体 */
    msg->msgId = ID_AT_HPA_RF_CFG_REQ;
    mask       = W_RF_MASK_RX_ARFCN | W_RF_MASK_RX_ANTSEL | W_RF_MASK_RX_RXONOFF;

    /* 按位标识配置类型 */
    msg->rfCfgPara.mask = mask;

    if (divOrPriOn == AT_RX_DIV_ON) {
        msg->rfCfgPara.rxAntSel = ANT_TWO;
        msg->rfCfgPara.rxEnable = DSP_CTRL_RX_ALL_ANT_ON;
    } else {
        msg->rfCfgPara.rxAntSel = ANT_ONE;
        msg->rfCfgPara.rxEnable = DSP_CTRL_RX_ANT1_ON;
    }

    msg->rfCfgPara.rxBand    = atDevCmdCtrl->dspBandArfcn.dspBand;
    msg->rfCfgPara.rxFreqNum = atDevCmdCtrl->dspBandArfcn.dlArfcn;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SendRfCfgAntSelToHPA: Send msg fail!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvDrvAgentSetFchanRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg         *rcvMsg       = VOS_NULL_PTR;
    DRV_AGENT_FchanSetCnf *fchanSetCnf  = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl     *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT8              indexNum     = 0;
    VOS_UINT32             error;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 初始化 */
    rcvMsg      = (DRV_AGENT_Msg *)msg;
    fchanSetCnf = (DRV_AGENT_FchanSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(fchanSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetFchanRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetFchanRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_FCHAN_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FCHAN_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 设置有错误的处理 */
    error = AT_SetFchanRspErr(fchanSetCnf->result);
    if (error != DRV_AGENT_FCHAN_SET_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, error);
        return VOS_OK;
    }

    /* 设置无错误的处理 */
    atDevCmdCtrl->dspLoadFlag   = VOS_TRUE;
    atDevCmdCtrl->deviceRatMode = (VOS_UINT8)fchanSetCnf->fchanSetReq.deviceRatMode;
    atDevCmdCtrl->deviceAtBand  = (VOS_UINT8)fchanSetCnf->fchanSetReq.deviceAtBand;
    atDevCmdCtrl->dspBandArfcn  = fchanSetCnf->fchanSetReq.dspBandArfcn;
    atDevCmdCtrl->fdac          = 0; /* FDAC清零，防止G/W范围错误 */

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_VOID At_HpaRfCfgCnfProc(HPA_AT_RfCfgCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (msg->errFlg == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_HpaRfCfgCnfProc: set rfcfg err");
        rslt = At_RfCfgCnfReturnErrProc(indexNum);
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, rslt);
    } else {
        rslt = AT_OK;
        At_RfCfgCnfReturnSuccProc(indexNum);

        /*
         * ^FRSSI?在GDSP LOAD情况会收到ID_HPA_AT_RF_CFG_CNF消息,不用上报,等收到
         * ID_HPA_AT_RF_RX_RSSI_IND消息时再上报
         */
        if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_QUERY_RSSI) {
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, rslt);
        }
    }
    return;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_VOID At_CHpaRfCfgCnfProc(CHPA_AT_RfCfgCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (msg->errFlg == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_CHpaRfCfgCnfProc: set rfcfg err");
        rslt = At_RfCfgCnfReturnErrProc(indexNum);
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, rslt);
    } else {
        rslt = AT_OK;
        At_RfCfgCnfReturnSuccProc(indexNum);

        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, rslt);
    }
    return;
}
#endif


VOS_UINT32 AT_RcvDrvAgentTseLrfSetRsp(struct MsgCB *msg)
{
    DRV_AGENT_TselrfSetCnf *event        = VOS_NULL_PTR;
    DRV_AGENT_Msg          *rcvMsg       = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl      *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32              ret;
    VOS_UINT8               indexNum = 0;

    /* 初始化 */
    rcvMsg       = (DRV_AGENT_Msg *)msg;
    event        = (DRV_AGENT_TselrfSetCnf *)rcvMsg->content;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentTseLrfSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentTseLrfSetRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待TSELRF设置命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TSELRF_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    g_atSendDataBuff.bufLen = 0;
    if (event->result == DRV_AGENT_TSELRF_SET_NO_ERROR) {
        /* 设置错误码为AT_OK */
        ret                         = AT_OK;
        atDevCmdCtrl->dspLoadFlag   = VOS_TRUE;
        atDevCmdCtrl->deviceRatMode = event->deviceRatMode;
        atDevCmdCtrl->fdac          = 0;
    } else {
        /* 查询失败返回ERROR字符串 */
        ret = AT_ERROR;
    }

    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_VOID At_RfFpowdetTCnfProc(PHY_AT_PowerDetCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPOWDET_QRY) {
        AT_WARN_LOG("At_RfFPOWDETCnfProc: CmdCurrentOpt is not AT_CMD_FPOWDET_QRY!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 应物理层要求，如果返回值为0x7FFF则为无效值，项查询者返回ERROR */
    if (msg->powerDet == 0x7FFF) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, msg->powerDet);

        g_atSendDataBuff.bufLen = length;

        At_FormatResultData(indexNum, AT_OK);
    }

    return;
}


VOS_UINT32 AT_RcvMtaPowerDetQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg            *mtaMsg         = VOS_NULL_PTR;
    MTA_AT_PowerDetQryCnf *powerDetQryCnf = VOS_NULL_PTR;
    PHY_AT_PowerDetCnf     powerNetMsg;

    /* 初始化消息变量 */
    mtaMsg         = (AT_MTA_Msg *)msg;
    powerDetQryCnf = (MTA_AT_PowerDetQryCnf *)mtaMsg->content;

    (VOS_VOID)memset_s(&powerNetMsg, sizeof(powerNetMsg), 0x00, sizeof(PHY_AT_PowerDetCnf));

    if (powerDetQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        powerNetMsg.powerDet = powerDetQryCnf->powerDet;
    } else {
        powerNetMsg.powerDet = 0x7FFF;
    }

    At_RfFpowdetTCnfProc(&powerNetMsg);

    return VOS_OK;
}


VOS_VOID At_RfPllStatusCnfProc(PHY_AT_RfPllStatusCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT16         length;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPLLSTATUS_QRY) {
        AT_WARN_LOG("At_RfPllStatusCnfProc: CmdCurrentOpt is not AT_CMD_FPLLSTATUS_QRY!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, msg->txStatus,
        msg->rxStatus);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, AT_OK);

    return;
}


VOS_VOID At_RfRssiIndProc(HPA_AT_RfRxRssiInd *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;
    VOS_UINT16         length;
    VOS_INT32          rssi;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (msg->agcGain == AT_DSP_RF_AGC_STATE_ERROR) { /* 错误 */
        AT_WARN_LOG("AT_RfRssiIndProc err");
        rslt = AT_FRSSI_OTHER_ERR;
    } else {
        g_atSendDataBuff.bufLen = 0;

        /* 由于RSSI测量值单位0.125dBm，为了消除浮点数*1000. */
        rssi = (VOS_INT32)msg->rssi * AT_DSP_RSSI_VALUE_MUL_THOUSAND;

        /*
         * 读取的RSSI值，采用正值上报，精确到0.1dBm定制值信息。如果当前的RSSI
         * 值为-85.1dBm，返回值为851. 由于之前乘1000，所以精确到0.1dBm这里要除100
         */
        if (rssi < 0) {
            rssi = (-1 * rssi) / 100;
        } else {
            rssi = rssi / 100;
        }

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, rssi);

        g_atSendDataBuff.bufLen = length;
        rslt                    = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return;
}


VOS_UINT32 At_PdmCtrlCnfProc(HPA_AT_PdmCtrlCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PDM_CTRL) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->result == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_PdmCtrlCnfProc: read PdmCtrl err");
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_VOID At_RfCfgCnfReturnSuccProc(VOS_UINT8 indexNum)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    atDevCmdCtrl = AT_GetDevCmdCtrl();

    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_SET_FTXON:
            atDevCmdCtrl->txOnOff = atDevCmdCtrl->tempRxorTxOnOff;

            /* 如果是打开发射机操作，需要记录最近一次执行的是打开发射机还是打开接收机操作 */
            if (atDevCmdCtrl->txOnOff == AT_DSP_RF_SWITCH_ON) {
                atDevCmdCtrl->rxonOrTxon = AT_TXON_OPEN;
            }
            break;

        case AT_CMD_SET_FRXON:
            atDevCmdCtrl->rxOnOff = atDevCmdCtrl->tempRxorTxOnOff;

            /* 如果是打开接收机操作，需要记录最近一次执行的是打开发射机还是打开接收机操作 */
            if (atDevCmdCtrl->rxOnOff == AT_DSP_RF_SWITCH_ON) {
                atDevCmdCtrl->rxonOrTxon = AT_RXON_OPEN;
            }
            break;

        case AT_CMD_QUERY_RSSI:
            break;

        case AT_CMD_SET_RXDIV:
            if ((At_SaveRxDivPara(atDevCmdCtrl->origBand, 1) == AT_OK) &&
                (atDevCmdCtrl->currentTMode == AT_TMODE_FTM)) {
                atDevCmdCtrl->priOrDiv = AT_RX_DIV_ON;
                atDevCmdCtrl->rxDiv    = atDevCmdCtrl->origBand;
            }
            break;

        case AT_CMD_SET_RXPRI:
            atDevCmdCtrl->priOrDiv = AT_RX_PRI_ON;
            atDevCmdCtrl->rxPri    = atDevCmdCtrl->origBand;
            break;

        default:
            break;
    }

    return;
}


VOS_VOID At_WTxCltIndProc(WPHY_AT_TxCltInd *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    /* 初始化全局变量 */
    atDevCmdCtrl = AT_GetDevCmdCtrl();
    (VOS_VOID)memset_s(&atDevCmdCtrl->cltInfo, sizeof(atDevCmdCtrl->cltInfo), 0x0, sizeof(AT_TX_CltInfo));

    /* 设置CLT信息有效标志 */
    atDevCmdCtrl->cltInfo.infoAvailableFlg = VOS_TRUE;

    /* 将接入层上报的信息记录下全局变量中 */
    atDevCmdCtrl->cltInfo.gammaReal = msg->gammaReal; /* 反射系数实部 */
    atDevCmdCtrl->cltInfo.gammaImag = msg->gammaImag; /* 反射系数虚部 */
    /* 驻波检测场景0反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc0 = msg->gammaAmpUc0;
    /* 驻波检测场景1反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc1 = msg->gammaAmpUc1;
    /* 驻波检测场景2反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc2        = msg->gammaAmpUc2;
    atDevCmdCtrl->cltInfo.gammaAntCoarseTune = msg->gammaAntCoarseTune; /* 粗调格点位置 */
    atDevCmdCtrl->cltInfo.ulwFomcoarseTune   = msg->fomcoarseTune;      /* 粗调FOM值 */
    atDevCmdCtrl->cltInfo.cltAlgState        = msg->cltAlgState;        /* 闭环算法收敛状态 */
    atDevCmdCtrl->cltInfo.cltDetectCount     = msg->cltDetectCount;     /* 闭环收敛总步数 */
    atDevCmdCtrl->cltInfo.dac0               = msg->dac0;               /* DAC0 */
    atDevCmdCtrl->cltInfo.dac1               = msg->dac1;               /* DAC1 */
    atDevCmdCtrl->cltInfo.dac2               = msg->dac2;               /* DAC2 */
    atDevCmdCtrl->cltInfo.dac3               = msg->dac3;               /* DAC3 */

    return;
}


VOS_UINT32 AT_RcvMtaRficSsiRdQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg          *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_RficssirdCnf *rficSsiRdCnf = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl   *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum     = 0;
    mtaMsg       = (AT_MTA_Msg *)msg;
    rficSsiRdCnf = (MTA_AT_RficssirdCnf *)mtaMsg->content;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    indexNum = atDevCmdCtrl->index;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaRficSsiRdQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_RFICSSIRD_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RFICSSIRD_SET) {
        AT_WARN_LOG("AT_RcvMtaRficSsiRdQryCnf: WARNING:Not AT_CMD_REFCLKFREQ_READ!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出寄存器的值 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            rficSsiRdCnf->regValue);
    result = AT_OK;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 At_MipiRdCnfProc(HPA_AT_MipiRdCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT32         data;
    VOS_UINT16         length;
    VOS_UINT8          indexNum;

    /* 初始化本地变量 */
    length       = 0;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPI_RD) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->result == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_MipiRdCnfProc: read mipi err");
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
        data = msg->value;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, data);
    }
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, rslt);
    return VOS_OK;
}


VOS_UINT32 At_MipiWrCnfProc(HPA_AT_MipiWrCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPI_WR) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->errFlg == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_MipiWrCnfProc: set MipiCfg err");
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 At_SsiWrCnfProc(HPA_AT_SsiWrCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SSI_WR) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->errFlg == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_SsiWrCnfProc: set rfcfg err");
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 At_SsiRdCnfProc(HPA_AT_SsiRdCnf *msg)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT32         data;
    VOS_UINT16         length;
    VOS_UINT8          indexNum;

    /* 初始化本地变量 */
    length       = 0;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 获取本地保存的用户索引 */
    indexNum = atDevCmdCtrl->index;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SSI_RD) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->result == AT_HPA_RSLT_FAIL) {
        AT_INFO_LOG("At_MipiRdCnfProc: read SSI err");
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
        data = msg->value;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, data);
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_SetFchanRspErr(DRV_AGENT_FchanSetErrorUint32 result)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    if (result == DRV_AGENT_FCHAN_BAND_NOT_MATCH) {
        return AT_FCHAN_BAND_NOT_MATCH;
    }

    if (result == DRV_AGENT_FCHAN_BAND_CHANNEL_NOT_MATCH) {
        return AT_FCHAN_BAND_CHANNEL_NOT_MATCH;
    }

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    if (result == DRV_AGENT_FCHAN_OTHER_ERR) {
        atDevCmdCtrl->dspLoadFlag = VOS_FALSE;
        AT_WARN_LOG("AT_SetFChanPara: DSP Load fail!");
        return AT_FCHAN_OTHER_ERR;
    }

    return DRV_AGENT_FCHAN_SET_NO_ERROR;
}


VOS_UINT32 At_RfCfgCnfReturnErrProc(VOS_UINT8 indexNum)
{
    VOS_UINT32 rslt;

    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_SET_FTXON:
            rslt = AT_FTXON_SET_FAIL;
            break;

        case AT_CMD_SET_FRXON:
            rslt = AT_FRXON_SET_FAIL;
            break;

        case AT_CMD_QUERY_RSSI:
            rslt = AT_FRSSI_OTHER_ERR;
            break;

        /* 设置主集和分集时收到DSP回复出错情况下返回的错误码相同 */
        case AT_CMD_SET_RXDIV:
        case AT_CMD_SET_RXPRI:
            rslt = AT_CME_RX_DIV_OTHER_ERR;
            break;

        default:
            rslt = AT_ERROR;
            break;
    }

    return rslt;
}

#else

VOS_VOID AT_MT_ClearMsgEntity(AT_MT_MsgHeader *msg)
{
    (VOS_VOID)memset_s(AT_MT_GET_MSG_ENTITY(msg), AT_MT_GET_MSG_LENGTH(msg), 0x00, AT_MT_GET_MSG_LENGTH(msg));

    return;
}


VOS_UINT32 At_SndRxOnOffReq(VOS_VOID)
{
    AT_BBIC_CAL_MT_RX_REQ *rxReq       = VOS_NULL_PTR;
    AT_MT_Info            *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 分配消息空间 */
    rxReq = (AT_BBIC_CAL_MT_RX_REQ *)PS_ALLOC_MSG(WUEPS_PID_AT, sizeof(AT_BBIC_CAL_MT_RX_REQ) - VOS_MSG_HEAD_LENGTH);
    if (rxReq == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(rxReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(rxReq, DSP_PID_BBA_CAL, ID_AT_BBIC_CAL_MT_RX_REQ);

    /* 填写消息内容 */
#if (FEATURE_LTEV == FEATURE_ON)
    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_LTEV) {
        rxReq->stComponentID.uwComponentType = AT_COMPONENTTYPE_RAT_LTEV;
    }
#endif
    rxReq->stMtRxPara.ucRxEnable      = atMtInfoCtx->atInfo.tempRxorTxOnOff;
    rxReq->stMtRxPara.uhwAgcGainIndex = atMtInfoCtx->atInfo.agcGainLevel;
    rxReq->stMtRxPara.enModemId       = MODEM_ID_0;
    rxReq->stMtRxPara.enRatMode       = atMtInfoCtx->bbicInfo.currtRatMode;
    rxReq->stMtRxPara.enBand          = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;
    rxReq->stMtRxPara.enBandWith      = atMtInfoCtx->bbicInfo.bandWidth;
    rxReq->stMtRxPara.dlFreqInfo      = atMtInfoCtx->bbicInfo.dspBandFreq.dlFreq;

    /* MIMO */
    if (atMtInfoCtx->bbicInfo.rxAntType == AT_ANT_TYPE_MIMO) {
        rxReq->stMtRxPara.usMimoType = atMtInfoCtx->bbicInfo.rxMimoType;
        rxReq->stMtRxPara.usAntMap   = atMtInfoCtx->bbicInfo.rxMimoAntNum;
    } else {
        rxReq->stMtRxPara.usMimoType = AT_SET_BIT32(1);
        rxReq->stMtRxPara.usAntMap   = atMtInfoCtx->bbicInfo.rxAntType;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, rxReq) != VOS_OK) {
        return VOS_FALSE;
    }

    AT_PR_LOGH("At_SndRxOnOffReq Exit");

    return VOS_TRUE;
}


VOS_UINT32 At_SndTxOnOffReq(VOS_UINT16 powerDetFlg)
{
    AT_BBIC_CAL_MT_TX_REQ *txReq       = VOS_NULL_PTR;
    AT_MT_Info            *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 分配消息空间 */
    txReq = (AT_BBIC_CAL_MT_TX_REQ *)PS_ALLOC_MSG(WUEPS_PID_AT, sizeof(AT_BBIC_CAL_MT_TX_REQ) - VOS_MSG_HEAD_LENGTH);
    if (txReq == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(txReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(txReq, DSP_PID_BBA_CAL, ID_AT_BBIC_CAL_MT_TX_REQ);

    /* 填写消息内容 */
#if (FEATURE_LTEV == FEATURE_ON)
    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_LTEV) {
        txReq->stComponentID.uwComponentType = AT_COMPONENTTYPE_RAT_LTEV;
    }
#endif
    txReq->stMtTxPara.ucTxEnable             = atMtInfoCtx->atInfo.tempRxorTxOnOff;
    txReq->stMtTxPara.ucTxPaMode             = atMtInfoCtx->bbicInfo.paLevel;
    txReq->stMtTxPara.enScsType              = atMtInfoCtx->bbicInfo.bbicScs;
    txReq->stMtTxPara.usMrxEanble            = powerDetFlg;
    txReq->stMtTxPara.enModemId              = MODEM_ID_0;
    txReq->stMtTxPara.enRatMode              = atMtInfoCtx->bbicInfo.currtRatMode;
    txReq->stMtTxPara.enBand                 = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;
    txReq->stMtTxPara.enBandWith             = atMtInfoCtx->bbicInfo.bandWidth;
    txReq->stMtTxPara.ulFreqInfo             = atMtInfoCtx->bbicInfo.dspBandFreq.ulFreq;
    txReq->stMtTxPara.enPowerCtrlMode        = POWER_CTRL_MODE_POWER;
    txReq->stMtTxPara.unPowerPara.shwTxPower = atMtInfoCtx->bbicInfo.fwavePower;
    txReq->stMtTxPara.rbCfgEnable            = atMtInfoCtx->atInfo.rbEnable;

    /* 单音还是调制 */
    if (atMtInfoCtx->bbicInfo.fwaveType == MODU_TYPE_BUTT) {
        txReq->stMtTxPara.ucIsSingleTone = VOS_TRUE;           /* 单音 */
        txReq->stMtTxPara.enModType      = MODU_TYPE_LTE_QPSK; /* 单音默认写QPSK */

        if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_GSM) {
            txReq->stMtTxPara.enModType = MODU_TYPE_GMSK;
        }

        if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_EDGE) {
            txReq->stMtTxPara.enModType = MODU_TYPE_8PSK;
        }
    } else {
        txReq->stMtTxPara.ucIsSingleTone = VOS_FALSE; /* 调制 */
        txReq->stMtTxPara.enModType      = atMtInfoCtx->bbicInfo.fwaveType;
    }

    /* MIMO类型 */
    if (atMtInfoCtx->bbicInfo.txAntType == AT_ANT_TYPE_MIMO) {
        txReq->stMtTxPara.usMimoType = atMtInfoCtx->bbicInfo.txMimoType;
        txReq->stMtTxPara.usAntMap   = atMtInfoCtx->bbicInfo.txMimoAntNum;
    } else {
        /* 默认是1 TX */
        txReq->stMtTxPara.usMimoType = AT_SET_BIT32(0);
        txReq->stMtTxPara.usAntMap   = AT_MIMO_ANT_NUM_1;
    }

    /* RB配置信息 */
    if (txReq->stMtTxPara.rbCfgEnable == VOS_TRUE) {
        txReq->stMtTxPara.rbNumCfg   = atMtInfoCtx->atInfo.rbNum;
        txReq->stMtTxPara.rbStartCfg = atMtInfoCtx->atInfo.rbStartCfg;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, txReq) != VOS_OK) {
        return VOS_FALSE;
    }

    AT_PR_LOGH("At_SndTxOnOffReq Exit");

    return VOS_TRUE;
}


VOS_UINT32 At_SndGsmTxOnOffReq_ModulatedWave(VOS_UINT16 powerDetFlg)
{
    BBIC_CAL_RF_DEBUG_GSM_TX_REQ_STRU *gsmTxReq    = VOS_NULL_PTR;
    AT_MT_Info                        *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                         totalNum;
    VOS_UINT32                         indexNum;
    MODU_TYPE_ENUM_UINT16              modType;

    modType     = MODU_TYPE_GMSK;
    atMtInfoCtx = AT_GetMtInfoCtx();
    /* 分配消息空间 */
    gsmTxReq = (BBIC_CAL_RF_DEBUG_GSM_TX_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                                 sizeof(BBIC_CAL_RF_DEBUG_GSM_TX_REQ_STRU) -
                                                                 VOS_MSG_HEAD_LENGTH);
    if (gsmTxReq == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(gsmTxReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(gsmTxReq, DSP_PID_BBA_CAL, ID_AT_BBIC_CAL_RF_DEBUG_GSM_TX_REQ);

    gsmTxReq->stPara.uhwTxEnable                = atMtInfoCtx->atInfo.tempRxorTxOnOff;
    gsmTxReq->stPara.uhwMrxEanble               = powerDetFlg;
    gsmTxReq->stPara.stCommonInfo.enModemId     = MODEM_ID_0;
    gsmTxReq->stPara.stCommonInfo.enRatMode     = atMtInfoCtx->bbicInfo.currtRatMode;
    gsmTxReq->stPara.stCommonInfo.enBand        = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;
    gsmTxReq->stPara.stCommonInfo.enBandWith    = atMtInfoCtx->bbicInfo.bandWidth;
    gsmTxReq->stPara.stCommonInfo.uhwSignalType = BBIC_CAL_SIGNAL_TYPE_MODU;
    gsmTxReq->stPara.stCommonInfo.uhwUlPath     = AT_GetGsmUlPathByBandNo(atMtInfoCtx->bbicInfo.dspBandFreq.dspBand);
    gsmTxReq->stPara.stCommonInfo.enModType     = 0;

    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_GSM) {
        modType = MODU_TYPE_GMSK;
    }

    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_EDGE) {
        modType = MODU_TYPE_8PSK;
    }

    /* GSM TX 没有MIMO */
    gsmTxReq->stPara.stCommonInfo.uhwMimoType  = 0;
    gsmTxReq->stPara.stCommonInfo.uwTxFreqInfo = atMtInfoCtx->bbicInfo.dspBandFreq.ulFreq;

    gsmTxReq->stPara.uhwDataPattern = 1; /* 默认采用随机数 */
    gsmTxReq->stPara.enMrxEstMode   = MRX_ESTIMATE_POWER_MODE;
    gsmTxReq->stPara.uhwSlotCnt     = atMtInfoCtx->atInfo.gsmTxSlotType;

    totalNum = atMtInfoCtx->atInfo.gsmTxSlotType; /* 需要填写几个数组 */
    if (atMtInfoCtx->atInfo.gsmTxSlotType == AT_GSM_TX_8_SLOT) {
        totalNum                    = AT_GSM_TX_1_SLOT;
        gsmTxReq->stPara.uhwSlotCnt = AT_BBIC_CAL_MAX_GSM_SLOT; /* 8个slot */
    }

    for (indexNum = 0; indexNum < totalNum; indexNum++) {
        gsmTxReq->stPara.astSlotPara[indexNum].uhwModuType                          = modType;
        gsmTxReq->stPara.astSlotPara[indexNum].stTxPowerPara.enPowerCtrlMode        = POWER_CTRL_MODE_POWER;
        gsmTxReq->stPara.astSlotPara[indexNum].stTxPowerPara.stPaPara.ucTxPaMode    = atMtInfoCtx->bbicInfo.paLevel;
        gsmTxReq->stPara.astSlotPara[indexNum].stTxPowerPara.unPowerPara.shwTxPower = atMtInfoCtx->bbicInfo.fwavePower;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, gsmTxReq) != VOS_OK) {
        return VOS_FALSE;
    }

    AT_PR_LOGH("At_SndGsmTxOnOffReq_ModulatedWave Exit");

    return VOS_TRUE;
}


VOS_UINT32 At_LoadPhy(VOS_VOID)
{
    AT_CCBT_LOAD_PHY_REQ_STRU *loadPhyReq  = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;

    /* 分配消息空间 */
    loadPhyReq = (AT_CCBT_LOAD_PHY_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                           sizeof(AT_CCBT_LOAD_PHY_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (loadPhyReq == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(loadPhyReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(loadPhyReq, CCPU_PID_CBT, ID_AT_CCBT_SET_WORK_MODE_REQ);

    /* 填写消息内容 */
#if (FEATURE_LTEV == FEATURE_ON)
    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_LTEV) {
        loadPhyReq->stComponentId.ucComponentType = AT_COMPONENTTYPE_RAT_LTEV;
    }
#endif
    loadPhyReq->stLoadphyPara.aenRatMode[0] = atMtInfoCtx->bbicInfo.currtRatMode;
    loadPhyReq->stLoadphyPara.enBusiness    = CBT_BUSINESS_TYPE_MT;

    loadPhyReq->stLoadphyPara.nosigType = CBT_NOSIG_TYPE_LF;
    if (AT_GetMtInfoCtx()->bbicInfo.dspBandFreq.dspBand >= AT_PROTOCOL_BAND_256) {
        loadPhyReq->stLoadphyPara.nosigType = CBT_NOSIG_TYPE_HF;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, loadPhyReq) != VOS_OK) {
        return VOS_FALSE;
    }

    AT_PR_LOGH("At_LoadPhy Exit");

    return VOS_TRUE;
}


VOS_VOID AT_ProcCbtMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 rst;
    VOS_UINT16 msgId;

    msgCnt = sizeof(g_atProcCbtMsgTab) / sizeof(AT_PROC_CbtMsg);

    msgId = ((AT_MT_MsgHeader *)msg)->usMsgId;

    /* g_atProcCbtMsgTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcCbtMsgTab[i].msgType == msgId) {
            rst = g_atProcCbtMsgTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcCbtMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcCbtMsg: Msg Id is invalid!");
    }

    return;
}


VOS_VOID AT_ProcUeCbtMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 rst;
    VOS_UINT16 msgId;

    msgCnt = sizeof(g_atProcUeCbtMsgTab) / sizeof(AT_PROC_UecbtMsg);

    msgId = ((AT_MT_MsgHeader *)msg)->usMsgId;

    for (i = 0; i < msgCnt; i++) {
        if (g_atProcUeCbtMsgTab[i].msgType == msgId) {
            rst = g_atProcUeCbtMsgTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcUeCbtMsg: Msg Proc Err!");
            }

            return;
        }
    }

    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcUeCbtMsg: Msg Id is invalid!");
    }

    return;
}


VOS_VOID AT_ProcDspIdleMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 rst;
    VOS_UINT16 msgId;

    msgCnt = sizeof(g_atProcDspIdleMsgTab) / sizeof(AT_PROC_DspIdleMsg);

    msgId = ((AT_MT_MsgHeader *)msg)->usMsgId;

    for (i = 0; i < msgCnt; i++) {
        if (g_atProcDspIdleMsgTab[i].msgType == msgId) {
            rst = g_atProcDspIdleMsgTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcDspIdleMsg: Msg Proc Err!");
            }

            return;
        }
    }

    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcDspIdleMsg: Msg Id is invalid!");
    }

    return;
}


VOS_VOID AT_ProcBbicMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 rst;
    VOS_UINT16 msgId;

    msgCnt = sizeof(g_atProcBbicMsgTab) / sizeof(AT_PROC_BbicMsg);

    msgId = ((AT_MT_MsgHeader *)msg)->usMsgId;

    /* g_atProcBbicMsgTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcBbicMsgTab[i].msgType == msgId) {
            rst = g_atProcBbicMsgTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcBbicMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcBbicMsg: Msg Id is invalid!");
    }

    return;
}


VOS_UINT32 AT_ProcSetWorkModeCnf(struct MsgCB *msg)
{
    AT_CCBT_LOAD_PHY_CNF_STRU *loadCnf     = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;

    loadCnf     = (AT_CCBT_LOAD_PHY_CNF_STRU *)msg;
    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;

    AT_PR_LOGH("AT_ProcSetWorkModeCnf Enter");

    /* 下标保护 */
    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcSetWorkModeCnf: ulIndex err!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_FCHAN_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FCHAN_SET) {
        AT_ERR_LOG("AT_ProcSetWorkModeCnf: Not AT_CMD_FCHAN_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 非0表示错误 */
    if (loadCnf->ulErrorCode != MT_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_FCHAN_LOAD_DSP_ERR);
        return VOS_OK;
    }

    g_atSendDataBuff.bufLen         = 0;
    atMtInfoCtx->atInfo.dspLoadFlag = VOS_TRUE;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_ProcGsmTxCnf_ModulateWave(struct MsgCB *msg)
{
    BBIC_CAL_MSG_CNF_STRU *cnf         = VOS_NULL_PTR;
    AT_MT_Info            *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT8              indexNum;

    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    cnf         = (BBIC_CAL_MSG_CNF_STRU *)msg;

    AT_PR_LOGH("AT_ProcGsmTxCnf_ModulateWave Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcGsmTxCnf_ModulateWave: ulIndex err!");
        return VOS_ERR;
    }

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SET_FTXON) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPOWDET_QRY)) {
        AT_ERR_LOG("AT_ProcGsmTxCnf_ModulateWave: Not Set FTXON,FPOWDET QRY !");
        return VOS_ERR;
    }

    /* Power Det是借助TXON的消息发送的，如果是查询power det，除了回这条消息，还要回复 ID_BBIC_TOOL_CAL_RF_DEBUG_TX_RESULT_IND */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_FPOWDET_QRY) {
        if (cnf->stPara.uwErrorCode != MT_OK) {
            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_ERROR);
            return VOS_OK;
        }

        return VOS_OK;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnf->stPara.uwErrorCode == MT_OK) {
        atMtInfoCtx->atInfo.txOnOff = atMtInfoCtx->atInfo.tempRxorTxOnOff;
        g_atSendDataBuff.bufLen     = 0;
        At_FormatResultData(indexNum, AT_OK);
    } else {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_FTXON_SET_FAIL);
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcTxCnf(struct MsgCB *msg)
{
    BBIC_CAL_AT_MT_TRX_CNF *cnf         = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT8               indexNum;

    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    cnf         = (BBIC_CAL_AT_MT_TRX_CNF *)msg;

    AT_PR_LOGH("AT_ProcTxCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcTxCnf: ulIndex err!");
        return VOS_ERR;
    }

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SET_FTXON) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPOWDET_QRY)) {
        AT_ERR_LOG("AT_ProcTxCnf: Not Set FTXON,FPOWDET QRY !");
        return VOS_ERR;
    }

    /* Power Det是借助TXON的消息发送的，如果是查询power det，除了回这条消息，还要回复 ID_BBIC_TOOL_CAL_RF_DEBUG_TX_RESULT_IND */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_FPOWDET_QRY) {
        if (cnf->ulErrCode != MT_OK) {
            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_ERROR);
        }

        return VOS_OK;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnf->ulErrCode == MT_OK) {
        atMtInfoCtx->atInfo.txOnOff = atMtInfoCtx->atInfo.tempRxorTxOnOff;
        g_atSendDataBuff.bufLen     = 0;
        At_FormatResultData(indexNum, AT_OK);
    } else {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_FTXON_SET_FAIL);
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcRxCnf(struct MsgCB *msg)
{
    BBIC_CAL_AT_MT_TRX_CNF *cnf         = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT8               indexNum;

    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    cnf         = (BBIC_CAL_AT_MT_TRX_CNF *)msg;

    AT_PR_LOGH("AT_ProcRxCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcRxCnf: ulIndex err!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SET_FRXON) {
        AT_ERR_LOG("AT_ProcRxCnf: Not Set FRXON QRY !");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnf->ulErrCode == MT_OK) {
        atMtInfoCtx->atInfo.rxOnOff = atMtInfoCtx->atInfo.tempRxorTxOnOff;
        g_atSendDataBuff.bufLen     = 0;
        At_FormatResultData(indexNum, AT_OK);
    } else {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_FRXON_SET_FAIL);
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcPowerDetCnf(struct MsgCB *msg)
{
    BBIC_CAL_AT_MT_TX_PD_IND *pdResultInd = VOS_NULL_PTR;
    AT_MT_Info               *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum;
    VOS_INT32                 powerValue;

    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    length      = 0;
    pdResultInd = (BBIC_CAL_AT_MT_TX_PD_IND *)msg;

    AT_PR_LOGH("AT_ProcPowerDetCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcPowerDetCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPOWDET_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (pdResultInd->mtTxIndPara.errorCode != MT_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    if ((atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_LTE) || (atMtInfoCtx->bbicInfo.currtRatMode == RAT_MODE_NR)) {
        /* LTE和NR DSP上报精度为0.125 */
        powerValue = (VOS_INT32)pdResultInd->mtTxIndPara.antPower * 10 / 8; /* 上报精度0.1dB */
    } else {
        /* GUC上报精度为0.1 */
        powerValue = pdResultInd->mtTxIndPara.antPower;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^FPOWDET:%d", powerValue);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_ProcPowerDetCnf_ModulateWave(struct MsgCB *msg)
{
    BBIC_CAL_RF_DEBUG_GTX_MRX_IND_STRU *pdResultInd = VOS_NULL_PTR;
    AT_MT_Info                         *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                          totalNum;
    VOS_UINT32                          j;
    VOS_UINT16                          length;
    VOS_UINT8                           indexNum;

    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    length      = 0;
    pdResultInd = (BBIC_CAL_RF_DEBUG_GTX_MRX_IND_STRU *)msg;

    AT_PR_LOGH("AT_ProcPowerDetCnf_ModulateWave Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcPowerDetCnf_ModulateWave: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPOWDET_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (pdResultInd->stPara.uwErrorCode != MT_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    totalNum = atMtInfoCtx->atInfo.gsmTxSlotType; /* 需要读取几个数组 */
    if (atMtInfoCtx->atInfo.gsmTxSlotType == AT_GSM_TX_8_SLOT) {
        totalNum = BBIC_CAL_GSM_TX_SLOT_NUM;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^FPOWDET:");

    for (j = 0; j < totalNum; j++) {
        /* GUC上报精度为0.1 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", pdResultInd->stPara.astSlotValue[j].antPower);
    }

    /* 将最后一个逗号去掉 */
    length = length - 1;

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_SndBbicCalMipiReadReq(VOS_UINT32 mipiPortSel, VOS_UINT32 secondaryId, VOS_UINT32 regAddr,
                                    VOS_UINT32 byteCnt, VOS_UINT32 readSpeed)
{
    BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU *msg = VOS_NULL_PTR;
    VOS_UINT32                            length;

    /* 申请BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU消息 */
    length = sizeof(BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    msg    = (BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SndBbicCalMipiReadReq: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_MIPI_READ_REQ);

    /* 与产品线确认，每次只读写一个MIPI */
    msg->stPara.uwMipiNum                = 1;
    msg->stPara.astCMD[0].bitMipiPortSel = mipiPortSel;
    msg->stPara.astCMD[0].bitSecondaryId = secondaryId;
    msg->stPara.astCMD[0].bitRegAddr     = regAddr;
    msg->stPara.astCMD[0].bitByteCnt     = byteCnt;
    msg->stPara.readSpeedType            = (VOS_UINT8)readSpeed;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SndBbicCalMipiReadReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("AT_SndBbicCalMipiReadReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 AT_SndBbicCalMipiWriteReq(VOS_UINT32 mipiPortSel, VOS_UINT32 secondaryId, VOS_UINT32 regAddr,
                                     VOS_UINT32 byteCnt, VOS_UINT32 value)
{
    BBIC_CAL_RF_DEBUG_WRITE_MIPI_REQ_STRU *msg = VOS_NULL_PTR;
    VOS_UINT32                             length;

    /* 申请BBIC_CAL_RF_DEBUG_READ_MIPI_REQ_STRU消息 */
    length = sizeof(BBIC_CAL_RF_DEBUG_WRITE_MIPI_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    msg    = (BBIC_CAL_RF_DEBUG_WRITE_MIPI_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SndBbicCalMipiWriteReqMsg: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_MIPI_WRITE_REQ);

    msg->stPara.uwMipiNum                       = 1;
    msg->stPara.astData[0].stCmd.bitMipiPortSel = mipiPortSel;
    msg->stPara.astData[0].stCmd.bitSecondaryId = secondaryId;
    msg->stPara.astData[0].stCmd.bitRegAddr     = regAddr;
    msg->stPara.astData[0].stCmd.bitByteCnt     = byteCnt;
    msg->stPara.astData[0].stData.bitByte0      = value;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SndBbicCalMipiWriteReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("AT_SndBbicCalMipiWriteReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvBbicCalMipiRedCnf(struct MsgCB *msg)
{
    BBIC_CAL_RF_DEBUG_READ_MIPI_IND_STRU *redCnf      = VOS_NULL_PTR;
    AT_MT_Info                           *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                            rslt;
    VOS_UINT32                            byteCnt;
    VOS_UINT16                            length;
    VOS_UINT8                             indexNum;

    rslt        = AT_OK;
    length      = 0;
    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 获取本地保存的用户索引 */
    indexNum = atMtInfoCtx->atInfo.indexNum;
    redCnf   = (BBIC_CAL_RF_DEBUG_READ_MIPI_IND_STRU *)msg;

    AT_PR_LOGH("AT_RcvBbicCalMipiRedCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_RcvBbicCalMipiRedCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPIOPERATE_SET) {
        AT_ERR_LOG("AT_RcvBbicCalMipiRedCnf: CmdCurrentOpt is not AT_CMD_MIPIOPERATE_SET!");
        return VOS_ERR;
    }

    if (redCnf->uwResult == MT_OK) {
        rslt    = AT_OK;
        byteCnt = redCnf->stPara.astData[0].stCmd.bitByteCnt;

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            redCnf->stPara.astData[0].stData.bitByte0);

        if (byteCnt > 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN - length, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redCnf->stPara.astData[0].stData.bitByte1);
        }

        if (byteCnt > AT_CAL_MIPIDEV_BYTE_CNT_VALUE_2) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN - length, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redCnf->stPara.astData[0].stData.bitByte2);
        }

        if (byteCnt > AT_CAL_MIPIDEV_BYTE_CNT_VALUE_3) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN - length, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redCnf->stPara.astData[0].stData.bitByte3);
        }

        g_atSendDataBuff.bufLen = length;
    } else {
        AT_INFO_LOG("AT_RcvBbicCalMipiRedCnfMsg: read mipi err");
        rslt = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_RcvBbicCalMipiWriteCnf(struct MsgCB *msg)
{
    BBIC_CAL_RF_DEBUG_WRITE_MIPI_IND_STRU *writeCnf    = VOS_NULL_PTR;
    AT_MT_Info                            *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                             rslt;
    VOS_UINT8                              indexNum;

    /* 获取本地保存的用户索引 */
    rslt        = AT_OK;
    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    writeCnf    = (BBIC_CAL_RF_DEBUG_WRITE_MIPI_IND_STRU *)msg;

    AT_PR_LOGH("AT_RcvBbicCalMipiWriteCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_RcvBbicCalMipiWriteCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPIOPERATE_SET) {
        AT_ERR_LOG("AT_RcvBbicCalMipiWriteCnf: CmdCurrentOpt is not AT_CMD_MIPIOPERATE_SET!");
        return VOS_ERR;
    }

    if (writeCnf->uwResult == MT_OK) {
        rslt = AT_OK;
    } else {
        rslt = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_SndBbicPllStatusReq(VOS_VOID)
{
    BBIC_CAL_PLL_QRY_REQ_STRU *msg         = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                 length;

    /* 申请BBIC_CAL_PLL_QRY_REQ_STRU消息 */
    length = sizeof(BBIC_CAL_PLL_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    msg    = (BBIC_CAL_PLL_QRY_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SndBbicPllStatusReq: Alloc msg fail!");
        return AT_FAILURE;
    }

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_PLL_QRY_REQ);

    msg->stPara.uhwBand = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;
    /* GSM  会用这个参数，但GSM不测调制模式 */
    msg->stPara.enModType = MODU_TYPE_BUTT;
    msg->stPara.enRatMode = atMtInfoCtx->bbicInfo.currtRatMode;

    /* 向对应PHY发送消息 */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SndBbicPllStatusReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("AT_SndBbicPllStatusReq Exit");

    return AT_SUCCESS;
}

VOS_UINT32 At_RcvBbicPllStatusCnf(struct MsgCB *msg)
{
    BBIC_CAL_PLL_QRY_IND_STRU *qryCnf      = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                 rslt;
    VOS_UINT32                 pllStatus;
    VOS_UINT16                 length;
    VOS_UINT8                  indexNum;

    /* 获取本地保存的用户索引 */
    rslt        = AT_OK;
    length      = 0;
    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    qryCnf      = (BBIC_CAL_PLL_QRY_IND_STRU *)msg;

    AT_PR_LOGH("At_RcvBbicPllStatusCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("At_RcvBbicPllStatusCnf: ulIndex err !");
        return VOS_ERR;
    }

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPLLSTATUS_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FPLLSTATUS_SET)) {
        AT_ERR_LOG("At_RcvBbicPllStatusCnf: CmdCurrentOpt err !");
        return VOS_ERR;
    }

    if (qryCnf->stPara.uwErrorCode != MT_OK) {
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_FPLLSTATUS_QRY) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                qryCnf->stPara.uwTxPllStatus, qryCnf->stPara.uwRxPllStatus);
        } else {
            pllStatus = (atMtInfoCtx->atInfo.antType == AT_MT_ANT_TYPE_TX) ? (qryCnf->stPara.uwTxPllStatus) :
                                                                             qryCnf->stPara.uwRxPllStatus;

            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, pllStatus);
        }
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_SndBbicRssiReq(VOS_VOID)
{
    AT_BBIC_CAL_MT_RX_RSSI_REQ *msg = VOS_NULL_PTR;
    VOS_UINT32                  length;
#if (FEATURE_LTEV == FEATURE_ON)
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
#endif

    /* 申请BBIC_CAL_RF_DEBUG_RSSI_REQ_STRU消息 */
    length = sizeof(AT_BBIC_CAL_MT_RX_RSSI_REQ) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_BBIC_CAL_MT_RX_RSSI_REQ *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SndBbicRssiReq: alloc msg fail!");
        return AT_FRSSI_OTHER_ERR;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_CAL_MT_RX_RSSI_REQ);
#if (FEATURE_LTEV == FEATURE_ON)
    atMtInfoCtx = AT_GetMtInfoCtx();
    if (atMtInfoCtx->atInfo.ratMode == AT_RAT_MODE_LTEV) {
        msg->stComponentID.uwComponentType = AT_COMPONENTTYPE_RAT_LTEV;
    }
#endif
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SndBbicRssiReq: Send msg fail!");
        return AT_FRSSI_OTHER_ERR;
    }

    AT_PR_LOGH("AT_SndBbicRssiReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 At_RcvBbicRssiInd(struct MsgCB *msg)
{
    BBIC_CAL_AT_MT_RX_RSSI_IND *rssiIndMsg  = VOS_NULL_PTR;
    AT_MT_Info                 *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                  rslt;
    VOS_INT32                   rssi;
    VOS_UINT16                  length;
    VOS_UINT8                   indexNum;

    /* 获取本地保存的用户索引 */
    rssi        = 0;
    rslt        = AT_OK;
    length      = 0;
    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    rssiIndMsg  = (BBIC_CAL_AT_MT_RX_RSSI_IND *)msg;

    AT_PR_LOGH("At_RcvBbicRssiInd Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("At_RcvBbicRssiInd: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_QUERY_RSSI) {
        AT_ERR_LOG("At_RcvBbicRssiInd: CmdCurrentOpt Not Query Rssi !");
        return VOS_ERR;
    }

    /* MIMO场景，1，3，5，7取ashwRssi[0]， 2，4，6，8取ashwRssi[1]，余2是为了区分奇偶 */
    if ((atMtInfoCtx->bbicInfo.rxAntType == AT_ANT_TYPE_PRI) ||
        ((atMtInfoCtx->atInfo.rxMimoAntNum % 2 == 1) && (atMtInfoCtx->bbicInfo.rxAntType == AT_ANT_TYPE_MIMO))) {
        /* 由于RSSI测量值单位0.125dBm，为了消除浮点数*1000. */
        rssi = (VOS_INT32)rssiIndMsg->mtRxIndPara.rssi[0] * AT_DSP_RSSI_VALUE_MUL_THOUSAND;
    } else {
        /* 由于RSSI测量值单位0.125dBm，为了消除浮点数*1000. */
        rssi = (VOS_INT32)rssiIndMsg->mtRxIndPara.rssi[1] * AT_DSP_RSSI_VALUE_MUL_THOUSAND;
    }

    if (rssiIndMsg->mtRxIndPara.errorCode != MT_OK) {
        AT_ERR_LOG("At_RcvBbicRssiIndProc err");
        rslt = AT_ERROR;
    } else {
        /*
         * 读取的RSSI值，采用正值上报，精确到0.01dBm定制值信息。如果当前的RSSI
         * 值为-85.1dBm，返回值为8510. 由于之前乘1000，所以精确到0.01dBm这里要除10
         */
        if (rssi < 0) {
            rssi = (-1 * rssi) / 100;
        } else {
            rssi = rssi / 100;
        }

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, rssi);

        g_atSendDataBuff.bufLen = length;
        rslt                    = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_SndBbicCalSetDpdtReq(BBIC_DPDT_OPERTYPE_ENUM_UINT16 operType, VOS_UINT32 value, VOS_UINT32 workType)
{
    BBIC_CAL_DPDT_REQ_STRU *msg         = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32              length;

    atMtInfoCtx = AT_GetMtInfoCtx();
    /* 申请BBIC_CAL_DPDT_REQ_STRU消息 */
    length = sizeof(BBIC_CAL_DPDT_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    msg    = (BBIC_CAL_DPDT_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("At_SndBbicCalDpdtReq: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_DPDT_REQ);

    msg->stPara.enRatMode        = atMtInfoCtx->bbicInfo.dpdtRatMode;
    msg->stPara.enOperType       = operType;
    msg->stPara.uwValue          = value;
    msg->stPara.uhwModemWorkType = (VOS_UINT16)workType;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("At_SndBbicCalDpdtReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("AT_SndBbicCalSetDpdtReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvBbicCalSetDpdtCnf(struct MsgCB *msg)
{
    BBIC_CAL_DPDT_IND_STRU *dpdtCnf     = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32              rslt;
    VOS_UINT8               indexNum = 0;

    rslt        = AT_ERROR;
    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 获取本地保存的用户索引 */
    indexNum = atMtInfoCtx->atInfo.indexNum;
    dpdtCnf  = (BBIC_CAL_DPDT_IND_STRU *)msg;

    AT_PR_LOGH("AT_RcvBbicCalSetDpdtCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_RcvBbicCalSetDpdtCnf: ulIndex err !");
        return VOS_ERR;
    }

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPDT_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPDTQRY_SET)) {
        AT_ERR_LOG("AT_RcvBbicCalSetDpdtCnf: CmdCurrentOpt is not AT_CMD_DPDT_SET or AT_CMD_DPDTQRY_SET!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (dpdtCnf->stPara.uwErrorCode != MT_OK) {
        AT_ERR_LOG1("AT_RcvBbicCalSetDpdtCnf: set dpdt error, ErrorCode is ", dpdtCnf->stPara.uwErrorCode);
        rslt = AT_ERROR;
    } else {
        /* 当操作类型状态为Get时，上报查询Dpdt结果 */
        if (dpdtCnf->stPara.unOperType == BBIC_DPDT_OPERTYPE_GET) {
            rslt = AT_OK;
            g_atSendDataBuff.bufLen =
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                    dpdtCnf->stPara.uwValue);
        }
        /* 当操作类型状态为Set时，返回设置成功 */
        else {
            rslt                    = AT_OK;
            g_atSendDataBuff.bufLen = 0;
        }
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_SndBbicCalQryFtemprptReq(INT16 channelNum)
{
    BBIC_CAL_TEMP_QRY_REQ_STRU *msg         = VOS_NULL_PTR;
    AT_MT_Info                 *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                  length;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 申请BBIC_CAL_TEMP_QRY_REQ_STRU消息 */
    length = sizeof(BBIC_CAL_TEMP_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    msg    = (BBIC_CAL_TEMP_QRY_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SndBbicCalQryFtemprptReq: alloc msg fail!");
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(msg);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_TEMP_QRY_REQ);

    msg->stPara.enChannelType = atMtInfoCtx->bbicInfo.currentChannelType;
    msg->stPara.hwChannelNum  = channelNum;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SndBbicCalQryFtemprptReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("AT_SndBbicCalQryFtemprptReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvBbicCalQryFtemprptCnf(struct MsgCB *msg)
{
    BBIC_CAL_TEMP_QRY_IND_STRU *ftemprptCnf = VOS_NULL_PTR;
    AT_MT_Info                 *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                  rslt;
    VOS_UINT8                   indexNum;

    rslt        = AT_ERROR;
    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 获取本地保存的用户索引 */
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    ftemprptCnf = (BBIC_CAL_TEMP_QRY_IND_STRU *)msg;

    AT_PR_LOGH("AT_RcvBbicCalQryFtemprptCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_RcvBbicCalQryFtemprptCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FTEMPRPT_QRY) {
        AT_ERR_LOG("AT_RcvBbicCalQryFtemprptCnf: CmdCurrentOpt is not AT_CMD_OPT_READ_CMD!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (ftemprptCnf->stPara.uwErrorCode != MT_OK) {
        rslt = AT_ERROR;
        AT_ERR_LOG1("AT_RcvBbicCalQryFtemprptCnf: qry Ftemprpt error, ErrorCode is ", ftemprptCnf->stPara.uwErrorCode);
        g_atSendDataBuff.bufLen = 0;
    } else {
        rslt = AT_OK;
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                ftemprptCnf->stPara.enChannelType, ftemprptCnf->stPara.hwChannelNum, ftemprptCnf->stPara.wTemperature);
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 At_SndDcxoReq(VOS_VOID)
{
    BBIC_CAL_DCXO_REQ_STRU *dcxoReq     = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;

    /* 分配消息空间 */
    dcxoReq = (BBIC_CAL_DCXO_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                     sizeof(BBIC_CAL_DCXO_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (dcxoReq == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(dcxoReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(dcxoReq, DSP_PID_BBA_CAL, ID_AT_BBIC_DCXO_REQ);

    dcxoReq->stPara.enSetType = atMtInfoCtx->bbicInfo.dcxoTempCompEnableFlg;
    dcxoReq->stPara.txArfcn   = atMtInfoCtx->atInfo.bandArfcn.ulChanNo;
    dcxoReq->stPara.usBand    = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, dcxoReq) != VOS_OK) {
        return VOS_FALSE;
    }

    AT_PR_LOGH("At_SndDcxoReq Exit");

    return VOS_TRUE;
}


VOS_UINT32 At_RcvBbicCalDcxoCnf(struct MsgCB *msg)
{
    BBIC_CAL_DCXO_IND_STRU *dcxoInd     = VOS_NULL_PTR;
    AT_MT_Info             *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32              rslt;
    VOS_UINT8               indexNum;

    atMtInfoCtx = AT_GetMtInfoCtx();
    /* 获取本地保存的用户索引 */
    indexNum = atMtInfoCtx->atInfo.indexNum;
    dcxoInd  = (BBIC_CAL_DCXO_IND_STRU *)msg;
    rslt     = AT_OK;

    AT_PR_LOGH("At_RcvBbicCalDcxoCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("At_RcvBbicCalDcxoCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DCXOTEMPCOMP_SET) {
        AT_ERR_LOG("At_RcvBbicCalDcxoCnf: CmdCurrentOpt is not AT_CMD_DCXOTEMPCOMP_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (dcxoInd->ulErrorCode != MT_OK) {
        rslt = AT_ERROR;
        AT_ERR_LOG1("At_RcvBbicCalDcxoCnf: ErrorCode is ", dcxoInd->ulErrorCode);
        g_atSendDataBuff.bufLen = 0;
    } else {
        rslt                    = AT_OK;
        g_atSendDataBuff.bufLen = 0;
        atMtInfoCtx->atInfo.dcxoTempCompEnableFlg =
            (AT_DCXOTEMPCOMP_EnableUint8)atMtInfoCtx->bbicInfo.dcxoTempCompEnableFlg;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 At_SndUeCbtRfIcMemTestReq(VOS_VOID)
{
    AT_UECBT_RficMemTestReqStru *testReq = VOS_NULL_PTR;

    /* 分配消息空间 */
    testReq = (AT_UECBT_RficMemTestReqStru *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                          sizeof(AT_UECBT_RficMemTestReqStru) - VOS_MSG_HEAD_LENGTH);
    if (testReq == VOS_NULL_PTR) {
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(testReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(testReq, CCPU_PID_PAM_MFG, ID_AT_UECBT_RFIC_MEM_TEST_REQ);

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, testReq) != VOS_OK) {
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndCbtRfIcMemTestNtf Exit");

    return AT_SUCCESS;
}


VOS_UINT32 At_ProcUeCbtRfIcMemTestCnf(struct MsgCB *msg)
{
    UECBT_AT_RficMemTestCnfStru *rficTestResult = VOS_NULL_PTR;
    AT_MT_Info                  *atMtInfoCtx    = VOS_NULL_PTR;

    AT_PR_LOGH("At_ProcUeCbtRfIcMemTestCnf Enter");

    atMtInfoCtx    = AT_GetMtInfoCtx();
    rficTestResult = (UECBT_AT_RficMemTestCnfStru *)msg;

    if (rficTestResult->errorCode == MT_OK) {
        atMtInfoCtx->atInfo.rficTestResult = AT_RFIC_MEM_TEST_PASS;
    } else {
        atMtInfoCtx->atInfo.rficTestResult = rficTestResult->errorCode;
    }

    return VOS_OK;
}


VOS_UINT32 At_SndDspIdleSerdesRtReq(VOS_VOID)
{
    AT_PHY_SerdesAgingTestReq *testReq = VOS_NULL_PTR;

    /* 分配消息空间 */
    testReq = (AT_PHY_SerdesAgingTestReq *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                        sizeof(AT_PHY_SerdesAgingTestReq) - VOS_MSG_HEAD_LENGTH);
    if (testReq == VOS_NULL_PTR) {
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(testReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(testReq, I0_PHY_PID_IDLE, ID_AT_PHY_SERDES_AGING_TEST_REQ);

    testReq->testNum     = g_atParaList[AT_FSERDESRT_TEST_NUM_INDEX].paraValue;
    testReq->syncLen     = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_SYNC_LEN_INDEX].paraValue);
    testReq->burstTime   = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_BURST_TIME_INDEX].paraValue);
    testReq->stallTime   = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_STALL_TIME_INDEX].paraValue);
    testReq->dlSyncLen   = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_DL_SYNC_LEN_INDEX].paraValue);
    testReq->dlBurstTime = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_DL_BURST_TIME_INDEX].paraValue);
    testReq->dlStallTime = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_DL_STALL_TIME_INDEX].paraValue);

    if (g_atParaList[AT_FSERDESRT_LS_TEST_INDEX].paraLen != 0) {
        testReq->lsTest = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_LS_TEST_INDEX].paraValue);
    }

    if (g_atParaList[AT_FSERDESRT_FWD_CLK_AMP_INDEX].paraLen != 0) {
        testReq->fwdClkAmp = (VOS_UINT16)(g_atParaList[AT_FSERDESRT_FWD_CLK_AMP_INDEX].paraValue);
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, testReq) != VOS_OK) {
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndPhySerDesrtTestReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 At_ProcDspIdleSerdesRtCnf(struct MsgCB *msg)
{
    PHY_AT_SerdesAgingTestCnf *serdesTestResult = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx      = VOS_NULL_PTR;

    AT_PR_LOGH("At_ProcPhySerDesrtTestCnf Enter");

    atMtInfoCtx      = AT_GetMtInfoCtx();
    serdesTestResult = (PHY_AT_SerdesAgingTestCnf *)msg;

    if (serdesTestResult->failNum == MT_OK) {
        atMtInfoCtx->rserTestResult = (VOS_INT32)AT_SERDES_TEST_PASS;
    } else {
        atMtInfoCtx->rserTestResult = serdesTestResult->failNum;
    }

    return VOS_OK;
}

VOS_UINT32 At_SndDspIdleSltTestReq(VOS_VOID)
{
    AT_PHY_SltTestReq *testReq = VOS_NULL_PTR;

    /* 分配消息空间 */
    testReq = (AT_PHY_SltTestReq *)PS_ALLOC_MSG(WUEPS_PID_AT, sizeof(AT_PHY_SltTestReq) - VOS_MSG_HEAD_LENGTH);

    if (testReq == VOS_NULL_PTR) {
        AT_ERR_LOG("At_SndDspIdleSltTestReq: alloc msg fail !");
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(testReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(testReq, I0_PHY_PID_IDLE, ID_AT_PHY_SLT_TEST_REQ);

    if (g_atParaList[0].paraValue == AT_SET_SLT_SERDES_TEST) {
        testReq->testType = SLT_SERDES_TSET;
    }

    if (g_atParaList[0].paraValue == AT_SET_SLT_ALINK_TEST) {
        testReq->testType = SLT_ALINK_TEST;
    }

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, testReq) != VOS_OK) {
        AT_ERR_LOG("At_SndDspIdleSltTestReq: Send msg fail !");
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndDspIdleSltTestReq Exit");

    return AT_SUCCESS;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_ProcDspIdleSltTestCnf(struct MsgCB *msg)
{
    PHY_AT_SltTestCnf *sltTestResult = VOS_NULL_PTR;
    VOS_UINT32         rslt;
    VOS_UINT8          indexNum;
    AT_MT_Info        *atMtInfoCtx = VOS_NULL_PTR;

    /* 获取本地保存的用户索引 */
    rslt          = AT_OK;
    atMtInfoCtx   = AT_GetMtInfoCtx();
    indexNum      = atMtInfoCtx->atInfo.indexNum;
    sltTestResult = (PHY_AT_SltTestCnf *)msg;

    AT_PR_LOGH("AT_ProcDspIdleSltTestCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_ProcDspIdleSltTestCnf: ulIndex err !");
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SLT_TEST_SET) {
        return VOS_ERR;
    }
    if (sltTestResult->result == MT_OK) {
        rslt = AT_OK;
    } else {
        rslt = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}
#endif


VOS_UINT32 At_SndDspIdleSerdesTestAsyncReq(VOS_VOID)
{
    AT_PHY_SerdesTestReq *testReq     = VOS_NULL_PTR;
    AT_MT_Info           *atMtInfoCtx = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 分配消息空间 */
    testReq = (AT_PHY_SerdesTestReq *)PS_ALLOC_MSG(WUEPS_PID_AT, sizeof(AT_PHY_SerdesTestReq) - VOS_MSG_HEAD_LENGTH);
    if (testReq == VOS_NULL_PTR) {
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(testReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(testReq, I0_PHY_PID_IDLE, ID_AT_PHY_SERDES_TEST_ASYNC_REQ);

    testReq->type                                       = atMtInfoCtx->serdesTestAsyncInfo.cmdType;
    testReq->serdesTestPara.atSerdesTestParaList.para1  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA1].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para2  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA2].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para3  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA3].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para4  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA4].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para5  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA5].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para6  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA6].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para7  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA7].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para8  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA8].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para9  =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA9].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para10 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA10].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para11 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA11].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para12 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA12].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para13 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA13].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para14 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA14].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para15 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA15].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para16 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA16].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para17 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA17].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para18 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA18].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para19 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA19].paraValue;
    testReq->serdesTestPara.atSerdesTestParaList.para20 =
        g_atParaList[AT_PHY_SERDES_TEST_PARALIST_PARA20].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, testReq) != VOS_OK) {
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndDspIdleSerdesTestAsyncReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 At_ProcDspIdleSerdesAsyncTestCnf(struct MsgCB *rcvMsg)
{
    PHY_AT_SerdesTestCnf *serdesTestResult = VOS_NULL_PTR;
    AT_MT_Info           *atMtInfoCtx      = VOS_NULL_PTR;

    atMtInfoCtx = AT_GetMtInfoCtx();

    AT_PR_LOGH("At_ProcDspIdleSerdesAsyncTestCnf Enter");

    serdesTestResult                        = (PHY_AT_SerdesTestCnf *)rcvMsg;
    atMtInfoCtx->serdesTestAsyncInfo.result = serdesTestResult->result;

    /* 主动上报 */
    if (atMtInfoCtx->serdesTestAsyncInfo.rptFlg == 1) {
        g_atSendDataBuff.bufLen = 0;
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s: %d%s", g_atCrLf, "^SERDESTESTASYNC",
            atMtInfoCtx->serdesTestAsyncInfo.result, g_atCrLf);

        At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}


VOS_UINT32 At_SndBbicCalSetTrxTasReq(VOS_UINT16 trxTasValue)
{
    BBIC_CAL_SetTrxTasReqStru *msg         = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                 length;

    atMtInfoCtx = AT_GetMtInfoCtx();
    length      = sizeof(BBIC_CAL_SetTrxTasReqStru) - VOS_MSG_HEAD_LENGTH;
    msg         = (BBIC_CAL_SetTrxTasReqStru *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("At_SndBbicCalSetTrxTasReq: alloc msg fail!");
        return AT_FAILURE;
    }

    AT_MT_CLR_MSG_ENTITY(msg);
    AT_CFG_MT_MSG_HDR(msg, DSP_PID_BBA_CAL, ID_AT_BBIC_TRX_TAS_REQ);

    msg->data.band        = atMtInfoCtx->bbicInfo.dspBandFreq.dspBand;
    msg->data.ratMode     = atMtInfoCtx->bbicInfo.trxTasRatMode;
    msg->data.trxTasValue = trxTasValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("At_SndBbicCalSetTrxTasReq: Send msg fail!");
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndBbicCalSetTrxTasReq Exit");

    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvBbicCalSetTrxTasCnf(struct MsgCB *msg)
{
    BBIC_CAL_SetTrxTasCnfStru *trxTasCnf   = VOS_NULL_PTR;
    AT_MT_Info                *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                 rslt;
    VOS_UINT8                  indexNum;

    rslt        = AT_ERROR;
    atMtInfoCtx = AT_GetMtInfoCtx();

    /* 获取本地保存的用户索引 */
    indexNum  = atMtInfoCtx->atInfo.indexNum;
    trxTasCnf = (BBIC_CAL_SetTrxTasCnfStru *)msg;

    AT_PR_LOGH("AT_RcvBbicCalSetTrxTasCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_RcvBbicCalSetTrxTasCnf: ulIndex err !");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TRX_TAS_SET) {
        AT_ERR_LOG("AT_RcvBbicCalSetTrxTasCnf: CmdCurrentOpt is not AT_CMD_TRX_TAS_SET!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (trxTasCnf->data.errorCode != MT_OK) {
        AT_ERR_LOG1("AT_RcvBbicCalSetTrxTasCnf: set trxtas error, ErrorCode is ", trxTasCnf->data.errorCode);
        rslt = AT_ERROR;
    } else {
        rslt                    = AT_OK;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, rslt);
    return VOS_OK;
}


VOS_UINT32 At_SndUeCbtRfIcIdExQryReq(VOS_VOID)
{
    AT_UECBT_DieId_Query_Req *qryReq = VOS_NULL_PTR;

    /* 分配消息空间 */
    qryReq = (AT_UECBT_DieId_Query_Req *)PS_ALLOC_MSG(WUEPS_PID_AT,
                                                      sizeof(AT_UECBT_DieId_Query_Req) - VOS_MSG_HEAD_LENGTH);
    if (qryReq == VOS_NULL_PTR) {
        return AT_FAILURE;
    }

    /* 初始化 */
    AT_MT_CLR_MSG_ENTITY(qryReq);

    /* 填写消息头 */
    AT_CFG_MT_MSG_HDR(qryReq, CCPU_PID_PAM_MFG, ID_AT_UECBT_RFIC_DIE_IE_QUERY_REQ);

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, qryReq) != VOS_OK) {
        return AT_FAILURE;
    }

    AT_PR_LOGH("At_SndUeCbtRfIcIdExQryReq Exit");
    return AT_SUCCESS;
}


VOS_UINT32 At_RcvUeCbtRfIcIdExQryCnf(struct MsgCB *msg)
{
    UECBT_AT_DieId_Query_Ind *rcvMsg      = VOS_NULL_PTR;
    AT_MT_Info               *atMtInfoCtx = VOS_NULL_PTR;
    VOS_UINT32                i;
    VOS_UINT32                j;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum;
    VOS_UINT8                 dataSize;
    VOS_UINT8                 atCrLfLen;

    /* 初始化局部变量 */
    length      = 0;
    dataSize    = 0;
    atMtInfoCtx = AT_GetMtInfoCtx();
    indexNum    = atMtInfoCtx->atInfo.indexNum;
    rcvMsg      = (UECBT_AT_DieId_Query_Ind *)msg;

    AT_PR_LOGH("At_RcvUeCbtRfIcIdExQryCnf Enter");

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("At_RcvUeCbtRfIcIdExQryCnf: indexNum err!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RFIC_DIE_ID_EX_QRY) {
        AT_WARN_LOG("AT_RcvMtaRficDieIDQryCnf: OPTION ERR!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((rcvMsg->errorCode != MT_OK) || (rcvMsg->chipNum > DIE_ID_QUERY_CHIP_MAX_NUM)) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    } else {
        atCrLfLen = (VOS_UINT8)strlen((VOS_CHAR *)g_atCrLf);
        for (i = 0; i < rcvMsg->chipNum; i++) {
            /* 打印命令名和序号,IC type */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"", g_parseContext[indexNum].cmdElement->cmdName,
                i, rcvMsg->dieIdInfo[i].chipType);
            /* RFIC ID 使用低八位数据 */
            dataSize = (VOS_UINT8)TAF_MIN((rcvMsg->dieIdInfo[i].infoSize), DIE_ID_MAX_LEN_BYTE);
            for (j = 0; j < dataSize; j++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%02x", rcvMsg->dieIdInfo[i].data[j]);
            }

            /* 打印一个换行 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);
        }

        /* 将最后一个换行符去掉 */
        g_atSendDataBuff.bufLen = length - atCrLfLen;
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}

#endif

