/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#ifndef __ACORE_NV_STRU_GUCNAS_H__
#define __ACORE_NV_STRU_GUCNAS_H__

#include "vos.h"
#include "acore_nv_id_gucnas.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define ADS_UL_QUEUE_SCHEDULER_PRI_MAX 9 /* ���ȼ���������� */


enum ADS_IpfMode {
    ADS_RD_INT   = 0x00, /* �ж������� */
    ADS_RD_THRD  = 0x01, /* �߳������� */

    ADS_IPFMODE_BUTT
};
typedef VOS_UINT8 ADS_IpfModeUint8;


typedef struct {
    /*
     * IPF����ADS�������ݵ�ģʽ��
     * 0���ж������ģ�
     * 1���߳������ġ�
     */
    ADS_IpfModeUint8 ipfMode;
    VOS_UINT8        reserved0; /* ����λ */
    VOS_UINT8        reserved1; /* ����λ */
    VOS_UINT8        reserved2; /* ����λ */
} TAF_NV_AdsIpfModeCfg;


typedef struct {
    /*
     * ���������Ƿ�ʹ�ܡ�
     * 1��ʹ��
     * 0����ֹ
     */
    VOS_UINT32 enable;
    VOS_UINT32 txWakeTimeout; /* ���ͳ���ʱ�䣬��λ�����롣 */
    VOS_UINT32 rxWakeTimeout; /* ���ճ���ʱ�䣬��λ�����롣 */
    VOS_UINT32 reserved;      /* �����ֽڡ� */
} TAF_NV_AdsWakeLockCfg;


typedef struct {
    /*
     * �ڴ��������
     * ע��BlkSizeΪ448 bytes���ڴ������СΪ512��BlkSizeΪ1540 bytes���ڴ������С768�顣
     */
    VOS_UINT16 blkNum;
    VOS_UINT16 reserved0; /* �����ֽڡ� */
    VOS_UINT16 reserved1; /* �����ֽڡ� */
    VOS_UINT16 reserved2; /* �����ֽڡ� */

} TAF_NV_AdsMemCfg;


typedef struct {
    /*
     * ��CAHCE�����Ƿ�ʹ�ܡ�
     * 1��ʹ�ܣ�
     * 0����ֹ��
     */
    VOS_UINT32 enable;
    /*
     * memCfg[0]��BlkSizeΪ448 bytes���ڴ����ã�
     * memCfg[1]��BlkSizeΪ1540 bytes���ڴ����ã�
     */
    TAF_NV_AdsMemCfg memCfg[2];
} TAF_NV_AdsMemPoolCfg;


typedef struct {
    /*
     * ���������Ƿ�ʹ�ܡ�
     * 1��ʹ��
     * 0����ֹ
     */
    VOS_UINT32 enabled;
    /* �������ֵ��ȡֵ��Χ[50,100]����λ���ٷֱȡ� */
    VOS_UINT32 errorRateThreshold;  /* �������ֵ��ȡֵ��Χ[50,100]����λ���ٷֱȡ� */
    VOS_UINT32 detectDuration;      /* ���������ʱ�䣬ȡֵ��Χ[1000,10000]����λ�����롣*/
    VOS_UINT32 reserved;            /* �����ֽ� */

} TAF_NV_AdsErrorFeedbackCfg;


typedef struct {
    /*
     * �Ƿ�ʹ��ADS_Queue_Scheduler_Pri���ԡ�
     * 0��δʹ�ܣ���ʾ���������ȼ�,�ȼ����ȴ���
     * 1���������ȼ��㷨��
     */
    VOS_UINT32 status;
    VOS_UINT16 priWeightedNum[ADS_UL_QUEUE_SCHEDULER_PRI_MAX]; /* ADS���ж������ȼ��ļ�Ȩֵ�� */
    VOS_UINT8  rsv[2];                                         /* ����λ */
} ADS_UL_QueueSchedulerPriNv;


typedef struct {
    VOS_UINT32 waterLevel1; /* ˮ�߽��1 */
    VOS_UINT32 waterLevel2; /* ˮ�߽��2 */
    VOS_UINT32 waterLevel3; /* ˮ�߽��3 */
    VOS_UINT32 waterLevel4; /* ˮ�߽��4,Ԥ�� */
} ADS_UL_WaterMarkLevel;


typedef struct {
    VOS_UINT32 threshold1; /* �ް�����1 */
    VOS_UINT32 threshold2; /* �ް�����2 */
    VOS_UINT32 threshold3; /* �ް�����3 */
    VOS_UINT32 threshold4; /* �ް�����4 */
} ADS_UL_ThresholdLevel;


typedef struct {
    VOS_UINT32                   activeFlag;       /* ʹ�ܱ�ʶ��0��ȥʹ�ܣ�1��ʹ�� */
    VOS_UINT32                   protectTmrExpCnt; /* ������ʱ����ʱ����ʱ������λ���롣 */
    ADS_UL_WaterMarkLevel waterMarkLevel;   /* ���ݰ�ˮ�߽�� */
    ADS_UL_ThresholdLevel  thresholdLevel;   /* ��̬������ޡ� */
    VOS_UINT32                   reserved[6];     /* �����ֶ� */
} ADS_NV_DynamicThreshold;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __ACORE_NV_STRU_GUCNAS_H__ */
