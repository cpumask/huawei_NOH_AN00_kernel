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

#ifndef _AT_CMD_PROC_H_
#define _AT_CMD_PROC_H_

#include "si_app_emat.h"
#include "AtCtx.h"
#include "si_app_pb.h"
#include "mn_error_code.h"
#include "taf_app_ppp.h"
#include "taf_drv_agent.h"
#include "mdrv.h"

#include "cbt_cpm_interface.h"
#include "dms_port_i.h"
#include "AtParse.h"
#include "AtMntn.h"
#include "AtSndMsg.h"
#include "nas_om_interface.h"
#ifdef MODEM_FUSION_VERSION
#include "mdrv_diag.h"
#else
#include "msp_errno.h"
#endif

#include "AcpuReset.h"
#include "si_app_pih.h"
#include "TafAgentInterface.h"

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#include "taf_app_xsms_interface.h"
#include "at_xpds_interface.h"
#endif

#include "at_mta_interface.h"
#include "TafLogPrivacyMatch.h"

#include "at_mt_interface.h"

#include "AtDeviceCmdTL.h"
#include "AtExtendCmd.h"
#include "AtExtendPrivateCmd.h"
#include "AtOutputProc.h"
#include "AtParseExtendCmd.h"
#include "AtSetParaCmd.h"
#include "at_lte_ct_proc.h"
#include "at_lte_eventreport.h"
#include "at_lte_ms_proc.h"
#include "at_lte_set_proc.h"
#include "at_lte_upgrade_proc.h"
#include "AtAbortParaCmd.h"
#include "AtQueryParaCmd.h"
#include "AtBasicCmd.h"
#if (FEATURE_LTEV == FEATURE_ON)
#include "taf_v2x_api.h"
#endif
#include "mn_comm_api.h"
#include "taf_msg_chk_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#define AT_USBCOM_DISABLED 0 /* ����ȥʹ��״̬suspend/disable */
#define AT_USBCOM_ENABLED 1  /* ����ʹ��״̬resume/enable */
#define AT_CC_CALSS_TYPE_INVALID 255

#define AT_APN_KEY_PARA_NUM 2 /* at^voipapnkey �������������Ŀ */
#define AT_MAX_APN_KEY_GROUP 1 /* ��ǰ��֧��һ������ */

/* ATģ������һ����������XML�������� */
#define AT_XML_MAX_LEN 1024

/* ATģ������һ����������UE POLICY�������� */
#define AT_UE_POLICY_MAX_LEN 4000

#define AT_CUSD_M_NETWORK_TIMEOUT 5

#define AT_CUSD_M_NETWORK_CANCEL 2

#define AT_SFEATURE_DIV_STRING_LEN 50

#define AT_PB_IRA_MAX_NUM 256
#define AT_PB_GSM_MAX_NUM 128
#define AT_PB_GSM7EXT_MAX_NUM 10
#define AT_PB_GSM7EXT_SYMBOL 0x1B
#define AT_PB_81CODE_HEADER_LEN 3
#define AT_PB_82CODE_HEADER_LEN 4

/* 81�����õ�8����15bit���б��룬��0hhh hhhh hXXX XXXX */
#define AT_PB_81_CODE_BASE_POINTER 0x7f80
/* GSM7Bit ����ķ�Χ��0~127 */
#define AT_PB_GSM7_CODE_MAX_VALUE 0x7f
/* GSM7Bit ����ĵ�8bit����Ϊ0 */
#define AT_PB_CODE_NO8_BIT 0x80

#define AT_AP_NVIM_XML_RPT_SRV_URL_LEN 127
#define AT_AP_XML_RPT_INFO_TYPE_LEN 127

#define AT_WEBUI_PWD_VERIFY 1

#define AT_ANTEN_LEV_MAX_NUM 6

/* Unsigned long���Ͷ�Ӧʮ����������λ�� */
#define AT_HEX_LEN_OF_ULONG_TYPE 8
/* at^syscfg roam����������δ�������Ϊ���ı� */
#define AT_ROAM_FEATURE_OFF_NOCHANGE 2
#define AT_SYSCFGEX_PARA_NUM 7 /* at^syscfgex��������Ĳ������� */
/* at^syscfgex��ѯ�ϱ�����ģʽ�ַ������ȣ������ģʽΪG,�ϱ�01�ַ�������Ϊ2 */
#define AT_SYSCFGEX_RAT_MODE_STR_LEN 2
#define AT_SYSCFGEX_RAT_NO_CHANGE_HEX_VALUE 0x99
#define AT_NETSCAN_PARA_MAX_NUM 4
#define AT_NETSCAN_PARA_MIN_NUM 2
#define AT_NETSCAN_POWER_PARA_MAX_LEN 4
#define AT_NETSCAN_POWER_PARA_MIN_LEN 3
#define AT_NETSCAN_POWER_MIN_VALUE (-110)
#define AT_NETSCAN_POWER_MAX_VALUE (-47)
#define AT_NETSCAN_LTE_POWER_MIN_VALUE (-125) /* ����LTE��RSRP������̽��-125dbm */
#define AT_ALL_GU_BAND 0x3FFFFFFF
#define AT_NETSCAN_PSC_MAX_VALUE 511 /* PSC�������ЧֵΪ511 */
#define TAF_SERIAL_NUM_LEN 16

#define TAF_SERIAL_NUM_NV_LEN (TAF_SERIAL_NUM_LEN + 4)


#define MN_MSG_SMS_MAX_NUM 255

#define AT_INVALID_INDEX 65535
#define AT_AP_SPEED_STRLEN 16

#define AT_MAX_CREATION_TIME_LEN 31 /* ϵͳ����ʱ�� */

/* AT^DISLOG��������255Ϊ�л�PCUI����DIAG����, 0Ϊ��DIAG��, 1Ϊ�ر�DIAG��  */
#define AT_DISLOG_DIAG_OPEN 0
#define AT_DISLOG_DIAG_CLOSE 1

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
/* AT^SHELL�������� , 0ΪWIFIʹ��SHELL��, 1Ϊ�ر�SHELL�� , 2Ϊ��SHELL�� */
#define AT_SHELL_WIFI_USE 0
#define AT_SHELL_CLOSE 1
#define AT_SHELL_OPEN 2

/* SHELL������У����������� */
#define AT_SHELL_PWD_VERIFY_MAX_TIMES 3
#endif

#define AT_USB_ENABLE_EVENT 3
#define AT_USB_SUSPEND_EVENT 5

#define AT_MCC_LENGTH 3            /* MCC���� */
#define AT_MCC_PLUS_COMMA_LENGTH 4 /* MCC�Ӷ��ų��� */

#define AT_UART_MODE_ASHELL 0
#define AT_UART_MODE_CSHELL 1

#define AT_MINUTE_TO_SECONDS 60
#define AT_HOUR_TO_MINUTES 60
#define AT_DAY_TO_HOURS 24
#define SLICE_TO_MAX_SECOND 0xFFFFFFFF /* ����ܹ�������ʱ�䣬��λ:s */
#define SLICE_TO_SECOND_UINT 32768     /* 1��=32768slice */

#define AT_DISP_PARA_MAX_LEN 15
#define AT_SET_PARA_MAX_LEN 3
#define AT_ACCMD_ARRAY_MAX_LEN 200

#if (TAF_BYTE_ORDER == TAF_BIG_ENDIAN)
#define AT_GET_AUTH_LENGTH(pucData) \
    (((((TAF_UINT16)(*(pucData))) << 8) & 0xFF00) | (((TAF_UINT16)(*((pucData) + 1))) & 0x00FF))
#define AT_GET_IPCP_LENGTH(pucData) \
    (((((TAF_UINT16)(*(pucData))) << 8) & 0xFF00) | (((TAF_UINT16)(*((pucData) + 1))) & 0x00FF))
#else
#define AT_GET_AUTH_LENGTH(pucData) \
    ((((TAF_UINT16)(*(pucData))) & 0x00FF) | ((((TAF_UINT16)(*((pucData) + 1)) << 8)) & 0xFF00))
#define AT_GET_IPCP_LENGTH(pucData) \
    ((((TAF_UINT16)(*(pucData))) & 0x00FF) | ((((TAF_UINT16)(*((pucData) + 1)) << 8)) & 0xFF00))
#endif

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
extern unsigned char g_cmux_active_ppp_pstask_dlc;
extern VOS_UINT8 g_cmuxOpen;
#define CMUX_NUM_MIN      1
#define CMUX_NUM_MAX      9
#define MAX_CMUX_PORT_NUM 4
#endif

#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
extern int g_current_user_at;

#define AT_STOP_TIMER_CMD_READY(ucIndex) do { \
    AT_StopRelTimer(ucIndex, &g_atClientTab[ucIndex].hTimer);         \
    g_parseContext[ucIndex].clientStatus = AT_FW_CLIENT_STATUS_READY; \
    g_atClientTab[ucIndex].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;   \
    g_atClientTab[ucIndex].opId          = 0;                         \
    if (g_current_user_at == ucIndex) {                                \
        g_atClientTab[AT_CLIENT_ID_APP].isWaitAts = 0;                \
        g_current_user_at = -1;                                       \
    }                                                                 \
} while (0)
#else
#define AT_STOP_TIMER_CMD_READY(ucIndex) do { \
    AT_StopRelTimer(ucIndex, &g_atClientTab[ucIndex].hTimer);         \
    g_parseContext[ucIndex].clientStatus = AT_FW_CLIENT_STATUS_READY; \
    g_atClientTab[ucIndex].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;   \
    g_atClientTab[ucIndex].opId          = 0;                         \
} while (0)
#endif

#define AT_ADDR_TYPE_UNKNOWN_ISDN (0x80 | (MN_MSG_TON_UNKNOWN << 4) | MN_MSG_NPI_ISDN)
#define AT_ADDR_TYPE_INTERNATIONAL_ISDN (0x80 | (MN_MSG_TON_INTERNATIONAL << 4) | MN_MSG_NPI_ISDN)
#define At_UpChar(Char) ((((Char) >= 'a') && ((Char) <= 'z')) ? ((Char)-0x20) : (Char))
#define At_GetCodeType(Char) (((Char) == '+') ? (AT_ADDR_TYPE_INTERNATIONAL_ISDN) : (AT_ADDR_TYPE_UNKNOWN_ISDN))

#define At_GetSmsArea(SaveArea) ((MN_MSG_MEM_STORE_SIM == (SaveArea)) ? AT_STRING_SM : AT_STRING_ME)

#define At_CheckCmdSms(Char, Mode) \
    (((Mode) == 0) ? ((Char) == (g_atS3)) : ((((Char) == (g_atS3)) || ((Char) == ('\x1a'))) || ((Char) == ('\x1b'))))
#define At_CheckSplitChar(Char) ((((Char) == (g_atS3)) || ((Char) == ('\x1a'))) || ((Char) == ('\x1b')))

#define AT_UNICODE2VALUE(x) ((x) = ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8)))

#define AT_CMD_BUF_LEN 12040
#define AT_CLIENT_NULL 0 /* δʹ��״̬ */
#define AT_CLIENT_USED 1 /* ռ��״̬ */

#define AT_UNICODE_SMS_MAX_LENGTH 140 /* ����ϢUNICODE����󳤶� */
#define AT_ASCII_SMS_MAX_LENGTH 160   /* ����ϢASCII����󳤶� */
#define AT_SMS_MAX_LENGTH 160         /* ����Ϣ������󳤶� */
#define AT_SMS_SEG_MAX_LENGTH 800
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
#define AT_COM_BUFF_LEN 3072
#else
#define AT_COM_BUFF_LEN 1600
#endif
#define AT_CALL_MAX_NUM 7 /* ��������Ŀ */
#define AT_SMS_RP_ADDRESS_MAX_LEN 12

#define AT_PB_NAME_MAX_LENGTH AT_MSG_MAX_TPDU_LEN

/* DTMF��� */
#define AT_DTMF_START 1
#define AT_DTMF_STOP 0
#define AT_VTS_DEFAULT_DTMF_LENGTH 100
#define AT_DTMF_DEFAULT_DTMF_LENGTH 60000

#define AT_DTMF_MIN_DTMF_OFF_LENGTH 10 /* osa��������С���ȵĶ�ʱ��Ϊ10ms */

#define AT_QUICK_START_DISABLE 0x00000000
#define AT_QUICK_START_ENABLE 0x01000101

#define AT_CNMR_QRY_GAS_NMR_DATA 1 /* ^cnmr��ѯ2g��nmr ���� */
#define AT_CNMR_QRY_WAS_NMR_DATA 2 /* ^cnmr��ѯ3g��nmr ���� */
#define AT_CNMR_QRY_LTE_NMR_DATA 3 /* ^cnmr��ѯ4g��nmr ���� */

#define AT_CTRL_Z 0x1A /* CTRL-Z */
#define AT_ESC 0x1B    /* ESC */
#define AT_CR 0x0D     /* CR */

#define AT_NVIM_PARA_LEN NV_ITEM_AT_PARA_SIZE

#define AT_UPCASE(c) ((((c) >= 'a') && ((c) <= 'z')) ? ((c) - 0x20) : (c))

#define AT_SMS_EVENT_TYPE 0
#define AT_CS_EVENT_TYPE 1
#define AT_PS_EVENT_TYPE 2
#define AT_PH_EVENT_TYPE 3
#define AT_PB_EVENT_TYPE 4
#define AT_SS_EVENT_TYPE 5
#define AT_DS_EVENT_TYPE 6
#define AT_SET_EVENT_TYPE 7
#define AT_QRY_EVENT_TYPE 8
#define AT_DATA_IND_EVENT_TYPE 9
#define AT_DATA_STATUS_EVENT_TYPE 10
#define AT_SAT_IND_EVENT_TYPE 11
#define AT_SAT_RSP_EVENT_TYPE 12
#define AT_SAT_DATA_EVENT_TYPE 13

#define AT_BUTT_INDEX 0xFF
#define AT_CNMI_STANDARD 0
#define AT_MSG_DELETE_SINGLE 0x01
#define AT_MSG_DELETE_READ 0x02
#define AT_MSG_DELETE_UNREAD 0x04
#define AT_MSG_DELETE_SENT 0x08
#define AT_MSG_DELETE_UNSENT 0x10
#define AT_MSG_DELETE_ALL 0x20
typedef TAF_UINT8 AT_MSG_DeleteUint8;

#define AT_SMS_NVIM_SUPPORT 0
#define AT_BUTT_CLIENT_ID ((MN_CLIENT_ID_T)(-1))
#define AT_CSCA_CSMP_STORAGE_INDEX 0

#define AT_MSG_MAX_TPDU_LEN 176
#define AT_MSG_INTERNAL_ISDN_ADDR_TYPE 145

#define AT_MSG_MAX_MSG_SEGMENT_NUM 2

#define AT_MN_INTERFACE_MSG_HEADER_LEN ((sizeof(MN_AT_IndEvt) - VOS_MSG_HEAD_LENGTH) - 4)

#define AT_MN_MAX_SMS_TPDU_LEN 164

/* Refer to 27005 <fo> SMS-DELIVER, SMS-SUBMIT (default 17) */
#define AT_CSMP_FO_DEFAULT_VALUE1 17
#define AT_CSMP_SUBMIT_VP_DEFAULT_VALUE 167

/* Refer to 27005 <fo>  SMS-STATUS-REPORT, or SMS-COMMAND (default 2) */
#define AT_CSMP_FO_DEFAULT_VALUE2 2

/* 23040 9.2.3.3 bit no 3 and 4 of the first octet of SMS SUBMIT */
#define AT_GET_MSG_TP_VPF(ucVpf, ucFo) ((ucVpf) = ((ucFo) & 0x18) >> 3)

/* �����úͲ�ѯʱ�����̣�����������������ʱ�� */
#define AT_SET_CFUN_TIME 155000
#define AT_SET_PARA_TIME 30000 /* <==A32D12591 */
#define AT_QRY_PARA_TIME 30000 /* <==A32D12429 */
#define AT_TEST_PARA_TIME 30000
#define AT_TEST_PLATFORM_WRITE_TIME 120000

#define AT_SET_CGLA_PARA_TIME 90000

/* ���2G�����Ͷ��Ų����ش�������AT������ǰ��������: 75000 -> 105000 */
/* ���DCM 461-0701-0202 461-0701-0203�ش�������AT������ǰ�������� */
#define AT_SMS_SET_PARA_TIME 105000 /* <==A32D12591 */

#if (FEATURE_IMS == FEATURE_ON)
/* increase 15s for CMGS and CMSS set command */

#define AT_SMS_CMGS_SET_PARA_TIME 210000

#define AT_SMS_CMSS_SET_PARA_TIME 180000
/* increase 15s for SS service request command */
#define AT_SS_CUSD_SET_PARA_TIME 45000
#else
#define AT_SMS_CMGS_SET_PARA_TIME 105000
#define AT_SMS_CMSS_SET_PARA_TIME 105000
#define AT_SS_CUSD_SET_PARA_TIME 30000
#endif

/* ��PDP����ͶϿ�ʱAT���TIMER��ʱ������ */
#define AT_ACT_PDP_TIME 362000   /* 362 ��, APS��360��, 3380��30��һ�ι�5��150�� */
#define AT_DETACT_PDP_TIME 50000 /* 50��, APS��48��, 3390��8��һ�ι�5��40�� */
#define AT_MOD_PDP_TIME 170000   /* 170��, APS��168��, 3381��8��һ��5��40�� */

#define AT_NOT_SET_TIME 0

#define AT_COPS_SPEC_SRCH_TIME 120000 /* COPSָ������ʱ�� */
#define AT_COPS_LIST_SRCH_TIME 240000 /* COPS�б�����ʱ�� */
/* COPS�б��Ѵ��ʱ��, ��15S����ΪMMA�ı���ʱ��Ϊ10S */
#define AT_COPS_LIST_ABORT_TIME 15000

#define AT_CSG_LIST_SRCH_TIME 220000 /* CSG�б�����ʱ�� */
/* CSG�б��Ѵ��ʱ��, ��15S����ΪMMA�ı���ʱ��Ϊ10S */
#define AT_CSG_LIST_ABORT_TIME 15000

#define AT_CSG_SPEC_SRCH_TIME 120000 /* CSGָ������ʱ�� */

/* ����ATģ��ȴ�SYSCFG/SYSCFGEX�Ļظ�ʱ�䶨ʱ�� */
#define AT_SYSCFG_SET_PARA_TIME (144 * 1000) /* iteration 13�޸�Ϊ��ʱ��Ϊ100S */


#define AT_NETSCAN_SET_PARA_TIME 120000

#define AT_ABORT_NETSCAN_SET_PARA_TIME (15 * 1000)

#define AT_UPGRADE_TIME_5S 5000
#define AT_UPGRADE_TIME_50S 50000

#define AT_CGATT_SET_PARA_TIME 240000

/* ����ҵ������150s��ʱ����at�ı�����ʱ����Ҫ��ss�����ı�����ʱ������λ160s */
#define AT_SET_SS_PARA_TIME 160000
#define AT_QRY_SS_PARA_TIME 160000

#define AT_SET_VC_PARA_TIME 10000
#define AT_QRY_VC_PARA_TIME 10000
#define AT_SET_CALL_PARA_TIME 100000

#define AT_PPP_PROTOCOL_REL_TIME 5000 /* �ȴ�PPP��ӦAT_PPP_PROTOCOL_REL_IND_MSG�ı�����ʱ��ʱ�� */
#define AT_HOLD_CMD_TIMER_LEN 360000  /* ����AT��������Ķ�ʱ��ʱ��������Ϊ�AT����ʱ��360000 */

#define AT_AUTH_PUBKEY_PROTECT_TIMER_LEN 3000

#define AT_SIMLOCK_WRITE_EX_PROTECT_TIMER_LEN 15000

#define AT_BCD_HEX(x) ((TAF_UINT8)((TAF_UINT8)(((x) & 0xF0) >> 4) + (TAF_UINT8)(((x) & 0x0F) * 0x0A)))

#define AT_BCD_REVERSE(x) (TAF_UINT8)((((x) << 4) & 0xf0) + (((x) >> 4) & 0x0f))

/* PPP����ʱ��������������δָ��CID(��*99#��������)����Ĭ��ʹ������CID */
#define AT_DIAL_DEFAULT_CID 1

#define AT_INVALID_TZ_VALUE 0x7F

/* AT�����������õĵ�ַ��󳤶� */
#define AT_NDIS_IPV4_ADDR_PARA_LEN 15
#define AT_NDIS_IPV6_ADDR_PARA_LEN 63

#define AT_SERVICE_CS_PS 2                       /* PS��CSҵ��ʹ�� */
#define AT_DEVICE_STATE_FROM_COFIGFILE 0         /* 0:MDM+DIAG+PCUI */
#define AT_DEVICE_STATE_FROM_UE 4372             /* 4372:CDROM/MDM+NDIS+DIAG+PCUI+CDROM+SD+BT */
#define AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED 2  /* ������޸�,��Ϊ�����������ĵ�Ĭ��ֵ */
#define AT_CUSTOMIZE_ITEM_DEFAULT_VAL_UNCHANGE 0 /* ������δ�޸�,Ϊ�����������ĵ�Ĭ��ֵ */
#define AT_HSUPA_ENABLE_STATE 1                  /* en_NV_Item_WAS_RadioAccess_Capa NV����HSUPAΪʹ��״̬ */
#define AT_HSUPA_DISABLE_STATE 0                 /* en_NV_Item_WAS_RadioAccess_Capa NV����HSUPAΪδʹ��״̬ */
#define AT_HSDPA_ENABLE_STATE 1                  /* en_NV_Item_WAS_RadioAccess_Capa NV����HSDPAΪʹ��״̬ */
#define AT_HSDPA_DISABLE_STATE 0                 /* en_NV_Item_WAS_RadioAccess_Capa NV����HSDPAΪδʹ��״̬ */
#define AT_HSDSCH_PHY_CATEGORY_DEFAULT_VAL 10    /* en_NV_Item_WAS_RadioAccess_Capa NV����HSDPA�����ȼ�Ĭ��ֵ */
/* en_NV_Item_WAS_RadioAccess_Capa NV����Access Stratum Release Indicator */
#define AT_PS_PTL_VER_R7 4

/* NV_ITEM_CUSTOMIZE_SIM_LOCKPLMN_INFO NV����mnc num��Ĭ��ֵ */
#define AT_SIM_LOCK_MNC_NUM_DEFAULT_VAL 0xaa
/* NV_ITEM_CUSTOMIZE_SIM_LOCKPLMN_INFO NV��range_begin��range_end��Ĭ��ֵ */
#define AT_SIM_LOCK_PLMN_RANGE_DEFAULT_VAL 0xAAU
/* NV_ITEM_CUSTOMIZE_SIM_LOCK_MAX_TIMES NV��LockMaxTimes��Ĭ��ֵ */
#define AT_SIM_LOCK_MAXTIMES_DEFAULT_VAL 10
#define AT_GPRS_ACT_TIMER_LEN_DEFAULT_VAL 0 /* en_NV_Item_GPRS_ActiveTimerLength NV��Ĭ��ֵΪ20s,��λ:�� */
#define AT_HSDSCH_PHY_CATEGORY_6 6          /* WAS_HSDSCH_PHY_CATEGORY_6 */
#define AT_HSDSCH_PHY_CATEGORY_8 8          /* WAS_HSDSCH_PHY_CATEGORY_8 */
#define AT_HSDSCH_PHY_CATEGORY_10 10
#define AT_HSDSCH_PHY_CATEGORY_11 11 /* WAS_HSDSCH_PHY_CATEGORY_11 */

#define AT_HSDSCH_PHY_CATEGORY_12 12 /* WAS_HSDSCH_PHY_CATEGORY_12 */
#define AT_HSDSCH_PHY_CATEGORY_14 14 /* WAS_HSDSCH_PHY_CATEGORY_14 */

/* enHSDSCHSupport */
#define AT_DEST_HSDPA_ENABLE 1
#define AT_DEST_HSDPA_DISABLE 0

/* enEDCHSupport */
#define AT_DEST_HSUPA_ENABLE 1
#define AT_DEST_HSUPA_DISABLE 0

/* Source WCDMA RRC Version */
#define AT_SRC_ONLY_WCDMA 0
#define AT_SRC_NOT_SUPPORT_HSUPA 1
#define AT_SRC_ALL_SUPPORT 2

/* �������ú������͵���չλ�����ڵײ���뺯��ͳһ������extλ�����ϱ�ʱͳһ���� */
#define AT_NUMBER_TYPE_EXT 0x80

#define AT_NUMBER_TYPE_UNKOWN 128;

#define AT_AP_RATE_STRLEN 16

#define AT_FASTDORM_DEFAULT_TIME_LEN 5

#define AT_FACTORY_INFO_LEN 78
#define AT_MMI_TEST_FLAG_OFFSET 24
#define AT_MMI_TEST_FLAG_LEN 4
#define AT_MMI_TEST_FLAG_SUCC "ST P"

#define AT_HUK_PARA_LEN 32                    /* ^HUK����������� */
#define AT_FACAUTHPUBKEY_PARA_LEN 1040        /* ^FACAUTHPUBKEY���Կ�������� */
#define AT_FACAUTHPUBKEY_SIGN_PARA_LEN 64     /* ^FACAUTHPUBKEY���Կǩ���������� */
#define AT_RSA_CIPHERTEXT_PARA_LEN 512        /* RSA���Ĳ������� */
#define AT_RSA_TLV_CIPHERTEXT_PARA_LEN 1024   /* RSA���Ĳ���TLV��ʽ���� */

#define AT_PERSONALIZATION_CODE_LEN 8               /* ��������ͨ�������Ŷγ��� */
#define AT_PERSONALIZATION_CODE_LEN_EX 10           /* ��չ����������ͨ�������Ŷγ��� */
#define AT_PERSONALIZATION_CP_CODE_LEN 10           /* ��������CP���������Ŷγ��� */
#define AT_PERSONALIZATION_NET_CODE_LEN 6           /* ��������NET���������Ŷγ��� */
#define AT_PERSONALIZATION_NET_CODE_BCD_LEN 3       /* ��������NET���������Ŷ�BCD����ĳ��� */
#define AT_PERSONALIZATION_SUBNET_CODE_BCD_LEN 4    /* ��������NETSUB���������Ŷ�BCD����ĳ��� */
#define AT_PERSONALIZATION_SP_CODE_BCD_LEN 4        /* ��������SP���������Ŷ�BCD����ĳ��� */
#define AT_PERSONALIZATION_CP_CODE_BCD_LEN 5        /* ��������CP���������Ŷ�BCD����ĳ��� */
#define AT_PERSONALIZATION_CODE_FOURTH_CHAR_INDEX 3 /* ���������Ŷε��ĸ��ַ������� */
#define AT_NVWRSECCTRL_PARA_SECTYPE_LEN 1           /* ^NVWRSECCTRL��ȫ�������Ͳ������� */

#define AT_APSEC_CMD_MAX_LEN 553 /* "AT^APSEC=<SP_request>"9+544 ������󳤶� */
#define AT_APSEC_CMD_MIN_LEN 21  /* "AT^APSEC=<SP_request>"9+12  ������С���� */

#define AT_BODYSARWCDMA_MAX_PARA_NUM 16
#define AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM 8
#define AT_BODYSARGSM_MAX_PARA_NUM 16
#define AT_BODYSARGSM_MAX_PARA_GROUP_NUM 8
#define AT_RF_BAND_ANY_STR "3FFFFFFF"
#define AT_RF_BAND_ANY 0x3FFFFFFF
#define AT_WCDMA_BAND_DEFAULT_POWER 24
#define AT_GSM_GPRS_850_DEFAULT_POWER 33
#define AT_GSM_EDGE_850_DEFAULT_POWER 27
#define AT_GSM_GPRS_900_DEFAULT_POWER 33
#define AT_GSM_EDGE_900_DEFAULT_POWER 27
#define AT_GSM_GPRS_1800_DEFAULT_POWER 30
#define AT_GSM_EDGE_1800_DEFAULT_POWER 26
#define AT_GSM_GPRS_1900_DEFAULT_POWER 30
#define AT_GSM_EDGE_1900_DEFAULT_POWER 26
#define AT_GSM_GPRS_BAND_OFFSET 0
#define AT_GSM_EDGE_BAND_OFFSET 16

#define AT_CERSSI_MAX_TIMER_INTERVAL 20  // �ź������ϱ�������ʱ��20S

/* �������ݲ���3�ı���ʱ ����ַ��������'='�� */
#define AT_BASE64_PADDING '='

/* ��ӽ��뼼��ƽ���acting plmn�ļ�ID�� */
#define USIMM_ATT_RAT_BALANCING_FID 0x4F36
#define USIMM_ATT_ACTING_PLMN_FID 0x4F34

#define AT_MEID_OCTET_NUM 7

#define AT_PLMN_STR_MAX_LEN 6
#define AT_PLMN_STR_MIN_LEN 5
#define AT_DECIMAL_BASE_NUM 10
#define AT_HEX_BASE_NUM 16
#define AT_DOUBLE_LENGTH 2
#define AT_THREE_TIMES_LENGTH 3
#define AT_ASCII_AND_HEX_CONVERSION_FACTOR 2     /* ASCII���ʮ�����Ƶ�ת��ϵ�� */
#define AT_DOUBLE_QUO_MARKS_LENGTH 2             /* ˫���ŵĳ��� */
#define AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE 0x30 /* ʮ���������ֺ�ASCII������ת���Ĺ̶�����ֵ */


enum B64ReultType {
    b64Result_OK = 0,
    b64Result_CODE_SIZE_ERROR,
    b64Result_DATA_SIZE_SMALLNESS,
    b64Result_CODE_ERROR
};

#define AT_RSFR_RSFW_NAME_LEN 7    /* ĿǰNAMEֻ֧��(VERSION/SIMLOCK/vSIM) */
#define AT_RSFR_RSFW_SUBNAME_LEN 7 /* SUBNAME���Ȳ�����7 */

/* (TAF_PH_SIMLOCK_PLMN_STR_LEN*2+1)*TAF_MAX_SIM_LOCK_RANGE_NUM ���� 340 */
/* SIMLOCK���ַ������340���ַ��������Ҳ���ᳬ��1024 */
#define AT_RSFR_RSFW_MAX_LEN 1024

/* RNIC����������󳤶� */
#define AT_RMNET_NAME_MAX_LENGTH 8

/* ��ʵ�������֧������RNIC���� */
#define AT_RMNET_MAX_COUNT 3

#define AT_CFREQLOCK_PARA_NUM_MIN 1
#define AT_CFREQLOCK_PARA_NUM_MAX 9

#define AT_MBMS_SERVICE_ID_LENGTH 6
#define AT_MBMS_MCC_LENGTH 3
#define AT_MBMS_MNC_MAX_LENGTH 3
#define AT_MBMS_SERVICE_ID_AND_MCC_LENGTH 9
#define AT_MBMS_TMGI_MAX_LENGTH 12

#define AT_MBMS_UTC_MAX_LENGTH 12

typedef VOS_UINT8 AT_MBMS_TYPE;
#define AT_MBMS_SERVICE_ENABLER_TYPE 0
#define AT_MBMS_ACTIVATE_TYPE 1
#define AT_MBMS_DEACTIVATE_TYPE 2
#define AT_MBMS_DEACTIVATE_ALL_TYPE 3
#define AT_MBMS_PREFERENCE_TYPE 4
#define AT_MBMS_SIB16_GET_NETWORK_TIME_TYPE 5
#define AT_MBMS_BSSI_SIGNAL_LEVEL_TYPE 6
#define AT_MBMS_NETWORK_INFORMATION_TYPE 7
#define AT_MBMS_MODEM_STATUS_TYPE 8

typedef VOS_UINT8 AT_COEX_PARA_TYPE;
#define AT_COEX_PARA_COEX_ENABLE 0
#define AT_COEX_PARA_TX_BEGIN 1
#define AT_COEX_PARA_TX_END 2
#define AT_COEX_PARA_TX_POWER 3
#define AT_COEX_PARA_RX_BEGIN 4
#define AT_COEX_PARA_RX_END 5

#define AT_COEX_PARA_COEX_ENABLE_MIN 0
#define AT_COEX_PARA_COEX_ENABLE_MAX 1
#define AT_COEX_PARA_TX_BEGIN_MIN 0
#define AT_COEX_PARA_TX_BEGIN_MAX 65535
#define AT_COEX_PARA_TX_END_MIN 0
#define AT_COEX_PARA_TX_END_MAX 65535
#define AT_COEX_PARA_TX_POWER_MIN (-32767)
#define AT_COEX_PARA_TX_POWER_MAX 32767
#define AT_COEX_PARA_RX_BEGIN_MIN 0
#define AT_COEX_PARA_RX_BEGIN_MAX 65535
#define AT_COEX_PARA_RX_END_MIN 0
#define AT_COEX_PARA_RX_END_MAX 65535

/* ��װOSA������Ϣ�ӿ� */
#define AT_ALLOC_MSG_WITH_HDR(msg_len) TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, (msg_len))

/* ��װOSA��Ϣͷ */
#if (VOS_OS_VER == VOS_WIN32)
#define AT_CFG_MSG_HDR(msg, rcv_pid, msg_id) do { \
    ((MSG_Header *)(msg))->ulSenderCpuId   = VOS_LOCAL_CPUID; \
    ((MSG_Header *)(msg))->ulSenderPid     = WUEPS_PID_AT;    \
    ((MSG_Header *)(msg))->ulReceiverCpuId = VOS_LOCAL_CPUID; \
    ((MSG_Header *)(msg))->ulReceiverPid   = (rcv_pid);       \
    ((MSG_Header *)(msg))->msgName         = (msg_id);        \
} while (0)
#else
#define AT_CFG_MSG_HDR(msg, rcv_pid, msg_id) do { \
    VOS_SET_SENDER_ID(msg, WUEPS_PID_AT);     \
    VOS_SET_RECEIVER_ID(msg, (rcv_pid));      \
    ((MSG_Header *)(msg))->msgName = (msg_id); \
} while (0)
#endif

/* ��װOSA��Ϣͷ(AT�ڲ���Ϣ) */
#define AT_CFG_INTRA_MSG_HDR(msg, msg_id) AT_CFG_MSG_HDR(msg, WUEPS_PID_AT, msg_id)

/* ��װOSA��Ϣͷ(RNIC��Ϣ) */
#define AT_CFG_RNIC_MSG_HDR(msg, msg_id) AT_CFG_MSG_HDR(msg, ACPU_PID_RNIC, msg_id)

/* ��װOSA��Ϣͷ(NDIS��Ϣ) */
#define AT_CFG_NDIS_MSG_HDR(msg, msg_id) AT_CFG_MSG_HDR(msg, PS_PID_APP_NDIS, msg_id)

/* ��ȡOSA��Ϣ���� */
#define AT_GET_MSG_ENTITY(msg) ((VOS_VOID *)&(((MSG_Header *)(msg))->msgName))

/* ��ȡOSA��Ϣ���� */
#define AT_GET_MSG_LENGTH(msg) (VOS_GET_MSG_LEN(msg))

/* ��װOSA������Ϣ�ӿ� */
#define AT_SEND_MSG(msg)                                 \
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {      \
        AT_ERR_LOG("AT_SEND_MSG: Send message fail!\n"); \
    }

#define AT_CGCATT_MODE_PS 1
#define AT_CGCATT_MODE_CS 2
#define AT_CGCATT_MODE_CS_PS 3

#define AT_COEX_INVALID 0x00ff0000

#define AT_CCONTDTMF_PARA_NUM_MIN 2
#define AT_CCONTDTMF_PARA_NUM_MAX 3

#define AT_BODY_SAR_GBAND_GPRS_850_MASK 0x00000001
#define AT_BODY_SAR_GBAND_GPRS_900_MASK 0x00000002
#define AT_BODY_SAR_GBAND_GPRS_1800_MASK 0x00000004
#define AT_BODY_SAR_GBAND_GPRS_1900_MASK 0x00000008
#define AT_BODY_SAR_GBAND_EDGE_850_MASK 0x00010000
#define AT_BODY_SAR_GBAND_EDGE_900_MASK 0x00020000
#define AT_BODY_SAR_GBAND_EDGE_1800_MASK 0x00040000
#define AT_BODY_SAR_GBAND_EDGE_1900_MASK 0x00080000

#define AT_SPY_TEMP_THRESHOLD_PARA_CHANGE 1   /* ��Ҫ���¸ò��� */
#define AT_SPY_TEMP_THRESHOLD_PARA_UNCHANGE 0 /* ������¸ò��� */

#if (FEATURE_LTE == FEATURE_ON)
#define AT_UE_LTE_CATEGORY_NUM_MAX 22 /* LTE��֧�ֵ����category���� */
#define AT_GetLteUeDlCategoryIndex() (g_atDlRateCategory.lteUeDlCategory)
#define AT_GetLteUeUlCategoryIndex() (g_atDlRateCategory.lteUeUlCategory)
#endif

#define AT_GET_INTERNAL_TIMEOUT_RSLT_FUNC_TBL_PTR() (g_atInterTimeoutResultTab)
#define AT_GET_INTERNAL_TIMEOUT_RSLT_FUNC_TBL_SIZE() (AT_ARRAY_SIZE(g_atInterTimeoutResultTab))

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#define AT_SINGLE_S_NSSAI_MAX_STR_LEN 20
#define AT_EVT_MULTI_S_NSSAI_LEN 512
#define AT_EAP_MSG_LEN 1500

#define AT_EVT_MULTI_S_EAPMSG_LEN 1500

#define AT_UE_POLICY_RSP_MAX_LEN 1000
#endif

#if (FEATURE_ECALL == FEATURE_ON)
#define AT_IMSA_ECALL_CONTENT_TYPE_MODE_CS_PREFER 0 /* ǿ��cs prefer */
#define AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_CUSTOM 1 /* ����ecall over imsģʽ������Я��content type */
#define AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_STANDARD 2 /* ��׼ecall over imsģʽ������ҪЯ��content type */
#define AT_IMSA_ECALL_CONTENT_TYPE_MODE_BUTT 3 /* ��Чֵ */
#endif

#define AT_CME_ERROR_STR_LEN 12
#define AT_CMS_ERROR_STR_LEN 12
#define AT_CRLF_STR_LEN 2


#define AT_SEND_DATA_BUFF_OFFSET 3

enum AT_CliValidity {
    AT_CLI_VALIDITY_VALID    = 0,
    AT_CLI_VALIDITY_USR_REJ  = 1,
    AT_CLI_VALIDITY_INTERACT = 2,
    AT_CLI_VALIDITY_PAYPHONE = 3,
    AT_CLI_VALIDITY_UNAVAL   = 4,
    AT_CLI_VALIDITY_BUTT
};
typedef VOS_UINT8 AT_CliValidityUint8;

/*
 * Э����:
 * ö��˵��: ��ѡ������
 */
enum AT_VOICE_DomainType {
    AT_VOICE_DOMAIN_TYPE_CS_ONLY          = 1,
    AT_VOICE_DOMAIN_TYPE_CS_PREFERRED     = 2,
    AT_VOICE_DOMAIN_TYPE_IMS_PS_PREFERRED = 3,
    AT_VOICE_DOMAIN_TYPE_IMS_PS_ONLY      = 4,
    AT_VOICE_DOMAIN_TYPE_BUTT
};
typedef VOS_UINT32 AT_VOICE_DomainTypeUint32;

/*
 * �ṹ˵��:
 *             0: AT_TEMPPRT_STATUS_IND_DISABLE  �±�״̬�ϱ�NV��ر�
 *             1: AT_TEMPPRT_STATUS_IND_ENABLE   �±�״̬�ϱ�NV���
 */
enum AT_TempprtStatusInd {
    AT_TEMPPRT_STATUS_IND_DISABLE = 0,
    AT_TEMPPRT_STATUS_IND_ENABLE,
    AT_TEMPPRT_STATUS_IND_BUTT
};
typedef VOS_UINT32 AT_TempprtStatusIndUint32;

/*
 * �ṹ˵��: ^RSIM �����ѯ����б�
 *           0: û���κο��Ӵ��ϣ�
 *           1: �Ѿ��Ӵ���SIM/USIM/UIM��
 *           2:SIM/USIM/UIM��æ��Ҫ�ȴ���
 */
enum AT_RsimStatus {
    AT_RSIM_STATUS_USIMM_CARD_ABSENT    = 0x00,
    AT_RSIM_STATUS_USIMM_CARD_AVAILABLE = 0x01,
    AT_RSIM_STATUS_USIMM_CARD_BUSY      = 0x02,
    AT_RSIM_STATUS_USIMM_CARD_BUTT      = 0xff
};
typedef VOS_UINT8 AT_RsimStatusUint8;

/*
 * Э����:
 * ö��˵��: ���������
 */
enum AT_PhynumType {
    AT_PHYNUM_TYPE_IMEI,
    AT_PHYNUM_TYPE_MEID,
    AT_PHYNUM_TYPE_MAC,
    AT_PHYNUM_TYPE_ESN,
    AT_PHYNUM_TYPE_SVN,
    AT_PHYNUM_TYPE_UMID,
    AT_PHYNUM_TYPE_BUTT
};

/* SMS version PHASE2�汾 SMS PHASE2+�汾 */
enum AT_CSMS_MsgVersion {
    AT_CSMS_MSG_VERSION_PHASE2,
    AT_CSMS_MSG_VERSION_PHASE2_PLUS
};
typedef TAF_UINT8 AT_CSMS_MsgVersionUint8;

/* AT TIMER ID list */
enum AT_TIMER_Type {
    /* ��������صļ�ʱ�����ڴ���AT����ʱʹ�� */
    AT_CMD_PROCESS_TYPE = 0x0000,
    /* AT�ڲ�����ļ�ʱ�������ں������������ */
    AT_INTERNAL_PROCESS_TYPE = 0x0100
};

enum AT_CLCC_Mode {
    AT_CLCC_MODE_VOICE, /* voice */
    AT_CLCC_MODE_DATA,  /* data */
    AT_CLCC_MODE_FAX,   /* fax */
    /* voice followed by data, voice mode */
    AT_CLCC_MODE_VFDV,
    /* alternating voice/data, voice mode */
    AT_CLCC_MODE_AVDV,
    /* alternating voice/fax, voice mode */
    AT_CLCC_MODE_AVFV,
    /* voice followed by data, data mode */
    AT_CLCC_MODE_VFDD,
    /* alternating voice/data, data mode */
    AT_CLCC_MODE_AVDD,
    AT_CLCC_MODE_AVFF,   /* alternating voice/fax, fax mode */
    AT_CLCC_MODE_UNKNOWN /* unknown */
};
typedef TAF_UINT8 AT_CLCC_ModeUint8;

enum AT_CPAS_Status {
    /* ready (MT allows commands from TA/TE) */
    AT_CPAS_STATUS_READY,
    /* unavailable (MT does not allow commands from TA/TE */
    AT_CPAS_STATUS_UNAVAILABLE,
    /* unknown (MT is not guaranteed to respond to instructions) */
    AT_CPAS_STATUS_UNKNOWN,
    /* ringing (MT is ready for commands from TA/TE, but the ringer is active) */
    AT_CPAS_STATUS_RING,
    /* call in progress (MT is ready for commands from TA/TE, but a call is in progress) */
    AT_CPAS_STATUS_CALL_IN_PROGRESS,
    /* asleep (MT is unable to process commands from TA/TE because it is in a low functionality state) */
    AT_CPAS_STATUS_ASLEEP
};
typedef TAF_UINT8 AT_CPAS_StatusUint8;

enum AT_WifiMode {
    AT_WIFI_MODE_ONLY_PA   = 0x00, /* WIFIֻ֧��PAģʽ */
    AT_WIFI_MODE_ONLY_NOPA = 0x01, /* WIFIֻ֧��NO PAģʽ */
    AT_WIFI_MODE_PA_NOPA   = 0x02, /* WIFIͬʱ֧��PAģʽ��NO PAģʽ */

    AT_WIFI_MODE_BUTT = 0xFF
};
typedef VOS_UINT8 AT_WifiModeUint8;


enum AT_GsmBand {
    AT_GSM_850 = 0,
    AT_GSM_900,
    AT_GSM_1800,
    AT_GSM_1900,
    AT_GSM_BAND_BUTT
};
typedef VOS_UINT16 AT_GsmBandUint16;

#if (FEATURE_LTEV_WL == FEATURE_ON)
enum RESTORE_STATE_ENUM {
    RESTORE_FAIL       = 0, /* �ָ���������ʧ�� */
    RESTORE_OK         = 1, /* �ָ���������ok */
    RESTORE_PROCESSING = 2, /* �ָ��������ý����� */
    RESTORE_BUTT
};
typedef VOS_UINT32 RESTORE_STATE_ENUM_Uint32;

enum SET_FTYRESET_ENUM {
    NOT_SET  = 0, /* δ�·�ftyreset�������� */
    SET_DONE = 1, /* ���·�ftyreset�������� */
    SET_FTYRESET_BUTT
};
typedef VOS_UINT32 SET_FTYRESET_ENUM_Uint32;

typedef enum {
    REVERT_FAIL    = 0,
    REVERT_SUCCESS = 1
} REVERT_NV_FLAG;
#endif
typedef enum {
    AT_CMD_BUTT,
    AT_CMD_APHPLMN,
    AT_CMD_ANQUERY,
    AT_CMD_CGDATA,
    AT_CMD_CGPADDR,
    AT_CMD_CGEQNEG,
    AT_CMD_CGDNS,
    AT_CMD_CEER,
    AT_CMD_CPAS,
    AT_CMD_CGMI,
    AT_CMD_CGMM,
    AT_CMD_GMM,
    AT_CMD_CGMR,
    AT_CMD_CGSN,
    AT_CMD_SN,
    AT_CMD_HWVER,
    AT_CMD_HVER,
    AT_CMD_FHVER,
    AT_CMD_PFVER,
    AT_CMD_CLAC,
    AT_CMD_GSN,
    AT_CMD_GCAP,
    AT_CMD_SRVST,
    AT_CMD_RSSI,
    AT_CMD_CERSSI,
    AT_CMD_CNMR,
    AT_CMD_CECELLID,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_CSERSSI,
    AT_CMD_C5GOPTION,
    AT_CMD_NRBANDBLOCKLIST,
    AT_CMD_NRSSBID,
    AT_CMD_NWDEPLOYMENT,
#endif
    AT_CMD_CESQ,
    AT_CMD_LFROMCONNTOIDLE,
    AT_CMD_LWTHRESHOLDCFG,
    AT_CMD_SYSMODE,
    AT_CMD_SIMST,
    AT_CMD_HS,
    AT_CMD_I,
    AT_CMD_T,
    AT_CMD_P,
    AT_CMD_X,
    AT_CMD_Z,
    AT_CMD_Q,
    AT_CMD_CIMI,
    AT_CMD_VTS,
    AT_CMD_DTMF,
    AT_CMD_CGANS,
    AT_CMD_CGANS_EXT,
    AT_CMD_APENDPPP,
    AT_CMD_CGCMOD,
    AT_CMD_CGACT,
    AT_CMD_CGDCONT,
    AT_CMD_CGDSCONT,
    AT_CMD_DSFLOWCLR,
    AT_CMD_DSFLOWQRY,
    AT_CMD_DSFLOWRPT,
    AT_CMD_CGTFT,
    AT_CMD_CGEQREQ,
    AT_CMD_CGEQMIN,
    AT_CMD_COPS,
    AT_CMD_CGAUTO,
    AT_CMD_CPBS,
    AT_CMD_CPBR,
    AT_CMD_CPBR2,
    AT_CMD_CPBF,
    AT_CMD_CPBW,
    AT_CMD_CPBW2,
    AT_CMD_SCPBW,
    AT_CMD_SCPBR,
    AT_CMD_CNUM,
    AT_CMD_CMEE,
    AT_CMD_CSCS,
    AT_CMD_CSIM,
    AT_CMD_CCHO,
    AT_CMD_CCHP,
    AT_CMD_CCHC,
    AT_CMD_CGLA,
    AT_CMD_CPIN,
    AT_CMD_CARDLOCK,
    AT_CMD_CARDATR,
    AT_CMD_CPIN_2,
    AT_CMD_CICCID,
    AT_CMD_CPWD,
    AT_CMD_CGCLASS,
    AT_CMD_CLIP,
    AT_CMD_CLIR,
    AT_CMD_COLP,
    AT_CMD_CLCC,
    AT_CMD_CLCK,
    AT_CMD_CTFR,
    AT_CMD_CMLCK,
    AT_CMD_CGSMS,
    AT_CMD_CGATT,
    AT_CMD_CGCATT,
    AT_CMD_CFUN,
    AT_CMD_CSQ,
    AT_CMD_CBC,
    AT_CMD_CBND,
    AT_CMD_CPDW,
    AT_CMD_NVIM,
    AT_CMD_DAM_PARASTUB,
    AT_CMD_ENOS_PARASTUB,
    AT_CMD_COMPARE_PLMN_SUPPORT_WILDCARD,
    AT_CMD_USIM_STUB,
    AT_CMD_REFRESH_STUB,
    AT_CMD_AUTO_RESEL_STUB,
    AT_CMD_DELAYBG_STUB,
    AT_CMD_PIDREINIT,
    AT_CMD_IMSRATSTUB,
    AT_CMD_IMSCAPSTUB,
    AT_CMD_DOMAINSTUB,
    AT_CMD_USIM,
    AT_CMD_SIM,
    AT_CMD_PDPSTUB,
    AT_CMD_CSND,
    AT_CMD_CSTA,
    AT_CMD_CCUG,
    AT_CMD_CGREG,
    AT_CMD_CEREG,
    AT_CMD_C5GREG,
    AT_CMD_CCWA,
    AT_CMD_CCFC,
    AT_CMD_CUSD,
    AT_CMD_CSSN,
    AT_CMD_CREG,
    AT_CMD_S0,
    AT_CMD_S3,
    AT_CMD_S4,
    AT_CMD_S5,
    AT_CMD_S6,
    AT_CMD_S7,
    AT_CMD_V,
    AT_CMD_E,
    AT_CMD_L,
    AT_CMD_M,
    AT_CMD_AMP_C,
    AT_CMD_AMP_D,
#if (FEATURE_AT_HSUART == FEATURE_ON)
    AT_CMD_AMP_S,
#endif
    AT_CMD_AMP_F,
    AT_CMD_CRC,
    AT_CMD_CMOD,
    AT_CMD_CMMI,
    AT_CMD_CBST,
    AT_CMD_CHUP,
    AT_CMD_D,
    AT_CMD_O,
    AT_CMD_H,
    AT_CMD_A,
    AT_CMD_CHLD,
    AT_CMD_ALS,
    AT_CMD_CUUS1,
    AT_CMD_CPAM,
    AT_CMD_CCIN,
    AT_CMD_TIME,
    AT_CMD_CTZR,

    AT_CMD_CPLS,
    AT_CMD_CPOL,
    AT_CMD_CSIN,
    AT_CMD_CSTR,
    AT_CMD_CSEN,
    AT_CMD_CSMN,
    AT_CMD_CSTC,
    AT_CMD_STSF,
#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
    AT_CMD_STGI,
    AT_CMD_STGR,
#endif
    AT_CMD_IMSICHG,
    AT_CMD_TEST,
    AT_CMD_CWAS,
    AT_CMD_CGAS,
    AT_CMD_APPDIALMODE,
    AT_CMD_LCSTARTTIME,
    AT_CMD_MAXFREELOCKSIZE,
    AT_CMD_CPULOAD,
    AT_CMD_CELLINFO,
    AT_CMD_DELWCELLENTITY,
    AT_CMD_MEANRPT,
    AT_CMD_AT2OM,
    AT_CMD_U2DIAG,
    AT_CMD_DWINS,
    AT_CMD_YJCX,
    AT_CMD_SETPID,
    AT_CMD_GETPORTMODE,
    AT_CMD_CVOICE,
    AT_CMD_DDSETEX,
    AT_CMD_NDISDUP,
    AT_CMD_AUTHDATA,
    AT_CMD_DHCP,
    AT_CMD_CRPN,
    AT_CMD_NDISSTATQRY,
    AT_CMD_DLOADVER,
    AT_CMD_DLOADINFO,
    AT_CMD_NVBACKUP,
    AT_CMD_NVRESTORE,
    AT_CMD_AUTHORITYVER,
    AT_CMD_AUTHORITYID,
    AT_CMD_GODLOAD,
    AT_CMD_RESET,
    AT_CMD_NVRSTSTTS,
    AT_CMD_HWNATQRY,
    AT_CMD_FLASHINFO,
    AT_CMD_CMSR,
    AT_CMD_CMGI,
    AT_CMD_CMMT,
    AT_CMD_USSDMODE,
    AT_CMD_ADCTEMP,
    AT_CMD_COMM,
    AT_CMD_HSPA,
    AT_CMD_CSNR,
    AT_CMD_FREQLOCK,
    AT_CMD_CPSC,
    AT_CMD_CFRQ,
    AT_CMD_CFPLMN,
    AT_CMD_CQST,
    AT_CMD_CAATT,
    AT_CMD_SYSINFO,
    AT_CMD_SYSINFOEX,
    AT_CMD_CEMODE,
    AT_CMD_LTECS,

    AT_CMD_SYSCFG,
    AT_CMD_SYSCFGEX,
    AT_CMD_CMM,
    AT_CMD_SPN,
    AT_CMD_CARDMODE,
    AT_CMD_DIALMODE,
    AT_CMD_CURC,
    AT_CMD_BOOT,
    AT_CMD_CIMEI,
    AT_CMD_TRIG,
    AT_CMD_GCFIND,
    AT_CMD_MDATE,
    AT_CMD_FACINFO,
    AT_CMD_RXPRI,
    AT_CMD_SIMLOCK,
    AT_CMD_MAXLCK_TIMES,
    AT_CMD_PHYNUM,
    AT_CMD_RSIM,
    AT_CMD_GTIMER,
    AT_CMD_PLATFORM,
    AT_CMD_CSVER,
    AT_CMD_QOS,
    AT_CMD_SDOMAIN,
    AT_CMD_DPACAT,
    AT_CMD_HSSPT,
    AT_CMD_BSN,
    AT_CMD_SFM,
    AT_CMD_TMODE,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_CMD_VMODE,
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_SRCID,
#endif
#endif
    AT_CMD_FCHAN,
    AT_CMD_FTXON,
    AT_CMD_FDAC,
    AT_CMD_CLT,
    AT_CMD_CLTINFO,
    AT_CMD_DCXOTEMPCOMP,

    AT_CMD_FTEMPRPT,

    AT_CMD_SD,
    AT_CMD_INFORRS,
    AT_CMD_INFORBU,
    AT_CMD_DATALOCK,
    AT_CMD_GPIOPL,
    AT_CMD_VERSION,

    AT_CMD_FRXON,
    AT_CMD_FPA,
    AT_CMD_FLNA,
    AT_CMD_FRSSI,
    AT_CMD_RXDIV,
    AT_CMD_CALLSRV,
    AT_CMD_CSDFLT,
    AT_CMD_SECUBOOTFEATURE,
    AT_CMD_SECUBOOT,
    AT_CMD_SECDBGSTATE,
    AT_CMD_AUTHKEYRD,
    AT_CMD_SETKEY,
    AT_CMD_GETKEYINFO,
    AT_CMD_WIENABLE,
    AT_CMD_WIMODE,
    AT_CMD_WIBAND,
    AT_CMD_WIFREQ,
    AT_CMD_WIRATE,
    AT_CMD_WIPOW,
    AT_CMD_WITX,
    AT_CMD_WIRX,
    AT_CMD_WIRPCKG,
    AT_CMD_TMMI,
    AT_CMD_TSELRF,
    AT_CMD_TCHRENABLE,
    AT_CMD_TCHRINFO,
    AT_CMD_CDUR,
    AT_CMD_TSCREEN,
    AT_CMD_SSID,
    AT_CMD_WIKEY,
    AT_CMD_WILOG,
    AT_CMD_WIPARANGE,
    AT_CMD_WIINFO,
    AT_CMD_WUPWD,
    AT_CMD_BATVOL,
    AT_CMD_PRODTYPE,
    AT_CMD_FEATURE,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CTA,
#endif
    AT_CMD_CGMTU,

    /* װ�� ����У׼ end */

    /*  AT_CMD_FCHAN, */
    /*  AT_CMD_FTXON, */
    /*  AT_CMD_FRXON, */
    AT_CMD_FANT,
    /*  AT_CMD_FPA, */
    /*  AT_CMD_FDAC, */
    /*  AT_CMD_FLNA, */
    /*  AT_CMD_FRSSI, */

    /* �������۲�start */
    AT_CMD_SRXPOW,
    AT_CMD_SMS_BEGAIN,
    AT_CMD_CSMS,
    AT_CMD_CPMS,
    AT_CMD_CMSQ,
    AT_CMD_CMGF,
    AT_CMD_CSCA,
    AT_CMD_CSMP,
    AT_CMD_CSDH,
    AT_CMD_CNMI,
    AT_CMD_CMGL,
    AT_CMD_CMGR,
    AT_CMD_CMGS,
    AT_CMD_CMGW,
    AT_CMD_CMGD,
    AT_CMD_CSSM,
    AT_CMD_CMGC,
    AT_CMD_CMSS,
    AT_CMD_CMST,
    AT_CMD_CNMA,
    AT_CMD_CCNMA,
    AT_CMD_CMSTUB,
    AT_CMD_CMMS,
    AT_CMD_RSTRIGGER,
#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
    AT_CMD_CSCB,
#endif
    AT_CMD_SMS_END,
    AT_CMD_CLVL,
    AT_CMD_VMSET,
    AT_CMD_CRSM,
    AT_CMD_PNN,
    AT_CMD_CPNN,
    AT_CMD_OPL,
    AT_CMD_GLASTERR,
    AT_CMD_PCSCINFO,
    AT_CMD_CELLSRCH,
    AT_CMD_CCC,
    AT_CMD_PORT,
    AT_CMD_APDIALMODE,
    AT_CMD_ACPUMEMINFO,
    AT_CMD_CCPUMEMINFO,
    AT_CMD_APCONNST,
    AT_CMD_APPWRONREG,
    AT_CMD_GMI,
    AT_CMD_GMR,
    AT_CMD_WIFIGLOBALMAC,
    AT_CMD_CSQLVL,
    AT_CMD_SCID,
    AT_CMD_CSQLVLEXT,
    AT_CMD_DISSD,
    AT_CMD_SDREBOOT,
    AT_CMD_DOCK,
    AT_CMD_APPDMVER,
    AT_CMD_OPWORD,
    AT_CMD_CPWORD,
    AT_CMD_DISLOG,
#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
    AT_CMD_SHELL,
    AT_CMD_SPWORD,
#endif
    AT_CMD_RSRPCFG,
    AT_CMD_RSCPCFG,
    AT_CMD_ECIOCFG,
    AT_CMD_CELLROAM,

    AT_CMD_FRSTATUS,
    AT_CMD_GETEXBANDINFO,
    AT_CMD_GETEXBANDTESTINFO,
    AT_CMD_APBATLVL,
    AT_CMD_OPENPORT,
    AT_CMD_SDLOAD,
    AT_CMD_AUTHVER,
    AT_CMD_IPV6CAP,
    AT_CMD_DHCPV6,
    AT_CMD_APRAINFO,
    AT_CMD_APLANADDR,
    AT_CMD_IPV6TEMPADDR,
    AT_CMD_VERTIME,
    AT_CMD_PRODNAME,
    AT_CMD_FWAVE,
    AT_CMD_EQVER,
    AT_CMD_APRPTSRVURL,
    AT_CMD_APXMLINFOTYPE,
    AT_CMD_APXMLRPTFLAG,

    AT_CMD_FASTDORM,

    AT_CMD_CGTFTRDP,
    AT_CMD_CGEQOS,
    AT_CMD_CGEQOSRDP,
    AT_CMD_CGCONTRDP,
    AT_CMD_CGSCONTRDP,

    AT_CMD_APCRADWIFIBS,
    AT_CMD_APNDISCON,
    AT_CMD_APCRADETHSTA,
    AT_CMD_APFMCDHCPSTA,
    AT_CMD_APDIALM,
    AT_CMD_APWIFIBS,
    AT_CMD_APWIFISEC,
    AT_CMD_APDHCP,
    AT_CMD_APCURPROF,
    AT_CMD_APPROFRD,

    AT_CMD_MEMQUERY,
    AT_CMD_TBAT,
    AT_CMD_PSTANDBY,
    AT_CMD_WIWEP,
    AT_CMD_CMDLEN,

    AT_CMD_LOCINFO,
    AT_CMD_CIPHERQRY,
    AT_CMD_QRYNVRESUME,

    AT_CMD_NVBACKUPSTAT,
    AT_CMD_NANDBBC,
    AT_CMD_NANDVER,
    AT_CMD_CHIPTEMP,

    AT_CMD_CPIN2,
    AT_CMD_CSASM,
    AT_CMD_DNSQUERY,

    AT_CMD_APRPTPORTSEL,
    AT_CMD_CHDATA,
    AT_CMD_GPIO,
    AT_CMD_USBSWITCH,

    AT_CMD_RSFR,
    AT_CMD_RSFW,
    AT_CMD_ANTSTATE,
    AT_CMD_SARREDUCTION,

    AT_CMD_HUK,
    AT_CMD_FACAUTHPUBKEY,
    AT_CMD_IDENTIFYSTART,
    AT_CMD_IDENTIFYEND,
    AT_CMD_SIMLOCKNWDATAWRITE,
    AT_CMD_SIMLOCKDATAWRITEEX,
    AT_CMD_SIMLOCKDATAREADEX,

    AT_CMD_SIMLOCKDATAWRITE,
    AT_CMD_PHONESIMLOCKINFO,
    AT_CMD_SIMLOCKDATAREAD,
    AT_CMD_GETMODEMSCID,

    AT_CMD_PHONEPHYNUM,
    AT_CMD_IDENTIFYOTASTART,
    AT_CMD_IDENTIFYOTAEND,
    AT_CMD_SIMLOCKOTADATAWRITE,
    AT_CMD_FACAUTHPUBKEYEX,
    AT_CMD_APSIMST,

    AT_CMD_CMOLR,
    AT_CMD_CMOLRE,
    AT_CMD_CMTLR,
    AT_CMD_CMTLRA,

    AT_CMD_CPOS,
    AT_CMD_CPOSR,
    AT_CMD_XCPOSR,
    AT_CMD_CGPSCLOCK,

    AT_CMD_XCPOSRRPT,

    AT_CMD_CGPSINFO,
    AT_CMD_LOGCFG,
    AT_CMD_LOGCPS,
    AT_CMD_LOGSAVE,
    AT_CMD_LOGNVE,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CCMGS,
    AT_CMD_CCMGW,
    AT_CMD_CCMGD,
    AT_CMD_CCSASM,
#endif

#if (VOS_WIN32 == VOS_OS_VER)
    AT_CMD_SETRPLMN,
#endif

    AT_CMD_NVM,

    AT_CMD_CMUT,
    AT_CMD_CISA,
    AT_CMD_SIMLOCKUNLOCK,

    AT_CMD_APDS,
    AT_CMD_CLPR,
    AT_CMD_CCSERR,
    AT_CMD_WLTHRESHOLDCFG,
    AT_CMD_ACINFO,
    AT_CMD_CLTEROAMALLOW,

    AT_CMD_SWVER,

    AT_CMD_CBG,

    AT_CMD_MMPLMNINFO,

    AT_CMD_EONSUCS2,

    AT_CMD_PLMN,
    AT_CMD_XLEMA,
    AT_CMD_COPN,
    AT_CMD_BODYSARON,

    AT_CMD_BODYSARWCDMA,
    AT_CMD_BODYSARGSM,

    AT_CMD_IMEIVERIFY,
    AT_CMD_LOGPORT,

    AT_CMD_NVRD,
    AT_CMD_NVWR,
    AT_CMD_NVRDLEN,
    AT_CMD_NVRDEX,
    AT_CMD_NVWREX,

    AT_CMD_NVWRPART,

    AT_CMD_NCELLMONITOR,
    AT_CMD_USERSRVSTATE,

    AT_CMD_SIMSLOT,

    AT_CMD_SIMINSERT,

    AT_CMD_MODEMLOOP,

    AT_CMD_RELEASERRC,

    AT_CMD_REFCLKFREQ,

    AT_CMD_PULLOMLOG,

    AT_CMD_REJINFO,

    AT_CMD_PLMNSELEINFO,

    AT_CMD_DIESN,
    AT_CMD_HANDLEDECT,

    AT_CMD_HVSDH,
    AT_CMD_HVSST,
    AT_CMD_HVSCONT,
    AT_CMD_HVPDH,
    AT_CMD_SCICHG,
    AT_CMD_BWT,
    AT_CMD_HVCHECKCARD,
    AT_CMD_PASSTHROUGH,
#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
    AT_CMD_SINGLEMODEMDUALSLOT,
#endif

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
    AT_CMD_SILENTPIN,
    AT_CMD_SILENTPININFO,
#endif

    AT_CMD_ESIMCLEAN,
    AT_CMD_ESIMCHECK,
    AT_CMD_ESIMEID,
    AT_CMD_PKID,
    AT_CMD_ESIMSWITCH,

    AT_CMD_PRIVATECCHO,
    AT_CMD_PRIVATECCHP,

    AT_CMD_PRIVATECGLA,

    AT_CMD_EOPLMN,

    AT_CMD_NETSCAN,

#if (FEATURE_AT_HSUART == FEATURE_ON)
    AT_CMD_IPR,
    AT_CMD_ICF,
    AT_CMD_IFC,
#endif

    AT_CMD_FPLLSTATUS,

    AT_CMD_ECID,

    AT_CMD_SWITCH_UART,

    AT_CMD_MIPICLK,

    AT_CMD_CLCC_IMS,
#if (FEATURE_IMS == FEATURE_ON)
    AT_CMD_CIREG,
    AT_CMD_CIREP,
    AT_CMD_VOLTEIMPU,
    AT_CMD_VOLTEIMPI,
    AT_CMD_VOLTEDOMAIN,
    AT_CMD_CCWAI,
    AT_CMD_UICCAUTH,
    AT_CMD_CURSM,
    AT_CMD_KSNAFAUTH,
    AT_CMD_IMSCTRLMSG,
    AT_CMD_IMSREGDOMAIN,
    AT_CMD_IMSDOMAINCFG,
    AT_CMD_IMSEMCRDP,
    AT_CMD_FUSIONCALLRAW,
#endif


    /* GUģAT����͹������������ID���ֵ����������IDʱҪ��ӵ���IDǰ */

    AT_CMD_CIMSROAMINGSTUB,
    AT_CMD_CREDIALSTUB,
    AT_CMD_IMSVOICEINTERSYSLAUSTUB,
    AT_CMD_IMSVOICEMMENABLESTUB,

    AT_CMD_CIMSUSSDSTUB,

#if (FEATURE_IMS == FEATURE_ON)
    AT_CMD_CACMIMS,
#endif

    AT_CMD_DPDTTEST,
    AT_CMD_DPDT,
    AT_CMD_DPDTQRY,

    AT_CMD_TRXTAS,
    AT_CMD_TRXTASQRY,

    AT_CMD_FRFICMEM,

    AT_CMD_FSERDESRT,
    AT_CMD_SERDESTESTASYNC,

    AT_CMD_TFDPDT,
    AT_CMD_TFDPDTQRY,

    AT_CMD_CECALL,
    AT_CMD_ECLSTART,
    AT_CMD_ECLSTOP,
    AT_CMD_ECLCFG,
    AT_CMD_ECLMSD,
    AT_CMD_ECLPUSH,
    AT_CMD_ECLLIST,
    AT_CMD_ECLABORT,
    AT_CMD_ECLMODE,
    AT_CMD_ECLIMSCFG,
#if (FEATURE_DSDS == FEATURE_ON)
    AT_CMD_PSPROTECTMODE,
#endif
    AT_CMD_PHYINIT,

    AT_CMD_FPOWDET,

    AT_CMD_RATFREQLOCK,

#if (FEATURE_IMS == FEATURE_ON)
    AT_CMD_CALL_MODIFY_INIT,
    AT_CMD_CALL_MODIFY_ANS,
    AT_CMD_ECONFDIAL,
    AT_CMD_CLCCECONF,
    AT_CMD_ECONFERR,
    AT_CMD_ENCRYPT,
#endif
    AT_CMD_CHLD_EX,

    AT_CMD_CDMACONNST,

    AT_CMD_GSMFREQLOCK,

    AT_CMD_ACTIVEMODEM,

    AT_CMD_CCLK,

    AT_CMD_CTZU,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CFSH,

    AT_CMD_CRM,
#endif

    AT_CMD_CARDTYPE,

    AT_CMD_CARDTYPEEX,

    AT_CMD_CARDVOLTAGE,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CQOSPRI,

    AT_CMD_CBURSTDTMF,
#endif

    AT_CMD_QCCB,

    AT_CMD_EMCCBM,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CLOCINFO,
    AT_CMD_CSID,

    /* �Ƴ�CDMA���뿪�� */

    AT_CMD_CSIDLIST,
    AT_CMD_ECCALL,
    AT_CMD_ECCTRL,
    AT_CMD_ECCAP,
    AT_CMD_ECRANDOM,
    AT_CMD_ECKMC,
    AT_CMD_ECCTEST,
    AT_CMD_CTROAMINFO,
    AT_CMD_CTOOSCOUNT,

    AT_CMD_CCLPR,

    AT_CMD_CDMACSQ,
    AT_CMD_CFREQLOCK,

    AT_CMD_HDRCSQ,
#endif

    AT_CMD_APDSFLOWRPTCFG,

    AT_CMD_IMSSWITCH,
    AT_CMD_CEVDP,

    AT_CMD_DSFLOWNVWRCFG,
#if (FEATURE_HUAWEI_VP == FEATURE_ON)
    AT_CMD_VOICEPREFER,
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

    AT_CMD_CUSTOMDIAL,

    AT_CMD_CDMAMODEMSWITCH,
    AT_CMD_CDMACAPRESUME,
    AT_CMD_FACTORYCDMACAP,
#endif
    AT_CMD_TTYMODE,
    AT_CMD_FEMCTRL,

    AT_CMD_NVWRSECCTRL,

    AT_CMD_CRLA,
    AT_CMD_CARDSESSION,

    AT_CMD_RATRFSWITCH,

    AT_CMD_CCMGG,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_PRFAPP,
    AT_CMD_UICCPRFAPP,
    AT_CMD_CCIMI,

    AT_CMD_1XCHAN,
    AT_CMD_CVER,

    AT_CMD_GETSTA,
    AT_CMD_GETESN,
    AT_CMD_GETMEID,
    AT_CMD_CHIGHVER,
#endif

#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)
    AT_CMD_MBMSCMD,
    AT_CMD_MBMSEV,
    AT_CMD_MBMSINTERESTLIST,
#endif
    AT_CMD_LTELOWPOWER,
    AT_CMD_ISMCOEX,
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CAGPSCFGPOSMODE,
    AT_CMD_CAGPSSTART,
    AT_CMD_CAGPSSTOP,
    AT_CMD_CAGPSCFGMPCADDR,
    AT_CMD_CAGPSCFGPDEADDR,
    AT_CMD_CAGPSQRYREFLOC,
    AT_CMD_CAGPSQRYTIME,
    AT_CMD_CAGPSREPLYNIREQ,
    AT_CMD_CAGPSPRMINFO,
    AT_CMD_CAGPSDATACALLSTATUS,
    AT_CMD_CUPBINDSTATUS,
    AT_CMD_CAGPSFORWARDDATA,

    AT_CMD_CAGPSPOSINFO,
    AT_CMD_CGPSCONTROLSTART,
    AT_CMD_CGPSCONTROLSTOP,

    AT_CMD_MEID,
#endif

    AT_CMD_IMSPDPCFG,

    AT_CMD_TRANSMODE,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CCONTDTMF,

    AT_CMD_CDORMTIMER,
#endif
    AT_CMD_MCCFREQ,
    AT_CMD_CLOUDDATA,
    AT_CMD_BORDERINFO,
    AT_CMD_HPLMN,
    AT_CMD_DPLMNLIST,
    AT_CMD_EXCHANGE_MODEM_INFO,
    AT_CMD_CSGIDSEARCH,
    AT_CMD_CHISFREQ,
    AT_CMD_UE_CENTER,
    AT_CMD_QUICK_CARD_SWITCH,
    AT_CMD_MIPIWR,
    AT_CMD_MIPIRD,
    AT_CMD_SSIWR,
    AT_CMD_SSIRD,

    AT_CMD_MIPIWREX,
    AT_CMD_MIPIRDEX,

    AT_CMD_LOGENALBE,

    AT_CMD_ACTPDPSTUB,

    AT_CMD_MONSC,
    AT_CMD_MONNC,
    AT_CMD_NVCHK,

    AT_CMD_PDMCTRL,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CSNID,
#endif

    AT_CMD_AFCCLKINFO,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CPMP,
#endif

    AT_CMD_SECURESTATE,
    AT_CMD_KCE,
    AT_CMD_SOCID,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_DIVERSITYSWITCH,

    AT_CMD_DOSYSEVENT,
    AT_CMD_DOSIGMASK,
#endif

    AT_CMD_ROAMIMSSERVICE,

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
    AT_CMD_M2MFREQLOCK,
#endif

    AT_CMD_SENSOR,
    AT_CMD_SCREEN,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_PRLID,
    AT_CMD_CNOCARDMODE,

    AT_CMD_RATCOMBINEDMODE,
#endif

    AT_CMD_FRATIGNITION,

    AT_CMD_PORTCONCURRENT,
#if ((FEATURE_VSIM == FEATURE_ON) && (FEATURE_VSIM_ICC_SEC_CHANNEL == FEATURE_ON))
    AT_CMD_ICCVSIMVER,
#endif

    AT_CMD_EFLOCIINFO,
    AT_CMD_EFPSLOCIINFO,

    AT_CMD_NICKNAME,
    AT_CMD_BATTERYINFO,
    AT_CMD_VOLTEREG,

    AT_CMD_TIMESET,

    AT_CMD_FCLASS,
    AT_CMD_GCI,

    AT_CMD_RXTESTMODE,

    AT_CMD_BESTFREQ,

    AT_CMD_ULFREQ,

    AT_CMD_RFICID,

    AT_CMD_RFICIDEX,

    AT_CMD_RFFEID,

    AT_CMD_PHYCOMCFG,

    AT_CMD_CRRCONN,
    AT_CMD_VTRLQUALRPT,

    AT_CMD_PMUDIESN,

    AT_CMD_TAS_TEST,
    AT_CMD_TAS_TEST_QUERY,

    AT_CMD_IMSVIDEOCALLCANCEL,

    AT_CMD_PACSP,

    AT_CMD_REJCALL,
    AT_CMD_CSCHANNELINFO,
    AT_CMD_IMSVTCAPCFG,
    AT_CMD_IMSSMSCFG,
    AT_CMD_CNAP,
    AT_CMD_CNAPEX,

    AT_CMD_IMSREGERRRPT,

    AT_CMD_CIMSERR,

    AT_CMD_IMSIPCAPCFG,

    AT_CMD_RSRP,
    AT_CMD_RSRQ,

    AT_CMD_CACDC,

    AT_CMD_ERRCCAPCFG,
    AT_CMD_ERRCCAPQRY,

    AT_CMD_PSEUCELL,
    AT_CMD_BLOCKCELLLIST,

    AT_CMD_MIPIREAD,

    AT_CMD_PHYMIPIWRITE,

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)
    AT_CMD_DCXOQRY,
#endif

    AT_CMD_IMSSRVSTATRPT,
    AT_CMD_IMSSRVSTATUS,

    AT_CMD_CHRALARMRLATCFG,

    AT_CMD_ECCCFG,

    AT_CMD_EPDU,
    AT_CMD_LINEINDEXLIST,
    AT_CMD_LINEDETAIL,
#if (FEATURE_LTE == FEATURE_ON)
    AT_CMD_LCACFG,
    AT_CMD_LCACELLEX,
    AT_CMD_LCACELLRPTCFG,
    AT_CMD_FINE_TIME,

    AT_CMD_LL2COMCFG,
    AT_CMD_LL2COMQRY,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_CONNECT_RECOVERY,
#endif
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_LENDC,

    AT_CMD_NL2COMCFG,
    AT_CMD_NL2COMQRY,
    AT_CMD_NPDCP_SLEEPTHRES,

    AT_CMD_NRRCCAPCFG,
    AT_CMD_NRRCCAPQRY,
    AT_CMD_NRPWRCTRL,
    AT_CMD_NRPOWERSAVINGCFG,
    AT_CMD_NRPOWERSAVINGQRY,
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_NRFREQLOCK,
#endif
    AT_CMD_NRCACELL,
    AT_CMD_NRCACELLRPTCFG,
    AT_CMD_NRNWCAP,
    AT_CMD_NRNWCAPRPTCFG,
    AT_CMD_NRNWCAPRPTQRY,
    AT_CMD_NRNWCAPQRY,
#endif

    AT_CMD_VTFLOWRPT,

    AT_CMD_WIEMCAID,
    AT_CMD_DMRCSCFG,
    AT_CMD_RCSSWITCH,
    AT_CMD_USERAGENTCFG,

    AT_CMD_DATASWITCH,
    AT_CMD_DATAROAMSWITCH,

    AT_CMD_COMMBOOSTER,

    AT_CMD_NVLOAD,

    AT_CMD_MTREATTACH,

    AT_CMD_CSDF,

    AT_CMD_CGEREP,
    AT_CMD_CIND,

    AT_CMD_SMSDOMAIN,

    AT_CMD_CGPIAF,
    AT_CMD_WS46,

#if (FEATURE_DSDS == FEATURE_ON)
    AT_CMD_DSDSSTATE,
#endif

    AT_CMD_SAMPLE,

    AT_CMD_GPSLOCSET,

    AT_CMD_USBTETHERINFO,

    AT_CMD_GAMEMODE,

    AT_CMD_VICECFG,
    AT_CMD_RTTCFG,
    AT_CMD_RTTMODIFY,

    AT_CMD_PSEUDBTS,

    AT_CMD_SUBCLFSPARAM,

    AT_CMD_NVREFRESH,
    AT_CMD_TIMEQRY,

    AT_CMD_CVHU,

    AT_CMD_SIMSQ,

    AT_CMD_JDCFG,
    AT_CMD_JDSWITCH,

    AT_CMD_EMRSSICFG,
    AT_CMD_EMRSSIRPT,
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CLDBDOMAINSTATUS,
#endif

#ifdef MBB_SLT
    AT_CMD_BSNEX,
    AT_CMD_USBTEST,
#endif
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_SLTTEST,
#endif

    AT_CMD_PLMNSRCH,

    AT_CMD_PSSCENE,

    AT_CMD_SMSANTIATTACK,
    AT_CMD_SMSNASCAP,
    AT_CMD_SMSANTIATTACKCAP,

    AT_CMD_FORCESYNC,

    AT_CMD_LTEATTACHINFO,
    AT_CMD_SIPPORT,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_C5GQOS,
    AT_CMD_C5GQOSRDP,
    AT_CMD_C5GPNSSAI,
    AT_CMD_C5GNSSAI,
    AT_CMD_C5GNSSAA,
    AT_CMD_C5GNSSAIRDP,
    AT_CMD_CSUEPOLICY,
    AT_CMD_CLADN,
    AT_CMD_IMSURSP,
#endif
    AT_CMD_LTEPWRCTRL,
    AT_CMD_GNSSNTY,

    AT_CMD_LTESARSTUB,

    AT_CMD_LOWPOWERMODE,
    AT_CMD_CCPUNIDDISABLE,
    AT_CMD_ACPUNIDDISABLE,

    AT_CMD_MIPIOPERATE,
    AT_CMD_FAGCGAIN,
    AT_CMD_FRBINFO,
    AT_CMD_DETECTPLMN,
    AT_CMD_VOIPAPNKEY,

    AT_CMD_RRCSTAT,
    AT_CMD_MCS,
    AT_CMD_TXPOWER,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_NTXPOWER,
#endif
    AT_CMD_HFREQINFO,

    AT_CMD_ROAMPDPTYPE,

    AT_CMD_WAKELOCK,

    AT_CMD_UARTTEST,
    AT_CMD_I2STEST,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_CPOLICYRPT,
    AT_CMD_CPOLICYCODE,
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_MONSSC,
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
    AT_CMD_NOCARD,
#endif
#endif
#if (FEATURE_LTEV == FEATURE_ON)
    AT_CMD_CATM,
    AT_CMD_CCUTLE,
    AT_CMD_CUSPCREQ,
    AT_CMD_CUTCR,
    AT_CMD_CCBRREQ,
    AT_CMD_CBR,
    AT_CMD_CV2XDTS,
    AT_CMD_VSYNCSRC,
    AT_CMD_VSYNCSRCRPT,
    AT_CMD_VSYNCMODE,

    AT_CMD_SLINFO,
    AT_CMD_PHYR,
    AT_CMD_PTRRPT,
    AT_CMD_RPPCFG,
    AT_CMD_GNSSTEST,
    AT_CMD_QRYDATA,
    AT_CMD_GNSSINFO,
    AT_CMD_VPHYSTAT,
    AT_CMD_VPHYSTATCLR,
    AT_CMD_VSNRRSRP,
    AT_CMD_V2XRSSI,
    AT_CMD_VRSSI,
    AT_CMD_SENDDATA,
    AT_CMD_FTYRESET,
    AT_CMD_PC5SYNC,
    AT_CMD_VTXPOWER,
    AT_CMD_CV2XL2ID,
#if (FEATURE_LTEV_WL == FEATURE_ON)
    AT_CMD_PC5SYNC,
    AT_CMD_MCGCFG,
    AT_CMD_FILEWR,
    AT_CMD_FILERD,
    AT_CMD_LEDTEST,
    AT_CMD_GPIO_TEST,
    AT_CMD_ANTTEST,
    AT_CMD_GE_TEST,
    AT_CMD_SFP_TEST,
    AT_CMD_SFP_SWITCH,
    AT_CMD_GPIO_HEAT_SET,
    AT_CMD_HKADCTEST,
    AT_CMD_QRYGPIO,
    AT_CMD_SETGPIO,
    AT_CMD_SETBOOTMODE,
    AT_CMD_MULTIUPG_MODE,
    AT_CMD_QRYTEMPPRT,
    AT_CMD_RSUSYNCST,
    AT_CMD_GNSSSYNCST,
#endif
#endif
    AT_CMD_RMNETCFG,

    AT_CMD_OOSSRCHSTGY,

    AT_CMD_HIFIRESET,

    AT_CMD_SINGLEPDNSWITCH,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_NRPHYCOMCFG,
#endif
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    AT_CMD_ATNLPROXY_STUB,
#endif
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    AT_CMD_CMUX,
#endif
#if (FEATURE_AT_PROXY == FEATURE_ON)
    AT_CMD_ATP,
#endif
    AT_CMD_EMC_STATUS,
#if (FEATURE_MBB_CUST == FEATURE_ON)
    AT_CMD_HSMF,
    AT_CMD_LTEPROFILE,
    AT_CMD_LTEAPNATTACH,
#endif
    AT_CMD_TRUSTCNUM,
    AT_CMD_TNUMCTL,
    AT_CMD_TRUSTNUM,

    AT_CMD_COMM_BUTT,

} AT_CmdIndex;

typedef enum {
    AT_CMD_CURRENT_OPT_BUTT = 0,
    AT_CMD_D_GET_NUMBER_BEFORE_CALL,

    AT_CMD_D_CS_VOICE_CALL_SET,

    AT_CMD_D_CS_DATA_CALL_SET,

    AT_CMD_D_PPP_CALL_SET,
    AT_CMD_PPP_ORG_SET,

    AT_CMD_PS_DATA_CALL_END_SET,

    AT_CMD_WAIT_PPP_PROTOCOL_REL_SET,

    AT_CMD_D_IP_CALL_SET,
    AT_CMD_CGDATA_SET,

    AT_CMD_CGACT_ORG_SET,
    AT_CMD_CGACT_END_SET,
    AT_CMD_CGANS_ANS_SET,
    AT_CMD_CGANS_ANS_EXT_SET,

    AT_CMD_CMMI_SET,
    AT_CMD_CMMI_QUERY_CLIP,
    AT_CMD_CMMI_QUERY_CLIR,

    AT_CMD_H_SET,
    AT_CMD_END_SET,
    AT_CMD_CALL_END_SET,
    AT_CMD_H_PS_SET,

    AT_CMD_A_SET,

    AT_CMD_S0_SET,
    AT_CMD_S0_READ,

    AT_CMD_CGMI_READ,

    AT_CMD_CGMM_READ,

    AT_CMD_CGMR_READ,

    AT_CMD_CGSN_READ,

    AT_CMD_CIMI_READ,
    AT_CMD_ICCID_READ,
    AT_CMD_CSTA_SET,
    AT_CMD_CSTA_READ,

    AT_CMD_CHUP_SET,

    AT_CMD_DTMF_SET,
    AT_CMD_VTS_SET,

    AT_CMD_CLIP_READ,

    AT_CMD_COLP_READ,
    AT_CMD_CLIR_SET,
    AT_CMD_CLIR_READ,
    AT_CMD_CLCC_SET,
    /* AT+CAPS����ʵ������ò���״̬ */
    AT_CMD_CPAS_SET,

    AT_CMD_COPS_SET_AUTOMATIC,
    AT_CMD_COPS_SET_MANUAL,
    AT_CMD_COPS_SET_DEREGISTER,
    AT_CMD_COPS_SET_MANUAL_AUTOMATIC_MANUAL,
    AT_CMD_COPS_READ,
    AT_CMD_COPS_TEST,
    AT_CMD_COPS_ABORT_PLMN_LIST,

    AT_CMD_CPOL_SET,
    AT_CMD_CPOL_READ,
    AT_CMD_CPOL_TEST,

    AT_CMD_CFUN_SET,
    AT_CMD_CFUN_READ,

    AT_CMD_CBND_SET,
    AT_CMD_CBND_READ,

    AT_CMD_CSQ_SET,
    AT_CMD_CBC_SET,

    AT_CMD_CPBF_SET,
    AT_CMD_CPBR_SET,
    AT_CMD_CPBR2_SET,
    AT_CMD_SCPBR_SET,
    AT_CMD_CPBW_SET,
    AT_CMD_CNUM_READ,

    AT_CMD_CSIM_SET,
    AT_CMD_CCHO_SET,
    AT_CMD_CCHP_SET,
    AT_CMD_CCHC_SET,
    AT_CMD_CGLA_SET,
    AT_CMD_CRSM_SET,
    AT_CMD_CPIN_VERIFY_SET,
    AT_CMD_CPIN_UNBLOCK_SET,
    AT_CMD_CPIN_READ,

    AT_CMD_CPWD_SET,
    AT_CMD_CPWD_CHANGE_PIN,

    AT_CMD_CLCK_PIN_HANDLE,
    AT_CMD_CARD_LOCK_SET,
    AT_CMD_CARD_LOCK_READ,
    AT_CMD_CARD_ATR_READ,
    AT_CMD_CLCK_UNLOCK,
    AT_CMD_CLCK_LOCK,
    AT_CMD_CLCK_QUERY,
    AT_CMD_CLCK_SIMLOCKUNLOCK,
    AT_CMD_CLCK_SIMLOCKDATAREAD,
    AT_CMD_CTFR_SET,

    AT_CMD_CGEQREQ_SET,
    AT_CMD_CGEQREQ_READ,

    AT_CMD_CGEQMIN_SET,
    AT_CMD_CGEQMIN_READ,

    AT_CMD_CGPADDR_SET,
    AT_CMD_CGEQNEG_SET,
    AT_CMD_CGEQNEG_TEST,

    AT_CMD_CGACT_READ,

    AT_CMD_CGCLASS_SET,
    AT_CMD_CGCLASS_READ,

    AT_CMD_CGATT_ATTACH_SET,
    AT_CMD_CGATT_DETAACH_SET,

    AT_CMD_CGCATT_PS_ATTACH_SET,
    AT_CMD_CGCATT_CS_ATTACH_SET,
    AT_CMD_CGCATT_PS_CS_ATTACH_SET,
    AT_CMD_CGCATT_PS_DETAACH_SET,
    AT_CMD_CGCATT_CS_DETAACH_SET,
    AT_CMD_CGCATT_PS_CS_DETAACH_SET,

    AT_CMD_CGDCONT_SET,
    AT_CMD_CGDCONT_READ,

    AT_CMD_CGREG_READ,

    AT_CMD_CEREG_READ,

    AT_CMD_C5GREG_READ,

    AT_CMD_CGDSCONT_SET,
    AT_CMD_CGDSCONT_READ,

    AT_CMD_CGTFT_SET,
    AT_CMD_CGTFT_READ,

    AT_CMD_CGDNS_SET,
    AT_CMD_CGDNS_READ,

    AT_CMD_CGCMOD_SET,

    AT_CMD_CGAUTO_SET,
    AT_CMD_CGAUTO_READ,

    AT_CMD_AUTHDATA_SET,
    AT_CMD_AUTHDATA_READ,

    /* �첽��Ϣʵ�����ӵĲ������ */
    AT_CMD_D_GPRS_SET,
    AT_CMD_DSFLOWCLR_SET,
    AT_CMD_DSFLOWQRY_SET,
    AT_CMD_DSFLOWRPT_SET,
    AT_CMD_DWINS_SET,
    AT_CMD_PDPSTUB_SET,
    AT_CMD_CGCONTRDP_SET,
    AT_CMD_CGSCONTRDP_SET,
    AT_CMD_CGTFTRDP_SET,

    AT_CMD_CGEQOS_SET,
    AT_CMD_CGEQOS_READ,
    AT_CMD_CGEQOSRDP_SET,

    AT_CMD_LTECS_READ,
    AT_CMD_CEMODE_SET,
    AT_CMD_CEMODE_READ,
    AT_CMD_CREG_READ,

    AT_CMD_CCUG_SET,
    AT_CMD_CCUG_READ,

    AT_CMD_CCWA_DISABLE,
    AT_CMD_CCWA_ENABLE,
    AT_CMD_CCWA_QUERY,

    AT_CMD_CNMR_QUERY,
    AT_CMD_CECELLID_QUERY,

    AT_CMD_CCFC_DISABLE,
    AT_CMD_CCFC_ENABLE,
    AT_CMD_CCFC_QUERY,
    AT_CMD_CCFC_REGISTRATION,
    AT_CMD_CCFC_ERASURE,

    AT_CMD_CUSD_REQ,

    AT_CMD_CHLD_SET,

    AT_CMD_CMOD_SET,
    AT_CMD_CMOD_READ,

    AT_CMD_CBST_SET,
    AT_CMD_CBST_READ,

    AT_CMD_CSMS_SET,
    AT_CMD_CSMS_READ,

    AT_CMD_CMSQ_SET,

    AT_CMD_CMGF_SET,
    AT_CMD_CMGF_READ,

    AT_CMD_CSCA_SET,
    AT_CMD_CSCA_READ,

    AT_CMD_CSMP_SET,
    AT_CMD_CSMP_READ,

    AT_CMD_CMGL_SET,

    AT_CMD_CMGR_SET,

    AT_CMD_CMGS_TEXT_SET,
    AT_CMD_CMGS_PDU_SET,

    AT_CMD_CNMA_TEXT_SET,
    AT_CMD_CNMA_PDU_SET,
    AT_CMD_CMSS_SET,
    AT_CMD_CMST_SET,

    AT_CMD_CMGW_TEXT_SET,
    AT_CMD_CMGW_PDU_SET,

    AT_CMD_CMGD_SET,
    AT_CMD_CMGD_TEST,

    AT_CMD_CMGI_SET,

    AT_CMD_CMGC_TEXT_SET,
    AT_CMD_CMGC_PDU_SET,

    AT_CMD_CGSMS_SET,
    AT_CMD_CGSMS_READ,

    AT_CMD_CMMT_SET,

    AT_CMD_CSCB_SET,
    AT_CMD_CSCB_READ,

    AT_CMD_CBMGR_SET,

    AT_CMD_CBMGL_SET,

    AT_CMD_CBMGD_SET,

    AT_CMD_CCNMA_SET,
    AT_CMD_CCNMA_READ,

    AT_CMD_CPBS_SET,
    AT_CMD_CPBS_READ,

    AT_CMD_CPAM_SET,
    AT_CMD_CPAM_READ,

    AT_CMD_CPIN2_VERIFY_SET,
    AT_CMD_CPIN2_UNBLOCK_SET,
    AT_CMD_CPIN2_READ,

    AT_CMD_CNMI_SET,

    AT_CMD_CSSM_SET,
    AT_CMD_CSSM_READ,

    AT_CMD_CPMS_SET,
    AT_CMD_CPMS_READ,

    AT_CMD_CMMS_SET,
    AT_CMD_CMMS_READ,

    AT_CMD_CMSTUB_SET,        /* ��ʾ����^CMSTUB���ò��� */
    AT_CMD_DLOADINFO_READ,    /* ��ʾ����^DLOADINFO��ѯ���� */
    AT_CMD_AUTHORITYVER_READ, /* ��ʾ����^AUTHORITYVER��ѯ���� */
    AT_CMD_AUTHORITYID_READ,  /* ��ʾ����^AUTHORITYID��ѯ���� */
    AT_CMD_GODLOAD_SET,       /* ��ʾ����^GODLOAD���ò��� */
    AT_CMD_SDLOAD_SET,        /* ��ʾ����^SDLOAD���ò��� */

    AT_CMD_HWNATQRY_READ,  /* ��ʾ����^HWNATQRY��ѯ���� */
    AT_CMD_FLASHINFO_READ, /* ��ʾ����^FLASHINFO��ѯ���� */
    AT_CMD_AUTHVER_READ,   /* ��ʾ����^AUTHVER��ѯ���� */

    AT_CMD_CDUR_READ,          /* ��ʾ����^CDUR��ѯ���� */
    AT_CMD_PFVER_READ,         /* ��ʾ����^PFVER��ѯ���� */
    AT_CMD_DLOADVER_READ,      /* ��ʾ����^DLOADVER��ѯ���� */
    AT_CMD_CPULOAD_READ,       /* ��ʾ����^CPULOAD��ѯ���� */
    AT_CMD_MFREELOCKSIZE_READ, /* ��ʾ����^MFREELOCKSIZE��ѯ���� */

    AT_CMD_MEMINFO_READ,

    AT_CMD_STPD_SET,
    AT_CMD_CLVL_SET,
    AT_CMD_VMSET_SET,
    AT_CMD_F_SET,
    AT_CMD_VMSET_READ,  /* AT^VMSET����ʵ���ѯ״̬ */
    AT_CMD_DDSETEX_SET, /* AT^DDSETEX����ʵ������״̬ */
    AT_CMD_NVRESTORE_READ,
    AT_CMD_NVRSTSTTS_READ,
    AT_CMD_STMN_SET,
    AT_CMD_STTR_SET,
    AT_CMD_STEN_SET,
#if (FEATURE_LTEV == FEATURE_ON)
    AT_CMD_VMODE_SET,
    AT_CMD_VMODE_QUERY,
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_SRCID_SET,
    AT_CMD_SRCID_QRY,
#endif
#endif

#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
    AT_CMD_STGI_SET,
    AT_CMD_STGR_SET,
#endif
    AT_CMD_IMSICHG_SET,
    AT_CMD_IMSICHG_READ,
    AT_CMD_INFORBU_SET,
    AT_CMD_INFORRS_SET,
    AT_CMD_CPNN_TEST,
    AT_CMD_CSEN_SET,
    AT_CMD_CSTR_SET,
    AT_CMD_CFRQ_SET,
    AT_CMD_CFRQ_READ,
    AT_CMD_CIMEI_SET,
    AT_CMD_CQST_SET,
    AT_CMD_CDEF_SET,
    AT_CMD_CQST_READ,
    AT_CMD_CAATT_SET,
    AT_CMD_CAATT_READ,
    AT_CMD_SYSINFO_READ,
    AT_CMD_SYSINFOEX_READ,
    AT_CMD_CFPLMN_SET,
    AT_CMD_CFPLMN_READ,
    AT_CMD_SYSCFG_SET,
    AT_CMD_SYSCFG_READ,
    AT_CMD_SYSCFG_TEST,
    AT_CMD_SYSCFGEX_READ,
    AT_CMD_SYSCFGEX_TEST,
    AT_CMD_PNN_READ,
    AT_CMD_CPNN_READ,
    AT_CMD_OPL_READ,
    AT_CMD_HS_READ,
    AT_CMD_SS_REGISTER,
    AT_CMD_SS_ERASE,
    AT_CMD_SS_ACTIVATE,
    AT_CMD_SS_DEACTIVATE,
    AT_CMD_SS_INTERROGATE,
    AT_CMD_SS_REGISTER_PSWD,
    /* Delete AT_CMD_SS_GET_PSWD */
    AT_CMD_SS_USSD,
    AT_CMD_SET_TMODE,
    AT_CMD_SET_FTXON,
    AT_CMD_SET_FRXON,
    AT_CMD_SET_FWAVE, /* ��ʾ����^FWAVE���ò��� */
    AT_CMD_QUERY_RSSI,
    AT_CMD_SET_RXDIV,
    AT_CMD_SET_RXPRI,
    AT_CMD_SS_DEACTIVE_CCBS,
    AT_CMD_SS_INTERROGATE_CCBS,
    AT_CMD_CSQLVLEXT_SET,
    AT_CMD_CSQLVL_SET,    /* ��ʾ����^CSQLVL���ò��� */
    AT_CMD_APPDMVER_READ, /* ��ʾ����^APPDMVER��ѯ���� */
    AT_CMD_APBATLVL_READ, /* ��ʾ����^APBATLVL��ѯ���� */
    AT_CMD_MSID_READ,
    AT_CMD_CWAS_QUERY,
    AT_CMD_CGAS_QUERY,
    AT_CMD_CELLINFO_QUERY,
    AT_CMD_MEANRPT_QUERY,
    AT_CMD_REGISTER_TIME_READ,
    AT_CMD_ANQUERY_READ,
    AT_CMD_HOMEPLMN_READ,
    AT_CMD_VERSIONTIME_READ,
    AT_CMD_FREQLOCK_SET,
    AT_CMD_FREQLOCK_QUERY,
    AT_CMD_CSNR_QUERY,
    AT_CMD_RRC_VERSION_SET,
    AT_CMD_RRC_VERSION_QUERY,
    AT_CMD_YJCX_SET,
    AT_CMD_YJCX_QUERY,
    AT_CMD_CRPN_QUERY,
    AT_CMD_SPN_QUERY,
    AT_CMD_CC_STATE_QUERY,
    AT_CMD_MM_TEST_CMD_SET,

    AT_CMD_RSIM_READ,
    AT_CMD_GPIOPL_SET,
    AT_CMD_GPIOPL_QRY,
    AT_CMD_DATALOCK_SET,
    AT_CMD_TBATVOLT_QRY,
    AT_CMD_VERSION_QRY,
    AT_CMD_SECUBOOT_QRY,
    AT_CMD_SECUBOOT_SET,
    AT_CMD_SECUBOOTFEATURE_QRY,
    AT_CMD_PRODTYPE_QRY,
    AT_CMD_SFEATURE_QRY,
    AT_CMD_LOWPWRMODE_SET,
    AT_CMD_FCHAN_SET,
    AT_CMD_RXPRI_QRY,
    AT_CMD_DCXOTEMPCOMP_SET,
    AT_CMD_TSELRF_SET,
    AT_CMD_TBAT_SET,
    AT_CMD_TBAT_QRY,
    AT_CMD_STANDBY_SET,
    AT_CMD_DRV_AGENT_HARDWARE_QRY,
    AT_CMD_DRV_AGENT_FULL_HARDWARE_QRY,
    AT_CMD_WAS_MNTN_SET_CELLSRH,
    AT_CMD_WAS_MNTN_QRY_CELLSRH,
    AT_CMD_DRV_AGENT_RXDIV_SET_REQ,
    AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ,
    AT_CMD_MMA_GET_CURRENT_ATTACH_STATUS,
    AT_CMD_APP_QRY_ALS_REQ,
    AT_CMD_MMA_MT_POWER_DOWN,
    AT_CMD_MMA_SET_PREF_PLMN,
    AT_CMD_MMA_SET_PREF_PLMN_TYPE,
    AT_CMD_APP_SET_UUSINFO_REQ,
    AT_CMD_APP_SET_ALS_REQ,
    AT_CMD_MSG_MMA_SET_PIN,
    AT_CMD_CSSN_SET,

    AT_CMD_BOOTROMVER_READ,

    AT_CMD_NVBACKUP_SET,
    AT_CMD_NVRESTORE_SET,
    AT_CMD_RESET_SET,
    AT_CMD_NVRSTSTTS_SET,
    AT_CMD_TBAT_READ,
    AT_CMD_SECUBOOTFEATURE_READ,

    AT_CMD_CURC_READ,
    AT_CMD_UNSOLICITED_RPT_SET,
    AT_CMD_UNSOLICITED_RPT_READ,
    AT_CMD_NCELL_MONITOR_SET,
    AT_CMD_NCELL_MONITOR_READ,
    AT_CMD_USER_SRV_STATE_READ,

    AT_CMD_TMODE_SET,
    AT_CMD_TMODE_READ,
    AT_CMD_TMODE_TEST,


    /*  AT_CMD_FCHAN_SET, */
    AT_CMD_FCHAN_READ,

    AT_CMD_FANT_SET,
    AT_CMD_FANT_READ,

    AT_CMD_FPA_SET,
    AT_CMD_FDAC_SET,

    AT_CMD_FLNA_SET,
    AT_CMD_FLNA_READ,

    AT_CMD_FTXON_SET,
    AT_CMD_FTXON_READ,

    AT_CMD_FRXON_SET,
    AT_CMD_FRXON_READ,

    AT_CMD_FRSSI_READ,

    AT_CMD_CIPERQRY_READ,
    AT_CMD_LOCINFO_READ,
    AT_CMD_ACINFO_READ,

    AT_CMD_CERSSI_READ,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_CSERSSI_READ,
#endif
    AT_CMD_CESQ_SET,

    // �������۲�end
    AT_CMD_ADC_SET,
    AT_CMD_FASTDORM_SET,
    AT_CMD_FASTDORM_READ,
    AT_CMD_APDIALMODE_READ, /* ��ʾ����^APDIALMODE��ѯ���� */

    AT_CMD_CCPUMEMINFO_QRY,

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
    AT_CMD_SPWORD_SET,
#endif
    AT_CMD_SIMLOCKSTATUS_READ,
    AT_CMD_CSDFLT_READ,
    AT_CMD_CGPADDR_TEST,

    AT_CMD_NVBACKUPSTAT_READ,
    AT_CMD_NANDBBC_READ,
    AT_CMD_NANDVER_READ,
    AT_CMD_CHIPTEMP_READ,

    AT_CMD_CSASM_SET,
    AT_CMD_DNSQUERY_SET,
    AT_CMD_CELLROAM_READ,

    AT_CMD_TCHRENABLE_SET,
    AT_CMD_TCHRENABLE_TEST,
    AT_CMD_RSFR_VERSION_QRY,

    AT_CMD_MAXLCKTMS_SET, /* ��ʾ����^MAXLCKTMS���ò��� */

    AT_CMD_HUK_SET,
    AT_CMD_FACAUTHPUBKEY_SET,
    AT_CMD_IDENTIFYSTART_SET,
    AT_CMD_IDENTIFYEND_SET,
    AT_CMD_SIMLOCKDATAWRITE_SET,
    AT_CMD_PHONESIMLOCKINFO_READ,
    AT_CMD_SIMLOCKDATAREAD_READ,
    AT_CMD_GETMODEMSCID_READ,

    AT_CMD_PHONEPHYNUM_SET,
    AT_CMD_OPWORD_SET,

    AT_CMD_FACAUTHPUBKEYEX_SET,
    AT_CMD_SIMLOCKDATAWRITEEX_SET,
    AT_CMD_SIMLOCKDATAREADEX_READ_SET,
    AT_CMD_CLVL_READ,

    AT_CMD_APSIMST_SET,

    AT_CMD_CMOLR_SET,
    AT_CMD_CMOLR_READ,
    AT_CMD_CMTLR_SET,
    AT_CMD_CMTLR_READ,
    AT_CMD_CMTLRA_SET,
    AT_CMD_CMTLRA_READ,

    AT_CMD_CPOS_SET,
    AT_CMD_CGPSCLOCK_SET,

    AT_CMD_CERSSI_SET,
    AT_CMD_CNMR_READ,
    AT_CMD_CELL_ID_READ,
    AT_CMD_CPSERR_SET,

    AT_CMD_CISA_SET,

    AT_CMD_CMUT_SET,
    AT_CMD_CMUT_READ,

    AT_CMD_SIMLOCKUNLOCK_SET,

    AT_CMD_CLPR_SET,
    AT_CMD_FWAVE_SET,
    AT_CMD_SWVER_SET,

    AT_CMD_CBG_SET,
    AT_CMD_CBG_READ,

    AT_CMD_MMPLMNINFO_QRY,

    AT_CMD_EONSUCS2_QRY,

    AT_CMD_PLMN_QRY,
    AT_CMD_XLEMA_QRY,

    AT_CMD_BODYSARON_SET,

    AT_CMD_IMEI_VERIFY_READ,

    AT_CMD_COPN_QRY,
    AT_CMD_STRXBER_SET,

    AT_CMD_SIMINSERT_SET,

    AT_CMD_RELEASE_RRC_SET,

    AT_CMD_REFCLKFREQ_SET,
    AT_CMD_REFCLKFREQ_READ,
    AT_CMD_RFICSSIRD_SET,

    AT_CMD_HANDLEDECT_SET,
    AT_CMD_HANDLEDECT_QRY,

    AT_CMD_HVSDH_SET,
    AT_CMD_HVSDH_READ,
    AT_CMD_HVSCONT_READ,
    AT_CMD_HVSST_SET,
    AT_CMD_HVSST_QRY,
    AT_CMD_HVPDH_SET,
    AT_CMD_SCICHG_SET,
    AT_CMD_SCICHG_QRY,
    AT_CMD_BWT_SET,
    AT_CMD_PASSTHROUGH_SET,
    AT_CMD_PASSTHROUGH_QRY,
#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
    AT_CMD_SINGLEMODEMDUALSLOT_SET,
    AT_CMD_SINGLEMODEMDUALSLOT_QRY,
#endif
#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
    AT_CMD_SILENTPIN_SET,
    AT_CMD_SILENTPININFO_SET,
#endif
    AT_CMD_ESIMCLEAN_SET,
    AT_CMD_ESIMCHECK_QRY,
    AT_CMD_ESIMEID_QRY,
    AT_CMD_ESIMPKID_QRY,

    AT_CMD_ESIMSWITCH_SET,
    AT_CMD_ESIMSWITCH_QRY,

    AT_CMD_PRIVATECCHO_SET,
    AT_CMD_PRIVATECCHP_SET,

    AT_CMD_PRIVATECGLA_REQ,

    AT_CMD_EOPLMN_SET,
    AT_CMD_EOPLMN_QRY,

    AT_CMD_NVMANUFACTUREEXT_SET,
    AT_CMD_NETSCAN_SET,
    AT_CMD_NETSCAN_ABORT,

    AT_CMD_FPLLSTATUS_QRY,
    AT_CMD_FPLLSTATUS_SET,

    AT_CMD_ECID_SET,

    AT_CMD_CLCC_QRY,
#if (FEATURE_IMS == FEATURE_ON)
    AT_CMD_CIREG_SET,
    AT_CMD_CIREG_QRY,
    AT_CMD_CIREP_SET,
    AT_CMD_CIREP_QRY,
    AT_CMD_IMSREGDOMAIN_QRY,
    AT_CMD_IMSDOMAINCFG_SET,
    AT_CMD_IMSDOMAINCFG_QRY,
    AT_CMD_IMPU_SET,

    AT_CMD_VOLTEIMPI_SET,
    AT_CMD_VOLTEDOMAIN_SET,

    AT_CMD_CCWAI_SET,
    AT_CMD_UICCAUTH_SET,
    AT_CMD_CURSM_SET,
    AT_CMD_KSNAFAUTH_SET,
    AT_CMD_CALLENCRYPT_SET,

    AT_CMD_PCSCF,
    AT_CMD_DMDYN,
    AT_CMD_DMTIMER,
    AT_CMD_IMSPSI,
    AT_CMD_DMUSER,
    AT_CMD_IMSVTCAPCFG_SET,
    AT_CMD_IMSSMSCFG_QRY,
    AT_CMD_IMSSMSCFG_SET,
#endif

    AT_CMD_CACMIMS_SET,

    AT_CMD_MODEMSTATUS,
    AT_CMD_RATCOMBINEPRIO,

    AT_CMD_MIPI_CLK_QRY,

    AT_CMD_DPDTTEST_SET,
    AT_CMD_DPDT_SET,
    AT_CMD_DPDTQRY_SET,

    AT_CMD_TRX_TAS_SET,
    AT_CMD_TRX_TAS_QRY,

    AT_CMD_TFDPDT_SET,
    AT_CMD_TFDPDTQRY_SET,

#if (FEATURE_DSDS == FEATURE_ON)
    AT_CMD_PSPROTECTMODE_SET,
#endif
    AT_CMD_PHYINIT_SET,
    AT_CMD_CECALL_SET,
    AT_CMD_CECALL_QRY,
    AT_CMD_ECLSTART_SET,
    AT_CMD_ECLSTOP_SET,
    AT_CMD_ECLCFG_SET,
    AT_CMD_ECLCFG_QRY,
    AT_CMD_ECLMSD_SET,
    AT_CMD_ECLMSD_QRY,
    AT_CMD_ECLIMSCFG_SET,
    AT_CMD_ECLIMSCFG_QRY,
    AT_CMD_ABORT_SET,
    AT_CMD_ECLPUSH_SET,
    AT_CMD_ECLLIST_QRY,
    AT_CMD_FPOWDET_QRY,

    AT_CMD_RATFREQLOCK_SET,

#if (FEATURE_IMS == FEATURE_ON)
    AT_CMD_CALL_MODIFY_INIT_SET,
    AT_CMD_CALL_MODIFY_ANS_SET,
    AT_CMD_ECONF_DIAL_SET,
    AT_CMD_CLCCECONF_QRY,
    AT_CMD_IMS_SWITCH_SET,
    AT_CMD_IMS_SWITCH_QRY,
    AT_CMD_VOICE_DOMAIN_SET,
    AT_CMD_VOICE_DOMAIN_QRY,
#endif
    AT_CMD_CHLD_EX_SET,
    AT_CMD_APDS_SET,

    AT_CMD_GSM_FREQLOCK_SET,

    AT_CMD_GSM_FREQLOCK_QRY,
    AT_CMD_DEL_CELLENTITY_SET,

    AT_CMD_CARDTYPE_QUERY,

    AT_CMD_CARDTYPEEX_QUERY,

    AT_CMD_CARDVOLTAGE_QUERY,

    AT_CMD_CRM_SET,

    AT_CMD_CQOSPRI_SET,

/* ���Ƶ� FEATURE_UE_MODE_CDMA ���� */
#if (FEATURE_LTE == FEATURE_ON)
    AT_CMD_FRSTATUS_SET,
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CFSH_SET,

    AT_CMD_CBURSTDTMF_SET,
    AT_CMD_CFREQLOCK_SET,
    AT_CMD_CFREQLOCK_QRY,

    AT_CMD_CCMGS_SET,
    AT_CMD_CCMGW_SET,
    AT_CMD_CCMGD_SET,

    AT_CMD_CCSASM_SET,

    AT_CMD_CDMACSQ_SET,
    AT_CMD_CDMACSQ_QRY,

    AT_CMD_HDR_CSQ_SET,
    AT_CMD_HDR_CSQ_QRY,
#endif

    AT_CMD_APDSFLOWRPTCFG_SET,
    AT_CMD_APDSFLOWRPTCFG_QRY,

    AT_CMD_DSFLOWNVWRCFG_SET,
    AT_CMD_DSFLOWNVWRCFG_QRY,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CTA_SET,
    AT_CMD_CTA_QRY,
#endif

#if (FEATURE_HUAWEI_VP == FEATURE_ON)
    AT_CMD_VOICEPREFER_SET,
    AT_CMD_VOICEPREFER_QRY,
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CUSTOMDIAL_SET,
#endif

    AT_CMD_TTYMODE_SET,
    AT_CMD_TTYMODE_READ,
    AT_CMD_FEMCTRL_SET,
    AT_CMD_NVWRSECCTRL_SET,

    AT_CMD_CRLA_SET,
    AT_CMD_CARDSESSION_QRY,

    AT_CMD_CCIMI_SET,

    AT_CMD_QCCB_SET,

    AT_CMD_EMCCBM_QRY,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CLOCINFO_QRY,

    AT_CMD_CSID_QRY,

    AT_CMD_CSIDLIST_SET,
    AT_CMD_CSID_SET,

    AT_CMD_CSNID_QRY,
    AT_CMD_ECCALL_SET,
    AT_CMD_ECCTRL_SET,
    AT_CMD_ECCAP_SET,
    AT_CMD_ECCAP_QRY,
    AT_CMD_ECCTEST_SET,
    AT_CMD_ECCTEST_QRY,
    AT_CMD_ECKMC_SET,
    AT_CMD_ECKMC_QRY,
    AT_CMD_ECRANDOM_QRY,
    AT_CMD_CLOCINFO_SET,
    AT_CMD_CPMP_SET,
    AT_CMD_CPMP_QRY,
    AT_CMD_CTROAMINFO_QRY,
    AT_CMD_CTOOSCOUNT_SET,
    AT_CMD_CTROAMINFO_SET,
    AT_CMD_PRLID_QRY,
    AT_CMD_RATCOMBINEDMODE_QRY,

    AT_CMD_NOCARDMODE_SET,
    AT_CMD_NOCARDMODE_QRY,
#endif
    AT_CMD_AFCCLKINFO_QRY,

    AT_CMD_CCLPR_SET,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_1XCHAN_SET,
    AT_CMD_1XCHAN_QRY,
    AT_CMD_CVER_QRY,
#endif
    AT_CMD_CCMGG_QRY,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_GETSTA_QRY,
    AT_CMD_GETESN_QRY,
    AT_CMD_GETMEID_QRY,
    AT_CMD_CHIGHVER_QRY,
#endif
#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)
    AT_CMD_MBMS_SERVICE_OPTION_SET,
    AT_CMD_MBMS_SERVICE_STATE_SET,
    AT_CMD_MBMS_PREFERENCE_SET,
    AT_CMD_MBMS_SIB16_NETWORK_TIME_QRY,
    AT_CMD_MBMS_BSSI_SIGNAL_LEVEL_QRY,
    AT_CMD_MBMS_NETWORK_INFO_QRY,
    AT_CMD_EMBMS_STATUS_QRY,
    AT_CMD_MBMS_UNSOLICITED_CFG_SET,
    AT_CMD_MBMS_INTERESTLIST_SET,
    AT_CMD_MBMS_AVL_SERVICE_LIST_QRY,
#endif
    AT_CMD_LTE_LOW_POWER_CONSUMPTION_SET,
    AT_CMD_LTE_WIFI_COEX_SET,
    AT_CMD_LTE_WIFI_COEX_QRY,
#endif

    AT_CMD_CAGPSCFGPOSMODE_SET,
    AT_CMD_CAGPSSTART_SET,
    AT_CMD_CAGPSSTOP_SET,
    AT_CMD_CAGPSCFGMPCADDR_SET,
    AT_CMD_CAGPSCFGPDEADDR_SET,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_MEID_SET,
    AT_CMD_MEID_QRY,
#endif

    AT_CMD_IMSPDPCFG_SET,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CCONTDTMF_SET,
#endif

    AT_CMD_TRANSMODE_READ,

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_DORMTIMER_SET,
    AT_CMD_DORMTIMER_QRY,
#endif
    AT_CMD_EHPLMN_LIST_QRY,
    AT_CMD_DPLMNLIST_SET,
    AT_CMD_DPLMNLIST_QRY,

    AT_CMD_MCCFREQ_SET,
    AT_CMD_MCCFREQ_QRY,
    AT_CMD_CLOUDDATA_SET,
    AT_CMD_BORDERINFO_SET,
    AT_CMD_BORDERINFO_QRY,

    AT_CMD_EXCHANGE_MODEM_INFO_SET,

    AT_CMD_CSG_LIST_SEARCH,
    AT_CMD_ABORT_CSG_LIST_SEARCH,

    AT_CMD_UE_CENTER_SET,
    AT_CMD_UE_CENTER_QRY,

    AT_CMD_QUICK_CARD_SWITCH_SET,

    AT_CMD_MIPI_WR,
    AT_CMD_MIPI_RD,
    AT_CMD_SSI_WR,
    AT_CMD_SSI_RD,

    AT_CMD_MIPI_WREX,
    AT_CMD_MIPI_RDEX,

    AT_CMD_MONSC_SET,
    AT_CMD_MONNC_SET,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_MONSSC_SET,
    AT_CMD_NWDEPLOYMENT_SET,
#endif
    AT_CMD_PDM_CTRL,

    AT_CMD_CSG_SPEC_SEARCH,
    AT_CMD_CSG_ID_INFO_QRY,

    AT_CMD_EVDO_SYS_EVENT_SET,
    AT_CMD_EVDO_SIG_MASK_SET,

    AT_CMD_ROAM_IMS_SET,
    AT_CMD_ROAM_IMS_QRY,

    AT_CMD_XCPOSR_SET,
    AT_CMD_XCPOSR_QRY,
    AT_CMD_XCPOSRRPT_SET,
    AT_CMD_XCPOSRRPT_QRY,
    AT_CMD_CLEAR_HISTORY_FREQ,

    AT_CMD_SENSOR_SET,
    AT_CMD_SCREEN_SET,
    AT_CMD_UART_TEST_SET,
    AT_CMD_I2S_TEST_SET,

    AT_CMD_PCSCF_SET,
    AT_CMD_PCSCF_QRY,
    AT_CMD_DMDYN_SET,
    AT_CMD_DMDYN_QRY,

    AT_CMD_DMTIMER_SET,
    AT_CMD_DMTIMER_QRY,
    AT_CMD_IMSPSI_SET,
    AT_CMD_IMSPSI_QRY,
    AT_CMD_DMUSER_QRY,

    AT_CMD_EFLOCIINFO_SET,
    AT_CMD_EFPSLOCIINFO_SET,
    AT_CMD_EFLOCIINFO_QRY,
    AT_CMD_EFPSLOCIINFO_QRY,

    AT_CMD_NICKNAME_SET,
    AT_CMD_NICKNAME_QRY,
    AT_CMD_BATTERYINFO_SET,

    AT_CMD_FRATIGNITION_SET,
    AT_CMD_FRATIGNITION_QRY,

    AT_CMD_PACSP_QRY,

    AT_CMD_MODEM_TIME_SET,

    AT_CMD_RXTESTMODE_SET,

    AT_CMD_BESTFREQ_SET,
    AT_CMD_BESTFREQ_QRY,

    AT_CMD_ULFREQRPT_SET,
    AT_CMD_ULFREQRPT_QRY,

    AT_CMD_RFIC_DIE_ID_QRY,

    AT_CMD_RFIC_DIE_ID_EX_QRY,

    AT_CMD_RFFE_DIE_ID_QRY,

    AT_CMD_PHY_COM_CFG_SET,

    AT_CMD_PMU_DIE_SN_QRY,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_NRPHY_COM_CFG_SET,
#endif

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
    AT_CMD_M2M_FREQLOCK_SET,
    AT_CMD_M2M_FREQLOCK_QRY,
#endif

    AT_CMD_CRRCONN_SET,
    AT_CMD_CRRCONN_QRY,
    AT_CMD_VTRLQUALRPT_SET,

    AT_CMD_MODEM_CAP_UPDATE_SET,

    AT_CMD_TAS_TEST_SET,
    AT_CMD_TAS_TEST_QRY,

    AT_CMD_REJCALL_SET,
    AT_CMD_CSCHANNELINFO_QRY,

    AT_CMD_IMSIPCAPCFG_SET,
    AT_CMD_IMSIPCAPCFG_QRY,

    AT_CMD_IMSVIDEOCALLCANCEL_SET,

    AT_CMD_IMSREGERRRPT_SET,
    AT_CMD_IMSREGERRRPT_QRY,

    AT_CMD_CNAP_QRY,
    AT_CMD_CNAPEX_QRY,

    AT_CMD_RSRP_QRY,
    AT_CMD_RSRQ_QRY,

    AT_CMD_ERRCCAPCFG_SET,
    AT_CMD_ERRCCAPQRY_SET,

    AT_CMD_PSEUCELL_SET,
    AT_CMD_BLOCKCELLLIST_SET,
    AT_CMD_BLOCKCELLLIST_QRY,

    AT_CMD_MIPIREAD_SET,

    AT_CMD_PHYMIPIWRITE_SET,

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)
    AT_CMD_DCXOQRY_QRY,
#endif

    AT_CMD_IMS_SRV_STAT_RPT_SET,
    AT_CMD_IMS_SRV_STAT_RPT_QRY,
    AT_CMD_IMS_SERVICE_STATUS_QRY,

    AT_CMD_CHRALARMRLATCFG_SET,

    AT_CMD_ECCCFG_SET,

    AT_CMD_EPDU_SET,
#if (FEATURE_LTE == FEATURE_ON)
    AT_CMD_LTE_CA_CFG_SET,
    AT_CMD_LTE_CA_CELLEX_QRY,
    AT_CMD_LCACELLRPTCFG_SET,
    AT_CMD_LCACELLRPTCFG_QRY,
    AT_CMD_FINE_TIME_SET,
    AT_CMD_OVER_HEATING_SET,
    AT_CMD_OVER_HEATING_QRY,
#endif

    AT_CMD_VTFLOWRPT_SET,

    AT_CMD_WIEMCAID_SET,

    AT_CMD_DMRCSCFG_SET,
    AT_CMD_RCSSWITCH_SET,
    AT_CMD_RCSSWITCH_QRY,

    AT_CMD_USERAGENTCFG_SET,

    AT_CMD_DATASWITCH_SET,
    AT_CMD_DATASWITCH_QRY,

    AT_CMD_DATAROAMSWITCH_SET,
    AT_CMD_DATAROAMSWITCH_QRY,

    AT_CMD_COMM_BOOSTER_SET,
    AT_CMD_COMM_BOOSTER_QRY,

    AT_CMD_NVLOAD_SET,

    AT_CMD_CIND_SET,

    AT_CMD_SMSDOMAIN_SET,
    AT_CMD_SMSDOMAIN_QRY,

    AT_CMD_APN_THROT_INFO_SET,

    AT_CMD_WS46_SET,
    AT_CMD_WS46_QRY,

#if (FEATURE_DSDS == FEATURE_ON)
    AT_CMD_DSDS_STATE_SET,
#endif

    AT_CMD_SAMPLE_SET,

    AT_CMD_GPSLOCSET_SET,

    AT_CMD_CCLK_QRY,

    AT_CMD_GAME_MODE_SET,

    AT_CMD_VICECFG_SET,
    AT_CMD_VICECFG_QRY,
    AT_CMD_RTTCFG_SET,
    AT_CMD_RTTMODIFY_SET,

    AT_CMD_PSEUDBTS_SET,

    AT_CMD_SUBCLFSPARAM_SET,
    AT_CMD_SUBCLFSPARAM_QRY,

    AT_CMD_NVREFRESH_SET,
    AT_CMD_TIMEQRY_QRY,

    AT_CMD_REJINFO_QRY,

    AT_CMD_JDCFG_SET,
    AT_CMD_JDCFG_READ,
    AT_CMD_JDSWITCH_SET,
    AT_CMD_JDSWITCH_READ,

    AT_CMD_EMRSSICFG_SET,
    AT_CMD_EMRSSICFG_QRY,
    AT_CMD_EMRSSIRPT_SET,
    AT_CMD_EMRSSIRPT_QRY,

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_SLT_TEST_SET,
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_CMD_CLDBDOMAINSTATUS_SET,
#endif

    AT_CMD_PLMNSRCH_SET,

    AT_CMD_CDMAMODEMSWITCH_SET,
    AT_CMD_CDMACAPRESUME_SET,

    AT_CMD_DCONNSTAT,

    AT_CMD_FTEMPRPT_QRY,

    AT_CMD_PSSCENE_SET,
    AT_CMD_PSSCENE_QRY,

    AT_CMD_SMSANTIATTACK_SET,
    AT_CMD_SMSNASCAP_QRY,
    AT_CMD_SMSANTIATTACKCAP_QRY,

    AT_CMD_FORCESYNC_SET,
    AT_CMD_LTEATTACHINFO_QRY,
    AT_CMD_SIPPORT_SET,
    AT_CMD_SIPPORT_QRY,

    AT_CMD_MIPIOPERATE_SET,

    AT_CMD_NDISDUP_SET,
    AT_CMD_IPV6TEMPADDR_SET,
    AT_CMD_DHCP_SET,
    AT_CMD_DHCP_QRY,
    AT_CMD_DHCPV6_SET,
    AT_CMD_DHCPV6_QRY,
    AT_CMD_APRAINFO_SET,
    AT_CMD_APRAINFO_QRY,
    AT_CMD_APLANADDR_SET,
    AT_CMD_APLANADDR_QRY,
    AT_CMD_APCONNST_SET,
    AT_CMD_APCONNST_QRY,
    AT_CMD_DCONNSTAT_QRY,
    AT_CMD_DCONNSTAT_TEST,
    AT_CMD_NDISSTATQRY_QRY,
    AT_CMD_CGMTU_SET,

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
    AT_CMD_NOCARD_SET,
#endif
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_LENDC_SET,
    AT_CMD_LENDC_QRY,
    AT_CMD_NL2COMCFG_SET,
    AT_CMD_NL2COMCFG_QRY,
    AT_CMD_NPDCP_SLEEPTHRES_SET,
    AT_CMD_NRRCCAPCFG_SET,
    AT_CMD_NRRCCAPQRY_SET,
    AT_CMD_NRCACELL_QRY,
    AT_CMD_NRCACELLRPTCFG_SET,
    AT_CMD_NRCACELLRPTCFG_QRY,
    AT_CMD_NRPWRCTRL_SET,
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    AT_CMD_NRFREQLOCK_SET,
    AT_CMD_NRFREQLOCK_QRY,
#endif
    AT_CMD_MULTI_PMU_DIE_ID_QRY,
    AT_CMD_NRNWCAPRPTCFG_SET,
    AT_CMD_NRNWCAPQRY_SET,
    AT_CMD_NRNWCAPRPTQRY_SET,
#endif

    AT_CMD_C5GQOS_SET,
    AT_CMD_C5GQOS_READ,
    AT_CMD_LTEPWRDISS_SET,

    AT_CMD_LTESARSTUB_SET,

    AT_CMD_C5GQOSRDP_SET,

    AT_CMD_LL2COMCFG_SET,
    AT_CMD_LL2COMCFG_QRY,

    AT_CMD_RRCSTAT_QRY,
    AT_CMD_MCS_SET,
    AT_CMD_TXPOWER_QRY,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_NTXPOWER_QRY,
    AT_CMD_CPOLICYRPT_SET,
    AT_CMD_CPOLICYCODE_QRY,

    AT_CMD_C5GPNSSAI_SET,
    AT_CMD_C5GDFTCFGNSSAI_SET,
    AT_CMD_C5GNSSAA_SET,
    AT_CMD_C5GNSSAIRDP_SET,
    AT_CMD_C5GPNSSAI_QRY,
    AT_CMD_C5GNSSAI_QRY,
    AT_CMD_CLADN_SET,
    AT_CMD_CLADN_QRY,
    AT_CMD_CSUEPOLICY_SET,
    AT_CMD_IMSURSP_SET,
#endif
    AT_CMD_HFREQINFO_QRY,

    /* GUģAT����͹�������ĵ�ǰ����ID�����������IDʱҪ��ӵ���IDǰ */
    AT_CMD_COMM_CURRENT_OPT,

    AT_CMD_ROAMPDPTYPE_SET,

    AT_CMD_LRRC_UE_CAP_SET,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_5G_OPTION_SET,
    AT_CMD_5G_OPTION_QRY,
    AT_CMD_NRRC_UE_CAP_SET,
    AT_CMD_NR_BAND_BLOCKLIST_QRY,
    AT_CMD_NR_BAND_BLOCKLIST_SET,
    AT_CMD_NR_SSB_ID_QRY,
#endif
#if (FEATURE_LTEV == FEATURE_ON)
    AT_CMD_CATM_SET,
    AT_CMD_CATM_QRY,
    AT_CMD_CCUTLE_SET,
    AT_CMD_CCUTLE_QRY,
    AT_CMD_CUSPCREQ_QRY,
    AT_CMD_CUTCR_SET,
    AT_CMD_CCBRREQ_QRY,
    AT_CMD_CBR_QRY,
    AT_CMD_CV2XDTS_SET,
    AT_CMD_CV2XDTS_QRY,
    AT_CMD_VSYNCSRC_QRY,
    AT_CMD_VSYNCSRCRPT_SET,
    AT_CMD_VSYNCSRCRPT_QRY,
    AT_CMD_VSYNCMODE_QRY,
    AT_CMD_VSYNCMODE_SET,

    AT_CMD_SLINFO_SET,
    AT_CMD_SLINFO_QRY,
    AT_CMD_PHYP_SET,
    AT_CMD_PHYP_QRY,
    AT_CMD_PTRRPT_SET,
    AT_CMD_PTRRPT_QRY,
    AT_CMD_RPPCFG_QRY,
    AT_CMD_RPPCFG_SET,
    AT_CMD_GNSS_SET,
    AT_CMD_DATA_QRY,
    AT_CMD_GNSSINFO_QRY,
    AT_CMD_VPHYSTAT_QRY,
    AT_CMD_VPHYSTATCLR_SET,
    AT_CMD_VSNRRSRP_QRY,
    AT_CMD_V2XRSSI_QRY,
    AT_CMD_VRSSI_QRY,
    AT_CMD_SENDDATA_SET,
    AT_CMD_VTXPOWER_SET,
    AT_CMD_PC5SYNC_QRY,
    AT_CMD_PC5SYNC_SET,
    AT_CMD_CV2XL2ID_SET,
    AT_CMD_CV2XL2ID_QRY,
#if (FEATURE_LTEV_WL == FEATURE_ON)
    AT_CMD_PC5SYNC_QRY,
    AT_CMD_PC5SYNC_SET,
    AT_CMD_MCGCFG_SET,
    AT_CMD_LEDTEST_SET,
    AT_CMD_GPIOTEST_SET,
    AT_CMD_ANTTEST_SET,
    AT_CMD_TEMPHEATTEST_SET,
    AT_CMD_HKADCTEST_SET,
    AT_CMD_GPIO_QRY,
    AT_CMD_GPIO_SET,
    AT_CMD_BOOTMODE_SET,
    AT_CMD_FILE_WRITE,
    AT_CMD_FILE_READ,
    AT_CMD_SYNCST_QRY,
    AT_CMD_GNSSSYNCST_QRY,
#endif
#endif
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    AT_CMD_ATNLPROXY_CURRENT_OPT,
#endif
#if (FEATURE_AT_PROXY == FEATURE_ON)
    AT_CMD_ATP_OPT,
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
    AT_CMD_HSMF_SET,
    AT_CMD_LTEPROFILE_SET,
    AT_CMD_LTEAPNATTACH_SWITCH,
#endif
    AT_CMD_SINGLEPDNSWITCH_SET,
    AT_CMD_SINGLEPDNSWITCH_QRY,

    AT_CMD_RMNETCFG_SET,

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_CMD_NRPOWERSAVINGCFG_SET,
    AT_CMD_NRPOWERSAVINGQRY_SET,
#endif

    AT_CMD_INVALID,
} AT_CmdCurrentOpt;

typedef enum {
    AT_STRING_SM = 0,
    AT_STRING_ME,
    AT_STRING_ON,
    AT_STRING_EN,
    AT_STRING_FD,
    AT_STRING_BD,
    AT_STRING_REC_UNREAD_TEXT,
    AT_STRING_REC_READ_TEXT,
    AT_STRING_STO_UNSENT_TEXT,
    AT_STRING_STO_SENT_TEXT,
    AT_STRING_ALL_TEXT,
    AT_STRING_REC_UNREAD_PDU,
    AT_STRING_REC_READ_PDU,
    AT_STRING_STO_UNSENT_PDU,
    AT_STRING_STO_SENT_PDU,
    AT_STRING_ALL_PDU,
    AT_STRING_IP,
    AT_STRING_IPv4,
    AT_STRING_PPP,
    AT_STRING_IPV6,
    AT_STRING_IPV4V6,
    AT_STRING_IPv6,
    AT_STRING_IPv4v6,
    AT_STRING_Ethernet,

    AT_STRING_0E0,
    AT_STRING_1E2,
    AT_STRING_7E3,
    AT_STRING_1E3,
    AT_STRING_1E4,
    AT_STRING_1E5,
    AT_STRING_1E6,
    AT_STRING_1E1,
    AT_STRING_5E2,
    AT_STRING_5E3,
    AT_STRING_4E3,
    AT_STRING_6E8,
    AT_STRING_CREG,
    AT_STRING_CGREG,
#if (FEATURE_LTE == FEATURE_ON)
    AT_STRING_CEREG,
#endif
    AT_STRING_SRVST,
    AT_STRING_MODE,
    AT_STRING_RSSI,
    AT_STRING_TIME,
    AT_STRING_CTZV,
    AT_STRING_CTZE,
    AT_STRING_ERRRPT,
    AT_STRING_CCALLSTATE,

    AT_STRING_CERSSI,

    AT_STRING_ACINFO,

    AT_STRING_CS_CHANNEL_INFO,

    AT_STRING_RESET,

    AT_STRING_REFCLKFREQ,

    AT_STRING_C5GNSSAA,

    AT_STRING_CPENDINGNSSAI,

    AT_STRING_REJINFO,

    AT_STRING_PLMNSELEINFO,

    AT_STRING_NETSCAN,

#if (FEATURE_IMS == FEATURE_ON)
    AT_STRING_CIREPH,
    AT_STRING_CIREPI,
    AT_STRING_CIREGU,

    AT_STRING_CALL_MODIFY_IND,
    AT_STRING_CALL_MODIFY_BEG,
    AT_STRING_CALL_MODIFY_END,

    AT_STRING_ECONFSTATE,

#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    AT_STRING_CDISP,
    AT_STRING_CCONNNUM,
    AT_STRING_CCALLEDNUM,
    AT_STRING_CCALLINGNUM,
    AT_STRING_CREDIRNUM,
    AT_STRING_CSIGTONE,
    AT_STRING_CLCTR,
    AT_STRING_CCWAC,
#endif

    AT_STRING_FILECHANGE,

#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_LTE_MBMS == FEATURE_ON))
    AT_STRING_MBMSEV,
#endif
    AT_STRING_SRCHEDPLMNLIST,

    AT_STRING_MCC,

    AT_STRING_CMOLRE,
    AT_STRING_CMOLRN,
    AT_STRING_CMOLRG,
    AT_STRING_CMTLR,

#if (FEATURE_IMS == FEATURE_ON)
    AT_STRING_DMCN,
#endif

    AT_STRING_IMS_REG_FAIL,

    AT_STRING_IMSI_REFRESH,

    AT_STRING_AFCCLKUNLOCK,

    AT_STRING_IMS_HOLD_TONE,

    AT_STRING_LIMITPDPACT,

    AT_STRING_IMS_REG_ERR,

    AT_STRING_BLOCK_CELL_MCC,

    AT_STRING_CLOUD_DATA,

    AT_STRING_ECC_STATUS,

    AT_STRING_EPDUR,

    AT_STRING_LCACELLEX,
    AT_STRING_VT_FLOW_RPT,
    AT_STRING_CALL_ALT_SRV,

    AT_STRING_FINETIMEINFO,
    AT_STRING_SFN,
    AT_STRING_FINETIMEFAIL,
    AT_STRING_LRRCUECAPINFONTF,
    AT_STRING_PHYCOMACK,

    AT_STRING_BOOSTERNTF,

    AT_STRING_MTREATTACH,

    AT_STRING_IMPU,
#if (FEATURE_DSDS == FEATURE_ON)
    AT_STRING_DSDSSTATE,
#endif

    AT_STRING_TEMPPROTECT,

    AT_STRING_DIALOGNTF,
    AT_STRINT_RTTEVENT,
    AT_STRINT_RTTERROR,

    AT_STRING_EMRSSIRPT,

    AT_STRING_ELEVATOR,

    AT_STRING_ULFREQRPT,

    AT_STRING_PSEUDBTS,
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_STRING_C5GREG,
    AT_STRING_CSERSSI,
    AT_STRING_NRRCUECAPINFONTF,
    AT_STRING_NRCACELLRPT,
    AT_STRING_NRNWCAP,
#endif

    AT_STRING_NDISSTAT,
    AT_STRING_NDISSTATEX,

    AT_STRING_DETECTPLMN,

    AT_STRING_BUTT
} AT_STRING_Content;

#define AT_CMD_CLVL_VAL_MAX 5
enum AT_CmdClvlValue {
    AT_CMD_CLVL_LEV_0 = 0,
    AT_CMD_CLVL_LEV_1 = 50,
    AT_CMD_CLVL_LEV_2 = 60,
    AT_CMD_CLVL_LEV_3 = 68,
    AT_CMD_CLVL_LEV_4 = 74,
    AT_CMD_CLVL_LEV_5 = 80
};

enum AT_CmdSubMode {
    AT_NORMAL_MODE,
    AT_SMS_MODE,
    AT_XML_MODE,
    AT_UE_POLICY_MODE,
    AT_IMS_URSP_MODE,

    AT_CMD_MODE_BUTT,
};
typedef VOS_UINT8 AT_CmdSubModeUint8;

typedef TAF_UINT8 AT_CMD_BLOCK_STATE_TYPE;
#define AT_CMD_READY 0
#define AT_CMD_PEND 1
#define AT_CMD_ABORT 2

typedef TAF_UINT8 AT_CHANNEL_TYPE;
#define AT_CMD_CHANNEL 0
#define AT_DATA_CHANNEL 1

typedef TAF_UINT8 AT_V_TYPE;
#define AT_V_BREVITE_TYPE 0
#define AT_V_ENTIRE_TYPE 1

typedef TAF_UINT8 AT_CMD_ECHO_TYPE;
#define AT_E_NO_ECHO_CMD 0
#define AT_E_ECHO_CMD 1

typedef TAF_UINT8 AT_COPS_MODE_TYPE;
#define AT_COPS_AUTOMATIC_TYPE 0
#define AT_COPS_MANUAL_TYPE 1
#define AT_COPS_DEREGISTER_TYPE 2
#define AT_COPS_ONLY_SET_FORMAT_TYPE 3
#define AT_COPS_MANUAL_AUTOMATIC_TYPE 4



enum AT_COPS_Rat {
    AT_COPS_RAT_GSM   = 0,
    AT_COPS_RAT_WCDMA = 2,
    AT_COPS_RAT_LTE   = 7,
    AT_COPS_RAT_NR    = 12,
    AT_COPS_RAT_BUTT
};

typedef VOS_UINT8 AT_COPS_RatUint8;

typedef TAF_UINT8 AT_D_GPRS_ACTIVE_TYPE;
#define AT_IP_ACTIVE_TE_PPP_MT_PPP_TYPE 0
#define AT_PPP_ACTIVE_TE_PPP_MT_NOT_PPP_TYPE 1
#define AT_IP_ACTIVE_TE_NOT_PPP_MT_NOT_PPP_TYPE 2

typedef TAF_UINT8 AT_DATA_DOMAIN_TYPE;
#define AT_DATA_CS_DOMAIN 0 /* CS�� */
#define AT_DATA_PS_DOMAIN 1 /* PS�� */

typedef TAF_UINT8 AT_SMT_BUFFER_STATE;
#define AT_SMT_BUFFER_EMPTY 0
#define AT_SMT_BUFFER_USED 1

/* ��Ҫ���ٴ���������At_QuickHandleCmd��aucQuickCmd�����е�λ�ö��壬 */
#define AT_QUICK_HANDLE_SYSINFO_CMD 0
#define AT_QUICK_HANDLE_COPS_QRY_CMD 1
#define AT_QUICK_HANDLE_COPS_SET_FORMAT_2_CMD 2
#define AT_QUICK_HANDLE_COPS_SET_FORMAT_0_CMD 3
#define AT_QUICK_HANDLE_CSQ_CMD 4
#define AT_QUICK_HANDLE_SPN_SIM 5
#define AT_QUICK_HANDLE_SPN_USIM 6
#define AT_QUICK_HANDLE_CPMS_SET_CMD 7

typedef enum {
    AT_COM_CMD_SUB_PROC_CONTINUE = 0, /* ������������Ĵ��� */
    AT_COM_CMD_SUB_PROC_ABORT,        /* ��ֹ��������Ĵ��� */
    AT_COM_CMD_SUB_PROC_FINISH,       /* ������������Ĵ��� */

    AT_COM_CMD_SUB_PROC_BUTT
} AT_CombineCmdSubsequentProcess;

typedef enum AT_NvimPara {
    AT_NVIM_CPMS_TYPE = 0,

    AT_NVIM_BUTT_TYPE /* ���� */
} AT_NvimPara;

/*
 * Э����:
 * ö��˵��: AT��������^CDSFLT���ö�����ΪĬ��ֵ
 */
enum AT_CustomizeItemDefaultVal {
    AT_CUSTOMIZE_ITEM_DEFAULT_VALUE_FROM_CFGFILE = 0, /* ������Ĭ��ֵΪ�����������ĵ��е�ֵ */
    AT_CUSTOMIZE_ITEM_DEFAULT_VALUE_FROM_UE,          /* �������е�ֵΪ�����Զ����Ĭ��ֵ */
    AT_CUSTOMIZE_ITEM_DEFAULT_VALUE_BUTT
};
typedef VOS_UINT8 AT_CustomizeItemDfltUint8;

/*
 * Э����:
 * ö��˵��: SD����������
 */
enum AT_SdOprt {
    AT_SD_OPRT_FORMAT = 0,   /* ��ʽ��SD������ */
    AT_SD_OPRT_ERASE,        /* ����SD������ */
    AT_SD_OPRT_WRITE,        /* д���ݵ�SD����ָ����ַ�� */
    AT_SD_OPRT_READ,         /* ��ȡSD��ָ����ַ������ */
    AT_SD_OPRT_QUERY_STATUS, /* ��ѯSD������״̬ */
    AT_SD_OPRT_BUTT
};
typedef VOS_UINT32 AT_SdOprtUint32;

/*
 * Э����:
 * ö��˵��: SD���������
 */
enum AT_SdOprtRslt {
    AT_SD_OPRT_RSLT_FINISH = 0, /* ��һ�� SD��������� */
    AT_SD_OPRT_RSLT_NOT_FINISH, /* ��һ��SD�����ڽ����� */
    AT_SD_OPRT_RSLT_ERR,        /* ��һ��SD������������ */
    AT_SD_OPRT_RSLT_BUTT,
};
typedef VOS_UINT8 AT_SdOprtRsltUint8;

/*
 * Э����:
 * ö��˵��: SD����λ״̬
 */
enum AT_SdExistStatus {
    AT_SD_STATUS_NOT_EXIST = 0, /* SD����λ */
    AT_SD_STATUS_EXIST,         /* SD��λ */
    AT_SD_STATUS_BUTT,
};
typedef VOS_UINT8 AT_SdExistStatusUint8;

/*
 * Э����:
 * ö��˵��: SD������������
 */
enum AT_SdErr {
    AT_SD_ERR_CRAD_NOT_EXIST = 0, /* ��ʾSD������λ */
    AT_SD_ERR_INIT_FAILED,        /* ��ʾSD����ʼ��ʧ�� */
    AT_SD_ERR_OPRT_NOT_ALLOWED,   /* ���������� */
    AT_SD_ERR_ADDR_ERR,           /* ��ʾ<address>��ַ�Ƿ� */
    AT_SD_ERR_OTHER_ERR,          /* ����δ֪���� */
    AT_SD_ERR_BUTT
};
typedef VOS_UINT32 AT_SdErrUint32;

/*
 * Э����:
 * ö��˵��: SD����������
 */
enum AT_SimlockOprt {
    AT_SIMLOCK_OPRT_UNLOCK = 0,    /* Simlock �������� */
    AT_SIMLOCK_OPRT_SET_PLMN_INFO, /* ���������Ŷ� */
    AT_SIMLOCK_OPRT_QRY_STATUS,    /* ��ѯ��ǰ������״̬ */
    AT_SIMLOCK_OPRT_BUTT
};
typedef VOS_UINT8 AT_SimlockOprtUint8;

/*
 * �ṹ˵��: Rrc�汾�ṹ��
 */
typedef struct {
    VOS_UINT8 srcWcdmaRrc; /* �汾�� */
    VOS_UINT8 destHsdpa;   /* �Ƿ�֧��DPA */
    VOS_UINT8 destHsupa;   /* �Ƿ�֧��UPA */
} AT_NvWcdmaRrcConvertTbl;

typedef TAF_UINT8 AT_CPMS_MEM_TYPE;
#define AT_CPMS_MEM_SM_TYPE 0
#define AT_CPMS_MEM_ME_TYPE 1

typedef TAF_UINT8 AT_CLCK_TYPE;
#define AT_CLCK_P2_TYPE 0
#define AT_CLCK_SC_TYPE 1
#define AT_CLCK_AO_TYPE 2
#define AT_CLCK_OI_TYPE 3
#define AT_CLCK_OX_TYPE 4
#define AT_CLCK_AI_TYPE 5
#define AT_CLCK_IR_TYPE 6
#define AT_CLCK_AB_TYPE 7
#define AT_CLCK_AG_TYPE 8
#define AT_CLCK_AC_TYPE 9
#define AT_CLCK_PS_TYPE 10
#define AT_CLCK_FD_TYPE 11
#define AT_CLCK_PN_TYPE 12
#define AT_CLCK_PU_TYPE 13
#define AT_CLCK_PP_TYPE 14

typedef TAF_UINT8 AT_CMLCK_TYPE;
#define AT_CMLCK_PS_TYPE 0
typedef TAF_UINT8 AT_SAT_ENVELOPE_TYPE;
#define AT_SAT_ENVELOPE_MENU 0xD3
#define AT_SAT_ENVELOPE_CALL 0xD4
#define AT_SAT_ENVELOPE_EVENT 0xD6
#define AT_SAT_ENVELOPE_TIMER 0xD7

/* NV��en_NV_Resume_Flag����Чֵ:0��1 */
#define AT_NV_RESUME_SUCC 0
#define AT_NV_RESUME_FAIL 1

/* DIALMODE */
#define AT_DIALMODE_MODEM 0
#define AT_DIALMODE_NDIS 1
#define AT_DIALMODE_DUAL 2
#define AT_DIALMODE_ABNORMAL 3

/* �������ͣ�����ģʽΪ0�� ǿ������Ϊ1 */
#define AT_NOM_LOAD 0
#define AT_FORC_LOAD 1

#define AT_NV_RESTORE_SUCCESS 0
#define AT_NV_RESTORE_FAIL 1

#define AT_NV_RESTORE_RESULT_INIT 0x5A5A5A5A
#define AT_NV_RESTORE_RUNNING 0x5A5A55AA

#define AT_SIMLOCK_MAX_ERROR_TIMES 3 /* Simlock�������ʧ�ܴ��� */

#define TAF_SW_VER_INFO_LEN 31
#define TAF_ISO_VER_INFO_LEN 50
#define TAF_PRODUCT_NAME_LEN 50
#define TAF_WEBUI_VER_LEN 127
#define TAF_FLASH_INFO_LEN 100
#define TAF_NET_MODE_LEN 10

/* WIFI��ģʽ */
#define AT_WIFI_START_NORMAL 0
#define AT_WIFI_START_TEST 1
/* WIFI ��ģʽ */
#define AT_WIFI_SUPPORT_MODE_CW 0
#define AT_WIFI_SUPPORT_MODE_A 1
#define AT_WIFI_SUPPORT_MODE_B 2
#define AT_WIFI_SUPPORT_MODE_G 3
#define AT_WIFI_SUPPORT_MODE_N 4

/* WIFI���� */
#define AT_WIFI_BAND_20M 0
#define AT_WIFI_BAND_40M 1

/* WIFI��������ջ�ģʽ */
#define AT_WIFI_TX_MODE 17
#define AT_WIFI_RX_MODE 18

/* WIFI�����״̬ */
#define AT_WIFI_TX_ON 1
#define AT_WIFI_TX_OFF 0

/* WIFI���ջ�״̬ */
#define AT_WIFI_RX_ON 1
#define AT_WIFI_RX_OFF 0

/* WIFI���ʵ������� */
#define AT_WIFI_POWER_MIN (-15)
#define AT_WIFI_POWER_MAX 30

#define AT_FEATURE_EXIST 1
#if (FEATURE_LTEV == FEATURE_ON)
#define AT_FEATURE_MAX 17 /* ����LTE-V�����֧�ֵ�17��FeatureName */
#else
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
#define AT_FEATURE_MAX 15
#else
#define AT_FEATURE_MAX 16
#endif
#endif
#if (FEATURE_VCOM_EXT == FEATURE_ON)
/* APPVCOM ��� 11 ���˿�Ϊ��AT �� */
#define AT_VCOM_AT_CHANNEL_MAX (APP_VCOM_DEV_INDEX_BUTT - 11)
#else
#define AT_VCOM_AT_CHANNEL_MAX (APP_VCOM_DEV_INDEX_BUTT)
#endif

#define AT_SIMLOCKUNLOCK_PWD_PARA_LEN 16

enum AT_CuusiMsgType {
    AT_CUUSI_MSG_ANY,
    AT_CUUSI_MSG_ALERT,
    AT_CUUSI_MSG_PROGRESS,
    AT_CUUSI_MSG_CONNECT,
    AT_CUUSI_MSG_RELEASE,
    AT_CUUSI_MSG_BUTT
};
typedef VOS_UINT32 AT_CuusiMsgTypeUint32;

enum AT_CuusuMsgType {
    AT_CUUSU_MSG_ANY,
    AT_CUUSU_MSG_SETUP,
    AT_CUUSU_MSG_DISCONNECT,
    AT_CUUSU_MSG_RELEASE_COMPLETE,
    AT_CUUSU_MSG_BUTT
};
typedef VOS_UINT32 AT_CuusuMsgTypeUint32;

typedef struct {
    MN_CALL_Uus1MsgTypeUint32 callMsgType;
    AT_CuusuMsgTypeUint32     cuusuMsgType;
} AT_CALL_CuusuMsg;

typedef struct {
    MN_CALL_Uus1MsgTypeUint32 callMsgType;
    AT_CuusiMsgTypeUint32     cuusiMsgType;
} AT_CALL_CuusiMsg;

typedef TAF_UINT32 (*pAtSetParaFunc)(TAF_UINT8 ucIndex);
typedef TAF_UINT32 (*pAtQueryParaFunc)(TAF_UINT8 ucIndex);

typedef TAF_UINT32 (*pAtAppCBF)(TAF_UINT8 ucIndex, TAF_UINT8 ucType, TAF_UINT8 *pData, TAF_UINT16 usLen);

/* DIAG���Ƿ�򿪵�ö�ٽṹ  */
enum AT_DiagOpenFlag {
    AT_DIAG_OPEN_FLAG_OPEN,
    AT_DIAG_OPEN_FLAG_CLOSE,
    AT_DIAG_OPEN_FLAG_BUTT
};
typedef VOS_UINT32 AT_DiagOpenFlagUint32;

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
/*  SHELL���Ƿ�򿪵�ö�ٽṹ  */
enum AT_ShellOpenFlag {
    AT_SHELL_OPEN_FLAG_WIFI,
    AT_SHELL_OPEN_FLAG_CLOSE,
    AT_SHELL_OPEN_FLAG_OPEN,
    AT_SHELL_OPEN_FLAG_BUTT
};
typedef VOS_UINT32 AT_ShellOpenFlagUint32;
#endif

/* ������NV�е�Ȩ������Ľṹ */
#define AT_DISLOG_PWD_LEN 16 /*  DISLOG����Ȩ�޵����볤��  */

#define AT_SETZ_LEN 16

/* 0 ʹ��SD��; 1 ��ֹSD�� */
enum AT_SdEnableFlag {
    AT_SD_ENABLE_FLAG_ENABLE,
    AT_SD_ENABLE_FLAG_DISABLE,
    AT_SD_ENABLE_FLAG_BUTT
};

enum AT_ClckMode {
    AT_CLCK_MODE_UNLOCK,
    AT_CLCK_MODE_LOCK,
    AT_CLCK_MODE_QUERY_STATUS,
    AT_CLCK_MODE_BUTT
};
typedef VOS_UINT32 AT_ClckModeUint32;

/* ģ���������士HEADER */

/* A CONVERSION_TABLE is an array mapping 7 or 8 bit values to 16 bit values. */
typedef struct {
    VOS_UINT8  octet;
    VOS_UINT8  reserve;
    VOS_UINT16 unicode;
} AT_PB_ConversionTable;

typedef struct {
    MN_MSG_AsciiAddr asciiAddr;                     /* <oa/da>[,<tooa/toda> */
    TAF_UINT8        buffer[AT_SMS_SEG_MAX_LENGTH]; /* ��ʼ��SMSC�ĵ�ַ��֮����TPDU */
    TAF_BOOL         waitForNvStorageStatus;
    TAF_BOOL         waitForUsimStorageStatus;
    TAF_BOOL         waitForCpmsSetRsp;
    TAF_UINT8        cnmaType; /* ��¼+CNMA[=<n>�е�<n> */
    /* TPDU���ֽ�����������SMSC��ַ���ֽ��� */
    TAF_UINT8               pduLen;
    MN_MSG_CommandTypeUint8 commandType;
    TAF_UINT8               messageNumber;     /* [1,3]���߾��Ա�� */
    TAF_UINT8               messageNumberType; /* ������ͣ����Ի������ */
    MN_MSG_StatusTypeUint8  smState;
    MN_MSG_MemStoreUint8    smMemStore;
    TAF_UINT8               msgDeleteTypes;
    TAF_UINT8               msgSentSmNum;
    TAF_UINT8               fo;
    MN_MSG_TpPidTypeUint8   pid; /* TP-PID */
    VOS_UINT8               reserved[1];
} AT_SMS;

typedef struct {
    MN_CALL_CugCfg       cugMode;
    MN_CALL_ClirCfgUint8 clirInfo;
    MN_CALL_TypeUint8    callType;
    MN_CALL_ID_T         callId;
    VOS_UINT8            reserved[5];
} AT_CALL_Info;

typedef struct {
    MN_CLIENT_ID_T    clientId; /* ָʾ��ǰ�û��� */
    MN_OPERATION_ID_T opId;     /* Operation ID, ��ʶ���β��� */
    TAF_UINT8         used;     /* ָʾ��ǰ�����Ƿ��ѱ�ʹ�� */

    DMS_PortIdUint16   portNo;    /* ָʾ��ǰUSB COM���ں� */
    AT_USER_TYPE       userType;    /* ָʾ��ǰ�û����� */
    AT_IND_MODE_TYPE   indMode;     /* ָʾ��ǰ����ģʽ��ֻ���MUX��APP */

    TAF_UINT8 csRabId;
    TAF_UINT8 exPsRabId;
    TAF_UINT8 cid;
    VOS_UINT8 reserved;

#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    TAF_UINT32 isWaitAts;                                        /* ָʾ�Ƿ��ڵȴ�ATS����AT������ʶATSͨ��*/
    TAF_INT magic;                                               /* ָʾ��ǰ�ܵ����ڵȴ���magic*/
    TAF_UINT32 canAbort;                                         /* ָʾ��ǰ�ܵ����ڿɴ��״̬*/
#endif
#if (FEATURE_AT_PROXY== FEATURE_ON)
    VOS_UINT32 magic;                          /* ָʾ��ǰ�ܵ����ڵȴ���magic*/
#endif
    AT_CmdCurrentOpt cmdCurrentOpt; /* ָʾ��ǰ�û�������������� */

    AT_CALL_Info atCallInfo;

    TAF_UINT16 pppId;

    TAF_UINT16 smsTxtLen; /* ָʾ��ǰ�����ı����� */
    TAF_UINT16 asyRtnNum; /* ָʾ��ǰ����������� */

    VOS_UINT8 ifaceId;
    VOS_UINT8 reserved2;

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    VOS_UINT8 dataId; /* ָʾCMUX SNC��Ӧ��DATAͨ��*/
    VOS_UINT8 ctrlId; /* ָʾCMUX SNCʹ��֮���·�ATD��ͨ��*/
    VOS_UINT8 reserved1[2];
#endif

    HTIMER     hTimer; /* ��Ӧ�Ķ�ʱ�� */
    TAF_UINT32 temp;

    AT_CmdIndex cmdIndex; /* ָʾ��ǰ�û����������� */

    AT_SMS atSmsData;
} AT_ClientManage;

typedef struct {
    TAF_UINT8  buffer[AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN]; /* �������ý������Ĳ����ַ��� */
    TAF_UINT16 bufLen;                                      /* ������ʶ�����ַ������� */
} AT_SEND_DataBuffer;

/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_RreturnCodeUint32 index;
    TAF_UINT8           *result[2];
} AT_RETURN_TabType;
/*lint +e958 +e959 ;cause:64bit*/

typedef struct {
    AT_CPMS_MEM_TYPE readMem1;
    AT_CPMS_MEM_TYPE deleteMem2;
    AT_CPMS_MEM_TYPE writeMem3;
    AT_CPMS_MEM_TYPE sendMem4;
} AT_CPMS_Type;

/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_STRING_Content strType;
    TAF_UINT8        *text;
} AT_STRING_Type;
/*lint +e958 +e959 ;cause:64bit*/

typedef TAF_INT (*USB_SERIAL_SEND_DATA)(TAF_UINT8 *pData, /* ����ָ�� */
                                        TAF_UINT16 usLen  /* ���ݳ��� */
);

#define AT_MSG_BUFFER_USED 1
#define AT_MSG_BUFFER_FREE 0

typedef struct {
    TAF_UINT8  index;
    TAF_UINT8  reserve1[5];
    TAF_UINT16 dataLen;
    TAF_UINT8 *data;
} AT_PS_ModemDataInd;

/* Taf_DefCsAutoAnswerInfo�����Ĳ������� */
typedef TAF_UINT32 TAF_CS_AUTO_ANSWER_INFO; /* ����CS���Զ�Ӧ��ʱ�䣬��λΪ�룬
                                               �����0����ʾ��֧���Զ�Ӧ�� */

typedef struct {
    TAF_UINT16 used;       /* ��ʹ�ü�¼�� */
    TAF_UINT16 total;      /* ����¼�� */
    TAF_UINT16 nameMaxLen; /* ������󳤶� */
    TAF_UINT16 numMaxLen;  /* ������󳤶� */
    TAF_UINT16 anrNumLen;  /*  ANR������󳤶� */
    TAF_UINT16 emailLen;   /*  Email��󳤶� */
} AT_PB_Info;

typedef MN_CALL_NumTypeUint8 AT_CSTA_NumTypeUint8;

/* USB�豸��̬NV��Ľṹ�� */
typedef struct {
    TAF_UINT32 status;   /* 1: NV��Ч��־λ��0����Ч */
    TAF_UINT32 value;    /* �豸��̬index */
    TAF_UINT32 reserve1; /* ���� */
    TAF_UINT32 reserve2; /* ���� */
} AT_USB_EnumStatus;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: en_NV_Item_WIFI_KEY NV���Ӧ�Ľṹ��
 *           ���ڱ���wifi ��ȫ��Ϣ
 */
typedef struct {
    VOS_UINT8 wifiSsid[AT_WIFI_SSID_LEN_MAX];
    VOS_UINT8 reserved[83];
} AT_WifiSsid;


typedef struct {
    VOS_UINT8 wifiAuthmode[AT_WIFI_WLAUTHMODE_LEN];
    VOS_UINT8 wifiBasicencryptionmodes[AT_WIFI_ENCRYPTIONMODES_LEN];
    VOS_UINT8 wifiWpaencryptionmodes[AT_WIFI_ENCRYPTIONMODES_LEN];
    VOS_UINT8 wifiWepKey1[AT_WIFI_KEY_LEN_MAX];
    VOS_UINT8 wifiWepKey2[AT_WIFI_KEY_LEN_MAX];
    VOS_UINT8 wifiWepKey3[AT_WIFI_KEY_LEN_MAX];
    VOS_UINT8 wifiWepKey4[AT_WIFI_KEY_LEN_MAX];
    VOS_UINT8 wifiWepKeyIndex[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT8 wifiWpapsk[AT_WIFI_WLWPAPSK_LEN];
    VOS_UINT8 wifiSuspectWEnbl;
    VOS_UINT8 wifiSuspectWCfg;
    VOS_UINT8 ucreserve;
} AT_WifiSec;

/* �豸��̬�ĸ���:A1,A2,A,B,D,E,1,2,3,4,5,6,16 */
#define AT_SETPORT_DEV_LEN 21

/* �豸��̬��Ӧ���� */
#define AT_SETPORT_DEV_MAP_LEN (AT_SETPORT_DEV_LEN + 1)

/* �豸��̬���17����� */
#define AT_SETPORT_PARA_MAX_LEN 17

/* �豸��̬��ϲ�������ַ��������17���豸��̬�����2���ַ���16������ */
#define AT_SETPORT_PARA_MAX_CHAR_LEN ((17 * 2) + 16)


typedef enum tagAT_DEV__PID_UNIFY_IF_PROT {
    AT_DEV_VOID         = 0x00,
    AT_DEV_MODEM        = 0x01,
    AT_DEV_PCUI         = 0x02,
    AT_DEV_DIAG         = 0x03,
    AT_DEV_PCSC         = 0x04,
    AT_DEV_GPS          = 0x05,
    AT_DEV_GPS_CONTROL  = 0x06,
    AT_DEV_NDIS         = 0x07,
    AT_DEV_NDISDATA     = 0x08,
    AT_DEV_NDISCTRL     = 0x09,
    AT_DEV_BLUE_TOOTH   = 0x0A,
    AT_DEV_FINGER_PRINT = 0x0B,
    AT_DEV_ACMCTRL      = 0x0C,
    AT_DEV_MMS          = 0x0D,
    AT_DEV_PC_VOICE     = 0x0E,
    AT_DEV_DVB          = 0x0F,
    AT_DEV_4G_MODEM     = 0x10,
    AT_DEV_4G_NDIS      = 0x11,
    AT_DEV_4G_PCUI      = 0x12,
    AT_DEV_4G_DIAG      = 0x13,
    AT_DEV_4G_GPS       = 0x14,
    AT_DEV_4G_PCVOICE   = 0x15,
    AT_DEV_NCM          = 0x16,
    AT_DEV_LTE_DIAG     = 0x17,
    AT_DEV_CDROM        = 0xA1,
    AT_DEV_SD           = 0xA2,
    AT_DEV_RNDIS        = 0xA3,
    AT_DEV_NONE         = 0xFF
} AT_DEV_PID_UNIFY_IF_PROT;
/* PORT */
typedef struct {
    VOS_UINT8 atSetPara[AT_SET_PARA_MAX_LEN];
    VOS_UINT8 drvPara;
    VOS_UINT8 atDispPara[AT_DISP_PARA_MAX_LEN];
} AT_SetPortParaMap;

/* AT����������������ݽṹ */

/* ���������ع��Ķ���(����Ŀǰ�������������Ĺ��) */
#define AT_COMBINE_MAX_SEMICOLON 40    /* һ��AT���������֧��40���ֺ�(refer to Q) */
#define AT_COMBINE_MAX_COMBINE_CMD 40  /* һ��AT�������ɷֽ�Ϊ40���������(refer to Q) */
#define AT_COMBINE_MAX_LEN 545         /* һ���������������545���ֽ�(refer to Q) */
#define AT_COMBINE_MAX_SUB_CMD 40      /* һ������������ɷֽ�Ϊ40������AT����(refer to Q) */
#define AT_INDEPENDENT_CMD_MAX_LEN 545 /* һ��������������ֽ�Ķ���AT�����Ϊ545���ֽ�(refer to Q) */

/* ����AT��������е�';'����ʹ�õĽṹ�� */
typedef struct {
    VOS_UINT32 count;                                /* AT���������';'�ų��ֵĴ��� */
    VOS_UINT16 scPosition[AT_COMBINE_MAX_SEMICOLON]; /* AT���������';'�ų��ֵ�λ��(��������) */
} AT_CombineCmdSemicolonParse;

typedef struct {
    VOS_UINT32 count; /* AT�������ֺ����ָ������������� */
    struct {
        VOS_UINT32 len;
        VOS_UINT8  combineCmd[AT_COMBINE_MAX_LEN];
        VOS_UINT8  reserved[3];
    } combineCmd[AT_COMBINE_MAX_COMBINE_CMD];
} AT_CombineCmdSplit; /* ���ݷֺŽ���AT�����ķָ�����AT�����ָ�Ϊ���ɸ����AT���� */

typedef struct {
    VOS_UINT32 totalCnt; /* ����AT����ĸ��� */
    VOS_UINT32 procCnt;  /* �Ѿ�������ϵĶ���AT������� */
    struct {
        VOS_UINT32 len;
        VOS_UINT8  atCmd[AT_INDEPENDENT_CMD_MAX_LEN];
        VOS_UINT8  reserved[3]; /* ���ֽڶ��� */
    } atCmd[AT_COMBINE_MAX_SUB_CMD];
} AT_CombineCmdParseRslt; /* ���AT���������� */

typedef struct {
    TAF_UINT32 buffLen;                   /* ���������ĳ��� */
    TAF_UINT8  dataBuff[AT_COM_BUFF_LEN]; /* �������� */
} AT_DataStreamBuffer;                    /* AT��������ṹ�� */

/* begin V7R1 PhaseI Modify */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    MN_PH_SysModeExUint8 sysMode;
    VOS_CHAR            *strSysModeName;
} AT_PH_SysModeTbl;

typedef struct {
    MN_PH_SubSysModeExUint8 subSysMode;
    VOS_CHAR               *strSubSysModeName;
} AT_PH_SubSysModeTbl;
/*lint +e958 +e959 ;cause:64bit*/

/* end V7R1 PhaseI Modify */


enum AT_RRC_Version {
    AT_RRC_VERSION_WCDMA       = 0,
    AT_RRC_VERSION_DPA         = 1,
    AT_RRC_VERSION_DPA_AND_UPA = 2,
    AT_RRC_VERSION_HSPA_PLUNS  = 6,
    AT_RRC_VERSION_BUTT
};
typedef VOS_UINT8 AT_RRC_VersionUint8;


enum AT_PtlVerType {
    AT_PTL_VER_ENUM_R3,
    AT_PTL_VER_ENUM_R4,
    AT_PTL_VER_ENUM_R5,
    AT_PTL_VER_ENUM_R6,
    AT_PTL_VER_ENUM_R7,
    AT_PTL_VER_ENUM_R8,
    AT_PTL_VER_ENUM_R9,
    AT_PTL_VER_ENUM_BUTT
};
typedef VOS_UINT8 AT_PtlVerTypeUint8;

/*
 * �ṹ˵��: DPACAT�������HSDPA���ʵȼ�
 *                         0: ֧�����ʵȼ�3.6M
 *                         1: ֧�����ʵȼ�7.2M
 *                         2: ֧�����ʵȼ�1.8M
 *                         3: ֧�����ʵȼ�14.4M
 *                         4: ֧�����ʵȼ�21M
 */
enum AT_DpacatCategoryType {
    AT_DPACAT_CATEGORY_TYPE_3600K  = 0,
    AT_DPACAT_CATEGORY_TYPE_7200K  = 1,
    AT_DPACAT_CATEGORY_TYPE_1800K  = 2,
    AT_DPACAT_CATEGORY_TYPE_14400K = 3,
    AT_DPACAT_CATEGORY_TYPE_21000K = 4,
    AT_DPACAT_CATEGORY_TYPE_BUTT
};
typedef VOS_UINT32 AT_DpacatCategoryTypeUint32;


typedef struct {
    /* �Ƿ�֧��enHSDSCHSupport�ı�־                */
    PS_BOOL_ENUM_UINT8 hsdschSupport;
    /* R6�汾֧��HS-DSCH���������ͱ�־                  */
    VOS_UINT8 hsdschPhyCategory;
    /* �Ƿ�֧����ǿ��HSDPA�ı�־                */
    PS_BOOL_ENUM_UINT8 macEhsSupport;
    /* R6�汾�����ϰ汾֧��HS-DSCH���������ͱ�־    */
    VOS_UINT8 hsdschPhyCategoryExt;
    /* �Ƿ�֧�� Multi cell support,���֧��MultiCell,Ex2���� */
    PS_BOOL_ENUM_UINT8 multiCellSupport;

} AT_DpacatPara;



typedef struct {
    /* 0��ʾδ����,��ôDPA��UPA��֧��;1��ʾ���� */
    VOS_UINT32 hspaStatus;

    /* RF ������Ϣ                                  */
    VOS_UINT8 rsv1[4];

    /* 16QAM������أ��Ƿ�֧��E-DPCCH Power Boosting */
    PS_BOOL_ENUM_UINT8 supportPwrBoosting;
    /* �Ƿ�֧��ul dpcch ʹ�� slotFormat4 */
    PS_BOOL_ENUM_UINT8 sf4Support;

    /* ENUMERATED  OPTIONAL                         */
    VOS_UINT8 rsv2[1];
    /* Access Stratum Release Indicator             */
    AT_PtlVerTypeUint8 asRelIndicator;

    /* �Ƿ�֧��enHSDSCHSupport�ı�־                */
    PS_BOOL_ENUM_UINT8 hsdschSupport;
    /* ֧��HS-DSCH���������ͱ�־                  */
    VOS_UINT8 hsdschPhyCategory;

    PS_BOOL_ENUM_UINT8 macEhsSupport;
    VOS_UINT8          hsdschPhyCategoryExt;
    /* �Ƿ�֧�� Multi cell support,���֧��MultiCell,Ex2���� */
    PS_BOOL_ENUM_UINT8 multiCellSupport;
    /* HS-DSCH physical layer category extension 2 */
    VOS_UINT8 hsdschPhyCategoryExt2;

    PS_BOOL_ENUM_UINT8 cellSpecTxDiversityForDc; /*  This IE is optionally present if Dual-Cell HSDPA is supported.
                                                      Otherwise it is not needed.
                                                       The IE is not needed in the INTER RAT HANDOVER INFO message.
                                                      Otherwise, it is optional */
    /* �����Ժ���չHS-DSCH physical layer category extension 3 */
    VOS_UINT8 rsv3[1];

    /* �Ƿ�֧��EDCH�ı�־                           */
    PS_BOOL_ENUM_UINT8 edchSupport;
    /* ֧��UPA�ĵȼ�                                */
    VOS_UINT8 edchPhyCategory;
    /* �Ƿ�֧������16QAM����֧��ʱucEDCHPhyCategoryExt����Ч */
    PS_BOOL_ENUM_UINT8 suppUl16Qam;
    VOS_UINT8          edchPhyCategoryExt; /* ���е��ز�֧��16QAMʱ����д7 */
    /* 16QAM������أ��Ƿ�֧��E-DPDCH power interpolation formula */
    PS_BOOL_ENUM_UINT8 suppEDpdchInterpolationFormula;
    PS_BOOL_ENUM_UINT8 suppHsdpaInFach; /* ֧��CELL_FACH��HS-DSCH�Ľ��� */
    /* ֧��CELL_PCH��URA_PCH��HS-DSCH�Ľ��� */
    PS_BOOL_ENUM_UINT8 suppHsdpaInPch;

    PS_BOOL_ENUM_UINT8 macIsSupport; /* �Ƿ�֧��MAC_I/MAC_Is */

    /* �Ƿ�֧��FDPCH�ı�־                          */
    PS_BOOL_ENUM_UINT8 fdpchSupport;

    /* �Ƿ�֧�� hsscchlessHsdschOperation           */
    PS_BOOL_ENUM_UINT8 hsscchLessSupport;
    /* �Ƿ�֧�� discontinuousDpcchTransmission      */
    PS_BOOL_ENUM_UINT8 ulDpcchDtxSupport;

    /* �Ƿ�֧�� Adjacent Frequency measurements without compressed mode */
    PS_BOOL_ENUM_UINT8 adjFreqMeasWithoutCmprMode;

    PS_BOOL_ENUM_UINT8 mimoSingleStreamStrict; /* �Ƿ�����ֻ��ʹ�õ���MIMO */
    /* R9���ԣ���MIMO����ʱ�����п����ŵ��Ƿ�����ʹ�÷ּ� */
    PS_BOOL_ENUM_UINT8 mimoWithDlTxDiversity;

    /* V7r1 ˫ģ����NV�� LTE���� */
    /* ֧��UTRA�е����ȼ���ѡ��Ĭ��Ϊ0��1Ϊ֧�֣�0Ϊ��֧�� */
    PS_BOOL_ENUM_UINT8 sptAbsPriBasedReselInUtra;

    /* HS-DSCH physical layer category extension 3 */
    VOS_UINT8          hsdschPhyCategoryExt3;
    PS_BOOL_ENUM_UINT8 dcMimoSupport; /* �Ƿ�֧��DC+MIMO */
    PS_BOOL_ENUM_UINT8 suppCommEdch;  /* E-RACH�������� */

    PS_BOOL_ENUM_UINT8 dcUpaSupport;        /* �Ƿ�֧��DC UPA�ı�־ */
    VOS_UINT8          edchPhyCategoryExt2; /* EDCH  category extension 2 */
    PS_BOOL_ENUM_UINT8 edpdchGainFactorFlg; /* E-DPDCH���ʻ�������ʹ�ܱ�־λ */
    VOS_UINT8          rsv4[7];
} AT_NvimUeCapa;


enum AT_NVWR_SecType {
    AT_NVWR_SEC_TYPE_OFF = 0,   /* ��ȫ���ƹر� */
    AT_NVWR_SEC_TYPE_ON,        /* ��ȫ���ƿ��� */
    AT_NVWR_SEC_TYPE_BLOCKLIST, /* ��ȫ����Block����ģʽ */
    AT_NVWR_SEC_TYPE_BUTT
};
typedef VOS_UINT8 AT_NVWR_SecTypeUint8;


enum AT_SyscfgexRatType {
    AT_SYSCFGEX_RAT_AUTO = 0,       /* ���뼼��Ϊ�Զ�ģʽ */
    AT_SYSCFGEX_RAT_GSM,            /* GSM���뼼�� */
    AT_SYSCFGEX_RAT_WCDMA,          /* WCDMA���뼼�� */
    AT_SYSCFGEX_RAT_LTE,            /* LTE���뼼�� */
    AT_SYSCFGEX_RAT_1X,             /* 1X���뼼�� */
    AT_SYSCFGEX_RAT_DO        = 7,  /* DO���뼼�� */
    AT_SYSCFGEX_RAT_NR        = 8,  /* NR���뼼�� */
    AT_SYSCFGEX_RAT_NO_CHANGE = 99, /* ���뼼���ޱ仯 */
    AT_SYSCFGEX_RAT_BUTT
};


enum AT_SYSCFG_RatType {
    AT_SYSCFG_RAT_AUTO        = 2,  /* ���뼼��Ϊ�Զ�ģʽ */
    AT_SYSCFG_RAT_1X          = 3,  /* 1X ģʽ */
    AT_SYSCFG_RAT_HRPD        = 4,  /* HRDP ģʽ */
    AT_SYSCFG_RAT_1X_AND_HRPD = 8,  /* 1X&HRPD ģʽ */
    AT_SYSCFG_RAT_GSM         = 13, /* GGSM���뼼�� */
    AT_SYSCFG_RAT_WCDMA       = 14, /* WCDMA���뼼�� */
    AT_SYSCFG_RAT_NO_CHANGE   = 16, /* ���뼼���ޱ仯 */
    AT_SYSCFG_RAT_BUTT
};
typedef VOS_UINT8 AT_SYSCFG_RatTypeUint8;


enum AT_SYSCFG_RatPrio {
    AT_SYSCFG_RAT_PRIO_AUTO = 0,  /* �������ȼ�����Ϊ�Զ� */
    AT_SYSCFG_RAT_PRIO_GSM,       /* �������ȼ�����ΪGSM����  */
    AT_SYSCFG_RAT_PRIO_WCDMA,     /* �������ȼ�����ΪWCDMA���� */
    AT_SYSCFG_RAT_PRIO_NO_CHANGE, /* �������ȼ�Ϊ���ı� */
    AT_SYSCFG_RAT_PRIO_BUTT
};

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)

enum AT_FreqlockModeType {
    AT_FREQLOCK_MODE_TYPE_GSM     = 1, /* GSM */
    AT_FREQLOCK_MODE_TYPE_WCDMA   = 2, /* WCDMA */
    AT_FREQLOCK_MODE_TYPE_TDSCDMA = 3, /* TD-SCDMA */
    AT_FREQLOCK_MODE_TYPE_LTE     = 4, /* LTE */
    AT_FREQLOCK_MODE_TYPE_BUTT
};
typedef VOS_UINT32 AT_FreqlockModeTypeUint32;


enum AT_FreqlockBandType {
    AT_FREQLOCK_BAND_TYPE_GSM_850  = 0, /* GSMƵ��850 */
    AT_FREQLOCK_BAND_TYPE_GSM_900  = 1, /* GSMƵ��900 */
    AT_FREQLOCK_BAND_TYPE_GSM_1800 = 2, /* GSMƵ��1800 */
    AT_FREQLOCK_BAND_TYPE_GSM_1900 = 3, /* GSMƵ��1900 */
    AT_FREQLOCK_BAND_TYPE_BUTT
};
typedef VOS_UINT32 AT_FreqlockBandTypeUint32;
#endif

typedef VOS_UINT32 (*MN_PS_EVT_FUNC)(VOS_UINT8 ucIndex, TAF_Ctrl *pEvtInfo);

/*
 * �ṹ˵��: PS���¼���������ṹ
 */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    VOS_UINT32        evtId;
    VOS_UINT32        msgLen;
    TAF_ChkMsgLenFunc chkFunc;
    MN_PS_EVT_FUNC    evtFunc;
} AT_PS_EvtFuncTbl;
/*lint +e958 +e959 ;cause:64bit*/

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
typedef struct {
    /* VOS_TRUE:������У�飬VOS_FALSE:����δУ�� */
    VOS_UINT8 shellPwdCheckFlag;
    VOS_UINT8 errTimes; /* ��¼�����������Ĵ��� */
    VOS_UINT8 rsv[2];
} AT_SP_WordCtx;
#endif


typedef struct {
    /* ��Ȩ��Կ����(δ��half-byte����) */
    VOS_UINT8 pubKey[AT_FACAUTHPUBKEY_PARA_LEN];
    /* ��Ȩ��ԿSSKǩ������(δ��half-byte����) */
    VOS_UINT8 pubKeySign[AT_FACAUTHPUBKEY_SIGN_PARA_LEN];
} AT_FACAUTHPUBKEY_SetReq;

typedef struct {
    TAF_UINT8 serialNum[TAF_SERIAL_NUM_NV_LEN];
} TAF_PH_SerialNum;

typedef struct {
    AT_WCDMA_BAND_SET_UN unWcdmaBand; /* ��¼WƵ����Ϣ */
    VOS_UINT32           rsv;
    AT_GSM_BAND_SET_UN   unGsmBand; /* ��¼GƵ����Ϣ */
} AT_NvWgRfMainBand;

#define AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM 8


typedef struct {
    VOS_UINT8  paraNum;                                        /* ��������������� */
    VOS_UINT8  rsv[3];                                         /* ����λ */
    VOS_INT16  asPower[AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM]; /* GƵ�ι�������ֵ */
    VOS_UINT32 band[AT_NVIM_BODYSARGSM_MAX_PARA_GROUP_NUM];    /* GƵ��λ�� */
} AT_BodysargsmSetPara;


typedef struct {
    VOS_UINT8  paraNum;                                     /* ��������������� */
    VOS_UINT8  rsv[3];                                      /* ����λ */
    VOS_INT16  asPower[AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM]; /* WƵ�ι�������ֵ */
    VOS_UINT32 band[AT_BODYSARWCDMA_MAX_PARA_GROUP_NUM];    /* WƵ��λ�� */
} AT_BodysarwcdmaSetPara;


typedef struct {
    VOS_UINT32 profileId;   /* ����ʹ�ó���������ǰ�����������µ磨ABB��TCXO��RF���Լ�RFͨ���Ŀ��ơ�
                             ��AT�����·����á�Ĭ��ֵΪ0��ȡֵ��Χ0-7�� */
    VOS_UINT32 reserved[3]; /* ������������չʹ�� */
} NAS_NvTriModeFemProfileId;


typedef struct {
    VOS_UINT16 enable; /* ȫ��ͨ���Կ��� */
    VOS_UINT16 reserved;
} NAS_NvTriModeEnable;

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)

typedef struct {
    AT_FreqlockModeTypeUint32 mode;
    VOS_UINT32                reserve;
    VOS_CHAR                 *strMode;
} AT_FreqlockModeTbl;


typedef struct {
    AT_FreqlockBandTypeUint32 band;
    VOS_UINT32                reserve;
    VOS_CHAR                 *strGsmBand;
} AT_FreqlockGsmBandTbl;
#endif


enum AT_IMS_CtrlMsgReceiveModule {
    AT_IMS_CTRL_MSG_RECEIVE_MODULE_NON_IMSA = 0, /* ��IMSҵ�� */
    AT_IMS_CTRL_MSG_RECEIVE_MODULE_IMSA     = 1, /* IMSҵ�� */
    AT_IMS_CTRL_MSG_RECEIVE_MODULE_BUTT
};
typedef VOS_UINT8 AT_IMS_CtrlMsgReceiveModuleUint8;

/* ɾ������AT^EOPLMN�����洢�����Ľṹ��AT_EOPLMN_SET_REQ_STRU */


typedef VOS_UINT8 (*AT_GET_CONNECT_RATE_FUNC_PTR)(VOS_VOID);


typedef struct {
    VOS_UINT32                   subSysMode;
    AT_GET_CONNECT_RATE_FUNC_PTR getConnectRateFunc;
} AT_SubSysModeConnectRatePair;


typedef struct {
    VOS_UINT32 layer;
    VOS_UINT32 total;
    VOS_UINT32 index;
    VOS_UINT16 simLockDataLen;
    VOS_UINT16 hmacLen;
    VOS_UINT8 *simLockData;
    VOS_UINT8 *hmac;
    VOS_UINT8  paraNum;
    VOS_UINT8  reserve[7];
} AT_SimlockWriteExPara;

/* ��Ϣ������ָ�� */
typedef VOS_VOID (*AT_PS_INTERNAL_TIMEOUT_FUNC)(REL_TimerMsgBlock *pMsg);


typedef struct {
    VOS_UINT32                  interTimerName;
    AT_PS_INTERNAL_TIMEOUT_FUNC internalTimeoutFunc; /*lint !e958 !e959 */
} AT_PS_InternalTimeoutResult;


typedef struct {
    VOS_UINT32 digite[AT_S_NSSAI_MAX_DIGIT_NUM + 1];
    VOS_UINT8  punctuation[AT_S_NSSAI_MAX_PUNC_NUM + 1];
    VOS_UINT8  digitParaNum;
    VOS_UINT8  punctuationNum;
    VOS_UINT8  reserve[2];
} AT_SNssaiParse;

extern const AT_PB_ConversionTable g_iraToUnicode[AT_PB_IRA_MAX_NUM];
extern const AT_PB_ConversionTable g_gsmToUnicode[AT_PB_GSM_MAX_NUM];
extern const AT_PB_ConversionTable g_gsm7extToUnicode[AT_PB_GSM7EXT_MAX_NUM];

extern AT_CMD_FORMAT_TYPE        g_atCmdFmtType;
extern pAtChkFuncType            g_atCheckFunc;
extern AT_V_TYPE                 g_atVType; /* ָʾ����������� */
extern AT_CSCS_TYPE              g_atCscsType;
extern AT_ClientManage           g_atClientTab[AT_MAX_CLIENT_NUM];
extern TAF_UINT8                 g_atCrLf[];           /* �س����� */
extern const TAF_UINT8           g_atCmeErrorStr[];    /* ������ʾ�ִ� */
extern const TAF_UINT8           g_atCmsErrorStr[];    /* ������ʾ�ִ� */
extern const TAF_UINT8           g_atTooManyParaStr[]; /* ����̫����ʾ�ִ� */
extern AT_SEND_DataBuffer        g_atSendDataBuff;
extern AT_SEND_DataBuffer        g_atCombineSendData[];
extern const AT_SMS_CmdTab       g_atSmsCmdTab[];
extern AT_CMD_ECHO_TYPE          g_atEType;
extern VOS_UINT32                g_atXType;
extern VOS_UINT32                g_atQType;
extern TAF_UINT8                *g_atSndCrLfAddr;
extern TAF_UINT8                *g_atSndCodeAddress;
extern TAF_UINT8                 g_atSendDataBuffer[];
extern const AT_RETURN_TabType   g_atReturnCodeTab[];
extern const AT_STRING_Type      g_atStringTab[];
extern TAF_UINT8                 g_atPppIndexTab[AT_MAX_CLIENT_NUM];
extern TAF_UINT32                g_pbPrintTag;
extern AT_PB_Info                g_pbatInfo;
extern VOS_UINT8                 g_xsmsRecNum;
extern TAF_LogPrivacyAtCmdUint32 g_logPrivacyAtCmd;

extern const TAF_UINT16     g_atSmsCmdNum;
extern MN_OPERATION_ID_T    g_opId;
extern AT_CSTA_NumTypeUint8 g_atCstaNumType;

extern AT_CombineCmdSplit     g_atComCmdSplit[];
extern AT_CombineCmdParseRslt g_atComCmdParseRslt[];
extern const TAF_UINT16       g_atBasicCmdNum;

extern AT_DataStreamBuffer g_atDataBuff[];

extern AT_SetPortParaMap g_setPortParaMap[AT_SETPORT_DEV_MAP_LEN];

extern VOS_BOOL g_atDataLocked;

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
extern AT_SP_WordCtx g_spWordCtx;
#endif

#if (MULTI_MODEM_NUMBER >= 2)
/* AT��������ε�PID��Ӧ��  */
extern const AT_ModemPidTab g_atModemPidTab[];
#endif

extern VOS_UINT8 *AT_GetCrlf(VOS_VOID);
extern TAF_UINT8 *AT_GetSendDataBuffer(VOS_VOID);
extern AT_SEND_DataBuffer *AT_GetSendDataBuff(VOS_VOID);
extern TAF_UINT8 *AT_GetSndCodeAddress(VOS_VOID);
extern AT_MTA_BodySarStateUint16 g_atBodySarState;

extern VOS_UINT32 At_ProcSimLockPara(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

/* **All the following function are defined in AtQueryParaCmd.c** */

extern TAF_VOID   At_TimeOutProc(REL_TimerMsgBlock *msg);
extern TAF_UINT32 At_ClientIdToUserId(TAF_UINT16 clientId, TAF_UINT8 *indexId);
extern TAF_UINT32 At_ClientIdToUserBroadCastId(TAF_UINT16 clientId, TAF_UINT8 *indexId);
extern TAF_UINT32 At_CheckPbSpecialCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseExtendCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseDCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseSCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseDMCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseBasicCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern VOS_VOID At_SetCmdSubMode(AT_ClientIdUint16 clientId, AT_CmdSubModeUint8 subMode);
extern AT_CmdSubModeUint8 At_GetCmdSubMode(AT_ClientIdUint16 clientId);
extern TAF_UINT32 At_ParsePara(TAF_UINT8 *data, TAF_UINT16 len);

extern VOS_VOID   At_ProcHoldAtCmd(VOS_UINT8 indexNum);
extern TAF_UINT32 At_CheckUsimStatus(TAF_UINT8 *cmdName, VOS_UINT8 indexNum);
extern TAF_UINT32 At_AtioRcvDataInd(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_PbAscii2Gsm7BitTransfer(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_VOID   At_PbGsm7bit2AsciiTransfer(TAF_UINT8 *alphaTag, TAF_UINT8 *alphaTagLen);
extern TAF_UINT32 At_UnicodeTranserToAscii(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_FindMinCodeOffset(const TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_UnicodeTransfer(TAF_UINT8 *data, TAF_UINT16 *len, TAF_UINT32 maxLength);
extern TAF_VOID   At_PbGsmToUnicode(TAF_UINT8 *src, TAF_UINT16 srcLen, TAF_UINT8 *dst, VOS_UINT32 dstBufLen,
                                    TAF_UINT16 *dstLen);
extern TAF_UINT8* At_GetStrContent(TAF_UINT32 type);
extern TAF_UINT32 At_SendRxOnOffToHPA(VOS_UINT32 rxSwitch, VOS_UINT8 indexNum);
extern TAF_UINT32 At_SendTxOnOffToHPA(TAF_UINT8 rxSwitch, TAF_UINT8 indexNum);
extern VOS_UINT32 At_SendRxOnOffToGHPA(VOS_UINT8 indexNum, VOS_UINT32 rxSwitch);
extern VOS_UINT32 At_SendTxOnOffToGHPA(VOS_UINT8 indexNum, VOS_UINT8 switchValue);
extern TAF_UINT32 At_String2Hex(TAF_UINT8 *nptr, TAF_UINT16 len, TAF_UINT32 *rtn);
extern TAF_UINT32 AT_CountDigit(TAF_UINT8 *data, TAF_UINT32 len, TAF_UINT8 Char, TAF_UINT32 indexNum);
extern TAF_UINT32 At_Ascii2UnicodePrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                        TAF_UINT16 srcLen);
extern TAF_UINT16 At_PrintReportData(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 dataCodeType, TAF_UINT8 *dst,
                                     TAF_UINT8 *src, TAF_UINT16 srcLen);
extern TAF_UINT32 At_ChgTafErrorCode(TAF_UINT8 indexNum, TAF_ERROR_CodeUint32 tafErrorCode);
/* Del At_AbortCmdProc */
extern TAF_UINT32 At_CheckCharWcmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_ParseWCmd(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_UINT32 At_Unicode2UnicodePrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                          TAF_UINT16 srcLen);
extern TAF_VOID   At_UsimGsm7bit2AsciiTransfer(TAF_UINT8 *data, TAF_UINT32 *dataLen);
extern TAF_UINT32 At_HexString2AsciiNumPrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                             TAF_UINT16 srcLen);
extern TAF_VOID   At_SendReportMsg(TAF_UINT8 type, TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_SmsMsgProc(MN_AT_IndEvt *data, TAF_UINT16 len);
extern TAF_VOID   At_SetMsgProc(TAF_SetRslt *setRslt);
extern TAF_VOID   At_QryMsgProc(TAF_QryRslt *qryRslt);
extern VOS_VOID   At_PppReleaseIndProc(AT_ClientIdUint16 clientId);
extern TAF_VOID   At_PhMsgProc(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_SsMsgProc(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_DataStatusMsgProc(TAF_DataStatusInd *dataStatus);
extern TAF_UINT32 At_PrintfString(TAF_UINT8 *str1, TAF_UINT8 *str2);
extern TAF_VOID   At_SendDataProc(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_CmdCnfMsgProc(TAF_UINT8 *data, TAF_UINT16 len);
extern TAF_VOID   At_VcMsgProc(MN_AT_IndEvt *data, TAF_UINT16 len);
extern TAF_INT32  AT_FormatReportString(TAF_INT32 maxLength, TAF_CHAR *headaddr, TAF_CHAR *curraddr,
    const TAF_CHAR *fmt, ...);
extern TAF_VOID   At_PIHMsgProc(struct MsgCB *msg);
extern TAF_VOID   At_STKMsgProc(struct MsgCB *msg);
TAF_VOID          At_EMATMsgProc(struct MsgCB *msg);
extern TAF_VOID   At_PbMsgProc(struct MsgCB *msg);
extern TAF_VOID   At_TAFPbMsgProc(TAF_UINT8 *data, TAF_UINT16 len);

/* ************���˿ڳ�ʼ������*************** */
extern VOS_UINT32 At_UsbModemRelInd(VOS_UINT8 portType, VOS_UINT8 dlci);

extern TAF_UINT32 At_HexAlpha2AsciiString(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                          TAF_UINT16 srcLen);

extern TAF_VOID         AT_MapCallTypeModeToClccMode(MN_CALL_TypeUint8 callType, VOS_UINT8 *clccMode);
extern AT_CmdCurrentOpt At_GetMnOptType(TAF_UINT8 type);

extern VOS_UINT32 AT_SetSelRfPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QrySelRfPara(VOS_UINT8 indexNum);
extern TAF_UINT32 At_SetCcnmaPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetCmsqPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetCssmPara(TAF_UINT8 indexNum);

extern TAF_UINT32 At_SetStpdPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetStmnPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetSttrPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetStenPara(TAF_UINT8 indexNum);

extern VOS_UINT32 At_UnlockSimLock(VOS_UINT8 indexNum, VOS_UINT32 paraCnt, AT_ParseParaType *paralist);

extern VOS_UINT32 AT_ConvertCharToHex(VOS_UINT8 charValue, VOS_UINT8 *hexValue);

extern TAF_UINT32 At_SetCmgsSegmentPara(TAF_UINT8 indexNum);

/* ������PDP��׮���� */
extern VOS_UINT32 AT_GetSimLockStatus(VOS_UINT8 indexNum);

extern TAF_UINT32 At_QryCcnmaPara(TAF_UINT8 indexNum);

extern TAF_UINT32 At_QryFrqPara(TAF_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern TAF_UINT32 At_QrySysCfgPara(TAF_UINT8 indexNum);
#endif

extern TAF_UINT32 At_QryDwinsPara(TAF_UINT8 indexNum);

extern TAF_UINT32 At_ReadNumTypePara(TAF_UINT8 *dst, TAF_UINT8 *src);

extern TAF_UINT32 At_ChgMnErrCodeToAt(TAF_UINT8 indexNum, TAF_UINT32 mnErrorCode);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
extern TAF_UINT32 At_ChgXsmsErrorCodeToAt(TAF_UINT32 xsmsError);
#endif

extern TAF_UINT32 At_PrintListMsg(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo,
                                  TAF_UINT8 *dst);

extern TAF_VOID At_PrintCsmsInfo(TAF_UINT8 indexNum);

extern MN_OPERATION_ID_T At_GetOpId(VOS_VOID);

extern TAF_VOID At_HandleSmtBuffer(VOS_UINT8 indexNum, AT_CNMI_BFR_TYPE bfrType);

extern TAF_UINT16 At_PrintAsciiAddr(MN_MSG_AsciiAddr *addr, TAF_UINT8 *dst);

extern TAF_UINT32 At_GetScaFromInputStr(const TAF_UINT8 *addr, MN_MSG_BcdAddr *bcdAddr, TAF_UINT32 *len);

extern MN_MSG_TsDataInfo* At_GetMsgMem(VOS_VOID);

extern TAF_UINT16 At_PrintMsgFo(MN_MSG_TsDataInfo *tsDataInfo, TAF_UINT8 *dst);

extern TAF_UINT16 At_PrintAddrType(MN_MSG_AsciiAddr *addr, TAF_UINT8 *dst);

extern TAF_UINT32 At_GetAsciiOrBcdAddr(TAF_UINT8 *addr, TAF_UINT16 addrLen, TAF_UINT8 addrType, TAF_UINT16 numTypeLen,
                                       MN_MSG_AsciiAddr *asciiAddr, MN_MSG_BcdAddr *bcdAddr);

extern TAF_VOID At_SmsModSmStatusRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_SmsInitResultProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsDeliverErrProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsInitSmspResultProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsSrvParmChangeProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsRcvMsgPathChangeProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsStorageListProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_SmsStorageExceedProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_PrintSetCpmsRsp(TAF_UINT8 indexNum);

extern TAF_VOID At_ZeroReplaceBlankInString(VOS_UINT8 *data, VOS_UINT32 len);

extern TAF_UINT16 At_PrintBcdAddr(MN_MSG_BcdAddr *bcdAddr, TAF_UINT8 *dst);

extern VOS_VOID At_PrintGetCpmsRsp(VOS_UINT8 indexNum);

extern TAF_UINT32 At_SmsPrintScts(const MN_MSG_Timestamp *timeStamp, TAF_UINT8 *dst);

extern TAF_UINT16 At_MsgPrintVp(MN_MSG_ValidPeriod *validPeriod, TAF_UINT8 *dst);

extern TAF_UINT32 At_SmsPrintState(AT_CmgfMsgFormatUint8 smsFormat, MN_MSG_StatusTypeUint8 status, TAF_UINT8 *dst);

extern MN_MSG_SendAckParm* At_GetAckMsgMem(VOS_VOID);

extern TAF_VOID At_MsgResultCodeFormat(TAF_UINT8 indexNum, TAF_UINT16 length);

extern VOS_VOID At_SmsDeliverProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_SetRcvPathRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_SetCscaCsmpRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_DeleteTestRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_ReadRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_ListRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_WriteSmRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_DeleteRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_SetCnmaRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern TAF_VOID At_SendSmRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);


extern TAF_VOID At_SetCmmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);
extern TAF_VOID At_SetCgsmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);
extern TAF_VOID At_GetCmmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);
extern TAF_VOID At_GetCgsmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);
extern VOS_VOID AT_QryCscaRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID   At_SmsStubRspProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event);
extern TAF_VOID   At_SmsRspNop(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);
extern TAF_UINT32 At_MsgDeleteCmdProc(TAF_UINT8 indexNum, MN_OPERATION_ID_T opId, MN_MSG_DeleteParam deleteInfo,
                                      TAF_UINT32 deleteTypes);

extern VOS_VOID At_QryParaRspCopsProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspSysinfoProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

/* begin V7R1 PhaseI Modify */
extern VOS_VOID AT_QryParaRspSysinfoExProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const TAF_UINT8 *para);
/* end V7R1 PhaseI Modify */

extern TAF_VOID At_QryParaRspCimiProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspCgclassProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern VOS_VOID At_QryParaRspCregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const TAF_UINT8 *para);

extern VOS_VOID At_QryParaRspCgregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspIccidProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspPnnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);
extern TAF_VOID At_QryParaRspCPnnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspOplProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspCfplmnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_VOID At_QryParaRspCgdnsProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_VOID *para);

extern TAF_VOID At_QryParaRspAuthdataProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_VOID *para);

extern TAF_VOID At_QryRspUsimRangeProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para);

extern TAF_UINT32 AT_QryNvRststtsPara(VOS_UINT8 indexNum);

extern TAF_UINT32 At_PbReadCnfProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event);

extern TAF_UINT32 At_PbCPBRCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event);

extern TAF_UINT32 At_PbCPBR2CmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event);

extern TAF_UINT32 At_PbCNUMCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event);

extern TAF_UINT32 At_PbSCPBRCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, SI_PB_EventInfo *event);

extern TAF_VOID At_PbSearchCnfProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event);

extern TAF_VOID   At_CombineCmdStruInit(VOS_VOID);
extern TAF_VOID   At_ResetCombineCmdStru(TAF_UINT8 indexNum);
extern TAF_UINT32 At_FormatCmdStr(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_ScanDelChar(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_ScanCtlChar(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_ScanQuoteChar(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_ScanBlankChar(TAF_UINT8 *data, TAF_UINT16 *len);
extern TAF_UINT32 At_ScanSemicolonChar(TAF_UINT8 *data, TAF_UINT16 len, AT_CombineCmdSemicolonParse *comCmdSCParse);
extern TAF_UINT32 At_CheckCharA(TAF_UINT8 Char);
extern TAF_UINT32 At_CheckCharT(TAF_UINT8 Char);
extern VOS_UINT32 At_CmdDataPreProc(VOS_UINT8 *data, VOS_UINT16 *cmdLen);
extern VOS_UINT32 At_FindPendAtChannelOrSmsEditMode(AT_USER_TYPE userType);

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))

#define AT_MAX_CBS_MSGID_NUM 50

extern VOS_VOID At_SmsDeliverCbmProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID At_GetCbActiveMidsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_VOID AT_ChangeCbMidsRsp(TAF_UINT8 indexNum, MN_MSG_EventInfo *event);

#if (FEATURE_ETWS == FEATURE_ON)
VOS_VOID At_ProcDeliverEtwsPrimNotify(VOS_UINT8 indexNum, MN_MSG_EventInfo *event);
#endif /* (FEATURE_ETWS == FEATURE_ON) */

#endif

extern TAF_UINT32 Ppp_ReleasePppReq(TAF_UINT16 pppId);
extern TAF_UINT8  Cca_CurState(VOS_VOID);
extern TAF_VOID   SysCtrl_RFLdoDown(VOS_VOID);

extern TAF_UINT32 At_SetCdtmfsPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_SetCdtmfePara(TAF_UINT8 indexNum);

extern VOS_INT    UdiagValueCheck(VOS_UINT32 diagValue);
extern VOS_UINT32 GetU2diagDefaultValue(VOS_VOID);
extern VOS_VOID   MMA_CardType(VOS_CHAR *pcDest);

/*
 * Ϊ�˹��Linux��̨���β���ʧ�����⣬���ȡ��ܷ�������ܷ����У�����������һ���ж��Ƿ�
 *   ΪLINUX����ϵͳ��API
 */
extern VOS_INT GetLinuxSysType(VOS_VOID);

extern VOS_VOID CheckCurrentModemStatus(VOS_INT status);

extern TAF_VOID AT_StubTriggerAutoReply(VOS_UINT8 indexNum, TAF_UINT8 cfgValue);

extern TAF_VOID AT_StubSaveAutoReplyData(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo);

extern TAF_UINT32 AT_StubSendAutoReplyMsg(VOS_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo);

#if (VOS_WIN32 == VOS_OS_VER)
extern VOS_VOID USIMM_SendSMSDataInd1(VOS_VOID);
extern VOS_VOID USIMM_SendSMSDataInd2(VOS_VOID);
extern VOS_VOID MN_MSG_SetClass0Tailor(MN_MSG_Class0TailorUint8 class0Tailor);
#endif

extern VOS_UINT32 MMA_PhoneGetAllOperNumByLongNameFromOperTbl(TAF_PH_OperatorName *operName, VOS_UINT8 *maxMncLen);
extern VOS_UINT32 MMA_PhoneGetAllOperNumByShortNameFromOperTbl(TAF_PH_OperatorName *operName, VOS_UINT8 *maxMncLen);
extern VOS_VOID   MMA_PhoneGetAllOperInfoByLongNameFromOperTbl(TAF_PH_OperatorName *origOperName,
                                                               TAF_PH_OperatorName *operName);
extern VOS_VOID   MMA_PhoneGetAllOperInfoByShortNameFromOperTbl(TAF_PH_OperatorName *origOperName,
                                                                TAF_PH_OperatorName *operName);

extern VOS_BOOL AT_PH_IsPlmnValid(TAF_PLMN_Id *plmnId);
extern VOS_BOOL MN_PH_IsUsingPlmnSel(VOS_VOID);

/* **All the following function are defined in AtSetParaCmd.c** */
#if (FEATURE_RSTRIGGER_SMS == FEATURE_ON)
/* **All the following function are defined in AtQueryParaCmd.c** */

#endif

/* **All the following function are defined in MnMsgSmsProc.c** */

extern TAF_UINT32 At_PrintMmTimeInfo(VOS_UINT8 indexNum, TAF_AT_CommTime *mmTimeInfo, TAF_UINT8 *dst);

extern TAF_UINT32 At_PrintTimeZoneInfo(NAS_MM_InfoInd *mmInfo, TAF_UINT8 *dst);

VOS_UINT32 AT_SetCclprPara(VOS_UINT8 indexNum);
/*
 * ��������: ��ʾ��ǰʱ����Ϣ(����ʱ����Ϣ)
 */
VOS_UINT32 AT_PrintTimeZoneInfoNoAdjustment(TAF_AT_CommTime *mmInfo, VOS_UINT8 *dst);

VOS_UINT32      AT_PrintTimeZoneInfoWithCtzeType(TAF_AT_CommTime *mmInfo, VOS_UINT8 *dst);
extern TAF_VOID At_PhIndProc(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event);

extern VOS_UINT32 MN_PH_QryApHplmn(VOS_UINT32 *mcc, VOS_UINT32 *mnc);

VOS_VOID AT_GetOnlyGURatOrder(TAF_MMA_MultimodeRatCfg *ratOrder);

TAF_VOID At_ResetGlobalVariable(VOS_VOID);

extern VOS_UINT32 AT_HandleDockSetCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 AT_DockHandleCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_BOOL AT_CheckDockName(VOS_UINT8 *data, VOS_UINT16 len);

VOS_VOID At_SendMsgFoAttr(VOS_UINT8 indexNum, MN_MSG_TsDataInfo *tsDataInfo);

extern VOS_VOID AT_QryParaAnQueryProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para);

extern VOS_VOID AT_QryParaHomePlmnProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para);

VOS_UINT32 AT_RcvSimLockQryRsp(struct MsgCB *msg);

extern VOS_UINT32 OM_AcpuTraceMsgHook(VOS_VOID *msg);

VOS_UINT32 AT_RcvDrvAgentAuthVerQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentGodloadSetRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmGetCdurCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvTafCcmCallSsInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentAppdmverQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentDloadverQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentDloadInfoQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentFlashInfoQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentAuthorityVerQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentAuthorityIdQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentUsimWriteStubSetRsp(VOS_VOID *msg);

VOS_UINT32 AT_RcvDrvAgentPfverQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentHwnatQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentSdloadSetRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentImsiChgQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentInfoRbuSetRsp(struct MsgCB *msg);
#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_RcvDrvAgentInfoRrsSetRsp(struct MsgCB *msg);
#endif

VOS_UINT32 AT_RcvDrvAgentCpnnQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentCpnnTestRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentNvBackupSetRsp(struct MsgCB *msg);

VOS_VOID AT_EventReport(VOS_UINT32 pid, NAS_OM_EventIdUint16 eventId, VOS_UINT8 *para, VOS_UINT32 len);

VOS_UINT32 AT_RcvDrvAgentCpuloadQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentMfreelocksizeQryRsp(struct MsgCB *msg);

VOS_UINT32 AT_RcvDrvAgentMemInfoQryRsp(struct MsgCB *msg);
VOS_UINT32 At_TestTmodePara(VOS_UINT8 indexNum);

VOS_UINT32 AT_ProcTestError(VOS_UINT8 indexNum);

#ifdef FEATURE_AT_NV_WRITE_SUPPORT
extern VOS_UINT32 AT_SetNVWRPartPara(VOS_UINT8 clientId);
#endif


extern VOS_UINT32 atSetBsnPara(VOS_UINT8 clientId);
extern VOS_UINT32 atQryBsnPara(VOS_UINT8 clientId);

extern VOS_UINT32 atSetGpioplPara(VOS_UINT8 clientId);
extern VOS_UINT32 atQryGpioplPara(VOS_UINT8 clientId);

extern VOS_UINT32 atQryRSimPara(VOS_UINT8 clientId);

extern VOS_UINT32 atQrySfeaturePara(VOS_UINT8 clientId);

extern VOS_UINT32 atSetGodLoadCnf(VOS_UINT8 clientId, VOS_VOID *msgBlock);
extern VOS_UINT32 atSetResetCnf(VOS_UINT8 clientId, VOS_VOID *msgBlock);

extern VOS_UINT32 AT_ParseSetAnyStrCmd(VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 AT_HandleAnyStrSetCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 AT_HandleFacAuthPubKeyExCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_VOID AT_HandleIdentifyEndCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
                    const VOS_CHAR *specialCmdName);

extern VOS_VOID AT_HandlePhonePhynumCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
                    const VOS_CHAR *specialCmdName);

extern VOS_UINT32 AT_HandleApModemSpecialCmdProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
                      const VOS_CHAR *specialCmdName);

VOS_UINT32 AT_HandleSimLockDataWriteExCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

VOS_UINT32 AT_HandleSimLockNWDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);
VOS_UINT32 AT_HandleSimLockOtaDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 AT_HandleSimLockDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 At_HandleApModemSpecialCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 At_SmsProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);
extern VOS_VOID   AT_DiscardInvalidCharForSms(TAF_UINT8 *data, TAF_UINT16 *len);

extern VOS_UINT32 At_TestCpasPara(VOS_UINT8 indexNum);

VOS_VOID AT_ClacCmdProc(VOS_VOID);
VOS_VOID At_CovertMsInternalRxDivParaToUserSet2(VOS_UINT16 curBandSwitch, VOS_UINT32 *userDivBandsLow,
                                                VOS_UINT32 *userDivBandsHigh);

extern VOS_UINT32 At_RegisterDeviceCmdTable(VOS_VOID);

extern VOS_VOID At_InitUsimStatus(VOS_VOID);

/* ����L4A��CNF��IND��Ϣ�������Ҷ�Ӧ�������д��� */

/* AT����LTEװ��FTM�ظ���Ϣ��� */

VOS_UINT32 At_TestCgatt(VOS_UINT8 indexNum);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryCcpuMemInfoPara(TAF_UINT8 indexNum);
#endif

extern VOS_UINT32 At_ReadCustomizeServiceNV(NAS_NVIM_CustomizeService *custSrv, VOS_UINT16 *valueInfo);

extern VOS_VOID AT_GetSpecificPort(VOS_UINT8 portType, VOS_UINT8 aucRewindPortStyle[], VOS_UINT32 *portPos,
                                   VOS_UINT32 *portNum);

VOS_VOID AT_SetMemStatusRspProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event);

extern VOS_UINT32 AT_SetChdataPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryChdataPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestChdataPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetHsspt(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryHsspt(VOS_UINT8 indexNum);

extern VOS_VOID AT_SpyMsgProc(struct MsgCB *msg);

extern TAF_UINT32 At_PortSwitchSndMsg(TAF_UINT32 switchMode);

extern VOS_UINT32 atSetTdsParaCnfProc(VOS_UINT16 clientId, VOS_VOID *msgBlock);
extern VOS_UINT32 atQryTdsFRSSIParaCnfProc(VOS_UINT8 clientId, VOS_VOID *msgBlock);

extern VOS_UINT32 atQryTdsTSELRFParaCnfProc(VOS_UINT8 clientId, VOS_VOID *msgBlock);
extern VOS_UINT32 atQryTdsFCHANParaCnfProc(VOS_UINT8 clientId, VOS_VOID *msgBlock);
extern VOS_UINT32 atQryTdsFTXONParaCnfProc(VOS_UINT8 clientId, VOS_VOID *msgBlock);
extern VOS_UINT32 atQryTdsFRXONParaCnfProc(VOS_UINT8 clientId, VOS_VOID *msgBlock);

extern VOS_UINT32 At_ProcXmlText(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len);

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern VOS_UINT32 AT_HandleApSndApduCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);
#endif

VOS_UINT32 At_AbortCopsPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_RcvDrvAgentSwverSetCnf(struct MsgCB *msg);

TAF_CALL_ChannelTypeUint8 AT_ConvertCodecTypeToChannelType(VOS_UINT8              isLocalAlertingFlag,
                                                           MN_CALL_CodecTypeUint8 codecType);

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_VOID AT_RegResetCallbackFunc(VOS_VOID);
#endif

VOS_UINT32 AT_RcvTempprtStatusInd(struct MsgCB *msg);

#if (FEATURE_DSDS == FEATURE_ON)
VOS_UINT32 AT_SetDsdsStatePara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 At_SetSIMSlotPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QrySIMSlotPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_String2Hex(VOS_UINT8 *nptr, VOS_UINT16 len, VOS_UINT32 *rtn);
/* vSIM Project Begin */
extern VOS_UINT32 AT_RcvDrvAgentHvpdhSetCnf(struct MsgCB *msg);
extern TAF_VOID   At_PIHIndProc(TAF_UINT8 indexNum, SI_PIH_EventInfo *event);
/* vSIM Project Begin */

extern VOS_UINT32 AT_RcvNvManufactureExtSetCnf(struct MsgCB *msg);

TAF_VOID   At_EMATIndProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event);
TAF_UINT32 At_EMATNotBroadIndProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event);
TAF_VOID   At_EMATRspProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event);

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_SetImsTimerPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetImsSmsPsiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsTimerPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsSmsPsiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDmUserPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetDmRcsCfgPara(VOS_UINT8 indexNum);
VOS_UINT32        AT_SetRcsSwitch(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryRcsSwitch(VOS_UINT8 indexNum);
#endif

extern VOS_UINT32 AT_RcvDrvAgentQryCcpuMemInfoCnf(struct MsgCB *data);

extern VOS_UINT32 AT_SetCclkPara(VOS_UINT8 indexNum);

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
extern VOS_UINT32 At_SetSilentPin(TAF_UINT8 indexNum);

extern VOS_UINT32 At_SetSilentPinInfo(TAF_UINT8 indexNum);
#endif

VOS_UINT32 At_SetEsimCleanProfile(VOS_UINT8 indexNum);

VOS_UINT32 At_QryEsimCheckProfile(VOS_UINT8 indexNum);

VOS_UINT32 At_QryEsimEid(VOS_UINT8 indexNum);

VOS_UINT32 At_QryEsimPKID(VOS_UINT8 indexNum);

VOS_UINT32 At_SetEsimSwitchPara(VOS_UINT8 indexNum);

VOS_UINT32 At_QryEsimSwitch(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

extern VOS_VOID At_XsmsIndProc(VOS_UINT8 indexNum, TAF_XSMS_AppMsgTypeUint32 eventType, TAF_XSMS_AppAtEventInfo *event);

extern VOS_VOID At_XsmsCnfProc(VOS_UINT8 indexNum, TAF_XSMS_AppMsgTypeUint32 eventType, TAF_XSMS_AppAtEventInfo *event);

extern VOS_VOID AT_ProcXsmsMsg(struct MsgCB *msg);

extern VOS_VOID AT_ProcXpdsMsg(struct MsgCB *msg);

extern VOS_UINT32 AT_HandleApXsmsSndCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

extern VOS_UINT32 AT_HandleApXsmsWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len);

/* �Ƶ� FEATURE_UE_MODE_CDMA ���� */

extern VOS_UINT32 AT_SetCBurstDTMFPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCfshPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_CheckCfshNumber(VOS_UINT8 *atPara, VOS_UINT16 len);

#endif

extern VOS_UINT32 AT_TestUECenterPara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
TAF_UINT32 At_CheckCurrRatModeIsCL(VOS_UINT8 indexNum);
#endif
#if ((FEATURE_LTEV == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern TAF_UINT32 AT_SetSourceId(VOS_UINT8 indexNum);
extern TAF_UINT32 AT_QrySourceId(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_SetCtzuPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCtzuPara(VOS_UINT8 indexNum);


extern VOS_UINT32 At_TestFclass(VOS_UINT8 indexNum);
extern VOS_UINT32 At_TestGci(VOS_UINT8 indexNum);

VOS_UINT32 At_ProcSpecificReturnCode(VOS_UINT8 indexNum, VOS_UINT32 returnCode);

VOS_UINT32 AT_ConvertMtaResult(MTA_AT_ResultUint32 result);

extern const AT_ParCmdElement* At_GetBasicCmdTable(VOS_VOID);


VOS_VOID AT_RcvTiS0Expired(REL_TimerMsgBlock *msg);
VOS_VOID AT_RcvTiShutDownExpired(REL_TimerMsgBlock *tmrMsg);

extern VOS_VOID AT_QryPlmnIdRspProc(VOS_UINT8 indexNum, VOS_UINT8 opId, VOS_VOID *para);
extern VOS_VOID AT_GetLiveTime(NAS_MM_InfoInd *aTtime, NAS_MM_InfoInd *newTime, VOS_UINT32 nwSecond);

extern VOS_UINT32 AT_GetSeconds(VOS_VOID);

#if (VOS_WIN32 == VOS_OS_VER) && (LLT_OS_VER == LLT_WIN)
extern VOS_INT32 Sock_Send(VOS_UINT8 portNo, VOS_UINT8 *data, VOS_UINT16 uslength);
#endif

VOS_UINT32 At_TestC5gqos(VOS_UINT8 indexNum);

VOS_UINT32 At_TestC5gQosRdp(VOS_UINT8 indexNum);

VOS_UINT32 At_TestC5gPNssai(VOS_UINT8 indexNum);

VOS_UINT32 At_TestC5gNssai(VOS_UINT8 indexNum);

VOS_UINT32 At_TestC5gNssaiRdp(VOS_UINT8 indexNum);

#if (FEATURE_LTEV == FEATURE_ON)
extern VOS_UINT32 AT_SetSideLinkInfo(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QrySideLinkInfo(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetPhyr(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryPhyr(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestPhyr(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetPtrRpt(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryPtrRpt(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetV2xResPoolPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryV2xResPoolPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestRPPCfg(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetGnssInfo(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryData(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestQryData(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_GnssInfo(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetLedTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryVPhyStat(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetVPhyStatClr(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryVSnrRsrp(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryV2xRssi(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryVRssi(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestVRssi(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SendData(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetVTxPower(VOS_UINT8 indexNum);
extern VOS_UINT32 At_RegisterLtevCmdTable(VOS_VOID);
extern TAF_UINT32 At_AsciiNum2Uint8(TAF_UINT8 *src, TAF_UINT16 *srcLen);
#endif

#if (FEATURE_LTEV_WL == FEATURE_ON)
extern VOS_UINT32 AT_SetPc5Sync(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryPc5Sync(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetRsuMcgCfg(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_WriteFile(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_ReadFile(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvTiLedTestExpired(REL_TimerMsgBlock *msg);
extern VOS_VOID   AT_RcvTiSfpTestExpired(REL_TimerMsgBlock *msg);
extern VOS_UINT32 At_QryRsuTempPara(TAF_UINT8 indexNum);
extern VOS_UINT32 AT_SetGpioTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_AntTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_GeTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SfpTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QrySfpTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SfpSwitch(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetGpioHeatSet(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_HkadcTest(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryGpio(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetGpio(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetBootMode(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetMultiupgMode(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryMultiupgMode(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QrySyncSt(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_TestSyncSt(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryGnssSyncSt(VOS_UINT8 indexNum);
#endif


extern VOS_VOID AT_ProcReportSimSqInfo(VOS_UINT8 indexNum, TAF_MMA_SimsqStateUint32 simsq);
extern DMS_PortIdUint16 AT_GetDmsPortIdByClientId(AT_ClientIdUint16 clientId);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_VOID At_ProcUePolicyInfoStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt);
VOS_VOID At_ProcImsUrspStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt);
#endif

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
extern TAF_UINT32 At_SetCmuxPara(TAF_UINT8 indexNum);
extern TAF_UINT32 At_QryCmuxPara(TAF_UINT8 indexNum);
VOS_UINT32        At_TestCmuxPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
VOS_VOID AtNlProxyDealwithUspaceData(VOS_VOID* buffer, VOS_INT32 ilength);
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON)
VOS_UINT32 AT_SetVoipApnKeyPara(VOS_UINT8 index);
VOS_UINT32 AT_QryVoipApnKeyPara(VOS_UINT8 index);
VOS_UINT32 AT_TestVoipApnKeyPara(VOS_UINT8 index);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _AT_CMD_PROC_H_ */
