/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
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

#include "ps_logfilter_comm.h"
#include "securec.h"
#include "ps_lib.h"
#include "gucttf_tag.h"
#include "ttf_comm.h"
#include "ps_log_filter_interface.h"


#define THIS_FILE_ID PS_FILE_ID_LOG_FILTER_COMM_C
#define THIS_MODU mod_logfilter

typedef struct {
    VOS_UINT32 regSuccCnt;
    VOS_UINT32 regFailCnt;
    VOS_UINT32 localCpuFilterCnt;
    VOS_UINT32 diffCpuFilterCnt;
} PsOmLayerMsgReplaceStatisticStru;

STATIC PsOmLayerMsgReplaceStatisticStru g_layerMsgFilterStatistic = { 0, 0, 0, 0 };

/* ���ں˼�ͨ����Ϣƥ����˽ӿ� */
STATIC MsgBlock* PS_OM_ReplaceMsg(PsOmPerPidRegCtrl *cpuPerPidCtrl, MsgBlock *msg)
{
    VOS_UINT32            loop;
    MsgBlock             *result    = msg;
    PsOmLayerMsgReplaceCb filterFun = VOS_NULL_PTR;

    if (cpuPerPidCtrl == VOS_NULL_PTR) {
        return result;
    }

    /* ����ѭ����senderPid��ע��Ĺ��˺��� */
    for (loop = 0; loop < cpuPerPidCtrl->useCnt; loop++) {
        filterFun = cpuPerPidCtrl->filterFunc[loop];
        if (filterFun != VOS_NULL_PTR) {
            result = filterFun(msg);
            if (msg != result) {
                break;
            }
        }
    }

    return result;
}

STATIC PsOmPerPidRegCtrl* PS_OM_GetLocalLayerMsgRegCtrl(VOS_UINT32 senderPid, PsOmLayerMsgReplaceCtrl *ctrlInfo)
{
    VOS_UINT16 filterIdx = (VOS_UINT16)senderPid;

    if ((ctrlInfo->localCpuFilter.pidMsgFilter != VOS_NULL_PTR) && (filterIdx < ctrlInfo->localCpuFilter.maxCnt)) {
        return &(ctrlInfo->localCpuFilter.pidMsgFilter[filterIdx]);
    }

    return VOS_NULL_PTR;
}

STATIC PsOmPerPidRegCtrl* PS_OM_GetDiffLayerMsgRegCtrl(VOS_UINT32 senderPid, PsOmLayerMsgReplaceCtrl *ctrlInfo)
{
    VOS_UINT32                 loop;
    const PsOmCpuLayerMsgFilterCtrl *diffCpuLayerMsgFilter = &(ctrlInfo->diffCpuFilter);

    /* ��ƥ��senderPid, �ҵ�senderpid��ѭ������ע��Ĺ��˺��� */
    for (loop = 0; loop < diffCpuLayerMsgFilter->useCnt; loop++) {
        if (diffCpuLayerMsgFilter->pidMsgFilter[loop].senderPid == senderPid) {
            return &(diffCpuLayerMsgFilter->pidMsgFilter[loop]);
        }
    }

    return VOS_NULL_PTR;
}

/* ��Ϣ�滻�ӿڣ�������ͬ����Ϣ���ǿ����Ϣ */
MsgBlock* PS_OM_CpuLayerMsgCommReplace(MsgBlock *msg, PsOmLayerMsgReplaceCtrl *ctrlInfo)
{
    MsgBlock          *result     = msg;
    PsOmPerPidRegCtrl *pidCpuCtrl = VOS_NULL_PTR;
    VOS_UINT32        *succCnt    = VOS_NULL_PTR;
    VOS_PID            senderId;

    if ((msg == VOS_NULL_PTR) || (ctrlInfo == VOS_NULL_PTR)) {
        return msg;
    }

    senderId = TTF_GET_MSG_SENDER_ID(msg);
    if (ctrlInfo->localCpuId == VOS_GET_CPU_ID(senderId)) {
        /* �ǿ�˲����Ϣ���� */
        pidCpuCtrl = PS_OM_GetLocalLayerMsgRegCtrl(senderId, ctrlInfo);
        succCnt    = &(g_layerMsgFilterStatistic.localCpuFilterCnt);
    } else {
        /* ��˲����Ϣ���� */
        pidCpuCtrl = PS_OM_GetDiffLayerMsgRegCtrl(senderId, ctrlInfo);
        succCnt    = &(g_layerMsgFilterStatistic.diffCpuFilterCnt);
    }

    result = PS_OM_ReplaceMsg(pidCpuCtrl, msg);
    if (result != msg) {
        (*succCnt) += 1;
    }

    return result;
}

STATIC VOS_UINT32 PS_OM_LayerMsgReplaceMemExpand(
    VOS_UINT32 senderPid, VOS_UINT16 elementSize, VOS_UINT16 expandCnt, VOS_UINT16 *curCnt, VOS_VOID** startAddr)
{
    const VOS_UINT32 oldCnt    = *curCnt;
    const VOS_UINT32 oldMemLen = elementSize * oldCnt;
    const VOS_UINT32 newCnt    = expandCnt + oldCnt;
    const VOS_UINT32 newMemLen = elementSize * newCnt;
    VOS_VOID        *oldAddr   = *startAddr;
    VOS_VOID        *newAddr   = VOS_MemAlloc(senderPid, DYNAMIC_MEM_PT, newMemLen);

    if (newAddr == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(newAddr, newMemLen, 0x0, newMemLen);

    if (oldAddr != VOS_NULL_PTR) {
        (VOS_VOID)memcpy_s(newAddr, newMemLen, oldAddr, oldMemLen);
        VOS_MemFree(senderPid, oldAddr);
    }

    *startAddr = newAddr;
    *curCnt    = (VOS_UINT16)newCnt;

    return VOS_OK;
}

STATIC VOS_UINT32 PS_OM_OnePidFilterFuncCommReg(VOS_UINT32 senderPid, PsOmLayerMsgReplaceCb func,
    PsOmCpuLayerMsgFilterCtrl *cpuFilter, PsOmPerPidRegCtrl *pidFilter)
{
    VOS_UINT32       result;
    const VOS_UINT32 expendNum = 8;

    if ((cpuFilter == VOS_NULL_PTR) || (pidFilter == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    /* ��Pid�±���ע��Ĺ��˻ص��ӿ��ڴ治�㣬�Ƚ����ڴ���չ�����������װ������һ���߼� */
    if (pidFilter->useCnt == pidFilter->maxCnt) {
        result = PS_OM_LayerMsgReplaceMemExpand(senderPid, sizeof(PsOmLayerMsgReplaceCb), expendNum,
            &(pidFilter->maxCnt), (VOS_VOID**)&(pidFilter->filterFunc));
        if (result != VOS_OK) {
            return VOS_ERR;
        }
    }

    /* ִ�е�����ʱһ��OK, ע����Ӧָ�벢�޸����Ӽ����ֶ� */
    if (pidFilter->useCnt == 0) {
        pidFilter->senderPid = senderPid;
        cpuFilter->useCnt++;
    }

    pidFilter->filterFunc[pidFilter->useCnt] = func;
    pidFilter->useCnt++;

    return VOS_OK;
}

STATIC VOS_UINT32 PS_OM_LocalLayerMsgReplaceFuncCommReg(
    PsOmLayerMsgReplaceCb func, PsOmLayerMsgReplaceCtrl *layerMsgReplaceCtrl, VOS_UINT32 senderPid)
{
    const VOS_UINT16           pidIdx = (VOS_UINT16)senderPid;
    VOS_UINT32                 result;
    PsOmCpuLayerMsgFilterCtrl *localCpuFilter = &(layerMsgReplaceCtrl->localCpuFilter);
    PsOmPerPidRegCtrl         *localPidFilter = VOS_NULL_PTR;

    /* ���ڲ����Ϣ��������ֻ����һ�Σ���Pid���������룬������չ */
    if (localCpuFilter->useCnt == 0) {
        result = PS_OM_LayerMsgReplaceMemExpand(senderPid, sizeof(PsOmPerPidRegCtrl),
            layerMsgReplaceCtrl->localCpuMaxPidCnt, &(localCpuFilter->maxCnt),
            (VOS_VOID**)&(localCpuFilter->pidMsgFilter));
        if (result != VOS_OK) {
            return VOS_ERR;
        }
    }

    /* �±걣�� */
    if (pidIdx >= localCpuFilter->maxCnt) {
        return VOS_ERR;
    }

    localPidFilter = &(localCpuFilter->pidMsgFilter[pidIdx]);
    return PS_OM_OnePidFilterFuncCommReg(senderPid, func, localCpuFilter, localPidFilter);
}

STATIC VOS_UINT32 PS_OM_DiffCpuLayerMsgReplaceFuncCommReg(
    PsOmLayerMsgReplaceCb func, PsOmLayerMsgReplaceCtrl *layerMsgReplaceCtrl, VOS_UINT32 senderPid)
{
    VOS_UINT32                 loop;
    VOS_UINT32                 result;
    const VOS_UINT32           expendNum     = 20;
    PsOmCpuLayerMsgFilterCtrl *diffCpuFilter = &(layerMsgReplaceCtrl->diffCpuFilter);
    PsOmPerPidRegCtrl         *diffPidFilter = VOS_NULL_PTR;

    /* �����Ϣ������Pid���٣���������ѭ�����ҷ��� */
    for (loop = 0; loop < diffCpuFilter->useCnt; loop++) {
        if (senderPid == diffCpuFilter->pidMsgFilter[loop].senderPid) {
            diffPidFilter = &(diffCpuFilter->pidMsgFilter[loop]);
            break;
        }
    }

    /* �Ƚ����ڴ���չ�����������װ������һ���߼� */
    if (diffPidFilter == VOS_NULL_PTR) {
        if (diffCpuFilter->maxCnt == diffCpuFilter->useCnt) {
            result = PS_OM_LayerMsgReplaceMemExpand(senderPid, sizeof(PsOmPerPidRegCtrl), expendNum,
                &(diffCpuFilter->maxCnt), (VOS_VOID**)&(diffCpuFilter->pidMsgFilter));
            if (result != VOS_OK) {
                return VOS_ERR;
            }
        }

        diffPidFilter = &(diffCpuFilter->pidMsgFilter[diffCpuFilter->useCnt]);
    }

    return PS_OM_OnePidFilterFuncCommReg(senderPid, func, diffCpuFilter, diffPidFilter);
}

/*
 * A/C��ע������Ϣƥ����˻ص��ӿڣ���������Ϣ���ͷ�ͳһʹ�÷���PID
 * ����ƥ����˽ӿ�ע�ᣬԭ����һ������PIDֻ����ע��һ��ƥ����˽ӿڣ�
 * ���ǶԿ����Ϣ��͸��ת����Ϣ�����ⳡ���������ͷ�����շ�ʹ��
 * ����PIDע����ƥ����˽ӿ�
 */
VOS_UINT32 PS_OM_LayerMsgReplaceCBCommReg(
    PsOmLayerMsgReplaceCtrl *layerMsgCtrl, VOS_UINT32 senderPid, PsOmLayerMsgReplaceCb func)
{
    VOS_UINT32 result;

    /* ��κϷ��Լ�顢PID��Χ��� */
    if ((layerMsgCtrl == VOS_NULL_PTR) || (func == VOS_NULL_PTR)) {
        g_layerMsgFilterStatistic.regFailCnt++;
        return VOS_ERR;
    }

    if (layerMsgCtrl->localCpuId == VOS_GET_CPU_ID(senderPid)) {
        /* ���ڲ����Ϣ���˻ص�ע�ᴦ�� */
        result = PS_OM_LocalLayerMsgReplaceFuncCommReg(func, layerMsgCtrl, senderPid);
    } else {
        /* ��˲����Ϣ���˻ص�ע�ᴦ�� */
        result = PS_OM_DiffCpuLayerMsgReplaceFuncCommReg(func, layerMsgCtrl, senderPid);
    }

    if (result == VOS_OK) {
        g_layerMsgFilterStatistic.regSuccCnt++;
    } else {
        g_layerMsgFilterStatistic.regFailCnt++;
    }

    return result;
}


