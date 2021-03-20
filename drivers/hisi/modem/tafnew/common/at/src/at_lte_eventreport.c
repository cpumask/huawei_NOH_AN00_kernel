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
/*lint -save -e537 -e718 -e746 -e734 -e958 -e713 -e740*/
#include "at_lte_eventreport.h"
#include "securec.h"
#include "osm.h"
#include "gen_msg.h"
#include "msp_at.h"
#include "at_lte_common.h"
#include "ATCmdProc.h"
#include "AtParse.h" /* 为包含mn_client.h */

#include "gen_common.h"
#include "at_common.h"
#include "AtCmdMiscProc.h"

/*lint -e767 原因:Log打印*/
#define THIS_FILE_ID MSP_FILE_ID_AT_LTE_EVENTREPORT_C
/*lint +e767 */
#define AT_ANLEVEL_INFO_RSRP_UNVALID 99
#define AT_ANLEVEL_INFO_RSRQ_UNVALID 99
#define AT_MSG_BLOCK_VALUE_LEN 2
#define AT_MNC_MIN_NUM 2
#define AT_NVIM_RSRP_VALUE_INDEX_1 1
#define AT_NVIM_RSRP_VALUE_INDEX_2 2
#define AT_NVIM_RSRP_VALUE_INDEX_3 3

#define AT_ANTENNA_LEVEL_NUM 3

#define AT_CASCELL_RSSI_VALUE_MIN (-120) /* RSSI 最小门限 */
#define AT_CASCELL_RSSI_VALUE_MAX (-25) /* RSSI 最大门限 */
#define AT_CASCELL_RSRP_VALUE_MIN (-140) /* RSRP 最小门限 */
#define AT_CASCELL_RSRP_VALUE_MAX (-44) /* RSRP 最大门限 */
#define AT_CASCELL_RSRQ_VALUE_MIN (-19) /* RSRQ 最小门限 */
#define AT_CASCELL_RSRQ_VALUE_MAX (-3) /* RSRQ 最大门限 */

VOS_UINT8 g_atAntennaLevel[AT_ANTENNA_LEVEL_NUM];
VOS_UINT8 g_antennaLevel = 0;
VOS_UINT32 g_ate5Order = 0;


/* ***************************************************************************** */

/* 函数名称: atCsqInfoIndProc */

/* 功能描述: 处理CSQ主动上报信息 */

/*  */

/* 参数说明: pMsgBlock，MSP MSG消息结构 */

/*  */

/* 返 回 值: */

/*  */

/* 调用要求: TODO: ... */

/* 调用举例: TODO: ... */


/* ***************************************************************************** */

/* 按照V1中的#define MSP_HAVE_AT_RSSI_REPORT修改的 */

VOS_UINT32 atCsqInfoIndProc(struct MsgCB *msgBlock)
{
    L4A_CSQ_INFO_IND_STRU *csqInfo = NULL;
    VOS_UINT16             length  = 0;
    VOS_INT16              rssi    = 0;

    csqInfo = (L4A_CSQ_INFO_IND_STRU *)msgBlock;

    if (csqInfo->errorCode == ERR_MSP_SUCCESS) {
        if (csqInfo->rssi == AT_RSSI_UNKNOWN) {
            rssi = AT_RSSI_UNKNOWN;
        } else if (csqInfo->rssi >= AT_RSSI_HIGH) {
            rssi = AT_CSQ_RSSI_HIGH;
        } else if (csqInfo->rssi <= AT_RSSI_LOW) {
            rssi = AT_CSQ_RSSI_LOW;
        } else {
            rssi = (csqInfo->rssi - AT_RSSI_LOW) / 2;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "%s%s%d%s", g_atCrLf, "^RSSI:", rssi, g_atCrLf);
        At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddr, length);
    }

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT8 AT_CalculateLTEAntennaLevel(VOS_INT16 rsrp)
{
    VOS_INT16 level = 0;
    /* 天线格式显示规则 */

    if (rsrp <= g_rsrpCfg.value[AT_NVIM_RSRP_VALUE_INDEX_3]) {
        level = AT_CMD_ANTENNA_LEVEL_4;
    } else if ((g_rsrpCfg.value[AT_NVIM_RSRP_VALUE_INDEX_3] < rsrp) &&
               (rsrp <= g_rsrpCfg.value[AT_NVIM_RSRP_VALUE_INDEX_2])) {
        level = AT_CMD_ANTENNA_LEVEL_3;
    } else if ((g_rsrpCfg.value[AT_NVIM_RSRP_VALUE_INDEX_2] < rsrp) &&
               (rsrp <= g_rsrpCfg.value[AT_NVIM_RSRP_VALUE_INDEX_1])) {
        level = AT_CMD_ANTENNA_LEVEL_2;
    } else {
        level = AT_CMD_ANTENNA_LEVEL_1;
    }
    return (VOS_UINT8)level;
}

VOS_UINT8 AT_GetSmoothLTEAntennaLevel(AT_CmdAntennaLevelUint8 level)
{
    VOS_UINT32 i;
    /* 丢网时 立即更新  */
    if (level == AT_CMD_ANTENNA_LEVEL_0) {
        g_antennaLevel = level;

        (VOS_VOID)memset_s(g_atAntennaLevel, sizeof(g_atAntennaLevel), 0x00, sizeof(g_atAntennaLevel));
        return g_antennaLevel;
    }

    /* 丢网到有服务状态  立即更新  */
    if (g_antennaLevel == AT_CMD_ANTENNA_LEVEL_0) {
        g_antennaLevel = level;

        memset_s(g_atAntennaLevel, sizeof(g_atAntennaLevel), 0x00, sizeof(g_atAntennaLevel));
        g_atAntennaLevel[AT_ANTENNA_LEVEL_NUM - 1] = level;
        return g_antennaLevel;
    }

    /* 与上次的信号格数比较, 变化比较大(超过1格)就立即更新 */
    if (level > (g_antennaLevel + 1)) {
        g_antennaLevel = level;

        memset_s(g_atAntennaLevel, sizeof(g_atAntennaLevel), 0x00, sizeof(g_atAntennaLevel));
        g_atAntennaLevel[AT_ANTENNA_LEVEL_NUM - 1] = level;
        return g_antennaLevel;
    } else if ((level + 1) < g_antennaLevel) {
        g_antennaLevel = level;

        memset_s(g_atAntennaLevel, sizeof(g_atAntennaLevel), 0x00, sizeof(g_atAntennaLevel));
        g_atAntennaLevel[AT_ANTENNA_LEVEL_NUM - 1] = level;
        return g_antennaLevel;
    } else {
        /* Do nothing... */
    }

    /* 先进先出存最近3次的查询结果 */
    for (i = 0; i < AT_ANTENNA_LEVEL_NUM - 1; i++) {
        g_atAntennaLevel[i] = g_atAntennaLevel[i + 1];
    }
    g_atAntennaLevel[i] = level;

    /* 格数波动则不更新，以达到平滑的效果 */
    for (i = 0; i < AT_ANTENNA_LEVEL_NUM - 1; i++) {
        if (g_atAntennaLevel[i] != g_atAntennaLevel[i + 1]) {
            return g_antennaLevel;
        }
    }

    /* 信号格数稳定了 AT_ANTENNA_LEVEL_MAX_NUM 次时才做更新 */
    g_antennaLevel = level;
    return g_antennaLevel;
}

VOS_UINT32 at_CsqInfoProc(struct MsgCB *msgBlock, AT_AnlevelInfoCnf *anlevelAnqueryInfo)
{
    VOS_UINT32             result  = ERR_MSP_SUCCESS;
    L4A_CSQ_INFO_IND_STRU *csqInfo = NULL;
    VOS_UINT16             rsrp;
    VOS_UINT8              curAntennaLevel;

    if ((msgBlock == NULL) || (anlevelAnqueryInfo == NULL)) {
        HAL_SDMLOG("pMsgBlock is NULL!\n");
        return AT_ERROR;
    }
    csqInfo                       = (L4A_CSQ_INFO_IND_STRU *)msgBlock;
    anlevelAnqueryInfo->clientId  = csqInfo->clientId;
    anlevelAnqueryInfo->errorCode = csqInfo->errorCode;

    /* RSSI 格式显示规则 */

    if (csqInfo->rssi == AT_RSSI_UNKNOWN) {
        anlevelAnqueryInfo->rssi = AT_RSSI_UNKNOWN;
    } else if (csqInfo->rssi >= AT_RSSI_HIGH) {
        anlevelAnqueryInfo->rssi = AT_CSQ_RSSI_HIGH;
    } else if (csqInfo->rssi <= AT_RSSI_LOW) {
        anlevelAnqueryInfo->rssi = AT_CSQ_RSSI_LOW;
    } else {
        anlevelAnqueryInfo->rssi = (VOS_UINT16)((csqInfo->rssi - AT_RSSI_LOW) / 2);
    }

    /*
     * 上报数据转换:将 Rscp、Ecio显示为非负值，若Rscp、Ecio为-145，-32，或者rssi为99，
     * 则转换为0
     */
    if ((csqInfo->rsrp == AT_RSSI_UNKNOWN) || (csqInfo->rssi == AT_RSSI_UNKNOWN)) {
        /* 丢网返回0, 对应应用的圈外 */
        curAntennaLevel = AT_CMD_ANTENNA_LEVEL_0;
    } else {
        /* 取绝对值 */
        rsrp = (VOS_UINT16)(-(csqInfo->rsrp));
        /* 调用函数AT_CalculateAntennaLevel来根据D25算法计算出信号格数 */
        curAntennaLevel = AT_CalculateLTEAntennaLevel((VOS_INT16)rsrp);
    }

    /* 信号磁滞处理 */
    anlevelAnqueryInfo->level = AT_GetSmoothLTEAntennaLevel(curAntennaLevel);

    anlevelAnqueryInfo->rsrp = csqInfo->rsrp;
    anlevelAnqueryInfo->rsrq = csqInfo->rsrq;

    return result;
}

VOS_UINT32 atAnlevelInfoIndProc(struct MsgCB *msgBlock)
{
    VOS_UINT16        length = 0;
    VOS_UINT32        result;
    VOS_UINT16        rsrp               = 0;
    VOS_UINT16        rsrq               = 0;
    AT_AnlevelInfoCnf anlevelAnqueryInfo = {0};

    result = at_CsqInfoProc(msgBlock, &anlevelAnqueryInfo);

    if (result == ERR_MSP_SUCCESS) {
        rsrp = (VOS_UINT16)((anlevelAnqueryInfo.rsrp == AT_ANLEVEL_INFO_RSRP_UNVALID) ?
                AT_ANLEVEL_INFO_RSRP_UNVALID : (-(anlevelAnqueryInfo.rsrp)));
        rsrq = (VOS_UINT16)((anlevelAnqueryInfo.rsrq == AT_ANLEVEL_INFO_RSRQ_UNVALID) ?
                AT_ANLEVEL_INFO_RSRQ_UNVALID : (-(anlevelAnqueryInfo.rsrq)));

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "%s%s0,99,%d,%d,%d,%d%s", g_atCrLf, "^ANLEVEL:",
            anlevelAnqueryInfo.rssi, anlevelAnqueryInfo.level, rsrp, rsrq, g_atCrLf);
        At_SendResultData(AT_CLIENT_ID_APP, g_atSndCodeAddr, length);
    } else {
        HAL_SDMLOG("ulResult = %d,\n", (VOS_INT)result);
    }

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atSetAnlevelCnfSameProc(struct MsgCB *msgBlock)
{
    VOS_UINT16 length = 0;
    VOS_UINT32 result;
    VOS_UINT16 rsrp = 0;
    VOS_UINT16 rsrq = 0;

    AT_AnlevelInfoCnf anlevelAnqueryInfo = {0};
    g_ate5Order                          = 1;

    result = at_CsqInfoProc(msgBlock, &anlevelAnqueryInfo);

    if (result == ERR_MSP_SUCCESS) {
        rsrp = (VOS_UINT16)((anlevelAnqueryInfo.rsrp == AT_ANLEVEL_INFO_RSRP_UNVALID) ?
                AT_ANLEVEL_INFO_RSRP_UNVALID : (-(anlevelAnqueryInfo.rsrp)));
        rsrq = (VOS_UINT16)((anlevelAnqueryInfo.rsrq == AT_ANLEVEL_INFO_RSRQ_UNVALID) ?
                AT_ANLEVEL_INFO_RSRQ_UNVALID : (-(anlevelAnqueryInfo.rsrq)));

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "%s%s0,99,%d,%d,%d,%d%s", g_atCrLf, "^ANQUERY:",
            anlevelAnqueryInfo.rssi, anlevelAnqueryInfo.level, rsrp, rsrq, g_atCrLf);
    } else {
        HAL_SDMLOG(" atSetAnlevelCnfSameProc ulResult = %d,\n", (VOS_INT)result);
    }

    CmdErrProc((VOS_UINT8)(anlevelAnqueryInfo.clientId), anlevelAnqueryInfo.errorCode, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 函数名称: atSetCsqCnfProc */

/* 功能描述: 处理SET CSQ命令回复信息 */

/*  */

/* 参数说明: pMsgBlock，MSP MSG消息结构 */

/*  */

/* 返 回 值: */

/*  */

/* 调用要求: TODO: ... */

/* 调用举例: TODO: ... */


/* ***************************************************************************** */

/* 这个函数跟AT命令表中的函数名字相同，是不是要重新改名字 */

VOS_UINT32 atSetCsqCnfSameProc(struct MsgCB *msgBlock)
{
    L4A_CSQ_InfoCnf *setCsqCnf   = NULL;
    VOS_UINT8        rssiValue   = 0;
    VOS_UINT8        channalQual = 0;

    VOS_UINT16 length = 0;

    setCsqCnf = (L4A_CSQ_InfoCnf *)msgBlock;

    if (setCsqCnf->errorCode == ERR_MSP_SUCCESS) {
        if (setCsqCnf->rssi == AT_RSSI_UNKNOWN) {
            rssiValue = AT_RSSI_UNKNOWN;
        } else if (setCsqCnf->rssi >= AT_RSSI_HIGH) {
            rssiValue = AT_CSQ_RSSI_HIGH;
        } else if (setCsqCnf->rssi <= AT_RSSI_LOW) {
            rssiValue = AT_CSQ_RSSI_LOW;
        } else {
            rssiValue = (VOS_UINT8)((setCsqCnf->rssi - AT_RSSI_LOW) / 2);
        }

        channalQual = AT_BER_UNKNOWN;
    } else {
        rssiValue   = AT_RSSI_UNKNOWN;
        channalQual = AT_BER_UNKNOWN;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "%s: %d,%d%s", "+CSQ", rssiValue, channalQual, g_atCrLf);

    CmdErrProc((VOS_UINT8)(setCsqCnf->clientId), ERR_MSP_SUCCESS, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atCerssiInfoIndProc(struct MsgCB *msgBlock)
{
    L4A_CSQ_INFO_IND_STRU *cerssi = NULL;
    VOS_UINT16             length = 0;

    cerssi = (L4A_CSQ_INFO_IND_STRU *)msgBlock;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "%s%s0,0,255,%d,%d,%d,%d,%d,%d%s", g_atCrLf, "^CERSSI:", cerssi->rsrp,
        cerssi->rsrq, cerssi->sinr, cerssi->cqi.ri, cerssi->cqi.cqi[0], cerssi->cqi.cqi[1], g_atCrLf);

    At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddr, length);

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atCerssiInfoCnfProc(struct MsgCB *msgBlock)
{
    L4A_CSQ_InfoCnf *cerssi   = NULL;
    VOS_UINT16       length   = 0;
    AT_ModemNetCtx  *netCtx   = VOS_NULL_PTR;
    VOS_UINT8        indexNum = 0;

    cerssi = (L4A_CSQ_InfoCnf *)msgBlock;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cerssi->clientId, &indexNum) == AT_FAILURE) {
        AT_PR_LOGI("WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (cerssi->errorCode == 0) {
        length = (VOS_UINT32)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr, "%s%s%d,%d,0,0,255,%d,%d,%d,%d,%d,%d%s", g_atCrLf, "^CERSSI:",
            netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval, cerssi->rsrp, cerssi->rsrq, cerssi->sinr,
            cerssi->cqi.ri, cerssi->cqi.cqi[0], cerssi->cqi.cqi[1], g_atCrLf);
    }

    CmdErrProc((VOS_UINT8)(cerssi->clientId), cerssi->errorCode, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}


VOS_UINT32 atLwclashCnfProc(struct MsgCB *msgBlock)
{
    L4A_READ_LwclashCnf *lwclash = NULL;
    VOS_UINT16           length;

    lwclash = (L4A_READ_LwclashCnf *)msgBlock;

    length = (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr,
        "^LWCLASH: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
        lwclash->lwclashInfo.state, lwclash->lwclashInfo.ulFreq, lwclash->lwclashInfo.ulBandwidth,
        lwclash->lwclashInfo.dlFreq, lwclash->lwclashInfo.dlBandwidth, lwclash->lwclashInfo.band, lwclash->scellNum,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlBandwidth, lwclash->lwclashInfo.dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlMimo, lwclash->dl256QamFlag);

    CmdErrProc((VOS_UINT8)(lwclash->clientId), lwclash->errorCode, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}

/*
 * 功能描述: ^LCACELL命令应答处理
 * 修改历史:
 */
VOS_UINT32 atLcacellCnfProc(struct MsgCB *msgBlock)
{
    L4A_READ_LcacellCnf *lcacell = NULL;
    VOS_UINT16           length  = 0;
    VOS_UINT32           statCnt = 0;

    lcacell = (L4A_READ_LcacellCnf *)msgBlock;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^LCACELL: ");

    for (statCnt = 0; statCnt < CA_MAX_CELL_NUM; statCnt++) {
        length +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                (VOS_CHAR *)g_atSndCodeAddr + length, "\"%d %d %d %d\",", statCnt,
                lcacell->lcacellInfo[statCnt].ulConfigured, lcacell->lcacellInfo[statCnt].dlConfigured,
                lcacell->lcacellInfo[statCnt].actived);
    }

    length--;

    CmdErrProc((VOS_UINT8)(lcacell->clientId), lcacell->errorCode, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atQryCellIdCnfProc(struct MsgCB *msgBlock)
{
    L4A_READ_CellIdCnf *cnf = NULL;
    VOS_UINT16          length;

    cnf = (L4A_READ_CellIdCnf *)msgBlock;

    /* MNC：用于识别移动用户所归属的移动通信网，2~3位数字组成 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr,
        ((cnf->mncNum == AT_MNC_MIN_NUM) ? "%s^CECELLID: %03x%02x,%d,%d,%d%s" : "%s^CECELLID: %03x%03x,%d,%d,%d%s"),
        g_atCrLf, cnf->mcc, (cnf->mncNum == AT_MNC_MIN_NUM) ? (cnf->mnc & 0xff) : cnf->mnc, cnf->ci, cnf->pci,
        cnf->tac, g_atCrLf);

    CmdErrProc((VOS_UINT8)(cnf->clientId), ERR_MSP_SUCCESS, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atQryCellInfoCnfProc(struct MsgCB *msgBlock)
{
    L4A_READ_CellInfoCnf *pstcnf = VOS_NULL_PTR;
    VOS_UINT32            i;
    VOS_UINT32            cellNum;
    VOS_UINT16            length;

    i       = 0;
    cellNum = 0;
    length  = 0;

    pstcnf = (L4A_READ_CellInfoCnf *)msgBlock;

    if (pstcnf->errorCode != 0) {
        CmdErrProc((VOS_UINT8)(pstcnf->clientId), pstcnf->errorCode, 0, NULL);

        return AT_FW_CLIENT_STATUS_READY;
    }

    if (pstcnf->ncellListInfo.cellFlag == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "%d:%d,%d,%d%s", 1, EN_SERVICE_CELL_ID,
            pstcnf->ncellListInfo.sevCellInfo.cellMeasInfo[0].measRslt.phyCellId,
            pstcnf->ncellListInfo.sevCellInfo.cellMeasInfo[0].measRslt.rssi, g_atCrLf);
    } else {
        /* 同频 */
        pstcnf->ncellListInfo.intraFreqNcellList.ncellNumber = AT_MIN(
            pstcnf->ncellListInfo.intraFreqNcellList.ncellNumber,
            LMAX_NEIGHBOR_CELL_NUM);
        for (i = 0; i < pstcnf->ncellListInfo.intraFreqNcellList.ncellNumber; i++) {
            cellNum++;
            length +=
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                    (VOS_CHAR *)g_atSndCodeAddr + length, "%d:%d,%d,%d%s", cellNum, EN_SYN_FREQ_CELL_ID,
                    pstcnf->ncellListInfo.intraFreqNcellList.cellMeasInfo[i].measRslt.phyCellId,
                    pstcnf->ncellListInfo.intraFreqNcellList.cellMeasInfo[i].measRslt.rssi, g_atCrLf);
        }
        /* 异频 */
        pstcnf->ncellListInfo.interFreqNcellList.ncellNumber = AT_MIN(
            pstcnf->ncellListInfo.interFreqNcellList.ncellNumber,
            LMAX_NEIGHBOR_CELL_NUM);
        for (i = 0; i < pstcnf->ncellListInfo.interFreqNcellList.ncellNumber; i++) {
            cellNum++;
            length +=
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                    (VOS_CHAR *)g_atSndCodeAddr + length, "%d:%d,%d,%d%s", cellNum, EN_ASYN_FREQ_CELL_ID,
                    pstcnf->ncellListInfo.interFreqNcellList.cellMeasInfo[i].measRslt.phyCellId,
                    pstcnf->ncellListInfo.interFreqNcellList.cellMeasInfo[i].measRslt.rssi, g_atCrLf);
        }
        /* W异频 */
        pstcnf->ncellListInfo.interRATUMTSNcellList.ncellNumber = AT_MIN(
            pstcnf->ncellListInfo.interRATUMTSNcellList.ncellNumber,
            LMAX_NEIGHBOR_CELL_NUM);
        for (i = 0; i < pstcnf->ncellListInfo.interRATUMTSNcellList.ncellNumber; i++) {
            cellNum++;
            length +=
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                    (VOS_CHAR *)g_atSndCodeAddr + length, "%d:%d,%d,%d%s", cellNum, EN_ASYN_UMTS_CELL_ID,
                    pstcnf->ncellListInfo.interRATUMTSNcellList.umtsNcellList[i].primaryScramCode,
                    pstcnf->ncellListInfo.interRATUMTSNcellList.umtsNcellList[i].cpichRscp, g_atCrLf);
        }
        /* G异频 */
        pstcnf->ncellListInfo.interRATGeranNcellList.ncellNumber = AT_MIN(
            pstcnf->ncellListInfo.interRATGeranNcellList.ncellNumber,
            LMAX_NEIGHBOR_CELL_NUM);
        for (i = 0; i < pstcnf->ncellListInfo.interRATGeranNcellList.ncellNumber; i++) {
            cellNum++;
            length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                (VOS_CHAR *)g_atSndCodeAddr + length, "%d:%d,%d,%d%s", cellNum, EN_ASYN_GSM_CELL_ID,
                (pstcnf->ncellListInfo.interRATGeranNcellList.geranNcellList[i].bsic.ncc) * AT_HEX_BASE_NUM *
                AT_HEX_BASE_NUM + pstcnf->ncellListInfo.interRATGeranNcellList.geranNcellList[i].bsic.bcc,
                pstcnf->ncellListInfo.interRATGeranNcellList.geranNcellList[i].rssi, g_atCrLf);
        }

        if (cellNum == 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                (VOS_CHAR *)g_atSndCodeAddr + length, "%d%s", cellNum, g_atCrLf);
        }
    }
    CmdErrProc((VOS_UINT8)(pstcnf->clientId), pstcnf->errorCode, length, g_atSndCodeAddr);

    return AT_FW_CLIENT_STATUS_READY;
}


VOS_UINT32 atLwclashInd(struct MsgCB *msgBlock)
{
    L4A_READ_LwclashInd *lwclash = NULL;
    VOS_UINT16           length;
    VOS_UINT8            indexNum = 0;

    lwclash = (L4A_READ_LwclashInd *)msgBlock;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserBroadCastId(lwclash->clientId, &indexNum) != AT_SUCCESS) {
        AT_WARN_LOG("atLwclashInd: At_ClientIdToUserBroadCastId is err!");
        return VOS_ERR;
    }

    length = (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr,
        "%s^LWURC: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
        lwclash->lwclashInfo.state, lwclash->lwclashInfo.ulFreq, lwclash->lwclashInfo.ulBandwidth,
        lwclash->lwclashInfo.dlFreq, lwclash->lwclashInfo.dlBandwidth, lwclash->lwclashInfo.band, lwclash->scellNum,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].ulFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].ulBandwidth,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlFreq,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlBandwidth, lwclash->lwclashInfo.dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_0].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_1].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_2].dlMimo,
        lwclash->scellInfo[AT_COMMAND_PARA_INDEX_3].dlMimo, lwclash->dl256QamFlag, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddr, length);

    return AT_FW_CLIENT_STATUS_READY;
}


VOS_UINT32 atLcacellInd(struct MsgCB *msgBlock)
{
    L4A_READ_LcacellInd *lcacell = NULL;
    VOS_UINT16           length  = 0;
    VOS_UINT32           statCnt = 0;

    lcacell = (L4A_READ_LcacellInd *)msgBlock;

    /* 此命令头尾添加\r\n */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "%s^LCACELLURC: ", g_atCrLf);

    for (statCnt = 0; statCnt < CA_MAX_CELL_NUM; statCnt++) {
        if (statCnt != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                (VOS_CHAR *)g_atSndCodeAddr + length, ",");
        }

        length +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
                (VOS_CHAR *)g_atSndCodeAddr + length, "\"%d %d %d %d\"", statCnt,
                lcacell->lcacellInfo[statCnt].ulConfigured, lcacell->lcacellInfo[statCnt].dlConfigured,
                lcacell->lcacellInfo[statCnt].actived);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "%s", g_atCrLf);

    At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddr, length);

    return AT_FW_CLIENT_STATUS_READY;
}


VOS_INT16 AT_GetValidValue(VOS_INT16 curValue, VOS_INT16 minValue, VOS_INT16 maxValue)
{
    VOS_INT16 validValue;

    if (curValue > maxValue) {
        validValue = maxValue;
    } else if (curValue < minValue) {
        validValue = minValue;
    } else {
        validValue = curValue;
    }
    return validValue;
}


VOS_UINT32 atScellInfoQryCnfProc(struct MsgCB *msgBlock)
{
    L4A_READ_ScellInfoCnf *scellInfoCnf = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT32             i;
    VOS_INT16              rssi;
    VOS_INT16              rsrp;
    VOS_INT16              rsrq;

    length = 0;
    scellInfoCnf = (L4A_READ_ScellInfoCnf *)msgBlock;

    if (scellInfoCnf->errorCode != AT_SUCCESS) {
        CmdErrProc((VOS_UINT8)(scellInfoCnf->clientId), scellInfoCnf->errorCode, 0, NULL);
        return AT_FW_CLIENT_STATUS_READY;
    }

    for (i = 0; i < scellInfoCnf->sCellCnt; i++) {
        /* 不是首次进入，需要打印回车换行 */
        if (length != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "%s", g_atCrLf);
        }

        /* 如果 RSSI\RSRP\RSRQ都为0直接上传不处理 */
        if ((scellInfoCnf->sCellInfo[i].rssi == 0) &&
            (scellInfoCnf->sCellInfo[i].rsrp == 0) &&
            (scellInfoCnf->sCellInfo[i].rsrq == 0)) {
            rssi = scellInfoCnf->sCellInfo[i].rssi;
            rsrp = scellInfoCnf->sCellInfo[i].rsrp;
            rsrq = scellInfoCnf->sCellInfo[i].rsrq;
        } else {
            rssi = AT_GetValidValue(scellInfoCnf->sCellInfo[i].rssi,
                                    AT_CASCELL_RSSI_VALUE_MIN,
                                    AT_CASCELL_RSSI_VALUE_MAX);
            rsrp = AT_GetValidValue(scellInfoCnf->sCellInfo[i].rsrp,
                                    AT_CASCELL_RSRP_VALUE_MIN,
                                    AT_CASCELL_RSRP_VALUE_MAX);
            rsrq = AT_GetValidValue(scellInfoCnf->sCellInfo[i].rsrq,
                                    AT_CASCELL_RSRQ_VALUE_MIN,
                                    AT_CASCELL_RSRQ_VALUE_MAX);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
            (VOS_CHAR *)g_atSndCodeAddr + length, "^CASCELLINFO: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            scellInfoCnf->sCellInfo[i].scellIndex, scellInfoCnf->sCellInfo[i].phyCellId,
            rssi, rsrp, rsrq, scellInfoCnf->sCellInfo[i].freqBandIndicator,scellInfoCnf->sCellInfo[i].scellUlFreqPoint,
            scellInfoCnf->sCellInfo[i].scellDlFreqPoint,scellInfoCnf->sCellInfo[i].scellUlFreq,
            scellInfoCnf->sCellInfo[i].scellDlFreq,scellInfoCnf->sCellInfo[i].scellUlBandWidth,
            scellInfoCnf->sCellInfo[i].scellDlBandWidth);
    }

    CmdErrProc((VOS_UINT8)(scellInfoCnf->clientId), scellInfoCnf->errorCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

typedef VOS_UINT32 (*AT_L4A_MSG_FUN)(struct MsgCB *pMsgBlock);

/*lint -e958 原因:只在A核使用 */
typedef struct {
    VOS_UINT32     msgId;
    AT_L4A_MSG_FUN atL4aMsgProc;
} AT_L4A_MSG_FUN_TABLE_STRU;
/*lint +e958 */
static const AT_L4A_MSG_FUN_TABLE_STRU g_atL4aCnfMsgFunTable[] = {
    { ID_MSG_L4A_CSQ_INFO_CNF, atSetCsqCnfSameProc },   { ID_MSG_L4A_ANQUERY_INFO_CNF, atSetAnlevelCnfSameProc },
    { ID_MSG_L4A_CELL_ID_CNF, atQryCellIdCnfProc },     { ID_MSG_L4A_LWCLASHQRY_CNF, atLwclashCnfProc },
    { ID_MSG_L4A_RADVER_SET_CNF, atSetRadverCnfProc },  { ID_MSG_L4A_CELL_INFO_CNF, atQryCellInfoCnfProc },
    { ID_MSG_L4A_CERSSI_INQ_CNF, atCerssiInfoCnfProc }, { ID_MSG_L4A_LCACELLQRY_CNF, atLcacellCnfProc },
    { ID_MSG_L4A_LCASCELLINFO_QRY_CNF, atScellInfoQryCnfProc },
};

static const AT_L4A_MSG_FUN_TABLE_STRU g_atL4aIndMsgFunTable[] = {
    { ID_MSG_L4A_RSSI_IND, atCsqInfoIndProc },      { ID_MSG_L4A_ANLEVEL_IND, atAnlevelInfoIndProc },
    { ID_MSG_L4A_CERSSI_IND, atCerssiInfoIndProc },
    { ID_MSG_L4A_LWCLASH_IND, atLwclashInd },       { ID_MSG_L4A_LCACELL_IND, atLcacellInd },
};

/* ***************************************************************************** */

/* 函数名称: atL4aGetMsgFun */

/* 功能描述: 获取处理L4A回复消息的函数 */

/*  */

/* 参数说明: ulMsgId，消息ID */

/*  */

/* 返 回 值: */

/*  */

/* 调用要求: TODO: ... */

/* 调用举例: TODO: ... */


/* ***************************************************************************** */

AT_L4A_MSG_FUN_TABLE_STRU* atL4aGetCnfMsgFun(VOS_UINT32 msgId)
{
    VOS_UINT32                       i = 0;
    VOS_UINT32                       tableLen;
    const AT_L4A_MSG_FUN_TABLE_STRU *table;

    table    = g_atL4aCnfMsgFunTable;
    tableLen = sizeof(g_atL4aCnfMsgFunTable);

    for (i = 0; i < (tableLen / sizeof(AT_L4A_MSG_FUN_TABLE_STRU)); i++) {
        if (msgId == (table + i)->msgId) {
            return (AT_L4A_MSG_FUN_TABLE_STRU *)(table + i);
        }
    }

    /* 如果找不到 */

    return NULL;
}

/* ***************************************************************************** */

/* 函数名称: atL4aGetIndMsgFun */

/* 功能描述: 获取处理L4A回复消息的函数 */

/*  */

/* 参数说明: ulMsgId，消息ID */

/*  */

/* 返 回 值: */

/*  */

/* 调用要求: TODO: ... */

/* 调用举例: TODO: ... */


/* ***************************************************************************** */

AT_L4A_MSG_FUN_TABLE_STRU* atL4aGetIndMsgFun(VOS_UINT32 msgId)
{
    VOS_UINT32                       i = 0;
    VOS_UINT32                       tableLen;
    const AT_L4A_MSG_FUN_TABLE_STRU *table;

    table    = g_atL4aIndMsgFunTable;
    tableLen = sizeof(g_atL4aIndMsgFunTable);

    for (i = 0; i < (tableLen / sizeof(AT_L4A_MSG_FUN_TABLE_STRU)); i++) {
        if (msgId == (table + i)->msgId) {
            return (AT_L4A_MSG_FUN_TABLE_STRU *)(table + i);
        }
    }

    /* 如果找不到 */

    return NULL;
}



/* ***************************************************************************** */

/* 函数名称: at_L4aCnfProc */

/* 功能描述: 接收L4A的CNF，IND消息，并查找对应函数进行处理 */

/*  */

/* 参数说明: pMsgBlockTmp，VOS消息 */

/*  */

/* 返 回 值: */

/*  */

/* 调用要求: TODO: ... */

/* 调用举例: TODO: ... */


/* ***************************************************************************** */

VOS_VOID at_L4aCnfProc(struct MsgCB *msgBlockTmp)
{
    VOS_UINT32                 msgId;
    VOS_UINT16                 indexNum  = 0;
    AT_L4A_MSG_FUN_TABLE_STRU *table     = NULL;
    AT_L4A_MSG_FUN_TABLE_STRU *table2    = NULL;
    AT_L4A_MSG_FUN             msgProc   = NULL;
    L4A_AT_CnfHeader          *msgHeader = VOS_NULL_PTR;

    msgHeader = (L4A_AT_CnfHeader *)msgBlockTmp;
    msgId     = msgHeader->msgId;

    if (At_ClientIdToUserId((VOS_UINT16)msgHeader->clientId, (TAF_UINT8 *)&indexNum) == AT_FAILURE) {
        return;
    }

    msgHeader->clientId = indexNum;
    table               = atL4aGetCnfMsgFun(msgId);
    table2              = atL4aGetIndMsgFun(msgId);
    if ((table != NULL) && (table->atL4aMsgProc)) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        msgProc = table->atL4aMsgProc;
    } else if ((table2 != NULL) && (table2->atL4aMsgProc)) {
        msgProc = table2->atL4aMsgProc;
    } else {
        ;
    }

    if (msgProc != NULL) {
        msgProc(msgBlockTmp);
    }

    return;
}

static const AT_FTM_CnfMsgProc g_lteAtFtmCnfMsgTbl[] = {
    { ID_MSG_SYM_SET_TMODE_CNF, atSetTmodeParaCnfProc },
    { ID_MSG_FTM_SET_FCHAN_CNF, atSetFCHANParaCnfProc },
    { ID_MSG_FTM_RD_FCHAN_CNF, atQryFCHANParaCnfProc },
    { ID_MSG_FTM_SET_TXON_CNF, atSetFTXONParaCnfProc },
    { ID_MSG_FTM_RD_TXON_CNF, atQryFTXONParaCnfProc },
    { ID_MSG_FTM_SET_RXON_CNF, atSetFRXONParaCnfProc },
    { ID_MSG_FTM_RD_RXON_CNF, atQryFRXONParaCnfProc },
    { ID_MSG_FTM_SET_FWAVE_CNF, atSetFWAVEParaCnfProc },
    { ID_MSG_FTM_SET_TSELRF_CNF, atSetTselrfParaCnfProc }, /* TSELRF 已实现未联调 */
    { ID_MSG_FTM_SET_AAGC_CNF, atSetFLNAParaCnfProc },
    { ID_MSG_FTM_RD_AAGC_CNF, atQryFLNAParaCnfProc },
    { ID_MSG_FTM_FRSSI_CNF, atQryFRSSIParaCnfProc },
    { ID_MSG_FTM_SET_TBAT_CNF, atSetTbatCnf },
    { ID_MSG_FTM_RD_TBAT_CNF, atRdTbatCnf },
    { ID_MSG_FTM_SET_SDLOAD_CNF, atSetSdloadCnf },
    { ID_MSG_FTM_SET_LTCOMMCMD_CNF, atSetLTCommCmdParaCnfProc },
    { ID_MSG_FTM_RD_LTCOMMCMD_CNF, atQryLTCommCmdParaCnfProc },
    { ID_MSG_FTM_RD_FPLLSTATUS_CNF, atQryFPllStatusParaCnfProc },
};

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
static const AT_FTM_IndMsgProc g_lteAtFtmIndMsgTbl[] = {
    { ID_MSG_FTM_TX_CLT_INFO_IND, At_ProcLteTxCltInfoReport },
};
#endif

/* ***************************************************************************** */

/* 函数名称: atGetFtmCnfMsgProc */

/* 功能描述: 根据回复的消息找到匹配的处理函数 */

/*  */

/* 参数说明: */

/*   MsgId  [in] 消息ID */

/* 返 回 值: */

/*    非NULL g_lteAtFtmCnfMsgTbl 元素地址，包含了处理函数 */

/*    NULL 匹配失败 */


/* ***************************************************************************** */
const AT_FTM_CnfMsgProc* atGetFtmCnfMsgProc(VOS_UINT32 msgId)
{
    VOS_UINT32 i        = 0;
    VOS_UINT32 tableLen = sizeof(g_lteAtFtmCnfMsgTbl) / sizeof(g_lteAtFtmCnfMsgTbl[0]);

    for (i = 0; i < tableLen; i++) {
        if (g_lteAtFtmCnfMsgTbl[i].msgId == msgId) {
            return &(g_lteAtFtmCnfMsgTbl[i]);
        }
    }

    return NULL;
}


const AT_FTM_IndMsgProc* At_GetFtmIndMsgProc(VOS_UINT32 msgId)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    VOS_UINT32 i;
    VOS_UINT32 tableLen;

    tableLen = sizeof(g_lteAtFtmIndMsgTbl) / sizeof(g_lteAtFtmIndMsgTbl[0]);

    for (i = 0; i < tableLen; i++) {
        if (g_lteAtFtmIndMsgTbl[i].msgId == msgId) {
            return &(g_lteAtFtmIndMsgTbl[i]);
        }
    }
#endif
    return VOS_NULL_PTR;
}

/* ***************************************************************************** */

/* 函数名称: atGetFtmCnfMsgProc */

/* 功能描述: AT处理LTE装备FTM回复消息入口 */

/*  */

/* 参数说明: */

/*   pMsg  [in] 核间消息结构 */

/* 返 回 值: */

/*    ERR_MSP_SUCCESS 成功 */

/*    ERR_MSP_FAILURE 失败 */


/* ***************************************************************************** */
VOS_VOID At_FtmEventMsgProc(struct MsgCB *msg)
{
    OS_MSG                  *osMsg       = NULL;
    VOS_UINT8               *msgBlock    = NULL;
    const AT_FTM_CnfMsgProc *msgProcItem = NULL;
    AT_FW_DataMsg           *dataMsg     = (AT_FW_DataMsg *)msg;
    VOS_VOID                *tmp         = VOS_NULL_PTR;
    errno_t                  memResult;

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    const AT_FTM_IndMsgProc *ftmIndMsgItem = VOS_NULL_PTR;
#endif

    msgBlock = VOS_MemAlloc(WUEPS_PID_AT, (DYNAMIC_MEM_PT),
                            (sizeof(MsgBlock) + sizeof(OS_MSG) - AT_MSG_BLOCK_VALUE_LEN));
    if (msgBlock == NULL) {
        return;
    }
    (VOS_VOID)memset_s(msgBlock,
        (VOS_SIZE_T)(sizeof(MsgBlock) + sizeof(OS_MSG) - AT_MSG_BLOCK_VALUE_LEN), 0x00,
        (VOS_SIZE_T)(sizeof(MsgBlock) + sizeof(OS_MSG) - AT_MSG_BLOCK_VALUE_LEN));

    /* 消息结构转换 */
    VOS_SET_SENDER_ID(msgBlock, VOS_GET_SENDER_ID(dataMsg));
    osMsg         = (OS_MSG *)(((MsgBlock *)(msgBlock))->value);
    osMsg->msgId  = dataMsg->msgId;
    osMsg->param2 = dataMsg->len;
    osMsg->param3 = VOS_GET_RECEIVER_ID(dataMsg);

    if (dataMsg->len == 0) {
        VOS_MemFree(WUEPS_PID_AT, msgBlock);
        AT_WARN_LOG("tmp cannot malloc space !");
        return;
    }
    tmp = VOS_MemAlloc(WUEPS_PID_AT, (DYNAMIC_MEM_PT), dataMsg->len);
    if (tmp == NULL) {
        VOS_MemFree(WUEPS_PID_AT, msgBlock);
        return;
    }
    (VOS_VOID)memset_s(tmp, dataMsg->len, 0x00, dataMsg->len);

    osMsg->param1 = tmp;

    memResult = memcpy_s((VOS_UINT8 *)(osMsg->param1), dataMsg->len, dataMsg->context, dataMsg->len);
    TAF_MEM_CHK_RTN_VAL(memResult, dataMsg->len, dataMsg->len);

    /* 消息处理 */
    msgProcItem = atGetFtmCnfMsgProc(dataMsg->msgId);
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    ftmIndMsgItem = At_GetFtmIndMsgProc(dataMsg->msgId);
#endif

    if (msgProcItem != NULL) {
        AT_STOP_TIMER_CMD_READY(dataMsg->clientId);
        msgProcItem->cnfMsgProc((VOS_UINT8)(dataMsg->clientId), (struct MsgCB *)msgBlock);
    }
#if (FEATURE_UE_MODE_NR == FEATURE_OFF) /* only CLT, not support NR MODE */
    else if (ftmIndMsgItem != VOS_NULL_PTR) {
        ftmIndMsgItem->indMsgProc((struct MsgCB *)msgBlock);
    }
#endif
    else {
        ;
    }

    VOS_MemFree(WUEPS_PID_AT, tmp);
    VOS_MemFree(WUEPS_PID_AT, msgBlock);
    return;
}

/*
 * 功能描述: 处理消息ID_TEMPPRT_AT_EVENT_IND
 * 修改历史:
 */
VOS_UINT32 AT_ProcTempprtEventInd(TEMP_PROTECT_EventAtInd *msg)
{
    VOS_UINT16               length;
    TEMP_PROTECT_EventAtInd *tempPrt = VOS_NULL;

    tempPrt = (TEMP_PROTECT_EventAtInd *)msg;

    HAL_SDMLOG("AT_ProcTempprtEventInd: Event %d Param %d\n", (VOS_INT)tempPrt->tempProtectEvent,
               (VOS_INT)tempPrt->tempProtectParam);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr, "%s%s%d,%d%s", g_atCrLf, "^TEMPPRT:", tempPrt->tempProtectEvent,
        tempPrt->tempProtectParam, g_atCrLf);

    At_SendResultData(AT_CLIENT_ID_APP, g_atSndCodeAddr, length);

    return VOS_OK;
}
/*lint -restore*/

