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
#include "AtParseCore.h"
#include "securec.h"
#include "AtCheckFunc.h"
#include "ATCmdProc.h"
#include "AtCmdMsgProc.h"
#include "AtInputProc.h"

#include "at_common.h"

#include "AcpuReset.h"

#include "AtInternalMsg.h"
#if (FEATURE_AT_PROXY == FEATURE_ON)
#include "at_atp_proc.h"
#endif


#define THIS_FILE_ID PS_FILE_ID_AT_PARSECORE_C
#define AT_AUC_TMP_CMD_ARRAY_LEN 9
#define AT_LF_LEN 1
#define AT_ZZ_LEN 2
#define AT_PEND_DATA_ARRAY_LEN 4
#define AT_CMD_NAME_MAX_NUM 3
#define AT_CMD_NAME_MAX_LEN 16
#define AT_CMD_NAME_COLUMNS_INDEX 2
#define AT_A0X2F_AND_CR_LEN 3
#define AT_PARSE_AT_LEN 2
#define AT_PARSE_AT_SEMICOLON_LEN 3
#define AT_CMD_AT_LEN 2 /* ????"AT"???? */

/* ???????????? */
HI_LIST_S g_cmdTblList = {0};

/* ???????????? */
AT_PARSECMD g_atParseCmd;

/* ???????? */
VOS_UINT8 g_atParaIndex = 0;

AT_ParseParaType g_atParaList[AT_MAX_PARA_NUMBER];

/* ???????????????????? */
AT_ParseContext g_parseContext[AT_MAX_CLIENT_NUM];

/* ????SMS??????????????????????????????????SMS???????????????? */
/*
 * ??????????????????????????????????????????
 * ??g_cmdElement=> g_cmdElement[AT_MAX_CLIENT_NUM]
 */
AT_ParCmdElement g_cmdElement[AT_MAX_CLIENT_NUM];
VOS_UINT8 AT_GetParaIndex(VOS_VOID)
{
    return g_atParaIndex;
}
AT_ParseParaType *AT_GetParaList(VOS_UINT8 indexNum)
{
    return &(g_atParaList[indexNum]);
}
AT_ParseContext *AT_GetParseContext(AT_ClientIdUint16 indexNum)
{
    return &(g_parseContext[indexNum]);
}


VOS_VOID At_ParseInit(VOS_VOID)
{
    VOS_UINT32 i = 0;

    (VOS_VOID)memset_s(&g_atParseCmd, sizeof(g_atParseCmd), 0x00, sizeof(g_atParseCmd));
    (VOS_VOID)memset_s(g_parseContext, sizeof(g_parseContext), 0x00, sizeof(g_parseContext));
    (VOS_VOID)memset_s(g_cmdElement, sizeof(g_cmdElement), 0x00, sizeof(g_cmdElement));

    /*lint -e717*/

    HI_INIT_LIST_HEAD(&g_cmdTblList);

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        HI_INIT_LIST_HEAD(&(g_parseContext[i].combineCmdInfo.combineCmdList));
    }

    /*lint -e717*/

    return;
}
/*lint -save -e429*/

VOS_UINT32 AT_RegisterCmdTable(const AT_ParCmdElement *cmdTblEntry, VOS_UINT16 cmdNum)
{
    HI_LIST_S        *cmdTblHeader = NULL;
    AT_ParCmdtblList *cmdTblNode   = NULL; /*lint !e830*/

    /* ????????????????????????????0 */
    if ((cmdTblEntry == NULL) || (cmdNum == 0)) {
        return ERR_MSP_FAILURE;
    }

    cmdTblHeader = &g_cmdTblList;

    /* ??????????errcodetbl???????????? */
    cmdTblNode = (AT_ParCmdtblList *)AT_MALLOC(sizeof(AT_ParCmdtblList));
    if (cmdTblNode == NULL) {
        return ERR_MSP_MALLOC_FAILUE;
    }

    (VOS_VOID)memset_s(cmdTblNode, sizeof(AT_ParCmdtblList), 0x00, sizeof(AT_ParCmdtblList));
    cmdTblNode->cmdNum = cmdNum;

    cmdTblNode->cmdElement = (AT_ParCmdElement *)cmdTblEntry;

    msp_list_add_tail((&cmdTblNode->cmdTblList), cmdTblHeader);

    return ERR_MSP_SUCCESS;
}
/*lint -restore */


#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)
VOS_VOID AT_ClacCmdProc(VOS_VOID)
{
    const AT_CmdTrustListMapTbl *atCmdTrustListTbl = VOS_NULL_PTR;
    VOS_UINT32                   trustListCmdNum;
    VOS_UINT32                   i = 0;

    atCmdTrustListTbl = AT_GetUserCmdTrustListTbl();
    trustListCmdNum   = AT_GetUserCmdTrustListTblCmdNum();
    for (i = 0; i < trustListCmdNum; i++) {
        /* ???????????????? */
        if (atCmdTrustListTbl[i].chkFlag == TRUST_LIST_CMD_CLAC_IS_VISIBLE) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%c%c",
                atCmdTrustListTbl[i].atCmd, g_atS3, g_atS4);
        }
    }

    return;
}
#else
VOS_VOID AT_ClacCmdProc(VOS_VOID)
{
    VOS_UINT32              i                = 0;
    const AT_ParCmdElement *cmdBasicElement  = VOS_NULL_PTR;
    const AT_ParCmdElement *cmdExtendElement = VOS_NULL_PTR;
    VOS_UINT32        cmdNum;

    cmdBasicElement = At_GetBasicCmdTable();
    cmdNum     = At_GetBasicCmdNum();

    for (i = 0; i < cmdNum; i++) {
        /* ???????????????? */
        if ((cmdBasicElement[i].chkFlag & CMD_TBL_CLAC_IS_INVISIBLE) == 0) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%c%c",
                cmdBasicElement[i].cmdName, g_atS3, g_atS4);
        }
    }

    for (i = 0; i < g_atSmsCmdNum; i++) {
        /* ???????????????? */
        if ((g_atSmsCmdTab[i].chkFlag & CMD_TBL_CLAC_IS_INVISIBLE) == 0) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%c%c",
                g_atSmsCmdTab[i].cmdName, g_atS3, g_atS4);
        }
    }

    cmdExtendElement = At_GetExtendCmdTable();
    cmdNum     = At_GetExtendCmdNum();

    for (i = 0; i < cmdNum; i++) {
        /* ???????????????? */
        if ((cmdExtendElement[i].chkFlag & CMD_TBL_CLAC_IS_INVISIBLE) == 0) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%c%c",
                cmdExtendElement[i].cmdName, g_atS3, g_atS4);
        }
    }

    return;
}
#endif


VOS_UINT8 AT_BlockCmdCheck(VOS_VOID)
{
    VOS_UINT8 i       = 0;
    VOS_UINT8 blockid = AT_MAX_CLIENT_NUM;

    /* ???????????????? */
    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        /* ????????????????ready?????????????????????? */
        if (g_parseContext[i].clientStatus != AT_FW_CLIENT_STATUS_READY) {
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
            /* ????????AT????APP??????????????AT????????????????????????????,??????????????????AT???????????????? */
            if((g_atClientTab[AT_CLIENT_ID_APP].isWaitAts == 1) &&
                (g_parseContext[AT_CLIENT_ID_APP].blockCmdLen != 0) &&
                (g_parseContext[AT_CLIENT_ID_APP].blockCmd != NULL_PTR)) {
                return AT_CLIENT_ID_APP;
            }
#endif
            return AT_MAX_CLIENT_NUM;
        }

        if ((g_parseContext[i].blockCmdLen != 0) && (g_parseContext[i].blockCmd != NULL)) {
            blockid = i;
        }
    }

    return blockid;
}


VOS_VOID AT_ClearBlockCmdInfo(VOS_UINT8 indexNum)
{
    if (g_parseContext[indexNum].blockCmd != NULL) {
        AT_FREE(g_parseContext[indexNum].blockCmd);
        g_parseContext[indexNum].blockCmd = VOS_NULL_PTR;
        g_parseContext[indexNum].blockCmd = NULL;
    }

    g_parseContext[indexNum].blockCmdLen = 0;

    return;
}



VOS_VOID AT_CheckProcBlockCmd(VOS_VOID)
{
    VOS_UINT8  indexNum;
    VOS_UINT32 ret;

    /* ???????????????????? */
    indexNum = AT_BlockCmdCheck();
    if (indexNum < AT_MAX_CLIENT_NUM) {
        /* ???????????????????? */
        ret = At_SendCmdMsg(indexNum, &indexNum, sizeof(indexNum), AT_COMBIN_BLOCK_MSG);
        if (ret != ERR_MSP_SUCCESS) {
            /* ?????????????????? */
        }
    }

    return;
}


VOS_VOID AT_ParseCmdOver(VOS_UINT8 indexNum)
{
    VOS_UINT32            ret            = ERR_MSP_FAILURE;
    AT_ParseContext      *clientContext  = NULL;
    AT_FW_CombineCmdInfo *combineCmdInfo = NULL;

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        return;
    }

    clientContext = &(g_parseContext[indexNum]);

    combineCmdInfo = &clientContext->combineCmdInfo;

    if (combineCmdInfo->procNum < combineCmdInfo->totalNum) {
        combineCmdInfo->procNum++;
    } else { /* ???????????? */
        combineCmdInfo->procNum = combineCmdInfo->totalNum;
    }

    /* ???????????????????????? */
    if ((combineCmdInfo->procNum < combineCmdInfo->totalNum)) {
        /* ?????????????????????? */
        ret = At_SendCmdMsg(indexNum, &indexNum, sizeof(indexNum), AT_COMBIN_BLOCK_MSG);
        if (ret != ERR_MSP_SUCCESS) {
            /* ?????????????????????????? */
            clientContext->clientStatus = AT_FW_CLIENT_STATUS_READY;
        }

        return;
    }

    return;
}


VOS_VOID AT_BlockCmdTimeOutProc(REL_TimerMsgBlock *msg)
{
    VOS_UINT8 indexNum;

    indexNum = (VOS_UINT8)((msg->name) >> 12);

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, AT_ERROR);

    AT_ClearBlockCmdInfo(indexNum);

    return;
}


VOS_VOID AT_PendClientProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    if ((len == AT_PEND_DATA_ARRAY_LEN) && (At_UpChar(data[0]) == 'S') && (At_UpChar(data[1]) == 'T') &&
        (At_UpChar(data[2]) == 'O') && (At_UpChar(data[3]) == 'P')) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_ResetCombinParseInfo(indexNum);
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_A_SET) {
        if ((At_UpChar(data[0]) != 'A') || (At_UpChar(data[1]) != 'T') || (At_UpChar(data[2]) != 'H')) {
        } else {
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_ResetCombinParseInfo(indexNum);
        }
    } else if (AT_IsAbortCmdStr(indexNum, data, len) == VOS_TRUE) {
        /* AT????????PEND?????????? */
        AT_AbortCmdProc(indexNum);
    }
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    else if (g_atClientTab[indexNum].canAbort == VOS_TRUE) {
        /* g_AppAtList??????????????AT????????????????????????????????????????AT
           * AT????????PEND??????????,??????????AT??????????
           */
        if ((sizeof(char) == len) && (data[0] == g_atS4)) {
            /* \r????????AT????????????????????????????????\n????????????????
                * ??????????????????????????????????????????????AT??????????
                */
        } else {
            AT_AbortCmdProc(indexNum);
        }
    }
#endif
    else {
    }

    return;
}


VOS_VOID AT_HoldBlockCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    errno_t          memResult;
    VOS_UINT32       timerName;
    VOS_UINT32       tempIndex;
    AT_ParseContext *clientContext = NULL;

    clientContext = &(g_parseContext[indexNum]);

    /* ????????????????????????????AT???????????????????????????????????????? */
    if (clientContext->blockCmd != NULL) {
        AT_FREE(clientContext->blockCmd);
        clientContext->blockCmd = VOS_NULL_PTR;
    }

    if (len == 0) {
       return;
    }

    clientContext->blockCmd = (VOS_UINT8 *)AT_MALLOC(len);
    if (clientContext->blockCmd == NULL) {
        clientContext->blockCmdLen = 0;
        return;
    }

    (VOS_VOID)memset_s(clientContext->blockCmd, len, 0x00, len);

    if (len > 0) {
        memResult = memcpy_s(clientContext->blockCmd, len, data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, len, len);
    }
    clientContext->blockCmdLen = len;

    tempIndex = (VOS_UINT32)indexNum;
    timerName = AT_HOLD_CMD_TIMER;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    /* ?????????? */
    VOS_StopRelTimer(&clientContext->timer);

    AT_StartRelTimer(&clientContext->timer, AT_HOLD_CMD_TIMER_LEN, timerName, 0, VOS_RELTIMER_NOLOOP);

    return;
}


VOS_UINT32 AT_ParseCmdIsComb(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_ParseContext *clientContext = NULL;

    clientContext = &(g_parseContext[indexNum]);

    /* ???????????????????????????? */
    if (clientContext->combineCmdInfo.procNum < clientContext->combineCmdInfo.totalNum) {
        CmdStringFormat(indexNum, data, &len);

        AT_PendClientProc(indexNum, data, len);

        return ERR_MSP_FAILURE;
    }

    return ERR_MSP_SUCCESS;
}


VOS_UINT32 AT_ParseCmdIsPend(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT32       i             = 0;
    AT_ParseContext *clientContext = NULL;

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        clientContext = &(g_parseContext[i]);

        if (i == indexNum) {
            /* ??????????PEND????????????????????STOP??ATH */
            if (clientContext->clientStatus == AT_FW_CLIENT_STATUS_PEND) {
                CmdStringFormat(indexNum, data, &len);

                AT_PendClientProc(indexNum, data, len);
                return ERR_MSP_FAILURE;
            }
        }
        /* ????????????PEND?????????????? */
        else if ((clientContext->clientStatus == AT_FW_CLIENT_STATUS_PEND) || (clientContext->mode == AT_SMS_MODE)) {
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
             /* ?????????????????????????????? */
             if ((indexNum == AT_CLIENT_ID_APP) && (g_atClientTab[AT_CLIENT_ID_APP].isWaitAts == 1)) {
                 continue;
             }
#endif

            /* ?????????????????????????????????????? */
            if (AT_IsConcurrentPorts(indexNum, (VOS_UINT8)i) == VOS_TRUE) {
                /* ?????????????????????? */
                continue;
            } else {
                /* ?????????????????????? */
                AT_HoldBlockCmd(indexNum, data, len);
                return ERR_MSP_FAILURE;
            }
        } else {
        }
    }

    return ERR_MSP_SUCCESS;
}


VOS_VOID AT_DiscardInvalidCharForSms(TAF_UINT8 *data, TAF_UINT16 *len)
{
    const TAF_CHAR cMGSCmd[] = "AT+CMGS=";
    const TAF_CHAR cMGWCmd[] = "AT+CMGW=";
    const TAF_CHAR cMGCCmd[] = "AT+CMGC=";
    TAF_CHAR       tmpCmd[AT_AUC_TMP_CMD_ARRAY_LEN];
    VOS_INT        iCMGSResult;
    VOS_INT        iCMGWResult;
    VOS_INT        iCMGCResult;
    errno_t  memResult;

    if (*len < VOS_StrLen(cMGSCmd)) {
        return;
    }

    memResult = memcpy_s(tmpCmd, sizeof(tmpCmd), data, sizeof(tmpCmd) - 1);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tmpCmd), sizeof(tmpCmd) - 1);
    At_UpString((TAF_UINT8 *)tmpCmd, sizeof(tmpCmd) - 1);
    tmpCmd[AT_AUC_TMP_CMD_ARRAY_LEN-1] = '\0';

    iCMGSResult = VOS_StrCmp(cMGSCmd, tmpCmd);
    iCMGWResult = VOS_StrCmp(cMGWCmd, tmpCmd);
    iCMGCResult = VOS_StrCmp(cMGCCmd, tmpCmd);
    if ((iCMGSResult != 0) && (iCMGWResult != 0) && (iCMGCResult != 0)) {
        return;
    }

    /*
     * MAC????????MP????????:AT+CMGS=**<CR><^z><Z>(??AT+CMGW=**<CR><^z><Z>)
     * ??????????????????????????????????????????????
     * ??????????????????????<^z><Z>????
     */
    if ((g_atS3 == data[*len - 3]) && (data[*len - 2] == '\x1a') &&
        ((data[*len - 1] == 'z') || (data[*len - 1] == 'Z'))) {
        /* ??????????<^z><Z>???? */
        *len -= AT_ZZ_LEN;
    }
    /* MAC????SFR????????: AT+CMGS=<LENGTH><CR><LF><CR> */
    else if ((g_atS3 == data[*len - 3]) && (g_atS4 == data[*len - 2]) && (g_atS3 == data[*len - 1])) {
        /* ??????????<LF><CR>???? */
        *len -= AT_CRLF_STR_LEN;
    }
    /* ????pData????????????????<CR><LF>???? */
    else if ((g_atS3 == data[*len - 2]) && (g_atS4 == data[*len - 1])) {
        /* ??????????<LF>???? */
        *len -= AT_LF_LEN;
    } else {
    }

    return;
}


VOS_VOID AT_ResetParseVariable(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_atParseCmd, sizeof(g_atParseCmd), 0x00, sizeof(AT_PARSECMD));

    (VOS_VOID)memset_s(g_atParaList, sizeof(g_atParaList), 0x00, sizeof(g_atParaList));

    g_atParseCmd.cmdOptType = AT_CMD_OPT_BUTT;
    g_atParseCmd.cmdFmtType = AT_CMD_TYPE_BUTT;

    g_atParaIndex = 0;
    return;
}


VOS_UINT32 AT_ParseCmdType(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 dataTmpLen = len - AT_PARSE_AT_LEN;
    VOS_UINT8 *dataTmp    = data + AT_PARSE_AT_LEN;
    VOS_UINT32 result     = 0;

    /* ??????????????pData?????? */

    if (len < AT_PARSE_AT_LEN) {
        return AT_ERROR;
    }

    if (At_CheckCharA(*data) == AT_SUCCESS) {           /* ??????'A'/'a' */
        if (At_CheckCharT(*(data + 1)) == AT_SUCCESS) { /* ??????'T'/'t' */
            if (len == AT_PARSE_AT_LEN) {
                return AT_OK;
            }

            if (len == AT_PARSE_AT_SEMICOLON_LEN) {
                if (At_CheckSemicolon(data[AT_PARSE_AT_SEMICOLON_LEN-1]) == AT_SUCCESS) {
                    return AT_OK;
                }
            }

            AT_ResetParseVariable(); /* ???????????????????????? */

            dataTmp    = data + AT_PARSE_AT_LEN;
            dataTmpLen = len - AT_PARSE_AT_LEN;

            /* ??D????????????????????????';' */
            if ((At_CheckCharD(*dataTmp)) != AT_SUCCESS && (*(dataTmp + dataTmpLen - 1) == ';')) {
                dataTmpLen--;
            }

            switch (*dataTmp) {
                /* ?????????????????????? */
                case '^': /* ???????? */
                case '+': /* ???????? */
                case '$': /* ???????? */
                    return atParseExtendCmd(dataTmp, dataTmpLen);

                case 'd':
                case 'D': /* D???? */

                    return ParseDCmdPreProc(dataTmp, dataTmpLen);

                case 's':
                case 'S': /* S???? */
                    return atParseSCmd(dataTmp, dataTmpLen);

                default: /* ???????? */
                {
                    result = atParseBasicCmd(dataTmp, dataTmpLen);

                    if (result == AT_FAILURE) {
                        result = AT_ERROR;
                    }

                    return result;
                }
            }
        }
    }

    return AT_ERROR;
}


VOS_UINT32 At_MatchSmsCmdName(VOS_UINT8 indexNum, VOS_CHAR *pszCmdName)
{
    VOS_UINT32      i      = 0;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    for (i = 0; i < g_atSmsCmdNum; i++) {
        if ((TAF_CHAR *)g_atSmsCmdTab[i].cmdName != VOS_NULL_PTR) {
            if (AT_STRCMP(pszCmdName, (TAF_CHAR *)g_atSmsCmdTab[i].cmdName) == ERR_MSP_SUCCESS) {
                if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) {
                    g_cmdElement[indexNum].param = g_atSmsCmdTab[i].paraText;
                } else {
                    g_cmdElement[indexNum].param = g_atSmsCmdTab[i].paraPDU;
                }

                g_cmdElement[indexNum].cmdIndex    = g_atSmsCmdTab[i].cmdIndex;
                g_cmdElement[indexNum].setProc     = g_atSmsCmdTab[i].setProc;
                g_cmdElement[indexNum].setTimeOut  = g_atSmsCmdTab[i].setTimeOut;
                g_cmdElement[indexNum].qryProc     = NULL;
                g_cmdElement[indexNum].qryTimeOut  = 0;
                g_cmdElement[indexNum].testProc    = g_atSmsCmdTab[i].testProc;
                g_cmdElement[indexNum].testTimeOut = g_atSmsCmdTab[i].testTimeOut;

                g_cmdElement[indexNum].abortProc    = g_atSmsCmdTab[i].abortProc;
                g_cmdElement[indexNum].abortTimeOut = g_atSmsCmdTab[i].abortTimeOut;

                g_cmdElement[indexNum].paramErrCode = g_atSmsCmdTab[i].paramErrCode;
                g_cmdElement[indexNum].chkFlag      = g_atSmsCmdTab[i].chkFlag;
                g_cmdElement[indexNum].cmdName      = g_atSmsCmdTab[i].cmdName;

                g_parseContext[indexNum].cmdElement = &g_cmdElement[indexNum];

                return AT_SUCCESS;
            }
        }
    }

    g_parseContext[indexNum].cmdElement = NULL;

    return AT_FAILURE;
}

#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)

VOS_UINT32 AT_MatchUserCustomCmdName(VOS_UINT8 indexNum, VOS_CHAR *cmdName)
{
    AT_ParCmdElement *appAtList = VOS_NULL_PTR;
    VOS_UINT32        appAtCount;
    VOS_UINT32        i;

    appAtList = AT_GetAppAtListTbl();
    appAtCount = AT_GetAppAtCount();
    if (appAtList == NULL) {
        AT_ERR_LOG("AT_MatchUserCustomCmdName g_AppAtList is NULL");
        return AT_CMD_NOT_SUPPORT;
    }
    for (i = 0; i < appAtCount; i++) {
        if (appAtList[i].cmdName == NULL) {
            continue;
        }

        if (AT_STRCMP(cmdName, (VOS_CHAR *)appAtList[i].cmdName) == ERR_MSP_SUCCESS) {
            g_parseContext[indexNum].cmdElement = &(appAtList[i]);

            return ERR_MSP_SUCCESS;
        }
    }

    AT_ERR_LOG("AT_MatchUserCustomCmdName return cmd not support");
    return AT_CMD_NOT_SUPPORT;
}
#endif

VOS_UINT32 AT_MatchTrustListCmdName(VOS_CHAR *cmdName, VOS_UINT16 cmdNameLen)
{
    VOS_UINT8  keyWord[AT_CMD_KEY_WORD_MAX_LEN + 1] = {0};
    VOS_UINT32 keyWordLen;
    VOS_UINT32 ret = ERR_MSP_FAILURE;

    keyWordLen = AT_GetCmdKeyWord(cmdName, cmdNameLen, keyWord, AT_CMD_KEY_WORD_MAX_LEN);
    if (keyWordLen == AT_CMD_KEY_WORD_LEN_ERR) {
        return AT_ERROR;
    }

    /* ??????????????Trust???????? */
    ret = AT_IsCmdInTrustList(keyWord, keyWordLen);
    if(ret != AT_SUCCESS) {
        AT_PR_LOGE("Trustlist check fail, cmdName is %s", cmdName);
        return AT_CMD_NOT_SUPPORT;
    }

    return ERR_MSP_SUCCESS;
}
#endif


VOS_UINT32 atMatchCmdName(VOS_UINT8 clientId, VOS_UINT32 cmdType)
{
    VOS_UINT32        i          = 0;
    HI_LIST_S        *me         = NULL;
    HI_LIST_S        *cmdTblList = NULL;
    AT_ParCmdtblList *cmdNode    = NULL;
    VOS_CHAR         *pszCmdName = NULL;

    /* ?????????????????????????????? */

    cmdTblList = &(g_cmdTblList);

    /* ???????????????????? */
    if (At_UpString(g_atParseCmd.cmdName.cmdName, TAF_MIN(g_atParseCmd.cmdName.cmdNameLen, AT_CMD_NAME_LEN))
        == AT_FAILURE) {
        return AT_ERROR;
    }

    pszCmdName = (VOS_CHAR *)g_atParseCmd.cmdName.cmdName;

    if ((cmdType == AT_EXTEND_CMD_TYPE) || (cmdType == AT_BASIC_CMD_TYPE)) {
#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
        if (AT_MatchUserCustomCmdName(clientId, pszCmdName) == ERR_MSP_SUCCESS) {
            return ERR_MSP_SUCCESS;
        }
#endif
        if (AT_MatchTrustListCmdName(pszCmdName, g_atParseCmd.cmdName.cmdNameLen) != ERR_MSP_SUCCESS) {
            return AT_CMD_NOT_SUPPORT;
        }
#endif
        /* ???????????????????? */
        if (At_MatchSmsCmdName(clientId, pszCmdName) == ERR_MSP_SUCCESS) {
            return ERR_MSP_SUCCESS;
        }

        msp_list_for_each(me, cmdTblList)
        {
            cmdNode = msp_list_entry(me, AT_ParCmdtblList, cmdTblList);

            for (i = 0; i < cmdNode->cmdNum; i++) {
                if ((cmdNode->cmdElement == NULL) || (cmdNode->cmdElement[i].cmdName == NULL)) {
                    continue;
                }

                if (AT_STRCMP((VOS_CHAR *)pszCmdName, (VOS_CHAR *)cmdNode->cmdElement[i].cmdName) == ERR_MSP_SUCCESS) {
                    g_parseContext[clientId].cmdElement = &(cmdNode->cmdElement[i]);

                    return ERR_MSP_SUCCESS;
                }
            }
        }
    }

    return AT_CMD_NOT_SUPPORT;
}


VOS_UINT32 ParseParam(AT_ParCmdElement *cmdElement)
{
    VOS_UINT32 paraLen = 0;

    if (g_atParseCmd.cmdOptType == AT_CMD_OPT_SET_PARA_CMD) {
        if ((cmdElement != NULL) && (cmdElement->param != NULL)) {
            paraLen = AT_STRLEN((VOS_CHAR *)cmdElement->param);                                   /* ???????????????? */
            if (atParsePara((VOS_UINT8 *)cmdElement->param, (VOS_UINT16)paraLen) != AT_SUCCESS) { /* ???????? */
                return AT_CME_INCORRECT_PARAMETERS;
            }
        }
    }

    return ERR_MSP_SUCCESS;
}

AT_RreturnCodeUint32 fwCmdTestProc(VOS_UINT8 indexNum, AT_ParCmdElement *cmdElement)
{
    AT_RreturnCodeUint32 result = AT_FAILURE;

    /* ???????????????????????? */

    if (cmdElement->testProc != NULL) {
        result = (AT_RreturnCodeUint32)cmdElement->testProc(indexNum);

        if (result == AT_WAIT_ASYNC_RETURN) {
            g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

            /* ???????? */
            if (At_StartTimer(cmdElement->testTimeOut, indexNum) != AT_SUCCESS) {
                return AT_ERROR;
            }

            At_SetAtCmdAbortTickInfo(indexNum, VOS_GetTick());
        }

        return result;
    } else if (cmdElement->param != NULL) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %s", cmdElement->cmdName, cmdElement->param);

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


AT_RreturnCodeUint32 atCmdDispatch(VOS_UINT8 indexNum)
{
    AT_RreturnCodeUint32 result;
    PFN_AT_FW_CMD_PROC   pfnCmdProc = NULL;
    VOS_UINT32           timerLen   = 0;
    AT_ParCmdElement    *cmdElement = g_parseContext[indexNum].cmdElement;

    switch (g_atParseCmd.cmdOptType) {
        case AT_CMD_OPT_SET_PARA_CMD:
        case AT_CMD_OPT_SET_CMD_NO_PARA:
            pfnCmdProc = cmdElement->setProc; /* ?????????????? */
            timerLen   = cmdElement->setTimeOut;
            break;

        case AT_CMD_OPT_READ_CMD:
            pfnCmdProc = cmdElement->qryProc; /* ???????? */
            timerLen   = cmdElement->qryTimeOut;
            break;

        case AT_CMD_OPT_TEST_CMD:
            return fwCmdTestProc(indexNum, cmdElement); /* ???????? */

        default:
            return AT_ERROR; /* ???????????????????? */
    }

    if (pfnCmdProc == NULL) {
        return AT_ERROR;
    }

    result = (AT_RreturnCodeUint32)pfnCmdProc(indexNum);

    if (result == AT_WAIT_ASYNC_RETURN) {
        /* ???????? */
        if (At_StartTimer(timerLen, indexNum) != AT_SUCCESS) {
            return AT_ERROR;
        }

        At_SetAtCmdAbortTickInfo(indexNum, VOS_GetTick());

        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;
    }

    return result;
}


VOS_UINT32 LimitedCmdProc(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 len, AT_ParCmdElement *cmdElement)
{
    VOS_BOOL             bE5CheckRight = VOS_TRUE;
    AT_RreturnCodeUint32 result        = AT_FAILURE;

    if (cmdElement == NULL) {
        return AT_ERROR;
    }

    /* ????E5????????????????????????E5??????????????????????????AT_ERROR??????????AT_OK */
    if ((cmdElement->chkFlag & CMD_TBL_E5_IS_LOCKED) == 0) {
        /* ??AT???? ??WIFI????, ?????????????? */
        if ((g_atClientTab[clientId].userType != AT_USBCOM_USER) &&
            (g_atClientTab[clientId].userType != AT_SOCK_USER)) {
            /* ????????PIN???????????? */
        } else {
            bE5CheckRight = AT_E5CheckRight(clientId, data, len);
            if (bE5CheckRight == VOS_TRUE) {
                /* E5????????????????PIN???????????? */
            } else {
                return AT_SUCCESS; /* AT_E5CheckRight ?????????????????? */
            }
        }
    }

    /* ??????E5 DOCK????????????????DOCK??????????????????AT_SUCCESS */
    if (cmdElement->chkFlag & CMD_TBL_IS_E5_DOCK) {
        if (g_atClientTab[clientId].userType == AT_USBCOM_USER) {
            result = (AT_RreturnCodeUint32)AT_DockHandleCmd(clientId, data, len);
            if (result == AT_SUCCESS) {
                return AT_SUCCESS; /* ????????????????E5?????????????????????????? */
            } else {
                return AT_ERROR; /* ??????????????ERROR */
            }
        }
    }

    /* ????PIN??????????????????????PIN????????????????????????????AT_ERROR??????????AT_OK */
    if ((cmdElement->chkFlag & CMD_TBL_PIN_IS_LOCKED) == 0) {
        result = (AT_RreturnCodeUint32)At_CheckUsimStatus((VOS_UINT8 *)cmdElement->cmdName, clientId);
        if (result == AT_SUCCESS) {
            /* PIN???????????????????????????? */
        } else {
            return result; /* ???????????????????? */
        }
    }

    return AT_OK;
}


VOS_UINT32 CmdParseProc(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_RreturnCodeUint32 result;
    AT_ParCmdElement    *cmdElement = NULL;

    /* ??????????????????ucClientId??????????pDataIn?????? */

    /* ???????????? */
    result = (AT_RreturnCodeUint32)atMatchCmdName(clientId, g_atParseCmd.cmdFmtType);
    if (result != ERR_MSP_SUCCESS) {
        return result;
    }

    cmdElement = g_parseContext[clientId].cmdElement;

    if (cmdElement == NULL) {
        return AT_ERROR;
    }

    /* ???????????????????????????????????????????????????????????????? */
    result = (AT_RreturnCodeUint32)LimitedCmdProc(clientId, data, len, cmdElement);
    if (result != AT_OK) {
        return result;
    }

    /* ???????????? */
    result = (AT_RreturnCodeUint32)ParseParam(cmdElement);

    if (result != ERR_MSP_SUCCESS) {
        if (cmdElement->paramErrCode == AT_RRETURN_CODE_BUTT) {
            return AT_CME_INCORRECT_PARAMETERS;
        } else {
            return (AT_RreturnCodeUint32)(cmdElement->paramErrCode);
        }
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    /* ??????AT??????????CL????????????????AT???????????????????????????????????????? */
    if (At_CheckCurrRatModeIsCL(clientId) == VOS_TRUE) {
        if (atCmdIsSupportedByCLMode(clientId) == VOS_TRUE) {
            return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
        }
    }
    else {
        if (atCmdIsSupportedByGULMode(clientId) == VOS_FALSE) {
            return AT_CME_OPERATION_NOT_ALLOWED;
        }
    }
#endif

    result = (AT_RreturnCodeUint32)atCmdDispatch(clientId);

    return result;
}


VOS_VOID RepeatCmdProc(AT_ParseContext *clientContext)
{
    VOS_UINT8 *data = clientContext->dataBuff;
    VOS_UINT32 len  = clientContext->dataLen;
    errno_t    memResult;

    if (g_atS4 == *data) {
        data++;
        len--;
    }

    /* ??????????"A/" */
    if (len == AT_A0X2F_AND_CR_LEN) {
        if (At_CheckCharA(*data) == AT_SUCCESS) {
            if (atCheckChar0x2f(*(data + 1)) == AT_SUCCESS) {
                if (clientContext->cmdLineLen == 0) {
                    clientContext->dataLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                        (VOS_CHAR *)clientContext->dataBuff, (VOS_CHAR *)clientContext->dataBuff, "AT%c", g_atS3);
                } else if (clientContext->cmdLineLen > AT_CMD_MAX_LEN) {
                        AT_ERR_LOG("clientContext->cmdLine cannot copy to clientContext->dataBuff");
                        return;
                } else {
                    memResult = memcpy_s(clientContext->dataBuff, sizeof(clientContext->dataBuff),
                                         (VOS_UINT8 *)clientContext->cmdLine, clientContext->cmdLineLen);
                    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(clientContext->dataBuff), clientContext->cmdLineLen);

                    clientContext->dataLen = clientContext->cmdLineLen;
                }
            }
        }
    }

    return;
}


VOS_VOID SaveRepeatCmd(AT_ParseContext *clientContext, VOS_UINT8 *data, VOS_UINT16 len)
{
    errno_t memResult;
    /* ????A/???????? */
    if (clientContext->cmdLine != NULL) {
        AT_FREE(clientContext->cmdLine);
        clientContext->cmdLine = VOS_NULL_PTR;
    }

    if (len == 0) {
       return;
    }

    clientContext->cmdLine = (VOS_UINT8 *)AT_MALLOC(len);
    if (clientContext->cmdLine == NULL) {
        return;
    }

    (VOS_VOID)memset_s(clientContext->cmdLine, len, 0x00, len);

    if (len > 0) {
        memResult = memcpy_s(clientContext->cmdLine, len, data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, len, len);
    }
    clientContext->cmdLineLen = len;

    return;
}


VOS_UINT32 ScanDelChar(VOS_UINT8 *data, VOS_UINT16 *len, VOS_UINT8 atS5)
{
    VOS_UINT16 chkLen  = 0;
    VOS_UINT16 lenTemp = 0;
    VOS_UINT8 *write   = data;
    VOS_UINT8 *read    = data;
    VOS_UINT16 num     = 0; /* ?????????????? */

    if (*len == 0) {
        return AT_FAILURE;
    }

    /* ???????????? */
    while (chkLen++ < *len) {
        if (*read == '"') {
            num++;
            *write++ = *read;
            lenTemp++;
        } else if ((atS5 == *read) && ((num % 2) == 0)) { /* ????????????,??2???????????????????????????? */
            if (lenTemp > 0) {
                write--;
                lenTemp--;
            }
        } else { /* ???????????? */
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }
    *len = lenTemp;
    return AT_SUCCESS;
}


VOS_UINT32 ScanCtlChar(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT8 *write   = data;
    VOS_UINT8 *read    = data;
    VOS_UINT16 chkLen  = 0;
    VOS_UINT16 lenTemp = 0;
    VOS_UINT16 num     = 0;

    if (*len == 0) {
        return AT_FAILURE;
    }

    while (chkLen++ < *len) {
        if (*read >= 0x20) { /* ???????????? */
            if (*read == '"') {
                num++;
            }
            *write++ = *read;
            lenTemp++;
        } else {
            if (num % 2) { /* ??????????????0x20???????????? */
                *write++ = *read;
                lenTemp++;
            }
        }
        read++;
    }

    *len = lenTemp;
    return AT_SUCCESS;
}

#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
/*
 * ????????: ??????????????????????????????????????
 */
VOS_VOID IsInQuotationMarks(VOS_UINT16 chkLen, VOS_UINT8 *read, VOS_UINT16 *quotationMarksNum, VOS_BOOL *isInQuotationMarks)
{
    if ((chkLen > 1) && (*read == '"') && (*(read - 1) != '\\')) {
        (*quotationMarksNum)++;
        *isInQuotationMarks = *quotationMarksNum % 2;
    }
}
#endif


VOS_UINT32 ScanBlankChar(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT8 *write        = data;
    VOS_UINT8 *read         = data;
    VOS_UINT16 chkLen       = 0;
    VOS_UINT16 lenTemp      = 0;
    VOS_UINT16 colonBankCnt = 0;
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    VOS_UINT16 quotationMarksNum = 0;
    VOS_BOOL   isInQuotationMarks = FALSE;
#endif
    VOS_BOOL   bIsColonBack = FALSE;
    VOS_BOOL   bIsEqual     = FALSE;

    if (*len == 0) {
        return AT_FAILURE;
    }


    /* ???? */
    while (chkLen++ < *len) {
        if (*read == '=') {
            bIsEqual = TRUE;
        }
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
        /* ?????????????????????????????????????? */
        IsInQuotationMarks(chkLen, read, &quotationMarksNum, &isInQuotationMarks);
#endif
        if ((*read == ' ') && (bIsEqual != TRUE)) {
            /* ???????????????? */
        } else { /* rim?????????????? */
            if (*read != 0x20 && *read != ',') {
                /* rim','?????? */
                if (bIsColonBack) {
                    write -= colonBankCnt;
                    lenTemp -= colonBankCnt;
                    colonBankCnt = 0;
                    bIsColonBack = FALSE;
                } else {
                    colonBankCnt = 0;
                }
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
            } else if ((*read == ',') && (isInQuotationMarks == FALSE)) {
                /* rim','?????? */
                write -= colonBankCnt;
                lenTemp -= colonBankCnt;
                colonBankCnt = 0;
                bIsColonBack = TRUE;
            } else if (isInQuotationMarks == TRUE) {
                colonBankCnt = 0;
#else
            } else if (*read == ',') {
                /* rim','?????? */
                write -= colonBankCnt;
                lenTemp -= colonBankCnt;
                colonBankCnt = 0;
                bIsColonBack = TRUE;
#endif
            } else {
                colonBankCnt++;
            }

            *write++ = *read;
            lenTemp++;
        }

        read++;
    }

    lenTemp -= colonBankCnt;

    *len = lenTemp;
    return AT_SUCCESS;
}


PRIVATE VOS_UINT32 ScanBlankCharAfterEq(IN OUT VOS_UINT8 *data, IN OUT VOS_UINT16 *len)
{
    VOS_UINT8 *write        = data;
    VOS_UINT8 *read         = data;
    VOS_UINT16 chkLen       = 0;
    VOS_UINT16 lenTemp      = 0;
    VOS_BOOL   bIsEqual     = FALSE;
    VOS_BOOL   bIsEqualNoSp = FALSE;

    if (*len == 0) {
        return AT_FAILURE;
    }

    /* ???? */
    while (chkLen++ < *len) {
        if (*read == '=') {
            bIsEqual = TRUE;
        } else {
            if ((bIsEqual != TRUE) || (bIsEqualNoSp == TRUE)) {
                /* ???????????????????????????????????????????????? */
            } else {
                if (*read == ' ') {
                    read++;
                    continue;
                } else {
                    bIsEqualNoSp = TRUE;
                }
            }
        }

        *write++ = *read;
        lenTemp++;

        read++;
    }

    *len = lenTemp;
    return AT_SUCCESS;
}


VOS_UINT32 FormatCmdStr(VOS_UINT8 *data, VOS_UINT16 *len, VOS_UINT8 atS3)
{
    VOS_UINT8 *check    = data;
    VOS_UINT32 chkS3Len = 0;
    VOS_UINT32 flg      = 0;
    VOS_UINT16 lenTemp  = 0;

    /* ?????????????????????? */
    while (chkS3Len++ < *len) {
        if (atS3 == *check++) {
            flg = 1;
            break;
        } else {
            lenTemp++;
        }
    }

    if (flg != 1) {
        return AT_FAILURE;
    } else {
        *len = lenTemp;
        return AT_SUCCESS;
    }
}


VOS_UINT32 AT_DiscardInvalidChar(VOS_UINT8 *data, VOS_UINT32 dataLength, VOS_UINT16 *len)
{
    VOS_UINT16 i;
    VOS_UINT16 lenTemp;
    errno_t    returnValue;

    if ((*len == 0) || (*len > AT_CMD_MAX_LEN)) {
        PS_PRINTF_WARNING("<AT_DiscardInvalidChar> usLen > AT_CMD_MAX_LEN or usLen = 0. usLen :%d", *len);
        return ERR_MSP_FAILURE;
    }
    for (i = 0; (i + 1) < (*len); i++) {
        if (At_CheckCharA(data[i]) != AT_SUCCESS) {
            continue;
        } else if (At_CheckCharT(data[i + 1]) != AT_SUCCESS) {
            continue;
        } else {
            if (i != 0) {
                lenTemp     = (*len - i);
                returnValue = memmove_s(data, dataLength, (data + i), lenTemp);
                TAF_MEM_CHK_RTN_VAL(returnValue, dataLength, lenTemp);
                *len = lenTemp;
            }

            return ERR_MSP_SUCCESS;
        }
    }

    return ERR_MSP_FAILURE;
}


VOS_UINT32 CmdStringFormat(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT32 ret;

    ret = FormatCmdStr(data, len, g_atS3);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_ERROR;
    }

    /* ?????????? */
    ret = ScanDelChar(data, len, g_atS5);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING;
    }

    /* ?????????? */
    ret = ScanCtlChar(data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING;
    }

    /* ?????????? */
    ret = ScanBlankChar(data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING;
    }

    /* ?????????????????????????? */
    ret = ScanBlankCharAfterEq(data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING;
    }

    return ERR_MSP_SUCCESS;
}

VOS_BOOL CheckAnyCharCmdName(VOS_UINT8 *data, VOS_UINT16 *len, VOS_UINT8 *plName, VOS_UINT8 *pbName)
{
    VOS_UINT16 i;
    VOS_UINT8  bFound = 0; /* check if found string ANY CHAR CMD */
    VOS_UINT8  offset = 0;
    VOS_UINT8 *header = data; /* remember the header of the pointer */
    errno_t    returnValue;

    for (i = 0; i < *len; i++, header++) {
        if (*header == ' ') {
            continue; /* skip space char */
        } else if ((plName[offset] == *header) || (pbName[offset] == *header)) {
            offset++;

            if (plName[offset] == '?') {
                bFound = 1; /* found string ANY CHAR CMD  */
            }
        } else {
            break; /* not ANY CHAR CMD, go out */
        }
    }

    if (bFound) {
        if ((VOS_StrLen((VOS_CHAR *)plName) - 1) == offset) { /* found string ANY CHAR CMD */
            *len -= i;                                       /* remove string "AT^CPBW=" */
            if (*len > 0) {
                returnValue = memmove_s(data, *len, data + i, *len);
                TAF_MEM_CHK_RTN_VAL(returnValue, *len, *len);
            }
            return TRUE;
        }
    }

    return FALSE;
}


#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
VOS_UINT32 AnyCharCmdParse(VOS_UINT8 *data, VOS_UINT16 len, VOS_UINT8 *name)
{
    VOS_UINT8 *para     = VOS_NULL_PTR;           /* ?????????????????????????? */
    VOS_UINT16 paraLen  = 0;   /* ?????????????????????? */
    errno_t    memResult;
    VOS_UINT16 i;
    VOS_UINT16 j;
    VOS_UINT8  bInQoute = 0;

    AT_ResetParseVariable(); /* ???????????????????????? */

    para = (TAF_UINT8 *)VOS_MemAlloc(WUEPS_PID_AT, DYNAMIC_MEM_PT, AT_PARA_MAX_LEN + 1);
    if (para == NULL) {
        AT_ERR_LOG("AnyCharCmdParse: Fail to malloc mem.");
        return AT_ERROR;
    }
    (VOS_VOID)memset_s(para, AT_PARA_MAX_LEN + 1, 0, AT_PARA_MAX_LEN + 1);

    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD; /* ???????????????? */
    g_atParseCmd.cmdFmtType = AT_EXTEND_CMD_TYPE;      /* ???????????? */

    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen((VOS_CHAR *)name) - AT_CMD_AT_LEN; /* ?????? ^CPBW ???? */

    /* pName??????????????????????????????2??????????????4??0 */
    memResult = memcpy_s(g_atParseCmd.cmdName.cmdName, sizeof(g_atParseCmd.cmdName.cmdName), name,
                         g_atParseCmd.cmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParseCmd.cmdName.cmdName), g_atParseCmd.cmdName.cmdNameLen);

    /* ^CPBW=[<index>][,<number>[,<type>[,<text>,<coding>]]] */
    /* ^SCPBW= (1-65535),(numa),(0-255),(numb),(0-255),(numc),(0-255),(numd),(0-255),(text),(0,1),(email) */
    /* +CPBW=[<index>][,<number>[,<type>[,<text>]]] */

    /* seperate para by char ',' */
    for (i = 0; i < len; i++, data++) {
        if (paraLen > AT_PARA_MAX_LEN) {
            VOS_MemFree(WUEPS_PID_AT, para);
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if ((*data == ' ') && (bInQoute == 0)) {
            continue; /* skip space char not in quatation */
        } else if (*data == ',') {
            if (bInQoute) {
                if (bInQoute == 1) {
                    /* ??????','???????????????????????????????? */
                    para[paraLen] = *data;
                    paraLen++;
                    continue;
                }

                /* ???????????????????????????????????????????????????????????? , ????????????','?????????????????? */
                bInQoute = 0;

                /* ???????????? : ??????????????','?????????????? */
                for (j = paraLen - 1; j > 0; j--) {
                    if (para[j] == ' ') {
                        paraLen--;
                    } else if (para[j] == '"') {
                        break;
                    } else {
                        VOS_MemFree(WUEPS_PID_AT, para);
                        return AT_CME_INCORRECT_PARAMETERS; /* ?????????????????????????????????????? */
                    }
                }
            }

            if (i < (len - 1)) {
                if (atfwParseSaveParam(para, paraLen) != ERR_MSP_SUCCESS) {
                    VOS_MemFree(WUEPS_PID_AT, para);
                    return AT_ERROR;
                }
                paraLen = 0;
                /* too many para */
            }
            continue;
        } else {
            /* ???????????????????? */
            para[paraLen] = *data;
            paraLen++;

            if (*data == '"') {
                bInQoute++;
            }
        }
    }

    if (atfwParseSaveParam(para, paraLen) != ERR_MSP_SUCCESS) {
        VOS_MemFree(WUEPS_PID_AT, para);
        return AT_ERROR;
    }

    VOS_MemFree(WUEPS_PID_AT, para);
    return AT_SUCCESS;
}
#else
VOS_UINT32 AnyCharCmdParse(VOS_UINT8 *data, VOS_UINT16 len, VOS_UINT8 *name)
{
    VOS_UINT16 i, j;
    VOS_UINT8  bInQoute = 0;

    VOS_UINT8  para[AT_PARA_MAX_LEN + 1] = {0}; /* ?????????????????????????? */
    VOS_UINT16 paraLen                   = 0;   /* ?????????????????????? */
    errno_t    memResult;

    AT_ResetParseVariable(); /* ???????????????????????? */

    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD; /* ???????????????? */
    g_atParseCmd.cmdFmtType = AT_EXTEND_CMD_TYPE;      /* ???????????? */

    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen((VOS_CHAR *)name) - AT_CMD_AT_LEN; /* ?????? ^CPBW ???? */

    /* pName??????????????????????????????2??????????????4??0 */
    memResult = memcpy_s(g_atParseCmd.cmdName.cmdName, sizeof(g_atParseCmd.cmdName.cmdName), name,
                         g_atParseCmd.cmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParseCmd.cmdName.cmdName), g_atParseCmd.cmdName.cmdNameLen);

    /* ^CPBW=[<index>][,<number>[,<type>[,<text>,<coding>]]] */
    /* ^SCPBW= (1-65535),(numa),(0-255),(numb),(0-255),(numc),(0-255),(numd),(0-255),(text),(0,1),(email) */
    /* +CPBW=[<index>][,<number>[,<type>[,<text>]]] */

    /* seperate para by char ',' */
    for (i = 0; i < len; i++, data++) {
        if (paraLen > AT_PARA_MAX_LEN) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if ((*data == ' ') && (bInQoute == 0)) {
            continue; /* skip space char not in quatation */
        } else if (*data == ',') {
            if (bInQoute) {
                if (bInQoute == 1) {
                    /* ??????','???????????????????????????????? */
                    para[paraLen] = *data;
                    paraLen++;

                    continue;
                }

                /* ???????????????????????????????????????????????????????????? , ????????????','?????????????????? */
                bInQoute = 0;

                /* ???????????? : ??????????????','?????????????? */
                for (j = paraLen - 1; j > 0; j--) {
                    if (para[j] == ' ') {
                        paraLen--;
                    } else if (para[j] == '"') {
                        break;
                    } else {
                        return AT_CME_INCORRECT_PARAMETERS; /* ?????????????????????????????????????? */
                    }
                }
            }

            if (i < (len - 1)) {
                if (atfwParseSaveParam(para, paraLen) != ERR_MSP_SUCCESS) {
                    return AT_ERROR;
                }

                paraLen = 0;

                /* too many para */
            }

            continue;
        } else {
            /* ???????????????????? */
            para[paraLen] = *data;
            paraLen++;

            if (*data == '"') {
                bInQoute++;
            }
        }
    }

    if (atfwParseSaveParam(para, paraLen) != ERR_MSP_SUCCESS) {
        return AT_ERROR;
    }

    return AT_SUCCESS;
}
#endif


VOS_UINT32 AnyCharCmdProc(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT16 i;
    VOS_UINT32 ret                                                        = 0;
    VOS_UINT8  cmdNameLowercase[AT_CMD_NAME_MAX_NUM][AT_CMD_NAME_MAX_LEN] = { "at^cpbw=?", "at+cpbw=?", "at^scpbw=?" };
    VOS_UINT8  cmdNameUppercase[AT_CMD_NAME_MAX_NUM][AT_CMD_NAME_MAX_LEN] = { "AT^CPBW=?", "AT+CPBW=?", "AT^SCPBW=?" };

    for (i = 0; i < AT_CMD_NAME_MAX_NUM; i++) {
        if (CheckAnyCharCmdName(data, len, cmdNameLowercase[i], cmdNameUppercase[i]) == TRUE) {
            ret = FormatCmdStr(data, len, g_atS3);
            if (ret != ERR_MSP_SUCCESS) {
                return AT_ERROR;
            }

            return AnyCharCmdParse(data, *len, &(cmdNameUppercase[i][AT_CMD_NAME_COLUMNS_INDEX]));
        }
    }

    return AT_FAILURE;
}


VOS_VOID At_ReadyClientCmdProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT32 dataLength, VOS_UINT16 len)
{
    VOS_UINT32           ret;
    AT_RreturnCodeUint32 result;
    AT_ParseContext     *clientContext = NULL;

    if (len < AT_CMD_MIN_LEN) {
        return;
    }

    clientContext = &(g_parseContext[indexNum]);

    SaveRepeatCmd(clientContext, data, len);

    At_ResetCombinParseInfo(indexNum);

    ret = AT_DiscardInvalidChar(data, dataLength, &len);
    if (ret != ERR_MSP_SUCCESS) {
        return;
    }

    ret = AnyCharCmdProc(data, &len);

    if (ret == AT_SUCCESS) { /* ???????????????????? */
        /* ???????? */
        ret = CmdParseProc(indexNum, data, len);
        if ((ret != AT_OK) && (ret != AT_WAIT_ASYNC_RETURN)) {
            At_ResetCombinParseInfo(indexNum);
        }

        At_FormatResultData(indexNum, ret);

        return;
    } else if (ret != AT_FAILURE) { /* ???????????????????????????????????? */
        At_FormatResultData(indexNum, ret);

        return;
    } else { /* ?????????????????????? */
        /* do nothing */
    }

    ret = CmdStringFormat(indexNum, data, &len);
    if (ret != ERR_MSP_SUCCESS) {
        At_FormatResultData(indexNum, ret);

        return;
    }

    /* ??????"AT"???????? */
    if (len < AT_PARSE_AT_LEN) {
        return;
    }

    /* ????V3R1????????????????????"AT"???????????????? */
    if (At_CheckCharA(*data) != AT_SUCCESS) { /* ????????'A'/'a' */
        return;
    }

    if (At_CheckCharT(*(data + 1)) != AT_SUCCESS) { /* ????????'T'/'t' */
        return;
    }

    /* SIMLOCK??????????61???????????????? */
    if (At_ProcSimLockPara(indexNum, data, len) == AT_SUCCESS) {
        return;
    }

    /* ^DOCK????????????"^"??"="?????????? */
    if (AT_HandleDockSetCmd(indexNum, data, len) == AT_SUCCESS) {
        return;
    }

    /*
     * AP-Modem????????????????AT^FACAUTHPUBKEY??AT^SIMLOCKDATAWRITE??????????????????????????????????
     * ????????????512??????????????
     */
    if (At_HandleApModemSpecialCmd(indexNum, data, len) == AT_SUCCESS) {
        return;
    }

    result = (AT_RreturnCodeUint32)At_CombineCmdChkProc(indexNum, data, len);

    /* ???????? */
    if ((result == AT_FAILURE) || (result == AT_SUCCESS)) {
        return;
    }

    if (result != AT_WAIT_ASYNC_RETURN) {
        At_FormatResultData(indexNum, result);
    }

    return;
}


VOS_VOID AT_ProcSmsStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt)
{
    AT_RreturnCodeUint32 result;

    result = (AT_RreturnCodeUint32)At_SmsProc(clientId, data, length);

    if ((result == AT_SUCCESS) || (result == AT_WAIT_SMS_INPUT)) {
        At_FormatResultData(clientId, result);
        return;
    }

    g_atClientTab[clientId].smsTxtLen = 0; /* ????BUFFER???? */

    At_SetCmdSubMode(clientId, AT_NORMAL_MODE);

    if (result == AT_WAIT_ASYNC_RETURN) {
        if (clientCxt->cmdElement != NULL) {
            if (At_StartTimer(clientCxt->cmdElement->setTimeOut, clientId) != AT_SUCCESS) {
                AT_ERR_LOG("atCmdMsgProc():ERROR:Start Timer Failed");
            }

            clientCxt->clientStatus = AT_FW_CLIENT_STATUS_PEND;
        }
        return;
    }

    At_FormatResultData(clientId, result);
}


VOS_VOID AT_ProcAgpsStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt)
{
    AT_RreturnCodeUint32 result;
    AT_ModemAgpsCtx     *agpsCtx = VOS_NULL_PTR;

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(clientId);
    result  = (AT_RreturnCodeUint32)At_ProcXmlText(clientId, data, length);

    /* ????????????????????">" */
    if (result == AT_WAIT_XML_INPUT) {
        At_FormatResultData(clientId, result);
        return;
    }

    /* ?????????? */
    PS_MEM_FREE(WUEPS_PID_AT, agpsCtx->xml.xmlTextHead); /* XML BUFFER???? */
    agpsCtx->xml.xmlTextHead = VOS_NULL_PTR;
    agpsCtx->xml.xmlTextCur  = VOS_NULL_PTR;

    At_SetCmdSubMode(clientId, AT_NORMAL_MODE);

    if (result == AT_WAIT_ASYNC_RETURN) {
        if (clientCxt->cmdElement != NULL) {
            if (At_StartTimer(clientCxt->cmdElement->setTimeOut, clientId) != AT_SUCCESS) {
                AT_ERR_LOG("AT_ProcAgpsStream():ERROR:Start Timer Failed");
            }

            clientCxt->clientStatus = AT_FW_CLIENT_STATUS_PEND;
        }
        return;
    }

    At_FormatResultData(clientId, result);
}


VOS_VOID atCmdMsgProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT32 dataLength, VOS_UINT16 len)
{
    VOS_UINT32       ret;
    AT_ParseContext *parseCtx = VOS_NULL_PTR;

    if ((len == 0) || (len > AT_CMD_MAX_LEN)) {
        return;
    }

    parseCtx = &(g_parseContext[indexNum]);

    switch (parseCtx->mode) {
        case AT_SMS_MODE:
            AT_ProcSmsStream(indexNum, data, len, parseCtx);
            return;

        case AT_XML_MODE:
            AT_ProcAgpsStream(indexNum, data, len, parseCtx);
            return;

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case AT_UE_POLICY_MODE:
            At_ProcUePolicyInfoStream(indexNum, data, len, parseCtx);
            return;

        case AT_IMS_URSP_MODE:
            At_ProcImsUrspStream(indexNum, data, len, parseCtx);
            return;
#endif
        default:
            break;
    }

    /* ???????????????????????????????????????? */
    ret = AT_ParseCmdIsComb(indexNum, data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return;
    }

    /* ??????????PEND?????????? */
    ret = AT_ParseCmdIsPend(indexNum, data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return;
    }

    At_ReadyClientCmdProc(indexNum, data, dataLength, len);

    return;
}


VOS_UINT32 At_CmdStreamRcv(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8       *head          = NULL;
    VOS_UINT16       count         = 0; /* ?????????????????? */
    VOS_UINT16       total         = 0; /* ?????????????????? */
    AT_ParseContext *clientContext = NULL;
    errno_t          memResult;

    /* ucIndex???????????????????????????????? */
    if ((data == NULL) || (indexNum >= AT_MAX_CLIENT_NUM)) {
        return ERR_MSP_INVALID_PARAMETER;
    }

    clientContext = &(g_parseContext[indexNum]);

    head = data;

    /* ????????(<CR>????????????) */
    while (count++ < len) {
        if (At_CheckCmdSms(*(data + count - 1), clientContext->mode)) {
            /* ???????????????? */
            if (clientContext->dataLen > 0) {
                if ((clientContext->dataLen + count) >= AT_CMD_MAX_LEN) {
                    clientContext->dataLen = 0;
                    return ERR_MSP_INSUFFICIENT_BUFFER;
                }

                memResult = memcpy_s(&clientContext->dataBuff[clientContext->dataLen],
                                     AT_CMD_MAX_LEN - clientContext->dataLen, head, count);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN - clientContext->dataLen, count);
                clientContext->dataLen += count;
            } else { /* ???????????? */
                memResult = memcpy_s(&clientContext->dataBuff[0], AT_CMD_MAX_LEN, head, (VOS_SIZE_T)(count - total));
                TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN, (VOS_SIZE_T)(count - total));
                clientContext->dataLen = count - total;
            }

            RepeatCmdProc(clientContext);

            atCmdMsgProc(indexNum, clientContext->dataBuff, sizeof(clientContext->dataBuff), clientContext->dataLen);

            head                   = data + count;
            total                  = count;
            clientContext->dataLen = 0;
        }
    }

    /* ?????????????????????????? */
    if (total < len) {
        if ((clientContext->dataLen + (len - total)) >= AT_CMD_MAX_LEN) {
            clientContext->dataLen = 0;
            return ERR_MSP_INSUFFICIENT_BUFFER;
        }

        memResult = memcpy_s(&clientContext->dataBuff[clientContext->dataLen], AT_CMD_MAX_LEN - clientContext->dataLen,
                             head, (VOS_UINT32)(len - total));
        TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN - clientContext->dataLen, (VOS_UINT32)(len - total));

        clientContext->dataLen += (VOS_UINT16)(len - total); /* (pData-pHead+ulLen); */
    }

    return ERR_MSP_SUCCESS;
}


VOS_VOID At_CombineBlockCmdProc(VOS_UINT8 indexNum)
{
    errno_t               memResult;
    AT_RreturnCodeUint32  result         = AT_WAIT_ASYNC_RETURN;
    AT_ParseContext      *clientContext  = NULL;
    AT_FW_CombineCmdInfo *combineCmdInfo = NULL;
    VOS_UINT8            *blockCmd       = NULL;
    VOS_UINT16            blockCmdLen    = 0;

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        return;
    }

    clientContext = &(g_parseContext[indexNum]);

    combineCmdInfo = &clientContext->combineCmdInfo;

    /* ???????????????????????? */
    if ((combineCmdInfo->procNum < combineCmdInfo->totalNum)) {
        /* ?????????????? */
        result = (AT_RreturnCodeUint32)At_CombineCmdProc(indexNum);

        /* ???????? */
        if ((result == AT_FAILURE) || (result == AT_SUCCESS)) {
            return;
        }

        if (result != AT_WAIT_ASYNC_RETURN) {
            At_FormatResultData(indexNum, result);
        }

        return;
    }

    /* ???????????????????? */
    indexNum = AT_BlockCmdCheck();
    if (indexNum < AT_MAX_CLIENT_NUM) {
        clientContext = &(g_parseContext[indexNum]);

        /* ???????????????????? */
        AT_StopRelTimer(indexNum, &clientContext->timer);

        blockCmdLen = clientContext->blockCmdLen;

        if (blockCmdLen == 0) {
            return;
        }

        blockCmd    = (VOS_UINT8 *)AT_MALLOC(blockCmdLen);
        if (blockCmd == NULL) {
            /* ???????????????????????????????????? */
            return;
        }

        (VOS_VOID)memset_s(blockCmd, blockCmdLen, 0x00, blockCmdLen);

        if (blockCmdLen > 0) {
            memResult = memcpy_s(blockCmd, blockCmdLen, clientContext->blockCmd, blockCmdLen);
            TAF_MEM_CHK_RTN_VAL(memResult, blockCmdLen, blockCmdLen);
        }

        AT_ClearBlockCmdInfo(indexNum);

        /* ???????????? */
        At_ReadyClientCmdProc(indexNum, blockCmd, blockCmdLen, blockCmdLen);

        AT_FREE(blockCmd);

        return;
    }

    return;
}


VOS_VOID At_CmdMsgDistr(struct MsgCB *msg)
{
    AT_CmdMsgNumCtrl *msgNumCtrlCtx = VOS_NULL_PTR;
    AT_Msg           *msgTemp       = VOS_NULL_PTR;
    VOS_UINT16        len;
    VOS_UINT8        *data = NULL;
    VOS_UINT8         indexNum;
    VOS_ULONG         lockLevel;

    msgTemp = (AT_Msg *)msg;

    if (msgTemp->type == AT_NORMAL_TYPE_MSG) {
        msgNumCtrlCtx = AT_GetMsgNumCtrlCtxAddr();

        lockLevel = 0;
        /* ???????? */
        /*lint -e571*/
        VOS_SpinLockIntLock(&(msgNumCtrlCtx->spinLock), lockLevel);
        /*lint +e571*/

        if (msgNumCtrlCtx->msgCount > 0) {
            msgNumCtrlCtx->msgCount--;
        }

        VOS_SpinUnlockIntUnlock(&(msgNumCtrlCtx->spinLock), lockLevel);
    }

    len      = msgTemp->len;
    data     = msgTemp->value;
    indexNum = msgTemp->indexNum;

#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    if (msgTemp->type == AT_ATNLPROXY_MSG) {
        AtNlProxyDealwithUspaceData((VOS_VOID*)data, (VOS_INT32)len);
        return;
    }
#endif

#if (FEATURE_AT_PROXY == FEATURE_ON)
    if (msgTemp->type == AT_ATP_MSG) {
        AT_ProcAtpMsg((VOS_VOID*)data, (VOS_UINT32)len);
        return;
    }
#endif

    /* C??/HIFI????????????????AT??AT???????? */
    switch (msgTemp->type) {
        case ID_CCPU_AT_RESET_START_IND:
            AT_RcvCcpuResetStartInd((struct MsgCB *)msgTemp);
            return;

        case ID_CCPU_AT_RESET_END_IND:
            AT_RcvCcpuResetEndInd((struct MsgCB *)msgTemp);
            return;

        case ID_HIFI_AT_RESET_BEGIN_IND:
            AT_RcvHifiResetBeginInd((struct MsgCB *)msgTemp);
            return;

        case ID_HIFI_AT_RESET_END_IND:
            AT_RcvHifiResetEndInd((struct MsgCB *)msgTemp);
            return;

        default:
            break;
    }

    AT_AddUsedClientId2Tab(indexNum);

    /* ????????C????????????????????AT???? */
    if (AT_GetResetFlag() == VOS_TRUE) {
        return;
    }


    if (msgTemp->type == AT_COMBIN_BLOCK_MSG) {
        At_CombineBlockCmdProc(indexNum);
        return;
    }

    At_CmdStreamRcv(indexNum, data, len);

    return;
}


VOS_UINT32 atfwParseSaveParam(VOS_UINT8 *stringBuf, VOS_UINT16 len)
{
    errno_t memResult;

    if ((len > AT_PARA_MAX_LEN) || (g_atParaIndex >= AT_MAX_PARA_NUMBER)) {
        return ERR_MSP_FAILURE;
    }

    /* ??????????0???????????????????????????????????? */
    if ((stringBuf == NULL) && (len != 0)) {
        return ERR_MSP_FAILURE;
    }

    /* ????????????????0?????????????? */
    if ((len != 0) && (stringBuf != NULL)) {
        memResult = memcpy_s(g_atParaList[g_atParaIndex].para, sizeof(g_atParaList[g_atParaIndex].para), stringBuf,
                             len);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParaList[g_atParaIndex].para), len);
    }

    g_atParaList[g_atParaIndex].paraLen = len;
    g_atParaIndex++;

    return ERR_MSP_SUCCESS;
}


VOS_UINT32 AT_SaveCdataDialParam(VOS_UINT8 *stringBuf, VOS_UINT16 len)
{
    VOS_UINT8 *currPtr = stringBuf;
    errno_t    memResult;
    VOS_UINT16 length;

    if ((len > AT_PARA_MAX_LEN) || (stringBuf == NULL)) {
        return ERR_MSP_FAILURE;
    }
    /*
     * ??????????"*99#", "*98#",
     * ??"#777"????????:
     * #,
     * #777,
     * 777
     */

    if (len > AT_CDATA_DIAL_777_LEN) {
        /* ????????'D' */
        currPtr++;

        /* ????'#' */
        g_atParaList[g_atParaIndex].para[0] = *currPtr;
        g_atParaList[g_atParaIndex].paraLen = 1;

        /* ????'#777' */
        g_atParaIndex++;
        length    = len - 1;
        memResult = memcpy_s(g_atParaList[g_atParaIndex].para, sizeof(g_atParaList[g_atParaIndex].para), currPtr,
                             length);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParaList[g_atParaIndex].para), length);
        g_atParaList[g_atParaIndex].paraLen = length;

        /* ????'777' */
        currPtr++;
        g_atParaIndex++;
        length--;
        memResult = memcpy_s(g_atParaList[g_atParaIndex].para, sizeof(g_atParaList[g_atParaIndex].para), currPtr,
                             length);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParaList[g_atParaIndex].para), length);
        g_atParaList[g_atParaIndex].paraLen = length;
    }

    g_atParaIndex++;

    return ERR_MSP_SUCCESS;
}



VOS_UINT32 At_CmdTestProcOK(VOS_UINT8 indexNum)
{
    return AT_OK;
}


VOS_UINT32 At_CmdTestProcERROR(VOS_UINT8 indexNum)
{
    return AT_ERROR;
}



VOS_UINT32 AT_IsAbortCmdStr(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_ABORT_CmdPara       *abortCmdPara = VOS_NULL_PTR;
    VOS_UINT32              abortCmdStrLen;
    VOS_UINT32              atCurrSetTick;
    AT_CMD_ABORT_TICK_INFO *cmdAbortTick = VOS_NULL_PTR;
    VOS_UINT32              timeInternal;

    atCurrSetTick = VOS_GetTick();
    cmdAbortTick  = At_GetAtCmdAbortTickInfo();

    /* ??????????????????????????????(<0x20??ASCII????),????, S3, S5??????. ?????????????????? */

    abortCmdStrLen = 0;

    /* ?????????????????? */
    abortCmdPara = AT_GetAbortCmdPara();

    if (abortCmdPara == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    /* ???????????????????????? */
    if (abortCmdPara->abortEnableFlg != VOS_TRUE) {
        return VOS_FALSE;
    }

    /* ????????????tick??????????????125ms????,???????????????????????????? */
    if (atCurrSetTick >= cmdAbortTick->atSetTick[indexNum]) {
        timeInternal = atCurrSetTick - cmdAbortTick->atSetTick[indexNum];
    } else {
        timeInternal = atCurrSetTick + (AT_MAX_TICK_TIME_VALUE - cmdAbortTick->atSetTick[indexNum]);
    }

    if (timeInternal < AT_MIN_ABORT_TIME_INTERNAL) {
        return VOS_FALSE;
    }

    /* ?????????????????????????????????????????????????? */
    if (At_GetAtCmdAnyAbortFlg() == VOS_TRUE) {
        return VOS_TRUE;
    }

    /* ???????? */
    abortCmdStrLen = VOS_StrLen((VOS_CHAR *)abortCmdPara->abortAtCmdStr);
    if ((len == 0) || (len != abortCmdStrLen)) {
        return VOS_FALSE;
    }

    /* ????????????????????????????????AT???????????????????????????????? */
    if ((VOS_StrNiCmp((VOS_CHAR *)data, (VOS_CHAR *)abortCmdPara->abortAtCmdStr, len) == 0)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID AT_AbortCmdProc(VOS_UINT8 indexNum)
{
    VOS_UINT32 timerLen;
    VOS_UINT32 result;

    /* ?????????? */
    if (g_parseContext[indexNum].cmdElement == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_AbortCmdProc: pstCmdElement NULL.");
        return;
    }

    if (g_parseContext[indexNum].cmdElement->abortProc == VOS_NULL_PTR) {
        return;
    }

    /* ??????????????????AT??????Abort???? */
    result = g_parseContext[indexNum].cmdElement->abortProc(indexNum);
    if (result == AT_WAIT_ASYNC_RETURN) {
        /* ????????AT??????????????????PEND????, ???????????????????????? */

        /* ??????????????????, ????????????AT????????ABORT */
        timerLen = g_parseContext[indexNum].cmdElement->abortTimeOut;
        if (timerLen == 0) {
            AT_WARN_LOG("AT_AbortCmdProc: TimerLen 0.");
            return;
        }

        /* ???????????? */
        if (At_StartTimer(timerLen, indexNum) != AT_SUCCESS) {
            AT_WARN_LOG("AT_AbortCmdProc: StartTimer Err.");
            return;
        }
    }
    /* ????AT???????? ???????????????? */
    else if ((result == AT_ABORT) || (result == AT_OK)) {
        result                  = AT_ABORT;
        g_atSendDataBuff.bufLen = 0;

        AT_STOP_TIMER_CMD_READY(indexNum);

        At_FormatResultData(indexNum, result);
    }
    /*
     * ???????????????????????? ????????????????????????????????????????
     * (??????????????????????????, ??????????????????????)???????? ????????????
     */
    else {
        AT_WARN_LOG1("AT_AbortCmdProc, WARNING, Return Code %d!", result);
    }
}


AT_ParCmdElement* AT_GetCmdElementInfo(VOS_UINT8 *cmdName, VOS_UINT32 cmdType)
{
    HI_LIST_S        *curList    = VOS_NULL_PTR;
    HI_LIST_S        *cmdTblList = VOS_NULL_PTR;
    AT_ParCmdtblList *curCmdNode = VOS_NULL_PTR;
    VOS_UINT32        i;

    /* ?????????????????????????????? */

    cmdTblList = &(g_cmdTblList);

    /* ?????????????????????????????????????????????????????? */
    if ((cmdType != AT_EXTEND_CMD_TYPE) && (cmdType != AT_BASIC_CMD_TYPE)) {
        return VOS_NULL_PTR;
    }

    /* ???????????????????????? */
    msp_list_for_each(curList, cmdTblList)
    {
        curCmdNode = msp_list_entry(curList, AT_ParCmdtblList, cmdTblList);

        for (i = 0; i < curCmdNode->cmdNum; i++) {
            /* ?????????????????????????????????? */
            if ((curCmdNode->cmdElement == NULL) || (curCmdNode->cmdElement[i].cmdName == NULL)) {
                continue;
            }
            /* ?????????????????????????? */
            if (AT_STRCMP((VOS_CHAR *)cmdName, (VOS_CHAR *)curCmdNode->cmdElement[i].cmdName) == ERR_MSP_SUCCESS) {
                return &(curCmdNode->cmdElement[i]);
            }
        }
    }

    return VOS_NULL_PTR;
}


VOS_VOID AT_SaveCmdElementInfo(VOS_UINT8 indexNum, VOS_UINT8 *cmdName, VOS_UINT32 cmdType)
{
    AT_ParCmdElement *getCmdElemnet = VOS_NULL_PTR;

    if (cmdName == VOS_NULL_PTR) {
        return;
    }

    /* ?????????????????????????????????? */
    getCmdElemnet = AT_GetCmdElementInfo(cmdName, cmdType);
    if (getCmdElemnet == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SaveCmdElementInfo: GetCmdElementInfo Failed.");
        return;
    }

    /* ?????????????????????? */
    g_parseContext[indexNum].cmdElement = getCmdElemnet;

    return;
}


VOS_UINT32 AT_IsAnyParseClientPend(VOS_VOID)
{
    AT_PortBuffCfgInfo *portBuffCfg = VOS_NULL_PTR;
    VOS_UINT32          i;
    VOS_UINT8           clientIndex;

    portBuffCfg = AT_GetPortBuffCfgInfo();

    if (portBuffCfg->num > AT_MAX_CLIENT_NUM) {
        portBuffCfg->num = AT_MAX_CLIENT_NUM;
    }

    /* ?????????????????????????????? */
    for (i = 0; i < portBuffCfg->num; i++) {
        clientIndex = (VOS_UINT8)portBuffCfg->usedClientId[i];

        if (clientIndex >= AT_CLIENT_ID_BUTT) {
            continue;
        }

        if (g_parseContext[clientIndex].clientStatus == AT_FW_CLIENT_STATUS_PEND) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_IsAllClientDataMode(VOS_VOID)
{
    AT_PortBuffCfgInfo *portBuffCfg = VOS_NULL_PTR;
    VOS_UINT32          i;
    VOS_UINT8           clientIndex;
    VOS_UINT32          dataModeNum;
    DMS_PortModeUint8   mode;

    dataModeNum = 0;
    portBuffCfg = AT_GetPortBuffCfgInfo();

    if (portBuffCfg->num > AT_MAX_CLIENT_NUM) {
        portBuffCfg->num = AT_MAX_CLIENT_NUM;
    }

    /* ????????????????????data???? */
    for (i = 0; i < portBuffCfg->num; i++) {
        clientIndex = (VOS_UINT8)portBuffCfg->usedClientId[i];
        if (clientIndex >= AT_CLIENT_ID_BUTT) {
            continue;
        }

        mode = DMS_PORT_GetMode(g_atClientTab[clientIndex].portNo);
        if (mode == DMS_PORT_MODE_ONLINE_DATA) {
            dataModeNum++;
        }
    }

    /* ??????????????data???? */
    if (dataModeNum == portBuffCfg->num) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT8 atCmdIsSupportedByCLMode(VOS_UINT8 indexNum)
{
    AT_ParCmdElement *cmdElement = VOS_NULL_PTR;
    VOS_UINT32        loop;
    VOS_UINT32        cmdArraySize;
    const VOS_UINT32  cmdIndex[] = {
        /*
         * AT_CMD_CLIP,
         * AT_CMD_DTMF,
         */
        AT_CMD_CARDMODE,

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
        AT_CMD_CSCB,
#endif
        /* AT_CMD_CNMI, */

        /* AT_CMD_CSASM, */
        AT_CMD_CUSD,
        /*
         * AT_CMD_CMGW,
         * AT_CMD_CMGD,
         */
        AT_CMD_CCWA,
        AT_CMD_CCFC,
        /* AT_CMD_CLIR, */

        AT_CMD_CELLINFO,
        AT_CMD_CPMS,
        AT_CMD_RELEASERRC,
        AT_CMD_EOPLMN,
        AT_CMD_CNMR
        /*  ????Volte??????????at???????????? */
    };

    cmdArraySize = sizeof((cmdIndex)) / sizeof((cmdIndex[0]));

    cmdElement = g_parseContext[indexNum].cmdElement;

    /* CLCK????????????SC??FD???????????????????????? */
    if (cmdElement->cmdIndex == AT_CMD_CLCK) {
        if ((g_atParaList[0].paraValue == AT_CLCK_SC_TYPE) || (g_atParaList[0].paraValue == AT_CLCK_FD_TYPE)) {
            return VOS_FALSE;
        } else {
            return VOS_TRUE;
        }
    }

    /* CPWD????????????SC??FD???????????????????????? */
    if (cmdElement->cmdIndex == AT_CMD_CPWD) {
        if ((g_atParaList[0].paraValue == AT_CLCK_SC_TYPE) || (g_atParaList[0].paraValue == AT_CLCK_P2_TYPE)) {
            return VOS_FALSE;
        } else {
            return VOS_TRUE;
        }
    }

    /* ??????????AT??????cmdindex????????????????CL??????????????AT??????index */
    for (loop = 0; loop < cmdArraySize; loop++) {
        if (cmdElement->cmdIndex == cmdIndex[loop]) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT8 atCmdIsSupportedByGULMode(VOS_UINT8 indexNum)
{
    AT_ParCmdElement *cmdElement = VOS_NULL_PTR;
    VOS_UINT32        loop;
    VOS_UINT32        cmdArraySize;
    ModemIdUint16     modemId;
    const VOS_UINT32  cmdIndex[] = {
        AT_CMD_ECCALL, AT_CMD_ECCTRL, AT_CMD_ECCAP, AT_CMD_ECRANDOM, AT_CMD_ECKMC, AT_CMD_ECCTEST
    };

    modemId = MODEM_ID_0;

    /* ????modemid??????????????????????AT???? */
    if (AT_GetModemIdFromClient((VOS_UINT16)indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("atCmdIsSupportedByGULMode:Get Modem Id fail!");
        return VOS_TRUE;
    }

    AT_NORM_LOG1("atCmdIsSupportedByGULMode: enModemId:", (VOS_INT32)modemId);

    cmdArraySize = sizeof((cmdIndex)) / sizeof((cmdIndex[0]));

    cmdElement = g_parseContext[indexNum].cmdElement;

    /* ??????????AT??????cmdindex????????????????GUL??????????????????AT??????index */
    for (loop = 0; loop < cmdArraySize; loop++) {
        if (cmdElement->cmdIndex == cmdIndex[loop]) {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}
#endif

