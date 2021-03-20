/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
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

#include "vos.h"
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#include "taf_msg_chk.h"
#endif
#include "taf_type_def.h"
#include "taf_ps_api.h"
#include "at_mta_interface.h"
#include "mta_comm_interface.h"
#include "TafAgentInterface.h"
#include "taf_iface_api.h"
#include "at_mn_interface.h"
#include "mn_msg_api.h"
#include "TafSsaApi.h"
#include "taf_ccm_api.h"
#include "app_vc_api.h"
#include "taf_drv_agent.h"
#include "nas_om_interface.h"
#include "taf_ps_api.h"
#include "app_vc_api.h"
#include "taf_msg_chk_api.h"
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#include "vc_codec_interface.h"
#include "taf_msg_chk_tbl.h"
#include "nas_mta_interface.h"
#include "mta_phy_interface.h"
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#include "nrmm_mta_slice_interface.h"
#include "mta_rrc_interface.h"
#include "mta_l2_interface.h"
#include "aps_nrsm_interface.h"
#include "DsmPcfInterface.h"
#endif
#endif


#define THIS_FILE_ID PS_FILE_ID_TAF_MSG_CHK_C

#if (FEATURE_ON == FEATURE_PHONE_SC)
STATIC VOS_UINT32 DRVAGENT_ChkDrvAgentSimLockWriteExSetMsgLen(const MSG_Header *msgHeader);
#endif
STATIC VOS_UINT32 TAF_MTA_ChkTafAgentMtaWriteAcoreNvReqMsgLen(const MSG_Header *msgHeader);
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
STATIC VOS_UINT32 TAF_DSM_ChkTafPs5GUePolicyInfoRspMsgLen(const MSG_Header *msg);
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
STATIC VOS_UINT32 TAF_MtaCheckNrrcCapInfoNtfMsgLen(const MSG_Header *msg);
#endif
#endif
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_IMS == FEATURE_ON)
STATIC VOS_UINT32 APP_VC_ChkVoiceImsReportIndMsgLen(const MSG_Header *msg);
#endif
#endif
STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_GetChkAtMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkTafMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_GetChkTafMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_CCM_GetChkTafMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_CCM_GetChkTafMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* APP_VC_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 APP_VC_GetChkAtMsgLenTblSize(VOS_VOID);
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
STATIC VOS_UINT32 APP_VC_GetChkVoiceMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* APP_VC_GetChkVoiceMsgLenTblAddr(VOS_VOID);
#endif
STATIC TAF_ChkMsgLenNameMapTbl* DRVAGENT_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 DRVAGENT_GetChkAtMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkAtMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkAtMsgLenTblSize(VOS_VOID);
#if (FEATURE_LTEV == FEATURE_ON)
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkTafMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkTafMsgLenTblSize(VOS_VOID);
#endif
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkTafAgentMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkTafAgentMsgLenTblSize(VOS_VOID);
STATIC VOS_UINT32 TAF_ChkMtaLrrcUeCapParamSetReqMsgLen(const MSG_Header *msg);
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
STATIC VOS_UINT32 TAF_MTA_ChkMtaNrrcUeCapParamSetReqMsgLen(const MSG_Header *msg);
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrmmMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkNrmmMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrsmMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkNrsmMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrrcMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkNrrcMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNgagentMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkNgagentMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNmacMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetChkNmacMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetNrdcpMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_MTA_GetNrdcpMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkPcfMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_DSM_GetChkPcfMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkNrsmMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_GetChkNrsmMsgLenTblSize(VOS_VOID);
STATIC VOS_UINT32 TAF_DSM_ChkPcfUePolicyIndMsgLen(const MSG_Header *msgHeader);
STATIC VOS_UINT32 TAF_APS_ChkNrsmLteHandoverToNrIndMsgLen(const MSG_Header *msgHeader);
#endif
#endif
STATIC TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkTafMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_DSM_GetChkTafMsgLenTblSize(VOS_VOID);
STATIC TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkTafAgentMsgLenTblAddr(VOS_VOID);
STATIC VOS_UINT32 TAF_DSM_GetChkTafAgentMsgLenTblSize(VOS_VOID);

/* UEPS_PID_MTA����WUEPS_PID_TAF����ϢMTA_COMM_MsgType */
#if (FEATURE_LTEV == FEATURE_ON)
static TAF_ChkMsgLenNameMapTbl g_mtaChkTafMsgLenTbl[] = {
    { ID_TAF_MTA_VMODE_SET_REQ, sizeof(TAF_MTA_VMODE_SetReq), VOS_NULL_PTR },
    { ID_TAF_MTA_VMODE_QRY_REQ, sizeof(TAF_MTA_VMODE_QryReq), VOS_NULL_PTR },
};
#endif

/* UEPS_PID_MTA����ACPU_PID_TAFAGENT����ϢTAFAGENT_MsgId */
static TAF_ChkMsgLenNameMapTbl g_mtaChkTafAgentMsgLenTbl[] = {
    { ID_TAFAGENT_MTA_GET_SYSMODE_REQ, sizeof(TAFAGENT_MTA_GetSysmodeReq), VOS_NULL_PTR },
    { ID_TAFAGENT_MTA_GET_ANT_STATE_REQ, sizeof(TAFAGENT_MTA_GetAntStateReq), VOS_NULL_PTR },
    { ID_TAFAGENT_MTA_WRITE_ACORE_NV_REQ, 0, TAF_MTA_ChkTafAgentMtaWriteAcoreNvReqMsgLen },
    { ID_TAFAGENT_MTA_PROC_ACORE_NV_REQ, sizeof(TAFAGENT_MTA_ProcAcoreNvReq), VOS_NULL_PTR },
    { ID_TAFAGENT_MTA_READ_ACORE_ENCRYPTION_NV_REQ, sizeof(TAFAGENT_MTA_ReadAcoreEncryptionNvReq), VOS_NULL_PTR }
};

/* UEPS_PID_MTA����WUEPS_PID_AT����ϢAT_MTA_MsgType */
static TAF_ChkMsgLenNameMapTbl g_mtaChkAtMsgLenTbl[] = {
    { ID_AT_MTA_CPOS_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_CposReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_CGPSCLOCK_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_PHONE_SC)
    { ID_AT_MTA_SIMLOCKUNLOCK_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SimlockunlockReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_QRY_NMR_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_QryNmrReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_RESEL_OFFSET_CFG_SET_NTF,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ReselOffsetCfgSetNtf) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_AUTOTEST_QRY_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_WrrAutotestQryPara) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_CELLINFO_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_MEANRPT_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_FREQLOCK_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(MTA_AT_WrrFreqlockCtrl) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_WRR_RRC_VERSION_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_CELLSRH_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_FREQLOCK_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_RRC_VERSION_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_WRR_CELLSRH_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_GAS_AUTOTEST_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(GAS_AT_CmdPara) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_DEL_CELLENTITY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_BODY_SAR_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_BodySarSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_CURC_SET_NOTIFY, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CurcSetNotify) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_CURC_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_UNSOLICITED_RPT_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_UnsolicitedRptSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_UnsolicitedRptQryReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_IMEI_VERIFY_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_CGSN_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_NCELL_MONITOR_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NcellMonitorSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NCELL_MONITOR_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_REFCLKFREQ_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_RefclkfreqSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_REFCLKFREQ_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_RFICSSIRD_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_RficssirdReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_HANDLEDECT_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_HANDLEDECT_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_DSDS)
    { ID_AT_MTA_RRC_PROTECT_PS_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_RrcProtectPsReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { ID_AT_MTA_PHY_INIT_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_PhyInitReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_ECID_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_EcidSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_MIPICLK_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_DPDTTEST_FLAG_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetDpdttestFlagReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_SET_DPDT_VALUE_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetDpdtValueReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_QRY_DPDT_VALUE_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_QryDpdtValueReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_SET_TRX_TAS_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetTrxTasReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_QRY_TRX_TAS_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_QryTrxTasReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_5G_NSSAI_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_5GNssaiSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_5G_NSSAI_QRY_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_5GNssaiQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SET_LADN_RPT_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_SetLadnRptReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_QRY_LADN_INFO_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NW_SLICE_AUTH_CMPL_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_5GNwSliceAuthCmplReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_SET_JAM_DETECT_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetJamDetectReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_QRY_JAM_DETECT_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_FREQ_LOCK_REQ, (sizeof(AT_MTA_Msg) + sizeof(TAF_NVIM_FreqLockCfg) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SET_GSM_FREQLOCK_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetGsmFreqlockReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_QRY_GSM_FREQLOCK_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_PROBE_FREQLOCK)
    { ID_AT_MTA_SET_M2M_FREQLOCK_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetM2MFreqlockReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_QRY_M2M_FREQLOCK_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_SET_FEMCTRL_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetFemctrlReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_LTE)
#if (FEATURE_ON == FEATURE_LTE_MBMS)
    { ID_AT_MTA_MBMS_SERVICE_OPTION_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MbmsServiceOptionSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_MBMS_SERVICE_STATE_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MbmsServiceStateSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_MBMS_PREFERENCE_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MbmsPreferenceSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_SIB16_NETWORK_TIME_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_BSSI_SIGNAL_LEVEL_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_NETWORK_INFO_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_EMBMS_STATUS_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_MBMS_UNSOLICITED_CFG_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MbmsUnsolicitedCfgSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_INTEREST_LIST_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MbmsInterestlistSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_MBMS_AVL_SERVICE_LIST_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_LTE_LOW_POWER_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_LowPowerConsumptionSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_LTE_WIFI_COEX_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_LteWifiCoexSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_LTE_WIFI_COEX_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_LTE_CA_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CaCfgSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_LTE_CA_CELLEX_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_LTE_CA_CELL_RPT_CFG_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CaCellSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_LTE_CA_CELL_RPT_CFG_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_FINE_TIME_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_FineTimeSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_OVERHEATING_CFG_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_OverHeatingSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_OVERHEATING_CFG_QRY_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(AT_MTA_OverHeatingQryReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_LRRC_UE_CAP_PARAM_SET_REQ, 0, TAF_ChkMtaLrrcUeCapParamSetReqMsgLen },
    { ID_AT_MTA_LL2_COM_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_L2ComCfgSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_LL2_COM_CFG_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_L2ComCfgQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_LTE_CONN_RECOVERY_NTF, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NL2_COM_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_L2ComCfgSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NL2_COM_CFG_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_L2ComCfgQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NPDCP_SLEEP_THRES_CFG_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NPdcpSleepThresCfgReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NRRC_UE_CAP_PARAM_SET_REQ, 0, TAF_MTA_ChkMtaNrrcUeCapParamSetReqMsgLen },
#endif
    { ID_AT_MTA_SET_FR_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetFrReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    { ID_AT_MTA_MEID_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MeidSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_MEID_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_EVDO_SYS_EVENT_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_EvdoSysEventSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_EVDO_SIG_MASK_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_EvdoSigMaskSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NO_CARD_MODE_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NoCardModeSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NO_CARD_MODE_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_TRANSMODE_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_UE_CENTER_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetUeCenterReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_UE_CENTER_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_NETMON_SCELL_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_NETMON_NCELL_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_QRY_AFC_CLK_FREQ_XOCOEF_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_ANQUERY_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_CSNR_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_CSQLVL_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_XCPOSR_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetXcposrReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_XCPOSR_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_XCPOSRRPT_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetXcposrrptReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_XCPOSRRPT_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_CLEAR_HISTORY_FREQ_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ClearHistoryFreqReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_SET_SENSOR_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetSensorReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SET_SCREEN_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetScreenReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_FRAT_IGNITION_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_FRAT_IGNITION_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_MODEM_TIME_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ModemTime) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_GNSS_NTY, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_Gnssnty) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_RX_TEST_MODE_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetRxtestmodeReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_BESTFREQ_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_BestfreqSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_BESTFREQ_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_RFIC_DIE_ID_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_RFFE_DIE_ID_QRY_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_MIPI_WREX_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MipiWrexReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_MIPI_RDEX_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MipiRdexReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_PHY_COM_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_PhyComCfgSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_AT_MTA_NRPHY_COM_CFG_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrphyComCfgSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_CRRCONN_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetCrrconnReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_CRRCONN_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_VTRLQUALRPT_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetVtrlqualrptReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_PMU_DIE_SN_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_MULTI_PMU_DIE_ID_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MultiPmuDieIdSet) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_MODEM_CAP_UPDATE_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ModemCapUpdateReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_TAS_CFG_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_TasTestCfg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_TAS_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_TasTestQry) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_RS_INFO_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_RsInfoQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_ERRCCAP_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ErrccapCfgReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_ERRCCAP_QRY_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ErrccapQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_PSEUCELL_INFO_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_PseucellInfoSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_POWER_DET_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_MIPIREAD_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_MipiReadReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_PHYMIPIWRITE_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_PhyMipiWriteReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_DCXO_HI1102_SAMPLE_SHARE)
    { ID_AT_MTA_DCXO_SAMPLE_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_DcxoSampeQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { ID_AT_MTA_CHRALARMRLAT_CFG_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_ChrAlarmRlatCfgSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_ECC_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetEccCfgReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_EPDU_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_EpduSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },

    { ID_AT_MTA_COMM_BOOSTER_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CommBoosterSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_COMM_BOOSTER_QRY_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CommBoosterQueryReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NVLOAD_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NvloadSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SMS_DOMAIN_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SmsDomainSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SMS_DOMAIN_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_SAMPLE_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetSampleReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_GPS_LOCSET_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_GpslocsetSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_CCLK_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_GAME_MODE_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CommGameModeSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    { ID_AT_MTA_CDMA_MODEM_CAP_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CdmaModemCapSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_CDMA_CAP_RESUME_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_CdmaCapResumeSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_NV_REFRESH_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NvRefreshSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_PSEUDBTS_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetPseudbtsReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SUBCLFSPARAM_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetSubclfsparamReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_SUBCLFSPARAM_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_FORCESYNC_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetForcesyncReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_LENDC_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_FREQLOCK_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetNrfreqlockReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NR_FREQLOCK_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_NRRCCAP_CFG_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrrccapCfgSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NRRCCAP_QRY_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrrccapQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_NRPOWERSAVING_CFG_SET_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrPowerSavingCfgSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NRPOWERSAVING_QRY_REQ,
      (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrPowerSavingQryReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { ID_AT_MTA_NRPWRCTRL_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_NrpwrctrlSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { ID_AT_MTA_SET_LTEPWRDISS_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_LtepwrdissSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_SFEATURE_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_AT_MTA_LOW_PWR_MODE_SET_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_LowPwrModeReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { ID_AT_MTA_TXPOWER_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_NTXPOWER_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#endif
    { ID_AT_MTA_QRY_TDD_SUBFRAME_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_MCS_SET_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_HFREQINFO_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SARREDUCTION_NTF, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { ID_AT_MTA_5G_OPTION_CFG_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_Set5GOptionReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_5G_OPTION_QRY_REQ, sizeof(AT_MTA_Msg), VOS_NULL_PTR },
    { ID_AT_MTA_SET_NETMON_SSCELL_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_SSB_ID_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_CA_CELL_INFO_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_CA_CELL_INFO_RPT_CFG_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_CA_CELL_INFO_RPT_CFG_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { ID_AT_MTA_NR_NW_CAP_INFO_RPT_CFG_SET_REQ,
      (sizeof(MN_APP_ReqMsg) - MN_APP_REQ_DEFAULT_CONTENT_LEN + sizeof(AT_MTA_NrNwCapInfoRptCfgSetReq)), VOS_NULL_PTR },
    { ID_AT_MTA_NR_NW_CAP_INFO_RPT_CFG_QRY_REQ,
      (sizeof(MN_APP_ReqMsg) - MN_APP_REQ_DEFAULT_CONTENT_LEN + sizeof(AT_MTA_NrNwCapInfoRptCfgQryReq)), VOS_NULL_PTR },
    { ID_AT_MTA_NR_NW_CAP_INFO_QRY_REQ,
      (sizeof(MN_APP_ReqMsg) - MN_APP_REQ_DEFAULT_CONTENT_LEN + sizeof(AT_MTA_NrNwCapInfoQryReq)), VOS_NULL_PTR },
#endif
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_AT_MTA_SET_LTESARSTUB_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_LteSarStubSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_AT_MTA_UART_TEST_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
#endif
#endif
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { ID_AT_MTA_FILE_WRITE_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_FileWrReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { ID_AT_MTA_FILE_READ_REQ, (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_FileRdReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { ID_AT_MTA_QUICK_CARD_SWITCH_REQ,
     (sizeof(AT_MTA_Msg) + sizeof(AT_MTA_SetQuickCardSwitchReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
};

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
/* UEPS_PID_MTA����UEPS_PID_NRMM����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNrmmMsgLenTbl[] = {
    { ID_NRNAS_MTA_GET_NRMM_CHR_STATS_INFO_CNF, sizeof(NRNAS_MTA_GetNrmmChrStatsInfoCnf), VOS_NULL_PTR },
    { ID_NRMM_MTA_SET_5G_NSSAI_CNF, sizeof(NRMM_MTA_Set5GNssaiCnf), VOS_NULL_PTR },
    { ID_NRMM_MTA_QRY_5G_NSSAI_CNF, sizeof(NRMM_MTA_Qry5GNssaiCnf), VOS_NULL_PTR },
    { ID_NRMM_MTA_LADN_INFO_IND, sizeof(NRMM_MTA_LadnInfoInd), VOS_NULL_PTR },
    { ID_NRMM_MTA_QRY_LADN_INFO_CNF, sizeof(NRMM_MTA_QryLadnInfoCnf), VOS_NULL_PTR },
    { ID_NRMM_MTA_QRY_NETWORK_PARA_CNF, sizeof(NRMM_MTA_QryNetworkParaCnf), VOS_NULL_PTR },
    { ID_NRMM_MTA_PENDING_NSSAI_IND, sizeof(NRMM_MTA_PendingNssaiInd), VOS_NULL_PTR },
    { ID_NRMM_MTA_NW_SLICE_AUTH_CMD_IND, sizeof(NRMM_MTA_NwSliceAuthInd), VOS_NULL_PTR },
    { ID_NRMM_MTA_NW_SLICE_AUTH_RSLT_IND, sizeof(NRMM_MTA_NwSliceAuthInd), VOS_NULL_PTR }
};

/* UEPS_PID_MTA����UEPS_PID_NRSM����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNrsmMsgLenTbl[] = {
    { ID_NRNAS_MTA_GET_NRSM_CHR_STATS_INFO_CNF, sizeof(NRNAS_MTA_GetNrsmChrStatsInfoCnf), VOS_NULL_PTR },
    { ID_NRSM_MTA_QRY_NETWORK_PARA_CNF, sizeof(NRSM_MTA_QryNetworkParaCnf), VOS_NULL_PTR }
};

/* UEPS_PID_MTA����UEPS_PID_NRRC����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNrrcMsgLenTbl[] = {
#if (FEATURE_ON == FEATURE_PHONE_ENG_AT_CMD)
    { ID_NRRC_MTA_FREQLOCK_SET_CNF, sizeof(NRRC_MTA_FreqlockSetCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_FREQLOCK_QRY_CNF, sizeof(NRRC_MTA_FreqlockQryCnf), VOS_NULL_PTR },
#endif
    { ID_NRRC_MTA_SUPPORT_BAND_QUERY_CNF, sizeof(NRRC_MTA_SupportBandQueryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CAP_CFG_CNF, sizeof(NRRC_MTA_CapCfgCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CAP_QRY_CNF, sizeof(NRRC_MTA_CapQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_SIB_FINE_TIME_CNF, sizeof(NRRC_MTA_SibFineTimeCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_QRY_DPDT_VALUE_CNF, sizeof(NRRC_MTA_QryDpdtValueCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_QRY_TRX_TAS_CNF, sizeof(NRRC_MTA_QryTrxTasCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CELLINFO_QRY_CNF, sizeof(NRRC_MTA_CellinfoQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_NETMON_CELL_QRY_CNF, sizeof(NRRC_MTA_NetmonCellQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_OVERHEATING_CFG_CNF, sizeof(NRRC_MTA_OverheatingCfgCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_OVERHEATING_QRY_CNF, sizeof(NRRC_MTA_OverheatingQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_UE_CAP_PARAM_SET_CNF, sizeof(NRRC_MTA_UeCapParamSetCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_UE_CAP_INFO_NTF, 0, TAF_MtaCheckNrrcCapInfoNtfMsgLen },
    { ID_NRRC_MTA_SSB_ID_QRY_CNF, sizeof(NRRC_MTA_SsbIdQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CIPHER_REPORT_IND, sizeof(NRRC_MTA_CipherReportInd), VOS_NULL_PTR },
    { ID_NRRC_MTA_CA_CELL_INFO_QRY_CNF, sizeof(NRRC_MTA_CaCellInfoQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CA_CELL_INFO_RPT_CFG_SET_CNF, sizeof(NRRC_MTA_CaCellInfoRptCfgSetCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CA_CELL_INFO_RPT_CFG_QRY_CNF, sizeof(NRRC_MTA_CaCellInfoRptCfgQryCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_CA_CELL_INFO_IND, sizeof(NRRC_MTA_CaCellInfoInd), VOS_NULL_PTR },
    { ID_NRRC_MTA_BOOSTER_DATA_SET_CNF, sizeof(NRRC_MTA_BoosterDataSetCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_POWER_SAVING_CFG_CNF, sizeof(NRRC_MTA_PowerSavingCfgCnf), VOS_NULL_PTR },
    { ID_NRRC_MTA_POWER_SAVING_QRY_CNF, sizeof(NRRC_MTA_PowerSavingQryCnf), VOS_NULL_PTR },
};

/* UEPS_PID_MTA����NRAGENT_PID_AGENT����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNgagentMsgLenTbl[] = {
    { ID_RTTAGENT_MTA_PWR_CTRL_SET_CNF, sizeof(RTTAGENT_MTA_PwrCtrlSetCnf), VOS_NULL_PTR },
    { ID_RTTAGENT_MTA_LOW_PWR_MODE_SET_CNF, sizeof(RTTAGENT_MTA_LowPwrModeSetCnf), VOS_NULL_PTR },
    { ID_NPHY_MTA_QRY_MCS_CNF, sizeof(NPHY_MTA_McsQryCnf), VOS_NULL_PTR },
    { ID_NPHY_MTA_QRY_NTXPOWER_CNF, sizeof(NPHY_MTA_NtxpowerQryCnf), VOS_NULL_PTR },
    { ID_NPHY_MTA_QRY_HFREQINFO_CNF, sizeof(NPHY_MTA_HfreqinfoQryCnf), VOS_NULL_PTR },
    { ID_NPHY_MTA_HFREQINFO_IND, sizeof(NPHY_MTA_HfreqinfoInd), VOS_NULL_PTR },
    { ID_NPHY_MTA_COM_CFG_SET_CNF, sizeof(NPHY_MTA_CommCfgCnf), VOS_NULL_PTR }
};

/* UEPS_PID_MTA����UEPS_PID_L2MA����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNmacMsgLenTbl[] = {
    { ID_L2MA_MTA_CMD_CFG_SET_CNF, sizeof(L2MA_MTA_CmdCfgSetCnf), VOS_NULL_PTR },
    { ID_L2MA_MTA_CMD_CFG_QRY_CNF, sizeof(L2MA_MTA_CmdCfgQryCnf), VOS_NULL_PTR },
};

/* UEPS_PID_MTA����I0_UEPS_PID_NPDCP_DL_NRT����Ϣ���ȼ�� */
static TAF_ChkMsgLenNameMapTbl g_mtaChkNrdcpMsgLenTbl[] = {
    { ID_NPDCP_MTA_SLEEP_THRES_CFG_CNF, sizeof(NPDCP_MTA_NrtSleepThresCfgCnf), VOS_NULL_PTR },
};

/* UEPS_PID_DSM����UEPS_PID_PCF����Ϣ */
static TAF_ChkMsgLenNameMapTbl g_dsmChkPcfMsgLenTbl[] = {
    { ID_PCF_DSM_URSP_UPDATE_IND, sizeof(PCF_DSM_UrspUpdateInd), VOS_NULL_PTR },
    { ID_PCF_DSM_LADN_UPDATE_IND, sizeof(PCF_DSM_LadnUpdateInd), VOS_NULL_PTR },
    { ID_PCF_DSM_NSSAI_UPDATE_IND, sizeof(PCF_DSM_NssaiUpdateInd), VOS_NULL_PTR },
    { ID_PCF_DSM_UE_POLICY_IND, 0, TAF_DSM_ChkPcfUePolicyIndMsgLen },
    { ID_PCF_DSM_UE_POLICY_RSP_CHECK_RSLT_IND, sizeof(PCF_DSM_UePolicyRspCheckRsltInd), VOS_NULL_PTR },
};

/* WUEPS_PID_TAF����UEPS_PID_NRSM����Ϣ */
static TAF_ChkMsgLenNameMapTbl g_tafChkNrsmMsgLenTbl[] = {
    { ID_NRSM_APS_DEVICE_TRIGGER_IND, sizeof(NRSM_APS_DeviceTriggerInd), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_EST_CNF, sizeof(NRSM_APS_PduSessionEstCnf), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_MODIFY_CNF, sizeof(NRSM_APS_PduSessionModifyCnf), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_MODIFY_IND, sizeof(NRSM_APS_PduSessionModifyInd), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_RELEASE_CNF, sizeof(NRSM_APS_PduSessionReleaseCnf), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_RELEASE_IND, sizeof(NRSM_APS_PduSessionReleaseInd), VOS_NULL_PTR },
    { ID_NRSM_APS_LTE_HANDOVER_TO_NR_IND, 0, TAF_APS_ChkNrsmLteHandoverToNrIndMsgLen },
    { ID_NRSM_APS_PDU_SESSION_SSM3_REEST_IND, sizeof(NRSM_APS_PduSessionSsm3ReestInd), VOS_NULL_PTR },
    { ID_NRSM_APS_PDU_SESSION_SSM2_INFO_UPDATE_IND, sizeof(NRSM_APS_PduSessionSsm2InfoUpdateInd), VOS_NULL_PTR },
    { ID_NRSM_APS_GET_SDF_PARA_IND, sizeof(NRSM_APS_GetSdfParaInd), VOS_NULL_PTR },
    { ID_NRSM_APS_CLEAR_ALL_PDU_SESSION_IND, sizeof(NRSM_APS_ClearAllIndPduSession), VOS_NULL_PTR },
    { ID_NRSM_APS_NSSAI_UPDATE_IND, sizeof(NRSM_APS_NssaiUpdateInd), VOS_NULL_PTR },
};
#endif
#endif

/*
 * UEPS_PID_DSM����WUEPS_PID_TAF����ϢTAF_IFACE_MsgId
 * UEPS_PID_DSM����WUEPS_PID_TAF����ϢTAF_PS_MsgId
 */
static TAF_ChkMsgLenNameMapTbl g_dsmChkTafMsgLenTbl[] = {
    { ID_MSG_TAF_IFACE_UP_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_IFACE_UpReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_IFACE_DOWN_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_IFACE_DownReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_GET_IFACE_DYNAMIC_PARA_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_IFACE_GetDynamicParaReq)),
      VOS_NULL_PTR },

    /* +CGACT */
    { ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetPdpStateReq)),
      VOS_NULL_PTR },

    /* +CGCMOD */
    { ID_MSG_TAF_PS_CALL_MODIFY_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_CallModifyReq)), VOS_NULL_PTR },

    /* +CGANS */
    { ID_MSG_TAF_PS_CALL_ANSWER_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_CallAnswerReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_CALL_HANGUP_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_CallHangupReq)), VOS_NULL_PTR },

    /* ^NDISCONN/^NDISDUP */
    { ID_MSG_TAF_PS_CALL_ORIG_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_CallOrigReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_CALL_END_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_CallEndReq)), VOS_NULL_PTR },

    { ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_PppDialOrigReq)), VOS_NULL_PTR },

    /* ^IMSCTRLMSG */
    { ID_MSG_TAF_PS_EPDG_CTRL_NTF, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_EpdgCtrlNtf)), VOS_NULL_PTR },

    /* +CGDCONT */
    { ID_MSG_TAF_PS_SET_PRIM_PDP_CONTEXT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetPrimPdpContextInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_PRIM_PDP_CONTEXT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetPrimPdpContextInfoReq)),
      VOS_NULL_PTR },

    /* +CGDSCONT */
    { ID_MSG_TAF_PS_SET_SEC_PDP_CONTEXT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetSecPdpContextInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_SEC_PDP_CONTEXT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetSecPdpContextInfoReq)),
      VOS_NULL_PTR },

    /* +CGTFT */
    { ID_MSG_TAF_PS_SET_TFT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetTftInfoReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_TFT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetTftInfoReq)), VOS_NULL_PTR },

    /* +CGEQREQ */
    { ID_MSG_TAF_PS_SET_UMTS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetUmtsQosInfoReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_UMTS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetUmtsQosInfoReq)), VOS_NULL_PTR },

    /* +CGEQMIN */
    { ID_MSG_TAF_PS_SET_UMTS_QOS_MIN_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetUmtsQosMinInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_UMTS_QOS_MIN_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetUmtsQosMinInfoReq)),
      VOS_NULL_PTR },

    /* +CGEQNEG */
    { ID_MSG_TAF_PS_GET_DYNAMIC_UMTS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamicUmtsQosInfoReq)),
      VOS_NULL_PTR },

    /* +CGACT */
    { ID_MSG_TAF_PS_GET_PDP_CONTEXT_STATE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetPdpStateReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_PDP_IP_ADDR_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetPdpIpAddrInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_PDPCONT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetPdpContextInfoReq)),
      VOS_NULL_PTR },

    /* +CGAUTO */
    { ID_MSG_TAF_PS_SET_ANSWER_MODE_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetAnswerModeInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_ANSWER_MODE_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetAnswerModeInfoReq)),
      VOS_NULL_PTR },

    /* +CGCONTRDP */
    { ID_MSG_TAF_PS_GET_DYNAMIC_PRIM_PDP_CONTEXT_INFO_REQ,
      (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamicPrimPdpContextInfoReq)), VOS_NULL_PTR },

    /* +CGSCONTRDP */
    { ID_MSG_TAF_PS_GET_DYNAMIC_SEC_PDP_CONTEXT_INFO_REQ,
      (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamicPrimPdpContextInfoReq)), VOS_NULL_PTR },

    /* +CGTFTRDP */
    { ID_MSG_TAF_PS_GET_DYNAMIC_TFT_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamicTftInfoReq)),
      VOS_NULL_PTR },

    /* +CGEQOS */
    { ID_MSG_TAF_PS_SET_EPS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetEpsQosInfoReq)), VOS_NULL_PTR },

    /* +CGTFTRDP */
    { ID_MSG_TAF_PS_GET_EPS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetEpsQosInfoReq)), VOS_NULL_PTR },

    /* +CGEQOSRDP */
    { ID_MSG_TAF_PS_GET_DYNAMIC_EPS_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamicEpsQosInfoReq)),
      VOS_NULL_PTR },

    /* D */
    { ID_MSG_TAF_PS_GET_D_GPRS_ACTIVE_TYPE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDGprsActiveTypeReq)),
      VOS_NULL_PTR },

    /* ^DWINS */
    { ID_MSG_TAF_PS_CONFIG_NBNS_FUNCTION_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_ConfigNbnsFunctionReq)),
      VOS_NULL_PTR },

    /* ^AUTHDATA */
    { ID_MSG_TAF_PS_SET_AUTHDATA_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetAuthdataInfoReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_AUTHDATA_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetAuthdataInfoReq) - 0),
      VOS_NULL_PTR },

    /* ^DNSQUERY */
    { ID_MSG_TAF_PS_GET_NEGOTIATION_DNS_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetNegotiationDnsReq)),
      VOS_NULL_PTR },

    /* ^CGDNS */
    { ID_MSG_TAF_PS_SET_PDP_DNS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetPdpDnsInfoReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_PDP_DNS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetPdpDnsInfoReq)), VOS_NULL_PTR },

    /* +CTA*/
    { ID_MSG_TAF_PS_SET_CTA_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetCtaInfoReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_CTA_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetCtaInfoReq)), VOS_NULL_PTR },

    /* ^IMSPDPCFG */
    { ID_MSG_TAF_PS_SET_IMS_PDP_CFG_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetImsPdpCfgReq)), VOS_NULL_PTR },

    /* ^CDORMTIMER */
    { ID_MSG_TAF_PS_SET_1X_DORM_TIMER_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_Set1XDormTimerReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_1X_DORM_TIEMR_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_Get1XDormTimerReq)),
      VOS_NULL_PTR },


    { ID_MSG_TAF_PS_GET_LTE_ATTACH_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetLteAttachInfoReq)),
      VOS_NULL_PTR },

    { ID_MSG_TAF_PS_GET_SINGLE_PDN_SWITCH_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetSinglePdnSwitchReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_SET_SINGLE_PDN_SWITCH_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetSinglePdnSwitchReq)),
      VOS_NULL_PTR },

    { ID_MSG_TAF_PS_SET_5G_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_Set5GQosInfoReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_5G_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_Get5gQosInfoReq)), VOS_NULL_PTR },

    /* +C5GQOSRDP */
    { ID_MSG_TAF_PS_GET_DYNAMIC_5G_QOS_INFO_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetDynamic5GQosInfoReq)),
      VOS_NULL_PTR },

    /* +CGTFTRDP */
    { ID_MSG_TAF_PS_SET_ROAMING_PDP_TYPE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetRoamingPdpTypeReq)),
      VOS_NULL_PTR },

    { ID_MSG_TAF_PS_SET_CUST_ATTACH_PROFILE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetCustAttachProfileInfoReq)),
        VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_CUST_ATTACH_PROFILE_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetCustAttachProfileInfoReq)),
        VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_CUST_ATTACH_PROFILE_COUNT_REQ, (sizeof(MSG_HEADER_STRU) +
        sizeof(TAF_PS_GetCustAttachProfileCountReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_SET_ATTACH_PROFILE_SWITCH_STATUS_REQ, (sizeof(MSG_HEADER_STRU) +
        sizeof(TAF_PS_SetAttachProfileSwitchStatusReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_ATTACH_PROFILE_SWITCH_STATUS_REQ, (sizeof(MSG_HEADER_STRU) +
        sizeof(TAF_PS_GetAttachProfileSwitchStatusReq)), VOS_NULL_PTR },

    /* ^CPOLICYRPT */
    { ID_MSG_TAF_PS_SET_UE_POLICY_RPT_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_SetUePolicyRptReq)),
      VOS_NULL_PTR },

    /* ^CPOLICYCODE */
    { ID_MSG_TAF_PS_GET_UE_POLICY_REQ, (sizeof(MSG_HEADER_STRU) + sizeof(TAF_PS_GetUePolicyReq)), VOS_NULL_PTR },

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    /* +CSUEPOLICY */
    { ID_MSG_TAF_PS_5G_UE_POLICY_INFO_RSP, 0, TAF_DSM_ChkTafPs5GUePolicyInfoRspMsgLen }
#endif

};

/* UEPS_PID_DSM����ACPU_PID_TAFAGENT����ϢTAFAGENT_MsgId */
static TAF_ChkMsgLenNameMapTbl g_dsmChkTafAgentMsgLenTbl[] = {
    { ID_TAFAGENT_PS_GET_CID_PARA_REQ, sizeof(TAFAGENT_PS_GetCidParaReq), VOS_NULL_PTR },
};

/* WUEPS_PID_TAF����WUEPS_PID_AT����ϢAT_MN_Msgtype */
static TAF_ChkMsgLenNameMapTbl g_tafChkAtMsgLenTbl[] = {
    { MN_MSG_MSGTYPE_SEND_RPDATA_DIRECT, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SendParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_SEND_RPDATA_FROMMEM,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SendFrommemParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_SEND_RPRPT, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SendAckParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_WRITE, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_WriteParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_READ, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ReadParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_LIST, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ListParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_DELETE, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_DeleteParam) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_DELETE_TEST, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ListParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_WRITE_SRV_PARA,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_WriteSrvParameter) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_READ_SRV_PARA, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ReadCommParam) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_DELETE_SRV_PARA, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_DeleteParam) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_MODIFY_STATUS, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ModifyStatusParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_SET_RCVPATH, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SetRcvmsgPathParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_RCVPATH, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_GetRcvmsgPathParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_STORAGESTATUS,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_GetStorageStatusParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_READ_STARPT, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_ReadCommParam) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_DELETE_STARPT, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_DeleteParam) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_STARPT_FROMMOMSG,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_StarptMomsgParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_MOMSG_FROMSTARPT,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_StarptMomsgParm) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_SET_MEMSTATUS, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SetMemstatusParm) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_SET_RELAY_LINK_CTRL,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_LinkCtrlType) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_RELAY_LINK_CTRL, (sizeof(MN_APP_ReqMsg) - (sizeof(VOS_UINT8) * TAF_DEFAULT_CONTENT_LEN)),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_STUB, (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_StubMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))
    { MN_MSG_MSGTYPE_GET_ALLCBMIDS, sizeof(MN_APP_ReqMsg) - (sizeof(VOS_UINT8) * TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_ADD_CBMIDS, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_CBA_CbmiRangeList) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_DELETE_CBMIDS, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_CBA_CbmiRangeList) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_EMPTY_CBMIDS, (sizeof(MN_APP_ReqMsg) - (sizeof(VOS_UINT8) * TAF_DEFAULT_CONTENT_LEN)),
      VOS_NULL_PTR },
#endif
    { MN_MSG_MSGTYPE_SET_SMS_SEND_DOMAIN,
      (sizeof(MN_APP_ReqMsg) + sizeof(MN_MSG_SmsSendDomainType) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { MN_MSG_MSGTYPE_GET_SMS_SEND_DOMAIN, (sizeof(MN_APP_ReqMsg) - (sizeof(VOS_UINT8) * TAF_DEFAULT_CONTENT_LEN)),
      VOS_NULL_PTR },

    /* MN_APP_MSG_CLASS_SSA */
    { TAF_MSG_REGISTERSS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_RegisterssReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_ERASESS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_ErasessReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_ACTIVATESS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_ActivatessReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_DEACTIVATESS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_DeactivatessReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_INTERROGATESS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_InterrogatessReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_REGPWD_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_RegpwdReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { TAF_MSG_PROCESS_USS_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_ProcessUssReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_ERASECCENTRY_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(TAF_SS_EraseccEntryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { TAF_MSG_RLEASE_MSG, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { TAF_MSG_SET_USSDMODE_MSG, (sizeof(MN_APP_ReqMsg) + sizeof(USSD_TRANS_Mode) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#if (FEATURE_MBB_CUST == FEATURE_ON)
    { TAF_MSG_ABORT_SS_MSG, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
};

/* UEPS_PID_CCM����WUEPS_PID_TAF(ʵ����WUEPS_PID_AT)����ϢTAF_CCM_MsgType */
static TAF_ChkMsgLenNameMapTbl g_ccmChkTafMsgLenTbl[] = {
    { ID_TAF_CCM_QRY_CHANNEL_INFO_REQ, sizeof(TAF_CCM_QryChannelInfoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CALL_MODIFY_REQ, sizeof(TAF_CCM_CallModifyReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CALL_ANSWER_REMOTE_MODIFY_REQ, sizeof(TAF_CCM_CallAnswerRemoteModifyReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_ECONF_CALLED_INFO_REQ, sizeof(TAF_CCM_QryEconfCalledInfoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CALL_ORIG_REQ, sizeof(TAF_CCM_CallOrigReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CANCEL_ADD_VIDEO_REQ, sizeof(TAF_CCM_CancelAddVideoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CALL_END_REQ, sizeof(TAF_CCM_CallEndReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CALL_SUPS_CMD_REQ, sizeof(TAF_CCM_CallSupsCmdReq), VOS_NULL_PTR },
    { ID_TAF_CCM_ECONF_DIAL_REQ, sizeof(TAF_CCM_EconfDialReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CUSTOM_DIAL_REQ, sizeof(TAF_CCM_CustomDialReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_CALL_INFO_REQ, sizeof(TAF_CCM_QryCallInfoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_CLPR_REQ, sizeof(TAF_CCM_QryClprReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_XLEMA_REQ, sizeof(TAF_CCM_QryXlemaReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SET_CSSN_REQ, sizeof(TAF_CCM_SetCssnReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CUSTOM_ECC_NUM_REQ, sizeof(TAF_CCM_CustomEccNumReq), VOS_NULL_PTR },
    { ID_TAF_CCM_START_DTMF_REQ, sizeof(TAF_CCM_StartDtmfReq), VOS_NULL_PTR },
    { ID_TAF_CCM_STOP_DTMF_REQ, sizeof(TAF_CCM_StopDtmfReq), VOS_NULL_PTR },
    { ID_TAF_CCM_GET_CDUR_REQ, sizeof(TAF_CCM_GetCdurReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SET_UUSINFO_REQ, sizeof(TAF_CCM_SetUusinfoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_UUSINFO_REQ, sizeof(TAF_CCM_QryUusinfoReq), VOS_NULL_PTR },
    { ID_TAF_CCM_QRY_ALS_REQ, sizeof(TAF_CCM_QryAlsReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SET_ALS_REQ, sizeof(TAF_CCM_SetAlsReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CCWAI_SET_REQ, sizeof(TAF_CCM_CcwaiSetReq), VOS_NULL_PTR },
    { ID_TAF_CCM_CNAP_QRY_REQ, sizeof(TAF_CCM_CnapQryReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SEND_FLASH_REQ, sizeof(TAF_CCM_SendFlashReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SEND_BURST_DTMF_REQ, sizeof(TAF_CCM_SendBurstDtmfReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SEND_CONT_DTMF_REQ, sizeof(TAF_CCM_SendContDtmfReq), VOS_NULL_PTR },
    { ID_TAF_CCM_ENCRYPT_VOICE_REQ, sizeof(TAF_CCM_EncryptVoiceReq), VOS_NULL_PTR },
    { ID_TAF_CCM_REMOTE_CTRL_ANSWER_REQ, sizeof(TAF_CCM_RemoteCtrlAnswerReq), VOS_NULL_PTR },
    { ID_TAF_CCM_ECC_SRV_CAP_QRY_REQ, sizeof(TAF_CCM_EccSrvCapQryReq), VOS_NULL_PTR },
    { ID_TAF_CCM_ECC_SRV_CAP_CFG_REQ, sizeof(TAF_CCM_EccSrvCapCfgReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SET_EC_TEST_MODE_REQ, sizeof(TAF_CCM_SetEcTestModeReq), VOS_NULL_PTR },
    { ID_TAF_CCM_GET_EC_RANDOM_REQ, sizeof(TAF_CCM_GetEcRandomReq), VOS_NULL_PTR },
    { ID_TAF_CCM_GET_EC_TEST_MODE_REQ, sizeof(TAF_CCM_GetEcTestModeReq), VOS_NULL_PTR },
    { ID_TAF_CCM_GET_EC_KMC_REQ, sizeof(TAF_CCM_GetEcKmcReq), VOS_NULL_PTR },
    { ID_TAF_CCM_SET_EC_KMC_REQ, sizeof(TAF_CCM_SetEcKmcReq), VOS_NULL_PTR },
    { ID_TAF_CCM_PRIVACY_MODE_SET_REQ, sizeof(TAF_CCM_PrivacyModeSetReq), VOS_NULL_PTR },
    { ID_TAF_CCM_PRIVACY_MODE_QRY_REQ, sizeof(TAF_CCM_PrivacyModeQryReq), VOS_NULL_PTR },
#if (FEATURE_ECALL == FEATURE_ON)
    { ID_TAF_CCM_SET_ECALL_DOMAIN_MODE_REQ, sizeof(TAF_CCM_SetEcallDomainModeReq), VOS_NULL_PTR },
#endif
};

/* WUEPS_PID_VC����WUEPS_PID_AT����ϢAPP_VC_Msg */
static TAF_ChkMsgLenNameMapTbl g_vcChkAtMsgLenTbl[] = {
    { APP_VC_MSG_REQ_SET_VOLUME, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_REQ_SET_MODE, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_REQ_QRY_MODE, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_REQ_QRY_VOLUME, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_SET_MUTE_STATUS_REQ, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_GET_MUTE_STATUS_REQ, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_SET_FOREGROUND_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_BACKGROUND_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_FOREGROUND_QRY, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_MODEMLOOP_REQ, sizeof(APP_VC_ReqMsg), VOS_NULL_PTR },
    { APP_VC_MSG_SET_MSD_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(APP_VC_MsgSetMsdReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { APP_VC_MSG_QRY_MSD_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_ECALL_CFG_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(APP_VC_MsgSetEcallCfgReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_QRY_ECALL_CFG_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_TTYMODE_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(APP_VC_SetTtymodeReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { APP_VC_MSG_QRY_TTYMODE_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_ECALL_OPRTMODE_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(APP_VC_SetOprtmodeReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_SET_ECALL_PUSH_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(APP_VC_MsgSetMsdReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { APP_VC_MSG_SET_ECALL_ABORT_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(VOS_UINT32) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { APP_VC_MSG_I2S_TEST_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_HIFI_RESET_BEGIN_NTF, (sizeof(APP_VC_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { APP_VC_MSG_HIFI_RESET_END_NTF, (sizeof(APP_VC_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
};

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
/* WUEPS_PID_VC����DSP_PID_VOICE/ACPU_PID_VOICE_AGENT����ϢVC_VOICE_MsgId */
static TAF_ChkMsgLenNameMapTbl g_vcChkVoiceMsgLenTbl[] = {
    { ID_VOICE_VC_START_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_SET_DEVICE_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_SET_VOLUME_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_STOP_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_SET_CODEC_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_FOREGROUND_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_BACKGROUND_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_GROUND_RSP, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
#if (FEATURE_ECALL == FEATURE_ON)
    { ID_VOICE_VC_ECALL_TRANS_STATUS_NTF, sizeof(VCVOICE_ECALL_TransStatusNtf), VOS_NULL_PTR },
    { ID_VOICE_VC_SET_ECALLCFG_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_ECALLABORT_CNF, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_AL_ACK_REPORT_IND, sizeof(VOICEVC_AL_AckReportInd), VOS_NULL_PTR },
    { ID_VOICE_VC_START_SEND_MSD_IND, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
#endif
#if (FEATURE_ON == FEATURE_IMS)
    { ID_VOICE_VC_IMS_REPORT_IND, 0, APP_VC_ChkVoiceImsReportIndMsgLen },
    { ID_VOICE_VC_IMS_CTRL_RSP, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
    { ID_VOICE_VC_RX_SUSPEND_CNF, sizeof(VC_VOICE_RxSuspendCnf), VOS_NULL_PTR },
#endif
    { ID_VOICE_VC_DTMF_IND, sizeof(VOICEVC_DTMF_Ind), VOS_NULL_PTR },
    { ID_VOICE_VC_NV_REFRESH_RSP, sizeof(VCVOICE_OP_Rslt), VOS_NULL_PTR },
#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_VOICE_VC_I2STEST_RSLT_IND, sizeof(VOICE_VC_I2sTestRsltInd), VOS_NULL_PTR },
#endif
};
#endif

/* WUEPS_PID_DRV_AGENT����WUEPS_PID_AT����ϢDRV_AGENT_MsgType */
static TAF_ChkMsgLenNameMapTbl g_drvAgentChkAtMsgLenTbl[] = {
    { DRV_AGENT_MSID_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_GCF_IND, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_GPIOPL_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_GpioplSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_GPIOPL_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_DATALOCK_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_DatalockSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_TBATVOLT_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_VERTIME_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_YJCX_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_YJCX_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_APPDMVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_DLOADINFO_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_FLASHINFO_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_AUTHVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_AUTHORITYVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_AUTHORITYID_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_GODLOAD_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_CPULOAD_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_MFREELOCKSIZE_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_MEMINFO_QRY_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(VOS_UINT32) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_HARDWARE_QRY, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_FULL_HARDWARE_QRY, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_TMODE_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_VERSION_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_PFVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_FCHAN_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_FchanSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_SDLOAD_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_PRODTYPE_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SDREBOOT_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_DLOADVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SIMLOCK_SET_REQ, (sizeof(MN_APP_ReqMsg ) + sizeof(DRV_AGENT_SimlockSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_IMSICHG_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_NVRSTSTTS_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_CPNN_TEST_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_NVBACKUP_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_CPNN_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_NVRESTORE_MANU_DEFAULT_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_ADC_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(SPY_TempThresholdPara) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_TSELRF_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_TselrfSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_HKADC_GET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_TBAT_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SIMLOCK_NV_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(TAF_CUSTOM_CardlockStatus) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_SECURITY_SHELL)
    { DRV_AGENT_SPWORD_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_SpwordSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
#endif
    { DRV_AGENT_PSTANDBY_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_PstandbyReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_NVBACKUPSTAT_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_NANDBBC_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_NANDVER_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_CHIPTEMP_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_PHONE_SC)
    { DRV_AGENT_HUK_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_HukSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_FACAUTHPUBKEY_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_FacauthpubkeySetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_IDENTIFYSTART_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_IDENTIFYEND_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_IdentifyendOtaSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_PHONESIMLOCKINFO_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SIMLOCKDATAREAD_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_PHONEPHYNUM_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_PhonephynumSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
#if ( FEATURE_ON == FEATURE_LTE )
    { DRV_AGENT_INFORRS_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
#endif
    { DRV_AGENT_MAX_LOCK_TIMES_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(TAF_CUSTOM_SimLockMaxTimes) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_AP_SIMST_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_ApSimstSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_SWVER_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_QRY_CCPU_MEM_INFO_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
#if (FEATURE_ON == FEATURE_PHONE_SC)
    { DRV_AGENT_SIMLOCKWRITEEX_SET_REQ, 0, DRVAGENT_ChkDrvAgentSimLockWriteExSetMsgLen },
    { DRV_AGENT_SIMLOCKDATAREADEX_READ_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_SimlockdatareadexReadReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { DRV_AGENT_LEDTEST_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_LedTestSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_GPIOTEST_SET_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_ANTTEST_QRY_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_GPIOHEAT_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_GpioTestSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_HKADCTEST_SET_REQ,
      (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_HkadctestSetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_GPIO_QRY_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_GpioQryReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_GPIO_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_GpioSetReq) - TAF_DEFAULT_CONTENT_LEN),
      VOS_NULL_PTR },
    { DRV_AGENT_BOOTMODE_SET_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
#endif
    { DRV_AGENT_GETMODEMSCID_QRY_REQ, (sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_GETDONGLEINFO_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_CHECKAUTHORITY_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_CheckAuthoritySetReq) - TAF_DEFAULT_CONTENT_LEN), VOS_NULL_PTR },
    { DRV_AGENT_CONFORMAUTHORITY_SET_REQ, (sizeof(MN_APP_ReqMsg) + sizeof(DRV_AGENT_ConfirmAuthoritySetReq) - TAF_DEFAULT_CONTENT_LEN),  VOS_NULL_PTR },
    { DRV_AGENT_NETDOGINTEGRITY_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SARDOWN_SET_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
    { DRV_AGENT_SARDOWN_QRY_REQ, sizeof(MN_APP_ReqMsg), VOS_NULL_PTR },
};

/* WUEPS_PID_TAF����WUEPS_PID_TAF(ʵ����WUEPS_PID_AT)����ϢTAF_SSA_MsgId��TAF_PS_MsgId */
static TAF_ChkMsgLenNameMapTbl g_tafChkTafMsgLenTbl[] = {
    /* #define TAF_SSA_MSG_ID_BASE 0x5000 */
    { ID_TAF_SSA_SET_LCS_MOLR_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_SetLcsMolrReq)), VOS_NULL_PTR },
    { ID_TAF_SSA_GET_LCS_MOLR_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_GetLcsMolrReq)), VOS_NULL_PTR },
    { ID_TAF_SSA_SET_LCS_MTLR_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_SetLcsMtlrReq)), VOS_NULL_PTR },
    { ID_TAF_SSA_GET_LCS_MTLR_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_GetLcsMtlrReq)), VOS_NULL_PTR },
    { ID_TAF_SSA_SET_LCS_MTLRA_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_SetLcsMtlraReq)), VOS_NULL_PTR },
    { ID_TAF_SSA_GET_LCS_MTLRA_REQ, (sizeof(MSG_Header) + sizeof(TAF_SSA_GetLcsMtlraReq)), VOS_NULL_PTR },

    /* ID_TAF_APS_INTERNAL_BASE 0x0000 */
    /* ^DSFLOWQRY */
    { ID_MSG_TAF_PS_GET_DSFLOW_INFO_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_GetDsflowInfoReq)), VOS_NULL_PTR },

    /* ^DSFLOWCLR */
    { ID_MSG_TAF_PS_CLEAR_DSFLOW_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_ClearDsflowReq)), VOS_NULL_PTR },

    /* ^DSFLOWRPT */
    { ID_MSG_TAF_PS_CONFIG_DSFLOW_RPT_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_ConfigDsflowRptReq)), VOS_NULL_PTR },

    /* ^TRIG */
    { ID_MSG_TAF_PS_TRIG_GPRS_DATA_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_TrigGprsDataReq)), VOS_NULL_PTR },

    /* ^LTECS */
    { ID_MSG_TAF_PS_GET_LTE_CS_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_LtecsReq)), VOS_NULL_PTR },

    /* ^CEMODE */
    { ID_MSG_TAF_PS_GET_CEMODE_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_CemodeReq)), VOS_NULL_PTR },

    /* ^APDSFLOWRPT */
    { ID_MSG_TAF_PS_SET_APDSFLOW_RPT_CFG_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_SetApdsflowRptCfgReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_APDSFLOW_RPT_CFG_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_GetApdsflowRptCfgReq)),
      VOS_NULL_PTR },

    { ID_MSG_TAF_PS_SET_DSFLOW_NV_WRITE_CFG_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_SetDsflowNvWriteCfgReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_DSFLOW_NV_WRITE_CFG_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_GetDsflowNvWriteCfgReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_SET_CDMA_DIAL_MODE_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_CdataDialModeReq)), VOS_NULL_PTR },

    /* ^VTFLOWRPT */
    { ID_MSG_TAF_PS_CONFIG_VTFLOW_RPT_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_ConfigVtflowRptReq)), VOS_NULL_PTR },

    /* ^DATASWITCH */
    { ID_MSG_TAF_PS_SET_DATA_SWITCH_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_SetDataSwitchReq)), VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_DATA_SWITCH_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_GET_DATA_SWITCH_REQ_STRU)), VOS_NULL_PTR },

    /* ^DATAROAMSWITCH */
    { ID_MSG_TAF_PS_SET_DATA_ROAM_SWITCH_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_SetDataRoamSwitchReq)),
      VOS_NULL_PTR },
    { ID_MSG_TAF_PS_GET_DATA_ROAM_SWITCH_REQ, (sizeof(MSG_Header) + sizeof(TAF_PS_GET_DATA_ROAM_SWITCH_REQ_STRU)),
      VOS_NULL_PTR },
};

/* UEPS_PID_MTA����A����Ϣ���ȼ�飬MTAĿǰ��������WUEPS_PID_TAF���漰����MODEM��TAF PID��Ϣ */
static TAF_ChkMsgLenSndPidMapTbl g_mtaChkMsgLenTbl[] = {
    { WUEPS_PID_AT, TAF_MTA_GetChkAtMsgLenTblAddr, TAF_MTA_GetChkAtMsgLenTblSize },
#if (FEATURE_LTEV == FEATURE_ON)
    { WUEPS_PID_TAF, TAF_MTA_GetChkTafMsgLenTblAddr, TAF_MTA_GetChkTafMsgLenTblSize },
#endif
    { ACPU_PID_TAFAGENT, TAF_MTA_GetChkTafAgentMsgLenTblAddr, TAF_MTA_GetChkTafAgentMsgLenTblSize },
    { MSP_PID_CHR, TAF_GetChkChrMsgLenTblAddr, TAF_GetChkChrMsgLenTblSize },
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
    { MSP_PID_DIAG_APP_AGENT, TAF_GetChkDiagAppAgentMsgLenTblAddr, TAF_GetChkDiagAppAgentMsgLenTblSize },
#endif
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { I0_UEPS_PID_NRMM, TAF_MTA_GetChkNrmmMsgLenTblAddr, TAF_MTA_GetChkNrmmMsgLenTblSize },
    { I1_UEPS_PID_NRMM, TAF_MTA_GetChkNrmmMsgLenTblAddr, TAF_MTA_GetChkNrmmMsgLenTblSize },
    { I0_UEPS_PID_NRSM, TAF_MTA_GetChkNrsmMsgLenTblAddr, TAF_MTA_GetChkNrsmMsgLenTblSize },
    { I1_UEPS_PID_NRSM, TAF_MTA_GetChkNrsmMsgLenTblAddr, TAF_MTA_GetChkNrsmMsgLenTblSize },
    { I0_UEPS_PID_NRRC, TAF_MTA_GetChkNrrcMsgLenTblAddr, TAF_MTA_GetChkNrrcMsgLenTblSize },
    { I1_UEPS_PID_NRRC, TAF_MTA_GetChkNrrcMsgLenTblAddr, TAF_MTA_GetChkNrrcMsgLenTblSize },
    { NRAGENT_PID_AGENT, TAF_MTA_GetChkNgagentMsgLenTblAddr, TAF_MTA_GetChkNgagentMsgLenTblSize },
    { UEPS_PID_L2MA, TAF_MTA_GetChkNmacMsgLenTblAddr, TAF_MTA_GetChkNmacMsgLenTblSize },
    { I0_UEPS_PID_NPDCP_DL_NRT, TAF_MTA_GetNrdcpMsgLenTblAddr, TAF_MTA_GetNrdcpMsgLenTblSize }
#endif
#endif
};

/* WUEPS_PID_DRV_AGENT����A����Ϣ���ȼ�� */
static TAF_ChkMsgLenSndPidMapTbl g_drvagentChkMsgLenTbl[] = {
    { WUEPS_PID_AT, DRVAGENT_GetChkAtMsgLenTblAddr, DRVAGENT_GetChkAtMsgLenTblSize },
};

/* UEPS_PID_DSM������Ϣ���ȼ�� */
static TAF_ChkMsgLenSndPidMapTbl g_dsmChkMsgLenTbl[] = {
    { I0_WUEPS_PID_TAF, TAF_DSM_GetChkTafMsgLenTblAddr, TAF_DSM_GetChkTafMsgLenTblSize },
    { I1_WUEPS_PID_TAF, TAF_DSM_GetChkTafMsgLenTblAddr, TAF_DSM_GetChkTafMsgLenTblSize },
    { I2_WUEPS_PID_TAF, TAF_DSM_GetChkTafMsgLenTblAddr, TAF_DSM_GetChkTafMsgLenTblSize },
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { I0_UEPS_PID_PCF, TAF_DSM_GetChkPcfMsgLenTblAddr, TAF_DSM_GetChkPcfMsgLenTblSize },
    { I1_UEPS_PID_PCF, TAF_DSM_GetChkPcfMsgLenTblAddr, TAF_DSM_GetChkPcfMsgLenTblSize },
#endif
#endif
    { ACPU_PID_TAFAGENT, TAF_DSM_GetChkTafAgentMsgLenTblAddr, TAF_DSM_GetChkTafAgentMsgLenTblSize },
};

/* WUEPS_PID_VC����A����Ϣ���ȼ�飬UEPS_PID_XCALL��UEPS_PID_GUCALL��VC��C��/C����Ϣ���TBD */
static TAF_ChkMsgLenSndPidMapTbl g_vcChkMsgLenTbl[] = {
    { WUEPS_PID_AT, APP_VC_GetChkAtMsgLenTblAddr, APP_VC_GetChkAtMsgLenTblSize },
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
    { DSP_PID_VOICE, APP_VC_GetChkVoiceMsgLenTblAddr, APP_VC_GetChkVoiceMsgLenTblSize },
    { ACPU_PID_VOICE_AGENT, APP_VC_GetChkVoiceMsgLenTblAddr, APP_VC_GetChkVoiceMsgLenTblSize },
#endif
};

/* UEPS_PID_CCM����A����Ϣ���ȼ�� */
static TAF_ChkMsgLenSndPidMapTbl g_ccmChkMsgLenTbl[] = {
    { I0_WUEPS_PID_TAF, TAF_CCM_GetChkTafMsgLenTblAddr, TAF_CCM_GetChkTafMsgLenTblSize },
    { I1_WUEPS_PID_TAF, TAF_CCM_GetChkTafMsgLenTblAddr, TAF_CCM_GetChkTafMsgLenTblSize },
    { I2_WUEPS_PID_TAF, TAF_CCM_GetChkTafMsgLenTblAddr, TAF_CCM_GetChkTafMsgLenTblSize },
};

/* WUEPS_PID_TAF����A����Ϣ���ȼ�� */
static TAF_ChkMsgLenSndPidMapTbl g_tafChkMsgLenTbl[] = {
    { WUEPS_PID_AT, TAF_GetChkAtMsgLenTblAddr, TAF_GetChkAtMsgLenTblSize },
    { I0_WUEPS_PID_TAF, TAF_GetChkTafMsgLenTblAddr, TAF_GetChkTafMsgLenTblSize },
    { I1_WUEPS_PID_TAF, TAF_GetChkTafMsgLenTblAddr, TAF_GetChkTafMsgLenTblSize },
    { I2_WUEPS_PID_TAF, TAF_GetChkTafMsgLenTblAddr, TAF_GetChkTafMsgLenTblSize },
    { MSP_PID_CHR, TAF_GetChkChrMsgLenTblAddr, TAF_GetChkChrMsgLenTblSize },
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
    { MSP_PID_DIAG_APP_AGENT, TAF_GetChkDiagAppAgentMsgLenTblAddr, TAF_GetChkDiagAppAgentMsgLenTblSize },
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    { I0_UEPS_PID_NRSM, TAF_GetChkNrsmMsgLenTblAddr, TAF_GetChkNrsmMsgLenTblSize },
    { I1_UEPS_PID_NRSM, TAF_GetChkNrsmMsgLenTblAddr, TAF_GetChkNrsmMsgLenTblSize },
#endif
#endif
};

/* WUEPS_FID_TAF�µ�PID������Ϣ��Ч���ȼ��MAP */
static TAF_ChkMsgLenRcvPidMapTbl g_tafFidChkMsgLenTbl[] = {
    TAF_MSG_CHECK_TBL_ITEM(I0_WUEPS_PID_TAF, g_tafChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_WUEPS_PID_TAF, g_tafChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_WUEPS_PID_TAF, g_tafChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I0_UEPS_PID_CCM, g_ccmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_UEPS_PID_CCM, g_ccmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_UEPS_PID_CCM, g_ccmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I0_WUEPS_PID_VC, g_vcChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_WUEPS_PID_VC, g_vcChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_WUEPS_PID_VC, g_vcChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I0_UEPS_PID_DSM, g_dsmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_UEPS_PID_DSM, g_dsmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_UEPS_PID_DSM, g_dsmChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I0_WUEPS_PID_DRV_AGENT, g_drvagentChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_WUEPS_PID_DRV_AGENT, g_drvagentChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_WUEPS_PID_DRV_AGENT, g_drvagentChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I0_UEPS_PID_MTA, g_mtaChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I1_UEPS_PID_MTA, g_mtaChkMsgLenTbl),
    TAF_MSG_CHECK_TBL_ITEM(I2_UEPS_PID_MTA, g_mtaChkMsgLenTbl)
};

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkAtMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkAtMsgLenTbl);
}

#if (FEATURE_LTEV == FEATURE_ON)
STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkTafMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkTafMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkTafMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkTafMsgLenTbl);
}
#endif

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkTafAgentMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkTafAgentMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkTafAgentMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkTafAgentMsgLenTbl);
}

TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkTafAgentMsgLenTblAddr(VOS_VOID)
{
    return g_dsmChkTafAgentMsgLenTbl;
}

VOS_UINT32 TAF_DSM_GetChkTafAgentMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_dsmChkTafAgentMsgLenTbl);
}

TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkTafMsgLenTblAddr(VOS_VOID)
{
    return g_dsmChkTafMsgLenTbl;
}

VOS_UINT32 TAF_DSM_GetChkTafMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_dsmChkTafMsgLenTbl);
}

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_ON == FEATURE_UE_MODE_NR)
TAF_ChkMsgLenNameMapTbl* TAF_DSM_GetChkPcfMsgLenTblAddr(VOS_VOID)
{
    return g_dsmChkPcfMsgLenTbl;
}

VOS_UINT32 TAF_DSM_GetChkPcfMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_dsmChkPcfMsgLenTbl);
}
#endif
#endif

STATIC TAF_ChkMsgLenNameMapTbl* APP_VC_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_vcChkAtMsgLenTbl;
}

STATIC VOS_UINT32 APP_VC_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_vcChkAtMsgLenTbl);
}

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
STATIC TAF_ChkMsgLenNameMapTbl* APP_VC_GetChkVoiceMsgLenTblAddr(VOS_VOID)
{
    return g_vcChkVoiceMsgLenTbl;
}

STATIC VOS_UINT32 APP_VC_GetChkVoiceMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_vcChkVoiceMsgLenTbl);
}
#endif

STATIC TAF_ChkMsgLenNameMapTbl* DRVAGENT_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_drvAgentChkAtMsgLenTbl;
}

STATIC VOS_UINT32 DRVAGENT_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_drvAgentChkAtMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_CCM_GetChkTafMsgLenTblAddr(VOS_VOID)
{
    return g_ccmChkTafMsgLenTbl;
}

STATIC VOS_UINT32 TAF_CCM_GetChkTafMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_ccmChkTafMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkAtMsgLenTblAddr(VOS_VOID)
{
    return g_tafChkAtMsgLenTbl;
}

STATIC VOS_UINT32 TAF_GetChkAtMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_tafChkAtMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkTafMsgLenTblAddr(VOS_VOID)
{
    return g_tafChkTafMsgLenTbl;
}

STATIC VOS_UINT32 TAF_GetChkTafMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_tafChkTafMsgLenTbl);
}

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
STATIC TAF_ChkMsgLenNameMapTbl* TAF_GetChkNrsmMsgLenTblAddr(VOS_VOID)
{
    return g_tafChkNrsmMsgLenTbl;
}

STATIC VOS_UINT32 TAF_GetChkNrsmMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_tafChkNrsmMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrmmMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNrmmMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkNrmmMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNrmmMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrsmMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNrsmMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkNrsmMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNrsmMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNrrcMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNrrcMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkNrrcMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNrrcMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNgagentMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNgagentMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkNgagentMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNgagentMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetChkNmacMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNmacMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetChkNmacMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNmacMsgLenTbl);
}

STATIC TAF_ChkMsgLenNameMapTbl* TAF_MTA_GetNrdcpMsgLenTblAddr(VOS_VOID)
{
    return g_mtaChkNrdcpMsgLenTbl;
}

STATIC VOS_UINT32 TAF_MTA_GetNrdcpMsgLenTblSize(VOS_VOID)
{
    return TAF_GET_ARRAY_NUM(g_mtaChkNrdcpMsgLenTbl);
}
#endif
#endif

#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_IMS == FEATURE_ON)
STATIC VOS_UINT32 APP_VC_ChkVoiceImsReportIndMsgLen(const MSG_Header *msg)
{
    const VOICEVC_IMS_ReportInd *reportInd = VOS_NULL_PTR;
    VOS_UINT32 minLen;
    VOS_UINT32 expectedLen;

    minLen = sizeof(VOICEVC_IMS_ReportInd) - TAF_DEFAULT_CONTENT_LEN - VOS_MSG_HEAD_LENGTH;
    if (VOS_GET_MSG_LEN(msg) < minLen) {
        return VOS_FALSE;
    }
    reportInd = (const VOICEVC_IMS_ReportInd*)msg;
    expectedLen = minLen + reportInd->ulMsgLen;

    return TAF_ChkMsgLenWithExpectedLen(msg, expectedLen);
}
#endif
#endif


STATIC VOS_UINT32 TAF_MTA_ChkTafAgentMtaWriteAcoreNvReqMsgLen(const MSG_Header *msgHeader)
{
    const TAFAGENT_MTA_WriteAcoreNvReq *req = VOS_NULL_PTR;
    VOS_UINT32                          minMsgLen;

    minMsgLen = sizeof(TAFAGENT_MTA_WriteAcoreNvReq);
    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < minMsgLen) {
        return VOS_FALSE;
    }
    req = (const TAFAGENT_MTA_WriteAcoreNvReq *)msgHeader;

    if (req->nvLength > sizeof(req->data)) {
        minMsgLen = sizeof(TAFAGENT_MTA_WriteAcoreNvReq) + req->nvLength - sizeof(req->data);
    }

    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < minMsgLen) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_PHONE_SC)

STATIC VOS_UINT32 DRVAGENT_ChkDrvAgentSimLockWriteExSetMsgLen(const MSG_Header *msgHeader)
{
    const MN_APP_ReqMsg                  *mnAppReqMsg = VOS_NULL_PTR;
    const DRV_AGENT_SimlockwriteexSetReq *simlockWriteExSetReq = VOS_NULL_PTR;
    VOS_UINT32                            minMsgLen;

    minMsgLen = sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN + sizeof(DRV_AGENT_SimlockwriteexSetReq) -
        TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msgHeader) + VOS_MSG_HEAD_LENGTH) < minMsgLen) {
        return VOS_FALSE;
    }

    mnAppReqMsg          = (const MN_APP_ReqMsg *)msgHeader;
    simlockWriteExSetReq = (const DRV_AGENT_SimlockwriteexSetReq *)mnAppReqMsg->content;

    minMsgLen += simlockWriteExSetReq->simlockDataLen;
    if (VOS_GET_MSG_LEN(msgHeader) < (minMsgLen - VOS_MSG_HEAD_LENGTH)) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
#endif



VOS_UINT32 TAF_ChkMtaLrrcUeCapParamSetReqMsgLen(const MSG_Header *msg)
{
    const MN_APP_ReqMsg        *mtaMsg = VOS_NULL_PTR;
    const AT_MTA_UeCapParamSet *ueCapParamReq = VOS_NULL_PTR;
    VOS_UINT32                  fixLen;

    fixLen = sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN + sizeof(AT_MTA_UeCapParamSet);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < fixLen) {
        return VOS_FALSE;
    }
    mtaMsg        = (const MN_APP_ReqMsg *)msg;
    ueCapParamReq = (const AT_MTA_UeCapParamSet *)(mtaMsg->content);

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH)  < (fixLen + ueCapParamReq->msgLen)) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_UE_MODE_NR)

VOS_UINT32 TAF_MTA_ChkMtaNrrcUeCapParamSetReqMsgLen(const MSG_Header *msg)
{
    const MN_APP_ReqMsg        *mtaMsg = VOS_NULL_PTR;
    const AT_MTA_UeCapParamSet *ueCapParamReq = VOS_NULL_PTR;
    VOS_UINT32                  fixLen;

    fixLen = sizeof(MN_APP_ReqMsg) - TAF_DEFAULT_CONTENT_LEN + sizeof(AT_MTA_UeCapParamSet);
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < fixLen) {
        return VOS_FALSE;
    }
    mtaMsg        = (const MN_APP_ReqMsg *)msg;
    ueCapParamReq = (const AT_MTA_UeCapParamSet *)(mtaMsg->content);

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < (fixLen + ueCapParamReq->msgLen)) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


STATIC VOS_UINT32 TAF_DSM_ChkTafPs5GUePolicyInfoRspMsgLen(const MSG_Header *msg)
{
    const TAF_PS_Msg           *rcvMsg = VOS_NULL_PTR;
    const TAF_PS_5GUePolicyRsp *uePolicyRsp = VOS_NULL_PTR;
    VOS_UINT32                  minLen;

    minLen = sizeof(TAF_PS_Msg) + sizeof(TAF_PS_5GUePolicyRsp) - TAF_DEFAULT_CONTENT_LEN;
    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < minLen) {
        return VOS_FALSE;
    }
    rcvMsg      = (const TAF_PS_Msg *)msg;
    uePolicyRsp = (const TAF_PS_5GUePolicyRsp *)rcvMsg->content;

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < minLen + uePolicyRsp->uePolicyInfo.infoLen) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#if (VOS_OSA_CPU == OSA_CPU_CCPU)

STATIC VOS_UINT32 TAF_MtaCheckNrrcCapInfoNtfMsgLen(const MSG_Header *msg)
{
    NRRC_MTA_UeCapInfoNtf *ueCapInfoNtf = VOS_NULL_PTR;
    VOS_UINT32             minMsgLen;

    minMsgLen = sizeof(NRRC_MTA_UeCapInfoNtf) - VOS_MSG_HEAD_LENGTH;
    if (VOS_GET_MSG_LEN(msg) < minMsgLen) {
        return VOS_FALSE;
    }

    ueCapInfoNtf = (NRRC_MTA_UeCapInfoNtf *)msg;
    minMsgLen = minMsgLen + ueCapInfoNtf->msgLen;
#if (VOS_OS_VER == VOS_WIN32)
    /* PC���Գ����£��������Ϣ��������������Ϣ���Ȳ�һ��ʱ�����ϴ���
     * ���峡���£��������Ϣ����С����������Ϣ���ȣ��Ŷ�����Ϣ����������Ҳ�������� */
    if (VOS_GET_MSG_LEN(msg) != minMsgLen) {
        return VOS_FALSE;
    }
#else
    if (VOS_GET_MSG_LEN(msg) < minMsgLen) {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}

STATIC VOS_UINT32 TAF_DSM_ChkPcfUePolicyIndMsgLen(const MSG_Header *msgHeader)
{
    const PCF_DSM_UePolicyInd *uePolicyInd = VOS_NULL_PTR;
    VOS_UINT32                 minMsgLen;

    minMsgLen = sizeof(PCF_DSM_UePolicyInd) - VOS_MSG_HEAD_LENGTH;
    if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
        return VOS_FALSE;
    }

    uePolicyInd = (const PCF_DSM_UePolicyInd *)msgHeader;
    minMsgLen = minMsgLen + uePolicyInd->pcfMsgLen;

#if (VOS_OS_VER == VOS_WIN32)
    if (VOS_GET_MSG_LEN(msgHeader) != minMsgLen) {
        return VOS_FALSE;
    }
#else
    if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}

STATIC VOS_UINT32 TAF_APS_ChkNrsmLteHandoverToNrIndMsgLen(const MSG_Header *msgHeader)
{
    const NRSM_APS_LteHandoverToNrInd *handoverToNrInd = VOS_NULL_PTR;
    VOS_UINT32                         minMsgLen;

    minMsgLen = sizeof(NRSM_APS_LteHandoverToNrInd) - VOS_MSG_HEAD_LENGTH;
    if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
        return VOS_FALSE;
    }

    handoverToNrInd = (const NRSM_APS_LteHandoverToNrInd *)msgHeader;
    minMsgLen = minMsgLen + handoverToNrInd->epsBearerNum * sizeof(PS_MAP_EpsBearerInfo) +
        handoverToNrInd->qosRuleNum * sizeof(PS_NR_QosRuleInfo);

#if (VOS_OS_VER == VOS_WIN32)
    if (VOS_GET_MSG_LEN(msgHeader) != minMsgLen) {
        return VOS_FALSE;
    }
#else
    if (VOS_GET_MSG_LEN(msgHeader) < minMsgLen) {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}
#endif
#endif

VOS_VOID TAF_SortTafFidMsgLenChkTbl(VOS_VOID)
{
    TAF_SortChkMsgLenTblByMsgName(g_tafChkAtMsgLenTbl, TAF_GET_ARRAY_NUM(g_tafChkAtMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_tafChkTafMsgLenTbl, TAF_GET_ARRAY_NUM(g_tafChkTafMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkTafAgentMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkTafAgentMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkAtMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkAtMsgLenTbl));
#if (FEATURE_LTEV == FEATURE_ON)
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkTafMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkTafMsgLenTbl));
#endif
    TAF_SortChkMsgLenTblByMsgName(g_dsmChkTafAgentMsgLenTbl, TAF_GET_ARRAY_NUM(g_dsmChkTafAgentMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_dsmChkTafMsgLenTbl, TAF_GET_ARRAY_NUM(g_dsmChkTafMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_vcChkAtMsgLenTbl, TAF_GET_ARRAY_NUM(g_vcChkAtMsgLenTbl));
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
    TAF_SortChkMsgLenTblByMsgName(g_vcChkVoiceMsgLenTbl, TAF_GET_ARRAY_NUM(g_vcChkVoiceMsgLenTbl));
#endif
    TAF_SortChkMsgLenTblByMsgName(g_drvAgentChkAtMsgLenTbl, TAF_GET_ARRAY_NUM(g_drvAgentChkAtMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_ccmChkTafMsgLenTbl, TAF_GET_ARRAY_NUM(g_ccmChkTafMsgLenTbl));
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    TAF_SortChkMsgLenTblByMsgName(g_dsmChkPcfMsgLenTbl, TAF_GET_ARRAY_NUM(g_dsmChkPcfMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_tafChkNrsmMsgLenTbl, TAF_GET_ARRAY_NUM(g_tafChkNrsmMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNrmmMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNrmmMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNrsmMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNrsmMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNrrcMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNrrcMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNgagentMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNgagentMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNmacMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNmacMsgLenTbl));
    TAF_SortChkMsgLenTblByMsgName(g_mtaChkNrdcpMsgLenTbl, TAF_GET_ARRAY_NUM(g_mtaChkNrdcpMsgLenTbl));
#endif
#endif
}


VOS_UINT32 TAF_ChkTafFidRcvMsgLen(const MsgBlock *msg)
{
    if (VOS_GET_SENDER_ID(msg) == VOS_PID_TIMER) {
        return TAF_ChkTimerMsgLen(msg);
    }

    return TAF_ChkMsgLen(msg, g_tafFidChkMsgLenTbl, TAF_GET_ARRAY_NUM(g_tafFidChkMsgLenTbl));
}


