/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#include "taf_msg_chk_api.h"
#include "v_timer.h"
#include "omerrorlog.h"
#include "taf_type_def.h"
#include "dms_msg_chk.h"
#include "taf_msg_chk.h"
#include "securec.h"
#if ((VOS_OSA_CPU == OSA_CPU_ACPU) || (defined(__PC_UT__)))
#include "TafStdlib.h"
#else
#include <stdlib.h> /* bsearch,qsort */
#endif


#define THIS_FILE_ID PS_FILE_ID_TAF_MSG_CHK_API_C

STATIC VOS_UINT32 TAF_ChkChrOmInfoCltReportReqMsgLen(const MSG_Header *msgHeader);
#if (VOS_OS_VER == VOS_WIN32)
LOCAL VOS_UINT32 TAF_ChkMsgWhenActRecNullOnPcTest(VOS_UINT32 senderPid, VOS_UINT32 receiverPid);
#endif

/* ����MSP_PID_CHR��Ϣ���ȼ�� */
TAF_ChkMsgLenNameMapTbl g_tafChkChrMsgLenTbl[] = {
    { ID_OM_ERR_LOG_REPORT_REQ, sizeof(chr_log_report_req_s), VOS_NULL_PTR },
    { ID_OM_INFO_CLT_REPORT_REQ, 0, TAF_ChkChrOmInfoCltReportReqMsgLen }
};

MODULE_EXPORTED TAF_ChkMsgLenNameMapTbl* TAF_GetChkChrMsgLenTblAddr(VOS_VOID)
{
    return g_tafChkChrMsgLenTbl;
}

MODULE_EXPORTED VOS_UINT32 TAF_GetChkChrMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_tafChkChrMsgLenTbl);
}


MODULE_EXPORTED VOS_UINT32 TAF_ChkTimerMsgLen(const MsgBlock *msg)
{
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < sizeof(REL_TimerMsgBlock)) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


MODULE_EXPORTED VOS_UINT32 TAF_ChkMsgLenWithExpectedLen(const MSG_Header *msgHeader, VOS_UINT32 expectedLen)
{
    /* PC�����д��ϼ��, ��ʵ������ֻҪ��С��Ԥ��ֵ����Ϊ�Ƿ���Ҫ��� */
#if (VOS_OS_VER == VOS_WIN32)
    if (VOS_GET_MSG_LEN(msgHeader) != expectedLen) {
        return VOS_FALSE;
    }
#else
    if (VOS_GET_MSG_LEN(msgHeader) < expectedLen) {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}


MODULE_EXPORTED VOS_UINT32 TAF_RunChkMsgLenFunc(const MSG_Header *msgHeader, VOS_UINT32 structLen, TAF_ChkMsgLenFunc checkFun)
{
    /* �������������ζ�ż���ӵĲ���, PC���̴��ϴ�����Ϊ���ʧ��, ��ʵ�������ݴ�����Ϊ���ͨ�� */
    if ((checkFun == VOS_NULL_PTR) && (structLen == 0)) {
#if (VOS_OS_VER == VOS_WIN32)
        return VOS_FALSE;
#else
        return VOS_TRUE;
#endif
    }

    if (checkFun == VOS_NULL_PTR) {
        return TAF_ChkMsgLenWithExpectedLen(msgHeader, structLen - VOS_MSG_HEAD_LENGTH);
    }

    return checkFun(msgHeader);
}


STATIC VOS_INT32 TAF_CmpChkMsgLenMsgNameRecord(const VOS_VOID *msgNameRecord1, const VOS_VOID *msgNameRecord2)
{
    /*
     * ���ȼ�����Ϣ����ֵ��С����˳�����У�Ϊ֧������Ͷ��ַ����ң��ȽϹ�������:
     * ��¼1����Ϣ��С�ڼ�¼2����Ϣ����ֵ�����ظ���
     * ��¼1����Ϣ�����ڼ�¼2����Ϣ����ֵ������0
     * ��¼1����Ϣ�����ڼ�¼2����Ϣ����ֵ����������
     */
    if (((TAF_ChkMsgLenNameMapTbl *)msgNameRecord1)->msgName < ((TAF_ChkMsgLenNameMapTbl *)msgNameRecord2)->msgName) {
        return SMALLER;
    }

    if (((TAF_ChkMsgLenNameMapTbl *)msgNameRecord1)->msgName == ((TAF_ChkMsgLenNameMapTbl *)msgNameRecord2)->msgName) {
        return EQUAL;
    }

    return BIGGER;
}

#if ((VOS_OSA_CPU == OSA_CPU_ACPU) || (defined(__PC_UT__)))

VOS_VOID *BsearchFunc(const VOS_VOID *key, const VOS_VOID *base, VOS_UINT32 numOfElements, VOS_UINT32 sizeOfElements,
    TAF_STD_Cmpfun CmpFunc)
{
    VOS_INT32 low = 0;
    VOS_INT32 high;

    if ((base == VOS_NULL_PTR) || (numOfElements < 1) || (sizeOfElements < 1) || (CmpFunc == VOS_NULL_PTR)) {
        return VOS_NULL_PTR;
    }

    high = (VOS_INT32)numOfElements - 1;

    while (low <= high) {
        VOS_UINT8 *midAddr = VOS_NULL_PTR;
        VOS_INT32  mid = (low + ((high - low) / HALF_NUMBER));

        midAddr = ((VOS_UINT8 *)base + (mid * sizeOfElements));
        switch(CmpFunc(midAddr, key)) {
            case BIGGER:
                high = mid - 1;
                break;
            case SMALLER:
                low = mid + 1;
                break;
            case EQUAL:
                return midAddr;
            default:
                return VOS_NULL_PTR;
        }
    }

    return VOS_NULL_PTR;
}
#endif


STATIC TAF_ChkMsgLenNameMapTbl *TAF_GetChkMsgLenMsgNameRecord(const TAF_ChkMsgLenNameMapTbl *mapAddr, VOS_UINT32 mapSize,
    VOS_UINT32 msgName)
{
    TAF_ChkMsgLenNameMapTbl  keyRec;
    TAF_ChkMsgLenNameMapTbl *retRec = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&keyRec, sizeof(keyRec), 0x00, sizeof(keyRec));

    keyRec.msgName = msgName;

#if ((VOS_OSA_CPU == OSA_CPU_ACPU) || (defined(__PC_UT__)))
    retRec = (TAF_ChkMsgLenNameMapTbl *)BsearchFunc(&keyRec, mapAddr, mapSize, sizeof(keyRec),
        (TAF_COMP_FUNC)TAF_CmpChkMsgLenMsgNameRecord);
#else
    retRec = (TAF_ChkMsgLenNameMapTbl *)bsearch(&keyRec, mapAddr, mapSize, sizeof(keyRec),
        (TAF_COMP_FUNC)TAF_CmpChkMsgLenMsgNameRecord);
#endif

    return retRec;
}

STATIC VOS_UINT32 TAF_GetChkMsgLenMsgName(const MsgBlock *msg)
{
    VOS_UINT32 tlbNum;
    VOS_UINT32 i;

    const TAF_PidMap twoByteMsgNameTbl[] = {
        { DSP_PID_VOICE, I0_WUEPS_PID_VC },
        { DSP_PID_VOICE, I1_WUEPS_PID_VC },
        { DSP_PID_VOICE, I2_WUEPS_PID_VC },
        { ACPU_PID_VOICE_AGENT, I0_WUEPS_PID_VC },
        { ACPU_PID_VOICE_AGENT, I1_WUEPS_PID_VC },
        { ACPU_PID_VOICE_AGENT, I2_WUEPS_PID_VC },
        { I0_DSP_PID_IDLE, WUEPS_PID_AT },
        { I1_DSP_PID_IDLE, WUEPS_PID_AT },
        { I2_DSP_PID_IDLE, WUEPS_PID_AT },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        /* �ںϼܹ���NR��/C�˲�����, ����ֻ�ڷ��ںϼܹ���� */
        { NRAGENT_PID_AGENT, I0_UEPS_PID_MTA },
        { NRAGENT_PID_AGENT, I1_UEPS_PID_MTA },
        { NRAGENT_PID_AGENT, I2_UEPS_PID_MTA },
#endif
    };

    tlbNum = sizeof(twoByteMsgNameTbl) / sizeof(twoByteMsgNameTbl[0]);

    /* ����PID����Ϣ��msgName�õ���16λ�ģ�������Ϣ���ܴ����ұ��Ҳ��������������ת������ */
    for (i = 0; i < tlbNum; i++) {
        if ((VOS_GET_SENDER_ID(msg) == twoByteMsgNameTbl[i].senderPid) &&
            (VOS_GET_RECEIVER_ID(msg) == twoByteMsgNameTbl[i].receiverPid)) {
            return ((const TAF_MsgHeaderWithTwoByteMsgName*)msg)->msgName;
        }
    }

    return ((const MSG_Header *)msg)->msgName;
}


MODULE_EXPORTED VOS_UINT32 TAF_ChkMsgLen(const MsgBlock *msg, const TAF_ChkMsgLenRcvPidMapTbl *tabAddr, VOS_UINT32 tabSize)
{
    const MSG_Header          *msgHeader = VOS_NULL_PTR;
    TAF_ChkMsgLenSndPidMapTbl *sndPidMapAddr = VOS_NULL_PTR;
    TAF_ChkMsgLenNameMapTbl   *msgNameMapAddr = VOS_NULL_PTR;
    TAF_ChkMsgLenNameMapTbl   *actRec = VOS_NULL_PTR;
    VOS_UINT32                 mapTblSize = 0;
    VOS_UINT32                 msgName;
    VOS_UINT32                 i, j;

    /* �������е���ָ������PID�ĳ��ȼ�������Ҫ�����ȼ�����Ϣ����¼ */
    for (i = 0; i < tabSize; i++) {
        if (VOS_GET_RECEIVER_ID(msg) == tabAddr[i].receiverPid) {
            sndPidMapAddr = tabAddr[i].tabAddr;
            mapTblSize    = tabAddr[i].tabSize;
            break;
        }
    }

    if (sndPidMapAddr == VOS_NULL_PTR) {
        return VOS_TRUE;
    }

    /* ������Ϣ�ķ���PID�ҵ���Ϣ��������Ҫ�����ȼ�����Ϣ����¼ */
    for (j = 0; j < mapTblSize; j++) {
        if (VOS_GET_SENDER_ID(msg) == sndPidMapAddr[j].sndPid) {
            msgNameMapAddr = sndPidMapAddr[j].getSndPidTblAddrFunc();
            mapTblSize     = sndPidMapAddr[j].getSndPidTblAddrSize();
            break;
        }
    }

    if (msgNameMapAddr == VOS_NULL_PTR) {
        return VOS_TRUE;
    }

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < sizeof(MSG_Header)) {
        return VOS_FALSE;
    }

    msgName = TAF_GetChkMsgLenMsgName(msg);
    /* ��Ϣ����Ϣ���ȼ����ȼ�鴦����ӳ�����С�������У�ͨ�����ַ�������ϢID�����¼ */
    actRec = TAF_GetChkMsgLenMsgNameRecord(msgNameMapAddr, mapTblSize, msgName);
    if (actRec == VOS_NULL_PTR) {
#if (VOS_OS_VER == VOS_WIN32)
        /*
         * (1)PC�������ϸ���, ���ڼ�ʱ�������⡣
         * (2)g_tafChkTafMsgLenTbl��g_dsmChkTafMsgLenTblĿǰ������TAF->TAF��TAF->DSM��ʵ����������
         * ��Ӧ�ü��ACPU_PID_TAF����TAF��DSM�ģ�����ǰ����TAF_FidMsgProc���Ƚ�ACPU_PID_TAFת��ΪTAF��
         * �ٽ��е���Ϣ���ȼ�飬����PC������Ҫ������������ʵ��ſ��飬��������ˣ�����
         * ID_TAF_TAF_PID_INIT_IND��ID_APS_DSM_MODE_CHANGE_IND����û�ڼ������Ϣ
         */
        return TAF_ChkMsgWhenActRecNullOnPcTest(VOS_GET_SENDER_ID(msg), VOS_GET_RECEIVER_ID(msg));
#else
        return VOS_TRUE;
#endif
    }

    msgHeader = (const MSG_Header *)msg;
    return TAF_RunChkMsgLenFunc(msgHeader, actRec->fixdedPartLen, actRec->chkFunc);
}

#if ((VOS_OSA_CPU == OSA_CPU_ACPU) || (defined(__PC_UT__)))
MODULE_EXPORTED VOS_VOID TAF_SortChkMsgLenTblByMsgName(TAF_ChkMsgLenNameMapTbl *tblAddr, VOS_UINT32 tblSize)
{
    TAF_ChkMsgLenNameMapTbl temp;
    VOS_UINT32              i;
    VOS_UINT32              j;
    VOS_UINT32              pos;

    if (tblSize == 0) {
        return;
    }

    for (i = 0; i < (tblSize - 1); i++) {
        pos = i;

        for (j = pos + 1; j < tblSize; j++) {
            if (tblAddr[j].msgName < tblAddr[pos].msgName) {
                pos = j;
            }
        }

        if (i != pos) {
            temp = tblAddr[pos];
            tblAddr[pos] = tblAddr[i];
            tblAddr[i] = temp;
        }
    }

    return;
}
#else

MODULE_EXPORTED VOS_VOID TAF_SortChkMsgLenTblByMsgName(TAF_ChkMsgLenNameMapTbl *tabAddr, VOS_UINT32 tabSize)
{
    qsort(tabAddr, tabSize, sizeof(TAF_ChkMsgLenNameMapTbl), TAF_CmpChkMsgLenMsgNameRecord);
}
#endif


VOS_UINT32 TAF_ChkChrOmInfoCltReportReqMsgLen(const MSG_Header *msgHeader){
    chr_info_clt_report_req_s *report_req = VOS_NULL_PTR;
    VOS_UINT32                 minMsgLen;

    minMsgLen = sizeof(chr_info_clt_report_req_s) - VOS_MSG_HEAD_LENGTH - TAF_DEFAULT_CONTENT_LEN;
    if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
        return VOS_FALSE;
    }

    report_req = (chr_info_clt_report_req_s *)msgHeader;
    if (report_req->scene_type == OM_INFO_CLT_SCENE_TYPE_FREQ_BAND) {
        minMsgLen = minMsgLen + report_req->msg_len;
        if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}

#if ((FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF) || (VOS_OSA_CPU == OSA_CPU_CCPU))

VOS_UINT32 TAF_ChkGunasLogPrivacyMsgLen(const MsgBlock *msg)
{
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < sizeof(MSG_Header)) {
        return VOS_FALSE;
    }

    if (AT_ChkAt2AtMsgLen((const MSG_Header *)msg) != VOS_TRUE) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_ChkAtSndLogPrivacyMsgLen(const MsgBlock *msg)
{
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < sizeof(MSG_Header)) {
        return VOS_FALSE;
    }

    if (AT_ChkAt2AtMsgLen((const MSG_Header *)msg) != VOS_TRUE) {
        return VOS_FALSE;
    }

    return TAF_ChkTafFidRcvMsgLen(msg);
}


VOS_UINT32 TAF_ChkTafSndLogPrivacyMsgLen(const MsgBlock *msg)
{
    if (DMS_ChkDmsFidRcvMsgLen(msg) != VOS_TRUE) {
        return VOS_FALSE;
    }

    return TAF_ChkTafFidRcvMsgLen(msg);
}
#endif

#if (VOS_OS_VER == VOS_WIN32)

LOCAL VOS_UINT32 TAF_ChkMsgWhenActRecNullOnPcTest(VOS_UINT32 senderPid, VOS_UINT32 receiverPid)
{
    if (senderPid == I0_WUEPS_PID_TAF) {
        return VOS_TRUE;
    }

    if (senderPid == I1_WUEPS_PID_TAF) {
        return VOS_TRUE;
    }

    if (senderPid == I2_WUEPS_PID_TAF) {
        return VOS_TRUE;
    }


    return VOS_FALSE;
}
#endif



