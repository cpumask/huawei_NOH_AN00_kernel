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

#include "AtCmdFtmProc.h"
#include "securec.h"
#include "at_mdrv_interface.h"
#include "ATCmdProc.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "msp_diag.h"
#endif

#include "AtTestParaCmd.h"
#include "taf_app_mma.h"

#include "AtDataProc.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_FTM_PROC_C

#define AT_LTEPWRCTRL_MAX_DELETE_CA_NUM (-8)
#define AT_LTEPWRCTRL_MAX_ADD_CA_NUM 8

#define AT_LTEPWRCTRL_DISABLE_REDUCE_RI 0
#define AT_LTEPWRCTRL_ENABLE_REDUCE_RI 1

#define AT_LTEPWRCTRL_MIN_SUPPORT_BSR_NUM 0
#define AT_LTEPWRCTRL_MAX_SUPPORT_BSR_NUM 63

#define AT_LOGPORT_PARA_MIN_NUM 0
#define AT_LOGPORT_PARA_MAX_NUM 2

#define AT_FREQLOCK_PARA_MAX_NUM 4
#define AT_FREQLOCK_PARA_MIN_NUM 1
#define AT_FREQLOCK_RATMODE 2
#define AT_FREQLOCK_BAND 3

#define AT_GFREQLOCK_PARA_MIN_NUM 1
#define AT_GFREQLOCK_PARA_MAX_NUM 3
#define AT_GFREQLOCK_ENABLE_FLAG 0
#define AT_GFREQLOCK_FREQ 1
#define AT_GFREQLOCK_BAND 2

#define AT_JDCFG_GSM_RSSI_THRESH 1
#define AT_JDCFG_GSM_RSSI_NUM 2
#define AT_JDCFG_WCDMA_RSSI_THRESH 1
#define AT_JDCFG_WCDMA_RSSI_PERCENT 2
#define AT_JDCFG_WCDMA_PSCH_THRESH 3
#define AT_JDCFG_WCDMA_PSCH_PERCENT 4
#define AT_JDCFG_LTE_RSSI_THRESH 1
#define AT_JDCFG_LTE_RSSI_PERCENT 2
#define AT_JDCFG_LTE_PSSRATIO_THRESH 3
#define AT_JDCFG_LTE_PSSRATIO_PERCENT 4
#define AT_JDCFG_RSSISRHFREQ_PERCENT 2
#define AT_JDCFG_PSCHSRHTHRE_SHOID 3
#define AT_JDCFG_PSCHSRHFREQ_PERCENT 4

#define AT_DPDTTEST_PARA_NUM 2
#define AT_EMRSSICFG_PARA_NUM 2

#define AT_LTEPWRCTRL_PARA 1
#define AT_LTEPWRCTRL_PARA_NUM 2
#define AT_LTEPWRCTRL_PARA_MAX_LEN 2

#define AT_RSLT_BASE_NUM 3

enum AT_LTEPWRCTRL_MODE_TYPE_ENUM {
    AT_LTEPWRCTRL_MODE_CC_NUM_CTR  = 0,
    AT_LTEPWRCTRL_MODE_RI_NUM_CTR  = 1,
    AT_LTEPWRCTRL_MODE_BSR_NUM_CTR = 2,
    AT_LTEPWRCTRL_MODE_BUTT        = 3,
};
typedef VOS_UINT32 AT_LTEPWRCTRL_MODE_TYPE_ENUM_UINT32;


VOS_UINT32 At_SetLogPortPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rslt;
    VOS_UINT32 omLogPort;  // 准备切换的LogPort

    AT_PR_LOGI("Rcv Msg");

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数过多或没有 */
    if ((g_atParaIndex > AT_LOGPORT_PARA_MAX_NUM) || (g_atParaIndex == AT_LOGPORT_PARA_MIN_NUM)) {
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_LOG_PORT_USB) {
        omLogPort = CPM_OM_PORT_TYPE_USB;
    } else {
        omLogPort = CPM_OM_PORT_TYPE_VCOM;
    }

    /* 参数只有一个，默认永久生效 */
    if (g_atParaIndex == 1) {
        g_atParaList[1].paraValue = VOS_TRUE;
    }

    /* 调用OM的接口 */
    rslt = AT_SwitchDiagLogPort(omLogPort, g_atParaList[1].paraValue);

    AT_PR_LOGI("Call interface success!");

    if (rslt == VOS_OK) {
        return AT_OK;
    } else if (rslt == ERR_MSP_AT_CHANNEL_BUSY) {
        return AT_CME_USB_TO_VCOM_IN_CONN_ERROR;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryLogPortPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;
    VOS_UINT32 omLogPort;
    VOS_UINT32 atLogPort;
    VOS_UINT32 rslt;

    length    = 0;
    omLogPort = AT_LOG_PORT_USB;

    AT_PR_LOGI("Rcv Msg");

    rslt = AT_QueryPpmLogPort(&omLogPort);

    AT_PR_LOGI("Call interface success!");

    if (rslt != VOS_OK) {
        return AT_ERROR;
    }

    if (omLogPort == COMM_LOG_PORT_USB) {
        atLogPort = AT_LOG_PORT_USB;
    } else {
        atLogPort = AT_LOG_PORT_VCOM;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", atLogPort);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 At_QryLogCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;
    VOS_UINT32 atLogCfg;

    length = 0;
    atLogCfg = 0;
    PS_PRINTF_INFO("at^logcfg?!\n");

    (void)AT_GetSocpCfgIndMode(&atLogCfg);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", atLogCfg);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}
/*
 * 功能描述: ^LOGCPS的查询函数
 */
VOS_UINT32 At_QryLogCpsPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;
    VOS_UINT32 atLogCps;
    length = 0;
    atLogCps = 0;
    PS_PRINTF_INFO("at^logcps?!\n");

    (void)AT_GetSocpCompressIndMode(&atLogCps);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", atLogCps);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 At_SetDpdtTestFlagPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetDpdttestFlagReq atCmd;
    VOS_UINT32                rst;

    /* 参数检查 */
    if (g_atParaIndex != AT_DPDTTEST_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT发送至MTA的消息结构赋值 */
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));
    atCmd.ratMode = (AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue;
    atCmd.flag    = (VOS_UINT8)g_atParaList[1].paraValue;

    /* 发送消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_DPDTTEST_FLAG_REQ,
                                 (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_SetDpdttestFlagReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPDTTEST_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaSetDpdtTestFlagCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg                *mtaMsg         = VOS_NULL_PTR;
    MTA_AT_SetDpdttestFlagCnf *setDpdtFlagCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;
    VOS_UINT32                 result;

    /* 初始化消息变量 */
    indexNum       = 0;
    mtaMsg         = (AT_MTA_Msg *)msg;
    setDpdtFlagCnf = (MTA_AT_SetDpdttestFlagCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtTestFlagCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtTestFlagCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DPDTTEST_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPDTTEST_SET) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtTestFlagCnf: WARNING:Not AT_CMD_DPDTTEST_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (setDpdtFlagCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetDpdtValueCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *mtaMsg          = VOS_NULL_PTR;
    MTA_AT_SetDpdtValueCnf *setDpdtValueCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    indexNum        = 0;
    mtaMsg          = (AT_MTA_Msg *)msg;
    setDpdtValueCnf = (MTA_AT_SetDpdtValueCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtValueCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtValueCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DPDT_SET或AT_CMD_TFDPDT_SET */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPDT_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TFDPDT_SET)) {
        AT_WARN_LOG("AT_RcvMtaSetDpdtValueCnf: WARNING:Not AT_CMD_DPDT_SET or AT_CMD_TFDPDT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (setDpdtValueCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryDpdtValueCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *mtaMsg          = VOS_NULL_PTR;
    MTA_AT_QryDpdtValueCnf *qryDpdtValueCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    indexNum        = 0;
    mtaMsg          = (AT_MTA_Msg *)msg;
    qryDpdtValueCnf = (MTA_AT_QryDpdtValueCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryDpdtValueCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryDpdtValueCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DPDTQRY_SET或AT_CMD_TFDPDTQRY_SET */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPDTQRY_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TFDPDTQRY_SET)) {
        AT_WARN_LOG("AT_RcvMtaQryDpdtValueCnf: WARNING:Not AT_CMD_DPDTQRY_SET or AT_CMD_TFDPDTQRY_SET !");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (qryDpdtValueCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                qryDpdtValueCnf->dpdtValue);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_SetGFreqLock(VOS_UINT8 indexNum)
{
    AT_MTA_SetGsmFreqlockReq gFreqLockInfo;
    VOS_UINT32               rst;

    /* 参数个数检查 */
    if ((g_atParaIndex != AT_GFREQLOCK_PARA_MIN_NUM) && (g_atParaIndex != AT_GFREQLOCK_PARA_MAX_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 初始化 */
    (VOS_VOID)memset_s(&gFreqLockInfo, sizeof(gFreqLockInfo), 0x00, sizeof(gFreqLockInfo));

    /* 参数有效性检查 */
    /* 第一个参数必须带 */
    if (g_atParaList[AT_GFREQLOCK_ENABLE_FLAG].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    gFreqLockInfo.enableFlag = (PS_BOOL_ENUM_UINT8)g_atParaList[0].paraValue;

    /* 若启动锁频，则必须带第二个参数和第三个参数 */
    if (gFreqLockInfo.enableFlag == PS_TRUE) {
        if ((g_atParaList[AT_GFREQLOCK_FREQ].paraLen == 0) || (g_atParaList[AT_GFREQLOCK_BAND].paraLen == 0)) {
            return AT_CME_INCORRECT_PARAMETERS;
        } else {
            gFreqLockInfo.freq = (VOS_UINT16)g_atParaList[AT_GFREQLOCK_FREQ].paraValue;
            gFreqLockInfo.band = (AT_MTA_GsmBandUint16)g_atParaList[AT_GFREQLOCK_BAND].paraValue;
        }
    }

    /* 发送消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_GSM_FREQLOCK_REQ,
                                 (VOS_UINT8 *)&gFreqLockInfo, sizeof(AT_MTA_SetGsmFreqlockReq), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GSM_FREQLOCK_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 AT_QryGFreqLock(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息ID_AT_MTA_QRY_GSM_FREQLOCK_REQ给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_GSM_FREQLOCK_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GSM_FREQLOCK_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


VOS_UINT32 AT_RcvMtaSetGFreqLockCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetGsmFreqlockCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetGsmFreqlockCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetGFreqLockCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetGFreqLockCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GSM_FREQLOCK_SET) {
        AT_WARN_LOG("AT_RcvMtaSetGFreqLockCnf : Current Option is not AT_CMD_GSM_FREQLOCK_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaGFreqLockQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_QryGsmFreqlockCnf *qryGFreqlockCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    qryGFreqlockCnf = (MTA_AT_QryGsmFreqlockCnf *)rcvMsg->content;
    result          = AT_OK;
    indexNum        = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaGFreqLockQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaGFreqLockQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GSM_FREQLOCK_QRY) {
        AT_WARN_LOG("AT_RcvMtaGFreqLockQryCnf : Current Option is not AT_CMD_GSM_FREQLOCK_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^GFREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (qryGFreqlockCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        if (qryGFreqlockCnf->lockFlg == VOS_FALSE) {
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                (TAF_INT32)qryGFreqlockCnf->lockFlg);
        } else {
            g_atSendDataBuff.bufLen =
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                    (TAF_INT32)qryGFreqlockCnf->lockFlg, (TAF_INT32)qryGFreqlockCnf->freq,
                    (TAF_INT32)qryGFreqlockCnf->band);
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_NetMonFmtPlmnId(VOS_UINT32 mcc, VOS_UINT32 mnc, VOS_CHAR *pstrPlmn, VOS_UINT16 *length)
{
    VOS_UINT32 maxLength;
    VOS_UINT32 lengthRlt;
    VOS_INT32  lengthTemp;

    lengthTemp = 0;
    lengthRlt  = 0;
    maxLength  = AT_NETMON_PLMN_STRING_MAX_LENGTH;

    /* 格式输出MCC MNC */
    if ((mnc & 0x0f0000) == 0x0f0000) {
        lengthTemp = snprintf_s((VOS_CHAR *)pstrPlmn, (VOS_UINT32)maxLength, (VOS_UINT32)(maxLength - 1),
                                            "%X%X%X,%X%X", (mcc & 0x0f), (mcc & 0x0f00) >> 8, (mcc & 0x0f0000) >> 16,
                                            (mnc & 0x0f), (mnc & 0x0f00) >> 8);
    } else {
        lengthTemp = snprintf_s((VOS_CHAR *)pstrPlmn, (VOS_UINT32)maxLength, (VOS_UINT32)(maxLength - 1),
                                            "%X%X%X,%X%X%X", (mcc & 0x0f), (mcc & 0x0f00) >> 8, (mcc & 0x0f0000) >> 16,
                                            (mnc & 0x0f), (mnc & 0xf00) >> 8, (mnc & 0x0f0000) >> 16);
    }

    if (lengthTemp < 0) {
        AT_ERR_LOG("AT_NetMonFmtPlmnId: MCC ulLength is error!");
        *(pstrPlmn + AT_NETMON_PLMN_STRING_MAX_LENGTH - 1) = 0;
        *length                                            = AT_NETMON_PLMN_STRING_MAX_LENGTH;
        return;
    }

    lengthRlt = (VOS_UINT32)lengthTemp;
    /* 长度翻转保护 */
    if (lengthRlt >= AT_NETMON_PLMN_STRING_MAX_LENGTH) {
        AT_ERR_LOG("AT_NetMonFmtPlmnId: MCC ulLength is error!");
        *(pstrPlmn + AT_NETMON_PLMN_STRING_MAX_LENGTH - 1) = 0;
        *length                                            = AT_NETMON_PLMN_STRING_MAX_LENGTH;
        return;
    }

    *length = (VOS_UINT16)lengthRlt;

    return;
}


VOS_VOID AT_NetMonFmtGsmSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length)
{
    VOS_UINT8  plmnstr[AT_NETMON_PLMN_STRING_MAX_LENGTH];
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_UINT16 plmnStrLen;
    VOS_UINT16 lengthTemp;

    lengthTemp = 0;
    plmnStrLen = 0;
    (VOS_VOID)memset_s(plmnstr, sizeof(plmnstr), 0x00, sizeof(plmnstr));

    mcc = sCellInfo->u.unSCellInfo.gsmSCellInfo.mcc;
    mnc = sCellInfo->u.unSCellInfo.gsmSCellInfo.mnc;

    AT_NetMonFmtPlmnId(mcc, mnc, (VOS_CHAR *)plmnstr, &plmnStrLen);

    /* 格式输出PLMN */
    lengthTemp +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "^MONSC: GSM,%s,%d,%u,%d,%X,%X", plmnstr,
            sCellInfo->u.unSCellInfo.gsmSCellInfo.band, sCellInfo->u.unSCellInfo.gsmSCellInfo.arfcn,
            sCellInfo->u.unSCellInfo.gsmSCellInfo.bsic, sCellInfo->u.unSCellInfo.gsmSCellInfo.cellId,
            sCellInfo->u.unSCellInfo.gsmSCellInfo.lac);

    /* RSSI无效值，不显示 */
    if (sCellInfo->u.unSCellInfo.gsmSCellInfo.rssi == AT_NETMON_GSM_RSSI_INVALID_VALUE) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", sCellInfo->u.unSCellInfo.gsmSCellInfo.rssi);
    }

    /* 无效值，不显示 */
    if (sCellInfo->u.unSCellInfo.gsmSCellInfo.rxQuality == AT_NETMON_GSM_RX_QUALITY_INVALID_VALUE) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", sCellInfo->u.unSCellInfo.gsmSCellInfo.rxQuality);
    }

    /* 输出TA */
    if (sCellInfo->u.unSCellInfo.gsmSCellInfo.opTa == PS_IE_PRESENT) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", sCellInfo->u.unSCellInfo.gsmSCellInfo.ta);
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    }

    *length = lengthTemp;

    return;
}


VOS_VOID AT_NetMonFmtUtranFddSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length)
{
    VOS_UINT8  plmnstr[AT_NETMON_PLMN_STRING_MAX_LENGTH];
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_UINT16 plmnStrLen;
    VOS_UINT16 lengthTemp;

    lengthTemp = 0;
    plmnStrLen = 0;
    (VOS_VOID)memset_s(plmnstr, sizeof(plmnstr), 0x00, sizeof(plmnstr));

    mcc = sCellInfo->u.unSCellInfo.utranSCellInfo.mcc;
    mnc = sCellInfo->u.unSCellInfo.utranSCellInfo.mnc;

    AT_NetMonFmtPlmnId(mcc, mnc, (VOS_CHAR *)plmnstr, &plmnStrLen);

    /* 格式输出PLMN */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "^MONSC: WCDMA,%s", plmnstr);

    /* 输出频点 */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%u", sCellInfo->u.unSCellInfo.utranSCellInfo.arfcn);

    /* PSC无效值，不显示 */
    if ((sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.rscp == AT_NETMON_UTRAN_FDD_RSCP_INVALID_VALUE) &&
        (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.ecn0 == AT_NETMON_UTRAN_FDD_ECN0_INVALID_VALUE)) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.psc);
    }

    /* 输出Cell ID */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.opCellId == PS_IE_PRESENT) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%X", sCellInfo->u.unSCellInfo.utranSCellInfo.cellId);
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    }

    /* 输出LAC */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.opLac == PS_IE_PRESENT) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%X", sCellInfo->u.unSCellInfo.utranSCellInfo.lac);
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    }

    /* RSCP无效值，不显示 */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.rscp == AT_NETMON_UTRAN_FDD_RSCP_INVALID_VALUE) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.rscp);
    }

    /* RSSI无效值，不显示 */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.rssi == AT_NETMON_UTRAN_FDD_RSSI_INVALID_VALUE) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.rssi);
    }

    /* ECN0无效值，不显示 */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.ecn0 == AT_NETMON_UTRAN_FDD_ECN0_INVALID_VALUE) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.ecn0);
    }

    /* 输出DRX */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.opDrx == PS_IE_PRESENT) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.drx);
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    }

    /* 输出URA Id */
    if (sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.opUra == PS_IE_PRESENT) {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d",
            sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsFdd.ura);
    } else {
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
    }

    *length = lengthTemp;

    return;
}


VOS_VOID AT_NetMonFmtGsmNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;

    length = inLen;

    if (nCellInfo->u.nCellInfo.gsmNCellCnt > NETMON_MAX_GSM_NCELL_NUM) {
        nCellInfo->u.nCellInfo.gsmNCellCnt = NETMON_MAX_GSM_NCELL_NUM;
    }

    /* GSM邻区显示 */
    for (loop = 0; loop < nCellInfo->u.nCellInfo.gsmNCellCnt; loop++) {
        /* 如果输出的不是第一个邻区，先打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: GSM,%d,%u",
            nCellInfo->u.nCellInfo.gsmNCellInfo[loop].band, nCellInfo->u.nCellInfo.gsmNCellInfo[loop].afrcn);

        /* 输出Bsic */
        if (nCellInfo->u.nCellInfo.gsmNCellInfo[loop].opBsic == PS_IE_PRESENT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", nCellInfo->u.nCellInfo.gsmNCellInfo[loop].bsic);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }

        /* 输出Cell ID */
        if (nCellInfo->u.nCellInfo.gsmNCellInfo[loop].opCellId == PS_IE_PRESENT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%X", nCellInfo->u.nCellInfo.gsmNCellInfo[loop].cellId);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }

        /* 输出LAC */
        if (nCellInfo->u.nCellInfo.gsmNCellInfo[loop].opLac == PS_IE_PRESENT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%X", nCellInfo->u.nCellInfo.gsmNCellInfo[loop].lac);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }

        /* 输出RSSI */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", nCellInfo->u.nCellInfo.gsmNCellInfo[loop].rssi);
    }

    *outLen = length;

    return;
}

VOS_VOID AT_NetMonFmtUtranFddNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;

    length = inLen;

    if (nCellInfo->u.nCellInfo.utranNCellCnt > NETMON_MAX_UTRAN_NCELL_NUM) {
        nCellInfo->u.nCellInfo.utranNCellCnt = NETMON_MAX_UTRAN_NCELL_NUM;
    }

    /* WCDMA 临区显示 */
    for (loop = 0; loop < nCellInfo->u.nCellInfo.utranNCellCnt; loop++) {
        /* 如果不是第一次打印邻区，打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: WCDMA,%u,%d,%d,%d",
            nCellInfo->u.nCellInfo.u.fddNCellInfo[loop].arfcn, nCellInfo->u.nCellInfo.u.fddNCellInfo[loop].psc,
            nCellInfo->u.nCellInfo.u.fddNCellInfo[loop].rscp, nCellInfo->u.nCellInfo.u.fddNCellInfo[loop].ecN0);
    }

    *outLen = length;

    return;
}


VOS_UINT32 At_SetNetMonSCellPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SET_NETMON_SCELL_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MONSC_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 At_SetNetMonNCellPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SET_NETMON_NCELL_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MONNC_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 At_SetNetMonSSCellPara(VOS_UINT8 atIndex)
{
    VOS_UINT32 rslt;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rslt = AT_FillAndSndAppReqMsg(g_atClientTab[atIndex].clientId, g_atClientTab[atIndex].opId,
                                  ID_AT_MTA_SET_NETMON_SSCELL_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rslt == TAF_SUCCESS) {
        g_atClientTab[atIndex].cmdCurrentOpt = AT_CMD_MONSSC_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaSetNetMonSSCellCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_NetmonCellInfo *cellInfo = VOS_NULL_PTR;
    VOS_UINT8              atIndex;
    VOS_UINT16             length;
    VOS_UINT32             msgLength;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cellInfo = (MTA_AT_NetmonCellInfo *)rcvMsg->content;

    length    = 0;
    atIndex   = 0;
    msgLength = sizeof(AT_MTA_Msg) - AT_DATA_DEFAULT_LENGTH + sizeof(MTA_AT_NetmonCellInfo) -
                VOS_MSG_HEAD_LENGTH;

    if (msgLength != VOS_GET_MSG_LEN(rcvMsg)) {
        AT_WARN_LOG("AT_RcvMtaSetNetMonSSCellCnf: WARNING: AT Length Is Wrong");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetNetMonSSCellCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(atIndex)) {
        AT_WARN_LOG("AT_RcvMtaSetNetMonSSCellCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[atIndex].cmdCurrentOpt != AT_CMD_MONSSC_SET) {
        AT_WARN_LOG("AT_RcvMtaSetNetMonSSCellCnf : Current Option is not AT_CMD_MONSSC_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(atIndex);

    if ((cellInfo->result == MTA_AT_RESULT_ERROR) || (cellInfo->cellType != MTA_NETMON_SSCELL_TYPE)) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_ERROR);
        return VOS_OK;
    }

    /* 如果收到特殊回复码，那么说明此时不在L下，回复NONE */
    if (cellInfo->result == MTA_AT_RESULT_OPERATION_NOT_ALLOWED) {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONSSC: NONE");
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_OK);
        return VOS_OK;
    }

    switch (cellInfo->ratType) {
        case MTA_AT_NETMON_CELL_INFO_NR: {
            AT_NetMonFmtNrSSCellData(cellInfo, &length);
            break;
        }

        default: {
            length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "^MONSSC: NONE");
            break;
        }
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(atIndex, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrSsbIdQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg    = VOS_NULL_PTR;
    MTA_AT_NrSsbIdQryCnf *ssbIdInfo = VOS_NULL_PTR;
    VOS_UINT8             atIndex;
    VOS_UINT16            length;
    VOS_UINT32            msgLength;

    /* 初始化 */
    rcvMsg    = (AT_MTA_Msg *)msg;
    ssbIdInfo = (MTA_AT_NrSsbIdQryCnf *)rcvMsg->content;

    length    = 0;
    atIndex   = 0;
    msgLength = sizeof(AT_MTA_Msg) - AT_DATA_DEFAULT_LENGTH + sizeof(MTA_AT_NrSsbIdQryCnf) -
                VOS_MSG_HEAD_LENGTH;

    if (VOS_GET_MSG_LEN(rcvMsg) != msgLength) {
        AT_WARN_LOG("AT_RcvMtaNrSsbIdQryCnf: WARNING: AT Length Is Wrong");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrSsbIdQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(atIndex)) {
        AT_WARN_LOG("AT_RcvMtaNrSsbIdQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[atIndex].cmdCurrentOpt != AT_CMD_NR_SSB_ID_QRY) {
        AT_WARN_LOG("AT_RcvMtaNrSsbIdQryCnf : Current Option is not AT_CMD_NR_SSB_ID_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(atIndex);

    if ((ssbIdInfo->result == MTA_AT_RESULT_ERROR) || (ssbIdInfo->result == MTA_AT_RESULT_OPTION_TIMEOUT)) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_ERROR);
        return VOS_OK;
    }

    /* 如果收到特殊回复码MTA_AT_RESULT_INCORRECT_PARAMETERS，那么说明此时在LTE但是不在NSA下，回复AT_CME_OPERATION_NOT_SUPPORTED */
    if (ssbIdInfo->result == MTA_AT_RESULT_INCORRECT_PARAMETERS) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_CME_OPERATION_NOT_SUPPORTED);
        return VOS_OK;
    }

    /* 如果收到特殊回复码MTA_AT_RESULT_OPERATION_NOT_ALLOWED，那么说明接入层回复失败，回复AT_CME_OPERATION_NOT_ALLOWED */
    if (ssbIdInfo->result == MTA_AT_RESULT_OPERATION_NOT_ALLOWED) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_CME_OPERATION_NOT_ALLOWED);
        return VOS_OK;
    }

    AT_FmtNrSsbIdData(ssbIdInfo, &length);

    /* 输出结果 */
    if (length != 0) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_OK);
    } else {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(atIndex, AT_ERROR);
    }

    return VOS_OK;
}

#endif


VOS_UINT32 AT_RcvMtaSetNetMonSCellCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NetmonCellInfo *setCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum;
    VOS_UINT16             length;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    setCnf = (MTA_AT_NetmonCellInfo *)rcvMsg->content;

    length   = 0;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetMonitServingCellCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetMonitServingCellCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MONSC_SET) {
        AT_WARN_LOG("AT_RcvMtaSetMonitServingCellCnf : Current Option is not AT_CMD_JDETEX_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((setCnf->result == MTA_AT_RESULT_ERROR) || (setCnf->cellType != MTA_NETMON_SCELL_TYPE)) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_ERROR);

        return VOS_OK;
    }

    switch (setCnf->ratType) {
        case MTA_AT_NETMON_CELL_INFO_GSM: {
            AT_NetMonFmtGsmSCellData(setCnf, &length);
            break;
        }
        case MTA_AT_NETMON_CELL_INFO_UTRAN_FDD: {
            AT_NetMonFmtUtranFddSCellData(setCnf, &length);
            break;
        }
#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
        case MTA_AT_NETMON_CELL_INFO_UTRAN_TDD: {
            AT_NetMonFmtUtranTddSCellData(setCnf, &length);
            break;
        }
#endif
#if (FEATURE_LTE == FEATURE_ON)
        case MTA_AT_NETMON_CELL_INFO_LTE: {
            AT_NetMonFmtEutranSCellData(setCnf, &length);
            break;
        }
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case MTA_AT_NETMON_CELL_INFO_NR: {
            AT_NetMonFmtNrSCellData(setCnf, &length);
            break;
        }
#endif

        default:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "^MONSC: NONE");
            break;
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetNetMonNCellCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NetmonCellInfo *setCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum;
    VOS_UINT16             length;
    VOS_UINT16             lengthTemp;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    setCnf = (MTA_AT_NetmonCellInfo *)rcvMsg->content;

    indexNum = 0;
    length   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetMonitNeighCellCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetMonitNeighCellCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MONNC_SET) {
        AT_WARN_LOG("AT_RcvMtaSetMonitNeighCellCnf : Current Option is not AT_CMD_JDETEX_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((setCnf->result == MTA_AT_RESULT_ERROR) || (setCnf->cellType != MTA_NETMON_NCELL_TYPE)) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_ERROR);

        return VOS_OK;
    }

    lengthTemp = 0;
    length     = 0;

    /* GSM邻区显示 */
    AT_NetMonFmtGsmNCellData(setCnf, lengthTemp, &length);

    /* UTRAN邻区显示 */
    lengthTemp = length;

    if (setCnf->u.nCellInfo.cellMeasTypeChoice == MTA_NETMON_UTRAN_FDD_TYPE) {
        AT_NetMonFmtUtranFddNCellData(setCnf, lengthTemp, &length);
    }
#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    else if (setCnf->u.nCellInfo.cellMeasTypeChoice == MTA_NETMON_UTRAN_TDD_TYPE) {
        AT_NetMonFmtUtranTddNCellData(setCnf, lengthTemp, &length);
    }
#endif
    else {
        /* 类型不对，不进行任何处理 */
        ;
    }

#if (FEATURE_LTE == FEATURE_ON)
    /* LTE邻区显示 */
    lengthTemp = length;

    AT_NetMonFmtEutranNCellData(setCnf, lengthTemp, &length);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    /* NR邻区显示 */
    lengthTemp = length;

    AT_NetMonFmtNrNCellData(setCnf, lengthTemp, &length);
#endif

    /* 无邻区，返回NONE */
    if ((setCnf->u.nCellInfo.gsmNCellCnt + setCnf->u.nCellInfo.utranNCellCnt + setCnf->u.nCellInfo.lteNCellCnt +
         setCnf->u.nCellInfo.nrNCellCnt) == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: NONE");
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_TDS == FEATURE_ON)

VOS_VOID AT_NetMonFmtUtranTddSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length)
{
    VOS_UINT8  plmnstr[AT_NETMON_PLMN_STRING_MAX_LENGTH];
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_UINT16 plmnStrLen;
    VOS_UINT16 lengthTemp;

    lengthTemp = 0;
    plmnStrLen = 0;
    (VOS_VOID)memset_s(plmnstr, sizeof(plmnstr), 0x00, sizeof(plmnstr));

    mcc = sCellInfo->u.unSCellInfo.utranSCellInfo.mcc;
    mnc = sCellInfo->u.unSCellInfo.utranSCellInfo.mnc;

    AT_NetMonFmtPlmnId(mcc, mnc, (VOS_CHAR *)plmnstr, &plmnStrLen);

    /* 格式输出PLMN */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "^MONSC: TD_SCDMA,%s", plmnstr);

    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%u,%d,%d,%X,%X,%d,%d,%d",
        sCellInfo->u.unSCellInfo.utranSCellInfo.arfcn,
        sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsTdd.syncID,
        sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsTdd.sc,
        sCellInfo->u.unSCellInfo.utranSCellInfo.cellId, sCellInfo->u.unSCellInfo.utranSCellInfo.lac,
        sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsTdd.rscp,
        sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsTdd.drx,
        sCellInfo->u.unSCellInfo.utranSCellInfo.u.cellMeasResultsTdd.rac);

    *length = lengthTemp;

    return;
}

VOS_VOID AT_NetMonFmtUtranTddNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;

    length = inLen;

    if (nCellInfo->u.nCellInfo.utranNCellCnt > NETMON_MAX_UTRAN_NCELL_NUM) {
        nCellInfo->u.nCellInfo.utranNCellCnt = NETMON_MAX_UTRAN_NCELL_NUM;
    }

    /* WCDMA 临区显示 */
    for (loop = 0; loop < nCellInfo->u.nCellInfo.utranNCellCnt; loop++) {
        /* 如果不是第一次打印邻区，打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: TD_SCDMA,%u,%d,%d,%d",
            nCellInfo->u.nCellInfo.u.tddNCellInfo[loop].arfcn, nCellInfo->u.nCellInfo.u.tddNCellInfo[loop].syncId,
            nCellInfo->u.nCellInfo.u.tddNCellInfo[loop].sc, nCellInfo->u.nCellInfo.u.tddNCellInfo[loop].rscp);
    }

    *outLen = length;

    return;
}
#endif

#if (FEATURE_LTE == FEATURE_ON)

VOS_VOID AT_NetMonFmtEutranSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length)
{
    VOS_UINT8  plmnstr[AT_NETMON_PLMN_STRING_MAX_LENGTH];
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_UINT16 plmnStrLen;
    VOS_UINT16 lengthTemp;

    lengthTemp = 0;
    plmnStrLen = 0;
    (VOS_VOID)memset_s(plmnstr, sizeof(plmnstr), 0x00, sizeof(plmnstr));

    mcc = sCellInfo->u.unSCellInfo.lteSCellInfo.mcc;
    mnc = sCellInfo->u.unSCellInfo.lteSCellInfo.mnc;

    AT_NetMonFmtPlmnId(mcc, mnc, (VOS_CHAR *)plmnstr, &plmnStrLen);

    /* 格式输出PLMN */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "^MONSC: LTE,%s", plmnstr);

    lengthTemp +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%u,%X,%X,%X,%d,%d,%d",
            sCellInfo->u.unSCellInfo.lteSCellInfo.arfcn, sCellInfo->u.unSCellInfo.lteSCellInfo.cellID,
            sCellInfo->u.unSCellInfo.lteSCellInfo.pid, sCellInfo->u.unSCellInfo.lteSCellInfo.tac,
            sCellInfo->u.unSCellInfo.lteSCellInfo.rsrp, sCellInfo->u.unSCellInfo.lteSCellInfo.rsrq,
            sCellInfo->u.unSCellInfo.lteSCellInfo.rssi);
    *length = lengthTemp;

    return;
}


VOS_VOID AT_NetMonFmtEutranNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;

    length = inLen;

    if (nCellInfo->u.nCellInfo.lteNCellCnt > NETMON_MAX_LTE_NCELL_NUM) {
        nCellInfo->u.nCellInfo.lteNCellCnt = NETMON_MAX_LTE_NCELL_NUM;
    }

    /* LTE邻区显示 */
    for (loop = 0; loop < nCellInfo->u.nCellInfo.lteNCellCnt; loop++) {
        /* 如果不是第一次打印邻区，打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: LTE,%u,%X,%d,%d,%d",
            nCellInfo->u.nCellInfo.lteNCellInfo[loop].arfcn, nCellInfo->u.nCellInfo.lteNCellInfo[loop].pid,
            nCellInfo->u.nCellInfo.lteNCellInfo[loop].rsrp, nCellInfo->u.nCellInfo.lteNCellInfo[loop].rsrq,
            nCellInfo->u.nCellInfo.lteNCellInfo[loop].rssi);
    }

    *outLen = length;

    return;
}

/*lint -save -e845 -specific(-e845)*/

VOS_VOID AT_FormatRsrp(VOS_UINT8 indexNum, MTA_AT_RsInfoQryCnf *rsInfoQryCnf)
{
    VOS_UINT32 i;
    VOS_UINT16 length;

    length = 0;

    /* 格式化AT+RSRP?查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    /* +RSRP: */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 判断查询操作是否成功 */
    if ((rsInfoQryCnf->result != MTA_AT_RESULT_NO_ERROR) || (rsInfoQryCnf->rsInfoRslt.rsInfoNum == 0)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "NONE");

        g_atSendDataBuff.bufLen = length;
        return;
    }

    for (i = 0; i < TAF_MIN(rsInfoQryCnf->rsInfoRslt.rsInfoNum, AT_MAX_RS_INFO_NUM); i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].cellId);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].earfcn);

        if (rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].rsrp >= 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d.%02d",
                rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].rsrp / AT_RS_INFO_MULTI,
                rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].rsrp % AT_RS_INFO_MULTI);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",-%d.%02d",
                (-rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].rsrp) / AT_RS_INFO_MULTI,
                (-rsInfoQryCnf->rsInfoRslt.u.rsrpInfo[i].rsrp) % AT_RS_INFO_MULTI);
        }

        if (i == rsInfoQryCnf->rsInfoRslt.rsInfoNum - 1) {
            break;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    g_atSendDataBuff.bufLen = length;
    return;
}


VOS_VOID AT_FormatRsrq(VOS_UINT8 indexNum, MTA_AT_RsInfoQryCnf *rsInfoQryCnf)
{
    VOS_UINT32 i;
    VOS_UINT16 length;

    length = 0;

    /* 格式化AT+RSRQ?查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    /* +RSRQ: */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 判断查询操作是否成功 */
    if ((rsInfoQryCnf->result != MTA_AT_RESULT_NO_ERROR) || (rsInfoQryCnf->rsInfoRslt.rsInfoNum == 0)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "NONE");

        g_atSendDataBuff.bufLen = length;
        return;
    }

    for (i = 0; i < TAF_MIN(rsInfoQryCnf->rsInfoRslt.rsInfoNum, AT_MAX_RS_INFO_NUM); i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].cellId);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].earfcn);

        if (rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].rsrq >= 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d.%02d",
                rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].rsrq / AT_RS_INFO_MULTI,
                rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].rsrq % AT_RS_INFO_MULTI);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",-%d.%02d",
                (-rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].rsrq) / AT_RS_INFO_MULTI,
                (-rsInfoQryCnf->rsInfoRslt.u.rsrqInfo[i].rsrq) % AT_RS_INFO_MULTI);
        }

        if (i == rsInfoQryCnf->rsInfoRslt.rsInfoNum - 1) {
            break;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    g_atSendDataBuff.bufLen = length;
    return;
}
/*lint -restore*/
#endif


VOS_VOID AT_FormatGasAtCmdRslt(MTA_AT_GasAutotestQryRslt *atCmdRslt)
{
    VOS_UINT32 i;
    VOS_UINT32 j;
    VOS_UINT16 length;

    i      = 0;
    j      = 0;
    length = 0;

    for (i = 0; i < (atCmdRslt->rsltNum / AT_RSLT_BASE_NUM); i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d:%d,", (i + 1), atCmdRslt->rslt[j]);
        j++;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "0x%X,", atCmdRslt->rslt[j] & 0xff);
        j++;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d\r\n", atCmdRslt->rslt[j]);
        j++;
    }

    g_atSendDataBuff.bufLen = length;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_NetMonFmtNrSCellData(MTA_AT_NetmonCellInfo *sCellInfo, VOS_UINT16 *length)
{
    VOS_UINT8  plmnstr[AT_NETMON_PLMN_STRING_MAX_LENGTH];
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
    VOS_UINT16 plmnStrLen;
    VOS_UINT16 lengthTemp;
    VOS_UINT8  cellIdStr[AT_CELLID_STRING_MAX_LEN];

    lengthTemp = 0;
    plmnStrLen = 0;
    (VOS_VOID)memset_s(plmnstr, sizeof(plmnstr), 0x00, sizeof(plmnstr));
    (VOS_VOID)memset_s(cellIdStr, sizeof(cellIdStr), 0x00, sizeof(cellIdStr));

    mcc = sCellInfo->u.unSCellInfo.nrSCellInfo.mcc;
    mnc = sCellInfo->u.unSCellInfo.nrSCellInfo.mnc;

    AT_NetMonFmtPlmnId(mcc, mnc, (VOS_CHAR *)plmnstr, &plmnStrLen);

    AT_ConvertCellIdToHexStrFormat(sCellInfo->u.unSCellInfo.nrSCellInfo.cellIdentityLowBit,
                                   sCellInfo->u.unSCellInfo.nrSCellInfo.cellIdentityHighBit, (VOS_CHAR *)cellIdStr);

    /* 格式输出PLMN */
    lengthTemp +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "^MONSC: NR,%s,%u,%d,%s,%X,%X,%d,%d,%d", plmnstr,
            sCellInfo->u.unSCellInfo.nrSCellInfo.arfcn, sCellInfo->u.unSCellInfo.nrSCellInfo.nrScsType, cellIdStr,
            sCellInfo->u.unSCellInfo.nrSCellInfo.phyCellId, sCellInfo->u.unSCellInfo.nrSCellInfo.tac,
            sCellInfo->u.unSCellInfo.nrSCellInfo.rsrp, sCellInfo->u.unSCellInfo.nrSCellInfo.rsrq,
            sCellInfo->u.unSCellInfo.nrSCellInfo.sinr);

    *length = lengthTemp;

    return;
}


VOS_VOID AT_NetMonFmtNrSSCellData(MTA_AT_NetmonCellInfo *cellInfo, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;
    VOS_UINT32 nrCcCnt;

    length  = 0;
    nrCcCnt = cellInfo->u.secSrvCellInfo.componentCarrierNum;

    if (nrCcCnt > NETMON_MAX_NR_CC_NUM) {
        nrCcCnt = NETMON_MAX_NR_CC_NUM;
    }

    /* NR CC打印 */
    for (loop = 0; loop < nrCcCnt; loop++) {
        /* 如果不是第一次打印CC，打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONSSC: NR,%u,%X,%d,%d,%d,%d",
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].arfcn,
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].phyCellId,
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].rsrp,
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].rsrq,
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].sinr,
            cellInfo->u.secSrvCellInfo.componentCarrierInfo[loop].measRsType);
    }

    *outLen = length;

    return;
}


VOS_VOID AT_NetMonFmtNrNCellData(MTA_AT_NetmonCellInfo *nCellInfo, VOS_UINT16 inLen, VOS_UINT16 *outLen)
{
    VOS_UINT32 loop;
    VOS_UINT16 length;
    VOS_UINT8  nrNCellCnt;

    length     = inLen;
    nrNCellCnt = nCellInfo->u.nCellInfo.nrNCellCnt;

    if (nrNCellCnt > NETMON_MAX_NR_NCELL_NUM) {
        nrNCellCnt = NETMON_MAX_NR_NCELL_NUM;
    }

    /* NR邻区显示 */
    for (loop = 0; loop < nrNCellCnt; loop++) {
        /* 如果不是第一次打印邻区，打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^MONNC: NR,%u,%X,%d,%d,%d",
            nCellInfo->u.nCellInfo.nrNCellInfo[loop].arfcn, nCellInfo->u.nCellInfo.nrNCellInfo[loop].phyCellId,
            nCellInfo->u.nCellInfo.nrNCellInfo[loop].rsrp, nCellInfo->u.nCellInfo.nrNCellInfo[loop].rsrq,
            nCellInfo->u.nCellInfo.nrNCellInfo[loop].sinr);
    }

    *outLen = length;

    return;
}


VOS_UINT32 AT_RcvMtaSetTrxTasCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *mtaMsg = VOS_NULL_PTR;
    MTA_AT_SetTrxTasCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    indexNum = 0;
    mtaMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetTrxTasCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetTrxTasCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetTrxTasCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TRX_TAS_SET) {
        AT_WARN_LOG("AT_RcvMtaSetTrxTasCnf: WARNING:Not AT_CMD_TRX_TAS_SET!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryTrxTasCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *mtaMsg = VOS_NULL_PTR;
    MTA_AT_QryTrxTasCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    indexNum = 0;
    mtaMsg   = (AT_MTA_Msg *)msg;
    qryCnf   = (MTA_AT_QryTrxTasCnf *)mtaMsg->content;

    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryTrxTasCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryTrxTasCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TRX_TAS_QRY) {
        AT_WARN_LOG("AT_RcvMtaQryTrxTasCnf: WARNING:Not AT_CMD_TRX_TAS_QRY !");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (qryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, qryCnf->ratMode,
            qryCnf->trxTasValue);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_FmtNrSsbIdData(MTA_AT_NrSsbIdQryCnf *ssbIdInfo, VOS_UINT16 *outLen)
{
    VOS_UINT32 loopI;
    VOS_UINT32 loopJ;
    VOS_UINT16 length;
    VOS_UINT32 cgiHighBit;
    VOS_UINT32 cgiLowBit;

    /* 如果连服务小区都没有，那么说明接入层上报逻辑错误，直接返回 */
    if (ssbIdInfo->spcellPresent == VOS_FALSE) {
        AT_WARN_LOG("AT_FmtNrSsbIdData : Rrc Error spcell not present.");
        *outLen = 0;
        return;
    }

    /* 如果邻区上报存在，但是个数为0，那么说明接入层上报逻辑错误，直接返回 */
    if ((ssbIdInfo->ncellPresent == VOS_TRUE) && (ssbIdInfo->ncellNum == 0)) {
        AT_WARN_LOG("AT_FmtNrSsbIdData : Rrc Error ncell present but cellnum is 0.");
        *outLen = 0;
        return;
    }

    /* 如果邻区上报不存在，但是个数不为0，那么说明接入层上报逻辑错误，直接返回 */
    if ((ssbIdInfo->ncellPresent == VOS_FALSE) && (ssbIdInfo->ncellNum != 0)) {
        AT_WARN_LOG("AT_FmtNrSsbIdData : Rrc Error ncell not present but cellnum is not 0.");
        *outLen = 0;
        return;
    }

    length = 0;

    /* Cgi */
    cgiLowBit  = ssbIdInfo->spcellSsbInfo.cgiLow;
    cgiHighBit = ssbIdInfo->spcellSsbInfo.cgiHigh;
    /* 接入层逻辑保证标志位可以代表数据的正确性，所以不判断数据是否为无效值 */

    /* NR SSB info打印 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^NRSSBID:%u,%08x%08x,%u,%d,%d,%d", ssbIdInfo->spcellSsbInfo.arFcn,
        cgiHighBit, cgiLowBit, ssbIdInfo->spcellSsbInfo.phyCellId, ssbIdInfo->spcellSsbInfo.rsrp,
        ssbIdInfo->spcellSsbInfo.sinr, ssbIdInfo->spcellSsbInfo.ta);

    for (loopI = 0; loopI < MTA_AT_MAX_NR_SSB_BEAM_NUM; loopI++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d", ssbIdInfo->spcellSsbInfo.pscellMeasRslt[loopI].ssbId,
            ssbIdInfo->spcellSsbInfo.pscellMeasRslt[loopI].rsrp);
    }

    /* 邻区数目 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", ssbIdInfo->ncellNum);

    if (ssbIdInfo->ncellPresent == VOS_TRUE) {
        /* 邻区信息打印 */
        for (loopI = 0; loopI < ssbIdInfo->ncellNum; loopI++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%u,%u,%d,%d", ssbIdInfo->ncellSsbInfo[loopI].phyCellId,
                ssbIdInfo->ncellSsbInfo[loopI].arFcn, ssbIdInfo->ncellSsbInfo[loopI].rsrp,
                ssbIdInfo->ncellSsbInfo[loopI].sinr);

            for (loopJ = 0; loopJ < MTA_AT_MAX_NR_NCELL_BEAM_RPT_NUM; loopJ++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d",
                    ssbIdInfo->ncellSsbInfo[loopI].ncellMeasRslt[loopJ].ssbId,
                    ssbIdInfo->ncellSsbInfo[loopI].ncellMeasRslt[loopJ].rsrp);
            }
        }
    }

    *outLen = length;

    return;
}
#endif


VOS_UINT32 AT_CheckJDCfgGsmPara(VOS_VOID)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    /* 判断平台是否支持GSM */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_GSM) != VOS_TRUE) {
        AT_ERR_LOG("AT_CheckJDCfgGsmPara: Not Support GSM.");
        return VOS_ERR;
    }

    /* 设置命令格式:AT^JDCFG=0,<rssi_thresh>,<rssi_num> */
    if (g_atParaIndex != AT_JAM_DETECT_GSM_PARA_NUM) {
        return VOS_ERR;
    }

    /* 配置参数范围检查 */
    if ((g_atParaList[AT_JDCFG_GSM_RSSI_THRESH].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_GSM_RSSI_THRESH].paraValue > AT_JAM_DETECT_GSM_THRESHOLD_MAX)) {
        return VOS_ERR;
    }

    if ((g_atParaList[AT_JDCFG_GSM_RSSI_NUM].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_GSM_RSSI_NUM].paraValue > AT_JAM_DETECT_GSM_FREQNUM_MAX)) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckJDCfgWcdmaPara(VOS_VOID)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    /* 判断平台是否支持WCDMA */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_WCDMA) != VOS_TRUE) {
        AT_ERR_LOG("AT_CheckJDCfgWcdmaPara: Not Support WCDMA.");
        return VOS_ERR;
    }

    /* 设置命令格式:AT^JDCFG=1,<rssi_thresh>,<rssi_percent>,<psch_thresh>,<psch_percent> */
    if (g_atParaIndex != AT_JAM_DETECT_WL_PARA_NUM) {
        return VOS_ERR;
    }

    /* 配置参数范围检查 */
    /* 第二个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_WCDMA_RSSI_THRESH].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_WCDMA_RSSI_THRESH].paraValue > AT_JAM_DETECT_WCDMA_RSSI_THRESHOLD_MAX)) {
        return VOS_ERR;
    }
     /* 第三个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_WCDMA_RSSI_PERCENT].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_WCDMA_RSSI_PERCENT].paraValue > AT_JAM_DETECT_WCDMA_RSSI_PERCENT_MAX)) {
        return VOS_ERR;
    }
    /* 第四个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_WCDMA_PSCH_THRESH].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_WCDMA_PSCH_THRESH].paraValue > AT_JAM_DETECT_WCDMA_PSCH_THRESHOLD_MAX)) {
        return VOS_ERR;
    }
     /* 第五个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_WCDMA_PSCH_PERCENT].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_WCDMA_PSCH_PERCENT].paraValue > AT_JAM_DETECT_WCDMA_PSCH_PERCENT_MAX)) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckJDCfgLtePara(VOS_VOID)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    /* 判断平台是否支持LTE */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_LTE) != VOS_TRUE) {
        AT_ERR_LOG("AT_CheckJDCfgLtePara: Not Support LTE.");
        return VOS_ERR;
    }

    /* 设置命令格式:AT^JDCFG=1,<rssi_thresh>,<rssi_percent>,<pssratio_thresh>,<pssratio_percent> */
    if (g_atParaIndex != AT_JAM_DETECT_WL_PARA_NUM) {
        return VOS_ERR;
    }

    /* 配置参数范围检查 */
     /* 第二个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_LTE_RSSI_THRESH].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_LTE_RSSI_THRESH].paraValue > AT_JAM_DETECT_LTE_RSSI_THRESHOLD_MAX)) {
        return VOS_ERR;
    }
     /* 第三个参数不为空，否则返回ERR */
    if ((g_atParaList[AT_JDCFG_LTE_RSSI_PERCENT].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_LTE_RSSI_PERCENT].paraValue > AT_JAM_DETECT_LTE_RSSI_PERCENT_MAX)) {
        return VOS_ERR;
    }

    if ((g_atParaList[AT_JDCFG_LTE_PSSRATIO_THRESH].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_LTE_PSSRATIO_THRESH].paraValue > AT_JAM_DETECT_LTE_PSSRATIO_THRESHOLD_MAX)) {
        return VOS_ERR;
    }

    if ((g_atParaList[AT_JDCFG_LTE_PSSRATIO_PERCENT].paraLen == 0) ||
        (g_atParaList[AT_JDCFG_LTE_PSSRATIO_PERCENT].paraValue > AT_JAM_DETECT_LTE_PSSRATIO_PERCENT_MAX)) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckJDCfgPara(VOS_VOID)
{
    VOS_UINT32 rst = VOS_ERR;

    /* 参数个数检测 */
    if ((g_atParaIndex != AT_JAM_DETECT_GSM_PARA_NUM) && (g_atParaIndex != AT_JAM_DETECT_WL_PARA_NUM)) {
        return VOS_ERR;
    }

    /* 检测GUL制式下，JD配置参数有效性 */
    switch ((AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue) {
        case AT_MTA_CMD_RATMODE_GSM:
            rst = AT_CheckJDCfgGsmPara();
            break;

        case AT_MTA_CMD_RATMODE_WCDMA:
            rst = AT_CheckJDCfgWcdmaPara();
            break;

        case AT_MTA_CMD_RATMODE_LTE:
            rst = AT_CheckJDCfgLtePara();
            break;

        default:
            break;
    }

    return rst;
}


VOS_UINT32 AT_ProcJDCfgPara(AT_MTA_SetJamDetectReq *atCmd)
{
    /* 获取GUL制式下，干扰检测配置参数 */
    atCmd->mode = AT_MTA_JAM_DETECT_MODE_UPDATE;
    atCmd->rat  = (AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue;

    switch ((AT_MTA_CmdRatmodeUint8)g_atParaList[0].paraValue) {
        case AT_MTA_CMD_RATMODE_GSM:
            atCmd->unJamPara.gsmPara.threshold = (VOS_UINT8)g_atParaList[1].paraValue;
            atCmd->unJamPara.gsmPara.freqNum   = (VOS_UINT8)g_atParaList[AT_JDCFG_RSSISRHFREQ_PERCENT].paraValue;
            break;

        case AT_MTA_CMD_RATMODE_WCDMA:
            atCmd->unJamPara.wcdmaPara.rssiSrhThreshold   = (VOS_UINT8)g_atParaList[1].paraValue;
            atCmd->unJamPara.wcdmaPara.rssiSrhFreqPercent =
                (VOS_UINT8)g_atParaList[AT_JDCFG_RSSISRHFREQ_PERCENT].paraValue;
            atCmd->unJamPara.wcdmaPara.pschSrhThreshold   =
                (VOS_UINT16)g_atParaList[AT_JDCFG_PSCHSRHTHRE_SHOID].paraValue;
            atCmd->unJamPara.wcdmaPara.pschSrhFreqPercent =
                (VOS_UINT8)g_atParaList[AT_JDCFG_PSCHSRHFREQ_PERCENT].paraValue;
            break;

        case AT_MTA_CMD_RATMODE_LTE:
            atCmd->unJamPara.ltePara.rssiThresh = (VOS_INT16)g_atParaList[1].paraValue -
                                                  AT_JAM_DETECT_LTE_RSSI_THRESHOLD_MAX;
            atCmd->unJamPara.ltePara.rssiPercent     = (VOS_UINT8)g_atParaList[AT_JDCFG_RSSISRHFREQ_PERCENT].paraValue;
            atCmd->unJamPara.ltePara.pssratioThresh  = (VOS_UINT16)g_atParaList[AT_JDCFG_PSCHSRHTHRE_SHOID].paraValue;
            atCmd->unJamPara.ltePara.pssratioPercent = (VOS_UINT8)g_atParaList[AT_JDCFG_PSCHSRHFREQ_PERCENT].paraValue;
            break;

        default:
            return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetJDCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetJamDetectReq atCmd;
    VOS_UINT32             rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数有效性检查 */
    rst = AT_CheckJDCfgPara();

    if (rst != VOS_OK) {
        AT_ERR_LOG("AT_SetJDCfgPara: AT_CheckJDCfgPara error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    /* 获取干扰检测配置参数 */
    rst = AT_ProcJDCfgPara(&atCmd);

    if (rst != VOS_OK) {
        return AT_ERROR;
    }

    /* 下发干扰检测参数更新请求消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SET_JAM_DETECT_REQ, (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_SetJamDetectReq),
                                 I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_JDCFG_SET;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryJDCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 下发干扰检测配置查询请求消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_QRY_JAM_DETECT_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_JDCFG_READ;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TestJDCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length = 0;

    /* 输出GSM的参数列表 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (0),(0-70),(0-255)%s",
        g_parseContext[indexNum].cmdElement->cmdName, g_atCrLf);

    /* 输出WCDMA的参数列表 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (1),(0-70),(0-100),(0-65535),(0-100)%s",
        g_parseContext[indexNum].cmdElement->cmdName, g_atCrLf);

    /* 输出LTE的参数列表 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (2),(0-70),(0-100),(0-1000),(0-100)",
        g_parseContext[indexNum].cmdElement->cmdName);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_SetJDSwitchPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetJamDetectReq atCmd = {0};
    VOS_UINT32             rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if (g_atParaIndex != 1) {
        return AT_ERROR;
    }

    /* 下发干扰检测开关配置请求消息给C核处理 */
    atCmd.mode = (VOS_UINT8)g_atParaList[0].paraValue;
    atCmd.rat  = AT_MTA_CMD_RATMODE_BUTT;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SET_JAM_DETECT_REQ, (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_SetJamDetectReq),
                                 I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_JDSWITCH_SET;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryJDSwitchPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 下发JD查询请求消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_QRY_JAM_DETECT_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_JDSWITCH_READ;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaSetJamDetectCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_SetJamDetectCnf *setCnf   = VOS_NULL_PTR;
    VOS_UINT32              result   = AT_OK;
    VOS_UINT8               indexNum = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    setCnf = (MTA_AT_SetJamDetectCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetJamDetectCfgCnf : WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetJamDetectCfgCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_JDCFG_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_JDSWITCH_SET)) {
        AT_WARN_LOG("AT_RcvMtaSetJamDetectCfgCnf : Current Option is not AT_CMD_JDCFG_SET or AT_CMD_JDSWITCH_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryJamDetectCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_QryJamDetectCnf *qryCnf   = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;
    VOS_UINT16              length   = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    qryCnf = (MTA_AT_QryJamDetectCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryJamDetectCfgCnf : WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryJamDetectCfgCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_JDCFG_READ:
            /* GSM制式，干扰检测配置参数 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d%s",
                g_parseContext[indexNum].cmdElement->cmdName, AT_MTA_CMD_RATMODE_GSM, qryCnf->gsmPara.threshold,
                qryCnf->gsmPara.freqNum, g_atCrLf);

            /* WCDMA制式，干扰检测配置参数 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d%s",
                g_parseContext[indexNum].cmdElement->cmdName, AT_MTA_CMD_RATMODE_WCDMA,
                qryCnf->wcdmaPara.rssiSrhThreshold, qryCnf->wcdmaPara.rssiSrhFreqPercent,
                qryCnf->wcdmaPara.pschSrhThreshold, qryCnf->wcdmaPara.pschSrhFreqPercent, g_atCrLf);

            /* LTE制式，干扰检测配置参数 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, AT_MTA_CMD_RATMODE_LTE,
                qryCnf->ltePara.rssiThresh + AT_JAM_DETECT_LTE_RSSI_THRESHOLD_MAX, qryCnf->ltePara.rssiPercent,
                qryCnf->ltePara.pssratioThresh, qryCnf->ltePara.pssratioPercent);

            g_atSendDataBuff.bufLen = length;

            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_OK);

            return VOS_OK;

        case AT_CMD_JDSWITCH_READ:
            /* GSM制式，干扰检测开关状态 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d%s", g_parseContext[indexNum].cmdElement->cmdName,
                qryCnf->gsmJamMode, AT_MTA_CMD_RATMODE_GSM, g_atCrLf);

            /* WCDMA制式，干扰检测开关状态 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d%s", g_parseContext[indexNum].cmdElement->cmdName,
                qryCnf->wcdmaJamMode, AT_MTA_CMD_RATMODE_WCDMA, g_atCrLf);

            /* LTE制式，干扰检测开关状态 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                qryCnf->lteJamMode, AT_MTA_CMD_RATMODE_LTE);

            g_atSendDataBuff.bufLen = length;

            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_OK);

            return VOS_OK;

        default:
            AT_WARN_LOG(
                "AT_RcvMtaMbbQryJamDetectCnf : Current Option is not AT_CMD_JDCFG_READ or AT_CMD_JDSWITCH_READ.");

            return VOS_ERR;
    }
}


VOS_UINT32 AT_RcvMtaJamDetectInd(struct MsgCB *msg)
{
    VOS_UINT8            indexNum     = 0;
    VOS_UINT16           length       = 0;
    AT_MTA_Msg          *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_JamDetectInd *jamDetectInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    mtaMsg       = (AT_MTA_Msg *)msg;
    jamDetectInd = (MTA_AT_JamDetectInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaJamDetectInd: WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    /* 上报干扰检测结果 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^JDINFO: %d,%d%s", g_atCrLf, jamDetectInd->jamResult, jamDetectInd->rat,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_SetEmRssiCfgPara(VOS_UINT8 indexNum)
{
    TAF_MMA_EmrssicfgReq emRssiCfgPara;

    (VOS_VOID)memset_s(&emRssiCfgPara, sizeof(emRssiCfgPara), 0x00, sizeof(emRssiCfgPara));

    /* 检查是否设置命令 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetEmRssiCfgPara: Not Set Command!");
        return AT_ERROR;
    }

    /* 检查参数个数 */
    if (g_atParaIndex != AT_EMRSSICFG_PARA_NUM) {
        AT_WARN_LOG("At_SetEmRssiCfgPara: Input parameters go wrong!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将入参封装到结构体中再发起请求 */
    emRssiCfgPara.emRssiCfgRat       = (VOS_UINT8)g_atParaList[0].paraValue;
    emRssiCfgPara.emRssiCfgThreshold = (VOS_UINT8)g_atParaList[1].paraValue;

    /* 发送消息给C核处理 */
    if (TAF_MMA_SetEmRssiCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                &emRssiCfgPara) == VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMRSSICFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("At_SetEmRssiCfgPara: AT send ERROR!");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryEmRssiCfgPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_MMA_QryEmRssiCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId) ==
        VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMRSSICFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetEmRssiRptPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 emRssiRptSwitch;

    /* 检查是否设置命令 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetEmRssiRptPara: Not Set Command!");
        return AT_ERROR;
    }

    /* 检查参数个数 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("At_SetEmRssiRptPara: Input parameters go wrong!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    emRssiRptSwitch = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 发送消息给C核处理 */
    if (TAF_MMA_SetEmRssiRptReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                &emRssiRptSwitch) == VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMRSSIRPT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryEmRssiRptPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_MMA_QryEmRssiRptReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId) ==
        VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMRSSIRPT_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaEmRssiCfgSetCnf(struct MsgCB *msg)
{
    TAF_MMA_EmrssicfgSetCnf *cnfMsg   = VOS_NULL_PTR;
    VOS_UINT8                indexNum = 0;
    VOS_UINT32               result   = AT_OK;

    cnfMsg = (TAF_MMA_EmrssicfgSetCnf *)msg;

    /* 通过clientid获取ucIndex */
    if (At_ClientIdToUserId(cnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMRSSICFG_SET) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgSetCnf: Current Option is not AT_CMD_EMRSSICFG_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnfMsg->errorCause != TAF_ERR_NO_ERROR) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgSetCnf: Set command go wrong!");
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEmRssiCfgQryCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum        = 0;
    VOS_UINT16               length          = 0;
    AT_RreturnCodeUint32     result          = AT_FAILURE;
    TAF_MMA_EmrssicfgQryCnf *emRssiCfgQryCnf = VOS_NULL_PTR;

    emRssiCfgQryCnf = (TAF_MMA_EmrssicfgQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(emRssiCfgQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMRSSICFG_QRY) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf: Current Option is not AT_CMD_EMRSSICFG_QRY.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (emRssiCfgQryCnf->errorCause != TAF_ERR_NO_ERROR) {
        result = AT_ERROR;
    } else {
        /* GSM EMRSSICFG输出 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d%s", g_parseContext[indexNum].cmdElement->cmdName,
            TAF_MMA_RAT_GSM, emRssiCfgQryCnf->emRssiCfgPara.emRssiCfgGsmThreshold, g_atCrLf);

        /* WCDMA EMRSSICFG输出 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d%s", g_parseContext[indexNum].cmdElement->cmdName,
            TAF_MMA_RAT_WCDMA, emRssiCfgQryCnf->emRssiCfgPara.emRssiCfgWcdmaThreshold, g_atCrLf);

        /* LTE EMRSSICFG输出 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            TAF_MMA_RAT_LTE, emRssiCfgQryCnf->emRssiCfgPara.emRssiCfgLteThreshold);

        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEmRssiRptSetCnf(struct MsgCB *msg)
{
    TAF_MMA_EmrssirptSetCnf *cnfMsg   = VOS_NULL_PTR;
    VOS_UINT8                indexNum = 0;
    VOS_UINT32               result   = AT_ERROR;

    cnfMsg = (TAF_MMA_EmrssirptSetCnf *)msg;

    /* 通过clientid获取ucIndex */
    if (At_ClientIdToUserId(cnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmRssiRptSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEmRssiRptSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMRSSIRPT_SET) {
        AT_WARN_LOG("AT_RcvMmaEmRssiRptSetCnf: Current Option is not AT_CMD_EMRSSIRPT_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnfMsg->errorCause != TAF_ERR_NO_ERROR) {
        AT_WARN_LOG("AT_RcvMmaEmRssiRptSetCnf: Set command go wrong!");
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEmRssiRptQryCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum        = 0;
    AT_RreturnCodeUint32     result          = AT_FAILURE;
    TAF_UINT16               length          = 0;
    TAF_MMA_EmrssirptQryCnf *emRssiRptQryCnf = VOS_NULL_PTR;

    emRssiRptQryCnf = (TAF_MMA_EmrssirptQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(emRssiRptQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf: AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMRSSIRPT_QRY) {
        AT_WARN_LOG("AT_RcvMmaEmRssiCfgQryCnf: Current Option is not AT_CMD_EMRSSIRPT_QRY.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (emRssiRptQryCnf->errorCause != TAF_ERR_NO_ERROR) {
        result = AT_ERROR;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            emRssiRptQryCnf->emRssiRptSwitch);

        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_ConvertRssiLevel(VOS_INT16 rssiValue, VOS_UINT8 *rssiLevel)
{
    if (rssiValue >= AT_HCSQ_VALUE_INVALID) {
        *rssiLevel = AT_HCSQ_VALUE_INVALID;
    } else if (rssiValue >= AT_HCSQ_RSSI_VALUE_MAX) {
        *rssiLevel = AT_HCSQ_RSSI_LEVEL_MAX;
    } else if (rssiValue < AT_HCSQ_RSSI_VALUE_MIN) {
        *rssiLevel = AT_HCSQ_LEVEL_MIN;
    } else {
        *rssiLevel = (VOS_UINT8)((rssiValue - AT_HCSQ_RSSI_VALUE_MIN) + 1);
    }

    return;
}


VOS_UINT32 AT_RcvMmaEmRssiRptInd(struct MsgCB *msg)
{
    TAF_MMA_RssiInfoInd *emRssiRptInd = VOS_NULL_PTR;
    VOS_UINT8            rssiValue    = 0;
    VOS_UINT8            indexNum     = 0;
    VOS_UINT16           length       = 0;
    VOS_INT16            emRssi       = 0;

    emRssiRptInd = (TAF_MMA_RssiInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(emRssiRptInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmRssiRptInd:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    switch (emRssiRptInd->rssiInfo.ratType) {
        case TAF_MMA_RAT_GSM: {
            emRssi = emRssiRptInd->rssiInfo.aRssi[0].u.gCellSignInfo.rssiValue;
            break;
        }
        case TAF_MMA_RAT_WCDMA: {
            emRssi = emRssiRptInd->rssiInfo.aRssi[0].u.wCellSignInfo.rscpValue -
                     emRssiRptInd->rssiInfo.aRssi[0].u.wCellSignInfo.ecioValue;
            break;
        }
        case TAF_MMA_RAT_LTE: {
            emRssi = emRssiRptInd->rssiInfo.aRssi[0].u.lCellSignInfo.rssi;
            break;
        }
        default: {
            return VOS_ERR;
        }
    }

    AT_ConvertRssiLevel(emRssi, &rssiValue);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_EMRSSIRPT].text,
        emRssiRptInd->rssiInfo.ratType, rssiValue, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}



LOCAL VOS_UINT32 AT_LtePwrDissParaCheck(AT_MTA_LtepwrdissSetReq *atCmd)
{
    VOS_INT32  val;
    VOS_UINT32 rst;

    val = 0;
    if (AT_AtoInt((VOS_CHAR *)g_atParaList[1].para, &val) == VOS_ERR) {
        return VOS_ERR;
    }

    rst = VOS_ERR;
    switch ((AT_LTEPWRCTRL_MODE_TYPE_ENUM_UINT32)g_atParaList[0].paraValue) {
        case AT_LTEPWRCTRL_MODE_CC_NUM_CTR: {
            if ((val >= AT_LTEPWRCTRL_MAX_DELETE_CA_NUM) && (val <= AT_LTEPWRCTRL_MAX_ADD_CA_NUM)) {
                rst = VOS_OK;
            }
            break;
        }
        case AT_LTEPWRCTRL_MODE_RI_NUM_CTR: {
            if ((val == AT_LTEPWRCTRL_DISABLE_REDUCE_RI) || (val == AT_LTEPWRCTRL_ENABLE_REDUCE_RI)) {
                rst = VOS_OK;
            }
            break;
        }
        case AT_LTEPWRCTRL_MODE_BSR_NUM_CTR: {
            if ((val >= AT_LTEPWRCTRL_MIN_SUPPORT_BSR_NUM) && (val <= AT_LTEPWRCTRL_MAX_SUPPORT_BSR_NUM)) {
                rst = VOS_OK;
            }
            break;
        }
        default: {
            break;
        }
    }

    if (rst == VOS_ERR) {
        return VOS_ERR;
    }

    atCmd->mode = (VOS_UINT8)g_atParaList[0].paraValue;
    atCmd->para = (VOS_INT16)val;

    return VOS_OK;
}


VOS_UINT32 AT_SetLtePwrDissPara(VOS_UINT8 indexNum)
{
    AT_MTA_LtepwrdissSetReq atCmd;
    VOS_UINT32              rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_LTEPWRCTRL_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数1和参数2的长度不能为0 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数2的输入最大范围为(-8~63),长度不能大于2 */
    if (g_atParaList[AT_LTEPWRCTRL_PARA].paraLen > AT_LTEPWRCTRL_PARA_MAX_LEN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&atCmd, sizeof(AT_MTA_LtepwrdissSetReq), 0, sizeof(AT_MTA_LtepwrdissSetReq));

    rst = AT_LtePwrDissParaCheck(&atCmd);

    if (rst == VOS_ERR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 下发参数更新请求消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_LTEPWRDISS_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_LtepwrdissSetReq), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEPWRDISS_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_TestLtePwrDissPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: (0-2),(-8-63)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}


VOS_UINT32 AT_RcvMtaAtLtePwrDissSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg                = VOS_NULL_PTR;
    AT_MTA_LtepwrdissSetCnf *mtaAtLtePwrDissSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum              = 0;

    /* 初始化 */
    rcvMsg                = (AT_MTA_Msg *)msg;
    mtaAtLtePwrDissSetCnf = (AT_MTA_LtepwrdissSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaAtLtePwrDissSetCnf:WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaAtLtePwrDissSetCnf:AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEPWRDISS_SET) {
        AT_WARN_LOG("AT_RcvMtaAtLtePwrDissSetCnf:Current Option is not AT_CMD_LTEPWRDISS_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (mtaAtLtePwrDissSetCnf->result != MTA_AT_RESULT_NO_ERROR) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMtaLowPwrModeSetCnf(struct MsgCB *msg)
{
    /* 初始化 */
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_LowPwrModeCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    result = AT_OK;
    setCnf = (MTA_AT_LowPwrModeCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLowPwrModeSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLowPwrModeSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SFEATURE_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LOWPWRMODE_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ConvertMtaResult(setCnf->result);
    } else {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMtaNrrcCapCfgCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_NrrccapCfgSetCnf *nrrcCapCfgCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg        = (AT_MTA_Msg *)msg;
    nrrcCapCfgCnf = (MTA_AT_NrrccapCfgSetCnf *)rcvMsg->content;
    indexNum      = 0;
    result        = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapCfgCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待AT^NRRCCAPCFG命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRRCCAPCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapCfgCnf : Current Option is not AT_CMD_NRRCCAPCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(nrrcCapCfgCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrrcCapQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_NrrccapQryCnf *nrrcCapQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg        = (AT_MTA_Msg *)msg;
    nrrcCapQryCnf = (MTA_AT_NrrccapQryCnf *)rcvMsg->content;
    indexNum      = 0;
    result        = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待^ERRCCAPQRY命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRRCCAPQRY_SET) {
        AT_WARN_LOG("AT_RcvMtaNrrcCapQryCnf : Current Option is not AT_CMD_NRRCCAPQRY_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (nrrcCapQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* ^NRRCCAPQRY:  */
        g_atSendDataBuff.bufLen +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
                "^NRRCCAPQRY: %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u", nrrcCapQryCnf->nrrcCfgNetMode,
                nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_0], nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_1],
                nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_2], nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_3],
                nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_4], nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_5],
                nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_6], nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_7],
                nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_8], nrrcCapQryCnf->para[AT_COMMAND_PARA_INDEX_9]);

        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(nrrcCapQryCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrPwrCtrlSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg               = VOS_NULL_PTR;
    MTA_AT_NrpwrctrlSetCnf *mtaAtNrPwrCtrlSetCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化 */
    rcvMsg               = (AT_MTA_Msg *)msg;
    mtaAtNrPwrCtrlSetCnf = (MTA_AT_NrpwrctrlSetCnf *)rcvMsg->content;
    result               = AT_ERROR;
    indexNum             = AT_CLIENT_ID_BUTT;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrPwrCtrlSetCnf:WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrPwrCtrlSetCnf:AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRPWRCTRL_SET) {
        AT_WARN_LOG("AT_RcvMtaNrPwrCtrlSetCnf:Current Option is not AT_CMD_NRPWRCTRL_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(mtaAtNrPwrCtrlSetCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif

