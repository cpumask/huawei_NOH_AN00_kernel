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

#include "AtCmdCagpsProc.h"
#include "securec.h"
#include "AtEventReport.h"
#include "mn_comm_api.h"

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMDCAGPSPROC_C

#define AT_CAGPSFORWARDDATA_PARA_MAX_NUM 5
#define AT_CAGPSFORWARDDATA_PARA_SERVER_MODE 0      /* CAGPSFORWARDDATA的第一个参数SERVER_MODE */
#define AT_CAGPSFORWARDDATA_PARA_TOTAL_SEGMENTS 1   /* CAGPSFORWARDDATA的第二个参数TOTAL_SEGMENTS */
#define AT_CAGPSFORWARDDATA_PARA_CURRENT_SEGMENTS 2 /* CAGPSFORWARDDATA的第三个参数CURRENT_SEGMENTS */
#define AT_CAGPSFORWARDDATA_PARA_DATA_LEN 3         /* CAGPSFORWARDDATA的第四个参数DATA_LEN */
#define AT_CAGPSFORWARDDATA_PARA_DATA 4             /* CAGPSFORWARDDATA的第五个参数DATA */

#define AT_CGPSCONTROLSTART_PARA_NUM 2
#define AT_CGPSCONTROLSTART_DATA_MAX_LEN 4
#define AT_CGPSCONTROLSTART_DATA_LEN 0
#define AT_CGPSCONTROLSTART_DATA 1

#define AT_CGPSCONTROLSTOP_PARA_NUM 2
#define AT_CGPSCONTROLSTOP_DATA_MAX_LEN 4
#define AT_CGPSCONTROLSTOP_DATA_LEN 0
#define AT_CGPSCONTROLSTOP_DATA 1

#define AT_CUPBINDSTATUS_PARA_MAX_NUM 2
#define AT_CUPBINDSTATUS_PARA_SERVERMODE 0 /* CUPBINDSTATUS的第一个参数SERVERMODE */
#define AT_CUPBINDSTATUS_PARA_BINDSTATUS 1 /* CUPBINDSTATUS的第二个参数BINDSTATUS */

#define AT_CAGPS_PARA_NUM 2

#define AT_CAGPSQRYTIME_PARA_NUM 2
#define AT_CAGPSQRYTIME_PARA_DATA 1 /* CAGPSQRYTIME的第二个参数DATA */

AT_CagpsCmdNameTlb g_atCagpsCmdNameTlb[] = {
    { ID_XPDS_AT_GPS_TIME_INFO_IND, 0, "^CAGPSTIMEINFO" },
    { ID_XPDS_AT_GPS_REFLOC_INFO_CNF, 0, "^CAGPSREFLOCINFO" },
    { ID_XPDS_AT_GPS_PDE_POSI_INFO_IND, 0, "^CAGPSPDEPOSINFO" },
    { ID_XPDS_AT_GPS_NI_SESSION_IND, 0, "^CAGPSNIREQ" },
    { ID_XPDS_AT_GPS_START_REQ, 0, "^CAGPSSTARTREQ" },
    { ID_XPDS_AT_GPS_CANCEL_IND, 0, "^CAGPSCANCELIND" },
    { ID_XPDS_AT_GPS_ACQ_ASSIST_DATA_IND, 0, "^CAGPSACQASSISTINFO" },
    { ID_XPDS_AT_GPS_ABORT_IND, 0, "^CAGPSABORT" },
    { ID_XPDS_AT_GPS_ION_INFO_IND, 0, "^CAGPSIONINFO" },
    { ID_XPDS_AT_GPS_EPH_INFO_IND, 0, "^CAGPSEPHINFO" },
    { ID_XPDS_AT_GPS_DEL_ASSIST_DATA_IND, 0, "^CAGPSDELASSISTDATAIND" },

    { ID_XPDS_AT_GPS_ALM_INFO_IND, 0, "^CAGPSALMINFO" },

    { ID_XPDS_AT_GPS_NI_CP_START, 0, "^CAGPSNICPSTART" },
    { ID_XPDS_AT_GPS_NI_CP_STOP, 0, "^CAGPSNICPSTOP" },
    { ID_XPDS_AT_GPS_UTS_TEST_START_REQ, 0, "^CGPSCONTROLSTART" },
    { ID_XPDS_AT_GPS_UTS_TEST_STOP_REQ, 0, "^CGPSCONTROLSTOP" },

    { ID_XPDS_AT_UTS_GPS_POS_INFO_IND, 0, "^UTSGPSPOSINFO" },
    { ID_XPDS_AT_GPS_OM_TEST_START_REQ, 0, "^CGPSTESTSTART" },
    { ID_XPDS_AT_GPS_OM_TEST_STOP_REQ, 0, "^CGPSTESTSTOP" },
};

AT_CagpsCmdOptTlb g_atCagpsCmdOptTbl[] = {
    { ID_XPDS_AT_GPS_START_CNF, AT_CMD_CAGPSSTART_SET },
    { ID_XPDS_AT_GPS_CFG_MPC_ADDR_CNF, AT_CMD_CAGPSCFGMPCADDR_SET },
    { ID_XPDS_AT_GPS_CFG_PDE_ADDR_CNF, AT_CMD_CAGPSCFGPDEADDR_SET },
    { ID_XPDS_AT_GPS_CFG_MODE_CNF, AT_CMD_CAGPSCFGPOSMODE_SET },
    { ID_XPDS_AT_GPS_STOP_CNF, AT_CMD_CAGPSSTOP_SET },
};


VOS_UINT32 At_SetAgpsDataCallStatus(VOS_UINT8 indexNum)
{
    AT_XPDS_ApDataCallStatusInd *dataCallInd = VOS_NULL_PTR;
    VOS_UINT32                   msgLength;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[0].paraValue >= AT_XPDS_DATA_CALL_STATUS_BUTT) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    msgLength = sizeof(AT_XPDS_ApDataCallStatusInd) - VOS_MSG_HEAD_LENGTH;

    dataCallInd = (AT_XPDS_ApDataCallStatusInd *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (dataCallInd == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)dataCallInd, WUEPS_PID_AT, AT_GetDestPid(dataCallInd->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    dataCallInd->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    dataCallInd->appCtrl.opId     = At_GetOpId();

    dataCallInd->channelState = g_atParaList[0].paraValue;

    dataCallInd->msgId = ID_AT_XPDS_AP_DATA_CALL_STATUS_IND;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, dataCallInd) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetAgpsUpBindStatus(VOS_UINT8 indexNum)
{
    AT_XPDS_ApServerBindStatusInd *msgBindInd = VOS_NULL_PTR;
    VOS_UINT32                     msgLength;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_CUPBINDSTATUS_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if ((g_atParaList[AT_CUPBINDSTATUS_PARA_SERVERMODE].paraValue >= AT_XPDS_SERVER_MODE_BUTT) ||
        (g_atParaList[AT_CUPBINDSTATUS_PARA_BINDSTATUS].paraValue >= AT_XPDS_SERVER_BIND_STATUS_BUTT)) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    msgLength = sizeof(AT_XPDS_ApServerBindStatusInd) - VOS_MSG_HEAD_LENGTH;

    msgBindInd = (AT_XPDS_ApServerBindStatusInd *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (msgBindInd == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgBindInd, WUEPS_PID_AT, AT_GetDestPid(msgBindInd->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    msgBindInd->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgBindInd->appCtrl.opId     = At_GetOpId();

    msgBindInd->serverMode = g_atParaList[AT_CUPBINDSTATUS_PARA_SERVERMODE].paraValue;
    msgBindInd->bindStatus = g_atParaList[AT_CUPBINDSTATUS_PARA_BINDSTATUS].paraValue;
    msgBindInd->msgId      = ID_AT_XPDS_AP_SERVER_BIND_STATUS_IND;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgBindInd) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetAgpsForwardData(VOS_UINT8 indexNum)
{
    VOS_UINT32                dataLen;
    VOS_UINT32                msgLength;
    AT_XPDS_ApForwardDataInd *msgFwdDataInd = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_CAGPSFORWARDDATA_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if ((g_atParaList[AT_CAGPSFORWARDDATA_PARA_SERVER_MODE].paraValue >= AT_XPDS_SERVER_MODE_BUTT) ||
        (g_atParaList[AT_CAGPSFORWARDDATA_PARA_CURRENT_SEGMENTS].paraValue >
         g_atParaList[AT_CAGPSFORWARDDATA_PARA_TOTAL_SEGMENTS].paraValue)) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    /* 第4个参数值为奇数，返回错误 */
    if ((g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA_LEN].paraValue % AT_DOUBLE_LENGTH) == 1) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    /* the value of para4 and the len of para5 must be equal */
    if (g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA_LEN].paraValue !=
        g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA].paraLen) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    dataLen = (g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA_LEN].paraValue / AT_ASCII_AND_HEX_CONVERSION_FACTOR);

    if (dataLen >= AT_DATA_DEFAULT_LENGTH) {
        msgLength = sizeof(AT_XPDS_ApForwardDataInd) + dataLen - VOS_MSG_HEAD_LENGTH - AT_DATA_DEFAULT_LENGTH;
    } else {
        msgLength = sizeof(AT_XPDS_ApForwardDataInd) - VOS_MSG_HEAD_LENGTH;
    }

    msgFwdDataInd = (AT_XPDS_ApForwardDataInd *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (msgFwdDataInd == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgFwdDataInd, WUEPS_PID_AT,
                  AT_GetDestPid(msgFwdDataInd->appCtrl.clientId, I0_UEPS_PID_XPDS), msgLength);

    msgFwdDataInd->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgFwdDataInd->appCtrl.opId     = At_GetOpId();

    msgFwdDataInd->serverMode = g_atParaList[AT_CAGPSFORWARDDATA_PARA_SERVER_MODE].paraValue;
    msgFwdDataInd->totalNum   = g_atParaList[AT_CAGPSFORWARDDATA_PARA_TOTAL_SEGMENTS].paraValue;
    msgFwdDataInd->curNum     = g_atParaList[AT_CAGPSFORWARDDATA_PARA_CURRENT_SEGMENTS].paraValue;
    msgFwdDataInd->dataLen    = dataLen;

    if (At_AsciiString2HexSimple(msgFwdDataInd->data, g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA].para,
                                 (VOS_UINT16)g_atParaList[AT_CAGPSFORWARDDATA_PARA_DATA_LEN].paraValue) != AT_SUCCESS) {
        PS_FREE_MSG(WUEPS_PID_AT, msgFwdDataInd);  //lint !e516

        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    msgFwdDataInd->msgId = ID_AT_XPDS_AP_FORWARD_DATA_IND;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgFwdDataInd) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvXpdsAgpsDataCallReq(struct MsgCB *msg)
{
    VOS_UINT8              indexNum;
    XPDS_AT_ApDataCallReq *msgDataCallReq = VOS_NULL_PTR;

    /* 初始化 */
    msgDataCallReq = (XPDS_AT_ApDataCallReq *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgDataCallReq->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsAgpsDataCallReq: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSDATACALLREQ: %d,%d%s", g_atCrLf, msgDataCallReq->agpsMode,
        msgDataCallReq->agpsOper, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvXpdsAgpsServerBindReq(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    XPDS_AT_ApServerBindReq *msgBindReq = VOS_NULL_PTR;

    /* 初始化 */
    msgBindReq = (XPDS_AT_ApServerBindReq *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgBindReq->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsAgpsServerBindReq: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSBINDCONN: %d,%d,%08x,%d%s", g_atCrLf, msgBindReq->serverMode,
        msgBindReq->dataCallCtrl, msgBindReq->ipAddr, msgBindReq->portNum, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvXpdsAgpsReverseDataInd(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum;
    VOS_UINT32                curNum;
    VOS_UINT32                totalNum;
    VOS_UINT32                offset;
    VOS_UINT32                outputLen;
    VOS_UINT32                remainLen;
    VOS_UINT16                length;
    VOS_UINT8                *src        = VOS_NULL_PTR;
    XPDS_AT_ApReverseDataInd *msgDataInd = VOS_NULL_PTR;

    /* 初始化 */
    msgDataInd = (XPDS_AT_ApReverseDataInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgDataInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsAgpsReverseDataInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    totalNum = (msgDataInd->dataLen / AT_XPDS_AGPS_DATAUPLEN_MAX);

    if ((msgDataInd->dataLen % AT_XPDS_AGPS_DATAUPLEN_MAX) != 0) {
        totalNum++;
    }

    if (totalNum >= VOS_NULL_WORD) {
        AT_WARN_LOG("AT_RcvXpdsAgpsReverseDataInd: Data Len Error.");
        return VOS_ERR;
    }

    offset = 0;

    src = msgDataInd->data;

    outputLen = 0;

    remainLen = msgDataInd->dataLen;

    for (curNum = 1; curNum <= totalNum; curNum++) {
        if (remainLen <= AT_XPDS_AGPS_DATAUPLEN_MAX) {
            outputLen = remainLen;
        } else {
            outputLen = AT_XPDS_AGPS_DATAUPLEN_MAX;

            remainLen -= AT_XPDS_AGPS_DATAUPLEN_MAX;
        }

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSREVERSEDATA: %d,%d,%d,%d,", g_atCrLf, msgDataInd->serverMode,
            totalNum, curNum, outputLen * AT_ASCII_AND_HEX_CONVERSION_FACTOR);

        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        length += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                           (TAF_UINT8 *)g_atSndCodeAddress + length, outputLen,
                                                           (src + offset));

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        offset += AT_XPDS_AGPS_DATAUPLEN_MAX;

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    return VOS_OK;
}


VOS_UINT32 AT_CagpsSndXpdsReq(VOS_UINT8 indexNum, AT_XPDS_MsgTypeUint32 msgType, VOS_UINT32 msgStructSize)
{
    errno_t      memResult;
    VOS_UINT32   dataLen;
    VOS_UINT32   msgLength;
    AT_XPDS_Msg *atXpdsMsg = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_CAGPS_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串长度不正确 */
    if (g_atParaList[0].paraValue != g_atParaList[1].paraLen) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将字符串转换为16进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[1].para, &g_atParaList[1].paraLen) == AT_FAILURE) {
        AT_ERR_LOG1("AT_CagpsSndXpdsReq: At_AsciiNum2HexString fail.", msgType);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    dataLen = msgStructSize - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) - sizeof(AT_APPCTRL);

    /* 消息内容长度是否正确 */
    if (dataLen != g_atParaList[1].paraLen) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (msgStructSize <= VOS_MSG_HEAD_LENGTH) {
        AT_ERR_LOG("AT_CagpsSndXpdsReq: atXpdsMsg cannot malloc space !");
        return AT_ERROR;
    }
    msgLength = msgStructSize - VOS_MSG_HEAD_LENGTH;


    atXpdsMsg = (AT_XPDS_Msg *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (atXpdsMsg == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)atXpdsMsg, WUEPS_PID_AT, AT_GetDestPid(atXpdsMsg->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    atXpdsMsg->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    atXpdsMsg->appCtrl.opId     = g_atClientTab[indexNum].opId;

    if (dataLen > 0) {
        memResult = memcpy_s(((VOS_UINT8 *)atXpdsMsg) + VOS_MSG_HEAD_LENGTH + sizeof(VOS_UINT32) + sizeof(AT_APPCTRL),
                             dataLen, g_atParaList[1].para, g_atParaList[1].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, dataLen, g_atParaList[1].paraLen);
    }

    atXpdsMsg->msgId = msgType;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, atXpdsMsg) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCagpsCfgPosMode(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_CFG_POSITION_MODE_REQ, sizeof(AT_XPDS_GpsCfgPositionModeReq));

    if (result == AT_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAGPSCFGPOSMODE_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return result;
}


VOS_UINT32 AT_SetCagpsStart(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_START_REQ, sizeof(AT_XPDS_GpsStartReq));

    if (result == AT_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAGPSSTART_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return result;
}


VOS_UINT32 AT_SetCagpsStop(VOS_UINT8 indexNum)
{
    AT_XPDS_GpsStopReq *msgStopReq = VOS_NULL_PTR;
    VOS_UINT32          msgLength;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    msgLength = sizeof(AT_XPDS_GpsStopReq) - VOS_MSG_HEAD_LENGTH;

    msgStopReq = (AT_XPDS_GpsStopReq *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (msgStopReq == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgStopReq, WUEPS_PID_AT, AT_GetDestPid(msgStopReq->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    msgStopReq->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgStopReq->appCtrl.opId     = g_atClientTab[indexNum].opId;

    msgStopReq->msgId = ID_AT_XPDS_GPS_STOP_REQ;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgStopReq) == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAGPSSTOP_SET;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCagpsCfgMpcAddr(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_CFG_MPC_ADDR_REQ, sizeof(AT_XPDS_GpsCfgMpcAddrReq));

    if (result == AT_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAGPSCFGMPCADDR_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return result;
}


VOS_UINT32 AT_SetCagpsCfgPdeAddr(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_CFG_PDE_ADDR_REQ, sizeof(AT_XPDS_GpsCfgPdeAddrReq));

    if (result == AT_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAGPSCFGPDEADDR_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return result;
}


VOS_UINT32 AT_SetCagpsQryRefloc(VOS_UINT8 indexNum)
{
    VOS_UINT32                   msgLength;
    AT_XPDS_GpsQryReflocInfoReq *msgQryRefLoc = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    msgLength = sizeof(AT_XPDS_GpsQryReflocInfoReq) - VOS_MSG_HEAD_LENGTH;

    msgQryRefLoc = (AT_XPDS_GpsQryReflocInfoReq *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (msgQryRefLoc == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgQryRefLoc, WUEPS_PID_AT,
                  AT_GetDestPid(msgQryRefLoc->appCtrl.clientId, I0_UEPS_PID_XPDS), msgLength);

    msgQryRefLoc->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgQryRefLoc->appCtrl.opId     = g_atClientTab[indexNum].opId;
    msgQryRefLoc->msgId            = ID_AT_XPDS_GPS_QRY_REFLOC_INFO_REQ;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgQryRefLoc) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCagpsQryTime(VOS_UINT8 indexNum)
{
    AT_XPDS_GpsQryTimeInfoReq *msgQryTime = VOS_NULL_PTR;
    VOS_UINT32                 msgLength;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_CAGPSQRYTIME_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将字符串转换为16进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].para,
                              &g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("AT_SetCagpsQryTime: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 消息内容长度是否正确 */
    if (sizeof(VOS_UINT32) != g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].paraLen) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 按照方案要求，1:reply time sync info；2:reply time sync info and GPS assist data */
    if ((g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].para[0] == 0) ||
        (g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].para[0] >= AT_XPDS_REPLY_SYNC_BUTT)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    msgLength = sizeof(AT_XPDS_GpsQryTimeInfoReq) - VOS_MSG_HEAD_LENGTH;

    msgQryTime = (AT_XPDS_GpsQryTimeInfoReq *)PS_ALLOC_MSG(WUEPS_PID_AT, msgLength);

    if (msgQryTime == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgQryTime, WUEPS_PID_AT, AT_GetDestPid(msgQryTime->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    msgQryTime->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgQryTime->appCtrl.opId     = g_atClientTab[indexNum].opId;

    msgQryTime->actionType = g_atParaList[AT_CAGPSQRYTIME_PARA_DATA].para[0];
    msgQryTime->msgId = ID_AT_XPDS_GPS_QRY_TIME_INFO_REQ;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgQryTime) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCagpsPrmInfo(VOS_UINT8 indexNum)
{
    return AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_PRM_INFO_RSP, sizeof(AT_XPDS_GpsPrmInfoRsp));
}


VOS_UINT32 AT_SetCagpsReplyNiReq(VOS_UINT8 indexNum)
{
    return AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_REPLY_NI_REQ, sizeof(AT_XPDS_GpsReplyNiRsp));
}


VOS_CHAR* AT_SearchCagpsATCmd(AT_XPDS_MsgTypeUint32 msgType)
{
    VOS_UINT32 i;

    for (i = 0; i < sizeof(g_atCagpsCmdNameTlb) / sizeof(AT_CagpsCmdNameTlb); i++) {
        if (msgType == g_atCagpsCmdNameTlb[i].msgType) {
            return g_atCagpsCmdNameTlb[i].pcATCmd;
        }
    }

    return "UNKOWN-MSG";
}


VOS_UINT32 At_SetCagpsPosInfo(VOS_UINT8 indexNum)
{
    if (AT_CagpsSndXpdsReq(indexNum, ID_AT_XPDS_GPS_POS_INFO_RSP, sizeof(AT_XPDS_GpsPosInfoRsp)) == AT_OK) {
        return AT_OK;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_RcvXpdsCagpsCnf(struct MsgCB *msg)
{
    AT_XPDS_Msg *rcvMsg = VOS_NULL_PTR;
    VOS_UINT8    indexNum;
    VOS_UINT32   contentLen;
    VOS_UINT16   length = 0;

    /* 初始化 */
    rcvMsg   = (AT_XPDS_Msg *)msg;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsCagpsCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, AT_SearchCagpsATCmd(rcvMsg->msgId));

    /* 获取消息内容长度 */
    contentLen = VOS_GET_MSG_LEN(rcvMsg) - (sizeof(rcvMsg->msgId) + sizeof(AT_APPCTRL));

    if (contentLen != 0) {
        /* <length>, */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ": %d,\"", contentLen * AT_ASCII_AND_HEX_CONVERSION_FACTOR);

        /* <command>, */
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length, rcvMsg->content,
                                                      (TAF_UINT16)contentLen);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


AT_CmdCurrentOpt AT_SearchCagpsATCmdOpt(AT_XPDS_MsgTypeUint32 msgType)
{
    VOS_UINT32 i;

    for (i = 0; i < sizeof(g_atCagpsCmdOptTbl) / sizeof(AT_CagpsCmdOptTlb); i++) {
        if (msgType == g_atCagpsCmdOptTbl[i].msgType) {
            return g_atCagpsCmdOptTbl[i].cmdOpt;
        }
    }

    return AT_CMD_CURRENT_OPT_BUTT;
}


VOS_UINT32 AT_RcvXpdsCagpsRlstCnf(struct MsgCB *msg)
{
    XPDS_AT_ResultCnf *msgRsltCnf = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum;
    AT_CmdCurrentOpt   cmdOpt;

    /* 初始化 */
    msgRsltCnf = (XPDS_AT_ResultCnf *)msg;
    result     = AT_OK;
    indexNum   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgRsltCnf->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsCagpsRlstCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    cmdOpt = AT_SearchCagpsATCmdOpt(msgRsltCnf->msgId);

    /* 当前AT是否在等待该命令返回 */
    if (cmdOpt != g_atClientTab[indexNum].cmdCurrentOpt) {
        AT_WARN_LOG("AT_RcvXpdsCagpsRlstCnf : Current Option is not correct.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (msgRsltCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvXpdsEphInfoInd(struct MsgCB *msg)
{
    VOS_UINT8              indexNum;
    VOS_UINT16             atCmdLength;
    VOS_UINT32             ephDataStrLen;
    VOS_UINT32             ephDataByteOffset;
    VOS_UINT8              loop;
    XPDS_AT_GpsEphInfoInd *msgEphInfoInd = VOS_NULL_PTR;
    VOS_UINT8             *ephData       = VOS_NULL_PTR;

    /* 初始化 */
    msgEphInfoInd = (XPDS_AT_GpsEphInfoInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgEphInfoInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsEphInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvXpdsEphInfoInd: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    ephData = ((VOS_UINT8 *)(msgEphInfoInd)) + VOS_MSG_HEAD_LENGTH + sizeof(VOS_UINT32) + sizeof(AT_APPCTRL);

    ephDataStrLen = AT_AGPS_EPH_INFO_FIRST_SEG_STR_LEN;

    /* 第1包数据 */
    atCmdLength = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSEPHINFO: %d,%d,%d,", g_atCrLf, AT_AGPS_EPH_INFO_MAX_SEG_NUM, 0,
        ephDataStrLen);

    /* 第1包数据 */
    /* 将16进制数转换为ASCII码后输入主动命令内容 */
    /* 第1包数据字节长度为8 / 2 = 4 */
    atCmdLength += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                            ((TAF_UINT8 *)g_atSndCodeAddress) + atCmdLength,
                                                            ephDataStrLen / 2, ephData);

    atCmdLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + atCmdLength, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, atCmdLength);

    ephDataByteOffset = ephDataStrLen / 2;

    /* 从第2包到第5包的数据 */
    /* 第2包到第5包数据的字节长度均为960 / 2 = 480 */
    for (loop = 0; loop < AT_AGPS_EPH_INFO_MAX_SEG_NUM - 1; loop++) {
        ephDataStrLen = AT_AGPS_EPH_INFO_NOT_FIRST_SEG_STR_LEN;

        atCmdLength = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSEPHINFO: %d,%d,%d,", g_atCrLf, AT_AGPS_EPH_INFO_MAX_SEG_NUM,
            loop + 1, ephDataStrLen);

        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        atCmdLength += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                                ((TAF_UINT8 *)g_atSndCodeAddress) + atCmdLength,
                                                                ephDataStrLen / 2, ephData + ephDataByteOffset);

        atCmdLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + atCmdLength, "%s", g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, atCmdLength);

        ephDataByteOffset += ephDataStrLen / 2;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvXpdsAlmInfoInd(struct MsgCB *msg)
{
    VOS_UINT8              indexNum;
    VOS_UINT16             atCmdLength;
    VOS_UINT32             almDataStrLen;
    VOS_UINT32             almDataByteOffset;
    VOS_UINT8              loop;
    XPDS_AT_GpsAlmInfoInd *msgAlmInfoInd = VOS_NULL_PTR;
    VOS_UINT8             *almData       = VOS_NULL_PTR;

    /* 初始化 */
    msgAlmInfoInd = (XPDS_AT_GpsAlmInfoInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msgAlmInfoInd->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXpdsAlmInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvXpdsAlmInfoInd: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    almData = ((VOS_UINT8 *)(msgAlmInfoInd)) + VOS_MSG_HEAD_LENGTH + sizeof(VOS_UINT32) + sizeof(AT_APPCTRL);

    almDataStrLen = AT_AGPS_ALM_INFO_FIRST_SEG_STR_LEN;

    /* 第1包数据 */
    atCmdLength = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSALMINFO: %d,%d,%d,", g_atCrLf, AT_AGPS_ALM_INFO_MAX_SEG_NUM, 0,
        almDataStrLen);

    /* 第1包数据 */
    /* 将16进制数转换为ASCII码后输入主动命令内容 */
    /* 第1包数据字节长度为8 / 2 = 4 */
    atCmdLength += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                            ((TAF_UINT8 *)g_atSndCodeAddress) + atCmdLength,
                                                            almDataStrLen / 2, almData);

    atCmdLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + atCmdLength, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, atCmdLength);

    almDataByteOffset = almDataStrLen / 2;

    /* 从第2包到第3包的数据 */
    /* 第2包数据和第3包数据的字节长度为896 / 2 = 448 */
    for (loop = 0; loop < AT_AGPS_ALM_INFO_MAX_SEG_NUM - 1; loop++) {
        almDataStrLen = AT_AGPS_ALM_INFO_NOT_FIRST_SEG_STR_LEN;

        atCmdLength = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s^CAGPSALMINFO: %d,%d,%d,", g_atCrLf, AT_AGPS_ALM_INFO_MAX_SEG_NUM,
            loop + 1, almDataStrLen);

        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        atCmdLength += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                                ((TAF_UINT8 *)g_atSndCodeAddress) + atCmdLength,
                                                                almDataStrLen / 2, almData + almDataByteOffset);

        atCmdLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + atCmdLength, "%s", g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, atCmdLength);

        almDataByteOffset += almDataStrLen / 2;
    }

    return VOS_OK;
}


VOS_UINT32 At_SetCgpsControlStart(VOS_UINT8 indexNum)
{
    AT_XPDS_Msg *msgStr = VOS_NULL_PTR;
    VOS_UINT32   msgLength;
    errno_t      memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_CGPSCONTROLSTART_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串长度不正确 */
    if (g_atParaList[AT_CGPSCONTROLSTART_DATA_LEN].paraValue != g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将字符串转换为16进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[AT_CGPSCONTROLSTART_DATA].para,
                              &g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_SetgpsControlStart: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen >= AT_CGPSCONTROLSTART_DATA_MAX_LEN) {
        msgLength = sizeof(AT_XPDS_Msg) + g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen -
                    VOS_MSG_HEAD_LENGTH - AT_CGPSCONTROLSTART_DATA_MAX_LEN;
    } else {
        msgLength = sizeof(AT_XPDS_Msg) - VOS_MSG_HEAD_LENGTH;
    }

    msgStr = (AT_XPDS_Msg *)PS_ALLOC_MSG(UEPS_PID_XPDS, msgLength);

    if (msgStr == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgStr, WUEPS_PID_AT, AT_GetDestPid(msgStr->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    msgStr->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgStr->appCtrl.opId     = g_atClientTab[indexNum].opId;

    if (g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen > 0) {
        memResult = memcpy_s(msgStr->content, g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen,
                             g_atParaList[AT_CGPSCONTROLSTART_DATA].para,
                             g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen,
                            g_atParaList[AT_CGPSCONTROLSTART_DATA].paraLen);
    }

    msgStr->msgId = ID_AT_XPDS_UTS_TEST_START_REQ;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgStr) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetCgpsControlStop(VOS_UINT8 indexNum)
{
    AT_XPDS_Msg *msgStr = VOS_NULL_PTR;
    VOS_UINT32   msgLength;
    errno_t      memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_CGPSCONTROLSTOP_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串长度不正确 */
    if (g_atParaList[AT_CGPSCONTROLSTOP_DATA_LEN].paraValue != g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将字符串转换为16进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[AT_CGPSCONTROLSTOP_DATA].para,
                              &g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_SetCgpsControlStop: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen >= AT_CGPSCONTROLSTOP_DATA_MAX_LEN) {
        msgLength = sizeof(AT_XPDS_Msg) + g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen - VOS_MSG_HEAD_LENGTH -
                    AT_CGPSCONTROLSTOP_DATA_MAX_LEN;
    } else {
        msgLength = sizeof(AT_XPDS_Msg) - VOS_MSG_HEAD_LENGTH;
    }

    msgStr = (AT_XPDS_Msg *)PS_ALLOC_MSG(UEPS_PID_XPDS, msgLength);

    if (msgStr == VOS_NULL_PTR) {
        return AT_ERROR;
    }

    TAF_CfgMsgHdr((MsgBlock *)msgStr, UEPS_PID_XPDS, AT_GetDestPid(msgStr->appCtrl.clientId, I0_UEPS_PID_XPDS),
                  msgLength);

    msgStr->appCtrl.clientId = g_atClientTab[indexNum].clientId;
    msgStr->appCtrl.opId     = g_atClientTab[indexNum].opId;

    if (g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen > 0) {
        memResult = memcpy_s(msgStr->content, g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen,
                             g_atParaList[AT_CGPSCONTROLSTOP_DATA].para,
                             g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen,
                            g_atParaList[AT_CGPSCONTROLSTOP_DATA].paraLen);
    }

    msgStr->msgId = ID_AT_XPDS_UTS_TEST_STOP_REQ;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msgStr) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}

#endif

