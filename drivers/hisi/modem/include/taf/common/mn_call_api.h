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
#ifndef MN_CALL_API_H
#define MN_CALL_API_H

#include "vos.h"
#include "taf_type_def.h"
#include "taf_app_ssa.h"
#include "at_mn_interface.h"
#include "product_config.h"
#include "taf_call_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ��ѡ���Ƿ���ڵı�־ */
#define MN_CALL_OPTION_EXIST (1)
#define MN_CALL_OPTION_NOT_EXIST (0)

#define MN_CALL_MAX_EMC_NUM (80)
#define MN_CALL_ECC_NUM_VALID_SIM_PRESENT (1)
#define MN_CALL_ECC_NUM_VALID_SIM_ABSENT (0)
#define MN_CALL_ECC_NUM_INVALID_CATEGORY (0)

#define MN_CALL_RPT_CFG_MAX_SIZE (8)
#define MN_CALL_NET_RAT_TYPE_WCDMA (0) /* ��ǰפ��ΪW */
#define MN_CALL_NET_RAT_TYPE_GSM (1)   /* ��ǰפ��ΪG */
#define MN_CALL_NET_RAT_TYPE_LTE (2)   /* ��ǰפ��ģΪL */

#define TAF_CALL_DTMF_MIN_ONLENGTH (65)        /* DTMF����Сʱ�� */
#define TAF_CALL_DTMF_DEFAULT_ONLENGTH (60000) /* Ĭ��DTMF��ʱ��60s */

/* stop dtmf ack��start dtmf req֮��ʱ��������70����С��600����,Ĭ��75���� */
#define TAF_CALL_DTMF_DEFAULT_OFFLENGTH (75)

#define TAF_CALL_MAX_BC_NUM (2) /* ���BC���� */
#define TAF_CALL_MIN_BC_NUM (1) /* ��СBC���� */

#if (FEATURE_ON == FEATURE_ECALL)
#define TAF_ECALL_REDIAL_PERIOD_TIMER_LENGTH (120000) /* 120s */
#define TAF_ECALL_REDIAL_INTERVAL_TIMER_LENGTH (5000) /* 5s */
#define TAF_CALL_T2_TIMER_LENGTH (3600000)            /* ʱ��Ϊ1Сʱ */
/* T9��ʱ��ʱ�������ã�Ĭ������Ϊ1Сʱ��������ʱ����Χ: 1 hour<= T9 <= 12 hours */
#define TAF_CALL_ONE_HOUR_TIMER_LENGTH (3600000)
#define TAF_CALL_T9_MIN_TIMER_LENGTH (1)  /* T9��Сʱ��Ϊ1Сʱ */
#define TAF_CALL_T9_MAX_TIMER_LENGTH (12) /* T9���ʱ��Ϊ12��Ϣ */
#endif

#define TAF_CALL_MAX_NUM_DIGIT_LEN (64)
#define TAF_CALL_MAX_FLASH_DIGIT_LEN (32)
#define TAF_CALL_MAX_BURST_DTMF_NUM (255)

#define TAF_XCALL_MAX_NUM (2)
#define TAF_CALL_MAX_DISPALY_CHARI_OCTET_NUM (64)
#define TAF_CALL_MAX_CALLED_NUMBER_CHARI_OCTET_NUM (64)
#define TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM (64)
#define TAF_CALL_MAX_CONNECTED_NUMBER_CHARI_OCTET_NUM (64)
#define TAF_CALL_MAX_REDIRECTING_NUMBER_CHARI_OCTET_NUM (64)
#define TAF_CALL_MAX_EXT_DISPLAY_DATA_NUM (3)
#define TAF_CALL_MAX_EXTENDED_DISPALY_CHARI_OCTET_NUM (64)

#define MN_CALL_EMER_CATEGORG_VAL_MAX_SUSPORT_ECALL                                                           \
    (((VOS_UINT8)MN_CALL_EMER_CATEGORG_POLICE) | ((VOS_UINT8)MN_CALL_EMER_CATEGORG_AMBULANCE) |               \
     ((VOS_UINT8)MN_CALL_EMER_CATEGORG_FIRE_BRIGADE) | ((VOS_UINT8)MN_CALL_EMER_CATEGORG_MARINE_GUARD) |      \
     ((VOS_UINT8)MN_CALL_EMER_CATEGORG_MOUNTAIN_RESCUE) | ((VOS_UINT8)MN_CALL_EMER_CATEGORG_MAN_INIT_ECALL) | \
     ((VOS_UINT8)MN_CALL_EMER_CATEGORG_AUTO_INIT_ECALL))

#define MN_CALL_EMER_CATEGORG_VAL_MAX_NOT_SUSPORT_ECALL                                                  \
    (((VOS_UINT8)MN_CALL_EMER_CATEGORG_POLICE) | ((VOS_UINT8)MN_CALL_EMER_CATEGORG_AMBULANCE) |          \
     ((VOS_UINT8)MN_CALL_EMER_CATEGORG_FIRE_BRIGADE) | ((VOS_UINT8)MN_CALL_EMER_CATEGORG_MARINE_GUARD) | \
     ((VOS_UINT8)MN_CALL_EMER_CATEGORG_MOUNTAIN_RESCUE))

#define TAF_CALL_APP_EC_RANDOM_NUM (32)
#define TAF_CALL_APP_EC_RANDOM_DATA_LENGTH (33)
#define TAF_CALL_ECC_KMC_PUBLIC_KEY_MAX_HEX_LENGTH (48)
#define TAF_CALL_ECC_KMC_PUBLIC_KEY_MAX_ASCII_LENGTH (96)
#define TAF_CALL_APP_ENCRYPTED_VOICE_DATA_MAX_LENGTH (1024)
#define TAF_CALL_APP_EC_RANDOM_MAX_GROUP (8)

#define TAF_CALL_EMC_CATEGORY_MAX_VALID_BIT_CNT (7)

#define TAF_CALL_CNAP_NAME_STR_MAX_LENGTH (161) /* CNAP���������ֶ���󳤶� */

#define TAF_APDS_CALL_PULL_DIALOG_ID (255)

#define MN_CALL_BC_MAX_LEN 14

/*
 * ö����    : NAS_CC_StateInfo
 * �ṹ˵��  : CC�ڲ�״̬�ṹ
 */
/* Call state value (octet 2) */
enum NAS_CC_CallState
/* Bits 6 5 4 3 2 1 */
{
    /* 0 0 0 0 0 0 UO - null               */
    NAS_CC_CALL_STATE_U0 = 0,
    /* 0 0 0 0 1 0 U0.1- MM con pending    */
    NAS_CC_CALL_STATE_U0_1 = 2,
    /* 1 0 0 0 1 0 U0.2- CC prompt present */
    NAS_CC_CALL_STATE_U0_2 = 34,
    /* 1 0 0 0 1 1 U0.3- Wait for network  */
    NAS_CC_CALL_STATE_U0_3 = 35,
    /* 1 0 0 1 0 0 U0.4- CC-Est present    */
    NAS_CC_CALL_STATE_U0_4 = 36,
    /* 1 0 0 1 0 1 U0.5- CC-Est confirmed  */
    NAS_CC_CALL_STATE_U0_5 = 37,
    /* 1 0 0 1 1 0 U0.6- Recall present    */
    NAS_CC_CALL_STATE_U0_6 = 38,
    /* 0 0 0 0 0 1 U1 - call initiated     */
    NAS_CC_CALL_STATE_U1 = 1,
    /* 0 0 0 0 1 1 U3 - MO call proc       */
    NAS_CC_CALL_STATE_U3 = 3,
    /* 0 0 0 1 0 0 U4 - call delivered     */
    NAS_CC_CALL_STATE_U4 = 4,
    /* 0 0 0 1 1 0 U6 - call present       */
    NAS_CC_CALL_STATE_U6 = 6,
    /* 0 0 0 1 1 1 U7 - call received      */
    NAS_CC_CALL_STATE_U7 = 7,
    /* 0 0 1 0 0 0 U8 - connect request    */
    NAS_CC_CALL_STATE_U8 = 8,
    /* 0 0 1 0 0 1 U9 - MT call Cnf        */
    NAS_CC_CALL_STATE_U9 = 9,
    /* 0 0 1 0 1 0 U10- active             */
    NAS_CC_CALL_STATE_U10 = 10,
    /* 0 0 1 0 1 1 U11- disconnect REQ     */
    NAS_CC_CALL_STATE_U11 = 11,
    /* 0 0 1 1 0 0 U12- disconnect IND     */
    NAS_CC_CALL_STATE_U12 = 12,
    /* 0 1 0 0 1 1 U19- release REQ        */
    NAS_CC_CALL_STATE_U19 = 19,
    /* 0 1 1 0 1 0 U26- MO modify          */
    NAS_CC_CALL_STATE_U26 = 26,
    /* 0 1 1 0 1 1 U27- MT modify          */
    NAS_CC_CALL_STATE_U27 = 27,
    NAS_CC_CALL_STATE_BUTT
};
typedef VOS_UINT8 NAS_CC_CallStateUint8;


enum NAS_NW_CcState { /* Call state value (octet 2) */ /* Bits 6 5 4 3 2 1 */
    NAS_NW_CC_CALL_STATE_N0   = 0,  /* 0 0 0 0 0 0 NO - null            */
    NAS_NW_CC_CALL_STATE_N0_1 = 2,  /* 0 0 0 0 1 0 N0.1- MM con pending */
    NAS_NW_CC_CALL_STATE_N0_2 = 34, /* 1 0 0 0 1 0 N0.2- CC con pending */
    NAS_NW_CC_CALL_STATE_N0_3 = 35, /* 1 0 0 0 1 1 N0.3- network answer pending */
    NAS_NW_CC_CALL_STATE_N0_4 = 36, /* 1 0 0 1 0 0 N0.4- CC-Est present   */
    NAS_NW_CC_CALL_STATE_N0_5 = 37, /* 1 0 0 1 0 1 N0.5- CC-Est confirmed */
    NAS_NW_CC_CALL_STATE_N0_6 = 38, /* 1 0 0 1 1 0 N0.6- Recall present   */
    NAS_NW_CC_CALL_STATE_N1   = 1,  /* 0 0 0 0 0 1 N1 - call initiated    */
    NAS_NW_CC_CALL_STATE_N3   = 3,  /* 0 0 0 0 1 1 N3 - MO call proc      */
    NAS_NW_CC_CALL_STATE_N4   = 4,  /* 0 0 0 1 0 0 N4 - call delivered    */
    NAS_NW_CC_CALL_STATE_N6   = 6,  /* 0 0 0 1 1 0 N6 - call present      */
    NAS_NW_CC_CALL_STATE_N7   = 7,  /* 0 0 0 1 1 1 N7 - call received     */
    NAS_NW_CC_CALL_STATE_N8   = 8,  /* 0 0 1 0 0 0 N8 - connect request   */
    NAS_NW_CC_CALL_STATE_N9   = 9,  /* 0 0 1 0 0 1 N9 - MT call Cnf       */
    NAS_NW_CC_CALL_STATE_N10  = 10, /* 0 0 1 0 1 0 N10- active            */
    NAS_NW_CC_CALL_STATE_N12  = 12, /* 0 0 1 1 0 0 N12- disconnect IND    */
    NAS_NW_CC_CALL_STATE_N19  = 19, /* 0 1 0 0 1 1 N19- release REQ       */
    NAS_NW_CC_CALL_STATE_N26  = 26, /* 0 1 1 0 1 0 N26- MO modify         */
    NAS_NW_CC_CALL_STATE_N27  = 27, /* 0 1 1 0 1 1 N27- MT modify         */
    NAS_NW_CC_CALL_STATE_N28  = 28, /* 0 1 1 0 1 1 N28- connect IND       */
    NAS_NW_CC_CALL_STATE_BUTT
};
typedef VOS_UINT8 NAS_NW_CcStateUint8;

/* Table 10.5.100/3GPP TS 24.008: Auxiliary states information element */
enum NAS_CC_HoldAuxState { /* Hold aux state (octet 3) */ /* Bits 4 3 */
    NAS_CC_HOLD_AUX_S_IDLE         = 0,
    NAS_CC_HOLD_AUX_S_HOLD_REQ     = 1,
    NAS_CC_HOLD_AUX_S_CALL_HELD    = 2,
    NAS_CC_HOLD_AUX_S_RETRIEVE_REQ = 3,
    NAS_CC_HOLD_AUX_S_BUTT
}; /* defined in 24.083 [27]. */
typedef VOS_UINT8 NAS_CC_HoldAuxStateUint8;

/* Table 10.5.101/3GPP TS 24.008: Auxiliary states information element */
enum NAS_CC_MptyAuxState { /* Multi party aux state (octet 3) */ /* Bits 2 1 */
    NAS_CC_MPTY_AUX_S_IDLE         = 0,
    NAS_CC_MPTY_AUX_S_MPTY_REQ     = 1,
    NAS_CC_MPTY_AUX_S_CALL_IN_MPTY = 2,
    NAS_CC_MPTY_AUX_S_SPLIT_REQ    = 3,
    NAS_CC_MPTY_AUX_S_BUTT
}; /* defined in 24.084 [28]. */
typedef VOS_UINT8 NAS_CC_MptyAuxStateUint8;


enum NAS_CC_CALL_TYPE {
    NAS_CC_CALL_TYPE_MO_NORMAL_CALL, /* ���������� */
    NAS_CC_CALL_TYPE_EMERGENCY_CALL, /* ���н����� */
    NAS_CC_CALL_TYPE_MT_NORMAL_CALL, /* ������ͨ�� */
    NAS_CC_CALL_TYPE_BUTT
};
typedef VOS_UINT8 NAS_CC_CallTypeUint8;


enum TAF_CALL_PrivacyMode {
    TAF_CALL_PRIVACY_MODE_PUBLIC = 0x00,

    TAF_CALL_PRIVACY_MODE_PRIVATE = 0x01,

    TAF_CALL_PRIVACY_MODE_BUTT = 0x02
};
typedef VOS_UINT8 TAF_CALL_PrivacyModeUint8;

/*
 * Description:
 */
typedef struct {
    VOS_UINT8                 callId;
    TAF_CALL_PrivacyModeUint8 privacyMode;
    VOS_UINT8                 reserved[2];
} TAF_CALL_CallPrivacyModeInfo;

/*
 * Description:
 */
typedef struct {
    TAF_CALL_PrivacyModeUint8 privacyMode;
    VOS_UINT8                 reserved[3];
} TAF_CALL_PrivacyModeSetPara;


typedef struct {
    VOS_UINT8                callId;       /* ����ID */
    VOS_UINT8                ti;           /* ����ID */
    NAS_CC_CallStateUint8    callState;    /* ����Э��״̬ */
    NAS_CC_HoldAuxStateUint8 holdAuxState; /* ���б��ָ���״̬ */
    NAS_CC_MptyAuxStateUint8 mptyAuxState; /* �෽���и���״̬ */
    VOS_UINT8                reserve1[3];
} NAS_CC_StateInfo;

/*
 * ö����: AT_CC_STATE_QRY_CNF_STRU
 * �ṹ˵��: CC�ظ�AT״̬��ѯ�ظ��ӿ�
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32       msgName; /* ��Ϣ�� */
    AT_APPCTRL       atAppCtrl;
    VOS_UINT8        callNum; /* ���и��� */
    VOS_UINT8        callExist[MN_CALL_MAX_NUM];
    NAS_CC_StateInfo ccStateInfoList[MN_CALL_MAX_NUM]; /* ��Ϣ���� */
} AT_CC_StateQryCnfMsg;


enum TAF_CALL_SubState {
    TAF_CALL_SUB_STATE_NULL,
    /* ata�������ϱ�ok��������connect ack��״̬ */
    TAF_CALL_SUB_STATE_INCOMING_WAIT_CONNECT_ACK,
    TAF_CALL_SUB_STATE_DISCONNECT_WAIT_EVT_REL,
    TAF_CALL_SUB_STATE_BUTT
};
typedef VOS_UINT8 TAF_CALL_SubStateUint8;

/* Call Control Protocol Cause Value */
enum MN_CALL_CcCause {
    /*   0. invalid cause value                        */
    MN_CALL_INVALID_CAUSE = 0,
    /*   1. Unassigned (unallocated) number            */
    MN_CALL_UNASSIGNED_CAUSE = 1,
    /*   3. No route to destination                    */
    MN_CALL_NO_ROUTE_TO_DEST = 3,
    /*   6. Channel unacceptable                       */
    MN_CALL_CHANNEL_UNACCEPTABLE = 6,
    /*   8. Operator determined barring                */
    MN_CALL_OPERATOR_DETERMINED_BARRING = 8,
    /*  16. Normal call clearing                       */
    MN_CALL_NORMAL_CALL_CLEARING = 16,
    /*  17. User busy                                  */
    MN_CALL_USER_BUSY = 17,
    /*  18. No user responding                         */
    MN_CALL_NO_USER_RESPONDING = 18,
    /*  19. User alerting, no answer                   */
    MN_CALL_USER_ALERTING_NO_ANSWER = 19,
    /*  21. Call rejected                              */
    MN_CALL_CALL_REJECTED = 21,
    /*  22. Number changed                             */
    MN_CALL_NUMBER_CHANGED = 22,
    /*  24. Call rejected due to feature at the destination */
    MN_CALL_CALL_REJECTED_DUE_TO_FEATURE_AT_THE_DESTINATION = 24,
    /*  25. Pre-emption                                */
    MN_CALL_PRE_EMPTION = 25,
    /*  26. Non selected user clearing                 */
    MN_CALL_NON_SELECTED_USER_CLEARING = 26,
    /*  27. Destination out of order                   */
    MN_CALL_DESTINATION_OUT_OF_ORDER = 27,
    /*  28. Invalid number format                      */
    MN_CALL_INVALID_NUMBER_FORMAT = 28,
    /*  29. Facility rejected                          */
    MN_CALL_FACILITY_REJECTED = 29,
    /*  30. Response to STATUS ENQUIRY                 */
    MN_CALL_RESPONSE_TO_STATUS_ENQUIRY = 30,
    /*  31. Normal, unspecified                        */
    MN_CALL_NORMAL_UNSPECIFIED = 31,
    /*  34. No circuit/channel available               */
    MN_CALL_NO_CIRCUIT_CHANNEL_AVAILABLE = 34,
    /*  38. Network out of order                       */
    MN_CALL_NETWORK_OUT_OF_ORDER = 38,
    /*  41. Temporary failure                          */
    MN_CALL_TEMPORARY_FAILURE = 41,
    /*  42. Switching equipment congestion             */
    MN_CALL_SWITCHING_EQUIPMENT_CONGESTION = 42,
    /*  43. Access information discarded               */
    MN_CALL_ACCESS_INFORMATION_DISCARDED = 43,
    /*  44. requested circuit/channel not available    */
    MN_CALL_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE = 44,
    /*  47. Resources unavailable, unspecified         */
    MN_CALL_RESOURCES_UNAVAILABLE_UNSPECIFIED = 47,
    /*  49. Quality of service unavailable             */
    MN_CALL_QUALITY_OF_SERVICE_UNAVAILABLE = 49,
    /*  50. Requested facility not subscribed          */
    MN_CALL_REQUESTED_FACILITY_NOT_SUBSCRIBED = 50,
    /*  55. Incoming calls barred within the CUG       */
    MN_CALL_INCOMING_CALL_BARRED_WITHIN_CUG = 55,
    /*  57. Bearer capability not authorized           */
    MN_CALL_BEARER_CAPABILITY_NOT_AUTHORISED = 57,
    /*  58. Bearer capability not presently available  */
    MN_CALL_BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE = 58,
    /*  63. Service or option not available, unspec    */
    MN_CALL_SERVICE_OR_OPTION_NOT_AVAILABLE = 63,
    /*  65. Bearer service not implemented             */
    MN_CALL_BEARER_SERVICE_NOT_IMPLEMENTED = 65,
    /*  68. ACM equal to or greater than ACMmax        */
    MN_CALL_ACM_GEQ_ACMMAX = 68,
    /*  69. Requested facility not implemented         */
    MN_CALL_REQUESTED_FACILITY_NOT_IMPLEMENTED = 69,
    /*  70. Only restricted digital info BC available  */
    MN_CALL_ONLY_RESTRICTED_DIGITAL_INFO_BC_AVAILABLE = 70,
    /*  79. Service or option not implemented, unspec  */
    MN_CALL_SERVICE_OR_OPTION_NOT_IMPLEMENTED = 79,
    /*  81. Invalid transaction identifier value       */
    MN_CALL_INVALID_TRANSACTION_ID_VALUE = 81,
    /*  87. User not member of CUG                     */
    MN_CALL_USER_NOT_MEMBER_OF_CUG = 87,
    /*  88. Incompatible destination Incompatible para */
    MN_CALL_INCOMPATIBLE_DESTINATION = 88,
    /*  91. Invalid transit network selection          */
    MN_CALL_INVALID_TRANSIT_NETWORK_SELECTION = 91,
    /*  95. Semantically incorrect message             */
    MN_CALL_SEMANTICALLY_INCORRECT_MESSAGE = 95,
    /*  96. Invalid mandatory information              */
    MN_CALL_INVALID_MANDATORY_INFORMATION = 96,
    /*  97. Msg type non-existent or not implemented   */
    MN_CALL_MESSAGE_TYPE_NON_EXISTENT = 97,
    /*  98. Msg type not compatible with protocol state */
    MN_CALL_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STATE = 98,
    /*  99. IE non-existent or not implemented         */
    MN_CALL_IE_NON_EXISTENT_OR_NOT_IMPLEMENTED = 99,
    /* 100. Conditional IE error                       */
    MN_CALL_CONDITIONAL_IE_ERROR = 100,
    /* 101. Message not compatible with protocol state */
    MN_CALL_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 101,
    /* 102. Recovery on timer expiry Timer number      */
    MN_CALL_RECOVERY_ON_TIMER_EXPIRY = 102,
    /* 111. Protocol error, unspecified                */
    MN_CALL_PROTOCOL_ERROR_UNSPECIFIED = 111,
    /* 127. Interworking, unspecified                  */
    MN_CALL_INTERWORKING_UNSPECIFIED = 127,
    MN_CALL_CC_CAUSE_BUTT
};
typedef VOS_UINT8 MN_CALL_CcCauseUint8;

#define TAF_ECC_CALL_MAX_BCD_NUM_LEN 6
typedef struct {
    MN_CALL_NumTypeUint8 numType;
    VOS_UINT8            numLen;
    VOS_UINT8            bcdNum[TAF_ECC_CALL_MAX_BCD_NUM_LEN];
} TAF_ECC_CallBcdNum;

/* ����CSSN������,�����ȥ���� */
enum MN_CALL_SetCssnType {
    MN_CALL_SET_CSSN_DEACT,
    MN_CALL_SET_CSSN_ACT,
    MN_CALL_SET_CSSN_BUTT
};
typedef VOS_UINT32 MN_CALL_SetCssnTypeUint32;

enum MN_CALL_Cuus1Flg {
    MN_CALL_CUUS1_DISABLE,
    MN_CALL_CUUS1_ENABLE,
    MN_CALL_CUUS1_BUTT
};
typedef VOS_UINT32 MN_CALL_Cuus1FlgUint32;

/* ����UUS1������,�����ȥ���� */
enum MN_CALL_SetUus1Type {
    MN_CALL_SET_UUS1_ACT,
    MN_CALL_SET_UUS1_DEACT,
    MN_CALL_SET_UUS1_BUTT
};
typedef VOS_UINT32 MN_CALL_SetUus1TypeUint32;

/*
 *  ���û��ϱ�UUS1����Ϣ����,�������м�����Ϣ,�������ANY,��Ĭ��Ϊ
 * ����SETUP,PROGRESS��Ϣֻ���ϱ�,��������
 */
enum MN_CALL_Uus1MsgType {
    MN_CALL_UUS1_MSG_ANY,
    MN_CALL_UUS1_MSG_SETUP,
    MN_CALL_UUS1_MSG_ALERT,
    MN_CALL_UUS1_MSG_CONNECT,
    MN_CALL_UUS1_MSG_DISCONNECT,
    MN_CALL_UUS1_MSG_RELEASE,
    MN_CALL_UUS1_MSG_RELEASE_COMPLETE,
    MN_CALL_UUS1_MSG_PROGRESS,
    MN_CALL_UUS1_MSG_BUTT
};
typedef VOS_UINT32 MN_CALL_Uus1MsgTypeUint32;

#define MN_CALL_MAX_UUS1_MSG_NUM (7)
#define MN_CALL_MAX_CSSN_MSG_NUM (2)

/*
 * The user-user is a type 4 information element with a minimum length of 3 octets
 * and a maximum length of either 35 or 131 octets. In the SETUP message the
 * user-user information element has a maximum size of 35 octets in a
 * GSM PLMN. In the USER INFORMATION, ALERTING, CONNECT, DISCONNECT,
 * PROGRESS, RELEASE and RELEASE COMPLETE messages the user-user information
 * element has a maximum size of 131 octets in a GSM PLMN.
 */

#define MN_CALL_MIN_UUIE_LEN (3)
#define MN_CALL_MAX_UUIE_LEN (131)
#define MN_CALL_MAX_SETUP_UUIE_CONTEND_LEN (33)
#define MN_CALL_MAX_OTHER_UUIE_CONTEND_LEN (129)
#define MN_CALL_MIN_UUIE_CONTEND_LEN (1)
#define MN_CALL_UUIE_HEADER_LEN (2)

/* �˴���UUIE������Э���й涨��UUIE����,������������ */
/*
 * |  8 |  7  | 6 |  5 |  4 |  3 |  2 |  1   |
 * |    |     User-user IEI                  | octet 1
 * |         Length of user-user contents    | octet 2                           |
 * |      User-user protocol discriminator   | octet 3
 * |     User-user information               | octet 4*
 * |       ...............                   |
 * |       ...............                   | octet N*
 */
#define MN_CALL_UUS_IEI (0x7E)
#define MN_CALL_IEI_POS (0x00)
#define MN_CALL_LEN_POS (0x01)
#define MN_CALL_PD_POS (0x02)
#define MN_CALL_CONTENT_POS (0x03)

typedef struct {
    MN_CALL_Uus1MsgTypeUint32 msgType;
    VOS_UINT8                 uuie[MN_CALL_MAX_UUIE_LEN];
    VOS_UINT8                 reserved1;
} MN_CALL_Uus1Info;

/* Call Event */
enum MN_CALL_Event {
    MN_CALL_EVT_ORIG      = 0x00, /* originate a MO Call */
    MN_CALL_EVT_CALL_PROC = 0x01, /* Call is Proceeding */
    MN_CALL_EVT_ALERTING  = 0x02, /* Alerting,MO Call */
    MN_CALL_EVT_CONNECT   = 0x03, /* Call Connect */
    MN_CALL_EVT_RELEASED  = 0x04, /* Call Released */
    MN_CALL_EVT_INCOMING  = 0x05, /* Incoming Call */
    /* Call Related Command is in progress */
    MN_CALL_EVT_SS_CMD_PROGRESS = 0x06,
    /* Call Related Command is Completed */
    MN_CALL_EVT_SS_CMD_RSLT       = 0x07,
    MN_CALL_EVT_SS_NOTIFY         = 0x08, /* SS Notify */
    MN_CALL_EVT_START_DTMF_CNF    = 0x09, /* Start DTMF tempooralily response */
    MN_CALL_EVT_STOP_DTMF_CNF     = 0x0b, /* Stop DTMF tempooralily response */
    MN_CALL_EVT_CCBS_POSSIBLE     = 0x0d, /* ���Լ���CCBS */
    MN_CALL_EVT_TIME_EXPIRED      = 0x0e, /* ��ʱ */
    MN_CALL_EVT_UUS1_INFO         = 0x0f, /* UUS1��Ϣ�ϱ� */
    MN_CALL_EVT_GET_CDUR_CNF      = 0x10, /* ͨ��ʱ�� */
    MN_CALL_EVT_CLCC_INFO         = 0x11, /* ��ǰ���к��е���Ϣ */
    MN_CALL_EVT_ALL_RELEASED      = 0x12, /* Call All Released */
    MN_CALL_EVT_SET_UUS1_INFO_CNF = 0x13, /* �����û���������1��Ϣ */
    MN_CALL_EVT_QRY_UUS1_INFO_CNF = 0x14, /* ��ѯ�û���������1��Ϣ */
    MN_CALL_EVT_SET_ALS_CNF       = 0x15, /* ���õ�ǰ��·�� */
    MN_CALL_EVT_QRY_ALS_CNF       = 0x16, /* ��ѯ��ǰ��·�� */

    MN_CALL_EVT_ECC_NUM_IND = 0x17,

    MN_CALL_EVT_HOLD     = 0x18, /* Call Hold ���б��� */
    MN_CALL_EVT_RETRIEVE = 0x19, /* Call Retrieve ���лָ� */

    MN_CALL_EVT_CLPR_SET_CNF = 0x1a,

    MN_CALL_EVT_XLEMA_CNF = 0x1d,

    MN_CALL_EVT_SET_CSSN_CNF = 0x1e,

    MN_CALL_EVT_CALL_ORIG_CNF = 0x1f,
    MN_CALL_EVT_SUPS_CMD_CNF  = 0x20,

    MN_CALL_EVT_START_DTMF_RSLT = 0x22,
    MN_CALL_EVT_STOP_DTMF_RSLT  = 0x23,

    MN_CALL_EVT_QRY_ECALL_INFO_CNF = 0x24,

    MN_CALL_EVT_CALL_MODIFY_CNF               = 0x25,
    MN_CALL_EVT_CALL_ANSWER_REMOTE_MODIFY_CNF = 0x26,
    MN_CALL_EVT_CALL_MODIFY_STATUS_IND        = 0x27,

    TAF_CALL_EVT_SEND_FLASH_RSLT, /* _H2ASN_MsgChoice TAF_CALL_EvtSendFlashRslt */
    /* _H2ASN_MsgChoice TAF_CALL_EvtCalledNumInfoInd */
    TAF_CALL_EVT_CALLED_NUM_INFO_IND,
    /* _H2ASN_MsgChoice TAF_CALL_EvtCallingNumInfoInd */
    TAF_CALL_EVT_CALLING_NUM_INFO_IND,
    TAF_CALL_EVT_DISPLAY_INFO_IND, /* _H2ASN_MsgChoice TAF_CALL_EvtDisplayInfoInd */
    /* _H2ASN_MsgChoice TAF_CALL_EvtExtDisplayInfoInd */
    TAF_CALL_EVT_EXT_DISPLAY_INFO_IND,
    TAF_CALL_EVT_CONN_NUM_INFO_IND, /* _H2ASN_MsgChoice TAF_CALL_EvtConnNumInfoInd */
    /* _H2ASN_MsgChoice TAF_CALL_EvtRedirNumInfoInd */
    TAF_CALL_EVT_REDIR_NUM_INFO_IND,
    TAF_CALL_EVT_SIGNAL_INFO_IND, /* _H2ASN_MsgChoice TAF_CALL_EvtSignalInfoInd */
    /* _H2ASN_MsgChoice TAF_CALL_EvtLineCtrlInfoInd */
    TAF_CALL_EVT_LINE_CTRL_INFO_IND,
    TAF_CALL_EVT_CALL_WAITING_IND, /* _H2ASN_MsgChoice TAF_CALL_EvtCallWaitingInd */
    TAF_CALL_EVT_ECONF_DIAL_CNF,
    /* imsa֪ͨSPMģ����ǿ�Ͷ෽ͨ�������ߵ�״̬���ͱ仯  */
    TAF_CALL_EVT_ECONF_NOTIFY_IND,
    TAF_CALL_EVT_CLCCECONF_INFO,

    /* _H2ASN_MsgChoice TAF_CALL_EvtSendBurstDtmfCnf */
    TAF_CALL_EVT_SEND_BURST_DTMF_CNF,
    /* _H2ASN_MsgChoice TAF_CALL_EvtSendBurstDtmfRslt */
    TAF_CALL_EVT_SEND_BURST_DTMF_RSLT,

    TAF_CALL_EVT_CCWAC_INFO_IND, /* _H2ASN_MsgChoice TAF_CALL_EvtCcwacInfoInd */

    /* _H2ASN_MsgChoice TAF_CALL_EvtSendContDtmfCnf */
    TAF_CALL_EVT_SEND_CONT_DTMF_CNF,
    /* _H2ASN_MsgChoice TAF_CALL_EvtSendContDtmfRslt */
    TAF_CALL_EVT_SEND_CONT_DTMF_RSLT,
    /* _H2ASN_MsgChoice TAF_CALL_EvtRcvContDtmfInd */
    TAF_CALL_EVT_RCV_CONT_DTMF_IND,
    /* _H2ASN_MsgChoice TAF_CALL_EvtRcvBurstDtmfInd */
    TAF_CALL_EVT_RCV_BURST_DTMF_IND,

    TAF_CALL_EVT_CCLPR_SET_CNF, /* _H2ASN_MsgChoice TAF_CALL_EvtCclprGetCnf */

    TAF_CALL_EVT_CCWAI_SET_CNF, /* _H2ASN_MsgChoice TAF_CALL_EVT_CCWAI_SET_CNF */

    TAF_CALL_EVT_CHANNEL_QRY_CNF,
    TAF_CALL_EVT_ECALL_ERROR_IND,
    MN_CALL_EVT_BUTT
};
typedef VOS_UINT32 MN_CALL_EventUint32;

/* [10.5.4.8] Called party subaddress */
/* ============================================================================ */
/* Table 10.5.119/3GPP TS 24.008: Called party subaddress */
/* Type of subaddress (octet 3)      */
/* Bits 7 6 5                        */
enum MN_CALL_TypeOfSubaddr {
    MN_CALL_SUBADDR_NSAP = 0,
    MN_CALL_USER_SPEC    = 2
}; /* All other values are reserved     */
typedef VOS_UINT8 MN_CALL_TypeOfSubaddrUint8;

/* ��ö�ٶ��嵽tafAppCall.h�ļ��� */

enum TAF_CALL_DigitMode {
    TAF_CALL_DIGIT_MODE_DTMF  = 0x00,
    TAF_CALL_DIGIT_MODE_ASCII = 0x01,
    TAF_CALL_DIGIT_MODE_BUTT  = 0x02
};
typedef VOS_UINT8 TAF_CALL_DigitModeUint8;


enum TAF_CALL_NumberPlan {
    TAF_CALL_NUMBER_PLAN_UNKNOW  = 0x00,
    TAF_CALL_NUMBER_PLAN_ISDN    = 0x01,
    TAF_CALL_NUMBER_PLAN_DATA    = 0x03,
    TAF_CALL_NUMBER_PLAN_TELEX   = 0x04,
    TAF_CALL_NUMBER_PLAN_PRIVATE = 0x09,
    TAF_CALL_NUMBER_PLAN_BUTT    = 0x0A
};
typedef VOS_UINT8 TAF_CALL_NumberPlanUint8;


enum TAF_CALL_NumberType {
    TAF_CALL_NUMBER_TYPE_UNKNOW           = 0x00,
    TAF_CALL_NUMBER_TYPE_INTERNATIONAL    = 0x01,
    TAF_CALL_NUMBER_TYPE_NATIONAL         = 0x02,
    TAF_CALL_NUMBER_TYPE_NETWORK_SPEC     = 0x03,
    TAF_CALL_NUMBER_TYPE_SUBSCRIBER       = 0x04,
    TAF_CALL_NUMBER_TYPE_RESERVED         = 0x05,
    TAF_CALL_NUMBER_TYPE_ABBREVIATED      = 0x06,
    TAF_CALL_NUMBER_TYPE_RESERVED_FOR_EXT = 0x07,
    TAF_CALL_NUMBER_TYPE_BUTT             = 0x08
};
typedef VOS_UINT8 TAF_CALL_NumberTypeUint8;


enum TAF_CALL_SendBurstDtmfCnfResult {
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_SUCCESS = 0,
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_ABNORMAL_STATE,
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_BUFFER_FULL,
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_ALLOC_MSG_FAIL,
    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_ALLOC_MEM_FAIL,

    TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_BUTT
};
typedef VOS_UINT8 TAF_CALL_SendBurstDtmfCnfResultUint8;


enum TAF_CALL_CniValidity {
    TAF_CALL_CNI_VALIDITY_VALID = 0, /* CNI Valid */
    /* CNI has been withheld by the originator */
    TAF_CALL_CNI_VALIDITY_WITHHELD,
    /* CNI is not available due to interworking problems or limitations of originating network */
    TAF_CALL_CNI_VALIDITY_NOT_AVAILABLE,
    TAF_CALL_CNI_VALIDITY_BUTT
};
typedef VOS_UINT8 TAF_CALL_CniValidityUint8;


enum TAF_CALL_Dcs {
    TAF_CALL_DCS_7BIT = 0,
    TAF_CALL_DCS_8BIT,
    TAF_CALL_DCS_UCS2, /* UCS-2 */
    TAF_CALL_DCS_BUTT
};
typedef VOS_UINT8 TAF_CALL_DcsUint8;


enum TAF_CS_CallDir {
    TAF_CS_CALL_DIR_NON = 0, /* ��CS����ں��е�CS�򲻴��ں���ʱ�����з���ʹ�øñ�־ */
    /* ��CS�򲻴��ں��е�CS����ں���ʱ���ҵ�һͨ������������ʱ��ʹ�øñ�־  */
    TAF_CS_CALL_DIR_MO = 1,
    /* ��CS�򲻴��ں��е�CS����ں���ʱ���ҵ�һͨ�������ڱ���ʱ��ʹ�øñ�־  */
    TAF_CS_CALL_DIR_MT = 2,
    TAF_CS_CALL_DIR_BUTT
};
typedef VOS_UINT8 TAF_CS_CallDirUint8;

enum TAF_CALL_RemoteCtrlType {
    TAF_CALL_REMOTE_CTRL_SEC_INFO_ERASE = 0,
    TAF_CALL_REMOTE_CTRL_PASSWORD_RESET = 1,
    TAF_CALL_REMOTE_CTRL_OTHER          = 2,
    TAF_CALL_REMOTE_CTRL_APP_TYPE_BUTT
};
typedef VOS_UINT32 TAF_CALL_RemoteCtrlTypeUint32;
enum TAF_CALL_EccSrvCapCfgResult {
    TAF_CALL_ECC_SRV_CAP_CFG_RESULT_SUCC = 0,
    TAF_CALL_ECC_SRV_CAP_CFG_RESULT_FAIL,
    TAF_CALL_ECC_SRV_CAP_CFG_RESULT_BUTT
};
typedef VOS_UINT32 TAF_CALL_EccSrvCapCfgResultUint32;
enum TAF_CALL_RemoteCtrlResult {
    TAF_CALL_REMOTE_CTRL_SUCC = 0,
    TAF_CALL_REMOTE_CTRL_FAIL,
    TAF_CALL_REMOTE_CTRL_RESULT_BUTT
};
typedef VOS_UINT32 TAF_CALL_RemoteCtrlResultUint32;
enum TAF_CALL_RemoteCtrlOperResult {
    TAF_CALL_SEND_RESULT_SUCC             = 0x00,
    TAF_CALL_SEND_RESULT_FAIL             = 0x01,
    TAF_CALL_SEND_RESULT_XSMS_POOL_FULL   = 0x02,
    TAF_CALL_SEND_RESULT_XSMS_LINK_ERR    = 0x03,
    TAF_CALL_SEND_RESULT_XSMS_NO_TL_ACK   = 0x04,
    TAF_CALL_SEND_RESULT_XSMS_ENCODE_ERR  = 0x05,
    TAF_CALL_SEND_RESULT_OPER_NOT_ALLOWED = 0x06,
    TAF_CALL_SEND_RESULT_OPER_TYPE_ERROR  = 0x07,
    TAF_CALL_SEND_RESULT_INT_ERRROR       = 0x08,
    TAF_CALL_REMOTE_CTRL_OPER_RESULT_BUTT
};
typedef VOS_UINT32 TAF_CALL_RemoteCtrlOperResultUint32;
enum TAF_CALL_EccSrvCap {
    TAF_CALL_ECC_SRV_CAP_DISABLE = 0,
    TAF_CALL_ECC_SRV_CAP_NO_CHANGE,
    TAF_CALL_ECC_SRV_CAP_ENABLE,
    TAF_CALL_ECC_SRV_CAP_BUTT
};
typedef VOS_UINT32 TAF_CALL_EccSrvCapUint32;
enum TAF_CALL_EccSrvStatus {
    TAF_CALL_ECC_SRV_STATUS_CLOSE = 0,
    TAF_CALL_ECC_SRV_STATUS_OPEN,
    TAF_CALL_ECC_SRV_STATUS_BUTT
};
typedef VOS_UINT32 TAF_CALL_EccSrvStatusUint32;
enum TAF_CALL_SetEcTestMode {
    TAF_CALL_SET_EC_TEST_MODE_DISABLE = 0,
    TAF_CALL_SET_EC_TEST_MODE_ENABLE,
    TAF_CALL_SET_EC_TEST_MODE_BUTT
};
typedef VOS_UINT32 TAF_CALL_SetEcTestModeUint32;
enum TAF_CALL_EncryptVoiceStatus {
    TAF_CALL_ENCRYPT_VOICE_SUCC = 0x0000,
    TAF_CALL_ENCRYPT_VOICE_TIMEOUT,
    TAF_CALL_ENCRYPT_VOICE_LOCAL_TERMINAL_NO_AUTHORITY,
    TAF_CALL_ENCRYPT_VOICE_REMOTE_TERMINAL_NO_AUTHORITY,
    TAF_CALL_ENCRYPT_VOICE_LOCAL_TERMINAL_ILLEGAL,
    TAF_CALL_ENCRYPT_VOICE_REMOTE_TERMINAL_ILLEGAL,
    TAF_CALL_ENCRYPT_VOICE_UNKNOWN_ERROR,
    TAF_CALL_ENCRYPT_VOICE_SIGNTURE_VERIFY_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_MT_CALL_NOTIFICATION,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_FAIL = 0x1000,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_POOL_FULL,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_LINK_ERR,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_NO_TL_ACK,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_ENCODE_ERR,
    TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_UNKNOWN,
    TAF_CALL_ENCRYPT_VOICE_SO_NEGO_FAILURE = 0x2000,
    TAT_CALL_APP_ENCRYPT_VOICE_TWO_CALL_ENTITY_EXIST,
    TAF_CALL_ENCRYPT_VOICE_NO_MO_CALL,
    TAF_CALL_ENCRYPT_VOICE_NO_MT_CALL,
    TAF_CALL_ENCRYPT_VOICE_NO_CALL_EXIST,
    TAF_CALL_ENCRYPT_VOICE_CALL_STATE_NOT_ALLOWED,
    TAF_CALL_ENCRYPT_VOICE_CALL_NUM_MISMATCH,
    TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_STATE_MISMATCH,
    TAF_CALL_ENCRYPT_VOICE_MSG_ENCODE_FAILUE,
    TAF_CALL_ENCRYPT_VOICE_MSG_DECODE_FAILUE,
    TAF_CALL_ENCRYPT_VOICE_GET_TEMP_PUB_PIVA_KEY_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_FILL_CIPHER_TEXT_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_ECC_CAP_NOT_SUPPORTED,
    TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_MODE_UNKNOWN,
    TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_MODE_MIMATCH,
    TAF_CALL_ENCRYPT_VOICE_CALL_RELEASED,
    TAF_CALL_ENCRYPT_VOICE_CALL_ANSWER_REQ_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_DECRYPT_KS_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_FAILURE_CAUSED_BY_INCOMING_CALL,
    TAF_CALL_ENCRYPT_VOICE_INIT_VOICE_FUNC_FAILURE,
    TAF_CALL_ENCRYPT_VOICE_TX01_TIMEOUT,
    TAF_CALL_ENCRYPT_VOICE_TX02_TIMEOUT,
    TAF_CALL_ENCRYPT_VOICE_STATUS_ENUM_BUTT
};
typedef VOS_UINT32 TAF_CALL_EncryptVoiceStatusUint32;
enum TAF_CALL_EncryptVoiceType {
    TAF_CALL_ENCRYPT_VOICE_TYPE_MO_MUNAUL_MODE = 0,
    TAF_CALL_ENCRYPT_VOICE_TYPE_MO_AUTO_MODE,
    TAF_CALL_ENCRYPT_VOICE_TYPE_MT_MUNAUL_MODE,
    TAF_CALL_ENCRYPT_VOICE_TYPE_MT_AUTO_MODE,
    TAF_CALL_ENCRYPT_VOICE_TYPE_BUTT
};
typedef VOS_UINT32 TAF_CALL_EncryptVoiceTypeUint32;
enum TAF_CALL_ResultType {
    TAF_CALL_RESULT_TYPE_SUCCESS,
    TAF_CALL_RESULT_TYPE_FAILURE,
    TAF_CALL_RESULT_TYPE_BUTT
};
typedef VOS_UINT32 TAF_CALL_ResultTypeUint32;

typedef struct {
    VOS_UINT8                 existFlag;   /* �����Ƿ�Я��CNAP��Ϣ��־ */
    TAF_CALL_CniValidityUint8 cniValidity; /* <CNI_Validity>���� */
    TAF_CALL_DcsUint8         dcs;         /* <name>������Data Coding Scheme */
    VOS_UINT8                 length;
    VOS_UINT8                 nameStr[TAF_CALL_CNAP_NAME_STR_MAX_LENGTH];
    VOS_UINT8                 reserved[3];
} TAF_CALL_Cnap;


typedef struct {
    VOS_UINT8 len;
    VOS_UINT8 reserved1[1];
    VOS_UINT8 bc[MN_CALL_BC_MAX_LEN];
} MN_CALL_Bc;

typedef struct {
    MN_CLIENT_ID_T                       clientId;
    MN_OPERATION_ID_T                    opId;
    MN_CALL_ID_T                         callId;
    TAF_CALL_SendBurstDtmfCnfResultUint8 result;
    VOS_UINT8                            reserved[3];
} TAF_CALL_EvtSendBurstDtmfCnf;


typedef struct {
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  result;
} TAF_CALL_EvtSendBurstDtmfRslt;

/* Call Info Structure */
typedef struct {
    MN_CLIENT_ID_T         clientId;  /* Client ID */
    MN_OPERATION_ID_T      opId;      /* op ID */
    MN_CALL_ID_T           callId;    /* Call ID */
    MN_CALL_StateUint8     callState; /* Call State */
    MN_CALL_MptyStateUint8 mptyState; /* MPTY State */
    MN_CALL_TypeUint8      callType;  /* Call Type */
    MN_CALL_ModeUint8      callMode;  /* Call Mode */
    MN_CALL_DirUint8       callDir;   /* Call Direction(MO/MT) */
    MN_CALL_PresentationIndUint8 callingNumPi;
    MN_CALL_PresentationIndUint8 redirectNumPi;
    TAF_CALL_SubStateUint8       callSubState;
    MN_CALL_CsDataCfgInfo        dataCfgInfo;     /* Data Configuration */
    MN_CALL_BcdNum               callNumber;      /* Call Number */
    MN_CALL_Subaddr              subCallNumber;   /* Call SubAddress */
    MN_CALL_CalledNum            calledNumber;    /* Called Number */
    MN_CALL_Subaddr              calledSubAddr;   /* Called SubAddress */
    MN_CALL_BcdNum               redirectNumber;  /* Redirect Number */
    MN_CALL_Subaddr              redirectSubaddr; /* Redirect Call SubAddress */
    MN_CALL_BcdNum               connectNumber;   /* Connect Number */
    MN_CALL_SupsCmdUint8         callSupsCmd;     /* Call Management Command */
    /* the result of the last Call Management Command */
    MN_CALL_SsResultUint8   ssResult;
    MN_CALL_SsErrCodeUint8  ssErrCode;  /* SS Error Code */
    VOS_UINT8               rabId;      /* RAB ID */
    MN_CALL_NoCliCauseUint8 noCliCause; /* Cause of the NO CLI */
    /* ALS Line number: 1 or 2,default is 1 */
    MN_CALL_AlsLineNoUint8 alsLineNo;

    VOS_UINT8 ataReportOkAsyncFlag; /*
                                     * VOS_TRUE:ata�첽�ϱ�ok������connect���ϱ�ok����������connect ack��
                                     * VOS_FALSE:ataͬ���ϱ�������connect����յ�����connect ack�����ϱ�ok
                                     */

    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    MN_CALL_SsNotify          ssNotify;    /* SS Notify */
    TAF_CS_CauseUint32        cause;       /* Cause Value of the Call Control */
    TAF_SS_CcbsFeature        ccbsFeature; /* CCBS feature info */
    MN_CALL_Uus1Info          uusInfo;
    VOS_UINT32                curCallTime;
    VOS_UINT32                totalCallTime;                        /* �ۼ�ͨ��ʱ�� */
    VOS_UINT32                preCallTime;                          /* �ϴ�ͨ��ʱ�� */
    MN_CALL_DIscDir           discDir;                              /* �Ҷϵ绰�ķ��� */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
    VOS_UINT8 encryptFlag; /* ���ܱ�� */
    /* ��¼����ͨ���Ƿ���ͨ��SRVCC������ */
    VOS_UINT8             srvccSuccFlg;
    MN_CALL_CfCauseUint8  callForwardCause;
    MN_CALL_HoldToneUint8 holdToneType;

    MN_CALL_DisplayName displayName;

    TAF_CALL_EvtSendBurstDtmfRslt sendBurstDtmfRslt;
    TAF_CALL_EvtSendBurstDtmfCnf  sendBurstDtmfCnf;

    MN_CALL_EmergencyCat emergencyCat; /* Emergency Catory */

    TAF_CALL_Cnap nameIndicator; /* Name Indicator */

    TAF_CALL_ErrorInfoText errInfoText;   /* �쳣��Ϣ�ı� */
    VOS_UINT8              startHifiFlag; /* IMSAָʾ�Ƿ���HIFI�ı�ʶ */
    TAF_CALL_ServiceTypeUint8 serviceType;
    VOS_UINT8                 econfFlag;
    VOS_UINT8                 reserved[1];

    TAF_CALL_EmcCallBackInfo  emcCallBackInfo;
    VOS_UINT32                endSource; /* ָʾ�绰�Ҷ���Դ */
} MN_CALL_Info;

typedef struct {
    MN_CALL_TypeUint8    callType; /* Call Type */
    MN_CALL_ClirCfgUint8 clirCfg;  /* CLIR Configuration */
    MN_CALL_ModeUint8    callMode; /* Call Mode */
    /* false: ������cs���Թ�����cs�򲻿��� true: CS��û���Թ�����cs����� */
    VOS_UINT8         csCallRetryFlg;
    MN_CALL_CalledNum dialNumber; /* Call Number */
    MN_CALL_CugCfg    cugCfg;     /* CUG Configuration */
    MN_CALL_CsDataCfg dataCfg;    /* Data Configuration */
    MN_CALL_Subaddr   subaddr;    /* Subaddress */

    MN_CALL_EmergencyCat emergencyCat; /* Emergency Catory */

    VOS_UINT8 imsInvitePtptFlag; /* IMS ���������ͨ����� */

    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */

    VOS_UINT8 encryptFlag; /* ���ܱ�� */

    VOS_UINT8 domainRedialFlag; /* ��ǰ�����ʧ�ܺ��������Ա�� */
    /* ucDomainRedialFlagΪtrue��IMS��ͨ�����յ�380ʱ���Ƿ�Ҫ�Ҷϵ�ǰ���������ͨ���б�־ */
    VOS_UINT8 hangupImsCallInCsRedialFlg;

    TAF_CALL_EmcDomainUint8 emcDomain;

    TAF_CALL_ServiceTypeUint8 serviceType; /* ҵ������ */

    VOS_UINT8 rttFlg; /* �Ƿ���RTT���У�0:��RTT���� */
    /* 1:��ȡ��һ·���У�0:����ȡ��һ·���� */
    VOS_UINT8 callPullFlg;
    /* 0-254:��ȡ��һ·����,ucCallPullFlg=1; 255:����ȡ��һ·����,ucCallPullFlg=0  */
    VOS_UINT8  callPullDialogId;
    VOS_UINT8  reserved;
    VOS_UINT8  is1xStkEmc;
    MN_CALL_Bc bc;
    MN_CALL_ExtEmcInfo extEmcInfo;
} MN_CALL_OrigParam;

typedef struct {
    MN_CALL_ID_T       callId;
    VOS_UINT8          reserved[3];
    TAF_CS_CauseUint32 endCause; /* End Cause */
} MN_CALL_EndParam;

typedef struct {
    VOS_UINT32 reserved;
} MN_CALL_AnsParam;

typedef struct {
    MN_CALL_SupsCmdUint8 callSupsCmd; /* Call Management Command */
    MN_CALL_ID_T         callId;      /* Call ID */
    MN_CALL_TypeUint8    callType;    /* ����Խ�������·���в������� */
    /* ��������ΪMN_CALL_SUPS_CMD_REL_INCOMING_OR_WAITINGʱ����IE����Ч */
    TAF_CALL_RejCauseUint8 callRejCause;
    MN_CALL_BcdNum         redirectNum; /* Redirect Number */
    MN_CALL_CalledNum      removeNum;   /* ��ǿ�ͻ�������Ҫ�Ƴ�������ߺ��� */
    VOS_UINT8              rttFlg;      /* 0: ��RTTͨ��, 1: RTTͨ�� */
    VOS_UINT8              reserved[3]; /* ����λ */
} MN_CALL_SupsParam;


typedef struct {
    VOS_UINT32 actNum; /* ��Ҫ���õ�CSSN�ĸ��� */
    /* aenSetType[0]:CSSI�Ƿ���Ҫ���enSetType[1]:CSSU�Ƿ���Ҫ���� */
    MN_CALL_SetCssnTypeUint32 setType[MN_CALL_MAX_CSSN_MSG_NUM];
} MN_CALL_SetCssnReq;


typedef struct {
    MN_CALL_TypeUint8 currCallType;   /* ��ǰ�������� */
    MN_CALL_TypeUint8 expectCallType; /* ������������ */
    VOS_UINT8         reserved[2];
} MN_CALL_ModifyReq;


typedef struct {
    MN_CALL_Cuus1FlgUint32    cuus1IFlg;                          /* CUUS1I��־ */
    MN_CALL_Cuus1FlgUint32    cuus1UFlg;                          /* CUUS1U��־ */
    VOS_UINT32                actNum;                             /* ��Ҫ���õ�UUS1�ĸ��� */
    MN_CALL_SetUus1TypeUint32 setType[MN_CALL_MAX_UUS1_MSG_NUM];  /* Ҫ���õ��û���������1���� */
    MN_CALL_Uus1Info          uus1Info[MN_CALL_MAX_UUS1_MSG_NUM]; /* ���õ��û�����������Ϣ */
} MN_CALL_Uus1Param;


typedef struct {
    MN_CALL_EventUint32 event;    /* �ظ����¼����� */
    MN_CLIENT_ID_T      clientId; /* �˿ں� */
    MN_OPERATION_ID_T   opId;     /* ����ID */
    VOS_UINT8           reserved; /* �����ֶ� */
    VOS_UINT32          ret;      /* �ظ���� */
} TAF_PH_SetUus1InfoCnf;


typedef struct {
    MN_CALL_EventUint32       event;                              /* �ظ����¼����� */
    MN_CLIENT_ID_T            clientId;                           /* �˿ں� */
    MN_OPERATION_ID_T         opId;                               /* ����ID */
    VOS_UINT8                 reserved;                           /* �����ֶ� */
    VOS_UINT32                ret;                                /* �ظ���� */
    VOS_UINT32                actNum;                             /* �û���������1�ĸ��� */
    MN_CALL_Uus1Info          uus1Info[MN_CALL_MAX_UUS1_MSG_NUM]; /* �û���������1����Ϣ */
    MN_CALL_SetUus1TypeUint32 setType[MN_CALL_MAX_UUS1_MSG_NUM];  /* Ҫ���õ��û���������1���� */
} TAF_PH_QryUus1InfoCnf;


typedef struct {
    MN_CALL_AlsLineNoUint8 alsLine;     /* Ҫʹ�õ���·�� */
    VOS_UINT8              reserved[3]; /* �����ֶ� */
} MN_CALL_SetAlsParam;


typedef struct {
    MN_CALL_EventUint32 event;    /* �ظ����¼����� */
    MN_CLIENT_ID_T      clientId; /* �˿ں� */
    MN_OPERATION_ID_T   opId;     /* ����ID */
    VOS_UINT8           reserved; /* �����ֶ� */
    VOS_UINT32          ret;      /* �ظ���� */
} MN_CALL_SetAlsCnf;


typedef struct {
    MN_CALL_EventUint32    event;        /* �ظ����¼����� */
    MN_CLIENT_ID_T         clientId;     /* �˿ں� */
    MN_OPERATION_ID_T      opId;         /* ����ID */
    VOS_UINT8              reserved1;    /* �����ֶ� */
    VOS_UINT32             ret;          /* �ظ���� */
    MN_CALL_AlsLineNoUint8 alsLine;      /* ��ǰʹ�õ���·�� */
    VOS_UINT8              reserved2[3]; /* �����ֶ� */
} MN_CALL_QryAlsCnf;

/*
 * �ṹ˵��: CALL��AT�ϱ���ǰ���еĽ����������б�
 */
typedef struct {
    VOS_UINT8 index;    /* ���ƽ������������� */
    VOS_UINT8 total;    /* ���ƽ������������� */
    VOS_UINT8 category; /* �������������� */
    /* �������������п�ʱ��Ч�����޿�ʱ��Ч��1�п�ʱ��Ч��0�޿�ʱ��Ч */
    VOS_UINT8      simPresent;
    MN_CALL_BcdNum eccNum; /* ���������� */
    VOS_UINT32     mcc;    /* MCC,3 bytes */
} MN_CALL_EccListInfo;


typedef struct {
    VOS_UINT32 opPeerVideoSupport : 1;
    VOS_UINT32 opSpare : 31;

    MN_CALL_ID_T              callId;      /* Call ID */
    MN_CALL_DirUint8          callDir;     /* Call Direction(MO/MT) */
    MN_CALL_StateUint8        callState;   /* Call State */
    MN_CALL_MptyStateUint8    mptyState;   /* MPTY State */
    MN_CALL_TypeUint8         callType;    /* Call Type */
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    /* Is Econf:true, Is not Econf:false */
    VOS_UINT8 eConferenceFlag;
    /* Is Encrypt:true, Is not Encrypt:false */
    VOS_UINT8         encryptFlag;
    MN_CALL_BcdNum    callNumber;    /* Call Number */
    MN_CALL_CalledNum calledNumber;  /* Called Number */
    MN_CALL_BcdNum    connectNumber; /* Connect Number */

    MN_CALL_DisplayName displayName;

    TAF_CALL_PeerVideoSupportUint8 peerVideoSupport;
    TAF_CALL_ImsDomainUint8        imsDomain;
    TAF_CALL_ServiceTypeUint8 serviceType;
    /* 0:��RTTͨ����1:RTTͨ��; 2:Զ��ΪTTYͨ��; 3: Զ�˲�֧��RTT  */
    TAF_CALL_RttIndUint8 rtt;
    VOS_INT32            rttChannelId; /* ͨ��ʹ�õ�channel ID��-1Ϊ��Чֵ */
    VOS_UINT32           cps;          /* RTTͨ��Э�̵�cpsֵ  */
} MN_CALL_InfoParam;

typedef struct {
    MN_CALL_EventUint32 event;    /* �ظ����¼����� */
    MN_CLIENT_ID_T      clientId; /* �˿ں� */
    MN_OPERATION_ID_T   opId;     /* ����ID */
    MN_CALL_ID_T        callId;   /* Call ID */
    TAF_CS_CauseUint32  cause;    /* ������ */
} TAF_CALL_EconfDialCnf;

typedef struct {
    MN_CALL_EventUint32 event;    /* �¼��� */
    MN_CLIENT_ID_T      clientId; /* �û�ID */
    VOS_UINT8           numOfMaxCalls;
    VOS_UINT8           numOfCalls; /* �෽ͨ������߸��� */
    /* ��������ͨ������Ϣ */
    TAF_CALL_EconfInfoParam callInfo[TAF_CALL_MAX_ECONF_CALLED_NUM];
} TAF_CALL_EconfInfoQryCnf;


typedef struct {
    MN_CALL_EventUint32 event; /* �¼��� */
    MN_CLIENT_ID_T      clientId;
    MN_OPERATION_ID_T   opId;       /* ����ID */
    VOS_UINT8           numOfCalls; /* ��������ͨ���ĸ��� */
    /* ��������ͨ������Ϣ */
    TAF_CALL_EconfInfoParam callInfo[TAF_CALL_MAX_ECONF_CALLED_NUM];
} TAF_CALL_EvtEconfNotifyInd;


typedef struct {
    MN_CALL_EventUint32 event;                      /* �¼��� */
    MN_CLIENT_ID_T      clientId;                   /* �û�ID */
    VOS_UINT8           numOfCalls;                 /* ��������ͨ���ĸ��� */
    VOS_CHAR            reserved;                   /* �����ֶ� */
    MN_CALL_InfoParam   callInfos[MN_CALL_MAX_NUM]; /* ��������ͨ������Ϣ */
} MN_CALL_InfoQryCnf;

/*
 * �ṹ˵��: AT��CALL������Ӫ�̶��ƽ���������ṹ
 */
typedef struct {
    VOS_UINT8 index;    /* ���ƽ������������� */
    VOS_UINT8 total;    /* ���ƽ������������� */
    VOS_UINT8 category; /* �������������� */
    /* �������������п�ʱ��Ч�����޿�ʱ��Ч��1�п�ʱ��Ч��0�޿�ʱ��Ч */
    VOS_UINT8      simPresent;
    VOS_UINT32     mcc;    /* MCC,3 bytes */
    MN_CALL_BcdNum eccNum; /* ���������� */
    /* �����������п�ʱ�Ƿ���ڷ���������ʱ��Ч��0:�κη���ʱ����Ч��1:������������ʱ��Ч */
    VOS_UINT8 abnormalServiceFlg;
    VOS_UINT8 reserved[3];
} MN_CALL_AppCustomEccNumReq;

/*
 * �ṹ˵��: ��Ӫ�̶��ƽ���������ṹ
 */
typedef struct {
    VOS_UINT8 category; /* �������������� */
    /* �������������п�ʱ��Ч�����޿�ʱ��Ч��1�п�ʱ��Ч��0�޿�ʱ��Ч */
    VOS_UINT8 validSimPresent;
    /* �����������п�ʱ�Ƿ���ڷ���������ʱ��Ч��0:�κη���ʱ����Ч��1:������������ʱ��Ч */
    VOS_UINT8  abnormalServiceFlg;
    VOS_UINT8  eccNumLen;
    VOS_UINT8  eccNum[MN_CALL_MAX_BCD_NUM_LEN];
    VOS_UINT32 mcc; /* MCC,3 bytes */
} MN_CALL_CustomEccNum;

/*
 * �ṹ˵��: CALL��AT�ϱ���ǰ���еĽ����������б�
 */
typedef struct {
    VOS_UINT32           eccNumCount;
    MN_CALL_CustomEccNum customEccNumList[MN_CALL_MAX_EMC_NUM];
} MN_CALL_EccNumInfo;

enum MN_CALL_RadioMode {
    MN_CALL_RADIO_MODE_GSM,
    MN_CALL_RADIO_MODE_WCDMA,
    MN_CALL_RADIO_MODE_IMS_EUTRAN,
    MN_CALL_RADIO_MODE_BUTT
};
typedef VOS_UINT8 MN_CALL_RadioModeUint8;

enum MN_CALL_ChannelRate {
    MN_CALL_CHANNEL_RATE_475, /* rate 4.75kbps/s */
    MN_CALL_CHANNEL_RATE_515, /* rate 5.15kbps/s */
    MN_CALL_CHANNEL_RATE_59,  /* rate 5.9kbps/s */
    MN_CALL_CHANNEL_RATE_67,  /* rate 6.7kbps/s */
    MN_CALL_CHANNEL_RATE_74,  /* rate 7.4kbps/s */
    MN_CALL_CHANNEL_RATE_795, /* rate 7.95kbps/s */
    MN_CALL_CHANNEL_RATE_102, /* rate 10.2kbps/s */
    MN_CALL_CHANNEL_RATE_122, /* rate 12.2kbps/s */
    MN_CALL_CHANNEL_RATE_BUTT /* invalid value */
};
typedef VOS_UINT8 MN_CALL_ChannelRateUint8;


enum TAF_CALL_Type {
    TAF_CALL_TYPE_NORMAL_CALL,
    TAF_CALL_TYPE_ECALL,
    TAF_CALL_TYPE_BUTT
};
typedef VOS_UINT16 TAF_CALL_TypeUint16;

typedef struct {
    MN_CALL_RadioModeUint8 mode;      /* GSM|WCDMA */
    MN_CALL_CodecTypeUint8 codecType; /* codec type */
    TAF_CALL_TypeUint16    callType;  /* eCall type */
} MN_CALL_ChannelParam;

typedef struct {
    VOS_BOOL             channelEnable; /* GSM|WCDMA */
    MN_CALL_ChannelParam channelParam;  /* channel rate */
} MN_CALL_ChannelInfo;

enum MN_CALL_ChannelEvent {
    MN_CALL_EVT_CHANNEL_OPEN,  /* open codec channel */
    MN_CALL_EVT_CHANNEL_CLOSE, /* close codec channel */
    MN_CALL_EVT_CHANNEL_PARA_CHANGE,

    MN_CALL_EVT_CHANNEL_LOCAL_ALERTING,
    MN_CALL_EVT_TYPE_BUFF /* reserved value */
};
typedef VOS_UINT32 MN_CALL_ChannelEventUint32;

/*
 * Description:
 */
typedef struct {
    VOS_UINT8  eccKmcVer;
    VOS_UINT8  reserved[1];
    VOS_UINT16 eccKmcLength;
    VOS_UINT8  eccKmcData[TAF_CALL_ECC_KMC_PUBLIC_KEY_MAX_HEX_LENGTH];
} MN_CALL_AppEcKmcData;


typedef struct {
    TAF_CALL_CcwaiModeUint8     mode;
    TAF_CALL_CcwaiSrvClassUint8 srvClass;
    VOS_UINT8                   reserve[2];
} TAF_CALL_CcwaiSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32         msgId;
    MN_CLIENT_ID_T     clientId;
    MN_OPERATION_ID_T  opId;
    MN_CALL_ID_T       callId;
    TAF_CALL_DtmfParam dtmf;
} MN_CALL_DtmfReqMsg;


typedef struct {
    MN_CALL_EventUint32       event; /* �¼��� */
    MN_CLIENT_ID_T            clientId;
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 reserve[1];
    VOS_UINT8                 callNum;
    VOS_UINT8                 callId[MN_CALL_MAX_NUM];
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} MN_CALL_EvtHold;


typedef struct {
    MN_CALL_EventUint32       event; /* �¼��� */
    MN_CLIENT_ID_T            clientId;
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 reserve[1];
    VOS_UINT8                 callNum;
    VOS_UINT8                 callId[MN_CALL_MAX_NUM];
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} MN_CALL_EvtRetrieve;


typedef struct {
    MN_CALL_EventUint32 event; /* �ظ����¼����� */
    VOS_UINT32          ret;
    AT_APPCTRL          appCtrl;
    MN_CALL_ClprInfo clprInfo;
    TAF_CALL_PresentationIndUint8 pi;
    TAF_CALL_QryClprModeUint8     qryClprModeType;
    MN_CALL_ID_T                  callId;
    VOS_UINT8                     rsv[1];
} MN_CALL_ClprGetCnf;


typedef struct {
    MN_CALL_EventUint32 eventId; /* �¼�����   */

    AT_APPCTRL         appCtrl; /* Ӧ�ÿ���ͷ */
    TAF_CS_CauseUint32 cause;   /* ������     */
} MN_CALL_EvtErrInd;


typedef struct {
    MN_CALL_EventUint32  event;    /* �ظ����¼����� */
    TAF_ERROR_CodeUint32 ret;      /* �ظ���� */
    MN_CLIENT_ID_T       clientId; /* �˿ں� */
    MN_OPERATION_ID_T    opId;     /* ����ID */
    VOS_UINT8            reserved; /* �����ֶ� */
} MN_CALL_SetCssnCnf;

typedef struct {
    MN_CALL_EventUint32 event; /* �¼��� */
    MN_CLIENT_ID_T      clientId;

    /* �Ƿ�Ϊ���ز��Ż�������ʶ1:���ز��Ż�������0:������� */
    VOS_UINT8              isLocalAlertingFlag;
    MN_CALL_CodecTypeUint8 codecType; /* codec type */

    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 rsv[3];
} MN_CALL_EvtChannelInfo;


typedef struct {
    MN_CLIENT_ID_T     clientId;
    MN_OPERATION_ID_T  opId;
    VOS_UINT8          reserved1[1];
    TAF_CS_CauseUint32 cause; /* ������ */

    MN_CALL_ID_T            callId;
    TAF_CALL_DtmfStateUint8 dtmfState;
    VOS_UINT8               dtmfCnt;
    VOS_UINT8               reserved2[1];

} TAF_CALL_EvtDtmfCnf;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         reserved;
    VOS_UINT32        result;
} TAF_CALL_EvtCcwaiCnf;


typedef struct {
    MN_CALL_ID_T       callId;      /* ��ǰ��call ID */
    MN_CALL_TypeUint8  ecallType;   /* eCall�������� */
    MN_CALL_StateUint8 callState;   /* ����״̬ */
    VOS_UINT8          reserved[1]; /* ���� */
    MN_CALL_CalledNum  dialNumber;  /* ���к��� */
} MN_CALL_EcallInfoPara;


typedef struct {
    VOS_UINT8             numOfEcall;                  /* ��ǰeCall���и��� */
    VOS_UINT8             reserved[3];                 /* ���� */
    MN_CALL_EcallInfoPara ecallInfos[MN_CALL_MAX_NUM]; /* eCall��Ϣ���� */
} MN_CALL_EcallInfo;


typedef struct {
    VOS_UINT16        clientId;
    VOS_UINT8         opId;
    VOS_UINT8         reserved[1];
    MN_CALL_EcallInfo ecallInfo;
} MN_CALL_EvtQryEcallInfoCnf;



typedef struct {
    VOS_UINT8 digitNum;
    /*
     * ASCII �ַ����Ϸ����ַ���������'0' - '9', '*', '#', '+'��'+'ֻ�ܳ����ں������ǰ�棬
     * �������󳤶Ȳ��ܳ���32��������"+"��
     */
    VOS_UINT8 digit[TAF_CALL_MAX_FLASH_DIGIT_LEN + 1];
    VOS_UINT8 reserved[2];
} TAF_CALL_FlashPara;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT16         msgId;        /* _H2ASN_Skip */ /* ��Ϣ�� */
    VOS_UINT8          reserved1[2]; /* _H2ASN_Skip */ /* ���� */
    MN_CLIENT_ID_T     clientId;     /* _H2ASN_Skip */
    MN_OPERATION_ID_T  opId;         /* _H2ASN_Skip */
    VOS_UINT8          reserved2[1]; /* _H2ASN_Skip */
    TAF_CALL_FlashPara flashPara;
} TAF_CALL_AppSendFlashReq;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         result; /* VOS_OK:�ɹ�; VOS_ERR:ʧ�� */
} TAF_CALL_EvtSendFlashRslt;


typedef struct {
    /* ����ID�����Я���ò����������ǵ�ǰ���еĺ���ID */
    MN_CALL_ID_T callId;
    VOS_UINT8    reserved[3];
    VOS_UINT8    digitNum;
    VOS_UINT8    digit[TAF_CALL_MAX_BURST_DTMF_NUM]; /* DTMF Keys: ������Ӧ���ַ�  */
    /* DTMF������ʱ������ʱ��(��λms) */
    VOS_UINT32 onLength;
    /* DTMF�����͵���Сʱ����(��λms)  */
    VOS_UINT32 offLength;
} TAF_CALL_BurstDtmfPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32             msgId;
    MN_CLIENT_ID_T         clientId;
    MN_OPERATION_ID_T      opId;
    MN_CALL_ID_T           callId;
    TAF_CALL_BurstDtmfPara burstDtmfPara;
} TAF_CALL_BurstDtmfReqMsg;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    VOS_UINT16        msgId;        /* _H2ASN_Skip */
    VOS_UINT8         reserved1[2]; /* _H2ASN_Skip */
    MN_CLIENT_ID_T    clientId;     /* _H2ASN_Skip */
    MN_OPERATION_ID_T opId;         /* _H2ASN_Skip */
    VOS_UINT8         callId;
} TAF_CALL_SndCclprReqMsg;


typedef struct {
    MN_CLIENT_ID_T           clientId;
    MN_OPERATION_ID_T        opId;
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[1];
    VOS_UINT8                digit[TAF_CALL_MAX_CALLED_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_EvtCalledNumInfoInd;


typedef struct {
    MN_CLIENT_ID_T           clientId;
    MN_OPERATION_ID_T        opId;
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[3];
    VOS_UINT8                digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_EvtCallingNumInfoInd;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         callId;
    VOS_UINT8         digitNum;
    VOS_UINT8         rsved[3];
    VOS_UINT8         digit[TAF_CALL_MAX_DISPALY_CHARI_OCTET_NUM];
} TAF_CALL_EvtDisplayInfoInd;


typedef struct {
    VOS_UINT8 dispalyTag;
    VOS_UINT8 dispalyLen;
    VOS_UINT8 digitNum;
    VOS_UINT8 rsved[1];
    VOS_UINT8 digit[TAF_CALL_MAX_EXTENDED_DISPALY_CHARI_OCTET_NUM];
} TAF_CALL_EvtExtDisplayInfoData;


typedef struct {
    MN_CLIENT_ID_T                 clientId;
    MN_OPERATION_ID_T              opId;
    VOS_UINT8                      callId;
    VOS_UINT8                      infoRecsDataNum;
    VOS_UINT8                      displayType;
    VOS_UINT8                      extDispInd;
    VOS_UINT8                      reserved;
    TAF_CALL_EvtExtDisplayInfoData infoRecsData[TAF_CALL_MAX_EXT_DISPLAY_DATA_NUM];
} TAF_CALL_EvtExtDisplayInfoInd;


typedef struct {
    MN_CLIENT_ID_T           clientId;
    MN_OPERATION_ID_T        opId;
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[3];
    VOS_UINT8                digit[TAF_CALL_MAX_CONNECTED_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_EvtConnNumInfoInd;


typedef struct {
    MN_CLIENT_ID_T           clientId;
    MN_OPERATION_ID_T        opId;
    VOS_UINT8                callId;
    VOS_UINT32               opPi : 1;
    VOS_UINT32               opSi : 1;
    VOS_UINT32               opRedirReason : 1;
    VOS_UINT32               opSpare : 29;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                redirReason;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[2];
    VOS_UINT8                digitNumArray[TAF_CALL_MAX_REDIRECTING_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_EvtRedirNumInfoInd;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         callId;
    VOS_UINT8         signalType;
    VOS_UINT8         alertPitch;
    VOS_UINT8         signal;
    VOS_UINT8         rsved[1];
} TAF_CALL_EvtSignalInfoInd;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         callId;
    VOS_UINT8         polarityIncluded;
    VOS_UINT8         toggleModePresent;
    VOS_UINT8         toggleMode;
    VOS_UINT8         reversePolarityPresent;
    VOS_UINT8         reversePolarity;
    VOS_UINT8         powerDenialTime;
    VOS_UINT8         reserved[2];
} TAF_CALL_EvtLineCtrlInfoInd;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         callId;
    VOS_UINT8         callWaitingInd;
    VOS_UINT8         rsved[3];
} TAF_CALL_EvtCallWaitingInd;


typedef struct {
    MN_CLIENT_ID_T           clientId;
    MN_OPERATION_ID_T        opId;
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                signalType;
    VOS_UINT8                alertPitch;
    VOS_UINT8                signal;
    VOS_UINT8                signalIsPresent;
    VOS_UINT8                rsved[2];
    VOS_UINT8                digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM + 1];
} TAF_CALL_EvtCcwacInfoInd;


typedef struct {
    MN_CALL_CalledNum dialNumber;
    VOS_UINT32        so;
    MN_CALL_TypeUint8 callType;
    VOS_UINT8         reserved[3];
} TAF_CALL_CustomDialPara;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT16              msgId;        /* _H2ASN_Skip */ /* ��Ϣ�� */
    VOS_UINT8               reserved1[2]; /* _H2ASN_Skip */ /* ���� */
    MN_CLIENT_ID_T          clientId;     /* _H2ASN_Skip */
    MN_OPERATION_ID_T       opId;         /* _H2ASN_Skip */
    VOS_UINT8               reserved2[1]; /* _H2ASN_Skip */
    TAF_CALL_CustomDialPara customDialPara;
} TAF_CALL_AppSendCustomDialReq;



enum TAF_CALL_ContDtmfSwitch {
    TAF_CALL_CONT_DTMF_STOP  = 0,
    TAF_CALL_CONT_DTMF_START = 1,
    TAF_CALL_CONT_DTMF_BUTT
};
typedef VOS_UINT8 TAF_CALL_ContDtmfSwitchUint8;


typedef struct {
    MN_CALL_ID_T callId; /* ����ID, ��ǰ���еĺ���ID */
    /* enDtmfSwitch: TAF_CALL_CONT_DTMF_START,TAF_CALL_CONT_DTMF_STOP  */
    TAF_CALL_ContDtmfSwitchUint8 dtmfSwitch;
    /* DTMF Keys: ������Ӧ���ַ���1-9��*��#  */
    VOS_UINT8 digit;
    VOS_UINT8 reserved;
} TAF_CALL_ContDtmfPara;


typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    VOS_UINT16            msgId;        /* _H2ASN_Skip */
    VOS_UINT8             reserved1[2]; /* _H2ASN_Skip */
    MN_CLIENT_ID_T        clientId;     /* _H2ASN_Skip */
    MN_OPERATION_ID_T     opId;         /* _H2ASN_Skip */
    VOS_UINT8             reserved2;    /* _H2ASN_Skip */
    TAF_CALL_ContDtmfPara contDtmfPara;
} TAF_CALL_ContDtmfReqMsg;


enum TAF_CALL_SendContDtmfCnfResult {
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_SUCCESS = 0,
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_ABNORMAL_STATE,
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_BUFFER_FULL,
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_ALLOC_MSG_FAIL,
    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_ALLOC_MEM_FAIL,

    TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_BUTT
};
typedef VOS_UINT8 TAF_CALL_SendContDtmfCnfResultUint8;


typedef struct {
    MN_CLIENT_ID_T                      clientId;
    MN_OPERATION_ID_T                   opId;
    MN_CALL_ID_T                        callId; /* ����ID, ��ǰ���еĺ���ID */
    TAF_CALL_SendContDtmfCnfResultUint8 result; /* ����BURST DTMF CNF�Ľ�� */
    VOS_UINT8                           reserved[3];
} TAF_CALL_EvtSendContDtmfCnf;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         result; /* ����ȡֵ:VOS_OK, VOS_ERR */
} TAF_CALL_EvtSendContDtmfRslt;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    MN_CALL_ID_T      callId; /* ����ID, ��ǰ���еĺ���ID */
    /* ucSwitch: TAF_CALL_CONT_DTMF_STOP,TAF_CALL_CONT_DTMF_START  */
    TAF_CALL_ContDtmfSwitchUint8 dtmfSwitch;
    VOS_UINT8                    digit; /* 1-9,0,*,# */
    VOS_UINT8                    reserved[2];
} TAF_CALL_EvtRcvContDtmfInd;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    MN_CALL_ID_T      callId;                             /* ����ID, ��ǰ���еĺ���ID */
    VOS_UINT8         digitNum;                           /* *<  Digit number */
    VOS_UINT8         digit[TAF_CALL_MAX_BURST_DTMF_NUM]; /* *<  DTMF digits */
    /* *<  refers to S.0005 3.7.3.3.2.9 */
    VOS_UINT32 onLength;
    /* *<  refers to S.0005 3.7.3.3.2.9 */
    VOS_UINT32 offLength;
} TAF_CALL_EvtRcvBurstDtmfInd;


typedef struct {
    MN_CLIENT_ID_T                clientId;
    MN_OPERATION_ID_T             opId;
    MN_CALL_ID_T                  callId;
    VOS_UINT32                    ret;
    TAF_CALL_PresentationIndUint8 pi;
    VOS_UINT8                     reserved[3];
} TAF_CALL_EvtCclprGetCnf;


typedef struct {
    TAF_CALL_GetCallInfoTypeUint8 getCallInfoType;
    VOS_UINT8                     reserved[3];
} TAF_CALL_QryCallInfoReqPara;


typedef struct {
    VOS_UINT8                     numOfCalls; /* ��������ͨ���ĸ��� */
    TAF_CALL_GetCallInfoTypeUint8 getCallInfoType;
    VOS_UINT8                     reserved[2];
    MN_CALL_InfoParam             callInfos[MN_CALL_MAX_NUM]; /* ��������ͨ������Ϣ */
} TAF_CALL_QryCallInfoCnfPara;


typedef struct {
    MN_CALL_ID_T       callId;    /* Call ID */
    MN_CALL_StateUint8 callState; /* Call State */
    MN_CALL_TypeUint8  callType;  /* Call Type */
    TAF_CALL_VoiceDomainUint8 voiceDomain;
    TAF_CALL_ServiceTypeUint8 serviceType;
    VOS_UINT8                 reserved[3];
} TAFAGENT_CALL_InfoParam;


typedef struct {
    VOS_UINT8               numOfCalls; /* ��������ͨ���ĸ��� */
    VOS_UINT8               reserved[3];
    TAFAGENT_CALL_InfoParam callInfos[MN_CALL_MAX_NUM]; /* ��������ͨ������Ϣ */
} TAFAGENT_CALL_QryCallInfoPara;


typedef struct {
    MN_CALL_ID_T              callId;
    TAF_CALL_QryClprModeUint8 qryClprModeType;
    VOS_UINT8                 reserved[2];
} TAF_CALL_QryClprPara;


typedef struct {
    TAF_CS_CauseUint32     cause;     /* Cause Value of the Call Control */
    MN_CALL_ID_T           callId;    /* Call ID */
    MN_CALL_StateUint8     callState; /* Call State */
    MN_CALL_TypeUint8      callType;
    VOS_UINT8              reserved;
    TAF_CALL_ErrorInfoText errInfoText; /* �쳣��Ϣ�ı� */
} TAF_CALL_OrigCnfPara;

typedef struct {
    /* the result of the last Call Management Command */
    MN_CALL_SsResultUint8 ssResult;
    VOS_UINT8             reserved[3];
} TAF_CALL_SupsCmdRsltPara;


typedef struct {
    MN_CALL_ID_T              callId;                               /* Call ID */
    MN_CALL_StateUint8        callState;                            /* Call State */
    MN_CALL_TypeUint8         callType;                             /* Call Type */
    TAF_CALL_VoiceDomainUint8 voiceDomain;                          /* VOICE DOMAIN */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
} TAF_CALL_OrigIndPara;


typedef struct {
    MN_CALL_ID_T              callId;                               /* Call ID */
    MN_CALL_StateUint8        callState;                            /* Call State */
    TAF_CALL_VoiceDomainUint8 voiceDomain;                          /* VOICE DOMAIN */
    MN_CALL_TypeUint8         callType;                             /* Call Type */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
} TAF_CALL_AlertingIndPara;

typedef struct {
    MN_CALL_ID_T              callId;                               /* Call ID */
    MN_CALL_StateUint8        callState;                            /* Call State */
    MN_CALL_TypeUint8         callType;                             /* Call Type */
    TAF_CALL_VoiceDomainUint8 voiceDomain;                          /* VOICE DOMAIN */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
} TAF_CALL_ProcIndPara;


typedef struct {
    MN_CALL_ID_T              callId;        /* Call ID */
    MN_CALL_StateUint8        callState;     /* Call State */
    MN_CALL_TypeUint8         callType;      /* Call Type */
    MN_CALL_DirUint8          callDir;       /* Call Direction(MO/MT) */
    MN_CALL_BcdNum            connectNumber; /* Connect Number */
    VOS_UINT8                 rabId;         /* RAB ID */
    TAF_CALL_VoiceDomainUint8 voiceDomain;   /* VOICE DOMAIN */
    VOS_UINT8                 reserved[2];
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    TAF_CALL_ErrorInfoText    errInfoText;                          /* �쳣��Ϣ�ı� */
} TAF_CALL_ConnectIndPara;


typedef struct {
    MN_CALL_ID_T              callId;      /* Call ID */
    MN_CALL_StateUint8        callState;   /* Call State */
    MN_CALL_TypeUint8         callType;    /* Call Type */
    MN_CALL_NoCliCauseUint8   noCliCause;  /* Cause of the NO CLI */
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    MN_CALL_DirUint8          callDir;     /* Call Direction(MO/MT) */
    VOS_UINT8                 reserved[2];
    TAF_CS_CauseUint32        cause;                                /* Cause Value of the Call Control */
    VOS_UINT32                preCallTime;                          /* �ϴ�ͨ��ʱ�� */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    TAF_CALL_ErrorInfoText    errInfoText;                          /* �쳣��Ϣ�ı� */
    MN_CALL_DIscDir           discDir;                              /* �Ҷϵ绰�ķ��� */
    TAF_CALL_EmcCallBackInfo  emcCallBackInfo;                      /* �������ز���Ϣ */
    VOS_UINT32                endSource;                            /* ���йҶ���Դ */
} TAF_CALL_ReleasedIndPara;


typedef struct {
    MN_CALL_ID_T            callId;     /* Call ID */
    MN_CALL_StateUint8      callState;  /* Call State */
    MN_CALL_TypeUint8       callType;   /* Call Type */
    MN_CALL_NoCliCauseUint8 noCliCause; /* Cause of the NO CLI */
    /* ALS Line number: 1 or 2,default is 1 */
    MN_CALL_AlsLineNoUint8    alsLineNo;
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 reserved[2];
    MN_CALL_BcdNum            callNumber;                           /* Call Number */
    MN_CALL_Subaddr           subCallNumber;                        /* Call SubAddress */
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    TAF_CALL_Cnap             nameIndicator;                        /* Name Indicator */
} TAF_CALL_IncomingIndPara;


typedef struct {
    TAF_CS_CauseUint32      cause; /* ������ */
    MN_CALL_ID_T            callId;
    TAF_CALL_DtmfStateUint8 dtmfState;
    VOS_UINT8               dtmfCnt;
    VOS_UINT8               reserved[1];
} TAF_CALL_StartDtmfPara;


typedef struct {
    TAF_CS_CauseUint32      cause; /* ������ */
    MN_CALL_ID_T            callId;
    TAF_CALL_DtmfStateUint8 dtmfState;
    VOS_UINT8               dtmfCnt;
    VOS_UINT8               reserved[1];
} TAF_CALL_StartDtmfRsltPara;


typedef struct {
    TAF_CS_CauseUint32      cause; /* ������ */
    MN_CALL_ID_T            callId;
    TAF_CALL_DtmfStateUint8 dtmfState;
    VOS_UINT8               dtmfCnt;
    VOS_UINT8               reserved[1];
} TAF_CALL_StopDtmfPara;


typedef struct {
    TAF_CS_CauseUint32      cause; /* ������ */
    MN_CALL_ID_T            callId;
    TAF_CALL_DtmfStateUint8 dtmfState;
    VOS_UINT8               dtmfCnt;
    VOS_UINT8               reserved[1];
} TAF_CALL_StopDtmfRsltPara;


typedef struct {
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 reserve[3];
    VOS_UINT8                 callNum;
    VOS_UINT8                 callId[MN_CALL_MAX_NUM];
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} TAF_CALL_HoldIndPara;


typedef struct {
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    VOS_UINT8                 reserve[3];
    VOS_UINT8                 callNum;
    VOS_UINT8                 callId[MN_CALL_MAX_NUM];
    VOS_UINT8                 curcRptCfg[MN_CALL_RPT_CFG_MAX_SIZE]; /* CURC���õ������ϱ���ʶ */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} TAF_CALL_RetrieveInd;


typedef struct {
    MN_CALL_ID_T              callId;      /* Call ID */
    MN_CALL_TypeUint8         callType;    /* Call Type */
    TAF_CALL_VoiceDomainUint8 voiceDomain; /* VOICE DOMAIN */
    MN_CALL_CfCauseUint8      callForwardCause;
    MN_CALL_HoldToneUint8     holdToneType;
    MN_CALL_StateUint8        callState; /* Call State */
    VOS_UINT8                 reserve[2];
    MN_CALL_BcdNum            callNumber;    /* Call Number */
    MN_CALL_BcdNum            connectNumber; /* Connect Number */
    MN_CALL_SsNotify          ssNotify;      /* SS Notify */
    TAF_SS_CcbsFeature        ccbsFeature;   /* CCBS feature info */
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} TAF_CALL_SsIndPara;


typedef struct {
    MN_CALL_ID_T       callId; /* Call ID */
    VOS_UINT8          reserve[3];
    TAF_CS_CauseUint32 cause; /* Cause Value of the Call Control */
    VOS_UINT32         curCallTime;
} TAF_CALL_QryCdurPara;


typedef struct {
    VOS_UINT32                ret;                                /* �ظ���� */
    VOS_UINT32                actNum;                             /* �û���������1�ĸ��� */
    MN_CALL_Uus1Info          uus1Info[MN_CALL_MAX_UUS1_MSG_NUM]; /* �û���������1����Ϣ */
    MN_CALL_SetUus1TypeUint32 setType[MN_CALL_MAX_UUS1_MSG_NUM];  /* Ҫ���õ��û���������1���� */
} TAF_CALL_QryUus1InfoPara;


typedef struct {
    MN_CALL_ID_T       callId;  /* Call ID */
    MN_CALL_DirUint8   callDir; /* Call Direction(MO/MT) */
    VOS_UINT8          reserve[2];
    TAF_CS_CauseUint32 cause; /* Cause Value of the Call Control */
    MN_CALL_Uus1Info   uusInfo;
    VOS_UINT32         curCallTime;
    /* �����������õ������ϱ���ʶ */
    VOS_UINT8 unsolicitedRptCfg[MN_CALL_RPT_CFG_MAX_SIZE];
} TAF_CALL_Uus1InfoIndPara;


typedef struct {
    VOS_UINT32             ret;         /* �ظ���� */
    MN_CALL_AlsLineNoUint8 alsLine;     /* ��ǰʹ�õ���·�� */
    VOS_UINT8              reserved[3]; /* �����ֶ� */
} TAF_CALL_QryAlsPara;


typedef struct {
    MN_CALL_ID_T       callId; /* Call ID */
    VOS_UINT8          reserved[3];
    TAF_CS_CauseUint32 cause;
} TAF_CALL_EconfDialPara;


typedef struct {
    VOS_UINT8               numOfCalls; /* ��������ͨ���ĸ��� */
    VOS_UINT8               reserved[3];
    TAF_CALL_EconfInfoParam callInfo[TAF_CALL_MAX_ECONF_CALLED_NUM]; /* ��������ͨ������Ϣ */
} TAF_CALL_EconfNotifyIndPara;


typedef struct {
    TAF_CS_CauseUint32 cause; /* Cause Value of the Call Control */
    /* VOS_TRUE:ata�첽�ϱ�ok������connect���ϱ�ok����������connect ack�� VOS_FALSE:ataͬ���ϱ�������connect����յ�����connect ack�����ϱ�ok */
    VOS_UINT8    ataReportOkAsyncFlag;
    MN_CALL_ID_T callId; /* Call ID */
    VOS_UINT8    reservd[2];
} TAF_CALL_SupsCmdPara;


typedef struct {
    MN_CALL_ID_T       callId; /* Call ID */
    VOS_UINT8          reserve[3];
    TAF_CS_CauseUint32 cause; /* Cause Value of the Call Control */
    VOS_UINT32         curCallTime;
} TAF_CALL_GetCdurPara;

/*
 * ö����: TAF_CALL_SupsCmdRsltInfo
 * �ṹ˵��: SUPS CMD RSLT��Ϣ�ṹ
 */
typedef struct {
    /* the result of the last Call Management Command */
    MN_CALL_SsResultUint8 ssResult;
    VOS_UINT8             reserved[3];
} TAF_CALL_SupsCmdRsltInfo;

VOS_UINT32 MN_CALL_CheckUus1ParmValid(MN_CALL_SetUus1TypeUint32 setType, MN_CALL_Uus1Info *uus1Info);

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

#endif

typedef struct {
    VOS_UINT8 result;
    VOS_UINT8 reserve[3];
} TAF_CALL_FlashCnfPara;

typedef struct {
    MN_CALL_ID_T                         callId;
    TAF_CALL_SendBurstDtmfCnfResultUint8 result;
    VOS_UINT8                            reserved[2];
} TAF_CALL_BurstDtmfCnfPara;
typedef struct {
    VOS_UINT8 digitNum;                           /* *<  Digit number */
    VOS_UINT8 digit[TAF_CALL_MAX_BURST_DTMF_NUM]; /* *<  DTMF digits */
    /* *<  refers to S.0005 3.7.3.3.2.9 */
    VOS_UINT32 onLength;
    /* *<  refers to S.0005 3.7.3.3.2.9 */
    VOS_UINT32 offLength;
} TAF_CALL_BurstDtmfIndPara;
typedef struct {
    TAF_CALL_SendContDtmfCnfResultUint8 result; /* ����BURST DTMF CNF�Ľ�� */
    VOS_UINT8                           reserved[3];
} TAF_CALL_ContDtmfCnfPara;
typedef struct {
    /* ucSwitch: TAF_CALL_CONT_DTMF_STOP,TAF_CALL_CONT_DTMF_START  */
    TAF_CALL_ContDtmfSwitchUint8 dtmfSwitch;
    VOS_UINT8                    digit; /* 1-9,0,*,# */
    VOS_UINT8                    reserved[2];
} TAF_CTRL_ContDtmfIndPara;
typedef struct {
    TAF_CALL_EncryptVoiceStatusUint32 eccVoiceStatus;
    TAF_ECC_CallBcdNum                callingNumber; /* Call Number */
} TAF_CALL_EncryptVoiceIndPara;
typedef struct {
    TAF_CALL_EccSrvCapUint32    eccSrvCap;
    TAF_CALL_EccSrvStatusUint32 eccSrvStatus;
} TAF_CALL_EccSrvCapQryCnfPara;
typedef struct {
    TAF_CALL_SetEcTestModeUint32 eccTestModeStatus;
    VOS_UINT32                   result;
} TAF_CALL_GetEcTestModeCnfPara;
typedef struct {
    VOS_INT8  eccData[TAF_CALL_APP_EC_RANDOM_DATA_LENGTH];
    VOS_UINT8 reserved[3];
} TAF_CALL_AppEcRandomData;
typedef struct {
    VOS_UINT32               result;
    TAF_CALL_AppEcRandomData eccRandom[TAF_CALL_APP_EC_RANDOM_NUM];
} TAF_CALL_GetEcRandomCnfPara;
typedef struct {
    MN_CALL_AppEcKmcData kmcData;
    VOS_UINT32           result;
} TAF_CALL_GetEcKmcCnfPara;
typedef struct {
    MN_CALL_AppEcKmcData kmcData;
    VOS_UINT32           result;
} TAF_CALL_SetKmcCnfPara;
typedef struct {
    VOS_UINT16 length;
    VOS_UINT8  rsved;
    VOS_UINT8  isTimeInfo;
    VOS_INT8   data[TAF_CALL_APP_ENCRYPTED_VOICE_DATA_MAX_LENGTH];
} TAF_CALL_EncryptedVoiceDataIndPara;
typedef struct {
    TAF_CALL_PrivacyModeUint8    privacyMode; /* ��ǰprivacy mode���� */
    VOS_UINT8                    reserved[3];
    TAF_CALL_CallPrivacyModeInfo callVoicePrivacyInfo; /* ��ǰ���е�privacy mode��Ϣ */
} TAF_CALL_PrivacyModeIndPara;
typedef struct {
    TAF_CALL_PrivacyModeUint8    privacyMode; /* ��ǰprivacy mode���� */
    VOS_UINT8                    callNums;
    VOS_UINT8                    reserved[2];
    TAF_CALL_CallPrivacyModeInfo callVoicePrivacyInfo[TAF_XCALL_MAX_NUM];
} TAF_CALL_PrivacyModeQryCnfPara;
typedef struct {
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                signalType;
    VOS_UINT8                alertPitch;
    VOS_UINT8                signal;
    VOS_UINT8                signalIsPresent;
    VOS_UINT8                rsved;
    VOS_UINT8                digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM + 1];
} TAF_CALL_CcwacInfoIndPara;
typedef struct {
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                digitNum;
    VOS_UINT8                digit[TAF_CALL_MAX_CALLED_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_CalledNumInfoIndPara;
typedef struct {
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[2];
    VOS_UINT8                digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_CallingNumInfoIndPara;
typedef struct {
    VOS_UINT8 callId;
    VOS_UINT8 digitNum;
    VOS_UINT8 rsved[2];
    VOS_UINT8 digit[TAF_CALL_MAX_DISPALY_CHARI_OCTET_NUM];
} TAF_CALL_DisplayInfoIndPara;
typedef struct {
    VOS_UINT8                      callId;
    VOS_UINT8                      infoRecsDataNum;
    VOS_UINT8                      displayType;
    VOS_UINT8                      extDispInd;
    TAF_CALL_EvtExtDisplayInfoData infoRecsData[TAF_CALL_MAX_EXT_DISPLAY_DATA_NUM];
} TAF_CALL_ExtDisplayInfoIndPara;
typedef struct {
    VOS_UINT8                callId;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                digitNum;
    VOS_UINT8                rsved[2];
    VOS_UINT8                digit[TAF_CALL_MAX_CONNECTED_NUMBER_CHARI_OCTET_NUM];
} TAF_CALL_ConnNumInfoIndPara;
typedef struct {
    VOS_UINT32               opPi : 1;
    VOS_UINT32               opSi : 1;
    VOS_UINT32               opRedirReason : 1;
    VOS_UINT32               opSpare : 29;
    TAF_CALL_NumberTypeUint8 numType;
    TAF_CALL_NumberPlanUint8 numPlan;
    VOS_UINT8                pi;
    VOS_UINT8                si;
    VOS_UINT8                redirReason;
    VOS_UINT8                digitNum;
    VOS_UINT8                callId;
    VOS_UINT8                rsved;
    VOS_UINT8                digitNumArray[TAF_CALL_MAX_REDIRECTING_NUMBER_CHARI_OCTET_NUM];
} TAF_REDIR_NumInfoIndPara;
typedef struct {
    VOS_UINT8 callId;
    VOS_UINT8 polarityIncluded;
    VOS_UINT8 toggleModePresent;
    VOS_UINT8 toggleMode;
    VOS_UINT8 reversePolarityPresent;
    VOS_UINT8 reversePolarity;
    VOS_UINT8 powerDenialTime;
    VOS_UINT8 reserved;
} TAF_CALL_LineCtrlInfoIndPara;
typedef struct {
    VOS_UINT8 signalType;
    VOS_UINT8 alertPitch;
    VOS_UINT8 signal;
    VOS_UINT8 callId;
} TAF_CALL_SignalInfoIndPara;
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* MN_CALL_API_H */
