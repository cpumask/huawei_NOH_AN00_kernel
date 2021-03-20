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

#include "AtNLProxyKernel.h"
#include "AtParse.h"
#include "ATCmdProc.h"
#include "AtDataProc.h"
#include "at_common.h"
#include "AtParseCore.h"
#include "AtCmdSupsProc.h"
#include "PsLogdef.h"
#include "securec.h"
#include "product_config.h"

#define THIS_FILE_ID PS_FILE_ID_AT_NLPROXYKERNEL_C

#if (FEATURE_IOT_ATNLPROXY== FEATURE_ON)

extern VOS_INT32 at_device_event_report(void *data, int len);

/* Ŀǰû��Ҫ������������ΪAT�Ǵ����ģ�����1024���·����С��1024�ϱ����� */
TAF_INT32 g_ATNL_rand = ATNL_MAGIC_LEN;
/* ��¼��ǰ�·�Ӧ�ò�AT�Ķ˿� */
TAF_INT32 g_current_user_at = -1;

AT_ParCmdElement* g_AppAtList = NULL;
VOS_UINT16 g_AppAtCount = 0;
TAF_CHAR g_NL_Transfer_Buf[sizeof(NLTRANSFER) + sizeof(AT_NL_DeviceEvent)];

VOS_INT32 g_registStatus[NL_DEVICE_ID_MAX_ID] = { 0 };

TAF_INT32 AL_NL_CopyParaFromATCore(AT_NL_ParseParaTypeStru *m_para_list,
    AT_ParseParaType *atParaList,char *paraPoolAddrBase,unsigned int offset,unsigned int *len)
{
    unsigned int paraLenCopy;
    errno_t memResult;

    paraLenCopy = atParaList->paraLen + OVER_CPY_PARANUM;  // ����ʱ���Ͻ�����

    if ((offset + paraLenCopy) > NL_AT_PARA_MAX_LEN_ALL) {
        return VOS_FALSE;
    }

    m_para_list->paraValue  = atParaList->paraValue;
    m_para_list->paralen    = atParaList->paraLen;
    m_para_list->paraLenCpy = (TAF_UINT16)paraLenCopy;

    m_para_list->byteOffsetInParaPool = offset;

    memResult = memcpy_s(paraPoolAddrBase + offset, NL_AT_PARA_MAX_LEN_ALL - offset, atParaList->para, paraLenCopy);
    TAF_MEM_CHK_RTN_VAL(memResult, (TAF_UINT32)(NL_AT_PARA_MAX_LEN_ALL - offset), (TAF_UINT32)paraLenCopy);

    *len = paraLenCopy;
    return VOS_TRUE;
}

/*
 * ��������: �ú������û�̬AT�����ִ�к���
 * ������λ���·�����������������ṹ��pEvent�У�����˳�����ħ���֣����ս�������û�̬
 */
VOS_UINT32 AtNlInnerProc(TAF_UINT8 ucIndex, AT_NL_TypeUint8 ucType)
{
    NLTRANSFER        *pnltrans     = NULL;
    AT_NL_DeviceEvent *pEvent       = NULL;
    TAF_UINT32         i            = 0;
    TAF_UINT32         ulCmdNameLen = 0;
    TAF_INT32          lRet;
    TAF_INT32          paraCpyOffset = 0;
    TAF_INT32          paraCpyLen    = 0;

    if (g_atParaIndex > NL_AT_MAX_PARA_NUMBER) {
        return VOS_FALSE;
    }

    /* ׼����ṹ��NL_DEVICE_EVENT��ʼ */
    (VOS_VOID)memset_s(g_NL_Transfer_Buf, sizeof(g_NL_Transfer_Buf), 0, sizeof(g_NL_Transfer_Buf));
    pEvent = (AT_NL_DeviceEvent *)g_NL_Transfer_Buf;

    pEvent->deviceId  = NL_DEVICE_ID_ATNLPROXY;
    pEvent->eventCode = (int)0;

    /* NL_DEVICE_EVENT�ṹ���е�data��len������NLTRANSFER */
    /* ���NLTRANSFER��ʼ */
    pnltrans          = (NLTRANSFER *)pEvent->data;
    pnltrans->index   = ucIndex;
    pnltrans->nlType  = ucType;
    pnltrans->optType = g_atParseCmd.cmdOptType;
    /* ħ���� */
    if (g_ATNL_rand > ATNL_MAX_MAGIC_LEN) {
        /* һֱ�ӵ���Խ�磬��ԭ */
        g_ATNL_rand = ATNL_MAGIC_LEN;
    }
    g_ATNL_rand++;
    pnltrans->magic = g_ATNL_rand;

    if (g_parseContext[ucIndex].cmdElement->cmdName != NULL) {
        ulCmdNameLen = VOS_StrLen((VOS_CHAR *)g_parseContext[ucIndex].cmdElement->cmdName);
        strncpy_s(pnltrans->atcmd, NL_MAX_AT_LEN, (VOS_CHAR *)g_parseContext[ucIndex].cmdElement->cmdName, ulCmdNameLen);
    }

    pnltrans->paraCount = g_atParaIndex;

    for (i = 0; i < g_atParaIndex; i++) {
        lRet = AL_NL_CopyParaFromATCore(&(pnltrans->paraList[i]),
                                        &g_atParaList[i],
                                        &pnltrans->atparapldpool[0],
                                        (TAF_UINT32)paraCpyOffset,
                                        (TAF_UINT32 *)&paraCpyLen);
        if (lRet != VOS_TRUE) {
            return VOS_FALSE;
        }
        paraCpyOffset += paraCpyLen;
    }
    /* ���NLTRANSFER���� */
    pEvent->len = sizeof(NLTRANSFER);
    /* ׼����ṹ��NL_DEVICE_EVENT���� */

    /* �Ƿ���ж�״̬ */
    for (i = 0; i < g_AppAtCount; i++) {
        if (VOS_StrCmp((VOS_CHAR *)g_AppAtList[i].cmdName, pnltrans->atcmd) == 0) {
            if (g_AppAtList[i].abortProc != NULL) {
                g_atClientTab[ucIndex].canAbort = 1;
            }
            break;
        }
    }
    /* ���õ�ǰ����ģʽ ͬʱҪ���������ط� */
    g_atClientTab[ucIndex].magic = pnltrans->magic;
    g_atClientTab[ucIndex].cmdCurrentOpt = AT_CMD_ATNLPROXY_CURRENT_OPT;
    g_atClientTab[AT_CLIENT_ID_APP].isWaitAts = 1;

    /* ��¼��ǰ�·�Ӧ�ò�AT�Ķ˿� */
    g_current_user_at = ucIndex;

    /* ������׼���õ�NLTRANSFER���͸�APP */
    lRet = at_device_event_report(g_NL_Transfer_Buf, sizeof(g_NL_Transfer_Buf));
    if (lRet != 0) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

TAF_UINT32 AtNlSetProc(TAF_UINT8 ucIndex)
{
    if (AtNlInnerProc(ucIndex, NL_TYPE_SET) == VOS_FALSE) {
        return AT_ERROR;
    }

    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 AtNlQryProc(TAF_UINT8 ucIndex)
{
    if (AtNlInnerProc(ucIndex, NL_TYPE_QUERY) == VOS_FALSE) {
        return AT_ERROR;
    }

    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 AtNlTestProc(TAF_UINT8 ucIndex)
{
    if (AtNlInnerProc(ucIndex, NL_TYPE_TEST) == VOS_FALSE) {
        return AT_ERROR;
    }

    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 AtNlAbortProc(TAF_UINT8 ucIndex)
{
    g_atClientTab[AT_CLIENT_ID_APP].isWaitAts = 0;
    g_current_user_at                         = -1;
    if (AtNlInnerProc(ucIndex, NL_TYPE_ABORT) == VOS_FALSE) {
        return AT_ERROR;
    }

    return AT_WAIT_ASYNC_RETURN;
}

VOS_VOID StrReplace(VOS_CHAR* dst, VOS_CHAR* src, VOS_CHAR* srch, VOS_CHAR* rep)
{
    TAF_CHAR  *pCur    = NULL;
    TAF_CHAR  *dst_pos = dst;
    TAF_CHAR  *src_pos = src;
    TAF_UINT32 rep_len;
    TAF_UINT32 srch_len;
    TAF_INT32  nLen = 0;
    errno_t    memResult;

    rep_len  = strlen(rep);
    srch_len = strlen(srch);

    do {
        pCur = strstr(src_pos, srch);

        if (pCur == NULL) {
            memResult = strcpy_s(dst_pos, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst)), src_pos);
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(memResult, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst)),
                (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst)));
            return;
        }

        /* ������һ���滻�����һ���滻���м���ַ��� */
        nLen = pCur - src_pos;
        if (nLen > 0) {
            memResult = memcpy_s(dst_pos, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst)), src_pos, (TAF_UINT32)nLen);
            TAF_MEM_CHK_RTN_VAL(memResult, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst)), (TAF_UINT32)nLen);
        }
        /* ������Ҫ�滻���ַ��� */
        if (rep_len) {
            memResult = memcpy_s(dst_pos + nLen, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst) - nLen), rep, rep_len);
            TAF_MEM_CHK_RTN_VAL(memResult, (TAF_UINT32)(AT_CMD_MAX_LEN - (dst_pos - dst) - nLen), rep_len);
        }

        src_pos = pCur + srch_len;
        dst_pos = dst_pos + nLen + rep_len;

    }while(pCur);

}

VOS_VOID AtNlSetAtClientConfig(TAF_UINT8 ucIndex, VOS_UINT8 ucStatus)
{
    TAF_AT_NvimAtClientCfg stAtClientCfg;
    AT_ClientCtx          *pstAtClientCtx = VOS_NULL_PTR;

    if ((ucStatus != 0) && (ucStatus != 4)) {
        return;
    }

    (VOS_VOID)memset_s(&stAtClientCfg, sizeof(TAF_AT_NvimAtClientCfg), 0, sizeof(TAF_AT_NvimAtClientCfg));

    if (VOS_OK != TAF_ACORE_NV_READ(MODEM_ID_0,
                                    NV_ITEM_AT_CLIENT_CFG,
                                    &stAtClientCfg,
                                    sizeof(TAF_AT_NvimAtClientCfg))) {
        return;
    }

    /* ������ֵ */
    stAtClientCfg.atClientConfig[ucIndex] = ucStatus;

    /* д��NV�� */
    if (NV_OK != TAF_ACORE_NV_WRITE(MODEM_ID_0,
                                    NV_ITEM_AT_CLIENT_CFG,
                                    (VOS_UINT8 *)&stAtClientCfg,
                                    sizeof(TAF_AT_NvimAtClientCfg))) {
        return;
    }

    /* ����ucReportFlg */
    pstAtClientCtx = AT_GetClientCtxAddr(ucIndex);

    pstAtClientCtx->clientConfiguration.reportFlg
        = (stAtClientCfg.atClientConfig[ucIndex] >> AT_NV_CLIENT_CONFIG_MODEM_ID_OFFSET) & 0x1;

    return;
}

VOS_VOID DealWithUserspaceProtocol(VOS_INT32 userprotocal)
{
#if (FEATURE_AT_HSUART == FEATURE_ON)
    if (userprotocal == AT_NL_PROTOCOL_OPEN_UART_UNSOLIDREPORT) {
        AtNlSetAtClientConfig(AT_CLIENT_ID_HSUART, 4);
    }

    if (userprotocal == AT_NL_PROTOCOL_CLOSE_UART_UNSOLIDREPORT) {
        AtNlSetAtClientConfig(AT_CLIENT_ID_HSUART, 0);
    }
#endif
}

VOS_VOID AtNlProxyCmdUnsolidreport(NLRESPONSE* presponse)
{
    VOS_UINT32 ulReturnCodeIndex = 0;
    VOS_UINT32 ulLen             = 0;

    /* ���������ϱ� */
    if (VOS_StrCmp((VOS_CHAR *)g_atCrLf, "\r\n") == 0) {
        if ((presponse->length == 0) && (presponse->retenum > AT_CME_ERROR_ENUM_BEGAIN) &&
            (presponse->retenum < AT_CME_ERROR_ENUM_END)) {
            if (AT_GetReturnCodeId(presponse->retenum, &ulReturnCodeIndex) == VOS_OK) {
                if (g_atCmeeType == AT_CMEE_ERROR_CODE) {
                    presponse->length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                                               (VOS_CHAR *)g_atSendDataBuffer,
                                                               (VOS_CHAR *)g_atSendDataBuffer,
                                                               "%s%s%s%s", g_atCrLf, g_atCmeErrorStr,
                                                               (TAF_CHAR *)g_atReturnCodeTab[ulReturnCodeIndex].result[0],
                                                               g_atCrLf);
                } else {
                    presponse->length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                                               (VOS_CHAR *)g_atSendDataBuffer,
                                                               (VOS_CHAR *)g_atSendDataBuffer,
                                                               "%s%s%s%s", g_atCrLf, g_atCmeErrorStr,
                                                               (TAF_CHAR *)g_atReturnCodeTab[ulReturnCodeIndex].result[1],
                                                               g_atCrLf);
                }
            }
        } else {
            /* ���������ϱ� */
            if (presponse->length <= AT_CMD_MAX_LEN) {
                (VOS_VOID)memcpy_s(g_atSendDataBuffer, (TAF_UINT32)presponse->length, presponse->buffer,
                    (TAF_UINT32)presponse->length);
            } else {
                ulLen = VOS_StrLen((VOS_CHAR *)presponse->buffer);
                (VOS_VOID)memcpy_s(g_atSendDataBuffer, AT_CMD_MAX_LEN, presponse->buffer, TAF_MIN(ulLen, AT_CMD_MAX_LEN));
                presponse->length = AT_CMD_MAX_LEN;
                AT_WARN_LOG("memcpy MAX length");
            }
        }
    }
#if (FEATURE_ON == FEATURE_IOT_HSUART_CMUX)
    if ((g_cmuxOpen == VOS_TRUE) && (presponse->uindex == AT_CLIENT_ID_UART)) {
        presponse->uindex = AT_CLIENT_ID_CMUXGPS;
    }
#endif
    AT_MntnTraceCmdResult((VOS_UINT8)presponse->uindex, g_atSendDataBuffer, (VOS_UINT16)presponse->length);
    AT_DisplaySelResultData((VOS_UINT16)presponse->length, (VOS_UINT8)presponse->uindex);
}

VOS_VOID AtNlProxyDealwithUspaceData(VOS_VOID* buffer, VOS_INT32 lLength)
{
    NLRESPONSE *presponse = NULL;
    TAF_UINT8   ucIndex;

    presponse = (NLRESPONSE *)buffer;
    ucIndex = (TAF_UINT8)presponse->uindex;

    /* ���<CR><LF>����\r\n���˴���Ҫ��\r\n��Ϊ<CR><LF> */
    if (VOS_StrCmp((VOS_CHAR *)g_atCrLf, "\r\n") != 0) {
        StrReplace((VOS_CHAR *)g_atSendDataBuffer, presponse->buffer, "\r\n", (VOS_CHAR *)g_atCrLf);
        (VOS_VOID)memcpy_s(presponse->buffer, (TAF_UINT32)presponse->length, g_atSendDataBuffer,
                (TAF_UINT32)presponse->length);
    }

    /* �����ϱ�Ĭ��Ϊ0 */
    if (presponse->magic < ATNL_MAGIC_LEN) {
        if (presponse->protocol == AT_NL_PROTOCOL_FIX_UNSOLIDREPORT) {
            AtNlProxyCmdUnsolidreport(presponse);
        } else {
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                                             (TAF_CHAR *)g_atSndCodeAddress,
                                                             (TAF_CHAR *)g_atSndCodeAddress,
                                                             "%s",
                                                             presponse->buffer);

            At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
        }
    }
    /* ������AT�� */
    else {
        /* �жϵ�ǰ�Ļظ��Ƿ����ڵȴ���AT�ظ� */
        if (presponse->magic != g_atClientTab[ucIndex].magic || g_atClientTab[AT_CLIENT_ID_APP].isWaitAts == 0) {
            AT_WARN_LOG1("overtime response  m_magic = <%d>", presponse->magic);
            return;
        }

        /* ����Э�鴦�� */
        if (presponse->protocol != 0) {
            DealWithUserspaceProtocol(presponse->protocol);
        }

        /* ʹ��error code */
        if (presponse->length == 0) {
            /* ��λAT״̬ */
            AT_STOP_TIMER_CMD_READY(ucIndex);
            /* ����AT_FormatResultData���������� */
            At_FormatResultData(ucIndex, presponse->retenum);
        } else {
            /* ��λAT״̬ */
            AT_STOP_TIMER_CMD_READY(ucIndex);
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                                             (TAF_CHAR *)g_atSndCodeAddress,
                                                             (TAF_CHAR *)g_atSndCodeAddress,
                                                             "%s",
                                                             presponse->buffer);

            At_SendResultData(ucIndex, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

            At_ResetCombinParseInfo(ucIndex);

            AT_CheckProcBlockCmd();
        }
    }

    return;
}

VOS_VOID AtnlproxyDispatchUspaceData(VOS_VOID* buffer, VOS_INT32 lLength)
{
    NLRESPONSE *presponse = NULL;
    TAF_UINT32  ulSendRet;
    TAF_UINT8   ucIndex;

    presponse = (NLRESPONSE *)buffer;

    ucIndex = (TAF_UINT8)presponse->uindex;

    ulSendRet = At_SendCmdMsg(ucIndex, (TAF_UINT8 *)buffer, (TAF_UINT16)lLength, AT_ATNLPROXY_MSG);
    if (ulSendRet != AT_SUCCESS) {
        AT_WARN_LOG("atnlproxy_dispatch_uspace_data  send to WUEPS_PID_AT error");
    }

    return;
}

VOS_INT32 AtnlproxyRegisterToKernel(AT_NL_DeviceId id, VOS_VOID* buff, VOS_INT32 lLength)
{
    /*
     * �ο�g_astAtDeviceCmdTbl��pucRegInfo�൱�ڴ˽ṹ��
     * �ο�AT_PAR_CMD_ELEMENT_STRU��pucSaveInfo�൱��pszCmdName��pszParamָ����ڴ�
     * ��ʵҲ���Զ�η��䣬������һ�η��䣬���ʹ�ã������ڴ���Ƭ�������ںܶ�ϵͳ��malloc�����ٲ���
     */
    VOS_UINT8 *pucRegInfo = VOS_NULL_PTR;
    VOS_UINT8 *pucSaveInfo = VOS_NULL_PTR;

    AT_NL_ParaCmdElementStru *pattemp = VOS_NULL_PTR;
    AT_ParCmdElement  element;
    TAF_UINT32        itemplength = 0;
    TAF_UINT32        iposition = 0;
    VOS_UINT16        iregcount = 0; /* pucRegInfo�ڴ����ж���ģ����������� */

    if (g_registStatus[id % NL_DEVICE_ID_MAX_ID] != 0) {
        AT_WARN_LOG1("atn1 rregist at cmd status:%d", g_registStatus[id % NL_DEVICE_ID_MAX_ID]);
        return VOS_ERROR;
    }

    if (lLength <= 0 || buff == VOS_NULL_PTR) {
        return VOS_ERROR;
    }

    pucSaveInfo = (VOS_UINT8 *)AT_MALLOC((VOS_UINT32)lLength);
    if (pucSaveInfo == VOS_NULL_PTR) {
        return VOS_ERROR;
    }
    pucRegInfo = (VOS_UINT8 *)AT_MALLOC((VOS_UINT32)lLength);
    if (pucRegInfo == VOS_NULL_PTR) {
        AT_FREE(pucSaveInfo);
        return VOS_ERROR;
    }
    /* �Ѵ�������ֵ�������� */
    (VOS_VOID)memcpy_s(pucSaveInfo, (TAF_UINT32)lLength, buff, (TAF_UINT32)lLength);
    (VOS_VOID)memset_s(pucRegInfo, (TAF_UINT32)lLength, 0, (TAF_UINT32)lLength);

    while ((TAF_UINT32)lLength > iposition + sizeof(AT_NL_FixedParaCmdElementStru)) {
        /* NL_FIXED_AT_PAR_CMD_ELEMENT_STRU */
        memset_s(&element, sizeof(AT_ParCmdElement), 0, sizeof(AT_ParCmdElement));
        pattemp = (AT_NL_ParaCmdElementStru *)(pucSaveInfo + iposition);
        element.setTimeOut   = (VOS_UINT32)pattemp->fixed.setTimeOut;
        element.abortTimeOut = (VOS_UINT32)pattemp->fixed.abortTimeOut;
        element.qryTimeOut   = (VOS_UINT32)pattemp->fixed.qryTimeOut;
        element.testTimeOut  = (VOS_UINT32)pattemp->fixed.testTimeOut;
        element.chkFlag      = (VOS_UINT32)pattemp->fixed.chkFlag;
        element.paramErrCode = pattemp->fixed.paramErrCode;
        element.cmdIndex = AT_CMD_ATNLPROXY_STUB;
        if (pattemp->fixed.pfnSetProc != NULL) {
            element.setProc = AtNlSetProc;
        }
        if (pattemp->fixed.pfnQryProc != NULL) {
            element.qryProc = AtNlQryProc;
        }
        if (pattemp->fixed.pfnTestProc != NULL) {
            element.testProc = AtNlTestProc;
        }
        if (pattemp->fixed.pfnAbortProc != NULL) {
            element.abortProc = AtNlAbortProc;
        }

        /* ������δ����ǰ���ƫ�Ƶķ�ʽ��ȡAT������е�pszCmdName��pszParam��iposition��ƫ���� */
        /* pszCmdName */
        iposition += sizeof(AT_NL_FixedParaCmdElementStru);

        /* ������ȷ���itemplength */
        (VOS_VOID)memcpy_s(&itemplength, sizeof(TAF_UINT32), pucSaveInfo + iposition, sizeof(TAF_UINT32));

        iposition += sizeof(TAF_UINT32);
        element.cmdName = pucSaveInfo + iposition;
        /* ulParaLength����������ǿ���Ϊ0�� */
        /* 1��'\0' */
        iposition = iposition + itemplength + 1;
        /* ȡ��������ĳ��� */
        (VOS_VOID)memcpy_s(&itemplength, sizeof(TAF_UINT32), pucSaveInfo + iposition, sizeof(TAF_UINT32));

        iposition = iposition + sizeof(TAF_UINT32);

        if (itemplength == 0) {
            element.param = NULL;
        } else {
            element.param = pucSaveInfo + iposition;
        }

        /* ipositionָ����һ��Ԫ�� */
        iposition = iposition + itemplength + 1;

        /* ��pattemp���浽pucRegInfo�� */
        (VOS_VOID)memcpy_s(pucRegInfo + iregcount * sizeof(AT_ParCmdElement), sizeof(AT_ParCmdElement), &element, sizeof(AT_ParCmdElement));
        iregcount++;
    }

    g_AppAtList = (AT_ParCmdElement *)pucRegInfo;
    g_AppAtCount = iregcount;

    AT_NORM_LOG("_________AT_RegisterCmdTable in atnlproxy_register_to_kernel_____________");
#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_OFF)
    AT_RegisterCmdTable(g_AppAtList, g_AppAtCount);
#endif

    g_registStatus[id % NL_DEVICE_ID_MAX_ID] = 1;
    return 0;
}

#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)

AT_ParCmdElement* AT_GetAppAtListTbl(VOS_VOID)
{
    return g_AppAtList;
}


VOS_UINT32 AT_GetAppAtCount(VOS_VOID)
{
    return g_AppAtCount;
}
#endif
#endif

