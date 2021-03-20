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

#ifndef __PSLOGFILTERCOMM_H__
#define __PSLOGFILTERCOMM_H__

#include "vos.h"
#include "ps_log_filter_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/*
 * ÿ��SendPid�´洢�Ĺ��˺���, �󲿷ֳ���һ��SendPidֻ����
 * ע��һ�����˺�������˺���Ϣ͸��ת�������ⳡ������ע����
 */
typedef struct {
    VOS_UINT32             senderPid;  /* ע���SendPid���洢���ǵ�16λ */
    VOS_UINT16             useCnt;     /* ÿ��SendPid��ע��Ĺ��˺����ĸ��� */
    VOS_UINT16             maxCnt;     /* ����ע��Ĺ��˺�����������ֵ�ɶ�̬��������������ﵽ���޺�������� */
    PsOmLayerMsgReplaceCb *filterFunc; /* ָ��洢���˺����Ķ�̬���� */
} PsOmPerPidRegCtrl;

/* �����Ϣ���˹��ܵĿ��ƽṹ����SendPidΪ��Ԫע��Ĺ��˻ص����� */
typedef struct {
    VOS_UINT16 useCnt;               /* ��ע���SendPid�ĸ��� */
    VOS_UINT16 maxCnt;               /* ����ע���SendPid���� */
    PsOmPerPidRegCtrl *pidMsgFilter; /* ָ����SendPidΪ��Ԫע��Ĺ��˻ص��������飬�����С��usMaxCntȷ����ʹ��������usUseCntȷ�� */
} PsOmCpuLayerMsgFilterCtrl;

/* �����Ϣ���˹��ܵĿ��ƽṹ */
typedef struct {
    VOS_UINT16                localCpuMaxPidCnt; /* ȡֵ��VOS_PID_BUTT��16λ��ͬ����ʾ��CPU�����PID������ */
    VOS_UINT8                 localCpuId;        /* ȡֵ��VOS_CPU_ID_ENUM��ͬ��0����C�ˣ�1����A�� */
    VOS_UINT8                 reserve[1];
    PsOmCpuLayerMsgFilterCtrl localCpuFilter; /* ͬ�˲����Ϣ���˿��ƽṹ */
    PsOmCpuLayerMsgFilterCtrl diffCpuFilter;  /* ��˲����Ϣ���˿��ƽṹ */
} PsOmLayerMsgReplaceCtrl;

MsgBlock* PS_OM_CpuLayerMsgCommReplace(MsgBlock *msg, PsOmLayerMsgReplaceCtrl *layerMsgReplaceCtrl);

/*
 * A C�˿�˲����Ϣƥ����˺���ע��ӿ�
 *
 * ��������ص��ӿڹ���:
 * 1.���ע��ص��ڲ�û�ж���Ϣ���д�������Ҫ�����ָ�뷵�أ�����
 *  ��ģ�鲻֪���Ƿ���Ҫ������Ϣ���ݸ���һ��ע��ص����д���
 * 2.���ע��ص��ڲ�����Ϣ�����˴����򷵻�ֵ�ܹ�ʵ����������:
 *  �ٷ���VOS_NULL���򽫴���Ϣ������ȫ���ˣ������ٹ�ȡ����
 *  �ڷ��������ָ�벻ͬ����һ��ָ�룬��ȡ����Ϣ����ʹ�÷��ص�ָ
 *  �������滻ԭ��Ϣ�����ݡ�
 */
VOS_UINT32 PS_OM_LayerMsgReplaceCBCommReg(
    PsOmLayerMsgReplaceCtrl *layerMsgCtrl, VOS_UINT32 senderPid, PsOmLayerMsgReplaceCb func);

VOS_VOID PS_OM_LogFilterCommShow(PsOmLayerMsgReplaceCtrl *layerMsgCtrl);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
