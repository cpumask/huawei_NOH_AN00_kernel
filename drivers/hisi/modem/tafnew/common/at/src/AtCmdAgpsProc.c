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

#include "AtCmdAgpsProc.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "AtMsgPrint.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_AGPS_PROC_C

#define AT_EPDU_PARA_TRANSACTIONID 0 /* EPDU的第一个参数TRANSACTIONID */
#define AT_EPDU_PARA_MSG_TYPE 1      /* EPDU的第二个参数MSG_TYPE */
#define AT_EPDU_PARA_CIEVFLG 2       /* EPDU的第三个参数CIEVFLG */
#define AT_EPDU_PARA_ENDFLAG 3       /* EPDU的第四个参数ENDFLAG */
#define AT_EPDU_PARA_LOCSOURCE 4     /* EPDU的第五个参数LOCSOURCE */
#define AT_EPDU_PARA_LOCCALCERR 5    /* EPDU的第六个参数LOCCALCERR */
#define AT_EPDU_PARA_ID 6            /* EPDU的第七个参数ID */
#define AT_EPDU_PARA_NAME 7          /* EPDU的第八个参数NAME */
#define AT_EPDU_PARA_TOTAL 8         /* EPDU的第九个参数TOTAL */
#define AT_EPDU_PARA_INDEX 9         /* EPDU的第十个参数INDEX */
#define AT_EPDU_PARA_DATA 10         /* EPDU的第十一个参数DATA */


VOS_UINT32 At_SetEpduPara(VOS_UINT8 indexNum)
{
    errno_t           memResult;
    AT_MTA_EpduSetReq atCmd;
    VOS_UINT32        rst;
    VOS_UINT16        epduLen;

    /* 参数检查 */
    if (g_atParaIndex != AT_CMD_EPDU_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_EPDU_PARA_NAME].paraLen > AT_MTA_EPDU_NAME_LENGTH_MAX) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_EPDU_PARA_DATA].paraLen > AT_MTA_EPDU_CONTENT_STRING_LEN_MAX) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*
     * g_atParaList[8].ulParaValue表示total值； g_atParaList[9].ulParaValue表示index值
     * 如果total值小于index值直接返回
     */
    if (g_atParaList[AT_EPDU_PARA_TOTAL].paraValue < g_atParaList[AT_EPDU_PARA_INDEX].paraValue) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    epduLen = 0;

    /* AT发送至MTA的消息结构赋值 */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_EpduSetReq));
    atCmd.transactionId    = (VOS_UINT16)g_atParaList[AT_EPDU_PARA_TRANSACTIONID].paraValue;
    atCmd.msgBodyType      = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_MSG_TYPE].paraValue;
    atCmd.commonIeValidFlg = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_CIEVFLG].paraValue;
    atCmd.endFlag          = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_ENDFLAG].paraValue;
    atCmd.locSource        = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_LOCSOURCE].paraValue;
    atCmd.locCalcErr       = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_LOCCALCERR].paraValue;
    atCmd.id               = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_ID].paraValue;
    atCmd.totalNum         = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_TOTAL].paraValue;
    atCmd.index            = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_INDEX].paraValue;

    if (g_atParaList[AT_EPDU_PARA_NAME].paraLen != 0) {
        atCmd.nameLength = (VOS_UINT8)g_atParaList[AT_EPDU_PARA_NAME].paraLen;
        memResult = memcpy_s(atCmd.name, AT_MTA_EPDU_NAME_LENGTH_MAX, g_atParaList[AT_EPDU_PARA_NAME].para,
                             g_atParaList[AT_EPDU_PARA_NAME].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_MTA_EPDU_NAME_LENGTH_MAX, g_atParaList[AT_EPDU_PARA_NAME].paraLen);
    }

    if (g_atParaList[AT_EPDU_PARA_DATA].paraLen != 0) {
        epduLen = g_atParaList[AT_EPDU_PARA_DATA].paraLen;

        if (At_AsciiNum2HexString(g_atParaList[AT_EPDU_PARA_DATA].para, &epduLen) == AT_FAILURE) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        atCmd.epduLength = epduLen;

        memResult = memcpy_s(atCmd.epduContent, AT_MTA_EPDU_CONTENT_LENGTH_MAX,
                             g_atParaList[AT_EPDU_PARA_DATA].para, epduLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_MTA_EPDU_CONTENT_LENGTH_MAX, epduLen);
    }

    /* 发送消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, ID_AT_MTA_EPDU_SET_REQ,
                                 (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_EpduSetReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EPDU_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_RcvMtaEpduDataInd(struct MsgCB *msg)
{
    errno_t             memResult;
    VOS_UINT8          *name             = VOS_NULL_PTR;
    AT_MTA_Msg         *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_EpduDataInd *mtaAtEpduDataInd = VOS_NULL_PTR;
    VOS_UINT32          i;
    VOS_UINT16          length;
    VOS_UINT8           indexNum;
    VOS_UINT32          dataSmallLen = 0;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    mtaAtEpduDataInd = (MTA_AT_EpduDataInd *)rcvMsg->content;
    indexNum         = 0;
    length           = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEpduDataInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 打印^EPDUR: */
    /* transaction_id, msg_type, common_info_valid_flg, end_flag, id */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,", g_atCrLf, g_atStringTab[AT_STRING_EPDUR].text,
        mtaAtEpduDataInd->transactionId, mtaAtEpduDataInd->msgBodyType, mtaAtEpduDataInd->commonIeValidFlg,
        mtaAtEpduDataInd->endFlg, mtaAtEpduDataInd->id);

    /* name */
    if (mtaAtEpduDataInd->nameLength > 0) {
        name = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, mtaAtEpduDataInd->nameLength + 1);

        /* 分配内存失败，直接返回 */
        if (name == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_RcvMtaEpduDataInd: name, Alloc mem fail");

            return VOS_ERR;
        }
        (VOS_VOID)memset_s(name, mtaAtEpduDataInd->nameLength + 1, 0x00, mtaAtEpduDataInd->nameLength + 1);

        memResult = memcpy_s(name, mtaAtEpduDataInd->nameLength, mtaAtEpduDataInd->name,
                             mtaAtEpduDataInd->nameLength);
        TAF_MEM_CHK_RTN_VAL(memResult, mtaAtEpduDataInd->nameLength, mtaAtEpduDataInd->nameLength);

        name[mtaAtEpduDataInd->nameLength] = '\0';

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", name);

        PS_MEM_FREE(WUEPS_PID_AT, name);
    }

    /* total, index */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,", mtaAtEpduDataInd->total, mtaAtEpduDataInd->index);
    dataSmallLen = (VOS_UINT32)TAF_MIN(mtaAtEpduDataInd->dataLength, MTA_MAX_EPDU_BODY_LEN);
    for (i = 0; i < dataSmallLen; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mtaAtEpduDataInd->data[i]);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEpduSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg        *rcvMsg     = VOS_NULL_PTR;
    MTA_AT_EpduSetCnf *epduSetCnf = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT32         result;

    /* 初始化 */
    rcvMsg     = (AT_MTA_Msg *)msg;
    epduSetCnf = (MTA_AT_EpduSetCnf *)rcvMsg->content;
    indexNum   = 0;
    result     = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEpduSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaEpduSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EPDU_SET) {
        AT_WARN_LOG("AT_RcvMtaEpduSetCnf : Current Option is not AT_CMD_EPDU_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (epduSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

