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

#include "at_lte_ms_proc.h"
#include "securec.h"
#include "osm.h"

#include "at_lte_common.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "nv_stru_lps.h"
#endif

#include "AtDeviceCmd.h"

/*lint -e767 原因:Log打印*/
#define THIS_FILE_ID MSP_FILE_ID_AT_LTE_MS_PROC_C
/*lint +e767 */
#define AT_NVRDEX_PARA_NUM 3
#define AT_NVRDEX_NVID 0
#define AT_NVRDEX_OFFSET 1
#define AT_NVRDEX_RDLEN 2
#define AT_NVWREX_NVID 0
#define AT_NVWREX_OFFFSET 1
#define AT_NVWREX_LENGTH 2
#define AT_NVWREX_DATA 3
#define AT_RADVER_PARA_NUM 2
#define AT_RADVER_MOD 0
#define AT_RADVER_VER 1
#define AT_DATA_MAX_LENGTH 128

VOS_UINT32 g_nvReadTl  = 0;
VOS_UINT32 g_nvWriteTl = 0;


// *****************************************************************************
// oˉêy??3?: atSetNVRDLenPara
// 1|?ü?èê?: ?áNV3¤?èY
// 2?êy?μ?÷:
//   ulIndex [in] ó??§?÷òy
// ·μ ?? ?μ:
//    TODO: ...
// μ÷ó?òa?ó: TODO: ...
// μ÷ó??ùày: TODO: ...
// ×÷    ??: óú???1?/00193980 [2013-02-24]
// *****************************************************************************
VOS_UINT32 atSetNVRDLenPara(VOS_UINT8 clientId)
{
    VOS_UINT32 ret;
    VOS_UINT16 nvid  = 0;
    VOS_UINT32 nVLen = 0;

    /* PCLINT */
    if (clientId == 0) {
    }

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        g_nvReadTl = 1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex > 1) {
        g_nvReadTl = 2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        g_nvReadTl = 3;
        return AT_CME_INCORRECT_PARAMETERS;
    } else {
        nvid = (VOS_UINT16)g_atParaList[0].paraValue;
    }
    ret = TAF_ACORE_NV_GET_LENGTH(nvid, &nVLen);

    if (ret != ERR_MSP_SUCCESS) {
        g_nvReadTl = 4;
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^NVRDLEN: %d", nVLen);
    g_nvReadTl              = 7;
    return AT_OK;
}

VOS_UINT32 atSetNVRDExPara(VOS_UINT8 clientId)
{
    VOS_UINT32 ret;
    VOS_UINT16 nvid;
    VOS_UINT32 nVLen = 0;
    VOS_UINT16 offset;
    VOS_UINT16 rdLen;
    VOS_UINT8 *pu8Data = NULL;
    VOS_UINT32 i       = 0;

    AT_PR_LOGI("Rcv Msg");

    /* PCLINT */
    if (clientId == 0) {
    }

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        g_nvReadTl = 1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != AT_NVRDEX_PARA_NUM) {
        g_nvReadTl = 2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_NVRDEX_NVID].paraLen == 0 || g_atParaList[AT_NVRDEX_OFFSET].paraLen == 0 ||
        g_atParaList[AT_NVRDEX_RDLEN].paraLen == 0) {
        g_nvReadTl = 3;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    nvid   = (VOS_UINT16)g_atParaList[AT_NVRDEX_NVID].paraValue;
    offset = (VOS_UINT16)g_atParaList[AT_NVRDEX_OFFSET].paraValue;
    rdLen  = (VOS_UINT16)g_atParaList[AT_NVRDEX_RDLEN].paraValue;

    ret = TAF_ACORE_NV_GET_LENGTH(nvid, &nVLen);

    AT_PR_LOGI("Call interface success!");

    if (ret != ERR_MSP_SUCCESS) {
        g_nvReadTl = 4;
        return AT_ERROR;
    }

    if ((((VOS_UINT32)offset + 1) > nVLen) || (((VOS_UINT32)offset + rdLen) > nVLen)) {
        g_nvReadTl = 5;
        return AT_ERROR;
    }

    pu8Data = VOS_MemAlloc(WUEPS_PID_AT, (DYNAMIC_MEM_PT), nVLen);
    if (pu8Data == NULL) {
        g_nvReadTl = 6;
        return AT_ERROR;
    }
    memset_s(pu8Data, nVLen, 0x00, nVLen);

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, (VOS_UINT32)nvid, pu8Data, nVLen);

    if (ret != ERR_MSP_SUCCESS) {
        VOS_MemFree(WUEPS_PID_AT, pu8Data);
        g_nvReadTl = 7;
        return AT_ERROR;
    }
    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^NVRDEX: %d,%d,%d,", nvid, offset, rdLen);

    for (i = offset; i < ((VOS_UINT32)offset + rdLen); i++) {
        if (offset == i) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%02X",
                pu8Data[i]);
        } else {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, " %02X",
                pu8Data[i]);
        }
    }

    VOS_MemFree(WUEPS_PID_AT, pu8Data);
    g_nvReadTl = 8;
    return AT_OK;
}



VOS_UINT32 AT_QryLwclashPara(VOS_UINT8 indexNum)
{
    VOS_UINT32                ret;
    L4A_READ_LWCLASH_REQ_STRU lwclash = {0};

    lwclash.ctrl.clientId = g_atClientTab[indexNum].clientId;

    lwclash.ctrl.opId = 0;
    lwclash.ctrl.pid  = WUEPS_PID_AT;

    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_LWCLASHQRY_REQ,
                           (VOS_UINT8 *)(&lwclash), sizeof(L4A_READ_LWCLASH_REQ_STRU));

    if (ret == ERR_MSP_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = (AT_CmdCurrentOpt)AT_CMD_LWCLASH_READ;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}


VOS_UINT32 AT_QryLcacellPara(VOS_UINT8 indexNum)
{
    VOS_UINT32                ret;
    L4A_READ_LCACELL_REQ_STRU lcacell = {0};

    lcacell.ctrl.clientId = g_atClientTab[indexNum].clientId;

    lcacell.ctrl.opId = 0;
    lcacell.ctrl.pid  = WUEPS_PID_AT;

    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_LCACELLQRY_REQ,
                           (VOS_UINT8 *)(&lcacell), sizeof(L4A_READ_LCACELL_REQ_STRU));

    if (ret == ERR_MSP_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = (AT_CmdCurrentOpt)AT_CMD_LCACELL_QUERY;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}

VOS_UINT32 AT_SetRadverPara(VOS_UINT8 indexNum)
{
    VOS_UINT32        ret;
    L4A_SET_RadverReq req = {0};

    if ((g_atParaIndex != AT_RADVER_PARA_NUM) || (g_atParaList[AT_RADVER_MOD].paraLen == 0) ||
        (g_atParaList[AT_RADVER_VER].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    req.ctrl.clientId = g_atClientTab[indexNum].clientId;
    req.ctrl.opId     = 0;
    req.ctrl.pid      = WUEPS_PID_AT;

    req.mod = g_atParaList[AT_RADVER_MOD].paraValue;
    req.ver = g_atParaList[AT_RADVER_VER].paraValue;

    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_RADVER_SET_REQ,
                           (VOS_UINT8 *)(&req), sizeof(req));

    if (ret == ERR_MSP_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = (AT_CmdCurrentOpt)AT_CMD_RADVER_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atSetRadverCnfProc(struct MsgCB *msgBlock)
{
    L4A_SET_RadverCnf *cnf = NULL;

    cnf = (L4A_SET_RadverCnf *)msgBlock;

    CmdErrProc((VOS_UINT8)(cnf->clientId), cnf->errorCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 At_QryCellIdPara(VOS_UINT8 indexNum)
{
    VOS_UINT32                ret;
    L4A_READ_CELL_ID_REQ_STRU req = {0};

    req.ctrl.clientId = g_atClientTab[indexNum].clientId;
    req.ctrl.opId     = 0;
    req.ctrl.pid      = WUEPS_PID_AT;
    ret               = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_CELL_ID_REQ,
                                         (VOS_UINT8 *)(&req), sizeof(L4A_READ_CELL_ID_REQ_STRU));

    if (ret == ERR_MSP_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CELL_ID_READ;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}

VOS_UINT32 At_SetLFromConnToIdlePara(VOS_UINT8 indexNum)
{
    VOS_UINT32                   ret;
    L4A_SET_LTE_TO_IDLE_REQ_STRU req            = {0};
    LPS_SWITCH_PARA_STRU         drxControlFlag = {0};

    req.ctrl.clientId = g_atClientTab[indexNum].clientId;
    req.ctrl.opId     = 0;
    req.ctrl.pid      = WUEPS_PID_AT;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_ACORE_NV_READ(MODEM_ID_0, EN_NV_ID_SWITCH_PARA, &drxControlFlag, sizeof(LPS_SWITCH_PARA_STRU)) != NV_OK) {
        return AT_ERROR;
    }

    if (((*((VOS_UINT32 *)(&(drxControlFlag.stPsFunFlag01)))) & LPS_NV_JP_DCOM_CON_TO_IDLE_BIT) == 0) {
        return AT_OK;
    }

    /* L模调用如下接口转发 */
    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_LTE_TO_IDLE_REQ,
                           (VOS_UINT8 *)(&req), sizeof(L4A_SET_LTE_TO_IDLE_REQ_STRU));

    if (ret == ERR_MSP_SUCCESS) {
        return AT_OK;
    }
    return AT_ERROR;
}

VOS_UINT32 At_SetLWThresholdCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32              ret;
    L4A_LW_ThreasholdCfgReq req            = {0};
    LPS_SWITCH_PARA_STRU    drxControlFlag = {0};

    req.ctrl.clientId = g_atClientTab[indexNum].clientId;
    req.ctrl.opId     = 0;
    req.ctrl.pid      = WUEPS_PID_AT;
    req.flag          = g_atParaList[0].paraValue;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, EN_NV_ID_SWITCH_PARA, &drxControlFlag, sizeof(LPS_SWITCH_PARA_STRU)) != NV_OK) {
        return AT_ERROR;
    }

    if (((*((VOS_UINT32 *)(&(drxControlFlag.stPsFunFlag01)))) & LPS_NV_JP_DCOM_REL_OFFSET_BIT) == 0) {
        return AT_OK;
    }

    /* L模调用如下接口转发 */
    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_LW_THRESHOLD_REQ,
                           (VOS_UINT8 *)(&req), sizeof(L4A_LW_ThreasholdCfgReq));

    if (ret == ERR_MSP_SUCCESS) {
        return AT_OK;
    }
    return AT_ERROR;
}

VOS_UINT32 AT_SetLFastDormPara(VOS_UINT8 indexNum)
{
    VOS_UINT32           ret;
    L4A_SET_FastDormReq  req            = {0};
    LPS_SWITCH_PARA_STRU drxControlFlag = {0};

    /* 参数检查在AT入口已做 */

    req.ctrl.clientId = g_atClientTab[indexNum].clientId;
    req.ctrl.opId     = 0;
    req.ctrl.pid      = WUEPS_PID_AT;
    req.flag          = (VOS_INT32)g_atParaList[0].paraValue;
    req.timerLen      = (VOS_INT32)g_atParaList[1].paraValue;

    if (TAF_ACORE_NV_READ(MODEM_ID_0, EN_NV_ID_SWITCH_PARA, &drxControlFlag, sizeof(LPS_SWITCH_PARA_STRU)) != NV_OK) {
        return AT_ERROR;
    }

    if (((*((VOS_UINT32 *)(&(drxControlFlag.stPsFunFlag01)))) & LPS_NV_JP_DCOM_FAST_DORM_BIT) == 0) {
        return AT_OK;
    }

    /* L模调用如下接口转发 */
    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_FAST_DORM_REQ,
                           (VOS_UINT8 *)(&req), sizeof(L4A_SET_FastDormReq));

    if (ret == ERR_MSP_SUCCESS) {
        return AT_OK;
    }
    return AT_ERROR;
}

VOS_UINT32 AT_SetLIndCfgReq(VOS_UINT8 indexNum, L4A_IND_Cfg *indCfgReq)
{
    L4A_IND_CfgReq indCfgReqInfo = {0};
    VOS_UINT32     ret;
    errno_t        memResult;

    indCfgReqInfo.ctrl.clientId = g_atClientTab[indexNum].clientId;
    indCfgReqInfo.ctrl.opId     = 0;
    indCfgReqInfo.ctrl.pid      = WUEPS_PID_AT;
    memResult = memcpy_s(&(indCfgReqInfo.indCfg), sizeof(L4A_IND_Cfg), indCfgReq, sizeof(L4A_IND_Cfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(L4A_IND_Cfg), sizeof(L4A_IND_Cfg));

    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_IND_CFG_REQ,
                           (VOS_UINT8 *)(&indCfgReqInfo), sizeof(L4A_IND_CfgReq));
    if (ret == ERR_MSP_SUCCESS) {
        return AT_OK;
    }
    return AT_ERROR;
}


VOS_UINT32 AT_QryCaScellInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32            ret;
    L4A_READ_ScellInfoReq sellInfoReq;

    /* 初始化 */
    (VOS_VOID)memset_s(&sellInfoReq, sizeof(sellInfoReq), 0x00, sizeof(sellInfoReq));
    sellInfoReq.ctrl.clientId = g_atClientTab[indexNum].clientId;
    sellInfoReq.ctrl.opId = 0;
    sellInfoReq.ctrl.pid  = WUEPS_PID_AT;

    ret = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_LCASCELLINFO_QRY_REQ,
                           (VOS_UINT8 *)(&sellInfoReq), sizeof(L4A_READ_ScellInfoReq));

    if (ret == ERR_MSP_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = (AT_CmdCurrentOpt)AT_CMD_CASCELLINFO_READ;
        return AT_WAIT_ASYNC_RETURN;
    }
    return AT_ERROR;
}

