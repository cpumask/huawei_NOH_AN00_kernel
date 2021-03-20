/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 * * This program is free software; you can redistribute it andor modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 * Otherwise, the following license terms apply:
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation andor other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
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
 */

#ifndef __NV_STRU_WAS_H__
#define __NV_STRU_WAS_H__

#include "ps_type_def.h"
#include "nv_id_guas.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 Include Headfile */
#pragma pack(push, 4)

/* 2 Macro */
#define WAS_NV_PTL_VER_R3                       (0)                             /* WAS_PTL_VER_R3 */
#define WAS_NV_PTL_VER_R4                       (1)                             /* WAS_PTL_VER_R4 */
#define WAS_NV_PTL_VER_R5                       (2)                             /* WAS_PTL_VER_R5 */
#define WAS_NV_PTL_VER_R6                       (3)                             /* WAS_PTL_VER_R6 */
#define WAS_NV_PTL_VER_R7                       (4)                             /* WAS_PTL_VER_R7 */
#define WAS_NV_PTL_VER_R8                       (5)                             /* WAS_PTL_VER_R8 */
#define WAS_NV_PTL_VER_R9                       (6)                             /* WAS_PTL_VER_R9 */

/*
 * ö����    : WAS_TX_RX_FREQ_SEPARAT_ENUM_UINT8
 * Э����  :
 * ASN.1���� :
 * ö��˵��  :
 */
enum WAS_TxRxFreqSeparat
{
    WAS_TX_RX_FREQ_SEPARAT_DEFAULT_TX_RX_SEPARATION = 0,
    WAS_TX_RX_FREQ_SEPARAT_SPARE2,
    WAS_TX_RX_FREQ_SEPARAT_SPARE1,
    WAS_TX_RX_FREQ_SEPARAT_BUTT
} ;
typedef VOS_UINT8 WAS_TxRxFreqSeparatUint8;

/*
 * ö����    : WAS_DL_SIMUL_HS_DSCH_CFG_ENUM_UINT8
 * Э����  :
 * ASN.1���� :
 * ö��˵��  :
 */
enum  WAS_DlSimulHsDschCfg
{
    WAS_DL_SIMUL_HS_DSCH_CFG_KBPS32 = 0,
    WAS_DL_SIMUL_HS_DSCH_CFG_KBPS64,
    WAS_DL_SIMUL_HS_DSCH_CFG_KBPS128,
    WAS_DL_SIMUL_HS_DSCH_CFG_KBPS384,
    WAS_DL_SIMUL_HS_DSCH_CFG_BUTT
};
typedef VOS_UINT8 WAS_DlSimulHsDschCfgUint8;

/*
 * ö����    : WAS_PtlVerTypeUint8
 * Э����  : 10.2.39    RRC CONNECTION REQUEST
 * ASN.1���� : AccessStratumReleaseIndicator
 * ö��˵��  : Э��汾
 *            Absence of the IE implies R3.
 *            The IE also indicates the release of the RRC transfer syntax
 *            supported by the UE 13 spare values are needed
 */
enum WAS_PtlVerType
{
    WAS_PTL_VER_ENUM_R3                 = WAS_NV_PTL_VER_R3, /* _H2ASN_Replace  WAS_NV_PTL_VER_R3 = 0 */
    WAS_PTL_VER_ENUM_R4                 = WAS_NV_PTL_VER_R4, /* _H2ASN_Replace  WAS_NV_PTL_VER_R4 = 1 */
    WAS_PTL_VER_ENUM_R5                 = WAS_NV_PTL_VER_R5, /* _H2ASN_Replace  WAS_NV_PTL_VER_R5 = 2 */
    WAS_PTL_VER_ENUM_R6                 = WAS_NV_PTL_VER_R6, /* _H2ASN_Replace  WAS_NV_PTL_VER_R6 = 3 */
    WAS_PTL_VER_ENUM_R7                 = WAS_NV_PTL_VER_R7, /* _H2ASN_Replace  WAS_NV_PTL_VER_R7 = 4 */
    WAS_PTL_VER_ENUM_R8                 = WAS_NV_PTL_VER_R8, /* _H2ASN_Replace  WAS_NV_PTL_VER_R8 = 5 */
    WAS_PTL_VER_ENUM_R9                 = WAS_NV_PTL_VER_R9, /* _H2ASN_Replace  WAS_NV_PTL_VER_R9 = 6 */
    WAS_PTL_VER_ENUM_BUTT
};
typedef VOS_UINT8 WAS_PtlVerTypeUint8;

/*
 * �ṹ��    : WAS_RF_CAPA_STRU
 * Э����  : 10.3.3.33 RF capability FDD
 * ASN.1���� :
 * �ṹ˵��  : RF������Ϣ
 */
typedef struct
{
    VOS_UINT8                      powerClass;                       /* UE���ʼ��� */
    WAS_TxRxFreqSeparatUint8       txRxFreqSeparate;                 /* Tx/Rx Ƶ������ */
    VOS_UINT8                      rsv[2];                     /* 4�ֽڶ��룬���� */
}WAS_RfCapa;

/*
 * �ṹ��    : WAS_NVIM_UE_CAPA_CUSTOMED_STRU
 * �ṹ˵��  : ��־WCDMA����������
 */
typedef struct
{
    /*
     * �����
     * 0��δ�����DPA��UPA��֧�֣�
     * 1������Ƿ�֧��DPA��UPA������NV����ơ�
     * ע�⣺enEDCHSupport��ucHSDSCHPhyCategory��enHSDSCHSupport��3��NV�ܴ˼���NV����ơ�
     * ��ulHspaStatus=1ʱ������3��NV���ò���Ч�������û��������ϱ������ȼ������򰴲�ƷĬ��ֵ�ϱ���
     */
    VOS_UINT32                              hspaStatus;

    WAS_RfCapa                              rfCapa; /* RF������Ϣ����μ�WAS_RF_CAPA_STRU�� */

    /*
     * 16QAM������أ��Ƿ�֧��E-DPCCH Power Boosting��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            supportPwrBoosting;
    PS_BoolUint8                            rsv1; /* ����λ */

    /*
     * ��HSDSCHͬʱ���е�DCH���ʡ�
     * 0��32kbit/s��
     * 1��64kbit/s��
     * 2��128kbit/s��
     * 3��384kbit/s��
     */
    WAS_DlSimulHsDschCfgUint8               dlSimulHsDschCfg;
    /*
     * UE W�����֧�ֵ�Э��汾��
     * 0��R3��
     * 1��R4��
     * 2��R5��
     * 3��R6��
     * 4��R7��
     * 5��R8��
     * 6��R9��
     */
    WAS_PtlVerTypeUint8                     asRelIndicator;

    /*
     * 0����֧��HS-DSCH��
     * 1��֧��HS-DSCH��
     * ��ulHspaStatus = 1ʱ���������Ч��
     */
    PS_BoolUint8                            hsDschSupport;
    /*
     * ����HS-DSCH category�����ȼ���
     * 6��֧������Ϊ3.6 Mbits��
     * 8��֧������Ϊ7.2 Mbits��
     * 10��֧������Ϊ14.4 Mbits��
     * ע�⣺Э��汾ΪR7������ʱ����������������ucHSDSCHPhyCategoryextΪ׼��������HSDPA��category<13���轫
     * ucHSDSCHPhyCategoryext��Ϊ0��
     * ��ֵӦ�ô��ڵ���1����С�ڵ���64����������˷�Χ��ʹ��ֵ6��
     */
    VOS_UINT8                               hsDschPhyCategory;

    /*
     * 0����֧��MAC-ehs��
     * 1��֧��MAC-ehs��
     */
    PS_BoolUint8                            macEhsSupport;
    /*
     * ����HS-DSCH category�����ȼ���չ��
     * ȡֵ��Χ12~20��
     * ����ֵ��14�������������MIMO����ʱ����Ҫ����NV��enMimoSingleStreamStrict�����Ƿ�����ֻ��ʹ�õ���MIMO��
     * ����ֵ=17����18����С��֧��64QAM�������������MIMO����ʱ����Ҫ����NV��enMimoSingleStreamStrict�����Ƿ�����ֻ��ʹ
     * �õ���MIMO��
     */
    VOS_UINT8                               hsDschPhyCategoryExt;
    /*
     * R8���ԣ��Ƿ�֧��DC�����Ը��ݲ�Ʒ���ƣ���
     * 0����֧��Multi Cell��
     * 1��֧��Multi Cell��
     */
    PS_BoolUint8                            multiCellSupport;
    /*
     * ����HS-DSCH category�����ȼ���չ2��enMultiCellSupportΪ1ʱ���������Ч��
     * ȡֵ��Χ21~24������MutiCell��֧�֡�
     */
    VOS_UINT8                               hsDschPhyCategoryExt2;

    /*
     * DC������أ��Ƿ�֧��DCС��ָ������ּ���
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            cellSpecTxDiversityForDc;
    PS_BoolUint8                            rsv2; /* ����λ */
    /*
     * 0����֧��HSUPA���ܣ�
     * 1��֧��HSUPA���ܡ�
     */
    PS_BoolUint8                            edchSupport;
    /*
     * ֧��UPA�ĵȼ���
     * 1��֧������Ϊ0.7296Mbit/s��
     * 2��֧������Ϊ1.4592Mbit/s��
     * 3��֧������Ϊ1.4592Mbit/s��
     * 4��֧������Ϊ2.9185Mbit/s��
     * 5��֧������Ϊ2Mbit/s��
     * 6��֧������Ϊ5.76Mbit/s��
     * ֧��DC UPA����ʱucEDCHPhyCategoryExt2ֵΪ9��
     */
    VOS_UINT8                               edchPhyCategory;
    /*
     * R7���ԣ��Ƿ�֧������16QAM����enSuppUl16QAM=1ʱucEDCHPhyCategoryExt����Ч��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            suppUl16QAM;
    /*
     * ֧��UPA�ĵȼ�����չ�
     * ���е��ز�֧��16QAMʱ����д7����д����ֵ��Ч��
     */
    VOS_UINT8                               edchPhyCategoryExt;
    /*
     * 16QAM������أ��Ƿ�֧��E-DPDCH power interpolation formula�������ڲ巽ʽ����
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            suppEDpdchInterpolationFormula;
    PS_BoolUint8                            rsv3; /* ����λ */
    PS_BoolUint8                            rsv4; /* ����λ */

    /*
     * R8������ǿL2���ԣ��Ƿ�֧��MAC_I/MAC_Is��
     * 0����֧�֣�
     * 1��֧�֡�
     * ֻ�е�UE֧��UPA(enEDCHSupport = 1)������MAC-ehs(enMacEhsSupport = 1)ʱ����֧��MAC-i��
     */
    PS_BoolUint8                            macIsSupport;

    PS_BoolUint8                            rsv5; /* ����λ */

    PS_BoolUint8                            rsv6; /* ����λ */
    PS_BoolUint8                            rsv7; /* ����λ */

    /*
     * R8 DC���ԣ��Ƿ�֧������ѹģ������½�����Ƶ������
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            adjFreqMeasWithoutCmprMode;

    /*
     * MIMO������أ��Ƿ�֧������ֻ��ʹ�õ���MIMO��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            mimoSingleStreamStrict;
    /*
     * MIMO������أ�MIMO����ʱ�����п����ŵ��Ƿ�֧������ʹ�÷ּ���
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            mimoWithDlTxDiversity;

    /*
     * �Ƿ�֧��UTRA�е����ȼ���ѡ��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            sptAbsPriBasedReselInUtra;

    /*
     * R9���ԣ�HS-DSCH�����ȼ���չ��3��
     * ȡֵ��Χ��25~28��������Χʱ��֧��DC+MIMO��
     */
    VOS_UINT8                               hsDschPhyCategoryExt3;
    /*
     * R9���ԣ��Ƿ�֧��DC+MIMO��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            dcMimoSupport;
    /*
     * R8���ԣ��Ƿ�֧������ʹ�ù���E-DCH�ŵ�(E-RACH��������)��
     * ֻ�е�֧��������ǿFACH��������ǿFACH��MAC-Iʱ���������Ч����enMacEhsSupport=1��enEDCHSupport=1��
     * enSuppHsdpaInFach=1ʱ������NV����Ч��
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            suppCommEDCH;

    /*
     * R9���ԣ��Ƿ�֧��DC UPA��
     * 0����֧�֣�
     * 1��֧�֡�
     * ���ucEDCHPhyCategoryExt2С��8���ߴ���9������ucEDCHPhyCategory������6�����NV����Ч������֧��DC UPA���ԡ�
     */
    PS_BoolUint8                            dcUpaSupport;
    /*
     * EDCH category extension 2��
     * �����֧��DC UPA����NV���Ч��
     * ���֧��DC UPA��enSuppUl16QAM=0������ֵΪ8��
     */
    VOS_UINT8                               edchPhyCategoryExt2;
    /*
     * E-DPDCH���ʻ�������ʹ�ܱ�־���µ綨�ơ�
     * 0����֧�֣�
     * 1��֧�֡�
     */
    PS_BoolUint8                            edpdchGainFactorFlg;
    PS_BoolUint8                            rsv8; /* ����λ */
    PS_BoolUint8                            rsv9; /* ����λ */
    VOS_UINT8                               rsv10[5];
}WAS_NVIM_UeCapaCustomed;

/*
 * �ṹ��    : WAS_NVIM_UE_CAPA_STRU
 * �ṹ˵��  : �洢��NVIM�е�UE������Ϣ
 */
typedef struct
{
    VOS_UINT32                              hspaStatus; /* 0��ʾδ����,��ôDPA��UPA��֧��;1��ʾ���� */

    WAS_RfCapa                              rfCapa;                           /* RF ������Ϣ */

    PS_BoolUint8                            supportPwrBoosting; /* 16QAM������أ��Ƿ�֧��E-DPCCH Power Boosting */
    PS_BoolUint8                            sf4Support;                       /* �Ƿ�֧��ul dpcch ʹ�� slotFormat4 */

    WAS_DlSimulHsDschCfgUint8               dlSimulHsDschCfg;                 /* ENUMERATED  OPTIONAL */
    WAS_PtlVerTypeUint8                     asRelIndicator;                   /* Access Stratum Release Indicator */

    PS_BoolUint8                            hsDschSupport;                    /* �Ƿ�֧��enHSDSCHSupport�ı�־ */
    VOS_UINT8                               hsDschPhyCategory;                /* ֧��HS-DSCH���������ͱ�־ */

    PS_BoolUint8                            macEhsSupport;
    VOS_UINT8                               hsDschPhyCategoryExt;
    /* �Ƿ�֧�� Multi cell support,���֧��MultiCell,Ex2���� */
    PS_BoolUint8                            multiCellSupport;
    VOS_UINT8                               hsDschPhyCategoryExt2; /* HS-DSCH physical layer category extension 2 */
    /*
     * This IE is optionally present if Dual-Cell HSDPA is supported. Otherwise it is not needed.
     * The IE is not needed in the INTER RAT HANDOVER INFO message. Otherwise, it is optional
     */
    PS_BoolUint8                            cellSpecTxDiversityForDc;
    PS_BoolUint8                            efdpchSupport; /* �Ƿ�֧��E-FDPCH�ı�־,FDPCH֧��ʱ��NV����Ч */
    PS_BoolUint8                            edchSupport;                      /* �Ƿ�֧��EDCH�ı�־ */
    VOS_UINT8                               edchPhyCategory;                  /* ֧��UPA�ĵȼ� */
    PS_BoolUint8                            suppUl16QAM; /* �Ƿ�֧������16QAM����֧��ʱucEDCHPhyCategoryExt����Ч */
    VOS_UINT8                               edchPhyCategoryExt;               /* ���е��ز�֧��16QAMʱ����д7 */
    /* 16QAM������أ��Ƿ�֧��E-DPDCH power interpolation formula */
    PS_BoolUint8                            suppEDpdchInterpolationFormula;
    PS_BoolUint8                            suppHsdpaInFach;                  /* ֧��CELL_FACH��HS-DSCH�Ľ��� */
    PS_BoolUint8                            suppHsdpaInPch; /* ֧��CELL_PCH��URA_PCH��HS-DSCH�Ľ��� */

    PS_BoolUint8                            macIsSupport;                     /* �Ƿ�֧��MAC_I/MAC_Is */

    PS_BoolUint8                            fdpchSupport;                     /* �Ƿ�֧��FDPCH�ı�־ */

    PS_BoolUint8                            hsscchLessSupport;                /* �Ƿ�֧�� hsscchlessHsdschOperation */
    PS_BoolUint8                            ulDpcchDtxSupport; /* �Ƿ�֧�� discontinuousDpcchTransmission */

    /* �Ƿ�֧�� Adjacent Frequency measurements without compressed mode */
    PS_BoolUint8                            adjFreqMeasWithoutCmprMode;

    PS_BoolUint8                            mimoSingleStreamStrict;           /* �Ƿ�����ֻ��ʹ�õ���MIMO */
    /* R9���ԣ���MIMO����ʱ�����п����ŵ��Ƿ�����ʹ�÷ּ� */
    PS_BoolUint8                            mimoWithDlTxDiversity;

    /* ֧��UTRA�е����ȼ���ѡ��Ĭ��Ϊ0��1Ϊ֧�֣�0Ϊ��֧�� */
    PS_BoolUint8                            sptAbsPriBasedReselInUtra;

    VOS_UINT8                               hsDschPhyCategoryExt3; /* HS-DSCH physical layer category extension 3 */
    PS_BoolUint8                            dcMimoSupport;                    /* �Ƿ�֧��DC+MIMO */
    PS_BoolUint8                            suppCommEDCH;                     /* E-RACH�������� */

    PS_BoolUint8                            dcUpaSupport;                     /* �Ƿ�֧��DC UPA�ı�־ */
    VOS_UINT8                               edchPhyCategoryExt2;              /* EDCH  category extension 2 */
    PS_BoolUint8                            edpdchGainFactorFlg;              /* E-DPDCH���ʻ�������ʹ�ܱ�־λ */
    PS_BoolUint8                            ho2EutranUnSupportFlg;            /* �Ƿ�֧�ֵ�L��HO */
    PS_BoolUint8                            eutranMeasUnSupportFlg;           /* �Ƿ�֧�ֵ�����̬L�Ĳ��� */
    VOS_UINT8                               rsv[5];
}WAS_NVIM_UeCapa;

#pragma pack(pop)

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

/* end of nv_stru_was.h */

