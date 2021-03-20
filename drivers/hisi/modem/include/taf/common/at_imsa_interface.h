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

#ifndef AT_IMSA_INTERFACE_H
#define AT_IMSA_INTERFACE_H

#include "vos.h"
#include "taf_type_def.h"

#include "taf_ps_api.h"
#include "at_mn_interface.h"
#include "mn_call_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_IMSA_IMPU_MAX_LENGTH (128)
#define AT_IMSA_IMPI_MAX_LENGTH (128)
#define AT_IMSA_DOMAIN_MAX_LENGTH (128)


/* equals IMSA_MAX_CALL_NUMBER_LENGTH */
#define AT_IMSA_CALL_ASCII_NUM_MAX_LENGTH (40)

#define AT_IMSA_PHONECONTEXT_MAX_LENGTH (128)

#define AT_IMSA_PUBLICEUSERID_MAX_LENGTH (128)
#define AT_IMSA_IPV4_ADDR_LEN (4)

#define AT_IMSA_IPV6_ADDR_LEN (16)

#define IMSA_IMS_STRING_LENGTH (128)
#define AT_IMSA_MAX_SMSPSI_LEN (128)

#define IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN (0x0100)

/* ��ǰimsע��ʧ�������ϱ�����ַ�������Ϊ255 */
#define IMSA_AT_REG_FAIL_CAUSE_STR_MAX_LEN (256)
#define IMSA_AT_PDN_FAIL_CAUSE_WIFI_NW_CAUSE_SECTION_BEGIN (0x10000)
#define AT_IMSA_MAX_EMERGENCY_AID_LEN (256)

#define AT_IMSA_USER_AGENT_STR_LEN (32)

#define IMSA_AT_DIALOG_NOTIFY_MAX_LEN (500)
#define IMSA_AT_RTT_REASON_TEXT_MAX_LEN (64)
#define AT_IMSA_CONTENT_TYPE_MAX_LEN 64
#define AT_IMSA_FUSIONCALL_DATA_MAX_LEN 500

enum AT_IMSA_MsgType {
    /* AT->IMSA */
    /* _H2ASN_MsgChoice AT_IMSA_CiregSetReq */
    ID_AT_IMSA_CIREG_SET_REQ = 0x0001,
    /* _H2ASN_MsgChoice AT_IMSA_CiregQryReq */
    ID_AT_IMSA_CIREG_QRY_REQ = 0x0002,
    /* _H2ASN_MsgChoice AT_IMSA_CirepSetReq */
    ID_AT_IMSA_CIREP_SET_REQ = 0x0003,
    /* _H2ASN_MsgChoice AT_IMSA_CirepQryReq */
    ID_AT_IMSA_CIREP_QRY_REQ = 0x0004,
    /* _H2ASN_MsgChoice AT_IMSA_VolteimpuQryReq */
    ID_AT_IMSA_VOLTEIMPU_QRY_REQ = 0x0005,



    /* _H2ASN_MsgChoice AT_IMSA_ImsRegDomainQryReq */
    ID_AT_IMSA_IMS_REG_DOMAIN_QRY_REQ = 0x0006,
    /* _H2ASN_MsgChoice AT_IMSA_ImsCtrlMsg */
    ID_AT_IMSA_IMS_CTRL_MSG = 0x0007,

    /* _H2ASN_MsgChoice AT_IMSA_CallEncryptSetReq */
    ID_AT_IMSA_CALL_ENCRYPT_SET_REQ = 0x0008,

    /* _H2ASN_MsgChoice AT_IMSA_RoamingImsQryReq */
    ID_AT_IMSA_ROAMING_IMS_QRY_REQ = 0x0009,

    /* _H2ASN_MsgChoice AT_IMSA_PcscfSetReq */
    ID_AT_IMSA_PCSCF_SET_REQ = 0x000A,
    /* _H2ASN_MsgChoice AT_IMSA_PcscfQryReq */
    ID_AT_IMSA_PCSCF_QRY_REQ = 0x000B,
    /* _H2ASN_MsgChoice AT_IMSA_DmdynSetReq */
    ID_AT_IMSA_DMDYN_SET_REQ = 0x000C,
    /* _H2ASN_MsgChoice AT_IMSA_DmdynQryReq */
    ID_AT_IMSA_DMDYN_QRY_REQ = 0x000D,

    /* _H2ASN_MsgChoice AT_IMSA_ImstimerSetReq */
    ID_AT_IMSA_IMSTIMER_SET_REQ = 0x000E,
    /* _H2ASN_MsgChoice AT_IMSA_ImstimerQryReq */
    ID_AT_IMSA_IMSTIMER_QRY_REQ = 0x000F,
    /* _H2ASN_MsgChoice AT_IMSA_SmspsiSetReq */
    ID_AT_IMSA_SMSPSI_SET_REQ = 0x0010,
    /* _H2ASN_MsgChoice AT_IMSA_SmspsiQryReq */
    ID_AT_IMSA_SMSPSI_QRY_REQ = 0x0011,
    ID_AT_IMSA_DMUSER_QRY_REQ = 0x0012,

    /* _H2ASN_MsgChoice AT_IMSA_NicknameSetReq */
    ID_AT_IMSA_NICKNAME_SET_REQ = 0x0013,
    /* _H2ASN_MsgChoice AT_IMSA_NicknameQryReq */
    ID_AT_IMSA_NICKNAME_QRY_REQ = 0x0014,
    /* _H2ASN_MsgChoice AT_IMSA_BatteryInfoSetReq */
    ID_AT_IMSA_BATTERYINFO_SET_REQ = 0x0015,
    /* _H2ASN_MsgChoice AT_IMSA_VolteregNtf */
    ID_AT_IMSA_VOLTEREG_NTF = 0x0016,

    /* _H2ASN_MsgChoice AT_IMSA_VolteimpiQryReq */
    ID_AT_IMSA_VOLTEIMPI_QRY_REQ = 0x0018,
    /* _H2ASN_MsgChoice AT_IMSA_VoltedomainQryReq */
    ID_AT_IMSA_VOLTEDOMAIN_QRY_REQ = 0x0019,
    /* _H2ASN_MsgChoice AT_IMSA_RegerrReportSetReq */
    ID_AT_IMSA_REGERR_REPORT_SET_REQ = 0x001A,
    /* _H2ASN_MsgChoice AT_IMSA_RegerrReportQryReq */
    ID_AT_IMSA_REGERR_REPORT_QRY_REQ = 0x001B,

    ID_AT_IMSA_IMS_IP_CAP_SET_REQ = 0x001C,
    ID_AT_IMSA_IMS_IP_CAP_QRY_REQ = 0x001D,

    /* _H2ASN_MsgChoice AT_IMSA_ImsSrvStatReportSetReq */
    ID_AT_IMSA_IMS_SRV_STAT_RPT_SET_REQ = 0x001E,
    /* _H2ASN_MsgChoice AT_IMSA_ImsSrvStatReportQryReq */
    ID_AT_IMSA_IMS_SRV_STAT_RPT_QRY_REQ = 0x001F,
    /* _H2ASN_MsgChoice AT_IMSA_ImsServiceStatusQryReq */
    ID_AT_IMSA_IMS_SERVICE_STATUS_QRY_REQ = 0x0020,

    ID_AT_IMSA_EMERGENCY_AID_SET_REQ = 0x0021,
    /* _H2ASN_MsgChoice AT_IMSA_DmRcsCfgSetReq */
    ID_AT_IMSA_DM_RCS_CFG_SET_REQ = 0x0022,

    ID_AT_IMSA_USER_AGENT_CFG_SET_REQ = 0x0023,

    /* _H2ASN_MsgChoice AT_IMSA_VicecfgSetReq */
    ID_AT_IMSA_VICECFG_SET_REQ = 0x0024,
    /* _H2ASN_MsgChoice AT_IMSA_VicecfgQryReq */
    ID_AT_IMSA_VICECFG_QRY_REQ = 0x0025,
    /* _H2ASN_MsgChoice AT_IMSA_RttcfgSetReq */
    ID_AT_IMSA_RTTCFG_SET_REQ = 0x0026,
    /* _H2ASN_MsgChoice AT_IMSA_RttModifySetReq */
    ID_AT_IMSA_RTT_MODIFY_SET_REQ = 0x0027,
    /* _H2ASN_MsgChoice AT_IMSA_TransportTypeSetReq */
    ID_AT_IMSA_TRANSPORT_TYPE_SET_REQ = 0x0028,
    /* _H2ASN_MsgChoice AT_IMSA_TransportTypeQryReq */
    ID_AT_IMSA_TRANSPORT_TYPE_QRY_REQ = 0x0029,
    ID_AT_IMSA_ECALL_ECONTENT_TYPE_SET_REQ = 0x002A,
    ID_AT_IMSA_ECALL_ECONTENT_TYPE_QRY_REQ = 0x002B,
    /* _H2ASN_MsgChoice AT_IMSA_ImsUrspSetReq */
    ID_AT_IMSA_IMS_URSP_SET_REQ = 0x002C,
    ID_AT_IMSA_FUSIONCALL_CTRL_MSG = 0x002D,
    ID_AT_IMSA_EMC_PDN_ACTIVATE_REQ = 0x002E,
    ID_AT_IMSA_EMC_PDN_DEACTIVATE_REQ = 0x002F,
    /* IMSA->AT */
    /* _H2ASN_MsgChoice IMSA_AT_CiregSetCnf */
    ID_IMSA_AT_CIREG_SET_CNF = 0x1001,
    /* _H2ASN_MsgChoice IMSA_AT_CiregQryCnf */
    ID_IMSA_AT_CIREG_QRY_CNF = 0x1002,
    /* _H2ASN_MsgChoice IMSA_AT_CirepSetCnf */
    ID_IMSA_AT_CIREP_SET_CNF = 0x1003,
    /* _H2ASN_MsgChoice IMSA_AT_CirepQryCnf */
    ID_IMSA_AT_CIREP_QRY_CNF = 0x1004,

    /* _H2ASN_MsgChoice IMSA_AT_VolteimpuQryCnf */
    ID_IMSA_AT_VOLTEIMPU_QRY_CNF = 0x1005,



    /* _H2ASN_MsgChoice IMSA_AT_CireguInd */
    ID_IMSA_AT_CIREGU_IND = 0x1006,
    /* _H2ASN_MsgChoice IMSA_AT_CirephInd */
    ID_IMSA_AT_CIREPH_IND = 0x1007,
    /* _H2ASN_MsgChoice IMSA_AT_CirepiInd */
    ID_IMSA_AT_CIREPI_IND = 0x1008,

    /* _H2ASN_MsgChoice IMSA_AT_VtPdpActivateInd */
    ID_IMSA_AT_VT_PDP_ACTIVATE_IND = 0x1009,
    /* _H2ASN_MsgChoice IMSA_AT_VtPdpDeactivateInd */
    ID_IMSA_AT_VT_PDP_DEACTIVATE_IND = 0x100A,

    /* _H2ASN_MsgChoice IMSA_AT_MtStatesInd */
    ID_IMSA_AT_MT_STATES_IND = 0x100B,

    /* _H2ASN_MsgChoice IMSA_AT_ImsRegDomainQryCnf */
    ID_IMSA_AT_IMS_REG_DOMAIN_QRY_CNF = 0x100C,
    /* _H2ASN_MsgChoice IMSA_AT_ImsCtrlMsg */
    ID_IMSA_AT_IMS_CTRL_MSG = 0x100D,

    /* _H2ASN_MsgChoice IMSA_AT_CallEncryptSetCnf */
    ID_IMSA_AT_CALL_ENCRYPT_SET_CNF = 0x100E,

    /* _H2ASN_MsgChoice IMSA_AT_RoamingImsQryCnf */
    ID_IMSA_AT_ROAMING_IMS_QRY_CNF = 0x100F,

    /* _H2ASN_MsgChoice IMSA_AT_ImsRatHandoverInd */
    ID_IMSA_AT_IMS_RAT_HANDOVER_IND = 0x1010,
    /* _H2ASN_MsgChoice IMSA_AT_ImsSrvStatusUpdateInd */
    ID_IMSA_AT_IMS_SRV_STATUS_UPDATE_IND = 0x1011,

    /* _H2ASN_MsgChoice IMSA_AT_PcscfSetCnf */
    ID_IMSA_AT_PCSCF_SET_CNF = 0x1012,
    /* _H2ASN_MsgChoice IMSA_AT_PcscfQryCnf */
    ID_IMSA_AT_PCSCF_QRY_CNF = 0x1013,

    /* _H2ASN_MsgChoice IMSA_AT_DmdynSetCnf */
    ID_IMSA_AT_DMDYN_SET_CNF = 0x1014,
    /* _H2ASN_MsgChoice IMSA_AT_DmdynQryCnf */
    ID_IMSA_AT_DMDYN_QRY_CNF = 0x1015,

    /* _H2ASN_MsgChoice IMSA_AT_DmcnInd */
    ID_IMSA_AT_DMCN_IND = 0x1016,

    /* _H2ASN_MsgChoice IMSA_AT_ImstimerSetCnf */
    ID_IMSA_AT_IMSTIMER_SET_CNF = 0x1017,
    /* _H2ASN_MsgChoice IMSA_AT_ImstimerQryCnf */
    ID_IMSA_AT_IMSTIMER_QRY_CNF = 0x1018,
    /* _H2ASN_MsgChoice IMSA_AT_SmspsiSetCnf */
    ID_IMSA_AT_SMSPSI_SET_CNF = 0x1019,
    /* _H2ASN_MsgChoice IMSA_AT_SmspsiSetCnf */
    ID_IMSA_AT_SMSPSI_QRY_CNF = 0x101A,
    ID_IMSA_AT_DMUSER_QRY_CNF = 0x101B,

    /* _H2ASN_MsgChoice IMSA_AT_NicknameSetCnf */
    ID_IMSA_AT_NICKNAME_SET_CNF = 0x101C,
    /* _H2ASN_MsgChoice IMSA_AT_NicknameQryCnf */
    ID_IMSA_AT_NICKNAME_QRY_CNF = 0x101D,
    /* _H2ASN_MsgChoice IMSA_AT_RegFailInd */
    ID_IMSA_AT_REGFAIL_IND = 0x101E,
    /* _H2ASN_MsgChoice IMSA_AT_BatteryInfoSetCnf */
    ID_IMSA_AT_BATTERYINFO_SET_CNF = 0x101F,

    /* _H2ASN_MsgChoice IMSA_AT_VolteimpiQryCnf */
    ID_IMSA_AT_VOLTEIMPI_QRY_CNF = 0x1021,
    /* _H2ASN_MsgChoice IMSA_AT_VoltedomainQryCnf */
    ID_IMSA_AT_VOLTEDOMAIN_QRY_CNF = 0x1022,
    /* _H2ASN_MsgChoice IMSA_AT_RegerrReportSetCnf */
    ID_IMSA_AT_REGERR_REPORT_SET_CNF = 0x1023,
    /* _H2ASN_MsgChoice IMSA_AT_RegerrReportQryCnf */
    ID_IMSA_AT_REGERR_REPORT_QRY_CNF = 0x1024,
    /* _H2ASN_MsgChoice IMSA_AT_RegerrReportInd */
    ID_IMSA_AT_REGERR_REPORT_IND = 0x1025,

    /* _H2ASN_MsgChoice IMSA_AT_IMS_UE_CAP_SET_CNF_STRU */
    ID_IMSA_AT_IMS_IP_CAP_SET_CNF = 0x1026,
    /* _H2ASN_MsgChoice IMSA_AT_IMS_UE_CAP_QRY_CNF_STRU */
    ID_IMSA_AT_IMS_IP_CAP_QRY_CNF = 0x1027,

    /* _H2ASN_MsgChoice IMSA_AT_EmcPdnActivateInd */
    ID_IMSA_AT_EMC_PDN_ACTIVATE_IND = 0x1028,
    /* _H2ASN_MsgChoice IMSA_AT_EmcPdnDeactivateInd */
    ID_IMSA_AT_EMC_PDN_DEACTIVATE_IND = 0x1029,

    /* _H2ASN_MsgChoice IMSA_AT_ImsSrvStatReportSetCnf */
    ID_IMSA_AT_IMS_SRV_STAT_RPT_SET_CNF = 0x102A,
    /* _H2ASN_MsgChoice IMSA_AT_ImsSrvStatReportQryCnf */
    ID_IMSA_AT_IMS_SRV_STAT_RPT_QRY_CNF = 0x102B,
    /* _H2ASN_MsgChoice IMSA_AT_ImsServiceStatusQryCnf */
    ID_IMSA_AT_IMS_SERVICE_STATUS_QRY_CNF = 0x102C,

    ID_IMSA_AT_EMERGENCY_AID_SET_CNF = 0x102D,
    /* _H2ASN_MsgChoice IMSA_AT_CallAltSrvInd */
    ID_IMSA_AT_CALL_ALT_SRV_IND = 0x102E,
    /* _H2ASN_MsgChoice IMSA_AT_DmRcsCfgSetCnf */
    ID_IMSA_AT_DM_RCS_CFG_SET_CNF = 0x102F,

    /* _H2ASN_MsgChoice IMSA_AT_UserAgentCfgSetCnf */
    ID_IMSA_AT_USER_AGENT_CFG_SET_CNF = 0x1030,

    ID_IMSA_AT_IMPU_TYPE_IND = 0x1031,

    /* _H2ASN_MsgChoice IMSA_AT_VicecfgSetCnf */
    ID_IMSA_AT_VICECFG_SET_CNF = 0x1032,
    /* _H2ASN_MsgChoice IMSA_AT_VicecfgQryCnf */
    ID_IMSA_AT_VICECFG_QRY_CNF = 0x1033,
    /* _H2ASN_MsgChoice IMSA_AT_DialogNotify */
    ID_IMSA_AT_DIALOG_NOTIFY = 0x1034,
    /* _H2ASN_MsgChoice IMSA_AT_RttcfgSetCnf */
    ID_IMSA_AT_RTTCFG_SET_CNF = 0x1035,
    /* _H2ASN_MsgChoice IMSA_AT_RttModifySetCnf */
    ID_IMSA_AT_RTT_MODIFY_SET_CNF = 0x1036,
    /* _H2ASN_MsgChoice IMSA_AT_RttEventInd */
    ID_IMSA_AT_RTT_EVENT_IND = 0x1037,
    /* _H2ASN_MsgChoice IMSA_AT_RttErrorInd */
    ID_IMSA_AT_RTT_ERROR_IND = 0x1038,
    /* _H2ASN_MsgChoice IMSA_AT_TransportTypeSetCnf */
    ID_IMSA_AT_TRANSPORT_TYPE_SET_CNF = 0x1039,
    /* _H2ASN_MsgChoice IMSA_AT_TransportTypeQryCnf */
    ID_IMSA_AT_TRANSPORT_TYPE_QRY_CNF = 0x103A,
    ID_IMSA_AT_ECALL_ECONTENT_TYPE_SET_CNF = 0x103B,
    ID_IMSA_AT_ECALL_ECONTENT_TYPE_QRY_CNF = 0x103C,
    /* _H2ASN_MsgChoice IMSA_AT_ImsUrspSetCnf */
    ID_IMSA_AT_IMS_URSP_SET_CNF = 0x103D,
    ID_IMSA_AT_FUSIONCALL_CTRL_MSG = 0x103E,
    ID_IMSA_AT_EMC_PDN_ACTIVATE_CNF = 0x103F,
    ID_IMSA_AT_EMC_PDN_DEACTIVATE_CNF = 0x1040,
    ID_AT_IMSA_MSG_BUTT
};
typedef VOS_UINT32 AT_IMSA_MsgTypeUint32;


enum AT_IMSA_ImsRegStateReport {
    AT_IMSA_IMS_REG_STATE_DISABLE_REPORT = 0,
    AT_IMSA_IMS_REG_STATE_ENABLE_REPORT,
    AT_IMSA_IMS_REG_STATE_ENABLE_EXTENDED_REPROT,

    AT_IMSA_IMS_REG_STATE_REPROT_BUTT
};
typedef VOS_UINT32 AT_IMSA_ImsRegStateReportUint32;


enum AT_IMSA_CcwaiMode {
    AT_IMSA_CCWAI_MODE_DISABLE = 0,
    AT_IMSA_CCWAI_MODE_ENABLE,

    AT_IMSA_CCWAI_MODE_BUTT
};
typedef VOS_UINT8 AT_IMSA_CcwaiModeUint8;



enum AT_IMSA_ImsvopsCapability {
    AT_IMSA_NW_NOT_SUPORT_IMSVOPS = 0,
    AT_IMSA_NW_SUPORT_IMSVOPS,

    AT_IMSA_IMSVOPS_CAPABILITY_BUTT
};
typedef VOS_UINT32 AT_IMSA_ImsvopsCapabilityUint32;


enum AT_IMSA_CirepReport {
    AT_IMSA_CIREP_REPORT_DISENABLE = 0,
    AT_IMSA_CIREP_REPORT_ENABLE,

    AT_IMSA_CIREP_REPORT_BUTT
};
typedef VOS_UINT32 AT_IMSA_CirepReportUint32;


enum AT_IMSA_SrvccHandvoer {
    AT_IMSA_SRVCC_HANDOVER_STARTED = 0,
    AT_IMSA_SRVCC_HANDOVER_SUCCESS,
    AT_IMSA_SRVCC_HANDOVER_CANCEL,
    AT_IMSA_SRVCC_HANDOVER_FAILURE,

    AT_IMSA_SRVCC_HANDOVER_BUTT
};
typedef VOS_UINT32 AT_IMSA_SrvccHandvoerUint32;


enum AT_IMSA_BatteryStatus {
    AT_IMSA_BATTERY_STATUS_NORMAL = 0, /* ���� */
    AT_IMSA_BATTERY_STATUS_LOW,        /* �͵� */
    AT_IMSA_BATTERY_STATUS_EXHAUST,    /* ��غľ� */

    AT_IMSA_BATTERY_STATUS_BUTT
};
typedef VOS_UINT8 AT_IMSA_BatteryStatusUint8;


enum IMSA_AT_RegFailCode {
    IMSA_AT_REG_FAIL_CODE_PERM = 1,

    IMSA_AT_REG_FAIL_CODE_BUTT
};
typedef VOS_UINT8 IMSA_AT_RegFailCodeUint8;


enum IMSA_AT_ImsRatType {
    IMSA_AT_IMS_RAT_TYPE_LTE   = 0x00,
    IMSA_AT_IMS_RAT_TYPE_WIFI  = 0x01,
    IMSA_AT_IMS_RAT_TYPE_UTRAN = 0x02,
    IMSA_AT_IMS_RAT_TYPE_GSM   = 0x03,
    IMSA_AT_IMS_RAT_TYPE_NR = 0x04,

    IMSA_AT_IMS_RAT_TYPE_BUTT = 0xFF
};
typedef VOS_UINT8 IMSA_AT_ImsRatTypeUint8;

enum AT_IMSA_RoamingImsSupport {
    AT_IMSA_ROAMING_IMS_SUPPORT     = 1,
    AT_IMSA_ROAMING_IMS_NOT_SUPPORT = 2,

    AT_IMSA_ROAMING_IMS_BUTT
};
typedef VOS_UINT32 AT_IMSA_RoamingImsSupportUint32;


enum IMSA_AT_ImsRegDomainType {
    IMSA_AT_IMS_REG_DOMAIN_TYPE_LTE   = 0x00,
    IMSA_AT_IMS_REG_DOMAIN_TYPE_WIFI  = 0x01,
    IMSA_AT_IMS_REG_DOMAIN_TYPE_UTRAN = 0x02,
    IMSA_AT_IMS_REG_DOMAIN_TYPE_GSM   = 0x03,
    IMSA_AT_IMS_REG_DOMAIN_TYPE_NR = 0x04,
    IMSA_AT_IMS_REG_DOMAIN_TYPE_UNKNOWN = 0xFF
};
typedef VOS_UINT8 IMSA_AT_ImsRegDomainTypeUint8;


enum IMSA_AT_ImsRatHoStatus {
    IMSA_AT_IMS_RAT_HO_STATUS_SUCCESS       = 0x00, /* RAT handover was successful */
    IMSA_AT_IMS_RAT_HO_STATUS_FAILURE       = 0x01, /* RAT handover was failed */
    IMSA_AT_IMS_RAT_HO_STATUS_NOT_TRIGGERED = 0x02, /* RAT handover could nut be triggered */
    IMSA_AT_IMS_RAT_HO_STATUS_BUTT
};
typedef VOS_UINT8 IMSA_AT_ImsRatHoStatusUint8;


enum IMSA_AT_ImsServiceStatus {
    IMSA_AT_IMS_SERVICE_STATUS_NO_SERVICE      = 0x00, /* IMS is no service */
    IMSA_AT_IMS_SERVICE_STATUS_LIMITED_SERVICE = 0x01, /* IMS is in limited service */
    IMSA_AT_IMS_SERVICE_STATUS_FULL_SERVICE    = 0x02, /* IMS is in full service */
    IMSA_AT_IMS_SERVICE_STATUS_SOS_PENDING = 0x08, /* IMS is in sos pending */
    IMSA_AT_IMS_SERVICE_STATUS_BUTT
};
typedef VOS_UINT8 IMSA_AT_ImsServiceStatusUint8;



enum IMSA_AT_HoCause {
    IMSA_AT_HO_CAUSE_SUCCESS                     = 0, /* *< HO����ִ�гɹ� */
    IMSA_AT_HO_CAUSE_FAIL_PARA_ERR               = 1, /* *< HO����ִ��ʧ�ܣ����ڲ������� */
    IMSA_AT_HO_CAUSE_FAIL_CN_REJ                 = 2, /* *< HO����ִ��ʧ�ܣ����ڱ�����ܾ� */
    IMSA_AT_HO_CAUSE_FAIL_TIMER_EXP              = 3, /* *< HO����ִ��ʧ�ܣ����ڶ�ʱ����ʱ */
    IMSA_AT_HO_CAUSE_FAIL_CONN_RELEASING         = 4, /* *< HO����ִ��ʧ�ܣ����������ͷ����� */
    IMSA_AT_HO_CAUSE_FAIL_PDP_ACTIVATE_LIMIT     = 5, /* *< HO����ִ��ʧ�ܣ����ڼ���ĳ��������� */
    IMSA_AT_HO_CAUSE_FAIL_SAME_APN_OPERATING     = 6, /* *< HO����ִ��ʧ�ܣ�����APS����ִ�в��� */
    IMSA_AT_HO_CAUSE_FAIL_TEMP_FORBIDDEN         = 7, /* *< HO����ִ��ʧ�ܣ��յ���ʱ����ԭ��ֵ�������಻��Ӧ */
    IMSA_AT_HO_CAUSE_FAIL_PERM_FORBIDDEN         = 8, /* *< HO����ִ��ʧ�ܣ��յ����ñ���ԭ��ֵ */
    IMSA_AT_HO_CAUSE_FAIL_WIFI_READY_IND_TIMEOUT = 9, /* *< HO����ִ��ʧ�ܣ�����WIFI�·���READY IND��ʱ */

    IMSA_AT_HO_CAUSE_FAIL_OHTERS           = 11, /* *< HO����ִ��ʧ�ܣ���������ԭ�� */
    IMSA_AT_HO_CAUSE_FAIL_NO_DSDS_RESOURCE = 12, /* *< HO����ִ��ʧ�ܣ�����DSDS������������Դʧ�� */

    IMSA_AT_HO_CAUSE_BUTT
};
typedef VOS_UINT32 IMSA_AT_HoCauseUint32;

enum IMSA_AT_PcscfSrc {
    IMSA_AT_PCSCF_SRC_DM_DEFAULT = 0, /* P-CSCF��ַ��Դ��DMĬ������ */
    IMSA_AT_PCSCF_SRC_DM         = 1, /* P-CSCF��ַ��Դ��DM������ */
    IMSA_AT_PCSCF_SRC_PCO        = 2, /* P-CSCF��ַ��Դ��PCO */

    IMSA_AT_PCSCF_SRC_BUTT
};
typedef VOS_UINT32 IMSA_AT_PcscfSrcUint32;

enum AT_IMSA_VoiceDomain {
    AT_IMSA_VOICE_DOMAIN_CS_ONLY          = 0, /* CS voice only */
    AT_IMSA_VOICE_DOMAIN_IMS_PS_ONLY      = 1, /* IMS PS voice only */
    AT_IMSA_VOICE_DOMAIN_CS_PREFERRED     = 2, /* CS voice preferred, IMS PS voice as secondary */
    AT_IMSA_VOICE_DOMAIN_IMS_PS_PREFERRED = 3, /* IMS PS voice preferred, CS voice as secondary  */

    AT_IMSA_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32 AT_IMSA_VoiceDomainUint32;

enum IMSA_AT_RegErrType {
    IMSA_AT_REG_ERR_TYPE_PDN_FAIL     = 0x00,
    IMSA_AT_REG_ERR_TYPE_IMS_REG_FAIL = 0x01,
    IMSA_AT_REG_ERR_TYPE_BUTT
};
typedef VOS_UINT8 IMSA_AT_RegErrTypeUint8;

enum IMSA_AT_PdnFailCause {
    IMSA_AT_PDN_FAIL_CAUSE_PARA_ERR = 1,
    /* IMSA_AT_PDN_FAIL_CAUSE_CN_REJ                               = 2, */
    IMSA_AT_PDN_FAIL_CAUSE_TIMER_EXP                              = 3,
    IMSA_AT_PDN_FAIL_CAUSE_CONN_RELEASING                         = 4,
    IMSA_AT_PDN_FAIL_CAUSE_PDP_ACTIVATE_LIMIT                     = 5,
    IMSA_AT_PDN_FAIL_CAUSE_SAME_APN_OPERATING                     = 6,
    IMSA_AT_PDN_FAIL_CAUSE_TEMP_FORBIDDEN                         = 7,
    IMSA_AT_PDN_FAIL_CAUSE_PERM_FORBIDDEN                         = 8,
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_READY_IND_TIMEOUT                 = 9,
    IMSA_AT_PDN_FAIL_CAUSE_HO_NEW_PDN_SETUP_FAIL                  = 10,
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_FAIL_TEMP_FORBIDDEN               = 11,
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_FAIL_PERM_FORBIDDEN               = 12,
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_FAIL_TEMP_FORBIDDEN_WITH_TIMERLEN = 13,
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_FAIL_LINKLOST                     = 14,
    IMSA_AT_PDN_FAIL_CAUSE_OHTERS                                 = 15,

    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_OPERATOR_DETERMINED_BARRING = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 8),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MBMS_BC_INSUFFICIENT        = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 24),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE        = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 25),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_INSUFFICIENT_RESOURCES      = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 26),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MISSING_OR_UKNOWN_APN       = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 27),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE    = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 28),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_USER_AUTHENTICATION_FAIL    = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 29),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_ACTIVATION_REJECTED_BY_GGSN_SGW_OR_PGW =
        (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 30),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_ACTIVATION_REJECTED_UNSPECIFIED   = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 31),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SERVICE_OPTION_NOT_SUPPORTED      = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 32),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_REQUESTED_SERVICE_NOT_SUBSCRIBED  = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 33),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER     = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 34),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_NSAPI_ALREADY_USED                = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 35),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_REGULAR_DEACTIVATION              = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 36),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_QOS_NOT_ACCEPTED                  = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 37),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_NETWORK_FAILURE                   = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 38),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_REACTIVATION_REQUESTED            = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 39),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_FEATURE_NOT_SUPPORT               = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 40),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SEMANTIC_ERR_IN_TFT               = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 41),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SYNTACTIC_ERR_IN_TFT              = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 42),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_UKNOWN_PDP_CONTEXT                = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 43),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER     = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 44),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER   = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 45),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_PDP_CONTEXT_WITHOUT_TFT_ACTIVATED = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 46),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MULTICAST_GROUP_MEMBERHHSHIP_TIMEOUT =
        (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 47),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_REQUEST_REJECTED_BCM_VIOLATION   = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 48),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_LAST_PDN_DISCONN_NOT_ALLOWED     = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 49),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED       = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 50),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED       = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 51),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 52),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_INFORMATION_NOT_RECEIVED         = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 53),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_PDN_CONNECTION_DOES_NOT_EXIST    = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 54),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SAME_APN_MULTI_PDN_CONNECTION_NOT_ALLOWED =
        (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 55),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_COLLISION_WITH_NW_INITIATED_REQUEST =
        (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 56),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_UNSUPPORTED_QCI_VALUE          = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 59),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_BEARER_HANDLING_NOT_SUPPORTED  = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 60),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_INVALID_TI                     = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 81),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SEMANTICALLY_INCORRECT_MESSAGE = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 95),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_INVALID_MANDATORY_INFO         = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 96),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MSG_TYPE_NON_EXISTENT          = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 97),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MSG_TYPE_NOT_COMPATIBLE        = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 98),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_IE_NON_EXISTENT                = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 99),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_CONDITIONAL_IE_ERR             = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 100),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_MSG_NOT_COMPATIBLE             = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 101),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED       = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 111),
    IMSA_AT_PDN_FAIL_CAUSE_SM_NW_APN_RESTRICTION_INCOMPATIBLE   = (IMSA_AT_PDN_FAIL_CAUSE_SM_NW_SECTION_BEGIN + 112),
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_NW_CAUSE_BEGIN                  = (IMSA_AT_PDN_FAIL_CAUSE_WIFI_NW_CAUSE_SECTION_BEGIN),
    IMSA_AT_PDN_FAIL_CAUSE_WIFI_NW_CAUSE_END = (IMSA_AT_PDN_FAIL_CAUSE_WIFI_NW_CAUSE_SECTION_BEGIN + 0xFFFF),
    IMSA_AT_PDN_FAIL_CAUSE_BUTT
};
typedef VOS_UINT32 IMSA_AT_PdnFailCauseUint32;

enum IMSA_AT_RegFailCause {
    IMSA_AT_REG_FAIL_CAUSE_FAIL              = 1,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_REMOTE       = 2,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_TIMEOUT      = 3,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_TRANSPORT    = 4,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_SA           = 5,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_MT_DEREG     = 6,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_NO_ADDR_PAIR = 7,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_LACK_PARAM   = 8,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_FORBIDDEN    = 9,
    IMSA_AT_REG_FAIL_CAUSE_FAIL_REREG_FAIL   = 10,

    /* IMS����ܾ�ԭ��ֵ 300~699,ֱ�������֣�����һһ�о�  */

    IMSA_AT_REG_FAIL_CAUSE_BUTT = 700,
};
typedef VOS_UINT32 IMSA_AT_RegFailCauseUint32;


enum AT_IMSA_ImsSrvStatReport {
    AT_IMSA_IMS_SRV_STAT_DISABLE_REPORT = 0,
    AT_IMSA_IMS_SRV_STAT_ENABLE_REPORT  = 1,

    AT_IMSA_IMS_SRV_STAT_REPROT_BUTT
};
typedef VOS_UINT32 AT_IMSA_ImsSrvStatReportUint32;


enum AT_IMSA_ImpuType {
    AT_IMSA_IMPU_TYPE_MSISDN = 0,
    AT_IMSA_IMPU_TYPE_IMSI   = 1,

    AT_IMSA_IMPU_TYPE_BUTT
};
typedef VOS_UINT32 AT_IMSA_ImpuTypeUint32;


enum AT_IMSA_RttModifyOpt {
    AT_IMSA_RTT_MODIFY_ADD    = 0,
    AT_IMSA_RTT_MODIFY_CLOSE  = 1,
    AT_IMSA_RTT_MODIFY_ALLOW  = 2,
    AT_IMSA_RTT_MODIFY_REJECT = 3,
    AT_IMSA_RTT_MODIFY_OPT_BUTT
};
typedef VOS_UINT8 AT_IMSA_RttModifyOptUint8;


enum IMSA_AT_RttEvent {
    IMSA_AT_RTT_EVENT_ADD      = 0,
    IMSA_AT_RTT_EVENT_CLOSE    = 1,
    IMSA_AT_RTT_EVENT_REQ_ADD  = 2,
    IMSA_AT_RTT_EVENT_PARM_CHG = 3,
    IMSA_AT_RTT_EVENT_BUTT
};
typedef VOS_UINT8 IMSA_AT_RttEventUint8;


enum IMSA_AT_RttEvtReason {
    IMSA_AT_RTT_EVT_REASON_LOC_USER_TRIGGER = 0,
    IMSA_AT_RTT_EVT_REASON_RMT_USER_TRIGGER = 1,
    IMSA_AT_RTT_EVT_REASON_BUTT
};
typedef VOS_UINT8 IMSA_AT_RttEvtReasonUint8;


enum IMSA_AT_RttOperation {
    IMSA_AT_RTT_OPERATION_ADD   = 0,
    IMSA_AT_RTT_OPERATION_CLOSE = 1,
    IMSA_AT_RTT_OPERATION_BUTT
};
typedef VOS_UINT8 IMSA_AT_RttOperationUint8;

enum AT_IMSA_FusionCallDstId {
    AT_IMSA_FUSIONCALL_DST_ID_IMSA   = 0,
    AT_IMSA_FUSIONCALL_DST_ID_IMS = 1,
    AT_IMSA_FUSIONCALL_DST_ID_BUTT
};
typedef VOS_UINT8 AT_IMSA_FusionCallDstIdUint8;


enum IMSA_AT_FusionCallSrcId {
    IMSA_AT_FUSIONCALL_SRC_ID_IMSA   = 0,
    IMSA_AT_FUSIONCALL_SRC_ID_IMS = 1,
    IMSA_AT_FUSIONCALL_SRC_ID_BUTT
};
typedef VOS_UINT8 IMSA_AT_FusionCallSrcIdUint8;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT8      content[4];
} AT_IMSA_Msg;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CnfMsg;


typedef AT_IMSA_Msg AT_IMSA_CiregQryReq;


typedef AT_IMSA_Msg AT_IMSA_CirepQryReq;


typedef AT_IMSA_Msg AT_IMSA_VolteimpuQryReq;



typedef AT_IMSA_Msg AT_IMSA_VolteimpiQryReq;


typedef AT_IMSA_Msg AT_IMSA_VoltedomainQryReq;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT32             msgId; /* _H2ASN_Skip */
    VOS_UINT16             clientId;
    VOS_UINT8              opId;
    AT_IMSA_CcwaiModeUint8 mode;
    VOS_UINT32             srvClass;
} AT_IMSA_CcwaiSetReq;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved;
    VOS_UINT32     result;
} IMSA_AT_CcwaiSetCnf;



typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT32     msgId;   /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved1[1];
    VOS_UINT8      encrypt; /* 0:�����ܣ�1:���� */
    VOS_UINT8      reserved2[3];
} AT_IMSA_CallEncryptSetReq;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved;
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CallEncryptSetCnf;


typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    VOS_UINT32                      msgId; /* _H2ASN_Skip */
    VOS_UINT16                      clientId;
    VOS_UINT8                       opId;
    VOS_UINT8                       reserved[1];
    AT_IMSA_ImsRegStateReportUint32 cireg;
} AT_IMSA_CiregSetReq;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CiregSetCnf;


typedef struct {
    VOS_MSG_HEADER                                 /* _H2ASN_Skip */
    VOS_UINT32                      msgId;         /* _H2ASN_Skip */
    VOS_UINT16                      clientId;
    VOS_UINT8                       opId;
    VOS_UINT8                       reserved[1];

    VOS_UINT32                      opExtInfo : 1; /* +CIREG=2ʱ,��־λ��1 */
    VOS_UINT32                      opSpare : 31;

    AT_IMSA_ImsRegStateReportUint32 cireg;
    VOS_UINT32                      regInfo;
    VOS_UINT32                      extInfo;       /* +CIREG=2ʱ ����չ��Ϣ��ʱ�ظ�0 */
    VOS_UINT32                      result;        /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CiregQryCnf;


typedef struct {
    VOS_MSG_HEADER           /* _H2ASN_Skip */
    VOS_UINT32     msgId;    /* _H2ASN_Skip */
    VOS_UINT16     clientId; /* �����ϱ�ʱ��0X3FFF */
    VOS_UINT8      opId;     /* ��0 */
    VOS_UINT8      reserved[1];

    VOS_UINT32     opExtInfo : 1;
    VOS_UINT32     opSpare : 31;

    VOS_UINT32     regInfo;
    VOS_UINT32     extInfo;
} IMSA_AT_CireguInd;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT32                msgId; /* _H2ASN_Skip */
    VOS_UINT16                clientId;
    VOS_UINT8                 opId;
    VOS_UINT8                 reserved[1];
    AT_IMSA_CirepReportUint32 report;
} AT_IMSA_CirepSetReq;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CirepSetCnf;


typedef struct {
    VOS_MSG_HEADER                          /* _H2ASN_Skip */
    VOS_UINT32                      msgId;  /* _H2ASN_Skip */
    VOS_UINT16                      clientId;
    VOS_UINT8                       opId;
    VOS_UINT8                       reserved[1];
    AT_IMSA_CirepReportUint32       report;
    AT_IMSA_ImsvopsCapabilityUint32 imsVops;
    VOS_UINT32                      result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_CirepQryCnf;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    VOS_UINT32     impuLen;
    VOS_CHAR       impu[AT_IMSA_IMPU_MAX_LENGTH];
    VOS_UINT32     impuLenVirtual;
    VOS_CHAR       impuVirtual[AT_IMSA_IMPU_MAX_LENGTH];
} IMSA_AT_VolteimpuQryCnf;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    VOS_UINT32     impiLen;
    VOS_CHAR       impi[AT_IMSA_IMPI_MAX_LENGTH];
} IMSA_AT_VolteimpiQryCnf;

typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    VOS_UINT32     domainLen;
    VOS_CHAR       domain[AT_IMSA_DOMAIN_MAX_LENGTH];
} IMSA_AT_VoltedomainQryCnf;


typedef AT_IMSA_Msg AT_IMSA_RoamingImsQryReq;


typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    VOS_UINT32                      msgId; /* _H2ASN_Skip */
    VOS_UINT16                      clientId;
    VOS_UINT8                       opId;
    VOS_UINT8                       reserved[1];
    AT_IMSA_RoamingImsSupportUint32 roamingImsSupportFlag;
    VOS_UINT32                      result;
} IMSA_AT_RoamingImsQryCnf;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT32                  msgId;    /* _H2ASN_Skip */
    VOS_UINT16                  clientId; /* �����ϱ�ʱ��0X3FFF */
    VOS_UINT8                   opId;     /* ��0 */
    VOS_UINT8                   reserved[1];
    AT_IMSA_SrvccHandvoerUint32 handover;
} IMSA_AT_CirephInd;


typedef struct {
    VOS_MSG_HEADER                            /* _H2ASN_Skip */
    VOS_UINT32                      msgId;    /* _H2ASN_Skip */
    VOS_UINT16                      clientId; /* �����ϱ�ʱ��0X3FFF */
    VOS_UINT8                       opId;     /* ��0 */
    VOS_UINT8                       reserved[1];
    AT_IMSA_ImsvopsCapabilityUint32 imsvops;
} IMSA_AT_CirepiInd;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32              msgId; /* _H2ASN_Skip */
    VOS_UINT16              clientId;
    VOS_UINT8               opId;
    IMSA_AT_ImsRatTypeUint8 ratType; /* ע���� */
    TAF_PDP_Addr            pdpAddr;
    TAF_PDP_Dns             ipv4Dns;
    TAF_PDP_Ipv6Dns         ipv6Dns;
} IMSA_AT_VtPdpActivateInd;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    VOS_UINT32              msgId;  /* _H2ASN_Skip */
    VOS_UINT16              clientId;
    VOS_UINT8               opId;
    TAF_PDP_TypeUint8       pdpType;
    IMSA_AT_ImsRatTypeUint8 ratType; /* ע���� */
    VOS_UINT8               reserved[3];
} IMSA_AT_VtPdpDeactivateInd;

/*
 * �ṹ˵��: ID_IMSA_AT_EMC_PDN_ACTIVATE_IND ��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32      msgId; /* _H2ASN_Skip */
    VOS_UINT16      clientId;
    VOS_UINT8       opId;
    VOS_UINT8       reaerved;
    TAF_PDP_Addr    pdpAddr;
    TAF_PDP_Dns     ipv4Dns;
    TAF_PDP_Ipv6Dns ipv6Dns;
    VOS_UINT16      mtu;
    VOS_UINT16      reserved;
} IMSA_AT_EmcPdnActivateInd;

/*
 * �ṹ˵��: ID_IMSA_AT_EMC_PDN_DEACTIVATE_IND ��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER           /* _H2ASN_Skip */
    VOS_UINT32        msgId; /* _H2ASN_Skip */
    VOS_UINT16        clientId;
    VOS_UINT8         opId;
    TAF_PDP_TypeUint8 pdpType;
} IMSA_AT_EmcPdnDeactivateInd;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     causeCode;
    VOS_UINT8      mtStatus;
    VOS_UINT8      rsv[3];
    VOS_UINT8      asciiCallNum[AT_IMSA_CALL_ASCII_NUM_MAX_LENGTH];
} IMSA_AT_MtStatesInd;


typedef AT_IMSA_Msg AT_IMSA_ImsRegDomainQryReq;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                    msgId; /* _H2ASN_Skip */
    VOS_UINT16                    clientId;
    VOS_UINT8                     opId;
    IMSA_AT_ImsRegDomainTypeUint8 imsRegDomain;
} IMSA_AT_ImsRegDomainQryCnf;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     wifiMsgLen;
    VOS_UINT8      wifiMsg[4];
} AT_IMSA_ImsCtrlMsg;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      opId;
    VOS_UINT8      reserved[1];
    VOS_UINT32     wifiMsgLen;
    VOS_UINT8      wifiMsg[4];
} IMSA_AT_ImsCtrlMsg;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT32                  msgId;    /* _H2ASN_Skip */
    VOS_UINT16                  clientId; /* �����ϱ�ʱ��0X3FFF */
    VOS_UINT8                   opId;     /* ��0 */
    IMSA_AT_ImsRatHoStatusUint8 hoStatus; /* �л�״̬ */
    IMSA_AT_ImsRatTypeUint8     srcRat;   /* ԭ��IMSע���� */
    IMSA_AT_ImsRatTypeUint8     dstRat;   /* Ŀ��IMSע���� */
    VOS_UINT8                   reserved[2];
    IMSA_AT_HoCauseUint32       cause;    /* �л�ʧ��ԭ��ֵ */
} IMSA_AT_ImsRatHandoverInd;


typedef struct {
    IMSA_AT_ImsServiceStatusUint8 smsSrvStatus;  /* ���ŵ�IMS����״̬ */
    IMSA_AT_ImsRatTypeUint8       smsSrvRat;     /* ���ŵ�IMS������ */
    IMSA_AT_ImsServiceStatusUint8 voIpSrvStatus; /* VoIP��IMS����״̬ */
    IMSA_AT_ImsRatTypeUint8       voIpSrvRat;    /* VoIP��IMS������ */
    IMSA_AT_ImsServiceStatusUint8 vtSrvStatus;   /* VT��IMS����״̬ */
    IMSA_AT_ImsRatTypeUint8       vtSrvRat;      /* VT��IMS������ */
    IMSA_AT_ImsServiceStatusUint8 vsSrvStatus;   /* VS��IMS����״̬ */
    IMSA_AT_ImsRatTypeUint8       vsSrvRat;      /* VS��IMS������ */
} IMSA_AT_ImsSrvStatusInfo;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    VOS_UINT32               msgId;    /* _H2ASN_Skip */
    VOS_UINT16               clientId; /* �����ϱ�ʱ��0X3FFF */
    VOS_UINT8                opId;     /* ��0 */
    VOS_UINT8                reserved[1];
    IMSA_AT_ImsSrvStatusInfo imsSrvStatInfo;
} IMSA_AT_ImsSrvStatusUpdateInd;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
} IMSA_AT_DmcnInd;


typedef struct {
    VOS_UINT32 opAmrWbOctetAcigned : 1;
    VOS_UINT32 opAmrWbBandWidthEfficient : 1;
    VOS_UINT32 opAmrOctetAcigned : 1;
    VOS_UINT32 opAmrBandWidthEfficient : 1;
    VOS_UINT32 opAmrWbMode : 1;
    VOS_UINT32 opDtmfWb : 1;
    VOS_UINT32 opDtmfNb : 1;
    VOS_UINT32 opSpeechStart : 1;
    VOS_UINT32 opSpeechEnd : 1;
    VOS_UINT32 opVideoStart : 1;
    VOS_UINT32 opVideoEnd : 1;
    VOS_UINT32 opRetryBaseTime : 1;
    VOS_UINT32 opRetryMaxTime : 1;
    VOS_UINT32 opPhoneContext : 1;
    VOS_UINT32 opPhoneContextImpu : 1;
    VOS_UINT32 opSpare : 17;

    VOS_UINT32 amrWbOctetAcigned;
    VOS_UINT32 amrWbBandWidthEfficient; /* �ӿ�Ԥ����IMSA������ */
    VOS_UINT32 amrOctetAcigned;
    VOS_UINT32 amrBandWidthEfficient; /* �ӿ�Ԥ����IMSA������ */
    VOS_UINT32 amrWbMode;
    VOS_UINT32 dtmfWb; /* �ӿ�Ԥ����IMSA������ */
    VOS_UINT32 dtmfNb; /* �ӿ�Ԥ����IMSA������ */
    VOS_UINT32 speechStart;
    VOS_UINT32 speechEnd;
    VOS_UINT32 videoStart;
    VOS_UINT32 videoEnd;
    VOS_UINT32 retryBaseTime;
    VOS_UINT32 retryMaxTime;
    VOS_CHAR   phoneContext[AT_IMSA_PHONECONTEXT_MAX_LENGTH + 1];
    VOS_UINT8  reserved1[3];
    VOS_CHAR   phoneContextImpu[AT_IMSA_PUBLICEUSERID_MAX_LENGTH + 1];
    VOS_UINT8  reserved2[3];

} AT_IMSA_Dmdyn;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    AT_IMSA_Dmdyn  dmdyn;
} AT_IMSA_DmdynSetReq;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_DmdynSetCnf;


typedef AT_IMSA_Msg AT_IMSA_DmdynQryReq;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result;
    AT_IMSA_Dmdyn  dmdyn;
} IMSA_AT_DmdynQryCnf;

/*
 * �ṹ˵��: IPv4 P-CSCF��ַ�ṹ��
 */
typedef struct {
    VOS_UINT32 opPrimPcscfAddr : 1;
    VOS_UINT32 opSecPcscfAddr : 1;
    VOS_UINT32 opThiPcscfAddr : 1;
    VOS_UINT32 opPrimPcscfSipPort : 1;
    VOS_UINT32 opSecPcscfSipPort : 1;
    VOS_UINT32 opThiPcscfSipPort : 1;
    VOS_UINT32 opSpare : 26;

    VOS_UINT8  primPcscfAddr[TAF_IPV4_ADDR_LEN];
    VOS_UINT8  secPcscfAddr[TAF_IPV4_ADDR_LEN];
    VOS_UINT8  thiPcscfAddr[TAF_IPV4_ADDR_LEN];
    VOS_UINT32 primPcscfSipPort;
    VOS_UINT32 secPcscfSipPort;
    VOS_UINT32 thiPcscfSipPort;
} IMSA_AT_Ipv4Pcscf;

/*
 * �ṹ˵��: IPv6 P-CSCF��ַ�ṹ��
 */
typedef struct {
    VOS_UINT32 opPrimPcscfAddr : 1;
    VOS_UINT32 opSecPcscfAddr : 1;
    VOS_UINT32 opThiPcscfAddr : 1;
    VOS_UINT32 opPrimPcscfSipPort : 1;
    VOS_UINT32 opSecPcscfSipPort : 1;
    VOS_UINT32 opThiPcscfSipPort : 1;
    VOS_UINT32 opSpare : 26;

    VOS_UINT8  primPcscfAddr[TAF_IPV6_ADDR_LEN];
    VOS_UINT8  secPcscfAddr[TAF_IPV6_ADDR_LEN];
    VOS_UINT8  thiPcscfAddr[TAF_IPV6_ADDR_LEN];
    VOS_UINT32 primPcscfSipPort;
    VOS_UINT32 secPcscfSipPort;
    VOS_UINT32 thiPcscfSipPort;
} IMSA_AT_Ipv6Pcscf;

typedef struct {
    IMSA_AT_PcscfSrcUint32 src; /* ����P-CSCF��ַ��Դ�� */
    IMSA_AT_Ipv6Pcscf      ipv6Pcscf;
    IMSA_AT_Ipv4Pcscf      ipv4Pcscf;
} IMSA_AT_Pcscf;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    IMSA_AT_Pcscf  atPcscf;
} AT_IMSA_PcscfSetReq;

typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_PcscfSetCnf;

typedef AT_IMSA_Msg AT_IMSA_PcscfQryReq;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result;
    IMSA_AT_Pcscf  atPcscf;
} IMSA_AT_PcscfQryCnf;


typedef struct {
    VOS_UINT32 opTimer1Value : 1;
    VOS_UINT32 opTimer2Value : 1;
    VOS_UINT32 opTimer4Value : 1;
    VOS_UINT32 opTimerAValue : 1;
    VOS_UINT32 opTimerBValue : 1;
    VOS_UINT32 opTimerCValue : 1;
    VOS_UINT32 opTimerDValue : 1;
    VOS_UINT32 opTimerEValue : 1;
    VOS_UINT32 opTimerFValue : 1;
    VOS_UINT32 opTimerGValue : 1;
    VOS_UINT32 opTimerHValue : 1;
    VOS_UINT32 opTimerIValue : 1;
    VOS_UINT32 opTimerJValue : 1;
    VOS_UINT32 opTimerKValue : 1;
    VOS_UINT32 opSpare : 18;

    VOS_UINT32 timer1Value;
    VOS_UINT32 timer2Value;
    VOS_UINT32 timer4Value;
    VOS_UINT32 timerAValue;
    VOS_UINT32 timerBValue;
    VOS_UINT32 timerCValue;
    VOS_UINT32 timerDValue;
    VOS_UINT32 timerEValue;
    VOS_UINT32 timerFValue;
    VOS_UINT32 timerGValue;
    VOS_UINT32 timerHValue;
    VOS_UINT32 timerIValue;
    VOS_UINT32 timerJValue;
    VOS_UINT32 timerKValue;

} IMSA_AT_ImsTimer;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32       msgId;
    AT_APPCTRL       appCtrl;
    IMSA_AT_ImsTimer imsTimer;
} AT_IMSA_ImstimerSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result; /* �ɹ����� VOS_OK,ʧ�ܷ��� VOS_ERR */
} IMSA_AT_ImstimerSetCnf;


typedef AT_IMSA_Msg AT_IMSA_ImstimerQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32       msgId;
    AT_APPCTRL       appCtrl;
    VOS_UINT32       result;
    IMSA_AT_ImsTimer imsTimer;
} IMSA_AT_ImstimerQryCnf;


typedef struct {
    VOS_CHAR  smsPsi[AT_IMSA_MAX_SMSPSI_LEN + 1];
    VOS_UINT8 reserved[3];
} IMSA_SMS_Psi;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgId;
    AT_APPCTRL   appCtrl;
    IMSA_SMS_Psi smsPsi;
} AT_IMSA_SmspsiSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result;
} IMSA_AT_SmspsiSetCnf;


typedef AT_IMSA_Msg AT_IMSA_SmspsiQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32   msgId;
    AT_APPCTRL   appCtrl;
    VOS_UINT32   result;
    IMSA_SMS_Psi smsPsi;

} IMSA_AT_SmspsiQryCnf;


typedef AT_IMSA_Msg AT_IMSA_DmuserQryReq;


typedef struct {
    VOS_CHAR                  impi[AT_IMSA_IMPI_MAX_LENGTH + 1];
    VOS_CHAR                  impu[AT_IMSA_IMPU_MAX_LENGTH + 1];
    VOS_CHAR                  homeNetWorkDomain[IMSA_IMS_STRING_LENGTH + 1];
    VOS_UINT8                 reserved[1];
    AT_IMSA_VoiceDomainUint32 voiceDomain;
    VOS_UINT32                ipsecEnable;
} IMSA_AT_Dmuser;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32     msgId;
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result;
    IMSA_AT_Dmuser dmUser;
} IMSA_AT_DmuserQryCnf;


typedef struct {
    VOS_UINT8 nickNameLen;
    VOS_CHAR  nickName[MN_CALL_DISPLAY_NAME_STRING_SZ];
    VOS_UINT8 reserved[2];
} IMSA_AT_NicknameInfo;


typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    VOS_UINT32           msgId; /* _H2ASN_Skip */
    AT_APPCTRL           appCtrl;
    IMSA_AT_NicknameInfo nickName;
} AT_IMSA_NicknameSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_NicknameSetCnf;


typedef AT_IMSA_Msg AT_IMSA_NicknameQryReq;


typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32           msgId;  /* _H2ASN_Skip */
    AT_APPCTRL           appCtrl;
    VOS_UINT32           result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    IMSA_AT_NicknameInfo nickName;
} IMSA_AT_NicknameQryCnf;


typedef struct {
    AT_IMSA_BatteryStatusUint8 batteryStatus;
    VOS_UINT8                  reserved[3];
} AT_IMSA_BatteryStatusInfo;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT32                msgId; /* _H2ASN_Skip */
    AT_APPCTRL                appCtrl;
    AT_IMSA_BatteryStatusInfo batteryStatusInfo;
} AT_IMSA_BatteryInfoSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_BatteryInfoSetCnf;


typedef struct {
    IMSA_AT_RegFailCodeUint8 failCode;
    VOS_UINT8                reserved[3];
} IMSA_AT_RegFailInfo;


typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    VOS_UINT32          msgId; /* _H2ASN_Skip */
    AT_APPCTRL          appCtrl;
    IMSA_AT_RegFailInfo regFailInfo;
} IMSA_AT_RegFailInd;


typedef AT_IMSA_Msg AT_IMSA_VolteregNtf;


typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    VOS_UINT32     msgId;      /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT8      reportFlag; /* VOS_TRUE:���������ϱ���VOS_FALSE:�������ϱ� */
    VOS_UINT8      reserv[3];
} AT_IMSA_RegerrReportSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_RegerrReportSetCnf;


typedef AT_IMSA_Msg AT_IMSA_RegerrReportQryReq;


typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32     msgId;  /* _H2ASN_Skip */

    AT_APPCTRL     appCtrl;
    VOS_UINT32     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */

    VOS_UINT8      reportFlag;
    VOS_UINT8      reserved[7];
} IMSA_AT_RegerrReportQryCnf;


typedef struct {
    VOS_MSG_HEADER                       /* _H2ASN_Skip */
    VOS_UINT32                    msgId; /* _H2ASN_Skip */
    AT_APPCTRL                    appCtrl;
    IMSA_AT_ImsRegDomainTypeUint8 imsaRegDomain;
    IMSA_AT_RegErrTypeUint8       imsaRegErrType;
    VOS_UINT8                     rsv[2];
    IMSA_AT_PdnFailCauseUint32    imsaPdnFailCause;
    IMSA_AT_RegFailCauseUint32    imsaRegFailCause;
    /* ���ַ���ʱȫ0 */
    VOS_CHAR                      imsRegFailReasonCtx[IMSA_AT_REG_FAIL_CAUSE_STR_MAX_LEN];
} IMSA_AT_RegerrReportInd;


typedef struct {
    VOS_UINT32 opIpsecFlag : 1;
    VOS_UINT32 opKeepAliveFlag : 1;
    VOS_UINT32 opRev : 30;
    /* VOS_TRUE:֧��IPSEC��VOS_FALSE:��֧��IPSEC */
    VOS_UINT32 ipsecFlag;
    /* VOS_TRUE:֧��KEEP ALIVE��VOS_FALSE:��֧��KEEP ALIVE */
    VOS_UINT32 keepAliveFlag;
} AT_IMSA_ImsIpCapSetInfo;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32              msgId; /* _H2ASN_Skip */
    AT_APPCTRL              appCtrl;
    AT_IMSA_ImsIpCapSetInfo ipCapInfo;
} AT_IMSA_ImsIpCapSetReq;

typedef IMSA_AT_CnfMsg IMSA_AT_ImsIpCapSetCnf;


typedef AT_IMSA_Msg AT_IMSA_ImsIpCapQryReq;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result;
    VOS_UINT32     ipsecFlag;
    VOS_UINT32     keepAliveFlag;
} IMSA_AT_ImsIpCapQryCnf;


typedef struct {
    VOS_MSG_HEADER                        /* _H2ASN_Skip */
    VOS_UINT32                     msgId; /* _H2ASN_Skip */
    AT_APPCTRL                     appCtrl;
    AT_IMSA_ImsSrvStatReportUint32 imsSrvStatRpt;
} AT_IMSA_ImsSrvStatReportSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_ImsSrvStatReportSetCnf;


typedef AT_IMSA_Msg AT_IMSA_ImsSrvStatReportQryReq;


typedef struct {
    VOS_MSG_HEADER                         /* _H2ASN_Skip */
    VOS_UINT32                     msgId;  /* _H2ASN_Skip */
    AT_APPCTRL                     appCtrl;
    VOS_UINT32                     result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    AT_IMSA_ImsSrvStatReportUint32 imsSrvStatRpt;
} IMSA_AT_ImsSrvStatReportQryCnf;


typedef AT_IMSA_Msg AT_IMSA_ImsServiceStatusQryReq;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    VOS_UINT32               msgId;  /* _H2ASN_Skip */
    AT_APPCTRL               appCtrl;
    VOS_UINT32               result; /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    IMSA_AT_ImsSrvStatusInfo imsSrvStatInfo;
} IMSA_AT_ImsServiceStatusQryCnf;


typedef struct {
    VOS_UINT32 addressIdLen; /* Address Id ��Ч���ȣ�����Ϊ0 */
    VOS_CHAR   addressId[AT_IMSA_MAX_EMERGENCY_AID_LEN];
} AT_IMSA_EmergencyAidInfo;


typedef struct {
    VOS_MSG_HEADER                  /* _H2ASN_Skip */
    VOS_UINT32               msgId; /* _H2ASN_Skip */
    AT_APPCTRL               appCtrl;
    AT_IMSA_EmergencyAidInfo emcAIdInfo;
} AT_IMSA_EmergencyAidSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_EmergencyAidSetCnf;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    AT_APPCTRL     appCtrl;
} IMSA_AT_CallAltSrvInd;

typedef struct {
    VOS_UINT32 featureTag;
    VOS_UINT32 devCfg;
} AT_IMSA_DmRcsCfgSetPara;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32              msgId; /* _H2ASN_Skip */
    AT_APPCTRL              appCtrl;

    AT_IMSA_DmRcsCfgSetPara dmRcsCfgPara;
} AT_IMSA_DmRcsCfgSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_DmRcsCfgSetCnf;


typedef struct {
    VOS_UINT8 para1[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 para2[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 para3[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 para4[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 para5[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 para6[AT_IMSA_USER_AGENT_STR_LEN + 1];
    VOS_UINT8 reserved[2];
} AT_IMSA_UserAgentCfg;


typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    VOS_UINT32           msgId; /* _H2ASN_Skip */
    AT_APPCTRL           appCtrl;

    AT_IMSA_UserAgentCfg uaCfgInfo;
} AT_IMSA_UserAgentCfgSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_UserAgentCfgSetCnf;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    VOS_UINT32             msgId; /* _H2ASN_Skip */
    AT_APPCTRL             appCtrl;

    AT_IMSA_ImpuTypeUint32 impuType;
} IMSA_AT_ImpuTypeInd;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserved1;
    VOS_UINT8  viceCfg; /* 0-1��VICE���ܿ��أ�0:���ܹر�  */
    VOS_UINT8  reserved2[3];
} AT_IMSA_VicecfgSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_VicecfgSetCnf;


typedef AT_IMSA_Msg AT_IMSA_VicecfgQryReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result;  /* �ɹ�����VOS_OK, ʧ�ܷ��� VOS_ERR  */
    VOS_UINT8  viceCfg; /* VICE���Կ��أ�0:���ܹر� */
    VOS_UINT8  reserved[3];
} IMSA_AT_VicecfgQryCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserved;
    VOS_UINT32 txtLength;
    VOS_UINT8  notify[IMSA_AT_DIALOG_NOTIFY_MAX_LEN];
} IMSA_AT_DialogNotify;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserved1;
    /* RTT���ܿ��أ�0:���ܹر� 1:���ܿ���  */
    VOS_UINT8 rttCfg;
    VOS_UINT8 reserved2[3];
} AT_IMSA_RttcfgSetReq;


typedef IMSA_AT_CnfMsg IMSA_AT_RttcfgSetCnf;


typedef struct {
    VOS_UINT8                 callId;    /* 1-7������ID */
    AT_IMSA_RttModifyOptUint8 operation; /* RTT����Modify�������� */
    VOS_UINT8                 reserved[2];
} AT_IMSA_RttModify;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32        msgId;
    AT_APPCTRL        appCtrl;
    AT_IMSA_RttModify rttModify;
} AT_IMSA_RttModifySetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 tcpThreshold;
} AT_IMSA_TransportTypeSetReq;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32  msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT8 context[AT_IMSA_CONTENT_TYPE_MAX_LEN + 1];
    VOS_UINT8 mode;
    VOS_UINT8 reserved[2];
} AT_IMSA_EcallContentTypeSetReq;


typedef AT_IMSA_Msg AT_IMSA_EcallContentTypeQryReq;

typedef AT_IMSA_Msg AT_IMSA_TransportTypeQryReq;


typedef IMSA_AT_CnfMsg IMSA_AT_RttModifySetCnf;


typedef IMSA_AT_CnfMsg IMSA_AT_TransportTypeSetCnf;

typedef IMSA_AT_CnfMsg IMSA_AT_EcallContentTypeSetCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32  msgId;
    AT_APPCTRL  appCtrl;
    VOS_UINT8   contentTypeMode;
    VOS_UINT8   contentTypeContext[AT_IMSA_CONTENT_TYPE_MAX_LEN + 1];
    VOS_UINT8   rsv[2];
    VOS_UINT32  result;/* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
} IMSA_AT_EcallContentTypeQryCnf;



typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result; /* �ɹ�����VOS_OK, ʧ�ܷ��� VOS_ERR  */
    VOS_UINT32 tcpThreshold;
} IMSA_AT_TransportTypeQryCnf;


typedef struct {
    VOS_UINT8                 callId;    /* ����IDָʾ��1-7  */
    IMSA_AT_RttEventUint8     event;     /* RTT�¼����ͣ�0-3 */
    IMSA_AT_RttEvtReasonUint8 evtReason; /* RTT�¼�����ԭ��,0-1 */
    VOS_UINT8                 reserved;
} IMSA_AT_RttEventPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32           msgId;
    AT_APPCTRL           appCtrl;
    IMSA_AT_RttEventPara rttEvent;
} IMSA_AT_RttEventInd;


typedef struct {
    VOS_UINT8                 callId;    /* ����IDָʾ��1-7  */
    IMSA_AT_RttOperationUint8 operation; /* RTT�쳣�������ͣ�0-1 */
    VOS_INT16                 causeCode; /* RTT�쳣ԭ��ȱʡ-1��1-127:Q850��400-699:Sip��Ӧ��  */
    /* �ַ������ͣ�RTT�쳣ԭ��ȱʡΪ"unknown" */
    VOS_CHAR reasonText[IMSA_AT_RTT_REASON_TEXT_MAX_LEN];
} IMSA_AT_RttError;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32       msgId;
    AT_APPCTRL       appCtrl;
    IMSA_AT_RttError rttError;
} IMSA_AT_RttErrorInd;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32  msgId;
    AT_APPCTRL  appCtrl;
    VOS_UINT16  version;
    VOS_UINT16  length;
    VOS_UINT8   data[0];
} AT_IMSA_ImsUrspSetReq;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32     msgId;
    AT_APPCTRL     appCtrl;
    VOS_UINT32     result;  /* �ɹ�����VOS_OK��ʧ�ܷ���VOS_ERR */
    VOS_UINT16     version;
    VOS_UINT8      reserved[2];
} IMSA_AT_ImsUrspSetCnf;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32     msgId;
    AT_APPCTRL     appCtrl;
    AT_IMSA_FusionCallDstIdUint8 dstId; /* 0�����͸�IMSA����Ϣ��1�����͸�IMS����Ϣ */
    VOS_UINT8 reserved[3];
    VOS_UINT32 msgLen;
    VOS_UINT8 msgData[4];
} AT_IMSA_FusionCallCtrlMsg;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32     msgId;
    AT_APPCTRL     appCtrl;
    IMSA_AT_FusionCallSrcIdUint8 srcId; /* 0�����͸�IMSA����Ϣ��1�����͸�IMS����Ϣ */
    VOS_UINT8 reserved[3];
    VOS_UINT32 msgLen;
    VOS_UINT8 msgData[4];
} IMSA_AT_FusionCallCtrlMsg;
/*
 �ṹ����  : IMSA_AT_EmcPdnActivateCnf
 �ṹ˵��  : ID_IMSA_AT_EMC_PDN_ACTIVATE_CNF ��Ϣ�ṹ
*/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result;
    TAF_PDP_Addr pdpAddr;
    TAF_PDP_Dns ipv4Dns;
    TAF_PDP_Ipv6Dns ipv6Dns;
    VOS_UINT16 mtu;
    VOS_UINT16 reserved;
} IMSA_AT_EmcPdnActivateCnf;

/*
 �ṹ����  : IMSA_AT_EMC_PDN_DEACTIVATE_CNF_STRU
 �ṹ˵��  : ID_IMSA_AT_EMC_PDN_DEACTIVATE_CNF ��Ϣ�ṹ
*/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    AT_APPCTRL appCtrl;
    VOS_UINT32 result;
} IMSA_AT_EmcPdnDeactivateCnf;

/* ASN�����ṹ */
typedef struct {
    /* _H2ASN_MsgChoice_Export AT_IMSA_MSG_TYPE_ENUM_UINT32 */
    VOS_UINT32 msgId;
    VOS_UINT8  msg[4];
    /*
     * _H2ASN_MsgChoice_When_Comment          AT_IMSA_MsgTypeUint32
     */
} AT_IMSA_InterfaceMsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    AT_IMSA_InterfaceMsgData msgData;
} AT_IMSA_InterfaceMsg;

typedef AT_IMSA_Msg AT_IMSA_EmcPdnActivateReq;
typedef AT_IMSA_Msg AT_IMSA_EmcPdnDeactivateReq;


#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
