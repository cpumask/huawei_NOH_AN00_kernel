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

#ifndef __ACORE_NV_STRU_GUCTTF_H__
#define __ACORE_NV_STRU_GUCTTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "vos.h"
#include "acore_nv_id_gucttf.h"

#pragma pack(push, 4)

#define FC_ACPU_DRV_ASSEM_NV_LEV 4

enum FC_AcpuDrvAssemLev {
    FC_ACPU_DRV_ASSEM_LEV_1    = 0,
    FC_ACPU_DRV_ASSEM_LEV_2    = 1,
    FC_ACPU_DRV_ASSEM_LEV_3    = 2,
    FC_ACPU_DRV_ASSEM_LEV_4    = 3,
    FC_ACPU_DRV_ASSEM_LEV_5    = 4,
    FC_ACPU_DRV_ASSEM_LEV_BUTT = 5
};
typedef VOS_UINT32 FC_AcpuDrvAssemLevUint32;

enum FC_MemThresholdLev {
    FC_MEM_THRESHOLD_LEV_1    = 0,
    FC_MEM_THRESHOLD_LEV_2    = 1,
    FC_MEM_THRESHOLD_LEV_3    = 2,
    FC_MEM_THRESHOLD_LEV_4    = 3,
    FC_MEM_THRESHOLD_LEV_5    = 4,
    FC_MEM_THRESHOLD_LEV_6    = 5,
    FC_MEM_THRESHOLD_LEV_7    = 6,
    FC_MEM_THRESHOLD_LEV_8    = 7,
    FC_MEM_THRESHOLD_LEV_BUTT = 8
};
typedef VOS_UINT32 FC_MemThresholdLevUint32;

enum TTF_AcoreBool {
    TTF_ACORE_FALSE = 0,
    TTF_ACORE_TRUE  = 1,

    TTF_ACORE_BOOL_BUTT
};
typedef VOS_UINT8 TTF_AcoreBoolUint8;

/* ����CPU LOAD��̬��������������� */
typedef struct {
    VOS_UINT8 hostOutTimeout; /* PC�������ʱ�䣬ȡֵ��Χ0~50����λus�� */
    VOS_UINT8 ethTxMinNum;    /* �����������������ȡֵ��Χ1~100�� */
    VOS_UINT8 ethTxTimeout;   /* �������������ʱʱ�䣬ȡֵ��Χ1~20����λms�� */
    VOS_UINT8 ethRxMinNum;    /* �����������������ȡֵ��Χ1~20�� */
    VOS_UINT8 ethRxTimeout;   /* �������������ʱʱ�䣬ȡֵ��Χ1~50����λms�� */
    VOS_UINT8 cdsGuDlThres;   /* �ò����Ѿ������� */
    VOS_UINT8 rsv[2];         /* 4�ֽڶ��룬Ԥ���� */
} FC_DrvAssemPara;

/* ����CPU LOAD��̬��������������� */
typedef struct {
    VOS_UINT32      cpuLoad;      /* CPU���أ�ȡֵ��Χ0~100�� */
    FC_DrvAssemPara drvAssemPara; /* ����cpuLoad��̬��������������� */
} FC_CpuDrvAssemPara;

/* FC_AcoreCfgCpu�ṹ,CPU���ص����޺�����ֵ */
typedef struct {
    VOS_UINT32 cpuOverLoadVal;  /* CPU������ˮ�ߣ�ȡֵ��Χ[1,100]����ʾCPUռ���ʴ�С�� */
    VOS_UINT32 cpuUnderLoadVal; /* CPUͣ����ˮ�ߣ�ȡֵ��Χ[1,cpuOverLoadVal]����ʾCPUռ���ʴ�С�� */
    VOS_UINT32 smoothTimerLen;  /* CPU��������ƽ��������ȡֵ��Χ[2,1000]����λ��CPU������ڡ� */
    VOS_UINT32 stopAttemptTimerLen; /* CPU���شﵽ��߼���������ʱ�����Խ����أ��ö�ʱ����ʱ������λ���룬ȡֵ��Χ���ڵ���0��0��ʾ��ʹ�á� */
    VOS_UINT32 umUlRateThreshold; /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0�� */
    VOS_UINT32 umDlRateThreshold; /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0�� */
    VOS_UINT32 rmRateThreshold;   /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0�� */
} FC_AcoreCfgCpu;

/* FC_AcoreCfgMemThreshold�ṹ,MEM���ص����޺�����ֵ */
typedef struct {
    VOS_UINT32 setThreshold;  /* �ڴ�������ˮ�ߡ� */
    VOS_UINT32 stopThreshold; /* �ڴ�ͣ����ˮ�ߡ� */
} FC_AcoreCfgMemThreshold;

/* FC_CfgMemThresholdCst�ṹ,MEM���ص����޺�����ֵ */
typedef struct {
    VOS_UINT32 setThreshold;                     /* Range:[0,4096], ������������ ��λ�ֽ� */
    VOS_UINT32 stopThreshold;                    /* Range:[0,4096], ֹͣ�������� ��λ�ֽ� */
} FC_CfgMemThresholdCst;

/*
 * ��ӦNV_ITEM_LINUX_CPU_MONITOR_TIMER_LEN��Ľṹ,A��CPUռ���ʼ������ʱ���ʱ��
 * �ṹ˵��  : A��CPUռ���ʼ������ʱ���ʱ����
 * ��A��CPU�����ϱ�CPU���أ����Ը���A������ģ��CPU������ڵ���Ҫ��������ʱ����
 * ע�⣺
 * ��NV���в���δ��Balongͬ�⣬��ֹ�޸ģ�
 */
typedef struct {
    VOS_UINT32 monitorTimerLen; /* A��CPUռ���ʼ������ʱ���ʱ������λ�����룬ȡֵ��Χ100-1000�� */
} CPULOAD_Cfg;

/*
 * ��ӦNV_ITEM_FC_ACPU_DRV_ASSEMBLE_PARA��Ľṹ������CPU LOAD��̬���������������
 * �ṹ˵��  : ����CPU LOAD��̬�����������������
 */
typedef struct {
    /*
     * ʹ�ܿ��ƣ�ֵ0��ʾ��ʹ�ܣ�ֵ1��ʾʹ�ܡ�
     * Bit0������ʹ�ܣ�
     * Bit1��PCʹ�ܡ�
     */
    VOS_UINT8          enableMask;
    VOS_UINT8          smoothCntUpLev;                            /* ���ϵ���ƽ��ϵ����ֵ��Ҫ��0�� */
    VOS_UINT8          smoothCntDownLev;                          /* ���µ���ƽ��ϵ����ֵ��Ҫ��0�� */
    VOS_UINT8          rsv;                                       /* ������ */
    FC_CpuDrvAssemPara cpuDrvAssemPara[FC_ACPU_DRV_ASSEM_NV_LEV]; /* CPU���ò��� */
} FC_CpuDrvAssemParaNv;

/*
 * ��ӦNV_ITEM_NETFILTER_HOOK_MASK��Ľṹ,���ù������NV��
 * �ṹ˵��  : ���ù������NV�Ԥ��5��������ϣ�netfilterPara1-netfilterPara5��ÿ������ȡֵ��ΧΪ0-FFFFFFFF,
 * ��������λΪ1����������λ��Ӧ�Ĺ��Ӻ������ܻᱻע�ᵽ�ں���
 */
typedef struct {
    VOS_UINT32 netfilterPara1; /* ���Ӻ����������1����Ӧ�������ź�ARP�Ĺ��Ӻ���������������ʱ������ֵ����Ϊ100����û������ʱ������ֵ����Ϊ96�� */
    VOS_UINT32 netfilterPara2; /* ���Ӻ����������2����Ӧ����IPЭ��ջ��ڴ��Ĺ��Ӻ�����ȡֵΪ4224�� */
    VOS_UINT32 netfilterPara3; /* ���Ӻ����������3����Ӧ����IPЭ��ջ���ڴ��Ĺ��Ӻ�����ȡֵΪ8448�� */

    VOS_UINT32 netfilterPara4; /* ���Ӻ����������4����Ӧ�����������صĹ��Ӻ�������������������ʱ������ֵ����Ϊ131072����û����������ʱ������ֵ����Ϊ0�� */
    VOS_UINT32 netfilterPara5; /* ���Ӻ����������5��Ԥ��������ֵ����Ϊ0�� */
} NfExtNv;

/* �ṹ˵��  : CST�������� */
typedef struct {
    FC_AcoreCfgMemThreshold threshold; /* CST���λ����������ˮ�ߣ���λ�ֽڣ��ܳ�4096�� */
} FC_CfgCst;

/* �ṹ˵��  : MEM���ص����޺�����ֵ */
typedef struct {
    VOS_UINT32              thresholdCnt;                         /* A���ڴ����ص�λ������ȡֵ��Χ[0,8]�� */
    FC_AcoreCfgMemThreshold threshold[FC_MEM_THRESHOLD_LEV_BUTT]; /* CST���λ����������ˮ�ߣ���λ�ֽڣ��ܳ�4096�� */
} FC_CfgMem;

/*
 * ��ӦNV_ITEM_ACORE_FLOW_CRTL_CONFIG��Ľṹ
 * �ṹ˵��  : ����A�����ع��ܵ�������Ϣ��
 * ʹ�ã����Ը�����Ҫ�򿪻�ر����ع��ܣ��Լ�����A��CPU���ڴ桢CST������ˮ�ߡ�
 */
typedef struct {
    /*
     * A������ʹ�ܿ��ء�ÿ������λ��0��ʾ��������ȥʹ�ܣ�1��ʾʹ�ܡ�
     * Bit0��MEM���أ�
     * Bit1��A��CPU���أ�
     * Bit2��CDS���أ�
     * Bit3��CST���أ�
     */
    VOS_UINT32     fcEnableMask;
    FC_AcoreCfgCpu fcCfgCpuA; /* A��CPU����ˮ�����á� */
    FC_CfgMem      fcCfgMem;  /* A���ڴ�����ˮ�����á� */
    FC_CfgCst      fcCfgCst;  /* CSTҵ������ˮ������ */
} FC_AcoreCfgNv;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
