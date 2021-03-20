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

#include "at_ltev_msg_proc.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "AtDataProc.h"
#include "AtCtx.h"

#if (FEATURE_LTEV == FEATURE_ON)
#if (FEATURE_LTEV_WL == FEATURE_ON)
#include "at_mta_interface.h"
#endif
#endif


#define THIS_FILE_ID PS_FILE_ID_AT_LTEV_MSG_PROC_C

#define AT_OTHERS_FILE_TYPE 0
#define AT_FILE_TYPE_ELABEL 1
#define AT_FILE_TYPE_INDEX 2

#if (FEATURE_LTEV == FEATURE_ON)

/* AT模块处理来自VRRC消息函数对应表 */
const AT_VRRC_ProcMsgTbl g_atProcMsgFromVrrcTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { VRRC_MSG_GNSS_TEST_START_CNF, 0, AT_RcvGnssTestStartCnf },
    { VRRC_MSG_GNSS_INFO_QRY_CNF, 0, AT_RcvGnssInfoQryCnf },
    { VRRC_MSG_RSU_SLINFO_QRY_CNF, 0, AT_RcvVrrcMsgQrySLInfoCnfProc },
    { VRRC_MSG_RSU_SLINFO_SET_CNF, 0, AT_RcvVrrcMsgSetSLInfoCnfProc },
    { VRRC_MSG_RSU_PHYR_SET_CNF, 0, AT_RcvRsuPhyrSetCnf },
    { VRRC_MSG_RSU_PHYR_QRY_CNF, 0, AT_RcvRsuPhyrQryCnf },
    { VRRC_MSG_RPPCFG_SET_CNF, 0, AT_RcvVrrcMsgSetRppCfgCnfProc },
    { VRRC_MSG_RPPCFG_QRY_CNF, 0, AT_RcvVrrcMsgQryRppCfgCnfProc },
    { VRRC_MSG_RSU_VPHYSTAT_QRY_CNF, 0, AT_RcvRsuVphyStatQryCnf },
    { VRRC_MSG_RSU_VPHYSTAT_CLR_SET_CNF, 0, AT_RcvRsuVphyStatClrSetCnf },
    { VRRC_MSG_RSU_VSNRRSRP_QRY_CNF, 0, AT_RcvRsuVsnrRsrpQryCnf },
    { VRRC_MSG_GNSS_INFO_GET_CNF, 0, AT_RcvGnssDebugInfoGetCnf },
    { VRRC_MSG_RSU_RSSI_QRY_CNF, 0, AT_RcvRsuV2xRssiQryCnf },
    { VRRC_AT_SYNC_SOURCE_QRY_CNF, 0, AT_RcvSyncSourceQryCnf },
    { VRRC_AT_SYNC_SOURCE_RPT_SET_CNF, 0, AT_RcvSyncSourceRptSetCnf },
    { VRRC_AT_SYNC_SOURCE_RPT_QRY_CNF, 0, AT_RcvSyncSourceRptQryCnf },
    { VRRC_AT_SYNC_SOURCE_RPT_IND, 0, AT_RcvSyncSourceRptInd },
    { VRRC_AT_SYNC_MODE_QRY_CNF, 0, AT_RcvSyncModeQryCnf },
    { VRRC_AT_SYNC_MODE_SET_CNF, 0, AT_RcvSyncModeSetCnf },
    { VRRC_AT_SET_TX_POWER_CNF, 0, AT_RcvVrrcMsgSetTxPowerCnfProc },
};

#if (FEATURE_LTEV_WL == FEATURE_ON)
/* AT 模块处理来自Vsync消息函数对应表 */
const AT_VSYNC_ProcMsgTbl g_atProcMsgFromVsyncTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { VRRC_MSG_RSU_SYNCST_QRY_CNF, 0, AT_RcvRsuSyncStatQryCnf },
    { VRRC_MSG_RSU_GNSSSYNCST_QRY_CNF, 0, AT_RcvRsuGnssSyncStatQryCnf },
    { VRRC_MSG_PC5SYNC_SET_CNF, 0, AT_RcvPc5SyncSetCnf },
    { VRRC_MSG_PC5SYNC_QRY_CNF, 0, AT_RcvPc5SyncQryCnf },
};
#endif

/* AT 模块处理来自Vmac消息函数对应表 */
const AT_VMAC_ProcMsgTbl g_atProcMsgFromVmacTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { VMAC_MSG_RSU_VRSSI_QRY_CNF, 0, AT_RcvVrssiQryCnf },
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { VMAC_MSG_RSU_MCG_CFG_SET_CNF, 0, AT_RcvRsuMcsCfgCnf },
#endif
    { VMAC_AT_RSSI_RPT_IND, 0, AT_RcvRssiRptInd },
    { VMAC_AT_CBR_QRY_CNF, 0, AT_RcvVmacCbrQryCnf },
    { VMAC_AT_CBR_RPT_IND, 0, AT_RcvCbrRptInd },
};

/* AT 模块处理来自Vpdcp消息函数对应表 */
const AT_VPDCP_ProcMsgTbl g_atProcMsgFromVpdcpTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { VPDCP_MSG_RSU_PTRRPT_QRY_CNF, 0, AT_RcvPtrRptQryCnf },
    { VPDCP_MSG_RSU_PTRRPT_SET_CNF, 0, AT_RcvPtrRptSetCnf },
};

/* AT处理来自VTC消息函数对应表 */
const AT_VTC_ProcMsgTbl g_atProcMsgFromVtcTab[] = {
    { VTC_AT_TEST_MODE_ACTIVE_STATE_SET_CNF, AT_RcvTestModeActiveStateSetCnf },
    { VTC_AT_TEST_MODE_ACTIVE_STATE_QRY_CNF, AT_RcvTestModeActiveStateQryCnf },
    { VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_SET_CNF, AT_RcvTestModeECloseStateSetCnf },
    { VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_CNF, AT_RcvTestModeECloseStateQryCnf },
    { VTC_AT_SIDELINK_PACKET_COUNTER_QRY_CNF, AT_RcvSidelinkPacketCounterQryCnf },
    { VTC_AT_CBR_QRY_CNF, AT_RcvVtcCbrQryCnf },
    { VTC_AT_RESET_UTC_TIME_SET_CNF, AT_RcvResetUtcTimeSetCnf },
    { VTC_AT_SENDING_DATA_ACTION_SET_CNF, AT_RcvSendingDataActionSetCnf },
    { VTC_AT_SENDING_DATA_ACTION_QRY_CNF, AT_RcvSendingDataActionQryCnf },
    { VTC_AT_PC5_SEND_DATA_CNF, AT_RcvPc5PktSndCnf },
    { VTC_AT_LAYER_TWO_ID_SET_CNF, AT_RcvTestModeModifyL2IdSetCnf },
    { VTC_AT_LAYER_TWO_ID_QRY_CNF, AT_RcvTestModeModifyL2IdQryCnf },
};

/* 将VTC结果码转化为AT返回值 */
const AT_ConvertVtcResultCodeTbl g_atChgVtcResultCodeTbl[] = {
    { VTC_AT_RESULT_OK, AT_OK },
    { VTC_AT_RESULT_ERR, AT_ERROR },
    { VTC_AT_RESULT_OPERATION_NOT_ALLOW, AT_CME_OPERATION_NOT_ALLOWED },
    { VTC_AT_RESULT_TIMEOUT, AT_DEVICE_TIMEOUT_ERR },
};

/* 将VRRC结果码转化为AT返回值 */
const AT_ConvertVrrcResultCodeTbl g_atChgVrrcResultCodeTbl[] = {
    { VRRC_AT_RESULT_OK, AT_OK },
    { VRRC_AT_RESULT_ERR, AT_ERROR },
    { VRRC_AT_RESULT_NO_SYNC_SOURCE, AT_CME_NOT_FOUND_SYNC_SOURCE },
};


VOS_VOID AT_ProcMsgFromVrrc(struct MsgCB *msg)
{
    MN_AT_IndEvt *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32    i;
    VOS_UINT32    msgCount;
    VOS_UINT16    msgName;
    VOS_UINT32    result;

    msgCount = AT_ARRAY_SIZE(g_atProcMsgFromVrrcTab);
    rcvMsg   = (MN_AT_IndEvt *)msg;
    msgName  = rcvMsg->msgName;

    for (i = 0; i < msgCount; i++) {
        if (g_atProcMsgFromVrrcTab[i].msgName == msgName) {
            result = g_atProcMsgFromVrrcTab[i].procMsgFunc(rcvMsg);
            if (result == VOS_ERR) {
                AT_ERR_LOG("AT_ProcMsgFromVrrc: Msg Proc Err!");
            }
            return;
        }
    }
    if (msgCount == i) {
        AT_ERR_LOG("AT_ProcMsgFromVrrc: Msg Id is invalid!");
    }
}


VOS_VOID AT_ProcMsgFromVmac(struct MsgCB *msg)
{
    MN_AT_IndEvt *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32    i;
    VOS_UINT32    msgCount;
    VOS_UINT16    msgName;
    VOS_UINT32    result;

    msgCount = AT_ARRAY_SIZE(g_atProcMsgFromVmacTab);
    rcvMsg   = (MN_AT_IndEvt *)msg;
    msgName  = rcvMsg->msgName;

    for (i = 0; i < msgCount; i++) {
        if (g_atProcMsgFromVmacTab[i].msgName == msgName) {
            result = g_atProcMsgFromVmacTab[i].procMsgFunc(rcvMsg);
            if (result == VOS_ERR) {
                AT_ERR_LOG("AT_ProcMsgFromVmac: Msg Proc Err!");
            }
            return;
        }
    }
    if (msgCount == i) {
        AT_ERR_LOG("AT_ProcMsgFromVmac: Msg Id is invalid!");
    }
}


VOS_VOID AT_ProcMsgFromVpdcp(struct MsgCB *msg)
{
    MN_AT_IndEvt *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32    i;
    VOS_UINT32    msgCnt;
    VOS_UINT16    msgName;
    VOS_UINT32    rst;

    msgCnt  = AT_ARRAY_SIZE(g_atProcMsgFromVpdcpTab);
    rcvMsg  = (MN_AT_IndEvt *)msg;
    msgName = rcvMsg->msgName;

    /* g_astAtProcMsgFromVpdcpTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromVpdcpTab[i].msgName == msgName) {
            rst = g_atProcMsgFromVpdcpTab[i].procMsgFunc(rcvMsg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcMsgFromVpdcp: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcMsgFromVpdcp: Msg Id is invalid!");
    }

    return;
}


VOS_VOID AT_ProcMsgFromVtc(struct MsgCB *msg)
{
    VTC_AT_MsgCnf       *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32           i;
    VOS_UINT32           msgCount;
    AT_VTC_MsgTypeUint32 msgName;
    VOS_UINT32           result;

    msgCount = AT_ARRAY_SIZE(g_atProcMsgFromVtcTab);
    rcvMsg   = (VTC_AT_MsgCnf *)msg;
    msgName  = rcvMsg->msgName;

    for (i = 0; i < msgCount; i++) {
        if (g_atProcMsgFromVtcTab[i].msgName == msgName) {
            result = g_atProcMsgFromVtcTab[i].procMsgFunc(rcvMsg);
            if (result == VOS_ERR) {
                AT_ERR_LOG("AT_ProcMsgFromVtc: Msg Proc Err!");
            }
            return;
        }
    }
    if (msgCount == i) {
        AT_ERR_LOG("AT_ProcMsgFromVtc: Msg Id is invalid!");
    }
}


AT_RreturnCodeUint32 AT_ConvertResultCodeFromVtc(VTC_AT_ResultCodeUint32 vtcResultCode)
{
    AT_RreturnCodeUint32 atReturnCode;
    VOS_UINT32           count;
    VOS_UINT32           i;

    count = AT_ARRAY_SIZE(g_atChgVtcResultCodeTbl);

    for (i = 0; i < count; i++) {
        if (g_atChgVtcResultCodeTbl[i].vtcResultCode == vtcResultCode) {
            atReturnCode = g_atChgVtcResultCodeTbl[i].atReturnCode;
            return atReturnCode;
        }
    }
    return AT_ERROR;
}


AT_RreturnCodeUint32 AT_ConvertResultCodeFromVrrc(VRRC_AT_ResultCodeUint32 vrrcResultCode)
{
    AT_RreturnCodeUint32 atReturnCode;
    VOS_UINT32           count;
    VOS_UINT32           i;

    count = AT_ARRAY_SIZE(g_atChgVrrcResultCodeTbl);

    for (i = 0; i < count; i++) {
        if (g_atChgVrrcResultCodeTbl[i].vrrcResultCode == vrrcResultCode) {
            atReturnCode = g_atChgVrrcResultCodeTbl[i].atReturnCode;
            return atReturnCode;
        }
    }
    return AT_ERROR;
}


VOS_UINT32 AT_RcvTestModeActiveStateSetCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_MsgSetCnf    *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CATM_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_MsgSetCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    At_FormatResultData(indexNum, atReturnCode);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTestModeActiveStateQryCnf(VTC_AT_MsgCnf *msg)
{
    VTC_AT_TestModeActiveStateQryCnf *para = VOS_NULL_PTR;
    AT_RreturnCodeUint32              atReturnCode;
    VOS_UINT8                         indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CATM_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_TestModeActiveStateQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    if (para->para.state == AT_VTC_TEST_MODE_STATE_DEACTIVATED) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CATM: %d", para->para.state);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CATM: %d,%d", para->para.state, para->para.type);
    }
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTestModeECloseStateSetCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_MsgSetCnf    *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCUTLE_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_MsgSetCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    At_FormatResultData(indexNum, atReturnCode);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTestModeECloseStateQryCnf(VTC_AT_MsgCnf *msg)
{
    VTC_AT_TestModeECloseStateQryCnf *para = VOS_NULL_PTR;
    AT_RreturnCodeUint32              atReturnCode;
    VOS_UINT8                         indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCUTLE_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_TestModeECloseStateQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    /* 退出测试模式E状态 */
    if (para->para.status == AT_VTC_TEST_MODE_STATUS_OPEN) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CCUTLE: %d", para->para.status);
    } else {
        /* 数据接收状态 */
        if (para->para.direction == AT_VTC_COMMUNICATION_DIRECTION_RECEIVE) {
            if (para->para.monitorList[AT_CCUTLE_MONITOR_LIST_CONTEXT_MAX_NUM] != '\0') {
                At_FormatResultData(indexNum, AT_ERROR);
                return VOS_OK;
            }
            g_atSendDataBuff.bufLen =
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress, "+CCUTLE: %d,%d,%d,%d,\"%s\"", para->para.status,
                    para->para.direction, para->para.format, para->para.length, para->para.monitorList);
        } else {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "+CCUTLE: %d,%d", para->para.status, para->para.direction);
        }
    }
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvResetUtcTimeSetCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_MsgSetCnf    *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CUTCR_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_MsgSetCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    At_FormatResultData(indexNum, atReturnCode);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSendingDataActionSetCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_MsgSetCnf    *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CV2XDTS_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_MsgSetCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    At_FormatResultData(indexNum, atReturnCode);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSendingDataActionQryCnf(VTC_AT_MsgCnf *msg)
{
    VTC_AT_SendingDataActionQryCnf *para = VOS_NULL_PTR;
    AT_RreturnCodeUint32            atReturnCode;
    VOS_UINT8                       indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CV2XDTS_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_SendingDataActionQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    if (para->para.action == AT_VTC_SENDING_DATA_ACTION_STOP) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CV2XDTS: %d", para->para.action);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CV2XDTS: %d,%d,%d", para->para.action, para->para.dataSize,
            para->para.periodicity);
    }
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSidelinkPacketCounterQryCnf(VTC_AT_MsgCnf *msg)
{
    VTC_AT_SidelinkPacketCounterQryCnf *para = VOS_NULL_PTR;
    AT_RreturnCodeUint32                atReturnCode;
    VOS_UINT8                           indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CUSPCREQ_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_SidelinkPacketCounterQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    /* 每种传输块或者数据包的LayerID下的数量都是用8位表示，这里检查字符串结尾，避免打印异常 */
    if ((para->para.numOfPscchTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM] != '\0') ||
        (para->para.numOfStchPdcpSduPackets[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM] != '\0') ||
        (para->para.numOfPsschTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM] != '\0')) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CUSPCREQ: %d,%d,%d,\"%s\",%d,%d,%d,\"%s\",%d,%d,%d,\"%s\"",
            AT_PSCCH_TRANSPORT_BLOCKS, AT_VTC_PACKET_COUNTER_FORMAT_BINARY, para->para.lenOfPscchTransportBlocks,
            para->para.numOfPscchTransportBlocks, AT_STCH_PDCP_SDU_PACKETS, AT_VTC_PACKET_COUNTER_FORMAT_BINARY,
            para->para.lenOfStchPdcpSduPackets, para->para.numOfStchPdcpSduPackets, AT_PSSCH_TRANSPORT_BLOCKS,
            AT_VTC_PACKET_COUNTER_FORMAT_BINARY, para->para.lenOfPsschTransportBlocks,
            para->para.numOfPsschTransportBlocks);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvVtcCbrQryCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_CbrQryCnf    *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCBRREQ_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_CbrQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    if (para->para.isPscchCbrValid == VOS_TRUE) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CCBRREQ: %d,%d", para->para.psschCbr, para->para.pscchCbr);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "+CCBRREQ: %d", para->para.psschCbr);
    }
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvVmacCbrQryCnf(MN_AT_IndEvt *msg)
{
    VMAC_AT_CbrQryCnf *para     = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBR_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VMAC_AT_CbrQryCnf *)msg->content;
    if (para->result != VOS_OK) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^CBR: %d", para->para.psschCbr);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvCbrRptInd(MN_AT_IndEvt *msg)
{
    VMAC_AT_CbrPara *para     = VOS_NULL_PTR;
    VOS_UINT8        indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    para                    = (VMAC_AT_CbrPara *)msg->content;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CBR: %d%s", g_atCrLf, para->psschCbr, g_atCrLf);
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    return VOS_OK;
}


VOS_UINT32 AT_RcvRssiRptInd(MN_AT_IndEvt *msg)
{
    VMAC_AT_Pc5RssiPara *para     = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    para = (VMAC_AT_Pc5RssiPara *)msg->content;
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^VRSSI: %d%s", g_atCrLf, para->rssi, g_atCrLf);
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncSourceQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_SyncSourceStateQryCnf *para     = VOS_NULL_PTR;
    VOS_UINT8                      indexNum = 0;
    AT_RreturnCodeUint32           atReturnCode;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VSYNCSRC_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VRRC_AT_SyncSourceStateQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVrrc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^VSYNCSRC: %d,%d,%d,%d,%d", para->para.type, para->para.status,
        para->para.earfcn, para->para.slssId, para->para.subSlssId);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncSourceRptSetCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_MsgSetCnf *para     = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VSYNCSRCRPT_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VRRC_AT_MsgSetCnf *)msg->content;
    if (para->result == VOS_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }
    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncSourceRptQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_SyncSourceRptSwitchQryCnf *para     = VOS_NULL_PTR;
    VOS_UINT8                          indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VSYNCSRCRPT_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VRRC_AT_SyncSourceRptSwitchQryCnf *)msg->content;
    if (para->result != VOS_OK) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^VSYNCSRCRPT: %d", para->para.status);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncSourceRptInd(MN_AT_IndEvt *msg)
{
    VRRC_AT_SyncSourceStatePara *para     = VOS_NULL_PTR;
    VOS_UINT8                    indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    para                    = (VRRC_AT_SyncSourceStatePara *)msg->content;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^VSYNCSRC: %d,%d,%d,%d,%d%s", g_atCrLf, para->type, para->status,
        para->earfcn, para->slssId, para->subSlssId, g_atCrLf);
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncModeQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_SyncModeQryCnf *para     = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VSYNCMODE_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VRRC_AT_SyncModeQryCnf *)msg->content;
    if (para->result != VOS_OK) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^VSYNCMODE: %d", para->para.mode);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvSyncModeSetCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_MsgSetCnf *para     = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VSYNCMODE_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VRRC_AT_MsgSetCnf *)msg->content;
    if (para->result == VOS_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }
    return VOS_OK;
}


VOS_UINT32 AT_RcvVrrcMsgSetSLInfoCnfProc(MN_AT_IndEvt *msg)
{
    VRRC_AT_SlinfoQryCnf *rssiQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    /* 初始化消息变量 */
    result     = AT_ERROR;
    rssiQryCnf = (VRRC_AT_SlinfoQryCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetSLInfoCnfProc: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetSLInfoCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_SLINFO_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetSLInfoCnfProc : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (rssiQryCnf->result == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvVrrcMsgQrySLInfoCnfProc(MN_AT_IndEvt *msg)
{
    VRRC_AT_SlinfoQryCnf *rssiQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    /* 初始化消息变量 */
    rssiQryCnf = (VRRC_AT_SlinfoQryCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrrcMsgQrySLInfoCnfProc: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrrcMsgQrySLInfoCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_SLINFO_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvVrrcMsgQrySLInfoCnfProc : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (rssiQryCnf->result == VOS_OK) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d,%d,%d%s", g_atCrLf, g_parseContext[indexNum].cmdElement->cmdName,
            rssiQryCnf->frequencyBand, rssiQryCnf->bandWidth, rssiQryCnf->centralFrequency, g_atCrLf);
        result                  = AT_OK;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuPhyrSetCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_PhyrSetCnf *phyrSetCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexNum = 0;

    /* 初始化消息变量 */
    result     = AT_ERROR;
    phyrSetCnf = (VRRC_AT_PhyrSetCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuPhyrSetCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuPhyrSetCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHYP_SET */
    if (AT_CMD_PHYP_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvRsuPhyrSetCnf : AT_CMD_PHYP_SET.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^PHYP设置物理层参数返回值 */
    if (phyrSetCnf->result == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuPhyrQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_PhyrQryCnf *phyrCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT16          length;
    VOS_UINT8           indexNum = 0;

    /* 初始化 */
    phyrCnf = (VRRC_AT_PhyrQryCnf *)msg->content;
    length  = 0;
    result  = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuPhyrQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuPhyrQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHYP_QRY */
    if (AT_CMD_PHYP_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvRsuPhyrQryCnf : AT_CMD_PHYP_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^PHYR查询命令返回 */
    if (phyrCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuPhyrQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            phyrCnf->maxTxPwr, phyrCnf->minMcs, phyrCnf->maxMcs, phyrCnf->minSubchn, phyrCnf->maxSubchn);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvPtrRptSetCnf(MN_AT_IndEvt *msg)
{
    VPDCP_AT_PtrrptQryCnf *ptrRptCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;

    /* 初始化 */
    ptrRptCnf = (VPDCP_AT_PtrrptQryCnf *)msg->content;
    length    = 0;
    result    = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvPtrRptSetCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvPtrRptSetCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PTRRPT_SET */
    if (AT_CMD_PTRRPT_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvPtrRptSetCnf : AT_CMD_PTRRPT_SET.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ptrRptCnf->pc5TxBytes, ptrRptCnf->pc5RxBytes, ptrRptCnf->pc5TxPkts, ptrRptCnf->pc5RxPkts);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvPtrRptQryCnf(MN_AT_IndEvt *msg)
{
    VPDCP_AT_PtrrptQryCnf *ptrRptCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;

    /* 初始化 */
    ptrRptCnf = (VPDCP_AT_PtrrptQryCnf *)msg->content;
    length    = 0;
    result    = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvPtrRptQryCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvPtrRptQryCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PTRRPT_QRY */
    if (AT_CMD_PTRRPT_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvPtrRptQryCnf : AT_CMD_PTRRPT_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ptrRptCnf->pc5TxBytes, ptrRptCnf->pc5RxBytes, ptrRptCnf->pc5TxPkts, ptrRptCnf->pc5RxPkts);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvVrrcMsgSetRppCfgCnfProc(MN_AT_IndEvt *msg)
{
    VOS_UINT32            result       = AT_ERROR;
    VOS_UINT8             indexNum     = 0;
    VRRC_AT_RppcfgSetCnf *rppCfgSetCnf = VOS_NULL_PTR;

    /* 初始化 */
    rppCfgSetCnf = (VRRC_AT_RppcfgSetCnf *)msg->content; /*lint !e838 */

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetRppCfgCnfProc: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetRppCfgCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_RPPCFG_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetRppCfgCnfProc : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (rppCfgSetCnf->result == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvGnssTestStartCnf(MN_AT_IndEvt *msg)
{
    VOS_UINT32 result;
    VOS_UINT8  indexNum = 0;

    /* 初始化 */
    result = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvGnssTestStartCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvGnssTestStartCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_GNSS_SET */
    if (AT_CMD_GNSS_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvGnssTestStartCnf : AT_CMD_GNSS_SET.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->content[0] == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvGnssInfoQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_GnssQryCnf *gnssQryCnf = VOS_NULL_PTR;
    AT_MTA_FileRdReq    atFileRdReq;
    VOS_UINT32          result;
    VOS_UINT16          length;
    VOS_UINT8           indexNum   = 0;
    errno_t             memResult;
    /* 初始化 */
    gnssQryCnf = (VRRC_AT_GnssQryCnf *)msg->content;

    memResult = memset_s(&atFileRdReq, sizeof(atFileRdReq), 0x00, sizeof(AT_MTA_FileRdReq));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atFileRdReq), sizeof(AT_MTA_FileRdReq));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvGnssInfoQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvGnssInfoQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DATA_QRY */
    if (AT_CMD_DATA_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvGnssInfoQryCnf : AT_CMD_DATA_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^GNSSTEST查询命令返回 */
    if (gnssQryCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvGnssInfoQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)(gnssQryCnf->perPpsStat), (VOS_INT32)(gnssQryCnf->syncStat), (VOS_INT32)(gnssQryCnf->satNum));

        g_atSendDataBuff.bufLen = length;
    }
    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvGnssDebugInfoGetCnf(MN_AT_IndEvt *msg)
{
    AT_VRRC_GnssDebugInfo *gnssInfo = VOS_NULL_PTR;
    VOS_UINT32             result   = AT_OK;
    VOS_UINT16             length   = 0;
    VOS_UINT8              indexNum = 0;
    VOS_UINT32             loop     = 0;

    /* 初始化 */
    gnssInfo = (AT_VRRC_GnssDebugInfo *)msg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvGnssDebugInfoGetCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvGnssDebugInfoGetCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_GNSSINFO_QRY */
    if (AT_CMD_GNSSINFO_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvGnssDebugInfoGetCnf : AT_CMD_GNSSINFO_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^GNSSINFO查询命令返回 */
    if (gnssInfo->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvGnssDebugInfoGetCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;
        if (gnssInfo->satNum == 0) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName,
                gnssInfo->satNum, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%s%s,%s%s%d%s",
                g_parseContext[indexNum].cmdElement->cmdName, g_atCrLf, gnssInfo->longitude, gnssInfo->latitude,
                g_atCrLf, gnssInfo->satNum, g_atCrLf);

            for (loop = 0; loop < gnssInfo->satNum; loop++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "No%d:%d    %d%s", loop,
                    gnssInfo->sysInfo[loop].gnssSystem, gnssInfo->sysInfo[loop].cno, g_atCrLf);
            }
        }
        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_VOID AT_InitBitMap(VOS_UINT8 *bitmap, VOS_UINT8 bitmapLen, VOS_UINT8 *scBitmap, VOS_UINT8 scLen)
{
    VOS_UINT8 i = 0;
    VOS_UINT8 k = 0;
    VOS_UINT8 j = 0;
    VOS_UINT8 b = 0;
    while (i < bitmapLen) {
        if ((k != 0) && (k % AT_UINT_BYTES_LEN == 0)) {
            j++;
            k = 0;
        }
        b         = (scBitmap[j] >> (AT_UINT_DISPLACE_MAX - k));
        bitmap[i] = '0' + b;
        scBitmap[j] -= (VOS_INT8)(b << (AT_UINT_DISPLACE_MAX - k));

        i++;
        k++;
    }
    bitmap[bitmapLen] = '\0';
}

VOS_UINT32 AT_RcvVrrcMsgQryRppCfgCnfProc(MN_AT_IndEvt *msg)
{
    VOS_UINT8             indexNum     = 0;
    VOS_UINT8             loop         = 0;
    VOS_UINT16            length       = 0;
    VOS_UINT8             bitmapLen    = 0;
    VOS_UINT8            *bitmap       = VOS_NULL_PTR;
    VRRC_AT_RppcfgQryCnf *rppCfgQryCnf = VOS_NULL_PTR;

    /* 初始化 */
    rppCfgQryCnf = (VRRC_AT_RppcfgQryCnf *)msg->content; /*lint !e838 */

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrrcMsgQryRppCfgCnfProc: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrrcMsgQryRppCfgCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_RPPCFG_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvVrrcMsgQryRppCfgCnfProc : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    for (loop = 0; loop < rppCfgQryCnf->poolNum; loop++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,%d,%d,%d,", g_atCrLf,
            g_parseContext[indexNum].cmdElement->cmdName, rppCfgQryCnf->groupType, rppCfgQryCnf->rtType,
            rppCfgQryCnf->resPool[loop].poolId, rppCfgQryCnf->resPool[loop].slOffsetIndicator);

        bitmapLen = rppCfgQryCnf->resPool[loop].bitmapLen;

        if (bitmapLen > 0) {
            bitmap = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, bitmapLen + 1);

            /* 分配内存失败，直接返回 */
            if (bitmap == VOS_NULL_PTR) {
                AT_ERR_LOG("AT_RcvVrrcMsgQryRppCfgCnfProc: bitmap, Alloc mem fail");

                return VOS_ERR;
            }
            memset_s(bitmap, bitmapLen + 1, 0x00, bitmapLen + 1);

            AT_InitBitMap(bitmap, bitmapLen, rppCfgQryCnf->resPool[loop].subChanBitmap, bitmapLen);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", bitmap);

            PS_MEM_FREE(WUEPS_PID_AT, bitmap);
            bitmap = VOS_NULL_PTR;
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d,%d,%d,%d%s", rppCfgQryCnf->resPool[loop].adjacency,
            rppCfgQryCnf->resPool[loop].subChnSize, rppCfgQryCnf->resPool[loop].subChnNum,
            rppCfgQryCnf->resPool[loop].subChnStartRB, rppCfgQryCnf->resPool[loop].pscchPoolStartRB,
            rppCfgQryCnf->poolNum, g_atCrLf);
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuVphyStatQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_VphystatQryCnf *vphyStatCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT16              length;
    VOS_UINT8               indexNum = 0;

    /* 初始化 */
    vphyStatCnf = (VRRC_AT_VphystatQryCnf *)msg->content;
    length      = 0;
    result      = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuVphyStatQryCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuVphyStatQryCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VPHYSTAT_QRY */
    if (AT_CMD_VPHYSTAT_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VPHYSTAT查询命令返回 */
    if (vphyStatCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuVphyStatQryCnf: AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            vphyStatCnf->firstSaDecSucSum, vphyStatCnf->secSaDecSucSum, vphyStatCnf->twiSaDecSucSum,
            vphyStatCnf->uplinkPackSum, vphyStatCnf->downlinkPackSum, vphyStatCnf->phyUplinkPackSum);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuVphyStatClrSetCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_VphystatClrSetCnf *vphyStatClrSetCnf;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum = 0;

    /* 初始化消息变量 */
    result            = AT_ERROR;
    vphyStatClrSetCnf = (VRRC_AT_VphystatClrSetCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuVphyStatClrSetCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuVphyStatClrSetCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VPHYSTATCLR_SET */
    if (AT_CMD_VPHYSTATCLR_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VPHYSTATCLR底层返回值 */
    if (vphyStatClrSetCnf->result == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuVsnrRsrpQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_VsnrrsrpQryCnf *vsnrRsrpQryCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum = 0;
    VOS_UINT8               loop;

    /* 初始化 */
    vsnrRsrpQryCnf = (VRRC_AT_VsnrrsrpQryCnf *)msg->content;
    result         = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuVsnrRsrpQryCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuVsnrRsrpQryCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VSNRRSRP_QRY */
    if (AT_CMD_VSNRRSRP_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VSNRRSRP查询命令返回 */
    if (vsnrRsrpQryCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuVsnrRsrpQryCnf: AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s:%d%s",
            g_parseContext[indexNum].cmdElement->cmdName, vsnrRsrpQryCnf->msgNum, g_atCrLf);

        for (loop = 0; loop < vsnrRsrpQryCnf->msgNum; ++loop) {
            g_atSendDataBuff.bufLen +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
                    "%s: %5d      %5d      %5d      %5d      %5d", g_parseContext[indexNum].cmdElement->cmdName,
                    vsnrRsrpQryCnf->saMainSNR[loop], vsnrRsrpQryCnf->saDiversitySNR[loop],
                    vsnrRsrpQryCnf->daMainSNR[loop], vsnrRsrpQryCnf->daDiversitySNR[loop],
                    vsnrRsrpQryCnf->daRsrp[loop]);
            if (loop != vsnrRsrpQryCnf->msgNum - 1) {
                g_atSendDataBuff.bufLen +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s", g_atCrLf);
            }
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuV2xRssiQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_RssiQryCnf *rssiQryCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexNum = 0;

    /* 初始化 */
    rssiQryCnf = (VRRC_AT_RssiQryCnf *)msg->content;
    result     = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuV2xRssiQryCnf: WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuV2xRssiQryCnf: AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为 AT_CMD_V2XRSSI_QRY */
    if (AT_CMD_V2XRSSI_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化 AT^V2XRSSI 查询命令返回 */
    if (rssiQryCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuV2xRssiQryCnf: AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s:%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, rssiQryCnf->daMainRSSIMax, rssiQryCnf->daDiversityRSSIMax);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvVrssiQryCnf(MN_AT_IndEvt *msg)
{
    VMAC_AT_VrssiQryCnf *vrssiCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT16           length;
    VOS_UINT8            indexNum = 0;

    /* 初始化 */
    vrssiCnf = (VMAC_AT_VrssiQryCnf *)msg->content;
    length   = 0;
    result   = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrssiQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrssiQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VRSSI_QRY */
    if (AT_CMD_VRSSI_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvVrssiQryCnf : AT_CMD_VRSSI_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VRSSI查询命令返回 */
    if (vrssiCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvVrssiQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, vrssiCnf->vrssi);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvPc5PktSndCnf(VTC_AT_MsgCnf *msg)
{
    VOS_UINT8               indexNum    = 0;
    VOS_UINT32              result      = AT_OK;
    VTC_AT_SendVMacDataCnf *sndDataStat = VOS_NULL_PTR;

    /* 初始化 */
    sndDataStat = (VTC_AT_SendVMacDataCnf *)msg->content; /*lint !e838 */

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvPc5PktSndCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvPc5PktSndCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (sndDataStat->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvVrrcMsgSetTxPowerCnfProc(MN_AT_IndEvt *msg)
{
    VRRC_AT_Set_Tx_PowerQryCnf         *powerOutputQryCnf = VOS_NULL_PTR;
    VOS_UINT32                          result;
    VOS_UINT8                           indexNum = 0;

    /* 初始化消息变量 */
    result            = AT_ERROR;
    powerOutputQryCnf = (VRRC_AT_Set_Tx_PowerQryCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetTxPowerCnfProc: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetTxPowerCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VTXPOWER_SET) {
        AT_WARN_LOG("AT_RcvVrrcMsgSetTxPowerCnfProc : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* phy会将实际设置的功率返回回来，对返回的功率进行粗略范围检查 */
    if (powerOutputQryCnf->result == VOS_OK) {
        if ((powerOutputQryCnf->txPower >= AT_SET_TX_POWER_MIN) &&
            (powerOutputQryCnf->txPower <= AT_SET_TX_POWER_MAX)) {
            result = AT_OK;
        } else {
            AT_WARN_LOG1("AT_RcvVrrcMsgSetTxPowerCnfProc, WARNING, Return result %d!", result);
            result = AT_ERROR;
        }
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvTestModeModifyL2IdSetCnf(VTC_AT_MsgCnf *msg)
{
    AT_RreturnCodeUint32 atReturnCode;
    VTC_AT_MsgSetCnf *para = VOS_NULL_PTR;
    VOS_UINT8 indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CV2XL2ID_SET) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para = (VTC_AT_MsgSetCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    At_FormatResultData(indexNum, atReturnCode);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTestModeModifyL2IdQryCnf(VTC_AT_MsgCnf *msg)
{
    VTC_AT_TestModeModifyL2IdQryCnf *para = VOS_NULL_PTR;
    AT_RreturnCodeUint32 atReturnCode;
    VOS_UINT8 indexNum = 0;

    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CV2XL2ID_QRY) {
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    para         = (VTC_AT_TestModeModifyL2IdQryCnf *)msg->content;
    atReturnCode = AT_ConvertResultCodeFromVtc(para->result);
    if (atReturnCode != AT_OK) {
        At_FormatResultData(indexNum, atReturnCode);
        return VOS_OK;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^CV2XL2ID: %d,%d", para->para.srcId, para->para.dstId);

    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}

#if (FEATURE_LTEV_WL == FEATURE_ON)

VOS_INT32 IsDigit(VOS_INT8 ch)
{
    if (ch >= '0' && ch <= '9')
        return 1;
    else
        return 0;
}


VOS_VOID AT_GetStrAfterchr(VOS_INT8 *src, VOS_INT8 c, VOS_INT8 *dst)
{
    VOS_UINT32 srcLoop = 0;
    VOS_UINT32 dstLoop = 0;
    while (src[srcLoop] != '\0' && src[srcLoop] != c) {
        srcLoop++;
    }
    srcLoop++;

    for (dstLoop = 0; IsDigit(src[srcLoop]) && src[srcLoop] != '\0'; dstLoop++)
        dst[dstLoop] = src[srcLoop++];
    dst[dstLoop] = '\0';
}


VOS_VOID AT_GetStrBeforechr(VOS_INT8 *src, VOS_INT8 c, VOS_INT8 *dst)
{
    VOS_UINT32 loop = 0;

    for (; src[loop] != c; loop++) {
        if (src[loop] == '\0')
            return;
        dst[loop] = src[loop];
    }
    dst[loop] = '\0';
}


VOS_VOID AT_GetIndexNum(VOS_UINT8 *data, VOS_UINT32 len, VOS_UINT32 *indexMin, VOS_UINT32 *indexMax)

{
    VOS_UINT8 infoBuf[AT_MTA_MAX_INFO_LEN]     = {0};
    VOS_UINT8 indexMinBuf[AT_MTA_MAX_INFO_LEN] = {0};
    VOS_UINT8 indexMaxBuf[AT_MTA_MAX_INFO_LEN] = {0};
    errno_t   memResult;

    /* 分配内存失败，直接返回 */
    if ((data == VOS_NULL_PTR)  || (indexMin == VOS_NULL_PTR) || (indexMax == VOS_NULL_PTR)) {
        AT_ERR_LOG("AT_BcdToAsciiCode: Parameter of the function is null.");
        return;
    }

    memResult = memcpy_s(infoBuf, AT_MTA_MAX_INFO_LEN, data + len + 1, AT_MTA_MAX_INFO_LEN - len - 1);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_MTA_MAX_INFO_LEN, AT_MTA_MAX_INFO_LEN - len - 1);

    AT_GetStrBeforechr((VOS_INT8 *)infoBuf, ';', (VOS_INT8 *)indexMinBuf);
    AT_GetStrAfterchr((VOS_INT8 *)infoBuf, ';', (VOS_INT8 *)indexMaxBuf);
    AT_AtoInt((VOS_CHAR *)indexMinBuf, (VOS_INT32 *)indexMin);
    AT_AtoInt((VOS_CHAR *)indexMaxBuf, (VOS_INT32 *)indexMax);
}


VOS_UINT32 TAF_GetFileSize(FILE *file, VOS_UINT32 *fileSize)
{
    VOS_INT32 size;
    /* 定位到文件尾部 */
    if ((-1) == mdrv_file_seek(file, 0L, SEEK_END)) {
        AT_WARN_LOG("TAF_GetFileSize:LSEEK error\n");
        return VOS_ERR;
    }

    /* 读取文件大小 */
    size = (int)mdrv_file_tell(file);

    if ((-1) == (size)) {
        *fileSize = 0;
        AT_WARN_LOG("TAF_GetFileSize:ftell error\n");
        return VOS_ERR;
    }

    *fileSize = (VOS_UINT32)size;
    return VOS_OK;
}


VOS_UINT32 AT_IndexRdProc(AT_MTA_FileRdReq *data, VOS_UINT8 *rec, VOS_UINT8 recSize)
{
    AT_MTA_FileRdReq *atFileRdReq                             = VOS_NULL_PTR;
    TAF_CHAR         *DirPath                                 = "/data/UET_TEST";
    VOS_VOID         *file                                    = VOS_NULL_PTR;
    TAF_CHAR          fileName[AT_UET_TEST_FILE_NAME_MAX_LEN] = {0};
    VOS_INT32         length;
    VOS_INT32         ret;
    VOS_UINT32        fileSize                     = 0;
    VOS_UINT8         infoBuf[AT_MTA_MAX_INFO_LEN] = {0};
    VOS_UINT32        loop                         = 0;
    errno_t           memResult;
    VOS_LONG          position = 0;

    atFileRdReq = data;
    length      = snprintf_s(fileName, (VOS_SIZE_T)sizeof(fileName),
                             (VOS_SIZE_T)sizeof(fileName) - 1, "%s%s", DirPath, "/INDEX.ini");

    if (length <= 0) {
        AT_WARN_LOG("AT_IndexRdProc: snprintf_s len <= 0");
        return VOS_ERR;
    }

    file = mdrv_file_open(fileName, "r");
    if (file == 0) {
        AT_WARN_LOG("AT_IndexRdProc: mdrv_file_open failed.\n");
        return VOS_ERR;
    }

    /* 读取文件大小 */
    ret = (VOS_INT32)TAF_GetFileSize(file, &fileSize);
    if (ret != VOS_OK) {
        mdrv_file_close(file);
        AT_WARN_LOG("AT_IndexRdProc: TAF_GetFileSize failed.\n");
        return VOS_ERR;
    }

    for (loop = 0; loop < fileSize / AT_MTA_MAX_INFO_LEN; loop++) {
        /* 定位到索引位置 */
        position = (VOS_LONG)loop * AT_MTA_MAX_INFO_LEN;
        ret      = mdrv_file_seek(file, position, SEEK_SET);
        if ((-1) == ret) {
            mdrv_file_close(file);
            AT_WARN_LOG("AT_IndexRdProc: LSEEK failed.\n");
            return VOS_ERR;
        }

        ret = mdrv_file_read(infoBuf, AT_MTA_MAX_INFO_LEN, 1, file);
        if (TAF_MEM_CMP(infoBuf, atFileRdReq->index, atFileRdReq->indexLen) == 0) {
            memResult = memcpy_s(rec, recSize, infoBuf, AT_MTA_MAX_INFO_LEN);
            TAF_MEM_CHK_RTN_VAL(memResult, recSize, AT_MTA_MAX_INFO_LEN);
            mdrv_file_close(file);
            return VOS_OK;
        }
    }

    mdrv_file_close(file);

    return VOS_ERR;
}


VOS_VOID AT_ProcMsgFromVsync(struct MsgCB *msg)
{
    MN_AT_IndEvt *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32    i;
    VOS_UINT32    msgCnt;
    VOS_UINT32    msgName;
    VOS_UINT32    rst;

    /* 从g_astAtProcMsgFromVsyncTab中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromVsyncTab) / sizeof(AT_VSYNC_ProcMsgTbl);
    rcvMsg = (MN_AT_IndEvt *)msg;

    /* 从消息包中获取MSG Name */
    msgName = rcvMsg->msgName;

    /* g_astAtProcMsgFromVrrcTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromVsyncTab[i].msgName == msgName) {
            rst = g_atProcMsgFromVsyncTab[i].procMsgFunc(rcvMsg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("At_ProcMsgFromDrvAgent: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_ProcMsgFromDrvAgent: Msg Id is invalid!");
    }
    return;
}


VOS_UINT32 AT_RcvPc5SyncSetCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_Pc5syncSetCnf *pc5SyncSetCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum = 0;

    /* 初始化消息变量 */
    result        = AT_ERROR;
    pc5SyncSetCnf = (VRRC_AT_Pc5syncSetCnf *)msg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvPc5SyncSetCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvPc5SyncSetCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PC5SYNC_SET */
    if (AT_CMD_PC5SYNC_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvPc5SyncSetCnf : AT_CMD_PC5SYNC_SET.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^PC5SYNC设置PC5同步参数返回值 */
    if (pc5SyncSetCnf->result == VOS_OK) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvPc5SyncQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_Pc5syncQryCnf *pc5SyncQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;

    /* 初始化 */
    pc5SyncQryCnf = (VRRC_AT_Pc5syncQryCnf *)msg->content;
    length        = 0;
    result        = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvPc5SyncQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvPc5SyncQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PC5SYNC_QRY */
    if (AT_CMD_PC5SYNC_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvPc5SyncQryCnf : AT_CMD_PC5SYNC_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^PC5SYNC查询命令返回 */
    if (pc5SyncQryCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvPc5SyncQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, pc5SyncQryCnf->syncSwitch, pc5SyncQryCnf->syncDfnSwitch,
            pc5SyncQryCnf->gnssValidTimerLen, pc5SyncQryCnf->syncOffsetInd1, pc5SyncQryCnf->syncOffsetInd2,
            pc5SyncQryCnf->syncOffsetInd3, pc5SyncQryCnf->syncTxThreshOoc, pc5SyncQryCnf->fltCoefficient,
            pc5SyncQryCnf->syncRefMinHyst, pc5SyncQryCnf->syncRefDiffHyst);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuMcsCfgCnf(MN_AT_IndEvt *msg)
{
    VOS_UINT8             indexNum     = 0;
    VMAC_AT_McgCfgSetCnf *mcgCfgSetCnf = VOS_NULL_PTR;

    /* 初始化 */
    mcgCfgSetCnf = (VMAC_AT_McgCfgSetCnf *)msg->content; /*lint !e838 */

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuMcsCfgCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuMcsCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_MCGCFG_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvRsuMcsCfgCnf : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }
    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetLedTestRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg      *rcvMsg                = VOS_NULL_PTR;
    DRV_AGENT_LedTestSetCnf *drvAgentLedTestSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum              = 0;
    VOS_UINT32               result;

    rcvMsg                = (DRV_AGENT_Msg *)msg;
    drvAgentLedTestSetCnf = (DRV_AGENT_LedTestSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentLedTestSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetLedTestRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetLedTestRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_LEDTEST_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (drvAgentLedTestSetCnf->isFail == VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetGpioTestRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg       *rcvMsg                 = VOS_NULL_PTR;
    DRV_AGENT_GpioTestSetCnf *drvAgentGpioTestSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum               = 0;
    VOS_UINT32                result;

    rcvMsg                 = (DRV_AGENT_Msg *)msg;
    drvAgentGpioTestSetCnf = (DRV_AGENT_GpioTestSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentGpioTestSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetGpioTestRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetGpioTestRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_GPIOTEST_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (DRV_AGENT_NO_ERROR == drvAgentGpioTestSetCnf->isFail) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryAntTestRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg      *rcvMsg                = VOS_NULL_PTR;
    DRV_AGENT_AntTestQryCnf *drvAgentAntTestQryCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum              = 0;
    VOS_UINT32               result;
    VOS_UINT16               length = 0;

    rcvMsg                = (DRV_AGENT_Msg *)msg;
    drvAgentAntTestQryCnf = (DRV_AGENT_AntTestQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentAntTestQryCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryAntTestRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryAntTestRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_ANTTEST_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (DRV_AGENT_NO_ERROR == drvAgentAntTestQryCnf->result) {
        /* 输出查询结果 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, drvAgentAntTestQryCnf->laAntState,
            drvAgentAntTestQryCnf->lbAntState, drvAgentAntTestQryCnf->lvaAntState, drvAgentAntTestQryCnf->lvbAntState,
            drvAgentAntTestQryCnf->gnssAntState);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetHkadcTestRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg        *rcvMsg                  = VOS_NULL_PTR;
    DRV_AGENT_HkadctestSetCnf *drvAgentHkadcTestSetCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum                = 0;
    VOS_UINT32                 result;
    VOS_UINT16                 length = 0;

    rcvMsg                  = (DRV_AGENT_Msg *)msg;
    drvAgentHkadcTestSetCnf = (DRV_AGENT_HkadctestSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentHkadcTestSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetHkadcTestRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetHkadcTestRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_HKADCTEST_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (DRV_AGENT_NO_ERROR == drvAgentHkadcTestSetCnf->result) {
        /* 输出查询结果 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            drvAgentHkadcTestSetCnf->temp, drvAgentHkadcTestSetCnf->volt);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentGpioQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg   *rcvMsg             = VOS_NULL_PTR;
    DRV_AGENT_GpioQryCnf *drvAgentGpioQryCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum           = 0;
    VOS_UINT32            result;
    VOS_UINT16            length = 0;

    rcvMsg             = (DRV_AGENT_Msg *)msg;
    drvAgentGpioQryCnf = (DRV_AGENT_GpioQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentGpioQryCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_GPIO_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (DRV_AGENT_NO_ERROR == drvAgentGpioQryCnf->result) {
        /* 输出查询结果 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            drvAgentGpioQryCnf->value);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentGpioSetRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg   *rcvMsg             = VOS_NULL_PTR;
    DRV_AGENT_GpioSetCnf *drvAgentGpioSetCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum           = 0;
    VOS_UINT32            result;

    rcvMsg             = (DRV_AGENT_Msg *)msg;
    drvAgentGpioSetCnf = (DRV_AGENT_GpioSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentGpioSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioSetRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioSetRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_GPIO_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (DRV_AGENT_NO_ERROR == drvAgentGpioSetCnf->result) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentBootModeSetRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg       *rcvMsg                 = VOS_NULL_PTR;
    DRV_AGENT_BootModeSetCnf *drvAgentBootModeSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum               = 0;

    rcvMsg                 = (DRV_AGENT_Msg *)msg;
    drvAgentBootModeSetCnf = (DRV_AGENT_BootModeSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentBootModeSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentBootModeSetRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentBootModeSetRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_BOOTMODE_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (DRV_AGENT_NO_ERROR == drvAgentBootModeSetCnf->isFail) {
        g_atSendDataBuff.bufLen = 0;
        /* 调用AT_FormATResultDATa发送命令结果 */
        At_FormatResultData(indexNum, AT_OK);
        VOS_TaskDelay(300); /* wait 300 tick */

        mdrv_sysboot_restart();
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetGpioHeatSetRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg       *rcvMsg                 = VOS_NULL_PTR;
    DRV_AGENT_GpioTestSetCnf *drvAgentGpioTestSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum               = 0;
    VOS_UINT32                result;

    rcvMsg                 = (DRV_AGENT_Msg *)msg;
    drvAgentGpioTestSetCnf = (DRV_AGENT_GpioTestSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(drvAgentGpioTestSetCnf->atAppCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetGpioHeatSetRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetGpioHeatSetRsp : AT_GPIOHEATSET_INDEX.");
        return VOS_ERR;
    }

    if (AT_CMD_TEMPHEATTEST_SET != g_atClientTab[indexNum].cmdCurrentOpt) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (drvAgentGpioTestSetCnf->isFail == VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaFileWrCnf(struct MsgCB *msg)
{
    AT_MTA_Msg  *rcvMsg    = VOS_NULL_PTR;
    MTA_AT_FileWrCnf *fileWrCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum = 0;

    /* 初始化 */
    rcvMsg    = (AT_MTA_Msg *)msg;
    fileWrCnf = (MTA_AT_FileWrCnf *)rcvMsg->content;
    result    = AT_OK;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvMtaFileWrCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaFileWrCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_FILE_WRITE != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvMtaFileWrCnf : Current Option is not AT_CMD_FILE_WRITE.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (MTA_AT_RESULT_NO_ERROR == fileWrCnf->result) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaFileRdCnf(struct MsgCB *msg)
{
    AT_MTA_Msg  *rcvMsg    = VOS_NULL_PTR;
    MTA_AT_FileRdCnf *fileRdCnf = VOS_NULL_PTR;
    AT_MTA_FileRdReq  atFileRdReq;
    VOS_UINT32        result;
    VOS_UINT8         indexNum = 0;
    VOS_UINT8        *rec      = VOS_NULL_PTR;
    TAF_CHAR         *DirPath  = "/data/UET_TEST";
    VOS_VOID         *file     = VOS_NULL_PTR;
    TAF_CHAR          fileName[AT_UET_TEST_FILE_NAME_MAX_LEN];
    VOS_INT32         length   = 0;
    VOS_UINT32        fileType = 0;
    VOS_INT32         ret;
    VOS_UINT32        fileSize                         = 0;
    VOS_UINT8         indexBuf[AT_MTA_MAX_INFO_LEN]    = {0};
    VOS_UINT8         elabelBuf[AT_MTA_MAX_ELABEL_LEN] = {0};
    VOS_UINT32        loop                             = 0;
    errno_t           memResult;

    /* 初始化 */
    rcvMsg    = (AT_MTA_Msg *)msg;
    fileRdCnf = (MTA_AT_FileRdCnf *)rcvMsg->content;
    result    = AT_OK;

    memResult = memset_s(&atFileRdReq, sizeof(atFileRdReq), 0x00, sizeof(AT_MTA_FileRdReq));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atFileRdReq), sizeof(AT_MTA_FileRdReq));
    memResult = memset_s(fileName, (VOS_SIZE_T)sizeof(fileName), 0x00, (VOS_SIZE_T)sizeof(fileName));
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(fileName), (VOS_SIZE_T)sizeof(fileName));


    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum)) {
        AT_WARN_LOG("AT_RcvMtaFileRdCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaFileRdCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_FILE_READ != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvMtaFileRdCnf : Current Option is not AT_CMD_FILE_READ.");
        return VOS_ERR;
    }

    memResult = memcpy_s(atFileRdReq.index, sizeof(atFileRdReq.index), fileRdCnf->index, fileRdCnf->indexLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atFileRdReq.index), fileRdCnf->indexLen);
    atFileRdReq.indexLen = fileRdCnf->indexLen;

    if (TAF_MEM_CMP("ELABEL", atFileRdReq.index, atFileRdReq.indexLen) == 0) {
        length   = snprintf_s(fileName, (VOS_SIZE_T)sizeof(fileName), (VOS_SIZE_T)sizeof(fileName) - 1,
                              "%s%s", DirPath, "/ELABEL.ini");
        fileType = AT_FILE_TYPE_ELABEL;
    } else if (TAF_MEM_CMP("INDEX", atFileRdReq.index, atFileRdReq.indexLen) == 0) {
        length   = snprintf_s(fileName, (VOS_SIZE_T)sizeof(fileName), (VOS_SIZE_T)sizeof(fileName) - 1,
                              "%s%s", DirPath, "/INDEX.ini");
        fileType = AT_FILE_TYPE_INDEX;
    } else
        fileType = AT_OTHERS_FILE_TYPE;

    if (fileType != 0 && length <= 0) {
        AT_WARN_LOG("AT_RcvMtaFileRdCnf: snprintf_s len <= 0\n");
        return VOS_ERR;
    }

    /* 文件夹不存在，返回错误 */
    if (mdrv_file_access(DirPath, 0) != VOS_OK) {
        AT_WARN_LOG("AT_RcvMtaFileRdCnf: mdrv_file_access\n");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (fileType != AT_OTHERS_FILE_TYPE) {
        file = mdrv_file_open(fileName, "r");
        if (file == 0) {
            AT_WARN_LOG("AT_RcvMtaFileRdCnf: mdrv_file_open failed.\n");
            return VOS_ERR;
        }

        /* 读取文件大小 */
        ret = (VOS_INT32)TAF_GetFileSize(file, &fileSize);
        if (ret != VOS_OK) {
            AT_WARN_LOG("AT_RcvMtaFileRdCnf: TAF_GetFileSize failed.\n");
            return VOS_ERR;
        }

        if (fileType == AT_FILE_TYPE_ELABEL) {
            /* 定位到索引位置 */
            ret = mdrv_file_seek(file, 0, SEEK_SET);
            if ((-1) == ret) {
                AT_WARN_LOG("AT_RcvMtaFileRdCnf: LSEEK failed.\n");
                return VOS_ERR;
            }

            ret = mdrv_file_read(elabelBuf, AT_MTA_MAX_ELABEL_LEN, 1, file);
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s:\r\n%s",
                g_parseContext[indexNum].cmdElement->cmdName, elabelBuf);
        }
        if (fileType == AT_FILE_TYPE_INDEX) {
            for (loop = 0; loop < fileSize / AT_MTA_MAX_INFO_LEN; loop++) {
                /* 定位到索引位置 */
                ret = mdrv_file_seek(file, (VOS_LONG)(loop * AT_MTA_MAX_INFO_LEN), SEEK_SET);
                if ((-1) == ret) {
                    AT_WARN_LOG("AT_RcvMtaFileRdCnf: LSEEK failed.\n");
                    return VOS_ERR;
                }

                ret = mdrv_file_read(indexBuf, AT_MTA_MAX_INFO_LEN, 1, file);
                g_atSendDataBuff.bufLen +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s:%s",
                        g_parseContext[indexNum].cmdElement->cmdName, indexBuf);
            }
        }
        mdrv_file_close(file);
    } else {

        rec = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_MTA_MAX_INFO_LEN);
        if (rec == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_RcvMtaFileRdCnf: rec, Alloc mem fail");
            return VOS_ERR;
        }
        memset_s(rec, AT_MTA_MAX_INFO_LEN, 0x00, AT_MTA_MAX_INFO_LEN);

        ret = (VOS_INT32)AT_IndexRdProc(&atFileRdReq, rec);

        if (MTA_AT_RESULT_NO_ERROR == fileRdCnf->result) {
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, rec);

            result = AT_OK;
        }
        if (ret == VOS_ERR) {
            result                  = AT_ERROR;
            g_atSendDataBuff.bufLen = 0;
        }

        PS_MEM_FREE(WUEPS_PID_AT, rec);
        rec = VOS_NULL_PTR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuSyncStatQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_SyncstQryCnf *syncStatCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT16            length;
    VOS_UINT8             indexNum = 0;

    /* 初始化 */
    syncStatCnf = (VRRC_AT_SyncstQryCnf *)msg->content;
    length      = 0;
    result      = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuSyncStatQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuSyncStatQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SYNCST_QRY */
    if (AT_CMD_SYNCST_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvRsuSyncStatQryCnf : AT_CMD_SYNCST_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    syncStatCnf->syncStat = (syncStatCnf->syncStat == 0) ? 1 : 0;
    /* 格式化AT^RSUSYNCST查询命令返回 */
    if (syncStatCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuSyncStatQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)(syncStatCnf->syncSrc), (VOS_INT32)(syncStatCnf->syncStat));

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvRsuGnssSyncStatQryCnf(MN_AT_IndEvt *msg)
{
    VRRC_AT_GnsssyncstQryCnf *syncStatCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum = 0;

    /* 初始化 */
    syncStatCnf = (VRRC_AT_GnsssyncstQryCnf *)msg->content;
    length      = 0;
    result      = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRsuGnssSyncStatQryCnf:WARNING:AT INDEX NOT FOUND!\n");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRsuGnssSyncStatQryCnf : AT_BROADCAST_INDEX.\n");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SYNCST_QRY */
    if (AT_CMD_GNSSSYNCST_QRY != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvRsuGnssSyncStatQryCnf : AT_CMD_GNSSSYNCST_QRY.\n");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    /* 格式化AT^RSUSYNCST查询命令返回 */
    if (syncStatCnf->qryRslt != VOS_OK) {
        AT_WARN_LOG("AT_RcvRsuGnssSyncStatQryCnf : AT_ERROR.\n");
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)(syncStatCnf->syncStat));

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#endif
#endif

