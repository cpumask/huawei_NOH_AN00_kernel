/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __LPSCOMMON_H__
#define __LPSCOMMON_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  #pragma pack(*)    �����ֽڶ��뷽ʽ
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 Macro
*****************************************************************************/

/* Msg ID����4����:
-----------------------------------------------------------------
  �ֽڸ�λ                                      �ֽڵ�λ
-----------------------------------------------------------------
| BYTE1  |  BYTE2    |   BYTE3    |    BYTE4   |
-----------------------------------------------------------------
BYTE1 ��Ϣ����:
                ȡֵ�ο�UE_MSG_TYPEö�ٵĶ���

BYTE2 Դģ���:
                ȡֵ��Χ�� UE_FUNC_ModuleId �� UE_APP_MODULE_ID_ENUM�Ķ���


BYTE3 Ŀ��ģ���:
                ȡֵ��ΧͬBYTE2

BYTE4  ��Ϣ��:0~255
*/


/* ����������ϢID�ĺ�:   enMsgType ȡֵ��UE_MSG_TYPE;
   enSrcModuleId �� enDestModuleId��ȡֵ��UE_FUNC_MODULE_ID_ENUM �� UE_APP_ModuleId;*/
#define  DEF_MSG(enMsgType, enSrcModuleId, enDestModuleId, ucNum) \
            (((0xff&enMsgType)<<24)|((0xff&enSrcModuleId)<<16)|((0xff& enDestModuleId)<<8)|ucNum)

/* ��32BIT��ulMsgId��ȡ����Ϣ���͵ĺ�*/
#define GET_MSG_TYPE(ulMsgId)           (((ulMsgId) & 0xff000000 ) >> 24)

/* ��32BIT��ulMsgId��ȡ����Ϣ����Դģ��ID�ĺ�*/
#define GET_SRC_MODULE_ID(ulMsgId)      (((ulMsgId) & 0x00ff0000 ) >> 16)

/* ��32BIT��ulMsgId��ȡ����Ϣ����ģ��ID�ĺ�*/
#define GET_DST_MODULE_ID(ulMsgId)      (((ulMsgId) & 0x0000ff00 ) >> 8)

/* ��32BIT��ulMsgId��ȡ����Ϣ���8λ�ĺ�*/
#define GET_MSG_INDEX(ulMsgId)          ((ulMsgId) & 0x000000ff )



/* APP��Ϣͷ�ĳ��� */
#define APP_OM_HEADER_LEN               12

/*��ϢID�ĳ���*/
#define MSG_ID_LENGTH                   4


/*APP����Ϣͷ*/
#define APP_MSG_HEADER                  VOS_UINT16   usOriginalId;\
                                        VOS_UINT16   usTerminalId;\
                                        VOS_UINT32   ulTimeStamp; \
                                        VOS_UINT32   ulSN;

#define PS_MSG_ID_BASE                  0x0500
#define PS_MSG_ID_END                   0x2000

/* V7R2DT  add rrc dt message base address, 2014/05/12 begin */
/* RRC��·����ϢID�Ǵ�0x8D00-0x8DFF */
#define PS_DT_MSG_ID_BASE                  0x8D00
/* V7R2DT  add rrc dt message base address, 2014/05/12 end */

/* Modem errlog report, relation event lps-om message base address begin */
#define PS_ERR_LOG_MSG_ID_BASE             0x8E00
/* Modem errlog report, relation event lps-om message base address end */

/* boston add begin */


#if (FEATURE_ON == FEATURE_MODEM1_SUPPORT_LTE)
#define LPS_MAX_MODEM_NUM                2
#else
#define LPS_MAX_MODEM_NUM                1
#endif
/* boston add end */

#define NAS_MM_MAX_SHARE_PLMN_NUM       16
/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

#define UE_INVALID_MODULE_ID            0xff  /*��Ч��ģ��ID����*/



/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : LTE_SecuKeyIndex
 �ṹ˵��  :
*****************************************************************************/
enum LTE_SecuKeyIndex
{
    /* ˫LTE��modem0 */
    I0_LTE_SECU_NAS_INTEGRITY_KEY_INDEX                           = 0,
    I0_LTE_SECU_NAS_CIPHER_KEY_INDEX                              = 1,
    I0_LTE_SECU_PDCP_UP_CIPHER_KEY_INDEX                          = 2,
    I0_LTE_SECU_RRC_INTEGRITY_KEY_INDEX                           = 3,
    I0_LTE_SECU_RRC_CIPHER_KEY_INDEX                              = 4,
    I0_LTE_SECU_LASTEST_PDCP_UP_CIPHER_KEY_INDEX                  = 5,
    I0_LTE_SECU_LRRC_KDF_OUTPUT_INDEX                             = 6,
    I0_LTE_SECU_LMM_KDF_OUTPUT_INDEX                              = 7,

    /* ˫LTE��modem1 */
    I1_LTE_SECU_NAS_INTEGRITY_KEY_INDEX                           = 8,
    I1_LTE_SECU_NAS_CIPHER_KEY_INDEX                              = 9,
    I1_LTE_SECU_PDCP_UP_CIPHER_KEY_INDEX                          = 10,
    I1_LTE_SECU_RRC_INTEGRITY_KEY_INDEX                           = 11,
    I1_LTE_SECU_RRC_CIPHER_KEY_INDEX                              = 12,
    I1_LTE_SECU_LASTEST_PDCP_UP_CIPHER_KEY_INDEX                  = 13,
    I1_LTE_SECU_LRRC_KDF_OUTPUT_INDEX                             = 14,
    I1_LTE_SECU_LMM_KDF_OUTPUT_INDEX                              = 15,

    /* modem0 DC support */
    I0_LTE_SECU_SCG_PDCP_UP_CIPHER_KEY_INDEX                      = 16,
    I0_LTE_SECU_SCG_LASTEST_PDCP_UP_CIPHER_KEY_INDEX              = 17,
    I0_LTE_SECU_SCG_LRRC_KDF_OUTPUT_INDEX                         = 18,

    /* modem1 DC support */
    I1_LTE_SECU_SCG_PDCP_UP_CIPHER_KEY_INDEX                      = 19,
    I1_LTE_SECU_SCG_LASTEST_PDCP_UP_CIPHER_KEY_INDEX              = 20,
    I1_LTE_SECU_SCG_LRRC_KDF_OUTPUT_INDEX                         = 21,

    /* begin, add for ENDC, */
    /* modem0 EN-DC support */
    I0_LTE_SECU_NR_SCG_PDCP_UP_CIPHER_KEY_INDEX                      = 22,
    I0_LTE_SECU_NR_SCG_LASTEST_PDCP_UP_CIPHER_KEY_INDEX              = 23,
    I0_LTE_SECU_NR_SCG_LRRC_KDF_OUTPUT_INDEX                         = 24,

    /* modem1 EN-DC support */
    I1_LTE_SECU_NR_SCG_PDCP_UP_CIPHER_KEY_INDEX                      = 25,
    I1_LTE_SECU_NR_SCG_LASTEST_PDCP_UP_CIPHER_KEY_INDEX              = 26,
    I1_LTE_SECU_NR_SCG_LRRC_KDF_OUTPUT_INDEX                         = 27,
    /* end, add for ENDC, */

    LTE_SECU_KEY_BUTT

};
typedef VOS_UINT32 LTE_SecuKeyIndexUint32;

/*****************************************************************************
 �ṹ��    : LPS_SecuKeyIndex
 �ṹ˵��  :
*****************************************************************************/
enum LPS_SecuKeyIndex
{
    /* ˫LTE��modem0 */
    LTE_SECU_NAS_INTEGRITY_KEY_INDEX                        = I0_LTE_SECU_NAS_INTEGRITY_KEY_INDEX,
    LTE_SECU_NAS_CIPHER_KEY_INDEX                           = I0_LTE_SECU_NAS_CIPHER_KEY_INDEX,
    LTE_SECU_LMM_KDF_OUTPUT_INDEX                           = I0_LTE_SECU_LMM_KDF_OUTPUT_INDEX,
    LTE_SECU_KEY_INDEX_BUTT
};
typedef VOS_UINT32 LPS_SecuKeyIndexUint32;

/*****************************************************************************
 �ṹ��    : PS_MsgIdSection
 �ṹ˵��  : ģ���Ľӿ���ϢID��ȡֵ��Χ:
             1�����ֻ�Ǵ�ģ��(RRC/NAS/L2)�ڲ�����Ϣ��
             ����Ҫ��������ģ�齻������Ϣ�����Բ���ʹ�ô˴�����ֵ�����Ե���ȡ
             ֵ(���0��ʼ);
             2������Ǵ�ģ������Ϣ��һ��Ҫ�ڴ˴�ȡֵ��
             3������Ǵ�ģ���ڲ�����Ϣ������Ҫ��PC������ʾ���ⲿ���ڲ���ϢIDҲҪ
             ������ķ�Χ��ȡֵ��
*****************************************************************************/
enum PS_MsgIdSection
{
    /* NAS��RRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_NAS_TO_RRC_BASE   = PS_MSG_ID_BASE,                       /*��NAS����RRC����ϢID����ʼֵ*/

    PS_MSG_ID_RRC_TO_NAS_BASE   = PS_MSG_ID_NAS_TO_RRC_BASE + 0x80,     /*��RRC����NAS����ϢID����ʼֵ*/

    /* L2��RRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_RRC_TO_MAC_BASE   = PS_MSG_ID_RRC_TO_NAS_BASE + 0x80,     /*��RRC����MAC����ϢID����ʼֵ*/
    PS_MSG_ID_MAC_TO_RRC_BASE   = PS_MSG_ID_RRC_TO_MAC_BASE + 0x80,     /*��MAC����RRC����ϢID����ʼֵ*/

    PS_MSG_ID_RRC_TO_RLC_BASE   = PS_MSG_ID_MAC_TO_RRC_BASE + 0x80,
    PS_MSG_ID_RLC_TO_RRC_BASE   = PS_MSG_ID_RRC_TO_RLC_BASE + 0x80,

    PS_MSG_ID_RRC_TO_PDCP_BASE  = PS_MSG_ID_RLC_TO_RRC_BASE + 0x80,
    PS_MSG_ID_PDCP_TO_RRC_BASE  = PS_MSG_ID_RRC_TO_PDCP_BASE + 0x80,

    /*RRC��PHY��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_ERRC_TO_PHY_BASE  = PS_MSG_ID_PDCP_TO_RRC_BASE + 0x40,
    PS_MSG_ID_PHY_TO_ERRC_BASE  = PS_MSG_ID_ERRC_TO_PHY_BASE + 0x40,

    PS_MSG_ID_ERMM_TO_PHY_BASE  = PS_MSG_ID_PHY_TO_ERRC_BASE + 0x40,
    PS_MSG_ID_PHY_TO_ERMM_BASE  = PS_MSG_ID_ERMM_TO_PHY_BASE + 0x40,

    /*L2��PHY��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_UL_MAC_TO_PHY_BASE = PS_MSG_ID_PHY_TO_ERMM_BASE + 0x40,
    PS_MSG_ID_PHY_TO_UL_MAC_BASE = PS_MSG_ID_UL_MAC_TO_PHY_BASE + 0x40,

    PS_MSG_ID_DL_MAC_TO_PHY_BASE = PS_MSG_ID_PHY_TO_UL_MAC_BASE + 0x40,
    PS_MSG_ID_PHY_TO_DL_MAC_BASE = PS_MSG_ID_DL_MAC_TO_PHY_BASE + 0x40,

    /*NAS��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_EMM_TO_APP_BASE   = PS_MSG_ID_PHY_TO_DL_MAC_BASE + 0x80,
    PS_MSG_ID_APP_TO_EMM_BASE   = PS_MSG_ID_EMM_TO_APP_BASE + 0x80,

    PS_MSG_ID_ESM_TO_APP_BASE   = PS_MSG_ID_APP_TO_EMM_BASE+ 0x80,
    PS_MSG_ID_APP_TO_ESM_BASE   = PS_MSG_ID_ESM_TO_APP_BASE+ 0x80,

    /*RRC��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_RRC_TO_APP_BASE   = PS_MSG_ID_APP_TO_ESM_BASE + 0x80,
    PS_MSG_ID_APP_TO_RRC_BASE   = PS_MSG_ID_RRC_TO_APP_BASE + 0x80,

    /*L2��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_L2_TO_APP_BASE    = PS_MSG_ID_APP_TO_RRC_BASE + 0x80,
    PS_MSG_ID_APP_TO_L2_BASE    = PS_MSG_ID_L2_TO_APP_BASE + 0x10,

    /*L2 MAC��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_MAC_TO_APP_BASE    = PS_MSG_ID_APP_TO_L2_BASE + 0x10,
    PS_MSG_ID_APP_TO_MAC_BASE    = PS_MSG_ID_MAC_TO_APP_BASE + 0x30,

    /*L2 RLC��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_RLC_TO_APP_BASE    = PS_MSG_ID_APP_TO_MAC_BASE + 0x30,
    PS_MSG_ID_APP_TO_RLC_BASE    = PS_MSG_ID_RLC_TO_APP_BASE + 0x20,


    /*L2 PDCP��APP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_PDCP_TO_APP_BASE    = PS_MSG_ID_APP_TO_RLC_BASE + 0x20,
    PS_MSG_ID_APP_TO_PDCP_BASE    = PS_MSG_ID_PDCP_TO_APP_BASE + 0x20,


    /*NAS�ڲ��ӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_EMM_TO_ESM_BASE   = PS_MSG_ID_APP_TO_PDCP_BASE + 0x20,
    PS_MSG_ID_ESM_TO_EMM_BASE   = PS_MSG_ID_EMM_TO_ESM_BASE + 0x20,

    PS_MSG_ID_ESM_TO_RABM_BASE  = PS_MSG_ID_ESM_TO_EMM_BASE + 0x20,
    PS_MSG_ID_RABM_TO_ESM_BASE  = PS_MSG_ID_ESM_TO_RABM_BASE + 0x20,

    PS_MSG_ID_RABM_TO_EMM_BASE  = PS_MSG_ID_RABM_TO_ESM_BASE + 0x20,
    PS_MSG_ID_EMM_TO_RABM_BASE  = PS_MSG_ID_RABM_TO_EMM_BASE + 0x20,

    PS_MSG_ID_AIR_BASE           = PS_MSG_ID_EMM_TO_RABM_BASE + 0x20,

    /*NAS��PDCP��Ļ���ģʽ������Ϣ��ȡֵ��Χ*/
    PS_MSG_ID_LTC_TO_LPDCP_BASE    = PS_MSG_ID_AIR_BASE + 0xA0,
    PS_MSG_ID_LPDCP_TO_LTC_BASE    = PS_MSG_ID_LTC_TO_LPDCP_BASE + 0x08,

    /* TC��EMM��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_TC_TO_EMM_BASE    = PS_MSG_ID_LPDCP_TO_LTC_BASE + 0x08,
    PS_MSG_ID_EMM_TO_TC_BASE    = PS_MSG_ID_TC_TO_EMM_BASE + 0x08,

    /* TC��RABM��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_TC_TO_RABM_BASE    = PS_MSG_ID_EMM_TO_TC_BASE + 0x08,
    PS_MSG_ID_RABM_TO_TC_BASE    = PS_MSG_ID_TC_TO_RABM_BASE + 0x08,

    /* NAS��IP��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_NAS_TO_IP_BASE     = PS_MSG_ID_RABM_TO_TC_BASE + 0x08,
    PS_MSG_ID_IP_TO_NAS_BASE     = PS_MSG_ID_NAS_TO_IP_BASE + 0x10,

    /* MMC��LMM��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LMM_TO_MMC_BASE    = PS_MSG_ID_IP_TO_NAS_BASE + 0x10,             /* 0x10B0 */
    PS_MSG_ID_MMC_TO_LMM_BASE    = PS_MSG_ID_LMM_TO_MMC_BASE + 0x80,            /* 0x1130 */

    /* ESM��SM��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_SM_TO_ESM_BASE     = PS_MSG_ID_MMC_TO_LMM_BASE + 0x80,
    PS_MSG_ID_ESM_TO_SM_BASE     = PS_MSG_ID_SM_TO_ESM_BASE + 0x10,

    /* LMM��GMM��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LMM_TO_GMM_BASE    = PS_MSG_ID_ESM_TO_SM_BASE + 0x10,
    PS_MSG_ID_GMM_TO_LMM_BASE    = PS_MSG_ID_LMM_TO_GMM_BASE + 0x20,

    /* LRRC��WRRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LRRC_TO_WRRC_BASE  = PS_MSG_ID_GMM_TO_LMM_BASE + 0x40,
    PS_MSG_ID_WRRC_TO_LRRC_BASE  = PS_MSG_ID_LRRC_TO_WRRC_BASE + 0x40,

    /* LRRC��GRR��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LRRC_TO_GRR_BASE   = PS_MSG_ID_WRRC_TO_LRRC_BASE + 0x40,
    PS_MSG_ID_GRR_TO_LRRC_BASE   = PS_MSG_ID_LRRC_TO_GRR_BASE + 0x40,
    /* PS��LHPA��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LHPA_TO_RRC_BASE     = PS_MSG_ID_GRR_TO_LRRC_BASE + 0x05,

    /* PS OM��APP֮�����Ϣ�ӿڵ�ȡֵ��Χ*/
    PS_MSG_ID_PS_TO_APP_BASE    = PS_MSG_ID_LHPA_TO_RRC_BASE + 0x20,  /*0x1315*/
    PS_MSG_ID_APP_TO_PS_BASE    = PS_MSG_ID_PS_TO_APP_BASE + 0x20,    /*0x1335*/

    /* TDS begin */
    PS_MSG_ID_TDS_PS_BASE    = PS_MSG_ID_APP_TO_PS_BASE + 0x20,
    PS_MSG_ID_TDS_AIR_BASE   = PS_MSG_ID_TDS_PS_BASE + 0x500,
    PS_MSG_ID_TRRC_TO_GRR_BASE = PS_MSG_ID_TDS_AIR_BASE + 0x20,
    PS_MSG_ID_GRR_TO_TRRC_BASE = PS_MSG_ID_TRRC_TO_GRR_BASE + 0x20,

    /* LRRC��TRRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_LRRC_TO_TRRC_BASE  = PS_MSG_ID_GRR_TO_TRRC_BASE + 0x100,
    PS_MSG_ID_TRRC_TO_LRRC_BASE  = PS_MSG_ID_LRRC_TO_TRRC_BASE + 0x40,
    /* TDS end */

    PS_MSG_ID_SMS_TO_LMM_BASE   = PS_MSG_ID_TRRC_TO_LRRC_BASE  + 0xa0,
    PS_MSG_ID_LMM_TO_SMS_BASE   = PS_MSG_ID_SMS_TO_LMM_BASE  + 0x20,

    /*CDS��ERABM֮�����Ϣ�ӿڵ�ȡֵ��Χ*/
    PS_MSG_ID_CDS_TO_ERABM_BASE = PS_MSG_ID_LMM_TO_SMS_BASE  + 0x20,
    PS_MSG_ID_ERABM_TO_CDS_BASE = PS_MSG_ID_CDS_TO_ERABM_BASE+ 0x20,

    /*CDS��ETC֮�����Ϣ�ӿڵ�ȡֵ��Χ*/
    PS_MSG_ID_CDS_TO_ETC_BASE   = PS_MSG_ID_ERABM_TO_CDS_BASE + 0x20,
    PS_MSG_ID_ETC_TO_CDS_BASE   = PS_MSG_ID_CDS_TO_ETC_BASE   + 0x20,

    /*CDS��NDClient֮�����Ϣ�ӿ�ȡֵ��Χ*/
    PS_MSG_ID_CDS_TO_NDCLIENT_BASE = PS_MSG_ID_ETC_TO_CDS_BASE + 0x20,
    PS_MSG_ID_NDCLIENT_TO_CDS_BASE = PS_MSG_ID_CDS_TO_NDCLIENT_BASE + 0x20,

    PS_MSG_ID_APS_TO_ND_BASE = PS_MSG_ID_NDCLIENT_TO_CDS_BASE + 0x20,
    PS_MSG_ID_ND_TO_APS_BASE = PS_MSG_ID_APS_TO_ND_BASE + 0x20,     /* 0x1B95 */

    PS_MSG_ID_MM_TO_LMM_BASE    = PS_MSG_ID_ND_TO_APS_BASE + 0x20,
    PS_MSG_ID_LMM_TO_MM_BASE    = PS_MSG_ID_MM_TO_LMM_BASE  + 0x20,

     /* LRRC��TRRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_APP_TO_TRRC_BASE  = PS_MSG_ID_LMM_TO_MM_BASE + 0x20,
    PS_MSG_ID_TRRC_TO_APP_BASE  = PS_MSG_ID_APP_TO_TRRC_BASE + 0x20,

    /* LRRC��TRRC��Ľӿ���Ϣ��ȡֵ��Χ */
    PS_MSG_ID_APP_TO_TDSL2_BASE  = PS_MSG_ID_TRRC_TO_APP_BASE + 0x20,
    PS_MSG_ID_TDSL2_TO_APP_BASE  = PS_MSG_ID_APP_TO_TDSL2_BASE + 0x20,
    /*��������Ҫ���ӵģ��������ڴ˴�����*/

    PS_MSG_ID_EMM_TO_EMM_OM_BASE  = PS_MSG_ID_TDSL2_TO_APP_BASE + 0x20,
    PS_MSG_ID_ESM_TO_ESM_OM_BASE  = PS_MSG_ID_EMM_TO_EMM_OM_BASE + 0x60,
    PS_MSG_ID_ERABM_TO_ERABM_OM_BASE= PS_MSG_ID_ESM_TO_ESM_OM_BASE + 0x20,

    /*�����Ϣ��� BEGIN*/
    TRRC_DEBUG_MSG_BASE = PS_MSG_ID_ERABM_TO_ERABM_OM_BASE+0x20 ,   /*0x1d15*/
    /*�����Ϣ��� END*/

    PS_MSG_ID_LNAS_TO_OM_BASE       = TRRC_DEBUG_MSG_BASE + 0x20,

    /* begin 2013-08-01 Modify DCM */
    PS_MSG_ID_OM_TO_EMM_BASE       = PS_MSG_ID_LNAS_TO_OM_BASE + 0x20,
    PS_MSG_ID_EMM_TO_OM_BASE       = PS_MSG_ID_OM_TO_EMM_BASE + 0x20,

    PS_MSG_ID_OM_TO_ESM_BASE       = PS_MSG_ID_EMM_TO_OM_BASE + 0x20,
    PS_MSG_ID_ESM_TO_OM_BASE       = PS_MSG_ID_OM_TO_ESM_BASE + 0x20,/*0x1DB5*/
    /* end 2013-08-01 Modify DCM */

    /* begin for ims 2014-1-6 */
    PS_MSG_ID_IMSA_TO_IMSA_OM_BASE  = PS_MSG_ID_ESM_TO_OM_BASE + 0x20,/*0x1DD5*/
    /* end for ims 2014-1-6 */

    /*DSDS feature,2014-7-18,begin*/
    PS_MSG_ID_DSDS_DEBUG_BASE  = PS_MSG_ID_IMSA_TO_IMSA_OM_BASE + 0x80,/*0x1e55*/

    PS_MSG_ID_IMSA_TO_ERRC_BASE = PS_MSG_ID_DSDS_DEBUG_BASE + 0x20,  /* 0x1e75 */
    /*DSDS feature,2014-7-18,end*/
    /* begin 2015-06-24 for css,begin */
    PS_MSG_ID_LPHY_TO_CSS_BASE  = PS_MSG_ID_IMSA_TO_ERRC_BASE + 0x20, /* 0x1e95 */
    /* begin 2015-06-24 for css,end */
    /* 2015-5-27 begin */
    /* KPI��������Ϣ */
    PS_MSG_ID_TL_KPI_BASE       = PS_MSG_ID_LPHY_TO_CSS_BASE + 0x20, /* 0x1eb5 */
    PS_MSG_ID_TL_PTL_BASE       = PS_MSG_ID_TL_KPI_BASE + 0x100, /* 0x1fb5 */
    /* 2015-5-27 end */


    PS_MSG_ID_TL_DBG_BASE       = PS_MSG_ID_TL_PTL_BASE + 0x200, /* 0x21b5 */
    PS_MSG_ID_TL_DT_BASE        = PS_MSG_ID_TL_DBG_BASE + 0x200, /* 0x23b5 */


    /* begin for dhcp 2015-06-18 */
    /*CDS��Dhcp֮�����Ϣ�ӿ�ȡֵ��Χ*/
    PS_MSG_ID_DHCP_TO_CDS_BASE = PS_MSG_ID_TL_DT_BASE + 0x100, /* 0x24b5 */
    PS_MSG_ID_CDS_TO_DHCP_BASE = PS_MSG_ID_DHCP_TO_CDS_BASE + 0x20, /* 0x24d5 */
    /* end for dhcp 2015-06-18 */
    /* ... ... */

    /* begin 2015-09-28 for LPP ,begin */
    /* LPP��LRRC֮�����Ϣ���ȡֵ��Χ */
    PS_MSG_ID_LPP_TO_LRRC_BASE = PS_MSG_ID_CDS_TO_DHCP_BASE + 0x20, /* 0x24f5 */
    PS_MSG_ID_LRRC_TO_LPP_BASE = PS_MSG_ID_LPP_TO_LRRC_BASE + 0X20, /* 0x2515 */
    /* begin 2015-09-28 for LPP ,begin */

    /* F-Project Phase I 2015-12-10 begin */
    /* EMM��IMSA������ϢBASE */
    PS_MSG_ID_EMM_TO_IMSA_BASE = PS_MSG_ID_LRRC_TO_LPP_BASE + 0x20, /* 0x2535 */
    /* F-Project Phase I 2015-12-10 end */
    /* begin 2016-01-28 for vowifi phaseII,begin */
    PS_MSG_ID_ERRC_TO_IMSA_BASE = PS_MSG_ID_EMM_TO_IMSA_BASE + 0x20, /* 0x2255 */
    /* begin 2016-01-28 for vowifi phaseII,end */
    /* V7R2DT  add rrc dt message base address, 2014/05/12 begin */

    /*L2����IMSA��Ϣ�Ļ�ַ*/
    PS_MSG_ID_L2_TO_IMSA_BASE = PS_MSG_ID_ERRC_TO_IMSA_BASE + 0x20, /* 0x2575 */

    /* add it for cellular prefer project, begin in 2016-09-28 */
    /* IMSA��CAS֮�����Ϣ�ӿ�ȡֵ��Χ */
    PS_MSG_ID_IMSA_TO_CAS_BASE   = PS_MSG_ID_L2_TO_IMSA_BASE + 0x20, /* 0x2595 */
    PS_MSG_ID_CAS_TO_IMSA_BASE   = PS_MSG_ID_IMSA_TO_CAS_BASE + 0x20, /* 0x25B5 */

    /* IMSA��WRR֮�����Ϣ�ӿ�ȡֵ��Χ */
    PS_MSG_ID_IMSA_TO_WRR_BASE   = PS_MSG_ID_CAS_TO_IMSA_BASE + 0x20, /* 0x25D5 */
    PS_MSG_ID_WRR_TO_IMSA_BASE   = PS_MSG_ID_IMSA_TO_WRR_BASE + 0x20, /* 0x25F5 */

    /* IMSA��GRR֮�����Ϣ�ӿ�ȡֵ��Χ */
    PS_MSG_ID_IMSA_TO_GRR_BASE   = PS_MSG_ID_WRR_TO_IMSA_BASE + 0x20, /* 0x2615 */
    PS_MSG_ID_GRR_TO_IMSA_BASE   = PS_MSG_ID_IMSA_TO_GRR_BASE + 0x20, /* 0x2635 */
    /* add it for cellular prefer project, end in 2016-09-28 */

    /* moded for GUNAS reconstruct 2018-03-09, begin */
    /* LMM��REGM֮�����Ϣ�ӿ�ȡֵ��Χ */
    PS_MSG_ID_LMM_TO_REGM_BASE   = PS_MSG_ID_GRR_TO_IMSA_BASE + 0x20, /* 0x2655*/
    PS_MSG_ID_REGM_TO_LMM_BASE   = PS_MSG_ID_LMM_TO_REGM_BASE + 0x40, /* 0x2695*/
    /* moded for GUNAS reconstruct 2018-03-09, end */

    /* TC interface with LRRC */
    PS_MSG_ID_TC_TO_LRRC_BASE   = PS_MSG_ID_REGM_TO_LMM_BASE + 0x20, /* 0x26B5*/
    PS_MSG_ID_LRRC_TO_TC_BASE   = PS_MSG_ID_TC_TO_LRRC_BASE + 0x40,  /* 0x26F5*/

    /* IMSA�͵���MSP֮���·����Ϣ�ӿ�ȡֵ��Χ */
    PS_MSG_ID_MSP_TO_IMS_BASE   = PS_MSG_ID_LRRC_TO_TC_BASE + 0x200,  /*0x28F5*/
    PS_MSG_ID_IMS_TO_MSP_BASE   = PS_MSG_ID_MSP_TO_IMS_BASE + 0x20,   /*0x2915*/

    /* RRC��·����ϢID�Ǵ�0x8D00-0x8DFF */
    PS_MSG_ID_RRC_TO_DT_BASE  = PS_DT_MSG_ID_BASE,
    /* V7R2DT  add rrc dt message base address, 2014/05/12 end */

    /* Modem errlog report, relation event lps-om message base address begin */
    PS_MSG_ID_LPSOM_PS_BASE   = PS_ERR_LOG_MSG_ID_BASE,
    /* Modem errlog report, relation event lps-om message base address end */                                                                                                                                                                                                                                                                                        /* V7R2DT  add rrc dt message base address, 2014/05/12 end */
    PS_MSG_ID_LPSOM_EMM_BASE  = PS_MSG_ID_LPSOM_PS_BASE + 0x40,

};
/* 2015-5-27 begin */

enum TL_KpiMsgIdSection
{
    TL_KPI_MSG_ID_LTE_NAS_BASE = PS_MSG_ID_TL_KPI_BASE,
    TL_KPI_MSG_ID_LTE_RRC_BASE = TL_KPI_MSG_ID_LTE_NAS_BASE + 0x40,
    TL_KPI_MSG_ID_TDS_RRC_BASE = TL_KPI_MSG_ID_LTE_RRC_BASE + 0x40,
    TL_KPI_MSG_ID_LTE_L2_BASE  = TL_KPI_MSG_ID_TDS_RRC_BASE + 0x40,
    TL_KPI_MSG_ID_TDS_L2_BASE  = TL_KPI_MSG_ID_LTE_L2_BASE  + 0x20,
    TL_KPI_MSG_ID_BUTT
};
/* 2015-5-27 end */

/* keyinfo report 2015-08-28 begin */

enum TL_DBG_MsgIdSection
{
    TL_DBG_MSG_ID_LTE_NAS_BASE = PS_MSG_ID_TL_DBG_BASE,
    TL_DBG_MSG_ID_LTE_RRC_BASE = TL_DBG_MSG_ID_LTE_NAS_BASE + 0x40,
    TL_DBG_MSG_ID_TDS_RRC_BASE = TL_DBG_MSG_ID_LTE_RRC_BASE + 0x40,
    TL_DBG_MSG_ID_LTE_L2_BASE  = TL_DBG_MSG_ID_TDS_RRC_BASE + 0x40,
    TL_DBG_MSG_ID_TDS_L2_BASE  = TL_DBG_MSG_ID_LTE_L2_BASE  + 0x20,
    TL_DBG_MSG_ID_BUTT
};
/* keyinfo report 2015-08-28 begin */


enum TL_PTL_MsgIdSection
{
    TL_PTL_MSG_ID_LTE_NAS_BASE = PS_MSG_ID_TL_PTL_BASE,
    TL_PTL_MSG_ID_LTE_RRC_BASE = TL_PTL_MSG_ID_LTE_NAS_BASE + 0x80,
    TL_PTL_MSG_ID_TDS_RRC_BASE = TL_PTL_MSG_ID_LTE_RRC_BASE + 0x80,
    TL_PTL_MSG_ID_LTE_L2_BASE  = TL_PTL_MSG_ID_TDS_RRC_BASE + 0x80,
    TL_PTL_MSG_ID_TDS_L2_BASE  = TL_PTL_MSG_ID_LTE_L2_BASE  + 0x40,
    TL_PTL_MSG_ID_BUTT
};


enum TL_DT_MsgIdSection
{
    TL_DT_MSG_ID_LTE_NAS_BASE = PS_MSG_ID_TL_DT_BASE,
    TL_DT_MSG_ID_LTE_RRC_BASE = TL_DT_MSG_ID_LTE_NAS_BASE + 0x40,
    TL_DT_MSG_ID_TDS_RRC_BASE = TL_DT_MSG_ID_LTE_RRC_BASE + 0x40,
    TL_DT_MSG_ID_LTE_L2_BASE  = TL_DT_MSG_ID_TDS_RRC_BASE + 0x40,
    TL_DT_MSG_ID_TDS_L2_BASE  = TL_DT_MSG_ID_LTE_L2_BASE  + 0x20,
    TL_DT_MSG_ID_BUTT
};


/*****************************************************************************
 �ṹ��    : UE_MSG_Type
 �ṹ˵��  : 32bit��message id����ߵ��ֽڵ���Ϣ����ȡֵ�Ķ���
*****************************************************************************/
enum UE_MSG_Type
{
    ENUM_STRATUM_MSG            = 0 ,       /* ģ�����Ϣ */
    ENUM_RESERVED_MSG               ,       /* ���� */
    ENUM_AIR_MSG                    ,       /* �տ���Ϣ */
    ENUM_GENERAL_CMD_MSG            ,       /* ��ͨ���� */
    ENUM_REAL_TIME_MSG              ,       /* ʵʱ������� */
    ENUM_KEY_EVENT_MSG              ,       /* �ؼ��¼� */
    ENUM_TRANSPARENT_MSG            ,       /* ͸������ */
    ENUM_MSG_BUTT

};
typedef VOS_UINT8 UE_MSG_TypeUint8;

/*****************************************************************************
 �ṹ��    : UE_FUNC_ModuleId
 �ṹ˵��  : UE���ģ��ID�Ķ���, PS��ģ��ID��Χ [0xF200 -- 0xF800)
*****************************************************************************/
//[NV elf������Ŀ] nv_define����ʧ�ܣ��˴��ú�ȥ��
#ifndef NV_DEFINE
enum UE_FUNC_ModuleId
{
    UE_MODULE_PS_ID             = 0x00,     /*UE��PS���ͳ��*/

    /*UE_MODULE_NAS_ID = 0x10,
 */
    UE_MODULE_ESM_ID            = PS_PID_ESM,
    UE_MODULE_MM_ID             = PS_PID_MM,
    UE_MODULE_RABM_ID           = PS_PID_RABM,

    /*UE_MODULE_RRC_ID = 0x40,
 */
    UE_MODULE_ERRC_ID           = PS_PID_ERRC,
    UE_MODULE_ERMM_ID           = PS_PID_ERMM,
    UE_MODULE_HPA_M_ID          = PS_PID_HPA,  /* ���߳�����RTTͨ�ŵ�HPAģ�� */

    /*UE_MODULE_L2_ID             = 0x70,    L2ͳ�ƣ�Ԥ�� */
    /*UE_MODULE_L2_PDCP_ID      = 0x71,    PDCPͳ�ƣ�Ԥ�� */
    /*UE_MODULE_L2_RLC_ID       = 0x72     RLCͳ�ƣ�Ԥ�� */
    /*UE_MODULE_L2_MAC_ID       = 0x73     MACͳ�ƣ�Ԥ�� */

    UE_MODULE_L2_COM_ID         = PS_PID_PDCP_UL,
    UE_MODULE_L2_PDCP_UL_ID     = PS_PID_PDCP_UL,
    UE_MODULE_L2_PDCP_DL_ID     = PS_PID_PDCP_DL,
    UE_MODULE_L2_RLC_UL_ID      = PS_PID_RLC_UL,
    UE_MODULE_L2_RLC_DL_ID      = PS_PID_RLC_DL,
    UE_MODULE_L2_MAC_UL_ID      = PS_PID_MAC_UL,
    UE_MODULE_L2_MAC_DL_ID      = PS_PID_MAC_DL,
    /*UE_MODULE_HPA_S_ID          = 0x7c,   ���߳��������߳�ͨ�ŵ�HPAģ�� */
    /*UE_MODULE_HPA_UL_ID         = 0x7d,   ���߳�����RTTͨ�ŵ�����HPAģ�� */
    /*UE_MODULE_HPA_DL_ID         = 0x7e,   ���߳�����RTTͨ�ŵ�����HPAģ�� */

    UE_MODULE_PS_OM             = PS_PID_OM,  /* PSOMģ�� */

    UE_MODULE_TC_ID             = PS_PID_TC,  /* TCģ�� */
    UE_MODULE_IP_ID             = PS_PID_IP,   /* IPģ�� */
    UE_MODULE_MMC_ID            = 0xF210,
    UE_MODULE_GMM_ID            = 0xF211,
    UE_MODULE_WAS_ID            = 0xF212,
    UE_MODULE_GAS_ID            = 0xF213,

    UE_MODULE_NDIS_UL           = 0xF214,
    UE_MODULE_NDIS_DL           = 0xF215,
    UE_MODULE_NDIS_COM          = 0xF216,

    /*ims begin */
    UE_MODULE_IMSA_ID           = 0xF217,
    /*ims end */
    /* Add FOR LPP begin */
    UE_MODULE_LPP_ID           = 0xF218,
    /* Add FOR LPP end */
    /* Add FOR LCS begin */
    UE_MODULE_LCS_ID           = 0xF219,
    /* Add FOR LCS end */
    /* Added for GU_BACK_OFF,2016-04-09,Begin */
    UE_MODULE_BACKOFF_ID       = 0xF21a
    /* Added for GU_BACK_OFF,2016-04-09,Begin */
};
typedef VOS_UINT32 UE_FUNC_ModuleIdUint32;
#endif
/*****************************************************************************
 �ṹ��    : UE_APP_ModuleId
 �ṹ˵��  : OM�����ģ��ID�Ķ���
*****************************************************************************/
enum UE_APP_ModuleId
{

    UE_APP_SUBSYS_ID            = 0xc0, /*PC���������ģ���ͳ��  */

    UE_MODULE_OM_AGENT_ID       = 0xc1,  /* PC��OM����ģ�� */
    UE_MODULE_OM_ID             = 0xc2,  /* ���߳�������OMģ�� */
    UE_MODULE_OM_SLAVE_ID       = 0xc3,  /* ���߳���OMģ�� */
    UE_MODULE_OM_BBU_RFOM_ID    = 0xc4,  /* BBU���RFOMģ��*/
    UE_MODULE_OM_MUX_ID         = 0xc5,  /* BBU���MUXģ��*/
    UE_MODULE_OM_RRU_RFOM_ID    = 0xc6,  /* RRU����RFOMģ�� */
    UE_MODULE_OM_RFDT_ID        = 0xc7,  /* RRU���RFDTģ��*/

    UE_MODULE_OMT_ID            = 0xd1,  /* PC��OMT����ģ�� */
    UE_MODULE_PROBE_ID          = 0xd2,  /* PC��·�⹤��ģ�� */
    UE_MODULE_BD_ID             = 0xd3   /* PC��װ������ģ�� */


};
typedef VOS_UINT8 UE_APP_ModuleIdUint8;

/*****************************************************************************
 �ṹ��    : LOG_SubModId
 �ṹ˵��  : NAS��ģ�鶨��
*****************************************************************************/
typedef enum
{
    LOG_SUBMOD_ID_NAS_MM_PUBM,
    LOG_SUBMOD_ID_NAS_MM_MMC,
    LOG_SUBMOD_ID_NAS_MM_EMM_ATTACH,
    LOG_SUBMOD_ID_NAS_MM_EMM_DETACH,
    LOG_SUBMOD_ID_NAS_MM_EMM_TAU,
    LOG_SUBMOD_ID_NAS_MM_EMM_SERVICE,
    LOG_SUBMOD_ID_NAS_MM_EMM_GUTI,
    LOG_SUBMOD_ID_NAS_MM_EMM_IDEN,
    LOG_SUBMOD_ID_NAS_MM_EMM_MRRC,
    LOG_SUBMOD_ID_NAS_MM_EMM_PLMN,
    LOG_SUBMOD_ID_NAS_MM_EMM_PUBU,
    LOG_SUBMOD_ID_NAS_MM_EMM_SECU,
    LOG_SUBMOD_ID_NAS_MM_BUTT
}LOG_SubModId;


typedef  VOS_UINT8  LOG_SubModIdUint8;

/*****************************************************************************
 ö����: LRRC_COMM_AccessStratumRel
 Э���ʽ:
 ASN.1����:
 ö��˵��: ֪ͨNAS��MTA������Э��汾
           �޸�ʱ��ͬ���޸Ŀտ�UE�����ϱ���Э��汾��ö��
*****************************************************************************/
enum LRRC_COMM_AccessStratumRel
{
    LRRC_COMM_ACCESS_STRATUM_REL8                  = 0,
    LRRC_COMM_ACCESS_STRATUM_REL9                  = 1,
    LRRC_COMM_ACCESS_STRATUM_REL10                 = 2,
    LRRC_COMM_ACCESS_STRATUM_REL11                 = 3,
    LRRC_COMM_ACCESS_STRATUM_REL12                 = 4,
    LRRC_COMM_ACCESS_STRATUM_REL13                 = 5,
    /*Added for R14_CR_PHASEI 2017-04-01 begin */
    LRRC_COMM_ACCESS_STRATUM_REL14                 = 6,
    /*Added for R14_CR_PHASEI 2017-04-01 end */
    LRRC_COMM_ACCESS_STRATUM_REL15                 = 7,
    LRRC_COMM_ACCESS_STRATUM_REL16                 = 8,
    LRRC_COMM_ACCESS_STRATUM_REL17                 = 9,
    LRRC_COMM_ACCESS_STRATUM_REL_SPARE1            = 10
};
typedef VOS_UINT8 LRRC_COMM_AccessStratumRelUint8;

/*****************************************************************************
  5 STRUCT
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : LPS_TimeStamp32K
 �ṹ˵��  : 32KTIMESTAMP
*****************************************************************************/
typedef struct
{
    VOS_UINT32   tStampHighBit32K;
    VOS_UINT32   tStampLowBit32K;
}LPS_TimeStamp32K;

typedef VOS_UINT64 LPS_TStamp32kU64;
typedef VOS_UINT64 LPS_MsTStampU64;

/*****************************************************************************
?��11??3?    :NAS_MM_PLMN_ID_STRU?D
��1��??��?��    :
    MCC, Mobile country code (aucPlmnId[0], aucPlmnId[1] bits 1 to 4)
    MNC, Mobile network code (aucPlmnId[2], aucPlmnId[1] bits 5 to 8).

    The coding of this field is the responsibility of each administration but BCD
    coding shall be used. The MNC shall consist of 2 or 3 digits. For PCS 1900 for NA,
    Federal regulation mandates that a 3-digit MNC shall be used. However a network
    operator may decide to use only two digits in the MNC over the radio interface.
    In this case, bits 5 to 8 of octet 4 shall be coded as "1111". Mobile equipment
    shall accept MNC coded in such a way.

    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2            |           MCC digit 1
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3            |           MCC digit 3
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2            |           MNC digit 1
    ---------------------------------------------------------------------------

    AT?����?��o
    at+cops=1,2,"mcc digit 1, mcc digit 2, mcc digit 3, mnc digit 1, mnc digit 2, mnc

digit 3",2 :

    e.g.
    at+cops=1,2,"789456",2 :
    --------------------------------------------------------------------------------
    (mcc digit 1)|(mcc digit 2)|(mcc digit 3)|(mnc digit 1)|(mnc digit 2)|(mnc digit 3)
    --------------------------------------------------------------------------------
       7         |     8       |      9      |     4       |      5      |     6
    --------------------------------------------------------------------------------

    ?��aucPlmnId[3]?D��?��?��???��?:
    ---------------------------------------------------------------------------
                 ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
    ---------------------------------------------------------------------------
    aucPlmnId[0] ||    MCC digit 2 = 8        |           MCC digit 1 = 7
    ---------------------------------------------------------------------------
    aucPlmnId[1] ||    MNC digit 3 = 6        |           MCC digit 3 = 9
    ---------------------------------------------------------------------------
    aucPlmnId[2] ||    MNC digit 2 = 5        |           MNC digit 1 = 4
    ---------------------------------------------------------------------------
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           plmnId[3];
    VOS_UINT8                           rsv;
}NAS_LPSCOMM_PlmnId;


/*****************************************************************************
?��11??3?    :NAS_MM_TAC_STRU
��1��??��?��    :TACD??��  24.301  9.9.3.26
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           tac;
    VOS_UINT8                           tacCnt;
    VOS_UINT8                           rsv[2];
}NAS_LPSCOMM_Tac;

/*****************************************************************************
 �ṹ��    : NAS_MM_TacAndSharePlmnInfo
 �ṹ˵��  : TA��Ϣ���ݽṹ
*****************************************************************************/
typedef struct
{
    NAS_LPSCOMM_Tac                     tacInfo;
    /*���������У���ͬһ��TAC�¿����ж��PLMN�����; Ŀǰ����ֻѡȡ��һ��PLMN�����ж�*/
    VOS_UINT32                          plmnNum;
    NAS_LPSCOMM_PlmnId                 plmnIdList[NAS_MM_MAX_SHARE_PLMN_NUM];
}NAS_MM_TacAndSharePlmnInfo;


extern PS_BOOL_ENUM_UINT8  IMSA_LRRC_Get23GNeighCellStatus( MODEM_ID_ENUM_UINT16 modemId );
/*
 * �ӿ�Լ��: LRRCֻ��ANYCELL SEARCH���ҳ�����EMCʱ�Ż�������������
 * 1��LRRC��anycell �ѽ���㲥�����LNAS�ṩ�Ľӿڣ�����������Ʒ������ж�SIB1�еĽ�������ʶ��
 * ������ط����Ʒ���Ͳ��ж�SIB1
*/
extern VOS_UINT32 NAS_IsSpecCellInLimitService(
    MODEM_ID_ENUM_UINT16                            modemId,
    NAS_MM_TacAndSharePlmnInfo             *tacSharePlmnInfo
);

extern VOS_VOID LHPA_TriggerLPdcpAllowSleepProc(VOS_UINT16 modemId);
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of lps_common.h */
