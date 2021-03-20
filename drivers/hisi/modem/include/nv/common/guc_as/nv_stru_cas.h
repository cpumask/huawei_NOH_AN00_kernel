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

#ifndef __NV_STRU_CAS_H__
#define __NV_STRU_CAS_H__


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "PsTypeDef.h"
#include "nv_id_cas.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : CPROC_1X_NVIM_DM_THRESHOLD_STRU
 Э����  :
 ASN.1���� :
�ṹ˵��  : ��NV���ڴ洢cproc_1x�Էּ����߿��Ƶ���ز�����
*****************************************************************************/
typedef struct
{
    /*
     * ��̬�ּ����߿��ƹ��ܿ��ء�
     * 0���رգ���̬�ּ��ر�ʱ���ּ����ز���Ҫ��̬���ƣ�һֱ���ڴ�״̬��
     * 1����
     */
    VOS_UINT8                           ucDiversitySwitch;
    /*
     * ����̬PCH CRCУ��ֵ�������޳�ʼֵ��
     * ��CRCУ��������ȷ�����ﵽ��ֵ���ּ����߽��رա�
     * ��λ����
     */
    VOS_UINT8                           ucIdleCrcInitCounter;
    /*
     * SIM���¶ȸ��ڸ�ֵʱ���ּ����߽��رգ����ڸ�ֵ������򿪡�
     * ��λ��0.1��
     */
    VOS_UINT16                          usSimCardTemperature;
    /*
     * ����̬RSCP����ֵ���ڸ�������򿪷ּ����ߡ�
     * ��λ��1/8 dBm
     */
    VOS_INT16                           shwIdleMeasRscpOn;
    /*
     * ����̬RSCP����ֵ���ڸ�����������رշּ����ߡ�
     * ��λ��1/8 dBm
     */
    VOS_INT16                           shwIdleMeasRscpOff;
    /*
     * ����̬EcN0����ֵ���ڸ�������򿪷ּ����ߡ�
     * ��λ��1/8dB
     */
    VOS_INT16                           shwIdleMeasEcN0On;
    /*
     * ����̬EcN0����ֵ���ڸ�����������رշּ����ߡ�
     * ��λ��1/8dB
     */
    VOS_INT16                           shwIdleMeasEcN0Off;
    /*
     * TCH̬CSҵ��FER��֡�������ޣ���ͳ�ƴ�
     * ��λ����
     */
    VOS_UINT8                           ucTchCSFerOnCounter;
    /*
     * TCH̬CSҵ��FER��֡�������ޣ���ͳ�ƴ�
     * ��λ����
     */
    VOS_UINT8                           ucTchCSFerOffCounter;
    /*
     * TCH̬CSҵ�񿪷ּ�����֡����ͳ�ƴ�����ͳ�ƴ��ڴﵽ����
     * ��λ����
     */
    VOS_UINT8                           ucTchCSFerOnWinSize;
    /*
     * TCH̬CSҵ��طּ�����֡����ͳ�ƴ�����ͳ�ƴ���δ�ﵽ����
     * ��λ����
     */
    VOS_UINT8                           ucTchCSFerOffWinSize;
}CPROC_1X_NVIM_DM_THRESHOLD_STRU;


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/


#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

/* end of nv_stru_cas.h */

