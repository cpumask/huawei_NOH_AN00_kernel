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

#ifndef FC_INTERFACE_H
#define FC_INTERFACE_H

#include "vos.h"
#include "PsTypeDef.h"
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#include "acore_nv_stru_gucttf.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/* ���ز������룬��Ӧλ���ã���ʾĳ�ֲ��� */
#define FC_POLICY_MASK_MEM (((VOS_UINT32)1) << FC_POLICY_ID_MEM)
#define FC_POLICY_MASK_CPU_A (((VOS_UINT32)1) << FC_POLICY_ID_CPU_A)
#define FC_POLICY_MASK_CDS (((VOS_UINT32)1) << FC_POLICY_ID_CDS)
#define FC_POLICY_MASK_CST (((VOS_UINT32)1) << FC_POLICY_ID_CST)
#define FC_POLICY_MASK_GPRS (((VOS_UINT32)1) << FC_POLICY_ID_GPRS)
#define FC_POLICY_MASK_TMP (((VOS_UINT32)1) << FC_POLICY_ID_TMP)
#define FC_POLICY_MASK_CPU_C (((VOS_UINT32)1) << FC_POLICY_ID_CPU_C)
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
#define FC_POLICY_MASK_CDMA (((VOS_UINT32)1) << FC_POLICY_ID_CDMA)
#endif

/* ���ز���ID */
enum FC_PolicyId {
    FC_POLICY_ID_MEM = 0,
    FC_POLICY_ID_CPU_A,
    FC_POLICY_ID_CDS,
    FC_POLICY_ID_CST,
    FC_POLICY_ID_GPRS,
    FC_POLICY_ID_TMP,
    FC_POLICY_ID_CPU_C,
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    FC_POLICY_ID_CDMA,
#endif
    FC_POLICY_ID_BUTT
};
typedef VOS_UINT8 FC_PolicyIdUint8;

/* �������ȼ����壬��ֵԽ�����ȼ�Խ�ߣ�Խ������ */
enum FC_PriType {
    FC_PRI_NULL = 0, /* �����ڲ�����ʹ�ã��ⲿ��ʹ�� */
    FC_PRI_1,
    FC_PRI_2,
    FC_PRI_3,
    FC_PRI_4,
    FC_PRI_5,
    FC_PRI_6,
    FC_PRI_7,
    FC_PRI_8,
    FC_PRI_9,
    FC_PRI_BUTT
};
typedef VOS_UINT8 FC_PriTypeUint8;

/* ���ص㶨�� */
enum FC_Id {
    FC_ID_BRIDGE_FORWARD_DISACRD = 0, /* ���ţ�E5ʹ�� */
    FC_ID_USB_ETH,                    /* USB������E5ʹ�� */
    FC_ID_WIFI_ETH,                   /* WIFI������E5ʹ�� */
    FC_ID_NIC_1,                      /* NDIS����1��������ͬ */
    FC_ID_NIC_2,
    FC_ID_NIC_3,
    FC_ID_NIC_4,
    FC_ID_NIC_5,
    FC_ID_NIC_6,
    FC_ID_NIC_7,
    FC_ID_NIC_8,
    FC_ID_NIC_9,
    FC_ID_NIC_10,
    FC_ID_NIC_11,
    FC_ID_NIC_12,
    FC_ID_MODEM,             /* Modem */
    FC_ID_DIPC_1,            /* DIPCʹ��ͨ������Ӧ�豸ΪUDI_ACM_HSIC_ACM1_ID */
    FC_ID_DIPC_2,            /* DIPCʹ��ͨ������Ӧ�豸ΪUDI_ACM_HSIC_ACM3_ID */
    FC_ID_DIPC_3,            /* DIPCʹ��ͨ������Ӧ�豸ΪUDI_ACM_HSIC_ACM5_ID */
    FC_ID_UL_RATE_1_FOR_CPU, /* C��CPU����ʹ�õ��������ʿ��ƣ�����ͬ */
    FC_ID_UL_RATE_2_FOR_CPU,
    FC_ID_UL_RATE_3_FOR_CPU,
    FC_ID_UL_RATE_4_FOR_CPU,
    FC_ID_UL_RATE_5_FOR_CPU,
    FC_ID_UL_RATE_6_FOR_CPU,
    FC_ID_UL_RATE_7_FOR_CPU,
    FC_ID_UL_RATE_8_FOR_CPU,
    FC_ID_UL_RATE_9_FOR_CPU,
    FC_ID_UL_RATE_10_FOR_CPU,
    FC_ID_UL_RATE_11_FOR_CPU,
    FC_ID_UL_RATE_1_FOR_TMP, /* C���±�����ʹ�õ��������ʿ��ƣ�����ͬ */
    FC_ID_UL_RATE_2_FOR_TMP,
    FC_ID_UL_RATE_3_FOR_TMP,
    FC_ID_UL_RATE_4_FOR_TMP,
    FC_ID_UL_RATE_5_FOR_TMP,
    FC_ID_UL_RATE_6_FOR_TMP,
    FC_ID_UL_RATE_7_FOR_TMP,
    FC_ID_UL_RATE_8_FOR_TMP,
    FC_ID_UL_RATE_9_FOR_TMP,
    FC_ID_UL_RATE_10_FOR_TMP,
    FC_ID_UL_RATE_11_FOR_TMP,
    FC_ID_BUTT
};
typedef VOS_UINT8 FC_IdUint8;

/* �ڴ�����ʹ�õ����ȼ� */
#define FC_PRI_FOR_MEM_LEV_1 (FC_PRI_1)
#define FC_PRI_FOR_MEM_LEV_2 (FC_PRI_2)
#define FC_PRI_FOR_MEM_LEV_3 (FC_PRI_3)
#define FC_PRI_FOR_MEM_LEV_4 (FC_PRI_4)

/* �������ص����ȼ� */
#define FC_PRI_FOR_BRIDGE_FORWARD_DISCARD (FC_PRI_FOR_MEM_LEV_1)

/* LTE PDN/GU QoSʹ�õ����ȼ� */
#define FC_PRI_FOR_PDN_LOWEST (FC_PRI_FOR_MEM_LEV_2)
#define FC_PRI_FOR_PDN_NONGBR (FC_PRI_FOR_MEM_LEV_3)
#define FC_PRI_FOR_PDN_GBR (FC_PRI_FOR_MEM_LEV_4)

/* ���غͽ�����غ��� */
typedef VOS_UINT32 (*FC_SetFunc)(VOS_UINT32 param1, VOS_UINT32 param2);
typedef VOS_UINT32 (*FC_ClrFunc)(VOS_UINT32 param1, VOS_UINT32 param2);
typedef VOS_UINT32 (*FC_RstFunc)(VOS_UINT32 param1, VOS_UINT32 param2);

typedef struct {
    ModemIdUint16    modemId;
    FC_PolicyIdUint8 policyId; /* �����ص�Ӱ�쵽�����ز��� */
    FC_IdUint8       fcId;
    FC_PriTypeUint8      fcPri; /* �����ص��ڸ����ز���������ȼ� */
    VOS_UINT8        rsv[3];
    VOS_UINT32       param1;                     /* ���������ص�ʹ�ã��������غͽ�����غ���ʱ����Ϊ������� */
    VOS_UINT32       param2;                     /* ���������ص�ʹ�ã��������غͽ�����غ���ʱ����Ϊ������� */
    FC_SetFunc      setFunc; /* ���غ��� */         /* _H2ASN_Replace VOS_UINT32  setFunc1; */
    FC_ClrFunc      clrFunc; /* �����غ��� */        /* _H2ASN_Replace VOS_UINT32  clrFunc1; */
    FC_RstFunc      rstFunc; /* �����ĸ�λ������ָ��ӿ� */ /* _H2ASN_Replace VOS_UINT32  rstFunc1; */
} FC_RegPointPara;

#if  defined(MODEM_FUSION_VERSION) && (VOS_OS_VER != VOS_WIN32)
/* fusion�汾��֧��FC����׮�ӿ� */
static inline VOS_UINT32 FC_RegPoint(FC_RegPointPara* fcRegPoint) { return VOS_OK; }
static inline VOS_UINT32 FC_DeRegPoint(FC_IdUint8 fcId, ModemIdUint16 modemId) { return VOS_OK; }
static inline VOS_UINT32 FC_ChangePoint(FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 pri, ModemIdUint16 modemId) { return VOS_OK; }
static inline  VOS_VOID FC_ChannelMapCreate(FC_IdUint8 fcId, VOS_UINT8 rabId, ModemIdUint16 modemId) {}
static inline  VOS_VOID FC_ChannelMapDelete(VOS_UINT8 rabId, ModemIdUint16 modemId) {}
static inline  VOS_VOID RITF_SetFlowLev(VOS_RatModeUint32 rateMode) { }

#else
 
extern VOS_UINT32 FC_RegPoint(FC_RegPointPara* fcRegPoint);

extern VOS_UINT32 FC_DeRegPoint(FC_IdUint8 fcId, ModemIdUint16 modemId);

extern VOS_UINT32 FC_ChangePoint(FC_IdUint8 fcId, FC_PolicyIdUint8 policyId, FC_PriTypeUint8 pri,
    ModemIdUint16 modemId);

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
extern VOS_VOID FC_ChannelMapCreate(FC_IdUint8 fcId, VOS_UINT8 rabId, ModemIdUint16 modemId);

extern VOS_VOID FC_ChannelMapDelete(VOS_UINT8 rabId, ModemIdUint16 modemId);

#if (VOS_OSA_CPU == OSA_CPU_ACPU)
typedef VOS_UINT32 (*FC_AcoreDrvAssembleParaFunc)(FC_DrvAssemPara* fcDrvAssemPara);
#endif
#endif

/* ֻ��C CORE�ṩ�ӿ� */
extern VOS_UINT32 FC_GetCpuLoadRecord(VOS_VOID);
extern VOS_VOID RITF_SetFlowLev(VOS_RatModeUint32 rateMode);

#endif

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
