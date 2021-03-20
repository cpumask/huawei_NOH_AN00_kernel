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

#ifndef __PPP_CONVERT_H__
#define __PPP_CONVERT_H__

#include "vos.h"
#include "ppp_comm_def.h"
#include "ppp_decode.h"
#include "ppp_encode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

enum PPP_ConvState {
    PPP_CONV_NULL,
    PPP_CONV_BUSY,
    PPP_CONV_BUTT
};
typedef VOS_UINT32 PPP_ConvStateUint32;

/* ����ͬ���첽ת������ʵ������Ļص��ӿ� */
typedef struct {
    PPP_DecCfg decCfg; /* �������� */
    PPP_EncCfg encCfg; /* �������� */
} PPP_ConvOps;

/* �������ʵ�� */
typedef struct {
    PPP_ConvStateUint32 state;
    PPP_DecCtrl         decCtrl;
    PPP_EncCfg          encCfg;
    VOS_UINT32          userData;
} PPP_ConvCtrl;

/* ���������ͬ�첽֡ת��ʵ�彨��, �ϲ���Ҫ��֤�����������ȷ�Ժ���Ч�ԣ���δЭ�̵Ĳ���ʹ��Ĭ��ֵ */
VOS_VOID PPP_ConvCreate(PPP_ConvCtrl* self, const PPP_ConvOps *ops);

/* ��ȡ��ǰpppIdʵ���������Ϣ */
VOS_VOID PPP_ConvGetCfg(const PPP_ConvCtrl* self, PPP_ConvOps *ops);

/* ����pppIdʵ�壬��Ҫ����LCPЭ����ɺ������·���������ˢ��ʵ�� */
VOS_VOID PPP_ConvReCfg(PPP_ConvCtrl* self, const PPP_ConvOps *ops);

/* ���������ͬ�첽֡ת��ʵ���ͷ�, �ϲ���Ҫ��֤�����������ȷ�Ժ���Ч�� */
VOS_UINT32 PPP_ConvRelease(PPP_ConvCtrl* self);

/*
 * ����������첽֡תͬ��֡�ӿ�, �ϲ���Ҫ��֤�����������ȷ�Ժ���Ч��
 * ע�⣺1.data�е��ڴ���Ҫ�ϲ�ģ��������ͷ�
 *       2.�ýӿڴ�������У����ܶ�λص�PppDecFrmDeliver���ص������漰�Ķ�̬�ڴ�Ҳ��Ҫ�ϲ��ͷ�
 */
VOS_UINT32 PPP_ConvDecode(PPP_ConvCtrl* self, const VOS_UINT8 *data, VOS_UINT16 dataLen, VOS_UINT32 userData);

/*
 * ���������ͬ��֡ת�첽֡�ӿ�, �ϲ���Ҫ��֤�����������ȷ�Ժ���Ч��
 * ע�⣺1.input�е��ڴ�����Ƕ�̬�����ڴ���Ҫ�ϲ�ģ��������ͷ�
 *       2.������VOS_OKʱ��output�а�����̬�����ڴ棬��Ҫ�ϲ�����ͷ�
 */
VOS_UINT32 PPP_ConvEncode(PPP_ConvCtrl* self, const PPP_EncInput *input, PPP_EncOutput *output);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
