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
#include "AtEventReport.h"
#include "taf_type_def.h"
#include "securec.h"

#include "ppp_interface.h"
#include "AtDataProc.h"
#include "taf_drv_agent.h"
#include "at_oam_interface.h"
#include "mn_comm_api.h"

#include "AtInputProc.h"
#include "fc_interface.h"
#include "AtCmdMsgProc.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "gen_msg.h"
#include "at_lte_common.h"
#endif

#include "taf_app_mma.h"

#include "app_vc_api.h"
#include "taf_app_rabm.h"
#include "AtCmdSimProc.h"

#include "product_config.h"

#include "TafStdlib.h"

#include "AtMsgPrint.h"
#include "AtCmdCallProc.h"

#include "AtCmdSupsProc.h"
#include "mnmsgcbencdec.h"
#include "dms_file_node_i.h"
#include "AtCmdMsgProc.h"
#include "at_mdrv_interface.h" /* DRV_OS_STATUS_SWITCH AT_SetModemState FREE_MEM_SIZE_GET */
#include "dms_msg_chk.h"
#if (FEATURE_MBB_CUST == FEATURE_ON)
#include "at_mbb_common.h"
#endif

/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_EVENTREPORT_C

#define AT_FIND_VEDIO_MODEM_DELAY_CNT 1500000
#define AT_SS_NTFY_ON_HOLD_RTN_NUM 2
#define AT_SS_NTFY_RETRIEVED_RTN_NUM 3
#define AT_SS_NTFY_ENTER_MPTY_RTN_NUM 4
#define AT_SS_NTFY_HOLD_RELEASED_RTN_NUM 5
#define AT_SS_NTFY_EXPLICIT_CALL_S_ALERTING_RTN_NUM 7
#define AT_SS_NTFY_EXPLICIT_CALL_TRANSFER_RTN_NUM 8
#define AT_SS_NTFY_DEFLECTED_CALL_RTN_NUM 9
#define AT_SS_NTFY_INCALL_FORWARDED_RTN_NUM 10
#define AT_CF_CAUSE_ALWAYS_RTN_NUM 0
#define AT_CF_CAUSE_POWER_OFF_RTN_NUM 2
#define AT_CF_CAUSE_NO_ANSWER_RTN_NUM 3
#define AT_CF_CAUSE_SHADOW_ZONE_RTN_NUM 4
#define AT_CF_CAUSE_DEFLECTION_480_RTN_NUM 5
#define AT_CF_CAUSE_DEFLECTION_487_RTN_NUM 6
#define AT_PROCESS_USS_REQ_CNF_TMP 2
#define AT_CLI_SS_UNKNOWN_REASON 2
#define AT_TAFMMA_CELL_ID_INDEX_0 0
#define AT_FOUR_BYTES_TO_BITS_LENS 32
#define AT_CMD_CLVL_VAL_NUM 6
#define AT_CTZE_DEFAULT_YEAR 2000
#define AT_SYS_MODE_NAME_ARRAY_LEN 255
#define AT_SUB_SYS_MODE_NAME_ARRAY_LEN 255
#define AT_SDU_ERR_RATIO_STATUS_0 0
#define AT_SDU_ERR_RATIO_STATUS_1 1
#define AT_SDU_ERR_RATIO_STATUS_2 2
#define AT_SDU_ERR_RATIO_STATUS_3 3
#define AT_SDU_ERR_RATIO_STATUS_4 4
#define AT_SDU_ERR_RATIO_STATUS_5 5
#define AT_SDU_ERR_RATIO_STATUS_6 6
#define AT_SDU_ERR_RATIO_STATUS_7 7
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_0 0
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_1 1
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_2 2
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_3 3
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_4 4
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_5 5
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_6 6
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_7 7
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_8 8
#define AT_RESIDUAL_BIT_ERR_RATIO_STATUS_9 9
#define AT_TMP_ARRAY_LEN 4
#define AT_TMP_ARRAY_INDEX_0 0
#define AT_TMP_ARRAY_INDEX_1 1
#define AT_TMP_ARRAY_INDEX_2 2
#define AT_HEX_OCCUPIED_BIT_NUM 4
#define AT_ASCII_ARRAY_LEN 250
#define AT_CLIR_SS_PROVIDED 0
#define AT_CLIR_SS_PERMANENT 1
#define AT_CLIR_SS_UNKNOWN_REASON 2
#define AT_CLIR_SS_TMP_DEFAULT_RESTRICTED 3
#define AT_CLIR_SS_TMP_DEFAULT_ALLOWED 4
#define AT_BIT_RATE_TRAFFIC_FLOWS_MAX_VALUE_RANGE 4
#define AT_MNC_LENGTH_TWO_BYTES 2
#define AT_MNC_LENGTH_THREE_BYTES 3
#define AT_BYTE_TO_BITS_LEN 8
#define AT_GET_PIH_AT_CNF_LEN_WITHOUT_UNION \
    (sizeof(MN_APP_PihAtCnf) - sizeof(((MN_APP_PihAtCnf *)16)->pihAtEvent.pihEvent))
#define AT_GET_EMAT_AT_CNF_LEN_WITHOUT_UNION \
    (sizeof(MN_APP_EmatAtCnf) - sizeof(((MN_APP_EmatAtCnf *)16)->ematAtCnf.ematEvent))

static const AT_SMS_ErrorCodeMap g_atSmsErrorCodeMap[] = {
    { TAF_MSG_ERROR_RP_CAUSE_UNASSIGNED_UNALLOCATED_NUMBER, AT_CMS_UNASSIGNED_UNALLOCATED_NUMBER },
    { TAF_MSG_ERROR_RP_CAUSE_OPERATOR_DETERMINED_BARRING, AT_CMS_OPERATOR_DETERMINED_BARRING },
    { TAF_MSG_ERROR_RP_CAUSE_CALL_BARRED, AT_CMS_CALL_BARRED },
    { TAF_MSG_ERROR_RP_CAUSE_SHORT_MESSAGE_TRANSFER_REJECTED, AT_CMS_SHORT_MESSAGE_TRANSFER_REJECTED },
    { TAF_MSG_ERROR_RP_CAUSE_DESTINATION_OUT_OF_ORDER, AT_CMS_DESTINATION_OUT_OF_SERVICE },
    { TAF_MSG_ERROR_RP_CAUSE_UNIDENTIFIED_SUBSCRIBER, AT_CMS_UNIDENTIFIED_SUBSCRIBER },
    { TAF_MSG_ERROR_RP_CAUSE_FACILITY_REJECTED, AT_CMS_FACILITY_REJECTED },
    { TAF_MSG_ERROR_RP_CAUSE_UNKNOWN_SUBSCRIBER, AT_CMS_UNKNOWN_SUBSCRIBER },
    { TAF_MSG_ERROR_RP_CAUSE_NETWORK_OUT_OF_ORDER, AT_CMS_NETWORK_OUT_OF_ORDER },
    { TAF_MSG_ERROR_RP_CAUSE_TEMPORARY_FAILURE, AT_CMS_TEMPORARY_FAILURE },
    { TAF_MSG_ERROR_RP_CAUSE_CONGESTION, AT_CMS_CONGESTION },
    { TAF_MSG_ERROR_RP_CAUSE_RESOURCES_UNAVAILABLE_UNSPECIFIED, AT_CMS_RESOURCES_UNAVAILABLE_UNSPECIFIED },
    { TAF_MSG_ERROR_RP_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED, AT_CMS_REQUESTED_FACILITY_NOT_SUBSCRIBED },
    { TAF_MSG_ERROR_RP_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED, AT_CMS_REQUESTED_FACILITY_NOT_IMPLEMENTED },
    { TAF_MSG_ERROR_RP_CAUSE_INVALID_SHORT_MESSAGE_TRANSFER_REFERENCE_VALUE, AT_CMS_INVALID_SHORT_MESSAGE_TRANSFER_REFERENCE_VALUE },
    { TAF_MSG_ERROR_RP_CAUSE_INVALID_MANDATORY_INFORMATION, AT_CMS_INVALID_MANDATORY_INFORMATION },
    { TAF_MSG_ERROR_RP_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED, AT_CMS_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED },
    { TAF_MSG_ERROR_RP_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_SHORT_MESSAGE_PROTOCOL_STATE, AT_CMS_MESSAGE_NOT_COMPATIBLE_WITH_SHORT_MESSAGE_PROTOCOL_STATE },
    { TAF_MSG_ERROR_RP_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED, AT_CMS_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED },
    { TAF_MSG_ERROR_RP_CAUSE_PROTOCOL_ERROR_UNSPECIFIED, AT_CMS_PROTOCOL_ERROR_UNSPECIFIED },
    { TAF_MSG_ERROR_RP_CAUSE_INTERWORKING_UNSPECIFIED, AT_CMS_INTERWORKING_UNSPECIFIED },
    { TAF_MSG_ERROR_TP_FCS_TELEMATIC_INTERWORKING_NOT_SUPPORTED, AT_CMS_TELEMATIC_INTERWORKING_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_SHORT_MESSAGE_TYPE_0_NOT_SUPPORTED, AT_CMS_SHORT_MESSAGE_TYPE_0_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_CANNOT_REPLACE_SHORT_MESSAGE, AT_CMS_CANNOT_REPLACE_SHORT_MESSAGE },
    { TAF_MSG_ERROR_TP_FCS_UNSPECIFIED_TPPID_ERROR, AT_CMS_UNSPECIFIED_TPPID_ERROR },
    { TAF_MSG_ERROR_TP_FCS_DATA_CODING_SCHEME_ALPHABET_NOT_SUPPORTED, AT_CMS_DATA_CODING_SCHEME_ALPHABET_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_MESSAGE_CLASS_NOT_SUPPORTED, AT_CMS_MESSAGE_CLASS_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_UNSPECIFIED_TPDCS_ERROR, AT_CMS_UNSPECIFIED_TPDCS_ERROR },
    { TAF_MSG_ERROR_TP_FCS_COMMAND_CANNOT_BE_ACTIONED, AT_CMS_COMMAND_CANNOT_BE_ACTIONED },
    { TAF_MSG_ERROR_TP_FCS_COMMAND_UNSUPPORTED, AT_CMS_COMMAND_UNSUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_UNSPECIFIED_TPCOMMAND_ERROR, AT_CMS_UNSPECIFIED_TPCOMMAND_ERROR },
    { TAF_MSG_ERROR_TP_FCS_TPDU_NOT_SUPPORTED, AT_CMS_TPDU_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_SC_BUSY, AT_CMS_SC_BUSY },
    { TAF_MSG_ERROR_TP_FCS_NO_SC_SUBSCRIPTION, AT_CMS_NO_SC_SUBSCRIPTION },
    { TAF_MSG_ERROR_TP_FCS_SC_SYSTEM_FAILURE, AT_CMS_SC_SYSTEM_FAILURE },
    { TAF_MSG_ERROR_TP_FCS_INVALID_SME_ADDRESS, AT_CMS_INVALID_SME_ADDRESS },
    { TAF_MSG_ERROR_TP_FCS_DESTINATION_SME_BARRED, AT_CMS_DESTINATION_SME_BARRED },
    { TAF_MSG_ERROR_TP_FCS_SM_REJECTEDDUPLICATE_SM, AT_CMS_SM_REJECTEDDUPLICATE_SM },
    { TAF_MSG_ERROR_TP_FCS_TPVPF_NOT_SUPPORTED, AT_CMS_TPVPF_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_TPVP_NOT_SUPPORTED, AT_CMS_TPVP_NOT_SUPPORTED },
    { TAF_MSG_ERROR_TP_FCS_SIM_SMS_STORAGE_FULL, AT_CMS_SIM_SMS_STORAGE_FULL },
    { TAF_MSG_ERROR_TP_FCS_NO_SMS_STORAGE_CAPABILITY_IN_SIM, AT_CMS_NO_SMS_STORAGE_CAPABILITY_IN_SIM },
    { TAF_MSG_ERROR_TP_FCS_ERROR_IN_MS, AT_CMS_ERROR_IN_MS },
    { TAF_MSG_ERROR_TP_FCS_MEMORY_CAPACITY_EXCEEDED, AT_CMS_MEMORY_CAPACITY_EXCEEDED },
    { TAF_MSG_ERROR_TP_FCS_SIM_APPLICATION_TOOLKIT_BUSY, AT_CMS_SIM_APPLICATION_TOOLKIT_BUSY },
    { TAF_MSG_ERROR_TP_FCS_SIM_DATA_DOWNLOAD_ERROR, AT_CMS_SIM_DATA_DOWNLOAD_ERROR },
    { TAF_MSG_ERROR_TP_FCS_UNSPECIFIED_ERROR_CAUSE, AT_CMS_UNSPECIFIED_ERROR_CAUSE },
    { TAF_MSG_ERROR_STATE_NOT_COMPATIBLE, AT_CMS_ME_FAILURE },
    { TAF_MSG_ERROR_NO_SERVICE, AT_CMS_NO_NETWORK_SERVICE },
    { TAF_MSG_ERROR_TC1M_TIMEOUT, AT_CMS_NETWORK_TIMEOUT },
    { TAF_MSG_ERROR_TR1M_TIMEOUT, AT_CMS_NETWORK_TIMEOUT },
    { TAF_MSG_ERROR_TR2M_TIMEOUT, AT_CMS_NO_CNMA_ACKNOWLEDGEMENT_EXPECTED },
};

static const TAF_PS_EvtIdUint32 g_atBroadcastPsEvtTbl[] = {
    ID_EVT_TAF_PS_REPORT_DSFLOW_IND,   ID_EVT_TAF_PS_APDSFLOW_REPORT_IND, ID_EVT_TAF_PS_CALL_PDP_DISCONNECT_IND,
    ID_EVT_TAF_PS_CALL_PDP_MANAGE_IND, ID_EVT_TAF_PS_CGMTU_VALUE_CHG_IND, ID_EVT_TAF_PS_CALL_LIMIT_PDP_ACT_IND,
    ID_EVT_TAF_PS_REPORT_VTFLOW_IND,   ID_EVT_TAF_PS_EPDG_CTRLU_NTF,

    ID_EVT_TAF_PS_UE_POLICY_RPT_IND,   ID_EVT_TAF_PS_ETHER_SESSION_CAP_IND,
#if (FEATURE_MBB_CUST == FEATURE_ON)
    ID_EVT_TAF_PS_SUCC_ATTACH_PROFILE_INDEX_IND,
#endif
};

static const AT_PS_EvtFuncTbl g_atPsEvtFuncTbl[] = {
    /* PS CALL */
    { ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpActivateCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpActivateCnf },
    { ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpActivateRej) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpActivateRej },
    { ID_EVT_TAF_PS_CALL_PDP_MANAGE_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpManageInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpManageInd },
    { ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpActivateInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpActivateInd },
    { ID_EVT_TAF_PS_CALL_PDP_MODIFY_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpModifyCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpModifyCnf },
    { ID_EVT_TAF_PS_CALL_PDP_MODIFY_REJ, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpModifyRej) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpModifyRej },
    { ID_EVT_TAF_PS_CALL_PDP_MODIFY_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpModifyInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpModifiedInd },
    { ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpDeactivateCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpDeactivateCnf },
    { ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND,  (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpDeactivateInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtPdpDeactivatedInd },

    { ID_EVT_TAF_PS_CALL_END_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallEndCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtCallEndCnf },
    { ID_EVT_TAF_PS_CALL_MODIFY_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallModifyCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtCallModifyCnf },
    { ID_EVT_TAF_PS_CALL_ANSWER_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallAnswerCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtCallAnswerCnf },
    { ID_EVT_TAF_PS_CALL_HANGUP_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallHangupCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsCallEvtCallHangupCnf },

    /* D */
    { ID_EVT_TAF_PS_GET_D_GPRS_ACTIVE_TYPE_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDGprsActiveTypeCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetGprsActiveTypeCnf },

    /* PPP */
    { ID_EVT_TAF_PS_PPP_DIAL_ORIG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_PppDialOrigCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtPppDialOrigCnf },

    /* +CGDCONT */
    { ID_EVT_TAF_PS_SET_PRIM_PDP_CONTEXT_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetPrimPdpContextInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetPrimPdpContextInfoCnf },
    { ID_EVT_TAF_PS_GET_PRIM_PDP_CONTEXT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetPrimPdpContextInfoCnfMsgLen, AT_RcvTafPsEvtGetPrimPdpContextInfoCnf },

    /* +CGDSCONT */
    { ID_EVT_TAF_PS_SET_SEC_PDP_CONTEXT_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetSecPdpContextInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetSecPdpContextInfoCnf },
    { ID_EVT_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetSecPdpContextInfoCnfMsgLen, AT_RcvTafPsEvtGetSecPdpContextInfoCnf },

    /* +CGTFT */
    { ID_EVT_TAF_PS_SET_TFT_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetTftInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetTftInfoCnf },
    { ID_EVT_TAF_PS_GET_TFT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetTftInfoCnfMsgLen, AT_RcvTafPsEvtGetTftInfoCnf },

    /* +CGEQREQ */
    { ID_EVT_TAF_PS_SET_UMTS_QOS_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetUmtsQosInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetUmtsQosInfoCnf },
    { ID_EVT_TAF_PS_GET_UMTS_QOS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetUmtsQosInfoCnfMsgLen, AT_RcvTafPsEvtGetUmtsQosInfoCnf },

    /* +CGEQMIN */
    { ID_EVT_TAF_PS_SET_UMTS_QOS_MIN_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetUmtsQosMinInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetUmtsQosMinInfoCnf },
    { ID_EVT_TAF_PS_GET_UMTS_QOS_MIN_INFO_CNF, 0,
      AT_ChkTafPsEvtGetUmtsQosMinInfoCnfMsgLen, AT_RcvTafPsEvtGetUmtsQosMinInfoCnf },

    /* +CGEQNEG */
    { ID_EVT_TAF_PS_GET_DYNAMIC_UMTS_QOS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamicUmtsQosInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamicUmtsQosInfoCnf },

    /* +CGACT */
    { ID_EVT_TAF_PS_SET_PDP_CONTEXT_STATE_CNF,  (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetPdpStateCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetPdpStateCnf },
    { ID_EVT_TAF_PS_GET_PDP_CONTEXT_STATE_CNF, 0,
      AT_ChkTafPsEvtGetPdpStateCnfMsgLen, AT_RcvTafPsEvtGetPdpStateCnf },

    /* +CGPADDR */
    { ID_EVT_TAF_PS_GET_PDP_IP_ADDR_INFO_CNF, 0,
      AT_ChkTafPsEvtGetPdpIpAddrInfoCnfMsgLen, AT_RcvTafPsEvtGetPdpIpAddrInfoCnf },
    { ID_EVT_TAF_PS_GET_PDP_CONTEXT_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetPdpContextInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetPdpContextInfoCnf },

    /* +CGAUTO */
    { ID_EVT_TAF_PS_SET_ANSWER_MODE_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetAnswerModeInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetAnsModeInfoCnf },
    { ID_EVT_TAF_PS_GET_ANSWER_MODE_INFO_CNF,  (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetAnswerModeInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetAnsModeInfoCnf },

    /* +CGCONTRDP */
    { ID_EVT_TAF_PS_GET_DYNAMIC_PRIM_PDP_CONTEXT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamicPrimPdpContextInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamicPrimPdpContextInfoCnf },
    /* +CGSCONTRDP */
    { ID_EVT_TAF_PS_GET_DYNAMIC_SEC_PDP_CONTEXT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamicSecPdpContextInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamicSecPdpContextInfoCnf },

    /* +CGTFTRDP */
    { ID_EVT_TAF_PS_GET_DYNAMIC_TFT_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamicTftInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamicTftInfoCnf },

    /* +CGEQOS */
    { ID_EVT_TAF_PS_SET_EPS_QOS_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetEpsQosInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetEpsQosInfoCnf },
    { ID_EVT_TAF_PS_GET_EPS_QOS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetEpsQosInfoCnfMsgLen, AT_RcvTafPsEvtGetEpsQosInfoCnf },

    /* +CGEQOSRDP */
    { ID_EVT_TAF_PS_GET_DYNAMIC_EPS_QOS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamicEpsQosInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamicEpsQosInfoCnf },

    /* ^CDQF/^DSFLOWQRY */
    { ID_EVT_TAF_PS_GET_DSFLOW_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDsflowInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetDsFlowInfoCnf },

    /* ^CDCF/^DSFLOWCLR */
    { ID_EVT_TAF_PS_CLEAR_DSFLOW_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ClearDsflowCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtClearDsFlowInfoCnf },

    /* ^CDSF/^DSFLOWRPT/^FLOWRPTCTRL */
    { ID_EVT_TAF_PS_CONFIG_DSFLOW_RPT_CNF,  (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ConfigDsflowRptCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtConfigDsFlowRptCnf },

    /* ^DSFLOWRPT */
    { ID_EVT_TAF_PS_REPORT_DSFLOW_IND,  (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ReportDsflowInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtReportDsFlowInd },

    /* ^VTFLOWRPT */
    { ID_EVT_TAF_PS_REPORT_VTFLOW_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ReportVtflowInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtReportVTFlowInd },
    { ID_EVT_TAF_PS_CONFIG_VTFLOW_RPT_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ConfigVtflowRptCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtConfigVTFlowRptCnf },

    /* ^CGDNS */
    { ID_EVT_TAF_PS_SET_PDP_DNS_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetPdpDnsInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetPdpDnsInfoCnf },
    { ID_EVT_TAF_PS_GET_PDP_DNS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetPdpDnsInfoCnfMsgLen, AT_RcvTafPsEvtGetPdpDnsInfoCnf },

    /* ^AUTHDATA */
    { ID_EVT_TAF_PS_SET_AUTHDATA_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetAuthDataInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetAuthDataInfoCnf },
    { ID_EVT_TAF_PS_GET_AUTHDATA_INFO_CNF, 0,
      AT_ChkTafPsEvtGetAuthDataInfoCnfMsgLen, AT_RcvTafPsEvtGetAuthDataInfoCnf },

    { ID_EVT_TAF_PS_CALL_PDP_DISCONNECT_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallPdpPdpDisconnectInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtPdpDisconnectInd },
    { ID_EVT_TAF_PS_GET_NEGOTIATION_DNS_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetNegotiationDnsCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetDynamicDnsInfoCnf },

#if (FEATURE_LTE == FEATURE_ON)
    { ID_EVT_TAF_PS_LTECS_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_LtecsCnf) - 4),
      VOS_NULL_PTR, atReadLtecsCnfProc },
    { ID_EVT_TAF_PS_CEMODE_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CemodeCnf) - 4),
      VOS_NULL_PTR, atReadCemodeCnfProc },

    { ID_EVT_TAF_PS_GET_CID_SDF_CNF, 0,
      VOS_NULL_PTR, AT_RcvTafPsEvtGetCidSdfInfoCnf },

    { ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetLteAttachInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafGetLteAttachInfoCnf },

#endif
    { ID_EVT_TAF_PS_SET_APDSFLOW_RPT_CFG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetApdsflowRptCfgCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetApDsFlowRptCfgCnf },
    { ID_EVT_TAF_PS_GET_APDSFLOW_RPT_CFG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetApdsflowRptCfgCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetApDsFlowRptCfgCnf },
    { ID_EVT_TAF_PS_APDSFLOW_REPORT_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_ApdsflowReportInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtApDsFlowReportInd },

    { ID_EVT_TAF_PS_SET_DSFLOW_NV_WRITE_CFG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetDsflowNvWriteCfgCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetDsFlowNvWriteCfgCnf },
    { ID_EVT_TAF_PS_GET_DSFLOW_NV_WRITE_CFG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDsflowNvWriteCfgCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetDsFlowNvWriteCfgCnf },

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_EVT_TAF_PS_SET_CTA_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetCtaInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetPktCdataInactivityTimeLenCnf },
    { ID_EVT_TAF_PS_GET_CTA_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetCtaInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetPktCdataInactivityTimeLenCnf },


    { ID_EVT_TAF_PS_SET_CDMA_DIAL_MODE_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CdataDialModeCnf) - 4),
      VOS_NULL_PTR, At_RcvTafPsEvtSetDialModeCnf },

    { ID_EVT_TAF_PS_CGMTU_VALUE_CHG_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CgmtuValueChgInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtCgmtuValueChgInd },
#endif

    { ID_EVT_TAF_PS_SET_IMS_PDP_CFG_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetImsPdpCfgCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetImsPdpCfgCnf },

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_EVT_TAF_PS_SET_1X_DORM_TIMER_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_Set1xDormTimerCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSet1xDormTimerCnf },

    { ID_EVT_TAF_PS_GET_1X_DORM_TIMER_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_Get1XDormTimerCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGet1xDormTimerCnf },
#endif

    /* 处理telcel pdp激活受限消息 */
    { ID_EVT_TAF_PS_CALL_LIMIT_PDP_ACT_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_CallLimitPdpActInd) - 4),
        VOS_NULL_PTR, AT_RcvTafPsCallEvtLimitPdpActInd },

    { ID_EVT_TAF_PS_SET_DATA_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SET_DATA_SWITCH_CNF_STRU) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetDataSwitchCnf },

    { ID_EVT_TAF_PS_GET_DATA_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDataSwitchCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetDataSwitchCnf },

    { ID_EVT_TAF_PS_SET_DATA_ROAM_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SET_DATA_ROAM_SWITCH_CNF_STRU) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetDataRoamSwitchCnf },

    { ID_EVT_TAF_PS_GET_DATA_ROAM_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDataRoamSwitchCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetDataRoamSwitchCnf },

    { ID_EVT_TAF_PS_EPDG_CTRLU_NTF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_EpdgCtrluNtf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtEpdgCtrluNtf },

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    /* +C5GQOS */
    { ID_EVT_TAF_PS_SET_5G_QOS_INFO_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_Set5gQosInfoCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSet5gQosInfoCnf },
    { ID_EVT_TAF_PS_GET_5G_QOS_INFO_CNF, 0,
        AT_ChkTafPsEvtGet5gQosInfoCnfMsgLen, AT_RcvTafPsEvtGet5gQosInfoCnf },

    /* +C5GQOSRDP */
    { ID_EVT_TAF_PS_GET_DYNAMIC_5G_QOS_INFO_CNF, 0,
      AT_ChkTafPsEvtGetDynamic5gQosInfoCnfMsgLen, AT_RcvTafPsEvtGetDynamic5gQosInfoCnf },

    /* ^CPOLICYRPT */
    { ID_EVT_TAF_PS_UE_POLICY_RPT_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_UePolicyRptInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtUePolicyRptInd },

    /* ^CPOLICYRPT */
    { ID_EVT_TAF_PS_SET_UE_POLICY_RPT_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetUePolicyRptCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetUePolicyRptCnf },

    /* ^CPOLICYCODE */
    { ID_EVT_TAF_PS_GET_UE_POLICY_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetUePolicyCnf) - 4),
      AT_ChkTafPsEvtGetUePolicyCnfMsgLen, AT_RcvTafPsEvtGetUePolicyCnf },

    /* ^CSUEPOLICY */
    { ID_EVT_TAF_PS_UE_POLICY_RSP_CHECK_RSLT_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_UePolicyRspCheckRsltInd) - 4),
        VOS_NULL_PTR, AT_RcvTafPsEvtUePolicyRspCheckRsltInd },

    { ID_EVT_TAF_PS_ETHER_SESSION_CAP_IND, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_EtherSessCapInd) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEtherSessCapInd },
#endif

    { ID_EVT_TAF_PS_SET_ROAMING_PDP_TYPE_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetRoamingPdpTypeCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetRoamPdpTypeCnf },
    { ID_MSG_TAF_PS_GET_SINGLE_PDN_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetSinglePdnSwitchCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtGetSinglePdnSwitchCnf },
    { ID_MSG_TAF_PS_SET_SINGLE_PDN_SWITCH_CNF, (sizeof(TAF_PS_Evt) + sizeof(TAF_PS_SetSinglePdnSwitchCnf) - 4),
      VOS_NULL_PTR, AT_RcvTafPsEvtSetSinglePdnSwitchCnf },
#if (FEATURE_MBB_CUST == FEATURE_ON)
    { ID_EVT_TAF_PS_SET_CUST_ATTACH_PROFILE_CNF, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_SetCustAttachProfileInfoCnf) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtSetCustomAttachProfileCnf },
    { ID_EVT_TAF_PS_GET_CUST_ATTACH_PROFILE_CNF, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_GetCustAttachProfileInfoCnf) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtGetCustomAttachProfileCnf },
    { ID_EVT_TAF_PS_GET_CUST_ATTACH_PROFILE_COUNT_CNF, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_GetCustAttachProfileCountCnf) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtGetCustomAttachProfileCountCnf },
    { ID_EVT_TAF_PS_SUCC_ATTACH_PROFILE_INDEX_IND, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_SuccAttachProfileIndexInd) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtSuccAttachProfileIndexInd },
    { ID_EVT_TAF_PS_SET_ATTACH_PROFILE_SWITCH_STATUS_CNF, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_SetAttachProfileSwitchStatusCnf) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtSetAttachProfileSwitchStatusCnf },
    { ID_EVT_TAF_PS_GET_ATTACH_PROFILE_SWITCH_STATUS_CNF, (sizeof(TAF_PS_Evt) +
        sizeof(TAF_PS_GetAttachProfileSwitchStatusCnf) - 4), VOS_NULL_PTR,
        AT_RcvTafPsEvtGetAttachProfileSwitchStatusCnf },
#endif
};

/* 主动上报命令与控制Bit位对应表 */
/* 命令对应顺序为Bit0~Bit63 */
static const AT_RptCmdIndexUint8 g_atCurcRptCmdTable[] = {
    AT_RPT_CMD_MODE,  AT_RPT_CMD_RSSI,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_SRVST,     AT_RPT_CMD_BUTT,
    AT_RPT_CMD_SIMST, AT_RPT_CMD_TIME,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_ANLEVEL,   AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_SMMEMFULL, AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_CTZV,
    AT_RPT_CMD_CTZE,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_DSFLOWRPT, AT_RPT_CMD_BUTT,      AT_RPT_CMD_ORIG,
    AT_RPT_CMD_CONF,  AT_RPT_CMD_CONN,      AT_RPT_CMD_CEND,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_STIN,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_CERSSI,    AT_RPT_CMD_LWCLASH,
    AT_RPT_CMD_XLEMA, AT_RPT_CMD_ACINFO,    AT_RPT_CMD_PLMN,      AT_RPT_CMD_CALLSTATE, AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,  AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT
};

static const AT_RptCmdIndexUint8 g_atUnsolicitedRptCmdTable[] = {
    AT_RPT_CMD_MODE,   AT_RPT_CMD_RSSI,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_SRVST,  AT_RPT_CMD_CREG,      AT_RPT_CMD_SIMST,
    AT_RPT_CMD_TIME,   AT_RPT_CMD_BUTT,   AT_RPT_CMD_ANLEVEL, AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_CTZV,   AT_RPT_CMD_CTZE,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_DSFLOWRPT, AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_CUSD,   AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_CSSI,   AT_RPT_CMD_CSSU,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_CERSSI, AT_RPT_CMD_LWURC,  AT_RPT_CMD_BUTT,    AT_RPT_CMD_CUUS1U, AT_RPT_CMD_CUUS1I,    AT_RPT_CMD_CGREG,
    AT_RPT_CMD_CEREG,  AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT,   AT_RPT_CMD_BUTT,      AT_RPT_CMD_BUTT,
    AT_RPT_CMD_BUTT,   AT_RPT_CMD_C5GREG, AT_RPT_CMD_BUTT,    AT_RPT_CMD_BUTT
};

static const AT_CmeCallErrCodeMap g_atCmeCallErrCodeMapTbl[] = {
    { AT_CME_INCORRECT_PARAMETERS, TAF_CS_CAUSE_INVALID_PARAMETER },
    { AT_CME_SIM_FAILURE, TAF_CS_CAUSE_SIM_NOT_EXIST },
    { AT_CME_SIM_PIN_REQUIRED, TAF_CS_CAUSE_SIM_PIN_NEED },
    { AT_CME_UNKNOWN, TAF_CS_CAUSE_NO_CALL_ID },
    { AT_CME_OPERATION_NOT_ALLOWED, TAF_CS_CAUSE_NOT_ALLOW },
    { AT_CME_INCORRECT_PARAMETERS, TAF_CS_CAUSE_STATE_ERROR },
    { AT_CME_FDN_FAILED, TAF_CS_CAUSE_FDN_CHECK_FAILURE },
    { AT_CME_CALL_CONTROL_BEYOND_CAPABILITY, TAF_CS_CAUSE_CALL_CTRL_BEYOND_CAPABILITY },
    { AT_CME_CALL_CONTROL_FAILED, TAF_CS_CAUSE_CALL_CTRL_TIMEOUT },
    { AT_CME_CALL_CONTROL_FAILED, TAF_CS_CAUSE_CALL_CTRL_NOT_ALLOWED },
    { AT_CME_CALL_CONTROL_FAILED, TAF_CS_CAUSE_CALL_CTRL_INVALID_PARAMETER },
    { AT_CME_UNKNOWN, TAF_CS_CAUSE_UNKNOWN }
};

static const AT_CMS_SmsErrCodeMap g_atCmsSmsErrCodeMapTbl[] = {
    { AT_CMS_U_SIM_BUSY, MN_ERR_CLASS_SMS_UPDATE_USIM },
    { AT_CMS_U_SIM_NOT_INSERTED, MN_ERR_CLASS_SMS_NOUSIM },
    { AT_CMS_INVALID_MEMORY_INDEX, MN_ERR_CLASS_SMS_EMPTY_REC },
    { AT_CMS_MEMORY_FULL, MN_ERR_CLASS_SMS_STORAGE_FULL },
    { AT_CMS_U_SIM_PIN_REQUIRED, MN_ERR_CLASS_SMS_NEED_PIN1 },
    { AT_CMS_U_SIM_PUK_REQUIRED, MN_ERR_CLASS_SMS_NEED_PUK1 },
    { AT_CMS_U_SIM_FAILURE, MN_ERR_CLASS_SMS_UNAVAILABLE },
    { AT_CMS_OPERATION_NOT_ALLOWED, MN_ERR_CLASS_SMS_FEATURE_INAVAILABLE },
    { AT_CMS_SMSC_ADDRESS_UNKNOWN, MN_ERR_CLASS_SMS_INVALID_SCADDR },
    { AT_CMS_INVALID_PDU_MODE_PARAMETER, MN_ERR_CLASS_SMS_MSGLEN_OVERFLOW },
    { AT_CMS_FDN_DEST_ADDR_FAILED, MN_ERR_CLASS_FDN_CHECK_DN_FAILURE },
    { AT_CMS_FDN_SERVICE_CENTER_ADDR_FAILED, MN_ERR_CLASS_FDN_CHECK_SC_FAILURE },
    { AT_CMS_MO_SMS_CONTROL_FAILED, MN_ERR_CLASS_SMS_MO_CTRL_ACTION_NOT_ALLOWED },
    { AT_CMS_MO_SMS_CONTROL_FAILED, MN_ERR_CLASS_SMS_MO_CTRL_USIM_PARA_ERROR },
    { AT_CMS_MEMORY_FAILURE, MN_ERR_NOMEM }
};

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))
static const AT_EncryptVoiceErrCodeMap g_atEncVoiceErrCodeMapTbl[] = {
    { AT_ENCRYPT_VOICE_SUCC, TAF_CALL_ENCRYPT_VOICE_SUCC },
    { AT_ENCRYPT_VOICE_TIMEOUT, TAF_CALL_ENCRYPT_VOICE_TIMEOUT },
    { AT_ENCRYPT_VOICE_TIMEOUT, TAF_CALL_ENCRYPT_VOICE_TX01_TIMEOUT },
    { AT_ENCRYPT_VOICE_TIMEOUT, TAF_CALL_ENCRYPT_VOICE_TX02_TIMEOUT },
    { AT_ENCRYPT_VOICE_LOCAL_TERMINAL_NO_AUTHORITY, TAF_CALL_ENCRYPT_VOICE_LOCAL_TERMINAL_NO_AUTHORITY },
    { AT_ENCRYPT_VOICE_REMOTE_TERMINAL_NO_AUTHORITY, TAF_CALL_ENCRYPT_VOICE_REMOTE_TERMINAL_NO_AUTHORITY },
    { AT_ENCRYPT_VOICE_LOCAL_TERMINAL_ILLEGAL, TAF_CALL_ENCRYPT_VOICE_LOCAL_TERMINAL_ILLEGAL },
    { AT_ENCRYPT_VOICE_REMOTE_TERMINAL_ILLEGAL, TAF_CALL_ENCRYPT_VOICE_REMOTE_TERMINAL_ILLEGAL },
    { AT_ENCRYPT_VOICE_UNKNOWN_ERROR, TAF_CALL_ENCRYPT_VOICE_UNKNOWN_ERROR },
    { AT_ENCRYPT_VOICE_SIGNTURE_VERIFY_FAILURE, TAF_CALL_ENCRYPT_VOICE_SIGNTURE_VERIFY_FAILURE },
    { AT_ENCRYPT_VOICE_MT_CALL_NOTIFICATION, TAF_CALL_ENCRYPT_VOICE_MT_CALL_NOTIFICATION },

    /* Internal err code */
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_FAIL, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_FAIL },
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_POOL_FULL, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_POOL_FULL },
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_LINK_ERR, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_LINK_ERR },
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_NO_TL_ACK, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_NO_TL_ACK },
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_ENCODE_ERR, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_ENCODE_ERR },
    { AT_ENCRYPT_VOICE_XSMS_SEND_RESULT_UNKNOWN, TAF_CALL_ENCRYPT_VOICE_XSMS_SEND_RESULT_UNKNOWN },
    { AT_ENCRYPT_VOICE_SO_NEGO_FAILURE, TAF_CALL_ENCRYPT_VOICE_SO_NEGO_FAILURE },
    { AT_ENCRYPT_VOICE_TWO_CALL_ENTITY_EXIST, TAT_CALL_APP_ENCRYPT_VOICE_TWO_CALL_ENTITY_EXIST },
    { AT_ENCRYPT_VOICE_NO_MO_CALL, TAF_CALL_ENCRYPT_VOICE_NO_MO_CALL },
    { AT_ENCRYPT_VOICE_NO_MT_CALL, TAF_CALL_ENCRYPT_VOICE_NO_MT_CALL },
    { AT_ENCRYPT_VOICE_NO_CALL_EXIST, TAF_CALL_ENCRYPT_VOICE_NO_CALL_EXIST },
    { AT_ENCRYPT_VOICE_CALL_STATE_NOT_ALLOWED, TAF_CALL_ENCRYPT_VOICE_CALL_STATE_NOT_ALLOWED },
    { AT_ENCRYPT_VOICE_CALL_NUM_MISMATCH, TAF_CALL_ENCRYPT_VOICE_CALL_NUM_MISMATCH },
    { AT_ENCRYPT_VOICE_ENC_VOICE_STATE_MISMATCH, TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_STATE_MISMATCH },
    { AT_ENCRYPT_VOICE_MSG_ENCODE_FAILUE, TAF_CALL_ENCRYPT_VOICE_MSG_ENCODE_FAILUE },
    { AT_ENCRYPT_VOICE_MSG_DECODE_FAILUE, TAF_CALL_ENCRYPT_VOICE_MSG_DECODE_FAILUE },
    { AT_ENCRYPT_VOICE_GET_TEMP_PUB_PIVA_KEY_FAILURE, TAF_CALL_ENCRYPT_VOICE_GET_TEMP_PUB_PIVA_KEY_FAILURE },
    { AT_ENCRYPT_VOICE_FILL_CIPHER_TEXT_FAILURE, TAF_CALL_ENCRYPT_VOICE_FILL_CIPHER_TEXT_FAILURE },
    { AT_ENCRYPT_VOICE_ECC_CAP_NOT_SUPPORTED, TAF_CALL_ENCRYPT_VOICE_ECC_CAP_NOT_SUPPORTED },
    { AT_ENCRYPT_VOICE_ENC_VOICE_MODE_UNKNOWN, TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_MODE_UNKNOWN },
    { AT_ENCRYPT_VOICE_ENC_VOICE_MODE_MIMATCH, TAF_CALL_ENCRYPT_VOICE_ENC_VOICE_MODE_MIMATCH },
    { AT_ENCRYPT_VOICE_CALL_RELEASED, TAF_CALL_ENCRYPT_VOICE_CALL_RELEASED },
    { AT_ENCRYPT_VOICE_CALL_ANSWER_REQ_FAILURE, TAF_CALL_ENCRYPT_VOICE_CALL_ANSWER_REQ_FAILURE },
    { AT_ENCRYPT_VOICE_DECRYPT_KS_FAILURE, TAF_CALL_ENCRYPT_VOICE_DECRYPT_KS_FAILURE },
    { AT_ENCRYPT_VOICE_FAILURE_CAUSED_BY_INCOMING_CALL, TAF_CALL_ENCRYPT_VOICE_FAILURE_CAUSED_BY_INCOMING_CALL },
    { AT_ENCRYPT_VOICE_INIT_VOICE_FUNC_FAILURE, TAF_CALL_ENCRYPT_VOICE_INIT_VOICE_FUNC_FAILURE },
    { AT_ENCRYPT_VOICE_ERROR_ENUM_BUTT, TAF_CALL_ENCRYPT_VOICE_STATUS_ENUM_BUTT }
};
#endif

AT_SMS_RSP_PROC_FUN g_atSmsMsgProcTable[MN_MSG_EVT_MAX] = {
    At_SendSmRspProc,           /* MN_MSG_EVT_SUBMIT_RPT */
    At_SetCnmaRspProc,          /* MN_MSG_EVT_MSG_SENT */
    At_SmsRspNop,               /* MN_MSG_EVT_MSG_STORED */
    At_SmsDeliverProc,          /* MN_MSG_EVT_DELIVER */
    At_SmsDeliverErrProc,       /* MN_MSG_EVT_DELIVER_ERR */
    At_SmsStorageListProc,      /* MN_MSG_EVT_SM_STORAGE_LIST */ /* 区分主动上报和响应消息的处理 */
    At_SmsRspNop,               /* MN_MSG_EVT_STORAGE_FULL */
    At_SmsStorageExceedProc,    /* MN_MSG_EVT_STORAGE_EXCEED */
    At_ReadRspProc,             /* MN_MSG_EVT_READ */
    At_ListRspProc,             /* MN_MSG_EVT_LIST */
    At_WriteSmRspProc,          /* MN_MSG_EVT_WRITE */
    At_DeleteRspProc,           /* MN_MSG_EVT_DELETE */
    At_DeleteTestRspProc,       /* MN_MSG_EVT_DELETE_TEST */
    At_SmsModSmStatusRspProc,   /* MN_MSG_EVT_MODIFY_STATUS */
    At_SetCscaCsmpRspProc,      /* MN_MSG_EVT_WRITE_SRV_PARM */
    AT_QryCscaRspProc,          /* MN_MSG_EVT_READ_SRV_PARM */
    At_SmsSrvParmChangeProc,    /* MN_MSG_EVT_SRV_PARM_CHANGED */
    At_SmsRspNop,               /* MN_MSG_EVT_DELETE_SRV_PARM */
    At_SmsRspNop,               /* MN_MSG_EVT_READ_STARPT */
    At_SmsRspNop,               /* MN_MSG_EVT_DELETE_STARPT */
    AT_SetMemStatusRspProc,     /* MN_MSG_EVT_SET_MEMSTATUS */
    At_SmsRspNop,               /* MN_MSG_EVT_MEMSTATUS_CHANGED */
    At_SmsRspNop,               /* MN_MSG_EVT_MATCH_MO_STARPT_INFO */
    At_SetRcvPathRspProc,       /* MN_MSG_EVT_SET_RCVMSG_PATH */
    At_SmsRspNop,               /* MN_MSG_EVT_GET_RCVMSG_PATH */
    At_SmsRcvMsgPathChangeProc, /* MN_MSG_EVT_RCVMSG_PATH_CHANGED */
    At_SmsInitSmspResultProc,   /* MN_MSG_EVT_INIT_SMSP_RESULT */
    At_SmsInitResultProc,       /* MN_MSG_EVT_INIT_RESULT */
    At_SetCmmsRspProc,          /* MN_MSG_EVT_SET_LINK_CTRL_PARAM */
    At_GetCmmsRspProc,          /* MN_MSG_EVT_GET_LINK_CTRL_PARAM */
    At_SmsStubRspProc,          /* MN_MSG_EVT_STUB_RESULT */
#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
    At_SmsDeliverCbmProc,      /* MN_MSG_EVT_DELIVER_CBM */
    At_GetCbActiveMidsRspProc, /* MN_MSG_EVT_GET_CBTYPE */
    AT_ChangeCbMidsRsp,        /* MN_MSG_EVT_ADD_CBMIDS */
    AT_ChangeCbMidsRsp,        /* MN_MSG_EVT_DELETE_CBMIDS */
    AT_ChangeCbMidsRsp,        /* MN_MSG_EVT_DELETE_ALL_CBMIDS */

#if (FEATURE_ETWS == FEATURE_ON)
    At_ProcDeliverEtwsPrimNotify, /* MN_MSG_EVT_DELIVER_ETWS_PRIM_NOTIFY */
#else
    At_SmsRspNop, /* MN_MSG_EVT_DELIVER_ETWS_PRIM_NOTIFY */
#endif

#else
    At_SmsRspNop, /* MN_MSG_EVT_DELIVER_CBM */
    At_SmsRspNop, /* MN_MSG_EVT_GET_CBTYPE */
    At_SmsRspNop, /* MN_MSG_EVT_ADD_CBMIDS */
    At_SmsRspNop, /* MN_MSG_EVT_DELETE_CBMIDS */
    At_SmsRspNop  /* MN_MSG_EVT_DELETE_ALL_CBMIDS */
    At_SmsRspNop, /* MN_MSG_EVT_DELIVER_ETWS_PRIM_NOTIFY */
#endif
    At_SetCgsmsRspProc, /* MN_MSG_EVT_SET_SEND_DOMAIN_PARAM */
    At_GetCgsmsRspProc, /* MN_MSG_EVT_GET_SEND_DOMAIN_PARAM */
};

/* 结构的最后一项不做处理，仅表示结尾 */
static const AT_QueryTypeFunc g_atQryTypeProcFuncTbl[] = {
    { TAF_PH_IMSI_ID_PARA, At_QryParaRspCimiProc },
    { TAF_PH_MS_CLASS_PARA, At_QryParaRspCgclassProc },

    { TAF_PH_ICC_ID, At_QryParaRspIccidProc },
    { TAF_PH_PNN_PARA, At_QryParaRspPnnProc },
    { TAF_PH_CPNN_PARA, At_QryParaRspCPnnProc },
    { TAF_PH_OPL_PARA, At_QryParaRspOplProc },

    { TAF_PH_PNN_RANGE_PARA, At_QryRspUsimRangeProc },
    { TAF_PH_OPL_RANGE_PARA, At_QryRspUsimRangeProc },

    { TAF_TELE_PARA_BUTT, TAF_NULL_PTR }
};

#if (VOS_WIN32 == VOS_OS_VER)
static const TAF_UINT8 g_atStin[] = "^STIN:";
static const TAF_UINT8 g_atStmn[] = "^STMN:";
static const TAF_UINT8 g_atStgi[] = "^STGI:";
static const TAF_UINT8 g_atStsf[] = "^STSF:";
static const TAF_UINT8 g_atCsin[] = "^CSIN:";
static const TAF_UINT8 g_atCstr[] = "^CSTR:";
static const TAF_UINT8 g_atCsen[] = "^CSEN:";
static const TAF_UINT8 g_atCsmn[] = "^CSMN:";
static const TAF_UINT8 g_atCcin[] = "^CCIN:";
#else
static const TAF_UINT8 g_atStin[] = "^STIN:";
static const TAF_UINT8 g_atStgi[] = "^STGI:";
static const TAF_UINT8 g_atCsin[] = "^CSIN:";
static const TAF_UINT8 g_atCstr[] = "^CSTR:";
static const TAF_UINT8 g_atCsen[] = "^CSEN:";
static const TAF_UINT8 g_atCsmn[] = "^CSMN:";
static const TAF_UINT8 g_atCcin[] = "^CCIN:";
#endif

static const AT_CALL_CuusuMsg g_cuusuMsgType[] = {
    { MN_CALL_UUS1_MSG_SETUP, AT_CUUSU_MSG_SETUP },
    { MN_CALL_UUS1_MSG_DISCONNECT, AT_CUUSU_MSG_DISCONNECT },
    { MN_CALL_UUS1_MSG_RELEASE_COMPLETE, AT_CUUSU_MSG_RELEASE_COMPLETE }
};

static const AT_CALL_CuusiMsg g_cuusiMsgType[] = {
    { MN_CALL_UUS1_MSG_ALERT, AT_CUUSI_MSG_ALERT },
    { MN_CALL_UUS1_MSG_PROGRESS, AT_CUUSI_MSG_PROGRESS },
    { MN_CALL_UUS1_MSG_CONNECT, AT_CUUSI_MSG_CONNECT },
    { MN_CALL_UUS1_MSG_RELEASE, AT_CUUSI_MSG_RELEASE }
};

/* begin V7R1 PhaseI Modify */
static const AT_PH_SysModeTbl g_sysModeTbl[] = {
    { MN_PH_SYS_MODE_EX_NONE_RAT, "NO SERVICE" },
    { MN_PH_SYS_MODE_EX_GSM_RAT, "GSM" },
    { MN_PH_SYS_MODE_EX_CDMA_RAT, "CDMA" },
    { MN_PH_SYS_MODE_EX_WCDMA_RAT, "WCDMA" },
    { MN_PH_SYS_MODE_EX_TDCDMA_RAT, "TD-SCDMA" },
    { MN_PH_SYS_MODE_EX_WIMAX_RAT, "WIMAX" },
    { MN_PH_SYS_MODE_EX_LTE_RAT, "LTE" },
    { MN_PH_SYS_MODE_EX_EVDO_RAT, "EVDO" },
    { MN_PH_SYS_MODE_EX_HYBRID_RAT, "CDMA1X+EVDO(HYBRID)" },
    { MN_PH_SYS_MODE_EX_SVLTE_RAT, "CDMA1X+LTE" },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { MN_PH_SYS_MODE_EX_EUTRAN_5GC_RAT, "EUTRAN-5GC" },
    { MN_PH_SYS_MODE_EX_NR_5GC_RAT, "NR-5GC" },
    { MN_PH_SYS_MODE_EX_1X_NR_5GC_RAT, "CDMA1X+NR-5GC" },
#endif
};

static const AT_PH_SubSysModeTbl g_subSysModeTbl[] = {
    { MN_PH_SUB_SYS_MODE_EX_NONE_RAT, "NO SERVICE" },
    { MN_PH_SUB_SYS_MODE_EX_GSM_RAT, "GSM" },
    { MN_PH_SUB_SYS_MODE_EX_GPRS_RAT, "GPRS" },
    { MN_PH_SUB_SYS_MODE_EX_EDGE_RAT, "EDGE" },
    { MN_PH_SUB_SYS_MODE_EX_WCDMA_RAT, "WCDMA" },
    { MN_PH_SUB_SYS_MODE_EX_HSDPA_RAT, "HSDPA" },
    { MN_PH_SUB_SYS_MODE_EX_HSUPA_RAT, "HSUPA" },
    { MN_PH_SUB_SYS_MODE_EX_HSPA_RAT, "HSPA" },
    { MN_PH_SUB_SYS_MODE_EX_HSPA_PLUS_RAT, "HSPA+" },
    { MN_PH_SUB_SYS_MODE_EX_DCHSPA_PLUS_RAT, "DC-HSPA+" },
    { MN_PH_SUB_SYS_MODE_EX_TDCDMA_RAT, "TD-SCDMA" },
    { MN_PH_SUB_SYS_MODE_EX_TD_HSDPA_RAT, "HSDPA" },
    { MN_PH_SUB_SYS_MODE_EX_TD_HSUPA_RAT, "HSUPA" },
    { MN_PH_SUB_SYS_MODE_EX_TD_HSPA_RAT, "HSPA" },
    { MN_PH_SUB_SYS_MODE_EX_TD_HSPA_PLUS_RAT, "HSPA+" },

    { MN_PH_SUB_SYS_MODE_EX_LTE_RAT, "LTE" },

    { MN_PH_SUB_SYS_MODE_EX_CDMA20001X_RAT, "CDMA2000 1X" },

    { MN_PH_SUB_SYS_MODE_EX_EVDOREL0_RAT, "EVDO Rel0" },
    { MN_PH_SUB_SYS_MODE_EX_EVDORELA_RAT, "EVDO RelA" },
    { MN_PH_SUB_SYS_MODE_EX_HYBIRD_EVDOREL0_RAT, "HYBRID(EVDO Rel0)" },
    { MN_PH_SUB_SYS_MODE_EX_HYBIRD_EVDORELA_RAT, "HYBRID(EVDO RelA)" },

    { MN_PH_SUB_SYS_MODE_EX_EHRPD_RAT, "EHRPD" },

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { MN_PH_SUB_SYS_MODE_EX_EUTRAN_5GC_RAT, "EUTRAN-5GC" },
    { MN_PH_SUB_SYS_MODE_EX_NR_5GC_RAT, "NR-5GC" },
#endif
};
/* end V7R1 PhaseI Modify */

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 g_guTmodeCnf  = 0;
VOS_UINT32 g_lteTmodeCnf = 0;
#endif

/* +CLCK命令参数CLASS与Service Type Code对应扩展表 */
static const AT_ClckClassServiceTbl g_clckClassServiceExtTbl[] = {
    { AT_CLCK_PARA_CLASS_VOICE, TAF_SS_TELE_SERVICE, TAF_ALL_SPEECH_TRANSMISSION_SERVICES_TSCODE },
    { AT_CLCK_PARA_CLASS_VOICE, TAF_SS_TELE_SERVICE, TAF_TELEPHONY_TSCODE },
    { AT_CLCK_PARA_CLASS_VOICE, TAF_SS_TELE_SERVICE, TAF_EMERGENCY_CALLS_TSCODE },
    { AT_CLCK_PARA_CLASS_DATA, TAF_SS_BEARER_SERVICE, TAF_ALL_BEARERSERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA, TAF_SS_TELE_SERVICE, TAF_ALL_DATA_TELESERVICES_TSCODE },
    { AT_CLCK_PARA_CLASS_FAX, TAF_SS_TELE_SERVICE, TAF_ALL_FACSIMILE_TRANSMISSION_SERVICES_TSCODE },
    { AT_CLCK_PARA_CLASS_FAX, TAF_SS_TELE_SERVICE, TAF_FACSIMILE_GROUP3_AND_ALTER_SPEECH_TSCODE },
    { AT_CLCK_PARA_CLASS_FAX, TAF_SS_TELE_SERVICE, TAF_AUTOMATIC_FACSIMILE_GROUP3_TSCODE },
    { AT_CLCK_PARA_CLASS_FAX, TAF_SS_TELE_SERVICE, TAF_FACSIMILE_GROUP4_TSCODE },
    { AT_CLCK_PARA_CLASS_VOICE_DATA_FAX, TAF_SS_TELE_SERVICE, TAF_ALL_TELESERVICES_EXEPTSMS_TSCODE },
    { AT_CLCK_PARA_CLASS_SMS, TAF_SS_TELE_SERVICE, TAF_ALL_SMS_SERVICES_TSCODE },
    { AT_CLCK_PARA_CLASS_SMS, TAF_SS_TELE_SERVICE, TAF_SMS_MT_PP_TSCODE },
    { AT_CLCK_PARA_CLASS_SMS, TAF_SS_TELE_SERVICE, TAF_SMS_MO_PP_TSCODE },
    { AT_CLCK_PARA_CLASS_VOICE_DATA_FAX_SMS, TAF_SS_TELE_SERVICE, TAF_ALL_TELESERVICES_TSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_DATA_CIRCUIT_SYNCHRONOUS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_DATACDS_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDS_1200BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDS_2400BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDS_4800BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDS_9600BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_ALTERNATE_SPEECH_DATACDS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_SPEECH_FOLLOWED_BY_DATACDS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC, TAF_SS_BEARER_SERVICE, TAF_GENERAL_DATACDS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_DATA_CIRCUIT_ASYNCHRONOUS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_DATACDA_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_300BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_1200BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_1200_75BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_2400BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_4800BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_DATACDA_9600BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_ALTERNATE_SPEECH_DATACDA_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC, TAF_SS_BEARER_SERVICE, TAF_ALL_SPEECH_FOLLOWED_BY_DATACDA_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PKT, TAF_SS_BEARER_SERVICE, TAF_ALL_DATAPDS_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PKT, TAF_SS_BEARER_SERVICE, TAF_DATAPDS_2400BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PKT, TAF_SS_BEARER_SERVICE, TAF_DATAPDS_4800BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PKT, TAF_SS_BEARER_SERVICE, TAF_DATAPDS_9600BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC_PKT, TAF_SS_BEARER_SERVICE, TAF_ALL_SYNCHRONOUS_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_ALL_PADACCESSCA_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_300BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_1200BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_1200_75BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_2400BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_4800BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_PAD, TAF_SS_BEARER_SERVICE, TAF_PADACCESSCA_9600BPS_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_ASYNC_PAD, TAF_SS_BEARER_SERVICE, TAF_ALL_ASYNCHRONOUS_SERVICES_BSCODE },
    { AT_CLCK_PARA_CLASS_DATA_SYNC_ASYNC_PKT_PKT, TAF_SS_BEARER_SERVICE, TAF_ALL_BEARERSERVICES_BSCODE },
};

static const AT_ChgTafErrCodeTbl g_atChgTafErrCodeTbl[] = {
    { TAF_ERR_GET_CSQLVL_FAIL, AT_ERROR },
    { TAF_ERR_USIM_SVR_OPLMN_LIST_INAVAILABLE, AT_ERROR },
    { TAF_ERR_TIME_OUT, AT_CME_NETWORK_TIMEOUT },
    { TAF_ERR_USIM_SIM_CARD_NOTEXIST, AT_CME_SIM_NOT_INSERTED },
    { TAF_ERR_NEED_PIN1, AT_CME_SIM_PIN_REQUIRED },
    { TAF_ERR_NEED_PUK1, AT_CME_SIM_PUK_REQUIRED },
    { TAF_ERR_SIM_FAIL, AT_CME_SIM_FAILURE },
    { TAF_ERR_PB_STORAGE_OP_FAIL, AT_CME_SIM_FAILURE },
    { TAF_ERR_UNSPECIFIED_ERROR, AT_CME_UNKNOWN },
    { TAF_ERR_PARA_ERROR, AT_CME_INCORRECT_PARAMETERS },
    { TAF_ERR_SS_NEGATIVE_PASSWORD_CHECK, AT_CME_INCORRECT_PASSWORD },
    { TAF_ERR_SIM_BUSY, AT_CME_SIM_BUSY },
    { TAF_ERR_SIM_LOCK, AT_CME_PH_SIM_PIN_REQUIRED },
    { TAF_ERR_SIM_INCORRECT_PASSWORD, AT_CME_INCORRECT_PASSWORD },
    { TAF_ERR_PB_NOT_FOUND, AT_CME_NOT_FOUND },
    { TAF_ERR_PB_DIAL_STRING_TOO_LONG, AT_CME_DIAL_STRING_TOO_LONG },
    { TAF_ERR_PB_STORAGE_FULL, AT_CME_MEMORY_FULL },
    { TAF_ERR_PB_WRONG_INDEX, AT_CME_INVALID_INDEX },
    { TAF_ERR_CMD_TYPE_ERROR, AT_CME_OPERATION_NOT_ALLOWED },
    { TAF_ERR_FILE_NOT_EXIST, AT_CME_FILE_NOT_EXISTS },
    { TAF_ERR_NO_NETWORK_SERVICE, AT_CME_NO_NETWORK_SERVICE },
    { TAF_ERR_AT_ERROR, AT_ERROR },
    { TAF_ERR_CME_OPT_NOT_SUPPORTED, AT_CME_OPERATION_NOT_SUPPORTED },
    { TAF_ERR_NET_SEL_MENU_DISABLE, AT_CME_NET_SEL_MENU_DISABLE },
    { TAF_ERR_SYSCFG_CS_IMS_SERV_EXIST, AT_CME_CS_IMS_SERV_EXIST },
    { TAF_ERR_NO_RF, AT_CME_NO_RF },
    { TAF_ERR_NEED_PUK2, AT_CME_SIM_PUK2_REQUIRED },
    { TAF_ERR_BUSY_ON_USSD, AT_CME_OPERATION_NOT_SUPPORTED },
    { TAF_ERR_BUSY_ON_SS, AT_CME_OPERATION_NOT_SUPPORTED },
    { TAF_ERR_SS_NET_TIMEOUT, AT_CME_NETWORK_TIMEOUT },
    { TAF_ERR_NO_SUCH_ELEMENT, AT_CME_NO_SUCH_ELEMENT },
    { TAF_ERR_MISSING_RESOURCE, AT_CME_MISSING_RESOURCE },
    { TAF_ERR_IMS_NOT_SUPPORT, AT_CME_IMS_NOT_SUPPORT },
    { TAF_ERR_IMS_SERVICE_EXIST, AT_CME_IMS_SERVICE_EXIST },
    { TAF_ERR_IMS_VOICE_DOMAIN_PS_ONLY, AT_CME_IMS_VOICE_DOMAIN_PS_ONLY },
    { TAF_ERR_IMS_STACK_TIMEOUT, AT_CME_IMS_STACK_TIMEOUT },
    { TAF_ERR_IMS_OPEN_LTE_NOT_SUPPORT, AT_CME_IMS_OPEN_LTE_NOT_SUPPORT },
    { TAF_ERR_1X_RAT_NOT_SUPPORTED, AT_CME_1X_RAT_NOT_SUPPORTED },
    { TAF_ERR_SILENT_AES_DEC_PIN_FAIL, AT_CME_SILENT_AES_DEC_PIN_ERROR },
    { TAF_ERR_SILENT_VERIFY_PIN_ERR, AT_CME_SILENT_VERIFY_PIN_ERROR },
    { TAF_ERR_SILENT_AES_ENC_PIN_FAIL, AT_CME_SILENT_AES_ENC_PIN_ERROR },
    { TAF_ERR_NOT_FIND_FILE, AT_CME_NOT_FIND_FILE },
    { TAF_ERR_NOT_FIND_NV, AT_CME_NOT_FIND_NV },
    { TAF_ERR_MODEM_ID_ERROR, AT_CME_MODEM_ID_ERROR },
    { TAF_ERR_NV_NOT_SUPPORT_ERR, AT_CME_NV_NOT_SUPPORT_ERR },
    { TAF_ERR_WRITE_NV_TIMEOUT, AT_CME_WRITE_NV_TimeOut },
    { TAF_ERR_NETWORK_FAILURE, AT_CME_NETWORK_FAILURE },
    { TAF_ERR_SCI_ERROR, AT_CME_SCI_ERROR },

    { TAF_ERR_EMAT_OPENCHANNEL_ERROR, AT_ERR_EMAT_OPENCHANNEL_ERROR },
    { TAF_ERR_EMAT_OPENCHANNEL_CNF_ERROR, AT_ERR_EMAT_OPENCHANNEL_CNF_ERROR },
    { TAF_ERR_EMAT_CLOSECHANNEL_ERROR, AT_ERR_EMAT_CLOSECHANNEL_ERROR },
    { TAF_ERR_EMAT_CLOSECHANNEL_CNF_ERROR, AT_ERR_EMAT_CLOSECHANNEL_CNF_ERROR },
    { TAF_ERR_EMAT_GETEID_ERROR, AT_ERR_EMAT_GETEID_ERROR },
    { TAF_ERR_EMAT_GETEID_DATA_ERROR, AT_ERR_EMAT_GETEID_DATA_ERROR },
    { TAF_ERR_EMAT_GETPKID_ERROR, AT_ERR_EMAT_GETPKID_ERROR },
    { TAF_ERR_EMAT_GETPKID_DATA_ERROR, AT_ERR_EMAT_GETPKID_DATA_ERROR },
    { TAF_ERR_EMAT_CLEANPROFILE_ERROR, AT_ERR_EMAT_CLEANPROFILE_ERROR },
    { TAF_ERR_EMAT_CLEANPROFILE_DATA_ERROR, AT_ERR_EMAT_CLEANPROFILE_DATA_ERROR },
    { TAF_ERR_EMAT_CHECKPROFILE_ERROR, AT_ERR_EMAT_CHECKPROFILE_ERROR },
    { TAF_ERR_EMAT_CHECKPROFILE_DATA_ERROR, AT_ERR_EMAT_CHECKPROFILE_DATA_ERROR },
    { TAF_ERR_EMAT_TPDU_CNF_ERROR, AT_ERR_EMAT_TPDU_CNF_ERROR },
    { TAF_ERR_EMAT_TPDU_DATASTORE_ERROR, AT_ERR_EMAT_TPDU_DATASTORE_ERROR },
    { TAF_ERR_ESIMSWITCH_SET_ERROR, AT_ERR_ESIMSWITCH_SET_ERROR },
    { TAF_ERR_ESIMSWITCH_SET_NOT_ENABLE_ERROR, AT_ERR_ESIMSWITCH_SET_NOT_ENABLE_ERROR },
    { TAF_ERR_ESIMSWITCH_QRY_ERROR, AT_ERR_ESIMSWITCH_QRY_ERROR },
    { TAF_ERR_CARRIER_MALLOC_FAIL, AT_ERR_CARRIER_MALLOC_FAIL },
    { TAF_ERR_CARRIER_READ_NV_ORIGINAL_DATA_ERR, AT_ERR_CARRIER_READ_NV_ORIGINAL_DATA_ERR },
    { TAF_ERR_CARRIER_FILE_LEN_ERROR, AT_ERR_CARRIER_FILE_LEN_ERROR },
    { TAF_ERR_CARRIER_NV_LEN_ERROR, AT_ERR_CARRIER_NV_LEN_ERROR },
    { TAF_ERR_CARRIER_WRITE_NV_FREE_FAIL, AT_ERR_CARRIER_WRITE_NV_FREE_FAIL },
    { TAF_ERR_CARRIER_NV_ERROR, AT_ERR_CARRIER_NV_ERROR },
    { TAF_ERR_CARRIER_HMAC_VERIFY_FAIL, AT_ERR_CARRIER_HMAC_VERIFY_FAIL},
};

static const AT_PIH_RspProcFunc g_atPihRspProcFuncTbl[] = {
    { SI_PIH_EVENT_FDN_CNF, sizeof(SI_PIH_EventFdnCnf), VOS_NULL_PTR, At_ProcPihFndBndCnf },
    { SI_PIH_EVENT_BDN_CNF, sizeof(SI_PIH_EventFdnCnf), VOS_NULL_PTR, At_ProcPihFndBndCnf },
    { SI_PIH_EVENT_GENERIC_ACCESS_CNF, sizeof(SI_PIH_CsimCommandCnf), VOS_NULL_PTR, At_ProcPihGenericAccessCnf },
    { SI_PIH_EVENT_ISDB_ACCESS_CNF, sizeof(SI_PIH_IsdbAccessCommandCnf), VOS_NULL_PTR, At_ProcPihIsdbAccessCnf },
    { SI_PIH_EVENT_CCHO_SET_CNF, sizeof(SI_PIH_OpenChannelCommandCnf), VOS_NULL_PTR, At_ProcPihCchoSetCnf },
    { SI_PIH_EVENT_CCHP_SET_CNF, sizeof(SI_PIH_OpenChannelCommandCnf), VOS_NULL_PTR, At_ProcPihCchpSetCnf },
    { SI_PIH_EVENT_CCHC_SET_CNF, 0, VOS_NULL_PTR, At_ProcPihCchcSetCnf },
    { SI_PIH_EVENT_SCICFG_SET_CNF, 0, VOS_NULL_PTR, At_ProcPihSciCfgSetCnf },
    { SI_PIH_EVENT_HVSST_SET_CNF, 0, VOS_NULL_PTR, At_ProcPihHvsstSetCnf },
    { SI_PIH_EVENT_CGLA_SET_CNF, sizeof(SI_PIH_CglaCommandCnf), VOS_NULL_PTR, At_ProcPihCglaSetCnf },
    { SI_PIH_EVENT_CARD_ATR_QRY_CNF, sizeof(SI_PIH_AtrQryCnf), VOS_NULL_PTR, At_ProcPihCardAtrQryCnf },
    { SI_PIH_EVENT_SCICFG_QUERY_CNF, sizeof(SI_PIH_SciCfgQueryCnf), VOS_NULL_PTR, At_SciCfgQueryCnf },
    { SI_PIH_EVENT_HVSST_QUERY_CNF, sizeof(SI_PIH_EventHvsstQueryCnf), VOS_NULL_PTR, At_HvsstQueryCnf },
    { SI_PIH_EVENT_CARDTYPE_QUERY_CNF, sizeof(SI_PIH_EventCardTypeQueryCnf), VOS_NULL_PTR, At_ProcPihCardTypeQryCnf },
    { SI_PIH_EVENT_CARDTYPEEX_QUERY_CNF, sizeof(SI_PIH_EventCardTypeQueryCnf), VOS_NULL_PTR, At_ProcPihCardTypeExQryCnf },
    { SI_PIH_EVENT_CARDVOLTAGE_QUERY_CNF, sizeof(SI_PIH_EventCardvoltageQueryCnf), VOS_NULL_PTR, At_ProcPihCardVoltageQryCnf },
    { SI_PIH_EVENT_PRIVATECGLA_SET_CNF, sizeof(SI_PIH_CglaHandleCnf), VOS_NULL_PTR, At_ProcPihPrivateCglaSetCnf },
    { SI_PIH_EVENT_CRSM_SET_CNF, sizeof(SI_PIH_RaccessCnf), VOS_NULL_PTR, At_ProcPihCrsmSetCnf },
    { SI_PIH_EVENT_CRLA_SET_CNF, sizeof(SI_PIH_RaccessCnf), VOS_NULL_PTR, At_ProcPihCrlaSetCnf },
    { SI_PIH_EVENT_SESSION_QRY_CNF, USIMM_CARDAPP_BUTT * sizeof(VOS_UINT32), VOS_NULL_PTR, At_ProcPihSessionQryCnf },
    { SI_PIH_EVENT_CIMI_QRY_CNF, sizeof(SI_PIH_EventImsi), VOS_NULL_PTR, At_ProcPihCimiQryCnf },
    { SI_PIH_EVENT_CCIMI_QRY_CNF, sizeof(SI_PIH_EventImsi), VOS_NULL_PTR, At_ProcPihCcimiQryCnf },
#if (FEATURE_IMS == FEATURE_ON)
    { SI_PIH_EVENT_UICCAUTH_CNF, sizeof(SI_PIH_UiccAuthCnf), VOS_NULL_PTR, AT_UiccAuthCnf },
    { SI_PIH_EVENT_URSM_CNF, sizeof(SI_PIH_UiccAccessfileCnf), VOS_NULL_PTR, AT_UiccAccessFileCnf },
#endif
#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))
    { SI_PIH_EVENT_SILENT_PIN_SET_CNF, sizeof(SI_PIH_CryptoPin), VOS_NULL_PTR, At_PrintSilentPinInfo },
    { SI_PIH_EVENT_SILENT_PININFO_SET_CNF, sizeof(SI_PIH_CryptoPin), VOS_NULL_PTR, At_PrintSilentPinInfo },
#endif

    { SI_PIH_EVENT_ESIMSWITCH_SET_CNF, 0, VOS_NULL_PTR, At_PrintSetEsimSwitchInfo },
    { SI_PIH_EVENT_ESIMSWITCH_QRY_CNF, sizeof(USIMM_SlotCardType), VOS_NULL_PTR, At_PrintQryEsimSwitchInfo },
    { SI_PIH_EVENT_BWT_SET_CNF, 0, VOS_NULL_PTR, At_ProcPihBwtSetCnf },
    { SI_PIH_EVENT_PRIVATECCHO_SET_CNF, sizeof(SI_PIH_OpenChannelCommandCnf), VOS_NULL_PTR, At_ProcPihPrivateCchoSetCnf },
    { SI_PIH_EVENT_PRIVATECCHP_SET_CNF, sizeof(SI_PIH_OpenChannelCommandCnf), VOS_NULL_PTR, At_ProcPihPrivateCchpSetCnf },
#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
    { SI_PIH_EVENT_NOCARD_SET_CNF, sizeof(VOS_UINT32), VOS_NULL_PTR,  AT_ProcNoCardSetCnf },
#endif
#endif
    { SI_PIH_EVENT_PASSTHROUGH_SET_CNF, 0, VOS_NULL_PTR, At_SetPassThroughCnf },
    { SI_PIH_EVENT_PASSTHROUGH_QUERY_CNF, sizeof(SI_PIH_PassThroughStateUint32), VOS_NULL_PTR, At_QryPassThroughCnf },
#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
    { SI_PIH_EVENT_SINGLEMODEMDUALSLOT_SET_CNF, 0, VOS_NULL_PTR, At_SetSingleModemDualSlotCnf },
    { SI_PIH_EVENT_SINGLEMODEMDUALSLOT_QUERY_CNF, sizeof(SI_PIH_CardSlotUint32), VOS_NULL_PTR, At_QrySingleModemDualSlotCnf },
#endif
};

static const AT_EmatRspProcFunc g_atEmatRspProcFuncTbl[] = {
    { SI_EMAT_EVENT_ESIM_CLEAN_CNF, 0, VOS_NULL_PTR, At_PrintEsimCleanProfileInfo },
    { SI_EMAT_EVENT_ESIM_CHECK_CNF,  sizeof(SI_EMAT_EventEsimCheckCnf), VOS_NULL_PTR, At_PrintEsimCheckProfileInfo },
    { SI_EMAT_EVENT_GET_ESIMEID_CNF, sizeof(SI_EMAT_EventEsimEidCnf),   VOS_NULL_PTR, At_PrintGetEsimEidInfo },
    { SI_EMAT_EVENT_GET_ESIMPKID_CNF, sizeof(SI_EMAT_EventEsimPkidCnf), VOS_NULL_PTR, At_PrintGetEsimPKIDInfo },
};

/*
 * 5 函数、变量声明
 */

LOCAL VOS_VOID AT_ConvertSysCfgStrToAutoModeStr(VOS_UINT8 *acqOrderBegin, VOS_UINT8 *acqOrder,
                                                VOS_UINT32 acqOrderLength, VOS_UINT8 ratNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
LOCAL VOS_VOID AT_RcvMmaNrCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                          VOS_UINT8 systemAppConfig);
#endif

#if (FEATURE_LTE == FEATURE_ON)
LOCAL VOS_VOID AT_RcvMmaLteCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                           VOS_UINT8 systemAppConfig);
#endif

LOCAL VOS_VOID AT_RcvMmaWcdmaCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                             VOS_UINT8 systemAppConfig);

LOCAL VOS_VOID AT_RcvMmaGsmCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                           VOS_UINT8 systemAppConfig);

LOCAL VOS_CHAR* AT_ConvertRatModeForQryParaPlmnList(TAF_PH_RA_MODE raMode);


AT_CliValidityUint8 AT_ConvertCLIValidity(TAF_CCM_CallIncomingInd *incomingInd)
{
    AT_CliValidityUint8 cliVality;
    /* 24008 10.5.4.30 */
    /*
     * Cause of No CLI information element provides the mobile station
     * the detailed reason why Calling party BCD nuber is not notified.
     */

    /* 号码长度不为0，则认为显示号码有效 */
    if (incomingInd->incomingIndPara.callNumber.numLen != 0) {
        return AT_CLI_VALIDITY_VALID;
    }

    switch (incomingInd->incomingIndPara.noCliCause) {
        case MN_CALL_NO_CLI_USR_REJ:
        case MN_CALL_NO_CLI_INTERACT:
        case MN_CALL_NO_CLI_PAYPHONE:
            cliVality = (AT_CliValidityUint8)incomingInd->incomingIndPara.noCliCause;
            break;

        /* 原因值不存在,号码合法或未提供 */
        case MN_CALL_NO_CLI_BUTT:
        case MN_CALL_NO_CLI_UNAVAL:
        default:
            cliVality = AT_CLI_VALIDITY_UNAVAL;
            break;
    }

    return cliVality;
}

VOS_UINT32 AT_CheckRptCmdStatus(VOS_UINT8 *rptCfg, AT_CmdRptCtrlTypeUint8 rptCtrlType, AT_RptCmdIndexUint8 rptCmdIndex)
{
    const AT_RptCmdIndexUint8 *rptCmdTblPtr = VOS_NULL_PTR;
    VOS_UINT32           rptCmdTblSize;
    VOS_UINT32           tableIndex;
    VOS_UINT32           offset;
    VOS_UINT8            bit;

    /* 主动上报命令索引错误，默认主动上报 */
    if (rptCmdIndex >= AT_RPT_CMD_BUTT) {
        return VOS_TRUE;
    }

    /* 主动上报受控类型填写错误，默认主动上报 */
    if (rptCtrlType == AT_CMD_RPT_CTRL_BUTT) {
        return VOS_TRUE;
    }

    if (rptCtrlType == AT_CMD_RPT_CTRL_BY_CURC) {
        rptCmdTblPtr  = AT_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_PTR();
        rptCmdTblSize = AT_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_SIZE();
    } else {
        rptCmdTblPtr  = AT_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_PTR();
        rptCmdTblSize = AT_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_SIZE();
    }

    for (tableIndex = 0; tableIndex < rptCmdTblSize; tableIndex++) {
        if (rptCmdIndex == rptCmdTblPtr[tableIndex]) {
            break;
        }
    }

    /* 与全局变量中的Bit位对比 */
    if (rptCmdTblSize != tableIndex) {
        /* 由于用户设置的字节序与Bit映射表序相反, 首先反转Bit位 */
        offset = AT_CURC_RPT_CFG_MAX_SIZE - ((VOS_UINT32)tableIndex / AT_BYTE_TO_BITS_LEN) - 1;
        bit    = (VOS_UINT8)(tableIndex % AT_BYTE_TO_BITS_LEN);

        return (VOS_UINT32)((rptCfg[offset] >> bit) & 0x1);
    }

    return VOS_TRUE;
}


VOS_UINT32 At_ChgMnErrCodeToAt(VOS_UINT8 indexNum, VOS_UINT32 mnErrorCode)
{
    VOS_UINT32            rtn;
    const AT_CMS_SmsErrCodeMap *smsErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32            smsErrMapTblSize;
    VOS_UINT32            cnt;
    AT_ModemSmsCtx       *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    smsErrMapTblPtr  = AT_GET_CMS_SMS_ERR_CODE_MAP_TBL_PTR();
    smsErrMapTblSize = AT_GET_CMS_SMS_ERR_CODE_MAP_TBL_SIZE();

    rtn = AT_CMS_UNKNOWN_ERROR;

    for (cnt = 0; cnt < smsErrMapTblSize; cnt++) {
        if (smsErrMapTblPtr[cnt].smsCause == mnErrorCode) {
            rtn = smsErrMapTblPtr[cnt].cmsCode;

            if ((smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) && (rtn == AT_CMS_INVALID_PDU_MODE_PARAMETER)) {
                rtn = AT_CMS_INVALID_TEXT_MODE_PARAMETER;
            }

            break;
        }
    }

    return rtn;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

TAF_UINT32 At_ChgXsmsErrorCodeToAt(TAF_UINT32 xsmsError)
{
    switch (xsmsError) {
        case TAF_XSMS_APP_SUCCESS:
            return AT_SUCCESS;

        case TAF_XSMS_APP_FAILURE:
            return AT_ERROR;

        case TAF_XSMS_APP_INIT_NOT_FINISH:
            return AT_CME_SIM_BUSY;

        case TAF_XSMS_APP_INVALID_INDEX:
            return AT_CME_INVALID_INDEX;

        case TAF_XSMS_APP_UIM_FAILURE:
            return AT_CME_SIM_FAILURE;

        case TAF_XSMS_APP_STORE_FULL:
            return AT_CMS_SIM_SMS_STORAGE_FULL;

        case TAF_XSMS_APP_NOT_SUPPORT_1X:
            return AT_CME_1X_RAT_NOT_SUPPORTED;

        case TAF_XSMS_APP_NORMAL_VOLTE:
        case TAF_XSMS_APP_UIM_LOCK_LOCKED:
        case TAF_XSMS_APP_ENCODE_ERROR:
        case TAF_XSMS_APP_UIM_MSG_STATUS_WRONG:
        case TAF_XSMS_APP_INSERT_SDND_POOL_FAIL:
        case TAF_XSMS_APP_NOT_INSERT_TO_POOL:
        case TAF_XSMS_APP_NOT_SUPPORT_LTE:
            return AT_ERROR;

        default:
            return AT_ERROR;
    }
}
#endif

TAF_UINT32 At_ChgTafErrorCode(TAF_UINT8 indexNum, TAF_ERROR_CodeUint32 tafErrorCode)
{
    TAF_UINT32 rtn = 0;
    VOS_UINT32 msgCnt;
    VOS_UINT32 i;
    VOS_UINT32 flag = 0;

    /* 获取消息个数 */
    msgCnt = sizeof(g_atChgTafErrCodeTbl) / sizeof(AT_ChgTafErrCodeTbl);

    for (i = 0; i < msgCnt; i++) {
        if (g_atChgTafErrCodeTbl[i].tafErrCode == tafErrorCode) {
            rtn  = g_atChgTafErrCodeTbl[i].atReturnCode;
            flag = flag + 1;
            break;
        }
    }

    if (flag == 0) {
        if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
            rtn = AT_CME_UNKNOWN;
        } else if (g_parseContext[indexNum].cmdElement == VOS_NULL_PTR) {
            rtn = AT_CME_UNKNOWN;
        } else if ((g_parseContext[indexNum].cmdElement->cmdIndex > AT_CMD_SMS_BEGAIN) &&
                   (g_parseContext[indexNum].cmdElement->cmdIndex < AT_CMD_SMS_END)) {
            rtn = AT_CMS_UNKNOWN_ERROR;
        } else {
            rtn = AT_CME_UNKNOWN;
        }
    }

    return rtn;
}

/*
 * Description: 把CCA返回的CLASS以字符串方式输出，注意，不完整
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_CcClass2Print(MN_CALL_TypeUint8 callType, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_PSAP_ECALL:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst, "VOICE");
            break;

        case MN_CALL_TYPE_FAX:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst, "FAX");
            break;

        case MN_CALL_TYPE_VIDEO:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst, "SYNC");
            break;

        case MN_CALL_TYPE_CS_DATA:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst, "DATA");
            break;

        default:
            AT_WARN_LOG("At_CcClass2Print CallType ERROR");
            break;
    }

    return length;
}

/* PC工程中AT从A核移到C核, At_sprintf有重复定义,故在此处添加条件编译宏 */


TAF_UINT32 At_HexAlpha2AsciiString(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                   TAF_UINT16 srcLen)
{
    TAF_UINT16 len    = 0;
    TAF_UINT16 chkLen = 0;
    TAF_UINT8 *write  = dst;
    TAF_UINT8 *read   = src;
    TAF_UINT8  high   = 0;
    TAF_UINT8  low    = 0;

    if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + (AT_ASCII_AND_HEX_CONVERSION_FACTOR * srcLen)) >= maxLength) {
        AT_ERR_LOG("At_HexAlpha2AsciiString too long");
        return 0;
    }

    if (srcLen != 0) {
        /* 扫完整个字串 */
        while (chkLen++ < srcLen) {
            high = 0x0F & (*read >> 4);
            low  = 0x0F & *read;

            len += 2; /* 记录长度,高位和低位长度为2 */

            if (high <= 0x09) { /* 0-9 */
                *write++ = high + 0x30;
            } else if (high >= 0x0A) { /* A-F */
                *write++ = high + 0x37;
            } else {
            }

            if (low <= 0x09) { /* 0-9 */
                *write++ = low + 0x30;
            } else if (low >= 0x0A) { /* A-F */
                *write++ = low + 0x37;
            } else {
            }

            /* 下一个字符 */
            read++;
        }
    }
    return len;
}


VOS_UINT32 AT_Hex2AsciiStrLowHalfFirst(VOS_UINT32 maxLength, VOS_INT8 *pcHeadaddr, VOS_UINT8 *dst, VOS_UINT8 *src,
                                       VOS_UINT16 srcLen)
{
    VOS_UINT16 len;
    VOS_UINT16 chkLen;
    VOS_UINT8 *pcWrite = VOS_NULL_PTR;
    VOS_UINT8 *pcRead  = VOS_NULL_PTR;
    VOS_UINT8  high;
    VOS_UINT8  low;

    len     = 0;
    chkLen  = 0;
    pcWrite = dst;
    pcRead  = src;

    if (((VOS_UINT32)(dst - (VOS_UINT8 *)pcHeadaddr) + (AT_ASCII_AND_HEX_CONVERSION_FACTOR * srcLen)) >= maxLength) {
        AT_ERR_LOG("AT_Hex2AsciiStrLowHalfFirst too long");
        return 0;
    }

    if (srcLen != 0) {
        /* 扫完整个字串 */
        while (chkLen++ < srcLen) {
            high = 0x0F & (*pcRead >> 4);
            low  = 0x0F & *pcRead;

            len += 2; /* 记录长度,高位和低位长度为2 */

            /* 先转换低半字节 */
            if (low <= 0x09) { /* 0-9 */
                *pcWrite++ = low + 0x30;
            } else if (low >= 0x0A) { /* A-F */
                *pcWrite++ = low + 0x37;
            } else {
            }

            /* 再转换高半字节 */
            if (high <= 0x09) { /* 0-9 */
                *pcWrite++ = high + 0x30;
            } else if (high >= 0x0A) { /* A-F */
                *pcWrite++ = high + 0x37;
            } else {
            }

            /* 下一个字符 */
            pcRead++;
        }
    }

    return len;
}

/*
 * Description: 读取ASCII类型的号码
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_ReadNumTypePara(TAF_UINT8 *dst, TAF_UINT8 *src)
{
    TAF_UINT16 length = 0;

    if (g_atCscsType == AT_CSCS_UCS2_CODE) { /* +CSCS:UCS2 */
        TAF_UINT16 srcLen = (TAF_UINT16)VOS_StrLen((TAF_CHAR *)src);

        length += (TAF_UINT16)At_Ascii2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length, src,
                                                    srcLen);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, "%s", src);
    }
    return length;
}


VOS_VOID AT_CsRspEvtReleasedProc(TAF_UINT8 indexNum, TAF_CCM_CallReleasedInd *callReleaseInd)
{
    TAF_UINT32     result = AT_FAILURE;
    TAF_UINT16     length = 0;
    VOS_UINT32     timerName;
    AT_ModemCcCtx *ccCtx = VOS_NULL_PTR;

    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /* 停止RING TE */
    AT_VoiceStopRingTe(callReleaseInd->releaseIndPara.callId);
#endif

    /* 记录cause值和文本信息 */
    AT_UpdateCallErrInfo(indexNum, callReleaseInd->releaseIndPara.cause, &(callReleaseInd->releaseIndPara.errInfoText));

    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CHUP_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_H_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CHLD_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CTFR_SET)) {
        if (ccCtx->s0TimeInfo.timerStart == VOS_TRUE) {
            timerName = ccCtx->s0TimeInfo.timerName;

            AT_StopRelTimer(timerName, &(ccCtx->s0TimeInfo.s0Timer));
            ccCtx->s0TimeInfo.timerStart = VOS_FALSE;
            ccCtx->s0TimeInfo.timerName  = 0;
        }

        AT_ReportCendResult(indexNum, callReleaseInd);

        return;
    } else {
        /*
         * 需要增加来电类型，传真，数据，可视电话，语音呼叫
         */

        /* g_atClientTab[ucIndex].ulCause没有使用点，赋值点删除 */

        if (ccCtx->s0TimeInfo.timerStart == VOS_TRUE) {
            timerName = ccCtx->s0TimeInfo.timerName;

            AT_StopRelTimer(timerName, &(ccCtx->s0TimeInfo.s0Timer));
            ccCtx->s0TimeInfo.timerStart = VOS_FALSE;
            ccCtx->s0TimeInfo.timerName  = 0;
        }

        /* 上报CEND，可视电话不需要上报^CEND */
        if ((At_CheckReportCendCallType(callReleaseInd->releaseIndPara.callType) == PS_TRUE) ||
            (AT_EVT_IS_PS_VIDEO_CALL(callReleaseInd->releaseIndPara.callType,
                                     callReleaseInd->releaseIndPara.voiceDomain))) {
            AT_ReportCendResult(indexNum, callReleaseInd);

            return;
        }

        result = AT_NO_CARRIER;

        if (AT_EVT_IS_VIDEO_CALL(callReleaseInd->releaseIndPara.callType)) {
            if (callReleaseInd->releaseIndPara.cause == TAF_CS_CAUSE_CC_NW_USER_ALERTING_NO_ANSWER) {
                result = AT_NO_ANSWER;
            }

            if (callReleaseInd->releaseIndPara.cause == TAF_CS_CAUSE_CC_NW_USER_BUSY) {
                result = AT_BUSY;
            }
        }

        /* AT命令触发的话，需要清除相应的状态变量 */
        if (AT_EVT_REL_IS_NEED_CLR_TIMER_STATUS_CMD(g_atClientTab[indexNum].cmdCurrentOpt)) {
            AT_STOP_TIMER_CMD_READY(indexNum);
        }
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


VOS_UINT32 AT_RcvTafCcmCallConnectInd(struct MsgCB *msg)
{
    TAF_CCM_CallConnectInd *connectIndMsg = VOS_NULL_PTR;
    TAF_UINT32              result        = AT_FAILURE;
    TAF_UINT16              length        = 0;
    TAF_UINT8               asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    AT_ModemSsCtx          *ssCtx = VOS_NULL_PTR;
    ModemIdUint16           modemId;
    VOS_UINT32              rslt;
    MN_CALL_TypeUint8       newCallType;
    VOS_UINT8               indexNum = 0;
    VOS_UINT8               bcdNumLen;

    modemId = MODEM_ID_0;

    connectIndMsg = (TAF_CCM_CallConnectInd *)msg;

    if (At_ClientIdToUserId(connectIndMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_CsRspEvtConnectProc At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /* 停止RING TE */
    AT_VoiceStopRingTe(connectIndMsg->connectIndPara.callId);
#endif

    /* CS呼叫成功, 清除CS域错误码和文本信息 */
    AT_UpdateCallErrInfo(indexNum, TAF_CS_CAUSE_SUCCESS, &(connectIndMsg->connectIndPara.errInfoText));

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_CsRspEvtConnectProc: Get modem id fail.");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        newCallType = MN_CALL_TYPE_VOICE;
        At_ChangeEcallTypeToCallType(connectIndMsg->connectIndPara.callType, &newCallType);

        if (AT_CheckRptCmdStatus(connectIndMsg->connectIndPara.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CONN) ==
            VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s^CONN:%d,%d%s", g_atCrLf,
                connectIndMsg->connectIndPara.callId, newCallType, g_atCrLf);
            At_SendResultData(indexNum, g_atSndCodeAddress, length);
        }

        AT_ReportCCallstateResult(modemId, connectIndMsg->connectIndPara.callId,
                                  connectIndMsg->connectIndPara.curcRptCfg, AT_CS_CALL_STATE_CONNECT,
                                  connectIndMsg->connectIndPara.voiceDomain);

        return VOS_OK;
    }

    ssCtx = AT_GetModemSsCtxAddrFromModemId(modemId);

    /* 需要判断来电类型，如VOICE或者DATA */
    if (connectIndMsg->connectIndPara.callDir == MN_CALL_DIR_MO) {
        if (ssCtx->colpType == AT_COLP_ENABLE_TYPE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s+COLP: ", g_atCrLf);
            if (connectIndMsg->connectIndPara.connectNumber.numLen != 0) {
                bcdNumLen = TAF_MIN(connectIndMsg->connectIndPara.connectNumber.numLen, MN_CALL_MAX_BCD_NUM_LEN);
                AT_BcdNumberToAscii(connectIndMsg->connectIndPara.connectNumber.bcdNum,
                                    bcdNumLen, (VOS_CHAR *)asciiNum);
                length +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",%d,\"\",,\"\"", asciiNum,
                        (connectIndMsg->connectIndPara.connectNumber.numType | AT_NUMBER_TYPE_EXT));
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",,\"\",,\"\"");
            }
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            At_SendResultData(indexNum, g_atSndCodeAddress, length);
            length = 0;
        }
    }

    /* Video下，通过At_FormatResultData来上报CONNECT */
    if (AT_EVT_IS_VIDEO_CALL(connectIndMsg->connectIndPara.callType)) {
        /* IMS Video不给上层报CONNECT，上报^CONN */
        if (connectIndMsg->connectIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            if (AT_CheckRptCmdStatus(connectIndMsg->connectIndPara.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC,
                                     AT_RPT_CMD_CONN) == VOS_TRUE) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s^CONN:%d,%d%s", g_atCrLf,
                    connectIndMsg->connectIndPara.callId, connectIndMsg->connectIndPara.callType, g_atCrLf);

                At_SendResultData(indexNum, g_atSndCodeAddress, length);
            }

            AT_ReportCCallstateResult(modemId, connectIndMsg->connectIndPara.callId,
                                      connectIndMsg->connectIndPara.curcRptCfg, AT_CS_CALL_STATE_CONNECT,
                                      connectIndMsg->connectIndPara.voiceDomain);

            return VOS_OK;
        }

        g_atClientTab[indexNum].csRabId = connectIndMsg->connectIndPara.rabId;
        result                          = AT_CONNECT;
    } else {
        newCallType = MN_CALL_TYPE_VOICE;
        At_ChangeEcallTypeToCallType(connectIndMsg->connectIndPara.callType, &newCallType);

        if (AT_CheckRptCmdStatus(connectIndMsg->connectIndPara.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CONN) ==
            VOS_TRUE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "^CONN:%d", connectIndMsg->connectIndPara.callId);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", newCallType);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            At_SendResultData(indexNum, g_atSndCodeAddress, length);
        }
        AT_ReportCCallstateResult(modemId, connectIndMsg->connectIndPara.callId,
                                  connectIndMsg->connectIndPara.curcRptCfg, AT_CS_CALL_STATE_CONNECT,
                                  connectIndMsg->connectIndPara.voiceDomain);

        return VOS_OK;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    AT_ReportCCallstateResult(modemId, connectIndMsg->connectIndPara.callId, connectIndMsg->connectIndPara.curcRptCfg,
                              AT_CS_CALL_STATE_CONNECT, connectIndMsg->connectIndPara.voiceDomain);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallOrigInd(struct MsgCB *msg)
{
    AT_ModemCcCtx       *ccCtx   = VOS_NULL_PTR;
    TAF_CCM_CallOrigInd *origInd = VOS_NULL_PTR;
    ModemIdUint16        modemId;
    VOS_UINT32           rslt;
    TAF_UINT16           length;
    VOS_UINT32           checkRptCmdStatusResult;
    MN_CALL_TypeUint8    newCallType;
    TAF_UINT8            indexNum;

    length  = 0;
    modemId = MODEM_ID_0;

    indexNum = 0;
    origInd  = (TAF_CCM_CallOrigInd *)msg;

    if (At_ClientIdToUserId(origInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvMnCallSetCssnCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_CsRspEvtOrigProc: Get modem id fail.");
        return VOS_ERR;
    }

    ccCtx = AT_GetModemCcCtxAddrFromModemId(modemId);

    /* 可视电话里面，这里不能上报^ORIG ，因此只有普通语音和紧急呼叫的情况下，才上报^ORIG */
    checkRptCmdStatusResult = AT_CheckRptCmdStatus(origInd->origIndPara.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC,
                                                   AT_RPT_CMD_ORIG);
    newCallType             = MN_CALL_TYPE_VOICE;
    At_ChangeEcallTypeToCallType(origInd->origIndPara.callType, &newCallType);

    if (((At_CheckReportOrigCallType(newCallType) == PS_TRUE) ||
         (AT_EVT_IS_PS_VIDEO_CALL(origInd->origIndPara.callType, origInd->origIndPara.voiceDomain))) &&
        (checkRptCmdStatusResult == VOS_TRUE) && (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum))) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "^ORIG:%d", origInd->origIndPara.callId);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", newCallType);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    /* 发起呼叫后收到回复的OK后，将当前是否存在呼叫标志置为TRUE */
    ccCtx->curIsExistCallFlag = VOS_TRUE;

    AT_ReportCCallstateResult(modemId, origInd->origIndPara.callId, origInd->origIndPara.curcRptCfg,
                              AT_CS_CALL_STATE_ORIG, origInd->origIndPara.voiceDomain);

    return VOS_OK;
}


VOS_UINT8 At_GetSsCode(MN_CALL_SsNotifyCodeUint8 code, MN_CALL_StateUint8 callState)
{
    switch (code) {
        case MN_CALL_SS_NTFY_FORWORDED_CALL:
            return 0;

        case MN_CALL_SS_NTFY_MT_CUG_INFO:
            return 1;

        case MN_CALL_SS_NTFY_ON_HOLD:
            return AT_SS_NTFY_ON_HOLD_RTN_NUM;

        case MN_CALL_SS_NTFY_RETRIEVED:
            return AT_SS_NTFY_RETRIEVED_RTN_NUM;

        case MN_CALL_SS_NTFY_ENTER_MPTY:
            return AT_SS_NTFY_ENTER_MPTY_RTN_NUM;

        case MN_CALL_SS_NTFY_HOLD_RELEASED:
            return AT_SS_NTFY_HOLD_RELEASED_RTN_NUM;

        case MN_CALL_SS_NTFY_DEFLECTED_CALL:
            return AT_SS_NTFY_DEFLECTED_CALL_RTN_NUM;

        case MN_CALL_SS_NTFY_INCALL_FORWARDED:
            return AT_SS_NTFY_INCALL_FORWARDED_RTN_NUM;

        case MN_CALL_SS_NTFY_EXPLICIT_CALL_TRANSFER:
            if (callState == MN_CALL_S_ALERTING) {
                return AT_SS_NTFY_EXPLICIT_CALL_S_ALERTING_RTN_NUM;
            }
            return AT_SS_NTFY_EXPLICIT_CALL_TRANSFER_RTN_NUM;

        case MN_CALL_SS_NTFY_CCBS_BE_RECALLED:
            return 0x16;

        default:
            return 0xFF;
    }
}


VOS_UINT8 At_GetCssiForwardCauseCode(MN_CALL_CfCauseUint8 code)
{
    switch (code) {
        case MN_CALL_CF_CAUSE_ALWAYS:
            return AT_CF_CAUSE_ALWAYS_RTN_NUM;

        case MN_CALL_CF_CAUSE_BUSY:
            return 1;

        case MN_CALL_CF_CAUSE_POWER_OFF:
            return AT_CF_CAUSE_POWER_OFF_RTN_NUM;

        case MN_CALL_CF_CAUSE_NO_ANSWER:
            return AT_CF_CAUSE_NO_ANSWER_RTN_NUM;

        case MN_CALL_CF_CAUSE_SHADOW_ZONE:
            return AT_CF_CAUSE_SHADOW_ZONE_RTN_NUM;

        case MN_CALL_CF_CAUSE_DEFLECTION_480:
            return AT_CF_CAUSE_DEFLECTION_480_RTN_NUM;

        case MN_CALL_CF_CAUSE_DEFLECTION_487:
            return AT_CF_CAUSE_DEFLECTION_487_RTN_NUM;

        default:
            AT_ERR_LOG1("At_GetCssiFormardCauseCode: enCode is fail, enCode is ", code);
            return 0xFF;
    }
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_VOID At_ProcCsEvtCssuexNotifiy_Ims(TAF_CCM_CallSsInd *callSsInd, VOS_UINT8 code, VOS_UINT16 *length)
{
    VOS_CHAR asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];

    memset_s(asciiNum, (VOS_UINT32)sizeof(asciiNum), 0, (VOS_UINT32)sizeof(asciiNum));

    /* ^CSSUEX: <code2>,[<index>],<callId>[,<number>,<type>[,<forward_cause>]] */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s^CSSUEX: ", g_atCrLf);

    /* <code2>, */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", code);

    /* [index], */
    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MT_CUG_INFO) {
        /* <index> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", callSsInd->ssIndPara.ssNotify.cugIndex);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    /* <callId> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", callSsInd->ssIndPara.callId);

    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MT_CUG_INFO) {
        if (callSsInd->ssIndPara.callNumber.numLen != 0) {
            AT_BcdNumberToAscii(callSsInd->ssIndPara.callNumber.bcdNum, callSsInd->ssIndPara.callNumber.numLen,
                                asciiNum);

            /* ,<number> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", asciiNum);

            /* ,<type> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d",
                (callSsInd->ssIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT));
        }
    }

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
}

VOS_VOID At_ProcCsEvtCssuNotifiy_Ims(TAF_CCM_CallSsInd *callSsInd, VOS_UINT8 code, VOS_UINT16 *length)
{
    VOS_CHAR asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];

    memset_s(asciiNum, (VOS_UINT32)sizeof(asciiNum), 0, (VOS_UINT32)sizeof(asciiNum));

    /* ^CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s^CSSU: ", g_atCrLf);

    /* <code2> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", code);

    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MT_CUG_INFO) {
        /* ,<index> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ssNotify.cugIndex);

        if (callSsInd->ssIndPara.callNumber.numLen != 0) {
            AT_BcdNumberToAscii(callSsInd->ssIndPara.callNumber.bcdNum, callSsInd->ssIndPara.callNumber.numLen,
                                asciiNum);

            /* ,<number> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", asciiNum);

            /* ,<type> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d",
                (callSsInd->ssIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT));
        }
    }

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);

    At_ProcCsEvtCssuexNotifiy_Ims(callSsInd, code, length);
}


VOS_VOID At_ProcCsEvtCssiNotifiy_Ims(TAF_CCM_CallSsInd *callSsInd, VOS_UINT16 *length)
{
    VOS_CHAR  asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    VOS_UINT8 forwardCauseCode;

    memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));
    forwardCauseCode = 0xFF;

    /* ^CSSI: <code1>,<index>,<callId>[,<number>,<type>[,<forward_cause>]] */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s^CSSI: ", g_atCrLf);

    /* <code1>, */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", callSsInd->ssIndPara.ssNotify.code);

    /* [index], */
    if ((callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MO_CUG_INFO) ||
        (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_BE_FORWORDED)) {
        /* ,<index> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", callSsInd->ssIndPara.ssNotify.cugIndex);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    /* <callId> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", callSsInd->ssIndPara.callId);

    if ((callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MO_CUG_INFO) ||
        (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_BE_FORWORDED)) {
        /* 被连号码显示 */
        if (callSsInd->ssIndPara.connectNumber.numLen != 0) {
            (VOS_VOID)AT_BcdNumberToAscii(callSsInd->ssIndPara.connectNumber.bcdNum,
                                          callSsInd->ssIndPara.connectNumber.numLen, asciiNum);

            /* ,<number> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", asciiNum);

            /* ,<type> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d",
                (callSsInd->ssIndPara.connectNumber.numType | AT_NUMBER_TYPE_EXT));

            /* [,<forward_cause>] */
            forwardCauseCode = At_GetCssiForwardCauseCode(callSsInd->ssIndPara.callForwardCause);

            if (forwardCauseCode != 0xFF) {
                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", forwardCauseCode);
            }
        }
    }

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
}


VOS_VOID At_ProcCsEvtImsHoldToneNotifiy_Ims(TAF_CCM_CallSsInd *callSsInd, VOS_UINT16 *length)
{
    /* ^IMSHOLDTONE: <hold_tone> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s: ", g_atCrLf, g_atStringTab[AT_STRING_IMS_HOLD_TONE].text);

    /* <hold_tone> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", callSsInd->ssIndPara.holdToneType);

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
}
#endif


VOS_VOID At_ProcCsEvtCssuNotifiy_NonIms(TAF_CCM_CallSsInd *callSsInd, VOS_UINT8 code, VOS_UINT16 *length)
{
    VOS_CHAR asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];

    memset_s(asciiNum, (VOS_UINT32)sizeof(asciiNum), 0, (VOS_UINT32)sizeof(asciiNum));

    /* +CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s+CSSU: ", g_atCrLf);

    /* <code2> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", code);

    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MT_CUG_INFO) {
        /* ,<index> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ssNotify.cugIndex);

        if (callSsInd->ssIndPara.callNumber.numLen != 0) {
            AT_BcdNumberToAscii(callSsInd->ssIndPara.callNumber.bcdNum, callSsInd->ssIndPara.callNumber.numLen,
                                asciiNum);

            /* ,<number> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", asciiNum);

            /* ,<type> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d",
                (callSsInd->ssIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT));
        }
    }

    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_EXPLICIT_CALL_TRANSFER) {
        /* ,<index> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ssNotify.cugIndex);

        if (callSsInd->ssIndPara.ssNotify.ectIndicator.rdn.presentationAllowedAddr.numLen != 0) {
            AT_BcdNumberToAscii(callSsInd->ssIndPara.ssNotify.ectIndicator.rdn.presentationAllowedAddr.bcdNum,
                                callSsInd->ssIndPara.ssNotify.ectIndicator.rdn.presentationAllowedAddr.numLen,
                                asciiNum);

            /* ,<number> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", asciiNum);

            /* ,<type> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d",
                (callSsInd->ssIndPara.ssNotify.ectIndicator.rdn.presentationAllowedAddr.numType | AT_NUMBER_TYPE_EXT));
        }
    }

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
}


VOS_VOID At_ProcCsEvtCssiNotifiy_NonIms(TAF_CCM_CallSsInd *callSsInd, VOS_UINT16 *length)
{
    /* +CSSI: <code1>[,<index>] */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s+CSSI: ", g_atCrLf);

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d", callSsInd->ssIndPara.ssNotify.code);

    if (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_MO_CUG_INFO) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ssNotify.cugIndex);
    }

    if ((callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_CCBS_RECALL) &&
        (callSsInd->ssIndPara.ccbsFeature.opCcbsIndex == MN_CALL_OPTION_EXIST)) {
        *length += (VOS_UINT16)At_CcClass2Print(callSsInd->ssIndPara.callType, g_atSndCodeAddress + *length);

        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);

        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ccbsFeature.ccbsIndex);

        if (callSsInd->ssIndPara.ccbsFeature.opBSubscriberNum == MN_CALL_OPTION_EXIST) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%s", callSsInd->ssIndPara.ccbsFeature.bsubscriberNum);
        }

        if (callSsInd->ssIndPara.ccbsFeature.opNumType == MN_CALL_OPTION_EXIST) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", callSsInd->ssIndPara.ccbsFeature.numType);
        }
    }

    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
}



VOS_VOID At_ProcCsRspEvtCssuNotifiy(VOS_UINT8 indexNum, TAF_CCM_CallSsInd *callSsInd, VOS_UINT16 *length)
{
    VOS_UINT8  code;
    VOS_UINT16 lengthTemp;
    VOS_UINT32 cssuRptStatus;

    lengthTemp = *length;

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        return;
    }

    cssuRptStatus = AT_CheckRptCmdStatus(callSsInd->ssIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                         AT_RPT_CMD_CSSU);

    /* +CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] */
    if (((cssuRptStatus == VOS_TRUE) && (callSsInd->ssIndPara.ssNotify.code > MN_CALL_SS_NTFY_BE_DEFLECTED) &&
         (callSsInd->ssIndPara.ssNotify.code != MN_CALL_SS_NTFY_CCBS_RECALL)) &&
        ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_D_CS_VOICE_CALL_SET) &&
         (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_D_CS_DATA_CALL_SET) &&
         (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_APDS_SET))) {
        code = At_GetSsCode(callSsInd->ssIndPara.ssNotify.code, callSsInd->ssIndPara.callState);

        if (code == 0xFF) {
            AT_ERR_LOG("At_ProcCsRspEvtCssuNotifiy: code error.");
            return;
        }

#if (FEATURE_IMS == FEATURE_ON)
        if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            At_ProcCsEvtCssuNotifiy_Ims(callSsInd, code, &lengthTemp);
        } else
#endif
        {
            At_ProcCsEvtCssuNotifiy_NonIms(callSsInd, code, &lengthTemp);
        }
    }

    *length = lengthTemp;

    return;
}


VOS_VOID At_ProcCsRspEvtCssiNotifiy(VOS_UINT8 indexNum, TAF_CCM_CallSsInd *callSsInd, VOS_UINT16 *length)
{
    VOS_UINT16 lengthTemp;
    VOS_UINT32 cssiRptStatus;

    lengthTemp = *length;

    cssiRptStatus = AT_CheckRptCmdStatus(callSsInd->ssIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                         AT_RPT_CMD_CSSI);

    if ((cssiRptStatus == VOS_TRUE) && ((callSsInd->ssIndPara.ssNotify.code <= MN_CALL_SS_NTFY_BE_DEFLECTED) ||
                                        (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_CCBS_RECALL))) {
#if (FEATURE_IMS == FEATURE_ON)
        /* ^CSSI: <code1>[,<index>[,<number>,<type>]] */
        if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            At_ProcCsEvtCssiNotifiy_Ims(callSsInd, &lengthTemp);
        } else
#endif
        /* +CSSI: <code1>[,<index>] */
        {
            At_ProcCsEvtCssiNotifiy_NonIms(callSsInd, &lengthTemp);
        }
    }

    *length = lengthTemp;

    return;
}


VOS_VOID AT_ProcCsRspEvtSsNotify(VOS_UINT8 indexNum, TAF_CCM_CallSsInd *callSsInd)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length = 0;

    At_ProcCsRspEvtCssiNotifiy(indexNum, callSsInd, &length);

    At_ProcCsRspEvtCssuNotifiy(indexNum, callSsInd, &length);

#if (FEATURE_IMS == FEATURE_ON)
    if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
        At_ProcCsEvtImsHoldToneNotifiy_Ims(callSsInd, &length);
    }
#endif

    if ((indexNum != AT_BROADCAST_CLIENT_INDEX_MODEM_0) && (indexNum != AT_BROADCAST_CLIENT_INDEX_MODEM_1) &&
        (indexNum != AT_BROADCAST_CLIENT_INDEX_MODEM_2)) {
        modemId = MODEM_ID_0;

        /* 获取client id对应的Modem Id */
        rslt = AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId);

        if (rslt != VOS_OK) {
            AT_WARN_LOG("AT_ProcCsRspEvtSsNotify: WARNING:MODEM ID NOT FOUND!");
            return;
        }

        /* CCALLSTATE需要广播上报，根据MODEM ID设置广播上报的Index */
        if (modemId == MODEM_ID_0) {
            indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
        } else if (modemId == MODEM_ID_1) {
            indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_1;
        } else {
            indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_2;
        }
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_UINT32 AT_RcvTafCcmCallProcInd(struct MsgCB *msg)
{
    TAF_CCM_CallProcInd *callProcInd = VOS_NULL_PTR;
    ModemIdUint16        modemId;
    VOS_UINT32           rslt;
    VOS_UINT32           checkRptCmdStatusResult;
    TAF_UINT16           length;
    TAF_UINT8            indexNum = 0;

    length  = 0;
    modemId = MODEM_ID_0;

    callProcInd = (TAF_CCM_CallProcInd *)msg;

    if (At_ClientIdToUserId(callProcInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcCsRspEvtCallProc At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_CsRspEvtCallProcProc: Get modem id fail.");
        return VOS_ERR;
    }

    /* CS可视电话里面，这里不能上报^CONF ，因此只有普通语音和紧急呼叫的情况下，才上报^CONF */
    checkRptCmdStatusResult = AT_CheckRptCmdStatus(callProcInd->procIndPata.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC,
                                                   AT_RPT_CMD_CONF);

    if (((At_CheckReportConfCallType(callProcInd->procIndPata.callType) == PS_TRUE) ||
         (AT_EVT_IS_PS_VIDEO_CALL(callProcInd->procIndPata.callType, callProcInd->procIndPata.voiceDomain))) &&
        (checkRptCmdStatusResult == VOS_TRUE) && (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum))) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "^CONF:%d", callProcInd->procIndPata.callId);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    AT_ReportCCallstateResult(modemId, callProcInd->procIndPata.callId, callProcInd->procIndPata.curcRptCfg,
                              AT_CS_CALL_STATE_CALL_PROC, callProcInd->procIndPata.voiceDomain);

    return VOS_OK;
}


TAF_VOID AT_CsSsNotifyEvtIndProc(TAF_UINT8 indexNum, TAF_CCM_CallSsInd *callSsInd)
{
    VOS_UINT8  code;
    VOS_UINT16 length;
    VOS_UINT32 cssiRptStatus;
    VOS_UINT32 cssuRptStatus;

    length        = 0;
    cssiRptStatus = AT_CheckRptCmdStatus(callSsInd->ssIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                         AT_RPT_CMD_CSSI);

    if ((cssiRptStatus == VOS_TRUE) && ((callSsInd->ssIndPara.ssNotify.code <= MN_CALL_SS_NTFY_BE_DEFLECTED) ||
                                        (callSsInd->ssIndPara.ssNotify.code == MN_CALL_SS_NTFY_CCBS_RECALL))) {
#if (FEATURE_IMS == FEATURE_ON)
        /* ^CSSI: <code1>[,<index>[,<number>,<type>]] */
        if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            At_ProcCsEvtCssiNotifiy_Ims(callSsInd, &length);
        } else
#endif
        /* +CSSI: <code1>[,<index>] */
        {
            At_ProcCsEvtCssiNotifiy_NonIms(callSsInd, &length);
        }
    }

    cssuRptStatus = AT_CheckRptCmdStatus(callSsInd->ssIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                         AT_RPT_CMD_CSSU);

    if ((cssuRptStatus == VOS_TRUE) && (callSsInd->ssIndPara.ssNotify.code > MN_CALL_SS_NTFY_BE_DEFLECTED) &&
        (callSsInd->ssIndPara.ssNotify.code != MN_CALL_SS_NTFY_CCBS_RECALL)) {
        code = At_GetSsCode(callSsInd->ssIndPara.ssNotify.code, callSsInd->ssIndPara.callState);

        if (code == 0xFF) {
            AT_ERR_LOG("AT_CsSsNotifyEvtIndProc: cssu code error.");
            return;
        }

#if (FEATURE_IMS == FEATURE_ON)
        if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            /* ^CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] */
            At_ProcCsEvtCssuNotifiy_Ims(callSsInd, code, &length);
        } else
#endif
        {
            /* +CSSU: <code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] */
            At_ProcCsEvtCssuNotifiy_NonIms(callSsInd, code, &length);
        }
    }

#if (FEATURE_IMS == FEATURE_ON)
    if (callSsInd->ssIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
        At_ProcCsEvtImsHoldToneNotifiy_Ims(callSsInd, &length);
    }
#endif

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}

TAF_VOID At_CsIncomingEvtOfIncomeStateIndProc(TAF_UINT8 indexNum, TAF_CCM_CallIncomingInd *incomingInd)
{
    TAF_UINT16     length;
    TAF_UINT8      cLIValid;
    TAF_UINT32     timerName;
    TAF_UINT8      asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    AT_ModemCcCtx *ccCtx = VOS_NULL_PTR;
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);
    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    length = 0;

    if (ssCtx->crcType == AT_CRC_ENABLE_TYPE) { /* 命令与协议不符 */
                                                /* +CRC -- +CRING: <type> */

#if (FEATURE_IMS == FEATURE_ON)
        if (incomingInd->incomingIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%sIRING%s", g_atCrLf, g_atCrLf);
        } else
#endif
        {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CRING: ", g_atCrLf);

            length += (TAF_UINT16)At_CcClass2Print(incomingInd->incomingIndPara.callType, g_atSndCodeAddress + length);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    } else {
#if (FEATURE_IMS == FEATURE_ON)
        if (incomingInd->incomingIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%sIRING%s", g_atCrLf, g_atCrLf);
        } else
#endif
        {
            /* +CRC -- RING */
            if (g_atVType == AT_V_ENTIRE_TYPE) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%sRING%s", g_atCrLf, g_atCrLf);
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "2\r");
            }
        }
    }

    if (ssCtx->clipType == AT_CLIP_ENABLE_TYPE) {
        /*
         * +CLIP: <number>,<type>
         * 其它部分[,<subaddr>,<satype>[,[<alpha>][,<CLI validity>]]]不用上报
         */
        AT_ERR_LOG("At_CsIncomingEvtOfIncomeStateIndProc:+CLIP:");
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CLIP: ", g_atCrLf);

        cLIValid = AT_ConvertCLIValidity(incomingInd);

        if (incomingInd->incomingIndPara.callNumber.numLen != 0) {
            AT_BcdNumberToAscii(incomingInd->incomingIndPara.callNumber.bcdNum,
                                incomingInd->incomingIndPara.callNumber.numLen, (VOS_CHAR *)asciiNum);

            /* 来电号码类型为国际号码，需要在号码前加+,否则会回拨失败 */
            if (((incomingInd->incomingIndPara.callNumber.numType >> 4) & 0x07) == MN_MSG_TON_INTERNATIONAL) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"+%s\",%d,\"\",,\"\",%d", asciiNum,
                    (incomingInd->incomingIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT), cLIValid);
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",%d,\"\",,\"\",%d", asciiNum,
                    (incomingInd->incomingIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT), cLIValid);
            }
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",,\"\",,\"\",%d", cLIValid);
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    }

    if (ssCtx->salsType == AT_SALS_ENABLE_TYPE) {
        /* 上报是线路1还是线路2的来电 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s^ALS: ", g_atCrLf);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d", incomingInd->incomingIndPara.alsLineNo);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    /* 上报+CNAP命令 */
    AT_ReportCnapInfo(indexNum, &(incomingInd->incomingIndPara.nameIndicator));

    /* 只有呼叫类型为voice时才支持自动接听功能，其他场景暂时不支持自动接听 */
    if ((incomingInd->incomingIndPara.callType == MN_CALL_TYPE_VOICE) && (ccCtx->s0TimeInfo.s0TimerLen != 0)) {
        /* 如果自动接听功能没启动，收到RING事件后启动 */
        if (ccCtx->s0TimeInfo.timerStart != TAF_TRUE) {
            timerName = AT_S0_TIMER;
            timerName |= AT_INTERNAL_PROCESS_TYPE;
            timerName |= (indexNum << 12);

            /* 乘1000是为了将秒转为毫秒 */
            AT_StartRelTimer(&(ccCtx->s0TimeInfo.s0Timer), (ccCtx->s0TimeInfo.s0TimerLen) * 1000, timerName,
                             incomingInd->incomingIndPara.callId, VOS_RELTIMER_NOLOOP);
            ccCtx->s0TimeInfo.timerStart = TAF_TRUE;
            ccCtx->s0TimeInfo.timerName  = timerName;
            AT_PR_LOGH("At_CsIncomingEvtOfIncomeStateIndProc: S0TimerLen = %d", ccCtx->s0TimeInfo.s0TimerLen);
        }
    }

    return;
}


TAF_VOID At_CsIncomingEvtOfWaitStateIndProc(TAF_UINT8 indexNum, TAF_CCM_CallIncomingInd *incomingInd)
{
    TAF_UINT16     length;
    TAF_UINT8      asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;
    AT_CliValidityUint8 cliValidity;

    length = 0;
    ssCtx  = AT_GetModemSsCtxAddrFromClientId(indexNum);

    if (ssCtx->ccwaType == AT_CCWA_ENABLE_TYPE) { /* 命令与协议不符 */
#if (FEATURE_IMS == FEATURE_ON)
        if (incomingInd->incomingIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^CCWA: ", g_atCrLf);
        } else
#endif
        {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CCWA: ", g_atCrLf);
        }

        /* 空口存到TAF的Cause of No CLI与AT输出的CLI Validity有差异 */
        cliValidity = AT_ConvertCLIValidity(incomingInd);

        /*
         * 27007 7.12
         * When CLI is not available, <number> shall be an empty string ("")
         * and <type> value will not be significant. TA may return the recommended
         * value 128 for <type>.
         */
        if (cliValidity == AT_CLI_VALIDITY_VALID) {
            AT_BcdNumberToAscii(incomingInd->incomingIndPara.callNumber.bcdNum,
                                incomingInd->incomingIndPara.callNumber.numLen, (VOS_CHAR *)asciiNum);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",%d", asciiNum,
                (incomingInd->incomingIndPara.callNumber.numType | AT_NUMBER_TYPE_EXT));
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",%d", AT_NUMBER_TYPE_EXT);
        }

        if ((incomingInd->incomingIndPara.callType == MN_CALL_TYPE_VOICE) ||
            (incomingInd->incomingIndPara.callType == MN_CALL_TYPE_PSAP_ECALL)) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",1");
        } else if (incomingInd->incomingIndPara.callType == MN_CALL_TYPE_VIDEO) {
            if (incomingInd->incomingIndPara.voiceDomain == TAF_CALL_VOICE_DOMAIN_IMS) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",2");
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",32");
            }
        } else if (incomingInd->incomingIndPara.callType == MN_CALL_TYPE_FAX) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",4");
        } else if (incomingInd->incomingIndPara.callType == MN_CALL_TYPE_CS_DATA) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",2");
        } else {
        }

        /* <alpha>,<CLI_validity> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",,%d", cliValidity);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        if (ssCtx->salsType == AT_SALS_ENABLE_TYPE) {
            /* 上报是线路1还是线路2的来电 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^ALS: ", g_atCrLf);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d", incomingInd->incomingIndPara.alsLineNo);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return;
}


VOS_UINT32 At_RcvTafCcmCallIncomingInd(struct MsgCB *msg)
{
    TAF_CCM_CallIncomingInd *incomingInd = VOS_NULL_PTR;
    AT_ModemCcCtx           *ccCtx       = VOS_NULL_PTR;
    TAF_UINT8                indexNum    = 0;
    ModemIdUint16            modemId;
    VOS_UINT32               rslt;

    modemId = MODEM_ID_0;

    incomingInd = (TAF_CCM_CallIncomingInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(incomingInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_CsIncomingEvtIndProc: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_CsRspEvtConnectProc: Get modem id fail.");
        return VOS_ERR;
    }

    /*
     * 需要增加来电类型，传真，数据，可视电话，语音呼叫
     */
    /*
     * +CRC -- +CRING: <type> || RING
     * +CLIP: <number>,<type>[,<subaddr>,<satype>[,[<alpha>][,<CLI validity>]]]
     */
    if (incomingInd->incomingIndPara.callState == MN_CALL_S_INCOMING) {
        At_CsIncomingEvtOfIncomeStateIndProc(indexNum, incomingInd);
        AT_ReportCCallstateResult(modemId, incomingInd->incomingIndPara.callId, incomingInd->incomingIndPara.curcRptCfg,
                                  AT_CS_CALL_STATE_INCOMMING, incomingInd->incomingIndPara.voiceDomain);
    } else if (incomingInd->incomingIndPara.callState == MN_CALL_S_WAITING) {
        At_CsIncomingEvtOfWaitStateIndProc(indexNum, incomingInd);
        AT_ReportCCallstateResult(modemId, incomingInd->incomingIndPara.callId, incomingInd->incomingIndPara.curcRptCfg,
                                  AT_CS_CALL_STATE_WAITING, incomingInd->incomingIndPara.voiceDomain);
    } else {
        ccCtx->curIsExistCallFlag = VOS_TRUE;
        return VOS_OK;
    }

#if (FEATURE_AT_HSUART == FEATURE_ON)
    if (At_CheckUartRingTeCallType(incomingInd->incomingIndPara.callType) == PS_TRUE) {
        /* 通过UART端口的RING脚输出波形通知TE */
        AT_VoiceStartRingTe(incomingInd->incomingIndPara.callId);
    }
#endif
    ccCtx->curIsExistCallFlag = VOS_TRUE;

    return VOS_OK;
}


VOS_UINT32 AT_ConCallMsgTypeToCuusiMsgType(MN_CALL_Uus1MsgTypeUint32 msgType, AT_CuusiMsgTypeUint32 *cuusiMsgType

)
{
    VOS_UINT32 i;

    for (i = 0; i < sizeof(g_cuusiMsgType) / sizeof(AT_CALL_CuusiMsg); i++) {
        if (msgType == g_cuusiMsgType[i].callMsgType) {
            *cuusiMsgType = g_cuusiMsgType[i].cuusiMsgType;
            return VOS_OK;
        }
    }

    return VOS_ERR;
}


VOS_UINT32 AT_ConCallMsgTypeToCuusuMsgType(MN_CALL_Uus1MsgTypeUint32 msgType, AT_CuusuMsgTypeUint32 *cuusuMsgType

)
{
    VOS_UINT32 i;

    for (i = 0; i < sizeof(g_cuusuMsgType) / sizeof(AT_CALL_CuusuMsg); i++) {
        if (msgType == g_cuusuMsgType[i].callMsgType) {
            *cuusuMsgType = g_cuusuMsgType[i].cuusuMsgType;
            return VOS_OK;
        }
    }

    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafCcmUus1InfoInd(struct MsgCB *msg)
{
    TAF_CCM_Uus1InfoInd  *uus1InfoInd = VOS_NULL_PTR;
    VOS_UINT32            msgType;
    AT_CuusiMsgTypeUint32 cuusiMsgType;
    AT_CuusuMsgTypeUint32 cuusuMsgType;
    VOS_UINT32            ret;
    MN_CALL_DirUint8      callDir;
    VOS_UINT16            length;
    VOS_UINT8             indexNum = 0;

    uus1InfoInd = (TAF_CCM_Uus1InfoInd *)msg;

    /* 根据ClientID获取通道索引 */
    if (At_ClientIdToUserId(uus1InfoInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallStartDtmfCnf: Get Index Fail!");
        return VOS_ERR;
    }

    callDir = uus1InfoInd->uus1InfoIndPara.callDir;
    msgType = AT_CUUSI_MSG_ANY;

    ret = AT_ConCallMsgTypeToCuusiMsgType(uus1InfoInd->uus1InfoIndPara.uusInfo.msgType, &cuusiMsgType);
    if (ret == VOS_OK) {
        msgType = cuusiMsgType;
        callDir = MN_CALL_DIR_MO;
    } else {
        ret = AT_ConCallMsgTypeToCuusuMsgType(uus1InfoInd->uus1InfoIndPara.uusInfo.msgType, &cuusuMsgType);
        if (ret == VOS_OK) {
            msgType = cuusuMsgType;
            callDir = MN_CALL_DIR_MT;
        }
    }

    if (ret != VOS_OK) {
        if (uus1InfoInd->uus1InfoIndPara.callDir == MN_CALL_DIR_MO) {
            msgType = AT_CUUSI_MSG_ANY;
        } else {
            msgType = AT_CUUSU_MSG_ANY;
        }
    }

    length = 0;

    if (callDir == MN_CALL_DIR_MO) {
        /* 未激活则不进行任何处理,不能上报 */
        if (AT_CheckRptCmdStatus(uus1InfoInd->uus1InfoIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                 AT_RPT_CMD_CUUS1I) == VOS_FALSE) {
            return VOS_ERR;
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CUUS1I:", g_atCrLf);
    } else {
        /* 未激活则不进行任何处理,不能上报 */
        if (AT_CheckRptCmdStatus(uus1InfoInd->uus1InfoIndPara.unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED,
                                 AT_RPT_CMD_CUUS1U) == VOS_FALSE) {
            return VOS_ERR;
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CUUS1U:", g_atCrLf);
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,", msgType);

    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                  uus1InfoInd->uus1InfoIndPara.uusInfo.uuie,
                                                  (uus1InfoInd->uus1InfoIndPara.uusInfo.uuie[MN_CALL_LEN_POS] +
                                                   MN_CALL_UUIE_HEADER_LEN));

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_VOID At_ProcSetClccResult(VOS_UINT8 numOfCalls, TAF_CCM_QryCallInfoCnf *qryCallInfoCnf, VOS_UINT8 indexNum)
{
    VOS_UINT32        tmp;
    AT_CLCC_ModeUint8 clccMode;
    VOS_UINT8         asciiNum[MN_CALL_MAX_CALLED_ASCII_NUM_LEN + 1];
    VOS_UINT16        length;

    VOS_UINT8 numberType;

    numberType = AT_NUMBER_TYPE_UNKOWN;

    length = 0;

    if ((numOfCalls != 0) && (numOfCalls <= AT_CALL_MAX_NUM)) {
        for (tmp = 0; tmp < numOfCalls; tmp++) {
            /* <CR><LF> */
            if (tmp != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            AT_MapCallTypeModeToClccMode(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callType, &clccMode);

            /* +CLCC:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName,              /* +CLCC: */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callId,     /* <id1>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callDir,    /* <dir>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callState,  /* <stat>, */
                clccMode,                                                  /* <mode>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].mptyState); /* <mpty>, */

            if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callDir == MN_CALL_DIR_MO) {
                if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numLen != 0) {
                    /* <number>, */
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numLen,
                                        (TAF_CHAR *)asciiNum);

                    /* <type>,不报<alpha>,<priority> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d,\"\",", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numType |
                             AT_NUMBER_TYPE_EXT));
                } else if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numLen != 0) {
                    /* <number>, */
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numLen,
                                        (TAF_CHAR *)asciiNum);

                    /* <type>,不报<alpha>,<priority> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d,\"\",", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numType | AT_NUMBER_TYPE_EXT));

                } else {
                    /* <type>,不报<alpha>,<priority> */
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"\",%d", numberType);
                }
            } else {
                if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numLen != 0) {
                    /* <number>, */
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numLen,
                                        (VOS_CHAR *)asciiNum);

                    /* <type>,不报<alpha>,<priority> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d,\"\",", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numType | AT_NUMBER_TYPE_EXT));
                } else {
                    /* <type>,不报<alpha>,<priority> */
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"\",%d", numberType);
                }
            }
        }
    }

    g_atSendDataBuff.bufLen = length;
}


VOS_VOID At_ReportClccDisplayName(MN_CALL_DisplayName *displayName, VOS_UINT16 *length)
{
    VOS_UINT32 i;

    /* ,<display name> */
    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",");

    if (displayName->numLen != 0) {
        /* 以UTF8格式显示，如中国对应E4B8ADE59BBD */
        displayName->numLen = AT_MIN(displayName->numLen, MN_CALL_DISPLAY_NAME_STRING_SZ);
        for (i = 0; i < displayName->numLen; i++) {
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "%X", (VOS_UINT8)displayName->displayName[i]);
        }
    }

    return;
}


VOS_VOID At_ReportPeerVideoSupport(MN_CALL_InfoParam *callInfo, VOS_UINT16 *length)
{
    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",");

    if (callInfo->opPeerVideoSupport == VOS_TRUE) {
        /* <terminal video support> */
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", callInfo->peerVideoSupport);
    }

    return;
}


VOS_VOID At_ReportClccImsDomain(MN_CALL_InfoParam *callInfo, VOS_UINT16 *length)
{
    AT_IMS_CallDomainUint8 imsDomain;

    imsDomain = AT_IMS_CALL_DOMAIN_LTE;

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",");

    if (callInfo->imsDomain != TAF_CALL_IMS_DOMAIN_NULL) {
        switch (callInfo->imsDomain) {
            case TAF_CALL_IMS_DOMAIN_LTE:
                imsDomain = AT_IMS_CALL_DOMAIN_LTE;
                break;

            case TAF_CALL_IMS_DOMAIN_WIFI:
                imsDomain = AT_IMS_CALL_DOMAIN_WIFI;
                break;
            case TAF_CALL_IMS_DOMAIN_NR:
                imsDomain = AT_IMS_CALL_DOMAIN_NR;
                break;

            default:
                break;
        }

        /* <imsDomain> */
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", imsDomain);
    }

    return;
}


VOS_VOID At_ReportClccRttPara(MN_CALL_InfoParam *callInfo, VOS_UINT16 *length)
{
    /* 不管是否为RTT通话，都需要输出 <RttFlg>,<RttChannelId>,<cps> 三个参数  */
    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d,%d,%d", callInfo->rtt, callInfo->rttChannelId, callInfo->cps);

    return;
}


VOS_VOID At_ReportClccEncryptPara(MN_CALL_InfoParam *callInfo, VOS_UINT16 *length)
{
    /* ,<isEncrypt> */
    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d", callInfo->encryptFlag);

    return;
}


VOS_VOID At_ProcQryClccResult(VOS_UINT8 numOfCalls, TAF_CCM_QryCallInfoCnf *qryCallInfoCnf, VOS_UINT8 indexNum)
{
    VOS_UINT32                tmp;
    VOS_UINT8                 numberType;
    VOS_UINT8                 asciiNum[MN_CALL_MAX_CALLED_ASCII_NUM_LEN + 1];
    VOS_UINT16                length;
    AT_CLCC_ModeUint8         clccMode;
    TAF_CALL_VoiceDomainUint8 voiceDomain;

    numberType = AT_NUMBER_TYPE_UNKOWN;
    length     = 0;

    if ((numOfCalls != 0) && (numOfCalls <= AT_CALL_MAX_NUM)) {
        for (tmp = 0; tmp < numOfCalls; tmp++) {
            /* <CR><LF> */
            if (tmp != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            AT_MapCallTypeModeToClccMode(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callType, &clccMode);

            if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].voiceDomain == TAF_CALL_VOICE_DOMAIN_3GPP2) {
                voiceDomain = TAF_CALL_VOICE_DOMAIN_3GPP;
            } else {
                voiceDomain = qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].voiceDomain;
            }

            /* ^CLCC:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName,                    /* ^CLCC: */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callId,           /* <id1>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callDir,          /* <dir>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callState,        /* <stat>, */
                clccMode,                                                        /* <mode>, */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].mptyState,        /* <mpty>, */
                voiceDomain,                                                     /* <voice_domain> */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callType,         /* <call_type> */
                qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].eConferenceFlag); /* <isEConference> */

            if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callDir == MN_CALL_DIR_MO) {
                if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numLen != 0) {
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numLen,
                                        (TAF_CHAR *)asciiNum);

                    /* ,<number>,<type> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].connectNumber.numType |
                             AT_NUMBER_TYPE_EXT));
                } else if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numLen != 0) {
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numLen,
                                        (TAF_CHAR *)asciiNum);

                    /* ,<number>,<type> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].calledNumber.numType | AT_NUMBER_TYPE_EXT));
                } else {
                    /* ,,<type> */
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"\",%d", numberType);
                }
            } else {
                if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numLen != 0) {
                    AT_BcdNumberToAscii(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.bcdNum,
                                        qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numLen,
                                        (VOS_CHAR *)asciiNum);

                    /* ,<number>,<type> */
                    length +=
                        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",%d", asciiNum,
                            (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callNumber.numType | AT_NUMBER_TYPE_EXT));
                } else {
                    /* ,,<type> */
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"\",%d", numberType);
                }
            }

            At_ReportClccDisplayName(&(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].displayName), &length);

            At_ReportPeerVideoSupport(&(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp]), &length);

            At_ReportClccImsDomain(&(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp]), &length);

            At_ReportClccRttPara(&(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp]), &length);

            At_ReportClccEncryptPara(&(qryCallInfoCnf->qryCallInfoPara.callInfos[tmp]), &length);
        }
    }

    g_atSendDataBuff.bufLen = length;
}
#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 At_ProcQryClccEconfResult(TAF_CCM_QryEconfCalledInfoCnf *callInfos, VOS_UINT8 indexNum)
{
    VOS_UINT16 length;
    VOS_UINT32 tmp;
    VOS_CHAR   asciiNum[MN_CALL_MAX_CALLED_ASCII_NUM_LEN + 1];

    length = 0;
    memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));

    /* ^CLCCECONF: Maximum-user-count, n_address */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress,
        "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, /* ^CLCCECONF: */
        callInfos->numOfMaxCalls,                                  /* Maximum-user-count */
        callInfos->numOfCalls);

    callInfos->numOfCalls = AT_MIN(callInfos->numOfCalls, TAF_CALL_MAX_ECONF_CALLED_NUM);
    if (callInfos->numOfCalls != 0) {
        /* n_address */
        for (tmp = 0; tmp < callInfos->numOfCalls; tmp++) {
            /* 转换电话号码 */
            if (callInfos->callInfo[tmp].callNumber.numLen != 0) {
                /* <number>, */
                AT_BcdNumberToAscii(callInfos->callInfo[tmp].callNumber.bcdNum,
                                    callInfos->callInfo[tmp].callNumber.numLen, asciiNum);
            }

            /* entity, Display-text,Status */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\",\"%s\",%d",
                asciiNum,                             /* 电话号码 */
                callInfos->callInfo[tmp].displaytext, /* display-text */
                callInfos->callInfo[tmp].callState);  /* Call State */
        }
    }

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}
#endif


VOS_UINT32 AT_RcvTafCcmQryCallInfoCnf(struct MsgCB *msg)
{
    VOS_UINT8               numOfCalls;
    VOS_UINT8               indexNum;
    VOS_UINT32              tmp;
    AT_CPAS_StatusUint8     cpas;
    VOS_UINT32              ret;
    TAF_CCM_QryCallInfoCnf *qryCallInfoCnf = VOS_NULL_PTR;

    /* 初始化 */
    indexNum       = 0;
    qryCallInfoCnf = (TAF_CCM_QryCallInfoCnf *)msg;

    /* 获取当前所有不为IDLE态的呼叫信息 */
    numOfCalls = qryCallInfoCnf->qryCallInfoPara.numOfCalls;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryCallInfoCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_CsAllCallInfoEvtCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_CsAllCallInfoEvtCnfProc: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 格式化命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CLCC_SET) {
        /* CLCC命令的结果回复 */
        At_ProcSetClccResult(numOfCalls, qryCallInfoCnf, indexNum);

        ret = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CLCC_QRY) {
        /* ^CLCC?命令的结果回复 */
        At_ProcQryClccResult(numOfCalls, qryCallInfoCnf, indexNum);

        ret = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPAS_SET) {
        /* CPAS命令的结果回复 */
        if (numOfCalls > AT_CALL_MAX_NUM) {
            At_FormatResultData(indexNum, AT_CME_UNKNOWN);
            return VOS_OK;
        }

        if (numOfCalls == 0) {
            cpas = AT_CPAS_STATUS_READY;
        } else {
            cpas = AT_CPAS_STATUS_CALL_IN_PROGRESS;
            for (tmp = 0; tmp < numOfCalls; tmp++) {
                if (qryCallInfoCnf->qryCallInfoPara.callInfos[tmp].callState == MN_CALL_S_INCOMING) {
                    cpas = AT_CPAS_STATUS_RING;
                    break;
                }
            }
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, cpas);

        ret = AT_OK;

    } else {
        return VOS_OK;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmQryAlsCnf(struct MsgCB *msg)
{
    TAF_CCM_QryAlsCnf *alsCnf   = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;
    VOS_UINT32         result;
    VOS_UINT16         len;
    AT_ModemSsCtx     *modemSsCtx = VOS_NULL_PTR;

    alsCnf = (TAF_CCM_QryAlsCnf *)msg;

    if (At_ClientIdToUserId(alsCnf->ctrl.clientId, &indexNum) != AT_SUCCESS) {
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_QryAlsCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    len                     = 0;
    result                  = AT_ERROR;
    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    modemSsCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    if (alsCnf->qryAlsPara.ret == TAF_ERR_NO_ERROR) {
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            modemSsCtx->salsType);

        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, ",%d", alsCnf->qryAlsPara.alsLine);

        g_atSendDataBuff.bufLen = len;

        result = AT_OK;
    } else {
        /* 当不支持ALS特性时，由call上报错误时间，AT返回error，AT不区分是否支持该特性 */
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmQryUus1InfoCnf(struct MsgCB *msg)
{
    TAF_CCM_QryUus1InfoCnf *uus1Cnf  = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              result;
    VOS_UINT32              i;
    VOS_UINT16              len;
    VOS_UINT32              uus1IFlg;
    VOS_UINT32              uus1UFlg;

    uus1Cnf = (TAF_CCM_QryUus1InfoCnf *)msg;

    if (At_ClientIdToUserId(uus1Cnf->ctrl.clientId, &indexNum) != AT_SUCCESS) {
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_QryUus1Cnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    len                     = 0;
    result                  = AT_ERROR;
    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 变量初始化为打开主动上报 */
    uus1IFlg = VOS_TRUE;
    uus1UFlg = VOS_TRUE;

    /* UUS1I是否打开 */
    if (uus1Cnf->qryUss1Info.setType[0] == MN_CALL_CUUS1_DISABLE) {
        uus1IFlg = VOS_FALSE;
    }

    /* UUS1U是否打开 */
    if (uus1Cnf->qryUss1Info.setType[1] == MN_CALL_CUUS1_DISABLE) {
        uus1UFlg = VOS_FALSE;
    }

    if (uus1Cnf->qryUss1Info.ret == TAF_ERR_NO_ERROR) {
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s", g_atCrLf);

        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + len), "%d,%d", uus1IFlg, uus1UFlg);

        uus1Cnf->qryUss1Info.actNum = AT_MIN(uus1Cnf->qryUss1Info.actNum, MN_CALL_MAX_UUS1_MSG_NUM);
        for (i = 0; i < uus1Cnf->qryUss1Info.actNum; i++) {
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)(g_atSndCodeAddress + len), ",%d,", uus1Cnf->qryUss1Info.uus1Info[i].msgType);

            len += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                       (VOS_UINT8 *)g_atSndCodeAddress + len,
                                                       uus1Cnf->qryUss1Info.uus1Info[i].uuie,
                                                       uus1Cnf->qryUss1Info.uus1Info[i].uuie[MN_CALL_LEN_POS] +
                                                           MN_CALL_UUIE_HEADER_LEN);
        }

        g_atSendDataBuff.bufLen = len;

        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmSetAlsCnf(struct MsgCB *msg)
{
    TAF_CCM_SetAlsCnf *setAlsCnf = VOS_NULL_PTR;
    VOS_UINT8          indexNum  = 0;
    VOS_UINT32         result;

    setAlsCnf = (TAF_CCM_SetAlsCnf *)msg;

    if (At_ClientIdToUserId(setAlsCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SetAlsCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetAlsCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setAlsCnf->ret == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_CME_UNKNOWN;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmSetUus1InfoCnf(struct MsgCB *msg)
{
    TAF_CCM_SetUus1InfoCnf *uus1Cnf  = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              result;

    uus1Cnf = (TAF_CCM_SetUus1InfoCnf *)msg;

    if (At_ClientIdToUserId(uus1Cnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SetUus1Cnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetUus1Cnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (uus1Cnf->ret == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_CME_INCORRECT_PARAMETERS;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


TAF_UINT32 At_CcfcQryReport(TAF_SS_CallIndependentEvent *event, TAF_UINT8 indexNum)
{
    TAF_UINT32 tmp    = 0;
    TAF_UINT16 length = 0;

    /*
     * +CCFC: <status>,<class1>[,<number>,<type>[,<subaddr>,<satype>[,<time>]]]
     */
    if (event->opSsStatus == 1) {
        /* +CCFC: <status>,<class1> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d", (TAF_SS_ACTIVE_STATUS_MASK & event->ssStatus),
            AT_CC_CALSS_TYPE_INVALID);

        return length;
    }

    if (event->opFwdFeaturelist == 1) {
        event->fwdFeaturelist.cnt = AT_MIN(event->fwdFeaturelist.cnt, TAF_SS_MAX_NUM_OF_FW_FEATURES);
        for (tmp = 0; tmp < event->fwdFeaturelist.cnt; tmp++) {
            if (tmp != 0) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            /* +CCFC:  */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            /* <status> */
            if (event->fwdFeaturelist.fwdFtr[tmp].opSsStatus == 1) {
                length +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "%d",
                        (TAF_SS_ACTIVE_STATUS_MASK & (event->fwdFeaturelist.fwdFtr[tmp].ssStatus)));
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "0");
            }

            /* <class1> */
            if (event->fwdFeaturelist.fwdFtr[tmp].opBsService == 1) {
                length +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                        At_GetClckClassFromBsCode(&(event->fwdFeaturelist.fwdFtr[tmp].bsService)));
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", AT_CC_CALSS_TYPE_INVALID);
            }

            /* <number> */
            if (event->fwdFeaturelist.fwdFtr[tmp].opFwdToNum == 1) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", event->fwdFeaturelist.fwdFtr[tmp].fwdToNum);

                /* <type> */
                if (event->fwdFeaturelist.fwdFtr[tmp].opNumType == 1) {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", event->fwdFeaturelist.fwdFtr[tmp].numType);
                } else {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                        At_GetCodeType(event->fwdFeaturelist.fwdFtr[tmp].fwdToNum[0]));
                }

                /* <subaddr> */
                if (event->fwdFeaturelist.fwdFtr[tmp].opFwdToSubAddr == 1) {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"",
                        event->fwdFeaturelist.fwdFtr[tmp].fwdToSubAddr);

                    /* <satype> */
                    if (event->fwdFeaturelist.fwdFtr[tmp].opSubAddrType == 1) {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                            event->fwdFeaturelist.fwdFtr[tmp].subAddrType);
                    } else {
                        length +=
                            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                                At_GetCodeType(event->fwdFeaturelist.fwdFtr[tmp].fwdToSubAddr[0]));
                    }
                } else {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",,");
                }

                /* <time> */
                if (event->fwdFeaturelist.fwdFtr[tmp].opNoRepCondTime == 1) {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                        event->fwdFeaturelist.fwdFtr[tmp].noRepCondTime);
                } else {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",");
                }
            }
        }
    }

    return length;
}


VOS_UINT32 At_ProcReportUssdStr_Nontrans(TAF_SS_CallIndependentEvent *event, VOS_UINT16 printOffSet)
{
    TAF_SS_UssdString ussdStrBuff;
    MN_MSG_CbdcsCode  dcsInfo;
    VOS_UINT32        defAphaLen;
    VOS_UINT32        asciiStrLen;
    errno_t           memResult;
    VOS_UINT16        outPrintOffSet;
    VOS_UINT32        ret;

    outPrintOffSet = 0;

    if (event->ussdString.cnt == 0) {
        AT_WARN_LOG("At_ProcReportUssdStr_Nontrans: UssdString Cnt is 0.");
        return outPrintOffSet;
    }

    memset_s(&dcsInfo, sizeof(dcsInfo), 0x00, sizeof(dcsInfo));

    /* USSD与CBS的DCS的协议相同，调用CBS的DCS解析函数解码，详细情况参考23038 */
    ret = MN_MSG_DecodeCbsDcs(event->dataCodingScheme, event->ussdString.ussdStr, event->ussdString.cnt, &dcsInfo);

    if (ret != MN_ERR_NO_ERROR) {
        AT_WARN_LOG("At_ProcReportUssdStr_Nontrans:WARNING: Decode Failure");
        return outPrintOffSet;
    }

    /* 先处理UCS2码流 */
    if (dcsInfo.msgCoding == MN_MSG_MSG_CODING_UCS2) {
        outPrintOffSet = (TAF_UINT16)At_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                             g_atSndCodeAddress + printOffSet,
                                                             event->ussdString.ussdStr, event->ussdString.cnt);
    } else {
        /* 7Bit需要额外先解码，归一化到Ascii码流 */
        if (dcsInfo.msgCoding == MN_MSG_MSG_CODING_7_BIT) {
            memset_s(&ussdStrBuff, sizeof(TAF_SS_UssdString), 0, sizeof(TAF_SS_UssdString));

            defAphaLen = (VOS_UINT32)event->ussdString.cnt * 8 / 7;

            (VOS_VOID)TAF_STD_UnPack7Bit(event->ussdString.ussdStr, defAphaLen, 0, ussdStrBuff.ussdStr);

            if ((ussdStrBuff.ussdStr[defAphaLen - 1]) == 0x0d) {
                defAphaLen--;
            }

            asciiStrLen = 0;

            TAF_STD_ConvertDefAlphaToAscii(ussdStrBuff.ussdStr, defAphaLen, ussdStrBuff.ussdStr, &asciiStrLen);

            ussdStrBuff.cnt = (VOS_UINT16)asciiStrLen;
        }
        /* 其他情况:8Bit 直接拷贝 */
        else {
            memResult = memcpy_s(&ussdStrBuff, sizeof(TAF_SS_UssdString), &(event->ussdString),
                                 sizeof(TAF_SS_UssdString));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_SS_UssdString), sizeof(TAF_SS_UssdString));
        }

        /* 非透传模式处理 */
        if (g_atCscsType == AT_CSCS_UCS2_CODE) { /* +CSCS:UCS2 */
            outPrintOffSet = (TAF_UINT16)At_Ascii2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                               g_atSndCodeAddress + printOffSet, ussdStrBuff.ussdStr,
                                                               ussdStrBuff.cnt);
        } else {
            if (ussdStrBuff.cnt > 0) {
                memResult = memcpy_s((TAF_CHAR *)g_atSndCodeAddress + printOffSet, ussdStrBuff.cnt, ussdStrBuff.ussdStr,
                                     ussdStrBuff.cnt);
                TAF_MEM_CHK_RTN_VAL(memResult, ussdStrBuff.cnt, ussdStrBuff.cnt);
            }
            outPrintOffSet = ussdStrBuff.cnt;
        }
    }

    return outPrintOffSet;
}


VOS_UINT16 AT_PrintUssdStr(TAF_SS_CallIndependentEvent *event, VOS_UINT8 indexNum, VOS_UINT16 length)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;
    VOS_UINT16     printOffSet;

    /* 没有USSD STRING需要打印 */
    /* 如果有USSD 字符串上报，必带DCS项 */
    if (event->opDataCodingScheme == 0) {
        AT_WARN_LOG("AT_PrintUssdStr: No DCS.");
        return length;
    }

    /* 如果主动上报的字符，放在USSDSting中 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        if (event->opUssdString == 0) {
            AT_WARN_LOG("AT_PrintUssdStr: BroadCast,No UssdString.");
            return length;
        }

    } else {
        /* 如果非主动上报的字符，可放在USSDSting中，也可放在USSData中 */
        /* 当网络29拒绝后，重发请求，网络的回复是放在USSData中 */
        if ((event->opUssdString == 0) && (event->opUSSData == 0)) {
            AT_WARN_LOG("AT_PrintUssdStr: No UssdSting & UssData.");
            return length;
        }
    }

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    printOffSet = length;
    printOffSet += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + printOffSet, ",\"");

    if (event->ussdString.cnt > sizeof(event->ussdString.ussdStr)) {
        AT_WARN_LOG1("AT_PrintUssdStr: Invalid pstEvent->UssdString.usCnt: ", event->ussdString.cnt);
        event->ussdString.cnt = sizeof(event->ussdString.ussdStr);
    }

    switch (ssCtx->ussdTransMode) {
        case AT_USSD_TRAN_MODE:
            printOffSet += (TAF_UINT16)At_HexString2AsciiNumPrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                                  g_atSndCodeAddress + printOffSet,
                                                                  event->ussdString.ussdStr, event->ussdString.cnt);
            break;

        case AT_USSD_NON_TRAN_MODE:
            /* 处理非透传模式下上报的7 8Bit UssdString */
            printOffSet += (TAF_UINT16)At_ProcReportUssdStr_Nontrans(event, printOffSet);
            break;

        default:
            break;
    }

    /* <dcs> */
    printOffSet += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + printOffSet, "\",%d", event->dataCodingScheme);

    return printOffSet;
}


TAF_VOID At_SsIndProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event)
{
    TAF_UINT16 length          = 0;
    TAF_UINT8  tmp             = 0;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    switch (event->ssEvent) {
        /* 其它事件 */
        case TAF_SS_EVT_USS_NOTIFY_IND:           /* 通知用户不用进一步操作 */
        case TAF_SS_EVT_USS_REQ_IND:              /* 通知用户进一步操作 */
        case TAF_SS_EVT_USS_RELEASE_COMPLETE_IND: /* 通知用户网络释放 */
        case TAF_SS_EVT_PROCESS_USS_REQ_CNF:
            /* <m> */
            if (event->ssEvent == TAF_SS_EVT_USS_NOTIFY_IND) {
                tmp = 0;
            } else if (event->ssEvent == TAF_SS_EVT_USS_REQ_IND) {
                tmp = 1;
            } else {
                if (*systemAppConfig == SYSTEM_APP_ANDROID) {
                    tmp = AT_PROCESS_USS_REQ_CNF_TMP;
                } else {
                    tmp = 0;
                }
            }
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s+CUSD: ", g_atCrLf);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d", tmp);
            /* <str> */

            length = AT_PrintUssdStr(event, indexNum, length);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            At_SendResultData(indexNum, g_atSndCodeAddress, length);
            return;

        case TAF_SS_EVT_ERROR:
            if (event->errorCode == TAF_ERR_USSD_NET_TIMEOUT) {
                length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress, "%s+CUSD: %d%s", g_atCrLf, AT_CUSD_M_NETWORK_TIMEOUT, g_atCrLf);

                At_SendResultData(indexNum, g_atSndCodeAddress, length);

                return;
            }

            if (event->errorCode == TAF_ERR_USSD_USER_TIMEOUT) {
                length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress, "%s+CUSD: %d%s", g_atCrLf, AT_CUSD_M_NETWORK_CANCEL, g_atCrLf);

                At_SendResultData(indexNum, g_atSndCodeAddress, length);

                return;
            }

            break;

        default:
            return;
    }
}


TAF_UINT8 At_GetClckClassFromBsCode(TAF_SS_BasicService *bs)
{
    VOS_UINT32 loop;
    VOS_UINT32 itemsNum;

    itemsNum = sizeof(g_clckClassServiceExtTbl) / sizeof(AT_ClckClassServiceTbl);

    /* 查表获取服务类型及服务码对应的Class */
    for (loop = 0; loop < itemsNum; loop++) {
        if ((g_clckClassServiceExtTbl[loop].serviceType == bs->bsType) &&
            (g_clckClassServiceExtTbl[loop].serviceCode == bs->bsServiceCode)) {
            return g_clckClassServiceExtTbl[loop].classType;
        }
    }

    return AT_UNKNOWN_CLCK_CLASS;
}


TAF_UINT32 At_SsRspCusdProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event)
{
    TAF_UINT32 result;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->ssEvent == TAF_SS_EVT_ERROR) {
        /* 本地发生错误: 清除+CUSD状态 */
        result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
    } else {
        /* 先报OK再发网络字符串 */
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return result;
}


TAF_VOID At_SsRspInterrogateCnfClipProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    TAF_UINT8      tmp   = 0;
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    /* +CLIP: <n>,<m> */
    if (event->opSsStatus == 1) { /* 查到状态 */
        tmp = (TAF_SS_PROVISIONED_STATUS_MASK & event->ssStatus) ? 1 : 0;
    } else { /* 没有查到状态 */
        tmp = AT_CLI_SS_UNKNOWN_REASON;
    }

    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ssCtx->clipType, tmp);

    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCnfColpProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    TAF_UINT8      tmp   = 0;
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    if (event->opSsStatus == 1) { /* 查到状态 */
        tmp = (TAF_SS_ACTIVE_STATUS_MASK & event->ssStatus);
    } else { /* 没有查到状态 */
        tmp = AT_CLI_SS_UNKNOWN_REASON;
    }

    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ssCtx->colpType, tmp);

    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCnfClirProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    TAF_UINT8                    tmp = 0;
    TAF_SS_CLI_RESTRICION_OPTION clirTmp;
    TAF_UINT8                    cliSsStatus;
    AT_ModemSsCtx               *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    if (event->opGenericServiceInfo == 1) { /* 查到状态 */
        cliSsStatus = TAF_SS_ACTIVE_STATUS_MASK & event->genericServiceInfo.ssStatus;
        if (cliSsStatus) {
            if (event->genericServiceInfo.opCliStrictOp == 1) {
                clirTmp = event->genericServiceInfo.cliRestrictionOp;
                if (clirTmp == TAF_SS_CLI_PERMANENT) {
                    tmp = AT_CLIR_SS_PERMANENT;
                } else if (clirTmp == TAF_SS_CLI_TMP_DEFAULT_RESTRICTED) {
                    tmp = AT_CLIR_SS_TMP_DEFAULT_RESTRICTED;
                } else if (clirTmp == TAF_SS_CLI_TMP_DEFAULT_ALLOWED) {
                    tmp = AT_CLIR_SS_TMP_DEFAULT_ALLOWED;
                } else {
                    tmp = AT_CLIR_SS_UNKNOWN_REASON;
                }
            } else {
                tmp = AT_CLIR_SS_UNKNOWN_REASON;
            }
        } else {
            tmp = AT_CLIR_SS_PROVIDED;
        }
    } else if (event->opSsStatus == 1) {
        tmp = AT_CLIR_SS_PROVIDED;
    } else { /* 没有查到状态 */
        tmp = AT_CLIR_SS_UNKNOWN_REASON;
    }

    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ssCtx->clirType, tmp);

    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCnfClckProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    TAF_UINT8  tmp = 0;
    TAF_UINT32 i;
    VOS_UINT32 customizeFlag;
    VOS_UINT32 successFlg;
    errno_t memResult;

    /* +CLCK: <status>,<class1> */
    if (event->opError == 1) {                                    /* 需要首先判断错误码 */
        *result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
        return;
    }

    if (event->opSsStatus == 1) { /* 查到状态 */
        tmp = (TAF_SS_ACTIVE_STATUS_MASK & event->ssStatus);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, tmp,
            AT_CLCK_PARA_CLASS_ALL);

        /* 输出网络IE SS-STATUS值给用户 */
        customizeFlag = AT_GetSsCustomizePara(AT_SS_CUSTOMIZE_CLCK_QUERY);
        if (customizeFlag == VOS_TRUE) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", event->ssStatus);
        }
    } else if (event->opBsServGroupList == 1) {
        successFlg = VOS_FALSE;
        event->bsServGroupList.cnt = AT_MIN(event->bsServGroupList.cnt, TAF_SS_MAX_NUM_OF_BASIC_SERVICES);
        for (i = 0; i < event->bsServGroupList.cnt; i++) {
            /* 此处用ucTmp保存class，而不是status参数 */
            tmp = At_GetClckClassFromBsCode(&event->bsServGroupList.bsService[i]);
            if (tmp != AT_UNKNOWN_CLCK_CLASS) {
                successFlg = VOS_TRUE;
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d%s",
                    g_parseContext[indexNum].cmdElement->cmdName, 1, tmp, g_atCrLf);
            }
        }

        if (successFlg == VOS_TRUE) {
            *length -= (TAF_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
            memResult = memset_s((VOS_UINT8 *)g_atSndCodeAddress + *length, AT_CMD_MAX_LEN - *length, 0x0,
                                 AT_CMD_MAX_LEN - *length);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN - *length, AT_CMD_MAX_LEN - *length);
        }

        if (successFlg == VOS_FALSE) {
            AT_WARN_LOG("+CLCK - Unknown class.");
            *result = AT_ERROR;
            return;
        }

    } else { /* 没有查到状态 */
        tmp = 0;
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, tmp);
    }

    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCnfCcwaProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    TAF_UINT8  tmp = 0;
    TAF_UINT32 i;
    VOS_UINT32 customizeFlag;
    VOS_UINT32 successFlg;
    errno_t memResult;

    /* +CCWA: <status>,<class1> */
    if (event->opError == 1) {                                    /* 需要首先判断错误码 */
        *result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
        return;
    }

    if (event->opSsStatus == 1) {
        /* 状态为激活 */
        tmp = (TAF_SS_ACTIVE_STATUS_MASK & event->ssStatus);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, tmp,
            AT_CLCK_PARA_CLASS_ALL);

        /* 输出网络IE SS-STATUS值给用户 */
        customizeFlag = AT_GetSsCustomizePara(AT_SS_CUSTOMIZE_CCWA_QUERY);
        if (customizeFlag == VOS_TRUE) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", event->ssStatus);
        }
    } else if (event->opBsServGroupList == 1) {
        successFlg = VOS_FALSE;
        event->bsServGroupList.cnt = AT_MIN(event->bsServGroupList.cnt, TAF_SS_MAX_NUM_OF_BASIC_SERVICES);
        for (i = 0; i < event->bsServGroupList.cnt; i++) {
            /* 此处用ucTmp保存class，而不是status参数 */
            tmp = At_GetClckClassFromBsCode(&event->bsServGroupList.bsService[i]);
            if (tmp != AT_UNKNOWN_CLCK_CLASS) {
                successFlg = VOS_TRUE;
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d%s",
                    g_parseContext[indexNum].cmdElement->cmdName, 1, tmp, g_atCrLf);
            }
        }

        if (successFlg == VOS_TRUE) {
            *length -= (TAF_UINT16)VOS_StrLen((VOS_CHAR *)g_atCrLf);
            memResult = memset_s((VOS_UINT8 *)g_atSndCodeAddress + *length, AT_CMD_MAX_LEN - *length, 0x0,
                                 AT_CMD_MAX_LEN - *length);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN - *length, AT_CMD_MAX_LEN - *length);
        }

        if (successFlg == VOS_FALSE) {
            AT_WARN_LOG("+CCWA - Unknown class.");
            *result = AT_ERROR;
            return;
        }
    } else { /* 状态为未激活 */
        tmp = 0;
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, tmp);
    }

    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCcbsCnfProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    VOS_UINT32 i = 0;

    if (event->opGenericServiceInfo == 1) {
        if (TAF_SS_PROVISIONED_STATUS_MASK & event->genericServiceInfo.ssStatus) {
            if (event->genericServiceInfo.opCcbsFeatureList == 1) {
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "Queue of Ccbs requests is: ");
                event->genericServiceInfo.ccbsFeatureList.cnt = AT_MIN(event->genericServiceInfo.ccbsFeatureList.cnt,
                                                                       TAF_SS_MAX_NUM_OF_CCBS_FEATURE);
                for (i = 0; i < event->genericServiceInfo.ccbsFeatureList.cnt; i++) {
                    if (event->genericServiceInfo.ccbsFeatureList.ccBsFeature[i].opCcbsIndex == VOS_TRUE) {
                        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
                        *length +=
                            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                (TAF_CHAR *)g_atSndCodeAddress + *length, "Index:%d",
                                event->genericServiceInfo.ccbsFeatureList.ccBsFeature[i].ccbsIndex);
                    }
                }
            } else {
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "Queue of Ccbs is empty");
            }
        } else {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "CCBS not provisioned");
        }
    } else if (event->opSsStatus == 1) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "CCBS not provisioned");
    } else {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "Unknown");
    }
    *result = AT_OK;
}


TAF_VOID At_SsRspInterrogateCnfCmmiProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    if (event->opError == 1) {                                    /* 需要首先判断错误码 */
        *result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
        return;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMMI_QUERY_CLIP) {
        if ((event->opSsStatus == 1) && (TAF_SS_PROVISIONED_STATUS_MASK & event->ssStatus)) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "CLIP provisioned");
        } else if (event->opSsStatus == 0) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "Unknown");
        } else {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "CLIP not provisioned");
        }

        *result = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMMI_QUERY_CLIR) {
        if (event->opGenericServiceInfo == 1) {
            if (TAF_SS_PROVISIONED_STATUS_MASK & event->genericServiceInfo.ssStatus) {
                if (event->genericServiceInfo.opCliStrictOp == 1) {
                    switch (event->genericServiceInfo.cliRestrictionOp) {
                        case TAF_SS_CLI_PERMANENT:
                            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + *length,
                                "CLIR provisioned in permanent mode");
                            break;

                        case TAF_SS_CLI_TMP_DEFAULT_RESTRICTED:
                            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + *length,
                                "CLIR temporary mode presentation restricted");
                            break;

                        case TAF_SS_CLI_TMP_DEFAULT_ALLOWED:
                            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + *length,
                                "CLIR temporary mode presentation allowed");
                            break;

                        default:
                            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + *length, "Unknown");
                            break;
                    }
                } else {
                    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + *length, "Unknown");
                }
            } else {
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "CLIR not provisioned");
            }
        } else if (event->opSsStatus == 1) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "CLIR not provisioned");
        } else {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "Unknown");
        }
        *result = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SS_INTERROGATE_CCBS) {
        At_SsRspInterrogateCcbsCnfProc(indexNum, event, result, length);
    } else {
        *result = AT_ERROR;
    }
}


TAF_VOID At_SsRspInterrogateCnfProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                    TAF_UINT16 *length)
{
    switch (g_parseContext[indexNum].cmdElement->cmdIndex) {
        case AT_CMD_CLIP:
            At_SsRspInterrogateCnfClipProc(indexNum, event, result, length);
            break;

        case AT_CMD_COLP:
            At_SsRspInterrogateCnfColpProc(indexNum, event, result, length);
            break;

        case AT_CMD_CLIR:
            At_SsRspInterrogateCnfClirProc(indexNum, event, result, length);
            break;

        case AT_CMD_CLCK:
            At_SsRspInterrogateCnfClckProc(indexNum, event, result, length);
            break;

        case AT_CMD_CCWA:
            At_SsRspInterrogateCnfCcwaProc(indexNum, event, result, length);
            break;

        case AT_CMD_CCFC:
            /* +CCFC: <status>,<class1>[,<number>,<type>[,<subaddr>,<satype>[,<time>]]] */
            if (event->opError == 1) {                                    /* 需要首先判断错误码 */
                *result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
                break;
            }

            *length = (TAF_UINT16)At_CcfcQryReport(event, indexNum);
            *result = AT_OK;
            break;

        case AT_CMD_CMMI:
            At_SsRspInterrogateCnfCmmiProc(indexNum, event, result, length);
            break;

        case AT_CMD_CNAP:
            AT_SsRspInterrogateCnfCnapProc(indexNum, event, result, length);
            break;

        default:
            break;
    }
}


TAF_VOID At_SsRspUssdProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT16 *length)
{
    TAF_UINT8  tmp             = 0;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* <m> 通知用户不用进一步操作 */
    if (event->ssEvent == TAF_SS_EVT_USS_NOTIFY_IND) {
        tmp = 0;
    /* 通知用户进一步操作 */
    } else if (event->ssEvent == TAF_SS_EVT_USS_REQ_IND) {
        tmp = 1;
    } else {
        if (*systemAppConfig == SYSTEM_APP_ANDROID) {
            tmp = AT_PROCESS_USS_REQ_CNF_TMP;
        } else if ((event->opUssdString == 0) && (event->opUSSData == 0)) {
            tmp = AT_PROCESS_USS_REQ_CNF_TMP;
        } else {
            tmp = 0;
        }
    }

    /* +CUSD: <m>[,<str>,<dcs>] */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "+CUSD: %d", tmp);

    /* <str> */
    /* 部分判断移到函数中了 */
    if (event->opError == 0) {
        *length = AT_PrintUssdStr(event, indexNum, *length);
    }
}


VOS_UINT32 AT_GetSsEventErrorCode(VOS_UINT8 indexNum, TAF_SS_CallIndependentEvent *event)
{
    if (event->opSsStatus == VOS_TRUE) {
        if ((TAF_SS_PROVISIONED_STATUS_MASK & event->ssStatus) == 0) {
            /* 返回业务未签约对应的错误码 */
            return AT_CME_SERVICE_NOT_PROVISIONED;
        }
    }

    return At_ChgTafErrorCode(indexNum, event->errorCode);
}


TAF_VOID At_SsRspProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event)
{
    TAF_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;

    /* CLIP CCWA CCFC CLCK CUSD CPWD */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CUSD_REQ) {
        (VOS_VOID)At_SsRspCusdProc(indexNum, event);
        return;
    }

    if (event->ssEvent == TAF_SS_EVT_ERROR) { /* 如果是ERROR事件，则直接判断错误码 */
        if (event->errorCode == TAF_ERR_USSD_NET_TIMEOUT) {
            length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s+CUSD: %d%s", g_atCrLf, AT_CUSD_M_NETWORK_TIMEOUT, g_atCrLf);

            At_SendResultData(indexNum, g_atSndCodeAddress, length);

            return;
        }

        if (event->errorCode == TAF_ERR_USSD_USER_TIMEOUT) {
            length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s+CUSD: %d%s", g_atCrLf, AT_CUSD_M_NETWORK_CANCEL, g_atCrLf);

            At_SendResultData(indexNum, g_atSndCodeAddress, length);

            return;
        }

        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CURRENT_OPT_BUTT) {
            return;
        }

        /* 从SS Event中获取用于AT返回的错误码 */
        result = AT_GetSsEventErrorCode(indexNum, event);

        AT_STOP_TIMER_CMD_READY(indexNum);
    } else {
        switch (event->ssEvent) {
            /* 其它事件 */
            case TAF_SS_EVT_INTERROGATESS_CNF: /* 查询结果上报 */
                At_SsRspInterrogateCnfProc(indexNum, event, &result, &length);
                AT_STOP_TIMER_CMD_READY(indexNum);
                break;

            case TAF_SS_EVT_ERASESS_CNF:
            case TAF_SS_EVT_REGISTERSS_CNF:
            case TAF_SS_EVT_ACTIVATESS_CNF:
            case TAF_SS_EVT_DEACTIVATESS_CNF:
            case TAF_SS_EVT_REG_PASSWORD_CNF:
            case TAF_SS_EVT_ERASE_CC_ENTRY_CNF:
                if (event->opError == 0) {
                    result = AT_OK;
                } else {
                    result = At_ChgTafErrorCode(indexNum, event->errorCode); /* 发生错误 */
                }
                AT_STOP_TIMER_CMD_READY(indexNum);
                break;

            case TAF_SS_EVT_USS_NOTIFY_IND:           /* 通知用户不用进一步操作 */
            case TAF_SS_EVT_USS_REQ_IND:              /* 通知用户进一步操作 */
            case TAF_SS_EVT_PROCESS_USS_REQ_CNF:      /* 通知用户网络释放 */
            case TAF_SS_EVT_USS_RELEASE_COMPLETE_IND: /* 通知用户网络释放 */
                At_SsRspUssdProc(indexNum, event, &length);
                break;

            /* Delete TAF_SS_EVT_GET_PASSWORD_IND分支 */
            default:
                return;
        }
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}

TAF_VOID At_SsMsgProc(TAF_UINT8 *data, TAF_UINT16 len)
{
    errno_t                      memResult;
    TAF_SS_CallIndependentEvent *event    = TAF_NULL_PTR;
    TAF_UINT8                    indexNum = 0;
    /*lint -save -e830 */
    event = (TAF_SS_CallIndependentEvent *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TAF_SS_CallIndependentEvent));
    /*lint -restore */
    if (event == TAF_NULL_PTR) {
        AT_WARN_LOG("At_SsMsgProc Mem Alloc FAILURE");
        return;
    }
    memset_s(event, sizeof(TAF_SS_CallIndependentEvent), 0x00, sizeof(TAF_SS_CallIndependentEvent));

    if (len > sizeof(TAF_SS_CallIndependentEvent)) {
        AT_WARN_LOG1("At_SsMsgProc: Invalid Para usLen: ", len);
        len = sizeof(TAF_SS_CallIndependentEvent);
    }

    if (len > 0) {
        memResult = memcpy_s(event, sizeof(TAF_SS_CallIndependentEvent), data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_SS_CallIndependentEvent), len);
    }

    AT_LOG1("At_SsMsgProc pEvent->ClientId", event->clientId);
    AT_LOG1("At_SsMsgProc pEvent->SsEvent", event->ssEvent);
    AT_LOG1("At_SsMsgProc pEvent->OP_Error", event->opError);
    AT_LOG1("At_SsMsgProc pEvent->ErrorCode", event->errorCode);
    AT_LOG1("At_SsMsgProc pEvent->SsCode", event->ssCode);
    AT_LOG1("At_SsMsgProc pEvent->Cause", event->cause);

    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SsMsgProc At_ClientIdToUserId FAILURE");
        /*lint -save -e830 */
        PS_MEM_FREE(WUEPS_PID_AT, event);
        /*lint -restore */
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_SsIndProc(indexNum, event);
    } else {
        AT_LOG1("At_SsMsgProc ucIndex", indexNum);
        AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

        At_SsRspProc(indexNum, event);
    }

    PS_MEM_FREE(WUEPS_PID_AT, event);
}


TAF_UINT32 At_PhReadCreg(TAF_PH_RegState *para, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if ((para->act == TAF_PH_ACCESS_TECH_CDMA_1X) || (para->act == TAF_PH_ACCESS_TECH_EVDO)) {
        /* lac */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, ",\"FFFF\"");
        /* ci */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, ",\"FFFFFFFF\"");

        if ((g_reportCregActParaFlg == VOS_TRUE) && (para->act < TAF_PH_ACCESS_TECH_BUTT)) {
            /* act */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",%d", para->act);
        }

        return length;
    }
#endif

    if (para->cellId.cellNum > 0) {
        /* lac */
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        if (para->act == TAF_PH_ACCESS_TECH_NR_5GC) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",\"%X%X%X%X%X%X\"", 0x000f & (para->lac >> 20), 0x000f & (para->lac >> 16),
                0x000f & (para->lac >> 12), 0x000f & (para->lac >> 8), 0x000f & (para->lac >> 4),
                0x000f & (para->lac >> 0));
        } else
#endif
        {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",\"%X%X%X%X\"", 0x000f & (para->lac >> 12), 0x000f & (para->lac >> 8),
                0x000f & (para->lac >> 4), 0x000f & (para->lac >> 0));
        }

        /* ci */
        if (g_ciRptByte == CREG_CGREG_CI_RPT_FOUR_BYTE) {
            /* VDF需求: CREG/CGREG的<CI>域以4字节方式上报 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",\"%08X\"", (para->cellId.cellId[0].cellIdLowBit));
        } else {
            /* <CI>域以2字节方式上报 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",\"%X%X%X%X\"",
                0x000f & (para->cellId.cellId[AT_TAFMMA_CELL_ID_INDEX_0].cellIdLowBit >> 12),
                0x000f & (para->cellId.cellId[AT_TAFMMA_CELL_ID_INDEX_0].cellIdLowBit >> 8),
                0x000f & (para->cellId.cellId[AT_TAFMMA_CELL_ID_INDEX_0].cellIdLowBit >> 4),
                0x000f & (para->cellId.cellId[AT_TAFMMA_CELL_ID_INDEX_0].cellIdLowBit >> 0));
        }

        if ((g_reportCregActParaFlg == VOS_TRUE) && (para->act < TAF_PH_ACCESS_TECH_BUTT)) {
            /* rat */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",%d", para->act);
        }
    }

    return length;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

TAF_UINT32 At_PhReadC5greg(TAF_PH_RegState *para, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    if (para->cellId.cellNum > 0) {
        /* tac */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, ",\"%X%X%X%X%X%X\"", 0x000f & (para->lac >> 20), 0x000f & (para->lac >> 16),
            0x000f & (para->lac >> 12), 0x000f & (para->lac >> 8), 0x000f & (para->lac >> 4),
            0x000f & (para->lac >> 0));

        /* ci */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)dst + length, ",\"%08X%08X\"", para->cellId.cellId[0].cellIdHighBit,
            para->cellId.cellId[0].cellIdLowBit);

        if (g_reportCregActParaFlg == VOS_TRUE) {
            /* rat */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",%d", para->act);
        } else {
            /* rat */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)dst + length, ",");
        }
    }

    return length;
}
#endif


VOS_VOID AT_PhSendPinReady(VOS_UINT16 modemID)
{
    VOS_UINT32 i;
    VOS_UINT16 length;

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if ((modemID == g_atClientCtx[i].clientConfiguration.modemId) && (g_atClientTab[i].userType == AT_APP_USER)) {
            break;
        }
    }

    /* 未找到E5 User,则不用上报 */
    if (i >= AT_MAX_CLIENT_NUM) {
        return;
    }

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^CPINNTY:READY");
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData((VOS_UINT8)i, g_atSndCodeAddress, length);
}


VOS_VOID AT_PhSendNeedPuk(VOS_UINT16 modemID)
{
    VOS_UINT32 i;
    VOS_UINT16 length;

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if ((modemID == g_atClientCtx[i].clientConfiguration.modemId) && (g_atClientTab[i].userType == AT_APP_USER)) {
            break;
        }
    }

    /* 未找到E5 User,则不用上报 */
    if (i >= AT_MAX_CLIENT_NUM) {
        return;
    }

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^CPINNTY:SIM PUK");

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData((VOS_UINT8)i, g_atSndCodeAddress, length);
}


VOS_VOID AT_PhSendSimLocked(VOS_VOID)
{
    VOS_UINT16 length;
    VOS_UINT32 i;

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if (g_atClientTab[i].userType == AT_APP_USER) {
            break;
        }
    }

    /* 未找到E5 User,则不用上报 */
    if (i >= AT_MAX_CLIENT_NUM) {
        return;
    }

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^CARDLOCKNTY:SIM LOCKED");

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData((VOS_UINT8)i, g_atSndCodeAddress, length);
}


VOS_VOID AT_PhSendRoaming(VOS_UINT8 tmpRoamStatus)
{
    VOS_UINT32 i;
    VOS_UINT16 length;
    VOS_UINT8  roamStatus;

    roamStatus = tmpRoamStatus;

    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if (g_atClientTab[i].userType == AT_APP_USER) {
            break;
        }
    }

    /* 未找到E5 User,则不用上报 */
    if (i >= AT_MAX_CLIENT_NUM) {
        return;
    }

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^APROAMRPT:%d%s", g_atCrLf, roamStatus, g_atCrLf);

    At_SendResultData((VOS_UINT8)i, g_atSndCodeAddress, length);
}


VOS_VOID AT_GetOnlyGURatOrder(TAF_MMA_MultimodeRatCfg *ratOrder)
{
    TAF_MMA_MultimodeRatCfg ratOrderInfo;
    VOS_UINT32              i;
    errno_t                 memResult;
    VOS_UINT8               indexNum;

    indexNum = 0;
    memset_s(&ratOrderInfo, (VOS_SIZE_T)sizeof(ratOrderInfo), 0x00, (VOS_SIZE_T)sizeof(ratOrderInfo));

    memResult = memcpy_s(&ratOrderInfo, (VOS_SIZE_T)sizeof(ratOrderInfo), ratOrder, (VOS_SIZE_T)sizeof(ratOrderInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(ratOrderInfo), (VOS_SIZE_T)sizeof(ratOrderInfo));

    /* 获取GU模信息 */
    ratOrderInfo.ratNum = AT_MIN(ratOrderInfo.ratNum, TAF_MMA_RAT_BUTT);
    for (i = 0; i < ratOrderInfo.ratNum; i++) {
        if ((ratOrderInfo.ratOrder[i] == TAF_MMA_RAT_WCDMA) || (ratOrderInfo.ratOrder[i] == TAF_MMA_RAT_GSM)) {
            ratOrder->ratOrder[indexNum] = ratOrderInfo.ratOrder[i];
            indexNum++;
        }
    }

    ratOrder->ratNum             = indexNum;
    ratOrder->ratOrder[indexNum] = TAF_MMA_RAT_BUTT;

    return;
}



VOS_VOID AT_ReportSysCfgQryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum)
{
    AT_SYSCFG_RatTypeUint8 accessMode;
    AT_SYSCFG_RatPrioUint8 acqorder;
    VOS_UINT16             length;

    length = 0;

    /* 从当前接入优先级中提取GU模接入优先级的信息 */
    AT_GetOnlyGURatOrder(&sysCfg->multiModeRatCfg);

    acqorder = sysCfg->userPrio;

    /* 把上报的TAF_MMA_RatOrder结构转换为mode和acqorder */
    sysCfg->multiModeRatCfg.ratNum = AT_MIN(sysCfg->multiModeRatCfg.ratNum, TAF_MMA_RAT_BUTT);
    switch (sysCfg->multiModeRatCfg.ratOrder[0]) {
        case TAF_MMA_RAT_GSM:
            if (AT_IsSupportWMode(&sysCfg->multiModeRatCfg) == VOS_TRUE) {
                accessMode = AT_SYSCFG_RAT_AUTO;
            } else {
                accessMode = AT_SYSCFG_RAT_GSM;
            }
            break;
        case TAF_MMA_RAT_WCDMA:
            if (AT_IsSupportGMode(&sysCfg->multiModeRatCfg) == VOS_TRUE) {
                accessMode = AT_SYSCFG_RAT_AUTO;
            } else {
                accessMode = AT_SYSCFG_RAT_WCDMA;
            }
            break;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case TAF_MMA_RAT_1X:
            if (AT_IsSupportHrpdMode(&sysCfg->multiModeRatCfg) == VOS_TRUE) {
                accessMode = AT_SYSCFG_RAT_1X_AND_HRPD;
            } else {
                accessMode = AT_SYSCFG_RAT_1X;
            }
            break;

        case TAF_MMA_RAT_HRPD:
            if (AT_IsSupport1XMode(&sysCfg->multiModeRatCfg) == VOS_TRUE) {
                accessMode = AT_SYSCFG_RAT_1X_AND_HRPD;
            } else {
                accessMode = AT_SYSCFG_RAT_HRPD;
            }
            break;
#endif

        default:
            /* 只支持L的情况 */
            accessMode = AT_SYSCFG_RAT_AUTO;

            acqorder = AT_SYSCFG_RAT_PRIO_AUTO;
            break;
    }

    /* 按syscfg查询格式上报^SYSCFG:<mode>,<acqorder>,<band>,<roam>,<srvdomain> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    if (sysCfg->guBand.bandHigh == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%X,%d,%d", accessMode, acqorder, sysCfg->guBand.bandLow,
            sysCfg->roam, sysCfg->srvDomain);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%X%08X,%d,%d", accessMode, acqorder,
            sysCfg->guBand.bandHigh, sysCfg->guBand.bandLow, sysCfg->roam, sysCfg->srvDomain);
    }
    g_atSendDataBuff.bufLen = length;

    return;
}


LOCAL VOS_VOID AT_ConvertSysCfgStrToAutoModeStr(VOS_UINT8 *acqOrderBegin, VOS_UINT8 *acqOrder,
                                                VOS_UINT32 acqOrderLength, VOS_UINT8 ratNum)
{
    errno_t returnValue;
#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && (FEATURE_LTE == FEATURE_ON))
    if ((VOS_StrCmp((VOS_CHAR *)acqOrderBegin, "08030201") == 0) && (ratNum == TAF_PH_MAX_GULNR_RAT_NUM)) {
        /* 接入技术的个数为4且接入优先顺序为NR->L->W->G,acqorder上报00 */
        acqOrder    = acqOrderBegin;
        returnValue = strcpy_s((VOS_CHAR *)acqOrder, acqOrderLength, "00");
        TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, acqOrderLength, acqOrderLength);
        acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
        *acqOrder = '\0';
    }
#elif (FEATURE_LTE == FEATURE_ON)
    if ((VOS_StrCmp((VOS_CHAR *)acqOrderBegin, "030201") == 0) && (ratNum == TAF_PH_MAX_GUL_RAT_NUM)) {
        /* 接入技术的个数为3且接入优先顺序为L->W->G,acqorder上报00 */
        acqOrder = acqOrderBegin;
        returnValue = strcpy_s((VOS_CHAR *)acqOrder, acqOrderLength, "00");
        TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, acqOrderLength, acqOrderLength);
        acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
        *acqOrder = '\0';
    }
#elif (FEATURE_UE_MODE_NR == FEATURE_ON)
    if ((VOS_StrCmp((VOS_CHAR *)acqOrderBegin, "080201") == 0) && (ratNum == TAF_PH_MAX_GUNR_RAT_NUM)) {
        /* 接入技术的个数为3且接入优先顺序为NR->W->G,acqorder上报00 */
        acqOrder       = acqOrderBegin;
        returnValue    = strcpy_s((VOS_CHAR *)acqOrder, acqOrderLength, "00");
        TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, acqOrderLength, acqOrderLength);
        acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
        *acqOrder = '\0';
    }
#else
    if ((VOS_StrCmp((VOS_CHAR *)acqOrderBegin, "0201") == 0) && (ratNum == TAF_PH_MAX_GU_RAT_NUM)) {
        /* 接入技术的个数为2且接入优先顺序为W->G,acqorder上报00 */
        acqOrder    = acqOrderBegin;
        returnValue = strcpy_s((VOS_CHAR *)acqOrder, acqOrderLength, "00");
        TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, acqOrderLength, acqOrderLength);
        acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
        *acqOrder = '\0';
    }
#endif

    return;
}


VOS_VOID AT_ConvertSysCfgRatOrderToStr(TAF_MMA_MultimodeRatCfg *ratOrder, VOS_UINT8 *acqOrder, VOS_UINT32 acqOrderLength)
{
    VOS_UINT32 i;
    VOS_UINT8 *acqOrderBegin = VOS_NULL_PTR;
    VOS_UINT32 length;
    errno_t    returnValue;

    acqOrderBegin = acqOrder;
    length        = acqOrderLength;

    ratOrder->ratNum = AT_MIN(ratOrder->ratNum, TAF_MMA_RAT_BUTT);
    for (i = 0; i < ratOrder->ratNum; i++) {
        if (ratOrder->ratOrder[i] == TAF_MMA_RAT_WCDMA) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "02");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        } else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_GSM) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "01");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        }
#if (FEATURE_LTE == FEATURE_ON)
        else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_LTE) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "03");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        }
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_1X) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "04");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        } else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_HRPD) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "07");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        }
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_NR) {
            returnValue = strcpy_s((VOS_CHAR *)acqOrder, length, "08");
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(returnValue, length, length);
            acqOrder += AT_SYSCFGEX_RAT_MODE_STR_LEN;
            length -= AT_SYSCFGEX_RAT_MODE_STR_LEN;
        }
#endif
        else {
        }
    }

    *acqOrder = '\0';

    AT_ConvertSysCfgStrToAutoModeStr(acqOrderBegin, acqOrder, acqOrderLength, ratOrder->ratNum);

    return;
}


VOS_INT8 AT_GetValidLteBandIndex(TAF_USER_SetLtePrefBandInfo *lBand)
{
    VOS_INT8 validIndex;

    if (lBand == VOS_NULL_PTR) {
        return 0;
    }

    for (validIndex = TAF_MMA_LTE_BAND_MAX_LENGTH - 1; validIndex >= 0; validIndex--) {
        if (lBand->bandInfo[validIndex] != 0) {
            return validIndex;
        }
    }

    return 0;
}



VOS_VOID AT_ReportSysCfgExQryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum)
{
    VOS_UINT8  acqorder[TAF_MMA_RAT_BUTT * 2 + 1] = {0};
    VOS_UINT8 *acqOrder = VOS_NULL_PTR;
    VOS_INT8   lteBandIndex;
    VOS_UINT16 length;

    length   = 0;
    acqOrder = acqorder;

    /* 把上报的TAF_MMA_MultimodeRatCfg结构转换为acqorder字符串 */
    AT_ConvertSysCfgRatOrderToStr(&sysCfg->multiModeRatCfg, acqOrder, sizeof(acqorder));

    /* 按syscfgex查询格式上报^SYSCFGEX: <acqorder>,<band>,<roam>,<srvdomain>,<lteband> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    if (sysCfg->guBand.bandHigh == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\",%X,%d,%d", acqOrder, sysCfg->guBand.bandLow, sysCfg->roam,
            sysCfg->srvDomain);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\",%X%08X,%d,%d", acqOrder, sysCfg->guBand.bandHigh,
            sysCfg->guBand.bandLow, sysCfg->roam, sysCfg->srvDomain);
    }
    lteBandIndex = AT_GetValidLteBandIndex(&(sysCfg->lBand));

    if (lteBandIndex == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%X", sysCfg->lBand.bandInfo[0]);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%X", sysCfg->lBand.bandInfo[lteBandIndex]);

        lteBandIndex--;

        while (lteBandIndex >= 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%08X", sysCfg->lBand.bandInfo[lteBandIndex]);

            lteBandIndex--;
        }
    }
    g_atSendDataBuff.bufLen = length;

    return;
}


VOS_UINT32 AT_ConvertSysCfgRatOrderToWs46No(TAF_MMA_MultimodeRatCfg *ratOrder, VOS_UINT32 *proctolNo)
{
    VOS_UINT8                       gsmSupportFlag;
    VOS_UINT8                       wcdmaSupportFlag;
    VOS_UINT8                       lteSupportFlag;
    VOS_UINT32                      i;
    const AT_WS46_RatTransformTbl    ws46RatTransFormTab[] = {
        /*     G,         U,         L,     Ws46No */
        { VOS_TRUE, VOS_FALSE, VOS_FALSE, 12 }, { VOS_FALSE, VOS_TRUE, VOS_FALSE, 22 },
        { VOS_TRUE, VOS_TRUE, VOS_TRUE, 25 },   { VOS_FALSE, VOS_FALSE, VOS_TRUE, 28 },
        { VOS_TRUE, VOS_TRUE, VOS_FALSE, 29 },  { VOS_TRUE, VOS_FALSE, VOS_TRUE, 30 },
        { VOS_FALSE, VOS_TRUE, VOS_TRUE, 31 },
    };

    gsmSupportFlag   = VOS_FALSE;
    wcdmaSupportFlag = VOS_FALSE;
    lteSupportFlag   = VOS_FALSE;
    ratOrder->ratNum = AT_MIN(ratOrder->ratNum, TAF_MMA_RAT_BUTT);
    for (i = 0; i < (VOS_UINT32)ratOrder->ratNum; i++) {
        if (ratOrder->ratOrder[i] == TAF_MMA_RAT_GSM) {
            gsmSupportFlag = VOS_TRUE;
        } else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_WCDMA) {
            wcdmaSupportFlag = VOS_TRUE;
        }
#if (FEATURE_LTE == FEATURE_ON)
        else if (ratOrder->ratOrder[i] == TAF_MMA_RAT_LTE) {
            lteSupportFlag = VOS_TRUE;
        }
#endif
        else {
        }
    }

    for (i = 0; i < sizeof(ws46RatTransFormTab) / sizeof(ws46RatTransFormTab[0]); i++) {
        if ((gsmSupportFlag == ws46RatTransFormTab[i].gsmSupportFlg) &&
            (wcdmaSupportFlag == ws46RatTransFormTab[i].wcdmaSupportFlg) &&
            (lteSupportFlag == ws46RatTransFormTab[i].lteSupportFlg)) {
            *proctolNo = ws46RatTransFormTab[i].ws46No;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_ReportWs46QryCmdResult(TAF_MMA_SysCfgPara *sysCfg, VOS_UINT8 indexNum)
{
    VOS_UINT32 ws46No;
    VOS_UINT16 length;

    ws46No = 0;
    length = 0;

    /* 把上报的TAF_MMA_MultimodeRatCfg结构转换为<n> */
    if (AT_ConvertSysCfgRatOrderToWs46No(&sysCfg->multiModeRatCfg, &ws46No) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* 按+WS46查询格式上报+WS46: <n> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", ws46No);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_VOID AT_ReportCeregResult(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInd, VOS_UINT16 *length)
{
    VOS_UINT32      rst;
    ModemIdUint16   modemId;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    modemId = MODEM_ID_0;

    rst = AT_GetModemIdFromClient(indexNum, &modemId);
    if (rst != VOS_OK) {
        AT_ERR_LOG1("AT_ReportCeregResult:Get ModemID From ClientID fail,ClientID:", indexNum);
        return;
    }

    /* 当前平台是否支持LTE */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_LTE) != VOS_TRUE) {
        return;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if ((netCtx->ceregType == AT_CEREG_RESULT_CODE_BREVITE_TYPE) && (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        /* +CEREG: <stat> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CEREG].text,
            regInd->regStatus.psRegState, g_atCrLf);
    } else if ((netCtx->ceregType == AT_CEREG_RESULT_CODE_ENTIRE_TYPE) &&
               (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        if ((regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
            (regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_ROAM)) {
            /* +CEREG: <stat>[,<lac>,<ci>,[rat]] */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d", g_atCrLf, g_atStringTab[AT_STRING_CEREG].text,
                regInd->regStatus.psRegState);

            /* GU下只上报+CGREG: <stat> */
            if (regInd->regStatus.ratType == TAF_PH_INFO_LTE_RAT) {
                *length += (VOS_UINT16)At_PhReadCreg(&(regInd->regStatus), g_atSndCodeAddress + *length);
            }

            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
        } else {
            /* +CEREG: <stat> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CEREG].text,
                regInd->regStatus.psRegState, g_atCrLf);
        }
    } else {
    }

    return;
}
#endif


VOS_VOID AT_ReportCgregResult(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInd, VOS_UINT16 *length)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if ((netCtx->cgregType == AT_CGREG_RESULT_CODE_BREVITE_TYPE) && (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        /* +CGREG: <stat> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CGREG].text,
            regInd->regStatus.psRegState, g_atCrLf);
    } else if ((netCtx->cgregType == AT_CGREG_RESULT_CODE_ENTIRE_TYPE) &&
               (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        if (((regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
             (regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_ROAM)) &&
            (regInd->regStatus.ratType != TAF_PH_INFO_NR_5GC_RAT)) {
            /* +CGREG: <stat>[,<lac>,<ci>,[rat]] */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d", g_atCrLf, g_atStringTab[AT_STRING_CGREG].text,
                regInd->regStatus.psRegState);

            *length += (VOS_UINT16)At_PhReadCreg(&(regInd->regStatus), g_atSndCodeAddress + *length);

            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
        } else {
            /* +CGREG: <stat> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CGREG].text,
                regInd->regStatus.psRegState, g_atCrLf);
        }
    } else {
    }

    return;
}


VOS_VOID AT_ReportCregResult(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInd, VOS_UINT16 *length)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if ((netCtx->cregType == AT_CREG_RESULT_CODE_BREVITE_TYPE) && (regInd->regStatus.OP_CsRegState == VOS_TRUE)) {
        /* +CREG: <stat> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CREG].text,
            regInd->regStatus.regState, g_atCrLf);
    } else if ((netCtx->cregType == AT_CREG_RESULT_CODE_ENTIRE_TYPE) && (regInd->regStatus.OP_CsRegState == VOS_TRUE)) {
        if ((regInd->regStatus.regState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
            (regInd->regStatus.regState == TAF_PH_REG_REGISTERED_ROAM)) {
            /* +CREG: <stat>[,<lac>,<ci>,[rat]] */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d", g_atCrLf, g_atStringTab[AT_STRING_CREG].text,
                regInd->regStatus.regState);

            *length += (VOS_UINT16)At_PhReadCreg(&(regInd->regStatus), (g_atSndCodeAddress + *length));

            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
        } else {
            /* +CREG: <stat> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_CREG].text,
                regInd->regStatus.regState, g_atCrLf);
        }
    } else {
    }

    return;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_ConvertMultiSNssaiToString(VOS_UINT8 sNssaiNum, PS_S_NSSAI_STRU *sNssai, VOS_CHAR *pcStrNssai,
                                       VOS_UINT32 srcNssaiLength, VOS_UINT32 *dsrLength)
{
    VOS_UINT32 length;
    VOS_UINT32 loop;
    VOS_INT32  bufLen;

    /*
     * 27007 rel15, 10.1.1章节
     * sst                                     only slice/service type (SST) is present
     * sst;mapped_sst                          SST and mapped configured SST are present
     * sst.sd                                  SST and slice differentiator (SD) are present
     * sst.sd;mapped_sst                       SST, SD and mapped configured SST are present
     * sst.sd;mapped_sst.mapped_sd             SST, SD, mapped configured SST and mapped configured SD are present
     */

    length = 0;
    *dsrLength = 0;

    for (loop = 0; loop < sNssaiNum; loop++) {
        if (length >= AT_EVT_MULTI_S_NSSAI_LEN) {
            AT_ERR_LOG1("AT_ConvertMultiSNssaiToString:ERROR: ulLength abnormal:", length);
            *dsrLength = 0;

            return;
        }

        /* 如果有多个S-NSSAI，每个S-NSSAI通过":"分割 */
        if (loop != 0) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, ":");
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, srcNssaiLength - length, (srcNssaiLength - length) - 1);
            if (bufLen > 0) {
                length = length + (VOS_UINT32)bufLen;
            }
        }

        if ((sNssai[loop].bitOpSd == VOS_TRUE) && (sNssai[loop].bitOpMappedSst == VOS_TRUE) &&
            (sNssai[loop].bitOpMappedSd == VOS_TRUE)) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1,
                                "%02x.%06x;%02x.%06x", sNssai[loop].ucSst, sNssai[loop].ulSd, sNssai[loop].ucMappedSst,
                                sNssai[loop].ulMappedSd);
            /* sst/sd/mapped sst/mapped sd共8个字节 */
            *dsrLength += 8;
        } else if ((sNssai[loop].bitOpSd == VOS_TRUE) && (sNssai[loop].bitOpMappedSst == VOS_TRUE)) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1,
                                "%02x.%06x;%02x", sNssai[loop].ucSst, sNssai[loop].ulSd, sNssai[loop].ucMappedSst);
            /* sst/sd/mapped sst共5个字节 */
            *dsrLength += 5;
        } else if (sNssai[loop].bitOpSd == VOS_TRUE) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, "%02x.%06x",
                                sNssai[loop].ucSst, sNssai[loop].ulSd);
            /* sst/sd共4个字节 */
            *dsrLength += 4;
        } else if (sNssai[loop].bitOpMappedSst == VOS_TRUE) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, "%02x;%02x",
                                sNssai[loop].ucSst, sNssai[loop].ucMappedSst);
            /* sst/mapped sst共2个字节 */
            *dsrLength += 2;
        } else {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, "%02x",
                                sNssai[loop].ucSst);
            /* sst1个字节 */
            *dsrLength += 1;
        }
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, srcNssaiLength - length, (srcNssaiLength - length) - 1);

        if (bufLen > 0) {
            length = length + (VOS_UINT32)bufLen;
        }
    }

    return;
}


VOS_VOID AT_ConvertMultiRejectSNssaiToString(VOS_UINT8 sNssaiNum, PS_REJECTED_SNssai *sNssai, VOS_CHAR *pcStrNssai,
                                             VOS_UINT32 srcNssaiLength, VOS_UINT32 *dsrLength)
{
    VOS_UINT32 length;
    VOS_UINT32 loop;
    VOS_INT32  bufLen;

    /*
     * 27007 rel15, 10.1.1章节
     * sst                                     only slice/service type (SST) is present
     * sst.sd                                  SST and slice differentiator (SD) are present
     */

    length = 0;
    *dsrLength = 0;

    for (loop = 0; loop < sNssaiNum; loop++) {
        if (length >= AT_EVT_MULTI_S_NSSAI_LEN) {
            AT_ERR_LOG1("AT_ConvertMultiRejectSNssaiToString :ERROR: ulLength abnormal:", length);
            *dsrLength = 0;

            return;
        }

        /* 如果有多个S-NSSAI，每个S-NSSAI通过":"分割 */
        if (loop != 0) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, ":");
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, srcNssaiLength - length, (srcNssaiLength - length) - 1);
            length = length + (VOS_UINT32)bufLen;
        }

        if (sNssai[loop].opSd == VOS_TRUE) {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, "%02x.%06x#%x",
                                sNssai[loop].sst, sNssai[loop].sd, sNssai[loop].cause);
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, srcNssaiLength - length, (srcNssaiLength - length) - 1);
            length = length + (VOS_UINT32)bufLen;
            /* sst/sd共4个字节 */
            *dsrLength += 4;
        } else {
            bufLen = snprintf_s(pcStrNssai + length, srcNssaiLength - length, (srcNssaiLength - length) - 1, "%02x#%x",
                                sNssai[loop].sst, sNssai[loop].cause);
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, srcNssaiLength - length, (srcNssaiLength - length) - 1);
            length = length + (VOS_UINT32)bufLen;
            /* sst1个字节 */
            *dsrLength += 1;
        }
    }

    return;
}


LOCAL VOS_VOID AT_ReportC5gregResult(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInd, VOS_UINT16 *length)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT32      rst;
    ModemIdUint16   modemId;
    VOS_UINT32      lengthTemp;
    VOS_CHAR        acStrAllowedNssai[AT_EVT_MULTI_S_NSSAI_LEN];

    modemId = MODEM_ID_0;

    rst = AT_GetModemIdFromClient(indexNum, &modemId);
    if (rst != VOS_OK) {
        AT_ERR_LOG1("AT_ReportC5gregResult: ERROR: Get ModemID From ClientID fail,ClientID:", indexNum);
        return;
    }

    /* 当前平台是否支持NR */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_NR) != VOS_TRUE) {
        return;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if ((netCtx->c5gregType == AT_C5GREG_RESULT_CODE_BREVITE_TYPE) && (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        /* +C5GREG: <stat> */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_C5GREG].text,
            regInd->regStatus.psRegState, g_atCrLf);
    } else if ((netCtx->c5gregType == AT_C5GREG_RESULT_CODE_ENTIRE_TYPE) &&
               (regInd->regStatus.OP_PsRegState == VOS_TRUE)) {
        if ((regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
            (regInd->regStatus.psRegState == TAF_PH_REG_REGISTERED_ROAM)) {
            /* +C5GREG:
             * <stat>[,<lac>,<ci>,[rat],[<Allowed_NSSAI_length>],<Allowed_NSSAI>][,<cause_type>,<reject_cause>]]   */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                              (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d", g_atCrLf,
                                              g_atStringTab[AT_STRING_C5GREG].text, regInd->regStatus.psRegState);

            if (regInd->regStatus.ratType == TAF_PH_INFO_NR_5GC_RAT) {
                *length += (VOS_UINT16)At_PhReadC5greg(&(regInd->regStatus), g_atSndCodeAddress + *length);

                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                                  (VOS_CHAR *)g_atSndCodeAddress + *length, ",");

                /* 组装切片消息格式 */
                lengthTemp = 0;
                memset_s(acStrAllowedNssai, sizeof(acStrAllowedNssai), 0, sizeof(acStrAllowedNssai));

                AT_ConvertMultiSNssaiToString(AT_MIN(regInd->regStatus.allowedNssai.snssaiNum,
                                                     PS_MAX_ALLOWED_S_NSSAI_NUM),
                                              &regInd->regStatus.allowedNssai.snssai[0], acStrAllowedNssai,
                                              sizeof(acStrAllowedNssai), &lengthTemp);

                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                                  (VOS_CHAR *)g_atSndCodeAddress + *length, "%d,", lengthTemp);

                if (lengthTemp != 0) {
                    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                                      (VOS_CHAR *)g_atSndCodeAddress + *length, "\"%s\"",
                                                      acStrAllowedNssai);
                }
            }

            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
        } else {
            /* +C5GREG: <stat> */
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                              (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%s%d%s", g_atCrLf,
                                              g_atStringTab[AT_STRING_C5GREG].text, regInd->regStatus.psRegState,
                                              g_atCrLf);
        }
    } else {
    }

    return;
}

#endif


VOS_VOID AT_ProcRegStatusInfoInd(VOS_UINT8 indexNum, TAF_MMA_RegStatusInd *regInfo)
{
    VOS_UINT16 length;

    length = 0;

    AT_ReportCregResult(indexNum, regInfo, &length);

    AT_ReportCgregResult(indexNum, regInfo, &length);

#if (FEATURE_LTE == FEATURE_ON)
    /* 通过NV判断当前是否支持LTE */
    AT_ReportCeregResult(indexNum, regInfo, &length);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    /* 通过NV判断当前是否支持NR */
    AT_ReportC5gregResult(indexNum, regInfo, &length);
#endif

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_VOID AT_ProcUsimInfoInd(VOS_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_ProcUsimInfoInd: Get modem id fail.");
        return;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^SIMST:%d,%d%s", g_atCrLf, event->simStatus, event->meLockStatus,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_VOID At_RcvMmaPsInitResultIndProc(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length = 0;

    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_RcvMmaPsInitResultIndProc: Get modem id fail.");
        return;
    }

    if (event->opPsInitRslt == VOS_FALSE) {
        AT_ERR_LOG("At_RcvMmaPsInitResultIndProc: invalid msg.");
        return;
    }

    /* 只有modem初始化成功才调用底软接口操作 */
    if (event->psInitRslt == TAF_MMA_PS_INIT_SUCC) {
        /* 拉GPIO管脚通知AP侧MODEM已经OK */
        DRV_OS_STATUS_SWITCH(VOS_TRUE);

        /* 收到PS INIT上报后写设备节点，启动成功 */
        AT_SetModemState(modemId, VOS_TRUE);

        dms_set_modem_status(modemId);
    }
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^PSINIT: %d%s", g_atCrLf, event->psInitRslt, g_atCrLf);

    At_SendResultData((VOS_UINT8)indexNum, g_atSndCodeAddress, length);
}

/*
 * 功能描述: 处理TAF_PH_EVT_USIM_MATCH_FILES_IND事件
 */
VOS_VOID AT_RcvMmaUsimMatchFilesInd(VOS_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaUsimMatchFilesInd: Get modem id fail.");
        return;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^MATCHFILE: %d,%d,%d,", g_atCrLf, event->usimMatchFile.fileType,
        event->usimMatchFile.totalLen, event->usimMatchFile.curLen);

    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + length, event->usimMatchFile.content,
                                                  event->usimMatchFile.curLen);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return;
}


VOS_VOID AT_ProcReportSimSqInfo(VOS_UINT8 indexNum, TAF_MMA_SimsqStateUint32 simsq)
{
    VOS_UINT8  simsqEnable;
    VOS_UINT16 length;

    simsqEnable = At_GetSimsqEnable();

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^SIMSQ: %d,%d%s", g_atCrLf, simsqEnable, simsq, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return;
}


VOS_VOID AT_ProcSimsqInd(VOS_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    VOS_UINT8     simsqEnable;
    ModemIdUint16 modemId;

    /* 将设置参数保存到CC上下文中 */
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_LOG1("AT_ProcSimsqInd AT_GetModemIdFromClient fail", indexNum);
        return;
    }

    simsqEnable = At_GetSimsqEnable();

    At_SetSimsqStatus(modemId, event->simStatus);

    if (simsqEnable == VOS_FALSE) {
        return;
    }

    AT_ProcReportSimSqInfo(indexNum, At_GetSimsqStatus(modemId));

    return;
}


TAF_VOID At_PhIndProc(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    switch (event->phoneEvent) {
        case TAF_PH_EVT_USIM_INFO_IND:
            AT_ProcUsimInfoInd(indexNum, event);
            return;

        case TAF_PH_EVT_SIMSQ_IND:
            AT_ProcSimsqInd(indexNum, event);
            return;

        case TAF_MMA_EVT_PS_INIT_RESULT_IND:
            At_RcvMmaPsInitResultIndProc(indexNum, event);
            return;

        case TAF_PH_EVT_OPER_MODE_IND:
            AT_NORM_LOG("At_PhIndProc TAF_PH_EVT_OPER_MODE_IND Do nothing");
            return;

        case MN_PH_EVT_SIMLOCKED_IND:
            AT_PhSendSimLocked();
            break;

        case MN_PH_EVT_ROAMING_IND:
            AT_PhSendRoaming(event->roamStatus);
            break;

        case TAF_PH_EVT_NSM_STATUS_IND:
            AT_RcvMmaNsmStatusInd(indexNum, event);
            break;

        case TAF_PH_EVT_USIM_MATCH_FILES_IND:
            AT_RcvMmaUsimMatchFilesInd(indexNum, event);
            break;
        default:
            AT_WARN_LOG("At_PhIndProc Other PhoneEvent");
            return;
    }
}

/* AT_PhnEvtPlmnList */


VOS_VOID At_QryCpinRspProc(VOS_UINT8 indexNum, TAF_PH_PIN_TYPE pinType, VOS_UINT16 *length)
{
    if (pinType == TAF_SIM_PIN) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PIN");
    } else if (pinType == TAF_SIM_PUK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PUK");
    } else if (pinType == TAF_PHNET_PIN) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-NET PIN");
    } else if (pinType == TAF_PHNET_PUK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-NET PUK");
    } else if (pinType == TAF_PHNETSUB_PIN) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-NETSUB PIN");
    } else if (pinType == TAF_PHNETSUB_PUK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-NETSUB PUK");
    } else if (pinType == TAF_PHSP_PIN) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-SP PIN");
    } else if (pinType == TAF_PHSP_PUK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-SP PUK");
    } else if (pinType == TAF_PHCP_PIN) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-CP PIN");
    } else if (pinType == TAF_PHCP_PUK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "PH-CP PUK");
    } else {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "READY");
    }

    return;
}

#if (FEATURE_LTE == FEATURE_ON)

TAF_UINT32 AT_ProcOperModeWhenLteOn(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
#else
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    atDevCmdCtrl = AT_GetDevCmdCtrl();
    return atSetTmodePara(indexNum, atDevCmdCtrl->currentTMode);
#else
    atMtInfoCtx = AT_GetMtInfoCtx();
    return atSetTmodePara(indexNum, atMtInfoCtx->currentTMode);
#endif
}
#endif


TAF_UINT32 At_ProcPinQuery(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_OK;

    /* AT+CLCK */
    if (g_parseContext[indexNum].cmdElement->cmdIndex == AT_CMD_CLCK) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        if (event->pinCnf.queryResult.usimmEnableFlg == TAF_PH_USIMM_ENABLE) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "1");
        } else {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "0");
        }
    }

    /* AT^CPIN */
    else if (g_parseContext[indexNum].cmdElement->cmdIndex == AT_CMD_CPIN_2) {
        if (event->pinCnf.pinType == TAF_SIM_PIN) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            *length +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PIN,%d,%d,%d,%d,%d",
                    event->pinCnf.remainTime.pin1RemainTime, event->pinCnf.remainTime.puk1RemainTime,
                    event->pinCnf.remainTime.pin1RemainTime, event->pinCnf.remainTime.puk2RemainTime,
                    event->pinCnf.remainTime.pin2RemainTime);
        } else if (event->pinCnf.pinType == TAF_SIM_PUK) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            *length +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PUK,%d,%d,%d,%d,%d",
                    event->pinCnf.remainTime.puk1RemainTime, event->pinCnf.remainTime.puk1RemainTime,
                    event->pinCnf.remainTime.pin1RemainTime, event->pinCnf.remainTime.puk2RemainTime,
                    event->pinCnf.remainTime.pin2RemainTime);
        } else {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "READY,,%d,%d,%d,%d",
                event->pinCnf.remainTime.puk1RemainTime, event->pinCnf.remainTime.pin1RemainTime,
                event->pinCnf.remainTime.puk2RemainTime, event->pinCnf.remainTime.pin2RemainTime);
        }
    }
    /* AT+CPIN */
    else {
        At_QryCpinRspProc(indexNum, event->pinCnf.pinType, length);
    }

    return result;
}

TAF_UINT32 At_ProcPin2Query(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_FAILURE;

    /* AT^CPIN2 */
    if (g_parseContext[indexNum].cmdElement->cmdIndex == AT_CMD_CPIN2) {
        if (event->pinCnf.pinType == TAF_SIM_PIN2) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            *length +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PIN2,%d,%d,%d,%d,%d",
                    event->pinCnf.remainTime.pin2RemainTime, event->pinCnf.remainTime.puk1RemainTime,
                    event->pinCnf.remainTime.pin1RemainTime, event->pinCnf.remainTime.puk2RemainTime,
                    event->pinCnf.remainTime.pin2RemainTime);
        } else if (event->pinCnf.pinType == TAF_SIM_PUK2) {
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            *length +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "SIM PUK2,%d,%d,%d,%d,%d",
                    event->pinCnf.remainTime.puk2RemainTime, event->pinCnf.remainTime.puk1RemainTime,
                    event->pinCnf.remainTime.pin1RemainTime, event->pinCnf.remainTime.puk2RemainTime,
                    event->pinCnf.remainTime.pin2RemainTime);
        } else {
            result = AT_CME_SIM_FAILURE;

            return result;
        }
    } else {
        result = AT_ERROR;

        return result;
    }

    result = AT_OK;

    return result;
}

TAF_UINT32 At_ProcPinResultPinOk(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, VOS_BOOL *pbNeedRptPinReady,
                                 TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_FAILURE;

    switch (event->pinCnf.cmdType) {
        case TAF_PIN_QUERY:

            result = At_ProcPinQuery(event, length, indexNum);

            break;

        case TAF_PIN2_QUERY:

            result = At_ProcPin2Query(event, length, indexNum);

            break;

        case TAF_PIN_VERIFY:
        case TAF_PIN_UNBLOCK:
            if (event->pinCnf.pinType == TAF_SIM_NON) {
                result = AT_ERROR;
            } else {
                result = AT_OK;

                *pbNeedRptPinReady = VOS_TRUE;
            }

            break;

        case TAF_PIN_CHANGE:
        case TAF_PIN_DISABLE:
        case TAF_PIN_ENABLE:

            result = AT_OK;

            break;

        default:

            return AT_RRETURN_CODE_BUTT;
    }

    return result;
}

TAF_UINT32 At_ProcPinResultNotPinOk(TAF_PHONE_EventInfo *event, VOS_BOOL *pbNeedRptNeedPuk)
{
    TAF_UINT32 result;

    result = AT_FAILURE;

    switch (event->pinCnf.opPinResult) {
        case TAF_PH_OP_PIN_NEED_PIN1:
            result = AT_CME_SIM_PIN_REQUIRED;
            break;

        case TAF_PH_OP_PIN_NEED_PUK1:
            *pbNeedRptNeedPuk = VOS_TRUE;
            result            = AT_CME_SIM_PUK_REQUIRED;
            break;

        case TAF_PH_OP_PIN_NEED_PIN2:
            result = AT_CME_SIM_PIN2_REQUIRED;
            break;

        case TAF_PH_OP_PIN_NEED_PUK2:
            result = AT_CME_SIM_PUK2_REQUIRED;
            break;

        case TAF_PH_OP_PIN_INCORRECT_PASSWORD:
            result = AT_CME_INCORRECT_PASSWORD;
            break;

        case TAF_PH_OP_PIN_OPERATION_NOT_ALLOW:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_PH_OP_PIN_SIM_FAIL:
            result = AT_CME_SIM_FAILURE;
            break;

        default:
            result = AT_CME_UNKNOWN;
            break;
    }

    return result;
}

TAF_UINT32 At_ProcPhoneEvtOperPinCnf(TAF_PHONE_EventInfo *event, ModemIdUint16 modemId, TAF_UINT8 indexNum)
{
    TAF_UINT16 length;
    TAF_UINT32 result;
    VOS_BOOL   bNeedRptPinReady;
    VOS_BOOL   bNeedRptNeedPuk;

    result           = AT_FAILURE;
    length           = 0;
    bNeedRptPinReady = VOS_FALSE;
    bNeedRptNeedPuk  = VOS_FALSE;

    if (event->opPhoneError == 1) {                               /* MT本地错误 */
        result = At_ChgTafErrorCode(indexNum, event->phoneError); /* 发生错误 */

        if ((result == AT_CME_SIM_PUK_REQUIRED) && (event->pinCnf.cmdType == TAF_PIN_VERIFY)) {
            bNeedRptNeedPuk = VOS_TRUE;
        }
    } else {
        if (event->pinCnf.opPinResult == TAF_PH_OP_PIN_OK) {
            result = At_ProcPinResultPinOk(event, &length, &bNeedRptPinReady, indexNum);

            if (result == AT_RRETURN_CODE_BUTT) {
                AT_NORM_LOG("At_ProcPhoneEvtOperPinCnf: return AT_RRETURN_CODE_BUTT");

                return AT_RRETURN_CODE_BUTT;
            }
        } else {
            result = At_ProcPinResultNotPinOk(event, &bNeedRptNeedPuk);
        }
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    if (bNeedRptPinReady == VOS_TRUE) {
        AT_PhSendPinReady(modemId);
    }

    if (bNeedRptNeedPuk == VOS_TRUE) {
        AT_PhSendNeedPuk(modemId);
    }

    return AT_RRETURN_CODE_BUTT;
}

TAF_UINT32 At_ProcPhoneEvtOperModeCnf(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, ModemIdUint16 modemId,
                                      TAF_UINT8 indexNum)
{
    TAF_UINT32 result;
#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT8 sptLteFlag;
    VOS_UINT8 sptUtralTDDFlag;
#endif

    result = AT_FAILURE;

    if (event->opPhoneError == 1) {                               /* MT本地错误 */
        result = At_ChgTafErrorCode(indexNum, event->phoneError); /* 发生错误 */
    } else if (event->operMode.cmdType == TAF_PH_CMD_QUERY) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%d", event->operMode.phMode);
        result = AT_OK;
    } else {
        result = AT_OK;
    }

    /* V7R2 ^PSTANDBY命令复用关机处理流程 */
    if ((AT_LTE_CmdCurrentOpt)g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PSTANDBY_SET) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        return AT_RRETURN_CODE_BUTT;
    }

#if ((FEATURE_LTE == FEATURE_ON) || (FEATURE_UE_MODE_TDS == FEATURE_ON))
    /* 如果GU处理结果正确，则发送到TL测并等待结果 */
    if (result == AT_OK) {
        sptLteFlag      = AT_IsModemSupportRat(modemId, TAF_MMA_RAT_LTE);
        sptUtralTDDFlag = AT_IsModemSupportUtralTDDRat(modemId);

        if ((sptLteFlag == VOS_TRUE) || (sptUtralTDDFlag == VOS_TRUE)) {
            if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_TMODE_SET) ||
                (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SET_TMODE)) {
                AT_ProcOperModeWhenLteOn(indexNum);

                return AT_RRETURN_CODE_BUTT;
            }
        }
    }
#endif

    AT_STOP_TIMER_CMD_READY(indexNum);

    return result;
}

TAF_UINT32 At_ProcMePersonalizationOpRsltOk(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;
    TAF_UINT32 tmp;

    result = AT_FAILURE;
    tmp    = 0;

    switch (event->mePersonalisation.cmdType) {
        case TAF_ME_PERSONALISATION_ACTIVE:
        case TAF_ME_PERSONALISATION_DEACTIVE:
        case TAF_ME_PERSONALISATION_SET:
        case TAF_ME_PERSONALISATION_PWD_CHANGE:
        case TAF_ME_PERSONALISATION_VERIFY:
            result = AT_OK;
            break;

        case TAF_ME_PERSONALISATION_QUERY:
            if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CARD_LOCK_READ) {
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "%d,",
                    event->mePersonalisation.unReportContent.OperatorLockInfo.operatorLockStatus);
                *length +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + *length, "%d,",
                        event->mePersonalisation.unReportContent.OperatorLockInfo.remainTimes);
                if ((event->mePersonalisation.unReportContent.OperatorLockInfo.operatorLen <
                     TAF_PH_ME_LOCK_OPER_LEN_MIN) ||
                    (event->mePersonalisation.unReportContent.OperatorLockInfo.operatorLen >
                     TAF_PH_ME_LOCK_OPER_LEN_MAX)) {
                    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + *length, "0");
                } else {
                    for (tmp = 0; tmp < event->mePersonalisation.unReportContent.OperatorLockInfo.operatorLen; tmp++) {
                        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + *length, "%d",
                            event->mePersonalisation.unReportContent.OperatorLockInfo.operatorArray[tmp]);
                    }
                }
                result = AT_OK;
            } else {
                *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                if (event->mePersonalisation.activeStatus == TAF_ME_PERSONALISATION_ACTIVE_STATUS) {
                    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + *length, "1");
                } else {
                    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + *length, "0");
                }
                result = AT_OK;
            }
            break;

        case TAF_ME_PERSONALISATION_RETRIEVE:
            *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            event->mePersonalisation.unReportContent.SimPersionalisationStr.dataLen = AT_MIN(
                event->mePersonalisation.unReportContent.SimPersionalisationStr.dataLen,
                TAF_MAX_IMSI_LEN);
            for (tmp = 0; tmp < event->mePersonalisation.unReportContent.SimPersionalisationStr.dataLen; tmp++) {
                *(g_atSndCodeAddress + *length + tmp) =
                    event->mePersonalisation.unReportContent.SimPersionalisationStr.simPersonalisationStr[tmp] + 0x30;
            }
            *length += (VOS_UINT16)tmp;
            break;

        default:
            result = AT_ERROR;
            break;
    }

    return result;
}


TAF_UINT32 At_ProcPhoneEvtMePersonalizationCnf(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_FAILURE;

    if (event->mePersonalisation.opRslt != TAF_PH_ME_PERSONALISATION_OK) {
        if (event->mePersonalisation.opRslt == TAF_PH_ME_PERSONALISATION_NO_SIM) {
            result = At_ChgTafErrorCode(indexNum, TAF_ERR_CMD_TYPE_ERROR);
        } else if (event->mePersonalisation.opRslt == TAF_PH_ME_PERSONALISATION_OP_NOT_ALLOW) {
            result = AT_CME_OPERATION_NOT_ALLOWED;
        } else if (event->mePersonalisation.opRslt == TAF_PH_ME_PERSONALISATION_WRONG_PWD) {
            result = AT_CME_INCORRECT_PASSWORD;
        } else {
            result = AT_ERROR;
        }
    } else {
        result = At_ProcMePersonalizationOpRsltOk(event, length, indexNum);
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    return result;
}

TAF_UINT32 At_ProcPhoneEvtPlmnListRej(TAF_PHONE_EventInfo *event, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_FAILURE;

    if (event->phoneError == TAF_ERR_NO_RF) {
        result = AT_CME_NO_RF;
    } else {
        result = AT_CME_OPERATION_NOT_ALLOWED;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    return result;
}

TAF_UINT32 At_ProcPhoneEvtUsimResponse(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_OK;

    /* +CSIM:  */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (event->opUsimAccessData == 1) {
        /* <length>, */
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%d,\"",
            event->usimAccessData.len * AT_ASCII_AND_HEX_CONVERSION_FACTOR);
        /* <command>, */
        *length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                       (TAF_UINT8 *)g_atSndCodeAddress + *length,
                                                       event->usimAccessData.response, event->usimAccessData.len);
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "\"");
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    return result;
}

TAF_UINT32 At_ProcPhoneEvtRestrictedAccessCnf(TAF_PHONE_EventInfo *event, TAF_UINT16 *length, TAF_UINT8 indexNum)
{
    TAF_UINT32 result;

    result = AT_OK;

    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (event->opUsimRestrictAccess == 1) {
        /* <sw1, sw2>, */
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "%d,%d", event->restrictedAccess.sW1,
            event->restrictedAccess.sW2);

        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, ",\"");

        if (event->restrictedAccess.len != 0) {
            /* <response> */
            *length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                           (TAF_UINT8 *)g_atSndCodeAddress + *length,
                                                           event->restrictedAccess.content,
                                                           event->restrictedAccess.len);
        }

        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + *length, "\"");
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    return result;
}


TAF_VOID At_PhRspProc(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    TAF_UINT32    result;
    TAF_UINT32    rst;
    TAF_UINT16    length;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;
    result  = AT_FAILURE;
    length  = 0;

    rst = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rst != VOS_OK) {
        AT_ERR_LOG1("At_PhRspProc:Get ModemID From ClientID fail,ClientID:", indexNum);

        return;
    }

    switch (event->phoneEvent) {
        case TAF_PH_EVT_ERR:

            result = At_ChgTafErrorCode(indexNum, event->phoneError); /* 发生错误 */

            AT_STOP_TIMER_CMD_READY(indexNum);

            break;

        case TAF_PH_EVT_PLMN_LIST_REJ:

            result = At_ProcPhoneEvtPlmnListRej(event, indexNum);

            break;

        case TAF_PH_EVT_OP_PIN_CNF:

            result = At_ProcPhoneEvtOperPinCnf(event, modemId, indexNum);

            if (result == AT_RRETURN_CODE_BUTT) {
                return;
            }

            break;

        case TAF_PH_EVT_OPER_MODE_CNF:

            result = At_ProcPhoneEvtOperModeCnf(event, &length, modemId, indexNum);

            if (result == AT_RRETURN_CODE_BUTT) {
                return;
            }

            break;

        case TAF_PH_EVT_USIM_RESPONSE:

            result = At_ProcPhoneEvtUsimResponse(event, &length, indexNum);

            break;

        case TAF_PH_EVT_RESTRICTED_ACCESS_CNF:

            result = At_ProcPhoneEvtRestrictedAccessCnf(event, &length, indexNum);

            break;

        case TAF_PH_EVT_OP_PINREMAIN_CNF:

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%d,%d ", event->pinRemainCnf.pIN1Remain,
                event->pinRemainCnf.pUK1Remain, event->pinRemainCnf.pIN2Remain, event->pinRemainCnf.pUK2Remain);

            AT_STOP_TIMER_CMD_READY(indexNum);

            result = AT_OK;

            break;

        case TAF_PH_EVT_ME_PERSONALISATION_CNF:

            result = At_ProcPhoneEvtMePersonalizationCnf(event, &length, indexNum);

            break;

        case TAF_PH_EVT_SETUP_SYSTEM_INFO_RSP:

            AT_NORM_LOG("At_PhRspProc EVT SETUP SYSTEM INFO RSP,Do nothing.");

            return;

        case TAF_PH_EVT_PLMN_LIST_ABORT_CNF:

            /*
             * 容错处理, 当前不在列表搜ABORT过程中则不上报ABORT.
             * 如AT的ABORT保护定时器已超时, 之后再收到MMA的ABORT_CNF则不上报ABORT
             */
            if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_ABORT_PLMN_LIST) {
                AT_WARN_LOG("At_PhRspProc  NOT ABORT PLMN LIST. ");

                return;
            }

            result = AT_ABORT;

            AT_STOP_TIMER_CMD_READY(indexNum);

            break;

        default:

            AT_WARN_LOG("At_PhRspProc Other PhoneEvent");

            return;
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);
}

TAF_VOID At_PhEventProc(TAF_UINT8 *data, TAF_UINT16 len)
{
    TAF_PHONE_EventInfo *event    = VOS_NULL_PTR;
    TAF_UINT8            indexNum = 0;

    event = (TAF_PHONE_EventInfo *)data;

    AT_LOG1("At_PhMsgProc pEvent->ClientId", event->clientId);
    AT_LOG1("At_PhMsgProc PhoneEvent", event->phoneEvent);
    AT_LOG1("At_PhMsgProc PhoneError", event->phoneError);

    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_PhRspProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_PhIndProc(indexNum, event);
    } else {
        AT_LOG1("At_PhMsgProc ucIndex", indexNum);
        AT_LOG1("g_atClientTab[ucIndex].CmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

        At_PhRspProc(indexNum, event);
    }
}
#if (FEATURE_CSG == FEATURE_ON)

VOS_VOID AT_ReportCsgListSearchCnfResult(TAF_MMA_CsgListCnfPara *csgList, VOS_UINT16 *length)
{
    VOS_UINT16 lengthTemp;
    VOS_UINT8  homeNodeBLen;
    VOS_UINT8  csgTypeLen;
    VOS_UINT32 i;
    VOS_UINT32 j;

    lengthTemp = *length;

    for (i = 0; i < (VOS_UINT32)AT_MIN(csgList->plmnWithCsgIdNum, TAF_MMA_MAX_CSG_ID_LIST_NUM); i++) {
        /* 除第一项外，其它以前要加逗号 */
        if ((i != 0) || (csgList->currIndex != 0)) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",");
        }

        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, "(");

        /* 打印长短名 */
        if ((csgList->csgIdListInfo[i].operatorNameLong[0] == '\0') ||
            (csgList->csgIdListInfo[i].operatorNameShort[0] == '\0')) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, "\"\"");
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",\"\"");
        } else {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, "\"%s\"", csgList->csgIdListInfo[i].operatorNameLong);
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",\"%s\"", csgList->csgIdListInfo[i].operatorNameShort);
        }

        /* 打印数字格式的运营商名称  */
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",\"%X%X%X",
            (0x0f00 & csgList->csgIdListInfo[i].plmnId.mcc) >> AT_OCTET_MOVE_EIGHT_BITS,
            (AT_OCTET_HIGH_FOUR_BITS & csgList->csgIdListInfo[i].plmnId.mcc) >> AT_OCTET_MOVE_FOUR_BITS,
            (AT_OCTET_LOW_FOUR_BITS & csgList->csgIdListInfo[i].plmnId.mcc));

        if (((0x0f00 & csgList->csgIdListInfo[i].plmnId.mnc) >> AT_OCTET_MOVE_EIGHT_BITS) != AT_OCTET_LOW_FOUR_BITS) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, "%X",
                (0x0f00 & csgList->csgIdListInfo[i].plmnId.mnc) >> AT_OCTET_MOVE_EIGHT_BITS);
        }

        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, "%X%X\"",
            (AT_OCTET_HIGH_FOUR_BITS & csgList->csgIdListInfo[i].plmnId.mnc) >> AT_OCTET_MOVE_FOUR_BITS,
            (AT_OCTET_LOW_FOUR_BITS & csgList->csgIdListInfo[i].plmnId.mnc));

        /* 打印CSG ID */
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",\"%X\"", csgList->csgIdListInfo[i].csgId);

        /*
         * 打印CSG ID TYPE, 1：CSG ID在Allowed CSG List中; 2：CSG ID在Operator CSG List中不在禁止CSG ID列表中;
         * 3：CSG ID在Operator CSG List中并且在禁止CSG ID列表中; 4：CSG ID不在Allowed CSG List和Operator CSG List中
         */
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d,", csgList->csgIdListInfo[i].plmnWithCsgIdType);

        /* 打印home NodeB Name, ucs2编码，最大长度48字节 */
        homeNodeBLen = AT_MIN(csgList->csgIdListInfo[i].csgIdHomeNodeBName.homeNodeBNameLen,
                              TAF_MMA_MAX_HOME_NODEB_NAME_LEN);

        for (j = 0; j < homeNodeBLen; j++) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%02X",
                csgList->csgIdListInfo[i].csgIdHomeNodeBName.homeNodeBName[j]);
        }

        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",");

        /* 打印CSG类型，以UCS-2 格式编码, 最大长度12字节 */
        csgTypeLen = AT_MIN(csgList->csgIdListInfo[i].csgType.csgTypeLen, TAF_MMA_MAX_CSG_TYPE_LEN);

        for (j = 0; j < csgTypeLen; j++) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%02X", csgList->csgIdListInfo[i].csgType.csgType[j]);
        }

        if (csgList->csgIdListInfo[i].raMode == TAF_PH_RA_GSM) { /* GSM */
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",0");
        } else if (csgList->csgIdListInfo[i].raMode == TAF_PH_RA_WCDMA) { /* W */
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",2");
        } else if (csgList->csgIdListInfo[i].raMode == TAF_PH_RA_LTE) { /* LTE */
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ",7");
        } else {
        }

        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", csgList->csgIdListInfo[i].signalValue1);
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", csgList->csgIdListInfo[i].signalValue2);
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + lengthTemp, ")");
    }

    *length = lengthTemp;
}


VOS_UINT32 AT_RcvMmaCsgListSearchCnfProc(struct MsgCB *msg)
{
    TAF_MMA_CsgListSearchCnf *csgListCnf = VOS_NULL_PTR;
    TAF_MMA_CsgListCnfPara   *csgList    = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum;
    TAF_MMA_PlmnListPara      csgListPara;
    AT_RreturnCodeUint32      result;

    length     = 0;
    csgListCnf = (TAF_MMA_CsgListSearchCnf *)msg;
    csgList    = &csgListCnf->csgListCnfPara;
    memset_s(&csgListPara, sizeof(csgListPara), 0x00, sizeof(csgListPara));

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(csgListCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCsgListSearchCnfProc : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCsgListSearchCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 容错处理, 如当前正处于CSG列表搜ABORT过程中则不上报列表搜结果 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSG_LIST_SEARCH) {
        AT_WARN_LOG("AT_RcvMmaCsgListSearchCnfProc, csg LIST CNF when Abort Plmn List or timeout. ");
        return VOS_ERR;
    }

    /* 如果是失败事件,直接上报ERROR */
    if (csgList->opError == VOS_TRUE) {
        result = (AT_RreturnCodeUint32)At_ChgTafErrorCode(indexNum, csgList->phoneError);

        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, result);

        return VOS_OK;
    }

    /* 首次查询上报结果时需要打印^CSGIDSRCH: */
    if (csgList->currIndex == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    }

    AT_ReportCsgListSearchCnfResult(csgList, &length);

    At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    length = 0;

    /* 如果本次上报的plmn数目与要求的相同，则认为C核仍有Plmn list没有上报，要继续发送请求进行查询 */
    if (csgList->plmnWithCsgIdNum == TAF_MMA_MAX_CSG_ID_LIST_NUM) {
        csgListPara.qryNum    = TAF_MMA_MAX_CSG_ID_LIST_NUM;
        csgListPara.currIndex = (VOS_UINT16)(csgList->currIndex + csgList->plmnWithCsgIdNum);

        if (TAF_MMA_CsgListSearchReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &csgListPara) == VOS_TRUE) {
            /* 设置当前操作类型 */
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSG_LIST_SEARCH;
            return VOS_OK;
        }

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\r\n");

    At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    length = 0;

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCsgListAbortCnf(struct MsgCB *msg)
{
    TAF_MMA_CsgListAbortCnf *plmnListAbortCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    plmnListAbortCnf = (TAF_MMA_CsgListAbortCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnListAbortCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCsgListAbortCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCsgListAbortCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    /*
     * 容错处理, 当前不在CSG列表搜ABORT过程中则不上报ABORT.
     * 如AT的ABORT保护定时器已超时, 之后再收到MMA的ABORT_CNF则不上报ABORT
     */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ABORT_CSG_LIST_SEARCH) {
        AT_WARN_LOG("AT_RcvMmaCsgListAbortCnf : Current Option is not correct.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ABORT;

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCsgSpecSearchCnfProc(struct MsgCB *msg)
{
    TAF_MMA_CsgSpecSearchCnf *plmnSpecialSelCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;

    plmnSpecialSelCnf = (TAF_MMA_CsgSpecSearchCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnSpecialSelCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCsgSpecSearchCnfProc : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCsgSpecSearchCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSG_SPEC_SEARCH) {
        AT_WARN_LOG("AT_RcvMmaCsgSpecSearchCnfProc : Current Option is not correct.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (plmnSpecialSelCnf->errorCause == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, plmnSpecialSelCnf->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaQryCampCsgIdInfoCnfProc(struct MsgCB *msg)
{
    VOS_UINT16                   length    = 0;
    TAF_MMA_QryCampCsgIdInfoCnf *qryCnfMsg = VOS_NULL_PTR;
    VOS_UINT8                    indexNum;

    qryCnfMsg = (TAF_MMA_QryCampCsgIdInfoCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryCnfMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaQryCampCsgIdInfoCnfProc : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaQryCampCsgIdInfoCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSG_ID_INFO_QRY) {
        AT_WARN_LOG("AT_RcvMmaQryCampCsgIdInfoCnfProc: WARNING:Not AT_CMD_CSG_ID_INFO_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /*
     * <CR><LF>^CSGIDSRCH: [<oper>[,<CSG ID>][,<rat>]]<CR><LF>
     * <CR>OK<LF>
     */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 如果PLMN ID非法或接入技术非法或CSG ID无效，则接下来只显示OK */
    if ((qryCnfMsg->plmnId.mcc == TAF_MMA_INVALID_MCC) || (qryCnfMsg->plmnId.mnc == TAF_MMA_INVALID_MNC) ||
        (qryCnfMsg->ratType >= TAF_MMA_RAT_BUTT)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"\",\"\",");
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_OK);
        return VOS_OK;
    }

    /* BCD码的MCC、MNC，输出时需要转换成字符串 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%X%X%X", (0x0f00 & qryCnfMsg->plmnId.mcc) >> 8,
        (0x00f0 & qryCnfMsg->plmnId.mcc) >> 4, (0x000f & qryCnfMsg->plmnId.mcc));

    if (((0x0f00 & qryCnfMsg->plmnId.mnc) >> 8) != 0x0F) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%X", (0x0f00 & qryCnfMsg->plmnId.mnc) >> 8);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%X%X\"", (0x00f0 & qryCnfMsg->plmnId.mnc) >> 4,
        (0x000f & qryCnfMsg->plmnId.mnc));

    /* 输出CSG ID */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%X\"", qryCnfMsg->csgId);

    /* <rat> */
    if (qryCnfMsg->ratType == TAF_MMA_RAT_LTE) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",7");
    }
    if (qryCnfMsg->ratType == TAF_PH_RA_GSM) { /* GSM */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0");
    } else if (qryCnfMsg->ratType == TAF_PH_RA_WCDMA) { /* W */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",2");
    } else {
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}

#endif


LOCAL VOS_CHAR* AT_ConvertRatModeForQryParaPlmnList(TAF_PH_RA_MODE raMode)
{
    VOS_CHAR *pcCopsRat = VOS_NULL_PTR;

    switch (raMode) {
        case TAF_PH_RA_GSM:
            pcCopsRat = "0";
            break;

        case TAF_PH_RA_WCDMA:
            pcCopsRat = "2";
            break;

        case TAF_PH_RA_LTE:
            pcCopsRat = "7";
            break;
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case TAF_PH_RA_NR:
            pcCopsRat = "12";
            break;
#endif
        default:
            pcCopsRat = "";
    }

    return pcCopsRat;
}

VOS_UINT32 At_QryParaPlmnListProc(struct MsgCB *msg)
{
    TAF_MMA_PlmnListCnf     *plmnListCnf = VOS_NULL_PTR;
    TAF_MMA_PlmnListCnfPara *plmnList    = VOS_NULL_PTR;
    TAF_MMA_PlmnListPara     plmnListPara;
    AT_RreturnCodeUint32     result;
    VOS_CHAR                *pcRatMode = VOS_NULL_PTR;
    VOS_UINT16               length;
    VOS_UINT32               tmp;
    VOS_UINT8                indexNum;

    length      = 0;
    plmnListCnf = (TAF_MMA_PlmnListCnf *)msg;
    plmnList    = &plmnListCnf->plmnListCnfPara;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnListCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDetachCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_PhPlmnListProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 容错处理, 如当前正处于列表搜ABORT过程中则不上报列表搜结果 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_TEST) {
        AT_WARN_LOG("At_PhPlmnListProc, TAF_PH_EVT_PLMN_LIST_CNF when Abort Plmn List or timeout. ");
        return VOS_ERR;
    }

    /* 如果是失败事件,直接上报ERROR */
    if (plmnList->opError == 1) {
        result = (AT_RreturnCodeUint32)At_ChgTafErrorCode(indexNum, plmnList->phoneError);

        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, result);

        return VOS_OK;
    }
    if (plmnList->currIndex == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    }

    plmnList->plmnNum = AT_MIN(plmnList->plmnNum, TAF_MMA_MAX_PLMN_NAME_LIST_NUM);
    for (tmp = 0; tmp < plmnList->plmnNum; tmp++) {
        if ((tmp != 0) || (plmnList->currIndex != 0)) { /* 除第一项外，其它以前要加逗号 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",");
        }

        plmnList->plmnName[tmp].operatorNameLong[TAF_PH_OPER_NAME_LONG - 1]   = '\0';
        plmnList->plmnName[tmp].operatorNameShort[TAF_PH_OPER_NAME_SHORT - 1] = '\0';
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "(%d,\"%s\",\"%s\"", plmnList->plmnInfo[tmp].plmnStatus,
            plmnList->plmnName[tmp].operatorNameLong, plmnList->plmnName[tmp].operatorNameShort);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%X%X%X", (0x0f00 & plmnList->plmnName[tmp].plmnId.mcc) >> 8,
            (0x00f0 & plmnList->plmnName[tmp].plmnId.mcc) >> 4, (0x000f & plmnList->plmnName[tmp].plmnId.mcc));

        if (((0x0f00 & plmnList->plmnName[tmp].plmnId.mnc) >> 8) != 0x0F) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X", (0x0f00 & plmnList->plmnName[tmp].plmnId.mnc) >> 8);
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X\"", (0x00f0 & plmnList->plmnName[tmp].plmnId.mnc) >> 4,
            (0x000f & plmnList->plmnName[tmp].plmnId.mnc));

        pcRatMode = AT_ConvertRatModeForQryParaPlmnList(plmnList->plmnInfo[tmp].raMode);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%s)", pcRatMode);
    }

    At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    length = 0;

    /* 如果本次上报的plmn数目与要求的相同，则认为C核仍有Plmn list没有上报，要继续发送请求进行查询 */
    if (plmnList->plmnNum == TAF_MMA_MAX_PLMN_NAME_LIST_NUM) {
        plmnListPara.qryNum    = TAF_MMA_MAX_PLMN_NAME_LIST_NUM;
        plmnListPara.currIndex = (VOS_UINT16)(plmnList->currIndex + plmnList->plmnNum);

        if (Taf_PhonePlmnList(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &plmnListPara) == VOS_TRUE) {
            /* 设置当前操作类型 */
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_COPS_TEST;
            return VOS_OK;
        }

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",,(0,1,2,3,4),(0,1,2)\r\n");

    At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    length = 0;

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 At_PlmnDetectIndProc(struct MsgCB *msg)
{
    TAF_MMA_PlmnDetectInd     *plmnDetectCnf = VOS_NULL_PTR;
    TAF_MMA_PlmnDetectIndPara *plmnDetect    = VOS_NULL_PTR;
    VOS_UINT16                 length;
    VOS_UINT8                  indexNum = 0;

    length        = 0;
    plmnDetectCnf = (TAF_MMA_PlmnDetectInd *)msg;
    plmnDetect    = &plmnDetectCnf->plmnDetectIndPara;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnDetectCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_PlmnDetectIndProc : WARNING:AT INDEX NOT FOUND!");
        return VOS_TRUE;
    }

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:", g_atCrLf, g_atStringTab[AT_STRING_DETECTPLMN].text);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X%X", (0x0000000f & plmnDetect->plmnId.mcc),
        (0x00000f00 & plmnDetect->plmnId.mcc) >> 8, (0x000f0000 & plmnDetect->plmnId.mcc) >> 16);

    if (((0x000f0000 & (plmnDetect->plmnId.mnc)) >> 16) != 0x0F) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X", (0x000f0000 & plmnDetect->plmnId.mnc) >> 16);
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X", (0x0000000f & plmnDetect->plmnId.mnc),
        (0x00000f00 & plmnDetect->plmnId.mnc) >> 8);

    /* <rat> */
    if (plmnDetect->ratMode == TAF_MMA_RAT_LTE) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",7");
    }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    else if (plmnDetect->ratMode == TAF_MMA_RAT_NR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",12");
    }
#endif
    else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",");
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", plmnDetect->rsrp);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


TAF_VOID At_PhMsgProc(TAF_UINT8 *data, TAF_UINT16 len)
{
    At_PhEventProc(data, len);
}


TAF_UINT32 At_Unicode2UnicodePrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                   TAF_UINT16 srcLen)
{
    TAF_UINT16 len    = 0;
    TAF_UINT16 chkLen = 0;
    TAF_UINT8  high1  = 0;
    TAF_UINT8  high2  = 0;
    TAF_UINT8  low1   = 0;
    TAF_UINT8  low2   = 0;
    TAF_UINT8 *write  = dst;
    TAF_UINT8 *read   = src;

    if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + (AT_ASCII_AND_HEX_CONVERSION_FACTOR * srcLen)) >= maxLength) {
        AT_ERR_LOG("At_Unicode2UnicodePrint too long");
        return 0;
    }

    /* 扫完整个字串 */
    while (chkLen < srcLen) {
        /* 第一个字节 */
        high1 = 0x0F & (*read >> 4);
        high2 = 0x0F & *read;

        if (high1 <= 0x09) { /* 0-9 */
            *write++ = high1 + 0x30;
        } else if (high1 >= 0x0A) { /* A-F */
            *write++ = high1 + 0x37;
        } else {
        }

        if (high2 <= 0x09) { /* 0-9 */
            *write++ = high2 + 0x30;
        } else if (high2 >= 0x0A) { /* A-F */
            *write++ = high2 + 0x37;
        } else {
        }

        /* 下一个字符 */
        chkLen++;
        read++;

        /* 第二个字节 */
        low1 = 0x0F & (*read >> 4);
        low2 = 0x0F & *read;

        if (low1 <= 0x09) { /* 0-9 */
            *write++ = low1 + 0x30;
        } else if (low1 >= 0x0A) { /* A-F */
            *write++ = low1 + 0x37;
        } else {
        }

        if (low2 <= 0x09) { /* 0-9 */
            *write++ = low2 + 0x30;
        } else if (low2 >= 0x0A) { /* A-F */
            *write++ = low2 + 0x37;
        } else {
        }

        /* 下一个字符 */
        chkLen++;
        read++;

        len += 4; /* 记录长度，包含两个高位两个低位 */
    }

    return len;
}
/*
 * Description: Unicode到Unicode打印转换
 * History:
 *  1.Date: 2009-08-19
 *    Modification: Created function
 */
TAF_UINT32 At_HexString2AsciiNumPrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                      TAF_UINT16 srcLen)
{
    TAF_UINT16 len    = 0;
    TAF_UINT16 chkLen = 0;
    TAF_UINT8  high1  = 0;
    TAF_UINT8  high2  = 0;
    TAF_UINT8 *write  = dst;
    TAF_UINT8 *read   = src;

    if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + (AT_ASCII_AND_HEX_CONVERSION_FACTOR * srcLen)) >= maxLength) {
        AT_ERR_LOG("At_Unicode2UnicodePrint too long");
        return 0;
    }

    /* 扫完整个字串 */
    while (chkLen < srcLen) {
        /* 第一个字节 */
        high1 = 0x0F & (*read >> 4);
        high2 = 0x0F & *read;

        if (high1 <= 0x09) { /* 0-9 */
            *write++ = high1 + 0x30;
        } else if (high1 >= 0x0A) { /* A-F */
            *write++ = high1 + 0x37;
        } else {
        }

        if (high2 <= 0x09) { /* 0-9 */
            *write++ = high2 + 0x30;
        } else if (high2 >= 0x0A) { /* A-F */
            *write++ = high2 + 0x37;
        } else {
        }

        /* 下一个字符 */
        chkLen++;
        read++;
        len += 2; /* 记录长度,高位和低位长度为2 */
    }

    return len;
}


TAF_UINT32 At_Ascii2UnicodePrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
                                 TAF_UINT16 srcLen)
{
    TAF_UINT16 len    = 0;
    TAF_UINT16 chkLen = 0;
    TAF_UINT8 *write  = dst;
    TAF_UINT8 *read   = src;
    TAF_UINT8  high   = 0;
    TAF_UINT8  low    = 0;

    /*
     * 16进制转ascii所以乘2,同时在函数内部会通过 *write++ = '0';*write++ = '0';
     * 代码给每个字符前的1、2位加上00，所以相当于其长度有扩大了2倍，所以最后是乘以4
     */
    if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + (4 * srcLen)) >= maxLength) {
        AT_ERR_LOG("At_Ascii2UnicodePrint too long");
        return 0;
    }

    /* 扫完整个字串 */
    while (chkLen++ < srcLen) {
        *write++ = '0';
        *write++ = '0';
        high     = 0x0F & (*read >> 4);
        low      = 0x0F & *read;

        /* 2个'0',一个高位，一个低位，长度为4 */
        len += 4; /* 记录长度 */

        if (high <= 0x09) { /* 0-9 */
            *write++ = high + 0x30;
        } else if (high >= 0x0A) { /* A-F */
            *write++ = high + 0x37;
        } else {
        }

        if (low <= 0x09) { /* 0-9 */
            *write++ = low + 0x30;
        } else if (low >= 0x0A) { /* A-F */
            *write++ = low + 0x37;
        } else {
        }

        /* 下一个字符 */
        read++;
    }

    return len;
}

TAF_UINT16 At_PrintReportData(TAF_UINT32 maxLength, TAF_INT8 *headaddr, MN_MSG_MsgCodingUint8 msgCoding, TAF_UINT8 *dst,
                              TAF_UINT8 *src, TAF_UINT16 srcLen)
{
    TAF_UINT16 length = 0;
    TAF_UINT32 printStrLen;
    TAF_UINT32 maxMemLength;
    VOS_UINT32 ret;
    errno_t    memResult;
    VOS_UINT8  printUserData[MN_MSG_MAX_8_BIT_LEN];

    printStrLen = 0;

    /* DCS: UCS2 */
    if (msgCoding == MN_MSG_MSG_CODING_UCS2) {
        length += (TAF_UINT16)At_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length, src,
                                                      srcLen);
    }
    /* DCS: 8BIT */
    else if (msgCoding == MN_MSG_MSG_CODING_8_BIT) { /* DATA:8BIT */
        if (g_atCscsType == AT_CSCS_GSM_7Bit_CODE) {
            memset_s(printUserData, sizeof(printUserData), 0, MN_MSG_MAX_8_BIT_LEN);

            srcLen      = TAF_MIN(srcLen, MN_MSG_MAX_8_BIT_LEN);
            printStrLen = 0;
            ret         = TAF_STD_ConvertAsciiToDefAlpha(src, srcLen, printUserData, &printStrLen, srcLen);
            if (ret != MN_ERR_NO_ERROR) {
                AT_NORM_LOG("At_PrintReportData : TAF_STD_ConvertAsciiToDefAlpha fail. ");
                return 0;
            }

            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                          printUserData, (VOS_UINT16)printStrLen);
        } else {
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                          src, srcLen);
        }
    }
    /* DCS: 7BIT */
    else {
        if (g_atCscsType == AT_CSCS_UCS2_CODE) { /* +CSCS:UCS2 */
            length += (TAF_UINT16)At_Ascii2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                        src, srcLen);
        } else {
            if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + srcLen) >= maxLength) {
                AT_ERR_LOG("At_PrintReportData too long");
                return 0;
            }

            if ((g_atCscsType == AT_CSCS_IRA_CODE) && (msgCoding == MN_MSG_MSG_CODING_7_BIT)) {
                TAF_STD_ConvertDefAlphaToAscii(src, srcLen, (dst + length), &printStrLen);
                length += (TAF_UINT16)printStrLen;
            } else {
                maxMemLength = maxLength - (TAF_UINT32)(dst - (TAF_UINT8 *)headaddr);
                if (srcLen > 0) {
                    memResult = memcpy_s((dst + length), maxMemLength, src, srcLen);
                    TAF_MEM_CHK_RTN_VAL(memResult, maxMemLength, srcLen);
                }
                length += srcLen;
            }
        }
    }

    return length;
}


TAF_UINT32 At_MsgPduInd(MN_MSG_BcdAddr *scAddr, MN_MSG_RawTsData *pdu, TAF_UINT8 *dst)
{
    TAF_UINT16 length = 0;

    /* <alpha> 不报 */

    /* <length> */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(dst + length), ",%d", pdu->len);

    /* <data> 有可能得到是UCS2，需仔细处理 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)dst + length, "%s", g_atCrLf);

    /* SCA */
    if (scAddr->bcdLen == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(dst + length), "00");
    } else {
        /* 字符串以/0结尾多一位，所以bcdLen长度+1 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(dst + length), "%X%X%X%X", (((scAddr->bcdLen + 1) & 0xf0) >> 4),
            ((scAddr->bcdLen + 1) & 0x0f), ((scAddr->addrType & 0xf0) >> 4), (scAddr->addrType & 0x0f));

        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                      scAddr->bcdNum, (TAF_UINT16)scAddr->bcdLen);
    }

    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                  pdu->data, (TAF_UINT16)pdu->len);

    return length;
}


VOS_UINT32 At_StaRptPduInd(MN_MSG_BcdAddr *scAddr, MN_MSG_RawTsData *pdu, VOS_UINT8 *dst)
{
    VOS_UINT16 length = 0;

    /* <length> */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(dst + length), "%d", pdu->len);

    /* <data> 有可能得到是UCS2，需仔细处理 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%s", g_atCrLf);

    /* SCA */
    if (scAddr->bcdLen == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(dst + length), "00");
    } else {
        /* 字符串以/0结尾多一位，所以bcdLen长度+1 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(dst + length), "%X%X%X%X", (((scAddr->bcdLen + 1) & 0xf0) >> 4),
            ((scAddr->bcdLen + 1) & 0x0f), ((scAddr->addrType & 0xf0) >> 4), (scAddr->addrType & 0x0f));

        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                      scAddr->bcdNum, (TAF_UINT16)scAddr->bcdLen);
    }

    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress, dst + length,
                                                  pdu->data, (TAF_UINT16)pdu->len);

    return length;
}


VOS_UINT32 AT_IsClientBlock(VOS_VOID)
{
    VOS_UINT32          atStatus;
    VOS_UINT32          atMode;
    AT_PortBuffCfgUint8 smsBuffCfg;

    smsBuffCfg = AT_GetPortBuffCfg();
    if (smsBuffCfg == AT_PORT_BUFF_DISABLE) {
        return VOS_FALSE;
    }

    atStatus = AT_IsAnyParseClientPend();
    atMode   = AT_IsAllClientDataMode();

    /* 若当前有一个通道处于 pend状态，则需要缓存短信 */
    if (atStatus == VOS_TRUE) {
        return VOS_TRUE;
    }

    /* 若当前所有通道都处于data模式，则缓存短信 */
    if (atMode == VOS_TRUE) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


TAF_VOID At_BufferMsgInTa(VOS_UINT8 indexNum, MN_MSG_EventUint32 event, MN_MSG_EventInfo *eventOutPara)
{
    MN_MSG_EventInfo *eventInfo = VOS_NULL_PTR;
    TAF_UINT8        *used      = VOS_NULL_PTR;
    AT_ModemSmsCtx   *smsCtx    = VOS_NULL_PTR;
    errno_t           memResult;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    smsCtx->smtBuffer.index = smsCtx->smtBuffer.index % AT_BUFFER_SMT_EVENT_MAX;
    used                    = &(smsCtx->smtBuffer.used[smsCtx->smtBuffer.index]);
    eventInfo               = &(smsCtx->smtBuffer.event[smsCtx->smtBuffer.index]);
    smsCtx->smtBuffer.index++;

    if (*used == AT_MSG_BUFFER_FREE) {
        *used = AT_MSG_BUFFER_USED;
    }

    memResult = memcpy_s(eventInfo, sizeof(MN_MSG_EventInfo), eventOutPara, sizeof(MN_MSG_EventInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(MN_MSG_EventInfo), sizeof(MN_MSG_EventInfo));
    return;
}


VOS_UINT16 AT_PrintSmsLength(MN_MSG_MsgCodingUint8 msgCoding, VOS_UINT32 length, VOS_UINT8 *dst)
{
    VOS_UINT16 lengthTemp;
    VOS_UINT16 smContentLength;

    /* UCS2编码显示字节长度应该是UCS2字符个数，不是BYTE数，所以需要字节数除以2 */
    if (msgCoding == MN_MSG_MSG_CODING_UCS2) {
        smContentLength = (VOS_UINT16)length >> 1;
    } else {
        smContentLength = (VOS_UINT16)length;
    }

    lengthTemp = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)dst,
        ",%d", smContentLength);

    return lengthTemp;
}


TAF_VOID At_ForwardMsgToTeInCmt(VOS_UINT8 indexNum, TAF_UINT16 *sendLength, MN_MSG_TsDataInfo *tsDataInfo,
                                MN_MSG_EventInfo *event)
{
    TAF_UINT16      length = *sendLength;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* +CMT */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "+CMT: ");

    if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) {
        /* +CMT: <oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data> */
        /* <oa> */
        length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.deliver.origAddr, (g_atSndCodeAddress + length));
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

        /* <alpha> 不报 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

        /* <scts> */
        length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.deliver.timeStamp, (g_atSndCodeAddress + length));
        if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");
            /* <tooa> */
            length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.deliver.origAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <fo> */
            length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));

            /* <pid> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.deliver.pid);

            /* <dcs> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.deliver.dcs.rawDcsData);

            /* <sca> */
            length += (TAF_UINT16)At_PrintBcdAddr(&event->u.deliverInfo.rcvMsgInfo.scAddr,
                                                  (g_atSndCodeAddress + length));

            /* <tosca> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", event->u.deliverInfo.rcvMsgInfo.scAddr.addrType);

            /* <length> */
            length += AT_PrintSmsLength(tsDataInfo->u.deliver.dcs.msgCoding, tsDataInfo->u.deliver.userData.len,
                                        (g_atSndCodeAddress + length));
        }

        /* <data> 有可能得到是UCS2，需仔细处理 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

        length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                 tsDataInfo->u.deliver.dcs.msgCoding, (g_atSndCodeAddress + length),
                                                 tsDataInfo->u.deliver.userData.orgData,
                                                 (TAF_UINT16)tsDataInfo->u.deliver.userData.len);
    } else {
        /* +CMT: [<alpha>],<length><CR><LF><pdu> */
        length += (TAF_UINT16)At_MsgPduInd(&event->u.deliverInfo.rcvMsgInfo.scAddr,
                                           &event->u.deliverInfo.rcvMsgInfo.tsRawData, (g_atSndCodeAddress + length));
    }
    *sendLength = length;
}

#if (FEATURE_BASTET == FEATURE_ON)

TAF_VOID At_ForwardMsgToTeInBst(TAF_UINT8 indexNum, TAF_UINT16 *sendLength, MN_MSG_TsDataInfo *tsDataInfo,
                                MN_MSG_EventInfo *event)
{
    TAF_UINT16        length = *sendLength;
    MN_MSG_BcdAddr   *scAddr = VOS_NULL_PTR;
    MN_MSG_RawTsData *pdu    = VOS_NULL_PTR;
    TAF_UINT8         blocklistFlag;

    scAddr = &event->u.deliverInfo.rcvMsgInfo.scAddr;
    pdu    = &event->u.deliverInfo.rcvMsgInfo.tsRawData;

    /* ^BST方式上报短信内容 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "^BST: ");

    /* <length> */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", pdu->len);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* Block名单标识字段,在短信PDU的首部增加一个字节，写Block名单短信标识信息，默认值255 */
    blocklistFlag = 0xFF;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), "%X%X", ((blocklistFlag & 0xf0) >> 4), (blocklistFlag & 0x0f));

    /* SCA 短信中心地址 */
    if (scAddr->bcdLen == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "00");
    } else {
        /* 字符串以/0结尾多一位，所以bcdLen长度+1 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%X%X%X%X", (((scAddr->bcdLen + 1) & 0xf0) >> 4),
            ((scAddr->bcdLen + 1) & 0x0f), ((scAddr->addrType & 0xf0) >> 4), (scAddr->addrType & 0x0f));

        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      g_atSndCodeAddress + length, scAddr->bcdNum,
                                                      (TAF_UINT16)scAddr->bcdLen);
    }

    /* 短信内容 */
    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  g_atSndCodeAddress + length, pdu->data, (TAF_UINT16)pdu->len);

    *sendLength = length;
}


TAF_VOID AT_BlockSmsReport(TAF_UINT8 indexNum, MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo)
{
    TAF_UINT16 length;

    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s", g_atCrLf);

    /* bst的方式上报 */
    At_ForwardMsgToTeInBst(indexNum, &length, tsDataInfo, event);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}
#endif


TAF_VOID AT_ForwardDeliverMsgToTe(MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo)
{
    TAF_BOOL        bCmtiInd = VOS_FALSE;
    TAF_UINT16      length;
    VOS_UINT8       indexNum;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ForwardDeliverMsgToTe: WARNING:AT INDEX NOT FOUND!");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    AT_LOG1("AT_ForwardDeliverMsgToTe: current mt is", smsCtx->cnmiType.cnmiMtType);

    if (smsCtx->cnmiType.cnmiMtType == AT_CNMI_MT_NO_SEND_TYPE) {
        return;
    }

    if ((event->u.deliverInfo.rcvSmAct == MN_MSG_RCVMSG_ACT_STORE) &&
        (event->u.deliverInfo.memStore != MN_MSG_MEM_STORE_NONE)) {
        if ((smsCtx->cnmiType.cnmiMtType == AT_CNMI_MT_CMTI_TYPE) ||
            (smsCtx->cnmiType.cnmiMtType == AT_CNMI_MT_CLASS3_TYPE)) {
            bCmtiInd = TAF_TRUE;
        }

        if (tsDataInfo->u.deliver.dcs.msgClass == MN_MSG_MSG_CLASS_2) {
            bCmtiInd = TAF_TRUE;
        }
    }

#if (FEATURE_BASTET == FEATURE_ON)
    if (event->u.deliverInfo.ucBlockRptFlag == VOS_TRUE) {
        AT_BlockSmsReport(indexNum, event, tsDataInfo);
        return;
    }
#endif

    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s", g_atCrLf);

    /*
     * 根据MT设置和接收到事件的CLASS类型得到最终的事件上报格式:
     * 协议要求MT为3时CLASS类型获取实际MT类型, 该情况下终端不上报事件与协议不一致
     */
    if (bCmtiInd == TAF_TRUE) {
        /* +CMTI: <mem>,<index> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "+CMTI: %s,%d",
            At_GetStrContent(At_GetSmsArea(event->u.deliverInfo.memStore)), event->u.deliverInfo.inex);
    } else {
        /* CMT的方式上报 */
        At_ForwardMsgToTeInCmt(indexNum, &length, tsDataInfo, event);
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


TAF_VOID AT_ForwardStatusReportMsgToTe(MN_MSG_EventInfo *event, MN_MSG_TsDataInfo *tsDataInfo)
{
    TAF_UINT16      length;
    VOS_UINT8       indexNum;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ForwardStatusReportMsgToTe: WARNING:AT INDEX NOT FOUND!");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    AT_LOG1("AT_ForwardStatusReportMsgToTe: current ds is ", smsCtx->cnmiType.cnmiDsType);

    if (smsCtx->cnmiType.cnmiDsType == AT_CNMI_DS_NO_SEND_TYPE) {
        return;
    }

    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s", g_atCrLf);

    if ((event->u.deliverInfo.rcvSmAct == MN_MSG_RCVMSG_ACT_STORE) &&
        (event->u.deliverInfo.memStore != MN_MSG_MEM_STORE_NONE)) {
        /* +CDSI: <mem>,<index> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "+CDSI: %s,%d",
            At_GetStrContent(At_GetSmsArea(event->u.deliverInfo.memStore)), event->u.deliverInfo.inex);
    } else {
        /* +CDS */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "+CDS: ");
        if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) {
            /* +CDS: <fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st> */
            /* <fo> */
            length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));

            /* <mr> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.staRpt.mr);

            /* <ra> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.staRpt.recipientAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <tora> */
            length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.staRpt.recipientAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <scts> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.timeStamp, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <dt> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.dischargeTime, (g_atSndCodeAddress + length));

            /* <st> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.staRpt.status);
        } else {
            /* +CDS: <length><CR><LF><pdu> */
            length += (VOS_UINT16)At_StaRptPduInd(&event->u.deliverInfo.rcvMsgInfo.scAddr,
                                                  &event->u.deliverInfo.rcvMsgInfo.tsRawData,
                                                  (g_atSndCodeAddress + length));
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


TAF_VOID AT_ForwardPppMsgToTe(MN_MSG_EventInfo *event)
{
    TAF_UINT32         ret;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;

    tsDataInfo = At_GetMsgMem();

    ret = MN_MSG_Decode(&event->u.deliverInfo.rcvMsgInfo.tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        return;
    }

    if (tsDataInfo->tpduType == MN_MSG_TPDU_DELIVER) {
        AT_ForwardDeliverMsgToTe(event, tsDataInfo);
    } else if (tsDataInfo->tpduType == MN_MSG_TPDU_STARPT) {
        AT_ForwardStatusReportMsgToTe(event, tsDataInfo);
    } else {
        AT_WARN_LOG("AT_ForwardPppMsgToTe: invalid tpdu type.");
    }

    return;
}

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))

VOS_VOID AT_ForwardCbMsgToTe(MN_MSG_EventInfo *event)
{
    TAF_UINT32      ret;
    MN_MSG_Cbpage   cbmPageInfo;
    VOS_UINT16      length;
    VOS_UINT8       indexNum;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ForwardCbMsgToTe: WARNING:AT INDEX NOT FOUND!");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    AT_LOG1("AT_ForwardCbMsgToTe: current bm is ", smsCtx->cnmiType.cnmiBmType);

    /*
     * 根据BM设置和接收到事件的CLASS类型得到最终的事件上报格式:
     * 不支持协议要求BM为3时,CBM的上报
     */

    ret = MN_MSG_DecodeCbmPage(&(event->u.cbsDeliverInfo.cbRawData), &cbmPageInfo);
    if (ret != MN_ERR_NO_ERROR) {
        return;
    }

    event->u.cbsDeliverInfo.geoAreaLen = AT_MIN(event->u.cbsDeliverInfo.geoAreaLen, TAF_CBS_CMAS_GEO_AREA_MAX_LEN);

    if (event->u.cbsDeliverInfo.geoInfoRptFlag == VOS_TRUE) {
        /* ^CBMEXT: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data><CR><LF><geoLen>,<geoPdu> (text mode enabled) */
        /* ^CBMEXT: <length><CR><LF><pdu><CR><LF><geoLen><CR><LF><geoPdu> (PDU mode enabled); or */
        length = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "^CBMEXT: ");
    } else {
        /* +CBM: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data> (text mode enabled) */
        /* +CBM: <length><CR><LF><pdu> (PDU mode enabled); or */
        length = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "+CBM: ");
    }

    if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d,", cbmPageInfo.sn.rawSnData);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d,", cbmPageInfo.mid);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d,", cbmPageInfo.dcs.rawDcsData);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d,", cbmPageInfo.pageIndex);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d", cbmPageInfo.pageNum);

        /* <data> 有可能得到是UCS2，需仔细处理 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

        cbmPageInfo.content.len = (VOS_UINT32)TAF_MIN(cbmPageInfo.content.len, TAF_CBA_MAX_RAW_CBDATA_LEN);

        length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
            cbmPageInfo.dcs.msgCoding, (g_atSndCodeAddress + length),
            cbmPageInfo.content.content, (TAF_UINT16)cbmPageInfo.content.len);

        if (event->u.cbsDeliverInfo.geoInfoRptFlag == VOS_TRUE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%d", event->u.cbsDeliverInfo.geoAreaLen);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                (g_atSndCodeAddress + length), event->u.cbsDeliverInfo.geoAreaStr,
                (TAF_UINT16)event->u.cbsDeliverInfo.geoAreaLen);
        }

    } else {
        /* +CBM: <length><CR><LF><pdu> (PDU mode enabled); or */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%d", event->u.cbsDeliverInfo.cbRawData.len);

        /* <data> 有可能得到是UCS2，需仔细处理 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
            (g_atSndCodeAddress + length), event->u.cbsDeliverInfo.cbRawData.data,
            (TAF_UINT16)event->u.cbsDeliverInfo.cbRawData.len);

       if (event->u.cbsDeliverInfo.geoInfoRptFlag == VOS_TRUE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%d", event->u.cbsDeliverInfo.geoAreaLen);

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                (g_atSndCodeAddress + length), event->u.cbsDeliverInfo.geoAreaStr,
                (TAF_UINT16)event->u.cbsDeliverInfo.geoAreaLen);
        }

    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* 此处还需多一个空行 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}

#endif

TAF_VOID At_ForwardMsgToTe(MN_MSG_EventUint32 event, MN_MSG_EventInfo *eventOutPara)
{
    AT_LOG1("At_ForwardMsgToTe: current Event is ", event);

    switch (event) {
        case MN_MSG_EVT_DELIVER:
            AT_ForwardPppMsgToTe(eventOutPara);
            break;

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
        case MN_MSG_EVT_DELIVER_CBM:
            AT_ForwardCbMsgToTe(eventOutPara);
            break;
#endif
        default:
            AT_WARN_LOG("At_SendSmtInd: invalid tpdu type.");
            break;
    }
    return;
}


TAF_VOID At_HandleSmtBuffer(VOS_UINT8 indexNum, AT_CNMI_BFR_TYPE bfrType)
{
    TAF_UINT32      loop;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (bfrType == AT_CNMI_BFR_SEND_TYPE) {
        for (loop = 0; loop < AT_BUFFER_SMT_EVENT_MAX; loop++) {
            if (smsCtx->smtBuffer.used[loop] == AT_MSG_BUFFER_USED) {
                At_ForwardMsgToTe(MN_MSG_EVT_DELIVER, &(smsCtx->smtBuffer.event[loop]));
            }
        }
    }

    (VOS_VOID)memset_s(&(smsCtx->smtBuffer), sizeof(smsCtx->smtBuffer), 0x00, sizeof(smsCtx->smtBuffer));

    return;
}


VOS_VOID AT_FlushSmsIndication(VOS_VOID)
{
    AT_ModemSmsCtx     *smsCtx      = VOS_NULL_PTR;
    AT_PortBuffCfgInfo *portBuffCfg = VOS_NULL_PTR;
    VOS_UINT16          i;
    VOS_UINT32          clientId;
    ModemIdUint16       modemId;
    VOS_UINT32          rslt;
    VOS_UINT8           modemFlag[MODEM_ID_BUTT];
    VOS_UINT32          j;

    j = 0;

    portBuffCfg = AT_GetPortBuffCfgInfo();
    memset_s(modemFlag, sizeof(modemFlag), 0x00, sizeof(modemFlag));

    if (portBuffCfg->num > AT_MAX_CLIENT_NUM) {
        portBuffCfg->num = AT_MAX_CLIENT_NUM;
    }

    /* 根据clientId查找需要flush 短信modem id */
    for (i = 0; i < portBuffCfg->num; i++) {
        clientId = portBuffCfg->usedClientId[i];
        rslt     = AT_GetModemIdFromClient((VOS_UINT8)clientId, &modemId);
        if (rslt != VOS_OK) {
            AT_ERR_LOG("AT_FlushSmsIndication: Get modem id fail");
            continue;
        }

        modemFlag[modemId] = VOS_TRUE;
    }

    /* flush SMS */
    for (i = 0; i < MODEM_ID_BUTT; i++) {
        if (modemFlag[i] == VOS_TRUE) {
            smsCtx = AT_GetModemSmsCtxAddrFromClientId(i);

            for (j = 0; j < AT_BUFFER_SMT_EVENT_MAX; j++) {
                if (smsCtx->smtBuffer.used[j] == AT_MSG_BUFFER_USED) {
                    At_ForwardMsgToTe(MN_MSG_EVT_DELIVER, &(smsCtx->smtBuffer.event[j]));
                }
            }

            memset_s(&(smsCtx->smtBuffer), sizeof(smsCtx->smtBuffer), 0x00, sizeof(smsCtx->smtBuffer));
        }
    }

    return;
}


TAF_VOID At_SmsModSmStatusRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    VOS_UINT32 ret;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SmsModSmStatusRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    if (event->u.modifyInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.deleteInfo.failCause);
    } else {
        ret = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, ret);
    return;
}


TAF_VOID At_SmsInitResultProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    smsCtx->cpmsInfo.usimStorage.totalRec = event->u.initResultInfo.totalSmRec;
    smsCtx->cpmsInfo.usimStorage.usedRec  = event->u.initResultInfo.usedSmRec;

    return;
}


VOS_VOID At_SmsDeliverErrProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if ((smsCtx->localStoreFlg == VOS_TRUE) && (event->u.deliverErrInfo.errorCode == TAF_MSG_ERROR_TR2M_TIMEOUT)) {
        smsCtx->cnmiType.cnmiMtType = AT_CNMI_MT_NO_SEND_TYPE;
        smsCtx->cnmiType.cnmiDsType = AT_CNMI_DS_NO_SEND_TYPE;
        AT_WARN_LOG("At_SmsDeliverErrProc: CnmiMtType and CnmiDsType changed!");
    }

    /* 短信接收流程因为写操作失败不会上报事件给AT，且该事件有ERROR LOG记录不需要上报给应用处理 */

    return;
}


VOS_VOID At_SmsInitSmspResultProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;
    errno_t         memResult;

    VOS_UINT8 defaultIndex;

    defaultIndex = event->u.initSmspResultInfo.defaultSmspIndex;

    AT_NORM_LOG1("At_SmsInitSmspResultProc: ucDefaultIndex", defaultIndex);

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (defaultIndex >= MN_MSG_MAX_USIM_EFSMSP_NUM) {
        defaultIndex = AT_CSCA_CSMP_STORAGE_INDEX;
    }

    /* 记录defaultSmspIndex, 在csca csmp 中使用 */
    smsCtx->cscaCsmpInfo.defaultSmspIndex = defaultIndex;

    memResult = memcpy_s(&(smsCtx->cscaCsmpInfo.parmInUsim), sizeof(smsCtx->cscaCsmpInfo.parmInUsim),
                         &event->u.initSmspResultInfo.srvParm[defaultIndex], sizeof(MN_MSG_SrvParam));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cscaCsmpInfo.parmInUsim), sizeof(MN_MSG_SrvParam));

    memResult = memcpy_s(&(smsCtx->cpmsInfo.rcvPath), sizeof(smsCtx->cpmsInfo.rcvPath),
                         &event->u.initSmspResultInfo.rcvMsgPath, sizeof(event->u.initSmspResultInfo.rcvMsgPath));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cpmsInfo.rcvPath), sizeof(event->u.initSmspResultInfo.rcvMsgPath));

    g_class0Tailor = event->u.initSmspResultInfo.class0Tailor;

    return;
}


VOS_VOID At_SmsSrvParmChangeProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;
    errno_t         memResult;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    memResult = memcpy_s(&(smsCtx->cscaCsmpInfo.parmInUsim), sizeof(smsCtx->cscaCsmpInfo.parmInUsim),
                         &event->u.srvParmChangeInfo.srvParm[smsCtx->cscaCsmpInfo.defaultSmspIndex],
                         sizeof(MN_MSG_SrvParam));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cscaCsmpInfo.parmInUsim), sizeof(MN_MSG_SrvParam));

    return;
}


VOS_VOID At_SmsRcvMsgPathChangeProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    smsCtx->cpmsInfo.rcvPath.rcvSmAct       = event->u.rcvMsgPathInfo.rcvSmAct;
    smsCtx->cpmsInfo.rcvPath.smMemStore     = event->u.rcvMsgPathInfo.smMemStore;
    smsCtx->cpmsInfo.rcvPath.rcvStaRptAct   = event->u.rcvMsgPathInfo.rcvStaRptAct;
    smsCtx->cpmsInfo.rcvPath.staRptMemStore = event->u.rcvMsgPathInfo.staRptMemStore;

    return;
}


VOS_VOID AT_ReportSmMeFull(VOS_UINT8 indexNum, MN_MSG_MemStoreUint8 memStore)
{
    VOS_UINT16 length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), "^SMMEMFULL: ");

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", At_GetStrContent(At_GetSmsArea(memStore)));
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}

VOS_VOID At_SmsStorageListProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    MN_MSG_StorageListEvtInfo *storageListInfo = VOS_NULL_PTR;
    AT_ModemSmsCtx            *smsCtx          = VOS_NULL_PTR;
    errno_t                    memResult;

    MN_MSG_MtCustomizeUint8 mtCustomize;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    storageListInfo = &event->u.storageListInfo;
    if (storageListInfo->memStroe == MN_MSG_MEM_STORE_SIM) {
        memResult = memcpy_s(&(smsCtx->cpmsInfo.usimStorage), sizeof(smsCtx->cpmsInfo.usimStorage),
                             &event->u.storageListInfo, sizeof(event->u.storageListInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cpmsInfo.usimStorage), sizeof(event->u.storageListInfo));
    } else {
        memResult = memcpy_s(&(smsCtx->cpmsInfo.nvimStorage), sizeof(smsCtx->cpmsInfo.nvimStorage),
                             &event->u.storageListInfo, sizeof(event->u.storageListInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cpmsInfo.nvimStorage), sizeof(event->u.storageListInfo));
    }

    mtCustomize = smsCtx->smMeFullCustomize.mtCustomize;
    if ((mtCustomize == MN_MSG_MT_CUSTOMIZE_FT) &&
        (event->u.storageListInfo.totalRec == event->u.storageListInfo.usedRec) &&
        (event->u.storageStateInfo.memStroe == MN_MSG_MEM_STORE_SIM)) {
        AT_INFO_LOG("At_SmsStorageListProc: FT memory full.");
        AT_ReportSmMeFull(indexNum, event->u.storageStateInfo.memStroe);
    }

    if (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        /* 接收到NV的短信容量上报，修改NV的短信容量等待标志已接收到NV的短信容量 */
#if (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON)
        if (storageListInfo->memStroe == MN_MSG_MEM_STORE_ME) {
            g_atClientTab[indexNum].atSmsData.waitForNvStorageStatus = TAF_FALSE;
        }
#endif
        /* 接收到SIM的短信容量上报，修改SIM的短信容量等待标志已接收到SIM的短信容量 */
        if (storageListInfo->memStroe == MN_MSG_MEM_STORE_SIM) {
            g_atClientTab[indexNum].atSmsData.waitForUsimStorageStatus = TAF_FALSE;
        }

        /* CPMS的设置操作需要等待所有容量信息和设置响应消息后完成 */
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPMS_SET) {
            if ((g_atClientTab[indexNum].atSmsData.waitForCpmsSetRsp == TAF_FALSE) &&
                (g_atClientTab[indexNum].atSmsData.waitForNvStorageStatus == TAF_FALSE) &&
                (g_atClientTab[indexNum].atSmsData.waitForUsimStorageStatus == TAF_FALSE)) {
                AT_STOP_TIMER_CMD_READY(indexNum);
                At_PrintSetCpmsRsp(indexNum);
            }
        }

        /* CPMS的读取操作需要等待所有容量信息后完成 */
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPMS_READ) {
            if ((g_atClientTab[indexNum].atSmsData.waitForNvStorageStatus == TAF_FALSE) &&
                (g_atClientTab[indexNum].atSmsData.waitForUsimStorageStatus == TAF_FALSE)) {
                AT_STOP_TIMER_CMD_READY(indexNum);
                At_PrintGetCpmsRsp(indexNum);
            }
        }
    }

    return;
}


VOS_VOID At_SmsStorageExceedProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    /* 初始化 */
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_SmsStorageExceedProc: Get modem id fail.");
        return;
    }

    AT_ReportSmMeFull(indexNum, event->u.storageStateInfo.memStroe);

    return;
}


VOS_VOID At_SmsDeliverProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT16         length = 0;
    TAF_UINT32         ret;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;
    VOS_UINT8          userId;
    AT_ModemSmsCtx    *smsCtx = VOS_NULL_PTR;

    userId = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucUserId */
    if (At_ClientIdToUserId(event->clientId, &userId) == AT_FAILURE) {
        AT_WARN_LOG("AT_SmsDeliverProc: WARNING:AT INDEX NOT FOUND!");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(userId);

    /*
     * 当前短信类型为CLass0且短信定制为
     * 1:H3G与意大利TIM Class 0短信需求相同，不对短信进行存储，要求将CLASS 0
     * 短信直接采用+CMT进行主动上报。不受CNMI以及CPMS设置的影响，如果后台已经
     * 打开，则后台对CLASS 0短信进行显示。
     * CLass0的短信此时不考虑MT,MODE的参数
     */

    tsDataInfo = At_GetMsgMem();
    ret        = MN_MSG_Decode(&event->u.deliverInfo.rcvMsgInfo.tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        return;
    }

    AT_StubSaveAutoReplyData(userId, event, tsDataInfo);

#if (FEATURE_AT_HSUART == FEATURE_ON)
    AT_SmsStartRingTe(VOS_TRUE);
#endif

    if ((tsDataInfo->tpduType == MN_MSG_TPDU_DELIVER) && (tsDataInfo->u.deliver.dcs.msgClass == MN_MSG_MSG_CLASS_0) &&
        (g_class0Tailor != MN_MSG_CLASS0_DEF)) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        if ((g_class0Tailor == MN_MSG_CLASS0_TIM) || (g_class0Tailor == MN_MSG_CLASS0_VIVO)) {
            /* +CMT格式上报 */
            At_ForwardMsgToTeInCmt(userId, &length, tsDataInfo, event);
        } else {
            /* +CMTI: <mem>,<index> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "+CMTI: %s,%d",
                At_GetStrContent(At_GetSmsArea(event->u.deliverInfo.memStore)), event->u.deliverInfo.inex);
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_SendResultData(userId, g_atSndCodeAddress, length);

        return;
    }

    if (smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_SEND_OR_DISCARD_TYPE) {
        At_ForwardMsgToTe(MN_MSG_EVT_DELIVER, event);
        return;
    }

    /* 当模式为0时缓存 */
    if (smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_BUFFER_TYPE) {
        At_BufferMsgInTa(indexNum, MN_MSG_EVT_DELIVER, event);
        return;
    }

    /* 当模式为2时缓存 */
    if (smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_SEND_OR_BUFFER_TYPE) {
        /* 判断是否具备缓存的条件 */
        if (AT_IsClientBlock() == VOS_TRUE) {
            At_BufferMsgInTa(indexNum, MN_MSG_EVT_DELIVER, event);
        } else {
            At_ForwardMsgToTe(MN_MSG_EVT_DELIVER, event);
        }
        return;
    }

    /* 目前不支持 AT_CNMI_MODE_EMBED_AND_SEND_TYPE */

    return;
}


TAF_VOID At_SetRcvPathRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT32      ret;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetRcvPathRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (event->u.rcvMsgPathInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.srvParmInfo.failCause);
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, ret);
        return;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPMS_SET) {
        /* 保存临时数据到内存和NVIM */
        smsCtx->cpmsInfo.rcvPath.smMemStore     = event->u.rcvMsgPathInfo.smMemStore;
        smsCtx->cpmsInfo.rcvPath.staRptMemStore = event->u.rcvMsgPathInfo.staRptMemStore;
        smsCtx->cpmsInfo.memReadorDelete        = smsCtx->cpmsInfo.tmpMemReadorDelete;
        smsCtx->cpmsInfo.memSendorWrite         = smsCtx->cpmsInfo.tmpMemSendorWrite;

        g_atClientTab[indexNum].atSmsData.waitForCpmsSetRsp = TAF_FALSE;

        /* CPMS的设置操作需要等待所有容量信息和设置响应消息后完成 */
        if ((g_atClientTab[indexNum].atSmsData.waitForCpmsSetRsp == TAF_FALSE) &&
            (g_atClientTab[indexNum].atSmsData.waitForNvStorageStatus == TAF_FALSE) &&
            (g_atClientTab[indexNum].atSmsData.waitForUsimStorageStatus == TAF_FALSE)) {
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_PrintSetCpmsRsp(indexNum);
        }

    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CSMS_SET) {
        /* 执行命令操作 */
        smsCtx->csmsMsgVersion                  = event->u.rcvMsgPathInfo.smsServVersion;
        smsCtx->cpmsInfo.rcvPath.smsServVersion = smsCtx->csmsMsgVersion;

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress), "%s: ",
            g_parseContext[indexNum].cmdElement->cmdName);
        At_PrintCsmsInfo(indexNum);
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_OK);
    } else { /* AT_CMD_CNMI_SET */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);

        smsCtx->cnmiType.cnmiBfrType          = smsCtx->cnmiType.cnmiTmpBfrType;
        smsCtx->cnmiType.cnmiDsType           = smsCtx->cnmiType.cnmiTmpDsType;
        smsCtx->cnmiType.cnmiBmType           = smsCtx->cnmiType.cnmiTmpBmType;
        smsCtx->cnmiType.cnmiMtType           = smsCtx->cnmiType.cnmiTmpMtType;
        smsCtx->cnmiType.cnmiModeType         = smsCtx->cnmiType.cnmiTmpModeType;
        smsCtx->cpmsInfo.rcvPath.rcvSmAct     = event->u.rcvMsgPathInfo.rcvSmAct;
        smsCtx->cpmsInfo.rcvPath.rcvStaRptAct = event->u.rcvMsgPathInfo.rcvStaRptAct;

        if (smsCtx->cnmiType.cnmiModeType != 0) {
            At_HandleSmtBuffer(indexNum, smsCtx->cnmiType.cnmiBfrType);
        }
    }
    return;
}


TAF_VOID At_SetCscaCsmpRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    errno_t         memResult;
    TAF_UINT32      ret;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetCscaCsmpRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (event->u.srvParmInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.srvParmInfo.failCause);
    } else {
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CSMP_SET) {
            memResult = memcpy_s(&(smsCtx->cscaCsmpInfo.vp), sizeof(smsCtx->cscaCsmpInfo.vp),
                                 &(smsCtx->cscaCsmpInfo.tmpVp), sizeof(smsCtx->cscaCsmpInfo.tmpVp));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cscaCsmpInfo.vp), sizeof(smsCtx->cscaCsmpInfo.tmpVp));
            smsCtx->cscaCsmpInfo.fo     = smsCtx->cscaCsmpInfo.tmpFo;
            smsCtx->cscaCsmpInfo.foUsed = TAF_TRUE;
        }
        memResult = memcpy_s(&(smsCtx->cscaCsmpInfo.parmInUsim), sizeof(smsCtx->cscaCsmpInfo.parmInUsim),
                             &event->u.srvParmInfo.srvParm, sizeof(event->u.srvParmInfo.srvParm));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cscaCsmpInfo.parmInUsim),
                            sizeof(event->u.srvParmInfo.srvParm));
        ret = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);
    return;
}


TAF_VOID At_DeleteRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT32         ret;
    MN_MSG_DeleteParam deleteInfo;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_DeleteRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    memset_s(&deleteInfo, sizeof(deleteInfo), 0x00, sizeof(deleteInfo));

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGD_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBMGD_SET)) {
        return;
    }

    deleteInfo.memStore = event->u.deleteInfo.memStore;
    deleteInfo.index    = event->u.deleteInfo.index;
    if (event->u.deleteInfo.success != TAF_TRUE) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.deleteInfo.failCause);
        At_FormatResultData(indexNum, ret);
        return;
    }

    if (event->u.deleteInfo.deleteType == MN_MSG_DELETE_SINGLE) {
        g_atClientTab[indexNum].atSmsData.msgDeleteTypes ^= AT_MSG_DELETE_SINGLE;
    }

    if (event->u.deleteInfo.deleteType == MN_MSG_DELETE_ALL) {
        g_atClientTab[indexNum].atSmsData.msgDeleteTypes ^= AT_MSG_DELETE_ALL;
    }

    if (event->u.deleteInfo.deleteType == MN_MSG_DELETE_READ) {
        g_atClientTab[indexNum].atSmsData.msgDeleteTypes ^= AT_MSG_DELETE_READ;
    }

    if (event->u.deleteInfo.deleteType == MN_MSG_DELETE_SENT) {
        g_atClientTab[indexNum].atSmsData.msgDeleteTypes ^= AT_MSG_DELETE_SENT;
    }

    if (event->u.deleteInfo.deleteType == MN_MSG_DELETE_NOT_SENT) {
        g_atClientTab[indexNum].atSmsData.msgDeleteTypes ^= AT_MSG_DELETE_UNSENT;
    }

    if (g_atClientTab[indexNum].atSmsData.msgDeleteTypes == 0) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_MsgDeleteCmdProc(indexNum, g_atClientTab[indexNum].opId, deleteInfo,
                            g_atClientTab[indexNum].atSmsData.msgDeleteTypes);
    }
    return;
}


VOS_VOID AT_QryCscaRspProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    errno_t         memResult;
    VOS_UINT16      length;
    VOS_UINT32      ret;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_QryCscaRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* AT模块在等待CSCA查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSCA_READ) {
        return;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->u.srvParmInfo.success == VOS_TRUE) {
        /* 更新短信中心号码到AT模块，解决MSG模块初始化完成事件上报时AT模块未启动问题 */
        memResult = memcpy_s(&(smsCtx->cscaCsmpInfo.parmInUsim.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr),
                             &event->u.srvParmInfo.srvParm.scAddr, sizeof(event->u.srvParmInfo.srvParm.scAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr),
                            sizeof(event->u.srvParmInfo.srvParm.scAddr));

        /* 设置错误码为AT_OK           构造结构为+CSCA: <sca>,<toda>格式的短信 */
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* 短信中心号码存在指示为存在且短信中心号码长度不为0 */
        if (((event->u.srvParmInfo.srvParm.parmInd & MN_MSG_SRV_PARM_MASK_SC_ADDR) == 0) &&
            (event->u.srvParmInfo.srvParm.scAddr.bcdLen != 0)) {
            /* 将SCA地址由BCD码转换为ASCII码 */
            length += At_PrintBcdAddr(&event->u.srvParmInfo.srvParm.scAddr, (g_atSndCodeAddress + length));

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", event->u.srvParmInfo.srvParm.scAddr.addrType);
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",128");
        }

        g_atSendDataBuff.bufLen = length;
        ret                     = AT_OK;
    } else {
        /* 根据pstEvent->u.stSrvParmInfo.ulFailCause调用At_ChgMnErrCodeToAt转换出AT模块的错误码 */
        g_atSendDataBuff.bufLen = 0;
        ret                     = At_ChgMnErrCodeToAt(indexNum, event->u.srvParmInfo.failCause);
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_SmsStubRspProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    VOS_UINT32 ret;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SmsStubRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* AT模块在等待CMSTUB命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMSTUB_SET) {
        return;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->u.result.errorCode == MN_ERR_NO_ERROR) {
        ret = AT_OK;
    } else {
        ret = AT_CMS_UNKNOWN_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, ret);
    return;
}


VOS_UINT32 AT_GetBitMap(VOS_UINT32 *bitMap, VOS_UINT32 indexNum)
{
    VOS_UINT8  x;
    VOS_UINT32 y;
    VOS_UINT32 mask;

    y    = indexNum / AT_FOUR_BYTES_TO_BITS_LENS;
    x    = (VOS_UINT8)indexNum % AT_FOUR_BYTES_TO_BITS_LENS;
    mask = ((VOS_UINT32)1 << x);
    if ((bitMap[y] & mask) != 0) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}


VOS_VOID AT_SmsListIndex(VOS_UINT16 length, MN_MSG_DeleteTestEvtInfo *para, VOS_UINT16 *printOffSet)
{
    TAF_UINT32 loop;
    TAF_UINT32 msgNum;

    msgNum = 0;

    for (loop = 0; loop < para->smCapacity; loop++) {
        if (AT_GetBitMap(para->validLocMap, loop) == TAF_TRUE) {
            msgNum++;
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,", loop);
        }
    }

    /* 删除最后一个"," */
    if (msgNum != 0) {
        length -= 1;
    }

    *printOffSet = length;

    return;
}


TAF_VOID At_DeleteTestRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT16                length;
    MN_MSG_DeleteTestEvtInfo *para = VOS_NULL_PTR;
    VOS_UINT32                loop;
    VOS_BOOL                  bMsgExist;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_DeleteTestRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    para = (MN_MSG_DeleteTestEvtInfo *)&event->u.deleteTestInfo;

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGD_TEST) {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: (", g_parseContext[indexNum].cmdElement->cmdName);

        AT_SmsListIndex(length, para, &length);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "),(0-4)");
    } else {
        /* 判断是否有短信索引列表输出: 无短信需要输出直接返回OK */
        bMsgExist = VOS_FALSE;

        for (loop = 0; loop < MN_MSG_CMGD_PARA_MAX_LEN; loop++) {
            if (para->validLocMap[loop] != 0) {
                bMsgExist = VOS_TRUE;
                break;
            }
        }

        if (bMsgExist == VOS_TRUE) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            AT_SmsListIndex(length, para, &length);
        } else {
            length = 0;
        }
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);

    At_FormatResultData(indexNum, AT_OK);

    return;
}


TAF_VOID At_ReadRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT16         length     = 0;
    TAF_UINT32         ret        = AT_OK;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;
    AT_ModemSmsCtx    *smsCtx     = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ReadRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (event->u.readInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.readInfo.failCause);
        At_FormatResultData(indexNum, ret);
        return;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)At_SmsPrintState(smsCtx->cmgfMsgFormat, event->u.readInfo.status,
                                           (g_atSndCodeAddress + length));

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

    tsDataInfo = At_GetMsgMem();
    ret        = MN_MSG_Decode(&event->u.readInfo.msgInfo.tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        ret = At_ChgMnErrCodeToAt(indexNum, ret);
        At_FormatResultData(indexNum, ret);
        return;
    }

    if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_PDU) { /* PDU */
        /* +CMGR: <stat>,[<alpha>],<length><CR><LF><pdu> */
        length += (TAF_UINT16)At_MsgPduInd(&event->u.readInfo.msgInfo.scAddr, &event->u.readInfo.msgInfo.tsRawData,
                                           (g_atSndCodeAddress + length));

        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_OK);
        return;
    }

    switch (event->u.readInfo.msgInfo.tsRawData.tpduType) {
        case MN_MSG_TPDU_DELIVER:
            /* +CMGR: <stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>, <sca>,<tosca>,<length>]<CR><LF><data> */
            /* <oa> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.deliver.origAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");
            /* <alpha> 不报 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <scts> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.deliver.timeStamp, (g_atSndCodeAddress + length));

            if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");
                /* <tooa> */
                length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.deliver.origAddr, (g_atSndCodeAddress + length));
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

                /* <fo> */
                length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));

                /* <pid> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.deliver.pid);

                /* <dcs> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.deliver.dcs.rawDcsData);

                /* <sca> */
                length += (TAF_UINT16)At_PrintBcdAddr(&event->u.readInfo.msgInfo.scAddr, (g_atSndCodeAddress + length));

                /* <tosca> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", event->u.readInfo.msgInfo.scAddr.addrType);

                /* <length> */
                length += AT_PrintSmsLength(tsDataInfo->u.deliver.dcs.msgCoding, tsDataInfo->u.deliver.userData.len,
                                            (g_atSndCodeAddress + length));
            }
            /* <data> 有可能得到是UCS2，需仔细处理 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     tsDataInfo->u.deliver.dcs.msgCoding, (g_atSndCodeAddress + length),
                                                     tsDataInfo->u.deliver.userData.orgData,
                                                     (TAF_UINT16)tsDataInfo->u.deliver.userData.len);

            break;
        case MN_MSG_TPDU_SUBMIT:
            /* +CMGR: <stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>], <sca>,<tosca>,<length>]<CR><LF><data> */
            /* <da> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.submit.destAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");
            /* <alpha> 不报 */

            if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

                /* <toda> */
                length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.submit.destAddr, (g_atSndCodeAddress + length));
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

                /* <fo> */
                length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));

                /* <pid> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.submit.pid);
                /* <dcs> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.submit.dcs.rawDcsData);
                /* <vp>,还需要仔细处理 */
                length += At_MsgPrintVp(&tsDataInfo->u.submit.validPeriod, (g_atSndCodeAddress + length));
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

                /* <sca> */
                length += At_PrintBcdAddr(&event->u.readInfo.msgInfo.scAddr, (g_atSndCodeAddress + length));

                /* <tosca> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", event->u.readInfo.msgInfo.scAddr.addrType);

                /* <length> */
                length += AT_PrintSmsLength(tsDataInfo->u.submit.dcs.msgCoding, tsDataInfo->u.submit.userData.len,
                                            (g_atSndCodeAddress + length));
            }

            /* <data> 有可能得到是UCS2，需仔细处理 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

            length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     tsDataInfo->u.submit.dcs.msgCoding, (g_atSndCodeAddress + length),
                                                     tsDataInfo->u.submit.userData.orgData,
                                                     (TAF_UINT16)tsDataInfo->u.submit.userData.len);

            break;
        case MN_MSG_TPDU_COMMAND:
            /* +CMGR: <stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],<length><CR><LF><cdata>] */
            /* <fo> */
            length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));
            /* <ct> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.command.cmdType);

            if (smsCtx->csdhType == AT_CSDH_SHOW_TYPE) {
                /* <pid> */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.command.pid);

                /* <mn>,还需要仔细处理 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.command.msgNumber);

                /* <da> */
                length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.command.destAddr, (g_atSndCodeAddress + length));
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

                /* <toda> */
                length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.command.destAddr, (g_atSndCodeAddress + length));

                /* <length>为0 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.command.commandDataLen);

                /* <data> 有可能得到是UCS2，需仔细处理 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)(g_atSndCodeAddress + length), "%s", g_atCrLf);

                length += (TAF_UINT16)At_PrintReportData(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         MN_MSG_MSG_CODING_8_BIT, (g_atSndCodeAddress + length),
                                                         tsDataInfo->u.command.cmdData,
                                                         tsDataInfo->u.command.commandDataLen);
            }
            break;
        case MN_MSG_TPDU_STARPT:
            /*
             * +CMGR: <stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>
             */
            /* <fo> */
            length += (TAF_UINT16)At_PrintMsgFo(tsDataInfo, (g_atSndCodeAddress + length));

            /* <mr> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d,", tsDataInfo->u.staRpt.mr);

            /* <ra> */
            length += (TAF_UINT16)At_PrintAsciiAddr(&tsDataInfo->u.staRpt.recipientAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <tora> */
            length += (TAF_UINT16)At_PrintAddrType(&tsDataInfo->u.staRpt.recipientAddr, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <scts> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.timeStamp, (g_atSndCodeAddress + length));
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

            /* <dt> */
            length += (TAF_UINT16)At_SmsPrintScts(&tsDataInfo->u.staRpt.dischargeTime, (g_atSndCodeAddress + length));

            /* <st> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)(g_atSndCodeAddress + length), ",%d", tsDataInfo->u.staRpt.status);
            break;
        default:
            break;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return;
}


TAF_VOID At_ListRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    errno_t            memResult;
    TAF_UINT16         length;
    TAF_UINT32         ret        = AT_OK;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;
    TAF_UINT32         loop;

    MN_MSG_ListParm listParm;

    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ListRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    (VOS_VOID)memset_s(&listParm, sizeof(listParm), 0x00, sizeof(MN_MSG_ListParm));

    if (event->u.listInfo.success != TAF_TRUE) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.listInfo.failCause);
        At_FormatResultData(indexNum, ret);
        return;
    }

    length = 0;
    if (event->u.listInfo.firstListEvt == VOS_TRUE) {
        if (g_atVType == AT_V_ENTIRE_TYPE) {
            length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s", g_atCrLf);
        }
    }
    tsDataInfo = At_GetMsgMem();

    for (loop = 0; loop < AT_MIN(event->u.listInfo.reportNum, MN_MSG_MAX_REPORT_EVENT_NUM); loop++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName,
            event->u.listInfo.smInfo[loop].index);

        length += (TAF_UINT16)At_SmsPrintState(smsCtx->cmgfMsgFormat, event->u.listInfo.smInfo[loop].status,
                                               (g_atSndCodeAddress + length));
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)(g_atSndCodeAddress + length), ",");

        ret = MN_MSG_Decode(&event->u.listInfo.smInfo[loop].msgInfo.tsRawData, tsDataInfo);
        if (ret != MN_ERR_NO_ERROR) {
            ret = At_ChgMnErrCodeToAt(indexNum, ret);
            At_FormatResultData(indexNum, ret);
            return;
        }

        if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_PDU) { /* PDU */
            /*
             * +CMGL: <index>,<stat>,[<alpha>],<length><CR><LF><pdu>
             * [<CR><LF>+CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu>
             * [...]]
             */
            /* ?? */
            length += (TAF_UINT16)At_MsgPduInd(&event->u.listInfo.smInfo[loop].msgInfo.scAddr,
                                               &event->u.listInfo.smInfo[loop].msgInfo.tsRawData,
                                               (g_atSndCodeAddress + length));
        } else {
            length += (TAF_UINT16)At_PrintListMsg(indexNum, event, tsDataInfo, (g_atSndCodeAddress + length));
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);

        length = 0;
    }

    if (event->u.listInfo.lastListEvt == TAF_TRUE) {
        g_atSendDataBuff.bufLen = 0;
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_OK);
    } else {
        /* 初始化 */
        memResult = memcpy_s(&listParm, sizeof(listParm), &(event->u.listInfo.receivedListPara), sizeof(MN_MSG_ListParm));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(listParm), sizeof(listParm));

        /* 通知SMS还需要继续显示剩下的短信 */
        listParm.isFirstTimeReq = VOS_FALSE;

        /* 执行命令操作 */
        if (MN_MSG_List(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &listParm) != MN_ERR_NO_ERROR) {
            g_atSendDataBuff.bufLen = 0;
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_ERROR);
            return;
        }
    }

    return;
}


TAF_VOID At_WriteSmRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT32 ret    = AT_OK;
    TAF_UINT16 length = 0;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_WriteSmRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    if (event->u.writeInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.writeInfo.failCause);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d", event->u.writeInfo.index);
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, ret);
    return;
}


TAF_VOID At_SetCnmaRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetCnmaRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CNMA_TEXT_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CNMA_PDU_SET)) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);
    }
    return;
}



VOS_UINT32 AT_GetSmsRpReportCause(TAF_MSG_ErrorUint32 msgCause)
{
    VOS_UINT32 i;
    VOS_UINT32 mapLength;

    /* 27005 3.2.5 0...127 3GPP TS 24.011 [6] clause E.2 values */
    /* 27005 3.2.5 128...255 3GPP TS 23.040 [3] clause 9.2.3.22 values.  */
    mapLength = sizeof(g_atSmsErrorCodeMap) / sizeof(g_atSmsErrorCodeMap[0]);

    for (i = 0; i < mapLength; i++) {
        if (g_atSmsErrorCodeMap[i].msgErrorCode == msgCause) {
            return g_atSmsErrorCodeMap[i].atErrorCode;
        }
    }

    return AT_CMS_UNKNOWN_ERROR;
}


TAF_VOID At_SendSmRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT32 ret    = AT_OK;
    TAF_UINT16 length = 0;

    AT_INFO_LOG("At_SendSmRspProc: step into function.");

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SendSmRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* 状态不匹配: 当前没有等待发送结果的AT命令，丢弃该结果事件上报 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGS_TEXT_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGS_PDU_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGC_TEXT_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGC_PDU_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMSS_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMST_SET)) {
        return;
    }

    g_atSendDataBuff.bufLen = 0;

    if (event->u.submitRptInfo.errorCode != TAF_MSG_ERROR_NO_ERROR) {
        AT_NORM_LOG("At_SendSmRspProc: pstEvent->u.stSubmitRptInfo.enRptStatus is not ok.");

        ret = AT_GetSmsRpReportCause(event->u.submitRptInfo.errorCode);
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, ret);
        return;
    }

    if (g_atClientTab[indexNum].atSmsData.msgSentSmNum < 1) {
        AT_WARN_LOG("At_SendSmRspProc: the number of sent message is zero.");
        return;
    }
    g_atClientTab[indexNum].atSmsData.msgSentSmNum--;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", event->u.submitRptInfo.mr);

    if (g_atClientTab[indexNum].atSmsData.msgSentSmNum == 0) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, ret);
    } else {
        At_MsgResultCodeFormat(indexNum, length);
    }
    return;
}

#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))


VOS_VOID At_SmsDeliverCbmProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if ((smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_SEND_OR_DISCARD_TYPE) ||
        (smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_SEND_OR_BUFFER_TYPE)) {
        At_ForwardMsgToTe(MN_MSG_EVT_DELIVER_CBM, event);
        return;
    }

    /* 目前CBS消息不缓存 */

    if (smsCtx->cnmiType.cnmiModeType == AT_CNMI_MODE_EMBED_AND_SEND_TYPE) {
        /* 目前不支持 */
    }

    return;
}


VOS_UINT32 AT_CbPrintRange(VOS_UINT16 length, TAF_CBA_CbmiRangeList *cbMidr)
{
    TAF_UINT32 loop;
    TAF_UINT16 addLen;

    addLen = length;
    cbMidr->cbmirNum = AT_MIN(cbMidr->cbmirNum, TAF_CBA_MAX_CBMID_RANGE_NUM);
    for (loop = 0; loop < cbMidr->cbmirNum; loop++) {
        if (cbMidr->cbmiRangeInfo[loop].msgIdFrom == cbMidr->cbmiRangeInfo[loop].msgIdTo) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d", cbMidr->cbmiRangeInfo[loop].msgIdFrom);
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d-%d", cbMidr->cbmiRangeInfo[loop].msgIdFrom,
                cbMidr->cbmiRangeInfo[loop].msgIdTo);
        }

        if (loop != (cbMidr->cbmirNum - 1)) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",");
        }
    }

    addLen = length - addLen;

    return addLen;
}


VOS_VOID At_GetCbActiveMidsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT16      length;
    TAF_UINT16      addLength;
    VOS_UINT32      ret;
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_GetCbActiveMidsRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* 停止定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->u.cbsCbMids.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.cbsCbMids.failCause);
        At_FormatResultData(indexNum, ret);
        return;
    }

    length = 0;

    /* 保持的永远是激活列表,所以固定填写0 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s:0,", g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    /* 输出消息的MID */
    addLength = (VOS_UINT16)AT_CbPrintRange(length, &(event->u.cbsCbMids.cbMidr));

    length += addLength;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\",\"");

    /* 输出的语言的MID */
    addLength = (VOS_UINT16)AT_CbPrintRange(length, &(smsCtx->cbsDcssInfo));

    length += addLength;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return;
}


VOS_VOID AT_ChangeCbMidsRsp(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_UINT32 ret;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_ChangeCbMidsRsp : AT_BROADCAST_INDEX.");
        return;
    }

    if (event->u.cbsChangeInfo.success != TAF_TRUE) {
        ret = At_ChgMnErrCodeToAt(indexNum, event->u.cbsChangeInfo.failCause);
    } else {
        ret = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);
}

#if (FEATURE_ETWS == FEATURE_ON)

VOS_VOID At_ProcDeliverEtwsPrimNotify(VOS_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    TAF_CBA_EtwsPrimNtfEvtInfo *primNtf = VOS_NULL_PTR;
    VOS_UINT16                  length;

    primNtf = &event->u.etwsPrimNtf;

    /* ^ETWSPN: <plmn id>,<warning type>,<msg id>,<sn>,<auth> [,<warning security information>] */
    /* 示例: ^ETWSPN: "46000",0180,4352,3000,1 */

    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(g_atSndCodeAddress + length), "%s^ETWSPN: ", g_atCrLf);

    /*
     * <plmn id>
     * ulMcc，ulMnc的说明及示例：
     * ulMcc的低8位    （即bit0--bit7），对应 MCC digit 1;
     * ulMcc的次低8位  （即bit8--bit15），对应 MCC digit 2;
     * ulMcc的次次低8位（即bit16--bit23），对应 MCC digit 3;
     * ulMnc的低8位    （即bit0--bit7），对应 MNC digit 1;
     * ulMnc的次低8位  （即bit8--bit15），对应 MNC digit 2;
     * ulMnc的次次低8位（即bit16--bit23），对应 MNC digit 3;
     */
    if ((primNtf->plmn.mnc & 0xFF0000) == 0x0F0000) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "\"%d%d%d%d%d\",", (primNtf->plmn.mcc & 0xFF),
            (primNtf->plmn.mcc & 0xFF00) >> 8, (primNtf->plmn.mcc & 0xFF0000) >> 16, (primNtf->plmn.mnc & 0xFF),
            (primNtf->plmn.mnc & 0xFF00) >> 8);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "\"%d%d%d%d%d%d\",", (primNtf->plmn.mcc & 0xFF),
            (primNtf->plmn.mcc & 0xFF00) >> 8, (primNtf->plmn.mcc & 0xFF0000) >> 16, (primNtf->plmn.mnc & 0xFF),
            (primNtf->plmn.mnc & 0xFF00) >> 8, (primNtf->plmn.mnc & 0xFF0000) >> 16);
    }

    /* <warning type> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(g_atSndCodeAddress + length), "%04X,", primNtf->warnType);
    /* <msg id> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(g_atSndCodeAddress + length), "%04X,", primNtf->msgId);
    /* <sn> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(g_atSndCodeAddress + length), "%04X,", primNtf->sn);

    /* <auth> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)(g_atSndCodeAddress + length), "%d%s", primNtf->authRslt, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}
#endif /* (FEATURE_ETWS == FEATURE_ON) */

#endif


TAF_VOID At_SetCmmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_RreturnCodeUint32 result = AT_CMS_UNKNOWN_ERROR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetCmmsRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->u.linkCtrlInfo.errorCode == MN_ERR_NO_ERROR) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return;
}


TAF_VOID At_GetCmmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_RreturnCodeUint32 result = AT_CMS_UNKNOWN_ERROR;
    /* event report:MN_MSG_EVT_SET_COMM_PARAM */
    MN_MSG_LinkCtrlEvtInfo *linkCtrlInfo = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_GetCmmsRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    g_atSendDataBuff.bufLen = 0;
    linkCtrlInfo            = &event->u.linkCtrlInfo;
    if (linkCtrlInfo->errorCode == MN_ERR_NO_ERROR) {
        result = AT_OK;
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                linkCtrlInfo->linkCtrl);
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return;
}


TAF_VOID At_SetCgsmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_RreturnCodeUint32 result;

    result = AT_CMS_UNKNOWN_ERROR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetCgsmsRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->u.sendDomainInfo.errorCode == MN_ERR_NO_ERROR) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return;
}


TAF_VOID At_GetCgsmsRspProc(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_RreturnCodeUint32 result;
    /* event report:MN_MSG_EVT_SET_COMM_PARAM */
    MN_MSG_SendDomainEvtInfo *sendDomainInfo = VOS_NULL_PTR;

    result = AT_CMS_UNKNOWN_ERROR;
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_GetCgsmsRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    g_atSendDataBuff.bufLen = 0;
    sendDomainInfo          = &event->u.sendDomainInfo;

    if (sendDomainInfo->errorCode == MN_ERR_NO_ERROR) {
        result = AT_OK;
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                sendDomainInfo->sendDomain);
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return;
}


TAF_VOID At_SmsRspNop(TAF_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    AT_INFO_LOG("At_SmsRspNop: no operation need for the event type ");
    return;
}


TAF_VOID At_SmsMsgProc(MN_AT_IndEvt *data, TAF_UINT16 len)
{
    MN_MSG_EventInfo  *event = VOS_NULL_PTR;
    MN_MSG_EventUint32 eventTemp;
    TAF_UINT8          indexNum = 0;
    TAF_UINT32         eventLen;
    errno_t            memResult;

    eventTemp = MN_MSG_EVT_MAX;

    AT_INFO_LOG("At_SmsMsgProc: Step into function.");
    AT_LOG1("At_SmsMsgProc: pstData->clientId,", data->clientId);

    eventLen  = sizeof(MN_MSG_EventUint32);
    memResult = memcpy_s(&eventTemp, sizeof(eventTemp), data->content, eventLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(eventTemp), eventLen);
    event = (MN_MSG_EventInfo *)&data->content[eventLen];

    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SmsMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        if (event->opId != g_atClientTab[indexNum].opId) {
            AT_LOG1("At_SmsMsgProc: pstEvent->opId,", event->opId);
            AT_LOG1("At_SmsMsgProc: g_atClientTab[ucIndex].opId,", g_atClientTab[indexNum].opId);
            AT_NORM_LOG("At_SmsMsgProc: invalid operation id.");
            return;
        }

        AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);
    }

    if (eventTemp >= MN_MSG_EVT_MAX) {
        AT_WARN_LOG("At_SmsRspProc: invalid event type.");
        return;
    }

    AT_LOG1("At_SmsMsgProc enEvent", eventTemp);
    g_atSmsMsgProcTable[eventTemp](indexNum, event);
    return;
}


VOS_VOID At_ProcVcSetVoiceMode(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvt)
{
    AT_VmsetCmdCtx *vmSetCmdCtx = VOS_NULL_PTR;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ProcVcSetVoiceMode : AT_BROADCAST_INDEX.");
        return;
    }

    /* 状态判断 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VMSET_SET) {
        AT_WARN_LOG("At_ProcVcSetVoiceMode : opt error.");
        return;
    }

    vmSetCmdCtx = AT_GetCmdVmsetCtxAddr();

    if (vcEvt->success != VOS_TRUE) {
        vmSetCmdCtx->result = AT_ERROR;
    }
#if (MULTI_MODEM_NUMBER >= 2)
    /* VMSET收齐所有MODEM回复后再上报结果 */
    vmSetCmdCtx->reportedModemNum++;
    if (vmSetCmdCtx->reportedModemNum < MULTI_MODEM_NUMBER) {
        return;
    }
#endif

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, vmSetCmdCtx->result);

    /* 初始化设置结果全局变量 */
    AT_InitVmSetCtx();
    return;
}


VOS_UINT32 At_ProcVcGetVolumeEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvt)
{
    const VOS_UINT8    intraVolume[AT_CMD_CLVL_VAL_NUM] = {
        AT_CMD_CLVL_LEV_0, AT_CMD_CLVL_LEV_1, AT_CMD_CLVL_LEV_2, AT_CMD_CLVL_LEV_3, AT_CMD_CLVL_LEV_4, AT_CMD_CLVL_LEV_5
    };
    VOS_UINT8          volumnLvl;
    VOS_UINT32         i;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("APP_VC_AppQryVolumeProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLVL_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (vcEvt->success == VOS_TRUE) {
        /* 格式化AT+CLVL命令返回 */
        g_atSendDataBuff.bufLen = 0;

        volumnLvl = 0;
        for (i = 0; i < AT_CMD_CLVL_VAL_NUM; i++) {
            if (intraVolume[i] == vcEvt->volume) {
                volumnLvl = (VOS_UINT8)i;
                break;
            }
        }

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, volumnLvl);

        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 At_ProcVcSetMuteStatusEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    VOS_UINT32 rslt;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ProcVcSetMuteStatusEvent : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMUT_SET) {
        return VOS_ERR;
    }

    if (vcEvtInfo->success != VOS_TRUE) {
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 At_ProcVcGetMuteStatusEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    VOS_UINT32 rslt;
    VOS_UINT16 length = 0;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ProcVcSetMuteStatusEvent : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMUT_READ) {
        return VOS_ERR;
    }

    if (vcEvtInfo->success == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            vcEvtInfo->muteStatus);

        rslt = AT_OK;

        g_atSendDataBuff.bufLen = length;
    } else {
        rslt = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_VOID At_VcEventProc(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvt, APP_VC_EventUint32 event)
{
    TAF_UINT32 ret;
    switch (event) {
        case APP_VC_EVT_SET_VOLUME:
            if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
                AT_WARN_LOG("At_VcEventProc : AT_BROADCAST_INDEX.");
                return;
            }

            if (vcEvt->success != TAF_TRUE) {
                ret = AT_ERROR;
            } else {
                ret = AT_OK;
            }

            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, ret);
            return;

        case APP_VC_EVT_SET_VOICE_MODE:
            At_ProcVcSetVoiceMode(indexNum, vcEvt);
            return;

        case APP_VC_EVT_GET_VOLUME:
            At_ProcVcGetVolumeEvent(indexNum, vcEvt);
            return;

        case APP_VC_EVT_PARM_CHANGED:
            return;

        case APP_VC_EVT_SET_MUTE_STATUS:
            At_ProcVcSetMuteStatusEvent(indexNum, vcEvt);
            return;

        case APP_VC_EVT_GET_MUTE_STATUS:
            At_ProcVcGetMuteStatusEvent(indexNum, vcEvt);
            return;

#if (FEATURE_ECALL == FEATURE_ON)
        case APP_VC_EVT_ECALL_TRANS_STATUS:
            At_ProcVcReportEcallStateEvent(indexNum, vcEvt);
            return;

        case APP_VC_EVT_SET_ECALL_CFG:
            At_ProcVcSetEcallCfgEvent(indexNum, vcEvt);
            return;
        /* eCall支持abort操作和ALACK上报AT */
        case APP_VC_EVT_ECALL_ABORT_CNF:
            (VOS_VOID)AT_ProcVcEcallAbortCnf(indexNum, vcEvt);
            return;
        case APP_VC_EVT_ECALL_TRANS_ALACK:
            (VOS_VOID)AT_ProcVcReportEcallAlackEvent(indexNum, vcEvt);
            return;
#endif

        default:
            return;
    }
}


TAF_VOID At_VcMsgProc(MN_AT_IndEvt *data, TAF_UINT16 len)
{
    APP_VC_EventInfo  *event = VOS_NULL_PTR;
    APP_VC_EventUint32 eventTemp;
    TAF_UINT8          indexNum = 0;
    TAF_UINT32         eventLen;
    errno_t            memResult;

    eventTemp = APP_VC_EVT_BUTT;

    AT_INFO_LOG("At_VcMsgProc: Step into function.");
    AT_LOG1("At_VcMsgProc: pstData->clientId,", data->clientId);

    eventLen  = sizeof(APP_VC_EventUint32);
    memResult = memcpy_s(&eventTemp, sizeof(eventTemp), data->content, eventLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(eventTemp), eventLen);
    event = (APP_VC_EventInfo *)&data->content[eventLen];

    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_VcMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_LOG1("At_VcMsgProc: ucIndex", indexNum);
        if (indexNum >= AT_MAX_CLIENT_NUM) {
            AT_WARN_LOG("At_VcMsgProc: invalid CLIENT ID or indexNum.");
            return;
        }

        if (event->opId != g_atClientTab[indexNum].opId) {
            AT_LOG1("At_VcMsgProc: pstEvent->opId,", event->opId);
            AT_LOG1("At_VcMsgProc: g_atClientTab[ucIndex].opId,", g_atClientTab[indexNum].opId);
            AT_NORM_LOG("At_VcMsgProc: invalid operation id.");
            return;
        }

        AT_LOG1("g_atClientTab[ucIndex].CmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);
    }

    if (eventTemp >= APP_VC_EVT_BUTT) {
        AT_WARN_LOG("At_SmsRspProc: invalid event type.");
        return;
    }

    AT_LOG1("At_VcMsgProc enEvent", eventTemp);
    At_VcEventProc(indexNum, event, eventTemp);
}


TAF_VOID At_SetParaRspProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_PARA_SET_RESULT result, TAF_PARA_TYPE paraType)
{
    AT_RreturnCodeUint32 ret    = AT_FAILURE;
    TAF_UINT16           length = 0;

    /* 如果是PS域的复合命令 */
    if (g_atClientTab[indexNum].asyRtnNum > 0) {
        g_atClientTab[indexNum].asyRtnNum--; /* 命令个数减1 */
        if (result == TAF_PARA_OK) {
            if (g_atClientTab[indexNum].asyRtnNum != 0) {
                return; /* 如果OK并且还有其它命令 */
            }
        } else {
            g_atClientTab[indexNum].asyRtnNum = 0; /* 如果ERROR则不再上报其它命令结果 */
        }
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    switch (result) {
        case TAF_PARA_OK:
            ret = AT_OK;
            break;

        case TAF_PARA_SIM_IS_BUSY:
            if (g_parseContext[indexNum].cmdElement->cmdIndex > AT_CMD_SMS_BEGAIN) {
                ret = AT_CMS_U_SIM_BUSY;
            } else {
                ret = AT_CME_SIM_BUSY;
            }
            break;

        default:
            if (g_parseContext[indexNum].cmdElement->cmdIndex > AT_CMD_SMS_BEGAIN) {
                ret = AT_CMS_UNKNOWN_ERROR;
            } else {
                ret = AT_CME_UNKNOWN;
            }
            break;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, ret);
}

TAF_VOID At_SetMsgProc(TAF_SetRslt *setRslt)
{
    TAF_UINT8 indexNum;

    AT_LOG1("At_SetMsgProc ClientId", setRslt->clientId);
    AT_LOG1("At_SetMsgProc Result", setRslt->rslt);
    AT_LOG1("At_SetMsgProc ParaType", setRslt->paraType);

    if (setRslt->clientId == AT_BUTT_CLIENT_ID) {
        AT_WARN_LOG("At_SetMsgProc Error ucIndex");
        return;
    }

    indexNum = 0;
    if (At_ClientIdToUserId(setRslt->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SetMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetMsgProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_SetMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    At_SetParaRspProc(indexNum, setRslt->opId, setRslt->rslt, setRslt->paraType);
}



VOS_UINT16 AT_GetOperNameLengthForCops(TAF_CHAR *pstr, TAF_UINT8 maxLen)
{
    VOS_UINT16 rsltLen;
    TAF_UINT8  i;

    rsltLen = 0;

    for (i = 0; i < maxLen; i++) {
        if (pstr[i] != '\0') {
            rsltLen = i + 1;
        }
    }

    return rsltLen;
}

VOS_VOID At_QryParaRspCopsProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const TAF_UINT8 *para)
{
    errno_t            memResult;
    VOS_UINT32         result     = AT_FAILURE;
    VOS_UINT16         length     = 0;
    VOS_UINT16         nameLength = 0;
    TAF_PH_Networkname cops;
    AT_ModemNetCtx    *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    memset_s(&cops, sizeof(cops), 0x00, sizeof(TAF_PH_Networkname));

    memResult = memcpy_s(&cops, sizeof(cops), para, sizeof(TAF_PH_Networkname));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cops), sizeof(TAF_PH_Networkname));

    /*
     * A32D07158
     * +COPS: <mode>[,<format>,<oper>[,<AcT>]], get the PLMN selection mode from msg sent by MMA
     */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        cops.plmnSelMode);

    /* MBB定制支持COPS=2查询时返回2，此场景同样只需要显示sel mode */
    if ((AT_PH_IsPlmnValid(&(cops.name.plmnId)) == VOS_FALSE)
    ) {
        /* 无效 PLMNId 只显示 sel mode */
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
        return;
    }

    /* <format> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", netCtx->copsFormatType);

    /* <oper> */
    switch (netCtx->copsFormatType) {
        /* 长名字，字符串类型 */
        case AT_COPS_LONG_ALPH_TYPE:
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"");

            /* 针对显示SPN中存在0x00有效字符的情况,获取其实际长度,At_sprintf以0x00结尾，不可用 */
            nameLength = AT_GetOperNameLengthForCops(cops.name.operatorNameLong, TAF_PH_OPER_NAME_LONG);

            if (nameLength > 0) {
                memResult = memcpy_s(g_atSndCodeAddress + length,
                                     AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET - length,
                                     cops.name.operatorNameLong, nameLength);
                TAF_MEM_CHK_RTN_VAL(memResult,
                                    AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET - length,
                                    nameLength);
            }

            length = length + nameLength;
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
            break;

            /* 短名字，字符串类型 */
        case AT_COPS_SHORT_ALPH_TYPE:
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"");

            /* 针对显示SPN中存在0x00有效字符的情况,获取其实际长度,At_sprintf以0x00结尾，不可用 */
            nameLength = AT_GetOperNameLengthForCops(cops.name.operatorNameShort, TAF_PH_OPER_NAME_SHORT);

            if (nameLength > 0) {
                memResult = memcpy_s(g_atSndCodeAddress + length,
                                     AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET - length,
                                     cops.name.operatorNameShort, nameLength);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET - length,
                                    nameLength);
            }

            length = length + nameLength;
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
            break;

        /* BCD码的MCC、MNC，需要转换成字符串 */
        default:
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%X%X%X", (0x0f00 & cops.name.plmnId.mcc) >> 8,
                (0x00f0 & cops.name.plmnId.mcc) >> 4, (0x000f & cops.name.plmnId.mcc));

            if (((0x0f00 & cops.name.plmnId.mnc) >> 8) != 0x0F) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%X", (0x0f00 & cops.name.plmnId.mnc) >> 8);
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%X%X\"", (0x00f0 & cops.name.plmnId.mnc) >> 4,
                (0x000f & cops.name.plmnId.mnc));
            break;
    }

    /* <AcT> */
    if (cops.raMode == TAF_PH_RA_GSM) { /* GSM */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0");
    } else if (cops.raMode == TAF_PH_RA_WCDMA) { /* CDMA */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",2");
    } else if (cops.raMode == TAF_PH_RA_LTE) { /* LTE */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",7");
    }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    else if (cops.raMode == TAF_PH_RA_NR) { /* NR */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",12");
    }
#endif
    else {
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


TAF_VOID At_QryParaRspSysinfoProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    errno_t        memResult;
    VOS_UINT32     result;
    VOS_UINT16     length;
    TAF_PH_Sysinfo sysInfo;
    VOS_UINT8     *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    result = AT_FAILURE;
    length = 0;

    memset_s(&sysInfo, sizeof(sysInfo), 0x00, sizeof(TAF_PH_Sysinfo));

    memResult = memcpy_s(&sysInfo, sizeof(sysInfo), para, sizeof(TAF_PH_Sysinfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sysInfo), sizeof(TAF_PH_Sysinfo));
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
        sysInfo.srvStatus);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.srvDomain);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.roamStatus);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.sysMode);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.simStatus);

    if (*systemAppConfig == SYSTEM_APP_WEBUI) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.simLockStatus);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",");
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.sysSubMode);

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}

/*
 * Description: 参数查询结果Cimi的上报处理
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_VOID At_QryParaRspCimiProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    errno_t    memResult;
    TAF_UINT32 result;
    TAF_UINT16 length = 0;

    TAF_PH_Imsi cimi;

    memResult = memcpy_s(&cimi, sizeof(cimi), para, sizeof(TAF_PH_Imsi));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cimi), sizeof(TAF_PH_Imsi));
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", cimi.imsi);

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


TAF_VOID At_QryParaRspCgclassProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    errno_t    memResult;
    TAF_UINT32 result;
    TAF_UINT16 length = 0;

    TAF_PH_MS_CLASS_TYPE cgclass;

    cgclass = TAF_PH_MS_CLASS_NULL;

    memResult = memcpy_s(&cgclass, sizeof(cgclass), para, sizeof(TAF_PH_MS_CLASS_TYPE));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgclass), sizeof(TAF_PH_MS_CLASS_TYPE));
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    if (cgclass == TAF_PH_MS_CLASS_A) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"A\"");
    } else if (cgclass == TAF_PH_MS_CLASS_B) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"B\"");
    } else if (cgclass == TAF_PH_MS_CLASS_CG) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"CG\"");
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"CC\"");
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


VOS_VOID At_QryParaRspCregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;

    TAF_PH_RegState creg;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    memset_s(&creg, sizeof(creg), 0x00, sizeof(TAF_PH_RegState));

    memResult = memcpy_s(&creg, sizeof(creg), para, sizeof(TAF_PH_RegState));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(creg), sizeof(TAF_PH_RegState));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        (VOS_UINT32)netCtx->cregType);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", creg.regState);

    if ((netCtx->cregType == AT_CREG_RESULT_CODE_ENTIRE_TYPE) &&
        ((creg.regState == TAF_PH_REG_REGISTERED_HOME_NETWORK) || (creg.regState == TAF_PH_REG_REGISTERED_ROAM))) {
        length += (VOS_UINT16)At_PhReadCreg(&creg, g_atSndCodeAddress + length);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_VOID At_QryParaRspCgregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;

    TAF_PH_RegState cgreg;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    memset_s(&cgreg, sizeof(cgreg), 0x00, sizeof(TAF_PH_RegState));

    memResult = memcpy_s(&cgreg, sizeof(cgreg), para, sizeof(TAF_PH_RegState));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgreg), sizeof(TAF_PH_RegState));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        (VOS_UINT32)netCtx->cgregType);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgreg.psRegState);

    if ((netCtx->cgregType == AT_CGREG_RESULT_CODE_ENTIRE_TYPE) &&
        ((cgreg.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
         (cgreg.psRegState == TAF_PH_REG_REGISTERED_ROAM)) &&
        (cgreg.act != TAF_PH_ACCESS_TECH_NR_5GC)) {
        length += (VOS_UINT16)At_PhReadCreg(&cgreg, g_atSndCodeAddress + length);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_VOID AT_QryParaRspCeregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;

    TAF_PH_RegState cereg;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    memset_s(&cereg, sizeof(cereg), 0x00, sizeof(TAF_PH_RegState));

    memResult = memcpy_s(&cereg, sizeof(cereg), para, sizeof(TAF_PH_RegState));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cereg), sizeof(TAF_PH_RegState));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        (VOS_UINT32)netCtx->ceregType);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cereg.psRegState);

    /* GU下查询只上报stat */
    if ((netCtx->ceregType == AT_CEREG_RESULT_CODE_ENTIRE_TYPE) && (cereg.act == TAF_PH_ACCESS_TECH_E_UTRAN) &&
        ((cereg.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
         (cereg.psRegState == TAF_PH_REG_REGISTERED_ROAM))) {
        length += (VOS_UINT16)At_PhReadCreg(&cereg, g_atSndCodeAddress + length);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_QryParaRspC5gregProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;

    TAF_PH_RegState c5greg;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT32      lengthTemp;
    VOS_CHAR        acStrAllowedNssai[AT_EVT_MULTI_S_NSSAI_LEN];

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    memset_s(&c5greg, sizeof(c5greg), 0x00, sizeof(TAF_PH_RegState));
    memResult = memcpy_s(&c5greg, sizeof(c5greg), para, sizeof(TAF_PH_RegState));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(c5greg), sizeof(TAF_PH_RegState));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        (VOS_UINT32)netCtx->c5gregType);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", c5greg.psRegState);

    if ((netCtx->c5gregType == AT_C5GREG_RESULT_CODE_ENTIRE_TYPE) && (c5greg.act == TAF_PH_ACCESS_TECH_NR_5GC) &&
        ((c5greg.psRegState == TAF_PH_REG_REGISTERED_HOME_NETWORK) ||
         (c5greg.psRegState == TAF_PH_REG_REGISTERED_ROAM))) {
        length += (VOS_UINT16)At_PhReadC5greg(&c5greg, g_atSndCodeAddress + length);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");

        /* 组装切片消息格式 */
        lengthTemp = 0;
        memset_s(acStrAllowedNssai, sizeof(acStrAllowedNssai), 0, sizeof(acStrAllowedNssai));

        AT_ConvertMultiSNssaiToString(AT_MIN(c5greg.allowedNssai.snssaiNum, PS_MAX_ALLOWED_S_NSSAI_NUM),
                                      &c5greg.allowedNssai.snssai[0], acStrAllowedNssai, sizeof(acStrAllowedNssai),
                                      &lengthTemp);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", lengthTemp);

        if (lengthTemp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrAllowedNssai);
        }
    }

    result = AT_OK;

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return;
}
#endif


TAF_VOID At_QryParaRspIccidProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    errno_t      memResult;
    TAF_UINT32   result;
    TAF_UINT16   length;
    TAF_PH_IccId iccId;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ICCID_READ) {
        return;
    }

    length = 0;
    memset_s(&iccId, sizeof(iccId), 0x00, sizeof(TAF_PH_IccId));
    memResult = memcpy_s(&iccId, sizeof(iccId), para, sizeof(TAF_PH_IccId));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(iccId), sizeof(TAF_PH_IccId));

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_Hex2AsciiStrLowHalfFirst(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                      (VOS_UINT8 *)g_atSndCodeAddress + length, iccId.iccId, iccId.len);

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


TAF_VOID At_QryRspUsimRangeProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    TAF_PH_QryUsimRangeInfo *usimRangeInfo = VOS_NULL_PTR;
    TAF_UINT16               length        = 0;
    TAF_UINT32               result;
    TAF_UINT8                simValue;
    VOS_BOOL                 bUsimInfoPrinted = VOS_FALSE;

    usimRangeInfo = (TAF_PH_QryUsimRangeInfo *)para;
    if ((usimRangeInfo->usimInfo.fileExist == VOS_TRUE) && (usimRangeInfo->usimInfo.icctype == TAF_PH_ICC_USIM)) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        simValue = 1;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,(1,%d),%d", simValue, usimRangeInfo->usimInfo.totalRecNum,
            usimRangeInfo->usimInfo.recordLen);
        bUsimInfoPrinted = VOS_TRUE;
    }
    if ((usimRangeInfo->simInfo.fileExist == VOS_TRUE) && (usimRangeInfo->simInfo.icctype == TAF_PH_ICC_SIM)) {
        if (bUsimInfoPrinted == VOS_TRUE) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        simValue = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,(1,%d),%d", simValue, usimRangeInfo->simInfo.totalRecNum,
            usimRangeInfo->simInfo.recordLen);
    }
    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}

TAF_VOID At_QryParaRspPnnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    TAF_UINT16 length = 0;

    TAF_PH_UsimPnnCnf    *pnn = VOS_NULL_PTR;
    TAF_UINT8             fullNameLen;
    TAF_UINT8             shortNameLen;
    TAF_UINT8             tag;
    TAF_UINT8             firstByte;
    VOS_UINT8             pnnOperNameLen;
    TAF_UINT32            i;
    TAF_UINT32            ret;
    TAF_PH_QryUsimInfo    usimInfo;
    MN_CLIENT_OperationId clientOperationId = {0};
    memset_s(&usimInfo, sizeof(usimInfo), 0x00, sizeof(usimInfo));

    pnn = (TAF_PH_UsimPnnCnf *)para;

    /* 查询PNN记录数和记录长度 */
    for (i = 0; i < pnn->totalRecordNum; i++) {
        fullNameLen  = 0;
        shortNameLen = 0;

        fullNameLen = pnn->pnnRecord[i].operNameLong.length;

        if (fullNameLen == 0) {
            continue;
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        /* 打印长名,需要加上TAG,长度和编码格式 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
        tag = FULL_NAME_IEI;
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length, &tag, 1);
        pnnOperNameLen = pnn->pnnRecord[i].operNameLong.length + 1;
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length, &pnnOperNameLen, 1);
        firstByte = (TAF_UINT8)((pnn->pnnRecord[i].operNameLong.ext << 7) |
                                (pnn->pnnRecord[i].operNameLong.coding << 4) |
                                (pnn->pnnRecord[i].operNameLong.addCi << 3) | (pnn->pnnRecord[i].operNameLong.spare));
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length, &firstByte, 1);
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                      pnn->pnnRecord[i].operNameLong.operatorName, fullNameLen);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

        shortNameLen = pnn->pnnRecord[i].operNameShort.length;

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",");
        if (shortNameLen != 0) {
            tag = SHORT_NAME_IEI;
            /* 打印短名,需要加上TAG,长度和编码格式 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length, &tag, 1);
            pnnOperNameLen = pnn->pnnRecord[i].operNameShort.length + 1;
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length, &pnnOperNameLen, 1);
            firstByte =
                (TAF_UINT8)((pnn->pnnRecord[i].operNameShort.ext << 7) | (pnn->pnnRecord[i].operNameShort.coding << 4) |
                            (pnn->pnnRecord[i].operNameShort.addCi << 3) | (pnn->pnnRecord[i].operNameShort.spare));
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length, &firstByte, 1);
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                          pnn->pnnRecord[i].operNameShort.operatorName, shortNameLen);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\"");
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",");

        if (pnn->pnnRecord[i].plmnAdditionalInfoLen != 0) {
            /* PNN的其它信息,需要加上tag和长度 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
            tag = PLMN_ADDITIONAL_INFO_IEI;
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length, &tag, 1);
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                          &pnn->pnnRecord[i].plmnAdditionalInfoLen, 1);
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                          pnn->pnnRecord[i].plmnAdditionalInfo,
                                                          pnn->pnnRecord[i].plmnAdditionalInfoLen);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\"");
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
        length = 0;
    }

    /* 如果本次上报的PNN数目与要求的相同，则认为C核仍有PNN没有上报，要继续发送请求进行查询 */
    if (pnn->totalRecordNum == TAF_MMA_PNN_INFO_MAX_NUM) {
        usimInfo.recNum                   = 0;
        usimInfo.efId                     = TAF_PH_PNN_FILE;
        usimInfo.icctype                  = pnn->icctype;
        usimInfo.pnnQryIndex.pnnNum       = TAF_MMA_PNN_INFO_MAX_NUM;
        usimInfo.pnnQryIndex.pnnCurrIndex = pnn->pnnCurrIndex + TAF_MMA_PNN_INFO_MAX_NUM;

        clientOperationId.clientId = g_atClientTab[indexNum].clientId;
        clientOperationId.opId     = 0;

        ret = MN_FillAndSndAppReqMsg(&clientOperationId, TAF_MSG_MMA_USIM_INFO, &usimInfo, sizeof(TAF_PH_QryUsimInfo),
                                     I0_WUEPS_PID_MMA);

        if (ret != TAF_SUCCESS) {
            /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_ERROR);
        }
    } else {
        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);
    }
}

TAF_VOID At_QryParaRspCPnnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    TAF_UINT32         result;
    TAF_UINT16         length = 0;
    TAF_UINT8          codingScheme;
    TAF_PH_UsimPnnCnf *pnn = VOS_NULL_PTR;
    TAF_UINT8          fullNameLen;
    TAF_UINT8          shortNameLen;
    TAF_UINT8          tag;

    pnn = (TAF_PH_UsimPnnCnf *)para;

    if (pnn->totalRecordNum != 0) {
        fullNameLen  = 0;
        shortNameLen = 0;

        fullNameLen = pnn->pnnRecord[0].operNameLong.length;

        if (fullNameLen != 0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

            /* 打印长名 */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
            tag = FULL_NAME_IEI;
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length, &tag, 1);
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                          (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                          pnn->pnnRecord[0].operNameLong.operatorName, fullNameLen);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

            codingScheme = pnn->pnnRecord[0].operNameLong.coding;
            if (pnn->pnnRecord[0].operNameLong.coding != 0) {
                codingScheme = 1;
            }

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d,%d", codingScheme, pnn->pnnRecord[0].operNameLong.addCi);

            shortNameLen = pnn->pnnRecord[0].operNameShort.length;

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",");

            if (shortNameLen != 0) {
                /* 打印短名 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"");
                length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                              (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                              pnn->pnnRecord[0].operNameShort.operatorName,
                                                              shortNameLen);
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

                codingScheme = pnn->pnnRecord[0].operNameShort.coding;
                if (codingScheme != 0) {
                    codingScheme = 1;
                }

                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",%d,%d", codingScheme,
                    pnn->pnnRecord[0].operNameShort.addCi);
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "\"\"");

                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",0,0");
            }
        }
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


TAF_UINT8 At_IsOplRecPrintable(TAF_PH_UsimOplRecord *oplRec, VOS_CHAR wildCard)
{
    TAF_UINT32 i;

    VOS_UINT8 wildCardTemp;

    wildCardTemp = 0x00;

    AT_ConvertCharToHex((VOS_UINT8)wildCard, &wildCardTemp);

    if (oplRec->pnnIndex == 0xFF) {
        return VOS_FALSE;
    }

    for (i = 0; i < (VOS_UINT32)AT_MIN(oplRec->plmnLen, AT_PLMN_MCC_MNC_LEN); i++) {
        if ((oplRec->plmn[i] >= 0xA) && (wildCardTemp != oplRec->plmn[i])) {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}

/*
 * Description: 参数查询结果Opl的上报处理
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_VOID At_QryParaRspOplProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    TAF_UINT32         result;
    TAF_UINT16         length = 0;
    TAF_UINT32         i;
    TAF_UINT32         j;
    TAF_PH_UsimOplCnf *opl           = VOS_NULL_PTR;
    TAF_UINT32         recCntPrinted = 0;

    opl = (TAF_PH_UsimOplCnf *)para;

    /* 查询PNN记录数和记录长度 */
    for (i = 0; i < opl->totalRecordNum; i++) {
        if (At_IsOplRecPrintable((opl->oplRecord + i), opl->wildCard) == VOS_FALSE) {
            continue;
        }
        if (recCntPrinted != 0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        opl->oplRecord[i].plmnLen = AT_MIN(opl->oplRecord[i].plmnLen, AT_PLMN_MCC_MNC_LEN);
        for (j = 0; j < opl->oplRecord[i].plmnLen; j++) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X", opl->oplRecord[i].plmn[j]);
        }
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0x%X-0x%X,%d", opl->oplRecord[i].lACLow,
            opl->oplRecord[i].lACHigh, opl->oplRecord[i].pnnIndex);

        ++recCntPrinted;
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


TAF_VOID At_QryParaRspCfplmnProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_UINT8 *para)
{
    TAF_UINT32         result;
    TAF_UINT16         length       = 0;
    TAF_USER_PlmnList *userPlmnList = VOS_NULL_PTR;
    TAF_UINT32         i;

    userPlmnList = (TAF_USER_PlmnList *)para;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        userPlmnList->plmnNum);
    userPlmnList->plmnNum = AT_MIN(userPlmnList->plmnNum, TAF_USER_MAX_PLMN_NUM);
    for (i = 0; i < userPlmnList->plmnNum; i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%X%X%X", (0x0f00 & userPlmnList->plmn[i].mcc) >> 8,
            (0x00f0 & userPlmnList->plmn[i].mcc) >> 4, (0x000f & userPlmnList->plmn[i].mcc));

        if ((0x0f00 & userPlmnList->plmn[i].mnc) == 0x0f00) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X\"", (0x00f0 & userPlmnList->plmn[i].mnc) >> 4,
                (0x000f & userPlmnList->plmn[i].mnc));
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X%X\"", (0x0f00 & userPlmnList->plmn[i].mnc) >> 8,
                (0x00f0 & userPlmnList->plmn[i].mnc) >> 4, (0x000f & userPlmnList->plmn[i].mnc));
        }
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


VOS_UINT32 AT_RcvTafCcmGetCdurCnf(struct MsgCB *msg)
{
    TAF_CCM_GetCdurCnf *cdurCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexNum = 0;

    cdurCnf = (TAF_CCM_GetCdurCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cdurCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallSsInd: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* AT模块在等待CDUR查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CDUR_READ) {
        return VOS_ERR;
    }

    result = AT_OK;

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cdurCnf->getCdurPara.cause == TAF_CS_CAUSE_SUCCESS) {
        /* 输出查询结果: 构造结构为^CDUR: <CurCallTime>格式 */
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            cdurCnf->getCdurPara.callId, cdurCnf->getCdurPara.curCallTime);

        result = AT_OK;

    } else {
        result = AT_ConvertCallError(cdurCnf->getCdurPara.cause);

        /* 调用At_FormatResultData输出结果 */
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentHkAdcGetRsp(struct MsgCB *msg)
{
    VOS_UINT32             ret;
    VOS_UINT8              indexNum = 0;
    DRV_AGENT_HkadcGetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg         *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_HkadcGetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentTseLrfSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentTseLrfSetRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待HKADC电压查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TBAT_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    g_atSendDataBuff.bufLen = 0;
    if (event->result == DRV_AGENT_HKADC_GET_NO_ERROR) {
        /* 设置错误码为AT_OK */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:1,%d", g_parseContext[indexNum].cmdElement->cmdName,
                event->tbatHkadc);

        ret = AT_OK;

    } else {
        /* 查询失败返回ERROR字符串 */
        ret = AT_ERROR;
    }

    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentAppdmverQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                ret;
    VOS_UINT8                 indexNum = 0;
    DRV_AGENT_AppdmverQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg            *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_AppdmverQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentAppdmverQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentAppdmverQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待APPDMVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_APPDMVER_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_APPDMVER_QRY_NO_ERROR) {
        /* 设置错误码为AT_OK           构造结构为^APPDMVER:<pdmver>格式 */
        ret = AT_OK;

        /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
        event->pdmver[AT_MAX_PDM_VER_LEN] = '\0';
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, event->pdmver);

    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentDloadverQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                ret;
    VOS_UINT8                 indexNum = 0;
    DRV_AGENT_DloadverQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg            *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_DloadverQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentDloadverQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentDloadverQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待APPDMVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DLOADVER_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_DLOADVER_QRY_NO_ERROR) {
        /* 设置错误码为AT_OK           构造结构为<dloadver>格式 */
        ret = AT_OK;

        /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
        event->versionInfo[TAF_MAX_VER_INFO_LEN] = '\0';
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s", event->versionInfo);
    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentAuthVerQryRsp(struct MsgCB *msg)
{
    VOS_UINT32               ret;
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_AuthverQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_AuthverQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthVerQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthVerQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待AUTHVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AUTHVER_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_AUTHVER_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK           构造结构为<CR><LF>^ AUTHVER: <value> <CR><LF>
         * <CR><LF>OK<CR><LF>格式
         */
        ret = AT_OK;
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
                event->simLockVersion);

    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentFlashInfoQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                 ret;
    VOS_UINT8                  indexNum = 0;
    VOS_UINT16                 length;
    DRV_AGENT_FlashinfoQryCnf *event  = VOS_NULL_PTR;
    DRV_AGENT_Msg             *rcvMsg = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_FlashinfoQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentFlashInfoQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentFlashInfoQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待^FLASHINFO查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FLASHINFO_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_FLASHINFO_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK
         * 构造结构为<CR><LF>~~~~~~FLASH INFO~~~~~~:<CR><LF>
         *  <CR><LF>MMC BLOCK COUNT:<blockcount>,
         *       PAGE SIZE:<pagesize>,
         *       PAGE COUNT PER BLOCK:<blocksize><CR><LF>
         *  <CR><LF>OK<CR><LF>格式
         */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s", "~~~~~~FLASH INFO~~~~~~:", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "MMC BLOCK COUNT:%d, PAGE SIZE:%d, PAGE COUNT PER BLOCK:%d",
            event->flashInfo.blockCount, event->flashInfo.pageSize, event->flashInfo.pgCntPerBlk);

        ret = AT_OK;

    } else {
        /* 查询失败返回ERROR字符串 */
        length = 0;
        ret    = AT_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentDloadInfoQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                 ret;
    VOS_UINT8                  indexNum = 0;
    DRV_AGENT_DloadinfoQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg             *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_DloadinfoQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentDloadInfoQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentDloadInfoQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待DLOADINFO查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DLOADINFO_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_DLOADINFO_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK
         *   构造结构为^DLOADINFO:<CR><LF>
         * <CR><LF>swver:<software version><CR><LF>
         * <CR><LF>isover:<iso version><CR><LF>
         * <CR><LF>product name:<product name><CR><LF>
         * <CR><LF>product name:<WebUiVer><CR><LF>
         * <CR><LF>dload type: <dload type><CR><LF>
         * <CR><LF>OK<CR><LF>格式
         */
        ret                     = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s", event->dlodInfo);

    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentHwnatQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                ret;
    VOS_UINT8                 indexNum = 0;
    DRV_AGENT_HwnatqryQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg            *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_HwnatqryQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentHwnatQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentHwnatQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待HWNAT查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HWNATQRY_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_HWNATQRY_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK
         * 构造结构为^HWNATQRY: <cur_mode> 格式
         */
        ret                     = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, event->netMode);
    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentAuthorityVerQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                    ret;
    VOS_UINT8                     indexNum = 0;
    DRV_AGENT_AuthorityverQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg                *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_AuthorityverQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthorityVerQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthorityVerQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待AUTHORITYVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AUTHORITYVER_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 :  */
    if (event->result == DRV_AGENT_AUTHORITYVER_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK 格式为<CR><LF><Authority Version><CR><LF>
         * <CR><LF>OK<CR><LF>
         */
        /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
        event->authority[TAF_AUTHORITY_LEN] = '\0';
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s", event->authority);

        ret = AT_OK;
    } else {
        /* 查询失败返回ERROR字符串 */
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentAuthorityIdQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                   ret;
    VOS_UINT8                    indexNum = 0;
    DRV_AGENT_AuthorityidQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg               *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_AuthorityidQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthorityIdQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentAuthorityIdQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待AUTHORITYID查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AUTHORITYID_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 :  */
    if (event->result == DRV_AGENT_AUTHORITYID_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK 格式为<CR><LF><Authority ID>, <Authority Type><CR><LF>
         * <CR><LF>OK<CR><LF>
         */
        /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
        event->authorityId[TAF_AUTH_ID_LEN] = '\0';
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s", event->authorityId);

        ret = AT_OK;
    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvDrvAgentGodloadSetRsp(struct MsgCB *msg)
{
    VOS_UINT32               ret;
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_GodloadSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_GodloadSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentGodloadSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGodloadSetRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待GODLOAD查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GODLOAD_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    g_atSendDataBuff.bufLen = 0;
    if (event->result == DRV_AGENT_GODLOAD_SET_NO_ERROR) {
        /* 设置错误码为AT_OK */
        ret = AT_OK;
    } else {
        /* 设置失败返回ERROR字符串 */
        ret = AT_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}
#endif

VOS_UINT32 AT_RcvDrvAgentPfverQryRsp(struct MsgCB *msg)
{
    VOS_UINT32             ret;
    VOS_UINT8              indexNum = 0;
    DRV_AGENT_PfverQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg         *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_PfverQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentPfverQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentPfverQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待PFVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PFVER_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_PFVER_QRY_NO_ERROR) {
        /*
         * 设置错误码为AT_OK           构造结构为<CR><LF>^PFVER: <PfVer>,<VerTime> <CR><LF>
         * <CR><LF>OK<CR><LF>格式
         */
        ret                     = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:\"%s %s\"", g_parseContext[indexNum].cmdElement->cmdName,
            event->pfverInfo.pfVer, event->pfverInfo.verTime);

    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSdloadSetRsp(struct MsgCB *msg)
{
    VOS_UINT32              ret;
    VOS_UINT8               indexNum = 0;
    DRV_AGENT_SdloadSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg          *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_SdloadSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSdloadSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSdloadSetRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待SDLOAD查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SDLOAD_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    g_atSendDataBuff.bufLen = 0;
    if (event->result == DRV_AGENT_SDLOAD_SET_NO_ERROR) {
        /* 设置错误码为AT_OK */
        ret = AT_OK;
    } else {
        /* 设置失败返回ERROR字符串 */
        ret = AT_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentCpuloadQryRsp(struct MsgCB *msg)
{
    VOS_UINT32               i;
    VOS_UINT16               length;
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_CpuloadQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    length = 0;
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_CpuloadQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentCpuloadQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentCpuloadQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待CPULOAD查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPULOAD_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    g_atSendDataBuff.bufLen = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    event->curCpuLoadCnt = TAF_MIN(event->curCpuLoadCnt, TAF_MAX_CPU_CNT);

    for (i = 0; i < event->curCpuLoadCnt; i++) {
        /* 100用来计算每个核的负载占用百分比的整数部分和小数部分 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%u.%02u", event->curCpuLoad[i] / 100,
            event->curCpuLoad[i] % 100);

        if ((event->curCpuLoadCnt - 1) == i) {
            break;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    /* 调用At_FormatResultData输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentMfreelocksizeQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                     ret;
    VOS_UINT8                      indexNum = 0;
    DRV_AGENT_MfreelocksizeQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg                 *rcvMsg   = VOS_NULL_PTR;
    VOS_UINT32                     aCoreMemfreeSize;

    /* 初始化 */
    rcvMsg           = (DRV_AGENT_Msg *)msg;
    event            = (DRV_AGENT_MfreelocksizeQryCnf *)rcvMsg->content;
    aCoreMemfreeSize = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentMfreelocksizeQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentMfreelocksizeQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待MFREELOCKSIZE查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MFREELOCKSIZE_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    g_atSendDataBuff.bufLen = 0;
    if (event->result == DRV_AGENT_MFREELOCKSIZE_QRY_NO_ERROR) {
        /* 获取A核的剩余系统内存 */
        aCoreMemfreeSize = FREE_MEM_SIZE_GET();

        /* 由于底软返回的是KB，转为字节 */
        aCoreMemfreeSize *= AT_KB_TO_BYTES_NUM;

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            event->maxFreeLockSize, aCoreMemfreeSize);

        /* 设置错误码为AT_OK */
        ret = AT_OK;
    } else {
        /* 设置失败返回ERROR字符串 */
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentImsiChgQryRsp(struct MsgCB *msg)
{
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_ImsichgQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_ImsichgQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentImsiChgQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentImsiChgQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待IMSICHG查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSICHG_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "^IMSICHG: %d,%d", event->dualIMSIEnable, event->curImsiSign);

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentInfoRbuSetRsp(struct MsgCB *msg)
{
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_InforbuSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_InforbuSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentInfoRbuSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentInfoRbuSetRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待INFORBU设置命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_INFORBU_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    if (event->rslt == NV_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 AT_RcvDrvAgentInfoRrsSetRsp(struct MsgCB *msg)
{
    VOS_UINT8                indexNum = 0;
    DRV_AGENT_InforrsSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_InforrsSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        return VOS_ERR;
    }

    /* AT模块在等待INFORRU设置命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_INFORRS_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置操作结果 :  */
    if (event->rslt == NV_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvDrvAgentCpnnQryRsp(struct MsgCB *msg)
{
    VOS_UINT8             indexNum = 0;
    DRV_AGENT_CpnnQryCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg        *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_CpnnQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentCpnnQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentCpnnQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待CPNN查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPNN_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->normalSrvStatus == VOS_TRUE) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentCpnnTestRsp(struct MsgCB *msg)
{
    VOS_UINT8              indexNum = 0;
    VOS_INT                bufLen;
    DRV_AGENT_CpnnTestCnf *event  = VOS_NULL_PTR;
    DRV_AGENT_Msg         *rcvMsg = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_CpnnTestCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentCpnnTestRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentCpnnTestRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待CPNN测试命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPNN_TEST) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((event->oplExistFlg == PS_USIM_SERVICE_AVAILIABLE) && (event->pnnExistFlg == PS_USIM_SERVICE_AVAILIABLE) &&
        (event->normalSrvStatus == VOS_TRUE)) {
        bufLen = snprintf_s((VOS_CHAR *)g_atSndCodeAddress,
                            AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET,
                            (AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET) - 1,
                            "%s:(0,1)", g_parseContext[indexNum].cmdElement->cmdName);
        if (bufLen < 0) {
            At_FormatResultData(indexNum, AT_ERROR);
        } else {
            g_atSendDataBuff.bufLen = (VOS_UINT16)bufLen;
            At_FormatResultData(indexNum, AT_OK);
        }
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentNvBackupSetRsp(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum = 0;
    DRV_AGENT_NvbackupSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg            *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvbackupSetCnf *)rcvMsg->content;

    AT_PR_LOGI("Rcv Msg");

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentNvBackupSetRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentNvBackupSetRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待NVBACKUP测试命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVBACKUP_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->rslt == NV_OK) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%d", event->rslt);
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentMemInfoQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg            = VOS_NULL_PTR;
    VOS_UINT8                indexNum          = 0;
    DRV_AGENT_MeminfoQryRsp *cCpuMemInfoCnfMsg = VOS_NULL_PTR;
    AT_PID_MemInfoPara      *pidMemInfo        = VOS_NULL_PTR;
    VOS_UINT32 aCpuMemBufSize;
    VOS_UINT32 aCpuPidTotal;
    VOS_UINT16 atLength;
    VOS_UINT32 i;

    /* 初始化 */
    rcvMsg            = (DRV_AGENT_Msg *)msg;
    cCpuMemInfoCnfMsg = (DRV_AGENT_MeminfoQryRsp *)rcvMsg->content;

    /* 指向CCPU的每个PID的内存信息 */
    pidMemInfo = (AT_PID_MemInfoPara *)cCpuMemInfoCnfMsg->data;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cCpuMemInfoCnfMsg->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvDrvAgentMemInfoQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* Client Id 为广播也返回ERROR */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_ERR_LOG("AT_RcvDrvAgentMemInfoQryRsp: AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* AT模块在等待MEMINFO查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MEMINFO_READ) {
        AT_ERR_LOG("AT_RcvDrvAgentMemInfoQryRsp: CmdCurrentOpt Error!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 对返回结果中的参数进行检查 */
    if ((cCpuMemInfoCnfMsg->result != VOS_OK) ||
        ((cCpuMemInfoCnfMsg->memQryType != AT_MEMQUERY_TTF) && (cCpuMemInfoCnfMsg->memQryType != AT_MEMQUERY_VOS))) {
        /* 调用At_FormatResultData返回ERROR字符串 */
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    /* 先打印命令名称 */
    atLength = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, g_atCrLf);

    atLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + atLength, "C CPU Pid:%d%s", cCpuMemInfoCnfMsg->pidNum, g_atCrLf);

    /* 依次打印C CPU每个PID的内存信息 */
    for (i = 0; i < cCpuMemInfoCnfMsg->pidNum; i++) {
        atLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + atLength, "%d,%d,%d%s", pidMemInfo[i].pid, pidMemInfo[i].msgPeakSize,
            pidMemInfo[i].memPeakSize, g_atCrLf);
    }

    /* fusion版本VOS接口无法查询内存 */
    /*
     * 如果是查询VOS内存，则获取并打印A CPU的VOS内存使用情况.
     * 由于暂无A CPU的TTF内存查询接口，TTF类型查询只打印C CPU的TTF内存使用情况
     */
    if (cCpuMemInfoCnfMsg->memQryType == AT_MEMQUERY_VOS) {
        aCpuMemBufSize = AT_PID_MEM_INFO_LEN * sizeof(AT_PID_MemInfoPara);

        /* 分配内存以查询A CPU的VOS内存使用信息 */
        /*lint -save -e516 */
        pidMemInfo = (AT_PID_MemInfoPara *)PS_MEM_ALLOC(WUEPS_PID_AT, aCpuMemBufSize);
        /*lint -restore */
        if (pidMemInfo != VOS_NULL_PTR) {
            (VOS_VOID)memset_s(pidMemInfo, aCpuMemBufSize, 0x00, aCpuMemBufSize);

            aCpuPidTotal = 0;

            if (VOS_AnalyzePidMemory(pidMemInfo, aCpuMemBufSize, &aCpuPidTotal) != VOS_ERR) {
                /* 依次打印 A CPU的每个PID的内存使用情况 */
                atLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + atLength, "A CPU Pid:%d%s", aCpuPidTotal, g_atCrLf);

                for (i = 0; i < aCpuPidTotal; i++) {
                    atLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + atLength, "%d,%d,%d%s", pidMemInfo[i].pid,
                        pidMemInfo[i].msgPeakSize, pidMemInfo[i].memPeakSize, g_atCrLf);
                }
            }

            /* 释放内存 */
            /*lint -save -e516 */
            PS_MEM_FREE(WUEPS_PID_AT, pidMemInfo);
            pidMemInfo = VOS_NULL_PTR;
            /*lint -restore */
        }
    }
    g_atSendDataBuff.bufLen = atLength;

    /* 调用At_FormatResultData返回OK字符串 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


TAF_VOID At_QryParaRspProc(TAF_UINT8 indexNum, TAF_UINT8 opId, TAF_PARA_TYPE queryType, TAF_UINT16 errorCode,
                           TAF_UINT8 *para)
{
    TAF_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;
    TAF_UINT32 tmp;
    TAF_UINT32 i;

    if (errorCode != 0) { /* 错误 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        result                  = At_ChgTafErrorCode(indexNum, errorCode); /* 发生错误 */
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
        return;
    }

    if (para == TAF_NULL_PTR) { /* 如果查询出错 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        At_FormatResultData(indexNum, AT_CME_UNKNOWN);

        return;
    }

    tmp = (sizeof(g_atQryTypeProcFuncTbl) / sizeof(g_atQryTypeProcFuncTbl[0]));
    for (i = 0; i != tmp; i++) {
        if (queryType == g_atQryTypeProcFuncTbl[i].queryType) {
            if (queryType != TAF_PH_ICC_ID) {
                AT_STOP_TIMER_CMD_READY(indexNum);
            }

            g_atQryTypeProcFuncTbl[i].atQryParaProcFunc(indexNum, opId, para);

            return;
        }
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    AT_WARN_LOG("At_QryParaRspProc QueryType FAILURE");
    return;
}

TAF_VOID At_QryMsgProc(TAF_QryRslt *qryRslt)
{
    TAF_UINT8     indexNum;

    AT_LOG1("At_QryMsgProc ClientId", qryRslt->clientId);
    AT_LOG1("At_QryMsgProc QueryType", qryRslt->queryType);
    AT_LOG1("At_QryMagProc usErrorCode", qryRslt->errorCode);
    if (qryRslt->clientId == AT_BUTT_CLIENT_ID) {
        AT_WARN_LOG("At_QryMsgProc Error ucIndex");
        return;
    }

    indexNum = 0;
    if (At_ClientIdToUserId(qryRslt->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_QryMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_QryMsgProc: AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_QryMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    At_QryParaRspProc(indexNum, qryRslt->opId, qryRslt->queryType, qryRslt->errorCode, qryRslt->para);
}


TAF_UINT32 At_PIHNotBroadIndProc(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_PIHNotBroadIndProc: Get modem id fail.");
        return VOS_ERR;
    }

    switch (event->eventType) {
        case SI_PIH_EVENT_PRIVATECGLA_SET_IND:

            /* ^CGLA查询请求下发后会有多条IND上报，通过当前通道上报不需要广播 */
            length += At_PrintPrivateCglaResult(indexNum, event);
            break;

        default:
            AT_WARN_LOG("At_PIHNotBroadIndProc: Abnormal EventType.");
            return VOS_ERR;
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


TAF_VOID At_PIHIndProc(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_PIHIndProc: Get modem id fail.");
        return;
    }

    switch (event->eventType) {
        case SI_PIH_EVENT_HVRDH_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^HVRDH: %d%s", g_atCrLf, event->pihEvent.hvrdhInd.reDhFlag,
                g_atCrLf);
            break;

        case SI_PIH_EVENT_TEETIMEOUT_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^TEETIMEOUT: %d%s", g_atCrLf,
                event->pihEvent.teeTimeOut.data, g_atCrLf);
            break;

        case SI_PIH_EVENT_SIM_ERROR_IND:
            length += At_CardErrorInfoInd(indexNum, event);
            break;

        case SI_PIH_EVENT_SIM_ICCID_IND:
            length += At_CardIccidInfoInd(indexNum, event);
            break;
        case SI_PIH_EVENT_SIM_HOTPLUG_IND:
            length += At_SimHotPlugStatusInd(indexNum, event);
            break;

        case SI_PIH_EVENT_SW_CHECK_IND:
            length += At_SWCheckStatusInd(event);
            break;

        case SI_PIH_EVENT_CARDSTATUS_IND:
            length += At_CardStatusInd(indexNum, event);
            break;

        case SI_PIH_EVENT_UPDATE_FILE_AT_IND:
            length += At_UpdateFileAtInd(event);
            break;

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
        case SI_PIH_EVENT_SIM_REFRESH_IND:
            length += AT_SimRefreshInd(event);
            break;
#endif
#endif

        default:
            AT_WARN_LOG("At_PIHIndProc: Abnormal EventType.");
            return;
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))

VOS_UINT32 At_PrintSilentPinInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    VOS_UINT8 *pinIv = VOS_NULL_PTR;
    VOS_UINT32 i;

    /* 判断当前操作类型是否为AT_CMD_SILENTPIN_SET/AT_CMD_SILENTPININFO_SET */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SILENTPIN_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SILENTPININFO_SET)) {
        AT_WARN_LOG("At_PrintSilentPinInfo : CmdCurrentOpt is not AT_CMD_SILENTPIN_SET/AT_CMD_SILENTPININFO_SET!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    for (i = 0; i < DRV_AGENT_PIN_CRYPTO_DATA_LEN; i++) {
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%02X", event->pihEvent.cryptoPin.cryptoPin[i]);
    }

    pinIv = (VOS_UINT8 *)event->pihEvent.cryptoPin.pinIv;

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), ",");

    for (i = 0; i < SI_CRYPTO_CBC_PIN_LEN; i++) {
        /* IV长度为16字节 */
        (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + (*length), "%02X", (*(pinIv + i)));
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), ",");

    for (i = 0; i < DRV_AGENT_HMAC_DATA_LEN; i++) {
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%02X", event->pihEvent.cryptoPin.hmacValue[i]);
    }

    return AT_OK;
}
#endif


VOS_UINT32 At_PrintSetEsimSwitchInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CmdEsimSwitch_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMSWITCH_SET) {
        AT_WARN_LOG("At_PrintSetEsimSwitchInfo : CmdCurrentOpt is not AT_CmdEsimSwitch_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_PrintQryEsimSwitchInfo(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_ESIMSWITCH_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMSWITCH_QRY) {
        AT_WARN_LOG("At_PrintQryEsimSwitchInfo : CmdCurrentOpt is not AT_CMD_ESIMSWITCH_QRY!");
        return AT_ERROR;
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ESIMSWITCH: %d,%d%s", g_atCrLf, event->pihEvent.slotCardType.slot0CardType,
        event->pihEvent.slotCardType.slot1CardType, g_atCrLf);

    return AT_OK;
}


TAF_VOID At_PIHRspProc(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    TAF_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;
    TAF_UINT32 tmp;
    TAF_UINT32 i;

    if (event->pihError != TAF_ERR_NO_ERROR) { /* 错误 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        result = At_ChgTafErrorCode(indexNum, (TAF_UINT16)(event->pihError)); /* 发生错误 */

        g_atSendDataBuff.bufLen = length;

        At_FormatResultData(indexNum, result);

        return;
    }

    tmp = (sizeof(g_atPihRspProcFuncTbl) / sizeof(g_atPihRspProcFuncTbl[0]));

    for (i = 0; i < tmp; i++) {
        /* 找到处理函数，进行输出相关处理 */
        if (event->eventType == g_atPihRspProcFuncTbl[i].eventType) {
            result = g_atPihRspProcFuncTbl[i].atPihRspProcFunc(indexNum, event, &length);

            if (result == AT_ERROR) {
                AT_WARN_LOG("At_PIHRspProc : pAtPihRspProcFunc is return error!");
                return;
            }

            break;
        }
    }

    /* 没找到处理函数，直接返回 */
    if (i == tmp) {
        AT_WARN_LOG("At_PIHRspProc : no find AT Proc Func!");
        return;
    }

    result = AT_OK;

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

    return;
}

#if ((FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF))
TAF_VOID At_PbReadyIndProc(TAF_UINT8 indexNum)
{
    TAF_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CPBREADY%s", g_atCrLf, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}
#endif

/*
 * Description: 电话簿管理上报消息处理函数
 *  1.Date: 2013-05-29
 *    Modification: Created function
 */
TAF_VOID At_PbIndMsgProc(TAF_UINT8 indexNum, SI_PB_EventInfo *event)
{
    if (event->pbEventType == SI_PB_EVENT_INFO_IND) {
        /* 当SIM卡且FDN使能时默认存储器是FD */
        if ((event->pbEvent.pbInfoInd.cardType == 0) &&
            (event->pbEvent.pbInfoInd.fdnState == SI_PIH_STATE_FDN_BDN_ENABLE)) {
            g_pbatInfo.nameMaxLen = event->pbEvent.pbInfoInd.fdnTextLen;
            g_pbatInfo.numMaxLen  = event->pbEvent.pbInfoInd.fdnNumberLen;
            g_pbatInfo.total      = event->pbEvent.pbInfoInd.fdnRecordNum;
            g_pbatInfo.anrNumLen  = event->pbEvent.pbInfoInd.anrNumberLen;
            g_pbatInfo.emailLen   = event->pbEvent.pbInfoInd.emailTextLen;
        } else {
            g_pbatInfo.nameMaxLen = event->pbEvent.pbInfoInd.adnTextLen;
            g_pbatInfo.numMaxLen  = event->pbEvent.pbInfoInd.adnNumberLen;
            g_pbatInfo.total      = event->pbEvent.pbInfoInd.adnRecordNum;
            g_pbatInfo.anrNumLen  = event->pbEvent.pbInfoInd.anrNumberLen;
            g_pbatInfo.emailLen   = event->pbEvent.pbInfoInd.emailTextLen;
        }

#if ((FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF))
        At_PbReadyIndProc(indexNum);
#endif
    }

    return;
}


VOS_VOID AT_PB_ReadContinueProc(AT_ClientIdUint16 indexNum)
{
    AT_CommPbCtx *commPbCntxt = VOS_NULL_PTR;
    AT_UartCtx   *uartCtx     = VOS_NULL_PTR;
    VOS_UINT32    result;

    commPbCntxt = AT_GetCommPbCtxAddr();
    uartCtx     = AT_GetUartCtxAddr();
    result      = AT_SUCCESS;

    /* 清除发送缓存低水线回调 */
    uartCtx->wmLowFunc = VOS_NULL_PTR;

    /* 更新当前读取的电话本索引 */
    commPbCntxt->currIdx++;

    if (SI_PB_Read(g_atClientTab[indexNum].clientId, 0, SI_PB_STORAGE_UNSPECIFIED, commPbCntxt->currIdx,
                   commPbCntxt->currIdx) == TAF_SUCCESS) {
        return;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData((VOS_UINT8)indexNum, result);
    return;
}


VOS_VOID AT_PB_ReadRspProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event)
{
    AT_CommPbCtx *commPbCntxt = VOS_NULL_PTR;
    AT_UartCtx   *uartCtx     = VOS_NULL_PTR;
    VOS_UINT32    result;

    commPbCntxt = AT_GetCommPbCtxAddr();
    uartCtx     = AT_GetUartCtxAddr();
    result      = AT_SUCCESS;

    /*
     * 读取单条: 按照错误码返回结果
     * 读取多条: 跳过未找到的电话本
     */
    if (commPbCntxt->singleReadFlg == VOS_TRUE) {
        result = (event->pbError == TAF_ERR_NO_ERROR) ? AT_OK :
                                                        At_ChgTafErrorCode(indexNum, (VOS_UINT16)event->pbError);
    } else if ((event->pbError == TAF_ERR_NO_ERROR) || (event->pbError == TAF_ERR_PB_NOT_FOUND)) {
        /* 检查当前读取位置是否已经到达最后一条 */
        if (commPbCntxt->currIdx == commPbCntxt->lastIdx) {
            result = AT_OK;
        } else {
            result = AT_WAIT_ASYNC_RETURN;
        }
    } else {
        result = At_ChgTafErrorCode(indexNum, (VOS_UINT16)event->pbError);
    }

    /* 电话本未读完, 读取下一条电话本 */
    if (result == AT_WAIT_ASYNC_RETURN) {
        /*
         * 如果发送缓存已经到达高水线:
         * 注册低水线回调, 待发送缓存到达低水线后继续读取下一条电话本
         */
        if (DMS_PORT_GetUartWaterMarkFlg() == VOS_TRUE) {
            uartCtx->wmLowFunc = AT_PB_ReadContinueProc;
            return;
        }

        /* 更新当前读取的电话本索引 */
        commPbCntxt->currIdx++;

        if (SI_PB_Read(g_atClientTab[indexNum].clientId, 0, SI_PB_STORAGE_UNSPECIFIED, commPbCntxt->currIdx,
                       commPbCntxt->currIdx) == TAF_SUCCESS) {
            return;
        } else {
            result = AT_ERROR;
        }
    }

    g_atSendDataBuff.bufLen = 0;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return;
}


TAF_VOID At_PbRspProc(TAF_UINT8 indexNum, SI_PB_EventInfo *event)
{
    errno_t    memResult;
    VOS_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;

    if (!((AT_CheckHsUartUser(indexNum) == VOS_TRUE) && ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR2_SET) ||
                                                         (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR_SET)))) {
        if (event->pbError != TAF_ERR_NO_ERROR) { /* 错误 */
            result                  = At_ChgTafErrorCode(indexNum, (TAF_UINT16)event->pbError);
            g_atSendDataBuff.bufLen = length;
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, result);
            return;
        }
    }

    switch (event->pbEventType) {
        case SI_PB_EVENT_SET_CNF:
            g_pbatInfo.nameMaxLen = event->pbEvent.pbSetCnf.textLen;
            g_pbatInfo.numMaxLen  = event->pbEvent.pbSetCnf.numLen;
            g_pbatInfo.total      = event->pbEvent.pbSetCnf.totalNum;
            g_pbatInfo.used       = event->pbEvent.pbSetCnf.inUsedNum;
            g_pbatInfo.anrNumLen  = event->pbEvent.pbSetCnf.anrNumberLen;
            g_pbatInfo.emailLen   = event->pbEvent.pbSetCnf.emailTextLen;

            AT_STOP_TIMER_CMD_READY(indexNum);

            result = AT_OK;

            break;

        case SI_PB_EVENT_READ_CNF:
        case SI_PB_EVENT_SREAD_CNF:
            if ((event->pbError == TAF_ERR_NO_ERROR) &&
                (event->pbEvent.pbReadCnf.pbRecord.validFlag == SI_PB_CONTENT_VALID)) { /* 当前的内容有效 */
                if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_GET_NUMBER_BEFORE_CALL) {
                    result = At_DialNumByIndexFromPb(indexNum, event);
                    if (result == AT_WAIT_ASYNC_RETURN) {
                        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

                        /* 开定时器 */
                        if (At_StartTimer(g_parseContext[indexNum].cmdElement->setTimeOut, indexNum) != AT_SUCCESS) {
                            AT_ERR_LOG("At_PbRspProc:ERROR:Start Timer");
                        }
                        result = AT_SUCCESS;

                    } else {
                        result = AT_ERROR;
                    }

                    break;
                }

                if (g_pbPrintTag == TAF_FALSE) {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                        (TAF_CHAR *)g_atSndCrLfAddr, "%s", "\r\n");
                }

                g_pbPrintTag = TAF_TRUE;

                if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR_SET) { /* 按照 ^CPBR 的方式进行打印 */
                    result = At_PbCPBRCmdPrint(indexNum, &length, g_atSndCrLfAddr, event);
                } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR2_SET) { /* 按照 +CPBR 的方式进行打印 */
                    result = At_PbCPBR2CmdPrint(indexNum, &length, g_atSndCrLfAddr, event);
                } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SCPBR_SET) { /* 按照 ^SCPBR 的方式进行打印 */
                    result = At_PbSCPBRCmdPrint(indexNum, &length, event);
                } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CNUM_READ) { /* 按照 CNUM 的方式进行打印 */
                    result = At_PbCNUMCmdPrint(indexNum, &length, g_atSndCrLfAddr, event);
                } else {
                    AT_ERR_LOG1("At_PbRspProc: the Cmd Current Opt %d is Unknow",
                                g_atClientTab[indexNum].cmdCurrentOpt);

                    return;
                }

                if (result == AT_SUCCESS) {
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                        (TAF_CHAR *)g_atSndCrLfAddr + length, "%s", "\r\n");
                }

                At_SendResultData(indexNum, g_atSndCrLfAddr, length);

                length = 0;
            }

            memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr, AT_SEND_CRLF_ADDR_MAX_LEN, (TAF_CHAR *)g_atCrLf, AT_CRLF_STR_LEN);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN, AT_CRLF_STR_LEN);

            if ((AT_CheckHsUartUser(indexNum) == VOS_TRUE) &&
                ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR2_SET) ||
                 (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR_SET))) {
                AT_PB_ReadRspProc(indexNum, event);
                return;
            }

            if ((g_pbPrintTag == VOS_FALSE) && ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR_SET) ||
                                                (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR2_SET))) {
                event->pbError = TAF_ERR_ERROR;
            }

            if (event->pbError != TAF_ERR_NO_ERROR) {                              /* 错误 */
                result = At_ChgTafErrorCode(indexNum, (TAF_UINT16)event->pbError); /* 发生错误 */

                g_atSendDataBuff.bufLen = length;
            } else {
                result = AT_OK;
            }

            AT_STOP_TIMER_CMD_READY(indexNum);
            break;

        case SI_PB_EVENT_SEARCH_CNF:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", "\r");
            AT_STOP_TIMER_CMD_READY(indexNum);
            result = AT_OK;
            break;

        case SI_PB_EVENT_ADD_CNF:
        case SI_PB_EVENT_SADD_CNF:
        case SI_PB_EVENT_MODIFY_CNF:
        case SI_PB_EVENT_SMODIFY_CNF:
        case SI_PB_EVENT_DELETE_CNF:
            AT_STOP_TIMER_CMD_READY(indexNum);
            result = AT_OK;
            break;

        case SI_PB_EVENT_QUERY_CNF:

            if (event->opId == 1) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%s: (\"SM\",\"EN\",\"ON\",\"FD\")",
                    g_parseContext[indexNum].cmdElement->cmdName);
                result = AT_OK;
                AT_STOP_TIMER_CMD_READY(indexNum);
                break;
            }

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            g_pbatInfo.nameMaxLen = event->pbEvent.pbQueryCnf.textLen;
            g_pbatInfo.numMaxLen  = event->pbEvent.pbQueryCnf.numLen;
            g_pbatInfo.total      = event->pbEvent.pbQueryCnf.totalNum;
            g_pbatInfo.used       = event->pbEvent.pbQueryCnf.inUsedNum;
            g_pbatInfo.anrNumLen  = event->pbEvent.pbQueryCnf.anrNumberLen;
            g_pbatInfo.emailLen   = event->pbEvent.pbQueryCnf.emailTextLen;

            switch (g_parseContext[indexNum].cmdElement->cmdIndex) {
                case AT_CMD_CPBR:
                case AT_CMD_CPBR2:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "(1-%d),%d,%d", g_pbatInfo.total,
                        g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen);
                    break;
                case AT_CMD_CPBW:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "(1-%d),%d,(128-255),%d", g_pbatInfo.total,
                        g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen);
                    break;
                case AT_CMD_CPBW2:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "(1-%d),%d,(128-255),%d", g_pbatInfo.total,
                        g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen);
                    break;
                case AT_CMD_SCPBR:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "(1-%d),%d,%d,%d", g_pbatInfo.total,
                        g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen, g_pbatInfo.emailLen);
                    break;
                case AT_CMD_SCPBW:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "(1-%d),%d,(128-255),%d,%d", g_pbatInfo.total,
                        g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen, g_pbatInfo.emailLen);
                    break;
                case AT_CMD_CPBS:
                    if (event->storage == SI_PB_STORAGE_SM) {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_SM].text);
                    } else if (event->storage == SI_PB_STORAGE_FD) {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_FD].text);
                    } else if (event->storage == SI_PB_STORAGE_ON) {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_ON].text);
                    } else if (event->storage == SI_PB_STORAGE_BD) {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_BD].text);
                    } else {
                        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_EN].text);
                    }

                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", event->pbEvent.pbQueryCnf.inUsedNum);
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", event->pbEvent.pbQueryCnf.totalNum);

                    break;
                case AT_CMD_CPBF:
                    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d", g_pbatInfo.numMaxLen, g_pbatInfo.nameMaxLen);
                    break;

                default:
                    break;
            }

            result = AT_OK;

            AT_STOP_TIMER_CMD_READY(indexNum);

            break;

        default:
            AT_ERR_LOG1("At_PbRspProc Unknow Event %d", event->pbEventType);
            break;
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);
}


TAF_VOID At_TAFPbMsgProc(TAF_UINT8 *data, TAF_UINT16 len)
{
    SI_PB_EventInfo *event = TAF_NULL_PTR;
    errno_t          memResult;
    TAF_UINT8        indexNum = 0;

    if (len == 0) {
        return;
    }

    /*lint -save -e516 */
    event = (SI_PB_EventInfo *)PS_MEM_ALLOC(WUEPS_PID_AT, len);
    /*lint -restore */
    if (event == TAF_NULL_PTR) {
        return;
    }
    (VOS_VOID)memset_s(event, len, 0x00, len);

    memResult = memcpy_s(event, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    AT_LOG1("At_PbMsgProc pEvent->ClientId", event->clientId);
    AT_LOG1("At_PbMsgProc PBEventType", event->pbEventType);
    AT_LOG1("At_PbMsgProc Event Error", event->pbError);

    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, event);
        /*lint -restore */
        AT_WARN_LOG("At_TAFPbMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, event);
        /*lint -restore */
        AT_WARN_LOG("At_TAFPbMsgProc: AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_PbMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    At_PbRspProc(indexNum, event);
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, event);
    /*lint -restore */
    return;
}


TAF_VOID At_PbMsgProc(struct MsgCB *msg)
{
    MN_APP_PbAtCnf *msgTemp  = VOS_NULL_PTR;
    TAF_UINT8       indexNum = 0;

    if (AT_ChkPbMsgLen((MSG_Header *)msg) != VOS_TRUE) {
        AT_ERR_LOG("At_PbMsgProc: message length is invalid!");
        return;
    }

    msgTemp = (MN_APP_PbAtCnf *)msg;

    AT_LOG1("At_PbMsgProc pEvent->ClientId", msgTemp->pbAtEvent.clientId);
    AT_LOG1("At_PbMsgProc PBEventType", msgTemp->pbAtEvent.pbEventType);
    AT_LOG1("At_PbMsgProc Event Error", msgTemp->pbAtEvent.pbError);

    if (At_ClientIdToUserId(msgTemp->pbAtEvent.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG1("At_PbMsgProc At_ClientIdToUserId FAILURE", msgTemp->pbAtEvent.clientId);
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_PbMsgProc: AT_BROADCAST_INDEX.");
        At_PbIndMsgProc(indexNum, &msgTemp->pbAtEvent);
        return;
    }

    AT_LOG1("At_PbMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    At_PbRspProc(indexNum, &msgTemp->pbAtEvent);

    return;
}


VOS_UINT32 AT_ChkPIHMsgLen(struct MsgCB *msg)
{
    MN_APP_PihAtCnf  *msgTemp  = VOS_NULL_PTR;
    SI_PIH_EventInfo *event = VOS_NULL_PTR;
    TAF_UINT32        size;
    TAF_UINT32        i;

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < AT_GET_PIH_AT_CNF_LEN_WITHOUT_UNION) {
        AT_WARN_LOG("AT_ChkPIHMsgLen: message length is invalid!");
    }
    msgTemp = (MN_APP_PihAtCnf *)msg;
    event = &msgTemp->pihAtEvent;

    size = (sizeof(g_atPihRspProcFuncTbl) / sizeof(g_atPihRspProcFuncTbl[0]));

    for (i = 0; i < size; i++) {
        if (event->eventType == g_atPihRspProcFuncTbl[i].eventType) {
#if (VOS_OS_VER == VOS_WIN32)
            if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) == sizeof(MN_APP_PihAtCnf)) {
                return VOS_TRUE;
            }
#endif
            if (TAF_RunChkMsgLenFunc((const MSG_Header *)msg,
                    (AT_GET_PIH_AT_CNF_LEN_WITHOUT_UNION + g_atPihRspProcFuncTbl[i].fixdedPartLen),
                    g_atPihRspProcFuncTbl[i].chkFunc) != VOS_TRUE) {
                AT_WARN_LOG("AT_ChkPIHMsgLen: message length is invalid!");
                return VOS_FALSE;
            }
            break;
        }
    }
    return VOS_TRUE;
}


TAF_VOID At_PIHMsgProc(struct MsgCB *msg)
{
    MN_APP_PihAtCnf *msgTemp  = VOS_NULL_PTR;
    TAF_UINT8        indexNum = 0;

    if (AT_ChkPIHMsgLen(msg) != VOS_TRUE) {
        AT_ERR_LOG("At_PIHMsgProc: message length is invalid!");
        return;
    }

    msgTemp = (MN_APP_PihAtCnf *)msg;

    if (msgTemp->msgId != PIH_AT_EVENT_CNF) {
        AT_ERR_LOG1("At_PIHMsgProc: The Msg Id is Wrong", msgTemp->msgId);
        return;
    }

    AT_LOG1("At_PIHMsgProc pEvent->ClientId", msgTemp->pihAtEvent.clientId);
    AT_LOG1("At_PIHMsgProc EventType", msgTemp->pihAtEvent.eventType);
    AT_LOG1("At_PIHMsgProc SIM Event Error", msgTemp->pihAtEvent.pihError);

    if (At_ClientIdToUserId(msgTemp->pihAtEvent.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("At_PIHMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_PIHIndProc(indexNum, &msgTemp->pihAtEvent);
        AT_WARN_LOG("At_PIHMsgProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_PIHMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    /* 非广播的主动上报 */
    if (At_PIHNotBroadIndProc(indexNum, &msgTemp->pihAtEvent) == VOS_OK) {
        return;
    }

    At_PIHRspProc(indexNum, &msgTemp->pihAtEvent);

    return;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_VOID At_XsmsIndProc(VOS_UINT8 indexNum, TAF_XSMS_AppMsgTypeUint32 eventType, TAF_XSMS_AppAtEventInfo *event)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    length  = 0;
    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_XsmsIndProc: Get modem id fail.");
        return;
    }

    switch (eventType) {
        case TAF_XSMS_APP_MSG_TYPE_SEND_SUCC_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^CCMGSS: %d%s", g_atCrLf, event->xSmsEvent.sndSuccInd.mr,
                g_atCrLf);
            break;

        case TAF_XSMS_APP_MSG_TYPE_SEND_FAIL_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^CCMGSF: %d%s", g_atCrLf,
                event->xSmsEvent.sndFailInd.courseCode, g_atCrLf);
            break;

        case TAF_XSMS_APP_MSG_TYPE_RCV_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^CCMT:", g_atCrLf);
            /* <length>, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"",
                AT_ASCII_AND_HEX_CONVERSION_FACTOR * sizeof(TAF_XSMS_Message)); /*lint !e559 */
            /* <PDU> */
            length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN,
                (TAF_INT8 *)g_atSndCodeAddress, (TAF_UINT8 *)g_atSndCodeAddress + length,
                (TAF_UINT8 *)&event->xSmsEvent.rcvInd.rcvMsg, sizeof(TAF_XSMS_Message));

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

            break;

        case TAF_XSMS_APP_MSG_TYPE_UIM_FULL_IND:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s^CSMMEMFULL: \"SM\"%s", g_atCrLf, g_atCrLf);
            break;

        default:
            AT_WARN_LOG("At_XsmsIndProc: Abnormal EventType.");
            return;
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_VOID At_XsmsCnfProc(VOS_UINT8 indexNum, TAF_XSMS_AppMsgTypeUint32 eventType, TAF_XSMS_AppAtEventInfo *event)
{
    TAF_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;

    if (event->error != TAF_ERR_NO_ERROR) { /* 错误 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        result = At_ChgXsmsErrorCodeToAt(event->error); /* 发生错误 */

        g_atSendDataBuff.bufLen = length;

        At_FormatResultData(indexNum, result);

        return;
    }

    switch (eventType) {
        /* 什么都不做，就等打印OK */
        case TAF_XSMS_APP_MSG_TYPE_SEND_CNF:
            break;

        case TAF_XSMS_APP_MSG_TYPE_WRITE_CNF:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "^CCMGW: %d", event->xSmsEvent.writeCnf.indexNum - 1);
            break;

        /* 什么都不做，就等打印OK */
        case TAF_XSMS_APP_MSG_TYPE_DELETE_CNF:

            break;

        /* 什么都不做，就等打印OK */
        case TAF_XSMS_APP_MSG_TYPE_UIM_MEM_FULL_CNF:

            break;

        default:
            return;
    }

    result = AT_OK;

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);
}


VOS_VOID AT_ProcXsmsMsg(struct MsgCB *msg)
{
    TAF_XSMS_AppAtCnf *msgTemp  = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;

    msgTemp = (TAF_XSMS_AppAtCnf *)msg;
    /* 消息类型不正确 */
    if (msgTemp->eventType >= TAF_XSMS_APP_MSG_TYPE_BUTT) {
        AT_ERR_LOG1("AT_ProcXsmsMsg: The Msg Id is Wrong", msgTemp->eventType);

        return;
    }

    AT_LOG1("AT_ProcXsmsMsg ClientId", msgTemp->xsmsAtEvent.clientId);
    AT_LOG1("AT_ProcXsmsMsg OpId", msgTemp->xsmsAtEvent.opId);

    if (At_ClientIdToUserId(msgTemp->xsmsAtEvent.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_ProcXsmsMsg At_ClientIdToUserId FAILURE");

        return;
    }

    /* 广播消息 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_XsmsIndProc(indexNum, msgTemp->eventType, &msgTemp->xsmsAtEvent);

        AT_NORM_LOG("At_PIHMsgProc : AT_BROADCAST_INDEX.");

        return;
    }

    AT_LOG1("At_PbMsgProc ucIndex", indexNum);

    /* AT命令回复处理 */
    At_XsmsCnfProc(indexNum, msgTemp->eventType, &msgTemp->xsmsAtEvent);

    return;
}
#endif

/* PC工程中AT从A核移到C核, At_sprintf有重复定义,故在此处添加条件编译宏 */
/*
 * Description: Sat消息处理函数
 * History:
 */
VOS_UINT32 At_ChangeSTKCmdNo(VOS_UINT32 cmdType, VOS_UINT8 *cmdNo)
{
    switch (cmdType) {
        case SI_STK_REFRESH:
            *cmdNo = SI_AT_CMD_REFRESH;
            break;
        case SI_STK_DISPLAYTET:
            *cmdNo = SI_AT_CMD_DISPLAY_TEXT;
            break;
        case SI_STK_GETINKEY:
            *cmdNo = SI_AT_CMD_GET_INKEY;
            break;
        case SI_STK_GETINPUT:
            *cmdNo = SI_AT_CMD_GET_INPUT;
            break;
        case SI_STK_PLAYTONE:
            *cmdNo = SI_AT_CMD_PLAY_TONE;
            break;
        case SI_STK_SELECTITEM:
            *cmdNo = SI_AT_CMD_SELECT_ITEM;
            break;
        case SI_STK_SETUPMENU:
            *cmdNo = SI_AT_CMD_SETUP_MENU;
            break;
        case SI_STK_SETUPIDLETEXT:
            *cmdNo = SI_AT_CMD_SETUP_IDLE_MODE_TEXT;
            break;
        case SI_STK_LAUNCHBROWSER:
            *cmdNo = SI_AT_CMD_LAUNCH_BROWSER;
            break;
        case SI_STK_SENDSS:
            *cmdNo = SI_AT_CMD_SEND_SS;
            break;
        case SI_STK_LANGUAGENOTIFICATION:
            *cmdNo = SI_AT_CMD_LANGUAGENOTIFICATION;
            break;
        case SI_STK_SETFRAMES:
            *cmdNo = SI_AT_CMD_SETFRAMES;
            break;
        case SI_STK_GETFRAMESSTATUS:
            *cmdNo = SI_AT_CMD_GETFRAMESSTATUS;
            break;
        default:
            return VOS_ERR;
    }

    return VOS_OK;
}



TAF_UINT32 At_HexText2AsciiStringSimple(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT32 len,
                                        TAF_UINT8 *str)
{
    TAF_UINT16 lenValue = 0;
    TAF_UINT16 chkLen   = 0;
    TAF_UINT8 *write    = dst;
    TAF_UINT8 *read     = str;
    TAF_UINT8  high     = 0;
    TAF_UINT8  low      = 0;
    /* "\"、"\"、"\0"三个字符所以长度+3 */
    if (((TAF_UINT32)(dst - (TAF_UINT8 *)headaddr) + (AT_ASCII_AND_HEX_CONVERSION_FACTOR * len) + 3) >= maxLength) {
        AT_ERR_LOG("At_HexText2AsciiString too long");
        return 0;
    }

    if (len != 0) {
        lenValue += 1;

        *write++ = '\"';

        /* 扫完整个字串 */
        while (chkLen++ < len) {
            high = 0x0F & (*read >> 4);
            low  = 0x0F & *read;
            lenValue += 2; /* 记录长度,高位和低位长度为2 */

            if (high <= 0x09) { /* 0-9 */
                *write++ = high + 0x30;
            } else if (high >= 0x0A) { /* A-F */
                *write++ = high + 0x37;
            } else {
            }

            if (low <= 0x09) { /* 0-9 */
                *write++ = low + 0x30;
            } else if (low >= 0x0A) { /* A-F */
                *write++ = low + 0x37;
            } else {
            }
            /* 下一个字符 */
            read++;
        }

        lenValue++;

        *write++ = '\"';

        *write++ = '\0';
    }

    return lenValue;
}

/*
 * Description: CSIN上报主动命令的打印输出函数
 * History:
 *  1.Date: 2012-5-17
 *    Modification: Created function
 */
TAF_VOID At_StkCsinIndPrint(TAF_UINT8 indexNum, SI_STK_EventInfo *event)
{
    TAF_UINT16 length = 0;

    /* 打印输入AT命令类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, g_atCsin);

    /* 打印输入主动命令类型长度和类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d",
        (event->cmdDataInfo.satCmd.satDataLen * AT_ASCII_AND_HEX_CONVERSION_FACTOR));

    /* 有主动命令时才输入 */
    if (event->cmdDataInfo.satCmd.satDataLen != 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ", %d, ", event->cmdDataInfo.satType);

        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        length += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                           (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                           event->cmdDataInfo.satCmd.satDataLen,
                                                           event->cmdDataInfo.satCmd.satCmdData);
    }

    /* 打印回车换行 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}

/*
 * Description: Sat消息处理函数
 * History:
 *  1.Date: 2009-07-04
 *    Author: zhuli
 *    Modification: Created function
 */
TAF_VOID At_STKCMDDataPrintSimple(TAF_UINT8 indexNum, SI_STK_EventInfo *event)
{
    TAF_UINT16 length = 0;

    if (event->cbEvent == SI_STK_CMD_IND_EVENT) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCsin);
    } else {
        if (event->cmdDataInfo.satType != SI_STK_SETUPMENU) {
            return;
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCsmn);
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d, %d, ",
        (event->cmdDataInfo.satCmd.satDataLen * AT_ASCII_AND_HEX_CONVERSION_FACTOR), event->cmdDataInfo.satType);

    length += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                       (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                       event->cmdDataInfo.satCmd.satDataLen,
                                                       event->cmdDataInfo.satCmd.satCmdData);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_CRLF_STR_LEN);

    return;
}

/*
 * Description: Sat消息处理函数
 * History:
 *  1.Date: 2009-07-04
 *    Author: zhuli
 *    Modification: Created function
 */
TAF_VOID At_STKCMDSWPrintSimple(TAF_UINT8 indexNum, STK_CallbackEventUint8 sTKCBEvent, SI_STK_SwInfo *sw)
{
    TAF_UINT16 length          = 0;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;
#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    VOS_UINT8 tmpSystemAppConfig;
#endif

#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    AT_SetSystemAppConfig((VOS_UINT16)indexNum, &tmpSystemAppConfig);
    systemAppConfig = &tmpSystemAppConfig;
#else
    /* 获取上层对接应用配置: MP/WEBUI/ANDROID */
    systemAppConfig = AT_GetSystemAppConfigAddr();
#endif

    if (*systemAppConfig != SYSTEM_APP_ANDROID) {
        return;
    }

    if (sTKCBEvent == SI_STK_TERMINAL_RSP_EVENT) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCstr);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCsen);
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d, %d%s", sw->sw1, sw->sw2, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_CRLF_STR_LEN);

    return;
}


VOS_VOID AT_SendSTKCMDTypeResultData(VOS_UINT8 indexNum, VOS_UINT16 length)
{
    errno_t memResult;
    if (g_atVType == AT_V_ENTIRE_TYPE) {
        /* Code前面加\r\n */
        memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr, AT_SEND_CRLF_ADDR_MAX_LEN, (TAF_CHAR *)g_atCrLf, AT_CRLF_STR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN, AT_CRLF_STR_LEN);
        At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_CRLF_STR_LEN);
    } else {
        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    return;
}


VOS_UINT32 At_STKCMDTypePrint(TAF_UINT8 indexNum, TAF_UINT32 satType, TAF_UINT32 eventType)
{
    VOS_UINT8    *systemAppConfig = VOS_NULL_PTR;
    TAF_UINT16    length          = 0;
    TAF_UINT8     cmdType         = 0;
    TAF_UINT32    result          = AT_SUCCESS;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;
#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    VOS_UINT8 tmpSystemAppConfig;
#endif

    /* 初始化 */
    modemId = MODEM_ID_0;

#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    AT_SetSystemAppConfig((VOS_UINT16)indexNum, &tmpSystemAppConfig);
    systemAppConfig = &tmpSystemAppConfig;
#else
    systemAppConfig = AT_GetSystemAppConfigAddr();
#endif

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_STKCMDTypePrint: Get modem id fail.");
        return AT_FAILURE;
    }

    /* 对接AP不需要检查 */
    if (*systemAppConfig != SYSTEM_APP_ANDROID) {
        if (eventType != SI_STK_CMD_END_EVENT) {
            result = At_ChangeSTKCmdNo(satType, &cmdType);
        }

        if (result == AT_FAILURE) {
            return AT_FAILURE;
        }
    }

    if (g_stkFunctionFlag == TAF_FALSE) {
        return AT_FAILURE;
    }

    switch (eventType) {
        case SI_STK_CMD_QUERY_RSP_EVENT:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s %d, 0%s", g_atStgi, cmdType, g_atCrLf);
            break;
        case SI_STK_CMD_IND_EVENT:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s %d, 0, 0%s", g_atStin, cmdType, g_atCrLf);
            break;
        case SI_STK_CMD_END_EVENT:
            if (*systemAppConfig == SYSTEM_APP_ANDROID) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%s 0, 0%s", g_atCsin, g_atCrLf);
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%s 99, 0, 0%s", g_atStin, g_atCrLf);
            }
            break;
        default:
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s %d, 0, 1%s", g_atStin, cmdType, g_atCrLf);
            break;
    }

    AT_SendSTKCMDTypeResultData(indexNum, length);

    return AT_SUCCESS;
}


TAF_VOID AT_STKCnfMsgProc(MN_APP_StkAtCnf *sTKCnfMsg)
{
    TAF_UINT8  indexNum;
    TAF_UINT32 result;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;
#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    VOS_UINT8 tmpSystemAppConfig;
#endif

    indexNum        = 0;
    result          = AT_OK;

    AT_LOG1("AT_STKCnfMsgProc pEvent->ClientId", sTKCnfMsg->stkAtCnf.clientId);
    AT_LOG1("AT_STKCnfMsgProc EventType", sTKCnfMsg->stkAtCnf.cbEvent);
    AT_LOG1("AT_STKCnfMsgProc SIM Event Error", sTKCnfMsg->stkAtCnf.errorNo);

    g_atSendDataBuff.bufLen = 0;

    if (At_ClientIdToUserId(sTKCnfMsg->stkAtCnf.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_STKCnfMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

#if (FEATURE_MBB_CUST == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_OFF)
    AT_SetSystemAppConfig((VOS_UINT16)indexNum, &tmpSystemAppConfig);
    systemAppConfig = &tmpSystemAppConfig;
#else
    systemAppConfig = AT_GetSystemAppConfigAddr();
#endif

    /* 如果不是主动命令，则停止定时器 */
    if ((sTKCnfMsg->stkAtCnf.cbEvent != SI_STK_CMD_IND_EVENT) &&
        (sTKCnfMsg->stkAtCnf.cbEvent != SI_STK_CMD_TIMEOUT_IND_EVENT) &&
        (sTKCnfMsg->stkAtCnf.cbEvent != SI_STK_CMD_END_EVENT) &&
        (sTKCnfMsg->stkAtCnf.cbEvent != SI_STK_CC_RESULT_IND_EVENT)

        && (sTKCnfMsg->stkAtCnf.cbEvent != SI_STK_SMSCTRL_RESULT_IND_EVENT)) {
        if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
            AT_WARN_LOG("AT_STKCnfMsgProc: AT_BROADCAST_INDEX.");
            return;
        }

        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_LOG1("AT_STKCnfMsgProc ucIndex", indexNum);
        AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);
    }

    if (sTKCnfMsg->stkAtCnf.errorNo != AT_SUCCESS) {
        result = At_ChgTafErrorCode(indexNum, (TAF_UINT16)sTKCnfMsg->stkAtCnf.errorNo); /* 发生错误 */

        At_FormatResultData(indexNum, result);
    } else {
        switch (sTKCnfMsg->stkAtCnf.cbEvent) {
            case SI_STK_CMD_IND_EVENT:
                if (*systemAppConfig == SYSTEM_APP_ANDROID) {
                    At_StkCsinIndPrint(indexNum, &(sTKCnfMsg->stkAtCnf));
                } else {
                    At_STKCMDTypePrint(indexNum, sTKCnfMsg->stkAtCnf.cmdDataInfo.satType, sTKCnfMsg->stkAtCnf.cbEvent);
                }

                break;
            case SI_STK_CMD_END_EVENT:
            case SI_STK_CMD_TIMEOUT_IND_EVENT:
                At_STKCMDTypePrint(indexNum, sTKCnfMsg->stkAtCnf.cmdDataInfo.satType, sTKCnfMsg->stkAtCnf.cbEvent);
                break;

            case SI_STK_CMD_QUERY_RSP_EVENT:
                At_STKCMDTypePrint(indexNum, sTKCnfMsg->stkAtCnf.cmdDataInfo.satType, sTKCnfMsg->stkAtCnf.cbEvent);
                At_FormatResultData(indexNum, result);
                break;

            case SI_STK_GET_CMD_RSP_EVENT:
                if (*systemAppConfig == SYSTEM_APP_ANDROID) {
                    At_StkCsinIndPrint(indexNum, &(sTKCnfMsg->stkAtCnf));
                } else {
                    At_STKCMDDataPrintSimple(indexNum, &(sTKCnfMsg->stkAtCnf));
                }

                At_FormatResultData(indexNum, result);
                break;

            case SI_STK_ENVELPOE_RSP_EVENT:
            case SI_STK_TERMINAL_RSP_EVENT:
                At_STKCMDSWPrintSimple(indexNum, sTKCnfMsg->stkAtCnf.cbEvent, &sTKCnfMsg->stkAtCnf.swInfo);
                At_FormatResultData(indexNum, result);
                break;

            case SI_STK_CC_RESULT_IND_EVENT:
            case SI_STK_SMSCTRL_RESULT_IND_EVENT:
                At_StkCcinIndPrint(indexNum, &(sTKCnfMsg->stkAtCnf));
                break;

            default:
                At_FormatResultData(indexNum, result);
                break;
        }
    }

    return;
}


TAF_VOID AT_STKPrintMsgProc(MN_APP_StkAtDataprint *sTKPrintMsg)
{
    errno_t   memResult;
    TAF_UINT8 indexNum = 0;

    if (At_ClientIdToUserId(sTKPrintMsg->stkAtPrint.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_STKPrintMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_STKPrintMsgProc: AT_BROADCAST_INDEX.");
        return;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    AT_LOG1("At_STKMsgProc pEvent->ClientId", sTKPrintMsg->stkAtPrint.clientId);
    AT_LOG1("At_STKMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    AT_SetStkCmdQualify(sTKPrintMsg->stkAtPrint.cmdQualify);

    sTKPrintMsg->stkAtPrint.dataLen = TAF_MIN(sTKPrintMsg->stkAtPrint.dataLen, STK_PRINT_MAX_LEN);
    if (sTKPrintMsg->stkAtPrint.dataLen > 0) {
        memResult = memcpy_s(g_atSndCodeAddress, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET,
                             sTKPrintMsg->stkAtPrint.data, sTKPrintMsg->stkAtPrint.dataLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET,
                            sTKPrintMsg->stkAtPrint.dataLen);
    }

    At_SendResultData(indexNum, g_atSndCrLfAddr, (VOS_UINT16)sTKPrintMsg->stkAtPrint.dataLen + AT_CRLF_STR_LEN);

    At_FormatResultData(indexNum, AT_OK);

    return;
}


TAF_VOID At_STKMsgProc(struct MsgCB *msg)
{
    MN_APP_StkAtDataprint *sTKPrintMsg = VOS_NULL_PTR;
    MN_APP_StkAtCnf       *sTKCnfMsg   = VOS_NULL_PTR;

    sTKCnfMsg   = (MN_APP_StkAtCnf *)msg;
    sTKPrintMsg = (MN_APP_StkAtDataprint *)msg;

    if (sTKCnfMsg->msgId == STK_AT_DATAPRINT_CNF) {
        AT_STKPrintMsgProc(sTKPrintMsg);
    } else if (sTKCnfMsg->msgId == STK_AT_EVENT_CNF) {
        AT_STKCnfMsgProc(sTKCnfMsg);
    } else {
        AT_ERR_LOG1("At_STKMsgProc:Msg ID Error", sTKPrintMsg->msgId);
    }

    return;
}


VOS_UINT32 At_PrintEsimCleanProfileInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CmdEsimSwitch_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMCLEAN_SET) {
        AT_WARN_LOG("At_PrintEsimCleanProfileInfo: CmdCurrentOpt is not AT_CMD_ESIMCLEAN_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_PrintEsimCheckProfileInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_ESIMCHECK_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMCHECK_QRY) {
        AT_WARN_LOG("At_PrintEsimCheckProfileInfo: CmdCurrentOpt is not AT_CMD_ESIMCHECK_QRY!");
        return AT_ERROR;
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ESIMCHECK: ", g_atCrLf);

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", event->ematEvent.esimCheckCnf.hasProfile);
    return AT_OK;
}


VOS_UINT32 At_PrintGetEsimEidInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_ESIMEID_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMEID_QRY) {
        AT_WARN_LOG("At_PrintGetEsimEidInfo : CmdCurrentOpt is not AT_CMD_ESIMEID_QRY!");
        return AT_ERROR;
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s^ESIMEID:\"", g_atCrLf);

    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     (TAF_UINT8 *)event->ematEvent.eidCnf.esimEID,
                                                     SI_EMAT_ESIM_EID_MAX_LEN);

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"%s", g_atCrLf);

    return AT_OK;
}


VOS_UINT32 At_PrintGetEsimPKIDInfo(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event, VOS_UINT16 *length)
{
    VOS_UINT32 pKIDNum;
    VOS_UINT32 loop = 0;

    /* 判断当前操作类型是否为AT_CMD_ESIMPKID_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ESIMPKID_QRY) {
        AT_WARN_LOG("At_PrintGetEsimPKIDInfo : CmdCurrentOpt is not AT_CMD_ESIMPKID_QRY!");
        return AT_ERROR;
    }

    pKIDNum = (event->ematEvent.pkIdCnf.pkIdNum > SI_EMAT_ESIM_PKID_GROUP_MAX_NUM) ? SI_EMAT_ESIM_PKID_GROUP_MAX_NUM :
                                                                                     event->ematEvent.pkIdCnf.pkIdNum;

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s^PKID:%d", g_atCrLf, pKIDNum);

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"");

    for (loop = 0; loop < pKIDNum; loop++) {
        (*length) += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         (TAF_UINT8 *)event->ematEvent.pkIdCnf.pkId[loop].esimPKID,
                                                         SI_EMAT_ESIM_PKID_VALUE_LEN);

        if (loop < pKIDNum - 1) {
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "\",\"");
        }
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"%s", g_atCrLf);

    return AT_OK;
}


TAF_VOID At_EMATIndProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event)
{
    return;
}


TAF_UINT32 At_EMATNotBroadIndProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event)
{
    return VOS_ERR;
}


TAF_VOID At_EMATRspProc(TAF_UINT8 indexNum, SI_EMAT_EventInfo *event)
{
    TAF_UINT32 result = AT_FAILURE;
    TAF_UINT16 length = 0;
    TAF_UINT32 tmp;
    TAF_UINT32 i;

    if (event->ematError != TAF_ERR_NO_ERROR) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        result = At_ChgTafErrorCode(indexNum, (TAF_UINT16)(event->ematError));

        g_atSendDataBuff.bufLen = length;

        At_FormatResultData(indexNum, result);

        return;
    }

    tmp = (sizeof(g_atEmatRspProcFuncTbl) / sizeof(g_atEmatRspProcFuncTbl[0]));

    for (i = 0; i < tmp; i++) {
        /* 找到处理函数，进行输出相关处理 */
        if (event->eventType == g_atEmatRspProcFuncTbl[i].eventType) {
            result = g_atEmatRspProcFuncTbl[i].atEMATRspProcFunc(indexNum, event, &length);

            if (result == AT_ERROR) {
                AT_WARN_LOG("At_EMATRspProc : pAtEMATRspProcFunc is return error!");
                return;
            }

            break;
        }
    }

    /* 没找到处理函数，直接返回 */
    if (i == tmp) {
        AT_WARN_LOG("At_EMATRspProc : no find AT Proc Func!");
        return;
    }

    result = AT_OK;

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_ChkEmatAtEventCnfMsgLen(const MSG_Header *msgHeader)
{
    const MN_APP_EmatAtCnf  *ematAtCnf  = VOS_NULL_PTR;
    const SI_EMAT_EventInfo *event = VOS_NULL_PTR;
    VOS_UINT32               size, i;

    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < AT_GET_EMAT_AT_CNF_LEN_WITHOUT_UNION) {
        AT_WARN_LOG("AT_ChkEmatAtEventCnfMsgLen: message length is invalid!");
        return VOS_FALSE;
    }
    ematAtCnf = (const MN_APP_EmatAtCnf *)msgHeader;
    event     = &ematAtCnf->ematAtCnf;

    size = (sizeof(g_atEmatRspProcFuncTbl) / sizeof(g_atEmatRspProcFuncTbl[0]));
    /*
     * 1、单板场景下，EMAT_AT_EVENT_CNF消息的检查原则是消息申请按照union最大申请或者该消息实际大小
     * 申请，只有(VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < fixdedPartLen这种情况认为检查失败
     * 2、PC环境下，EMAT_AT_EVENT_CNF消息的检查原则，消息申请的大小即不是按照union最大申请，也不是
     * 消息实际大小申请时，认为检查失败
     */
#if (VOS_OS_VER == VOS_WIN32)
    for (i = 0; i < size; i++) {
        if (event->eventType == g_atEmatRspProcFuncTbl[i].eventType) {
            if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH != sizeof(MN_APP_EmatAtCnf)) &&
                (TAF_RunChkMsgLenFunc(msgHeader,
                    (AT_GET_EMAT_AT_CNF_LEN_WITHOUT_UNION + g_atEmatRspProcFuncTbl[i].fixdedPartLen),
                    g_atEmatRspProcFuncTbl[i].chkFunc) != VOS_TRUE)) {
                AT_WARN_LOG("AT_ChkEmatAtEventCnfMsgLen: message length is invalid!");
                return VOS_FALSE;
            }
            break;
        }
    }
#else
    for (i = 0; i < size; i++) {
        if (event->eventType == g_atEmatRspProcFuncTbl[i].eventType) {
            if (TAF_RunChkMsgLenFunc(msgHeader,
                    (AT_GET_EMAT_AT_CNF_LEN_WITHOUT_UNION + g_atEmatRspProcFuncTbl[i].fixdedPartLen),
                    g_atEmatRspProcFuncTbl[i].chkFunc) != VOS_TRUE) {
                AT_WARN_LOG("AT_ChkEmatAtEventCnfMsgLen: message length is invalid!");
                return VOS_FALSE;
            }
            break;
        }
    }
#endif
    return VOS_TRUE;
}


TAF_VOID At_EMATMsgProc(struct MsgCB *msg)
{
    MN_APP_EmatAtCnf *msgTemp  = VOS_NULL_PTR;
    TAF_UINT8         indexNum = 0;

    msgTemp = (MN_APP_EmatAtCnf *)msg;

    if (msgTemp->msgId != EMAT_AT_EVENT_CNF) {
        AT_ERR_LOG1("At_EMATMsgProc: The Msg Id is Wrong", msgTemp->msgId);
        return;
    }

    AT_LOG1("At_EMATMsgProc pEvent->ClientId", msgTemp->ematAtCnf.clientId);
    AT_LOG1("At_EMATMsgProc EventType", msgTemp->ematAtCnf.eventType);
    AT_LOG1("At_EMATMsgProc SIM Event Error", msgTemp->ematAtCnf.ematError);

    if (At_ClientIdToUserId(msgTemp->ematAtCnf.clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("At_EMATMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_EMATIndProc(indexNum, &msgTemp->ematAtCnf);
        AT_WARN_LOG("At_EMATMsgProc : AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_EMATMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    /* 非广播的主动上报 */
    if (At_EMATNotBroadIndProc(indexNum, &msgTemp->ematAtCnf) == VOS_OK) {
        return;
    }

    At_EMATRspProc(indexNum, &msgTemp->ematAtCnf);

    return;
}


TAF_VOID At_DataStatusMsgProc(TAF_DataStatusInd *dataStatus)
{
    AT_RreturnCodeUint32 result   = AT_FAILURE;
    TAF_UINT8            indexNum = 0;

    AT_LOG1("At_DataStatusMsgProc: ClientId", dataStatus->clientId);
    AT_LOG1("At_DataStatusMsgProc: ucDomain", dataStatus->domain);
    AT_LOG1("At_DataStatusMsgProc: ucRabId", dataStatus->rabId);
    AT_LOG1("At_DataStatusMsgProc: ucStatus", dataStatus->status);
    AT_LOG1("At_DataStatusMsgProc: ucRabId", dataStatus->cause);
    if (At_ClientIdToUserId(dataStatus->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_DataStatusMsgProc: At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_DataStatusMsgProc: AT_BROADCAST_INDEX.");
        return;
    }

    AT_LOG1("At_DataStatusMsgProc: ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    switch (dataStatus->status) {
        case TAF_RABM_STOP_DATA:
        case TAF_DATA_STOP:
            break;

        default:
            break;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
}


VOS_UINT32 AT_ConvertCallError(TAF_CS_CauseUint32 cause)
{
    const AT_CmeCallErrCodeMap *callErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32                  callErrMapTblSize;
    VOS_UINT32                  cnt;

    callErrMapTblPtr  = AT_GET_CME_CALL_ERR_CODE_MAP_TBL_PTR();
    callErrMapTblSize = AT_GET_CME_CALL_ERR_CODE_MAP_TBL_SIZE();

    for (cnt = 0; cnt < callErrMapTblSize; cnt++) {
        if (callErrMapTblPtr[cnt].csCause == cause) {
            return callErrMapTblPtr[cnt].cmeCode;
        }
    }

    return AT_CME_UNKNOWN;
}

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))

AT_EncryptVoiceErrorUint32 AT_MapEncVoiceErr(TAF_CALL_EncryptVoiceStatusUint32 tafEncVoiceErr)
{
    VOS_UINT32                          i;
    const AT_EncryptVoiceErrCodeMap    *atEncVoiceErrMapTbl = VOS_NULL_PTR;
    VOS_UINT32                          atEncVoiceErrMapSize;

    atEncVoiceErrMapTbl  = AT_GET_ENC_VOICE_ERR_CODE_MAP_TBL_PTR();
    atEncVoiceErrMapSize = AT_GET_ENC_VOICE_ERR_CODE_MAP_TBL_SIZE();

    for (i = 0; i < atEncVoiceErrMapSize; i++) {
        if (atEncVoiceErrMapTbl[i].tafEncErr == tafEncVoiceErr) {
            return atEncVoiceErrMapTbl[i].atEncErr;
        }
    }
    return AT_ENCRYPT_VOICE_ERROR_ENUM_BUTT;
}
#endif


TAF_UINT32 At_CmdCmgdMsgProc(TAF_UINT8 indexNum, TAF_UINT32 errorCode)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;
    TAF_UINT32 result          = AT_FAILURE;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    if ((errorCode == MN_ERR_CLASS_SMS_EMPTY_REC) && (*systemAppConfig != SYSTEM_APP_ANDROID)) {
        result = AT_OK;
        AT_STOP_TIMER_CMD_READY(indexNum);
    } else {
        result = At_ChgMnErrCodeToAt(indexNum, errorCode); /* 发生错误 */
        AT_STOP_TIMER_CMD_READY(indexNum);
    }

    return result;
}


TAF_VOID At_CmdCnfMsgProc(TAF_UINT8 *data, TAF_UINT16 len)
{
    AT_CmdCnfEvent *cmdCnf = VOS_NULL_PTR;
    MN_CLIENT_ID_T  clientId;
    TAF_UINT32      errorCode;
    TAF_UINT8       indexNum = 0;
    TAF_UINT32      result   = AT_FAILURE;
    TAF_UINT16      length   = 0;

    cmdCnf = (AT_CmdCnfEvent *)data;

    clientId  = cmdCnf->clientId;
    errorCode = cmdCnf->errorCode;

    if (At_ClientIdToUserId(clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_CmdCnfMsgProc At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_CmdCnfMsgProc: AT_BROADCAST_INDEX.");
        return;
    }

    if (g_parseContext[indexNum].clientStatus == AT_FW_CLIENT_STATUS_READY) {
        AT_WARN_LOG("At_CmdCnfMsgProc : AT command entity is released.");
        return;
    }

    /*
     * call业务上报的是TAF_CS_CAUSE_SUCCESS，而短信业务上报的是MN_ERR_NO_ERROR,
     * 他们的实际值都为0
     */
    if (errorCode == MN_ERR_NO_ERROR) {
        /*
         * 因判断是否有呼叫在C核上实现，在无呼叫的情况下上报MN_ERR_NO_ERROR
         * AT命令返回结果需要为AT_OK
         */
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_H_SET ||
            g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CHUP_SET) {
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_OK);
        }

        AT_NORM_LOG("At_CmdCnfMsgProc Rsp No Err");
        return;
    }

    AT_LOG1("At_CmdCnfMsgProc ucIndex", indexNum);
    AT_LOG1("g_atClientTab[ucIndex].cmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CDUR_READ:
        /* CCWA命令相关 */
        case AT_CMD_CCWA_DISABLE:
        case AT_CMD_CCWA_ENABLE:
        case AT_CMD_CCWA_QUERY:

        /* CCFC命令 */
        case AT_CMD_CCFC_DISABLE:
        case AT_CMD_CCFC_ENABLE:
        case AT_CMD_CCFC_QUERY:
        case AT_CMD_CCFC_REGISTRATION:
        case AT_CMD_CCFC_ERASURE:

        /* CUSD相关命令 */
        case AT_CMD_CUSD_REQ:

        /* CLCK相关命令 */
        case AT_CMD_CLCK_UNLOCK:
        case AT_CMD_CLCK_LOCK:
        case AT_CMD_CLCK_QUERY:

        /* CLOP命令 */
        case AT_CMD_COLP_READ:

        /* CLIR命令 */
        case AT_CMD_CLIR_READ:

        /* CLIP命令 */
        case AT_CMD_CLIP_READ:
        /* CPWD命令 */
        case AT_CMD_CPWD_SET:

        case AT_CMD_CNAP_QRY:
            result = AT_ConvertCallError(errorCode);
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;
        case AT_CMD_CSCA_READ:
            result = At_ChgMnErrCodeToAt(indexNum, errorCode);
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;
        case AT_CMD_CPMS_SET:
        case AT_CMD_CPMS_READ:
            result = AT_CMS_UNKNOWN_ERROR;
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;

            /*
             * 如果ulErrorCode不为TAF_CS_CAUSE_NO_CALL_ID，则AT_CMD_D_CS_VOICE_CALL_SET
             * 和AT_CMD_D_CS_DATA_CALL_SET业务统一上报AT_NO_CARRIER错误值
             */

        case AT_CMD_D_CS_VOICE_CALL_SET:
        case AT_CMD_APDS_SET:
            if (errorCode == TAF_CS_CAUSE_NO_CALL_ID) {
                result = AT_ERROR;
                AT_STOP_TIMER_CMD_READY(indexNum);
                break;
            }
            /* fall-through */

        case AT_CMD_D_CS_DATA_CALL_SET:
            result = AT_NO_CARRIER;
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;

        case AT_CMD_CHLD_SET:
        case AT_CMD_CHUP_SET:
        case AT_CMD_A_SET:
        case AT_CMD_CHLD_EX_SET:
        case AT_CMD_H_SET:
            result = AT_ConvertCallError(errorCode); /* 发生错误 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;
        case AT_CMD_CMGR_SET:
        case AT_CMD_CMGD_SET:
            result = At_CmdCmgdMsgProc(indexNum, errorCode);
            break;
            /* fall through */
        case AT_CMD_CSMS_SET:
        case AT_CMD_CMMS_SET:
        case AT_CMD_CGSMS_SET:
        case AT_CMD_CGSMS_READ:
        case AT_CMD_CMMS_READ:
        case AT_CMD_CSMP_READ: /* del */
        case AT_CMD_CMGS_TEXT_SET:
        case AT_CMD_CMGS_PDU_SET:
        case AT_CMD_CMGC_TEXT_SET:
        case AT_CMD_CMGC_PDU_SET:
        case AT_CMD_CMSS_SET:
        case AT_CMD_CMST_SET:
        case AT_CMD_CNMA_TEXT_SET:
        case AT_CMD_CNMA_PDU_SET:
        case AT_CMD_CMGW_PDU_SET:
        case AT_CMD_CMGW_TEXT_SET:
        case AT_CMD_CMGL_SET:
        case AT_CMD_CMGD_TEST:
        case AT_CMD_CSMP_SET:
        case AT_CMD_CSCA_SET:
#if ((FEATURE_GCBS == FEATURE_ON) || (FEATURE_WCBS == FEATURE_ON))
        case AT_CMD_CSCB_SET:
        case AT_CMD_CSCB_READ:
#endif
            result = At_ChgMnErrCodeToAt(indexNum, errorCode); /* 发生错误 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;
        default:
            /* 默认值不知道是不是该填这个，暂时先写这个 */
            result = AT_CME_UNKNOWN;
            AT_STOP_TIMER_CMD_READY(indexNum);
            break;
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}


TAF_UINT32 At_PrintTimeZoneInfo(NAS_MM_InfoInd *mmInfo, VOS_UINT8 *dst)
{
    VOS_INT8   timeZone;
    VOS_UINT8  timeZoneValue;
    VOS_UINT16 length;

    length   = 0;
    timeZone = AT_INVALID_TZ_VALUE;

    /* 获取网络上报的时区信息 */
    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        timeZone = mmInfo->universalTimeandLocalTimeZone.timeZone;
    }

    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ) {
        timeZone = mmInfo->localTimeZone;
    }

    if (timeZone < 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "-");

        timeZoneValue = (VOS_UINT8)(timeZone * (-1));
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "+");

        timeZoneValue = (VOS_UINT8)timeZone;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%02d", timeZoneValue);

    /* 结尾 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "\"%s", g_atCrLf);
    return length;
}


VOS_UINT32 AT_PrintTimeZoneInfoNoAdjustment(TAF_AT_CommTime *mmInfo, VOS_UINT8 *dst)
{
    VOS_INT8   timeZone;
    VOS_UINT8  timeZoneValue;
    VOS_UINT16 length;

    length   = 0;
    timeZone = AT_INVALID_TZ_VALUE;

    /* 获得时区 */
    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        timeZone = mmInfo->universalTimeandLocalTimeZone.timeZone;
    }


    if (timeZone < 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "-");

        timeZoneValue = (VOS_UINT8)(timeZone * (-1));
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "+");

        timeZoneValue = (VOS_UINT8)timeZone;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d", timeZoneValue);

    /* 显示夏时制或冬时制信息 */
    if (((mmInfo->ieFlg & NAS_MM_INFO_IE_DST) == NAS_MM_INFO_IE_DST) && (mmInfo->dst > 0)) {
        /* 夏时制: DST字段存在, 且值大于0， */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, ",%02d\"%s", mmInfo->dst, g_atCrLf);
    } else {
        /* 冬时制 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, ",00\"%s", g_atCrLf);
    }

    return length;
}


VOS_UINT32 AT_PrintLocalTimeWithCtzeType(TAF_AT_CommTime *mmInfo, VOS_UINT8 *dst)
{
    VOS_UINT16 length = 0;

    /* YY */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, ",\"%d/", mmInfo->universalTimeandLocalTimeZone.year);
    /* MM */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d%d/", mmInfo->universalTimeandLocalTimeZone.month / AT_DECIMAL_BASE_NUM,
        mmInfo->universalTimeandLocalTimeZone.month % AT_DECIMAL_BASE_NUM);
    /* dd */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d%d,", mmInfo->universalTimeandLocalTimeZone.day / AT_DECIMAL_BASE_NUM,
        mmInfo->universalTimeandLocalTimeZone.day % AT_DECIMAL_BASE_NUM);

    /* hh */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d%d:", mmInfo->universalTimeandLocalTimeZone.hour / AT_DECIMAL_BASE_NUM,
        mmInfo->universalTimeandLocalTimeZone.hour % AT_DECIMAL_BASE_NUM);

    /* mm */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d%d:", mmInfo->universalTimeandLocalTimeZone.minute / AT_DECIMAL_BASE_NUM,
        mmInfo->universalTimeandLocalTimeZone.minute % AT_DECIMAL_BASE_NUM);

    /* ss */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%d%d\"", mmInfo->universalTimeandLocalTimeZone.second / AT_DECIMAL_BASE_NUM,
        mmInfo->universalTimeandLocalTimeZone.second % AT_DECIMAL_BASE_NUM);

    return length;
}


VOS_UINT64 AT_TransUniversalTimeToUTC(TAF_AT_CommTime *mmInfo)
{
    TAF_STD_TimeZoneType universalTime = {0};

    /* 输入的year值为两位数，默认从2000年开始计算 */
    universalTime.year = (VOS_UINT16)(mmInfo->universalTimeandLocalTimeZone.year + AT_CTZE_DEFAULT_YEAR);
    universalTime.month = (VOS_UINT16)mmInfo->universalTimeandLocalTimeZone.month;
    universalTime.day = (VOS_UINT16)mmInfo->universalTimeandLocalTimeZone.day;
    universalTime.hour = (VOS_UINT16)mmInfo->universalTimeandLocalTimeZone.hour;
    universalTime.minute = (VOS_UINT16)mmInfo->universalTimeandLocalTimeZone.minute;
    universalTime.second = (VOS_UINT16)mmInfo->universalTimeandLocalTimeZone.second;

    return TAF_STD_DateTimeToSecondTime(&universalTime);
}


VOS_VOID AT_TransUTCToLocalTime(TAF_AT_CommTime *mmInfo, VOS_UINT64 secondTime)
{
    TAF_STD_TimeZoneType  dateTime = {0};

    TAF_STD_SecondTimeToDateTime(secondTime, &dateTime);

    mmInfo->universalTimeandLocalTimeZone.year = (VOS_UINT16)dateTime.year;
    mmInfo->universalTimeandLocalTimeZone.month = (VOS_UINT8)dateTime.month;
    mmInfo->universalTimeandLocalTimeZone.day = (VOS_UINT8)dateTime.day;
    mmInfo->universalTimeandLocalTimeZone.hour = (VOS_UINT8)dateTime.hour;
    mmInfo->universalTimeandLocalTimeZone.minute = (VOS_UINT8)dateTime.minute;
    mmInfo->universalTimeandLocalTimeZone.second = (VOS_UINT8)dateTime.second;
}


VOS_UINT32 AT_PrintTimeZoneInfoWithCtzeType(TAF_AT_CommTime *mmInfo, VOS_UINT8 *dst)
{
    VOS_UINT64 timep = 0;
    VOS_UINT16 length = 0;
    VOS_INT8   timeZone = AT_INVALID_TZ_VALUE;
    VOS_UINT8  timeZoneValue = 0;

    /* 获取网络上报的时区信息 */
    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        timeZone = mmInfo->universalTimeandLocalTimeZone.timeZone;
    }

    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ) {
        timeZone = mmInfo->localTimeZone;
    }

    /* 将网络时间(年/月/日/时/分/秒)转换为UTC(秒为单位) */
    timep = AT_TransUniversalTimeToUTC(mmInfo);

    if (timeZone < 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "-");

        timeZoneValue = (VOS_UINT8)(timeZone * (-1));
        /* 减去时区时间(秒) */
        if (timeZoneValue <= AT_LOCAL_TIME_ZONE_DEL_MAX_VALUE) {
            timep -= timeZoneValue * AT_MINUTES_OF_ONE_QUARTER * AT_SECONDS_OF_ONE_MINUTE;
        }
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "+");

        timeZoneValue = (VOS_UINT8)timeZone;
        /* 累加时区时间(秒) */
        if (timeZoneValue <= AT_LOCAL_TIME_ZONE_ADD_MAX_VALUE) {
            timep += timeZoneValue * AT_MINUTES_OF_ONE_QUARTER * AT_SECONDS_OF_ONE_MINUTE;
        }
    }

    /* 将UTC(秒为单位)转换为本地时间(年/月/日/时/分/秒) */
    AT_TransUTCToLocalTime(mmInfo, timep);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%02d\"", timeZoneValue);

    /* 显示夏时制或冬时制信息 */
    if (((mmInfo->ieFlg & NAS_MM_INFO_IE_DST) == NAS_MM_INFO_IE_DST) && (mmInfo->dst > 0)) {
        /* 夏时制: DST字段存在, 且值大于0 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, ",%01d", mmInfo->dst);
    } else {
        /* 冬时制 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, ",0");
    }

    /* 显示时间信息 */
    if ((mmInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
       length += AT_PrintLocalTimeWithCtzeType(mmInfo, dst + length);
    }

    /* 结尾 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)dst + length, "%s", g_atCrLf);

    return length;
}


VOS_UINT8 At_GetDaysForEachMonth(VOS_UINT8 year, VOS_UINT8 month)
{
    VOS_UINT16 adjustYear;

    /* 输入的year值为两位数，默认从2000年开始计算 */
    adjustYear = 2000 + (VOS_UINT16)year;

    if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) ||
        (month == 12)) {
        /* 1,3,5,7,8,10,12月有31天 */
        return 31;
    } else if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
        /* 4,6,9,11月有30天 */
        return 30;
    } else {
        /* 看2月是否为闰年，是则为29天，否则为28天 */
        if ((((adjustYear % 4) == 0) && ((adjustYear % 100) != 0)) || ((adjustYear % 400) == 0)) {
            /* 闰年，2月天数 */
            return 29;
        } else {
            /* 非闰年，2月天数 */
            return 28;
        }
    }
}


VOS_VOID At_AdjustLocalDate(TIME_ZONE_Time *uinversalTime, VOS_INT8 adjustValue, TIME_ZONE_Time *localTime)
{
    VOS_UINT8 day;

    /* 调整日期 */
    day = (VOS_UINT8)(uinversalTime->day + adjustValue);

    if (day == 0) {
        /* 月份减一 */
        if (uinversalTime->month == 1) {
            /* 调整为上一年的12月份,年份减一 */
            localTime->month = 12;

            if (uinversalTime->year == 0) {
                /* 如果是2000年，调整为1999年 */
                localTime->year = 99;
            } else {
                localTime->year = uinversalTime->year - 1;
            }
        } else {
            localTime->month = uinversalTime->month - 1;
            localTime->year  = uinversalTime->year;
        }

        /* 日期调整为上个月的最后一天, */
        localTime->day = At_GetDaysForEachMonth(localTime->year, localTime->month);
    } else if (day > At_GetDaysForEachMonth(uinversalTime->year, uinversalTime->month)) {
        /* 日期调整为下个月一号 */
        localTime->day = 1;

        /* 月份加一 当前月份为12 */
        if (uinversalTime->month == 12) {
            /* 调整为下一年的1月份,年份加一 */
            localTime->month = 1;
            localTime->year  = uinversalTime->year + 1;
        } else {
            localTime->month = uinversalTime->month + 1;
            localTime->year  = uinversalTime->year;
        }
    } else {
        localTime->day   = day;
        localTime->month = uinversalTime->month;
        localTime->year  = uinversalTime->year;
    }
}


TAF_UINT32 At_PrintMmTimeInfo(VOS_UINT8 indexNum, TAF_AT_CommTime *mmTimeInfo, TAF_UINT8 *dst)
{
    TAF_UINT16      length;
    TAF_INT8        timeZone;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    ModemIdUint16   modemId;
    VOS_UINT32      rslt;
    VOS_UINT32      chkTimeFlg;
    VOS_UINT32      chkCtzvFlg;
    VOS_UINT32      chkCtzeFlg;

    length = 0;

    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_PrintMmTimeInfo: Get modem id fail.");
        return length;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if ((mmTimeInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        /* 保存网络下发的时间信息，无该字段，则使用原有值 */
        netCtx->timeInfo.ieFlg |= NAS_MM_INFO_IE_UTLTZ;

        /* 除以100是为了取年份的后两位 */
        netCtx->timeInfo.universalTimeandLocalTimeZone.year     = mmTimeInfo->universalTimeandLocalTimeZone.year % 100;
        netCtx->timeInfo.universalTimeandLocalTimeZone.month    = mmTimeInfo->universalTimeandLocalTimeZone.month;
        netCtx->timeInfo.universalTimeandLocalTimeZone.day      = mmTimeInfo->universalTimeandLocalTimeZone.day;
        netCtx->timeInfo.universalTimeandLocalTimeZone.hour     = mmTimeInfo->universalTimeandLocalTimeZone.hour;
        netCtx->timeInfo.universalTimeandLocalTimeZone.minute   = mmTimeInfo->universalTimeandLocalTimeZone.minute;
        netCtx->timeInfo.universalTimeandLocalTimeZone.second   = mmTimeInfo->universalTimeandLocalTimeZone.second;
        netCtx->timeInfo.universalTimeandLocalTimeZone.reserved = 0x00;
        netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone = mmTimeInfo->universalTimeandLocalTimeZone.timeZone;
        netCtx->nwSecond                                        = AT_GetSeconds();
    }

    /* 更新DST信息 */
    if ((mmTimeInfo->ieFlg & NAS_MM_INFO_IE_DST) == NAS_MM_INFO_IE_DST) {
        /* 保存网络下发的时间信息 */
        netCtx->timeInfo.ieFlg |= NAS_MM_INFO_IE_DST;
        netCtx->timeInfo.dst = mmTimeInfo->dst;
    } else {
        netCtx->timeInfo.ieFlg &= ~NAS_MM_INFO_IE_DST;
    }

    chkCtzvFlg = AT_CheckRptCmdStatus(mmTimeInfo->unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED, AT_RPT_CMD_CTZV);
    chkTimeFlg = AT_CheckRptCmdStatus(mmTimeInfo->unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED, AT_RPT_CMD_TIME);
    chkCtzeFlg = AT_CheckRptCmdStatus(mmTimeInfo->unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED, AT_RPT_CMD_CTZE);

    /* 时区显示格式: +CTZV: "GMT±tz, Summer(Winter) Time" */
    if ((AT_CheckRptCmdStatus(mmTimeInfo->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CTZV) == VOS_TRUE) &&
        (chkCtzvFlg == VOS_TRUE)) {
        if ((mmTimeInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
            timeZone = mmTimeInfo->universalTimeandLocalTimeZone.timeZone;
        } else {
            timeZone = mmTimeInfo->localTimeZone;
        }

        if (timeZone != netCtx->timeInfo.localTimeZone) {
            /* 保存网络下发的时区信息 */
            netCtx->timeInfo.ieFlg |= NAS_MM_INFO_IE_LTZ;
            netCtx->timeInfo.localTimeZone                          = timeZone;
            netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone = timeZone;

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)dst + length, "%s%s\"", g_atCrLf, g_atStringTab[AT_STRING_CTZV].text);

            length += (VOS_UINT16)At_PrintTimeZoneInfo(&(netCtx->timeInfo), dst + length);
        }
    }

    /* 时区显示格式:+CTZE: "(+/-)tz,dst,yyyy/mm/dd,hh:mm:ss" */
    if ((AT_CheckRptCmdStatus(mmTimeInfo->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CTZE) == VOS_TRUE) &&
        (chkCtzeFlg == VOS_TRUE)) {
        if ((mmTimeInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
            timeZone = mmTimeInfo->universalTimeandLocalTimeZone.timeZone;
        } else {
            timeZone = mmTimeInfo->localTimeZone;
        }

        if (timeZone != netCtx->timeInfo.localTimeZone) {
            /* 保存网络下发的时区信息 */
            netCtx->timeInfo.ieFlg |= NAS_MM_INFO_IE_LTZ;
            netCtx->timeInfo.localTimeZone                          = timeZone;
            netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone = timeZone;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%s%s\"", g_atCrLf, g_atStringTab[AT_STRING_CTZE].text);

        length += (VOS_UINT16)AT_PrintTimeZoneInfoWithCtzeType(mmTimeInfo, dst + length);
    }
    /* 时间显示格式: ^TIME: "yy/mm/dd,hh:mm:ss(+/-)tz,dst" */
    if ((AT_CheckRptCmdStatus(mmTimeInfo->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_TIME) == VOS_TRUE) &&
        (chkTimeFlg == VOS_TRUE)
        && ((mmTimeInfo->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ)) {
        /* "^TIME: */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%s%s", g_atCrLf, g_atStringTab[AT_STRING_TIME].text);

        /* YY */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "\"%d%d/",
            netCtx->timeInfo.universalTimeandLocalTimeZone.year / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.year % AT_DECIMAL_BASE_NUM);
        /* MM */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%d%d/",
            netCtx->timeInfo.universalTimeandLocalTimeZone.month / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.month % AT_DECIMAL_BASE_NUM);
        /* dd */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%d%d,",
            netCtx->timeInfo.universalTimeandLocalTimeZone.day / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.day % AT_DECIMAL_BASE_NUM);

        /* hh */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%d%d:",
            netCtx->timeInfo.universalTimeandLocalTimeZone.hour / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.hour % AT_DECIMAL_BASE_NUM);

        /* mm */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%d%d:",
            netCtx->timeInfo.universalTimeandLocalTimeZone.minute / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.minute % AT_DECIMAL_BASE_NUM);

        /* ss */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)dst + length, "%d%d",
            netCtx->timeInfo.universalTimeandLocalTimeZone.second / AT_DECIMAL_BASE_NUM,
            netCtx->timeInfo.universalTimeandLocalTimeZone.second % AT_DECIMAL_BASE_NUM);

        /* GMT±tz, Summer(Winter) Time" */
        length += (VOS_UINT16)AT_PrintTimeZoneInfoNoAdjustment(mmTimeInfo, dst + length);
    }

    return length;
}

/* begin V7R1 PhaseI Modify */

VOS_UINT32 AT_GetSysModeName(MN_PH_SysModeExUint8 sysMode, VOS_CHAR *sysModeName, VOS_UINT32 maxMemLength)
{
    VOS_UINT32 i;
    errno_t    stringRet;

    for (i = 0; i < sizeof(g_sysModeTbl) / sizeof(AT_PH_SysModeTbl); i++) {
        if (g_sysModeTbl[i].sysMode == sysMode) {
            stringRet = strncpy_s(sysModeName, maxMemLength, g_sysModeTbl[i].strSysModeName,
                                  VOS_StrLen(g_sysModeTbl[i].strSysModeName));
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(stringRet, maxMemLength, VOS_StrLen(g_sysModeTbl[i].strSysModeName));
            return VOS_OK;
        }
    }

    return VOS_ERR;
}


VOS_UINT32 AT_GetSubSysModeName(MN_PH_SubSysModeExUint8 subSysMode, VOS_CHAR *subSysModeName, VOS_UINT32 maxMemLength)
{
    VOS_UINT32 i;
    errno_t    stringRet;

    for (i = 0; i < sizeof(g_subSysModeTbl) / sizeof(AT_PH_SubSysModeTbl); i++) {
        if (g_subSysModeTbl[i].subSysMode == subSysMode) {
            stringRet = strncpy_s(subSysModeName, maxMemLength, g_subSysModeTbl[i].strSubSysModeName,
                                  VOS_StrLen(g_subSysModeTbl[i].strSubSysModeName));
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(stringRet, maxMemLength, VOS_StrLen(g_subSysModeTbl[i].strSubSysModeName));

            return VOS_OK;
        }
    }

    return VOS_ERR;
}


VOS_VOID AT_QryParaRspSysinfoExProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t        memResult;
    VOS_UINT32     result;
    VOS_UINT16     length;
    VOS_CHAR       sysModeName[AT_SYS_MODE_NAME_ARRAY_LEN];
    VOS_CHAR       subSysModeName[AT_SUB_SYS_MODE_NAME_ARRAY_LEN];
    TAF_PH_Sysinfo sysInfo;
    VOS_UINT8     *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    (VOS_VOID)memset_s(&sysInfo, sizeof(sysInfo), 0x00, sizeof(TAF_PH_Sysinfo));
    memResult = memcpy_s(&sysInfo, sizeof(sysInfo), para, sizeof(TAF_PH_Sysinfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sysInfo), sizeof(TAF_PH_Sysinfo));

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
        sysInfo.srvStatus);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.srvDomain);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.roamStatus);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.simStatus);

    if (*systemAppConfig == SYSTEM_APP_WEBUI) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.simLockStatus);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",");
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.sysMode);

    (VOS_VOID)memset_s(sysModeName, sizeof(sysModeName), 0x00, sizeof(sysModeName));
    (VOS_VOID)memset_s(subSysModeName, sizeof(subSysModeName), 0x00, sizeof(subSysModeName));

    /* 获取SysMode的名字 */
    AT_GetSysModeName(sysInfo.sysMode, sysModeName, (TAF_UINT32)sizeof(sysModeName));

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", sysModeName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", sysInfo.sysSubMode);

    /* 获取SubSysMode的名字 */
    AT_GetSubSysModeName(sysInfo.sysSubMode, subSysModeName, (TAF_UINT32)sizeof(subSysModeName));

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", subSysModeName);
    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
}
/* end V7R1 PhaseI Modify */


VOS_VOID AT_QryParaAnQueryProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t                 memResult;
    VOS_UINT32              result;
    MN_MMA_AnqueryPara      anqueryPara;
    AT_CmdAntennaLevelUint8 curAntennaLevel;
    AT_ModemNetCtx         *netCtx = VOS_NULL_PTR;
#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;
    VOS_INT16  rsrp;
    VOS_INT16  rsrq;
    VOS_UINT8  level;
    VOS_INT16  rssi;
#endif

    /* 初始化 */
    result = AT_OK;

    (VOS_VOID)memset_s(&anqueryPara, sizeof(anqueryPara), 0x00, sizeof(MN_MMA_AnqueryPara));
    memResult = memcpy_s(&anqueryPara, sizeof(anqueryPara), para, sizeof(MN_MMA_AnqueryPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(anqueryPara), sizeof(MN_MMA_AnqueryPara));

    if ((anqueryPara.serviceSysMode == TAF_MMA_RAT_GSM) || (anqueryPara.serviceSysMode == TAF_MMA_RAT_WCDMA)) {
        netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

        /*
         * 上报数据转换:将 Rscp、Ecio显示为非负值，若Rscp、Ecio为-145，-32，或者rssi为99，
         * 则转换为0
         */
        if (((anqueryPara.u.st2G3GCellSignInfo.cpichRscp == 0) && (anqueryPara.u.st2G3GCellSignInfo.cpichEcNo == 0)) ||
            (anqueryPara.u.st2G3GCellSignInfo.rssi == TAF_PH_RSSIUNKNOW)) {
            /* 丢网返回0, 对应应用的圈外 */
            curAntennaLevel = AT_CMD_ANTENNA_LEVEL_0;
        } else {
            /* 调用函数AT_CalculateAntennaLevel来根据D25算法计算出信号格数 */
            curAntennaLevel = AT_CalculateAntennaLevel(anqueryPara.u.st2G3GCellSignInfo.cpichRscp,
                                                       anqueryPara.u.st2G3GCellSignInfo.cpichEcNo);
        }

        /* 信号磁滞处理 */
        AT_GetSmoothAntennaLevel(indexNum, curAntennaLevel);

        anqueryPara.u.st2G3GCellSignInfo.cpichRscp = -anqueryPara.u.st2G3GCellSignInfo.cpichRscp;
        anqueryPara.u.st2G3GCellSignInfo.cpichEcNo = -anqueryPara.u.st2G3GCellSignInfo.cpichEcNo;

#if (FEATURE_LTE == FEATURE_ON)
        systemAppConfig = AT_GetSystemAppConfigAddr();

        if (*systemAppConfig == SYSTEM_APP_WEBUI) {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d,0,0", g_parseContext[indexNum].cmdElement->cmdName,
                (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.cpichRscp,
                (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.cpichEcNo,
                (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.rssi, (VOS_INT32)netCtx->calculateAntennaLevel);


            /* 回复用户命令结果 */
            At_FormatResultData(indexNum, result);

            return;
        }
#endif
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,%d,0x%X", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.cpichRscp,
            (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.cpichEcNo, (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.rssi,
            (VOS_INT32)netCtx->calculateAntennaLevel, (VOS_INT32)anqueryPara.u.st2G3GCellSignInfo.cellId);

        /* 回复用户命令结果 */
        At_FormatResultData(indexNum, result);

        return;
    } else if (anqueryPara.serviceSysMode == TAF_MMA_RAT_LTE) {
#if (FEATURE_LTE == FEATURE_ON)
        rsrp = anqueryPara.u.st4GCellSignInfo.rsrp;
        rsrq = anqueryPara.u.st4GCellSignInfo.rsrq;
        rssi = anqueryPara.u.st4GCellSignInfo.rssi;
        level = 0;

        AT_CalculateLTESignalValue(&rssi, &level, &rsrp, &rsrq);

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:0,99,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)rssi, (VOS_INT32)level, (VOS_INT32)rsrp, (VOS_INT32)rsrq);


        /* 回复用户命令结果 */
        At_FormatResultData(indexNum, result);

        return;
#endif
    } else {
        AT_WARN_LOG("AT_QryParaAnQueryProc:WARNING: THE RAT IS INVALID!");
        return;
    }
}


VOS_VOID AT_QryParaHomePlmnProc(VOS_UINT8 indexNum, VOS_UINT8 opId, const VOS_UINT8 *para)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length;

    TAF_MMA_HplmnWithMncLen hplmn;

    /* 初始化 */
    result = AT_OK;
    length = 0;

    (VOS_VOID)memset_s(&hplmn, sizeof(hplmn), 0x00, sizeof(hplmn));

    memResult = memcpy_s(&hplmn, sizeof(hplmn), para, sizeof(TAF_MMA_HplmnWithMncLen));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(hplmn), sizeof(TAF_MMA_HplmnWithMncLen));

    /* 上报MCC和MNC */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:", (VOS_INT8 *)g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%X%X%X", (0x0f & hplmn.hplmn.mcc),
        (0x0f00 & hplmn.hplmn.mcc) >> 8, (0x0f0000 & hplmn.hplmn.mcc) >> 16);

    if (hplmn.hplmnMncLen == AT_MNC_LENGTH_TWO_BYTES) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X", (0x0f & hplmn.hplmn.mnc),
            (0x0f00 & hplmn.hplmn.mnc) >> 8);
    } else if (hplmn.hplmnMncLen == AT_MNC_LENGTH_THREE_BYTES) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X%X", (0x0f & hplmn.hplmn.mnc),
            (0x0f00 & hplmn.hplmn.mnc) >> 8, (0x0f0000 & hplmn.hplmn.mnc) >> 16);
    } else {
        AT_WARN_LOG("AT_QryParaHomePlmnProc HPLMN MNC LEN INVAILID");
    }

    g_atSendDataBuff.bufLen = length;

    /* 回复用户命令结果 */
    At_FormatResultData(indexNum, result);

    return;
}


VOS_VOID AT_PrcoPsEvtErrCode(VOS_UINT8 indexNum, TAF_PS_CauseUint32 cuase)
{
    VOS_UINT32 result;

    /* 转换错误码格式 */
    if (cuase != TAF_PS_CAUSE_SUCCESS) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 清除AT操作符, 并停止定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
}


VOS_VOID AT_LogPrintMsgProc(TAF_MntnLogPrint *msg)
{
    AT_PR_LOGI("[MDOEM:%d]%s", msg->modemId, msg->log);
    return;
}


VOS_UINT32 AT_IsBroadcastPsEvt(TAF_PS_EvtIdUint32 evtId)
{
    VOS_UINT32 i;

    for (i = 0; i < AT_ARRAY_SIZE(g_atBroadcastPsEvtTbl); i++) {
        if (evtId == g_atBroadcastPsEvtTbl[i]) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID AT_ConvertPdpContextIpAddrParaToString(TAF_PDP_DynamicPrimExt *cgdcont, VOS_UINT16 *length)
{
    VOS_CHAR acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));

    if ((cgdcont->opIpAddr == VOS_TRUE) && (cgdcont->opSubMask == VOS_TRUE)) {
        if (cgdcont->pdpAddr.pdpType == TAF_PDP_IPV4) {
            AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), cgdcont->pdpAddr.ipv4Addr);
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s", acIpv4StrTmp);

            AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), cgdcont->subnetMask.ipv4Addr);
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ".%s\"", acIpv4StrTmp);
        } else if (cgdcont->pdpAddr.pdpType == TAF_PDP_IPV6) {
            (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, cgdcont->pdpAddr.ipv6Addr,
                                                   cgdcont->subnetMask.ipv6Addr);
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", acIpv6StrTmp);
        } else {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
        }
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID AT_ConvertPdpContextAddrParaToString(VOS_UINT32 opAddr, TAF_PDP_TypeUint8 pdpType, VOS_UINT8 *number,
                                              VOS_UINT8 aucIpv6Addr[], VOS_UINT16 *stringLength)
{
    VOS_CHAR acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, TAF_MAX_IPV4_ADDR_STR_LEN);
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, AT_IPV6_ADDR_MASK_FORMAT_STR_LEN);

    if (opAddr == VOS_TRUE) {
        if ((pdpType == TAF_PDP_IPV4) || (pdpType == TAF_PDP_PPP)) {
            AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), number);
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",\"%s\"", acIpv4StrTmp);
        } else if (pdpType == TAF_PDP_IPV6) {
            (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, aucIpv6Addr, VOS_NULL_PTR);
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",\"%s\"", acIpv6StrTmp);
        } else {
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",");
        }
    } else {
        *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",");
    }

    return;
}


VOS_VOID AT_ConvertULParaToString(VOS_UINT32 opPara, VOS_UINT32 paraVal, VOS_UINT16 *length)
{
    if (opPara == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", paraVal);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID AT_ConvertULParaToXString(VOS_UINT32 opPara, VOS_UINT32 paraVal, VOS_UINT16 *length)
{
    if (opPara == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%X", paraVal);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID AT_ConvertRangeParaToString(VOS_UINT32 opPara1, VOS_UINT32 opPara2, VOS_UINT32 paraVal1, VOS_UINT32 paraVal2,
                                     VOS_UINT16 *length)
{
    if (opPara1 == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", paraVal1);
    } else if (opPara2 == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%d", paraVal1);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ".%d\"", paraVal2);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID AT_ConvertIpAddrAndMaskParaToString(TAF_TFT_QureyInfo *cgtft, VOS_UINT8 tmp2, VOS_UINT16 *length)
{
    VOS_CHAR acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));

    if (cgtft->pfInfo[tmp2].bitOpRmtIpv4AddrAndMask == VOS_TRUE) {
        AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), cgtft->pfInfo[tmp2].rmtIpv4Address);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s", acIpv4StrTmp);
        AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), cgtft->pfInfo[tmp2].rmtIpv4Mask);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ".%s\"", acIpv4StrTmp);
    } else if (cgtft->pfInfo[tmp2].bitOpRmtIpv6AddrAndMask == VOS_TRUE) {
        (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, cgtft->pfInfo[tmp2].rmtIpv6Address,
                                               cgtft->pfInfo[tmp2].rmtIpv6Mask);
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%s\"", acIpv6StrTmp);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
    }

    return;
}


VOS_VOID AT_ConvertTftSrcAddrParaToString(TAF_TFT_Ext *tftInfo, VOS_UINT16 *stringLength)
{
    VOS_CHAR acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));

    if (tftInfo->opSrcIp == VOS_TRUE) {
        if ((tftInfo->sourceIpaddr.pdpType == TAF_PDP_IPV4) || (tftInfo->sourceIpaddr.pdpType == TAF_PDP_PPP)) {
            AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), tftInfo->sourceIpaddr.ipv4Addr);
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",\"%s", acIpv4StrTmp);

            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ".");

            AT_Ipv4AddrItoa(acIpv4StrTmp, sizeof(acIpv4StrTmp), tftInfo->sourceIpMask.ipv4Addr);
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, "%s\"", acIpv4StrTmp);
        } else if (tftInfo->sourceIpaddr.pdpType == TAF_PDP_IPV6) {
            (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, tftInfo->sourceIpaddr.ipv6Addr,
                                                   tftInfo->sourceIpMask.ipv6Addr);
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",\"%s\"", acIpv6StrTmp);
        } else {
            *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",");
        }
    } else {
        *stringLength += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *stringLength, ",");
    }

    return;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_ConvertSNssaiToString(PS_S_NSSAI_STRU *sNssai, VOS_UINT16 *length)
{
    /*
     * 27007 rel15, 10.1.1章节
     * sst                                     only slice/service type (SST) is present
     * sst;mapped_sst                          SST and mapped configured SST are present
     * sst.sd                                  SST and slice differentiator (SD) are present
     * sst.sd;mapped_sst                       SST, SD and mapped configured SST are present
     * sst.sd;mapped_sst.mapped_sd             SST, SD, mapped configured SST and mapped configured SD are present
     */

    if ((sNssai->bitOpSd == VOS_TRUE) && (sNssai->bitOpMappedSst == VOS_TRUE) && (sNssai->bitOpMappedSd == VOS_TRUE)) {
        /* sst.sd;mapped_sst.mapped_sd             SST, SD, mapped configured SST and mapped configured SD are present */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%02x.%06x;%02x.%06x\"", sNssai->ucSst, sNssai->ulSd,
            sNssai->ucMappedSst, sNssai->ulMappedSd);
    } else if ((sNssai->bitOpSd == VOS_TRUE) && (sNssai->bitOpMappedSst == VOS_TRUE)) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%02x.%06x;%02x\"", sNssai->ucSst, sNssai->ulSd,
            sNssai->ucMappedSst);
    } else if (sNssai->bitOpSd == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%02x.%06x\"", sNssai->ucSst, sNssai->ulSd);
    } else if (sNssai->bitOpMappedSst == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%02x;%02x\"", sNssai->ucSst, sNssai->ucMappedSst);
    } else {
        if (sNssai->ucSst == 0) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",");
        } else {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",\"%02x\"", sNssai->ucSst);
        }
    }

    return;
}

#endif


VOS_VOID AT_RcvTafPsEvt(TAF_PS_Evt *evt)
{
    VOS_UINT32     i;
    VOS_UINT32     result;
    VOS_UINT8      indexNum = 0;
    MN_PS_EVT_FUNC evtFunc;
    TAF_Ctrl      *ctrl = VOS_NULL_PTR;

    /* 初始化 */
    result  = VOS_OK;
    evtFunc = VOS_NULL_PTR;
    ctrl    = (TAF_Ctrl *)(evt->content);

    if (At_ClientIdToUserId(ctrl->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafPsEvt: At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        /*
         * 广播IDNEX不可以作为数组下标使用，需要在事件处理函数中仔细核对，避免数组越界。
         * 目前只有流量上报/NW ACT/NW DISCONNET为广播事件，需要添加其它广播事件，请仔细核对，
         */
        if (AT_IsBroadcastPsEvt(evt->evtId) == VOS_FALSE) {
            AT_WARN_LOG("AT_RcvTafPsEvt: AT_BROADCAST_INDEX,but not Broadcast Event.");
            return;
        }
    }

    /* 在事件处理表中查找处理函数 */
    for (i = 0; i < AT_ARRAY_SIZE(g_atPsEvtFuncTbl); i++) {
        if (evt->evtId == g_atPsEvtFuncTbl[i].evtId) {
            if (TAF_RunChkMsgLenFunc((const MSG_Header *)evt, g_atPsEvtFuncTbl[i].msgLen,
                    g_atPsEvtFuncTbl[i].chkFunc) == VOS_FALSE) {
                AT_ERR_LOG("AT_RcvTafPsEvt: Check MsgLenth Err");
                return;
            }

            /* 事件ID匹配 */
            evtFunc = g_atPsEvtFuncTbl[i].evtFunc;
            break;
        }
    }

    /* 如果处理函数存在则调用 */
    if (evtFunc != VOS_NULL_PTR) {
        result = evtFunc(indexNum, (TAF_Ctrl *)evt->content);
    } else {
        AT_ERR_LOG1("AT_RcvTafPsEvt: Unexpected event received! <EvtId>", evt->evtId);
        result = VOS_ERR;
    }

    /* 根据处理函数的返回结果, 决定是否清除AT定时器以及操作符: 该阶段不考虑 */
    if (result != VOS_OK) {
        AT_ERR_LOG1("AT_RcvTafPsEvt: Can not handle this message! <MsgId>", evt->evtId);
    }

    return;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpActivateCnf *event = VOS_NULL_PTR;
    ModemIdUint16       modemId;
    DMS_PortIdUint16    portId;
    VOS_UINT16          pppId;

    /* 初始化 */
    event = (TAF_PS_CallPdpActivateCnf *)evtInfo;

    /* 记录<CID> */
    g_atClientTab[indexNum].cid = event->cid;

    /* 记录<RabId> */
    modemId = MODEM_ID_0;
    portId  = g_atClientTab[indexNum].portNo;
    pppId   = AT_PS_GET_PPPID(indexNum);

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsCallEvtPdpActivateCnf: Get modem id fail.");
        return AT_ERROR;
    }

    /* 保存为扩展RABID 等于 modemId + rabId */
    g_atClientTab[indexNum].exPsRabId = AT_BUILD_EXRABID(modemId, event->rabId);

    /* 清除PS域呼叫错误码 */
    AT_PS_SetPsCallErrCause(indexNum, TAF_PS_CAUSE_SUCCESS);

    AT_PS_AddIpAddrMap(indexNum, event);

    /* 根据操作类型 */
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CGACT_ORG_SET:
        case AT_CMD_CGANS_ANS_SET:
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_OK);
            break;

        case AT_CMD_CGDATA_SET:
            AT_STOP_TIMER_CMD_READY(indexNum);
            DMS_PORT_SwitchToPppDataMode(portId, DMS_PORT_DATA_PPP_RAW, PPP_PullRawDataEvent, pppId);
            At_FormatResultData(indexNum, AT_CONNECT);
            break;

        case AT_CMD_CGANS_ANS_EXT_SET:
            AT_AnswerPdpActInd(indexNum, event);
            break;

        case AT_CMD_D_IP_CALL_SET:
        case AT_CMD_PPP_ORG_SET:
            /*
             * Modem发起的PDP激活成功，
             * AT_CMD_D_IP_CALL_SET为PPP类型
             * AT_CMD_PPP_ORG_SET为IP类型
             */
            AT_ModemPsRspPdpActEvtCnfProc(indexNum, event);
            break;

        default:
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateRej(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpActivateRej *event = VOS_NULL_PTR;

    event = (TAF_PS_CallPdpActivateRej *)evtInfo;

    /* 记录PS域呼叫错误码 */
    AT_PS_SetPsCallErrCause(indexNum, event->cause);

    /* 按用户类型分别处理 */
    switch (g_atClientTab[indexNum].userType) {
        /* MODEM拨号处理 */
        case AT_HSUART_USER:
        case AT_MODEM_USER:
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
        case AT_CMUXAT_USER:
        case AT_CMUXMDM_USER:
        case AT_CMUXEXT_USER:
        case AT_CMUXGPS_USER:
#endif
            AT_ModemPsRspPdpActEvtRejProc(indexNum, event);
            return VOS_OK;

        /* NDIS拨号处理 */
        case AT_NDIS_USER:
        /* E5和闪电卡使用同一个端口名 */
        case AT_APP_USER:
            if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGACT_ORG_SET) {
                /* AT+CGACT拨号 */
                AT_STOP_TIMER_CMD_READY(indexNum);
                At_FormatResultData(indexNum, AT_ERROR);
            }

            return VOS_OK;

        /* 其他端口全部返回ERROR */
        default:
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, AT_ERROR);
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpManageInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                  memResult;
    VOS_UINT16               length;
    TAF_PS_CallPdpManageInd *event      = VOS_NULL_PTR;
    AT_ModemSsCtx           *modemSsCtx = VOS_NULL_PTR;
    VOS_UINT8 tempValue[TAF_MAX_APN_LEN + 1];

    /* 初始化 */
    length = 0;
    event  = (TAF_PS_CallPdpManageInd *)evtInfo;

    /* 命令与协议不符 */
    modemSsCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    if (modemSsCtx->crcType == AT_CRC_ENABLE_TYPE) {
        /* +CRC -- +CRING: GPRS <PDP_type>, <PDP_addr>[,[<L2P>][,<APN>]] */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s+CRING: GPRS ", g_atCrLf);

        /* <PDP_type> */
        if (event->pdpAddr.pdpType == TAF_PDP_IPV4) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_IP].text);
        } else if (event->pdpAddr.pdpType == TAF_PDP_IPV6) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_IPV6].text);
        } else if (event->pdpAddr.pdpType == TAF_PDP_IPV4V6) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_IPV4V6].text);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atStringTab[AT_STRING_PPP].text);
        }

        /* <PDP_addr> */
        (VOS_VOID)memset_s(tempValue, sizeof(tempValue), 0x00, sizeof(tempValue));
        AT_Ipv4Addr2Str((VOS_CHAR *)tempValue, sizeof(tempValue), event->pdpAddr.ipv4Addr, TAF_IPV4_ADDR_LEN);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", tempValue);

        /* <L2P>没有，<APN> */
        (VOS_VOID)memset_s(tempValue, sizeof(tempValue), 0x00, sizeof(tempValue));
        if (event->apn.length > sizeof(event->apn.value)) {
            AT_WARN_LOG1("AT_RcvTafPsCallEvtPdpManageInd: Invalid pstEvent->stApn.ucLength: ", event->apn.length);
            event->apn.length = sizeof(event->apn.value);
        }

        if (event->apn.length > 0) {
            memResult = memcpy_s(tempValue, sizeof(tempValue), event->apn.value, event->apn.length);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempValue), event->apn.length);
        }
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",,\"%s\"%s", tempValue, g_atCrLf);
    } else {
        /* +CRC -- RING */
        if (g_atVType == AT_V_ENTIRE_TYPE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%sRING%s", g_atCrLf, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "2\r");
        }
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpActivateInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    /* 不处理 */
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpModifyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpModifyCnf *event = VOS_NULL_PTR;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGCMOD_SET) {
        return VOS_ERR;
    }

    event = (TAF_PS_CallPdpModifyCnf *)evtInfo;

    switch (g_atClientTab[indexNum].userType) {
        case AT_HSUART_USER:
        case AT_MODEM_USER:
            /* 向FC指示修改流控点 */
            AT_NotifyFcWhenPdpModify(event, FC_ID_MODEM);
            break;

        case AT_NDIS_USER:
        case AT_APP_USER:
            AT_PS_ProcCallModifyEvent(indexNum, event);
            break;

        default:
            break;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpModifyRej(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGCMOD_SET) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_ERROR);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpModifiedInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpModifyCnf *event = VOS_NULL_PTR;

    event = (TAF_PS_CallPdpModifyInd *)evtInfo;

    switch (g_atClientTab[indexNum].userType) {
        case AT_HSUART_USER:
        case AT_MODEM_USER:
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
        case AT_CMUXAT_USER:
        case AT_CMUXMDM_USER:
        case AT_CMUXEXT_USER:
        case AT_CMUXGPS_USER:
#endif
            /* 向FC指示修改流控点 */
            AT_NotifyFcWhenPdpModify(event, FC_ID_MODEM);
            break;

        case AT_NDIS_USER:
        case AT_APP_USER:
            AT_PS_ProcCallModifyEvent(indexNum, event);
            break;

        default:
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpDeactivateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpDeactivateCnf *event = VOS_NULL_PTR;

    event = (TAF_PS_CallPdpDeactivateCnf *)evtInfo;

    AT_PS_DeleteIpAddrMap(indexNum, event);

    /* 还是应该先判断是否是数传状态，再决定处理 */
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CGACT_END_SET:
            AT_STOP_TIMER_CMD_READY(indexNum);
            DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);
            At_FormatResultData(indexNum, AT_OK);
            break;

        case AT_CMD_H_PS_SET:
        case AT_CMD_PS_DATA_CALL_END_SET:
            AT_ModemPsRspPdpDeactEvtCnfProc(indexNum, event);
            break;

        default:
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtPdpDeactivatedInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallPdpDeactivateInd *event = VOS_NULL_PTR;

    event = (TAF_PS_CallPdpDeactivateInd *)evtInfo;

    AT_PS_DeleteIpAddrMap(indexNum, event);

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGACT_END_SET) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);
        At_FormatResultData(indexNum, AT_OK);

        return VOS_OK;
    }

    /* 记录PS域呼叫错误码 */
    AT_PS_SetPsCallErrCause(indexNum, event->cause);

    switch (g_atClientTab[indexNum].userType) {
        case AT_HSUART_USER:
        case AT_USBCOM_USER:
        case AT_MODEM_USER:
        case AT_CTR_USER:
        /* 部分CPE产品由于版本一致性要求，不允许存在pcui端口 */
        case AT_SOCK_USER:
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
        case AT_CMUXAT_USER:
        case AT_CMUXMDM_USER:
        case AT_CMUXEXT_USER:
        case AT_CMUXGPS_USER:
#endif
            AT_ModemPsRspPdpDeactivatedEvtProc(indexNum, event);
            break;

        default:
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtCallEndCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallEndCnf *callEndCnf = VOS_NULL_PTR;

    /* 初始化 */
    callEndCnf = (TAF_PS_CallEndCnf *)evtInfo;

    if (AT_IsPppUser(indexNum) == VOS_TRUE) {
        AT_ModemProcCallEndCnfEvent(callEndCnf->cause, indexNum);
        return VOS_OK;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtCallModifyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallModifyCnf *callModifyCnf = VOS_NULL_PTR;

    /* 初始化 */
    callModifyCnf = (TAF_PS_CallModifyCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGCMOD_SET) {
        return VOS_ERR;
    }

    /*
     * (1)协议栈异常错误, 未发起PDP修改, 直接上报ERROR
     * (2)协议栈正常, 发起PDP修改, 根据PDP修改事件返回结果
     */
    if (callModifyCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtCallAnswerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallAnswerCnf *callAnswerCnf = VOS_NULL_PTR;
    VOS_UINT32            result;

    /* 初始化 */
    callAnswerCnf = (TAF_PS_CallAnswerCnf *)evtInfo;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafPsCallEvtCallAnswerCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 检查当前命令的操作类型 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGANS_ANS_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGANS_ANS_EXT_SET)) {
        return VOS_ERR;
    }

    /*
     * (1)协议栈异常错误, 未发起PDP应答, 直接上报ERROR
     * (2)协议栈正常, 发起PDP应答, 根据PDP激活事件返回结果
     */

    /* IP类型的应答，需要先给上层回CONNECT */
    if (callAnswerCnf->cause == TAF_ERR_AT_CONNECT) {
        result = At_SetDialGprsPara(indexNum, callAnswerCnf->cid, TAF_IP_ACTIVE_TE_PPP_MT_PPP_TYPE);

        /* 如果是connect，CmdCurrentOpt不清，At_RcvTeConfigInfoReq中使用 */
        if (result == AT_ERROR) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
        }

        AT_StopRelTimer(indexNum, &g_atClientTab[indexNum].hTimer);
        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;
        g_atClientTab[indexNum].opId          = 0;
        At_FormatResultData(indexNum, result);

        return VOS_OK;
    }

    /* 其他错误，命令返回ERROR */
    if (callAnswerCnf->cause != TAF_ERR_NO_ERROR) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsCallEvtCallHangupCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32            result;
    TAF_PS_CallHangupCnf *callHangUpCnf = VOS_NULL_PTR;

    callHangUpCnf = (TAF_PS_CallHangupCnf *)evtInfo;

    if (callHangUpCnf->cause == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 根据操作类型 */
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CGANS_ANS_SET:
            AT_STOP_TIMER_CMD_READY(indexNum);
            At_FormatResultData(indexNum, result);
            break;

        default:
            break;
    }

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvTafPsEvtSet5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_Set5gQosInfoCnf *set5gQosInfoCnf = VOS_NULL_PTR;

    set5gQosInfoCnf = (TAF_PS_Set5gQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GQOS_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, set5gQosInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGet5gQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_Get5GQosInfoCnf *qosInfo = VOS_NULL_PTR;
    const TAF_PS_Evt             *event   = VOS_NULL_PTR;
    VOS_UINT32                    length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_Get5GQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_WARN_LOG2("AT_ChkTafPsEvtGet5gQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    qosInfo = (const TAF_PS_Get5GQosInfoCnf *)(event->content);
    length += (qosInfo->cidNum * sizeof(TAF_5G_QosExt));
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_WARN_LOG2("AT_ChkTafPsEvtGet5gQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGet5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;
    VOS_UINT32 tmp    = 0;

    TAF_5G_QosExt           cg5qos;
    TAF_PS_Get5GQosInfoCnf *get5gQosInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&cg5qos, sizeof(cg5qos), 0x00, sizeof(TAF_5G_QosExt));

    get5gQosInfoCnf = (TAF_PS_Get5GQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GQOS_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < get5gQosInfoCnf->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cg5qos, sizeof(cg5qos), &get5gQosInfoCnf->ast5gQosInfo[tmp], sizeof(TAF_5G_QosExt));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cg5qos), sizeof(TAF_5G_QosExt));

        /* +C5GQOS:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cg5qos.cid);
        /* <5QI> */
        if (cg5qos.op5Qi == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cg5qos.uc5Qi);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <DL GFBR> */
        if (cg5qos.opDlgfbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cg5qos.dlgfbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <UL GFBR> */
        if (cg5qos.opUlgfbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cg5qos.ulgfbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <DL MFBR> */
        if (cg5qos.opDlmfbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cg5qos.dlmfbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <UL MFBR> */
        if (cg5qos.opUlmfbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cg5qos.ulmfbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_PrintDynamic5gQosInfo(TAF_PS_GetDynamic5GQosInfoCnf *getDynamic5gQosInfoCnf, VOS_UINT8 indexNum)
{
    TAF_PS_5GDynamicQosExt st5gDynamicQos;
    VOS_UINT16             length;
    VOS_UINT32             i;
    errno_t                memResult;

    length = 0;
    memset_s(&st5gDynamicQos, sizeof(st5gDynamicQos), 0x00, sizeof(TAF_PS_5GDynamicQosExt));

    /* AT命令格式: [+C5GQOSRDP:
     * <cid>,<5QI>[,<DL_GFBR>,<UL_GFBR>[,<DL_MFBR>,<UL_MFBR>[,<DL_SAMBR>,<UL_SAMBR>[,<Averaging_window>]]]]] */
    for (i = 0; i < getDynamic5gQosInfoCnf->cidNum; i++) {
        if (i != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&st5gDynamicQos, sizeof(st5gDynamicQos), &getDynamic5gQosInfoCnf->ast5gQosInfo[i],
                             sizeof(TAF_PS_5GDynamicQosExt));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(st5gDynamicQos), sizeof(TAF_PS_5GDynamicQosExt));

        /* +C5GQOSRDP:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, "%d", st5gDynamicQos.cid);

        /* <QCI> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", st5gDynamicQos.uc5Qi);

        /* 如果各个参数如果存在就打印出来，目前按照这种方式打印出来 */
        /* <DL GFBR> */
        AT_ConvertULParaToString(st5gDynamicQos.opDlgfbr, st5gDynamicQos.dlgfbr, &length);

        /* <UL GFBR> */
        AT_ConvertULParaToString(st5gDynamicQos.opUlgfbr, st5gDynamicQos.ulgfbr, &length);

        /* <DL MFBR> */
        AT_ConvertULParaToString(st5gDynamicQos.opDlmfbr, st5gDynamicQos.dlmfbr, &length);

        /* <UL MFBR> */
        AT_ConvertULParaToString(st5gDynamicQos.opUlmfbr, st5gDynamicQos.ulmfbr, &length);

        /* <Session ambr> */
        if (st5gDynamicQos.opSambr == VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d",
                                             st5gDynamicQos.ambr.dlSessionAmbr);

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d",
                                             st5gDynamicQos.ambr.ulSessionAmbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",,");
        }

        /* <Average Window> */
        if (st5gDynamicQos.opAveragWindow == VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d",
                                             st5gDynamicQos.averagWindow);
        }
    }

    g_atSendDataBuff.bufLen = length;

    return;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamic5gQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetDynamic5GQosInfoCnf *epsQos = VOS_NULL_PTR;
    const TAF_PS_Evt                    *event = VOS_NULL_PTR;
    VOS_UINT32                           length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamic5GQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamic5gQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    epsQos  = (const TAF_PS_GetDynamic5GQosInfoCnf *)(event->content);
    length += sizeof(TAF_PS_5GDynamicQosExt) * epsQos->cidNum;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamic5gQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamic5gQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDynamic5GQosInfoCnf *getDynamic5gQosInfoCnf = VOS_NULL_PTR;
    VOS_UINT32                     result                 = AT_FAILURE;

    getDynamic5gQosInfoCnf = (TAF_PS_GetDynamic5GQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GQOSRDP_SET) {
        return VOS_ERR;
    }

    if (getDynamic5gQosInfoCnf->cause == TAF_PS_CAUSE_SUCCESS) {
        AT_PrintDynamic5gQosInfo(getDynamic5gQosInfoCnf, indexNum);
        result = AT_OK;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtUePolicyRptInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_UePolicyRptInd *uePolicyInd = VOS_NULL_PTR;
    VOS_UINT16             length;
    ModemIdUint16          modemId;

    uePolicyInd = (TAF_PS_UePolicyRptInd *)evtInfo;
    modemId     = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtUePolicyRptInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ^CPOLICYRPT: <total_len>,<section_num>,<protocal_ver> */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CPOLICYRPT: %d,%d,%04x%s", g_atCrLf, uePolicyInd->totalLength,
        uePolicyInd->sectionNum, uePolicyInd->protocalVer, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetUePolicyRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetUePolicyRptCnf *setUePolicyRptCnf = VOS_NULL_PTR;

    setUePolicyRptCnf = (TAF_PS_SetUePolicyRptCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOLICYRPT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setUePolicyRptCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetUePolicyCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetUePolicyCnf     *uePolicyCnf = VOS_NULL_PTR;
    const TAF_PS_Evt                *event = VOS_NULL_PTR;
    VOS_UINT32                       length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetUePolicyCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUePolicyCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event       = (const TAF_PS_Evt *)msg;
    uePolicyCnf = (const TAF_PS_GetUePolicyCnf *)(event->content);
    length     += uePolicyCnf->length;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUePolicyCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetUePolicyCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetUePolicyCnf *getUePolicytCnf = VOS_NULL_PTR;
    VOS_UINT16             length;

    getUePolicytCnf = (TAF_PS_GetUePolicyCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOLICYCODE_QRY) {
        return VOS_ERR;
    }

    if (getUePolicytCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        /* 处理错误码 */
        AT_PrcoPsEvtErrCode(indexNum, getUePolicytCnf->cause);

        return VOS_OK;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d,", g_atCrLf, g_parseContext[indexNum].cmdElement->cmdName,
        getUePolicytCnf->index);

    /* 将16进制数转换为ASCII码 */
    length += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                       (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                       getUePolicytCnf->length, getUePolicytCnf->content);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtUePolicyRspCheckRsltInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_UePolicyRspCheckRsltInd *checkRsltInd = VOS_NULL_PTR;
    VOS_UINT32                      result;

    checkRsltInd = (TAF_PS_UePolicyRspCheckRsltInd *)evtInfo;

    /* AT模块在等待^CSUEPOLICY命令的操作结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSUEPOLICY_SET) {
        AT_WARN_LOG("AT_RcvTafPsEvtUePolicyRspCheckRsltInd: WARNING:Not AT_CMD_CSUEPOLICY_SET!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (checkRsltInd->rslt == TAF_PS_UE_POLICY_RSP_CHECK_RSLT_SUCC) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEtherSessCapInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_EtherSessCapInd *etherSessCapInd = (TAF_PS_EtherSessCapInd *)evtInfo;

    AT_GetCommPsCtxAddr()->etherCap = etherSessCapInd->enable;
    return VOS_OK;
}

#endif


VOS_UINT32 AT_RcvTafPsEvtSetPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetPrimPdpContextInfoCnf *setPdpCtxInfoCnf = VOS_NULL_PTR;

    setPdpCtxInfoCnf = (TAF_PS_SetPrimPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDCONT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setPdpCtxInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetPrimPdpContextInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetPrimPdpContextInfoCnf *pdpContxt = VOS_NULL_PTR;
    const TAF_PS_Evt                      *event     = VOS_NULL_PTR;
    VOS_UINT32                             length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetPrimPdpContextInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPrimPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event     = (const TAF_PS_Evt *)msg;
    pdpContxt = (const TAF_PS_GetPrimPdpContextInfoCnf *)(event->content);
    length   += (sizeof(TAF_PRI_PdpQueryInfo) * pdpContxt->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPrimPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                          memResult;
    VOS_UINT32                       result;
    VOS_UINT16                       length = 0;
    VOS_UINT32                       tmp    = 0;
    TAF_PRI_PdpQueryInfo             cgdcont;
    TAF_PS_GetPrimPdpContextInfoCnf *getPrimPdpCtxInfoCnf = VOS_NULL_PTR;

    VOS_UINT8 str[TAF_MAX_APN_LEN + 1];

    memset_s(&cgdcont, sizeof(cgdcont), 0x00, sizeof(TAF_PRI_PdpQueryInfo));
    getPrimPdpCtxInfoCnf = (TAF_PS_GetPrimPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDCONT_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < getPrimPdpCtxInfoCnf->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgdcont, sizeof(cgdcont), &getPrimPdpCtxInfoCnf->pdpContextQueryInfo[tmp],
                             sizeof(TAF_PRI_PdpQueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgdcont), sizeof(TAF_PRI_PdpQueryInfo));

        /* +CGDCONT:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgdcont.cid);
        /* <PDP_type> */
        if (cgdcont.priPdpInfo.pdpAddr.pdpType == TAF_PDP_IPV4) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_IP].text);
        } else if (cgdcont.priPdpInfo.pdpAddr.pdpType == TAF_PDP_IPV6) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_IPV6].text);
        } else if (cgdcont.priPdpInfo.pdpAddr.pdpType == TAF_PDP_IPV4V6) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_IPV4V6].text);
        } else if (cgdcont.priPdpInfo.pdpAddr.pdpType == TAF_PDP_ETHERNET) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_Ethernet].text);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_PPP].text);
        }
        /* <APN> */
        (VOS_VOID)memset_s(str, sizeof(str), 0x00, sizeof(str));

        if (cgdcont.priPdpInfo.apn.length > sizeof(cgdcont.priPdpInfo.apn.value)) {
            AT_WARN_LOG1("AT_RcvTafPsEvtGetPrimPdpContextInfoCnf: stCgdcont.stPriPdpInfo.stApn.ucLength: ",
                         cgdcont.priPdpInfo.apn.length);
            cgdcont.priPdpInfo.apn.length = sizeof(cgdcont.priPdpInfo.apn.value);
        }

        if (cgdcont.priPdpInfo.apn.length > 0) {
            memResult = memcpy_s(str, sizeof(str), cgdcont.priPdpInfo.apn.value, cgdcont.priPdpInfo.apn.length);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(str), cgdcont.priPdpInfo.apn.length);
        }
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", str);
        /* <PDP_addr> */
        (VOS_VOID)memset_s(str, sizeof(str), 0x00, sizeof(str));
        AT_Ipv4Addr2Str((VOS_CHAR *)str, sizeof(str), cgdcont.priPdpInfo.pdpAddr.ipv4Addr, TAF_IPV4_ADDR_LEN);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", str);
        /* <d_comp> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.pdpDcomp);
        /* <h_comp> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.pdpHcomp);

        /* <IPv4AddrAlloc>  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.ipv4AddrAlloc);
        /* <Emergency Indication> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.emergencyInd);
        /* <P-CSCF_discovery> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.pcscfDiscovery);
        /* <IM_CN_Signalling_Flag_Ind> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.imCnSignalFlg);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.nasSigPrioInd);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        /* the following five parameters omit: securePCO, Ipv4_mtu_discovery, Local_Addr_ind, Non-Ip_Mtu_discovery and
         * Reliable data service */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, "%s", ",,,,,");

        /* ssc mode */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.sscMode);
        /* SNssai */

        AT_ConvertSNssaiToString(&cgdcont.priPdpInfo.sNssai, &length);

        /* Pref Access Type */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d",
                                         cgdcont.priPdpInfo.prefAccessType);
        /* Reflect Qos Ind */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.rQosInd);
        /* Ipv6 multi-homing */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.priPdpInfo.mh6Pdu);
        /* Always on ind */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                         (VOS_CHAR *)g_atSndCodeAddress + length, ",%d",
                                         cgdcont.priPdpInfo.alwaysOnInd);
#endif
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32                   result;
    VOS_UINT16                   length;
    VOS_UINT32                   tmp;
    TAF_PS_GetPdpContextInfoCnf *getPdpCtxInfoCnf = VOS_NULL_PTR;

    result           = AT_FAILURE;
    length           = 0;
    tmp              = 0;
    getPdpCtxInfoCnf = (TAF_PS_GetPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGPADDR_TEST) {
        return VOS_ERR;
    }

    /* +CGPADDR:  */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "(");

    for (tmp = 0; tmp < getPdpCtxInfoCnf->cidNum; tmp++) {
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", getPdpCtxInfoCnf->cid[tmp]);

        if ((tmp + 1) >= getPdpCtxInfoCnf->cidNum) {
            break;
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ")");

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetSecPdpContextInfoCnf *setPdpCtxInfoCnf = VOS_NULL_PTR;

    setPdpCtxInfoCnf = (TAF_PS_SetSecPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDSCONT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setPdpCtxInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetSecPdpContextInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetSecPdpContextInfoCnf *pdpContxt = VOS_NULL_PTR;
    const TAF_PS_Evt                     *event     = VOS_NULL_PTR;
    VOS_UINT32                            length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetSecPdpContextInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetSecPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event     = (const TAF_PS_Evt *)msg;
    pdpContxt = (const TAF_PS_GetSecPdpContextInfoCnf *)(event->content);
    length   += (sizeof(TAF_PDP_SecContext) * pdpContxt->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetSecPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                         memResult;
    VOS_UINT32                      result;
    VOS_UINT16                      length = 0;
    VOS_UINT32                      tmp    = 0;
    TAF_PDP_SecContext              secPdpInfo;
    TAF_PS_GetSecPdpContextInfoCnf *getSecPdpCtxInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&secPdpInfo, sizeof(secPdpInfo), 0x00, sizeof(TAF_PDP_SecContext));

    getSecPdpCtxInfoCnf = (TAF_PS_GetSecPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDSCONT_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < getSecPdpCtxInfoCnf->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&secPdpInfo, sizeof(secPdpInfo), &getSecPdpCtxInfoCnf->pdpContextQueryInfo[tmp],
                             sizeof(TAF_PDP_SecContext));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(secPdpInfo), sizeof(TAF_PDP_SecContext));
        /* +CGDSCONT:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", secPdpInfo.cid);
        /* <p_cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", secPdpInfo.linkdCid);
        /* <d_comp> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", secPdpInfo.pdpDcomp);
        /* <h_comp> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", secPdpInfo.pdpHcomp);
        /* <IM_CN_Signalling_Flag_Ind> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", secPdpInfo.imCnSignalFlg);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetTftInfoCnf *setTftInfoCnf = VOS_NULL_PTR;

    setTftInfoCnf = (TAF_PS_SetTftInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGTFT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setTftInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetTftInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetTftInfoCnf *pdpContxt = VOS_NULL_PTR;
    const TAF_PS_Evt           *event     = VOS_NULL_PTR;
    VOS_UINT32                  length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetTftInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetTftInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event     = (const TAF_PS_Evt *)msg;
    pdpContxt = (const TAF_PS_GetTftInfoCnf *)(event->content);
    length   += (sizeof(TAF_TFT_QureyInfo) * pdpContxt->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetTftInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t               memResult;
    VOS_UINT32            result;
    VOS_UINT16            length = 0;
    VOS_UINT32            tmp1   = 0;
    VOS_UINT8             tmp2   = 0;
    VOS_CHAR              acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR              acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];
    VOS_CHAR              localIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_UINT8             localIpv6Mask[APP_MAX_IPV6_ADDR_LEN];
    TAF_TFT_QureyInfo    *cgtft         = VOS_NULL_PTR;
    TAF_PS_GetTftInfoCnf *getTftInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));
    (VOS_VOID)memset_s(localIpv4StrTmp, sizeof(localIpv4StrTmp), 0x00, sizeof(localIpv4StrTmp));
    (VOS_VOID)memset_s(localIpv6Mask, sizeof(localIpv6Mask), 0x00, sizeof(localIpv6Mask));

    getTftInfoCnf = (TAF_PS_GetTftInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGTFT_READ) {
        return VOS_ERR;
    }

    /* 动态申请内存 */
    cgtft = (TAF_TFT_QureyInfo *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TAF_TFT_QureyInfo));
    if (cgtft == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    (VOS_VOID)memset_s(cgtft, sizeof(TAF_TFT_QureyInfo), 0x00, sizeof(TAF_TFT_QureyInfo));

    for (tmp1 = 0; tmp1 < getTftInfoCnf->cidNum; tmp1++) {
        memResult = memcpy_s(cgtft, sizeof(TAF_TFT_QureyInfo), &getTftInfoCnf->tftQueryInfo[tmp1],
                             sizeof(TAF_TFT_QureyInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_TFT_QureyInfo), sizeof(TAF_TFT_QureyInfo));

        cgtft->pfNum = AT_MIN(cgtft->pfNum, TAF_MAX_SDF_PF_NUM);
        for (tmp2 = 0; tmp2 < cgtft->pfNum; tmp2++) {
            if (!(tmp1 == 0 && tmp2 == 0)) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }
            /* +CGTFT:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            /* <cid> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgtft->cid);
            /* <packet filter identifier> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgtft->pfInfo[tmp2].packetFilterId);
            /* <evaluation precedence index> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgtft->pfInfo[tmp2].precedence);
            /* <source address and subnet mask> */
            AT_ConvertIpAddrAndMaskParaToString(cgtft, tmp2, &length);

            /* <protocol number (ipv4) / next header (ipv6)> */
            AT_ConvertULParaToString(cgtft->pfInfo[tmp2].bitOpProtocolId, cgtft->pfInfo[tmp2].protocolId, &length);

            /* <destination port range> */
            AT_ConvertRangeParaToString(cgtft->pfInfo[tmp2].bitOpSingleLocalPort,
                                        cgtft->pfInfo[tmp2].bitOpLocalPortRange, cgtft->pfInfo[tmp2].lcPortLowLimit,
                                        cgtft->pfInfo[tmp2].lcPortHighLimit, &length);

            /* <source port range> */
            AT_ConvertRangeParaToString(cgtft->pfInfo[tmp2].bitOpSingleRemotePort,
                                        cgtft->pfInfo[tmp2].bitOpRemotePortRange, cgtft->pfInfo[tmp2].rmtPortLowLimit,
                                        cgtft->pfInfo[tmp2].rmtPortHighLimit, &length);

            /* <ipsec security parameter index (spi)> */
            if (cgtft->pfInfo[tmp2].bitOpSecuParaIndex == 1) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%X\"", cgtft->pfInfo[tmp2].secuParaIndex);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }

            /* <type of service (tos) (ipv4) and mask / traffic class (ipv6) and mask> */
            AT_ConvertRangeParaToString(VOS_FALSE, cgtft->pfInfo[tmp2].bitOpTypeOfService,
                                        cgtft->pfInfo[tmp2].typeOfService, cgtft->pfInfo[tmp2].typeOfServiceMask,
                                        &length);

            /* <flow label (ipv6)> */
            if (cgtft->pfInfo[tmp2].bitOpFlowLabelType == VOS_TRUE) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%X", cgtft->pfInfo[tmp2].flowLabelType);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }
            /* <direction> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgtft->pfInfo[tmp2].direction);

            if (AT_IsSupportReleaseRst(AT_ACCESS_STRATUM_REL11)) {
                /* <local address and subnet mask> */
                if (cgtft->pfInfo[tmp2].bitOpLocalIpv4AddrAndMask == VOS_TRUE) {
                    AT_Ipv4AddrItoa(localIpv4StrTmp, sizeof(localIpv4StrTmp), cgtft->pfInfo[tmp2].localIpv4Addr);
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s", localIpv4StrTmp);

                    AT_Ipv4AddrItoa(localIpv4StrTmp, sizeof(localIpv4StrTmp), cgtft->pfInfo[tmp2].localIpv4Mask);
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ".%s\"", localIpv4StrTmp);
                } else if (cgtft->pfInfo[tmp2].bitOpLocalIpv6AddrAndMask == VOS_TRUE) {
                    AT_GetIpv6MaskByPrefixLength(cgtft->pfInfo[tmp2].localIpv6Prefix, localIpv6Mask);
                    (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, cgtft->pfInfo[tmp2].localIpv6Addr,
                                                           localIpv6Mask);
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIpv6StrTmp);
                } else {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                }

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
                AT_ConvertULParaToString(cgtft->pfInfo[tmp2].bitOpQri, cgtft->pfInfo[tmp2].qri, &length);
#endif
            }
        }
    }

    /* 释放动态申请的内存 */
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, cgtft);
    /*lint -restore */
    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    /* 处理错误码 */
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetUmtsQosInfoCnf *setUmtsQosInfoCnf = VOS_NULL_PTR;

    setUmtsQosInfoCnf = (TAF_PS_SetUmtsQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQREQ_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setUmtsQosInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetUmtsQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetUmtsQosInfoCnf *qosInfo = VOS_NULL_PTR;
    const TAF_PS_Evt               *event   = VOS_NULL_PTR;
    VOS_UINT32                      length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetUmtsQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUmtsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    qosInfo = (const TAF_PS_GetUmtsQosInfoCnf *)(event->content);
    length += (sizeof(TAF_UMTS_QosQueryInfo) * qosInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUmtsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                   memResult;
    TAF_UINT32                result;
    VOS_UINT16                length      = 0;
    TAF_UINT32                tmp         = 0;
    TAF_PS_GetUmtsQosInfoCnf *umtsQosInfo = VOS_NULL_PTR;
    TAF_UMTS_QosQueryInfo     cgeq;

    (VOS_VOID)memset_s(&cgeq, sizeof(cgeq), 0x00, sizeof(TAF_UMTS_QosQueryInfo));

    umtsQosInfo = (TAF_PS_GetUmtsQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQREQ_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < umtsQosInfo->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgeq, sizeof(cgeq), &umtsQosInfo->umtsQosQueryInfo[tmp], sizeof(TAF_UMTS_QosQueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgeq), sizeof(TAF_UMTS_QosQueryInfo));
        /* +CGEQREQ:+CGEQMIN   */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgeq.cid);
        /* <Traffic class> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.trafficClass);
        /* <Maximum bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitUl);
        /* <Maximum bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitDl);
        /* <Guaranteed bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitUl);
        /* <Guaranteed bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitDl);
        /* <Delivery order> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverOrder);
        /* <Maximum SDU size> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxSduSize);
        /* <SDU error ratio> */
        switch (cgeq.qosInfo.sduErrRatio) {
            case AT_SDU_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_7E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E1].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Residual bit error ratio> */
        switch (cgeq.qosInfo.residualBer) {
            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_4E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_8:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_9:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_6E8].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Delivery of erroneous SDUs> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverErrSdu);
        /* <Transfer delay> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.transDelay);
        /* <Traffic handling priority> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.traffHandlePrior);

        /* <Source Statistics Descriptor> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.srcStatisticsDescriptor);
        /* <Signalling Indication> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.signallingIndication);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetUmtsQosMinInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetUmtsQosMinInfoCnf *setUmtsQosMinInfoCnf = VOS_NULL_PTR;

    setUmtsQosMinInfoCnf = (TAF_PS_SetUmtsQosMinInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQMIN_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setUmtsQosMinInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetUmtsQosMinInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetUmtsQosMinInfoCnf *qosInfo = VOS_NULL_PTR;
    const TAF_PS_Evt                  *event   = VOS_NULL_PTR;
    VOS_UINT32                         length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetUmtsQosMinInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUmtsQosMinInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    qosInfo = (const TAF_PS_GetUmtsQosMinInfoCnf *)(event->content);
    length += (sizeof(TAF_UMTS_QosQueryInfo) * qosInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetUmtsQosMinInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetUmtsQosMinInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                      memResult;
    TAF_UINT32                   result;
    TAF_UINT16                   length         = 0;
    TAF_UINT32                   tmp            = 0;
    TAF_PS_GetUmtsQosMinInfoCnf *umtsQosMinInfo = VOS_NULL_PTR;
    TAF_UMTS_QosQueryInfo        cgeq;

    (VOS_VOID)memset_s(&cgeq, sizeof(cgeq), 0x00, sizeof(TAF_UMTS_QosQueryInfo));

    umtsQosMinInfo = (TAF_PS_GetUmtsQosMinInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQMIN_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < umtsQosMinInfo->cidNum; tmp++) {
        if (tmp != 0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgeq, sizeof(cgeq), &umtsQosMinInfo->umtsQosQueryInfo[tmp],
                             sizeof(TAF_UMTS_QosQueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgeq), sizeof(TAF_UMTS_QosQueryInfo));
        /* +CGEQREQ:+CGEQMIN   */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d", cgeq.cid);
        /* <Traffic class> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.trafficClass);
        /* <Maximum bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitUl);
        /* <Maximum bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitDl);
        /* <Guaranteed bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitUl);
        /* <Guaranteed bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitDl);
        /* <Delivery order> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverOrder);
        /* <Maximum SDU size> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxSduSize);
        /* <SDU error ratio> */
        switch (cgeq.qosInfo.sduErrRatio) {
            case AT_SDU_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_7E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E1].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Residual bit error ratio> */
        switch (cgeq.qosInfo.residualBer) {
            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_4E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_8:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_9:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_6E8].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Delivery of erroneous SDUs> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverErrSdu);
        /* <Transfer delay> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.transDelay);
        /* <Traffic handling priority> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.traffHandlePrior);

        /* <Source Statistics Descriptor> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.srcStatisticsDescriptor);
        /* <Signalling Indication> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.signallingIndication);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamicUmtsQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetDynamicUmtsQosInfoCnf *qosInfo = VOS_NULL_PTR;
    const TAF_PS_Evt                      *event   = VOS_NULL_PTR;
    VOS_UINT32                             length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamicUmtsQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicUmtsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    qosInfo = (const TAF_PS_GetDynamicUmtsQosInfoCnf *)(event->content);
    length += (sizeof(TAF_UMTS_QosQueryInfo) * qosInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicUmtsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicUmtsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                          memResult;
    VOS_UINT32                       result;
    VOS_UINT16                       length            = 0;
    VOS_UINT32                       tmp               = 0;
    TAF_PS_GetDynamicUmtsQosInfoCnf *dynUmtsQosMinInfo = VOS_NULL_PTR;
    TAF_UMTS_QosQueryInfo            cgeq;

    (VOS_VOID)memset_s(&cgeq, sizeof(cgeq), 0x00, sizeof(TAF_UMTS_QosQueryInfo));

    dynUmtsQosMinInfo = (TAF_PS_GetDynamicUmtsQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQNEG_SET) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < dynUmtsQosMinInfo->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgeq, sizeof(cgeq), &dynUmtsQosMinInfo->umtsQosQueryInfo[tmp],
                             sizeof(TAF_UMTS_QosQueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgeq), sizeof(TAF_UMTS_QosQueryInfo));
        /* +CGEQREQ:+CGEQMIN   */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgeq.cid);
        /* <Traffic class> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.trafficClass);
        /* <Maximum bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitUl);
        /* <Maximum bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxBitDl);
        /* <Guaranteed bitrate UL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitUl);
        /* <Guaranteed bitrate DL> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.guarantBitDl);
        /* <Delivery order> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverOrder);
        /* <Maximum SDU size> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.maxSduSize);
        /* <SDU error ratio> */
        switch (cgeq.qosInfo.sduErrRatio) {
            case AT_SDU_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_7E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_SDU_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E1].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Residual bit error ratio> */
        switch (cgeq.qosInfo.residualBer) {
            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_0:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_0E0].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_1:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_2:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E2].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_3:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_5E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_4:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_4E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_5:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E3].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_6:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E4].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_7:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E5].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_8:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_1E6].text);
                break;

            case AT_RESIDUAL_BIT_ERR_RATIO_STATUS_9:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_6E8].text);
                break;

            default:
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                break;
        }
        /* <Delivery of erroneous SDUs> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.deliverErrSdu);
        /* <Transfer delay> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.transDelay);
        /* <Traffic handling priority> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeq.qosInfo.traffHandlePrior);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetPdpStateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetPdpStateCnf *setPdpStateCnf = VOS_NULL_PTR;

    setPdpStateCnf = (TAF_PS_SetPdpStateCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGACT_ORG_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGACT_END_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDATA_SET)) {
        return VOS_ERR;
    }

    /*
     * (1)协议栈异常错误, 未发起PDP激活, 直接上报ERROR
     * (2)协议栈正常, 发起PDP激活, 根据PDP激活事件返回结果
     */

    if (setPdpStateCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        /* 记录PS域呼叫错误码 */
        AT_PS_SetPsCallErrCause(indexNum, setPdpStateCnf->cause);

        AT_STOP_TIMER_CMD_READY(indexNum);

        if (setPdpStateCnf->cause == TAF_PS_CAUSE_PDP_ACTIVATE_LIMIT) {
            At_FormatResultData(indexNum, AT_CME_PDP_ACT_LIMIT);
        } else {
            At_FormatResultData(indexNum, AT_ERROR);
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtCgactQryCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                memResult;
    VOS_UINT16             length = 0;
    VOS_UINT32             tmp    = 0;
    TAF_CID_State          cgact;
    TAF_PS_GetPdpStateCnf *pdpState = VOS_NULL_PTR;

    pdpState = (TAF_PS_GetPdpStateCnf *)evtInfo;

    (VOS_VOID)memset_s(&cgact, sizeof(cgact), 0x00, sizeof(TAF_CID_State));

    /* 检查当前命令的操作类型 */
    for (tmp = 0; tmp < pdpState->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgact, sizeof(cgact), &pdpState->cidStateInfo[tmp], sizeof(TAF_CID_State));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgact), sizeof(TAF_CID_State));
        /* +CGACT:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgact.cid);
        /* <state> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgact.state);
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtCgeqnegTestCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                memResult;
    VOS_UINT16             length;
    VOS_UINT32             qosnegNum;
    VOS_UINT32             tmp;
    TAF_CID_State          cgact;
    TAF_PS_GetPdpStateCnf *pdpState = VOS_NULL_PTR;

    length    = 0;
    qosnegNum = 0;
    pdpState  = (TAF_PS_GetPdpStateCnf *)evtInfo;

    (VOS_VOID)memset_s(&cgact, sizeof(cgact), 0x00, sizeof(TAF_CID_State));

    /* CGEQNEG的测试命令 */

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", "(");

    for (tmp = 0; tmp < pdpState->cidNum; tmp++) {
        memResult = memcpy_s(&cgact, sizeof(cgact), &pdpState->cidStateInfo[tmp], sizeof(TAF_CID_State));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgact), sizeof(TAF_CID_State));

        if (cgact.state == TAF_PDP_ACTIVE) { /* 如果该CID是激活态,则打印该CID和可能的一个逗号;否则跳过该CID */
            if (qosnegNum == 0) {            /* 如果是第一个CID，则CID前不打印逗号 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%d", cgact.cid);
            } else { /* 如果不是第一个CID，则CID前打印逗号 */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", cgact.cid);
            }

            qosnegNum++;
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", ")");

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetPdpStateCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetPdpStateCnf *pdpState = VOS_NULL_PTR;
    const TAF_PS_Evt            *event    = VOS_NULL_PTR;
    VOS_UINT32                   length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetPdpStateCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpStateCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event    = (const TAF_PS_Evt *)msg;
    pdpState = (const TAF_PS_GetPdpStateCnf *)(event->content);
    length  += sizeof(TAF_CID_State) * pdpState->cidNum;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpStateCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetPdpStateCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGACT_READ) {
        return AT_RcvTafPsEvtCgactQryCnf(indexNum, evtInfo);
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGEQNEG_TEST) {
        return AT_RcvTafPsEvtCgeqnegTestCnf(indexNum, evtInfo);

    } else {
        return VOS_ERR;
    }
}

VOS_UINT32 AT_ChkTafPsEvtGetPdpIpAddrInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetPdpIpAddrInfoCnf *ipAddrInfo = VOS_NULL_PTR;
    const TAF_PS_Evt                 *event      = VOS_NULL_PTR;
    VOS_UINT32                        length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetPdpIpAddrInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpIpAddrInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event      = (const TAF_PS_Evt *)msg;
    ipAddrInfo = (const TAF_PS_GetPdpIpAddrInfoCnf *)(event->content);
    length    += (sizeof(TAF_PDP_AddrQueryInfo) * ipAddrInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpIpAddrInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetPdpIpAddrInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                     memResult;
    VOS_UINT16                  length = 0;
    VOS_UINT32                  tmp    = 0;
    VOS_CHAR                    aStrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR                    acIPv6Str[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];
    TAF_PDP_AddrQueryInfo       pdpAddrQuery;
    TAF_PS_GetPdpIpAddrInfoCnf *pdpIpAddr = VOS_NULL_PTR;

    /* 初始化 */
    pdpIpAddr = (TAF_PS_GetPdpIpAddrInfoCnf *)evtInfo;
    (VOS_VOID)memset_s(aStrTmp, sizeof(aStrTmp), 0x00, sizeof(aStrTmp));
    (VOS_VOID)memset_s(acIPv6Str, sizeof(acIPv6Str), 0x00, sizeof(acIPv6Str));
    (VOS_VOID)memset_s(&pdpAddrQuery, sizeof(pdpAddrQuery), 0x00, sizeof(pdpAddrQuery));

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGPADDR_SET) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < pdpIpAddr->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&pdpAddrQuery, sizeof(pdpAddrQuery), &pdpIpAddr->pdpAddrQueryInfo[tmp],
                             sizeof(TAF_PDP_AddrQueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pdpAddrQuery), sizeof(TAF_PDP_AddrQueryInfo));

        /* +CGPADDR:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", pdpAddrQuery.cid);

        /* <PDP_addr> */
        if ((pdpAddrQuery.pdpAddr.pdpType == TAF_PDP_IPV4) || (pdpAddrQuery.pdpAddr.pdpType == TAF_PDP_PPP)) {
            AT_Ipv4AddrItoa(aStrTmp, sizeof(aStrTmp), pdpAddrQuery.pdpAddr.ipv4Addr);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", aStrTmp);
        } else if (pdpAddrQuery.pdpAddr.pdpType == TAF_PDP_IPV6) {
            (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIPv6Str, pdpAddrQuery.pdpAddr.ipv6Addr, VOS_NULL_PTR);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIPv6Str);
        } else if (pdpAddrQuery.pdpAddr.pdpType == TAF_PDP_IPV4V6) {
            AT_Ipv4AddrItoa(aStrTmp, sizeof(aStrTmp), pdpAddrQuery.pdpAddr.ipv4Addr);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", aStrTmp);

            (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIPv6Str, pdpAddrQuery.pdpAddr.ipv6Addr, VOS_NULL_PTR);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIPv6Str);
        } else {
            /* TAF_PDP_TYPE_BUTT */
            return VOS_ERR;
        }
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetAnsModeInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetAnswerModeInfoCnf *setAnsModeInfoCnf = VOS_NULL_PTR;

    setAnsModeInfoCnf = (TAF_PS_SetAnswerModeInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGAUTO_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setAnsModeInfoCnf->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetAnsModeInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT16                   length;
    TAF_PS_GetAnswerModeInfoCnf *callAns = VOS_NULL_PTR;

    /* 初始化 */
    length  = 0;
    callAns = (TAF_PS_GetAnswerModeInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGAUTO_READ) {
        return VOS_ERR;
    }

    /* +CGAUTO */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", callAns->ansMode);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamicPrimPdpContextInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_Evt                             *event = VOS_NULL_PTR;
    const TAF_PS_GetDynamicPrimPdpContextInfoCnf *pdpContxt = VOS_NULL_PTR;
    VOS_UINT32                                    length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamicPrimPdpContextInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicPrimPdpContextInfoCnfMsgLen: msgLen, structLen.",
            VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event     = (const TAF_PS_Evt *)msg;
    pdpContxt = (const TAF_PS_GetDynamicPrimPdpContextInfoCnf *)(event->content);
    length   += (sizeof(TAF_PDP_DynamicPrimExt) * pdpContxt->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicPrimPdpContextInfoCnfMsgLen: msgLen, structLen.",
            VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicPrimPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t    memResult;
    VOS_UINT32 result = AT_FAILURE;
    VOS_UINT32 tmp;
    VOS_UINT16 length = 0;
    VOS_CHAR   acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR   acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];

    VOS_UINT8 str[TAF_MAX_APN_LEN + 1];

    TAF_PDP_DynamicPrimExt                  cgdcont;
    TAF_PS_GetDynamicPrimPdpContextInfoCnf *getDynamicPdpCtxInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));
    (VOS_VOID)memset_s(&cgdcont, sizeof(cgdcont), 0x00, sizeof(cgdcont));

    getDynamicPdpCtxInfoCnf = (TAF_PS_GetDynamicPrimPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGCONTRDP_SET) {
        return VOS_ERR;
    }

    if (getDynamicPdpCtxInfoCnf->cause == VOS_OK) {
        for (tmp = 0; tmp < getDynamicPdpCtxInfoCnf->cidNum; tmp++) {
            if (tmp != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            memResult = memcpy_s(&cgdcont, sizeof(cgdcont), &getDynamicPdpCtxInfoCnf->pdpContxtInfo[tmp],
                                 sizeof(TAF_PDP_DynamicPrimExt));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgdcont), sizeof(TAF_PDP_DynamicPrimExt));

            /* +CGCONTRDP:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

            /* <p_cid> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgdcont.primayCid);

            /* <bearer_id> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.bearerId);

            /* <APN> */
            if (cgdcont.opApn == 1) {
                (VOS_VOID)memset_s(str, sizeof(str), 0x00, sizeof(str));
                memResult = memcpy_s(str, sizeof(str), cgdcont.apn, TAF_MAX_APN_LEN);
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(str), TAF_MAX_APN_LEN);
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", cgdcont.apn);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }

            /* <ip_addr> */
            AT_ConvertPdpContextIpAddrParaToString(&cgdcont, &length);

            /* <gw_addr> */
            AT_ConvertPdpContextAddrParaToString(cgdcont.opGwAddr, cgdcont.gwAddr.pdpType, cgdcont.gwAddr.ipv4Addr,
                                                 cgdcont.gwAddr.ipv6Addr, &length);

            /* <DNS_prim_addr> */
            AT_ConvertPdpContextAddrParaToString(cgdcont.opDnsPrimAddr, cgdcont.dnsPrimAddr.pdpType,
                                                 cgdcont.dnsPrimAddr.ipv4Addr, cgdcont.dnsPrimAddr.ipv6Addr, &length);

            /* <DNS_sec_addr> */
            AT_ConvertPdpContextAddrParaToString(cgdcont.opDnsSecAddr, cgdcont.dnsSecAddr.pdpType,
                                                 cgdcont.dnsSecAddr.ipv4Addr, cgdcont.dnsSecAddr.ipv6Addr, &length);

            /* <P-CSCF_prim_addr> */
            AT_ConvertPdpContextAddrParaToString(cgdcont.opPcscfPrimAddr, cgdcont.pcscfPrimAddr.pdpType,
                                                 cgdcont.pcscfPrimAddr.ipv4Addr, cgdcont.pcscfPrimAddr.ipv6Addr,
                                                 &length);

            /* <P-CSCF_sec_addr> */
            AT_ConvertPdpContextAddrParaToString(cgdcont.opPcscfSecAddr, cgdcont.pcscfSecAddr.pdpType,
                                                 cgdcont.pcscfSecAddr.ipv4Addr, cgdcont.pcscfSecAddr.ipv6Addr, &length);

            /* <im-cn-signal-flag> */
            AT_ConvertULParaToString(cgdcont.opImCnSignalFlg, cgdcont.imCnSignalFlg, &length);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

            /* <LIPA indication omit> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");

            /* <ipv4 mtu> */
            AT_ConvertULParaToString(cgdcont.opIpv4Mtu, cgdcont.ipv4Mtu, &length);

            /* the following six parameters omit: <wlan offload>, <local addr ind>, <Non-Ip mtu>, <Serving plmn rate
             * control value>, <Reliable data service> and <Ps data off support> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",,,,,,");

            /* <pdu session id> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.pduSessionId);

            /* <qfi> */
            AT_ConvertULParaToString(cgdcont.opQfi, cgdcont.qfi, &length);

            /* <ssc mode> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.sscMode);

            AT_ConvertSNssaiToString(&cgdcont.sNssai, &length);

            /* <access type> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.accessType);

            /* <reflect qos timer value> */
            AT_ConvertULParaToString(cgdcont.opRqTimer, cgdcont.rqTimer, &length);

            /* <AlwaysOnInd> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdcont.alwaysOnInd);

#endif
        }
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;

    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamicSecPdpContextInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetDynamicSecPdpContextInfoCnf *pdpContxt = VOS_NULL_PTR;
    const TAF_PS_Evt                            *event = VOS_NULL_PTR;
    VOS_UINT32                                   length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamicSecPdpContextInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicSecPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg),
            length);
        return VOS_FALSE;
    }

    event     = (const TAF_PS_Evt *)msg;
    pdpContxt = (const TAF_PS_GetDynamicSecPdpContextInfoCnf *)(event->content);
    length   += (sizeof(TAF_PDP_DynamicSecExt) * pdpContxt->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicSecPdpContextInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg),
            length);
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicSecPdpContextInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t    memResult;
    VOS_UINT32 result = AT_FAILURE;
    VOS_UINT32 tmp    = 0;
    VOS_UINT16 length = 0;

    TAF_PDP_DynamicSecExt                  cgdscont;
    TAF_PS_GetDynamicSecPdpContextInfoCnf *getDynamicPdpCtxInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&cgdscont, sizeof(cgdscont), 0x00, sizeof(TAF_PDP_DynamicSecExt));
    getDynamicPdpCtxInfoCnf = (TAF_PS_GetDynamicSecPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGSCONTRDP_SET) {
        return VOS_ERR;
    }

    if (getDynamicPdpCtxInfoCnf->cause == VOS_OK) {
        for (tmp = 0; tmp < getDynamicPdpCtxInfoCnf->cidNum; tmp++) {
            if (tmp != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            memResult = memcpy_s(&cgdscont, sizeof(cgdscont), &getDynamicPdpCtxInfoCnf->pdpContxtInfo[tmp],
                                 sizeof(TAF_PDP_DynamicSecExt));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgdscont), sizeof(TAF_PDP_DynamicSecExt));

            /* +CGSCONTRDP:  */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
            /* <cid> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgdscont.cid);
            /* <p_cid> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdscont.primaryCid);
            /* <bearer_id> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdscont.bearerId);
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
            /* the following two parameters omit: im_cn_signalling_flag and wlan offload */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",,");
            /* pdu_session_id */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdscont.pduSessionId);
            /* qfi */
            if (cgdscont.opQfi == VOS_TRUE) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgdscont.qfi);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }
#endif
        }

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamicTftInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetDynamicTftInfoCnf *tftInfo = VOS_NULL_PTR;
    const TAF_PS_Evt                  *event = VOS_NULL_PTR;
    VOS_UINT32                         length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamicTftInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicTftInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    tftInfo = (const TAF_PS_GetDynamicTftInfoCnf *)(event->content);
    length += (sizeof(TAF_PF_Tft) * tftInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicTftInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicTftInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t    memResult;
    VOS_UINT32 result = AT_FAILURE;
    VOS_UINT32 index1 = 0;
    VOS_UINT32 index2 = 0;
    VOS_UINT16 length = 0;
    VOS_CHAR   acIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_CHAR   acIpv6StrTmp[AT_IPV6_ADDR_MASK_FORMAT_STR_LEN];
    VOS_CHAR   localIpv4StrTmp[TAF_MAX_IPV4_ADDR_STR_LEN];
    VOS_UINT8  localIpv6Mask[APP_MAX_IPV6_ADDR_LEN];

    TAF_PF_Tft                  *cgtft                = VOS_NULL_PTR;
    TAF_PS_GetDynamicTftInfoCnf *getDynamicTftInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(acIpv4StrTmp, sizeof(acIpv4StrTmp), 0x00, sizeof(acIpv4StrTmp));
    (VOS_VOID)memset_s(acIpv6StrTmp, sizeof(acIpv6StrTmp), 0x00, sizeof(acIpv6StrTmp));
    (VOS_VOID)memset_s(localIpv4StrTmp, sizeof(localIpv4StrTmp), 0x00, sizeof(localIpv4StrTmp));
    (VOS_VOID)memset_s(localIpv6Mask, sizeof(localIpv6Mask), 0x00, sizeof(localIpv6Mask));

    getDynamicTftInfoCnf = (TAF_PS_GetDynamicTftInfoCnf *)evtInfo;

    cgtft = (TAF_PF_Tft *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TAF_PF_Tft));
    if (cgtft == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    (VOS_VOID)memset_s(cgtft, sizeof(TAF_PF_Tft), 0x00, sizeof(TAF_PF_Tft));

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGTFTRDP_SET) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, cgtft);
        /*lint -restore */
        return VOS_ERR;
    }

    if (getDynamicTftInfoCnf->cause == VOS_OK) {
        for (index1 = 0; index1 < getDynamicTftInfoCnf->cidNum; index1++) {
            for (index2 = 0; index2 < AT_MIN(getDynamicTftInfoCnf->pfTftInfo[index1].pfNum, TAF_MAX_SDF_PF_NUM); index2++) {
                if (!(index1 == 0 && index2 == 0)) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
                }

                memResult = memcpy_s(cgtft, sizeof(TAF_PF_Tft), &getDynamicTftInfoCnf->pfTftInfo[index1],
                                     sizeof(TAF_PF_Tft));
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_PF_Tft), sizeof(TAF_PF_Tft));

                /* +CGTFTRDP:  */
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
                /* <cid> */
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgtft->cid);
                /* <packet filter identifier> */
                AT_ConvertULParaToString(cgtft->tftInfo[index2].opPktFilterId, cgtft->tftInfo[index2].packetFilterId,
                                         &length);

                /* <evaluation precedence index> */
                AT_ConvertULParaToString(cgtft->tftInfo[index2].opPrecedence, cgtft->tftInfo[index2].precedence,
                                         &length);

                /* <source address and subnet> */
                AT_ConvertTftSrcAddrParaToString(&cgtft->tftInfo[index2], &length);

                /* <protocal number(ipv4)/next header ipv6> */
                AT_ConvertULParaToString(cgtft->tftInfo[index2].opProtocolId, cgtft->tftInfo[index2].protocolId,
                                         &length);

                /* <destination port range> */
                AT_ConvertRangeParaToString(VOS_FALSE, cgtft->tftInfo[index2].opDestPortRange,
                                            cgtft->tftInfo[index2].lowDestPort, cgtft->tftInfo[index2].highDestPort,
                                            &length);

                /* <source port range> */
                AT_ConvertRangeParaToString(VOS_FALSE, cgtft->tftInfo[index2].opSrcPortRange,
                                            cgtft->tftInfo[index2].lowSourcePort, cgtft->tftInfo[index2].highSourcePort,
                                            &length);

                /* <ipsec security parameter index(spi)> */
                AT_ConvertULParaToXString(cgtft->tftInfo[index2].opSpi, cgtft->tftInfo[index2].secuParaIndex, &length);

                /* <type os service(tos) (ipv4) and mask> */
                AT_ConvertRangeParaToString(VOS_FALSE, cgtft->tftInfo[index2].opTosMask,
                                            cgtft->tftInfo[index2].typeOfService,
                                            cgtft->tftInfo[index2].typeOfServiceMask, &length);

                /* <traffic class (ipv6) and mask> */

                /* <flow lable (ipv6)> */
                AT_ConvertULParaToXString(cgtft->tftInfo[index2].opFlowLable, cgtft->tftInfo[index2].flowLable,
                                          &length);

                /* <direction> */
                AT_ConvertULParaToString(cgtft->tftInfo[index2].opDirection, cgtft->tftInfo[index2].direction, &length);

                /* <NW packet filter Identifier> */
                AT_ConvertULParaToString(cgtft->tftInfo[index2].opNwPktFilterId, cgtft->tftInfo[index2].nwPktFilterId,
                                         &length);

                if (AT_IsSupportReleaseRst(AT_ACCESS_STRATUM_REL11)) {
                    /* <local address and subnet> */
                    if (cgtft->tftInfo[index2].opLocalIpv4AddrAndMask == 1) {
                        AT_Ipv4AddrItoa(localIpv4StrTmp, sizeof(localIpv4StrTmp), cgtft->tftInfo[index2].localIpv4Addr);
                        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s", localIpv4StrTmp);

                        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ".");

                        AT_Ipv4AddrItoa(localIpv4StrTmp, sizeof(localIpv4StrTmp), cgtft->tftInfo[index2].localIpv4Mask);
                        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, "%s\"", localIpv4StrTmp);
                    } else if (cgtft->tftInfo[index2].opLocalIpv6AddrAndMask == 1) {
                        AT_GetIpv6MaskByPrefixLength(cgtft->tftInfo[index2].localIpv6Prefix, localIpv6Mask);
                        (VOS_VOID)AT_Ipv6AddrMask2FormatString(acIpv6StrTmp, cgtft->tftInfo[index2].localIpv6Addr,
                                                               localIpv6Mask);
                        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acIpv6StrTmp);
                    } else {
                        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
                    }

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
                    /* <qri> */
                    AT_ConvertULParaToString(cgtft->tftInfo[index2].opQri, cgtft->tftInfo[index2].qri, &length);
#endif
                }
            }

            /* <3,0,0,"192.168.0.2.255.255.255.0">,0,"0.65535","0.65535",0,"0.0",0,0 */
        }

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, cgtft);
    /*lint -restore */
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetEpsQosInfoCnf *setEpsqosInfoCnf = VOS_NULL_PTR;

    setEpsqosInfoCnf = (TAF_PS_SetPrimPdpContextInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQOS_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setEpsqosInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetEpsQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetEpsQosInfoCnf *qosInfo = VOS_NULL_PTR;
    const TAF_PS_Evt              *event   = VOS_NULL_PTR;
    VOS_UINT32                     length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetEpsQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetEpsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    qosInfo = (const TAF_PS_GetEpsQosInfoCnf *)(event->content);
    length += (sizeof(TAF_EPS_QosExt) * qosInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetEpsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 length = 0;
    VOS_UINT32 tmp    = 0;

    TAF_EPS_QosExt           cgeqos;
    TAF_PS_GetEpsQosInfoCnf *getEpsQosInfoCnf = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&cgeqos, sizeof(cgeqos), 0x00, sizeof(TAF_EPS_QosExt));

    getEpsQosInfoCnf = (TAF_PS_GetEpsQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQOS_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < getEpsQosInfoCnf->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgeqos, sizeof(cgeqos), &getEpsQosInfoCnf->epsQosInfo[tmp], sizeof(TAF_EPS_QosExt));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgeqos), sizeof(TAF_EPS_QosExt));

        /* +CGEQOS:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgeqos.cid);
        /* <QCI> */
        if (cgeqos.opQci == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.qci);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <DL GBR> */
        if (cgeqos.opDlgbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.dlgbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <UL GBR> */
        if (cgeqos.opUlgbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.ulgbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <DL MBR> */
        if (cgeqos.opDlmbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.dlmbr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <UL MBR> */
        if (cgeqos.opUlmbr == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.ulmbr);
        }
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_VOID AT_PrintDynamicEpsQosInfo(TAF_PS_GetDynamicEpsQosInfoCnf *getDynamicEpsQosInfoCnf, VOS_UINT8 indexNum)
{
    errno_t        memResult;
    TAF_EPS_QosExt cgeqos;
    VOS_UINT16     length = 0;
    VOS_UINT32     tmp    = 0;

    (VOS_VOID)memset_s(&cgeqos, sizeof(cgeqos), 0x00, sizeof(TAF_EPS_QosExt));

    for (tmp = 0; tmp < getDynamicEpsQosInfoCnf->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&cgeqos, sizeof(cgeqos), &getDynamicEpsQosInfoCnf->epsQosInfo[tmp],
                             sizeof(TAF_EPS_QosExt));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cgeqos), sizeof(TAF_EPS_QosExt));

        /* +CGEQOSRDP:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cgeqos.cid);
        /* <QCI> */
        if (cgeqos.opQci == 1) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.qci);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        if (cgeqos.qci <= AT_BIT_RATE_TRAFFIC_FLOWS_MAX_VALUE_RANGE) {
            /* <DL GBR> */
            if (cgeqos.opDlgbr == 1) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.dlgbr);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }
            /* <UL GBR> */
            if (cgeqos.opUlgbr == 1) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.ulgbr);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }
            /* <DL MBR> */
            if (cgeqos.opDlmbr == 1) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.dlmbr);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",");
            }
            /* <UL MBR> */
            if (cgeqos.opUlmbr == 1) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", cgeqos.ulmbr);
            }
        }
    }

    g_atSendDataBuff.bufLen = length;

    return;
}

VOS_UINT32 AT_ChkTafPsEvtGetDynamicEpsQosInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_Evt                     *event = VOS_NULL_PTR;
    const TAF_PS_GetDynamicEpsQosInfoCnf *epsQos = VOS_NULL_PTR;
    VOS_UINT32                            length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetDynamicEpsQosInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicEpsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event   = (const TAF_PS_Evt *)msg;
    epsQos  = (const TAF_PS_GetDynamicEpsQosInfoCnf *)(event->content);
    length += (sizeof(TAF_EPS_QosExt) * epsQos->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetDynamicEpsQosInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicEpsQosInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDynamicEpsQosInfoCnf *getDynamicEpsQosInfoCnf = VOS_NULL_PTR;
    VOS_UINT32                      result                  = AT_FAILURE;

    getDynamicEpsQosInfoCnf = (TAF_PS_GetDynamicEpsQosInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGEQOSRDP_SET) {
        return VOS_ERR;
    }

    if (getDynamicEpsQosInfoCnf->cause == VOS_OK) {
        AT_PrintDynamicEpsQosInfo(getDynamicEpsQosInfoCnf, indexNum);
        result = AT_OK;
    }
    else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetDsFlowInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDsflowInfoCnf *evtMsg    = VOS_NULL_PTR;
    TAF_DSFLOW_QueryInfo    *queryInfo = VOS_NULL_PTR;
    VOS_UINT16               length    = 0;

    evtMsg    = (TAF_PS_GetDsflowInfoCnf *)evtInfo;
    queryInfo = &evtMsg->queryInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSFLOWQRY_SET) {
        return VOS_ERR;
    }

    /* 上报流量查询结果 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%08X,%08X%08X,%08X%08X", queryInfo->currentFlowInfo.linkTime,
        queryInfo->currentFlowInfo.sendFluxHigh, queryInfo->currentFlowInfo.sendFluxLow,
        queryInfo->currentFlowInfo.recvFluxHigh, queryInfo->currentFlowInfo.recvFluxLow);

    length +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%08X,%08X%08X,%08X%08X", queryInfo->totalFlowInfo.linkTime,
            queryInfo->totalFlowInfo.sendFluxHigh, queryInfo->totalFlowInfo.sendFluxLow,
            queryInfo->totalFlowInfo.recvFluxHigh, queryInfo->totalFlowInfo.recvFluxLow);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtClearDsFlowInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ClearDsflowCnf *evtMsg = VOS_NULL_PTR;

    evtMsg = (TAF_PS_ClearDsflowCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSFLOWCLR_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, evtMsg->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtConfigDsFlowRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ConfigDsflowRptCnf *evtMsg = VOS_NULL_PTR;

    evtMsg = (TAF_PS_ConfigDsflowRptCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSFLOWRPT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, evtMsg->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtConfigVTFlowRptCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ConfigVtflowRptCnf *evtMsg = VOS_NULL_PTR;

    evtMsg = (TAF_PS_ConfigVtflowRptCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VTFLOWRPT_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, evtMsg->cause);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtReportDsFlowInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ReportDsflowInd *evtMsg  = VOS_NULL_PTR;
    TAF_DSFLOW_Report      *rptInfo = VOS_NULL_PTR;
    ModemIdUint16           modemId = MODEM_ID_0;
    VOS_UINT16              length  = 0;

    evtMsg  = (TAF_PS_ReportDsflowInd *)evtInfo;
    rptInfo = &evtMsg->reportInfo;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtReportDsFlowInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* 上报流量信息 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DSFLOWRPT: %08X,%08X,%08X,%08X%08X,%08X%08X,%08X,%08X%s",
        g_atCrLf, rptInfo->flowInfo.linkTime, rptInfo->sendRate, rptInfo->recvRate, rptInfo->flowInfo.sendFluxHigh,
        rptInfo->flowInfo.sendFluxLow, rptInfo->flowInfo.recvFluxHigh, rptInfo->flowInfo.recvFluxLow,
        rptInfo->qosSendRate, rptInfo->qosRecvRate, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtReportVTFlowInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ReportVtflowInd *evtMsg  = VOS_NULL_PTR;
    TAF_VTFLOW_Report      *rptInfo = VOS_NULL_PTR;
    ModemIdUint16           modemId = MODEM_ID_0;
    VOS_UINT16              length  = 0;

    evtMsg  = (TAF_PS_ReportVtflowInd *)evtInfo;
    rptInfo = &evtMsg->reportInfo;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtReportVTFlowInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* 上报视频流量信息 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%08X,%08X%08X,%08X%08X%s", g_atCrLf,
        g_atStringTab[AT_STRING_VT_FLOW_RPT].text, rptInfo->linkTime, rptInfo->sendFluxHigh,
        rptInfo->sendFluxLow, rptInfo->recvFluxHigh, rptInfo->recvFluxLow, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetApDsFlowRptCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetApdsflowRptCfgCnf *evtMsg = VOS_NULL_PTR;

    evtMsg = (TAF_PS_SetApdsflowRptCfgCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_APDSFLOWRPTCFG_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, evtMsg->cause);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetApDsFlowRptCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetApdsflowRptCfgCnf *evtMsg = VOS_NULL_PTR;
    VOS_UINT16                   length;

    evtMsg = (TAF_PS_GetApdsflowRptCfgCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_APDSFLOWRPTCFG_QRY) {
        return VOS_ERR;
    }

    /* 检查错误码 */
    if (evtMsg->cause != TAF_PS_CAUSE_SUCCESS) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 上报查询结果 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%u", g_parseContext[indexNum].cmdElement->cmdName,
        evtMsg->rptCfg.rptEnabled, evtMsg->rptCfg.fluxThreshold);

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtApDsFlowReportInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_ApdsflowReportInd *evtMsg  = VOS_NULL_PTR;
    TAF_APDSFLOW_Report      *rptInfo = VOS_NULL_PTR;
    ModemIdUint16             modemId = MODEM_ID_0;
    VOS_UINT16                length;

    evtMsg  = (TAF_PS_ApdsflowReportInd *)evtInfo;
    rptInfo = &evtMsg->reportInfo;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtApDsFlowReportInd: Get modem id fail.");
        return VOS_ERR;
    }

    /*
     * ^APDSFLOWRPT: <curr_ds_time>,<tx_rate>,<rx_rate>,
     * <curr_tx_flow>,<curr_rx_flow>,<total_tx_flow>,<total_rx_flow>
     */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^APDSFLOWRPT: %08X,%08X,%08X,%08X%08X,%08X%08X,%08X%08X,%08X%08X%s",
        g_atCrLf, rptInfo->currentFlowInfo.linkTime, rptInfo->sendRate, rptInfo->recvRate,
        rptInfo->currentFlowInfo.sendFluxHigh, rptInfo->currentFlowInfo.sendFluxLow,
        rptInfo->currentFlowInfo.recvFluxHigh, rptInfo->currentFlowInfo.recvFluxLow,
        rptInfo->totalFlowInfo.sendFluxHigh, rptInfo->totalFlowInfo.sendFluxLow,
        rptInfo->totalFlowInfo.recvFluxHigh, rptInfo->totalFlowInfo.recvFluxLow, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetDsFlowNvWriteCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetDsflowNvWriteCfgCnf *setNvWriteCfgCnf = VOS_NULL_PTR;

    setNvWriteCfgCnf = (TAF_PS_SetDsflowNvWriteCfgCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSFLOWNVWRCFG_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setNvWriteCfgCnf->cause);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetDsFlowNvWriteCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDsflowNvWriteCfgCnf *getNvWriteCfgCnf = VOS_NULL_PTR;
    VOS_UINT16                     length;

    getNvWriteCfgCnf = (TAF_PS_GetDsflowNvWriteCfgCnf *)evtInfo;
    length           = 0;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSFLOWNVWRCFG_QRY) {
        return VOS_ERR;
    }

    /* 检查错误码 */
    if (getNvWriteCfgCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 上报查询结果 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        getNvWriteCfgCnf->nvWriteCfg.enabled, getNvWriteCfgCnf->nvWriteCfg.interval);

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvTafPsEvtSetPktCdataInactivityTimeLenCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetCtaInfoCnf *setPktCdataInactivityTimeLenCnf = VOS_NULL_PTR;

    setPktCdataInactivityTimeLenCnf = (TAF_PS_SetCtaInfoCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CTA_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setPktCdataInactivityTimeLenCnf->rslt);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetPktCdataInactivityTimeLenCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT16            length;
    TAF_PS_GetCtaInfoCnf *getPktCdataInactivityTimeLenCnf = VOS_NULL_PTR;

    getPktCdataInactivityTimeLenCnf = (TAF_PS_GetCtaInfoCnf *)evtInfo;
    length                          = 0;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CTA_QRY) {
        return VOS_ERR;
    }

    /* 检查错误码 */
    if (getPktCdataInactivityTimeLenCnf->rslt != TAF_PS_CAUSE_SUCCESS) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 上报查询结果 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        getPktCdataInactivityTimeLenCnf->pktCdataInactivityTmrLen);

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtCgmtuValueChgInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CgmtuValueChgInd *cgmtuChgInd = VOS_NULL_PTR;
    VOS_UINT16               length;
    ModemIdUint16            modemId;

    cgmtuChgInd = (TAF_PS_CgmtuValueChgInd *)evtInfo;
    length      = 0;
    modemId     = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtCgmtuValueChgInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ^CGMTU: <curr_mtu_value> */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^CGMTU: %d%s", g_atCrLf, cgmtuChgInd->mtuValue, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

#endif

VOS_UINT32 AT_RcvTafPsEvtSetPdpDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetPdpDnsInfoCnf *setPdpDnsInfoCnf = VOS_NULL_PTR;

    setPdpDnsInfoCnf = (TAF_PS_SetPdpDnsInfoCnf *)evtInfo;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDNS_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setPdpDnsInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetPdpDnsInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetPdpDnsInfoCnf *pdpDnsInfo = VOS_NULL_PTR;
    const TAF_PS_Evt              *event      = VOS_NULL_PTR;
    VOS_UINT32                     length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetPdpDnsInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpDnsInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event      = (const TAF_PS_Evt *)msg;
    pdpDnsInfo = (const TAF_PS_GetPdpDnsInfoCnf *)(event->content);
    length    += (sizeof(TAF_DNS_QueryInfo) * pdpDnsInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetPdpDnsInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetPdpDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t memResult;
    /* 移植At_QryParaRspCgdnsProc的实现逻辑 */
    VOS_UINT32               result;
    VOS_UINT16               length = 0;
    VOS_UINT32               tmp    = 0;
    TAF_DNS_QueryInfo        pdpDns;
    TAF_PS_GetPdpDnsInfoCnf *pdpDnsInfo = VOS_NULL_PTR;
    VOS_INT8                 acDnsAddr[TAF_MAX_IPV4_ADDR_STR_LEN];

    (VOS_VOID)memset_s(&pdpDns, sizeof(pdpDns), 0x00, sizeof(TAF_DNS_QueryInfo));
    (VOS_VOID)memset_s(acDnsAddr, sizeof(acDnsAddr), 0x00, TAF_MAX_IPV4_ADDR_STR_LEN);
    pdpDnsInfo = (TAF_PS_GetPdpDnsInfoCnf *)evtInfo;

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGDNS_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < pdpDnsInfo->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&pdpDns, sizeof(pdpDns), &pdpDnsInfo->pdpDnsQueryInfo[tmp], sizeof(TAF_DNS_QueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pdpDns), sizeof(TAF_DNS_QueryInfo));
        /* +CGDNS:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", pdpDns.cid);
        /* <PriDns> */
        if (pdpDns.dnsInfo.bitOpPrimDnsAddr == 1) {
            AT_Ipv4Addr2Str((VOS_CHAR *)acDnsAddr, sizeof(acDnsAddr), pdpDns.dnsInfo.primDnsAddr, TAF_IPV4_ADDR_LEN);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acDnsAddr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        /* <SecDns> */
        if (pdpDns.dnsInfo.bitOpSecDnsAddr == 1) {
            AT_Ipv4Addr2Str((VOS_CHAR *)acDnsAddr, sizeof(acDnsAddr), pdpDns.dnsInfo.secDnsAddr, TAF_IPV4_ADDR_LEN);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acDnsAddr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetAuthDataInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetAuthDataInfoCnf *setAuthDataInfoCnf = VOS_NULL_PTR;

    /* 初始化 */
    setAuthDataInfoCnf = (TAF_PS_SetAuthDataInfoCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AUTHDATA_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setAuthDataInfoCnf->cause);

    return VOS_OK;
}

VOS_UINT32 AT_ChkTafPsEvtGetAuthDataInfoCnfMsgLen(const MSG_Header *msg)
{
    const TAF_PS_GetAuthdataInfoCnf *authInfo = VOS_NULL_PTR;
    const TAF_PS_Evt                *event    = VOS_NULL_PTR;
    VOS_UINT32                       length;

    length = sizeof(TAF_PS_Evt) + sizeof(TAF_PS_GetAuthdataInfoCnf) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetAuthDataInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    event    = (const TAF_PS_Evt*)msg;
    authInfo = (const TAF_PS_GetAuthdataInfoCnf *)(event->content);
    length  += (sizeof(TAF_AUTHDATA_QueryInfo) * authInfo->cidNum);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < length) {
        AT_ERR_LOG2("AT_ChkTafPsEvtGetAuthDataInfoCnfMsgLen: msgLen, structLen.", VOS_GET_MSG_LEN(msg), length);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtGetAuthDataInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                    memResult;
    VOS_UINT32                 result;
    VOS_UINT32                 tmp    = 0;
    VOS_UINT16                 length = 0;
    TAF_AUTHDATA_QueryInfo     pdpAuthData;
    TAF_PS_GetAuthdataInfoCnf *pdpAuthDataInfo = VOS_NULL_PTR;

    memset_s(&pdpAuthData, sizeof(pdpAuthData), 0x00, sizeof(TAF_AUTHDATA_QueryInfo));

    pdpAuthDataInfo = (TAF_PS_GetAuthdataInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AUTHDATA_READ) {
        return VOS_ERR;
    }

    for (tmp = 0; tmp < pdpAuthDataInfo->cidNum; tmp++) {
        if (tmp != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        memResult = memcpy_s(&pdpAuthData, sizeof(pdpAuthData), &pdpAuthDataInfo->authDataQueryInfo[tmp],
                             sizeof(TAF_AUTHDATA_QueryInfo));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pdpAuthData), sizeof(TAF_AUTHDATA_QueryInfo));
        /* ^AUTHDATA:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        /* <cid> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", pdpAuthData.cid);

        /* <Auth_type> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", pdpAuthData.authDataInfo.authType);

        /* <passwd> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", pdpAuthData.authDataInfo.password);

        /* <username> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", pdpAuthData.authDataInfo.username);

        /* <PLMN> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", pdpAuthData.authDataInfo.plmn);
    }

    result                  = AT_OK;
    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    memset_s(&pdpAuthData, sizeof(pdpAuthData), 0x00, sizeof(TAF_AUTHDATA_QueryInfo));

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetGprsActiveTypeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32                    result;
    TAF_PS_GetDGprsActiveTypeCnf *getGprsActiveTypeCnf = VOS_NULL_PTR;

    /* 初始化 */
    result               = AT_FAILURE;
    getGprsActiveTypeCnf = (TAF_PS_GetDGprsActiveTypeCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_D_GPRS_SET) {
        return VOS_ERR;
    }

    /* 转换错误码格式 */
    if (getGprsActiveTypeCnf->cause == TAF_PARA_OK) {
        result = At_SetDialGprsPara(indexNum, getGprsActiveTypeCnf->cidGprsActiveType.cid,
                                    getGprsActiveTypeCnf->cidGprsActiveType.activeType);
    } else {
        result = AT_ERROR;
    }

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_ERROR) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
        }

        AT_StopRelTimer(indexNum, &g_atClientTab[indexNum].hTimer);
        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;
        g_atClientTab[indexNum].opId          = 0;

        At_FormatResultData(indexNum, result);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtPppDialOrigCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_PppDialOrigCnf *pppDialOrigCnf = VOS_NULL_PTR;

    /* 初始化 */
    pppDialOrigCnf = (TAF_PS_PppDialOrigCnf *)evtInfo;

    AT_ProcPppDialCnf(pppDialOrigCnf->cause, indexNum);

    return VOS_OK;

}


VOS_UINT32 AT_RcvTafPsEvtGetCidSdfInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    return VOS_OK;
}


VOS_BOOL AT_PH_IsPlmnValid(TAF_PLMN_Id *plmnId)
{
    VOS_UINT32 i;

    for (i = 0; i < AT_MCC_LENGTH; i++) {
        if ((((plmnId->mcc >> (i * AT_HEX_OCCUPIED_BIT_NUM)) & 0x0F) > AT_DEC_MAX_NUM) ||
            ((((plmnId->mnc >> (i * AT_HEX_OCCUPIED_BIT_NUM)) & 0x0F) > AT_DEC_MAX_NUM) &&
             (i != AT_MNC_LENGTH_TWO_BYTES)) ||
            ((((plmnId->mnc >> (i * AT_HEX_OCCUPIED_BIT_NUM)) & 0x0F) > AT_DEC_MAX_NUM) &&
            (((plmnId->mnc >> (i * AT_HEX_OCCUPIED_BIT_NUM)) & 0x0F) != 0x0F))) {
            /* PLMN ID无效 */
            return VOS_FALSE;
        }
    }
    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafPsEvtPdpDisconnectInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32 result;

    VOS_UINT16 length;

    length = 0;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        /* 未应答的场景直接上报NO CARRIER */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", "NO CARRIER");

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);

        return VOS_OK;
    }

    /* IP类型网络激活^CGANS应答过程中上报ID_EVT_TAF_PS_CALL_PDP_DISCONNECT_IND */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PPP_ORG_SET) {
        result = AT_NO_CARRIER;

        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_REQ);

        /* 返回命令模式 */
        DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);

    } else if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGANS_ANS_EXT_SET) ||
               (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGANS_ANS_SET)) {
        /*
         * 1.PPP类型网络激活^CGANS应答过程中上报ID_EVT_TAF_PS_CALL_PDP_DISCONNECT_IND
         * 2.+CGANS应答
         * 以上两种情况都还没有切数传通道，直接回ERROR
         */
        result = AT_ERROR;
    } else {
        result = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_SetMemStatusRspProc(VOS_UINT8 indexNum, MN_MSG_EventInfo *event)
{
    VOS_UINT32 result;

    /* 检查用户索引值 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_SetMemStatusRspProc: AT_BROADCAST_INDEX.");
        return;
    }

#if (FEATURE_MBB_CUST == FEATURE_ON)
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSASM_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HSMF_SET)) {
        return;
    }
#else
    /* 判断当前操作类型是否为AT_CMD_CSASM_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSASM_SET) {
        return;
    }
#endif

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->u.memStatusInfo.success == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_RcvTafPsEvtGetDynamicDnsInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32                   result     = AT_ERROR;
    VOS_UINT16                   length     = 0;
    TAF_PS_GetNegotiationDnsCnf *negoDnsCnf = VOS_NULL_PTR;
    VOS_INT8                     acDnsAddr[TAF_MAX_IPV4_ADDR_STR_LEN];

    (VOS_VOID)memset_s(acDnsAddr, sizeof(acDnsAddr), 0x00, sizeof(acDnsAddr));

    negoDnsCnf = (TAF_PS_GetNegotiationDnsCnf *)evtInfo;

    /* 检查用户索引值 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafPsEvtGetDynamicDnsInfoCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DNSQUERY_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (negoDnsCnf->cause != TAF_PARA_OK) {
        result = AT_ERROR;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <PriDns> */
        if (negoDnsCnf->negotiationDns.dnsInfo.bitOpPrimDnsAddr == VOS_TRUE) {
            AT_Ipv4Addr2Str((VOS_CHAR *)acDnsAddr, sizeof(acDnsAddr), negoDnsCnf->negotiationDns.dnsInfo.primDnsAddr,
                            TAF_IPV4_ADDR_LEN);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acDnsAddr);
        }
        /* <SecDns> */
        if (negoDnsCnf->negotiationDns.dnsInfo.bitOpSecDnsAddr == VOS_TRUE) {
            AT_Ipv4Addr2Str((VOS_CHAR *)acDnsAddr, sizeof(acDnsAddr), negoDnsCnf->negotiationDns.dnsInfo.secDnsAddr,
                            TAF_IPV4_ADDR_LEN);
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", acDnsAddr);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 At_RcvTafPsEvtSetDialModeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    VOS_UINT32               result;
    TAF_PS_CdataDialModeCnf *dialMode = VOS_NULL_PTR;

    /* 初始化 */
    result   = AT_OK;
    dialMode = (TAF_PS_CdataDialModeCnf *)evtInfo;

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRM_SET) {
        AT_WARN_LOG("At_RcvTafPsEvtSetDialModeCnf : Current Option is not AT_CMD_CRM_SET.");
        return VOS_OK;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (dialMode->cause != TAF_PS_CAUSE_SUCCESS) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 atReadLtecsCnfProc(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_LtecsCnf *ltecsReadCnf = NULL;
    VOS_UINT16       length       = 0;
    VOS_UINT32       result;

    ltecsReadCnf = (TAF_PS_LtecsCnf *)evtInfo;

    if (ltecsReadCnf->cause == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "^LTECS:");
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d", ltecsReadCnf->lteCs.sg, ltecsReadCnf->lteCs.ims);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d", ltecsReadCnf->lteCs.csfb, ltecsReadCnf->lteCs.vcc);
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", ltecsReadCnf->lteCs.voLga);

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

VOS_UINT32 atReadCemodeCnfProc(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CemodeCnf *cemodeReadCnf = NULL;
    VOS_UINT16        length        = 0;
    VOS_UINT32        result;

    cemodeReadCnf = (TAF_PS_CemodeCnf *)evtInfo;

    if (cemodeReadCnf->cause == VOS_OK) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "+CEMODE:");
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", cemodeReadCnf->cemode.currentUeMode);

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * 功能描述: ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_CNF事件处理函数
 */
VOS_UINT32 AT_RcvTafGetLteAttachInfoCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    errno_t                     memResult;
    TAF_PS_GetLteAttachInfoCnf *getLteAttachInfoCnf      = VOS_NULL_PTR;
    VOS_UINT16                  length                   = 0;
    VOS_UINT32                  result                   = AT_ERROR;
    VOS_UINT8                   str[TAF_MAX_APN_LEN + 1] = {0};

    getLteAttachInfoCnf = (TAF_PS_GetLteAttachInfoCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEATTACHINFO_QRY) {
        AT_WARN_LOG("AT_RcvTafGetLteAttachInfoCnf : Current Option is not AT_CMD_LTEATTACHINFO_QRY.");
        return VOS_ERR;
    }

    /* 如果获取成功则填充信息 */
    if (getLteAttachInfoCnf->cause == TAF_PS_CAUSE_SUCCESS) {
        /* ^LTEATTACHINFO:  */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <PDP_type> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", getLteAttachInfoCnf->lteAttQueryInfo.pdpType);

        /* <APN> */
        getLteAttachInfoCnf->lteAttQueryInfo.apn.length =
            TAF_MIN(getLteAttachInfoCnf->lteAttQueryInfo.apn.length,
                    sizeof(getLteAttachInfoCnf->lteAttQueryInfo.apn.value));

        if (getLteAttachInfoCnf->lteAttQueryInfo.apn.length > 0) {
            memResult = memcpy_s(str, sizeof(str), getLteAttachInfoCnf->lteAttQueryInfo.apn.value,
                                 getLteAttachInfoCnf->lteAttQueryInfo.apn.length);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(str), getLteAttachInfoCnf->lteAttQueryInfo.apn.length);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", str);

        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_VOID AT_ConvertNasMncToBcdType(VOS_UINT32 nasMnc, VOS_UINT32 *mnc)
{
    VOS_UINT32 i;
    VOS_UINT8  tmp[AT_TMP_ARRAY_LEN];

    *mnc = 0;

    for (i = 0; i < AT_TMP_ARRAY_LEN - 1; i++) {
        tmp[i] = nasMnc & 0x0f;
        nasMnc >>= 8;
    }

    if (tmp[AT_TMP_ARRAY_INDEX_2] == 0xf) {
        *mnc = ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_0] << 4) |
                ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_1]) | 0xf00;
    } else {
        *mnc = ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_0] << 8) |
                ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_1] << 4) | tmp[AT_TMP_ARRAY_INDEX_2];
    }
}


VOS_VOID AT_ConvertNasMccToBcdType(VOS_UINT32 nasMcc, VOS_UINT32 *mcc)
{
    VOS_UINT32 i;
    VOS_UINT8  tmp[AT_TMP_ARRAY_LEN];

    *mcc = 0;

    for (i = 0; i < AT_TMP_ARRAY_LEN - 1; i++) {
        tmp[i] = nasMcc & 0x0f;
        nasMcc >>= 8;
    }

    *mcc = ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_0] << 8) |
            ((VOS_UINT32)tmp[AT_TMP_ARRAY_INDEX_1] << 4) | tmp[AT_TMP_ARRAY_INDEX_2];
}


VOS_UINT32 AT_RcvTafCcmEccNumInd(struct MsgCB *msg)
{
    TAF_CCM_EccNumInd *eccNumInfo = VOS_NULL_PTR;
    VOS_UINT8          asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    VOS_UINT8          indexNum = 0;
    VOS_UINT32         i;
    VOS_UINT16         length;
    VOS_UINT32         mcc;
    ModemIdUint16      modemId;
    VOS_UINT32         rslt;

    modemId = MODEM_ID_0;

    (VOS_VOID)memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));

    /* 获取上报的紧急呼号码信息 */
    eccNumInfo = (TAF_CCM_EccNumInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(eccNumInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvMnCallEccNumIndProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_RcvMnCallEccNumIndProc: Get modem id fail.");
        return VOS_ERR;
    }

    /* 向APP逐条上报紧急呼号码 */
    eccNumInfo->eccNumInd.eccNumCount = AT_MIN(eccNumInfo->eccNumInd.eccNumCount, MN_CALL_MAX_EMC_NUM);
    for (i = 0; i < eccNumInfo->eccNumInd.eccNumCount; i++) {
        /* 将BCD码转化为ASCII码 */
        AT_BcdNumberToAscii(eccNumInfo->eccNumInd.customEccNumList[i].eccNum,
                            eccNumInfo->eccNumInd.customEccNumList[i].eccNumLen, (VOS_CHAR *)asciiNum);

        /* 将NAS格式的MCC转化为BCD格式 */
        AT_ConvertNasMccToBcdType(eccNumInfo->eccNumInd.customEccNumList[i].mcc, &mcc);

        length = 0;
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s^XLEMA:", g_atCrLf);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d,%s,%d,%d,", (i + 1), eccNumInfo->eccNumInd.eccNumCount,
            asciiNum, eccNumInfo->eccNumInd.customEccNumList[i].category,
            eccNumInfo->eccNumInd.customEccNumList[i].validSimPresent);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x", (mcc & 0x0f00) >> 8, (mcc & 0xf0) >> 4, (mcc & 0x0f));

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d%s",
            eccNumInfo->eccNumInd.customEccNumList[i].abnormalServiceFlg, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);

        (VOS_VOID)memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));
    }

    return VOS_OK;
}


VOS_VOID AT_RcvMmaNsmStatusInd(TAF_UINT8 indexNum, TAF_PHONE_EventInfo *event)
{
    VOS_UINT16 length;

    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s+PACSP", g_atCrLf);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", event->plmnMode);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


LOCAL VOS_VOID AT_RcvMmaGsmCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                           VOS_UINT8 systemAppConfig)
{
    VOS_UINT16 length;
    length = 0;

    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CERSSI].text, rssiInfoInd->rssiInfo.aRssi[0].u.gCellSignInfo.rssiValue, 0, 255, 0,
            0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99, 99, 99, 0, 0, 0, g_atCrLf);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CERSSI].text, rssiInfoInd->rssiInfo.aRssi[0].u.gCellSignInfo.rssiValue, 0, 255, 0,
            0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0, g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
}


LOCAL VOS_VOID AT_RcvMmaWcdmaCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                             VOS_UINT8 systemAppConfig)
{
    VOS_UINT16 length;
    length = 0;

    if (rssiInfoInd->rssiInfo.currentUtranMode == TAF_UTRANCTRL_UTRAN_MODE_FDD) {
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.rscpValue,
                rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.ecioValue, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99,
                99, 99, 99, 99, 99, 0, 0, 0, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.rscpValue,
                rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.ecioValue, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0,
                g_atCrLf);
        }
    } else {
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            /* 非fdd 3g 小区，ecio值为无效值255 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.rscpValue,
                255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99, 99, 99, 0, 0, 0, g_atCrLf);
        } else {
            /* 非fdd 3g 小区，ecio值为无效值255 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, rssiInfoInd->rssiInfo.aRssi[0].u.wCellSignInfo.rscpValue,
                255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0, g_atCrLf);
        }
    }
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}

#if (FEATURE_LTE == FEATURE_ON)

LOCAL VOS_VOID AT_RcvMmaLteCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                           VOS_UINT8 systemAppConfig)
{
    VOS_UINT16 length;
    length = 0;

    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CERSSI].text, 0, 0, 255, rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrp,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrq, rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.sinr,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.ri,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.cqi[1],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.rxANTNum,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR0],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR1],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR2],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR3],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR0],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR1],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR2],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR3], 0, 0, 0, g_atCrLf);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CERSSI].text, 0, 0, 255, rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrp,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrq, rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.sinr,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.ri,
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
            rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.cqi.cqi[1], 0, 0, 0, g_atCrLf);
    }
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

LOCAL VOS_VOID AT_RcvMmaNrCERssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd,
                                          VOS_UINT8 systemAppConfig)
{
    VOS_UINT16 length;
    length = 0;

    /* SA, use ^CERSSI to report */
    if (rssiInfoInd->isNsaRptFlg == VOS_FALSE) {
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99,
                99, 99, rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.l5GSinr, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CERSSI].text, 0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.l5GSinr, g_atCrLf);
        }
        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    /* NSA, use ^CSERSSI to report */
    else {
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CSERSSI].text, 0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99,
                99, 99, 99, rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.l5GSinr, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                g_atStringTab[AT_STRING_CSERSSI].text, 0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                rssiInfoInd->rssiInfo.aRssi[0].u.nrCellSignInfo.l5GSinr, g_atCrLf);
        }

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    return;
}
#endif


VOS_VOID AT_RcvMmaRssiChangeInd(TAF_UINT8 indexNum, TAF_MMA_RssiInfoInd *rssiInfoInd)
{
    VOS_UINT16 length;
    VOS_UINT32 rptCmdRssi;
    VOS_UINT32 rptCmdCerssi;
#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT32 rptCmdAnlevel;
    VOS_INT16  rsrp;
    VOS_INT16  rsrq;
    VOS_UINT8  level;
    VOS_INT16  rssi;
#endif
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    length = 0;

    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaRssiChangeInd: Get modem id fail.");
        return;
    }

    rptCmdRssi    = AT_CheckRptCmdStatus(rssiInfoInd->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_RSSI);
    rptCmdCerssi  = AT_CheckRptCmdStatus(rssiInfoInd->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CERSSI);
    rptCmdAnlevel = AT_CheckRptCmdStatus(rssiInfoInd->curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_ANLEVEL);

    if ((AT_CheckRptCmdStatus(rssiInfoInd->unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED, AT_RPT_CMD_RSSI) ==
         VOS_TRUE) &&
        (rptCmdRssi == VOS_TRUE)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_RSSI].text,
            rssiInfoInd->rssiInfo.aRssi[0].rssiValue, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    if ((AT_CheckRptCmdStatus(rssiInfoInd->unsolicitedRptCfg, AT_CMD_RPT_CTRL_BY_UNSOLICITED, AT_RPT_CMD_CERSSI) ==
         VOS_TRUE) &&
        (rptCmdCerssi == VOS_TRUE)) {
        systemAppConfig = AT_GetSystemAppConfigAddr();

        if (rssiInfoInd->rssiInfo.ratType == TAF_MMA_RAT_GSM) {
            AT_RcvMmaGsmCERssiChangeInd(indexNum, rssiInfoInd, *systemAppConfig);

            return;
        }

        if (rssiInfoInd->rssiInfo.ratType == TAF_MMA_RAT_WCDMA) {
            AT_RcvMmaWcdmaCERssiChangeInd(indexNum, rssiInfoInd, *systemAppConfig);

            return;
        }

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        if (rssiInfoInd->rssiInfo.ratType == TAF_MMA_RAT_NR) {
            AT_RcvMmaNrCERssiChangeInd(indexNum, rssiInfoInd, *systemAppConfig);

            return;
        }
#endif

        /* 上报LTE 的CERSSI */
#if (FEATURE_LTE == FEATURE_ON)
        if (rssiInfoInd->rssiInfo.ratType == TAF_MMA_RAT_LTE) {
            AT_RcvMmaLteCERssiChangeInd(indexNum, rssiInfoInd, *systemAppConfig);
        }
#endif
    }

    /* 上报ANLEVEL */
#if (FEATURE_LTE == FEATURE_ON)

    length = 0;
    if (rptCmdAnlevel == VOS_TRUE) {
        rsrp = rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrp;
        rsrq = rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rsrq;
        rssi = rssiInfoInd->rssiInfo.aRssi[0].u.lCellSignInfo.rssi;

        AT_CalculateLTESignalValue(&rssi, &level, &rsrp, &rsrq);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s0,99,%d,%d,%d,%d%s", g_atCrLf, "^ANLEVEL:", rssi, level,
            rsrp, rsrq, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }
#endif

    return;
}


VOS_VOID At_StkNumPrint(VOS_UINT16 *length, VOS_UINT8 *data, VOS_UINT16 dataLen, VOS_UINT8 numType)
{
    errno_t    memResult;
    VOS_UINT16 lengthTemp = *length;
    /* 打印数据 */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",\"");

    if (dataLen > 0) {
        memResult = memcpy_s(g_atSndCodeAddress + lengthTemp, AT_CMD_MAX_LEN - lengthTemp , data, dataLen);
        TAF_MEM_CHK_RTN_VAL(memResult, dataLen, dataLen);
    }

    lengthTemp += dataLen;

    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "\"");

    /* 打印类型 */
    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", numType);

    *length = lengthTemp;

    return;
}


VOS_VOID AT_BcdHalfByteToAscii(VOS_UINT8 bcdHalfByte, VOS_UINT8 *asciiNum)
{
    if (bcdHalfByte <= AT_DEC_MAX_NUM) { /* 转换数字 */
        *asciiNum = bcdHalfByte + 0x30;
    } else if (bcdHalfByte == 0x0A) { /* 转换*字符 */
        *asciiNum = 0x2a;
    } else if (bcdHalfByte == 0x0B) { /* 转换#字符 */
        *asciiNum = 0x23;
    } else if (bcdHalfByte == 0x0C) { /* 转换'P'字符 */
        *asciiNum = 0x50;
    } else if (bcdHalfByte == 0x0D) { /* 转换'?'字符 */
        *asciiNum = 0x3F;
    } else { /* 转换字母 */
        *asciiNum = bcdHalfByte + 0x57;
    }

    return;
}


VOS_VOID AT_BcdToAscii(VOS_UINT8 bcdNumLen, VOS_UINT8 *bcdNum, VOS_UINT8 *asciiNum, VOS_UINT8 asciiNumBufflen,
                       VOS_UINT8 *len)
{
    VOS_UINT32 tmp;
    VOS_UINT8 lenTemp = 0;
    VOS_UINT8 firstNumber;
    VOS_UINT8 secondNumber;

    for (tmp = 0; tmp < bcdNumLen; tmp++) {
        if (bcdNum[tmp] == 0xFF) {
            break;
        }

        firstNumber = (VOS_UINT8)(bcdNum[tmp] & 0x0F); /* 取出高半字节 */

        secondNumber = (VOS_UINT8)((bcdNum[tmp] >> 4) & 0x0F); /* 取出低半字节 */

        /* 代码安全走读添加，防止越界访问 */
        if (asciiNumBufflen <= lenTemp) {
            lenTemp = 0;
            break;
        }

        AT_BcdHalfByteToAscii(firstNumber, asciiNum);

        asciiNum++;

        lenTemp++;

        if (secondNumber == 0x0F) {
            break;
        }

        /* 代码安全走读添加，防止越界访问 */
        if (asciiNumBufflen <= lenTemp) {
            lenTemp = 0;
            break;
        }

        AT_BcdHalfByteToAscii(secondNumber, asciiNum);

        asciiNum++;

        lenTemp++;
    }

    *len = lenTemp;

    return;
}

#if ((FEATURE_VSIM == FEATURE_ON) && (FEATURE_VSIM_ICC_SEC_CHANNEL == FEATURE_OFF))

VOS_VOID At_StkHvsmrIndPrint(VOS_UINT8 indexNum, SI_STK_EventInfo *event)
{
    VOS_UINT16 length = 0;

    /* 打印输入AT命令类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, "^HVSMR: ");

    /* 打印输入主动命令类型长度和类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,",
        (event->cmdDataInfo.cmdStru.smsPpDlDataInfo.len * AT_ASCII_AND_HEX_CONVERSION_FACTOR));

    if (event->cmdDataInfo.cmdStru.smsPpDlDataInfo.len != VOS_NULL) {
        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        length += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                           (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                           event->cmdDataInfo.cmdStru.smsPpDlDataInfo.len,
                                                           event->cmdDataInfo.cmdStru.smsPpDlDataInfo.data);
    }

    /* 打印回车换行 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}
#endif


VOS_VOID At_StkCcinIndPrint(VOS_UINT8 indexNum, SI_STK_EventInfo *event)
{
    VOS_UINT16 length                    = 0;
    VOS_UINT8  ascii[AT_ASCII_ARRAY_LEN] = {0};
    VOS_UINT8  asciiLen                  = 0;
    /* 打印输入AT命令类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCcin);

    /* 打印call/sms control 类型 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", (event->cmdDataInfo.cmdStru.ccIndInfo.type));

    /* 打印Call/SMS Control的结果 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", (event->cmdDataInfo.cmdStru.ccIndInfo.result));

    /* 打印ALPHAID标识 */
    if (event->cmdDataInfo.cmdStru.ccIndInfo.alphaIdInfo.alphaLen != 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s",
            (event->cmdDataInfo.cmdStru.ccIndInfo.alphaIdInfo.alphaId));

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",\"\"");
    }

    if (event->cmdDataInfo.cmdStru.ccIndInfo.type == SI_STK_SMS_CTRL) {
        /* 将目的号码由BCD码转换成acsii */
        AT_BcdToAscii(event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.dstAddrInfo.addrLen,
                      event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.dstAddrInfo.addr, ascii, sizeof(ascii),
                      &asciiLen);

        /* 打印目的地址和类型 */
        At_StkNumPrint(&length, ascii, asciiLen,
                       event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.dstAddrInfo.numType);

        /* 将服务中心号码由BCD码转换成acsii */
        AT_BcdToAscii(event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.serCenterAddrInfo.addrLen,
                      event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.serCenterAddrInfo.addr, ascii,
                      sizeof(ascii), &asciiLen);

        /* 打印服务中心地址和类型 */
        At_StkNumPrint(&length, ascii, asciiLen,
                       event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.moSmsCtrlInfo.serCenterAddrInfo.numType);

    } else if (event->cmdDataInfo.cmdStru.ccIndInfo.type == SI_STK_USSD_CALL_CTRL) {
        /* 打印dcs字段和data字段 */
        At_StkNumPrint(&length, event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.data,
                       event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.dataLen,
                       event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.dataType);
    } else {
        /* 将目的号码由BCD码转换成acsii */
        AT_BcdToAscii((VOS_UINT8)event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.dataLen,
                      event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.data, ascii, sizeof(ascii), &asciiLen);

        /* 打印目的地址和类型 */
        At_StkNumPrint(&length, ascii, asciiLen, event->cmdDataInfo.cmdStru.ccIndInfo.uInfo.ctrlDataInfo.dataType);
    }

    /* 打印回车换行 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_CRLF_STR_LEN);

    return;
}


MODULE_EXPORTED VOS_UINT16 AT_GetRealClientId(ModemIdUint16 modemId)
{
    if (modemId == MODEM_ID_0) {
        return AT_BROADCAST_CLIENT_ID_MODEM_0;
    }

    if (modemId == MODEM_ID_1) {
        return AT_BROADCAST_CLIENT_ID_MODEM_1;
    }

    if (modemId == MODEM_ID_2) {
        return AT_BROADCAST_CLIENT_ID_MODEM_2;
    }

    AT_ERR_LOG1("AT_GetRealClientId, enModemId err", modemId);

    return MN_CLIENT_ID_BROADCAST;
}


VOS_VOID AT_ReportCCallstateResult(ModemIdUint16 modemId, VOS_UINT8 callId, VOS_UINT8 *rptCfg,
                                   AT_CS_CallStateUint8 callState, TAF_CALL_VoiceDomainUint8 voiceDomain)
{
    VOS_UINT16 length   = 0;
    VOS_UINT8  indexNum = 0;

    VOS_UINT16 clientId;

    clientId = AT_GetRealClientId(modemId);

    /* 获取client id对应的Modem Id */
    /* 通过clientid获取index */
    if (At_ClientIdToUserId(clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ReportCCallstateResult:WARNING:AT INDEX NOT FOUND!");
        return;
    }

    if (AT_CheckRptCmdStatus(rptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CALLSTATE) == VOS_TRUE) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CCALLSTATE].text, callId, callState, voiceDomain, g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_UINT32 AT_RcvTafCcmCallHoldInd(struct MsgCB *msg)
{
    TAF_CCM_CallHoldInd *callHoldInd = VOS_NULL_PTR;
    VOS_UINT32           loop;
    VOS_UINT32           rslt;
    ModemIdUint16        modemId;
    VOS_UINT8            indexNum = 0;

    callHoldInd = (TAF_CCM_CallHoldInd *)msg;

    if (At_ClientIdToUserId(callHoldInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallHoldInd At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafCcmCallHoldInd: Get modem id fail.");
        return VOS_ERR;
    }

    callHoldInd->holdIndPara.callNum = AT_MIN(callHoldInd->holdIndPara.callNum, MN_CALL_MAX_NUM);
    for (loop = 0; loop < (callHoldInd->holdIndPara.callNum); loop++) {
        AT_ReportCCallstateResult(modemId, callHoldInd->holdIndPara.callId[loop], callHoldInd->holdIndPara.curcRptCfg,
                                  AT_CS_CALL_STATE_HOLD, callHoldInd->holdIndPara.voiceDomain);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallRetrieveInd(struct MsgCB *msg)
{
    TAF_CCM_CallRetrieveInd *callRetrieveInd = VOS_NULL_PTR;
    VOS_UINT32               loop;
    VOS_UINT32               rslt;
    ModemIdUint16            modemId;
    VOS_UINT8                indexNum = 0;

    callRetrieveInd = (TAF_CCM_CallRetrieveInd *)msg;

    if (At_ClientIdToUserId(callRetrieveInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallHoldInd At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafCcmCallHoldInd: Get modem id fail.");
        return VOS_ERR;
    }

    callRetrieveInd->retrieveInd.callNum = AT_MIN(callRetrieveInd->retrieveInd.callNum, MN_CALL_MAX_NUM);
    for (loop = 0; loop < (callRetrieveInd->retrieveInd.callNum); loop++) {
        AT_ReportCCallstateResult(modemId, callRetrieveInd->retrieveInd.callId[loop],
                                  callRetrieveInd->retrieveInd.curcRptCfg, AT_CS_CALL_STATE_RETRIEVE,
                                  callRetrieveInd->retrieveInd.voiceDomain);
    }

    return VOS_OK;
}

VOS_VOID AT_ReportCendResult(VOS_UINT8 indexNum, TAF_CCM_CallReleasedInd *callReleaseInd)
{
    VOS_UINT16    length;
    ModemIdUint16 modemId;
    VOS_UINT32    rslt;

    modemId = MODEM_ID_0;

    length = 0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_ReportCendResult: Get modem id fail.");
        return;
    }

    if (AT_CheckRptCmdStatus(callReleaseInd->releaseIndPara.curcRptCfg, AT_CMD_RPT_CTRL_BY_CURC, AT_RPT_CMD_CEND) ==
        VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^CEND:%d,%d,%d,%d%s", g_atCrLf,
            callReleaseInd->releaseIndPara.callId,
            callReleaseInd->releaseIndPara.preCallTime,
            callReleaseInd->releaseIndPara.noCliCause, callReleaseInd->releaseIndPara.cause, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }

    return;
}

VOS_UINT16 At_PrintClprInfo(VOS_UINT8 indexNum, TAF_CCM_QryClprCnf *clprGetCnf)
{
    errno_t    memResult;
    VOS_UINT16 length;
    VOS_UINT8  type;
    VOS_CHAR   asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    VOS_UINT32 asiciiLen;
    VOS_UINT8  bcdNumLen;

    /* 初始化 */
    length = 0;
    memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* 输出<RedirectNumPI>参数 */
    if (clprGetCnf->clprCnf.clprInfo.opCallingNumPi == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", clprGetCnf->clprCnf.clprInfo.callingNumPi);

    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    /* 输出<no_CLI_cause>参数 */
    if (clprGetCnf->clprCnf.clprInfo.opNoCliCause == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", clprGetCnf->clprCnf.clprInfo.noCliCause);

    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    /* 输出<redirect_num>和<num_type>参数 */
    if (clprGetCnf->clprCnf.clprInfo.redirectInfo.opRedirectNum == VOS_TRUE) {
        bcdNumLen = TAF_MIN(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectNum.numLen, MN_CALL_MAX_BCD_NUM_LEN);
        AT_BcdNumberToAscii(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectNum.bcdNum, bcdNumLen, asciiNum);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\",%d,", asciiNum,
            (clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectNum.numType | AT_NUMBER_TYPE_EXT));

    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"\",,");
    }

    /* 输出<redirect_subaddr>和<num_type>参数 */
    type = (MN_CALL_IS_EXIT | (MN_CALL_SUBADDR_NSAP << 4));
    if ((clprGetCnf->clprCnf.clprInfo.redirectInfo.opRedirectSubaddr == VOS_TRUE) &&
        (type == clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.octet3)) {
        if (clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.lastOctOffset <
            sizeof(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.octet3)) {
            AT_WARN_LOG1("At_PrintClprInfo: pstClprGetCnf->stRedirectInfo.stRedirectSubaddr.LastOctOffset: ",
                         clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.lastOctOffset);
            clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.lastOctOffset =
                sizeof(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.octet3);
        }

        asiciiLen = clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.lastOctOffset -
                    sizeof(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.octet3);

        asiciiLen = TAF_MIN(asiciiLen, sizeof(clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.subAddrInfo));
        if (asiciiLen > 0) {
            memResult = memcpy_s(asciiNum, sizeof(asciiNum),
                                 clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.subAddrInfo, asiciiLen);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNum), asiciiLen);
        }

        asciiNum[asiciiLen] = '\0';

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\",%d,", asciiNum,
            clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectSubaddr.octet3);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"\",,");
    }

    /* 输出<CallingNumPI>参数 */
    if (clprGetCnf->clprCnf.clprInfo.redirectInfo.opRedirectNumPi == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d", clprGetCnf->clprCnf.clprInfo.redirectInfo.redirectNumPi);
    }

    return length;
}

VOS_UINT32 AT_RcvTafCcmQryClprCnf(struct MsgCB *msg)
{
    TAF_CCM_QryClprCnf *clprGetCnf = VOS_NULL_PTR;
    VOS_UINT16          length;
    VOS_UINT8           indexNum;

    /* 初始化 */
    clprGetCnf = (TAF_CCM_QryClprCnf *)msg;
    indexNum   = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(clprGetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_SetClprCnf: WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    /* 如果为广播类型，则返回AT_ERROR */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_SetClprCnf: WARNING:AT_BROADCAST_INDEX!");

        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CLPR_SET */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLPR_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCLPR_SET)) {
        AT_WARN_LOG("At_SetClprCnf: WARNING:Not AT_CMD_CLPR_GET!");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (clprGetCnf->clprCnf.ret != TAF_ERR_NO_ERROR) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    length = 0;

    if (clprGetCnf->clprCnf.qryClprModeType == TAF_CALL_QRY_CLPR_MODE_GUL) {
        length = At_PrintClprInfo(indexNum, clprGetCnf);
    } else {
        if (clprGetCnf->clprCnf.pi != TAF_CALL_PRESENTATION_BUTT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                clprGetCnf->clprCnf.pi);
        }
    }

    /* 打印结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSwverSetCnf(struct MsgCB *msg)
{
    VOS_UINT32             ret;
    VOS_UINT8              indexNum = 0;
    DRV_AGENT_SwverSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg         *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_SwverSetCnf *)rcvMsg->content;

    AT_PR_LOGI("Rcv Msg");

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSwverSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSwverSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待SWVER查询命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SWVER_SET) {
        AT_WARN_LOG("AT_RcvDrvAgentSwverSetCnf: WARNING:Not AT_CMD_SWVER_SET!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_NO_ERROR) {
        /*
         * 设置错误码为AT_OK           构造结构为<CR><LF>^SWVER: <SwVer>_(<VerTime>)<CR><LF>
         * <CR><LF>OK<CR><LF>格式
         */
        ret = AT_OK;

        /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
        event->swverInfo.swVer[TAF_MAX_REVISION_ID_LEN]             = '\0';
        event->swverInfo.verTime[AT_AGENT_DRV_VERSION_TIME_LEN - 1] = '\0';
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s_(%s)", g_parseContext[indexNum].cmdElement->cmdName,
            event->swverInfo.swVer, event->swverInfo.verTime);

    } else {
        /* 查询失败返回ERROR字符串 */
        ret                     = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmSetCssnCnf(struct MsgCB *msg)
{
    TAF_CCM_SetCssnCnf *cssnCnf  = VOS_NULL_PTR;
    VOS_UINT8           indexNum = 0;
    VOS_UINT32          result;

    cssnCnf = (TAF_CCM_SetCssnCnf *)msg;

    if (At_ClientIdToUserId(cssnCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvMnCallSetCssnCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvMnCallSetCssnCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待设置命令的回复结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSSN_SET) {
        AT_WARN_LOG("At_RcvMnCallSetCssnCnf: WARNING:Not AT_CMD_APP_SET_CSSN_REQ!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cssnCnf->ret == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_CME_INCORRECT_PARAMETERS;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


TAF_CALL_ChannelTypeUint8 AT_ConvertCodecTypeToChannelType(VOS_UINT8              isLocalAlertingFlag,
                                                           MN_CALL_CodecTypeUint8 codecType)
{
    TAF_CALL_ChannelTypeUint8 channelType;

    channelType = TAF_CALL_CHANNEL_TYPE_NONE;

    if (isLocalAlertingFlag == VOS_TRUE) {
        /* 本地振铃,无带内音信息 */
        channelType = TAF_CALL_CHANNEL_TYPE_NONE;
    } else {
        /* 网络振铃，需要设置channel type */
        /* 带内音可用，默认窄带语音 */
        channelType = TAF_CALL_CHANNEL_TYPE_NARROW;

        if ((codecType == MN_CALL_CODEC_TYPE_AMRWB) || (codecType == MN_CALL_CODEC_TYPE_EVS)) {
            /* 带内音可用，宽带语音 */
            channelType = TAF_CALL_CHANNEL_TYPE_WIDE;
        }

        if (codecType == MN_CALL_CODEC_TYPE_EVS_NB) {
            channelType = TAF_CALL_CHANNEL_TYPE_EVS_NB;
        }

        if (codecType == MN_CALL_CODEC_TYPE_EVS_WB) {
            channelType = TAF_CALL_CHANNEL_TYPE_EVS_WB;
        }

        if (codecType == MN_CALL_CODEC_TYPE_EVS_SWB) {
            channelType = TAF_CALL_CHANNEL_TYPE_EVS_SWB;
        }
    }

    return channelType;
}


VOS_UINT32 AT_RcvTafCcmQryXlemaCnf(struct MsgCB *msg)
{
    TAF_CCM_QryXlemaCnf *qryXlemaCnf = VOS_NULL_PTR;
    VOS_UINT8            asciiNum[(MN_CALL_MAX_BCD_NUM_LEN * 2) + 1];
    VOS_UINT8            indexNum;
    VOS_UINT32           i;
    VOS_UINT16           length;
    VOS_UINT32           mcc;

    qryXlemaCnf = (TAF_CCM_QryXlemaCnf *)msg;
    indexNum    = 0;
    mcc         = 0;

    memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryXlemaCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvXlemaQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 如果为广播类型，则返回 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvXlemaQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_XLEMA_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_XLEMA_QRY) {
        AT_WARN_LOG("At_RcvXlemaQryCnf: WARNING:Not AT_CMD_XLEMA_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 向APP逐条上报紧急呼号码 */
    qryXlemaCnf->qryXlemaPara.eccNumCount = AT_MIN(qryXlemaCnf->qryXlemaPara.eccNumCount, MN_CALL_MAX_EMC_NUM);
    for (i = 0; i < qryXlemaCnf->qryXlemaPara.eccNumCount; i++) {
        /* 将BCD码转化为ASCII码 */
        AT_BcdNumberToAscii(qryXlemaCnf->qryXlemaPara.customEccNumList[i].eccNum,
                            qryXlemaCnf->qryXlemaPara.customEccNumList[i].eccNumLen, (VOS_CHAR *)asciiNum);

        /* 将NAS格式的MCC转化为BCD格式 */
        AT_ConvertNasMccToBcdType(qryXlemaCnf->qryXlemaPara.customEccNumList[i].mcc, &mcc);

        length = 0;
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s^XLEMA: ", g_atCrLf);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d,%s,%d,%d,", (i + 1), qryXlemaCnf->qryXlemaPara.eccNumCount,
            asciiNum, qryXlemaCnf->qryXlemaPara.customEccNumList[i].category,
            qryXlemaCnf->qryXlemaPara.customEccNumList[i].validSimPresent);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x", (mcc & 0x0f00) >> 8, (mcc & 0xf0) >> 4, (mcc & 0x0f));

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d%s",
            qryXlemaCnf->qryXlemaPara.customEccNumList[i].abnormalServiceFlg, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);

        memset_s(asciiNum, sizeof(asciiNum), 0x00, sizeof(asciiNum));
    }

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmStartDtmfCnf(struct MsgCB *msg)
{
    TAF_CCM_StartDtmfCnf *dtmfCNf  = VOS_NULL_PTR;
    VOS_UINT8             indexNum = 0;

    dtmfCNf = (TAF_CCM_StartDtmfCnf *)msg;

    /* 根据ClientID获取通道索引 */
    if (At_ClientIdToUserId(dtmfCNf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallStartDtmfCnf: Get Index Fail!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCallStartDtmfCnf: At Is Broadcast!");
        return VOS_ERR;
    }

    /* AT模块在等待^DTMF命令/+VTS命令的操作结果事件上报 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DTMF_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VTS_SET)) {
        AT_WARN_LOG("AT_RcvTafCallStartDtmfCnf: Error Option!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 根据临时响应的错误码打印命令的结果 */
    if (dtmfCNf->startDtmfPara.cause != TAF_CS_CAUSE_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmStopDtmfCnf(struct MsgCB *msg)
{
    TAF_CCM_StopDtmfCnf *dtmfCNf  = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;

    dtmfCNf = (TAF_CCM_StopDtmfCnf *)msg;

    /* 根据ClientID获取通道索引 */
    if (At_ClientIdToUserId(dtmfCNf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallStopDtmfCnf: Get Index Fail!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCallStopDtmfCnf: At Is Broadcast!");
        return VOS_ERR;
    }

    /* AT模块在等待^DTMF命令/+VTS命令的操作结果事件上报 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DTMF_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VTS_SET)) {
        AT_WARN_LOG("AT_RcvTafCallStopDtmfCnf: Error Option!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 根据临时响应的错误码打印命令的结果 */
    if (dtmfCNf->stopDtmfPara.cause != TAF_CS_CAUSE_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 At_RcvTafCcmCallOrigCnf(struct MsgCB *msg)
{
    TAF_UINT8            indexNum;
    TAF_CCM_CallOrigCnf *origCnf = VOS_NULL_PTR;
    TAF_UINT32           result;
    TAF_UINT16           length;

    result   = AT_FAILURE;
    length   = 0;
    indexNum = 0;

    origCnf = (TAF_CCM_CallOrigCnf *)msg;

    AT_LOG1("At_RcvTafCallOrigCnf pEvent->ClientId", origCnf->ctrl.clientId);
    AT_LOG1("At_RcvTafCallOrigCnf usMsgName", origCnf->msgName);

    if (At_ClientIdToUserId(origCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_CsEventProc At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvTafCallOrigCnf: At Is Broadcast!");
        return VOS_ERR;
    }

    /* 成功时，回复OK；失败时，回复NO CARRIER */
    if (origCnf->origCnfPara.cause == TAF_CS_CAUSE_SUCCESS) {
        /* 可视电话里面，这里不能上报OK，因此只有普通语音和紧急呼叫的情况下，才上报OK，AT命令在这个阶段相当于阻塞一段时间 */
        if (At_CheckOrigCnfCallType(origCnf->origCnfPara.callType, indexNum) == PS_TRUE) {
            result = AT_OK;
        } else {
            if (origCnf->origCnfPara.callType == MN_CALL_TYPE_VIDEO) {
                AT_STOP_TIMER_CMD_READY(indexNum);
            }
            return VOS_OK;
        }

    } else {
        if (origCnf->origCnfPara.cause == TAF_CS_CAUSE_NO_CALL_ID) {
            result = AT_ERROR;
        } else {
            result = AT_NO_CARRIER;
        }

        AT_UpdateCallErrInfo(indexNum, origCnf->origCnfPara.cause, &(origCnf->origCnfPara.errInfoText));
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


PS_BOOL_ENUM_UINT8 AT_CheckCurrentOptType_SupsCmdSuccess(VOS_UINT8 ataReportOkAsyncFlag, TAF_UINT8 indexNum)
{
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_H_SET:
        case AT_CMD_CHUP_SET:
        case AT_CMD_REJCALL_SET:
#if (FEATURE_ECALL == FEATURE_ON)
        case AT_CMD_ECLSTOP_SET:
#endif
            return PS_TRUE;
        case AT_CMD_A_SET:
        case AT_CMD_CHLD_SET:
        case AT_CMD_CHLD_EX_SET:
            if (ataReportOkAsyncFlag == VOS_TRUE) {
                return PS_TRUE;
            }

            return PS_FALSE;
        default:
            return PS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 AT_CheckCurrentOptType_SupsCmdOthers(TAF_UINT8 indexNum)
{
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CHLD_SET:
        case AT_CMD_CHUP_SET:
        case AT_CMD_A_SET:
        case AT_CMD_CHLD_EX_SET:
        case AT_CMD_H_SET:
        case AT_CMD_REJCALL_SET:
            return PS_TRUE;

        default:
            return PS_FALSE;
    }
}


VOS_UINT32 At_RcvTafCcmCallSupsCmdCnf(struct MsgCB *msg)
{
    TAF_UINT8               indexNum;
    TAF_CCM_CallSupsCmdCnf *callSupsCmdCnf = VOS_NULL_PTR;
    TAF_UINT32              result;
    TAF_UINT16              length;

    result   = AT_FAILURE;
    length   = 0;
    indexNum = 0;

    callSupsCmdCnf = (TAF_CCM_CallSupsCmdCnf *)msg;

    AT_LOG1("At_RcvTafCallOrigCnf pEvent->ClientId", callSupsCmdCnf->ctrl.clientId);
    AT_LOG1("At_RcvTafCallOrigCnf usMsgName", callSupsCmdCnf->msgName);

    AT_PR_LOGH("At_RcvTafCcmCallSupsCmdCnf Enter : enCause = %d", callSupsCmdCnf->supsCmdPara.cause);

    if (At_ClientIdToUserId(callSupsCmdCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvTafCallSupsCmdCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvTafCallSupsCmdCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT口已经释放 */
    if (g_parseContext[indexNum].clientStatus == AT_FW_CLIENT_STATUS_READY) {
        AT_WARN_LOG("At_RcvTafCallSupsCmdCnf : AT command entity is released.");
        return VOS_ERR;
    }

    /* 挂断电话成功时回复OK */
    if (callSupsCmdCnf->supsCmdPara.cause == TAF_CS_CAUSE_SUCCESS) {
        if (AT_CheckCurrentOptType_SupsCmdSuccess(callSupsCmdCnf->supsCmdPara.ataReportOkAsyncFlag, indexNum) ==
            PS_TRUE) {
            result = AT_OK;
        } else {
            return VOS_OK;
        }
    } else {
        if (AT_CheckCurrentOptType_SupsCmdOthers(indexNum) == PS_TRUE) {
            result = AT_ConvertCallError(callSupsCmdCnf->supsCmdPara.cause);
        } else {
            result = AT_CME_UNKNOWN;
        }
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallSupsCmdRsltInd(struct MsgCB *msg)
{
    TAF_CCM_CallSupsCmdRslt *callSupsCmdRslt = VOS_NULL_PTR;
    VOS_UINT32               result          = 0;
    TAF_UINT16               length          = 0;
    VOS_UINT8                indexNum        = 0;

    callSupsCmdRslt = (TAF_CCM_CallSupsCmdRslt *)msg;

    AT_PR_LOGH("AT_RcvTafCcmCallSupsCmdRsltInd Enter : enCause = %d", callSupsCmdRslt->supsCmdRsltPara.ssResult);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(callSupsCmdRslt->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallSupsCmdRsltInd: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmCallSupsCmdRsltInd: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CURRENT_OPT_BUTT) {
        return VOS_ERR;
    }

    if (callSupsCmdRslt->supsCmdRsltPara.ssResult == MN_CALL_SS_RES_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID At_CsIndEvtReleaseProc(TAF_UINT8 indexNum, TAF_CCM_CallReleasedInd *callReleaseInd)
{
    AT_ModemCcCtx *ccCtx = VOS_NULL_PTR;
    VOS_UINT32     timerName;

    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);

    AT_UpdateCallErrInfo(indexNum, callReleaseInd->releaseIndPara.cause, &(callReleaseInd->releaseIndPara.errInfoText));

    if (ccCtx->s0TimeInfo.timerStart == VOS_TRUE) {
        timerName = ccCtx->s0TimeInfo.timerName;

        AT_StopRelTimer(timerName, &(ccCtx->s0TimeInfo.s0Timer));
        ccCtx->s0TimeInfo.timerStart = VOS_FALSE;
        ccCtx->s0TimeInfo.timerName  = 0;
    }

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /* 停止RING TE */
    AT_VoiceStopRingTe(callReleaseInd->releaseIndPara.callId);
#endif

    AT_ReportCendResult(indexNum, callReleaseInd);

    return;
}


VOS_UINT32 AT_RcvTafCcmCallReleaseInd(struct MsgCB *msg)
{
    TAF_CCM_CallReleasedInd *callReleaseInd = VOS_NULL_PTR;
    VOS_UINT8                indexNum       = 0;
    ModemIdUint16            modemId;
    VOS_UINT32               rslt;

    modemId = MODEM_ID_0;

    callReleaseInd = (TAF_CCM_CallReleasedInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(callReleaseInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallReleaseInd: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        At_CsIndEvtReleaseProc(indexNum, callReleaseInd);
    } else {
        AT_LOG1("AT_RcvTafCcmCallReleaseInd ucIndex", indexNum);
        AT_LOG1("AT_RcvTafCcmCallReleaseInd[ucIndex].CmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

        AT_CsRspEvtReleasedProc(indexNum, callReleaseInd);
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafCcmCallReleaseInd: Get modem id fail.");
        return VOS_ERR;
    }

    AT_ReportCCallstateResult(modemId, callReleaseInd->releaseIndPara.callId, callReleaseInd->releaseIndPara.curcRptCfg,
                              AT_CS_CALL_STATE_RELEASED, callReleaseInd->releaseIndPara.voiceDomain);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallAllReleasedInd(struct MsgCB *msg)
{
    AT_ModemCcCtx              *ccCtx          = VOS_NULL_PTR;
    TAF_CCM_CallAllReleasedInd *callReleaseInd = VOS_NULL_PTR;
    VOS_UINT8                   indexNum       = 0;

    callReleaseInd = (TAF_CCM_CallAllReleasedInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(callReleaseInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallAllReleased: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    ccCtx                     = AT_GetModemCcCtxAddrFromClientId(indexNum);
    ccCtx->curIsExistCallFlag = VOS_FALSE;

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallAlertingInd(struct MsgCB *msg)
{
    TAF_CCM_CallAlertingInd *callAlertingInd = VOS_NULL_PTR;
    VOS_UINT32               rslt;
    ModemIdUint16            modemId;
    VOS_UINT8                indexNum = 0;

    callAlertingInd = (TAF_CCM_CallAlertingInd *)msg;

    if (At_ClientIdToUserId(callAlertingInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallAlertingInd At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafCcmCallAlertingInd: Get modem id fail.");
        return VOS_ERR;
    }

    AT_ReportCCallstateResult(modemId, callAlertingInd->alertingIndPara.callId,
                              callAlertingInd->alertingIndPara.curcRptCfg, AT_CS_CALL_STATE_ALERTING,
                              callAlertingInd->alertingIndPara.voiceDomain);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallSsInd(struct MsgCB *msg)
{
    TAF_CCM_CallSsInd *callSsInd = VOS_NULL_PTR;
    VOS_UINT8          indexNum  = 0;

    callSsInd = (TAF_CCM_CallSsInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(callSsInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallSsInd: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_CsSsNotifyEvtIndProc(indexNum, callSsInd);
    } else {
        AT_LOG1("AT_RcvTafCcmCallSsInd ucIndex", indexNum);
        AT_LOG1("AT_RcvTafCcmCallSsInd[ucIndex].CmdCurrentOpt", g_atClientTab[indexNum].cmdCurrentOpt);

        AT_ProcCsRspEvtSsNotify(indexNum, callSsInd);
    }

    return VOS_OK;
}

#if ((FEATURE_VSIM == FEATURE_ON) && (FEATURE_VSIM_ICC_SEC_CHANNEL == FEATURE_OFF))

VOS_UINT32 AT_RcvDrvAgentHvpdhSetCnf(struct MsgCB *msg)
{
    VOS_UINT32          ret;
    VOS_UINT8           indexNum;
    DRV_AGENT_HvpdhCnf *event  = VOS_NULL_PTR;
    DRV_AGENT_Msg      *rcvMsg = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_HvpdhCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentHvpdhSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentHvpdhSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待HVPDH设置命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HVPDH_SET) {
        AT_WARN_LOG("AT_RcvDrvAgentHvpdhSetCnf: WARNING:Not AT_CMD_HVPDH_SET!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (event->result == DRV_AGENT_HVPDH_NO_ERROR) {
        ret = AT_OK;
    } else {
        /* 查询失败返回ERROR字符串 */
        ret = AT_ERROR;
    }

    g_atSendDataBuff.usBufLen = 0;

    /* 调用At_FormatResultData输出结果 */
    At_FormatResultData(indexNum, ret);
    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvNvManufactureExtSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                         indexNum = 0;
    DRV_AGENT_NvmanufactureextSetCnf *event    = VOS_NULL_PTR;
    DRV_AGENT_Msg                    *rcvMsg   = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvmanufactureextSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvNvManufactureExtSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvNvManufactureExtSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待NvManufactureExt设置命令的结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVMANUFACTUREEXT_SET) {
        AT_WARN_LOG("AT_RcvNvManufactureExtSetCnf: WARNING:Not AT_CMD_NVMANUFACTUREEXT_SET!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->rslt == NV_OK) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%d", event->rslt);
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_VOID At_ChangeEcallTypeToCallType(MN_CALL_TypeUint8 ecallType, MN_CALL_TypeUint8 *callType)
{
    switch (ecallType) {
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
        case MN_CALL_TYPE_PSAP_ECALL:
            *callType = MN_CALL_TYPE_VOICE;
            break;

        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
            *callType = MN_CALL_TYPE_EMERGENCY;
            break;

        default:
            *callType = ecallType;
            break;
    }
}


VOS_UINT32 At_IsCmdCurrentOptSendedOrigReq(AT_CmdCurrentOpt cmdCurrentOpt)
{
    switch (cmdCurrentOpt) {
        case AT_CMD_APDS_SET:
        case AT_CMD_D_CS_VOICE_CALL_SET:
        case AT_CMD_CECALL_SET:
        case AT_CMD_ECLSTART_SET:
        case AT_CMD_CACMIMS_SET:
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case AT_CMD_CUSTOMDIAL_SET:
#endif
            return VOS_TRUE;

        default:
            return VOS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 At_CheckOrigCnfCallType(MN_CALL_TypeUint8 callType, VOS_UINT8 indexNum)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
        case MN_CALL_TYPE_VIDEO_RX:
        case MN_CALL_TYPE_VIDEO_TX:
        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
            if (At_IsCmdCurrentOptSendedOrigReq(g_atClientTab[indexNum].cmdCurrentOpt) == VOS_TRUE) {
                return PS_TRUE;
            }

            return PS_FALSE;
        case MN_CALL_TYPE_VIDEO:
            if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_APDS_SET) {
                return PS_TRUE;
            } else {
                return PS_FALSE;
            }
            /* fall-through */

        default:
            return PS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 At_CheckReportCendCallType(MN_CALL_TypeUint8 callType)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
        case MN_CALL_TYPE_PSAP_ECALL:
            return PS_TRUE;
        default:
            return PS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 At_CheckReportOrigCallType(MN_CALL_TypeUint8 callType)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
            return PS_TRUE;

        default:
            return PS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 At_CheckReportConfCallType(MN_CALL_TypeUint8 callType)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
        case MN_CALL_TYPE_PSAP_ECALL:
            return PS_TRUE;
        default:
            return PS_FALSE;
    }
}


PS_BOOL_ENUM_UINT8 At_CheckUartRingTeCallType(MN_CALL_TypeUint8 callType)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
        case MN_CALL_TYPE_PSAP_ECALL:
            return PS_TRUE;
        default:
            return PS_FALSE;
    }
}

#if (FEATURE_ECALL == FEATURE_ON)

VOS_UINT32 At_ProcVcReportEcallStateEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    if ((vcEvtInfo->ecallState == APP_VC_ECALL_MSD_TRANSMITTING_FAIL) ||
        (vcEvtInfo->ecallState == APP_VC_ECALL_PSAP_MSD_REQUIRETRANSMITTING)) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECLSTAT: %d,%d%s", g_atCrLf, vcEvtInfo->ecallState,
            vcEvtInfo->ecallDescription, g_atCrLf);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECLSTAT: %d%s", g_atCrLf, vcEvtInfo->ecallState, g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 At_ProcVcSetEcallCfgEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    VOS_UINT32 rslt;

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_ProcVcSetMuteStatusEvent : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLCFG_SET) {
        return VOS_ERR;
    }

    if (vcEvtInfo->success != VOS_TRUE) {
        rslt = AT_ERROR;
    } else {
        rslt = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_VOID At_MapEcallType(MN_CALL_TypeUint8 callType, AT_ECALL_TypeUint8 *ecallType)
{
    switch (callType) {
        case MN_CALL_TYPE_TEST:
            *ecallType = AT_ECALL_TYPE_TEST;
            break;
        case MN_CALL_TYPE_RECFGURATION:
            *ecallType = AT_ECALL_TYPE_RECFGURATION;
            break;
        case MN_CALL_TYPE_MIEC:
            *ecallType = AT_ECALL_TYPE_MIEC;
            break;
        case MN_CALL_TYPE_AIEC:
            *ecallType = AT_ECALL_TYPE_AIEC;
            break;
        default:
            *ecallType = AT_ECALL_TYPE_BUTT;
            break;
    }
    return;
}

#endif

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_RcvTafCcmCallModifyStatusInd(struct MsgCB *msg)
{
    TAF_CCM_CallModifyStatusInd *statusInd = VOS_NULL_PTR;
    VOS_UINT16                   length;
    VOS_UINT8                    indexNum;

    length    = 0;
    indexNum  = 0;
    statusInd = (TAF_CCM_CallModifyStatusInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(statusInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCallModifyStatusInd:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (statusInd->modifyStatus == MN_CALL_MODIFY_REMOTE_USER_REQUIRE_TO_MODIFY) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CALL_MODIFY_IND].text, statusInd->callId, statusInd->currCallType,
            statusInd->voiceDomain, statusInd->expectCallType, statusInd->voiceDomain, statusInd->modifyReason,
            g_atCrLf);
    } else if (statusInd->modifyStatus == MN_CALL_MODIFY_PROC_BEGIN) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CALL_MODIFY_BEG].text, statusInd->callId, statusInd->voiceDomain, g_atCrLf);
    } else if (statusInd->modifyStatus == MN_CALL_MODIFY_PROC_END) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CALL_MODIFY_END].text, statusInd->callId, statusInd->voiceDomain, statusInd->cause,
            g_atCrLf);
    } else {
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmEconfDialCnf(struct MsgCB *msg)
{
    TAF_CCM_EconfDialCnf *econfDialCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;

    indexNum = 0;

    econfDialCnf = (TAF_CCM_EconfDialCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(econfDialCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafEconfDialCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafEconfDialCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECONF_DIAL_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CACMIMS_SET)) {
        AT_WARN_LOG("AT_RcvTafEconfDialCnf: WARNING:Not AT_CMD_ECONF_DIAL_SET or AT_CMD_CACMIMS_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断操作是否成功 */
    if (econfDialCnf->econfDialCnf.cause != TAF_CS_CAUSE_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmEconfNotifyInd(struct MsgCB *msg)
{
    errno_t                 memResult;
    VOS_UINT8               indexNum = 0;
    VOS_UINT16              length;
    TAF_CCM_EconfNotifyInd *notifyInd = VOS_NULL_PTR;
    AT_ModemCcCtx          *ccCtx     = VOS_NULL_PTR;

    length    = 0;
    notifyInd = (TAF_CCM_EconfNotifyInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(notifyInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafEconfNotifyInd:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    ccCtx = AT_GetModemCcCtxAddrFromClientId(notifyInd->ctrl.clientId);
    ccCtx->econfInfo.numOfCalls = AT_MIN(notifyInd->econfNotifyIndPara.numOfCalls, TAF_CALL_MAX_ECONF_CALLED_NUM);

    if (ccCtx->econfInfo.numOfCalls > 0) {
        memResult = memcpy_s(ccCtx->econfInfo.callInfo, sizeof(ccCtx->econfInfo.callInfo),
                             notifyInd->econfNotifyIndPara.callInfo,
                             (sizeof(TAF_CALL_EconfInfoParam) * ccCtx->econfInfo.numOfCalls));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ccCtx->econfInfo.callInfo),
                            (sizeof(TAF_CALL_EconfInfoParam) * ccCtx->econfInfo.numOfCalls));
    }

    /* call_num取pstNotifyInd->ucNumOfCalls，而不是pstCcCtx->stEconfInfo.ucNumOfCalls，可以方便发现错误 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d%s", g_atCrLf, g_atStringTab[AT_STRING_ECONFSTATE].text,
        notifyInd->econfNotifyIndPara.numOfCalls, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCcwaiSetCnf(struct MsgCB *msg)
{
    VOS_UINT8            indexNum = 0;
    TAF_CCM_CcwaiSetCnf *ccwaiCnf = VOS_NULL_PTR;
    VOS_UINT32           result;

    ccwaiCnf = (TAF_CCM_CcwaiSetCnf *)msg;

    /* 根据ClientID获取通道索引 */
    if (At_ClientIdToUserId(ccwaiCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCcwaiSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCallCcwaiSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* AT模块在等待^CCWAI命令的操作结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCWAI_SET) {
        AT_WARN_LOG("AT_RcvTafCallCcwaiSetCnf: WARNING:Not AT_CMD_CCWAI_SET!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (ccwaiCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCancelAddVideoCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    TAF_CCM_CancelAddVideoCnf *videoCnf = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    videoCnf = (TAF_CCM_CancelAddVideoCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(videoCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCancelAddVideoCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmCancelAddVideoCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSVIDEOCALLCANCEL_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSVIDEOCALLCANCEL_SET) {
        AT_WARN_LOG("AT_RcvTafCcmCancelAddVideoCnf: WARNING:Not AT_CMD_IMSVIDEOCALLCANCEL_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断设置操作是否成功 */
    if (videoCnf->result == VOS_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvTafPsEvtSetImsPdpCfgCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetImsPdpCfgCnf *setImsPdpCfgCnf = VOS_NULL_PTR;

    setImsPdpCfgCnf = (TAF_PS_SetImsPdpCfgCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSPDPCFG_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setImsPdpCfgCnf->cause);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvTafPsEvtSet1xDormTimerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_Set1xDormTimerCnf *set1xDormTimerCnf = VOS_NULL_PTR;

    set1xDormTimerCnf = (TAF_PS_Set1xDormTimerCnf *)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DORMTIMER_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, set1xDormTimerCnf->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGet1xDormTimerCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_Get1XDormTimerCnf *get1xDormTiCnf = VOS_NULL_PTR;
    VOS_UINT16                length;

    get1xDormTiCnf = (TAF_PS_Get1XDormTimerCnf *)evtInfo;
    length         = 0;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DORMTIMER_QRY) {
        return VOS_ERR;
    }

    /* 上报查询结果 */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        get1xDormTiCnf->socmDormTiVal, get1xDormTiCnf->userCfgDormTival);

    g_atSendDataBuff.bufLen = length;

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}
#endif



VOS_UINT32 AT_RcvTafPsCallEvtLimitPdpActInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_CallLimitPdpActInd *limitPdpActInd = VOS_NULL_PTR;
    ModemIdUint16              modemId;
    VOS_UINT16                 length;

    length         = 0;
    modemId        = MODEM_ID_0;
    limitPdpActInd = (TAF_PS_CallLimitPdpActInd *)evtInfo;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsCallEvtLimitPdpActInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ^LIMITPDPACT: <FLG>,<CAUSE><CR><LF> */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_LIMITPDPACT].text,
        limitPdpActInd->limitFlg, limitPdpActInd->cause, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_CHAR* AT_GetPdpTypeStr(TAF_PDP_TypeUint8 pdpType)
{
    switch (pdpType) {
        case TAF_PDP_IPV4:
            return (VOS_CHAR *)g_atStringTab[AT_STRING_IPv4].text;
        case TAF_PDP_IPV6:
            return (VOS_CHAR *)g_atStringTab[AT_STRING_IPv6].text;
        case TAF_PDP_IPV4V6:
            return (VOS_CHAR *)g_atStringTab[AT_STRING_IPv4v6].text;
        case TAF_PDP_ETHERNET:
            return (VOS_CHAR *)g_atStringTab[AT_STRING_Ethernet].text;
        default:
            AT_WARN_LOG1("AT_GetPdpTypeStr: Invalid PDP Type!", pdpType);
            break;
    }

    return (VOS_CHAR *)g_atStringTab[AT_STRING_IP].text;
}


VOS_UINT32 AT_RcvTafPsEvtSetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SET_DATA_SWITCH_CNF_STRU *setDataSwitchCnf = VOS_NULL_PTR;
    setDataSwitchCnf                                  = (TAF_PS_SET_DATA_SWITCH_CNF_STRU *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DATASWITCH_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setDataSwitchCnf->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetDataSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDataSwitchCnf *getDataSwitchCnf = VOS_NULL_PTR;
    getDataSwitchCnf                          = (TAF_PS_GetDataSwitchCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DATASWITCH_QRY) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 上报查询结果 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            getDataSwitchCnf->dataSwitch);

    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}

VOS_UINT32 AT_RcvTafPsEvtSetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SET_DATA_ROAM_SWITCH_CNF_STRU *setDataRoamSwitchCnf = VOS_NULL_PTR;
    setDataRoamSwitchCnf                                       = (TAF_PS_SET_DATA_ROAM_SWITCH_CNF_STRU *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DATAROAMSWITCH_SET) {
        return VOS_ERR;
    }

    /* 处理错误码 */
    AT_PrcoPsEvtErrCode(indexNum, setDataRoamSwitchCnf->cause);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetDataRoamSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetDataRoamSwitchCnf *getDataSwitchCnf = VOS_NULL_PTR;
    getDataSwitchCnf                              = (TAF_PS_GetDataRoamSwitchCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DATAROAMSWITCH_QRY) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 上报查询结果 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            getDataSwitchCnf->dataRoamSwitch);

    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetRoamPdpTypeCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetRoamingPdpTypeCnf *setPdpTypeCnf = VOS_NULL_PTR;

    setPdpTypeCnf = (TAF_PS_SetRoamingPdpTypeCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ROAMPDPTYPE_SET) {
        AT_WARN_LOG("AT_RcvTafPsEvtSetRoamPdpTypeCnf: WARNING: CmdCurrentOpt != AT_CMD_ROAMPDPTYPE_SET!");
        return VOS_ERR;
    }

    /* 处理错误码 上报结果 */
    AT_PrcoPsEvtErrCode(indexNum, setPdpTypeCnf->cause);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtGetSinglePdnSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetSinglePdnSwitchCnf *getSinglePdnSwitchCnf = VOS_NULL_PTR;

    getSinglePdnSwitchCnf = (TAF_PS_GetSinglePdnSwitchCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SINGLEPDNSWITCH_QRY) {
        AT_WARN_LOG("AT_RcvTafPsEvtGetSinglePdnSwitchCnf: WARNING: CmdCurrentOpt != AT_CMD_SINGLEPDNSWITCH_QRY!");
        return VOS_ERR;
    }

    /* 上报查询结果 */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            getSinglePdnSwitchCnf->enable);

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtSetSinglePdnSwitchCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetSinglePdnSwitchCnf *setSinglePdnSwitchCnf = VOS_NULL_PTR;

    setSinglePdnSwitchCnf = (TAF_PS_SetSinglePdnSwitchCnf *)evtInfo;

    /* 检查当前AT操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SINGLEPDNSWITCH_SET) {
        AT_WARN_LOG("AT_RcvTafPsEvtSetSinglePdnSwitchCnf: WARNING: CmdCurrentOpt != AT_CMD_SINGLEPDNSWITCH_SET!");
        return VOS_ERR;
    }

    /* 处理错误码 上报结果 */
    AT_PrcoPsEvtErrCode(indexNum, setSinglePdnSwitchCnf->cause);
    return VOS_OK;
}
