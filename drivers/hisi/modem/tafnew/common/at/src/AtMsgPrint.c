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
#include "AtMsgPrint.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "product_config.h"
#include "AtCheckFunc.h"
#include "AtCmdSmsProc.h"



#define THIS_FILE_ID PS_FILE_ID_AT_MSG_PRINT_C
#define AT_BCD_NUM_ARRAY_LEN 2
#define AT_ABSOLUTE_TIME_FORMAT_LEN 22
#define AT_SEND_DATA_BUFF_OFFSET_1 1
#define AT_ABSOLUTE_TIME_YEAR_VALID_LEN 2
#define AT_ABSOLUTE_TIME_MONTH_VALID_LEN 2
#define AT_ABSOLUTE_TIME_DAY_VALID_LEN 2
#define AT_ABSOLUTE_TIME_HOUR_VALID_LEN 2
#define AT_ABSOLUTE_TIME_MIN_VALID_LEN 2
#define AT_ABSOLUTE_TIME_SEC_VALID_LEN 2
#define AT_ABSOLUTE_TIME_TIMEZONE_VALID_LEN 2
#define AT_RELATIVE_VALID_PERIOD_MAX_VALUE 255
#define AT_ONE_HOUR_TIMEZONE_VALUE 4

/*
 * 3类型定义
 */


TAF_UINT32 AT_StubSendAutoReplyMsg(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo)
{
    MN_MSG_SendParm    sendMsg;
    MN_MSG_TsDataInfo *tsSubmitInfo = VOS_NULL_PTR;
    MN_MSG_Submit     *submit       = VOS_NULL_PTR;
    TAF_UINT32         ret;
    errno_t            memResult;

    /* 1. 为自动回复消息SUBMIT申请内存并情况 */
    tsSubmitInfo = (MN_MSG_TsDataInfo *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_MSG_TsDataInfo));
    if (tsSubmitInfo == VOS_NULL_PTR) {
        return AT_ERROR;
    }
    (VOS_VOID)memset_s(tsSubmitInfo, sizeof(MN_MSG_TsDataInfo), 0x00, sizeof(MN_MSG_TsDataInfo));

    /* 2. 为自动回复消息SUBMIT填写TPDU数据内容 */
    tsSubmitInfo->tpduType = MN_MSG_TPDU_SUBMIT;
    submit                 = (MN_MSG_Submit *)&tsSubmitInfo->u.submit;
    memResult              = memcpy_s(&submit->destAddr, sizeof(submit->destAddr), &tsDataInfo->u.deliver.origAddr,
                                      sizeof(tsDataInfo->u.deliver.origAddr));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(submit->destAddr), sizeof(tsDataInfo->u.deliver.origAddr));
    memResult = memcpy_s(&submit->dcs, sizeof(submit->dcs), &tsDataInfo->u.deliver.dcs,
            sizeof(tsDataInfo->u.deliver.dcs));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(submit->dcs), sizeof(tsDataInfo->u.deliver.dcs));
    submit->validPeriod.validPeriod = MN_MSG_VALID_PERIOD_NONE;

    /* 3. 为自动回复消息SUBMIT编码 */
    ret = MN_MSG_Encode(tsSubmitInfo, &sendMsg.msgInfo.tsRawData);
    if (ret != MN_ERR_NO_ERROR) {
        /*lint -save -e830 */
        PS_MEM_FREE(WUEPS_PID_AT, tsSubmitInfo);
        /*lint -restore */
        return AT_ERROR;
    }

    /* 4. 填写回复消息的短信中心, 存储设备，消息类型和发送域 */
    sendMsg.memStore                   = MN_MSG_MEM_STORE_NONE;
    sendMsg.clientType                 = MN_MSG_CLIENT_NORMAL;
    sendMsg.msgInfo.tsRawData.tpduType = MN_MSG_TPDU_SUBMIT;
    memResult = memcpy_s(&sendMsg.msgInfo.scAddr, sizeof(sendMsg.msgInfo.scAddr),
                         &event->u.deliverInfo.rcvMsgInfo.scAddr, sizeof(event->u.deliverInfo.rcvMsgInfo.scAddr));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sendMsg.msgInfo.scAddr), sizeof(event->u.deliverInfo.rcvMsgInfo.scAddr));

    /* 5. 发送回复消息 */
    ret = MN_MSG_Send(indexNum, 0, &sendMsg);
    if (ret != MN_ERR_NO_ERROR) {
        ret = AT_ERROR;
    } else {
        ret = AT_OK;
    }
    PS_MEM_FREE(WUEPS_PID_AT, tsSubmitInfo);

    return ret;
}


TAF_VOID AT_StubClearSpecificAutoRelyMsg(VOS_UINT8 clientIndex, TAF_UINT32 bufferIndex)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(clientIndex);

    if (smsCtx->smsMtBuffer[bufferIndex].event != VOS_NULL_PTR) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, smsCtx->smsMtBuffer[bufferIndex].event);
        smsCtx->smsMtBuffer[bufferIndex].event = VOS_NULL_PTR;
        /*lint -restore */
    }

    if (smsCtx->smsMtBuffer[bufferIndex].tsDataInfo != VOS_NULL_PTR) {
        PS_MEM_FREE(WUEPS_PID_AT, smsCtx->smsMtBuffer[bufferIndex].tsDataInfo);
        smsCtx->smsMtBuffer[bufferIndex].tsDataInfo = VOS_NULL_PTR;
    }

    return;
}


TAF_VOID AT_StubTriggerAutoReply(VOS_UINT8 indexNum, TAF_UINT8 cfgValue)
{
    TAF_UINT8       loop;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    smsCtx->smsAutoReply = cfgValue;

    /* 若关闭自动回复功能，则清空相关动态内存 */
    if (smsCtx->smsAutoReply == 0) {
        for (loop = 0; loop < AT_SMSMT_BUFFER_MAX; loop++) {
            AT_StubClearSpecificAutoRelyMsg(indexNum, loop);
            smsCtx->smsMtBuffer[loop].used = TAF_FALSE;
        }

        return;
    }

    /* 若已启用自动回复功能，按顺序回复接收到的短信 */
    for (loop = 0; loop < AT_SMSMT_BUFFER_MAX; loop++) {
        if (smsCtx->smsMtBuffer[loop].used != TAF_TRUE) {
            AT_StubClearSpecificAutoRelyMsg(indexNum, loop);
            continue;
        }

        if ((smsCtx->smsMtBuffer[loop].event == VOS_NULL_PTR) ||
            (smsCtx->smsMtBuffer[loop].tsDataInfo == VOS_NULL_PTR)) {
            AT_StubClearSpecificAutoRelyMsg(indexNum, loop);
            smsCtx->smsMtBuffer[loop].used = TAF_FALSE;
            continue;
        }

        AT_StubSendAutoReplyMsg(indexNum, smsCtx->smsMtBuffer[loop].event, smsCtx->smsMtBuffer[loop].tsDataInfo);
        AT_StubClearSpecificAutoRelyMsg(indexNum, loop);
        smsCtx->smsMtBuffer[loop].used = TAF_FALSE;
        break;
    }

    return;
}


TAF_VOID AT_StubSaveAutoReplyData(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo)
{
    errno_t         memResult;
    TAF_UINT8       loop;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* 自动回复功能未开启直接返回; */
    if (smsCtx->smsAutoReply == 0) {
        return;
    }

    /* 接收消息不是DELIVER短信或TP-RP没有置位直接返回 */
    if ((tsDataInfo->tpduType != MN_MSG_TPDU_DELIVER) || (tsDataInfo->u.deliver.replayPath != VOS_TRUE)) {
        return;
    }

    /* 申请并保存自动回复相关参数到缓存 */
    for (loop = 0; loop < AT_SMSMT_BUFFER_MAX; loop++) {
        if (smsCtx->smsMtBuffer[loop].used == TAF_TRUE) {
            continue;
        }

        AT_StubClearSpecificAutoRelyMsg(indexNum, loop);

        /* 记录接收短信信息记录到内存，用于 GCF测试用例34。2。8 */
        smsCtx->smsMtBuffer[loop].event = (MN_MSG_EventInfo *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_MSG_EventInfo));
        if (smsCtx->smsMtBuffer[loop].event == VOS_NULL_PTR) {
            AT_WARN_LOG("At_SmsDeliverProc: Fail to alloc memory.");
            return;
        }

        smsCtx->smsMtBuffer[loop].tsDataInfo = (MN_MSG_TsDataInfo *)PS_MEM_ALLOC(WUEPS_PID_AT,
                                                                                 sizeof(MN_MSG_TsDataInfo));
        if (smsCtx->smsMtBuffer[loop].tsDataInfo == VOS_NULL_PTR) {
            /*lint -save -e516 */
            PS_MEM_FREE(WUEPS_PID_AT, smsCtx->smsMtBuffer[loop].event);
            smsCtx->smsMtBuffer[loop].event = VOS_NULL_PTR;
            /*lint -restore */
            AT_WARN_LOG("At_SmsDeliverProc: Fail to alloc memory.");
            return;
        }

        memResult = memcpy_s(smsCtx->smsMtBuffer[loop].event, sizeof(MN_MSG_EventInfo), event,
                             sizeof(MN_MSG_EventInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_EventInfo), sizeof(MN_MSG_EventInfo));
        memResult = memcpy_s(smsCtx->smsMtBuffer[loop].tsDataInfo, sizeof(MN_MSG_TsDataInfo), tsDataInfo,
                             sizeof(MN_MSG_TsDataInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_TsDataInfo), sizeof(MN_MSG_TsDataInfo));

        smsCtx->smsMtBuffer[loop].used = TAF_TRUE;

        break;
    }

    return;
}


VOS_UINT32 At_ParseCsmpFo(VOS_UINT8 *fo)
{
    TAF_UINT32 ret;

    /* 检查<fo>,数字类型 */
    ret = At_CheckNumString(g_atParaList[0].para, g_atParaList[0].paraLen);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    /* 注意: g_atParaList[0].ulParaValue此时尚未转换，检查其它命令的这种情况 */
    ret = At_Auc2ul(g_atParaList[0].para, g_atParaList[0].paraLen, &g_atParaList[0].paraValue);
    if (ret == AT_FAILURE) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    /* 检查<fo>,一个字节 */
    if (g_atParaList[0].paraValue > 0xff) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    if (((0x03 & g_atParaList[0].paraValue) != 0x01) && ((0x03 & g_atParaList[0].paraValue) != 0x02)) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    *fo = (TAF_UINT8)g_atParaList[0].paraValue;

    return AT_SUCCESS;
}


TAF_UINT32 At_GetAbsoluteTime(TAF_UINT8 *timeStr, TAF_UINT16 timeStrLen, MN_MSG_Timestamp *absoluteTime)
{
    TAF_UINT32 ret;
    TAF_UINT8  bcdNum[AT_BCD_NUM_ARRAY_LEN];
    TAF_UINT32 tmp = 0;

    bcdNum[0] = 0;
    bcdNum[1] = 0;

    /*
     * 6th of May 1994, 22:10:00 GMT+2 "94/05/06,22:10:00+08"
     * 注意:还要判断中间字符是否合法
     */
    if ((timeStr == TAF_NULL_PTR) || (absoluteTime == TAF_NULL_PTR)) {
        AT_WARN_LOG("At_GetAbsoluteTime: parameter is NULL.");
        return AT_ERROR;
    }

    /* 检查<vp>,字符串类型，格式必须符合"yy/mm/dd,hh:mm:ss(+/-)tz,dst", 否则返回错误 */
    if ((timeStrLen != AT_ABSOLUTE_TIME_FORMAT_LEN) ||
        (timeStr[0] != '"') ||(timeStr[timeStrLen - 1] != '"') || /* '"' */
        (timeStr[3] != '/') ||                                    /* '/' */
        (timeStr[6] != '/') ||                                    /* '/' */
        (timeStr[9] != ',') ||                                    /* ',' */
        (timeStr[12] != ':') ||                                   /* ':' */
        (timeStr[15] != ':')) {                                   /* ':' */
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    /* Year */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[1], AT_ABSOLUTE_TIME_YEAR_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->year = AT_BCD_REVERSE(bcdNum[0]);

    /* Month */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[4], AT_ABSOLUTE_TIME_MONTH_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->month = AT_BCD_REVERSE(bcdNum[0]);

    /* Day */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[7], AT_ABSOLUTE_TIME_DAY_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->day = AT_BCD_REVERSE(bcdNum[0]);

    /* Hour */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[10], AT_ABSOLUTE_TIME_HOUR_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->hour = AT_BCD_REVERSE(bcdNum[0]);

    /* Minute */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[13], AT_ABSOLUTE_TIME_MIN_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->minute = AT_BCD_REVERSE(bcdNum[0]);

    /* Second */
    ret = At_AsciiNum2BcdNum(bcdNum, &timeStr[16], AT_ABSOLUTE_TIME_SEC_VALID_LEN);
    if (ret != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
    absoluteTime->second = AT_BCD_REVERSE(bcdNum[0]);
   /* timezone */
    if (At_Auc2ul(&timeStr[19], AT_ABSOLUTE_TIME_TIMEZONE_VALID_LEN, &tmp) == AT_FAILURE) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    if (((tmp % AT_ONE_HOUR_TIMEZONE_VALUE) != 0) || (tmp > AT_MAX_TIMEZONE_VALUE)) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    /* '+' 或者 '-' */
    switch (timeStr[18]) {
        case '+':
            absoluteTime->timezone = (TAF_INT8)tmp;
            break;

        case '-':
            absoluteTime->timezone = (TAF_INT8)((-1) * tmp);
            break;

        default:
            return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetAbsoluteValidPeriod(VOS_UINT8 indexNum, TAF_UINT8 *para, TAF_UINT16 paraLen,
                                     MN_MSG_ValidPeriod *validPeriod)
{
    errno_t         memResult;
    TAF_UINT32      ret;
    TAF_UINT8       dateInvalidType;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (paraLen == 0) {
        if (smsCtx->cscaCsmpInfo.vp.validPeriod == MN_MSG_VALID_PERIOD_ABSOLUTE) {
            memResult = memcpy_s(validPeriod, sizeof(MN_MSG_ValidPeriod), &(smsCtx->cscaCsmpInfo.vp),
                                 sizeof(MN_MSG_ValidPeriod));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_ValidPeriod), sizeof(MN_MSG_ValidPeriod));
            return AT_SUCCESS;
        } else {
            return AT_ERROR;
        }
    } else {
        ret = At_GetAbsoluteTime(para, paraLen, &validPeriod->u.absoluteTime);
        if (ret != AT_SUCCESS) {
            return ret;
        }

        ret = MN_MSG_ChkDate(&validPeriod->u.absoluteTime, &dateInvalidType);
        if (ret != MN_ERR_NO_ERROR) {
            return AT_CMS_OPERATION_NOT_ALLOWED;
        }

        return AT_SUCCESS;
    }
}


VOS_UINT32 AT_SetRelativeValidPeriod(VOS_UINT8 indexNum, TAF_UINT8 *para, TAF_UINT16 paraLen,
                                     MN_MSG_ValidPeriod *validPeriod)
{
    errno_t         memResult;
    TAF_UINT32      ret;
    TAF_UINT32      relativeValidPeriod;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (paraLen == 0) {
        if (smsCtx->cscaCsmpInfo.vp.validPeriod == MN_MSG_VALID_PERIOD_RELATIVE) {
            memResult = memcpy_s(validPeriod, sizeof(MN_MSG_ValidPeriod), &(smsCtx->cscaCsmpInfo.vp),
                                 sizeof(MN_MSG_ValidPeriod));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_ValidPeriod), sizeof(MN_MSG_ValidPeriod));
            return AT_SUCCESS;
        } else if (smsCtx->cscaCsmpInfo.vp.validPeriod == MN_MSG_VALID_PERIOD_NONE) {
            validPeriod->u.otherTime = AT_CSMP_SUBMIT_VP_DEFAULT_VALUE;
            return AT_SUCCESS;
        } else {
            return AT_ERROR;
        }
    } else {
        /* 检查<vp>,数字类型 */
        ret = At_CheckNumString(para, paraLen);
        if (ret != AT_SUCCESS) {
            return AT_CMS_OPERATION_NOT_ALLOWED;
        }

        /* 注意: g_atParaList[1].ulParaValue此时尚未转换，检查其它命令的这种情况 */
        ret = At_Auc2ul(para, paraLen, &relativeValidPeriod);
        if (ret == AT_FAILURE) {
            return AT_CMS_OPERATION_NOT_ALLOWED;
        }

        if (relativeValidPeriod > AT_RELATIVE_VALID_PERIOD_MAX_VALUE) {
            return AT_CMS_OPERATION_NOT_ALLOWED;
        }

        validPeriod->u.otherTime = (TAF_UINT8)relativeValidPeriod;
        return AT_SUCCESS;
    }
}


VOS_UINT32 At_ParseCsmpVp(VOS_UINT8 indexNum, MN_MSG_ValidPeriod *vp)
{
    errno_t         memResult;
    TAF_UINT32      ret;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* 获取当前配置的TP-VPF值，若用户不配置TP-VPF和TP-VP两项，则复制当前结构到临时结构并推出 */
    if (g_atParaList[0].paraLen != 0) {
        AT_GET_MSG_TP_VPF(vp->validPeriod, smsCtx->cscaCsmpInfo.tmpFo);
    } else if (g_atParaList[1].paraLen != 0) {
        vp->validPeriod = smsCtx->cscaCsmpInfo.vp.validPeriod;
    } else {
        memResult = memcpy_s(vp, sizeof(MN_MSG_ValidPeriod), &(smsCtx->cscaCsmpInfo.vp), sizeof(MN_MSG_ValidPeriod));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_ValidPeriod), sizeof(MN_MSG_ValidPeriod));
        return AT_SUCCESS;
    }

    /* 短信有效期类型设置为无效，<VP>参数项必须为空 */
    if (vp->validPeriod == MN_MSG_VALID_PERIOD_NONE) {
        if (g_atParaList[1].paraLen != 0) {
            return AT_ERROR;
        }
        (VOS_VOID)memset_s(vp, sizeof(MN_MSG_ValidPeriod), 0x00, sizeof(MN_MSG_ValidPeriod));
        return AT_SUCCESS;
    }
    /* 短信有效期类型设置为相对有效期， */
    else if (vp->validPeriod == MN_MSG_VALID_PERIOD_RELATIVE) {
        ret = AT_SetRelativeValidPeriod(indexNum, g_atParaList[1].para, g_atParaList[1].paraLen, vp);
        return ret;
    } else if (vp->validPeriod == MN_MSG_VALID_PERIOD_ABSOLUTE) {
        ret = AT_SetAbsoluteValidPeriod(indexNum, g_atParaList[1].para, g_atParaList[1].paraLen, vp);
        return ret;
    } else {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }
}


TAF_VOID At_MsgResultCodeFormat(TAF_UINT8 indexNum, TAF_UINT16 length)
{
    errno_t memResult;
    if (g_atVType == AT_V_ENTIRE_TYPE) {
        memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr,
                             AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET_1,
                             (TAF_CHAR *)g_atCrLf, AT_CRLF_STR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET_1,
                            AT_CRLF_STR_LEN);
        At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_CRLF_STR_LEN);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    return;
}


TAF_VOID At_GetMsgFoValue(MN_MSG_TsDataInfo *tsDataInfo, TAF_UINT8 *fo)
{
    TAF_UINT8 foTemp = 0;

    switch (tsDataInfo->tpduType) {
        case MN_MSG_TPDU_DELIVER:
            /* TP-MTI, TP-MMS, TP-RP, TP_UDHI, TP-SRI: */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_DELIVER);
            AT_SET_MSG_TP_MMS(foTemp, tsDataInfo->u.deliver.moreMsg);
            AT_SET_MSG_TP_RP(foTemp, tsDataInfo->u.deliver.replayPath);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.deliver.userDataHeaderInd);
            AT_SET_MSG_TP_SRI(foTemp, tsDataInfo->u.deliver.staRptInd);
            break;
        case MN_MSG_TPDU_DELIVER_RPT_ACK:
            /* TP MTI  TP-UDHI  */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_DELIVER_REPORT);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.deliverRptAck.userDataHeaderInd);
            break;
        case MN_MSG_TPDU_DELIVER_RPT_ERR:
            /* TP MTI  TP-UDHI  */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_DELIVER_REPORT);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.deliverRptErr.userDataHeaderInd);
            break;
        case MN_MSG_TPDU_STARPT:
            /* TP MTI TP UDHI TP MMS TP SRQ */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_STATUS_REPORT);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.staRpt.userDataHeaderInd);
            AT_SET_MSG_TP_MMS(foTemp, tsDataInfo->u.staRpt.moreMsg);
            AT_SET_MSG_TP_SRQ(foTemp, tsDataInfo->u.staRpt.staRptQualCommand); /* ?? */
            break;
        case MN_MSG_TPDU_SUBMIT:
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_SUBMIT);
            AT_SET_MSG_TP_RD(foTemp, tsDataInfo->u.submit.rejectDuplicates);
            AT_SET_MSG_TP_VPF(foTemp, tsDataInfo->u.submit.validPeriod.validPeriod);
            AT_SET_MSG_TP_RP(foTemp, tsDataInfo->u.submit.replayPath);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.submit.userDataHeaderInd);
            AT_SET_MSG_TP_SRR(foTemp, tsDataInfo->u.submit.staRptReq);
            break;
        case MN_MSG_TPDU_COMMAND:
            /* TP MTI TP UDHI TP SRR */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_COMMAND);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.command.userDataHeaderInd);
            AT_SET_MSG_TP_SRR(foTemp, tsDataInfo->u.command.staRptReq);
            break;
        case MN_MSG_TPDU_SUBMIT_RPT_ACK:
            /* TP MTI  TP-UDHI  */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_SUBMIT_REPORT);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.submitRptAck.userDataHeaderInd);
            break;
        case MN_MSG_TPDU_SUBMIT_RPT_ERR:
            /* TP MTI  TP-UDHI  */
            AT_SET_MSG_TP_MTI(foTemp, AT_MSG_TP_MTI_SUBMIT_REPORT);
            AT_SET_MSG_TP_UDHI(foTemp, tsDataInfo->u.submitRptErr.userDataHeaderInd);
            break;
        default:
            AT_NORM_LOG("At_GetMsgFoValue: invalid TPDU type.");
            break;
    }

    *fo = foTemp;
    return;
}


VOS_VOID At_SendMsgFoAttr(VOS_UINT8 indexNum, MN_MSG_TsDataInfo *tsDataInfo)
{
    TAF_UINT8       fo     = 0;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGC_TEXT_SET) {
        fo = g_atClientTab[indexNum].atSmsData.fo;
    } else {
        /* 判断FO的有效性 */
        if (smsCtx->cscaCsmpInfo.foUsed == TAF_TRUE) {
            fo = smsCtx->cscaCsmpInfo.fo;
        } else {
            if ((tsDataInfo->tpduType == MN_MSG_TPDU_DELIVER) || (tsDataInfo->tpduType == MN_MSG_TPDU_SUBMIT)) {
                fo = AT_CSMP_FO_DEFAULT_VALUE1;
            } else if ((tsDataInfo->tpduType == MN_MSG_TPDU_STARPT) || (tsDataInfo->tpduType == MN_MSG_TPDU_COMMAND)) {
                fo = AT_CSMP_FO_DEFAULT_VALUE2;
            } else {
                AT_NORM_LOG("At_SendMsgFoAttr: invalid enTpduType.");
                return;
            }
        }
    }

    switch (tsDataInfo->tpduType) {
        case MN_MSG_TPDU_COMMAND:
            /* TP MTI TP UDHI TP SRR */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.command.userDataHeaderInd, fo);
            AT_GET_MSG_TP_SRR(tsDataInfo->u.command.staRptReq, fo);
            break;

        case MN_MSG_TPDU_DELIVER:
            /* TP-MTI, TP-MMS, TP-RP, TP_UDHI, TP-SRI: */
            /* decode fo:TP MTI TP MMS TP RP TP UDHI TP SRI */
            AT_GET_MSG_TP_MMS(tsDataInfo->u.deliver.moreMsg, fo);
            AT_GET_MSG_TP_RP(tsDataInfo->u.deliver.replayPath, fo);
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.deliver.userDataHeaderInd, fo);
            AT_GET_MSG_TP_SRI(tsDataInfo->u.deliver.staRptInd, fo);
            break;

        case MN_MSG_TPDU_STARPT:
            /* TP MTI ignore TP UDHI TP MMS TP SRQ */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.staRpt.userDataHeaderInd, fo);
            AT_GET_MSG_TP_MMS(tsDataInfo->u.staRpt.moreMsg, fo);
            AT_GET_MSG_TP_SRQ(tsDataInfo->u.staRpt.staRptQualCommand, fo);
            break;

        case MN_MSG_TPDU_SUBMIT:
            AT_GET_MSG_TP_RD(tsDataInfo->u.submit.rejectDuplicates, fo);
            /* TP VPF 23040 9.2.3.3 */
            AT_GET_MSG_TP_VPF(tsDataInfo->u.submit.validPeriod.validPeriod, fo);
            /* TP RP  23040 9.2.3.17 */
            AT_GET_MSG_TP_RP(tsDataInfo->u.submit.replayPath, fo);
            /* TP UDHI23040 9.2.3.23 */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.submit.userDataHeaderInd, fo);
            /* TP SRR 23040 9.2.3.5 */
            AT_GET_MSG_TP_SRR(tsDataInfo->u.submit.staRptReq, fo);
            break;

        case MN_MSG_TPDU_DELIVER_RPT_ACK:
            /* TP MTI ignore  TP-UDHI  */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.deliverRptAck.userDataHeaderInd, fo);
            break;

        case MN_MSG_TPDU_DELIVER_RPT_ERR:
            /* TP MTI ignore  TP-UDHI  */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.deliverRptErr.userDataHeaderInd, fo);
            break;

        case MN_MSG_TPDU_SUBMIT_RPT_ACK:
            /* TP MTI ignore TP-UDHI  */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.submitRptAck.userDataHeaderInd, fo);
            break;

        case MN_MSG_TPDU_SUBMIT_RPT_ERR:
            /* TP MTI ignore TP-UDHI  */
            AT_GET_MSG_TP_UDHI(tsDataInfo->u.submitRptErr.userDataHeaderInd, fo);
            break;

        default:
            AT_NORM_LOG("At_SendMsgFoAttr: invalid pdu type.");
            break;
    }
    return;
}


TAF_VOID At_PrintCsmsInfo(TAF_UINT8 indexNum)
{
    AT_MSG_Serv msgServInfo = { AT_MSG_SERV_STATE_SUPPORT, AT_MSG_SERV_STATE_SUPPORT, AT_MSG_SERV_STATE_SUPPORT };

    g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "%d,%d,%d", msgServInfo.smsMT, msgServInfo.smsMO,
        msgServInfo.smsBM);
    return;
}


TAF_UINT16 At_PrintAsciiAddr(MN_MSG_AsciiAddr *addr, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)dst + length, "\"");
    if ((addr->len > 0) && (addr->len <= MN_MAX_ASCII_ADDRESS_NUM)) {
        addr->asciiNum[addr->len] = 0;

        if (addr->numType == MN_MSG_TON_INTERNATIONAL) {
            length += (TAF_UINT16)At_ReadNumTypePara((dst + length), (TAF_UINT8 *)"+");
        }

        length += (TAF_UINT16)At_ReadNumTypePara((dst + length), addr->asciiNum);
    }
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)dst + length, "\"");

    return length;
}


VOS_UINT32 AT_BcdAddrToAscii(MN_MSG_BcdAddr *bcdAddr, MN_MSG_AsciiAddr *asciiAddr)
{
    VOS_UINT32 ret;

    if ((bcdAddr == VOS_NULL_PTR) || (asciiAddr == VOS_NULL_PTR)) {
        AT_WARN_LOG("MN_MSG_BcdAddrToAscii: Parameter of the function is null.");
        return MN_ERR_NULLPTR;
    }

    if (bcdAddr->bcdLen > MN_MSG_MAX_BCD_NUM_LEN) {
        AT_WARN_LOG("AT_BcdAddrToAscii: length of BcdAddr ucBcdLen is invalid.");

        return MN_ERR_INVALIDPARM;
    }

    asciiAddr->numType = ((bcdAddr->addrType >> 4) & 0x07);
    asciiAddr->numPlan = (bcdAddr->addrType & 0x0f);
    if ((bcdAddr->bcdNum[bcdAddr->bcdLen - 1] & 0xF0) != 0xF0) {
        asciiAddr->len = (VOS_UINT32)bcdAddr->bcdLen * AT_DOUBLE_LENGTH;
    } else {
        asciiAddr->len = ((VOS_UINT32)bcdAddr->bcdLen * AT_DOUBLE_LENGTH) - 1;
    }

    if (asciiAddr->len > MN_MAX_ASCII_ADDRESS_NUM) {
        AT_WARN_LOG("MN_MSG_BcdAddrToAscii: length of number is invalid.");
        return MN_ERR_INVALIDPARM;
    }

    ret = AT_BcdNumberToAscii(bcdAddr->bcdNum, bcdAddr->bcdLen, (VOS_CHAR *)asciiAddr->asciiNum);
    if (ret == MN_ERR_NO_ERROR) {
        return MN_ERR_NO_ERROR;
    } else {
        return MN_ERR_INVALIDPARM;
    }
}


TAF_UINT16 At_PrintBcdAddr(MN_MSG_BcdAddr *bcdAddr, TAF_UINT8 *dst)
{
    TAF_UINT16       length;
    TAF_UINT32       ret;
    MN_MSG_AsciiAddr asciiAddr;

    (VOS_VOID)memset_s(&asciiAddr, sizeof(MN_MSG_AsciiAddr), 0x00, sizeof(asciiAddr));

    if (bcdAddr->bcdLen != 0) {
        ret = AT_BcdAddrToAscii(bcdAddr, &asciiAddr);
        if (ret != MN_ERR_NO_ERROR) {
            AT_WARN_LOG("At_PrintBcdAddr: Fail to convert BCD to ASCII.");
            return 0;
        }
    }

    length = At_PrintAsciiAddr(&asciiAddr, dst);
    return length;
}


TAF_UINT16 At_PrintAddrType(MN_MSG_AsciiAddr *addr, TAF_UINT8 *dst)
{
    TAF_UINT8  addrType;
    TAF_UINT16 length = 0;

    addrType = 0x80;
    addrType |= addr->numPlan;
    addrType |= ((addr->numType << 4) & 0x70);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(dst + length), "%d", addrType);

    return length;
}


TAF_UINT16 At_PrintMsgFo(MN_MSG_TsDataInfo *tsDataInfo, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;
    TAF_UINT8  fo     = 0;

    At_GetMsgFoValue(tsDataInfo, &fo);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)dst + length, "%d", fo);

    return length;
}


VOS_UINT32 AT_ChkSmsNumType(MN_MSG_TonUint8 numType)
{
    return MN_ERR_NO_ERROR;
}


VOS_UINT32 AT_ChkSmsNumPlan(MN_MSG_NpiUint8 numPlan)
{
    return MN_ERR_NO_ERROR;
}


VOS_UINT32 AT_AsciiToBcdCode(VOS_CHAR asciiCode, VOS_UINT8 *bcdCode)
{
    if (bcdCode == VOS_NULL_PTR) {
        AT_NORM_LOG("AT_AsciiToBcdCode: Parameter of the function is null.");
        return MN_ERR_NULLPTR;
    }

    if ((asciiCode >= '0') && (asciiCode <= '9')) {
        *bcdCode = (VOS_UINT8)(asciiCode - '0');
    } else if (asciiCode == '*') {
        *bcdCode = 0x0a;
    } else if (asciiCode == '#') {
        *bcdCode = 0x0b;
    } else if ((asciiCode == 'a') || (asciiCode == 'b') || (asciiCode == 'c')) {
        *bcdCode = (VOS_UINT8)((asciiCode - 'a') + 0x0c);
    } else if ((asciiCode == 'A') || (asciiCode == 'B') || (asciiCode == 'C')) {
        *bcdCode = (VOS_UINT8)((asciiCode - 'A') + 0x0c);
    }
    else if (asciiCode == '+') {
        return MN_ERR_PLUS_SIGN;
    }
    else {
        AT_NORM_LOG("AT_AsciiToBcdCode: Parameter of the function is invalid.");
        return MN_ERR_INVALID_ASCII;
    }

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 AT_AsciiNumberToBcd(const VOS_CHAR *pcAsciiNumber, VOS_UINT8 *bcdNumber, VOS_UINT8 *bcdLen)
{
    VOS_UINT8 inputLoop;
    VOS_UINT8 outputLoop;
    VOS_UINT8  bcdCode = 0;
    VOS_UINT32 ret;

    if ((pcAsciiNumber == TAF_NULL_PTR) || (bcdNumber == TAF_NULL_PTR) || (bcdLen == TAF_NULL_PTR)) {
        AT_NORM_LOG("AT_AsciiNumberToBcd: Parameter of the function is null.");
        return MN_ERR_NULLPTR;
    }

    for (inputLoop = 0, outputLoop = 0; pcAsciiNumber[inputLoop] != '\0'; inputLoop++) {
        ret = AT_AsciiToBcdCode(pcAsciiNumber[inputLoop], &bcdCode);

        /* AT_IsDCmdValidChar以前是将号码中的+滤掉 */
        /* 现在在这里滤掉，因为BCD码中没有对+的表示 */
        if ((ret == MN_ERR_INVALID_ASCII) || (ret == MN_ERR_NULLPTR)) {
            return ret;
        } else if (ret == MN_ERR_PLUS_SIGN) {
            continue;
        } else {
            /* for lint */
        }

        /* 将当前需要填入的空间清0 */
        /* 一个字节低四位和高四位各保留一位bcd数字码 */
        bcdNumber[(outputLoop / 2)] &= (VOS_UINT8)(((outputLoop % 2) == 1) ? 0x0F : 0xF0);

        /* 将数字填入相应的空间 */
        bcdNumber[(outputLoop / 2)] |= (((outputLoop % 2) == 1) ? ((bcdCode << 4) & 0xF0) : (bcdCode & 0x0F));

        outputLoop++;
    }

    /* 如果长度为奇数，则最后一个字符需要填 F */
    if ((outputLoop % 2) == 1) {
        bcdNumber[(outputLoop / 2)] |= 0xF0;
    }

    *bcdLen = (outputLoop + 1) / 2;

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 AT_BcdToAsciiCode(VOS_UINT8 bcdCode, VOS_CHAR *pcAsciiCode)
{
    VOS_CHAR asciiCode;

    if (pcAsciiCode == TAF_NULL_PTR) {
        AT_NORM_LOG("AT_BcdToAsciiCode: Parameter of the function is null.");
        return MN_ERR_NULLPTR;
    }

    if (bcdCode <= 0x09) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x30);
    } else if (bcdCode == 0x0A) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x20); /* 字符'*' */
    } else if (bcdCode == 0x0B) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x18); /* 字符'#' */
    } else if ((bcdCode == 0x0C) || (bcdCode == 0x0D) || (bcdCode == 0x0E)) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x55); /* 字符'a', 'b', 'c' */
    } else {
        AT_NORM_LOG("AT_BcdToAsciiCode: Parameter of the function is invalid.");
        return MN_ERR_INVALID_BCD;
    }

    *pcAsciiCode = asciiCode;

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 AT_BcdNumberToAscii(const VOS_UINT8 *bcdNumber, VOS_UINT8 bcdLen, VOS_CHAR *pcAsciiNumber)
{
    VOS_UINT8  loop;
    VOS_UINT8  len;
    VOS_UINT8  bcdCode;
    VOS_UINT32 ret;

    if ((bcdNumber == TAF_NULL_PTR) || (pcAsciiNumber == TAF_NULL_PTR)) {
        AT_NORM_LOG("AT_BcdNumberToAscii: Parameter of the function is null.");
        return MN_ERR_NULLPTR;
    }

    /* 整理号码字符串，去除无效的0XFF数据 */
    while (bcdLen > 1) {
        if (bcdNumber[bcdLen - 1] == 0xFF) {
            bcdLen--;
        } else {
            break;
        }
    }

    /*
     * 判断pucBcdAddress所指向的字符串的最后一个字节的高位是否为1111，
     * 如果是，说明号码位数为奇数，否则为偶数
     */
    if ((bcdNumber[bcdLen - 1] & 0xF0) == 0xF0) {
        len = (VOS_UINT8)((bcdLen * 2) - 1);
    } else {
        len = (VOS_UINT8)(bcdLen * 2);
    }

    /* 解析号码 */
    for (loop = 0; loop < len; loop++) {
        /* 判断当前解码的是奇数位号码还是偶数位号码，从0开始，是偶数 */
        if ((loop % 2) == 1) {
            /* 如果是奇数位号码，则取高4位的值 */
            bcdCode = ((bcdNumber[(loop / 2)] >> 4) & 0x0F);
        } else {
            /* 如果是偶数位号码，则取低4位的值 */
            bcdCode = (bcdNumber[(loop / 2)] & 0x0F);
        }

        /* 将二进制数字转换成Ascii码形式 */
        ret = AT_BcdToAsciiCode(bcdCode, &(pcAsciiNumber[loop]));
        if (ret != MN_ERR_NO_ERROR) {
            return ret;
        }
    }

    pcAsciiNumber[loop] = '\0'; /* 字符串末尾为0 */

    return MN_ERR_NO_ERROR;
}


TAF_UINT32 At_GetAsciiOrBcdAddr(TAF_UINT8 *addr, TAF_UINT16 addrLen, TAF_UINT8 addrType, TAF_UINT16 numTypeLen,
                                MN_MSG_AsciiAddr *asciiAddr, MN_MSG_BcdAddr *bcdAddr)
{
    TAF_UINT8 asciiNum[MN_MAX_ASCII_ADDRESS_NUM + 2]; /* array  of ASCII Num */
    /* 指向实际号码（不包括+号）的指针 */
    TAF_UINT8      *num = VOS_NULL_PTR;
    TAF_UINT32      asciiAddrLen;
    TAF_UINT32      ret;
    errno_t         memResult;
    MN_MSG_TonUint8 numType; /* type of number */
    MN_MSG_NpiUint8 numPlan; /* Numbering plan identification */

    if ((asciiAddr == TAF_NULL_PTR) && (bcdAddr == TAF_NULL_PTR)) {
        AT_WARN_LOG("At_GetAsciiOrBcdAddr: output parameter is null. ");
        return AT_ERROR;
    }

    if (asciiAddr != TAF_NULL_PTR) {
        (VOS_VOID)memset_s(asciiAddr, sizeof(MN_MSG_AsciiAddr), 0x00, sizeof(MN_MSG_AsciiAddr));
    }

    if (bcdAddr != TAF_NULL_PTR) {
        (VOS_VOID)memset_s(bcdAddr, sizeof(MN_MSG_BcdAddr), 0x00, sizeof(MN_MSG_BcdAddr));
    }

    if (addrLen == 0) {
        return AT_OK;
    }

    if (At_CheckNumLen((MN_MAX_ASCII_ADDRESS_NUM + 1), addrLen) == AT_FAILURE) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    *(addr + addrLen) = '\0';

    /* MN_MAX_ASCII_ADDRESS_NUM + 2为aucAsciiNum的长度 */
    (VOS_VOID)memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));
    if (At_SetNumTypePara(asciiNum, sizeof(asciiNum), (TAF_UINT8 *)addr, addrLen) != AT_SUCCESS) {
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    /* 设置<toda> */
    num = (TAF_UINT8 *)asciiNum;
    if ((At_GetCodeType(asciiNum[0])) == AT_MSG_INTERNAL_ISDN_ADDR_TYPE) {
        num = (TAF_UINT8 *)(asciiNum + 1);
        if ((numTypeLen != 0) && (addrType != AT_MSG_INTERNAL_ISDN_ADDR_TYPE)) {
            return AT_CMS_OPERATION_NOT_ALLOWED;
        }
    }

    if (numTypeLen == 0) {
        addrType = (TAF_UINT8)At_GetCodeType(asciiNum[0]);
    }

    asciiAddrLen = VOS_StrLen((TAF_CHAR *)num);
    if (asciiAddrLen > MN_MAX_ASCII_ADDRESS_NUM) {
        AT_NORM_LOG("At_GetAsciiOrBcdAddr: invalid address length.");
        return AT_CMS_OPERATION_NOT_ALLOWED;
    }

    At_GetNumTypeFromAddrType(numType, addrType);
    At_GetNumPlanFromAddrType(numPlan, addrType);
    ret = AT_ChkSmsNumPlan(numPlan);
    if (ret != MN_ERR_NO_ERROR) {
        AT_ERR_LOG("At_GetAsciiOrBcdAddr: Numbering plan is invalid");
        return AT_ERROR;
    }
    ret = AT_ChkSmsNumType(numType);
    if (ret != MN_ERR_NO_ERROR) {
        AT_ERR_LOG("At_GetAsciiOrBcdAddr: Number type is invalid");
        return AT_ERROR;
    }

    if (asciiAddr != TAF_NULL_PTR) {
        asciiAddr->numType = numType;
        asciiAddr->numPlan = numPlan;
        asciiAddr->len     = asciiAddrLen;
        if (asciiAddr->len > 0) {
            memResult = memcpy_s(asciiAddr->asciiNum, sizeof(asciiAddr->asciiNum), num, asciiAddr->len);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiAddr->asciiNum), asciiAddr->len);
        }
    }

    if (bcdAddr != TAF_NULL_PTR) {
        bcdAddr->addrType = addrType;
        ret               = AT_AsciiNumberToBcd((TAF_CHAR *)num, bcdAddr->bcdNum, &bcdAddr->bcdLen);
        if (ret != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        }
    }

    return AT_OK;
}


VOS_VOID AT_JudgeIsPlusSignInDialString(const VOS_CHAR *pcAsciiNumber, VOS_UINT16 len, VOS_UINT8 *isExistPlusSign,
                                        VOS_UINT8 *plusSignLocation)
{
    VOS_UINT8 loop;

    for (loop = 0; loop < len && pcAsciiNumber[loop] != '\0'; loop++) {
        if (pcAsciiNumber[loop] == '+') {
            *isExistPlusSign  = VOS_TRUE;
            *plusSignLocation = loop;
            break;
        }
    }

    return;
}


TAF_UINT32 At_PrintListMsg(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo, TAF_UINT8 *dst)
{
    TAF_UINT16      length = 0;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    switch (tsDataInfo->tpduType) {
        case MN_MSG_TPDU_DELIVER:
            /* +CMGL: <index>,<stat>,<oa/da>,[<alpha>],[<scts>][,<tooa/toda>,<length>]<CR><LF><data>[<CR><LF> */
            /* <oa> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.deliver.origAddr, (dst + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");
            /* <alpha> 不报 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            /* <scts> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.deliver.timeStamp, (dst + length));
            if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(dst + length), ",");

                /* <tooa> */
                length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.deliver.origAddr, (dst + length));

                /* <length> */
                length += AT_PrintSmsLength(tsDataInfo->u.deliver.dcs.msgCoding, tsDataInfo->u.deliver.userData.len,
                                            (dst + length));
            }

            /* <data> 有可能得到是UCS2，需仔细处理 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)dst, (TAF_CHAR *)(dst + length),
                "%s", g_atCrLf);

            length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN - (TAF_UINT32)(dst - g_atSndCodeAddress),
                                                     (TAF_INT8 *)dst, tsDataInfo->u.deliver.dcs.msgCoding,
                                                     (dst + length), tsDataInfo->u.deliver.userData.orgData,
                                                     (TAF_UINT16)tsDataInfo->u.deliver.userData.len);

            break;
        case MN_MSG_TPDU_SUBMIT:
            /* +CMGL: <index>,<stat>,<oa/da>,[<alpha>],[<scts>][,<tooa/toda>,<length>]<CR><LF><data>[<CR><LF> */
            /* <da> */
            length += At_PrintAsciiAddr(&tsDataInfo->u.submit.destAddr, (TAF_UINT8 *)(dst + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            /* <alpha> 不报 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(dst + length), ",");

                /* <toda> */
                length += At_PrintAddrType(&tsDataInfo->u.submit.destAddr, (dst + length));

                /* <length> */
                length += AT_PrintSmsLength(tsDataInfo->u.deliver.dcs.msgCoding, tsDataInfo->u.submit.userData.len,
                                            (dst + length));
            }

            /* <data> 有可能得到是UCS2，需仔细处理 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)dst, (TAF_CHAR *)(dst + length),
                "%s", g_atCrLf);

            length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)dst,
                                                     tsDataInfo->u.submit.dcs.msgCoding,
                                                     (dst + length), tsDataInfo->u.submit.userData.orgData,
                                                     (TAF_UINT16)tsDataInfo->u.submit.userData.len);

            break;
        case MN_MSG_TPDU_COMMAND:
            /* +CMGL: <index>,<stat>,<fo>,<ct>[<CR><LF> */
            /* <fo> */
            length += At_PrintMsgFo(tsDataInfo, (dst + length));
            /* <ct> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)dst, (TAF_CHAR *)(dst + length),
                ",%d", tsDataInfo->u.command.cmdType);
            break;
        case MN_MSG_TPDU_STARPT:
            /*
             * +CMGL: <index>,<stat>,<fo>,<mr>,
             * [<ra>],[<tora>],<scts>,<dt>,<st>
             * [<CR><LF>
             */
            /* <fo> */
            length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (dst + length));
            /* <mr> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",%d,", tsDataInfo->u.staRpt.mr);

            /* <ra> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.staRpt.recipientAddr, (dst + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            /* <tora> */
            length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.staRpt.recipientAddr, (dst + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            /* <scts> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.timeStamp, (dst + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",");

            /* <dt> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.dischargeTime, (dst + length));

            /* <st> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(dst + length), ",%d", tsDataInfo->u.staRpt.status);
            break;
        default:
            break;
    }

    return length;
}


TAF_VOID At_GetCpmsMemStatus(VOS_UINT8 indexNum, MN_MSG_MemStoreUint8 memType, TAF_UINT32 *totalRec,
                             TAF_UINT32 *usedRec)
{
    MN_MSG_StorageListEvtInfo *storageList = VOS_NULL_PTR;
    AT_ModemSmsCtx            *smsCtx      = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (memType == MN_MSG_MEM_STORE_SIM) {
        storageList = &(smsCtx->cpmsInfo.usimStorage);
    } else if (memType == MN_MSG_MEM_STORE_ME) {
        storageList = &(smsCtx->cpmsInfo.nvimStorage);
    } else { /* 无存储设备 */
        *usedRec  = 0;
        *totalRec = 0;
        return;
    }

    *totalRec = storageList->totalRec;
    *usedRec  = storageList->usedRec;

    return;
}


TAF_VOID At_PrintSetCpmsRsp(TAF_UINT8 indexNum)
{
    TAF_UINT16 length = 0;
    TAF_UINT32 totalRec; /* sms capacity of NVIM or USIM */
    /* used records including all status */
    TAF_UINT32      usedRec;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <used1>,<total1>,<used2>,<total2>,<used3>,<total3> */
    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.memReadorDelete, &totalRec, &usedRec);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d,", usedRec, totalRec);

    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.memSendorWrite, &totalRec, &usedRec);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d,", usedRec, totalRec);

    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.rcvPath.smMemStore, &totalRec, &usedRec);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d", usedRec, totalRec);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return;
}


TAF_UINT8* At_GetCpmsMemTypeStr(MN_MSG_MemStoreUint8 memType)
{
    TAF_UINT32 memoryType;

    if (memType == MN_MSG_MEM_STORE_SIM) {
        memoryType = AT_STRING_SM;
    } else if (memType == MN_MSG_MEM_STORE_ME) {
        memoryType = AT_STRING_ME;
    } else {
        memoryType = AT_STRING_BUTT;
    }
    return g_atStringTab[memoryType].text;
}


VOS_VOID At_PrintGetCpmsRsp(VOS_UINT8 indexNum)
{
    VOS_UINT16 length = 0;
    VOS_UINT32 totalRec; /* sms capacity of NVIM or USIM */
    /* used records including all status */
    VOS_UINT32      usedRec;
    VOS_UINT8      *memTypeStr = VOS_NULL_PTR;
    AT_ModemSmsCtx *smsCtx     = VOS_NULL_PTR;

    totalRec = 0;
    usedRec  = 0;
    smsCtx   = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <used1>,<total1>,<used2>,<total2>,<used3>,<total3> */
    memTypeStr = At_GetCpmsMemTypeStr(smsCtx->cpmsInfo.memReadorDelete);
    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.memReadorDelete, &totalRec, &usedRec);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s,%d,%d,", memTypeStr, usedRec, totalRec);

    memTypeStr = At_GetCpmsMemTypeStr(smsCtx->cpmsInfo.memSendorWrite);
    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.memSendorWrite, &totalRec, &usedRec);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s,%d,%d,", memTypeStr, usedRec, totalRec);

    memTypeStr = At_GetCpmsMemTypeStr(smsCtx->cpmsInfo.rcvPath.smMemStore);
    At_GetCpmsMemStatus(indexNum, smsCtx->cpmsInfo.rcvPath.smMemStore, &totalRec, &usedRec);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s,%d,%d", memTypeStr, usedRec, totalRec);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return;
}


TAF_VOID AT_PrintTimeZone(TAF_INT8 timezone, TAF_UINT8 *dst, TAF_UINT16 *length)
{
    TAF_UINT8  timeZone;
    TAF_UINT16 lengthTemp;

    if (timezone < 0) {
        lengthTemp = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)dst,
            "-");
        timeZone   = (TAF_UINT8)(timezone * (-1));
    } else {
        lengthTemp = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)dst,
            "+");
        timeZone   = (TAF_UINT8)timezone;
    }

    if (timeZone > MN_MSG_MAX_TIMEZONE_VALUE) {
        AT_WARN_LOG("AT_PrintTimeZone: Time zone is invalid.");
        timeZone = 0;
    }

    /* 除10是为了取高位数，余10是为了取低位 */
    lengthTemp += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(dst + lengthTemp), "%d%d\"", (0x0f & (timeZone / 10)), (timeZone % 10));

    *length = lengthTemp;

    return;
}


TAF_UINT32 At_SmsPrintScts(const MN_MSG_Timestamp *timeStamp, TAF_UINT8 *dst)
{
    TAF_UINT16                  length;
    TAF_UINT16                  timeZoneLength;
    MN_MSG_DateInvalidTypeUint8 dateInvalidType;
    TAF_UINT32                  ret;

    ret = MN_MSG_ChkDate(timeStamp, &dateInvalidType);
    if (ret != MN_ERR_NO_ERROR) {
        AT_WARN_LOG("At_SmsPrintScts: Date is invalid.");
    }

    /* "yy/MM/dd,hh:mm:ss±zz" */
    if ((MN_MSG_DATE_INVALID_YEAR & dateInvalidType) == 0) {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)dst,
            "\"%d%d/", ((timeStamp->year >> 4) & 0x0f), (timeStamp->year & 0x0f));
    } else {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)dst,
            "\"00/");
    }

    /* MM */
    if ((MN_MSG_DATE_INVALID_MONTH & dateInvalidType) == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%d%d/", ((timeStamp->month >> 4) & 0x0f), (timeStamp->month & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "01/");
    }

    /* dd */
    if ((MN_MSG_DATE_INVALID_DAY & dateInvalidType) == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%d%d,", ((timeStamp->day >> 4) & 0x0f), (timeStamp->day & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "01,");
    }

    /* hh */
    if ((MN_MSG_DATE_INVALID_HOUR & dateInvalidType) == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%d%d:", ((timeStamp->hour >> 4) & 0x0f), (timeStamp->hour & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "00:");
    }

    /* mm */
    if ((MN_MSG_DATE_INVALID_MINUTE & dateInvalidType) == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%d%d:", ((timeStamp->minute >> 4) & 0x0f), (timeStamp->minute & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "00:");
    }

    /* ss */
    if ((MN_MSG_DATE_INVALID_SECOND & dateInvalidType) == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%d%d", ((timeStamp->second >> 4) & 0x0f), (timeStamp->second & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "00");
    }

    /* ±zz */
    AT_PrintTimeZone(timeStamp->timezone, (dst + length), &timeZoneLength);
    length += timeZoneLength;

    return length;
}


TAF_UINT16 At_MsgPrintVp(MN_MSG_ValidPeriod *validPeriod, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    switch (validPeriod->validPeriod) {
        case MN_MSG_VALID_PERIOD_RELATIVE:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, "%d", validPeriod->u.otherTime);
            break;
        case MN_MSG_VALID_PERIOD_ABSOLUTE:
            length += (TAF_UINT16)At_SmsPrintScts(&validPeriod->u.absoluteTime, dst);
            break;
        default:
            break;
    }
    return length;
}


TAF_UINT32 At_SmsPrintState(AT_CmgfMsgFormatUint8 smsFormat, MN_MSG_StatusTypeUint8 status, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    if (smsFormat == AT_CMGF_MSG_FORMAT_TEXT) { /* TEXT */
        switch (status) {
            case MN_MSG_STATUS_MT_NOT_READ:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_REC_UNREAD_TEXT].text);
                break;

            case MN_MSG_STATUS_MT_READ:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_REC_READ_TEXT].text);
                break;

            case MN_MSG_STATUS_MO_NOT_SENT:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_STO_UNSENT_TEXT].text);
                break;

            case MN_MSG_STATUS_MO_SENT:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_STO_SENT_TEXT].text);
                break;

            case MN_MSG_STATUS_NONE:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_ALL_TEXT].text);
                break;

            default:
                return 0;
        }
    } else { /* PDU */
        switch (status) {
            case MN_MSG_STATUS_MT_NOT_READ:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_REC_UNREAD_PDU].text);
                break;

            case MN_MSG_STATUS_MT_READ:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_REC_READ_PDU].text);
                break;

            case MN_MSG_STATUS_MO_NOT_SENT:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_STO_UNSENT_PDU].text);
                break;

            case MN_MSG_STATUS_MO_SENT:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_STO_SENT_PDU].text);
                break;

            case MN_MSG_STATUS_NONE:
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)dst + length, "%s", g_atStringTab[AT_STRING_ALL_PDU].text);
                break;

            default:
                return 0;
        }
    }

    return length;
}


TAF_UINT32 At_GetScaFromInputStr(const TAF_UINT8 *addr, MN_MSG_BcdAddr *bcdAddr, TAF_UINT32 *len)
{
    TAF_UINT32       ret;
    MN_MSG_AsciiAddr asciiAddr;

    ret = MN_MSG_DecodeAddress(addr, TAF_TRUE, &asciiAddr, len);
    if (ret != MN_ERR_NO_ERROR) {
        return ret;
    }

    bcdAddr->addrType = 0x80;
    bcdAddr->addrType |= asciiAddr.numPlan;
    bcdAddr->addrType |= ((asciiAddr.numType << 4) & 0x70);

    ret = AT_AsciiNumberToBcd((TAF_CHAR *)asciiAddr.asciiNum, bcdAddr->bcdNum, &bcdAddr->bcdLen);

    return ret;
}


TAF_UINT32 At_MsgDeleteCmdProc(TAF_UINT8 indexNum, MN_OPERATION_ID_T opId, MN_MSG_DeleteParam deleteInfo,
                               TAF_UINT32 deleteTypes)
{
    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes & AT_MSG_DELETE_SINGLE) {
        deleteInfo.deleteType = MN_MSG_DELETE_SINGLE;
        if (MN_MSG_Delete(g_atClientTab[indexNum].clientId, opId, &deleteInfo) != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        } else {
            return AT_OK;
        }
    }

    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes & AT_MSG_DELETE_READ) {
        deleteInfo.deleteType = MN_MSG_DELETE_READ;
        if (MN_MSG_Delete(g_atClientTab[indexNum].clientId, opId, &deleteInfo) != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        } else {
            return AT_OK;
        }
    }

    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes & AT_MSG_DELETE_SENT) {
        deleteInfo.deleteType = MN_MSG_DELETE_SENT;
        if (MN_MSG_Delete(g_atClientTab[indexNum].clientId, opId, &deleteInfo) != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        } else {
            return AT_OK;
        }
    }

    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes & AT_MSG_DELETE_UNSENT) {
        deleteInfo.deleteType = MN_MSG_DELETE_NOT_SENT;
        if (MN_MSG_Delete(g_atClientTab[indexNum].clientId, opId, &deleteInfo) != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        } else {
            return AT_OK;
        }
    }

    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes & AT_MSG_DELETE_ALL) {
        deleteInfo.deleteType = MN_MSG_DELETE_ALL;
        if (MN_MSG_Delete(g_atClientTab[indexNum].clientId, opId, &deleteInfo) != MN_ERR_NO_ERROR) {
            return AT_ERROR;
        } else {
            return AT_OK;
        }
    }

    return AT_ERROR;
}

