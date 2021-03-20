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

#include "AtCombinecmd.h"
#include "securec.h"
#include "AtCheckFunc.h"

#include "ATCmdProc.h"

#include "at_common.h"
#include "AtCmdCallProc.h"


#define THIS_FILE_ID PS_FILE_ID_AT_COMBINEDCMD_C

#define AT_ATD_CMD_MIN_LENGTH 3

#define AT_FW_LEN_AT_ADD_SEMI 3
#define AT_CMD_AT_INSIDE_SYMBOL_SEMI 2

VOS_UINT32 At_GetSecondAddr(VOS_UINT8 *data, VOS_UINT16 len, VOS_UINT8 **ppDataOut)
{
    errno_t    memResult;
    VOS_UINT8 *firAddr     = NULL;
    VOS_UINT8 *secAddr     = NULL;
    VOS_UINT32 basicCmdLen = 0;
    VOS_UINT8 *pcTmp       = NULL;

    /* �Ƚ��ַ�����Ҫ��֤�ַ���������\0 */
    pcTmp = (VOS_UINT8 *)AT_MALLOC(len + 1);
    if (pcTmp == NULL) {
        return ERR_MSP_FAILURE;
    }
    (VOS_VOID)memset_s(pcTmp, len + 1, 0x00, len + 1);

    if (len > 0) {
        memResult = memcpy_s(pcTmp, len + 1, data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, len + 1, len);
    }
    pcTmp[len] = '\0';

    firAddr = At_GetFirstBasicCmdAddr(pcTmp, &basicCmdLen);
    if (firAddr == NULL) {
        AT_FREE(pcTmp);
        return ERR_MSP_FAILURE;
    }

    secAddr = At_GetFirstBasicCmdAddr((firAddr + basicCmdLen), &basicCmdLen);

    if (secAddr == NULL) {
        *ppDataOut = data + len;
    } else {
        *ppDataOut = data + (secAddr - pcTmp);
    }

    AT_FREE(pcTmp);
    return ERR_MSP_SUCCESS;
}


VOS_UINT32 At_GetFirstCmdLen(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8 *begin  = data;
    VOS_UINT8 *end    = data;
    VOS_UINT16 lenFir = 0;

    if (At_GetSecondAddr(begin, len, &end) == ERR_MSP_SUCCESS) {
        lenFir = (VOS_UINT16)(end - begin);
    } else {
        /* ������ҵ�һ������ʧ�ܣ��򷵻������ַ����ĳ��� */
        lenFir = len;
    }

    return lenFir;
}


VOS_VOID At_ResetCombinCmdInfo(HI_LIST_S *combList)
{
    AT_FW_CombineCmdInfo *combineCmdInfo = VOS_NULL_PTR;
    AT_FW_CombineCmdNode *node           = VOS_NULL_PTR;
    HI_LIST_S            *tmp            = VOS_NULL_PTR;
    HI_LIST_S            *me             = VOS_NULL_PTR;

    if ((combList == VOS_NULL_PTR) || (combList->next == VOS_NULL_PTR)) {
        return;
    }

    /* �������в���ÿ���������� */
    msp_list_for_each_safe(me, tmp, combList)
    {
        if (me == NULL) {
            combineCmdInfo           = msp_list_entry(combList, AT_FW_CombineCmdInfo, combineCmdList);
            combineCmdInfo->totalNum = 0;
            combineCmdInfo->procNum  = 0;
            /*lint -e717*/
            HI_INIT_LIST_HEAD(combList);
            /*lint +e717*/
            return;
        }

        node = msp_list_entry(me, AT_FW_CombineCmdNode, combCmdList);
        /*lint -e774*/
        if (node == VOS_NULL_PTR) {
            combineCmdInfo           = msp_list_entry(combList, AT_FW_CombineCmdInfo, combineCmdList);
            combineCmdInfo->totalNum = 0;
            combineCmdInfo->procNum  = 0;
            /*lint -e717*/
            HI_INIT_LIST_HEAD(combList);
            /*lint +e717*/
            return;
        }
        /*lint +e774*/

        msp_list_del(&(node->combCmdList));
        AT_FREE((VOS_UINT8 *)node->combCmd);
        node->combCmd = VOS_NULL_PTR;
        AT_FREE((VOS_UINT8 *)node);
        node = VOS_NULL_PTR;
    }

    combineCmdInfo           = msp_list_entry(combList, AT_FW_CombineCmdInfo, combineCmdList);
    combineCmdInfo->totalNum = 0;
    combineCmdInfo->procNum  = 0;

    return;
}


VOS_VOID At_ResetCombinParseInfo(VOS_UINT8 indexNum)
{
    HI_LIST_S       *combList      = NULL;
    AT_ParseContext *clientContext = NULL;

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        return;
    }

    clientContext = &(g_parseContext[indexNum]);

    combList = &clientContext->combineCmdInfo.combineCmdList;

    if (combList->next == NULL) {
        /*lint -e717*/
        HI_INIT_LIST_HEAD(&(clientContext->combineCmdInfo.combineCmdList));
        /*lint -e717*/
    }

    At_ResetCombinCmdInfo(combList);

    /* ������buffer */
    g_atCombineSendData[indexNum].bufLen = 0;

    return;
}

/* ��ӡ�����������������Ϣ�������� */
VOS_VOID At_PrintCombinCmd(VOS_VOID)
{
    VOS_UINT32            i             = 0;
    VOS_UINT32            j             = 0;
    HI_LIST_S            *me            = NULL;
    HI_LIST_S            *combList      = NULL;
    AT_ParseContext      *clientContext = NULL;
    AT_FW_CombineCmd     *combCmd       = NULL;
    AT_FW_CombineCmdNode *combCmdNode   = NULL;

    for (j = 0; j < AT_MAX_CLIENT_NUM; j++) {
        clientContext = &(g_parseContext[j]);

        combList = &clientContext->combineCmdInfo.combineCmdList;

        /* ��client�����в���ָ����ClientId */
        msp_list_for_each(me, combList)
        {
            combCmdNode = msp_list_entry(me, AT_FW_CombineCmdNode, combCmdList);
            combCmd     = combCmdNode->combCmd;

            if (combCmd == NULL) {
                continue;
            }

            PS_PRINTF_INFO("-%d-:", combCmd->len);

            for (i = 0; i < combCmd->len; i++) {
                PS_PRINTF_INFO("%c", combCmd->data[i]);
            }
        }
    }

    return;
}

/*lint -save -e429*/

LOCAL VOS_UINT32 At_StoreSubCombCmd(HI_LIST_S *combList, VOS_UINT8 *dataIn, VOS_UINT16 lenIn)
{
    AT_FW_CombineCmd     *combCmd        = NULL;
    AT_FW_CombineCmdNode *combCmdNode    = NULL; /*lint !e830*/
    AT_FW_CombineCmdInfo *combineCmdInfo = NULL;
    errno_t               memResult;

    /* ���õĵط���ָ֤�벻Ϊ�� */

    combineCmdInfo = msp_list_entry(combList, AT_FW_CombineCmdInfo, combineCmdList);
    if (combineCmdInfo->totalNum >= AT_MAX_NUM_COMBINE_CMD) {
        PS_PRINTF_WARNING("usTotalNum %d\n", combineCmdInfo->totalNum);
        return ERR_MSP_FAILURE;
    }

    combCmdNode = (AT_FW_CombineCmdNode *)AT_MALLOC(sizeof(AT_FW_CombineCmdNode));
    if (combCmdNode == NULL) {
        return ERR_MSP_MALLOC_FAILUE;
    }
    (VOS_VOID)memset_s(combCmdNode, sizeof(AT_FW_CombineCmdNode), 0x00, sizeof(AT_FW_CombineCmdNode));

    combCmd = (AT_FW_CombineCmd *)AT_MALLOC(sizeof(AT_FW_CombineCmd) + lenIn + AT_FW_LEN_AT);
    if (combCmd == NULL) {
        AT_FREE((VOS_UINT8 *)combCmdNode);

        return ERR_MSP_MALLOC_FAILUE;
    }
    (VOS_VOID)memset_s(combCmd, (VOS_SIZE_T)sizeof(AT_FW_CombineCmd) + lenIn + AT_FW_LEN_AT, 0x00,
                       (VOS_SIZE_T)sizeof(AT_FW_CombineCmd) + lenIn + AT_FW_LEN_AT);

    combCmdNode->combCmd = combCmd;
    combCmd->len         = (VOS_UINT32)lenIn + AT_FW_LEN_AT;
    memResult            = memcpy_s(combCmd->data, lenIn + AT_FW_LEN_AT, "AT", AT_FW_LEN_AT);
    TAF_MEM_CHK_RTN_VAL(memResult, lenIn + AT_FW_LEN_AT, AT_FW_LEN_AT);
    if (lenIn > 0) {
        memResult = memcpy_s(&(combCmd->data[AT_FW_LEN_AT]), lenIn, dataIn, lenIn);
        TAF_MEM_CHK_RTN_VAL(memResult, lenIn, lenIn);
    }

    msp_list_add_tail(&combCmdNode->combCmdList, combList);

    combineCmdInfo->totalNum++;

    return ERR_MSP_SUCCESS;
}
/*lint -restore */

/* �������������� */
static VOS_UINT32 At_BasicCombineCmdParse(HI_LIST_S *combList, VOS_UINT8 *dataIn, VOS_UINT16 lenIn)
{
    VOS_UINT32 ret    = ERR_MSP_FAILURE;
    VOS_UINT16 lenFir = 0;
    VOS_UINT16 chkLen = 0;
    VOS_UINT8 *data   = dataIn;

    while (chkLen < lenIn) {
        lenFir = (VOS_UINT16)At_GetFirstCmdLen(data, (lenIn - chkLen));

        ret = At_StoreSubCombCmd(combList, data, lenFir);
        if (ret != ERR_MSP_SUCCESS) {
            At_ResetCombinCmdInfo(combList);
            return ret;
        }

        chkLen = chkLen + lenFir;
        data   = dataIn + chkLen;
    }

    return ERR_MSP_SUCCESS;
}

/* �����������չ������Ͻ��� */
VOS_UINT32 At_BasicExCombineCmdParse(HI_LIST_S *combList, VOS_UINT8 *dataIn, VOS_UINT16 lenIn, VOS_UINT16 firIndex)
{
    VOS_UINT32 ret  = ERR_MSP_FAILURE;
    VOS_UINT8 *data = dataIn;

    /* ���õĵط���֤pstCombList��pDataIn��Ϊ�� */

    if (firIndex >= lenIn) {
        return ERR_MSP_FAILURE;
    }

    /* ��0���ַ�����"+^$"����֮ǰ���ǻ������� */
    if (firIndex != 0) {
        ret = At_BasicCombineCmdParse(combList, data, firIndex);
        if (ret != ERR_MSP_SUCCESS) {
            At_ResetCombinCmdInfo(combList);
            return ret;
        }
    }

    ret = At_StoreSubCombCmd(combList, (data + firIndex), (lenIn - firIndex));
    if (ret != ERR_MSP_SUCCESS) {
        At_ResetCombinCmdInfo(combList);
    }

    return ret;
}


static VOS_VOID At_UpStringCmdName(VOS_UINT8 *data, VOS_UINT16 len)
{
    TAF_UINT8 *tmp    = data; /* current Char */
    TAF_UINT32 chkLen = 0;

    if (len == 0) {
        return;
    }

    while ((chkLen++ < len) && ((*tmp) != '=')) {
        if ((*tmp >= 'a') && (*tmp <= 'z')) {
            *tmp = *tmp - 0x20;
        }
        tmp++;
    }

    return;
}


VOS_UINT32 At_SemicolonCmdParse(HI_LIST_S *combList, VOS_UINT8 *dataIn, VOS_UINT16 lenIn)
{
    VOS_UINT32 ret       = ERR_MSP_FAILURE;
    VOS_UINT16 numQuota  = 0;
    VOS_UINT16 numSymbol = 0;
    VOS_UINT16 firIndex  = 0;
    VOS_UINT16 i         = 0;

    if ((combList == NULL) || (dataIn == NULL)) {
        return ERR_MSP_FAILURE;
    }

    if (At_CheckCharD(*dataIn) != AT_SUCCESS) {
        At_UpStringCmdName(dataIn, lenIn);
    } else {
        At_UpStringCmdName(dataIn, 1);
    }

    /* ';'����������Ľ�β������D���ֱ�Ӵ���Ϊ������������ */
    if (lenIn > (AT_ATD_CMD_MIN_LENGTH - 1)) {
        if (At_CheckCharD(*dataIn) == AT_SUCCESS) {
            if (At_CheckSemicolon(*(dataIn + lenIn - 1)) == AT_SUCCESS) {
                ret = At_StoreSubCombCmd(combList, dataIn, lenIn);
                if (ret != ERR_MSP_SUCCESS) {
                    At_ResetCombinCmdInfo(combList);
                }

                return ret;
            }
        }
    }

    /* ATD+117���ͣ�ֱ�Ӵ���Ϊ���ݺ������� */
    if (lenIn > (AT_ATD_CMD_MIN_LENGTH - 1)) {
        if ((At_CheckCharD(*dataIn) == AT_SUCCESS) && (dataIn[1] == '+')) {
            ret = At_StoreSubCombCmd(combList, dataIn, lenIn);
            if (ret != ERR_MSP_SUCCESS) {
                At_ResetCombinCmdInfo(combList);
            }

            return ret;
        }
    }

    /* ͳ�������������չ�����ʶ������ */
    while (i < lenIn) {
        if (dataIn[i] == '"') {
            numQuota++;
        /* ��2�����ʾ'"'����ӦΪż�� */
        } else if (((dataIn[i] == '+') || (dataIn[i] == '^') || (dataIn[i] == '$')) && ((numQuota % 2) == 0)) {
            numSymbol++;
            if (numSymbol == 1) {
                firIndex = i; /* ��¼��һ��"+^$"��λ�� */
            }
        } else {
        }

        i++;
    }

    /* ��չ�����ʶ������Ϊ0���������������Ϊ���ɻ���AT�������� */
    if (numSymbol == 0) {
        ret = At_BasicCombineCmdParse(combList, dataIn, lenIn);
    }
    /* ��չ�����ʶ������Ϊ1�������ǵ���һ����չ���Ҳ���������ɻ���������Ϻ�һ����չ����  */
    else if (numSymbol == 1) {
        ret = At_BasicExCombineCmdParse(combList, dataIn, lenIn, firIndex);
    }
    /* ��������ÿ�������У���չ�����ʶ��('+'��'^'��'$')���ֻ�ܳ���һ�� */
    else {
        ret = ERR_MSP_FAILURE;
    }

    return ret;
}


PRIVATE VOS_UINT32 At_CombineCmdPreProc(HI_LIST_S *combList, VOS_UINT8 *dataIn, VOS_UINT16 lenIn)
{
    VOS_UINT32 i        = 0;
    VOS_UINT32 ret      = ERR_MSP_FAILURE;
    VOS_UINT16 numQuota = 0;
    VOS_UINT8 *data     = dataIn;
    VOS_UINT8 *dataHead = dataIn;

    if ((combList == NULL) || (dataIn == NULL)) {
        return ERR_MSP_FAILURE;
    }

    while (i++ < lenIn) {
        /* �ֺ��������� */
        /* ��2�����ʾ'"'����ӦΪż�� */
        if ((*data == ';') && ((numQuota % 2) == 0)) {
            ret = At_SemicolonCmdParse(combList, dataHead, (VOS_UINT16)(data - dataHead + 1));
            if (ret != ERR_MSP_SUCCESS) {
                return ret;
            }

            dataHead = (data + 1);
        } else if (*data == '"') {
            numQuota++;
        } else {
        }

        data++;
    }

    /* ���һ���ַ����Ƿֺ�ʱҪ�����洢���һ��������� */
    if (dataHead != (dataIn + lenIn)) {
        ret = At_SemicolonCmdParse(combList, dataHead, (VOS_UINT16)(data - dataHead));
        if (ret != ERR_MSP_SUCCESS) {
            return ret;
        }
    }

    return ERR_MSP_SUCCESS;
}


VOS_UINT32 AT_IsDCmdValidChar(VOS_UINT8 para)
{
    /* ֧��dialing digits: 0-9,*,#,A-D */
    if ((para >= '0') && (para <= '9')) {
        return VOS_TRUE;
    }

    if ((para >= 'a') && (para <= 'c')) {
        return VOS_TRUE;
    }

    if ((para >= 'A') && (para <= 'C')) {
        return VOS_TRUE;
    }

    if ((para == '*') || (para == '#')) {
        return VOS_TRUE;
    }

    /* ֧��GSM/UMTS modifier characters: > i g I G */
    if (para == '>') {
        return VOS_TRUE;
    }

    if ((para == 'i') || (para == 'I')) {
        return VOS_TRUE;
    }

    if ((para == 'g') || (para == 'G')) {
        return VOS_TRUE;
    }

    if (para == '+') {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID AT_InsertDCmdGIPara(VOS_UINT32 *srcStrLen, VOS_UINT8 *srcStr, VOS_UINT32 maxMemLength, VOS_UINT32 insertStrLen,
                             VOS_UINT8 *insertStr)
{
    errno_t    memResult;
    VOS_UINT32 srcStrLenTemp;

    srcStrLenTemp = *srcStrLen;

    if (srcStr[srcStrLenTemp - 1] == ';') {
        if ((insertStrLen > 0) && (maxMemLength >= srcStrLenTemp)) {
            memResult = memcpy_s(&(srcStr[srcStrLenTemp - 1]), maxMemLength - (srcStrLenTemp - 1), insertStr,
                                 insertStrLen);
            TAF_MEM_CHK_RTN_VAL(memResult, maxMemLength - (srcStrLenTemp - 1), insertStrLen);
        }

        srcStrLenTemp += insertStrLen;

        srcStr[srcStrLenTemp - 1] = ';';
    } else {
        if ((insertStrLen > 0) && (maxMemLength > srcStrLenTemp)) {
            memResult = memcpy_s(&(srcStr[srcStrLenTemp]), maxMemLength - srcStrLenTemp, insertStr, insertStrLen);
            TAF_MEM_CHK_RTN_VAL(memResult, maxMemLength - srcStrLenTemp, insertStrLen);
        }

        srcStrLenTemp += insertStrLen;
    }

    *srcStrLen = srcStrLenTemp;

    return;
}


VOS_VOID AT_ProcDCmdGIPara(VOS_UINT32 *srcStrLen, VOS_UINT8 *srcStr)
{
    VOS_UINT32 loop;
    VOS_BOOL   bIExist;
    VOS_BOOL   biExist;
    VOS_BOOL   bGExist;
    VOS_BOOL   bgExist;
    VOS_UINT8  insertStr[AT_CALL_D_GI_PARA_LEN];
    VOS_UINT32 insertStrLen;
    VOS_UINT32 pos;
    VOS_UINT32 dstStrLen;
    VOS_UINT8 *dstPara = VOS_NULL_PTR;
    VOS_UINT32 maxMemLength;
    errno_t    returnValue;

    bIExist = VOS_FALSE;
    biExist = VOS_FALSE;
    bGExist = VOS_FALSE;
    bgExist = VOS_FALSE;

    dstStrLen    = *srcStrLen;
    maxMemLength = *srcStrLen;

    if (dstStrLen == 0) {
        AT_ERR_LOG("dstPara cannot malloc space !");
        return;
    }
    dstPara      = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, dstStrLen);
    if (dstPara == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_ProcDCmdGIPara : fail to alloc memory . ");
        return;
    }
    (VOS_VOID)memset_s(dstPara, dstStrLen, 0x00, dstStrLen);

    /*
     * ��ȡD�����GI�������ԣ�������ڳ�ͻ��I��i�ַ������һ���ַ�����Ϊ׼��
     * ɾ���ַ����е������ַ�GgIi
     */
    for (loop = 0, pos = 0; loop < dstStrLen; loop++) {
        if (srcStr[loop] == 'I') {
            bIExist = VOS_TRUE;
            biExist = VOS_FALSE;
            continue;
        }

        if (srcStr[loop] == 'i') {
            biExist = VOS_TRUE;
            bIExist = VOS_FALSE;
            continue;
        }

        if (srcStr[loop] == 'G') {
            bGExist = VOS_TRUE;
            continue;
        }

        if (srcStr[loop] == 'g') {
            bgExist = VOS_TRUE;
            continue;
        }

        *(dstPara + pos) = *(srcStr + loop);

        pos++;
    }

    dstStrLen = pos;

    /* �����޸ĵ��û������ַ��� */
    *srcStrLen = dstStrLen;
    if (dstStrLen > 0) {
        returnValue = memmove_s(srcStr, dstStrLen, dstPara, dstStrLen);
        TAF_MEM_CHK_RTN_VAL(returnValue, dstStrLen, dstStrLen);
    }

    PS_MEM_FREE(WUEPS_PID_AT, dstPara);

    /* ����GI�����ַ��� */
    (VOS_VOID)memset_s(insertStr, sizeof(insertStr), 0x00, sizeof(insertStr));

    insertStrLen = 0;
    if (bIExist == VOS_TRUE) {
        insertStr[insertStrLen] = 'I';
        insertStrLen++;
    } else {
        if (biExist == VOS_TRUE) {
            insertStr[insertStrLen] = 'i';
            insertStrLen++;
        }
    }

    if ((bGExist == VOS_TRUE) || (bgExist == VOS_TRUE)) {
        insertStr[insertStrLen] = 'G';
        insertStrLen++;
    }

    /* ��GI�����ַ������뵽���к���ͷֺ��ַ�֮�� */
    AT_InsertDCmdGIPara(srcStrLen, srcStr, maxMemLength, insertStrLen, insertStr);

    return;
}


VOS_VOID At_FilterDCmdSpecCharacter(VOS_UINT32 *paraLen, VOS_UINT8 *para)
{
    VOS_UINT32 i;
    VOS_UINT16 beginPos;
    VOS_UINT32 ret;
    errno_t    returnValue;

    if (*paraLen <= AT_ATD_CMD_MIN_LENGTH) {
        return;
    }

    /* �������ATD����������� */
    /* ��Ϊ'a'����'A'ʱ���� */
    if (At_CheckCharA(para[0]) != AT_SUCCESS) {
        return;
    }
    /* ��Ϊ't'����'T'ʱ���� */
    if (At_CheckCharT(para[1]) != AT_SUCCESS) {
        return;
    }
    /* ��Ϊ'D'����'d'ʱ���� */
    if (At_CheckCharD(para[AT_ATD_CMD_MIN_LENGTH - 1]) != AT_SUCCESS) {
        return;
    }

    beginPos = AT_ATD_CMD_MIN_LENGTH;
    /* ISDN���ŵ�ATDI<STRING>��ATD<STRING>��ͬ��ɾ��I�ַ����ִ�������һ���� */
    if (para[beginPos] == 'I') {
        *paraLen = *paraLen - 1;
        if (*paraLen > beginPos) {
            returnValue = memmove_s(&(para[beginPos]), *paraLen - beginPos, &(para[beginPos + 1]), *paraLen - beginPos);
            TAF_MEM_CHK_RTN_VAL(returnValue, *paraLen - beginPos, *paraLen - beginPos);
        }
    }

    /* ��һ���ַ�Ϊ'+'ʱ��Ϊ���ʺ����ʾ,���ܹ��� */
    if (para[beginPos] == '+') {
        beginPos++;
    }

    /* ��ΪATD��������һ���ַ�һ�����ַ���������'\0',�������������λ�����в�����ȡֵ�Ƿ�Խ�� */
    for (i = beginPos; i < *paraLen; i++) {
        ret = AT_IsDCmdValidChar(para[i]);
        if (ret == VOS_TRUE) {
            continue;
        }

        /* ���һ��������';'ʱ������ */
        if (i == (*paraLen - 1)) {
            if (para[i] == ';') {
                continue;
            }
        }

        *paraLen = *paraLen - 1;
        if (*paraLen > i) {
            returnValue = memmove_s(&(para[i]), *paraLen - i, &(para[i + 1]), *paraLen - i);
            TAF_MEM_CHK_RTN_VAL(returnValue, *paraLen - i, *paraLen - i);
        }
        i--;
    }

    AT_ProcDCmdGIPara(paraLen, para);

    return;
}


VOS_UINT32 At_CombineCmdProc(VOS_UINT8 clientId)
{
    VOS_UINT32       i        = 0;
    HI_LIST_S       *me       = NULL;
    HI_LIST_S       *listHead = NULL;
    VOS_UINT32       ret;
    AT_ParseContext *clientContext = NULL;

    AT_FW_CombineCmd     *combCmd        = NULL;
    AT_FW_CombineCmdNode *combCmdNode    = NULL;
    AT_FW_CombineCmdInfo *combineCmdInfo = NULL;

    AT_RreturnCodeUint32 result;

    /* �ú������ô��ɱ�֤ucClientId�ĺϷ��ԣ�pDataIn��Ϊ�� */

    clientContext = &(g_parseContext[clientId]);

    combineCmdInfo = &clientContext->combineCmdInfo;
    listHead       = &combineCmdInfo->combineCmdList;

    /* �жϺϷ��� */
    if ((combineCmdInfo->totalNum == 0) || (combineCmdInfo->totalNum <= combineCmdInfo->procNum)) {
        return AT_ERROR;
    }

    /* ��������������в��Ҵ���������� */
    msp_list_for_each(me, listHead)
    {
        if (i < combineCmdInfo->procNum) {
            i++;
            continue;
        }

        combCmdNode = msp_list_entry(me, AT_FW_CombineCmdNode, combCmdList);
        combCmd     = combCmdNode->combCmd;
        break;
    }

    /* û���ҵ������������ */
    if ((i != combineCmdInfo->procNum) || (combCmd == NULL)) {
        return AT_ERROR;
    }

    /*
     * ����Э�顶T-REC-V[1].250-200307-I_MSW-E.doc��
     * 6.3.1	Dial
     *     Any characters appearing in the dial string that the DCE does not recognize
     *     as a valid part of the call addressing information or as a valid modifier
     *     shall be ignored. This permits characters such as parentheses and hyphens
     *     to be included that are typically used in formatting of telephone numbers.
     *   ���԰������ַ����˵�
     */

    At_FilterDCmdSpecCharacter(&(combCmd->len), combCmd->data);

    /* ���������ַ��� */
    result = (AT_RreturnCodeUint32)AT_ParseCmdType(combCmd->data, (VOS_UINT16)combCmd->len);

    /* �����������ش����� */
    if (result != ERR_MSP_SUCCESS) {
        if (atMatchCmdName(clientId, g_atParseCmd.cmdFmtType) == ERR_MSP_SUCCESS) {
            if (g_parseContext[clientId].cmdElement != NULL) {
                return g_parseContext[clientId].cmdElement->paramErrCode;
            }
        }

        return result;
    }

    AT_MntnSaveCurCmdName(clientId);

    /* ����� */
    ret = CmdParseProc(clientId, combCmd->data, (VOS_UINT16)combCmd->len);
    if ((ret != AT_OK) && (ret != AT_WAIT_ASYNC_RETURN)) {
        At_ResetCombinCmdInfo(listHead);
    }

    return ret;
}


VOS_UINT32 At_CombineCmdChkProc(VOS_UINT8 clientId, VOS_UINT8 *dataIn, VOS_UINT16 lenIn)
{
    VOS_UINT16 len  = lenIn;
    VOS_UINT8 *data = dataIn;
    VOS_UINT32 ret;

    AT_ParseContext *clientContext = NULL;
    HI_LIST_S       *listHead      = NULL;

    /* �ú������ô��ɱ�֤ucClientId�ĺϷ��ԣ�pDataIn��Ϊ�� */

    clientContext = &(g_parseContext[clientId]);
    listHead      = &clientContext->combineCmdInfo.combineCmdList;

    /* ǰ�����ַ�����AT��ֱ�ӷ���ʧ�� */
    if (At_CheckCharA(data[0]) != AT_SUCCESS) {
        return AT_ERROR;
    }

    if (At_CheckCharT(data[1]) != AT_SUCCESS) {
        return AT_ERROR;
    }

    /* "AT" or "AT;" */
    if (lenIn == AT_FW_LEN_AT) {
        return AT_OK;
    }

    /* �������ַ��Ƿֺ� */
    if (lenIn == AT_FW_LEN_AT_ADD_SEMI) {
        if (At_CheckSemicolon(data[AT_CMD_AT_INSIDE_SYMBOL_SEMI]) == AT_SUCCESS) {
            return AT_OK;
        }
    }

    /* ����ǰ����"AT"�ַ�������ͳһ���� */
    data = data + AT_FW_LEN_AT;
    len  = len - AT_FW_LEN_AT;

    /* ��������Ԥ�����������ַ��뻺���� */
    ret = At_CombineCmdPreProc(listHead, data, len);
    if (ret != ERR_MSP_SUCCESS) {
        return AT_ERROR;
    }

    ret = At_CombineCmdProc(clientId);

    return ret;
}

