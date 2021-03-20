/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
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

#ifndef __PPP_DECODE_H__
#define __PPP_DECODE_H__

#include "vos.h"
#include "ppp_comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

enum PPP_DecState {
    PPP_DEC_NULL,
    PPP_DEC_SYN,
    PPP_DEC_FRM,
    PPP_DEC_BUTT
};
typedef VOS_UINT32 PPP_DecStateUint32;

/* �ϱ����ϲ���Ч֡�Ľ����� */
typedef struct {
    PPP_UplayerMem buf;      /* ͨ��PppDecBufAlloc�������Ϣ�������� */
    VOS_UINT32     used;     /* �Ѵ洢�����ݳ��ȣ�������FCS */
    VOS_UINT16     proto;    /* Э���ֶ�ȡֵ */
    VOS_UINT32     pppId;    /* �����ϲ���Ҫ���ص���Ϣ����ǰ���ڴ洢PppId */
} PPP_DecFrame;

/* �ϱ����ϲ���Ч֡�Ľ��������Ӻ������ϲ�Ӧ���Լ������ͷ�PppUplayerMem��Ӧ���ڴ� */
typedef VOS_VOID (*PppDecFrameDeliver)(PPP_DecFrame *frm);

/* ����������ýṹ */
typedef struct {
    PPP_UplayerMemAlloc alloc;
    PPP_UplayerMemFree  free;
    PppDecFrameDeliver  deliver;
    VOS_UINT32          memMaxLen;
    VOS_UINT32          ipMode;
} PPP_DecCfg;

/* �������ʵ�� */
typedef struct {
    PPP_DecStateUint32 state;
    VOS_BOOL           escMode;
    PPP_DecCfg         ops;
    PPP_UplayerMem     buf;       /* ���ݻ����� */
    VOS_UINT16         used;      /* �Ѵ洢�����ݳ��� */
    VOS_UINT16         fcs;       /* ����Ĺ����м����FCS */
    VOS_UINT8          getHead;
    VOS_UINT8          rsv;
    VOS_UINT16         proto;
    VOS_UINT32         transData; /* ͸�����ϲ�����ݣ����㲻ʹ�� */
} PPP_DecCtrl;

VOS_UINT32 PPP_DecProc(PPP_DecCtrl *ctrl, const VOS_UINT8 *data, VOS_UINT16 dataLen, VOS_UINT32 pppId);
VOS_VOID   PPP_DecCtrlCreate(PPP_DecCtrl *ctrl, const PPP_DecCfg *ops);
VOS_VOID   PPP_DecCtrlRelease(PPP_DecCtrl *ctrl);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
