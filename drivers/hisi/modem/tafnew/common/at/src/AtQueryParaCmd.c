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
#include "AtQueryParaCmd.h"
#include "securec.h"

#include "at_mdrv_interface.h"
#include "ppp_interface.h"
#include "taf_drv_agent.h"
#include "taf_tafm_remote.h"
#include "ttf_mem_at_interface.h"
#include "ATCmdProc.h"
#include "AtCheckFunc.h"
#include "AtParseCmd.h"
#include "at_phy_interface.h"
#include "AtDataProc.h"
#include "AtCmdMsgProc.h"
#include "AtSndMsg.h"
#include "AtRnicInterface.h"
#include "AtInputProc.h"
#include "AtEventReport.h"
#include "AtDeviceCmd.h"
#include "AtTafAgentInterface.h"
#include "app_vc_api.h"
#include "taf_oam_interface.h"
#include "taf_iface_api.h"
#include "at_lte_eventreport.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "mdrv_nvim.h"
#include "at_common.h"
#include "at_lte_common.h"
#endif

#include "nv_stru_sys.h"
#include "nv_stru_gas.h"
#include "nv_stru_msp_interface.h"
#include "nv_stru_msp.h"

#include "css_at_interface.h"

#include "AtMsgPrint.h"

#include "ps_common_def.h"
#include "ps_lib.h"
#include "product_config.h"
#include "AtCmdMiscProc.h"
#include "taf_ccm_api.h"
#include "mn_comm_api.h"
#if (FEATURE_LTEV == FEATURE_ON)
#include "vnas_at_interface.h"
#endif

#if (VOS_OS_VER == VOS_WIN32)
#include "NasStub.h"
#endif

/*lint -esym(516,free,malloc)*/


#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
extern cmux_info_type* AT_GetCmuxInfo(VOS_VOID);
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_QUERYPARACMD_C
#define AT_PUC_IMSI_ARRAY_INDEX_1 1
#define AT_PUC_IMSI_ARRAY_INDEX_2 2
#define AT_BSN_SERIAL_NUM_LEN 17
#define AT_MNC_LEN_MAX_VALID_VALUE 3
#define AT_MNC_LEN_MIN_VALID_VALUE 2
#define AT_RPT_CFG_BIT_32_INDEX_0 0
#define AT_RPT_CFG_BIT_32_INDEX_1 1
#define AT_RPT_CFG_BIT_32_INDEX_2 2
#define AT_RSRP_CFG_SS_VALUE_LEN 4
#define AT_RSRP_CFG_SS_VALUE_INDEX_3 3
#define AT_RSCP_CFG_SS_VALUE_LEN 4
#define AT_RSCP_CFG_SS_VALUE_INDEX_3 3
#define AT_ECIO_CFG_SS_VALUE_LEN 4
#define AT_ECIO_CFG_SS_VALUE_INDEX_3 3
#define AT_SOCP_DST_CHAN_DELAY 1
#define AT_SOCP_DST_CHAN_DTS 2


#define AT_NVIM_ECIO_VALUE_INDEX_1 1
#define AT_NVIM_ECIO_VALUE_INDEX_2 2
#define AT_NVIM_ECIO_VALUE_INDEX_3 3

#define AT_NVIM_RSCP_VALUE_INDEX_1 1
#define AT_NVIM_RSCP_VALUE_INDEX_2 2
#define AT_NVIM_RSCP_VALUE_INDEX_3 3

#define AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_1 1
#define AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_2 2
#define AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_3 3

#define AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_1 1
#define AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_2 2
#define AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_3 3

/*
 * 3 函数、变量声明
 */


VOS_UINT32 AT_QryGTimerPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;
    VOS_UINT32 gTimerLength;

    gTimerLength = 0;

    /* 读取NV项NV_ITEM_GPRS_ACTIVE_TIMER_LEN获取GPRS定时器时长 */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_GPRS_ACTIVE_TIMER_LEN, &gTimerLength, sizeof(gTimerLength));
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_QryGTimerPara: Fail to read NV_ITEM_GPRS_ACTIVE_TIMER_LEN");
        return AT_DEVICE_OTHER_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, gTimerLength);

    return AT_OK;
}


VOS_UINT32 AT_QryRsimPara(VOS_UINT8 indexNum)
{
    VOS_UINT32       cardStatus;
    VOS_UINT32       cardType;
    ModemIdUint16    modemId;
    VOS_UINT32       getModemIdRslt;
    AT_USIM_InfoCtx *usimInfoCtx = VOS_NULL_PTR;

    modemId = MODEM_ID_0;

    getModemIdRslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (getModemIdRslt != VOS_OK) {
        AT_ERR_LOG("At_SetCardModePara:Get Modem Id fail!");
        return AT_ERROR;
    }

    usimInfoCtx = AT_GetUsimInfoCtxFromModemId(modemId);

    /* 获取 SIM 卡类型和状态 */
    cardType   = usimInfoCtx->cardType;
    cardStatus = usimInfoCtx->cardStatus;

    AT_NORM_LOG1("AT_QryRsimPara: ucCardStatus is ", (VOS_INT32)cardStatus);
    AT_NORM_LOG1("AT_QryRsimPara: ucCardType is ", (VOS_INT32)cardType);

    if (cardStatus == USIMM_CARDAPP_SERVIC_BUTT) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_RSIM_STATUS_USIMM_CARD_BUSY);
        return AT_OK;
    }

    /* 输出卡类型为USIMM_CARD_ROM_SIM指示当前为快速开机等效于无卡 */
    if ((usimInfoCtx->cardMediumType == USIMM_CARD_MEDIUM_ROM) || (cardStatus == USIMM_CARDAPP_SERVIC_ABSENT)) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_RSIM_STATUS_USIMM_CARD_ABSENT);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_RSIM_STATUS_USIMM_CARD_AVAILABLE);
    }

    return AT_OK;
}


VOS_UINT32 AT_QryBatVolPara(VOS_UINT8 indexNum)
{
    /* 获取 电池电压值 */
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_TBATVOLT_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TBATVOLT_QRY; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                 /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_DeciDigit2Ascii(VOS_UINT8 aucDeciDigit[], VOS_UINT32 length, VOS_UINT8 aucAscii[])
{
    VOS_UINT32 loop;

    for (loop = 0; loop < length; loop++) {
        if (aucDeciDigit[loop] > AT_DEC_MAX_NUM) {
            return VOS_ERR;
        }

        aucAscii[loop] = aucDeciDigit[loop] + '0';
    }

    return VOS_OK;
}


VOS_VOID AT_ConvertImsiDigit2String(VOS_UINT8 *imsi, VOS_UINT8 *imsiString, VOS_UINT32 strBufLen)
{
    VOS_UINT8  imsiLen;
    VOS_UINT32 tempNum;
    VOS_UINT8  imsiNum[NAS_IMSI_STR_LEN];
    VOS_UINT32 i;

    AT_NORM_LOG("AT_ConvertImsiDigit2String enter.");

    if (strBufLen <= NAS_IMSI_STR_LEN) {
        return;
    }

    (VOS_VOID)memset_s(imsiNum, sizeof(imsiNum), 0x00, NAS_IMSI_STR_LEN);
    tempNum = 0;

    imsiLen = TAF_MIN(imsi[0], NAS_MAX_IMSI_LENGTH - 1);

    imsiNum[tempNum] = (imsi[AT_PUC_IMSI_ARRAY_INDEX_1] & 0xf0) >> 4;
    tempNum++;

    for (i = AT_PUC_IMSI_ARRAY_INDEX_2; i <= imsiLen; i++) {
        imsiNum[tempNum] = imsi[i] & 0x0f;
        tempNum++;
        imsiNum[tempNum] = (imsi[i] & 0xf0) >> 4;
        tempNum++;
    }

    /* 非'f'的IMSI号转换为字符 */
    i = 0;

    AT_LOG1("AT_ConvertImsiDigit2String: ulTempNum", tempNum);

    while (i < tempNum) {
        if (imsiNum[i] != 0x0f) {
            /* 增加支持A~E转码，将10~15的数字转化成对应的A~E */
            if ((imsiNum[i] >= 10) && (imsiNum[i] < 15)) {
                imsiString[i] = imsiNum[i] - 10 + 'A'; /* 将十进制数转换成字符 */
            } else {
                imsiString[i] = imsiNum[i] + '0';
            }

            i++;
        } else {
            break;
        }
    }

    AT_LOG1("AT_ConvertImsiDigit2String: i", i);

    imsiString[i] = '\0';

    /* GDPR排查不能打印IMSI */

    return;
}


VOS_UINT32 AT_GetPhynumMac(VOS_UINT8 aucMac[], VOS_UINT32 macLen)
{
    VOS_UINT32 loop;
    VOS_UINT32 ret;
    VOS_UINT8  e5GwMacAddr[AT_MAC_ADDR_LEN + 1]; /* MAC地址 */
    VOS_UINT32 e5GwMacAddrOffset;
    VOS_UINT32 macOffset;
    errno_t    memResult;

    (VOS_VOID)memset_s(e5GwMacAddr, sizeof(e5GwMacAddr), 0x00, sizeof(e5GwMacAddr));

    /* 获取MAC地址字符串 */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WIFI_MAC_ADDR, e5GwMacAddr, AT_MAC_ADDR_LEN);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_GetPhynumMac: Fail to read en_Nv_Item_Imei_Svn.");
        return AT_ERROR;
    }

    /* MAC地址格式匹配: 7A:FE:E2:21:11:E4=>7AFEE22111E4 */
    macOffset         = 0;
    e5GwMacAddrOffset = 0;
    for (loop = 0; loop < (1 + AT_PHYNUM_MAC_COLON_NUM); loop++) {
        memResult = memcpy_s(&aucMac[macOffset], macLen - macOffset, &e5GwMacAddr[e5GwMacAddrOffset],
                             AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS);
        TAF_MEM_CHK_RTN_VAL(memResult, macLen - macOffset, AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS);
        macOffset += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS;
        e5GwMacAddrOffset += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS + VOS_StrLen(":");
    }

    aucMac[macLen - 1] = '\0';

    return AT_OK;
}


VOS_UINT32 AT_QryPhyNumPara(VOS_UINT8 indexNum)
{
    VOS_UINT32    ret;
    VOS_UINT16    length;
    VOS_UINT8     asciiImei[TAF_PH_IMEI_LEN + 1];
    TAF_SVN_Data  svn;
    VOS_UINT8     asciiSvn[TAF_SVN_DATA_LENGTH + 1];
    VOS_UINT8     mac[AT_PHYNUM_MAC_LEN + 1]; /* MAC地址 */
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    (VOS_VOID)memset_s(&svn, sizeof(svn), 0x00, sizeof(svn));
    (VOS_VOID)memset_s(asciiImei, sizeof(asciiImei), 0x00, sizeof(asciiImei));

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_QryPhyNumPara: Get modem id fail.");
        return AT_ERROR;
    }

    /* 读取NV项获取IMEI */
    ret = AT_GetImeiValue(modemId, asciiImei, sizeof(asciiImei));

    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_QryPhyNumPara: Fail to read en_NV_Item_IMEI.");
        return AT_ERROR;
    }

    /* 读取NV项获取SVN */
    ret = TAF_ACORE_NV_READ(modemId, NV_ITEM_IMEI_SVN, &svn, sizeof(svn));
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_QryPhyNumPara: Fail to read en_Nv_Item_Imei_Svn.");
        return AT_ERROR;
    } else {
        if (svn.activeFlag != NV_ITEM_ACTIVE) {
            /* nv没配，初始化成字符串'0' */
            (VOS_VOID)memset_s(asciiSvn, sizeof(asciiSvn), '0', TAF_SVN_DATA_LENGTH);
        } else {
            AT_DeciDigit2Ascii(svn.svn, TAF_SVN_DATA_LENGTH, asciiSvn);
        }

        asciiSvn[TAF_SVN_DATA_LENGTH] = '\0';
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:IMEI,%s%s", g_parseContext[indexNum].cmdElement->cmdName, asciiImei,
        g_atCrLf);

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
        ret = AT_GetPhynumMac(mac, AT_PHYNUM_MAC_LEN + 1);
        if (ret != AT_OK) {
            AT_WARN_LOG("AT_QryPhyNumPara: Fail to read en_NV_Item_IMEI.");
            return AT_ERROR;
        }

        /* MAC地址输出 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:MACWLAN,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
            mac, g_atCrLf);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:SVN,%s", g_parseContext[indexNum].cmdElement->cmdName, asciiSvn);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if (FEATURE_RSTRIGGER_SMS == FEATURE_ON)

VOS_UINT32 AT_WriteActiveMessage(ModemIdUint16 modemId, MN_MSG_ActiveMessage *orgActiveMessageInfo,
                                 MN_MSG_ActiveMessage *activeMessageInfo)
{
    errno_t    memResult;
    VOS_UINT32 ret;
    VOS_INT32  memCmpRlt;
    VOS_UINT8 *activeMessagePara = VOS_NULL_PTR;
    VOS_UINT8 *evaluate          = VOS_NULL_PTR;

    /* 1.判断待写入的激活短信参数与NVIM中的参数是否一致；一致则不用写NVIM直接退出 */
    if (orgActiveMessageInfo->activeStatus == activeMessageInfo->activeStatus) {
        if (orgActiveMessageInfo->url.len == activeMessageInfo->url.len) {
            if (orgActiveMessageInfo->msgCoding == activeMessageInfo->msgCoding) {
                memCmpRlt = VOS_MemCmp(orgActiveMessageInfo->url.url, activeMessageInfo->url.url, activeMessageInfo->url.len);
                if (memCmpRlt == 0) {
                    return MN_ERR_NO_ERROR;
                }
            }
        }
    }

    /* 2.写激活短信参数到NVIM中 */
    /* 2.1 为NVIM存储的数据流申请内存 */
    activeMessagePara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (activeMessagePara == VOS_NULL_PTR) {
        return MN_ERR_NOMEM;
    }

    (VOS_VOID)memset_s(activeMessagePara, MN_MSG_ACTIVE_MESSAGE_PARA_LEN, 0x00, MN_MSG_ACTIVE_MESSAGE_PARA_LEN);

    /* 2.2 将激活短信参数数据结构转换成NVIM存储的数据流 */
    evaluate  = activeMessagePara;
    *evaluate = activeMessageInfo->activeStatus;
    evaluate++;

    *evaluate = activeMessageInfo->msgCoding;
    evaluate++;

    *evaluate = (VOS_UINT8)(activeMessageInfo->url.len & 0xff);
    evaluate++;
    *evaluate = (VOS_UINT8)((activeMessageInfo->url.len >> 8) & 0xff);
    evaluate++;
    *evaluate = (VOS_UINT8)((activeMessageInfo->url.len >> 16) & 0xff);
    evaluate++;
    *evaluate = (VOS_UINT8)((activeMessageInfo->url.len >> 24) & 0xff);
    evaluate++;

    /* evaluate指针在memcpy之前偏移6，指针长度需要减去6 */
    if ((VOS_UINT16)activeMessageInfo->url.len > 0) {
        memResult = memcpy_s(evaluate, MN_MSG_ACTIVE_MESSAGE_PARA_LEN - 6, activeMessageInfo->url.url,
                             (VOS_UINT16)activeMessageInfo->url.len);
        TAF_MEM_CHK_RTN_VAL(memResult, MN_MSG_ACTIVE_MESSAGE_PARA_LEN - 6, (VOS_UINT16)activeMessageInfo->url.len);
    }

    /* 2.3 写激活短信信息到NVIM */
    ret = TAF_ACORE_NV_WRITE(modemId, NV_ITEM_SMS_ACTIVE_MESSAGE_PARA, activeMessagePara, MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (ret != NV_OK) {
        ret = MN_ERR_CLASS_SMS_NVIM;
    } else {
        ret = MN_ERR_NO_ERROR;
    }

    PS_MEM_FREE(WUEPS_PID_AT, activeMessagePara);
    activeMessagePara = VOS_NULL_PTR;
    return ret;
}


TAF_UINT32 AT_QryRstriggerPara(TAF_UINT8 indexNum)
{
    TAF_UINT32           ret;
    TAF_UINT16           length;
    MN_MSG_ActiveMessage activeMessageInfo;
    ModemIdUint16        modemId;
    VOS_UINT32           rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_SetRstriggerPara: Get modem id fail.");
        return AT_ERROR;
    }

    /* 获取NVIM中记录的激活短信参数 */
    ret = AT_ReadActiveMessage(modemId, &activeMessageInfo);
    if (ret != MN_ERR_NO_ERROR) {
        AT_LOG1("AT_QryRstriggerPara: fail to get NVIM information, cause is %d", ret);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: 255,\"\"", g_parseContext[indexNum].cmdElement->cmdName);
    } else {
        if (activeMessageInfo.activeStatus != MN_MSG_ACTIVE_MESSAGE_STATUS_ACTIVE) {
            activeMessageInfo.activeStatus = MN_MSG_ACTIVE_MESSAGE_STATUS_DEACTIVE;
        }

        /* 拼接响应字符串: 命令字，激活状态和URL信息 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"", activeMessageInfo.activeStatus);

        length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                 activeMessageInfo.msgCoding, (g_atSndCodeAddress + length),
                                                 activeMessageInfo.url.url,
                                                 (TAF_UINT16)activeMessageInfo.url.len);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
    }

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}
#endif


TAF_UINT32 At_QryClipPara(TAF_UINT8 indexNum)
{
    TAF_SS_InterrogatessReq para;

    /* 初始化 */
    (VOS_VOID)memset_s(&para, sizeof(para), 0x00, sizeof(para));

    /* 设置SsCode */
    para.ssCode = TAF_CLIP_SS_CODE;

    if (TAF_InterrogateSSReq(g_atClientTab[indexNum].clientId, 0, &para) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLIP_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}

TAF_UINT32 At_QryClirPara(TAF_UINT8 indexNum)
{
    TAF_SS_InterrogatessReq para;

    /* 初始化 */
    (VOS_VOID)memset_s(&para, sizeof(para), 0x00, sizeof(para));

    /* 设置SsCode */
    para.ssCode = TAF_CLIR_SS_CODE;

    if (TAF_InterrogateSSReq(g_atClientTab[indexNum].clientId, 0, &para) == TAF_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLIR_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryColpPara(TAF_UINT8 indexNum)
{
    TAF_SS_InterrogatessReq para;

    /* 初始化 */
    (VOS_VOID)memset_s(&para, sizeof(para), 0x00, sizeof(para));

    /* 设置SsCode */
    para.ssCode = TAF_COLP_SS_CODE;

    para.opBsService = 0;

    if (TAF_InterrogateSSReq(g_atClientTab[indexNum].clientId, 0, &para) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_COLP_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}

TAF_UINT32 At_QryS0Para(TAF_UINT8 indexNum)
{
    AT_ModemCcCtx *ccCtx = VOS_NULL_PTR;

    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%d", ccCtx->s0TimeInfo.s0TimerLen);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_QryS3Para(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%03d", g_atS3);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_QryS4Para(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%03d", g_atS4);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_QryS5Para(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%03d", g_atS5);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2009-05-27
 *    Modification: Created function
 */
TAF_UINT32 At_QryS6Para(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%03d", g_atS6);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2009-05-27
 *    Modification: Created function
 */
TAF_UINT32 At_QryS7Para(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%03d", g_atS7);
    return AT_OK;
}

TAF_UINT32 At_QryCusdPara(TAF_UINT8 indexNum)
{
    AT_MTA_UnsolicitedRptQryReq atCmd;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_UnsolicitedRptQryReq));
    atCmd.reqType = AT_MTA_QRY_CUSD_RPT_TYPE;

    /* 给MTA发送+cusd查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_UnsolicitedRptQryReq), I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_READ;

    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 At_QryCcwaPara(TAF_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ssCtx->ccwaType);
    return AT_OK;
}
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 *  1.Date: 2009-08-25
 *    Modification:
 */
TAF_UINT32 At_QryCpinPara(TAF_UINT8 indexNum)
{
    TAF_PH_PinData pinData;

    (VOS_VOID)memset_s(&pinData, sizeof(pinData), 0x00, sizeof(pinData));

    pinData.cmdType = TAF_PIN_QUERY;

    pinData.pinType = TAF_SIM_NON;

    if (Taf_PhonePinHandle(g_atClientTab[indexNum].clientId, 0, &pinData) == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPIN_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

/*
 * Description: ^ICCID查询
 * History:
 *  1.Date: 2009-08-27
 *    Modification: Created function
 */
TAF_UINT32 At_QryIccidPara(TAF_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (Taf_ParaQuery(g_atClientTab[indexNum].clientId, 0, TAF_PH_ICC_ID, TAF_NULL_PTR) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ICCID_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryCardTypePara(TAF_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_CardTypeQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("SI_PIH_CardTypeQuery: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CARDTYPE_QUERY;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryCardTypeExPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_CardTypeExQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("SI_PIH_CardTypeQuery: At_QryCardTypeExPara fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CARDTYPEEX_QUERY;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryPNNPara(TAF_UINT8 indexNum)
{

    return AT_OK;
}


TAF_UINT32 At_QryCPNNPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, DRV_AGENT_CPNN_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_WUEPS_PID_DRV_AGENT);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPNN_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryOPLPara(TAF_UINT8 indexNum)
{

    return AT_OK;
}

TAF_UINT32 At_QryCardlockPara(TAF_UINT8 indexNum)
{
    TAF_ME_PersonalisationData mePersonalisationData;

    (VOS_VOID)memset_s(&mePersonalisationData, sizeof(mePersonalisationData), 0x00, sizeof(TAF_ME_PersonalisationData));

    /* 安全命令类型为查询 */
    mePersonalisationData.cmdType = TAF_ME_PERSONALISATION_QUERY;
    /* 锁卡操作为锁网络 */
    mePersonalisationData.mePersonalType = TAF_OPERATOR_PERSONALISATION;
    /* 执行命令操作 */
    if (Taf_MePersonalisationHandle(g_atClientTab[indexNum].clientId, 0, &mePersonalisationData) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CARD_LOCK_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryCpin2Para(TAF_UINT8 indexNum)
{
    TAF_PH_PinData pinData;

    (VOS_VOID)memset_s(&pinData, sizeof(pinData), 0x00, sizeof(pinData));

    pinData.cmdType = TAF_PIN2_QUERY;

    pinData.pinType = TAF_SIM_NON;

    if (Taf_PhonePinHandle(g_atClientTab[indexNum].clientId, 0, &pinData) == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPIN2_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

TAF_UINT32 At_QryCpbsPara(TAF_UINT8 indexNum)
{
    g_pbPrintTag = VOS_TRUE;

    if (SI_PB_Query(g_atClientTab[indexNum].clientId, 0) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPBS_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}

TAF_UINT32 At_QryCfunPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryPhoneModeReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CFUN_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QryCpamPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryAccessModeReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPAM_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif

TAF_UINT32 At_QryStsfPara(TAF_UINT8 indexNum)
{
    USIMM_StkCfg  profileContent = {0};
    TAF_UINT32    result;
    TAF_UINT16    nVLen  = sizeof(USIMM_StkCfg);
    TAF_UINT16    length = 0;
    ModemIdUint16 modemId;

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_TERMINALPROFILE_SET, &profileContent, nVLen);

    if (result != NV_OK) {
        return AT_ERROR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "^STSF:%d,", profileContent.funcEnable);

    if (AT_GetUsimInfoCtxFromModemId(modemId)->cardType == TAF_MMA_USIMM_CARD_USIM) {
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                      profileContent.usimProfile.profile,
                                                      TAF_MIN(profileContent.usimProfile.profileLen,
                                                              sizeof(profileContent.usimProfile.profile)));
    } else {
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                      profileContent.simProfile.profile,
                                                      TAF_MIN(profileContent.simProfile.profileLen,
                                                              sizeof(profileContent.simProfile.profile)));
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", profileContent.timer);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_QryStgiPara(TAF_UINT8 indexNum)
{
    if (g_stkFunctionFlag == TAF_FALSE) {
        return AT_ERROR;
    }

    if (SI_STK_QuerySTKCommand(g_atClientTab[indexNum].clientId, USAT_CMDQRYSTGI) == AT_SUCCESS) {
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


TAF_UINT32 At_QryImsichgPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (SI_STKIsDualImsiSupport() == VOS_FALSE) {
        return AT_CMD_NOT_SUPPORT;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, DRV_AGENT_IMSICHG_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_WUEPS_PID_DRV_AGENT);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSICHG_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryCgclassPara(TAF_UINT8 indexNum)
{
    VOS_UINT16 length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"A\"");

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

TAF_UINT32 At_QryCopsPara(TAF_UINT8 indexNum)
{
    /* 放开CL模式下AT+COPS查询功能 */

    if (TAF_MMA_QryCopsInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_COPS_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryHplmnPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryApHplmnInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EHPLMN_LIST_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_QryDplmnListPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryDplmnListReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DPLMNLIST_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryBorderInfoPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryBorderInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BORDERINFO_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
#if (FEATURE_CSG == FEATURE_ON)

VOS_UINT32 At_QryCampCsgIdInfo(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryCampCsgIdInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSG_ID_INFO_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

#endif


VOS_UINT32 At_QryCgcattPara(VOS_UINT8 indexNum)
{
    /* AT给MMA模块发消息，要求MMA返回CS和PS的注册状态 */
    if (TAF_MMA_AttachStatusQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                   TAF_MMA_SERVICE_DOMAIN_CS_PS) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MMA_GET_CURRENT_ATTACH_STATUS;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}

VOS_UINT32 At_QryCgattPara(VOS_UINT8 indexNum)
{
    /* AT给MMA模块发消息，要求MMA返回CS和PS的注册状态 */
    if (TAF_MMA_AttachStatusQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                   TAF_MMA_SERVICE_DOMAIN_PS) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MMA_GET_CURRENT_ATTACH_STATUS;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}


TAF_UINT32 AT_QryCgdnsPara(TAF_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetPdpDnsInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGDNS_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 AT_QryCgautoPara(TAF_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetAnsModeInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGAUTO_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 AT_QryCgtftPara(TAF_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetTftInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGTFT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 AT_QryCgactPara(TAF_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetPdpContextState(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGACT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCgdcontPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetPrimPdpContextInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGDCONT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCgeqreqPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetUmtsQosInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGEQREQ_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryCgeqminPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetUmtsQosMinInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGEQMIN_READ;

    return AT_WAIT_ASYNC_RETURN;
}

VOS_UINT32 AT_QryCgdscontPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetSecPdpContextInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGDSCONT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryCrcPara(TAF_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ssCtx->crcType);
    return AT_OK;
}


TAF_UINT32 At_QryCbstPara(TAF_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            ssCtx->cbstDataCfg.speed, ssCtx->cbstDataCfg.name, ssCtx->cbstDataCfg.connElem);

    return AT_OK;
}

TAF_UINT32 At_QryCmodPara(TAF_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ssCtx->cModType);
    return AT_OK;
}


TAF_UINT32 At_QryCstaPara(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, g_atCstaNumType);

    return AT_OK;
}


TAF_UINT32 At_QryCcugPara(TAF_UINT8 indexNum)
{
    TAF_UINT16     length = 0;
    TAF_UINT16     indexTemp;
    TAF_UINT16     info;
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    indexTemp = (TAF_UINT16)ssCtx->ccugCfg.index;

    info = (TAF_UINT16)((ssCtx->ccugCfg.suppressPrefer << 1) | ssCtx->ccugCfg.suppressOa);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        ssCtx->ccugCfg.enable);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", indexTemp);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", info);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


TAF_UINT32 At_QryCssnPara(TAF_UINT8 indexNum)
{
    AT_MTA_UnsolicitedRptQryReq atCmd;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_UnsolicitedRptQryReq));
    atCmd.reqType = AT_MTA_QRY_CSSN_RPT_TYPE;

    /* 给MTA发送^cssn查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_UnsolicitedRptQryReq), I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryCnmiPara(TAF_UINT8 indexNum)
{
    TAF_UINT16      length = 0;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        smsCtx->cnmiType.cnmiModeType);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", smsCtx->cnmiType.cnmiMtType);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", smsCtx->cnmiType.cnmiBmType);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", smsCtx->cnmiType.cnmiDsType);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", smsCtx->cnmiType.cnmiBfrType);

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}


TAF_UINT32 At_QryCmgfPara(TAF_UINT8 indexNum)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "%s: %d",
        g_parseContext[indexNum].cmdElement->cmdName, smsCtx->cmgfMsgFormat);

    return AT_OK;
}


VOS_UINT32 At_QryCscaPara(VOS_UINT8 indexNum)
{
    VOS_UINT32           ret;
    MN_MSG_ReadCommParam readParam;

    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;
    smsCtx                 = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /*
     * 设置要查询的短信中心号码存储位置 :
     * AT模块获取短信中心号码等参数默认从SIM卡的EFSMSP文件的第一条记录中获取
     */

    readParam.index = (VOS_UINT32)smsCtx->cscaCsmpInfo.defaultSmspIndex;

    readParam.memStore = MN_MSG_MEM_STORE_SIM;
    (VOS_VOID)memset_s(readParam.reserve1, sizeof(readParam.reserve1), 0x00, sizeof(readParam.reserve1));

    /* 发消息到C核获取短信中心号码 */
    g_atClientTab[indexNum].opId = At_GetOpId();
    ret = MN_MSG_ReadSrvParam(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &readParam);
    if (ret != MN_ERR_NO_ERROR) {
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSCA_READ;
    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryCsmsPara(TAF_UINT8 indexNum)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "%s: %d,",
        g_parseContext[indexNum].cmdElement->cmdName, smsCtx->csmsMsgVersion);
    At_PrintCsmsInfo(indexNum);
    return AT_OK;
}


TAF_UINT32 At_QryCsmpPara(TAF_UINT8 indexNum)
{
    TAF_UINT16              length = 0;
    AT_ModemSmsCtx         *smsCtx = VOS_NULL_PTR;
    MN_MSG_ValidPeriodUint8 validPeriod; /* TP Validity Period Format */

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (smsCtx->cscaCsmpInfo.foUsed == TAF_TRUE) {
        /* <fo> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName,
            smsCtx->cscaCsmpInfo.fo);

        AT_GET_MSG_TP_VPF(validPeriod, smsCtx->cscaCsmpInfo.fo);

        /* <vp> */
        if (validPeriod == smsCtx->cscaCsmpInfo.vp.validPeriod) {
            length += At_MsgPrintVp(&(smsCtx->cscaCsmpInfo.vp), (g_atSndCodeAddress + length));
        }

    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ,", g_parseContext[indexNum].cmdElement->cmdName);
    }

    /* <pid>  <dcs> */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d,%d", smsCtx->cscaCsmpInfo.parmInUsim.pid,
        smsCtx->cscaCsmpInfo.parmInUsim.dcs);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

TAF_UINT32 At_QryCpmsPara(TAF_UINT8 indexNum)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (At_GetSmsStorage(indexNum, smsCtx->cpmsInfo.memReadorDelete, smsCtx->cpmsInfo.memSendorWrite,
                         smsCtx->cpmsInfo.rcvPath.smMemStore) != AT_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPMS_READ;
    return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
}


TAF_UINT32 At_QryCgsmsPara(TAF_UINT8 indexNum)
{
    TAF_UINT32 ret;

    g_atClientTab[indexNum].opId = At_GetOpId();

    ret = MN_MSG_GetSendDomain(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (ret == MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGSMS_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryCmgdPara(TAF_UINT8 indexNum)
{
    MN_MSG_ListParm listPara;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    (VOS_VOID)memset_s(&listPara, sizeof(MN_MSG_ListParm), 0x00, sizeof(MN_MSG_ListParm));

    listPara.changeFlag = VOS_FALSE;
    listPara.memStore   = smsCtx->cpmsInfo.memReadorDelete;
    listPara.status     = MN_MSG_STATUS_NONE;

    g_atClientTab[indexNum].opId = At_GetOpId();
    if (MN_MSG_Delete_Test(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &listPara) ==
        MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMGD_TEST;
        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryCmmsPara(TAF_UINT8 indexNum)
{
    TAF_UINT32 ret;

    g_atClientTab[indexNum].opId = At_GetOpId();
    ret                          = MN_MSG_GetLinkCtrl(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (ret == MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMMS_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))


VOS_UINT32 At_QryCscbPara(VOS_UINT8 indexNum)
{
    TAF_UINT32 ret;

    g_atClientTab[indexNum].opId = At_GetOpId();

    ret = MN_MSG_GetAllCbMids(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (ret == MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSCB_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


TAF_UINT32 At_QryCgregPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryRegStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, TAF_MMA_QRY_REG_STATUS_TYPE_PS) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGREG_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 At_QryCeregPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryRegStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, TAF_MMA_QRY_REG_STATUS_TYPE_EPS) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CEREG_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 At_QryC5gregPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryRegStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, TAF_MMA_QRY_REG_STATUS_TYPE_5GC) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_C5GREG_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_QryLendcPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_LENDC_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryLendcPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LENDC_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryNrFreqLockPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NR_FREQLOCK_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNrFreqLockPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRFREQLOCK_QRY;

    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 AT_QryC5gqosPara(VOS_UINT8 indexNum)
{
    if (TAF_PS_Get5gQosInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_C5GQOS_READ;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryC5gPNssaiPara(VOS_UINT8 indexNum)
{
    AT_MTA_5GNssaiQryReq st5gNssaiQryReq;
    VOS_UINT32           rslt;

    /* 初始化 */
    (VOS_VOID)memset_s(&st5gNssaiQryReq, sizeof(st5gNssaiQryReq), 0x00, sizeof(AT_MTA_5GNssaiQryReq));
    st5gNssaiQryReq.opPlmn        = VOS_FALSE;
    st5gNssaiQryReq.sNssaiQryType = AT_MTA_NSSAI_QRY_PREFER_NSSAI;

    rslt = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                  ID_AT_MTA_5G_NSSAI_QRY_REQ, (VOS_UINT8 *)&st5gNssaiQryReq,
                                  (VOS_SIZE_T)sizeof(st5gNssaiQryReq), I0_UEPS_PID_MTA);

    if (rslt != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryC5gPNssaiPara: AT_FillAndSndAppReqMsg fail.");

        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_C5GPNSSAI_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryC5gNssaiPara(VOS_UINT8 indexNum)
{
    AT_MTA_5GNssaiQryReq st5gNssaiQryReq;
    VOS_UINT32           rslt;

    /* 初始化 */
    (VOS_VOID)memset_s(&st5gNssaiQryReq, sizeof(st5gNssaiQryReq), 0x00, sizeof(AT_MTA_5GNssaiQryReq));
    st5gNssaiQryReq.opPlmn        = VOS_FALSE;
    st5gNssaiQryReq.sNssaiQryType = AT_MTA_NSSAI_QRY_DEF_CFG_NSSAI_ONLY;

    rslt = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                  ID_AT_MTA_5G_NSSAI_QRY_REQ, (VOS_UINT8 *)&st5gNssaiQryReq,
                                  (VOS_SIZE_T)sizeof(st5gNssaiQryReq), I0_UEPS_PID_MTA);

    if (rslt != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryC5gNssaiPara: AT_FillAndSndAppReqMsg fail.");

        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_C5GNSSAI_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryLadnInfoPara(VOS_UINT8 atIndex)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[atIndex].clientId, g_atClientTab[atIndex].opId,
                                    ID_AT_MTA_QRY_LADN_INFO_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryLadnInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
    g_atClientTab[atIndex].cmdCurrentOpt = AT_CMD_CLADN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#endif


VOS_UINT32 AT_QryRrcStatPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryRrcStatReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RRCSTAT_QRY;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}


VOS_UINT32 AT_QryTxPowerPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_TXPOWER_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryTxPowerPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型，返回命令处理挂起状态 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TXPOWER_QRY;
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_QryNtxPowerPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NTXPOWER_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNtxPowerPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型，返回命令处理挂起状态 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NTXPOWER_QRY;
    return AT_WAIT_ASYNC_RETURN;
}
#endif

VOS_UINT32 AT_QryHfreqInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_HFREQINFO_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryHfreqinfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型，返回命令处理挂起状态 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HFREQINFO_QRY;
    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 At_QryCregPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryRegStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, TAF_MMA_QRY_REG_STATUS_TYPE_CS) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CREG_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

TAF_UINT32 At_QryCsdhPara(TAF_UINT8 indexNum)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, smsCtx->csdhType);

    return AT_OK;
}

TAF_UINT32 At_QryCscsPara(TAF_UINT8 indexNum)
{
    if (g_atCscsType == AT_CSCS_IRA_CODE) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: \"IRA\"", g_parseContext[indexNum].cmdElement->cmdName);
    } else if (g_atCscsType == AT_CSCS_UCS2_CODE) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: \"UCS2\"", g_parseContext[indexNum].cmdElement->cmdName);
    } else {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: \"GSM\"", g_parseContext[indexNum].cmdElement->cmdName);
    }
    return AT_OK;
}


TAF_UINT32 At_QryCmeePara(TAF_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, g_atCmeeType);
    return AT_OK;
}


TAF_UINT32 At_QryFPlmnPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryFPlmnInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CFPLMN_READ; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                /* 等待异步事件返回 */
    } else {
        return AT_ERROR; /* 错误返回 */
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 At_QryMaxFreelockSizePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取 MFREELOCKSIZE信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_MFREELOCKSIZE_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryMaxFreelockSizePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MFREELOCKSIZE_READ;
    return AT_WAIT_ASYNC_RETURN;
}
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 At_QryCpuLoadPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取 CPULOAD信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_CPULOAD_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryCpuLoadPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPULOAD_READ;
    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 At_QryLcStartTimePara(VOS_UINT8 indexNum)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    if (*systemAppConfig != SYSTEM_APP_WEBUI) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, g_lcStartTime);
    return AT_OK;
}


TAF_UINT32 At_QryDialModePara(TAF_UINT8 indexNum)
{
    VOS_UINT8  dialmode;
    VOS_UINT8  cdcSpec;
    VOS_UINT32 rst;

    /* 调用底软接口，根据当前的设备形态来获取当前支持的拨号方式 */
    /* ucRst:       VOS_OK/VOS_ERR */
    /* ucDialmode:  0 - 使用Modem拨号; 1 - 使用NDIS拨号; 2 - Modem和NDIS共存 */
    /*
     * ucCdcSpec:   0 - Modem/NDIS都符合CDC规范; 1 - Modem符合CDC规范;
     * 2 - NDIS符合CDC规范;         3 - Modem/NDIS都符合CDC规范
     */

    dialmode = 0;
    cdcSpec  = 0;

    rst = (VOS_UINT32)DRV_GET_DIAG_MODE_VALUE(&dialmode, &cdcSpec);

    if (rst != VOS_OK) {
        AT_ERR_LOG("At_QryDialModePara:ERROR:DRV_GET_DIAG_MODE_VALUE Return Err!");
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName, dialmode, cdcSpec);
    return AT_OK;
}


TAF_UINT32 At_QryCurcPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 result;

    AT_PR_LOGI("Rcv Msg");

    /* AT 给MTA 发送CURC查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_CURC_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryCurcPara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURC_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryTimePara(TAF_UINT8 indexNum)
{
    AT_MTA_UnsolicitedRptQryReq atCmd;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_UnsolicitedRptQryReq));
    atCmd.reqType = AT_MTA_QRY_TIME_RPT_TYPE;

    /* 给MTA发送^time查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_UnsolicitedRptQryReq), I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_QryCtzrPara(TAF_UINT8 indexNum)
{
    AT_MTA_UnsolicitedRptQryReq atCmd;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_UnsolicitedRptQryReq));
    atCmd.reqType = AT_MTA_QRY_CTZR_RPT_TYPE;

    /* 给MTA发送^ctzr查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_UnsolicitedRptQryReq), I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryTimeQryPara(VOS_UINT8 indexNum)
{
    VOS_UINT16      length = 0;
    VOS_UINT32      nwSecond;
    AT_ModemNetCtx *netCtx   = VOS_NULL_PTR;
    NAS_MM_InfoInd *timeInfo = VOS_NULL_PTR;

    netCtx   = AT_GetModemNetCtxAddrFromModemId(MODEM_ID_0);
    timeInfo = &(netCtx->timeInfo);
    nwSecond = netCtx->nwSecond;

    /* 如果当前已经拿到C核的MM INFO直接返回结果 */
    if ((timeInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        length = AT_PrintNwTimeInfo(timeInfo, (VOS_CHAR *)g_parseContext[indexNum].cmdElement->cmdName, nwSecond);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    }

    /* 如果没有拿到过MM INFO的时间信息则需要从C核拿PLMN */
    if (TAF_MMA_QryPlmnReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TIMEQRY_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT16 AT_PrintNwTimeInfo(NAS_MM_InfoInd *mmInfo, CONST_T VOS_CHAR *pcCmd, VOS_UINT32 nwSecond)
{
    VOS_INT8       timeZone;
    VOS_UINT8      dst;
    VOS_UINT16     length = 0;
    NAS_MM_InfoInd localAtTimeInfo;

    (VOS_VOID)memset_s(&localAtTimeInfo, sizeof(NAS_MM_InfoInd), 0x00, sizeof(NAS_MM_InfoInd));

    /* 时间显示格式: ^TIME: "yy/mm/dd,hh:mm:ss(+/-)tz,dst" */
    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        AT_GetLiveTime(mmInfo, &localAtTimeInfo, nwSecond);

        /* YY/MM/DD,HH:MM:SS */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d%d/%d%d/%d%d,%d%d:%d%d:%d%d", (VOS_CHAR *)pcCmd,
            localAtTimeInfo.universalTimeandLocalTimeZone.year / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.year % 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.month / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.month % 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.day / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.day % 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.hour / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.hour % 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.minute / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.minute % 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.second / 10,
            localAtTimeInfo.universalTimeandLocalTimeZone.second % 10);
        /* 获得时区 */
        if ((localAtTimeInfo.ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ) {
            timeZone = localAtTimeInfo.localTimeZone;
        } else {
            timeZone = localAtTimeInfo.universalTimeandLocalTimeZone.timeZone;
        }

        if (timeZone != AT_INVALID_TZ_VALUE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%02d", (timeZone < 0) ? "-" : "+",
                (timeZone < 0) ? (-timeZone) : timeZone);
        }

        /* 显示夏时制或冬时制信息 */
        if (((localAtTimeInfo.ieFlg & NAS_MM_INFO_IE_DST) == NAS_MM_INFO_IE_DST) && (localAtTimeInfo.dst > 0)) {
            dst = localAtTimeInfo.dst;
        } else {
            dst = 0;
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d%d", dst / 10, dst % 10);
    }
    return length;
}


VOS_UINT32 At_QryQuickStart(TAF_UINT8 indexNum)
{
    AT_PR_LOGI("Rcv Msg");

    if (TAF_MMA_QryQuickStartReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作模式 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CQST_READ;
        /* 等待异步事件返回 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        /* 错误返回 */
        return AT_ERROR;
    }
}

VOS_UINT32 At_QryAutoAttach(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QryAutoAttachInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CAATT_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QrySysCfgExPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QrySyscfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SYSCFGEX_READ; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                  /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QrySysCfgPara(TAF_UINT8 indexNum)
{
    if (TAF_MMA_QrySyscfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SYSCFG_READ; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}
#endif

#if (FEATURE_LTE == FEATURE_ON)

TAF_UINT32 At_QryCemode(TAF_UINT8 indexNum)
{
    /* 与MMA进行交互不成功返回失败，交互成功返回命令处理挂起状态 */

    if (TAF_MMA_QryCemodeReq(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CEMODE_READ; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                /* 等待异步事件返回 */
    } else {
        PS_PRINTF_WARNING("<At_QryCemode> TAF_PS_GetCemodeInfo fail\n");
        return AT_ERROR;
    }
}


TAF_UINT32 At_QryLtecsInfo(TAF_UINT8 indexNum)
{
    if (TAF_PS_GetLteCsInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTECS_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif

VOS_UINT32 At_QryClvlPara(VOS_UINT8 indexNum)
{
    if (APP_VC_GetVoiceVolume(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLVL_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryVMSETPara(VOS_UINT8 indexNum)
{
    /* 不是查询命令返回参数错误 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 获取当前语音设备模式：0 手持；1 手持免提；2 车载免提；3 耳机；4 蓝牙；5 PC语音模式 */
    if (APP_VC_GetVoiceMode(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VMSET_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QryRRCVersion(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息ID_AT_MTA_WRR_RRC_VERSION_QRY_REQ给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_WRR_RRC_VERSION_QRY_REQ, VOS_NULL_PTR,
                                 0, I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RRC_VERSION_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


TAF_UINT32 At_QryCSNR(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 ID_AT_MTA_WRR_ANQUERY_QRY_REQ 给 MTA 处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_CSNR_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSNR_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QryFreqLock(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息ID_AT_MTA_WRR_FREQLOCK_QRY_REQ给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_WRR_FREQLOCK_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FREQLOCK_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif
#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)

TAF_UINT32 At_QryM2MFreqLock(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息ID_AT_MTA_QRY_M2M_FREQLOCK_REQ给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_M2M_FREQLOCK_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_M2M_FREQLOCK_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


TAF_UINT32 At_QryU2DiagPara(TAF_UINT8 indexNum)
{
    AT_USB_EnumStatus usbEnumStatus;
    NV_PID_EnableType pidEnableType;

    pidEnableType.pidEnabled = VOS_FALSE;

    (VOS_VOID)memset_s(&usbEnumStatus, sizeof(usbEnumStatus), 0x00, sizeof(usbEnumStatus));

    /* 读取PID使能NV项 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_PID_ENABLE_TYPE, &pidEnableType, sizeof(NV_PID_EnableType)) != NV_OK) {
        return AT_ERROR;
    }

    /* NV读取成功，检查PID是否使能，若使能，返回ERROR */
    if (pidEnableType.pidEnabled != VOS_FALSE) {
        return AT_ERROR;
    }

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_USB_ENUM_STATUS, &usbEnumStatus, sizeof(AT_USB_EnumStatus)) != NV_OK) {
        return AT_ERROR;
    }

    if (usbEnumStatus.status == 0) {
        /* 若NV_ITEM_USB_ENUM_STATUS未设置，则调用底软的API来获取默认的USB设备形态 */
        usbEnumStatus.value = DRV_GET_U2DIAG_DEFVALUE();
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, usbEnumStatus.value);
    return AT_OK;
}


VOS_UINT32 At_QryPort(VOS_UINT8 indexNum)
{
    VOS_UINT32                i;
    VOS_UINT32                j;
    VOS_UINT16                length;
    VOS_UINT8                 count;
    VOS_UINT32                result;
    DRV_DYNAMIC_PID_TYPE_STRU dynamicPidType;
    NV_PID_EnableType         pidEnableType;

    length = 0;
    count  = 0;

    pidEnableType.pidEnabled = VOS_FALSE;

    (VOS_VOID)memset_s(&dynamicPidType, sizeof(dynamicPidType), 0x00, sizeof(DRV_DYNAMIC_PID_TYPE_STRU));

    /* 读取PID使能NV项 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_PID_ENABLE_TYPE, &pidEnableType, sizeof(NV_PID_EnableType)) != NV_OK) {
        AT_ERR_LOG("At_QryPort:Read NV fail!");
        return AT_ERROR;
    }

    /* NV读取成功，检查PID是否使能，若不使能，返回ERROR */
    if (pidEnableType.pidEnabled != VOS_TRUE) {
        AT_WARN_LOG("At_QryPort:The PID is not enabled!");
        return AT_ERROR;
    }

    /* 查询当前端口形态 */
    result = DRV_SET_PORT_QUIRY(&dynamicPidType);
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("At_QryPort:Qry the pot type fail!");
        return AT_ERROR;
    }

    /* ^SETPORT: */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    /* OLD PORT */
    for (i = 0; i < AT_SETPORT_PARA_MAX_LEN; i++) {
        for (j = 0; j < AT_SETPORT_DEV_MAP_LEN; j++) {
            if (dynamicPidType.aucFirstPortStyle[i] == g_setPortParaMap[j].drvPara) {
                /* 大于1个参数的显示格式: */
                if (count > 0) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                }

                /* 参数 */
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_setPortParaMap[j].atSetPara);

                count++;
                break;
            }
        }
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ";");

    count = 0;

    /* CUR PORT */
    for (i = 0; i < AT_SETPORT_PARA_MAX_LEN; i++) {
        for (j = 0; j < AT_SETPORT_DEV_LEN; j++) {
            if (dynamicPidType.aucRewindPortStyle[i] == g_setPortParaMap[j].drvPara) {
                /* 大于1个参数的显示格式: */
                if (count > 0) {
                    /* 回车换行 */
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                }

                /* 参数 */
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_setPortParaMap[j].atSetPara);

                count++;

                break;
            }
        }
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


TAF_UINT32 At_QryPcscInfo(TAF_UINT8 indexNum)
{
    VOS_UINT32 portState;

    /* PCSC 口的开关状态, 0 打开; 1 关闭 */
    if (AT_ExistSpecificPort(AT_DEV_PCSC) == VOS_TRUE) {
        portState = VOS_TRUE;
    } else {
        portState = VOS_FALSE;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, portState);
    return AT_OK;
}


VOS_UINT32 At_QryCellSearch(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 ID_AT_MTA_WRR_CELLSRH_QRY_REQ 给 DRV AGENT 处理， */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_WRR_CELLSRH_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_WAS_MNTN_QRY_CELLSRH;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

/*
 * History:
 *  1.Date: 2009-07-13
 *    Modification: Created function
 */
TAF_UINT32 At_QryGetportmodePara(TAF_UINT8 indexNum)
{
    /* 根据需求直接返回ERROR */
    return AT_ERROR;
}


TAF_UINT32 At_QryCmsrPara(TAF_UINT8 indexNum)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, smsCtx->paraCmsr);
    return AT_OK;
}


TAF_UINT32 At_QryUssdModePara(TAF_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    /* 0：USSD非透传方案（即单板进行USSD字符编解码）1：USSD透传方案（即单板不进行USSD字符编解码，只是透传) */
    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            ssCtx->ussdTransMode);

    return AT_OK;
}


TAF_UINT32 At_QryAdcTempPara(TAF_UINT8 indexNum)
{
    SPY_TempProtectNv tempProtectNv;

    (VOS_VOID)memset_s(&tempProtectNv, sizeof(tempProtectNv), 0x00, sizeof(tempProtectNv));

    /* 读取热保护门限值的NV项 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_USIM_TEMP_PROTECT_NEW, (VOS_UINT8 *)&tempProtectNv,
                          sizeof(SPY_TempProtectNv)) != NV_OK) {
        AT_ERR_LOG("Spy_SetTempPara: TAF_ACORE_NV_READ en_NV_Item_USIM_TEMP_PROTECT Fail!");

        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "^ADCTEMP: %d,%d,%d,%d", tempProtectNv.isEnable,
        tempProtectNv.closeAdcThreshold, tempProtectNv.alarmAdcThreshold, tempProtectNv.resumeAdcThreshold);

    return AT_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QryCommDebugPara(TAF_UINT8 indexNum)
{
    TAF_AT_NvimCommdegbugCfg debugFlag;

    debugFlag.commDebugFlag = 0;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_COMMDEGBUG_CFG, (VOS_UINT8 *)&(debugFlag.commDebugFlag),
                          sizeof(TAF_AT_NvimCommdegbugCfg)) != NV_OK) {
        AT_ERR_LOG("At_QryOamDebugPara: TAF_ACORE_NV_READ en_NV_Item_USIM_TEMP_PROTECT Fail!");

        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "^COMM: 0x%X", debugFlag.commDebugFlag);

    return AT_OK;
}
#endif

TAF_UINT32 At_QryDwinsPara(TAF_UINT8 indexNum)
{
    WINS_Config wins;

    (VOS_VOID)memset_s(&wins, sizeof(wins), 0x00, sizeof(wins));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WINS_CONFIG, &wins, sizeof(WINS_Config)) != NV_OK) {
        return AT_ERROR;
    }

    if (wins.ucStatus == 0) {
        /* 若en_NV_Item_WINS未设置，则默认WINS设置为使能 */
        wins.ucWins = WINS_CONFIG_ENABLE;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, wins.ucWins);
    return AT_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 At_QryYjcxPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送DRV_AGENT_YJCX_QRY_REQ消息给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, DRV_AGENT_YJCX_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_YJCX_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


VOS_UINT32 At_QryCplsPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            netCtx->prefPlmnType);

    return AT_OK;
}


TAF_UINT32 At_QryCpolPara(TAF_UINT8 indexNum)
{
    VOS_UINT32      ret;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    TAF_MMA_CpolInfoQueryReq cpolInfo;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /*
     * AT向MMA请求运营商信息:
     * 因为核间消息限制，不能一次获取所有运营商信息，这里定义为一次获取37条运营商信息
     * 第一条请求消息，从索引0开始要求连续的37条运营商信息
     */
    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    (VOS_VOID)memset_s(&cpolInfo, sizeof(cpolInfo), 0x00, sizeof(cpolInfo));

    cpolInfo.prefPLMNType = netCtx->prefPlmnType;
    cpolInfo.fromIndex    = 0;

    cpolInfo.plmnNum = TAF_MMA_MAX_PLMN_NAME_LIST_NUM;

    /* 向MMA发消息请求运营商信息 */
    ret = TAF_MMA_QueryCpolReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &cpolInfo);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }


    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPOL_READ;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 AT_QryAuthdataPara(TAF_UINT8 indexNum)
{
    if (TAF_PS_GetAuthDataInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_AUTHDATA_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryIpv6CapPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 ipv6Capability;

    switch (AT_GetIpv6Capability()) {
        case AT_IPV6_CAPABILITY_IPV4_ONLY:
            ipv6Capability = AT_IPV6_CAPABILITY_IPV4_ONLY;
            break;

        case AT_IPV6_CAPABILITY_IPV6_ONLY:
            ipv6Capability = AT_IPV6_CAPABILITY_IPV6_ONLY;
            break;

        case AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP:
            ipv6Capability = AT_IPV6_CAPABILITY_IPV4_ONLY + AT_IPV6_CAPABILITY_IPV6_ONLY +
                             AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP;
            break;

        case AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP:
            ipv6Capability = AT_IPV6_CAPABILITY_IPV4_ONLY + AT_IPV6_CAPABILITY_IPV6_ONLY +
                             AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP;
            break;

        default:
            ipv6Capability = AT_IPV6_CAPABILITY_IPV4_ONLY;
            break;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ipv6Capability);
    return AT_OK;
}

/*
 * Description: ^CRPN?
 * History:
 *  1.Date: 2009-09-01
 *    Modification: Created function
 */
TAF_UINT32 At_QryCrpnPara(TAF_UINT8 indexNum)
{
    return AT_OK;
}


VOS_UINT32 AT_QryAlsPara(TAF_UINT8 indexNum)
{
    VOS_UINT32    rst;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送TAF_CS_ALS_LINE_NO_QRY消息给CCM，由CCM返回当前的线路号。 */
    rst = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_QRY_ALS_REQ, 0, modemId);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APP_QRY_ALS_REQ;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}


VOS_VOID At_CovertMsInternalRxDivParaToUserSet(VOS_UINT16 curBandSwitch, VOS_UINT32 *userDivBandsLow,
                                               VOS_UINT32 *userDivBandsHigh)
{
    *userDivBandsLow  = 0;
    *userDivBandsHigh = 0;

    /*
     * 用户设置的接收分集格式如下:
     *   0x80000            GSM850
     *   0x300              GSM900
     *   0x80               DCS1800
     *   0x200000           PCS1900
     *   0x400000           WCDMA2100
     *   0x800000           WCDMA1900
     *   0x1000000          WCDMA1800
     *   0x2000000          WCDMA_AWS
     *   0x4000000          WCDMA850
     *   0x2000000000000    WCDMA900
     *   0x4000000000000    WCDMA1700
     * 而MS支持的接收分集格式如下:
     *   2100M/ bit1  1900M/bit2  1800M/bit3  1700M/bit4  1600M/bit5
     *   1500M/bit6   900M/bit7   850M/bit8   800M/bit9   450M/bit10
     * 需要把用户设置的接收分集转换成MS支持的格式
     */
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_900) != 0) {
        *userDivBandsHigh |= TAF_PH_BAND_WCDMA_VIII_900;
    }
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_IX_1700) != 0) {
        *userDivBandsHigh |= TAF_PH_BAND_WCDMA_IX_1700;
    }
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_2100) != 0) {
        *userDivBandsLow |= TAF_PH_BAND_WCDMA_I_IMT_2100;
    }
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_1900) != 0) {
        *userDivBandsLow |= TAF_PH_BAND_WCDMA_II_PCS_1900;
    }
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_1800) != 0) {
        *userDivBandsLow |= TAF_PH_BAND_WCDMA_III_1800;
    }
    if ((curBandSwitch & AT_MS_SUPPORT_RX_DIV_850) != 0) {
        *userDivBandsLow |= TAF_PH_BAND_WCDMA_V_850;
    }

    return;
}


VOS_UINT32 At_QryCuus1Para(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送跨核消息ID_TAF_CCM_QRY_CLPR_REQ到C核 */
    rst = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_QRY_UUSINFO_REQ, 0, modemId);

    if (rst == MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APP_SET_UUSINFO_REQ;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}


VOS_UINT32 At_QryApHplmn(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryApHplmnInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HOMEPLMN_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_QryAnQuery(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 ID_AT_MTA_WRR_ANQUERY_QRY_REQ 给 MTA 处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_ANQUERY_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ANQUERY_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


AT_CmdAntennaLevelUint8 AT_CalculateAntennaLevel(VOS_INT16 rscp, VOS_INT16 ecio)
{
    VOS_UINT16              rscpTemp;
    VOS_UINT16              ecioTemp;
    AT_CmdAntennaLevelUint8 rlstAntennaLevel;

    /* RSCP and ECIO different Thresholds variables */
    VOS_INT32 alRscpThreshold[AT_ANTEN_LEV_MAX_NUM] = { 125, 108, 102, 96 };
    VOS_INT32 alEcioThreshold[AT_ANTEN_LEV_MAX_NUM] = { 30, 16, 13, 10 };

#if (FEATURE_LTE == FEATURE_ON)

    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;
    systemAppConfig            = AT_GetSystemAppConfigAddr();

    if (*systemAppConfig == SYSTEM_APP_WEBUI) {
        /* 取绝对值 */
        rscpTemp = (VOS_UINT16)-rscp;
        ecioTemp = (VOS_UINT16)-ecio;

        if ((ecioTemp <= g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_3]) &&
            (rscpTemp <= g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_3])) {
            rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_4;
        } else if (((ecioTemp <= g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_2]) &&
                    (ecioTemp > g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_3]) &&
                    (rscpTemp <= g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_2])) ||
                   ((rscpTemp <= g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_2]) &&
                    (rscpTemp > g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_3]) &&
                    (ecioTemp < g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_2]))) {
            rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_3;
        } else if (((ecioTemp <= g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_1]) &&
                    (ecioTemp > g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_2]) &&
                    (rscpTemp <= g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_1])) ||
                   ((rscpTemp <= g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_1]) &&
                    (rscpTemp > g_rscpCfg.value[AT_NVIM_RSCP_VALUE_INDEX_2]) &&
                    (ecioTemp < g_ecioCfg.value[AT_NVIM_ECIO_VALUE_INDEX_1]))) {
            rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_2;
        } else {
            rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_1;
        }
        return rlstAntennaLevel;
    }
#endif


    /* 取绝对值 */
    rscpTemp = (VOS_UINT16)-rscp;
    ecioTemp = (VOS_UINT16)-ecio;

    if ((ecioTemp <= alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_3]) &&
        (rscpTemp <= alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_3])) {
        rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_4;
    } else if (((ecioTemp <= alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_2]) &&
                (ecioTemp > alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_3]) &&
                (rscpTemp <= alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_2])) ||
               ((rscpTemp <= alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_2]) &&
                (rscpTemp > alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_3]) &&
                (ecioTemp < alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_2]))) {
        rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_3;
    } else if (((ecioTemp <= alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_1]) &&
                (ecioTemp > alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_2]) &&
                (rscpTemp <= alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_1])) ||
               ((rscpTemp <= alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_1]) &&
                (rscpTemp > alRscpThreshold[AT_ANTEN_LEV_RSCP_THRESHOLD_INDEX_2]) &&
                (ecioTemp < alEcioThreshold[AT_ANTEN_LEV_ECIO_THRESHOLD_INDEX_1]))) {
        rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_2;
    } else {
        rlstAntennaLevel = AT_CMD_ANTENNA_LEVEL_1;
    }

    return rlstAntennaLevel;
}

VOS_VOID AT_GetSmoothAntennaLevel(VOS_UINT8 indexNum, AT_CmdAntennaLevelUint8 level)
{
    VOS_UINT8       i;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 丢网时 立即更新  */
    if (level == AT_CMD_ANTENNA_LEVEL_0) {
        netCtx->calculateAntennaLevel = level;

        (VOS_VOID)memset_s(netCtx->antennaLevel, sizeof(netCtx->antennaLevel), 0x00, sizeof(netCtx->antennaLevel));
        return;
    }

    /* 丢网到有服务状态  立即更新  */
    if (netCtx->calculateAntennaLevel == AT_CMD_ANTENNA_LEVEL_0) {
        netCtx->calculateAntennaLevel = level;

        (VOS_VOID)memset_s(netCtx->antennaLevel, sizeof(netCtx->antennaLevel), 0x00, sizeof(netCtx->antennaLevel));
        netCtx->antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM - 1] = level;
        return;
    }

    /* 与上次的信号格数比较, 变化比较大(超过1格)就立即更新 */
    if (level > (netCtx->antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM - 1] + 1)) {
        netCtx->calculateAntennaLevel = level;

        (VOS_VOID)memset_s(netCtx->antennaLevel, sizeof(netCtx->antennaLevel), 0x00, sizeof(netCtx->antennaLevel));
        netCtx->antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM - 1] = level;
        return;
    } else if ((level + 1) < netCtx->antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM - 1]) {
        netCtx->calculateAntennaLevel = level;

        (VOS_VOID)memset_s(netCtx->antennaLevel, sizeof(netCtx->antennaLevel), 0x00, sizeof(netCtx->antennaLevel));
        netCtx->antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM - 1] = level;
        return;
    } else {
        /* Do nothing... */
    }

    /* 先进先出存最近3次的查询结果 */
    for (i = 0; i < AT_ANTENNA_LEVEL_MAX_NUM - 1; i++) {
        netCtx->antennaLevel[i] = netCtx->antennaLevel[i + 1];
    }
    netCtx->antennaLevel[i] = level;

    /* 格数波动则不更新，以达到平滑的效果 */
    for (i = 0; i < AT_ANTENNA_LEVEL_MAX_NUM; i++) {
        if (netCtx->calculateAntennaLevel == netCtx->antennaLevel[i]) {
            return;
        }
    }

    /* 信号格数稳定了 AT_ANTENNA_LEVEL_MAX_NUM 次时才做更新 */
    netCtx->calculateAntennaLevel = level;
}

#if (FEATURE_LTE == FEATURE_ON)

TAF_VOID AT_CalculateLTESignalValue(VOS_INT16 *psRssi, VOS_UINT8 *level, VOS_INT16 *psRsrp, VOS_INT16 *psRsrq

)
{
    VOS_UINT8 curAntennaLevel;
    VOS_INT16 rsrp;
    VOS_INT16 rsrq;

    /* 与 atSetAnlevelCnfSameProc处理相同 */
    /*
     * 上报数据转换:将 Rscp、Ecio显示为非负值，若Rscp、Ecio为-145，-32，或者rssi为99，
     * 则转换为0
     */
    if ((*psRsrp == TAF_PH_RSSIUNKNOW) || (*psRssi == TAF_PH_RSSIUNKNOW)) {
        /* 丢网返回0, 对应应用的圈外 */
        curAntennaLevel = AT_CMD_ANTENNA_LEVEL_0;
    } else {
        /* 取绝对值 */
        rsrp = (-(*psRsrp));

        /* 调用函数AT_CalculateAntennaLevel来根据D25算法计算出信号格数 */
        curAntennaLevel = AT_CalculateLTEAntennaLevel((VOS_INT16)(rsrp));
    }

    /* 信号磁滞处理 */
    *level = AT_GetSmoothLTEAntennaLevel(curAntennaLevel);

    if (*psRssi == AT_RSSI_UNKNOWN) {
        *psRssi = AT_RSSI_UNKNOWN;
    } else if (*psRssi >= AT_RSSI_HIGH) {
        *psRssi = AT_CSQ_RSSI_HIGH;
    } else if (*psRssi <= AT_RSSI_LOW) {
        *psRssi = AT_CSQ_RSSI_LOW;
    } else {
        *psRssi = (VOS_INT16)((*psRssi - AT_RSSI_LOW) / 2);
    }

    rsrp = (*psRsrp == TAF_PH_RSSIUNKNOW) ? TAF_PH_RSSIUNKNOW : (-(*psRsrp));
    rsrq = (*psRsrq == TAF_PH_RSSIUNKNOW) ? TAF_PH_RSSIUNKNOW : (-(*psRsrq));

    *psRsrp = rsrp;
    *psRsrq = rsrq;

    return;
}
#endif



TAF_UINT32 At_QryGlastErrPara(TAF_UINT8 indexNum)
{
    TAF_UINT16 length;
    TAF_UINT32 atErrType;

    atErrType = AT_GetErrType();
    /* 将错误码上报给后台 */
    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", atErrType);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if (FEATURE_LTEV == FEATURE_ON)

TAF_UINT32 At_QryVModePara(VOS_UINT8 indexNum)
{
    VOS_UINT32   rst;
    TAF_MTA_Ctrl ctrl = {0};

    /* 端口控制信息 */
    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    rst = TAF_MTA_VModeQryReq(&ctrl);
    if (rst == TAF_MTA_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VMODE_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

TAF_UINT32 AT_QrySourceId(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息ID_AT_MTA_VMODE_QRY_REQ给AT代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_VNAS_LTEV_SOURCE_ID_QRY_REQ, VOS_NULL_PTR,
                                 0, I0_PS_PID_VNAS);
    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SRCID_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif
#endif



TAF_UINT32 At_QryPlatForm(TAF_UINT8 indexNum)
{
    VOS_UINT32        ret;
    NAS_NVIM_Platform platform;

    platform.platform = 0;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SUB_PLATFORM_INFO, &platform.platform, sizeof(platform.platform));
    if (ret != NV_OK) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, 1,
            platform.platform);

    return AT_OK;
}

VOS_UINT32 At_QryDataLock(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, g_atDataLocked);

    return AT_OK;
}

VOS_UINT32 At_DelCtlAndBlankCharWithEndPadding(VOS_UINT8 *data, VOS_UINT16 *cmdLen)
{
    VOS_UINT16 origLen;

    origLen = *cmdLen;

    /* 扫描控制符 */
    if (At_ScanCtlChar(data, cmdLen) == AT_FAILURE) {
        return AT_FAILURE;
    }

    /* 扫描引号以外的空格符 */
    if (At_ScanBlankChar(data, cmdLen) == AT_FAILURE) {
        return AT_FAILURE;
    }

    (VOS_VOID)memset_s(data + (*cmdLen), origLen - (*cmdLen), 0x00, origLen - (*cmdLen));

    return AT_SUCCESS;
}

VOS_UINT32 At_QryVersion(VOS_UINT8 indexNum)
{
    AT_PR_LOGI("Rcv Msg");

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_VERSION_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VERSION_QRY;

        AT_PR_LOGI("Snd Msg");
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}



VOS_UINT32 At_QryBsn(VOS_UINT8 indexNum)
{
    /* 序列号长度为16,再加最后一位存储结束符 */
    VOS_UINT8     bsnSerialNum[AT_BSN_SERIAL_NUM_LEN];
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    ret;

    modemId = MODEM_ID_0;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    length = sizeof(bsnSerialNum);

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("At_SetSDomainPara: Get modem id fail.");
        return AT_ERROR;
    }

    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_MANUFACTURE_INFO, bsnSerialNum, length) != NV_OK) {
        AT_WARN_LOG("At_QryBsn:WARNING:NVIM Read NV_ITEM_SERIAL_NUM falied!");
        return AT_ERROR;
    } else {
        bsnSerialNum[AT_BSN_SERIAL_NUM_LEN - 1] = 0; /* 将最后一位置以结束符号'\0' */
        length           = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", bsnSerialNum);
    }
    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}

VOS_UINT32 At_QryCsVer(VOS_UINT8 indexNum)
{
    VOS_UINT32     ret;
    TAF_NVIM_CsVer csver;

    csver.csver = 0;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CSVER, &csver, sizeof(VOS_UINT16));
    if (ret != NV_OK) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, csver.csver);

    return AT_OK;
}

VOS_UINT32 At_QryQosPara(VOS_UINT8 indexNum)
{
    AT_TrafficClassCustomize aTTrafficClass;
    VOS_UINT16               readLength;
    VOS_UINT16               length;
    ModemIdUint16            modemId;
    VOS_UINT32               ret;

    modemId = MODEM_ID_0;

    aTTrafficClass.status       = NV_ITEM_DEACTIVE;
    aTTrafficClass.trafficClass = AT_QOS_TRAFFIC_CLASS_SUBSCRIBE;

    /* 命令状态类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 读取NV中的值到stATTrafficClass结构体变量 */
    readLength = sizeof(aTTrafficClass);

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("At_QryQosPara: Get modem id fail.");
        return AT_ERROR;
    }

    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_TRAFFIC_CLASS_TYPE, &aTTrafficClass, readLength) != NV_OK) {
        AT_WARN_LOG("At_QryQosPara:WARNING:NVIM Read NV_ITEM_TRAFFIC_CLASS_TYPE failed!");
        return AT_DEVICE_OTHER_ERROR;
    }

    /* NV处于非激活态,直接上报默认值:AT_QOS_TRAFFIC_CLASS_INTERACTIVE */
    if (aTTrafficClass.status == NV_ITEM_DEACTIVE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_QOS_TRAFFIC_CLASS_SUBSCRIBE);

    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            aTTrafficClass.trafficClass);
    }
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 At_QrySDomainPara(VOS_UINT8 indexNum)
{
    NAS_NVIM_MsClass msClass;
    VOS_UINT16       length;
    ModemIdUint16    modemId;
    VOS_UINT32       ret;

    modemId = MODEM_ID_0;

    (VOS_VOID)memset_s(&msClass, sizeof(msClass), 0x00, sizeof(NAS_NVIM_MsClass));
    msClass.msClass = TAF_PH_MS_CLASS_A;

    /* 命令状态类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 读取NV */
    length = sizeof(TAF_PH_MS_CLASS_TYPE);

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("At_QrySDomainPara: Get modem id fail.");
        return AT_ERROR;
    }
    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_MMA_MS_CLASS, &msClass, sizeof(NAS_NVIM_MsClass)) != NV_OK) {
        AT_WARN_LOG("At_QrySDomainPara:WARNING:NVIM Read NV_ITEM_MMA_MS_CLASS failed!");
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 上报从NV中读取的MsClass值 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, msClass.msClass);
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 At_QryGPIOPL(VOS_UINT8 indexNum)
{
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, DRV_AGENT_GPIOPL_QRY_REQ,
                               VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GPIOPL_QRY; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                               /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryMDatePara(VOS_UINT8 indexNum)
{
    VOS_UINT32                 ret;
    VOS_UINT16                 length;
    TAF_AT_NvimManufactureDate manufactrueDate;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AT_MANUFACTURE_DATE, &manufactrueDate, AT_MDATE_STRING_LENGTH);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_QryMDatePara: fail to read NVIM . ");
        return AT_ERROR;
    }

    /* 拼接响应字符串: 命令字，生产日期信息 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    manufactrueDate.mDate[AT_MDATE_STRING_LENGTH - 1] = '\0';
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", manufactrueDate.mDate);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryFacInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;
    VOS_UINT8 *facInfo = TAF_NULL_PTR;
    VOS_UINT16 length;

    /* 获取NV中已经存储的制造信息 */
    facInfo = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_FACINFO_STRING_LENGTH);
    if (facInfo == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_QryFacInfoPara: fail to alloc memory.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(facInfo, AT_FACINFO_STRING_LENGTH, 0x00, AT_FACINFO_STRING_LENGTH);

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AT_FACTORY_INFO, facInfo, AT_FACINFO_STRING_LENGTH);
    if (ret != NV_OK) {
        AT_WARN_LOG("AT_QryFacInfoPara: fail to read NV_ITEM_AT_FACTORY_INFO . ");
        PS_MEM_FREE(WUEPS_PID_AT, facInfo);
        return AT_ERROR;
    }

    /* 容错处理，强制给NV的INFO字段加结束符 */
    *(facInfo + (AT_FACINFO_STRING_LENGTH - 1)) = '\0';
    *(facInfo + AT_FACINFO_INFO1_LENGTH)        = '\0';

    /* 拼接响应字符串: 命令字，第一段制造信息 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:0,%s", g_parseContext[indexNum].cmdElement->cmdName, facInfo);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* 拼接响应字符串: 命令字，第二段制造信息 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:1,%s", g_parseContext[indexNum].cmdElement->cmdName,
        ((facInfo + AT_FACINFO_INFO1_LENGTH) + 1));

    g_atSendDataBuff.bufLen = length;

    PS_MEM_FREE(WUEPS_PID_AT, facInfo);
    return AT_OK;
}


VOS_UINT32 At_ReadDpaCatFromNV(VOS_UINT8 *dpaRate)
{
    AT_NvimUeCapa uECapa;
    AT_DpacatPara dhpaCategory[AT_DPACAT_CATEGORY_TYPE_BUTT] = {
        /* 支持速率等级3.6M  */
        { PS_TRUE, AT_HSDSCH_PHY_CATEGORY_6, PS_FALSE, 0, PS_FALSE },
        /* 支持速率等级7.2M  */
        { PS_TRUE, AT_HSDSCH_PHY_CATEGORY_8, PS_FALSE, 0, PS_FALSE },
        /* 支持速率等级1.8M  */
        { PS_TRUE, AT_HSDSCH_PHY_CATEGORY_11, PS_FALSE, 0, PS_FALSE },
        /*  支持速率等级14.4M */
        { PS_TRUE, AT_HSDSCH_PHY_CATEGORY_10, PS_FALSE, 0, PS_FALSE },
        /*  支持速率等级21M */
        { PS_TRUE, AT_HSDSCH_PHY_CATEGORY_10, PS_TRUE, AT_HSDSCH_PHY_CATEGORY_14, PS_FALSE }
    };
    VOS_UINT8 loop;

    (VOS_VOID)memset_s(&uECapa, sizeof(uECapa), 0x00, sizeof(uECapa));

    /* 输入参数空指针检查 */
    if (dpaRate == VOS_NULL_PTR) {
        AT_WARN_LOG("At_ReadDpaCatFromNV: null PTR.");
        return VOS_ERR;
    }

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WAS_RADIO_ACCESS_CAPA_NEW, &uECapa, sizeof(AT_NvimUeCapa)) != NV_OK) {
        AT_WARN_LOG("At_ReadDpaCatFromNV():en_NV_Item_WAS_RadioAccess_Capa NV Read Fail!");
        return VOS_ERR;
    }

    for (loop = 0; loop < AT_DPACAT_CATEGORY_TYPE_BUTT; loop++) {
        if ((uECapa.hsdschSupport == dhpaCategory[loop].hsdschSupport) &&
            (uECapa.hsdschPhyCategory == dhpaCategory[loop].hsdschPhyCategory) &&
            (uECapa.macEhsSupport == dhpaCategory[loop].macEhsSupport) &&
            (uECapa.hsdschPhyCategoryExt == dhpaCategory[loop].hsdschPhyCategoryExt) &&
            (uECapa.multiCellSupport == dhpaCategory[loop].multiCellSupport)) {
            *dpaRate = loop;
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))


VOS_UINT32 At_QryDpaCat(VOS_UINT8 indexNum)
{
    VOS_UINT8  dpaRate;
    VOS_UINT32 wasResult;
    VOS_UINT16 length;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 调用读NV接口函数: At_ReadDpaCatFromNV,返回操作结果 */
    wasResult = At_ReadDpaCatFromNV(&dpaRate);
    if (wasResult == VOS_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", (VOS_UINT32)dpaRate);
        g_atSendDataBuff.bufLen = length;
        return AT_OK;
    } else {
        AT_WARN_LOG("At_QryDpaCat:WARNING:WAS_MNTN_QueryDpaCat failed!");
        return AT_ERROR;
    }
}
#endif

VOS_UINT32 AT_ReadRrcVerFromNV(VOS_UINT8 *rrcVer)
{
    AT_NvimUeCapa uECapa;

    (VOS_VOID)memset_s(&uECapa, sizeof(uECapa), 0x00, sizeof(uECapa));

    /* 输入参数非空性检查 */
    if (rrcVer == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_ReadRrcVerFromNV: null PTR.");
        return VOS_ERR;
    }

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WAS_RADIO_ACCESS_CAPA_NEW, &uECapa, sizeof(AT_NvimUeCapa)) != NV_OK) {
        AT_WARN_LOG("AT_ReadRrcVerFromNV: TAF_ACORE_NV_READ NV_ITEM_WAS_RADIO_ACCESS_CAPA_NEW fail!");
        return VOS_ERR;
    }

    /* NV未使能 */
    if (uECapa.hspaStatus == VOS_FALSE) {
        *rrcVer = AT_RRC_VERSION_DPA_AND_UPA;
        return VOS_OK;
    }

    /* NV使能 */
    /* HSPA+ */
    if ((uECapa.asRelIndicator >= AT_PTL_VER_ENUM_R7) && (uECapa.edchSupport == VOS_TRUE) &&
        (uECapa.hsdschSupport == VOS_TRUE)) {
        *rrcVer = AT_RRC_VERSION_HSPA_PLUNS;
    }
    /* DPA/UPA */
    else if ((uECapa.edchSupport == VOS_TRUE) && (uECapa.hsdschSupport == VOS_TRUE)) {
        *rrcVer = AT_RRC_VERSION_DPA_AND_UPA;
    }
    /* DPA */
    else if (uECapa.hsdschSupport == VOS_TRUE) {
        *rrcVer = AT_RRC_VERSION_DPA;
    }
    /* WCDMA */
    else {
        *rrcVer = AT_RRC_VERSION_WCDMA;
    }

    return VOS_OK;
}


VOS_UINT32 At_QryCallSrvPara(VOS_UINT8 indexNum)
{
    NAS_NVIM_CustomizeService custSrv;
    VOS_UINT16                length;

    custSrv.status           = NV_ITEM_DEACTIVE;
    custSrv.customizeService = 0;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOMIZE_SERVICE, &custSrv, sizeof(custSrv)) != NV_OK) {
        AT_WARN_LOG("At_QryCallSrvPara():NV_ITEM_CUSTOMIZE_SERVICE NV Read Fail!");
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 如果NV项未激活，返回单板默认值FALSE */
    if (custSrv.status == NV_ITEM_DEACTIVE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, VOS_FALSE);
    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            custSrv.customizeService);
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryCsdfltPara(VOS_UINT8 indexNum)
{
    /* 发消息到C核获取SIMLOCK 状态信息 */
    if (Taf_ParaQuery(g_atClientTab[indexNum].clientId, 0, TAF_PH_SIMLOCK_VALUE_PARA, TAF_NULL_PTR) != TAF_SUCCESS) {
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSDFLT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_JudgeU8ArrayZero(VOS_UINT8 aucValue[], VOS_UINT32 length)
{
    VOS_UINT32 loop;

    for (loop = 0; loop < length; loop++) {
        if (aucValue[loop] != 0) {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_GetWifiNvValue(VOS_UINT16 *csdValue)
{
    TAF_AT_MultiWifiSec  wifiKey;
    TAF_AT_MultiWifiSsid wifiSsid;
    VOS_UINT32           ret;
    VOS_UINT32           loop;

    /* 不支持WIFI情况下WIFI的NV项不关注，直接返回未修改 */
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_UNCHANGE;
        return VOS_OK;
    }

    (VOS_VOID)memset_s(&wifiKey, sizeof(wifiKey), 0x00, sizeof(wifiKey));
    (VOS_VOID)memset_s(&wifiSsid, sizeof(wifiSsid), 0x00, sizeof(wifiSsid));

    /*
     * 判断en_NV_Item_WIFI_KEY是否为默认值，关注下述字段是否全0:
     * aucWifiWpapsk aucWifiWepKey1 aucWifiWepKey2 aucWifiWepKey3 aucWifiWepKey4
     */
    if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiKey,
                                   sizeof(TAF_AT_MultiWifiSec)) != NV_OK) {
        AT_WARN_LOG("AT_GetWifiNvValue:READ NV ERROR");
        return VOS_ERR;
    }

    /*
     * 判断en_NV_Item_WIFI_STATUS_SSID是否为默认值，关注下述字段是否全0:
     * aucWifiSsid
     */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, &wifiSsid, sizeof(TAF_AT_MultiWifiSsid)) !=
        VOS_OK) {
        AT_WARN_LOG("AT_GetWifiNvValue:READ NV ERROR");
        return VOS_ERR;
    }

    for (loop = 0; loop < AT_WIFI_MAX_SSID_NUM; loop++) {
        ret = AT_JudgeU8ArrayZero(wifiKey.wifiWpapsk[loop], sizeof(wifiKey.wifiWpapsk[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }

        ret = AT_JudgeU8ArrayZero(wifiKey.wifiWepKey1[loop], sizeof(wifiKey.wifiWepKey1[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }

        ret = AT_JudgeU8ArrayZero(wifiKey.wifiWepKey2[loop], sizeof(wifiKey.wifiWepKey2[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }

        ret = AT_JudgeU8ArrayZero(wifiKey.wifiWepKey3[loop], sizeof(wifiKey.wifiWepKey3[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }

        ret = AT_JudgeU8ArrayZero(wifiKey.wifiWepKey4[loop], sizeof(wifiKey.wifiWepKey4[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }

        ret = AT_JudgeU8ArrayZero(wifiSsid.wifiSsid[loop], sizeof(wifiSsid.wifiSsid[loop]));
        if (ret != VOS_OK) {
            *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
            return VOS_OK;
        }
    }

    *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_UNCHANGE;
    return VOS_OK;
}


VOS_UINT32 AT_GetCsdValue(VOS_BOOL bSimlockEnableFlg, VOS_UINT16 *csdValue)
{
    VOS_UINT32 ret;

    /* 判断SIM LOCK Status定制项的内容是否为默认值DISABLE */
    /* 获取SIM Lock Status定制项的值并检查是否为默认值 */
    if (bSimlockEnableFlg != VOS_FALSE) {
        *csdValue = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
        return VOS_OK;
    }

    /* 判断WIFI相关的NV项是否为默认值 */
    ret = AT_GetWifiNvValue(csdValue);

    return ret;
}


VOS_UINT32 AT_OutputCsdfltDefault(VOS_UINT8 indexNum, VOS_BOOL bSimlockEnableFlg)
{
    VOS_UINT16 length;
    VOS_UINT32 csdfltFlg;
    VOS_UINT16 csdfltValue;
    VOS_UINT32 rslt;

    csdfltFlg = VOS_TRUE;

    rslt = AT_GetCsdValue(bSimlockEnableFlg, &csdfltValue);

    if (rslt != VOS_OK) {
        return AT_ERROR;
    }

    /*
     * 显示定制值信息，0:表示定制项中的值为单板自定义的默认值;
     * 1:表示定制项中的值为非单板自定义的默认值
     */
    if (csdfltValue == AT_CUSTOMIZE_ITEM_DEFAULT_VAL_UNCHANGE) {
        csdfltFlg = VOS_FALSE;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, csdfltFlg);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_SimlockPlmnNumToAscii(const VOS_UINT8 *plmnRange, VOS_UINT8 plmnRangeLen, VOS_UINT8 *asciiStr)
{
    VOS_UINT8 loop;
    VOS_UINT8 len;
    VOS_UINT8 bcdCode;

    if ((plmnRange == VOS_NULL_PTR) || (asciiStr == VOS_NULL_PTR)) {
        AT_WARN_LOG("At_SimlockPlmnNumToAscii: Parameter of the function is null.");
        return AT_FAILURE;
    }

    /* 整理号码字符串，去除无效的0XFF数据 */
    while (plmnRangeLen > 1) {
        if (plmnRange[plmnRangeLen - 1] == 0xFF) {
            plmnRangeLen--;
        } else {
            break;
        }
    }

    /*
     * 判断pucPlmnRange所指向的字符串的最后一个字节的低位是否为1111，
     * 如果是，说明号码位数为奇数，否则为偶数
     */
    if ((plmnRange[plmnRangeLen - 1] & 0x0F) == 0x0F) {
        /* 号码位数为奇数 */
        len = (VOS_UINT8)((plmnRangeLen * 2) - 1);
    } else {
        /* 号码位数为偶数 */
        len = (VOS_UINT8)(plmnRangeLen * 2);
    }

    /* 解析号码 */
    for (loop = 0; loop < len; loop++) {
        /* 余2用于判断当前解码的是奇数位号码还是偶数位号码从0开始是偶数 */
        if ((loop % 2) == 0) {
            /* 如果是偶数位号码，则取高4位的值 */
            bcdCode = ((plmnRange[(loop / 2)] >> 4) & 0x0F);
        } else {
            /* 如果是奇数位号码，则取低4位的值 */
            bcdCode = (plmnRange[(loop / 2)] & 0x0F);
        }

        /* 将数字转换成Ascii码形式 */
        if (bcdCode <= AT_DEC_MAX_NUM) {
            asciiStr[loop] = (bcdCode + '0');
        } else {
            return AT_FAILURE;
        }
    }

    asciiStr[loop] = '\0'; /* 字符串末尾为0 */

    return AT_SUCCESS;
}


VOS_UINT32 At_QrySimLockPlmnInfo(VOS_UINT8 indexNum)
{
    TAF_CUSTOM_SimLockPlmnInfo simLockPlmnInfo;
    VOS_UINT32                 i;
    VOS_UINT16                 length;
    VOS_UINT8                  mncLen;
    VOS_UINT32                 plmnIdx;
    VOS_UINT32                 rsltChkPlmnBegin;
    VOS_UINT32                 rsltChkPlmnEnd;
    VOS_UINT32                 totalPlmnNum;
    VOS_BOOL                   abPlmnValidFlg[TAF_MAX_SIM_LOCK_RANGE_NUM] = {VOS_FALSE};
    VOS_UINT8                  asciiNumBegin[(TAF_PH_SIMLOCK_PLMN_STR_LEN * 2) + 1];
    VOS_UINT8                  asciiNumEnd[(TAF_PH_SIMLOCK_PLMN_STR_LEN * 2) + 1];

    (VOS_VOID)memset_s(&simLockPlmnInfo, sizeof(simLockPlmnInfo), 0x00, sizeof(simLockPlmnInfo));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOMIZE_SIM_LOCKPLMN_INFO, &simLockPlmnInfo, sizeof(simLockPlmnInfo)) !=
        NV_OK) {
        return AT_ERROR;
    }

    totalPlmnNum = 0;

    if (simLockPlmnInfo.status == NV_ITEM_DEACTIVE) {
        /* 状态非激活时，显示Plmn个数为0 */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, totalPlmnNum);

        g_atSendDataBuff.bufLen = length;
        return AT_OK;
    }

    /*  判断Plmn号段是否有效 */
    for (i = 0; i < TAF_MAX_SIM_LOCK_RANGE_NUM; i++) {
        mncLen = simLockPlmnInfo.simLockPlmnRange[i].mncNum;

        rsltChkPlmnBegin = At_SimlockPlmnNumToAscii(simLockPlmnInfo.simLockPlmnRange[i].rangeBegin,
                                                    TAF_PH_SIMLOCK_PLMN_STR_LEN, asciiNumBegin);

        rsltChkPlmnEnd = At_SimlockPlmnNumToAscii(simLockPlmnInfo.simLockPlmnRange[i].rangeBegin,
                                                  TAF_PH_SIMLOCK_PLMN_STR_LEN, asciiNumEnd);

        if ((rsltChkPlmnBegin == AT_SUCCESS) && (rsltChkPlmnEnd == AT_SUCCESS) &&
            ((mncLen <= AT_MNC_LEN_MAX_VALID_VALUE) && (mncLen >= AT_MNC_LEN_MIN_VALID_VALUE))) {
            abPlmnValidFlg[i] = VOS_TRUE;
            ++totalPlmnNum;
        }
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, totalPlmnNum);

    if (totalPlmnNum == 0) {
        g_atSendDataBuff.bufLen = length;
        return AT_OK;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    }

    plmnIdx = 0;
    for (i = 0; i < TAF_MAX_SIM_LOCK_RANGE_NUM; i++) {
        if (abPlmnValidFlg[i] == VOS_FALSE) {
            continue;
        }

        ++plmnIdx;

        At_SimlockPlmnNumToAscii(simLockPlmnInfo.simLockPlmnRange[i].rangeBegin, TAF_PH_SIMLOCK_PLMN_STR_LEN,
                                 asciiNumBegin);

        At_SimlockPlmnNumToAscii(simLockPlmnInfo.simLockPlmnRange[i].rangeEnd, TAF_PH_SIMLOCK_PLMN_STR_LEN,
                                 asciiNumEnd);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%s,%s", g_parseContext[indexNum].cmdElement->cmdName,
            plmnIdx, simLockPlmnInfo.simLockPlmnRange[i].mncNum, asciiNumBegin, asciiNumEnd);

        if (plmnIdx < totalPlmnNum) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

VOS_UINT32 At_QryMaxLockTimes(VOS_UINT8 indexNum)
{
    TAF_CUSTOM_SimLockMaxTimes simLockMaxTimes;
    VOS_UINT32                 result;
    VOS_UINT16                 length;

    simLockMaxTimes.status       = NV_ITEM_DEACTIVE;
    simLockMaxTimes.lockMaxTimes = 0;

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOMIZE_SIM_LOCK_MAX_TIMES, &simLockMaxTimes,
                               sizeof(simLockMaxTimes));
    if (result != NV_OK) {
        AT_WARN_LOG("At_QryMaxLockTimes():NV_ITEM_CUSTOMIZE_SIM_LOCK_MAX_TIMES TAF_ACORE_NV_READ Fail!");
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 如果NV项未激活，返回单板默认值 10 */
    if (simLockMaxTimes.status == NV_ITEM_DEACTIVE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            TAF_PH_CARDLOCK_DEFAULT_MAXTIME);
    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            simLockMaxTimes.lockMaxTimes);
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryAppWronReg(VOS_UINT8 indexNum)
{
    /* 发送消息给MMA，查询注册时间 */
    if (TAF_MMA_QryApPwrOnAndRegTimeReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_REGISTER_TIME_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryNdisdupPara(VOS_UINT8 indexNum)
{
    /* 其他的PDP TYPE 没有提出需求，目前只返回OK */
    return AT_OK;
}


VOS_UINT32 At_QryApDialModePara(VOS_UINT8 indexNum)
{
    AT_RNIC_DialModeReq *msg = VOS_NULL_PTR;

    /* 申请ID_RNIC_AT_DIAL_MODE_REQ消息 */
    /*lint -save -e516 */
    msg = (AT_RNIC_DialModeReq *)TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, sizeof(AT_RNIC_DialModeReq));
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_QryApDialModePara: alloc msg fail!");
        return VOS_ERR;
    }

    /* 初始化消息 */
    (VOS_VOID)memset_s((VOS_CHAR *)msg + VOS_MSG_HEAD_LENGTH,
                       sizeof(AT_RNIC_DialModeReq) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_RNIC_DialModeReq) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头 */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, ACPU_PID_RNIC, sizeof(AT_RNIC_DialModeReq) - VOS_MSG_HEAD_LENGTH);
    msg->msgId    = ID_AT_RNIC_DIAL_MODE_REQ;
    msg->clientId = g_atClientTab[indexNum].clientId;

    /* 发ID_RNIC_AT_DIAL_MODE_REQ消息给RNIC获取当前的流速 */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APDIALMODE_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("At_QryApDialModePara: Send msg failed!");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryAppdmverPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取 PDM版本号 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_APPDMVER_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryAppdmverPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APPDMVER_READ;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDislogPara(VOS_UINT8 indexNum)
{
    AT_DiagOpenFlagUint32 diagPortState;

#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT32 retDiag;
    VOS_UINT32 ret3GDiag;
    VOS_UINT32 retGps;

    ret3GDiag = AT_ExistSpecificPort(AT_DEV_DIAG);
    retDiag   = AT_ExistSpecificPort(AT_DEV_4G_DIAG);
    retGps    = AT_ExistSpecificPort(AT_DEV_4G_GPS);

    if ((ret3GDiag == VOS_TRUE) && (retDiag == VOS_TRUE) && (retGps == VOS_TRUE)) {
        diagPortState = AT_DIAG_OPEN_FLAG_OPEN;
    } else {
        diagPortState = AT_DIAG_OPEN_FLAG_CLOSE;
    }
#else
    /* 从NV中读取 DISLOG 操作的密码及DIAG口的开关状态 */

    /* DIAG 口的开关状态, 0 打开; 1 关闭 */
    if (AT_ExistSpecificPort(AT_DEV_DIAG) != VOS_TRUE) {
        diagPortState = AT_DIAG_OPEN_FLAG_CLOSE;
    } else {
        diagPortState = AT_DIAG_OPEN_FLAG_OPEN;
    }

#endif

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, diagPortState);
    return AT_OK;
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
/*
 * 功能描述: 查询装备写入的APN KEY
 */
VOS_UINT32 AT_QryVoipApnKeyPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_ApnKeyList encrptPwd;

    /* 特性是否打开 */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->voipApnKey == (VOS_FALSE)) {
        return AT_ERROR;
    }

    /* 用来保存NV中密文的 */
    (VOS_VOID)memset_s(&encrptPwd, sizeof(TAF_NVIM_ApnKeyList), 0x00, sizeof(TAF_NVIM_ApnKeyList));
    /* 读取voip apn对应的NV项 */
    if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_VOIP_APN_KEY, (VOS_UINT8 *)&encrptPwd, sizeof(TAF_NVIM_ApnKeyList)) != VOS_OK) {
        AT_WARN_LOG("AT_QryVoipApnKeyPara:READ NV FAIL");
        (VOS_VOID)memset_s(&encrptPwd, sizeof(TAF_NVIM_ApnKeyList), 0x00, sizeof(TAF_NVIM_ApnKeyList));
        return AT_ERROR;
    }
    /* 解密后的密码是否有效，useFlag为1表示有效 */
    if (encrptPwd.useFlag == 1) {
        /* 密码有效，上报AT，组数固定为1，当前只支持1组密码，不超过120个字节 */
        g_atSendDataBuff.bufLen= (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d%s%s:%d,\"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
            1, g_atCrLf, g_parseContext[indexNum].cmdElement->cmdName, 0, encrptPwd.apnKey);
    } else {
        /* 密码无效，返回0 */
        g_atSendDataBuff.bufLen= (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, 0);
    }

    (VOS_VOID)memset_s(&encrptPwd, sizeof(TAF_NVIM_ApnKeyList), 0x00, sizeof(TAF_NVIM_ApnKeyList));
    return AT_OK;
}
#endif

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)

VOS_UINT32 AT_QryShellPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 shellStatus;

    shellStatus = AT_SHELL_CLOSE;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_AT_SHELL_OPEN_FLAG, &shellStatus, sizeof(shellStatus)) != NV_OK) {
        AT_WARN_LOG("AT_QryShellPara:WARNING:TAF_ACORE_NV_READ SHELL_OPEN_FLAG faild!");

        return AT_ERROR;
    }

    /* shell的状态只能为0,1,2 */
    if (shellStatus > AT_SHELL_OPEN) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, shellStatus);
    return AT_OK;
}
#endif


VOS_UINT32 AT_QryWifiGlobalMacPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;
    VOS_UINT32 result;
    VOS_UINT8  e5GwMacAddr[AT_MAC_ADDR_LEN + 1]; /* MAC地址 */

    /* 读取网关MAC地址 */
    result                       = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WIFI_MAC_ADDR, e5GwMacAddr, AT_MAC_ADDR_LEN);
    e5GwMacAddr[AT_MAC_ADDR_LEN] = '\0';

    if (result != NV_OK) {
        AT_WARN_LOG("AT_QryApMacPara:read en_NV_Item_GWMAC_ADDR failed or no need to updatee");
        return AT_ERROR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, e5GwMacAddr);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryFlashInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取FLASH信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_FLASHINFO_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryFlashInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FLASHINFO_READ;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryAuthverPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取SIMLOCK MANAGER版本信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_AUTHVER_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryAuthverPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_AUTHVER_READ;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryProdTypePara(VOS_UINT8 indexNum)
{
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_PRODTYPE_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PRODTYPE_QRY; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                 /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_QryTmmiPara(VOS_UINT8 indexNum)
{
    VOS_UINT8  facInfo[AT_FACTORY_INFO_LEN];
    VOS_UINT8  mmiFlag;
    VOS_UINT32 result;

    result = TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_Factory_Info, facInfo, AT_FACTORY_INFO_LEN);

    if (result != NV_OK) {
        return AT_ERROR;
    }

    if (VOS_MemCmp(&facInfo[AT_MMI_TEST_FLAG_OFFSET], AT_MMI_TEST_FLAG_SUCC, AT_MMI_TEST_FLAG_LEN) == VOS_OK) {
        mmiFlag = VOS_TRUE;
    } else {
        mmiFlag = VOS_FALSE;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, mmiFlag);

    return AT_OK;
}


VOS_UINT32 AT_GetWcdmaBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen, AT_UE_BandCapaSt *bandCapa)
{
    VOS_UINT32 len;
    len = 0;

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaI2100 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC1,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaIi1900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC2,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaIii1800 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC3,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaIv1700 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC4,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaV850 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC5,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaVi800 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC6,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaVii2600 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC7,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaViii900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC8,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaIxJ1700 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC9,");
    }

    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaXi1500 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "BC11,");
    }
    if (bandCapa->unWRFSptBand.bitBand.bandWcdmaXix850 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "BC19,");
    }

    if (len != 0) {
        gsmBandstr[len - 1] = '\0';
    }

    return len;
}


VOS_UINT32 AT_GetWcdmaDivBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen)
{
    VOS_UINT32 len;
    VOS_UINT32 band;

    AT_WCDMA_PrefBand *bitBand = VOS_NULL_PTR;

    /*
     * 获取UMTS支持的分集频带集合
     * V3R2版本是en_NV_Item_W_RF_DIV_BAND，V3R1是en_NV_Item_ANTENNA_CONFIG
     */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_W_RF_DIV_BAND, &band, sizeof(band)) != NV_OK) {
        AT_WARN_LOG("AT_GetWcdmaDivBandStr: Read NVIM Smss Error");
        return 0;
    }

    len     = 0;
    bitBand = (AT_WCDMA_PrefBand *)&band;

    /*
     * 单板分集支持的BAND 通路，数据为十进制数，转化为二进制后
     * 从右往左依次为Band1、2、3……。
     */

    if (bitBand->bandWcdmaI2100 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U1,");
    }

    if (bitBand->bandWcdmaIi1900 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U2,");
    }

    if (bitBand->bandWcdmaIii1800 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U3,");
    }

    if (bitBand->bandWcdmaIv1700 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U4,");
    }

    if (bitBand->bandWcdmaV850 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U5,");
    }

    if (bitBand->bandWcdmaVi800 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U6,");
    }

    if (bitBand->bandWcdmaVii2600 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U7,");
    }

    if (bitBand->bandWcdmaViii900 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U8,");
    }

    if (bitBand->bandWcdmaIxJ1700 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U9,");
    }

    if (bitBand->bandWcdmaXi1500 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U11,");
    }

    if (bitBand->bandWcdmaXix850 == 1) {
        len += (VOS_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (VOS_CHAR *)gsmBandstr,
            (VOS_CHAR *)gsmBandstr + len, "U19,");
    }

    if (len != 0) {
        gsmBandstr[len - 1] = '\0';
    }

    return len;
}


VOS_UINT32 AT_GetGsmBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen, AT_UE_BandCapaSt *bandCapa)
{
    VOS_UINT32 len;

    len = 0;

    if (bandCapa->unGRFSptBand.bitBand.bandGsm450 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "450,");
    }

    if (bandCapa->unGRFSptBand.bitBand.bandGsm480 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "480,");
    }

    if (bandCapa->unGRFSptBand.bitBand.bandGsm850 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "850,");
    }

    if (bandCapa->unGRFSptBand.bitBand.bandGsmP900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "P900,");
    }
    if (bandCapa->unGRFSptBand.bitBand.bandGsmR900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "R900,");
    }
    if (bandCapa->unGRFSptBand.bitBand.bandGsmE900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "E900,");
    }
    if (bandCapa->unGRFSptBand.bitBand.bandGsm1800 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "1800,");
    }
    if (bandCapa->unGRFSptBand.bitBand.bandGsm1900 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "1900,");
    }
    if (bandCapa->unGRFSptBand.bitBand.bandGsm700 == 1) {
        len += (TAF_UINT32)AT_FormatReportString((VOS_INT32)gsmBandLen, (TAF_CHAR *)gsmBandstr,
            (TAF_CHAR *)gsmBandstr + len, "700,");
    }

    if (len != 0) {
        gsmBandstr[len - 1] = '\0';
    }

    return len;
}

VOS_UINT32 AT_QryFeaturePara(VOS_UINT8 indexNum)
{
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               ID_AT_MTA_SFEATURE_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SFEATURE_QRY; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                 /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryProdNamePara(VOS_UINT8 indexNum)
{
    TAF_AT_ProductId productId;
    VOS_UINT32       ret;

    (VOS_VOID)memset_s(&productId, sizeof(productId), 0x00, sizeof(TAF_AT_ProductId));

    /* 从NV50048中读取产品名称 */
    /* 读取NV项NV_ITEM_PRODUCT_ID获取产品名称 */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_PRODUCT_ID, &productId,
                            sizeof(productId.nvStatus) + sizeof(productId.productId));

    if (ret != NV_OK) {
        AT_WARN_LOG("AT_QryProdNamePara: Fail to read NV_ITEM_PRODUCT_ID");
        return AT_ERROR;
    }

    /* 该NV是否使能 */
    if (productId.nvStatus != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, productId.productId);

    return AT_OK;
}


VOS_UINT32 AT_QryEqverPara(VOS_UINT8 indexNum)
{
    /*
     * <version>装备归一化 AT命令版本号，版本号以三位表示，每位为0~9的数字字符。
     * 该版本号取决于单板实现的AT命令时参考的本文档的版本号。
     * 114 支持归一化AT命令版本查询的第一个版本
     * balong固定返回115
     */
    VOS_UINT32   ret;
    TAF_AT_EqVer eqver;

    eqver.eqver = 0;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_EQVER, &eqver.eqver, sizeof(eqver.eqver));
    if (ret != NV_OK) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, eqver.eqver);

    return AT_OK;
}


VOS_UINT32 AT_QryApRptSrvUrlPara(VOS_UINT8 indexNum)
{
    VOS_UINT8              apRptSrvUrl[AT_AP_NVIM_XML_RPT_SRV_URL_LEN + 1];
    VOS_UINT32             ret;
    errno_t                memResult;
    TAF_AT_NvimApRptSrvUrl apRptSrvUrlInfo;

    (VOS_VOID)memset_s(apRptSrvUrl, sizeof(apRptSrvUrl), 0x00, sizeof(apRptSrvUrl));

    (VOS_VOID)memset_s(&apRptSrvUrlInfo, sizeof(apRptSrvUrlInfo), 0x00, sizeof(apRptSrvUrlInfo));

    /* 读NV:NV_ITEM_AP_RPT_SRV_URL */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AP_RPT_SRV_URL, &apRptSrvUrlInfo, AT_AP_NVIM_XML_RPT_SRV_URL_LEN);

    if (ret != NV_OK) {
        AT_ERR_LOG("AT_QryApRptSrvUrlPara:Read NV fail");
        return AT_ERROR;
    }
    memResult = memcpy_s(apRptSrvUrl, sizeof(apRptSrvUrl), apRptSrvUrlInfo.apRptSrvUrl, AT_AP_NVIM_XML_RPT_SRV_URL_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(apRptSrvUrl), AT_AP_NVIM_XML_RPT_SRV_URL_LEN);
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:\"%s\"", g_parseContext[indexNum].cmdElement->cmdName, apRptSrvUrl);

    return AT_OK;
}


VOS_UINT32 AT_QryApXmlInfoTypePara(VOS_UINT8 indexNum)
{
    VOS_UINT8                apXmlInfoType[AT_AP_XML_RPT_INFO_TYPE_LEN + 1];
    VOS_UINT32               ret;
    errno_t                  memResult;
    TAF_AT_NvimApXmlInfoType apXmlInfoTypeInfo;

    (VOS_VOID)memset_s(apXmlInfoType, sizeof(apXmlInfoType), 0x00, sizeof(apXmlInfoType));

    (VOS_VOID)memset_s(&apXmlInfoTypeInfo, sizeof(apXmlInfoTypeInfo), 0x00, sizeof(apXmlInfoTypeInfo));

    /* 读NV:NV_ITEM_AP_XML_INFO_TYPE */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AP_XML_INFO_TYPE, &apXmlInfoTypeInfo, AT_AP_NVIM_XML_RPT_INFO_TYPE_LEN);

    if (ret != NV_OK) {
        AT_ERR_LOG("AT_QryApXmlInfoTypePara:Read NV fail");
        return AT_ERROR;
    }

    memResult = memcpy_s(apXmlInfoType, sizeof(apXmlInfoType), apXmlInfoTypeInfo.apXmlInfoType,
                         AT_AP_NVIM_XML_RPT_INFO_TYPE_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(apXmlInfoType), AT_AP_NVIM_XML_RPT_INFO_TYPE_LEN);

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, apXmlInfoType);

    return AT_OK;
}


VOS_UINT32 AT_QryApXmlRptFlagPara(VOS_UINT8 indexNum)
{
    VOS_UINT32             ret;
    TAF_AT_NvimApXmlRptFlg apXmlRptFlg;

    apXmlRptFlg.apXmlRptFlg = VOS_FALSE;

    /* 读NV:NV_ITEM_AP_XML_RPT_FLAG */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AP_XML_RPT_FLAG, &apXmlRptFlg, sizeof(TAF_AT_NvimApXmlRptFlg));

    if (ret != NV_OK) {
        AT_ERR_LOG("AT_QryApXmlRptFlagPara:Read NV fail");
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            apXmlRptFlg.apXmlRptFlg);

    return AT_OK;
}


VOS_UINT32 AT_QryFastDormPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rslt;

    /* 调用MN_FillAndSndAppReqMsg()，等待RABM的回复 */
    rslt = AT_SndQryFastDorm(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (rslt == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FASTDORM_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_QryAcpuMemInfoPara(TAF_UINT8 indexNum)
{
    PS_PRINTF_INFO("<AT_QryAcpuMemInfoPara> at cmd\n");

    /* 命令状态类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCcpuMemInfoPara(TAF_UINT8 indexNum)
{
    VOS_UINT32 ret;

    PS_PRINTF_INFO("<AT_QryCcpuMemInfoPara> at cmd\n");

    /* 命令状态类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_DEVICE_OTHER_ERROR;
    }

    /* 发消息到C核处理CCPU内存泄漏检查 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_QRY_CCPU_MEM_INFO_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (ret == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCPUMEMINFO_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_QryCcpuMemInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCipherPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 发消息给MMA获取Cipher信息 */
    result = TAF_MMA_QryCipherReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);

    /* 执行命令操作 */
    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CIPERQRY_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


VOS_UINT32 AT_QryLocinfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 发消息给MMA获取当前UE的位置信息 */
    result = TAF_MMA_QryLocInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);

    /* 执行命令操作 */
    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LOCINFO_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryNvResumePara(VOS_UINT8 indexNum)
{
    VOS_UINT16 nvResumeFlag;

    nvResumeFlag = AT_NV_RESUME_SUCC;

    /* 读取en_NV_Resume_Flag */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Resume_Flag, &nvResumeFlag, sizeof(VOS_UINT16)) != NV_OK) {
        AT_WARN_LOG("AT_QryNvResumePara:READ NV FAIL");
        return AT_ERROR;
    }

    /* 读出的值只能为0或者1 */
    if ((nvResumeFlag != AT_NV_RESUME_SUCC) && (nvResumeFlag != AT_NV_RESUME_FAIL)) {
        AT_WARN_LOG("AT_QryNvResumePara:PARA IS INVALID");
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, nvResumeFlag);
    return AT_OK;
}


VOS_UINT32 AT_QryNvBackupStatusPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取当前NV备份状态信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_NVBACKUPSTAT_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNvBackupStatusPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NVBACKUPSTAT_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryNandBadBlockPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取NAND FLASH的所有坏块索引列表信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_NANDBBC_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNandBadBlockPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NANDBBC_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryNandDevInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取NAND FLASH的型号信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_NANDVER_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNandDevInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NANDVER_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryChipTempPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 发消息到C核获取PA、SIM卡和电池的温度信息 */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_CHIPTEMP_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryChipTempPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CHIPTEMP_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryApRptPortSelectPara(VOS_UINT8 indexNum)
{
    AT_PortRptCfg                 rptCfg;
    AT_ClientConfiguration       *clientCfg = VOS_NULL_PTR;
    const AT_ClientCfgMapTab     *cfgMapTbl = VOS_NULL_PTR;
    VOS_UINT8                     i;

    (VOS_VOID)memset_s(&rptCfg, sizeof(rptCfg), 0x00, sizeof(rptCfg));

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    for (i = 0; i < AT_GET_CLIENT_CFG_TAB_LEN(); i++) {
        cfgMapTbl = AT_GetClientCfgMapTbl(i);
        clientCfg = AT_GetClientConfig(cfgMapTbl->clientId);

        if (clientCfg->reportFlg == VOS_TRUE) {
            rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_0] |= cfgMapTbl->rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_0];
            rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_1] |= cfgMapTbl->rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_1];
            rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_2] |= cfgMapTbl->rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_2];
        }
    }

    /* 返回查询结果 */
    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %08X, %08X, %08X", g_parseContext[indexNum].cmdElement->cmdName,
            rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_0], rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_1],
            rptCfg.rptCfgBit32[AT_RPT_CFG_BIT_32_INDEX_2]);

    return AT_OK;
}


VOS_UINT32 At_QryUsbSwitchPara(VOS_UINT8 indexNum)
{
    VOS_UINT8  usbStatus;
    VOS_UINT32 result;

    usbStatus = USB_SWITCH_OFF;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    if (DRV_USB_PHY_SWITCH_GET(&usbStatus) != VOS_OK) {
        return AT_ERROR;
    }

    result = (usbStatus == USB_SWITCH_ON) ? AT_USB_SWITCH_SET_VBUS_VALID : AT_USB_SWITCH_SET_VBUS_INVALID;

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "+USBSWITCH: %X", result);

    return AT_OK;
}


VOS_UINT32 AT_QryAntState(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;
    VOS_UINT16 antState;

    AT_PR_LOGI("Rcv Msg");

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    antState = 0;

    ret = TAF_AGENT_GetAntState(indexNum, &antState);
    AT_PR_LOGI("Call interface success!");
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_QryAntState: TAF_AGENT_GetAntState fail");

        return AT_ERROR;
    }

    /* 根据接口约定，天线状态非0时，直接返回1 */
#ifndef FEATURE_ANTEN_DETECT
    if ((antState & AT_CMD_MAX_ANT_BIT_MASK) == 0)
#else
    if (antState == 0)
#endif
    {
        AT_NORM_LOG("AT_QryAntState: usAntState Set to 0");
        antState = 0;
    } else {
        AT_NORM_LOG("AT_QryAntState: usAntState Set to 1");
        antState = 1;
    }

    /* AT输出 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, antState);
    return AT_OK;
}


VOS_UINT32 AT_QrySARReduction(VOS_UINT8 indexNum)
{
    VOS_UINT32         ret;
    VOS_UINT16         uETestMode;
    AT_ModemMtInfoCtx *mtInfoCtx = VOS_NULL_PTR;
    ModemIdUint16      modemId;
    VOS_UINT16         antState;

    uETestMode = VOS_FALSE;

    /* 读取NV项判定是否为测试模式 */
    ret = TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_RF_SAR_BACKOFF_TESTMODE, &uETestMode, sizeof(uETestMode));

    if (ret != NV_OK) {
        AT_ERR_LOG("AT_QrySARReduction:Read NV fail");
        return AT_ERROR;
    }

    modemId = MODEM_ID_0;

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_QrySARReduction:Get modem id fail");
        return AT_ERROR;
    }

    mtInfoCtx = AT_GetModemMtInfoCtxAddrFromModemId(modemId);

    antState = 0;
    ret      = TAF_AGENT_GetAntState(indexNum, &antState);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_QrySARReduction:Get modem id fail");
        return AT_ERROR;
    }

#ifndef FEATURE_ANTEN_DETECT
    if ((antState == AT_ANT_CONDUCTION_MODE) && (uETestMode != VOS_TRUE))
#else
    if ((antState != AT_ANT_AIRWIRE_MODE) && (uETestMode != VOS_TRUE))
#endif
    {
        AT_ERR_LOG("AT_QrySARReduction: ant state wrong");
        return AT_ERROR;
    }

    /* AT输出 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%u", g_parseContext[indexNum].cmdElement->cmdName,
            mtInfoCtx->sarReduction);

    return AT_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 AT_QryRsrpCfgPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_LteRsrpCfg rsrpCfg;
    VOS_UINT32   i = 0;

    (VOS_VOID)memset_s(&rsrpCfg, sizeof(rsrpCfg), 0x00, sizeof(rsrpCfg));

    if (g_ate5RightFlag == AT_E5_RIGHT_FLAG_YES) {
        if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_TAF_LTE_RSRP_CFG, &rsrpCfg, sizeof(rsrpCfg)) != NV_OK) {
            PS_PRINTF_WARNING("read RSRP_CFG NV fail!\n");
            return AT_ERROR;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        for (i = 0; i < AT_RSRP_CFG_SS_VALUE_LEN - 1; i++) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(%d)%d,",
                i + 1, rsrpCfg.value[i]);
        }

        g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(4)%d",
            rsrpCfg.value[AT_RSRP_CFG_SS_VALUE_INDEX_3]);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_QryRscpCfgPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_LteRscpCfg rscpCfg;
    VOS_UINT32   i = 0;

    (VOS_VOID)memset_s(&rscpCfg, sizeof(rscpCfg), 0x00, sizeof(rscpCfg));

    if (g_ate5RightFlag == AT_E5_RIGHT_FLAG_YES) {
        if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_TAF_LTE_RSCP_CFG, &rscpCfg, sizeof(rscpCfg)) != NV_OK) {
            return AT_ERROR;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        for (i = 0; i < AT_RSCP_CFG_SS_VALUE_LEN - 1; i++) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(%d)%d,",
                i + 1, rscpCfg.value[i]);
        }

        g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(4)%d",
            rscpCfg.value[AT_RSCP_CFG_SS_VALUE_INDEX_3]);
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}

VOS_UINT32 AT_QryEcioCfgPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_LteEcioCfg ecioCfg;
    VOS_UINT32   i = 0;

    (VOS_VOID)memset_s(&ecioCfg, sizeof(ecioCfg), 0x00, sizeof(ecioCfg));

    if (g_ate5RightFlag == AT_E5_RIGHT_FLAG_YES) {
        if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_TAF_LTE_ECIO_CFG, &ecioCfg, sizeof(ecioCfg)) != NV_OK) {
            PS_PRINTF_WARNING("read ECIO_CFG NV fail!\n");
            return AT_ERROR;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        for (i = 0; i < AT_ECIO_CFG_SS_VALUE_LEN - 1; i++) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(%d)%d,",
                i + 1, ecioCfg.value[i]);
        }

        g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "(4)%d",
            ecioCfg.value[AT_ECIO_CFG_SS_VALUE_INDEX_3]);
    } else {
        return AT_ERROR; /* 如果权限未打开，返回ERROR */
    }

    return AT_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 At_QrySfm(VOS_UINT8 indexNum)
{
    OM_SW_VerFlag customVersion;

    customVersion.nvSwVerFlag = 0;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SW_VERSION_FLAG, &customVersion.nvSwVerFlag,
                          sizeof(customVersion.nvSwVerFlag)) != VOS_OK) {
        return AT_ERROR;
    } else {
        /* AT命令与NV中存储的信息相反 */
        if (customVersion.nvSwVerFlag == 0) {
            customVersion.nvSwVerFlag = 1;

        } else {
            customVersion.nvSwVerFlag = 0;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "^SFM: %d", customVersion.nvSwVerFlag);

        return AT_OK;
    }
}
#endif
#endif

#if (FEATURE_PHONE_SC == FEATURE_ON)

VOS_UINT32 AT_QryPhoneSimlockInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    AT_PR_LOGI("Rcv Msg");

    /* 发送跨核消息到C核, 查询锁网锁卡信息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_PHONESIMLOCKINFO_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryPhoneSimlockInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    AT_PR_LOGI("Snd Msg");

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHONESIMLOCKINFO_READ;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QrySimlockDataReadPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    AT_PR_LOGI("Rcv Msg");

    /* 发送跨核消息到C核, 查询锁网锁卡信息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_SIMLOCKDATAREAD_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QrySimlockDataReadPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    AT_PR_LOGI("Snd Msg");

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIMLOCKDATAREAD_READ;
    return AT_WAIT_ASYNC_RETURN;
}

/*
 * 功能描述: ^GETMODEMSCID查询命令处理函数
 */
VOS_UINT32 AT_QryModemScIdPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    AT_PR_LOGI("Rcv Msg");

    /* 发送跨核消息到C核, 查询锁网锁卡信息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_GETMODEMSCID_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryModemScIdPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    AT_PR_LOGI("Snd Msg");

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GETMODEMSCID_READ;
    return AT_WAIT_ASYNC_RETURN;
}
#endif


#if (VOS_WIN32 == VOS_OS_VER)

VOS_UINT32 At_QryTinTypeStub(VOS_UINT8 indexNum)
{
    VOS_UINT8 tinType;

#if (FEATURE_LTE == FEATURE_ON)
    tinType = NAS_GetTinType();
#endif
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, tinType);
    return AT_OK;
}
#endif


VOS_UINT32 AT_QryCposrPara(VOS_UINT8 indexNum)
{
    AT_ModemAgpsCtx *agpsCtx = VOS_NULL_PTR;

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(indexNum);

    /* 打印+CPOSR主动上报控制当前状态 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            agpsCtx->cposrReport);

    return AT_OK;
}


VOS_UINT32 AT_QryXcposrPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 给MTA发送+xcposr查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_XCPOSR_QRY_REQ, VOS_NULL_PTR, 0,
                                    I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryXcposrPara: send Msg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_XCPOSR_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryXcposrRptPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 给MTA发送查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_XCPOSRRPT_QRY_REQ, VOS_NULL_PTR, 0,
                                    I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryXcposrRptPara: send Msg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_XCPOSRRPT_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCPsErrPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        AT_PS_GetPsCallErrCause(indexNum));

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryCmutPara(VOS_UINT8 indexNum)
{
    if (APP_VC_GetMuteStatus(g_atClientTab[indexNum].clientId, 0) != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMUT_READ;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCCsErrPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        AT_GetCsCallErrCause(indexNum));

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryCerssiPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = TAF_MMA_QryCerssiReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    /* 执行命令操作 */
    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CERSSI_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 At_QryCserssiPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = TAF_MMA_QryCserssiReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    /* 执行命令操作 */
    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSERSSI_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryNrBandBlockListPara(TAF_UINT8 atIndex)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_MMA_QryNrBandBlockListReq(WUEPS_PID_AT, g_atClientTab[atIndex].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[atIndex].cmdCurrentOpt = AT_CMD_NR_BAND_BLOCKLIST_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif

#if (FEATURE_LTE == FEATURE_ON)


VOS_UINT32 At_QryCecellidPara(VOS_UINT8 indexNum)
{
    /* 调用L 提供接口 */
    return At_QryCellIdPara(indexNum);
}
#endif


VOS_UINT32 AT_QryCbgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* AT 给VC 发送模式查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    APP_VC_MSG_FOREGROUND_QRY, VOS_NULL_PTR, 0, I0_WUEPS_PID_VC);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryCbgPara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CBG_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryAcInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 发消息给MMA获取当前UE的位置信息 */
    result = TAF_MMA_QryAcInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    /* 执行命令操作 */
    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ACINFO_READ;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_QryCLteRoamAllowPara(VOS_UINT8 indexNum)
{
    VOS_UINT8                           lteRoamAllow;
    NAS_MMC_NvimLteInternationalRoamCfg nvimLteRoamAllowedFlg;

    /* 局部变量初始化 */
    lteRoamAllow                            = VOS_FALSE;
    nvimLteRoamAllowedFlg.lteRoamAllowedFlg = VOS_FALSE;

    /* 读取NV, 该NV的结构为 NAS_MMC_NvimLteInternationalRoamCfg, 只读取第1个字节 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_LTE_INTERNATION_ROAM_CONFIG, &nvimLteRoamAllowedFlg,
                          sizeof(NAS_MMC_NvimLteInternationalRoamCfg)) != NV_OK) {
        AT_WARN_LOG("AT_QryCLteRoamAllowPara(): NV_ITEM_LTE_INTERNATION_ROAM_CONFIG TAF_ACORE_NV_READ Error");
        return AT_ERROR;
    }

    /* 容错处理, NV中值为VOS_FALSE时即不允许漫游, 为其它值时即为允许漫游 */
    if (nvimLteRoamAllowedFlg.lteRoamAllowedFlg == VOS_FALSE) {
        lteRoamAllow = VOS_FALSE;
    } else {
        lteRoamAllow = VOS_TRUE;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, lteRoamAllow);
    return AT_OK;
}
#endif


VOS_UINT32 At_QryMmPlmnInfoPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryMMPlmnInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MMPLMNINFO_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryEonsUcs2Para(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryEonsUcs2Req(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EONSUCS2_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryPlmnPara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryPlmnReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PLMN_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryXlemaPara(VOS_UINT8 indexNum)
{
    VOS_UINT32    rst;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发消息到C核查询紧急呼叫号码 */
    rst = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_QRY_XLEMA_REQ, 0, modemId);

    if (rst != MN_ERR_NO_ERROR) {
        AT_WARN_LOG("At_QryXlemaPara: TAF_CCM_CallCommonReq fail.");
        return AT_ERROR;
    }

    /* 设置结束直接返回OK */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_XLEMA_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryBodySarOnPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, g_atBodySarState);
    return AT_OK;
}


VOS_UINT32 AT_FillBodySarWcdmaQryPara(MTA_BodySarPara *bodySarPara, AT_BodysarwcdmaSetPara *bodySarWcdmaPara)
{
    VOS_UINT32 loop1;
    VOS_UINT8         loop2;
    AT_NvWgRfMainBand wGBand;
    VOS_UINT32        tmpBand;

    (VOS_VOID)memset_s(&wGBand, sizeof(wGBand), 0x00, sizeof(wGBand));
    tmpBand = 0;

    /* 获取WCDMA Band能力值 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_WG_RF_MAIN_BAND, &wGBand, sizeof(wGBand)) != NV_OK) {
        AT_ERR_LOG("AT_FillBodySarWcdmaQryPara: Read NV fail!");
        return VOS_ERR;
    }

    for (loop1 = 0; loop1 < (VOS_UINT32)TAF_MIN(bodySarPara->wBandNum, MTA_BODY_SAR_WBAND_MAX_NUM); loop1++) {
        for (loop2 = 0; loop2 < TAF_MIN(bodySarWcdmaPara->paraNum, AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM); loop2++) {
            if (bodySarPara->wBandPara[loop1].power == bodySarWcdmaPara->asPower[loop2]) {
                break;
            }
        }

        if ((loop2 == bodySarWcdmaPara->paraNum) && (loop2 < AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM)) {
            bodySarWcdmaPara->paraNum++;
            bodySarWcdmaPara->asPower[loop2] = bodySarPara->wBandPara[loop1].power;
        }

        if (loop2 < AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM) {
            bodySarWcdmaPara->band[loop2] |= 0x00000001U << (bodySarPara->wBandPara[loop1].band - 1);
            tmpBand |= bodySarWcdmaPara->band[loop2];
        }
    }

    /* 剩余未设置的频段返回默认值 */
    tmpBand = wGBand.unWcdmaBand.band & (~tmpBand);
    if ((tmpBand != 0) && (bodySarWcdmaPara->paraNum < AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM)) {
        bodySarWcdmaPara->band[bodySarWcdmaPara->paraNum]    = tmpBand;
        bodySarWcdmaPara->asPower[bodySarWcdmaPara->paraNum] = AT_WCDMA_BAND_DEFAULT_POWER;
        bodySarWcdmaPara->paraNum++;
    }

    return VOS_OK;
}


VOS_UINT32 AT_QryBodySarWcdmaPara(VOS_UINT8 indexNum)
{
    MTA_BodySarPara        bodySarPara;
    AT_BodysarwcdmaSetPara bodySarWcdmaPara;
    VOS_UINT8              loop;
    VOS_UINT16             length;

    (VOS_VOID)memset_s(&bodySarPara, sizeof(bodySarPara), 0x00, sizeof(bodySarPara));
    (VOS_VOID)memset_s(&bodySarWcdmaPara, sizeof(bodySarWcdmaPara), 0x00, sizeof(bodySarWcdmaPara));
    length = 0;

    /* 从NV项中读取Body SAR功率门限值 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_BODY_SAR_PARA, &bodySarPara, sizeof(bodySarPara)) != NV_OK) {
        AT_ERR_LOG("AT_QryBodySarWcdmaPara: Read NV fail!");
        return AT_ERROR;
    }

    /* 从Body SAR参数结构体填充^BODYSARWCDMA命令参数结构体 */
    if (AT_FillBodySarWcdmaQryPara(&bodySarPara, &bodySarWcdmaPara) != VOS_OK) {
        AT_ERR_LOG("AT_QryBodySarWcdmaPara: AT_FillBodySarWcdmaQryPara fail!");
        return AT_ERROR;
    }

    /* 打印命令名 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 打印WCDMA频段Body SAR参数 */
    if (bodySarWcdmaPara.paraNum == 1) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "(%d,%08X)", bodySarWcdmaPara.asPower[0], AT_RF_BAND_ANY);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    }

    for (loop = 0; loop < bodySarWcdmaPara.paraNum; loop++) {
        if (loop != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)(g_atSndCodeAddress + length), ",");
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "(%d,%08X)", bodySarWcdmaPara.asPower[loop],
            bodySarWcdmaPara.band[loop]);
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_FillBodySarGsmDefaultPower(MTA_BodySarPara *bodySarPara)
{
    VOS_UINT32 gBandCapa;
    VOS_UINT32 tmpBand;

    gBandCapa = 0;
    tmpBand   = 0;

    /* 获取GSM Band能力值 */
    if (AT_GetGsmBandCapa(&gBandCapa) != VOS_OK) {
        AT_ERR_LOG("AT_FillBodySarGsmDefaultPower: AT_GetGsmBandCapa fail!");
        return VOS_ERR;
    }

    tmpBand = gBandCapa & (~bodySarPara->gBandMask);

    if ((AT_BODY_SAR_GBAND_GPRS_850_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_GPRS_850_MASK;
        bodySarPara->gBandPara[AT_GSM_850].gprsPower = AT_GSM_GPRS_850_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_EDGE_850_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_EDGE_850_MASK;
        bodySarPara->gBandPara[AT_GSM_850].edgePower = AT_GSM_EDGE_850_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_GPRS_900_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_GPRS_900_MASK;
        bodySarPara->gBandPara[AT_GSM_900].gprsPower = AT_GSM_GPRS_900_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_EDGE_900_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_EDGE_900_MASK;
        bodySarPara->gBandPara[AT_GSM_900].edgePower = AT_GSM_EDGE_900_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_GPRS_1800_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_GPRS_1800_MASK;
        bodySarPara->gBandPara[AT_GSM_1800].gprsPower = AT_GSM_GPRS_1800_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_EDGE_1800_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_EDGE_1800_MASK;
        bodySarPara->gBandPara[AT_GSM_1800].edgePower = AT_GSM_EDGE_1800_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_GPRS_1900_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_GPRS_1900_MASK;
        bodySarPara->gBandPara[AT_GSM_1900].gprsPower = AT_GSM_GPRS_1900_DEFAULT_POWER;
    }

    if ((AT_BODY_SAR_GBAND_EDGE_1900_MASK & tmpBand) != 0) {
        bodySarPara->gBandMask |= AT_BODY_SAR_GBAND_EDGE_1900_MASK;
        bodySarPara->gBandPara[AT_GSM_1900].edgePower = AT_GSM_EDGE_1900_DEFAULT_POWER;
    }

    return VOS_OK;
}


VOS_VOID AT_FillBodySarGsmByModulationMode(MTA_BodySarPara *bodySarPara, VOS_UINT8 modulationOffset,
                                           AT_BodysargsmSetPara *bodySarGsmPara)
{
    VOS_UINT8 loop1;
    VOS_UINT8 loop2;

    for (loop1 = AT_GSM_850; loop1 < AT_GSM_BAND_BUTT; loop1++) {
        for (loop2 = 0; loop2 < AT_MIN(bodySarGsmPara->paraNum, AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM); loop2++) {
            if (modulationOffset == AT_GSM_GPRS_BAND_OFFSET) {
                if (bodySarPara->gBandPara[loop1].gprsPower == bodySarGsmPara->asPower[loop2]) {
                    break;
                }
            } else {
                if (bodySarPara->gBandPara[loop1].edgePower == bodySarGsmPara->asPower[loop2]) {
                    break;
                }
            }
        }

        if ((loop2 == bodySarGsmPara->paraNum) && (loop2 < AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM)) {
            bodySarGsmPara->paraNum++;
            if (modulationOffset == AT_GSM_GPRS_BAND_OFFSET) {
                bodySarGsmPara->asPower[loop2] = bodySarPara->gBandPara[loop1].gprsPower;
            } else {
                bodySarGsmPara->asPower[loop2] = bodySarPara->gBandPara[loop1].edgePower;
            }
        }
        if (loop2 < AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM) {
            bodySarGsmPara->band[loop2] |= 0x00000001U << (loop1 + modulationOffset);
        }
    }

    return;
}


VOS_UINT32 AT_QryBodySarGsmPara(VOS_UINT8 indexNum)
{
    MTA_BodySarPara      bodySarPara;
    AT_BodysargsmSetPara bodySarGsmPara;
    VOS_UINT8            loop;
    VOS_UINT16           length;

    (VOS_VOID)memset_s(&bodySarPara, sizeof(bodySarPara), 0x00, sizeof(bodySarPara));
    (VOS_VOID)memset_s(&bodySarGsmPara, sizeof(bodySarGsmPara), 0x00, sizeof(bodySarGsmPara));
    length = 0;

    /* 从NV项中读取Body SAR功率门限值 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_BODY_SAR_PARA, &bodySarPara, sizeof(bodySarPara)) != NV_OK) {
        AT_ERR_LOG("AT_QryBodySarGsmPara: Read NV fail!");
        return AT_ERROR;
    }

    /* 填充GSM频段Body SAR参数默认值 */
    if (AT_FillBodySarGsmDefaultPower(&bodySarPara) != VOS_OK) {
        AT_ERR_LOG("AT_QryBodySarGsmPara: AT_FillBodySarGsmDefaultPower fail!");
        return AT_ERROR;
    }

    /* 从Body SAR参数结构体填充^BODYSARGSM命令参数结构体 */
    /* GPRS调制方式 */
    AT_FillBodySarGsmByModulationMode(&bodySarPara, AT_GSM_GPRS_BAND_OFFSET, &bodySarGsmPara);

    /* EDGE调制方式 */
    AT_FillBodySarGsmByModulationMode(&bodySarPara, AT_GSM_EDGE_BAND_OFFSET, &bodySarGsmPara);

    /* 打印命令名 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 打印GSM频段Body SAR参数 */
    if (bodySarGsmPara.paraNum == 1) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "(%d,%08X)", bodySarGsmPara.asPower[0], AT_RF_BAND_ANY);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    }

    for (loop = 0; loop < AT_MIN(bodySarGsmPara.paraNum, AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM); loop++) {
        if (loop != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)(g_atSndCodeAddress + length), ",");
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "(%d,%08X)", bodySarGsmPara.asPower[loop],
            bodySarGsmPara.band[loop]);
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryNvLoadPara(VOS_UINT8 indexNum)
{
    AT_ModemNvloadCtx  *nvloadCarrierCtx = VOS_NULL_PTR;
    errno_t             memResult;
    VOS_UINT16          length  = 0;
    ModemIdUint16       modemId = MODEM_ID_0;
    VOS_UINT8           carrierName[AT_SET_NV_CARRIER_NAME_LEN + 1];

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_QryNvLoadPara: Get modem id fail.");
        return AT_ERROR;
    }

    nvloadCarrierCtx = AT_GetModemNvloadCtxAddrFromModemId(modemId);

    (VOS_VOID)memset_s(carrierName, sizeof(carrierName), 0, sizeof(carrierName));

    memResult = memcpy_s(carrierName, sizeof(carrierName), nvloadCarrierCtx->carrierName, AT_SET_NV_CARRIER_NAME_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(carrierName), AT_SET_NV_CARRIER_NAME_LEN);

    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, " \"%s\",", carrierName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, " %d", nvloadCarrierCtx->specialEffectiveFlg);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_QryIMEIVerifyPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 ID_AT_MTA_IMEI_VERIFY_QRY_REQ 给 MTA 处理， */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_IMEI_VERIFY_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMEI_VERIFY_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryNCellMonitorPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 给MTA发送^NCELLMONITOR查询请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_NCELL_MONITOR_QRY_REQ, VOS_NULL_PTR,
                                    0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NCELL_MONITOR_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryUserSrvStatePara(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QryUserSrvStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_USER_SRV_STATE_READ;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryRefclkfreqPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询GPS参考时钟状态 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_REFCLKFREQ_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryRefclkfreqPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_REFCLKFREQ_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryHandleDect(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    /* 读取NV项中当前产品形态 */
    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* 非ANDROID系统不支持 */
    if (*systemAppConfig != SYSTEM_APP_ANDROID) {
        return AT_CMD_NOT_SUPPORT;
    }

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 ID_AT_MTA_HANDLEDECT_QRY_REQ 给MTA处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, At_GetOpId(), ID_AT_MTA_HANDLEDECT_QRY_REQ,
                                 VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HANDLEDECT_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryLogNvePara(VOS_UINT8 indexNum)
{
    VOS_UINT32                    result;
    VOS_UINT16                    length;
    VOS_UINT32                    enable;
    VOS_UINT32                    timeout;
    VOS_UINT32                    bufferSize;
    socp_encdst_buf_log_cfg_s logCfg = {0};

    enable     = 0;
    timeout    = 0;
    bufferSize = 0;

    result = AT_GetSocpSdLogCfg(&logCfg);

    if (result != VOS_OK) {
        return AT_ERROR;
    }

    enable = logCfg.log_on_flag;
    /* dts方式获取的内存，不支持延时上报 */
    if ((logCfg.log_on_flag == AT_SOCP_DST_CHAN_DELAY) || (logCfg.log_on_flag == AT_SOCP_DST_CHAN_DTS)) {
        enable = AT_SOCP_DST_CHAN_DELAY;
    }
    timeout    = logCfg.cur_time_out;
    bufferSize = logCfg.buffer_size;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName, enable, timeout,
        bufferSize);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_QryCiregPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_CIREG_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryCiregPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CIREG_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCirepPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_IMSA_CIREP_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryCirepPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CIREP_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryImsRegDomainPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_IMS_REG_DOMAIN_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryImsRegDomainPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSREGDOMAIN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryImsDomainCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MMA 发送查询请求消息 */
    result = TAF_MMA_QryImsDomainCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);

    if (result == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSDOMAINCFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryImsEmcRdpPara(VOS_UINT8 indexNum)
{
    AT_IMS_EmcRdp *imsEmcRdp = VOS_NULL_PTR;
    VOS_CHAR       acIpv4AddrStr[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_UINT8      ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT16     length = 0;

    (VOS_VOID)memset_s(acIpv4AddrStr, sizeof(acIpv4AddrStr), 0x00, sizeof(acIpv4AddrStr));
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));

    /* 获取IMS EMC RDP */
    imsEmcRdp = AT_GetImsEmcRdpByClientId(indexNum);
    if (imsEmcRdp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_QryImsEmcRdpPara: ImsEmcRdp not found.");
        return AT_ERROR;
    }

    /* 检查当前是否有IMS EMC PDN连接 */
    if ((imsEmcRdp->opIPv4PdnInfo != VOS_TRUE) && (imsEmcRdp->opIPv6PdnInfo != VOS_TRUE)) {
        return AT_ERROR;
    }

    if (imsEmcRdp->opIPv4PdnInfo == VOS_TRUE) {
        /* ^IMSEMCRDP: */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        /* IP Type */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", "\"IPV4\"");
        /* IP Addr */
        AT_Ipv4AddrItoa(acIpv4AddrStr, sizeof(acIpv4AddrStr), imsEmcRdp->iPv4PdnInfo.ipAddr);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIpv4AddrStr);

        /* Primary DNS */
        AT_Ipv4AddrItoa(acIpv4AddrStr, sizeof(acIpv4AddrStr), imsEmcRdp->iPv4PdnInfo.dnsPrimAddr);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIpv4AddrStr);

        /* Secondary DNS */
        AT_Ipv4AddrItoa(acIpv4AddrStr, sizeof(acIpv4AddrStr), imsEmcRdp->iPv4PdnInfo.dnsSecAddr);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIpv4AddrStr);

        /* MTU */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", imsEmcRdp->iPv4PdnInfo.mtu);
    }

    if ((imsEmcRdp->opIPv4PdnInfo == VOS_TRUE) && (imsEmcRdp->opIPv6PdnInfo == VOS_TRUE)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    }

    if (imsEmcRdp->opIPv6PdnInfo == VOS_TRUE) {
        /* ^IMSEMCRDP: */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        /* IP Type */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", "\"IPV6\"");

        /* IP Addr */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, imsEmcRdp->iPv6PdnInfo.ipAddr, TAF_IPV6_STR_RFC2373_TOKENS);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", ipv6AddrStr);

        /* Primary DNS */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, imsEmcRdp->iPv6PdnInfo.dnsPrimAddr, TAF_IPV6_STR_RFC2373_TOKENS);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", ipv6AddrStr);

        /* Secondary DNS */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, imsEmcRdp->iPv6PdnInfo.dnsSecAddr, TAF_IPV6_STR_RFC2373_TOKENS);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", ipv6AddrStr);

        /* MTU */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", imsEmcRdp->iPv6PdnInfo.mtu);
    }

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}
#endif


VOS_UINT32 AT_QryClccPara(VOS_UINT8 indexNum)
{
    VOS_UINT32                  ret;
    TAF_Ctrl                    ctrl;
    TAF_CALL_QryCallInfoReqPara qryCallInfoPara;
    ModemIdUint16               modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&qryCallInfoPara, sizeof(qryCallInfoPara), 0x00, sizeof(qryCallInfoPara));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    qryCallInfoPara.getCallInfoType = TAF_CALL_GET_CALL_INFO_TYPE_CLCC;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发消息到C核获取当前所有通话信息 */
    ret = TAF_CCM_CallCommonReq(&ctrl, &qryCallInfoPara, ID_TAF_CCM_QRY_CALL_INFO_REQ, sizeof(qryCallInfoPara),
                                modemId);

    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_QryClccPara: MN_CALL_GetCallInfos fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLCC_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_QryClccEconfInfo(VOS_UINT8 indexNum)
{
    TAF_Ctrl      ctrl;
    VOS_UINT32    rst;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(TAF_Ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* AT向CCM发送增强型多方通话查询消息 */
    rst = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_QRY_ECONF_CALLED_INFO_REQ, 0, modemId);

    if (rst != VOS_OK) {
        AT_WARN_LOG("AT_QryClccEconfInfo: TAF_CCM_CallCommonReq fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLCCECONF_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryEconfErrPara(VOS_UINT8 indexNum)
{
    VOS_UINT16     i;
    VOS_UINT16     length;
    AT_ModemCcCtx *ccCtx     = VOS_NULL_PTR;
    AT_EconfInfo  *econfInfo = VOS_NULL_PTR;
    VOS_UINT8      asciiNum[MN_CALL_MAX_CALLED_ASCII_NUM_LEN + 1];
    VOS_UINT8      numOfCalls;
    VOS_UINT8      errNum;

    errNum     = 0;
    length     = 0;
    ccCtx      = AT_GetModemCcCtxAddrFromClientId(indexNum);
    econfInfo  = &(ccCtx->econfInfo);
    numOfCalls = econfInfo->numOfCalls;
    (VOS_VOID)memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));

    for (i = 0; ((i < numOfCalls) && (i < TAF_CALL_MAX_ECONF_CALLED_NUM)); i++) {
        /* 查询错误原因值 */
        if ((econfInfo->callInfo[i].callNumber.numLen != 0) && (econfInfo->callInfo[i].cause != TAF_CS_CAUSE_SUCCESS)) {
            /* <CR><LF> */
            if (errNum != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            AT_BcdNumberToAscii(econfInfo->callInfo[i].callNumber.bcdNum, econfInfo->callInfo[i].callNumber.numLen,
                                (VOS_CHAR *)asciiNum);

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: \"%s\",%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, asciiNum,
                (econfInfo->callInfo[i].callNumber.numType | AT_NUMBER_TYPE_EXT), econfInfo->callInfo[i].cause);

            errNum++;
        }
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryImsSwitchPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MMA 发送查询请求消息 */
    rst = TAF_MMA_QryImsSwitchReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMS_SWITCH_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCevdpPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MMA 发送查询请求消息 */
    rst = TAF_MMA_QryVoiceDomainReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VOICE_DOMAIN_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryRoamImsServicePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_IMSA_ROAMING_IMS_QRY_REQ, VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryRoamImsServicePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 查询结束挂起通道 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ROAM_IMS_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#endif


VOS_UINT32 AT_QryUserCfgOPlmnPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送消息 ID_AT_MTA_HANDLEDECT_QRY_REQ 给MTA处理 */
    rst = TAF_MMA_QryEOPlmnReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (rst == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EOPLMN_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

#if (FEATURE_AT_HSUART == FEATURE_ON)

VOS_UINT32 AT_QryIprPara(VOS_UINT8 indexNum)
{
    AT_UartCtx *uartCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    /* 初始化 */
    uartCtx = AT_GetUartCtxAddr();
    length  = 0;

    /* 通道检查 */
    if ((AT_CheckHsUartUser(indexNum) != VOS_TRUE) &&
        (AT_CheckUartUser(indexNum) != VOS_TRUE)) {
        return AT_OK;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %u", g_parseContext[indexNum].cmdElement->cmdName,
        uartCtx->phyConfig.baudRate);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryIcfPara(VOS_UINT8 indexNum)
{
    AT_UartCtx *uartCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    /* 初始化 */
    uartCtx = AT_GetUartCtxAddr();
    length  = 0;

    /* 通道检查 */
    if (AT_CheckHsUartUser(indexNum) != VOS_TRUE) {
        return AT_OK;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        uartCtx->phyConfig.frame.format, uartCtx->phyConfig.frame.parity);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryIfcPara(VOS_UINT8 indexNum)
{
    AT_UartCtx *uartCtx = VOS_NULL_PTR;
    VOS_UINT16  length;

    /* 初始化 */
    uartCtx = AT_GetUartCtxAddr();
    length  = 0;

    /* 通道检查 */
    if (AT_CheckHsUartUser(indexNum) != VOS_TRUE) {
        return AT_OK;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        uartCtx->flowCtrl.dceByDte, uartCtx->flowCtrl.dteByDce);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}
#endif



VOS_UINT32 AT_QryModemStatusPara(VOS_UINT8 indexNum)
{
    return AT_OK;
}


VOS_UINT32 AT_QryRATCombinePara(VOS_UINT8 indexNum)
{
    TAF_NV_LC_CTRL_PARA_STRU nVData;
    VOS_UINT16               length;
    VOS_UINT8                ratMode;
    ModemIdUint16            modemId;

    (VOS_VOID)memset_s(&nVData, sizeof(nVData), 0x00, sizeof(nVData));

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_QryRATCombinePara: Get modem id fail.");

        return AT_ERROR;
    }

    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_LC_CTRL_PARA, &nVData, sizeof(TAF_NV_LC_CTRL_PARA_STRU)) != NV_OK) {
        AT_ERR_LOG("AT_QryRATCombinePara: Get NV_ITEM_LC_CTRL_PARA id fail.");

        return AT_ERROR;
    }

    if (nVData.ucLCEnableFlg != VOS_TRUE) {
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    if (nVData.enRatCombined == TAF_NVIM_LC_RAT_COMBINED_CL) {
        ratMode = 0;
    } else if (nVData.enRatCombined == TAF_NVIM_LC_RAT_COMBINED_GUL) {
        ratMode = 1;
    } else {
        return AT_ERROR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ratMode);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryMipiClkValue(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* AT 给MTA 发送查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_MIPICLK_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMipiClkValue: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPI_CLK_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryBestFreqPara(VOS_UINT8 indexNum)
{
    VOS_UINT32    result;
    VOS_UINT32    rslt;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_BUTT;
    rslt    = AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId);

    /* 如果ModemID获取失败或不在Modem0，返回失败 */
    if ((modemId != MODEM_ID_0) || (rslt != VOS_OK)) {
        AT_WARN_LOG("AT_QryBestFreqPara: Not  Modem0 or fail to get current ModemID!");
        return AT_ERROR;
    }

    /* AT 给MTA发送查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_BESTFREQ_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryBestFreqPara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BESTFREQ_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

VOS_VOID Show_Time(ModemIdUint16 modemId)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    PS_PRINTF_INFO("Time:\n");
    PS_PRINTF_INFO("ucIeFlg: %d\n", netCtx->timeInfo.ieFlg);
    PS_PRINTF_INFO("cLocalTimeZone: %d\n", netCtx->timeInfo.localTimeZone);
    PS_PRINTF_INFO("ucDST: %d\n", netCtx->timeInfo.dst);
#if ((!defined(CHOOSE_MODEM_USER)) && (FEATURE_DEBUG_PRINT_ADDRESS == FEATURE_ON))
    PS_PRINTF_INFO("ucLSAID: %p\n", netCtx->timeInfo.lsaid);
#endif
    PS_PRINTF_INFO("cTimeZone: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone);
    PS_PRINTF_INFO("ucYear: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.year);
    PS_PRINTF_INFO("ucMonth: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.month);
    PS_PRINTF_INFO("ucDay: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.day);
    PS_PRINTF_INFO("ucHour: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.hour);
    PS_PRINTF_INFO("ucMinute: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.minute);
    PS_PRINTF_INFO("ucSecond: %d\n", netCtx->timeInfo.universalTimeandLocalTimeZone.second);

    return;
}

VOS_UINT32 AT_QryCclkPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT32      rslt;
    ModemIdUint16   modemId;
    TIME_ZONE_Time  time;

    modemId = MODEM_ID_0;
    (VOS_VOID)memset_s(&time, sizeof(time), 0x00, sizeof(time));

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_QryCclkPara: Get modem id fail.");
        return AT_ERROR;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if ((netCtx->timeInfo.ieFlg & NAS_MM_INFO_IE_UTLTZ) != NAS_MM_INFO_IE_UTLTZ) {
        /* AT 给MTA发送查询请求消息 */
        rslt = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_CCLK_QRY_REQ, VOS_NULL_PTR, 0,
                                      I0_UEPS_PID_MTA);

        if (rslt != TAF_SUCCESS) {
            AT_WARN_LOG("AT_QryCclkPara: send Msg fail.");
            return AT_ERROR;
        }

        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCLK_QRY;

        return AT_WAIT_ASYNC_RETURN;
    }

    /* 获得时区 */
    time.timeZone = netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone;

    /* 若已经获得 Local time zone,则时区修改为 Local time zone */
    if ((netCtx->timeInfo.ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ) {
        time.timeZone = netCtx->timeInfo.localTimeZone;
    }

    time.year   = netCtx->timeInfo.universalTimeandLocalTimeZone.year;
    time.month  = netCtx->timeInfo.universalTimeandLocalTimeZone.month;
    time.day    = netCtx->timeInfo.universalTimeandLocalTimeZone.day;
    time.hour   = netCtx->timeInfo.universalTimeandLocalTimeZone.hour;
    time.minute = netCtx->timeInfo.universalTimeandLocalTimeZone.minute;
    time.second = netCtx->timeInfo.universalTimeandLocalTimeZone.second;

    AT_PrintCclkTime(indexNum, &time, modemId);

    return AT_OK;
}

#if (FEATURE_HUAWEI_VP == FEATURE_ON)

VOS_UINT32 AT_QryVoicePreferPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rslt;

    /* 调用MN_FillAndSndAppReqMsg()，等待RABM的回复 */
    rslt = AT_SndQryVoicePrefer(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (rslt == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VOICEPREFER_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryCLocInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 向MMA发消息请求运营商信息 */
    ret = TAF_MMA_QryCLocInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLOCINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 AT_QryEmcCallBack(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ret = TAF_MMA_QryCurrEmcCallBackMode(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMCCBM_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))


VOS_UINT32 AT_QryEncryptCallCap(VOS_UINT8 indexNum)
{
    VOS_UINT32    ret;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送ID_TAF_CCM_ECC_SRV_CAP_QRY_REQ消息 */
    ret = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_ECC_SRV_CAP_QRY_REQ, 0, modemId);

    if (ret != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECCAP_QRY;

    return AT_WAIT_ASYNC_RETURN;
}
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)


VOS_UINT32 AT_QryEncryptCallRandom(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送ID_TAF_CCM_GET_EC_RANDOM_REQ消息 */
    ret = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_GET_EC_RANDOM_REQ, 0, modemId);

    if (ret != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECRANDOM_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

VOS_UINT32 AT_QryEncryptCallKmc(VOS_UINT8 indexNum)
{
    VOS_UINT32    ret;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送ID_TAF_CCM_GET_EC_KMC_REQ消息 */
    ret = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_GET_EC_KMC_REQ, 0, modemId);
    if (ret != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECKMC_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

VOS_UINT32 AT_QryEccTestMode(VOS_UINT8 indexNum)
{
    VOS_UINT32    ret;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送ID_TAF_CCM_GET_EC_TEST_MODE_REQ消息 */
    ret = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_GET_EC_TEST_MODE_REQ, 0, modemId);

    if (ret != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECCTEST_QRY;

    return AT_WAIT_ASYNC_RETURN;
}
#endif
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryPrivacyModePreferred(VOS_UINT8 indexNum)
{
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 检查当前命令操作类型 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送ID_TAF_CCM_PRIVACY_MODE_QRY_REQ消息 */
    if (TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_PRIVACY_MODE_QRY_REQ, 0, modemId) == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPMP_QRY;

        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_QryCFreqLockInfo(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcCFreqLockQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CFREQLOCK_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryCdmaCsqPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcCdmaCsqQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CDMACSQ_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}
#endif


VOS_UINT32 AT_QryTTYModePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给VC 发送模式查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    APP_VC_MSG_QRY_TTYMODE_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_VC);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryTTYModePara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TTYMODE_READ;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCtaPara(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 执行命令操作 */
    if (TAF_PS_GetCtaInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CTA_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;

#else
    return AT_ERROR;
#endif
}



VOS_UINT32 At_QryRatRfSwitch(VOS_UINT8 indexNum)
{
    NAS_NvTriModeEnable       triModeEnableStru;
    NAS_NvTriModeFemProfileId triModeFemProfileIdStru;

    (VOS_VOID)memset_s(&triModeEnableStru, sizeof(triModeEnableStru), 0x00, sizeof(triModeEnableStru));
    (VOS_VOID)memset_s(&triModeFemProfileIdStru, sizeof(triModeFemProfileIdStru), 0x00,
            sizeof(triModeFemProfileIdStru));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 读取NV项 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_TRI_MODE_ENABLE, &triModeEnableStru, sizeof(triModeEnableStru)) !=
        NV_OK) {
        AT_WARN_LOG("At_QryRatRfSwitch:read en_NV_Item_TRI_MODE_ENABLE failed");
        return AT_ERROR;
    }

    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_TRI_MODE_FEM_PROFILE_ID, &triModeFemProfileIdStru,
                          sizeof(triModeFemProfileIdStru)) != NV_OK) {
        AT_WARN_LOG("At_QryRatRfSwitch:read en_NV_Item_TRI_MODE_FEM_PROFILE_ID failed");
        return AT_ERROR;
    }

    /* 输出 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%u", g_parseContext[indexNum].cmdElement->cmdName,
        triModeEnableStru.enable, triModeFemProfileIdStru.profileId);
    return AT_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_Qry1xChanPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_Proc1xChanQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_1XCHAN_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}
#endif
#endif


VOS_UINT32 AT_QryProRevInUse(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcProRevInUseQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CVER_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}


VOS_UINT32 AT_QryCasState(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcStateQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GETSTA_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryProGetEsn(VOS_UINT8 indexNum)
{
    NV_ESN_Meid esnMeId;
    VOS_INT32   loop;
    VOS_UINT32  esn;

    esn = 0;
    (VOS_VOID)memset_s(&esnMeId, sizeof(esnMeId), 0x00, sizeof(NV_ESN_Meid));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 读取en_NV_Item_ESN_MEID */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_ESN_MEID, &esnMeId, sizeof(NV_ESN_Meid)) != NV_OK) {
        AT_WARN_LOG("AT_QryProGetEsn:Read Nvim Failed");
        return AT_ERROR;
    }

    /* 若未使能，回复失败还是填0 ??? */
    if ((esnMeId.esnMeIDFlag == ESN_ME_ENABLE_FLAG) || (esnMeId.esnMeIDFlag == ESN_MEID_ENABLE_BOTH)) {
        /* Read from high to low */
        for (loop = PS_MIN(esnMeId.esn[0], sizeof(esnMeId.esn) - 1); loop > 0; loop--) {
            /* Update the current value, 手机ESN内容，第一个字节表示长度信息，是Uint32类型，所以乘8 */
            esn <<= sizeof(esnMeId.esn[0]) * 8;

            /* Add lower bits */
            esn |= esnMeId.esn[loop];
        }

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, esn);

    } else {
        AT_WARN_LOG("AT_QryProGetEsn: ESN is disabled");
    }

    return AT_OK;
}


VOS_UINT32 AT_QryProGetMeid(VOS_UINT8 indexNum)
{
    NV_ESN_Meid esnMeId;
    VOS_UINT8   meId[2 * AT_MEID_OCTET_NUM + 1];

    (VOS_VOID)memset_s(&esnMeId, sizeof(esnMeId), 0x00, sizeof(NV_ESN_Meid));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 读取en_NV_Item_ESN_MEID */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_ESN_MEID, &esnMeId, sizeof(NV_ESN_Meid)) != NV_OK) {
        AT_WARN_LOG("AT_QryProGetMeid:Read Nvim Failed");
        return AT_ERROR;
    }

    /* 若未使能，回复失败还是填0 ??? */
    if ((esnMeId.esnMeIDFlag == MEID_ME_ENABLE_FLAG) || (esnMeId.esnMeIDFlag == ESN_MEID_ENABLE_BOTH)) {
        AT_Hex2Ascii_Revers(&(esnMeId.meId[1]), AT_MEID_OCTET_NUM, meId);
        meId[2 * AT_MEID_OCTET_NUM] = '\0';

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s%s", g_parseContext[indexNum].cmdElement->cmdName, meId, g_atCrLf);

    } else {
        AT_WARN_LOG("AT_QryProGetMeid: MEID is disabled");
    }

    return AT_OK;
}
#endif


VOS_UINT32 AT_QryHighVer(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcCHVerQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CHIGHVER_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}


VOS_UINT32 AT_QryTransModePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_TRANSMODE_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryTransModePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TRANSMODE_READ;

    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryCdmaDormTimerVal(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 执行命令操作 */
    if (TAF_PS_ProcCdmaDormTimerQryReq(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DORMTIMER_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

#endif


VOS_UINT32 AT_QryMccFreqPara(VOS_UINT8 indexNum)
{
    AT_CSS_MccVersionInfoReq *msg = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG("AT_QryMccFreqPara: Invalid Cmd Type");

        return AT_ERROR;
    }

    /* Allocating memory for message */
    /*lint -save -e516 */
    msg = (AT_CSS_MccVersionInfoReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_CSS_MccVersionInfoReq));
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_QryMccFreqPara: Mem Alloc failed");

        return AT_ERROR;
    }

    (VOS_VOID)memset_s(((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH),
                       sizeof(AT_CSS_MccVersionInfoReq) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_CSS_MccVersionInfoReq) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头 */
    AT_CFG_MSG_HDR(msg, PS_PID_CSS, ID_AT_CSS_MCC_VERSION_INFO_REQ);

    msg->clientId = g_atClientTab[indexNum].clientId;

    AT_SEND_MSG(msg);

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MCCFREQ_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryBlockCellListPara(VOS_UINT8 indexNum)
{
    AT_CSS_BlockCellListQueryReq *msg = VOS_NULL_PTR;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        AT_ERR_LOG("AT_QryBlockCellListPara: Invalid Cmd Type");

        return AT_ERROR;
    }

    /* Allocating memory for message */
    /*lint -save -e516 */
    msg = (AT_CSS_BlockCellListQueryReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_CSS_BlockCellListQueryReq));
    /*lint -restore */

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_QryBlockCellListPara: Mem Alloc failed");

        return AT_ERROR;
    }

    (VOS_VOID)memset_s(((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH),
                       sizeof(AT_CSS_BlockCellListQueryReq) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_CSS_BlockCellListQueryReq) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头 */
    AT_CFG_MSG_HDR(msg, PS_PID_CSS, ID_AT_CSS_BLOCK_CELL_LIST_QUERY_REQ);

    msg->clientId = g_atClientTab[indexNum].clientId;

    AT_SEND_MSG(msg);

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BLOCKCELLLIST_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryUECenterPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_UE_CENTER_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryUEModePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UE_CENTER_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryHdrCsqPara(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_ProcHdrCsqQryReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HDR_CSQ_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;

#else
    return AT_ERROR;
#endif
}


VOS_UINT32 AT_QryCurrSidNid(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = TAF_MMA_QryCurrSidNid(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSNID_QRY;

    return AT_WAIT_ASYNC_RETURN;
#else
    return AT_ERROR;
#endif
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryCtRoamInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = TAF_MMA_QryCtRoamInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CTROAMINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

VOS_UINT32 AT_QryPRLID(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = TAF_MMA_QryPrlIdInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PRLID_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#endif


VOS_UINT32 AT_QryAfcClkInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_QRY_AFC_CLK_FREQ_XOCOEF_REQ,
                                 VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_AFCCLKINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryFratIgnitionInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_FRAT_IGNITION_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryFratIgnitionInfo: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FRATIGNITION_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryPacspPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_MMA_QryPacspReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PACSP_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_QryNoCardMode(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_NO_CARD_MODE_QRY_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNoCardMode: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NOCARDMODE_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryRatCombinedMode(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = TAF_MMA_QryRatCombinedMode(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0);
    if (ret != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RATCOMBINEDMODE_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#endif


VOS_UINT32 AT_QryFclassPara(VOS_UINT8 indexNum)
{
    AT_WARN_LOG("AT_QryFclassPara: Not support this command!");
    (VOS_VOID)indexNum;
    return AT_ERROR;
}


VOS_UINT32 AT_QryGciPara(VOS_UINT8 indexNum)
{
    AT_WARN_LOG("AT_QryGciPara: Not support this command!");
    (VOS_VOID)indexNum;
    return AT_ERROR;
}


VOS_UINT32 AT_QryRsrp(VOS_UINT8 indexNum)
{
    VOS_UINT32          ret;
    AT_MTA_RsInfoQryReq rsInfoQryReq;

    rsInfoQryReq.rsInfoType = AT_MTA_RSRP_TYPE;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_RS_INFO_QRY_REQ,
                                 (VOS_UINT8 *)&rsInfoQryReq, (VOS_UINT32)sizeof(AT_MTA_RsInfoQryReq), I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RSRP_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryRsrq(VOS_UINT8 indexNum)
{
    VOS_UINT32          ret;
    AT_MTA_RsInfoQryReq rsInfoQryReq;

    rsInfoQryReq.rsInfoType = AT_MTA_RSRQ_TYPE;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_RS_INFO_QRY_REQ,
                                 (VOS_UINT8 *)&rsInfoQryReq, (VOS_UINT32)sizeof(AT_MTA_RsInfoQryReq), I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RSRQ_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCsdfPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        netCtx->csdfCfg.mode, netCtx->csdfCfg.auxMode);

    return AT_OK;
}


VOS_UINT32 AT_QryRejInfoPara(VOS_UINT8 indexNum)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (TAF_MMA_QryRejinfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_REJINFO_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryLteAttachInfoPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetLteAttchInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId),
                               g_atClientTab[indexNum].opId) != VOS_OK) {
        AT_WARN_LOG("AT_QryLteAttachInfoPara: TAF_PS_GetLteAttchInfo fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEATTACHINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)

TAF_UINT32 At_QryCmuxPara(TAF_UINT8 indexNum)
{
    cmux_info_type* cmux_info = VOS_NULL_PTR;
    VOS_UINT16      len = 0;

    /* 检查通道，只支持UART端口下发 */
    if ((AT_CMUX_CheckUartUser(indexNum) != VOS_TRUE) && (AT_CheckCmuxUser(indexNum)) != VOS_TRUE) {
        return AT_ERROR;
    }
    cmux_info = AT_GetCmuxInfo();
    len = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d,%d,%d,%d,%d,%d",
                                g_parseContext[indexNum].cmdElement->cmdName,
                                cmux_info->operating_mode, cmux_info->subset,
                                cmux_info->port_speed, cmux_info->max_frame_size_N1,
                                cmux_info->response_timer_T1 / T1_T2_FACTOR, cmux_info->max_cmd_num_tx_times_N2,
                                cmux_info->response_timer_T2 / T1_T2_FACTOR, cmux_info->response_timer_T3,
                                cmux_info->window_size_k);

    g_atSendDataBuff.bufLen = len;

    return AT_OK;
}
#endif


VOS_UINT32 AT_QrySinglePdnSwitch(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetSinglePdnSwitch(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        AT_WARN_LOG("AT_QrySinglePdnSwitch: TAF_PS_GetSinglePdnSwitch fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SINGLEPDNSWITCH_QRY;
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
/*
 * 功能描述: 查询无卡模式
 * 修改历史:
 *  1.日    期: 2020年05月21日
 *    修改内容: 新生成函数
 */
VOS_UINT32 AT_QueryNoCard(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    AT_USIM_InfoCtx *usimInfoCtx = VOS_NULL_PTR;
    ModemIdUint16 modemId = MODEM_ID_0;

    result = AT_GetModemIdFromClient(indexNum, &modemId);
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_QueryNoCard:Get Modem Id fail!");
        return AT_ERROR;
    }

    /* 获取当前NOCARD的状态 */
    usimInfoCtx = AT_GetUsimInfoCtxFromModemId(modemId);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        usimInfoCtx->noCardMode);
    return AT_OK;
}
#endif
#endif

