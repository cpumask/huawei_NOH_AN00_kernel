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

/*
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2012-2019
 * ��������: PIH�¼���ͷ�ļ�
 * ��������: 2008��10��13��
 */

#ifndef __SI_APP_PIH_H__
#define __SI_APP_PIH_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "vos.h"
#include "product_config.h"
#include "usimm_ps_interface.h"
#include "si_typedef.h"

#if (OSA_CPU_NRCPU != VOS_OSA_CPU)
#include "mn_client.h"
#endif

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#include "ccore_nv_stru_pam.h"
#endif
#include "nv_stru_pam.h"

#define SI_PIH_POLL_TIMER_LEN 28000
#define SI_PIH_IMSI_TIMER_LEN 5000
#define SI_PIH_CALL_TIMER_LEN 4000
#define SI_PIH_TIMER_LEN_NO_CHANGE 0

#define SI_PH_PINCODELENMAX 8
#define SI_MAX_IMSI_LEN 9

#define SI_MAX_PLMN_ID_LEN 3 /* PLMN ID���� */
#define SI_MAX_PLMN_ID_NUM 4 /* ����ʱ֧�ֵ�PLMN ID�� */

#define SI_APDU_MAX_LEN 260 /* APDU���ݵ���󳤶� */

#define SI_PRIVATECGLA_APDU_MAX_LEN (256 * 3) /* APDU���ݵ���󳤶� */

#define SI_ATR_MAX_LEN 80 /* ATR����󳤶� */

#define SI_PIH_VSIMAPN_MAX 100 /* VSIM APN ����󳤶ȣ�����TAF���������� */

typedef VOS_UINT32 SI_PIH_ERROR;

#define SI_RSFW_MAX_INDEX 255    /* ֧��RSFWX�ְ��������� */
#define SI_PIH_HPLMN_MAX_NUM 32  /* ֧��HPLMN/EHPLMN�������� */
#define SI_PIH_EHPLMN_BYTE_NUM 3 /* EHPLMN�ļ���ÿ��EHPLMN��Ԫռ��3���ֽ� */

#define SI_AUTH_DATA_MAX 256 /* ֧�ּ�Ȩ���ݵ���󳤶� */

#define SI_IMSI_MAX_LEN (15 + 1) /* IMSI ת���ɿɼ��ַ����� */

#define SI_PIH_KEYFILE_MAX_NUM 2 /* ���֧��2���ؼ��ļ��� */

#define SI_PIH_KEYFILE_SEND_PARA 0xABABAB /* �ؼ��ļ������ļ�send para */

#define SI_PIH_KEYFILE_INIT_TIME_LEN 5 /* �ؼ��ļ�����ʼ��ʱ��ʱ������μ��ʧ�ܺ󣬶�ʱ������ */

#define SI_PIH_KEYFILE_INIT_CHECK_TIMER 5 /* �ؼ��ļ����ճ�ʼ��ʱ��ʱ���������� */

#define SI_PIH_KEYFILE_TIME_LEN 60 /* �ؼ��ļ����������ʱ��ʱ�� */

#define SI_CRYPTO_CBC_PIN_LEN 16 /* ��������ĳ���AES_CCB���� */

#define SI_SIGNATURE_LEN 32 /* ǩ������ */

#define SI_PIH_SEND_PAPA(clientId, eventType) (((clientId)) | (((eventType)) << 16))
#define SI_PIH_CLIENT_ID(para) (((para)) & 0xFFFF)
#define SI_PIH_EVENT_TYPE(para) ((((para)) & 0xFFFF0000) >> 16)

enum SI_PIH_PinType {
    SI_PIN = 0,       /* PIN1 */
    SI_PUK = 1,       /* PUK1 */
    SI_PIN2 = 6,      /* PIN2 */
    SI_PUK2 = 7,      /* PUK2 */
    SI_PHNET_PIN = 8, /* PH-NET PIN */
    SI_PHSIM_PIN = 9, /* PH-SIM PIN */
    SI_SIM_NON = 255, /* ��ȷ������PIN������ʱ�����ֵ */
    SI_PIN_BUTT
};
typedef VOS_UINT32 SI_PIH_PinTypeUint32;

enum SI_PIH_RestricCmd {
    SI_PIH_READ_BINARY = 176,
    SI_PIH_READ_RECORD = 178,
    SI_PIH_UPDATE_BINARY = 214,
    SI_PIH_UPDATE_RECORD = 220,
    SI_PIH_STATUS = 242,
    SI_PIH_CMD_BUTT
};

typedef VOS_UINT32 SI_PIH_RestricCmdUint32;

enum SI_PIH_Event {
    SI_PIH_EVENT_PIN_OPER_CNF = 1,       /* ����PIN�뷵�ؽ��        */
    SI_PIH_EVENT_PIN_QUERY_CNF = 2,      /* ��ѯPIN�뷵�ؽ��        */
    SI_PIH_EVENT_PIN_INFO_IND = 3,       /* ����PIN�������ϱ�        */
    SI_PIH_EVENT_SIM_INFO_IND = 4,       /* SIM����Ϣ�ϱ�            */
    SI_PIH_EVENT_GENERIC_ACCESS_CNF = 5, /* +CSIM �ظ�               */
    SI_PIH_EVENT_RESTRIC_ACCESS_CNF = 6, /* +CRSM �ظ�               */
    SI_PIH_EVENT_FDN_CNF = 7,            /* FDN�ظ�                  */
    SI_PIH_EVENT_BDN_CNF = 8,            /* BDN�ظ�                  */
    SI_PIH_EVENT_PERO_LOCK_CNF = 9,      /* �����ظ�                 */
    SI_PIH_EVENT_ISDB_ACCESS_CNF = 10,   /* ISDB͸���ظ�             */
    SI_PIH_EVENT_HVSST_QUERY_CNF = 11,   /* ^HVSST��ѯ�����       */
    SI_PIH_EVENT_HVSST_SET_CNF = 12,     /* ^HVSST���������       */
    SI_PIH_EVENT_HVSDH_SET_CNF = 13,     /* ^HVSDH���������       */
    SI_PIH_EVENT_HVSDH_QRY_CNF = 14,     /* ^HVSDH��ѯ�����       */
    SI_PIH_EVENT_HVSCONT_QUERY_CNF = 15, /* ^HVSCONT��ѯ�����     */
    SI_PIH_EVENT_FILE_WRITE_CNF = 16,    /* ^RSFW���������        */
    SI_PIH_EVENT_CCHO_SET_CNF = 17,      /* +CCHO���������        */
    SI_PIH_EVENT_CCHC_SET_CNF = 18,      /* +CCHC���������        */
    SI_PIH_EVENT_CGLA_SET_CNF = 19,      /* +CGLA���������        */
    SI_PIH_EVENT_CARD_ATR_QRY_CNF = 20,  /* ^CARDATR��ѯ�����     */
    SI_PIH_EVENT_HVRDH_IND = 21,         /* ^HVRDHָʾ�ϱ�           */
    SI_PIH_EVENT_UICCAUTH_CNF = 22,      /* ^UICCAUTH���������    */
    SI_PIH_EVENT_URSM_CNF = 23,          /* ^URSM���������        */
    SI_PIH_EVENT_CARDTYPE_QUERY_CNF = 24,
    SI_PIH_EVENT_CRSM_SET_CNF = 25,
    SI_PIH_EVENT_CRLA_SET_CNF = 26,
    SI_PIH_EVENT_SESSION_QRY_CNF = 27,

    SI_PIH_EVENT_SCICFG_SET_CNF = 28,
    SI_PIH_EVENT_SCICFG_QUERY_CNF = 29,
    SI_PIH_EVENT_HVTEE_SET_CNF = 30,
    SI_PIH_EVENT_TEETIMEOUT_IND = 31,
    SI_PIH_EVENT_HVCHECKCARD_CNF = 32,
    SI_PIH_EVENT_CIMI_QRY_CNF = 33,
    SI_PIH_EVENT_CCIMI_QRY_CNF = 34,
    SI_PIH_EVENT_SIM_ERROR_IND = 35, /* SIM��Error��Ϣ�ϱ�       */

    SI_PIH_EVENT_SIM_ICCID_IND = 36,
    SI_PIH_CALL_BACK_TYPE_ICC_SEC_CHANNEL = 37, /* icc sec ch call back */

    SI_PIH_EVENT_SIM_HOTPLUG_IND = 38,

    SI_PIH_EVENT_CCHP_SET_CNF = 39, /* +CCHP��������� */

    SI_PIH_EVENT_SW_CHECK_IND = 40, /* Apdu SW check */

    SI_PIH_EVENT_CARDVOLTAGE_QUERY_CNF = 41,
    SI_PIH_EVENT_PRIVATECGLA_SET_IND = 42, /* ^CGLA���������ϱ�        */
    SI_PIH_EVENT_PRIVATECGLA_SET_CNF = 43, /* ^CGLA���������        */
    SI_PIH_EVENT_CARDTYPEEX_QUERY_CNF = 44,
#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
    SI_PIH_EVENT_SILENT_PIN_SET_CNF = 45,
    SI_PIH_EVENT_SILENT_PININFO_SET_CNF = 46,
#endif
    SI_PIH_EVENT_IMSI_POLLING_SET_CNF = 47, /* PIH������IMSI��ѯʹ�ã�Ϊ�˸�ʽͳһ */

    SI_PIH_EVENT_CARDSTATUS_IND = 48,

    SI_PIH_EVENT_ESIMSWITCH_SET_CNF = 49,
    SI_PIH_EVENT_ESIMSWITCH_QRY_CNF = 50,
    SI_PIH_EVENT_BWT_SET_CNF = 51,
    SI_PIH_EVENT_PRIVATECCHO_SET_CNF = 52,
    SI_PIH_EVENT_PRIVATECCHP_SET_CNF = 53,

    SI_PIH_EVENT_UPDATE_FILE_AT_IND = 54, /* ����EFLOCI EFPSLOCI EFEPSLOCI�ļ�������������֪ͨAP */

    SI_PIH_EVENT_PASSTHROUGH_SET_CNF = 55,
    SI_PIH_EVENT_PASSTHROUGH_QUERY_CNF = 56,
#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
    SI_PIH_EVENT_SINGLEMODEMDUALSLOT_SET_CNF = 57,
    SI_PIH_EVENT_SINGLEMODEMDUALSLOT_QUERY_CNF = 58,
#endif

    /* ��500��ʼ��ΪPIH�ڲ���ά�ɲ���Ϣ�Ĺ��� */
    SI_PIH_USIMREG_PID_HOOK = 500,
    SI_PIH_REFRESHREG_PID_HOOK = 501,
    SI_PIH_ISIMREG_PID_HOOK = 502,
    SI_PIH_HVTEE_DATA_HOOK = 503,
#if (FEATURE_VSIM == FEATURE_ON)
    SI_PIH_VSIM_APN_DATA_HOOK = 504,
    SI_PIH_VSIM_FILE_DATA_HOOK = 505,
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
    SI_PIH_EVENT_SIM_REFRESH_IND = 506,
    SI_PIH_EVENT_NOCARD_SET_CNF = 507,
#endif
#endif

    SI_PIH_EVENT_BUTT
};
typedef VOS_UINT32 SI_PIH_EventUint32;

enum SI_PIH_PollTimerState {
    SI_PIH_POLLTIMER_DISABLE = 0,
    SI_PIH_POLLTIMER_ENABLE = 1,
    SI_PIH_POLLTIMER_BUTT
};
typedef VOS_UINT32 SI_PIH_PollTimerStateUint32;

enum SI_PIH_TimerName {
    SI_PIH_TIMER_NAME_CHECKSTATUS = 0,
    SI_PIH_TIMER_NAME_CALL = 1,
    SI_PIH_TIMER_NAME_CHECKIMSI = 2,
    SI_PIH_TIMER_NAME_CHECK_KEYFILE = 3,

    SI_PIH_TIMER_NAME_BUTT
};
typedef VOS_UINT32 SI_PIH_TimerNameUint32;

enum SI_PIH_QueryType {
    SI_PIH_BDN_QUERY = 0,
    SI_PIH_FDN_QUERY = 1,
    SI_PIH_QUERY_BUTT
};
typedef VOS_UINT32 SI_PIH_QueryTypeUint32;

enum SI_PIH_FdnBdnState {
    SI_PIH_STATE_FDN_BDN_DISABLE = 0, /* FDN/BDN����δʹ�� */
    SI_PIH_STATE_FDN_BDN_ENABLE = 1,  /* FDN/BDN����ʹ�� */
    SI_PIH_STATE_FDN_BDN_BUTT
};
typedef VOS_UINT32 SI_PIH_FdnBdnStateUint32;

enum SI_PIH_FdnBdnCmd {
    SI_PIH_FDN_BDN_DISABLE = 0, /* ȥ����FDN/BDN���� */
    SI_PIH_FDN_BDN_ENABLE = 1,  /* ����FDN/BDN���� */
    SI_PIH_FDN_BDN_QUERY = 2,   /* ȥ����FDN/BDN״̬��ѯ */
    SI_PIH_FDN_BDN_CMD_BUTT
};
typedef VOS_UINT32 SI_PIH_FdnBdnCmdUint32;

enum SI_PIH_SimIndex {
    SI_PIH_SIM_REAL_SIM1 = 1,
    SI_PIH_SIM_VIRT_SIM1 = 11,
    SI_PIH_SIM_INDEX_BUTT
};
typedef VOS_UINT8 SI_PIH_SimIndexUint8;

enum SI_PIH_SimSlot {
    SI_PIH_SIM_SLOT1 = 1,
    SI_PIH_SIM_SLOT2 = 2,
    SI_PIH_SIM_SLOT3 = 3,
    SI_PIH_SIM_SLOT_BUTT
};
typedef VOS_UINT8 SI_PIH_SimSlotUint8;

enum SI_PIH_SimState {
    SI_PIH_SIM_DISABLE = 0,
    SI_PIH_SIM_ENABLE = 1,
    SI_PIH_SIM_STATE_BUTT
};
typedef VOS_UINT8 SI_PIH_SimStateUint8;

enum SI_PIH_CardUse {
    SI_PIH_CARD_NOUSE = 0,
    SI_PIH_CARD_USE = 1,
    SI_PIH_CARD_USE_BUTT
};
typedef VOS_UINT8 SI_PIH_CardUseUint8;

/* PASSTHROUGH AT��������ʱʹ�������ֵ */
enum SI_PIH_PassThroughState {
    SI_PIH_PASSTHROUGH_NONE = 0,
    SI_PIH_PASSTHROUGH_POWER_ON = 1,
    SI_PIH_PASSTHROUGH_POWER_OFF = 2,
    SI_PIH_PASSTHROUGH_BUTT
};
typedef VOS_UINT32 SI_PIH_PassThroughStateUint32;

enum SI_PIH_CardType {
    SI_PIH_RSIM = 0,
    SI_PIH_VSIM,
    SI_PIH_CARD_BUTT,
};

enum SI_PIH_AtuiccAuthType {
    SI_PIH_ATUICCAUTH_USIMAKA = 0,
    SI_PIH_ATUICCAUTH_ISIMAKA,
    SI_PIH_ATUICCAUTH_USIMGBA,
    SI_PIH_ATUICCAUTH_ISIMGBA,
    SI_PIH_ATUICCAUTH_BUTT
};

enum SI_PIH_AtksnafAuthType {
    SI_PIH_ATKSNAFAUTH_USIM = 0,
    SI_PIH_ATKSNAFAUTH_ISIM,
    SI_PIH_ATKSNAFAUTH_BUTT
};

enum SI_PIH_UiccAuthType {
    SI_PIH_UICCAUTH_AKA,
    SI_PIH_UICCAUTH_GBA,
    SI_PIH_UICCAUTH_NAF,
    SI_PIH_UICCAUTH_BUTT
};
typedef VOS_UINT32 SI_PIH_UiccAuthUint32;

enum SI_PIH_UiccApp {
    SI_PIH_UICCAPP_USIM,
    SI_PIH_UICCAPP_ISIM,
    SI_PIH_UICCAPP_BUTT
};
typedef VOS_UINT32 SI_PIH_UiccappUint32;

enum SI_PIH_AccessType {
    SI_PIH_ACCESS_READ,
    SI_PIH_ACCESS_WRITE,
    SI_PIH_ACCESS_BUTT
};
typedef VOS_UINT8 SI_PIH_AccessTypeUint8;

enum SI_PIH_AuthStatus {
    SI_PIH_AUTH_SUCCESS,
    SI_PIH_AUTH_FAIL,
    SI_PIH_AUTH_SYNC,
    SI_PIH_AUTH_UNSUPPORT,
    SI_PIH_AUTHSTATUS_BUTT
};
typedef VOS_UINT32 SI_PIH_AuthStatusUint32;

enum SI_PIH_CardVersionType {
    SI_PIH_CARD_NON_TYPE = 0,
    SI_PIH_CARD_ICC_TYPE = 1,
    SI_PIH_CARD_UICC_TYPE = 2,
    SI_PIH_CARD_VERSION_TYPE_BUTT
};
typedef VOS_UINT8 SI_PIH_CardVersionTypeUint8;

enum SI_PIH_HvteeError {
    SI_PIH_HVTEE_NOERROR = VOS_OK,
    SI_PIH_HVTEE_ADDR_ERROR,
    SI_PIH_HVTEE_HEAD_ERROR,
    SI_PIH_HVTEE_END_ERROR,
    SI_PIH_HVTEE_DATA_ERROR,
    SI_PIH_HVTEE_LEN_ERROR,
    SI_PIH_HVTEE_ENCODE_ERROR,
    SI_PIH_HVTEE_ERROR_BUTT
};
typedef VOS_UINT32 SI_PIH_HvteeErrorUint32;

enum SI_PIH_HvCheckCardStatus {
    SI_PIH_HVCHECKCARD_CARDIN = 0,
    SI_PIH_HVCHECKCARD_ABSENT = 1,
    SI_PIH_HVCHECKCARD_BUTT
};
typedef VOS_UINT32 SI_PIH_HvCheckCardStatusUint32;

enum SI_PIH_ChangePollTimer {
    SI_PIH_CHANGEPOLLTIMER_TIMERLEN = 0,
    SI_PIH_CHANGEPOLLTIMER_TIMEROFF = 1,
    SI_PIH_CHANGEPOLLTIMER_CALLON = 2,
    SI_PIH_CHANGEPOLLTIMER_CALLOFF = 3,
    SI_PIH_CHANGEPOLLTIMER_BUTT
};
typedef VOS_UINT32 SI_PIH_ChangePollTimerUint32;

/*
 * ����˵��: SI_PIH������Ϣ����ö��
 */
enum SI_PIH_CmdReqType {
    SI_PIH_QUERY_CARDSTATUS_REQ = 0x1000,
    SI_PIH_START_CHECK_KEYFILE_NTF,
    SI_PIH_STOP_CHECK_KEYFILE_NTF,
    SI_PIH_CMD_REQ_TYPE_ENUM_BUTT
};
typedef VOS_UINT32 SI_PIH_CmdReqTypeUint32;

/*
 * ����˵��: SI_PIH����ظ���Ϣ����ö��
 */
enum SI_PIH_CmdCnfType {
    SI_PIH_QUERY_CARDSTATUS_CNF = 0x2000,

    SI_PIH_CHECK_KEYFILE_RLST_IND,

    SI_PIH_VSIM_APN_VALUE_IND,

    SI_PIH_CMD_CNF_TYPE_ENUM_BUTT
};
typedef VOS_UINT32 SI_PIH_CmdCnfTypeUint32;

/*
 * ����˵��: ISDB͸��APDU�����ݽṹ
 * 1. ��    ��: 2012��08��28��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 len;                     /* ����APDU���ݳ��� */
    VOS_UINT8 command[SI_APDU_MAX_LEN]; /* ����APDU�������� */
} SI_PIH_IsdbAccessCommand;

#if ((OSA_CPU_ACPU == VOS_OSA_CPU) || (defined(DMT))) || (defined(__PC_UT__) || \
    (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON))
/*
 * ����˵��: ���߼�ͨ�������ݽṹ
 * 1. ��    ��: 2013��05��14��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 aidLen; /* AID�ĳ��� */
    VOS_UINT32 rsv;
    VOS_UINT8 *adfName; /* ����ADF������ */
} SI_PIH_CchoCommand;

/*
 * ����˵��: ���߼�ͨ�������ݽṹ,��APDU����P2
 * 1. ��    ��: 2016��09��26��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 aidLen; /* AID�ĳ��� */
    VOS_UINT8 apdup2;  /* Save APDU para P2 for OMA3.0 */
    VOS_UINT8 rsv[3];
    VOS_UINT8 *adfName; /* ����ADF������ */
} SI_PIH_CchpCommand;

/*
 * ����˵��: ͸�����߼�ͨ��APDU�����ݽṹ
 * 1. ��    ��: 2013��05��14��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 sessionID; /* ͨ���� */
    VOS_UINT32 len;       /* ����APDU���ݳ��� */
    VOS_UINT8 *command;   /* ����APDU�������� */
} SI_PIH_CglaCommand;
#endif

/*
 * ����˵��: �����߼�ͨ�����ݻظ����
 * 1. ��    ��: 2013��05��14��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT16 len;                     /* ���APDU���ݳ��� */
    VOS_UINT8 sw1;                      /* ����״̬��1      */
    VOS_UINT8 sw2;                      /* ����״̬��2      */
    VOS_UINT8 command[SI_APDU_MAX_LEN]; /* ���APDU�������� */
} SI_PIH_CglaCommandCnf;

/*
 * ����˵��: �����߼�ͨ�������ϱ����
 * 1. ��    ��: 2017��02��21��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT16 len; /* ���APDU���ݳ��� */
    VOS_UINT8 lastDataFlag;
    VOS_UINT8 sw1; /* ����״̬��1      */
    VOS_UINT8 sw2; /* ����״̬��2      */
    VOS_UINT8 rsv[3];
    VOS_UINT8 command[SI_PRIVATECGLA_APDU_MAX_LEN]; /* ���APDU�������� */
} SI_PIH_CglaHandleCnf;

/*
 * ����˵��: ��ѯATR���ݻظ����
 * 1. ��    ��: 2013��08��22��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 len;                    /* ���APDU���ݳ��� */
    VOS_UINT8 command[SI_ATR_MAX_LEN]; /* ���APDU�������� */
} SI_PIH_AtrQryCnf;

/*
 * ����˵��: ISDB͸��APDU�����ݻظ����
 */
typedef struct {
    VOS_UINT16 len;                     /* ���APDU���ݳ��� */
    VOS_UINT8 sw1;                      /* ����״̬��1      */
    VOS_UINT8 sw2;                      /* ����״̬��2      */
    VOS_UINT8 command[SI_APDU_MAX_LEN]; /* ���APDU�������� */
} SI_PIH_IsdbAccessCommandCnf;

typedef struct {
    VOS_UINT32 len;
    VOS_UINT8 command[SI_APDU_MAX_LEN];
} SI_PIH_CsimCommand;

typedef struct {
    SI_PIH_FdnBdnCmdUint32 fdnCmd;
    SI_PIH_FdnBdnStateUint32 fdnState;
} SI_PIH_EventFdnCnf;

typedef struct {
    SI_PIH_FdnBdnCmdUint32 bdnCmd;
    SI_PIH_FdnBdnStateUint32 bdnState;
} SI_PIH_EventBdnCnf;

typedef struct {
    VOS_UINT16 len;
    VOS_UINT8 sw1;
    VOS_UINT8 sw2;
    VOS_UINT8 command[256];
} SI_PIH_CsimCommandCnf;

typedef struct {
    SI_PIH_RestricCmdUint32 cmdType;
    VOS_UINT16 fileId;
    VOS_UINT8 para1;
    VOS_UINT8 para2;
    VOS_UINT8 para3;
    VOS_UINT8 rsv[3];
    VOS_UINT8 command[256];
} SI_RESTRIC_Access;

typedef struct {
    SI_PIH_SimStateUint8 vSimState; /* vSIM��״̬����Ӳ��״̬���� */
    SI_PIH_CardUseUint8 cardUse;    /* ���ܷ�ʹ�� */
    VOS_UINT8 rsv[2];
} SI_PIH_EventHvsstQueryCnf;

typedef struct {
    VOS_UINT8 index;
    SI_PIH_SimStateUint8 simSet;
    VOS_UINT8 slot;
    VOS_UINT8 rsv;
} SI_PIH_HvsstSet;

/*
 * ����˵��: AT^ESIMSWITCH������Ϣ�ṹ��
 */
typedef struct {
    VOS_UINT8 slot;
    VOS_UINT8 cardType;
    VOS_UINT8 rsv[2];
} SI_PIH_EsimSwitchSet;

typedef struct {
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
} SI_PIH_Plmn;

typedef struct {
    VOS_UINT8 index;
    VOS_UINT8 cardCap;  /* ��BITλ���д�ţ�BIT1:SIM,BIT2:USIM */
    VOS_UINT8 cardType; /* ��BITλ���д�ţ�BIT1:SIM,BIT2:USIM */
    VOS_UINT8 rsv;
    VOS_UINT8 imsi[USIMM_EF_IMSI_LEN * 2];  /* IMSI,��Ҫ����ASCII�볤������һ�� */
    VOS_UINT16 plmnNum;                     /* PLMN���� */
    SI_PIH_Plmn plmn[SI_PIH_HPLMN_MAX_NUM]; /* PLMN���� */
} SI_PIH_CardContent;

typedef struct {
    SI_PIH_CardContent simCard[SI_PIH_CARD_BUTT]; /* ֧�ֿ�������������Ӳ�������⿨ */
} SI_PIH_HvsContQueryCnf;

typedef struct {
    VOS_BOOL reDhFlag;
} SI_PIH_HvrdhInd;

typedef struct {
    VOS_UINT8 ik[USIMM_AUTH_IKSPACELEN];       /* Len+Data */
    VOS_UINT8 ck[USIMM_AUTH_CKSPACELEN];       /* Len+Data */
    VOS_UINT8 auts[USIMM_AUTH_AUTHSPACELEN];   /* Len+Data */
    VOS_UINT8 authRes[USIMM_AUTH_RESSPACELEN]; /* Len+Data */
    VOS_UINT8 rsv[2];
} SI_PIH_UiccAkaData;

typedef struct {
    VOS_UINT8 ksExtNaf[USIMM_T0_APDU_MAX_LEN]; /* Len+Data */
} SI_PIH_UiccNafData;

typedef struct {
    SI_PIH_AuthStatusUint32 status;
    SI_PIH_UiccappUint32 appType;   /* Ӧ������ */
    SI_PIH_UiccAuthUint32 authType; /* ��Ȩ���� */
    SI_PIH_UiccAkaData akaData;
    SI_PIH_UiccNafData nAFData;
} SI_PIH_UiccAuthCnf;

typedef struct {
    SI_PIH_AccessTypeUint8 cmdType;
    VOS_UINT8 recordNum;
    VOS_UINT16 efId;
    VOS_UINT32 result;
    VOS_UINT32 dataLen;
    VOS_UINT8 command[USIMM_T0_APDU_MAX_LEN];
} SI_PIH_UiccAccessfileCnf;

typedef struct {
    VOS_UINT32 dataLen;
    VOS_UINT32 rsv;
    VOS_UINT8 *data;
} SI_PIH_U8LvData;

typedef struct {
    SI_PIH_U8LvData fileName;
    SI_PIH_U8LvData fileSubName;
    VOS_UINT32 ref;
    VOS_UINT32 totalNum;
    VOS_UINT32 index;
    VOS_UINT32 rsv;
    SI_PIH_U8LvData fileData;
} SI_PIH_FileWriteData;

typedef struct {
    VOS_UINT32 randLen;
    VOS_UINT8 rand[SI_AUTH_DATA_MAX];
    VOS_UINT32 authLen;
    VOS_UINT8 auth[SI_AUTH_DATA_MAX];
} SI_PIH_UiccAuthAka;

typedef struct {
    VOS_UINT32 nafIdLen;
    VOS_UINT8 nafId[SI_AUTH_DATA_MAX];
    VOS_UINT32 impiLen;
    VOS_UINT8 impi[SI_AUTH_DATA_MAX];
} SI_PIH_UiccAuthNaf;

typedef struct {
    SI_PIH_UiccAuthUint32 authType;
    SI_PIH_UiccappUint32 appType;
    union {
        SI_PIH_UiccAuthAka aka;
        SI_PIH_UiccAuthNaf ksNAF;
    } authData;
} SI_PIH_UiccAuth;

typedef struct {
    SI_PIH_UiccappUint32 appType;
    SI_PIH_AccessTypeUint8 cmdType;
    VOS_UINT8 recordNum;
    VOS_UINT16 efId;
    VOS_UINT32 rsv1;
    VOS_UINT32 dataLen;
    VOS_UINT8 command[USIMM_T0_APDU_MAX_LEN];
    VOS_UINT16 pathLen;
    VOS_UINT16 path[USIMM_MAX_PATH_LEN];
    VOS_UINT16 rsv2;
} SI_PIH_AccessFile;

/*
 * ����˵��: CRSM����
 * 1. ��    ��: 2015��04��09��
 *    �޸�����: �½�
 */
typedef struct {
    USIMM_RestricCmdUint32 cmdType;
    VOS_UINT16 efId;
    VOS_UINT16 pathLen;
    VOS_UINT8 p1;
    VOS_UINT8 p2;
    VOS_UINT8 p3;
    VOS_UINT8 rsv;
    VOS_UINT16 path[USIMM_MAX_PATH_LEN];
    VOS_UINT8 content[USIMM_T0_APDU_MAX_LEN + 1];
    VOS_UINT8 rsv2[3];
} SI_PIH_Crsm;

/*
 * ����˵��: CRLA����
 * 1. ��    ��: 2015��04��09��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 sessionID;
    USIMM_RestricCmdUint32 cmdType;
    VOS_UINT16 efId;
    VOS_UINT16 pathLen;
    VOS_UINT8 p1;
    VOS_UINT8 p2;
    VOS_UINT8 p3;
    VOS_UINT8 rsv;
    VOS_UINT16 path[USIMM_MAX_PATH_LEN];
    VOS_UINT8 content[USIMM_T0_APDU_MAX_LEN + 1];
    VOS_UINT8 rsv2[3];
} SI_PIH_Crla;

typedef struct {
    VOS_BOOL bAPNFlag;
    VOS_BOOL bDHParaFlag;
    VOS_BOOL bVSIMDataFlag;
    VOS_UINT32 rfu;
} SI_PIH_HvteeSet;

typedef struct {
    SI_PIH_CardVersionTypeUint8 mode;
    VOS_UINT8 hasCModule;
    VOS_UINT8 hasGModule;
    VOS_UINT8 rfu;
} SI_PIH_EventCardTypeQueryCnf;

/*
 * ����˵��: CRSM&CRLA����ظ�
 * 1. ��    ��: 2015��04��08��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT8 sw1;                             /* Status Word 1 */
    VOS_UINT8 sw2;                             /* Status Word 2 */
    VOS_UINT16 len;                            /* �������ݳ���,������SW1��SW2       */
    VOS_UINT8 content[USIMM_APDU_RSP_MAX_LEN]; /* ����Data */
} SI_PIH_RaccessCnf;

typedef struct {
    SI_PIH_CardSlotUint32 card0Slot;
    SI_PIH_CardSlotUint32 card1Slot;
    SI_PIH_CardSlotUint32 card2Slot;
} SI_PIH_SciCfgQueryCnf;

typedef struct {
    VOS_UINT32 apnResult;
    VOS_UINT32 dhresult;
    VOS_UINT32 vSIMResult;
    VOS_UINT32 rsv;
} SI_PIH_HvteeSetCnf;

typedef struct {
    VOS_UINT32 data;
    VOS_UINT32 rsv;
} SI_PIH_TeeTimeoutInd;

typedef struct {
    SI_PIH_HvCheckCardStatusUint32 data;
} SI_PIH_HvCheckCardCnf;

/* +CIMI - ��ȡIMSI */
typedef struct {
    VOS_UINT8 imsi[SI_IMSI_MAX_LEN];
} SI_PIH_EventImsi;

typedef struct {
    VOS_UINT32 voltage;
    VOS_UINT8 charaByte;
    VOS_UINT8 rsv[3];
} SI_PIH_EventCardvoltageQueryCnf;

/*
 * 1. ��    ��: 2017��07��07��
 * �޸�����: �����ṹ
 */
typedef struct {
    VOS_UINT8 cryptoPin[SI_CRYPTO_CBC_PIN_LEN]; /* PIN������ */
    VOS_UINT32 pinIv[4];                        /* IVֵ */
    VOS_UINT8 hmacValue[SI_SIGNATURE_LEN];      /* hmacǩ�� */
} SI_PIH_CryptoPin;

/*
 * ����˵��: ���߼�ͨ�����ݻظ�������sessionId��ѡ��Ӧ�õĻظ�����
 * 1. ��    ��: 2019��04��09��
 *    �޸�����: �½�
 */
typedef struct {
    VOS_UINT32 sessionID;
    VOS_UINT8 sw1;                      /* ����״̬��1  */
    VOS_UINT8 sw2;                      /* ����״̬��2  */
    VOS_UINT16 rspDataLen;              /* ������ݳ��� */
    VOS_UINT8 rspDate[SI_APDU_MAX_LEN]; /* ����������� */
} SI_PIH_OpenChannelCommandCnf;

typedef struct {
    VOS_UINT16 clientId;
    VOS_UINT8 opId;
    VOS_UINT8 reserve;
    SI_PIH_EventUint32 eventType;
    SI_PIH_ERROR pihError;
    union {
        SI_PIH_EventFdnCnf fdnCnf;
        SI_PIH_EventBdnCnf bdnCnf;
        SI_PIH_CsimCommandCnf gAccessCnf;
        SI_PIH_IsdbAccessCommandCnf isdbAccessCnf;
        SI_PIH_OpenChannelCommandCnf openChannelCnf;
        SI_PIH_CglaCommandCnf cglaCmdCnf;
        SI_PIH_AtrQryCnf atrQryCnf;
        SI_PIH_EventHvsstQueryCnf hvsstQueryCnf;

        SI_PIH_HvsContQueryCnf hvsContCnf;
        SI_PIH_HvrdhInd hvrdhInd;
        SI_PIH_UiccAuthCnf uiccAuthCnf;
        SI_PIH_UiccAccessfileCnf uiccAcsFileCnf;
        SI_PIH_EventCardTypeQueryCnf cardTypeCnf;
        SI_PIH_RaccessCnf rAccessCnf;
        VOS_UINT32 sessionID[USIMM_CARDAPP_BUTT];
        SI_PIH_SciCfgQueryCnf sciCfgCnf;
        SI_PIH_HvteeSetCnf hVTEECnf;
        SI_PIH_TeeTimeoutInd teeTimeOut;
        SI_PIH_HvCheckCardCnf hvCheckCardCnf;
        SI_PIH_EventImsi imsi;
        USIMM_ErrorInfoData usimmErrorInd;
        VOS_UINT8 iccidContent[USIMM_ICCID_FILE_LEN];

        VOS_UINT32 simHotPlugStatus;

        VOS_UINT32 apduSWCheckResult;

        SI_PIH_EventCardvoltageQueryCnf cardVoltageCnf;
        SI_PIH_CglaHandleCnf cglaHandleCnf;

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
        SI_PIH_CryptoPin cryptoPin;
#endif
        USIMM_CardStatusInd cardStatusInd;
        USIMM_SlotCardType slotCardType;
        SI_PIH_PassThroughStateUint32 passThroughState;

#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
        SI_PIH_CardSlotUint32 singleModemSlotCfg;
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
        VOS_UINT32 refreshType;
        VOS_UINT32 noCardMode;
#endif
#endif

    } pihEvent;
} SI_PIH_EventInfo;

typedef struct {
    VOS_MSG_HEADER
    SI_PIH_ChangePollTimerUint32 msgName; /* ��Ϣ�� */
    VOS_UINT32 timerLen;
} SI_PIH_ChangePollTimerReq;

/*
 * ����˵��: NAS�ؼ��ļ���ͨ�б�ṹ��
 */
typedef struct {
    USIMM_CardAppUint32 appType;
    USIMM_FilePathInfo filePath;
} SI_PIH_FileInfo;

/*
 * ����˵��: NAS�ؼ��ļ���֪ͨPIH��Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdReqTypeUint32 msgName;
    VOS_UINT32 fileNum;
    SI_PIH_FileInfo fileInfo[SI_PIH_KEYFILE_MAX_NUM];
} SI_PIH_StartCheckKeyFileNtf;

/*
 * ����˵��: �ؼ��ļ���ֹͣ��Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdReqTypeUint32 msgName;
} SI_PIH_StopCheckKeyFileNtf;

/*
 * ����˵��: �ؼ��ļ����֪ͨNAS��Ϣ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdCnfTypeUint32 msgName;
    USIMM_CardAppUint32 appType;
    USIMM_FilePathInfo filePath;
    VOS_UINT32 fileLen;
    VOS_UINT8 fileData[4];
} SI_PIH_CheckKeyFileRlstInd;

/*
 * ����˵��: ��Ӧ��״̬��ѯ�ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdReqTypeUint32 msgName;
} SI_PIH_QueryCardStatusReq;

/*
 * ����˵��: ��Ӧ��״̬��Ϣ
 */
typedef struct {
    USIMM_CardAppServicUint32 cardAppService;
    VOS_UINT32 isTestCard; /* VOS_TRUEΪ���Կ���VOS_FALSEΪ�ǲ��Կ� */
} SI_PIH_CardAppStatus;

/*
 * ����˵��: ��Ӧ��״̬�ظ��ṹ��
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdCnfTypeUint32 msgName;
    USIMM_PhyCardTypeUint32 phyCardType; /* ����״̬ */
    SI_PIH_CardAppStatus usimSimInfo;    /* GUL SIM״̬ */
    SI_PIH_CardAppStatus csimUimInfo;    /* CDMA SIM״̬ */
    SI_PIH_CardAppStatus isimInfo;       /* ISIM״̬ */
    USIMM_CardStatusAddinfo addInfo;     /* ��״̬��Чʱ�����ʹ���������Ϣ */
} SI_PIH_QueryCardStatusCnf;

/* �ⲿ�ӿ� */
#if (FEATURE_ON == FEATURE_VSIM)
/*
 * ����˵��: PIHģ��͸��APN���ݸ�MMA
 * 1. ��    ��: 2017��1��11��
 *    �޸�����: ����
 */
typedef struct {
    VOS_MSG_HEADER
    SI_PIH_CmdCnfTypeUint32 msgName; /* ��Ϣ�� */
    VOS_UINT8 length;                /* VSIM APN���ݳ��� */
    VOS_UINT8 rev[3];
    VOS_UINT8 vsimApn[SI_PIH_VSIMAPN_MAX]; /* VSIM APN���� */
} SI_PIH_VsimApnInd;

#ifdef CONFIG_TZDRIVER
typedef struct {
    unsigned int timeType; /* Timer Type */
    unsigned int timeId;   /* Timer ID */
    unsigned int rev1;
    unsigned int rev2;
} TEEC_TIMER_Property;

extern int TC_NS_RegisterServiceCallbackFunc(char *uuid, void *func, void *privateData);

extern VOS_VOID SI_PIH_TEETimeOutCB(VOS_VOID *timerDataCb);

#endif /* CONFIG_TZDRIVER */
#endif /* (FEATURE_ON == FEATURE_VSIM) */

#if (OSA_CPU_NRCPU != VOS_OSA_CPU)
#if ((OSA_CPU_ACPU == VOS_OSA_CPU) || (defined(DMT))) || (defined(__PC_UT__) || \
    (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON))
extern VOS_UINT32 SI_PIH_GetReceiverPid(MN_CLIENT_ID_T clientId, VOS_UINT32 *receiverPid);

extern VOS_UINT32 SI_PIH_FdnEnable(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const VOS_UINT8 *pin2,
    VOS_UINT32 len);

extern VOS_UINT32 SI_PIH_FdnDisable(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const VOS_UINT8 *pin2,
    VOS_UINT32 len);

extern VOS_UINT32 SI_PIH_FdnBdnQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, SI_PIH_QueryTypeUint32 queryType);

extern VOS_UINT32 SI_PIH_GenericAccessReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_CsimCommand *data);

extern VOS_UINT32 SI_PIH_IsdbAccessReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_IsdbAccessCommand *data);

extern VOS_UINT32 SI_PIH_CchoSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_CchoCommand *cchoCmd);

extern VOS_UINT32 SI_PIH_CchpSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_CchpCommand *cchopCmd);

VOS_UINT32 SI_PIH_PrivateCchoSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_CchoCommand *cchoCmd);

VOS_UINT32 SI_PIH_PrivateCchpSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_CchpCommand *cchopCmd);

extern VOS_UINT32 SI_PIH_CchcSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT32 sessionID);

extern VOS_UINT32 SI_PIH_HvSstQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_HvSstSet(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_HvsstSet *hvSStSet);

VOS_UINT32 SI_PIH_PassThroughSet(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    SI_PIH_PassThroughStateUint32 state);

VOS_UINT32 SI_PIH_PassThroughQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
VOS_UINT32 SI_PIH_SingleModemDualSlotSet(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    SI_PIH_CardSlotUint32 slotId);
VOS_UINT32 SI_PIH_SingleModemDualSlotQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
#endif

extern VOS_UINT32 SI_PIH_CglaSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, SI_PIH_CglaCommand *data);

extern VOS_UINT32 SI_PIH_GetCardATRReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_CrsmSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_Crsm *crsmPara);

extern VOS_UINT32 SI_PIH_CrlaSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_Crla *crlaPara);

extern VOS_UINT32 SI_PIH_CardSessionQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_CimiSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_CCimiSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_SciCfgSet(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, SI_PIH_CardSlotUint32 card0Slot,
    SI_PIH_CardSlotUint32 card1Slot, SI_PIH_CardSlotUint32 card2Slot);

extern VOS_UINT32 SI_PIH_SciCfgQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_BwtSet(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT16 protectTime);

extern VOS_UINT32 SI_PIH_HvCheckCardQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_UiccAuthReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const SI_PIH_UiccAuth *data);

extern VOS_UINT32 SI_PIH_AccessUICCFileReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_AccessFile *data);

extern VOS_UINT32 SI_PIH_CardTypeQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_CardTypeExQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
extern VOS_UINT32 SI_PIH_GetSilentPinInfoReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const VOS_UINT8 *pin,
    VOS_UINT32 len);

extern VOS_UINT32 SI_PIH_SetSilentPinReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_CryptoPin *cryptoPin);
#endif
extern VOS_UINT32 SI_PIH_CardVoltageQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

extern VOS_UINT32 SI_PIH_PrivateCglaSetReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_CglaCommand *data);

VOS_UINT32 SI_PIH_GetSecIccVsimVer(VOS_VOID);

VOS_UINT32 SI_PIH_EsimSwitchSetFunc(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
    const SI_PIH_EsimSwitchSet *esimSwitchSet);

VOS_UINT32 SI_PIH_EsimSwitchQuery(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);
#endif

#if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (defined(DMT)))

extern VOS_UINT32 WuepsPIHPidInit(enum VOS_InitPhaseDefine initPhrase);

extern VOS_UINT32 PIH_RegUsimCardStatusIndMsg(ModemIdUint16 modemId, VOS_UINT32 regPID);

extern VOS_UINT32 PIH_DeregUsimCardStatusIndMsg(ModemIdUint16 modemId, VOS_UINT32 regPID);
extern VOS_UINT32 PIH_DeregCardRefreshIndMsg(ModemIdUint16 modemId, VOS_UINT32 regPID);
#endif /* #if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (defined(DMT))) */
#endif /* #if (OSA_CPU_NRCPU != VOS_OSA_CPU) */

#if (VOS_OSA_CPU != OSA_CPU_ACPU)
VOS_UINT32 PIH_RegCardRefreshIndMsg(ModemIdUint16 modemId, VOS_UINT32 regPID);
#endif

#if ((VOS_OSA_CPU == OSA_CPU_ACPU) || (defined(DMT)))
#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
VOS_UINT32 SI_PIH_SendNoCardReq(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T operationId, VOS_UINT32 setValue);
#endif
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
