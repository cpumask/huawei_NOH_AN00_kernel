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

#include "ps_acpu_logfilter.h"
#include "gucttf_tag.h"
#include "ttf_comm.h"
#include "ps_logfilter_comm.h"
#include "msp_diag_comm.h"


#define THIS_FILE_ID PS_FILE_ID_ACPU_LOG_FILTER_C
#define THIS_MODU mod_logfilter

STATIC PsOmLayerMsgReplaceCtrl g_acpuLayerMsgReplaceCtrl = { (VOS_UINT16)VOS_PID_BUTT, (VOS_UINT8)VOS_CPU_ID_ACPU, {0},
    { 0, 0, VOS_NULL_PTR }, { 0, 0, VOS_NULL_PTR }};

STATIC VOS_VOID* PS_OM_ACpuLayerMsgReplaceCB(VOS_VOID *msg)
{
    return PS_OM_CpuLayerMsgCommReplace((MsgBlock*)msg, &g_acpuLayerMsgReplaceCtrl);
}

STATIC VOS_UINT32 PS_OM_ACpuLayerMsgReplaceMemFreeCB(VOS_UINT32 senderPid, VOS_VOID *addr)
{
    return VOS_MemFree(senderPid, addr);
}

/*
 * ע������Ϣ�滻�ص��ӿ�
 * ��������ص��ӿڹ���:
 * 1.���ݷ���PID����ע��
 * 2.ע����Ϣ���˻ص���
 *  ���ע��ص��ڲ�����Ϣδ���д�������ԭ���ָ�룻
 *  ���ע��ص��ڲ�����Ϣ�����˴����򷵻�ֵ�ܹ�ʵ����������:
 *   �ٷ���VOS_NULL���򽫴���Ϣ������ȫ���ˣ������ٹ�ȡ����
 *   �ڷ��������ָ�벻ͬ����һ��ָ�룬��ȡ����Ϣ����ʹ�÷��ص�ָ
 *  �������滻ԭ��Ϣ�����ݡ�
 */
MODULE_EXPORTED VOS_UINT32 PS_OM_LayerMsgReplaceCBReg(VOS_UINT32 senderPid, PsOmLayerMsgReplaceCb func)
{
    static VOS_UINT32 firstRegFlag   = 0;
    const VOS_UINT32  regMagicNumber = 0x5A5AA5A5;

    /* ����ע�ᣬ�����ע��ص����� */
    if (firstRegFlag == 0) {
        (VOS_VOID)mdrv_diag_layer_msg_match_func_reg(PS_OM_ACpuLayerMsgReplaceCB);
        (VOS_VOID)mdrv_diag_layer_msg_notify_func_reg(PS_OM_ACpuLayerMsgReplaceMemFreeCB);
        firstRegFlag = regMagicNumber;
    }

    return PS_OM_LayerMsgReplaceCBCommReg(&g_acpuLayerMsgReplaceCtrl, senderPid, func);
}


