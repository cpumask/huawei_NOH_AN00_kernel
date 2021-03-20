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

#ifndef __NV_STRU_GAS_H__
#define __NV_STRU_GAS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 2)

#include "vos.h"
#include "nv_id_guas.h"

#define NVIM_TDS_MAX_SUPPORT_BANDS_NUM 8

enum NVIM_TdsFreqBandList {
    ID_NVIM_TDS_FREQ_BAND_A = 0x01, /* Ƶ�㷶Χ: 9504~9596  10054~10121 */
    ID_NVIM_TDS_FREQ_BAND_B = 0x02, /* Ƶ�㷶Χ: 9254~9546  9654~9946 */
    ID_NVIM_TDS_FREQ_BAND_C = 0x04, /* Ƶ�㷶Χ: 9554~9646 */
    ID_NVIM_TDS_FREQ_BAND_D = 0x08, /* Ƶ�㷶Χ: 12854~13096 */
    ID_NVIM_TDS_FREQ_BAND_E = 0x10, /* Ƶ�㷶Χ: 11504~11996 */
    ID_NVIM_TDS_FREQ_BAND_F = 0x20, /* Ƶ�㷶Χ: 9404~9596 */
    ID_NVIM_TDS_FREQ_BAND_BUTT
};
typedef VOS_UINT8 NVIM_TdsFreqBandListUint8;


/*
 * �ṹ˵��: ��ʶMS��EGPRS��ʱ϶�����ȼ����μ�Э��24008�µ�10.1.5.7��
 */
typedef struct {
    /*
     * 0x0000��MS��EGPRS��ʱ϶�����ȼ�Ϊ0x000C��
     * 0x0001��0x000C��MS��EGPRS��ʱ϶�����ȼ���
     * ��Χ�� [0, 12]
     */
    VOS_UINT16 egprsMultiSlotClass;
    VOS_UINT8  rsv[2];
} NVIM_EgprsMultiSlotClass;

/*
 * �ṹ˵��: MS�Ƿ�֧��EGPRS����NV�������ÿ��Modem�������á�
 */
typedef struct {
    /*
     * 0x0000����֧��EGPRS��
     * 0x0001��֧��EGPRS��
     */
    VOS_UINT16                          egprsFlag;
    VOS_UINT8                           rsv[2];  /* Ԥ�� */
} NVIM_EgprsFlag;

/*
 * �ṹ˵��: ��ʾGPRS Active Timer�ĳ��ȡ�GPRS Active Timer���й����У�����GDSP���𱳾�������
 */
typedef struct {
    /*
     * 0x0000_0000������GPRS Active Timer��GPRS Active Timer �ĳ���Ϊ100ms��
     * 0x0000_0001��GPRS Active Timer�ĳ���Ϊ100ms��
     * 0x0000_0002��GPRS Active Timer�ĳ���Ϊ200ms��
     * �Դ����ơ�
     */
    VOS_UINT32                          gprsActiveTimerLength;
} NVIM_GprsActiveTimerLen;

/*
 * �ṹ˵��: ��ʶMS��GPRS��ʱ϶�����ȼ���
 */
typedef struct {
    /*
     * 0x0000����ȡֵΪ0x0000ʱMS��GPRS��ʱ϶�����ȼ�Ϊ0x000C��
     * 0x0001~0x000C��MS��GPRS��ʱ϶�����ȼ���
     */
    VOS_UINT16                          gprsMultiSlotClass;
    VOS_UINT8                           rsv[2];  /* Ԥ�� */
} NVIM_GprsMultiSlotClass;

/*
 * �ṹ˵��: MS�Ƿ�֧��multislot classe 33��High Multislot Capability���嶨��μ�Э��24008-10.5.1.7��
 */
typedef struct {
    /*
     * 0��High multislot class��Ч��
     * 1��High multislot class��Ч��
     */
    VOS_UINT16                              highMultislotClassFlg;
    /*
     * 0��High Multislot Class�ȼ�Ϊ0��Ŀǰ��֧������Ϊ��ֵ��
     * ��NV����Ҫ��Gprs_MultiSlotClass����Egprs_MultiSlotClass NV������ʹ�á���High multislot class��Ч��High Multislot
     * Class�ȼ�Ϊ0����Gprs_MultiSlotClass�ȼ�Ϊ12����Egprs_MultiSlotClassΪ12ʱ����ʾMS֧�ֶ�ʱ϶�����ȼ�33��
     */
    VOS_UINT16                              highMultislotClass;
} NVIM_GAS_HighMultiSlotClass;

/*
 * �ṹ˵��: TDS֧�ֵ�Ƶ�θ����Լ�Ƶ�κš�
 */
typedef struct {
    /* ֧�ֵ�TDSƵ�θ�����Ŀǰ���֧������Ƶ�Σ�A/E/F������֧��B/C/D������Ƶ�Ρ�ȡֵ��Χ0~6�� */
    VOS_UINT8                               bandCnt;
    VOS_UINT8                               rsv[3];  /* Ԥ�� */
    /*
     * aucBandNo[x]��ʾ֧�ֵ�TDSƵ�κţ�Ʃ�磺
     * aucBandNo[0]=0x01, aucBandNo[1]=0x10,
     * aucBandNo[2]=0x20��
     */
    VOS_UINT8                               bandNo[NVIM_TDS_MAX_SUPPORT_BANDS_NUM];
} NVIM_UeTdsSupportFreqBandList;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

/* end of nv_stru_gas.h */
