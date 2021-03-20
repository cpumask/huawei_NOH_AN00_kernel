/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
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
#include "at_mbb_cmd.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "at_mbb_common.h"


#define THIS_FILE_ID PS_FILE_ID_AT_MBB_CMD_C

static const AT_ParCmdElement g_atMbbCmdTbl[] = {
    { AT_CMD_TNUMCTL,
    AT_SetTrustNumCtrlPara, AT_NOT_SET_TIME, AT_QryTrustNumCtrlPara, AT_NOT_SET_TIME, AT_TestTrustNumCtrlPara,
    AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^TNUMCTL", (VOS_UINT8 *)"(0-1),(0-1),(0)" },

    { AT_CMD_TRUSTCNUM,
    AT_SetTrustCallNumPara, AT_NOT_SET_TIME, AT_QryTrustCallNumPara, AT_NOT_SET_TIME, AT_TestCallTrustNumPara,
    AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^TRUSTCNUM", (VOS_UINT8 *)"(0-19),(number)" },

    { AT_CMD_TRUSTNUM,
    AT_SetSmsTrustNumPara, AT_NOT_SET_TIME, AT_QrySmsTrustNumPara, AT_NOT_SET_TIME, AT_TestSmsTrustNumPara,
    AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^TRUSTNUM", (VOS_UINT8 *)"(0-19),(number)" },

#if (FEATURE_MBB_CUST == FEATURE_ON)
    { AT_CMD_HSMF,
    AT_SetHsmfPara, AT_SET_PARA_TIME, AT_QryHsmfPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^HSMF", (VOS_UINT8 *)"(0,1)" },
#endif
#if (FEATURE_MBB_CUST == FEATURE_ON)
    { AT_CMD_LTEPROFILE,
    AT_SetCustProfilePara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, AT_TestCustProfilePara, AT_TEST_PARA_TIME,
    VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
    (VOS_UINT8 *)"^LTEPROFILE", (VOS_UINT8 *)"(0,1),(0-65535),(\"IP\",\"IPV6\",\"IPV4V6\"),(@ImsiPrefix),(APN),(UserName),(UserPwd),(0-3),(@profileName)" },

    { AT_CMD_LTEAPNATTACH,
    AT_SetCustProfileAttachSwitch, AT_SET_PARA_TIME, AT_QryCustProfileAttachSwitch, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR, AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
    (VOS_UINT8 *)"^LTEAPNATTACH", (VOS_UINT8 *)"(0,1)" },
#endif
};


VOS_UINT32 AT_RegisterMbbCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atMbbCmdTbl, sizeof(g_atMbbCmdTbl) / sizeof(g_atMbbCmdTbl[0]));
}

/* ��ʼ��ά�����Կ��ƿ��� */
VOS_VOID AT_InitMtTrustListCtrl(VOS_VOID)
{
    AT_MtTrustListCtrl        *mtTrustCtrl = VOS_NULL_PTR;
    TAF_NVIM_MtListAndEidCtrl  nvimMtCtrl;
    VOS_UINT32                 result;
    errno_t                    memResult;

    /* ��ȡά�����Կ��ƿ��������ĵ�ַ */
    mtTrustCtrl = AT_GetCommMtTrustCtrlAddr();
    memResult = memset_s(mtTrustCtrl, sizeof(AT_MtTrustListCtrl), 0x00, sizeof(AT_MtTrustListCtrl));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_MtTrustListCtrl), sizeof(AT_MtTrustListCtrl));

    /* ��ȡNV�л�ȡMT��������Trust�������ƿ��� */
    memResult = memset_s(&nvimMtCtrl, sizeof(TAF_NVIM_MtListAndEidCtrl), 0, sizeof(TAF_NVIM_MtListAndEidCtrl));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_NVIM_MtListAndEidCtrl), sizeof(TAF_NVIM_MtListAndEidCtrl));

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_LIST_AND_EID_CTRL, &nvimMtCtrl, sizeof(TAF_NVIM_MtListAndEidCtrl));

    if (result != NV_OK) {
        AT_ERR_LOG("AT_InitMtTrustListCtrl: Read NV fail.");
    } else {
        /* NV��ȡ�ɹ�������MT��������Trust�������ƿ��ص�AT SDC�� */
        mtTrustCtrl->mtSmsTrustListEnable  = nvimMtCtrl.mtSmsTrustListEnable;
        mtTrustCtrl->mtCallTrustListEnable = nvimMtCtrl.mtCallTrustListEnable;
    }
}

/* AT^TNUMCTL����������� */
VOS_UINT32 AT_SetTrustNumCtrlPara(VOS_UINT8 indexNum)
{
    AT_MtTrustListCtrl        *mtTrustCtrl = VOS_NULL_PTR;
    TAF_NVIM_MtListAndEidCtrl  nvimMtCtrl;
    VOS_UINT32                 result;
    errno_t                    memResult;

    /* �������� */
    if (g_atParaIndex > AT_SET_TRUST_NUM_CTRL_MAX_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����Ϊ�� */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0) ||
        (g_atParaList[2].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ȡȫ�ֱ�����Trust������������ */
    mtTrustCtrl = AT_GetCommMtTrustCtrlAddr();

    /* ��ȡNV�л�ȡMT��������Trust�������ƿ��� */
    memResult = memset_s(&nvimMtCtrl, sizeof(TAF_NVIM_MtListAndEidCtrl), 0, sizeof(TAF_NVIM_MtListAndEidCtrl));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_NVIM_MtListAndEidCtrl), sizeof(TAF_NVIM_MtListAndEidCtrl));

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_LIST_AND_EID_CTRL, &nvimMtCtrl,
        sizeof(TAF_NVIM_MtListAndEidCtrl));

    if (result != NV_OK) {
        AT_ERR_LOG("AT_SetSmsTrustNumPara: Read NV fail.");
        return AT_ERROR;
    }

    /* ����Trust�����������õ�NV */
    nvimMtCtrl.mtSmsTrustListEnable  = (VOS_UINT8)g_atParaList[0].paraValue;
    nvimMtCtrl.mtCallTrustListEnable = (VOS_UINT8)g_atParaList[1].paraValue;

    result = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MT_LIST_AND_EID_CTRL, (VOS_UINT8 *)(&nvimMtCtrl),
        sizeof(TAF_NVIM_MtListAndEidCtrl));

    if (result != NV_OK) {
        /* дNVʧ�ܺ󣬲�����ȫ�ֱ���ֵ */
        AT_ERR_LOG("AT_SetTrustNumCtrlPara: Write NV fail.");
        return AT_ERROR;
    } else {
        /* дNV�ɹ��󣬸���ȫ�ֱ���ֵ */
        mtTrustCtrl->mtSmsTrustListEnable  = nvimMtCtrl.mtSmsTrustListEnable;
        mtTrustCtrl->mtCallTrustListEnable = nvimMtCtrl.mtCallTrustListEnable;
    }

    return AT_OK;
}

/* AT^TNUMCTL��ѯ������� */
VOS_UINT32 AT_QryTrustNumCtrlPara(VOS_UINT8 indexNum)
{
    AT_MtTrustListCtrl *mtCtrl = VOS_NULL_PTR;
    VOS_UINT8           trustBlockNumEnable = 0; /* Block�������أ���ǰ��֧�֣�����ǰĬ����0 */

    /* ��ȡά�����Կ��ƿ��������ĵ�ַ */
    mtCtrl = AT_GetCommMtTrustCtrlAddr();

    /* ��ѯ���ؽ���ֱ�Ϊ������Trust�������ء�����Trust�������ء�Block�������أ���ǰ��֧�֣� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        mtCtrl->mtSmsTrustListEnable, mtCtrl->mtCallTrustListEnable, trustBlockNumEnable);

    return AT_OK;
}

/* AT^TNUMCTL����������� */
VOS_UINT32 AT_TestTrustNumCtrlPara(VOS_UINT8 indexNum)
{
    /* Block�������ܣ���ǰ��֧�֣� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-1),(0-1),(0)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}

/* ����\����Trust��������Ϸ���У�� */
VOS_UINT32 AT_CheckTrustNumPara(const VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 count = 0;
    const VOS_UINT8 *temp = VOS_NULL_PTR;

    temp = data;
    /* ��һλ��'+'ʱ������ĺ�����󳤶�+1 */
    if (*temp == '+') {
        if (len > (AT_TRUSTLIST_NUM_PARA_MAX_LEN + 1)) {
            return AT_CME_DIAL_STRING_TOO_LONG;
        }
        temp++;
        count++;
    } else {
        if (len > AT_TRUSTLIST_NUM_PARA_MAX_LEN) {
            return AT_CME_DIAL_STRING_TOO_LONG;
        }
    }

    /* ����Ƿ�����Ϸ��ַ� */
    for (; count < len; count++) {
        if (((*temp >= '0') && (*temp <= '9')) || (*temp == '*') || (*temp == '#')) {
            temp++;
        } else {
            return AT_CME_INVALID_CHARACTERS_IN_DIAL_STRING;
        }
    }
    return AT_SUCCESS;
}

/* ��ȡ��Ч���� */
const VOS_UINT8* AT_GetValidNum(const VOS_UINT8 *trustNum, VOS_UINT32 trustNumLen, VOS_UINT8 *trustNumPrefix)
{
    if (trustNum[0] == '+') {
        *trustNumPrefix = VOS_TRUE;
        return (trustNum + 1);
    }
    if (trustNumLen >= AT_TRUSTLIST_NUM_PREFIX_MAX_LEN) {
        if ((trustNum[0] == '0') && (trustNum[1] == '0')) {
            *trustNumPrefix = VOS_TRUE;
            return (trustNum + AT_TRUSTLIST_NUM_PREFIX_MAX_LEN);
        }
    }
    return trustNum;
}

/* ����ƥ�� */
VOS_UINT32 AT_TrustNumCmp(const VOS_UINT8 *checkNum, VOS_UINT32 checkNumLen,
    const VOS_UINT8 *nvTrustNum, VOS_UINT32 nvTrustNumLen)
{
    const VOS_UINT8 *checkNumTmp      = VOS_NULL_PTR;
    const VOS_UINT8 *nvTrustNumTmp    = VOS_NULL_PTR;
    VOS_UINT8        checkNumPrefix   = VOS_FALSE;
    VOS_UINT8        nvTrustNumPrefix = VOS_FALSE;

    if ((nvTrustNumLen < AT_TRUSTLIST_NUM_MIN_LEN) || (nvTrustNumLen > (AT_TRUSTLIST_NUM_PARA_MAX_LEN + 1)) ||
        (checkNumLen < AT_TRUSTLIST_NUM_MIN_LEN) || (checkNumLen > (AT_TRUSTLIST_NUM_PARA_MAX_LEN + 1))) {
        AT_ERR_LOG("AT_TrustNumCmp: Length is invalid.");
        return VOS_FALSE;
    }
    nvTrustNumTmp = AT_GetValidNum(nvTrustNum, nvTrustNumLen, &nvTrustNumPrefix);
    checkNumTmp = AT_GetValidNum(checkNum, checkNumLen, &checkNumPrefix);
    if ((nvTrustNumTmp == VOS_NULL_PTR) || (checkNumTmp == VOS_NULL_PTR)) {
        AT_ERR_LOG("AT_TrustNumCmp: Get address is invalid.");
        return VOS_FALSE;
    }
    if ((VOS_StrCmp((VOS_CHAR *)nvTrustNumTmp, (VOS_CHAR *)checkNumTmp) == 0) && (checkNumPrefix == nvTrustNumPrefix)) {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}

/* �������ĺ����Ƿ��Ѵ��� */
VOS_UINT32 AT_CheckDeduplicationTrustNum(const TAF_NVIM_NumType *nvTrustList, VOS_UINT32 trustMaxNum)
{
    VOS_UINT32 num;
    VOS_UINT32 result;
    VOS_UINT32 nvTrustNumLen;

    for (num = 0; num < trustMaxNum; num++) {
        nvTrustNumLen = VOS_StrLen((VOS_CHAR *)nvTrustList[num].number);
        if (nvTrustNumLen != 0) {
            result = AT_TrustNumCmp(g_atParaList[1].para, g_atParaList[1].paraLen,
                                    nvTrustList[num].number, nvTrustNumLen);
            if (result == VOS_TRUE) {
                AT_ERR_LOG("AT_CheckDeduplicationTrustNum: Number already exists.");
                return VOS_TRUE;
            }
        }
    }
    return VOS_FALSE;
}

/* ����ָ����������\����Trust�������� */
VOS_UINT32 AT_TrustNumParaProc(TAF_NVIM_NumType *recordNumType)
{
    errno_t memResult;
    VOS_UINT32 result;

    /* �������Ϊ�գ���ʾɾ��ָ������Trust�������� */
    if (g_atParaList[1].paraLen == 0) {
        recordNumType->index = 0;
        memResult = memset_s(recordNumType->number, sizeof(recordNumType->number), 0, sizeof(recordNumType->number));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(recordNumType->number), sizeof(recordNumType->number));
    } else {
        /* ����/����Trust��������Ϸ��Լ�� */
        result = AT_CheckTrustNumPara(g_atParaList[1].para, g_atParaList[1].paraLen);
        if (result != AT_SUCCESS) {
            return result;
        }
    }
    return AT_SUCCESS;
}

/* AT^TRUSTNUM����������� */
VOS_UINT32 AT_SetSmsTrustNumPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_MtSmsNumTrustList smsTrustList;
    VOS_UINT32                 listIndex;
    VOS_UINT32                 result;
    errno_t                    memResult;

    /* �����Ϸ��Լ�� */
    if ((g_atParaIndex > AT_SET_SMS_TRUST_NUM_MAX_PARA_NUM) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��NV�ж�ȡ����Trust�����б� */
    (VOS_VOID)memset_s(&smsTrustList, sizeof(smsTrustList), 0, sizeof(smsTrustList));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_SMS_NUM_TRUST_LIST, &smsTrustList, sizeof(smsTrustList)) != NV_OK) {
        AT_ERR_LOG("AT_SetSmsTrustNumPara: Read NV fail.");
        return AT_ERROR;
    }

    listIndex = g_atParaList[0].paraValue;
    /* ��������Ϊ1��ʱ����ʾ��ȡ��Ӧ�����Ķ���Trust�������� */
    if (g_atParaIndex == 1) {
        if (VOS_StrLen((VOS_CHAR *)smsTrustList.recordList[listIndex].number) != 0) {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,\"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
                listIndex, smsTrustList.recordList[listIndex].number);
            return AT_OK;
        } else {
            AT_INFO_LOG("The sms trustlist record of the specified index is empty.");
            return AT_OK;
        }
    } else {
        /* ��������Ϊ2��ʱ,Trust��������Ϸ��Լ�� */
        result = AT_TrustNumParaProc(&smsTrustList.recordList[listIndex]);
        if (result != AT_SUCCESS) {
            return result;
        }
        /* Trust���������ظ��Լ�� */
        if (AT_CheckDeduplicationTrustNum(smsTrustList.recordList, TAF_TRUSTLIST_MAX_LIST_SIZE) == VOS_TRUE) {
            return AT_OK;
        }
        /* ����ָ��������Trust�������� */
        smsTrustList.recordList[listIndex].index = (VOS_UINT8)g_atParaList[0].paraValue;
        (VOS_VOID)memset_s(smsTrustList.recordList[listIndex].number, sizeof(smsTrustList.recordList[listIndex].number),
                           0x00, sizeof(smsTrustList.recordList[listIndex].number));

        memResult = memcpy_s(smsTrustList.recordList[listIndex].number,
                             sizeof(smsTrustList.recordList[listIndex].number),
                             g_atParaList[1].para,
                             g_atParaList[1].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsTrustList.recordList[listIndex].number), g_atParaList[1].paraLen);
    }
    /* ���¶���Trust����NV */
    result = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MT_SMS_NUM_TRUST_LIST, (VOS_UINT8 *)(&smsTrustList), sizeof(smsTrustList));
    if (result != NV_OK) {
        AT_ERR_LOG("AT_SetSmsTrustNumPara: Write NV fail.");
        return AT_ERROR;
    }
    return AT_OK;
}

/* AT^TRUSTNUM��ѯ������� */
VOS_UINT32 AT_QrySmsTrustNumPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_MtSmsNumTrustList smsTrustList;
    VOS_UINT32                 result;
    VOS_UINT32                 listIndex;
    errno_t                    memResult;
    VOS_UINT16                 length = 0;
    VOS_UINT8                  count  = 0;

    /* ��NV�ж�ȡ����Trust�����б� */
    memResult = memset_s(&smsTrustList, sizeof(smsTrustList), 0, sizeof(smsTrustList));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsTrustList), sizeof(smsTrustList));

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_SMS_NUM_TRUST_LIST, &smsTrustList, sizeof(smsTrustList));
    if (result != NV_OK) {
        AT_ERR_LOG("AT_QrySmsTrustNumPara: Read NV fail.");
        return AT_ERROR;
    }

    /* �����ЧTrust�������� */
    for (listIndex = 0; listIndex < TAF_TRUSTLIST_MAX_LIST_SIZE; listIndex++) {
        if (VOS_StrLen((VOS_CHAR *)smsTrustList.recordList[listIndex].number) != 0) {
            if (count != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,\"%s\"",
                g_parseContext[indexNum].cmdElement->cmdName,
                smsTrustList.recordList[listIndex].index, smsTrustList.recordList[listIndex].number);
            count++;
        }
    }

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}

/* AT^TRUSTNUM����������� */
VOS_UINT32 AT_TestSmsTrustNumPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-19)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}

/* AT^TRUSTCNUM����������� */
VOS_UINT32 AT_SetTrustCallNumPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_MtCallNumTrustList callTrustList;
    VOS_UINT32                  listIndex;
    VOS_UINT32                  result;
    errno_t                     memResult;

    /* �����Ϸ��Լ�� */
    if ((g_atParaIndex > 2) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��NV�ж�ȡ����Trust�����б� */
    memResult = memset_s(&callTrustList, sizeof(callTrustList), 0, sizeof(callTrustList));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callTrustList), sizeof(callTrustList));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_CALL_NUM_TRUST_LIST, &callTrustList, sizeof(callTrustList)) != NV_OK) {
        AT_ERR_LOG("AT_SetTrustCallNumPara: Read NV Fail.");
        return AT_ERROR;
    }

    listIndex = g_atParaList[0].paraValue;
    /* ��������Ϊ1��ʱ����ʾ��ȡ��Ӧ����������Trust�������� */
    if (g_atParaIndex == 1) {
        if (VOS_StrLen((VOS_CHAR *)callTrustList.recordList[listIndex].number) != 0) {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,\"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
                listIndex, callTrustList.recordList[listIndex].number);
            return AT_OK;
        } else {
            AT_INFO_LOG("The call trust List record of the specified index is empty.");
            return AT_OK;
        }
    } else {
        /* ��������Ϊ2��ʱ,Trust��������Ϸ��Լ�� */
        result = AT_TrustNumParaProc(&callTrustList.recordList[listIndex]);
        if (result != AT_SUCCESS) {
            return result;
        }
        /* Trust���������ظ��Լ�� */
        if (AT_CheckDeduplicationTrustNum(callTrustList.recordList, TAF_TRUSTLIST_MAX_LIST_SIZE) == VOS_TRUE) {
            return AT_OK;
        }
        /* ����ָ��������Trust�������� */
        callTrustList.recordList[listIndex].index = (VOS_UINT8)g_atParaList[0].paraValue;
        memResult = memset_s(callTrustList.recordList[listIndex].number,
                             sizeof(callTrustList.recordList[listIndex].number),
                             0,
                             sizeof(callTrustList.recordList[listIndex].number));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callTrustList.recordList[listIndex].number),
                            sizeof(callTrustList.recordList[listIndex].number));
        memResult = memcpy_s(callTrustList.recordList[listIndex].number,
                             sizeof(callTrustList.recordList[listIndex].number),
                             g_atParaList[1].para,
                             g_atParaList[1].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callTrustList.recordList[listIndex].number), g_atParaList[1].paraLen);
    }

    /* ��������Trust����NV */
    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MT_CALL_NUM_TRUST_LIST, (VOS_UINT8 *)(&callTrustList), sizeof(callTrustList)) != NV_OK) {
        AT_ERR_LOG("AT_SetTrustCallNumPara: Write NV Fail.");
        return AT_ERROR;
    }
    return AT_OK;
}

/* AT^TRUSTCNUM��ѯ������� */
VOS_UINT32 AT_QryTrustCallNumPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_MtCallNumTrustList callTrustList;
    VOS_UINT32                  result;
    errno_t                     memResult;
    VOS_UINT16                  length = 0;
    VOS_UINT8                   count  = 0;
    VOS_UINT8                   listIndex;

    /* ��NV�ж�ȡ����Trust�����б� */
    memResult = memset_s(&callTrustList, sizeof(callTrustList), 0, sizeof(callTrustList));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callTrustList), sizeof(callTrustList));

    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MT_CALL_NUM_TRUST_LIST, &callTrustList, sizeof(callTrustList));
    if (result != NV_OK) {
        AT_ERR_LOG("AT_QryTrustCallNumPara: Read NV Fail.");
        return AT_ERROR;
    }

    /* �����ЧTrust�������� */
    for (listIndex = 0; listIndex < TAF_TRUSTLIST_MAX_LIST_SIZE; listIndex++) {
        if (VOS_StrLen((VOS_CHAR *)callTrustList.recordList[listIndex].number) != 0) {
            if (count != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,\"%s\"",
                g_parseContext[indexNum].cmdElement->cmdName,
                callTrustList.recordList[listIndex].index, callTrustList.recordList[listIndex].number);
            count++;
        }
    }
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

/* AT^TRUSTCNUM����������� */
VOS_UINT32 AT_TestCallTrustNumPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-19)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
/*
 * ����˵��: WebSDK֪ͨModem��Ӧ�ñ��ض������ݿ��޶��ſռ�����ж��ſռ�
 * �������: index: AT�·�ͨ��
 * ���ؽ��: AT_WAIT_ASYNC_RETURN: �첽�ȴ�״̬
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_SetHsmfPara(VOS_UINT8 indexNum)
{
    MN_MSG_SetMemstatusParm memStatus;

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&memStatus, sizeof(memStatus), 0x00, sizeof(memStatus));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����Ϊ�� */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���ò��� */
    if (g_atParaList[0].paraValue == 0) {
        /* ����HSMF��0����δ�� */
        memStatus.memFlag = MN_MSG_MEM_FULL_UNSET;
    } else {
        memStatus.memFlag = MN_MSG_MEM_FULL_SET;
    }

    /* ����MN��ϢAPI�������������MSGģ�� */
    if (MN_MSG_SetMemStatus(g_atClientTab[indexNum].clientId,
                            g_atClientTab[indexNum].opId,
                            &memStatus) != MN_ERR_NO_ERROR) {
        return AT_ERROR;
    }

    /* ���õ�ǰ�������� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HSMF_SET;

    return AT_WAIT_ASYNC_RETURN;
}

/*
 * ����˵��: WebSDKͨ��modem��ѯӦ�ñ��ض������ݿ��Ƿ��пռ�洢����
 * �������: index: AT�·�ͨ��
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_QryHsmfPara(VOS_UINT8 indexNum)
{
    TAF_NVIM_NvAppSmsFullFlag appMemStatus;

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&appMemStatus, sizeof(appMemStatus), 0x00, sizeof(appMemStatus));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_APP_SMS_FULL_FLG, &appMemStatus, sizeof(appMemStatus)) != NV_OK) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        appMemStatus.smsFullFlg);

    return AT_OK;
}
#endif


