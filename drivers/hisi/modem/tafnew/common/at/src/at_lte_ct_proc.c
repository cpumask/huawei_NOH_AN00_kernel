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

/* ***************************************************************************** */

/* PROJECT   : */

/* SUBSYSTEM : */

/* MODULE    : */

/* OWNER     : */

/* ***************************************************************************** */

#include "at_lte_ct_proc.h"
#include "securec.h"
#include "osm.h"
#include "gen_msg.h"

#include "at_lte_common.h"
/*lint --e{7,64,537,322,958,734,813,718,746,830,438,409}*/
#include "ATCmdProc.h"
#include "AtCmdMsgProc.h"
#include "taf_drv_agent.h"
#include "nv_stru_lps.h"

#include "AtCtx.h"

#include "at_mdrv_interface.h"
#include "AtMtCommFun.h"
/*lint -e767 原因:Log打印*/
#define THIS_FILE_ID MSP_FILE_ID_AT_LTE_CT_PROC_C
/*lint +e767 */
#define AT_FCHAN_LISTENING_PATH_FLG 3
#define AT_TBAT_VALUE 2
#define AT_TBAT_VALUE_MIN 2
#define AT_TBAT_VALUE_MAX 3
#define AT_FCHAN_MAX_PARA_NUM 3
#define AT_FWAVE_MAX_PARA_NUM 2
#define AT_TSELRF_CMD_MAX_LEN 200
#define AT_TBAT_MAX_PARA_NUM 4

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
/* ***************************************************************************** */

/* 功能描述: 打开上行信道 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atSetFTXONPara(VOS_UINT8 clientId)
{
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
    FTM_SetTxonReq     fTXONSetReq  = {
        (FTM_TXON_SWT_ENUM)0,
    };
    VOS_UINT32 rst;

    atDevCmdCtrl = AT_GetDevCmdCtrl();
    // 参数检查

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fTXONSetReq.swtich = (FTM_TXON_SWT_ENUM)(g_atParaList[0].paraValue);
    /* 只有在^FTXON=1 时才下发 */
    if (fTXONSetReq.swtich == EN_FTM_TXON_UL_RFBBP_OPEN) {
        fTXONSetReq.cltEnableFlg = (FTM_CltEnableUint8)atDevCmdCtrl->cltEnableFlg;
    }

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_TXON_REQ, clientId, (VOS_UINT8 *)(&fTXONSetReq),
                           sizeof(fTXONSetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FTXON_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}
#endif

VOS_UINT32 atSetFTXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG         *event = NULL;
    FTM_SetTxonCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetTxonCnf *)event->param1;

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 查询上行信道打开状态 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atQryFTXONPara(VOS_UINT8 clientId)
{
    FTM_RdTxonReq fTXONQryReq = {0};
    VOS_UINT32    rst;


    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_TXON_REQ, clientId, (VOS_UINT8 *)(&fTXONQryReq),
                           sizeof(fTXONQryReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FTXON_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atQryFTXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_RdTxonCnf *cnf   = NULL;
    OS_MSG        *event = NULL;
    VOS_UINT16     length;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdTxonCnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FTXON:%d", cnf->swtich);

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 atQryFPllStatusPara(VOS_UINT8 clientId)
{
    FTM_RdFpllstatusReq fPLLSTATUSQryReq = {0};
    VOS_UINT32          rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_FPLLSTATUS_REQ, clientId, (VOS_UINT8 *)(&fPLLSTATUSQryReq),
                           sizeof(fPLLSTATUSQryReq));
    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = (AT_CmdCurrentOpt)AT_CMD_FPLLSTATUS_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atQryFPllStatusParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_RdFpllstatusCnf *cnf   = NULL;
    OS_MSG              *event = NULL;
    VOS_UINT16           length;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdFpllstatusCnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FPLLSTATUS: %d,%d", cnf->txStatus, cnf->rxStatus);

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 打开下行信道 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atSetFRXONPara(VOS_UINT8 clientId)
{
    FTM_SetRxonReq fRXONSetReq = {0};
    VOS_UINT32     rst;

    /* 参数检查 */

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fRXONSetReq.rxSwt = g_atParaList[0].paraValue;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_RXON_REQ, clientId, (VOS_UINT8 *)(&fRXONSetReq),
                           sizeof(fRXONSetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FRXON_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

VOS_UINT32 atSetFRXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG         *event = NULL;
    FTM_SetRxonCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetRxonCnf *)event->param1;

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 查询下行信道开启状态 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atQryFRXONPara(VOS_UINT8 clientId)
{
    FTM_RdRxonReq fRXONQryReq = {0};
    VOS_UINT32    rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_RXON_REQ, clientId, (VOS_UINT8 *)(&fRXONQryReq),
                           sizeof(fRXONQryReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FRXON_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atQryFRXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_RdRxonCnf *cnf   = NULL;
    OS_MSG        *event = NULL;
    VOS_UINT16     length;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdRxonCnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FRXON:%d", cnf->rxSwt);

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 设置非信令的信道 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atSetFCHANPara(VOS_UINT8 clientId)
{
    FTM_SetFchanReq fCHANSetReq = {
        (FCHAN_MODE_ENUM)0,
    };
    VOS_UINT32 rst;
    /* 参数检查 */

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaIndex != AT_FCHAN_MAX_PARA_NUM) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[0].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[1].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[AT_FCHAN_CHANNEL_NO].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[AT_FCHAN_LISTENING_PATH_FLG].paraLen == 0) {
        g_atParaList[AT_FCHAN_LISTENING_PATH_FLG].paraValue = 0;
    }

    fCHANSetReq.fchanMode = (FCHAN_MODE_ENUM)(g_atParaList[0].paraValue);
    fCHANSetReq.band      = (VOS_UINT8)(g_atParaList[1].paraValue);
    fCHANSetReq.channel   = (g_atParaList[AT_FCHAN_CHANNEL_NO].paraValue);

    fCHANSetReq.listeningPathFlg = (VOS_UINT16)(g_atParaList[AT_FCHAN_LISTENING_PATH_FLG].paraValue);
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    (VOS_VOID)AT_SetGlobalFchan((VOS_UINT8)(g_atParaList[0].paraValue));
#endif

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_FCHAN_REQ, clientId, (VOS_UINT8 *)(&fCHANSetReq),
                           sizeof(fCHANSetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FCHAN_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

VOS_UINT32 atSetFCHANParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG          *event = NULL;
    FTM_SetFchanCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetFchanCnf *)event->param1;


    if (cnf->errCode == ERR_MSP_SUCCESS) {
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
        (VOS_VOID)AT_SetGlobalFchan((VOS_UINT8)(cnf->fchanMode));
#endif
    }

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atSetFWAVEPara(VOS_UINT8 clientId)
{
    VOS_UINT32      rst;
    FTM_SetFwaveReq fWaveSetReq = {0};

    /* 参数检查 */

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaIndex != AT_FWAVE_MAX_PARA_NUM) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[0].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[1].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    fWaveSetReq.type  = g_atParaList[0].paraValue;
    fWaveSetReq.power = g_atParaList[1].paraValue;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_FWAVE_REQ, clientId, (VOS_UINT8 *)(&fWaveSetReq),
                           sizeof(fWaveSetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FWAVE_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

VOS_UINT32 atSetFWAVEParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG          *event = NULL;
    FTM_SetFwaveCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetFwaveCnf *)event->param1;

    CmdErrProc(clientId, cnf->errCode, 0, NULL);
    return AT_FW_CLIENT_STATUS_READY;
}


/* ***************************************************************************** */

/* 功能描述: 查询非信令的信道 */

/* 参数说明: */

/*   ulIndex [in] ... */

/* 返 回 值: */

/*    TODO: ... */
/* ***************************************************************************** */
VOS_UINT32 atQryFCHANPara(VOS_UINT8 clientId)
{
    FTM_RdFchanReq fCHANQryReq = {0};
    VOS_UINT32     rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_FCHAN_REQ, clientId, (VOS_UINT8 *)(&fCHANQryReq),
                           sizeof(fCHANQryReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FCHAN_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

#define AT_CT_FREQ_INVALID_VALUE 65536 /* 36.101 0-65535 */

VOS_UINT32 atQryFCHANParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_RdFchanCnf *cnf    = NULL;
    OS_MSG         *event  = NULL;
    VOS_UINT16      length = 0;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdFchanCnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FCHAN:%d,%d,%u,%u", cnf->fchanMode, cnf->band, cnf->ulChannel,
        cnf->dlChannel);

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: AT^TSELRF  选择射频通路指令 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 AT_SetWifiTselrfPara(VOS_UINT8 path)
{
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    VOS_INT32        bufLen;
    VOS_CHAR         acCmd[AT_TSELRF_CMD_MAX_LEN] = {0};
    VOS_UINT32       rst = ERR_MSP_SUCCESS;


    /*lint -e774*/
    if (path == FTM_TSELRF_WIFI && rst == ERR_MSP_SUCCESS) {
        /*lint +e774*/
        /* WIFI未Enable直接返回失败 */
        if ((VOS_UINT32)AT_GetWifiStatus() == VOS_FALSE) {
            return AT_ERROR;
        }

        if ((g_atParaList[AT_TSELRF_GROUP].paraValue != 0) && (g_atParaList[AT_TSELRF_GROUP].paraValue != 1)) {
            return AT_ERROR;
        }

        AT_SetWifiRF(g_atParaList[AT_TSELRF_GROUP].paraValue);

        bufLen = snprintf_s(acCmd, (VOS_SIZE_T)sizeof(acCmd), (VOS_SIZE_T)(sizeof(acCmd) - 1), "wl txchain %d",
                            (g_atParaList[AT_TSELRF_GROUP].paraValue + 1));
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        bufLen = snprintf_s(acCmd, (VOS_SIZE_T)sizeof(acCmd), (VOS_SIZE_T)(sizeof(acCmd) - 1), "wl rxchain %d",
                            (g_atParaList[AT_TSELRF_GROUP].paraValue + 1));
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        bufLen = snprintf_s(acCmd, (VOS_SIZE_T)sizeof(acCmd), (VOS_SIZE_T)(sizeof(acCmd) - 1), "wl txant %d",
                            g_atParaList[AT_TSELRF_GROUP].paraValue);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        bufLen = snprintf_s(acCmd, (VOS_SIZE_T)sizeof(acCmd), (VOS_SIZE_T)(sizeof(acCmd) - 1), "wl antdiv %d",
                            g_atParaList[AT_TSELRF_GROUP].paraValue);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        return AT_OK;
    }
#endif

    return ERR_MSP_INVALID_PARAMETER;
}

VOS_UINT32 atSetTselrfPara(VOS_UINT8 clientId)
{
    FTM_SetTselrfReq tselrfSetReq = {0};
    VOS_UINT32       rst          = AT_SUCCESS;
    VOS_UINT8        path;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if ((g_atParaIndex != AT_TSELRF_PARA_MIN_NUM) && (g_atParaIndex != AT_TSELRF_PARA_MAX_NUM)) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    if (g_atParaList[AT_TSELRF_PATH_ID].paraLen == 0) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    path = (VOS_UINT8)(g_atParaList[AT_TSELRF_PATH_ID].paraValue);

    if ((path == FTM_TSELRF_FDD_LTE_MAIN) || (path == FTM_TSELRF_TDD_LTE_MAIN) || (path == FTM_TSELRF_FDD_LTE_SUB) ||
        (path == FTM_TSELRF_TDD_LTE_SUB) || (path == FTM_TSELRF_FDD_LTE_MIMO) || (path == FTM_TSELRF_TDD_LTE_MIMO) ||
        (path == FTM_TSELRF_TD)) {
        tselrfSetReq.path  = path;
        tselrfSetReq.group = (VOS_UINT8)g_atParaList[AT_TSELRF_GROUP].paraValue;
    } else {
        return AT_SetWifiTselrfPara(path);
    }

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_TSELRF_REQ, clientId, (VOS_UINT8 *)(&tselrfSetReq),
                           sizeof(tselrfSetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_TSELRF_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return rst;
}

VOS_UINT32 atSetTselrfParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG           *event = NULL;
    FTM_SetTselrfCnf *cnf   = NULL;

    HAL_SDMLOG("\n enter atSetTselrfParaCnfProc !!!\n");

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetTselrfCnf *)event->param1;

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: AT^TSELRF  选择射频通路指令 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atQryTselrfPara(VOS_UINT8 clientId)
{
    /*
     * 平台不提供该接口，由产品线实现。具体使用如下三个NV项获取:
     * 10000 NV_WG_RF_MAIN_BAND
     * 0xD22C NV_ID_UE_CAPABILITY
     * 0xD304 EN_NV_ID_TDS_SUPPORT_FREQ_BAND
     */
    return AT_OK;
}
/* ***************************************************************************** */

/* 功能描述: 设置接收机LNA的等级 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atSetFLNAPara(VOS_UINT8 clientId)
{
    FTM_SetAagcReq fLNASetReq = {0};
    VOS_UINT32     rst;

    /* 参数检查 */

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fLNASetReq.aggcLvl = (VOS_UINT8)(g_atParaList[0].paraValue);

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_AAGC_REQ, clientId, (VOS_UINT8 *)(&fLNASetReq),
                           sizeof(fLNASetReq));

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FLNA_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

VOS_UINT32 atSetFLNAParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG         *event = NULL;
    FTM_SetAagcCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetAagcCnf *)event->param1;

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 查询接收机LNA的等级 */

/*  */

/* 参数说明: */

/*   ulIndex [in] ... */

/*                ... */

/*  */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */

VOS_UINT32 atQryFLNAPara(VOS_UINT8 clientId)
{
    FTM_RdAagcReq fLNAQryReq = {0};
    VOS_UINT32    rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_AAGC_REQ, clientId, (VOS_UINT8 *)(&fLNAQryReq),
                           sizeof(fLNAQryReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FLNA_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atQryFLNAParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_RdAagcCnf *cnf   = NULL;
    OS_MSG        *event = NULL;
    VOS_UINT16     length;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdAagcCnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FLNA:%d", cnf->aggcLvl);

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

/* ***************************************************************************** */

/* 功能描述: 查询RSSI */

/* 参数说明: */

/*   ulIndex [in] ... */

/* 返 回 值: */

/*    TODO: ... */

/* ***************************************************************************** */
VOS_UINT32 atQryFRSSIPara(VOS_UINT8 clientId)
{
    FTM_FRSSI_Req fRssiQryReq = {0};
    VOS_UINT32    rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_FRSSI_REQ, clientId, (VOS_UINT8 *)(&fRssiQryReq),
                           sizeof(fRssiQryReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_FRSSI_READ;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
VOS_UINT32 atQryFRSSIParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG        *event = NULL;
    FTM_FRSSI_Cnf *cnf   = NULL;
    VOS_UINT16     length;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_FRSSI_Cnf *)event->param1;

    if (cnf == NULL) {
        return ERR_MSP_FAILURE;
    }

    length = 0;

    /* 适配V7R5版本4RX接收，GU只报一个值，其他报0，L根据FTM上报结果，支持4RX接收上报4个值，不支持时上报1个值 */
#if (FEATURE_LTE_4RX == FEATURE_ON)
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FRSSI:%d,%d,%d,%d", cnf->lValue1, cnf->lValue2, cnf->lValue3,
        cnf->lValue4);
#elif (FEATURE_LTE_8RX == FEATURE_ON)
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FRSSI:%d", cnf->value1);
#else
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr + length, "^FRSSI:%d", cnf->value1);
#endif

    CmdErrProc(clientId, cnf->errCode, length, g_atSndCodeAddr);
    return AT_FW_CLIENT_STATUS_READY;
}

VOS_UINT32 AT_GetLteFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure)
{
    VOS_UINT32 rst;

    VOS_UINT8 bandStr[AT_FEATURE_BAND_STR_LEN_MAX] = {0};
    VOS_UINT8 bandFlag = 0;

    VOS_UINT32 strlen1 = 0;
    VOS_UINT32 i       = 0;

    VOS_UINT32 bandNv                                       = 0;
    VOS_UINT32 bandNum[AT_FEATURE_LTE_MAX_SUPPORT_BAND + 1] = {0};
    LRRC_NV_UE_EUTRA_CAP_STRU *eutraCap = NULL;

    eutraCap = VOS_MemAlloc(WUEPS_PID_AT, (DYNAMIC_MEM_PT), sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));

    if (eutraCap == NULL) {
        return ERR_MSP_MALLOC_FAILUE;
    }

    memset_s(eutraCap, sizeof(LRRC_NV_UE_EUTRA_CAP_STRU), 0x00, sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));

    rst = TAF_ACORE_NV_READ(MODEM_ID_0, EN_NV_ID_UE_CAPABILITY, eutraCap, sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));
    if (rst != ERR_MSP_SUCCESS) {
        VOS_MemFree(WUEPS_PID_AT, eutraCap);
        eutraCap = VOS_NULL_PTR;
        return ERR_MSP_FAILURE;
    }

    /* RRC_MAX_NUM_OF_BANDS */

    eutraCap->stRfPara.usCnt = AT_MIN(eutraCap->stRfPara.usCnt, RRC_MAX_NUM_OF_BANDS);
    for (i = 0; i < eutraCap->stRfPara.usCnt; i++) {
        if (eutraCap->stRfPara.astSuppEutraBandList[i].ucEutraBand > 0) {
            bandNv = eutraCap->stRfPara.astSuppEutraBandList[i].ucEutraBand;
            if (bandNv < (AT_FEATURE_LTE_MAX_SUPPORT_BAND + 1)) {
                bandNum[bandNv] = 1;
                bandFlag++;
            }
        }
    }

    /* 解析获取LTE band信息 */

    if (bandFlag > 0) {
        feATure[AT_FEATURE_LTE].featureFlag = AT_FEATURE_EXIST;
    } else {
        VOS_MemFree(WUEPS_PID_AT, eutraCap);
        return ERR_MSP_FAILURE;
    }

    for (i = 1; i < AT_FEATURE_LTE_MAX_SUPPORT_BAND + 1; i++) {
        if (bandNum[i] == 1) {
            strlen1 += (VOS_UINT32)AT_FormatReportString(AT_FEATURE_BAND_STR_LEN_MAX, (VOS_CHAR *)bandStr,
                (VOS_CHAR *)(bandStr + strlen1), "B%d,", i);
        }
    }

    strlen1 = AT_MIN(strlen1, AT_FEATURE_CONTENT_LEN_MAX);
    for (i = 1; i < strlen1; i++) {
        feATure[AT_FEATURE_LTE].content[i - 1] = bandStr[i - 1];
    }

    for (i = 0; i < strlen1; i++) {
        bandStr[i] = 0;
    }

    strlen1 = 0;

    VOS_MemFree(WUEPS_PID_AT, eutraCap);
    eutraCap = VOS_NULL_PTR;
    return ERR_MSP_SUCCESS;
}

VOS_UINT32 atSetTBATPara(VOS_UINT8 clientId)
{
    FTM_SetTbatReq tbatSet = {0};
    FTM_RdTbatReq  tbatRd  = {0};

    VOS_UINT8  type;
    VOS_UINT8  opr;
    VOS_UINT16 value = 0;
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex > AT_TBAT_MAX_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    type = (VOS_UINT8)(g_atParaList[0].paraValue);
    opr  = (VOS_UINT8)(g_atParaList[1].paraValue);

    if (opr == 1) {
        /* 设置 */
        if (g_atParaList[AT_TBAT_VALUE_MIN].paraLen != 0) {
            value = g_atParaList[AT_TBAT_VALUE_MIN].paraLen;

            tbatSet.opr      = opr;
            tbatSet.type     = type;
            tbatSet.valueMin = (VOS_UINT16)g_atParaList[AT_TBAT_VALUE_MIN].paraValue;
            tbatSet.valueMax = (VOS_UINT16)g_atParaList[AT_TBAT_VALUE_MAX].paraValue;

            HAL_SDMLOG("\n stTbatSet.usValueMin=%d,stTbatSet.usValueMax=%d\n", tbatSet.valueMin, tbatSet.valueMax);

            rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_TBAT_REQ, clientId, (VOS_UINT8 *)(&tbatSet),
                                   sizeof(tbatSet));
            if (rst == AT_SUCCESS) {
                g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_TBAT_SET;
                return AT_WAIT_ASYNC_RETURN;
            }
            return AT_ERROR;
        } else {
            return AT_ERROR;
        }
    } else if (opr == 0) {
        /* 查询 */
        value = (VOS_UINT16)g_atParaList[AT_TBAT_VALUE].paraLen;

        tbatRd.opr   = opr;
        tbatRd.type  = type;
        tbatRd.value = value;

        rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_RD_TBAT_REQ, clientId, (VOS_UINT8 *)(&tbatRd),
                               sizeof(tbatRd));
        if (rst == AT_SUCCESS) {
            g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_TBAT_READ;
            return AT_WAIT_ASYNC_RETURN;
        }
        return AT_ERROR;

    } else {
        return AT_CME_INCORRECT_PARAMETERS;
    }
}

VOS_UINT32 atSetTbatCnf(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG         *event = NULL;
    FTM_SetTbatCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetTbatCnf *)event->param1;
    HAL_SDMLOG("\n enter into atSetTbatCnf\n");
    CmdErrProc(clientId, cnf->errCode, 0, NULL);
    return AT_OK;
}

VOS_UINT32 atRdTbatCnf(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG        *event = NULL;
    FTM_RdTbatCnf *cnf   = NULL;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_RdTbatCnf *)event->param1;
    HAL_SDMLOG("\n enter into atRdTbatCnf\n");
    HAL_SDMLOG("\n pstCnf->ucType=%d,pstCnf->usValue=%d \n", (VOS_INT)cnf->type, (VOS_INT)cnf->value);

    g_atSendData.bufLen = 0;
    g_atSendData.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddr,
        (VOS_CHAR *)g_atSndCodeAddr, "^TBAT:%d,%d", cnf->type, cnf->value);

    CmdErrProc(clientId, cnf->errCode, g_atSendData.bufLen, g_atSndCodeAddr);

    return AT_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)

VOS_UINT32 At_ProcLteTxCltInfoReport(struct MsgCB *msgBlock)
{
    FTM_CltInfoInd    *txCltInfoInd = VOS_NULL_PTR;
    OS_MSG            *pstpOsMsg    = VOS_NULL_PTR;
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;

    pstpOsMsg    = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    txCltInfoInd = (FTM_CltInfoInd *)pstpOsMsg->param1;
    atDevCmdCtrl = AT_GetDevCmdCtrl();

    /* 初始化全局变量 */
    (VOS_VOID)memset_s(&(atDevCmdCtrl->cltInfo), sizeof(atDevCmdCtrl->cltInfo), 0x0, sizeof(atDevCmdCtrl->cltInfo));

    /* 设置CLT信息有效标志 */
    atDevCmdCtrl->cltInfo.infoAvailableFlg = VOS_TRUE;

    /* 将接入层上报的信息记录下全局变量中 */
    /* 反射系数实部 */
    atDevCmdCtrl->cltInfo.gammaReal = txCltInfoInd->gammaReal;
    /* 反射系数虚部 */
    atDevCmdCtrl->cltInfo.gammaImag = txCltInfoInd->gammaImag;
    /* 驻波检测场景0反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc0 = txCltInfoInd->gammaAmpUc0;
    /* 驻波检测场景1反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc1 = txCltInfoInd->gammaAmpUc1;
    /* 驻波检测场景2反射系数幅度 */
    atDevCmdCtrl->cltInfo.gammaAmpUc2 = txCltInfoInd->gammaAmpUc2;
    /* 粗调格点位置 */
    atDevCmdCtrl->cltInfo.gammaAntCoarseTune = txCltInfoInd->gammaAntCoarseTune;
    /* 粗调FOM值 */
    atDevCmdCtrl->cltInfo.ulwFomcoarseTune = txCltInfoInd->ulwFomcoarseTune;
    /* 闭环算法收敛状态 */
    atDevCmdCtrl->cltInfo.cltAlgState = txCltInfoInd->cltAlgState;
    /* 闭环收敛总步数 */
    atDevCmdCtrl->cltInfo.cltDetectCount = txCltInfoInd->cltDetectCount;
    atDevCmdCtrl->cltInfo.dac0           = txCltInfoInd->dac0; /* DAC0 */
    atDevCmdCtrl->cltInfo.dac1           = txCltInfoInd->dac1; /* DAC1 */
    atDevCmdCtrl->cltInfo.dac2           = txCltInfoInd->dac2; /* DAC2 */
    atDevCmdCtrl->cltInfo.dac3           = txCltInfoInd->dac3; /* DAC3 */

    return VOS_TRUE;
}
#endif


