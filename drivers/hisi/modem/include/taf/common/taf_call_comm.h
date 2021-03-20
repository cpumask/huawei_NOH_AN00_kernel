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

#ifndef __TAF_CALL_COMM_H__
#define __TAF_CALL_COMM_H__

#include "vos.h"
#include "taf_type_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* 最大呼叫个数 */
#define MN_CALL_MAX_NUM (7)
#define TAF_CALL_MAX_ECONF_CALLED_NUM (5)
#define TAF_IMSA_ALPHA_STRING_SZ (129)

#define MN_CALL_DISPLAY_NAME_STRING_SZ (129)

#define TAF_CALL_ERROR_INFO_TEXT_STRING_SZ (257)
/* 10.5.4.8 Called party subaddress */
#define MN_CALL_MAX_SUBADDR_INFO_LEN (20)

#define SRVCC_CALL_DTMF_REQ_MSG_MAX_NUM (16)
#define MN_CALL_EENL_URN_LEN 64

/* 把原有的枚举定义进行调整 */
/* TAF_CS_CAUSE需与NAS_CC_CAUSE对应 添加时注意 */
#define TAF_CS_CAUSE_CC_NW_SECTION_BEGIN (0x0000)
#define TAF_CS_CAUSE_CM_SRV_REJ_BEGIN (0x0100)
#define TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN (0x0200)
#define TAF_CS_CAUSE_SMS_CP_ERR_BEGIN (0x0300)
#define TAF_CS_CAUSE_SMS_RP_ERR_BEGIN (0x0400)
#define TAF_CS_CAUSE_SS_NW_REJ_BEGIN (0x0500)
#define TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN (0x0600)
#define TAF_CS_CAUSE_RR_REL_BEGIN (0x0700)
#define TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN (0x0800)
#define TAF_CS_CAUSE_MM_INTER_ERR_BEGIN (0x0900)
#define TAF_CS_CAUSE_CALL_BEGIN (0x0A00)
#define TAF_CS_CAUSE_MSG_INTER_ERR_BEGIN (0x0B00)
#define TAF_CS_CAUSE_SS_INTER_ERR_BEGIN (0x0C00)
#define TAF_CS_CAUSE_VC_BEGIN (0x0D00)
#define TAF_CS_CAUSE_IMS_BEGIN (0x4800)
#define TAF_CS_CAUSE_IMSA_BEGIN (0x4C00)
#define TAF_CS_CAUSE_XCALL_BEGIN (0x9000)

enum MN_CALL_AlsLineNo {
    MN_CALL_ALS_LINE_NO_1 = 1,
    MN_CALL_ALS_LINE_NO_2 = 2,
    MN_CALL_ALS_LINE_NO_MAX,
    MN_CALL_ALS_LINE_NO_BUTT
};
typedef VOS_UINT8 MN_CALL_AlsLineNoUint8;

/* Call Number Type  */
/*
 * 填充方式如下
 * |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |
 * |   1   |    Type of number     | Numbering plan identification |
 */
enum MN_CALL_Exit {
    MN_CALL_IS_EXIT = 0x80, /* 000: unknown */
    MN_CALL_EXIT_BUTT
};
typedef VOS_UINT8 MN_CALL_ExitUint8;

/* Type of Number,5,6,7bit of the MN_CALL_NUM_TYPE_ENUM_U8 */
enum MN_CALL_Ton {
    MN_CALL_TON_UNKNOWN,       /* 000: unknown */
    MN_CALL_TON_INTERNATIONAL, /* 001: international number */
    MN_CALL_TON_NATIONAL,      /* 010: national number */
    MN_CALL_TON_NETWORK_SPEC,  /* 011: network specific number */
    /* 100: dedicated access, short code */
    MN_CALL_TON_SHORT_CODE,
    MN_CALL_TON_BUTT
};
typedef VOS_UINT8 MN_CALL_TonUint8;


enum TAF_CALL_VoiceDomain {
    TAF_CALL_VOICE_DOMAIN_3GPP  = 0,
    TAF_CALL_VOICE_DOMAIN_IMS   = 1,
    TAF_CALL_VOICE_DOMAIN_3GPP2 = 2,

    TAF_CALL_VOICE_DOMAIN_AUTO = 3,

    TAF_CALL_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_CALL_VoiceDomainUint8;


enum TAF_CALL_EmcDomain {
    TAF_CALL_EMC_DOMAIN_CS       = 0, /* CS域紧急呼 */
    TAF_CALL_EMC_DOMAIN_IMS_3GPP = 1, /* IMS 3GPP域紧急呼 */
    TAF_CALL_EMC_DOMAIN_IMS_WIFI = 2, /* IMS WIFI域紧急呼 */
    TAF_CALL_EMC_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_CALL_EmcDomainUint8;


enum TAF_CALL_ServiceType {
    TAF_CALL_SERVICE_TYPE_NONE = 0, /* 非SUSPECTW CALL */
    TAF_CALL_SERVICE_TYPE_SUSPECTW  = 1, /* SUSPECTW CALL */
    TAF_CALL_SERVICE_TYPE_TEST = 2, /* 用运营商的测试号码做呼叫 */

    TAF_CALL_SERVICE_TYPE_MIEC = 3,
    TAF_CALL_SERVICE_TYPE_AIEC = 4,
    TAF_CALL_SERVICE_TYPE_TEST_ECALL = 5,
    TAF_CALL_SERVICE_TYPE_RECFGURATION_ECALL = 6,
    TAF_CALL_SERVICE_TYPE_PSAP_ECALL = 7,

    TAF_CALL_SERVICE_TYPE_BUTT
};
typedef VOS_UINT8 TAF_CALL_ServiceTypeUint8;


enum TAF_CALL_ECallNumberType {
    TAF_CALL_ECALL_NUMBER_TYPE_USER,
    TAF_CALL_ECALL_NUMBER_TYPE_SIM_CS = 1, /* CS ECall number */
    TAF_CALL_ECALL_NUMBER_TYPE_SIM_IMS, /* IMS ECall number */

    TAF_CALL_ECALL_NUMBER_TYPE_BUTT
};
typedef VOS_UINT8 TAF_CALL_ECallNumberTypeUint8;


enum TAF_CALL_PeerVideoSupport {
    TAF_CALL_PEER_VIDEO_UNSUPPORT = 0, /* 对端视频能力不支持 */
    TAF_CALL_PEER_VIDEO_SUPPORT   = 1, /* 对端视频能力支持 */
    TAF_CALL_PEER_VIDEO_BUTT
};
typedef VOS_UINT8 TAF_CALL_PeerVideoSupportUint8;


enum TAF_CALL_ImsDomain {
    TAF_CALL_IMS_DOMAIN_NULL = 0, /* 不是IMS电话 */
    TAF_CALL_IMS_DOMAIN_LTE  = 1, /* IMS电话在VOLTE上 */
    TAF_CALL_IMS_DOMAIN_WIFI = 2, /* IMS电话在VOWIFI上 */
    TAF_CALL_IMS_DOMAIN_NR   = 3, /* IMS电话在NR上 */
    TAF_CALL_IMS_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_CALL_ImsDomainUint8;


enum MN_CALL_380CsCallType {
    MN_CALL_380_CS_CALL_TYPE_NORMAL    = 0x0,
    MN_CALL_380_CS_CALL_TYPE_EMERGENCY = 0x1,

    /* 如果IMSA该值填为BUTT时，表示该字段无效，SPM仅需要根据切换的类型做重拨即可 */
    MN_CALL_380_CS_CALL_TYPE_BUTT
};

typedef VOS_UINT8 MN_CALL_380CsCallTypeUint8;

typedef VOS_UINT8 MN_CALL_ID_T;

enum MN_CALL_Type {
    MN_CALL_TYPE_VOICE,         /* normal voice call */
    MN_CALL_TYPE_VIDEO_TX,      /* Tx video call, Two way voice */
    MN_CALL_TYPE_VIDEO_RX,      /* Rx video call, Two way voice */
    MN_CALL_TYPE_VIDEO,         /* video call */
    MN_CALL_TYPE_FAX,           /* group 3 fax */
    MN_CALL_TYPE_CS_DATA,       /* CS data call */
    MN_CALL_TYPE_EMERGENCY = 9, /* emergency call */
    MN_CALL_TYPE_MIEC,          /* manually initiated ecall */
    MN_CALL_TYPE_AIEC,          /* automatic initiated ecall */
    MN_CALL_TYPE_TEST,          /* test ecall */
    MN_CALL_TYPE_RECFGURATION,  /* reconfiguration call */
    /* 设置PSAP回呼的呼叫类型，且收到MSD传输请求 */
    MN_CALL_TYPE_PSAP_ECALL,
    /* 设置PSAP回呼的呼叫类型，未收到MSD传输请求 */
    MN_CALL_TYPE_PSAP_CALL,
    MN_CALL_TYPE_BUTT
};
typedef VOS_UINT8 MN_CALL_TypeUint8;

/* 紧急呼叫的Category,3gpp 31102中定义 */
typedef enum MN_CALL_EMER_CATEGORY_TYPE {
    MN_CALL_EMER_CATEGORG_POLICE          = 0x01,
    MN_CALL_EMER_CATEGORG_AMBULANCE       = 0x02,
    MN_CALL_EMER_CATEGORG_FIRE_BRIGADE    = 0x04,
    MN_CALL_EMER_CATEGORG_MARINE_GUARD    = 0x08,
    MN_CALL_EMER_CATEGORG_MOUNTAIN_RESCUE = 0x10,
    MN_CALL_EMER_CATEGORG_MAN_INIT_ECALL  = 0x20,
    MN_CALL_EMER_CATEGORG_AUTO_INIT_ECALL = 0x40,

    MN_CALL_EMER_EXT_CATEGORG_NATIONAL_INTELLIGENCE_SERVICE = 0x06,
    MN_CALL_EMER_EXT_CATEGORG_SPY_REPORT                    = 0x03,
    MN_CALL_EMER_EXT_CATEGORG_SCHOOL_VIOLENCE_REPORT        = 0x12,
    MN_CALL_EMER_EXT_CATEGORG_CYBER_TERRORISM_REPORT        = 0x13,
    MN_CALL_EMER_EXT_CATEGORG_CONTRABANDS_REPORT            = 0x09,

    MN_CALL_EMER_CATEGORG_MAX = 0x80,

    MN_CALL_EMER_CATEGORG_ERR_VALUE = 0xFF,

} MN_CALL_EmerCategoryType;
typedef VOS_UINT8 MN_CALL_EmerCategorgTypeUint8;

enum MN_CALL_ExState {
    MN_CALL_S_ACTIVE,              /* active */
    MN_CALL_S_HELD,                /* held */
    MN_CALL_S_DIALING,             /* dialing (MO call) */
    MN_CALL_S_ALERTING,            /* alerting (MO call) */
    MN_CALL_S_INCOMING,            /* incoming (MT call) */
    MN_CALL_S_WAITING,             /* waiting (MT call) */
    MN_CALL_S_IDLE,                /* idle */
    MN_CALL_S_CCBS_WAITING_ACTIVE, /* CCBS等待激活态 */
    MN_CALL_S_CCBS_WAITING_RECALL, /* CCBS等待回呼态 */
    MN_CALL_S_UNKNOWN,             /* unknown state */

    /* 该状态为SRVCC过程前用户接听失败，成功后在GU下发送connect */
    MN_CALL_S_WAITING_ACCEPT,
    MN_CALL_S_WAITING_SRV_ACQ,  /* 等待业务捕获确认 */
    MN_CALL_S_WAITING_RF_AVAIL, /* 等待RF资源可用 */

    MN_CALL_S_BUTT
};
typedef VOS_UINT8 MN_CALL_StateUint8;

enum MN_CALL_MptyState {
    MN_CALL_NOT_IN_MPTY,
    MN_CALL_IN_MPTY,
    MN_CALL_MPYT_STATE_BUTT
};
typedef VOS_UINT8 MN_CALL_MptyStateUint8;

/* call direction */
enum MN_CALL_Dir {
    MN_CALL_DIR_MO,   /* MO CALL */
    MN_CALL_DIR_MT,   /* MT CALL */
    MN_CALL_DIR_CCBS, /* CCBS CALL */
    MN_CALL_DIR_BUTT
};
typedef VOS_UINT8 MN_CALL_DirUint8;

/* see 3GPP 27.007 +CBST, para speed */
enum MN_CALL_CsDataSpeed {
    MN_CALL_CSD_SPD_AUTOBAUD   = 0,
    MN_CALL_CSD_SPD_300_V21    = 1,
    MN_CALL_CSD_SPD_1K2_V22    = 2,
    MN_CALL_CSD_SPD_2K4_V22BIS = 4,
    MN_CALL_CSD_SPD_2K4_V26TER = 5,
    MN_CALL_CSD_SPD_4K8_V32    = 6,
    MN_CALL_CSD_SPD_9K6_V32    = 7,
    MN_CALL_CSD_SPD_9K6_V34    = 12,
    MN_CALL_CSD_SPD_14K4_V34   = 14,
    MN_CALL_CSD_SPD_19K2_V34   = 15,
    MN_CALL_CSD_SPD_28K8_V34   = 16,
    MN_CALL_CSD_SPD_33K6_V34   = 17,
    MN_CALL_CSD_SPD_1K2_V120   = 34,
    MN_CALL_CSD_SPD_2K4_V120   = 36,
    MN_CALL_CSD_SPD_4K8_V120   = 38,
    MN_CALL_CSD_SPD_9K6_V120   = 39,
    MN_CALL_CSD_SPD_14K4_V120  = 43,
    MN_CALL_CSD_SPD_19K2_V120  = 47,
    MN_CALL_CSD_SPD_28K8_V120  = 48,
    MN_CALL_CSD_SPD_38K4_V120  = 49,
    MN_CALL_CSD_SPD_48K_V120   = 50,
    MN_CALL_CSD_SPD_56K_V120   = 51,
    MN_CALL_CSD_SPD_300_V110   = 65,
    MN_CALL_CSD_SPD_1K2_V110   = 66,
    MN_CALL_CSD_SPD_2K4_V110   = 68,
    MN_CALL_CSD_SPD_4K8_V110   = 70,
    MN_CALL_CSD_SPD_9K6_V110   = 71,
    MN_CALL_CSD_SPD_14K4_V110  = 75,
    MN_CALL_CSD_SPD_19K2_V110  = 79,
    MN_CALL_CSD_SPD_28K8_V110  = 80,
    MN_CALL_CSD_SPD_38K4_V110  = 81,
    MN_CALL_CSD_SPD_48K_V110   = 82,
    MN_CALL_CSD_SPD_56K_FTM    = 83,
    MN_CALL_CSD_SPD_64K_FTM    = 84,
    MN_CALL_CSD_SPD_56K_BT     = 115,
    MN_CALL_CSD_SPD_64K_BT     = 116,
    MN_CALL_CSD_SPD_32K_PIAFS  = 120,
    MN_CALL_CSD_SPD_64K_PIAFS  = 121,
    MN_CALL_CSD_SPD_64K_MULTI  = 134,
    MN_CALL_CSD_SPD_BUTT
};
typedef VOS_UINT8 MN_CALL_CsDataSpeedUint8;

/* see 3GPP 27.007 +CBST, para name */
enum MN_CALL_CsDataName {
    /* data circuit asynchronous (UDI or 3.1 kHz modem) */
    MN_CALL_CSD_NAME_ASYNC_UDI,
    /* data circuit synchronous (UDI or 3.1 kHz modem)  */
    MN_CALL_CSD_NAME_SYNC_UDI,
    MN_CALL_CSD_NAME_ASYNC_RDI, /* data circuit asynchronous (RDI) */
    MN_CALL_CSD_NAME_SYNC_RDI,  /* data circuit synchronous (RDI) */
    MN_CALL_CSD_NAME_BUTT
};
typedef VOS_UINT8 MN_CALL_CsDataNameUint8;

/* see 3GPP 27.007 +CBST, para CE(Connection element) */
enum MN_CALL_CsDataCe {
    MN_CALL_CSD_CE_T,      /* transparent  */
    MN_CALL_CSD_CE_NT,     /* non-transparent  */
    MN_CALL_CSD_CE_BOTH_T, /* both, transparent preferred  */
    /* both, non-transparent preferred  */
    MN_CALL_CSD_CE_BOTH_NT,
    MN_CALL_CSD_CE_BUTT
};
typedef VOS_UINT8 MN_CALL_CsDataCeUint8;

/* for CS DATA service configration */
typedef struct {
    MN_CALL_CsDataSpeedUint8 speed;
    MN_CALL_CsDataNameUint8  name;
    MN_CALL_CsDataCeUint8    connElem;
    VOS_UINT8                reserved[5];
} MN_CALL_CsDataCfg;

enum MN_CALL_CsDataSpdSimple {
    MN_CALL_CSD_SPD_300,
    MN_CALL_CSD_SPD_1K2,
    MN_CALL_CSD_SPD_2K4,
    MN_CALL_CSD_SPD_4K8,
    MN_CALL_CSD_SPD_9K6,
    MN_CALL_CSD_SPD_12K,
    MN_CALL_CSD_SPD_14K4,
    MN_CALL_CSD_SPD_19K2,
    MN_CALL_CSD_SPD_28K8,
    MN_CALL_CSD_SPD_31K2,
    MN_CALL_CSD_SPD_32K,
    MN_CALL_CSD_SPD_33K6,
    MN_CALL_CSD_SPD_38K4,
    MN_CALL_CSD_SPD_48K,
    MN_CALL_CSD_SPD_56K,
    MN_CALL_CSD_SPD_64K,
    MN_CALL_CSD_SPD_SIMPLE_BUTT
};
typedef VOS_UINT8 MN_CALL_CsDataSpdSimpleUint8;

/* for report CS DATA service configration */
typedef struct {
    MN_CALL_CsDataSpdSimpleUint8 speed;
    MN_CALL_CsDataNameUint8      name;
    MN_CALL_CsDataCeUint8        connElem;
    VOS_UINT8                    reserved1[5];
} MN_CALL_CsDataCfgInfo;

/* 3GPP 27.007 +CMOD */
enum MN_CALL_Mode {
    MN_CALL_MODE_SINGLE, /* single mode  */
    MN_CALL_MODE_AVF,    /* alternating voice/fax */
    MN_CALL_MODE_BUTT
};
typedef VOS_UINT8 MN_CALL_ModeUint8;

enum MN_CALL_ClirCfg {
    MN_CALL_CLIR_AS_SUBS,  /* CLIR AS SUBSCRIBE */
    MN_CALL_CLIR_INVOKE,   /* CLIR invocation */
    MN_CALL_CLIR_SUPPRESS, /* CLIR suppression  */
    MN_CALL_CLIR_BUTT
};
typedef VOS_UINT8 MN_CALL_ClirCfgUint8;

typedef struct {
    VOS_BOOL   enable;         /* if enable cug */
    VOS_BOOL   suppressPrefer; /* suppress preferential CUG */
    VOS_BOOL   suppressOa;     /* suppress OA(Outgoing Access) */
    VOS_BOOL   indexPresent;
    VOS_UINT32 index; /* CUG Index */
    VOS_UINT8  reserved[4];
} MN_CALL_CugCfg;

/* 3GPP 27.007 +CHLD */
enum MN_CALL_SupsCmd {
    /* Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call */
    MN_CALL_SUPS_CMD_REL_HELD_OR_UDUB,
    /* Releases all active calls (if any exist) and accepts the other (held or waiting) call */
    MN_CALL_SUPS_CMD_REL_ACT_ACPT_OTH,
    /* Releases a specific active call X */
    MN_CALL_SUPS_CMD_REL_CALL_X,
    /* Releases all active call and held call and waiting call */
    MN_CALL_SUPS_CMD_REL_ALL_CALL,
    /* Places all active calls (if any exist) on hold and accepts the other (held or waiting) call */
    MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH,
    /* Places all active calls on hold except call X with which communication shall be supported */
    MN_CALL_SUPS_CMD_HOLD_ALL_EXCPT_X,
    /* Adds a held call to the conversation */
    MN_CALL_SUPS_CMD_BUILD_MPTY,
    /* Connects the two calls and disconnects the subscriber from both calls (ECT) */
    MN_CALL_SUPS_CMD_ECT,
    /* Redirect an incoming or a waiting call to the specified followed by SEND directory number */
    MN_CALL_SUPS_CMD_DEFLECT_CALL,
    /* Activates the Completion of Calls to Busy Subscriber Request */
    MN_CALL_SUPS_CMD_ACT_CCBS,
    /* Releases all calls (if any exist) except waiting call */
    MN_CALL_SUPS_CMD_REL_ALL_EXCEPT_WAITING_CALL,
    MN_CALL_SUPS_CMD_REL_HELD,         /* Releases all held calls */
    MN_CALL_SUPS_CMD_REL_ACTIVE,       /* Releases all active calls */
    MN_CALL_SUPS_CMD_REL_ECALL,        /* Releases eCall */
    MN_CALL_SUPS_CMD_ECONF_REL_USER,   /* Releases member in econf  */
    MN_CALL_SUPS_CMD_ECONF_MERGE_CALL, /* Merge econf and normal call */
    MN_CALL_SUPS_CMD_ACPT_WAITING_CALL,
    MN_CALL_SUPS_CMD_REL_INCOMING_OR_WAITING,

    MN_CALL_SUPS_CMD_BUTT
};
typedef VOS_UINT8 MN_CALL_SupsCmdUint8;

/* Call relate SS Command Execute Result */
enum MN_CALL_SsResult {
    MN_CALL_SS_RES_SUCCESS,
    MN_CALL_SS_RES_FAIL,
    MN_CALL_SS_RES_BUTT
};
typedef VOS_UINT8 MN_CALL_SsResultUint8;

/* Call relate SS Error Code */
enum MN_CALL_SsErrCode {
    MN_CALL_ERR_SS_UNSPECIFIC                      = 0,   /* UNSPECIFIC Error */
    MN_CALL_ERR_SS_ILLEGAL_SUBSCRIBER              = 9,   /* IllegalSubscriber */
    MN_CALL_ERR_SS_ILLEGAL_EQUIPMENT               = 12,  /* IllegalEquipment */
    MN_CALL_ERR_SS_CALL_BARRED                     = 13,  /* CallBarred */
    MN_CALL_ERR_SS_FORWARDING_VIOLATION            = 14,  /* ForwardingViolation */
    MN_CALL_ERR_SS_ILLEGALSS_OPERATION             = 16,  /* IllegalSS-Operation */
    MN_CALL_ERR_SS_ERROR_STATUS                    = 17,  /* SS-ErrorStatus */
    MN_CALL_ERR_SS_NOT_AVAILABLE                   = 18,  /* SS-NotAvailable */
    MN_CALL_ERR_SS_SUBSCRIPTION_VIOLATION          = 19,  /* SS-SubscriptionViolation */
    MN_CALL_ERR_SS_INCOMPATIBILITY                 = 20,  /* SS-Incompatibility */
    MN_CALL_ERR_SS_FACILITY_NOT_SUPPORT            = 21,  /* FacilityNotSupported */
    MN_CALL_ERR_SS_ABSENT_SUBSCRIBER               = 27,  /* AbsentSubscriber */
    MN_CALL_ERR_SS_INCOMPATIBLE_TERMINAL           = 28,  /* IncompatibleTerminal */
    MN_CALL_ERR_SS_SHORT_TERM_DENIAL               = 29,  /* ShortTermDenial */
    MN_CALL_ERR_SS_LONG_TERM_DENIAL                = 30,  /* LongTermDenial */
    MN_CALL_ERR_SS_SYSTEM_FAILURE                  = 34,  /* SystemFailure */
    MN_CALL_ERR_SS_DATA_MISSING                    = 35,  /* DataMissing */
    MN_CALL_ERR_SS_UNEXPECTED_DATA_VALUE           = 36,  /* SystemFailure */
    MN_CALL_ERR_SS_FORWARDING_FAILED               = 47,  /* ForwardingFailed */
    MN_CALL_ERR_SS_RESOURCE_LIMITATION             = 51,  /* ResourceLimitation */
    MN_CALL_ERR_SS_DEFLECTION_TO_SERVED_SUBSCRIBER = 123, /* DeflectionToServedSubscriber  */
    MN_CALL_ERR_SS_SPECIAL_SERVICE_CODE            = 124, /* specialServiceCode */
    MN_CALL_ERR_SS_INVALID_DEFLECTED_TO_NUM        = 125, /* InvalidDeflectedToNumber */
    /* MaxNumberOfMPTY-ParticipantsExceeded */
    MN_CALL_ERR_SS_MAX_NUM_MPTY_EXCEEDED  = 126,
    MN_CALL_ERR_SS_RESOURCE_NOT_AVAILABLE = 127, /* resourcesNotAvailable */
    MN_CALL_ERR_SS_BUTT
};
typedef VOS_UINT8 MN_CALL_SsErrCodeUint8;

/* the reason for no num of the incoming call */
enum MN_CALL_NoCliCause {
    MN_CALL_NO_CLI_UNAVAL,   /* num unavailable */
    MN_CALL_NO_CLI_USR_REJ,  /* user reject to provide num */
    MN_CALL_NO_CLI_INTERACT, /* interact by other service */
    MN_CALL_NO_CLI_PAYPHONE, /* Pay Phone */
    MN_CALL_NO_CLI_BUTT
};
typedef VOS_UINT8 MN_CALL_NoCliCauseUint8;

/* 3GPP 27.007 +CSSI/+CSSU */
enum MN_CALL_SsNotifyCode {
    /* unconditional call forwarding is active  */
    MN_CALL_SS_NTFY_UNCOND_FWD_ACTIVE,
    /* some of the conditional call forwardings are active  */
    MN_CALL_SS_NTFY_COND_FWD_ACTIVE,
    MN_CALL_SS_NTFY_BE_FORWORDED, /* call has been forwarded  */
    MN_CALL_SS_NTFY_IS_WAITING,   /* call is waiting */
    /* this is a CUG call (also <index> present)  */
    MN_CALL_SS_NTFY_MO_CUG_INFO,
    MN_CALL_SS_NTFY_OUTGOING_BARRED,   /* outgoing calls are barred */
    MN_CALL_SS_NTFY_INCOMING_BARRED,   /* incoming calls are barred  */
    MN_CALL_SS_NTFY_CLIR_SUPPRESS_REJ, /* CLIR suppression rejected */
    MN_CALL_SS_NTFY_BE_DEFLECTED,      /* call has been deflected  */
    /* this is a forwarded call (MT call setup)  */
    MN_CALL_SS_NTFY_FORWORDED_CALL,
    /* this is a CUG call (also <index> present) (MT call setup)  */
    MN_CALL_SS_NTFY_MT_CUG_INFO,
    /* call has been put on hold (during a voice call)  */
    MN_CALL_SS_NTFY_ON_HOLD,
    /* call has been retrieved (during a voice call) */
    MN_CALL_SS_NTFY_RETRIEVED,
    /* multiparty call entered (during a voice call) */
    MN_CALL_SS_NTFY_ENTER_MPTY,
    /* this is a deflected call (MT call setup)  */
    MN_CALL_SS_NTFY_DEFLECTED_CALL,
    /* Explicit Call Transfer (ECT) Supplementary Service */
    MN_CALL_SS_NTFY_EXPLICIT_CALL_TRANSFER,
    MN_CALL_SS_NTFY_CCBS_RECALL,      /* this is a CCBS recall */
    MN_CALL_SS_NTFY_CCBS_BE_RECALLED, /* call has been CCBS recalled */
    /* additional incoming call forwarded */
    MN_CALL_SS_NTFY_INCALL_FORWARDED,
    /* In held state or held, call release */
    MN_CALL_SS_NTFY_HOLD_RELEASED,
    MN_CALL_SS_NTFY_BUTT
};
typedef VOS_UINT8 MN_CALL_SsNotifyCodeUint8;

/* Numbering plan identification,1,2,3,4bit fo the MN_CALL_NUM_TYPE_ENUM_U8 */
enum MN_CALL_Npi {
    MN_CALL_NPI_UNKNOWN = 0, /* 0000: unknown */
    /* 0001: ISDN/telephony numbering plan  */
    MN_CALL_NPI_ISDN,
    MN_CALL_NPI_DATA     = 3, /* 0011: data numbering plan  */
    MN_CALL_NPI_TELEX    = 4, /* 0100: telex numbering plan  */
    MN_CALL_NPI_NATIONAL = 8, /* 1000: national numbering plane */
    MN_CALL_NPI_PRIVATE,      /* 1001: private numbering plan */
    MN_CALL_NPI_BUTT
};
typedef VOS_UINT8 MN_CALL_NpiUint8;
typedef VOS_UINT8 MN_CALL_NumTypeUint8;

/* Call Number Structure */
#define MN_CALL_MAX_BCD_NUM_LEN 20
typedef struct {
    MN_CALL_NumTypeUint8 numType;
    VOS_UINT8            numLen;
    VOS_UINT8            reserved[2];
    VOS_UINT8            bcdNum[MN_CALL_MAX_BCD_NUM_LEN];
} MN_CALL_BcdNum;

enum MN_CALL_EctCallState {
    MN_CALL_ECT_STATE_ALERTING,
    MN_CALL_ECT_STATE_ACTIVE,
    MN_CALL_ECT_STATE_BUTT
};
typedef VOS_UINT8 MN_CALL_EctCallStateUint8;

enum MN_CALL_EctRdn {
    MN_CALL_ECT_RDN_PRESENTATION_ALLOWED_ADDR, /* presentationAllowedAddress */
    MN_CALL_ECT_RDN_PRESENTATION_RESTRICTED,   /* presentationRestricted */
    /* numberNotAvailableDueToInterworking */
    MN_CALL_ECT_RDN_NUM_NOT_AVAILABLE,
    MN_CALL_ECT_RDN_PRESENTATION_RESTRICTED_ADDR, /* presentationRestrictedAddress */
    MN_CALL_ECT_RDN_BUTT
};
typedef VOS_UINT8 MN_CALL_EctRdnUint8;

typedef struct {
    MN_CALL_EctCallStateUint8 ectCallState;
    MN_CALL_EctRdnUint8       ectRdnType;
    VOS_UINT8                 reserved[2];

    union {
        MN_CALL_BcdNum presentationAllowedAddr;
        MN_CALL_BcdNum presentationRestrictedAddr;
    } rdn;

} MN_CALL_EctInd;

typedef struct {
    MN_CALL_SsNotifyCodeUint8 code;
    VOS_UINT8                 reserve1[3];
    /* CUG Index，is valid while Code is MO_CUG_INFO or MT_CUG_INFO */
    VOS_UINT32     cugIndex;
    MN_CALL_EctInd ectIndicator; /*  */
} MN_CALL_SsNotify;


enum TAF_CS_Cause {
    TAF_CS_CAUSE_SUCCESS = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 0),

    /* CS域网络上报的错误原因值 */
    /*   1. Unassigned (unallocated) number            */
    TAF_CS_CAUSE_CC_NW_UNASSIGNED_CAUSE            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 1),
    TAF_CS_CAUSE_CC_NW_NO_ROUTE_TO_DEST            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 3),
    TAF_CS_CAUSE_CC_NW_CHANNEL_UNACCEPTABLE        = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 6),
    TAF_CS_CAUSE_CC_NW_OPERATOR_DETERMINED_BARRING = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 8),
    TAF_CS_CAUSE_CC_NW_NORMAL_CALL_CLEARING        = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 16),
    TAF_CS_CAUSE_CC_NW_USER_BUSY                   = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 17),
    TAF_CS_CAUSE_CC_NW_NO_USER_RESPONDING          = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 18),
    /*  19. User alerting, no answer                   */
    TAF_CS_CAUSE_CC_NW_USER_ALERTING_NO_ANSWER                         = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 19),
    TAF_CS_CAUSE_CC_NW_CALL_REJECTED                                   = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 21),
    TAF_CS_CAUSE_CC_NW_NUMBER_CHANGED                                  = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 22),
    TAF_CS_CAUSE_CC_NW_CALL_REJECTED_DUE_TO_FEATURE_AT_THE_DESTINATION = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 24),
    /*  25. Pre-emption                                */
    TAF_CS_CAUSE_CC_NW_PRE_EMPTION                = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 25),
    TAF_CS_CAUSE_CC_NW_NON_SELECTED_USER_CLEARING = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 26),
    TAF_CS_CAUSE_CC_NW_DESTINATION_OUT_OF_ORDER   = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 27),
    TAF_CS_CAUSE_CC_NW_INVALID_NUMBER_FORMAT      = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 28),
    TAF_CS_CAUSE_CC_NW_FACILITY_REJECTED          = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 29),
    TAF_CS_CAUSE_CC_NW_RESPONSE_TO_STATUS_ENQUIRY = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 30),
    /*  31. Normal, unspecified                        */
    TAF_CS_CAUSE_CC_NW_NORMAL_UNSPECIFIED                      = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 31),
    TAF_CS_CAUSE_CC_NW_NO_CIRCUIT_CHANNEL_AVAILABLE            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 34),
    TAF_CS_CAUSE_CC_NW_NETWORK_OUT_OF_ORDER                    = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 38),
    TAF_CS_CAUSE_CC_NW_TEMPORARY_FAILURE                       = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 41),
    TAF_CS_CAUSE_CC_NW_SWITCHING_EQUIPMENT_CONGESTION          = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 42),
    TAF_CS_CAUSE_CC_NW_ACCESS_INFORMATION_DISCARDED            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 43),
    TAF_CS_CAUSE_CC_NW_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 44),
    /*  47. Resources unavailable, unspecified         */
    TAF_CS_CAUSE_CC_NW_RESOURCES_UNAVAILABLE_UNSPECIFIED         = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 47),
    TAF_CS_CAUSE_CC_NW_QUALITY_OF_SERVICE_UNAVAILABLE            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 49),
    TAF_CS_CAUSE_CC_NW_REQUESTED_FACILITY_NOT_SUBSCRIBED         = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 50),
    TAF_CS_CAUSE_CC_NW_INCOMING_CALL_BARRED_WITHIN_CUG           = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 55),
    TAF_CS_CAUSE_CC_NW_BEARER_CAPABILITY_NOT_AUTHORISED          = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 57),
    TAF_CS_CAUSE_CC_NW_BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 58),
    /*  63. Service or option not available, unspec    */
    TAF_CS_CAUSE_CC_NW_SERVICE_OR_OPTION_NOT_AVAILABLE           = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 63),
    TAF_CS_CAUSE_CC_NW_BEARER_SERVICE_NOT_IMPLEMENTED            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 65),
    TAF_CS_CAUSE_CC_NW_ACM_GEQ_ACMMAX                            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 68),
    TAF_CS_CAUSE_CC_NW_REQUESTED_FACILITY_NOT_IMPLEMENTED        = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 69),
    TAF_CS_CAUSE_CC_NW_ONLY_RESTRICTED_DIGITAL_INFO_BC_AVAILABLE = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 70),
    /*  79. Service or option not implemented, unspec  */
    TAF_CS_CAUSE_CC_NW_SERVICE_OR_OPTION_NOT_IMPLEMENTED = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 79),
    TAF_CS_CAUSE_CC_NW_INVALID_TRANSACTION_ID_VALUE      = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 81),
    TAF_CS_CAUSE_CC_NW_USER_NOT_MEMBER_OF_CUG            = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 87),
    TAF_CS_CAUSE_CC_NW_INCOMPATIBLE_DESTINATION          = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 88),
    TAF_CS_CAUSE_CC_NW_INVALID_TRANSIT_NETWORK_SELECTION = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 91),
    TAF_CS_CAUSE_CC_NW_SEMANTICALLY_INCORRECT_MESSAGE    = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 95),
    TAF_CS_CAUSE_CC_NW_INVALID_MANDATORY_INFORMATION     = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 96),
    /*  97. Msg type non-existent or not implemented   */
    TAF_CS_CAUSE_CC_NW_MESSAGE_TYPE_NON_EXISTENT                   = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 97),
    TAF_CS_CAUSE_CC_NW_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STATE = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 98),
    /*  99. IE non-existent or not implemented         */
    TAF_CS_CAUSE_CC_NW_IE_NON_EXISTENT_OR_NOT_IMPLEMENTED = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 99),
    /* 100. Conditional IE error                       */
    TAF_CS_CAUSE_CC_NW_CONDITIONAL_IE_ERROR = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 100),
    /* 101. Message not compatible with protocol state */
    TAF_CS_CAUSE_CC_NW_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 101),
    /* 102. Recovery on timer expiry Timer number      */
    TAF_CS_CAUSE_CC_NW_RECOVERY_ON_TIMER_EXPIRY = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 102),
    /* 111. Protocol error, unspecified                */
    TAF_CS_CAUSE_CC_NW_PROTOCOL_ERROR_UNSPECIFIED = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 111),
    /* 127. Interworking, unspecified                  */
    TAF_CS_CAUSE_CC_NW_INTERWORKING_UNSPECIFIED = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 127),
    /* 128. MO interrrupted by network release order   */
    TAF_CS_CAUSE_CC_NW_RELEASE_ORDER = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 128),
    /* 128. MO interrrupted by network release order   */
    TAF_CS_CAUSE_CC_NW_AUTH_FAIL = (TAF_CS_CAUSE_CC_NW_SECTION_BEGIN + 129),

    /* CM SERVICE REJECT */
    TAF_CS_CAUSE_CM_SRV_REJ_IMSI_UNKNOWN_IN_HLR                                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 2),
    TAF_CS_CAUSE_CM_SRV_REJ_ILLEGAL_MS                                          = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 3),
    TAF_CS_CAUSE_CM_SRV_REJ_IMSI_UNKNOWN_IN_VLR                                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 4),
    TAF_CS_CAUSE_CM_SRV_REJ_IMEI_NOT_ACCEPTED                                   = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 5),
    TAF_CS_CAUSE_CM_SRV_REJ_ILLEGAL_ME                                          = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 6),
    TAF_CS_CAUSE_CM_SRV_REJ_PLMN_NOT_ALLOWED                                    = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 11),
    TAF_CS_CAUSE_CM_SRV_REJ_LOCATION_AREA_NOT_ALLOWED                           = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 12),
    TAF_CS_CAUSE_CM_SRV_REJ_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA           = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 13),
    TAF_CS_CAUSE_CM_SRV_REJ_NO_SUITABLE_CELLS_IN_LOCATION_AREA                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 15),
    TAF_CS_CAUSE_CM_SRV_REJ_NETWORK_FAILURE                                     = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 17),
    TAF_CS_CAUSE_CM_SRV_REJ_MAC_FAILURE                                         = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 20),
    TAF_CS_CAUSE_CM_SRV_REJ_SYNCH_FAILURE                                       = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 21),
    TAF_CS_CAUSE_CM_SRV_REJ_CONGESTION                                          = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 22),
    TAF_CS_CAUSE_CM_SRV_REJ_GSM_AUTHENTICATION_UNACCEPTABLE                     = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 23),
    TAF_CS_CAUSE_CM_SRV_REJ_NOT_AUTHORIZED_FOR_THIS_CSG                         = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 25),
    TAF_CS_CAUSE_CM_SRV_REJ_SERVICE_OPTION_NOT_SUPPORTED                        = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 32),
    TAF_CS_CAUSE_CM_SRV_REJ_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED             = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 33),
    TAF_CS_CAUSE_CM_SRV_REJ_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER             = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 34),
    TAF_CS_CAUSE_CM_SRV_REJ_CALL_CANNOT_BE_IDENTIFIED                           = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 38),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_0                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 48),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_1                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 49),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_2                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 50),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_3                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 51),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_4                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 52),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_5                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 53),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_6                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 54),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_7                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 55),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_8                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 56),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_9                  = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 57),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_10                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 58),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_11                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 59),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_12                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 60),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_13                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 61),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_14                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 62),
    TAF_CS_CAUSE_CM_SRV_REJ_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_15                 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 63),
    TAF_CS_CAUSE_CM_SRV_REJ_SEMANTICALLY_INCORRECT_MESSAGE                      = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 95),
    TAF_CS_CAUSE_CM_SRV_REJ_INVALID_MANDATORY_INFORMATION                       = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 96),
    TAF_CS_CAUSE_CM_SRV_REJ_MESSAGE_TYPE_NONEXISTENT_OR_NOT_IMPLEMENTED         = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 97),
    TAF_CS_CAUSE_CM_SRV_REJ_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCAL_STATE = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 98),
    TAF_CS_CAUSE_CM_SRV_REJ_INFOMATION_ELEMENT_NONEXISTENT_OR_NOT_IMPLEMENTED   = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 99),
    TAF_CS_CAUSE_CM_SRV_REJ_CONDITIONAL_IE_ERROR                                = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 100),
    TAF_CS_CAUSE_CM_SRV_REJ_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE      = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 101),
    TAF_CS_CAUSE_CM_SRV_REJ_PROTOCOL_ERROR_UNSPECIFIED                          = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 111),

    TAF_CS_CAUSE_CM_SRV_REJ_CONGESTION_WITH_T3246 = (TAF_CS_CAUSE_CM_SRV_REJ_BEGIN + 127),

    /* CSFB SERVICE REJECT */
    TAF_CS_CAUSE_CSFB_SRV_REJ_ILLEGAL_UE                                   = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 3),
    TAF_CS_CAUSE_CSFB_SRV_REJ_ILLEGAL_ME                                   = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 6),
    TAF_CS_CAUSE_CSFB_SRV_REJ_EPS_SERVICES_NOT_ALLOWED                     = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 7),
    TAF_CS_CAUSE_CSFB_SRV_REJ_EPS_SERVICES_AND_NONEPS_SERVICES_NOT_ALLOWED = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 8),
    TAF_CS_CAUSE_CSFB_SRV_REJ_UE_IDENTITY_CANNOT_BE_DERIVED_BY_NW          = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 9),
    TAF_CS_CAUSE_CSFB_SRV_REJ_IMPLICITLY_DETACHED                          = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 10),
    TAF_CS_CAUSE_CSFB_SRV_REJ_PLMN_NOT_ALLOWED                             = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 11),
    TAF_CS_CAUSE_CSFB_SRV_REJ_TRACKING_AREA_NOT_ALLOWED                    = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 12),
    TAF_CS_CAUSE_CSFB_SRV_REJ_ROAMING_NOT_ALLOWED_IN_THIS_TA               = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 13),
    TAF_CS_CAUSE_CSFB_SRV_REJ_NO_SUITABLE_CELLS_IN_TRACKING_AREA           = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 15),
    TAF_CS_CAUSE_CSFB_SRV_REJ_CS_DOMAIN_NOT_AVAILABLE                      = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 18),
    TAF_CS_CAUSE_CSFB_SRV_REJ_CONGESTION                                   = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 22),
    TAF_CS_CAUSE_CSFB_SRV_REJ_NOT_AUTHORIZED_FOR_THIS_CSG                  = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 25),
    TAF_CS_CAUSE_CSFB_SRV_REJ_REQ_SER_OPTION_NOT_AUTHORIZED_IN_PLMN        = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 35),
    TAF_CS_CAUSE_CSFB_SRV_REJ_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE         = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 39),
    TAF_CS_CAUSE_CSFB_SRV_REJ_NO_EPS_BEARER_CONTEXT_ACTIVATED              = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 40),
    TAF_CS_CAUSE_CSFB_SRV_REJ_SEVERE_NETWORK_FAILURE                       = (TAF_CS_CAUSE_CSFB_SRV_REJ_BEGIN + 42),

    /* AS rr connection fail  */
    TAF_CS_CAUSE_RR_CONN_FAIL_CONGESTION                     = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 1),
    TAF_CS_CAUSE_RR_CONN_FAIL_UNSPECIFIED                    = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 2),
    TAF_CS_CAUSE_RR_CONN_FAIL_ACCESS_BAR                     = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 3),
    TAF_CS_CAUSE_RR_CONN_FAIL_EST_CONN_FAIL                  = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 4),
    TAF_CS_CAUSE_RR_CONN_FAIL_IMMEDIATE_ASSIGN_REJECT        = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 5),
    TAF_CS_CAUSE_RR_CONN_FAIL_RANDOM_ACCESS_REJECT           = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 6),
    TAF_CS_CAUSE_RR_CONN_FAIL_T3122_RUNING                   = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 7),
    TAF_CS_CAUSE_RR_CONN_FAIL_NO_RF                          = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 8),
    TAF_CS_CAUSE_RR_CONN_FAIL_LOW_LEVEL_SEARCHING_NETWORK    = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 9),
    TAF_CS_CAUSE_RR_CONN_FAIL_RANDOM_ACCESS_SEND_FAIL        = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 10),
    TAF_CS_CAUSE_RR_CONN_FAIL_NO_VALID_INFO                  = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 11),
    TAF_CS_CAUSE_RR_CONN_FAIL_UE_NOT_ALLOW                   = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 12),
    TAF_CS_CAUSE_RR_CONN_FAIL_TIME_OUT                       = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 13),
    TAF_CS_CAUSE_RR_CONN_FAIL_NO_RANDOM_ACCESS_RESOURCE      = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 14),
    TAF_CS_CAUSE_RR_CONN_FAIL_INVALID_IMMEDIATE_ASSIGNED_MSG = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 15),
    TAF_CS_CAUSE_RR_CONN_FAIL_ACTIVE_PHYSICAL_CHANNEL_FAIL   = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 16),
    TAF_CS_CAUSE_RR_CONN_FAIL_AIR_MSG_DECODE_ERROR           = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 17),
    TAF_CS_CAUSE_RR_CONN_FAIL_CURRENT_PROTOCOL_NOT_SUPPORT   = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 18),
    TAF_CS_CAUSE_RR_CONN_FAIL_INVALID_UE_STATE               = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 19),
    TAF_CS_CAUSE_RR_CONN_FAIL_CELL_BARRED                    = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 20),
    TAF_CS_CAUSE_RR_CONN_FAIL_FAST_RETURN_TO_LTE             = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 21),
    TAF_CS_CAUSE_RR_CONN_FAIL_RA_FAIL_NO_VALID_INFO          = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 22),
    TAF_CS_CAUSE_RR_CONN_FAIL_RJ_INTER_RAT                   = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 23),
    TAF_CS_CAUSE_RR_CONN_FAIL_CSFB_NO_RF = (TAF_CS_CAUSE_RR_CONN_FAIL_BEGIN + 24),

    /* AS Rel ind */
    TAF_CS_CAUSE_RR_REL_AUTH_REJ                   = (TAF_CS_CAUSE_RR_REL_BEGIN + 0),
    TAF_CS_CAUSE_RR_REL_NORMAL_EVENT               = (TAF_CS_CAUSE_RR_REL_BEGIN + 1),
    TAF_CS_CAUSE_RR_REL_NORMAL_UNSPECIFIED         = (TAF_CS_CAUSE_RR_REL_BEGIN + 2),
    TAF_CS_CAUSE_RR_REL_PREEMPTIVE_RELEASE         = (TAF_CS_CAUSE_RR_REL_BEGIN + 3),
    TAF_CS_CAUSE_RR_REL_CONGESTION                 = (TAF_CS_CAUSE_RR_REL_BEGIN + 4),
    TAF_CS_CAUSE_RR_REL_REEST_REJ                  = (TAF_CS_CAUSE_RR_REL_BEGIN + 5),
    TAF_CS_CAUSE_RR_REL_DIRECTED_SIGNAL_CONN_REEST = (TAF_CS_CAUSE_RR_REL_BEGIN + 6),
    TAF_CS_CAUSE_RR_REL_USER_INACTIVE              = (TAF_CS_CAUSE_RR_REL_BEGIN + 7),
    TAF_CS_CAUSE_RR_REL_UTRAN_RELEASE              = (TAF_CS_CAUSE_RR_REL_BEGIN + 8),
    TAF_CS_CAUSE_RR_REL_RRC_ERROR                  = (TAF_CS_CAUSE_RR_REL_BEGIN + 9),
    TAF_CS_CAUSE_RR_REL_RL_FAILURE                 = (TAF_CS_CAUSE_RR_REL_BEGIN + 10),
    TAF_CS_CAUSE_RR_REL_OTHER_REASON               = (TAF_CS_CAUSE_RR_REL_BEGIN + 11),
    TAF_CS_CAUSE_RR_REL_NO_RF                      = (TAF_CS_CAUSE_RR_REL_BEGIN + 12),
    TAF_CS_CAUSE_RR_REL_RLC_ERR_OR                 = (TAF_CS_CAUSE_RR_REL_BEGIN + 13),
    TAF_CS_CAUSE_RR_REL_CELL_UP_DATE_FAIL          = (TAF_CS_CAUSE_RR_REL_BEGIN + 14),
    TAF_CS_CAUSE_RR_REL_NAS_REL_REQ                = (TAF_CS_CAUSE_RR_REL_BEGIN + 15),
    /* redial */
    TAF_CS_CAUSE_RR_REL_CONN_FAIL = (TAF_CS_CAUSE_RR_REL_BEGIN + 16),
    /* redial */
    TAF_CS_CAUSE_RR_REL_NAS_DATA_ABSENT = (TAF_CS_CAUSE_RR_REL_BEGIN + 17),
    TAF_CS_CAUSE_RR_REL_T314_EXPIRED    = (TAF_CS_CAUSE_RR_REL_BEGIN + 18),
    TAF_CS_CAUSE_RR_REL_W_RL_FAIL       = (TAF_CS_CAUSE_RR_REL_BEGIN + 19),

    TAF_CS_CAUSE_RR_REL_G_RL_FAIL = (TAF_CS_CAUSE_RR_REL_BEGIN + 20),
    TAF_CS_CAUSE_RR_REL_OTHER_REASON_NO_EST_CNF = (TAF_CS_CAUSE_RR_REL_BEGIN + 21),

    /* CSFB LMM Error */
    TAF_CS_CAUSE_CSFB_LMM_FAIL_LMM_LOCAL_FAIL     = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 0,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_AUTH_REJ           = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 1,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_T3417_TIME_OUT     = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 2,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_APP_DETACH_SERVICE = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 3,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_CN_DETACH_SERVICE  = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 4,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_FOR_OTHERS         = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 5,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_FOR_EMM_STATE      = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 6,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_SMS_ONLY           = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 7,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_PS_ONLY            = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 8,
    TAF_CS_CAUSE_CSFB_LMM_FAIL_TAU_COLL_ABNORMAL  = (TAF_CS_CAUSE_CSFB_LMM_FAIL_BEGIN) + 9,

    /* MM Inter Error */
    TAF_CS_CAUSE_MM_INTER_ERR_FORB_LA                          = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 0),
    TAF_CS_CAUSE_MM_INTER_ERR_FORB_OTHER                       = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 1),
    TAF_CS_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR                    = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 2),
    TAF_CS_CAUSE_MM_INTER_ERR_CS_DETACH                        = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 3),
    TAF_CS_CAUSE_MM_INTER_ERR_CS_SIM_INVALID                   = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 4),
    TAF_CS_CAUSE_MM_INTER_ERR_CS_SERVICE_CONGESTION            = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 5),
    TAF_CS_CAUSE_MM_INTER_ERR_T3230_TIMER_OUT                  = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 6),
    TAF_CS_CAUSE_MM_INTER_ERR_SEARCHING_NETWORK                = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 7),
    TAF_CS_CAUSE_MM_INTER_ERR_INTER_RAT_SYSTEM_CHANGE          = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 8),
    TAF_CS_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 9),
    TAF_CS_CAUSE_MM_INTER_ERR_OUT_OF_COVERAGE                  = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 10),
    TAF_CS_CAUSE_MM_INTER_ERR_ATTEMPTING_TO_UPDATE             = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 11),
    TAF_CS_CAUSE_MM_INTER_ERR_CSFB_ALREADY_EXISTS              = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 12),
    TAF_CS_CAUSE_MM_INTER_ERR_RESUME_TO_GU_FAIL                = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 13),
    TAF_CS_CAUSE_MM_INTER_ERR_TI_INVALID                       = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 14),
    /* redial */
    TAF_CS_CAUSE_MM_INTER_ERR_WAIT_EST_CNF_TIME_OUT  = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 15),
    TAF_CS_CAUSE_MM_INTER_ERR_CC_CONN_REQ_EXIST      = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 16),
    TAF_CS_CAUSE_MM_INTER_ERR_UE_INVALID_STATE       = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 17),
    TAF_CS_CAUSE_MM_INTER_ERR_WAIT_CC_REEST_TIME_OUT = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 18),
    TAF_CS_CAUSE_MM_INTER_ERR_BACK_TO_LTE            = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 19),
    TAF_CS_CAUSE_MM_INTER_ERR_RESUME_TO_EHRPD        = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 20),
    /* 短信业务SAPI3发送失败,不重拨 */
    TAF_CS_CAUSE_MM_INTER_ERR_SND_SAPI3_FAIL = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 21),
    /* 短信业务SAPI3建立时GAS回复失败,重拨 */
    TAF_CS_CAUSE_MM_INTER_ERR_EST_SAPI3_FAIL = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 22),
    /* eCall Inactive状态,不重拨 */
    TAF_CS_CAUSE_MM_INTER_ERR_ECALL_INACTIVE = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 23),
    /* MM reest状态收到est_cnf(失败)或rel ind,不重拨 */
    TAF_CS_CAUSE_MM_INTER_ERR_REEST_FAIL          = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 24),
    TAF_CS_CAUSE_MM_INTER_ERR_CC_REL_REQ          = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 25),
    TAF_CS_CAUSE_MM_INTER_ERR_LTE_LIMITED_SERVICE = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 26),
    TAF_CS_CAUSE_MM_INTER_ERR_GU_LIMITED_SERVICE = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 27),
    TAF_CS_CAUSE_MM_INTER_ERR_INTER_RAT_SYSTEM_CHANGE_CSFB_MT_EXIST = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 28),
    TAF_CS_CAUSE_MM_INTER_ERR_OUT_OF_COVERAGE_CSFB_MT_EXIST         = (TAF_CS_CAUSE_MM_INTER_ERR_BEGIN + 29),

    /* 平台内部的错误原因值 */
    /* 参数错误 */
    TAF_CS_CAUSE_INVALID_PARAMETER = (TAF_CS_CAUSE_CALL_BEGIN + 1),
    /* USIM卡不存在 */
    TAF_CS_CAUSE_SIM_NOT_EXIST = (TAF_CS_CAUSE_CALL_BEGIN + 2),
    /* 需要SIM卡的PIN码 */
    TAF_CS_CAUSE_SIM_PIN_NEED = (TAF_CS_CAUSE_CALL_BEGIN + 3),
    /* Call Id 分配失败 */
    TAF_CS_CAUSE_NO_CALL_ID = (TAF_CS_CAUSE_CALL_BEGIN + 4),
    /* 呼叫被禁止 */
    TAF_CS_CAUSE_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 5),
    /* 当前呼叫状态异常 */
    TAF_CS_CAUSE_STATE_ERROR = (TAF_CS_CAUSE_CALL_BEGIN + 6),
    /* FDN检查失败 */
    TAF_CS_CAUSE_FDN_CHECK_FAILURE = (TAF_CS_CAUSE_CALL_BEGIN + 8),
    /* CALL CONTROL业务修改了呼叫参数，UE不支持修改后的参数 */
    TAF_CS_CAUSE_CALL_CTRL_BEYOND_CAPABILITY = (TAF_CS_CAUSE_CALL_BEGIN + 9),
    /* CALL CONTROL业务等待USIM的响应超时 */
    TAF_CS_CAUSE_CALL_CTRL_TIMEOUT = (TAF_CS_CAUSE_CALL_BEGIN + 10),
    /* CALL CONTROL业务USIM禁止呼出 */
    TAF_CS_CAUSE_CALL_CTRL_NOT_ALLOWED = (TAF_CS_CAUSE_CALL_BEGIN + 11),
    /* CALL CTRL业务解码失败或发送消息失败都认为参数错误 */
    TAF_CS_CAUSE_CALL_CTRL_INVALID_PARAMETER = (TAF_CS_CAUSE_CALL_BEGIN + 13),
    /* DTMF缓存满了 */
    TAF_CS_CAUSE_DTMF_BUF_FULL = (TAF_CS_CAUSE_CALL_BEGIN + 14),
    /* 重复的STOP DTMF请求 */
    TAF_CS_CAUSE_DTMF_REPEAT_STOP = (TAF_CS_CAUSE_CALL_BEGIN + 15),
    /* 网络拒绝START DTMF请求 */
    TAF_CS_CAUSE_DTMF_REJ = (TAF_CS_CAUSE_CALL_BEGIN + 16),
    /* 呼叫被保持 */
    TAF_CS_CAUSE_CALL_ON_HOLD = (TAF_CS_CAUSE_CALL_BEGIN + 17),
    /* 呼叫释放 */
    TAF_CS_CAUSE_CALL_RELEASE = (TAF_CS_CAUSE_CALL_BEGIN + 18),
    /* 软关机 */
    TAF_CS_CAUSE_POWER_OFF = (TAF_CS_CAUSE_CALL_BEGIN + 19),
    /* 当前没有可用通话 */
    TAF_CS_CAUSE_NOT_IN_SPEECH_CALL = (TAF_CS_CAUSE_CALL_BEGIN + 20),
    /* 卡无效 */
    TAF_CS_CAUSE_SIM_INVALID = (TAF_CS_CAUSE_CALL_BEGIN + 21),
    /* 业务域选择失败 */
    TAF_CS_CAUSE_DOMAIN_SELECTION_FAILURE = (TAF_CS_CAUSE_CALL_BEGIN + 22),
    /* 业务域选择缓存超时 */
    TAF_CS_CAUSE_DOMAIN_SELECTION_TIMER_EXPIRED = (TAF_CS_CAUSE_CALL_BEGIN + 23),
    /* MODEM关机 */
    TAF_CS_CAUSE_MODEM_POWER_OFF = (TAF_CS_CAUSE_CALL_BEGIN + 24),
    /* CS域不支持语音加密 */
    TAF_CS_CAUSE_CS_NOT_SUPPORT_ENCRYPT = (TAF_CS_CAUSE_CALL_BEGIN + 25),

    /* ECALL呼叫模式不允许 */
    TAF_CS_CAUSE_ECALL_MODE_NOT_ALLOWED = (TAF_CS_CAUSE_CALL_BEGIN + 26),
    /* 缓存消息失败 */
    TAF_CS_CAUSE_ECALL_CACHE_MSG_FAIL = (TAF_CS_CAUSE_CALL_BEGIN + 27),
    /* 温保状态下开机失败 */
    TAF_CS_CAUSE_ECALL_LOWERPOWER_POWER_ON_FAIL = (TAF_CS_CAUSE_CALL_BEGIN + 28),
    TAF_CS_CAUSE_CALL_NUMBER_NOT_EXIST          = (TAF_CS_CAUSE_CALL_BEGIN + 29),

    TAF_CS_CAUSE_NO_RF = (TAF_CS_CAUSE_CALL_BEGIN + 30),

    /* CC Inter Error */
    TAF_CS_CAUSE_CC_INTER_ERR_T303_TIME_OUT     = (TAF_CS_CAUSE_CALL_BEGIN + 31),
    TAF_CS_CAUSE_CC_INTER_ERR_T335_TIME_OUT     = (TAF_CS_CAUSE_CALL_BEGIN + 32),
    TAF_CS_CAUSE_CC_INTER_ERR_WAIT_RAB_TIME_OUT = (TAF_CS_CAUSE_CALL_BEGIN + 33),
    TAF_CS_CAUSE_CC_INTER_ERR_NO_TCH            = (TAF_CS_CAUSE_CALL_BEGIN + 34),

    TAF_CS_CAUSE_SYSCFG_MODE_CHANGE = (TAF_CS_CAUSE_CALL_BEGIN + 35),
    TAF_CS_CAUSE_LTE_ONLY_BAND12 = (TAF_CS_CAUSE_CALL_BEGIN + 36),

    TAF_CS_CAUSE_CALL_SRV_ACQ_FAIL = (TAF_CS_CAUSE_CALL_BEGIN + 37),

    /* 紧急呼叫存在 */
    TAF_CS_CAUSE_EMC_EXIST_OPT_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 38),
    /* CS呼叫存在 */
    TAF_CS_CAUSE_CS_CALL_EXIST_OPT_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 39),
    /* IMS呼叫存在 */
    TAF_CS_CAUSE_IMS_CALL_EXIST_OPT_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 40),
    /* NV配置不支持呼叫 */
    TAF_CS_CAUSE_NV_NOT_SUPPORT_OPT_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 41),
    /* 新分配的CALL状态不对 */
    TAF_CS_CAUSE_NEW_CALL_STATE_INCORRECT_OPT_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 42),
    /* ECALL的系统模式不对 */
    TAF_CS_CAUSE_ECALL_SYS_MODE_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 43),
    /* ECALL的呼叫模式不对 */
    TAF_CS_CAUSE_ECALL_CALL_MODE_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 44),
    /* 增强型通话的呼叫类型不对 */
    TAF_CS_CAUSE_ECONF_CALL_TYPE_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 45),
    /* 重复异步接听 */
    TAF_CS_CAUSE_REPEAT_ASYNC_ANS_NOT_ALLOW = (TAF_CS_CAUSE_CALL_BEGIN + 46),
    TAF_CS_CAUSE_CALL_SRV_ACQ_FAIL_NO_RF = (TAF_CS_CAUSE_CALL_BEGIN + 47),
    /* SPM状态机运行达最大个数 */
    TAF_CS_CAUSE_CALL_SPM_BEYOND_CONCURRENCY_CAPABILITY = (TAF_CS_CAUSE_CALL_BEGIN + 48),
    TAF_CS_CAUSE_CALL_WAIT_RF_TIMER_EXP                 = (TAF_CS_CAUSE_CALL_BEGIN + 49),
    TAF_CS_CAUSE_CALL_CS_NUMBER_NOT_ALLOW               = (TAF_CS_CAUSE_CALL_BEGIN + 50),
    /* 未知错误 */
    TAF_CS_CAUSE_UNKNOWN = (TAF_CS_CAUSE_CALL_BEGIN + 0xFF),

    /* VC Error */
    /* HIFI已经启动 */
    TAF_CS_CAUSE_VC_ERR_STARTED = (TAF_CS_CAUSE_VC_BEGIN + 1),
    /* 端口配置失败 */
    TAF_CS_CAUSE_VC_ERR_PORT_CFG_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 2),
    /* 设置Device失败 */
    TAF_CS_CAUSE_VC_ERR_SET_DEVICE_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 3),
    /* start失败 */
    TAF_CS_CAUSE_VC_ERR_SET_START_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 4),
    /* 设置音量失败 */
    TAF_CS_CAUSE_VC_ERR_SET_VOLUME_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 5),
    /* 速率采样失败 */
    TAF_CS_CAUSE_VC_ERR_SAMPLE_RATE_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 6),
    /* start保护定时器超时 */
    TAF_CS_CAUSE_VC_ERR_TI_START_EXPIRED = (TAF_CS_CAUSE_VC_BEGIN + 7),
    /* 设置codec失败 */
    TAF_CS_CAUSE_VC_ERR_SET_CODEC_FAIL = (TAF_CS_CAUSE_VC_BEGIN + 8),

    /* IMS的错误原因值 */
    TAF_CS_CAUSE_IMS_OK                               = (TAF_CS_CAUSE_IMS_BEGIN + 200),
    TAF_CS_CAUSE_IMS_MULTIPLE_CHOICES                 = (TAF_CS_CAUSE_IMS_BEGIN + 300),
    TAF_CS_CAUSE_IMS_MOVED_PERMANENTLY                = (TAF_CS_CAUSE_IMS_BEGIN + 301),
    TAF_CS_CAUSE_IMS_MOVED_TEMPORARILY                = (TAF_CS_CAUSE_IMS_BEGIN + 302),
    TAF_CS_CAUSE_IMS_USE_PROXY                        = (TAF_CS_CAUSE_IMS_BEGIN + 305),
    TAF_CS_CAUSE_IMS_ALTERNATIVE_SERVICE              = (TAF_CS_CAUSE_IMS_BEGIN + 380),
    TAF_CS_CAUSE_IMS_BAD_REQUEST                      = (TAF_CS_CAUSE_IMS_BEGIN + 400),
    TAF_CS_CAUSE_IMS_UNAUTHORIZED                     = (TAF_CS_CAUSE_IMS_BEGIN + 401),
    TAF_CS_CAUSE_IMS_PAYMENT_REQUIRED                 = (TAF_CS_CAUSE_IMS_BEGIN + 402),
    TAF_CS_CAUSE_IMS_FORBIDDEN                        = (TAF_CS_CAUSE_IMS_BEGIN + 403),
    TAF_CS_CAUSE_IMS_NOT_FOUND                        = (TAF_CS_CAUSE_IMS_BEGIN + 404),
    TAF_CS_CAUSE_IMS_METHOD_NOT_ALLOWED               = (TAF_CS_CAUSE_IMS_BEGIN + 405),
    TAF_CS_CAUSE_IMS_NOT_ACCEPTABLE                   = (TAF_CS_CAUSE_IMS_BEGIN + 406),
    TAF_CS_CAUSE_IMS_PROXY_AUTHENTICATION_REQUIRED    = (TAF_CS_CAUSE_IMS_BEGIN + 407),
    TAF_CS_CAUSE_IMS_REQUEST_TIMEOUT                  = (TAF_CS_CAUSE_IMS_BEGIN + 408),
    TAF_CS_CAUSE_IMS_CONFLICT_DEPRECATED              = (TAF_CS_CAUSE_IMS_BEGIN + 409),
    TAF_CS_CAUSE_IMS_GONE                             = (TAF_CS_CAUSE_IMS_BEGIN + 410),
    TAF_CS_CAUSE_IMS_CONDITIONAL_REQUEST_FAILED       = (TAF_CS_CAUSE_IMS_BEGIN + 412),
    TAF_CS_CAUSE_IMS_REQUEST_ENTITY_TOO_LARGE         = (TAF_CS_CAUSE_IMS_BEGIN + 413),
    TAF_CS_CAUSE_IMS_REQUEST_URI_TOO_LONG             = (TAF_CS_CAUSE_IMS_BEGIN + 414),
    TAF_CS_CAUSE_IMS_UNSUPPORTED_MEDIA_TYPE           = (TAF_CS_CAUSE_IMS_BEGIN + 415),
    TAF_CS_CAUSE_IMS_UNSUPPORTED_URI_SCHEME           = (TAF_CS_CAUSE_IMS_BEGIN + 416),
    TAF_CS_CAUSE_IMS_UNKNOWN_RESOURCE_PRIORITY        = (TAF_CS_CAUSE_IMS_BEGIN + 417),
    TAF_CS_CAUSE_IMS_BAD_EXTENSION                    = (TAF_CS_CAUSE_IMS_BEGIN + 420),
    TAF_CS_CAUSE_IMS_EXTENSION_REQUIRED               = (TAF_CS_CAUSE_IMS_BEGIN + 421),
    TAF_CS_CAUSE_IMS_SESSION_INTERVAL_TOO_SMALL       = (TAF_CS_CAUSE_IMS_BEGIN + 422),
    TAF_CS_CAUSE_IMS_INTERVAL_TOO_BRIEF               = (TAF_CS_CAUSE_IMS_BEGIN + 423),
    TAF_CS_CAUSE_IMS_BAD_LOCATION_INFORMATION         = (TAF_CS_CAUSE_IMS_BEGIN + 424),
    TAF_CS_CAUSE_IMS_USE_IDENTITY_HEADER              = (TAF_CS_CAUSE_IMS_BEGIN + 428),
    TAF_CS_CAUSE_IMS_PROVIDE_REFERRER_IDENTITY        = (TAF_CS_CAUSE_IMS_BEGIN + 429),
    TAF_CS_CAUSE_IMS_FLOW_FAILED                      = (TAF_CS_CAUSE_IMS_BEGIN + 430),
    TAF_CS_CAUSE_IMS_ANONYMITY_DISALLOWED             = (TAF_CS_CAUSE_IMS_BEGIN + 433),
    TAF_CS_CAUSE_IMS_BAD_IDENTITY_INFO                = (TAF_CS_CAUSE_IMS_BEGIN + 436),
    TAF_CS_CAUSE_IMS_UNSUPPORTED_CERTIFICATE          = (TAF_CS_CAUSE_IMS_BEGIN + 437),
    TAF_CS_CAUSE_IMS_INVALID_IDENTITY_HEADER          = (TAF_CS_CAUSE_IMS_BEGIN + 438),
    TAF_CS_CAUSE_IMS_FIRST_HOP_LACKS_OUTBOUND_SUPPORT = (TAF_CS_CAUSE_IMS_BEGIN + 439),
    TAF_CS_CAUSE_IMS_MAX_BREADTH_EXCEEDED             = (TAF_CS_CAUSE_IMS_BEGIN + 440),
    TAF_CS_CAUSE_IMS_BAD_INFO_PACKEAGE                = (TAF_CS_CAUSE_IMS_BEGIN + 469),
    TAF_CS_CAUSE_IMS_CONSENT_NEEDED                   = (TAF_CS_CAUSE_IMS_BEGIN + 470),
    TAF_CS_CAUSE_IMS_TEMPORARILY_UNAVAILABLE          = (TAF_CS_CAUSE_IMS_BEGIN + 480),
    TAF_CS_CAUSE_IMS_CALL_TRANSACTION_DOES_NOT_EXIST  = (TAF_CS_CAUSE_IMS_BEGIN + 481),
    TAF_CS_CAUSE_IMS_LOOP_DETECTED                    = (TAF_CS_CAUSE_IMS_BEGIN + 482),
    TAF_CS_CAUSE_IMS_TOO_MANY_HOPS                    = (TAF_CS_CAUSE_IMS_BEGIN + 483),
    TAF_CS_CAUSE_IMS_ADDRESS_INCOMPLETE               = (TAF_CS_CAUSE_IMS_BEGIN + 484),
    TAF_CS_CAUSE_IMS_AMBIGUOUS                        = (TAF_CS_CAUSE_IMS_BEGIN + 485),
    TAF_CS_CAUSE_IMS_BUSY_HERE                        = (TAF_CS_CAUSE_IMS_BEGIN + 486),
    TAF_CS_CAUSE_IMS_REQUEST_TERMINATED               = (TAF_CS_CAUSE_IMS_BEGIN + 487),
    TAF_CS_CAUSE_IMS_NOT_ACCEPTABLE_HERE              = (TAF_CS_CAUSE_IMS_BEGIN + 488),
    TAF_CS_CAUSE_IMS_BAD_EVENT                        = (TAF_CS_CAUSE_IMS_BEGIN + 489),
    TAF_CS_CAUSE_IMS_REQUEST_PENDING                  = (TAF_CS_CAUSE_IMS_BEGIN + 491),
    TAF_CS_CAUSE_IMS_UNDECIPHERABLE                   = (TAF_CS_CAUSE_IMS_BEGIN + 493),
    TAF_CS_CAUSE_IMS_SECURITY_AGREEMENT_REQUIRED      = (TAF_CS_CAUSE_IMS_BEGIN + 494),
    TAF_CS_CAUSE_IMS_SERVER_INTERNAL_ERROR            = (TAF_CS_CAUSE_IMS_BEGIN + 500),
    TAF_CS_CAUSE_IMS_NOT_IMPLEMENTED                  = (TAF_CS_CAUSE_IMS_BEGIN + 501),
    TAF_CS_CAUSE_IMS_BAD_GATEWAY                      = (TAF_CS_CAUSE_IMS_BEGIN + 502),
    TAF_CS_CAUSE_IMS_SERVICE_UNAVAILABLE              = (TAF_CS_CAUSE_IMS_BEGIN + 503),
    TAF_CS_CAUSE_IMS_SERVER_TIME_OUT                  = (TAF_CS_CAUSE_IMS_BEGIN + 504),
    TAF_CS_CAUSE_IMS_VERSION_NOT_SUPPORTED            = (TAF_CS_CAUSE_IMS_BEGIN + 505),
    TAF_CS_CAUSE_IMS_MESSAGE_TOO_LARGE                = (TAF_CS_CAUSE_IMS_BEGIN + 513),
    TAF_CS_CAUSE_IMS_PRECONDITION_FAILURE             = (TAF_CS_CAUSE_IMS_BEGIN + 580),
    TAF_CS_CAUSE_IMS_BUSY_EVERYWHERE                  = (TAF_CS_CAUSE_IMS_BEGIN + 600),
    TAF_CS_CAUSE_IMS_DECLINE                          = (TAF_CS_CAUSE_IMS_BEGIN + 603),
    TAF_CS_CAUSE_IMS_DOES_NOT_EXIST_ANYWHERE          = (TAF_CS_CAUSE_IMS_BEGIN + 604),
    TAF_CS_CAUSE_IMS_GLOBAL_NOT_ACCEPTABLE            = (TAF_CS_CAUSE_IMS_BEGIN + 606),
    /* CL下，IMS开关打开但是IMS不可用 */
    TAF_CS_CAUSE_CL_AND_IMS_SUPPORT_BUT_UNAVAILBALE = (TAF_CS_CAUSE_IMS_BEGIN + 607),
    /* 新增原因值, 该原因值用于不用去CS域重播时的IMS SIP原因值转换 */
    TAF_CS_CAUSE_IMS_OTHERS = (TAF_CS_CAUSE_IMS_BEGIN + 699),

    /* IMSA的错误原因值 */
    /* 一般错误 */
    TAF_CS_CAUSE_IMSA_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 1),
    /* IMSA不支持该命令 */
    TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CMD = (TAF_CS_CAUSE_IMSA_BEGIN + 2),
    /* IMSA分配呼叫实体失败 */
    TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 3),
    /* 通过CS域再尝试此呼叫 */
    TAF_CS_CAUSE_IMSA_RETRY_VIA_CS = (TAF_CS_CAUSE_IMSA_BEGIN + 4),
    /* 命令执行超时 */
    TAF_CS_CAUSE_IMSA_TIMEOUT = (TAF_CS_CAUSE_IMSA_BEGIN + 5),
    /* 不支持的呼叫类型 */
    TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CALL_TYPE = (TAF_CS_CAUSE_IMSA_BEGIN + 6),
    /* 呼叫对应的服务不可用 */
    TAF_CS_CAUSE_IMSA_SERVICE_NOT_AVAILABLE = (TAF_CS_CAUSE_IMSA_BEGIN + 7),
    /* SRVCC过程中缓存消息失败 */
    TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 8),
    /* SRVCC成功 */
    TAF_CS_CAUSE_IMSA_SRVCC_SUCC = (TAF_CS_CAUSE_IMSA_BEGIN + 9),
    /* SRVCC异常，例如关机等 */
    TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL = (TAF_CS_CAUSE_IMSA_BEGIN + 10),
    /* SRVCC过程中出现DEREG */
    TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL_DEREG = (TAF_CS_CAUSE_IMSA_BEGIN + 11),
    /* Tcall超时后CS域重播 */
    TAF_CS_CAUSE_IMSA_TCALL_TIMEOUT = (TAF_CS_CAUSE_IMSA_BEGIN + 12),
    /* Tqos超时后CS域重播 */
    TAF_CS_CAUSE_IMSA_CMCC_TQOS_TIMEOUT = (TAF_CS_CAUSE_IMSA_BEGIN + 13),
    /* 通话中没有语音包 */
    TAF_CS_CAUSE_IMSA_STRM_RTP_BREAK = (TAF_CS_CAUSE_IMSA_BEGIN + 14),
    /* 新增原因值, 该原因值用于不用去CS域重播时的IMS内部原因值转换 */
    /* IMS内部错误 */
    TAF_CS_CAUSE_IMSA_IMS_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 15),
    /* DSDS没有申请到资源 */
    TAF_CS_CAUSE_IMSA_NO_RF = (TAF_CS_CAUSE_IMSA_BEGIN + 16),
    /* 紧急呼叫触发的域选失败 */
    TAF_CS_CAUSE_IMSA_EMC_DOMAIN_TMP_SEL_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 17),
    /* LTE信号质量差 */
    TAF_CS_CAUSE_IMSA_SERVICE_LTE_SIGNAL_BAD = (TAF_CS_CAUSE_IMSA_BEGIN + 18),
    /* 呼叫忙，一般是主被叫冲突 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_CALL_BUSY = (TAF_CS_CAUSE_IMSA_BEGIN + 19),
    /* SDP协商失败 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_SDP_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 20),
    /* 当前不支持该能力 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_NOT_SUP_ABILITY = (TAF_CS_CAUSE_IMSA_BEGIN + 21),
    /* 网络异常 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_NETWORK_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 22),
    /* 呼叫前转 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_CALL_FORWARD = (TAF_CS_CAUSE_IMSA_BEGIN + 23),
    /* 资源不可用 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_RESOURCE_UNAVAILABLE = (TAF_CS_CAUSE_IMSA_BEGIN + 24),
    /* 呼叫数目大于最大值 */
    TAF_CS_CAUSE_IMSA_OMIT_CALL_ERR_CODE_OVERFLOW_MAX_CALL_COUNT = (TAF_CS_CAUSE_IMSA_BEGIN + 25),
    /* CS和IMS呼叫冲突 */
    TAF_CS_CAUSE_IMSA_CS_AND_IMS_CALL_CONFILICT = (TAF_CS_CAUSE_IMSA_BEGIN + 26),
    /* WIFI信号差引发的RTP break导致VOWIFI电话掉话 */
    TAF_CS_CAUSE_IMSA_STRM_RTP_BREAK_WIFI_SIGNAL_BAD = (TAF_CS_CAUSE_IMSA_BEGIN + 27),
    /* WIFI信号差拥塞引发的RTP break导致VOWIFI电话掉话 */
    TAF_CS_CAUSE_IMSA_STRM_RTP_BREAK_WIFI_CONGEST = (TAF_CS_CAUSE_IMSA_BEGIN + 28),
    /* 仅用作IMSA上报给TAF的可忽略错误原因值 */
    TAF_CS_CAUSE_IMSA_IMS_IGNORE_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 29),
    /* LTE信号质量差 */
    TAF_CS_CAUSE_IMSA_SERVICE_WIFI_SIGNAL_BAD = (TAF_CS_CAUSE_IMSA_BEGIN + 30),
    TAF_CS_CAUSE_IMSA_FT_IMS_SPECIAL_FAIL_CELLULAR_ROAM_IMS_IN_WIFI = (TAF_CS_CAUSE_IMSA_BEGIN + 31),
    TAF_CS_CAUSE_IMSA_SSAC_BAR = (TAF_CS_CAUSE_IMSA_BEGIN + 32),
    /* ESFB过程中SR被拒 */
    TAF_CS_CAUSE_IMSA_EMC_ESFB_SR_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 33),
    /* ESFB过程中SR超时 */
    TAF_CS_CAUSE_IMSA_EMC_ESFB_SR_TIMEOUT = (TAF_CS_CAUSE_IMSA_BEGIN + 34),
    /* 紧急呼业务捕获没有搜到IMS域 */
    TAF_CS_CAUSE_IMSA_EMC_ACQUIRE_IMS_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 35),
    /* 紧急呼业务捕获搜到非IMS域 */
    TAF_CS_CAUSE_IMSA_EMC_ACQUIRE_NOT_IMS = (TAF_CS_CAUSE_IMSA_BEGIN + 36),
    /* 紧急呼业务捕获搜网失败 */
    TAF_CS_CAUSE_IMSA_EMC_ACQUIRE_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 37),
    /* IMS呼叫失败默认场景 */
    TAF_CS_CAUSE_IMSA_IMS_CALL_FAIL = (TAF_CS_CAUSE_IMSA_BEGIN + 38),
    /* 媒体承载丢失 */
    TAF_CS_CAUSE_IMSA_MEDIA_PDP_LOST = (TAF_CS_CAUSE_IMSA_BEGIN + 39),
    /* 信令承载丢失 */
    TAF_CS_CAUSE_IMSA_SIP_PDP_LOST = (TAF_CS_CAUSE_IMSA_BEGIN + 40),
    /* HIFI异常 */
    TAF_CS_CAUSE_IMSA_HIFI_ERROR = (TAF_CS_CAUSE_IMSA_BEGIN + 41),
    /* ESFB失败 */
    TAF_CS_CAUSE_IMSA_ESFB_FAILURE = (TAF_CS_CAUSE_IMSA_BEGIN + 42),
    /* 当前PLMN呼叫失败，需要换PLMN重试 */
    TAF_CS_CAUSE_IMSA_IMS_CALL_RETRY_SKIP_CURRENT_PLMN = (TAF_CS_CAUSE_IMSA_BEGIN + 43),
    /* ESFB过程中SR成功，TAU被拒 */
    TAF_CS_CAUSE_IMSA_ESFB_TAU_REJ = (TAF_CS_CAUSE_IMSA_BEGIN + 44),

    /* XCALL 错误原因值 */
    /* No Service\x{ff0c}Call Redial */
    TAF_CS_CAUSE_XCALL_NO_SERVICE = (TAF_CS_CAUSE_XCALL_BEGIN + 1),
    /* Max Access Probes, Call Redial */
    TAF_CS_CAUSE_XCALL_MAX_ACCESS_PROBES = (TAF_CS_CAUSE_XCALL_BEGIN + 2),
    /* Reorder Order, Call Redial */
    TAF_CS_CAUSE_XCALL_REORDER = (TAF_CS_CAUSE_XCALL_BEGIN + 3),
    /* Intercept Order, Call Redial */
    TAF_CS_CAUSE_XCALL_INTERCEPT = (TAF_CS_CAUSE_XCALL_BEGIN + 4),
    /* Access Denied, Call Redial */
    TAF_CS_CAUSE_XCALL_ACCESS_DENYIED = (TAF_CS_CAUSE_XCALL_BEGIN + 5),
    /* Lock, Call not Redial */
    TAF_CS_CAUSE_XCALL_LOCK = (TAF_CS_CAUSE_XCALL_BEGIN + 6),
    /* Acct Block, Call Redial */
    TAF_CS_CAUSE_XCALL_ACCT_BLOCK = (TAF_CS_CAUSE_XCALL_BEGIN + 7),
    /* Access Control based on Call Type, redial */
    TAF_CS_CAUSE_XCALL_NDSS = (TAF_CS_CAUSE_XCALL_BEGIN + 8),
    /* Redirection, Call Redial */
    TAF_CS_CAUSE_XCALL_REDIRECTION = (TAF_CS_CAUSE_XCALL_BEGIN + 9),
    /* BS Reject, used in Register process, no need redial */
    TAF_CS_CAUSE_XCALL_NOT_ACCEPT_BY_BS = (TAF_CS_CAUSE_XCALL_BEGIN + 10),
    /* Access In Progress, Call Redial */
    TAF_CS_CAUSE_XCALL_ACCESS_IN_PROGRESS = (TAF_CS_CAUSE_XCALL_BEGIN + 11),
    /* Access fail, Call Redial */
    TAF_CS_CAUSE_XCALL_ACCESS_FAIL = (TAF_CS_CAUSE_XCALL_BEGIN + 12),
    /* State can not Process Call, Call Redial */
    TAF_CS_CAUSE_XCALL_ABORT = (TAF_CS_CAUSE_XCALL_BEGIN + 13),
    /* Signal Fade, Call Redial */
    TAF_CS_CAUSE_XCALL_SIGNAL_FADE_IN_ACH = (TAF_CS_CAUSE_XCALL_BEGIN + 14),
    /* Access Time Out, Call Redial */
    TAF_CS_CAUSE_XCALL_CHANNEL_ASSIGN_TIMEOUT = (TAF_CS_CAUSE_XCALL_BEGIN + 15),
    /* BS, Call Redial */
    TAF_CS_CAUSE_XCALL_BS_RELEASE = (TAF_CS_CAUSE_XCALL_BEGIN + 16),
    /* Already In Traffic Channel, no need dial */
    TAF_CS_CAUSE_XCALL_OTHER_SERVICE_IN_TCH = (TAF_CS_CAUSE_XCALL_BEGIN + 17),
    /* Concurrent Service not Support, Call Redial */
    TAF_CS_CAUSE_XCALL_CCS_NOT_SUPPORT = (TAF_CS_CAUSE_XCALL_BEGIN + 18),
    /* Normal Call end, Call not Redial */
    TAF_CS_CAUSE_XCALL_MS_NORMAL_RELEASE = (TAF_CS_CAUSE_XCALL_BEGIN + 19),
    /* CAS_CNAS_1X_TERMINATE_REASON_SO_REJ */
    TAF_CS_CAUSE_XCALL_SO_REJ = (TAF_CS_CAUSE_XCALL_BEGIN + 20),
    /* CAS_CNAS_1X_TERMINATE_REASON_RELEASE_TIME_OUT */
    TAF_CS_CAUSE_XCALL_RELEASE_TIME_OUT = (TAF_CS_CAUSE_XCALL_BEGIN + 21),
    /* L2 ACK FAILURE */
    TAF_CS_CAUSE_XCALL_CONNECT_ORDER_ACK_FAILURE = (TAF_CS_CAUSE_XCALL_BEGIN + 22),
    /* TiWaitForXcallIncomingRsp TIME OUT */
    TAF_CS_CAUSE_XCALL_INCOMING_RSP_TIME_OUT = (TAF_CS_CAUSE_XCALL_BEGIN + 23),
    /* TiWaitL2Ack TIME OUT */
    TAF_CS_CAUSE_XCALL_L2_ACK_TIME_OUT = (TAF_CS_CAUSE_XCALL_BEGIN + 24),
    /* 关机原因导致的原因 */
    TAF_CS_CAUSE_XCALL_POWER_DOWN_IND     = (TAF_CS_CAUSE_XCALL_BEGIN + 25),
    TAF_CS_CAUSE_XCALL_CONNID_NOT_FOUND   = (TAF_CS_CAUSE_XCALL_BEGIN + 26),
    TAF_CS_CAUSE_XCALL_APS_TIMEOUT        = (TAF_CS_CAUSE_XCALL_BEGIN + 27),
    TAF_CS_CAUSE_XCALL_ACCESS_CNF_TIMEOUT = (TAF_CS_CAUSE_XCALL_BEGIN + 28),
    TAF_CS_CAUSE_XCALL_ACCESS_IND_TIMEOUT = (TAF_CS_CAUSE_XCALL_BEGIN + 29),
    TAF_CS_CAUSE_XCALL_UNKNOWN            = (TAF_CS_CAUSE_XCALL_BEGIN + 30),
    TAF_CS_CAUSE_XCALL_CALL_NOT_ALLOWED   = (TAF_CS_CAUSE_XCALL_BEGIN + 31),
    TAF_CS_CAUSE_XCALL_XCALL_HANGUP       = (TAF_CS_CAUSE_XCALL_BEGIN + 32),
    TAF_CS_CAUSE_XCALL_ACCESS_REL_IND     = (TAF_CS_CAUSE_XCALL_BEGIN + 33),

    /* 当前无服务 */
    TAF_CS_CAUSE_XCALL_INTERNAL_NO_SERVICE = (TAF_CS_CAUSE_XCALL_BEGIN + 34),
    TAF_CS_CAUSE_XCALL_WAIT_L2_ACK_RELEASE = (TAF_CS_CAUSE_XCALL_BEGIN + 35),
    /* 紧急呼叫中止，后续状态尝试重拨 */
    TAF_CS_CAUSE_XCALL_EMERGENCY_CALL_ABORT = (TAF_CS_CAUSE_XCALL_BEGIN + 36),
    /* 紧急呼叫在被叫存在时以flash消息发上去 */
    TAF_CS_CAUSE_XCALL_EMERGENCY_CALL_FLASHED = (TAF_CS_CAUSE_XCALL_BEGIN + 37),

    /* 呼叫过程中发生重定向，需要重拨 */
    TAF_CS_CAUSE_XCALL_NDSS_REDIAL_IND = (TAF_CS_CAUSE_XCALL_BEGIN + 38),

    TAF_CS_CAUSE_XCALL_MT_SMS_IN_TCH = (TAF_CS_CAUSE_XCALL_BEGIN + 39),
    TAF_CS_CAUSE_XCALL_NW_NORMAL_RELEASE = (TAF_CS_CAUSE_XCALL_BEGIN + 40),

    TAF_CS_CAUSE_XCALL_SIGNAL_FADE_IN_TCH = (TAF_CS_CAUSE_XCALL_BEGIN + 41),

    TAF_CS_CAUSE_XCALL_EXIST_VOICE_CALL      = (TAF_CS_CAUSE_XCALL_BEGIN + 42),
    TAF_CS_CAUSE_XCALL_CLIENT_END            = (TAF_CS_CAUSE_XCALL_BEGIN + 43),
    TAF_CS_CAUSE_XCALL_INCOMING_CALL         = (TAF_CS_CAUSE_XCALL_BEGIN + 44),
    TAF_CS_CAUSE_XCALL_REDIAL_PERIOD_TIMEOUT = (TAF_CS_CAUSE_XCALL_BEGIN + 45),
    TAF_CS_CAUSE_XCALL_CAS_MS_END            = (TAF_CS_CAUSE_XCALL_BEGIN + 46),
    TAF_CS_CAUSE_XCALL_NO_RF                 = (TAF_CS_CAUSE_XCALL_BEGIN + 47),
    TAF_CS_CAUSE_XCALL_RETRY_FAIL            = (TAF_CS_CAUSE_XCALL_BEGIN + 48),

    TAF_CS_CAUSE_XCALL_SRV_ACQ_CNF_FAIL = (TAF_CS_CAUSE_XCALL_BEGIN + 49),

    TAF_CS_CAUSE_XCALL_TCH_LOST                = (TAF_CS_CAUSE_XCALL_BEGIN + 50),
    TAF_CS_CAUSE_XCALL_T50_EXPIRED             = (TAF_CS_CAUSE_XCALL_BEGIN + 51),
    TAF_CS_CAUSE_XCALL_T51_EXPIRED             = (TAF_CS_CAUSE_XCALL_BEGIN + 52),
    TAF_CS_CAUSE_XCALL_TCH_WAIT_BS_ACK_EXPIRED = (TAF_CS_CAUSE_XCALL_BEGIN + 53),
    /* power save 引起的主叫失败 */
    TAF_CS_CAUSE_XCALL_POWER_DOWN_POWER_SAVE = (TAF_CS_CAUSE_XCALL_BEGIN + 54),

    TAF_CS_CAUSE_XCALL_INTER_ABORT = (TAF_CS_CAUSE_XCALL_BEGIN + 55),

    /* 将TAF_CS_CAUSE_XCALL_CLIENT_END拆分 */
    TAF_CS_CAUSE_XCALL_CLIENT_END_REDIAL_INTERVAL_RUNNING = (TAF_CS_CAUSE_XCALL_BEGIN + 56),
    TAF_CS_CAUSE_XCALL_CLIENT_END_WAITING_RF_AVAIL        = (TAF_CS_CAUSE_XCALL_BEGIN + 57),
    TAF_CS_CAUSE_XCALL_CLIENT_END_WAITING_SRV_ACQ         = (TAF_CS_CAUSE_XCALL_BEGIN + 58),
    TAF_CS_CAUSE_XCALL_CLIENT_END_DIALING                 = (TAF_CS_CAUSE_XCALL_BEGIN + 59),
    TAF_CS_CAUSE_XCALL_CLIENT_END_ALERTING                = (TAF_CS_CAUSE_XCALL_BEGIN + 60),
    TAF_CS_CAUSE_XCALL_CLIENT_END_ACTIVE                  = (TAF_CS_CAUSE_XCALL_BEGIN + 61),
    TAF_CS_CAUSE_XCALL_POWER_DOWN_IMS_REGED = (TAF_CS_CAUSE_XCALL_BEGIN + 62),

    TAF_CS_CAUSE_BUTT = (0xFFFFFFFF)
};
typedef VOS_UINT32 TAF_CS_CauseUint32;

/* [10.5.4.9] Calling party BCD number */
/* ============================================================================ */
/* Table 10.5.120/3GPP TS 24.008: Calling party BCD number */
/* Presentation indicator (octet 3a) */
/* Bits 7 6                          */
enum MN_CALL_PresentationInd {
    MN_CALL_PRESENTATION_ALLOWED       = 0,
    MN_CALL_PRESENTATION_RESTRICTED    = 1,
    MN_CALL_PRESENTATION_NOT_AVAILABLE = 2,
    MN_CALL_PRESENTATION_RESERVED      = 3
};
typedef VOS_UINT8 MN_CALL_PresentationIndUint8;


/* ============================================================================ */
/* Table 2.7.4.41. /3GPP2 C.S005-A v6.0: Calling party number */
/* Presentation indicator (octet 3a) */
/* Bits 7 6                          */
enum TAF_CALL_PresentationInd {
    TAF_CALL_PRESENTATION_ALLOWED       = 0,
    TAF_CALL_PRESENTATION_RESTRICTED    = 1,
    TAF_CALL_PRESENTATION_NOT_AVAILABLE = 2,
    TAF_CALL_PRESENTATION_RESERVED      = 3,
    TAF_CALL_PRESENTATION_BUTT          = 4
};

typedef VOS_UINT8 TAF_CALL_PresentationIndUint8;


enum MN_CALL_ModifyStatus {
    MN_CALL_MODIFY_REMOTE_USER_REQUIRE_TO_MODIFY, /* 远程用户发起的modify */
    MN_CALL_MODIFY_PROC_BEGIN,                    /* modify流程开始 */
    MN_CALL_MODIFY_PROC_END,                      /* modify流程结束 */
    MN_CALL_MODIFY_STATUS_BUTT
};
typedef VOS_UINT8 MN_CALL_ModifyStatusUint8;


enum TAF_CALL_RejCause {
    /* CS域挂断电话原因值为#17 User Busy, IMS域挂断电话原语为486 Busy Here */
    TAF_CALL_REJ_CAUSE_USER_BUSY = 0,
    /* CS域挂断电话原因值为#21 Call Rejection, IMS域挂断电话原语为603 Decline */
    TAF_CALL_REJ_CAUSE_CALL_REJECTION = 1,

    TAF_CALL_REJ_CAUSE_BUTT
};
typedef VOS_UINT8 TAF_CALL_RejCauseUint8;


enum MN_CALL_CfCause {
    MN_CALL_CF_CAUSE_NONE = 0,
    MN_CALL_CF_CAUSE_ALWAYS,
    MN_CALL_CF_CAUSE_BUSY,
    MN_CALL_CF_CAUSE_POWER_OFF,
    MN_CALL_CF_CAUSE_NO_ANSWER,
    MN_CALL_CF_CAUSE_SHADOW_ZONE,
    MN_CALL_CF_CAUSE_DEFLECTION_480,
    MN_CALL_CF_CAUSE_DEFLECTION_487,

    MN_CALL_CF_CAUSE_BUTT
};
typedef VOS_UINT8 MN_CALL_CfCauseUint8;


enum MN_CALL_HoldTone {
    MN_CALL_HOLD_TONE_DISABLE = 0,
    MN_CALL_HOLD_TONE_ENABLE,

    MN_CALL_HOLD_TONE_BUTT
};
typedef VOS_UINT8 MN_CALL_HoldToneUint8;


enum TAF_CALL_ModifyReason {
    TAF_CALL_MODIFY_REASON_REMOTE               = 0,
    TAF_CALL_MODIFY_REASON_RTP_TIMEOUT          = 1,
    TAF_CALL_MODIFY_REASON_RTCP_TIMEOUT         = 2,
    TAF_CALL_MODIFY_REASON_RTP_AND_RTCP_TIMEOUT = 3,
    TAF_CALL_MODIFY_REASON_BUTT
};
typedef VOS_UINT8 TAF_CALL_ModifyReasonUint8;


enum TAF_CALL_EmcCallBackDomain {
    TAF_CALL_EMC_CALL_BACK_MODE_DOMAIN_3GPP = 0,
    TAF_CALL_EMC_CALL_BACK_MODE_DOMAIN_WIFI = 1,

    TAF_CALL_EMC_CALL_BACK_MODE_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_CALL_EmcCallBackDomainUint8;


enum TAF_CALL_RttInd {
    TAF_CALL_RTT_IND_NOT_RTT_CALL = 0,
    TAF_CALL_RTT_IND_RTT_CALL     = 1,
    TAF_CALL_RTT_IND_RMT_TTY_CALL = 2,
    TAF_CALL_RTT_IND_RMT_NOT_SUPT = 3,
    TAF_CALL_RTT_IND_BUTT
};
typedef VOS_UINT8 TAF_CALL_RttIndUint8;

enum TAF_CALL_EcallDomainPrefer {
    /* eCall优选域默认值。eCall优先在PS域发起，PS域不支持时再CS域发起 */
    TAF_CALL_ECALL_DOMAIN_PS_PREFER = 0,
    /* 针对MIEC、AIEC eCall考虑支持紧急呼换域重拨，优选域配置为1时优先在CS域发起。
        针对test、recfg eCall，普通呼不支持低制式向高制式换域，优先域配置为1时，eCall只允许在CS域发起 */
    TAF_CALL_ECALL_DOMAIN_CS_PREFER = 1, /* csfb prefer */
    TAF_CALL_ECALL_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_CALL_EcallDomainPreferUint8;


typedef struct {
    VOS_UINT8 isExist;
    VOS_UINT8 lastOctOffset;
    VOS_UINT8 octet3;
    VOS_UINT8 subAddrInfo[MN_CALL_MAX_SUBADDR_INFO_LEN];
    VOS_UINT8 reserved;
} MN_CALL_Subaddr;

#define MN_CALL_MAX_CALLED_BCD_NUM_LEN (40) /* 3GPP TS 24.008 10.5.4.7 */
#define MN_CALL_MAX_CALLED_ASCII_NUM_LEN (MN_CALL_MAX_CALLED_BCD_NUM_LEN * 2)

typedef struct {
    MN_CALL_NumTypeUint8 numType;
    VOS_UINT8            numLen;
    VOS_UINT8            bcdNum[MN_CALL_MAX_CALLED_BCD_NUM_LEN];

    VOS_UINT8 isExistPlusSign;  /* 被叫号码中间位置是否存在'+' */
    VOS_UINT8 plusSignLocation; /* '+'在号段的位置 */
    VOS_UINT8 reserved[4];

} MN_CALL_CalledNum;


typedef struct {
    VOS_BOOL  exist;
    VOS_UINT8 emergencyCat;
    VOS_UINT8 reserve[3];
} MN_CALL_EmergencyCat;


typedef struct {
    VOS_BOOL   isExtEmc;
    VOS_UINT32 urnLen;
    VOS_UINT8  urn[MN_CALL_EENL_URN_LEN];
} MN_CALL_ExtEmcInfo;


typedef struct {
    VOS_UINT8 isUser;
    VOS_UINT8 discardCallFlag;
    VOS_UINT8 reserved[2];
} MN_CALL_DIscDir;


typedef struct {
    VOS_UINT8 numLen;
    VOS_CHAR  displayName[MN_CALL_DISPLAY_NAME_STRING_SZ];
    VOS_UINT8 reserved[2];
} MN_CALL_DisplayName;


typedef struct {
    VOS_UINT16 textLen;
    VOS_CHAR   errInfoText[TAF_CALL_ERROR_INFO_TEXT_STRING_SZ];
    VOS_UINT8  reserved;
} TAF_CALL_ErrorInfoText;


typedef struct {
    MN_CALL_ID_T callId;
    VOS_CHAR     key;      /* DTMF Key */
    VOS_UINT16   onLength; /* DTMF持续时间，0: 代表停止DTMF */
    /* stop dtmf req和start dtmf req时间间隔，单位ms */
    VOS_UINT16        offLength;
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         reserved[3];
} TAF_CALL_DtmfReqInfo;


typedef struct {
    VOS_UINT8            num;
    VOS_UINT8            reserve[3];
    TAF_CALL_DtmfReqInfo dtmf[SRVCC_CALL_DTMF_REQ_MSG_MAX_NUM];
} TAF_CALL_DtmfBuff;


typedef struct {
    VOS_UINT8                       isEmcCallBackAllowed; /* 是否允许进入紧急呼回呼模式 */
    TAF_CALL_EmcCallBackDomainUint8 emcCallBackDomain;    /* 紧急呼回拨域:  3GPP, WIFI */
    VOS_UINT8                       reserved[2];
} TAF_CALL_EmcCallBackInfo;


typedef struct {
    MN_CALL_ID_T callId; /* 呼叫ID，0: 代表当前默认呼叫 */
    VOS_CHAR     key;    /* DTMF Key 只在START_DTMF时关注 */
    /* DTMF持续时间(单位ms)，0: 代表停止DTMF */
    VOS_UINT16 onLength;

    VOS_UINT16 offLength; /* stop dtmf req和start dtmf req发送的时间间隔(单位ms)
                             大于70ms小于600ms */
    VOS_UINT8 reserved[2];
} TAF_CALL_DtmfParam;


typedef struct {
    VOS_UINT32        callNum;
    MN_CALL_CalledNum dialNumber[TAF_CALL_MAX_ECONF_CALLED_NUM];
    MN_CALL_Subaddr   subaddr[TAF_CALL_MAX_ECONF_CALLED_NUM];
} TAF_CALL_EconfCallList;


typedef struct {
    MN_CALL_TypeUint8         callType;          /* Call Type */
    MN_CALL_ClirCfgUint8      clirCfg;           /* CLIR Configuration */
    MN_CALL_ModeUint8         callMode;          /* Call Mode */
    TAF_CALL_VoiceDomainUint8 voiceDomain;       /* VOICE DOMAIN */
    VOS_UINT8                 imsInvitePtptFlag; /* IMS 邀请第三方通话标记 */
    VOS_UINT8                 reserved[3];
    MN_CALL_CugCfg            cugCfg;        /* CUG Configuration */
    MN_CALL_CsDataCfg         dataCfg;       /* Data Configuration */
    TAF_CALL_EconfCallList    econfCalllist; /* 邀请参加多方通话会议者的信息 */
} TAF_CALL_EconfDialInfo;


enum TAF_CALL_EconfState {
    TAF_CALL_ECONF_STATE_DISCONNECTED,
    TAF_CALL_ECONF_STATE_CONNECTED,
    TAF_CALL_ECONF_STATE_ONHOLD,
    TAF_CALL_ECONF_STATE_PREDIALING,
    TAF_CALL_ECONF_STATE_DIALING,
    TAF_CALL_ECONF_STATE_BUTT
};
typedef VOS_UINT8 TAF_CALL_EconfStateUint8;


typedef struct {
    TAF_CALL_EconfStateUint8 callState; /* Call State */
    VOS_UINT8                auReserve1[3];
    TAF_CS_CauseUint32       cause;                                 /* disconnec时候需要携带原因值 */
    MN_CALL_CalledNum        callNumber;                            /* Call Number */
    VOS_CHAR                 displaytext[TAF_IMSA_ALPHA_STRING_SZ]; /* display-text */
    VOS_UINT8                auReserve2[7];
} TAF_CALL_EconfInfoParam;



enum MN_CALL_CodecType {
    MN_CALL_CODEC_TYPE_AMR, /* amr */
    MN_CALL_CODEC_TYPE_EFR, /* enhanced full rate */
    MN_CALL_CODEC_TYPE_FR,  /* full rate */
    MN_CALL_CODEC_TYPE_HR,  /* half rate */
    MN_CALL_CODEC_TYPE_AMR2,
    MN_CALL_CODEC_TYPE_AMRWB,
    MN_CALL_CODEC_TYPE_G711,
    MN_CALL_CODEC_TYPE_EVS,

    MN_CALL_CODEC_TYPE_EVS_NB,
    MN_CALL_CODEC_TYPE_EVS_WB,
    MN_CALL_CODEC_TYPE_EVS_SWB,

    MN_CALL_CODEC_TYPE_BUTT /* invalid value */
};
typedef VOS_UINT8 MN_CALL_CodecTypeUint8;


enum TAF_CALL_CcwaiSrvClass {
    TAF_CALL_CCWAI_SRV_CLASS_VOICE = 1,
    TAF_CALL_CCWAI_SRV_CLASS_VIDEO = 2,
    TAF_CALL_CCWAI_SRV_CLASS_SET_NW = 3,

    TAF_CALL_CCWAI_SRV_CLASS_BUTT
};
typedef VOS_UINT8 TAF_CALL_CcwaiSrvClassUint8;


enum TAF_CALL_CcwaiMode {
    TAF_CALL_CCWAI_MODE_DISABLE = 0,
    TAF_CALL_CCWAI_MODE_ENABLE  = 1,

    TAF_CALL_CCWAI_MODE_BUTT
};
typedef VOS_UINT8 TAF_CALL_CcwaiModeUint8;


enum TAF_CALL_CcwaCtrlMode {
    TAF_CALL_CCWA_CTRL_BY_NW = 0,
    TAF_CALL_CCWA_CTRL_BY_UE = 1,

    TAF_CALL_CCWA_CTRL_MODE_BUTT
};
typedef VOS_UINT8 TAF_CALL_CcwaCtrlModeUint8;


enum TAF_SRVCC_CallState {
    TAF_SRVCC_CALL_STATE_DIALING  = 0, /* dialing (MO call) */
    TAF_SRVCC_CALL_STATE_ALERTING = 1, /* alerting (MO call) */
    /* activing --这个状态协议规定要求GU下重传connect消息 */
    TAF_SRVCC_CALL_STATE_ACTVING  = 2,
    TAF_SRVCC_CALL_STATE_ACTIVE   = 3, /* active */
    TAF_SRVCC_CALL_STATE_HELD     = 4, /* held */
    TAF_SRVCC_CALL_STATE_INCOMING = 5, /* incoming (MT call) */
    TAF_SRVCC_CALL_STATE_WAITING  = 6, /* waiting (MT call) */
    TAF_SRVCC_CALL_STATE_BUTT
};
typedef VOS_UINT8 TAF_SRVCC_CallStateUint8;


enum TAF_SRVCC_CallImsEarlyMediaType {
    TAF_SRVCC_CALL_IMS_EARLY_MEDIA_INVALID = 0, /* 无效值 */
    TAF_SRVCC_CALL_IMS_EARLY_MEDIA_LOCAL   = 1, /* 本地放音 */
    TAF_SRVCC_CALL_IMS_EARLY_MEDIA_NW      = 2, /* 网络早媒体放音 */

    TAF_SRVCC_CALL_IMS_EARLY_MEDIA_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SRVCC_CallImsEarlyMediaTypeUint8;


enum TAF_CALL_QryClprMode {
    TAF_CALL_QRY_CLPR_MODE_GUL = 0,
    TAF_CALL_QRY_CLPR_MODE_C   = 1,

    TAF_CALL_QRY_CLPR_MODE_BUTT
};
typedef VOS_UINT8 TAF_CALL_QryClprModeUint8;


enum TAF_CALL_GET_CALL_INFO_TYPE_ENMU {
    TAF_CALL_GET_CALL_INFO_TYPE_CLCC = 0, /* CLCC查询请求获取CALL信息 */
    TAF_CALL_GET_CALL_INFO_TYPE_ATA  = 1, /* ATA查询请求获取CALL信息 */

    TAF_CALL_GET_CALL_INFO_TYPE_ENMU_BUTT
};
typedef VOS_UINT8 TAF_CALL_GetCallInfoTypeUint8;


typedef struct {
    VOS_UINT32                   opRedirectNumPi : 1;
    VOS_UINT32                   opRedirectNum : 1;
    VOS_UINT32                   opRedirectSubaddr : 1;
    VOS_UINT32                   opSpare : 29;
    MN_CALL_PresentationIndUint8 redirectNumPi;
    VOS_UINT8                    rsv[3];
    MN_CALL_BcdNum  redirectNum;     /* BCD number */
    MN_CALL_Subaddr redirectSubaddr; /* Subaddress */
} MN_CALL_RediretInfo;

typedef struct {
    VOS_UINT32                   opCallingNumPi : 1;
    VOS_UINT32                   opNoCliCause : 1;
    VOS_UINT32                   opSpare : 30;
    MN_CALL_PresentationIndUint8 callingNumPi;
    MN_CALL_NoCliCauseUint8      noCliCause;
    VOS_UINT8                    rsv[2];
    MN_CALL_RediretInfo          redirectInfo;
} MN_CALL_ClprInfo;


enum TAF_CALL_DtmfState {
    TAF_CALL_DTMF_IDLE = 0,       /* 当前DTMF空闲 */
    TAF_CALL_DTMF_WAIT_START_CNF, /* 发出了START请求后等待回复 */
    TAF_CALL_DTMF_WAIT_STOP_CNF,  /* 发出了STOP请求后等待回复 */
    /* 定时器超时自动发出STOP请求后等待回复 */
    TAF_CALL_DTMF_WAIT_AUTO_STOP_CNF,
    TAF_CALL_DTMF_WAIT_ON_LENGTH_TIME_OUT, /* 启动DTMF定时器后等待超时 */
    /* 收到stop dtmf ack启定时器超时再处理下条start dtmf req */
    TAF_CALL_DTMF_WAIT_OFF_LENGTH_TIME_OUT,

    TAF_CALL_DTMF_STATE_BUTT
};
typedef VOS_UINT8 TAF_CALL_DtmfStateUint8;


enum TAF_CALL_ChannelType {
    TAF_CALL_CHANNEL_TYPE_NONE   = 0x00, /* 无带内音信息 */
    TAF_CALL_CHANNEL_TYPE_NARROW = 0x01, /* 带内音可用，窄带语音 */
    TAF_CALL_CHANNEL_TYPE_WIDE   = 0x02, /* 带内音可用，宽带语音 */

    TAF_CALL_CHANNEL_TYPE_EVS_NB  = 0x03, /* 带内音可用，EVS NB */
    TAF_CALL_CHANNEL_TYPE_EVS_WB  = 0x04, /* 带内音可用，EVS WB */
    TAF_CALL_CHANNEL_TYPE_EVS_SWB = 0x05, /* 带内音可用，EVS SWB */

    TAF_CALL_CHANNEL_TYPE_BUTT
};
typedef VOS_UINT8 TAF_CALL_ChannelTypeUint8;



enum TAF_CALL_SrvccStatus {
    TAF_CALL_SRVCC_STATUS_START   = 0, /* SRVCC开始 */
    TAF_CALL_SRVCC_STATUS_SUCCESS = 1, /* SRVCC成功 */
    TAF_CALL_SRVCC_STATUS_FAIL    = 2, /* SRVCC失败 */

    TAF_CALL_SRVCC_STATUS_BUTT
};
typedef VOS_UINT32 TAF_CALL_SrvccStatusUint32;


typedef struct {
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         callId;
    /* TI信息，保护TI FLAG和TI两项，具体格式参考上面描述 */
    VOS_UINT8 ti;
    /* IMSA同步过来的IMS域下的呼叫状态 */
    TAF_SRVCC_CallStateUint8 callState;
    MN_CALL_ModeUint8        callMode;
    MN_CALL_MptyStateUint8   mptyState;
    MN_CALL_DirUint8         callDir;
    MN_CALL_TypeUint8        callType;
    VOS_UINT8                localAlertedFlag; /* IMS域是否需要进行本地振铃 */
    /* 用于标识通话是否是增强型多方通话: VOS_TRUE: 增强型多方通话 VOS_FALSE: 非增强型多方通话 */
    VOS_UINT8                    eConferenceFlag;
    MN_CALL_PresentationIndUint8 callingNumPi;
    MN_CALL_PresentationIndUint8 redirectNumPi;
    MN_CALL_NoCliCauseUint8      noCliCause; /* Cause of the NO CLI */
    TAF_CALL_ServiceTypeUint8    serviceType;
    MN_CALL_BcdNum               callNumber;
    MN_CALL_CalledNum            calledNumber;
    MN_CALL_BcdNum               redirectNumber;
    MN_CALL_BcdNum               connectNumber;
    VOS_UINT8                    needActiveFlag;
    TAF_SRVCC_CallImsEarlyMediaTypeUint8 earlyMediaType; /* IMS早媒体类型指示 */
    VOS_UINT8                            rsv[2];

    VOS_UINT32 startCallTime; /* VOLTE通话开始时间 */
} TAF_SRVCC_CallInfo;


typedef struct {
    MN_CALL_ID_T callId;
    VOS_CHAR     key;      /* DTMF Key */
    VOS_UINT16   onLength; /* DTMF持续时间，0: 代表停止DTMF */

    /* stop dtmf req和start dtmf req时间间隔，单位ms */
    VOS_UINT16        offLength;
    MN_CLIENT_ID_T    clientId;
    MN_OPERATION_ID_T opId;
    VOS_UINT8         reserved1[3];
} TAF_CALL_DtmfInfo;

/* 呼叫挂断来源枚举 */
enum TAF_CALL_EndSource {
    TAF_CALL_SERVICE_NOT_AVAILABLE = 21, /* 服务不可得 */
    TAF_CALL_USER_HANG_UP = 25, /* 用户挂断 */
    TAF_CALL_SUBLAYER_RELEASE = 100, /* 底层释放 */
    TAF_CALL_REFUSE_INCALL = 102, /* 拒接来电 */
    TAF_CALL_NETWORK_RELEASE = 104 /* 网络释放 */
};
typedef VOS_UINT32 TAF_CALL_EndSourceUint32;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of taf_call_comm.h */
