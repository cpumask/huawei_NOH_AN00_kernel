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

#ifndef __APP_NAS_COMM_H__
#define __APP_NAS_COMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "lps_common.h" /*lint !e537*/

#pragma pack(push, 4)

/* ���������� */
#define APP_ERR_COMMON_SECTION_BEGIN (0)
#define APP_ERR_SM_SECTION_BEGIN (200)
#define APP_ERR_SM_NW_SECTION_BEGIN (400)
#define APP_ERR_MM_SECTION_BEGIN (600)
#define APP_ERR_MM_NW_SECTION_BEGIN (800)
/* Added for HO ATTACH, 2018-12-20 begin */
#define APP_ERR_EHSM_ESM_SECTION_BEGIN (1000)
/* Added for HO ATTACH, 2018-12-20 end */

/* ���������� */
#define APP_SUCCESS (APP_ERR_COMMON_SECTION_BEGIN + 0)
#define APP_FAILURE (APP_ERR_COMMON_SECTION_BEGIN + 1)
#define APP_ERR_NULL_PTR (APP_ERR_COMMON_SECTION_BEGIN + 2)
#define APP_ERR_PARA_ERROR (APP_ERR_COMMON_SECTION_BEGIN + 3)
#define APP_ERR_TIME_OUT (APP_ERR_COMMON_SECTION_BEGIN + 4)
#define APP_ERR_ID_INVALID (APP_ERR_COMMON_SECTION_BEGIN + 5)
#define APP_ERR_NUM_VALUE_INVALID (APP_ERR_COMMON_SECTION_BEGIN + 6)
#define APP_ERR_NUM_LEN_INVALID (APP_ERR_COMMON_SECTION_BEGIN + 7)
#define APP_ERR_CAPABILITY_ERROR (APP_ERR_COMMON_SECTION_BEGIN + 8)
#define APP_ERR_CLIENTID_NO_FREE (APP_ERR_COMMON_SECTION_BEGIN + 9)
#define APP_ERR_CALLBACK_FUNC_ERROR (APP_ERR_COMMON_SECTION_BEGIN + 10)
#define APP_ERR_MSG_DECODING_FAIL (APP_ERR_COMMON_SECTION_BEGIN + 11)
#define APP_ERR_TI_ALLOC_FAIL (APP_ERR_COMMON_SECTION_BEGIN + 12)
#define APP_ERR_TI_GET_FAIL (APP_ERR_COMMON_SECTION_BEGIN + 13)
#define APP_ERR_CMD_TYPE_ERROR (APP_ERR_COMMON_SECTION_BEGIN + 14)
#define APP_ERR_MUX_LINK_EST_FAIL (APP_ERR_COMMON_SECTION_BEGIN + 15)
#define APP_ERR_USIM_SIM_CARD_NOTEXIST (APP_ERR_COMMON_SECTION_BEGIN + 16)
#define APP_ERR_CLIENTID_NOT_EXIST (APP_ERR_COMMON_SECTION_BEGIN + 17)
#define APP_ERR_NEED_PIN1 (APP_ERR_COMMON_SECTION_BEGIN + 18)
#define APP_ERR_NEED_PUK1 (APP_ERR_COMMON_SECTION_BEGIN + 19)
#define APP_ERR_USIM_SIM_INVALIDATION (APP_ERR_COMMON_SECTION_BEGIN + 20)
#define APP_ERR_UNSPECIFIED_ERROR (APP_ERR_COMMON_SECTION_BEGIN + 21)
#define APP_ERR_INSUFFICIENT_RESOURCES (APP_ERR_COMMON_SECTION_BEGIN + 22)

#define APP_ERR_CONN_OR_NO_IMSI (APP_ERR_COMMON_SECTION_BEGIN + 23)
#define APP_ERR_NOT_SUPPORT_NOW (APP_ERR_COMMON_SECTION_BEGIN + 24)
#define APP_ERR_NOT_STARTED_YET (APP_ERR_COMMON_SECTION_BEGIN + 25)

/* MM��APP������ */
/* Modified for PDP_CAUSE_RPT,2017-09-11,Begin */
#define APP_ERR_MM_NW_BEAR_NOT_SYNC (APP_ERR_MM_SECTION_BEGIN + 0)
#define APP_ERR_MM_REL_IND (APP_ERR_MM_SECTION_BEGIN + 1)
#define APP_ERR_MM_AUTH_TIME_OUT (APP_ERR_MM_SECTION_BEGIN + 2)
#define APP_ERR_MM_T3410_TIME_OUT (APP_ERR_MM_SECTION_BEGIN + 3)
#define APP_ERR_MM_LINK_ERR (APP_ERR_MM_SECTION_BEGIN + 4)
#define APP_ERR_MM_AUTH_FAIL (APP_ERR_MM_SECTION_BEGIN + 5)
#define APP_ERR_MM_THROT (APP_ERR_MM_SECTION_BEGIN + 6)
#define APP_ERR_MM_PLMN_SRCH_INTERRUPT (APP_ERR_MM_SECTION_BEGIN + 7)
#define APP_ERR_MM_SYS_INFO_INTERRUPT (APP_ERR_MM_SECTION_BEGIN + 8)
#define APP_ERR_MM_SUSPEND_INTERRUPT (APP_ERR_MM_SECTION_BEGIN + 9)
#define APP_ERR_MM_DETACH (APP_ERR_MM_SECTION_BEGIN + 10)
#define APP_ERR_MM_LRRC_ERR_IND (APP_ERR_MM_SECTION_BEGIN + 11)
#define APP_ERR_MM_IMSI_PAGING_DETACH (APP_ERR_MM_SECTION_BEGIN + 12)
#define APP_ERR_MM_ATTACH_EST_FAIL (APP_ERR_MM_SECTION_BEGIN + 13)
#define APP_ERR_MM_STATUS_CANT_TRIGGER (APP_ERR_MM_SECTION_BEGIN + 14)
#define APP_ERR_MM_POWER_OFF (APP_ERR_MM_SECTION_BEGIN + 15)
#define APP_ERR_MM_AREA_LOST (APP_ERR_MM_SECTION_BEGIN + 16)
#define APP_ERR_MM_SYS_CFG_OR_UE_CAP_CHG (APP_ERR_MM_SECTION_BEGIN + 17)
#define APP_ERR_MM_GU_ATTACH_FAIL (APP_ERR_MM_SECTION_BEGIN + 18)
#define APP_ERR_MM_GU_DETACH (APP_ERR_MM_SECTION_BEGIN + 19)
#define APP_ERR_MM_ATTACH_MMC_REL (APP_ERR_MM_SECTION_BEGIN + 20)
#define APP_ERR_MM_ATTACH_CS_CALL_INTERRUPT (APP_ERR_MM_SECTION_BEGIN + 21)
#define APP_ERR_MM_USIM_PULL_OUT (APP_ERR_MM_SECTION_BEGIN + 22)
#define APP_ERR_MM_NW_NOT_SUPPORT_EMC_BEARER (APP_ERR_SM_SECTION_BEGIN + 23)

#define APP_ERR_MM_UNKNOWN (APP_ERR_MM_SECTION_BEGIN + 199)

/* MM����������� */
/* Э�鶨�������ܾ�ԭ��ֵ */

#define APP_ERR_MM_NW_REJ_NULL (APP_ERR_MM_NW_SECTION_BEGIN + 0)
#define APP_ERR_MM_NW_REJ_IMSI_UNKNOWN_IN_HSS (APP_ERR_MM_NW_SECTION_BEGIN + 2)
#define APP_ERR_MM_NW_REJ_ILLEGAL_UE (APP_ERR_MM_NW_SECTION_BEGIN + 3)
#define APP_ERR_MM_NW_REJ_IMEI_NOT_ACCEPTED (APP_ERR_MM_NW_SECTION_BEGIN + 5)
#define APP_ERR_MM_NW_REJ_ILLEGAL_ME (APP_ERR_MM_NW_SECTION_BEGIN + 6)
#define APP_ERR_MM_NW_REJ_EPS_SERV_NOT_ALLOW (APP_ERR_MM_NW_SECTION_BEGIN + 7)
#define APP_ERR_MM_NW_REJ_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW (APP_ERR_MM_NW_SECTION_BEGIN + 8)
#define APP_ERR_MM_NW_REJ_UE_ID_NOT_DERIVED (APP_ERR_MM_NW_SECTION_BEGIN + 9)
#define APP_ERR_MM_NW_REJ_IMPLICIT_DETACHED (APP_ERR_MM_NW_SECTION_BEGIN + 10)
#define APP_ERR_MM_NW_REJ_PLMN_NOT_ALLOW (APP_ERR_MM_NW_SECTION_BEGIN + 11)
#define APP_ERR_MM_NW_REJ_TA_NOT_ALLOW (APP_ERR_MM_NW_SECTION_BEGIN + 12)
#define APP_ERR_MM_NW_REJ_ROAM_NOT_ALLOW (APP_ERR_MM_NW_SECTION_BEGIN + 13)
#define APP_ERR_MM_NW_REJ_EPS_SERV_NOT_ALLOW_IN_PLMN (APP_ERR_MM_NW_SECTION_BEGIN + 14)
#define APP_ERR_MM_NW_REJ_NO_SUITABL_CELL (APP_ERR_MM_NW_SECTION_BEGIN + 15)
#define APP_ERR_MM_NW_REJ_MSC_UNREACHABLE (APP_ERR_MM_NW_SECTION_BEGIN + 16)
#define APP_ERR_MM_NW_REJ_NETWORK_FAILURE (APP_ERR_MM_NW_SECTION_BEGIN + 17)
#define APP_ERR_MM_NW_REJ_CS_NOT_AVAIL (APP_ERR_MM_NW_SECTION_BEGIN + 18)
#define APP_ERR_MM_NW_REJ_ESM_FAILURE (APP_ERR_MM_NW_SECTION_BEGIN + 19)
#define APP_ERR_MM_NW_REJ_MAC_FAILURE (APP_ERR_MM_NW_SECTION_BEGIN + 20)
#define APP_ERR_MM_NW_REJ_SYNCH_FAILURE (APP_ERR_MM_NW_SECTION_BEGIN + 21)
#define APP_ERR_MM_NW_REJ_PROCEDURE_CONGESTION (APP_ERR_MM_NW_SECTION_BEGIN + 22)
#define APP_ERR_MM_NW_REJ_UE_SECU_CAP_MISMATCH (APP_ERR_MM_NW_SECTION_BEGIN + 23)
#define APP_ERR_MM_NW_REJ_SECU_MODE_REJECTED_UNSPECIFIED (APP_ERR_MM_NW_SECTION_BEGIN + 24)
#define APP_ERR_MM_NW_REJ_NOT_AUTHORIZED_FOR_THIS_CSG (APP_ERR_MM_NW_SECTION_BEGIN + 25)
#define APP_ERR_MM_NW_REJ_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN (APP_ERR_MM_NW_SECTION_BEGIN + 35)
#define APP_ERR_MM_NW_REJ_CS_FALLBACK_CALL_EST_NOT_ALLOWED (APP_ERR_MM_NW_SECTION_BEGIN + 38)
#define APP_ERR_MM_NW_REJ_CS_DOMAIN_TMP_NOT_ALLOWED (APP_ERR_MM_NW_SECTION_BEGIN + 39)
#define APP_ERR_MM_NW_REJ_NO_EPS_BEARER_CONTEXT_ACTIVATED (APP_ERR_MM_NW_SECTION_BEGIN + 40)
#define APP_ERR_MM_NW_REJ_SERVER_NETWORK_FAILURE (APP_ERR_MM_NW_SECTION_BEGIN + 42)
#define APP_ERR_MM_NW_REJ_SEMANTICALLY_INCORRECT_MSG (APP_ERR_MM_NW_SECTION_BEGIN + 95)
#define APP_ERR_MM_NW_REJ_INVALID_MANDATORY_INF (APP_ERR_MM_NW_SECTION_BEGIN + 96)
#define APP_ERR_MM_NW_REJ_MSG_NONEXIST_NOTIMPLEMENTE (APP_ERR_MM_NW_SECTION_BEGIN + 97)
#define APP_ERR_MM_NW_REJ_MSG_TYPE_NOT_COMPATIBLE (APP_ERR_MM_NW_SECTION_BEGIN + 98)
#define APP_ERR_MM_NW_REJ_IE_NONEXIST_NOTIMPLEMENTED (APP_ERR_MM_NW_SECTION_BEGIN + 99)
#define APP_ERR_MM_NW_REJ_CONDITIONAL_IE_ERROR (APP_ERR_MM_NW_SECTION_BEGIN + 100)
#define APP_ERR_MM_NW_REJ_MSG_NOT_COMPATIBLE (APP_ERR_MM_NW_SECTION_BEGIN + 101)
#define APP_ERR_MM_NW_REJ_PROTOCOL_ERROR (APP_ERR_MM_NW_SECTION_BEGIN + 111)
#define APP_ERR_MM_NW_REJ_OTHERS (APP_ERR_MM_NW_SECTION_BEGIN + 255)
/* Modified for PDP_CAUSE_RPT,2017-09-11,End */

/* SM��APP������ */
#define APP_ERR_SM_CALL_CID_INVALID (APP_ERR_SM_SECTION_BEGIN + 0)
#define APP_ERR_SM_CALL_CID_ACTIVE (APP_ERR_SM_SECTION_BEGIN + 1)
#define APP_ERR_SM_CALL_MOD_CID_NOT_MATCH (APP_ERR_SM_SECTION_BEGIN + 2)
#define APP_ERR_SM_CALL_CID_NOT_MATCH_BEARER (APP_ERR_SM_SECTION_BEGIN + 3)
#define APP_ERR_SM_BEARER_TYPE_NOT_DEDICATED (APP_ERR_SM_SECTION_BEGIN + 4)
#define APP_ERR_SM_NO_INCOMING_CALL (APP_ERR_SM_SECTION_BEGIN + 5)
#define APP_ERR_SM_LLC_OR_SNDCP_FAIL (APP_ERR_SM_SECTION_BEGIN + 6)
#define APP_ERR_SM_REACTIV_REQ (APP_ERR_SM_SECTION_BEGIN + 7)
#define APP_ERR_SM_FEATURE_NOT_SUPPORT (APP_ERR_SM_SECTION_BEGIN + 8)
#define APP_ERR_SM_GPRS_ATTACH_FAIL (APP_ERR_SM_SECTION_BEGIN + 9)
#define APP_ERR_SM_OTHER_BEARER_CONT_EXIST (APP_ERR_SM_SECTION_BEGIN + 10)
#define APP_ERR_SM_NSAPI_IN_USE (APP_ERR_SM_SECTION_BEGIN + 11)
#define APP_ERR_SM_COLLISION_WITH_NW (APP_ERR_SM_SECTION_BEGIN + 12)
#define APP_ERR_SM_BEARER_INACTIVE (APP_ERR_SM_SECTION_BEGIN + 13)
#define APP_ERR_SM_DETACHED (APP_ERR_SM_SECTION_BEGIN + 14)
#define APP_ERR_SM_LINK_CID_INVALID (APP_ERR_SM_SECTION_BEGIN + 15)
#define APP_ERR_SM_LINK_BEARER_INACTIVE (APP_ERR_SM_SECTION_BEGIN + 16)
#define APP_ERR_SM_APN_LEN_ILLEGAL (APP_ERR_SM_SECTION_BEGIN + 17)
#define APP_ERR_SM_APN_SYNTACTICAL_ERROR (APP_ERR_SM_SECTION_BEGIN + 18)
#define APP_ERR_SM_SET_APN_BEFORE_SET_AUTH (APP_ERR_SM_SECTION_BEGIN + 19)
#define APP_ERR_SM_AUTH_TYPE_ILLEGAL (APP_ERR_SM_SECTION_BEGIN + 20)
#define APP_ERR_SM_USER_NAME_TOO_LONG (APP_ERR_SM_SECTION_BEGIN + 21)
#define APP_ERR_SM_USER_PASSWORD_TOO_LONG (APP_ERR_SM_SECTION_BEGIN + 22)
#define APP_ERR_SM_ACCESS_NUM_TOO_LONG (APP_ERR_SM_SECTION_BEGIN + 23)
#define APP_ERR_SM_CALL_CID_IN_OPERATION (APP_ERR_SM_SECTION_BEGIN + 24)
#define APP_ERR_SM_BEARER_TYPE_NOT_DEFAULT (APP_ERR_SM_SECTION_BEGIN + 25)
#define APP_ERR_SM_BEARER_TYPE_ILLEGAL (APP_ERR_SM_SECTION_BEGIN + 26)
#define APP_ERR_SM_MUST_EXIST_DEFAULT_TYPE_CID (APP_ERR_SM_SECTION_BEGIN + 27)
#define APP_ERR_SM_PDN_TYPE_ILLEGAL (APP_ERR_SM_SECTION_BEGIN + 28)
#define APP_ERR_SM_IPV4_ADDR_ALLOC_TYPE_ILLEGAL (APP_ERR_SM_SECTION_BEGIN + 29)
#define APP_ERR_SM_SUSPENDED (APP_ERR_SM_SECTION_BEGIN + 30)
#define APP_ERR_SM_MULTI_EMERGENCY_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 31)
#define APP_ERR_SM_NON_EMERGENCY_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 32)
#define APP_ERR_SM_MODIFY_EMERGENCY_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 33)
#define APP_ERR_SM_DEDICATED_FOR_EMERGENCY_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 34)
#define APP_ERR_SM_ONLY_EMERGENCY_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 35)

/* mod for program 2015-01-15 begin */
#define APP_ERR_SM_BACKOFF_ALG_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 36)
/* mod for program 2015-01-15 end */

#define APP_ERR_SM_THROT_ALG_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 37)

/* add for provide PDP_NOT_ALLOWED 2016-05-20 begin */
#define APP_ERR_SM_BACKOFF_ALG_NOT_ALLOWED_IGNORE (APP_ERR_SM_SECTION_BEGIN + 38)
#define APP_ERR_SM_BACKOFF_ALG_NOT_ALLOWED_PERM (APP_ERR_SM_SECTION_BEGIN + 39)
#define APP_ERR_SM_BACKOFF_ALG_NOT_ALLOWED_TEMP (APP_ERR_SM_SECTION_BEGIN + 40)
#define APP_ERR_SM_BACKOFF_ALG_NOT_ALLOWED_RSV (APP_ERR_SM_SECTION_BEGIN + 41)
/* add for provide PDP_NOT_ALLOWED 2016-05-20 end */
#define APP_ERR_SM_THROT_T3396_IS_RUNNING (APP_ERR_SM_SECTION_BEGIN + 42)
#define APP_ERR_SM_NAS_SIG_LOW_PRIORITY_NOT_SUPPROT (APP_ERR_SM_SECTION_BEGIN + 43)

/* Added for Boston_R13_CR_PHASEI, 2016-10-17, Begin */
#define APP_ERR_SM_NAS_MO_SIG_ACCESS_BAR (APP_ERR_SM_SECTION_BEGIN + 44)
/* Added for Boston_R13_CR_PHASEI, 2016-10-17, End */
/* Added for Boston_R13_CR_PHASEII, 2016-12-01, begin */
#define APP_ERR_SM_MAX_EPS_NUM_REACHED (APP_ERR_SM_SECTION_BEGIN + 45)
/* Added for Boston_R13_CR_PHASEII, 2016-12-01, end */
/* Added for Boston_R13_CR_PHASEIII, 2017-01-16, Begin */
#define APP_ERR_SM_THROT_BACKOFF_IS_RUNNING (APP_ERR_SM_SECTION_BEGIN + 46)
/* Added for Boston_R13_CR_PHASEIII, 2017-01-16, End */
/* Added for PDP_CAUSE_RPT,2017-09-11,Begin */
#define APP_ERR_SM_MM_NOT_SYNC (APP_ERR_SM_SECTION_BEGIN + 47)
#define APP_ERR_SM_NW_RECONFIGURE (APP_ERR_SM_SECTION_BEGIN + 48)
#define APP_ERR_SM_DELETE_SDF (APP_ERR_SM_SECTION_BEGIN + 49)
/* Added for PDP_CAUSE_RPT,2017-09-11,End */
#define APP_ERR_SM_NOT_IN_ACL_LIST (APP_ERR_SM_SECTION_BEGIN + 50)
#define APP_ERR_SM_APN_DISABLE (APP_ERR_SM_SECTION_BEGIN + 51)
#define APP_ERR_SM_IMS_DEREG_NOT_ATTACH (APP_ERR_SM_SECTION_BEGIN + 52)
#define APP_ERR_SM_BEARER_ALLOC_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 53)

#define APP_ERR_SM_BEARER_MOD_NOT_ALLOWED (APP_ERR_SM_SECTION_BEGIN + 54)

/* added 2019-03-01,started */
#define APP_ERR_SM_NR_QOS_FLOW_MAP_FAIL (APP_ERR_SM_SECTION_BEGIN + 55)
/* added 2019-03-01,end */

#define APP_ERR_SM_REDIAL (APP_ERR_SM_SECTION_BEGIN + 56)
/* added 2020-04-13,started */
#define APP_ERR_SM_TIME_OUT_WITH_SEND_FAILED (APP_ERR_SM_SECTION_BEGIN + 57)
/* added 2020-04-13,end */


/* SM����������� */
#define APP_ERR_SM_NW_OPERATOR_DETERMINED_BARRING (APP_ERR_SM_NW_SECTION_BEGIN + 0)
#define APP_ERR_SM_NW_INSUFFICIENT_RESOURCES (APP_ERR_SM_NW_SECTION_BEGIN + 1)
#define APP_ERR_SM_NW_MISSING_OR_UKNOWN_APN (APP_ERR_SM_NW_SECTION_BEGIN + 2)
#define APP_ERR_SM_NW_UNKNOWN_PDN_TYPE (APP_ERR_SM_NW_SECTION_BEGIN + 3)
#define APP_ERR_SM_NW_USER_AUTH_FAIL (APP_ERR_SM_NW_SECTION_BEGIN + 4)
#define APP_ERR_SM_NW_REQ_REJ_BY_SGW_OR_PGW (APP_ERR_SM_NW_SECTION_BEGIN + 5)
#define APP_ERR_SM_NW_REQ_REJ_UNSPECITY (APP_ERR_SM_NW_SECTION_BEGIN + 6)
#define APP_ERR_SM_NW_SERVICE_OPTION_NOT_SUPPORT (APP_ERR_SM_NW_SECTION_BEGIN + 7)
#define APP_ERR_SM_NW_REQ_SERVICE_NOT_SUBSCRIBE (APP_ERR_SM_NW_SECTION_BEGIN + 8)
#define APP_ERR_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER (APP_ERR_SM_NW_SECTION_BEGIN + 9)
#define APP_ERR_SM_NW_PTI_ALREADY_IN_USE (APP_ERR_SM_NW_SECTION_BEGIN + 10)
#define APP_ERR_SM_NW_REGULAR_DEACT (APP_ERR_SM_NW_SECTION_BEGIN + 11)
#define APP_ERR_SM_NW_EPS_QOS_NOT_ACCEPT (APP_ERR_SM_NW_SECTION_BEGIN + 12)
#define APP_ERR_SM_NW_NET_FAIL (APP_ERR_SM_NW_SECTION_BEGIN + 13)
#define APP_ERR_SM_NW_SEMANTIC_ERR_IN_TFT (APP_ERR_SM_NW_SECTION_BEGIN + 14)
#define APP_ERR_SM_NW_SYNTACTIC_ERR_IN_TFT (APP_ERR_SM_NW_SECTION_BEGIN + 15)
#define APP_ERR_SM_NW_INVALID_EPS_BERER_IDENTITY (APP_ERR_SM_NW_SECTION_BEGIN + 16)
#define APP_ERR_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER (APP_ERR_SM_NW_SECTION_BEGIN + 17)
#define APP_ERR_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER (APP_ERR_SM_NW_SECTION_BEGIN + 18)
#define APP_ERR_SM_NW_BEARER_WITHOUT_TFT_ACT (APP_ERR_SM_NW_SECTION_BEGIN + 19)
#define APP_ERR_SM_NW_PTI_MISMATICH (APP_ERR_SM_NW_SECTION_BEGIN + 20)
#define APP_ERR_SM_NW_LAST_PDN_DISCONN_NOT_ALLOWED (APP_ERR_SM_NW_SECTION_BEGIN + 21)
#define APP_ERR_SM_NW_PDN_TPYE_IPV4_ONLY_ALLOWED (APP_ERR_SM_NW_SECTION_BEGIN + 22)
#define APP_ERR_SM_NW_PDN_TPYE_IPV6_ONLY_ALLOWED (APP_ERR_SM_NW_SECTION_BEGIN + 23)
#define APP_ERR_SM_NW_SINGLE_ADDR_BERERS_ONLY_ALLOWED (APP_ERR_SM_NW_SECTION_BEGIN + 24)
#define APP_ERR_SM_NW_ESM_INFO_NOT_RECEIVED (APP_ERR_SM_NW_SECTION_BEGIN + 25)
#define APP_ERR_SM_NW_PDN_CONN_NOT_EXIST (APP_ERR_SM_NW_SECTION_BEGIN + 26)
#define APP_ERR_SM_NW_MULTI_PDN_CONN_FOR_ONE_APN_NOT_ALLOWED (APP_ERR_SM_NW_SECTION_BEGIN + 27)
#define APP_ERR_SM_NW_COLLISION_WITH_NW_INTIAL_REQEST (APP_ERR_SM_NW_SECTION_BEGIN + 28)
#define APP_ERR_SM_NW_INVALID_PTI_VALUE (APP_ERR_SM_NW_SECTION_BEGIN + 29)
#define APP_ERR_SM_NW_SYNCTACTIC_INCORRECT_MSG (APP_ERR_SM_NW_SECTION_BEGIN + 30)
#define APP_ERR_SM_NW_INVALID_MANDATORY_INFOR (APP_ERR_SM_NW_SECTION_BEGIN + 31)
#define APP_ERR_SM_NW_MSG_TYPE_NON_EXIST (APP_ERR_SM_NW_SECTION_BEGIN + 32)
#define APP_ERR_SM_NW_MSG_TYPE_NOT_COMPATIBLE_WITH_PROT (APP_ERR_SM_NW_SECTION_BEGIN + 33)
#define APP_ERR_SM_NW_INFOR_ELEM_NON_EXIST (APP_ERR_SM_NW_SECTION_BEGIN + 34)
#define APP_ERR_SM_NW_CONDITIONAL_IE_ERROR (APP_ERR_SM_NW_SECTION_BEGIN + 35)
#define APP_ERR_SM_NW_MSG_NOT_COMPATIBLE_WITH_PROT (APP_ERR_SM_NW_SECTION_BEGIN + 36)
#define APP_ERR_SM_NW_PROT_ERR_UNSPECIFIED (APP_ERR_SM_NW_SECTION_BEGIN + 37)
#define APP_ERR_SM_NW_APN_RESTRICTION_INCOMPATIBLE_WITH_ACT_EPS_BEARER (APP_ERR_SM_NW_SECTION_BEGIN + 38)
#define APP_ERR_SM_NW_UNSUPPORTED_QCI_VALUE (APP_ERR_SM_NW_SECTION_BEGIN + 39)
#define APP_ERR_SM_NW_APN_NOT_SUPPORT_IN_CURRENT_RAT_AND_PLMN (APP_ERR_SM_NW_SECTION_BEGIN + 40)
#define APP_ERR_SM_NW_REACTIVATION_REQUESTED (APP_ERR_SM_NW_SECTION_BEGIN + 41)
/* Added 2016-10-31, begin */
#define APP_ERR_SM_NW_BEARER_HANDLING_NOT_SUPPORTED (APP_ERR_SM_NW_SECTION_BEGIN + 42)
#define APP_ERR_SM_NW_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED (APP_ERR_SM_NW_SECTION_BEGIN + 43)
/* Added 2016-10-31, end */

/* EHSM��ESM �������ԭ��ֵ */
/* Added for HO ATTACH, 2018-12-20 begin */
#define APP_ERR_EHSM_ESM_SUCCESS (APP_ERR_EHSM_ESM_SECTION_BEGIN + 0)
#define APP_ERR_EHSM_ESM_L2C_HANDOVER_FAIL (APP_ERR_EHSM_ESM_SECTION_BEGIN + 1)
#define APP_ERR_EHSM_ESM_ATTACH_FAIL (APP_ERR_EHSM_ESM_SECTION_BEGIN + 2)
#define APP_ERR_EHSM_ESM_POWER_OFF (APP_ERR_EHSM_ESM_SECTION_BEGIN + 3)
#define APP_ERR_EHSM_ESM_PS_DETACH (APP_ERR_EHSM_ESM_SECTION_BEGIN + 4)
#define APP_ERR_EHSM_ESM_BEARER_SYNC_ERROR (APP_ERR_EHSM_ESM_SECTION_BEGIN + 5)
#define APP_ERR_EHSM_ESM_LOCAL_DEACT (APP_ERR_EHSM_ESM_SECTION_BEGIN + 6)
#define APP_ERR_EHSM_ESM_PPPC_ACTED_PDN_LESS_THAN_EHSM (APP_ERR_EHSM_ESM_SECTION_BEGIN + 7)
#define APP_ERR_EHSM_ESM_NOT_SUPPORT_EHRPD (APP_ERR_EHSM_ESM_SECTION_BEGIN + 8)
#define APP_ERR_EHSM_ESM_PPPC_EXIST_NOT_OPEN_PDN (APP_ERR_EHSM_ESM_SECTION_BEGIN + 9)

/* Added for HO ATTACH, 2018-12-20 end */

#define NAS_MM_MAX_TA_NUM 64 /* TA �б������� */
#define NAS_MM_MAX_LA_NUM 64 /* LA �б������� */

#define NAS_MM_MAX_PLMN_NUM 64
#define NAS_MM_MAX_UEID_BUF_SIZE 10 /* UNIT: BYTE  */
#define NAS_MM_MAX_MSID_SIZE 10

#define NAS_MM_PLMN_AND_ACT_LEN 5

#define NAS_MM_SERVICE_TABLE_LEN 12

#define NAS_MM_MAX_PLMN_AND_ACT_LEN (NAS_MM_PLMN_AND_ACT_LEN * NAS_MM_MAX_PLMN_NUM)
#define NAS_MM_LRPLMNSI_LEN 1

#define NAS_MM_AUTH_KEY_ASME_LEN 32
#define NAS_MM_AUTH_CKEY_LEN 16
#define NAS_MM_AUTH_IKEY_LEN 16
#define NAS_MM_AUTH_KEY_NAS_ENC_LEN 16
#define NAS_MM_AUTH_KEY_NAS_INT_LEN 16

#define NAS_MM_MIN_UE_NET_CAP_LEN 2
#define NAS_MM_MAX_UE_NET_CAP_LEN 13 /* UE����������󳤶�Ϊ13BYTE */
#define NAS_MM_MAX_MS_NET_CAP_LEN 8  /* UNIT: BYTE  */

#define NAS_MMC_MAX_UPLMN_NUM NAS_MM_MAX_PLMN_NUM /* USIM����UPLMN������ */
#define NAS_MMC_MAX_OPLMN_NUM NAS_MM_MAX_PLMN_NUM /* USIM����OPLMN������ */

#define NAS_EMM_USIM_SEQ_UINT32_LEN 2

/* SQN verify */
#define NAS_EMM_USIM_SEQ_ARRAY_LEN 32
#define NAS_EMM_USIM_SQN_LEN 6

#define NAS_EMM_SOFT_USIM_KEY_LEN 16
#define NAS_EMM_SOFT_USIM_OP_LEN 16

#define MAX_SUPPORTED_CODEC_NUM 10

/* moded for nw-cause adaption, 2017-09-22, begin */
#define NAS_LMM_MAX_CAUSE_NUM 20
/* moded for nw-cause adaption, 2017-09-22, end */

/* mod for program 2015-01-04 begin */
#define NAS_MM_MAX_PDP_REJ_CAUSE_NUM 16
#define NAS_MM_DAM_MAX_PLMN_NUM 16

#define NAS_LMM_DAM_NV_CSERVICE_MAX_VALUE 60
#define NAS_LMM_DAM_NV_CSERVICE_MIN_VALUE 1
#define NAS_LMM_DAM_CSERVICE_DEFAULT_VALUE 5

/* mod for program 2015-01-04 end */

/* mod for program phaseIII 2015-03-15 begin */
#define NAS_TMO_IMSI_HPLMN_MAX_NUM 8
/* mod for program phaseIII 2015-03-15 end */
/* Add for LTE CSG,2015-09-24, Begin */
#define NAS_MM_MAX_HOME_NODEB_NAME_LEN 48
/* Add for LTE CSG,2015-09-24, End */
/* Added by for 2019_06 cr refresh throt related, 2019-7-11, begin */
#define NAS_THROT_EPLMN_MAX_NUM        16
/* Added by for 2019_06 cr refresh throt related, 2019-7-11, begin */
#define NAS_MM_MAX_T3402_PLMN_NUM      16

/* Added for DSDS OPTIMIZE MT DETACH BY TAU, 2016-06-16, Begin */
#define NAS_MT_DETACH_TAU_PLMN_MAX_NUM 6
/* Added for DSDS OPTIMIZE MT DETACH BY TAU, 2016-06-16, End */

#define NAS_MM_MAX_T3396_PLMN_NUM      7
/* Added for Band66 support ,2017-04-20,begin */
#define NAS_LMM_BAND_MAX_LENTH         8
/* Added for Band66 support ,2017-04-20,end */

/*
 * ö��˵��: ���뼼����ȡֵ
 */
enum NAS_MM_RatType {
    NAS_MM_RAT_WCDMA    = 0,
    NAS_MM_RAT_GSM_GPRS = 1,
    NAS_MM_RAT_LTE_FDD  = 2,
    NAS_MM_RAT_LTE_TDD  = 3,
    NAS_MM_RAT_TYPE_BUTT
};
typedef VOS_UINT32           NAS_MM_RatTypeUint32;
typedef NAS_MM_RatTypeUint32 NAS_EMM_RatTypeUint32;
typedef NAS_MM_RatTypeUint32 NAS_MMC_RatTypeUint32;

/* ö��˵��: ѡ��ģʽ */
enum NAS_MM_SelMode {
    NAS_MM_PLMN_SEL_AUTO   = 0x00,                    /* �Զ�����ѡ��ģʽ */
    NAS_MM_PLMN_SEL_MANUAL = 0x01,                    /* �ֶ�����ѡ��ģʽ */
    /* NAS_MM_PLMN_SEL_NO_IMSI             = 0x02, */ /* ����Чģʽ */
    NAS_MM_PLMN_SEL_BUTT
};
typedef VOS_UINT32 NAS_MM_SelModeUint32;

/* ö��˵��: EMM UPDATE״̬ */
enum NAS_MM_UpdateState {
    NAS_MM_US_EU1_UPDATED             = 0x00, /* ����״̬       */
    NAS_MM_US_EU2_NOT_UPDATED         = 0x01, /* δ����״̬     */
    NAS_MM_US_EU3_ROAMING_NOT_ALLOWED = 0x02, /* ���β�����״̬ */

    NAS_MM_US_BUTT
};
typedef VOS_UINT32 NAS_MM_UpdateStateUint32;

/* �ṹ˵��: ��USIM���AUTHʹ�õ��㷨 */
enum NAS_EMM_SoftUsimAuthAlg {
    NAS_EMM_SOFT_USIM_ALG_MILLENGE = 0, /* Millenge�㷨 */
    NAS_EMM_SOFT_USIM_ALG_TEST     = 1, /* Test�㷨 */

    NAS_EMM_SOFT_USIM_ALG_BUTT
};
typedef VOS_UINT32 NAS_EMM_SoftUsimAuthAlgUint32;
/* =======================LAST RPLMN SELECTION INDICATION============================ */
enum NAS_MM_Lrplmnsi {
    NAS_MM_LRPLMNSI_RPLMN = 0x00,
    NAS_MM_LRPLMNSI_HPLMN,
    NAS_MM_LRPLMNSI_BUTT
};
typedef VOS_UINT8 NAS_MM_LrplmnsiUint8;

/* �ṹ˵��: UE�Ĳ���ģʽ */
enum NAS_LMM_UeOperationMode {
    NAS_LMM_UE_PS_MODE_1    = 1, /* PS MODE 1 */
    NAS_LMM_UE_PS_MODE_2    = 2, /* PS MODE 2 */
    NAS_LMM_UE_CS_PS_MODE_1 = 3, /* CS/PS MODE 1 */
    NAS_LMM_UE_CS_PS_MODE_2 = 4, /* CS/PS MODE 2 */

    NAS_LMM_UE_MODE_BUTT
};
typedef VOS_UINT32 NAS_LMM_UeOperationModeUint32;

/* �ṹ˵��: UE���ŵ�CSҵ������ */
enum NAS_LMM_CsService {
    NAS_LMM_CS_SERVICE_CSFB_SMS = 1, /* CSFB AND SMS */
    NAS_LMM_CS_SERVICE_SMS_ONLY = 2, /* SMS ONLY */
    NAS_LMM_CS_SERVICE_1xCSFB   = 3, /* 1xCSFB */

    NAS_LMM_CS_SERVICE_BUTT
};
typedef VOS_UINT32 NAS_LMM_CsServiceUint32;

/* for ue mode begin */
/* �ṹ˵��: GU��UE����ģʽ */
enum NAS_LMM_GuUeMode {
    NAS_LMM_GU_UE_MODE_PS    = 1,
    NAS_LMM_GU_UE_MODE_CS    = 2,
    NAS_LMM_GU_UE_MODE_CS_PS = 3,

    NAS_LMM_GU_UE_MODE_BUTT
};
typedef VOS_UINT32 NAS_LMM_GuUeModeUint32;

/* �ṹ˵��: DISABLE LTE��ԭ�� */
enum NAS_LMM_DisableLteCause {
    NAS_LMM_DISABLE_LTE_CAUSE_NULL = 1,

    NAS_LMM_DISABLE_LTE_CAUSE_BUTT
};
typedef VOS_UINT32 NAS_LMM_DisableLteCauseUint32;

/* �ṹ˵��: �Ƿ�֧��ĳ���� */
enum NAS_LMM_Sup {
    NAS_LMM_NOT_SUP = 0, /* �� NOT AVAILABLE */
    NAS_LMM_SUP     = 1, /* �� AVAILABLE */

    NAS_LMM_SUP_BUTT
};
typedef VOS_UINT32 NAS_LMM_SupUint32;

/* �ṹ˵��: UE֧�ֵ�Voice Domain���� */
enum NAS_LMM_SupVoiceDomain {
    NAS_LMM_SUP_VOICE_DOMAIN_CS  = 0,
    NAS_LMM_SUP_VOICE_DOMAIN_IMS = 1,

    NAS_LMM_SUP_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32 NAS_LMM_SupVoiceDomainUint32;

/* �ṹ˵��: UE֧�ֵ�SMS Domain���� */
enum NAS_LMM_SupSmsDomain {
    NAS_LMM_SUP_SMS_DOMAIN_SGS = 0,
    NAS_LMM_SUP_SMS_DOMAIN_IP  = 1,

    NAS_LMM_SUP_SMS_DOMAIN_BUTT
};
typedef VOS_UINT32 NAS_LMM_SupSmsDomainUint32;

/* �ṹ˵��: */
enum NAS_EMM_UsageSetting {
    EMM_SETTING_VOICE_CENTRIC = 0x00, /* �������� */
    EMM_SETTING_DATA_CENTRIC,

    EMM_USAGE_SETTING_BUTT
};

typedef VOS_UINT32 NAS_EMM_UsageSettingUint32;

typedef NAS_EMM_UsageSettingUint32 LNAS_LMM_UeCenterUint32;
/* CHANGE for ue mode */

/* �ṹ˵��: UE��Voice Domain���� */
enum NAS_LMM_VoiceDomain {
    NAS_LMM_VOICE_DOMAIN_CS_ONLY          = 0, /* CS voice only */
    NAS_LMM_VOICE_DOMAIN_IMS_PS_ONLY      = 1, /* IMS PS voice only */
    NAS_LMM_VOICE_DOMAIN_CS_PREFERRED     = 2, /* CS voice preferred, IMS PS Voice as secondary */
    NAS_LMM_VOICE_DOMAIN_IMS_PS_PREFERRED = 3, /* IMS PS voice preferred, CS Voice as secondary */

    NAS_LMM_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32 NAS_LMM_VoiceDomainUint32;

/* �ṹ˵��: */
enum NAS_ESM_BearerPrio {
    NAS_ESM_BEARER_PRIO_NORMAL = 0, /* CS voice only */
    NAS_ESM_BEARER_PRIO_LOW    = 1, /* IMS PS voice only */

    NAS_ESM_BEARER_PRIO_BUTT
};
typedef VOS_UINT32 NAS_ESM_BearerPrioUint32;

/* Add  for LTE CSG, 2015-10-23, Begin */
enum NAS_MM_CellType {
    NAS_MM_CELL_TYPE_MACRO  = 0, /* С�������Ǻ�С�� */
    NAS_MM_CELL_TYPE_HYBRID = 1, /* С������Ϊ���С�� */
    NAS_MM_CELL_TYPE_CSG    = 2, /* С������ΪCSGС�� */
    NAS_MM_CELL_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MM_CellTypeUint8;
/* Add  for LTE CSG, 2015-10-23, End */
/* Added  for GU_BACK_OFF, 2016-04-07, Begin */
enum NAS_BACKOFF_RatSupport {
    NAS_BACKOFF_RAT_SUPPORT_ALL  = 0, /* ֧��GUL */
    NAS_BACKOFF_RAT_SUPPORT_LTE  = 1, /* ֧��LTE */
    NAS_BACKOFF_RAT_SUPPORT_GU   = 2, /* ֧��GU */
    NAS_BACKOFF_RAT_SUPPORT_NONE = 3, /* GUL����֧�� */
    NAS_BACKOFF_RAT_SUPPORT_BUTT
};
typedef VOS_UINT32 NAS_BACKOFF_RatSupportUint32;
/* Added  for GU_BACK_OFFCSG, 2016-04-07, End */

/*
 * ʹ��˵��:
 *     MCC, Mobile country code (aucPlmnId[0], aucPlmnId[1] bits 1 to 4)
 *     MNC, Mobile network code (aucPlmnId[2], aucPlmnId[1] bits 5 to 8).
 *     The coding of this field is the responsibility of each administration but BCD
 *     coding shall be used. The MNC shall consist of 2 or 3 digits. For PCS 1900 for NA,
 *     Federal regulation mandates that a 3-digit MNC shall be used. However a network
 *     operator may decide to use only two digits in the MNC over the radio interface.
 *     In this case, bits 5 to 8 of octet 4 shall be coded as "1111". Mobile equipment
 *     shall accept MNC coded in such a way.
 *     ---------------------------------------------------------------------------
 *                  ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[0] ||    MCC digit 2            |           MCC digit 1
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[1] ||    MNC digit 3            |           MCC digit 3
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[2] ||    MNC digit 2            |           MNC digit 1
 *     ---------------------------------------------------------------------------
 *     AT���
 *     at+cops=1,2,"mcc digit 1, mcc digit 2, mcc digit 3, mnc digit 1, mnc digit 2, mnc
 * digit 3",2:
 *     e.g.
 *     at+cops=1,2,"789456",2:
 *     --------------------------------------------------------------------------------
 *     (mcc digit 1)|(mcc digit 2)|(mcc digit 3)|(mnc digit 1)|(mnc digit 2)|(mnc digit 3)
 *     --------------------------------------------------------------------------------
 *        7         |     8       |      9      |     4       |      5      |     6
 *     --------------------------------------------------------------------------------
 *     ��aucPlmnId[3]�еĴ�Ÿ�ʽ:
 *     ---------------------------------------------------------------------------
 *                  ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[0] ||    MCC digit 2 = 8        |           MCC digit 1 = 7
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[1] ||    MNC digit 3 = 6        |           MCC digit 3 = 9
 *     ---------------------------------------------------------------------------
 *     aucPlmnId[2] ||    MNC digit 2 = 5        |           MNC digit 1 = 4
 *     ---------------------------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 plmnId[3]; /* Plmn ID���洢��3�ֽ� */
    VOS_UINT8 rsv;       /* ������ */
} NAS_MM_PlmnId;
typedef NAS_MM_PlmnId NAS_EMM_PlmnId;
typedef NAS_MM_PlmnId NAS_MMC_PlmnId;

/*
 * �ṹ˵��: Operator Controlled PLMN���ݽṹ
 */
typedef struct {
    VOS_UINT32    plmnNum; /* PLMN ID����   */
    NAS_MM_PlmnId plmnIdInfo[NAS_MMC_MAX_OPLMN_NUM];
} NAS_MMC_OprtCtrl;

/* ʹ��˵��:LAC��Ϣ �ɲο�24.008 10.5.1.3 */
typedef struct {
    VOS_UINT8 lac;
    VOS_UINT8 lacCnt;
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_Lac;
typedef NAS_MM_Lac NAS_EMM_Lac;
typedef NAS_MM_Lac NAS_MMC_Lac;

/* ʹ��˵��:RAC��Ϣ �ɲο�24.008 10.5.5.15 */
typedef struct {
    VOS_UINT8 rac;
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_Rac;
typedef NAS_MM_Rac NAS_EMM_Rac;
typedef NAS_MM_Rac NAS_MMC_Rac;

/* ʹ��˵��:TAC��Ϣ  24.301  9.9.3.26 */
typedef struct {
    VOS_UINT8 tac;    /* TAC */
    VOS_UINT8 tacCnt; /* TAC (continued) */
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_Tac;
typedef NAS_MM_Tac NAS_EMM_Tac;
typedef NAS_MM_Tac NAS_MMC_Tac;

/* �ṹ˵��: LA��Ϣ���ݽṹ */
typedef struct {
    NAS_MM_PlmnId plmnId;
    NAS_MM_Lac    lacInfo;
} NAS_MM_La;
typedef NAS_MM_La NAS_EMM_La;
typedef NAS_MM_La NAS_MMC_La;

/* �ṹ˵��: RA��Ϣ���ݽṹ */
typedef struct {
    NAS_MM_PlmnId plmnId;
    NAS_MM_Lac    lacInfo;
    NAS_MM_Rac    racInfo;
} NAS_MM_Ra;
typedef NAS_MM_Ra NAS_EMM_Ra;
typedef NAS_MM_Ra NAS_MMC_Ra;

/* �ṹ˵��: TA��Ϣ���ݽṹ */
typedef struct {
    NAS_MM_PlmnId plmnId;  /* PLMN ID�� */
    NAS_MM_Tac    tacInfo; /* TAC�� */
} NAS_MM_Ta;
typedef NAS_MM_Ta NAS_EMM_Ta;
typedef NAS_MM_Ta NAS_MMC_Ta;

/* �ṹ˵��: TA��Ϣ���ݽṹ */
typedef struct {
    VOS_UINT32 taNum;                     /* TA�ĸ���    */
    NAS_MM_Ta  taInfo[NAS_MM_MAX_TA_NUM]; /* TA��Ϣ */
} NAS_MM_Talist;

typedef NAS_MM_Talist NAS_EMM_TaList;
typedef NAS_MM_Talist NAS_MMC_TaList;

/*
 * �ṹ˵��  : FORB TA list���ݽṹ
 */
typedef NAS_MM_Talist     NAS_MM_ForbTaList;
typedef NAS_MM_ForbTaList NAS_EMM_ForbTaList;
typedef NAS_MM_ForbTaList NAS_MMC_ForbTaList;

/*
 * �ṹ˵��: LA list���ݽṹ
 */
typedef struct {
    VOS_UINT32 laNum;
    NAS_MM_La  laInfo[NAS_MM_MAX_LA_NUM];
} NAS_MM_LaList;
typedef NAS_MM_LaList NAS_EMM_LaList;
typedef NAS_MM_LaList NAS_MMC_LaList;

/*
 * �ṹ˵��  : FORB LA list���ݽṹ
 */
typedef NAS_MM_LaList     NAS_MM_ForbLaList;
typedef NAS_MM_ForbLaList NAS_EMM_ForbLaList;
typedef NAS_MM_ForbLaList NAS_MMC_ForbLaList;

/* ʹ��˵��:10.5.13/3GPP TS 24.008 PLMN List information element */
typedef struct {
    VOS_UINT32    plmnNum;                     /* �б��е�PLMN���� */
    NAS_MM_PlmnId plmnId[NAS_MM_MAX_PLMN_NUM]; /* PLMN�б� */
} NAS_MM_PlmnList;
typedef NAS_MM_PlmnList NAS_EMM_PlmnList;
typedef NAS_MM_PlmnList NAS_MMC_PlmnList;
/* begin for r11 2014-09-04 */
typedef NAS_MM_PlmnList NAS_ESM_PlmnList;
/* end for r11 2014-09-04 */
/*
 * �ṹ˵��: ���н��뼼����PLMN
 */
typedef struct {
    VOS_UINT32 plmnNum;
    VOS_UINT8  plmnId[NAS_MM_MAX_PLMN_AND_ACT_LEN];
} NAS_MM_PlmnActList;

/*
 * �ṹ˵��: 24.301 9.9.3.10  EPS mobile identity
 *           ������Ա�Ľṹ����:
 *               ----------------------------------------------------------
 *           (BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *               ----------------------------------------------------------
 *               Length of EPS mobile identity contents, UNIT is byte
 *               ----------------------------------------------------------
 *               1     1      1      1  | OorE |  Type of identity
 *               ----------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 lenth; /* GUTI���ȡ� */
    VOS_UINT8 oeToi; /* ��ʶ�Ƿ���GUTI�� */
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_GutiHead;
typedef NAS_MM_GutiHead NAS_EMM_GutiHead;
typedef NAS_MM_GutiHead NAS_MMC_GutiHead;

/*
 * ʹ��˵��:NAS_MM_MME_GROUPID_STRU����ز���
 * Э�������� ��DIGIT���ֽ�/λ�Ĺ�ϵ����:
 *     ----------------------------------------------------------------------------------
 *     (BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     ----------------------------------------------------------------------------------
 *                                GroupId
 *     ----------------------------------------------------------------------------------
 *                               GroupIdCnt
 *     ----------------------------------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 groupId;    /* Group ID�� */
    VOS_UINT8 groupIdCnt; /* Group ID������ */
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_MmeGroupId;
typedef NAS_MM_MmeGroupId NAS_EMM_MmeGroupId;
typedef NAS_MM_MmeGroupId NAS_MMC_MmeGroupId;

/* ʹ��˵��:NAS_MM_MME_CODE_STRU����ز��� */
typedef struct {
    VOS_UINT8 mmeCode; /* MME Code�� */
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_MmeCode;
typedef NAS_MM_MmeCode NAS_EMM_MmeCode;
typedef NAS_MM_MmeCode NAS_MMC_MmeCode;

/*
 * ʹ��˵��:NAS_MM_MTMSI_STRU����ز���
 * Э�������� ��DIGIT���ֽ�/λ�Ĺ�ϵ����:
 *     ----------------------------------------------------------------------------------
 *     (BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     ----------------------------------------------------------------------------------
 *                                MTmsi
 *     ----------------------------------------------------------------------------------
 *     ......
 *     ----------------------------------------------------------------------------------
 *                              MTmsiCnt3
 *     ----------------------------------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 mtmsi;     /* MTMSI�� */
    VOS_UINT8 mtmsiCnt1; /* MTMSI�� */
    VOS_UINT8 mtmsiCnt2; /* MTMSI�� */
    VOS_UINT8 mtmsiCnt3; /* MTMSI�� */
} NAS_MM_Mtmsi;
typedef NAS_MM_Mtmsi NAS_EMM_Mtmsi;
typedef NAS_MM_Mtmsi NAS_MMC_Mtmsi;

/* ʹ��˵��:NAS_MM_GUTI_STRU����ز��� */
typedef struct {
    NAS_MM_GutiHead   gutiHead;   /* GUTI Head�� */
    NAS_MM_PlmnId     plmnId;     /* PLMN ID */
    NAS_MM_MmeGroupId mmeGroupId; /* MME Group ID�� */
    NAS_MM_MmeCode    mmeCode;    /* MME Code�� */
    NAS_MM_Mtmsi      mtmsi;      /* M-TMSI�� */
} NAS_MM_Guti;
typedef NAS_MM_Guti NAS_EMM_Guti;
typedef NAS_MM_Guti NAS_MMC_Guti;

/*
 * ʹ��˵��: 24.008 10.5.6.13
 *           MBMS Service ID (octet 3, 4 and 5)
 *           In the MBMS Service ID field bit 8 of octet 3 is the most
 *           significant bit and bit 1 of octet 5 the least significant bit.
 *           The coding of the MBMS Service ID is the responsibility of each
 *           administration. Coding using full hexadecimal representation may
 *           be used. The MBMS Service ID consists of 3 octets.
 */
typedef struct {
    VOS_UINT32 mbmsSerId;
} NAS_MM_MbmsSerId;
typedef NAS_MM_MbmsSerId NAS_EMM_MbmsSerId;
typedef NAS_MM_MbmsSerId NAS_MMC_MbmsSerId;

/* ʹ��˵��:NAS_MM_TMGI_STRU����ز��� */
typedef struct {
    VOS_UINT32 opPlmn : 1; /* 1: VALID; 0: INVALID */
    VOS_UINT32 opSpare : 31;

    NAS_MM_MbmsSerId mbmsSer;
    NAS_MM_PlmnId    plmnId;
} NAS_MM_Tmgi;
typedef NAS_MM_Tmgi NAS_EMM_Tmgi;
typedef NAS_MM_Tmgi NAS_MMC_Tmgi;

/* ʹ��˵��:NAS_MM_TMGI_MBMS_HEAD_STRU����ز��� */
typedef struct {
    VOS_UINT8 lenth;
    VOS_UINT8 oeTi;
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_TmgiMbmsHead;
typedef NAS_MM_TmgiMbmsHead NAS_EMM_TmgiMbmsHead;
typedef NAS_MM_TmgiMbmsHead NAS_MMC_TmgiMbmsHead;

/* ʹ��˵��:NAS_MM_TMGI_MBMS_STRU����ز��� */
typedef struct {
    VOS_UINT32 opMbmsSesId : 1; /* 1: VALID; 0: INVALID */
    VOS_UINT32 opSpare : 31;

    NAS_MM_TmgiMbmsHead msidHead;
    NAS_MM_Tmgi         tmgi;
    VOS_UINT8           mbmsSesId;
    VOS_UINT8           rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_TmgiMbms;
typedef NAS_MM_TmgiMbms NAS_EMM_TmgiMbms;
typedef NAS_MM_TmgiMbms NAS_MMC_TmgiMbms;

/* ʹ��˵��: */
typedef struct {
    VOS_UINT8 splitPgCode; /* SPLIT PG CYCLE CODE */
    VOS_UINT8 psDrxLen;    /* DRX length         */
    VOS_UINT8 splitOnCcch; /* SPLIT on CCCH      */
    VOS_UINT8 nonDrxTimer; /* non-DRX timer      */
} NAS_MM_Drx;
typedef NAS_MM_Drx NAS_EMM_Drx;
typedef NAS_MM_Drx NAS_MMC_Drx;
/* Add for LTE CSG, 2015-09-24, Begin */
/* ʹ��˵��: */
typedef struct {
    VOS_UINT8 homeNodebNameLen;
    VOS_UINT8 reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT8 homeNodebName[NAS_MM_MAX_HOME_NODEB_NAME_LEN];
} NAS_MM_CsgIdHomeNodebName;

/* ʹ��˵��: */
typedef struct {
    NAS_MM_CellTypeUint8 cellType;
    VOS_UINT8            reserved[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT32           csgId;
} NAS_MM_CsgInfo;

/* Add for LTE CSG, 2015-09-24, End */

/* ʹ��˵��: */
typedef struct {
    VOS_UINT32 opPlmnId : 1; /* 1: VALID; 0: INVALID */
    VOS_UINT32 opLac : 1;    /* 1: VALID; 0: INVALID */
    VOS_UINT32 opRac : 1;    /* 1: VALID; 0: INVALID */
    VOS_UINT32 opTac : 1;    /* 1: VALID; 0: INVALID */
    VOS_UINT32 opCellId : 1;
    /* Mod for LNAS_POSITION_INFO, 2015-10-15, Begin */
    VOS_UINT32 opLteBand : 1;
    VOS_UINT32 opRsv : 26;
    /* Mod for LNAS_POSITION_INFO, 2015-10-15, End */
    NAS_MM_PlmnId plmnIdInfo;
    NAS_MM_Lac    lacInfo;
    NAS_MM_Rac    racInfo;
    NAS_MM_Tac    tacInfo;
    VOS_UINT32    cellId; /* Cell Identity */
    /* emergency tau&service begin */
    VOS_UINT32 forbiddenInfo;
    VOS_UINT32 cellStatus;
    /* emergency tau&service end */
    /* Mod for LNAS_POSITION_INFO, 2015-10-12, Begin */
    /* Modified for Band66 support ,2017-04-20,begin */
    VOS_UINT32 lteBand[NAS_LMM_BAND_MAX_LENTH];
    /* Modified for Band66 support ,2017-04-20,end */
    VOS_UINT32 arfcn; /* פ��Ƶ����Ϣ */
    /* Mod for LNAS_POSITION_INFO, 2015-10-12, End */
    /* mod for ProcedureCompatibilityPhaseI, 2018-06-06, Begin */
    VOS_UINT16           physCellId; /* ����С��ID */
    NAS_MM_CellTypeUint8 cellType;
    /* Begin Mod for AccessType Issue 20180925 */
    VOS_UINT8 accessType;
    /* End Mod for AccessType Issue 20180925 */
    VOS_UINT32 csgId;
    /* mod for ProcedureCompatibilityPhaseI, 2018-06-06, End */
    /* Begin Add for AccessType Issue 20180925 */
    VOS_UINT8 supportImsEmcFlag;
    VOS_UINT8 bandWidth;
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
    /* End Add for AccessType Issue 20180925 */
} NAS_MM_NetworkId;
typedef NAS_MM_NetworkId NAS_EMM_NetworkId;
typedef NAS_MM_NetworkId NAS_MMC_NetworkId;

/* ʹ��˵��:KSI and sequence number, 24.301 9.9.3.17 */
typedef struct {
    VOS_UINT8 ksiSn;
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_KsiSn;
typedef NAS_MM_KsiSn NAS_EMM_KsiSn;
typedef NAS_MM_KsiSn NAS_MMC_KsiSn;

/* ʹ��˵��:NAS key set identifier, 24.301 9.9.3.19 */
typedef struct {
    VOS_UINT8 nksi;
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_Nksi;
typedef NAS_MM_Nksi NAS_EMM_Nksi;
typedef NAS_MM_Nksi NAS_MMC_Nksi;

/*
 * ʹ��˵��:24.301-950 9.9.3.34
 * (BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     --------------------------------------------------------------
 *     Length of UE network capability contents, UNIT is byte      octet 0
 *     --------------------------------------------------------------
 *     EEA0 |EEA 1|EEA2 |EEA3 |EEA4 |EEA5 |EEA6 |EEA7              octet 1
 *     --------------------------------------------------------------
 *     EIA0 |EIA1 |EIA2 |EIA3 |EIA4 |EIA5 |EIA6 |EIA7              octet 2
 *     --------------------------------------------------------------
 *     UEA0 |UEA1 |UEA2 |UEA3 |UEA4 |UEA5 |UEA6 |UEA7              octet 3
 *     --------------------------------------------------------------
 *     UCS2 |UIA1 |UIA2 |UIA3 |UIA4 |UIA5 |UIA6 |UIA7              octet 4
 *     --------------------------------------------------------------
 *     0    |0    |0    |0    |LPP  |LCS  |1xSR |NF                octet 5
 *     spare|spare|spare|spar |     |     |VCC  |
 *     --------------------------------------------------------------
 *      spare                                                      octet6-13
 *     --------------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 ueNetCapLen;                         /* UE Network�������ݳ��ȣ���λ���ֽڡ� */
    VOS_UINT8 ueNetCap[NAS_MM_MAX_UE_NET_CAP_LEN]; /* UE Network�������ݣ���ο����Э�顣 */
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_MM_UeNetCap;
typedef NAS_MM_UeNetCap NAS_EMM_UeNetCap;
typedef NAS_MM_UeNetCap NAS_MMC_UeNetCap;

/*
 * ʹ��˵��:24.008 10.5.5.12
 *     aucMsNetCap��������Э���������ֽ�/λ�Ĺ�ϵ����:
 *     --------------------------------------------------------------
 *     (BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
 *     --------------------------------------------------------------
 *         Length of MS network capability contents, UNIT is byte      octet 0
 *     --------------------------------------------------------------
 *         MS network capability value                                 octet 1-8
 *     --------------------------------------------------------------
 */
typedef struct {
    VOS_UINT8 msNetCapLen;
    VOS_UINT8 msNetCap[NAS_MM_MAX_MS_NET_CAP_LEN];
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_MsNetCap;
typedef NAS_MM_MsNetCap NAS_EMM_MsNetCap;
typedef NAS_MM_MsNetCap NAS_MMC_MsNetCap;

/* ʹ��˵��:24.008 840 10.5.1.5 */
typedef struct {
    VOS_UINT8 value;
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} NAS_MM_MsClassmark1;

/* ʹ��˵��:24.008 840 10.5.1.6 */
typedef struct {
    VOS_UINT8 lenLvLen;

    VOS_UINT8 opRfPowerCap : 3;
    VOS_UINT8 opA51 : 1;
    VOS_UINT8 opEsind : 1;
    VOS_UINT8 opRevisionLev : 2;
    VOS_UINT8 opSpare1 : 1;

    VOS_UINT8 opFc : 1;
    VOS_UINT8 opVgcs : 1;
    VOS_UINT8 opVbs : 1;
    VOS_UINT8 opSmCap : 1;
    VOS_UINT8 opSsScreenInd : 2;
    VOS_UINT8 opPsCap : 1;
    VOS_UINT8 opSpare2 : 1;

    VOS_UINT8 opA52 : 1;
    VOS_UINT8 opA53 : 1;
    VOS_UINT8 opCmsp : 1;
    VOS_UINT8 opSolsa : 1;
    VOS_UINT8 opUcs2 : 1;
    VOS_UINT8 opLcsvaCap : 1;
    VOS_UINT8 opSpare3 : 1;
    VOS_UINT8 opCm3 : 1;

} NAS_MM_MsClassmark2;

/* ʹ��˵��:24.008 840 10.5.1.7 */
typedef struct {
    VOS_UINT8 lenLvLen;
    VOS_UINT8 value[32];
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */

} NAS_MM_MsClassmark3;

/*
 * ʹ��˵��:24.008 840 10.5.1.6
 *         0   EPS encryption algorithm *** not supported
 *         1   EPS encryption algorithm *** supported
 *         #define NAS_MM_ALGORITHM_NOT_SUPPORT    0
 *         #define NAS_MM_ALGORITHM_SUPPORT        1
 */
typedef struct {
    VOS_UINT32 opClassMark1 : 1;
    VOS_UINT32 opClassMark2 : 1;
    VOS_UINT32 opClassMark3 : 1;

    VOS_UINT32 opRsv : 29;

    NAS_MM_MsClassmark1 classMark1;
    NAS_MM_MsClassmark2 classMark2;
    NAS_MM_MsClassmark3 classMark3;

} NAS_MM_MsClassmark;
typedef NAS_MM_MsClassmark NAS_EMM_MsClassmark;
typedef NAS_MM_MsClassmark NAS_MMC_MsClassmark;

/* ʹ��˵��:MM�Ĺ�����Ϣ */
typedef struct {
    VOS_UINT16 emmSrvInfoLen;
    VOS_UINT8  emmSrvInfo[NAS_MM_SERVICE_TABLE_LEN];
    VOS_UINT8  rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} NAS_EMM_SRV_INFO_STRU;

typedef struct {
    VOS_UINT32 seq[NAS_EMM_USIM_SEQ_UINT32_LEN];
} NAS_EMM_UsimSeq;

/* ʹ��˵��:��Ȩ������RES�����ݽṹ */
typedef struct {
    NAS_EMM_UsimSeq seq[NAS_EMM_USIM_SEQ_ARRAY_LEN];
    NAS_EMM_UsimSeq maxSeq;
    VOS_UINT8       maxInd;
    VOS_UINT8       maxSqn[NAS_EMM_USIM_SQN_LEN];
    VOS_UINT8       rsv[1]; /* �����ֶΣ�����Ϊ1,����4�ֽڶ��� */
} NAS_EMM_UsimControl;

/* ʹ��˵��:��USIMģ����ɼ�Ȩ������Ҫ�Ĳ���,֧��TEST��Millenge�����㷨 */
typedef struct {
    NAS_EMM_SoftUsimAuthAlgUint32 softUsimAlg;
    VOS_UINT8                     key[NAS_EMM_SOFT_USIM_KEY_LEN];
    VOS_UINT8                     op[NAS_EMM_SOFT_USIM_OP_LEN];
} NAS_EMM_SoftUsimAuthParam;

/* self-adaption NW cause modify begin */

/* ʹ��˵��:�û���������ԭ������ݽṹ */
typedef struct {
    VOS_UINT8 cnCause;       /* ����ʵ��Я����ԭ��ֵ��Ĭ��Ϊ0�� */
    VOS_UINT8 hplmnCause;    /* UE��HPLMNʱ����Ӧת����ԭ��ֵ��Ĭ��Ϊ0�� */
    VOS_UINT8 notHplmnCause; /* UE�ڷ�HPLMNʱ����Ӧת����ԭ��ֵ��Ĭ��Ϊ0�� */
    VOS_UINT8 rsv;           /* �����ֽڣ�Ĭ��Ϊ0�� */
} NAS_LMM_ADAPTION_CAUSE_STRU;

typedef NAS_LMM_ADAPTION_CAUSE_STRU NAS_LMM_AdaptionCause;

/* ʹ��˵��:�û���������ԭ������ݽṹ */
/* �ṹ˵��  : ATTACH��TAU REJ����/ATTACH��TAU EPS ONLY����������Я��ԭ��ֵת��������NV��(ע:REJ��EPS ONLY��������) */
typedef struct {
    VOS_UINT8                   causeNum;                          /* ���õ�ת��ԭ��ֵ������Ĭ��Ϊ0�� */
    VOS_UINT8                   rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    NAS_LMM_ADAPTION_CAUSE_STRU adaptCause[NAS_LMM_MAX_CAUSE_NUM]; /* ���õľ���ת��ԭ��ֵ��Ĭ��Ϊȫ0�� */
} LNAS_LMM_ConfigNwCause;
/* self-adaption NW cause modify end */

/* mod for program 2015-01-04 begin */
/* ʹ��˵��:����DAM��Ч��PLMN�б� */
typedef struct {
    /*
     * PLMN���������ֵΪ16��
     * Ĭ��ֵ��1��
     */
    VOS_UINT32    plmnNum;
    /*
     * PLMN�б�
     * Ĭ��ֵ��0x13 0x00 0x14��
     */
    NAS_MM_PlmnId plmnId[NAS_MM_DAM_MAX_PLMN_NUM];
} NAS_MM_DamPlmnList;

/* ʹ��˵��:NV���õ�PDP�����ԭ��ֵ�б� */
typedef struct {
    VOS_UINT8 causeNum; /* �û����õ�ԭ��ֵ�� */
    VOS_UINT8 rsv[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
    VOS_UINT8 cause[NAS_MM_MAX_PDP_REJ_CAUSE_NUM]; /* �û����õľ���ԭ��ֵ�� */
} NAS_ConfigPdpRejCause;

typedef NAS_ConfigPdpRejCause NAS_ConfigPdpPermCause;
typedef NAS_ConfigPdpRejCause NAS_ConfigPdpTempCause;
typedef NAS_MM_DamPlmnList    NAS_DamImsiPlmnList;

/* ʹ��˵��:DAM�������õ�ĳ��Ӫ�̵�MCC-MNC�κ�DAM��Ч��PLMN�б� */
typedef struct {
    NAS_DamImsiPlmnList imsiPlmnList; /* IMSI��MCC-MNC���б�.
                                       * DAM���Կ��ƿ���ʹ��ucDamFlag[0]����������ʹ�ô˽ṹ�塣���ڲ�Ʒ���Ѷ��Ƹýṹ������˽ṹ����������������ʹ�á� */
    NAS_MM_DamPlmnList damPlmnList;   /* DAM������Ч��PLMN�б�DAM������Чʱ��RPLMN�����ڴ��б��� */
} NAS_DamConfigPLmnPara;
/* ʹ��˵��:NV���õ�Back-off�㷨����Fx */
typedef struct {
    VOS_UINT8 fxIgnore; /* 1Сʱ�ڳ�ʱ�޶�����     */
    VOS_UINT8 fxPerm;   /* 1Сʱ�����þܾ��޶����� */
    VOS_UINT8 fxTemp;   /* 1Сʱ����ʱ�ܾ��޶����� */
    VOS_UINT8 rsv;
} NAS_ConfigBackoffFxPara;
/* mod for program 2015-01-04 end */

/* mod for program phaseIII 2015-03-15 begin */
/*
 * �ṹ˵��  : ����������Ч��IMSI PLMN�б�
 */
typedef struct {
    VOS_UINT32    plmnNum;                            /* Plmn������ */
    NAS_MM_PlmnId plmnId[NAS_TMO_IMSI_HPLMN_MAX_NUM]; /* Plmn ID */
} NAS_TmoImsiHplmnList;
/* mod for program phaseIII 2015-03-15 end */
/* add for separate special part from original NV, 2017-03-07, begin */
/*
 * �ṹ˵��  : ����������Ч��IMSI PLMN�б�
 * ��ע : �˶���NV��֧��˫LTE�İ汾��������Ч����֧��˫LTE�İ汾Ӧ����0xD21C�еĶ�Ӧ���
 */
typedef NAS_TmoImsiHplmnList LNAS_LMM_NvTmoImsiHplmnListConfig;
/* add for separate special part from original NV, 2017-03-07, end */
/* Added by for 2019_06 cr refresh throt related, 2019-7-11, begin */
/* ʹ��˵��:EPLMN�б� */
typedef struct {
    VOS_UINT32    plmnNum;                         /* Plmn������ */
    NAS_MM_PlmnId plmnId[NAS_THROT_EPLMN_MAX_NUM]; /* Plmn ID */
} NAS_ThrotEplmnList;
/* Added by for 2019_06 cr refresh throt related, 2019-7-11, end */
typedef struct {
    NAS_MM_PlmnId plmnId;         /* �ܿ��Ƶ�PLMN ID */
    VOS_UINT32    timerLen;       /* ��������T3402��ʱ��ʱ��(UNIT: ��) */
    VOS_UINT32    timerRemainLen; /* �ػ�ʱTimer��ͣ״̬�µ�ʣ��ʱ��(UNIT: ms) */
} NAS_PlmnT3402;

/* Added for DSDS OPTIMIZE MT DETACH BY TAU, 2016-06-16, Begin */
/* ʹ��˵��:TAU�Ż���������DETACH��Ч��PLMN�б� */
typedef struct {
    VOS_UINT32    plmnNum;
    NAS_MM_PlmnId plmnId[NAS_MT_DETACH_TAU_PLMN_MAX_NUM];
} NAS_MM_MtDetachTauPlmnList;
/* Added for DSDS OPTIMIZE MT DETACH BY TAU, 2016-06-16, End */
typedef struct {
    NAS_MM_PlmnId plmnId;                  /* �ܿ��Ƶ�PLMN ID */
    VOS_UINT32    t3396RemainLenForCustom; /* �ػ�ʱTimer��ͣ״̬�µ�ʣ��ʱ��(UNIT: ms)�����ƣ� */
} NAS_PlmnT3396;
/* Added for PDP_CAUSE_RPT,2017-09-11,Begin */
/* �ṹ˵��: ESM��APS��ԭ��ֵת�� */
typedef struct {
    VOS_UINT32 esmCause;
    VOS_UINT32 appCause;
} ESM_AppErrCodeMap;
/* Added for PDP_CAUSE_RPT,2017-09-11,End */

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AppNasComm.h */
