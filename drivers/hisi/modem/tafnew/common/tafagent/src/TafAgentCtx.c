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

#include "TafAgentCtx.h"
#include "TafAgentLog.h"
#include "securec.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */

#define THIS_FILE_ID PS_FILE_ID_TAF_AGENT_CTX_C

TAFAGENT_CTX        g_tafAgentCtx   = {0};
TAF_AGENT_StatsInfo g_tafAgentStats = {0};


VOS_UINT32 Taf_Agent_InitCtx(VOS_VOID)
{
    g_tafAgentCtx.tafAcpuCnfMsg = VOS_NULL_PTR;

    /* ���以���ź��� */
    if (VOS_OK != VOS_SmMCreate("SYNC", VOS_SEMA4_FIFO, &g_tafAgentCtx.tafAcpuSyncSem)) {
        TAFAGENT_ERROR_LOG(ACPU_PID_TAFAGENT, "Create aps acpu sycn sem failed!");
        TAF_AGENT_DBG_CREATE_MUTEX_SEM_FAIL_NUM(1);
        g_tafAgentCtx.initFlg = VOS_FALSE;
        return VOS_ERR;
    } else {
        TAF_AGENT_DBG_SAVE_MUTEX_SEM_ID(g_tafAgentCtx.tafAcpuSyncSem);
    }

    /* ����������ź��� */
    if (VOS_OK != VOS_SmBCreate("CNF", 0, VOS_SEMA4_FIFO, &g_tafAgentCtx.tafAcpuCnfSem)) {
        TAFAGENT_ERROR_LOG(ACPU_PID_TAFAGENT, "Create aps acpu cnf sem failed!");
        TAF_AGENT_DBG_CREATE_BINARY_SEM_FAIL_NUM(1);
        g_tafAgentCtx.initFlg = VOS_FALSE;
        return VOS_ERR;
    } else {
        TAF_AGENT_DBG_SAVE_BINARY_SEM_ID(g_tafAgentCtx.tafAcpuCnfSem);
    }

    g_tafAgentCtx.initFlg = VOS_TRUE;

    TAF_AGENT_SET_ACPU_CNF_SEM_LOCK_FLG(VOS_FALSE);

    TAF_AGENT_ClearMsg();

    return VOS_OK;
}


VOS_SEM TAF_AGENT_GetTafAcpuSyncSem(VOS_VOID)
{
    return g_tafAgentCtx.tafAcpuSyncSem;
}


VOS_SEM TAF_AGENT_GetTafAcpuCnfSem(VOS_VOID)
{
    return g_tafAgentCtx.tafAcpuCnfSem;
}


VOS_UINT8* TAF_AGENT_GetTafAcpuCnfMsg(VOS_VOID)
{
    return g_tafAgentCtx.tafAcpuCnfMsg;
}


VOS_VOID TAF_AGENT_SetTafAcpuCnfMsg(VOS_UINT8 *msg)
{
    g_tafAgentCtx.tafAcpuCnfMsg = msg;
}


VOS_VOID TAF_AGENT_ClearMsg(VOS_VOID)
{
    (VOS_VOID)memset_s(g_tafAgentCtx.msg, sizeof(g_tafAgentCtx.msg), 0x00, sizeof(g_tafAgentCtx.msg));
}


VOS_UINT8* TAF_AGENT_SaveMsg(VOS_UINT8 *msg, VOS_UINT32 len)
{
    errno_t memResult;
    if ((TAFAGENT_APS_MAX_MSG_LEN - VOS_MSG_HEAD_LENGTH) >= len) {
        /* ���ݻظ���Ϣ */
        memResult = memcpy_s(g_tafAgentCtx.msg, sizeof(g_tafAgentCtx.msg), msg, len + VOS_MSG_HEAD_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_tafAgentCtx.msg), len + VOS_MSG_HEAD_LENGTH);

        return &(g_tafAgentCtx.msg[0]);
    } else {
        PS_PRINTF("TAF_AGENT_SaveMsg-->msg len too big\n");
        return VOS_NULL_PTR;
    }
}


VOS_VOID TAF_AGENT_ShowStats(VOS_VOID)
{
    PS_PRINTF_INFO("TAF AGENT STATS START:                   \n");
    PS_PRINTF_INFO("ulInitFlg                              %d\n", g_tafAgentCtx.initFlg);
    PS_PRINTF_INFO("hTafAcpuSyncSem                        %u\n", g_tafAgentCtx.tafAcpuSyncSem); /*lint !e559 */
    PS_PRINTF_INFO("hTafAcpuCnfSem                         %u\n", g_tafAgentCtx.tafAcpuCnfSem);  /*lint !e559 */
    PS_PRINTF_INFO("ulMutexSemId                           %u\n", g_tafAgentStats.mutexSemId);   /*lint !e559 */
    PS_PRINTF_INFO("ulBinarySemId                          %u\n", g_tafAgentStats.binarySemId);  /*lint !e559 */
    PS_PRINTF_INFO("ulCreateMutexSemFailNum                %d\n", g_tafAgentStats.createMutexSemFailNum);
    PS_PRINTF_INFO("ulCreateBinarySemFailNum               %d\n", g_tafAgentStats.createBinarySemFailNum);
    PS_PRINTF_INFO("ulLockMutexSemFailNum                  %d\n", g_tafAgentStats.lockMutexSemFailNum);
    PS_PRINTF_INFO("ulLockBinarySemFailNum                 %d\n", g_tafAgentStats.lockBinarySemFailNum);
    PS_PRINTF_INFO("ulLastMutexSemErrRslt                  %x\n", g_tafAgentStats.lastMutexSemErrRslt);
    PS_PRINTF_INFO("ulLastBinarySemErrRslt                 %x\n", g_tafAgentStats.lastBinarySemErrRslt);
    PS_PRINTF_INFO("ulSyncMsgIsNullNum                     %d\n", g_tafAgentStats.syncMsgIsNullNum);
    PS_PRINTF_INFO("ulSyncMsgNotMatchNum                   %d\n", g_tafAgentStats.syncMsgNotMatchNum);

    PS_PRINTF_INFO("TAF AGENT STATS END.                     \n");
}

