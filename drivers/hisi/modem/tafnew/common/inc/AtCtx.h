/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __ATCTX_H__
#define __ATCTX_H__

#include "product_config.h"
#include "v_id.h"
#include "v_typdef.h"
#include "v_msg.h"
#include "ps_common_def.h"
#include "PsTypeDef.h"
#include "mdrv_nvim.h"
#include "mn_client.h"
#include "taf_api.h"
#include "mn_msg_api.h"
#include "mn_call_api.h"
#include "taf_type_def.h"
#include "taf_ps_api.h"
#include "taf_app_mma.h"
#include "taf_app_ssa.h"
#include "taf_tafm_remote.h"
#include "AtCtxPacket.h"
#include "AtTimer.h"
#include "AtInternalMsg.h"
#include "AtMntn.h"
#include "at_imsa_interface.h"
#include "taf_drv_agent.h"
#include "at_mt_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/*
 * �㲥ClientIndex˵������ߵ�2��bitλ��ʾModemId��
 * Modem0��Ӧ�Ĺ㲥Client indexΪ00111111(0x3F)��
 * Modem1��Ӧ�Ĺ㲥Client indexΪ01111111(0x7F)
 * Modem2��Ӧ�Ĺ㲥Client indexΪ10111111(0xBF)
 */

/* �㲥Client Id��Ӧ��indexֵ */
/* Modem 0�Ĺ㲥Client Index */
#define AT_BROADCAST_CLIENT_INDEX_MODEM_0 0x3F

/* Modem 1�Ĺ㲥Client Index */
#define AT_BROADCAST_CLIENT_INDEX_MODEM_1 0x7F

/* Modem 2�Ĺ㲥Client Index */
#define AT_BROADCAST_CLIENT_INDEX_MODEM_2 0xBF

/* �ж��Ƿ�Ϊ�㲥Client Id */
#define AT_IS_BROADCAST_CLIENT_INDEX(i)                                                          \
    (((i) == AT_BROADCAST_CLIENT_INDEX_MODEM_0) || ((i) == AT_BROADCAST_CLIENT_INDEX_MODEM_1) || \
     ((i) == AT_BROADCAST_CLIENT_INDEX_MODEM_2))

/* Modem ID��NV���е�Bitλ */
#define AT_NV_CLIENT_CONFIG_MODEM_ID_OFFSET 2

/* ********************************PORT Begin**************************** */

#define AT_INVALID_SPE_PORT (-1)

#define AT_MIN_ABORT_TIME_INTERNAL 13
#define AT_MAX_TICK_TIME_VALUE 0xFFFFFFFFU

/* ================================PORT NO End================================ */
/* ********************************PORT End**************************** */

/* ********************************COMM Begin**************************** */
typedef TAF_UINT8 AT_CMEE_TYPE;
#define AT_CMEE_ONLY_ERROR 0
#define AT_CMEE_ERROR_CODE 1
#define AT_CMEE_ERROR_CONTENT 2

#define AT_OCTET_MOVE_FOUR_BITS 0x04  /* ��һ���ֽ��ƶ�4λ */
#define AT_OCTET_MOVE_EIGHT_BITS 0x08 /* ��һ���ֽ��ƶ�8λ */
#define AT_OCTET_LOW_FOUR_BITS 0x0f   /* ��ȡһ���ֽ��еĵ�4λ */
#define AT_OCTET_HIGH_FOUR_BITS 0xf0  /* ��ȡһ���ֽ��еĸ�4λ */
#define AT_MIN(n1, n2) (((n1) > (n2)) ? (n2) : (n1))
/* ********************************COMM End**************************** */

/* ********************************CC/SS Begin**************************** */
typedef VOS_UINT8 AT_SALS_TYPE;
#define AT_SALS_DISABLE_TYPE 0
#define AT_SALS_ENABLE_TYPE 1

typedef VOS_UINT8 AT_CLIP_TYPE;
#define AT_CLIP_DISABLE_TYPE 0
#define AT_CLIP_ENABLE_TYPE 1

typedef VOS_UINT8 AT_COLP_TYPE;
#define AT_COLP_DISABLE_TYPE 0
#define AT_COLP_ENABLE_TYPE 1

typedef VOS_UINT8 AT_CLIR_TYPE;
#define AT_CLIR_AS_SUBSCRIPT 0
#define AT_CLIR_INVOKE 1
#define AT_CLIR_SUPPRESS 2

typedef VOS_UINT8 AT_CRC_TYPE;
#define AT_CRC_DISABLE_TYPE 0
#define AT_CRC_ENABLE_TYPE 1

typedef VOS_UINT8 AT_CCWA_TYPE;
#define AT_CCWA_DISABLE_TYPE 0
#define AT_CCWA_ENABLE_TYPE 1

#define AT_SIMST_DISABLE_TYPE 0
#define AT_SIMST_ENABLE_TYPE 1

typedef VOS_UINT8 AT_CUSD_STATE_TYPE;
#define AT_CUSD_MT_IDL_STATE 0
#define AT_CUSD_MT_REQ_STATE 1
#define AT_CUSD_NT_IND_STATE 2

typedef MN_CALL_ModeUint8 AT_CMOD_MODE_ENUM;

/* ********************************CC/SS End**************************** */

/* ********************************SMS Begin*********************************** */
#define AT_SMSMT_BUFFER_MAX 2

typedef VOS_UINT8 AT_CSCS_TYPE;
#define AT_CSCS_IRA_CODE 0
#define AT_CSCS_UCS2_CODE 1
#define AT_CSCS_GSM_7Bit_CODE 2

typedef VOS_UINT8 AT_CSDH_TYPE;
#define AT_CSDH_NOT_SHOW_TYPE 0
#define AT_CSDH_SHOW_TYPE 1

typedef VOS_UINT8 AT_CNMI_MODE_TYPE;
#define AT_CNMI_MODE_BUFFER_TYPE 0
#define AT_CNMI_MODE_SEND_OR_DISCARD_TYPE 1
#define AT_CNMI_MODE_SEND_OR_BUFFER_TYPE 2
#define AT_CNMI_MODE_EMBED_AND_SEND_TYPE 3

typedef VOS_UINT8 AT_CNMI_MT_TYPE;
#define AT_CNMI_MT_NO_SEND_TYPE 0
#define AT_CNMI_MT_CMTI_TYPE 1
#define AT_CNMI_MT_CMT_TYPE 2
#define AT_CNMI_MT_CLASS3_TYPE 3
#define AT_CNMI_MT_TYPE_MAX 4

typedef VOS_UINT8 AT_CNMI_BM_TYPE;
#define AT_CNMI_BM_NO_SEND_TYPE 0
#define AT_CNMI_BM_CBMI_TYPE 1
#define AT_CNMI_BM_CBM_TYPE 2
#define AT_CNMI_BM_CLASS3_TYPE 3

typedef VOS_UINT8 AT_CNMI_DS_TYPE;
#define AT_CNMI_DS_NO_SEND_TYPE 0
#define AT_CNMI_DS_CDS_TYPE 1
#define AT_CNMI_DS_CDSI_TYPE 2
#define AT_CNMI_DS_TYPE_MAX 3

typedef VOS_UINT8 AT_CNMI_BFR_TYPE;
#define AT_CNMI_BFR_SEND_TYPE 0
#define AT_CNMI_BFR_CLEAR_TYPE 1

#define AT_BUFFER_CBM_EVENT_MAX 5
#define AT_BUFFER_SMT_EVENT_MAX 5

/* ********************************SMS End*********************************** */

/* ********************************NET Begin*********************************** */
typedef VOS_UINT8 AT_MODE_REPORT_TYPE;
#define AT_MODE_RESULT_CODE_NOT_REPORT_TYPE 0
#define AT_MODE_RESULT_CODE_REPORT_TYPE 1

typedef VOS_UINT8 AT_CREG_TYPE;
#define AT_CREG_RESULT_CODE_NOT_REPORT_TYPE 0
#define AT_CREG_RESULT_CODE_BREVITE_TYPE 1
#define AT_CREG_RESULT_CODE_ENTIRE_TYPE 2

typedef VOS_UINT8 AT_CGREG_TYPE;
#define AT_CGREG_RESULT_CODE_NOT_REPORT_TYPE 0
#define AT_CGREG_RESULT_CODE_BREVITE_TYPE 1
#define AT_CGREG_RESULT_CODE_ENTIRE_TYPE 2

#define AT_ROAM_FEATURE_OFF 0
#define AT_ROAM_FEATURE_ON 1
/* ********************************NET End*********************************** */

/* ********************************CTRL Begin*********************************** */
/* ��¼�����źŸ���  */
#define AT_ANTENNA_LEVEL_MAX_NUM 3

#define AT_RSSI_RESULT_CODE_NOT_REPORT_TYPE 0

#define AT_CURC_RPT_CFG_MAX_SIZE 8
#define AT_CURC_RPT_CFG_PARA_LEN 16

/* ********************************CTRL End*********************************** */

/*
 * ����ΪUART��غ궨��
 */
/* �¶���RING�Ų��θߵ͵�ƽ����ʱ��(ms) */
#define AT_UART_DEFAULT_SMS_RI_ON_INTERVAL 1000
#define AT_UART_DEFAULT_SMS_RI_OFF_INTERVAL 1000

/* ����RING�Ų��ε͵�ƽ����ʱ��(ms) */
#define AT_UART_DEFAULT_VOICE_RI_ON_INTERVAL 1000
#define AT_UART_DEFAULT_VOICE_RI_OFF_INTERVAL 4000

/* ����RING�Ų���ѭ������ */
#define AT_UART_DEFAULT_VOICE_RI_CYCLE_TIMES 10

#define AT_UART_DEFAULT_BAUDRATE (AT_UART_BAUDRATE_115200)
#define AT_UART_DEFAULT_FORMAT (AT_UART_FORMAT_8DATA_1STOP)
/*
 * ע: ��ͬ�汾ѡ�õ�UART IP��ͬ, Ĭ��У�鷽ʽ��������
 * V3R3            --- ��У�� SPACE
 * V7R11(or later) --- ��У�� ODD
 */
#define AT_UART_DEFAULT_PARITY (AT_UART_PARITY_ODD)

#define AT_UART_DEFAULT_FC_DCE_BY_DTE (AT_UART_FC_DCE_BY_DTE_NONE)
#define AT_UART_DEFAULT_FC_DTE_BY_DCE (AT_UART_FC_DTE_BY_DCE_NONE)

#if !defined(AT_ARRAY_SIZE)
#define AT_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

/* ��port��Ӧ��λ����Ϊ1 */
#define AT_SET_BIT32(n) ((VOS_UINT32)1 << (n))

#define AT_SET_BIT32_SELECT0(n) (((n) < 32) ? AT_SET_BIT32((n) % 32) : 0)

#define AT_SET_BIT32_SELECT1(n) ((((n) >= 32) && ((n) < 64)) ? AT_SET_BIT32((n) % 32) : 0)

#define AT_SET_BIT32_SELECT2(n) ((((n) >= 64) && ((n) < 96)) ? AT_SET_BIT32((n) % 32) : 0)

#define AT_CLIENT_CFG_ELEMENT(port)                                                                         \
    {                                                                                                       \
        AT_CLIENT_ID_##port, TAF_NV_CLIENT_CFG_##port, #port,                                               \
        {                                                                                                   \
            AT_SET_BIT32_SELECT0(TAF_NV_CLIENT_CFG_##port), AT_SET_BIT32_SELECT1(TAF_NV_CLIENT_CFG_##port), \
                AT_SET_BIT32_SELECT2(TAF_NV_CLIENT_CFG_##port)                                              \
        }                                                                                                   \
    }

#define AT_GET_CLIENT_CFG_TAB_LEN() (g_atClientCfgMapTabLen)

#define AT_PORT_LEN_MAX 20

/* WIFI nģʽWL���������ֵΪ0~7����8�� */
#define AT_WIFI_N_RATE_NUM 8

#define AT_RELEASE_R11 0x00000b00

#define AT_MAX_MSG_NUM 80

#define AT_TEST_ECC_FILE_NAME_MAX_LEN 75

#define AT_SET_SIMLOCK_DATA_HMAC_LEN 64

#define AT_SET_NV_CARRIER_NAME_LEN 48

#define AT_CONST_NUM_0 0
#define AT_CONST_NUM_1 1
#define AT_CONST_NUM_2 2
#define AT_CONST_NUM_3 3
#define AT_CONST_NUM_4 4
#define AT_CONST_NUM_5 5
#define AT_CONST_NUM_6 6
#define AT_CONST_NUM_22 22
#define AT_CONST_NUM_23 23
#define AT_CONST_NUM_24 24
#define AT_CONST_ONE 1
#define AT_CONST_TEN 10
#define AT_CONST_HUNDRED 100
#define AT_CONST_THOUSAND 1000
#define AT_CONST_TEN_THOUSAND 10000
#define AT_CONST_HUNDRED_THOUSAND 100000
#define AT_CONST_MILLION 1000000

#define AT_AP_PRESET_DPLMN_INVALID_RAT 0
#define AT_RPT_CFG_BIT32_LEN 3
#define AT_RPT_CFG_BIT32_INDEX_0 0
#define AT_RPT_CFG_BIT32_INDEX_1 1
#define AT_RPT_CFG_BIT32_INDEX_2 2
#define AT_MAX_IMSI_LEN 9
#define AT_DATA_DEFAULT_LENGTH 4

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#define AT_AP_PRESET_DPLMN_ALL_RAT 15
#else
#define AT_AP_PRESET_DPLMN_ALL_RAT 7
#endif

/* S-NSSAI��SD�����λ��Ϊ6 */
#define AT_S_NSSAI_SD_MAX_DIGIT_NUM 6
/* S-NSSAI�����������4����ֵ */
#define AT_S_NSSAI_MAX_DIGIT_NUM 4
/* S-NSSAI�ַ����������3���ָ��� */
#define AT_S_NSSAI_MAX_PUNC_NUM 3

#define AT_CELLID_STRING_MAX_LEN 17

#define AT_WIFI_CHANNEL_MAX_NUM 14 /* 802.11bgn��ʽ֧������ŵ��� */

#define AT_ECALL_ALACK_NUM 20
#define AT_CR_STR_LEN 1

#define AT_COMMAND_PARA_INDEX_0 0
#define AT_COMMAND_PARA_INDEX_1 1
#define AT_COMMAND_PARA_INDEX_2 2
#define AT_COMMAND_PARA_INDEX_3 3
#define AT_COMMAND_PARA_INDEX_4 4
#define AT_COMMAND_PARA_INDEX_5 5
#define AT_COMMAND_PARA_INDEX_6 6
#define AT_COMMAND_PARA_INDEX_7 7
#define AT_COMMAND_PARA_INDEX_8 8
#define AT_COMMAND_PARA_INDEX_9 9
#define AT_COMMAND_PARA_INDEX_10 10
#define AT_COMMAND_PARA_INDEX_11 11

#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)
#define AT_COMMAND_BIT_0 0
#define AT_COMMAND_BIT_1 1
#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern struct wakeup_source g_atWakeLock;
#endif

#if (FEATURE_CALL_WAKELOCK == FEATURE_ON)
extern struct wakeup_source g_callWakeLock;
#endif
#endif

/* ********************************TIMER Begin**************************** */
enum AT_InternalTimerType {
    AT_S0_TIMER       = 1,
    AT_HOLD_CMD_TIMER = 2,
    AT_SHUTDOWN_TIMER = 8,

    AT_VOICE_RI_TIMER = 9,
    AT_SMS_RI_TIMER   = 10,

    AT_AUTH_PUBKEY_TIMER = 11,

    AT_SIMLOCKWRITEEX_TIMER = 12,

#if (FEATURE_LTEV_WL == FEATURE_ON)
    LEDTEST_TIMER_NAME = 13,
    SFPTEST_TIMER_NAME = 14,
#endif
    AT_INTERNAL_TIMER_BUTT
};
/* ********************************TIMER End**************************** */

/* ********************************PORT Begin**************************** */

/* ********************************PROT End**************************** */

/* ********************************CTRL Begin**************************** */

enum AT_CmdAntennaLevel {
    AT_CMD_ANTENNA_LEVEL_0,
    AT_CMD_ANTENNA_LEVEL_1,
    AT_CMD_ANTENNA_LEVEL_2,
    AT_CMD_ANTENNA_LEVEL_3,
    AT_CMD_ANTENNA_LEVEL_4,
    AT_CMD_ANTENNA_LEVEL_BUTT
};
typedef VOS_UINT8 AT_CmdAntennaLevelUint8;
/* ********************************CTRL End*********************************** */

/* ********************************COMM Begin********************************* */
enum AT_ClckParaClass {
    AT_CLCK_PARA_CLASS_VOICE                   = 0x01,
    AT_CLCK_PARA_CLASS_DATA                    = 0x02,
    AT_CLCK_PARA_CLASS_FAX                     = 0x04,
    AT_CLCK_PARA_CLASS_VOICE_FAX               = 0x05,
    AT_CLCK_PARA_CLASS_VOICE_DATA_FAX          = 0x07,
    AT_CLCK_PARA_CLASS_SMS                     = 0x08,
    AT_CLCK_PARA_CLASS_FAX_SMS                 = 0x0C,
    AT_CLCK_PARA_CLASS_VOICE_FAX_SMS           = 0x0D,
    AT_CLCK_PARA_CLASS_VOICE_DATA_FAX_SMS      = 0x0F,
    AT_CLCK_PARA_CLASS_DATA_SYNC               = 0x10,
    AT_CLCK_PARA_CLASS_DATA_ASYNC              = 0x20,
    AT_CLCK_PARA_CLASS_DATA_PKT                = 0x40,
    AT_CLCK_PARA_CLASS_DATA_SYNC_PKT           = 0x50,
    AT_CLCK_PARA_CLASS_DATA_PAD                = 0x80,
    AT_CLCK_PARA_CLASS_DATA_ASYNC_PAD          = 0xA0,
    AT_CLCK_PARA_CLASS_DATA_SYNC_ASYNC_PKT_PKT = 0xF0,
    AT_CLCK_PARA_CLASS_ALL                     = 0xFF,
};
typedef VOS_UINT8 AT_ClckParaClassUint8;

/* ********************************COMM End*********************************** */

/* ********************************CC/SS Begin**************************** */

/* ********************************CC/SS End**************************** */

/* ********************************SMS Begin*********************************** */
enum AT_CmgfMsgFormat {
    AT_CMGF_MSG_FORMAT_PDU,
    AT_CMGF_MSG_FORMAT_TEXT
};
typedef TAF_UINT8 AT_CmgfMsgFormatUint8;

/* ********************************SMS End*********************************** */

/* ********************************NET Begin*********************************** */

enum AT_McsDirection {
    AT_MCS_DIRECTION_UL,
    AT_MCS_DIRECTION_DL,
    AT_MCS_DIRECTION_BUTT
};

typedef VOS_UINT8 AT_McsDirectionUint8;


enum AT_CeregType {
    AT_CEREG_RESULT_CODE_NOT_REPORT_TYPE, /* ��ֹ+CEREG �������ϱ� */
    AT_CEREG_RESULT_CODE_BREVITE_TYPE,    /* ����ע��״̬�����ı�ʱ��ʹ��+CEREG: <stat>�������ϱ���ʽ�����ϱ� */
    /* С����Ϣ�����ı�ʱ��ʹ��+CEREG:<stat>[,<tac>,<ci>[,<AcT>]]�������ϱ���ʽ�����ϱ� */
    AT_CEREG_RESULT_CODE_ENTIRE_TYPE,
    AT_CEREG_RESULT_CODE_TYPE_BUTT
};
typedef VOS_UINT8 AT_CeregTypeUint8;


enum AT_C5GREG_Type {
    AT_C5GREG_RESULT_CODE_NOT_REPORT_TYPE, /* ��ֹ+C5GREG �������ϱ� */
    AT_C5GREG_RESULT_CODE_BREVITE_TYPE,    /* ����ע��״̬�����ı�ʱ��ʹ��+C5GREG: <stat>�������ϱ���ʽ�����ϱ� */
    /* С����Ϣ�����ı�ʱ��ʹ��+C5GREG:<stat>[,<tac>,<ci>[,<AcT>]][<Allowed_NSSAI_length>,<Allowed_NSSAI>]�������ϱ���ʽ�����ϱ� */
    AT_C5GREG_RESULT_CODE_ENTIRE_TYPE,
    AT_C5GREG_RESULT_CODE_TYPE_BUTT
};
typedef VOS_UINT8 AT_C5GREG_TypeUint8;


enum AT_CerssiReportType {
    AT_CERSSI_REPORT_TYPE_NOT_REPORT,        /* ��ֹ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_1DB_CHANGE_REPORT, /* �ź������ı䳬��1dbʱ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_2DB_CHANGE_REPORT, /* �ź������ı䳬��2dbʱ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_3DB_CHANGE_REPORT, /* �ź������ı䳬��3dbʱ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_4DB_CHANGE_REPORT, /* �ź������ı䳬��4dbʱ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_5DB_CHANGE_REPORT, /* �ź������ı䳬��5dbʱ^CERSSI�������ϱ� */
    AT_CERSSI_REPORT_TYPE_BUTT
};
typedef VOS_UINT8 AT_CerssiReportTypeUint8;


enum AT_RptCmdIndex {
    AT_RPT_CMD_MODE = 0,
    AT_RPT_CMD_RSSI,
    AT_RPT_CMD_SRVST,
    AT_RPT_CMD_SIMST,
    AT_RPT_CMD_TIME,
    AT_RPT_CMD_SMMEMFULL,
    AT_RPT_CMD_CTZV,

    AT_RPT_CMD_CTZE,

    AT_RPT_CMD_DSFLOWRPT,
    AT_RPT_CMD_ORIG,
    AT_RPT_CMD_CONF,
    AT_RPT_CMD_CONN,
    AT_RPT_CMD_CEND,
    AT_RPT_CMD_STIN,

    AT_RPT_CMD_CERSSI,
    AT_RPT_CMD_ANLEVEL,
    AT_RPT_CMD_LWCLASH,
    AT_RPT_CMD_XLEMA,
    AT_RPT_CMD_ACINFO,

    AT_RPT_CMD_PLMN,

    /* call state���ϱ���CURC���� */
    AT_RPT_CMD_CALLSTATE,

    /* ��������������Ƶ������ϱ����� */
    AT_RPT_CMD_CREG,
    AT_RPT_CMD_CUSD,
    AT_RPT_CMD_CSSI,
    AT_RPT_CMD_CSSU,
    AT_RPT_CMD_LWURC,
    AT_RPT_CMD_CUUS1I,
    AT_RPT_CMD_CUUS1U,
    AT_RPT_CMD_CGREG,
    AT_RPT_CMD_CEREG,
    AT_RPT_CMD_C5GREG,

    AT_RPT_CMD_BUTT
};
typedef VOS_UINT8 AT_RptCmdIndexUint8;


enum AT_CmdRptCtrlType {
    AT_CMD_RPT_CTRL_BY_CURC,        /* �����ϱ���CURC���� */
    AT_CMD_RPT_CTRL_BY_UNSOLICITED, /* �����ϱ��ܵ������������ */
    AT_CMD_RPT_CTRL_BUTT
};
typedef VOS_UINT8 AT_CmdRptCtrlTypeUint8;

/*
 * Э����: ��
 * �ṹ˵��: WCDMAģʽ������ö��
 */
enum AT_RateWcdmaCategory {
    AT_RATE_WCDMA_R99         = 0,
    AT_RATE_WCDMA_CATEGORY_6  = 6,
    AT_RATE_WCDMA_CATEGORY_8  = 8,
    AT_RATE_WCDMA_CATEGORY_9  = 9,
    AT_RATE_WCDMA_CATEGORY_10 = 10,
    AT_RATE_WCDMA_CATEGORY_12 = 12,
    AT_RATE_WCDMA_CATEGORY_14 = 14,
    AT_RATE_WCDMA_CATEGORY_18 = 18,
    AT_RATE_WCDMA_CATEGORY_24 = 24,
    AT_RATE_WCDMA_CATEGORY_26 = 26,
    AT_RATE_WCDMA_CATEGORY_28 = 28
};
typedef VOS_UINT8 AT_RateWcdmaCategoryUint8;

/* VDF����: CREG/CGREG����<CI>���ֽ��ϱ���ö�� */
enum ELF_CREG_CgregCiRptByte {
    CREG_CGREG_CI_RPT_TWO_BYTE  = 0, /* CREG/CGREG��<CI>����2�ֽڷ�ʽ�ϱ� */
    CREG_CGREG_CI_RPT_FOUR_BYTE = 1, /* CREG/CGREG��<CI>����4�ֽڷ�ʽ�ϱ� */
    CREG_CGREG_CI_RPT_BUTT
};
typedef VOS_UINT8 CREG_CGREG_CI_RPT_BYTE_ENUM;
/* ********************************NET End*********************************** */

/* ********************************AGPS Begin*********************************** */


enum AT_CposrFlg {
    AT_CPOSR_DISABLE = 0, /* �����������ϱ� */
    AT_CPOSR_ENABLE,      /* ���������ϱ� */
    AT_CPOSR_BUTT
};
typedef VOS_UINT8 AT_CposrFlgUnit8;


enum AT_XcposrFlg {
    AT_XCPOSR_DISABLE = 0, /* �����������ϱ� */
    AT_XCPOSR_ENABLE,      /* ���������ϱ� */
    AT_XCPOSR_BUTT
};
typedef VOS_UINT8 AT_XcposrFlgUnit8;


enum AT_CmolreType {
    AT_CMOLRE_NUMERIC = 0, /* ���ָ�ʽ */
    AT_CMOLRE_VERBOSE,     /* �ַ�������ʽ */

    AT_CMOLRE_TYPE_BUTT
};
typedef VOS_UINT8 AT_CmolreTypeUnit8;


enum AT_XmlEndChar {
    AT_XML_CTRL_Z = 0, /* �����ַ�Ϊ��ctrl-Z�� */
    AT_XML_ESC,        /* �����ַ�Ϊ��ESC�� */
    AT_XML_BUTT
};
typedef VOS_UINT8 AT_XmlEndCharUnit8;


enum AT_XmlProcType {
    AT_XML_PROC_TYPE_FINISH   = 0, /* ������"ctrl-Z"��"ESC"    */
    AT_XML_PROC_TYPE_CONTINUE = 1, /* �����˻س���             */
    AT_XML_PROC_TYPE_BUTT
};
typedef VOS_UINT8 AT_XmlProcTypeUint8;
/* ********************************AGPS End*********************************** */


enum AT_AntState {
    AT_ANT_AIRWIRE_MODE    = 0, /* ����ģʽ */
    AT_ANT_CONDUCTION_MODE = 1, /* ����ģʽ */
    AT_ANT_MODE_BUTT
};
typedef VOS_UINT16 AT_AntStateUint16;


enum AT_UartBaudrate {
    AT_UART_BAUDRATE_0       = 0,       /* ��ʾ����������Ӧ */
    AT_UART_BAUDRATE_300     = 300,     /* ��Ӧ������Ϊ300 */
    AT_UART_BAUDRATE_600     = 600,     /* ��Ӧ������Ϊ600 */
    AT_UART_BAUDRATE_1200    = 1200,    /* ��Ӧ������Ϊ1200 */
    AT_UART_BAUDRATE_2400    = 2400,    /* ��Ӧ������Ϊ2400 */
    AT_UART_BAUDRATE_4800    = 4800,    /* ��Ӧ������Ϊ4800 */
    AT_UART_BAUDRATE_9600    = 9600,    /* ��Ӧ������Ϊ9600 */
    AT_UART_BAUDRATE_19200   = 19200,   /* ��Ӧ������Ϊ19200 */
    AT_UART_BAUDRATE_38400   = 38400,   /* ��Ӧ������Ϊ38400 */
    AT_UART_BAUDRATE_57600   = 57600,   /* ��Ӧ������Ϊ57600 */
    AT_UART_BAUDRATE_115200  = 115200,  /* ��Ӧ������Ϊ115200 */
    AT_UART_BAUDRATE_230400  = 230400,  /* ��Ӧ������Ϊ230400 */
    AT_UART_BAUDRATE_250000  = 250000,  /* ��Ӧ������Ϊ250000 */
    AT_UART_BAUDRATE_300000  = 300000,  /* ��Ӧ������Ϊ300000 */
    AT_UART_BAUDRATE_375000  = 375000,  /* ��Ӧ������Ϊ375000 */
    AT_UART_BAUDRATE_460800  = 460800,  /* ��Ӧ������Ϊ460800 */
    AT_UART_BAUDRATE_600000  = 600000,  /* ��Ӧ������Ϊ600000 */
    AT_UART_BAUDRATE_750000  = 750000,  /* ��Ӧ������Ϊ750000 */
    AT_UART_BAUDRATE_921600  = 921600,  /* ��Ӧ������Ϊ921600 */
    AT_UART_BAUDRATE_1000000 = 1000000, /* ��Ӧ������Ϊ1000000 */
    AT_UART_BAUDRATE_1152000 = 1152000, /* ��Ӧ������Ϊ1152000 */
    AT_UART_BAUDRATE_1500000 = 1500000, /* ��Ӧ������Ϊ1500000 */
    AT_UART_BAUDRATE_2000000 = 2000000, /* ��Ӧ������Ϊ2000000 */
    AT_UART_BAUDRATE_2250000 = 2250000, /* ��Ӧ������Ϊ2250000 */
    AT_UART_BAUDRATE_2764800 = 2764800, /* ��Ӧ������Ϊ2764800 */
    AT_UART_BAUDRATE_3000000 = 3000000, /* ��Ӧ������Ϊ3000000 */
    AT_UART_BAUDRATE_4000000 = 4000000, /* ��Ӧ������Ϊ4000000 */

    AT_UART_BAUDRATE_BUTT
};
typedef VOS_UINT32 AT_UartBaudrateUint32;


enum AT_UartFormat {
    AT_UART_FORMAT_AUTO_DETECT         = 0x00, /* auto detect */
    AT_UART_FORMAT_8DATA_2STOP         = 0x01,
    AT_UART_FORMAT_8DATA_1PARITY_1STOP = 0x02,
    AT_UART_FORMAT_8DATA_1STOP         = 0x03,
    AT_UART_FORMAT_7DATA_2STOP         = 0x04,
    AT_UART_FORMAT_7DATA_1PARITY_1STOP = 0x05,
    AT_UART_FORMAT_7DATA_1STOP         = 0x06,

    AT_UART_FORMAT_BUTT
};
typedef VOS_UINT8 AT_UartFormatUint8;


enum AT_UartParity {
    AT_UART_PARITY_ODD   = 0x00, /* ODD �� */
    AT_UART_PARITY_EVEN  = 0x01, /* EVEN ż */
    AT_UART_PARITY_MARK  = 0x02, /* MARK */
    AT_UART_PARITY_SPACE = 0x03, /* SPACE */

    AT_UART_PARITY_BUTT
};
typedef VOS_UINT8 AT_UartParityUint8;


enum AT_UartDataLen {
    AT_UART_DATA_LEN_5_BIT = 5,
    AT_UART_DATA_LEN_6_BIT = 6,
    AT_UART_DATA_LEN_7_BIT = 7,
    AT_UART_DATA_LEN_8_BIT = 8,

    AT_UART_DATA_LEN_BUTT
};
typedef VOS_UINT8 AT_UartDataLenUint8;


enum AT_UartStopLen {
    AT_UART_STOP_LEN_1_BIT = 1,
    AT_UART_STOP_LEN_2_BIT = 2,

    AT_UART_STOP_LEN_BUTT
};
typedef VOS_UINT8 AT_UartStopLenUint8;


enum AT_UartParityLen {
    AT_UART_PARITY_LEN_0_BIT = 0,
    AT_UART_PARITY_LEN_1_BIT = 1,

    AT_UART_PARITY_LEN_BUTT
};
typedef VOS_UINT8 AT_UartParityLenUint8;


enum AT_UartFcDceByDte {
    AT_UART_FC_DCE_BY_DTE_NONE               = 0x00,
    AT_UART_FC_DCE_BY_DTE_XON_OR_XOFF_REMOVE = 0x01,
    AT_UART_FC_DCE_BY_DTE_RTS                = 0x02,
    AT_UART_FC_DCE_BY_DTE_XON_OR_XOFF_PASS   = 0x03,

    AT_UART_FC_DCE_BY_DTE_BUTT
};
typedef VOS_UINT8 AT_UartFcDceByDteUint8;


enum AT_UartFcDteByDce {
    AT_UART_FC_DTE_BY_DCE_NONE               = 0x00,
    AT_UART_FC_DTE_BY_DCE_XON_OR_XOFF_REMOVE = 0x01,
    AT_UART_FC_DTE_BY_DCE_CTS                = 0x02,

    AT_UART_FC_DTE_BY_DCE_BUTT
};
typedef VOS_UINT8 AT_UartFcDteByDceUint8;


enum AT_UartRiType {
    AT_UART_RI_TYPE_VOICE = 0x00,
    AT_UART_RI_TYPE_SMS   = 0x01,

    AT_UART_RI_TYPE_BUTT
};
typedef VOS_UINT32 AT_UartRiTypeUint32;


enum AT_UartRiStatus {
    AT_UART_RI_STATUS_STOP    = 0x00,
    AT_UART_RI_STATUS_RINGING = 0x01,

    AT_UART_RI_STATUS_BUTT
};
typedef VOS_UINT8 AT_UartRiStatusUint8;


enum AT_PortBuffCfg {
    AT_PORT_BUFF_DISABLE = 0,
    AT_PORT_BUFF_ENABLE,

    AT_PORT_BUFF_BUTT
};

typedef VOS_UINT8 AT_PortBuffCfgUint8;

typedef VOS_VOID (*AT_UART_WM_LOW_FUNC)(AT_ClientIdUint16 clientId);


enum AT_CnapType {
    AT_CNAP_DISABLE_TYPE = 0,
    AT_CNAP_ENABLE_TYPE  = 1,
    AT_CNAP_TYPE_BUTT,
};
typedef VOS_UINT8 AT_CnapTypeUint8;

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
enum AT_NoCardMode {
    AT_NOCARD_MODE_OFF = 0,
    AT_NOCARD_MODE_ON = 1,
    AT_NOCARD_MODE_BUTT,
};
typedef VOS_UINT32 AT_NoCardModeTypeUint32;
#endif
#endif


enum AT_StreamProcType {
    AT_STREAM_PROC_TYPE_FINISH   = 0, /* ������"ctrl-Z"    */
    AT_STREAM_PROC_TYPE_CONTINUE = 1, /* �����˻س���      */
    AT_STREAM_PROC_TYPE_CANCEL   = 2, /* ������"ESC"       */
    AT_STREAM_PROC_TYPE_BUTT
};
typedef VOS_UINT8 AT_StreamProcTypeUint8;

#if (defined(LLT_OS_VER))
typedef unsigned long AT_SPINLOCK;
#else
typedef VOS_SPINLOCK AT_SPINLOCK;
#endif


typedef struct {
    VOS_UINT32 modem0Pid;
    VOS_UINT32 modem1Pid;
    VOS_UINT32 modem2Pid;
    VOS_UINT32 reserved;
} AT_ModemPidTab;

/* ********************************Modem CTX Begin**************************** */

typedef struct {
    TAF_MMA_UsimmCardTypeUint32 cardType;                              /* ������:SIM��USIM */
    USIMM_CardAppServicUint32   cardStatus;                            /* ��״̬ */
    USIMM_CardMediumTypeUint32  cardMediumType;                        /* ��MEDIUM���� */
    VOS_UINT8                   imsi[AT_MAX_IMSI_LEN]; /* IMSI�� */
    VOS_UINT8                   imsiLen;                               /* IMSI���� */
    VOS_UINT8                   reserve[2];
    TAF_MMA_SimsqStateUint32    simsqStatus;
    VOS_UINT32 noCardMode;
} AT_USIM_InfoCtx;


typedef struct {
    VOS_UINT8 platformSptLte;
    VOS_UINT8 platformSptWcdma;
    VOS_UINT8 platformSptGsm;
    VOS_UINT8 platformSptUtralTdd;
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    VOS_UINT8 platformSptNr;
    VOS_UINT8 reserved[3];
#else
    VOS_UINT8 aucReserved[4];
#endif
} AT_ModemSptRat;


typedef struct {
    AT_ModemSptRat platformRatList;
    VOS_UINT8      isClMode;
    VOS_UINT8      reserved[7];
} AT_ModemSptCap;

/* ********************************CTRL CTX Begin**************************** */
typedef struct {
    /* NV�Ƿ񼤻��־, 0: �����1: ���� */
    VOS_UINT8 status;
    VOS_UINT8 atStatusReportGeneralControl; /* ˽�������Ƿ�����״̬�ϱ� */
} NVIM_AT_PrivateCmdStatusRpt;


typedef struct {
    AT_ClckParaClassUint8  classType;
    TAF_BASIC_SERVICE_TYPE serviceType;
    TAF_BASIC_SERVICE_CODE serviceCode;
} AT_ClckClassServiceTbl;

/* ********************************CTRL CTX Begin**************************** */

/* ********************************CC/SS CTX Begin**************************** */

typedef MN_CALL_CsDataCfg AT_CBST_DATA_CFG_STRU;

typedef struct {
    HTIMER     s0Timer;
    VOS_BOOL   timerStart;
    VOS_UINT16 clientId;
    VOS_UINT8  s0TimerLen;
    VOS_UINT8  reserved; /* ���� */
    VOS_UINT32 timerName;
    VOS_UINT8  reserved1[4];
} AT_S0_TimerInfo;

typedef MN_CALL_CugCfg AT_CCUG_CFG_STRU;

/* ********************************CC/SS CTX End**************************** */

/* ********************************SMS CTX Begin**************************** */
typedef struct {
    AT_CNMI_MODE_TYPE cnmiModeType;
    AT_CNMI_MT_TYPE   cnmiMtType;
    AT_CNMI_BM_TYPE   cnmiBmType;
    AT_CNMI_DS_TYPE   cnmiDsType;
    AT_CNMI_BFR_TYPE  cnmiBfrType;
    AT_CNMI_MODE_TYPE cnmiTmpModeType;
    AT_CNMI_MT_TYPE   cnmiTmpMtType;
    AT_CNMI_BM_TYPE   cnmiTmpBmType;
    AT_CNMI_DS_TYPE   cnmiTmpDsType;
    AT_CNMI_BFR_TYPE  cnmiTmpBfrType;
    VOS_UINT8         reserved[6];
} AT_CnmiType;

typedef struct {
    MN_MSG_EventInfo event[AT_BUFFER_CBM_EVENT_MAX];
    VOS_UINT8        used[AT_BUFFER_CBM_EVENT_MAX];
    VOS_UINT8        index;
    VOS_UINT8        reserved[2];
} AT_CbmBuffer;


typedef struct {
    VOS_UINT8               actFlg;
    MN_MSG_MtCustomizeUint8 mtCustomize;
    VOS_UINT8               reserved[6];
} AT_SMS_MtCustomize;

typedef struct {
    VOS_BOOL  used;
    VOS_UINT8 fo;
    VOS_UINT8 tmpFo;
    VOS_UINT8 reserve1[2];
} AT_MSG_Fo;

typedef struct {
    VOS_BOOL  foUsed;
    VOS_UINT8 fo;
    VOS_UINT8 tmpFo;
    VOS_UINT8 defaultSmspIndex;
    VOS_UINT8 reserve1[1];
    MN_MSG_ValidPeriod vp;
    MN_MSG_ValidPeriod tmpVp;
    AT_MSG_Fo          atMsgFo;
    MN_MSG_SrvParam    parmInUsim;
} AT_CSCA_CsmpInfo;

typedef struct {
    MN_MSG_MemStoreUint8      memReadorDelete;
    MN_MSG_MemStoreUint8      memSendorWrite;
    MN_MSG_MemStoreUint8      tmpMemReadorDelete;
    MN_MSG_MemStoreUint8      tmpMemSendorWrite;
    VOS_UINT8                 reserved[4];
    MN_MSG_SetRcvmsgPathParm  rcvPath;
    MN_MSG_StorageListEvtInfo usimStorage;
    MN_MSG_StorageListEvtInfo nvimStorage;
} AT_MSG_Cpms;

typedef struct {
    TAF_BOOL           used;
    VOS_UINT8          reserved[4];
    MN_MSG_EventInfo  *event;
    MN_MSG_TsDataInfo *tsDataInfo;
} AT_SMS_MtBuffer;

typedef struct {
    MN_MSG_EventInfo event[AT_BUFFER_SMT_EVENT_MAX];
    VOS_UINT8        used[AT_BUFFER_SMT_EVENT_MAX];
    VOS_UINT8        index;
    VOS_UINT8        reserved[2];
} AT_SMT_Buffer;

/* ********************************SMS CTX End**************************** */

/* ********************************NET CTX Begin**************************** */
typedef struct {
    AT_COPS_FORMAT_TYPE formatType;
} AT_COPS_Type;

/* ********************************NET CTX End**************************** */

/* ********************************AGPS CTX Begin**************************** */
/*
 * �ṹ˵��: ATģ��XML�ı��ṹ
 */
typedef struct {
    VOS_CHAR *xmlTextHead; /* XML�����洢�� */
    VOS_CHAR *xmlTextCur;  /* ָʾ��ǰXML�ı����� */
} AT_XML;

/* ********************************AGPS CTX End**************************** */


typedef struct {
    VOS_UINT8               numOfCalls; /* ��������ͨ���ĸ��� */
    VOS_UINT8               reserved[7];
    TAF_CALL_EconfInfoParam callInfo[TAF_CALL_MAX_ECONF_CALLED_NUM]; /* ÿһ����Ա����Ϣ */
} AT_EconfInfo;


typedef struct {
    /*
     * 1 DD-MMM-YYYY
     * 2 DD-MM-YY
     * 3 MM/DD/YY
     * 4 DD/MM/YY
     * 5 DD.MM.YY
     * 6 YYMMDD
     * 7 YY-MM-DD
     */
    VOS_UINT8 mode;
    VOS_UINT8 auxMode;
    VOS_UINT8 reserved[6];
} AT_CsdfCfg;


typedef struct {
    VOS_UINT8 mode;
    VOS_UINT8 bfr;
    VOS_UINT8 reserved[6];
} AT_CgerepCfg;


typedef struct {
    VOS_UINT32         curIsExistCallFlag; /* ��ǰ�Ƿ���ں��б�־ */
    TAF_CS_CauseUint32 csErrCause;         /* ���ڼ�¼CS������� */

    /* �Զ�Ӧ��ʱ�䣬0��ʾ�ر��Զ�Ӧ��1~255�������Զ�Ӧ���ʱ�� */
    AT_S0_TimerInfo s0TimeInfo;

    AT_EconfInfo econfInfo;

    /* SIP CANCEL��Ϣ����ԭ��ֵ��Ӧ���ı���Ϣ */
    TAF_CALL_ErrorInfoText errInfoText;

    /* CVHU�������ƣ������Ƿ�֧��ATH�Ҷ����� */
    AT_CvhuModeUint8 cvhuMode;
    VOS_UINT8        reserved[3];

} AT_ModemCcCtx;


typedef struct {
    VOS_UINT16        ussdTransMode; /* USSD����ģʽ */
    AT_CMOD_MODE_ENUM cModType;      /* ����ģʽ���� */
    AT_SALS_TYPE      salsType;      /* ʹ��/��ֹ��·ѡ�� */
    AT_CLIP_TYPE      clipType;      /* ʹ��/��ֹ���к�����ʾ */
    AT_CLIR_TYPE      clirType;      /* ʹ��/��ֹ���к������� */
    AT_COLP_TYPE      colpType;      /* ʹ��/��ֹ���Ӻ�����ʾ */
    AT_CRC_TYPE       crcType;       /* ʹ��/��ֹ����ָʾ��չ�ϱ���ʽ */
    AT_CCWA_TYPE      ccwaType;      /* ʹ��/��ֹ���еȴ� */
    AT_CnapTypeUint8  cnapType;      /* ʹ��/��ֹ����������ʾ */
    VOS_UINT8         reserved[6];

    AT_CBST_DATA_CFG_STRU cbstDataCfg; /* ����(CBST)���� */
    AT_CCUG_CFG_STRU      ccugCfg;     /* �պ��û�Ⱥ(CUG)���� */
} AT_ModemSsCtx;


typedef struct {
    AT_CmgfMsgFormatUint8       cmgfMsgFormat;      /* ���Ÿ�ʽ TXT or PDU */
    AT_CSDH_TYPE                csdhType;           /* �ı�ģʽ�²�����ʾ���� */
    VOS_UINT8                   paraCmsr;           /* ��ȡ���ű�� */
    VOS_UINT8                   smsAutoReply;       /* �Զ��ظ���־ */
    MN_MSG_CsmsMsgVersionUint8  csmsMsgVersion;     /* ���ŷ������� */
    MN_MSG_MeStorageStatusUint8 msgMeStorageStatus; /* ME�洢״̬��־ */
    VOS_UINT8                   localStoreFlg;
    VOS_UINT8                   reserve2;

    AT_CnmiType      cnmiType;                         /* CNMI���� */
    AT_CSCA_CsmpInfo cscaCsmpInfo;                     /* �ı�ģʽ���� */
    AT_MSG_Cpms      cpmsInfo;                         /* ���Ŵ洢�� */
    AT_SMS_MtBuffer  smsMtBuffer[AT_SMSMT_BUFFER_MAX]; /* �յ����ŵĻ��� */
    /* ���Ż�״̬���治�洢ֱ���ϱ�PDU���� */
    AT_SMT_Buffer smtBuffer;

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
    AT_CbmBuffer          cbmBuffer;
    TAF_CBA_CbmiRangeList cbsDcssInfo; /* ���������Ϣֱ�ӱ�����AT�� */
#endif
    AT_SMS_MtCustomize smMeFullCustomize; /* ���Ž��ն������� */
} AT_ModemSmsCtx;


typedef struct {
    /* �����ϱ���� */
    /* VOS_TRUE: ^CERSSI�ϱ���VOS_FALSE: ���ϱ� */
    AT_CerssiReportTypeUint8 cerssiReportType;
    /* VOS_TRUE: CS�����״̬�ϱ���VOS_FALSE: ���ϱ� */
    AT_CREG_TYPE cregType;
    /* VOS_TRUE: PS�����״̬�ϱ���VOS_FALSE: ���ϱ� */
    AT_CGREG_TYPE       cgregType;
    AT_COPS_FORMAT_TYPE copsFormatType; /* COPS����PLMN�ĸ�ʽ */

    MN_PH_PrefPlmnTypeUint8 prefPlmnType;   /* CPOL����PLMN������ */
    AT_COPS_FORMAT_TYPE     cpolFormatType; /* CPOL����PLMN�ĸ�ʽ */
    VOS_UINT8               roamFeature;    /* ���������Ƿ�ʹ�� */
    VOS_UINT8               spnType;        /* ��¼AT^SPN��ѯ��SPN���� */

    AT_CmdAntennaLevelUint8 calculateAntennaLevel;                  /* ���ʹ��������߸��� */
    AT_CmdAntennaLevelUint8 antennaLevel[AT_ANTENNA_LEVEL_MAX_NUM]; /* ��¼3�����߸��� */
    VOS_UINT8               cerssiMinTimerInterval;                 /* �ź��ϱ�����С���ʱ�� */
    VOS_UINT8               reserve1[3];

    NAS_MM_InfoInd timeInfo; /* ʱ����Ϣ */

#if (FEATURE_LTE == FEATURE_ON)
    /* VOS_TRUE: EPS�����״̬�ϱ���VOS_FALSE: ���ϱ� */
    AT_CeregTypeUint8 ceregType;
    VOS_UINT8         reserve2[7];
#endif

    AT_CsdfCfg csdfCfg;

    AT_CgerepCfg cgerepCfg;
    VOS_UINT32   nwSecond;

    AT_McsDirectionUint8 mcsDirection;
    VOS_UINT8            reserve3[3];

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_C5GREG_TypeUint8 c5gregType;
    VOS_UINT8           reserve4[7];
#endif
} AT_ModemNetCtx;


typedef struct {
    /* +CPOSR���������ϱ����ƣ��ϵ翪��Ĭ��Ϊ�����������ϱ� */
    AT_CposrFlgUnit8 cposrReport;
    /* +XCPOSR���������ϱ����ƣ��ϵ翪��Ĭ��Ϊ�����������ϱ� */
    AT_XcposrFlgUnit8 xcposrReport;
    /* +CMOLRE����������ϱ���ʽ��Ĭ��Ϊ���ָ�ʽ */
    AT_CmolreTypeUnit8 cmolreType;
    VOS_UINT8          atCgpsClockEnableFlag; /* AT^CGPSCLOCK�е�һ������ */
    VOS_UINT8          reserve[4];

    /* XML�����ṹ���洢AGPS�·���������Ϣ */
    AT_XML xml;
} AT_ModemAgpsCtx;


typedef struct {
    AT_IMSA_BatteryStatusUint8 tempBatteryInfo;
    AT_IMSA_BatteryStatusUint8 currBatteryInfo;
    VOS_UINT8                  reserved[6];
} AT_ModemImsBatteryinfo;


typedef struct {
    AT_ModemImsBatteryinfo batteryInfo;
} AT_ModemImsContext;

typedef struct {
    PS_URSP_ProtocalVersionUint32 protocalVer;
    VOS_UINT8                 pti;
    TAF_PS_PolicyMsgTypeUint8 uePolicyMsgType;
    VOS_UINT8                 ueOsIdLen;
    VOS_UINT8                 classMarkLen;
    VOS_UINT8                 classMark[TAF_PS_UE_POLICY_CLASSMARK_MAX_LEN];
    VOS_UINT8                 ueOsIdInfo[TAF_PS_UE_OS_ID_INFO_MAX_LEN];
    VOS_UINT32                uePolicyLen;
    VOS_UINT8                *uePolicyHead; /* UE POLICY�����洢�� */
    VOS_UINT8                *uePolicyCur;  /* ָʾ��ǰUE POLICY�����洢ƫ��λ�� */
    VOS_UINT32                imsUrspVer;
    VOS_UINT32                imsUrspLen;
    VOS_UINT8                *imsUrspHead; /* UE POLICY�����洢�� */
    VOS_UINT8                *imsUrspCur;  /* ָʾ��ǰUE POLICY�����洢ƫ��λ�� */
} AT_ModemUePolicyCtx;


typedef struct {
    /* �濨��Ӫ�����ƣ��ַ������ͣ���󳤶�Ϊ48�ֽڡ� */
    VOS_UINT8  carrierName[AT_SET_NV_CARRIER_NAME_LEN];
    /*
     * ������Ч���ԣ�����ֵ��ȡֵ��Χ0~2��
     * 0�����ػ���NVREFERSH��Ч��
     * 1������sim����Ч��
     * 2������modem��Ч��
     */
    VOS_UINT8  specialEffectiveFlg;
    VOS_UINT8  rsv[7];
} AT_ModemNvloadCtx;

/* ********************************Modem CTX End**************************** */

/* ********************************Client CTX Begin**************************** */

typedef struct {
    ModemIdUint16 modemId; /* �ö˿������ĸ�modem */
    /* �ö˿��Ƿ����������ϱ���VOS_TRUEΪ����VOS_FALSEΪ������Ĭ������ */
    VOS_UINT8 reportFlg;
    VOS_UINT8 rsv[1];
} AT_ClientConfiguration;
/* ********************************Client CTX End**************************** */


typedef struct {
    VOS_UINT32 sarReduction;
    VOS_UINT8  rsv[4];
} AT_ModemMtInfoCtx;

/*
 * �ṹ˵��: MBB ������Ϣ������
 */
typedef struct {
    VOS_UINT8 voipApnKey; /* ��³Claro VOIP APN KEY���� */
    VOS_UINT8 dialStickFlg; /* stick�������� */
    VOS_UINT8 dtmfCustFlg; /* DTMF�������� */
    VOS_UINT8 ssAbortEnable; /* ����ҵ����ж� */
    VOS_UINT8 smsAbortEnable; /* ����ҵ����ж� */
    VOS_UINT8 standbyNoReportFlag; /* A�����߲����Ѷ��� */
    VOS_UINT8 rsv[2];
} AT_ModemMbbCustmizeCtx;


typedef struct {
    VOS_UINT8 filterEnableFlg; /* ��˽���˹����Ƿ�ʹ�ܱ�ʶ */
    VOS_UINT8 reserved[7];
} AT_ModemPrivacyFilterCtx;


typedef struct {
    /* ����CDMAMODEMSWITCH���������� */
    VOS_UINT8 enableFlg;
    VOS_UINT8 reserved[7]; /* ����λ */
} AT_ModemCdmamodemswitchCtx;

/* ********************************Modem CTX End**************************** */


typedef struct {
    VOS_SEM    hResetSem;    /* �������ź��������ڸ�λ����  */
    VOS_UINT32 resetingFlag; /* ��λ��־ */
    VOS_UINT32 reserved;
} AT_ResetCtx;


typedef struct {
    AT_UartFormatUint8    format;
    AT_UartDataLenUint8   dataBitLength;
    AT_UartStopLenUint8   stopBitLength;
    AT_UartParityLenUint8 parityBitLength;

} AT_UartFormatParam;


typedef struct {
    AT_UartFormatUint8 format;
    AT_UartParityUint8 parity;
    VOS_UINT8          reserved[2];

} AT_UartFrame;


typedef struct {
    AT_UartBaudrateUint32 baudRate;
    AT_UartFrame          frame;

} AT_UartPhyCfg;


typedef struct {
    AT_UartFcDceByDteUint8 dceByDte;
    AT_UartFcDteByDceUint8 dteByDce;
    VOS_UINT8              reserved[2];

} AT_UartFlowCtrl;


typedef struct {
    VOS_UINT32 smsRiOnInterval;  /* ����RI���߳���ʱ��(ms) */
    VOS_UINT32 smsRiOffInterval; /* ����RI���ͳ���ʱ��(ms) */

    VOS_UINT32 voiceRiOnInterval;  /* ����RI���߳���ʱ��(ms) */
    VOS_UINT32 voiceRiOffInterval; /* ����RI���ͳ���ʱ��(ms) */
    VOS_UINT8  voiceRiCycleTimes;  /* ����RI�������ڴ���     */
    VOS_UINT8  reserved[7];

} AT_UartRiCfg;


typedef struct {
    VOS_UINT32          runFlg; /* ��ʾ�Ƿ��ж��Ż��������� */
    AT_UartRiTypeUint32 type;   /* RI ���� */

    HTIMER                hVoiceRiTmrHdl;    /* ����RI��ʱ����� */
    AT_TIMER_StatusUint32 voiceRiTmrStatus;  /* ����RI��ʱ��״̬ */
    VOS_UINT32            voiceRiCycleCount; /* �����ϱ����ڼ��� */
    AT_UartRiStatusUint8  voiceRiStatus[MN_CALL_MAX_NUM + 1];
    /* ����RI״̬(���CALLID) */

    HTIMER                hSmsRiTmrHdl;     /* ����RI��ʱ����� */
    AT_TIMER_StatusUint32 smsRiTmrStatus;   /* ����RI��ʱ��״̬ */
    VOS_UINT32            smsRiOutputCount; /* ����RI������� */

} AT_UartRiStateInfo;


typedef struct {
    AT_UartPhyCfg       phyConfig;   /* UART �������� */
    AT_UartFlowCtrl     flowCtrl;    /* UART ����ģʽ */
    VOS_UINT32          reserve;
    AT_UartRiCfg        riConfig;    /* UART RI ���� */
    AT_UartRiStateInfo  riStateInfo; /* UART RI ״̬ */
    AT_UART_WM_LOW_FUNC wmLowFunc;   /* UART TX��ˮ�ߴ�����ָ�� */
} AT_UartCtx;


typedef struct {
    AT_PortBuffCfgUint8 smsBuffCfg;
    VOS_UINT8           num; /* ��ǰ���еĸ��� */
    VOS_UINT8           rcv[6];
    VOS_UINT32          usedClientId[AT_MAX_CLIENT_NUM]; /* �Ѿ�ʹ�õ�client ID��¼ */
} AT_PortBuffCfgInfo;


typedef struct {
    VOS_UINT16 currIdx;       /* �绰����ȡ��ǰ���� */
    VOS_UINT16 lastIdx;       /* �绰����ȡĿ������ */
    VOS_UINT32 singleReadFlg; /* �绰����ȡ������ʶ */
} AT_CommPbCtx;


typedef struct {
    VOS_UINT8  clientId;
    VOS_UINT8  settingFlag;
    VOS_UINT8  curIdx;
    VOS_UINT8  totalNum;
    VOS_UINT16 paraLen;
    VOS_UINT16 reserve;
    VOS_UINT8 *data;
    HTIMER     hAuthPubkeyProtectTimer;
} AT_AUTH_PUBKEYEX_CMD_PROC_CTX;


typedef struct {
    VOS_UINT8          *typeValue;
    VOS_UINT8          *phynumValue;
    VOS_UINT8          *hmacValue;
    VOS_UINT16          typeValueLen;
    VOS_UINT16          phynumValueLen;
    VOS_UINT16          hmacValueLen;
    VOS_UINT8           reserve[2];
} AT_PhonePhynumCmdProcCtx;


typedef struct {
    AT_SimlockTypeUint8 netWorkFlg;
    VOS_UINT8           reserve[7];
    VOS_UINT8           clientId;
    VOS_UINT8           settingFlag;
    VOS_UINT8           layer;
    VOS_UINT8           curIdx;
    VOS_UINT8           totalNum;
    VOS_UINT8           hmacLen;
    VOS_UINT16          simlockDataLen;
    VOS_UINT8          *data;
    VOS_UINT8           hmac[AT_SET_SIMLOCK_DATA_HMAC_LEN];
    HTIMER              hSimLockWriteExProtectTimer;
} AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX;


typedef struct {
    VOS_UINT32 reportedModemNum;
    VOS_UINT32 result;
} AT_VmsetCmdCtx;

typedef struct {
    AT_AUTH_PUBKEYEX_CMD_PROC_CTX      authPubkeyExCmdCtx;
    AT_VmsetCmdCtx                     vmSetCmdCtx;
    AT_PhonePhynumCmdProcCtx           phonePhynumCmdCtx;
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX simLockWriteExCmdCtx;
} AT_CmdProcCtx;


typedef struct {
    AT_SPINLOCK spinLock;
    VOS_UINT32  msgCount;
    VOS_UINT8   reserve[4];
} AT_CmdMsgNumCtrl;

typedef struct {
    VOS_UINT8      ecallAlackValue;
    VOS_UINT8      reserved[3];
    NAS_MM_InfoInd ecallAlackTimeInfo;
} AT_ECALL_AlackValue;


typedef struct {
    VOS_UINT8           ecallAlackNum;
    VOS_UINT8           ecallAlackBeginNum;
    VOS_UINT8           reserved[2];
    AT_ECALL_AlackValue ecallAlackInfo[AT_ECALL_ALACK_NUM];
} AT_ECALL_AlackInfo;

typedef struct {
    AT_ECALL_AlackInfo ecallAlackInfo;
    VOS_UINT8          ecallMode;
    VOS_UINT8          reserved[3];
} AT_CmdEcallCtx;


typedef struct {
    VOS_UINT8          mtSmsTrustListEnable;
    VOS_UINT8          mtCallTrustListEnable;
    VOS_UINT8          reserved[6];
} AT_MtTrustListCtrl;


typedef struct {
    VOS_UINT8 systemAppConfigAddr; /* �����̨�汾 */
    VOS_UINT8 reserve[7];

    AT_CommPsCtx       psCtx;   /* PS����صĹ��������� */
    AT_UartCtx         uartCtx; /* UART��������� */
    AT_PortBuffCfgInfo portBuffCfg;

    AT_CommPbCtx commPbCtx; /* �绰����ع��������� */

    AT_CmdProcCtx cmdProcCtx;

    AT_CmdMsgNumCtrl msgNumCtrlCtx;

    NAS_NVIM_CustomUsimmCfg customUsimmCfg;
    VOS_UINT8               rsv[4];
    AT_CmdEcallCtx          ecallAtCtx;

    AT_MtTrustListCtrl      mtTrustListCtrl;
} AT_CommCtx;


typedef struct {
    AT_USIM_InfoCtx          atUsimInfoCtx;   /* ��״̬ */
    AT_ModemSptCap           platformCapList; /* AT Modem����ģʽ */
    AT_ModemCcCtx            ccCtx;           /* ������ص������� */
    AT_ModemSsCtx            ssCtx;           /* ����ҵ����ص������� */
    AT_ModemSmsCtx           smsCtx;          /* ������ص������� */
    AT_ModemNetCtx           netCtx;          /* ������ص������� */
    AT_ModemAgpsCtx          agpsCtx;         /* AGPS��ص������� */
    AT_ModemPsCtx            psCtx;           /* PS��ص������� */
    AT_ModemMtInfoCtx        mtInfoCtx;       /* �ն���Ϣ�������� */
    AT_ModemImsContext       atImsCtx;        /* IMS��Ϣ�������� */
    AT_ModemPrivacyFilterCtx filterCtx;       /* ��˽��Ϣ���˵������� */
    AT_ModemCdmamodemswitchCtx cdmaModemSwitchCtx; /* CdmaModemSwitch���������������� */
    AT_ModemUePolicyCtx uePolicyCtx;
    AT_ModemNvloadCtx   atNvloadCtx;        /* NVLOAD��Ϣ�������� */
    AT_ModemMbbCustmizeCtx   mbbCustCtx;      /* Mbb��ض��������� */
} AT_ModemCtx;


typedef struct {
    AT_ClientConfiguration clientConfiguration; /* ÿ���˿ڵ�������Ϣ */
} AT_ClientCtx;

/* ********************************������Ҫ������************************************ */
/* ********************************Other Begin**************************** */

enum AT_E5_RightFlag {
    AT_E5_RIGHT_FLAG_NO,
    AT_E5_RIGHT_FLAG_YES,
    AT_E5_RIGHT_FLAG_BUTT
};
typedef VOS_UINT32 AT_E5_RightFlagUint32;

enum AT_RightOpenFlag {
    AT_RIGHT_OPEN_FLAG_CLOSE,
    AT_RIGHT_OPEN_FLAG_OPEN,
    AT_RIGHT_OPEN_FLAG_BUTT
};
typedef VOS_UINT32 AT_RightOpenFlagUint32;

#define AT_RIGHT_PWD_LEN 16

typedef struct {
    AT_RightOpenFlagUint32 rightOpenFlg;
    VOS_INT8               password[AT_RIGHT_PWD_LEN];
} AT_RightOpenFlagInfo;


typedef struct {
    /* AT��Ͽ��ر�־. VOS_TRUE: ʹ��; VOS_FALE: δʹ�� */
    VOS_UINT8 abortEnableFlg;
    /* ��NV�еĽṹ��һ���ֽ����ڱ�֤���ַ��������� */
    VOS_UINT8 abortAtCmdStr[AT_MAX_ABORT_CMD_STR_LEN + 1];
    VOS_UINT8 abortAtRspStr[AT_MAX_ABORT_RSP_STR_LEN + 1];
    VOS_UINT8 anyAbortFlg;
} AT_ABORT_CmdPara;

typedef struct {
    VOS_UINT32 atSetTick[AT_MAX_CLIENT_NUM];
} AT_CMD_ABORT_TICK_INFO;

typedef struct {
    AT_ABORT_CmdPara       atAbortCmdPara;
    AT_CMD_ABORT_TICK_INFO cmdAbortTick;
} AT_ABORT_CmdCtx;

/* ���Ŵ������ϱ� */
enum {
    PPP_DIAL_ERR_CODE_DISABLE = 0, /* �����벻�ϱ��� */
    PPP_DIAL_ERR_CODE_ENABLE  = 1, /* �������ϱ� */
    PPP_DIAL_ERR_CODE_BUTT
};
typedef VOS_UINT8 PPP_DIAL_ERR_CODE_ENUM;

typedef struct {
    VOS_UINT8 gsmConnectRate;
    VOS_UINT8 gprsConnectRate;
    VOS_UINT8 edgeConnectRate;
    VOS_UINT8 wcdmaConnectRate;
    VOS_UINT8 dpaConnectRate;
    VOS_UINT8 reserve1;
    VOS_UINT8 reserve2;
    VOS_UINT8 reserve3;
} AT_DIAL_ConnectDisplayRate;

/*
 * Э����: ��
 * �ṹ˵��: UE������Ϣ
 */
typedef struct {
    VOS_INT8  wasRelIndicator; /* Access Stratum Release Indicator */
    VOS_UINT8 wasCategory;
    VOS_UINT8 gasMultislotClass33Flg;
    VOS_UINT8 lteUeDlCategory; /* LTE��UE����category */
    VOS_UINT8 lteUeUlCategory; /* LTE��UE����category */
    VOS_UINT8 reserve[3];      /* ����λ */

} AT_DOWNLINK_RateCategory;


typedef struct {
    AT_InterMsgIdUint32 cmdMsgId;
    AT_InterMsgIdUint32 resultMsgId;
} AT_TRACE_MsgidTab;


typedef enum {
    AT_SS_CUSTOMIZE_CCWA_QUERY,
    AT_SS_CUSTOMIZE_CLCK_QUERY,
    AT_SS_CUSTOMIZE_BUTT,
} AT_SS_CUSTOMIZE_TYPE;
typedef VOS_UINT8 AT_SS_CUSTOMIZE_TYPE_UINT8;

#define AT_SS_CUSTOMIZE_SERVICE_MASK 0x1


enum AT_AccessStratumRel {
    AT_ACCESS_STRATUM_REL8       = 0,
    AT_ACCESS_STRATUM_REL9       = 1,
    AT_ACCESS_STRATUM_REL10      = 2,
    AT_ACCESS_STRATUM_REL11      = 3,
    AT_ACCESS_STRATUM_REL12      = 4,
    AT_ACCESS_STRATUM_REL13      = 5,
    AT_ACCESS_STRATUM_REL14      = 6,
    AT_ACCESS_STRATUM_REL_SPARE1 = 7
};
typedef VOS_UINT8 AT_AccessStratumRelUint8;

/* ********************************Other End**************************** */


typedef struct {
    VOS_UINT8 modemId : 2;
    VOS_UINT8 reportFlg : 1;
    VOS_UINT8 reserved : 5;
} AT_ClinetConfigDesc;


typedef struct {
    VOS_UINT32 rptCfgBit32[AT_RPT_CFG_BIT32_LEN];
} AT_PortRptCfg;

enum TAF_NvClientCfg {
    TAF_NV_CLIENT_CFG_PCUI       = 0,
    TAF_NV_CLIENT_CFG_CTRL       = 1,
    TAF_NV_CLIENT_CFG_MODEM      = 2,
    TAF_NV_CLIENT_CFG_NDIS       = 3,
    TAF_NV_CLIENT_CFG_UART       = 4,
    TAF_NV_CLIENT_CFG_SOCK       = 5,
    TAF_NV_CLIENT_CFG_APPSOCK    = 6,
    TAF_NV_CLIENT_CFG_HSIC1      = 7,
    TAF_NV_CLIENT_CFG_HSIC2      = 8,
    TAF_NV_CLIENT_CFG_HSIC3      = 9,
    TAF_NV_CLIENT_CFG_HSIC4      = 10,
    TAF_NV_CLIENT_CFG_MUX1       = 11,
    TAF_NV_CLIENT_CFG_MUX2       = 12,
    TAF_NV_CLIENT_CFG_MUX3       = 13,
    TAF_NV_CLIENT_CFG_MUX4       = 14,
    TAF_NV_CLIENT_CFG_MUX5       = 15,
    TAF_NV_CLIENT_CFG_MUX6       = 16,
    TAF_NV_CLIENT_CFG_MUX7       = 17,
    TAF_NV_CLIENT_CFG_MUX8       = 18,
    TAF_NV_CLIENT_CFG_APP        = 19,
    TAF_NV_CLIENT_CFG_APP1       = 20,
    TAF_NV_CLIENT_CFG_APP2       = 21,
    TAF_NV_CLIENT_CFG_APP3       = 22,
    TAF_NV_CLIENT_CFG_APP4       = 23,
    TAF_NV_CLIENT_CFG_APP5       = 24,
    TAF_NV_CLIENT_CFG_APP6       = 25,
    TAF_NV_CLIENT_CFG_APP7       = 26,
    TAF_NV_CLIENT_CFG_APP8       = 27,
    TAF_NV_CLIENT_CFG_APP9       = 28,
    TAF_NV_CLIENT_CFG_APP10      = 29,
    TAF_NV_CLIENT_CFG_APP11      = 30,
    TAF_NV_CLIENT_CFG_APP12      = 31,
    TAF_NV_CLIENT_CFG_APP13      = 32,
    TAF_NV_CLIENT_CFG_APP14      = 33,
    TAF_NV_CLIENT_CFG_APP15      = 34,
    TAF_NV_CLIENT_CFG_APP16      = 35,
    TAF_NV_CLIENT_CFG_APP17      = 36,
    TAF_NV_CLIENT_CFG_APP18      = 37,
    TAF_NV_CLIENT_CFG_APP19      = 38,
    TAF_NV_CLIENT_CFG_APP20      = 39,
    TAF_NV_CLIENT_CFG_APP21      = 40,
    TAF_NV_CLIENT_CFG_APP22      = 41,
    TAF_NV_CLIENT_CFG_APP23      = 42,
    TAF_NV_CLIENT_CFG_APP24      = 43,
    TAF_NV_CLIENT_CFG_APP25      = 44,
    TAF_NV_CLIENT_CFG_APP26      = 45,
    TAF_NV_CLIENT_CFG_HSIC_MODEM = 46,
    TAF_NV_CLIENT_CFG_HSUART     = 47,
    TAF_NV_CLIENT_CFG_PCUI2      = 48,

    TAF_NV_CLIENT_CFG_APP27 = 49,
    TAF_NV_CLIENT_CFG_APP28 = 50,
    TAF_NV_CLIENT_CFG_APP29 = 51,
    TAF_NV_CLIENT_CFG_APP30 = 52,
    TAF_NV_CLIENT_CFG_APP31 = 53,
    TAF_NV_CLIENT_CFG_APP32 = 54,
    TAF_NV_CLIENT_CFG_APP33 = 55,
    TAF_NV_CLIENT_CFG_APP34 = 56,
    TAF_NV_CLIENT_CFG_APP35 = 57,
    TAF_NV_CLIENT_CFG_APP36 = 58,
    TAF_NV_CLIENT_CFG_APP37 = 59,
    TAF_NV_CLIENT_CFG_APP38 = 60,
    TAF_NV_CLIENT_CFG_APP39 = 61,
    TAF_NV_CLIENT_CFG_APP40 = 62,
    TAF_NV_CLIENT_CFG_APP41 = 63,
    TAF_NV_CLIENT_CFG_APP42 = 64,
    TAF_NV_CLIENT_CFG_APP43 = 65,
    TAF_NV_CLIENT_CFG_APP44 = 66,
    TAF_NV_CLIENT_CFG_APP45 = 67,
    TAF_NV_CLIENT_CFG_APP46 = 68,
    TAF_NV_CLIENT_CFG_APP47 = 69,
    TAF_NV_CLIENT_CFG_APP48 = 70,
    TAF_NV_CLIENT_CFG_APP49 = 71,
    TAF_NV_CLIENT_CFG_APP50 = 72,
    TAF_NV_CLIENT_CFG_APP51 = 73,
    TAF_NV_CLIENT_CFG_APP52 = 74,
    TAF_NV_CLIENT_CFG_CMUXAT = 75,
    TAF_NV_CLIENT_CFG_CMUXMDM = 76,
    TAF_NV_CLIENT_CFG_CMUXEXT = 77,
    TAF_NV_CLIENT_CFG_CMUXGPS = 78,

    TAF_NV_CLIENT_CFG_MAX   = 96, /* TAF_AT_NVIM_CLIENT_CFG_LEN */
};
typedef VOS_UINT16 TAF_NvClientCfgUint16;


typedef struct {
    AT_ClientIdUint16     clientId;
    TAF_NvClientCfgUint16 nvIndex;
    VOS_UINT8             portName[AT_PORT_LEN_MAX];
    VOS_UINT32            rptCfgBit32[AT_RPT_CFG_BIT32_LEN];
} AT_ClientCfgMapTab;

#if (VOS_WIN32 == VOS_OS_VER)

typedef struct {
    VOS_UINT32            fileNum;
    USIMM_DefFileidUint32 fileId;
} AT_USIMM_FileNumToId;
#endif


typedef struct {
    AT_AccessStratumRelUint8 accessStratumRel; /* ������Э��汾 */
    VOS_UINT8                rsv[3];
} AT_AccessStratumRelease;


typedef struct {
    VOS_UINT8 gsmSupportFlg;
    VOS_UINT8 wcdmaSupportFlg;
    VOS_UINT8 lteSupportFlg;
    VOS_UINT8 ws46No;
} AT_WS46_RatTransformTbl;

typedef VOS_UINT32 (*AT_USER_RESEL_PROC_FUNC)(VOS_UINT8 ucIndex);


typedef struct {
    VOS_UINT32              userResel;
    VOS_UINT32              reserved;
    AT_USER_RESEL_PROC_FUNC userReselProc;
} AT_UserReselProcTbl;


typedef struct {
    VOS_CHAR *strDlSpeed; /* ��������������� */
    VOS_CHAR *strUlSpeed; /* ��������������� */
} AT_DisplayRatePair;


typedef struct {
    VOS_UINT32 infoAvailableFlg;   /* ��ǰ��¼�Ƿ���Ч */
    VOS_INT16  gammaReal;          /* ����ϵ��ʵ�� */
    VOS_INT16  gammaImag;          /* ����ϵ���鲿 */
    VOS_UINT16 gammaAmpUc0;        /* פ����ⳡ��0����ϵ������ */
    VOS_UINT16 gammaAmpUc1;        /* פ����ⳡ��1����ϵ������ */
    VOS_UINT16 gammaAmpUc2;        /* פ����ⳡ��2����ϵ������ */
    VOS_UINT16 gammaAntCoarseTune; /* �ֵ����λ�� */
    VOS_UINT32 ulwFomcoarseTune;   /* �ֵ�FOMֵ */
    VOS_UINT16 cltAlgState;        /* �ջ��㷨����״̬ */
    VOS_UINT16 cltDetectCount;     /* �ջ������ܲ��� */
    VOS_UINT16 dac0;               /* DAC0 */
    VOS_UINT16 dac1;               /* DAC1 */
    VOS_UINT16 dac2;               /* DAC2 */
    VOS_UINT16 dac3;               /* DAC3 */
} AT_TX_CltInfo;

/* AT��һ�����ƽṹ�����ڼ�¼��ǰʹ���ĸ�DSP����ǰ��TMODEֵ */
typedef struct {
    VOS_BOOL                  dspLoadFlag;
    TAF_UINT8                 index; /* AT���������Indexֵ */
    TAF_PH_TmodeUint8         currentTMode;
    AT_DEVICE_CmdRatModeUint8 deviceRatMode;
    AT_DEVICE_CmdBandUint8    deviceAtBand;
    AT_DSP_BandArfcn          dspBandArfcn;
    /* AT^FDAC���õ�FDACֵ��AT^FWAVE���õ�powerֵ��ʾ�ĺ�����ͬ��ȡ�����õ�ֵ */
    VOS_BOOL            fdacFlag; /* FDAC�Ƿ���Ч */
    VOS_UINT16          fdac;     /* ��ӦW��AGC,G��TxVpa */
    VOS_UINT16          rxDiv;    /* �ּ����� */
    VOS_UINT16          rxPri;    /* �������� */
    VOS_UINT16          origBand; /* ��¼��ʽת�����������ּ����� */
    AT_DSP_RfOnOffUint8 rxOnOff;  /* Rx on offֵ */
    VOS_UINT8           paLevel;  /* �������PA�ȼ� */
    VOS_UINT8           lnaLevel; /* ���ջ�LNA�ȼ� */
    VOS_UINT8           priOrDiv; /* ��ǰ���������û��Ƿּ����� */
    AT_DSP_RfOnOffUint8 txOnOff;  /* Tx On offֵ */
    /* ��¼���һ��ִ�е��Ǵ򿪽��ջ�(RXON=1)�������Ǵ򿪷����(TXON=1)���� */
    VOS_UINT8           rxonOrTxon;
    AT_DSP_RfOnOffUint8 tempRxorTxOnOff; /* ��ʱ��¼Rx �� Tx on offֵ */

    AT_DSP_CltEnableUint8 cltEnableFlg; /* ���߱ջ���гʹ�ܱ�־ */
    VOS_BOOL              powerFlag;    /* Power�Ƿ���Ч */
    VOS_UINT16            power;        /* AT^FWAVE���õĹ���ֵ */
    VOS_UINT16            txMode;       /* AT^FWAVE���õ�<type>ֵת��Ϊ���͸�������ֵ(�ñ���ֻ��G�����ʹ��):
                                         * <type>Ϊ0�������������Σ�usTxMode��Ϊ1��
                                         * <type>Ϊ����ֵ��usTxMode��Ϊ0
                                         */
    AT_DCXOTEMPCOMP_EnableUint8 dcxoTempCompEnableFlg;
    VOS_UINT8                   reserved[7];

    AT_TX_CltInfo cltInfo;
} AT_DEVICE_CmdCtrl;

#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)

typedef struct {
    VOS_CHAR  *atCmd;
    VOS_UINT32 chkFlag;
} AT_CmdTrustListMapTbl;
#endif

extern AT_CommCtx g_atCommCtx;

extern AT_ModemCtx g_atModemCtx[];

extern AT_ClientCtx g_atClientCtx[];

/* ********************************������Ҫ������************************************ */
extern AT_E5_RightFlagUint32 g_ate5RightFlag;

extern VOS_INT8 g_atOpwordPwd[];

extern AT_RightOpenFlagInfo g_atRightOpenFlg;

extern VOS_INT8 g_ate5DissdPwd[];

extern VOS_UINT8 g_atCmdNotSupportStr[];

extern PPP_DIAL_ERR_CODE_ENUM g_pppDialErrCodeRpt;

extern AT_DIAL_ConnectDisplayRate g_dialConnectDisplayRate;

extern AT_DOWNLINK_RateCategory g_atDlRateCategory;

extern VOS_UINT8 g_atS3;
extern VOS_UINT8 g_atS4;
extern VOS_UINT8 g_atS5;
extern VOS_UINT8 g_atS6;
extern VOS_UINT8 g_atS7;

extern AT_CMEE_TYPE g_atCmeeType; /* ָʾ��������������� */

extern TAF_UINT32 g_stkFunctionFlag;

/* ********************************SMS Begin************************************ */
extern MN_MSG_Class0TailorUint8 g_class0Tailor;
/* ********************************SMS End************************************ */

/* ********************************NET Begin************************************ */
extern VOS_UINT16 g_reportCregActParaFlg;

/* CREG/CGREG��<CI>����4�ֽ��ϱ��Ƿ�ʹ��(VDF����) */
extern CREG_CGREG_CI_RPT_BYTE_ENUM g_ciRptByte;

/* ********************************NET End************************************ */
#if (FEATURE_LTE == FEATURE_ON)
extern TAF_NVIM_LteRsrpCfg g_rsrpCfg;
extern TAF_NVIM_LteRscpCfg g_rscpCfg;
extern TAF_NVIM_LteEcioCfg g_ecioCfg;

extern AT_AccessStratumRelease g_releaseInfo;
#endif

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON) && \
     (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))

extern VOS_UINT8 g_currEncVoiceDataWriteFileNum;
extern VOS_UINT32 g_atCurrEncVoiceDataCount;
extern const TAF_CHAR* g_currEncVoiceDataWriteFilePath[];
extern VOS_UINT8 g_atCurrEncVoiceTestFileNum;
extern TAF_CHAR g_atCurrDocName[AT_TEST_ECC_FILE_NAME_MAX_LEN];
#endif

extern AT_SS_CustomizePara g_atSsCustomizePara;

extern const AT_ClientCfgMapTab g_atClientCfgMapTbl[];
extern const VOS_UINT8    g_atClientCfgMapTabLen;
extern VOS_UINT32 g_ctzuFlag;

extern VOS_VOID AT_InitUsimStatus(ModemIdUint16 modemId);
extern VOS_VOID AT_InitPlatformRatList(ModemIdUint16 modemId);

extern VOS_VOID AT_InitCommPbCtx(VOS_VOID);

extern VOS_VOID AT_InitCommCtx(VOS_VOID);
extern VOS_VOID AT_InitModemCcCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitModemSmsCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitModemNetCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitModemAgpsCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitModemImsCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitNvloadCarrierCtx(ModemIdUint16 modemId);
extern VOS_VOID AT_InitCtx(VOS_VOID);
extern VOS_VOID AT_InitModemCtx(ModemIdUint16 modemId);

extern VOS_VOID         AT_InitClientConfiguration(VOS_VOID);
extern VOS_VOID         AT_InitClientCtx(VOS_VOID);
VOS_VOID                AT_CleanDataChannelCfg(AT_PS_DataChanlCfg *chanCfg);
ModemIdUint16           AT_GetModemIDFromPid(VOS_UINT32 pid);
extern AT_USIM_InfoCtx* AT_GetUsimInfoCtxFromModemId(ModemIdUint16 modemId);
extern AT_ModemSptRat*  AT_GetSptRatFromModemId(ModemIdUint16 modemId);
extern VOS_UINT8        AT_IsModemSupportRat(ModemIdUint16 modemId, TAF_MMA_RatTypeUint8 rat);

extern VOS_UINT8 AT_IsModemSupportUtralTDDRat(ModemIdUint16 modemId);

extern AT_CommCtx*   AT_GetCommCtxAddr(VOS_VOID);
extern AT_CommPsCtx* AT_GetCommPsCtxAddr(VOS_VOID);
AT_MtTrustListCtrl *AT_GetCommMtTrustCtrlAddr(VOS_VOID);
#if (FEATURE_IMS == FEATURE_ON)
extern AT_IMS_EmcRdp* AT_GetImsEmcRdpByClientId(VOS_UINT16 clientId);
#endif

extern VOS_UINT32 AT_GetPsIPv6IIDTestModeConfig(VOS_VOID);

extern AT_CommPbCtx* AT_GetCommPbCtxAddr(VOS_VOID);

extern AT_CmdMsgNumCtrl* AT_GetMsgNumCtrlCtxAddr(VOS_VOID);

extern AT_ModemCcCtx*   AT_GetModemCcCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemCcCtx*   AT_GetModemCcCtxAddrFromClientId(VOS_UINT16 clientId);
extern AT_ModemSsCtx*   AT_GetModemSsCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemSsCtx*   AT_GetModemSsCtxAddrFromClientId(VOS_UINT16 clientId);
extern AT_ModemSmsCtx*  AT_GetModemSmsCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemSmsCtx*  AT_GetModemSmsCtxAddrFromClientId(VOS_UINT16 clientId);
extern AT_ModemNetCtx*  AT_GetModemNetCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemNetCtx*  AT_GetModemNetCtxAddrFromClientId(VOS_UINT16 clientId);
extern AT_ModemAgpsCtx* AT_GetModemAgpsCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemAgpsCtx* AT_GetModemAgpsCtxAddrFromClientId(VOS_UINT16 clientId);

extern AT_ModemPsCtx* AT_GetModemPsCtxAddrFromModemId(ModemIdUint16 modemId);

extern AT_ModemPsCtx* AT_GetModemPsCtxAddrFromClientId(VOS_UINT16 clientId);

extern AT_ModemImsContext* AT_GetModemImsCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemImsContext* AT_GetModemImsCtxAddrFromClientId(VOS_UINT16 clientId);

extern AT_ModemMbbCustmizeCtx* AT_GetModemMbbCustCtxAddrFromModemId(ModemIdUint16 modemId);
extern AT_ModemMbbCustmizeCtx* AT_GetModemMbbCustCtxAddrFromClientId(VOS_UINT16 clientId);

extern AT_ClientCtx* AT_GetClientCtxAddr(AT_ClientIdUint16 clientId);
extern VOS_UINT8*    AT_GetSystemAppConfigAddr(VOS_VOID);
extern VOS_UINT32    AT_GetDestPid(MN_CLIENT_ID_T clientId, VOS_UINT32 rcvPid);

extern TAF_CS_CauseUint32 AT_GetCsCallErrCause(VOS_UINT16 clientId);

extern AT_ABORT_CmdPara* AT_GetAbortCmdPara(VOS_VOID);
extern VOS_UINT8*        AT_GetAbortRspStr(VOS_VOID);

extern VOS_UINT32 AT_GetSsCustomizePara(AT_SS_CUSTOMIZE_TYPE_UINT8 ssCustomizeType);

extern VOS_VOID     AT_InitResetCtx(VOS_VOID);
extern AT_ResetCtx* AT_GetResetCtxAddr(VOS_VOID);
extern VOS_SEM      AT_GetResetSem(VOS_VOID);
extern VOS_UINT32   AT_GetResetFlag(VOS_VOID);
extern VOS_VOID     AT_SetResetFlag(VOS_UINT32 flag);

AT_ModemPrivacyFilterCtx* AT_GetModemPrivacyFilterCtxAddrFromModemId(ModemIdUint16 modemId);

AT_ModemCdmamodemswitchCtx* AT_GetModemCdmaModemSwitchCtxAddrFromModemId(ModemIdUint16 modemId);

extern AT_ModemMtInfoCtx* AT_GetModemMtInfoCtxAddrFromModemId(ModemIdUint16 modemId);

extern AT_ModemNvloadCtx* AT_GetModemNvloadCtxAddrFromModemId(ModemIdUint16 modemId);

extern VOS_VOID                            AT_ClearSimLockWriteExCtx(VOS_VOID);
extern AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX* AT_GetSimLockWriteExCmdCtxAddr(VOS_VOID);

extern VOS_VOID                            AT_ClearPhonePhynumCtx(VOS_VOID);
extern AT_PhonePhynumCmdProcCtx*           AT_GetPhonePhynumCmdCtxAddr(VOS_VOID);

VOS_VOID            AT_InitUartCtx(VOS_VOID);
AT_UartCtx*         AT_GetUartCtxAddr(VOS_VOID);
AT_UartPhyCfg*      AT_GetUartPhyCfgInfo(VOS_VOID);
AT_UartFlowCtrl*    AT_GetUartFlowCtrlInfo(VOS_VOID);
AT_UartRiCfg*       AT_GetUartRiCfgInfo(VOS_VOID);
AT_UartRiStateInfo* AT_GetUartRiStateInfo(VOS_VOID);

VOS_VOID            AT_InitTraceMsgTab(VOS_VOID);
AT_InterMsgIdUint32 AT_GetResultMsgID(VOS_UINT8 index);
AT_InterMsgIdUint32 AT_GetCmdMsgID(VOS_UINT8 index);

VOS_VOID AT_ConfigTraceMsg(VOS_UINT8 index, AT_InterMsgIdUint32 cmdMsgId, AT_InterMsgIdUint32 resultMsgId);

VOS_VOID                At_SetAtCmdAbortTickInfo(VOS_UINT8 index, VOS_UINT32 tick);
AT_CMD_ABORT_TICK_INFO* At_GetAtCmdAbortTickInfo(VOS_VOID);
VOS_UINT8               At_GetAtCmdAnyAbortFlg(VOS_VOID);
VOS_VOID                At_SetAtCmdAnyAbortFlg(VOS_UINT8 flg);

AT_PortBuffCfgInfo* AT_GetPortBuffCfgInfo(VOS_VOID);

AT_PortBuffCfgUint8 AT_GetPortBuffCfg(VOS_VOID);

VOS_VOID AT_InitPortBuffCfg(VOS_VOID);

VOS_VOID AT_AddUsedClientId2Tab(VOS_UINT16 clientId);

VOS_VOID AT_RmUsedClientIdFromTab(VOS_UINT16 clientId);

AT_ClientConfiguration* AT_GetClientConfig(AT_ClientIdUint16 clientId);
const AT_ClientCfgMapTab*     AT_GetClientCfgMapTbl(VOS_UINT8 index);

VOS_UINT8 AT_GetPrivacyFilterEnableFlg(VOS_VOID);

VOS_VOID AT_ConvertCellIdToHexStrFormat(VOS_UINT32 cellIdentityLowBit, VOS_UINT32 cellIdentityHighBit,
                                        VOS_CHAR *pcCellIdStr);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT8* AT_GetModemCLModeCtxAddrFromModemId(ModemIdUint16 modemId);
#endif
VOS_UINT8 AT_IsSupportReleaseRst(AT_AccessStratumRelUint8 releaseType);

VOS_VOID AT_InitReleaseInfo(VOS_VOID);

VOS_VOID                       AT_ClearAuthPubkeyCtx(VOS_VOID);
VOS_VOID                       AT_InitCmdProcCtx(VOS_VOID);
AT_AUTH_PUBKEYEX_CMD_PROC_CTX* AT_GetAuthPubkeyExCmdCtxAddr(VOS_VOID);
AT_CmdProcCtx*                 AT_GetCmdProcCtxAddr(VOS_VOID);

AT_VmsetCmdCtx* AT_GetCmdVmsetCtxAddr(VOS_VOID);
VOS_VOID        AT_InitVmSetCtx(VOS_VOID);

VOS_VOID AT_UpdateCallErrInfo(VOS_UINT16 clientId, TAF_CS_CauseUint32 csErrCause, TAF_CALL_ErrorInfoText *errInfoText);

TAF_CALL_ErrorInfoText* AT_GetCallErrInfoText(VOS_UINT16 clientId);

VOS_UINT8 AT_GetCgpsCLockEnableFlgByModemId(ModemIdUint16 modemId);
VOS_VOID  AT_SetCgpsCLockEnableFlgByModemId(ModemIdUint16 modemId, VOS_UINT8 enableFLg);

VOS_VOID AT_InitMsgNumCtrlCtx(VOS_VOID);
VOS_UINT8 At_GetGsmConnectRate(VOS_VOID);

VOS_UINT8 At_GetGprsConnectRate(VOS_VOID);

VOS_UINT8 At_GetEdgeConnectRate(VOS_VOID);

VOS_UINT8 At_GetWcdmaConnectRate(VOS_VOID);

VOS_UINT8 At_GetDpaConnectRate(VOS_VOID);

VOS_VOID AT_RcvTiSimlockWriteExExpired(REL_TimerMsgBlock *tmrMsg);
VOS_VOID AT_RcvTiAuthPubkeyExpired(REL_TimerMsgBlock *tmrMsg);


extern TAF_MMA_SimsqStateUint32 At_GetSimsqStatus(ModemIdUint16 modemId);


extern VOS_VOID At_SetSimsqStatus(ModemIdUint16 modemId, TAF_MMA_SimsqStateUint32 simsqStatus);


extern VOS_UINT8 At_GetSimsqEnable(VOS_VOID);


extern VOS_VOID At_SetSimsqEnable(VOS_UINT8 simsqEnable);

#if (FEATURE_ECALL == FEATURE_ON)
VOS_VOID            AT_InitCommEcallCtx(VOS_VOID);
AT_ECALL_AlackInfo* AT_EcallAlAckInfoAddr(VOS_VOID);
VOS_VOID            AT_SetEclModeValue(VOS_UINT8 eclmode);
VOS_UINT8           AT_GetEclModeValue(VOS_VOID);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
AT_ModemUePolicyCtx* AT_GetModemUePolicyCtxAddrFromModemId(MODEM_ID_ENUM_UINT16 modemId);

AT_ModemUePolicyCtx* AT_GetModemUePolicyCtxAddrFromClientId(VOS_UINT16 clientId);

VOS_VOID AT_InitModemUePolciyCtx(MODEM_ID_ENUM_UINT16 modemId);

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
AT_DEVICE_CmdCtrl* AT_GetDevCmdCtrl(VOS_VOID);
#else
AT_MT_Info* AT_GetMtInfoCtx(VOS_VOID);
#endif

const TAF_CHAR* AT_GetPppDialRateDisplay(VOS_UINT32 atRateIndex);
const VOS_CHAR* AT_GetDialRateDisplayNv(VOS_UINT32 atRateIndex);

#if (FEATURE_LTE == FEATURE_ON)
const AT_DisplayRatePair AT_GetLteRateDisplay(VOS_UINT32 atRateIndex);
VOS_VOID                 AT_SetWifiRF(VOS_UINT32 atWifiRF);
VOS_UINT32               AT_GetWifiRF(VOS_VOID);
#endif

VOS_VOID   AT_SetStkCmdQualify(TAF_UINT8 atStkCmdQualify);
TAF_UINT8  AT_GetStkCmdQualify(VOS_VOID);
VOS_VOID   AT_SetErrType(TAF_UINT32 atErrType);
TAF_UINT32 AT_GetErrType(VOS_VOID);
VOS_VOID   AT_SetWifiFreq(VOS_UINT32 atWifiFreq);
VOS_UINT32 AT_GetWifiFreq(VOS_VOID);
VOS_VOID   AT_SetWifiMode(VOS_UINT32 atWifiMode);
VOS_UINT32 AT_GetWifiMode(VOS_VOID);
VOS_VOID   AT_SetWifiRate(VOS_UINT32 atWifiRate);
VOS_UINT32 AT_GetWifiRate(VOS_VOID);
VOS_VOID   AT_SetWifiPower(VOS_INT32 atWifiPower);
VOS_INT32  AT_GetWifiPower(VOS_VOID);
VOS_UINT*  AT_GetUcastWifiRxPkts(VOS_VOID);

#if (FEATURE_AT_CMD_TRUST_LIST == FEATURE_ON)
const AT_CmdTrustListMapTbl* AT_GetUserCmdTrustListTbl(VOS_VOID);
VOS_UINT32 AT_GetUserCmdTrustListTblCmdNum(VOS_VOID);
VOS_UINT32 AT_GetCmdKeyWord(const VOS_CHAR *atCmd, VOS_UINT16 atCmdLen, VOS_CHAR *keyWord, VOS_UINT32 keyWordMaxLen);
VOS_UINT32 AT_IsCmdInTrustList(VOS_CHAR *keyWord, VOS_UINT32 keyWordLen);
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
VOS_UINT32 AT_GetStkReportAppConfigFlag(VOS_UINT16 clientId);
VOS_VOID AT_SetStkReportAppConfigFlag(ModemIdUint16 modemId, VOS_UINT32 flag);
VOS_VOID AT_SetSystemAppConfig(VOS_UINT16 clientId, VOS_UINT8 *systemAppConfig);
#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCtx.h */
