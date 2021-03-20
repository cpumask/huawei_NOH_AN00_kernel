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

#include "AtCmdPacketProc.h"
#include "securec.h"

#include "AtTafAgentInterface.h"

#include "product_config.h"
#include "mn_comm_api.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_PACKET_PROC_C

#define AT_CGPIAF_MAX_PARA_NUM 4
#define AT_CGPIAF_IPV6_ADDRFORMAT 0
#define AT_CGPIAF_IPV6_SUBNETNOTATION 1
#define AT_CGPIAF_IPV6_LEADINGZEROS 2
#define AT_CGPIAF_IPV6_COMPRESSZEROS 3


#define AT_APDSFLOWRPTCFG_PARA_MAX_NUM 4
#define AT_APDSFLOWRPTCFG_PARA_MIN_NUM 1

#define AT_CHDATA_PARA_MAX_NUM 2
#define AT_DSFLOWNVWRCFG_PARA_MAX_NUM 2
#define AT_CGEREP_PARA_MAX_NUM 2
#define AT_ROAMPDPTYPEPARA_PARA_MAX_NUM 2
#define AT_IMSPDPCFG_PARA_NUM 2
#define AT_USBTETHERINFO_PARA_NUM 2
#define AT_RMNETCFG_PARA_NUM 3
#define AT_TOKENS_BIT_LEN 16
#define AT_EMC_PDN_STAT_PAPA_VALUE 1


AT_APP_ConnStateUint32 AT_AppConvertPdpStateToConnStatus(TAF_IFACE_StateUint8 pdpState)
{
    AT_APP_ConnStateUint32 connStatus;

    switch (pdpState) {
        case TAF_IFACE_STATE_ACTING:
            connStatus = AT_APP_DIALING;
            break;

        case TAF_IFACE_STATE_ACTED:
        case TAF_IFACE_STATE_DEACTING:
            connStatus = AT_APP_DIALED;
            break;

        case TAF_IFACE_STATE_IDLE:
        default:
            connStatus = AT_APP_UNDIALED;
            break;
    }

    return connStatus;
}


TAF_IFACE_DynamicInfo* AT_GetDynamicInfoBaseAddr(TAF_IFACE_GetDynamicParaCnf *evt)
{
    return &(evt->dynamicInfo[0]);
}


VOS_UINT32 AT_GetCidByCidMask(VOS_UINT32 cidMask, VOS_UINT8 *cid)
{
    VOS_UINT32 cidTemp;

    for (cidTemp = 1; cidTemp <= TAF_MAX_CID_NV; cidTemp++) {
        /* 当前Bit位为1，则此位为对应的Cid */
        if ((cidMask & ((VOS_UINT32)0x00000001 << cidTemp)) != 0) {
            *cid = (VOS_UINT8)cidTemp;
            return VOS_OK;
        }
    }

    *cid = TAF_INVALID_CID;

    return VOS_ERR;
}


VOS_UINT8 AT_GetUserIndexByClientIndex(VOS_UINT8 clientIndex)
{
    VOS_UINT8 userIndex;

    userIndex = clientIndex;

    /* PCUI口且已设置PCUI口模拟NDISDUP拨号 */
    if (g_atClientTab[clientIndex].userType == AT_USBCOM_USER) {
        if (AT_GetPcuiPsCallFlag() == VOS_TRUE) {
            userIndex = AT_GetPcuiUsertId();
        }
    }

    /* CTRL口且已设置CTRL口模拟NDISDUP拨号 */
    if (g_atClientTab[clientIndex].userType == AT_CTR_USER) {
        if (AT_GetCtrlPsCallFlag() == VOS_TRUE) {
            userIndex = AT_GetCtrlUserId();
        }
    }

    /* PCUI2口且已设置PCUI2口模拟NDISDUP拨号 */
    if (g_atClientTab[clientIndex].userType == AT_PCUI2_USER) {
        if (AT_GetPcui2PsCallFlag() == VOS_TRUE) {
            userIndex = AT_GetPcui2UserId();
        }
    }

    return userIndex;
}


VOS_UINT32 AT_SetChdataPara_AppUser(VOS_UINT8 indexNum)
{
    AT_CommPsCtx       *psCtx             = VOS_NULL_PTR;
    AT_PS_DataChanlCfg *dataChanCfg       = VOS_NULL_PTR;
    CONST AT_ChdataRnicRmnetId *chDataCfg = VOS_NULL_PTR;
    AT_CH_DataChannelUint32     dataChannelId;
    ModemIdUint16               modemId;
    VOS_UINT8                   cid;

    modemId = MODEM_ID_0;
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_WARN_LOG("AT_SetChdataPara_AppUser: get modem id error!");
        return AT_ERROR;
    }

    cid         = (VOS_UINT8)g_atParaList[0].paraValue;
    dataChanCfg = AT_PS_GetDataChanlCfg(indexNum, cid);

    /*
     * 指定CID的PDP若已经激活或者在激活过程中，则不允许删除或修改该CID
     * 的通道映射关系，直接返回ERROR
     */
    if ((dataChanCfg->used == VOS_TRUE) &&
        ((dataChanCfg->ifaceActFlg == VOS_TRUE) || (dataChanCfg->portIndex < AT_CLIENT_ID_BUTT))) {
        AT_WARN_LOG("AT_SetChdataPara_AppUser: already act or acting!");
        return AT_ERROR;
    }

    /* 第二个参数为空，则表示删除配置关系 */
    if (g_atParaList[1].paraLen == 0) {
        AT_CleanDataChannelCfg(dataChanCfg);
        return AT_OK;
    }

    dataChannelId = g_atParaList[1].paraValue;

    /* 获取网卡ID */
    chDataCfg = AT_PS_GetChDataCfgByChannelId(dataChannelId);
    if (chDataCfg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_SetChdataPara_AppUser: get chdata cfg fail!");
        return AT_ERROR;
    }

    psCtx = AT_GetCommPsCtxAddr();

    /* 检查通道是否被使用过 */
    if (psCtx->channelCfg[dataChannelId].used == VOS_TRUE) {
        if (psCtx->channelCfg[dataChannelId].cid != cid) {
            AT_WARN_LOG("AT_SetChdataPara_AppUser: already other cid used!");
            return AT_ERROR;
        }

        if (psCtx->channelCfg[dataChannelId].modemId != modemId) {
            AT_WARN_LOG("AT_SetChdataPara_AppUser: already other modem used!");
            return AT_ERROR;
        }

        return AT_OK;
    }

    /* 删除原来的配置 */
    AT_CleanDataChannelCfg(dataChanCfg);

    /* 配置新的数传通道映射表 */
    psCtx->channelCfg[dataChannelId].used    = VOS_TRUE;
    psCtx->channelCfg[dataChannelId].rmNetId = chDataCfg->rnicRmNetId;
    psCtx->channelCfg[dataChannelId].ifaceId = chDataCfg->ifaceId;
    psCtx->channelCfg[dataChannelId].cid     = cid;
    psCtx->channelCfg[dataChannelId].modemId = modemId;

    return AT_OK;
}


VOS_UINT32 AT_SetChdataPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 userIndex;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_CHDATA_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    userIndex = AT_GetUserIndexByClientIndex(indexNum);

    /* APP通道的处理 */
    if (AT_CheckAppUser(userIndex) == VOS_TRUE) {
        return AT_SetChdataPara_AppUser(userIndex);
    }

    return AT_ERROR;
}


VOS_UINT32 AT_QryChdataPara_AppUser(TAF_UINT8 indexNum)
{
    AT_PS_DataChanlCfg     *dataChanCfg = VOS_NULL_PTR;
    AT_CH_DataChannelUint32 dataChannelId;
    VOS_UINT32              rslt;
    VOS_UINT16              length;
    VOS_UINT8               loop;
    VOS_UINT8               num;

    num    = 0;
    length = 0;

    /* 输出结果 */
    for (loop = 1; loop <= TAF_MAX_CID; loop++) {
        dataChanCfg = AT_PS_GetDataChanlCfg(indexNum, loop);
        if ((dataChanCfg->used == VOS_TRUE) && (dataChanCfg->rmNetId != AT_PS_INVALID_RMNET_ID)) {
            if (num != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            /* ^CHDATA:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "^CHDATA: ");

            /* <cid> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", loop);

            /* <datachannel> */
            rslt = AT_PS_GetChDataValueFromRnicRmNetId((RNIC_DEV_ID_ENUM_UINT8)(dataChanCfg->rmNetId), &dataChannelId);

            if (rslt != VOS_OK) {
                return AT_ERROR;
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", dataChannelId);

            num++;
        }
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_QryChdataPara(TAF_UINT8 indexNum)
{
    VOS_UINT8 userId;

    /* 命令状态类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    userId = AT_GetUserIndexByClientIndex(indexNum);

    /* APP通道的处理 */
    if (AT_CheckAppUser(userId) == VOS_TRUE) {
        return AT_QryChdataPara_AppUser(userId);
    }

    return AT_ERROR;
}


VOS_UINT32 AT_TestChdataPara(VOS_UINT8 indexNum)
{
    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName,
            CHDATA_TEST_CMD_PARA_STRING);

    return AT_OK;
}


VOS_UINT32 At_SetIfaceDynamicParaComCheck(const VOS_UINT8 indexNum, TAF_Ctrl *ctrl)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 构造控制结构体 */
    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, ctrl) == VOS_ERR) {
        return AT_ERROR;
    }

    return VOS_OK;
}


VOS_UINT32 At_QryIfaceDynamicParaComCheck(const VOS_UINT8 indexNum, TAF_Ctrl *ctrl)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 构造控制结构体 */
    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, ctrl) == VOS_ERR) {
        return AT_ERROR;
    }

    return VOS_OK;
}


VOS_UINT32 At_SetDhcpPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("At_SetDhcpPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DHCP_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryDhcpPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("At_QryDhcpPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DHCP_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestDhcpPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetDhcpv6Para(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_SetDhcpv6Para: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DHCPV6_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDhcpv6Para(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_QryDhcpv6Para: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DHCPV6_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestDhcpv6Para(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetApRaInfoPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_SetApRaInfoPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APRAINFO_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryApRaInfoPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_QryApRaInfoPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APRAINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestApRaInfoPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetApLanAddrPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    /* 构造控制结构体 */
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_SetApLanAddrPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APLANADDR_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryApLanAddrPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_QryApLanAddrPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APLANADDR_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestApLanAddrPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetIPv6TempAddrPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    /* 构造控制结构体 */
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    if (AT_GetPsIPv6IIDTestModeConfig() != IPV6_ADDRESS_TEST_MODE_ENABLE) {
        AT_ERR_LOG("AT_PS_ReportIPv6TempAddr: Test mode is disabled!");
        return AT_CME_OPERATION_NOT_ALLOWED;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_SetIPv6TempAddrPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IPV6TEMPADDR_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestIPv6TempAddrPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetCgpiafPara(VOS_UINT8 indexNum)
{
    AT_CommPsCtx *commPsCtx = VOS_NULL_PTR;

    commPsCtx = AT_GetCommPsCtxAddr();

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_CGPIAF_MAX_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* 参数<IPv6_AddressFormat> */
    if (g_atParaList[AT_CGPIAF_IPV6_ADDRFORMAT].paraLen != 0) {
        commPsCtx->opIpv6AddrFormat = (VOS_UINT8)g_atParaList[AT_CGPIAF_IPV6_ADDRFORMAT].paraValue;
    }

    /* 参数<IPv6_SubnetNotation> */
    if (g_atParaList[AT_CGPIAF_IPV6_SUBNETNOTATION].paraLen != 0) {
        commPsCtx->opIpv6SubnetNotation = (VOS_UINT8)g_atParaList[AT_CGPIAF_IPV6_SUBNETNOTATION].paraValue;
    }

    /* 参数<IPv6_LeadingZeros> */
    if (g_atParaList[AT_CGPIAF_IPV6_LEADINGZEROS].paraLen != 0) {
        commPsCtx->opIpv6LeadingZeros = (VOS_UINT8)g_atParaList[AT_CGPIAF_IPV6_LEADINGZEROS].paraValue;
    }

    /* 参数<IPv6_CompressZeros> */
    if (g_atParaList[AT_CGPIAF_IPV6_LEADINGZEROS].paraLen != 0) {
        commPsCtx->opIpv6CompressZeros = (VOS_UINT8)g_atParaList[AT_CGPIAF_IPV6_COMPRESSZEROS].paraValue;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCgpiafPara(VOS_UINT8 indexNum)
{
    AT_CommPsCtx *commPsCtx = VOS_NULL_PTR;

    commPsCtx = AT_GetCommPsCtxAddr();

    /* 命令操作类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 输出查询结果 */
    /* +CGPIAF: <IPv6_AddressFormat>,<IPv6_SubnetNotation>,<IPv6_LeadingZeros>,<IPv6_CompressZeros> */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        commPsCtx->opIpv6AddrFormat, commPsCtx->opIpv6SubnetNotation, commPsCtx->opIpv6LeadingZeros,
        commPsCtx->opIpv6CompressZeros);

    return AT_OK;
}


VOS_UINT32 At_SetApConnStPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    /* 构造控制结构体 */
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("At_SetApConnStPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APCONNST_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryApConnStPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* BitCid 构造 */
    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("At_SetApConnStPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APCONNST_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestApConnStPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-11)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetApDsFlowRptCfgPara(VOS_UINT8 indexNum)
{
    TAF_APDSFLOW_RptCfg rptCfg;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 命令参数个数检查 */
    if ((g_atParaIndex < AT_APDSFLOWRPTCFG_PARA_MIN_NUM) || (g_atParaIndex > AT_APDSFLOWRPTCFG_PARA_MAX_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT^APDSFLOWRPTCFG=, */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<enable> */
    rptCfg.rptEnabled = g_atParaList[0].paraValue;

    if (rptCfg.rptEnabled == VOS_TRUE) {
        /* AT^APDSFLOWRPTCFG等于1 */
        if (g_atParaIndex == 1) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* AT^APDSFLOWRPTCFG=1, */
        if (g_atParaList[1].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* AT^APDSFLOWRPTCFG=1,<threshold> */
        rptCfg.fluxThreshold = g_atParaList[1].paraValue;

        /* AT^APDSFLOWRPTCFG等于1,0 */
        if (rptCfg.fluxThreshold == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
        /* AT^APDSFLOWRPTCFG等于0 */
        rptCfg.fluxThreshold = 0;
    }

    /* 执行命令操作 */
    if (TAF_PS_SetApDsFlowRptCfg(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0, &rptCfg) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APDSFLOWRPTCFG_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryApDsFlowRptCfgPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetApDsFlowRptCfg(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_APDSFLOWRPTCFG_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetDsFlowNvWriteCfgPara(VOS_UINT8 indexNum)
{
    TAF_DSFLOW_NvWriteCfg writeNvCfg;

    (VOS_VOID)memset_s(&writeNvCfg, sizeof(writeNvCfg), 0x00, sizeof(writeNvCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 命令参数个数检查 */
    if (g_atParaIndex > AT_DSFLOWNVWRCFG_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT^DSFLOWNVWRCFG= */
    if (g_atParaIndex == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* AT^DSFLOWNVWRCFG=,<interval> */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 获取<enable> */
    writeNvCfg.enabled = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 获取<interval> */
    if (g_atParaIndex > 1) {
        if (g_atParaList[1].paraLen != 0) {
            /* AT^DSFLOWNVWRCFG=<enable>,<interval> */
            writeNvCfg.interval = (VOS_UINT8)g_atParaList[1].paraValue;
        } else {
            /* AT^DSFLOWNVWRCFG=<enable>, */
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
        /* AT^DSFLOWNVWRCFG=<enable> */
        writeNvCfg.interval = TAF_DEFAULT_DSFLOW_NV_WR_INTERVAL;
    }

    /* AT^DSFLOWNVWRCFG等于1,0 */
    if ((writeNvCfg.enabled == VOS_TRUE) && (writeNvCfg.interval == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    if (TAF_PS_SetDsFlowNvWriteCfg(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0,
                                   &writeNvCfg) != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DSFLOWNVWRCFG_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDsFlowNvWriteCfgPara(VOS_UINT8 indexNum)
{
    /* 执行命令操作 */
    if (TAF_PS_GetDsFlowNvWriteCfg(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DSFLOWNVWRCFG_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_SetImsPdpCfg(VOS_UINT8 indexNum)
{
    TAF_IMS_PdpCfg imsPdpCfg;

    /* 参数过多 */
    if (g_atParaIndex != AT_IMSPDPCFG_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&imsPdpCfg, sizeof(imsPdpCfg), 0x00, sizeof(imsPdpCfg));

    /* 参数赋值 */
    imsPdpCfg.cid     = (VOS_UINT8)g_atParaList[0].paraValue;
    imsPdpCfg.imsFlag = (VOS_UINT8)g_atParaList[1].paraValue;

    /* 发送跨核消息 */
    if (TAF_PS_SetImsPdpCfg(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0, &imsPdpCfg) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_IMSPDPCFG_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}
#endif



VOS_UINT32 AT_SetCgerepPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_CGEREP_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* +CGEREP=[<mode>[,<bfr>]] */
    if (g_atParaIndex == 0) {
        netCtx->cgerepCfg.mode = 0;
        netCtx->cgerepCfg.bfr  = 0;
    } else if (g_atParaIndex == 1) {
        if (g_atParaList[0].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        netCtx->cgerepCfg.mode = (VOS_UINT8)g_atParaList[0].paraValue;
        netCtx->cgerepCfg.bfr  = 0;
    } else {
        if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        netCtx->cgerepCfg.mode = (VOS_UINT8)g_atParaList[0].paraValue;
        netCtx->cgerepCfg.bfr  = (VOS_UINT8)g_atParaList[1].paraValue;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCgerepPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        netCtx->cgerepCfg.mode, netCtx->cgerepCfg.bfr);

    return AT_OK;
}


VOS_UINT32 AT_SetCindPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx   *netCtx = VOS_NULL_PTR;
    TAF_START_InfoInd startInfoInd;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);
    (VOS_VOID)memset_s(&startInfoInd, sizeof(startInfoInd), 0x00, sizeof(startInfoInd));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex == 0) {
        return AT_OK;
    }

    /* +CIND=[<ind>] 目前只支持signal的设置 */
    if (g_atParaList[0].paraLen != 0) {
        netCtx->cerssiReportType = (AT_CerssiReportTypeUint8)g_atParaList[0].paraValue;

        startInfoInd.actionType          = TAF_START_EVENT_INFO_FOREVER;
        startInfoInd.signThreshold       = (VOS_UINT8)g_atParaList[0].paraValue;
        startInfoInd.rrcMsgType          = TAF_EVENT_INFO_CELL_SIGN;
        startInfoInd.minRptTimerInterval = netCtx->cerssiMinTimerInterval;

        if (TAF_MMA_SetCindReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &startInfoInd) == VOS_TRUE) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CIND_SET;
            return AT_WAIT_ASYNC_RETURN;
        } else {
            return AT_ERROR;
        }
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCindPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* +CIND: <ind> 目前只支持signal的查询 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            netCtx->cerssiReportType);

    return AT_OK;
}


VOS_UINT32 AT_TestCindPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: \"signal\",(0-5)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}
#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_SetEmcStatus(VOS_UINT8 indexNum)
{
    VOS_UINT32      ret;
    VOS_UINT16      msgType;
    VOS_UINT8       emcPdnAt;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetEmcPdn: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_CMD_PARA_INDEX_ONE) {
        AT_WARN_LOG("AT_SetEmcPdn: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数长度检查 */
    if (g_atParaList[0].paraLen != AT_CMD_PARA_LEN_ONE) {
        AT_WARN_LOG("AT_SetEmcPdn: paraLen  is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    emcPdnAt = (VOS_UINT8)g_atParaList[0].paraValue;

    msgType = (emcPdnAt == AT_EMC_PDN_STAT_PAPA_VALUE) ? ID_AT_IMSA_EMC_PDN_ACTIVATE_REQ :
        ID_AT_IMSA_EMC_PDN_DEACTIVATE_REQ;

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, msgType,
        VOS_NULL_PTR, 0, PS_PID_IMSA);

    if (ret != VOS_OK)
    {
        AT_WARN_LOG("AT_SetEmcPdn:AT_FillAndSndAppReqMsg fail");
        return AT_ERROR;
    }

    return AT_OK;
}
#endif


VOS_UINT32 AT_SetDataSwitchStatus(VOS_UINT8 indexNum)
{
    VOS_UINT8 dataSwitchAT;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetDataSwitchStatus: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetDataSwitchStatus: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen != 1) {
        AT_WARN_LOG("AT_SetDataSwitchStatus: usParaLen  is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    dataSwitchAT = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 给TAF APS发送通知消息 */
    if (TAF_PS_Set_DataSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0,
                                    dataSwitchAT) != VOS_OK) {
        AT_WARN_LOG("AT_SetDataSwitchStatus():TAF_PS_Set_DataSwitchStatus fail");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATASWITCH_SET;
    return AT_WAIT_ASYNC_RETURN;
}

VOS_UINT32 AT_SetDataRoamSwitchStatus(VOS_UINT8 indexNum)
{
    VOS_UINT8 dataRoamSwitchAT;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetDataRoamSwitchStatus: NOT AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetDataRoamSwitchStatus: para num is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen != 1) {
        AT_WARN_LOG("AT_SetDataRoamSwitchStatus: usParaLen  is not equal 1!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    dataRoamSwitchAT = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 给TAF APS发送通知消息 */
    if (TAF_PS_Set_DataRoamSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0,
                                        dataRoamSwitchAT) != VOS_OK) {
        AT_WARN_LOG("AT_SetDataRoamSwitchStatus():TAF_PS_Set_DataRoamSwitchStatus fail");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATAROAMSWITCH_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDataSwitchStatus(VOS_UINT8 indexNum)
{
    /* 发送跨核消息 */
    if (TAF_PS_Get_DataSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATASWITCH_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryDataRoamSwitchStatus(VOS_UINT8 indexNum)
{
    /* 发送跨核消息 */
    if (TAF_PS_Get_DataRoamSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0) !=
        VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATAROAMSWITCH_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetUsbTetherInfo(VOS_UINT8 indexNum)
{
    AT_RNIC_UsbTetherInfoInd *msg = VOS_NULL_PTR;
    errno_t                   memResult;

    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数目错误 */
    if (g_atParaIndex != AT_USBTETHERINFO_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0) ||
        (g_atParaList[1].paraLen > RNIC_RMNET_NAME_MAX_LEN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    AT_WARN_LOG1("At_SetUsbTetherInfo: para2 len is ", g_atParaList[1].paraLen);

    msg = (AT_RNIC_UsbTetherInfoInd *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_RNIC_UsbTetherInfoInd));

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SetUsbTetherInfo: alloc msg fail!");
        return AT_ERROR;
    }

    /* 初始化消息体 */
    memResult = memset_s((VOS_INT8 *)msg + VOS_MSG_HEAD_LENGTH,
                         (VOS_SIZE_T)sizeof(AT_RNIC_UsbTetherInfoInd) - VOS_MSG_HEAD_LENGTH, 0x00,
                         (VOS_SIZE_T)sizeof(AT_RNIC_UsbTetherInfoInd) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(AT_RNIC_UsbTetherInfoInd) - VOS_MSG_HEAD_LENGTH,
                        (VOS_SIZE_T)sizeof(AT_RNIC_UsbTetherInfoInd) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头和消息ID */
    AT_CFG_MSG_HDR(msg, ACPU_PID_RNIC, ID_AT_RNIC_USB_TETHER_INFO_IND);

    /* 填写消息体 */
    msg->tetherConnStat = (VOS_UINT8)g_atParaList[0].paraValue;
    memResult = memcpy_s(msg->rmnetName, RNIC_RMNET_NAME_MAX_LEN, g_atParaList[1].para, g_atParaList[1].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, RNIC_RMNET_NAME_MAX_LEN, g_atParaList[1].paraLen);

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_WARN_LOG("At_SetUsbTetherInfo: Send msg fail!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_TestUsbTetherInfo(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (\"0:disconn\",\"1:connect\"),(\"rmnet0 - rmnet6\")",
        g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetRmnetCfg(VOS_UINT8 indexNum)
{
    AT_RNIC_RmnetCfgReq *msg = VOS_NULL_PTR;
    errno_t memResult;

    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetRmnetCfg: cmdOptType is not AT_CMD_OPT_SET_PARA_CMD!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数目错误 */
    if (g_atParaIndex != AT_RMNETCFG_PARA_NUM) {
        AT_ERR_LOG1("AT_SetRmnetCfg: para number error, numer is !", g_atParaIndex);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数长度异常 */
    if (g_atParaList[1].paraLen > RNIC_RMNET_NAME_MAX_LEN) {
        AT_ERR_LOG1("AT_SetRmnetCfg: para len error, len of para 1 is !", g_atParaList[1].paraLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    msg = (AT_RNIC_RmnetCfgReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_RNIC_RmnetCfgReq));

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SetRmnetCfg: alloc msg fail!");
        return AT_ERROR;
    }

    /* 初始化消息体 */
    memResult = memset_s(AT_GET_MSG_ENTITY(msg), AT_GET_MSG_LENGTH(msg), 0x00,
                     AT_GET_MSG_LENGTH(msg));
    TAF_MEM_CHK_RTN_VAL(memResult, AT_GET_MSG_LENGTH(msg), AT_GET_MSG_LENGTH(msg));

    /* 填写消息头和消息ID */
    AT_CFG_RNIC_MSG_HDR(msg, ID_AT_RNIC_RMNET_CFG_REQ);

    /* 填写消息体 */
    msg->clientId = g_atClientTab[indexNum].clientId;
    msg->featureMode = (VOS_UINT8)g_atParaList[0].paraValue;
    if (g_atParaList[1].paraLen > 0) {
        memResult = memcpy_s(msg->rmnetName, RNIC_RMNET_NAME_MAX_LEN, g_atParaList[1].para,
                             g_atParaList[1].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, RNIC_RMNET_NAME_MAX_LEN, g_atParaList[1].paraLen);
    }
    msg->weight = (VOS_UINT8)g_atParaList[2].paraValue;

    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_ERR_LOG("AT_SetRmnetCfg: Send msg fail!");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RMNETCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_PS_Ipv6IfaceDynamicRstCheck(const VOS_UINT8 indexNum, TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo,
                                          VOS_UINT8 *cid)
{
    TAF_IFACE_DynamicInfo *dynamicInfo = VOS_NULL_PTR;
    VOS_UINT8              cidTemp;

    cidTemp = TAF_INVALID_CID;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cidTemp) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_Ipv6IfaceDynamicRstCheck:ERROR: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if (dynamicInfo[cidTemp].opIpv6Valid == VOS_FALSE) {
        AT_WARN_LOG("AT_PS_Ipv6IfaceDynamicRstCheck:ERROR: bitOpIpv6Valid is FALSE");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    if (dynamicInfo[cidTemp].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
        AT_WARN_LOG("AT_PS_Ipv6IfaceDynamicRstCheck: PDP is not actived in cellular.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    if (dynamicInfo[cidTemp].ipv6Info.raInfo.opPrefixAddr != VOS_TRUE) {
        AT_WARN_LOG("AT_PS_Ipv6IfaceDynamicRstCheck: Prefix address is not received.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    *cid = cidTemp;

    return VOS_OK;
}


VOS_UINT32 At_QryDconnStatPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("At_QryDconnStatPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DCONNSTAT_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestDconnStatPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 构造控制结构体 */
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &ctrl) == VOS_ERR) {
        return AT_ERROR;
    }

    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_TestDconnStatPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DCONNSTAT_TEST;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryNdisStatPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_QryIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    /* 支持1-11的cid */
    bitCid = TAF_IFACE_ALL_BIT_NV_CID;

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_QryNdisStatPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NDISSTATQRY_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetCgmtuPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl   ctrl;
    VOS_UINT32 bitCid;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    rst = At_SetIfaceDynamicParaComCheck(indexNum, &ctrl);

    if (rst != VOS_OK) {
        return rst;
    }

    bitCid = (VOS_UINT32)(0x01UL << g_atParaList[0].paraValue);

    /* 发送查询消息 */
    if (TAF_IFACE_GetDynamicPara(&ctrl, bitCid) != VOS_OK) {
        AT_ERR_LOG("AT_SetCgmtuPara: TAF_IFACE_GetDynamicPara return ERROR!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGMTU_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_PS_ReportSetIpv6TempAddrRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_PS_Ipv6IfaceDynamicRstCheck(indexNum, ifaceDynamicInfo, &cid) != VOS_OK) {
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if (!AT_PS_IS_IPV6_ADDR_IID_VALID(dynamicInfo[cid].ipv6Info.dhcpInfo.tmpAddr)) {
        AT_WARN_LOG("AT_PS_ReportSetIpv6TempAddrRst: IID is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[cid].ipv6Info.dhcpInfo.tmpAddr,
                                      TAF_IPV6_STR_RFC2373_TOKENS);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", ipv6AddrStr);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetDhcpRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    AT_DisplayRate               speed;
    AT_DHCP_Para                 dhcpConfig;
    VOS_UINT16                   length;
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cid) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetDhcpRst: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if (dynamicInfo[cid].opIpv4Valid == VOS_FALSE) {
        AT_WARN_LOG("AT_PS_ReportSetDhcpRst: bitOpIpv4Valid is FALSE");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    if (dynamicInfo[cid].ipv4Info.state != TAF_IFACE_STATE_ACTED) {
        AT_WARN_LOG("AT_PS_ReportSetDhcpRst: PDP is not actived.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 获取接入理论带宽 */
    (VOS_VOID)memset_s(&speed, sizeof(speed), 0x00, (VOS_SIZE_T)(sizeof(speed)));
    if (AT_GetDisplayRate(indexNum, &speed) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetDhcpRst: AT_GetDisplayRate Error!");
    }

    /* 获取DHCP参数(网络序) */
    (VOS_VOID)memset_s(&dhcpConfig, sizeof(dhcpConfig), 0x00, (VOS_SIZE_T)(sizeof(dhcpConfig)));
    AT_GetDhcpPara(&dhcpConfig, &(dynamicInfo[cid].ipv4Info.dhcpInfo));

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.iPAddr);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.subNetMask);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.gateWay);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.gateWay);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.primDNS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.sndDNS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s,", speed.dlSpeed);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", speed.ulSpeed);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryDhcpRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    AT_DisplayRate               speed;
    AT_DHCP_Para                 dhcpConfig;
    VOS_UINT32                   i;
    VOS_UINT32                   rst;
    VOS_UINT16                   length;

    length           = 0;
    rst              = AT_ERROR;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    (VOS_VOID)memset_s(&speed, sizeof(speed), 0x00, (VOS_SIZE_T)(sizeof(speed)));
    (VOS_VOID)memset_s(&dhcpConfig, sizeof(dhcpConfig), 0x00, (VOS_SIZE_T)(sizeof(dhcpConfig)));

    /* 目前DHCP的查询只支持1-11的cid */
    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
            continue;
        }

        if (dynamicInfo[i].opIpv4Valid == VOS_FALSE) {
            continue;
        }

        if (dynamicInfo[i].ipv4Info.state != TAF_IFACE_STATE_ACTED) {
            continue;
        }

        /* 获取接入理论带宽 */
        if (AT_GetDisplayRate(indexNum, &speed) == VOS_ERR) {
            AT_WARN_LOG("AT_PS_ReportQryDhcpRst: AT_GetDisplayRate Error!");
        }

        /* 获取DHCP参数(网络序) */
        AT_GetDhcpPara(&dhcpConfig, &(dynamicInfo[i].ipv4Info.dhcpInfo));

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.iPAddr);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.subNetMask);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.gateWay);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.gateWay);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.primDNS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,", dhcpConfig.dhcpCfg.sndDNS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s,", speed.dlSpeed);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", speed.ulSpeed);

        rst = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rst);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetDhcpv6Rst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    AT_DisplayRate               speed;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_PS_Ipv6IfaceDynamicRstCheck(indexNum, ifaceDynamicInfo, &cid) != VOS_OK) {
        return VOS_ERR;
    }

    /* 获取接入理论带宽 */
    (VOS_VOID)memset_s(&speed, sizeof(speed), 0x00, (VOS_SIZE_T)(sizeof(speed)));
    if (AT_GetDisplayRate(indexNum, &speed) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetDhcpv6Rst: AT_GetDisplayRate Error!");
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));
    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    /* 填写IPV6地址 */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[cid].ipv6Info.dhcpInfo.addr,
                                      TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", ipv6AddrStr);

    /* 填写IPV6掩码, 该字段填全0 */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

    /* 填写IPV6网关, 该字段填全0 */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

    /* 填写DHCP IPV6, 该字段填全0 */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

    /* 填写IPV6 Primary DNS */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[cid].ipv6Info.dhcpInfo.primDns,
                                      TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

    /* 填写IPV6 Secondary DNS */
    AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[cid].ipv6Info.dhcpInfo.secDns,
                                      TAF_IPV6_STR_RFC2373_TOKENS);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

    /* 填写MAX RX/TX Rate */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", speed.dlSpeed);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", speed.ulSpeed);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryDhcpv6Rst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    AT_DisplayRate               speed;
    VOS_UINT32                   i;
    VOS_UINT32                   rst;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];

    length           = 0;
    rst              = AT_ERROR;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    (VOS_VOID)memset_s(&speed, sizeof(speed), 0x00, (VOS_SIZE_T)(sizeof(speed)));
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));

    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
            continue;
        }

        if (dynamicInfo[i].opIpv6Valid == VOS_FALSE) {
            continue;
        }

        if (dynamicInfo[i].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
            continue;
        }

        if (dynamicInfo[i].ipv6Info.raInfo.opPrefixAddr != VOS_TRUE) {
            continue;
        }

        /* 获取接入理论带宽 */
        if (AT_GetDisplayRate(indexNum, &speed) == VOS_ERR) {
            AT_WARN_LOG("AT_PS_ReportQryDhcpv6Rst: AT_GetDisplayRate Error!");
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* 填写IPV6地址 */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[i].ipv6Info.dhcpInfo.addr,
                                          TAF_IPV6_STR_RFC2373_TOKENS);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", ipv6AddrStr);

        /* 填写IPV6掩码, 该字段填全0 */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

        /* 填写IPV6网关, 该字段填全0 */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

        /* 填写DHCP IPV6, 该字段填全0 */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

        /* 填写IPV6 Primary DNS */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[i].ipv6Info.dhcpInfo.primDns,
                                          TAF_IPV6_STR_RFC2373_TOKENS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

        /* 填写IPV6 Secondary DNS */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[i].ipv6Info.dhcpInfo.secDns,
                                          TAF_IPV6_STR_RFC2373_TOKENS);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);

        /* 填写MAX RX/TX Rate */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", speed.dlSpeed);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", speed.ulSpeed);

        rst = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rst);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetApRaInfoRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    VOS_UINT32                   mtuSize;
    VOS_UINT32                   prefixBitLen;
    VOS_UINT32                   preferredLifetime;
    VOS_UINT32                   validLifetime;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cid) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetApRaInfoRst: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if (dynamicInfo[cid].opIpv6Valid == VOS_FALSE) {
        AT_WARN_LOG("AT_PS_ReportSetApRaInfoRst: bitOpIpv6Valid is FALSE");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    if (dynamicInfo[cid].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
        AT_WARN_LOG("AT_PS_ReportSetApRaInfoRst: PDP is not actived in cellular.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 填写MTU */
    if (dynamicInfo[cid].ipv6Info.raInfo.opMtuSize == VOS_FALSE) {
        mtuSize = 0;
    } else {
        mtuSize = dynamicInfo[cid].ipv6Info.raInfo.mtuSize;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", mtuSize);
    /* 填写Prefix */
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));

    if (dynamicInfo[cid].ipv6Info.raInfo.opPrefixAddr == VOS_FALSE) {
        prefixBitLen = 0;
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
    } else {
        prefixBitLen = dynamicInfo[cid].ipv6Info.raInfo.prefixBitLen;
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, dynamicInfo[cid].ipv6Info.raInfo.prefixAddr,
                                          TAF_IPV6_STR_RFC2373_TOKENS);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s/%d\"", ipv6AddrStr, prefixBitLen);

    /* 填写Preferred Lifetime */
    if (dynamicInfo[cid].ipv6Info.raInfo.opPreferredLifetime == VOS_FALSE) {
        preferredLifetime = 0;
    } else {
        preferredLifetime = dynamicInfo[cid].ipv6Info.raInfo.preferredLifetime;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%u", preferredLifetime);

    if (dynamicInfo[cid].ipv6Info.raInfo.opValidLifetime == VOS_FALSE) {
        validLifetime = 0;
    } else {
        validLifetime = dynamicInfo[cid].ipv6Info.raInfo.validLifetime;
    }

    /* 填写Valid Lifetime */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%u", validLifetime);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryApRaInfoRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    TAF_IFACE_Ipv6RaInfo        *appRaInfoAddr    = VOS_NULL_PTR;
    VOS_UINT32                   mtuSize;
    VOS_UINT32                   prefixBitLen;
    VOS_UINT32                   preferredLifetime;
    VOS_UINT32                   validLifetime;
    VOS_UINT32                   i;
    VOS_UINT32                   rst;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];

    length           = 0;
    rst              = AT_ERROR;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));

    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        appRaInfoAddr = &(dynamicInfo[i].ipv6Info.raInfo);

        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & ((VOS_UINT32)1 << i)) == 0) {
            continue;
        }

        if (dynamicInfo[i].opIpv6Valid == VOS_FALSE) {
            continue;
        }

        if (dynamicInfo[i].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
            continue;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName, i);
        /* 填写MTU */
        if (appRaInfoAddr->opMtuSize == VOS_FALSE) {
            mtuSize = 0;
        } else {
            mtuSize = appRaInfoAddr->mtuSize;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", mtuSize);

        /* 填写Prefix */
        if (appRaInfoAddr->opPrefixAddr == VOS_FALSE) {
            prefixBitLen = 0;
            AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
        } else {
            prefixBitLen = appRaInfoAddr->prefixBitLen;
            AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, appRaInfoAddr->prefixAddr,
                                              (VOS_UINT8)prefixBitLen / AT_TOKENS_BIT_LEN);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s/%d\"", ipv6AddrStr, prefixBitLen);

        /* 填写Preferred Lifetime */
        if (appRaInfoAddr->opPreferredLifetime == VOS_FALSE) {
            preferredLifetime = 0;
        } else {
            preferredLifetime = appRaInfoAddr->preferredLifetime;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%u", preferredLifetime);

        if (appRaInfoAddr->opValidLifetime == VOS_FALSE) {
            validLifetime = 0;
        } else {
            validLifetime = appRaInfoAddr->validLifetime;
        }

        /* 填写Valid Lifetime */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%u%s", validLifetime, g_atCrLf);
    }

    if (length < (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf)) {
        AT_ERR_LOG("AT_PS_ReportQryApRaInfoRst: CID INFO is invalid.");
    } else {
        length -= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
        rst = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rst);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetApLanAddrRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    TAF_IFACE_Ipv6RaInfo        *ipv6RaInfo       = VOS_NULL_PTR;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cid) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetApLanAddrRst: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if (dynamicInfo[cid].opIpv6Valid == VOS_FALSE) {
        AT_WARN_LOG("AT_PS_ReportSetApLanAddrRst: bitOpIpv6Valid is FALSE");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 当前未激活，直接返回error */
    if (dynamicInfo[cid].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
        AT_WARN_LOG("AT_PS_ReportSetApLanAddrRst: PDP is not actived in cellular.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    ipv6RaInfo = &(dynamicInfo[cid].ipv6Info.raInfo);
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (ipv6RaInfo->opLanAddr == VOS_FALSE) {
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
    } else {
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, ipv6RaInfo->lanAddr, TAF_IPV6_STR_RFC2373_TOKENS);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", ipv6AddrStr);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", ipv6RaInfo->prefixBitLen);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryApLanAddrRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    TAF_IFACE_Ipv6RaInfo        *appRaInfoAddr    = VOS_NULL_PTR;
    VOS_UINT32                   i;
    VOS_UINT32                   rst;
    VOS_UINT16                   length;
    VOS_UINT8                    ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT8                    invalidIpv6Addr[TAF_IPV6_ADDR_LEN];

    length           = 0;
    rst              = AT_ERROR;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));
    (VOS_VOID)memset_s(invalidIpv6Addr, sizeof(invalidIpv6Addr), 0x00, sizeof(invalidIpv6Addr));

    if (g_atClientTab[indexNum].userType == AT_APP_USER) {
        for (i = 1; i <= TAF_MAX_CID_NV; i++) {
            appRaInfoAddr = &(dynamicInfo[i].ipv6Info.raInfo);

            /* 判断此CID是否有效 */
            if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
                continue;
            }

            if (dynamicInfo[i].opIpv6Valid == VOS_FALSE) {
                continue;
            }

            /* 当前未激活 */
            if (dynamicInfo[i].ipv6Info.state != TAF_IFACE_STATE_ACTED) {
                continue;
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName, i);

            if (appRaInfoAddr->opLanAddr == VOS_FALSE) {
                AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, invalidIpv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);
            } else {
                AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, appRaInfoAddr->lanAddr, TAF_IPV6_STR_RFC2373_TOKENS);
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", ipv6AddrStr);

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d%s", appRaInfoAddr->prefixBitLen, g_atCrLf);
        }
    }

    if (length < (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf)) {
        AT_ERR_LOG("AT_PS_ReportQryApLanAddrRst: CID INFO is invalid.");
    } else {
        length -= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
        rst = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rst);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetApConnStRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);
    AT_APP_ConnStateUint32       ipv4Status = AT_APP_UNDIALED;
    AT_APP_ConnStateUint32       ipv6Status = AT_APP_UNDIALED;
    VOS_UINT16                   length = 0;
    VOS_UINT8                    cid = TAF_INVALID_CID;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cid) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetApConnStRst: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    if (dynamicInfo[cid].opEtherValid == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,\"IPV4\",%d,\"IPV6\",%d,\"Ethernet\"",
            g_parseContext[indexNum].cmdElement->cmdName, AT_APP_UNDIALED, AT_APP_UNDIALED,
            AT_AppConvertPdpStateToConnStatus(dynamicInfo[cid].etherInfo.state));
    } else {
        if ((dynamicInfo[cid].opIpv4Valid == VOS_FALSE) && (dynamicInfo[cid].opIpv6Valid == VOS_FALSE)) {
            AT_WARN_LOG("AT_PS_ReportSetApConnStRst: bitOpIpv4v6Valid both FALSE");
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_ERROR);
            return VOS_ERR;
        }

        switch (AT_GetIpv6Capability()) {
            case AT_IPV6_CAPABILITY_IPV4_ONLY:
                ipv4Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[cid].ipv4Info.state);

                length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d,\"IPV4\"", ipv4Status);
                break;

            case AT_IPV6_CAPABILITY_IPV6_ONLY:
                ipv6Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[cid].ipv6Info.state);

                length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d,\"IPV6\"", ipv6Status);
                break;

            case AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP:
            case AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP:
                ipv4Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[cid].ipv4Info.state);
                ipv6Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[cid].ipv6Info.state);

                length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d,\"IPV4\"", ipv4Status);
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,\"IPV6\"", ipv6Status);
                break;

            default:
                break;
        }
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryApConnStRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);
    AT_APP_ConnStateUint32       ipv4Status = AT_APP_UNDIALED;
    AT_APP_ConnStateUint32       ipv6Status = AT_APP_UNDIALED;
    VOS_UINT32                   i;
    VOS_UINT16                   length = 0;

    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & ((VOS_UINT32)1 << i)) == 0) {
            continue;
        }

        if (dynamicInfo[i].opEtherValid == VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"IPV4\",%d,\"IPV6\",%d,\"Ethernet\"%s",
                g_parseContext[indexNum].cmdElement->cmdName, i, AT_APP_UNDIALED, AT_APP_UNDIALED,
                AT_AppConvertPdpStateToConnStatus(dynamicInfo[i].etherInfo.state), g_atCrLf);
            continue;
        }

        if ((dynamicInfo[i].opIpv4Valid == VOS_FALSE) && (dynamicInfo[i].opIpv6Valid == VOS_FALSE)) {
            continue;
        }

        switch (AT_GetIpv6Capability()) {
            case AT_IPV6_CAPABILITY_IPV4_ONLY:
                ipv4Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[i].ipv4Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"IPV4\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv4Status, g_atCrLf);
                break;

            case AT_IPV6_CAPABILITY_IPV6_ONLY:
                ipv6Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[i].ipv6Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"IPV6\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv6Status, g_atCrLf);
                break;

            case AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP:
            case AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP:
                ipv4Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[i].ipv4Info.state);
                ipv6Status = AT_AppConvertPdpStateToConnStatus(dynamicInfo[i].ipv6Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"IPV4\",%d,\"IPV6\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv4Status, ipv6Status, g_atCrLf);
                break;

            default:
                break;
        }
    }

    if (length >= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf)) {
        length -= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


AT_PDP_StatusUint32 AT_NdisGetConnStatus(TAF_IFACE_StateUint8 pdpState)
{
    AT_PDP_StatusUint32 connStatus;

    connStatus = AT_PDP_STATUS_DEACT;

    if ((pdpState == TAF_IFACE_STATE_ACTED) || (pdpState == TAF_IFACE_STATE_DEACTING)) {
        connStatus = AT_PDP_STATUS_ACT;
    }

    return connStatus;
}

VOS_UINT32 AT_PS_ReportQryDconnStatRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);
    TAF_PDP_PrimContext          pdpPriPara;
    AT_PDP_StatusUint32          ipv4Status;
    AT_PDP_StatusUint32          ipv6Status;
    AT_PDP_StatusUint32          etherStatus;
    AT_DIALTYPE_Uint32           dialType;
    VOS_UINT32                   i;
    errno_t                      memResult;
    VOS_UINT16                   length = 0;
    VOS_UINT8                    str[TAF_MAX_APN_LEN + 1];

    (VOS_VOID)memset_s(&pdpPriPara, sizeof(pdpPriPara), 0x00, sizeof(pdpPriPara));
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 目前查询只支持1-11的cid */
    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* ^DCONNSTAT: */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* CID */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", i);

        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            continue;
        }

        if ((dynamicInfo[i].opIpv4Valid == VOS_FALSE) && (dynamicInfo[i].opIpv6Valid == VOS_FALSE) &&
            (dynamicInfo[i].opEtherValid == VOS_FALSE)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            continue;
        }

        ipv4Status  = AT_NdisGetConnStatus(dynamicInfo[i].ipv4Info.state);
        ipv6Status  = AT_NdisGetConnStatus(dynamicInfo[i].ipv6Info.state);
        etherStatus = AT_NdisGetConnStatus(dynamicInfo[i].etherInfo.state);

        if ((ipv4Status == AT_PDP_STATUS_DEACT) && (ipv6Status == AT_PDP_STATUS_DEACT) &&
            (etherStatus == AT_PDP_STATUS_DEACT)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            continue;
        }

        if (dynamicInfo[i].userType == TAF_IFACE_USER_TYPE_NDIS) {
            dialType = AT_DIALTYPE_NDIS;
        } else if (dynamicInfo[i].userType == TAF_IFACE_USER_TYPE_APP) {
            dialType = AT_DIALTYPE_APP;
        } else {
            dialType = AT_DIALTYPE_BUTT;
        }

        /* APN */
        (VOS_VOID)memset_s(str, sizeof(str), 0x00, sizeof(str));
        if (dynamicInfo[i].apnLen > 0) {
            memResult = memcpy_s(str, sizeof(str), dynamicInfo[i].apn, TAF_MIN(dynamicInfo[i].apnLen, TAF_MAX_APN_LEN));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(str), TAF_MIN(dynamicInfo[i].apnLen, TAF_MAX_APN_LEN));
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", str);

        if (AT_GetEthernetCap() != VOS_TRUE) {
            /* IPV4 IPV6 STATUS */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d%s", ipv4Status, ipv6Status, dialType, g_atCrLf);
        } else {
            /* IPV4 IPV6 Ethernet STATUS */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d,%d%s", ipv4Status, ipv6Status, dialType,
                etherStatus, g_atCrLf);
        }
    }

    length -= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportTestDconnStatRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);
    AT_PDP_StatusUint32          ipv4Status;
    AT_PDP_StatusUint32          ipv6Status;
    AT_PDP_StatusUint32          etherStatus;
    VOS_UINT32                   i;
    VOS_UINT16                   length = 0;
    VOS_UINT8                    hasDialedFlg = VOS_FALSE;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, "(");

    /* 目前查询只支持1-11的cid */
    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
            continue;
        }

        if ((dynamicInfo[i].opIpv4Valid == VOS_FALSE) && (dynamicInfo[i].opIpv6Valid == VOS_FALSE) &&
            (dynamicInfo[i].opEtherValid == VOS_FALSE)) {
            continue;
        }

        ipv4Status  = AT_NdisGetConnStatus(dynamicInfo[i].ipv4Info.state);
        ipv6Status  = AT_NdisGetConnStatus(dynamicInfo[i].ipv6Info.state);
        etherStatus = AT_NdisGetConnStatus(dynamicInfo[i].etherInfo.state);

        if ((ipv4Status == AT_PDP_STATUS_ACT) || (ipv6Status == AT_PDP_STATUS_ACT) ||
            (etherStatus == AT_PDP_STATUS_ACT)) {
            hasDialedFlg = VOS_TRUE;

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", i);
        }
    }

    if (hasDialedFlg == VOS_TRUE) {
        /* 删除最后一个逗号 */
        length--;

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", ")");
    } else {
        /* 若无连接态CID,直接返回OK */
        length = 0;
    }

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportQryNdisStatRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);
    AT_PDP_StatusUint32          ipv4Status = AT_PDP_STATUS_DEACT;
    AT_PDP_StatusUint32          ipv6Status = AT_PDP_STATUS_DEACT;
    VOS_UINT32                   i;
    VOS_UINT16                   length = 0;
    const VOS_UINT8              strIpv4[] = "IPV4";
    const VOS_UINT8              strIpv6[] = "IPV6";
    VOS_UINT8                    strEther[] = "Ethernet";

    for (i = 1; i <= TAF_MAX_CID_NV; i++) {
        /* 判断此CID是否有效 */
        if ((ifaceDynamicInfo->cid & (0x01UL << i)) == 0) {
            continue;
        }

        if (dynamicInfo[i].opEtherValid == VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,,,\"%s\"%s",
                g_parseContext[indexNum].cmdElement->cmdName, i, AT_NdisGetConnStatus(dynamicInfo[i].etherInfo.state),
                strEther, g_atCrLf);
            continue;
        }

        if ((dynamicInfo[i].opIpv4Valid == VOS_FALSE) && (dynamicInfo[i].opIpv6Valid == VOS_FALSE)) {
            continue;
        }

        switch (AT_GetIpv6Capability()) {
            case AT_IPV6_CAPABILITY_IPV4_ONLY:
                ipv4Status = AT_NdisGetConnStatus(dynamicInfo[i].ipv4Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,,,\"%s\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv4Status, strIpv4, g_atCrLf);
                break;

            case AT_IPV6_CAPABILITY_IPV6_ONLY:
                ipv6Status = AT_NdisGetConnStatus(dynamicInfo[i].ipv6Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,,,\"%s\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv6Status, strIpv6, g_atCrLf);
                break;

            case AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP:
            case AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP:
                ipv4Status = AT_NdisGetConnStatus(dynamicInfo[i].ipv4Info.state);
                ipv6Status = AT_NdisGetConnStatus(dynamicInfo[i].ipv6Info.state);

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,,,\"%s\",%d,,,\"%s\"%s",
                    g_parseContext[indexNum].cmdElement->cmdName, i, ipv4Status, strIpv4, ipv6Status, strIpv6,
                    g_atCrLf);
                break;

            default:
                break;
        }
    }

    if (length >= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf)) {
        length -= (VOS_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
    }

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_PS_ReportSetCgmtuRst(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_GetDynamicParaCnf *ifaceDynamicInfo = VOS_NULL_PTR;
    TAF_IFACE_DynamicInfo       *dynamicInfo      = VOS_NULL_PTR;
    VOS_UINT16                   length;
    VOS_UINT8                    cid;

    cid              = TAF_INVALID_CID;
    ifaceDynamicInfo = (TAF_IFACE_GetDynamicParaCnf *)evtInfo;

    if (AT_GetCidByCidMask(ifaceDynamicInfo->cid, &cid) == VOS_ERR) {
        AT_WARN_LOG("AT_PS_ReportSetCgmtuRst: ulBitCid is invalid.");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    dynamicInfo = AT_GetDynamicInfoBaseAddr(ifaceDynamicInfo);

    if ((dynamicInfo[cid].opIpv4Valid == VOS_FALSE) && (dynamicInfo[cid].opIpv6Valid == VOS_FALSE) &&
        (dynamicInfo[cid].opEtherValid == VOS_FALSE)) {
        AT_WARN_LOG("AT_PS_ReportSetCgmtuRst: bitOpIpv4Valid and bitOpIpv6Valid is FALSE");
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 上报查询结果 */
    if (AT_GetEthernetCap() == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName, cid,
            dynamicInfo[cid].ipv4Info.mtu, dynamicInfo[cid].ipv6Info.raInfo.mtuSize, dynamicInfo[cid].etherInfo.mtu);
    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName, cid,
            dynamicInfo[cid].ipv4Info.mtu, dynamicInfo[cid].ipv6Info.raInfo.mtuSize);
    }
    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_SetRoamPdpTypePara(VOS_UINT8 indexNum)
{
    TAF_PS_RoamingPdpTypeInfo pdpCxtInfo;
    VOS_UINT32                rslt;

    /* 初始化 */
    (VOS_VOID)memset_s(&pdpCxtInfo, sizeof(pdpCxtInfo), 0, sizeof(pdpCxtInfo));

    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_ROAMPDPTYPEPARA_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<CID> */
    pdpCxtInfo.cid = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 如果参数只有<CID>, 无需检查其它参数 */
    if (g_atParaIndex == 1) {
        pdpCxtInfo.defined = VOS_FALSE;
    } else {
        /* 不设置ip类型 或类型错误直接返回 */
        if ((g_atParaList[1].paraLen == 0) || (g_atParaList[1].paraValue >= TAF_PDP_IPV4V6)) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        pdpCxtInfo.defined = VOS_TRUE;

        /* 设置<PDP_type> IP:1, IPV6:2, IPV4V6:3 */
        pdpCxtInfo.roamingPdpType = (VOS_UINT8)(g_atParaList[1].paraValue + 1);
    }

    rslt = TAF_PS_SetRoamPdpType(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0, &pdpCxtInfo);
    /* 执行命令操作 */
    if (rslt != VOS_OK) {
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ROAMPDPTYPE_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_SetCpolicyRptPara(VOS_UINT8 indexNum)
{
    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_PS_SetUePolicyRpt(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0,
                              (VOS_UINT8)g_atParaList[0].paraValue) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPOLICYRPT_SET;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_GetCpolicyCodePara(VOS_UINT8 indexNum)
{
    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_PS_GetUePolicyInfo(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), 0,
                               (VOS_UINT8)g_atParaList[0].paraValue) != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPOLICYCODE_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}
#endif

