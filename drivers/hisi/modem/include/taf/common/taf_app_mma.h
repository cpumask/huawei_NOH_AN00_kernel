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

#ifndef __TAF_APP_MMA_H__
#define __TAF_APP_MMA_H__

#include "taf_type_def.h"
#include "usimm_ps_interface.h"

#include "at_mn_interface.h"

#include "app_vc_api.h"

#include "mn_call_api.h"
#include "nv_stru_gucnas.h"
#include "product_config.h"

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
#include "nas_comm_packet_ser.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define TAF_MMA_PNN_INFO_MAX_NUM (16) /* ATģ��Ҫ��MMAһ���ϱ���PNN���� */

/* MNC����󳤶�Ϊ3λ��ʵ�ʳ���Ϊ2λ��3λ����ѡ */
#define TAF_MMA_PLMN_MNC_TWO_BYTES (2)

/* ���������Ⱥ�TAF_PH_OPER_NAME_LONG��40��չ��100��A��һ�β�ѯ�ϱ�����Ӫ��������Ϣ��Ҫ���� */
/* ATģ��Ҫ��MMAһ���ϱ�����Ӫ�̸��� */
#define TAF_MMA_COPN_PLMN_MAX_NUM (16)

#define TAF_MMA_NET_SCAN_MAX_FREQ_NUM (20)

#define TAF_MMA_REG_MAX_PLMN_NUM (8)
#define TAF_MMA_REG_MAX_CELL_NUM (8)

#define TAF_MMA_MAX_CDMA_PN_NUM (511)
#define TAF_MMA_WILDCARD_CDMA_PN_NUM (0xFFFF)
#define TAF_MMA_MAX_EVDO_PN_NUM (511)
#define TAF_MMA_WILDCARD_EVDO_PN_NUM (0xFFFF)

#define TAF_MMA_SRV_ACQ_RAT_NUM_MAX (3)

#define TAF_MMA_MAX_TRUST_LOCK_SID_NUM (5)
#define TAF_MMA_MAX_ANT_NUM (4)
#define TAF_MMA_MAX_ANT_NUM_EX (8)

#define TAF_MMA_VERSION_INFO_LEN (9)
#define TAF_MMA_MAX_EHPLMN_NUM (17)
#define TAF_MMA_MAX_DPLMN_NUM (256)
#define TAF_MMA_SIM_FORMAT_PLMN_LEN (3)
#define TAF_MMA_MAX_STR_LEN (1600)
#define TAF_MMA_MAX_EHPLMN_STR (128)

#define TAF_MMA_MAX_CFG_BORDER_NUM (256U)

/* CSG list��������ϱ�8������csg��Ϣʱ��С2k */
#define TAF_MMA_MAX_CSG_ID_LIST_NUM (8)

#if (FEATURE_ON == FEATURE_CSG)
#define TAF_MMA_MAX_HOME_NODEB_NAME_LEN (48)
#define TAF_MMA_MAX_CSG_TYPE_LEN (12)
#else
#define TAF_MMA_MAX_HOME_NODEB_NAME_LEN (4)
#define TAF_MMA_MAX_CSG_TYPE_LEN (4)
#endif

#define TAF_MMA_INVALID_CSG_ID_VALUE (0xFFFFFFFF)
#define TAF_MMA_INVALID_MCC (0xFFFFFFFF)
#define TAF_MMA_INVALID_MNC (0xFFFFFFFF)

#define TAF_MMA_OSID_ORIGINAL_LEN (32)
#define TAF_MMA_OSID_LEN (16)
#define TAF_MMA_MAX_APPID_LEN (128)
#define TAF_MMA_ACDC_PARA_NUM (3)

#define TAF_MMA_LTE_BAND_MAX_LENGTH (8)

/* ĿǰNR��BAND��࣬��NR�����BAND����Ϊ׼ */
#define TAF_MMA_BAND_MAX_LEN (32)
#define MSG_HEADER_LENGTH    (20)

#define TAF_MMA_SET_OOS_SCENE_MAX_NUM (10)
#define TAF_MMA_OOS_PHASE_NUM (2)

#define TAF_MMA_NV_CARRIER_OPT_NV_ERROR                  0xFFFFFFFF /* NV���� */
#define TAF_MMA_NV_CARRIER_OPT_NO_ERROR                  0x00000000
#define TAF_MMA_NV_CARRIER_OPT_INVALID_PARA              0x100f0002 /* ��Ч�Ĳ��� */
#define TAF_MMA_NV_CARRIER_OPT_MALLOC_FAIL               0x100f0003 /* �����ڴ�ʧ�� */
#define TAF_MMA_NV_CARRIER_OPT_NOT_FIND_FILE             0x100f0006 /* �ļ������� */
#define TAF_MMA_NV_CARRIER_OPT_READ_NV_ORIGINAL_DATA_ERR 0x100f000D /* ��ȡNVԭʼ���ݴ��� */
#define TAF_MMA_NV_CARRIER_OPT_FILE_LEN_ERROR            0x100f0015 /* �ļ����ȴ��� */
#define TAF_MMA_NV_CARRIER_OPT_NOT_FIND_NV               0x100f0016 /* û�з���NV */
#define TAF_MMA_NV_CARRIER_OPT_NV_LEN_ERROR              0x100f0018 /* NV��ȴ��� */
#define TAF_MMA_NV_CARRIER_OPT_WRITE_NV_TIMEOUT          0x100f0035 /* дNV��ʱ,(ʵ����д���ļ�ϵͳ��ʱ) */
#define TAF_MMA_NV_CARRIER_OPT_WRITE_NV_FREE_FAIL        0x100f0036 /* д�ڴ��ͷ���ʧ�� */
#define TAF_MMA_NV_CARRIER_OPT_WRITE_NV_NOT_SUPPORT_ERR  0x100f0037 /* ��֧�ִ��� */
#define TAF_MMA_NV_CARRIER_OPT_MODEM_ID_ERROR            0x100f0039 /* modem id �������� */
#define TAF_MMA_NV_CARRIER_OPT_HMAC_VERIFY_FAIL          0x100f0057 /* HMACУ��ʧ�� */
#define TAF_MMA_NV_CARRIER_OPT_BUTT                      0x100f0058 /* ����ʧ�� */

/*
 * �ṹ˵��: ��Ӫ����ϢPLMN�����ָ�ʽMNC���ֶε�����
 *           0: DRV_AGENT_TSELRF_SET_ERROR_NO_ERROR ���ò����ɹ���
 *           1: DRV_AGENT_TSELRF_SET_ERROR_LOADDSP  ���ò���ʧ�ܣ�
 */
enum TAF_MMA_PlmnMncDigitMask {
    TAF_MMA_PLMN_MNC_DIGIT1_MASK = 0x00f,
    TAF_MMA_PLMN_MNC_DIGIT2_MASK = 0x0f0,
    TAF_MMA_PLMN_MNC_DIGIT3_MASK = 0xf00,
    TAF_MMA_PLMN_MNC_DIGIT_MASK_BUTT
};

enum TAF_MMA_PlmnMncDigitOffset {
    TAF_MMA_PLMN_MNC_DIGIT1_OFFSET = 0,
    TAF_MMA_PLMN_MNC_DIGIT2_OFFSET = 4,
    TAF_MMA_PLMN_MNC_DIGIT3_OFFSET = 8,
    TAF_MMA_PLMN_MNC_DIGIT_OFFSET_BUTT
};

enum TAF_MMA_PlmnMccDigitMask {
    TAF_MMA_PLMN_MCC_DIGIT1_MASK = 0x00f,
    TAF_MMA_PLMN_MCC_DIGIT2_MASK = 0x0f0,
    TAF_MMA_PLMN_MCC_DIGIT3_MASK = 0xf00,
    TAF_MMA_PLMN_MCC_DIGIT_MASK_BUTT
};

enum TAF_MMA_PlmnMccDigitOffset {
    TAF_MMA_PLMN_MCC_DIGIT1_OFFSET = 0,
    TAF_MMA_PLMN_MCC_DIGIT2_OFFSET = 4,
    TAF_MMA_PLMN_MCC_DIGIT3_OFFSET = 8,
    TAF_MMA_PLMN_MCC_DIGIT_OFFSET_BUTT
};

/*
 * Description:
 */
enum TAF_MMA_CdmaCallType {
    TAF_MMA_1X_EMERGENCY_VOICE_CALL = 0x00,
    TAF_MMA_1X_NORMAL_VOICE_CALL    = 0x01,
    TAF_MMA_1X_NORMAL_DATA_CALL     = 0x02,
    TAF_MMA_DO_NORMAL_DATA_CALL     = 0x03,
    TAF_MMA_CDMA_CALL_TYPE_BUTT     = 0x04
};
typedef VOS_UINT8 TAF_MMA_CdmaCallTypeUint8;

/*
 * �ṹ˵��: ����C-PDP-COUNT����ö��
 */
enum TAF_MMA_UpdateTypeCPdpCount {
    TAF_MMA_UPDATE_TYPE_C_PDP_1 = 0,
    TAF_MMA_UPDATE_TYPE_C_PDP_2,
    TAF_MMA_UPDATE_TYPE_C_PDP_3,
    TAF_MMA_UPDATE_TYPE_C_PDP_4,

    TAF_MMA_UPDATE_TYPE_C_PDP_MAX
};
typedef VOS_UINT32 TAF_MMA_UpdateTypecPdpCountUint32;

#define NAS_MM_INFO_IE_NULL (0x00)
#define NAS_MM_INFO_IE_LTZ 0x01              /* Local time zone */
#define NAS_MM_INFO_IE_UTLTZ 0x02            /* Universal time and local time zone */
#define NAS_MM_INFO_IE_LSA 0x04              /* LSA Identity */
#define NAS_MM_INFO_IE_DST 0x08              /* Network Daylight Saving Time */
#define NAS_MM_INFO_IE_NETWORKFULLNAME 0x10  /* Full name for network */
#define NAS_MM_INFO_IE_NETWORKSHORTNAME 0x20 /* Short name for network */

/* ����IMSI���ȶ��� */
#define NAS_MAX_IMSI_LENGTH (9)
#define NAS_VERSION_LEN (9)
#define NAS_IMSI_STR_LEN (15)

/* +CSQLVL ��صĺ궨�� */
#define AT_CSQLVL_MAX_NUM (3)
#define AT_CSQLVL_LEVEL_0 (0)
#define AT_CSQLVL_LEVEL_1 (20)
#define AT_CSQLVL_LEVEL_2 (40)
#define AT_CSQLVL_LEVEL_3 (60)

#define DRVAGENT_GPIOPL_MAX_LEN (20)

#define TAF_CDROM_VERSION_LEN 128

/* Ӳ���汾����󳤶� */
#define DRV_AGENT_MAX_HARDWARE_LEN (31)

#define TAF_MAX_REVISION_ID_LEN (31)

#define TAF_MAX_MODEL_ID_LEN (31)

#define TAF_MAX_HARDWARE_LEN (31)

#define PRODUCTION_CARD_TYPE "WCDMA"
#define PRODUCTION_CARD_TYPE_MAX_LEN (5)

#define TAF_UTRANCTRL_UTRAN_MODE_FDD (0)
#define TAF_UTRANCTRL_UTRAN_MODE_TDD (1)

#define AT_HS_PARA_MAX_LENGTH (9)

#define TAF_MMA_UTRA_BLER_INVALID (99)
#define TAF_MMA_RSSI_LOWER_LIMIT (-114)
#define TAF_MMA_UTRA_RSCP_UNVALID (-140)

#define TAF_MMA_RPT_CFG_MAX_SIZE (8)

#define TAF_MMA_MSG_ID_BASE (0x1000)

#define TAF_MMA_MAX_SRCHED_LAI_NUM 12
#define TAF_MMA_SAVE_SRCHED_LAI_MAX_NUM 12

#define TAF_MMA_PLMN_ID_LEN (6)

#define AT_PLMN_LEN_FIVE (5)
#define AT_PLMN_LEN_SIX (6)

#define TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM 10
#define TAF_MMA_MAX_NR_BAND_INDEX 512

/*
 * �ṹ˵��: MMA��Ϣ�ӿ�ö��
 */
enum TAF_MMA_MsgType {
    ID_TAF_MMA_MSG_TYPE_BEGIN = TAF_MMA_MSG_ID_BASE, /* 0x1000 */

    /* _H2ASN_MsgChoice AT_MMA_UsimStatusInd */
    ID_TAF_MMA_USIM_STATUS_IND = TAF_MMA_MSG_ID_BASE + 0x02,
    /* _H2ASN_MsgChoice AT_MMA_SimlockStatus */
    ID_TAF_MMA_SIMLOCK_STAUS_QUERY_CNF = TAF_MMA_MSG_ID_BASE + 0x03,

    /* _H2ASN_MsgChoice TAF_MMA_PhoneModeSetReq */
    ID_TAF_MMA_PHONE_MODE_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x04,
    /* _H2ASN_MsgChoice TAF_MMA_PhoneModeSetCnf */
    ID_TAF_MMA_PHONE_MODE_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x05,
    /* _H2ASN_MsgChoice TAF_MMA_SysCfgReq */
    ID_TAF_MMA_SYS_CFG_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x06,
    /* _H2ASN_MsgChoice TAF_MMA_SysCfgCnf */
    ID_TAF_MMA_SYS_CFG_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x07,
    /* _H2ASN_MsgChoice TAF_MMA_DetachReqMsg */
    ID_TAF_MMA_DETACH_REQ = TAF_MMA_MSG_ID_BASE + 0x0C,
    /* _H2ASN_MsgChoice TAF_MMA_DetachCnf */
    ID_TAF_MMA_DETACH_CNF = TAF_MMA_MSG_ID_BASE + 0x0D,
    /* _H2ASN_MsgChoice TAF_MMA_SimStatusInd */
    ID_TAF_MMA_SIM_STATUS_IND = TAF_MMA_MSG_ID_BASE + 0x13,

    /* _H2ASN_MsgChoice TAF_MMA_SrvAcqReq */
    ID_TAF_MMA_SRV_ACQ_REQ = TAF_MMA_MSG_ID_BASE + 0x14,
    /* _H2ASN_MsgChoice TAF_MMA_SrvAcqCnf */
    ID_TAF_MMA_SRV_ACQ_CNF = TAF_MMA_MSG_ID_BASE + 0x15,

    /* _H2ASN_MsgChoice TAF_MMA_SrvInfoNotify */
    ID_TAF_MMA_SRV_INFO_NOTIFY = TAF_MMA_MSG_ID_BASE + 0x16,

    /* *< @sa TAF_MMA_CdmaMoCallStartNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CdmaMoCallStartNtf */
    ID_TAF_MMA_CDMA_MO_CALL_START_NTF = TAF_MMA_MSG_ID_BASE + 0x17,
    /* *< @sa TAF_MMA_CdmaMoCallSuccessNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CdmaMoCallSuccessNtf */
    ID_TAF_MMA_CDMA_MO_CALL_SUCCESS_NTF = TAF_MMA_MSG_ID_BASE + 0x18,
    /* *< @sa TAF_MMA_CdmaCallRedialSystemAcquireNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CdmaCallRedialSystemAcquireNtf */
    ID_TAF_MMA_CDMA_CALL_REDIAL_SYSTEM_ACQUIRE_NTF = TAF_MMA_MSG_ID_BASE + 0x19,
    /* *< @sa TAF_MMA_CdmaMoCallEndNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CdmaMoCallEndNtf */
    ID_TAF_MMA_CDMA_MO_CALL_END_NTF = TAF_MMA_MSG_ID_BASE + 0x1A,

    /* _H2ASN_MsgChoice TAF_MMA_CfreqLockSetReq */
    ID_TAF_MMA_CDMA_FREQ_LOCK_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x1B,
    /* _H2ASN_MsgChoice TAF_MMA_CfreqLockSetCnf */
    ID_TAF_MMA_CDMA_FREQ_LOCK_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x1C,
    /* _H2ASN_MsgChoice TAF_MMA_CfreqLockQueryReq */
    ID_TAF_MMA_CDMA_FREQ_LOCK_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x1D,
    /* _H2ASN_MsgChoice TAF_MMA_CfreqLockQueryCnf */
    ID_TAF_MMA_CDMA_FREQ_LOCK_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x1E,

    /* _H2ASN_MsgChoice TAF_MMA_CdmacsqSetReq */
    ID_TAF_MMA_CDMACSQ_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x1F,
    /* _H2ASN_MsgChoice TAF_MMA_CdmacsqSetCnf */
    ID_TAF_MMA_CDMACSQ_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x20,
    /* _H2ASN_MsgChoice TAF_MMA_CdmacsqQryReq */
    ID_TAF_MMA_CDMACSQ_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x21,
    /* _H2ASN_MsgChoice TAF_MMA_CdmacsqQueryCnf */
    ID_TAF_MMA_CDMACSQ_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x22,
    /* _H2ASN_MsgChoice TAF_MMA_CdmacsqInd */
    ID_TAF_MMA_CDMACSQ_IND = TAF_MMA_MSG_ID_BASE + 0x23,

    /* _H2ASN_MsgChoice TAF_MMA_CtimeInd */
    ID_TAF_MMA_CTIME_IND = TAF_MMA_MSG_ID_BASE + 0x24,

    /* _H2ASN_MsgChoice TAF_MMA_CfplmnSetReq */
    ID_TAF_MMA_CFPLMN_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x25,
    /* _H2ASN_MsgChoice TAF_MMA_CfplmnQueryReq */
    ID_TAF_MMA_CFPLMN_QUERY_REQ = TAF_MMA_MSG_ID_BASE + 0x26,
    /* _H2ASN_MsgChoice TAF_MMA_CfplmnSetCnf */
    ID_TAF_MMA_CFPLMN_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x27,
    /* _H2ASN_MsgChoice TAF_MMA_CfplmnQueryCnf */
    ID_TAF_MMA_CFPLMN_QUERY_CNF = TAF_MMA_MSG_ID_BASE + 0x28,

    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnSetReq */
    ID_TAF_MMA_PREF_PLMN_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x29,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnSetCnf */
    ID_TAF_MMA_PREF_PLMN_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x2A,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnQueryReq */
    ID_TAF_MMA_PREF_PLMN_QUERY_REQ = TAF_MMA_MSG_ID_BASE + 0x2B,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnQueryCnf */
    ID_TAF_MMA_PREF_PLMN_QUERY_CNF = TAF_MMA_MSG_ID_BASE + 0x2C,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnTestReq */
    ID_TAF_MMA_PREF_PLMN_TEST_REQ = TAF_MMA_MSG_ID_BASE + 0x2D,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnTestCnf */
    ID_TAF_MMA_PREF_PLMN_TEST_CNF = TAF_MMA_MSG_ID_BASE + 0x2E,

    /* _H2ASN_MsgChoice TAF_MMA_PhoneModeQryReq */
    ID_TAF_MMA_PHONE_MODE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x2F,
    /* _H2ASN_MsgChoice TAF_MMA_PhoneModeQryCnf */
    ID_TAF_MMA_PHONE_MODE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x30,

    /* _H2ASN_MsgChoice TAF_MMA_QuickstartSetReq */
    ID_TAF_MMA_QUICKSTART_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x31,
    /* _H2ASN_MsgChoice TAF_MMA_QuickstartSetCnf */
    ID_TAF_MMA_QUICKSTART_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x32,
    /* _H2ASN_MsgChoice TAF_MMA_QuickstartQryReq */
    ID_TAF_MMA_QUICKSTART_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x33,
    /* _H2ASN_MsgChoice TAF_MMA_QuickstartQryCnf */
    ID_TAF_MMA_QUICKSTART_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x34,

    /* _H2ASN_MsgChoice TAF_MMA_AutoAttachSetReq */
    ID_TAF_MMA_AUTO_ATTACH_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x35,
    /* _H2ASN_MsgChoice TAF_MMA_AutoAttachSetCnf */
    ID_TAF_MMA_AUTO_ATTACH_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x36,
    /* _H2ASN_MsgChoice TAF_MMA_AutoAttachQryReq */
    ID_TAF_MMA_AUTO_ATTACH_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x37,
    /* _H2ASN_MsgChoice TAF_MMA_AutoAttachQryCnf */
    ID_TAF_MMA_AUTO_ATTACH_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x38,

    /* _H2ASN_MsgChoice TAF_MMA_SyscfgQryReq */
    ID_TAF_MMA_SYSCFG_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x39,
    /* _H2ASN_MsgChoice TAF_MMA_SyscfgQryCnf */
    ID_TAF_MMA_SYSCFG_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x3A,
    /* _H2ASN_MsgChoice TAF_MMA_SyscfgTestReq */
    ID_TAF_MMA_SYSCFG_TEST_REQ = TAF_MMA_MSG_ID_BASE + 0x3B,
    /* _H2ASN_MsgChoice TAF_MMA_SyscfgTestCnf */
    ID_TAF_MMA_SYSCFG_TEST_CNF = TAF_MMA_MSG_ID_BASE + 0x3c,

    /* _H2ASN_MsgChoice TAF_MMA_CrpnQryReq */
    ID_TAF_MMA_CRPN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x3D,
    /* _H2ASN_MsgChoice TAF_MMA_CrpnQryCnf */
    ID_TAF_MMA_CRPN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x3E,

    /* _H2ASN_MsgChoice TAF_MMA_CmmSetReq */
    ID_TAF_MMA_CMM_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x3F,
    /* _H2ASN_MsgChoice TAF_MMA_CmmSetCnf */
    ID_TAF_MMA_CMM_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x40,

    /* _H2ASN_MsgChoice TAF_MMA_PlmnSearchReq */
    ID_TAF_MMA_PLMN_SEARCH_REQ = TAF_MMA_MSG_ID_BASE + 0x41,
    /* _H2ASN_MsgChoice TAF_MMA_PlmnSearchCnf */
    ID_TAF_MMA_PLMN_SEARCH_CNF = TAF_MMA_MSG_ID_BASE + 0x42,
    /* _H2ASN_MsgChoice reserved */

    /* TAF_MMA_MSG_ID_BASE + 0x43 reserved */
    /* TAF_MMA_MSG_ID_BASE + 0x44 reserved */

    /* _H2ASN_MsgChoice TAF_MMA_PlmnListAbortReq */
    ID_TAF_MMA_PLMN_LIST_ABORT_REQ = TAF_MMA_MSG_ID_BASE + 0x45,
    /* _H2ASN_MsgChoice TAF_MMA_PlmnListAbortCnf */
    ID_TAF_MMA_PLMN_LIST_ABORT_CNF = TAF_MMA_MSG_ID_BASE + 0x46,

    /* _H2ASN_MsgChoice TAF_MMA_CerssiSetReq */
    ID_TAF_MMA_CERSSI_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x47,
    /* _H2ASN_MsgChoice TAF_MMA_CerssiSetCnf */
    ID_TAF_MMA_CERSSI_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x48,
    /* _H2ASN_MsgChoice TAF_MMA_CerssiInfoQryReq */
    ID_TAF_MMA_CERSSI_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x49,
    /* _H2ASN_MsgChoice TAF_MMA_CerssiInfoQryCnf */
    ID_TAF_MMA_CERSSI_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x4A,
    /* _H2ASN_MsgChoice TAF_MMA_RssiInfoInd */
    ID_TAF_MMA_RSSI_INFO_IND = TAF_MMA_MSG_ID_BASE + 0x4B,

    /* _H2ASN_MsgChoice TAF_MMA_MtPowerDownReq */
    ID_TAF_MMA_MT_POWER_DOWN_REQ = TAF_MMA_MSG_ID_BASE + 0x4C,
    /* _H2ASN_MsgChoice TAF_MMA_MtPowerDownCnf */
    ID_TAF_MMA_MT_POWER_DOWN_CNF = TAF_MMA_MSG_ID_BASE + 0x4D,

    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnTypeSetReq */
    ID_TAF_MMA_PREF_PLMN_TYPE_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x4E,
    /* _H2ASN_MsgChoice TAF_MMA_PrefPlmnTypeSetCnf */
    ID_TAF_MMA_PREF_PLMN_TYPE_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x4F,

    /* _H2ASN_MsgChoice TAF_MMA_CipherQryReq */
    ID_TAF_MMA_CIPHER_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x50,
    /* _H2ASN_MsgChoice TAF_MMA_CipherQryCnf */
    ID_TAF_MMA_CIPHER_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x51,

    /* _H2ASN_MsgChoice TAF_MMA_LocationInfoQryReq */
    ID_TAF_MMA_LOCATION_INFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x52,
    /* _H2ASN_MsgChoice TAF_MMA_LocationInfoQryCnf */
    ID_TAF_MMA_LOCATION_INFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x53,
    /* _H2ASN_MsgChoice TAF_MMA_CdmaLocinfoQryReq */
    ID_TAF_MMA_CDMA_LOCINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x54,
    /* _H2ASN_MsgChoice TAF_MMA_CdmaLocinfoQryCnf */
    ID_TAF_MMA_CDMA_LOCINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x55,
    /* _H2ASN_MsgChoice TAF_MMA_CSIDNID_IND_STRU */
    ID_TAF_MMA_CSID_IND = TAF_MMA_MSG_ID_BASE + 0x56,

    /* _H2ASN_MsgChoice TAF_MMA_AcInfoQryReq */
    ID_TAF_MMA_AC_INFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x57,
    /* _H2ASN_MsgChoice TAF_MMA_AcInfoQryCnf */
    ID_TAF_MMA_AC_INFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x58,
    /* _H2ASN_MsgChoice TAF_MMA_AcInfoChangeInd */
    ID_TAF_MMA_AC_INFO_CHANGE_IND = TAF_MMA_MSG_ID_BASE + 0x59,

    /* _H2ASN_MsgChoice TAF_MMA_CopnInfoQryReq */
    ID_TAF_MMA_COPN_INFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x5A,
    /* _H2ASN_MsgChoice TAF_MMA_CopnInfoQryCnf */
    ID_TAF_MMA_COPN_INFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x5B,

    /* _H2ASN_MsgChoice TAF_MMA_SimInsertReq */
    ID_TAF_MMA_SIM_INSERT_REQ = TAF_MMA_MSG_ID_BASE + 0x5C,
    /* _H2ASN_MsgChoice TAF_MMA_SimInsertCnf */
    ID_TAF_MMA_SIM_INSERT_CNF = TAF_MMA_MSG_ID_BASE + 0x5D,

    /* _H2ASN_MsgChoice TAF_MMA_EoplmnSetReq */
    ID_TAF_MMA_EOPLMN_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x5E,
    /* _H2ASN_MsgChoice TAF_MMA_EoplmnSetCnf */
    ID_TAF_MMA_EOPLMN_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x5F,
    /* _H2ASN_MsgChoice TAF_MMA_EoplmnQryReq */
    ID_TAF_MMA_EOPLMN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x60,
    /* _H2ASN_MsgChoice TAF_MMA_EoplmnQryCnf */
    ID_TAF_MMA_EOPLMN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x61,

    /* _H2ASN_MsgChoice TAF_MMA_NetScanReq */
    ID_TAF_MMA_NET_SCAN_REQ = TAF_MMA_MSG_ID_BASE + 0x62,
    /* _H2ASN_MsgChoice TAF_MMA_NetScanCnf */
    ID_TAF_MMA_NET_SCAN_CNF = TAF_MMA_MSG_ID_BASE + 0x63,
    /* _H2ASN_MsgChoice TAF_MMA_NetScanAbortReq */
    ID_TAF_MMA_NET_SCAN_ABORT_REQ = TAF_MMA_MSG_ID_BASE + 0x64,

    /* _H2ASN_MsgChoice TAF_MMA_AccessModeQryReq */
    ID_TAF_MMA_ACCESS_MODE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x65,
    /* _H2ASN_MsgChoice TAF_MMA_AccessModeQryCnf */
    ID_TAF_MMA_ACCESS_MODE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x66,

    /* _H2ASN_MsgChoice TAF_MMA_CopsQryReq */
    ID_TAF_MMA_COPS_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x67,
    /* _H2ASN_MsgChoice TAF_MMA_CopsQryCnf */
    ID_TAF_MMA_COPS_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x68,

    /* _H2ASN_MsgChoice TAF_MMA_RegStateQryReq */
    ID_TAF_MMA_REG_STATE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x69,
    /* _H2ASN_MsgChoice TAF_MMA_RegStateQryCnf */
    ID_TAF_MMA_REG_STATE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x6A,

    /* _H2ASN_MsgChoice TAF_MMA_SysinfoQryReq */
    ID_TAF_MMA_SYSINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x6B,
    /* _H2ASN_MsgChoice TAF_MMA_SysinfoQryCnf */
    ID_TAF_MMA_SYSINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x6C,

    /* �޵��õ�ɾ�� */

    /* _H2ASN_MsgChoice TAF_MMA_CsqQryReq */
    ID_TAF_MMA_CSQ_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x6F,
    /* _H2ASN_MsgChoice TAF_MMA_CsqQryCnf */
    ID_TAF_MMA_CSQ_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x70,

    /* �޵��õ�ɾ�� */

    /* _H2ASN_MsgChoice TAF_MMA_HomePlmnQryReq */
    ID_TAF_MMA_HOME_PLMN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x75,
    /* _H2ASN_MsgChoice TAF_MMA_HomePlmnQryCnf */
    ID_TAF_MMA_HOME_PLMN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x76,

    /* _H2ASN_MsgChoice TAF_MMA_SpnQryReq */
    ID_TAF_MMA_SPN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x77,
    /* _H2ASN_MsgChoice TAF_MMA_SpnQryCnf */
    ID_TAF_MMA_SPN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x78,

    /* _H2ASN_MsgChoice TAF_MMA_MmplmninfoQryReq */
    ID_TAF_MMA_MMPLMNINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x79,
    /* _H2ASN_MsgChoice TAF_MMA_MmplmninfoQryCnf */
    ID_TAF_MMA_MMPLMNINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x7A,

    /* _H2ASN_MsgChoice TAF_MMA_LastCampPlmnQryReq */
    ID_TAF_MMA_LAST_CAMP_PLMN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x7B,
    /* _H2ASN_MsgChoice TAF_MMA_LastCampPlmnQryCnf */
    ID_TAF_MMA_LAST_CAMP_PLMN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x7C,

    /* _H2ASN_MsgChoice TAF_MMA_UserSrvStateQryReq */
    ID_TAF_MMA_USER_SRV_STATE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x7D,
    /* _H2ASN_MsgChoice TAF_MMA_UserSrvStateQryCnf */
    ID_TAF_MMA_USER_SRV_STATE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x7E,

    /* _H2ASN_MsgChoice TAF_MMA_PowerOnAndRegTimeQryReq */
    ID_TAF_MMA_POWER_ON_AND_REG_TIME_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x7F,
    /* _H2ASN_MsgChoice TAF_MMA_PowerOnAndRegTimeQryCnf */
    ID_TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x80,

    /* _H2ASN_MsgChoice TAF_MMA_BatteryCapacityQryReq */
    ID_TAF_MMA_BATTERY_CAPACITY_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x81,
    /* _H2ASN_MsgChoice TAF_MMA_BatteryCapacityQryCnf */
    ID_TAF_MMA_BATTERY_CAPACITY_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x82,

    /* _H2ASN_MsgChoice TAF_MMA_HandShakeQryReq */
    ID_TAF_MMA_HAND_SHAKE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x83,
    /* _H2ASN_MsgChoice TAF_MMA_HandShakeQryCnf */
    ID_TAF_MMA_HAND_SHAKE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x84,

    /* _H2ASN_MsgChoice TAF_MMA_TimeChangeInd */
    ID_TAF_MMA_TIME_CHANGE_IND = TAF_MMA_MSG_ID_BASE + 0x85,
    /* _H2ASN_MsgChoice TAF_MMA_ModeChangeInd */
    ID_TAF_MMA_MODE_CHANGE_IND = TAF_MMA_MSG_ID_BASE + 0x86,
    /* _H2ASN_MsgChoice TAF_MMA_PlmnChangeInd */
    ID_TAF_MMA_PLMN_CHANGE_IND = TAF_MMA_MSG_ID_BASE + 0x87,

    /* _H2ASN_MsgChoice TAF_MMA_SrvStatusInd */
    ID_TAF_MMA_SRV_STATUS_IND = TAF_MMA_MSG_ID_BASE + 0x88,
    /* _H2ASN_MsgChoice TAF_MMA_RegStatusInd */
    ID_TAF_MMA_REG_STATUS_IND = TAF_MMA_MSG_ID_BASE + 0x89,
    /* _H2ASN_MsgChoice TAF_MMA_RegRejInfoInd */
    ID_TAF_MMA_REG_REJ_INFO_IND = TAF_MMA_MSG_ID_BASE + 0x8A,
    /* _H2ASN_MsgChoice TAF_MMA_PlmnSelectionInfoInd */
    ID_TAF_MMA_PLMN_SELECTION_INFO_IND = TAF_MMA_MSG_ID_BASE + 0x8B,

    ID_TAF_MMA_PLMN_LIST_REQ     = TAF_MMA_MSG_ID_BASE + 0x8C,
    ID_TAF_MMA_PLMN_LIST_CNF     = TAF_MMA_MSG_ID_BASE + 0x8D,
    ID_TAF_MSG_MMA_EONS_UCS2_REQ = TAF_MMA_MSG_ID_BASE + 0x8E,
    ID_TAF_MSG_MMA_EONS_UCS2_CNF = TAF_MMA_MSG_ID_BASE + 0x8F,

    /* _H2ASN_MsgChoice TAF_MMA_ImsSwitchSetReq */
    ID_TAF_MMA_IMS_SWITCH_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x90,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSwitchSetCnf */
    ID_TAF_MMA_IMS_SWITCH_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x91,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSwitchQryReq */
    ID_TAF_MMA_IMS_SWITCH_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x92,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSwitchQryCnf */
    ID_TAF_MMA_IMS_SWITCH_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x93,
    /* _H2ASN_MsgChoice TAF_MMA_VoiceDomainSetReq */
    ID_TAF_MMA_VOICE_DOMAIN_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x94,
    /* _H2ASN_MsgChoice TAF_MMA_VoiceDomainSetCnf */
    ID_TAF_MMA_VOICE_DOMAIN_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x95,
    /* _H2ASN_MsgChoice TAF_MMA_VoiceDomainQryReq */
    ID_TAF_MMA_VOICE_DOMAIN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x96,
    /* _H2ASN_MsgChoice TAF_MMA_VoiceDomainQryCnf */
    ID_TAF_MMA_VOICE_DOMAIN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x97,

    /* _H2ASN_MsgChoice TAF_MMA_AttachReq */
    ID_TAF_MMA_ATTACH_REQ = TAF_MMA_MSG_ID_BASE + 0x98,
    /* _H2ASN_MsgChoice TAF_MMA_AttachCnf */
    ID_TAF_MMA_ATTACH_CNF = TAF_MMA_MSG_ID_BASE + 0x99,
    /* _H2ASN_MsgChoice TAF_MMA_AttachStatusQryReq */
    ID_TAF_MMA_ATTACH_STATUS_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x9A,
    /* _H2ASN_MsgChoice TAF_MMA_AttachStatusQryCnf */
    ID_TAF_MMA_ATTACH_STATUS_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x9B,

    /* _H2ASN_MsgChoice TAF_MMA_PsRatTypeNtf */
    ID_TAF_MMA_PS_RAT_TYPE_NTF = TAF_MMA_MSG_ID_BASE + 0x9C,

    ID_TAF_MMA_1XCHAN_SET_REQ     = TAF_MMA_MSG_ID_BASE + 0x9D,
    ID_TAF_MMA_1XCHAN_SET_CNF     = TAF_MMA_MSG_ID_BASE + 0x9E,
    ID_TAF_MMA_1XCHAN_QUERY_REQ   = TAF_MMA_MSG_ID_BASE + 0x9F,
    ID_TAF_MMA_1XCHAN_QUERY_CNF   = TAF_MMA_MSG_ID_BASE + 0xA0,
    ID_TAF_MMA_CVER_QUERY_REQ     = TAF_MMA_MSG_ID_BASE + 0xA1,
    ID_TAF_MMA_CVER_QUERY_CNF     = TAF_MMA_MSG_ID_BASE + 0xA2,
    ID_TAF_MMA_GETSTA_QUERY_REQ   = TAF_MMA_MSG_ID_BASE + 0xA3,
    ID_TAF_MMA_GETSTA_QUERY_CNF   = TAF_MMA_MSG_ID_BASE + 0xA4,
    ID_TAF_MMA_CHIGHVER_QUERY_REQ = TAF_MMA_MSG_ID_BASE + 0xA5,
    ID_TAF_MMA_CHIGHVER_QUERY_CNF = TAF_MMA_MSG_ID_BASE + 0xA6,

    /* _H2ASN_MsgChoice TAF_MMA_QuitCallbackSetReq */
    ID_TAF_MMA_QUIT_CALLBACK_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xA7,
    /* _H2ASN_MsgChoice TAF_MMA_QuitCallbackSetCnf */
    ID_TAF_MMA_QUIT_CALLBACK_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xA8,
    /* _H2ASN_MsgChoice TAF_MMA_CsidlistSetReq */
    ID_TAF_MMA_CSIDLIST_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xA9,
    /* _H2ASN_MsgChoice TAF_MMA_CsidlistSetCnf */
    ID_TAF_MMA_CSIDLIST_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xAA,
    /* _H2ASN_MsgChoice TAF_MMA_EmcCallBackQryReq */
    ID_TAF_MMA_EMC_CALL_BACK_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xAB,
    /* _H2ASN_MsgChoice TAF_MMA_EmcCallBackQryCnf */
    ID_TAF_MMA_EMC_CALL_BACK_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xAC,
    /* _H2ASN_MsgChoice TAF_MMA_EmcCallBackNtf */
    ID_TAF_MMA_EMC_CALL_BACK_NTF = TAF_MMA_MSG_ID_BASE + 0xAD,

    /* _H2ASN_MsgChoice TAF_MMA_SrchedPlmnInfoInd */
    ID_TAF_MMA_SRCHED_PLMN_INFO_IND = TAF_MMA_MSG_ID_BASE + 0xAE,

    /* _H2ASN_MsgChoice TAF_MMA_HdrCsqSetReq              */
    ID_TAF_MMA_HDR_CSQ_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xAF,
    /* _H2ASN_MsgChoice MMA_TAF_HdrCsqSetCnf              */
    ID_MMA_TAF_HDR_CSQ_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xB0,
    /* _H2ASN_MsgChoice TAF_MMA_HdrCsqQrySettingReq      */
    ID_TAF_MMA_HDR_CSQ_QRY_SETTING_REQ = TAF_MMA_MSG_ID_BASE + 0xB1,
    /* _H2ASN_MsgChoice MMA_TAF_HdrCsqQrySettingCnf      */
    ID_MMA_TAF_HDR_CSQ_QRY_SETTING_CNF = TAF_MMA_MSG_ID_BASE + 0xB2,
    /* _H2ASN_MsgChoice MMA_TAF_HdrCsqValueInd            */
    ID_MMA_TAF_HDR_CSQ_VALUE_IND = TAF_MMA_MSG_ID_BASE + 0xB3,
    /* _H2ASN_MsgChoice TAF_MMA_DplmnQryReq */
    ID_TAF_MMA_DPLMN_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xB4,
    /* _H2ASN_MsgChoice TAF_MMA_DplmnQryCnf */
    ID_TAF_MMA_DPLMN_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xB5,
    /* _H2ASN_MsgChoice TAF_MMA_DplmnSetReq */
    ID_TAF_MMA_DPLMN_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xB6,
    /* _H2ASN_MsgChoice TAF_MMA_DplmnSetCnf */
    ID_TAF_MMA_DPLMN_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xB7,

    /* _H2ASN_MsgChoice TAF_MMA_BorderInfoSetReq */
    ID_TAF_MMA_BORDER_INFO_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xB8,
    /* _H2ASN_MsgChoice TAF_MMA_BorderInfoSetCnf */
    ID_TAF_MMA_BORDER_INFO_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xB9,
    /* _H2ASN_MsgChoice TAF_MMA_BorderInfoQryReq */
    ID_TAF_MMA_BORDER_INFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xBA,
    /* _H2ASN_MsgChoice TAF_MMA_BorderInfoQryCnf */
    ID_TAF_MMA_BORDER_INFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xBB,

    /* _H2ASN_MsgChoice TAF_MMA_ClmodeInd */
    ID_TAF_MMA_CLMODE_IND = TAF_MMA_MSG_ID_BASE + 0xBC,
    /* _H2ASN_MsgChoice TAF_MMA_CurrSidNidQryReq */
    ID_TAF_MMA_CURR_SID_NID_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xBD,
    /* _H2ASN_MsgChoice TAF_MMA_CurrSidNidQryCnf */
    ID_TAF_MMA_CURR_SID_NID_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xBE,

    /* _H2ASN_MsgChoice TAF_MMA_CsgListSearchReq */
    ID_TAF_MMA_CSG_LIST_SEARCH_REQ = TAF_MMA_MSG_ID_BASE + 0xBF,
    /* _H2ASN_MsgChoice TAF_MMA_CsgListSearchCnf */
    ID_TAF_MMA_CSG_LIST_SEARCH_CNF = TAF_MMA_MSG_ID_BASE + 0xC0,
    /* _H2ASN_MsgChoice TAF_MMA_CsgListAbortReq */
    ID_TAF_MMA_CSG_LIST_ABORT_REQ = TAF_MMA_MSG_ID_BASE + 0xC1,
    /* _H2ASN_MsgChoice TAF_MMA_CsgListAbortCnf */
    ID_TAF_MMA_CSG_LIST_ABORT_CNF = TAF_MMA_MSG_ID_BASE + 0xC2,

    /* _H2ASN_MsgChoice TAF_MMA_ClocinfoInd */
    ID_TAF_MMA_CLOCINFO_IND = TAF_MMA_MSG_ID_BASE + 0xC3,

    /* _H2ASN_MsgChoice TAF_MMA_InitLocInfoInd */
    ID_TAF_MMA_INIT_LOC_INFO_IND = TAF_MMA_MSG_ID_BASE + 0xC4,

    /* _H2ASN_MsgChoice TAF_MMA_RoamingModeSwitchInd */
    ID_TAF_MMA_ROAMING_MODE_SWITCH_IND = TAF_MMA_MSG_ID_BASE + 0xC5,

    /* _H2ASN_MsgChoice TAF_MMA_CtccRoamingNwInfoReportInd */
    ID_TAF_MMA_CTCC_ROAMING_NW_INFO_REPORT_IND = TAF_MMA_MSG_ID_BASE + 0xC6,

    /* _H2ASN_MsgChoice TAF_MMA_CtccRoamingNwInfoQryReq */
    ID_TAF_MMA_CTCC_ROAMING_NW_INFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xC7,

    /* _H2ASN_MsgChoice TAF_MMA_CtccRoamingNwInfoQryCnf */
    ID_TAF_MMA_CTCC_ROAMING_NW_INFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xC8,

    /* _H2ASN_MsgChoice TAF_MMA_CtccOosCountSetReq */
    ID_TAF_MMA_CTCC_OOS_COUNT_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xC9,
    /* _H2ASN_MsgChoice TAF_MMA_CtccOosCountSetCnf */
    ID_TAF_MMA_CTCC_OOS_COUNT_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xCA,
    /* _H2ASN_MsgChoice TAF_MMA_CsgSpecSearchReq */
    ID_TAF_MMA_CSG_SPEC_SEARCH_REQ = TAF_MMA_MSG_ID_BASE + 0xCB,
    /* _H2ASN_MsgChoice TAF_MMA_CsgSpecSearchCnf */
    ID_TAF_MMA_CSG_SPEC_SEARCH_CNF = TAF_MMA_MSG_ID_BASE + 0xCC,
    /* _H2ASN_MsgChoice TAF_MMA_QryCampCsgIdInfoReq */
    ID_TAF_MMA_QRY_CAMP_CSG_ID_INFO_REQ = TAF_MMA_MSG_ID_BASE + 0xCD,
    /* _H2ASN_MsgChoice TAF_MMA_QryCampCsgIdInfoCnf */
    ID_TAF_MMA_QRY_CAMP_CSG_ID_INFO_CNF = TAF_MMA_MSG_ID_BASE + 0xCE,

    ID_TAF_MMA_IMS_DOMAIN_CFG_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xCF,
    ID_TAF_MMA_IMS_DOMAIN_CFG_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xD0,
    ID_TAF_MMA_IMS_DOMAIN_CFG_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xD1,
    ID_TAF_MMA_IMS_DOMAIN_CFG_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xD2,

    /* _H2ASN_MsgChoice TAF_MMA_RoamImsSupportSetReq */
    ID_TAF_MMA_ROAM_IMS_SUPPORT_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xD3,
    /* _H2ASN_MsgChoice TAF_MMA_RoamImsSupportSetCnf */
    ID_TAF_MMA_ROAM_IMS_SUPPORT_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xD4,

    /* _H2ASN_MsgChoice TAF_MMA_CtccRoamingNwInfoRtpCfgSetReq */
    ID_TAF_MMA_CTCC_ROAMING_NW_INFO_RTP_CFG_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xD5,
    /* _H2ASN_MsgChoice TAF_MMA_CtccRoamingNwInfoRtpCfgSetCnf */
    ID_TAF_MMA_CTCC_ROAMING_NW_INFO_RTP_CFG_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xD6,

    /* _H2ASN_MsgChoice TAF_MMA_PrlidQryReq */
    ID_TAF_MMA_PRLID_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xD7,
    /* _H2ASN_MsgChoice TAF_MMA_PrlidQryCnf */
    ID_TAF_MMA_PRLID_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xD8,

    /* _H2ASN_MsgChoice TAF_MMA_ImsRegDomainNotify */
    ID_TAF_MMA_IMS_REG_DOMAIN_NOTIFY = TAF_MMA_MSG_ID_BASE + 0xD9,

    /* _H2ASN_MsgChoice TAF_MMA_C_PDP_COUNT_MAINTAIN_NOTIFY_STRU */
    ID_TAF_MMA_UPDATE_TYPE_C_PDP_COUNT_NOTIFY = TAF_MMA_MSG_ID_BASE + 0xDA,

    /* _H2ASN_MsgChoice TAF_MMA_CombinedModeSwitchInd */
    ID_TAF_MMA_COMBINED_MODE_SWITCH_IND = TAF_MMA_MSG_ID_BASE + 0xDB,

    ID_TAF_MMA_CDMA_MT_CALL_END_NTF = TAF_MMA_MSG_ID_BASE + 0xDC,

    /* _H2ASN_MsgChoice TAF_MMA_ImsiRefreshInd */
    ID_TAF_MMA_IMSI_REFRESH_IND = TAF_MMA_MSG_ID_BASE + 0xDD,

    /* _H2ASN_MsgChoice TAF_MMA_ResetNtf */
    ID_TAF_MMA_RESET_NTF = TAF_MMA_MSG_ID_BASE + 0xDE,

    ID_TAF_MMA_CDMA_ENTER_DORM_FOR_PRIOR_SYS_NTF = TAF_MMA_MSG_ID_BASE + 0xDF,

    /* _H2ASN_MsgChoice TAF_MMA_EflociinfoSetReq */
    ID_TAF_MMA_EFLOCIINFO_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xE0,
    /* _H2ASN_MsgChoice TAF_MMA_EflociinfoSetCnf */
    ID_TAF_MMA_EFLOCIINFO_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xE1,
    /* _H2ASN_MsgChoice TAF_MMA_EflociinfoQryReq */
    ID_TAF_MMA_EFLOCIINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xE2,
    /* _H2ASN_MsgChoice TAF_MMA_EflociinfoQryCnf */
    ID_TAF_MMA_EFLOCIINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xE3,
    /* _H2ASN_MsgChoice TAF_MMA_EfpslociinfoSetReq */
    ID_TAF_MMA_EFPSLOCIINFO_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xE4,
    /* _H2ASN_MsgChoice TAF_MMA_EfpslociinfoSetCnf */
    ID_TAF_MMA_EFPSLOCIINFO_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xE5,
    /* _H2ASN_MsgChoice TAF_MMA_EfpslociinfoQryReq */
    ID_TAF_MMA_EFPSLOCIINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xE6,
    /* _H2ASN_MsgChoice TAF_MMA_EfpslociinfoQryCnf */
    ID_TAF_MMA_EFPSLOCIINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xE7,

    /* _H2ASN_MsgChoice TAF_MMA_IccAppTypeSwitchInd */
    ID_TAF_MMA_ICC_APP_TYPE_SWITCH_IND = TAF_MMA_MSG_ID_BASE + 0xE8,

    /* _H2ASN_MsgChoice TAF_MMA_RatCombinedModeQryReq */
    ID_TAF_MMA_RAT_COMBINED_MODE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xE9,
    /* _H2ASN_MsgChoice TAF_MMA_RatCombinedModeQryCnf */
    ID_TAF_MMA_RAT_COMBINED_MODE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xEA,

    /* *< @sa TAF_MMA_CdmaMoCallStartNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CdmaMoSmsStartNtf */
    ID_TAF_MMA_CDMA_MO_SMS_START_NTF = TAF_MMA_MSG_ID_BASE + 0xEB,
    /* *< @sa TAF_MMA_CdmaMoCallEndNtf */
    /* _H2ASN_MsgChoice TAF_MMA_CDMA_MT_SMS_END_NTF_STRU */
    ID_TAF_MMA_CDMA_MO_SMS_END_NTF = TAF_MMA_MSG_ID_BASE + 0xEC,

    ID_TAF_MMA_PACSP_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xED,
    ID_TAF_MMA_PACSP_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xEE,

    ID_TAF_MMA_SMC_NO_ENTITY_NOTIFY = TAF_MMA_MSG_ID_BASE + 0xEF,

    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgSetReq */
    ID_TAF_MMA_IMS_SMS_CFG_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xF0,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgSetCnf */
    ID_TAF_MMA_IMS_SMS_CFG_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xF1,

    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgQryReq */
    ID_TAF_MMA_IMS_SMS_CFG_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xF2,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgQryCnf */
    ID_TAF_MMA_IMS_SMS_CFG_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xF3,

    /* _H2ASN_MsgChoice TAF_MMA_StopSrvacqNotify */
    ID_TAF_MMA_STOP_SRVACQ_NOTIFY = TAF_MMA_MSG_ID_BASE + 0xF4,

    /* _H2ASN_MsgChoice TAF_MMA_AcdcAppNotify */
    ID_TAF_MMA_ACDC_APP_NOTIFY = TAF_MMA_MSG_ID_BASE + 0xF5,

    /* _H2ASN_MsgChoice TAF_MMA_RestartReq */
    ID_TAF_MMA_RESTART_REQ = TAF_MMA_MSG_ID_BASE + 0xF6,
    /* _H2ASN_MsgChoice TAF_MMA_RestartCnf */
    ID_TAF_MMA_RESTART_CNF = TAF_MMA_MSG_ID_BASE + 0xF7,

    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgSetReq */
    ID_TAF_MMA_IMS_VIDEO_CALL_CAP_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xF8,
    /* _H2ASN_MsgChoice TAF_MMA_ImsSmsCfgSetCnf */
    ID_TAF_MMA_IMS_VIDEO_CALL_CAP_SET_CNF = TAF_MMA_MSG_ID_BASE + 0xF9,

    /* _H2ASN_MsgChoice TAF_MMA_RcsSwitchQryReq */
    ID_TAF_MMA_RCS_SWITCH_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0xFA,
    /* _H2ASN_MsgChoice TAF_MMA_RcsSwitchQryCnf */
    ID_TAF_MMA_RCS_SWITCH_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0xFB,

    /* _H2ASN_MsgChoice TAF_MMA_ExchangeModemInfoReq */
    ID_TAF_MMA_EXCHANGE_MODEM_INFO_REQ = TAF_MMA_MSG_ID_BASE + 0xFC,
    /* _H2ASN_MsgChoice TAF_MMA_ExchangeModemInfoCnf */
    ID_TAF_MMA_EXCHANGE_MODEM_INFO_CNF = TAF_MMA_MSG_ID_BASE + 0xFD,

    /* _H2ASN_MsgChoice TAF_MMA_MtreattachInd */
    ID_TAF_MMA_MT_REATTACH_IND = TAF_MMA_MSG_ID_BASE + 0xFE,

    /* _H2ASN_MsgChoice TAF_MMA_CIND_SET_REQ_STRU */
    ID_TAF_MMA_CIND_SET_REQ = TAF_MMA_MSG_ID_BASE + 0xFF,
    /* _H2ASN_MsgChoice TAF_MMA_CIND_SET_CNF_STRU */
    ID_TAF_MMA_CIND_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x100,

    ID_TAF_MMA_DSDS_STATE_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x101,
    ID_TAF_MMA_DSDS_STATE_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x102,
    ID_TAF_MMA_DSDS_STATE_NOTIFY  = TAF_MMA_MSG_ID_BASE + 0x103,

    ID_TAF_MMA_CEMODE_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x104,
    ID_TAF_MMA_CEMODE_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x105,
    ID_TAF_MMA_CEMODE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x106,
    ID_TAF_MMA_CEMODE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x107,

    ID_TAF_MMA_VOIMS_EMC_380_FAIL_NTF = TAF_MMA_MSG_ID_BASE + 0x108,

    ID_TAF_MMA_REJINFO_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x109,
    ID_TAF_MMA_REJINFO_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x10A,

    ID_TAF_MMA_SMS_STATUS_NOTIFY = TAF_MMA_MSG_ID_BASE + 0x10B,

    ID_TAF_MMA_EMRSSICFG_SET_REQ      = TAF_MMA_MSG_ID_BASE + 0x110,
    ID_TAF_MMA_EMRSSICFG_QRY_REQ      = TAF_MMA_MSG_ID_BASE + 0x111,
    ID_TAF_MMA_EMRSSICFG_SET_CNF      = TAF_MMA_MSG_ID_BASE + 0x112,
    ID_TAF_MMA_EMRSSICFG_QRY_CNF      = TAF_MMA_MSG_ID_BASE + 0x113,
    ID_TAF_MMA_EMRSSIRPT_SET_REQ      = TAF_MMA_MSG_ID_BASE + 0x114,
    ID_TAF_MMA_EMRSSIRPT_SET_CNF      = TAF_MMA_MSG_ID_BASE + 0x115,
    ID_TAF_MMA_EMRSSIRPT_QRY_REQ      = TAF_MMA_MSG_ID_BASE + 0x116,
    ID_TAF_MMA_EMRSSIRPT_QRY_CNF      = TAF_MMA_MSG_ID_BASE + 0x117,
    ID_TAF_MMA_EMRSSIRPT_IND          = TAF_MMA_MSG_ID_BASE + 0x118,
    ID_TAF_MMA_CDMA_MT_CALL_START_NTF = TAF_MMA_MSG_ID_BASE + 0x119,

    ID_TAF_MMA_ELEVATOR_STATE_IND = TAF_MMA_MSG_ID_BASE + 0X11A,

    ID_TAF_MMA_CLIMS_CFG_INFO_IND           = TAF_MMA_MSG_ID_BASE + 0x11B,
    ID_TAF_MMA_CL_DBDOMAIN_STATUSE_SET_REQ  = TAF_MMA_MSG_ID_BASE + 0x11C,
    ID_TAF_MMA_CL_DBDOMAIN_STATUSE_SET_CNF  = TAF_MMA_MSG_ID_BASE + 0x11D,
    ID_TAF_MMA_CL_DBDOMAIN_STATUSE_INFO_IND = TAF_MMA_MSG_ID_BASE + 0x11E,

    ID_TAF_MMA_ULFREQRPT_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x11F,
    ID_TAF_MMA_ULFREQRPT_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x120,
    ID_TAF_MMA_ULFREQRPT_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x121,
    ID_TAF_MMA_ULFREQRPT_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x122,
    ID_TAF_MMA_ULFREQ_CHANGE_IND = TAF_MMA_MSG_ID_BASE + 0x123,

    ID_TAF_MMA_PS_SCENE_SET_REQ = TAF_MMA_MSG_ID_BASE + 0x124,
    ID_TAF_MMA_PS_SCENE_SET_CNF = TAF_MMA_MSG_ID_BASE + 0x125,
    ID_TAF_MMA_PS_SCENE_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0x126,
    ID_TAF_MMA_PS_SCENE_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0x127,

    ID_TAF_MMA_PLMN_DETECT_NTF = TAF_MMA_MSG_ID_BASE + 0x128,
    ID_TAF_MMA_PLMN_DETECT_IND = TAF_MMA_MSG_ID_BASE + 0x129,

    ID_TAF_MMA_RRCSTAT_QRY_REQ            = TAF_MMA_MSG_ID_BASE + 0X12A,
    ID_TAF_MMA_RRCSTAT_QRY_CNF            = TAF_MMA_MSG_ID_BASE + 0X12B,
    ID_TAF_MMA_RRCSTAT_IND                = TAF_MMA_MSG_ID_BASE + 0X12C,
    ID_TAF_MMA_STOP_SRV_ACQ_REQ           = TAF_MMA_MSG_ID_BASE + 0x12D,
    ID_TAF_MMA_STOP_SRV_ACQ_CNF           = TAF_MMA_MSG_ID_BASE + 0x12E,
    ID_TAF_MMA_NR_BAND_BLOCK_LIST_SET_REQ = TAF_MMA_MSG_ID_BASE + 0X12F,
    ID_TAF_MMA_NR_BAND_BLOCK_LIST_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0X130,
    ID_TAF_MMA_NR_BAND_BLOCK_LIST_SET_CNF = TAF_MMA_MSG_ID_BASE + 0X131,
    ID_TAF_MMA_NR_BAND_BLOCK_LIST_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0X132,
    ID_TAF_MMA_SMS_ANTI_ATTACK_CAP_QRY_REQ = TAF_MMA_MSG_ID_BASE + 0X133,
    ID_TAF_MMA_SMS_ANTI_ATTACK_CAP_QRY_CNF = TAF_MMA_MSG_ID_BASE + 0X134,
    ID_TAF_MMA_SMS_ANTI_ATTACK_SET_REQ     = TAF_MMA_MSG_ID_BASE + 0X135,
    ID_TAF_MMA_SMS_ANTI_ATTACK_SET_CNF     = TAF_MMA_MSG_ID_BASE + 0X136,
    ID_TAF_MMA_NAS_SMS_CAP_QRY_REQ         = TAF_MMA_MSG_ID_BASE + 0X137,
    ID_TAF_MMA_NAS_SMS_CAP_QRY_CNF         = TAF_MMA_MSG_ID_BASE + 0X138,
    ID_TAF_MMA_PSEUD_BTS_IDENT_IND         = TAF_MMA_MSG_ID_BASE + 0X139,
    ID_TAF_MMA_SET_OOS_SRCH_STRATEGY_IND   = TAF_MMA_MSG_ID_BASE + 0X13A,
    ID_TAF_MMA_EMC_SRCH_RSLT_IND           = TAF_MMA_MSG_ID_BASE + 0X13C,

    ID_TAF_MMA_QUICK_CARD_SWITCH_IND       = TAF_MMA_MSG_ID_BASE + 0x13D,
#if (FEATURE_MBB_CUST == FEATURE_ON)
    ID_TAF_MMA_NETSELOPT_SET_REQ        = TAF_MMA_MSG_ID_BASE + 0X13E,
    ID_TAF_MMA_NETSELOPT_SET_CNF        = TAF_MMA_MSG_ID_BASE + 0X13F,
    ID_TAF_MMA_NETSELOPT_INFO_IND       = TAF_MMA_MSG_ID_BASE + 0X140,
    ID_TAF_MMA_EONS_QRY_REQ             = TAF_MMA_MSG_ID_BASE + 0X141,
    ID_TAF_MMA_EONS_QRY_CNF             = TAF_MMA_MSG_ID_BASE + 0X142,
    ID_TAF_MMA_EONS_CHANGE_IND          = TAF_MMA_MSG_ID_BASE + 0X143,
    ID_TAF_MMA_SUBNET_ADAPTER_INFO_IND  = TAF_MMA_MSG_ID_BASE + 0X144,
    ID_TAF_MMA_HCSQ_QRY_REQ             = TAF_MMA_MSG_ID_BASE + 0X145,
    ID_TAF_MMA_HCSQ_QRY_CNF             = TAF_MMA_MSG_ID_BASE + 0X146,
#endif

    ID_TAF_MMA_MSG_TYPE_BUTT
};
typedef VOS_UINT32 TAF_MMA_MsgTypeUint32;


enum TAF_MMA_Rrcstat {
    TAF_MMA_RRCSTAT_IDLE,
    TAF_MMA_RRCSTAT_CONN,
    TAF_MMA_RRCSTAT_INACTIVE,
    TAF_MMA_RRCSTAT_BUTT
};
typedef VOS_UINT8 TAF_MMA_RrcstatUint8;


enum TAF_MMA_CampStat {
    TAF_MMA_CAMPSTAT_CAMPED = 98,
    TAF_MMA_CAMPSTAT_NOT_CAMPED = 99
};
typedef VOS_UINT8 TAF_MMA_CampStatUint8;


enum TAF_MMA_RatType {
    TAF_MMA_RAT_GSM,   /* GSM���뼼�� */
    TAF_MMA_RAT_WCDMA, /* WCDMA���뼼�� */
    TAF_MMA_RAT_LTE,   /* LTE���뼼�� */
    TAF_MMA_RAT_1X,    /* 1X���뼼�� */
    TAF_MMA_RAT_HRPD,  /* HRPD���뼼�� */

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    TAF_MMA_RAT_NR, /* NR���뼼�� */
#endif

    TAF_MMA_RAT_BUTT

};
typedef VOS_UINT8 TAF_MMA_RatTypeUint8;


enum TAF_MMA_PlmnStatus {
    TAF_MMA_PLMN_STATUS_UNKNOW    = 0, /* ��ǰ����״̬δ֪ */
    TAF_MMA_PLMN_STATUS_AVAILABLE = 1, /* ��ǰ������� */
    TAF_MMA_PLMN_STATUS_CURRENT   = 2, /* ��ǰ������ע�� */
    TAF_MMA_PLMN_STATUS_FORBIDDEN = 3, /* ��ǰ�����ֹ */
    TAF_MMA_PLMN_STATUS_BUTT
};
typedef VOS_UINT8 TAF_MMA_PlmnStatusUint8;


enum TAF_MMA_RejRatType {
    TAF_MMA_REJ_RAT_GSM   = 0, /* GSM���뼼�� */
    TAF_MMA_REJ_RAT_WCDMA = 1, /* WCDMA���뼼�� */
    TAF_MMA_REJ_RAT_LTE   = 2, /* LTE���뼼�� */
    TAF_MMA_REJ_RAT_1X    = 3, /* 1X���뼼�� */
    TAF_MMA_REJ_RAT_HRPD  = 4, /* HRPD���뼼�� */
    TAF_MMA_REJ_RAT_NR    = 5, /* NR-5GC���뼼�� */

    TAF_MMA_REJ_RAT_BUTT
};
typedef VOS_UINT8 TAF_MMA_RejRatTypeUint8;


enum TAF_MMA_AppCfplmnOperResult {
    TAF_MMA_APP_CFPLMN_OPER_RESULT_SUCCESS  = 0, /* �����ɹ� */
    TAF_MMA_APP_CFPLMN_OPER_RESULT_FAILURE  = 1, /* ����ʧ�� */
    TAF_MMA_APP_CFPLMN_OPER_RESULT_NO_USIM  = 2, /* USIM�������� */
    TAF_MMA_APP_CFPLMN_OPER_RESULT_NULL_PTR = 3, /* ��ָ�� */
    TAF_MMA_APP_CFPLMN_OPER_RESULT_TIME_OUT = 4, /* ��ʱ */
    TAF_MMA_APP_CFPLMN_OPER_RESULT_BUTT
};
typedef VOS_UINT32 TAF_MMA_AppCfplmnOperResultUint32;


enum TAF_MMA_UsimmCardType {
    TAF_MMA_USIMM_CARD_SIM     = 0,
    TAF_MMA_USIMM_CARD_USIM    = 1,
    TAF_MMA_USIMM_CARD_ROM_SIM = 2,
    TAF_MMA_USIMM_CARD_ISIM    = 3, /* ISIM�� */
    TAF_MMA_USIMM_CARD_UIM     = 4, /* UIM�� */
    TAF_MMA_USIMM_CARD_CSIM    = 5, /* SCIM�� */
    TAF_MMA_USIMM_CARD_NOCARD  = 255,
    TAF_MMA_USIMM_CARD_TYPE_BUTT
};
typedef VOS_UINT32 TAF_MMA_UsimmCardTypeUint32;


enum TAF_MMA_RatNoChangeCmdType {
    TAF_MMA_RAT_NO_CHANGE_CMD_CPAM     = 1,
    TAF_MMA_RAT_NO_CHANGE_CMD_SYSCFG   = 2,
    TAF_MMA_RAT_NO_CHANGE_CMD_SYSCFGEX = 3,

    TAF_MMA_RAT_NO_CHANGE_CMD_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_RatNoChangeCmdTypeUint8;


enum TAF_MMA_SetCemode {
    TAF_MMA_MODE_PS_MODE2    = 0,
    TAF_MMA_MODE_CS_PS_MODE1 = 1,
    TAF_MMA_MODE_CS_PS_MODE2 = 2,
    TAF_MMA_MODE_PS_MODE1    = 3,
    TAF_MMA_MODE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SetCemodeUint8;


enum TAF_MMA_CemodeSetRst {
    TAF_MODE_SET_UE_USAGE_SETTING = 0, /* �л�UE�������� */
    TAF_MODE_SET_SERVICE_DOMAIN   = 1, /* �л������� */
    TAF_MODE_SET_NOT_ALLOWED      = 2, /* not allowed */
    TAF_MODE_SET_OK               = 3,
    TAF_MODE_SET_FAILED           = 4,
    TAF_MODE_SET_BUTT
};
typedef VOS_UINT8 TAF_MMA_CemodeSetRstUint8;

/* AT����MN_CALLBACK_CMD_CNF�¼�ʱ��ʹ�õ����ݽṹ */
typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    TAF_UINT8         rsv;
    TAF_UINT32        errorCode;
} AT_CmdCnfEvent;

/*
 * ö����: AT_MMA_UsimStatusInd
 * �ṹ˵��: AT��MMA AT_MMA_USIM_STATUS_IND��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                           /* _H2ASN_Skip */
    VOS_UINT32                  msgName;     /* _H2ASN_Skip */
    TAF_MMA_UsimmCardTypeUint32 cardType;    /* ������:SIM��USIM��ROM-SIM */
    USIMM_CardAppServicUint32   cardStatus;  /* ��״̬ */
    USIMM_PhyCardTypeUint32     phyCardType; /* Ӳ������ */
    USIMM_CardMediumTypeUint32  cardMediumType;
    VOS_UINT8                   imsi[NAS_MAX_IMSI_LENGTH]; /* IMSI�� */
    VOS_UINT8                   imsiLen;                   /* IMSI���� */
    VOS_UINT8                   rsv[2];
} AT_MMA_UsimStatusInd;

/*
 * ö����: AT_MMA_SimlockStatus
 * �ṹ˵��: AT��MMA AT_MMA_SimlockStatus��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    AT_APPCTRL     atAppCtrl;
    VOS_BOOL       simlockEnableFlg;
} AT_MMA_SimlockStatus;


enum TAF_MMA_CellRestrictionType {
    TAF_MMA_CELL_RESTRICTION_TYPE_NONE              = 0, /* ��ǰС���������κ�ҵ�� */
    TAF_MMA_CELL_RESTRICTION_TYPE_NORMAL_SERVICE    = 1, /* ��ǰС����������ҵ�� */
    TAF_MMA_CELL_RESTRICTION_TYPE_EMERGENCY_SERVICE = 2, /* ��ǰС�����ƽ���ҵ�� */
    /* ��ǰС����������ҵ��ͽ���ҵ�� */
    TAF_MMA_CELL_RESTRICTION_TYPE_ALL_SERVICE = 3,
    TAF_MMA_CELL_RESTRICTION_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_CellRestrictionTypeUint8;

/*
 * Э����:
 * ASN.1����:
 * ö��˵��: CSQLVLEXT����BER����ֵö���б�Ŀǰ�汾��֧��BER������ѯ��ֱ�ӷ���99
 */
enum MN_PH_CsqlvlextBerValue {
    MN_PH_CSQLVLEXT_BER_VALUE_99 = 99,
    MN_PH_CSQLVLEXT_BER_VALUE_BUTT
};
typedef VOS_UINT8 MN_PH_CsqlvlextBerValueUint8;

/*
 * Э����:
 * ASN.1����:
 * ö��˵��: CSQLVLEXT����RSSILV�źŸ�������ö���б�
 */
enum MN_PH_CsqlvlextRssilv {
    MN_PH_CSQLVLEXT_RSSILV_0,
    MN_PH_CSQLVLEXT_RSSILV_1,
    MN_PH_CSQLVLEXT_RSSILV_2,
    MN_PH_CSQLVLEXT_RSSILV_3,
    MN_PH_CSQLVLEXT_RSSILV_4,
    MN_PH_CSQLVLEXT_RSSILV_5,
    MN_PH_CSQLVLEXT_RSSILV_BUTT
};
typedef VOS_UINT8 MN_PH_CsqlvlextRssilvUint8;

/*
 * Э����:
 * ASN.1����:
 * ö��˵��: CSQLVLEXT����RSSILV�źŸ�������ö���б�
 */
enum MN_PH_CsqlvlextRssilvValue {
    MN_PH_CSQLVLEXT_RSSILV_VALUE_0  = 0,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_20 = 20,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_40 = 40,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_60 = 60,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_80 = 80,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_99 = 99,
    MN_PH_CSQLVLEXT_RSSILV_VALUE_BUTT
};
typedef VOS_UINT8 MN_PH_CsqlvlextRssilvValueUint8;


enum TAF_SysSubmode {
    TAF_SYS_SUBMODE_NONE         = 0,  /* �޷��� */
    TAF_SYS_SUBMODE_GSM          = 1,  /* GSMģʽ */
    TAF_SYS_SUBMODE_GPRS         = 2,  /* GPRSģʽ */
    TAF_SYS_SUBMODE_EDGE         = 3,  /* EDGEģʽ */
    TAF_SYS_SUBMODE_WCDMA        = 4,  /* WCDMAģʽ */
    TAF_SYS_SUBMODE_HSDPA        = 5,  /* HSDPAģʽ */
    TAF_SYS_SUBMODE_HSUPA        = 6,  /* HSUPAģʽ */
    TAF_SYS_SUBMODE_HSDPA_HSUPA  = 7,  /* HSDPA+HSUPAģʽ */
    TAF_SYS_SUBMODE_TD_SCDMA     = 8,  /* TD_SCDMAģʽ */
    TAF_SYS_SUBMODE_HSPA_PLUS    = 9,  /* HSPA+ģʽ */
    TAF_SYS_SUBMODE_LTE          = 10, /* LTEģʽ */
    TAF_SYS_SUBMODE_DC_HSPA_PLUS = 17, /* DC-HSPA+ģʽ */
    TAF_SYS_SUBMODE_DC_MIMO      = 18, /* MIMO-HSPA+ģʽ */

    TAF_SYS_SUBMODE_CDMA_1X    = 23, /* CDMA 1Xģʽ */
    TAF_SYS_SUBMODE_EVDO_REL_0 = 24, /* EVDO Rel0 */
    TAF_SYS_SUBMODE_EVDO_REL_A = 25, /* EVDO RelA */

    TAF_SYS_SUBMODE_HYBRID_EVDO_REL_0 = 28, /* Hybrid(EVDO Rel0) */

    TAF_SYS_SUBMODE_HYBRID_EVDO_REL_A = 29, /* Hybrid(EVDO RelA) */

    TAF_SYS_SUBMODE_EHRPD = 31, /* EHRPD */

    TAF_SYS_SUBMODE_EUTRAN_5GC = 110, /* 4G������ʽ5G������ģʽ */
    TAF_SYS_SUBMODE_NR_5GC     = 111, /* 5G������ʽ5G������ģʽ */

    TAF_SYS_SUBMODE_BUTT
};
typedef VOS_UINT8 TAF_SysSubmodeUint8;

/* ucSysModeExȡֵ���£� */
enum MN_PH_SysModeEx {
    MN_PH_SYS_MODE_EX_NONE_RAT,   /* �޷��� */
    MN_PH_SYS_MODE_EX_GSM_RAT,    /* GSMģʽ */
    MN_PH_SYS_MODE_EX_CDMA_RAT,   /* CDMAģʽ */
    MN_PH_SYS_MODE_EX_WCDMA_RAT,  /* WCDMAģʽ */
    MN_PH_SYS_MODE_EX_TDCDMA_RAT, /* TD-SCDMAģʽ */
    MN_PH_SYS_MODE_EX_WIMAX_RAT,  /* Wimaxģʽ */
    MN_PH_SYS_MODE_EX_LTE_RAT,    /* LTEģʽ */

    MN_PH_SYS_MODE_EX_EVDO_RAT,   /* EVDOģʽ */
    MN_PH_SYS_MODE_EX_HYBRID_RAT, /* CDMA 1X+EVDOģʽ */
    MN_PH_SYS_MODE_EX_SVLTE_RAT,  /* CDMA 1X+LTEģʽ */

    MN_PH_SYS_MODE_EX_EUTRAN_5GC_RAT, /* 4G������ʽ5G������ģʽ */
    MN_PH_SYS_MODE_EX_NR_5GC_RAT,     /* 5G������ʽ5G������ģʽ */
    MN_PH_SYS_MODE_EX_1X_NR_5GC_RAT,  /* 1x+NR+5GCģʽ */

    MN_PH_SYS_MODE_EX_BUTT_RAT
};
typedef VOS_UINT8 MN_PH_SysModeExUint8;

/* ucSubSysModeExȡֵ���£� */
enum MN_PH_SubSysModeEx {
    MN_PH_SUB_SYS_MODE_EX_NONE_RAT, /* �޷��� */
    MN_PH_SUB_SYS_MODE_EX_GSM_RAT,  /* GSMģʽ */
    MN_PH_SUB_SYS_MODE_EX_GPRS_RAT, /* GPRSģʽ */
    MN_PH_SUB_SYS_MODE_EX_EDGE_RAT, /* EDGEģʽ */

    MN_PH_SUB_SYS_MODE_EX_IS95A_RAT = 21,        /* IS95Aģʽ */
    MN_PH_SUB_SYS_MODE_EX_IS95B_RAT,             /* IS95Bģʽ */
    MN_PH_SUB_SYS_MODE_EX_CDMA20001X_RAT,        /* CDMA20001Xģʽ */
    MN_PH_SUB_SYS_MODE_EX_EVDOREL0_RAT,          /* EVDORel0ģʽ */
    MN_PH_SUB_SYS_MODE_EX_EVDORELA_RAT,          /* EVDORelAģʽ */
    MN_PH_SUB_SYS_MODE_EX_EVDORELB_RAT,          /* EVDORelBģʽ */
    MN_PH_SUB_SYS_MODE_EX_HYBIRD_CDMA20001X_RAT, /* HYBIRD CDMA20001Xģʽ */
    MN_PH_SUB_SYS_MODE_EX_HYBIRD_EVDOREL0_RAT,   /* HYBIRD EVDORel0ģʽ */
    MN_PH_SUB_SYS_MODE_EX_HYBIRD_EVDORELA_RAT,   /* HYBIRD EVDORelAģʽ */
    MN_PH_SUB_SYS_MODE_EX_HYBIRD_EVDORELB_RAT,   /* HYBIRD EVDORelAģʽ */

    MN_PH_SUB_SYS_MODE_EX_EHRPD_RAT = 31, /* EHRPDģʽ */

    MN_PH_SUB_SYS_MODE_EX_WCDMA_RAT       = 41, /* WCDMAģʽ */
    MN_PH_SUB_SYS_MODE_EX_HSDPA_RAT       = 42, /* HSDPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_HSUPA_RAT       = 43, /* HSUPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_HSPA_RAT        = 44, /* HSPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_HSPA_PLUS_RAT   = 45, /* HSPA+ģʽ */
    MN_PH_SUB_SYS_MODE_EX_DCHSPA_PLUS_RAT = 46, /* DCHSPA+ģʽ */

    MN_PH_SUB_SYS_MODE_EX_TDCDMA_RAT = 61, /* TD-SCDMAģʽ */

    MN_PH_SUB_SYS_MODE_EX_TD_HSDPA_RAT     = 62, /* HSDPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_TD_HSUPA_RAT     = 63, /* HSUPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_TD_HSPA_RAT      = 64, /* HSPAģʽ */
    MN_PH_SUB_SYS_MODE_EX_TD_HSPA_PLUS_RAT = 65, /* HSPA+ģʽ */

    MN_PH_SUB_SYS_MODE_EX_LTE_RAT = 101, /* LTEģʽ */

    MN_PH_SUB_SYS_MODE_EX_EUTRAN_5GC_RAT = 110, /* 4G������ʽ5G������ģʽ */
    MN_PH_SUB_SYS_MODE_EX_NR_5GC_RAT     = 111, /* 5G������ʽ5G������ģʽ */

    MN_PH_SUB_SYS_MODE_EX_BUTT_RAT
};
typedef VOS_UINT8 MN_PH_SubSysModeExUint8;


enum TAF_PH_AccessTech {
    TAF_PH_ACCESS_TECH_GSM         = 0, /* GSM */
    TAF_PH_ACCESS_TECH_GSM_COMPACT = 1, /* GSM Compact,��֧�� */
    TAF_PH_ACCESS_TECH_UTRAN       = 2, /* UTRAN */
    TAF_PH_ACCESS_TECH_EGPRS       = 3, /* EGPRS */
    TAF_PH_ACCESS_TECH_HSDPA       = 4, /* HSDPA */
    TAF_PH_ACCESS_TECH_HSUPA       = 5, /* HSUPA */
    TAF_PH_ACCESS_TECH_HSDPA_HSUPA = 6, /* HSDPA+HSUPA */
    TAF_PH_ACCESS_TECH_E_UTRAN     = 7, /* E-UTRAN */

    TAF_PH_ACCESS_TECH_EUTRAN_5GC = 10, /* E-UTRAN-5GC */
    TAF_PH_ACCESS_TECH_NR_5GC     = 11, /* NR-5GC */

    TAF_PH_ACCESS_TECH_CDMA_1X = 20, /* CDMA 1X */
    TAF_PH_ACCESS_TECH_EVDO    = 21, /* EVDO */
    TAF_PH_ACCESS_TECH_BUTT
};
typedef VOS_UINT8 TAF_PH_AccessTechUint8;

/*
 * Э����:
 * ASN.1����:
 * ö��˵��: ָʾ��ǰSIM����״̬:������Ƴ�
 */
enum TAF_SIM_InsertState {
    TAF_SIM_INSERT_STATE_REMOVE = 0, /* �Ƴ� */
    TAF_SIM_INSERT_STATE_INSERT = 1, /* ���� */
    TAF_SIM_INSERT_STATE_BUTT

};
typedef VOS_UINT32 TAF_SIM_InsertStateUint32;


enum TAF_MMA_PlmnListAbortProc {
    TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT    = 0, /* ���ڴ���ʱ����ʱ����б��� */
    TAF_MMA_PLMN_LIST_ABORT_PROCESSING_USER_ABORT = 1, /* ���ڴ����û�����б��� */
    TAF_MMA_PLMN_LIST_ABORT_BUTT
};
typedef VOS_UINT8 TAF_MMA_PlmnListAbortProcUint8;

enum TAF_MMA_PsInitRslt {
    TAF_MMA_PS_INIT_FAIL             = 0, /* ����ȷԭ�������ʧ�� */
    TAF_MMA_PS_INIT_SUCC             = 1, /* ��ʼ���ɹ� */
    TAF_MMA_PS_INIT_PHY_FAIL         = 2, /* ������ʼ��ʧ�� */
    TAF_MMA_PS_INIT_PHY_TIMEOUT      = 3, /* ������ʼ����ʱ */
    TAF_MMA_PS_INIT_PLATCAP_CFG_FAIL = 4, /* ����˳���ʼ��ʧ�� */
    TAF_PS_INIT_BUTT
};
typedef VOS_UINT32 TAF_MMA_PsInitRsltUint32;

enum TAF_MMA_AcqReason {
    TAF_MMA_ACQ_REASON_POWER_UP  = 0, /* ������ȡ���� */
    TAF_MMA_ACQ_REASON_OOS       = 1, /* ����������ȡ���� */
    TAF_MMA_ACQ_REASON_HIGH_PRIO = 2, /* ��ȡ�������ȼ����� */
    TAF_MMA_ACQ_REASON_BUTT
};
typedef VOS_UINT8 TAF_MMA_AcqReasonUint8;


enum TAF_MMA_EpsAttachReason {
    TAF_MMA_ATTACH_REASON_INITIAL  = 0, /* ��C->L��ѡʱ����д��ԭ�� */
    TAF_MMA_ATTACH_REASON_HANDOVER = 1, /* C->L��ѡʱ��д��ԭ�� */
    TAF_MMA_ATTACH_REASON_BUTT
};
typedef VOS_UINT8 TAF_MMA_EpsAttachReasonUint8;


enum TAF_MMA_AppOperResult {
    TAF_MMA_APP_OPER_RESULT_SUCCESS       = 0, /* �����ɹ� */
    TAF_MMA_APP_OPER_RESULT_FAILURE       = 1, /* ����ʧ�� */
    TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL = 2, /* ACQʧ�� */
    TAF_MMA_APP_OPER_RESULT_REJECT        = 3, /* ��������иò��� */
    TAF_MMA_APP_OPER_RESULT_BUTT
};
typedef VOS_UINT32 TAF_MMA_AppOperResultUint32;


enum TAF_MMA_DetachCause {
    TAF_MMA_DETACH_CAUSE_USER_DETACH        = 0, /* �û�������detach���� */
    TAF_MMA_DETACH_CAUSE_RAT_OR_BAND_DETACH = 1, /* system configure��RAT��band������detach */
    TAF_MMA_DETACH_CAUSE_SRV_DOMAIN_DETACH  = 2, /* system configure�з����򴥷���detach */
    TAF_MMA_DETACH_CAUSE_3GPP2_ATTACHED     = 3, /* ��3GPP2�ϸ��ųɹ� */
    TAF_MMA_DETACH_CAUSE_COPS               = 4, /* AT+COPS=2������detach���� */
    TAF_MMA_DETACH_CAUSE_SMS_ANTI_ATTACK = 5,
    TAF_MMA_DETACH_CAUSE_BUTT
};
typedef VOS_UINT8 TAF_MMA_DetachCauseUint8;


enum TAF_MMA_PlmnWithCsgIdType {
    TAF_MMA_CSG_ID_TYPE_IN_ALLOWED_CSG_LIST = 1, /* CSG ID��Allowed CSG List�� */
    /* CSG ID��Operator CSG List�в��ڽ�ֹCSG ID�б��� */
    TAF_MMA_CSG_ID_TYPE_IN_OPERATOR_CSG_LIST_NOT_FOBIDDEN = 2,
    /* CSG ID��Operator CSG List�в����ڽ�ֹCSG ID�б��� */
    TAF_MMA_CSG_ID_TYPE_IN_OPERATOR_CSG_LIST_FOBIDDEN = 3,
    /* CSG ID����Allowed CSG List��Operator CSG List�� */
    TAF_MMA_CSG_ID_TYPE_NOT_IN_ALLOWED_AND_OPERATOR_CSG_LIST = 4,
    TAF_MMA_CSG_ID_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_PlmnWithCsgIdTypeUint8;


enum TAF_MMA_AppPrefPlmnOperResult {
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_SUCCESS        = 0, /* �����ɹ� */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_FAILURE        = 1, /* ����ʧ�� */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_USIM_ABSENT    = 2, /* �޿� */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_NULL_PTR       = 3, /* ��ָ�� */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_CHECK_PARA_ERR = 4, /* �����������cpol��������ȷ */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_TIMEOUT        = 5, /* ��ʱ����ʱ */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_INVALID_PLMN   = 6, /* plmn��Ч */
    TAF_MMA_APP_PREF_PLMN_OPER_RESULT_BUTT
};
typedef VOS_UINT32 TAF_MMA_AppPrefPlmnOperResultUint32;


enum TAF_MMA_VoiceDomain {
    TAF_MMA_VOICE_DOMAIN_CS_ONLY          = 0, /* CS voice only */
    TAF_MMA_VOICE_DOMAIN_IMS_PS_ONLY      = 1, /* IMS PS voice only */
    TAF_MMA_VOICE_DOMAIN_CS_PREFERRED     = 2, /* CS vocie preferred, IMS PS voice as secondary */
    TAF_MMA_VOICE_DOMAIN_IMS_PS_PREFERRED = 3, /* IMS PS voice preferred, CS vocie as secondary */

    TAF_MMA_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32 TAF_MMA_VoiceDomainUint32;


enum TAF_MMA_ImsSwitchSet {
    TAF_MMA_IMS_SWITCH_SET_OFF = 0,
    TAF_MMA_IMS_SWITCH_SET_ON  = 1,
    TAF_MMA_IMS_SWITCH_SET_BUTT
};
typedef VOS_UINT8 TAF_MMA_ImsSwitchSetUint8;

/*
 * Description:
 */
enum TAF_MMA_1XCasPRev {
    TAF_MMA_1X_CAS_P_REV_JST_008       = 0x01,
    TAF_MMA_1X_CAS_P_REV_IS_95         = 0x02,
    TAF_MMA_1X_CAS_P_REV_IS_95A        = 0x03,
    TAF_MMA_1X_CAS_P_REV_IS_95B_CUSTOM = 0x04,
    TAF_MMA_1X_CAS_P_REV_IS_95B        = 0x05,
    TAF_MMA_1X_CAS_P_REV_IS_2000       = 0x06,
    TAF_MMA_1X_CAS_P_REV_IS_2000A      = 0x07,
    TAF_MMA_1X_CAS_P_REV_ENUM_BUTT     = 0x08
};
typedef VOS_UINT8 TAF_MMA_1XCasPRevUint8;


enum TAF_MMA_PsRatType {
    TAF_MMA_PS_RAT_TYPE_NULL,  /* ��ǰδ���κ�������פ�� */
    TAF_MMA_PS_RAT_TYPE_GSM,   /* GSM���뼼�� */
    TAF_MMA_PS_RAT_TYPE_WCDMA, /* WCDMA���뼼�� */
    TAF_MMA_PS_RAT_TYPE_LTE,   /* LTE���뼼�� */
    TAF_MMA_PS_RAT_TYPE_1X,    /* CDMA-1X���뼼�� */
    TAF_MMA_PS_RAT_TYPE_HRPD,  /* CDMA-HRPD���뼼�� */
    TAF_MMA_PS_RAT_TYPE_EHRPD, /* CDMA-EHRPD���뼼�� */
    TAF_MMA_PS_RAT_TYPE_BUTT   /* ��Ч�Ľ��뼼�� */
};
typedef VOS_UINT32 TAF_MMA_PsRatTypeUint32;


enum TAF_MMA_QryRegStatusType {
    TAF_MMA_QRY_REG_STATUS_TYPE_CS,  /* CREG��ѯCSע��״̬ */
    TAF_MMA_QRY_REG_STATUS_TYPE_PS,  /* CREG��ѯPSע��״̬ */
    TAF_MMA_QRY_REG_STATUS_TYPE_EPS, /* CREG��ѯEPSע��״̬ */
    TAF_MMA_QRY_REG_STATUS_TYPE_5GC, /* CREG��ѯ5GCע��״̬ */
    TAF_MMA_QRY_REG_STATUS_TYPE_BUTT /* ��Ч�Ĳ�ѯ���� */
};
typedef VOS_UINT32 TAF_MMA_QryRegStatusTypeUint32;



enum TAF_MMA_ImsDomainCfgType {
    TAF_MMA_IMS_DOMAIN_CFG_TYPE_WIFI_PREFER     = 0, /* WIFI prefer */
    TAF_MMA_IMS_DOMAIN_CFG_TYPE_LTE_PREFER      = 1, /* LTE prefer */
    TAF_MMA_IMS_DOMAIN_CFG_TYPE_WIFI_ONLY       = 2, /* WIFI only */
    TAF_MMA_IMS_DOMAIN_CFG_TYPE_CELLULAR_PREFER = 3, /* CELLULAR prefer */
    TAF_MMA_IMS_DOMAIN_CFG_TYPE_BUTT                 /* ��Ч�Ĳ�ѯ���� */
};
typedef VOS_UINT32 TAF_MMA_ImsDomainCfgTypeUint32;


enum TAF_MMA_ImsDomainCfgResult {
    TAF_MMA_IMS_DOMAIN_CFG_RESULT_SUCCESS = 0,
    TAF_MMA_IMS_DOMAIN_CFG_RESULT_FAILED  = 1,
    TAF_MMA_IMS_DOMAIN_CFG_RESULT_BUTT
};
typedef VOS_UINT32 TAF_MMA_ImsDomainCfgResultUint32;


enum TAF_MMA_RoamImsSupport {
    TAF_MMA_ROAM_IMS_UNSUPPORT = 0,
    TAF_MMA_ROAM_IMS_SUPPORT   = 1,

    TAF_MMA_ROAM_IMS_BUTT
};
typedef VOS_UINT32 TAF_MMA_RoamImsSupportUint32;


enum TAF_MMA_RoamImsSupportResult {
    TAF_MMA_ROAM_IMS_SUPPORT_RESULT_SUCCESS = 0,
    TAF_MMA_ROAM_IMS_SUPPORT_RESULT_FAILED  = 1,
    TAF_MMA_ROAM_IMS_SUPPORT_RESULT_BUTT
};
typedef VOS_UINT32 TAF_MMA_RoamImsSupportResultUint32;


enum TAF_MMA_ImsRegDomain {
    TAF_MMA_IMS_REG_DOMAIN_NULL = 0,
    TAF_MMA_IMS_REG_DOMAIN_LTE  = 1,
    TAF_MMA_IMS_REG_DOMAIN_WIFI = 2,
    TAF_MMA_IMS_REG_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_MMA_ImsRegDomainUint8;

enum TAF_MMA_ImsRegStatus {
    TAF_MMA_IMS_REG_STATUS_DEREG    = 0x00,
    TAF_MMA_IMS_REG_STATUS_DEREGING = 0x01,
    TAF_MMA_IMS_REG_STATUS_REGING   = 0x02,
    TAF_MMA_IMS_REG_STATUS_REG      = 0x03,

    TAF_MMA_IMS_REG_STATUS_BUTT = 0xFF
};
typedef VOS_UINT8 TAF_MMA_ImsRegStatusUint8;


enum TAF_MMA_LocUpdateStatus {
    TAF_MMA_LOC_UPDATE_STATUS_UPDATED                   = 0,
    TAF_MMA_LOC_UPDATE_STATUS_NOT_UPDATED               = 1,
    TAF_MMA_LOC_UPDATE_STATUS_PLMN_NOT_ALLOWED          = 2,
    TAF_MMA_LOC_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED = 3,
    TAF_MMA_LOC_UPDATE_STATUS_BUTT
};
typedef VOS_UINT8 TAF_MMA_LocUpdateStatusUint8;


enum TAF_MMA_CombinedMode {
    TAF_MMA_COMBINED_MODE_GUL  = 0x00, /* ���ģʽΪGULģʽ */
    TAF_MMA_COMBINED_MODE_CL   = 0x01, /* ���ģʽΪCLģʽ */
    TAF_MMA_COMBINED_MODE_BUTT = 0x02
};
typedef VOS_UINT32 TAF_MMA_CombinedModeUint32;


enum TAF_MMA_IccAppTypeSwitchState {
    TAF_MMA_ICC_APP_TYPE_SWITCH_STATE_BEGIN = 0x00, /* �л���ʼ */
    TAF_MMA_ICC_APP_TYPE_SWITCH_STATE_END   = 0x01, /* �л����� */
    TAF_MMA_ICC_APP_TYPE_SWITCH_STATE_BUTT  = 0x02
};
typedef VOS_UINT32 TAF_MMA_IccAppTypeSwitchStateUint32;


enum TAF_MMA_SmsAntiAttackState {
    TAF_MMA_SMS_ANTI_ATTACK_END   = 0,
    TAF_MMA_SMS_ANTI_ATTACK_START = 1,
    TAF_MMA_SMS_ANTI_ATTACK_BUTT
};
typedef VOS_UINT8 TAF_MMA_SmsAntiAttackStateUint8;


enum TAF_MMA_SmsAntiAttackCurrentRat {
    TAF_MMA_SMS_ANTI_ATTACK_CURRENT_RAT_NULL = 0,
    TAF_MMA_SMS_ANTI_ATTACK_CURRENT_RAT_GSM  = 1,
    TAF_MMA_SMS_ANTI_ATTACK_CURRENT_RAT_BUTT
};
typedef VOS_UINT8 TAF_MMA_SmsAntiAttackCurrentRatUint8;


enum TAF_MMA_SmsAntiAttackSupportResult {
    TAF_MMA_SMS_ANTI_ATTACK_SUPPORT     = 0,
    TAF_MMA_SMS_ANTI_ATTACK_NOT_SUPPORT = 1,
    TAF_MMA_SMS_ANTI_ATTACK_SUPPORT_BUTT
};
typedef VOS_UINT8 TAF_MMA_SmsAntiAttackSupportResultUint8;

enum TAF_MMA_SmsAntiAttackNotSupportCause {
    TAF_MMA_SUPPORT                           = 0,
    TAF_MMA_IN_SMS_ANTI_ATTACK_STATE          = 1,
    TAF_MMA_SMS_ANTI_ATTACK_CAPBILITY_DISABLE = 2,
    TAF_MMA_SMS_CAPBILITY_DISABLE             = 3,
    TAF_MMA_CURRENT_PLMN_NOT_CFG_IN_NV        = 4,
    TAF_MMA_HAVE_CALL_EXISTS                  = 5,
    TAF_MMA_SENSOR_STATE_NOT_CFG_IN_NV        = 6,
    TAF_MMA_NOT_SUPPORT_SILENT_SMS_TYPE       = 7,
    TAF_MMA_NOT_SUPPORT_CAUSE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SmsAntiAttackNotSupportCauseUint8;


enum TAF_MMA_AntiAttackSmsType {
    TAF_MMA_ANTI_ATTACK_VERIFICATION_SMS_TYPE = 0,
    TAF_MMA_ANTI_ATTACK_SLIENT_SMS_TYPE       = 1,
};
typedef VOS_UINT8 TAF_MMA_AntiAttackSmsTypeUint8;

#define TAF_INVALID_TAB_INDEX 0
/* TAF��ģ��ID */
#ifndef SUBMOD_NULL
#define SUBMOD_NULL 0
#endif

/* IMEI��Ч���ݳ���: IMEI��Ϣ��IMEI(TAC8λ,SNR6λ)��У��λ��������� */
#define TAF_IMEI_DATA_LENGTH (15)

typedef struct {
    TAF_UINT32 mcc;
    TAF_UINT32 mnc;
} TAF_PLMN_Id;


typedef struct {
    TAF_PLMN_Id hplmn;
    VOS_UINT8   hplmnMncLen;
    VOS_UINT8   reserved[3];
} TAF_MMA_HplmnWithMncLen;

#define TAF_PH_RELEASEDATE_LEN 10
typedef struct {
    TAF_UINT8 releaseDate[TAF_PH_RELEASEDATE_LEN + 1]; /* �������� */
    VOS_UINT8 reserved;
} TAF_PH_ReleaseDate;

#define TAF_PH_RELEASETIME_LEN 8
typedef struct {
    TAF_UINT8 releaseTime[TAF_PH_RELEASETIME_LEN + 1]; /* ����ʱ�� */
    VOS_UINT8 reserved[3];
} TAF_PH_ReleaseTime;

#define TAF_PH_CAPLITYCLASS1 1
#define TAF_PH_CAPLITYCLASS2 4
#define TAF_PH_CAPLITYCLASS3 16
#define TAF_PH_COMPLETECAPILITYLIST_LEN 16
#define TAF_PH_CAPILITYLIST_TYPE 3
typedef struct {
    /* �����Ĺ����б� */
    TAF_UINT8 completeCapilityList[TAF_PH_CAPILITYLIST_TYPE][TAF_PH_COMPLETECAPILITYLIST_LEN];
} TAF_PH_Compcaplist;


enum MN_MMA_CpamRatType {
    MN_MMA_CPAM_RAT_TYPE_WCDMA,     /* WCDMA��ģ* */
    MN_MMA_CPAM_RAT_TYPE_WCDMA_GSM, /* WCDMA/GSM˫ģ */
    MN_MMA_CPAM_RAT_TYPE_GSM,       /* GSM��ģ */
    MN_MMA_CPAM_RAT_TYPE_NOCHANGE,  /* ���ı� */
    MN_MMA_CPAM_RAT_TYPE_BUTT
};
typedef VOS_UINT8 MN_MMA_CpamRatTypeUint8;

#define TAF_PHONE_ACCESS_MODE_LEN (2)

/* Taf_USIM_RAT */
typedef TAF_UINT16 TAF_MMC_USIM_RAT;
#define TAF_MMC_USIM_UTRN_RAT 0x8000
#define TAF_MMC_USIM_GSM_RAT 0x0080
#define TAF_MMC_USIM_UTRN_GSM_RAT 0x8080
#define TAF_MMC_USIM_GSM_COMPACT_RAT 0x0040
#define TAF_MMC_USIM_NO_RAT 0x0000
#define TAF_MMC_USIM_E_UTRN_RAT 0x4000
#define MMC_MS_LTE 2

#define TAF_MMC_USIM_NR_RAT 0x0800

typedef TAF_UINT8 TAF_PHONE_SERVICE_STATUS;

typedef TAF_UINT8 TAF_PHONE_CHANGED_SERVICE_STATUS;  /* ����״̬�仯�����ϱ�(^SRVST) */
#define TAF_REPORT_SRVSTA_NO_SERVICE 0               /* �޷���״̬       */
#define TAF_REPORT_SRVSTA_LIMITED_SERVICE 1          /* ���Ʒ���״̬     */
#define TAF_REPORT_SRVSTA_NORMAL_SERVICE 2           /* ��������״̬     */
#define TAF_REPORT_SRVSTA_REGIONAL_LIMITED_SERVICE 3 /* �����Ƶ�������� */
#define TAF_REPORT_SRVSTA_DEEP_SLEEP 4               /* ʡ�����˯��״̬ */

/* typedef TAF_UINT16 TAF_PHONE_ERROR; */

typedef struct {
    TAF_PHONE_SERVICE_STATUS csSrvSta; /* CS����״̬ */
    TAF_PHONE_SERVICE_STATUS psSrvSta; /* PS����״̬ */
    VOS_UINT8                reserved[2];
} TAF_PH_SrvSta;

typedef TAF_UINT8 TAF_PH_PLMN_PRIO;
#define TAF_PLMN_PRIO_AUTO 0  /* �Զ�ѡ������ */
#define TAF_PLMN_GSM_PRIO 1   /* ��ѡ2G���� */
#define TAF_PLMN_WCDMA_PRIO 2 /* ��ѡ3G���� */
#define TAF_PLMN_PRIO_NOCHANGE 3

#define TAF_PLMN_PRIO_DIST_BTWN_CPAM_SYSCFG 1 /* CPAM��SYSCFG�Ľ������ȼ����õĲ�� 1 */

typedef TAF_UINT8 TAF_PH_MS_CLASS_TYPE;
#define TAF_PH_MS_CLASS_CC 1 /* ֻ֧��CS���ֻ���Ӧ��MMC��D_MMC_MODE_CC */
#define TAF_PH_MS_CLASS_CG 2 /* ֻ֧��PS���ֻ���Ӧ��MMC��D_MMC_MODE_CG */
#define TAF_PH_MS_CLASS_A 3

#define TAF_PH_MS_CLASS_ANY 4 /* �൱��ֻ֧��CS�� */

#define TAF_PH_MS_CLASS_NULL 5
#define TAF_PH_MS_CLASS_B 6 /* ��Ӧ��GSM��B���ֻ� */

/* ԭ���궨���(TAF_PH_SERVICE_DOMAIN)�ĳ�ö�� */

enum TAF_MMA_ServiceDomain {
    TAF_MMA_SERVICE_DOMAIN_CS       = 0, /* CS�� */
    TAF_MMA_SERVICE_DOMAIN_PS       = 1, /* PS�� */
    TAF_MMA_SERVICE_DOMAIN_CS_PS    = 2, /* CS��PS�� */
    TAF_MMA_SERVICE_DOMAIN_ANY      = 3, /* CS��PS�� */
    TAF_MMA_SERVICE_DOMAIN_NOCHANGE = 4, /* �����򲻱� */
    TAF_MMA_SERVICE_DOMAIN_NULL     = 5, /* �޷����� */

    TAF_MMA_SERVICE_DOMAIN_BUTT
};
typedef TAF_UINT8 TAF_MMA_ServiceDomainUint8;


enum TAF_MMA_AttachType {
    TAF_MMA_ATTACH_TYPE_NULL      = 0,
    TAF_MMA_ATTACH_TYPE_GPRS      = 1, /* PS */
    TAF_MMA_ATTACH_TYPE_IMSI      = 2, /* CS */
    TAF_MMA_ATTACH_TYPE_GPRS_IMSI = 3, /* PS & CS */

    TAF_MMA_ATTACH_TYPE_BUTT
};
typedef TAF_UINT8 TAF_MMA_AttachTypeUint8;


enum TAF_MMA_DetachType {
    TAF_MMA_DETACH_TYPE_NULL      = 0,
    TAF_MMA_DETACH_TYPE_GPRS      = 1, /* PS */
    TAF_MMA_DETACH_TYPE_IMSI      = 2, /* CS */
    TAF_MMA_DETACH_TYPE_GPRS_IMSI = 3, /* PS & CS */

    TAF_MMA_DETACH_TYPE_BUTT
};
typedef TAF_UINT8 TAF_MMA_DetachTypeUint8;


enum TAF_MMA_AttachStatus {
    TAF_MMA_ATTACH_STATUS_DETACHED = 0,
    TAF_MMA_ATTACH_STATUS_ATTACHED = 1,

    TAF_MMA_ATTACH_STATUS_BUTT
};
typedef TAF_UINT8 TAF_MMA_AttachStatusUint8;

typedef TAF_UINT8 TAF_PH_ImsiLockStatusUint8;
#define TAF_PH_IMSI_LOCK_ENABLED 1
#define TAF_PH_IMSI_LOCK_DISABLED 0


enum TAF_MMA_NvCopyResult {
    TAF_MMA_NV_FILE_COPY_NO_ERROR               = 0x00000000,
    TAF_MMA_NV_FILE_COPY_MEM_ALLOC_FAIL         = 0x100f0003, /* �ڴ�����ʧ�� */
    TAF_MMA_NV_FILE_COPY_FILE_READ_FAIL         = 0x100f0007, /* �ļ���ȡʧ�� */
    TAF_MMA_NV_FILE_COPY_FILE_WRITE_FAIL        = 0x100f0008, /* �ļ�д��ʧ�� */
    TAF_MMA_NV_FILE_COPY_FILE_CLOSE_FAIL        = 0x100f000c, /* �ļ��ر�ʧ�� */
    TAF_MMA_NV_FILE_COPY_FILE_OPEN_FAIL         = 0x100f0024, /* �ļ���ʧ�� */
    TAF_MMA_NV_FILE_COPY_GET_FILE_LEN_FAIL      = 0x100f0066, /* ��ȡ�ļ�����ʧ�� */
    TAF_MMA_NV_FILE_COPY_BUTT
};
typedef VOS_UINT32 TAF_MMA_NvCopyResultUint32;


enum TAF_MMA_NvCarrierType {
    TAF_MMA_NV_CARRIER_TYPE_COMM        = 0,
    TAF_MMA_NV_CARRIER_TYPE_CARRIER,
    TAF_MMA_NV_CARRIER_TYPE_COMM_CARRIER,
    TAF_MMA_NV_CARRIER_TYPE_BUTT
};
typedef VOS_UINT32 TAF_MMA_NvCarrierTypeUint32;

/* ��ȡ�˿���Ϣ */
#define TAF_MAX_PORT_INFO_LEN 100
typedef struct {
    TAF_UINT8 portInfo[TAF_MAX_PORT_INFO_LEN + 1];
    TAF_UINT8 rsv[3];
} TAF_PH_PortInfo;

/*
 * �ṹ��    : TAF_PH_CreationTime
 * Э����  :
 * ASN.1���� :
 * �ṹ˵��  : ϵͳ����ʱ��
 */
#define TAF_MAX_CREATION_TIME_LEN 31
typedef struct {
    TAF_UINT8 creationTime[TAF_MAX_CREATION_TIME_LEN + 1];
} TAF_PH_CreationTime;

#define TAF_CFG_VERSION (1004)

/* +CGMR - ��ȡģ������汾�� */
typedef struct {
    TAF_UINT8 revisionId[TAF_MAX_REVISION_ID_LEN + 1];
} TAF_PH_RevisionId;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: Ӳ������
 */
typedef struct {
    TAF_UINT8 hwVer[TAF_MAX_HARDWARE_LEN + 1];
} TAF_PH_HwVer;

/* +CGMM - ��ȡģ���ʶ */
typedef struct {
    TAF_UINT8 modelId[TAF_MAX_MODEL_ID_LEN + 1];
} TAF_PH_ModelId;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ��̨����汾��
 */

typedef struct {
    TAF_UINT8 isoInfo[TAF_CDROM_VERSION_LEN];
} TAF_PH_CdromVer;

/* +CGSN - ��ȡIMEI */
typedef TAF_UINT8 TAF_PH_IMEI_TYPE;
#define TAF_PH_IMEI_LEN 16
#define TAF_PH_IMEIV_Mode 1
#define TAF_PH_IMEI_Mode 0

typedef struct {
    TAF_PH_IMEI_TYPE imeiMode;
    TAF_UINT8        imei[TAF_PH_IMEI_LEN + 1];
    VOS_UINT8        reserved[2];
} TAF_PH_Imei;

/* +CIMI - ��ȡIMSI */
#define TAF_PH_IMSI_LEN 15
typedef struct {
    TAF_UINT8 imsi[TAF_PH_IMSI_LEN + 1];
} TAF_PH_Imsi;

/* +CREG - ��������ע��״̬�ϱ���ʽ */
#define TAF_PH_LAC_LENGTH 4
#define TAF_PH_CI_LENGTH 4
#define TAF_PH_RAC_LENGTH 2

/* ע��״̬ */
typedef TAF_UINT8 TAF_PH_REG_STATE_TYPE;
#define TAF_PH_REG_NOT_REGISTERED_NOT_SEARCH 0 /* û��ע�ᣬMS���ڲ�û������ѰҪע����µ���Ӫ�� */
#define TAF_PH_REG_REGISTERED_HOME_NETWORK 1   /* ע���˱������� */
#define TAF_PH_REG_NOT_REGISTERED_SEARCHING 2  /* û��ע�ᣬ��MS������ѰҪע����µ���Ӫ�� */
#define TAF_PH_REG_REGISTERED_DENIED 3         /* ע�ᱻ�ܾ� */
#define TAF_PH_REG_UNKNOWN 4                   /* δ֪ԭ�� */
#define TAF_PH_REG_REGISTERED_ROAM 5           /* ע������������ */

#define TAF_MMC_NORMAL_SERVICE 0

#define TAF_MMC_LIMITED_SERVICE (1)        /* ���Ʒ��� */
#define TAF_MMC_LIMITED_SERVICE_REGION (2) /* ������������� */
#define TAF_MMC_NO_IMSI (3)                /* ��imsi */
#define TAF_MMC_NO_SERVICE (4)             /* �޷��� */

/* ����ģʽ��������+CFUN  */
typedef TAF_UINT8 TAF_PH_MODE;
#define TAF_PH_MODE_MINI 0     /* minimum functionality */
#define TAF_PH_MODE_FULL 1     /* full functionality */
#define TAF_PH_MODE_TXOFF 2    /* disable phone transmit RF circuits only */
#define TAF_PH_MODE_RXOFF 3    /* disable phone receive RF circuits only */
#define TAF_PH_MODE_RFOFF 4    /* disable phone both transmit and receive RF circuits */
#define TAF_PH_MODE_FT 5       /* factory-test functionality */
#define TAF_PH_MODE_RESET 6    /* reset */
#define TAF_PH_MODE_VDFMINI 7  /* mini mode required by VDF */
#define TAF_PH_MODE_POWEROFF 8 /* �ػ��µ�ģʽ */
#define TAF_PH_MODE_LOWPOWER 9
#if (FEATURE_MBB_CUST == FEATURE_ON)
#define TAF_PH_MODE_FAKEOFF  10
#define TAF_PH_MODE_FAKEON   11
#define TAF_PH_MODE_NUM_MAX  12
#else
#define TAF_PH_MODE_NUM_MAX 10
#endif

typedef TAF_UINT8 TAF_PH_RESET_FLG;
#define TAF_PH_OP_MODE_UNRESET 0
#define TAF_PH_OP_MODE_RESET 1

typedef TAF_UINT8 TAF_PH_CMD_TYPE;
#define TAF_PH_CMD_SET 0   /* set cmd */
#define TAF_PH_CMD_QUERY 1 /* query cmd */

typedef struct {
    TAF_PH_CMD_TYPE  cmdType; /* �������� */
    TAF_PH_MODE      prePhMode;
    TAF_PH_MODE      phMode;  /* ģʽ */
    TAF_PH_RESET_FLG phReset; /* �Ƿ���Ҫ���� */
} TAF_PH_OpMode;
typedef struct {
    TAF_PH_CMD_TYPE cmdType; /* �������� */
    TAF_PH_MODE     phMode;  /* ģʽ */
    VOS_UINT8       reserved[2];
} TAF_PH_OpModeCnf;


typedef struct {
    VOS_INT16 rscpValue; /* С���ź���������3g��^cerssi�ϱ�ʹ�� */
    VOS_INT16 ecioValue; /* С�����������3g��^cerssi�ϱ�ʹ�� */
    /* ����ECNO, ��ԭ�е�sEcioValue����0.5���ȣ�Ŀǰ��ΪAT����CESQʹ�á���ΧΪ: -49~0 */
    VOS_INT8  ecno;
    VOS_UINT8 reserved[3];
} TAF_MMA_WCellSignInfo;


typedef struct {
    VOS_INT16 rssiValue; /* С���ź���������2g��^cerssi�ϱ�ʹ��,2gû��rscp�ĸ����õ���rssi */
    VOS_UINT8 reserve1[2];
} TAF_MMA_GCellSignInfo;


typedef struct {
    VOS_UINT16 ri; /* RI */
    VOS_UINT8  res[2];
    VOS_UINT16 cqi[2]; /* CQI�������� */

} TAF_MMA_CqiInfo;

typedef struct {
    VOS_UINT8 rxANTNum; /* �������� */
    VOS_UINT8 rev[3];
    VOS_INT16 rsrpRx[TAF_MMA_MAX_ANT_NUM]; /* �����ߵ�rsrpֵ */
#if (FEATURE_ON == FEATURE_LTE_R14)
    VOS_INT32 sinrRx[TAF_MMA_MAX_ANT_NUM_EX]; /* �����ߵ�sinrֵ */
#else
    VOS_INT32            sinrRx[TAF_MMA_MAX_ANT_NUM];      /* �����ߵ�sinrֵ */
#endif
} TAF_MMA_RxAntInfo;

typedef struct {
    VOS_INT16         rssi;      /* Rssi */
    VOS_INT16         rsd;       /* ����ֶ� */
    VOS_INT16         rsrp;      /* ��Χ��(-141,-44), 99Ϊ��Ч */
    VOS_INT16         rsrq;      /* ��Χ��(-40, -6) , 99Ϊ��Ч */
    VOS_INT32         sinr;      /* SINR RS_SNR */
    TAF_MMA_CqiInfo   cqi;       /* Channle Quality Indicator */
    TAF_MMA_RxAntInfo rxAntInfo; /* ������Ŀ���������ź� */
} TAF_MMA_LCellSignInfo;

#if (FEATURE_ON == FEATURE_UE_MODE_NR)

typedef struct {
    VOS_INT16 s5GRsrp; /* ��Χ��(-141,-44), 99Ϊ��Ч */
    VOS_INT16 s5GRsrq; /* ��Χ��(-40, -6) , 99Ϊ��Ч */
    VOS_INT32 l5GSinr; /* SINR RS_SNR */
} TAF_MMA_NrCellSignInfo;
#endif

/* RSSI��ѯ��������+CSQ */
/*
 * 0       ����0��С�� -113 dBm
 * 1                -111 dBm
 * 2...30           -109...  -53 dBm
 * 31     ���ڻ���� -51 dBm
 * 99     δ֪�򲻿ɲ⡣
 */
#define TAF_PH_RSSIUNKNOW 99
#define TAF_PH_RSSIZERO 0
#define TAF_PH_RSSI_LOW (-113)
#define TAF_PH_RSSI_HIGH (-51)
#define TAF_PH_CSQ_RSSI_LOW (0)
#define TAF_PH_CSQ_RSSI_HIGH (31)
#define TAF_PH_BER_UNKNOWN (99)

typedef struct {
    TAF_UINT8 rssiValue;   /* ��ת��Ϊ�ȼ���ʾ���ź�ǿ�� */
    TAF_UINT8 channalQual; /* ������,ֻ������ҵ���ʱ�����,����Ч��99 */
    VOS_UINT8 reserved1[2];

    union {
        TAF_MMA_WCellSignInfo wCellSignInfo;
        TAF_MMA_GCellSignInfo gCellSignInfo;
        TAF_MMA_LCellSignInfo lCellSignInfo;
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
        TAF_MMA_NrCellSignInfo nrCellSignInfo;
#endif
    } u;

} TAF_PH_RssiValue;

#define TAF_PH_RSSI_MAX_NUM 8

typedef struct {
    TAF_UINT8 rssiNum; /* �ϱ��ź�ǿ��С���ĸ��� */

    TAF_MMA_RatTypeUint8 ratType; /* ����ģʽ0:G;1:W */
    VOS_UINT8            currentUtranMode;
    VOS_UINT8            reserv1;

    VOS_UINT16 cellDlFreq; /* С��Ƶ��(����) */
    VOS_UINT16 cellUlFreq; /* ��ǰƵ��(����) */
    VOS_INT16  ueRfPower;  /* ���书�� */
    VOS_UINT8  reserved[2];

    TAF_PH_RssiValue aRssi[TAF_PH_RSSI_MAX_NUM]; /* ÿ��С�����ź�ǿ���������� */
} TAF_PH_Rssi;

typedef struct {
    TAF_INT32 rscpValue;
    TAF_INT32 ecioValue;
    TAF_UINT8 rssiValue; /* ��ת��Ϊ�ȼ���ʾ���ź�ǿ�� */
    TAF_UINT8 anlevelValue;
    TAF_UINT8 channalQual; /* ������,ֻ������ҵ���ʱ�����,����Ч��99 */
    VOS_UINT8 reserved[1];
} TAF_PH_AnlevelValue;

#define TAF_PH_ANLEVEL_MAX_NUM 8

typedef struct {
    TAF_UINT8           anlevelNum; /* �ϱ��ź�ǿ��С���ĸ��� */
    VOS_UINT8           reserved[3];
    TAF_PH_AnlevelValue aAnlevel[TAF_PH_ANLEVEL_MAX_NUM]; /* ÿ��С�����ź�ǿ���������� */
} TAF_PH_Anlevel;

#define TAF_UE_RFPOWER_INVALID 0x1fff
#define TAF_FREQ_INVALID 0xffff
typedef struct {
    TAF_UINT32 cellId;
    TAF_INT16  cellRSCP;
    VOS_UINT8  reserved[2];
} TAF_PH_CELL_Rscp;

typedef struct {
    TAF_UINT16       cellNum;
    VOS_UINT8        reserved1[2];
    TAF_PH_CELL_Rscp cellRscp[TAF_PH_RSSI_MAX_NUM];
    TAF_UINT16       cellDlFreq; /* С��Ƶ��(����) */
    VOS_UINT8        reserved2[2];
} TAF_PH_CellRssi;

typedef struct {
    TAF_UINT16 cellUlFreq; /* ��ǰƵ��(����) */
    TAF_INT16  ueRfPower;  /* ���书�� */
} TAF_PH_UeRfpowerFreq;

/* ��ǰ��ص���+CBC */
/*
 * ״̬���ͣ�
 * 0 ������ڹ���
 * 1 �������ӣ����ǵ��δ����
 * 2 û����������
 * 3 ��Դ����, ͨ����ֹ
 */
#define TAF_PH_BATTERYCHARGING 0
#define TAF_PH_BATTERYNOSUPPLY 1
#define TAF_PH_NOBATTERYFIXED 2
#define TAF_PH_POWERSOURCEERROR 3
typedef TAF_UINT8 TAF_PH_BatteryPowerUint8;

/*
 * 0��Դ�����û����������
 * 1...100 Ŀǰʣ��������ٷֱ�
 */
typedef TAF_UINT8 TAF_PH_BatteryRemains;
typedef struct {
    TAF_PH_BatteryPowerUint8 batteryPowerStatus;
    TAF_PH_BatteryRemains    batteryRemains;
    VOS_UINT8                reserved[2];
} TAF_MMA_Batterypower;

typedef TAF_UINT8 TAF_ME_PersonalisationCmdType;
#define TAF_ME_PERSONALISATION_SET 0x01        /* �ƶ��豸˽���������� */
#define TAF_ME_PERSONALISATION_VERIFY 0x02     /* �ƶ��豸˽������У�� */
#define TAF_ME_PERSONALISATION_PWD_CHANGE 0x03 /* �����ƶ��豸˽������ */
#define TAF_ME_PERSONALISATION_ACTIVE 0x04     /* �����ƶ��豸˽�й��� */
#define TAF_ME_PERSONALISATION_DEACTIVE 0x05   /* ȥ�����ƶ��豸˽�й��� */
#define TAF_ME_PERSONALISATION_RETRIEVE 0x06   /* ��ȡ��ǰ�ƶ��豸˽�������б� */
#define TAF_ME_PERSONALISATION_QUERY 0x07      /* ��ѯ��ǰ�ƶ��豸˽������״̬ */

/* ��ǰֻ֧�ֶ������Ĳ��� */
typedef TAF_UINT8 TAF_ME_PersonalisationType;
#define TAF_SIM_PERSONALISATION 0x01             /* ���� */
#define TAF_NETWORK_PERSONALISATION 0x02         /* ������ */
#define TAF_NETWORK_SUBSET_PERSONALISATION 0x03  /* �������Ӽ� */
#define TAF_SERVICE_PROVIDE_PERSONALISATION 0x04 /* ��ҵ��Ӧ�� */
#define TAF_CORPORATE_PERSONALISATION 0x05       /* ����˾* */
#define TAF_OPERATOR_PERSONALISATION 0x06        /* �Զ���:������,�����ն�����ʵ�� */

/* ���嵱ǰ����״̬ */
typedef TAF_UINT8 TAF_ME_PersonalisationStatusFlag;
#define TAF_ME_PERSONALISATION_ACTIVE_STATUS 0
#define TAF_ME_PERSONALISATION_DEACTIVE_STATUS 1

/* �Զ���:���嵱ǰ����Ӫ�̼���״̬ */
#define TAF_OPERATOR_LOCK_STATUS_NOT_UNLOCK TAF_ME_PERSONALISATION_ACTIVE_STATUS
#define TAF_OPERATOR_LOCK_STATUS_UNLOCKED TAF_ME_PERSONALISATION_DEACTIVE_STATUS

/* �ϱ�������Ӫ��״̬ */
enum TAF_OPERATOR_LockStatus {
    TAF_OPERATOR_LOCK_NEED_UNLOCK_CODE   = 1, /* ��Ҫ��������� */
    TAF_OPERATOR_LOCK_NONEED_UNLOCK_CODE = 2, /* ����Ҫ��������� */
    TAF_OPERATOR_LOCK_LOCKED             = 3, /* ��������״̬ */
    TAF_OPERATOR_LOCK_BUTT
};
typedef TAF_UINT32 TAF_OPERATOR_LockStatusUint32;
#define TAF_OPERATOR_UNLOCK_TIMES_MAX 10 /* ��Ӫ���������������ֵ */
#define TAF_OPERATOR_UNLOCK_TIMES_MIN 0  /* ��Ӫ��������������Сֵ */
#define TAF_PH_ME_LOCK_OPER_LEN_MAX 15   /* ��Ӫ�����ṩ������Ӫ������󳤶� */
#define TAF_PH_ME_LOCK_OPER_LEN_MIN 5    /* ��Ӫ�����ṩ������Ӫ������С���� */

typedef struct {
    TAF_UINT32                    remainTimes;
    TAF_OPERATOR_LockStatusUint32 operatorLockStatus;
    TAF_UINT8                     operatorLen;
    TAF_UINT8                     operatorArray[TAF_PH_ME_LOCK_OPER_LEN_MAX];
} TAF_OPERATOR_LockInfo;

#define TAF_MAX_IMSI_LEN 15
typedef struct {
    TAF_UINT8 dataLen;
    TAF_UINT8 simPersonalisationStr[TAF_MAX_IMSI_LEN]; /* �趨����������   */
} TAF_SIM_PersionalisationStr;

typedef union {
    /* �����ִ�,CmdTypeΪTAF_ME_PERSONALISATION_RETRIEVE��MePersonalTypeΪTAF_SIM_PERSONALISATIONʱ��Ч */
    TAF_SIM_PersionalisationStr SimPersionalisationStr;
} TAF_ME_PERSONALISATION_SET_UN;

typedef union {
    /* �����ִ�,CmdTypeΪTAF_ME_PERSONALISATION_RETRIEVE��MePersonalTypeΪTAF_SIM_PERSONALISATIONʱ��Ч */
    TAF_SIM_PersionalisationStr SimPersionalisationStr;

    /* ��Ӫ������Ϣ,CmdTypeΪTAF_ME_PERSONALISATION_QUERY��MePersonalTypeΪTAF_OPERATOR_PERSONALISATIONʱ��Ч */
    TAF_OPERATOR_LockInfo OperatorLockInfo;
} TAF_ME_PERSONALISATION_REPORT_CONTENT_UN;

#define TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX 8

typedef struct {
    TAF_ME_PersonalisationCmdType cmdType;
    TAF_ME_PersonalisationType    mePersonalType;
    TAF_UINT8                     oldPwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 1];
    TAF_UINT8                     newPwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX + 1];
    TAF_ME_PERSONALISATION_SET_UN unMePersonalisationSet;
} TAF_ME_PersonalisationData;

typedef TAF_UINT8 TAF_OP_ME_PERSONALISATION_RESULT;
#define TAF_PH_ME_PERSONALISATION_OK 0
#define TAF_PH_ME_PERSONALISATION_IMST_STR_NOT_EXIST 1
#define TAF_PH_ME_PERSONALISATION_ERROR 2
#define TAF_PH_ME_PERSONALISATION_WRONG_PARA 3
#define TAF_PH_ME_PERSONALISATION_WRONG_PWD 4
#define TAF_PH_ME_PERSONALISATION_LOCK_PHONE_TO_SIM 5
#define TAF_PH_ME_PERSONALISATION_NO_SIM 6
#define TAF_PH_ME_PERSONALISATION_OP_NOT_ALLOW 7

#define TAF_PH_CARDLOCK_DEFAULT_MAXTIME 10

typedef struct {
    TAF_OP_ME_PERSONALISATION_RESULT         opRslt;               /* �ƶ��豸˽�в������ */
    TAF_ME_PersonalisationCmdType            cmdType;              /* �ƶ��豸˽�в����������� */
    TAF_ME_PersonalisationStatusFlag         activeStatus;         /* �ƶ��豸˽�в�������״̬ */
    TAF_ME_PersonalisationType               mePersonalisatonType; /* �ƶ��豸˽������ */
    TAF_ME_PERSONALISATION_REPORT_CONTENT_UN unReportContent;      /* �ƶ��豸˽�в����ϱ����� */
} TAF_PH_MePersonlisationCnf;

typedef struct {
    TAF_UINT8 len;
    TAF_UINT8 sW1;
    TAF_UINT8 sW2;
    VOS_UINT8 reserved[1];
    TAF_UINT8 content[USIMM_T0_APDU_MAX_LEN];
} TAF_PH_RestrictedAccessCnf;
/* PIN�����������+CPIN,+CPWD,+CLCK */
/*
 * �ײ�API�ṩ�ĺ������õ����ݽṹ
 * TAF_UINT8 ucCmdType, TAF_UINT8 ucPINType, TAF_UINT8 *pucOldPIN, TAF_UINT8 *pucNewPIN
 */

#define TAF_PH_PINCODELENMAX 8
#define TAF_PH_PINCODELENMIN 4
#define TAF_PH_UNLOCKCODELEN 16

#define TAF_SIM_PIN 0       /* PIN1 */
#define TAF_SIM_PUK 1       /* PUK1 */
#define TAF_SIM_PIN2 6      /* PIN2 */
#define TAF_SIM_PUK2 7      /* PUK2 */
#define TAF_PHSIM_PIN 8     /* PH-SIM PIN */
#define TAF_PHNET_PIN 9     /* PH-NET PIN */
#define TAF_PHNET_PUK 10    /* PH-NET PUK */
#define TAF_PHNETSUB_PIN 11 /* PH-NETSUB PIN */
#define TAF_PHNETSUB_PUK 12 /* PH-NETSUB PUK */
#define TAF_PHSP_PIN 13     /* PH-SP PIN */
#define TAF_PHSP_PUK 14     /* PH-SP PUK */
#define TAF_PHCP_PIN 15     /* PH-SP PIN */
#define TAF_PHCP_PUK 16     /* PH-SP PUK */

#define TAF_SIM_NON 255 /* ��ȷ������PIN������ʱ�����ֵ */
typedef TAF_UINT8 TAF_PH_PIN_TYPE;

typedef TAF_UINT8 TAF_PH_PIN_CMD_TYPE;
#define TAF_PIN_VERIFY 0x20  /* У��PIN�� */
#define TAF_PIN_CHANGE 0x24  /* ����PIN������� */
#define TAF_PIN_DISABLE 0x26 /* ��ֹʹ��PIN�� */
#define TAF_PIN_ENABLE 0x28  /* ʹ��PIN�� */
#define TAF_PIN_UNBLOCK 0x2c /* �����ֹPIN�� */

/* ����PIN���ѯ���� */
#define TAF_PIN_QUERY 0x30 /* ��ѯPIN��״̬ */
#define TAF_PIN_RemainTime 0x31
#define TAF_PIN2_QUERY 0x32 /* ��ѯPIN2��״̬ */
#define TAF_PIN_CMD_NULL 0x00

typedef struct {
    TAF_PH_PIN_CMD_TYPE cmdType;
    TAF_PH_PIN_TYPE     pinType;
    TAF_UINT8           oldPin[TAF_PH_PINCODELENMAX + 1];
    TAF_UINT8           newPin[TAF_PH_PINCODELENMAX + 1];
} TAF_PH_PinData;

/* ��Ϊ��ѯʱ,PIN���صĽ������Ҫ�Ľṹ�� */
typedef TAF_UINT8 TAF_PH_USIMM_NEED_TYPE;

#define TAF_PH_USIMM_NONEED 0 /* ����Ҫ��֤ */
#define TAF_PH_USIMM_NEED 1   /* ��Ҫ��֤ */
#define TAF_PH_USIMM_PUK 2    /* need verifer puk */
#define TAF_PH_USIMM_ADM 3    /* puk block */

typedef TAF_UINT8 TAF_PH_USIMM_ENABLE_FLG;
#define TAF_PH_USIMM_ENABLE 1   /* �Ѿ�ʹ�� */
#define TAF_PH_USIMM_UNENABLE 0 /* δʹ�� */

/* ��ѯUSIM״̬API������� */
/* ��ѯ�����PIN��� */
typedef TAF_UINT8 TAF_OP_PIN_RESULT;
#define TAF_PH_OP_PIN_OK 0
#define TAF_PH_PIN_ERROR 1
#define TAF_PH_QUERY_USIMM_WRONG_PARA 2
#define TAF_PH_QUERY_USIMM_NOAPP 3
#define TAF_PH_QUERY_USIMM_NOINITIAL 4
#define TAF_PH_QUERY_USIMM_ALLOCMEM_FAILED 5
#define TAF_PH_QUERY_USIMM_SENDMSG_FAILED 6
#define TAF_PH_OP_PIN_NEED_PIN1 7
#define TAF_PH_OP_PIN_NEED_PUK1 8
#define TAF_PH_OP_PIN_NEED_PIN2 9
#define TAF_PH_OP_PIN_NEED_PUK2 10
#define TAF_PH_OP_PIN_INCORRECT_PASSWORD 11
#define TAF_PH_OP_PIN_OPERATION_NOT_ALLOW 12
#define TAF_PH_OP_PIN_SIM_FAIL 13

typedef struct {
    TAF_PH_USIMM_NEED_TYPE  usimmNeedType;
    TAF_PH_USIMM_ENABLE_FLG usimmEnableFlg;
    VOS_UINT8               reserved[2];
} TAF_PH_PinQueryCnf;

typedef struct {
    TAF_UINT8 result;
} TAF_PH_PinOperationCnf;

typedef struct {
    TAF_UINT8 pin1RemainTime; /* PIN1��ʣ����� */
    TAF_UINT8 pin2RemainTime; /* PIN2��ʣ����� */
    TAF_UINT8 puk1RemainTime; /* PUK1��ʣ����� */
    TAF_UINT8 puk2RemainTime; /* PUK2��ʣ����� */
} TAF_PH_PinRemain;
/* ������δ��ʼ���ǣ�Ϊ255 */
#define MMA_PIN_REMAINS_UNINIT 255

typedef struct {
    TAF_OP_PIN_RESULT   opPinResult; /* PIN�����Ľ�� */
    TAF_PH_PIN_CMD_TYPE cmdType;     /* ��PIN�Ķ������� */
    TAF_PH_PIN_TYPE     pinType;     /* ��ѯ�����PIN���� */
    TAF_UINT8           remain;      /* ��PIN����ʣ����� */
    TAF_PH_PinQueryCnf  queryResult; /* ��ѯPIN���ؽ�� */
    TAF_PH_PinRemain    remainTime;  /* ����PIN���ʣ����� */
} TAF_PH_PinCnf;

typedef struct {
    TAF_OP_PIN_RESULT   opPinResult; /* PIN�����Ľ�� */
    TAF_PH_PIN_CMD_TYPE cmdType;     /* ��PIN�Ķ������� */
    TAF_PH_PIN_TYPE     pinType;     /* ��ѯ�����PIN���� */
    TAF_UINT8           pIN1Remain;  /* ��PIN����ʣ����� */
    TAF_UINT8           pUK1Remain;
    TAF_UINT8           pIN2Remain;
    TAF_UINT8           pUK2Remain;
    VOS_UINT8           reserved2[1];
    TAF_PH_PinQueryCnf  queryResult; /* ��ѯPIN���ؽ�� */
} TAF_PH_PinTimecnf;

typedef TAF_UINT32 TAF_FPLMN_OPERATE_TYPE;
#define TAF_PH_FPLMN_QUREY 0
#define TAF_PH_FPLMN_DEL_ALL 1
#define TAF_PH_FPLMN_DEL_ONE 2
#define TAF_PH_FPLMN_ADD_ONE 3

typedef struct {
    TAF_FPLMN_OPERATE_TYPE cmdType;
    TAF_PLMN_Id            plmn;
} TAF_PH_FplmnOperate;

/*
 * USIM״̬�ṹ��
 * AT��ʱδ��MMA��ȡUSIM״̬��
 * ��MMA����Ҳ��ҪUSIM״̬
 */
/*
 * 0��     USIM��״̬��Ч
 * 1:      USIM��״̬��Ч
 * 2��     USIM��CS����Ч
 * 3��     USIM��PS����Ч
 * 4��     USIM��PS+CS�¾���Ч
 * 255��   USIM��������
 */
typedef TAF_UINT8 TAF_USIM_STATUS;
#define TAF_PH_USIM_STATUS_UNAVAIL 0
#define TAF_PH_USIM_STATUS_AVAIL 1

#define TAF_PH_USIM_STATUS_CS_UNVAIL 2
#define TAF_PH_USIM_STATUS_PS_UNVAIL 3
#define TAF_PH_USIM_STATUS_CSPS_UNVAIL 4

#define TAF_PH_USIM_ROMSIM 240
#define TAF_PH_USIM_NON 255

/* ��ȡ��Ӫ����Ϣ���ɻ���AT�����COPS�� */
typedef TAF_UINT8 TAF_PH_NETWORKNAME_FORMAT;
#define TAF_PH_NETWORKNAME_LONG 0
#define TAF_PH_NETWORKNAME_SHORT 1
#define TAF_PH_NETWORKNAME_NUMBER 2
#define TAF_PH_NETWORKNAME_ALL 4
/*
 * ��ǰע������״̬��ʶ
 * 0:δ֪��
 * 1:���õ�
 * 2:��ǰע��
 * 3:��ֹ
 */
typedef TAF_UINT8 TAF_PH_NETWORK_STATUS_FLAG;
#define TAF_PH_NETWORK_STATUS_UNKNOW 0
#define TAF_PH_NETWORK_STATUS_AVAILIBLE 1
#define TAF_PH_NETWORK_STATUS_CURRENT 2
#define TAF_PH_NETWORK_STATUS_FORBIDDEN 3

typedef TAF_UINT8 TAF_PH_RA_MODE; /* ����ģʽ */
#define TAF_PH_RA_GSM 0
#define TAF_PH_RA_WCDMA 1
#define TAF_PH_RA_LTE 2
#define TAF_PH_RA_NR 3
#define TAF_PH_RA_MODE_DEFAULT 4

#define TAF_NAMELENGTH_MAX 32

/* STK���� Access Technology Change Event ��غ궨�� */
#define TAF_STK_ACC_TECH_GSM 0
#define TAF_STK_ACC_TECH_TIA_EIA_553 1
#define TAF_STK_ACC_TECH_TIA_EIA_136 2
#define TAF_STK_ACC_TECH_UTRAN 3
#define TAF_STK_ACC_TECH_TETRA 4
#define TAF_STK_ACC_TECH_TIA_EIA_95 5
#define TAF_STK_ACC_TECH_TIA_EIA_IS_2000 6

/* NV���Ƿ񼤻�ָʾ */
#define NV_ITEM_DEACTIVE 0
#define NV_ITEM_ACTIVE 1

/* ��ȡ������PLMN�б��ϱ� */
#define TAF_PH_OPER_NAME_LONG (100)
#define TAF_PH_OPER_NAME_SHORT (100)
#define TAF_PH_OPER_NAME (100)

#define TAF_PH_BITS_PER_OCTET 8
#define TAF_PH_BITS_PER_SEPTET 7

/* #define TAF_PH_OPER_NAME_PLMN_ID_LENGTH         8 */

#define TAF_PH_MAX_PLMN_ADDITIONAL_INFO_LEN (50)

typedef struct {
    TAF_PLMN_Id plmnId;
    TAF_CHAR    operatorNameShort[TAF_PH_OPER_NAME_SHORT];
    TAF_CHAR    operatorNameLong[TAF_PH_OPER_NAME_LONG];
} TAF_PH_OperatorName;

typedef TAF_UINT8 TAF_PH_GET_OPERATOR_INFO_OP;
#define TAF_PH_OPER_NAME_PLMN2ALPHANUMERICNAME 0
#define TAF_PH_OPER_NAME_SHORTNAME2OTHERS 1
#define TAF_PH_OPER_NAME_LONGNAME2OTERS 2
#define TAF_PH_OPER_NAME_ERROROP 3

typedef struct {
    TAF_PH_OperatorName         operName;
    TAF_PH_GET_OPERATOR_INFO_OP listOp;
    VOS_UINT8                   reserved[3];
} TAF_PH_OperatorNameFormatOp;

typedef TAF_UINT8 TAF_PH_PLMN_SEL_MODE;           /* PLMN selection mode     */
#define TAF_PH_PLMN_SEL_MODE_AUTO 0               /* automatic selection mode */
#define TAF_PH_PLMN_SEL_MODE_MANUAL 1             /* manual selection mode   */
#define TAF_PH_PLMN_SEL_MODE_DEREG_FROM_NETWORK 2 /* deregister form network */
#define TAF_PH_PLMN_SEL_MODE_BUTT (0xFF)

typedef struct {
    TAF_PH_OperatorName       name;
    TAF_PH_NETWORKNAME_FORMAT nameFormat;  /* ������Ӫ�����ָ�ʽ */
    TAF_PH_RA_MODE            raMode;      /* ���߽���ģʽ,3G/2G */
    TAF_PH_PLMN_SEL_MODE      plmnSelMode; /* PLMN selection mode */
    VOS_UINT8                 reserved[1];
} TAF_PH_Networkname;

typedef struct {
    TAF_PH_NETWORK_STATUS_FLAG status;
    VOS_UINT8                  reserved1[3];
    TAF_PLMN_Id                plmn;
    TAF_PH_RA_MODE             raMode;
    VOS_UINT8                  reserved2[3];
} TAF_PLMN_Info;

/* ��ȡbandֵ����MS��RACIEZ�л�� */
/*
 * 80��CM_BAND_PREF_GSM_DCS_1800��              GSM DCS systems
 * 100��CM_BAND_PREF_GSM_EGSM_900��             Extended GSM 900
 * 200��CM_BAND_PREF_GSM_PGSM_900��             Primary GSM 900
 * 100000��CM_BAND_PREF_GSM_RGSM_900��          GSM Railway GSM 900
 * 200000��CM_BAND_PREF_GSM_PCS_1900��          GSM PCS
 * 400000��CM_BAND_PREF_WCDMA_I_IMT_2000��      WCDMA IMT 2000
 * 3FFFFFFF��CM_BAND_PREF_ANY��                 �κ�Ƶ��
 * 40000000��CM_BAND_PREF_NO_CHANGE��           Ƶ�����仯
 */
typedef struct {
    TAF_UINT32 bandLow;  /* ��32λ */
    TAF_UINT32 bandHigh; /* ��32λ */
} TAF_UserSetPrefBand64;


typedef struct {
    TAF_UINT32 bandInfo[TAF_MMA_LTE_BAND_MAX_LENGTH]; /* ��8��U32�ֶα�ʶLTEƵ�� */
} TAF_USER_SetLtePrefBandInfo;


#define TAF_PH_BAND_GSM_DCS_1800 0x00000080
#define TAF_PH_BAND_GSM_EGSM_900 0x00000100
#define TAF_PH_BAND_GSM_PGSM_900 0x00000200
#define TAF_PH_BAND_WCDMA_VII_2600 0x00010000
#define TAF_PH_BAND_WCDMA_VIII_900 0x00020000
#define TAF_PH_BAND_WCDMA_IX_1700 0x00040000
#define TAF_PH_BAND_GSM_850 0x00080000
#define TAF_PH_BAND_GSM_RGSM_900 0x00100000
#define TAF_PH_BAND_GSM_PCS_1900 0x00200000
#define TAF_PH_BAND_WCDMA_I_IMT_2100 0x00400000
#define TAF_PH_BAND_WCDMA_II_PCS_1900 0x00800000
#define TAF_PH_BAND_WCDMA_III_1800 0x01000000
#define TAF_PH_BAND_WCDMA_IV_1700 0x02000000
#define TAF_PH_BAND_WCDMA_V_850 0x04000000
#define TAF_PH_BAND_WCDMA_VI_800 0x08000000
#define TAF_PH_BAND_WCDMA_XI_1500 0x20000000
/* ���Ӷ�BAND19��֧�� */
#define TAF_PH_BAND_WCDMA_XIX_850 0x10000000
#define TAF_PH_BAND_ANY 0x3FFFFFFF
#define TAF_PH_BAND_NO_CHANGE 0x40000000

#define MN_MMA_LTE_HIGH_BAND_ANY (0x7FFFFFFF)
#define MN_MMA_LTE_LOW_BAND_ANY (0xFFFFFFFF)
#define MN_MMA_LTE_EUTRAN_BAND1 (0x1)
#define MN_MMA_LTE_EUTRAN_BAND7 (0x40)
#define MN_MMA_LTE_EUTRAN_BAND13 (0x1000)
#define MN_MMA_LTE_EUTRAN_BAND17 (0x10000)
#define MN_MMA_LTE_EUTRAN_BAND40 (0x80)

#define MN_MMA_LTE_LOW_BAND_ALL \
    (MN_MMA_LTE_EUTRAN_BAND1 | MN_MMA_LTE_EUTRAN_BAND7 | MN_MMA_LTE_EUTRAN_BAND13 | MN_MMA_LTE_EUTRAN_BAND17)

#define MN_MMA_LTE_HIGH_BAND_ALL (MN_MMA_LTE_EUTRAN_BAND40)

/* ����BAND6,BAND19��֧�� */
#define TAF_PH_BAND_WCDMA_BAND_ALL                                                                                 \
    (TAF_PH_BAND_WCDMA_I_IMT_2100 | TAF_PH_BAND_WCDMA_II_PCS_1900 | TAF_PH_BAND_WCDMA_IV_1700 |                    \
     TAF_PH_BAND_WCDMA_V_850 | TAF_PH_BAND_WCDMA_VI_800 | TAF_PH_BAND_WCDMA_VIII_900 | TAF_PH_BAND_WCDMA_XI_1500 | \
     TAF_PH_BAND_WCDMA_XIX_850)
#define TAF_PH_BAND_GSM_BAND_ALL                                                                            \
    (TAF_PH_BAND_GSM_DCS_1800 | TAF_PH_BAND_GSM_EGSM_900 | TAF_PH_BAND_GSM_PGSM_900 | TAF_PH_BAND_GSM_850 | \
     TAF_PH_BAND_GSM_PCS_1900)

#define TAF_PH_BAND_ALL (TAF_PH_BAND_WCDMA_BAND_ALL | TAF_PH_BAND_GSM_BAND_ALL)

/* ���ڷ���SYSCFG=?��Ƶ�η���1�ķ�Χ */

#define TAF_PH_BAND_GSM_900 (TAF_PH_BAND_GSM_EGSM_900 | TAF_PH_BAND_GSM_PGSM_900)

/* ���Ӷ�BAND19��֧�� */
#define TAF_PH_BAND_GROUP1_FOR_SYSCFG                                                                            \
    (TAF_PH_BAND_WCDMA_I_IMT_2100 | TAF_PH_BAND_WCDMA_VIII_900 | TAF_PH_BAND_WCDMA_IX_1700 |                     \
     TAF_PH_BAND_WCDMA_VI_800 | TAF_PH_BAND_GSM_DCS_1800 | TAF_PH_BAND_GSM_EGSM_900 | TAF_PH_BAND_GSM_PGSM_900 | \
     TAF_PH_BAND_WCDMA_XI_1500 | TAF_PH_BAND_WCDMA_XIX_850)
/* ���ڷ���SYSCFG=?��Ƶ�η���2�ķ�Χ */
#define TAF_PH_BAND_GROUP2_FOR_SYSCFG                                                                                 \
    (TAF_PH_BAND_WCDMA_II_PCS_1900 | TAF_PH_BAND_WCDMA_IV_1700 | TAF_PH_BAND_WCDMA_V_850 | TAF_PH_BAND_GSM_PCS_1900 | \
     TAF_PH_BAND_GSM_850)

/* ��Ҫ��64λ��32λת����Ƶ�� */
#define TAF_PH_BAND_NEED_CHANGE_TO_64BIT \
    (TAF_PH_BAND_WCDMA_VIII_900 | TAF_PH_BAND_WCDMA_IX_1700 | TAF_PH_BAND_WCDMA_XI_1500 | TAF_PH_BAND_WCDMA_XIX_850)

typedef struct {
    VOS_UINT32 bandMask; /* Ƶ������ */
    TAF_CHAR  *bandStr;  /* Ƶ���ַ��� */
} TAF_PH_BandName;

#define TAF_DEVICE_AT_MAX_W_BAND_NUM 9 /* ��ǰ�û������õ�WƵ�η�Χ�� */

/* ����֧��ѡ��0��1��ʹ�ã�ֻʹ��δ�仯��2�� */

enum TAF_MMA_RoamMode {
    TAF_MMA_ROAM_NATIONAL_ON_INTERNATIONAL_ON,   /* �������ڹ������� */
    TAF_MMA_ROAM_NATIONAL_ON_INTERNATIONAL_OFF,  /* �����������Σ��رչ������� */
    TAF_MMA_ROAM_NATIONAL_OFF_INTERNATIONAL_ON,  /* �رչ������Σ������������� */
    TAF_MMA_ROAM_NATIONAL_OFF_INTERNATIONAL_OFF, /* �رչ��ڹ������� */
    TAF_MMA_ROAM_UNCHANGE,                       /* ��������δ�ı� */

    TAF_MMA_ROAM_BUTT
};
typedef VOS_UINT8 TAF_MMA_RoamModeUint8;

/* ����״̬ */
typedef TAF_UINT8 TAF_PH_ROAM_STA;
#define TAF_PH_ROAMING 0
#define TAF_PH_NO_ROAM 1
#define TAF_PH_ROAM_UNKNOW 2

#if (FEATURE_ON == FEATURE_LTE)
/* CELLROAM��ʽ���� */
enum {
    TAF_MMA_CELLROAM_RAT_NONE = 0,
    TAF_MMA_CELLROAM_RAT_GSM  = 1, /* GSM */
    TAF_MMA_CELLROAM_RAT_WCDMA,    /* WCDMA */
    TAF_MMA_CELLROAM_RAT_LTE,      /* LTE */
    TAF_MMA_CELLROAM_RAT_NR,       /* NR */
    TAF_MMA_CELLROAM_RAT_BUTT
};
typedef VOS_UINT8 TAF_MMA_CellRoamRatMode;

/* CELLROAM�������Ͷ��� */
enum {
    TAF_MMA_CELLROAM_UNKNOWN = 0, /* δ֪ */
    TAF_MMA_CELLROAM_DOMESTIC,    /* ���� */
    TAF_MMA_CELLROAM_FOREIGN,     /* ���� */
    TAF_MMA_CELLROAM_BUTT
};
typedef VOS_UINT8 TAF_MMA_CellRoamMode;

typedef struct {
    TAF_MMA_CellRoamRatMode ratMode;   /* ���߽���ģʽ,3G/2G */
    TAF_MMA_CellRoamMode    roamMode;  /* ROAM mode */
    VOS_UINT8               rsv[2];
} TAF_PH_Cellroam;
#endif

/* ------------    ��״̬��־��     ------------ */
/* ��״̬��־�������� */
/*
 * 0��    �޷���
 * 1��    ��CS����
 * 2��    ��PS����
 * 3��    PS+CS����
 * 4��    CS��PS��δע�ᣬ����������״̬
 */
typedef TAF_UINT8 TAF_PH_DOMAIN_FLAG;

typedef struct {
    VOS_UINT32            band;       /* Ƶ������ */
    TAF_PH_DOMAIN_FLAG    domainFlag; /* ��״̬��־ */
    TAF_MMA_RoamModeUint8 roamMode;   /* ����ѡ�� */
    /* ֻ��,����д,��ȡ��ǰ����״̬ */
    TAF_PH_ROAM_STA roamSta;
    VOS_UINT8       reserved[1];
} TAF_PH_SystemMode;

#define TAF_SPDI_PLMN_LIST_IEI (0x80)

typedef TAF_UINT8 TAF_PH_SPN_DISP_MODE;

typedef TAF_UINT8 TAF_PH_SPN_CODING;
#define TAF_PH_GSM_7BIT_DEFAULT 0
#define TAF_PH_RAW_MODE 1
#define TAF_PH_SPN_CODING_UNKNOW 255

/* �˴��궨����TAF_SDC_MAX_OPER_NAME_NUM �󶨣����ɸ��� */
#define TAF_PH_SPN_NAME_MAXLEN 100

/* ������AT�����ȡSPN�ļ���Ӧ */
typedef struct {
    TAF_PH_SPN_DISP_MODE dispRplmnMode;
    TAF_UINT8            length;
    TAF_UINT8            coding;
    TAF_UINT8            spareBit;
    TAF_UINT8            spnName[TAF_PH_SPN_NAME_MAXLEN];
} TAF_PH_UsimSpn;

/* USIM��SPN�ļ����� */
typedef struct {
    TAF_UINT8 dispRplmnMode;
    TAF_UINT8 spnName[TAF_PH_SPN_NAME_MAXLEN];
    VOS_UINT8 reserved[3];
} TAF_PH_UsimSpnCnf;

/* �ϱ���Ӧ�ò��SPN��Ϣ */
typedef struct {
    TAF_PH_SPN_DISP_MODE dispRplmnMode;
    TAF_PH_ROAM_STA      phRoam;
    TAF_UINT8            spnName[TAF_PH_SPN_NAME_MAXLEN];
    VOS_UINT8            reserved[2];
} TAF_PH_SpnInfo;

#define FULL_NAME_IEI 0x43
#define SHORT_NAME_IEI 0x45
#define PLMN_ADDITIONAL_INFO_IEI 0x80

typedef struct {
    VOS_BOOL  cphsOperNameValid;
    VOS_UINT8 cphsOperNameStr[TAF_PH_OPER_NAME_LONG + 1];
    VOS_UINT8 reserved[3];
} TAF_PH_SimCphsOperName;


typedef struct {
    VOS_UINT8 ext : 1;
    VOS_UINT8 coding : 3; /* �����ʽ */
    VOS_UINT8 addCi : 1;
    VOS_UINT8 spare : 3; /* spare bit */
    VOS_UINT8 length;    /* ��Ӫ�����Ƴ��� */
    VOS_UINT8 reserved[2];
    VOS_UINT8 operatorName[TAF_PH_OPER_NAME]; /* ��Ӫ�̵����� */
} TAF_PH_OperName;

typedef struct {
    TAF_PH_OperName operNameLong;
    TAF_PH_OperName operNameShort;
    VOS_UINT8       plmnAdditionalInfoLen; /* PLMN Additional Information ���� */
    /* PLMN Additional Information ����,��ʽΪV */
    VOS_UINT8 plmnAdditionalInfo[TAF_PH_MAX_PLMN_ADDITIONAL_INFO_LEN];
    VOS_UINT8 reserve[1];
} TAF_PH_UsimPnnRecord;

typedef TAF_UINT8 TAF_PH_IccType;
#define TAF_PH_ICC_UNKNOW 0
#define TAF_PH_ICC_SIM 1
#define TAF_PH_ICC_USIM 2

typedef struct {
    TAF_UINT32           cardType;
    TAF_UINT32           totalRecordNum;
    VOS_UINT16           pnnCurrIndex; /* ��ѯPNN����ʼλ�� */
    TAF_PH_IccType       icctype;
    VOS_UINT8            reserve[1];
    TAF_PH_UsimPnnRecord pnnRecord[1];
} TAF_PH_UsimPnnCnf;

typedef struct {
    TAF_UINT16 plmnLen;
    TAF_UINT8  plmn[6];
    TAF_UINT16 lACLow;
    TAF_UINT16 lACHigh;
    TAF_UINT16 pnnIndex;
    VOS_UINT8  reserved[2];
} TAF_PH_UsimOplRecord;

typedef struct {
    TAF_UINT32           cardType;
    TAF_UINT32           totalRecordNum;
    VOS_CHAR             wildCard;
    VOS_UINT8            reserved[3];
    TAF_PH_UsimOplRecord oplRecord[1];
} TAF_PH_UsimOplCnf;

typedef struct {
    TAF_PH_UsimPnnCnf *pnnCnf;
    TAF_PH_UsimOplCnf *oplCnf;
} TAF_PH_UsimCpnnCnf;

/* С����Ϣ */
#define TAFMMA_CELL_MAX_NUM 8
typedef struct {
    TAF_UINT8   cellNum;
    VOS_UINT8   reserved[3];
    TAF_CELL_Id cellId[TAFMMA_CELL_MAX_NUM];
} TAF_CELL_Info;

typedef struct {
    TAF_UINT32             OP_CsRegState : 1;
    TAF_UINT32             OP_PsRegState : 1;
    TAF_UINT32             OP_ucAct : 1;
    TAF_UINT32             OP_Lac : 1;
    TAF_UINT32             OP_CellId : 1;
    TAF_UINT32             OP_Rac : 1;
    TAF_UINT32             OP_Plmn : 1;
    TAF_UINT32             OP_Spare : 25;
    TAF_PH_REG_STATE_TYPE  regState;   /* MMע��״̬ */
    TAF_PH_REG_STATE_TYPE  psRegState; /* PSע��״̬ */
    TAF_PH_AccessTechUint8 act;        /* ��ǰפ������Ľ��뼼�� */
    TAF_UINT8              rac;        /* RAC */
    VOS_UINT8              ratType;
    VOS_UINT8              reserved[3];
    TAF_UINT32             lac;    /* λ������Ϣ */
    TAF_CELL_Info          cellId; /* С��ID */
    TAF_PLMN_Id            plmn;   /* �ṩPLMN��Ϣ����ǰδ������ */
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    PS_ALLOW_Nssai allowedNssai; /* �������Ƭ��Ϣ */
#endif
} TAF_PH_RegState;

/* ����Ϣ��������NVIM����ȡ */
typedef struct {
    TAF_PH_Imei        imeisV;                 /* IMEISV */
    TAF_PH_FmrId       fmrId;                  /* �ֻ����쳧����Ϣ */
    TAF_PH_ProductName productName;            /* ��Ʒ���� */
    TAF_PH_ModelId     modelId;                /* �ֻ�Ӳ���汾��Ϣ */
    TAF_PH_RevisionId  softVersion;            /* �ֻ�����汾�� */
    TAF_PH_ReleaseDate releaseDate;            /* �������� */
    TAF_PH_ReleaseTime releaseTime;            /* ����ʱ�� */
    TAF_PH_Compcaplist completeCapitiyList;    /* ���������б� */
    TAF_PH_Compcaplist gsmCompleteCapitiyList; /* ���������б� */
} TAF_PH_Meinfo;

#define TAF_PH_SYSCFG_GROUP_BAND_LEN 256

typedef struct {
    TAF_CHAR strSysCfgBandGroup[TAF_PH_SYSCFG_GROUP_BAND_LEN];

} TAF_PH_SysCfgBand;

typedef struct {
    TAF_PH_ProductName productName; /* ���ݿ����� */
    TAF_PH_FmrId       fmrId;       /* �ֻ����쳧����Ϣ */
    TAF_PH_PortInfo    portId;      /* ���ݿ��˿ڷ�����Ϣ */
} TAF_PH_Port;

/* Taf_PhoneNetworkOperate */
typedef TAF_UINT8 TAF_AVAIL_PLMN_LIST_TYPE;
#define TAF_PLMN_LIST 0       /* ��������PLMN���� */
#define TAF_PLMN_LIST_ABORT 1 /* ��ֹ����PLMN���� */

typedef TAF_UINT8 TAF_PLMN_RESEL_MODE_TYPE;
#define TAF_PLMN_RESELETION_AUTO 0   /* PLMN��ѡ���Զ� */
#define TAF_PLMN_RESELETION_MANUAL 1 /* PLMN��ѡ���ֹ� */

typedef struct {
    TAF_PH_NETWORK_STATUS_FLAG plmnStatus;
    TAF_PH_RA_MODE             raMode; /* ���߽���ģʽ,3G/2G */
    VOS_UINT8                  reserved[2];
} TAF_PLMN_Status;

typedef TAF_UINT8 AT_COPS_FORMAT_TYPE;
#define AT_COPS_LONG_ALPH_TYPE 0
#define AT_COPS_SHORT_ALPH_TYPE 1
#define AT_COPS_NUMERIC_TYPE 2


enum TAF_MMA_ExtendSearchType {
    TAF_MMA_EXTEND_SEARCH_TYPE_FULLBAND = 0,
    TAF_MMA_EXTEND_SEARCH_TYPE_HISTORY  = 1,
    TAF_MMA_EXTEND_SEARCH_TYPE_PREFBAND = 2,
    TAF_MMA_EXTEND_SEARCH_TYPE_DETECT   = 3,
    TAF_MMA_EXTEND_SEARCH_TYPE_BUTT
};

typedef VOS_UINT8 TAF_MMA_ExtendSearchTypeUint8;


typedef struct {
    AT_COPS_FORMAT_TYPE           formatType;
    TAF_MMA_RatTypeUint8          accessMode;
    VOS_UINT16                    operNameLen;
    VOS_CHAR                      operName[TAF_PH_OPER_NAME_LONG];
    VOS_UINT8                     bgSrchFlag; /* 0:ָ���ѣ�1:������ */
    TAF_MMA_ExtendSearchTypeUint8 srchType;
    VOS_UINT8                     resv[2];
    VOS_UINT8                     rsrpThresholdFlg; /* �Ƿ�֧���ź�ǿ����ֵ���� */
    VOS_UINT8                     reserved;
    VOS_INT16                     rsrp; /* [-141,-44] */
} TAF_PLMN_UserSel;


typedef struct {
    TAF_MMA_RatTypeUint8 accessMode; /* ��ǰ̽��ֻ֧��L,������ʽֱ�ӷ���ERROR */
    VOS_UINT8            reserved[3];
    TAF_PLMN_Id          plmnId;
} TAF_DETECT_Plmn;


/* ����һ��PLMN NAME��������������100�ֽ����ң����˼�ͨ�Ż������Ϊ4K������ֻ�ܴ�Ÿ�����PLMN NAME */
#define TAF_MMA_MAX_PLMN_NAME_LIST_NUM 12
typedef struct {
    TAF_UINT32          plmnNum;
    VOS_BOOL            plmnSelFlg;
    TAF_PH_OperatorName plmnName[TAF_MMA_MAX_PLMN_NAME_LIST_NUM];
    TAF_MMC_USIM_RAT    plmnRat[TAF_MMA_MAX_PLMN_NAME_LIST_NUM];
} TAF_PLMN_NameList;

#define TAF_MAX_PLMN_NUM 37
typedef struct {
    TAF_UINT8       plmnNum;
    VOS_UINT8       reserved[3];
    TAF_PLMN_Id     plmn[TAF_MAX_PLMN_NUM];
    TAF_PLMN_Status plmnInfo[TAF_MAX_PLMN_NUM];
} TAF_PLMN_List;

/*
 * ȡMMC_MAX_OPLMN_NUM  NAS_MMC_MAX_UPLMN_NUM MMC_MAX_SELPLMN_NUM
 * MMC_MAX_HPLMN_WITH_ACT_NUM                           �е����ֵ
 */
#define TAF_USER_MAX_PLMN_NUM 256
typedef struct {
    TAF_UINT16       plmnNum;
    TAF_UINT8        reserved[2];
    TAF_PLMN_Id      plmn[TAF_USER_MAX_PLMN_NUM];
    TAF_MMC_USIM_RAT plmnRat[TAF_USER_MAX_PLMN_NUM];
} TAF_USER_PlmnList;

typedef struct {
    TAF_UINT8               index;
    VOS_UINT8               reserved1[3];
    TAF_PLMN_Id             plmn;
    MN_MMA_CpamRatTypeUint8 plmnRat;
    VOS_UINT8               reserved2[3];
} TAF_USER_PlmnInfo;

typedef struct {
    TAF_UINT8 len;
    VOS_UINT8 reserved[3];
    TAF_UINT8 command[USIMM_T0_APDU_MAX_LEN];
} TAF_PH_UsimCommand;

/* ucActionType��ȡֵ */
#define TAF_START_EVENT_INFO_FOREVER 0 /* ָʾ�����ϱ����� */
#define TAF_START_EVENT_INFO_ONCE 1    /* ָʾ�ϱ����һ�� */
#define TAF_START_EVENT_INFO_STOP 2    /* ָʾֹͣ�ϱ� */

/* ucMsgType��ȡֵ */
#define TAF_EVENT_INFO_CELL_SIGN 1 /* ��ʾ��ȡС���ź�ǿ�� */
#define TAF_EVENT_INFO_CELL_ID 2   /* ��ʾ��ȡС��ID */
#define TAF_EVENT_INFO_CELL_BLER 4 /* ��ʾ��ȡ�ŵ������� */

/* �����¼������ϱ���Ϣ�Ľṹ�� */
typedef struct {
    TAF_UINT8 actionType;
    TAF_UINT8 rrcMsgType;

    VOS_UINT8 signThreshold;       /*
                                    * 1:rssi�仯����1db�����ϱ�^cerssi; 2:rssi�仯����2db�����ϱ�
                                    * 3:rssi�仯����3db�����ϱ���4:rssi�仯����4db�����ϱ���5:rssi�仯����5db�����ϱ�;
                                    */
    VOS_UINT8 minRptTimerInterval; /* �ϱ��ļ��ʱ��(1-20)S */
} TAF_START_InfoInd;

typedef struct {
    TAF_UINT16 highFreq;
    TAF_UINT16 lowFreq;
} TAF_PH_FreqInfo;

/* Ƶ����Ϣ�ṹ�� */
typedef struct {
    TAF_UINT32      gsmFreq;
    TAF_PH_FreqInfo wcdmaFrq;
} TAF_PH_Frq;

#define TAF_PH_ICC_ID_MAX 10
typedef struct {
    TAF_UINT8 len;
    TAF_UINT8 iccId[TAF_PH_ICC_ID_MAX + 1];
} TAF_PH_IccId;

/*    ucSrvStatusȡֵ���£� */
#define TAF_PH_INFO_NO_SERV 0         /* �޷��� */
#define TAF_PH_INFO_LIMT_SERV 1       /* �����Ʒ��� */
#define TAF_PH_INFO_NOMRL_SERV 2      /* ������Ч */
#define TAF_PH_INFO_LIMT_REGON_SERV 3 /* �����Ƶ�������� */
#define TAF_PH_INFO_DEEP_SLEEP 4      /* ʡ�����˯״̬ */
/*    ucSrvDomainȡֵ���£� */
#define TAF_PH_INFO_NO_DOMAIN 0     /* �޷��� */
#define TAF_PH_INFO_ONLY_CS 1       /* ��CS���� */
#define TAF_PH_INFO_ONLY_PS 2       /* ��PS���� */
#define TAF_PH_INFO_DUAL_DOMAIN 3   /* PS+CS���� */
#define TAF_PH_INFO_SEARCH_DOMAIN 4 /* CS��PS��δע�ᣬ����������״̬  */
/*    ucRoamStatusȡֵ���£� */
#define TAF_PH_INFO_NONE_ROMAING 0 /* ������״̬ */
#define TAF_PH_INFO_ROMAING 1      /* ����״̬ */
#define TAF_PH_INFO_ROAM_UNKNOW 2  /* δ֪״̬ */

typedef TAF_UINT8 TAF_PH_INFO_RAT_TYPE;
/* ucSysModeȡֵ���£� */

enum TAF_SysMode {
    TAF_PH_INFO_NONE_RAT        = 0,  /* �޷��� */
    TAF_PH_INFO_CDMA_1X_RAT     = 2,  /* CDMA 1Xģʽ */
    TAF_PH_INFO_GSM_RAT         = 3,  /* GSM/GPRSģʽ */
    TAF_PH_INFO_HRPD_RAT        = 4,  /* HRPD ģʽ */
    TAF_PH_INFO_WCDMA_RAT       = 5,  /* WCDMAģʽ */
    TAF_PH_INFO_LTE_RAT         = 7,  /* LTEģʽ */
    TAF_PH_INFO_HYBRID_RAT      = 8,  /* HRPD ���ģʽ */
    TAF_PH_INFO_EUTRAN_5GC_RAT  = 10, /* 4G������ʽ5G������ */
    TAF_PH_INFO_NR_5GC_RAT      = 11, /* 5G������ʽ5G������ */
    TAF_PH_INFO_TD_SCDMA_RAT    = 15, /* TD-SCDMAģʽ */
    TAF_PH_INFO_SVLTE_SRLTE_RAT = 16, /* SVLTE/SRLTEģʽ */
    TAF_PH_INFO_1X_NR_5GC_RAT   = 17, /* 1X_NR_5GCģʽ */
};
typedef VOS_UINT8 TAF_SysModeUint8;

/*    ucSimStatusȡֵ���� */
#define TAF_PH_INFO_USIM_PIN_INVAILD 0 /* USIM��״̬��Ч����PIN��û��У������ */
#define TAF_PH_INFO_USIM_VAILD 1       /* USIM��״̬��Ч */
#define TAF_PH_INFO_USIM_CS_INVAILD 2  /* USIM��CS����Ч */
#define TAF_PH_INFO_USIM_PS_INVAILD 3  /* USIM��PS����Ч */
#define TAF_PH_INFO_USIM_INVAILD 4     /* USIM��PS+CS�¾���Ч */
#define TAF_PH_INFO_USIM_ABSENT 255    /* USIM�������� */

typedef struct {
    TAF_UINT8 srvStatus;     /* ����״̬����֧��ȡֵ0,1,2 */
    TAF_UINT8 srvDomain;     /* ������ */
    TAF_UINT8 roamStatus;    /* ����״̬��������PLMN ID��ͬ���� */
    TAF_UINT8 sysMode;       /* ϵͳģʽ */
    TAF_UINT8 simStatus;     /* SIM��״̬ */
    TAF_UINT8 sysSubMode;    /* ϵͳ��ģʽ */
    TAF_UINT8 simLockStatus; /* SIM����LOCK״̬ */
    VOS_UINT8 reserved[1];
} TAF_PH_Sysinfo;

typedef struct {
    TAF_UINT8  ratType;
    TAF_UINT8  gsmBand;
    TAF_UINT16 indFreq;
} TAF_IND_Freq;

#define TAF_MAX_USIM_RSP_LEN 255
typedef struct {
    TAF_UINT8 len;
    TAF_UINT8 response[TAF_MAX_USIM_RSP_LEN];
} TAF_PH_UsimResponse;

/* ϵͳ�������ݽṹ */
typedef struct {
    TAF_PH_CMD_TYPE            cmdType;
    MN_MMA_CpamRatTypeUint8    accessMode;
    TAF_PH_PLMN_PRIO           plmnPrio;
    TAF_MMA_RoamModeUint8      roam;
    VOS_UINT32                 band;
    TAF_MMA_ServiceDomainUint8 srvDomain;
    VOS_UINT8                  reserved[3];
    TAF_UserSetPrefBand64      origUserSetBand;
} TAF_PH_SysCfg;


typedef struct {
    TAF_MMA_ServiceDomainUint8       srvDomain;         /* ������ 0:CS,1:ps */
    TAF_MMA_CellRestrictionTypeUint8 cellAcType;        /* С���������� */
    VOS_UINT8                        restrictRegister;  /* �Ƿ�����ע�� */
    VOS_UINT8                        restrictPagingRsp; /* �Ƿ�������ӦѰ�� */
} TAF_MMA_CellAcInfo;
typedef TAF_UINT8 TAF_PH_PIN_STATUS;
#define TAF_PH_PIN_ENABLE 0
#define TAF_PH_PIN_DISABLE 1

typedef TAF_UINT8 TAF_PH_PIN_VERIFY_STATUS;
#define TAF_PH_PIN_NEED 0
#define TAF_PH_PIN_NONEED 1
#define TAF_PH_PIN_PUK 2
typedef TAF_UINT8 TAF_PH_ME_LOCK_STATUS;
#define TAF_PH_ME_SIM_UNLOCK 0
#define TAF_PH_ME_SIM_LOCK 1
#define TAF_PH_ME_NO_SIM 2

typedef struct {
    TAF_PH_IccType           mmaIccType;
    TAF_PH_PIN_STATUS        mmaIccPinStatus;
    TAF_PH_PIN_VERIFY_STATUS mmaIccPinVerifyStatus;
    TAF_UINT8                remainTimes;
} TAF_PH_UiccType;

enum MN_PH_PrefPlmnType {
    MN_PH_PREF_PLMN_UPLMN,
    MN_PH_PREF_PLMN_OPLMN,
    MN_PH_PREF_PLMN_HPLMN
};
typedef VOS_UINT8 MN_PH_PrefPlmnTypeUint8;

enum MN_PH_PrefPlmnOperateType {
    MN_PH_PrefPLMN_QUREY,
    MN_PH_PrefPLMN_DEL_ONE,
    MN_PH_PrefPLMN_ADD_ONE,
    MN_PH_PrefPLMN_MODIFY_ONE
};
typedef VOS_UINT8 MN_PH_PrefPlmnOperateTypeUint8;

typedef struct {
    MN_PH_PrefPlmnTypeUint8        prefPLMNType;
    MN_PH_PrefPlmnOperateTypeUint8 prefPlmnOperType;
    TAF_MMC_USIM_RAT               rat; /* USIM��ʽ�� ���뼼�� */
    VOS_UINT32                     index;
    TAF_PLMN_Id                    plmn;
} TAF_PH_PrefPlmnOperate;


typedef struct {
    MN_PH_PrefPlmnTypeUint8 prefPlmnType;                    /* ѡ�е����ȼ������б����� */
    VOS_UINT8               atParaIndex;                     /* �������� */
    VOS_UINT16              indexLen;                        /* ���������ĳ��� */
    VOS_UINT16              formatLen;                       /* ���������͵ĳ��� */
    VOS_UINT16              operNameLen;                     /* ��Ӫ�����ֵĳ��� */
    VOS_UINT16              gsmRatLen;                       /* GSM���뼼���ĳ��� */
    VOS_UINT16              gsmCompactRatLen;                /* GSM COMPACT���뼼���ĳ��� */
    VOS_UINT16              utrnLen;                         /* UTRN���뼼���ĳ��� */
    VOS_UINT16              reserve;                         /* �����ֶ� */
    VOS_UINT32              index;                           /* ����ֵ */
    VOS_UINT32              format;                          /* ��������ֵ */
    VOS_CHAR                operName[TAF_PH_OPER_NAME_LONG]; /* ��Ӫ���� */
    VOS_UINT32              gsmRat;                          /* GSM���뼼��ֵ */
    VOS_UINT32              gsmCompactRat;                   /* GSM COMPACK���뼼��ֵ */
    VOS_UINT32              utrn;                            /* UTRN���뼼��ֵ */
    VOS_UINT16              eutrnLen;                        /* E-UTRN���뼼���ĳ��� */
    VOS_UINT16              nrRatLen;                        /* NR���뼼���ĳ��� */
    VOS_UINT32              eutrn;                           /* E-UTRN���뼼��ֵ */
    VOS_UINT32              nrRat;                           /* NR���뼼��ֵ */
} TAF_PH_SetPrefplmn;


typedef struct {
    VOS_UINT8  para0[TAF_PH_UNLOCKCODELEN]; /* �������ý������Ĳ����ַ��� */
    VOS_UINT16 para0Len;                    /* ������ʶ�����ַ������� */
    VOS_UINT8  para1[TAF_PH_PINCODELENMAX]; /* �������ý������Ĳ����ַ��� */
    VOS_UINT16 para1Len;                    /* ������ʶ�����ַ������� */
} MN_PH_SetCpinReq;
enum TAF_PH_UsimFile {
    TAF_PH_PNN_FILE,
    TAF_PH_OPL_FILE,
    TAF_PH_MAX_FILE
};
typedef VOS_UINT16 TAF_PH_UsimFileUint16;
#define TAF_PH_INVALID_USIM_REC 0xFFFFFFFFU


typedef struct {
    VOS_UINT16 pnnCurrIndex; /* ��ѯPNN����ʼλ�� */
    /* ��ѯPNN�ĸ���������A�˺�C��ͨѶ����Ϣ��С�����ƣ����һ������ѯ10����¼ */
    VOS_UINT16 pnnNum;
} TAF_MMA_PnnInfoQueryReqIndex;

typedef struct {
    TAF_PH_IccType               icctype;
    VOS_UINT8                    reserved;
    TAF_PH_UsimFileUint16        efId;
    TAF_UINT32                   recNum;
    TAF_MMA_PnnInfoQueryReqIndex pnnQryIndex;
} TAF_PH_QryUsimInfo;

typedef struct {
    TAF_PH_IccType icctype;
    VOS_UINT8      reserved[3];
    VOS_BOOL       fileExist;
    TAF_UINT32     totalRecNum;
    TAF_UINT32     recordLen;
} TAF_PH_UsimInfo;

typedef struct {
    TAF_PH_UsimInfo usimInfo;
    TAF_PH_UsimInfo simInfo;
} TAF_PH_QryUsimRangeInfo;

typedef TAF_UINT8 TAF_PH_ICC_STATUS;
#define TAF_PH_ICC_INVALID 0
#define TAF_PH_ICC_VALID 1
#define TAF_PH_ICC_CSINVALID 2
#define TAF_PH_ICC_PSINVALID 3
#define TAF_PH_ICC_CSPSINVALID 4
#define TAF_PH_ICC_NOTEXIT 255

/* PHONE�¼����� */
typedef TAF_UINT8 TAF_PHONE_EVENT;
#define TAF_PH_EVT_ERR 0                 /* �¼�ͨ�ô��� */
#define TAF_PH_EVT_PLMN_LIST_ABORT_CNF 1 /* ��ȡ����PLMN�б���ֹȷ����Ϣ */
#define TAF_PH_EVT_PLMN_LIST_IND 2       /* PLMN�б������ϱ�ָʾ */
#define TAF_PH_EVT_OPER_MODE_CNF 3       /* ����ģʽ */
#define TAF_PH_EVT_SYS_CFG_CNF 4         /* ϵͳ���� */
#define TAF_PH_EVT_PLMN_LIST_CNF 5       /* APP�����ȡPLMN�б�ȷ���¼� */
#define TAF_PH_EVT_PLMN_LIST_REJ 6       /* �ܾ������ȡPLMN�б���� */
#define TAF_PH_EVT_PLMN_SEL_CNF 7        /* APPָ��PLMNѡ��ȷ����Ϣ */
#define TAF_PH_EVT_PLMN_RESEL_CNF 8      /* APP�����PLMN��ѡȷ����Ϣ */
#define TAF_PH_EVT_SERVICE_STATUS_IND 9  /* �����ϱ��²�ķ���״̬ */

#define TAF_PH_EVT_POWER_STA_IND 17         /* ��Դ״ָ̬ʾ */
#define TAF_PH_EVT_OP_PIN_CNF 18            /* ����PIN�뷵�ؽ�� */
#define TAF_PH_EVT_SETUP_SYSTEM_INFO_RSP 20 /* ����ϵͳ��Ϣ�ϱ���־ */
#define TAF_PH_EVT_USIM_RESPONSE 21         /* ��ȡ����USIM */
#define TAF_PH_EVT_USIM_INFO_IND 22         /* ��״̬��Ϣָʾ */
#define TAF_PH_EVT_OP_PINREMAIN_CNF 23
#define TAF_PH_EVT_ME_PERSONALISATION_CNF 24 /* �ƶ��豸˽�в������ؽ�� */
#define TAF_PH_EVT_RESTRICTED_ACCESS_CNF 25  /* �����Ƶ�SIM���������ݷ��� */
#define TAF_PH_EVT_OPER_MODE_IND 26          /* ����ģʽ�仯�����ϱ� */

#define MN_PH_EVT_SIMLOCKED_IND 29                  /* SIMLOCK�ϱ� */
#define MN_PH_EVT_ROAMING_IND 30                    /* ROAMING�ϱ� */
#define TAF_PH_EVT_GET_CURRENT_ATTACH_STATUS_CNF 31 /* ��ȡ��ǰע��״̬�ظ���Ϣ */

/* MMA��AT�ϱ��¼���������ʾ+PACSP1����+PACSP0 */
#define TAF_PH_EVT_NSM_STATUS_IND (37)

#define TAF_MMA_EVT_PS_INIT_RESULT_IND (45) /* Э��ջ��ʼ�����ָʾ��Ϣ */

#define TAF_PH_EVT_USIM_MATCH_FILES_IND (46)

#define TAF_PH_EVT_SIMSQ_IND (47)

/* AT^syscfgex��acqorder����Ľ��뼼������ */
#define TAF_MMA_MAX_GUC_RAT_NUM (4)
/* AT^syscfgex��acqorder����Ľ��뼼������,G U L NR 1X HRPD */
#define TAF_PH_MAX_SUPPORT_RAT_NUM (6)

/* AT^syscfgex��acqorder����Ľ��뼼������ */
#define TAF_PH_MAX_GUL_RAT_NUM (3)
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
#define TAF_PH_MAX_GULNR_RAT_NUM (4)
#define TAF_PH_MAX_GUNR_RAT_NUM (3)
#endif

/* AT^syscfgex��acqorder����Ľ��뼼������ */
#define TAF_PH_MAX_GU_RAT_NUM (2)
/* AT^syscfgex��acqorder����Ľ��뼼������ */
#define TAF_PH_MAX_LU_RAT_NUM (2)


typedef struct {
    /* syscfgex�����õ�acqoder�е�ָʾ���� */
    VOS_UINT8 ratOrderNum;
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    /* at^syscfgex�����õ�acqoder��˳�� */
    TAF_MMA_RatTypeUint8 ratOrder[TAF_PH_MAX_GULNR_RAT_NUM];
    VOS_UINT8            rsv[3];
#else
    TAF_MMA_RatTypeUint8 ratOrder[TAF_PH_MAX_GUL_RAT_NUM]; /* at^syscfgex�����õ�acqoder��˳�� */
#endif
} TAF_MMA_RatOrder;


typedef struct {
    VOS_UINT8 ratNum; /* ��ǰ֧�ֵĽ��뼼������  */

    TAF_MMA_RatTypeUint8 ratOrder[TAF_MMA_RAT_BUTT]; /*
                                                      * ����GUL��RAT��ʽ�����ݵ�ǰ��������ֵ��Ⱥ�˳�򣬾������ȼ����ȳ�������
                                                      * ����CLģʽ����û��RAT���ȼ�����
                                                      */
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    VOS_UINT8 reserved[1];
#else
    VOS_UINT8            reserved[2];
#endif
} TAF_MMA_MultimodeRatCfg;


enum TAF_MMA_UserSetPrioRat {
    TAF_MMA_USER_SET_PRIO_AUTO,
    TAF_MMA_USER_SET_PRIO_GSM_PREFER,
    TAF_MMA_USER_SET_PRIO_WCDMA_PREFER,
    TAF_MMA_USER_SET_PRIO_NOCHANGE,
    TAF_MMA_USER_SET_PRIO_BUTT
};
typedef VOS_UINT8 TAF_MMA_UserSetPrioRatUint8;


enum TAF_PH_UserSetRatType {
    TAF_PH_USER_SET_RAT_WCDMA,
    TAF_PH_USER_SET_RAT_WCDMA_GSM,
    TAF_PH_USER_SET_RAT_GSM,
    TAF_PH_USER_SET_RAT_BUTT
};
typedef VOS_UINT8 TAF_PH_UserSetRatTypeUint8;


typedef struct {
    /* ����������0���������ڹ������Σ�1�������������Σ��رչ������Σ�2���رչ������Σ������������Σ�3���رչ��ڹ������� */
    TAF_MMA_RoamModeUint8 roam;
    /* ������0��CS ONLY; 1:PS ONLY; 2:CS_PS; 3:any��ͬ��CS ONLY��4:�޷����� 5�����ı������ */
    TAF_MMA_ServiceDomainUint8  srvDomain;
    TAF_MMA_UserSetPrioRatUint8 userPrio;
    /* ָʾ����һ��at Cmd�·���Rat No Change */
    TAF_MMA_RatNoChangeCmdTypeUint8 ratCfgNoChangeType;
    TAF_MMA_MultimodeRatCfg         multiModeRatCfg; /* �û���ģRAT���� */
    TAF_UserSetPrefBand64           guBand;          /* ��¼GUƵ�� */
    TAF_USER_SetLtePrefBandInfo     lBand;           /* ��¼LƵ�� */
#if (FEATURE_MBB_CUST == FEATURE_ON)
    VOS_UINT8                       utranMode; /* 0:��TDSCDMA��1:TDSCDMA */
    VOS_UINT8                       nwScanFlag; /* ��¼NWSCAN��־λ */
    VOS_UINT8                       rsv[2];
#endif
} TAF_MMA_SysCfgPara;

#define TAF_MAX_GROUP_CFG_OPLMN_NUM (50)       /* �����������õ�OPLMN�������� */
#define TAF_MAX_USER_CFG_OPLMN_GROUP_INDEX (5) /* �������õ������Index,֧������(0-5)��6�� */
#define TAF_MAX_USER_CFG_OPLMN_VERSION_LEN (8) /* �û����õ�OPLMN�汾����󳤶� */
/* ������������OPLMN������ֽ���������SIM���ļ���ʽ��5���ֽڱ�ʾ1�������뼼����OPLMN */
#define TAF_MAX_GROUP_CFG_OPLMN_DATA_LEN (250)
/* �û��������õ��ܵ�OPLMN����ֽ���������SIM���ļ���ʽ��5���ֽڱ�ʾ1�������뼼����OPLMN */
#define TAF_MAX_USER_CFG_OPLMN_DATA_LEN (1280)
#define TAF_SIM_PLMN_WITH_RAT_LEN (5) /* SIM�������뼼��PLMN�ֽ��� */
#define TAF_AT_PLMN_WITH_RAT_LEN (10) /* AT�����·��Ĵ����뼼��PLMN�ֽ��� */
#define TAF_AT_EOPLMN_PARA_NUM (4)    /* ^EOPLMN������������ĸ��� */


typedef struct {
    TAF_UINT8 version[TAF_MAX_USER_CFG_OPLMN_VERSION_LEN]; /* OPLMN List�汾�� */
    TAF_UINT8 indexNum;                                    /* ��ǰ���õ���Index */
    TAF_UINT8 oPlmnCount;                                  /* ����ʵ������OPLMN�ĸ��� */
    /* OPLMN��PDU���ݣ���EFOplmn�ļ�һ�� */
    TAF_UINT8 oPlmnWithRat[TAF_MAX_GROUP_CFG_OPLMN_DATA_LEN];
} TAF_MMA_SetEoplmnList;


typedef struct {
    TAF_PHONE_EVENT   phoneEvent;
    MN_OPERATION_ID_T opId;
    MN_CLIENT_ID_T    clientId;

    VOS_UINT32 result;
} TAF_PHONE_EventEoplmnSetCnf;


typedef struct {
    TAF_PHONE_EVENT   phoneEvent;
    MN_OPERATION_ID_T opId;
    MN_CLIENT_ID_T    clientId;

    VOS_UINT32 result;
    VOS_UINT8  version[TAF_MAX_USER_CFG_OPLMN_VERSION_LEN]; /* OPLMN List�汾�� */
    TAF_UINT16 oPlmnNum;                                    /* OPLMN���� */
    TAF_UINT8  reserved[2];
    /* OPLMN��PDU���ݣ���EFOplmn�ļ�һ�� */
    TAF_UINT8 oPlmnList[TAF_MAX_USER_CFG_OPLMN_DATA_LEN];
} TAF_PHONE_EventEoplmnQryCnf;

/*
 * �ṹ˵��: AT��MMA����cops��ʾ���͵Ľӿ�
 */
typedef struct {
    VOS_UINT8 copsFormatType;
    VOS_UINT8 reserved[3];
} TAF_MMA_CopsFormatTypeSetReq;

#define TAF_MMA_MAX_EF_LEN (240)
/*
 * �ṹ˵��: usim�ظ����ļ����ݸ�mma�ӿ�
 */
typedef struct {
    VOS_UINT16 efId;
    VOS_UINT16 efLen;
    VOS_UINT8  totalNum;
    VOS_UINT8  reserved[3];
    VOS_UINT32 apptype;
    VOS_UINT8  ef[TAF_MMA_MAX_EF_LEN];
} TAF_MMA_UsimStubSetReq;

#define TAF_MMA_MAX_FILE_ID_NUM (6)
/*
 * �ṹ˵��: ģ��pih��ָ��ģ�鷢��refresh��Ϣ
 */
typedef struct {
    VOS_UINT32 receivePid;
    VOS_UINT16 refreshFileType;
    VOS_UINT8  totalNum;
    VOS_UINT8  reserved[1];
    VOS_UINT16 efId[TAF_MMA_MAX_FILE_ID_NUM];
} TAF_MMA_RefreshStubSetReq;


typedef struct {
    VOS_UINT8 activeFlg;
    VOS_UINT8 acuReserved[3];
} TAF_MMA_AutoReselStubSetReq;


enum TAF_MMA_NrScsInfo {
    TAF_MMA_NR_SCS_INFO_15K  = 0x00, /* 15kHz */
    TAF_MMA_NR_SCS_INFO_30K  = 0x01, /* 30kHz */
    TAF_MMA_NR_SCS_INFO_60K  = 0x02, /* 60kHz */
    TAF_MMA_NR_SCS_INFO_120K = 0x03, /* 120kHz */
    TAF_MMA_NR_SCS_INFO_240K = 0x04, /* 240kHz */

    TAF_MMA_NR_SCS_INFO_BUTT
};
typedef VOS_UINT8 TAF_MMA_NrScsInfoUint8;


typedef struct {
    VOS_UINT32 band[TAF_MMA_BAND_MAX_LEN]; /* BAND���ϣ�ÿ��Bit����һ��BAND */
} TAF_MMA_BandSet;


typedef struct {
    VOS_UINT32 arfcn; /* Ƶ�� */
    /* L��Ҫʹ��physical cell id��Ƶ������ȷ��һ��С��,Netscan����Ƶ/������С��ʹ�� */
    VOS_UINT32      phyId;
    VOS_UINT16      c1;     /* ����GSMС����ѡ��C1ֵ(�ݲ�֧��) */
    VOS_UINT16      c2;     /* ����GSMС����ѡ��C2ֵ(�ݲ�֧��) */
    VOS_UINT32      lac;    /* λ������ */
    VOS_UINT32      mcc;    /* PLMN������ */
    VOS_UINT32      mnc;    /* PLMN������ */
    VOS_UINT16      bsic;   /* GSMС����վ�� */
    VOS_INT16       rxlev;  /* ���յ����ź�ǿ�� */
    VOS_INT16       rssi;   /* GSM��BCCH�ŵ���RSSI��WCDMA��RSCP */
    VOS_UINT16      psc;    /* WCDMA:��ֵ�������� */
    TAF_CELL_Id     cellId; /* С��ID */
    TAF_MMA_BandSet band;   /* Ƶ�� */

    VOS_UINT8              nrFlg; /* ��С���Ƿ�ʱNRС�� */
    TAF_MMA_NrScsInfoUint8 nrScsInfo;
    VOS_UINT8              reserved[2];
    VOS_INT16              nrRsrp; /* ��Χ��(-157,-30), 99Ϊ��Ч */
    VOS_INT16              nrRsrq; /* ��Χ��(-87,  40), 99Ϊ��Ч */
    VOS_INT32              nrSinr; /* ��Χ��(-47,  80), 99Ϊ��Ч */
} TAF_MMA_NetScanInfo;

/*
 * �ṹ˵��: TAF_MMA_NetScanCnf��Ϣ�е�enResult;
 */
enum TAF_MMA_NetScanResult {
    TAF_MMA_NET_SCAN_RESULT_SUCCESS = 0,
    TAF_MMA_NET_SCAN_RESULT_FAILURE = 1,
    TAF_MMA_NET_SCAN_RESULT_BUTT
};
typedef VOS_UINT8 TAF_MMA_NetScanResultUint8;


enum TAF_MMA_NetScanCause {
    TAF_MMA_NET_SCAN_CAUSE_NULL              = 0,
    TAF_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST      = 1, /* ������������ */
    TAF_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED = 2, /* ��ǰδפ�� */
    TAF_MMA_NET_SCAN_CAUSE_FREQ_LOCK         = 3, /* ��ǰ��Ƶ״̬ */
    /* �������󣬰������뼼����֧�֣�Ƶ�β�֧�֣�Ƶ������������޴��� */
    TAF_MMA_NET_SCAN_CAUSE_PARA_ERROR        = 4,
    TAF_MMA_NET_SCAN_CAUSE_CONFLICT          = 5, /* ����ԭ��ֵ��ͻ */
    TAF_MMA_NET_SCAN_CAUSE_SERVICE_EXIST     = 6,
    TAF_MMA_NET_SCAN_CAUSE_NOT_CAMPED        = 7,
    TAF_MMA_NET_SCAN_CAUSE_TIMER_EXPIRED     = 8,
    TAF_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR    = 9,
    TAF_MMA_NET_SCAN_CAUSE_MMA_STATE_DISABLE = 10,

    TAF_MMA_NET_SCAN_CAUSE_BUTT
};
typedef VOS_UINT8 TAF_MMA_NetScanCauseUint8;


enum TAF_MMA_SetBorderInfoOperateType {
    TAF_MMA_SET_BORDER_INFO_OPERATE_TYPE_ADD     = 0, /* ���ӱ߾���Ϣ */
    TAF_MMA_SET_BORDER_INFO_OPERATE_TYPE_DEL_ALL = 1, /* ɾ�����еı߾���Ϣ */
    TAF_MMA_SET_BORDER_INFO_OPERATE_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SetBorderInfoOperateTypeUint8;


enum NAS_MML_LacTacType {
    TAF_MMA_LAC_TAC_TYPE_LAC = 0, /* ����ΪLAC */
    TAF_MMA_LAC_TAC_TYPE_TAC = 1, /* ����ΪTAC */
    TAF_MMA_LAC_TAC_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_LacTacTypeUint8;


typedef struct {
    /* �ò��ֱ��������ǰ�棬ATģ�����ȡ�ò������ж���Ϣ���� */

    TAF_PLMN_Id plmnId;
    TAF_UINT32  rejCause;

    TAF_MMA_RejRatTypeUint8 rat;

    TAF_MMA_ServiceDomainUint8 srvDomain;

    VOS_UINT8 rejType;
    VOS_UINT8   rac;
    VOS_UINT32  lac;
    VOS_UINT8   originalRejCause;
    VOS_UINT8   pdnRejCause;
    VOS_UINT8   rsv[2];
    TAF_CELL_Id cellId;

} TAF_PH_RegRejInfo;


enum TAF_PH_PlmnSelectionResult {
    TAF_PH_PLMN_SELECTION_RESULT_NORMAL_SERVICE  = 0, /* �������� */
    TAF_PH_PLMN_SELECTION_RESULT_LIMITED_SERVICE = 1, /* ���Ʒ��� */
    TAF_PH_PLMN_SELECTION_RESULT_NO_SERVICE      = 2, /* �޷��� */
    TAF_PH_PLMN_SELECTION_RESULT_INTERRUPT       = 3, /* �������ж� */
    TAF_PH_PLMN_SELECTION_RESULT_BUTT
};
typedef VOS_UINT32 TAF_PH_PlmnSelectionResultUint32;


enum TAF_PH_PlmnSelectionStartEndFlag {
    TAF_PH_PLMN_SELECTION_START     = 0, /* ������ʼ */
    TAF_PH_PLMN_SELECTION_END       = 1, /* �������� */
    TAF_PH_PLMN_SELECTION_INTERRUPT = 2, /* ��������� */
    TAF_PH_PLMN_SELECTION_BUTT
};
typedef VOS_UINT32 TAF_PH_PlmnSelectionStartEndFlagUint32;


typedef struct {
    TAF_PH_PlmnSelectionResultUint32       plmnSelectRlst;
    TAF_PH_PlmnSelectionStartEndFlagUint32 plmnSelectFlag; /* ��ʾ������ʼ���������� */
} TAF_MMA_PlmnSelectionInfo;


typedef struct {
    TAF_UINT8            opError;
    TAF_UINT8            reserved[3];
    TAF_ERROR_CodeUint32 phoneError;
    TAF_UINT32           currIndex;
    TAF_UINT32           plmnNum;
    TAF_PH_OperatorName  plmnName[TAF_MMA_MAX_PLMN_NAME_LIST_NUM];
    TAF_PLMN_Status      plmnInfo[TAF_MMA_MAX_PLMN_NAME_LIST_NUM];
} TAF_MMA_PlmnListCnfPara;


typedef struct {
    VOS_UINT8 mcgCipherAlg;
    VOS_UINT8 scgCipherAlg;
    VOS_UINT8 reserved[2];
} TAF_MMA_CIPHER_Algoinfo;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32              msgName;
    MN_OPERATION_ID_T       opId;
    TAF_UINT8               reserved[1];
    MN_CLIENT_ID_T          clientId;
    TAF_MMA_PlmnListCnfPara plmnListCnfPara;
} TAF_MMA_PlmnListCnf;


typedef struct {
    TAF_PLMN_Id          plmnId;
    TAF_UINT8            opError;
    TAF_MMA_RatTypeUint8 ratMode; /* ��ǰ̽��ֻ֧��L,������ʽֱ�ӷ���ERROR */
    VOS_INT16            rsrp;
} TAF_MMA_PlmnDetectIndPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                msgName;
    MN_OPERATION_ID_T         opId;
    TAF_UINT8                 reserved[1];
    MN_CLIENT_ID_T            clientId;
    TAF_MMA_PlmnDetectIndPara plmnDetectIndPara;
} TAF_MMA_PlmnDetectInd;

enum TAF_USIM_MatchFile {
    TAF_USIM_MATCH_FILE_GID1 = 0,
    TAF_USIM_MATCH_FILE_GID2 = 1,
    TAF_USIM_MATCH_FILE_SST  = 2, /* for USIM is EF-UST, for SIM is EF-SST */
    TAF_USIM_MATCH_FILE_SPN  = 3,
    TAF_USIM_MATCH_FILE_MAX
};

typedef VOS_UINT8 TAF_USIM_MatchFileUint8;

typedef struct {
    VOS_UINT16 totalLen;                      /* File length */
    VOS_UINT16 curLen;                        /* File length */
    VOS_UINT8  fileType;                      /* TAF_USIM_MatchFileInfo  */
    VOS_UINT8  content[TAF_MAX_USIM_RSP_LEN]; /* File content */
} TAF_USIM_MatchFileInfo;

typedef struct {
    /* �ò��ֱ��������ǰ�棬ATģ�����ȡ�ò������ж���Ϣ���� */
    TAF_PHONE_EVENT   phoneEvent;
    MN_OPERATION_ID_T opId;
    MN_CLIENT_ID_T    clientId;

    TAF_UINT32 opRegPlmn : 1;
    TAF_UINT32 opCsServiceStatus : 1;
    TAF_UINT32 opPsServiceStatus : 1;

    TAF_UINT32 opPlmnReselMode : 1;
    TAF_UINT32 opPhoneError : 1;
    TAF_UINT32 opCurRegSta : 1;
    TAF_UINT32 opRssi : 1;

    TAF_UINT32 opBatteryPower : 1;
    TAF_UINT32 opPin : 1;
    TAF_UINT32 opUsimAccessData : 1;
    TAF_UINT32 opPinRemain : 1;

    TAF_UINT32 opMeLockStatus : 1;
    TAF_UINT32 opMePersonalisation : 1;
    TAF_UINT32 opOperMode : 1;
    TAF_UINT32 opSpnInfo : 1;

    TAF_UINT32 opMode : 1;
    TAF_UINT32 opSrvst : 1;
    TAF_UINT32 opUsimRestrictAccess : 1;
    TAF_UINT32 opUsimPNN : 1;

    TAF_UINT32 opSysCfg : 1;
    TAF_UINT32 opUsimOPL : 1;
    TAF_UINT32 opMmInfo : 1;
    TAF_UINT32 opPrefPlmnList : 1;

    TAF_UINT32 opDomainAttachState : 1;
    TAF_UINT32 opSysCfgBandGroup : 1;

    TAF_UINT32 opPlmnMode : 1;

    TAF_UINT32 opCurPlmn : 1;

    TAF_UINT32 opCellAcInfo : 1;
    TAF_UINT32 opSpare : 3;
    TAF_UINT32 opPsInitRslt : 1;

    TAF_ERROR_CodeUint32 phoneError;

    TAF_PH_OpModeCnf operMode;

    TAF_MMA_SysCfgPara sysCfg;

    TAF_PLMN_Id                      regPlmn;
    TAF_PH_INFO_RAT_TYPE             ratType;
    TAF_PHONE_SERVICE_STATUS         csServiceStatus;
    TAF_PHONE_SERVICE_STATUS         psServiceStatus;
    TAF_PHONE_CHANGED_SERVICE_STATUS serviceStatus;
    TAF_PLMN_RESEL_MODE_TYPE         plmnReselMode;
    VOS_UINT8                        reserved1[3];
    TAF_PH_RegState                  curRegSta;
    TAF_PH_PinCnf                    pinCnf;
    TAF_PH_UsimResponse              usimAccessData;
    TAF_USIM_STATUS                  simStatus;
    TAF_PH_ME_LOCK_STATUS            meLockStatus;
    VOS_UINT8                        reserved2[2];
    TAF_PH_PinTimecnf                pinRemainCnf;
    TAF_PH_MePersonlisationCnf       mePersonalisation;
    TAF_PH_SpnInfo                   phSpnInfo;
    TAF_PH_RestrictedAccessCnf       restrictedAccess;

    VOS_UINT8 reserved3[2];

    VOS_UINT8 roamStatus; /* �ϱ���Ӧ�õ�����״̬ */
    VOS_UINT8 plmnMode;

    VOS_UINT8 curcRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE];

    TAF_MMA_PsInitRsltUint32 psInitRslt; /* Э��ջ��ʼ����� */
    TAF_USIM_MatchFileInfo usimMatchFile;
} TAF_PHONE_EventInfo;

#define TAF_SIM_LOCK_MCC_LEN (3)

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: NV_ITEM_CARDLOCK_STATUS NV���Ӧ�Ľṹ,
 *           ���ڱ���SIM LOCK��״̬�Լ�ʣ���������
 */
typedef struct {
    VOS_UINT32                    status;            /* �Ƿ񼤻0�����1���� */
    TAF_OPERATOR_LockStatusUint32 cardlockStatus;
    VOS_UINT32                    remainUnlockTimes; /* ����ʣ����� */
} TAF_CUSTOM_CardlockStatus;

#define EVT_NAS_MMA_NV_CONTEXT 0xaacc /* PC�طŹ���NV�ļ�������Ϣ */

/*
 * Structure: NAS_MMA_NvContext
 * Description: PC�طŹ��̣�NV�ļ��ṹ
 * Message origin:
 */
typedef struct {
    VOS_UINT8  auNvFileName[40]; /* �ļ������40���ַ� */
    VOS_UINT32 nvFileLen;        /* �ļ����� */
    VOS_UINT32 packetLen;        /* ��ǰ���ݱ��ڶ�ȡ���ļ������ݳ��� */
    VOS_UINT8  auNvFileData[4];  /* �ļ����� */
} NAS_MMA_NvContext;

/*
 * Structure: NAS_MMA_NvMsg
 * Description: PC�طŹ��̣�NV�ļ��ط���Ϣ
 * Message origin:
 */
typedef struct {
    VOS_MSG_HEADER            /* _H2ASN_Skip */
    VOS_UINT16        msgID;  /* _H2ASN_Skip */
    /* ��PACK(1)��PACK(4)�����ж���ı����ֽ� */
    VOS_UINT16        reserved;
    NAS_MMA_NvContext nVCtxt; /* NV�ļ� */
} NAS_MMA_NvMsg;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: CSQLVLEX��ѯ�����Ľ���ϱ��ṹ
 *           ����CSQLVLEX��ѯ��ȡ�����źŸ����ͱ��������ʰٷֱȣ���ǰ�汾��֧��BER��ѯ����99
 */
typedef struct {
    MN_PH_CsqlvlextRssilvValueUint8 rssilv; /* CSQLVLEX��ѯ��ȡ�����źŸ��� */
    /* ���������ʰٷֱȣ���ʱ��֧��BER��ѯ����99 */
    MN_PH_CsqlvlextBerValueUint8 ber;
} TAF_MMA_CsqlvlextPara;

/*
 * �ṹ˵��: CSQLVL��ѯ�����Ľ���ϱ��ṹ
 *           ����RSCP �ȼ���RSCP �ľ���ֵ
 */
typedef struct {
    VOS_UINT16 rscp;  /* RSCP �ȼ� */
    VOS_UINT16 level; /* RSCP �ľ���ֵ */
} TAF_MMA_CsqlvlPara;


enum AT_CPAM_RatPrio {
    MN_MMA_CPAM_RAT_PRIO_GSM       = 0, /* �������ȼ�����ΪGSM����  */
    MN_MMA_CPAM_RAT_PRIO_WCDMA     = 1, /* �������ȼ�����ΪWCDMA���� */
    MN_MMA_CPAM_RAT_PRIO_NO_CHANGE = 2, /* �������ȼ�Ϊ���ı� */
    MN_MMA_CPAM_RAT_BUTT
};
typedef VOS_UINT8 MN_MMA_CpamRatPrioUint8;


typedef struct {
    VOS_INT16  cpichRscp;
    VOS_INT16  cpichEcNo;
    VOS_UINT32 cellId;
    VOS_UINT8  rssi; /* Rssi, GU��ʹ�� */
    VOS_UINT8  reserve[3];

} MN_MMA_Anquery2G3GPara;


typedef struct {
    VOS_INT16 rsrp; /* ��Χ��(-141,-44), 99Ϊ��Ч */
    VOS_INT16 rsrq; /* ��Χ��(-40, -6) , 99Ϊ��Ч */
    VOS_INT16 rssi; /* Rssi, LTE��ʹ�� */
    VOS_UINT8 reserve[2];
} MN_MMA_Anquery4GPara;


typedef struct {
    TAF_MMA_RatTypeUint8 serviceSysMode; /* ָʾ�ϱ�ģʽ */
    VOS_UINT8            reserve[3];
    union {
        MN_MMA_Anquery2G3GPara st2G3GCellSignInfo; /* Rssi, GU��ʹ�� */
        MN_MMA_Anquery4GPara   st4GCellSignInfo;   /* Rssi, LTE��ʹ�� */
    } u;
} MN_MMA_AnqueryPara;

/*
 * �ṹ˵��: AT+CSNR��ѯ�����Ľ���ϱ��ṹ
 */
typedef struct {
    VOS_INT16 cpichRscp;
    VOS_INT16 cpichEcNo;
} TAF_MMA_CsnrPara;


typedef struct {
    VOS_UINT8  currIndex;
    VOS_UINT8  qryNum;
    VOS_UINT16 plmnNameLen;
    VOS_UINT8  plmnType;
    VOS_UINT8  reserved[3];
    VOS_UINT8  plmnName[TAF_PH_OPER_NAME_LONG];
} TAF_MMA_CrpnQryPara;

/* NAME���������ˣ�ÿ���ϱ��ĸ���ҲҪ���� */
#define TAF_PH_CRPN_PLMN_MAX_NUM (6) /* AT^CRPN�������PLMN���� */

typedef struct {
    VOS_UINT8           currIndex;
    VOS_UINT8           totalNum;
    VOS_UINT8           maxMncLen;
    VOS_UINT8           reserved[1];
    TAF_MMA_CrpnQryPara mnMmaCrpnQry;
    TAF_PH_OperatorName operNameList[TAF_PH_CRPN_PLMN_MAX_NUM];
} TAF_MMA_CrpnQryInfo;


typedef struct {
    TAF_PH_UsimSpn simSpnInfo;
    TAF_PH_UsimSpn usimSpnInfo;
} TAF_MMA_SpnPara;

/* MM��������壬�ڴ˻�������չ */
enum MM_TEST_AtCmd {
    MM_TEST_AT_CMD_MM_STATE         = 0, /* MM�Ӳ�״̬ */
    MM_TEST_AT_CMD_GMM_STATE        = 1, /* GMM�Ӳ�״̬ */
    MM_TEST_AT_CMD_GPRS_STATE       = 2, /* GPRS�ƶ��Թ���״̬ */
    MM_TEST_AT_CMD_MM_REG_STATE     = 3, /* MMע��״̬ */
    MM_TEST_AT_CMD_GMM_REG_STATE    = 4, /* GMMע��״̬ */
    MM_TEST_AT_CMD_PLMN_SEARCH_MODE = 5, /* �趨����ģʽ, ֻ����NV�� */
    MM_TEST_AT_CMD_SET_RAT_MODE     = 6, /* �趨����ģʽ, ֻ����NV�� */

    MM_TEST_AT_CMD_SET_ADDITIONAL_UPDATE_RESULT_IE = 7, /* �趨ADDITIONAL_UPDATE_RESULT_IE */
    MM_TEST_AT_CMD_SET_UE_USAGE_SETTING            = 8, /* �趨UE_USAGE_SETTING */

    MM_TEST_AT_CMD_SET_GMM_DEALY_SUSPENDRSP = 9, /* �趨gmm�ӳٻظ���������MMC */

    MM_TEST_AT_CMD_SET_CSFB_HIGH_PRIO_FLG = 10,

    MM_TEST_AT_CMD_CLEAR_USER_PLMN_ = 11,

    MM_TEST_AT_CMD_SET_ORIGINAL_REJECT_CAUSE = 12,
    MM_TEST_AT_CMD_BUTT,
};
typedef VOS_UINT8 MM_TEST_AtCmdUint8;


enum TAF_MMA_CardStatus {
    TAF_MMA_CARD_STATUS_AVAILABLE     = 0,
    TAF_MMA_CARD_STATUS_NOT_AVAILABLE = 1,
    TAF_MMA_CARD_STATUS_BUTT
};
typedef VOS_UINT8 TAF_MMA_CardStatusUint8;


typedef struct {
    MM_TEST_AtCmdUint8 cmd;
    TAF_UINT8          paraNum;
    TAF_UINT16         reserve;
    TAF_UINT32         para[10];
} MM_TEST_AtCmdPara;


typedef struct {
    VOS_UINT32 rsltNum;
    VOS_UINT32 rslt[20];
} MM_TEST_AtRslt;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */

    /* UE��PC���ߵ����ӱ�־, VOS_TRUE: ������; VOS_FALSE: δ���� */
    VOS_UINT8      omConnectFlg;
    /* OM���õķ���NAS PC�ط���Ϣ��ʹ�ܱ�־ */
    VOS_UINT8      omPcRecurEnableFlg;

    VOS_UINT8      rsv2[2]; /* ����   */
} ATMMA_OM_MaintainInfoInd;


typedef struct {
    VOS_UINT8 longNameLen;
    VOS_UINT8 longName[TAF_PH_OPER_NAME_LONG];
    VOS_UINT8 shortNameLen;
    VOS_UINT8 shortName[TAF_PH_OPER_NAME_SHORT];
    VOS_UINT8 reserved[2]; /* ����   */
} TAF_MMA_MmInfoPlmnName;

typedef struct {
    VOS_UINT32 csSrvExistFlg; /* CSҵ���Ƿ���ڱ�־ */
    VOS_UINT32 psSrvExistFlg; /* PSҵ���Ƿ���ڱ�־ */
} TAF_USER_SrvState;


typedef struct {
    VOS_UINT8 rsv[4];
} TAF_MMA_CerssiInfoQueryReq;


typedef struct {
    MN_PH_PrefPlmnTypeUint8 prefPLMNType; /* ��ѯ��Ӫ�����Ƶ����� */
    VOS_UINT8               reserved1[3];
    VOS_UINT32              fromIndex; /* ��ѯ��Ӫ�����Ƶ���ʼλ�� */
    /* ��ѯ��Ӫ�����Ƶĸ���������A�˺�C��ͨѶ����Ϣ��С�����ƣ����һ������ѯ50����¼ */
    VOS_UINT32 plmnNum;
    VOS_UINT32 validPlmnNum; /* ��Ч�������Ӫ�����Ƹ��� */
} TAF_MMA_CpolInfoQueryReq;


typedef struct {
    VOS_UINT32 moduleId; /* ����PID */
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserved[1];
} TAF_MMA_Ctrl;


typedef struct {
    VOS_UINT16 currIndex; /* ������Ҫ��ȡ��PNN list��ʼλindex */
    VOS_UINT16 readNum;   /* ���ζ�ȡ��PNN ��Ŀ */
} TAF_MMA_PlmnListCtrl;


typedef struct {
    TAF_PH_MODE      phMode;  /* ģʽ */
    TAF_PH_RESET_FLG phReset; /* �Ƿ���Ҫ���� */
    VOS_UINT8        reserve[2];
} TAF_MMA_PhoneModePara;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_PhoneModePara phoneModePara;
} TAF_MMA_PhoneModeSetReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_SysCfgPara    sysCfgPara;
} TAF_MMA_SysCfgReq;

/*
 * �ṹ˵��: AT^EONSUCS2 ��ѯ������Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EonsUcs2Req;

/*
 * �ṹ˵��: AT^EONSUCS2 ��ѯ���ݽṹ��
 */
typedef struct {
    VOS_UINT8 longNameLen;
    VOS_UINT8 longName[TAF_PH_OPER_NAME_LONG];
    VOS_UINT8 shortNameLen;
    VOS_UINT8 shortName[TAF_PH_OPER_NAME_SHORT];
    VOS_UINT8 reserved[2]; /* ����   */
} TAF_MMA_EonsUcs2PlmnName;

/*
 * �ṹ˵��: AT^EONSUCS2 ��ѯ���ݽṹ��
 */
typedef struct {
    VOS_UINT8 homeNodeBNameLen;
    VOS_UINT8 reserved[3];
    VOS_UINT8 homeNodeBName[TAF_MMA_MAX_HOME_NODEB_NAME_LEN];
} TAF_MMA_EonsUcs2HnbName;

/*
 * �ṹ˵��: AT^EONSUCS2 ��ѯ������ݽṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                  msgName;
    VOS_UINT16                  clientId;
    VOS_UINT8                   opId;
    VOS_UINT8                   reserve[1];
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
    TAF_MMA_EonsUcs2PlmnName    eonsUcs2PlmnName;
    TAF_MMA_EonsUcs2HnbName     eonsUcs2HNBName;
} TAF_MMA_EonsUcs2Cnf;


typedef struct {
    VOS_UINT32  arfcn;
    VOS_UINT8   plmnNum;
    VOS_UINT8   cellNum;
    VOS_UINT8   reserved[2];
    TAF_PLMN_Id plmnId[TAF_MMA_REG_MAX_PLMN_NUM];
    VOS_UINT16  cellId[TAF_MMA_REG_MAX_CELL_NUM];
} TAF_MMA_RegCellInfo;


enum TAF_MMA_SrvAcqResult {
    TAF_MMA_SRV_ACQ_RESULT_SUCCESS = 0, /* �ɹ� */
    TAF_MMA_SRV_ACQ_RESULT_FAIL    = 1, /* ʧ�� */

    TAF_MMA_SRV_ACQ_RESULT_NO_RF = 2, /* NO RF */

    TAF_MMA_SRV_ACQ_RESULT_POWER_DOWN = 3, /* �ػ� */

    TAF_MMA_SRV_ACQ_RESULT_BUTT
};
typedef VOS_UINT8 TAF_MMA_SrvAcqResultUint8;


enum TAF_MMA_SrvAcqFailCause {
    TAF_MMA_SRV_ACQ_FAIL_CAUSE_UNKNOW = 0, /* δ֪ */
    TAF_MMA_SRV_ACQ_FAIL_CAUSE_NO_RF  = 1, /* NO RF */
    TAF_MMA_SRV_ACQ_FAIL_CAUSE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SrvAcqFailCauseUint8;


enum TAF_MMA_SrvType {
    TAF_MMA_SRV_TYPE_CS_MO_NORMAL_CALL,
    TAF_MMA_SRV_TYPE_CS_MO_SS,
    TAF_MMA_SRV_TYPE_CS_MO_SMS,
    TAF_MMA_SRV_TYPE_PS_CONVERSAT_CALL,
    TAF_MMA_SRV_TYPE_PS_STREAM_CALL,
    TAF_MMA_SRV_TYPE_PS_INTERACT_CALL,
    TAF_MMA_SRV_TYPE_PS_BACKGROUND_CALL,
    TAF_MMA_SRV_TYPE_PS_SUBSCRIB_TRAFFIC_CALL,
    TAF_MMA_SRV_TYPE_PS_MO_EMERGENCY_CALL,
    TAF_MMA_SRV_TYPE_CS_MO_SMS_T3346_RUNNING,
    TAF_MMA_SRV_TYPE_PS_MO_EMERGENCY_CALL_RETRY,

    TAF_MMA_SRV_TYPE_PS_MO_EMERGENCY_CALL_VOLTE_380,
    TAF_MMA_SRV_TYPE_WIFI2LTE_HANDOVER,
    TAF_MMA_SRV_TYPE_MO_EMERGENCY_CALL,
    TAF_MMA_SRV_TYPE_PS_MO_NORMAL_CALL_NR_TO_LTE,
    TAF_MMA_SRV_TYPE_PS_MO_NORMAL_CALL_NR_TO_GU,

    TAF_MMA_SRV_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SrvTypeUint8;


enum TAF_MMA_PifEmcSrchRslt {
    TAF_MMA_EMC_SRCH_RSLT_SUCC,
    TAF_MMA_EMC_SRCH_RSLT_FAIL,
    TAF_MMA_EMC_SRCH_RSLT_BUTT
};
typedef VOS_UINT8 TAF_MMA_EmcSrchRsltUint8;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32        msgName; /* _H2ASN_Skip */
    TAF_MMA_EmcSrchRsltUint8     result;
    TAF_MMA_RatTypeUint8         rat;
    VOS_UINT8                    reserve[2];
} TAF_MMA_EmcSrchRsltInd;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32        msgName; /* _H2ASN_Skip */
    TAF_MMA_SrvAcqResultUint8    result;
    TAF_MMA_SrvAcqFailCauseUint8 cause;   /* ʧ��ԭ��ֵ */
    TAF_MMA_SrvTypeUint8         srvType;
    TAF_MMA_RatTypeUint8         campRat;
} TAF_MMA_SrvAcqCnf;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
} TAF_MMA_StopSrvAcqCnf;


enum TAF_MMA_ApplicationsIndication {
    TAF_MMA_APPLICATIONS_INDICATION_START = 0,
    TAF_MMA_APPLICATIONS_INDICATION_STOP  = 1,

    TAF_MMA_APPLICATIONS_INDICATION_BUTT
};
typedef VOS_UINT32 TAF_MMA_ApplicationsIndicationUint32;


enum TAF_MMA_SrvAcqRatType {
    TAF_MMA_SRV_ACQ_RAT_TYPE_GUL,
    TAF_MMA_SRV_ACQ_RAT_TYPE_1X,
    TAF_MMA_SRV_ACQ_RAT_TYPE_HRPD,
    TAF_MMA_SRV_ACQ_RAT_TYPE_BUTT
};

typedef VOS_UINT8 TAF_MMA_SrvAcqRatTypeUint8;

#if (FEATURE_ON == FEATURE_UE_MODE_NR)

enum TAF_MMA_NrBandSetOption {
    TAF_MMA_NR_BAND_SET_OPTION_TURN_FR2_ON              = 0,
    TAF_MMA_NR_BAND_SET_OPTION_TURN_FR2_OFF             = 1,
    TAF_MMA_NR_BAND_SET_OPTION_DEL_BAND_FROM_BLOCK_LIST = 2,
    TAF_MMA_NR_BAND_SET_OPTION_ADD_BAND_TO_BLOCK_LIST   = 3,
    TAF_MMA_NR_BAND_SET_OPTION_BUTT
};
typedef VOS_UINT32 TAF_MMA_NrBandSetOptionUint32;


enum TAF_MMA_NrFr2SupportSwStatus {
    TAF_MMA_NR_FR2_SUPPORT_SW_STATUS_ON  = 0,
    TAF_MMA_NR_FR2_SUPPORT_SW_STATUS_OFF = 1,
    TAF_MMA_NR_FR2_SUPPORT_SW_STATUS_BUTT
};
typedef VOS_UINT32 TAF_MMA_NrFr2SupportSwStatusUint32;
#endif

enum TAF_MMA_StrategyMode {
    TAF_MMA_STRATEGY_MODE_CLEAR = 0,
    TAF_MMA_STRATEGY_MODE_SET   = 1,
    TAF_MMA_STRATEGY_MODE_BUTT
};
typedef VOS_UINT8 TAF_MMA_StrategyModeUint8;

enum TAF_MMA_StrategyScene {
    TAF_MMA_STRATEGY_SCENE_RELATIVE_STILL   = 0,
    TAF_MMA_STRATEGY_SCENE_DATA_MODEM       = 1,
    TAF_MMA_STRATEGY_SCENE_NO_DATA_MODEM    = 2,
    TAF_MMA_STRATEGY_SCENE_BUTT             = TAF_MMA_SET_OOS_SCENE_MAX_NUM
};
typedef VOS_UINT8 TAF_MMA_StrategySceneUint8;


typedef struct {
    VOS_UINT8                  ratNum;
    TAF_MMA_SrvAcqRatTypeUint8 ratType[TAF_MMA_SRV_ACQ_RAT_NUM_MAX];
} TAF_MMA_SrvAcqRatList;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_SrvTypeUint8  srvType;
    VOS_UINT8             reserved[3];
    TAF_MMA_SrvAcqRatList ratList;
} TAF_MMA_SrvAcqReqMsg;


typedef struct {
    TAF_MMA_DetachCauseUint8 detachCause;
    /* ������0��CS ONLY; 1:PS ONLY; 2:CS_PS; 3:any��ͬ��CS ONLY��4�����ı������ */
    TAF_MMA_ServiceDomainUint8 detachDomain;
    VOS_UINT8                  reserve[2];
} TAF_MMA_DetachPara;


typedef struct {
    VOS_UINT16 currIndex; /* ��ѯ����ʼλ�� */
    VOS_UINT16 qryNum;    /* ��ѯ�ĸ���������A�˺�C��ͨѶ����Ϣ��С������ */
} TAF_MMA_PlmnListPara;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            reserved;
} TAF_MMA_CsgListAbortReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CsgListAbortCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_PlmnListPara  plmnListPara;
} TAF_MMA_CsgPlmnListSearchReqMsg;


typedef struct {
    VOS_UINT8 csgTypeLen;
    VOS_UINT8 reserved[3];
    VOS_UINT8 csgType[TAF_MMA_MAX_CSG_TYPE_LEN];
} TAF_MMA_CsgType;


typedef struct {
    VOS_UINT8 homeNodeBNameLen;
    VOS_UINT8 reserved[3];
    VOS_UINT8 homeNodeBName[TAF_MMA_MAX_HOME_NODEB_NAME_LEN];
} TAF_MMA_CsgIdHomeNodebName;


typedef struct {
    TAF_PLMN_Id                    plmnId;
    VOS_UINT32                     csgId;
    TAF_MMA_CsgType                csgType;
    TAF_MMA_CsgIdHomeNodebName     csgIdHomeNodeBName;
    TAF_PH_RA_MODE                 raMode; /* ���߽���ģʽ,4G/3G/2G */
    TAF_MMA_PlmnWithCsgIdTypeUint8 plmnWithCsgIdType;
    VOS_UINT8                      reserved[2];
    TAF_CHAR                       operatorNameShort[TAF_PH_OPER_NAME_SHORT];
    TAF_CHAR                       operatorNameLong[TAF_PH_OPER_NAME_LONG];
    VOS_INT16 signalValue1; /* LTE:<RSRP> WCDMA:<RSCP> GSM:<RSSI> */
    VOS_INT16 signalValue2; /* LTE:<RSRQ> WCDMA:<EC/IO> GSM:0 */
} TAF_MMA_CsgIdListInfo;


typedef struct {
    TAF_ERROR_CodeUint32  phoneError;
    VOS_UINT32            currIndex;
    VOS_UINT8             opError;
    VOS_UINT8             plmnWithCsgIdNum;
    VOS_UINT8             reserved[2];
    TAF_MMA_CsgIdListInfo csgIdListInfo[TAF_MMA_MAX_CSG_ID_LIST_NUM];
} TAF_MMA_CsgListCnfPara;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32  msgName; /* _H2ASN_Skip */
    VOS_UINT16             clientId;
    VOS_UINT8              opId;
    VOS_UINT8              reserved;
    TAF_MMA_CsgListCnfPara csgListCnfPara;
} TAF_MMA_CsgListSearchCnf;


typedef struct {
    TAF_PLMN_Id          plmnId;
    VOS_UINT32           csgId;
    TAF_MMA_RatTypeUint8 ratType;
    VOS_UINT8            reserved[3];
} TAF_MMA_CsgSpecSearchInfo;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl              ctrl;
    TAF_MMA_CsgSpecSearchInfo csgSpecSearchInfo;
} TAF_MMA_CsgSpecSearchReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            reserved;
} TAF_MMA_QryCampCsgIdInfoReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_PLMN_Id           plmnId;
    TAF_PH_RA_MODE        ratType;
    VOS_UINT8             reserved[3];
    VOS_UINT32            csgId;
} TAF_MMA_QryCampCsgIdInfoCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CsgSpecSearchCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_DetachPara    detachPara;
} TAF_MMA_DetachReqMsg;


typedef struct {
    TAF_MMA_ImsSwitchSetUint8 lteEnable;
    TAF_MMA_ImsSwitchSetUint8 utranEnable;
    TAF_MMA_ImsSwitchSetUint8 gsmEnable;
    VOS_UINT8                 reserved;
} TAF_MMA_ImsSwitchRat;


typedef struct {
    VOS_UINT32 bitOpImsSwitch : 1;
    VOS_UINT32 bitOpRcsSwitch : 1;
    VOS_UINT32 bitOpSpare : 30;

    TAF_MMA_ImsSwitchRat imsSwitchRat;
    VOS_UINT32           rcsSwitch;
} TAF_MMA_ImsSwitchInfo;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;

    TAF_MMA_ImsSwitchInfo imsSwitchInfo;
} TAF_MMA_ImsSwitchSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;

    VOS_UINT32 bitOpImsSwitch : 1;
    VOS_UINT32 bitOpRcsSwitch : 1;
    VOS_UINT32 bitOpSpare : 30;

    TAF_ERROR_CodeUint32 result;
} TAF_MMA_ImsSwitchSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_ImsSwitchQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    TAF_MMA_Ctrl         ctrl;
    TAF_ERROR_CodeUint32 result;
    TAF_MMA_ImsSwitchRat imsSwitchRat;
} TAF_MMA_ImsSwitchQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_RcsSwitchQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;

    TAF_ERROR_CodeUint32 result;
    VOS_UINT32           rcsSwitch;
} TAF_MMA_RcsSwitchQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32     msgName;
    TAF_MMA_Ctrl              ctrl;
    TAF_MMA_VoiceDomainUint32 voiceDomain;
} TAF_MMA_VoiceDomainSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                  msgName;
    VOS_UINT16                  clientId;
    VOS_UINT8                   opid;
    VOS_UINT8                   reserve[1];
    TAF_MMA_AppOperResultUint32 result;
    TAF_ERROR_CodeUint32        errorCause;
} TAF_MMA_VoiceDomainSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_VoiceDomainQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                msgName;
    VOS_UINT16                clientId;
    VOS_UINT8                 opid;
    VOS_UINT8                 reserve[1];
    TAF_MMA_VoiceDomainUint32 voiceDomain;
} TAF_MMA_VoiceDomainQryCnf;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                  msgName; /* _H2ASN_Skip */
    VOS_UINT16                  clientId;
    VOS_UINT8                   opid;
    TAF_PH_MODE                 phMode;  /* ģʽ */
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
} TAF_MMA_PhoneModeSetCnf;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                  msgName; /* _H2ASN_Skip */
    VOS_UINT16                  clientId;
    VOS_UINT8                   opid;
    VOS_UINT8                   reserve[1];
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
} TAF_MMA_SysCfgCnf;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                  msgName; /* _H2ASN_Skip */
    VOS_UINT16                  clientId;
    VOS_UINT8                   opid;
    VOS_UINT8                   reserve[1];
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
} TAF_MMA_DetachCnf;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                  msgName; /* _H2ASN_Skip */
    VOS_UINT16                  clientId;
    VOS_UINT8                   opid;
    TAF_MMA_CardStatusUint8     cardStatus; /* ��״̬ */
    TAF_MMA_UsimmCardTypeUint32 cardType;   /* ������:SIM��USIM��ROM-SIM  */
} TAF_MMA_SimStatusInd;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_PlmnListPara  plmnListPara;
} TAF_MMA_PlmnListReq;


typedef struct {
    VOS_UINT8         imsCallFlg;
    MN_CALL_TypeUint8 callType;
    TAF_CALL_EmcDomainUint8 imsEmcDomin;
    VOS_UINT8               isMtCall;
} TAF_MMA_ImsCallInfo;


typedef struct {
    VOS_UINT8 imsSmsFlg;
    VOS_UINT8 isMtSms;
    VOS_UINT8 reserve[2];
} TAF_MMA_ImsSmsInfo;


typedef struct {
    VOS_UINT8 imsSsFlg;
    VOS_UINT8 reserve[3];
} TAF_MMA_ImsSsInfo;


typedef struct {
    VOS_UINT8 emcFlg;
    VOS_UINT8 reserve[3];
} TAF_MMA_EmcInfo;


typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32          msgName; /* _H2ASN_Skip */
    VOS_UINT32          bitOpImsCall : 1;
    VOS_UINT32          bitOpImsSms : 1;
    VOS_UINT32          bitOpImsSs : 1;
    VOS_UINT32          bitOpEmc : 1;
    VOS_UINT32          bitSpare : 28;
    TAF_MMA_ImsCallInfo imsCallInfo;
    TAF_MMA_ImsSmsInfo  imsSmsInfo;
    TAF_MMA_ImsSsInfo   imsSsInfo;

    TAF_MMA_EmcInfo emcInfo;
} TAF_MMA_SrvInfoNotify;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgName;
    VOS_UINT32          bitOpImsCall : 1;
    VOS_UINT32          bitOpImsSms : 1;
    VOS_UINT32          bitOpImsSs : 1;
    VOS_UINT32          bitOpEmc : 1;
    VOS_UINT32          bitOpCsCall : 1;
    VOS_UINT32          bitOpCsSms : 1;
    VOS_UINT32          bitOpCsSs : 1;
    VOS_UINT32          bitSpare : 25;
    TAF_MMA_ImsCallInfo imsCallInfo;
    TAF_MMA_ImsSmsInfo  imsSmsInfo;
    TAF_MMA_ImsSsInfo   imsSsInfo;
    TAF_MMA_EmcInfo     emcInfo;
    VOS_UINT32          csCallFlag;
    VOS_UINT32          csSmsFlag;
    VOS_UINT32          csSsFlag;
} TAF_MMA_ExtendSrvInfoNotify;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
} TAF_MMA_StopSrvacqNotify;

typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
} TAF_MMA_StopSrvAcqReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT32            moduleId;
} TAF_MMA_RestartReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
} TAF_MMA_RestartCnf;


typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    VOS_UINT32                        msgName; /* _H2ASN_Skip */
    TAF_MMA_UpdateTypecPdpCountUint32 cPdpCount;
} TAF_MMA_UpdateTypeCPdpCountNotify;


typedef struct {
    VOS_UINT32 lac;
    TAF_MMA_RatTypeUint8 rat;
    VOS_UINT8 rsv;
    VOS_INT16 rscp;
} TAF_LacWithRatAndRscp;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                 msgName;
    VOS_UINT16                 clientId;
    VOS_UINT8                  opid;
    VOS_UINT8                  rsv;
    VOS_UINT32                 plmnNum;
    TAF_PH_NETWORK_STATUS_FLAG plmnStatus[TAF_MMA_MAX_SRCHED_LAI_NUM];
    TAF_LacWithRatAndRscp      laiWithRscp[TAF_MMA_MAX_SRCHED_LAI_NUM];
    TAF_PH_OperatorName        plmnName[TAF_MMA_MAX_SRCHED_LAI_NUM];
} TAF_MMA_SrchedPlmnInfoInd;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgId; /* _H2ASN_Skip */
    TAF_MMA_CdmaCallTypeUint8 callType;
    VOS_UINT8                 reserve[3];
} TAF_MMA_CdmaMoCallStartNtf;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgId; /* _H2ASN_Skip */
    TAF_MMA_CdmaCallTypeUint8 callType;
    VOS_UINT8                 reserve[3];
} TAF_MMA_CdmaMoCallSuccessNtf;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT8             reserve[4];
} TAF_MMA_CdmaMoSmsStartNtf;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT8             reserve[4];
} TAF_MMA_CdmaMoSmsEndNtf;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgId; /* _H2ASN_Skip */
    TAF_MMA_CdmaCallTypeUint8 callType;
    VOS_UINT8                 rsv[3];
    VOS_UINT32                cause;
} TAF_MMA_CdmaCallRedialSystemAcquireNtf;

/*
 * Description:
 */
typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgId; /* _H2ASN_Skip */
    TAF_MMA_CdmaCallTypeUint8 callType;
    VOS_UINT8                 reserve[3];
} TAF_MMA_CdmaMoCallEndNtf;

typedef TAF_MMA_CdmaMoCallStartNtf TAF_MMA_CdmaMtCallStartNtf;

typedef TAF_MMA_CdmaMoCallEndNtf TAF_MMA_CdmaMtCallEndNtf;

/*
 * Description: the reason TAF APS entering dormant
 */
typedef struct {
    VOS_MSG_HEADER                            /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32   msgId;            /* _H2ASN_Skip */
    TAF_MMA_PsRatTypeUint32 ratTypeEnterDorm; /* ����dormant�Ľ��뼼�� */
} TAF_MMA_CdmaEnterDormForPriorSysNtf;


typedef struct {
    VOS_UINT8 year;
    VOS_UINT8 month;
    VOS_UINT8 day;
    VOS_UINT8 hour;
    VOS_UINT8 minute;
    VOS_UINT8 second;
    VOS_INT8  timeZone;
    VOS_UINT8 dayltSavings;
} TAF_MMA_TimeZoneCtime;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32            msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opid;
    VOS_UINT8             reserve[1];
    TAF_MMA_TimeZoneCtime timezoneCTime;
} TAF_MMA_CtimeInd;


typedef TAF_NVIM_CfreqLockCfg TAF_MMA_CfreqLockSetPara;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT32               msgName;  /* _H2ASN_Skip */
    VOS_UINT32               moduleId; /* ����PID */
    VOS_UINT16               clientId;
    VOS_UINT8                opId;
    VOS_UINT8                reserve[1];
    TAF_MMA_CfreqLockSetPara cFreqLockPara;
} TAF_MMA_CfreqLockSetReq;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_UINT32     rslt;
} TAF_MMA_CfreqLockSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CfreqLockQueryReq;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT32               msgName;  /* _H2ASN_Skip */
    VOS_UINT32               moduleId; /* ����PID */
    TAF_MMA_Ctrl             ctrl;
    TAF_MMA_CfreqLockSetPara cFreqLockPara;
} TAF_MMA_CfreqLockQueryCnf;


enum TAF_MMA_CdmaCsqMode {
    TAF_MMA_CDMA_CSQ_MODE_DISABLE = 0,
    TAF_MMA_CDMA_CSQ_MODE_ENABLE  = 1,
    TAF_MMA_CDMA_CSQ_MODE_BUTT
};

typedef VOS_UINT8 TAF_MMA_CdmaCsqModeUint8;


enum TAF_MMA_HdrCsqMode {
    TAF_MMA_HDR_CSQ_MODE_DISABLE = 0,
    TAF_MMA_HDR_CSQ_MODE_ENABLE  = 1,
    TAF_MMA_HDR_CSQ_MODE_BUTT
};

typedef VOS_UINT8 TAF_MMA_HdrCsqModeUint8;


typedef struct {
    TAF_MMA_HdrCsqModeUint8 mode; /* �����ϱ�ģʽ */
    VOS_UINT8               rsv[3];
    VOS_UINT8               timeInterval; /* �����ϱ��ź�ǿ�ȵ���С���ʱ�� */
    /* �ϱ�����ֵ����RSSI��ǿ�ȱ仯���ڵ���ucRssiThresholdʱ�ϱ� */
    VOS_UINT8 rssiThreshold;
    /* �ϱ�����ֵ����SNR��ǿ�ȱ仯���ڵ���ucSnrThresholdʱ�ϱ�,��δʹ�� */
    VOS_UINT8 snrThreshold;
    /* �ϱ�����ֵ����ECIO��ǿ�ȱ仯���ڵ���ucEcioThresholdʱ�ϱ�,��δʹ�� */
    VOS_UINT8 ecioThreshold;
} TAF_MMA_HdrCsqPara;


typedef struct {
    TAF_MMA_CdmaCsqModeUint8 mode;         /* �����ϱ�ģʽ */
    VOS_UINT8                timeInterval; /* �����ϱ��ź�ǿ�ȵ���С���ʱ�� */
    /* �ϱ�����ֵ����RSSI��ǿ�ȱ仯���ڵ���ucRssiRptThresholdʱ�ϱ� */
    VOS_UINT8 rssiRptThreshold;
    /* �ϱ�����ֵ����ec/Io��ǿ�ȱ仯���ڵ���ucEcIoRptThresholdʱ�ϱ� */
    VOS_UINT8 ecIoRptThreshold;
} TAF_MMA_CdmacsqPara;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_CdmacsqPara   cdmaCsqPara;
} TAF_MMA_CdmacsqSetReq;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                  msgName; /* _H2ASN_Skip */
    VOS_UINT16                  clientId;
    VOS_UINT8                   opId;
    VOS_UINT8                   reserve[1];
    TAF_MMA_AppOperResultUint32 rslt;
} TAF_MMA_CdmacsqSetCnf;

typedef struct {
    VOS_INT16 cdmaRssi;
    VOS_INT16 cdmaEcIo;
} TAF_MMA_SigQualityRptInfo;

typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT32                msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl              ctrl;
    TAF_MMA_CdmaCsqModeUint8  mode;
    VOS_UINT8                 timeInterval;
    VOS_UINT8                 rssiRptThreshold;
    VOS_UINT8                 ecIoRptThreshold;
    TAF_MMA_SigQualityRptInfo sigQualityInfo;
} TAF_MMA_CdmacsqQueryCnf;

typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opid;
    VOS_UINT8      reserve[1];
    VOS_INT16      cdmaRssi;
    VOS_INT16      cdmaEcIo;
} TAF_MMA_CdmacsqInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CdmacsqQryReq;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opid;
    VOS_UINT8      isCLMode;
} TAF_MMA_ClmodeInd;


typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32          msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl        ctrl;
    TAF_PH_FplmnOperate cFPlmnPara;
} TAF_MMA_CfplmnSetReq;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
} TAF_MMA_CfplmnQueryReq;


typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32             msgName; /* _H2ASN_Skip */
    TAF_MMA_AppCfplmnOperResultUint32 rslt;
    TAF_MMA_Ctrl                      ctrl;
    TAF_UINT16                        plmnNum;
    TAF_UINT8                         reserved[2];
    TAF_PLMN_Id                       plmn[TAF_USER_MAX_PLMN_NUM];
} TAF_MMA_CfplmnSetCnf;

typedef struct {
    VOS_MSG_HEADER                             /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32             msgName; /* _H2ASN_Skip */
    TAF_MMA_AppCfplmnOperResultUint32 rslt;
    TAF_MMA_Ctrl                      ctrl;
    TAF_UINT16                        plmnNum;
    TAF_UINT8                         reserved[2];
    TAF_PLMN_Id                       plmn[TAF_USER_MAX_PLMN_NUM];
} TAF_MMA_CfplmnQueryCnf;


typedef struct {
    VOS_UINT8 simPlmn[TAF_MMA_SIM_FORMAT_PLMN_LEN];
    VOS_UINT8 reserve[1];
} TAF_MMA_SimFormatPlmn;


typedef struct {
    TAF_PLMN_Id plmnId; /* PLMN ID */
    VOS_UINT16  simRat; /* SIM����֧�ֵĽ��뼼�� */
    VOS_UINT8   reserve[2];
} TAF_MMA_PlmnWithSimRat;


typedef struct {
    VOS_UINT16             dplmnNum;  /* Ԥ��DPLMN�б�ĸ��� */
    VOS_UINT8              ehPlmnNum; /* EHPLMN�ĸ��� */
    VOS_UINT8              reserve;
    TAF_PLMN_Id            ehPlmnInfo[TAF_MMA_MAX_EHPLMN_NUM]; /* EHPLMN ID�б� */
    TAF_MMA_PlmnWithSimRat dplmnList[TAF_MMA_MAX_DPLMN_NUM];   /* DPLMN�б� */
} TAF_MMA_DplmnInfoSet;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT32           msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl         ctrl;
    VOS_UINT8            seq;                                 /* ��ˮ�� */
    VOS_UINT8            versionId[TAF_MMA_VERSION_INFO_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    VOS_UINT8            reserve[2];
    TAF_MMA_DplmnInfoSet dplmnInfo;
} TAF_MMA_DplmnSetReq;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    ModemIdUint16  firstModemId;
    ModemIdUint16  secondModemId;
} TAF_MMA_ExchangeModemInfoReqMsg;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_UINT32     rslt;
} TAF_MMA_ExchangeModemInfoCnf;


typedef struct {
    VOS_UINT8 seq; /* ��ˮ�� */
    /* �汾�ţ��̶�Ϊxx.xx.xxx */
    VOS_UINT8 versionId[TAF_MMA_VERSION_INFO_LEN];
    VOS_UINT8 reserve[1];
    VOS_UINT8 ehPlmnNum; /* EHPLMN�ĸ��� */
    /* EHPLMN ID�б� */
    TAF_PLMN_Id ehPlmnList[TAF_MMA_MAX_EHPLMN_NUM];
    VOS_UINT32  borderInfoLen;     /* �߾���Ϣ���� */
    VOS_UINT8   borderInfoBuff[4]; /* �߾���Ϣ */
} TAF_MMA_BorderInfo;


typedef struct {
    VOS_MSG_HEADER                                 /* _H2ASN_Skip */
    VOS_UINT32                            msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl                          ctrl;
    /* �������� 0:���� 1:ɾ������ */
    TAF_MMA_SetBorderInfoOperateTypeUint8 operateType;
    VOS_UINT8                             reserve[3];
    TAF_MMA_BorderInfo                    borderInfo; /* �߾���Ϣ */
} TAF_MMA_BorderInfoSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT32   rslt;
} TAF_MMA_BorderInfoSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_BorderInfoQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT8    versionId[NAS_VERSION_LEN];
    VOS_UINT8    reverse[3];
} TAF_MMA_BorderInfoQryCnf;

#if (FEATURE_ON == FEATURE_DSDS)

typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_UINT8      enable; /* �������� 0:���������ϱ� 1:���������ϱ� */
    VOS_UINT8      reserve[3];
} TAF_MMA_DsdsStateSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT32   rslt;
} TAF_MMA_DsdsStateSetCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  supportDsds3; /* �Ƿ�֧��DSDS3.0�� 1��DSDS3.0 0��DSDS2.0 */
} TAF_MMA_DsdsStateNotify;
#endif


typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    VOS_UINT32         msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl       ctrl;
    TAF_PH_SetPrefplmn prefPlmn;
} TAF_MMA_PrefPlmnSetReq;


typedef struct {
    VOS_MSG_HEADER                               /* _H2ASN_Skip */
    VOS_UINT32                          msgName; /* _H2ASN_Skip */
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved[1];
    TAF_MMA_AppPrefPlmnOperResultUint32 rslt;
} TAF_MMA_PrefPlmnSetCnf;


typedef struct {
    VOS_MSG_HEADER                    /* _H2ASN_Skip */
    VOS_UINT32               msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl             ctrl;
    TAF_MMA_CpolInfoQueryReq cpolInfo;
} TAF_MMA_PrefPlmnQueryReq;


typedef struct {
    VOS_MSG_HEADER                               /* _H2ASN_Skip */
    VOS_UINT32                          msgName; /* _H2ASN_Skip */
    VOS_UINT16                          clientId;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserved[1];
    VOS_UINT32                          fromIndex;
    VOS_UINT32                          validPlmnNum; /* ��Ч�������Ӫ�����Ƹ��� */
    TAF_PLMN_NameList                   plmnName;
    TAF_MMA_AppPrefPlmnOperResultUint32 rslt;
} TAF_MMA_PrefPlmnQueryCnf;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT32              msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl            ctrl;
    MN_PH_PrefPlmnTypeUint8 prefPlmnType;
    VOS_UINT8               reserve[3];
} TAF_MMA_PrefPlmnTestReq;


typedef struct {
    VOS_MSG_HEADER                               /* _H2ASN_Skip */
    VOS_UINT32                          msgName; /* _H2ASN_Skip */
    VOS_UINT16                          clientId;
    VOS_UINT16                          plmnNum;
    VOS_UINT8                           opId;
    VOS_UINT8                           reserve[3];
    TAF_MMA_AppPrefPlmnOperResultUint32 rslt;
} TAF_MMA_PrefPlmnTestCnf;


typedef struct {
    /* _H2ASN_Skip */
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PhoneModeQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    TAF_PH_CMD_TYPE       cmdType; /* �������� */
    TAF_PH_MODE           phMode;  /* ģʽ */
    VOS_UINT8             reserved[2];
} TAF_MMA_PhoneModeQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            quickStartMode;
} TAF_MMA_QuickstartSetReq;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    TAF_PARA_SET_RESULT   result;
} TAF_MMA_QuickstartSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            autoAttachEnable;
} TAF_MMA_AutoAttachSetReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_PARA_SET_RESULT   result;
    VOS_UINT8             reserved[3];
} TAF_MMA_AutoAttachSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_SyscfgQryReq;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT32           msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl         ctrl;
    TAF_ERROR_CodeUint32 errorCause;
    TAF_MMA_SysCfgPara   sysCfg;
} TAF_MMA_SyscfgQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_SyscfgTestReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    TAF_PH_SysCfgBand     bandInfo;
#if (FEATURE_MBB_CUST == FEATURE_ON)
    TAF_USER_SetLtePrefBandInfo ueSupportLteBand;
#endif
} TAF_MMA_SyscfgTestCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_CrpnQryPara   crpnQryPara;
} TAF_MMA_CrpnQryReq;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT32           msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl         ctrl;
    TAF_ERROR_CodeUint32 errorCause;
    TAF_MMA_CrpnQryInfo  crpnQryInfo;
} TAF_MMA_CrpnQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    MM_TEST_AtCmdPara     cmmSetReq;
} TAF_MMA_CmmSetReq;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_UINT32     result;
    MM_TEST_AtRslt atCmdRslt; /* AT����ִ�з��� */
} TAF_MMA_CmmSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             actionType;
    VOS_UINT8             rrcMsgType;
    VOS_UINT8             signThreshold; /* 1:rssi�仯����1db�����ϱ�^cerssi; 2:rssi�仯����2db�����ϱ�
                                          * 3:rssi�仯����3db�����ϱ���4:rssi�仯����4db�����ϱ���
                                          * 5:rssi�仯����5db�����ϱ�;
                                          */
    VOS_UINT8             minRptTimerInterval; /* �ϱ��ļ��ʱ��(1-20)S */
} TAF_MMA_CerssiSetReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CerssiSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    VOS_UINT8 nsaQryFlag; /* �Ƿ���NSA��ѯ */
    VOS_UINT8 rsv[3];
#endif
} TAF_MMA_CerssiInfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_PH_Rssi           cerssi;  /* ��ǰС���ź�������Ϣ */
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CerssiInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_MtPowerDownReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_MtPowerDownCnf;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32   msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl            ctrl;
    MN_PH_PrefPlmnTypeUint8 prefPlmnType;
    VOS_UINT8               reserve[3];
} TAF_MMA_PrefPlmnTypeSetReq;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32   msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl            ctrl;
    TAF_ERROR_CodeUint32    errorCause;
    MN_PH_PrefPlmnTypeUint8 prefPlmnType;
    VOS_UINT8               reserved[3];
} TAF_MMA_PrefPlmnTypeSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CipherQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    VOS_UINT32            result;
    VOS_UINT8             mcgCipherInfo;
    VOS_UINT8             scgCipherInfo;
    VOS_UINT8             nrFlag;
    VOS_UINT8             reserved2;
} TAF_MMA_CipherQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_LocationInfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            result;
    VOS_UINT32            mcc;
    VOS_UINT32            mnc;
    VOS_UINT32            lac;
    VOS_UINT8             rac;
    VOS_UINT8             rsv[3];
    TAF_CELL_Id           cellId;
} TAF_MMA_LocationInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PlmnListAbortReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_PlmnListAbortCnf;


typedef TAF_MMA_CerssiSetReq TAF_MMA_CIND_SET_REQ_STRU;


typedef TAF_MMA_CerssiSetCnf TAF_MMA_CIND_SET_CNF_STRU;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_AcInfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            rslt;
    TAF_MMA_CellAcInfo    cellCsAcInfo;
    TAF_MMA_CellAcInfo    cellPsAcInfo;
} TAF_MMA_AcInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_MMA_CellAcInfo    cellAcInfo;
} TAF_MMA_AcInfoChangeInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT16            fromIndex;
    VOS_UINT16            plmnNum;
} TAF_MMA_CopnInfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT16            plmnNum;
    VOS_UINT16            fromIndex;
    VOS_UINT8             content[4];
} TAF_MMA_CopnInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl              ctrl;
    TAF_SIM_InsertStateUint32 simInsertState;
} TAF_MMA_SimInsertReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            result;
} TAF_MMA_SimInsertCnf;

/* EOPLMN Req */

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_SetEoplmnList eOPlmnSetPara;
} TAF_MMA_EoplmnSetReq;

/* EOPLMN Cnf */

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    VOS_UINT32            result;
} TAF_MMA_EoplmnSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EoplmnQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            result;
    VOS_UINT8             version[TAF_MAX_USER_CFG_OPLMN_VERSION_LEN]; /* OPLMN List�汾�� */
    TAF_UINT16            oPlmnNum;                                    /* OPLMN���� */
    TAF_UINT8             reserved[2];
    /* OPLMN��PDU���ݣ���EFOplmn�ļ�һ�� */
    TAF_UINT8             oPlmnList[TAF_MAX_USER_CFG_OPLMN_DATA_LEN];
} TAF_MMA_EoplmnQryCnf;

/* Net SCAN request */

enum TAF_MMA_NetScanRatMode {
    TAF_MMA_NET_SCAN_RAT_MODE_GSM,  /* GSM���뼼�� */
    TAF_MMA_NET_SCAN_RAT_MODE_UMTS, /* UMTS���뼼�� */
    TAF_MMA_NET_SCAN_RAT_MODE_LTE,  /* LTE���뼼�� */

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    TAF_MMA_NET_SCAN_RAT_MODE_LTE_NRDC, /* ENDCģʽ */
    TAF_MMA_NET_SCAN_RAT_MODE_NR,       /* NR���뼼�� */
#endif

    TAF_MMA_NET_SCAN_RAT_MODE_BUTT
};
typedef VOS_UINT8 TAF_MMA_NetScanRatModeUint8;


typedef struct {
    VOS_UINT16                  cellNum; /* ��Ҫɨ����������޵�С������ */
    VOS_INT16                   cellPow; /* С������ֵ */
    TAF_MMA_NetScanRatModeUint8 rat;     /* ��Ҫɨ��Ľ���ģʽ */
    VOS_UINT8                   reserve[3];
    TAF_MMA_BandSet             band; /* Ƶ�� */
} TAF_MMA_NetScanSetPara;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32  msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl           ctrl;
    TAF_MMA_NetScanSetPara para;
} TAF_MMA_NetScanReqMsg;

/* Net SCAN Confirmation */

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32      msgName; /* _H2ASN_Skip */
    VOS_UINT16                 clientId;
    VOS_UINT8                  opId;
    TAF_MMA_NetScanResultUint8 result;
    TAF_MMA_NetScanCauseUint8  cause;
    VOS_UINT8                  freqNum;
    VOS_UINT8                  reserved[2];
    TAF_MMA_NetScanInfo        netScanInfo[TAF_MMA_NET_SCAN_MAX_FREQ_NUM];
} TAF_MMA_NetScanCnf;

/* ABORT_NET_SCAN_REQ */

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_NetScanAbortReqMsg;

/*
 * Description:
 */
enum TAF_MMA_CdmaLocInfoRatMode {
    TAF_MMA_CDMA_LOC_INFO_RAT_MODE_INVALID = 0x00,
    TAF_MMA_CDMA_LOC_INFO_RAT_MODE_1X      = 0x01,
    TAF_MMA_CDMA_LOC_INFO_RAT_MODE_DO      = 0x02,
};
typedef VOS_UINT8 TAF_MMA_CdmaLocInfoRatModeUint8;

/* CLOCINFO */

typedef struct {
    TAF_MMA_CdmaLocInfoRatModeUint8 ratMode;
    VOS_UINT8                       rsv[3];
    VOS_UINT32                      mcc;
    VOS_UINT32                      mnc;
    VOS_INT32                       sid;
    VOS_INT32                       nid;
    VOS_UINT16                      prevInUse;
    VOS_UINT8                       reserve[2];
    VOS_UINT32                      baseId;        /* Base stationidentification */
    VOS_INT32                       baseLatitude;  /* Base stationlatitude */
    VOS_INT32                       baseLongitude; /* Base stationLongitude */
} TAF_MMA_ClocinfoPara;


typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_ClocinfoPara  clocinfoPara;
} TAF_MMA_ClocinfoInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CdmaLocinfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            rslt;
    TAF_MMA_ClocinfoPara  clocinfoPara;
} TAF_MMA_CdmaLocinfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_BatteryCapacityQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_Batterypower  batteryStatus;
} TAF_MMA_BatteryCapacityQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_HandShakeQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             buf[AT_HS_PARA_MAX_LENGTH];
    VOS_UINT8             reserve[3];
} TAF_MMA_HandShakeQryCnf;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_INT32      sid;

} TAF_MMA_CsidInd;


typedef struct {
    VOS_MSG_HEADER                            /* _H2ASN_Skip */
    VOS_UINT32                       msgName; /* _H2ASN_Skip */
    VOS_UINT16                       clientId;
    VOS_UINT8                        opid;
    TAF_PHONE_CHANGED_SERVICE_STATUS srvStatus;
} TAF_MMA_SrvStatusInd;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opid;
    VOS_UINT8      reserve;
} TAF_MMA_ImsiRefreshInd;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl   ctrl;
    VOS_UINT8      resetStep;
    VOS_UINT8      reserve[3];
} TAF_MMA_ResetNtf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    VOS_UINT8 isNsaRptFlg;
#else
    VOS_UINT8            rsv;
#endif
    VOS_UINT8 curcRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8   unsolicitedRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE];
    TAF_PH_Rssi rssiInfo;
} TAF_MMA_RssiInfoInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_PH_RegState       regStatus;
} TAF_MMA_RegStatusInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_PH_RegRejInfo     regRejInfo;
} TAF_MMA_RegRejInfoInd;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgName; /* _H2ASN_Skip */
    VOS_UINT16                clientId;
    VOS_UINT8                 opId;
    VOS_UINT8                 reserved[1];
    TAF_MMA_PlmnSelectionInfo plmnSelectInfo;
} TAF_MMA_PlmnSelectionInfoInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_SpnQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_MMA_SpnPara       mnMmaSpnInfo;
} TAF_MMA_SpnQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_MmplmninfoQryReq;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32  msgName; /* _H2ASN_Skip */
    VOS_UINT16             clientId;
    VOS_UINT8              opId;
    VOS_UINT8              reserved[1];
    TAF_MMA_MmInfoPlmnName mmPlmnInfo;
} TAF_MMA_MmplmninfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_LastCampPlmnQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_PLMN_Id           plmnId;
} TAF_MMA_LastCampPlmnQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_UserSrvStateQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    VOS_UINT32            csSrvExistFlg; /* CS???????? */
    VOS_UINT32            psSrvExistFlg; /* PS???????? */
} TAF_MMA_UserSrvStateQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PowerOnAndRegTimeQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    VOS_UINT32            costTime;
} TAF_MMA_PowerOnAndRegTimeQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_AccessModeQryReq;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32   msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl            ctrl;
    MN_MMA_CpamRatTypeUint8 accessMode;
    TAF_PH_PLMN_PRIO        plmnPrio;
    VOS_UINT8               reserved[2];
} TAF_MMA_AccessModeQryCnf;

/*
 * ö����: TAF_MMA_QuickstartQryReq
 * �ṹ˵��: ����APP��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_QuickstartQryReq;

/*
 * ö����: TAF_MMA_QuickstartQryCnf
 * �ṹ˵��: ����APP��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_ERROR_CodeUint32  errorCause;
    VOS_UINT32            quickStartMode; /* ģʽ */
} TAF_MMA_QuickstartQryCnf;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CsnrQryReq;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_CsnrPara      csnrPara;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CsnrQryCnf;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CsqQryReq;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
    TAF_PH_Rssi           csq;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_CsqQryCnf;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CsqlvlQryReq;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣ�� */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    TAF_MMA_CsqlvlPara    csqLvlPara;
    TAF_MMA_CsqlvlextPara csqLvlExtPara;
    VOS_UINT8             reserved[2];
} TAF_MMA_CsqlvlQryCnf;


typedef struct {
    MSG_Header     msgHeader; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT8      lSAID[3];
    VOS_UINT8      ieFlg;
    VOS_INT8       localTimeZone;
    VOS_UINT8      dST;
    VOS_UINT8      reserve[2];
    VOS_UINT8      curcRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE];
    VOS_UINT8      unsolicitedRptCfg[TAF_MMA_RPT_CFG_MAX_SIZE];
    TIME_ZONE_Time universalTimeandLocalTimeZone;
} TAF_MMA_TimeChangeInd;


typedef struct {
    MSG_Header           msgHeader; /* _H2ASN_Skip */
    VOS_UINT16           clientId;
    VOS_UINT8            opId;
    TAF_PH_INFO_RAT_TYPE ratType;
    TAF_SysSubmodeUint8  sysSubMode;
    VOS_UINT8            reserve[3];
} TAF_MMA_ModeChangeInd;


typedef struct {
    MSG_Header  msgHeader; /* _H2ASN_Skip */
    VOS_UINT16  clientId;
    VOS_UINT8   opId;
    VOS_UINT8   reserved[1];
    TAF_PLMN_Id curPlmn;
} TAF_MMA_PlmnChangeInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CopsQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             reserved[1];
    TAF_PH_Networkname    copsInfo;
} TAF_MMA_CopsQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_DplmnQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT8    versionId[NAS_VERSION_LEN];
    VOS_UINT8    reverse[3];
} TAF_MMA_DplmnQryCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT32   rslt;
} TAF_MMA_DplmnSetCnf;


typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32          msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl                   ctrl;
    TAF_MMA_QryRegStatusTypeUint32 qryRegStaType;
} TAF_MMA_RegStateQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_PH_RegState       regInfo;
} TAF_MMA_RegStateQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_AutoAttachQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    VOS_UINT8             autoAttachFlag;
    VOS_UINT8             reserved[3];
} TAF_MMA_AutoAttachQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    /* ��������^sysinfo ���� ^sysinfoex ���� */
    VOS_UINT32            sysInfoExFlag;
} TAF_MMA_SysinfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opId;
    VOS_UINT8             isSupport;
    TAF_PH_Sysinfo        sysInfo;
} TAF_MMA_SysinfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_AntennaInfoQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    MN_MMA_AnqueryPara    antennaInfo;
} TAF_MMA_AntennaInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_HomePlmnQryReq;


typedef struct {
    VOS_UINT8   imsi[NAS_MAX_IMSI_LENGTH];
    VOS_UINT8   hplmnMncLen;
    VOS_UINT8   reserve[2];
    VOS_UINT32  eHplmnNum;
    TAF_PLMN_Id eHplmnList[TAF_MMA_MAX_EHPLMN_NUM];
} TAF_MMA_EhplmnInfo;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
    TAF_MMA_EhplmnInfo    eHplmnInfo;
} TAF_MMA_HomePlmnQryCnf;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32   msgName; /* _H2ASN_Skip */
    TAF_MMA_PsRatTypeUint32 ratType;
} TAF_MMA_PsRatTypeNtf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32   msgName;
    TAF_MMA_Ctrl            ctrl;
    TAF_MMA_AttachTypeUint8 attachType;
    VOS_UINT8               rsved[3];
} TAF_MMA_AttachReqMsg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
} TAF_MMA_AttachCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32      msgName;
    TAF_MMA_Ctrl               ctrl;
    TAF_MMA_ServiceDomainUint8 domainType;
    VOS_UINT8                  reserved[3];
} TAF_MMA_AttachStatusQryReqMsg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_AppOperResultUint32 rslt;
    TAF_ERROR_CodeUint32        errorCause;
    TAF_MMA_ServiceDomainUint8  domainType;
    TAF_MMA_AttachStatusUint8   csStatus;
    TAF_MMA_AttachStatusUint8   psStatus;
    VOS_UINT8                   reserved[1];
} TAF_MMA_AttachStatusQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_1XchanQueryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    AT_APPCTRL atAppCtrl;
    VOS_UINT32 rslt;
    VOS_UINT16 channel;
    VOS_UINT8  reserve[2];
} TAF_MMA_1XchanQueryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CverQueryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32             msgName;
    AT_APPCTRL             atAppCtrl;
    VOS_UINT32             rslt;
    TAF_MMA_1XCasPRevUint8 prevInUse;
    VOS_UINT8              reserve[3];
} TAF_MMA_CverQueryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_StateQueryReq;


typedef struct {
    VOS_UINT8 casState;
    VOS_UINT8 casSubSta;
    VOS_UINT8 reserve[2];
} TAF_MMA_HandsetState;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    AT_APPCTRL           atAppCtrl;
    VOS_UINT32           rslt;
    TAF_MMA_HandsetState handsetSta;
} TAF_MMA_StateQueryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_ChighverQueryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32             msgName;
    AT_APPCTRL             atAppCtrl;
    VOS_UINT32             rslt;
    TAF_MMA_1XCasPRevUint8 highRev;
    VOS_UINT8              reserve[3];
} TAF_MMA_ChighverQueryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_QuitCallbackSetReq;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCode;
} TAF_MMA_QuitCallbackSetCnf;


typedef struct {
    VOS_UINT16 startSid;
    VOS_UINT16 endSid;
    VOS_UINT32 mcc;
} TAF_MMA_SidWhiteList;


typedef struct {
    VOS_UINT8 enable; /* Trust�����Ƿ�ʹ�� */
    VOS_UINT8 reserved;
    /* ֧��Trust�����ĸ���,����Ϊ0ʱ��ʾ��֧��Trust���� */
    VOS_UINT16           trustSysNum;
    TAF_MMA_SidWhiteList sysInfo[TAF_MMA_MAX_TRUST_LOCK_SID_NUM];
} TAF_MMA_OperLockTrustSid;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32    msgName;
    TAF_MMA_Ctrl             ctrl;
    TAF_MMA_OperLockTrustSid trustSidInfo;

} TAF_MMA_CsidlistSetReq;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCode;
} TAF_MMA_CsidlistSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */ /* ��Ϣͷ    */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */ /* ��Ϣͷ    */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            isInCallBack; /* �Ƿ���CallBackģʽ�� */
} TAF_MMA_EmcCallBackNtf;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EmcCallBackQryReq;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            isInCallBackMode;
    TAF_ERROR_CodeUint32  errorCode;
} TAF_MMA_EmcCallBackQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_HdrCsqPara    hdrCsqSetting;

} TAF_MMA_HdrCsqSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_AppOperResultUint32 rslt;
} MMA_TAF_HdrCsqSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_HdrCsqQrySettingReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_HdrCsqPara    hdrCsq;
    VOS_INT16             hdrRssi;
    VOS_INT16             hdrSnr;
    VOS_INT16             hdrEcio;
    VOS_UINT8             rsv[2];
} MMA_TAF_HdrCsqQrySettingCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_INT16             hdrRssi;
    VOS_INT16             hdrSnr;
    VOS_INT16             hdrEcio;
    VOS_UINT8             rsv[2];
} MMA_TAF_HdrCsqValueInd;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CurrSidNidQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCode;
    VOS_INT32             sid; /* *<  System identification */
    VOS_INT32             nid; /* *<  Network identification */
} TAF_MMA_CurrSidNidQryCnf;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opid;
    VOS_UINT8      reserve[1];
    VOS_UINT32     mcc; /* LTE�ĳ�ʼλ����Ϣ�Ĺ����룬Mcc����ȫf����ʾ��Чֵ */
    VOS_INT32      sid; /* 1x�ĳ�ʼλ����Ϣ��Sid�� sid����-1����ʾ��Чֵ */
} TAF_MMA_InitLocInfoInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opid;
    VOS_UINT8             reserve[1];
} TAF_MMA_RoamingModeSwitchInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    VOS_UINT16            clientId;
    VOS_UINT8             opid;
    VOS_UINT8             reserve[1];
    VOS_INT32             sid;
    VOS_UINT32            ul3Gpp2Mcc;
    VOS_UINT32            ul3GppMcc;
    VOS_UINT32            modeType;
} TAF_MMA_CtccRoamingNwInfoReportInd;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CtccRoamingNwInfoQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_INT32             sid;        /* *<  System identification */
    VOS_UINT32            ul3Gpp2Mcc; /* *<  3GPP2 MCC */
    VOS_UINT32            ul3GppMcc;  /* *<  3GPP MCC */
    VOS_UINT32            modeType;   /* ָʾ��ǰ��CL����GUL��0��ʾGUL��1��ʾCL */
} TAF_MMA_CtccRoamingNwInfoQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             ctRoamRtpFlag;
    VOS_UINT8             reserved[3];
} TAF_MMA_CtccRoamingNwInfoRtpCfgSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCode;
} TAF_MMA_CtccRoamingNwInfoRtpCfgSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT16            clOosCount;
    VOS_UINT16            gulOosCount;
} TAF_MMA_CtccOosCountSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCode;
} TAF_MMA_CtccOosCountSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32          msgName;
    TAF_MMA_Ctrl                   ctrl;
    TAF_MMA_ImsDomainCfgTypeUint32 imsDoaminCfg;
} TAF_MMA_ImsDomainCfgSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_ImsDomainCfgQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32            msgName;
    VOS_UINT16                       clientId;
    VOS_UINT8                        opid;
    VOS_UINT8                        reserve[1];
    TAF_MMA_ImsDomainCfgResultUint32 imsDomainCfgResult;
} TAF_MMA_ImsDomainCfgSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32          msgName;
    VOS_UINT16                     clientId;
    VOS_UINT8                      opid;
    VOS_UINT8                      reserve[1];
    TAF_MMA_ImsDomainCfgTypeUint32 imsDomainCfgType;
} TAF_MMA_ImsDomainCfgQryCnf;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32        msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl                 ctrl;
    TAF_MMA_RoamImsSupportUint32 roamingImsSupportFlag;
} TAF_MMA_RoamImsSupportSetReqMsg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32              msgName;
    VOS_UINT16                         clientId;
    VOS_UINT8                          opid;
    VOS_UINT8                          reserve[1];
    TAF_MMA_RoamImsSupportResultUint32 roamImsSupportResult;
} TAF_MMA_RoamImsSupportSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PrlidQryReq;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_INT32             prlSrcType;
    VOS_INT32             prlId;
    VOS_INT32             mlplMsplSrcType;
    VOS_INT32             mlplId;
    VOS_INT32             msplId;
} TAF_MMA_PrlidQryCnf;


typedef struct {
    VOS_UINT32                   tmsi;
    TAF_PLMN_Id                  plmnId;
    VOS_UINT16                   lac;
    TAF_MMA_LocUpdateStatusUint8 locationUpdateStatus;
    VOS_UINT8                    rfu;
    VOS_UINT32                   reserve[2];
} TAF_MMA_Eflociinfo;


typedef struct {
    VOS_UINT32                   pTmsi;
    TAF_PLMN_Id                  plmnId;
    VOS_UINT32                   pTmsiSignature;
    VOS_UINT16                   lac;
    VOS_UINT8                    rac;
    TAF_MMA_LocUpdateStatusUint8 psLocationUpdateStatus;
    VOS_UINT32                   reserve[2];
} TAF_MMA_Efpslociinfo;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_Eflociinfo    eflociInfo;
} TAF_MMA_EflociinfoSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_AppOperResultUint32 rslt;
} TAF_MMA_EflociinfoSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EflociinfoQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_Eflociinfo          eflociInfo;
    TAF_MMA_AppOperResultUint32 rslt;
} TAF_MMA_EflociinfoQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_Efpslociinfo  psEflociInfo;
} TAF_MMA_EfpslociinfoSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_AppOperResultUint32 rslt;
} TAF_MMA_EfpslociinfoSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EfpslociinfoQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_Efpslociinfo        psEflociInfo;
    TAF_MMA_AppOperResultUint32 rslt;
} TAF_MMA_EfpslociinfoQryCnf;


typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32      msgName; /* _H2ASN_Skip */
    VOS_UINT16                 clientId;
    VOS_UINT8                  opid;
    VOS_UINT8                  reserve[1];
    TAF_MMA_CombinedModeUint32 combinedMode;
} TAF_MMA_CombinedModeSwitchInd;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_RatCombinedModeQryReq;

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32      msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl               ctrl;
    TAF_MMA_CombinedModeUint32 combinedMode;
} TAF_MMA_RatCombinedModeQryCnf;

typedef struct {
    VOS_MSG_HEADER                               /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32               msgName; /* _H2ASN_Skip */
    VOS_UINT16                          clientId;
    VOS_UINT8                           opid;
    VOS_UINT8                           reserve[1];
    TAF_MMA_IccAppTypeSwitchStateUint32 switchState;
} TAF_MMA_IccAppTypeSwitchInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_SrvTypeUint8  srvType;
    VOS_UINT8             reserved[3];
} TAF_MMA_SrvEndNotify;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32     msgName; /* _H2ASN_Skip */
    TAF_MMA_ImsRegDomainUint8 imsRegDomain;
    TAF_MMA_ImsRegStatusUint8 imsRegStatus;
    VOS_UINT8                 reserved[2];
} TAF_MMA_ImsRegDomainNotifyMsg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PacspQryReq;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             plmnMode;
    VOS_UINT8             reserved[3];
} TAF_MMA_PacspQryCnf;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
} TAF_MMA_SmcNoEntityNotify;


enum TAF_MMA_ImsVideoCallCapType {
    TAF_MMA_IMS_VIDEO_CALL_CAP_SWITCH = 0x00000000, /* IMS��Ƶ�绰��̬���� */
    TAF_MMA_IMS_VIDEO_CALL_CAP_CCWA   = 0x00000001, /* IMS��Ƶ�绰���еȴ����� */

    TAF_MMA_IMS_VIDEO_CALL_CAP_BUTT
};
typedef VOS_UINT32 TAF_MMA_ImsVideoCallCapTypeUint32;


enum TAF_MMA_IsSamePlmnFlag {
    TAF_MMA_DIFFERENT_PLMN = 0,
    TAF_MMA_SAME_PLMN = 1,
    TAF_MMA_IS_SAME_PLMN_BUTT
};
typedef VOS_UINT8 TAF_MMA_IsSamePlmnFlagUint8;


typedef struct {
    TAF_MMA_ImsVideoCallCapTypeUint32 videoCallCapType; /* ��Ҫ���õ�IMS VT�������� */
    /* IMS VTĳ������������ֵ: VOS_FALSE,�أ�VOS_TRUE,�� */
    VOS_UINT32 videoCallCapValue;
} TAF_MMA_ImsVideoCallCap;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32   msgName;
    TAF_MMA_Ctrl            ctrl;
    TAF_MMA_ImsVideoCallCap imsVtCap;
} TAF_MMA_ImsVideoCallCapReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;

    TAF_ERROR_CodeUint32 result;
} TAF_MMA_ImsVideoCallCapSetCnf;


typedef struct {
    VOS_UINT8 wifiEnable;
    VOS_UINT8 lteEnable;
    VOS_UINT8 utranEnable;
    VOS_UINT8 gsmEnable;
} TAF_MMA_ImsSmsCfg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_ImsSmsCfg     imsSmsCfg;
} TAF_MMA_ImsSmsCfgSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_ImsSmsCfgQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;

    TAF_ERROR_CodeUint32 result;
} TAF_MMA_ImsSmsCfgSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;

    TAF_ERROR_CodeUint32 result;
    TAF_MMA_ImsSmsCfg    imsSmsCfg;
} TAF_MMA_ImsSmsCfgQryCnf;


typedef struct {
    VOS_UINT8                            osId[TAF_MMA_OSID_LEN];
    VOS_UINT8                            appId[TAF_MMA_MAX_APPID_LEN + 1];
    VOS_UINT8                            reserve[3];
    TAF_MMA_ApplicationsIndicationUint32 appIndication;
} TAF_MMA_AcdcAppInfo;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_AcdcAppInfo   acdcAppInfo;
} TAF_MMA_AcdcAppNotifyMsg;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32       msgName;
    TAF_MMA_Ctrl                ctrl;
    TAF_MMA_IsSamePlmnFlagUint8 isSamePlmn;
    VOS_UINT8                   reserve[3];
} TAF_MMA_QuickCardSwithIndMsg;


typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgName; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opid;
    VOS_UINT8      reserve;
} TAF_MMA_MtreattachInd;


enum TAF_MMA_SearchType {
    TAF_MMA_SEARCH_TYPE_AUTO        = 0,
    TAF_MMA_SEARCH_TYPE_MANUAL      = 1,
    TAF_MMA_SEARCH_TYPE_MANUAL_AUTO = 2,
    TAF_MMA_SEARCH_TYPE_BUTT
};

typedef VOS_UINT32 TAF_MMA_SearchTypeUint32;


typedef struct {
    VOS_MSG_HEADER                    /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32    msgName; /* _H2ASN_Skip */
    /* 0:��ʾ�Զ����� 1:��ʾ�ֶ����� 2:���ֶ��������Զ����� */
    TAF_MMA_SearchTypeUint32 searchType;
    TAF_MMA_Ctrl             ctrl;
    TAF_PLMN_UserSel         plmnUserSel;
} TAF_MMA_PlmnSearchReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_DETECT_Plmn       plmnDetect;
} TAF_MMA_PlmnDetectReqMsg;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  errorCause;
} TAF_MMA_PlmnSearchCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32  msgName;
    TAF_MMA_Ctrl           ctrl;
    TAF_MMA_SetCemodeUint8 ceMode;
    TAF_UINT8              reserved[3];
} TAF_MMA_CemodeSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_CemodeSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_CemodeQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32  msgName;
    TAF_MMA_Ctrl           ctrl;
    TAF_ERROR_CodeUint32   result;
    TAF_MMA_SetCemodeUint8 ceMode;
    TAF_UINT8              reserved[3];
} TAF_MMA_CemodeQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
} TAF_MMA_VoimsEmc380FailNtf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_RejinfoQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_PH_RegRejInfo     phoneRejInfo;
} TAF_MMA_RejinfoQryCnf;

/*
 * ö��˵��: SIMSQ״̬ö�ٱ���
 */
enum TAF_MMA_SimsqState {
    TAF_MMA_SIMSQ_NOT_INSERTED   = 0,
    TAF_MMA_SIMSQ_INSERTED       = 1,
    TAF_MMA_SIMSQ_PIN_PUK        = 2,
    TAF_MMA_SIMSQ_SIMLOCK        = 3,
    TAF_MMA_SIMSQ_INITIALIZING   = 10,
    TAF_MMA_SIMSQ_INITIALIZED    = 11,
    TAF_MMA_SIMSQ_READY          = 12,
    TAF_MMA_SIMSQ_PUKLOCK_DAMAGE = 98,
    TAF_MMA_SIMSQ_REMOVED        = 99,
    TAF_MMA_SIMSQ_INIT_FAIL      = 100,
    TAF_MMA_SIMSQ_BUTT
};
typedef VOS_UINT32 TAF_MMA_SimsqStateUint32;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    VOS_BOOL   smsReady;
} TAF_MMA_SmsStatusNotify;


typedef struct {
    VOS_INT16  lastGsmEmRssi;
    VOS_INT16  lastWcdmaEmRssi;
    VOS_INT16  lastLteEmRssi;
    VOS_UINT16 reserved;
    VOS_UINT8  emRssiCfgGsmThreshold;
    VOS_UINT8  emRssiCfgWcdmaThreshold;
    VOS_UINT8  emRssiCfgLteThreshold;
    VOS_UINT8  emRssiRptSwitch;
} TAF_MMA_Emrssiinfo;


typedef struct {
    VOS_UINT8 emRssiCfgRat;
    VOS_UINT8 emRssiCfgThreshold;
    VOS_UINT8 reserved[2];
} TAF_MMA_EmrssicfgReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_EmrssicfgReq  emRssiCfg;
} TAF_MMA_EmrssicfgSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    VOS_UINT16           clientId;
    VOS_UINT8            opid;
    VOS_UINT8            reserved;
    TAF_ERROR_CodeUint32 errorCause;
} TAF_MMA_EmrssicfgSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EmrssicfgQryReq;


typedef struct {
    VOS_UINT8 emRssiCfgGsmThreshold;
    VOS_UINT8 emRssiCfgWcdmaThreshold;
    VOS_UINT8 emRssiCfgLteThreshold;
    VOS_UINT8 reserved;
} TAF_MMA_Emrssicfg;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    TAF_MMA_Ctrl         ctrl;
    TAF_ERROR_CodeUint32 errorCause;
    TAF_MMA_Emrssicfg    emRssiCfgPara;
} TAF_MMA_EmrssicfgQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             emRssiRptSwitch;
    VOS_UINT8             reserved[3];
} TAF_MMA_EmrssirptSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    VOS_UINT16           clientId;
    VOS_UINT8            opid;
    VOS_UINT8            reserved;
    TAF_ERROR_CodeUint32 errorCause;
} TAF_MMA_EmrssirptSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_EmrssirptQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    TAF_MMA_Ctrl         ctrl;
    TAF_ERROR_CodeUint32 errorCause;
    VOS_UINT8            emRssiRptSwitch;
    VOS_UINT8            reserved[3];
} TAF_MMA_EmrssirptQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT8             clImsSupportFlag;  /* CL��IMS�Ƿ�֧��,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8             clVolteMode;       /* ����Volte����ģʽ 0:�ֶ��� 1:�Զ��� */
    VOS_UINT8             lteImsSupportFlag; /* LTE IMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8             reserved;
} TAF_MMA_ClimsCfgInfoInd;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_UINT8             enableFlag;
    TAF_UINT8             reserved[3];
} TAF_MMA_ClDbdomainStatusSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_ClDbdomainStatusSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    VOS_UINT16            clientId;
    TAF_UINT8             enableReportFlag;
    TAF_UINT8             uc1xSrvStatus;
    TAF_UINT8             imsVoiceCap;
    TAF_UINT8             reserved[3];
} TAF_MMA_ClDbdomainStatusInfoInd;


enum TAF_MMA_SensorStateScene {
    TAF_MMA_SENSOR_STATE_SCENE_ELEVATOR, /* ����״̬�仯�ϱ� */
    TAF_MMA_SENSOR_STATE_SCENE_GARAGE,   /* ����״̬�仯�ϱ� */
    TAF_MMA_SENSOR_STATE_SCENE_BUTT
};
typedef VOS_UINT8 TAF_MMA_SensorStateSceneUint8;


typedef struct {
    VOS_UINT32                    sensorState;
    VOS_UINT8                     serviceState;
    TAF_MMA_SensorStateSceneUint8 sensorScene;
    VOS_UINT8                     reserved[2];
} TAF_MMA_SensorStateReportPara;


typedef struct {
    MSG_Header                    msgHeader; /* _H2ASN_Skip */
    VOS_UINT16                    clientId;
    VOS_UINT8                     opid;
    VOS_UINT8                     resv;
    TAF_MMA_SensorStateReportPara sensorPara;
} TAF_MMA_ElevatorStateInd;


enum TAF_MMA_UlfreqRat {
    TAF_MMA_ULFREQ_RAT_NO_SERVICE = 0,
    TAF_MMA_ULFREQ_RAT_WCDMA      = 1,
    TAF_MMA_ULFREQ_RAT_HRPD       = 2,
    TAF_MMA_ULFREQ_RAT_LTE        = 3,
    TAF_MMA_ULFREQ_RAT_BUTT
};
typedef VOS_UINT8 TAF_MMA_UlfreqRatUint8;


enum TAF_MMA_UlfreqSwitch {
    TAF_MMA_ULFREQRPT_DISABLE = 0, /* ����Ƶ�������ϱ��ر� */
    TAF_MMA_ULFREQRPT_ENABLE  = 1, /* ����Ƶ�������ϱ��� */
    TAF_MMA_ULFREQRPT_BUTT
};
typedef VOS_UINT8 TAF_MMA_UlfreqSwitchUint8;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32     msgName;
    TAF_MMA_Ctrl              ctrl;
    TAF_MMA_UlfreqSwitchUint8 mode;
    VOS_UINT8                 reserved[3];
} TAF_MMA_UlfreqrptSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgName;
    VOS_UINT16           clientId;
    VOS_UINT8            opid;
    VOS_UINT8            reserved;
    TAF_ERROR_CodeUint32 result;
} TAF_MMA_UlfreqrptSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_UlfreqrptQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32     msgName;
    TAF_MMA_Ctrl              ctrl;
    TAF_ERROR_CodeUint32      errorCause;
    VOS_UINT32                ulFreq;    /* ����Ƶ�ʣ���λ100kHz */
    VOS_UINT16                bandwidth; /* ������λ1kHz */
    TAF_MMA_UlfreqRatUint8    rat;       /* ���뼼����0:�޷���1:WCDMA,2:HRPD */
    TAF_MMA_UlfreqSwitchUint8 mode;      /* �����ϱ��Ƿ�ʹ�ܣ�0:�رգ�1:ʹ�� */
} TAF_MMA_UlfreqrptQryCnf;


typedef struct {
    MSG_Header             msgHeader; /* _H2ASN_Skip */
    VOS_UINT16             clientId;
    VOS_UINT8              opid;
    TAF_MMA_UlfreqRatUint8 rat;       /* ���뼼����0:�޷���1:WCDMA,2:HRPD */
    VOS_UINT32             freq;      /* ����Ƶ�ʣ���λ100kHz */
    VOS_UINT16             bandWidth; /* ������λ1kHz */
    VOS_UINT8              reserved[2];
} TAF_MMA_UlFreqInd;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT16 clientId; /* �ͻ���ID */
    VOS_UINT8  opId;     /* ������ID */
    VOS_UINT8  reserved[1];
    VOS_UINT32 ulFreq;      /* ����Ƶ�� */
    VOS_UINT16 ulBandwidth; /* ���д��� */
    VOS_UINT8  resrved1[2];
} TAF_MMA_LteUlFreqChangeInd;


typedef struct {
    VOS_UINT8 psServiceState; /* ����PSҵ��״̬��0:��������PSҵ��;1:��ʼ����PSҵ�� */
    VOS_UINT8 psDetachFlag;   /* �Ƿ���ҪDETACH PS��0������Ҫdetach ps�� */
    VOS_UINT8 reserve[2];
} TAF_MMA_PsScenePara;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_PsScenePara   psStatePara;
} TAF_MMA_PsSceneSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_PsSceneSetCnf;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_PsSceneQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            psSceneState;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_PsSceneQryCnf;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_RrcstatQryReq;


typedef struct {
    TAF_MMA_RatTypeUint8  netRat;
    TAF_MMA_CampStatUint8 tempCampStat;
    TAF_MMA_RrcstatUint8  rrcStat;
    VOS_UINT8             rsv;
} TAF_MMA_RrcCampStat;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  rslt;
    VOS_UINT8             reportFlg;
    VOS_UINT8             rsv[3];
    TAF_MMA_RrcCampStat   rrcCampStat;
} TAF_MMA_RrcstatQryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    TAF_MMA_RrcCampStat   rrcCampStat;
} TAF_MMA_RrcstatInd;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32          msgName;
    TAF_MMA_Ctrl                   ctrl;
    TAF_MMA_AntiAttackSmsTypeUint8 smstype;
    VOS_UINT8                      aucRsv[3];
} TAF_MMA_SmsAntiAttackCapQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32                     msgName;
    TAF_MMA_Ctrl                              ctrl;
    TAF_MMA_SmsAntiAttackSupportResultUint8   result;
    TAF_MMA_SmsAntiAttackNotSupportCauseUint8 cause;
    VOS_UINT8                                 reserve[2];
} TAF_MMA_SmsAntiAttackCapQryCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32           msgName;
    TAF_MMA_Ctrl                    ctrl;
    TAF_MMA_SmsAntiAttackStateUint8 state;
    VOS_UINT8                       reserve[3];
} TAF_MMA_SmsAntiAttackSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_SmsAntiAttackStateSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_SmsNasCapQryReq;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    TAF_MMA_MsgTypeUint32 msgName; /* _H2ASN_Skip */
    TAF_MMA_Ctrl          ctrl;
    VOS_UINT32            smsNasCap;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_SmsNasCapQryCnf;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32                msgName;
    TAF_MMA_Ctrl                         ctrl;
    TAF_MMA_SmsAntiAttackCurrentRatUint8 currentRat;
    VOS_UINT8                            rsv[3];
} TAF_MMA_PseudBtsIdentInd;

typedef struct {
    MSG_Header msgHeader;
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserve;
} TAF_MMA_SmsAntiAttackInd;


#if (FEATURE_ON == FEATURE_UE_MODE_NR)

typedef struct {
    TAF_MMA_NrBandSetOptionUint32 option;
    VOS_UINT32                    bandNum;
    VOS_UINT16                    bandList[TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM];
} TAF_MMA_NrBandBlockListSetPara;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32          msgName;
    TAF_MMA_Ctrl                   ctrl;
    TAF_MMA_NrBandBlockListSetPara nrBandBlocklistPara;
} TAF_MMA_NrBandBlockListSetReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
} TAF_MMA_NrBandBlockListQryReq;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl          ctrl;
    TAF_ERROR_CodeUint32  result;
} TAF_MMA_NrBandBlockListSetCnf;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32              msgName;
    TAF_MMA_Ctrl                       ctrl;
    TAF_ERROR_CodeUint32               result;
    TAF_MMA_NrFr2SupportSwStatusUint32 fr2SupportSw;
    VOS_UINT32                         bandNum;
    VOS_UINT16                         bandList[TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM];
} TAF_MMA_NrBandBlockListQryCnf;
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
/* AT^NETSELOPT��Scene���� */
typedef enum {
    SCENE_TYPE_INTELLIGENT_NET_SEL = 1, /* ����ѡ�� */
    SCENE_TYPE_BUTT
} AtNetSelOptSceneType;

/* ����ѡ����OPERATE���� */
typedef enum {
    INTELLIGENT_NET_SEL_OPERATE_CLEAN = 0, /* ������ʷС���б� */
    INTELLIGENT_NET_SEL_OPERATE_READ, /* ��ȡ��ѡƵ����Ϣ */
    INTELLIGENT_NET_SEL_OPERATE_BUTT,
} IntelligentNetSelOperate;

/* A�˷��͵�NETSELOPT_REQ�������ݽṹ */
typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32   msgName;
    TAF_MMA_Ctrl            ctrl;
    VOS_UINT32              scene; /* �����Ż����� */
    VOS_UINT32              operate; /* �Ż������Ĳ�����ʽ */
} TafMmaNetSelOptSetReq;

/* ����ѡ���ṹ����Ϣ */
typedef struct {
    VOS_UINT8 rat; /* ��ʽ��Ϣ */
    VOS_UINT8 band; /* Ƶ����Ϣ */
    VOS_UINT8 rsv[2]; /* Ԥ��λ */
} TafMmaIntelligentNetInfo;

/* NETSELOPT�ظ���Ϣ������ */
typedef union {
    TafMmaIntelligentNetInfo intelligentNetSel; /* ����ѡ�������ṹ */
} TafMmaNetSelOptInfo;

/* C�˷���NETSELOPT_CNFִ�н�������ݽṹ */
typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32   msgName;
    TAF_MMA_Ctrl            ctrl;
    TAF_ERROR_CodeUint32    errorCause;
    VOS_UINT32              scene; /* �����Ż����� */
    VOS_UINT32              operate; /* �Ż������Ĳ�����ʽ */
    TafMmaNetSelOptInfo   netInfo; /* ��Ϣ�ظ��������� */
} TafMmaNetSelOptSetCnf;

typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName;
    TAF_MMA_Ctrl ctrl;
    VOS_UINT32 scene; /* �����Ż����� */
    VOS_UINT32 operate; /* �Ż������Ĳ�����ʽ */
    TafMmaNetSelOptInfo netInfo; /* ��Ϣ�ظ��������� */
} TafMmaNetSelOptSetInd;
#endif


typedef struct {
    VOS_UINT16 totalTimerLen; /* ���׶�������ʱ��,��λ:s */
    VOS_UINT16 sleepTimerLen; /* ���������˯��ʱ��,��λ:s */
    VOS_UINT8  historyNum;    /* ��ʷ�Ѵ��� */
    VOS_UINT8  prefBandNum;   /* PrefBand�������� */
    VOS_UINT8  fullBandNum;   /* FullBand�����ܴ��� */
    VOS_UINT8  reserve1;
} TAF_MMA_StrategyCfg;

/*
 * �ṹ˵��: AT����OOS������Ϣ�ṹ
 */
typedef struct {
    TAF_MMA_StrategySceneUint8  stragetyScene;
    VOS_UINT8                   rsv[3];
    TAF_MMA_StrategyCfg         strategyCfg[TAF_MMA_OOS_PHASE_NUM];
} TAF_MMA_OosSrchStrategyInfo;

typedef struct {
    TAF_MMA_StrategyModeUint8       strategyMode;
    VOS_UINT8                       strategyNum;
    VOS_UINT8                       rsv[2];
    TAF_MMA_OosSrchStrategyInfo     strategyInfo[TAF_MMA_SET_OOS_SCENE_MAX_NUM];
} TAF_MMA_SetOosSrchStrategyPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                      msgName;
    TAF_MMA_Ctrl                    ctrl;
    TAF_MMA_SetOosSrchStrategyPara  setPara;
} TAF_MMA_SetOosSrchStrategyInd;


#if (FEATURE_MBB_CUST == FEATURE_ON)
/* ��Ӧ�����USC2��󳤶� */
#define TAF_PH_OPER_PNN_USC2_CODE_LEN 128
/* PLMN ID���볤�� */
#define TAF_PLMN_ID_LEN_5 5
#define TAF_PLMN_ID_LEN_6 6
/* AT^EONS=5ʱPLMN NAMEĬ�ϵ���󳤶� */
#define TAF_EONS_PLMN_NAME_DEFAULT_LEN 20
/* �ӱ����б��л�ȡ����Ӫ�����Ƴ��� */
#define TAF_PH_LOCAL_NETWORK_NAME_LEN 32
#define TAF_PLMN_MCC_LEN 3 /* PLMN�����볤�� */

/* EONS���뷽ʽö�� */
typedef enum {
    TAF_EONS_GSM_7BIT_DEFAULT = 0, /* GSM 7Bit */
    TAF_EONS_GSM_7BIT_PACK, /* ѹ��7Bit */
    TAF_EONS_UCS2_COMM, /* UCS2 �������� */
    TAF_EONS_UCS2_80, /* UCS2 80���� */
    TAF_EONS_UCS2_81, /* UCS2 81���� */
    TAF_EONS_UCS2_82, /* UCS2 82���� */
    TAF_EONS_ASCII, /* ASCII */
    TAF_EONS_HEX_ASCII, /* 16����ASCII */
    TAF_EONS_CODE_BUTT,
} TafEonsCodeType;

/* EONS���÷�ʽö�� */
typedef enum {
    TAF_EONS_TYPE_AUTO = 1, /* AUTOģʽ */
    TAF_EONS_TYPE_MM_INFO, /* �����·������� */
    TAF_EONS_TYPE_PNN, /* SIM������ */
    TAF_EONS_TYPE_ME, /* �ն�SE13�б� */
    TAF_EONS_TYPE_SPDI, /* ����b1/b2��SPDIָʾ�������� */
    TAF_EONS_TYPE_BUTT,
} TafEonsSetType;

/* EONS���뷽ʽ���� */
typedef enum {
    TAF_EONS_CODE_UCS2_80 = 0x80, /* UCS2 80���� */
    TAF_EONS_CODE_UCS2_81 = 0x81, /* UCS2 81���� */
    TAF_EONS_CODE_UCS2_82 = 0x82, /* UCS2 82���� */
} TafEonsUnicodeType;

/* ��������Ϣ�ṹ�� */
typedef struct {
    VOS_UINT8 nameLen; /* ���� */
    VOS_UINT8 nameCode; /* ���� */
    VOS_UINT8 name[TAF_PH_OPER_PNN_USC2_CODE_LEN + 1]; /* �����ַ��� */
    VOS_UINT8 rsv;
} TafMmaNameInfo;

/* EONS�����ȡ��������Ϣ�ṹ�� */
typedef struct {
    TafMmaNameInfo longName; /* ���� */
    TafMmaNameInfo shortName; /* ���� */
} TafMmaEonsNameInfo;

/* EONS�����ȡSPN��Ϣ�ṹ�� */
typedef struct {
    VOS_UINT8 dispMode; /* SPN��ʾģʽ */
    VOS_UINT8 spnLen; /* SPN���� */
    VOS_UINT8 spnCode; /* SPN���뷽ʽ */
    VOS_UINT8 spn[TAF_PH_SPN_NAME_MAXLEN + 1]; /* SPN���� */
} TafMmaEonsSpnInfo;

/* EONS�����ȡ�������ƵĽṹ�� */
typedef struct {
    TafEonsSetType eonsType; /* EONS C�˻�ȡ������������ */
    VOS_UINT8 serviceStatus; /* EONS C�˻�ȡ���ķ���״̬��Ϣ */
    VOS_UINT8 plmnLen; /* EONS C�˷��ظ�A�˵�plmn���� */
    VOS_UINT8 rsv[2]; /* ���� */
    TAF_PLMN_Id plmn; /* EONS C�˷��ظ�A�˵�plmn���� */
    TafMmaEonsSpnInfo spnInfo; /* SPN��Ϣ */
    TafMmaEonsNameInfo nameInfo; /* �������� */
} TafMmaEonsResponsePara;

/* EONS�����A���������ƵĽṹ�� */
typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName; /* ��Ϣ���� */
    TAF_MMA_Ctrl ctrlInfo; /* ��Ϣ���ݿ�����Ϣ */
    TafMmaEonsResponsePara eonsResponsePara; /* EONS��� */
} TafMmaEonsResponse;

/* EONS�������õĵڶ���������Ϣ */
typedef struct {
    VOS_UINT8 plmnLen; /* AT����ĵڶ��������ĳ��� */
    VOS_CHAR plmnId[TAF_PLMN_ID_LEN_6 + 1]; /* AT��εڶ������� */
} TafMmaEonsPlmnInfo;

/* EONS��������ṹ�� */
typedef struct {
    TafEonsSetType eonsType; /* AT��ε�һ������ */
    VOS_UINT8 maxNwNameLen; /* AT��ε��������� */
    VOS_UINT8 rsv[3]; /* ����λ */
    TafMmaEonsPlmnInfo eonsPlmnInfo; /* AT��εڶ������� */
} TafMmaEonsRequestPara;

/* EONS�����C�˻�ȡ��Ӫ�����Ƶ����ݽṹ */
typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32 msgName; /* ��Ϣ���� */
    TAF_MMA_Ctrl ctrlInfo; /* ��Ϣ���ݿ�����Ϣ */
    TafMmaEonsRequestPara eonsRequestPara; /* EONS�������� */
} TafMmaEonsRequest;

/* �յ�MM INFO��Ӫ�����Ƹ��µ�֪ͨ */
typedef struct {
    MSG_Header msgHeader; /* ��Ϣͷ */
    VOS_UINT16 clientId; /* Client ID */
    VOS_UINT8 opId; /* ����ID */
    VOS_UINT8 changeFlg; /* �仯��ʶ */
} TafMmaNwNameChangeInd;

/* HCSQ�����C�˻�ȡ�����ڵ���ź���� */
typedef struct {
    VOS_MSG_HEADER /* ��Ϣͷ */
    TAF_MMA_MsgTypeUint32 msgName; /* ��Ϣ���� */
    TAF_MMA_Ctrl ctrlInfo; /* ������Ϣ */
} TafMmaHcsqQryRequest;

/* HCSQ�źŽ�� */
typedef struct {
    VOS_UINT32 mrdcFlag; /* ��ʶ�Ƿ�ΪMRDC״̬ */
    TAF_PH_Rssi mnSignal; /* MN���ź���� */
    TAF_PH_Rssi snSignal; /* SN���ź���� */
} TafMmaHcsqQryResponsePara;

/* C�˷��ص�HCSQ����Ľ�� */
typedef struct {
    VOS_MSG_HEADER /* ��Ϣͷ */
    TAF_MMA_MsgTypeUint32 msgName; /* ��Ϣ���� */
    TAF_MMA_Ctrl ctrlInfo; /* ������Ϣ */
    TafMmaHcsqQryResponsePara hcsqResponsePara; /* ���ص�С���źŽ�� */
} TafMmaHcsqQryResponse;

#define TAF_SUBNET_ADAPTER_IMSI_MAX_LEN     8
#define TAF_SUBNET_ADAPTER_ICCID_MAX_LEN    10


typedef struct {
    VOS_UINT8 status;                        /* sim��״̬ */
    VOS_UINT8 imsiLen;                       /* IMSI���ݳ��� */
    VOS_UINT8 iccidLen;                      /* ICCID���ݳ��� */
    VOS_UINT8 reserved[1];
    VOS_UINT8 imsi[TAF_SUBNET_ADAPTER_IMSI_MAX_LEN];    /* IMSI */
    VOS_UINT8 iccid[TAF_SUBNET_ADAPTER_ICCID_MAX_LEN];  /* ICCID */
    VOS_UINT8 reserved1[2];
}TAF_MMA_SubnetAdapterCardInfo;


typedef struct {
    VOS_MSG_HEADER
    TAF_MMA_MsgTypeUint32         msgName;
    VOS_UINT16                    clientId;
    VOS_UINT8                     opId;
    VOS_UINT8                     reserved;
    TAF_MMA_SubnetAdapterCardInfo cardInfo;
} TAF_MMA_SubnetAdapterInfoInd;
#endif

TAF_UINT32 Taf_DefPhClassType(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_PH_MS_CLASS_TYPE msClass);

/* Taf_PhoneAttach */
TAF_UINT32 Taf_PhonePinHandle(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_PH_PinData *pinData);
TAF_UINT32 Taf_PhonePlmnList(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                             TAF_MMA_PlmnListPara *plmnListPara);

VOS_UINT32 TAF_MMA_SetQuickStartReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 setValue);
VOS_UINT32 TAF_MMA_QryQuickStartReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 Taf_SetCopsFormatTypeReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                    TAF_MMA_CopsFormatTypeSetReq *copsFormatType);

VOS_UINT32 TAF_SetAutoReselStub(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                TAF_MMA_AutoReselStubSetReq *autoReselStub);
TAF_UINT32 TAF_DefLoadDefault(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
VOS_BOOL   Taf_IsWBandSupported(VOS_UINT16 bandNo);

VOS_VOID Taf_GetSyscfgBandGroupStr(TAF_PH_SysCfgBand *sysCfgBandStr);

/* MN_MMA_GetSyscfgExLteBandGroupStr */

/* �޵��õ㣬��ʵ����Ŀɾ�� */

/* �޵��õ㣬��ʵ����Ŀɾ�� */

TAF_UINT32 Taf_PhoneGetNetworkNameForListPlmn(TAF_PH_OperatorNameFormatOp *operName);

VOS_UINT32 Taf_MePersonalisationHandle(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                       TAF_ME_PersonalisationData *mePersonalData);

TAF_UINT32 TAF_SetPrefPlmnType(MN_PH_PrefPlmnTypeUint8 prefPlmnType);

TAF_UINT32 TAF_QryUsimInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_PH_QryUsimInfo *info);

/* ��Nasrrcinterface.h�������ķ���ֵ����һ�£�����WAS��DMT������ʧ�� */
VOS_UINT32 TAF_IsNormalSrvStatus(VOS_VOID);

TAF_UINT32 TAF_QryCpnnInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_PH_IccType iccType);

VOS_UINT32 MN_PH_RptCostTime(VOS_VOID);

VOS_UINT8 APP_MN_PH_AppQryPhMode(VOS_VOID);

VOS_UINT8 MN_MMA_GetServiceDomain(VOS_VOID);

TAF_UINT32 Taf_DefMmTestPara(TAF_VOID *input, TAF_VOID *rslt);

VOS_VOID Taf_GetCurrentAttachStatus(TAF_MMA_AttachStatusUint8 *csResult, TAF_MMA_AttachStatusUint8 *psResult);

TAF_UINT32 Taf_ParaQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_PARA_TYPE paraType, TAF_VOID *para);

VOS_UINT32 TAF_MMA_PhoneModeSetReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   TAF_MMA_PhoneModePara *phoneModePara);

VOS_UINT32 TAF_MMA_QryAutoAttachInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryPhoneModeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#if (FEATURE_ON == FEATURE_CSG)
VOS_UINT32 TAF_MMA_CsgListSearchReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                    TAF_MMA_PlmnListPara *plmnListPara);
VOS_UINT32 TAF_MMA_AbortCsgListSearchReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetCsgIdSearch(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                  TAF_MMA_CsgSpecSearchInfo *userSelCsgId);
VOS_UINT32 TAF_MMA_QryCampCsgIdInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#endif

VOS_UINT32 TAF_MMA_SetSysCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                TAF_MMA_SysCfgPara *sysCfgPara);

VOS_UINT32 TAF_MMA_QryEonsUcs2Req(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_DetachReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, TAF_MMA_DetachPara *detachPara);

VOS_UINT32 TAF_MMA_AttachReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                             TAF_MMA_AttachTypeUint8 attachType);
VOS_UINT32 TAF_MMA_QrySmsAntiAttackCapReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                          VOS_UINT8 antiSmsType);
VOS_UINT32 TAF_MMA_SetSmsAntiAttackState(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                         TAF_MMA_SmsAntiAttackStateUint8 state);
VOS_UINT32 TAF_MMA_QrySmsNasCapReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_UINT32 TAF_MMA_AttachStatusQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                      TAF_MMA_ServiceDomainUint8 domainType);

VOS_VOID TAF_MMA_SrvAcqReq(TAF_MMA_SrvTypeUint8 srvType, TAF_MMA_SrvAcqRatList *ratList, VOS_UINT32 moduleId);

VOS_VOID TAF_MMA_SrvAcqStop(VOS_UINT32 moduleId);
VOS_VOID TAF_MMA_SndStopSrvAcqReq(VOS_UINT32 moduleId);
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_UINT32 TAF_MMA_ProcCFreqLockSetReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                       TAF_MMA_CfreqLockSetPara *cFreqLockPara);

VOS_UINT32 TAF_MMA_ProcCdmaCsqSetReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                     TAF_MMA_CdmacsqPara *cdmaCsqPara);

VOS_UINT32 TAF_MMA_ProcCdmaCsqQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_ProcCFreqLockQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_Proc1xChanSetReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                    TAF_MMA_CfreqLockSetPara *cFreqLockPara);

VOS_UINT32 TAF_MMA_Proc1xChanQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_ProcProRevInUseQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_ProcStateQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_ProcCHVerQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 TAF_MMA_SetCSidList(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                               TAF_MMA_OperLockTrustSid *trustSidList);
#endif
/* �Ƴ�CDMA���뿪�� */

VOS_UINT32 TAF_MMA_ProcHdrCsqSetReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                    TAF_MMA_HdrCsqPara *hdrCsqPara);

VOS_UINT32 TAF_MMA_ProcHdrCsqQryReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCurrSidNid(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCtRoamInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetCtOosCount(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 clOosCount,
                                 VOS_UINT32 gulOosCount);

VOS_UINT32 TAF_MMA_SetCtRoamInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT8 ctRoamRtpFlag);

VOS_UINT32 TAF_MMA_QryPrlIdInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_ProcResetNtf(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT8 resetStep);

VOS_UINT32 TAF_MMA_QryRatCombinedMode(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCLocInfoReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

#endif

VOS_UINT32 TAF_MMA_SetQuitCallBack(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCurrEmcCallBackMode(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

extern VOS_UINT32 TAF_MMA_SetFPlmnInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                       TAF_PH_FplmnOperate *cFPlmnPara);
extern VOS_UINT32 TAF_MMA_QryFPlmnInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetCpolReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, TAF_PH_SetPrefplmn *prefPlmn);

VOS_UINT32 TAF_MMA_QueryCpolReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                TAF_MMA_CpolInfoQueryReq *cpolInfo);

VOS_UINT32 TAF_MMA_TestCpolReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                               MN_PH_PrefPlmnTypeUint8 prefPlmnType);

VOS_UINT32 TAF_MMA_SetAutoAttachReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 setValue);

extern VOS_UINT32 TAF_MMA_AbortPlmnListReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

extern VOS_UINT32 TAF_MMA_QryLocInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

extern VOS_UINT32 TAF_MMA_QryCipherReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

extern VOS_UINT32 TAF_MMA_SetPrefPlmnTypeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                             const MN_PH_PrefPlmnTypeUint8 *prefPlmnType);

VOS_UINT32 TAF_MMA_SetCerssiReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                TAF_START_InfoInd *startInfoInd);

VOS_UINT32 TAF_MMA_QryCerssiReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
VOS_UINT32 TAF_MMA_QryCserssiReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#endif

VOS_UINT32 TAF_MMA_QryAccessModeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCopsInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetEflociInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                 TAF_MMA_Eflociinfo *efLociInfo);

VOS_UINT32 TAF_MMA_QryEflociInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetPsEflociInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   TAF_MMA_Efpslociinfo *psefLociInfo);

VOS_UINT32 TAF_MMA_QryPsEflociInfo(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryRegStateReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                  TAF_MMA_QryRegStatusTypeUint32 regStaType);

#if (FEATURE_ON == FEATURE_DSDS)
VOS_UINT32 TAF_MMA_SetDsdsStateReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT32 enable);
#endif

VOS_UINT32 TAF_MMA_SetDplmnListReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 seq, const VOS_UINT8 *version,
                                   TAF_MMA_DplmnInfoSet *dplmnInfo);

VOS_UINT32 TAF_MMA_QryDplmnListReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
VOS_UINT32 TAF_MMA_ExchangeModemInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, ModemIdUint16 firstModemId,
                                        ModemIdUint16 secondModemId);
#endif

VOS_UINT32 TAF_MMA_SetBorderInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId,
                                    TAF_MMA_SetBorderInfoOperateTypeUint8 operateType, TAF_MMA_BorderInfo *borderInfo);
VOS_UINT32 TAF_MMA_QryBorderInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QrySystemInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 sysInfoExFlag);

VOS_UINT32 TAF_MMA_QryAntennaInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryApHplmnInfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCsnrReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryCsqReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_UINT32 TAF_MMA_QryCsqlvlReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryBatteryCapacityReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_UINT32 TAF_MMA_QryHandShakeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryAcInfoReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryCopnInfoReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, VOS_UINT16 fromIndex,
                                  MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_SetEOPlmnReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                TAF_MMA_SetEoplmnList *eOPlmnCfg);

VOS_UINT32 TAF_MMA_QryEOPlmnReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_NetScanReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                              TAF_MMA_NetScanSetPara *netScanSetPara);

VOS_UINT32 TAF_MMA_NetScanAbortReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QrySpnReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryMMPlmnInfoReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryPlmnReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryUserSrvStateReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryApPwrOnAndRegTimeReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

VOS_UINT32 TAF_MMA_QryCrpnReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                              TAF_MMA_CrpnQryPara *crpnQryReq);

VOS_UINT32 TAF_MMA_SetCmmReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, MM_TEST_AtCmdPara *testAtCmd);

VOS_UINT32 TAF_MMA_TestSysCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

extern VOS_UINT32 TAF_MMA_QrySyscfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32 TAF_MMA_SetImsSwitchReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   TAF_MMA_ImsSwitchInfo *imsSwitchInfo);

VOS_UINT32 TAF_MMA_QryImsSwitchReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryRcsSwitchReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetVoiceDomainReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                     TAF_MMA_VoiceDomainUint32 voiceDomain);

VOS_UINT32 TAF_MMA_QryVoiceDomainReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_UINT32 TAF_MMA_SetRoamImsSupportReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                        TAF_MMA_RoamImsSupportUint32 roamImsSupport);
#endif

#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32 TAF_MMA_SetImsDomainCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                      TAF_MMA_ImsDomainCfgTypeUint32 imsDomainCfg);

VOS_UINT32 TAF_MMA_QryImsDomainCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_VOID TAF_MMA_ImsRegDomainNotify(TAF_MMA_ImsRegDomainUint8 imsRegDomain, TAF_MMA_ImsRegStatusUint8 imsRegStatus);
VOS_UINT32 TAF_MMA_SetImsVtCapCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                     TAF_MMA_ImsVideoCallCap *imsVtCap);
VOS_UINT32 TAF_MMA_SetImsSmsCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   TAF_MMA_ImsSmsCfg *imsSmsCfg);

VOS_UINT32 TAF_MMA_QryImsSmsCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_VOID TAF_MMA_VoimsEmc380FailNotify(VOS_VOID);
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
VOS_UINT32 TAF_MMA_SetClDbDomainStatus(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT8 mode);
#endif

VOS_UINT32 TAF_MMA_QryPacspReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_VOID TAF_MMA_SendSmcNoEntityNtf(VOS_VOID);
VOS_VOID TAF_MMA_CardQuickSwitchNotify(VOS_UINT32 moduleId, VOS_UINT16 clientId,
    VOS_UINT8 opId, TAF_MMA_IsSamePlmnFlagUint8 isSamePlmnInfo);

VOS_UINT32 TAF_MMA_AcdcAppNotify(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                 TAF_MMA_AcdcAppInfo *acdcAppInfo);

/* �ýӿ��Ǹ���2���õģ�luxiaotong���𿪷���2�Ĵ��� */
VOS_VOID TAF_MMA_SndRestartReq(VOS_UINT32 moduleId, ModemIdUint16 modemId);

VOS_UINT32 TAF_MMA_SetCindReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                              TAF_START_InfoInd *startInfoInd);

VOS_UINT32 TAF_MMA_PlmnSearchReq(TAF_MMA_Ctrl *ctrl, TAF_MMA_SearchTypeUint32 searchType,
                                 TAF_PLMN_UserSel *plmnUserSel);

VOS_UINT32 TAF_MMA_PlmnDetectReq(TAF_MMA_Ctrl *ctrl, TAF_DETECT_Plmn *plmnDetect);

VOS_UINT32 TAF_MMA_SetCemodeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 setValue);
VOS_UINT32 TAF_MMA_QryCemodeReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_QryRejinfoReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetEmRssiCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   TAF_MMA_EmrssicfgReq *emRssiCfgPara);
VOS_UINT32 TAF_MMA_QryEmRssiCfgReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
VOS_UINT32 TAF_MMA_SetEmRssiRptReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                   const VOS_UINT8 *emRssiRptSwitch);
VOS_UINT32 TAF_MMA_QryEmRssiRptReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 TAF_MMA_SetUlFreqRptReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT8 mode);

VOS_UINT32 TAF_MMA_QryUlFreqRptReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 TAF_SetUsimStub(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_MMA_UsimStubSetReq *usimStub);

VOS_UINT32 TAF_SetRefreshStub(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, TAF_MMA_RefreshStubSetReq *refreshStub);

VOS_UINT32 TAF_MMA_SimInsertReq(VOS_UINT32 moduleId, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                TAF_SIM_InsertStateUint32 simInsertState);

extern VOS_UINT32 TAF_MMA_MtPowerDownReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#endif

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
VOS_UINT32 TAF_MMA_SetPsSceneReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                 TAF_MMA_PsScenePara *psSrvStatePara);
VOS_UINT32 TAF_MMA_QryPsSceneReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#endif

VOS_UINT32 TAF_MMA_QryRrcStatReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 TAF_MMA_SetNrBandBlockListReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId,
                                         TAF_MMA_NrBandBlockListSetPara *nrBandBlockListSetStru);

VOS_UINT32 TAF_MMA_QryNrBandBlockListReq(VOS_UINT32 moduleId, VOS_UINT16 clientId, VOS_UINT8 opId);
#endif

VOS_UINT32 TAF_MMA_SndOosSrchStrategySetInd(TAF_MMA_Ctrl *ctrl,
    TAF_MMA_SetOosSrchStrategyPara *srchStrategy);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of taf_app_mma.h */
