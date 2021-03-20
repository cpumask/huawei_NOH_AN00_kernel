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

#ifndef __PPP_COMM_DEF_H__
#define __PPP_COMM_DEF_H__

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* PPPЭ��ؼ������ַ� */
#define PPP_SYN_C 0x7E
#define PPP_ESC_C 0x7D
#define PPP_XOR_C 0x20

/* ��ַ������ */
#define PPP_ADDR_C 0xFF
#define PPP_CTRL_C 0x03
#define PPP_ACF_S 0xFF03
#define PPP_ACF_LEN 2
#define PPP_CACF_LEN 0

/* Э������� */
#define PPP_PF_MASK 0x01
#define PPP_PF_LEN 2
#define PPP_CPF_LEN 1

#define PPP_DEFAULT_ACCMAP 0xFFFFFFFF

#define PPP_LCP_P 0xc021 /* Link Control Protocol */

#define PPP_COMM_CLEAR_UPLAYER_MEM(mem) do { \
    (mem)->raw  = VOS_NULL_PTR;     \
    (mem)->data = VOS_NULL_PTR;     \
    (mem)->size = 0;                \
} while (0)

typedef struct tagPPP_UplayerMem {
    VOS_VOID  *raw;  /* �ϲ������ԭʼ�ڴ�ָ�룬��ģ��ֻ���𱣴棬����֪�ϲ��ڴ�ṹ */
    VOS_UINT8 *data; /* �洢���ݵ������ڴ���ʼ��ַ */
    VOS_UINT32 size; /* �洢���ݵ������ڴ��С */
} PPP_UplayerMem;

/* �ڴ����빳�Ӻ����������߽�������ڴ���д������buf�з��ظ���ģ�� */
typedef VOS_UINT32 (*PPP_UplayerMemAlloc)(VOS_UINT32 memMaxLen, PPP_UplayerMem *buf);

/* �ڴ��ͷŹ��Ӻ��� */
typedef VOS_VOID (*PPP_UplayerMemFree)(PPP_UplayerMem *buf);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
