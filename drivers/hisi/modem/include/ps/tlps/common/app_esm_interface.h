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
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, 
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

#ifndef __APP_ESM_INTERFACE_H__
#define __APP_ESM_INTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "app_nas_comm.h"
#include "app_rrc_interface.h"
#include "nas_comm_packet_ser.h"

#pragma pack(push, 4)

/* IP地址长度宏定义 */
#define APP_MAX_IPV4_ADDR_LEN 4
#define APP_MAX_IPV6_ADDR_LEN 16
#define APP_ESM_PCSCF_ADDR_MAX_NUM 8

/* Added 2016-06-20,Begin */
#define APP_MAX_IPV6_PREFIX_LEN 8
/* Added 2016-06-20,End */

#define APP_ESM_MAX_IPV6_PREFIX_NUM 6
#define APP_ESM_IPV6_ADDR_LEN 16

#define APP_ESM_MAX_LENGTH_OF_APN 100

#define APP_MAX_APN_LEN 99
#define APP_ESM_MAX_EPSB_NUM 11 /* 最大承载数 */
#define APP_ESM_MAX_ACCESS_NUM_LEN 32

/* 产品线at手册规定AT^AUTHDATA用户名和密码长度最大为99 */
#define APP_ESM_MAX_USER_NAME_LEN 99
#define APP_ESM_MAX_PASSWORD_LEN 99

#define APP_ESM_MAX_SDF_PF_NUM 16

/* APP->SM Command消息前3个字节宏定义 APP->SM */
#define APP_ESM_MSG_ID_HEADER PS_MSG_ID_APP_TO_ESM_BASE

/* SM->APP Command消息前3个字节宏定义 SM->APP */
#define ESM_APP_MSG_ID_HEADER PS_MSG_ID_ESM_TO_APP_BASE

#define APP_ESM_PPP_CHAP_CHALLNGE_LEN 16
#define APP_ESM_PPP_CHAP_CHALLNGE_NAME_LEN 16
#define APP_ESM_PPP_CHAP_RESPONSE_LEN 16

#define APP_ESM_PPP_MAX_USERNAME_LEN 99
#define APP_ESM_PPP_MAX_PASSWORD_LEN 99

#define APP_ESM_TFT_MAX_PF_NUM 16

/* DT begin */
#define APP_ESM_DT_REPORT_STATUS_OPEN 1
#define APP_ESM_DT_REPORT_STATUS_CLOSE 0
/* DT end */

/* moded for pco requirement 2017-03-25, begin */
#define APP_ESM_MAX_CUSTOM_PCO_CONTAINER_NUM 3
#define APP_ESM_MAX_CUSTOM_PCO_CONTAINER_CONTENT_LEN 53
/* moded for pco requirement 2017-03-25, end */

#define APP_ESM_MAX_APN_INFO_NUM 8
#define APP_ESM_MAX_IPV4_EPDG_NUM 2
#define APP_ESM_MAX_IPV6_EPDG_NUM 2

#define APP_MAX_PROFILE_NUM         17
#define APP_MAX_LTE_ATTACH_PROFILE_NAME_LEN 32
#define APP_MAX_LTE_ATTACH_PROFILE_USERNAME_LEN 32
#define APP_MAX_LTE_ATTACH_PROFILE_USERPWD_LEN 32

/* 枚举说明: APP与SM消息取值 */
enum APP_ESM_MsgType {
    /* 参数设置原语 */
    ID_APP_ESM_SET_TFT_REQ = 0x01 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetTftReq */
    ID_APP_ESM_SET_TFT_CNF = 0x02 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetTftCnf */

    ID_APP_ESM_SET_QOS_REQ = 0x03 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetQosReq */
    ID_APP_ESM_SET_QOS_CNF = 0x04 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetQosCnf */

    ID_APP_ESM_SET_EPS_QOS_REQ = 0x05 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetEpsQosReq */
    ID_APP_ESM_SET_EPS_QOS_CNF = 0x06 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetEpsQosCnf */

    ID_APP_ESM_SET_APN_REQ = 0x07 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetApnReq */
    ID_APP_ESM_SET_APN_CNF = 0x08 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetApnCnf */

    ID_APP_ESM_SET_PCO_REQ = 0x09 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetPcoReq (deleted) */
    ID_APP_ESM_SET_PCO_CNF = 0x0A + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetPcoCnf */

    ID_APP_ESM_SET_PDN_TYPE_REQ = 0x0B + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetPdnTypeReq */
    ID_APP_ESM_SET_PDN_TYPE_CNF = 0x0C + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetPdnTypeCnf */

    ID_APP_ESM_SET_BEARER_TYPE_REQ = 0x0D + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetBearerTypeReq */
    ID_APP_ESM_SET_BEARER_TYPE_CNF = ESM_APP_MSG_ID_HEADER + 0x0E, /* _H2ASN_MsgChoice  APP_ESM_SetBearerTypeCnf */

    ID_APP_ESM_SET_PDP_MANAGER_TYPE_REQ = 0x0F + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetPdpManagerTypeReq */
    ID_APP_ESM_SET_PDP_MANAGER_TYPE_CNF = 0x10 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetPdpManagerTypeCnf */

    ID_APP_ESM_SET_GW_AUTH_REQ = 0x11 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetGwAuthReq */
    ID_APP_ESM_SET_GW_AUTH_CNF = 0x11 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetGwAuthCnf */
    /* modify begin */
    ID_APP_ESM_IPV6_INFO_NOTIFY = 0x12 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_Ipv6InfoNotify */

    ID_APP_ESM_PROCEDURE_ABORT_NOTIFY = 0x13 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_ProcedureAbortNotify */
    /* modify end */

    /* begin 2013-05-29 Modify L4A */
    ID_APP_ESM_NDISCONN_REQ = 0x14 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_NdisconnReq */
    ID_APP_ESM_NDISCONN_CNF = 0x12 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_NdisconnCnf */

    ID_APP_ESM_SET_CGDCONT_REQ = 0x15 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetCgdcontReq */
    ID_APP_ESM_SET_CGDCONT_CNF = 0x13 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_SetCgdcontCnf */
    /* end 2013-05-29 Modify L4A */
    ID_APP_ESM_SET_APN_THROT_INFO_REQ = 0x17 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetApnThrotInfoReq */
    ID_APP_ESM_SET_APN_THROT_INFO_CNF = 0x14 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetApnThrotInfoCnf */

    ID_APP_ESM_SET_ROAMING_PDN_TYPE_REQ = 0x18 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetRoamingPdnTypeReq */
    ID_APP_ESM_SET_ROAMING_PDN_TYPE_CNF = 0x15 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SetRoamingPdnTypeCnf */

    /* 参数查询原语 */
    ID_APP_ESM_INQ_TFT_REQ = 0x31 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqTftReq */
    ID_APP_ESM_INQ_TFT_CNF = 0x32 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqTftCnf */

    ID_APP_ESM_INQ_QOS_REQ = 0x33 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqQosReq */
    ID_APP_ESM_INQ_QOS_CNF = 0x34 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqQosCnf */

    ID_APP_ESM_INQ_EPS_QOS_REQ = 0x35 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqEpsQosReq */
    ID_APP_ESM_INQ_EPS_QOS_CNF = 0x36 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqEpsQosCnf */

    ID_APP_ESM_INQ_APN_REQ = 0x37 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqApnReq */
    ID_APP_ESM_INQ_APN_CNF = 0x38 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqApnCnf */

    ID_APP_ESM_INQ_PCO_REQ = 0x39 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPcoReq */
    ID_APP_ESM_INQ_PCO_CNF = 0x3A + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPcoCnf (deleted) */

    ID_APP_ESM_INQ_PDN_TYPE_REQ = 0x3B + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPdnTypeReq */
    ID_APP_ESM_INQ_PDN_TYPE_CNF = 0x3C + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPdnTypeCnf */

    ID_APP_ESM_INQ_BEARER_TYPE_REQ = 0x3D + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqBearerTypeReq */
    ID_APP_ESM_INQ_BEARER_TYPE_CNF = ESM_APP_MSG_ID_HEADER + 0x3E, /* _H2ASN_MsgChoice  APP_ESM_InqBearerTypeCnf */

    ID_APP_ESM_INQ_PDP_MANAGER_TYPE_REQ = 0x3F + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqPdpManagerTypeReq */
    ID_APP_ESM_INQ_PDP_MANAGER_TYPE_CNF = 0x40 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqPdpManagerTypeCnf */

    ID_APP_ESM_INQ_BEAER_QOS_REQ = 0x41 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqBeaerQosReq */
    ID_APP_ESM_INQ_BEAER_QOS_CNF = 0x42 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqBeaerQosCnf */
    ID_APP_ESM_INQ_BEAER_QOS_IND = 0x43 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqBeaerQosInd */

    ID_APP_ESM_INQ_DYNAMIC_PDP_CONT_REQ = 0x44 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqDynamicPdpContReq */
    ID_APP_ESM_INQ_DYNAMIC_PDP_CONT_CNF = 0x45 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqDynamicPdpContCnf */

    ID_APP_ESM_INQ_DYNAMIC_EPS_QOS_REQ = 0x46 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqDynamicEpsQosReq */
    ID_APP_ESM_INQ_DYNAMIC_EPS_QOS_CNF = 0x47 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_InqDynamicEpsQosCnf */

    ID_APP_ESM_INQ_DYNAMIC_TFT_REQ = 0x48 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqDynamicTftReq */
    ID_APP_ESM_INQ_DYNAMIC_TFT_CNF = 0x49 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqDynamicTftCnf */

    ID_APP_ESM_INQ_GW_AUTH_REQ = 0x4A + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqGwAuthReq */
    ID_APP_ESM_INQ_GW_AUTH_CNF = 0x4A + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqGwAuthCnf */

    /* DT PDP   2014/03/21  start */
    /* DT begin */
    ID_APP_ESM_DT_INQ_PDP_INFO_REQ = 0x4B + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPdpInfoReq */
    ID_APP_ESM_DT_INQ_PDP_INFO_CNF = 0x4B + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPdpInfoCnf */
    ID_APP_ESM_DT_INQ_PDP_INFO_IND = 0x4C + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_InqPdpInfoInd */
    /* DT end */
    /* DT PDP   2014/03/21  end */

    ID_ESM_DSM_NOTIFICATION_IND = 0x4D + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice ESM_DSM_NotificationInd */
    ID_ESM_DSM_REG_CID_IND      = 0x4E + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice ESM_DSM_RegCidInd */
    /* 承载建立、修改和释放消息原语结构 */
    ID_APP_ESM_PDP_SETUP_REQ = 0x61 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpSetupReq */
    ID_APP_ESM_PDP_SETUP_CNF = 0x62 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpSetupCnf */
    ID_APP_ESM_PDP_SETUP_IND = 0x63 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpSetupInd */

    ID_APP_ESM_PDP_MODIFY_REQ = 0x64 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpModifyReq */
    ID_APP_ESM_PDP_MODIFY_CNF = 0x65 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpModifyCnf */
    ID_APP_ESM_PDP_MODIFY_IND = 0x66 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpModifyInd */

    ID_APP_ESM_PDP_RELEASE_REQ = 0x67 + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpReleaseReq */
    ID_APP_ESM_PDP_RELEASE_CNF = 0x68 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpReleaseCnf */
    ID_APP_ESM_PDP_RELEASE_IND = 0x69 + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpReleaseInd */

    ID_APP_ESM_PDP_MANAGER_IND = 0x6A + ESM_APP_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpManagerInd */
    ID_APP_ESM_PDP_MANAGER_RSP = 0x6B + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice  APP_ESM_PdpManagerRsp */

    ID_APP_ESM_NR_HANDOVER_TO_LTE_IND = 0x6C + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_NrHandoverToLteInd */
    /* Added for HO ATTACH, 2018-12-15 begin */
    ID_APP_ESM_SYNC_BEARER_INFO_NTF = 0x6D + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APP_ESM_SyncBearerInfoNotify */
    /* Added for HO ATTACH, 2018-12-15 end */

    ID_APS_ESM_LOCAL_RELEASE_NTF = 0x6E + APP_ESM_MSG_ID_HEADER, /* _H2ASN_MsgChoice APS_ESM_LocalReleaseNotify */
    ID_APP_ESM_PROFILE_LIST_NTF = 0x6F + APP_ESM_MSG_ID_HEADER,
    ID_APP_ESM_PROFILE_REG_STATUS_NTF = 0x70 + APP_ESM_MSG_ID_HEADER,

    ID_APP_ESM_MSG_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_MsgTypeUint32;

/* 枚举说明: 参数设置类型取值 */
enum APP_ESM_ParaSet {
    APP_ESM_PARA_SETTING = 0x00,
    APP_ESM_PARA_DELETE  = 0x01,
    APP_ESM_PARA_BUTT
};
typedef VOS_UINT32 APP_ESM_ParaSetUint32;

/* 枚举说明: 参数设置结果取值 */
enum APP_ESM_ParaSetResult {
    APP_ESM_PARA_SET_SUCCESS = 0x00,
    APP_ESM_PARA_SET_FAIL    = 0x01,
    APP_ESM_PARA_SET_BUTT
};
typedef VOS_UINT32 APP_ESM_ParaSetResultUint32;

/* 枚举说明: 承载状态取值 */
enum APP_ESM_BearerState {
    APP_ESM_BEARER_STATE_INACTIVE  = 0x00,
    APP_ESM_BEARER_STATE_ACTIVATED = 0x01,
    APP_ESM_BEARER_STATE_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerStateUint32;

/*
 * 枚举说明  : SM原因取值
 */
typedef VOS_UINT32 APP_ESM_CauseUint32;

/* 枚举说明: 承载类型取值 */
enum APP_ESM_BearerType {
    APP_ESM_BEARER_TYPE_DEFAULT   = 0x00,
    APP_ESM_BEARER_TYPE_DEDICATED = 0x01,
    APP_ESM_BEARER_TYPE_EMERGENCY = 0x02,
    APP_ESM_BEARER_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerTypeUint32;

/* 枚举说明: 承载修改内容取值 */
enum APP_ESM_BearerModify {
    APP_ESM_BEARER_MODIFY_TFT     = 0x00,
    APP_ESM_BEARER_MODIFY_QOS     = 0x01,
    APP_ESM_BEARER_MODIFY_TFT_QOS = 0x02,
    APP_ESM_BEARER_MODIFY_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerModifyUint32;

/* 枚举说明: 承载激活类型取值 */
enum APP_ESM_BearerActType {
    APP_ESM_BEARER_ACT_TYPE_ACTIVE = 0x00,
    APP_ESM_BEARER_ACT_TYPE_MODIFY = 0x01,
    APP_ESM_BEARER_ACT_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerActTypeUint32;

/* 枚举说明: 承载激活结果取值 */
enum APP_ESM_BearerActResult {
    APP_ESM_BEARER_ACT_ACCEPT = 0x00,
    APP_ESM_BEARER_ACT_REJ    = 0x01,
    APP_ESM_BEARER_ACT_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerActResultUint32;

/* 枚举说明: 承载答复模式取值 */
enum APP_ESM_AnswerMode {
    APP_ESM_ANSWER_MODE_AUTO = 0x00,
    APP_ESM_ANSWER_MODE_MANI = 0x01,
    APP_ESM_ANSWER_MODE_BUTT
};
typedef VOS_UINT32 APP_ESM_AnswerModeUint32;

/* 枚举说明: 自动承载答复取值 */
enum APP_ESM_AnswerResult {
    APP_ESM_ANSWER_ACCEPT = 0x00,
    APP_ESM_ANSWER_REJ    = 0x01,
    APP_ESM_ANSWER_RESULT_BUTT
};
typedef VOS_UINT32 APP_ESM_AnswerResultUint32;

/* 枚举说明: PDN类型取值 */
enum APP_ESM_PdnType {
    APP_ESM_PDN_TYPE_IPV4      = 0x01, /* 遵从协议24301 9.9.4.10规定 */
    APP_ESM_PDN_TYPE_IPV6      = 0x02,
    APP_ESM_PDN_TYPE_IPV4_IPV6 = 0x03,
    APP_ESM_PDN_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_PdnTypeUint32;

/* Modify for CL multimode 2014-01-09 begin */
/* 枚举说明: PDN请求类型取值 24008 10.5.6.17 */
enum APP_ESM_PdnRequestType {
    APP_ESM_PDN_REQUEST_TYPE_INITIAL  = 0x01, /* 建立一个新的PDN连接 */
    APP_ESM_PDN_REQUEST_TYPE_HANDOVER = 0x02, /* 保持non-3GPP下建好的PDN连接 */
    APP_ESM_PDN_REQUEST_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_PdnRequestTypeUint32;
/* Modify for CL multimode 2014-01-09 end */

/* 枚举说明: CID类型取值 */
enum APP_ESM_CidType {
    APP_ESM_CID_TYPE_DEFAULT   = 0x00,
    APP_ESM_CID_TYPE_DEDICATED = 0x01,
    APP_ESM_CID_TYPE_EMERGENCY = 0x02,
    APP_ESM_CID_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_CidTypeUint32;

enum APP_ESM_TransferDirection {
    APP_ESM_TRANSFER_DIRECTION_PRE_REL7_TFT_FILTER = 0x00,
    APP_ESM_TRANSFER_DIRECTION_DOWNLINK            = 0x01,
    APP_ESM_TRANSFER_DIRECTION_UPLINK              = 0x02,
    APP_ESM_TRANSFER_DIRECTION_UPLINK_DOWNLINK     = 0x03,
    APP_ESM_TRANSFER_DIRECTION_BUTT
};
typedef VOS_UINT8 APP_ESM_TransferDirectionUint8;

enum APP_ESM_BearerOperateType {
    APP_ESM_BEARER_OPERATE_TYPE_DEF_ACT  = 0x00,
    APP_ESM_BEARER_OPERATE_TYPE_DEDI_ACT = 0x01,
    APP_ESM_BEARER_OPERATE_TYPE_MODIFY   = 0x02,
    APP_ESM_BEARER_OPERATE_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerOperateTypeUint32;

enum APP_ESM_Ipv4AddrAllocType {
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_NAS_SIGNALING = 0x00,
    APP_ESM_IPV4_ADDR_ALLOC_TYPE_DHCP          = 0x01,

    APP_ESM_IPV4_ADDR_ALLOC_TYPE_BUTT
};
typedef VOS_UINT32 APP_ESM_Ipv4AddrAllocTypeUint32;

enum APP_ESM_PdpSetupType {
    APP_ESM_PDP_SETUP_TYPE_NORMAL = 0x00,
    APP_ESM_PDP_SETUP_TYPE_PPP    = 0x01,

    APP_ESM_PDP_SETUP_TYPE_BUTT = 0xFF
};
typedef VOS_UINT32 APP_ESM_PdpSetupTypeUint32;

/* 用于EAB特性，记录APP下发的承载优先级 */
enum APP_ESM_BearerPrio {
    APP_ESM_BEARER_PRIO_NORMAL = 0x00,
    APP_ESM_BEARER_PRIO_LOW    = 0x01,

    APP_ESM_BEARER_PRIO_BUTT
};
typedef VOS_UINT32 APP_ESM_BearerPrioUint32;

/*
 * 枚举说明: Authentication Type
 */
enum APP_ESM_AuthType {
    APP_ESM_AUTH_TYPE_NONE = 0x00,
    APP_ESM_AUTH_TYPE_PAP  = 0x01,
    APP_ESM_AUTH_TYPE_CHAP = 0x02,

    APP_ESM_AUTH_TYPE_BUTT = 0xFF
};
typedef VOS_UINT8 APP_ESM_AuthTypeUint8;

enum APP_ESM_BearerManageType {
    APP_ESM_BEARER_MANAGE_TYPE_ACCEPT = 0, /* 接受 */
    APP_ESM_BEARER_MANAGE_TYPE_REJ,        /* 拒绝 */

    APP_ESM_BEARER_MANANGE_TYPE_BUTT
};
typedef VOS_UINT8 APP_ESM_BearerManageTypeUint8;

enum APP_ESM_BearerManageMode {
    APP_ESM_BEARER_MANAGE_MODE_AUTO = 0, /* 自动 */
    APP_ESM_BEARER_MANAGE_MODE_MANUAL,   /* 手动 */

    APP_ESM_BEARER_MANAGE_MODE_BUTT
};
typedef VOS_UINT8 APP_ESM_BearerManageModeUint8;

/* DT begin */
/* 结构说明: 给路测上报的承载类型 */
enum NAS_ESM_DtBearerType {
    NAS_ESM_DT_BEARER_TYPE_DEFAULT = 0, /* 缺省承载 */
    NAS_ESM_DT_BEARER_TYPE_DEDICATED,   /* 专有承载 */
    /* DT ,2014/5/6, begin */
    NAS_ESM_DT_BEARER_TYPE_EMERGENCY, /* 紧急承载 */
    /* DT ,2014/5/6, end */

    NAS_ESM_DT_BEARER_TYPE_BUTT
}; /* 承载的属性 */
typedef VOS_UINT8 NAS_ESM_DtBearerTypeUint8;
/* DT end */

/* ims begin */
enum APP_ESM_PcscfDiscovery {
    APP_ESM_PCSCF_DISCOVERY_NOT_INFLUENCED  = 0x00,
    APP_ESM_PCSCF_DISCOVERY_THROUGH_NAS_SIG = 0x01,
    APP_ESM_PCSCF_DISCOVERY_THROUGH_DHCP    = 0x02,

    APP_ESM_PCSCF_DISCOVERY_BUTT
};
typedef VOS_UINT32 APP_ESM_PcscfDiscoveryUint32;

enum APP_ESM_ImsCnSigFlag {
    APP_ESM_PDP_NOT_FOR_IMS_CN_SIG_ONLY = 0x00,
    APP_ESM_PDP_FOR_IMS_CN_SIG_ONLY     = 0x01,

    APP_ESM_IMS_CN_SIG_FLAG_BUTT
};
typedef VOS_UINT32 APP_ESM_ImsCnSigFlagUint32;
/* ims end */

/* modify for 2014-05-09 begin */
enum APP_ESM_PdpEmcInd {
    APP_ESM_PDP_NOT_FOR_EMC = 0x00,
    APP_ESM_PDP_FOR_EMC     = 0x01,

    APP_ESM_PDP_EMC_IND_BUTT
};
typedef VOS_UINT32 APP_ESM_PdpEmcIndUint32;
/* modify for 2014-05-09 end */

/* 枚举说明: PDP释放原因 */
enum APP_ESM_PdpReleaseCause {
    APP_ESM_PDP_RELEASE_CAUSE_NORMAL               = 0,
    APP_ESM_PDP_RELEASE_CAUSE_IPV6_ADDR_ALLOC_FAIL = 1,
    APP_ESM_PDP_RELEASE_CAUSE_IPV6_ADDR_TIME_OUT = 2,
    APP_ESM_PDP_RELEASE_CAUSE_PCSCF_ADDR_ALLOC_FAIL = 3,
    /* del reattach type */


    APP_ESM_PDP_RELEASE_CAUSE_DATA_OFF = 4,

    /* Added for 2018-1-8, begin */
    APP_ESM_PDP_RELEASE_CAUSE_CELLULAR2W_HO = 5,
    /* Added for 2018-1-8, end */

    APP_ESM_PDP_RELEASE_CAUSE_BUTT
};
typedef VOS_UINT8 APP_ESM_PdpReleaseCauseUint8;

/* 枚举说明: 注册使用的APN 类型 */
enum APP_ESM_ApnType {
    APP_ESM_APN_TYPE_INVALID = 0x00,
    APP_ESM_APN_TYPE_CLASS1  = 0x01,
    APP_ESM_APN_TYPE_CLASS2  = 0x02,
    APP_ESM_APN_TYPE_CLASS3  = 0x03,
    APP_ESM_APN_TYPE_CLASS4  = 0x04,
    APP_ESM_APN_TYPE_CLASS5  = 0x05,
    APP_ESM_APN_TYPE_CLASS6  = 0x06,
    APP_ESM_APN_TYPE_CLASS7  = 0x07,
    APP_ESM_APN_TYPE_CLASS8  = 0x08,

    APP_ESM_APN_TYPE_BUTT
};
typedef VOS_UINT8 APP_ESM_ApnTypeUint8;

/* 枚举说明: SRVCC处理类型 */
enum ESM_DSM_NotificationIndType {
    ESM_DSM_NOTIFICATION_IND_SRVCC_HO_CANCELLED = 0,

    ESM_DSM_NOTIFICATION_IND_BUTT
};
typedef VOS_UINT32 ESM_DSM_NotificationIndTypeUint32;

/*
 * 结构说明: BEARER CONTROL MODE枚举
 */
enum APP_ESM_Bcm {
    APP_ESM_BCM_MS_ONLY = 0x01,
    APP_ESM_BCM_MS_NW   = 0x02,

    APP_ESM_BCM_BUTT
};
typedef VOS_UINT8 APP_ESM_BcmUint8;

/* 枚举说明: 切换结果取值 */
enum APP_ESM_HandoverResult {
    APP_ESM_HANDOVER_RESULT_SUCC = 0x00,
    APP_ESM_HANDOVER_RESULT_FAIL = 0x01,
    APP_ESM_HANDOVER_RESULT_BUTT
};
typedef VOS_UINT32 APP_ESM_HandoverResultUint32;

/* 枚举说明: 参数设置类型取值 */
enum APS_ESM_LocalRelCause {
    /* 去激活定时器超时 */
    APS_ESM_LOCAL_REL_CAUSE_DEACT_TIMEOUT,
    /*
     * LTE/NR/EHRPD模式下建立的数据业务，切换到1X/HRPD时需要本地去激活
     * LTE/NR模式下建立的数据业务，切换到GU(无TI值)时需要本地去激活
     * GU模式下建立的数据业务，丢网到NR时需要本地去激活
     */
    APS_ESM_LOCAL_REL_CAUSE_MODE_CHANGE,
    /* 本地去激活专有承载 */
    APS_ESM_LOCAL_REL_CAUSE_DEDICATED_BEARER,
    /* ESM通知APS的参数错误 */
    APS_ESM_LOCAL_REL_CAUSE_PARA_INVALID,
    /* APS在处理内部本地去激活IDN消息时，都使用此原因值 */
    APS_ESM_LOCAL_REL_CAUSE_APS_INTER_DEACT,
    /* APS收到EHSM去激活后，防止EHSM未通知ESM，由APS通知ESM本地去激活 */
    APS_ESM_LOCAL_REL_CAUSE_EHSM_DEACT,

    APS_ESM_LOCAL_REL_CAUSE_BUTT
};
typedef VOS_UINT32 APS_ESM_LocalRelCauseUint32;

/* 参数设置消息结构 */

/* Added for 5G Iteration 5, 2018-1-22, begin */
/*
 * 枚举名: ESM_DSM_NotificationInd
 * 枚举说明: SRVCC通知信息
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                        msgId;
    ESM_DSM_NotificationIndTypeUint32 notificationIndicator;
} ESM_DSM_NotificationInd;

/*
 * 结构说明: ESM -> DSM
 *           将注册时使用的SDF中的CID通知给DSM
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 regCid;
} ESM_DSM_RegCidInd;

/* 结构说明: 注册APN信息结构 */
typedef struct {
    VOS_UINT32           cid;         /* CID [0,11] */
    APP_ESM_ApnTypeUint8 apnType;     /* apn class */
    VOS_UINT8            disableFlag; /* apn disable flag */
    VOS_UINT8            reserved[2]; /* 保留字段，长度为2,便于4字节对齐 */
    VOS_UINT32           inactivityTimerValue; /* Inactivity Timer Value in second */
} APP_ESM_ApnClassInfo;

/* 结构说明: 参数设置结果回复的通用结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;    /* 本次操作标识 */
    VOS_UINT32 cid;     /* 上下文序列号:0~31   */
    VOS_UINT32 setRslt; /* 参数设置结果:0:成功,1:失败   */
} APP_ESM_ParaSetCnf;

/* begin for r11 2014-09-18 */
/* 结构说明: TFT  结构 */
typedef struct {
    VOS_UINT32 opRmtIpv4AddrAndMask : 1;
    VOS_UINT32 opRmtIpv6AddrAndMask : 1;
    VOS_UINT32 opProtocolId : 1;
    VOS_UINT32 opSingleLocalPort : 1;
    VOS_UINT32 opLocalPortRange : 1;
    VOS_UINT32 opSingleRemotePort : 1;
    VOS_UINT32 opRemotePortRange : 1;
    VOS_UINT32 opSecuParaIndex : 1;
    VOS_UINT32 opTypeOfService : 1;
    VOS_UINT32 opFlowLabelType : 1;
    /* begin for r11 2014-09-18 */
    VOS_UINT32 opLocalIpv4AddrAndMask : 1;
    VOS_UINT32 opLocalIpv6AddrAndMask : 1;
    /* end for r11 2014-09-18 */
    VOS_UINT32 opSpare : 20;

    VOS_UINT8                      packetFilterId;
    VOS_UINT8                      nwPacketFilterId;
    APP_ESM_TransferDirectionUint8 direction;
    VOS_UINT8                      precedence; /* packet filter evaluation precedence */

    VOS_UINT32 secuParaIndex; /* SPI */
    VOS_UINT16 singleLcPort;
    VOS_UINT16 lcPortHighLimit;
    VOS_UINT16 lcPortLowLimit;
    VOS_UINT16 singleRmtPort;
    VOS_UINT16 rmtPortHighLimit;
    VOS_UINT16 rmtPortLowLimit;
    VOS_UINT8  protocolId;    /* 协议号 */
    VOS_UINT8  typeOfService; /* TOS */

    VOS_UINT8 typeOfServiceMask; /* TOS Mask */
    VOS_UINT8 reserved[1]; /* 保留字段，长度为1,便于4字节对齐 */

    VOS_UINT8 rmtIpv4Address[APP_MAX_IPV4_ADDR_LEN];
    /*
     * ucSourceIpAddress[0]为IP地址高字节位
     * ucSourceIpAddress[3]为低字节位
     */
    VOS_UINT8 rmtIpv4Mask[APP_MAX_IPV4_ADDR_LEN];
    /*
     * ucSourceIpMask[0]为IP地址高字节位 ,
     * ucSourceIpMask[3]为低字节位
     */
    VOS_UINT8 rmtIpv6Address[APP_MAX_IPV6_ADDR_LEN];
    /*
     * ucRmtIpv6Address[0]为IPv6接口标识高字节位
     * ucRmtIpv6Address[7]为IPv6接口标识低字节位
     */
    VOS_UINT8 rmtIpv6Mask[APP_MAX_IPV6_ADDR_LEN];
    /*
     * ucRmtIpv6Mask[0]为高字节位
     * ucRmtIpv6Mask[7]为低字节位
     */

    VOS_UINT32 flowLabelType; /* FlowLabelType */
    /* begin for r11 2014-09-18 */
    VOS_UINT8 localIpv4Addr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 localIpv4Mask[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 localIpv6Addr[APP_MAX_IPV6_ADDR_LEN];
    VOS_UINT8 localIpv6Prefix;
    VOS_UINT8 reserved2[3]; /* 保留字段，长度为3,便于4字节对齐 */
    /* end for r11 2014-09-18 */
} APP_ESM_TftInfo;
/* end for r11 2014-09-18 */

/* 结构说明: TFT  参数设置 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32            msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32            opId;       /* 本次操作标识 */
    VOS_UINT32            cid;        /* 上下文序列号:0~31       */
    APP_ESM_ParaSetUint32 setType;    /* 设置类型:0:设置,1:删除 */
    APP_ESM_TftInfo       appTftInfo; /* TFT消息 */
} APP_ESM_SetTftReq;

typedef APP_ESM_ParaSetCnf APP_ESM_SetTftCnf;

/* 结构说明: QOS  结构 */
typedef struct {
    VOS_UINT8 delayClass;
    VOS_UINT8 reliabClass;
    VOS_UINT8 peakThrough;
    VOS_UINT8 precedenceClass;
    VOS_UINT8 meanThrough;
    VOS_UINT8 trafficClass;
    VOS_UINT8 deliverOrder;
    VOS_UINT8 deliverOfErrSdu;
    VOS_UINT8 maximSduSize;
    VOS_UINT8 maxBitRateForUp;
    VOS_UINT8 maxBitRateForDown;
    VOS_UINT8 residualBer;
    VOS_UINT8 sduErrRatio;
    VOS_UINT8 transDelay;
    VOS_UINT8 traffHandlPrior;
    VOS_UINT8 guarantBitRateForUp;
    VOS_UINT8 guarantBitRateForDown;
    VOS_UINT8 srcStatisticsDescriptor;
    VOS_UINT8 signallingIndication;
    VOS_UINT8 maxBitRateForDownExt;
    VOS_UINT8 guarantBitRateForDownExt;
    VOS_UINT8 maxBitRateForUpExt;
    VOS_UINT8 guarantBitRateForUpExt;
    /* begin for r11 2014-09-02 */
    VOS_UINT8 maxBitRateForDownExt2;
    VOS_UINT8 guarantBitRateForDownExt2;
    VOS_UINT8 maxBitRateForUpExt2;
    VOS_UINT8 guarantBitRateForUpExt2;
    /* end for r11 2014-09-02 */

    VOS_UINT8 reserved[1]; /* 保留字段，长度为1,便于4字节对齐 */
} APP_ESM_QosInfo;

/* 结构说明: QOS  参数设置 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32            msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32            opId;
    VOS_UINT32            cid;        /* 上下文序列号:0~31       */
    APP_ESM_ParaSetUint32 setType;    /* 设置类型:0:设置,1:删除 */
    APP_ESM_QosInfo       appQoSInfo; /* APP_ESM_QosInfo消息结构 */
} APP_ESM_SetQosReq;

typedef APP_ESM_ParaSetCnf APP_ESM_SetQosCnf;

/* 结构说明: EPS_QOS  参数 */
typedef struct {
    VOS_UINT8  qci;
    VOS_UINT8  rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT32 ulMaxRate;  /* 单位为kbps */
    VOS_UINT32 dlMaxRate;  /* 单位为kbps */
    VOS_UINT32 ulGMaxRate; /* 单位为kbps */
    VOS_UINT32 dlGMaxRate; /* 单位为kbps */
} APP_ESM_EpsQosInfo;

typedef struct {
    VOS_UINT32         epsbId;
    APP_ESM_EpsQosInfo qos;
} APP_EPS_Qos;

/* 结构说明: SDF QOS  参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    APP_ESM_ParaSetUint32 setType; /* 0:SET;1:DEL */
    APP_ESM_EpsQosInfo    sdfQosInfo;
} APP_ESM_SetEpsQosReq;

typedef APP_ESM_ParaSetCnf APP_ESM_SetEpsQosCnf;

/* 结构说明: APN  参数 */
typedef struct {
    VOS_UINT8 apnLen;
    VOS_UINT8 apnName[APP_MAX_APN_LEN];
} APP_ESM_ApnInfo;

/*
 * 结构说明: 详见TS 24.008 section 10.5.6.1 APN为码流字节(未译码)
 */
typedef struct {
    VOS_UINT8 length;      /* APN长度    */
    VOS_UINT8 reserved[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT8 value[APP_ESM_MAX_LENGTH_OF_APN];
} APP_ESM_Apn;

/* 结构说明: APN  参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    APP_ESM_ParaSetUint32 setType; /* 0:SET;1:DEL */
    APP_ESM_ApnInfo       apnInfo;
} APP_ESM_SetApnReq;

typedef APP_ESM_ParaSetCnf APP_ESM_SetApnCnf;

/* 结构说明: APN  参数设置 */
typedef struct {
    APP_ESM_ApnInfo apnInfo;

    VOS_UINT32 waitTime;        /* 在PDN去连接之后，在waittime之后才能发起PDN建立流程，单位:s */
    VOS_UINT32 pdnMaxConnTime;  /* 最大连接时间，标识每一个PDN第一次发起以来经过的最大时间，单位:s */
    VOS_UINT32 pdnMaxConnCount; /* 最大连接次数，在最大连接时间，允许发起PDN的最大连接次数 */

    VOS_UINT8            cid;
    APP_ESM_ApnTypeUint8 apnType;
    VOS_UINT8            rsv[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_ExcessivePdnCtrlPara;

/* 结构说明: APN  参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER

    APP_ESM_ExcessivePdnCtrlPara excessivePdnCtrlPara;
} APP_ESM_SetApnThrotInfoReq;

/* 结构说明: APN  参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER

    VOS_UINT32 rslt;
} APP_ESM_SetApnThrotInfoCnf;

/* 结构说明: APN AMBR 参数 */
typedef struct {
    VOS_UINT32 dlApnAmbr; /* 上行速率值,单位KBPS */
    VOS_UINT32 ulApnAmbr; /* 下行速率值,单位KBPS */
} APP_ESM_ApnAmbrInfo;



/* 结构说明: PDN_TYPE 参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opIpv4AddrAllocType : 1;
    VOS_UINT32 opSpare : 31;

    APP_ESM_ParaSetUint32           setType; /* 0:SET;1:DEL */
    APP_ESM_PdnTypeUint32           pdnType; /* 1:IPV4,2:IPV6,3:BOTH */
    APP_ESM_Ipv4AddrAllocTypeUint32 ipv4AddrAllocType;
} APP_ESM_SetPdnTypeReq;
typedef APP_ESM_ParaSetCnf APP_ESM_SetPdnTypeCnf;

/* 结构说明: BEARER_TYPE  承载类型参数设置 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opLinkdCid : 1;
    VOS_UINT32 opSpare : 31;

    APP_ESM_ParaSetUint32 setType;
    APP_ESM_CidTypeUint32 bearCidType;

    VOS_UINT32 linkdCid;
} APP_ESM_SetBearerTypeReq;

typedef APP_ESM_ParaSetCnf APP_ESM_SetBearerTypeCnf;

/* 结构说明: 设置APP对承载激活的应答方式 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;

    APP_ESM_ParaSetUint32      setType;
    APP_ESM_AnswerModeUint32   ansMode; /* APP应答模式:0:自动,1:手动 */
    APP_ESM_AnswerResultUint32 ansType; /* APP应答类型:0:接收,1:拒绝,手动模式下此参数无效 */
} APP_ESM_SetPdpManagerTypeReq;

typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 setRslt;
} APP_ESM_SetPdpManagerTypeCnf;

/* 结构说明: 设置网关鉴权数据信息 */
typedef struct {
    APP_ESM_AuthTypeUint8 gwAuthType;

    VOS_UINT8 accNumLen;
    VOS_UINT8 rsv1[2]; /* 保留字段，长度为2,便于4字节对齐 */
    VOS_UINT8 accessNum[APP_ESM_MAX_ACCESS_NUM_LEN]; /* 此参数保留，暂时不使用 */
    VOS_UINT8 userNameLen;
    VOS_UINT8 rsv2[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT8 userName[APP_ESM_MAX_USER_NAME_LEN + 1];
    VOS_UINT8 pwdLen;
    VOS_UINT8 rsv3[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT8 pwd[APP_ESM_MAX_PASSWORD_LEN + 1];
} APP_ESM_GwAuthInfo;

/* 结构说明: APP与ESM 业务流SDF参数的数据结构 */
typedef struct {
    VOS_UINT32 opPdnType : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opSdfQos : 1;
    VOS_UINT32 opLinkdCid : 1;
    VOS_UINT32 opIpv4AddrAllocType : 1;
    VOS_UINT32 opGwAuthInfo : 1;
    /* ims begin */
    VOS_UINT32 opPcscfDiscovery : 1;
    VOS_UINT32 opImsCnSignalFlag : 1;
    VOS_UINT32 opRoamPdnTypeFlag : 1;
    VOS_UINT32 opSpare : 23;
    /* ims end */

    APP_ESM_PdnTypeUint32           pdnType;
    APP_ESM_PdnTypeUint32           roamPdnType;
    APP_ESM_Ipv4AddrAllocTypeUint32 ipv4AddrAllocType;
    APP_ESM_BearerTypeUint32        bearerCntxtType; /* SM上下文对应承载的类型 */

    VOS_UINT32 cid;      /* 承载上下文ID */
    VOS_UINT32 linkdCid; /* 关联CID */

    VOS_UINT32         pfNum;
    APP_ESM_EpsQosInfo sdfQosInfo;
    APP_ESM_ApnInfo    apnInfo;
    APP_ESM_GwAuthInfo gwAuthInfo;
    APP_ESM_TftInfo    cntxtTftInfo[APP_ESM_MAX_SDF_PF_NUM];
    /* ims begin */
    APP_ESM_PcscfDiscoveryUint32 pcscfDiscovery;
    APP_ESM_ImsCnSigFlagUint32   imsCnSignalFlag;
    /* ims end */
} APP_ESM_SdfPara;

/*lint -save -e959*/
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    APP_ESM_ParaSetUint32 setType;
    APP_ESM_GwAuthInfo    gwAuthInfo;
} APP_ESM_SetGwAuthReq;
/*lint -restore*/

typedef APP_ESM_ParaSetCnf APP_ESM_SetGwAuthCnf;

/* 参数查询消息结构 */
/* 结构说明: 参数查询通用结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
} APP_ESM_InqParaReq;

typedef APP_ESM_InqParaReq APP_ESM_InqTftReq;

/* 结构说明: 参数查询:TFT */
typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32      msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32      opId;
    VOS_UINT32      cid;
    VOS_UINT32      rslt;
    VOS_UINT32      pfNum; /* 有效的PF数 */
    APP_ESM_TftInfo pfInfo[APP_ESM_TFT_MAX_PF_NUM];
} APP_ESM_InqTftCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqQosReq;

/* 结构说明: 参数查询:QOS */
typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32      msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32      opId;
    VOS_UINT32      cid;
    VOS_UINT32      rslt;
    APP_ESM_QosInfo stAppQoSInfo; /* APP_ESM_QosInfo结构定义 */
} APP_ESM_InqQosCnf;



typedef APP_ESM_InqParaReq APP_ESM_InqBearerTypeReq;

/* 结构说明: 参数查询:BEARER_TYPE */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 opLinkdCid : 1;
    VOS_UINT32 opSpare : 31;

    VOS_UINT32            rslt;
    APP_ESM_CidTypeUint32 bearCidType;
    VOS_UINT32            linkdCid; /* 关联承载的CID */
} APP_ESM_InqBearerTypeCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqPdnTypeReq;

/* 结构说明: 参数查询:PDN_TYPE */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opIpv4AddrAllocType : 1;
    VOS_UINT32 opSpare : 31;

    VOS_UINT32                      rslt;
    APP_ESM_PdnTypeUint32           pdnType; /* 1:IPV4,2:IPV6,3:BOTH */
    APP_ESM_Ipv4AddrAllocTypeUint32 ipv4AddrAllocType;
} APP_ESM_InqPdnTypeCnf;


/* 结构说明: 参数查询:APN */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32      rslt;
    APP_ESM_ApnInfo apnInfo;
} APP_ESM_InqApnCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqEpsQosReq;

/* 结构说明: 参数查询:SDF QOS */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 rslt;

    APP_ESM_EpsQosInfo sdfQosInfo;
} APP_ESM_InqEpsQosCnf;

/* 结构说明: 参数查询:BEARER_MANAGER_TYPE */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
} APP_ESM_InqPdpManagerTypeReq;

/* 结构说明: 参数查询:BEARER_MANAGER_TYPE */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;

    VOS_UINT32                 rslt;
    APP_ESM_AnswerModeUint32   ansMode; /* APP应答模式:0:自动,1:手动 */
    APP_ESM_AnswerResultUint32 ansType; /* APP应答类型:0:接收,1:拒绝,手动模式下此参数无效 */
} APP_ESM_InqPdpManagerTypeCnf;

/* 结构说明: 参数查询:BEARER_QOS */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */

    APP_MSG_HEADER
    VOS_UINT32 epsbId;
} APP_ESM_InqBeaerQosReq;

/* 结构说明: 参数查询:BEARER_QOS */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */

    APP_MSG_HEADER
    VOS_UINT32                 epsbIdNum;
    APP_EPS_Qos                qosInfo[APP_ESM_MAX_EPSB_NUM];
    APP_ESM_AnswerResultUint32 ansType;
} APP_ESM_InqBeaerQosCnf;

typedef struct {
    VOS_MSG_HEADER            /* _H2ASN_Skip */
    VOS_UINT32         msgId; /* _H2ASN_Skip */

    APP_MSG_HEADER
    VOS_UINT32         epsbId;
    APP_ESM_EpsQosInfo qosInfo;
} APP_ESM_InqBeaerQosInd;

typedef APP_ESM_InqParaReq APP_ESM_InqDynamicPdpContReq;

/* 结构说明: IP地址的结构体 */
typedef struct {
    VOS_UINT8 ipType;
    VOS_UINT8 reserved[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT8 ipv4Addr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 ipv6Addr[APP_MAX_IPV6_ADDR_LEN];
} APP_ESM_IpAddr;

/* 结构说明: 查询回复:动态PDP上下文信息 */
typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
        VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opPdnAddr : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opLinkCid : 1;
    VOS_UINT32 opDnsPrim : 1;
    VOS_UINT32 opDnsSec : 1;
    VOS_UINT32 opPcscfPrim : 1;
    VOS_UINT32 opPcscfSec : 1;
    VOS_UINT32 opGateWayAddrInfo : 1;
    /* ims begin */
    VOS_UINT32 opImsCnSignalFlag : 1;
    /* ims end */
    /* Modified for IPV4 MTU, 2016-11-15, Begin */
    VOS_UINT32 opIpv4Mtu : 1;
    VOS_UINT32 opSpare : 22;
    /* Modified for IPV4 MTU, 2016-11-15, End */

    VOS_UINT32      rslt;
    VOS_UINT32      epsbId;
    APP_ESM_IpAddr  pdnAddrInfo;
    APP_ESM_IpAddr  subnetMask;
    APP_ESM_IpAddr  gateWayAddrInfo;
    APP_ESM_IpAddr  dnsPrimAddrInfo;   /* 主DNS信息 */
    APP_ESM_IpAddr  dnsSecAddrInfo;    /* 辅DNS信息 */
    APP_ESM_IpAddr  pcscfPrimAddrInfo; /* 主P-CSCF信息 */
    APP_ESM_IpAddr  pcscfSecAddrInfo;  /* 辅P-CSCF信息 */
    APP_ESM_ApnInfo apnInfo;
    VOS_UINT32      linkCid;
    /* ims begin */
    APP_ESM_ImsCnSigFlagUint32 imsCnSignalFlag;
    /* ims end */
    /* Added for IPV4 MTU, 2016-11-15, Begin */
    VOS_UINT16 ipv4Mtu;
    VOS_UINT8  reserved[2]; /* 保留字段，长度为2,便于4字节对齐 */
    /* Added for IPV4 MTU, 2016-11-15, End */
} APP_ESM_InqDynamicPdpContCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqDynamicEpsQosReq;

/* 结构说明: 查询回复:动态EPS QoS信息 */
typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
        VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32         rslt;
    APP_ESM_EpsQosInfo sdfQosInfo;
} APP_ESM_InqDynamicEpsQosCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqDynamicTftReq;

/* 结构说明: 查询回复:动态TFT信息 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32      rslt;
    VOS_UINT32      pfNum; /* 有效的PF数 */
    APP_ESM_TftInfo pfInfo[APP_ESM_TFT_MAX_PF_NUM];
} APP_ESM_InqDynamicTftCnf;

typedef APP_ESM_InqParaReq APP_ESM_InqGwAuthReq;

/* 结构说明: 查询回复:网关鉴权信息 */
/*lint -save -e959*/
typedef struct {
    VOS_MSG_HEADER            /* _H2ASN_Skip */
    VOS_UINT32         msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32         opId;
    VOS_UINT32         cid;
    VOS_UINT32         rslt;
    APP_ESM_GwAuthInfo gwAuthInfo;
} APP_ESM_InqGwAuthCnf;
/*lint -restore*/

/* 承载建立、释放结构 */
/*
 * 结构说明: The struct contains the parameter of CHAP.
 *           challengeLen     - Length of username
 *           achallenge       - Array of challenge
 *           challengeNameLen - Length of challenge name
 *           challengeName    - Array of challenge name
 *           responseLen      - Length of response
 *           response         - Array of response
 *           responseNameLen  - Length of response name (username)
 *           responseName     - Array of response name (username)
 */
typedef struct {
    VOS_UINT32 challengeLen;
    VOS_UINT8  challenge[APP_ESM_PPP_CHAP_CHALLNGE_LEN];
    VOS_UINT32 challengeNameLen;
    VOS_UINT8  challengeName[APP_ESM_PPP_CHAP_CHALLNGE_NAME_LEN];
    VOS_UINT32 responseLen;
    VOS_UINT8  response[APP_ESM_PPP_CHAP_RESPONSE_LEN];
    VOS_UINT8  responseNameLen;
    VOS_UINT8  responseName[APP_ESM_PPP_MAX_USERNAME_LEN];
} APP_ESM_ChapParam;

/*
 * 结构说明: The struct contains the parameter of PAP.
 *           usernameLen - Length of username
 *           username    - Array of username
 *           passwordLen - Length of password
 *           password    - Array of password
 */
typedef struct {
    VOS_UINT8 usernameLen;
    VOS_UINT8 username[APP_ESM_PPP_MAX_USERNAME_LEN];
    VOS_UINT8 passwordLen;
    VOS_UINT8 password[APP_ESM_PPP_MAX_PASSWORD_LEN];
} APP_ESM_PapParam;

/*
 * 结构说明: The struct contains the authentication information.
 *           authType  - Auth Type (NONE/PAP/CHAP)
 *           chapParam - Parameters of CHAP
 *           papParam  - Parameters of PAP
 */
typedef struct {
    APP_ESM_AuthTypeUint8 authType;
    VOS_UINT8             authId;
    VOS_UINT8             rsv[2]; /* 保留字段，长度为2,便于4字节对齐 */
    union {
        APP_ESM_ChapParam chapParam;
        APP_ESM_PapParam  papParam;
    } unAuthParam;
} APP_ESM_AuthInfo;

/*
 * 结构说明: The struct contains the IPCP information.
 *           primDns  - Primary DNS address
 *           secDns   - Secondary DNS address
 *           primNbns - Primary NBNS address
 *           secNbns  - Secondary NBNS address
 */
typedef struct {
    VOS_UINT32 opIpAddr : 1;
    VOS_UINT32 opPrimDns : 1;
    VOS_UINT32 opSecDns : 1;
    VOS_UINT32 opPrimNbns : 1;
    VOS_UINT32 opSecNbns : 1;
    VOS_UINT32 opSpare : 27;

    VOS_UINT8 ipAddr[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 primDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 secDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 primNbns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 secNbns[APP_MAX_IPV4_ADDR_LEN];
} APP_ESM_IpcpInfo;

/*
 * 结构说明: The struct contains the PPP information.
 *           authInfo - Authentication information
 *           ipcpInfo - IPCP information
 */
typedef struct {
    APP_ESM_AuthInfo authInfo; /* PPP拨号鉴权信息 */
    APP_ESM_IpcpInfo ipcpInfo; /* PPP拨号IPCP信息，ESM不使用 */
} APP_ESM_PppInfo;

typedef struct {
    VOS_UINT32                epsbId;
    NAS_ESM_DtBearerTypeUint8 bearerType; /* 承载类型 */
    VOS_UINT8                 rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */
    APP_ESM_ApnInfo           apn;        /* APN信息 */
    APP_ESM_IpAddr            ipAddr;     /* UE IP地址信息 */
    APP_ESM_EpsQosInfo        sdfQosInfo; /* QoS参数 */
} NAS_OM_EpsbInfo;

typedef struct {
    VOS_UINT32      actEpsbNum; /* 激活承载的个数 */
    NAS_OM_EpsbInfo epsbInfo[APP_ESM_MAX_EPSB_NUM];
} NAS_OM_ActPdpInfo;

typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */

    APP_MSG_HEADER
    VOS_UINT32 opId;

    DT_CmdUint8 cmd;
    VOS_UINT8   rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */
    VOS_UINT32  rptPeriod;
} APP_ESM_DtInqCmdReq;

typedef APP_ESM_DtInqCmdReq APP_ESM_InqPdpInfoReq;

/* DT ,2014/4/24, CNF 不上报消息体，在IND上报,begin */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 rslt;
} APP_ESM_InqPdpInfoCnf;
/* DT ,2014/4/24, CNF 不上报消息体，在IND上报,end */

typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;

    NAS_OM_ActPdpInfo actPdpInfo;
} APP_ESM_InqPdpInfoInd;
/* DT end */

/* 承载建立、释放结构 */

/* moded for pco requirement 2017-03-25, begin */
/*
 * 协议表格: PCO信元格式参考24008-10.5.6.3
 *           PCO-FF00H项的格式:
 *           Container Identifier     -FF00H(2 bytes)
 *           Container Content Length -MAX 50 octets(1 bytes)
 *           Container Content Format -MCCMNC(3 octets),
 *                                     ACTION(value:0-5;  6-255:reserved)
 *           4字节对齐
 * 结构说明: ESM -> APS
 */
typedef struct {
    VOS_UINT16 containerId;
    VOS_UINT8  contentLen;
    VOS_UINT8  contents[APP_ESM_MAX_CUSTOM_PCO_CONTAINER_CONTENT_LEN];
} APP_ESM_CustomPcoContainer;

/*
 * 协议表格: PCO信元中的运营商定制项，PCO信元格式参考24008-10.5.6.3，
 *           除定制项外,预留两项预备其他运营商定制用
 *           定制列表中的3项默认第一项为FF00H
 * 结构说明: ESM -> APS
 */
typedef struct {
    VOS_UINT32                 containerNum;
    APP_ESM_CustomPcoContainer containerList[APP_ESM_MAX_CUSTOM_PCO_CONTAINER_NUM];
} APP_ESM_CustomPcoInfo;
/* moded for pco requirement 2017-03-25, end */

typedef struct {
    VOS_UINT8 ipv4Addr[APP_MAX_IPV4_ADDR_LEN];
} APP_ESM_Ipv4Epdg;

typedef struct {
    VOS_UINT8 ipv6Addr[APP_MAX_IPV6_ADDR_LEN];
} APP_ESM_Ipv6Epdg;

/*
 * 协议表格: 定制EPDG地址信息
 * 结构说明: ESM -> TAF
 */
typedef struct {
    VOS_UINT16       ipv4EpdgNum;
    VOS_UINT16       ipv6EpdgNum;
    APP_ESM_Ipv4Epdg ipv4EpdgList[APP_ESM_MAX_IPV4_EPDG_NUM];
    APP_ESM_Ipv6Epdg ipv6EpdgList[APP_ESM_MAX_IPV6_EPDG_NUM];
} APP_ESM_EpdgInfo;

/* 结构说明: APP发起承载建立请求消息 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId; /* 本次操作标识符 */

    APP_ESM_PdpSetupTypeUint32 setupType; /* 正常PDP激活还是PPP拨号导致 */

    VOS_UINT32      cid; /* 上下文序列号，范围:0~31 */
    VOS_UINT8       pduSessionId;
    VOS_UINT8       rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */
    APP_ESM_PppInfo pppInfo; /* PPP拨号参数信息 */
    APP_ESM_BearerPrioUint32 bearerPrio; /* NAS signalling low priority标识 */
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */
} APP_ESM_PdpSetupReq;


typedef struct {
    VOS_UINT8 plmnId[3]; /* plmnId用3字节存储 */
    VOS_UINT8 rsv;
} APP_ESM_PlmnId;

/*
 * 结构说明: ESM给APS同步的S_NSSAI的结构,参考24.501 9.10.2.6
 *           比5G下的切片结构多一个PLMN ID
 * 修改历史:
 *  1.日    期: 2018年08月16日
 *    修改内容: 新增结构
 */
typedef struct {
    PS_S_NSSAI_STRU snssai;
    APP_ESM_PlmnId  plmnId;
} APP_ESM_SNssai;

/* 结构说明: 承载建立回复消息 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opLinkCid : 1;
    VOS_UINT32 opBearerState : 1;
    VOS_UINT32 opBearerType : 1;
    VOS_UINT32 opPdnAddr : 1;
    VOS_UINT32 opRabId : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opSdfQos : 1;
    VOS_UINT32 opDnsPrim : 1;
    VOS_UINT32 opDnsSec : 1;
    VOS_UINT32 opPcscfPrim : 1;
    VOS_UINT32 opPcscfSec : 1;
    VOS_UINT32 opGateWayAddrInfo : 1;
    /* added for pco requirement 2017-03-25, begin */
    VOS_UINT32 opCustomPco : 1; /* 用于定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */
    /* added for pco requirement 2017-03-25, end */
    VOS_UINT32 opEpdgInfo : 1; /* 用于定制需求中,把EPDG地址上报给APS时,指示是否要上报给IMSA。 1:需要 0:不需要 */
    VOS_UINT32 opSscMode : 1;
    VOS_UINT32 opSnssai : 1;
    VOS_UINT32 opSessionAmbr : 1;
    VOS_UINT32 opSpare : 15;

    VOS_UINT32 rslt; /* 删除操作结果;取值范围:参见附录3.1 */
    VOS_UINT32 linkCid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    APP_ESM_BearerStateUint32 bearerState; /* 当前承载状态:0~1 */
    APP_ESM_BearerTypeUint32  bearerType;  /* 承载类型 */
    APP_ESM_IpAddr            pdnAddrInfo;
    APP_ESM_IpAddr            subnetMask;
    APP_ESM_IpAddr            gateWayAddrInfo;   /* 网关信息 */
    APP_ESM_IpAddr            dnsPrimAddrInfo;   /* 主DNS信息 */
    APP_ESM_IpAddr            dnsSecAddrInfo;    /* 辅DNS信息 */
    APP_ESM_IpAddr            pcscfPrimAddrInfo; /* 主P-CSCF信息 */
    APP_ESM_IpAddr            pcscfSecAddrInfo;  /* 辅P-CSCF信息 */
    VOS_UINT32                rabId;             /* 用户面承载标识，数传时使用，其值暂时为ulEpsBid */
    APP_ESM_ApnInfo           apnInfo;
    APP_ESM_EpsQosInfo        sdfQosInfo;

    /* add for active pdp timer expire, 2016-10-20, begin */
    VOS_UINT8 expiredCount; /* 发起激活的超时次数 */
    /* add for active pdp timer expire, 2016-10-20, end */
    PS_SSC_ModeUint8 sscMode; /* 24501-9.10.4.12 */
    VOS_UINT8        pduSessionId;
    VOS_UINT8        reserve;
    /* added for pco requirement 2017-03-25, begin */
    APP_ESM_CustomPcoInfo customPcoInfo;
    /* added for pco requirement 2017-03-25, end */
    APP_ESM_EpdgInfo epdgInfo;

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    /* 为了减少非NR版本中消息大小，暂时用宏控制 */
    APP_ESM_SNssai     snssaiInfo;     /* 5G S-NASSAI */
    PS_PDU_SessionAmbr pduSessionAmbr; /* 5G SESSION AMBR */

    PS_EPS_MapQosFlowList mapQosFlowList;
    PS_EPS_MapQosRuleList mapQosRuleList;
#endif
} APP_ESM_PdpSetupCnf;

typedef APP_ESM_PdpSetupCnf APP_ESM_PdpSetupInd;

/* 结构说明: APP发起承载修改请求消息 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId; /* 本次操作标识符 */
    VOS_UINT32 cid;  /* 上下文序列号，范围:0~31 */
    APP_ESM_BearerPrioUint32 bearerPrio; /* NAS signalling priority标识 */
    /* add for PS CALL ID,2018-05-22,begin */
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */
    /* add for PS CALL ID,2018-05-22,end */
} APP_ESM_PdpModifyReq;

typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    APP_ESM_BearerModifyUint32 bearerModifyType;

    VOS_UINT32 opBearerState : 1;
    VOS_UINT32 opBearerType : 1;
    VOS_UINT32 opRabId : 1;
    VOS_UINT32 opSdfQos : 1;
    /* added for  pco requirement 2017-03-25, begin */
    VOS_UINT32 opCustomPco : 1; /* 用于定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */
    /* added for  pco requirement 2017-03-25, end */
    VOS_UINT32 opSpare : 27;

    VOS_UINT32 rslt; /* 删除操作结果;取值范围:参见附录3.1 */

    APP_ESM_BearerStateUint32 bearerState; /* 当前承载状态:0~1 */
    APP_ESM_BearerTypeUint32  bearerType;  /* 承载类型 */

    VOS_UINT32 rabId; /* 用户面承载标识，数传时使用，其值暂时为ulEpsBid */

    APP_ESM_EpsQosInfo sdfQosInfo;

    /* added for  pco requirement 2017-03-25, begin */
    APP_ESM_CustomPcoInfo customPcoInfo;
    /* added for  pco requirement 2017-03-25, end */
} APP_ESM_PdpModifyCnf;

typedef APP_ESM_PdpModifyCnf APP_ESM_PdpModifyInd;

/* 结构说明: APP发起承载释放请求 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    APP_ESM_PdpReleaseCauseUint8 cause;     /* 释放原因 */
    VOS_UINT8                    detachInd; /* 是否需要执行DETACH，VOS_TRUE是，VOS_FALS否 */
    VOS_UINT8                    rsv[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_PdpReleaseReq;

/* 结构说明: 承载释放回复消息 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    VOS_UINT32 opLinkCid : 1;
    VOS_UINT32 opPdnAddr : 1;
    /* added for  pco requirement 2017-03-25, begin */
    VOS_UINT32 opCustomPco : 1; /* 用于定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */
    /* added for  pco requirement 2017-03-25, end */
    VOS_UINT32 opSpare : 29;

    VOS_UINT32 rslt;

    VOS_UINT32     linkCid;
    APP_ESM_IpAddr pdnAddrInfo;

    /* add for VIA CL MT-DETACH make CID mod problem begin */
    /* 给APP上报RELEASE IND时(RELEASE CNF不填);    */
    VOS_UINT32 epsbId;
    /* add for VIA CL MT-DETACH make CID mod problem end */

    /* added for  pco requirement 2017-03-25, begin */
    APP_ESM_CustomPcoInfo customPcoInfo;
    /* added for  pco requirement 2017-03-25, end */
    VOS_UINT8 pduSessionId;
    /* fix pclint, add begin, 2018-11-06 */
    VOS_UINT8 reserved[3]; /* 保留字段，长度为3,便于4字节对齐 */
    /* fix pclint, add end, 2018-11-06 */
} APP_ESM_PdpReleaseCnf;

typedef APP_ESM_PdpReleaseCnf APP_ESM_PdpReleaseInd;

/* 结构说明: 承载建立请求APP响应 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31，上报前，先调用TAF_APS_GetUnusedPsCallId()接口获取 */
    VOS_UINT32 opLinkCid : 1;
    VOS_UINT32 opModifyTpye : 1;
    VOS_UINT32 opSpare : 30;
    VOS_UINT32 linkCid;

    APP_ESM_BearerOperateTypeUint32 operateType;
    APP_ESM_BearerModifyUint32      bearerModifyType;
} APP_ESM_PdpManagerInd;

/* 结构说明: APP响应承载建立请求 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    APP_ESM_BearerActResultUint32 setupRslt; /* 响应结果:0:接收分配或修改请求,1:拒绝请求 */
} APP_ESM_PdpManagerRsp;

/*
 * 结构说明  : APS-->ESM IPv6 Prefix结构
 */
/* APS融合新增内容, IPV6前缀 */
typedef struct {
    VOS_UINT8 prefixLen;
    VOS_UINT8 reserved[3]; /* 保留字段，长度为3,便于4字节对齐 */

    VOS_UINT8 prefix[APP_ESM_IPV6_ADDR_LEN];
} APP_ESM_Ipv6Prefix;

/* 结构说明: APS-->ESM IPv6 Notify */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32         opId;
    VOS_UINT8          epsbId;
    VOS_UINT8          ipv6PrefixNum;
    VOS_UINT8          reserved[2]; /* 保留字段，长度为2,便于4字节对齐 */
    APP_ESM_Ipv6Prefix ipv6PrefixArray[APP_ESM_MAX_IPV6_PREFIX_NUM];
} APP_ESM_Ipv6InfoNotify;

/* modify begin */
/* 结构说明: APS-->ESM 流程终止消息结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */
} APP_ESM_ProcedureAbortNotify;
/* modify end */

/* by begin 2013-05-29 Modify L4A */

/* ims begin */
/* 结构说明: APS-->ESM 拨号消息结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    VOS_UINT8 pduSessionId;
    VOS_UINT8 rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */

    VOS_UINT32 opPdnType : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opGwAuth : 1;
    /* modify 2014-05-09 begin */
    VOS_UINT32 opIpv4AddrAlloc : 1;  /* Ipv4AddrAlloc */
    VOS_UINT32 opPcscfDiscovery : 1; /* P-CSCF discovery */
    VOS_UINT32 opImCnSignalFlg : 1;  /* IM CN Signalling Flag */
    VOS_UINT32 opSpare : 26;
    /* modify 2014-05-09 end */

    APP_ESM_ApnInfo       apnInfo;
    APP_ESM_PdnTypeUint32 pdnType;
    APP_ESM_GwAuthInfo    gwAuthInfo;
    /* Modify for CL multimode 2014-02-14 begin */
    APP_ESM_PdnRequestTypeUint32 pdnReqestType;
    /* Modify for CL multimode 2014-02-14 end */
    /* 2014-05-09 begin */
    APP_ESM_PdpEmcIndUint32         emergencyInd;
    APP_ESM_Ipv4AddrAllocTypeUint32 ipv4AddrAlloc;
    APP_ESM_PcscfDiscoveryUint32    pcscfDiscovery;
    APP_ESM_ImsCnSigFlagUint32      imCnSignalFlg;
    /* 2014-05-09 end */
    APP_ESM_BearerPrioUint32 bearerPrio;
} APP_ESM_NdisconnReq;
/* ims end */

/* 结构说明: 回复APS-->ESM 拨号消息结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;
    VOS_UINT32 psCallId; /* 呼叫实体ID，范围:0~31 */

    VOS_UINT32 opBearerState : 1;
    VOS_UINT32 opBearerType : 1;
    VOS_UINT32 opPdnAddr : 1;
    VOS_UINT32 opRabId : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opSdfQos : 1;
    VOS_UINT32 opDnsPrim : 1;
    VOS_UINT32 opDnsSec : 1;
    VOS_UINT32 opPcscfPrim : 1;
    VOS_UINT32 opPcscfSec : 1;
    VOS_UINT32 opGateWayAddrInfo : 1;
    /* added for  pco requirement 2017-03-25, begin */
    VOS_UINT32 opCustomPco : 1; /* 用于定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */
    /* added for  pco requirement 2017-03-25, end */
    VOS_UINT32 opEpdgInfo : 1; /* 用于定制需求中,把EPDG地址上报给APS时,指示是否上报给IMSA。 1:需要 0:不需要 */
    VOS_UINT32 opSscMode : 1;
    VOS_UINT32 opSnssai : 1;
    VOS_UINT32 opSessionAmbr : 1;
    VOS_UINT32 opSpare : 16;

    VOS_UINT32 rslt; /* 取值范围:参见附录3.1 */

    APP_ESM_BearerStateUint32 bearerState; /* 当前承载状态:0~1 */
    APP_ESM_BearerTypeUint32  bearerType;  /* 承载类型 */

    APP_ESM_IpAddr     pdnAddrInfo;
    APP_ESM_IpAddr     subnetMask;
    APP_ESM_IpAddr     gateWayAddrInfo;
    APP_ESM_IpAddr     dnsPrimAddrInfo;   /* 主DNS信息 */
    APP_ESM_IpAddr     dnsSecAddrInfo;    /* 辅DNS信息 */
    APP_ESM_IpAddr     pcscfPrimAddrInfo; /* 主P-CSCF信息 */
    APP_ESM_IpAddr     pcscfSecAddrInfo;  /* 辅P-CSCF信息 */
    VOS_UINT32         rabId;             /* 用户面承载标识，数传时使用，其值为ulEpsbId */
    APP_ESM_ApnInfo    apnInfo;
    APP_ESM_EpsQosInfo sdfQosInfo;

    /* add for active pdp timer expire, 2016-10-20, begin */
    VOS_UINT8 expiredCount; /* 发起激活的超时次数 */
    /* add for active pdp timer expire, 2016-10-20, end */
    PS_SSC_ModeUint8 sscMode; /* 24501-9.10.4.12 */
    VOS_UINT8        pduSessionId;
    VOS_UINT8        reserve;
    /* added for  pco requirement 2017-03-25, begin */
    APP_ESM_CustomPcoInfo customPcoInfo;
    /* added for  pco requirement 2017-03-25, end */
    APP_ESM_EpdgInfo epdgInfo;

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    /* 为了减少非NR版本中消息大小，暂时用宏控制 */
    APP_ESM_SNssai     snssaiInfo;     /* 5G S-NASSAI */
    PS_PDU_SessionAmbr pduSessionAmbr; /* 5G SESSION AMBR */

    PS_EPS_MapQosFlowList mapQosFlowList;
    PS_EPS_MapQosRuleList mapQosRuleList;
#endif
} APP_ESM_NdisconnCnf;

/* 结构说明: CGDCONTAT命令的消息结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 opBearType : 1;
    VOS_UINT32 opPdnType : 1;
    VOS_UINT32 opApn : 1;

    VOS_UINT32 opIpv4AddrAllocType : 1;
    /* ims begin */
    VOS_UINT32 opPcscfDiscovery : 1;
    VOS_UINT32 opImsCnSignalFlag : 1;
    /* ims end */
    VOS_UINT32 opSpare : 26;

    APP_ESM_ParaSetUint32           setType; /* 0:SET;1:DEL */
    APP_ESM_CidTypeUint32           bearCidType;
    APP_ESM_ApnInfo                 apnInfo;
    APP_ESM_PdnTypeUint32           pdnType;
    APP_ESM_Ipv4AddrAllocTypeUint32 ipv4AddrAllocType;
    /* ims begin */
    APP_ESM_PcscfDiscoveryUint32 pcscfDiscovery;
    APP_ESM_ImsCnSigFlagUint32   imsCnSignalFlag;
    /* ims end */
} APP_ESM_SetCgdcontReq;

/* 结构说明: 回复CGDCONTAT命令的消息结构 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    APP_MSG_HEADER
    VOS_UINT32 opId;
    VOS_UINT32 cid;

    VOS_UINT32 rslt;
} APP_ESM_SetCgdcontCnf;
/* end 2013-05-29 Modify L4A */

/* 结构说明: PDP管理模式信息结构 */
typedef struct {
    APP_ESM_BearerManageModeUint8 mode;        /* 承载管理模式 */
    APP_ESM_BearerManageTypeUint8 type;        /* 承载管理类型 */
    VOS_UINT8                     reserve1[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_PdpManageInfo;

/* 结构说明: 数据业务开关信息结构 */
typedef struct {
    VOS_UINT8 dataSwitch;     /* 数据业务开关状态 */
    VOS_UINT8 dataRoamSwitch; /* 数据业务漫游开关状态 */

    VOS_UINT8 resv[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_DataSwitchInfo;

typedef struct {
    APP_ESM_MsgTypeUint32 msgId; /* _H2ASN_MsgChoice_Export APP_ESM_MsgTypeUint32  */
    VOS_UINT8             msg[4]; /* 消息前4字节 */
    /* _H2ASN_MsgChoice_When_Comment          APP_ESM_MSG_TYPE_ENUM_UINT32 */
} APP_ESM_InterfaceMsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    APP_ESM_InterfaceMsgData stMsgData;
} APP_ESM_InterfaceMsg;

/*
 * 结构说明: CID联合PF的结构
 */
typedef struct {
    VOS_UINT32      cid;
    APP_ESM_TftInfo pf;
} APP_ESM_CidPf;
/*
 * 结构说明: TFT结构
 */
typedef struct {
    VOS_UINT32    pfNum;
    APP_ESM_CidPf cidPf[PS_MAX_PF_NUM_IN_TFT];
} APP_ESM_Tft;

/*
 * 结构说明: Transaction ID 参数
 */
typedef struct {
    VOS_UINT8 tiFlag;
    VOS_UINT8 tiValue;

    VOS_UINT8 reserved[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_TransactionId;


typedef struct {
    VOS_UINT8 pcscfAddr[APP_MAX_IPV4_ADDR_LEN];
} APP_ESM_Ipv4Pcscf;


typedef struct {
    VOS_UINT8 pcscfAddr[APP_MAX_IPV6_ADDR_LEN];
} APP_ESM_Ipv6Pcscf;


typedef struct {
    VOS_UINT8 ipv4PcscfAddrNum; /* IPV4的P-CSCF地址个数，有效范围[0,8] */
    VOS_UINT8 rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */

    APP_ESM_Ipv4Pcscf ipv4PcscfAddrList[APP_ESM_PCSCF_ADDR_MAX_NUM];
} APP_ESM_Ipv4PcscfList;


typedef struct {
    VOS_UINT8 ipv6PcscfAddrNum; /* IPV6的P-CSCF地址个数，有效范围[0,8] */
    VOS_UINT8 rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */

    APP_ESM_Ipv6Pcscf ipv6PcscfAddrList[APP_ESM_PCSCF_ADDR_MAX_NUM];
} APP_ESM_Ipv6PcscfList;

/*
 * 结构说明: priDns   - Primary DNS server Address
 *           secDns   - Secondary DNS server Address
 *           gateWay  - Peer IP address
 *           priNbns  - Primary WINS DNS address
 *           secNbns  - Seocndary WINS DNS address
 *           priPcscf - Primary P-CSCF address
 *           secPcscf - Seocndary P-CSCF address
 *           gateWay[0]为地址高字节位，gateWay[3]为地址低字节位
 *           priNbns[0]为地址高字节位，priNbns[3]为地址低字节位
 *           secNbns[0]为地址高字节位，secNbns[3]为地址低字节位
 */
typedef struct {
    VOS_UINT32 opPriDns : 1;
    VOS_UINT32 opSecDns : 1;
    VOS_UINT32 opGateWay : 1;
    VOS_UINT32 opPriNbns : 1;
    VOS_UINT32 opSecNbns : 1;
    VOS_UINT32 opIpv4Mtu : 1;
    VOS_UINT32 opSpare : 26;

    VOS_UINT8 priDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 secDns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 gateWay[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 priNbns[APP_MAX_IPV4_ADDR_LEN];
    VOS_UINT8 secNbns[APP_MAX_IPV4_ADDR_LEN];

    APP_ESM_Ipv4PcscfList ipv4PcscfList;

    VOS_UINT16 ipv4Mtu;
    VOS_UINT8  reserved[2]; /* 保留字段，长度为2,便于4字节对齐 */
} APP_ESM_PcoIpv4Item;

/*
 * 结构说明: priDns   - Primary DNS server Address
 *           secDns   - Secondary DNS server Address
 *           priPcscf - Primary P-CSCF address
 *           secPcscf - Seocndary P-CSCF address
 *           priDns[0]为地址高字节位，priDns[15]为地址低字节位
 *           secDns[0]为地址高字节位，secDns[15]为地址低字节位
 */
typedef struct {
    VOS_UINT32 opPriDns : 1;
    VOS_UINT32 opSecDns : 1;
    VOS_UINT32 opSpare : 30;

    VOS_UINT8 priDns[APP_MAX_IPV6_ADDR_LEN];
    VOS_UINT8 secDns[APP_MAX_IPV6_ADDR_LEN];

    APP_ESM_Ipv6PcscfList ipv6PcscfList;
} APP_ESM_PcoIpv6Item;

/*
 * 结构说明: ESM -> APS
 *           EPS Bearer的任何操作(Activate, Modify, Deactivate), 如果EPS的承载信息
 *           中包含TI, 则在次消息中必须填写. 如果消息中不携带TI, SM直接丢弃该消息
 *           linked EPS bearer identity (if available) 映射为 linked TI;
 *           PDN address and APN of the default EPS bearer context映射为PDP address and APN of the default PDP context;
 *           TFT of the default EPS bearer context映射为 the TFT of the default PDP context;
 *           TFTs of the dedicated EPS bearer contexts 映射为TFTs of the secondary PDP contexts;
 *           当操作类型为SM_ESM_PDP_OPT_MODIFY时，OP域为0，则表示不更新此项；OP域为1，则表示更新此项；
 *           当需要删除TFT信息时，将bitOpPf设为1，ulPfNum设为0；
 *           当要删除DNS，NBNS,P-CSCF等项时，需要将相应OP域设为1，相应项内容填为全0；
 *           ucLlcSapi，ucRadioPriority，ucPacketFlowId这三项的合法性由GU模的SM来做
 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */

    VOS_UINT32 opLinkedEpsbId : 1;
    VOS_UINT32 opEpsQos : 1;
    VOS_UINT32 opTft : 1;
    VOS_UINT32 opApn : 1;
    VOS_UINT32 opApnAmbr : 1;
    VOS_UINT32 opTransId : 1;
    VOS_UINT32 opNegQos : 1;
    VOS_UINT32 opLlcSapi : 1;
    VOS_UINT32 opRadioPriority : 1;
    VOS_UINT32 opPacketFlowId : 1;
    VOS_UINT32 opBcm : 1;
    VOS_UINT32 opEmcInd : 1;
    VOS_UINT32 opImCnSigalFlag : 1;
    VOS_UINT32 opErrorCode : 1;
    VOS_UINT32 opCustomPco : 1; /* 用于定制需求中,把FF00H上报给APS时,指示FF00H是否需要上报。 1:需要 0:不需要 */
    /* Added for no n26, 2019-1-17, begin */
    VOS_UINT32 opSnssai : 1;
    /* Added for no n26, 2019-1-17, end */

    VOS_UINT32 opSpare : 16;

    APP_ESM_HandoverResultUint32 result;
    VOS_UINT32                   epsbId;
    VOS_UINT32                   bitCid;
    VOS_UINT32                   linkedEpsbId; /* 与此承载关联的EPS承载ID */

    VOS_UINT8        psCallId;
    VOS_UINT8        pduSessionId;
    PS_SSC_ModeUint8 sscMode; /* 默认为SSC MODE1，具体参考24501-9.10.4.12 */
    VOS_UINT8        rsv[1]; /* 保留字段，长度为1,便于4字节对齐 */

    APP_ESM_IpAddr      pdnAddr;
    PS_EPS_ApnAmbr      epsApnAmbr;
    APP_ESM_EpsQosInfo  epsQos;
    APP_ESM_Tft         tft;
    APP_ESM_Apn         apn;
    APP_ESM_QosInfo     negQos;
    APP_ESM_PcoIpv4Item pcoIpv4Item;
    APP_ESM_PcoIpv6Item pcoIpv6Item;

    VOS_UINT8             llcSapi;
    VOS_UINT8             radioPriority;
    VOS_UINT8             packetFlowId;
    APP_ESM_BcmUint8      bcm;
    APP_ESM_TransactionId transId;

    APP_ESM_PdpEmcIndUint32    emergencyInd;
    APP_ESM_ImsCnSigFlagUint32 imCnSignalFlag;

    VOS_UINT32            errorCode; /* 收到网侧原因值后通知SM的错误码 */
    APP_ESM_CustomPcoInfo customPcoInfo;
    APP_ESM_SNssai        snssaiInfo; /* 5G S-NASSAI */
} APP_ESM_NrHandoverToLteInd;

/* Added for HO ATTACH, 2018-12-15 begin */
/* 结构说明: APS -> ESM 承载同步通知 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
} APP_ESM_SyncBearerInfoNotify;
/* Added for HO ATTACH, 2018-12-15 end */

/* 结构说明: 漫游PDN IP类型设置请求 */
typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32            msgId; /* _H2ASN_Skip */
    VOS_UINT8             cid;
    VOS_UINT8             rsv[3]; /* 保留字段，长度为3,便于4字节对齐 */
    APP_ESM_ParaSetUint32 setType; /* 0:SET;1:DEL */
    APP_ESM_PdnTypeUint32 pdpType;
} APP_ESM_SetRoamingPdnTypeReq;

/* 结构说明: 漫游PDN IP类型设置回复 */
typedef struct {
    VOS_MSG_HEADER    /* _H2ASN_Skip */
    VOS_UINT32 msgId; /* _H2ASN_Skip */
    VOS_UINT32 rslt;
} APP_ESM_SetRoamingPdnTypeCnf;

/* 结构说明: APS -> ESM 本地去激活通知 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    APS_ESM_LocalRelCauseUint32 cause;
    VOS_UINT8  pduSessionId;
    VOS_UINT8  psCallId;
    VOS_UINT8  epsbid;
    VOS_UINT8  rsv;
} APS_ESM_LocalReleaseNotify;

typedef struct {
    VOS_UINT32                  opPdnType     : 1;
    VOS_UINT32                  opApn         : 1;
    VOS_UINT32                  opGwAuthInfo  : 1;
    VOS_UINT32                  opSpare       : 29;


    APP_ESM_PdnTypeUint32       pdnType;
    APP_ESM_AuthTypeUint8       gwAuthType;
    VOS_UINT8                   apnLen;
    VOS_UINT8                   userNameLen;
    VOS_UINT8                   pwdLen;
    VOS_UINT8                   apn[APP_MAX_LTE_ATTACH_PROFILE_NAME_LEN + 1];
    VOS_UINT8                   userName[APP_MAX_LTE_ATTACH_PROFILE_USERNAME_LEN + 1];
    VOS_UINT8                   pwd[APP_MAX_LTE_ATTACH_PROFILE_USERPWD_LEN + 1];
    VOS_UINT8                   rsv;
}APP_ESM_AttachProfileInfo;



/* 结构说明: DSM -> ESM通知定制profile list */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                      msgId;
    VOS_UINT8                       totalProNum; /* 总profile个数 */
    VOS_UINT8                       curProNum;   /* 当前列表中的profile个数 */
    VOS_UINT8                       sendCmplFlg; /* 发送完成标识 */
    VOS_UINT8                       rsv;
    APP_ESM_AttachProfileInfo       profileList[APP_MAX_PROFILE_NUM];

} DSM_ESM_ProfileListNtf;


/* 枚举说明: PROFILE的注册结果 */
enum ESM_DSM_RegRslt {
    ESM_DSM_REG_RSLT_CUR_PROFILE_SUCC,
    ESM_DSM_REG_RSLT_ALL_PROFILE_FAIL,
    ESM_DSM_REG_RSLT_BUTT
};
typedef VOS_UINT32 ESM_DSM_RegRsltUint32;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                      msgId;
    ESM_DSM_RegRsltUint32           rslt;
    VOS_UINT32                      succRegProIdx; /* 仅当rslt为CUR_PROFILE_SUCC时使用 */
} ESM_DSM_ProfileRegStatusNtf;

extern VOS_UINT32 APP_GetSdfParaByModemId(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT32* sdfNum,
                                          APP_ESM_SdfPara* sdfPara);
extern VOS_UINT32 APP_GetPdpManageInfoByModemId(MODEM_ID_ENUM_UINT16   modemId,
                                                APP_ESM_PdpManageInfo* pdpManageInfo);
extern VOS_UINT32 APP_GetCidImsSuppFlagByModemId(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT8 cid,
                                                 VOS_UINT8* imsSuppFlag);
extern VOS_UINT32 APP_ESM_GetAllApnClassInfo(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT32* apnNum,
                                             APP_ESM_ApnClassInfo* apnClassInfo);

extern VOS_UINT32 APP_GetDataSwitchInfoByModemId(MODEM_ID_ENUM_UINT16    modemId,
                                                 APP_ESM_DataSwitchInfo* dataSwitchInfo);

extern VOS_UINT32 APP_AllocPsCallIdByModemId(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT8* psCallId);

extern VOS_UINT32 APP_AllocPduSessionIdByModemId(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT8* pduSessionId);

extern VOS_VOID APP_FreePduSessionIdByModemId(MODEM_ID_ENUM_UINT16 modemId, VOS_UINT8 pduSessionId);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of app_esm_interface.h */
