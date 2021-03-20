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

#ifndef __AT_AGENT_H__
#define __AT_AGENT_H__

#include "at_mn_interface.h"
#include "taf_app_mma.h"
#include "mdrv.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_adc.h"
#include "mdrv_version.h"
#include "mdrv_sysboot.h"
#endif
#include "at_phy_interface.h"

#include "v_typdef.h"

#ifdef MBB_COMMON
#include "MbbTafFactoryAtComm.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef WIN32
#pragma warning(disable : 4200) /* zero-sized array in struct/union */
#endif

#pragma pack(push, 4)

#define AT_MAX_VERSION_LEN (128) /* �汾��Ϣ��󳤶� */

#define AT_MAX_PDM_VER_LEN (16) /* PDM�汾�ŵ���󳤶� */

#define TAF_DLOAD_INFO_LEN (1024)

#define TAF_AUTH_ID_LEN (50)

#define AT_DLOAD_TASK_DELAY_TIME (500)

#define TAF_MAX_VER_INFO_LEN (6)

#define TAF_MAX_PROPLAT_LEN (63) /* ���ƽ̨����汾���� */

#define TAF_AUTHORITY_LEN (50) /* AUTHORITYVER�������������� */

/* ����ӿ�DRV_GET_VERSION_TIME����ʱ���ַ�����󳤶� */
#define AT_AGENT_DRV_VERSION_TIME_LEN (32)

#define TAF_MAX_FLAFH_INFO_LEN (255)

#define TAF_MAX_GPIO_INFO_LEN (32)

/* Ӳ���汾����󳤶� */
#define DRV_AGENT_MAX_MODEL_ID_LEN (31)
#define DRV_AGENT_MAX_REVISION_ID_LEN (31)

/* MS֧�ֵĽ��շּ����Ͷ��� */
#define DRV_AGENT_MS_SUPPORT_RX_DIV_2100 (0x0001)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_1900 (0x0002)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_1800 (0x0004)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_AWS_1700 (0x0008)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_850 (0x0010)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_800 (0x0020)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_2600 (0x0040)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_900 (0x0080)
#define DRV_AGENT_MS_SUPPORT_RX_DIV_IX_1700 (0x0100)

#define DRV_AGENT_DSP_RF_SWITCH_ON (1)

#define DRV_AGENT_SIMLOCK_MAX_ERROR_TIMES (3) /* Simlock�������ʧ�ܴ��� */

#define AT_PID_MEM_INFO_LEN (150) /* VOS�ڴ��ѯʱ֧�ֵ����PID���� */

#define AT_KB_TO_BYTES_NUM (1024) /* KB���ֽ�ת�����1024 */

#if (FEATURE_ON == FEATURE_SECURITY_SHELL)
/* ��ǿSHELL�����뱣����ض��� */
#ifndef MBB_COMMON
#define AT_SHELL_PWD_LEN (8)
#endif
#endif

/*
 * �趨һ������ֵ100�����ǵ�PAD ��NANDΪ256MB������block sizeΪ128kB������2048�飬
 * ������һ��Ҫ����5%��Ϊ�Ǻϸ��FLASH����2048*5%=102.4��ȡ100Ϊ����������ޣ���
 * ���������������100������ֻ���ǰ100����������������������л���������
 * �����ص�<totalNum>�԰���ʵ���ܿ��������
 */
#define DRV_AGENT_NAND_BADBLOCK_MAX_NUM (100)

#define DRV_AGENT_CHIP_TEMP_ERR (65535)

#define DRV_AGENT_HUK_LEN (16)                    /* HUKΪ128Bits������, ����Ϊ16Bytes */
#define DRV_AGENT_PUBKEY_LEN (1032)               /* ��Ȩ��Կ����, ����Ϊ1032Bytes */
#define DRV_AGENT_RSA_CIPHERTEXT_LEN 256          /* RSA���ĳ���, ͳһΪ256Bytes */
#define DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN 512      /* RSA����TLV��ʽ����, ͳһΪ512Bytes */
#define DRV_AGENT_SIMLOCKDATAWRITE_STRU_LEN (516) /* ��������������������ṹ�峤�� */
#define DRV_AGENT_PUBKEY_SIGNATURE_LEN (32)       /* ��Ȩ��ԿSSKǩ������ */

/* ֧�ֵ���������CATEGORY�������Ŀǰֻ֧������:network/network subset/SP/CP */
#define DRV_AGENT_SUPPORT_CATEGORY_NUM (4)

#define DRV_AGENT_SIMLOCK_ENCRYPT_KEYID_NUM 4

#define DRV_AGENT_PH_LOCK_CODE_GROUP_NUM (20) /* ���������Ŷ����� */

/* ��������������֧�ֵ���������CATEGORY�������Ŀǰֻ֧������:network/network subset/SP/CP */
#define DRV_AGENT_SUPPORT_CATEGORY_NUM_EXTERED (4)
#define DRV_AGENT_PH_LOCK_CODE_GROUP_NUM_EXTERED (10) /* ���������������ϱ���������������Ŷ����� */
#define DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED (6)       /* ���������Ŷγ��� */

#define DRV_AGENT_PH_LOCK_CODE_LEN (4) /* ���������Ŷγ��� */

#define DRV_AGENT_PH_PHYNUM_LEN (16)                                   /* ����ų��� */
#define DRV_AGENT_PH_PHYNUM_IMEI_LEN (15)                              /* IMEI�ų��� */
#define DRV_AGENT_PH_PHYNUM_IMEI_NV_LEN (16)                           /* IMEI��NV��� */
#define DRV_AGENT_PH_PHYNUM_SN_LEN (16)                                /* SN���� */
#define DRV_AGENT_PH_PHYNUM_SN_NV_LEN (DRV_AGENT_PH_PHYNUM_SN_LEN + 4) /* SN��NV��� */
#define DRV_AGENT_PORT_PASSWORD_LEN (16)                               /* ����ͨ�Ŷ˿������볤�� */

#define DRV_AGENT_DH_PUBLICKEY_LEN (128) /* DH�㷨ʹ�õķ�������Կ��Modem�๫Կ���� */
#define DRV_AGENT_DH_PRIVATEKEY_LEN (48) /* DH�㷨ʹ�õ�Modem��˽Կ���� */

#define DRV_AGENT_HMAC_DATA_LEN (32) /* HMAC���ݵĳ��� */

#if (FEATURE_ON == FEATURE_PHONE_SC)
#define DRV_AGENT_PIN_CRYPTO_DATA_LEN (16) /* PIN�������ݵĳ��� */
#define DRV_AGENT_PIN_CRYPTO_IV_LEN (16)   /* PIN IV���ݵĳ��� */
/* SCĿǰ�漰��KEY ID���� */
#define DRV_AGENT_SUPPORT_SC_KEY_ID_NUM  4
#endif

#define TAF_MAX_CPU_CNT (4)
#define TAF_SRE_ERROR_CODE (0xffffffff)

#define TAF_OTA_SIMLOCK_PUBLIC_ID_NUM 3

#define DRV_AGENT_RSA2048_ENCRYPT_LEN 256
#define DRV_AGENT_NETDOG_AUTH_RANDOM_LEN 16
#define DRV_AGENT_PHY_OM_SAR_MASK_REDUCTION 0x2

/*
 * �ṹ˵��: ^TSELRF �������ò�������������б�
 *           0: DRV_AGENT_TSELRF_SET_ERROR_NO_ERROR ���ò����ɹ���
 *           1: DRV_AGENT_TSELRF_SET_ERROR_LOADDSP  ���ò���ʧ�ܣ�
 */
enum DRV_AGENT_TselrfSetError {
    DRV_AGENT_TSELRF_SET_NO_ERROR = 0,
    DRV_AGENT_TSELRF_SET_LOADDSP_FAIL,

    DRV_AGENT_TSELRF_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_TselrfSetErrorUint32;

/*
 * �ṹ˵��: ��ȡHKADC��ѹ��������������б�
 *           0: DRV_AGENT_HKADC_GET_NO_ERROR�����ɹ���
 *           1: DRV_AGENT_HKADC_GET_FAIL         ����ʧ�ܣ�
 */
enum DRV_AGENT_HkadcGetError {
    DRV_AGENT_HKADC_GET_NO_ERROR = 0,
    DRV_AGENT_HKADC_GET_FAIL,

    DRV_AGENT_HKADC_GET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HkadcGetErrorUint32;

/*
 * �ṹ˵��: USIMWRITESTUB �������ò�������������б�
 *           0: ���ò����ɹ���
 *           1: ���ò���ʧ�ܣ�
 */
enum DRV_AGENT_Error {
    DRV_AGENT_NO_ERROR = 0,
    DRV_AGENT_ERROR,
    DRV_AGENT_PARAM_ERROR,
    DRV_AGENT_CME_ERROR,
    DRV_AGENT_CME_RX_DIV_OTHER_ERR,
    DRV_AGENT_CME_RX_DIV_NOT_SUPPORTED,
    DRV_AGENT_CME_RX_DIV_BAND_ERR,

    DRV_AGENT_BUTT
};
typedef VOS_UINT32 DRV_AGENT_ErrorUint32;


enum DRV_AGENT_MsidQryError {
    DRV_AGENT_MSID_QRY_NO_ERROR              = 0,
    DRV_AGENT_MSID_QRY_READ_PRODUCT_ID_ERROR = 1,
    DRV_AGENT_MSID_QRY_READ_SOFT_VER_ERROR   = 2,
    DRV_AGENT_MSID_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_MsidQryErrorUint32;


enum DRV_AGENT_AppdmverQryError {
    DRV_AGENT_APPDMVER_QRY_NO_ERROR = 0,
    DRV_AGENT_APPDMVER_QRY_ERROR    = 1,

    DRV_AGENT_APPDMVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AppdmverQryErrorUint32;


enum DRV_AGENT_DloadinfoQryError {
    DRV_AGENT_DLOADINFO_QRY_NO_ERROR = 0,
    DRV_AGENT_DLOADINFO_QRY_ERROR    = 1,

    DRV_AGENT_DLOADINFO_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_DloadinfoQryErrorUint32;


enum DRV_AGENT_AuthorityverQryError {
    DRV_AGENT_AUTHORITYVER_QRY_NO_ERROR = 0,
    DRV_AGENT_AUTHORITYVER_QRY_ERROR    = 1,

    DRV_AGENT_AUTHORITYVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AuthorityverQryErrorUint32;


enum DRV_AGENT_AuthorityidQryError {
    DRV_AGENT_AUTHORITYID_QRY_NO_ERROR = 0,
    DRV_AGENT_AUTHORITYID_QRY_ERROR    = 1,

    DRV_AGENT_AUTHORITYID_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AuthorityidQryErrorUint32;


enum DRV_AGENT_GodloadSetError {
    DRV_AGENT_GODLOAD_SET_NO_ERROR = 0,
    DRV_AGENT_GODLOAD_SET_ERROR    = 1,

    DRV_AGENT_GODLOAD_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_GodloadSetErrorUint32;


enum DRV_AGENT_SdloadSetError {
    DRV_AGENT_SDLOAD_SET_NO_ERROR = 0,
    DRV_AGENT_SDLOAD_SET_ERROR    = 1,

    DRV_AGENT_SDLOAD_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_SdloadSetErrorUint32;


enum DRV_AGENT_HwnatqryQryError {
    DRV_AGENT_HWNATQRY_QRY_NO_ERROR = 0,
    DRV_AGENT_HWNATQRY_QRY_ERROR    = 1,

    DRV_AGENT_HWNATQRY_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HwnatqryQryErrorUint32;


enum DRV_AGENT_CpuloadQryError {
    DRV_AGENT_CPULOAD_QRY_NO_ERROR = 0,
    DRV_AGENT_CPULOAD_QRY_ERROR    = 1,

    DRV_AGENT_CPULOAD_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_CpuloadQryErrorUint32;


enum DRV_AGENT_MfreelocksizeQryError {
    DRV_AGENT_MFREELOCKSIZE_QRY_NO_ERROR = 0,
    DRV_AGENT_MFREELOCKSIZE_QRY_ERROR    = 1,

    DRV_AGENT_MFREELOCKSIZE_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_MfreelocksizeQryErrorUint32;


enum DRV_AGENT_AuthverQryError {
    DRV_AGENT_AUTHVER_QRY_NO_ERROR = 0,
    DRV_AGENT_AUTHVER_QRY_ERROR    = 1,

    DRV_AGENT_AUTHVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_AuthverQryErrorUint32;


enum DRV_AGENT_FlashinfoQryError {
    DRV_AGENT_FLASHINFO_QRY_NO_ERROR = 0,
    DRV_AGENT_FLASHINFO_QRY_ERROR    = 1,

    DRV_AGENT_FLASHINFO_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_FlashinfoQryErrorUint32;


enum DRV_AGENT_PfverQryError {
    DRV_AGENT_PFVER_QRY_NO_ERROR               = 0,
    DRV_AGENT_PFVER_QRY_ERROR_GET_VERSION_TIME = 1,
    DRV_AGENT_PFVER_QRY_ERROR_GET_VERSION      = 2,
    DRV_AGENT_PFVER_QRY_ERROR_ALLOC_MEM        = 3,

    DRV_AGENT_PFVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PfverQryErrorUint32;

/*
 * �ṹ˵��: DLOADVER �����ѯ��������������б�
 *           0: ��ѯ�����ɹ���
 *           1: ��ѯ����ʧ�ܣ�
 */
enum DRV_AGENT_DloadverQryError {
    DRV_AGENT_DLOADVER_QRY_NO_ERROR = 0,
    DRV_AGENT_DLOADVER_QRY_ERROR    = 1,

    DRV_AGENT_DLOADVER_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_DloadverQryErrorUint32;


enum DRV_AGENT_PersonalizationErr {
    DRV_AGENT_PERSONALIZATION_NO_ERROR = 0,                     /* �����ɹ� */
    DRV_AGENT_PERSONALIZATION_IDENTIFY_FAIL,                    /* ���߼�Ȩʧ�� */
    DRV_AGENT_PERSONALIZATION_SIGNATURE_FAIL,                   /* ǩ��У��ʧ�� */
    DRV_AGENT_PERSONALIZATION_DK_INCORRECT,                     /* �˿�������� */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_LEN_ERROR,              /* ����ų��ȴ��� */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_VALUE_ERROR,            /* ���������� */
    DRV_AGENT_PERSONALIZATION_PH_PHYNUM_TYPE_ERROR,             /* ��������ʹ��� */
    DRV_AGENT_PERSONALIZATION_RSA_ENCRYPT_FAIL,                 /* RSA����ʧ�� */
    DRV_AGENT_PERSONALIZATION_RSA_DECRYPT_FAIL,                 /* RSA����ʧ�� */
    DRV_AGENT_PERSONALIZATION_GET_RAND_NUMBER_FAIL,             /* ��ȡ�����ʧ��(crypto_rand) */
    DRV_AGENT_PERSONALIZATION_WRITE_HUK_FAIL,                   /* HUKд����� */
    DRV_AGENT_PERSONALIZATION_FLASH_ERROR,                      /* Flash���� */
    DRV_AGENT_PERSONALIZATION_OTHER_ERROR,                      /* �������� */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_UPDATE,                  /* OTA�������ݸ��¹����� */
    DRV_AGENT_PERSONALIZATION_OTA_RAND_CMP_FAIL,                /* OTA������Ȩ������Ƚ�ʧ�� */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_CAT_CHECK_FAIL,       /* OTA����֧�ֲ���ʧ�� */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_MODEM_NUM_CHECK_FAIL, /* OTA����֧��modem����ʧ�� */
    DRV_AGENT_PERSONALIZATION_OTA_SUPPORT_DATA_CFG_CHECK_FAIL,  /* OTA�����������ü��ʧ�� */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_LAYER_CHECK_FAIL,        /* OTA������������� */
    DRV_AGENT_PERSONALIZATION_OTA_DATA_SIZE_CHECK_FAIL,         /* OTA������������У��ʧ�� */
    DRV_AGENT_PERSONALIZATION_INTERGITY_CHECK_FAIL,             /* �����Բ������ʹ��� */

    DRV_AGENT_PERSONALIZATION_ERR_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PersonalizationErrUint32;

enum DRV_AGENT_EncryptRsaAlgorithm {
    DRV_AGENT_ENCRYPT_ALGORITHM_RSA2048 = 1,
    DRV_AGENT_ENCRYPT_ALGORITHM_RSA_BUTT,
};
typedef VOS_UINT32 DRV_AGENT_EncryptRsaAlgorithmUint32;


enum DRV_AGENT_PersonalizationCategory {
    DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK          = 0x00, /* category:���� */
    DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET   = 0x01, /* category:������ */
    DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER = 0x02, /* category:��SP */
    DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE        = 0x03, /* category:������ */
    DRV_AGENT_PERSONALIZATION_CATEGORY_SIM_USIM         = 0x04, /* category:��(U)SIM�� */

    DRV_AGENT_PERSONALIZATION_CATEGORY_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationCategoryUint8;


enum DRV_AGENT_PersonalizationIndicator {
    DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE = 0x00, /* δ���� */
    DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE   = 0x01, /* �Ѽ��� */

    DRV_AGENT_PERSONALIZATION_INDICATOR_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationIndicatorUint8;


enum DRV_AGENT_PersonalizationStatus {
    DRV_AGENT_PERSONALIZATION_STATUS_READY = 0x00, /* �Ѿ����� */
    DRV_AGENT_PERSONALIZATION_STATUS_PIN   = 0x01, /* δ����״̬������������� */
    DRV_AGENT_PERSONALIZATION_STATUS_PUK   = 0x02, /* Block״̬��������UnBlock�� */

    DRV_AGENT_PERSONALIZATION_STATUS_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PersonalizationStatusUint8;


enum DRV_AGENT_PhPhynumType {
    DRV_AGENT_PH_PHYNUM_IMEI = 0x00, /* IMEI */
    DRV_AGENT_PH_PHYNUM_SN   = 0x01, /* SN */

    DRV_AGENT_PH_PHYNUM_BUTT
};
typedef VOS_UINT8 DRV_AGENT_PhPhynumTypeUint8;


enum DRV_AGENT_PortStatus {
    DRV_AGENT_PORT_STATUS_OFF = 0, /* �˿ڹر� */
    DRV_AGENT_PORT_STATUS_ON  = 1, /* �˿ڴ� */

    DRV_AGENT_PORT_STATUS_BUTT
};
typedef VOS_UINT32 DRV_AGENT_PortStatusUint32;


enum DRV_AGENT_UsimOperate {
    DRV_AGENT_USIM_OPERATE_DEACT = 0, /* ȥ����USIM */
    DRV_AGENT_USIM_OPERATE_ACT   = 1, /* ����USIM */

    DRV_AGENT_USIM_OPERATE_BUTT
};
typedef VOS_UINT32 DRV_AGENT_UsimOperateUint32;


enum DRV_AGENT_MsgType {
    /* ��Ϣ���� */ /* ��ϢID */ /* ��ע */
    /* AT����DRV AGENT����Ϣ */
    DRV_AGENT_MSID_QRY_REQ = 0x0000, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_GCF_IND      = 0x0002,

    DRV_AGENT_GPIOPL_SET_REQ        = 0x000E,
    DRV_AGENT_GPIOPL_QRY_REQ        = 0x0010,
    DRV_AGENT_DATALOCK_SET_REQ      = 0x0012,
    DRV_AGENT_TBATVOLT_QRY_REQ      = 0x0014,
    DRV_AGENT_VERTIME_QRY_REQ       = 0x0016,
    DRV_AGENT_YJCX_SET_REQ          = 0x0020,
    DRV_AGENT_YJCX_QRY_REQ          = 0x0022,
    DRV_AGENT_APPDMVER_QRY_REQ      = 0x0024, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_DLOADINFO_QRY_REQ     = 0x0028, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_FLASHINFO_QRY_REQ     = 0x002A, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_AUTHVER_QRY_REQ       = 0x002C, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_AUTHORITYVER_QRY_REQ  = 0x0030, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_AUTHORITYID_QRY_REQ   = 0x0032, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_GODLOAD_SET_REQ       = 0x0034, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_CPULOAD_QRY_REQ       = 0x0038, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_MFREELOCKSIZE_QRY_REQ = 0x003A, /* _H2ASN_MsgChoice AT_APPCTRL */

    DRV_AGENT_MEMINFO_QRY_REQ = 0x003B, /* _H2ASN_MsgChoice AT_APPCTRL */

    DRV_AGENT_HARDWARE_QRY            = 0x003E, /* ��ѯӲ���汾�� */
    DRV_AGENT_FULL_HARDWARE_QRY       = 0x0040, /* ��ѯ������Ӳ���汾�� */
    DRV_AGENT_TMODE_SET_REQ           = 0x0046,
    DRV_AGENT_VERSION_QRY_REQ         = 0x0048,
    DRV_AGENT_PFVER_QRY_REQ           = 0x004E,
    DRV_AGENT_FCHAN_SET_REQ           = 0x0052,
    DRV_AGENT_SDLOAD_SET_REQ          = 0x0056,
    DRV_AGENT_PRODTYPE_QRY_REQ        = 0x0060,
    DRV_AGENT_SDREBOOT_REQ            = 0x0062,
    DRV_AGENT_DLOADVER_QRY_REQ        = 0x0066,

    DRV_AGENT_SIMLOCK_SET_REQ = 0x006C,
    DRV_AGENT_IMSICHG_QRY_REQ = 0x0072, /* _H2ASN_MsgChoice AT_APPCTRL */
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRESTORE_SET_REQ = 0x0074,
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRSTSTTS_QRY_REQ = 0x0078,
    DRV_AGENT_CPNN_TEST_REQ     = 0x007A, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_NVBACKUP_SET_REQ  = 0x007C, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_CPNN_QRY_REQ      = 0x007E, /* _H2ASN_MsgChoice AT_APPCTRL */
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRESTORE_MANU_DEFAULT_REQ = 0x0080,
    /* _H2ASN_MsgChoice SPY_TempThresholdPara */
    DRV_AGENT_ADC_SET_REQ = 0x0082,
    /* _H2ASN_MsgChoice DRV_AGENT_TselrfSetReq */
    DRV_AGENT_TSELRF_SET_REQ   = 0x0084,
    DRV_AGENT_HKADC_GET_REQ    = 0x0086, /* _H2ASN_MsgChoice  */
    DRV_AGENT_TBAT_QRY_REQ     = 0x0088,

    DRV_AGENT_SIMLOCK_NV_SET_REQ = 0x008c,

    DRV_AGENT_SPWORD_SET_REQ = 0x008e,

    DRV_AGENT_PSTANDBY_SET_REQ = 0x0090,

    DRV_AGENT_NVBACKUPSTAT_QRY_REQ = 0x0092, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_NANDBBC_QRY_REQ      = 0x0094, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_NANDVER_QRY_REQ      = 0x0096, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_CHIPTEMP_QRY_REQ     = 0x0098, /* _H2ASN_MsgChoice AT_APPCTRL */

    /* _H2ASN_MsgChoice DRV_AGENT_HukSetReq */
    DRV_AGENT_HUK_SET_REQ = 0x009A,
    /* _H2ASN_MsgChoice DRV_AGENT_FacauthpubkeySetReq */
    DRV_AGENT_FACAUTHPUBKEY_SET_REQ = 0x009C,
    DRV_AGENT_IDENTIFYSTART_SET_REQ = 0x009E, /* _H2ASN_MsgChoice AT_APPCTRL */
    /* _H2ASN_MsgChoice DRV_AGENT_IdentifyendSetReq */
    DRV_AGENT_IDENTIFYEND_SET_REQ = 0x00A0,
    DRV_AGENT_PHONESIMLOCKINFO_QRY_REQ = 0x00A4, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_SIMLOCKDATAREAD_QRY_REQ  = 0x00A6, /* _H2ASN_MsgChoice AT_APPCTRL */
    /* _H2ASN_MsgChoice DRV_AGENT_PhonephynumSetReq */
    DRV_AGENT_PHONEPHYNUM_SET_REQ = 0x00A8,

    DRV_AGENT_INFORRS_SET_REQ = 0x0100, /* _H2ASN_MsgChoice AT_APPCTRL */

    DRV_AGENT_MAX_LOCK_TIMES_SET_REQ = 0x0102,

    /* _H2ASN_MsgChoice DRV_AGENT_ApSimstSetReq */
    DRV_AGENT_AP_SIMST_SET_REQ = 0x0104,

    /* _H2ASN_MsgChoice DRV_AGENT_SwverInfo */
    DRV_AGENT_SWVER_SET_REQ         = 0x0106,
    DRV_AGENT_QRY_CCPU_MEM_INFO_REQ = 0x0108,




    DRV_AGENT_SIMLOCKWRITEEX_SET_REQ     = 0x0112,
    DRV_AGENT_SIMLOCKDATAREADEX_READ_REQ = 0x0114,
#if (FEATURE_LTEV_WL == FEATURE_ON)
    DRV_AGENT_LEDTEST_SET_REQ     = 0x0116,
    DRV_AGENT_GPIOTEST_SET_REQ    = 0x0118,
    DRV_AGENT_ANTTEST_QRY_REQ     = 0x011A,
    DRV_AGENT_GPIOHEAT_SET_REQ    = 0x011E,
    DRV_AGENT_HKADCTEST_SET_REQ   = 0x0126,
    DRV_AGENT_GPIO_QRY_REQ        = 0x012A,
    DRV_AGENT_GPIO_SET_REQ        = 0x012C,
    DRV_AGENT_BOOTMODE_SET_REQ    = 0x012E,
#endif
    DRV_AGENT_GETMODEMSCID_QRY_REQ = 0x0132,

    DRV_AGENT_GETDONGLEINFO_QRY_REQ = 0x0136,
    DRV_AGENT_CHECKAUTHORITY_SET_REQ = 0x0138,
    DRV_AGENT_CONFORMAUTHORITY_SET_REQ = 0x0140,
    DRV_AGENT_NETDOGINTEGRITY_QRY_REQ = 0x0142,
    DRV_AGENT_SARDOWN_SET_REQ = 0x0144,
    DRV_AGENT_SARDOWN_QRY_REQ = 0x0146,
    /* DRV AGENT����AT����Ϣ */
    /* _H2ASN_MsgChoice DRV_AGENT_MsidQryCnf */
    DRV_AGENT_MSID_QRY_CNF = 0x0001,

    DRV_AGENT_GPIOPL_SET_CNF   = 0x000D,
    DRV_AGENT_GPIOPL_QRY_CNF   = 0x000F,
    DRV_AGENT_DATALOCK_SET_CNF = 0x0011,
    DRV_AGENT_TBATVOLT_QRY_CNF = 0x0013,
    DRV_AGENT_VERTIME_QRY_CNF  = 0x0015,
    DRV_AGENT_YJCX_SET_CNF     = 0x001F,
    DRV_AGENT_YJCX_QRY_CNF     = 0x0021,
    /* _H2ASN_MsgChoice DRV_AGENT_AppdmverQryCnf */
    DRV_AGENT_APPDMVER_QRY_CNF = 0x0023,
    /* _H2ASN_MsgChoice DRV_AGENT_DloadinfoQryCnf */
    DRV_AGENT_DLOADINFO_QRY_CNF = 0x0027,
    /* _H2ASN_MsgChoice DRV_AGENT_FlashinfoQryCnf */
    DRV_AGENT_FLASHINFO_QRY_CNF = 0x0029,
    /* _H2ASN_MsgChoice DRV_AGENT_AuthverQryCnf */
    DRV_AGENT_AUTHVER_QRY_CNF = 0x002B,
    /* _H2ASN_MsgChoice DRV_AGENT_AuthorityverQryCnf */
    DRV_AGENT_AUTHORITYVER_QRY_CNF = 0x002F,
    /* _H2ASN_MsgChoice DRV_AGENT_AuthorityidQryCnf */
    DRV_AGENT_AUTHORITYID_QRY_CNF = 0x0031,
    /* _H2ASN_MsgChoice DRV_AGENT_GodloadSetCnf */
    DRV_AGENT_GODLOAD_SET_CNF = 0x0033,
    /* _H2ASN_MsgChoice DRV_AGENT_HwnatqryQryCnf */
    DRV_AGENT_HWNATQRY_QRY_CNF      = 0x0035,
    DRV_AGENT_CPULOAD_QRY_CNF       = 0x0037, /* _H2ASN_MsgChoice AT_APPCTRL */
    DRV_AGENT_MFREELOCKSIZE_QRY_CNF = 0x0039, /* _H2ASN_MsgChoice AT_APPCTRL */

    DRV_AGENT_MEMINFO_QRY_CNF = 0x003A, /* _H2ASN_MsgChoice AT_APPCTRL */

    DRV_AGENT_HARDWARE_QRY_RSP        = 0x003D, /* �ظ�Ӳ���汾�� */
    DRV_AGENT_FULL_HARDWARE_QRY_RSP   = 0x003F, /* �ظ�������Ӳ���汾�� */
    DRV_AGENT_VERSION_QRY_CNF         = 0x0047,
    /* _H2ASN_MsgChoice DRV_AGENT_PfverQryCnf */
    DRV_AGENT_PFVER_QRY_CNF    = 0x004D,
    DRV_AGENT_FCHAN_SET_CNF    = 0x0053,
    DRV_AGENT_SDLOAD_SET_CNF   = 0x0057,
    DRV_AGENT_PRODTYPE_QRY_CNF = 0x0061,
    DRV_AGENT_DLOADVER_QRY_CNF = 0x0065,
    DRV_AGENT_RXDIV_QRY_CNF    = 0x0069,
    DRV_AGENT_SIMLOCK_SET_CNF  = 0x006B,
    /* _H2ASN_MsgChoice DRV_AGENT_ImsichgQryCnf */
    DRV_AGENT_IMSICHG_QRY_CNF = 0x0071,
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRESTORE_SET_CNF = 0x0073,
    /* _H2ASN_MsgChoice DRV_AGENT_InforbuSetCnf */
    DRV_AGENT_INFORBU_SET_CNF = 0x0075,
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRSTSTTS_QRY_CNF = 0x0077,
    /* _H2ASN_MsgChoice DRV_AGENT_CpnnTestCnf */
    DRV_AGENT_CPNN_TEST_CNF = 0x0079,
    /* _H2ASN_MsgChoice DRV_AGENT_NvbackupSetCnf */
    DRV_AGENT_NVBACKUP_SET_CNF = 0x007B,
    /* _H2ASN_MsgChoice DRV_AGENT_CpnnQryCnf */
    DRV_AGENT_CPNN_QRY_CNF = 0x007D,
    /* _H2ASN_MsgChoice DRV_AGENT_NvrestoreRst */
    DRV_AGENT_NVRESTORE_MANU_DEFAULT_CNF = 0x007F,
    DRV_AGENT_ADC_SET_CNF                = 0x0081,
    /* _H2ASN_MsgChoice DRV_AGENT_TselrfSetCnf */
    DRV_AGENT_TSELRF_SET_CNF = 0x0083,
    /* _H2ASN_MsgChoice DRV_AGENT_HkadcGetCnf */
    DRV_AGENT_HKADC_GET_CNF = 0x0085,
    DRV_AGENT_TBAT_QRY_CNF  = 0x0087,
    /* _H2ASN_MsgChoice DRV_AGENT_SecubootSetCnf */
    DRV_AGENT_SPWORD_SET_CNF   = 0x008B,

    /* _H2ASN_MsgChoice DRV_AGENT_NvbackupstatQryCnf */
    DRV_AGENT_NVBACKUPSTAT_QRY_CNF = 0x008D,
    /* _H2ASN_MsgChoice DRV_AGENT_NandbbcQryCnf */
    DRV_AGENT_NANDBBC_QRY_CNF = 0x008F,
    /* _H2ASN_MsgChoice DRV_AGENT_NandverQryCnf */
    DRV_AGENT_NANDVER_QRY_CNF = 0x0091,
    /* _H2ASN_MsgChoice DRV_AGENT_ChiptempQryCnf */
    DRV_AGENT_CHIPTEMP_QRY_CNF = 0x0093,

    /* _H2ASN_MsgChoice DRV_AGENT_HukSetCnf */
    DRV_AGENT_HUK_SET_CNF = 0x0095,
    /* _H2ASN_MsgChoice DRV_AGENT_FacauthpubkeySetCnf */
    DRV_AGENT_FACAUTHPUBKEY_SET_CNF = 0x0097,
    /* _H2ASN_MsgChoice DRV_AGENT_IdentifystartSetCnf */
    DRV_AGENT_IDENTIFYSTART_SET_CNF = 0x0099,
    /* _H2ASN_MsgChoice DRV_AGENT_IdentifyendSetCnf */
    DRV_AGENT_IDENTIFYEND_SET_CNF = 0x009B,
    /* _H2ASN_MsgChoice DRV_AGENT_SimlockdatawriteSetCnf */
    DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF = 0x009D,
    /* _H2ASN_MsgChoice DRV_AGENT_PhonesimlockinfoQryCnf */
    DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF = 0x009F,
    /* _H2ASN_MsgChoice DRV_AGENT_SimlockdatareadQryCnf */
    DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF = 0x00A1,
    /* _H2ASN_MsgChoice DRV_AGENT_PhonephynumSetCnf */
    DRV_AGENT_PHONEPHYNUM_SET_CNF = 0x00A3,
    /* _H2ASN_MsgChoice DRV_AGENT_OpwordSetCnf */
    DRV_AGENT_OPWORD_SET_CNF   = 0x00AD,
    /* _H2ASN_MsgChoice DRV_AGENT_InforrsSetCnf */
    DRV_AGENT_INFORRS_SET_CNF = 0x0101,

    /* _H2ASN_MsgChoice DRV_AGENT_MaxLockTmsSetCnf */
    DRV_AGENT_MAX_LOCK_TIMES_SET_CNF = 0x0103,

    /* _H2ASN_MsgChoice DRV_AGENT_ApSimstSetCnf */
    DRV_AGENT_AP_SIMST_SET_CNF = 0x0105,

    /* _H2ASN_MsgChoice DRV_AGENT_SwverSetCnf */
    DRV_AGENT_SWVER_SET_CNF = 0x0107,


    DRV_AGENT_NVMANUFACTUREEXT_SET_CNF = 0x010B,


    DRV_AGENT_QRY_CCPU_MEM_INFO_CNF = 0x0111,

    DRV_AGENT_SIMLOCKWRITEEX_SET_CNF       = 0x0113,
    DRV_AGENT_SIMLOCKDATAREADEX_SET_CNF    = 0x0115,
    DRV_AGENT_PHONESIMLOCKINFO_QRY_NEW_CNF = 0x0117,

#if (FEATURE_LTEV_WL == FEATURE_ON)
    DRV_AGENT_LEDTEST_SET_CNF   = 0x0119,
    DRV_AGENT_GPIOTEST_SET_CNF  = 0x011B,
    DRV_AGENT_ANTTEST_QRY_CNF   = 0x011D,
    DRV_AGENT_GPIOHEAT_SET_CNF  = 0x0121,
    DRV_AGENT_HKADCTEST_SET_CNF = 0x0129,
    DRV_AGENT_GPIO_QRY_CNF      = 0x012D,
    DRV_AGENT_GPIO_SET_CNF      = 0x012F,
    DRV_AGENT_BOOTMODE_SET_CNF  = 0x0131,
#endif
    DRV_AGENT_GETMODEMSCID_QRY_CNF = 0x0135,

    DRV_AGENT_GETDONGLEINFO_QRY_CNF = 0x0137,
    DRV_AGENT_CHECKAUTHORITY_SET_CNF = 0x0139,
    DRV_AGENT_CONFORMAUTHORITY_SET_CNF = 0x0141,
    DRV_AGENT_NETDOGINTEGRITY_QRY_CNF = 0x0143,
    DRV_AGENT_SARDOWN_QRY_CNF = 0x0145,

    DRV_AGENT_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT32 DRV_AGENT_MsgTypeUint32;


enum AT_DEVICE_CmdRatMode {
    AT_RAT_MODE_WCDMA = 0,
    AT_RAT_MODE_CDMA,
    AT_RAT_MODE_TDSCDMA,
    AT_RAT_MODE_GSM,
    AT_RAT_MODE_EDGE,
    AT_RAT_MODE_AWS,
    AT_RAT_MODE_FDD_LTE,
    AT_RAT_MODE_TDD_LTE,
    AT_RAT_MODE_WIFI,
    AT_RAT_MODE_NR,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_RAT_MODE_LTEV = 10,
#endif
    AT_RAT_MODE_BUTT
};
typedef VOS_UINT8 AT_DEVICE_CmdRatModeUint8;

/* ��ѯ����ڴ���������  */
enum AT_MEMQUERY_Para {
    AT_MEMQUERY_VOS = 0, /* ��ѯVOS�İ뾲̬�ڴ�ʵ�ʵ�ռ����� */
    AT_MEMQUERY_TTF,     /* ��ѯTTF�İ뾲̬�ڴ�ʵ�ʵ�ռ����� */
    AT_MEMQUERY_BUTT
};
typedef VOS_UINT32 AT_MEMQUERY_ParaUint32;


enum DRV_AGENT_DhKeyType {
    DRV_AGENT_DH_KEY_SERVER_PUBLIC_KEY = 0x00, /* ��������Կ */
    DRV_AGENT_DH_KEY_MODEM_PUBLIC_KEY  = 0x01, /* MODEM�๫Կ */
    DRV_AGENT_DH_KEY_MODEM_PRIVATE_KEY = 0x02, /* MODEM��˽Կ */

    DRV_AGENT_DH_KEY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_DhKeyTypeUint32;


enum DRV_AGENT_HvpdhErr {
    DRV_AGENT_HVPDH_NO_ERROR      = 0x00, /* �����ɹ� */
    DRV_AGENT_HVPDH_AUTH_UNDO     = 0x01, /* ��ʱ��δ���й����߼�Ȩ */
    DRV_AGENT_HVPDH_NV_READ_FAIL  = 0x02, /* NV��ȡʧ�� */
    DRV_AGENT_HVPDH_NV_WRITE_FAIL = 0x03, /* NVд��ʧ�� */
    DRV_AGENT_HVPDH_OTHER_ERROR   = 0x04, /* �������� */

    DRV_AGENT_HVPDH_ERR_BUTT
};
typedef VOS_UINT32 DRV_AGENT_HvpdhErrUint32;


enum AT_SimlockType {
    AT_SIMLOCK_TYPE_FAC = 0, /* ��֧��������� */
    AT_SIMLOCK_TYPE_NW  = 1, /* ֧��������� */
    AT_SIMLOCK_TYPE_OTA = 2,
    AT_SIMLOCK_TYPE_BUTT
};
typedef VOS_UINT8 AT_SimlockTypeUint8;


typedef struct {
    /* ����������״̬��ʱ�䳤�ȣ�(��λΪms),ȡֵ��Χ 0~65535,Ĭ��Ϊ5000�� */
    VOS_UINT32 standbyTime;
    /* ����ָ��PC�·������������������״̬���л�ʱ�䡣����λΪms����ȡֵ��ΧΪ 0~65535��Ĭ��Ϊ500�� */
    VOS_UINT32 switchTime;
} DRV_AGENT_PstandbyReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT8  content[4]; /* �޸������Сʱ��ͬ���޸�TAF_DEFAULT_CONTENT_LEN�� */
} DRV_AGENT_Msg;


enum GAS_AT_Cmd {
    GAS_AT_CMD_GAS_STA             = 0,  /* GAS״̬ */
    GAS_AT_CMD_NCELL               = 1,  /* ����״̬ */
    GAS_AT_CMD_SCELL               = 2,  /* ����С��״̬ */
    GAS_AT_CMD_MEASURE_DOWN        = 3,  /* �������� */
    GAS_AT_CMD_MEASURE_UP          = 4,  /* �������� */
    GAS_AT_CMD_FREQ_LOCK           = 5,  /* ��Ƶ�㣬��������ѡ */
    GAS_AT_CMD_FREQ_UNLOCK         = 6,  /* ���� */
    GAS_AT_CMD_FREQ_SPEC_SEARCH    = 7,  /* ָ��Ƶ������ */
    GAS_AT_CMD_ADD_FORB_CELL       = 8,  /* ��ӽ�ֹС�� */
    GAS_AT_CMD_DEL_FORB_CELL       = 9,  /* ɾ����ֹС�� */
    GAS_AT_CMD_DEL_HISTORY_SI_DATA = 10, /* ɾ����ʷ��Ϣ */
    GAS_AT_CMD_BUTT
};

typedef VOS_UINT8 GAS_AT_CmdUint8;


typedef struct {
    GAS_AT_CmdUint8 cmd;
    VOS_UINT8       paraNum;
    VOS_UINT16      reserve;
    VOS_UINT32      para[10];
} GAS_AT_CmdPara;


typedef struct {
    AT_APPCTRL                   atAppCtrl;                                  /* AT�û����ƽṹ */
    DRV_AGENT_MsidQryErrorUint32 result;                                     /* ��Ϣ������ */
    VOS_INT8                     modelId[TAF_MAX_MODEL_ID_LEN + 1];          /* ģ����Ϣ */
    VOS_INT8                     softwareVerId[TAF_MAX_REVISION_ID_LEN + 1]; /* ����汾��Ϣ */
    VOS_UINT8                    imei[TAF_PH_IMEI_LEN];
} DRV_AGENT_MsidQryCnf;


typedef struct {
    VOS_UINT8 loadDspMode;
    VOS_UINT8 deviceRatMode;
} DRV_AGENT_TselrfSetReq;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_AppdmverQryErrorUint32 result;
    VOS_INT8                         pdmver[AT_MAX_PDM_VER_LEN + 1];
    VOS_UINT8                        reserved1[3];
} DRV_AGENT_AppdmverQryCnf;


typedef struct {
    AT_APPCTRL                        atAppCtrl;
    DRV_AGENT_DloadinfoQryErrorUint32 result;
    VOS_UCHAR                         dlodInfo[TAF_DLOAD_INFO_LEN];
} DRV_AGENT_DloadinfoQryCnf;


typedef struct {
    AT_APPCTRL                           atAppCtrl;
    DRV_AGENT_AuthorityverQryErrorUint32 result;
    VOS_UINT8                            authority[TAF_AUTHORITY_LEN + 1];
    VOS_UINT8                            reserved1[1];
} DRV_AGENT_AuthorityverQryCnf;


typedef struct {
    AT_APPCTRL                          atAppCtrl;
    DRV_AGENT_AuthorityidQryErrorUint32 result;
    VOS_UINT8                           authorityId[TAF_AUTH_ID_LEN + 1];
    VOS_UINT8                           reserved1[1];
} DRV_AGENT_AuthorityidQryCnf;


typedef struct {
    VOS_UINT8 pfVer[TAF_MAX_PROPLAT_LEN + 1];
    VOS_CHAR  verTime[AT_AGENT_DRV_VERSION_TIME_LEN];
} DRV_AGENT_PfverInfo;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_PfverQryErrorUint32 result;
    DRV_AGENT_PfverInfo           pfverInfo;
} DRV_AGENT_PfverQryCnf;


typedef struct {
    AT_APPCTRL                     atAppCtrl;
    DRV_AGENT_SdloadSetErrorUint32 result;
} DRV_AGENT_SdloadSetCnf;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_DloadverQryErrorUint32 result;
    VOS_CHAR                         versionInfo[TAF_MAX_VER_INFO_LEN + 1];
    VOS_UINT8                        reserved1;
} DRV_AGENT_DloadverQryCnf;


typedef struct {
    AT_APPCTRL                      atAppCtrl;
    DRV_AGENT_GodloadSetErrorUint32 result;
} DRV_AGENT_GodloadSetCnf;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_HwnatqryQryErrorUint32 result;
    VOS_UINT                         netMode;
} DRV_AGENT_HwnatqryQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 curCpuLoadCnt;
    VOS_UINT32 curCpuLoad[TAF_MAX_CPU_CNT];
} DRV_AGENT_CpuloadQryCnf;


typedef struct {
    AT_APPCTRL                            atAppCtrl;
    DRV_AGENT_MfreelocksizeQryErrorUint32 result;
    VOS_INT32                             maxFreeLockSize;
} DRV_AGENT_MfreelocksizeQryCnf;


typedef struct {
    VOS_UINT32 blockCount;  /* Block���� */
    VOS_UINT32 pageSize;    /* pageҳ��С */
    VOS_UINT32 pgCntPerBlk; /* һ��Block�е�page���� */
} DRV_AGENT_DloadFlash;


typedef struct {
    AT_APPCTRL                        atAppCtrl;
    DRV_AGENT_FlashinfoQryErrorUint32 result;
    DRV_AGENT_DloadFlash              flashInfo;
} DRV_AGENT_FlashinfoQryCnf;


typedef struct {
    AT_APPCTRL                      atAppCtrl;
    DRV_AGENT_AuthverQryErrorUint32 result;
    VOS_UINT32                      simLockVersion;
} DRV_AGENT_AuthverQryCnf;


typedef struct {
    AT_APPCTRL                       atAppCtrl;
    DRV_AGENT_AppdmverQryErrorUint32 result;
} DRV_AGENT_SdloadQryCnf;


typedef struct {
    VOS_UINT8 type;        /* �������: COMP_TYPE_I */
    VOS_UINT8 mode;
    VOS_UINT8 reserved[1]; /* ����ָ�� */
    VOS_UINT8 len;         /* ���ݳ��� */
    VOS_CHAR  data[AT_MAX_VERSION_LEN];
} DRV_AGENT_VersionCtrl;


typedef struct {
    VOS_UINT16 dspBand;      /* DSP��ʽ��Ƶ��ֵ */
    VOS_UINT8  rxOnOff;      /* Rx on offֵ */
    VOS_UINT8  reserve;      /* ����λ */
    VOS_UINT32 setLowBands;  /* ת��λ���ָ�ʽ�ķּ���λ */
    VOS_UINT32 setHighBands; /* ת��λ���ָ�ʽ�ķּ���λ */
} AT_DRV_AgentRxdivSet;

typedef struct {
    AT_APPCTRL atAppCtrl;   /* ��Ϣͷ */
    VOS_UINT32 result;      /* ���ؽ�� */
    VOS_UINT8  rxOnOff;     /* RX�Ƿ��Ѿ��� */
    VOS_UINT8  reserve;     /* ����λ */
    VOS_UINT16 setDivBands; /* Ҫ���õ�BAND */
} DRV_AGENT_AtRxdivCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;     /* ��Ϣͷ */
    VOS_UINT32 result;        /* ���ؽ�� */
    VOS_UINT16 drvDivBands;   /* ֧�ֵ�BAND */
    VOS_UINT16 curBandSwitch; /* ��ǰ���õ�BAND */
} DRV_AGENT_QryRxdivCnf;


typedef struct {
    VOS_UINT32 pwdLen;
    VOS_UINT8  pwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 4];
} DRV_AGENT_SimlockSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32 result;    /* ���ؽ�� */
} DRV_AGENT_SetSimlockCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;                             /* ��Ϣͷ */
    VOS_UINT32 result;                                /* ���ؽ�� */
    VOS_UINT8  hwVer[DRV_AGENT_MAX_HARDWARE_LEN + 1]; /* Ӳ���汾�� */
} DRV_AGENT_HardwareQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;                                     /* ��Ϣͷ */
    VOS_UINT32 result;                                        /* ���ؽ�� */
    VOS_UINT8  modelId[DRV_AGENT_MAX_MODEL_ID_LEN + 1];       /* MODE ID�� */
    VOS_UINT8  revisionId[DRV_AGENT_MAX_REVISION_ID_LEN + 1]; /* ����汾�� */
    VOS_UINT8  hwVer[DRV_AGENT_MAX_HARDWARE_LEN + 1];         /* Ӳ���汾�� */
} DRV_AGENT_FullHardwareQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32 result;    /* ���ؽ�� */
} DRV_AGENT_NvrestoreRst;

#define AT_SD_DATA_UNIT_LEN (512)
#define AT_GPIOPL_MAX_LEN (20)


typedef struct {
    VOS_UINT32 gpioOper;
    VOS_UINT8  gpiopl[AT_GPIOPL_MAX_LEN];

} DRV_AGENT_GpioplSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;

} DRV_AGENT_GpioplSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  gpiopl[AT_GPIOPL_MAX_LEN];

} DRV_AGENT_GpioplQryCnf;


typedef struct {
    VOS_UINT8 pwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 1];

} DRV_AGENT_DatalockSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail; /* DATALOCKУ���Ƿ�ɹ� */

} DRV_AGENT_DatalockSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_INT32  batVol;

} DRV_AGENT_TbatvoltQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;

} DRV_AGENT_TmodeSetCnf;


enum DRV_AGENT_VersionQryError {
    DRV_AGENT_VERSION_QRY_NO_ERROR      = 0,
    DRV_AGENT_VERSION_QRY_VERTIME_ERROR = 1,

    DRV_AGENT_VERSION_QRY_BUTT
};
typedef VOS_UINT32 DRV_AGENT_VersionQryErrorUint32;


typedef struct {
    AT_APPCTRL                      atAppCtrl;
    TAF_PH_RevisionId               softVersion;                            /* ����汾�� */
    TAF_PH_HwVer                    fullHwVer;                              /* �ⲿӲ���汾�� */
    TAF_PH_HwVer                    interHwVer;                             /* �ڲ�Ӳ���汾�� */
    TAF_PH_ModelId                  modelId;                                /* �ⲿ��ƷID */
    TAF_PH_ModelId                  interModelId;                           /* �ڲ���ƷID */
    TAF_PH_CdromVer                 isoVer;                                 /* ��̨����汾�� */
    DRV_AGENT_VersionQryErrorUint32 result;                                 /* ������ö�� */
    VOS_CHAR                        verTime[AT_AGENT_DRV_VERSION_TIME_LEN]; /* ����ʱ�� */

} DRV_AGENT_VersionQryCnf;

#ifndef MBB_COMMON

enum AT_FEATURE_DrvType {
    AT_FEATURE_LTE = 0,
    AT_FEATURE_HSPAPLUS,
    AT_FEATURE_HSDPA,
    AT_FEATURE_HSUPA,
    AT_FEATURE_DIVERSITY,
    AT_FEATURE_UMTS,
    AT_FEATURE_EVDO,
    AT_FEATURE_EDGE,
    AT_FEATURE_GPRS,
    AT_FEATURE_GSM,
    AT_FEATURE_CDMA,
    AT_FEATURE_WIMAX,
    AT_FEATURE_WIFI,
    AT_FEATURE_GPS,
    AT_FEATURE_TDSCDMA,

    AT_FEATURE_NR,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_FEATURE_LTEV = 16,
#endif
};
typedef VOS_UINT32 AT_FEATURE_DrvTypeUint32;
#endif


typedef struct {
    VOS_UINT8 powerClass;       /* UE���ʼ��� */
    VOS_UINT8 txRxFreqSeparate; /* Tx/Rx Ƶ������ */
    VOS_UINT8 reserve1[2];      /* 4�ֽڶ��룬���� */
} AT_RF_Capa;


typedef struct {
    VOS_UINT32 hspaStatus; /* 0��ʾδ����,��ôDPA��UPA��֧��;1��ʾ���� */
    VOS_UINT8  macEhsSupport;
    VOS_UINT8  hsdschPhyCat;
    VOS_UINT8  reserve1[2];       /* ����һ��������ulHspaStatus���п��ƣ��˱�������ʹ�� */
    AT_RF_Capa rfCapa;            /* RF ������Ϣ */
    VOS_UINT8  dlSimulHsDschCfg;  /* ENUMERATED  OPTIONAL */
    VOS_UINT8  asRelIndicator;    /* Access Stratum Release Indicator */
    VOS_UINT8  hsdschPhyCategory; /* ֧��HS-DSCH���������ͱ�־ */
    VOS_UINT8  edchSupport;       /* �Ƿ�֧��EDCH�ı�־ */
    VOS_UINT8  edchPhyCategory;   /* ֧��UPA�ĵȼ� */
    VOS_UINT8  fdpchSupport;      /* �Ƿ�֧��FDPCH�ı�־ */
    VOS_UINT8  v690Container;     /* V690Container��д */
    VOS_UINT8  hsdschSupport;     /* �Ƿ�֧��enHSDSCHSupport�ı�־ */
} AT_UE_Capa;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
} DRV_AGENT_SecubootSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  secuBootEnable;
    VOS_UINT8  reserve1[3]; /* 4�ֽڶ��룬���� */
} DRV_AGENT_SecubootQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;
    VOS_UINT8  secuBootFeature;
    VOS_UINT8  reserve1[7]; /* 4�ֽڶ��룬���� */
} DRV_AGENT_SecubootfeatureQryCnf;


enum AT_DEVICE_CmdBand {
    AT_BAND_2100M = 0,
    AT_BAND_1900M,
    AT_BAND_1800M,
    AT_BAND_1700M,
    AT_BAND_1600M,
    AT_BAND_1500M,
    AT_BAND_900M,
    AT_BAND_850M,
    AT_BAND_800M,
    AT_BAND_450M,
    AT_BAND_2600M,
    AT_BAND_1X_BC0,
    AT_BAND_WIFI = 15,
    AT_BAND_BUTT
};
typedef VOS_UINT8 AT_DEVICE_CmdBandUint8;


typedef struct {
    VOS_UINT16 ulArfcn; /* ���е�Channel No */
    VOS_UINT16 dlArfcn; /* ���е�Channel No */
    VOS_UINT16 dspBand; /* DSP��ʽ��Ƶ��ֵ */
    VOS_UINT8  reserved[2];
} AT_DSP_BandArfcn;


typedef struct {
    VOS_UINT16 arfcnMin;
    VOS_UINT16 arfcnMax;
} AT_ARFCN_Range;


enum AT_TMODE {
    AT_TMODE_NORMAL = 0, /* ����ģʽ,��������״̬ */
    AT_TMODE_FTM,        /* ������ģʽ,У׼ģʽ */
    AT_TMODE_UPGRADE,    /* ����ģʽ,�汾����ʱʹ�� */
    AT_TMODE_RESET,      /* ��������  */
    AT_TMODE_POWEROFF,
    AT_TMODE_SIGNALING = 11,
    AT_TMODE_OFFLINE,
#if ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_UE_MODE_TDS))
    AT_TMODE_F_NONESIGNAL   = 13,
    AT_TMODE_SYN_NONESIGNAL = 14,
    AT_TMODE_SET_SECONDARY  = 15,
    AT_TMODE_GU_BT          = 16,
    AT_TMODE_TDS_FAST_CT    = 17,
    AT_TMODE_TDS_BT         = 18,
    AT_TMODE_COMM_CT        = 19,
#endif
    AT_TMODE_BUTT
};
typedef VOS_UINT8 TAF_PH_TmodeUint8;


typedef struct {
    VOS_UINT8                 loadDspMode;
    VOS_UINT8                 currentDspMode;
    VOS_UINT16                channelNo;
    AT_DEVICE_CmdRatModeUint8 deviceRatMode;
    AT_DEVICE_CmdBandUint8    deviceAtBand;
    VOS_UINT8                 reserved[2];
    VOS_BOOL                  dspLoadFlag;
    AT_DSP_BandArfcn          dspBandArfcn;
} DRV_AGENT_FchanSetReq;


enum DRV_AGENT_FchanSetError {
    DRV_AGENT_FCHAN_SET_NO_ERROR           = 0,
    DRV_AGENT_FCHAN_BAND_NOT_MATCH         = 1,
    DRV_AGENT_FCHAN_BAND_CHANNEL_NOT_MATCH = 2,
    DRV_AGENT_FCHAN_OTHER_ERR              = 3,

    DRV_AGENT_FCHAN_SET_BUTT
};
typedef VOS_UINT32 DRV_AGENT_FchanSetErrorUint32;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_FchanSetReq         fchanSetReq;
    DRV_AGENT_FchanSetErrorUint32 result;

} DRV_AGENT_FchanSetCnf;


#define AT_MS_SUPPORT_RX_DIV_2100 0x0001
#define AT_MS_SUPPORT_RX_DIV_1900 0x0002
#define AT_MS_SUPPORT_RX_DIV_1800 0x0004
#define AT_MS_SUPPORT_RX_DIV_AWS_1700 0x0008
#define AT_MS_SUPPORT_RX_DIV_850 0x0010
#define AT_MS_SUPPORT_RX_DIV_800 0x0020
#define AT_MS_SUPPORT_RX_DIV_2600 0x0040
#define AT_MS_SUPPORT_RX_DIV_900 0x0080
#define AT_MS_SUPPORT_RX_DIV_IX_1700 0x0100


enum AT_DSP_RfOnOff {
    AT_DSP_RF_SWITCH_OFF = 0,
    AT_DSP_RF_SWITCH_ON,
    AT_DSP_RF_SWITCH_BUTT
};
typedef VOS_UINT8 AT_DSP_RfOnOffUint8;


enum AT_DSP_CltEnable {
    AT_DSP_CLT_DISABLE = 0,
    AT_DSP_CLT_ENABLE,

    AT_DSP_CLT_BUTT
};
typedef VOS_UINT8 AT_DSP_CltEnableUint8;


enum AT_DCXOTEMPCOMP_Enable {
    AT_DCXOTEMPCOMP_DISABLE = 0,
    AT_DCXOTEMPCOMP_ENABLE,

    AT_DCXOTEMPCOMP_BUTT
};
typedef VOS_UINT8 AT_DCXOTEMPCOMP_EnableUint8;

typedef struct {
    VOS_PID    pid;
    VOS_UINT32 msgPeakSize;
    VOS_UINT32 memPeakSize;
} AT_PID_MemInfoPara;


typedef struct {
    VOS_UINT32       setLowBands;
    VOS_UINT32       setHighBands;
    AT_DSP_BandArfcn dspBandArfcn;
} DRV_AGENT_RxpriSetReq;


typedef struct {
    AT_APPCTRL            atAppCtrl;
    DRV_AGENT_ErrorUint32 result;
    DRV_AGENT_RxpriSetReq rxpriSetReq;
    VOS_UINT16            wPriBands;
    VOS_UINT16            gPriBands;
    VOS_UINT16            setPriBands;
    VOS_UINT8             reserved[2];
} DRV_AGENT_RxpriSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 prodType;

} DRV_AGENT_ProdtypeQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 tbatType;

} DRV_AGENT_TbatQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT16 wDrvPriBands;
    VOS_UINT16 gDrvPriBands;
    VOS_BOOL   fail;
} DRV_AGENT_RxpriQryCnf;


typedef struct {
    VOS_UINT32 isEnable;           /* �Ƿ���Ҫ������������, 0Ϊ�رգ�1Ϊ���� */
    VOS_INT32  closeAdcThreshold;  /* ����͹���ģʽ���¶����� */
    VOS_INT32  alarmAdcThreshold;  /* ��Ҫ���и����������¶����� */
    VOS_INT32  resumeAdcThreshold; /* �ָ�������ģʽ���¶����� */
} SPY_TempThresholdPara;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   fail;

} DRV_AGENT_AdcSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT8  len;
    VOS_UINT8  reserved[2];
    VOS_CHAR   data[AT_AGENT_DRV_VERSION_TIME_LEN + 1];
} DRV_AGENT_VersionTime;

/*
 * �ṹ˵��: AT��AT AGENT AT^YJCX��������ظ��ṹ
 */
typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    VOS_UCHAR  flashInfo[TAF_MAX_FLAFH_INFO_LEN + 1];
} DRV_AGENT_YjcxSetCnf;

/*
 * �ṹ˵��: AT��AT AGENT AT^YJCX��������ظ��ṹ
 */
typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    VOS_UCHAR  gpioInfo[TAF_MAX_GPIO_INFO_LEN + 1];
    VOS_UINT8  reserved[3];
} DRV_AGENT_YjcxQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 curImsiSign;
    VOS_UINT16 dualIMSIEnable;
    VOS_UINT8  reserved[2];
} DRV_AGENT_ImsichgQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_InforbuSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_InforrsSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   normalSrvStatus;
    VOS_UINT32 pnnExistFlg;
    VOS_UINT32 oplExistFlg;

} DRV_AGENT_CpnnTestCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_NvbackupSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_BOOL   normalSrvStatus;
} DRV_AGENT_CpnnQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
    /* ��ѯ�����ͣ�0:VOS�ڴ�, 1: TTF�ڴ� */
    AT_MEMQUERY_ParaUint32 memQryType;
    VOS_UINT32             pidNum;  /* PID�ĸ��� */
    VOS_UINT8              data[4]; /* ÿ��PID���ڴ�ʹ�����,�޸������Сʱ��ͬ���޸�TAF_DEFAULT_CONTENT_LEN�� */
} DRV_AGENT_MeminfoQryRsp;


typedef struct {
    AT_APPCTRL                     atAppCtrl;
    DRV_AGENT_TselrfSetErrorUint32 result;
    VOS_UINT8                      deviceRatMode;
    VOS_UINT8                      reserved[3];
} DRV_AGENT_TselrfSetCnf;


typedef struct {
    AT_APPCTRL                    atAppCtrl;
    DRV_AGENT_HkadcGetErrorUint32 result;
    VOS_INT32                     tbatHkadc;
} DRV_AGENT_HkadcGetCnf;

#if (FEATURE_ON == FEATURE_SECURITY_SHELL)

typedef struct {
    VOS_CHAR shellPwd[AT_SHELL_PWD_LEN];
} DRV_AGENT_SpwordSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_SpwordSetCnf;
#endif


typedef struct {
    AT_APPCTRL atAppCtrl;    /* ��Ϣͷ */
    VOS_UINT32 result;       /* ���ؽ�� */
    VOS_UINT32 nvBackupStat; /* NV����״̬ */
} DRV_AGENT_NvbackupstatQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;         /* ��Ϣͷ */
    VOS_UINT32 result;            /* ���ؽ�� */
    VOS_UINT32 badBlockNum;       /* �������� */
    VOS_UINT32 badBlockIndex[0];  //lint !e43 /* ���������� */
} DRV_AGENT_NandbbcQryCnf;


#define DRV_AGENT_NAND_MFU_NAME_MAX_LEN 16 /* ����������󳤶� */
#define DRV_AGENT_NAND_DEV_SPEC_MAX_LEN 32 /* �豸�����󳤶� */

typedef struct {
    VOS_UINT32 mufId;                                    /* ����ID */
    VOS_UINT8  mufName[DRV_AGENT_NAND_MFU_NAME_MAX_LEN]; /* ���������ַ��� */
    VOS_UINT32 devId;                                    /* �豸ID */
    VOS_UINT8  devSpec[DRV_AGENT_NAND_DEV_SPEC_MAX_LEN]; /* �豸����ַ��� */
} DRV_AGENT_NandDevInfo;


typedef struct {
    AT_APPCTRL            atAppCtrl;   /* ��Ϣͷ */
    VOS_UINT32            result;      /* ���ؽ�� */
    DRV_AGENT_NandDevInfo nandDevInfo; /* �豸��Ϣ */
} DRV_AGENT_NandverQryCnf;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32 result;    /* ���ؽ�� */
    VOS_INT    gpaTemp;   /* G PA�¶� */
    VOS_INT    wpaTemp;   /* W PA�¶� */
    VOS_INT    lpaTemp;   /* L PA�¶� */
    VOS_INT    simTemp;   /* SIM���¶� */
    VOS_INT    batTemp;   /* ����¶� */
} DRV_AGENT_ChiptempQryCnf;



typedef struct {
    AT_APPCTRL atAppCtrl; /* ��Ϣͷ */
    VOS_UINT16 antState;
    VOS_UINT8  rsv[2];
} DRV_AGENT_AntStateInd;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_MaxLockTmsSetCnf;


typedef struct {
    VOS_UINT32 usimState;
} DRV_AGENT_ApSimstSetReq;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 result;
} DRV_AGENT_ApSimstSetCnf;


typedef struct {
    VOS_UINT8 huk[DRV_AGENT_HUK_LEN]; /* HUK���� 128Bits */
} DRV_AGENT_HukSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_HukSetCnf;


typedef struct {
    /* ��Ȩ��Կ���� */
    VOS_UINT8 pubKey[DRV_AGENT_PUBKEY_LEN];
    /* ��Ȩ��ԿSSKǩ������ */
    VOS_UINT8 pubKeySign[DRV_AGENT_PUBKEY_SIGNATURE_LEN];
} DRV_AGENT_FacauthpubkeySetReq;


typedef struct {
    /* 2:OTA�·� 1: �����·� 0: �����·� */
    AT_SimlockTypeUint8 netWorkFlg;
    VOS_UINT8           layer;
    VOS_UINT8           reserve[2];
    VOS_UINT32          total; /* �ܹ��ּ���д�� */
    VOS_UINT32          hmacLen;
    VOS_UINT32          simlockDataLen;
    VOS_UINT8           hmac[DRV_AGENT_HMAC_DATA_LEN];
    VOS_UINT8           simlockData[4]; /* �޸������Сʱ��ͬ��֪���޸�TAF_DEFAULT_CONTENT_LEN�� */
} DRV_AGENT_SimlockwriteexSetReq;


typedef struct {
    VOS_UINT32                         total;     /* д��Ĵ��� */
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_SimlockwriteexSetCnf;


typedef struct {
    VOS_UINT8 layer;    /* �ڼ������� */
    VOS_UINT8 indexNum; /* �ڼ������� */
} DRV_AGENT_SimlockdatareadexReadReq;


typedef struct {
    AT_APPCTRL atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32 result;
    VOS_UINT8  layer;    /* �ڼ������� */
    VOS_UINT8  indexNum; /* �ڼ������� */
    VOS_UINT8  total;    /* ���ּ������� */
    VOS_UINT8  reserve;
    VOS_UINT32 dataLen; /* ���ݰ��ĳ��� */
    VOS_UINT8  data[4]; /* ���� */
} DRV_AGENT_SimlockdatareadexReadCnf;


typedef struct {
    VOS_UINT8 para[DRV_AGENT_PUBKEY_LEN];
} DRV_AGENT_FacauthpubkeyexSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_FacauthpubkeySetCnf;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
    /* ʹ�ù�Կ����RSA���ܺ������ */
    VOS_UINT8  rsaText[DRV_AGENT_RSA_CIPHERTEXT_LEN];
    VOS_UINT32 publicId[TAF_OTA_SIMLOCK_PUBLIC_ID_NUM];
    VOS_UINT8  otaFlag; /* �Ƿ�OTA��Ȩ:0-��OTA,1-OTA */
    VOS_UINT8  rsv[3];
} DRV_AGENT_IdentifystartSetCnf;


typedef struct {
    /* ʹ��˽Կ����RSA���ܺ�����ĳ��� */
    VOS_UINT32 rsaTextlen;
    /* ʹ��˽Կ����RSA���ܺ������ */
    VOS_UINT8  rsaText[DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN];
} DRV_AGENT_IdentifyendSetReq;


typedef struct {
    /* ʹ��˽Կ����RSA���ܺ������ */
    DRV_AGENT_IdentifyendSetReq rsa;
    VOS_UINT8                   otaFlag;
    VOS_UINT8                   rsv[3];
} DRV_AGENT_IdentifyendOtaSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_IdentifyendSetCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_SimlockdatawriteSetCnf;


typedef struct {
    VOS_UINT8 phLockCodeBegin[DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED];
    VOS_UINT8 phLockCodeEnd[DRV_AGENT_PH_LOCK_CODE_LEN_EXTERNED];
} DRV_AGENT_PhLockCodeNew;


typedef struct {
    VOS_UINT8 phLockCodeBegin[DRV_AGENT_PH_LOCK_CODE_LEN];
    VOS_UINT8 phLockCodeEnd[DRV_AGENT_PH_LOCK_CODE_LEN];
} DRV_AGENT_PhLockCode;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ����������category��� */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ���������ļ���ָʾ */
    DRV_AGENT_PersonalizationStatusUint8    status;
    VOS_UINT8                               maxUnlockTimes;    /* �������������������� */
    VOS_UINT8                               remainUnlockTimes; /* ����������ʣ��������� */
    VOS_UINT8                               rsv[3];            /* �����ֽڣ��������ֽڶ��� */
} DRV_AGENT_SimlockDataCategory;


typedef struct {
    AT_APPCTRL                         atAppCtrl;          /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;             /* ����ִ�н�� */
    VOS_UINT8                          supportCategoryNum; /* ��ǰcategory���� */
    VOS_UINT8                          rsv[3];             /* �����ֽڣ��������ֽڶ��� */

    /* Ŀǰ֧��3��category���ṹ�����鰴��network->network subset->SP��˳������ */
    DRV_AGENT_SimlockDataCategory categoryData[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_SimlockdatareadQryCnf;

/*
 * ����˵��: �ṩ��TAF����AT^GETMODEMSCID���ڻ�ȡ����SCʹ����ԿID���������ݽṹ
 */
typedef struct {
    DRV_AGENT_EncryptRsaAlgorithmUint32 scAlgorithm;
    VOS_UINT32                          scEncryptIdNum;
    VOS_UINT32                          scEncryptId[DRV_AGENT_SIMLOCK_ENCRYPT_KEYID_NUM];
} DRV_AGENT_SimlockEncryptIdInfo;

/*
 * �ṹ˵��: AT��DRV AGENT��ѯ����������ȫ���ݵ���Ϣ�ظ��ṹ
 */
typedef struct {
    AT_APPCTRL                                atAppCtrl;          /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32        result;             /* ����ִ�н�� */
    DRV_AGENT_SimlockEncryptIdInfo            scEncryptIdInfo;
} DRV_AGENT_GetSimlockEncryptIdQryCnf ;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
    /* Ŀǰ֧��4��category���ṹ�����鰴��network->network subset->SP->CP��˳������ */
    DRV_AGENT_SimlockDataCategory categoryData[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_SimlockdatareadQryCnfEx;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ����������category��� */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ���������ļ���ָʾ */
    VOS_UINT8                               flag;      /* 0: Trust���� 1: Block���� */
    VOS_UINT8                               rsv;       /* 4�ֽڶ���, ����λ */
    /* �Ŷθ�����һ��begin/end��һ���Ŷ� */
    VOS_UINT32 groupNum;
    /* ���������ĺŶ����� */
    DRV_AGENT_PhLockCodeNew lockCode[DRV_AGENT_PH_LOCK_CODE_GROUP_NUM_EXTERED];
} DRV_AGENT_SimlockInfoCategoryNew;


typedef struct {
    DRV_AGENT_PersonalizationCategoryUint8  category;  /* ����������category��� */
    DRV_AGENT_PersonalizationIndicatorUint8 indicator; /* ���������ļ���ָʾ */
    /* �Ŷθ�����һ��begin/end��һ���Ŷ� */
    VOS_UINT8 groupNum;
    VOS_UINT8 rsv; /* 4�ֽڶ���, ����λ */
    /* ���������ĺŶ����� */
    DRV_AGENT_PhLockCode lockCode[DRV_AGENT_PH_LOCK_CODE_GROUP_NUM];
} DRV_AGENT_SimlockInfoCategory;


typedef struct {
    AT_APPCTRL                         atAppCtrl;          /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;             /* ����ִ�н�� */
    VOS_UINT8                          supportCategoryNum; /* ��ǰcategory���� */
    VOS_UINT8                          reserve[3];
    DRV_AGENT_SimlockInfoCategoryNew   categoryInfo[DRV_AGENT_SUPPORT_CATEGORY_NUM_EXTERED];
} DRV_AGENT_PhonesimlockinfoQryCnfNew;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */

    /* Ŀǰ֧��3��category���ṹ�����鰴��network->network subset->SP��˳������ */
    DRV_AGENT_SimlockInfoCategory categoryInfo[DRV_AGENT_SUPPORT_CATEGORY_NUM];
} DRV_AGENT_PhonesimlockinfoQryCnf;


typedef struct {
    DRV_AGENT_PhPhynumTypeUint8 phynumType; /* ��������� */
    VOS_UINT8  reserve[3];
    VOS_UINT8  phynumValue[DRV_AGENT_RSA_TLV_CIPHERTEXT_LEN]; /* �����RSA�������� */
    VOS_UINT32 rsaIMEILen;                                /* Rsa���ܺ�����ĳ��� */
    VOS_UINT8  hmacData[DRV_AGENT_HMAC_DATA_LEN];         /* hmac���� */
    VOS_UINT32 hmacLen;                                   /* hmacǩ������ */
} DRV_AGENT_PhonephynumSetReq;


typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_PhonephynumSetCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_PortattribsetSetCnf;


typedef struct {
    AT_APPCTRL                         atAppCtrl;  /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;     /* ����ִ�н�� */
    DRV_AGENT_PortStatusUint32         portStatus; /* ͨ�Ŷ˿�״̬ */
} DRV_AGENT_PortattribsetQryCnf;



typedef struct {
    AT_APPCTRL                         atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_PersonalizationErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_OpwordSetCnf;


typedef struct {
    DRV_AGENT_DhKeyTypeUint32 keyType; /* DH��Կ���� */
    VOS_UINT32                keyLen;  /* DH��Կ���� */
    /* DH��Կ������󳤶����������飬�Ա����ڴ洢��Կ��˽Կ */
    VOS_UINT8 key[DRV_AGENT_DH_PUBLICKEY_LEN];
} DRV_AGENT_HvpdhReq;


typedef struct {
    AT_APPCTRL               atAppCtrl; /* ��Ϣͷ */
    DRV_AGENT_HvpdhErrUint32 result;    /* ����ִ�н�� */
} DRV_AGENT_HvpdhCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
} DRV_AGENT_NvmanufactureextSetCnf;


typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 chkPoolLeakRslt;
    VOS_UINT32 chkNodeLeakRslt;
    VOS_UINT32 chkRrmNodeLeakRslt;
} DRV_AGENT_QryCcpuMemInfoCnf;

#if (FEATURE_LTEV_WL == FEATURE_ON)

typedef struct {
    VOS_UINT8 reserve;
    VOS_UINT8 ledTest;
} DRV_AGENT_LedTestSetReq;


typedef struct {
    AT_APPCTRL      atAppCtrl;
    VOS_BOOL        isFail;
} DRV_AGENT_LedTestSetCnf;

typedef struct {
    VOS_UINT8 reserve[3];
    VOS_UINT8 gpioTest;
} DRV_AGENT_GpioTestSetReq;


typedef struct {
    VOS_UINT8 reserve[3];
    VOS_UINT8 hkadcChannel;
} DRV_AGENT_HkadctestSetReq;


typedef struct {
    VOS_UINT16 gpio;
    VOS_UINT16 rev;
} DRV_AGENT_GpioQryReq;


typedef struct {
    VOS_UINT16 gpio;
    VOS_UINT16 value;
} DRV_AGENT_GpioSetReq;


typedef struct {
    AT_APPCTRL      atAppCtrl;
    VOS_BOOL        isFail;
} DRV_AGENT_BootModeSetCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl;
    VOS_BOOL        isFail;
} DRV_AGENT_GpioTestSetCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl; /* ��Ϣͷ */
    VOS_UINT8       reserve[2];
    VOS_UINT8       result;       /* ���ؽ�� */
    VOS_UINT8       laAntState;   /* LTE A����״̬ */
    VOS_UINT8       lbAntState;   /* LTE B����״̬ */
    VOS_UINT8       lvaAntState;  /* LTE-V A����״̬ */
    VOS_UINT8       lvbAntState;  /* LTE-V B����״̬ */
    VOS_UINT8       gnssAntState; /* GNSS����״̬ */
} DRV_AGENT_AntTestQryCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32      result;    /* ���ؽ�� */
    VOS_UINT32      wifiState; /* USB WIFI���µ�״̬ */
    VOS_BOOL        succ;      /* �Ƿ�ɹ� */
} DRV_AGENT_UsbTestSetCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl; /* ��Ϣͷ */
    VOS_INT32       result;    /* ���ؽ�� */
    VOS_INT32       temp;      /* �¶� */
    VOS_UINT32      volt;      /* ��ѹ */
} DRV_AGENT_HkadctestSetCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl; /* ��Ϣͷ */
    VOS_UINT16      result;    /* ���ؽ�� */
    VOS_UINT16      value;     /* ��ƽֵ */
} DRV_AGENT_GpioQryCnf;


typedef struct {
    AT_APPCTRL      atAppCtrl; /* ��Ϣͷ */
    VOS_UINT32      result;    /* ���ؽ�� */
} DRV_AGENT_GpioSetCnf;

#endif
/*
 * H2ASN������Ϣ�ṹ����
 */
typedef struct {
    DRV_AGENT_MsgTypeUint32 msgId; /* _H2ASN_MsgChoice_Export DRV_AGENT_MsgTypeUint32  */
    VOS_UINT8               msgBlock[4];
    /*
     * _H2ASN_MsgChoice_When_Comment          DRV_AGENT_MsgTypeUint32
     */
} DRV_AGENT_MsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    DRV_AGENT_MsgData msgData;
} TafDrvAgent_Msg;


typedef struct {
    VOS_UINT8 swVer[TAF_MAX_REVISION_ID_LEN + 1];
    VOS_CHAR  verTime[AT_AGENT_DRV_VERSION_TIME_LEN];
} DRV_AGENT_SwverInfo;


typedef struct {
    AT_APPCTRL            atAppCtrl;
    DRV_AGENT_ErrorUint32 result;
    DRV_AGENT_SwverInfo   swverInfo;
} DRV_AGENT_SwverSetCnf;


typedef struct {
    VOS_UINT8 drvAgentSystemAppConfigAddr; /* DrvAgentģ�鱣��ƽ̨���� */
    VOS_UINT8 gcfInd;                      /* ָʾ��ǰ�Ƿ�ΪGCF���� */
    VOS_UINT8 simLockErrTimes;             /* ͳ��SIMLOCK������������Ĵ��� */
    VOS_UINT8 reserve[5];                  /* PCLINT-64 ��Ҫ8�ֽڶ��� */
} DRV_AGENT_Context;

typedef struct {
    VOS_UINT8 randomData[DRV_AGENT_NETDOG_AUTH_RANDOM_LEN];
} DRV_AGENT_CheckAuthoritySetReq;

typedef struct {
    VOS_UINT8 rsa[DRV_AGENT_RSA2048_ENCRYPT_LEN];
} DRV_AGENT_ConfirmAuthoritySetReq;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_INT32 result;
    VOS_UINT32 rsa[DRV_AGENT_RSA2048_ENCRYPT_LEN];
} DRV_AGENT_CheckAuthoritySetCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_INT32 result;
} DRV_AGENT_ConfirmAuthoritySetCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_INT32 result;
    VOS_INT32 integrity;
} DRV_AGENT_NetDogIntegrityQryCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_INT32 result;
    VOS_UINT32 ids[3]; /* NETDOG��3����Կ */
} DRV_AGENT_GetDongleInfoQryCnf;

typedef struct {
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 sarDownState;
    VOS_INT32 result;
} DRV_AGENT_SarDownQryCnf;

extern DRV_AGENT_Context* DRVAGENT_GetCtxAddr(VOS_VOID);

extern VOS_VOID   MMA_GetProductionVersion(VOS_CHAR *pcDest);
extern VOS_UINT32 At_SendRfCfgAntSelToHPA(VOS_UINT8 divOrPriOn, VOS_UINT8 index);
extern VOS_UINT32 At_DelCtlAndBlankCharWithEndPadding(VOS_UINT8 *data, VOS_UINT16 *cmdLen);
extern VOS_UINT32 Spy_SetTempPara(const SPY_TempThresholdPara *tempPara);

#if (VOS_RTOSCK == VOS_OS_VER)
extern VOS_UINT32 SRE_CpuUsageGet(VOS_UINT32 coreId);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of TafDrvAgent.h */
