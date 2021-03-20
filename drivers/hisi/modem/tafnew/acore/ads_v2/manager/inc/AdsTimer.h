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

#ifndef __ADS_TIMER_H__
#define __ADS_TIMER_H__

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

/* ADS��ͬʱ���еĶ�ʱ���������Ŀ,Ŀǰֻ��һ�� */
#define ADS_MAX_TIMER_NUM (TI_ADS_TIMER_BUTT)

/* ��ʱ��TI_ADS_DSFLOW_STATS��ʱ����1��ͳ��һ������ */
#define TI_ADS_DSFLOW_STATS_LEN (1000)

/*
 * ö��˵��: ADS��ʱ����ID
 */
enum ADS_TIMER_Id {
    TI_ADS_TIMER_RESERVED = 0x00,
    TI_ADS_DSFLOW_STATS   = 0x01, /* ����ͳ�ƶ�ʱ�� */

    TI_ADS_TIMER_BUTT
};
typedef VOS_UINT32 ADS_TIMER_IdUint32;

/*
 * ö��˵��: ADS_TIMER_OperationType
 * ö��˵��: ADS��ʱ��������ȡֵ������SDT����ʾ
 */
enum ADS_TIMER_OperationType {
    ADS_TIMER_OPERATION_START, /* ������ʱ�� */
    ADS_TIMER_OPERATION_STOP,  /* ֹͣ��ʱ�� */
    ADS_TIMER_OPERATION_TYPE_ENUM_BUTT
};
typedef VOS_UINT8 ADS_TIMER_OperationTypeUint8;

/*
 * ö��˵��: ADS_TIMER_StopCause
 * ö��˵��: ADS��ʱ��ֹͣ��ԭ������SDT����ʾ
 */
enum ADS_TIMER_StopCause {
    ADS_TIMER_STOP_CAUSE_USER,    /* �û�����ֹͣ�� */
    ADS_TIMER_STOP_CAUSE_TIMEOUT, /* ��ʱ����ʱ��ʾֹͣ�� */
    ADS_TIMER_STOP_CAUSE_ENUM_BUTT
};
typedef VOS_UINT8 ADS_TIMER_StopCauseUint8;

/*
 * 4 STRUCT&UNION����
 */

/*
 * �ṹ˵��: ADS��ʱ������ʱ��������
 */
typedef struct {
    HTIMER    timer;  /* ��ʱ��������ָ�� */
    VOS_UINT8 rsv[8]; /* ���� */
} ADS_TIMER_Ctx;

/*
 * �ṹ˵��: ADS��ʱ����Ϣ�ṹ������SDT����ʾ
 */
typedef struct {
    VOS_MSG_HEADER                               /* _H2ASN_Skip */
    ADS_TIMER_IdUint32           timerId;        /* _H2ASN_Skip */
    VOS_UINT32                   timerLen;       /* ��ʱ������ */
    ADS_TIMER_OperationTypeUint8 timerAction;    /* ��ʱ���������� */
    ADS_TIMER_StopCauseUint8     timerStopCause; /* ��ʱ��ֹͣ��ԭ�� */
    VOS_UINT8                    reserved[2];
} ADS_TIMER_Info;

VOS_VOID ADS_StartTimer(ADS_TIMER_IdUint32 timerId, VOS_UINT32 len);

VOS_VOID ADS_StopTimer(ADS_TIMER_IdUint32 timerId, ADS_TIMER_StopCauseUint8 stopCause);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __ADS_TIMER_H__ */
