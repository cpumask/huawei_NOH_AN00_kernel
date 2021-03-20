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

#include "AtCmdMsgProc.h"
#include "securec.h"
#include "om_api.h"
#include "si_app_pb.h"
#include "si_app_stk.h"
#include "AtMsgPrint.h"
#include "at_mdrv_interface.h"
#include "at_phy_interface.h"
#include "ppp_interface.h"
#include "AtDataProc.h"
#include "AtEventReport.h"
#include "at_rabm_interface.h"
#include "AtRnicInterface.h"
#include "AtDeviceCmd.h"
#include "AtInit.h"
#include "at_common.h"

#if (FEATURE_LTE == FEATURE_ON)
#include "mdrv_nvim.h"
#include "at_lte_common.h"
#endif

#include "nv_stru_sys.h"
#include "nv_stru_was.h"
#include "nv_stru_gas.h"

#include "app_vc_api.h"

#include "AtCmdImsProc.h"

#include "product_config.h"

#include "AtCmdCallProc.h"
#include "AtCmdFtmProc.h"
#include "AtCmdMiscProc.h"
#include "AtCmdCagpsProc.h"
#include "AtCmdCssProc.h"
#include "at_acore_only_cmd.h"

#include "AtCmdSupsProc.h"
#include "AtCmdAgpsProc.h"
#include "AtMtMsgProc.h"
#include "at_file_handle.h"

#include "AtMtCommFun.h"
#include "mn_comm_api.h"
#if (FEATURE_LTEV == FEATURE_ON)
#include "at_ltev_msg_proc.h"
#include "vnas_at_interface.h"
#endif
#include "at_mta_interface.h"
#include "taf_phy_pid_def.h"
#include "dms_msg_chk.h"

#include "at_mbb_common.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMDMSGPROC_C

#define AT_CMD_TMP_MAX_LENGTH 20
#define AT_DECODE_MAX_LENGTH 500


#define AT_MAX_MNC_LEN 6
#define AT_PLMN_TYPE_NUM 2
#define AT_LTE_CATEGORY_DEFAULT_VALUE 6
#define AT_AUTOTEST_VALID_RSULT_NUM 3
#define NAS_OM_EVENTIND_DATA_LEN 4
#define AT_SFEATURE_TMP_STRING_LEN 50
#define AT_SIMLOCKCODE_LAST2CHAR_LEN 2
#define AT_HSPA_STATUS_ACTIVATE 1 /* 0表示未激活,那么DPA和UPA都支持;1表示激活 */
#define AT_CGAS_PARA_GAS_STATUS 0 /* CGAS的第一个参数GAS_STATUS */
#define AT_SYMBOL_CR_LF_LEN 2

#define AT_BUF_ELEM_PROROCOL 4
#define AT_BUF_ELEM_UE_CUR_CFUN_MODE 5
#define AT_BUF_ELEM_PRODUCT_CLASS 6
#define AT_BUF_ELEM_PRODUCT_ID 7

#define AT_PLMN_PLMNID 0      /* PLMN ID  */
#define AT_PLMN_PLMNSTATUS 1  /* PLMN状态 */
#define AT_PLMN_ACCESS_MODE 2 /* 接入方式 */

#define AT_ADDITION_NUMBER_MAX_NUM 3
#define AT_U16_BYTE_LEN 2
#define AT_HEX_0X_LENGTH 2

/* AT 模块处理来自AT AGENT消息函数对应表 */
static const AT_PROC_MsgFromDrvAgent g_atProcMsgFromDrvAgentTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { DRV_AGENT_MSID_QRY_CNF, AT_RcvDrvAgentMsidQryCnf },

    { DRV_AGENT_HARDWARE_QRY_RSP, AT_RcvDrvAgentHardwareQryRsp },
    { DRV_AGENT_FULL_HARDWARE_QRY_RSP, AT_RcvDrvAgentFullHardwareQryRsp },
    { DRV_AGENT_SIMLOCK_SET_CNF, AT_RcvDrvAgentSetSimlockCnf },

    { DRV_AGENT_VERTIME_QRY_CNF, AT_RcvDrvAgentVertimeQryRsp },
    { DRV_AGENT_YJCX_SET_CNF, AT_RcvDrvAgentYjcxSetCnf },
    { DRV_AGENT_YJCX_QRY_CNF, AT_RcvDrvAgentYjcxQryCnf },
    { DRV_AGENT_GPIOPL_SET_CNF, AT_RcvDrvAgentSetGpioplRsp },
    { DRV_AGENT_GPIOPL_QRY_CNF, AT_RcvDrvAgentQryGpioplRsp },
    { DRV_AGENT_DATALOCK_SET_CNF, AT_RcvDrvAgentSetDatalockRsp },
    { DRV_AGENT_TBATVOLT_QRY_CNF, AT_RcvDrvAgentQryTbatvoltRsp },
    { DRV_AGENT_VERSION_QRY_CNF, AT_RcvDrvAgentQryVersionRsp },
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    { DRV_AGENT_FCHAN_SET_CNF, AT_RcvDrvAgentSetFchanRsp },
#endif
    { DRV_AGENT_PRODTYPE_QRY_CNF, AT_RcvDrvAgentQryProdtypeRsp },

    { DRV_AGENT_CPULOAD_QRY_CNF, AT_RcvDrvAgentCpuloadQryRsp },
    { DRV_AGENT_MFREELOCKSIZE_QRY_CNF, AT_RcvDrvAgentMfreelocksizeQryRsp },
    { DRV_AGENT_MEMINFO_QRY_CNF, AT_RcvDrvAgentMemInfoQryRsp },
    { DRV_AGENT_DLOADINFO_QRY_CNF, AT_RcvDrvAgentDloadInfoQryRsp },
    { DRV_AGENT_FLASHINFO_QRY_CNF, AT_RcvDrvAgentFlashInfoQryRsp },
    { DRV_AGENT_AUTHORITYVER_QRY_CNF, AT_RcvDrvAgentAuthorityVerQryRsp },
    { DRV_AGENT_AUTHORITYID_QRY_CNF, AT_RcvDrvAgentAuthorityIdQryRsp },
    { DRV_AGENT_AUTHVER_QRY_CNF, AT_RcvDrvAgentAuthVerQryRsp },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { DRV_AGENT_GODLOAD_SET_CNF, AT_RcvDrvAgentGodloadSetRsp },
#endif
    { DRV_AGENT_PFVER_QRY_CNF, AT_RcvDrvAgentPfverQryRsp },
    { DRV_AGENT_HWNATQRY_QRY_CNF, AT_RcvDrvAgentHwnatQryRsp },
    { DRV_AGENT_SDLOAD_SET_CNF, AT_RcvDrvAgentSdloadSetRsp },
    { DRV_AGENT_APPDMVER_QRY_CNF, AT_RcvDrvAgentAppdmverQryRsp },
    { DRV_AGENT_DLOADVER_QRY_CNF, AT_RcvDrvAgentDloadverQryRsp },

    { DRV_AGENT_IMSICHG_QRY_CNF, AT_RcvDrvAgentImsiChgQryRsp },
    { DRV_AGENT_INFORBU_SET_CNF, AT_RcvDrvAgentInfoRbuSetRsp },
#if (FEATURE_LTE == FEATURE_ON)
    { DRV_AGENT_INFORRS_SET_CNF, AT_RcvDrvAgentInfoRrsSetRsp },
#endif
    { DRV_AGENT_CPNN_QRY_CNF, AT_RcvDrvAgentCpnnQryRsp },
    { DRV_AGENT_CPNN_TEST_CNF, AT_RcvDrvAgentCpnnTestRsp },
    { DRV_AGENT_NVBACKUP_SET_CNF, AT_RcvDrvAgentNvBackupSetRsp },

    { DRV_AGENT_NVRESTORE_SET_CNF, AT_RcvDrvAgentSetNvRestoreCnf },
    { DRV_AGENT_NVRSTSTTS_QRY_CNF, AT_RcvDrvAgentQryNvRestoreRstCnf },
    { DRV_AGENT_NVRESTORE_MANU_DEFAULT_CNF, AT_RcvDrvAgentNvRestoreManuDefaultRsp },

    { DRV_AGENT_ADC_SET_CNF, AT_RcvDrvAgentSetAdcRsp },
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    { DRV_AGENT_TSELRF_SET_CNF, AT_RcvDrvAgentTseLrfSetRsp },
#endif
    { DRV_AGENT_HKADC_GET_CNF, AT_RcvDrvAgentHkAdcGetRsp },

    { DRV_AGENT_TBAT_QRY_CNF, AT_RcvDrvAgentQryTbatRsp },
#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
    { DRV_AGENT_SPWORD_SET_CNF, AT_RcvDrvAgentSetSpwordRsp },
#endif

    { DRV_AGENT_NVBACKUPSTAT_QRY_CNF, AT_RcvDrvAgentNvBackupStatQryRsp },
    { DRV_AGENT_NANDBBC_QRY_CNF, AT_RcvDrvAgentNandBadBlockQryRsp },
    { DRV_AGENT_NANDVER_QRY_CNF, AT_RcvDrvAgentNandDevInfoQryRsp },
    { DRV_AGENT_CHIPTEMP_QRY_CNF, AT_RcvDrvAgentChipTempQryRsp },

    { DRV_AGENT_MAX_LOCK_TIMES_SET_CNF, AT_RcvDrvAgentSetMaxLockTmsRsp },

    { DRV_AGENT_AP_SIMST_SET_CNF, AT_RcvDrvAgentSetApSimstRsp },

    { DRV_AGENT_HUK_SET_CNF, AT_RcvDrvAgentHukSetCnf },
    { DRV_AGENT_FACAUTHPUBKEY_SET_CNF, AT_RcvDrvAgentFacAuthPubkeySetCnf },
    { DRV_AGENT_IDENTIFYSTART_SET_CNF, AT_RcvDrvAgentIdentifyStartSetCnf },
    { DRV_AGENT_IDENTIFYEND_SET_CNF, AT_RcvDrvAgentIdentifyEndSetCnf },
    { DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF, AT_RcvDrvAgentSimlockDataWriteSetCnf },
    { DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF, AT_RcvDrvAgentPhoneSimlockInfoQryCnf },
    { DRV_AGENT_PHONESIMLOCKINFO_QRY_NEW_CNF, AT_RcvDrvAgentPhoneSimlockInfoQryCnf },
    { DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF, AT_RcvDrvAgentSimlockDataReadQryCnf },
    { DRV_AGENT_PHONEPHYNUM_SET_CNF, AT_RcvDrvAgentPhonePhynumSetCnf },
    { DRV_AGENT_OPWORD_SET_CNF, AT_RcvDrvAgentOpwordSetCnf },

    { DRV_AGENT_SWVER_SET_CNF, AT_RcvDrvAgentSwverSetCnf },

    { DRV_AGENT_NVMANUFACTUREEXT_SET_CNF, AT_RcvNvManufactureExtSetCnf },


    { DRV_AGENT_QRY_CCPU_MEM_INFO_CNF, AT_RcvDrvAgentQryCcpuMemInfoCnf },

    { DRV_AGENT_SIMLOCKWRITEEX_SET_CNF, AT_RcvDrvAgentSimlockWriteExSetCnf },
    { DRV_AGENT_SIMLOCKDATAREADEX_SET_CNF, AT_RcvDrvAgentSimlockDataReadExReadCnf },
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { DRV_AGENT_LEDTEST_SET_CNF, AT_RcvDrvAgentSetLedTestRsp },
    { DRV_AGENT_GPIOTEST_SET_CNF, AT_RcvDrvAgentSetGpioTestRsp },
    { DRV_AGENT_ANTTEST_QRY_CNF, AT_RcvDrvAgentQryAntTestRsp },
    { DRV_AGENT_GPIOHEAT_SET_CNF, AT_RcvDrvAgentSetGpioHeatSetRsp },
    { DRV_AGENT_HKADCTEST_SET_CNF, AT_RcvDrvAgentSetHkadcTestRsp },
    { DRV_AGENT_GPIO_QRY_CNF, AT_RcvDrvAgentGpioQryRsp },
    { DRV_AGENT_GPIO_SET_CNF, AT_RcvDrvAgentGpioSetRsp },
    { DRV_AGENT_BOOTMODE_SET_CNF, AT_RcvDrvAgentBootModeSetRsp },
#endif
    { DRV_AGENT_GETMODEMSCID_QRY_CNF, AT_RcvDrvAgentGetSimlockEncryptIdQryCnf },
};

/* AT模块处理来自MTA消息函数对应表 */
static const AT_PROC_MsgFromMta g_atProcMsgFromMtaTab[] = {
    /* 消息ID */ /* 消息处理函数 */
    { ID_MTA_AT_CPOS_SET_CNF, AT_RcvMtaCposSetCnf },
    { ID_MTA_AT_CPOSR_IND, AT_RcvMtaCposrInd },
    { ID_MTA_AT_XCPOSRRPT_IND, AT_RcvMtaXcposrRptInd },
    { ID_MTA_AT_CGPSCLOCK_SET_CNF, AT_RcvMtaCgpsClockSetCnf },

    { ID_MTA_AT_SIMLOCKUNLOCK_SET_CNF, AT_RcvMtaSimlockUnlockSetCnf },

    { ID_MTA_AT_QRY_NMR_CNF, AT_RcvMtaQryNmrCnf },

    { ID_MTA_AT_WRR_AUTOTEST_QRY_CNF, AT_RcvMtaWrrAutotestQryCnf },
    { ID_MTA_AT_WRR_CELLINFO_QRY_CNF, AT_RcvMtaWrrCellinfoQryCnf },
    { ID_MTA_AT_WRR_MEANRPT_QRY_CNF, AT_RcvMtaWrrMeanrptQryCnf },
    { ID_MTA_AT_WRR_FREQLOCK_SET_CNF, AT_RcvMtaWrrFreqLockSetCnf },
    { ID_MTA_AT_WRR_RRC_VERSION_SET_CNF, AT_RcvMtaWrrRrcVersionSetCnf },
    { ID_MTA_AT_WRR_CELLSRH_SET_CNF, AT_RcvMtaWrrCellSrhSetCnf },
    { ID_MTA_AT_WRR_FREQLOCK_QRY_CNF, AT_RcvMtaWrrFreqLockQryCnf },
    { ID_MTA_AT_WRR_RRC_VERSION_QRY_CNF, AT_RcvMtaWrrRrcVersionQryCnf },
    { ID_MTA_AT_WRR_CELLSRH_QRY_CNF, AT_RcvMtaWrrCellSrhQryCnf },

    { ID_MTA_AT_GAS_AUTOTEST_QRY_CNF, AT_RcvMtaGrrAutotestQryCnf },

    { ID_MTA_AT_BODY_SAR_SET_CNF, AT_RcvMtaBodySarSetCnf },

    { ID_MTA_AT_CURC_QRY_CNF, AT_RcvMtaQryCurcCnf },
    { ID_MTA_AT_UNSOLICITED_RPT_SET_CNF, AT_RcvMtaSetUnsolicitedRptCnf },
    { ID_MTA_AT_UNSOLICITED_RPT_QRY_CNF, AT_RcvMtaQryUnsolicitedRptCnf },

    { ID_MTA_AT_IMEI_VERIFY_QRY_CNF, AT_RcvMtaImeiVerifyQryCnf },
    { ID_MTA_AT_CGSN_QRY_CNF, AT_RcvMtaCgsnQryCnf },
    { ID_MTA_AT_NCELL_MONITOR_SET_CNF, AT_RcvMtaSetNCellMonitorCnf },
    { ID_MTA_AT_NCELL_MONITOR_QRY_CNF, AT_RcvMtaQryNCellMonitorCnf },
    { ID_MTA_AT_NCELL_MONITOR_IND, AT_RcvMtaNCellMonitorInd },

    { ID_MTA_AT_REFCLKFREQ_SET_CNF, AT_RcvMtaRefclkfreqSetCnf },
    { ID_MTA_AT_REFCLKFREQ_QRY_CNF, AT_RcvMtaRefclkfreqQryCnf },
    { ID_MTA_AT_REFCLKFREQ_IND, AT_RcvMtaRefclkfreqInd },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_NW_SLICE_AUTH_CMD_IND, AT_RcvMtaNwSliceAuthCmdInd },
    { ID_MTA_AT_NW_SLICE_AUTH_RSLT_IND, AT_RcvMtaNwSliceAuthRsltInd },
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    { ID_MTA_AT_RFICSSIRD_QRY_CNF, AT_RcvMtaRficSsiRdQryCnf },
#endif

    { ID_MTA_AT_HANDLEDECT_SET_CNF, AT_RcvMtaHandleDectSetCnf },
    { ID_MTA_AT_HANDLEDECT_QRY_CNF, AT_RcvMtaHandleDectQryCnf },

    { ID_MTA_AT_PS_TRANSFER_IND, AT_RcvMtaPsTransferInd },

#if (FEATURE_DSDS == FEATURE_ON)
    { ID_MTA_AT_RRC_PROTECT_PS_CNF, AT_RcvMtaPsProtectSetCnf },
#endif
    { ID_MTA_AT_PHY_INIT_CNF, AT_RcvMtaPhyInitCnf },

    { ID_MTA_AT_ECID_SET_CNF, AT_RcvMtaEcidSetCnf },

    { ID_MTA_AT_MIPICLK_QRY_CNF, AT_RcvMtaMipiInfoCnf },
    { ID_MTA_AT_MIPICLK_INFO_IND, AT_RcvMtaMipiInfoInd },
    { ID_MTA_AT_SET_DPDTTEST_FLAG_CNF, AT_RcvMtaSetDpdtTestFlagCnf },
    { ID_MTA_AT_SET_DPDT_VALUE_CNF, AT_RcvMtaSetDpdtValueCnf },
    { ID_MTA_AT_QRY_DPDT_VALUE_CNF, AT_RcvMtaQryDpdtValueCnf },

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_SET_TRX_TAS_CNF, AT_RcvMtaSetTrxTasCnf },
    { ID_MTA_AT_QRY_TRX_TAS_CNF, AT_RcvMtaQryTrxTasCnf },
#endif

    { ID_MTA_AT_SET_JAM_DETECT_CNF, AT_RcvMtaSetJamDetectCnf },
    { ID_MTA_AT_QRY_JAM_DETECT_CNF, AT_RcvMtaQryJamDetectCnf },
    { ID_MTA_AT_JAM_DETECT_IND, AT_RcvMtaJamDetectInd },

    { ID_MTA_AT_SET_GSM_FREQLOCK_CNF, AT_RcvMtaSetGFreqLockCnf },

    { ID_MTA_AT_QRY_GSM_FREQLOCK_CNF, AT_RcvMtaGFreqLockQryCnf },

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
    { ID_MTA_AT_SET_M2M_FREQLOCK_CNF, AT_RcvMtaSetM2MFreqLockCnf },
    { ID_MTA_AT_QRY_M2M_FREQLOCK_CNF, AT_RcvMtaQryM2MFreqLockCnf },
#endif
    { ID_MTA_AT_XPASS_INFO_IND, AT_RcvMtaXpassInfoInd },

    { ID_AT_MTA_SET_FEMCTRL_CNF, AT_RcvMtaSetFemctrlCnf },


#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)
    { ID_MTA_AT_MBMS_SERVICE_OPTION_SET_CNF, AT_RcvMtaMBMSServiceOptSetCnf },
    { ID_MTA_AT_MBMS_SERVICE_STATE_SET_CNF, AT_RcvMtaMBMSServiceStateSetCnf },
    { ID_MTA_AT_MBMS_PREFERENCE_SET_CNF, AT_RcvMtaMBMSPreferenceSetCnf },
    { ID_MTA_AT_SIB16_NETWORK_TIME_QRY_CNF, AT_RcvMtaMBMSSib16NetworkTimeQryCnf },
    { ID_MTA_AT_BSSI_SIGNAL_LEVEL_QRY_CNF, AT_RcvMtaMBMSBssiSignalLevelQryCnf },
    { ID_MTA_AT_NETWORK_INFO_QRY_CNF, AT_RcvMtaMBMSNetworkInfoQryCnf },
    { ID_MTA_AT_EMBMS_STATUS_QRY_CNF, AT_RcvMtaMBMSModemStatusQryCnf },
    { ID_MTA_AT_MBMS_UNSOLICITED_CFG_SET_CNF, AT_RcvMtaMBMSEVSetCnf },
    { ID_MTA_AT_MBMS_SERVICE_EVENT_IND, AT_RcvMtaMBMSServiceEventInd },
    { ID_MTA_AT_INTEREST_LIST_SET_CNF, AT_RcvMtaMBMSInterestListSetCnf },
    { ID_MTA_AT_MBMS_AVL_SERVICE_LIST_QRY_CNF, AT_RcvMtaMBMSCmdQryCnf },
#endif
    { ID_MTA_AT_LTE_LOW_POWER_SET_CNF, AT_RcvMtaLteLowPowerSetCnf },
    { ID_MTA_AT_LTE_WIFI_COEX_SET_CNF, AT_RcvMtaIsmCoexSetCnf },
    { ID_MTA_AT_LTE_WIFI_COEX_QRY_CNF, AT_RcvMtaIsmCoexQryCnf },
    { ID_MTA_AT_LTE_CA_CFG_SET_CNF, AT_RcvMtaLteCaCfgSetCnf },
    { ID_MTA_AT_LTE_CA_CELLEX_QRY_CNF, AT_RcvMtaLteCaCellExQryCnf },
    { ID_MTA_AT_LTE_CA_CELLEX_INFO_NTF, AT_RcvMtaLteCaCellExInfoNtf },
    { ID_MTA_AT_LTE_CA_CELL_RPT_CFG_SET_CNF, AT_RcvMtaCACellSetCnf },
    { ID_MTA_AT_LTE_CA_CELL_RPT_CFG_QRY_CNF, AT_RcvMtaCACellQryCnf },
    { ID_MTA_AT_FINE_TIME_SET_CNF, AT_RcvMtaFineTimeSetCnf },
    { ID_MTA_AT_SIB_FINE_TIME_NTF, AT_RcvMtaSibFineTimeNtf },
    { ID_MTA_AT_LPP_FINE_TIME_NTF, AT_RcvMtaLppFineTimeNtf },
    { ID_MTA_AT_LL2_COM_CFG_SET_CNF, AT_RcvMtaLL2ComCfgSetCnf },
    { ID_MTA_AT_LL2_COM_CFG_QRY_CNF, AT_RcvMtaLL2ComCfgQryCnf },
    { ID_MTA_AT_LRRC_UE_CAP_PARA_INFO_NTF, AT_RcvMtaLrrcUeCapNtf },
    { ID_MTA_AT_LRRC_UE_CAP_PARA_SET_CNF, AT_RcvMtaLrrcUeCapSetCnf },
    { ID_MTA_AT_OVERHEATING_CFG_SET_CNF, AT_RcvMtaOverHeatingSetCnf },
    { ID_MTA_AT_OVERHEATING_CFG_QRY_CNF, AT_RcvMtaOverHeatingQryCnf },

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_NL2_COM_CFG_SET_CNF, AT_RcvMtaNL2ComCfgSetCnf },
    { ID_MTA_AT_NL2_COM_CFG_QRY_CNF, AT_RcvMtaNL2ComCfgQryCnf },
    { ID_MTA_AT_NPDCP_SLEEP_TRHRES_CFG_CNF, AT_RcvMtaNPdcpSleepThresCfgCnf },
    { ID_MTA_AT_NRRC_UE_CAP_PARA_SET_CNF, AT_RcvMtaNrrcUeCapSetCnf },
    { ID_MTA_AT_NRRC_UE_CAP_PARA_INFO_NTF, AT_RcvMtaNrrcUeCapNtf },
#endif

    { ID_MTA_AT_SET_FR_CNF, AT_RcvMtaSetFrCnf },

    { ID_MTA_AT_SIB16_TIME_UPDATE_IND, AT_RcvMtaSib16TimeUpdateInd },

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_LENDC_QRY_CNF, AT_RcvMtaLendcQryCnf },
    { ID_MTA_AT_LENDC_INFO_IND, AT_RcvMtaLendcInfoInd },
    { ID_MTA_AT_NRRCCAP_CFG_SET_CNF, AT_RcvMtaNrrcCapCfgCnf },
    { ID_MTA_AT_NRRCCAP_QRY_CNF, AT_RcvMtaNrrcCapQryCnf },
    { ID_MTA_AT_NRPOWERSAVING_CFG_SET_CNF, AT_RcvMtaNrPowerSavingCfgCnf },
    { ID_MTA_AT_NRPOWERSAVING_QRY_CNF, AT_RcvMtaNrPowerSavingQryCnf },
    { ID_MTA_AT_NRPWRCTRL_SET_CNF, AT_RcvMtaNrPwrCtrlSetCnf },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_MTA_AT_NR_FREQLOCK_SET_CNF, AT_RcvMtaNrFreqLockSetCnf },
    { ID_MTA_AT_NR_FREQLOCK_QRY_CNF, AT_RcvMtaNrFreqLockQryCnf },
#endif
    { ID_MTA_AT_NR_CA_CELL_INFO_QRY_CNF, AT_RcvMtaNrCaCellInfoQryCnf },
    { ID_MTA_AT_NR_CA_CELL_INFO_RPT_CFG_SET_CNF, AT_RcvMtaNrCaCellInfoRptCfgSetCnf },
    { ID_MTA_AT_NR_CA_CELL_INFO_RPT_CFG_QRY_CNF, AT_RcvMtaNrCaCellInfoRptCfgQryCnf },
    { ID_MTA_AT_NR_CA_CELL_INFO_IND, AT_RcvMtaNrCaCellInfoInd },
    { ID_MTA_AT_NR_NW_CAP_INFO_REPORT_IND, AT_RcvMtaNrNwCapInfoReportInd },
    { ID_MTA_AT_NR_NW_CAP_INFO_RPT_CFG_SET_CNF, AT_RcvMtaNrNwCapInfoRptCfgSetCnf },
    { ID_MTA_AT_NR_NW_CAP_INFO_QRY_CNF, AT_RcvMtaNrNwCapInfoQryCnf },
    { ID_MTA_AT_NR_NW_CAP_INFO_RPT_CFG_QRY_CNF, AT_RcvMtaNrNwCapInfoRptCfgQryCnf },
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_MTA_AT_MEID_SET_CNF, AT_RcvMtaMeidSetCnf },
    { ID_MTA_AT_MEID_QRY_CNF, AT_RcvMtaMeidQryCnf },

    { ID_MTA_AT_EVDO_SYS_EVENT_CNF, AT_RcvMtaEvdoSysEventSetCnf },
    { ID_MTA_AT_EVDO_SIG_MASK_CNF, AT_RcvMtaEvdoSigMaskSetCnf },
    { ID_MTA_AT_EVDO_REVA_RLINK_INFO_IND, AT_RcvMtaEvdoRevARLinkInfoInd },
    { ID_MTA_AT_EVDO_SIG_EXEVENT_IND, AT_RcvMtaEvdoSigExEventInd },
    { ID_MTA_AT_NO_CARD_MODE_SET_CNF, AT_RcvMtaNoCardModeSetCnf },
    { ID_MTA_AT_NO_CARD_MODE_QRY_CNF, AT_RcvMtaNoCardModeQryCnf },

#endif
    { ID_MTA_AT_TRANSMODE_QRY_CNF, AT_RcvMtaTransModeQryCnf },

    { ID_MTA_AT_UE_CENTER_SET_CNF, AT_RcvMtaUECenterSetCnf },
    { ID_MTA_AT_UE_CENTER_QRY_CNF, AT_RcvMtaUECenterQryCnf },
    { ID_MTA_AT_SET_NETMON_SCELL_CNF, AT_RcvMtaSetNetMonSCellCnf },
    { ID_MTA_AT_SET_NETMON_NCELL_CNF, AT_RcvMtaSetNetMonNCellCnf },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_SET_NETMON_SSCELL_CNF, AT_RcvMtaSetNetMonSSCellCnf },
    { ID_MTA_AT_NR_SSB_ID_QRY_CNF, AT_RcvMtaNrSsbIdQryCnf },
#endif
    { ID_MTA_AT_QRY_AFC_CLK_FREQ_XOCOEF_CNF, AT_RcvMtaAfcClkInfoCnf },
    { ID_MTA_AT_ANQUERY_QRY_CNF, AT_RcvMtaAnqueryQryCnf },
    { ID_MTA_AT_CSNR_QRY_CNF, AT_RcvMtaCsnrQryCnf },
    { ID_MTA_AT_CSQLVL_QRY_CNF, AT_RcvMtaCsqlvlQryCnf },
    { ID_MTA_AT_XCPOSR_SET_CNF, AT_RcvMtaSetXCposrCnf },
    { ID_MTA_AT_XCPOSR_QRY_CNF, AT_RcvMtaQryXcposrCnf },
    { ID_MTA_AT_XCPOSRRPT_SET_CNF, AT_RcvMtaSetXcposrRptCnf },
    { ID_MTA_AT_XCPOSRRPT_QRY_CNF, AT_RcvMtaQryXcposrRptCnf },
    { ID_MTA_AT_CLEAR_HISTORY_FREQ_CNF, AT_RcvMtaClearHistoryFreqCnf },
    { ID_MTA_AT_SET_SENSOR_CNF, AT_RcvMtaSetSensorCnf },
    { ID_MTA_AT_SET_SCREEN_CNF, AT_RcvMtaSetScreenCnf },
    { ID_MTA_AT_FRAT_IGNITION_QRY_CNF, AT_RcvFratIgnitionQryCnf },
    { ID_MTA_AT_FRAT_IGNITION_SET_CNF, AT_RcvFratIgnitionSetCnf },

    { ID_MTA_AT_SET_MODEM_TIME_CNF, AT_RcvMtaSetModemTimeCnf },

    { ID_MTA_AT_RX_TEST_MODE_SET_CNF, AT_RcvMtaSetRxTestModeCnf },

    { ID_MTA_AT_AFC_CLK_UNLOCK_CAUSE_IND, AT_RcvMtaAfcClkUnlockCauseInd },

    { ID_MTA_AT_BESTFREQ_SET_CNF, AT_RcvMtaSetBestFreqCnf },
    { ID_MTA_AT_BEST_FREQ_INFO_IND, AT_RcvMtaBestFreqInfoInd },
    { ID_MTA_AT_BESTFREQ_QRY_CNF, AT_RcvMtaQryBestFreqCnf },

    { ID_MTA_AT_RFIC_DIE_ID_QRY_CNF, AT_RcvMtaRficDieIDQryCnf },

    { ID_MTA_AT_RFFE_DIE_ID_QRY_CNF, AT_RcvMtaRffeDieIDQryCnf },

    { ID_MTA_AT_MIPI_WREX_CNF, AT_RcvMtaMipiWrEXCnf },
    { ID_MTA_AT_MIPI_RDEX_CNF, AT_RcvMtaMipiRdEXCnf },

    { ID_MTA_AT_PHY_COM_CFG_SET_CNF, AT_RcvMtaPhyComCfgSetCnf },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    {ID_MTA_AT_NRPHY_COM_CFG_SET_CNF, AT_RcvMtaNrPhyComCfgSetCnf},
#endif
    { ID_MTA_AT_CRRCONN_SET_CNF, AT_RcvMtaSetCrrconnCnf },
    { ID_MTA_AT_CRRCONN_QRY_CNF, AT_RcvMtaQryCrrconnCnf },
    { ID_MTA_AT_CRRCONN_STATUS_IND, AT_RcvMtaCrrconnStatusInd },
    { ID_MTA_AT_VTRLQUALRPT_SET_CNF, AT_RcvMtaSetVtrlqualrptCnf },
    { ID_MTA_AT_RL_QUALITY_INFO_IND, AT_RcvMtaRlQualityInfoInd },
    { ID_MTA_AT_VIDEO_DIAG_INFO_RPT, AT_RcvMtaVideoDiagInfoRpt },

    { ID_MTA_AT_PMU_DIE_SN_QRY_CNF, AT_RcvMtaPmuDieSNQryCnf },

    { ID_MTA_AT_MODEM_CAP_UPDATE_CNF, AT_RcvMtaModemCapUpdateCnf },

    { ID_MTA_AT_TAS_TEST_CFG_CNF, AT_RcvMtaTasTestCfgCnf },
    { ID_MTA_AT_TAS_TEST_QRY_CNF, AT_RcvMtaTasTestQryCnf },

    { ID_MTA_AT_LTE_CATEGORY_INFO_IND, AT_RcvMtaLteCategoryInfoInd },

    { ID_MTA_AT_ACCESS_STRATUM_REL_IND, AT_RcvMtaAccessStratumRelInd },

    { ID_MTA_AT_RS_INFO_QRY_CNF, AT_RcvMtaRsInfoQryCnf },

    { ID_MTA_AT_ERRCCAP_CFG_SET_CNF, AT_RcvMtaErrcCapCfgCnf },
    { ID_MTA_AT_ERRCCAP_QRY_SET_CNF, AT_RcvMtaErrcCapQryCnf },

    { ID_MTA_AT_DEL_CELLENTITY_CNF, AT_RcvMtaDelCellEntityCnf },

    { ID_MTA_AT_PSEUCELL_INFO_SET_CNF, AT_RcvMtaPseucellInfoSetCnf },
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    { ID_MTA_AT_POWER_DET_QRY_CNF, AT_RcvMtaPowerDetQryCnf },
#endif

    { ID_MTA_AT_MIPIREAD_SET_CNF, AT_RcvMtaSetMipiReadCnf },

    { ID_MTA_AT_PHYMIPIWRITE_SET_CNF, AT_RcvMtaSetPhyMipiWriteCnf },

    { ID_MTA_AT_CHRALARMRLAT_CFG_SET_CNF, AT_RcvMtaChrAlarmRlatCfgSetCnf },

    { ID_MTA_AT_ECC_CFG_SET_CNF, AT_RcvMtaEccCfgCnf },
    { ID_MTA_AT_ECC_STATUS_IND, AT_RcvMtaEccStatusInd },

    { ID_MTA_AT_EPDUR_DATA_IND, AT_RcvMtaEpduDataInd },
    { ID_MTA_AT_EPDU_SET_CNF, AT_RcvMtaEpduSetCnf },

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)
    { ID_MTA_AT_DCXO_SAMPLE_QRY_CNF, AT_RcvMtaRcmDcxoSampleQryCnf },
#endif

    { ID_MTA_AT_PHY_COMM_ACK_IND, AT_RcvMtaPhyCommAckInd },

    { ID_MTA_AT_COMM_BOOSTER_SET_CNF, AT_RcvMtaCommBoosterSetCnf },
    { ID_MTA_AT_COMM_BOOSTER_IND, AT_RcvMtaCommBoosterInd },
    { ID_MTA_AT_COMM_BOOSTER_QUERY_CNF, AT_RcvMtaCommBoosterQueryCnf },

    { ID_MTA_AT_NVLOAD_SET_CNF, AT_RcvMtaNvLoadSetCnf },

    { ID_MTA_AT_SMS_DOMAIN_SET_CNF, AT_RcvMtaSmsDomainSetCnf },
    { ID_MTA_AT_SMS_DOMAIN_QRY_CNF, AT_RcvMtaSmsDomainQryCnf },

    { ID_MTA_AT_SET_SAMPLE_CNF, AT_RcvMtaSetSampleCnf },

    { ID_MTA_AT_GPS_LOCSET_SET_CNF, AT_RcvMtaGpsLocSetCnf },

    { ID_MTA_AT_CCLK_QRY_CNF, AT_RcvMtaCclkQryCnf },

    { ID_MTA_AT_TEMP_PROTECT_IND, AT_RcvMtaTempProtectInd },

    { ID_AT_MTA_GAME_MODE_SET_CNF, AT_RcvGameModeSetCnf },
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_MTA_AT_CDMA_MODEM_CAP_SET_CNF, AT_RcvCdmaModemSetCnf },
    { ID_MTA_AT_CDMA_CAP_RESUME_SET_CNF, AT_RcvCdmaCapResumeSetCnf },
#endif
    { ID_MTA_AT_NV_REFRESH_SET_CNF, AT_RcvMtaNvRefreshSetCnf },

    { ID_MTA_AT_PSEUDBTS_SET_CNF, AT_RcvMtaPseudBtsSetCnf },

    { ID_MTA_AT_SUBCLFSPARAM_SET_CNF, AT_RcvMtaSubClfSparamSetCnf },
    { ID_MTA_AT_SUBCLFSPARAM_QRY_CNF, AT_RcvMtaSubClfSparamQryCnf },

    { ID_MTA_AT_PSEUD_BTS_IDENT_IND, AT_RcvMtaPseudBtsIdentInd },

    { ID_MTA_AT_FORCESYNC_SET_CNF, AT_RcvMtaForceSyncSetCnf },
    { ID_AT_MTA_SET_LTEPWRDISS_CNF, AT_RcvMtaAtLtePwrDissSetCnf },

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_AT_MTA_SET_LTESARSTUB_CNF, AT_RcvMtaLteSarStubSetCnf },
#endif

    { ID_MTA_AT_SFEATURE_QRY_CNF, AT_RcvMtaQrySfeatureRsp },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_MTA_AT_LOW_PWR_MODE_SET_CNF, AT_RcvMtaLowPwrModeSetCnf },
#endif
    { ID_MTA_AT_TXPOWER_QRY_CNF, AT_RcvMtaTxPowerQryCnf },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_NTXPOWER_QRY_CNF, AT_RcvMtaNtxPowerQryCnf },
    { ID_MTA_AT_5G_NSSAI_SET_CNF, AT_RcvMta5gNssaiSetCnf },
    { ID_MTA_AT_5G_NSSAI_QRY_CNF, AT_RcvMta5gNssaiQryCnf },
    { ID_MTA_AT_SET_LADN_RPT_CNF, AT_RcvMtaLadnRptSetCnf },
    { ID_MTA_AT_QRY_LADN_INFO_CNF, AT_RcvMtaLadnInfoQryCnf },
    { ID_MTA_AT_LADN_INFO_IND, AT_RcvMtaLadnInfoInd },
    { ID_MTA_AT_PENDING_NSSAI_IND, AT_RcvMtaPendingNssaiInd },
#endif
    { ID_MTA_AT_MCS_SET_CNF, AT_RcvMtaMcsSetCnf },
    { ID_MTA_AT_HFREQINFO_QRY_CNF, AT_RcvMtaHfreqinfoQryCnf },
    { ID_MTA_AT_HFREQINFO_IND, AT_RcvMtaHfreqinfoInd },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_MTA_AT_FAST_RETURN_5G_ENDC_IND, AT_RcvMtaFastReturn5gEndcInd },
    { ID_MTA_AT_5G_OPTION_SET_CNF, AT_RcvMta5gOptionSetCnf },
    { ID_MTA_AT_5G_OPTION_QRY_CNF, AT_RcvMta5gOptionQryCnf },
#endif
#if (FEATURE_LTEV == FEATURE_ON)
    { ID_TAF_MTA_VMODE_SET_CNF, AT_RcvMtaVModeSetCnf },
    { ID_TAF_MTA_VMODE_QRY_CNF, AT_RcvMtaVModeQryCnf },
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { ID_MTA_AT_FILE_WRITE_CNF, AT_RcvMtaFileWrCnf },
    { ID_MTA_AT_FILE_READ_CNF, AT_RcvMtaFileRdCnf },
#endif
#endif

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_MTA_AT_UART_TEST_CNF, AT_RcvMtaUartTestCnf },
    { ID_MTA_AT_UART_TEST_RSLT_IND, AT_RcvMtaUartTestRsltInd },
#endif

#if ((FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON))
    { ID_MTA_AT_HSRCELLINFO_IND, AT_RcvMtaHsrcellInfoInd },
#endif /* (FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON) */
    { ID_MTA_AT_NV_REFRESH_NTF, AT_RcvMtaNvRefreshNtf },
    { ID_MTA_AT_QUICK_CARD_SWITCH_CNF, AT_RcvMtaQuickCardSwitchSetCnf },
};

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))
static const AT_PROC_MsgFromXpds g_atProcMsgFromXpdsTab[] = {
    { ID_XPDS_AT_GPS_START_CNF, AT_RcvXpdsCagpsRlstCnf },
    { ID_XPDS_AT_GPS_CFG_MPC_ADDR_CNF, AT_RcvXpdsCagpsRlstCnf },
    { ID_XPDS_AT_GPS_CFG_PDE_ADDR_CNF, AT_RcvXpdsCagpsRlstCnf },
    { ID_XPDS_AT_GPS_CFG_MODE_CNF, AT_RcvXpdsCagpsRlstCnf },
    { ID_XPDS_AT_GPS_STOP_CNF, AT_RcvXpdsCagpsRlstCnf },

    { ID_XPDS_AT_GPS_TIME_INFO_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_REFLOC_INFO_CNF, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_PDE_POSI_INFO_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_NI_SESSION_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_START_REQ, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_CANCEL_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_ACQ_ASSIST_DATA_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_ABORT_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_DEL_ASSIST_DATA_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_ION_INFO_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_EPH_INFO_IND, AT_RcvXpdsEphInfoInd },
    { ID_XPDS_AT_GPS_ALM_INFO_IND, AT_RcvXpdsAlmInfoInd },
    { ID_XPDS_AT_GPS_NI_CP_STOP, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_NI_CP_START, AT_RcvXpdsCagpsCnf },

    { ID_XPDS_AT_AP_DATA_CALL_REQ, AT_RcvXpdsAgpsDataCallReq },
    { ID_XPDS_AT_AP_SERVER_BIND_REQ, AT_RcvXpdsAgpsServerBindReq },
    { ID_XPDS_AT_AP_REVERSE_DATA_IND, AT_RcvXpdsAgpsReverseDataInd },

    { ID_XPDS_AT_GPS_UTS_TEST_START_REQ, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_UTS_TEST_STOP_REQ, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_UTS_GPS_POS_INFO_IND, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_OM_TEST_START_REQ, AT_RcvXpdsCagpsCnf },
    { ID_XPDS_AT_GPS_OM_TEST_STOP_REQ, AT_RcvXpdsCagpsCnf },
};
#endif

const AT_PROC_MsgFromCcm g_atProcMsgFromCcmTab[] = {
    { ID_TAF_CCM_QRY_CHANNEL_INFO_CNF, AT_RcvTafCcmCSChannelInfoQryCnf },
    { ID_TAF_CCM_CALL_CHANNEL_INFO_IND, AT_RcvTafCcmChannelInfoInd },
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_TAF_CCM_SEND_FLASH_CNF, AT_RcvTafCcmSndFlashRslt },
    { ID_TAF_CCM_SEND_BURST_DTMF_CNF, AT_RcvTafCcmSndBurstDTMFCnf },
    { ID_TAF_CCM_BURST_DTMF_IND, AT_RcvTafCcmRcvBurstDtmfInd },
    { ID_TAF_CCM_SEND_CONT_DTMF_CNF, AT_RcvTafCcmSndContinuousDTMFCnf },
    { ID_TAF_CCM_CONT_DTMF_IND, AT_RcvTafCcmRcvContinuousDtmfInd },
    { ID_TAF_CCM_CCWAC_INFO_IND, AT_RcvTafCcmCCWACInd },
    { ID_TAF_CCM_CALLED_NUM_INFO_IND, AT_RcvTafCcmCalledNumInfoInd },
    { ID_TAF_CCM_CALLING_NUM_INFO_IND, AT_RcvTafCcmCallingNumInfoInd },
    { ID_TAF_CCM_DISPLAY_INFO_IND, AT_RcvTafCcmDispInfoInd },
    { ID_TAF_CCM_EXT_DISPLAY_INFO_IND, AT_RcvTafCcmExtDispInfoInd },
    { ID_TAF_CCM_CONN_NUM_INFO_IND, AT_RcvTafCcmConnNumInfoInd },
    { ID_TAF_CCM_REDIR_NUM_INFO_IND, AT_RcvTafCcmRedirNumInfoInd },
    { ID_TAF_CCM_SIGNAL_INFO_IND, AT_RcvTafCcmSignalInfoInd },
    { ID_TAF_CCM_LINE_CTRL_INFO_IND, AT_RcvTafCcmLineCtrlInfoInd },
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON)
    { ID_TAF_CCM_ENCRYPT_VOICE_CNF, AT_RcvTafCcmEncryptCallCnf },
    { ID_TAF_CCM_ENCRYPT_VOICE_IND, AT_RcvTafCcmEncryptCallInd },
    { ID_TAF_CCM_EC_REMOTE_CTRL_IND, AT_RcvTafCcmEccRemoteCtrlInd },
    { ID_TAF_CCM_REMOTE_CTRL_ANSWER_CNF, AT_RcvTafCcmRemoteCtrlAnsCnf },
    { ID_TAF_CCM_ECC_SRV_CAP_CFG_CNF, AT_RcvTafCcmEccCapSetCnf },
    { ID_TAF_CCM_ECC_SRV_CAP_QRY_CNF, AT_RcvTafCcmEccCapQryCnf },
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
    { ID_TAF_CCM_SET_EC_TEST_MODE_CNF, AT_RcvTafCcmSetEccTestModeCnf },
    { ID_TAF_CCM_GET_EC_TEST_MODE_CNF, AT_RcvTafCcmQryEccTestModeCnf },
    { ID_TAF_CCM_GET_EC_RANDOM_CNF, AT_RcvTafCcmQryEccRandomCnf },
    { ID_TAF_CCM_GET_EC_KMC_CNF, AT_RcvTafCcmQryEccKmcCnf },
    { ID_TAF_CCM_SET_EC_KMC_CNF, AT_RcvTafCcmSetEccKmcCnf },
    { ID_TAF_CCM_ENCRYPTED_VOICE_DATA_IND, AT_RcvTafCcmEncryptedVoiceDataInd },
#endif
#endif
    { ID_TAF_CCM_PRIVACY_MODE_SET_CNF, AT_RcvTafCcmPrivacyModeSetCnf },
    { ID_TAF_CCM_PRIVACY_MODE_QRY_CNF, AT_RcvTafCcmPrivacyModeQryCnf },
    { ID_TAF_CCM_PRIVACY_MODE_IND, AT_RcvTafCcmPrivacyModeInd },

#endif
#if (FEATURE_IMS == FEATURE_ON)
    { ID_TAF_CCM_CALL_MODIFY_CNF, At_RcvTafCcmCallModifyCnf },
    { ID_TAF_CCM_CALL_ANSWER_REMOTE_MODIFY_CNF, At_RcvTafCcmCallAnswerRemoteModifyCnf },
    { ID_TAF_CCM_QRY_ECONF_CALLED_INFO_CNF, AT_RcvTafCcmQryEconfCalledInfoCnf },
    { ID_TAF_CCM_CALL_MODIFY_STATUS_IND, AT_RcvTafCcmCallModifyStatusInd },
    { ID_TAF_CCM_CCWAI_SET_CNF, AT_RcvTafCcmCcwaiSetCnf },
    { ID_TAF_CCM_CANCEL_ADD_VIDEO_CNF, AT_RcvTafCcmCancelAddVideoCnf },
#endif
    { ID_TAF_CCM_QRY_XLEMA_CNF, AT_RcvTafCcmQryXlemaCnf },
    { ID_TAF_CCM_QRY_CALL_INFO_CNF, AT_RcvTafCcmQryCallInfoCnf },
    { ID_TAF_CCM_SET_ALS_CNF, AT_RcvTafCcmSetAlsCnf },
    { ID_TAF_CCM_SET_UUS1_INFO_CNF, AT_RcvTafCcmSetUus1InfoCnf },
    { ID_TAF_CCM_QRY_UUS1_INFO_CNF, AT_RcvTafCcmQryUus1InfoCnf },
    { ID_TAF_CCM_QRY_ALS_CNF, AT_RcvTafCcmQryAlsCnf },
    { ID_TAF_CCM_ECC_NUM_IND, AT_RcvTafCcmEccNumInd },
    { ID_TAF_CCM_QRY_CLPR_CNF, AT_RcvTafCcmQryClprCnf },
    { ID_TAF_CCM_SET_CSSN_CNF, AT_RcvTafCcmSetCssnCnf },
    { ID_TAF_CCM_CALL_ORIG_IND, AT_RcvTafCcmCallOrigInd },
    { ID_TAF_CCM_CALL_ORIG_CNF, At_RcvTafCcmCallOrigCnf },
    { ID_TAF_CCM_CALL_SUPS_CMD_CNF, At_RcvTafCcmCallSupsCmdCnf },
    { ID_TAF_CCM_CALL_SUPS_CMD_RSLT_IND, AT_RcvTafCcmCallSupsCmdRsltInd },
    { ID_TAF_CCM_CALL_CONNECT_IND, AT_RcvTafCcmCallConnectInd },
    { ID_TAF_CCM_CALL_INCOMING_IND, At_RcvTafCcmCallIncomingInd },
    { ID_TAF_CCM_CALL_RELEASED_IND, AT_RcvTafCcmCallReleaseInd },
    { ID_TAF_CCM_CALL_ALL_RELEASED_IND, AT_RcvTafCcmCallAllReleasedInd },
    { ID_TAF_CCM_START_DTMF_CNF, AT_RcvTafCcmStartDtmfCnf },
    { ID_TAF_CCM_STOP_DTMF_CNF, AT_RcvTafCcmStopDtmfCnf },
    { ID_TAF_CCM_CALL_PROC_IND, AT_RcvTafCcmCallProcInd },
    { ID_TAF_CCM_CALL_ALERTING_IND, AT_RcvTafCcmCallAlertingInd },
    { ID_TAF_CCM_CALL_HOLD_IND, AT_RcvTafCcmCallHoldInd },
    { ID_TAF_CCM_CALL_RETRIEVE_IND, AT_RcvTafCcmCallRetrieveInd },
    { ID_TAF_CCM_CALL_SS_IND, AT_RcvTafCcmCallSsInd },
    { ID_TAF_CCM_GET_CDUR_CNF, AT_RcvTafCcmGetCdurCnf },
    { ID_TAF_CCM_UUS1_INFO_IND, AT_RcvTafCcmUus1InfoInd },
    { ID_TAF_CCM_CNAP_INFO_IND, AT_RcvTafCcmCnapInfoInd },
    { ID_TAF_CCM_CNAP_QRY_CNF, AT_RcvTafCcmCnapQryCnf },
#if (FEATURE_IMS == FEATURE_ON)
    { ID_TAF_CCM_ECONF_DIAL_CNF, AT_RcvTafCcmEconfDialCnf },
    { ID_TAF_CCM_ECONF_NOTIFY_IND, AT_RcvTafCcmEconfNotifyInd },
#endif
};

static const AT_PROC_MsgFromMma g_atProcMsgFromMmaTab[] = {
/* 消息ID */ /* 消息处理函数 */
    { ID_TAF_MMA_USIM_STATUS_IND, AT_RcvAtMmaUsimStatusInd },
    { ID_TAF_MMA_CMM_SET_CNF, AT_RcvMmaCmmSetCmdRsp },
    { ID_TAF_MMA_SIMLOCK_STAUS_QUERY_CNF, AT_RcvSimLockQryRsp },
    { ID_TAF_MMA_AC_INFO_QRY_CNF, AT_RcvMmaAcInfoQueryCnf },
    { ID_TAF_MMA_AC_INFO_CHANGE_IND, AT_RcvMmaAcInfoChangeInd },
    { ID_TAF_MMA_EOPLMN_SET_CNF, AT_RcvMmaEOPlmnSetCnf },
    { ID_TAF_MMA_NET_SCAN_CNF, AT_RcvMmaNetScanCnf },
    { ID_TAF_MMA_USER_SRV_STATE_QRY_CNF, AT_RcvMmaUserSrvStateQryCnf },
    { ID_TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF, AT_RcvMmaPwrOnAndRegTimeQryCnf },
    { ID_TAF_MMA_SPN_QRY_CNF, AT_RcvMmaSpnQryCnf },
    { ID_TAF_MMA_MMPLMNINFO_QRY_CNF, AT_RcvMmaMMPlmnInfoQryCnf },
    { ID_TAF_MMA_LAST_CAMP_PLMN_QRY_CNF, AT_RcvMmaPlmnQryCnf },
    { ID_TAF_MMA_EOPLMN_QRY_CNF, AT_RcvMmaEOPlmnQryCnf },
    { ID_TAF_MMA_COPN_INFO_QRY_CNF, AT_RcvMmaCopnInfoQueryCnf },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_TAF_MMA_SIM_INSERT_CNF, AT_RcvMmaSimInsertRsp },
#endif
    { ID_TAF_MMA_SYS_CFG_SET_CNF, AT_RcvMmaSysCfgSetCnf },
    { ID_TAF_MMA_PHONE_MODE_SET_CNF, AT_RcvMmaPhoneModeSetCnf },
    { ID_TAF_MMA_DETACH_CNF, AT_RcvMmaDetachCnf },
    { ID_TAF_MMA_ATTACH_CNF, AT_RcvMmaAttachCnf },
    { ID_TAF_MMA_ATTACH_STATUS_QRY_CNF, AT_RcvMmaAttachStatusQryCnf },

    { ID_TAF_MMA_EMC_CALL_BACK_NTF, AT_RcvMmaEmcCallBackNtf },

    { ID_TAF_MMA_EMC_CALL_BACK_QRY_CNF, AT_RcvMmaQryEmcCallBackCnf },
    { ID_TAF_MMA_QUIT_CALLBACK_SET_CNF, AT_RcvMmaQuitCallBackCnf },

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_TAF_MMA_CL_DBDOMAIN_STATUSE_SET_CNF, AT_RcvMmaClDbDomainStatusSetCnf },
    { ID_TAF_MMA_CL_DBDOMAIN_STATUSE_INFO_IND, AT_RcvMmaClDbdomainStatusReportInd },
    { ID_TAF_MMA_CDMA_LOCINFO_QRY_CNF, AT_RcvMmaCLocInfoQueryCnf },
    { ID_TAF_MMA_CSID_IND, AT_RcvMmaCSidInd },

/* 移出CDMA编译开关 */
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_TAF_MMA_CSIDLIST_SET_CNF, AT_RcvMmaSetCSidListCnf },
#endif
    /* 移出CDMA编译开关 */
    { ID_TAF_MMA_CURR_SID_NID_QRY_CNF, AT_RcvMmaQryCurrSidNidCnf },

    { ID_TAF_MMA_CLOCINFO_IND, AT_RcvMmaClocinfoInd },
    { ID_TAF_MMA_ROAMING_MODE_SWITCH_IND, AT_RcvMmaRoamingModeSwitchInd },
    { ID_TAF_MMA_CTCC_ROAMING_NW_INFO_QRY_CNF, AT_RcvMmaCtRoamInfoCnf },
    { ID_TAF_MMA_CTCC_ROAMING_NW_INFO_REPORT_IND, AT_RcvMmaCtRoamingInfoChgInd },
    { ID_TAF_MMA_CTCC_OOS_COUNT_SET_CNF, AT_RcvMmaCtOosCountCnf },
    { ID_TAF_MMA_CTCC_ROAMING_NW_INFO_RTP_CFG_SET_CNF, AT_RcvMmaCtRoamRptCfgSetCnf },
    { ID_TAF_MMA_PRLID_QRY_CNF, AT_RcvMmaPrlIdQryCnf },

    { ID_TAF_MMA_COMBINED_MODE_SWITCH_IND, AT_RcvMmaCombinedModeSwitchInd },

    { ID_TAF_MMA_ICC_APP_TYPE_SWITCH_IND, AT_RcvMmaIccAppTypeSwitchInd },
    { ID_TAF_MMA_RAT_COMBINED_MODE_QRY_CNF, AT_RcvMmaRatCombinedModeQryCnf },
    { ID_TAF_MMA_CTIME_IND, AT_RcvMmaCTimeInd },

    { ID_TAF_MMA_CDMA_FREQ_LOCK_SET_CNF, AT_RcvMmaCFreqLockSetCnf },
    { ID_TAF_MMA_CDMA_FREQ_LOCK_QRY_CNF, AT_RcvMmaCFreqLockQueryCnf },

    { ID_TAF_MMA_CDMACSQ_SET_CNF, AT_RcvMmaCdmaCsqSetCnf },
    { ID_TAF_MMA_CDMACSQ_QRY_CNF, AT_RcvMmaCdmaCsqQryCnf },
    { ID_TAF_MMA_CDMACSQ_IND, AT_RcvMmaCdmaCsqInd },

    { ID_MMA_TAF_HDR_CSQ_VALUE_IND, AT_RcvMmaHdrCsqInd },
    { ID_MMA_TAF_HDR_CSQ_SET_CNF, AT_RcvMmaHdrCsqSetCnf },
    { ID_MMA_TAF_HDR_CSQ_QRY_SETTING_CNF, AT_RcvMmaHdrCsqQryCnf },
#endif

    { ID_TAF_MMA_CFPLMN_SET_CNF, AT_RcvMmaCFPlmnSetCnf },
    { ID_TAF_MMA_CFPLMN_QUERY_CNF, AT_RcvMmaCFPlmnQueryCnf },

    { ID_TAF_MMA_PREF_PLMN_SET_CNF, AT_RcvMmaPrefPlmnSetCnf },
    { ID_TAF_MMA_PREF_PLMN_QUERY_CNF, AT_RcvMmaPrefPlmnQueryCnf },
    { ID_TAF_MMA_PREF_PLMN_TEST_CNF, AT_RcvMmaPrefPlmnTestCnf },

    { ID_TAF_MMA_PLMN_SEARCH_CNF, AT_RcvMmaPlmnSearchCnf },
    { ID_TAF_MMA_PLMN_LIST_ABORT_CNF, AT_RcvMmaPlmnListAbortCnf },
    { ID_TAF_MMA_LOCATION_INFO_QRY_CNF, AT_RcvMmaLocInfoQueryCnf },
    { ID_TAF_MMA_CIPHER_QRY_CNF, AT_RcvMmaCipherInfoQueryCnf },
    { ID_TAF_MMA_PREF_PLMN_TYPE_SET_CNF, AT_RcvMmaPrefPlmnTypeSetCnf },
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { ID_TAF_MMA_MT_POWER_DOWN_CNF, AT_RcvMmaPowerDownCnf },
#endif

    { ID_TAF_MMA_PHONE_MODE_QRY_CNF, AT_RcvMmaPhoneModeQryCnf },
    { ID_TAF_MMA_TIME_CHANGE_IND, AT_RcvMmaTimeChangeInd },
    { ID_TAF_MMA_MODE_CHANGE_IND, AT_RcvMmaModeChangeInd },
    { ID_TAF_MMA_PLMN_CHANGE_IND, AT_RcvMmaPlmnChangeInd },

    { ID_TAF_MMA_CERSSI_SET_CNF, AT_RcvMmaCerssiSetCnf },
    { ID_TAF_MMA_CERSSI_QRY_CNF, AT_RcvMmaCerssiInfoQueryCnf },
    { ID_TAF_MMA_ACCESS_MODE_QRY_CNF, AT_RcvMmaAccessModeQryCnf },
    { ID_TAF_MMA_COPS_QRY_CNF, AT_RcvMmaCopsQryCnf },
    { ID_TAF_MMA_REG_STATE_QRY_CNF, AT_RcvMmaRegStateQryCnf },
    { ID_TAF_MMA_AUTO_ATTACH_QRY_CNF, AT_RcvMmaAutoAttachQryCnf },
    { ID_TAF_MMA_SYSINFO_QRY_CNF, AT_RcvMmaSysInfoQryCnf },
    /* ANTENNA_INFO_QRY_CNF 移到MTA模块处理 */
    { ID_TAF_MMA_HOME_PLMN_QRY_CNF, AT_RcvMmaEHplmnInfoQryCnf },

    { ID_TAF_MMA_SRV_STATUS_IND, AT_RcvMmaSrvStatusInd },
    { ID_TAF_MMA_RSSI_INFO_IND, AT_RcvMmaRssiInfoInd },
    { ID_TAF_MMA_REG_STATUS_IND, AT_RcvMmaRegStatusInd },
    { ID_TAF_MMA_REG_REJ_INFO_IND, AT_RcvMmaRegRejInfoInd },
    { ID_TAF_MMA_PLMN_SELECTION_INFO_IND, AT_RcvMmaPlmnSelectInfoInd },

    { ID_TAF_MMA_QUICKSTART_SET_CNF, AT_RcvTafMmaQuickStartSetCnf },
    { ID_TAF_MMA_AUTO_ATTACH_SET_CNF, AT_RcvTafMmaAutoAttachSetCnf },
    { ID_TAF_MMA_SYSCFG_QRY_CNF, AT_RcvTafMmaSyscfgQryCnf },
    { ID_TAF_MMA_SYSCFG_TEST_CNF, AT_RcvTafMmaSyscfgTestCnf },
    { ID_TAF_MMA_CRPN_QRY_CNF, AT_RcvTafMmaCrpnQryCnf },
    { ID_TAF_MMA_QUICKSTART_QRY_CNF, AT_RcvTafMmaQuickStartQryCnf },
    /* CSNR_QRY_CNF移到MTA模块处理 */
    { ID_TAF_MMA_CSQ_QRY_CNF, AT_RcvTafMmaCsqQryCnf },
    /* CSQLVL_QRY_CNF移到MTA模块处理 */

    { ID_TAF_MMA_BATTERY_CAPACITY_QRY_CNF, AT_RcvMmaCbcQryCnf },
    { ID_TAF_MMA_HAND_SHAKE_QRY_CNF, AT_RcvMmaHsQryCnf },
    { ID_TAF_MMA_PLMN_LIST_CNF, At_QryParaPlmnListProc },
    { ID_TAF_MSG_MMA_EONS_UCS2_CNF, AT_RcvMmaEonsUcs2Cnf },

    { ID_TAF_MMA_PLMN_DETECT_IND, At_PlmnDetectIndProc },

#if (FEATURE_CSG == FEATURE_ON)
    { ID_TAF_MMA_CSG_LIST_SEARCH_CNF, AT_RcvMmaCsgListSearchCnfProc },
    { ID_TAF_MMA_CSG_LIST_ABORT_CNF, AT_RcvMmaCsgListAbortCnf },
    { ID_TAF_MMA_CSG_SPEC_SEARCH_CNF, AT_RcvMmaCsgSpecSearchCnfProc },
    { ID_TAF_MMA_QRY_CAMP_CSG_ID_INFO_CNF, AT_RcvMmaQryCampCsgIdInfoCnfProc },
#endif
#if (FEATURE_IMS == FEATURE_ON)
    { ID_TAF_MMA_IMS_SWITCH_SET_CNF, AT_RcvMmaImsSwitchSetCnf },
    { ID_TAF_MMA_IMS_SWITCH_QRY_CNF, AT_RcvMmaImsSwitchQryCnf },
    { ID_TAF_MMA_VOICE_DOMAIN_SET_CNF, AT_RcvMmaVoiceDomainSetCnf },
    { ID_TAF_MMA_VOICE_DOMAIN_QRY_CNF, AT_RcvMmaVoiceDomainQryCnf },

    { ID_TAF_MMA_IMS_DOMAIN_CFG_SET_CNF, AT_RcvMmaImsDomainCfgSetCnf },
    { ID_TAF_MMA_IMS_DOMAIN_CFG_QRY_CNF, AT_RcvMmaImsDomainCfgQryCnf },

    { ID_TAF_MMA_ROAM_IMS_SUPPORT_SET_CNF, AT_RcvMmaRoamImsSupportSetCnf },

    { ID_TAF_MMA_IMS_VIDEO_CALL_CAP_SET_CNF, AT_RcvMmaImsVideoCallCapSetCnf },

    { ID_TAF_MMA_IMS_SMS_CFG_QRY_CNF, AT_RcvMmaImsSmsCfgQryCnf },
    { ID_TAF_MMA_IMS_SMS_CFG_SET_CNF, AT_RcvMmaImsSmsCfgSetCnf },

    { ID_TAF_MMA_RCS_SWITCH_QRY_CNF, AT_RcvMmaRcsSwitchQryCnf },
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_TAF_MMA_CLIMS_CFG_INFO_IND, AT_RcvMmaCLImsCfgInfoInd },
#endif
#endif
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_TAF_MMA_1XCHAN_SET_CNF, AT_RcvMma1xChanSetCnf },
    { ID_TAF_MMA_1XCHAN_QUERY_CNF, AT_RcvMma1xChanQueryCnf },
    { ID_TAF_MMA_CVER_QUERY_CNF, AT_RcvMmaCVerQueryCnf },
    { ID_TAF_MMA_GETSTA_QUERY_CNF, AT_RcvMmaStateQueryCnf },
    { ID_TAF_MMA_CHIGHVER_QUERY_CNF, AT_RcvMmaCHverQueryCnf },
#endif
    { ID_TAF_MMA_SRCHED_PLMN_INFO_IND, AT_RcvMmaSrchedPlmnInfoInd },

    { ID_TAF_MMA_DPLMN_SET_CNF, AT_RcvMmaDplmnSetCnf },
    { ID_TAF_MMA_DPLMN_QRY_CNF, AT_RcvMmaDplmnQryCnf },
    { ID_TAF_MMA_BORDER_INFO_SET_CNF, AT_RcvMmaBorderInfoSetCnf },
    { ID_TAF_MMA_BORDER_INFO_QRY_CNF, AT_RcvMmaBorderInfoQryCnf },

    { ID_TAF_MMA_EXCHANGE_MODEM_INFO_CNF, AT_RcvMmaExchangeModemInfoCnf },

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { ID_TAF_MMA_CLMODE_IND, AT_RcvMmaCLModInd },

#endif
    { ID_TAF_MMA_INIT_LOC_INFO_IND, AT_RcvMmaInitLocInfoInd },

    { ID_TAF_MMA_EFLOCIINFO_SET_CNF, AT_RcvMmaEflociInfoSetCnf },
    { ID_TAF_MMA_EFLOCIINFO_QRY_CNF, AT_RcvMmaEflociInfoQryCnf },
    { ID_TAF_MMA_EFPSLOCIINFO_SET_CNF, AT_RcvMmaPsEflociInfoSetCnf },
    { ID_TAF_MMA_EFPSLOCIINFO_QRY_CNF, AT_RcvMmaPsEflociInfoQryCnf },

    { ID_TAF_MMA_IMSI_REFRESH_IND, AT_RcvMmaImsiRefreshInd },

    { ID_TAF_MMA_PACSP_QRY_CNF, AT_RcvMmaPacspQryCnf },

    { ID_TAF_MMA_MT_REATTACH_IND, AT_RcvMmaMtReattachInd },

    { ID_TAF_MMA_CIND_SET_CNF, AT_RcvMmaCindSetCnf },
#if (FEATURE_DSDS == FEATURE_ON)
    { ID_TAF_MMA_DSDS_STATE_SET_CNF, AT_RcvMmaDsdsStateSetCnf },
    { ID_TAF_MMA_DSDS_STATE_NOTIFY, AT_RcvMmaDsdsStateNotify },
#endif

    { ID_TAF_MMA_CEMODE_SET_CNF, AT_RcvMmaCemodeSetCnf },
    { ID_TAF_MMA_CEMODE_QRY_CNF, AT_RcvMmaCemodeQryCnf },

    { ID_TAF_MMA_REJINFO_QRY_CNF, AT_RcvMmaRejinfoQryCnf },

    { ID_TAF_MMA_EMRSSICFG_SET_CNF, AT_RcvMmaEmRssiCfgSetCnf },
    { ID_TAF_MMA_EMRSSICFG_QRY_CNF, AT_RcvMmaEmRssiCfgQryCnf },
    { ID_TAF_MMA_EMRSSIRPT_SET_CNF, AT_RcvMmaEmRssiRptSetCnf },
    { ID_TAF_MMA_EMRSSIRPT_QRY_CNF, AT_RcvMmaEmRssiRptQryCnf },
    { ID_TAF_MMA_EMRSSIRPT_IND, AT_RcvMmaEmRssiRptInd },

    { ID_TAF_MMA_ELEVATOR_STATE_IND, AT_RcvMmaElevatorStateInd },

    { ID_TAF_MMA_ULFREQRPT_SET_CNF, AT_RcvMmaUlFreqRptSetCnf },
    { ID_TAF_MMA_ULFREQ_CHANGE_IND, AT_RcvMmaUlFreqChangeInd },
    { ID_TAF_MMA_ULFREQRPT_QRY_CNF, AT_RcvMmaUlFreqRptQryCnf },

    { ID_TAF_MMA_SMS_ANTI_ATTACK_CAP_QRY_CNF, AT_RcvSmsAntiAttackCapQryCnf },
    { ID_TAF_MMA_SMS_ANTI_ATTACK_SET_CNF, AT_RcvMmaSmsAntiAttackSetCnf },
    { ID_TAF_MMA_NAS_SMS_CAP_QRY_CNF, AT_RcvMmaSmsNasCapQryCnf },
    { ID_TAF_MMA_PSEUD_BTS_IDENT_IND, AT_RcvMmaPseudBtsIdentInd },

    { ID_TAF_MMA_PS_SCENE_SET_CNF, AT_RcvMmaPsSceneSetCnf },
    { ID_TAF_MMA_PS_SCENE_QRY_CNF, AT_RcvMmaPsSceneQryCnf },

    { ID_TAF_MMA_RRCSTAT_QRY_CNF, AT_RcvMmaRrcStatQryCnf },
    { ID_TAF_MMA_RRCSTAT_IND, AT_RcvMmaRrcStatInd },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_TAF_MMA_NR_BAND_BLOCK_LIST_SET_CNF, AT_RcvMmaNrBandBlockListSetCnf },
    { ID_TAF_MMA_NR_BAND_BLOCK_LIST_QRY_CNF, AT_RcvMmaNrBandBlockListQryCnf },
#endif
};

#if ((FEATURE_LTEV == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
AT_VNAS_ProcMsgTbl g_atProcMsgFromVnasTbl[] = {
    { ID_VNAS_AT_LTEV_SOURCE_ID_SET_CNF, AT_RcvVnasSrcidSetCnf },
    { ID_VNAS_AT_LTEV_SOURCE_ID_QRY_CNF, AT_RcvVnasSrcidQryCnf },
};
#endif

static const AT_PROC_MsgFromDms g_atProcMsgFromDmsTab[] = {
    /* 消息ID */                            /* 消息处理函数 */
    { ID_DMS_PORT_SUBSCRIP_EVENT_NOTIFY,    AT_RcvDmsSubscripEventNotify },
};

const AT_PROC_EventFromDms g_atProcEventFromDmsTab[] = {
    /* event ID */                    /* 消息处理函数 */
    { ID_DMS_EVENT_LOW_WATER_MARK, sizeof(struct DMS_PORT_SubscripEventNotify), VOS_NULL_PTR,
      AT_RcvDmsLowWaterMarkEvent },
    { ID_DMS_EVENT_NCM_CONN_BREAK, sizeof(struct DMS_PORT_SubscripEventNotify) + sizeof(struct DMS_NcmConnBreakInfo),
      VOS_NULL_PTR, AT_RcvDmsNcmConnBreakEvent },
    { ID_DMS_EVENT_USB_DISCONNECT, sizeof(struct DMS_PORT_SubscripEventNotify), VOS_NULL_PTR,
      AT_RcvDmsUsbDisconnectEvent },
    { ID_DMS_EVENT_SWITCH_GW_MODE, sizeof(struct DMS_PORT_SubscripEventNotify), VOS_NULL_PTR,
      AT_RcvDmsSwitchGwModeEvent },
    { ID_DMS_EVENT_ESCAPE_SEQUENCE, sizeof(struct DMS_PORT_SubscripEventNotify), VOS_NULL_PTR,
      AT_RcvDmsEscapeSequenceEvent },
    { ID_DMS_EVENT_DTR_DEASSERT, sizeof(struct DMS_PORT_SubscripEventNotify), VOS_NULL_PTR,
      AT_RcvDmsDtrDeassertedEvent },
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    { ID_DMS_EVENT_UART_INIT, 0, VOS_NULL_PTR, AT_RcvDmsUartInitEvent },
#endif
};

AT_MSG_Proc g_atMsgProc[] = {
    { WUEPS_PID_AT, At_CmdMsgDistr },
    { I0_WUEPS_PID_TAF, At_TafAndDmsMsgProc },
    { I1_WUEPS_PID_TAF, At_TafAndDmsMsgProc },
    { I2_WUEPS_PID_TAF, At_TafAndDmsMsgProc },
    { I0_UEPS_PID_DSM, At_TafAndDmsMsgProc },
    { I1_UEPS_PID_DSM, At_TafAndDmsMsgProc },
    { I2_UEPS_PID_DSM, At_TafAndDmsMsgProc },
    { PS_PID_APP_PPP, At_PppMsgProc },
    { PS_PID_APP_NDIS, AT_RcvNdisMsg },
    { I0_DSP_PID_WPHY, At_HPAMsgProc },
    { I1_DSP_PID_WPHY, At_HPAMsgProc },
    { I0_DSP_PID_GPHY, At_GHPAMsgProc },
    { I1_DSP_PID_GPHY, At_GHPAMsgProc },
    { I2_DSP_PID_GPHY, At_GHPAMsgProc },
    { I0_WUEPS_PID_MMA, At_MmaMsgProc },
    { I1_WUEPS_PID_MMA, At_MmaMsgProc },
    { I2_WUEPS_PID_MMA, At_MmaMsgProc },
    { I0_WUEPS_PID_DRV_AGENT, At_ProcMsgFromDrvAgent },
    { I1_WUEPS_PID_DRV_AGENT, At_ProcMsgFromDrvAgent },
    { I2_WUEPS_PID_DRV_AGENT, At_ProcMsgFromDrvAgent },
    { I0_WUEPS_PID_VC, At_ProcMsgFromVc },
    { I1_WUEPS_PID_VC, At_ProcMsgFromVc },
    { I1_WUEPS_PID_VC, At_ProcMsgFromVc },
    { ACPU_PID_RNIC, At_RcvRnicMsg },
    { I0_WUEPS_PID_CC, At_ProcMsgFromCc },
    { I1_WUEPS_PID_CC, At_ProcMsgFromCc },
    { I1_WUEPS_PID_CC, At_ProcMsgFromCc },
#if (FEATURE_LTE == FEATURE_ON)
    { I0_MSP_L4_L4A_PID, at_L4aCnfProc },
    { I1_MSP_L4_L4A_PID, at_L4aCnfProc },
    { I0_MSP_SYS_FTM_PID, At_FtmEventMsgProc },
    { I1_MSP_SYS_FTM_PID, At_FtmEventMsgProc },
#endif
    { I0_MAPS_STK_PID, At_STKMsgProc },
    { I1_MAPS_STK_PID, At_STKMsgProc },
    { I2_MAPS_STK_PID, At_STKMsgProc },
    { I0_MAPS_PB_PID, At_PbMsgProc },
    { I1_MAPS_PB_PID, At_PbMsgProc },
    { I2_MAPS_PB_PID, At_PbMsgProc },
    { I0_MAPS_PIH_PID, At_PIHMsgProc },
    { I1_MAPS_PIH_PID, At_PIHMsgProc },
    { I2_MAPS_PIH_PID, At_PIHMsgProc },
    { I0_WUEPS_PID_RABM, AT_RabmMsgProc },
    { I1_WUEPS_PID_RABM, AT_RabmMsgProc },
    { I2_WUEPS_PID_RABM, AT_RabmMsgProc },
    { WUEPS_PID_SPY, AT_SpyMsgProc },
    { I0_UEPS_PID_MTA, At_ProcMtaMsg },
    { I1_UEPS_PID_MTA, At_ProcMtaMsg },
    { I2_UEPS_PID_MTA, At_ProcMtaMsg },
#if (FEATURE_IMS == FEATURE_ON)
    { I0_PS_PID_IMSA, AT_ProcImsaMsg },
    { I1_PS_PID_IMSA, AT_ProcImsaMsg },
#endif
    { PS_PID_CSS, AT_ProcCssMsg },
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { I0_UEPS_PID_XSMS, AT_ProcXsmsMsg },
    { I1_UEPS_PID_XSMS, AT_ProcXsmsMsg },
    { I2_UEPS_PID_XSMS, AT_ProcXsmsMsg },
    { DSP_PID_PROCSTUB, At_CHPAMsgProc },
#endif
#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))
    { I0_UEPS_PID_XPDS, AT_ProcXpdsMsg },
    { I1_UEPS_PID_XPDS, AT_ProcXpdsMsg },
    { I2_UEPS_PID_XPDS, AT_ProcXpdsMsg },
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { CCPU_PID_CBT, AT_ProcCbtMsg },
    { DSP_PID_BBA_CAL, AT_ProcBbicMsg },
    { CCPU_PID_PAM_MFG, AT_ProcUeCbtMsg },
    { I0_PHY_PID_IDLE, AT_ProcDspIdleMsg },
#endif
    { I0_UEPS_PID_CCM, At_CcmMsgProc },
    { I1_UEPS_PID_CCM, At_CcmMsgProc },
    { I2_UEPS_PID_CCM, At_CcmMsgProc },
    { I0_MAPS_EMAT_PID, At_EMATMsgProc },
    { I1_MAPS_EMAT_PID, At_EMATMsgProc },
    { I2_MAPS_EMAT_PID, At_EMATMsgProc },
#if (FEATURE_LTEV == FEATURE_ON)
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { I0_PS_PID_VSYNC, AT_ProcMsgFromVsync },
#endif
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { I0_PS_PID_VNAS, AT_ProcMsgFromVnas },
#endif
    { I0_PS_PID_VERRC, AT_ProcMsgFromVrrc },
    { I0_PS_PID_VMAC_UL, AT_ProcMsgFromVmac },
    { I0_PS_PID_VTC, AT_ProcMsgFromVtc },
    { I0_PS_PID_VPDCP_UL, AT_ProcMsgFromVpdcp },
#endif
    { PS_PID_DMS, AT_ProcDmsMsg },
};

/*
 * AT码流缓存数组(包括AT命令或其它数据)。
 * 设置该缓存数组的原因:底软任务在调用At_CmdStreamPreProc接口时，某些场景下(如直接使用超级终端发送AT命令),则会
 * 出现AT码流以一个字符为单位发送到AT的消息队列中，导致AT的消息队列慢，触发单板复位。
 */
AT_DataStreamBuffer g_atDataBuff[AT_MAX_CLIENT_NUM];

/* CREG/CGREG的<CI>域以4字节上报是否使能(VDF需求) */

AT_ClientManage g_atClientTab[AT_MAX_CLIENT_NUM];

TAF_UINT32 g_pbPrintTag = VOS_FALSE;

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
static const AT_FreqlockModeTbl g_freqLockModeTbl[] = {
    { AT_FREQLOCK_MODE_TYPE_GSM, 0, "01" },
    { AT_FREQLOCK_MODE_TYPE_WCDMA, 0, "02" },
    { AT_FREQLOCK_MODE_TYPE_TDSCDMA, 0, "03" },
    { AT_FREQLOCK_MODE_TYPE_LTE, 0, "04" },
};

static const AT_FreqlockGsmBandTbl g_freqLockGsmBandTbl[] = {
    { AT_FREQLOCK_BAND_TYPE_GSM_850, 0, "00" },
    { AT_FREQLOCK_BAND_TYPE_GSM_900, 0, "01" },
    { AT_FREQLOCK_BAND_TYPE_GSM_1800, 0, "02" },
    { AT_FREQLOCK_BAND_TYPE_GSM_1900, 0, "03" },
};
#endif /* FEATURE_PROBE_FREQLOCK == FEATURE_ON */

typedef struct {
    VOS_UINT16 mcc;
    VOS_INT8   zone;
    VOS_UINT8  reserved;
} MCC_ZONE_INFO_STRU;

static const MCC_ZONE_INFO_STRU g_mccZoneInfoTbl[] = {
    { 0x202, +30 * 4 / 10, 0 },  /* Greece */
    { 0x204, +10 * 4 / 10, 0 },  /* Greece */
    { 0x206, +10 * 4 / 10, 0 },  /* Holland */
    { 0x208, +10 * 4 / 10, 0 },  /* France */
    { 0x212, +10 * 4 / 10, 0 },  /* Monaco */
    { 0x213, +10 * 4 / 10, 0 },  /* Andorra */
    { 0x214, +10 * 4 / 10, 0 },  /* Spain */
    { 0x216, +10 * 4 / 10, 0 },  /* Hungary */
    { 0x218, +10 * 4 / 10, 0 },  /* Bosnia and Herzegovina, the Republic of */
    { 0x219, +10 * 4 / 10, 0 },  /* Croatia */
    { 0x220, +10 * 4 / 10, 0 },  /* Montenegro / Monte Nige Lu / Serbia */
    { 0x222, +10 * 4 / 10, 0 },  /* Italy */
    { 0x226, +20 * 4 / 10, 0 },  /* Romania */
    { 0x228, +10 * 4 / 10, 0 },  /* Switzerland */
    { 0x230, +10 * 4 / 10, 0 },  /* Czech */
    { 0x231, +10 * 4 / 10, 0 },  /* Slovakia */
    { 0x232, +10 * 4 / 10, 0 },  /* Austria */
    { 0x234, +10 * 4 / 10, 0 },  /* Guernsey */
    { 0x238, +10 * 4 / 10, 0 },  /* Denmark */
    { 0x240, +10 * 4 / 10, 0 },  /* Sweden */
    { 0x242, +10 * 4 / 10, 0 },  /* Norway */
    { 0x244, +20 * 4 / 10, 0 },  /* Finland */
    { 0x246, +20 * 4 / 10, 0 },  /* Lithuania */
    { 0x247, +20 * 4 / 10, 0 },  /* Latvia */
    { 0x248, +20 * 4 / 10, 0 },  /* Estonia */
    { 0x250, +80 * 4 / 10, 0 },  /* Russia GMT+3 ~ GMT+12 */
    { 0x255, +20 * 4 / 10, 0 },  /* Ukraine */
    { 0x257, +20 * 4 / 10, 0 },  /* Belarus */
    { 0x259, +20 * 4 / 10, 0 },  /* Moldova */
    { 0x260, +10 * 4 / 10, 0 },  /* Poland */
    { 0x262, +10 * 4 / 10, 0 },  /* Germany */
    { 0x266, +10 * 4 / 10, 0 },  /* Gibraltar */
    { 0x268, +0 * 4 / 10, 0 },   /* Portugal */
    { 0x270, +10 * 4 / 10, 0 },  /* Luxembourg */
    { 0x272, +0 * 4 / 10, 0 },   /* Ireland  GMT+0 ~ GMT+1 */
    { 0x274, +0 * 4 / 10, 0 },   /* Iceland  */
    { 0x276, +10 * 4 / 10, 0 },  /* Albania */
    { 0x278, +10 * 4 / 10, 0 },  /* Malta */
    { 0x280, +20 * 4 / 10, 0 },  /* Cyprus */
    { 0x282, +40 * 4 / 10, 0 },  /* Georgia */
    { 0x283, +40 * 4 / 10, 0 },  /* Armenia */
    { 0x284, +20 * 4 / 10, 0 },  /* Bulgaria */
    { 0x286, +20 * 4 / 10, 0 },  /* Turkey */
    { 0x288, +0 * 4 / 10, 0 },   /* Faroe Islands */
    { 0x290, -20 * 4 / 10, 0 },  /* Greenland GMT+0 ~ GMT-4 */
    { 0x292, +10 * 4 / 10, 0 },  /* San Marino */
    { 0x293, +10 * 4 / 10, 0 },  /* Slovenia */
    { 0x294, +10 * 4 / 10, 0 },  /* Macedonia */
    { 0x295, +10 * 4 / 10, 0 },  /* Liechtenstein */
    { 0x302, -40 * 4 / 10, 0 },  /* Canada */
    { 0x308, -30 * 4 / 10, 0 },  /* Saint-Pierre and Miquelon */
    { 0x310, -80 * 4 / 10, 0 },  /* America */
    { 0x311, -80 * 4 / 10, 0 },  /* America */
    { 0x330, -40 * 4 / 10, 0 },  /* Puerto Rico */
    { 0x334, -70 * 4 / 10, 0 },  /* Mexico */
    { 0x338, -50 * 4 / 10, 0 },  /* Jamaica */
    { 0x340, -40 * 4 / 10, 0 },  /* Martinique */
    { 0x342, -40 * 4 / 10, 0 },  /* Barbados */
    { 0x344, -40 * 4 / 10, 0 },  /* Antigua and Barbuda */
    { 0x346, -50 * 4 / 10, 0 },  /* Cayman Islands */
    { 0x348, -40 * 4 / 10, 0 },  /* The British Virgin Islands */
    { 0x350, -30 * 4 / 10, 0 },  /* Bermuda */
    { 0x352, -40 * 4 / 10, 0 },  /* Grenada */
    { 0x354, -40 * 4 / 10, 0 },  /* Montserrat */
    { 0x356, -40 * 4 / 10, 0 },  /* Saint Kitts and Nevis */
    { 0x358, -40 * 4 / 10, 0 },  /* St. Lucia */
    { 0x360, -40 * 4 / 10, 0 },  /* Saint Vincent and the Grenadines */
    { 0x362, -40 * 4 / 10, 0 },  /* Netherlands Antilles */
    { 0x363, -40 * 4 / 10, 0 },  /* Aruba */
    { 0x364, -50 * 4 / 10, 0 },  /* Bahamas */
    { 0x365, -40 * 4 / 10, 0 },  /* Anguilla */
    { 0x366, -50 * 4 / 10, 0 },  /* Dominican */
    { 0x368, -50 * 4 / 10, 0 },  /* Cuba */
    { 0x370, -50 * 4 / 10, 0 },  /* Dominican Republic */
    { 0x372, -50 * 4 / 10, 0 },  /* Haiti */
    { 0x374, -40 * 4 / 10, 0 },  /* The Republic of Trinidad and Tobago */
    { 0x376, -50 * 4 / 10, 0 },  /* Turks and Caicos Islands */
    { 0x400, +40 * 4 / 10, 0 },  /* Republic of Azerbaijan */
    { 0x401, +40 * 4 / 10, 0 },  /* Kazakhstan */
    { 0x402, +60 * 4 / 10, 0 },  /* Bhutan */
    { 0x404, +55 * 4 / 10, 0 },  /* India */
    { 0x405, +55 * 4 / 10, 0 },  /* India */
    { 0x410, +50 * 4 / 10, 0 },  /* Pakistan */
    { 0x412, +45 * 4 / 10, 0 },  /* Afghanistan */
    { 0x413, +55 * 4 / 10, 0 },  /* Sri Lanka */
    { 0x414, +65 * 4 / 10, 0 },  /* Myanmar */
    { 0x415, +30 * 4 / 10, 0 },  /* Lebanon */
    { 0x416, +20 * 4 / 10, 0 },  /* Jordan */
    { 0x417, +20 * 4 / 10, 0 },  /* Syria */
    { 0x418, +30 * 4 / 10, 0 },  /* Iraq */
    { 0x419, +30 * 4 / 10, 0 },  /* Kuwait */
    { 0x420, +30 * 4 / 10, 0 },  /* Saudi Arabia */
    { 0x421, +30 * 4 / 10, 0 },  /* Yemen */
    { 0x422, +40 * 4 / 10, 0 },  /* Oman */
    { 0x424, +40 * 4 / 10, 0 },  /* United Arab Emirates */
    { 0x425, +20 * 4 / 10, 0 },  /* Israel */
    { 0x426, +30 * 4 / 10, 0 },  /* Bahrain  */
    { 0x427, +30 * 4 / 10, 0 },  /* Qatar */
    { 0x428, +80 * 4 / 10, 0 },  /* Mongolia */
    { 0x429, +58 * 4 / 10, 0 },  /* Nepal */
    { 0x432, +35 * 4 / 10, 0 },  /* Iran */
    { 0x434, +50 * 4 / 10, 0 },  /* Uzbekistan */
    { 0x436, +50 * 4 / 10, 0 },  /* Tajikistan */
    { 0x437, +60 * 4 / 10, 0 },  /* Kyrgyzstan */
    { 0x438, +50 * 4 / 10, 0 },  /* Turkmenistan */
    { 0x440, +90 * 4 / 10, 0 },  /* Japan */
    { 0x450, +90 * 4 / 10, 0 },  /* South Korea */
    { 0x452, +70 * 4 / 10, 0 },  /* Vietnam */
    { 0x454, +80 * 4 / 10, 0 },  /* Hong Kong */
    { 0x455, +80 * 4 / 10, 0 },  /* Macau */
    { 0x456, +70 * 4 / 10, 0 },  /* Cambodia */
    { 0x457, +70 * 4 / 10, 0 },  /* Laos */
    { 0x460, +80 * 4 / 10, 0 },  /* China */
    { 0x466, +80 * 4 / 10, 0 },  /* Taiwan */
    { 0x467, +90 * 4 / 10, 0 },  /* North Korea */
    { 0x470, +60 * 4 / 10, 0 },  /* Bangladesh */
    { 0x472, +50 * 4 / 10, 0 },  /* Maldives */
    { 0x502, +80 * 4 / 10, 0 },  /*   */
    { 0x505, +90 * 4 / 10, 0 },  /*   */
    { 0x510, +80 * 4 / 10, 0 },  /*   */
    { 0x514, +90 * 4 / 10, 0 },  /*   */
    { 0x515, +80 * 4 / 10, 0 },  /*   */
    { 0x520, +70 * 4 / 10, 0 },  /*   */
    { 0x525, +80 * 4 / 10, 0 },  /*   */
    { 0x528, +80 * 4 / 10, 0 },  /*   */
    { 0x530, +120 * 4 / 10, 0 }, /*   */
    { 0x537, +100 * 4 / 10, 0 }, /*   */
    { 0x539, +130 * 4 / 10, 0 }, /*   */
    { 0x540, +110 * 4 / 10, 0 }, /*   */
    { 0x541, +110 * 4 / 10, 0 }, /*   */
    { 0x542, +120 * 4 / 10, 0 }, /*   */
    { 0x544, -110 * 4 / 10, 0 }, /* American Samoa */
    { 0x545, +130 * 4 / 10, 0 }, /* GMT +13 - +15 */
    { 0x546, +110 * 4 / 10, 0 }, /*   */
    { 0x547, -100 * 4 / 10, 0 }, /*   */
    { 0x548, -100 * 4 / 10, 0 }, /*   */
    { 0x549, -110 * 4 / 10, 0 }, /*   */
    { 0x550, +100 * 4 / 10, 0 }, /*   */
    { 0x552, +90 * 4 / 10, 0 },  /*   */
    { 0x602, +20 * 4 / 10, 0 },  /*   */
    { 0x603, +10 * 4 / 10, 0 },  /*   */
    { 0x604, +0 * 4 / 10, 0 },   /*   */
    { 0x605, +10 * 4 / 10, 0 },  /*   */
    { 0x606, +20 * 4 / 10, 0 },  /*   */
    { 0x607, +0 * 4 / 10, 0 },   /*   */
    { 0x608, +0 * 4 / 10, 0 },   /*   */
    { 0x609, +0 * 4 / 10, 0 },   /*   */
    { 0x610, +0 * 4 / 10, 0 },   /*   */
    { 0x611, +0 * 4 / 10, 0 },   /*   */
    { 0x612, +0 * 4 / 10, 0 },   /*   */
    { 0x613, +0 * 4 / 10, 0 },   /*   */
    { 0x614, +10 * 4 / 10, 0 },  /*   */
    { 0x615, +0 * 4 / 10, 0 },   /*   */
    { 0x616, +10 * 4 / 10, 0 },  /*   */
    { 0x617, +40 * 4 / 10, 0 },  /*   */
    { 0x618, +0 * 4 / 10, 0 },   /*   */
    { 0x619, +0 * 4 / 10, 0 },   /*   */
    { 0x620, +0 * 4 / 10, 0 },   /*   */
    { 0x621, +10 * 4 / 10, 0 },  /*   */
    { 0x622, +10 * 4 / 10, 0 },  /*   */
    { 0x623, +10 * 4 / 10, 0 },  /*   */
    { 0x624, +10 * 4 / 10, 0 },  /*   */
    { 0x625, +10 * 4 / 10, 0 },  /*   */
    { 0x626, +0 * 4 / 10, 0 },   /*   */
    { 0x627, +10 * 4 / 10, 0 },  /*   */
    { 0x628, +10 * 4 / 10, 0 },  /*   */
    { 0x629, +10 * 4 / 10, 0 },  /*   */
    { 0x630, +20 * 4 / 10, 0 },  /*   */
    { 0x631, +10 * 4 / 10, 0 },  /*   */
    { 0x632, +0 * 4 / 10, 0 },   /*   */
    { 0x633, +40 * 4 / 10, 0 },  /*   */
    { 0x634, +30 * 4 / 10, 0 },  /*   */
    { 0x635, +20 * 4 / 10, 0 },  /*   */
    { 0x636, +30 * 4 / 10, 0 },  /* Ethiopia */
    { 0x637, +30 * 4 / 10, 0 },  /* Somalia */
    { 0x638, +30 * 4 / 10, 0 },  /* Djibouti */
    { 0x639, +30 * 4 / 10, 0 },  /* Kenya */
    { 0x640, +30 * 4 / 10, 0 },  /* Tanzania */
    { 0x641, +30 * 4 / 10, 0 },  /* Uganda */
    { 0x642, +20 * 4 / 10, 0 },  /* Burundi */
    { 0x643, +20 * 4 / 10, 0 },  /* Mozambique */
    { 0x645, +20 * 4 / 10, 0 },  /* Zambia */
    { 0x646, +30 * 4 / 10, 0 },  /* Madagascar */
    { 0x647, +0 * 4 / 10, 0 },   /* not sure */
    { 0x648, +20 * 4 / 10, 0 },  /* Zimbabwe */
    { 0x649, +10 * 4 / 10, 0 },  /* Namibia */
    { 0x650, +20 * 4 / 10, 0 },  /* Malawi */
    { 0x651, +20 * 4 / 10, 0 },  /* Lesotho */
    { 0x652, +20 * 4 / 10, 0 },  /* Botswana */
    { 0x653, +20 * 4 / 10, 0 },  /* Swaziland */
    { 0x654, +20 * 4 / 10, 0 },  /* Comoros */
    { 0x655, +20 * 4 / 10, 0 },  /* South Africa */
    { 0x659, +30 * 4 / 10, 0 },  /* sudan */
    { 0x702, -60 * 4 / 10, 0 },  /* Belize */
    { 0x704, -60 * 4 / 10, 0 },  /* Guatemala */
    { 0x706, -60 * 4 / 10, 0 },  /* Salvador */
    { 0x708, -60 * 4 / 10, 0 },  /* Honduras */
    { 0x710, -60 * 4 / 10, 0 },  /* Nicaragua */
    { 0x712, -60 * 4 / 10, 0 },  /* Costa Rica */
    { 0x714, -50 * 4 / 10, 0 },  /* Panama */
    { 0x716, -50 * 4 / 10, 0 },  /* Peru */
    { 0x722, -30 * 4 / 10, 0 },  /* Argentina */
    { 0x724, -30 * 4 / 10, 0 },  /* Brazil */
    { 0x730, -50 * 4 / 10, 0 },  /* Chile */
    { 0x732, -50 * 4 / 10, 0 },  /* Colombia */
    { 0x734, -45 * 4 / 10, 0 },  /* Venezuela */
    { 0x736, -40 * 4 / 10, 0 },  /* Bolivia */
    { 0x738, -30 * 4 / 10, 0 },  /* Guyana */
    { 0x740, -50 * 4 / 10, 0 },  /* Ecuador */
    { 0x744, -40 * 4 / 10, 0 },  /* Paraguay */
    { 0x746, -30 * 4 / 10, 0 },  /* Suriname */
    { 0x748, -30 * 4 / 10, 0 },  /* Uruguay */
};

/*
 * 3 函数、变量声明
 */
VOS_UINT32 AT_PlmnTimeZoneProc(TAF_UINT32 mcc, NAS_MM_InfoInd *mmTimeInfo);

LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnGsm(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig);

LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnWcdma(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                                 VOS_UINT8 systemAppConfig);

#if (FEATURE_LTE == FEATURE_ON)
LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnLte(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnNr(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                              VOS_UINT8 systemAppConfig);
#endif

LOCAL VOS_VOID AT_ProcCerssiInfoQueryNotCampOn(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig);


#if (FEATURE_CALL_WAKELOCK == FEATURE_ON && FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)

VOS_VOID AT_SetCsCallStateWakeLock(TAF_CCM_MsgTypeUint32 msgName)
{
    switch(msgName) {
        case ID_TAF_CCM_CALL_ORIG_IND:
        case ID_TAF_CCM_CALL_CONNECT_IND:
        case ID_TAF_CCM_CALL_INCOMING_IND:
        case ID_TAF_CCM_CALL_PROC_IND:
        case ID_TAF_CCM_CALL_ALERTING_IND:
        case ID_TAF_CCM_CALL_HOLD_IND:
        case ID_TAF_CCM_CALL_RETRIEVE_IND:
            AT_WakeLock(&g_callWakeLock);
            break;

        case ID_TAF_CCM_CALL_ALL_RELEASED_IND:
            AT_WakeUnLock(&g_callWakeLock);
            break;

        default:
            break;
    }

    return;
}
#endif


VOS_VOID AT_ProcDmsMsg(struct MsgCB *msg)
{
    MSG_Header      *msgTemp = VOS_NULL_PTR;
    VOS_UINT32       i;
    VOS_UINT32       msgCnt;
    VOS_UINT32       rst;

    /* 从g_atProcMsgFromDmsTab中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromDmsTab) / sizeof(AT_PROC_MsgFromDms);
    msgTemp   = (MSG_Header *)msg;

    /* g_atProcMsgFromDmsTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromDmsTab[i].msgName == msgTemp->msgName) {
            rst = g_atProcMsgFromDmsTab[i].procMsgFunc((struct MsgCB *)msgTemp);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcDmsMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcDmsMsg: Msg Id is invalid!");
    }

    return;
}


VOS_UINT32 AT_ChkDmsSubscripEventNotify(const MSG_Header *msg)
{
    struct DMS_PORT_SubscripEventNotify *event = VOS_NULL_PTR;
    VOS_UINT32 size;
    VOS_UINT32 len;
    VOS_UINT32 i;

    len = sizeof(struct DMS_PORT_SubscripEventNotify);
    if (VOS_GET_MSG_LEN(msg) < len) {
        AT_WARN_LOG("AT_ChkDmsSubscripEventNotify: message length is invalid!");
    }

    event = (struct DMS_PORT_SubscripEventNotify *)msg;

    size = (sizeof(g_atProcEventFromDmsTab) / sizeof(g_atProcEventFromDmsTab[0]));

    for (i = 0; i < size; i++) {
        if (event->eventId == g_atProcEventFromDmsTab[i].eventId) {
            if (TAF_RunChkMsgLenFunc((const MSG_Header *)msg, g_atProcEventFromDmsTab[i].fixdedPartLen,
                    g_atProcEventFromDmsTab[i].chkFunc) != VOS_TRUE) {
                AT_WARN_LOG("AT_ChkDmsSubscripEventNotify: message length is invalid!");

                return VOS_FALSE;
            }
            break;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvDmsSubscripEventNotify(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify  *event = VOS_NULL_PTR;
    VOS_UINT32                            i;
    VOS_UINT32                            eventCnt;
    VOS_UINT32                            rst;

    /* 从g_atProcEventFromDmsTab中获取事件个数 */
    eventCnt = sizeof(g_atProcEventFromDmsTab) / sizeof(AT_PROC_EventFromDms);
    event  = (struct DMS_PORT_SubscripEventNotify *)msg;

    /* g_atProcEventFromDmsTab查表，进行事件分发 */
    for (i = 0; i < eventCnt; i++) {
        if (g_atProcEventFromDmsTab[i].eventId == event->eventId) {
            rst = g_atProcEventFromDmsTab[i].procEventFunc((struct MsgCB *)msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_RcvDmsSubscripEventNotify: event Proc Err!");
            }

            return rst;
        }
    }

    /* 没有找到匹配的消息 */
    if (eventCnt == i) {
        AT_ERR_LOG("AT_RcvDmsSubscripEventNotify: event Id is invalid!");
    }

    return VOS_ERR;
}


TAF_VOID At_MmaMsgProc(struct MsgCB *msg)
{
    MSG_Header *msgTemp = VOS_NULL_PTR;
    VOS_UINT32  i;
    VOS_UINT32  msgCnt;
    VOS_UINT32  rst;

    /* 从g_atProcMsgFromMmaTab中获取消息个数 */
    msgCnt  = sizeof(g_atProcMsgFromMmaTab) / sizeof(AT_PROC_MsgFromMma);
    msgTemp = (MSG_Header *)msg;

    /* g_atProcMsgFromMtaTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromMmaTab[i].msgName == msgTemp->msgName) {
            rst = g_atProcMsgFromMmaTab[i].procMsgFunc((struct MsgCB *)msgTemp);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("At_MmaMsgProc: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_MmaMsgProc: Msg Id is invalid!");
    }

    return;
}


VOS_VOID At_CcmMsgProc(struct MsgCB *msg)
{
    MSG_Header *msgTemp = VOS_NULL_PTR;
    VOS_UINT32  i;
    VOS_UINT32  msgCnt;
    VOS_UINT32  rst;

    msgTemp = (MSG_Header *)msg;

    /* 从g_atProcMsgFromCcmTab中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromCcmTab) / sizeof(AT_PROC_MsgFromCcm);

#if (FEATURE_CALL_WAKELOCK == FEATURE_ON && FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    AT_SetCsCallStateWakeLock(msgTemp->msgName);
#endif

    /* g_atProcMsgFromCcmTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if ((g_atProcMsgFromCcmTab[i].msgName == msgTemp->msgName) && (msgTemp->msgName != ID_TAF_CCM_MSG_TYPE_BUTT)) {
            rst = g_atProcMsgFromCcmTab[i].procMsgFunc((struct MsgCB *)msgTemp);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("At_CcmMsgProc: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_CcmMsgProc: Msg Id is invalid!");
    }

    return;
}


TAF_VOID At_EventMsgProc(MN_AT_IndEvt *msg)
{
    switch (msg->msgName) {
        case MN_CALLBACK_MSG:
            At_SmsMsgProc(msg, msg->len);
            return;

        case MN_CALLBACK_SET:
            At_SetMsgProc((TAF_SetRslt *)msg->content);
            return;

        case MN_CALLBACK_QRY:
            At_QryMsgProc((TAF_QryRslt *)msg->content);
            return;

        case MN_CALLBACK_PS_CALL:
            /* PS域事件处理 */
            AT_RcvTafPsEvt((TAF_PS_Evt *)msg);
            return;

        case MN_CALLBACK_DATA_STATUS:
            At_DataStatusMsgProc((TAF_DataStatusInd *)msg->content);
            return;

        case MN_CALLBACK_SS:
            AT_RcvTafSsaEvt((TAF_SSA_Evt *)msg);
            return;

        case MN_CALLBACK_PHONE:
            At_PhMsgProc(msg->content, msg->len);
            break;

        case MN_CALLBACK_PHONE_BOOK:
            At_TAFPbMsgProc(msg->content, msg->len);
            break;

        case MN_CALLBACK_CMD_CNF:
            At_CmdCnfMsgProc(msg->content, msg->len);
            break;

        case MN_CALLBACK_VOICE_CONTROL:
            At_VcMsgProc(msg, msg->len);
            break;

        case MN_CALLBACK_LOG_PRINT:
            AT_LogPrintMsgProc((TAF_MntnLogPrint *)msg);
            break;

        case MN_CALLBACK_IFACE:
            AT_RcvTafIfaceEvt((TAF_PS_Evt *)msg);
            break;

        default:
            AT_LOG1("At_MsgProc: invalid message name.", msg->msgName);
            return;
    }
}


VOS_BOOL AT_E5CheckRight(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    static VOS_BOOL bIsRight = VOS_FALSE;
    VOS_UINT8       cmdTmp[AT_CMD_TMP_MAX_LENGTH]; /* 须大于aucQuickCmd[]的长度 */
    const VOS_UINT8 quickCmd[] = "AT^OPENPORT=";
    VOS_UINT16      quickCmdLen;
    VOS_UINT16      leftLen;
    VOS_UINT8      *pwdPos = VOS_NULL_PTR;
    VOS_CHAR        acPassword[AT_RIGHT_PWD_LEN + 1];
    VOS_UINT8      *atStart = VOS_NULL_PTR;
    errno_t         memResult;

    /* 当前权限未开启,直接返回  */
    if (g_atRightOpenFlg.rightOpenFlg == AT_RIGHT_OPEN_FLAG_CLOSE) {
        return VOS_TRUE;
    }

    /* 如果已经获取到权限,则直接返回 */
    if (bIsRight == VOS_TRUE) {
        return VOS_TRUE;
    }

    /* SSCOM 输入的字符串前有 0x0A */
    atStart = data;
    while ((*atStart != 'a') && (*atStart != 'A')) {
        atStart++;
        if (atStart >= (data + len)) {
            /* 未找到 a 或 A break */
            break;
        }
    }
    len -= (VOS_UINT16)(atStart - data);
    data = atStart;

    /* 长度非法,直接返回无权限 */
    quickCmdLen = (VOS_UINT16)VOS_StrLen((VOS_CHAR *)quickCmd);
    if (len <= quickCmdLen) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_FALSE;
    }

    memResult = memset_s(cmdTmp, sizeof(cmdTmp), 0x00, (VOS_SIZE_T)sizeof(cmdTmp));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cmdTmp), (VOS_SIZE_T)sizeof(cmdTmp));

    if (quickCmdLen > 0) {
        memResult = memcpy_s(cmdTmp, sizeof(cmdTmp), data, (VOS_SIZE_T)quickCmdLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cmdTmp), (VOS_SIZE_T)quickCmdLen);
    }

    /* 不是AT^OPENPORT命令直接返回无权限 */
    if (VOS_StrNiCmp((VOS_CHAR *)cmdTmp, (VOS_CHAR *)quickCmd, (VOS_SIZE_T)quickCmdLen) != 0) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_FALSE;
    }

    leftLen = len - quickCmdLen;
    pwdPos  = data + quickCmdLen;

    if (leftLen >= sizeof(cmdTmp)) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_FALSE;
    }

    (VOS_VOID)memset_s(cmdTmp, sizeof(cmdTmp), 0x00, sizeof(cmdTmp));
    memResult = memcpy_s(cmdTmp, sizeof(cmdTmp), pwdPos, leftLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cmdTmp), leftLen);

    (VOS_VOID)memset_s(acPassword, sizeof(acPassword), 0x00, sizeof(acPassword));
    memResult = memcpy_s(acPassword, sizeof(acPassword), g_atRightOpenFlg.password, sizeof(g_atRightOpenFlg.password));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acPassword), sizeof(g_atRightOpenFlg.password));

    /* 比较密码是否相同 */
    if (VOS_StrCmp(acPassword, (VOS_CHAR *)cmdTmp) == 0) {
        /* 上报OK并记录有权限 */
        At_FormatResultData(indexNum, AT_OK);
        bIsRight = VOS_TRUE;
        (VOS_VOID)memset_s(acPassword, sizeof(acPassword), 0x00, sizeof(acPassword));
        return VOS_FALSE;
    }
    (VOS_VOID)memset_s(acPassword, sizeof(acPassword), 0x00, sizeof(acPassword));

    /* 上报ERROR */
    At_FormatResultData(indexNum, AT_ERROR);
    return VOS_FALSE;
}


TAF_VOID At_PppProtocolRelIndProc(const AT_PPP_ProtocolRelInd *msg)
{
    DMS_PortIdUint16        portId;
    VOS_UINT16              pppId;
    VOS_UINT8               indexNum;
    DMS_PortModeUint8       mode;
    DMS_PortDataModeUint8   dataMode;

    pppId    = msg->pppId;
    indexNum = g_atPppIndexTab[pppId];
    portId   = g_atClientTab[indexNum].portNo;

    mode     = DMS_PORT_GetMode(portId);
    dataMode = DMS_PORT_GetDataMode(portId);

    if (AT_IsPppUser(indexNum) == VOS_TRUE) {
        if ((mode == DMS_PORT_MODE_ONLINE_DATA) && (dataMode == DMS_PORT_DATA_PPP) &&
            (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_WAIT_PPP_PROTOCOL_REL_SET)) {

            DMS_PORT_ResumeCmdMode(portId);

            /* 停止定时器 */
            AT_STOP_TIMER_CMD_READY(indexNum);

            /* 回复NO CARRIER */
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_NO_CARRIER);
        }
    }

    return;
}

/*
 * Description: AT处理PPP发送的消息
 *   History:
 *   1.Date: 2009-09-24
 *   Modification: Created function
 */
TAF_VOID At_PppMsgProc(struct MsgCB *msg)
{
    MSG_Header *msgTemp = VOS_NULL_PTR;

    msgTemp = (MSG_Header *)msg;

    switch (msgTemp->msgName) {
        case AT_PPP_RELEASE_IND_MSG:
            At_PppReleaseIndProc(((AT_PPP_ReleaseInd *)msgTemp)->clientId);
            break;

        case AT_PPP_PROTOCOL_REL_IND_MSG:
            At_PppProtocolRelIndProc((AT_PPP_ProtocolRelInd *)msgTemp);
            break;

        default:
            AT_LOG1("At_PppMsgProc: msgName is abnormal!", msgTemp->msgName);
            break;
    }
}
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)

VOS_UINT32 AT_ChkHPAMsgLenFunc(struct MsgCB *msg, TAF_ChkMsgLenNameMapTbl *msgNameMapAddr,
    VOS_UINT32 mapTblSize)
{
    HPA_AT_Header *header = VOS_NULL_PTR;
    VOS_UINT32     i;

    if ((VOS_GET_MSG_LEN(msg) + VOS_MSG_HEAD_LENGTH) < sizeof(HPA_AT_Header)) {
        return VOS_FALSE;
    }

    header = (HPA_AT_Header *)msg;

    for (i = 0; i < mapTblSize; i++) {
        if (header->msgId == (VOS_UINT16)msgNameMapAddr[i].msgName) {
                return TAF_RunChkMsgLenFunc((const MSG_Header*)msg,
                    msgNameMapAddr[i].fixdedPartLen, msgNameMapAddr[i].chkFunc);
        }
    }
    return VOS_TRUE;
}
#endif

VOS_VOID At_HPAMsgProc(struct MsgCB *msg)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    HPA_AT_Header           *header = VOS_NULL_PTR;
    TAF_ChkMsgLenNameMapTbl *msgNameMapAddr = VOS_NULL_PTR;
    VOS_UINT32               mapTblSize;

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_HpaMsgProc: pstMsg is NULL");
        return;
    }
    msgNameMapAddr = AT_GetChkDspWphyMsgLenTblAddr();
    mapTblSize = AT_GetChkDspWphyMsgLenTblSize();

    if (AT_ChkHPAMsgLenFunc(msg, msgNameMapAddr, mapTblSize) == VOS_FALSE) {
        AT_WARN_LOG("At_HpaMsgProc: pstMsg msgLen Err");
        return;
    }
    header = (HPA_AT_Header *)msg;

    switch (header->msgId) {
        case ID_HPA_AT_RF_CFG_CNF:
            At_HpaRfCfgCnfProc((HPA_AT_RfCfgCnf *)msg);
            break;

        case ID_HPA_AT_RF_RX_RSSI_IND:
            At_RfRssiIndProc((HPA_AT_RfRxRssiInd *)msg);
            break;

        case ID_WPHY_AT_TX_CLT_IND:
            At_WTxCltIndProc((WPHY_AT_TxCltInd *)msg);
            break;

        case ID_AT_WPHY_RF_PLL_STATUS_CNF:
            At_RfPllStatusCnfProc((PHY_AT_RfPllStatusCnf *)msg);
            break;

        case ID_AT_PHY_POWER_DET_CNF:
            At_RfFpowdetTCnfProc((PHY_AT_PowerDetCnf *)msg);
            break;
        case ID_HPA_AT_MIPI_WR_CNF:
            At_MipiWrCnfProc((HPA_AT_MipiWrCnf *)msg);
            break;
        case ID_HPA_AT_MIPI_RD_CNF:
            At_MipiRdCnfProc((HPA_AT_MipiRdCnf *)msg);
            break;
        case ID_HPA_AT_SSI_WR_CNF:
            At_SsiWrCnfProc((HPA_AT_SsiWrCnf *)msg);
            break;

        case ID_HPA_AT_SSI_RD_CNF:
            At_SsiRdCnfProc((HPA_AT_SsiRdCnf *)msg);
            break;

        case ID_HPA_AT_PDM_CTRL_CNF:
            At_PdmCtrlCnfProc((HPA_AT_PdmCtrlCnf *)msg);
            break;

        default:
            AT_WARN_LOG("At_HpaMsgProc: msgName is Abnormal!");
            break;
    }
#endif

    return;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_VOID At_CHPAMsgProc(struct MsgCB *msg)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    HPA_AT_Header *header = VOS_NULL_PTR;

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_HpaMsgProc: pstMsg is NULL");
        return;
    }

    header = (HPA_AT_Header *)msg;

    switch (header->msgId) {
        case ID_CHPA_AT_RF_CFG_CNF:
            At_CHpaRfCfgCnfProc((CHPA_AT_RfCfgCnf *)msg);
            break;

        case ID_AT_PHY_POWER_DET_CNF:
            At_RfFpowdetTCnfProc((PHY_AT_PowerDetCnf *)msg);
            break;

        default:
            AT_WARN_LOG("At_CHPAMsgProc: msgName is Abnormal!");
            break;
    }
#endif

    return;
}
#endif


VOS_VOID At_GHPAMsgProc(struct MsgCB *msg)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    HPA_AT_Header           *header = VOS_NULL_PTR;
    TAF_ChkMsgLenNameMapTbl *msgNameMapAddr = VOS_NULL_PTR;
    VOS_UINT32               mapTblSize;

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_HpaMsgProc: pstMsg is NULL");
        return;
    }
    msgNameMapAddr = AT_GetChkDspGphyMsgLenTblAddr();
    mapTblSize = AT_GetChkDspGphyMsgLenTblSize();

    if (AT_ChkHPAMsgLenFunc(msg, msgNameMapAddr, mapTblSize) == VOS_FALSE) {
        AT_WARN_LOG("At_GHpaMsgProc: pstMsg msgLen Err");
        return;
    }
    header = (HPA_AT_Header *)msg;

    switch (header->msgId) {
        case ID_GHPA_AT_RF_MSG_CNF:
            At_HpaRfCfgCnfProc((HPA_AT_RfCfgCnf *)msg);
            break;

        case ID_GHPA_AT_RF_RX_RSSI_IND:
            At_RfRssiIndProc((HPA_AT_RfRxRssiInd *)msg);
            break;

        case ID_AT_GPHY_RF_PLL_STATUS_CNF:
            At_RfPllStatusCnfProc((PHY_AT_RfPllStatusCnf *)msg);
            break;

        case ID_HPA_AT_MIPI_WR_CNF:
            At_MipiWrCnfProc((HPA_AT_MipiWrCnf *)msg);
            break;

        case ID_HPA_AT_MIPI_RD_CNF:
            At_MipiRdCnfProc((HPA_AT_MipiRdCnf *)msg);
            break;

        case ID_HPA_AT_SSI_WR_CNF:
            At_SsiWrCnfProc((HPA_AT_SsiWrCnf *)msg);
            break;

        case ID_HPA_AT_SSI_RD_CNF:
            At_SsiRdCnfProc((HPA_AT_SsiRdCnf *)msg);
            break;

        case ID_AT_PHY_POWER_DET_CNF:
            At_RfFpowdetTCnfProc((PHY_AT_PowerDetCnf *)msg);
            break;

        default:
            AT_WARN_LOG("At_HpaMsgProc: msgName is Abnormal!");
            break;
    }
#endif
    return;
}


VOS_UINT32 AT_FormatAtiCmdQryString(ModemIdUint16 modemId, DRV_AGENT_MsidQryCnf *drvAgentMsidQryCnf)
{
    TAF_PH_Meinfo meInfo;
    VOS_UINT8     tmp[AT_MAX_VERSION_LEN];
    VOS_UINT32    i;
    errno_t       memResult;
    VOS_UINT16    dataLen;
    VOS_UINT8     checkData;

    /* 初始化 */
    dataLen   = 0;
    i         = 0;
    checkData = 0;
    (VOS_VOID)memset_s(&meInfo, sizeof(meInfo), 0x00, sizeof(TAF_PH_Meinfo));

    (VOS_VOID)memset_s(tmp, sizeof(tmp), 0x00, sizeof(tmp));

    /* 读取Model信息 */
    if ((drvAgentMsidQryCnf == VOS_NULL_PTR) || (drvAgentMsidQryCnf->result != DRV_AGENT_MSID_QRY_NO_ERROR)) {
        return AT_ERROR;
    }

    /* 读取Manufacturer信息 */
    dataLen = TAF_MAX_MFR_ID_LEN + 1;
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_FMRID, tmp, dataLen) != NV_OK) {
        AT_WARN_LOG("AT_FormatAtiCmdQryString:WARNING:NVIM Read NV_ITEM_FMRID falied!");
        return AT_ERROR;
    } else {
        memResult = memcpy_s(meInfo.fmrId.mfrId, sizeof(meInfo.fmrId.mfrId), tmp, dataLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(meInfo.fmrId.mfrId), dataLen);
    }

    /* 读取IMEI信息 */
    for (i = 0; i < TAF_PH_IMEI_LEN; i++) {
        meInfo.imeisV.imei[i] = drvAgentMsidQryCnf->imei[i] + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
    }
    /* checkdata和结尾符单独赋值，所以减2，一次循环赋i和i+1的值，所以循环加2 */
    for (i = 0; i < (TAF_PH_IMEI_LEN - 2); i += 2) {
        checkData += (TAF_UINT8)(((drvAgentMsidQryCnf->imei[i]) +
                                   ((drvAgentMsidQryCnf->imei[i + 1UL] * AT_DOUBLE_LENGTH) / AT_DECIMAL_BASE_NUM)) +
                                  ((drvAgentMsidQryCnf->imei[i + 1UL] * AT_DOUBLE_LENGTH) % AT_DECIMAL_BASE_NUM));
    }
    checkData = (AT_DECIMAL_BASE_NUM - (checkData % AT_DECIMAL_BASE_NUM)) % AT_DECIMAL_BASE_NUM;

    meInfo.imeisV.imei[TAF_PH_IMEI_LEN - 2] = checkData + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
    meInfo.imeisV.imei[TAF_PH_IMEI_LEN - 1] = 0;

    /* 输出ATI命令返回结果 */
    dataLen = 0;
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s: %s\r\n", "Manufacturer", meInfo.fmrId.mfrId);
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s: %s\r\n", "Model", drvAgentMsidQryCnf->modelId);
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s: %s\r\n", "Revision", drvAgentMsidQryCnf->softwareVerId);
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s: %s\r\n", "IMEI", meInfo.imeisV.imei);
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s", "+GCAP: +CGSM,+DS,+ES");

    g_atSendDataBuff.bufLen = dataLen;

    return AT_OK;
}


VOS_UINT32 AT_RcvDrvAgentMsidQryCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg        *rcvMsg             = VOS_NULL_PTR;
    DRV_AGENT_MsidQryCnf *drvAgentMsidQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;
    ModemIdUint16         modemId;
    VOS_UINT32            ret;

    modemId = MODEM_ID_0;

    /* 初始化 */
    rcvMsg             = (DRV_AGENT_Msg *)msg;
    drvAgentMsidQryCnf = (DRV_AGENT_MsidQryCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(drvAgentMsidQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentMsidQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentMsidQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_MSID_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_MSID;
        result            = AT_FormatAtiCmdQryString(modemId, drvAgentMsidQryCnf);
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGMM_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s", drvAgentMsidQryCnf->modelId);
        result                  = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGMR_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        drvAgentMsidQryCnf->softwareVerId[TAF_MAX_REVISION_ID_LEN] = '\0';
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s", drvAgentMsidQryCnf->softwareVerId);
        result                  = AT_OK;
    } else {
        return VOS_ERR;
    }

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

        AT_ERR_LOG("At_SetImeiPara: Get modem id fail.");
        return VOS_ERR;
    }

    if (result == AT_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentHardwareQryRsp(struct MsgCB *msg)
{
    VOS_UINT32                ret;
    TAF_UINT16                length;
    VOS_UINT8                 indexNum    = 0;
    DRV_AGENT_Msg            *rcvMsg      = VOS_NULL_PTR;
    DRV_AGENT_HardwareQryCnf *hardWareCnf = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg      = (DRV_AGENT_Msg *)msg;
    hardWareCnf = (DRV_AGENT_HardwareQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(hardWareCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentHardwareQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentHardwareQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DRV_AGENT_HARDWARE_QRY) {
        return VOS_ERR;
    }

    length                  = 0;
    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (hardWareCnf->result == DRV_AGENT_NO_ERROR) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\"", hardWareCnf->hwVer);

        g_atSendDataBuff.bufLen = length;
        ret                     = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentVertimeQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg         *rcvMsg              = VOS_NULL_PTR;
    DRV_AGENT_VersionTime *drvAgentVersionTime = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum = 0;

    /* 初始化 */
    rcvMsg              = (DRV_AGENT_Msg *)msg;
    drvAgentVersionTime = (DRV_AGENT_VersionTime *)(rcvMsg->content);
    result              = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(drvAgentVersionTime->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentVertimeQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentVertimeQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VERSIONTIME_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VERTIME命令返回 */
    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName,
            drvAgentVersionTime->data);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentYjcxSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg        *rcvMsg     = VOS_NULL_PTR;
    DRV_AGENT_YjcxSetCnf *yjcxSetCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;
    VOS_UINT16            dataLen;

    /* 初始化 */
    rcvMsg     = (DRV_AGENT_Msg *)msg;
    yjcxSetCnf = (DRV_AGENT_YjcxSetCnf *)(rcvMsg->content);
    result     = AT_OK;
    dataLen    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(yjcxSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentYjcxSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentYjcxSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_YJCX_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^YJCX设置命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (yjcxSetCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        dataLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        yjcxSetCnf->flashInfo[TAF_MAX_FLAFH_INFO_LEN] = '\0';
        dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s", yjcxSetCnf->flashInfo);
        g_atSendDataBuff.bufLen = dataLen;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentYjcxQryCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg        *rcvMsg     = VOS_NULL_PTR;
    DRV_AGENT_YjcxQryCnf *yjcxQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;
    VOS_UINT16            dataLen;

    /* 初始化 */
    rcvMsg     = (DRV_AGENT_Msg *)msg;
    yjcxQryCnf = (DRV_AGENT_YjcxQryCnf *)(rcvMsg->content);
    result     = AT_OK;
    dataLen    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(yjcxQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentYjcxQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentYjcxQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_YJCX_QUERY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^YJCX查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (yjcxQryCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        dataLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        yjcxQryCnf->gpioInfo[TAF_MAX_GPIO_INFO_LEN] = '\0';
        dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%s", yjcxQryCnf->gpioInfo);

        g_atSendDataBuff.bufLen = dataLen;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID At_QryEonsUcs2RspProc(VOS_UINT8 indexNum, VOS_UINT8 opId, VOS_UINT32 result,
                               TAF_MMA_EonsUcs2PlmnName *eonsUcs2PlmnName, TAF_MMA_EonsUcs2HnbName *eonsUcs2HNBName)
{
    TAF_MMA_EonsUcs2PlmnName *plmnName = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT32                loop;

#if (FEATURE_CSG == FEATURE_ON)
    TAF_MMA_EonsUcs2HnbName *hnbName = VOS_NULL_PTR;
    VOS_UINT8                homeNodeBLen;

    hnbName = (TAF_MMA_EonsUcs2HnbName *)eonsUcs2HNBName;

    length = 0;

    if (hnbName != VOS_NULL_PTR) {
        if (hnbName->homeNodeBNameLen != 0) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

            /* 打印home NodeB Name, ucs2编码，最大长度48字节 */
            homeNodeBLen = AT_MIN(hnbName->homeNodeBNameLen, TAF_MMA_MAX_HOME_NODEB_NAME_LEN);

            for (loop = 0; loop < homeNodeBLen; loop++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", hnbName->homeNodeBName[loop]);
            }
            g_atSendDataBuff.bufLen = length;

            At_FormatResultData(indexNum, result);

            return;
        }
    }

#endif

    /* 变量初始化 */
    plmnName = (TAF_MMA_EonsUcs2PlmnName *)eonsUcs2PlmnName;

    if (plmnName == VOS_NULL_PTR) {
        return;
    }

    /* 转换LongName及ShortName */
    if (plmnName->longNameLen <= TAF_PH_OPER_NAME_LONG && plmnName->shortNameLen <= TAF_PH_OPER_NAME_SHORT) {
        /* ^EONSUCS2:<long name>,<short name> */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        for (loop = 0; loop < plmnName->longNameLen; loop++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", plmnName->longName[loop]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");

        for (loop = 0; loop < plmnName->shortNameLen; loop++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", plmnName->shortName[loop]);
        }

        g_atSendDataBuff.bufLen = length;
    } else {
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_RcvMmaEonsUcs2Cnf(struct MsgCB *msg)
{
    TAF_MMA_EonsUcs2Cnf *eonsUcs2QryCnfMsg = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum = 0;

    /* 初始化 */
    eonsUcs2QryCnfMsg = (TAF_MMA_EonsUcs2Cnf *)msg;
    result            = AT_OK;

    if (eonsUcs2QryCnfMsg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMmaCrpnQueryRsp:MSG IS NULL!");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(eonsUcs2QryCnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCrpnQueryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEonsUcs2Cnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EONSUCS2_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^EONSUCS2查询命令返回 */
    if (eonsUcs2QryCnfMsg->rslt != TAF_ERR_NO_ERROR) {
        result = At_ChgTafErrorCode(indexNum, (VOS_UINT16)eonsUcs2QryCnfMsg->errorCause);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, result);
        return VOS_OK;
    }
    At_QryEonsUcs2RspProc(indexNum, eonsUcs2QryCnfMsg->opId, result, &eonsUcs2QryCnfMsg->eonsUcs2PlmnName,
                          &eonsUcs2QryCnfMsg->eonsUcs2HNBName);

    return VOS_OK;
}


VOS_UINT32 AT_RcvAtMmaUsimStatusInd(struct MsgCB *msg)
{
    AT_MMA_UsimStatusInd *atMmaUsimStatusIndMsg = VOS_NULL_PTR;
    AT_USIM_InfoCtx      *usimInfoCtx           = VOS_NULL_PTR;
    errno_t               memResult;
    ModemIdUint16         modemId;

    /* 初始化 */
    atMmaUsimStatusIndMsg = (AT_MMA_UsimStatusInd *)msg;

    modemId = AT_GetModemIDFromPid(VOS_GET_SENDER_ID(atMmaUsimStatusIndMsg));

    if (modemId >= MODEM_ID_BUTT) {
        AT_PR_LOGE("enModemId :%d , ulSenderPid :%d", modemId, VOS_GET_SENDER_ID(atMmaUsimStatusIndMsg));

        return VOS_ERR;
    }

    usimInfoCtx = AT_GetUsimInfoCtxFromModemId(modemId);

    /* 刷新卡状态全局变量 */
    usimInfoCtx->cardType       = atMmaUsimStatusIndMsg->cardType;
    usimInfoCtx->cardMediumType = atMmaUsimStatusIndMsg->cardMediumType;
    usimInfoCtx->cardStatus     = atMmaUsimStatusIndMsg->cardStatus;
    usimInfoCtx->imsiLen        = atMmaUsimStatusIndMsg->imsiLen;
    memResult                   = memcpy_s(usimInfoCtx->imsi, sizeof(usimInfoCtx->imsi), atMmaUsimStatusIndMsg->imsi,
                                           NAS_MAX_IMSI_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(usimInfoCtx->imsi), NAS_MAX_IMSI_LENGTH);

    AT_PR_LOGI("CardType: %d , CardStatus: %d , ulSenderPid: %d", atMmaUsimStatusIndMsg->cardType,
               atMmaUsimStatusIndMsg->cardStatus, VOS_GET_SENDER_ID(atMmaUsimStatusIndMsg));

    return VOS_OK;
}


VOS_UINT32 At_RcvAtCcMsgStateQryCnfProc(struct MsgCB *msg)
{
    AT_CC_StateQryCnfMsg *atCcStateQryCnfMsg = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;
    VOS_UINT32            i;
    VOS_UINT16            length;

    /* 初始化 */
    atCcStateQryCnfMsg = (AT_CC_StateQryCnfMsg *)msg;
    result             = AT_OK;
    length             = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(atCcStateQryCnfMsg->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvAtCcMsgStateQryCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvAtCcMsgStateQryCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前未等待该命令回复 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CC_STATE_QUERY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CCC查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (atCcStateQryCnfMsg->callNum == 0) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
        atCcStateQryCnfMsg->callNum = AT_MIN(atCcStateQryCnfMsg->callNum, MN_CALL_MAX_NUM);
        for (i = 0; i < atCcStateQryCnfMsg->callNum; i++) {
            AT_ShowCccRst(&atCcStateQryCnfMsg->ccStateInfoList[i], &length);
        }

        if (length == 0) {
            result = AT_ERROR;
        }
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCmmSetCmdRsp(struct MsgCB *msg)
{
    TAF_MMA_CmmSetCnf *mnMmTestCmdRspMsg = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum = 0;
    VOS_UINT32         i;
    VOS_UINT16         length;

    /* 初始化 */
    mnMmTestCmdRspMsg = (TAF_MMA_CmmSetCnf *)msg;
    result            = AT_OK;
    length            = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(mnMmTestCmdRspMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCmmSetCmdRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCmmSetCmdRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CMM查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (mnMmTestCmdRspMsg->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        if (mnMmTestCmdRspMsg->atCmdRslt.rsltNum > 0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d", mnMmTestCmdRspMsg->atCmdRslt.rslt[0]);

            mnMmTestCmdRspMsg->atCmdRslt.rsltNum = AT_MIN(mnMmTestCmdRspMsg->atCmdRslt.rsltNum, AT_CMD_PADDING_LEN);
            for (i = 1; i < mnMmTestCmdRspMsg->atCmdRslt.rsltNum; i++) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", mnMmTestCmdRspMsg->atCmdRslt.rslt[i]);
            }
            g_atSendDataBuff.bufLen = length;
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentFullHardwareQryRsp(struct MsgCB *msg)
{
    TAF_UINT16                    length;
    VOS_UINT8                     indexNum = 0;
    DRV_AGENT_Msg                *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_FullHardwareQryCnf *event    = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_FullHardwareQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentFullHardwareQryRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentFullHardwareQryRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DRV_AGENT_FULL_HARDWARE_QRY) {
        return VOS_ERR;
    }

    length                  = 0;
    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->result == DRV_AGENT_NO_ERROR) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s ", event->modelId);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s,", event->revisionId);

        event->hwVer[DRV_AGENT_MAX_HARDWARE_LEN] = '\0';
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s\"", event->hwVer);

        g_atSendDataBuff.bufLen = length;

        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetSimlockCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_SetSimlockCnf *event  = VOS_NULL_PTR;
    VOS_UINT32               ret;
    VOS_UINT8                indexNum = 0;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_SetSimlockCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetSimlockCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetSimlockCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ) {
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (event->result == DRV_AGENT_NO_ERROR) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetNvRestoreCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum = 0;
    DRV_AGENT_Msg          *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_NvrestoreRst *event    = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvrestoreRst *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetNvRestoreCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetNvRestoreCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVRESTORE_SET) {
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%d", event->result);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryNvRestoreRstCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              restoreStatus;
    DRV_AGENT_Msg          *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_NvrestoreRst *event  = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvrestoreRst *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryNvRestoreRstCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryNvRestoreRstCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVRSTSTTS_READ) {
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((event->result == AT_NV_RESTORE_RESULT_INIT) || (event->result == AT_NV_RESTORE_RUNNING)) {
        restoreStatus = AT_NV_RESTORE_FAIL;
    } else if (event->result == VOS_OK) {
        restoreStatus = AT_NV_RESTORE_SUCCESS;
    } else {
        restoreStatus = AT_NV_RESTORE_FAIL;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%d", restoreStatus);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentNvRestoreManuDefaultRsp(struct MsgCB *msg)
{
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              rst;
    VOS_UINT32              result;
    DRV_AGENT_Msg          *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_NvrestoreRst *event  = VOS_NULL_PTR;

    /* 初始化 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvrestoreRst *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentNvRestoreManuDefaultRsp: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentNvRestoreManuDefaultRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_F_SET) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    result = event->result;
    if (result != NV_OK) {
        rst = AT_ERROR;
    } else {
        /* E5通知APP恢复用户设置  */
        AT_PhSendRestoreFactParm();

        rst = AT_OK;
    }
    At_FormatResultData(indexNum, rst);

    return VOS_OK;
}


VOS_UINT32 AT_GetImeiValue(ModemIdUint16 modemId, VOS_UINT8 imei[TAF_PH_IMEI_LEN + 1], VOS_UINT8 imeiMaxLength)
{
    NV_SC_PersCtrl scPersCtrl;
    VOS_UINT8      buf[TAF_PH_IMEI_LEN + 1];
    VOS_UINT8      checkData;
    VOS_UINT32     dataLen;
    VOS_UINT32     i;

    if (imeiMaxLength < (TAF_PH_IMEI_LEN + 1)) {
        return VOS_ERR;
    }

    checkData = 0;
    dataLen   = TAF_PH_IMEI_LEN;

    (VOS_VOID)memset_s(buf, sizeof(buf), 0x00, sizeof(buf));

    (VOS_VOID)memset_s(&scPersCtrl, sizeof(scPersCtrl), 0x00, sizeof(scPersCtrl));

    if (TAF_ACORE_NV_READ(modemId, en_NV_Item_IMEI, buf, dataLen) != NV_OK) {
        AT_WARN_LOG("TAF_GetImeiValue:Read IMEI Failed!");
        return VOS_ERR;
    }

#if (MULTI_MODEM_NUMBER == 3)
#if (VOS_WIN32 != VOS_OS_VER)
    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_NV_SC_PERS_CTRL_CFG, &scPersCtrl, sizeof(scPersCtrl)) != NV_OK) {
        AT_WARN_LOG("TAF_GetImeiValue:Read NV_ITEM_NV_SC_PERS_CTRL_CFG Failed!");
        return VOS_ERR;
    }

    /* NV 4008读取值为0x5a5a表示生效，其他值表示不生效 */
    if ((modemId == MODEM_ID_2) && (scPersCtrl.imei0ReplaseImei2 == 0x5A5A)) {
        /* IMEI all zero */
        for (i = 0; i < dataLen; i++) {
            if (buf[i] != 0) {
                break;
            }
        }

        if (i >= dataLen) {
            (VOS_VOID)memset_s(buf, sizeof(buf), 0x00, sizeof(buf));

            if (TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_IMEI, buf, dataLen) != NV_OK) {
                AT_WARN_LOG("TAF_GetImeiValue:Read IMEI Failed!");
                return VOS_ERR;
            }
        }
    }
#endif
#endif

    checkData = 0;
    /* checkdata和结尾符单独赋值，所以减2，一次循环赋i和i+1的值，所以循环加2 */
    for (i = 0; i < (TAF_PH_IMEI_LEN - 2); i += 2) {
        checkData += buf[i] + ((buf[i + 1UL] + buf[i + 1UL]) / AT_DECIMAL_BASE_NUM) +
                     ((buf[i + 1UL] + buf[i + 1UL]) % AT_DECIMAL_BASE_NUM);
    }
    checkData = (AT_DECIMAL_BASE_NUM - (checkData % AT_DECIMAL_BASE_NUM)) % AT_DECIMAL_BASE_NUM;
    AT_DeciDigit2Ascii(buf, (VOS_UINT8)dataLen, imei);
    imei[TAF_PH_IMEI_LEN - 2] = checkData + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
    imei[TAF_PH_IMEI_LEN - 1] = 0;

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetGpioplRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg          *rcvMsg             = VOS_NULL_PTR;
    DRV_AGENT_GpioplSetCnf *drvAgentGpioSetCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum           = 0;
    VOS_UINT32              result;

    rcvMsg             = (DRV_AGENT_Msg *)msg;
    drvAgentGpioSetCnf = (DRV_AGENT_GpioplSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(drvAgentGpioSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioplRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGpioplRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GPIOPL_SET) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (drvAgentGpioSetCnf->fail == VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryGpioplRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg          *rcvMsg     = VOS_NULL_PTR;
    DRV_AGENT_GpioplQryCnf *gpioQryCnf = VOS_NULL_PTR;
    VOS_UINT16              length;
    VOS_UINT32              i;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              result;

    /* 初始化消息，获取ucContent */
    rcvMsg     = (DRV_AGENT_Msg *)msg;
    gpioQryCnf = (DRV_AGENT_GpioplQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(gpioQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryGpioplRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryGpioplRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_GPIOPL_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GPIOPL_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (gpioQryCnf->fail != VOS_TRUE) {
        /* 打印结果 */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        for (i = 0; i < DRVAGENT_GPIOPL_MAX_LEN; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)(g_atSndCodeAddress + length), "%02X", gpioQryCnf->gpiopl[i]);
        }

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else {
        /* 调用AT_FormATResultDATa发送命令结果 */
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetDatalockRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg            *rcvMsg         = VOS_NULL_PTR;
    DRV_AGENT_DatalockSetCnf *datalockSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum       = 0;
    VOS_UINT32                result;

    /* 初始化 */
    rcvMsg         = (DRV_AGENT_Msg *)msg;
    datalockSetCnf = (DRV_AGENT_DatalockSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(datalockSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetDatalockRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetDatalockRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DATALOCK_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DATALOCK_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (datalockSetCnf->fail != VOS_TRUE) {
        /* 设置dATalock已解锁 */
        g_atDataLocked = VOS_FALSE;
        result         = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用AT_FormAtResultData发送命令结果  */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_GetSimLockStatus(VOS_UINT8 indexNum)
{
    /* 发消息到C核获取SIMLOCK 状态信息 */
    if (Taf_ParaQuery(g_atClientTab[indexNum].clientId, 0, TAF_PH_SIMLOCK_VALUE_PARA, VOS_NULL_PTR) != TAF_SUCCESS) {
        AT_WARN_LOG("AT_GetSimLockStatus: Taf_ParaQuery fail.");
        return VOS_ERR;
    }

    /* ^SIMLOCK=2查询UE的锁卡状态不在AT命令处理的主流程，需要本地启动保护定时器并更新端口状态 */
    if (At_StartTimer(AT_SET_PARA_TIME, indexNum) != AT_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_ERR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIMLOCKSTATUS_READ;

    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryTbatvoltRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg            *rcvMsg         = VOS_NULL_PTR;
    DRV_AGENT_TbatvoltQryCnf *tbatvoltQryCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum       = 0;
    VOS_UINT32                result;

    /* 初始化 */
    rcvMsg         = (DRV_AGENT_Msg *)msg;
    tbatvoltQryCnf = (DRV_AGENT_TbatvoltQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(tbatvoltQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryTbatvoltRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryTbatvoltRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_TBATVOLT_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TBATVOLT_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 电池电压查询结果判断 */
    if (tbatvoltQryCnf->fail != VOS_TRUE) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
                tbatvoltQryCnf->batVol);
        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用AT_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_VOID At_RcvVcMsgQryModeCnfProc(MN_AT_IndEvt *data)
{
    MN_AT_IndEvt     *rcvMsg   = VOS_NULL_PTR;
    APP_VC_EventInfo *event    = VOS_NULL_PTR;
    VOS_UINT8         indexNum = 0;
    VOS_UINT16        devMode;
    VOS_UINT32        ret;

    /* 初始化 */
    rcvMsg = data;
    event  = (APP_VC_EventInfo *)data->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgQryModeCnfProc:WARNING:AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvVcMsgQryModeCnfProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* 格式化VMSET命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_VMSET_READ) {
        devMode = event->devMode;

        /* 无效的模式，直接返回ERROR */
        if (devMode >= VC_PHY_DEVICE_MODE_BUTT) {
            g_atSendDataBuff.bufLen = 0;
            ret                     = AT_ERROR;
        } else {
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, devMode);
            ret                     = AT_OK;
        }
    } else {
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_UINT32 AT_RcvRnicDialModeCnf(MsgBlock *msg)
{
    VOS_UINT16           length;
    VOS_UINT8            indexNum = 0;
    RNIC_AT_DialModeCnf *rcvMsg   = VOS_NULL_PTR;

    length = 0;
    rcvMsg = (RNIC_AT_DialModeCnf *)msg;

    if (At_ClientIdToUserId(rcvMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRnicDialModeCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRnicDialModeCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d", rcvMsg->dialMode);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", rcvMsg->eventReportFlag);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", rcvMsg->idleTime);

    g_atSendDataBuff.bufLen = length;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_VOID AT_RcvRnicRmnetCfgCnf(MsgBlock *msg)
{
    RNIC_AT_RmnetCfgCnf *rmnetCfgCnf;
    VOS_UINT32 ret;
    VOS_UINT8 indexNum = 0;

    rmnetCfgCnf = (RNIC_AT_RmnetCfgCnf *)msg;

    if (At_ClientIdToUserId(rmnetCfgCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvRnicRmnetCfgCnf: WARNING: AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvRnicRmnetCfgCnf: AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RMNETCFG_SET) {
        AT_WARN_LOG("AT_RcvRnicRmnetCfgCnf: WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    ret = (rmnetCfgCnf->result == VOS_OK) ? AT_OK : AT_ERROR;

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_ZeroReplaceBlankInString(VOS_UINT8 *data, VOS_UINT32 len)
{
    TAF_UINT32 chkLen = 0;
    TAF_UINT8 *write  = data;
    TAF_UINT8 *read   = data;

    /* 输入参数检查 */
    while (chkLen < len) {
        /*
         * 时间格式 May  5 2011 17:08:00
         * 转换成   May 05 2011 17:08:00
         */
        if (*read++ == ' ') {
            if (*read == ' ') {
                write++;
                *write = '0';
                read++;
                chkLen++;
            }
        }
        write++;
        chkLen++;
    }

    return;
}


VOS_UINT32 AT_RcvDrvAgentQryVersionRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg            = VOS_NULL_PTR;
    DRV_AGENT_VersionQryCnf *versionQryCnfInfo = VOS_NULL_PTR;
    VOS_UINT8                indexNum          = 0;
    VOS_UINT32               len;
    VOS_CHAR                 acTmpTime[AT_AGENT_DRV_VERSION_TIME_LEN] = {0};
    VOS_UINT16               length;
    VOS_BOOL                 bPhyNumIsNull;

    VOS_UINT32     opt;
    VOS_UINT32     ret;
    errno_t        memResult;
    TAF_NVIM_CsVer csver;
    ModemIdUint16  modemId;

    modemId = MODEM_ID_0;

    AT_PR_LOGI("Rcv Msg");

    csver.csver = 0;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CSVER, &(csver.csver), sizeof(csver.csver));

    AT_PR_LOGI("Call interface success!");
    if (ret != NV_OK) {
        return AT_ERROR;
    }

    /* 初始化消息，获取ucContent */
    len               = 0;
    rcvMsg            = (DRV_AGENT_Msg *)msg;
    versionQryCnfInfo = (DRV_AGENT_VersionQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(versionQryCnfInfo->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryVersionRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryVersionRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VERSION_QRY/AT_CMD_RSFR_VERSION_QRY, ^RSFR命令也借用此接口 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VERSION_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RSFR_VERSION_QRY)) {
        return VOS_ERR;
    }

    ret = AT_GetModemIdFromClient(indexNum, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RcvDrvAgentQryVersionRsp: Get modem id fail.");
        return VOS_ERR;
    }

    opt = g_atClientTab[indexNum].cmdCurrentOpt;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 查询出错 */
    if (versionQryCnfInfo->result != DRV_AGENT_VERSION_QRY_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    /* 版本编译时间格式转换，将时间戳中连续两个空格的后一个空格用0替换 */
    len       = VOS_StrLen(versionQryCnfInfo->verTime);
    memResult = memcpy_s(acTmpTime, sizeof(acTmpTime), versionQryCnfInfo->verTime, len + 1);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acTmpTime), len + 1);
    At_ZeroReplaceBlankInString((VOS_UINT8 *)acTmpTime, len);

    /* 字符串预解析 */
    length = TAF_CDROM_VERSION_LEN;
    At_DelCtlAndBlankCharWithEndPadding(versionQryCnfInfo->isoVer.isoInfo, &length);

    /* 获取物理号是否为空的标识 */
    if (AT_PhyNumIsNull(modemId, AT_PHYNUM_TYPE_IMEI, &bPhyNumIsNull) != AT_OK) {
        AT_WARN_LOG("AT_RcvDrvAgentQryVersionRsp(): AT_PhyNumIsNull Error!");
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    length = 0;

    /*
     * 内部版本号在单板未写入物理号前，查询需如实显示；写入物理号后，如查询版本号前未
     * 输入解锁指令，内部版本号显示为空，如果已经输入解锁指令，内部版本号如实显示
     */
    if (g_atDataLocked == VOS_FALSE || (bPhyNumIsNull == VOS_TRUE)) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:BDT:%s%s", "^VERSION", acTmpTime, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTS:%s%s", "^VERSION",
            versionQryCnfInfo->softVersion.revisionId, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTS:%s%s", "^VERSION",
            versionQryCnfInfo->softVersion.revisionId, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTD:%s%s", "^VERSION", versionQryCnfInfo->isoVer.isoInfo,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTD:%s%s", "^VERSION", versionQryCnfInfo->isoVer.isoInfo,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTH:%s%s", "^VERSION", versionQryCnfInfo->interHwVer.hwVer,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTH:%s%s", "^VERSION", versionQryCnfInfo->fullHwVer.hwVer,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTU:%s%s", "^VERSION", versionQryCnfInfo->modelId.modelId,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTU:%s%s", "^VERSION",
            versionQryCnfInfo->interModelId.modelId, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:CFG:%d%s", "^VERSION", csver.csver, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:PRL:", "^VERSION");

        g_atSendDataBuff.bufLen = length;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:BDT:%s%s", "^VERSION", acTmpTime, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTS:%s%s", "^VERSION",
            versionQryCnfInfo->softVersion.revisionId, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTS:%s", "^VERSION", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTD:%s%s", "^VERSION", versionQryCnfInfo->isoVer.isoInfo,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTD:%s", "^VERSION", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTH:%s%s", "^VERSION", versionQryCnfInfo->interHwVer.hwVer,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTH:%s", "^VERSION", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:EXTU:%s%s", "^VERSION", versionQryCnfInfo->modelId.modelId,
            g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:INTU:%s", "^VERSION", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:CFG:%d%s", "^VERSION", csver.csver, g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "%s:PRL:", "^VERSION");

        g_atSendDataBuff.bufLen = length;
    }

    /* 如果是^RSFR命令发起的查询VERSION信息的请求,则由^RSFR命令去处理 */
    if (opt == AT_CMD_RSFR_VERSION_QRY) {
        AT_SetRsfrVersionCnf(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

        /* AT返回的字符串在AT_SetRsfrVersionCnf中处理 */
        return VOS_OK;
    }

    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_VOID AT_SupportHsdpa(AT_NvimUeCapa *uECapa, VOS_BOOL *pbSupportHsdpa)
{
    /* 初始化为不支持DPA */
    *pbSupportHsdpa = VOS_FALSE;

    if (uECapa->asRelIndicator >= AT_PTL_VER_ENUM_R5) {
        if (uECapa->hspaStatus == AT_HSPA_STATUS_ACTIVATE) {
            if (uECapa->hsdschSupport == PS_TRUE) {
                *pbSupportHsdpa = VOS_TRUE;
            }
        } else {
            *pbSupportHsdpa = VOS_TRUE;
        }
    }
}


VOS_VOID AT_SupportHsupa(AT_NvimUeCapa *uECapa, VOS_BOOL *pbSupportHsupa)
{
    /* 初始化为不支持UPA */
    *pbSupportHsupa = VOS_FALSE;

    if (uECapa->asRelIndicator >= AT_PTL_VER_ENUM_R6) {
        if (uECapa->hspaStatus == AT_HSPA_STATUS_ACTIVATE) {
            if (uECapa->edchSupport == PS_TRUE) {
                *pbSupportHsupa = VOS_TRUE;
            }
        } else {
            *pbSupportHsupa = VOS_TRUE;
        }
    }
}


VOS_UINT32 AT_GetWFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure, MTA_AT_SfeatureQryCnf *atAgentSfeatureQryCnf)
{
    AT_NvimUeCapa uECapa;
    VOS_UINT32    len;
    VOS_BOOL      bSupportHsdpa;
    VOS_BOOL      bSupportHsupa;
    VOS_UINT8     strTmp[AT_SFEATURE_TMP_STRING_LEN] = {0};
    VOS_UINT8     strDiv[AT_SFEATURE_DIV_STRING_LEN];
    VOS_UINT32    divLen;
    errno_t       memResult;

    bSupportHsdpa = VOS_FALSE;
    bSupportHsupa = VOS_FALSE;

    (VOS_VOID)memset_s(&uECapa, sizeof(uECapa), 0x00, sizeof(uECapa));

    /* 读取HSPA,HSPA+支持能力 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WAS_RADIO_ACCESS_CAPA_NEW, &uECapa, sizeof(AT_NvimUeCapa)) != NV_OK) {
        AT_WARN_LOG("AT_GetWFeatureInfo: en_NV_Item_WAS_RadioAccess_Capa NV Read  Fail!");
        return VOS_ERR;
    }

    /* 获取Wcdma频段信息 */
    /* !!!如果增加频段，aucStrTmp 长度需要增加!!! */
    len = (VOS_UINT32)AT_GetWcdmaBandStr(strTmp, sizeof(strTmp), &(atAgentSfeatureQryCnf->bandFeature));

    /* HSPA+是否支持 */
    if ((uECapa.asRelIndicator >= PS_PTL_VER_R7) && (uECapa.macEhsSupport == PS_TRUE) &&
        (uECapa.hsdschPhyCategory >= AT_HSDSCH_PHY_CATEGORY_10) && (uECapa.hsdschPhyCategoryExt != 0) &&
        (uECapa.hsdschSupport == PS_TRUE)) {
        feATure[AT_FEATURE_HSPAPLUS].featureFlag = AT_FEATURE_EXIST;
        if (len > 0) {
            memResult = memcpy_s(feATure[AT_FEATURE_HSPAPLUS].content, sizeof(feATure[AT_FEATURE_HSPAPLUS].content),
                                 strTmp, len);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_HSPAPLUS].content), len);
        }
    }

    /* DPA是否支持 */
    AT_SupportHsdpa(&uECapa, &bSupportHsdpa);
    if (bSupportHsdpa == VOS_TRUE) {
        feATure[AT_FEATURE_HSDPA].featureFlag = AT_FEATURE_EXIST;
        if (len > 0) {
            memResult = memcpy_s(feATure[AT_FEATURE_HSDPA].content, sizeof(feATure[AT_FEATURE_HSDPA].content), strTmp,
                                 len);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_HSDPA].content), len);
        }
    }

    /* UPA是否支持 */
    AT_SupportHsupa(&uECapa, &bSupportHsupa);
    if (bSupportHsupa == VOS_TRUE) {
        feATure[AT_FEATURE_HSUPA].featureFlag = AT_FEATURE_EXIST;
        if (len > 0) {
            memResult = memcpy_s(feATure[AT_FEATURE_HSUPA].content, sizeof(feATure[AT_FEATURE_HSUPA].content), strTmp,
                                 len);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_HSUPA].content), len);
        }
    }

    /* 分集信息 */
    feATure[AT_FEATURE_DIVERSITY].featureFlag = AT_FEATURE_EXIST;
    /* UMTS的分集信息输出 */
    (VOS_VOID)memset_s(strDiv, sizeof(strDiv), 0x00, sizeof(strDiv));

    /* !!!如果增加频段，aucStrDiv 长度需要增加!!! */
    divLen = (VOS_UINT32)AT_GetWcdmaDivBandStr(strDiv, sizeof(strDiv));

    if (divLen > 0) {
        memResult = memcpy_s(feATure[AT_FEATURE_DIVERSITY].content, sizeof(feATure[AT_FEATURE_DIVERSITY].content),
                             strDiv, divLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_DIVERSITY].content), divLen);
    }

    /* UMTS */
    feATure[AT_FEATURE_UMTS].featureFlag = AT_FEATURE_EXIST;
    if (len > 0) {
        memResult = memcpy_s(feATure[AT_FEATURE_UMTS].content, sizeof(feATure[AT_FEATURE_UMTS].content), strTmp, len);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_UMTS].content), len);
    }

    return VOS_OK;
}


VOS_VOID AT_GetGFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure, MTA_AT_SfeatureQryCnf *aTAgentSfeatureQryCnf)
{
    VOS_UINT32 len;
    errno_t    memResult;

    VOS_UINT16 egprsFlag;
    VOS_UINT8  strTmp[AT_SFEATURE_TMP_STRING_LEN] = {0};

    egprsFlag = 0;

    /* 获取GSM频段信息 */
    /* !!!如果增加频段，aucStrDiv 长度需要增加!!! */
    len = (VOS_UINT32)AT_GetGsmBandStr(strTmp, sizeof(strTmp), &(aTAgentSfeatureQryCnf->bandFeature));

    if (len == 0) {
        PS_PRINTF_WARNING("<AT_GetGFeatureInfo> Not Support G.\n");
        return;
    }

    /* 读取EDGE支持能力 */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_EGPRS_FLAG, &egprsFlag, sizeof(VOS_UINT16)) != NV_OK) {
        AT_WARN_LOG("AT_GetGFeatureInfo: NV_ITEM_EGPRS_FLAG NV Read  Fail!");
        feATure[AT_FEATURE_EDGE].featureFlag = AT_FEATURE_EXIST;
        memResult = memcpy_s(feATure[AT_FEATURE_EDGE].content, sizeof(feATure[AT_FEATURE_EDGE].content), strTmp, len);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_EDGE].content), len);
    } else {
        if (egprsFlag != 0) {
            feATure[AT_FEATURE_EDGE].featureFlag = AT_FEATURE_EXIST;
            memResult = memcpy_s(feATure[AT_FEATURE_EDGE].content, sizeof(feATure[AT_FEATURE_EDGE].content), strTmp,
                                 len);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_EDGE].content), len);
        }
    }

    /* GPRS */
    feATure[AT_FEATURE_GPRS].featureFlag = AT_FEATURE_EXIST;
    memResult = memcpy_s(feATure[AT_FEATURE_GPRS].content, sizeof(feATure[AT_FEATURE_GPRS].content), strTmp, len);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_GPRS].content), len);

    /* GSM */
    feATure[AT_FEATURE_GSM].featureFlag = AT_FEATURE_EXIST;
    memResult = memcpy_s(feATure[AT_FEATURE_GSM].content, sizeof(feATure[AT_FEATURE_GSM].content), strTmp, len);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_GSM].content), len);
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_GetNrFeatureInfo(MTA_NRRC_BandInfo nrBandInfo, AT_FEATURE_SUPPORT_ST *feATure)
{
    VOS_UINT32 i;
    VOS_UINT32 strLen;
    VOS_UINT32 bandCnt;
    errno_t    memResult;
    VOS_UINT8  tempStr[AT_FEATURE_BAND_STR_LEN_MAX];

    /* 初始化 */
    strLen = 0;
    (VOS_VOID)memset_s(tempStr, sizeof(tempStr), 0x0, sizeof(tempStr));

    /* 设置Flag标志 */
    feATure[AT_FEATURE_NR].featureFlag = AT_FEATURE_EXIST;

    bandCnt = (nrBandInfo.bandCnt > AT_MTA_UECAP_MAX_NR_BAND_NUM) ? AT_MTA_UECAP_MAX_NR_BAND_NUM : nrBandInfo.bandCnt;

    for (i = 0; i < bandCnt; i++) {
        strLen += (VOS_UINT32)AT_FormatReportString(AT_FEATURE_BAND_STR_LEN_MAX, (VOS_CHAR *)tempStr,
            (VOS_CHAR *)(tempStr + strLen), "B%d,", nrBandInfo.bandInfo[i]);
    }

    if (strLen > 0) {
        /* 屏蔽掉最后一个逗号 */
        tempStr[strLen - 1] = '\0';

        memResult = memcpy_s(feATure[AT_FEATURE_NR].content, sizeof(feATure[AT_FEATURE_NR].content), tempStr, strLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[AT_FEATURE_NR].content), strLen);
    }

    return;
}

#endif


#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
VOS_UINT32 AT_GetTdsFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure)
{
    VOS_UINT32                    rst;
    NVIM_UeTdsSupportFreqBandList tdsFeature;
    VOS_UINT32                    i = 0;
    VOS_UINT32                    len;
    VOS_INT32                     bufLen;

    (VOS_VOID)memset_s(&tdsFeature, sizeof(tdsFeature), 0x00, sizeof(tdsFeature));

    rst = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_UTRAN_TDD_FREQ_BAND, &tdsFeature,
                            sizeof(NVIM_UeTdsSupportFreqBandList));
    if (rst != ERR_MSP_SUCCESS) {
        PS_PRINTF_WARNING("Read EN_NV_ID_TDS_SUPPORT_FREQ_BAND failed:0x%x.\n", NV_ITEM_UTRAN_TDD_FREQ_BAND);
        return ERR_MSP_FAILURE;
    }
    if (tdsFeature.bandCnt > NVIM_TDS_MAX_SUPPORT_BANDS_NUM) {
        PS_PRINTF_WARNING("Read NV_ITEM_UTRAN_TDD_FREQ_BAND stTdsFeature.ucBandCnt:%d.\n", tdsFeature.bandCnt);
        return ERR_MSP_FAILURE;
    }

    /* 读取NV成功，支持TDS */
    feATure[AT_FEATURE_TDSCDMA].featureFlag = AT_FEATURE_EXIST;

    len = 0;

    /* BandA:2000Hz, BanE:2300Hz, BandF:1900Hz */
    for (i = 0; i < tdsFeature.bandCnt; i++) {
        if (tdsFeature.bandNo[i] == ID_NVIM_TDS_FREQ_BAND_A) {
            bufLen = snprintf_s((VOS_CHAR *)(feATure[AT_FEATURE_TDSCDMA].content + len),
                                AT_FEATURE_CONTENT_LEN_MAX - len, (AT_FEATURE_CONTENT_LEN_MAX - len) - 1, "2000,");
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, AT_FEATURE_CONTENT_LEN_MAX - len,
                                             (AT_FEATURE_CONTENT_LEN_MAX - len) - 1);
            len = VOS_StrLen((VOS_CHAR *)feATure[AT_FEATURE_TDSCDMA].content);
            continue;
        }

        if (tdsFeature.bandNo[i] == ID_NVIM_TDS_FREQ_BAND_E) {
            bufLen = snprintf_s((VOS_CHAR *)(feATure[AT_FEATURE_TDSCDMA].content + len),
                                AT_FEATURE_CONTENT_LEN_MAX - len, (AT_FEATURE_CONTENT_LEN_MAX - len) - 1, "2300,");
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, AT_FEATURE_CONTENT_LEN_MAX - len,
                                             (AT_FEATURE_CONTENT_LEN_MAX - len) - 1);
            len = VOS_StrLen((VOS_CHAR *)feATure[AT_FEATURE_TDSCDMA].content);
            continue;
        }

        if (tdsFeature.bandNo[i] == ID_NVIM_TDS_FREQ_BAND_F) {
            bufLen = snprintf_s((VOS_CHAR *)(feATure[AT_FEATURE_TDSCDMA].content + len),
                                AT_FEATURE_CONTENT_LEN_MAX - len, (AT_FEATURE_CONTENT_LEN_MAX - len) - 1, "1900,");
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, AT_FEATURE_CONTENT_LEN_MAX - len,
                                             (AT_FEATURE_CONTENT_LEN_MAX - len) - 1);
            len = VOS_StrLen((VOS_CHAR *)feATure[AT_FEATURE_TDSCDMA].content);
            continue;
        }
    }

    if (len > 0) {
        /* 屏蔽掉最后一个逗号 */
        feATure[AT_FEATURE_TDSCDMA].content[len - 1] = '\0';
    }

    return ERR_MSP_SUCCESS;
}
#endif

#if (FEATURE_LTEV == FEATURE_ON)

VOS_VOID AT_GetLtevFeatureInfo(AT_FEATURE_SUPPORT_ST *featureSupport, MTA_AT_SfeatureQryCnf *featureQryCnf)
{
    VOS_UINT32 strLen = 0;
    VOS_UINT32 bandNum;
    VOS_UINT32 i;

    bandNum = AT_MIN(featureQryCnf->bandFeature.ltevBand.bandNum, MTA_AT_LTEV_BAND_MAX_NUM);
    if (bandNum != 0) {
        featureSupport[AT_FEATURE_LTEV].featureFlag = AT_FEATURE_EXIST;
    } else {
        return;
    }
    for (i = 0; i < bandNum; i++) {
        if (featureQryCnf->bandFeature.ltevBand.opBand[i] != 0) {
            strLen += (VOS_UINT32)AT_FormatReportString(AT_FEATURE_CONTENT_LEN_MAX,
                (VOS_CHAR *)(featureSupport[AT_FEATURE_LTEV].content),
                (VOS_CHAR *)(&featureSupport[AT_FEATURE_LTEV].content[strLen]), "B%d,",
                featureQryCnf->bandFeature.ltevBand.opBand[i]);
        }
    }
    /* 去掉最后一个逗号 */
    if (strLen > 0) {
        featureSupport[AT_FEATURE_LTEV].content[--strLen] = '\0';
    }
    return;
}
#endif


VOS_UINT32 AT_RcvMtaQrySfeatureRsp(struct MsgCB *msg)
{
    errno_t memResult;
    /* 初始化 */
    AT_MTA_Msg            *rcvMsg                = VOS_NULL_PTR;
    MTA_AT_SfeatureQryCnf *atAgentSfeatureQryCnf = VOS_NULL_PTR;
    AT_FEATURE_SUPPORT_ST *feATure               = VOS_NULL_PTR;
    VOS_UINT8              indexNum              = 0;
    VOS_UINT32             len;
    VOS_UINT32             reult;
    VOS_UINT8              feATrueNum;
    VOS_UINT8              indexTmp;
    VOS_INT                bufLen;
    const VOS_UINT8        feATureName[][AT_FEATURE_NAME_LEN_MAX] = {
        "LTE",  "HSPA+", "HSDPA",  "HSUPA", "DIVERSITY", "UMTS", "EVDO", "EDGE",
        "GPRS", "GSM",   "CDMA1X", "WIMAX", "WIFI",      "GPS",  "TD",

        "NR", "LTE-V"
    };

    rcvMsg                = (AT_MTA_Msg *)msg;
    atAgentSfeatureQryCnf = (MTA_AT_SfeatureQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQrySfeatureRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQrySfeatureRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SFEATURE_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SFEATURE_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (atAgentSfeatureQryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        reult                   = AT_ConvertMtaResult(atAgentSfeatureQryCnf->result);
        At_FormatResultData(indexNum, reult);
        return VOS_OK;
    }

    /*lint -save -e516 */
    feATure = (AT_FEATURE_SUPPORT_ST *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_FEATURE_MAX * sizeof(AT_FEATURE_SUPPORT_ST));
    /*lint -restore */
    if (feATure == VOS_NULL) {
        AT_WARN_LOG("AT_RcvMtaQrySfeatureRsp: GET MEM Fail!");
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }
    (VOS_VOID)memset_s(feATure, AT_FEATURE_MAX * sizeof(AT_FEATURE_SUPPORT_ST), 0x00,
                       AT_FEATURE_MAX * sizeof(AT_FEATURE_SUPPORT_ST));

    reult = AT_GetWFeatureInfo(feATure, atAgentSfeatureQryCnf);
    if (reult != VOS_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, feATure);
        /*lint -restore */
        return VOS_OK;
    }

#if (FEATURE_LTE == FEATURE_ON)
    reult = AT_GetLteFeatureInfo(feATure);
    if (reult != VOS_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, feATure);
        /*lint -restore */
        return VOS_OK;
    }
#endif
#if (FEATURE_LTEV == FEATURE_ON)
    AT_GetLtevFeatureInfo(feATure, atAgentSfeatureQryCnf);
#endif
#if (FEATURE_UE_MODE_TDS == FEATURE_ON)
    reult = AT_GetTdsFeatureInfo(feATure);
    if (reult != VOS_OK) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, feATure);
        /*lint -restore */
        return VOS_OK;
    }
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_GetNrFeatureInfo(atAgentSfeatureQryCnf->bandFeature.nrBand, feATure);
#endif

    AT_GetGFeatureInfo(feATure, atAgentSfeatureQryCnf);

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_SUPPORT) {
        /* WIFI */
        feATure[AT_FEATURE_WIFI].featureFlag = AT_FEATURE_EXIST;
        bufLen = snprintf_s((VOS_CHAR *)feATure[AT_FEATURE_WIFI].content, sizeof(feATure[AT_FEATURE_WIFI].content),
                            sizeof(feATure[AT_FEATURE_WIFI].content) - 1, "B,G,N");
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(feATure[AT_FEATURE_WIFI].content),
                                         sizeof(feATure[AT_FEATURE_WIFI].content) - 1);
    }

    /* 计算支持特性的个数 */
    feATrueNum = 0;
    for (indexTmp = 0; indexTmp < AT_FEATURE_MAX; indexTmp++) {
        memResult = memcpy_s(feATure[indexTmp].featureName, sizeof(feATure[indexTmp].featureName),
                             feATureName[indexTmp], sizeof(feATureName[indexTmp]));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(feATure[indexTmp].featureName), sizeof(feATureName[indexTmp]));
        if (feATure[indexTmp].featureFlag == AT_FEATURE_EXIST) {
            feATrueNum++;
        }
    }

    len = 0;
    /* 打印输出支持的特性数 */
    len = (TAF_UINT32)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + len, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName, feATrueNum,
        g_atCrLf);

    /* 分行打印输出支持的特性 */
    for (indexTmp = 0; indexTmp < AT_FEATURE_MAX; indexTmp++) {
        if (feATure[indexTmp].featureFlag == AT_FEATURE_EXIST) {
            len += (TAF_UINT32)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "%s:%s,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
                feATure[indexTmp].featureName, feATure[indexTmp].content, g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)(len - (VOS_UINT32)VOS_StrLen((VOS_CHAR *)g_atCrLf));

    At_FormatResultData(indexNum, AT_OK);
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, feATure);
    /*lint -restore */
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryProdtypeRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg            *rcvMsg      = VOS_NULL_PTR;
    DRV_AGENT_ProdtypeQryCnf *prodTypeCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum    = 0;

    /* 初始化消息 */
    rcvMsg      = (DRV_AGENT_Msg *)msg;
    prodTypeCnf = (DRV_AGENT_ProdtypeQryCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(prodTypeCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryProdtypeRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryProdtypeRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PRODTYPE_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PRODTYPE_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            prodTypeCnf->prodType);

    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_VOID At_ProcMsgFromDrvAgent(struct MsgCB *msg)
{
    DRV_AGENT_Msg *msgTemp = VOS_NULL_PTR;
    VOS_UINT32     i;
    VOS_UINT32     msgCnt;
    VOS_UINT32     msgId;
    VOS_UINT32     rst;

    /* 从g_drvAgentMsgProcTab中获取消息个数 */
    msgCnt  = sizeof(g_atProcMsgFromDrvAgentTab) / sizeof(AT_PROC_MsgFromDrvAgent);
    msgTemp = (DRV_AGENT_Msg *)msg;

    /* 从消息包中获取MSG ID */
    msgId = ((DRV_AGENT_Msg *)msgTemp)->msgId;

    /* g_drvAgentMsgProcTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromDrvAgentTab[i].msgType == msgId) {
            rst = g_atProcMsgFromDrvAgentTab[i].procMsgFunc((struct MsgCB *)msgTemp);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("At_ProcMsgFromDrvAgent: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_ProcMsgFromDrvAgent: Msg Id is invalid!");
    }

    return;
}


TAF_UINT32 At_PB_Unicode2UnicodePrint(TAF_UINT32 maxLength, TAF_INT8 *headaddr, TAF_UINT8 *dst, TAF_UINT8 *src,
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

        len += 2; /* 记录长度，两个高位长度为2 */
    }

    return len;
}


TAF_UINT16 At_PbGsmFormatPrint(TAF_UINT16 maxLength, TAF_UINT8 *dst, TAF_UINT8 *src, TAF_UINT16 srcLen)
{
    TAF_UINT32 i;

    if (srcLen > maxLength) {
        AT_ERR_LOG("At_PbGsmFormatPrint error: too long");

        return 0;
    }

    for (i = 0; i < srcLen; i++) {
        /* 将最高bit清零 */
        dst[i] = src[i] & AT_PB_GSM7_CODE_MAX_VALUE;
    }

    return srcLen;
}


TAF_UINT32 At_PbOneUnicodeToIra(TAF_UINT16 unicodeChar, TAF_UINT8 *dst)
{
    TAF_UINT16 indexNum;

    for (indexNum = 0; indexNum < AT_PB_IRA_MAX_NUM; indexNum++) {
        if (unicodeChar == g_iraToUnicode[indexNum].unicode) {
            *dst = g_iraToUnicode[indexNum].octet;

            return AT_SUCCESS;
        }
    }

    /* 未找到UCS2对应的IRA编码 */
    AT_LOG1("At_PbOneUnicodeToIra error: no found [%x]", unicodeChar);

    return AT_FAILURE;
}


TAF_UINT16 At_PbUnicodeToIraFormatPrint(TAF_UINT16 maxLength, TAF_UINT8 *dst, TAF_UINT8 *src, TAF_UINT16 srcLen)
{
    TAF_UINT16 indexNum;
    TAF_UINT16 retLen;
    TAF_UINT8 *data = src;
    TAF_UINT16 unicodeChar;

    /* IRA码流的长度是UCS2码流长度的一半 */
    retLen = srcLen >> 1;

    /* 检查存储空间是否足够 */
    if (retLen > maxLength) {
        AT_ERR_LOG("At_PbUnicodeToIraFormatPrint error: too long");

        return 0;
    }

    /* 逐个将UCS2字符通过查表转换为IRA编码 */
    for (indexNum = 0; indexNum < retLen; indexNum++) {
        /* 先将UINT8数据转为UINT16 */
        unicodeChar = (*data << 8) | (*(data + 1));
        data += AT_U16_BYTE_LEN;

        if (At_PbOneUnicodeToIra(unicodeChar, &dst[indexNum]) != AT_SUCCESS) {
            AT_ERR_LOG("At_PbUnicodeToIraFormatPrint error: no found");

            return 0;
        }
    }

    return retLen;
}


TAF_VOID At_PbUnicode82FormatPrint(TAF_UINT8 *puc82Code, VOS_UINT32 srcCodeLen,
                                   TAF_UINT8 *dst, VOS_UINT32 decBufLen, TAF_UINT16 *dstLen)
{
    TAF_UINT32  indexNum;
    TAF_UINT16  baseCode;
    TAF_UINT16  unicodeValue;
    TAF_UINT16  srcLen;
    TAF_UINT16  dstIndex = 0;
    TAF_UINT16  gsmCodeLen;
    TAF_UINT16  gsm2UCS2CodeLen;
    TAF_UINT16 *unicode = (TAF_UINT16 *)dst;
    TAF_UINT8  *src     = VOS_NULL_PTR;

    /* 取得82编码的长度 */
    srcLen = puc82Code[0];

    /* 取得82编码的Basecode */
    baseCode = (puc82Code[1] << 8) | puc82Code[2];

    if ((srcLen > srcCodeLen) || (baseCode == 0xFFFF)) { /* 数据长度错误 */
        *dstLen = 0;

        return;
    }

    /* 取得82编码的码流, 第四个及以后的Byte表示82压缩后的码流，加3是为了将指针偏移到第四个地址 */
    src = puc82Code + 3;

    /* 从第四个字节开始是82编码的码流 */
    for (indexNum = 0; indexNum < srcLen; indexNum++) {
        gsmCodeLen = 0;
        while (((indexNum + gsmCodeLen) < srcLen) && (src[indexNum + gsmCodeLen] <= AT_PB_GSM7_CODE_MAX_VALUE)) {
            gsmCodeLen++;
        }

        if (gsmCodeLen != 0) {
            gsm2UCS2CodeLen = 0;
            At_PbGsmToUnicode((src + indexNum), gsmCodeLen, (TAF_UINT8 *)(unicode + dstIndex),
                              (decBufLen - (dstIndex << 1)), &gsm2UCS2CodeLen);

            indexNum += (gsmCodeLen - 1);
            dstIndex += (gsm2UCS2CodeLen >> 1);

            continue;
        }

        unicodeValue = baseCode + (TAF_UINT16)(src[indexNum] & AT_PB_GSM7_CODE_MAX_VALUE);

        AT_UNICODE2VALUE(unicodeValue);

        unicode[dstIndex] = unicodeValue;
        dstIndex++;
    }

    *dstLen = (TAF_UINT16)(dstIndex << 1);

    return;
}


TAF_VOID At_PbUnicode81FormatPrint(TAF_UINT8 *puc81Code, VOS_UINT32 srcCodeLen,
                                   TAF_UINT8 *dst, VOS_UINT32 decBufLen, TAF_UINT16 *dstLen)
{
    TAF_UINT32  indexNum;
    TAF_UINT16  unicodeValue;
    TAF_UINT16  baseCode;
    TAF_UINT16  srcLen;
    TAF_UINT16  dstIndex = 0;
    TAF_UINT16  gsmCodeLen;
    TAF_UINT16  gsm2UCS2CodeLen;
    TAF_UINT16 *unicode = (TAF_UINT16 *)dst;
    TAF_UINT8  *src     = VOS_NULL_PTR;

    if (puc81Code[0] > srcCodeLen) { /* 字符个数不能大于姓名的最大值 */
        *dstLen = 0;

        return;
    }

    /* 取得81编码的长度 */
    srcLen = puc81Code[0];

    /* 取得81编码的基指针 */
    baseCode = puc81Code[1] << 7;

    /* 取得81编码的码流, 第三个及以后的Byte表示81压缩后的码流，加2是为了将指针偏移到第三个地址 */
    src = puc81Code + 2;

    /* 从第三个字节开始是81编码的码流 */
    for (indexNum = 0; indexNum < srcLen; indexNum++) {
        gsmCodeLen = 0;
        while ((src[indexNum + gsmCodeLen] <= AT_PB_GSM7_CODE_MAX_VALUE) && ((indexNum + gsmCodeLen) < srcLen)) {
            gsmCodeLen++;
        }

        if (gsmCodeLen != 0) {
            gsm2UCS2CodeLen = 0;
            At_PbGsmToUnicode((src + indexNum), gsmCodeLen, (TAF_UINT8 *)(unicode + dstIndex),
                              (decBufLen - (dstIndex << 1)), &gsm2UCS2CodeLen);

            indexNum += (gsmCodeLen - 1);
            dstIndex += (gsm2UCS2CodeLen >> 1);

            continue;
        }

        /* 81编码先将第8bit置零再与基本码相加 */
        unicodeValue = baseCode + (TAF_UINT16)(src[indexNum] & AT_PB_GSM7_CODE_MAX_VALUE);

        AT_UNICODE2VALUE(unicodeValue);

        unicode[dstIndex] = unicodeValue;
        dstIndex++;
    }

    *dstLen = (TAF_UINT16)(dstIndex << 1);

    return;
}


TAF_VOID At_PbUnicode80FormatPrint(TAF_UINT8 *src, TAF_UINT16 srcLen, TAF_UINT8 *dst, VOS_UINT32 decMaxBufLen, TAF_UINT16 *dstLen)
{
    errno_t memResult;

    if ((src[0] == 0xFF) && (src[1] == 0xFF)) { /* 当首字符为FFFF，认为当前姓名为空 */
        *dstLen = 0;
    } else {
        *dstLen = 0;

        /* 跳过80标志位 */
        if (srcLen <= decMaxBufLen) {
            if (srcLen > 0) {
                memResult = memcpy_s(dst, decMaxBufLen, src, srcLen);
                TAF_MEM_CHK_RTN_VAL(memResult, decMaxBufLen, srcLen);
            }

            *dstLen = srcLen;
        }
    }

    return;
}


TAF_UINT32 At_PbGsmExtToUnicode(TAF_UINT8 gsmExtChar, TAF_UINT16 *unicodeChar)
{
    TAF_UINT32 indexNum;

    /* 查找GSM到UNICODE扩展表，找到则返回成功，否则返回失败 */
    for (indexNum = 0; indexNum < AT_PB_GSM7EXT_MAX_NUM; indexNum++) {
        if (gsmExtChar == g_gsm7extToUnicode[indexNum].octet) {
            *unicodeChar = g_gsm7extToUnicode[indexNum].unicode;

            return AT_SUCCESS;
        }
    }

    AT_LOG1("At_PbGsmExtToUnicode warning: no found [0x%x]", gsmExtChar);

    return AT_FAILURE;
}


TAF_VOID At_PbGsmToUnicode(TAF_UINT8 *src, TAF_UINT16 srcLen, TAF_UINT8 *dst, VOS_UINT32 dstBufLen, TAF_UINT16 *dstLen)
{
    TAF_UINT32  indexNum;
    TAF_UINT16  unicodeLen = 0;
    TAF_UINT16  unicodeChar;
    VOS_UINT32  dstBufUniLen = dstBufLen >> 1;
    TAF_UINT16 *unicode      = (TAF_UINT16 *)dst;

    for (indexNum = 0; ((indexNum < srcLen) && (unicodeLen < dstBufUniLen)); indexNum++) {
        /* 为兼容有些平台再GSM模式下写入记录时未对最高BIT置零 */
        src[indexNum] = src[indexNum] & AT_PB_GSM7_CODE_MAX_VALUE;

        /* 非0x1B字符，直接查GSM到UNICODE基本表 */
        if (src[indexNum] != AT_PB_GSM7EXT_SYMBOL) {
            unicodeChar = g_gsmToUnicode[src[indexNum]].unicode;
            AT_UNICODE2VALUE(unicodeChar);
            TAF_PUT_HOST_UINT16(unicode, unicodeChar);
            unicode++;
            unicodeLen++;
            continue;
        }

        /* 当前GSM编码为0x1b时,可能为扩展标志 */
        if ((srcLen - indexNum) < AT_HEX_0X_LENGTH) {
            /* 最后一个字节为0x1B，查GSM到UNICODE基本表, 将0x1B译为SPACE */
            unicodeChar = g_gsmToUnicode[src[indexNum]].unicode;
            AT_UNICODE2VALUE(unicodeChar);
            TAF_PUT_HOST_UINT16(unicode, unicodeChar);
            unicode++;
            unicodeLen++;
            continue;
        }

        /*
         * 对于0x1B 0x1B的情况，因为我们不支持第三张扩展表，因此直接将0x1B
         * 0x1B译为SPACE SPACE
         */
        if (src[indexNum + 1] == AT_PB_GSM7EXT_SYMBOL) {
            unicodeChar = g_gsmToUnicode[src[indexNum]].unicode;
            AT_UNICODE2VALUE(unicodeChar);
            TAF_PUT_HOST_UINT16(unicode, unicodeChar);
            unicode++;

            unicodeChar = g_gsmToUnicode[src[indexNum + 1]].unicode;
            AT_UNICODE2VALUE(unicodeChar);
            TAF_PUT_HOST_UINT16(unicode, unicodeChar);
            unicode++;
            indexNum++;
            unicodeLen += AT_HEX_0X_LENGTH;
            continue;
        }

        /* 0x1B可能为标志，查找GSM到UNICODE扩展表 */
        if (At_PbGsmExtToUnicode(src[indexNum + 1], &unicodeChar) == AT_SUCCESS) {
            AT_UNICODE2VALUE(unicodeChar);
            TAF_PUT_HOST_UINT16(unicode, unicodeChar);
            unicode++;
            indexNum++;
            unicodeLen++;
            continue;
        }

        /* 对于1BXX，未在扩展表中，且XX不为0x1B的情况，译为SPACE+XX对应的字符 */
        unicodeChar = g_gsmToUnicode[src[indexNum]].unicode;
        AT_UNICODE2VALUE(unicodeChar);
        TAF_PUT_HOST_UINT16(unicode, unicodeChar);
        unicode++;
        unicodeLen++;
    }

    /* GSM7BIT 码流中可能有扩展表中字符，GSM7bit到UNICODE转换长度不一定是原码流长度的2倍 */
    *dstLen = (TAF_UINT16)(unicodeLen << 1);

    return;
}


TAF_VOID At_PbRecordToUnicode(SI_PB_EventInfo *event, TAF_UINT8 *decode, VOS_UINT32 decBufLen, TAF_UINT16 *decodeLen)
{
    TAF_UINT16 decodeLenTemp = 0;

    switch (event->pbEvent.pbReadCnf.pbRecord.alphaTagType) {
        case SI_PB_ALPHATAG_TYPE_GSM:
            At_PbGsmToUnicode(event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                              event->pbEvent.pbReadCnf.pbRecord.alphaTagLength, decode, decBufLen, &decodeLenTemp);
            break;
        case SI_PB_ALPHATAG_TYPE_UCS2_80:
            At_PbUnicode80FormatPrint(event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                      event->pbEvent.pbReadCnf.pbRecord.alphaTagLength,
                                      decode,
                                      decBufLen,
                                      &decodeLenTemp);
            break;
        case SI_PB_ALPHATAG_TYPE_UCS2_81:
            At_PbUnicode81FormatPrint(event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                      sizeof(event->pbEvent.pbReadCnf.pbRecord.alphaTag),
                                      decode, decBufLen, &decodeLenTemp);
            break;
        case SI_PB_ALPHATAG_TYPE_UCS2_82:
            At_PbUnicode82FormatPrint(event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                      sizeof(event->pbEvent.pbReadCnf.pbRecord.alphaTag),
                                      decode, decBufLen, &decodeLenTemp);
            break;
        default:
            AT_ERR_LOG("At_PbRecordToUnicode error: not support code type");

            break;
    }

    *decodeLen = decodeLenTemp;

    return;
}


TAF_UINT32 At_Pb_AlaphPrint(TAF_UINT16 *dataLen, SI_PB_EventInfo *event, TAF_UINT8 *data)
{
    TAF_UINT16 length                       = *dataLen;
    TAF_UINT16 decodeLen                    = 0;
    TAF_UINT16 returnLen                    = 0;
    TAF_UINT8  decode[AT_DECODE_MAX_LENGTH] = {0};

    if (g_atCscsType == AT_CSCS_GSM_7Bit_CODE) {
        /* 在GSM字符集下，不支持UCS2编码 */
        if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_80) ||
            (event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_81) ||
            (event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_82)) {
            AT_LOG1("At_Pb_AlaphPrint error: GSM7 doesn't read UCS code[%d]",
                    event->pbEvent.pbReadCnf.pbRecord.alphaTagType);
            return AT_FAILURE;
        }

        returnLen = At_PbGsmFormatPrint((AT_CMD_MAX_LEN - length), (data + length),
                                        event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                        event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);
    } else { /* 当前字符集为UCS2或IRA时 */
        /* 当前字符集为UCS2或IRA时，先根据存储格式转换为UCS2编码 */
        At_PbRecordToUnicode(event, decode, sizeof(decode), &decodeLen);

        /* 当前字符集为IRA时，将UCS2转换为IRA码流输出 */
        if (g_atCscsType == AT_CSCS_IRA_CODE) {
            returnLen = At_PbUnicodeToIraFormatPrint((AT_CMD_MAX_LEN - length), (data + length), decode, decodeLen);
        }

        /* 当前字符集为UCS2时，将UCS2转换为打印格式输出 */
        if (g_atCscsType == AT_CSCS_UCS2_CODE) {
            returnLen = (TAF_UINT16)At_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)data, (data + length), decode,
                                                            decodeLen);
        }
    }

    if (returnLen == 0) {
        g_atSendDataBuff.bufLen = 0;

        AT_LOG1("At_Pb_AlaphPrint error: usReturnLen = 0,g_atCscsType = [%d]", g_atCscsType);

        return AT_FAILURE;
    }

    length += returnLen;
    *dataLen = length;

    return AT_SUCCESS;
}


TAF_UINT32 At_Pb_CnumAlaphPrint(TAF_UINT16 *dataLen, SI_PB_EventInfo *event, TAF_UINT8 *data)
{
    TAF_UINT16 length                       = *dataLen;
    TAF_UINT16 decodeLen                    = 0;
    TAF_UINT16 returnLen                    = 0;
    TAF_UINT8  decode[AT_DECODE_MAX_LENGTH] = {0};

    if (g_atCscsType == AT_CSCS_GSM_7Bit_CODE) {
        /* 在GSM字符集下，不支持UCS2编码 */
        if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_80) ||
            (event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_81) ||
            (event->pbEvent.pbReadCnf.pbRecord.alphaTagType == SI_PB_ALPHATAG_TYPE_UCS2_82)) {
            AT_LOG1("At_Pb_AlaphPrint error: GSM7 doesn't read UCS code[%d]",
                    event->pbEvent.pbReadCnf.pbRecord.alphaTagType);
            return AT_SUCCESS;
        }

        returnLen = At_PbGsmFormatPrint((AT_CMD_MAX_LEN - length), (data + length),
                                        event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                        event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);
    } else { /* 当前字符集为UCS2或IRA时 */
        /* 当前字符集为UCS2或IRA时，先根据存储格式转换为UCS2编码 */
        At_PbRecordToUnicode(event, decode, sizeof(decode), &decodeLen);

        /* 当前字符集为IRA时，将UCS2转换为IRA码流输出 */
        if (g_atCscsType == AT_CSCS_IRA_CODE) {
            returnLen = At_PbUnicodeToIraFormatPrint((AT_CMD_MAX_LEN - length), (data + length), decode, decodeLen);
        }

        /* 当前字符集为UCS2时，将UCS2转换为打印格式输出 */
        if (g_atCscsType == AT_CSCS_UCS2_CODE) {
            returnLen = (TAF_UINT16)At_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)data, (data + length), decode,
                                                            decodeLen);
        }
    }

    if (returnLen == 0) {
        AT_LOG1("At_Pb_AlaphPrint error: usReturnLen = 0,g_atCscsType = [%d]", g_atCscsType);

        return AT_SUCCESS;
    }

    length += returnLen;
    *dataLen = length;

    return AT_SUCCESS;
}


TAF_VOID AT_Pb_NumberPrint(TAF_UINT16 *dataLen, SI_PB_EventInfo *event, TAF_UINT8 *data)
{
    errno_t    memResult;
    TAF_UINT16 length = *dataLen;

    if (event->pbEvent.pbReadCnf.pbRecord.numberLength == 0) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length,
            ",\"\",129");
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length,
            ",\"");

        if (event->pbEvent.pbReadCnf.pbRecord.numberType == PB_NUMBER_TYPE_INTERNATIONAL) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length,
                "+");
        }

        if (event->pbEvent.pbReadCnf.pbRecord.numberLength <= (AT_SEND_CRLF_ADDR_MAX_LEN - length)) {
            memResult = memcpy_s(data + length, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                 event->pbEvent.pbReadCnf.pbRecord.number,
                                 event->pbEvent.pbReadCnf.pbRecord.numberLength);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                event->pbEvent.pbReadCnf.pbRecord.numberLength);

            length += (TAF_UINT16)event->pbEvent.pbReadCnf.pbRecord.numberLength;
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "\"");

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, ",%d",
            event->pbEvent.pbReadCnf.pbRecord.numberType);
    }

    *dataLen = length;

    return;
}


TAF_UINT32 At_PbCNUMCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event)
{
    TAF_UINT16 length = *dataLen;
    TAF_UINT32 result;

    if (event->pbEvent.pbReadCnf.pbRecord.validFlag == SI_PB_CONTENT_INVALID) {
        return AT_SUCCESS;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "%s: ",
        g_parseContext[indexNum].cmdElement->cmdName);

    /* 注意:内容需要根据编码类型进行输出，如果是ASCII码直接打印，如果UNICODE需要转换 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "\"");

    if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength != 0) &&
        (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= SI_PB_ALPHATAG_MAX_LEN)) {
        result = At_Pb_CnumAlaphPrint(&length, event, data);

        if (result != AT_SUCCESS) {
            At_FormatResultData(indexNum, AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING);

            return AT_FAILURE;
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "\"");

    /* 将电话号码及号码类型处理封装为函数 */
    AT_Pb_NumberPrint(&length, event, data);

    *dataLen = length;

    return AT_SUCCESS;
}


TAF_UINT32 At_PbCPBR2CmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event)
{
    TAF_UINT16 length = *dataLen;
    TAF_UINT32 result;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "%s: ",
        g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "%d",
        event->pbEvent.pbReadCnf.pbRecord.index);

    /* 将电话号码及号码类型处理封装为函数 */
    AT_Pb_NumberPrint(&length, event, data);

    /* 注意:内容需要根据编码类型进行输出，如果是ASCII码直接打印，如果UNICODE需要转换 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, ",\"");

    if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength != 0) &&
        (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= SI_PB_ALPHATAG_MAX_LEN)) {
        result = At_Pb_AlaphPrint(&length, event, data);

        if (result != AT_SUCCESS) {
            At_FormatResultData(indexNum, AT_CME_INVALID_CHARACTERS_IN_TEXT_STRING);

            return AT_FAILURE;
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "\"");

    *dataLen = length;

    return AT_SUCCESS;
}


TAF_UINT32 At_PbCPBRCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, TAF_UINT8 *data, SI_PB_EventInfo *event)
{
    errno_t    memResult;
    TAF_UINT16 length = *dataLen;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "%s: ",
        g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "%d",
        event->pbEvent.pbReadCnf.pbRecord.index);

    /* 将电话号码及号码类型处理封装为函数 */
    AT_Pb_NumberPrint(&length, event, data);

    /* 注意:内容需要根据编码类型进行输出，如果是ASCII码直接打印，如果UNICODE需要转换 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, ",\"");

    if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength != 0) &&
        (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= SI_PB_ALPHATAG_MAX_LEN)) {
        if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType & 0x80) == SI_PB_ALPHATAG_TYPE_UCS2) { /* DATA:IRA */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length,
                "%X", event->pbEvent.pbReadCnf.pbRecord.alphaTagType);

            length += (TAF_UINT16)At_PB_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)data, data + length,
                                                             event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                                             event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);

            if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength % 2) == 0) { /* 对于偶数个中文需要后面补充FF */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data,
                    (TAF_CHAR *)data + length, "FF");
            }
        } else { /* DATA:UCS2 */
            if (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= (AT_SEND_CRLF_ADDR_MAX_LEN - length)) {
                memResult = memcpy_s((TAF_CHAR *)data + length, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                     event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                     event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                    event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);

                length += event->pbEvent.pbReadCnf.pbRecord.alphaTagLength;
            }
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, "\"");

    if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType & 0x80) == SI_PB_ALPHATAG_TYPE_UCS2) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, ",1");
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)data, (TAF_CHAR *)data + length, ",0");
    }

    *dataLen = length;

    return AT_SUCCESS;
}


TAF_VOID At_PbEmailPrint(TAF_UINT8 *src, TAF_UINT16 srcLen, TAF_UINT8 *dst, TAF_UINT16 *dstLen)
{
    TAF_UINT8  uCS2Code[2 * SI_PB_EMAIL_MAX_LEN];
    TAF_UINT16 uCS2CodeLen;
    TAF_UINT16 eMailLen = srcLen;

    TAF_UINT16 returnLen;

    (VOS_VOID)memset_s(uCS2Code, sizeof(uCS2Code), 0x00, sizeof(uCS2Code));

    /* 对EMAIL长度大于64Byte，进行截断处理 */
    if (eMailLen > SI_PB_EMAIL_MAX_LEN) {
        eMailLen = SI_PB_EMAIL_MAX_LEN;
    }

    /* 先将GSM模式转成UCS2模式 */
    At_PbGsmToUnicode(src, eMailLen, uCS2Code, sizeof(uCS2Code), &uCS2CodeLen);

    /* 再将UCS2模式转成IRA模式 */
    returnLen = At_PbUnicodeToIraFormatPrint(srcLen, dst, uCS2Code, uCS2CodeLen);
    if (returnLen == 0) {
        AT_INFO_LOG("At_PbEmailPrint error: usReturnLen = 0");
        return;
    }

    *dstLen = (TAF_UINT16)(uCS2CodeLen >> 1);

    return;
}


TAF_UINT32 At_PbSCPBRCmdPrint(VOS_UINT8 indexNum, TAF_UINT16 *dataLen, SI_PB_EventInfo *event)
{
    errno_t    memResult;
    TAF_UINT16 length = *dataLen;
    TAF_UINT32 i;
    TAF_UINT16 emailLen = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, "%d", event->pbEvent.pbReadCnf.pbRecord.index);

    /* 将电话号码及号码类型处理封装为函数 */
    AT_Pb_NumberPrint(&length, event, g_atSndCrLfAddr);

    for (i = 0; i < AT_ADDITION_NUMBER_MAX_NUM; i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
            (TAF_CHAR *)g_atSndCrLfAddr + length, ",\"");

        /* +2是为了防止写越界 */
        if ((event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength == 0) ||
            (event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength > (SI_PB_PHONENUM_MAX_LEN + 2))) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr + length, "\",129");

            continue;
        }

        if (event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberType == PB_NUMBER_TYPE_INTERNATIONAL) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr + length, "+");
        }

        if (event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength <= (AT_SEND_CRLF_ADDR_MAX_LEN - length)) {
            memResult = memcpy_s(g_atSndCrLfAddr + length, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                 event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].number,
                                 event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength);

            length += (TAF_UINT16)event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberLength;
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
            (TAF_CHAR *)g_atSndCrLfAddr + length, "\"");

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
            (TAF_CHAR *)g_atSndCrLfAddr + length, ",%d",
            event->pbEvent.pbReadCnf.pbRecord.additionNumber[i].numberType);
    }

    /* 注意:内容需要根据编码类型进行输出，如果是ASCII码直接打印，如果UNICODE需要转换 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, ",\"");

    if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength != 0) &&
        (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= SI_PB_ALPHATAG_MAX_LEN)) {
        if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType & 0x80) == SI_PB_ALPHATAG_TYPE_UCS2) { /* DATA:IRA */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr + length, "%X", event->pbEvent.pbReadCnf.pbRecord.alphaTagType);

            length += (TAF_UINT16)At_PB_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCrLfAddr,
                                                             g_atSndCrLfAddr + length,
                                                             event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                                             event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);

            if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagLength % 2) == 0) { /* 对于偶数个中文需要后面补充FF */
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                    (TAF_CHAR *)g_atSndCrLfAddr + length, "FF");
            }
        } else { /* DATA:UCS2 */
            if (event->pbEvent.pbReadCnf.pbRecord.alphaTagLength <= (AT_SEND_CRLF_ADDR_MAX_LEN - length)) {
                memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr + length, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                     event->pbEvent.pbReadCnf.pbRecord.alphaTag,
                                     event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN - length,
                                    event->pbEvent.pbReadCnf.pbRecord.alphaTagLength);

                length += event->pbEvent.pbReadCnf.pbRecord.alphaTagLength;
            }
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, "\"");

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SCPBR_SET) {
        if ((event->pbEvent.pbReadCnf.pbRecord.alphaTagType & 0x80) == SI_PB_ALPHATAG_TYPE_UCS2) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr + length, ",1");
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr + length, ",0");
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, ",\"");

    if (event->pbEvent.pbReadCnf.pbRecord.email.emailLen != 0) {
        At_PbEmailPrint(event->pbEvent.pbReadCnf.pbRecord.email.email,
                        (TAF_UINT16)event->pbEvent.pbReadCnf.pbRecord.email.emailLen,
                        (TAF_UINT8 *)(g_atSndCrLfAddr + length), &emailLen);

        length += emailLen;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
        (TAF_CHAR *)g_atSndCrLfAddr + length, "\"");

    *dataLen = length;

    return AT_SUCCESS;
}

/*
 * Description: 电话簿查找数据打印函数
 *  1.Date: 2009-03-14
 *    Author:
 *    Modification: Created function
 */
TAF_VOID At_PbSearchCnfProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event)
{
    errno_t    memResult;
    TAF_UINT16 length = 0;

    g_pbPrintTag = TAF_TRUE;

    if (event->pbEvent.pbSearchCnf.pbRecord.validFlag == SI_PB_CONTENT_INVALID) { /* 当前的内容无效 */
        return;                                                                   /* 返回不处理 */
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", event->pbEvent.pbSearchCnf.pbRecord.index);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",\"");

    if (event->pbEvent.pbSearchCnf.pbRecord.numberType == PB_NUMBER_TYPE_INTERNATIONAL) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "+");
    }

    if (event->pbEvent.pbSearchCnf.pbRecord.numberLength <= (AT_SEND_CODE_ADDR_MAX_LEN - length)) {
        if (event->pbEvent.pbSearchCnf.pbRecord.numberLength > 0) {
            memResult = memcpy_s(g_atSndCodeAddress + length, AT_SEND_CODE_ADDR_MAX_LEN - length,
                                 event->pbEvent.pbSearchCnf.pbRecord.number,
                                 event->pbEvent.pbSearchCnf.pbRecord.numberLength);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CODE_ADDR_MAX_LEN - length,
                                event->pbEvent.pbSearchCnf.pbRecord.numberLength);
        }

        length += (TAF_UINT16)event->pbEvent.pbSearchCnf.pbRecord.numberLength;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d,", event->pbEvent.pbSearchCnf.pbRecord.numberType);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    if ((event->pbEvent.pbSearchCnf.pbRecord.alphaTagType & 0x80) == SI_PB_ALPHATAG_TYPE_UCS2) { /* DATA:IRA */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X", event->pbEvent.pbSearchCnf.pbRecord.alphaTagType);

        length += (TAF_UINT16)At_PB_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         g_atSndCodeAddress + length,
                                                         event->pbEvent.pbSearchCnf.pbRecord.alphaTag,
                                                         event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength);

        if ((event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength % 2) == 0) { /* 对于奇数个中文需要后面补充FF */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "FF");
        }
    } else {                                     /* DATA:UCS2 */
        if (g_atCscsType == AT_CSCS_UCS2_CODE) { /* +CSCS:UCS2 */
            length += (TAF_UINT16)At_Ascii2UnicodePrint(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                        g_atSndCodeAddress + length,
                                                        event->pbEvent.pbSearchCnf.pbRecord.alphaTag,
                                                        event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength);
        } else {
            if (event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength <= (AT_SEND_CODE_ADDR_MAX_LEN - length)) {
                if (event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength > 0) {
                    memResult = memcpy_s((TAF_CHAR *)g_atSndCodeAddress + length, AT_SEND_CODE_ADDR_MAX_LEN - length,
                                         event->pbEvent.pbSearchCnf.pbRecord.alphaTag,
                                         event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength);
                    TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CODE_ADDR_MAX_LEN - length,
                                        event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength);
                }

                length += (TAF_UINT16)event->pbEvent.pbSearchCnf.pbRecord.alphaTagLength;
            }
        }
    }
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    if (g_atVType == AT_V_ENTIRE_TYPE) {
        memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr, AT_SEND_CRLF_ADDR_MAX_LEN, (TAF_CHAR *)g_atCrLf,
                             AT_SYMBOL_CR_LF_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN, AT_SYMBOL_CR_LF_LEN);
        At_SendResultData(indexNum, g_atSndCrLfAddr, length + AT_SYMBOL_CR_LF_LEN);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, length);
    }
}


TAF_UINT32 At_PbReadCnfProc(VOS_UINT8 indexNum, SI_PB_EventInfo *event)
{
    errno_t    memResult;
    VOS_UINT32 result;
    VOS_UINT16 pBReadPrintLength;
    result            = AT_FAILURE;
    pBReadPrintLength = 0;

    if (event->pbEvent.pbReadCnf.pbRecord.validFlag == SI_PB_CONTENT_INVALID) { /* 当前的内容无效 */
        return AT_SUCCESS;                                                      /* 返回不处理 */
    } else {
        if (g_pbPrintTag == TAF_FALSE) {
            pBReadPrintLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
                (TAF_CHAR *)g_atSndCrLfAddr, "%s", "\r\n");
        }

        g_pbPrintTag = TAF_TRUE;

        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR_SET) { /* 按照 ^CPBR 的方式进行打印 */
            result = At_PbCPBRCmdPrint(indexNum, &pBReadPrintLength, g_atSndCrLfAddr, event);
        } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CPBR2_SET) { /* 按照 +CPBR 的方式进行打印 */
            result = At_PbCPBR2CmdPrint(indexNum, &pBReadPrintLength, g_atSndCrLfAddr, event);
        } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SCPBR_SET) { /* 按照 ^SCPBR 的方式进行打印 */
            result = At_PbSCPBRCmdPrint(indexNum, &pBReadPrintLength, event);
        } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CNUM_READ) { /* 按照 CNUM 的方式进行打印 */
            result = At_PbCNUMCmdPrint(indexNum, &pBReadPrintLength, g_atSndCrLfAddr, event);
        } else {
            AT_ERR_LOG1("At_PbReadCnfProc: the Cmd Current Opt %d is Unknow", g_atClientTab[indexNum].cmdCurrentOpt);

            return AT_FAILURE;
        }
    }

    if (result == AT_SUCCESS) {
        pBReadPrintLength += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCrLfAddr,
            (TAF_CHAR *)g_atSndCrLfAddr + pBReadPrintLength, "%s", "\r\n");
    }

    At_SendResultData(indexNum, g_atSndCrLfAddr, pBReadPrintLength);

    memResult = memcpy_s((TAF_CHAR *)g_atSndCrLfAddr, AT_SEND_CRLF_ADDR_MAX_LEN, (TAF_CHAR *)g_atCrLf,
                         AT_SYMBOL_CR_LF_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CRLF_ADDR_MAX_LEN, AT_SYMBOL_CR_LF_LEN);

    return result;
}


MODULE_EXPORTED TAF_VOID At_PbCallBackFunc(SI_PB_EventInfo *event)
{
    TAF_UINT32 sendMsg  = 0;
    TAF_UINT8  indexNum = 0;

    if (event == TAF_NULL_PTR) { /* 参数错误 */
        AT_WARN_LOG("At_PbCallBackFunc pEvent NULL");

        return;
    }

    if (At_ClientIdToUserId(event->clientId, &indexNum) == AT_FAILURE) { /* 回复的客户端内容错误 */
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_PbMsgProc : AT_BROADCAST_INDEX.");
        return;
    }

    if (g_atClientTab[indexNum].cmdIndex == AT_CMD_CNUM) {
        event->pbError = ((event->pbError == TAF_ERR_PB_NOT_FOUND) ? TAF_ERR_NO_ERROR : event->pbError);
    }

    if (event->pbError != TAF_ERR_NO_ERROR) { /* 出错，发送消息处理 */
        sendMsg = SI_TRUE;
    } else if (((event->pbEventType == SI_PB_EVENT_READ_CNF) || (event->pbEventType == SI_PB_EVENT_SREAD_CNF))
               /* 由于读取的特殊要求，因此不能通过一次性的消息发送数据进行处理 */
               && (event->pbLastTag != SI_PB_LAST_TAG_TRUE)) {
        if (At_PbReadCnfProc(indexNum, event) != AT_SUCCESS) {
            AT_WARN_LOG("At_PbCallBackFunc:At_PbReadCnfProc Error");
        }

        sendMsg = SI_FALSE;
    } else if (event->pbEventType == SI_PB_EVENT_SEARCH_CNF) {
        if (event->pbLastTag == SI_PB_LAST_TAG_TRUE) {
            sendMsg = SI_TRUE;
        } else {
            At_PbSearchCnfProc(indexNum, event);
            return;
        }
    } else { /* 其它的回复可以通过消息进行一次性发送处理 */
        sendMsg = SI_TRUE;
    }

    if (sendMsg == SI_TRUE) {
        At_SendReportMsg(MN_CALLBACK_PHONE_BOOK, (TAF_UINT8 *)event, sizeof(SI_PB_EventInfo));
    }

    return;
}


VOS_VOID At_RcvVcMsgSetGroundCnfProc(MN_AT_IndEvt *data)
{
    VOS_UINT8  indexNum = 0;
    VOS_UINT32 ret;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgSetGroundCnfProc:WARNING:AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvVcMsgSetGroundCnfProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBG_SET) {
        AT_WARN_LOG("At_RcvVcMsgSetGroundCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    if (data->content[0] == VOS_OK) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_RcvVcMsgQryGroundRspProc(MN_AT_IndEvt *data)
{
    APP_VC_QryGroungRsp *qryRslt  = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;
    VOS_UINT32           ret;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgQryGroundRspProc:WARNING:AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvVcMsgQryGroundRspProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBG_READ) {
        AT_WARN_LOG("At_RcvVcMsgQryGroundRspProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    /* 初始化 */
    qryRslt = (APP_VC_QryGroungRsp *)data->content;

    if (qryRslt->qryRslt == VOS_OK) {
        /* 输出查询结果 */
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "^CBG:%d", qryRslt->ground);
        ret                     = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_RcvVcMsgQryTTYModeCnfProc(MN_AT_IndEvt *data)
{
    APP_VC_QryTtymodeCnf *tTYMode = VOS_NULL_PTR;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum = 0;

    /* 初始化 */
    tTYMode = (APP_VC_QryTtymodeCnf *)data->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgQryTTYModeCnfProc: WARNING: AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvVcMsgQryTTYModeCnfProc: AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TTYMODE_READ) {
        AT_WARN_LOG("At_RcvVcMsgQryTTYModeCnfProc: WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    /* 查询的TTY MODE */
    if (tTYMode->qryRslt == VOS_OK) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %u", g_parseContext[indexNum].cmdElement->cmdName,
                tTYMode->ttyMode);
        ret = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_RcvVcMsgSetTTYModeCnfProc(MN_AT_IndEvt *data)
{
    VOS_UINT32 ret;
    VOS_UINT8 *result   = VOS_NULL_PTR;
    VOS_UINT8  indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgSetTTYModeCnfProc: WARNING: AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvVcMsgSetTTYModeCnfProc: AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TTYMODE_SET) {
        AT_WARN_LOG("At_RcvVcMsgSetTTYModeCnfProc: WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    result = data->content;
    if (*result == VOS_OK) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return;
}


VOS_VOID At_ProcMsgFromVc(struct MsgCB *msg)
{
    MN_AT_IndEvt *msgTemp = VOS_NULL_PTR;

    msgTemp = (MN_AT_IndEvt *)msg;

    switch (msgTemp->msgName) {
        case APP_VC_MSG_CNF_QRY_MODE:
            At_RcvVcMsgQryModeCnfProc(msgTemp);
            break;

        case APP_VC_MSG_SET_FOREGROUND_CNF:
        case APP_VC_MSG_SET_BACKGROUND_CNF:
            /* AT 处理 VC 上报的前台模式、后台模式CNF MSG */
            At_RcvVcMsgSetGroundCnfProc(msgTemp);
            break;

        case APP_VC_MSG_FOREGROUND_RSP:
            /* AT 处理VC 上报的查询前台模式回复消息 */
            At_RcvVcMsgQryGroundRspProc(msgTemp);
            break;
        case APP_VC_MSG_QRY_TTYMODE_CNF:
            At_RcvVcMsgQryTTYModeCnfProc(msgTemp);
            break;
        case APP_VC_MSG_SET_TTYMODE_CNF:
            At_RcvVcMsgSetTTYModeCnfProc(msgTemp);
            break;

#if (FEATURE_ECALL == FEATURE_ON)
        case APP_VC_MSG_SET_MSD_CNF:
            AT_RcvVcMsgSetMsdCnfProc(msgTemp);
            break;

        case APP_VC_MSG_QRY_MSD_CNF:
            AT_RcvVcMsgQryMsdCnfProc(msgTemp);
            break;

        case APP_VC_MSG_QRY_ECALL_CFG_CNF:
            AT_RcvVcMsgQryEcallCfgCnfProc(msgTemp);
            break;
        case APP_VC_MSG_SET_ECALL_PUSH_CNF:
            AT_RcvVcMsgEcallPushCnfProc(msgTemp);
            break;
#endif

        case APP_VC_MSG_DTMF_DECODER_IND:
            At_RcvVcMsgDtmfDecoderIndProc(msgTemp);
            break;

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
        case APP_VC_MSG_I2S_TEST_CNF:
            At_RcvVcI2sTestCnfProc(msgTemp);
            break;

        case APP_VC_MSG_I2S_TEST_RSLT_IND:
            AT_RcvVcI2sTestRsltIndProc((struct MsgCB *)msgTemp);
            break;
#endif

        default:
            return;
    }
}


VOS_VOID At_RcvRnicMsg(struct MsgCB *msg)
{
    MSG_Header *msgHeader = VOS_NULL_PTR;

    msgHeader = (MSG_Header *)msg;

    switch (msgHeader->msgName) {
        /* 处理RNIC发来的拨号模式查询回复 */
        case ID_RNIC_AT_DIAL_MODE_CNF:

            AT_RcvRnicDialModeCnf((MsgBlock *)msg);
            break;

        case ID_AT_RNIC_RMNET_CFG_CNF:
            AT_RcvRnicRmnetCfgCnf((MsgBlock *)msg);
            break;

        default:
            AT_ERR_LOG("At_RcvRnicMsg: msgName err.");
            break;
    }
    return;
}


VOS_VOID At_ProcMsgFromCc(struct MsgCB *msg)
{
    MSG_Header *msgHeader = VOS_NULL_PTR;

    msgHeader = (MSG_Header *)msg;

    switch (msgHeader->msgName) {
        case AT_CC_MSG_STATE_QRY_CNF:
            At_RcvAtCcMsgStateQryCnfProc(msg);
            break;

        default:
            break;
    }
    return;
}


VOS_VOID AT_RcvNdisPdnInfoCfgCnf(AT_NDIS_PdnInfoCfgCnf *ndisPdnInfoCfgCnf)
{
    if (ndisPdnInfoCfgCnf->result == AT_NDIS_PDNCFG_CNF_FAIL) {
        AT_ERR_LOG("AT_RcvNdisPdnInfoRelCnf: Failed.");
    }

    return;
}


VOS_VOID AT_RcvNdisPdnInfoRelCnf(AT_NDIS_PdnInfoRelCnf *ndisPdnInfoRelCnf)
{
    if (ndisPdnInfoRelCnf->result != AT_NDIS_SUCC) {
        AT_ERR_LOG("AT_RcvNdisPdnInfoRelCnf: Failed.");
    }

    return;
}


VOS_VOID AT_RcvNdisMsg(struct MsgCB *msg)
{
    MSG_Header *msgTemp = VOS_NULL_PTR;

    msgTemp = (MSG_Header *)msg;
    switch (msgTemp->msgName) {
        case ID_AT_NDIS_PDNINFO_CFG_CNF:
            AT_RcvNdisPdnInfoCfgCnf((AT_NDIS_PdnInfoCfgCnf *)msgTemp);
            break;

        case ID_AT_NDIS_PDNINFO_REL_CNF:
            AT_RcvNdisPdnInfoRelCnf((AT_NDIS_PdnInfoRelCnf *)msgTemp);
            break;

        default:
            AT_WARN_LOG("AT_RcvNdisMsg: Unexpected MSG is received.");
            break;
    }

    return;
}


VOS_UINT32 AT_ProcRabmSetFastDormParaCnf(RABM_AT_SetFastdormParaCnf *msg)
{
    VOS_UINT8  indexNum = 0;
    VOS_UINT32 rslt;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_ProcRabmSetFastDormParaCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;
    rslt                    = AT_ERROR;
    if (msg->rslt == AT_RABM_PARA_SET_RSLT_SUCC) {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_ProcRabmQryFastDormParaCnf(RABM_AT_QryFastdormParaCnf *msg)
{
    VOS_UINT8  indexNum = 0;
    VOS_UINT32 result;
    VOS_UINT16 length;

    result = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvAtCcMsgStateQryCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (msg->rslt == VOS_TRUE) {
        /* 输出查询结果 */
        length = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "^FASTDORM:%d", msg->fastDormPara.fastDormOperationType);

        if (msg->fastDormPara.fastDormOperationType != AT_RABM_FASTDORM_STOP_FD_ASCR) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", msg->fastDormPara.timeLen);
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_ProcRabmReleaseRrcCnf(RABM_AT_ReleaseRrcCnf *msg)
{
    VOS_UINT8  indexNum = 0;
    VOS_UINT32 rslt;

    rslt = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcRabmReleaseRrcCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;

    if (msg->rslt == VOS_OK) {
        rslt = AT_OK;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_VOID At_RcvVcI2sTestCnfProc(MN_AT_IndEvt *event)
{
    APP_VC_I2sTestCnf *i2sTestCnf = VOS_NULL_PTR;
    VOS_UINT8          indexId;
    VOS_UINT32         ret;

    /* 初始化 */
    indexId = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(event->clientId, &indexId) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcI2sTestCnfProc:WARNING:AT INDEX NOT FOUND!");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexId)) {
        AT_WARN_LOG("At_RcvVcI2sTestCnfProc : AT_BROADCAST_INDEX.");
        return;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexId].cmdCurrentOpt != AT_CMD_I2S_TEST_SET) {
        AT_WARN_LOG("At_RcvVcI2sTestCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return;
    }

    i2sTestCnf = (APP_VC_I2sTestCnf *)event->content;

    if (i2sTestCnf->result == VOS_OK) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexId);
    At_FormatResultData(indexId, ret);
}


AT_I2sTestStateUint8 AT_ConvertI2sTestResult(APP_VC_I2sTestRsltUint32 i2sTestRslt)
{
    VOS_UINT8 returnCode;

    switch (i2sTestRslt) {
        case APP_VC_I2S_TEST_RSLT_SUCCESS:
            returnCode = AT_I2S_TEST_PASS;
            break;
        case APP_VC_I2S_TEST_RSLT_FAILED:
            returnCode = AT_I2S_TEST_FAILED;
            break;
        case APP_VC_I2S_TEST_RSLT_TIME_OUT:
            returnCode = AT_I2S_TEST_TIMEOUT;
            break;
        default:
            returnCode = AT_I2S_TEST_FAILED;
            break;
    }
    return returnCode;
}


VOS_UINT32 AT_RcvVcI2sTestRsltIndProc(struct MsgCB *msg)
{
    MN_AT_IndEvt          *atMsg          = VOS_NULL_PTR;
    APP_VC_I2sTestRsltInd *i2sTestRsltInd = VOS_NULL_PTR;

    atMsg = (MN_AT_IndEvt *)msg;

    i2sTestRsltInd = (APP_VC_I2sTestRsltInd *)atMsg->content;

    AT_SetI2sTestState(AT_ConvertI2sTestResult(i2sTestRsltInd->result));

    return VOS_OK;
}
#endif

#if (FEATURE_HUAWEI_VP == FEATURE_ON)

VOS_UINT32 AT_ProcRabmSetVoicePreferParaCnf(RABM_AT_SetVoicepreferParaCnf *msg)
{
    VOS_UINT8  indexNum;
    VOS_UINT32 rslt;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcRabmSetVoicePreferParaCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出设置结果 */
    g_atSendDataBuff.bufLen = 0;
    rslt                      = AT_ERROR;
    if (msg->rslt == AT_RABM_PARA_SET_RSLT_SUCC) {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}

VOS_UINT32 AT_ProcRabmQryVoicePreferEnableParaCnf(RABM_AT_QryVoicepreferParaCnf *msg)
{
    VOS_UINT8  indexNum;
    VOS_UINT32 result;
    VOS_UINT16 length;

    result = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcRabmQryVoicePreferEnableParaCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    length = 0;
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d%s", g_atCrLf, g_parseContext[indexNum].cmdElement->cmdName,
        msg->rslt, g_atCrLf);
    result                    = AT_OK;
    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_ProcRabmVoicePreferStatusReport(RABM_AT_VoicepreferStatusReport *msg)
{
    VOS_UINT8  indexNum;
    VOS_UINT16 length;

    indexNum = 0;
    length   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(msg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcRabmVoicePreferStatusReport:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 输出上报结果 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^VOICEPREFERSTATUS:%d%s", g_atCrLf, msg->vpStatus, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}
#endif


TAF_VOID AT_RabmMsgProc(struct MsgCB *msg)
{
    MSG_Header *msgTemp = VOS_NULL_PTR;

    msgTemp = (MSG_Header *)msg;
    switch (msgTemp->msgName) {
        case ID_RABM_AT_SET_FASTDORM_PARA_CNF:
            AT_ProcRabmSetFastDormParaCnf((RABM_AT_SetFastdormParaCnf *)msgTemp);
            break;

        case ID_RABM_AT_QRY_FASTDORM_PARA_CNF:
            AT_ProcRabmQryFastDormParaCnf((RABM_AT_QryFastdormParaCnf *)msgTemp);
            break;

        case ID_RABM_AT_SET_RELEASE_RRC_CNF:
            AT_ProcRabmReleaseRrcCnf((RABM_AT_ReleaseRrcCnf *)msgTemp);
            break;

#if (FEATURE_HUAWEI_VP == FEATURE_ON)
        case ID_RABM_AT_SET_VOICEPREFER_PARA_CNF:
            AT_ProcRabmSetVoicePreferParaCnf((RABM_AT_SetVoicepreferParaCnf *)msgTemp);
            break;

        case ID_RABM_AT_QRY_VOICEPREFER_PARA_CNF:
            AT_ProcRabmQryVoicePreferEnableParaCnf((RABM_AT_QryVoicepreferParaCnf *)msgTemp);
            break;

        case ID_RABM_AT_VOICEPREFER_STATUS_REPORT:
            AT_ProcRabmVoicePreferStatusReport((RABM_AT_VoicepreferStatusReport *)msgTemp);
            break;
#endif
        default:
            AT_WARN_LOG("At_RabmMsgProc:WARNING:Wrong Msg!");
            break;
    }
}


VOS_UINT32 At_DsmMsgProc(const MSG_Header *msg)
{
    /* NDIS网卡相关的交互先通过AT转，之后AT整体下移之后由DSM直接与NDIS进行交互 */
    switch (msg->msgName) {
        case ID_DSM_NDIS_IFACE_UP_IND:
            AT_PS_ProcNdisIfaceUpCfg((DSM_NDIS_IfaceUpInd *)msg);
            break;

        case ID_DSM_NDIS_IFACE_DOWN_IND:
            AT_PS_ProcNdisIfaceDownCfg((DSM_NDIS_IfaceDownInd *)msg);
            break;

        case ID_DSM_NDIS_CONFIG_IPV6_DNS_IND:
            AT_PS_ProcNdisConfigIpv6Dns((DSM_NDIS_ConfigIpv6DnsInd *)msg);
            break;

        default:
            return AT_ERROR;
    }

    return AT_OK;
}


VOS_VOID At_TafAndDmsMsgProc(struct MsgCB *msg)
{
    /* NDIS网卡相关的交互先通过AT转，之后AT整体下移之后由DSM直接与NDIS进行交互 */
    if (At_DsmMsgProc((MSG_Header *)msg) == AT_OK) {
        return;
    }

    At_EventMsgProc((MN_AT_IndEvt *)msg);

    return;
}


AT_MSG_PROC_FUNC At_GetMsgProcFunBySndPid(VOS_UINT32 sndPid)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 sndPidNo;

    sndPidNo = sizeof(g_atMsgProc) / sizeof(g_atMsgProc[0]);

    for (indexNum = 0; indexNum < sndPidNo; indexNum++) {
        if (sndPid == g_atMsgProc[indexNum].sndPid) {
            return g_atMsgProc[indexNum].procMsgFunc;
        }
    }

    return VOS_NULL_PTR;
}


TAF_VOID At_MsgProc(MsgBlock *msg)
{
    AT_MSG_PROC_FUNC procFun;
    VOS_UINT32       sendPid;
    VOS_UINT32       msgName;
    VOS_UINT32       sliceStart;
    VOS_UINT32       sliceEnd;
    VOS_UINT32       type;

    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("At_MsgProc: msg is null.");
        return;
    }

    sliceStart = VOS_GetSlice();
    sendPid    = VOS_GET_SENDER_ID(msg);

    if (DMS_ChkDmsFidRcvMsgLen(msg) != VOS_TRUE) {
        AT_ERR_LOG("At_MsgProc: message length is invalid!");
        return;
    }
    msgName    = ((MSG_Header *)msg)->msgName;

    /* 超时处理 */
    if (sendPid == VOS_PID_TIMER) {
        At_TimeOutProc((REL_TimerMsgBlock *)msg);
        sliceEnd = VOS_GetSlice();
        AT_RecordAtMsgInfo(sendPid, msgName, sliceStart, sliceEnd);

        return;
    }

    if (sendPid == WUEPS_PID_AT) {
        type    = ((AT_Msg *)msg)->type;
        msgName = (type << 16) | msgName;
    }

    if ((sendPid == I0_WUEPS_PID_TAF) || (sendPid == I1_WUEPS_PID_TAF) || (sendPid == I2_WUEPS_PID_TAF) ||
        (sendPid == I0_UEPS_PID_DSM) || (sendPid == I1_UEPS_PID_DSM) || (sendPid == I2_UEPS_PID_DSM)) {
        msgName = ((MN_AT_IndEvt *)msg)->msgName;
    }

    procFun = At_GetMsgProcFunBySndPid(sendPid);

    if (procFun == VOS_NULL_PTR) {
        AT_LOG1("At_MsgProc other PID msg", sendPid);
    } else {
        procFun((struct MsgCB *)msg);
    }

    sliceEnd = VOS_GetSlice();
    AT_RecordAtMsgInfo(sendPid, msgName, sliceStart, sliceEnd);

    return;
}


VOS_VOID AT_EventReport(VOS_UINT32 pid, NAS_OM_EventIdUint16 eventId, VOS_UINT8 *para, VOS_UINT32 len)
{
    mdrv_diag_event_ind_s diagEvent;
    NAS_OM_EventInd      *atEvent = VOS_NULL_PTR;
    VOS_UINT8            *data    = para;
    VOS_UINT32            atEventMsgLen;
    VOS_UINT32            rslt;
    errno_t               memResult;

    /* 入参检查 */
    if ((data == VOS_NULL_PTR) && (len != 0)) {
        /* 错误打印 */
        TAF_LOG(pid, VOS_NULL, PS_LOG_LEVEL_WARNING, "NAS_EventReport:pPara is NULL.");
        return;
    }

    /* 申请内存 */
    if (len <= NAS_OM_EVENTIND_DATA_LEN) {
        atEventMsgLen = sizeof(NAS_OM_EventInd);
        atEvent       = (NAS_OM_EventInd *)PS_MEM_ALLOC(pid, atEventMsgLen);
    } else {
        atEventMsgLen = sizeof(NAS_OM_EventInd) + len - NAS_OM_EVENTIND_DATA_LEN;
        atEvent       = (NAS_OM_EventInd *)PS_MEM_ALLOC(pid, atEventMsgLen);
    }

    if (atEvent == VOS_NULL_PTR) {
        TAF_LOG(pid, VOS_NULL, PS_LOG_LEVEL_WARNING, "NAS_EventReport:Alloc mem fail.");
        return;
    }

    /* 初始化 */
    (VOS_VOID)memset_s(atEvent, atEventMsgLen, 0x00, atEventMsgLen);

    /* 填写事件消息内容 */
    atEvent->eventId = eventId;
    atEvent->reserve = 0;
    if ((data != VOS_NULL_PTR) && (len > 0)) {
        memResult = memcpy_s(atEvent->data, len, data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, len, len);
    }

    /* 填写发给DIAG的结构体 */
    diagEvent.ulLength  = sizeof(NAS_OM_EventInd) - NAS_OM_EVENTIND_DATA_LEN + len;
    diagEvent.ulPid     = pid;
    diagEvent.ulEventId = (VOS_UINT32)eventId;
    diagEvent.ulModule  = MDRV_DIAG_GEN_MODULE(MODEM_ID_0, DIAG_MODE_UMTS);
    diagEvent.pData     = atEvent;

    rslt = mdrv_diag_event_report(&diagEvent);
    if (rslt != VOS_OK) {
        AT_WARN_LOG("AT_EventReport:OM_AcpuEvent Fail.");
    }

    /*lint -save -e516 */
    PS_MEM_FREE(pid, atEvent);
    /*lint -restore */

    return;
}


VOS_UINT32 AT_RcvDrvAgentSetAdcRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg       *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_AdcSetCnf *adcCnf   = VOS_NULL_PTR;
    VOS_UINT8            indexNum = 0;
    VOS_UINT32           result;

    /* 初始化消息 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    adcCnf = (DRV_AGENT_AdcSetCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(adcCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetAdcRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetAdcRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ADC_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ADC_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 设置门限值是否成功 */
    if (adcCnf->fail != VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用AT_FormATResultDATa发送命令结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentQryTbatRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg        *rcvMsg      = VOS_NULL_PTR;
    DRV_AGENT_TbatQryCnf *tbatTypeCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum    = 0;

    /* 初始化消息 */
    rcvMsg      = (DRV_AGENT_Msg *)msg;
    tbatTypeCnf = (DRV_AGENT_TbatQryCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(tbatTypeCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryTbatRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 自动应答开启情况下被叫死机问题 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryTbatRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_TBAT_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TBAT_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            tbatTypeCnf->tbatType);

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvSimLockQryRsp(struct MsgCB *msg)
{
    VOS_UINT8             indexNum = 0;
    VOS_UINT32            ret;
    AT_MMA_SimlockStatus *event = VOS_NULL_PTR;

    event = (AT_MMA_SimlockStatus *)msg;

    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvSimLockQryRsp: AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIMLOCKSTATUS_READ或AT_CMD_CSDFLT_READ */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKSTATUS_READ) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSDFLT_READ)) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SIMLOCKSTATUS_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", "^SIMLOCK", event->simlockEnableFlg);
        ret                     = AT_OK;

    } else {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_OutputCsdfltDefault(indexNum, event->simlockEnableFlg);
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)

VOS_UINT32 AT_RcvDrvAgentSetSpwordRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg          *rcvMsg       = VOS_NULL_PTR;
    DRV_AGENT_SpwordSetCnf *spwordSetCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum     = 0;

    /* 初始化消息 */
    rcvMsg       = (DRV_AGENT_Msg *)msg;
    spwordSetCnf = (DRV_AGENT_SpwordSetCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(spwordSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("pstSpwordSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("pstSpwordSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SPWORD_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SPWORD_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 密码校验通过，错误次数需要清零 */
    if (spwordSetCnf->result == VOS_OK) {
        g_spWordCtx.shellPwdCheckFlag = VOS_TRUE;
        g_spWordCtx.errTimes          = 0;

        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_OK);
    }
    /* 密码校验失败，错误次数加1 */
    else {
        g_spWordCtx.errTimes++;

        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}

#endif

VOS_UINT32 AT_RcvMmaCipherInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_CipherQryCnf *cipherQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    /* 初始化 */
    cipherQryCnf = (TAF_MMA_CipherQryCnf *)msg;
    result       = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cipherQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCipherInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCipherInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CIPERQRY_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIPERQRY_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CIPERQRY查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (cipherQryCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
        if (cipherQryCnf->nrFlag == VOS_TRUE) {
            g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                cipherQryCnf->mcgCipherInfo, cipherQryCnf->scgCipherInfo);
        } else {
            g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
                cipherQryCnf->mcgCipherInfo);
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaLocInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_LocationInfoQryCnf *locInfoCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT16                  length;
    VOS_UINT8                   cellIdStr[AT_CELLID_STRING_MAX_LEN];
    VOS_UINT8                   indexNum = 0;

    /* 初始化 */
    locInfoCnf = (TAF_MMA_LocationInfoQryCnf *)msg;
    length     = 0;
    result     = AT_OK;
    (VOS_VOID)memset_s(cellIdStr, sizeof(cellIdStr), 0x00, sizeof(cellIdStr));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(locInfoCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaLocInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaLocInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_LOCINFO_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LOCINFO_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CIPERQRY查询命令返回 */
    if (locInfoCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        /* 上报MCC和MNC */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%X%X%X", (locInfoCnf->mcc & 0x0f),
            (locInfoCnf->mcc & 0x0f00) >> 8, (locInfoCnf->mcc & 0x0f0000) >> 16);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X", (locInfoCnf->mnc & 0x0f),
            (locInfoCnf->mnc & 0x0f00) >> 8);

        /* MNC最后一位不等于F，则MNC为三位，否则为两位 */
        if (((locInfoCnf->mnc & 0x0f0000) >> 16) != 0x0f) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X", (locInfoCnf->mnc & 0x0f0000) >> 16);
        }

        /* Lac */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0x%X", locInfoCnf->lac);

        /* Rac */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0x%X", locInfoCnf->rac);

        /* Cell ID */
        AT_ConvertCellIdToHexStrFormat(locInfoCnf->cellId.cellIdLowBit, locInfoCnf->cellId.cellIdHighBit,
                                       (VOS_CHAR *)cellIdStr);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",0x%s", cellIdStr);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentNvBackupStatQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg                *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_NvbackupstatQryCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                     indexNum = 0;
    VOS_UINT32                    result;
    VOS_UINT16                    length;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NvbackupstatQryCnf *)rcvMsg->content;
    length = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentNvBackupStatQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentNvBackupStatQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NVBACKUPSTAT_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVBACKUPSTAT_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_NO_ERROR) {
        if (event->nvBackupStat == NV_OK) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                VOS_TRUE);
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                VOS_FALSE);
        }

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentNandBadBlockQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_NandbbcQryCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                indexNum = 0;
    VOS_UINT32               result;
    VOS_UINT16               length;
    VOS_UINT32               uli;
    VOS_UINT32               maxBBNum;

    /* 初始化消息变量 */
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_NandbbcQryCnf *)rcvMsg->content;
    length   = 0;
    uli      = 0;
    maxBBNum = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentNandBadBlockQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentNandBadBlockQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NANDBBC_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NANDBBC_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_NO_ERROR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            event->badBlockNum);

        maxBBNum = (event->badBlockNum > DRV_AGENT_NAND_BADBLOCK_MAX_NUM) ? DRV_AGENT_NAND_BADBLOCK_MAX_NUM :
                                                                            event->badBlockNum;
        for (uli = 0; uli < maxBBNum; uli++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s: %d", g_atCrLf,
                g_parseContext[indexNum].cmdElement->cmdName, event->badBlockIndex[uli]);
        }

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentNandDevInfoQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_NandverQryCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                indexNum = 0;
    VOS_UINT32               result;
    VOS_UINT16               length;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_NandverQryCnf *)rcvMsg->content;
    length = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentNandDevInfoQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentNandDevInfoQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NANDVER_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NANDVER_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_NO_ERROR) {
        /* 输出查询结果 */

        /* buf从外部接口返回的，增加结束符，防止缓冲器溢出 */
        event->nandDevInfo.mufName[DRV_AGENT_NAND_MFU_NAME_MAX_LEN - 1] = '\0';
        event->nandDevInfo.devSpec[DRV_AGENT_NAND_DEV_SPEC_MAX_LEN - 1] = '\0';
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,\"%s\",%d,\"%s\"",
            g_parseContext[indexNum].cmdElement->cmdName, event->nandDevInfo.mufId, event->nandDevInfo.mufName,
            event->nandDevInfo.devId, event->nandDevInfo.devSpec);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentChipTempQryRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg            *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_ChiptempQryCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                 indexNum = 0;
    VOS_UINT32                result;
    VOS_UINT16                length;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_ChiptempQryCnf *)rcvMsg->content;
    length = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentChipTempQryRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentChipTempQryRsp: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CHIPTEMP_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CHIPTEMP_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_NO_ERROR) {
        /* 输出查询结果 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, event->gpaTemp, event->wpaTemp, event->lpaTemp,
            event->simTemp, event->batTemp);

        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetMaxLockTmsRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg              *rcvMsg           = VOS_NULL_PTR;
    DRV_AGENT_MaxLockTmsSetCnf *maxlockTmsSetCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum         = 0;

    /* 初始化消息 */
    rcvMsg           = (DRV_AGENT_Msg *)msg;
    maxlockTmsSetCnf = (DRV_AGENT_MaxLockTmsSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(maxlockTmsSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetMaxLockTmsRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetMaxLockTmsRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MAXLCKTMS_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MAXLCKTMS_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^MAXLCKTMS设置命令返回 */
    if (maxlockTmsSetCnf->result != VOS_OK) {
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_DEVICE_OTHER_ERROR);
    } else {
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSetApSimstRsp(struct MsgCB *msg)
{
    DRV_AGENT_Msg           *rcvMsg        = VOS_NULL_PTR;
    DRV_AGENT_ApSimstSetCnf *apSimStSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum      = 0;

    /* 初始化消息 */
    rcvMsg        = (DRV_AGENT_Msg *)msg;
    apSimStSetCnf = (DRV_AGENT_ApSimstSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(apSimStSetCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSetApSimstRsp:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSetApSimstRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_APSIMST_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_APSIMST_SET) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^APSIMST设置命令返回 */
    if (apSimStSetCnf->result != VOS_OK) {
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentHukSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg       *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_HukSetCnf *event  = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    AT_PR_LOGI("enter");

    /* 初始化 */
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_HukSetCnf *)rcvMsg->content;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    result   = VOS_NULL;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentHukSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentHukSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_HUK_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HUK_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentFacAuthPubkeySetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                 *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_FacauthpubkeySetCnf *event  = VOS_NULL_PTR;
    VOS_UINT8                      indexNum;
    VOS_UINT32                     result;

    AT_PR_LOGI("enter");

    /* 初始化 */
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_FacauthpubkeySetCnf *)rcvMsg->content;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    result   = VOS_NULL;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentFacAuthPubkeySetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentFacAuthPubkeySetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_FACAUTHPUBKEY_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FACAUTHPUBKEYEX_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentIdentifyStartSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                 *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_IdentifystartSetCnf *event  = VOS_NULL_PTR;
    VOS_UINT8                      indexNum;
    VOS_UINT32                     result;
    VOS_UINT16                     length;
    VOS_UINT32                     i;

    AT_PR_LOGI("enter");

    /* 初始化 */
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_IdentifystartSetCnf *)rcvMsg->content;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    length   = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentIdentifyStartSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentIdentifyStartSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IDENTIFYSTART_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IDENTIFYSTART_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        if (event->otaFlag == VOS_TRUE) {
            for (i = 0; i < TAF_OTA_SIMLOCK_PUBLIC_ID_NUM; i++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "0x%X,", event->publicId[i]);
            }
        }
        for (i = 0; i < DRV_AGENT_RSA_CIPHERTEXT_LEN; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", event->rsaText[i]);
        }
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentIdentifyEndSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg               *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_IdentifyendSetCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                    indexNum = 0;
    VOS_UINT32                   result;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_IdentifyendSetCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentIdentifyEndSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentIdentifyEndSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IDENTIFYEND_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IDENTIFYEND_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSimlockDataWriteSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                    *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_SimlockdatawriteSetCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                         indexNum = 0;
    VOS_UINT32                        result;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_SimlockdatawriteSetCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataWriteSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataWriteSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIMLOCKDATAWRITE_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKDATAWRITE_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_CHAR AT_Num2AsciiNum(VOS_UINT8 num)
{
    if (num <= AT_DEC_MAX_NUM) {
        return (VOS_CHAR)('0' + num);
    } else if (num <= 0x0F) {
        return (VOS_CHAR)('A' + (num - 0x0A));
    } else {
        return '*';
    }
}


VOS_UINT32 AT_CheckSimlockCodeLast2Char(DRV_AGENT_PersonalizationCategoryUint8 category, VOS_CHAR *pcStrLast2Char)
{
    VOS_UINT32 i = 0;

    /* 检测最后两位的有效性, NET无需检测 */
    if (category > DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK) {
        for (i = 0; i < AT_SIMLOCKCODE_LAST2CHAR_LEN; i++) {
            /* NETSUB支持范围: 0x00~0x99 */
            if ((pcStrLast2Char[i] >= '0') && (pcStrLast2Char[i] <= '9')) {
                continue;
            }
            /* SP支持范围: 0x00~0xFF */
            else if ((category == DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER) && (pcStrLast2Char[i] >= 'A') &&
                     (pcStrLast2Char[i] <= 'F')) {
                continue;
            } else {
                return VOS_ERR;
            }
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckSimlockCodeStr(DRV_AGENT_PersonalizationCategoryUint8 category, VOS_CHAR *pcStrCode)
{
    VOS_UINT32 i = 0;
    VOS_UINT32 j = 0;
    VOS_UINT8 len; /* 锁网号码长度 */
    VOS_CHAR *pcTmpStr = VOS_NULL_PTR;

    pcTmpStr = pcStrCode;

    switch (category) {
        case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK:
            len = AT_PERSONALIZATION_NET_CODE_LEN;
            break;
        case DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE:
            len = AT_PERSONALIZATION_CP_CODE_LEN;
            break;
        default:
            len = AT_PERSONALIZATION_CODE_LEN;
            break;
    }

    /* 检测PLMN合法性及其位数 */
    for (i = 0; i < AT_PERSONALIZATION_NET_CODE_LEN; i++) {
        if ((pcTmpStr[i] >= '0') && (pcTmpStr[i] <= '9')) {
            continue;
        } else if ((i == (AT_PERSONALIZATION_NET_CODE_LEN - 1)) && (pcTmpStr[i] == 'F')) {
            for (j = i + 1; j < len; j++) {
                pcTmpStr[j - 1] = pcTmpStr[j];
            }
            len = len - 1;
            break;
        } else {
            return VOS_ERR;
        }
    }

    pcTmpStr[len] = 0;

    /* 检测锁网锁卡号码最后两位的合法性 */
    if (AT_CheckSimlockCodeLast2Char(category, &pcTmpStr[len - AT_SIMLOCKCODE_LAST2CHAR_LEN]) != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SimlockCodeBcd2Str(DRV_AGENT_PersonalizationCategoryUint8 category, VOS_UINT8 *bcdNum, VOS_CHAR *pcStrNum,
                                 VOS_UINT32 strBufLen)
{
    VOS_UINT32 i = 0;
    VOS_UINT8 decodeLen;
    VOS_UINT8 firstNum;
    VOS_UINT8 secondNum;
    VOS_CHAR *pcStrTmp = pcStrNum;
    VOS_CHAR  tmpChar;

    switch (category) {
        case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK:
            decodeLen = AT_PERSONALIZATION_NET_CODE_BCD_LEN;
            break;
        case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET:
            decodeLen = AT_PERSONALIZATION_SUBNET_CODE_BCD_LEN;
            break;
        case DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER:
            decodeLen = AT_PERSONALIZATION_SP_CODE_BCD_LEN;
            break;
        case DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE:
            decodeLen = AT_PERSONALIZATION_CP_CODE_BCD_LEN;
            break;
        default:
            return VOS_ERR;
    }

    if (strBufLen <= AT_PERSONALIZATION_CODE_LEN_EX) {
        return VOS_ERR;
    }

    /* 对锁网锁卡号码进行高低字节转换 */
    for (i = 0; i < decodeLen; i++) {
        /* 分别取出高低字节 */
        firstNum  = (VOS_UINT8)((*(bcdNum + i)) & 0x0F);
        secondNum = (VOS_UINT8)(((*(bcdNum + i)) >> 4) & 0x0F);

        *pcStrTmp++ = AT_Num2AsciiNum(firstNum);
        *pcStrTmp++ = AT_Num2AsciiNum(secondNum);
    }

    /* 将第四位号码后移到第六位(与产线对接) */
    pcStrTmp    = &pcStrNum[AT_PERSONALIZATION_CODE_FOURTH_CHAR_INDEX];
    tmpChar     = pcStrTmp[0];
    pcStrTmp[0] = pcStrTmp[1];
    pcStrTmp[1] = pcStrTmp[2];
    pcStrTmp[2] = tmpChar;

    /* 对高低字节转换后的锁网锁卡号码进行合法性检查 */
    if (AT_CheckSimlockCodeStr(category, pcStrNum) != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 AT_PhoneSimlockInfoPrint(DRV_AGENT_PhonesimlockinfoQryCnfNew *phoneSimlockInfo, VOS_UINT8 indexNum,
                                    VOS_UINT16 *length)
{
    VOS_UINT16 lengthTemp;
    VOS_CHAR   acCodeBegin[AT_PERSONALIZATION_CODE_LEN_EX + 1];
    VOS_CHAR   acCodeEnd[AT_PERSONALIZATION_CODE_LEN_EX + 1];
    VOS_UINT32 codeBeginRslt;
    VOS_UINT32 codeEndRslt;
    VOS_UINT32 printGroupNum;
    VOS_UINT32 catIndex;
    VOS_UINT32 groupIndex;
    errno_t    memResult;
    VOS_UINT8  catNum;

    /* 局部变量初始化 */
    lengthTemp = *length;
    catIndex   = 0;
    groupIndex = 0;

    memResult = memset_s(acCodeBegin, sizeof(acCodeBegin), 0x00, (AT_PERSONALIZATION_CODE_LEN_EX + 1));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acCodeBegin), (AT_PERSONALIZATION_CODE_LEN_EX + 1));
    memResult = memset_s(acCodeEnd, sizeof(acCodeEnd), 0x00, (AT_PERSONALIZATION_CODE_LEN_EX + 1));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(acCodeEnd), (AT_PERSONALIZATION_CODE_LEN_EX + 1));

    codeBeginRslt = VOS_NULL;
    codeEndRslt   = VOS_NULL;
    catNum        = AT_MIN(phoneSimlockInfo->supportCategoryNum, DRV_AGENT_SUPPORT_CATEGORY_NUM_EXTERED);

    for (catIndex = 0; catIndex < catNum; catIndex++) {
        if (catIndex != 0) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%s", g_atCrLf);
        }

        /* ^PHONESIMLOCKINFO: <cat>,<indicator> [,<total_group_num>,<flag>,(<code_begin>,<code_end>)...] */
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        switch (phoneSimlockInfo->categoryInfo[catIndex].category) {
            case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "NET");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "NETSUB");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "SP");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "CP");
                break;

            case DRV_AGENT_PERSONALIZATION_CATEGORY_SIM_USIM:
            default:
                return VOS_ERR;
        }

        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", phoneSimlockInfo->categoryInfo[catIndex].indicator);

        if (phoneSimlockInfo->categoryInfo[catIndex].indicator == DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", phoneSimlockInfo->categoryInfo[catIndex].groupNum);

            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", phoneSimlockInfo->categoryInfo[catIndex].flag);

            /* 最多打印上报10组号段 */
            printGroupNum = AT_MIN(phoneSimlockInfo->categoryInfo[catIndex].groupNum,
                                   DRV_AGENT_PH_LOCK_CODE_GROUP_NUM_EXTERED);

            for (groupIndex = 0; groupIndex < printGroupNum; groupIndex++) {
                codeBeginRslt =
                    AT_SimlockCodeBcd2Str(phoneSimlockInfo->categoryInfo[catIndex].category,
                                          phoneSimlockInfo->categoryInfo[catIndex].lockCode[groupIndex].phLockCodeBegin,
                                          acCodeBegin, sizeof(acCodeBegin));
                codeEndRslt =
                    AT_SimlockCodeBcd2Str(phoneSimlockInfo->categoryInfo[catIndex].category,
                                          phoneSimlockInfo->categoryInfo[catIndex].lockCode[groupIndex].phLockCodeEnd,
                                          acCodeEnd, sizeof(acCodeEnd));
                if ((codeBeginRslt == VOS_OK) && (codeEndRslt == VOS_OK)) {
                    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",(%s,%s)", acCodeBegin, acCodeEnd);
                } else {
                    return VOS_ERR;
                }
            }
        }
    }
    *length = lengthTemp;
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentPhoneSimlockInfoQryCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                       *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_PhonesimlockinfoQryCnfNew *event  = VOS_NULL_PTR;
    VOS_UINT8                            indexNum;
    VOS_UINT32                           result;
    VOS_UINT16                           length;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;

    event = (DRV_AGENT_PhonesimlockinfoQryCnfNew *)rcvMsg->content;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    length   = 0;

    AT_PR_LOGI("enter");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentPhoneSimlockInfoQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentPhoneSimlockInfoQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHONESIMLOCKINFO_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PHONESIMLOCKINFO_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
        /* 打印结果 */
        if (AT_PhoneSimlockInfoPrint(event, indexNum, &length) != VOS_OK) {
            result = AT_PERSONALIZATION_OTHER_ERROR;
        }
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = length;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SimlockDataReadPrint(DRV_AGENT_SimlockdatareadQryCnf simlockDataRead, VOS_UINT8 indexNum,
                                   VOS_UINT16 *length)
{
    VOS_UINT16 lengthTemp = *length;
    VOS_UINT32 catIndex   = 0;
    VOS_UINT8  catNum;

    catNum = AT_MIN(simlockDataRead.supportCategoryNum, DRV_AGENT_SUPPORT_CATEGORY_NUM);

    for (catIndex = 0; catIndex < catNum; catIndex++) {
        if (catIndex != 0) {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%s", g_atCrLf);
        }

        /* ^SIMLOCKDATAREAD: <cat>,<indicator>,<lock_status>,<max_times>,<remain_times> */
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        switch (simlockDataRead.categoryData[catIndex].category) {
            case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "NET");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "NETSUB");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "SP");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE:
                lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, "CP");
                break;
            case DRV_AGENT_PERSONALIZATION_CATEGORY_SIM_USIM:
            default:
                return VOS_ERR;
        }
        lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",%d", simlockDataRead.categoryData[catIndex].indicator);
        if (simlockDataRead.categoryData[catIndex].indicator == DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE) {
            switch (simlockDataRead.categoryData[catIndex].status) {
                case DRV_AGENT_PERSONALIZATION_STATUS_READY:
                    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",READY,,");
                    break;
                case DRV_AGENT_PERSONALIZATION_STATUS_PIN:
                    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",PIN,%d,%d",
                        simlockDataRead.categoryData[catIndex].maxUnlockTimes,
                        simlockDataRead.categoryData[catIndex].remainUnlockTimes);
                    break;
                case DRV_AGENT_PERSONALIZATION_STATUS_PUK:
                    lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",PUK,,");
                    break;
                default:
                    return VOS_ERR;
            }
        } else {
            lengthTemp += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + lengthTemp, ",,,");
        }
    }
    *length = lengthTemp;
    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentSimlockDataReadQryCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                   *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_SimlockdatareadQryCnf *event  = VOS_NULL_PTR;
    VOS_UINT8                        indexNum;
    VOS_UINT8                        simlockType;
    VOS_UINT32                       result;
    VOS_UINT16                       length;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    rcvMsg      = (DRV_AGENT_Msg *)msg;
    event       = (DRV_AGENT_SimlockdatareadQryCnf *)rcvMsg->content;
    indexNum    = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    simlockType = (VOS_UINT8)AT_GetSimlockUnlockCategoryFromClck();
    length      = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataReadQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataReadQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKDATAREAD_READ &&
        g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLCK_SIMLOCKDATAREAD) {
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
        /* 打印结果 */
        if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLCK_SIMLOCKDATAREAD) {
            if (AT_SimlockDataReadPrint(*event, indexNum, &length) != VOS_OK) {
                result = AT_PERSONALIZATION_OTHER_ERROR;
            }
        } else {
            /* 读越界保护,取值超过最大范围时，不能读越界 */
            if (simlockType >= DRV_AGENT_SUPPORT_CATEGORY_NUM) {
                AT_WARN_LOG1("AT_RcvDrvAgentSimlockDataReadQryCnf: error simlockType", simlockType);
                simlockType = 0;
            }
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                event->categoryData[simlockType].indicator);
        }
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = length;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

/*
 * 功能描述: ^GETMODEMSCID命令查询回复处理函数
 */
VOS_UINT32 AT_RcvDrvAgentGetSimlockEncryptIdQryCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                          *rcvMsg  = VOS_NULL_PTR;
    DRV_AGENT_GetSimlockEncryptIdQryCnf    *event = VOS_NULL_PTR;
    VOS_UINT32                              result;
    VOS_UINT32                              scIdNum;
    VOS_UINT32                              i;
    VOS_UINT16                              length;
    VOS_UINT8                               indexNum;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    rcvMsg       = (DRV_AGENT_Msg *)msg;
    event        = (DRV_AGENT_GetSimlockEncryptIdQryCnf *)rcvMsg->content;
    indexNum     = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    length = 0;
    result = AT_OK;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentGetSimlockEncryptIdQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentGetSimlockEncryptIdQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_GETMODEMSCID_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GETMODEMSCID_READ) {
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%u,", event->scEncryptIdInfo.scAlgorithm);

        scIdNum = AT_MIN(event->scEncryptIdInfo.scEncryptIdNum, DRV_AGENT_SIMLOCK_ENCRYPT_KEYID_NUM);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%u", scIdNum);


        for (i = 0; i < scIdNum; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%X", event->scEncryptIdInfo.scEncryptId[i]);
        }


    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = length;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDrvAgentPhonePhynumSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg               *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_PhonephynumSetCnf *event    = VOS_NULL_PTR;
    VOS_UINT8                    indexNum = 0;
    VOS_UINT32                   result;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_PhonephynumSetCnf *)rcvMsg->content;

    AT_PR_LOGI("enter");

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentPhonePhynumSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentPhonePhynumSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHONEPHYNUM_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PHONEPHYNUM_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvDrvAgentOpwordSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg          *rcvMsg   = VOS_NULL_PTR;
    DRV_AGENT_OpwordSetCnf *event    = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    rcvMsg = (DRV_AGENT_Msg *)msg;
    event  = (DRV_AGENT_OpwordSetCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentOpwordSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentOpwordSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_OPWORD_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_OPWORD_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 密码比对成功，获取权限 */
        g_ate5RightFlag = AT_E5_RIGHT_FLAG_YES;
        result          = AT_OK;
    } else {
        /* 输出设置结果, 转换错误码 */
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvMtaCposSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg     *cposMsg  = VOS_NULL_PTR;
    MTA_AT_CposCnf *event    = VOS_NULL_PTR;
    VOS_UINT8       indexNum = 0;
    VOS_UINT32      result;

    /* 初始化消息变量 */
    cposMsg = (AT_MTA_Msg *)msg;
    event   = (MTA_AT_CposCnf *)cposMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cposMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCposSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCposSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CPOS_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOS_SET) {
        AT_WARN_LOG("AT_RcvMtaCposSetCnf: WARNING:Not AT_CMD_CPOS_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_ReportCposrInd(VOS_UINT8 indexNum, VOS_CHAR *pcXmlText)
{
    VOS_UINT16 len;
    VOS_UINT8  finalUsbIndex;
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    VOS_UINT8     finalVcomIndex;
    ModemIdUint16 modemId;
#endif

    finalUsbIndex = AT_CLIENT_ID_PCUI;
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    finalVcomIndex = AT_CLIENT_ID_APP9;
    modemId        = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_ReportCposrInd: Get modem id fail.");
        return;
    }

    switch (modemId) {
        case MODEM_ID_0:
            finalUsbIndex  = AT_CLIENT_ID_PCUI;
            finalVcomIndex = AT_CLIENT_ID_APP9;
            break;
        case MODEM_ID_1:
            finalUsbIndex  = AT_CLIENT_ID_CTRL;
            finalVcomIndex = AT_CLIENT_ID_APP12;
            break;
        case MODEM_ID_2:
            finalUsbIndex  = AT_CLIENT_ID_PCUI2;
            finalVcomIndex = AT_CLIENT_ID_APP24;
            break;
        default:
            AT_ERR_LOG("AT_ReportCposrInd: Invalid modem id!");
            return;
    }
#endif

    len = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s+CPOSR: %s%s", g_atCrLf, pcXmlText, g_atCrLf);

    At_SendResultData(finalUsbIndex, g_atSndCodeAddress, len);
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    At_SendResultData(finalVcomIndex, g_atSndCodeAddress, len);
#endif

    return;
}


VOS_UINT32 AT_RcvMtaCposrInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    VOS_UINT8        indexNum = 0;
    AT_MTA_Msg      *cposrMsg = VOS_NULL_PTR;
    MTA_AT_CposrInd *event    = VOS_NULL_PTR;
    AT_ModemAgpsCtx *agpsCtx  = VOS_NULL_PTR;

    /* 初始化消息变量 */
    cposrMsg = (AT_MTA_Msg *)msg;
    event    = (MTA_AT_CposrInd *)cposrMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cposrMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCposrInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(indexNum);

    /* 根据当前的AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_0)->enCposrReport的值判断是否允许主动上报辅助数据和指示 */
    if (agpsCtx->cposrReport == AT_CPOSR_ENABLE) {
        AT_ReportCposrInd(indexNum, event->xmlText);
    } else {
        AT_WARN_LOG1("AT_RcvMtaCposrInd: +CPOSR: ", agpsCtx->cposrReport);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaXcposrRptInd(struct MsgCB *msg)
{
    VOS_UINT8            indexNum     = 0;
    AT_MTA_Msg          *xcposrRptMsg = VOS_NULL_PTR;
    MTA_AT_XcposrrptInd *event        = VOS_NULL_PTR;

    /* 初始化消息变量 */
    xcposrRptMsg = (AT_MTA_Msg *)msg;
    event        = (MTA_AT_XcposrrptInd *)xcposrRptMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(xcposrRptMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaXcposrRptInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 根据当前的AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_0)->enXcposrReport的值判断是否允许上报主动清除申请 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^XCPOSRRPT: %d%s", g_atCrLf, event->clearFlg, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCgpsClockSetCnf(struct MsgCB *msg)
{
#if (FEATURE_AGPS == FEATURE_ON && FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    /* 定义局部变量 */
    VOS_BOOL             bEnable;
    ModemIdUint16        modemId;
    VOS_UINT8            indexNum;
    AT_MTA_Msg          *mtaMsg = VOS_NULL_PTR;
    MTA_AT_CgpsclockCnf *setCnf = VOS_NULL_PTR;

    mtaMsg = (AT_MTA_Msg *)msg;

    setCnf = (MTA_AT_CgpsclockCnf *)mtaMsg->content;

    /* 初始化消息变量 */
    indexNum = 0;
    bEnable  = 0;
    modemId  = MODEM_ID_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCgpsClockSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCgpsClockSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGPSCLOCK_SET) {
        AT_WARN_LOG("AT_RcvMtaCgpsClockSetCnf : Current Option is not AT_CMD_CGPSCLOCK_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        /* 调用AT_FormATResultDATa发送命令结果 */
        At_FormatResultData(indexNum, AT_ERROR);

        return VOS_OK;
    }

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        modemId = MODEM_ID_0;
    }

    bEnable = AT_GetCgpsCLockEnableFlgByModemId(modemId);

    if (AT_SetCgpsClock(bEnable, setCnf->modemId, setCnf->ratMode) != VOS_OK) {
        g_atSendDataBuff.bufLen = 0;
        /* 调用AT_FormATResultDATa发送命令结果 */
        At_FormatResultData(indexNum, AT_ERROR);

        AT_ERR_LOG("AT_RcvMtaCgpsClockSetCnf: AT_CfgGpsRefClk() failed!");
        return VOS_OK;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
#else
    AT_ERR_LOG("AT_RcvMtaCgpsClockSetCnf: AGPS is not open!");
    return VOS_OK;
#endif
}

VOS_VOID At_ProcMtaMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 msgId;
    VOS_UINT32 rst;

    /* 从g_atProcMsgFromMtaTab中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromMtaTab) / sizeof(AT_PROC_MsgFromMta);

    /* 从消息包中获取MSG ID */
    msgId = ((AT_MTA_Msg *)msg)->msgId;

    /* g_atProcMsgFromMtaTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromMtaTab[i].msgType == msgId) {
            rst = g_atProcMsgFromMtaTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("At_ProcMtaMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_ProcMtaMsg: Msg Id is invalid!");
    }

    return;
}

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))

VOS_VOID AT_ProcXpdsMsg(struct MsgCB *msg)
{
    VOS_UINT32 i;
    VOS_UINT32 msgCnt;
    VOS_UINT32 msgId;
    VOS_UINT32 rst;

    /* 从g_atProcMsgFromXpdsTab中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromXpdsTab) / sizeof(AT_PROC_MsgFromXpds);

    /* 从消息包中获取MSG ID */
    msgId = ((AT_XPDS_Msg *)msg)->msgId;

    /* g_atProcMsgFromXpdsTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromXpdsTab[i].msgType == msgId) {
            rst = g_atProcMsgFromXpdsTab[i].procMsgFunc(msg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcXpdsMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("At_ProcXpdsMsg: Msg Id is invalid!");
    }

    return;
}

#endif


VOS_UINT32 AT_RcvMtaSimlockUnlockSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SimlockunlockCnf *event  = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    event    = (MTA_AT_SimlockunlockCnf *)rcvMsg->content;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSimlockUnlockSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSimlockUnlockSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKUNLOCK_SET &&
        g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLCK_SIMLOCKUNLOCK) {
        AT_WARN_LOG("AT_RcvMtaSimlockUnlockSetCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断回复消息中的错误码 */
    if (event->result == MTA_AT_RESULT_NO_ERROR) {
        /* 解锁成功，输出OK */
        result = AT_OK;
    } else {
        /* 解锁失败，输出ERROR */
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryNmrCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg    = VOS_NULL_PTR;
    MTA_AT_QryNmrCnf *qryNmrCnf = VOS_NULL_PTR;
    VOS_UINT8         indexNum  = 0;
    VOS_UINT32        nmrDataIndex;
    VOS_UINT16        length;

    /* 初始化 */
    rcvMsg    = (AT_MTA_Msg *)msg;
    qryNmrCnf = (MTA_AT_QryNmrCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryNmrCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryNmrCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CNMR_QUERY) {
        return VOS_ERR;
    }

    if (qryNmrCnf->result == MTA_AT_RESULT_ERROR) {
        /* 查询失败直接上报error */
        g_atSendDataBuff.bufLen = 0;

        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        /* 输出结果 */
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    length = 0;

    /* 按AT^CNMR查询命令返回将接入层返回的数据码流显示上报 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (qryNmrCnf->totalIndex == 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d", 0, 0);

        g_atSendDataBuff.bufLen = length;

        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        /* 输出结果 */
        At_FormatResultData(indexNum, AT_OK);
        return VOS_OK;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,", qryNmrCnf->totalIndex, qryNmrCnf->currIndex);

    for (nmrDataIndex = 0; nmrDataIndex < qryNmrCnf->nmrLen; nmrDataIndex++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02x", qryNmrCnf->nmrData[nmrDataIndex]);
    }

    if (qryNmrCnf->totalIndex != qryNmrCnf->currIndex) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    }

    if (qryNmrCnf->totalIndex == qryNmrCnf->currIndex) {
        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);

        g_atSendDataBuff.bufLen = 0;

        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        /* 输出结果 */
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrAutotestQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_WrrAutotestQryCnf *wrrAutotestCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;
    VOS_UINT32                i;
    VOS_UINT32 rsltNum;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    wrrAutotestCnf = (MTA_AT_WrrAutotestQryCnf *)rcvMsg->content;
    result         = AT_OK;
    indexNum       = 0;
    rsltNum        = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrAutotestQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrAutotestQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CWAS_QUERY) {
        AT_WARN_LOG("AT_RcvMtaWrrAutotestQryCnf : Current Option is not AT_CMD_CWAS_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CWAS命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrAutotestCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        if (wrrAutotestCnf->wrrAutoTestRslt.rsltNum > 0) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%d",
                wrrAutotestCnf->wrrAutoTestRslt.rslt[0]);

            rsltNum = AT_MIN(wrrAutotestCnf->wrrAutoTestRslt.rsltNum, (MTA_AT_WRR_AUTOTEST_MAX_RSULT_NUM - 1));

            for (i = 1; i < rsltNum; i++) {
                g_atSendDataBuff.bufLen +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d",
                        wrrAutotestCnf->wrrAutoTestRslt.rslt[i]);
            }
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_RcvMtaGrrAutotestQryProc(MTA_AT_GasAutotestQryCnf *mtaAutotestCnf, VOS_UINT32 *ulRslt)
{
    VOS_UINT16 length;
    VOS_UINT32 rsltTemp;
    VOS_UINT32 i;

    length   = 0;
    rsltTemp = AT_OK;
    i        = 0;

    /* 邻区状态 */
    if (g_atParaList[AT_CGAS_PARA_GAS_STATUS].paraValue == GAS_AT_CMD_NCELL) {
        /* 邻区状态下，获取的测试结果个数应该是3的整数倍 */
        if ((mtaAutotestCnf->grrAutoTestRslt.rsltNum > 0) && ((mtaAutotestCnf->grrAutoTestRslt.rsltNum % 3) == 0) &&
            (mtaAutotestCnf->grrAutoTestRslt.rsltNum <= MTA_AT_GAS_AUTOTEST_MAX_RSULT_NUM)) {
            AT_FormatGasAtCmdRslt(&(mtaAutotestCnf->grrAutoTestRslt));
        } else if (mtaAutotestCnf->grrAutoTestRslt.rsltNum == 0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "0");
            g_atSendDataBuff.bufLen = length;
        } else {
            rsltTemp = AT_ERROR;
        }
    } else if (g_atParaList[AT_CGAS_PARA_GAS_STATUS].paraValue == GAS_AT_CMD_SCELL) {
        /* 服务小区状态 */
        if (mtaAutotestCnf->grrAutoTestRslt.rsltNum != AT_AUTOTEST_VALID_RSULT_NUM) {
            rsltTemp = AT_ERROR;
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,", mtaAutotestCnf->grrAutoTestRslt.rslt[0]);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "0x%X,", mtaAutotestCnf->grrAutoTestRslt.rslt[1] & 0xff);
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d", mtaAutotestCnf->grrAutoTestRslt.rslt[2]);
            g_atSendDataBuff.bufLen = length;
            rsltTemp                = AT_OK;
        }
    } else if (mtaAutotestCnf->grrAutoTestRslt.rsltNum > 0) {
        /* 其它命令 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d", mtaAutotestCnf->grrAutoTestRslt.rslt[0]);

        mtaAutotestCnf->grrAutoTestRslt.rsltNum = AT_MIN(mtaAutotestCnf->grrAutoTestRslt.rsltNum,
                                                         MTA_AT_GAS_AUTOTEST_MAX_RSULT_NUM);
        for (i = 1; i < mtaAutotestCnf->grrAutoTestRslt.rsltNum; i++) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", mtaAutotestCnf->grrAutoTestRslt.rslt[i]);
        }
        g_atSendDataBuff.bufLen = length;
    } else {
    }

    *ulRslt = rsltTemp;
}


VOS_UINT32 AT_RcvMtaGrrAutotestQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_GasAutotestQryCnf *grrAutotestCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    grrAutotestCnf = (MTA_AT_GasAutotestQryCnf *)rcvMsg->content;
    result         = AT_OK;
    indexNum       = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaGrrAutotestQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaGrrAutotestQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGAS_QUERY) {
        AT_WARN_LOG("AT_RcvMtaGrrAutotestQryCnf : Current Option is not AT_CMD_CGAS_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CGAS命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (grrAutotestCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        AT_RcvMtaGrrAutotestQryProc(grrAutotestCnf, &result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrCellinfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg            = VOS_NULL_PTR;
    MTA_AT_WrrCellinfoQryCnf *wrrCellinfoQryCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;
    VOS_UINT32                i;
    VOS_UINT32 cellNum;

    /* 初始化 */
    rcvMsg            = (AT_MTA_Msg *)msg;
    wrrCellinfoQryCnf = (MTA_AT_WrrCellinfoQryCnf *)rcvMsg->content;
    result            = AT_OK;
    indexNum          = 0;
    cellNum           = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrCellinfoQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrCellinfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CELLINFO_QUERY) {
        AT_WARN_LOG("AT_RcvMtaWrrCellinfoQryCnf : Current Option is not AT_CMD_CELLINFO_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CELLINFO命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrCellinfoQryCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        /* 没有获取的小区信息，打印0 */
        if (wrrCellinfoQryCnf->wrrCellInfo.cellNum == 0) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
                "no cellinfo rslt");
        }

        cellNum = AT_MIN(wrrCellinfoQryCnf->wrrCellInfo.cellNum, MTA_AT_WRR_MAX_NCELL_NUM);

        for (i = 0; i < cellNum; i++) {
            g_atSendDataBuff.bufLen +=
                (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%d,%d,%d,%d\r\n",
                    wrrCellinfoQryCnf->wrrCellInfo.wCellInfo[i].cellFreq,
                    wrrCellinfoQryCnf->wrrCellInfo.wCellInfo[i].primaryScramCode,
                    wrrCellinfoQryCnf->wrrCellInfo.wCellInfo[i].cpichRscp,
                    wrrCellinfoQryCnf->wrrCellInfo.wCellInfo[i].cpichEcN0);
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaDelCellEntityCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_DelCellentityCnf *delCellEntityCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    delCellEntityCnf = (MTA_AT_DelCellentityCnf *)rcvMsg->content;
    indexNum         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaDelCellentityCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaDelCellentityCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DEL_CELLENTITY_SET) {
        AT_WARN_LOG("AT_RcvMtaDelCellentityCnf : Current Option is not AT_CMD_DEL_CELLENTITY_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出结果 */
    if (delCellEntityCnf->result == VOS_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrMeanrptQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_WrrMeanrptQryCnf *wrrMeanrptQryCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;
    VOS_UINT16               length;
    VOS_UINT32               i;
    VOS_UINT32               cellNumLoop;
    VOS_UINT32 rptNum;
    VOS_UINT16 cellNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    wrrMeanrptQryCnf = (MTA_AT_WrrMeanrptQryCnf *)rcvMsg->content;
    result           = AT_OK;
    length           = 0;
    indexNum         = 0;
    rptNum           = 0;
    cellNum          = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrMeanrptQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrMeanrptQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MEANRPT_QUERY) {
        AT_WARN_LOG("AT_RcvMtaWrrMeanrptQryCnf : Current Option is not AT_CMD_MEANRPT_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^MEANRPT命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrMeanrptQryCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d", wrrMeanrptQryCnf->meanRptRslt.rptNum);

        rptNum = AT_MIN(wrrMeanrptQryCnf->meanRptRslt.rptNum, MTA_AT_WRR_MAX_MEANRPT_NUM);

        for (i = 0; i < rptNum; i++) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\r\n0x%X,%d",
                wrrMeanrptQryCnf->meanRptRslt.meanRptInfo[i].eventId,
                wrrMeanrptQryCnf->meanRptRslt.meanRptInfo[i].cellNum);

            cellNum = AT_MIN(wrrMeanrptQryCnf->meanRptRslt.meanRptInfo[i].cellNum, MTA_AT_WRR_ONE_MEANRPT_MAX_CELL_NUM);

            for (cellNumLoop = 0; cellNumLoop < cellNum; cellNumLoop++) {
                length +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d",
                        wrrMeanrptQryCnf->meanRptRslt.meanRptInfo[i].primaryScramCode[cellNumLoop]);
            }
        }
        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrCellSrhSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_WrrCellsrhSetCnf *wrrCellSrhSetCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    wrrCellSrhSetCnf = (MTA_AT_WrrCellsrhSetCnf *)rcvMsg->content;
    result           = AT_OK;
    indexNum         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_WAS_MNTN_SET_CELLSRH) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhSetCnf : Current Option is not AT_CMD_WAS_MNTN_SET_CELLSRH.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CELLSRH命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrCellSrhSetCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrCellSrhQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_WrrCellsrhQryCnf *wrrCellSrhQryCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    wrrCellSrhQryCnf = (MTA_AT_WrrCellsrhQryCnf *)rcvMsg->content;
    result           = AT_OK;
    indexNum         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_WAS_MNTN_QRY_CELLSRH) {
        AT_WARN_LOG("AT_RcvMtaWrrCellSrhQryCnf : Current Option is not AT_CMD_WAS_MNTN_QRY_CELLSRH.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrCellSrhQryCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            (TAF_INT32)wrrCellSrhQryCnf->cellSearchType);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrFreqLockSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_WrrFreqlockSetCnf *wrrFreqlockCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    wrrFreqlockCnf = (MTA_AT_WrrFreqlockSetCnf *)rcvMsg->content;
    result         = AT_OK;
    indexNum       = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FREQLOCK_SET) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockSetCnf : Current Option is not AT_CMD_FREQLOCK_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrFreqlockCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrFreqLockQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_WrrFreqlockQryCnf *wrrFreqlockCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    wrrFreqlockCnf = (MTA_AT_WrrFreqlockQryCnf *)rcvMsg->content;
    result         = AT_OK;
    indexNum       = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FREQLOCK_QUERY) {
        AT_WARN_LOG("AT_RcvMtaWrrFreqLockQryCnf : Current Option is not AT_CMD_FREQLOCK_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (wrrFreqlockCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        if (wrrFreqlockCnf->freqLockInfo.freqLockEnable == VOS_FALSE) {
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                (TAF_INT32)wrrFreqlockCnf->freqLockInfo.freqLockEnable);
        } else {
            g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                (TAF_INT32)wrrFreqlockCnf->freqLockInfo.freqLockEnable,
                (TAF_INT32)wrrFreqlockCnf->freqLockInfo.lockedFreq);
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrRrcVersionSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_WrrRrcVersionSetCnf *wrrVersionSetCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    wrrVersionSetCnf = (MTA_AT_WrrRrcVersionSetCnf *)rcvMsg->content;
    result           = AT_OK;
    indexNum         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RRC_VERSION_SET) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionSetCnf : Current Option is not AT_CMD_RRC_VERSION_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrVersionSetCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        /* 设置成功后重新读写一下NV */
        AT_ReadWasCapabilityNV();

        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaWrrRrcVersionQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_WrrRrcVersionQryCnf *wrrVersionQryCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    wrrVersionQryCnf = (MTA_AT_WrrRrcVersionQryCnf *)rcvMsg->content;
    result           = AT_OK;
    indexNum         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RRC_VERSION_QUERY) {
        AT_WARN_LOG("AT_RcvMtaWrrRrcVersionQryCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (wrrVersionQryCnf->result != VOS_OK) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            (TAF_INT32)wrrVersionQryCnf->rrcVersion);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAcInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_AcInfoQryCnf *acInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT16            length;
    VOS_UINT8             indexNum = 0;

    /* 初始化 */
    acInfoQueryCnf = (TAF_MMA_AcInfoQryCnf *)msg;
    length         = 0;
    result         = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(acInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAcInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaAcInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_LOCINFO_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ACINFO_READ) {
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询结果是否失败,如果失败则返回ERROR */
    if (acInfoQueryCnf->rslt != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_UINT8)(acInfoQueryCnf->cellCsAcInfo.srvDomain),          /* 上报服务域 */
            (VOS_UINT8)(acInfoQueryCnf->cellCsAcInfo.cellAcType),         /* 上报小区禁止接入类型 */
            (VOS_UINT8)(acInfoQueryCnf->cellCsAcInfo.restrictRegister),   /* 上报是否注册受限 */
            (VOS_UINT8)(acInfoQueryCnf->cellCsAcInfo.restrictPagingRsp)); /* 上报是否寻呼受限 */

        /* 换行 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_UINT8)(acInfoQueryCnf->cellPsAcInfo.srvDomain),          /* 上报服务域 */
            (VOS_UINT8)(acInfoQueryCnf->cellPsAcInfo.cellAcType),         /* 上报小区禁止接入类型 */
            (VOS_UINT8)(acInfoQueryCnf->cellPsAcInfo.restrictRegister),   /* 上报是否注册受限 */
            (VOS_UINT8)(acInfoQueryCnf->cellPsAcInfo.restrictPagingRsp)); /* 上报是否寻呼受限 */

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryCurcCnf(struct MsgCB *msg)
{
    AT_MTA_Msg        *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_CurcQryCnf *mtaAtQryCurcCnf = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum;
    VOS_UINT16         length;
    VOS_UINT32         loop;

    /* 初始化 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    mtaAtQryCurcCnf = (MTA_AT_CurcQryCnf *)rcvMsg->content;
    result          = AT_OK;
    indexNum        = 0;
    length          = 0;

    AT_PR_LOGI("Rcv Msg");

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryCurcCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryCurcCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CURC_READ) {
        AT_WARN_LOG("AT_RcvMtaQryCurcCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CURC查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (mtaAtQryCurcCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaAtQryCurcCnf->curcRptType);

        /* 如果<mode>参数为2，需要打印参数<report_cfg> */
        if (mtaAtQryCurcCnf->curcRptType == AT_MTA_RPT_GENERAL_CONTROL_CUSTOM) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",0x");

            for (loop = 0; loop < AT_CURC_RPT_CFG_MAX_SIZE; loop++) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mtaAtQryCurcCnf->rptCfg[loop]);
            }
        }
    }

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetUnsolicitedRptCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                  *rcvMsg            = VOS_NULL_PTR;
    MTA_AT_UnsolicitedRptSetCnf *setUnsolicitedCnf = VOS_NULL_PTR;
    VOS_UINT32                   result;
    VOS_UINT8                    indexNum;

    /* 初始化 */
    rcvMsg            = (AT_MTA_Msg *)msg;
    setUnsolicitedCnf = (MTA_AT_UnsolicitedRptSetCnf *)rcvMsg->content;
    result            = AT_OK;
    indexNum          = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetUnsolicitedRptCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetUnsolicitedRptCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_UNSOLICITED_RPT_SET) {
        AT_WARN_LOG("AT_RcvMtaSetUnsolicitedRptCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setUnsolicitedCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryUnsolicitedRptCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                  *rcvMsg            = VOS_NULL_PTR;
    MTA_AT_UnsolicitedRptQryCnf *qryUnsolicitedCnf = VOS_NULL_PTR;
    VOS_UINT32                   result;
    VOS_UINT8                    indexNum;

    /* 初始化 */
    rcvMsg            = (AT_MTA_Msg *)msg;
    qryUnsolicitedCnf = (MTA_AT_UnsolicitedRptQryCnf *)rcvMsg->content;
    result            = AT_OK;
    indexNum          = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryUnsolicitedRptCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryUnsolicitedRptCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_UNSOLICITED_RPT_READ) {
        AT_WARN_LOG("AT_RcvMtaQryUnsolicitedRptCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (qryUnsolicitedCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_CME_UNKNOWN;
    } else {
        /* ucIndex参数带下来 */
        result = AT_ProcMtaUnsolicitedRptQryCnf(indexNum, msg);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_ProcMtaUnsolicitedRptQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg)
{
    AT_MTA_Msg                  *rcvMsg            = VOS_NULL_PTR;
    MTA_AT_UnsolicitedRptQryCnf *qryUnsolicitedCnf = VOS_NULL_PTR;
    VOS_UINT32                   result;
    /* ucIndex通过上层函数传下来 */

    rcvMsg            = (AT_MTA_Msg *)msg;
    qryUnsolicitedCnf = (MTA_AT_UnsolicitedRptQryCnf *)rcvMsg->content;
    result            = AT_OK;

    switch (qryUnsolicitedCnf->reqType) {
        /* TIME查询的处理 */
        case AT_MTA_QRY_TIME_RPT_TYPE:
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                qryUnsolicitedCnf->u.timeRptFlg);
            break;

        /* pstQryUnsolicitedCnf数据结构体修改 */
        /* CTZR查询的处理 */
        case AT_MTA_QRY_CTZR_RPT_TYPE:
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                qryUnsolicitedCnf->u.ctzrRptFlg);
            break;

        /* CSSN查询的处理 */
        case AT_MTA_QRY_CSSN_RPT_TYPE:
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                qryUnsolicitedCnf->u.cssnRptFlg.cssiRptFlg, qryUnsolicitedCnf->u.cssnRptFlg.cssuRptFlg);
            break;

        /* CUSD查询的处理 */
        case AT_MTA_QRY_CUSD_RPT_TYPE:
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                qryUnsolicitedCnf->u.cusdRptFlg);
            break;

        default:
            result = AT_CME_UNKNOWN;
    }

    return result;
}


LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnGsm(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT16      length;

    length = 0;
    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.gCellSignInfo.rssiValue, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99,
            99, 99, 99, 99, 99, 99, 99, 0, 0, 0);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.gCellSignInfo.rssiValue, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0);
    }

    g_atSendDataBuff.bufLen = length;

    return;
}


LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnWcdma(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                                 VOS_UINT8 systemAppConfig)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT16      length;

    length = 0;
    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (cerssiInfoQueryCnf->cerssi.currentUtranMode == TAF_UTRANCTRL_UTRAN_MODE_FDD) {
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue,
                cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.ecioValue, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99,
                99, 99, 99, 99, 99, 99, 0, 0, 0);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue,
                cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.ecioValue, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0);
        }

        g_atSendDataBuff.bufLen = length;
    } else {
        /* 非fdd 3g 小区，ecio值为无效值255 */
        if (systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0,
                99, 99, 99, 99, 99, 99, 99, 99, 0, 0, 0);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0,
                0, 0);
        }

        g_atSendDataBuff.bufLen = length;
    }

    return;
}

#if (FEATURE_LTE == FEATURE_ON)

LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnLte(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig)
{
    VOS_UINT16      length;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    length = 0;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* LTE下的cerssi查询处理由L4A移到NAS，和atCerssiInfoCnfProc的处理相同 */
    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval, 0,
            0, 255,
            (cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp == AT_SIGNAL_INVALID_VALUE ?
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp :
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp / AT_SIGNAL_DIVISOR_EIGHT),
            (cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq == AT_SIGNAL_INVALID_VALUE ?
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq :
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq / AT_SIGNAL_DIVISOR_EIGHT),
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.sinr,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.ri,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[1],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rxANTNum,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR0],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR1],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR2],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR3],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR0],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR1],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR2],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR3], 0, 0, 0);

    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval, 0,
            0, 255,
            (cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp == AT_SIGNAL_INVALID_VALUE ?
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp :
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp / AT_SIGNAL_DIVISOR_EIGHT),
            (cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq == AT_SIGNAL_INVALID_VALUE ?
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq :
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq / AT_SIGNAL_DIVISOR_EIGHT),
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.sinr,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.ri,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[1], 0, 0, 0);
    }
    g_atSendDataBuff.bufLen = length;

    return;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

LOCAL VOS_VOID AT_ProcCerssiInfoQueryCampOnNr(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                              VOS_UINT8 systemAppConfig)
{
    VOS_UINT16      length;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    length = 0;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval, 0,
            0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99, 99, 99,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrp,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrq,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.l5GSinr);

        g_atSendDataBuff.bufLen = length;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval, 0,
            0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrp,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrq,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.l5GSinr);

        g_atSendDataBuff.bufLen = length;
    }

    return;
}
#endif


LOCAL VOS_VOID AT_ProcCerssiInfoQueryNotCampOn(TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf, VOS_UINT8 indexNum,
                                               VOS_UINT8 systemAppConfig)
{
    VOS_UINT16      length;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    length = 0;

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (systemAppConfig != SYSTEM_APP_ANDROID) {
        /* 刚开机没有接入模式，参数都返回无效值 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.gCellSignInfo.rssiValue,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99,
            99, 99, 99, 99, 99, 99, 0, 0, 0);
    } else {
        /* 刚开机没有接入模式，参数都返回无效值 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.gCellSignInfo.rssiValue,
            cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0);
    }

    g_atSendDataBuff.bufLen = length;

    return;
}


VOS_UINT32 AT_ProcCerssiInfoQuery(struct MsgCB *msg)
{
    TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    /* 初始化 */
    cerssiInfoQueryCnf = (TAF_MMA_CerssiInfoQryCnf *)msg;
    systemAppConfig = AT_GetSystemAppConfigAddr();
    indexNum        = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cerssiInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCerssiInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCerssiInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_GSM) {
        AT_ProcCerssiInfoQueryCampOnGsm(cerssiInfoQueryCnf, indexNum, *systemAppConfig);
    } else if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_WCDMA) {
        AT_ProcCerssiInfoQueryCampOnWcdma(cerssiInfoQueryCnf, indexNum, *systemAppConfig);
    }
#if (FEATURE_LTE == FEATURE_ON)
    else if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_LTE) {
        AT_ProcCerssiInfoQueryCampOnLte(cerssiInfoQueryCnf, indexNum, *systemAppConfig);
    }
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    else if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_NR) {
        AT_ProcCerssiInfoQueryCampOnNr(cerssiInfoQueryCnf, indexNum, *systemAppConfig);
    }
#endif

    /* 还没有收到小区信号 */
    else {
        AT_ProcCerssiInfoQueryNotCampOn(cerssiInfoQueryCnf, indexNum, *systemAppConfig);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_ProcCserssiInfoQuery(struct MsgCB *msg)
{
    TAF_MMA_CerssiInfoQryCnf *cserssiInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_INT16 rsrp;
    VOS_INT16 rsrq;
    VOS_UINT8       indexNum;
    AT_ModemNetCtx *netCtx          = VOS_NULL_PTR;
    VOS_UINT8      *systemAppConfig = VOS_NULL_PTR;

    /* 初始化 */
    cserssiInfoQueryCnf = (TAF_MMA_CerssiInfoQryCnf *)msg;
    length              = 0;
    indexNum            = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    systemAppConfig     = AT_GetSystemAppConfigAddr();

    rsrp = cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp == AT_SIGNAL_INVALID_VALUE ?
               cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp :
               cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp / AT_SIGNAL_DIVISOR_EIGHT;
    rsrq = cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq == AT_SIGNAL_INVALID_VALUE ?
               cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq :
               cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq / AT_SIGNAL_DIVISOR_EIGHT;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cserssiInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcCserssiInfoQuery: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_ProcCserssiInfoQuery: WARNING: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (cserssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_NR) {
        if (*systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99, 99, 99,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.l5GSinr);

            g_atSendDataBuff.bufLen = length;

        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrp,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.s5GRsrq,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.nrCellSignInfo.l5GSinr);

            g_atSendDataBuff.bufLen = length;
        }
    } else if (cserssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_LTE) {
        if (*systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, rsrp, rsrq, cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.sinr,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.ri,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[1],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rxANTNum,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR0],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR1],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR2],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.rsrpRx[AT_RX_RSRPR3],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR0],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR1],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR2],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rxAntInfo.sinrRx[AT_RX_SINRR3], 0, 0, 0);

        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, rsrp, rsrq,  cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.sinr,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.ri,
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[0],
                cserssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.cqi.cqi[1], 0, 0, 0);
        }
        g_atSendDataBuff.bufLen = length;
    } else {
        if (*systemAppConfig != SYSTEM_APP_ANDROID) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length,
                "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 99, 99, 99, 99, 99, 99, 99, 99, 0, 0, 0);

            g_atSendDataBuff.bufLen = length;

        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, netCtx->cerssiReportType, netCtx->cerssiMinTimerInterval,
                0, 0, 255, 0, 0, 0, 0, 0x7f7f, 0x7f7f, 0, 0, 0);

            g_atSendDataBuff.bufLen = length;
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}
#endif


VOS_INT16 AT_ConvertCerssiRssiToCesqRxlev(VOS_INT16 cerssiRssi)
{
    VOS_INT16 cesqRxlev;

    /*
     * 转换原则如下:
     * 0                        sCerssiRssi < -110dBm
     * 1             -110dBm <= sCerssiRssi < -109dBm
     * 2             -109dBm <= sCerssiRssi < -108dBm
     * 3             -108dBm <= sCerssiRssi < -107dBm
     * ...           ...
     * 62            -49dBm  <= sCerssiRssi < -48 dBm
     * 63            -48dBm  <= sCerssiRssi
     * 99            not known or not detectable
     */

    if (cerssiRssi < AT_CMD_CESQ_RXLEV_LOWER_BOUNDARY_VALUE) {
        cesqRxlev = AT_CMD_CESQ_RXLEV_MIN_VALUE;
    } else if (cerssiRssi < AT_CMD_CESQ_RXLEV_UPPER_BOUNDARY_VALUE) {
        cesqRxlev = cerssiRssi + AT_CMD_CESQ_RXLEV_CONVERT_BASE_VALUE;
    } else {
        cesqRxlev = AT_CMD_CESQ_RXLEV_MAX_VALUE;
    }

    return cesqRxlev;
}


VOS_INT16 AT_ConvertCerssiRscpToCesqRscp(VOS_INT16 cerssiRscp)
{
    VOS_INT16 cesqRscp;

    /*
     * 转换原则如下:
     * 0                        sCerssiRscp < -120dBm
     * 1             -120dBm <= sCerssiRscp < -119dBm
     * 2             -119dBm <= sCerssiRscp < -118dBm
     * 3             -118dBm <= sCerssiRscp < -117dBm
     * ...           ...
     * 95            -26dBm  <= sCerssiRscp < -25 dBm
     * 96            -25dBm  <= sCerssiRscp
     * 255            not known or not detectable
     */

    if (cerssiRscp < AT_CMD_CESQ_RSCP_LOWER_BOUNDARY_VALUE) {
        cesqRscp = AT_CMD_CESQ_RSCP_MIN_VALUE;
    } else if (cerssiRscp < AT_CMD_CESQ_RSCP_UPPER_BOUNDARY_VALUE) {
        cesqRscp = cerssiRscp + AT_CMD_CESQ_RSCP_CONVERT_BASE_VALUE;
    } else {
        cesqRscp = AT_CMD_CESQ_RSCP_MAX_VALUE;
    }

    return cesqRscp;
}


VOS_INT8 AT_ConvertCerssiEcnoToCesqEcno(VOS_INT8 ecno)
{
    VOS_INT8 cesqEcno;

    /*
     * 转换原则如下:
     * 【原有sEcioValue转换方法】
     *        0                        sEcioValue < -24dB
     *        1             -24dB   <= sEcioValue < -23.5dB
     *        2             -23.5dB <= sEcioValue < -23dB
     *        ...           ...
     *        47            -1dB    <= sEcioValue < -0.5dB
     *        48            -0.5dB  <= sEcioValue < 0dB
     *        49             0dB    <= sEcioValue
     *        255            not known or not detectable
     * 【新增cEcno接口, 比原有的sEcioValue增加0.5精度，目前仅为AT命令CESQ使用。范围为: -49~0】
     *        0                        cEcno < -48dB
     *        1             -48dB   <= cEcno < -47dB
     *        2             -47dB   <= cEcno < -46dB
     *        3             -46dB   <= cEcno < -45dB
     *        4             -45dB   <= cEcno < -44dB
     *        ...           ...
     *        47            -2dB    <= cEcno < -1dB
     *        48            -1dB    <= cEcno < 0dB
     *        49            0dB     <= cEcno
     *        255           not known or not detectable
     */

    if (ecno < AT_CMD_CESQ_ECNO_LOWER_BOUNDARY_VALUE) {
        cesqEcno = AT_CMD_CESQ_ECNO_MIN_VALUE;
    } else if (ecno < AT_CMD_CESQ_ECNO_UPPER_BOUNDARY_VALUE) {
        cesqEcno = ecno + AT_CMD_CESQ_ECNO_CONVERT_BASE_VALUE;
    } else {
        cesqEcno = AT_CMD_CESQ_ECNO_MAX_VALUE;
    }

    return cesqEcno;
}


VOS_INT16 AT_ConvertCerssiRsrqToCesqRsrq(VOS_INT16 cerssiRsrq)
{
    VOS_INT16 cesqRsrq;
    VOS_INT16 result;

    /*
     * 转换原则如下:
     * 【Rsrq底层上报原值，这里除以4, 所以没有小数, 目前sCerssiRsrq按照-40~-6上报的】
     *        0                       sCerssiRsrq < -39dB
     *        1             -39dB  <= sCerssiRsrq < -38dB
     *        2             -38dB  <= sCerssiRsrq < -37dB
     *        3             -37dB  <= sCerssiRsrq < -36dB
     *        ...           ...
     *        33            -7dB   <= sCerssiRsrq < -6dB
     *        34            -6 dB  <= sCerssiRsrq
     *        255           not known or not detectable
     */

    result = cerssiRsrq / AT_SIGNAL_DIVISOR_FOUR;

    if (result < AT_CMD_CESQ_RSRQ_LOWER_BOUNDARY_VALUE) {
        cesqRsrq = AT_CMD_CESQ_RSRQ_MIN_VALUE;
    } else if (result < AT_CMD_CESQ_RSRQ_UPPER_BOUNDARY_VALUE) {
        cesqRsrq = result + AT_CMD_CESQ_RSRQ_CONVERT_BASE_VALUE;
    } else {
        cesqRsrq = AT_CMD_CESQ_RSRQ_MAX_VALUE;
    }

    return cesqRsrq;
}


VOS_INT16 AT_ConvertCerssiRsrpToCesqRsrp(VOS_INT16 cerssiRsrp)
{
    VOS_INT16 cesqRsrp;

    /*
     * 转换原则如下:
     * 0                        sCerssiRsrp < -140dBm
     * 1             -140dBm <= sCerssiRsrp < -139dBm
     * 2             -139dBm <= sCerssiRsrp < -138dBm
     * 3             -138dBm <= sCerssiRsrp < -137dBm
     * ...           ...
     * 96            -45dBm  <= sCerssiRsrp < -44 dBm
     * 97            -44dBm  <= sCerssiRsrp
     * 255            not known or not detectable
     */
    /* LRRC上报给空口的测量报告按照除法处理，此处需要保持一致 */
    cerssiRsrp = cerssiRsrp / AT_SIGNAL_DIVISOR_EIGHT;

    if (cerssiRsrp < AT_CMD_CESQ_RSRP_LOWER_BOUNDARY_VALUE) {
        cesqRsrp = AT_CMD_CESQ_RSRP_MIN_VALUE;
    } else if (cerssiRsrp < AT_CMD_CESQ_RSRP_UPPER_BOUNDARY_VALUE) {
        cesqRsrp = cerssiRsrp + AT_CMD_CESQ_RSRP_CONVERT_BASE_VALUE;
    } else {
        cesqRsrp = AT_CMD_CESQ_RSRP_MAX_VALUE;
    }

    return cesqRsrp;
}


VOS_UINT32 AT_ProcCesqInfoSet(struct MsgCB *msg)
{
    TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    cerssiInfoQueryCnf = (TAF_MMA_CerssiInfoQryCnf *)msg;
    length             = 0;
    indexNum           = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cerssiInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_ProcCesqInfoSet:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_ProcCesqInfoSet : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* GSM */
    if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_GSM) {
        length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName,
            AT_ConvertCerssiRssiToCesqRxlev(cerssiInfoQueryCnf->cerssi.aRssi[0].u.gCellSignInfo.rssiValue),
            cerssiInfoQueryCnf->cerssi.aRssi[0].channalQual, AT_CMD_CESQ_RSCP_INVALID_VALUE,
            AT_CMD_CESQ_ECNO_INVALID_VALUE, AT_CMD_CESQ_RSRQ_INVALID_VALUE, AT_CMD_CESQ_RSRP_INVALID_VALUE);

        g_atSendDataBuff.bufLen = length;

        /* 输出结果 */
        At_FormatResultData(indexNum, AT_OK);
        return VOS_OK;
    }

    if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_WCDMA) {
        /* FDD 3G小区 */
        if (cerssiInfoQueryCnf->cerssi.currentUtranMode == TAF_UTRANCTRL_UTRAN_MODE_FDD) {
            length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, AT_CMD_CESQ_RXLEV_INVALID_VALUE,
                AT_CMD_CESQ_BER_INVALID_VALUE,
                AT_ConvertCerssiRscpToCesqRscp(cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue),
                AT_ConvertCerssiEcnoToCesqEcno(cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.ecno),
                AT_CMD_CESQ_RSRQ_INVALID_VALUE, AT_CMD_CESQ_RSRP_INVALID_VALUE);
            g_atSendDataBuff.bufLen = length;

            /* 输出结果 */
            At_FormatResultData(indexNum, AT_OK);
            return VOS_OK;
        } else {
            /* 非FDD 3G小区，ecio值为无效值255 */
            length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, AT_CMD_CESQ_RXLEV_INVALID_VALUE,
                AT_CMD_CESQ_BER_INVALID_VALUE,
                AT_ConvertCerssiRscpToCesqRscp(cerssiInfoQueryCnf->cerssi.aRssi[0].u.wCellSignInfo.rscpValue),
                AT_CMD_CESQ_ECNO_INVALID_VALUE, AT_CMD_CESQ_RSRQ_INVALID_VALUE, AT_CMD_CESQ_RSRP_INVALID_VALUE);

            g_atSendDataBuff.bufLen = length;

            /* 输出结果 */
            At_FormatResultData(indexNum, AT_OK);
            return VOS_OK;
        }
    }

#if (FEATURE_LTE == FEATURE_ON)
    /* LTE */
    if (cerssiInfoQueryCnf->cerssi.ratType == TAF_MMA_RAT_LTE) {
        length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, AT_CMD_CESQ_RXLEV_INVALID_VALUE,
            AT_CMD_CESQ_BER_INVALID_VALUE, AT_CMD_CESQ_RSCP_INVALID_VALUE, AT_CMD_CESQ_ECNO_INVALID_VALUE,
            AT_ConvertCerssiRsrqToCesqRsrq(cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrq),
            AT_ConvertCerssiRsrpToCesqRsrp(cerssiInfoQueryCnf->cerssi.aRssi[0].u.lCellSignInfo.rsrp));

        g_atSendDataBuff.bufLen = length;

        /* 输出结果 */
        At_FormatResultData(indexNum, AT_OK);
        return VOS_OK;
    }
#endif

    /* 刚开机没有接入模式，参数都返回无效值 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        AT_CMD_CESQ_RXLEV_INVALID_VALUE, AT_CMD_CESQ_BER_INVALID_VALUE, AT_CMD_CESQ_RSCP_INVALID_VALUE,
        AT_CMD_CESQ_ECNO_INVALID_VALUE, AT_CMD_CESQ_RSRQ_INVALID_VALUE, AT_CMD_CESQ_RSRP_INVALID_VALUE);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCerssiInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_CerssiInfoQryCnf *cerssiInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum           = 0;

    /* 初始化 */
    cerssiInfoQueryCnf = (TAF_MMA_CerssiInfoQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cerssiInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCerssiInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCerssiInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断查询结果是否失败,如果失败则返回ERROR */
    if (cerssiInfoQueryCnf->errorCause != TAF_ERR_NO_ERROR) {
        At_FormatResultData(indexNum, AT_ERROR);

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        return VOS_ERR;
    } else {
        switch (g_atClientTab[indexNum].cmdCurrentOpt) {
            case AT_CMD_CERSSI_READ:
                AT_ProcCerssiInfoQuery((struct MsgCB *)cerssiInfoQueryCnf);
                break;

            case AT_CMD_CESQ_SET:
                AT_ProcCesqInfoSet((struct MsgCB *)cerssiInfoQueryCnf);
                break;

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
            case AT_CMD_CSERSSI_READ:
                AT_ProcCserssiInfoQuery((struct MsgCB *)cerssiInfoQueryCnf);
                break;
#endif

            default:
                return VOS_ERR;
        }
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaBodySarSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_ResultCnf *bodySarSetCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum = 0;

    /* 初始化 */
    rcvMsg        = (AT_MTA_Msg *)msg;
    bodySarSetCnf = (MTA_AT_ResultCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaBodySarSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaBodySarSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BODYSARON_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断回复消息中的错误码 */
    if (bodySarSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;

        /* 更新BODYSAR 状态 */
        if (g_atBodySarState == AT_MTA_BODY_SAR_OFF) {
            g_atBodySarState = AT_MTA_BODY_SAR_ON;
        } else {
            g_atBodySarState = AT_MTA_BODY_SAR_OFF;
        }
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_ReportResetCmd(AT_ResetReportCauseUint32 cause)
{
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_RESET].text, cause,
        g_atCrLf);

    /* 需要上报给上层的双RIL */
    At_SendResultData(AT_CLIENT_ID_APP, g_atSndCodeAddress, length);
    At_SendResultData(AT_CLIENT_ID_PCUI, g_atSndCodeAddress, length);

#if (MULTI_MODEM_NUMBER > 1)
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    At_SendResultData(AT_CLIENT_ID_APP5, g_atSndCodeAddress, length);
#endif
    At_SendResultData(AT_CLIENT_ID_CTRL, g_atSndCodeAddress, length);
#if (MULTI_MODEM_NUMBER > 2)
#if (FEATURE_VCOM_EXT == FEATURE_ON)
    At_SendResultData(AT_CLIENT_ID_APP20, g_atSndCodeAddress, length);
#endif
    At_SendResultData(AT_CLIENT_ID_PCUI2, g_atSndCodeAddress, length);
#endif
#endif

    return;
}


VOS_VOID AT_StopAllTimer(VOS_VOID)
{
    VOS_UINT8        modemIndex;
    VOS_UINT32       clientIndex;
    VOS_UINT32       timerName;
    AT_ModemCcCtx   *ccCtx         = VOS_NULL_PTR;
    AT_ParseContext *parseContext  = VOS_NULL_PTR;
    AT_ClientManage *clientContext = VOS_NULL_PTR;

    for (modemIndex = 0; modemIndex < MODEM_ID_BUTT; modemIndex++) {
        ccCtx = AT_GetModemCcCtxAddrFromClientId(modemIndex);

        /* 停止S0定时器 */
        if (ccCtx->s0TimeInfo.timerStart == VOS_TRUE) {
            timerName = ccCtx->s0TimeInfo.timerName;

            AT_StopRelTimer(timerName, &(ccCtx->s0TimeInfo.s0Timer));
            ccCtx->s0TimeInfo.timerStart = VOS_FALSE;
            ccCtx->s0TimeInfo.timerName  = 0;
        }
    }

    for (clientIndex = 0; clientIndex < AT_MAX_CLIENT_NUM; clientIndex++) {
        parseContext = &(g_parseContext[clientIndex]);
        AT_StopRelTimer(clientIndex, &parseContext->timer);

        clientContext = &(g_atClientTab[clientIndex]);
        AT_StopRelTimer(clientIndex, &clientContext->hTimer);
    }

    return;
}


VOS_VOID AT_ResetParseCtx(VOS_VOID)
{
    VOS_UINT8        clientIndex;
    AT_ParseContext *parseContext = VOS_NULL_PTR;

    for (clientIndex = 0; clientIndex < AT_MAX_CLIENT_NUM; clientIndex++) {
        parseContext = &(g_parseContext[clientIndex]);

        parseContext->clientStatus = AT_FW_CLIENT_STATUS_READY;

        /* 清空所有的缓存的AT命令 */
        AT_ClearBlockCmdInfo(clientIndex);

        /* 重置AT组合命令解析的信息 */
        At_ResetCombinParseInfo(clientIndex);

        parseContext->mode       = AT_NORMAL_MODE;
        parseContext->dataLen    = 0;
        parseContext->cmdLineLen = 0;

        (VOS_VOID)memset_s(parseContext->dataBuff, sizeof(parseContext->dataBuff), 0x00, sizeof(parseContext->dataBuff));

        if (parseContext->cmdLine != NULL) {
            AT_FREE(parseContext->cmdLine);
            parseContext->cmdLine = VOS_NULL_PTR;
            parseContext->cmdLine = NULL;
        }
    }

    return;
}


VOS_VOID AT_ResetClientTab(VOS_VOID)
{
    VOS_UINT32 loop;
    VOS_UINT32 indexNum;
    VOS_UINT32 clientIndex;

    for (clientIndex = 0; clientIndex < AT_MAX_CLIENT_NUM; clientIndex++) {
        /* 清空对应表项 */
        (VOS_VOID)memset_s(&g_atClientTab[clientIndex], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    }

    /* USB PCUI */
    indexNum                         = AT_CLIENT_ID_PCUI;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_PCUI;
    g_atClientTab[indexNum].portNo   = DMS_PORT_PCUI;
    g_atClientTab[indexNum].userType = AT_USBCOM_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* USB Control */
    indexNum                         = AT_CLIENT_ID_CTRL;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_CTRL;
    g_atClientTab[indexNum].portNo   = DMS_PORT_CTRL;
    g_atClientTab[indexNum].userType = AT_CTR_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    indexNum                         = AT_CLIENT_ID_PCUI2;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_PCUI2;
    g_atClientTab[indexNum].portNo   = DMS_PORT_PCUI2;
    g_atClientTab[indexNum].userType = AT_PCUI2_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* USB MODEM */
    indexNum                         = AT_CLIENT_ID_MODEM;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_MODEM;
    g_atClientTab[indexNum].portNo   = DMS_PORT_MODEM;
    g_atClientTab[indexNum].userType = AT_MODEM_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* NDIS MODEM */
    indexNum                         = AT_CLIENT_ID_NDIS;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_NDIS;
    g_atClientTab[indexNum].portNo   = DMS_PORT_NCM_CTRL;
    g_atClientTab[indexNum].userType = AT_NDIS_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* USB UART */
    indexNum                         = AT_CLIENT_ID_UART;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_UART;
    g_atClientTab[indexNum].portNo   = DMS_PORT_UART;
    g_atClientTab[indexNum].userType = AT_UART_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

#if (FEATURE_AT_HSUART == FEATURE_ON)
    /* HSUART */
    indexNum                         = AT_CLIENT_ID_HSUART;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_HSUART;
    g_atClientTab[indexNum].portNo   = DMS_PORT_HSUART;
    g_atClientTab[indexNum].userType = AT_HSUART_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;
#endif

    /* SOCK */
    indexNum                         = AT_CLIENT_ID_SOCK;
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_SOCK;
    g_atClientTab[indexNum].portNo   = DMS_PORT_SOCK;
    g_atClientTab[indexNum].userType = AT_SOCK_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* APP */
    for (loop = 0; loop < DMS_APP_PORT_SIZE; loop++) {
        indexNum                             = AT_CLIENT_ID_APP + loop;
        g_atClientTab[indexNum].clientId = AT_CLIENT_ID_APP + (VOS_UINT16)loop;
        g_atClientTab[indexNum].portNo   = DMS_PORT_APP + (VOS_UINT16)loop;
        g_atClientTab[indexNum].userType = AT_APP_USER;
        g_atClientTab[indexNum].used     = AT_CLIENT_USED;
    }

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    /* CMUX */
    for (loop = 0; loop < MAX_CMUX_PORT_NUM; loop++) {
        indexNum                         = AT_CLIENT_ID_CMUXAT + loop;
        g_atClientTab[indexNum].clientId = AT_CLIENT_ID_CMUXAT + loop;
        g_atClientTab[indexNum].portNo   = DMS_PORT_CMUXAT + loop;
        g_atClientTab[indexNum].userType = AT_CMUXAT_USER + loop;
        g_atClientTab[indexNum].used     = AT_CLIENT_USED;
    }
#endif
}


VOS_VOID AT_ResetOtherCtx(VOS_VOID)
{
    /* <CR> */
    g_atS3 = 13;

    g_atCrLf[0] = g_atS3;

    /* <LF> */
    g_atS4 = 10;

    g_atCrLf[1] = g_atS4;

    /* <DEL> */
    g_atS5 = 8;

    /* 指示命令返回码类型 */
    g_atVType = AT_V_ENTIRE_TYPE;

    /* 指示TE编码类型 */
    g_atCscsType = AT_CSCS_IRA_CODE;

    g_atEType = AT_E_ECHO_CMD;

    /*
     * CONNECT <text> result code is given upon entering online data state.
     * Dial tone and busy detection are disabled.
     */
    g_atXType = 0;

    /* 默认LTE上下行category都为6 */
    g_atDlRateCategory.lteUeDlCategory = AT_LTE_CATEGORY_DEFAULT_VALUE;
    g_atDlRateCategory.lteUeUlCategory = AT_LTE_CATEGORY_DEFAULT_VALUE;
}


VOS_VOID AT_ResetMscLevel(DMS_PortIdUint16 portId)
{
    /* 与AT变量保持一致,拉低所有管脚信号 */
    DMS_PORT_DeassertDcd(portId);
    DMS_PORT_DeassertDsr(portId);
    DMS_PORT_DeassertCts(portId);
    DMS_PORT_DeassertRi(portId);
}

#if (FEATURE_ON == FEATURE_AT_HSUART)

VOS_VOID AT_InitUartCtrlInfo(VOS_VOID)
{
    AT_UartCtx *uartCtx = VOS_NULL_PTR;

    uartCtx = AT_GetUartCtxAddr();

    /* 初始化FLOW CTRL默认值 */
    uartCtx->flowCtrl.dceByDte = AT_UART_DEFAULT_FC_DCE_BY_DTE;
    uartCtx->flowCtrl.dteByDce = AT_UART_DEFAULT_FC_DTE_BY_DCE;

    /* 初始化UART LINE CTRL默认值 */
    DMS_PORT_SetDcdMode(DMS_PORT_UART_DEFAULT_DCD_MODE);
    DMS_PORT_SetDtrMode(DMS_PORT_UART_DEFAULT_DTR_MODE);
    DMS_PORT_SetDsrMode(DMS_PORT_UART_DEFAULT_DSR_MODE);
}


VOS_VOID AT_ResetHsuartPppInfo(VOS_UINT8 indexNum)
{
    DMS_PortIdUint16    portId;

    portId   = AT_GetDmsPortIdByClientId(indexNum);

    /* 切换为命令模式 */
    DMS_PORT_ResumeCmdMode(portId);

    /* 清除缓存 */
    DMS_PORT_FlushTxData(portId);

    /* 恢复为低水线 */
    DMS_PORT_SetUartWaterMarkFlg(VOS_FALSE);

    /* 重置客户端信息 */
    AT_HSUART_InitLink(indexNum);

    /* 重置流控、管脚配置模式 */
    AT_InitUartCtrlInfo();

    /* 重置管脚信号 */
    AT_ResetMscLevel(portId);

    /* 重置波特率 帧格式配置 */
    AT_HSUART_ConfigDefaultPara();

}
#endif


VOS_VOID AT_ResetModemPppInfo(VOS_UINT8 indexNum)
{
    DMS_PortIdUint16    portId;

    portId   = AT_GetDmsPortIdByClientId(indexNum);

    /* 切换为命令模式 */
    DMS_PORT_ResumeCmdMode(portId);

    /* 与AT变量保持一致,拉低所有管脚信号 */
    AT_ResetMscLevel(portId);
}


VOS_VOID AT_ResetPppDailInfo(VOS_VOID)
{
#if (FEATURE_ON == FEATURE_AT_HSUART)
    AT_ResetHsuartPppInfo(AT_CLIENT_ID_HSUART);
#endif
    AT_ResetModemPppInfo(AT_CLIENT_ID_MODEM);

}

VOS_UINT32 AT_RcvCcpuResetStartInd(struct MsgCB *msg)
{
    AT_PR_LOGI("enter %u", VOS_GetSlice());

#if (FEATURE_CCORE_RESET_RPT_BY_FILE_NODE != FEATURE_ON)
    /* 上报^RESET:0命令 */
    AT_ReportResetCmd(AT_RESET_REPORT_CAUSE_CCPU_START);
#endif

    /* 停止所有启动的内部定时器 */
    AT_StopAllTimer();

    /* 初始化上下文信息 */
    AT_InitCtx();

    AT_PR_LOGI("nv write begin %u", VOS_GetSlice());

    AT_PR_LOGI("nv write end %u", VOS_GetSlice());

    /* 读取NV项 */
    AT_ReadNV();

    /* 装备初始化 */
    AT_InitDeviceCmd();

    /* STK初始化 */
    AT_InitStk();

    AT_PR_LOGI("nv read end %u", VOS_GetSlice());

    /* AT模块参数的初始化 */
    AT_InitPara();

    /* 重置客户端解析信息 */
    AT_ResetParseCtx();

    /* 重置用户信息 */
    AT_ResetClientTab();

    /* 重置其他散落的全局变量 */
    AT_ResetOtherCtx();

    /* 重置PPP拨号相关信息 */
    AT_ResetPppDailInfo();

    AT_PR_LOGI("set modem state %u", VOS_GetSlice());

    /* C核单独复位后设置设备节点，当前未启动 */
    AT_SetModemState(MODEM_ID_0, PS_FALSE);

    AT_PR_LOGI("leave %u", VOS_GetSlice());

    /* 释放信号量，使得调用API任务继续运行 */
    VOS_SmV(AT_GetResetSem());

    return VOS_OK;
}

VOS_UINT32 AT_RcvCcpuResetEndInd(struct MsgCB *msg)
{
    AT_PR_LOGI("enter %u", VOS_GetSlice());

    /* 设置复位完成的标志 */
    AT_SetResetFlag(VOS_FALSE);

#if (FEATURE_CCORE_RESET_RPT_BY_FILE_NODE != FEATURE_ON)
    /* 上报^RESET:1命令 */
    AT_ReportResetCmd(AT_RESET_REPORT_CAUSE_CCPU_END);
#endif

    AT_PR_LOGI("leave %u", VOS_GetSlice());

    return VOS_OK;
}

VOS_UINT32 AT_RcvHifiResetBeginInd(struct MsgCB *msg)
{
    APP_VC_MsgUint16          msgName = APP_VC_MSG_HIFI_RESET_BEGIN_NTF;
    APP_VC_HifiResetTypeUint8 hifiResetType = APP_VC_HIFI_RESET_FROM_CP;
    AT_PR_LOGI("%u", VOS_GetSlice());

    /* 上报^RESET:2命令 */
    AT_ReportResetCmd(AT_RESET_REPORT_CAUSE_HIFI_BEGIN);

    AT_DBG_SAVE_HIFI_RESET_NUM(1);

    APP_VC_SendHifiResetNtf(I0_WUEPS_PID_VC, hifiResetType, msgName);

#if (MULTI_MODEM_NUMBER == 2)
    APP_VC_SendHifiResetNtf(I1_WUEPS_PID_VC, hifiResetType, msgName);
#endif

#if (MULTI_MODEM_NUMBER == 3)
    APP_VC_SendHifiResetNtf(I1_WUEPS_PID_VC, hifiResetType, msgName);
    APP_VC_SendHifiResetNtf(I2_WUEPS_PID_VC, hifiResetType, msgName);
#endif

    return VOS_OK;
}


VOS_UINT32 AT_RcvHifiResetEndInd(struct MsgCB *msg)
{
    APP_VC_MsgUint16          msgName = APP_VC_MSG_HIFI_RESET_END_NTF;
    APP_VC_HifiResetTypeUint8 hifiResetType = APP_VC_HIFI_RESET_FROM_CP;
    AT_PR_LOGI("%u", VOS_GetSlice());

    /* 上报^RESET:3命令 */
    AT_ReportResetCmd(AT_RESET_REPORT_CAUSE_HIFI_END);
    APP_VC_SendHifiResetNtf(I0_WUEPS_PID_VC, hifiResetType, msgName);
#if (MULTI_MODEM_NUMBER == 2)
    APP_VC_SendHifiResetNtf(I1_WUEPS_PID_VC, hifiResetType, msgName);
#endif
#if (MULTI_MODEM_NUMBER == 3)
    APP_VC_SendHifiResetNtf(I1_WUEPS_PID_VC, hifiResetType, msgName);
    APP_VC_SendHifiResetNtf(I2_WUEPS_PID_VC, hifiResetType, msgName);
#endif

    return VOS_OK;
}


VOS_VOID AT_SpyMsgProc(struct MsgCB *msg)
{
    TEMP_PROTECT_EventAtInd *dataMsg = (TEMP_PROTECT_EventAtInd *)msg;

    switch (dataMsg->msgId) {
#if (FEATURE_LTE == FEATURE_ON)
        case ID_TEMPPRT_AT_EVENT_IND:
            AT_ProcTempprtEventInd(dataMsg);
            break;
#endif

        case ID_TEMPPRT_STATUS_AT_EVENT_IND:
            AT_RcvTempprtStatusInd((struct MsgCB *)dataMsg);
            break;

        default:
            AT_WARN_LOG("AT_SpyMsgProc:WARNING:Wrong Msg!\n");
            break;
    }

    return;
}


VOS_UINT32 AT_RcvTempprtStatusInd(struct MsgCB *msg)
{
    TEMP_PROTECT_EventAtInd *tempPrt = VOS_NULL_PTR;
    TAF_TempProtectConfig    tempProtectPara;
    VOS_UINT32               result;

    (VOS_VOID)memset_s(&tempProtectPara, sizeof(tempProtectPara), 0x00, sizeof(tempProtectPara));

    /* 读取温度保护状态主动上报NV项 */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_TEMP_PROTECT_CONFIG, &tempProtectPara,
                               sizeof(TAF_TempProtectConfig));

    if (result != NV_OK) {
        AT_ERR_LOG("AT_RcvTempprtStatusInd: Read NV fail");
        return VOS_ERR;
    }

    if (tempProtectPara.spyStatusIndSupport == AT_TEMPPRT_STATUS_IND_ENABLE) {
        tempPrt = (TEMP_PROTECT_EventAtInd *)msg;

        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^THERM: %d%s", g_atCrLf, tempPrt->tempProtectEvent, g_atCrLf);

        At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

        return VOS_OK;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaImeiVerifyQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg     = VOS_NULL_PTR;
    VOS_UINT32 *imeiVerify = VOS_NULL_PTR;
    VOS_UINT16  length;
    VOS_UINT8   indexNum = 0;

    /* 初始化 */
    rcvMsg     = (AT_MTA_Msg *)msg;
    imeiVerify = (VOS_UINT32 *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaImeiVerifyQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaImeiVerifyQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMEI_VERIFY_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 打印^IMEIVERIFY */
    length = 0;
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^IMEIVERIFY: ");

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d", *imeiVerify);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCgsnQryCnf(struct MsgCB *msg)
{
    VOS_UINT32         i;
    VOS_UINT8          checkData;
    VOS_UINT8          indexNum = 0;
    AT_MTA_Msg        *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_CgsnQryCnf *cgsn     = VOS_NULL_PTR;
    VOS_UINT8          imeiAscii[TAF_PH_IMEI_LEN];

    /* 初始化 */
    checkData = 0;
    (VOS_VOID)memset_s(imeiAscii, sizeof(imeiAscii), 0x00, sizeof(imeiAscii));

    rcvMsg = (AT_MTA_Msg *)msg;
    cgsn   = (MTA_AT_CgsnQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCgsnQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCgsnQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGSN_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 读取IMEI信息，先减去最后2个元素的长度，在最后另外赋值，一次循环赋值2个元素 */
    for (i = 0; i < (TAF_PH_IMEI_LEN - 2); i += 2) {
        imeiAscii[i]       = cgsn->imei[i] + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
        imeiAscii[i + 1UL] = cgsn->imei[i + 1UL] + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
        /* 除10是为了取高位，余10是为了取低位 */
        checkData += (TAF_UINT8)(cgsn->imei[i] + ((cgsn->imei[i + 1UL] * AT_DOUBLE_LENGTH) / AT_DECIMAL_BASE_NUM) +
                                ((cgsn->imei[i + 1UL] * AT_DOUBLE_LENGTH) % AT_DECIMAL_BASE_NUM));
    }
    checkData = (AT_DECIMAL_BASE_NUM - (checkData % AT_DECIMAL_BASE_NUM)) % AT_DECIMAL_BASE_NUM;

    /* 给最后两位元素另外赋值 */
    imeiAscii[TAF_PH_IMEI_LEN - 2] = checkData + AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE;
    imeiAscii[TAF_PH_IMEI_LEN - 1] = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s", imeiAscii);

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_CGSN;
    At_FormatResultData(indexNum, AT_OK);
    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCopnInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_CopnInfoQryCnf *copnInfo = VOS_NULL_PTR;
    TAF_PH_OperatorName    *plmnName = VOS_NULL_PTR;
    VOS_UINT16              fromIndex;
    VOS_UINT16              length;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              loop;
    VOS_UINT32              ret;

    /* 初始化 */
    copnInfo = (TAF_MMA_CopnInfoQryCnf *)msg;
    length   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(copnInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCopnInfoQueryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCopnInfoQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_COPN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPN_QRY) {
        return VOS_ERR;
    }

    /* 参照+COPS或^MMPLMNINFO命令处理 */
    plmnName = (TAF_PH_OperatorName *)copnInfo->content;

    for (loop = 0; loop < copnInfo->plmnNum; loop++) {
        /*  +COPN: <operator in numeric format><operator in long alphanumeric format>     */
        /* 输出命令名 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* 输出数字格式运营商名称 */
        length += (VOS_UINT16)AT_FormatReportString( AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)(g_atSndCodeAddress + length), "\"%X%X%X",
            (TAF_MMA_PLMN_MCC_DIGIT3_MASK & plmnName->plmnId.mcc) >> TAF_MMA_PLMN_MCC_DIGIT3_OFFSET,
            (TAF_MMA_PLMN_MCC_DIGIT2_MASK & plmnName->plmnId.mcc) >> TAF_MMA_PLMN_MCC_DIGIT2_OFFSET,
            (TAF_MMA_PLMN_MCC_DIGIT1_MASK & plmnName->plmnId.mcc) >> TAF_MMA_PLMN_MCC_DIGIT1_OFFSET);

        if ((TAF_MMA_PLMN_MNC_DIGIT3_MASK & plmnName->plmnId.mnc) != TAF_MMA_PLMN_MNC_DIGIT3_MASK) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)(g_atSndCodeAddress + length), "%X",
                (TAF_MMA_PLMN_MNC_DIGIT3_MASK & plmnName->plmnId.mnc) >> TAF_MMA_PLMN_MNC_DIGIT3_OFFSET);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%X%X\"",
            (TAF_MMA_PLMN_MNC_DIGIT2_MASK & plmnName->plmnId.mnc) >> TAF_MMA_PLMN_MNC_DIGIT2_OFFSET,
            (TAF_MMA_PLMN_MNC_DIGIT1_MASK & plmnName->plmnId.mnc) >> TAF_MMA_PLMN_MNC_DIGIT1_OFFSET);

        /* 添加逗号分隔符 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"");

        /* 输出运营商长名 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", plmnName->operatorNameLong);

        /* 添加引号 */
        /* 添加逗号分隔符 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);

        length = 0;

        plmnName++;
    }

    /*
     * AT模块根据MMA上报的运营商个数来确认运营商信息是否收集完成: 小于请求的运营商
     * 个数则认为运营商信息已经收集完成，原因如下所述:
     * AT模块每次请求50条运营商信息，起始位置为已经输出的最后一个运营商的下一条运营商信息索引
     * 如果待输出的运营商信息总数不足50条，则按实际的输出，
     * 如果运营商数为50的倍数，则AT会再发一次运营商信息请求，MMA回复的运营商信息总数为0
     */
    if (copnInfo->plmnNum < TAF_MMA_COPN_PLMN_MAX_NUM) {
        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);
    } else {
        fromIndex = copnInfo->fromIndex + copnInfo->plmnNum;

        ret = TAF_MMA_QryCopnInfoReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, fromIndex, 0);

        if (ret != VOS_TRUE) {
            /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_ERROR);
        }
    }
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetNCellMonitorCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetNCellMonitorCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetNCellMonitorCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NCELL_MONITOR_SET) {
        AT_WARN_LOG("AT_RcvMtaSetNCellMonitorCnf : Current Option is not AT_CMD_NCELL_MONITOR_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryNCellMonitorCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_NcellMonitorQryCnf *mtaAtQryCnf = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;
    VOS_UINT16                 length;

    /* 初始化 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    mtaAtQryCnf = (MTA_AT_NcellMonitorQryCnf *)rcvMsg->content;
    result      = AT_OK;
    indexNum    = 0;
    length      = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryNCellMonitorCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryNCellMonitorCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NCELL_MONITOR_READ) {
        AT_WARN_LOG("AT_RcvMtaQryNCellMonitorCnf : Current Option is not AT_CMD_NCELL_MONITOR_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^NCELLMONITOR查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (mtaAtQryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        /* 查询时，还需要上报邻区状态 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaAtQryCnf->switchFlag, mtaAtQryCnf->ncellState);
    }

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNCellMonitorInd(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_NcellMonitorInd *mtaAtInd = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT16              length;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaAtInd = (MTA_AT_NcellMonitorInd *)rcvMsg->content;
    indexNum = 0;
    length   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNCellMonitorInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^NCELLMONITOR: %d%s", g_atCrLf, mtaAtInd->ncellState, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMmaSimInsertRsp(struct MsgCB *msg)
{
    TAF_MMA_SimInsertCnf *simInsertRsp = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    simInsertRsp = (TAF_MMA_SimInsertCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(simInsertRsp->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSimInsertRsp : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSimInsertRsp : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIMINSERT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMINSERT_SET) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (simInsertRsp->result == VOS_OK) {
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


VOS_UINT32 AT_RcvMtaRefclkfreqSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg       *mtaMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *rltCnf = VOS_NULL_PTR;
    VOS_UINT8         indexNum;
    VOS_UINT32        result;

    /* 初始化消息变量 */
    indexNum = 0;
    mtaMsg   = (AT_MTA_Msg *)msg;
    rltCnf   = (MTA_AT_ResultCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_REFCLKFREQ_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_REFCLKFREQ_SET) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqSetCnf: WARNING:Not AT_CMD_REFCLKFREQ_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (rltCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaRefclkfreqQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg              *mtaMsg        = VOS_NULL_PTR;
    MTA_AT_RefclkfreqQryCnf *refclkfreqCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    /* 初始化消息变量 */
    indexNum      = 0;
    mtaMsg        = (AT_MTA_Msg *)msg;
    refclkfreqCnf = (MTA_AT_RefclkfreqQryCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_REFCLKFREQ_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_REFCLKFREQ_READ) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqQryCnf: WARNING:Not AT_CMD_REFCLKFREQ_READ!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (refclkfreqCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* 输出GPS参考时钟信息，命令版本号默认为0 */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: 0,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                refclkfreqCnf->freq, refclkfreqCnf->precision, refclkfreqCnf->status);
        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaRefclkfreqInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    VOS_UINT8             indexNum      = 0;
    AT_MTA_Msg           *mtaMsg        = VOS_NULL_PTR;
    MTA_AT_RefclkfreqInd *refclkfreqInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    mtaMsg        = (AT_MTA_Msg *)msg;
    refclkfreqInd = (MTA_AT_RefclkfreqInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRefclkfreqInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 输出GPS参考时钟信息，命令版本号默认为0 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: 0,%d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_REFCLKFREQ].text,
        refclkfreqInd->freq, refclkfreqInd->precision, refclkfreqInd->status, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMtaNwSliceAuthCmdInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg               *mtaMsg = VOS_NULL_PTR;
    MTA_AT_NwSliceAuthCmdInd *cmdInd = VOS_NULL_PTR;
    VOS_UINT8                *eapMsg = VOS_NULL_PTR;
    VOS_UINT32                nssaiLen;
    VOS_UINT32                eapMsgLen;
    VOS_UINT32                rslt;
    VOS_CHAR                  nssai[AT_EVT_MULTI_S_NSSAI_LEN] = {0};
    VOS_UINT8                 indexNum = 0;

    /* 初始化消息变量 */
    mtaMsg = (AT_MTA_Msg *)msg;
    cmdInd = (MTA_AT_NwSliceAuthCmdInd *)mtaMsg->content;
    if (cmdInd->eapMsg.eapMsgSize > MTA_AT_EAP_MSG_MAX_NUM) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthCmdInd: WARNING: eapMsgSize is too large!");
        return VOS_ERR;
    }
    /* 取2是因为原始EAP消息符号数是上报AP的字符数的2倍；3为两个引号加一个\0的字符数 */
    eapMsgLen = cmdInd->eapMsg.eapMsgSize * sizeof(VOS_UINT8) * 2 + 3;
    eapMsg = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, eapMsgLen);
    if (eapMsg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthCmdInd: WARNING: Alloc Mem Fail!");
        return VOS_ERR;
    }
    (VOS_VOID)memset_s(eapMsg, sizeof(*eapMsg), 0x00, sizeof(*eapMsg));
    eapMsg[eapMsgLen - 1] = '\0';

    rslt = AT_HexToAsciiString(&eapMsg[1], (eapMsgLen - 3), cmdInd->eapMsg.eapMsg, cmdInd->eapMsg.eapMsgSize);
    if (rslt != AT_OK) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthCmdInd: WARNING: Hex to Ascii trans fail!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    eapMsg[0] = '"';
    eapMsg[eapMsgLen - AT_DOUBLE_QUO_MARKS_LENGTH] = '"';

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthCmdInd: WARNING: AT INDEX NOT FOUND!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    /* sNssai只有1个，因此sNssaiNum为1 */
    AT_ConvertMultiSNssaiToString(1, &cmdInd->sNssai, nssai, sizeof(nssai), &nssaiLen);
    if (nssaiLen == 0) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthCmdInd: WARNING: SNssai's string length is 0!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s:\"%s\",%s%s", g_atCrLf, g_atStringTab[AT_STRING_C5GNSSAA].text, nssai,
        eapMsg, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNwSliceAuthRsltInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg                *mtaMsg = VOS_NULL_PTR;
    MTA_AT_NwSliceAuthRsltInd *rsltInd = VOS_NULL_PTR;
    VOS_UINT8                 *eapMsg = VOS_NULL_PTR;
    VOS_UINT32                 nssaiLen;
    VOS_UINT32                 eapMsgLen;
    VOS_UINT32                 rslt;
    VOS_CHAR                   nssai[AT_EVT_MULTI_S_NSSAI_LEN] = {0};
    VOS_UINT8                  indexNum = 0;

    /* 初始化消息变量 */
    mtaMsg = (AT_MTA_Msg *)msg;
    rsltInd = (MTA_AT_NwSliceAuthRsltInd *)mtaMsg->content;
    if (rsltInd->eapMsg.eapMsgSize > MTA_AT_EAP_MSG_MAX_NUM) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthRsltInd: WARNING: eapMsgSize is too large!");
        return VOS_ERR;
    }
    eapMsgLen = rsltInd->eapMsg.eapMsgSize * sizeof(VOS_UINT8) * 2 + 3;
    eapMsg = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, eapMsgLen);
    if (eapMsg == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthRsltInd: WARNING: Alloc Mem Fail!");
        return VOS_ERR;
    }
    (VOS_VOID)memset_s(eapMsg, sizeof(*eapMsg), 0x00, sizeof(*eapMsg));
    eapMsg[eapMsgLen - 1] = '\0';

    rslt = AT_HexToAsciiString(&eapMsg[1], (eapMsgLen - 3), rsltInd->eapMsg.eapMsg, rsltInd->eapMsg.eapMsgSize);
    if (rslt != AT_OK) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthRsltInd: WARNING: Hex to Ascii trans fail!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    eapMsg[0] = '"';
    eapMsg[eapMsgLen - AT_DOUBLE_QUO_MARKS_LENGTH] = '"';

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthRsltInd: WARNING: AT INDEX NOT FOUND!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    /* sNssai只有1个，因此sNssaiNum为1 */
    AT_ConvertMultiSNssaiToString(1, &rsltInd->sNssai, nssai, sizeof(nssai), &nssaiLen);
    if (nssaiLen == 0) {
        AT_WARN_LOG("AT_RcvMtaNwSliceAuthRsltInd: WARNING: SNssai's string length is 0!");
        PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s:\"%s\",%s%s", g_atCrLf, g_atStringTab[AT_STRING_C5GNSSAA].text, nssai,
        eapMsg, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    PS_MEM_FREE(WUEPS_PID_AT, eapMsg);
    return VOS_OK;

}
#endif

VOS_UINT32 AT_RcvMtaHandleDectSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_HandledectSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_HandledectSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaHandleDectSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaHandleDectSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HANDLEDECT_SET) {
        AT_WARN_LOG("AT_RcvMtaHandleDectSetCnf : Current Option is not AT_CMD_HANDLEDECT_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaHandleDectQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_HandledectQryCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    qryCnf   = (MTA_AT_HandledectQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaHandleDectQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaHandleDectQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HANDLEDECT_QRY) {
        AT_WARN_LOG("AT_RcvMtaHandleDectQryCnf : Current Option is not AT_CMD_HANDLEDECT_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^HANDLEDECT?查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (qryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                (VOS_UINT16)qryCnf->handle);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaPsTransferInd(struct MsgCB *msg)
{
    VOS_UINT8             indexNum;
    AT_MTA_Msg           *mtaMsg        = VOS_NULL_PTR;
    MTA_AT_PsTransferInd *psTransferInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum      = 0;
    mtaMsg        = (AT_MTA_Msg *)msg;
    psTransferInd = (MTA_AT_PsTransferInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPsTransferInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^PSTRANSFER: %d%s", g_atCrLf, psTransferInd->cause, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMipiInfoInd(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    AT_MTA_Msg             *mtaMsg     = VOS_NULL_PTR;
    MTA_AT_RfLcdMipiclkInd *mipiClkCnf = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum   = 0;
    mtaMsg     = (AT_MTA_Msg *)msg;
    mipiClkCnf = (MTA_AT_RfLcdMipiclkInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMipiInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^MIPICLK: %d%s", g_atCrLf, mipiClkCnf->mipiClk, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMipiInfoCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    VOS_UINT32              result;
    AT_MTA_Msg             *mtaMsg     = VOS_NULL_PTR;
    MTA_AT_RfLcdMipiclkCnf *mipiClkCnf = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum   = 0;
    mtaMsg     = (AT_MTA_Msg *)msg;
    mipiClkCnf = (MTA_AT_RfLcdMipiclkCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMipiInfoCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMipiInfoCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPI_CLK_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    if (mipiClkCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                mipiClkCnf->mipiClk);
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_AT_HSUART == FEATURE_ON)

VOS_VOID AT_ProcDmsEscSeqInPppDataMode(AT_ClientIdUint16 clientId)
{
    DMS_PortIdUint16    portId;

    /*
     * 处理原则(enPortIndex对应的端口):
     * (1) ONLINE-DATA模式 - 上报OK
     * (2) 其他模式        - 丢弃
     */
    portId   = AT_GetDmsPortIdByClientId(clientId);

    /* 目前只支持PPP和IP模式下切换为ONLINE-COMMAND模式 */
    if (DMS_PORT_IsPppPacketTransMode(portId) == VOS_TRUE) {
        if (DMS_PORT_IsDsrModeConnectOn() == VOS_TRUE) {
            DMS_PORT_DeassertDsr(portId);
        }

        /*
         * HSUART端口由DATA模式切换到ONLINE_CMD模式:
         * 清除HSUART的缓存队列数据, 防止当前缓存队列满时, 主动上报的命令丢失
         */
        DMS_PORT_FlushTxData(portId);

        /* 切换到online-cmd模式，更新通道的模式，由于是临时切换到online-cmd模式，不做数据子模式更新 */
        DMS_PORT_SetMode(portId, DMS_PORT_MODE_ONLINE_CMD);

        DMS_PORT_SetDataState(portId, DMS_PORT_DATA_STATE_STOP);

        At_FormatResultData((VOS_UINT8)clientId, AT_OK);
    }

    return;
}
#endif

#if (FEATURE_DSDS == FEATURE_ON)

VOS_UINT32 AT_RcvMtaPsProtectSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_RrcProtectPsCnf *event    = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;
    VOS_UINT32              result;
    /* 初始化消息变量 */
    rcvMsg = (AT_MTA_Msg *)msg;
    event  = (MTA_AT_RrcProtectPsCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPsProtectSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaPsProtectSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PSPROTECTMODE_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PSPROTECTMODE_SET) {
        AT_WARN_LOG("AT_RcvMtaCgpsClockSetCnf: WARNING:Not AT_CMD_CGPSCLOCK_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaPhyInitCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg        *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_PhyInitCnf *event    = VOS_NULL_PTR;
    VOS_UINT8          indexNum = 0;
    VOS_UINT32         result;

    /* 初始化消息变量 */
    rcvMsg = (AT_MTA_Msg *)msg;
    event  = (MTA_AT_PhyInitCnf *)rcvMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPhyInitCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaPhyInitCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHYINIT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PHYINIT_SET) {
        AT_WARN_LOG("AT_RcvMtaPhyInitCnf: WARNING:Not AT_CMD_PHYINIT!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (event->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEcidSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    VOS_UINT8          indexNum;
    AT_MTA_Msg        *mtaMsg     = VOS_NULL_PTR;
    MTA_AT_EcidSetCnf *ecidSetCnf = VOS_NULL_PTR;
    VOS_UINT32         result;

    /* 初始化消息变量 */
    indexNum   = 0;
    mtaMsg     = (AT_MTA_Msg *)msg;
    ecidSetCnf = (MTA_AT_EcidSetCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEcidSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaEcidSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_REFCLKFREQ_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECID_SET) {
        AT_WARN_LOG("AT_RcvMtaEcidSetCnf: WARNING:Not AT_CMD_REFCLKFREQ_READ!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (ecidSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* 输出查询的增强型小区信息 */
        /* +ECID=<version>,<rat>,[<cell_description>] */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s=%s", g_parseContext[indexNum].cmdElement->cmdName,
                ecidSetCnf->cellInfoStr);
        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s=%s", g_parseContext[indexNum].cmdElement->cmdName, "0,NONE:,");

        result = AT_OK;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaSysCfgSetCnf(struct MsgCB *msg)
{
    TAF_MMA_SysCfgCnf *cnfMsg = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT32         result;

    cnfMsg = (TAF_MMA_SysCfgCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSysCfgSetCnf : WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSysCfgSetCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnfMsg->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cnfMsg->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPhoneModeSetCnf(struct MsgCB *msg)
{
    TAF_MMA_PhoneModeSetCnf *cnfMsg = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;
    ModemIdUint16            modemId;
#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT8 sptLteFlag;
    VOS_UINT8 sptUtralTDDFlag;
#endif

    cnfMsg   = (TAF_MMA_PhoneModeSetCnf *)msg;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    modemId  = MODEM_ID_0;
    result   = AT_FAILURE;
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG1("AT_RcvMmaPhoneModeSetCnf:Get ModemID From ClientID fail,ClientID:", indexNum);

        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSysCfgSetCnf : WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPhoneModeSetCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    if (cnfMsg->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cnfMsg->errorCause); /* 发生错误 */
    }

    /* V7R2 ^PSTANDBY命令复用关机处理流程 */
    if ((AT_LTE_CmdCurrentOpt)g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PSTANDBY_SET) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        return VOS_OK;
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
                return VOS_OK;
            }
        }
    }
#endif

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaDetachCnf(struct MsgCB *msg)
{
    TAF_MMA_DetachCnf *detachCnf = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT32         result;

    detachCnf = (TAF_MMA_DetachCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(detachCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDetachCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaDetachCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (detachCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        /* g_atClientTab[ucIndex].ulCause没有使用点，赋值点删除 */
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAttachCnf(struct MsgCB *msg)
{
    TAF_MMA_AttachCnf *attachCnf = VOS_NULL_PTR;
    VOS_UINT8          indexNum;

    attachCnf = (TAF_MMA_AttachCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(attachCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAttachCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 去除广播CLIENT类型的情况 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaAttachCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 停止保护定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 只输出结果(OK/ERROR)，不输出其他信息 */
    g_atSendDataBuff.bufLen = 0;

    if (attachCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_OK);
    } else {
        /* g_atClientTab[ucIndex].ulCause没有使用点，赋值点删除 */

        /* 调用At_FormatResultData发送命令结果 */
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAttachStatusQryCnf(struct MsgCB *msg)
{
    TAF_MMA_AttachStatusQryCnf *attachStatusQryCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;

    attachStatusQryCnf = (TAF_MMA_AttachStatusQryCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(attachStatusQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAttachStatusQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 去除广播CLIENT类型的情况 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaAttachStatusQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 检查当前操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MMA_GET_CURRENT_ATTACH_STATUS) {
        AT_WARN_LOG("AT_RcvMmaAttachStatusQryCnf : CmdCurrentOpt Not Match.");
        return VOS_ERR;
    }

    /* 停止保护定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 检查查询结果 */
    if (attachStatusQryCnf->rslt != TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_ERROR;
    }

    /* 根据Domain Type输出返回结果 */
    if (attachStatusQryCnf->domainType == TAF_MMA_SERVICE_DOMAIN_CS_PS) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                (VOS_UINT32)attachStatusQryCnf->psStatus, (VOS_UINT32)attachStatusQryCnf->csStatus);

        At_FormatResultData(indexNum, AT_OK);
    } else if (attachStatusQryCnf->domainType == TAF_MMA_SERVICE_DOMAIN_PS) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_UINT32)attachStatusQryCnf->psStatus);

        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_ERROR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaQryEmcCallBackCnf(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_MMA_EmcCallBackQryCnf *qryEmcCallBackCnf = VOS_NULL_PTR;
    VOS_UINT16                 length;
    VOS_UINT32                 result;

    /* 初始化消息变量 */
    indexNum          = 0;
    length            = 0;
    qryEmcCallBackCnf = (TAF_MMA_EmcCallBackQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryEmcCallBackCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaQryEmcCallBackCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaQryEmcCallBackCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMCCBM_QRY) {
        AT_WARN_LOG("AT_RcvMmaQuitCallBackCnf : Current Option is not AT_CMD_EMCCBM_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^QCCB查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (qryEmcCallBackCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            qryEmcCallBackCnf->isInCallBackMode);
        g_atSendDataBuff.bufLen = length;
    } else {
        result = AT_CME_UNKNOWN;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEmcCallBackNtf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_MMA_EmcCallBackNtf *emcCallBack = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum    = 0;
    emcCallBack = (TAF_MMA_EmcCallBackNtf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(emcCallBack->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEmcCallBackNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^EMCCBM: %d%s", g_atCrLf, emcCallBack->isInCallBack, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

#if (FEATURE_ECALL == FEATURE_ON)

VOS_UINT32 AT_RcvVcMsgSetMsdCnfProc(MN_AT_IndEvt *data)
{
    VOS_UINT8         indexNum;
    VOS_UINT32        ret;
    APP_VC_SetMsdCnf *rslt = VOS_NULL_PTR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVcMsgSetMsdCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVcMsgSetMsdCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLMSD_SET) {
        AT_WARN_LOG("AT_RcvVcMsgSetMsdCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return VOS_ERR;
    }

    rslt = (APP_VC_SetMsdCnf *)data->content;

    if (rslt->rslt == VOS_OK) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvVcMsgQryMsdCnfProc(MN_AT_IndEvt *data)
{
    APP_VC_MsgQryMsdCnf *qryMsd = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           ret;
    VOS_UINT32           i;
    VOS_UINT16           length;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVcMsgQryMsdCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVcMsgQryMsdCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLMSD_QRY) {
        AT_WARN_LOG("AT_RcvVcMsgQryMsdCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return VOS_ERR;
    }

    /* 初始化 */
    qryMsd = (APP_VC_MsgQryMsdCnf *)data->content;

    if (qryMsd->qryRslt == VOS_OK) {
        /* 输出查询结果 */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: \"", g_parseContext[indexNum].cmdElement->cmdName);

        for (i = 0; i < APP_VC_MSD_DATA_LEN; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02x", qryMsd->msdData[i]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");

        g_atSendDataBuff.bufLen = length;
        ret                     = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvVcMsgQryEcallCfgCnfProc(MN_AT_IndEvt *data)
{
    APP_VC_MsgQryEcallCfgCnf *qryCfg = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                ret;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVcMsgQryEcallCfgCnfProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVcMsgQryEcallCfgCnfProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLCFG_QRY) {
        AT_WARN_LOG("AT_RcvVcMsgQryEcallCfgCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");
        return VOS_ERR;
    }

    /* 初始化 */
    qryCfg = (APP_VC_MsgQryEcallCfgCnf *)data->content;

    if (qryCfg->qryRslt == VOS_OK) {
        /* 输出查询结果 */
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, qryCfg->mode,
            qryCfg->vocConfig);
        ret                     = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        ret                     = AT_ERROR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}
#endif

VOS_UINT32 AT_RcvMmaQuitCallBackCnf(struct MsgCB *msg)
{
    VOS_UINT8                   indexNum;
    TAF_MMA_QuitCallbackSetCnf *qccbCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;

    /* 初始化消息变量 */
    indexNum = 0;
    qccbCnf  = (TAF_MMA_QuitCallbackSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qccbCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaQuitCallBackCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaQuitCallBackCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_QCCB_SET) {
        AT_WARN_LOG("AT_RcvMmaQuitCallBackCnf : Current Option is not AT_CMD_QCCB_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^QCCB查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (qccbCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)


VOS_UINT32 AT_RcvMmaCLocInfoQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_CdmaLocinfoQryCnf *cLocInfoQueryCnf = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT16                 length;
    VOS_UINT8                  indexNum = 0;


    /* 初始化 */
    cLocInfoQueryCnf = (TAF_MMA_CdmaLocinfoQryCnf *)msg;
    length           = 0;
    result           = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cLocInfoQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCLocInfoQueryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCLocInfoQueryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CLOCINFO_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CLOCINFO_QRY) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        if (cLocInfoQueryCnf->rslt == VOS_OK) {


            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName,  cLocInfoQueryCnf->clocinfoPara.ratMode,
                cLocInfoQueryCnf->clocinfoPara.prevInUse, cLocInfoQueryCnf->clocinfoPara.mcc,
                cLocInfoQueryCnf->clocinfoPara.mnc, cLocInfoQueryCnf->clocinfoPara.sid,
                cLocInfoQueryCnf->clocinfoPara.nid, cLocInfoQueryCnf->clocinfoPara.baseId,
                cLocInfoQueryCnf->clocinfoPara.baseLatitude, cLocInfoQueryCnf->clocinfoPara.baseLongitude);

            g_atSendDataBuff.bufLen = length;
        } else {
            /* 查询失败，上报^CLOCINFO: 0 */
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                VOS_FALSE);

            g_atSendDataBuff.bufLen = length;
        }
    } else {
        return VOS_ERR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCSidInd(struct MsgCB *msg)
{
    VOS_UINT8        indexNum;
    TAF_MMA_CsidInd *cSidInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    cSidInd  = (TAF_MMA_CsidInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cSidInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCSidInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CSID: %d%s", g_atCrLf, cSidInd->sid, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMmaSetCSidListCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_MMA_CsidlistSetCnf *setCSidListCnf = VOS_NULL_PTR;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    indexNum       = 0;
    setCSidListCnf = (TAF_MMA_CsidlistSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(setCSidListCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSetCSidListCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSetCSidListCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSIDLIST_SET) {
        AT_WARN_LOG("AT_RcvMmaSetCSidListCnf : Current Option is not AT_CMD_CSIDLIST_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CSIDLIST设置命令 */
    g_atSendDataBuff.bufLen = 0;

    if (setCSidListCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMmaQryCurrSidNidCnf(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum;
    TAF_MMA_CurrSidNidQryCnf *qryCurrSidNidCnf = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT32                result;

    /* 初始化消息变量 */
    indexNum         = 0;
    length           = 0;
    qryCurrSidNidCnf = (TAF_MMA_CurrSidNidQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryCurrSidNidCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaQryCurrSidNidCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaQryCurrSidNidCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSNID_QRY) {
        AT_WARN_LOG("AT_RcvMmaQryCurrSidNidCnf : Current Option is not AT_CMD_EMCCBM_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CSNID查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (qryCurrSidNidCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            qryCurrSidNidCnf->sid, qryCurrSidNidCnf->nid);

        g_atSendDataBuff.bufLen = length;
    } else {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRoamingModeSwitchInd(struct MsgCB *msg)
{
    TAF_MMA_RoamingModeSwitchInd *roamingModeSwitchInd = VOS_NULL_PTR;
    VOS_UINT8                     indexNum;

    /* 初始化消息变量 */
    indexNum             = 0;
    roamingModeSwitchInd = (TAF_MMA_RoamingModeSwitchInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(roamingModeSwitchInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRoamingModeSwitchInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ROAMINGMODESWITCH%s", g_atCrLf, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

VOS_UINT32 AT_RcvMmaCombinedModeSwitchInd(struct MsgCB *msg)
{
    TAF_MMA_CombinedModeSwitchInd *combinedModeSwitchInd = VOS_NULL_PTR;
    VOS_UINT8                      indexNum;

    /* 初始化消息变量 */
    indexNum              = 0;
    combinedModeSwitchInd = (TAF_MMA_CombinedModeSwitchInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(combinedModeSwitchInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRoamingModeSwitchInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^RATCOMBINEDMODE: %d%s", g_atCrLf, combinedModeSwitchInd->combinedMode,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRatCombinedModeQryCnf(struct MsgCB *msg)
{
    VOS_UINT8                      indexNum;
    TAF_MMA_RatCombinedModeQryCnf *rcvMsg = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_MMA_RatCombinedModeQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRatCombinedModeQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRatCombinedModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RATCOMBINEDMODE_QRY) {
        AT_WARN_LOG("AT_RcvMmaRatCombinedModeQryCnf : Current Option is not AT_CMD_PRLID_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^RATCOMBINEDMODE: %d%s", g_atCrLf, rcvMsg->combinedMode, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaIccAppTypeSwitchInd(struct MsgCB *msg)
{
    TAF_MMA_IccAppTypeSwitchInd *iccAppTypeSwitchInd = VOS_NULL_PTR;
    VOS_UINT8                    indexNum            = 0;

    /* 初始化消息变量 */
    indexNum            = 0;
    iccAppTypeSwitchInd = (TAF_MMA_IccAppTypeSwitchInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(iccAppTypeSwitchInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaIccAppTypeSwitchInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ICCAPPTYPESWITCH: %d%s", g_atCrLf, iccAppTypeSwitchInd->switchState,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCtRoamInfoCnf(struct MsgCB *msg)
{
    VOS_UINT8                        indexNum;
    TAF_MMA_CtccRoamingNwInfoQryCnf *qryMultiModeSidMcc = VOS_NULL_PTR;
    VOS_UINT16                       length;
    VOS_UINT32                       result;

    /* 初始化消息变量 */
    indexNum           = 0;
    length             = 0;
    qryMultiModeSidMcc = (TAF_MMA_CtccRoamingNwInfoQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryMultiModeSidMcc->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCtRoamInfoCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCtRoamInfoCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CTROAMINFO_QRY) {
        AT_WARN_LOG("AT_RcvMmaCtRoamInfoCnf : Current Option is not AT_CMD_MULTIMODESIDMCC_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CSNID查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    result = AT_OK;
    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%x%x%x,%x%x%x,%d", g_parseContext[indexNum].cmdElement->cmdName,
        qryMultiModeSidMcc->sid, (qryMultiModeSidMcc->ul3Gpp2Mcc & 0x0f00) >> 8,
        (qryMultiModeSidMcc->ul3Gpp2Mcc & 0xf0) >> 4, (qryMultiModeSidMcc->ul3Gpp2Mcc & 0x0f),
        (qryMultiModeSidMcc->ul3GppMcc & 0x0f00) >> 8, (qryMultiModeSidMcc->ul3GppMcc & 0xf0) >> 4,
        (qryMultiModeSidMcc->ul3GppMcc & 0x0f), qryMultiModeSidMcc->modeType);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCtRoamingInfoChgInd(struct MsgCB *msg)
{
    VOS_UINT8                           indexNum;
    TAF_MMA_CtccRoamingNwInfoReportInd *multiModeSidMccChgInd = VOS_NULL_PTR;
    VOS_UINT16                          length;

    /* 初始化消息变量 */
    indexNum              = 0;
    length                = 0;
    multiModeSidMccChgInd = (TAF_MMA_CtccRoamingNwInfoReportInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(multiModeSidMccChgInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCtRoamingInfoChgInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 格式化AT^CSNID查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CTROAMINFO: %d,%x%x%x,%x%x%x,%d%s", g_atCrLf, multiModeSidMccChgInd->sid,
        (multiModeSidMccChgInd->ul3Gpp2Mcc & 0x0f00) >> 8, (multiModeSidMccChgInd->ul3Gpp2Mcc & 0xf0) >> 4,
        (multiModeSidMccChgInd->ul3Gpp2Mcc & 0x0f), (multiModeSidMccChgInd->ul3GppMcc & 0x0f00) >> 8,
        (multiModeSidMccChgInd->ul3GppMcc & 0xf0) >> 4, (multiModeSidMccChgInd->ul3GppMcc & 0x0f),
        multiModeSidMccChgInd->modeType, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCtOosCountCnf(struct MsgCB *msg)
{
    TAF_MMA_CtccOosCountSetCnf *oosCountCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;
    VOS_UINT32                  result;

    /* 初始化消息变量 */
    indexNum    = 0;
    oosCountCnf = (TAF_MMA_CtccOosCountSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(oosCountCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCtOosCountCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCtOosCountCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CTOOSCOUNT_SET) {
        AT_WARN_LOG("AT_RcvMmaCtOosCountCnf : Current Option is not AT_CMD_CTOOSCOUNT_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^QCCB查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (oosCountCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON)


VOS_UINT32 AT_RcvTafCcmEncryptCallCnf(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_CCM_EncryptVoiceCnf   *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32                 result;
    AT_EncryptVoiceErrorUint32 ecnErr;
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
    TAF_CHAR  acNewDocName[AT_TEST_ECC_FILE_NAME_MAX_LEN];
    VOS_INT32 length;

    (VOS_VOID)memset_s(acNewDocName, (VOS_SIZE_T)sizeof(acNewDocName), 0x00, (VOS_SIZE_T)sizeof(acNewDocName));
    length = 0;
#endif

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_EncryptVoiceCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCALL_SET) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapQryCnf : Current Option is not AT_CMD_ECCALL_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    /* 格式化AT^ECCALL  SET命令返回 */
    if (rcvMsg->eccVoiceStatus == TAF_CALL_ENCRYPT_VOICE_SUCC) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
        ecnErr = AT_MapEncVoiceErr(rcvMsg->eccVoiceStatus);

        /* 密话主叫结果其他原因值上报 */
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCALL: %d%s", g_atCrLf, ecnErr, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
        length = snprintf_s(acNewDocName, (VOS_SIZE_T)sizeof(acNewDocName), (VOS_SIZE_T)(sizeof(acNewDocName) - 1),
                            MODEM_LOG_ROOT "/ECC_TEST/Encrypted_call_data%d", g_atCurrEncVoiceTestFileNum);
        if (length <= 0) {
            AT_WARN_LOG("AT_RcvTafCcmEccCapQryCnf(): snprintf_s len <= 0");

            return VOS_ERR;
        }

        length = snprintf_s((VOS_CHAR *)(acNewDocName + length), ((VOS_SIZE_T)sizeof(acNewDocName) - length),
                            ((VOS_SIZE_T)sizeof(acNewDocName) - length - 1), "_[%d]records.txt",
                            g_atCurrEncVoiceDataCount);

        if (length <= 0) {
            AT_WARN_LOG("AT_RcvTafCcmEccCapQryCnf(): snprintf_s len <= 0 (2)");

            return VOS_ERR;
        }

        (VOS_VOID)at_file_rename(g_currEncVoiceDataWriteFilePath[g_currEncVoiceDataWriteFileNum], acNewDocName);
#endif
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmEccCapQryCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_CCM_EccSrvCapQryCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32               result;
    indexNum = 0;
    rcvMsg   = (TAF_CCM_EccSrvCapQryCnf *)msg;
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXcallEccCapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvXcallEccCapQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCAP_QRY) {
        AT_WARN_LOG("AT_RcvXcallEccCapQryCnf : Current Option is not AT_CMD_ECCAP_QRY.");
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (rcvMsg->eccSrvCapQryCnfPara.eccSrvCap == TAF_CALL_ECC_SRV_CAP_DISABLE) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCAP: %d,%d%s", g_atCrLf, rcvMsg->eccSrvCapQryCnfPara.eccSrvCap,
            TAF_CALL_ECC_SRV_STATUS_CLOSE, g_atCrLf);
    } else if (rcvMsg->eccSrvCapQryCnfPara.eccSrvCap == TAF_CALL_ECC_SRV_CAP_ENABLE) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCAP: %d,%d%s", g_atCrLf, rcvMsg->eccSrvCapQryCnfPara.eccSrvCap,
            rcvMsg->eccSrvCapQryCnfPara.eccSrvStatus, g_atCrLf);
    } else {
        result = AT_ERROR;
    }
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmEncryptCallInd(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_CCM_EncryptVoiceInd   *rcvMsg = VOS_NULL_PTR;
    VOS_UINT8                  asciiNum[MN_CALL_MAX_CALLED_BCD_NUM_LEN + 1];
    AT_EncryptVoiceErrorUint32 ecnErr;
    errno_t                    memResult;
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
    TAF_CHAR  acNewDocName[AT_TEST_ECC_FILE_NAME_MAX_LEN];
    VOS_INT32 length;

    (VOS_VOID)memset_s(acNewDocName, (VOS_SIZE_T)sizeof(acNewDocName), 0x00, (VOS_SIZE_T)sizeof(acNewDocName));
    length = 0;
#endif

    /* 初始化消息变量 */
    indexNum  = 0;
    rcvMsg    = (TAF_CCM_EncryptVoiceInd *)msg;
    memResult = memset_s(asciiNum, sizeof(asciiNum), 0x00, MN_CALL_MAX_CALLED_BCD_NUM_LEN + 1);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(asciiNum), MN_CALL_MAX_CALLED_BCD_NUM_LEN + 1);

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmEncryptCallInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    ecnErr = AT_MapEncVoiceErr(rcvMsg->encryptVoiceIndPara.eccVoiceStatus);

    /* AT^ECCALL ind命令返回 */
    if (ecnErr == AT_ENCRYPT_VOICE_MT_CALL_NOTIFICATION) {
        /* 密话被叫主动上报 */
        AT_BcdNumberToAscii(rcvMsg->encryptVoiceIndPara.callingNumber.bcdNum,
                            rcvMsg->encryptVoiceIndPara.callingNumber.numLen, (VOS_CHAR *)asciiNum);
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCALL: %d,\"%s\"%s", g_atCrLf, ecnErr, asciiNum, g_atCrLf);
    } else if (ecnErr == AT_ENCRYPT_VOICE_ERROR_ENUM_BUTT) {
        /* 异常场景 */
        return VOS_ERR;
    } else {
        /* 密话主叫结果其他原因值上报 */
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCALL: %d%s", g_atCrLf, ecnErr, g_atCrLf);

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
        length = snprintf_s(acNewDocName, (VOS_SIZE_T)sizeof(acNewDocName), (VOS_SIZE_T)(sizeof(acNewDocName) - 1),
                            MODEM_LOG_ROOT "/ECC_TEST/Encrypted_call_data%d", g_atCurrEncVoiceTestFileNum);
        if (length <= 0) {
            AT_WARN_LOG("AT_RcvTafCcmEncryptCallInd(): snprintf_s len <= 0");

            return VOS_ERR;
        }

        length = snprintf_s((VOS_CHAR *)(acNewDocName + length), ((VOS_SIZE_T)sizeof(acNewDocName) - length),
                            ((VOS_SIZE_T)sizeof(acNewDocName) - length - 1), "_[%d]records.txt",
                            g_atCurrEncVoiceDataCount);

        if (length <= 0) {
            AT_WARN_LOG("AT_RcvTafCcmEncryptCallInd(): snprintf_s len <= 0 (2)");

            return VOS_ERR;
        }

        if (ecnErr != AT_ENCRYPT_VOICE_SUCC) {
            (VOS_VOID)at_file_rename(g_currEncVoiceDataWriteFilePath[g_currEncVoiceDataWriteFileNum], acNewDocName);

            memResult = memcpy_s(g_atCurrDocName, (VOS_SIZE_T)sizeof(g_atCurrDocName), acNewDocName,
                                 (VOS_SIZE_T)sizeof(acNewDocName));
            TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(g_atCurrDocName), (VOS_SIZE_T)sizeof(acNewDocName));
        }
#endif
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmEccRemoteCtrlInd(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_CCM_EcRemoteCtrlInd *rcvMsg = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_EcRemoteCtrlInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvXcallEcRemoteCtrlInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCTRL: %d%s", g_atCrLf, rcvMsg->remoteCtrlType, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmRemoteCtrlAnsCnf(struct MsgCB *msg)
{
    VOS_UINT8                    indexNum;
    TAF_CCM_RemoteCtrlAnswerCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32                   result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_RemoteCtrlAnswerCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmRemoteCtrlAnsCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmRemoteCtrlAnsCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCTRL_SET) {
        AT_WARN_LOG("AT_RcvTafCcmRemoteCtrlAnsCnf : Current Option is not AT_CMD_ECCTRL_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    /* 格式化AT^ECCTRL  SET命令返回 */
    if (rcvMsg->result == TAF_CALL_SEND_RESULT_SUCC) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmEccCapSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_CCM_EccSrvCapCfgCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32               result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_EccSrvCapCfgCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCAP_SET) {
        AT_WARN_LOG("AT_RcvTafCcmEccCapSetCnf : Current Option is not AT_CMD_ECCAP_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECCAP  SET命令返回 */
    if (rcvMsg->result == TAF_CALL_ECC_SRV_CAP_CFG_RESULT_SUCC) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)

VOS_UINT32 AT_RcvTafCcmSetEccTestModeCnf(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum;
    TAF_CCM_SetEcTestModeCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32                result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_SetEcTestModeCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccTestModeCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccTestModeCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCTEST_SET) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccTestModeCnf : Current Option is not AT_CMD_ECCTEST_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECCTEST  SET命令返回 */
    if (rcvMsg->result == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmQryEccTestModeCnf(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum;
    TAF_CCM_GetEcTestModeCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32                result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_GetEcTestModeCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccTestModeCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccTestModeCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCTEST_QRY) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccTestModeCnf : Current Option is not AT_CMD_ECCTEST_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECCTEST  SET命令返回 */
    if (rcvMsg->testModeCnfPara.result == VOS_TRUE) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECCTEST: %d%s", g_atCrLf, rcvMsg->testModeCnfPara.eccTestModeStatus,
            g_atCrLf);
    } else {
        result = AT_ERROR;
    }
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmQryEccRandomCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_CCM_GetEcRandomCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT32              loop;
    VOS_UINT16              length;
    VOS_UINT8               randomIndex;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_GetEcRandomCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccRandomCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccRandomCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECRANDOM_QRY) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccRandomCnf : Current Option is not AT_CMD_ECRANDOM_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECRANDOM  QRY命令返回 */
    if (rcvMsg->ecRandomData.result == VOS_TRUE) {
        result      = AT_OK;
        randomIndex = 0;

        for (loop = 1; loop < TAF_CALL_APP_EC_RANDOM_MAX_GROUP + 1; loop++) {
            length = 0;
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s^ECRANDOM: %d,%s", g_atCrLf, loop,
                rcvMsg->ecRandomData.eccRandom[randomIndex].eccData);
            randomIndex++;

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", rcvMsg->ecRandomData.eccRandom[randomIndex].eccData);
            randomIndex++;

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", rcvMsg->ecRandomData.eccRandom[randomIndex].eccData);
            randomIndex++;

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s", rcvMsg->ecRandomData.eccRandom[randomIndex].eccData,
                g_atCrLf);
            randomIndex++;
            g_atSendDataBuff.bufLen = length;
            At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
        }

    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmQryEccKmcCnf(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    TAF_CCM_GetEcKmcCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            kmcAsciiData[TAF_CALL_ECC_KMC_PUBLIC_KEY_MAX_ASCII_LENGTH + 1];

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_GetEcKmcCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccKmcCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccKmcCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECKMC_QRY) {
        AT_WARN_LOG("AT_RcvTafCcmQryEccKmcCnf : Current Option is not AT_CMD_ECKMC_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECKMC  QRY命令返回 */
    if (rcvMsg->kmcCnfPara.result == VOS_TRUE) {
        result = AT_OK;

        /* 把kmc data转换为ascii码 */
        if (AT_HexToAsciiString(kmcAsciiData, sizeof(kmcAsciiData), rcvMsg->kmcCnfPara.kmcData.eccKmcData,
                                rcvMsg->kmcCnfPara.kmcData.eccKmcLength) != AT_OK) {
            return VOS_ERR;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^ECKMC: %d,\"%s\"%s", g_atCrLf, rcvMsg->kmcCnfPara.kmcData.eccKmcVer,
            kmcAsciiData, g_atCrLf);
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmSetEccKmcCnf(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    TAF_CCM_SetEcKmcCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_SetEcKmcCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccKmcCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccKmcCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECKMC_SET) {
        AT_WARN_LOG("AT_RcvTafCcmSetEccKmcCnf : Current Option is not AT_CMD_ECKMC_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^ECKMC  SET命令返回 */
    if (rcvMsg->result == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmEncryptedVoiceDataInd(struct MsgCB *msg)
{
    VOS_INT32 ret;
    int       fd;
    TAF_CHAR *dirPath = MODEM_LOG_ROOT "/ECC_TEST";
    TAF_CCM_EncryptedVoiceDataInd *rcvMsg = VOS_NULL_PTR;
    TAF_CHAR  acNewDocName[AT_TEST_ECC_FILE_NAME_MAX_LEN];
    TAF_CHAR  acTimeDocName[AT_TEST_ECC_FILE_NAME_MAX_LEN];
    VOS_INT32 length;

    rcvMsg = (TAF_CCM_EncryptedVoiceDataInd *)msg;
    (VOS_VOID)memset_s(acNewDocName, (VOS_SIZE_T)sizeof(acNewDocName), 0x00, (VOS_SIZE_T)sizeof(acNewDocName));
    length = 0;

    /* 如果目录不存在则先创建目录 */
    if (at_file_access(dirPath, 0) != VOS_OK) {
        if (at_file_mkdir(dirPath) != VOS_OK) {
            AT_WARN_LOG("AT_RcvTafCcmEncryptedVoiceDataInd: mkdir /data/hisi_logs/modem_log/ECC_TEST failed.\n");
            return VOS_ERR;
        }
    }

    if (rcvMsg->encVoiceDataIndPara.isTimeInfo == VOS_TRUE) {
        (VOS_VOID)memset_s(acTimeDocName, (VOS_SIZE_T)sizeof(acTimeDocName), 0x00, (VOS_SIZE_T)sizeof(acTimeDocName));

        length = snprintf_s(acTimeDocName, (VOS_SIZE_T)sizeof(acTimeDocName), (VOS_SIZE_T)(sizeof(acTimeDocName) - 1),
                            MODEM_LOG_ROOT "/ECC_TEST/Encrypted_call_execute_time%d.txt", g_atCurrEncVoiceTestFileNum);
        if (length <= 0) {
            AT_WARN_LOG("AT_RcvTafCcmEncryptedVoiceDataInd(): snprintf_s len <= 0");

            return VOS_ERR;
        }

        fd = at_file_open(acTimeDocName, "a+");
    } else {
        if (PS_MEM_CMP(acNewDocName, g_atCurrDocName, sizeof(acNewDocName)) != 0) {
            fd = at_file_open(g_atCurrDocName, "a+");
        } else {
            fd = at_file_open(g_currEncVoiceDataWriteFilePath[g_currEncVoiceDataWriteFileNum], "a+");
        }
    }

    if (fd < 0) {
        AT_WARN_LOG("AT_RcvTafCcmEncryptedVoiceDataInd: open file failed.\n");
        return VOS_ERR;
    }

    ret = at_file_write(fd, rcvMsg->encVoiceDataIndPara.data, rcvMsg->encVoiceDataIndPara.length);
    if (ret != rcvMsg->encVoiceDataIndPara.length) {
        AT_WARN_LOG("AT_RcvTafCcmEncryptedVoiceDataInd: write file failed.\n");
        at_file_close(fd);
        return VOS_ERR;
    }

    if (rcvMsg->encVoiceDataIndPara.isTimeInfo == VOS_FALSE) {
        g_atCurrEncVoiceDataCount++;
    }

    at_file_close(fd);


    return VOS_OK;
}

#endif
#endif

VOS_UINT32 AT_RcvMmaClocinfoInd(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    TAF_MMA_ClocinfoInd *clocinfoInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum    = 0;
    clocinfoInd = (TAF_MMA_ClocinfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(clocinfoInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaClocinfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CLOCINFO: %d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
        clocinfoInd->clocinfoPara.ratMode, clocinfoInd->clocinfoPara.prevInUse, clocinfoInd->clocinfoPara.mcc,
        clocinfoInd->clocinfoPara.mnc, clocinfoInd->clocinfoPara.sid, clocinfoInd->clocinfoPara.nid,
        clocinfoInd->clocinfoPara.baseId, clocinfoInd->clocinfoPara.baseLatitude,
        clocinfoInd->clocinfoPara.baseLongitude, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmPrivacyModeSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_CCM_PrivacyModeSetCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32                 result;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_PrivacyModeSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPMP_SET) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeSetCnf : Current Option is not AT_CMD_CPMP_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化AT^CPMP  SET命令返回 */
    if (rcvMsg->result == TAF_CALL_RESULT_TYPE_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmPrivacyModeQryCnf(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_CCM_PrivacyModeQryCnf *rcvMsg = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_PrivacyModeQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPMP_QRY) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeQryCnf : Current Option is not AT_CMD_ECCAP_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* X模下，只有一路激活的呼叫 */
    if (rcvMsg->privacyModeQryCnfPara.callNums != 0) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s^CPMP: %d,%d,%d%s", g_atCrLf,
                rcvMsg->privacyModeQryCnfPara.privacyMode,
                rcvMsg->privacyModeQryCnfPara.callVoicePrivacyInfo[0].callId,
                rcvMsg->privacyModeQryCnfPara.callVoicePrivacyInfo[0].privacyMode, g_atCrLf);
    } else {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^CPMP: %d%s", g_atCrLf, rcvMsg->privacyModeQryCnfPara.privacyMode,
            g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmPrivacyModeInd(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_CCM_PrivacyModeInd *rcvMsg = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_CCM_PrivacyModeInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmPrivacyModeInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^CPMP: %d,%d,%d%s", g_atCrLf, rcvMsg->privacyModeIndPara.privacyMode,
            rcvMsg->privacyModeIndPara.callVoicePrivacyInfo.callId,
            rcvMsg->privacyModeIndPara.callVoicePrivacyInfo.privacyMode, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCtRoamRptCfgSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                              indexNum;
    TAF_MMA_CtccRoamingNwInfoRtpCfgSetCnf *ctRoamRptSetCnf = VOS_NULL_PTR;
    VOS_UINT32                             result;

    /* 初始化消息变量 */
    indexNum        = 0;
    ctRoamRptSetCnf = (TAF_MMA_CtccRoamingNwInfoRtpCfgSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(ctRoamRptSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCtRoamRptCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCtRoamRptCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CTROAMINFO_SET) {
        AT_WARN_LOG("AT_RcvMmaCtRoamRptCfgSetCnf : Current Option is not AT_CMD_CTROAMINFO_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CTROAM设置命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (ctRoamRptSetCnf->errorCode == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPrlIdQryCnf(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    TAF_MMA_PrlidQryCnf *rcvMsg = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rcvMsg   = (TAF_MMA_PrlidQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPrlIdQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPrlIdQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PRLID_QRY) {
        AT_WARN_LOG("AT_RcvMmaPrlIdQryCnf : Current Option is not AT_CMD_PRLID_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^PRLID: %d,%d,%d,%d,%d%s", g_atCrLf, rcvMsg->prlSrcType, rcvMsg->prlId,
        rcvMsg->mlplMsplSrcType, rcvMsg->mlplId, rcvMsg->msplId, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCTimeInd(struct MsgCB *msg)
{
    VOS_UINT8         indexNum;
    TAF_MMA_CtimeInd *cTimeInd = VOS_NULL_PTR;
    VOS_INT8          timeZone;

    /* 初始化消息变量 */
    indexNum = 0;
    cTimeInd = (TAF_MMA_CtimeInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cTimeInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCTimeInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 底层上报的cTimeZone是以30分钟为单位的，AP要求以15分钟为单位，乘2是为了将15分钟为单位转化为30分钟为单位 */
    if (cTimeInd->timezoneCTime.timeZone < 0) {
        timeZone = (VOS_INT8)(-1 * 2 * cTimeInd->timezoneCTime.timeZone); /* -1是为了将负的cTimeZone转换为正值 */
    } else {
        timeZone = (VOS_INT8)(2 * cTimeInd->timezoneCTime.timeZone);
    }

    g_atSendDataBuff.bufLen = 0;
    /* 时间显示格式: ^CTIME: "yy/mm/dd,hh:mm:ss(+/-)tz,dst" */
    if (cTimeInd->timezoneCTime.timeZone >= 0) {
         /* cTimeInd->timezoneCTime.year % 100是为了将年份四位表示的形式转换为两位的形式表示 */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s^CTIME: \"%02d/%02d/%02d,%02d:%02d:%02d+%d,%02d\"%s", g_atCrLf,
                cTimeInd->timezoneCTime.year % 100, cTimeInd->timezoneCTime.month, cTimeInd->timezoneCTime.day,
                cTimeInd->timezoneCTime.hour, cTimeInd->timezoneCTime.minute, cTimeInd->timezoneCTime.second, timeZone,
                cTimeInd->timezoneCTime.dayltSavings, g_atCrLf);
    } else {
        /* cTimeInd->timezoneCTime.year % 100是为了将年份四位表示的形式转换为两位的形式表示 */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s^CTIME: \"%02d/%02d/%02d,%02d:%02d:%02d-%d,%02d\"%s", g_atCrLf,
                cTimeInd->timezoneCTime.year % 100, cTimeInd->timezoneCTime.month, cTimeInd->timezoneCTime.day,
                cTimeInd->timezoneCTime.hour, cTimeInd->timezoneCTime.minute, cTimeInd->timezoneCTime.second, timeZone,
                cTimeInd->timezoneCTime.dayltSavings, g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCFreqLockSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_MMA_CfreqLockSetCnf *cFreqLockSetCnf = VOS_NULL_PTR;
    VOS_UINT32               result;

    /* 初始化消息变量 */
    indexNum        = 0;
    cFreqLockSetCnf = (TAF_MMA_CfreqLockSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cFreqLockSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFREQLOCK_SET) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockSetCnf : Current Option is not AT_CMD_CFREQLOCK_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CFREQLOCK命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (cFreqLockSetCnf->rslt != VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCFreqLockQueryCnf(struct MsgCB *msg)
{
    VOS_UINT8                  indexNum;
    TAF_MMA_CfreqLockQueryCnf *cFreqLockQryCnf = VOS_NULL_PTR;
    VOS_UINT16                 length;

    /* 初始化消息变量 */
    indexNum        = 0;
    length          = 0;
    cFreqLockQryCnf = (TAF_MMA_CfreqLockQueryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cFreqLockQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockQueryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFREQLOCK_QRY) {
        AT_WARN_LOG("AT_RcvMmaCFreqLockQueryCnf : Current Option is not AT_CMD_CFREQLOCK_QUERY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CFREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (cFreqLockQryCnf->cFreqLockPara.freqLockMode == TAF_MMA_CFREQ_LOCK_MODE_OFF) {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            cFreqLockQryCnf->cFreqLockPara.freqLockMode);
    } else {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d,%d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, cFreqLockQryCnf->cFreqLockPara.freqLockMode,
            cFreqLockQryCnf->cFreqLockPara.sid, cFreqLockQryCnf->cFreqLockPara.nid,
            cFreqLockQryCnf->cFreqLockPara.cdmaBandClass, cFreqLockQryCnf->cFreqLockPara.cdmaFreq,
            cFreqLockQryCnf->cFreqLockPara.cdmaPn, cFreqLockQryCnf->cFreqLockPara.evdoBandClass,
            cFreqLockQryCnf->cFreqLockPara.evdoFreq, cFreqLockQryCnf->cFreqLockPara.evdoPn);
    }

    g_atSendDataBuff.bufLen = length;
    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCdmaCsqSetCnf(struct MsgCB *msg)
{
    TAF_MMA_CdmacsqSetCnf *cdmaCsqCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum;
    VOS_UINT32             result;

    cdmaCsqCnf = (TAF_MMA_CdmacsqSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cdmaCsqCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cdmaCsqCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCdmaCsqQryCnf(struct MsgCB *msg)
{
    TAF_MMA_CdmacsqQueryCnf *cdmaCsqQueryCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT16               length;
    VOS_UINT8                indexNum = 0;

    /* 初始化 */
    cdmaCsqQueryCnf = (TAF_MMA_CdmacsqQueryCnf *)msg;
    length          = 0;
    result          = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cdmaCsqQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CDMACSQ_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CDMACSQ_QRY) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        cdmaCsqQueryCnf->mode, cdmaCsqQueryCnf->timeInterval, cdmaCsqQueryCnf->rssiRptThreshold,
        cdmaCsqQueryCnf->ecIoRptThreshold, cdmaCsqQueryCnf->sigQualityInfo.cdmaRssi,
        cdmaCsqQueryCnf->sigQualityInfo.cdmaEcIo);


    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCdmaCsqInd(struct MsgCB *msg)
{
    VOS_UINT8           indexNum;
    TAF_MMA_CdmacsqInd *cdmaCsqInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum   = 0;
    cdmaCsqInd = (TAF_MMA_CdmacsqInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cdmaCsqInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CDMACSQ: %d,%d%s", g_atCrLf, cdmaCsqInd->cdmaRssi, cdmaCsqInd->cdmaEcIo,
        g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCLModInd(struct MsgCB *msg)
{
    VOS_UINT8          indexNum;
    TAF_MMA_ClmodeInd *cLModeInd = VOS_NULL_PTR;
    ModemIdUint16      modemId;
    VOS_UINT32         rslt;
    VOS_UINT8         *isCLMode = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum  = 0;
    cLModeInd = (TAF_MMA_ClmodeInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cLModeInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCLModInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    modemId = MODEM_ID_0;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaCLModInd: Get modem id fail.");
        return VOS_ERR;
    }

    isCLMode = AT_GetModemCLModeCtxAddrFromModemId(modemId);

    *isCLMode = cLModeInd->isCLMode;
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaClDbDomainStatusSetCnf(struct MsgCB *msg)
{
    TAF_MMA_ClDbdomainStatusSetCnf *clDbdomainStatusCnf = VOS_NULL_PTR;
    VOS_UINT8                       indexNum;
    VOS_UINT32                      result;

    clDbdomainStatusCnf = (TAF_MMA_ClDbdomainStatusSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(clDbdomainStatusCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaClDbDomainStatusSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaClDbDomainStatusSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLDBDOMAINSTATUS_SET) {
        AT_WARN_LOG("AT_RcvMmaClDbDomainStatusSetCnf : Current Option is not AT_CMD_CFPLMN_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (clDbdomainStatusCnf->result == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaClDbdomainStatusReportInd(struct MsgCB *msg)
{
    TAF_MMA_ClDbdomainStatusInfoInd *clDbdomainStaRInd = VOS_NULL_PTR;
    VOS_UINT8                        indexNum;

    /* 初始化消息变量 */
    indexNum          = 0;
    clDbdomainStaRInd = (TAF_MMA_ClDbdomainStatusInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(clDbdomainStaRInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRoamingModeSwitchInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^CLDBDOMAINSTATUS: %d,%d,%d%s", g_atCrLf,
            clDbdomainStaRInd->enableReportFlag, clDbdomainStaRInd->uc1xSrvStatus, clDbdomainStaRInd->imsVoiceCap,
            g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaXpassInfoInd(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    AT_MTA_Msg          *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_XpassInfoInd *xpassInfoInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum     = 0;
    mtaMsg       = (AT_MTA_Msg *)msg;
    xpassInfoInd = (MTA_AT_XpassInfoInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaXpassInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^XPASSINFO: %d,%d%s", g_atCrLf, xpassInfoInd->gphyXpassMode,
        xpassInfoInd->wphyXpassMode, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCFPlmnSetCnf(struct MsgCB *msg)
{
    TAF_MMA_CfplmnSetCnf *cFPLmnCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    cFPLmnCnf = (TAF_MMA_CfplmnSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cFPLmnCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFPLMN_SET) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnSetCnf : Current Option is not AT_CMD_CFPLMN_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cFPLmnCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        /* 这次代码的修改涉及接口的重构，为了保持和原有GU代码的一致性，返回结果出错时，结果设置为AT_CME_UNKNOWN  */
        result = AT_CME_UNKNOWN;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvMmaCFPlmnQueryCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_MMA_CfplmnQueryCnf *cFPlmnQrynf = VOS_NULL_PTR;
    TAF_USER_PlmnList      *cFPlmnList  = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT32              loop;

    /* 初始化消息变量 */
    indexNum    = 0;
    cFPlmnQrynf = (TAF_MMA_CfplmnQueryCnf *)msg;
    /*lint -save -e516 */
    cFPlmnList = (TAF_USER_PlmnList *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TAF_USER_PlmnList));
    /*lint -restore */
    if (cFPlmnList == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnQueryCnf : alloc fail");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(cFPlmnList, sizeof(TAF_USER_PlmnList), 0x00, sizeof(TAF_USER_PlmnList));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cFPlmnQrynf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnQueryCnf : WARNING:AT INDEX NOT FOUND!");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, cFPlmnList);
        /*lint -restore */
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnQueryCnf : AT_BROADCAST_INDEX.");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, cFPlmnList);
        /*lint -restore */
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFPLMN_READ) {
        AT_WARN_LOG("AT_RcvMmaCFPlmnQueryCnf : Current Option is not AT_CMD_CFREQLOCK_QUERY.");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, cFPlmnList);
        /*lint -restore */
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cFPlmnQrynf->rslt != TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        g_atSendDataBuff.bufLen = 0;
        /* 这次代码的修改涉及接口的重构，为了保持和原有GU代码的一致性，返回结果出错时，结果设置为AT_CME_UNKNOWN  */
        result = AT_CME_UNKNOWN;
        At_FormatResultData(indexNum, result);
    } else {
        if (cFPlmnQrynf->plmnNum > TAF_USER_MAX_PLMN_NUM) {
            cFPlmnQrynf->plmnNum = TAF_USER_MAX_PLMN_NUM;
        }

        cFPlmnList->plmnNum = cFPlmnQrynf->plmnNum;

        for (loop = 0; (loop < cFPlmnQrynf->plmnNum); loop++) {
            cFPlmnList->plmn[loop].mcc = cFPlmnQrynf->plmn[loop].mcc;
            cFPlmnList->plmn[loop].mnc = cFPlmnQrynf->plmn[loop].mnc;
        }

        At_QryParaRspCfplmnProc(indexNum, cFPlmnQrynf->ctrl.opId, (TAF_UINT8 *)cFPlmnList);
    }
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, cFPlmnList);
    /*lint -restore */
    return VOS_OK;
}


VOS_VOID AT_ReportQryPrefPlmnCmdPara(TAF_MMA_PrefPlmnQueryCnf *cpolQryCnf, AT_ModemNetCtx *netCtx, VOS_UINT16 *length,
                                     VOS_UINT32 indexNum, VOS_UINT32 loop)
{
    TAF_PLMN_NameList  *availPlmnInfo = VOS_NULL_PTR;
    AT_COPS_FORMAT_TYPE currFormat;

    availPlmnInfo = (TAF_PLMN_NameList *)&cpolQryCnf->plmnName;

    /* +CPOL:   */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <indexNum> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", ((loop + cpolQryCnf->fromIndex) + 1));

    currFormat = netCtx->cpolFormatType;

    if ((availPlmnInfo->plmnName[loop].operatorNameLong[0] == '\0') && (currFormat == AT_COPS_LONG_ALPH_TYPE)) {
        currFormat = AT_COPS_NUMERIC_TYPE;
    }

    if ((availPlmnInfo->plmnName[loop].operatorNameShort[0] == '\0') && (currFormat == AT_COPS_SHORT_ALPH_TYPE)) {
        currFormat = AT_COPS_NUMERIC_TYPE;
    }

    /* <format> */
    *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d", currFormat);

    /* <oper1> */
    if (currFormat == AT_COPS_LONG_ALPH_TYPE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"%s\"", availPlmnInfo->plmnName[loop].operatorNameLong);
    } else if (currFormat == AT_COPS_SHORT_ALPH_TYPE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"%s\"", availPlmnInfo->plmnName[loop].operatorNameShort);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"%X%X%X",
            (0x0f00 & availPlmnInfo->plmnName[loop].plmnId.mcc) >> 8,
            (0x00f0 & availPlmnInfo->plmnName[loop].plmnId.mcc) >> 4,
            (0x000f & availPlmnInfo->plmnName[loop].plmnId.mcc));

        if (((0x0f00 & availPlmnInfo->plmnName[loop].plmnId.mnc) >> 8) != 0x0F) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "%X",
                (0x0f00 & availPlmnInfo->plmnName[loop].plmnId.mnc) >> 8);
        }

        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%X%X\"",
            (0x00f0 & availPlmnInfo->plmnName[loop].plmnId.mnc) >> 4,
            (0x000f & availPlmnInfo->plmnName[loop].plmnId.mnc));
    }

    return;
}


VOS_VOID AT_ReportQryPrefPlmnCmd(TAF_MMA_PrefPlmnQueryCnf *cpolQryCnf, VOS_UINT32 *validPlmnNum, AT_ModemNetCtx *netCtx,
                                 VOS_UINT16 *length, VOS_UINT32 indexNum)
{
    VOS_UINT32         i;
    TAF_PLMN_NameList *availPlmnInfo = VOS_NULL_PTR;
    TAF_MMC_USIM_RAT   plmnRat;

    availPlmnInfo = (TAF_PLMN_NameList *)&cpolQryCnf->plmnName;

    availPlmnInfo->plmnNum = AT_MIN(availPlmnInfo->plmnNum, TAF_MMA_MAX_PLMN_NAME_LIST_NUM);
    for (i = 0; i < availPlmnInfo->plmnNum; i++) {
        if (AT_PH_IsPlmnValid(&(availPlmnInfo->plmnName[i].plmnId)) == VOS_FALSE) {
            continue;
        }

        (*validPlmnNum)++;

        if ((cpolQryCnf->validPlmnNum == 0) && (*validPlmnNum == 1)) {
            /* 参考V.250协议5.7.2: 根据V命令的设置给信息字段的头尾增加回车换行符 */
            if (g_atVType == AT_V_ENTIRE_TYPE) {
                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s", g_atCrLf);
            }
        } else {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s", g_atCrLf);
        }

        AT_ReportQryPrefPlmnCmdPara(cpolQryCnf, netCtx, length, indexNum, i);

        if ((netCtx->prefPlmnType == MN_PH_PREF_PLMN_UPLMN) && (availPlmnInfo->plmnSelFlg == VOS_TRUE)) {
            continue;
        }

        /* <GSM_AcT1> */
        /* <GSM_Compact_AcT1> */
        /* <UTRAN_AcT1> */
        plmnRat = availPlmnInfo->plmnRat[i];

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        *length +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d,%d,%d,%d,%d", (plmnRat & TAF_MMC_USIM_GSM_RAT) >> 7,
                (plmnRat & TAF_MMC_USIM_GSM_COMPACT_RAT) >> 6, (plmnRat & TAF_MMC_USIM_UTRN_RAT) >> 15,
                (plmnRat & TAF_MMC_USIM_E_UTRN_RAT) >> 14, (plmnRat & TAF_MMC_USIM_NR_RAT) >> 11);
#else
        *length +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d,%d,%d,%d", (plmnRat & TAF_MMC_USIM_GSM_RAT) >> 7,
                (plmnRat & TAF_MMC_USIM_GSM_COMPACT_RAT) >> 6, (plmnRat & TAF_MMC_USIM_UTRN_RAT) >> 15,
                (plmnRat & TAF_MMC_USIM_E_UTRN_RAT) >> 14);
#endif
    }

    return;
}

VOS_UINT32 AT_RcvMmaPrefPlmnSetCnf(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_MMA_PrefPlmnSetCnf *prefPlmnCnf = VOS_NULL_PTR;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    indexNum    = 0;
    result      = AT_ERROR;
    prefPlmnCnf = (TAF_MMA_PrefPlmnSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(prefPlmnCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOL_SET) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnSetCnf : Current Option is not AT_CMD_CPOL_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (prefPlmnCnf->rslt != TAF_MMA_APP_PREF_PLMN_OPER_RESULT_SUCCESS) {
        result = AT_CME_UNKNOWN;
    } else {
        result = AT_OK;
    }

    /* 格式化AT+CPOL测试命令返回 */
    g_atSendDataBuff.bufLen = 0;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPrefPlmnQueryCnf(struct MsgCB *msg)
{
    VOS_UINT32                result        = AT_FAILURE;
    VOS_UINT16                length        = 0;
    TAF_PLMN_NameList        *availPlmnInfo = VOS_NULL_PTR;
    AT_ModemNetCtx           *netCtx        = VOS_NULL_PTR;
    VOS_UINT32                validPlmnNum;
    TAF_MMA_CpolInfoQueryReq  cpolInfo;
    VOS_UINT8                 indexNum   = 0;
    TAF_MMA_PrefPlmnQueryCnf *cpolQryCnf = VOS_NULL_PTR;

    cpolQryCnf = (TAF_MMA_PrefPlmnQueryCnf *)msg;

    availPlmnInfo = (TAF_PLMN_NameList *)&cpolQryCnf->plmnName;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cpolQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_QryParaRspPrefPlmnProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_QryParaRspPrefPlmnProc : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 判断当前操作类型是否为AT_CMD_CPOL_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOL_READ) {
        return VOS_ERR;
    }

    if (cpolQryCnf->rslt != TAF_MMA_APP_PREF_PLMN_OPER_RESULT_SUCCESS) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_CME_UNKNOWN);

        return VOS_OK;
    }

    validPlmnNum = 0;
    AT_ReportQryPrefPlmnCmd(cpolQryCnf, &validPlmnNum, netCtx, &length, indexNum);

    /*
     * AT模块根据MMA上报的运营商个数来确认运营商信息是否收集完成: 小于请求的运营商
     * 个数则认为运营商信息已经收集完成，原因如下所述:
     * AT模块每次请求37条运营商信息，起始位置为已经输出的最后一个运营商的下一条运营商信息索引
     * 如果待输出的运营商信息总数不足37条，则按实际的输出，
     * 如果运营商数为37的倍数，则AT会再发一次运营商信息请求，MMA回复的运营商信息总数为0
     */
    if (availPlmnInfo->plmnNum < TAF_MMA_MAX_PLMN_NAME_LIST_NUM) {
        /* 参考V.250协议5.7.2: 根据V命令的设置给信息字段的头尾增加回车换行符 */
        if ((g_atVType == AT_V_ENTIRE_TYPE) && ((cpolQryCnf->validPlmnNum + validPlmnNum) != 0)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);

        cpolInfo.prefPLMNType = netCtx->prefPlmnType;
        cpolInfo.fromIndex    = (cpolQryCnf->fromIndex + availPlmnInfo->plmnNum);
        cpolInfo.plmnNum      = TAF_MMA_MAX_PLMN_NAME_LIST_NUM;
        cpolInfo.validPlmnNum = (cpolQryCnf->validPlmnNum + validPlmnNum);
        result                = TAF_MMA_QueryCpolReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &cpolInfo);
        if (result != VOS_TRUE) {
            /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_CME_UNKNOWN);
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPrefPlmnTestCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_MMA_PrefPlmnTestCnf *prefPlmnCnf = VOS_NULL_PTR;
    VOS_UINT16               length;
    VOS_UINT32               result;

    /* 初始化消息变量 */
    indexNum    = 0;
    length      = 0;
    result      = AT_ERROR;
    prefPlmnCnf = (TAF_MMA_PrefPlmnTestCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(prefPlmnCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTestCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTestCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPOL_TEST) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTestCnf : Current Option is not AT_CMD_CPOL_TEST.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (prefPlmnCnf->rslt != TAF_MMA_APP_PREF_PLMN_OPER_RESULT_SUCCESS) {
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_CME_UNKNOWN);

        return VOS_OK;
    }

    /* 格式化AT+CPOL测试命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (prefPlmnCnf->plmnNum == 0) {
        result = AT_CME_OPERATION_NOT_ALLOWED;
    } else {
        length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (1-%d),(0-2)", g_parseContext[indexNum].cmdElement->cmdName,
            prefPlmnCnf->plmnNum);

        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAcInfoChangeInd(struct MsgCB *msg)
{
    VOS_UINT16               length;
    TAF_MMA_CellAcInfo      *cellAcInfo = VOS_NULL_PTR;
    ModemIdUint16            modemId;
    VOS_UINT32               rslt;
    TAF_MMA_AcInfoChangeInd *psAcInfoChangeInd = VOS_NULL_PTR;
    VOS_UINT8                indexNum          = 0;

    psAcInfoChangeInd = (TAF_MMA_AcInfoChangeInd *)msg;

    if (At_ClientIdToUserId(psAcInfoChangeInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAcInfoChangeInd : AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    modemId = MODEM_ID_0;

    length     = 0;
    cellAcInfo = (TAF_MMA_CellAcInfo *)(&psAcInfoChangeInd->cellAcInfo);

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaAcInfoChangeInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* 通过AT^CURC控制紧急呼叫号码不上报 */

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d%s", g_atCrLf,
        g_atStringTab[AT_STRING_ACINFO].text, cellAcInfo->srvDomain, /* 上报服务域 */
        cellAcInfo->cellAcType,        /* 上报小区禁止接入类型 */
        cellAcInfo->restrictRegister,  /* 上报是否注册受限 */
        cellAcInfo->restrictPagingRsp, /* 上报是否寻呼受限 */
        g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

/*
 * Function Name:   AT_RcvMmaEOPlmnSetCnf
 * Input parameters:   struct MsgCB                           *pstMsg
 *     1)  Date:   2015-03-24
 *         Modify content:    Create
 */
VOS_UINT32 AT_RcvMmaEOPlmnSetCnf(struct MsgCB *msg)
{
    VOS_UINT8             indexNum = 0;
    VOS_UINT32            rslt;
    TAF_MMA_EoplmnSetCnf *eOPlmnSetCnf = VOS_NULL_PTR;

    eOPlmnSetCnf = (TAF_MMA_EoplmnSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(eOPlmnSetCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEOPlmnSetCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEOPlmnSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EOPLMN_SET) {
        AT_WARN_LOG("AT_RcvMmaEOPlmnSetCnf: WARNING:Not AT_CMD_EOPLMN_SET!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (eOPlmnSetCnf->result == TAF_ERR_NO_ERROR) {
        rslt = AT_OK;
    } else {
        rslt = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEOPlmnQryCnf(struct MsgCB *msg)
{
    VOS_UINT16            len;
    VOS_UINT8             indexNum = 0;
    VOS_UINT32            rslt;
    TAF_MMA_EoplmnQryCnf *eOPlmnQryCnf = VOS_NULL_PTR;

    len          = 0;
    eOPlmnQryCnf = (TAF_MMA_EoplmnQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(eOPlmnQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEOPlmnQryCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_EOPLMN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EOPLMN_QRY) {
        AT_WARN_LOG("AT_RcvMmaEOPlmnQryCnf: WARNING:Not AT_CMD_EOPLMN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出查询结果 */
    if (eOPlmnQryCnf->result == TAF_ERR_NO_ERROR) {
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%s: \"%s\",%d,", g_parseContext[indexNum].cmdElement->cmdName,
            eOPlmnQryCnf->version, eOPlmnQryCnf->oPlmnNum * TAF_AT_PLMN_WITH_RAT_LEN);

        /* 将16进制数转换为ASCII码后输入主动命令内容 */
        len += (TAF_UINT16)At_HexText2AsciiStringSimple(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                        (TAF_UINT8 *)g_atSndCodeAddress + len,
                                                        eOPlmnQryCnf->oPlmnNum * TAF_SIM_PLMN_WITH_RAT_LEN,
                                                        eOPlmnQryCnf->oPlmnList);

        g_atSendDataBuff.bufLen = len;

        rslt = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        rslt                    = AT_ERROR;
    }

    At_FormatResultData(indexNum, rslt);

    return VOS_OK;
}


VOS_VOID AT_ReportNetScanInfo(VOS_UINT8 indexNum, VOS_UINT8 freqNum, TAF_MMA_NetScanInfo *netScanInfo)
{
    VOS_CHAR  *pcBandFormat = VOS_NULL_PTR;
    VOS_UINT16 length;
    VOS_UINT32 i;
    VOS_INT32  j;
    VOS_CHAR   cellIdStr[AT_CELLID_STRING_MAX_LEN];
    VOS_BOOL   bFirstBand;
    VOS_UINT8  freqNumTmp;

    length     = 0;
    freqNumTmp = (VOS_UINT8)AT_MIN(freqNum, TAF_MMA_NET_SCAN_MAX_FREQ_NUM);
    (VOS_VOID)memset_s(cellIdStr, sizeof(cellIdStr), 0x00, sizeof(cellIdStr));

    for (i = 0; i < freqNumTmp; i++) {
        bFirstBand = VOS_FALSE;

        /* ^NETSCAN: [<arfcn>],[<c1>],[<c2>],[<lac>],[<mcc>],[<mnc>] */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %u,,,%x,%x%x%x,%x%x",
            g_parseContext[indexNum].cmdElement->cmdName, netScanInfo[i].arfcn, netScanInfo[i].lac,
            (netScanInfo[i].mcc & 0x0f), (netScanInfo[i].mcc & 0x0f00) >> 8, (netScanInfo[i].mcc & 0x0f0000) >> 16,
            (netScanInfo[i].mnc & 0x0f), (netScanInfo[i].mnc & 0x0f00) >> 8);

        /* MNC如果最后一个不是F，则需要显示 */
        if (((netScanInfo[i].mnc & 0x0f0000) >> 16) != 0x0f) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%x", (netScanInfo[i].mnc & 0x0f0000) >> 16);
        }

        /* NR下CellId后续扩展为U64再打印高4字节 */
        /* ,[<bsic>],[<rxlev>][,<cid>, */
        AT_ConvertCellIdToHexStrFormat(netScanInfo[i].cellId.cellIdLowBit, netScanInfo[i].cellId.cellIdHighBit,
                                       cellIdStr);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%s,", netScanInfo[i].bsic, netScanInfo[i].rxlev,
            cellIdStr);

        /* [<band>] */
        for (j = TAF_MMA_BAND_MAX_LEN - 1; j >= 0; j--) {
            if ((netScanInfo[i].band.band[j] == 0) && (bFirstBand == VOS_FALSE)) {
                continue;
            }

            if (bFirstBand == VOS_FALSE) {
                pcBandFormat = "%X";
                bFirstBand   = VOS_TRUE;
            } else {
                pcBandFormat = "%08X";
            }

            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, pcBandFormat, netScanInfo[i].band.band[j]);
        }

        /* ,[psc],[pci] */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%u", netScanInfo[i].psc, netScanInfo[i].phyId);

        if (netScanInfo[i].nrFlg == VOS_TRUE) {
            /* [,<5GSCS>,<5GRSRP>,<5GRSRQ>,<5GSINR>]] */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d,%d", netScanInfo[i].nrScsInfo,
                netScanInfo[i].nrRsrp, netScanInfo[i].nrRsrq, netScanInfo[i].nrSinr);
        }

        if (i != (freqNum - 1)) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 如果FREQNUM ==0，则只上报OK */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return;
}


VOS_UINT32 AT_ConvertMmaNetScanCauseToAt(TAF_MMA_NetScanCauseUint8 netScanCause)
{
    VOS_UINT32 result;

    /* 根据NetScan上报的错误码转换成AT显示的错误码 */
    switch (netScanCause) {
        case TAF_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST:
            result = AT_ERROR;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_FREQ_LOCK:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_PARA_ERROR:
            result = AT_CME_INCORRECT_PARAMETERS;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_CONFLICT:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_SERVICE_EXIST:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_NOT_CAMPED:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_TIMER_EXPIRED:
            result = AT_ERROR;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR:
            result = AT_CME_OPERATION_NOT_ALLOWED;
            break;

        case TAF_MMA_NET_SCAN_CAUSE_MMA_STATE_DISABLE:
            result = AT_ERROR;
            break;

        default:
            result = AT_ERROR;
            break;
    }

    return result;
}


VOS_UINT32 AT_RcvMmaNetScanCnf(struct MsgCB *msg)
{
    TAF_MMA_NetScanCnf *netScanCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexNum = 0;

    netScanCnf = (TAF_MMA_NetScanCnf *)msg;

    if (At_ClientIdToUserId(netScanCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaNetScanCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaNetScanCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (netScanCnf->result == TAF_MMA_NET_SCAN_RESULT_SUCCESS) {
        AT_ReportNetScanInfo(indexNum, netScanCnf->freqNum, netScanCnf->netScanInfo);
    } else {
        AT_STOP_TIMER_CMD_READY(indexNum);

        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ConvertMmaNetScanCauseToAt(netScanCnf->cause);
        At_FormatResultData(indexNum, result);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaUserSrvStateQryCnf(struct MsgCB *msg)
{
    TAF_MMA_UserSrvStateQryCnf *userSrvStateCnf = VOS_NULL_PTR;
    VOS_UINT16                  length;
    VOS_UINT8                   indexNum = 0;

    userSrvStateCnf = (TAF_MMA_UserSrvStateQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(userSrvStateCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaUserSrvStateQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaUserSrvStateQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_USER_SRV_STATE_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_USER_SRV_STATE_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^USERSRVSTATE: ");

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,%d", userSrvStateCnf->csSrvExistFlg,
        userSrvStateCnf->psSrvExistFlg);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPwrOnAndRegTimeQryCnf(struct MsgCB *msg)
{
    VOS_UINT32                       costTime;
    VOS_UINT16                       length;
    VOS_UINT32                       result;
    TAF_MMA_PowerOnAndRegTimeQryCnf *appwronregCnf = VOS_NULL_PTR;
    VOS_UINT8                        indexNum      = 0;

    appwronregCnf = (TAF_MMA_PowerOnAndRegTimeQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(appwronregCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPwrOnAndRegTimeQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPwrOnAndRegTimeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_REGISTER_TIME_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_REGISTER_TIME_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    costTime = appwronregCnf->costTime;

    /* 初始化 */
    result = AT_OK;
    length = 0;

    /*
     * MMA报给AT时，按slice上报，(32 * 1024)个slice是1S
     * 如果slice为0，表示没有注册成功；如果slice小于1S,AT按1S上报
     */

    /* 将查询时间上报给APP */
    if (costTime == 0) {
        result = AT_ERROR;
    } else {
        costTime = costTime / (32 * 1024); /* MMA报给AT时，按slice上报，(32 * 1024)个slice是1S */

        if (costTime == 0) {
            costTime = 1;
        }

        result = AT_OK;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, costTime);
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaSpnQryCnf(struct MsgCB *msg)
{
    errno_t            memResult;
    TAF_MMA_SpnQryCnf *spnCnf = VOS_NULL_PTR;
    VOS_UINT32         result;
    TAF_PH_UsimSpn     atSPNRslt;
    VOS_UINT16         length;
    VOS_UINT16         datalen;
    VOS_UINT8         *systemAppConfig = VOS_NULL_PTR;
    VOS_UINT8          indexNum        = 0;
    AT_ModemNetCtx    *netCtx          = VOS_NULL_PTR;

    spnCnf = (TAF_MMA_SpnQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(spnCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSpnQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSpnQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SPN_QUERY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SPN_QUERY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* 初始化 */
    result  = AT_OK;
    length  = 0;
    datalen = 0;

   (VOS_VOID)memset_s(&atSPNRslt, (VOS_SIZE_T)sizeof(atSPNRslt), 0x00, (VOS_SIZE_T)sizeof(atSPNRslt));
    if (netCtx->spnType == 1) {
        memResult = memcpy_s(&atSPNRslt, sizeof(atSPNRslt), &(spnCnf->mnMmaSpnInfo.usimSpnInfo),
                             sizeof(TAF_PH_UsimSpn));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atSPNRslt), sizeof(TAF_PH_UsimSpn));
    } else {
        memResult = memcpy_s(&atSPNRslt, sizeof(atSPNRslt), &(spnCnf->mnMmaSpnInfo.simSpnInfo), sizeof(TAF_PH_UsimSpn));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atSPNRslt), sizeof(TAF_PH_UsimSpn));
    }

    if (atSPNRslt.length == 0) {
        /*  SPN file not exist */
        result = AT_CME_SPN_FILE_NOT_EXISTS;
    } else {
        /* display format: ^SPN:disp_rplmn,coding,spn_name */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        if (*systemAppConfig == SYSTEM_APP_WEBUI) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,\"", atSPNRslt.dispRplmnMode & 0x03, atSPNRslt.coding);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,\"", atSPNRslt.dispRplmnMode & 0x01, atSPNRslt.coding);
        }

        if (atSPNRslt.coding == TAF_PH_GSM_7BIT_DEFAULT) {
            datalen = TAF_MIN(atSPNRslt.length, TAF_PH_SPN_NAME_MAXLEN);
            if ((length + TAF_PH_SPN_NAME_MAXLEN) <= AT_SEND_CODE_ADDR_MAX_LEN) {
                memResult = memcpy_s(g_atSndCodeAddress + length, AT_SEND_CODE_ADDR_MAX_LEN - length, atSPNRslt.spnName,
                                     datalen);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_SEND_CODE_ADDR_MAX_LEN - length,
                                    datalen);
            }
            length = length + datalen;
        } else {
            datalen = atSPNRslt.length;

            length += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                          (VOS_UINT8 *)g_atSndCodeAddress + length, atSPNRslt.spnName,
                                                          (VOS_UINT16)datalen);
        }
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
    }

    g_atSendDataBuff.bufLen = length;

    /* 回复用户命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaMMPlmnInfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_MmplmninfoQryCnf *mMPlmnInfoCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT16                length;
    VOS_UINT32                i;
    VOS_UINT8                 indexNum = 0;

    /* 变量初始化 */
    mMPlmnInfoCnf = (TAF_MMA_MmplmninfoQryCnf *)msg;
    result        = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(mMPlmnInfoCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaMMPlmnInfoQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaMMPlmnInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MMPLMNINFO_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MMPLMNINFO_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 转换LongName及ShortName */
    if (mMPlmnInfoCnf->mmPlmnInfo.longNameLen <= TAF_PH_OPER_NAME_LONG &&
        mMPlmnInfoCnf->mmPlmnInfo.shortNameLen <= TAF_PH_OPER_NAME_SHORT) {
        /* ^MMPLMNINFO:<long name>,<short name> */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

        for (i = 0; i < mMPlmnInfoCnf->mmPlmnInfo.longNameLen; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mMPlmnInfoCnf->mmPlmnInfo.longName[i]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");

        for (i = 0; i < mMPlmnInfoCnf->mmPlmnInfo.shortNameLen; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mMPlmnInfoCnf->mmPlmnInfo.shortName[i]);
        }

        result                  = AT_OK;
        g_atSendDataBuff.bufLen = length;
    } else {
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPlmnQryCnf(struct MsgCB *msg)
{
    TAF_MMA_LastCampPlmnQryCnf *plmnCnf = VOS_NULL_PTR;
    NAS_MM_InfoInd              mmTimeInfo;
    VOS_UINT16                  length;
    VOS_UINT8                   indexNum = 0;

    plmnCnf = (TAF_MMA_LastCampPlmnQryCnf *)msg;
    length  = 0;
    (VOS_VOID)memset_s(&mmTimeInfo, sizeof(NAS_MM_InfoInd), 0x00, sizeof(NAS_MM_InfoInd));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPlmnQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPlmnQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PLMN_QRY 或 AT_CMD_TIMEQRY_QRY */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PLMN_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TIMEQRY_QRY)) {
        return VOS_ERR;
    }

    /* 无效的PLMN */
    if ((plmnCnf->plmnId.mcc == 0) && (plmnCnf->plmnId.mnc == 0)) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_OK);
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PLMN_QRY) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^PLMN: ", g_atCrLf);

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x,", (plmnCnf->plmnId.mcc & 0x0f00) >> 8,
            (plmnCnf->plmnId.mcc & 0xf0) >> 4, (plmnCnf->plmnId.mcc & 0x0f));

        if ((plmnCnf->plmnId.mnc & 0x0f00) == 0x0f00) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x", (plmnCnf->plmnId.mnc & 0xf0) >> 4,
                (plmnCnf->plmnId.mnc & 0x0f));
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x", (plmnCnf->plmnId.mnc & 0x0f00) >> 8,
                (plmnCnf->plmnId.mnc & 0xf0) >> 4, (plmnCnf->plmnId.mnc & 0x0f));
        }

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    } else {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        if (AT_PlmnTimeZoneProc(plmnCnf->plmnId.mcc, &mmTimeInfo) == VOS_OK) {
            length = AT_PrintNwTimeInfo(&mmTimeInfo, (VOS_CHAR *)g_parseContext[indexNum].cmdElement->cmdName, 0);
        } else {
            At_FormatResultData(indexNum, AT_ERROR);
            return VOS_ERR;
        }
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT32 AT_GetTimeZoneByMcc(VOS_UINT16 plmnMcc, VOS_INT8 *pcMccZoneInfo)
{
    VOS_UINT32 totalMccNum;
    VOS_UINT32 i   = 0;
    VOS_UINT32 ret = VOS_FALSE;

    totalMccNum = AT_ARRAY_SIZE(g_mccZoneInfoTbl);

    for (i = 0; i < totalMccNum; i++) {
        if (plmnMcc == g_mccZoneInfoTbl[i].mcc) {
            *pcMccZoneInfo = g_mccZoneInfoTbl[i].zone;

            ret = VOS_TRUE;

            break;
        }
    }

    return ret;
}


VOS_UINT32 AT_PlmnTimeZoneProc(TAF_UINT32 mcc, NAS_MM_InfoInd *mmTimeInfo)
{
    VOS_UINT32 result = VOS_ERR;
    VOS_UINT16 mccTemp;
    VOS_INT8   timeZone = 0;

    /* Get MCC */
    mccTemp = (VOS_UINT16)mcc;

    /* ^TIMEQRY在没收到EMM INFO的时候根据MCC计算时区 */
    if (AT_GetTimeZoneByMcc(mccTemp, &timeZone) == VOS_TRUE) {
        /* 能走到这个分支说明之前A核没有拿到过MM INFO时间，因此填充一个默认值 */
        mmTimeInfo->ieFlg                                  = NAS_MM_INFO_IE_UTLTZ;
        mmTimeInfo->universalTimeandLocalTimeZone.year     = AT_CMD_TIMEQRY_YEAR_DEFAULT_VALUE;
        mmTimeInfo->universalTimeandLocalTimeZone.month    = AT_CMD_TIMEQRY_MONTH_DEFAULT_VALUE;
        mmTimeInfo->universalTimeandLocalTimeZone.day      = AT_CMD_TIMEQRY_DAY_DEFAULT_VALUE;
        mmTimeInfo->universalTimeandLocalTimeZone.hour     = AT_CMD_TIMEQRY_HOUR_DEFAULT_VALUE;
        mmTimeInfo->universalTimeandLocalTimeZone.timeZone = timeZone;
        mmTimeInfo->localTimeZone                          = timeZone;

        result = VOS_OK;
    } else {
        result = VOS_ERR;
    }

    return result;
}


VOS_UINT32 AT_RcvMmaCerssiSetCnf(struct MsgCB *msg)
{
    TAF_MMA_CerssiSetCnf *cerssiSetCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum     = 0;
    VOS_UINT32            result;

    cerssiSetCnf = (TAF_MMA_CerssiSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cerssiSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCerssiSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCerssiSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CERSSI_SET) {
        AT_WARN_LOG("AT_RcvMmaCerssiSetCnf : Current Option is not AT_CMD_CERSSI_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cerssiSetCnf->errorCause == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cerssiSetCnf->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCindSetCnf(struct MsgCB *msg)
{
    TAF_MMA_CIND_SET_CNF_STRU *cindSetCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum   = 0;
    VOS_UINT32                 result;

    cindSetCnf = (TAF_MMA_CIND_SET_CNF_STRU *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cindSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCindSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCindSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIND_SET) {
        AT_WARN_LOG("AT_RcvMmaCindSetCnf : Current Option is not AT_CMD_CIND_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cindSetCnf->errorCause == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cindSetCnf->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPlmnSearchCnf(struct MsgCB *msg)
{
    TAF_MMA_PlmnSearchCnf *plmnSearchCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum;
    VOS_UINT32             result;

    plmnSearchCnf = (TAF_MMA_PlmnSearchCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnSearchCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPlmnSearchCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPlmnSearchCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_SET_AUTOMATIC) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_SET_MANUAL) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_SET_MANUAL_AUTOMATIC_MANUAL) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PLMNSRCH_SET)) {
        AT_WARN_LOG("AT_RcvMmaPlmnSearchCnf : Current Option is invalid.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (plmnSearchCnf->errorCause == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, plmnSearchCnf->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPlmnListAbortCnf(struct MsgCB *msg)
{
    TAF_MMA_PlmnListAbortCnf *plmnListAbortCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;

    plmnListAbortCnf = (TAF_MMA_PlmnListAbortCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnListAbortCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPlmnSpecialSelSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPlmnSpecialSelSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    /*
     * 容错处理, 当前不在列表搜ABORT过程中则不上报ABORT.
     * 如AT的ABORT保护定时器已超时, 之后再收到MMA的ABORT_CNF则不上报ABORT
     */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_ABORT_PLMN_LIST) {
        AT_WARN_LOG("AT_RcvMmaPlmnSpecialSelSetCnf : Current Option is not AT_CMD_COPS_SET_MANUAL.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ABORT;

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMmaPowerDownCnf(struct MsgCB *msg)
{
    TAF_MMA_MtPowerDownCnf *prefPlmnTypeSetCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;

    prefPlmnTypeSetCnf = (TAF_MMA_MtPowerDownCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(prefPlmnTypeSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPowerDownCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPowerDownCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MMA_MT_POWER_DOWN) {
        AT_WARN_LOG("AT_RcvMmaPowerDownCnf : Current Option is not AT_CMD_MMA_MT_POWER_DOWN.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (prefPlmnTypeSetCnf->errorCause == TAF_ERR_NO_ERROR) {
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

VOS_UINT32 AT_RcvMmaPrefPlmnTypeSetCnf(struct MsgCB *msg)
{
    TAF_MMA_PrefPlmnTypeSetCnf *prefPlmnTypeSetCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;
    VOS_UINT32                  result;
    AT_ModemNetCtx             *netCtx = VOS_NULL_PTR;

    prefPlmnTypeSetCnf = (TAF_MMA_PrefPlmnTypeSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(prefPlmnTypeSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTypeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTypeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MMA_SET_PREF_PLMN_TYPE) {
        AT_WARN_LOG("AT_RcvMmaPrefPlmnTypeSetCnf : Current Option is not AT_CMD_MMA_SET_PREF_PLMN_TYPE.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    netCtx->prefPlmnType = prefPlmnTypeSetCnf->prefPlmnType;

    result = AT_OK;

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvMmaPhoneModeQryCnf(struct MsgCB *msg)
{
    TAF_MMA_PhoneModeQryCnf *phoneModeQryCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum        = 0;
    VOS_UINT32               result;

    phoneModeQryCnf = (TAF_MMA_PhoneModeQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(phoneModeQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPhoneModeQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPhoneModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前操作是否为查询操作AT_CMD_CFUN_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFUN_READ) {
        AT_WARN_LOG("AT_RcvMmaPhoneModeQryCnf : Current Option is not AT_CMD_CFUN_READ.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (phoneModeQryCnf->errorCause == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                phoneModeQryCnf->phMode);
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, phoneModeQryCnf->errorCause);
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaQuickStartSetCnf(struct MsgCB *msg)
{
    TAF_MMA_QuickstartSetCnf *quickStartSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum         = 0;
    VOS_UINT32                result;

    AT_PR_LOGI("Rcv Msg");

    quickStartSetCnf = (TAF_MMA_QuickstartSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(quickStartSetCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaQuickStartSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaQuickStartSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前操作是否为查询操作AT_CMD_CQST_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CQST_SET) {
        AT_WARN_LOG("AT_RcvTafMmaQuickStartSetCnf : Current Option is not AT_CMD_CQST_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (quickStartSetCnf->result == TAF_PARA_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaAutoAttachSetCnf(struct MsgCB *msg)
{
    TAF_MMA_AutoAttachSetCnf *autoAttachSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    AT_RreturnCodeUint32      result = AT_FAILURE;

    autoAttachSetCnf = (TAF_MMA_AutoAttachSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(autoAttachSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaAutoAttachSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaAutoAttachSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前操作是否为查询操作AT_CMD_CAATT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CAATT_SET) {
        AT_WARN_LOG("AT_RcvTafMmaAutoAttachSetCnf : Current Option is not AT_CMD_CAATT_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (autoAttachSetCnf->result == TAF_PARA_OK) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaSyscfgQryCnf(struct MsgCB *msg)
{
    TAF_MMA_SyscfgQryCnf *syscfgQryCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    AT_RreturnCodeUint32  result = AT_FAILURE;

    syscfgQryCnf = (TAF_MMA_SyscfgQryCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    AT_PR_LOGI("Rcv Msg");

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(syscfgQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaSyscfgQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaSyscfgQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if (syscfgQryCnf->errorCause != TAF_ERR_NO_ERROR) {                  /* MT本地错误 */
        result = At_ChgTafErrorCode(indexNum, syscfgQryCnf->errorCause); /* 发生错误 */
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SYSCFG_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_ReportSysCfgQryCmdResult(&(syscfgQryCnf->sysCfg), indexNum);

        result = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SYSCFGEX_READ) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_ReportSysCfgExQryCmdResult(&(syscfgQryCnf->sysCfg), indexNum);

        result = AT_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_WS46_QRY) {
        AT_STOP_TIMER_CMD_READY(indexNum);

        result = AT_ReportWs46QryCmdResult(&(syscfgQryCnf->sysCfg), indexNum);
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAccessModeQryCnf(struct MsgCB *msg)
{
    TAF_MMA_AccessModeQryCnf *accessModeCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum = 0;

    /* 初始化 */
    accessModeCnf = (TAF_MMA_AccessModeQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(accessModeCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAccessModeQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaAccessModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CPAM_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CPAM_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 如果查询的接入模式为无效值，则返回ERROR，例如当前LTE only不支持GU */
    if (accessModeCnf->accessMode >= MN_MMA_CPAM_RAT_TYPE_BUTT) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            accessModeCnf->accessMode, accessModeCnf->plmnPrio);
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCopsQryCnf(struct MsgCB *msg)
{
    TAF_MMA_CopsQryCnf *copsQryCnf = VOS_NULL_PTR;
    VOS_UINT8           indexNum   = 0;

    /* 初始化 */
    copsQryCnf = (TAF_MMA_CopsQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(copsQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCopsQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCopsQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_COPS_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COPS_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    At_QryParaRspCopsProc(indexNum, copsQryCnf->opId, (VOS_UINT8 *)&(copsQryCnf->copsInfo));

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRegStateQryCnf(struct MsgCB *msg)
{
    TAF_MMA_RegStateQryCnf *regStateCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum    = 0;

    /* 初始化 */
    regStateCnf = (TAF_MMA_RegStateQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(regStateCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRegStateQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRegStateQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CREG_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CREG_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        At_QryParaRspCregProc(indexNum, regStateCnf->ctrl.opId, (VOS_UINT8 *)&(regStateCnf->regInfo));
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CGREG_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        At_QryParaRspCgregProc(indexNum, regStateCnf->ctrl.opId, (VOS_UINT8 *)&(regStateCnf->regInfo));
    }
#if (FEATURE_LTE == FEATURE_ON)
    else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CEREG_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_QryParaRspCeregProc(indexNum, regStateCnf->ctrl.opId, (VOS_UINT8 *)&(regStateCnf->regInfo));
    }
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_C5GREG_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_QryParaRspC5gregProc(indexNum, regStateCnf->ctrl.opId, (VOS_UINT8 *)&(regStateCnf->regInfo));
    }
#endif
    else {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaAutoAttachQryCnf(struct MsgCB *msg)
{
    TAF_MMA_AutoAttachQryCnf *autoAttachCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum = 0;

    /* 初始化 */
    autoAttachCnf = (TAF_MMA_AutoAttachQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(autoAttachCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaAutoAttachQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaAutoAttachQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CAATT_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CAATT_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (autoAttachCnf->errorCause == TAF_ERR_NO_ERROR) {
        result = AT_OK;

        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            autoAttachCnf->autoAttachFlag);
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaSysInfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_SysinfoQryCnf *sysInfoCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum   = 0;

    /* 初始化 */
    sysInfoCnf = (TAF_MMA_SysinfoQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(sysInfoCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSysInfoQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (sysInfoCnf->isSupport == VOS_FALSE) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        At_FormatResultData(indexNum, AT_CMD_NOT_SUPPORT);

        return VOS_OK;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSysInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SYSINFO_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SYSINFO_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        At_QryParaRspSysinfoProc(indexNum, sysInfoCnf->opId, (TAF_UINT8 *)&(sysInfoCnf->sysInfo));

        return VOS_OK;
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SYSINFOEX_READ) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        AT_QryParaRspSysinfoExProc(indexNum, sysInfoCnf->opId, (TAF_UINT8 *)&(sysInfoCnf->sysInfo));

        return VOS_OK;
    } else {
        return VOS_ERR;
    }
}


VOS_UINT32 AT_RcvMtaAnqueryQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg            = VOS_NULL_PTR;
    MTA_AT_AntennaInfoQryCnf *antennaInfoQryCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum = 0;

    /* 初始化 */
    rcvMsg            = (AT_MTA_Msg *)msg;
    antennaInfoQryCnf = (MTA_AT_AntennaInfoQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaAnqueryQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaAnqueryQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ANQUERY_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ANQUERY_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (antennaInfoQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        AT_QryParaAnQueryProc(indexNum, rcvMsg->appCtrl.opId, (VOS_UINT8 *)&(antennaInfoQryCnf->antennaInfo));
    } else {
        result = AT_CME_UNKNOWN;

        At_FormatResultData(indexNum, result);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEHplmnInfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_HomePlmnQryCnf *hplmnCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum = 0;

    hplmnCnf = (TAF_MMA_HomePlmnQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(hplmnCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEHplmnInfoQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEHplmnInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 根据当前操作类型进入^HPLMN/^APHPLMN的处理 */
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_HOMEPLMN_READ) {
        /* ^APHPLMN */
        return AT_RcvMmaApHplmnQryCnf(indexNum, msg);
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_EHPLMN_LIST_QRY) {
        /* ^HPLMN */
        return AT_RcvMmaHplmnQryCnf(indexNum, msg);
    } else {
        return VOS_ERR;
    }
}


VOS_UINT32 AT_RcvMmaApHplmnQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg)
{
    TAF_MMA_HomePlmnQryCnf *hplmnCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    TAF_MMA_HplmnWithMncLen hplmn;

    /* 初始化 */
    hplmnCnf = (TAF_MMA_HomePlmnQryCnf *)msg;

    /* index的异常判断移到AT_RcvMmaEHplmnInfoQryCnf中 */

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (hplmnCnf->errorCause == TAF_ERR_NO_ERROR) {
        (VOS_VOID)memset_s(&hplmn, sizeof(hplmn), 0x00, sizeof(hplmn));
        hplmn.hplmnMncLen = hplmnCnf->eHplmnInfo.hplmnMncLen;

        hplmn.hplmn.mcc = (hplmnCnf->eHplmnInfo.imsi[1] & 0xF0) >> 4;
        hplmn.hplmn.mcc |= (hplmnCnf->eHplmnInfo.imsi[2] & 0x0F) << 8;
        hplmn.hplmn.mcc |= (hplmnCnf->eHplmnInfo.imsi[2] & 0xF0) << 12;

        hplmn.hplmn.mnc = (hplmnCnf->eHplmnInfo.imsi[3] & 0x0F);
        hplmn.hplmn.mnc |= (hplmnCnf->eHplmnInfo.imsi[3] & 0xF0) << 4;
        hplmn.hplmn.mnc |= (hplmnCnf->eHplmnInfo.imsi[4] & 0x0F) << 16;

        AT_QryParaHomePlmnProc(indexNum, hplmnCnf->ctrl.opId, (VOS_UINT8 *)&hplmn);

        return VOS_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, hplmnCnf->errorCause);

        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, result);

        return VOS_ERR;
    }
}


VOS_UINT32 AT_RcvMmaSrvStatusInd(struct MsgCB *msg)
{
    TAF_MMA_SrvStatusInd *srvStatusInd = VOS_NULL_PTR;
    ModemIdUint16         modemId;
    VOS_UINT8             indexNum;
    VOS_UINT16            length;

    /* 初始化消息变量 */
    indexNum     = 0;
    length       = 0;
    srvStatusInd = (TAF_MMA_SrvStatusInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(srvStatusInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSrvStatusInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaSrvStatusInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_SRVST].text,
        srvStatusInd->srvStatus, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaImsiRefreshInd(struct MsgCB *msg)
{
    TAF_MMA_ImsiRefreshInd *imsiRefreshInd = VOS_NULL_PTR;
    ModemIdUint16           modemId;
    VOS_UINT8               indexNum;
    VOS_UINT16              length;

    /* 初始化消息变量 */
    indexNum       = 0;
    length         = 0;
    imsiRefreshInd = (TAF_MMA_ImsiRefreshInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(imsiRefreshInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsiRefreshInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaImsiRefreshInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_IMSI_REFRESH].text,
        g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}



VOS_UINT32 AT_RcvMmaRssiInfoInd(struct MsgCB *msg)
{
    TAF_MMA_RssiInfoInd *rssiInfoInd = VOS_NULL_PTR;
    VOS_UINT8            indexNum;

    /* 初始化消息变量 */
    indexNum    = 0;
    rssiInfoInd = (TAF_MMA_RssiInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rssiInfoInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRssiInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_RcvMmaRssiChangeInd(indexNum, rssiInfoInd);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRegStatusInd(struct MsgCB *msg)
{
    TAF_MMA_RegStatusInd *regStatusInd = VOS_NULL_PTR;
    VOS_UINT8             indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    regStatusInd = (TAF_MMA_RegStatusInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(regStatusInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRegStatusInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_ProcRegStatusInfoInd(indexNum, regStatusInd);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRegRejInfoInd(struct MsgCB *msg)
{
    TAF_MMA_RegRejInfoInd *regRejInd = VOS_NULL_PTR;
    ModemIdUint16          modemId;
    VOS_UINT8              indexNum;
    VOS_UINT16             length;

    /* 初始化消息变量 */
    indexNum  = 0;
    length    = 0;
    regRejInd = (TAF_MMA_RegRejInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(regRejInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRegRejInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaRegRejInfoInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, g_atStringTab[AT_STRING_REJINFO].text);

    /* 打印rejinfo AT命令参数 */
    length = AT_PrintRejinfo(length, &(regRejInd->regRejInfo));

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPlmnSelectInfoInd(struct MsgCB *msg)
{
    TAF_MMA_PlmnSelectionInfoInd *plmnSelecInd = VOS_NULL_PTR;
    ModemIdUint16                 modemId;
    VOS_UINT16                    length;
    VOS_UINT8                     indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    length       = 0;
    plmnSelecInd = (TAF_MMA_PlmnSelectionInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(plmnSelecInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPlmnSelectInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaPlmnSelectInfoInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, g_atStringTab[AT_STRING_PLMNSELEINFO].text);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d%s", plmnSelecInd->plmnSelectInfo.plmnSelectFlag,
        plmnSelecInd->plmnSelectInfo.plmnSelectRlst, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaSyscfgTestCnf(struct MsgCB *msg)
{
    TAF_MMA_SyscfgTestCnf *sysCfgTestCnf = VOS_NULL_PTR;
    VOS_UINT8              indexNum      = 0;
    AT_ModemNetCtx        *netCtx        = VOS_NULL_PTR;
    VOS_CHAR              *pcRoamPara    = VOS_NULL_PTR;
    VOS_CHAR              *pcRatPara     = VOS_NULL_PTR;

    sysCfgTestCnf = (TAF_MMA_SyscfgTestCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(sysCfgTestCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaSyscfgTestCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaSyscfgTestCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (netCtx->roamFeature == AT_ROAM_FEATURE_OFF) {
        pcRoamPara = "(0-2)";
    } else {
        pcRoamPara = "(0-3)";
    }

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_UE_MODE_NR == FEATURE_ON))
    pcRatPara = "(\"01\",\"02\",\"03\",\"04\",\"07\",\"08\")";
#elif (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    pcRatPara = "(\"01\",\"02\",\"03\",\"04\",\"07\")";
#elif (FEATURE_UE_MODE_NR == FEATURE_ON)
    pcRatPara = "(\"01\",\"02\",\"03\",\"08\")";
#else
    pcRatPara = "(\"01\",\"02\",\"03\")";
#endif

    /* SYSCFG */
    sysCfgTestCnf->bandInfo.strSysCfgBandGroup[TAF_PH_SYSCFG_GROUP_BAND_LEN - 1] = '\0';
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_SYSCFG_TEST) {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: (2,13,14,16),(0-3),%s,%s,(0-4)",
                g_parseContext[indexNum].cmdElement->cmdName, sysCfgTestCnf->bandInfo.strSysCfgBandGroup, pcRoamPara);
    }
    /* SYSCFGEX */
    else {
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s,%s,%s,(0-4),((7fffffffffffffff,\"All bands\"))",
            g_parseContext[indexNum].cmdElement->cmdName, pcRatPara, sysCfgTestCnf->bandInfo.strSysCfgBandGroup,
            pcRoamPara);

    }
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaQuickStartQryCnf(struct MsgCB *msg)
{
    TAF_MMA_QuickstartQryCnf *quickStartQryCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum = 0;

    /* 初始化 */
    quickStartQryCnf = (TAF_MMA_QuickstartQryCnf *)msg;
    result           = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(quickStartQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaQuickStartQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaQuickStartQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CQST_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CQST_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 如果查询的接入模式为无效值，则返回ERROR，例如当前LTE only不支持GU */
    if (quickStartQryCnf->errorCause != TAF_ERR_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            quickStartQryCnf->quickStartMode);
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCsnrQryCnf(struct MsgCB *msg)
{
    MTA_AT_CsnrQryCnf *pstrCsnrQryCnf = VOS_NULL_PTR;
    AT_MTA_Msg        *rcvMsg         = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum = 0;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    pstrCsnrQryCnf = (MTA_AT_CsnrQryCnf *)rcvMsg->content;
    result         = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCsnrQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCsnrQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CSNR_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSNR_QUERY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (pstrCsnrQryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            (TAF_INT32)pstrCsnrQryCnf->csnrPara.cpichRscp, (TAF_INT32)pstrCsnrQryCnf->csnrPara.cpichEcNo);
    }

    /* 回复用户命令结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvTafMmaCsqQryCnf(struct MsgCB *msg)
{
    errno_t            memResult;
    TAF_MMA_CsqQryCnf *pstrCsqQryCnf = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum = 0;
    TAF_UINT16         length   = 0;
    TAF_PH_Rssi        csq;

    /* 初始化 */
    pstrCsqQryCnf = (TAF_MMA_CsqQryCnf *)msg;
    result        = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(pstrCsqQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaCsqQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaCsqQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CSQ_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSQ_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    memResult = memcpy_s(&csq, sizeof(csq), &pstrCsqQryCnf->csq, sizeof(TAF_PH_Rssi));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(csq), sizeof(TAF_PH_Rssi));
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (pstrCsqQryCnf->errorCause != TAF_ERR_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
        if (csq.rssiNum > 0) {
            /* 增加范围限制 */
            if ((csq.aRssi[0].rssiValue >= TAF_PH_CSQ_RSSI_HIGH) && (csq.aRssi[0].rssiValue < TAF_PH_BER_UNKNOWN)) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%d", TAF_PH_CSQ_RSSI_HIGH);
            } else {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%d", csq.aRssi[0].rssiValue);
            }
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", csq.aRssi[0].channalQual);
        }
    }

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCsqlvlQryCnf(struct MsgCB *msg)
{
    errno_t              memResult;
    AT_MTA_Msg          *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_CsqlvlQryCnf *pstrCsqlvlQryCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum = 0;
    AT_MTA_CsqlvlPara    csqlvl;
    AT_MTA_CsqlvlextPara csqlvlext;

    /* 初始化 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    pstrCsqlvlQryCnf = (MTA_AT_CsqlvlQryCnf *)rcvMsg->content;
    result           = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCsqlvlQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCsqlvlQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    memResult = memcpy_s(&csqlvlext, sizeof(csqlvlext), &pstrCsqlvlQryCnf->csqLvlExtPara, sizeof(AT_MTA_CsqlvlextPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(csqlvlext), sizeof(AT_MTA_CsqlvlextPara));
    csqlvl = pstrCsqlvlQryCnf->csqLvlPara;

    if (pstrCsqlvlQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CSQLVL_SET) {
            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);

            result = AT_OK;
            /* 根据MMA上报的查询结果显示Csqlvl到串口； */
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                csqlvl.level, csqlvl.rscp);
        } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CSQLVLEXT_SET) {
            /* 复位AT状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);

            result = AT_OK;
            /* 根据MMA上报的查询结果显示Csqlvl到串口； */
            /* 根据MMA上报的查询结果显示CsqlvlExt到串口； */
            g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                csqlvlext.rssilv, csqlvlext.ber);
        } else {
            AT_WARN_LOG("AT_RcvMtaCsqlvlQryCnf : CmdCurrentOpt is invalid.");
            result = AT_ERROR;
        }

    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_TRUE;
}


VOS_UINT32 AT_RcvMmaTimeChangeInd(struct MsgCB *msg)
{
    VOS_UINT8              indexNum;
    TAF_MMA_TimeChangeInd *timeChangeInd = VOS_NULL_PTR;
    TAF_AT_CommTime        mmTimeInfo;
    errno_t                memResult;
    TAF_UINT16             length;

    /* 初始化消息变量 */
    indexNum      = 0;
    timeChangeInd = (TAF_MMA_TimeChangeInd *)msg;
    length        = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(timeChangeInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaTimeChangeInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(&mmTimeInfo, sizeof(mmTimeInfo), 0x00, sizeof(mmTimeInfo));

    mmTimeInfo.dst           = timeChangeInd->dST;
    mmTimeInfo.ieFlg         = timeChangeInd->ieFlg;
    mmTimeInfo.localTimeZone = timeChangeInd->localTimeZone;

    memResult = memcpy_s(mmTimeInfo.curcRptCfg, sizeof(mmTimeInfo.curcRptCfg), timeChangeInd->curcRptCfg,
                         sizeof(timeChangeInd->curcRptCfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(mmTimeInfo.curcRptCfg), sizeof(timeChangeInd->curcRptCfg));

    memResult = memcpy_s(mmTimeInfo.unsolicitedRptCfg, sizeof(mmTimeInfo.unsolicitedRptCfg),
                         timeChangeInd->unsolicitedRptCfg, sizeof(timeChangeInd->unsolicitedRptCfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(mmTimeInfo.unsolicitedRptCfg), sizeof(timeChangeInd->unsolicitedRptCfg));

    mmTimeInfo.universalTimeandLocalTimeZone.year     = timeChangeInd->universalTimeandLocalTimeZone.year;
    mmTimeInfo.universalTimeandLocalTimeZone.month    = timeChangeInd->universalTimeandLocalTimeZone.month;
    mmTimeInfo.universalTimeandLocalTimeZone.day      = timeChangeInd->universalTimeandLocalTimeZone.day;
    mmTimeInfo.universalTimeandLocalTimeZone.hour     = timeChangeInd->universalTimeandLocalTimeZone.hour;
    mmTimeInfo.universalTimeandLocalTimeZone.minute   = timeChangeInd->universalTimeandLocalTimeZone.minute;
    mmTimeInfo.universalTimeandLocalTimeZone.second   = timeChangeInd->universalTimeandLocalTimeZone.second;
    mmTimeInfo.universalTimeandLocalTimeZone.timeZone = timeChangeInd->universalTimeandLocalTimeZone.timeZone;

    if (((timeChangeInd->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) ||
        ((timeChangeInd->ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ)) {
        /* 存在时间信息 */

        length += (TAF_UINT16)At_PrintMmTimeInfo(indexNum, &mmTimeInfo, (g_atSndCodeAddress + length));
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaModeChangeInd(struct MsgCB *msg)
{
    VOS_UINT16             length;
    ModemIdUint16          modemId;
    VOS_UINT32             rslt;
    VOS_UINT8              indexNum = 0;
    TAF_MMA_ModeChangeInd *rcvMsg   = VOS_NULL_PTR;
    rcvMsg                          = (TAF_MMA_ModeChangeInd *)msg;
    length                          = 0;
    modemId                         = MODEM_ID_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaCsqlvlextQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaModeChangeInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_MODE].text,
        rcvMsg->ratType, rcvMsg->sysSubMode, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    /* 此处全局变量维护不再准确，需要删除，用时从C核获取 */

    return VOS_TRUE;
}


VOS_UINT32 AT_RcvMmaPlmnChangeInd(struct MsgCB *msg)
{
    VOS_UINT16             length;
    ModemIdUint16          modemId;
    VOS_UINT32             rslt;
    VOS_UINT8              indexNum = 0;
    TAF_MMA_PlmnChangeInd *rcvMsg   = VOS_NULL_PTR;

    length  = 0;
    rcvMsg  = (TAF_MMA_PlmnChangeInd *)msg;
    modemId = MODEM_ID_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaCsqlvlextQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_RcvMmaPlmnChangeIndProc: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^PLMN:", g_atCrLf);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x,", (rcvMsg->curPlmn.mcc & 0x0f00) >> 8,
        (rcvMsg->curPlmn.mcc & 0xf0) >> 4, (rcvMsg->curPlmn.mcc & 0x0f));

    if ((rcvMsg->curPlmn.mnc & 0x0f00) == 0x0f00) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x", (rcvMsg->curPlmn.mnc & 0xf0) >> 4,
            (rcvMsg->curPlmn.mnc & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x", (rcvMsg->curPlmn.mnc & 0x0f00) >> 8,
            (rcvMsg->curPlmn.mnc & 0xf0) >> 4, (rcvMsg->curPlmn.mnc & 0x0f));
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData((VOS_UINT8)indexNum, g_atSndCodeAddress, length);
    return VOS_TRUE;
}


VOS_UINT32 AT_RcvTafMmaCrpnQryCnf(struct MsgCB *msg)
{
    errno_t              memResult;
    TAF_MMA_CrpnQryCnf  *crpnQryCnf      = VOS_NULL_PTR;
    TAF_MMA_CrpnQryInfo *mnMmaCrpnQryCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum = 0;
    VOS_UINT16           length;
    VOS_UINT32           loop;
    VOS_UINT8            plmn[TAF_MAX_AUTHDATA_PLMN_LEN + 1];
    TAF_MMA_CrpnQryPara  mnMmaCrpnQry;

    /* 初始化 */
    crpnQryCnf      = (TAF_MMA_CrpnQryCnf *)msg;
    mnMmaCrpnQryCnf = &crpnQryCnf->crpnQryInfo;
    result          = AT_OK;
    length          = 0;
    loop            = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(crpnQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafMmaCrpnQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafMmaCrpnQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRPN_QUERY) {
        return VOS_ERR;
    }

    /* 格式化AT^CRPN查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (crpnQryCnf->errorCause != TAF_ERR_NO_ERROR) {
        result = At_ChgTafErrorCode(indexNum, (VOS_UINT16)crpnQryCnf->errorCause);
        ;

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);

        /* 输出结果 */
        At_FormatResultData(indexNum, result);

        return VOS_OK;
    }

    result = AT_OK;

    mnMmaCrpnQryCnf->totalNum = AT_MIN(mnMmaCrpnQryCnf->totalNum, TAF_PH_CRPN_PLMN_MAX_NUM);
    for (loop = 0; loop < mnMmaCrpnQryCnf->totalNum; loop++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        /* ^CRPN:  */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* <PLMN> */
        /* 如果命令中输入的是PLMN ID，则输出时直接采用输入的PLMN ID */
        if (mnMmaCrpnQryCnf->mnMmaCrpnQry.plmnType == AT_PLMN_TYPE_NUM) {
            (VOS_VOID)memset_s(plmn, sizeof(plmn), 0x00, sizeof(plmn));
            if (mnMmaCrpnQryCnf->mnMmaCrpnQry.plmnNameLen > 0) {
                memResult = memcpy_s(plmn, sizeof(plmn), mnMmaCrpnQryCnf->mnMmaCrpnQry.plmnName,
                                     TAF_MIN(TAF_MAX_AUTHDATA_PLMN_LEN, mnMmaCrpnQryCnf->mnMmaCrpnQry.plmnNameLen));
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(plmn),
                                    TAF_MIN(TAF_MAX_AUTHDATA_PLMN_LEN, mnMmaCrpnQryCnf->mnMmaCrpnQry.plmnNameLen));
            }
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\"", plmn);
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%X%X%X",
                (0x0f00 & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mcc) >> 8,
                (0x00f0 & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mcc) >> 4,
                (0x000f & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mcc));

            if (mnMmaCrpnQryCnf->maxMncLen == AT_MAX_MNC_LEN) {
                length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%X",
                    (0x0f00 & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mnc) >> 8);
            }
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X\"",
                (0x00f0 & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mnc) >> 4,
                (0x000f & mnMmaCrpnQryCnf->operNameList[loop].plmnId.mnc));
        }

        /* <short name> */
        mnMmaCrpnQryCnf->operNameList[loop].operatorNameShort[TAF_PH_OPER_NAME_SHORT - 1] = '\0';
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", mnMmaCrpnQryCnf->operNameList[loop].operatorNameShort);

        /* <long name> */
        mnMmaCrpnQryCnf->operNameList[loop].operatorNameLong[TAF_PH_OPER_NAME_LONG - 1] = '\0';
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", mnMmaCrpnQryCnf->operNameList[loop].operatorNameLong);
    }

    g_atSendDataBuff.bufLen = length;

    At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
    length = 0;

    /* 如果本次上报的CRPN数目与要求的相同，则认为C核仍有Plmn list没有上报，要继续发送请求进行查询 */
    if (mnMmaCrpnQryCnf->totalNum == TAF_PH_CRPN_PLMN_MAX_NUM) {
        memResult = memcpy_s(&mnMmaCrpnQry, sizeof(mnMmaCrpnQry), &mnMmaCrpnQryCnf->mnMmaCrpnQry,
                             sizeof(TAF_MMA_CrpnQryPara));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(mnMmaCrpnQry), sizeof(TAF_MMA_CrpnQryPara));
        mnMmaCrpnQry.currIndex = mnMmaCrpnQryCnf->currIndex + mnMmaCrpnQryCnf->totalNum;
        mnMmaCrpnQry.qryNum    = TAF_PH_CRPN_PLMN_MAX_NUM;

        /* 发送ID_TAF_MMA_CRPN_QRY_REQ消息给MMA处理 */
        if (TAF_MMA_QryCrpnReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &mnMmaCrpnQry) == VOS_TRUE) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CRPN_QUERY;
        } else {
            /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
            AT_STOP_TIMER_CMD_READY(indexNum);
            g_atSendDataBuff.bufLen = 0;
            At_FormatResultData(indexNum, AT_ERROR);
        }
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        At_BufferorSendResultData(indexNum, g_atSndCodeAddress, length);
        length = 0;

        /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCbcQryCnf(struct MsgCB *msg)
{
    TAF_MMA_BatteryCapacityQryCnf *cbcCnf = VOS_NULL_PTR;
    VOS_UINT32                     result;
    VOS_UINT8                      indexNum = 0;
    TAF_UINT16                     length   = 0;

    /* 初始化 */
    cbcCnf = (TAF_MMA_BatteryCapacityQryCnf *)msg;
    result = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cbcCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCbcQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCbcQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CBC_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBC_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        cbcCnf->batteryStatus.batteryPowerStatus);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", cbcCnf->batteryStatus.batteryRemains);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaHsQryCnf(struct MsgCB *msg)
{
    TAF_MMA_HandShakeQryCnf *hsCnf = VOS_NULL_PTR;
    VOS_UINT32               id;
    VOS_UINT32               result;
    TAF_UINT16               length   = 0;
    VOS_UINT8                indexNum = 0;

    /* 初始化 */
    hsCnf  = (TAF_MMA_HandShakeQryCnf *)msg;
    result = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(hsCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaHsQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaHsQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_HS_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HS_READ) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    id = TAF_GET_HOST_UINT32(hsCnf->buf);

    /* id */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, id);
    /* protocol */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", hsCnf->buf[AT_BUF_ELEM_PROROCOL]);
    /* is_offline */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", hsCnf->buf[AT_BUF_ELEM_UE_CUR_CFUN_MODE]);
    /* product_class */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", hsCnf->buf[AT_BUF_ELEM_PRODUCT_CLASS]);
    /* product_id */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", hsCnf->buf[AT_BUF_ELEM_PRODUCT_ID]);

    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvMmaHdrCsqInd(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    MMA_TAF_HdrCsqValueInd *hdrcsqInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum  = 0;
    hdrcsqInd = (MMA_TAF_HdrCsqValueInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(hdrcsqInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCdmaCsqInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^HDRCSQ: %d,%d,%d%s", g_atCrLf, hdrcsqInd->hdrRssi, hdrcsqInd->hdrSnr,
        hdrcsqInd->hdrEcio, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaHdrCsqSetCnf(struct MsgCB *msg)
{
    MMA_TAF_HdrCsqSetCnf *hdrCsqCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    hdrCsqCnf = (MMA_TAF_HdrCsqSetCnf *)msg;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(hdrCsqCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaHdrCsqSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaHdrCsqSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (hdrCsqCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaHdrCsqQryCnf(struct MsgCB *msg)
{
    MMA_TAF_HdrCsqQrySettingCnf *hdrCsqQueryCnf = VOS_NULL_PTR;
    VOS_UINT32                   result;
    VOS_UINT16                   length;
    VOS_UINT8                    indexNum = 0;

    /* 初始化 */
    hdrCsqQueryCnf = (MMA_TAF_HdrCsqQrySettingCnf *)msg;
    length         = 0;
    result         = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(hdrCsqQueryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaHdrCsqQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaHdrCsqQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_HDR_CSQ_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HDR_CSQ_QRY) {
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d,%d,%d",
        g_parseContext[indexNum].cmdElement->cmdName, hdrCsqQueryCnf->hdrCsq.mode, hdrCsqQueryCnf->hdrCsq.timeInterval,
        hdrCsqQueryCnf->hdrCsq.rssiThreshold, hdrCsqQueryCnf->hdrCsq.snrThreshold,
        hdrCsqQueryCnf->hdrCsq.ecioThreshold, hdrCsqQueryCnf->hdrRssi, hdrCsqQueryCnf->hdrSnr,
        hdrCsqQueryCnf->hdrEcio);

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    /* 输出结果 */
    return VOS_OK;
}
#endif

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_RcvMmaImsSwitchSetCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsSwitchSetCnf *cnfMsg = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    cnfMsg   = (TAF_MMA_ImsSwitchSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cnfMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsSwitchSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsSwitchSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型 */
    if (cnfMsg->bitOpImsSwitch == VOS_TRUE) {
        if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMS_SWITCH_SET) {
            AT_WARN_LOG("AT_RcvMmaImsSwitchSetCnf: WARNING:Not AT_CMD_IMS_SWITCH_SET!");
            return VOS_ERR;
        }
    } else if (cnfMsg->bitOpRcsSwitch == VOS_TRUE) {
        if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RCSSWITCH_SET) {
            AT_WARN_LOG("AT_RcvMmaImsSwitchSetCnf: WARNING:Not AT_CMD_RCSSWITCH_SET!");
            return VOS_ERR;
        }
    } else {
        AT_WARN_LOG("AT_RcvMmaImsSwitchSetCnf: WARNING:option id error!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnfMsg->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cnfMsg->result);
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaImsSwitchQryCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsSwitchQryCnf *switchQryCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum     = 0;
    VOS_UINT32               result;

    /* 初始化 */
    switchQryCnf = (TAF_MMA_ImsSwitchQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(switchQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsSwitchQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsSwitchQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMS_SWITCH_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMS_SWITCH_QRY) {
        AT_WARN_LOG("AT_RcvMmaImsSwitchQryCnf: WARNING:Not AT_CMD_IMS_SWITCH_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (switchQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                switchQryCnf->imsSwitchRat.lteEnable, switchQryCnf->imsSwitchRat.utranEnable,
                switchQryCnf->imsSwitchRat.gsmEnable);
        result = AT_OK;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = At_ChgTafErrorCode(indexNum, switchQryCnf->result);
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaVoiceDomainSetCnf(struct MsgCB *msg)
{
    TAF_MMA_VoiceDomainSetCnf *cnfMsg = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;
    VOS_UINT32                 result;

    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    cnfMsg   = (TAF_MMA_VoiceDomainSetCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cnfMsg->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VOICE_DOMAIN_SET) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainSetCnf: WARNING:Not AT_CMD_VOICE_DOMAIN_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnfMsg->result == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cnfMsg->errorCause);
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_VoiceDomainTransToOutputValue(TAF_MMA_VoiceDomainUint32 voiceDoman, VOS_UINT32 *value)
{
    VOS_UINT32 rst;

    rst = VOS_TRUE;

    switch (voiceDoman) {
        case TAF_MMA_VOICE_DOMAIN_CS_ONLY:
            *value = AT_VOICE_DOMAIN_TYPE_CS_ONLY;
            break;

        case TAF_MMA_VOICE_DOMAIN_IMS_PS_ONLY:
            *value = AT_VOICE_DOMAIN_TYPE_IMS_PS_ONLY;
            break;

        case TAF_MMA_VOICE_DOMAIN_CS_PREFERRED:
            *value = AT_VOICE_DOMAIN_TYPE_CS_PREFERRED;
            break;

        case TAF_MMA_VOICE_DOMAIN_IMS_PS_PREFERRED:
            *value = AT_VOICE_DOMAIN_TYPE_IMS_PS_PREFERRED;
            break;

        default:
            *value = AT_VOICE_DOMAIN_TYPE_BUTT;
            rst    = VOS_FALSE;
            break;
    }

    return rst;
}


VOS_UINT32 AT_RcvMmaVoiceDomainQryCnf(struct MsgCB *msg)
{
    TAF_MMA_VoiceDomainQryCnf *domainQryCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum     = 0;
    VOS_UINT32                 value;
    VOS_UINT32                 result;

    /* 初始化 */
    value        = 0;
    domainQryCnf = (TAF_MMA_VoiceDomainQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(domainQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_VOICE_DOMAIN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VOICE_DOMAIN_QRY) {
        AT_WARN_LOG("AT_RcvMmaVoiceDomainQryCnf: WARNING:Not AT_CMD_VOICE_DOMAIN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (AT_VoiceDomainTransToOutputValue(domainQryCnf->voiceDomain, &value) != VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = 0;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, value);
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaImsDomainCfgSetCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsDomainCfgSetCnf *domainCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum = 0;

    domainCfgSetCnf = (TAF_MMA_ImsDomainCfgSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(domainCfgSetCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgSetCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgSetCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSDOMAIN_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSDOMAINCFG_SET) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgSetCnf : WARNING:Not AT_CMD_IMSDOMAINCFG_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (domainCfgSetCnf->imsDomainCfgResult == TAF_MMA_IMS_DOMAIN_CFG_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaImsDomainCfgQryCnf(struct MsgCB *msg)
{
    TAF_MMA_ImsDomainCfgQryCnf *domainCfgQryCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum        = 0;

    domainCfgQryCnf = (TAF_MMA_ImsDomainCfgQryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(domainCfgQryCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgQryCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgQryCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_IMSDOMAINCFG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_IMSDOMAINCFG_QRY) {
        AT_WARN_LOG("AT_RcvMmaImsDomainCfgQryCnf : WARNING:Not AT_CMD_IMSDOMAIN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            domainCfgQryCnf->imsDomainCfgType);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRoamImsSupportSetCnf(struct MsgCB *msg)
{
    TAF_MMA_RoamImsSupportSetCnf *roamImsSupportSetCnf = VOS_NULL_PTR;
    VOS_UINT32                    result;
    VOS_UINT8                     indexNum = 0;

    roamImsSupportSetCnf = (TAF_MMA_RoamImsSupportSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(roamImsSupportSetCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRoamImsSupportSetCnf :WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRoamImsSupportSetCnf : AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_ROAM_IMS_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ROAM_IMS_SET) {
        AT_WARN_LOG("AT_RcvMmaRoamImsSupportSetCnf : WARNING:Not AT_CMD_ROAM_IMS_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (roamImsSupportSetCnf->roamImsSupportResult == TAF_MMA_ROAM_IMS_SUPPORT_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvMmaCLImsCfgInfoInd(struct MsgCB *msg)
{
    TAF_MMA_ClimsCfgInfoInd *clImsCfgInfo = VOS_NULL_PTR;
    VOS_UINT8                indexNum;

    indexNum     = 0;
    clImsCfgInfo = (TAF_MMA_ClimsCfgInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(clImsCfgInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCLImsCfgInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CLIMSCFG: %d,%d,%d%s", g_atCrLf, clImsCfgInfo->clImsSupportFlag,
        clImsCfgInfo->clVolteMode, clImsCfgInfo->lteImsSupportFlag, g_atCrLf);

    /* 调用At_SendResultData发送命令结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}
#endif

#endif


VOS_UINT32 AT_RcvMtaSetFemctrlCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetFemctrlCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;
    VOS_UINT32            result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetFemctrlCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetFemctrlCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetFemctrlCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FEMCTRL_SET) {
        AT_WARN_LOG("AT_RcvMtaSetFemctrlCnf : Current Option is not AT_CMD_FEMCTRL_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvMma1xChanSetCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum;
    TAF_MMA_CfreqLockSetCnf *cFreqLockSetCnf = VOS_NULL_PTR;
    VOS_UINT32               result;

    /* 初始化消息变量 */
    indexNum        = 0;
    cFreqLockSetCnf = (TAF_MMA_CfreqLockSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cFreqLockSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMma1xChanSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMma1xChanSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_1XCHAN_SET) {
        AT_WARN_LOG("AT_RcvMma1xChanSetCnf : Current Option is not AT_CMD_CFREQLOCK_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^1XCHAN命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (cFreqLockSetCnf->rslt != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMma1xChanQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_1XchanQueryCnf *pst1xChanQueryCnf = VOS_NULL_PTR;
    VOS_UINT16              length;
    VOS_UINT8               indexNum = 0;
    VOS_INT16               invalidChan;

    /* 初始化 */
    pst1xChanQueryCnf = (TAF_MMA_1XchanQueryCnf *)msg;
    length            = 0;
    invalidChan       = -1;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(pst1xChanQueryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMma1xChanQueryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMma1xChanQueryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CVER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_1XCHAN_QRY) {
        AT_WARN_LOG("AT_RcvMma1xChanQueryCnf : Current Option is not AT_CMD_1XCHAN_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (pst1xChanQueryCnf->rslt == VOS_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            pst1xChanQueryCnf->channel);

        g_atSendDataBuff.bufLen = length;
    } else {
        /* 查询失败，上报^1XCHAN: BUTT */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            invalidChan);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCVerQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_CverQueryCnf  *cVerQueryCnf = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;
    TAF_MMA_1XCasPRevUint8 invalidVersion;

    /* 初始化 */
    cVerQueryCnf   = (TAF_MMA_CverQueryCnf *)msg;
    length         = 0;
    invalidVersion = TAF_MMA_1X_CAS_P_REV_ENUM_BUTT;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cVerQueryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCVerQueryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCVerQueryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CVER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CVER_QRY) {
        AT_WARN_LOG("AT_RcvMmaCVerQueryCnf : Current Option is not AT_CMD_CVER_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cVerQueryCnf->rslt == VOS_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            cVerQueryCnf->prevInUse);

        g_atSendDataBuff.bufLen = length;
    } else {
        /* 查询失败，上报^CVER: BUTT */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            invalidVersion);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaStateQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_StateQueryCnf *stQryCnf = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;
    VOS_INT8               invalidSta;

    /* 初始化 */
    stQryCnf   = (TAF_MMA_StateQueryCnf *)msg;
    length     = 0;
    invalidSta = -1;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(stQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaStateQueryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaStateQueryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CVER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GETSTA_QRY) {
        AT_WARN_LOG("AT_RcvMmaStateQueryCnf : Current Option is not AT_CMD_GETST_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (stQryCnf->rslt == VOS_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            stQryCnf->handsetSta.casState, stQryCnf->handsetSta.casSubSta);

        g_atSendDataBuff.bufLen = length;
    } else {
        /* 查询失败，上报^GETSTA: BUTT */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            invalidSta, invalidSta);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCHverQueryCnf(struct MsgCB *msg)
{
    TAF_MMA_ChighverQueryCnf *staQryCnf = VOS_NULL_PTR;
    VOS_UINT16                length;
    VOS_UINT8                 indexNum = 0;
    TAF_MMA_1XCasPRevUint8    invalidVer;

    /* 初始化 */
    staQryCnf  = (TAF_MMA_ChighverQueryCnf *)msg;
    length     = 0;
    invalidVer = TAF_MMA_1X_CAS_P_REV_ENUM_BUTT;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(staQryCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCHverQueryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCHverQueryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CHIGHVER_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CHIGHVER_QRY) {
        AT_WARN_LOG("AT_RcvMmaCHverQueryCnf : Current Option is not AT_CMD_CHIGHVER_QRY.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (staQryCnf->rslt == VOS_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            staQryCnf->highRev);

        g_atSendDataBuff.bufLen = length;
    } else {
        /* 查询失败，上报^CHIGHVER: BUTT */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            invalidVer);

        g_atSendDataBuff.bufLen = length;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#endif

VOS_UINT32 AT_RcvMmaHplmnQryCnf(VOS_UINT8 indexNum, struct MsgCB *msg)
{
    TAF_MMA_HomePlmnQryCnf *hplmnQryCnf = VOS_NULL_PTR;
    VOS_UINT16              length;
    VOS_UINT32              i;
    VOS_UINT8               imsiString[NAS_IMSI_STR_LEN + 1];

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(imsiString, sizeof(imsiString), 0x00, sizeof(imsiString));
    hplmnQryCnf = (TAF_MMA_HomePlmnQryCnf *)msg;
    length      = 0;

    /* index的异常判断移到AT_RcvMmaEHplmnInfoQryCnf中 */

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if ((hplmnQryCnf->eHplmnInfo.eHplmnNum == 0) || (hplmnQryCnf->errorCause != TAF_ERR_NO_ERROR)) {
        AT_WARN_LOG("AT_RcvMmaHplmnQryCnf : EHPLMN num error.");

        /* 查询命令失败，上报错误情况 */
        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, At_ChgTafErrorCode(indexNum, hplmnQryCnf->errorCause));

        return VOS_ERR;
    }

    /* 将IMSI数字转换为字符串 */
    AT_ConvertImsiDigit2String(hplmnQryCnf->eHplmnInfo.imsi, imsiString, sizeof(imsiString));

    /* 查询命令成功，上报AT^HPLMN */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %s,%d,%d,", g_parseContext[indexNum].cmdElement->cmdName,
        imsiString, hplmnQryCnf->eHplmnInfo.hplmnMncLen, hplmnQryCnf->eHplmnInfo.eHplmnNum);

    for (i = 0; i < (VOS_UINT8)(hplmnQryCnf->eHplmnInfo.eHplmnNum - 1); i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x",
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0x0f00) >> 8,
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0xf0) >> 4,
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0x0f));

        if ((hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f00) == 0x0f00) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x,",
                (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0xf0) >> 4,
                (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f));
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x,",
                (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f00) >> 8,
                (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0xf0) >> 4,
                (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f));
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x", (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0x0f00) >> 8,
        (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0xf0) >> 4, (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mcc & 0x0f));

    if ((hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f00) == 0x0f00) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x", (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0xf0) >> 4,
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x",
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f00) >> 8,
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0xf0) >> 4,
            (hplmnQryCnf->eHplmnInfo.eHplmnList[i].mnc & 0x0f));
    }

    g_atSendDataBuff.bufLen = length;

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_HPLMN;
    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);
    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaDplmnQryCnf(struct MsgCB *msg)
{
    errno_t              memResult;
    TAF_MMA_DplmnQryCnf *dplmnQryCnf = VOS_NULL_PTR;
    VOS_UINT16           length;
    ModemIdUint16        modemId;
    VOS_UINT8            indexNum = 0;
    VOS_UINT8            tempVersion[NAS_VERSION_LEN + 1];

    /* 局部变量初始化 */
    dplmnQryCnf = (TAF_MMA_DplmnQryCnf *)msg;
    length      = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(dplmnQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDplmnQryCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaDplmnQryCnf: Get modem id fail.");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaDplmnQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DPLMNLIST_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPLMNLIST_QRY) {
        AT_WARN_LOG("AT_RcvMmaDplmnQryCnf : Current Option is not AT_CMD_DPLMNLIST_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 将version数字转换为字符串 */
    memResult = memcpy_s(tempVersion, sizeof(tempVersion), dplmnQryCnf->versionId, NAS_VERSION_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempVersion), NAS_VERSION_LEN);

    /* 加字符串结束符 */
    tempVersion[NAS_VERSION_LEN] = '\0';

    /* 查询命令成功，上报^DPLMN版本号 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, tempVersion);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaDplmnSetCnf(struct MsgCB *msg)
{
    TAF_MMA_DplmnSetCnf *dplmnSetCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum    = 0;
    ModemIdUint16        modemId;
    VOS_UINT32           result;
    VOS_UINT32           retVal;

    /* 局部变量初始化 */
    dplmnSetCnf = (TAF_MMA_DplmnSetCnf *)msg;
    retVal      = VOS_ERR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(dplmnSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDplmnSetCnf At_ClientIdToUserId FAILURE");
        return retVal;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaDplmnSetCnf: Get modem id fail.");
        return retVal;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaDplmnSetCnf: AT_BROADCAST_INDEX.");
        return retVal;
    }

    /* 判断当前操作类型是否为AT_CMD_DPLMNLIST_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DPLMNLIST_SET) {
        AT_WARN_LOG("AT_RcvMmaDplmnSetCnf : Current Option is not AT_CMD_DPLMNLIST_SET.");
        return retVal;
    }

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (dplmnSetCnf->rslt == VOS_OK) {
        result = AT_OK;
        retVal = VOS_OK;
    } else {
        result = AT_CME_UNKNOWN;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return retVal;
}


VOS_UINT32 AT_RcvMmaExchangeModemInfoCnf(struct MsgCB *msg)
{
    TAF_MMA_ExchangeModemInfoCnf *exchangeNvimCnf = VOS_NULL_PTR;
    VOS_UINT8                     indexNum;
    VOS_UINT32                    result;
    VOS_UINT32                    retVal;

    /* 局部变量初始化 */
    exchangeNvimCnf = (TAF_MMA_ExchangeModemInfoCnf *)msg;
    retVal          = VOS_ERR;
    indexNum        = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(exchangeNvimCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaExchangeModemInfoCnf: At_ClientIdToUserId FAILURE");
        return retVal;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaExchangeModemInfoCnf: AT_BROADCAST_INDEX.");
        return retVal;
    }

    /* 判断当前操作类型是否为AT_CMD_EXCHANGE_NVIM_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EXCHANGE_MODEM_INFO_SET) {
        AT_WARN_LOG("AT_RcvMmaExchangeModemInfoCnf: Current Option is not AT_CMD_EXCHANGE_MODEM_INFO_SET.");
        return retVal;
    }

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (exchangeNvimCnf->rslt == VOS_OK) {
        result = AT_OK;
        retVal = VOS_OK;
    } else {
        result = AT_CME_UNKNOWN;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return retVal;
}


VOS_UINT32 AT_RcvMmaBorderInfoSetCnf(struct MsgCB *msg)
{
    TAF_MMA_BorderInfoSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT32                retVal;
    ModemIdUint16             modemId;
    VOS_UINT8                 indexNum;

    /* 局部变量初始化 */
    setCnf   = (TAF_MMA_BorderInfoSetCnf *)msg;
    retVal   = VOS_ERR;
    modemId  = MODEM_ID_BUTT;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(setCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoSetCnf At_ClientIdToUserId FAILURE");
        return retVal;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaBorderInfoSetCnf: Get modem id fail.");
        return retVal;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoSetCnf: AT_BROADCAST_INDEX.");
        return retVal;
    }

    /* 判断当前操作类型是否为AT_CMD_BORDERINFO_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BORDERINFO_SET) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoSetCnf : Current Option is not AT_CMD_BORDERINFO_SET.");
        return retVal;
    }

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setCnf->rslt == VOS_OK) {
        result = AT_OK;
        retVal = VOS_OK;
    } else {
        result = AT_CME_UNKNOWN;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return retVal;
}


VOS_UINT32 AT_RcvMmaBorderInfoQryCnf(struct MsgCB *msg)
{
    errno_t                   memResult;
    TAF_MMA_BorderInfoQryCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT8                 tempVersion[NAS_VERSION_LEN + 1];
    VOS_UINT16                length;
    ModemIdUint16             modemId;
    VOS_UINT8                 indexNum;

    /* 局部变量初始化 */
    qryCnf   = (TAF_MMA_BorderInfoQryCnf *)msg;
    length   = 0;
    modemId  = MODEM_ID_BUTT;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(qryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoQryCnf At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaBorderInfoQryCnf: Get modem id fail.");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_BORDERINFO_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BORDERINFO_QRY) {
        AT_WARN_LOG("AT_RcvMmaBorderInfoQryCnf : Current Option is not AT_CMD_BORDERINFO_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 将version数字转换为字符串 */
    memResult = memcpy_s(tempVersion, sizeof(tempVersion), qryCnf->versionId, NAS_VERSION_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempVersion), NAS_VERSION_LEN);

    /* 加字符串结束符 */
    tempVersion[NAS_VERSION_LEN] = '\0';

    /* 查询命令成功，上报^BORDERINFO版本号 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, tempVersion);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 AT_RcvMtaSetFrCnf(struct MsgCB *msg)
{
    AT_MTA_Msg      *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_SetFrCnf *setFrCnf = VOS_NULL_PTR;
    VOS_UINT8        indexNum;
    VOS_UINT32       result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setFrCnf = (MTA_AT_SetFrCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetFrCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetFrCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FRSTATUS_SET) {
        AT_WARN_LOG("AT_RcvMtaSetFrCnf : Current Option is not AT_CMD_FRSTATUS_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setFrCnf->rslt == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSib16TimeUpdateInd(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_Sib16TimeUpdate *sib16TimeUpdate = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT16              length;

    /* 初始化消息变量 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    sib16TimeUpdate = (MTA_AT_Sib16TimeUpdate *)rcvMsg->content;
    indexNum        = 0;
    length          = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSib16TimeUpdateInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    switch (sib16TimeUpdate->rptOptType) {
        case MTA_AT_TIME_INFO_RPT_OPT_MMINFO:
            length += (VOS_UINT16)At_PrintMmTimeInfo(indexNum, &(sib16TimeUpdate->commTimeInfo),
                                                     (g_atSndCodeAddress + length));
            break;

        case MTA_AT_TIME_INFO_RPT_OPT_SIB16TIME:
            AT_WARN_LOG("AT_RcvMtaSib16TimeUpdateInd: Not support SIB16Time Report!");
            return VOS_ERR;

        default:
            break;
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


/*lint -save -e838 -specific(-e838)*/
/*lint -save -e826 -specific(-e826)*/
VOS_UINT32 AT_RcvMtaAccessStratumRelInd(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg              = VOS_NULL_PTR;
    MTA_AT_AccessStratumRelInd *accessStratumRelInd = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;

    /* 初始化消息变量 */
    rcvMsg              = (AT_MTA_Msg *)msg;
    accessStratumRelInd = (MTA_AT_AccessStratumRelInd *)rcvMsg->content;
    indexNum            = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaAccessStratumRelInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    switch (accessStratumRelInd->accessStratumRel) {
        case MTA_AT_ACCESS_STRATUM_REL8:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL8;
            break;

        case MTA_AT_ACCESS_STRATUM_REL9:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL9;
            break;

        case MTA_AT_ACCESS_STRATUM_REL10:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL10;
            break;

        case MTA_AT_ACCESS_STRATUM_REL11:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL11;
            break;

        case MTA_AT_ACCESS_STRATUM_REL12:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL12;
            break;

        case MTA_AT_ACCESS_STRATUM_REL13:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL13;
            break;
        case MTA_AT_ACCESS_STRATUM_REL14:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL14;
            break;
        case MTA_AT_ACCESS_STRATUM_REL_SPARE1:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL_SPARE1;
            break;

        default:
            g_releaseInfo.accessStratumRel = AT_ACCESS_STRATUM_REL8;
            AT_NORM_LOG("AT_RcvMtaAccessStratumRelInd: Wrong Release Number!");
            break;
    }

    return VOS_OK;
}
/*lint -restore*/
/*lint -restore*/

/*lint -save -e838 -specific(-e838)*/
/*lint -save -e826 -specific(-e826)*/

VOS_UINT32 AT_RcvMtaRsInfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg       = VOS_NULL_PTR;
    MTA_AT_RsInfoQryCnf *rsInfoQryCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;

    /* 初始化 */
    rcvMsg       = (AT_MTA_Msg *)msg;
    rsInfoQryCnf = (MTA_AT_RsInfoQryCnf *)rcvMsg->content;
    indexNum     = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRsInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaRsInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待+RSRP/+RSRQ命令返回 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RSRP_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RSRQ_QRY)) {
        AT_WARN_LOG("AT_RcvMtaRsInfoQryCnf : Current Option is not AT_CMD_RSRP_QRY or AT_CMD_RSRQ_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (rsInfoQryCnf->rsInfoRslt.rsInfoType == AT_MTA_RSRP_TYPE) {
        AT_FormatRsrp(indexNum, rsInfoQryCnf);
    } else {
        AT_FormatRsrq(indexNum, rsInfoQryCnf);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}
/*lint -restore*/
/*lint -restore*/


/*lint -save -e838 -specific(-e838)*/
/*lint -save -e826 -specific(-e826)*/
VOS_UINT32 AT_RcvMtaEccStatusInd(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg    = VOS_NULL_PTR;
    MTA_AT_EccStatusInd *eccStatus = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT16           length;

    /* 初始化消息变量 */
    rcvMsg    = (AT_MTA_Msg *)msg;
    eccStatus = (MTA_AT_EccStatusInd *)rcvMsg->content;
    indexNum  = 0;
    length    = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEccStatusInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* "^ECCSTATUS: */
    length +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d%s",
            g_atCrLf, g_atStringTab[AT_STRING_ECC_STATUS].text, eccStatus->validflag, eccStatus->rxAntNum,
            eccStatus->tmMode, eccStatus->corrQR1Data0011, eccStatus->corrQR1Data01Iq, eccStatus->corrQR2Data0011,
            eccStatus->corrQR2Data01Iq, eccStatus->corrQR3Data0011, eccStatus->corrQR3Data01Iq, eccStatus->rsrpR0,
            eccStatus->rsrpR1, eccStatus->rsrpR2, eccStatus->rsrpR3, eccStatus->rssiR0, eccStatus->rssiR1,
            eccStatus->rssiR2, eccStatus->rssiR3, g_atCrLf);

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEccCfgCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                  *rcvMsg       = VOS_NULL_PTR;
    MTA_AT_SET_ECC_CFG_CNF_STRU *eccCfgSetCnf = VOS_NULL_PTR;
    VOS_UINT32                   result;
    VOS_UINT8                    indexNum;

    /* 初始化 */
    rcvMsg       = (AT_MTA_Msg *)msg;
    eccCfgSetCnf = (MTA_AT_SET_ECC_CFG_CNF_STRU *)rcvMsg->content;
    indexNum     = 0;
    result       = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEccCfgCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaEccCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECCCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaEccCfgCnf : Current Option is not AT_CMD_ECCCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (eccCfgSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
/*lint -restore*/
/*lint -restore*/

/*lint -save -e838 -specific(-e838)*/
/*lint -save -e826 -specific(-e826)*/

VOS_UINT32 AT_RcvMtaErrcCapCfgCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_ErrccapCfgCnf *errcCapCfgCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg        = (AT_MTA_Msg *)msg;
    errcCapCfgCnf = (MTA_AT_ErrccapCfgCnf *)rcvMsg->content;
    indexNum      = 0;
    result        = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaErrcCapCfgCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaErrcCapCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待AT^ERRCCAPCFG命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ERRCCAPCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaErrcCapCfgCnf : Current Option is not AT_CMD_ERRCCAPCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (errcCapCfgCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaErrcCapQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_ErrccapQryCnf *errcCapQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg        = (AT_MTA_Msg *)msg;
    errcCapQryCnf = (MTA_AT_ErrccapQryCnf *)rcvMsg->content;
    indexNum      = 0;
    result        = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaErrcCapQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaErrcCapCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待^ERRCCAPQRY命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ERRCCAPQRY_SET) {
        AT_WARN_LOG("AT_RcvMtaErrcCapCfgCnf : Current Option is not AT_CMD_ERRCCAPQRY_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (errcCapQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* ^ERRCCAPQRY:  */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "^ERRCCAPQRY: %u,%u,%u,%u",
            errcCapQryCnf->errcCapType, errcCapQryCnf->para1, errcCapQryCnf->para2, errcCapQryCnf->para3);
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
/*lint -restore*/
/*lint -restore*/
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)


VOS_UINT32 AT_RcvMtaLendcQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg         *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_LendcQryCnf *lendcQryCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexNum;

    /* 初始化 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    lendcQryCnf = (MTA_AT_LendcQryCnf *)rcvMsg->content;
    indexNum    = 0;
    result      = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLendcQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLendcQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待^LENDC命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LENDC_QRY) {
        AT_WARN_LOG("AT_RcvMtaLendcQryCnf : Current Option is not AT_CMD_LENDC_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (lendcQryCnf->rslt == MTA_AT_RESULT_NO_ERROR) {
        /* ^LEND: <enable>,<endc_available>,<endc_plmn_avail>,<endc_restricted>,<nr_pscell> */
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                lendcQryCnf->reportFlag, lendcQryCnf->lendcInfo.endcAvaliableFlag,
                lendcQryCnf->lendcInfo.endcPlmnAvaliableFlag, lendcQryCnf->lendcInfo.endcEpcRestrictedFlag,
                lendcQryCnf->lendcInfo.nrPscellFlag);

        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(lendcQryCnf->rslt);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLendcInfoInd(struct MsgCB *msg)
{
    VOS_UINT8            indexNum;
    AT_MTA_Msg          *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_LendcInfoInd *lendcInfoInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum     = 0;
    mtaMsg       = (AT_MTA_Msg *)msg;
    lendcInfoInd = (MTA_AT_LendcInfoInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLendcInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* ^LEND: <endc_available>,<endc_plmn_avail>,<endc_restricted>,<nr_pscell> */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^LENDC: %d,%d,%d,%d%s", g_atCrLf, lendcInfoInd->lendcInfo.endcAvaliableFlag,
        lendcInfoInd->lendcInfo.endcPlmnAvaliableFlag, lendcInfoInd->lendcInfo.endcEpcRestrictedFlag,
        lendcInfoInd->lendcInfo.nrPscellFlag, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_RcvMtaNrFreqLockSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                        *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_NRRC_FREQLOCK_SET_CNF_STRU *nrrcFreqlockCnf = VOS_NULL_PTR;
    VOS_UINT32                         result;
    VOS_UINT8                          indexNum;

    /* 初始化 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    nrrcFreqlockCnf = (MTA_AT_NRRC_FREQLOCK_SET_CNF_STRU *)rcvMsg->content;
    result          = AT_OK;
    indexNum        = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRFREQLOCK_SET) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockSetCnf : Current Option is not AT_CMD_NRFREQLOCK_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^NRFREQLOCK命令返回 */
    g_atSendDataBuff.bufLen = 0;

    result = AT_ConvertMtaResult(nrrcFreqlockCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrFreqLockQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_NrrcFreqlockQryCnf *nrrcFreqlockCnf = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;

    /* 初始化 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    nrrcFreqlockCnf = (MTA_AT_NrrcFreqlockQryCnf *)rcvMsg->content;
    result          = AT_OK;
    indexNum        = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRFREQLOCK_QRY) {
        AT_WARN_LOG("AT_RcvMtaNrFreqLockQryCnf : Current Option is not AT_CMD_NRFREQLOCK_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^NRFREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (nrrcFreqlockCnf->result != VOS_OK) {
        result = AT_ConvertMtaResult(nrrcFreqlockCnf->result);
    } else {
        result = AT_OK;

        switch (nrrcFreqlockCnf->freqType) {
            case MTA_AT_FREQLOCK_TYPE_LOCK_NONE:
                g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                    (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress, "%s: %d",
                    g_parseContext[indexNum].cmdElement->cmdName, nrrcFreqlockCnf->freqType);
                break;

            case MTA_AT_FREQLOCK_TYPE_LOCK_FREQ:
                g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                    (TAF_CHAR *)g_atSndCodeAddress, (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%u",
                    g_parseContext[indexNum].cmdElement->cmdName, nrrcFreqlockCnf->freqType, nrrcFreqlockCnf->scsType,
                    nrrcFreqlockCnf->band, nrrcFreqlockCnf->nrArfcn);
                break;

            case MTA_AT_FREQLOCK_TYPE_LOCK_CELL:
                g_atSendDataBuff.bufLen =
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%u,%d",
                        g_parseContext[indexNum].cmdElement->cmdName, nrrcFreqlockCnf->freqType,
                        nrrcFreqlockCnf->scsType, nrrcFreqlockCnf->band, nrrcFreqlockCnf->nrArfcn,
                        nrrcFreqlockCnf->phyCellId);
                break;

            case MTA_AT_FREQLOCK_TYPE_LOCK_BAND:
                g_atSendDataBuff.bufLen =
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                        nrrcFreqlockCnf->freqType, nrrcFreqlockCnf->scsType, nrrcFreqlockCnf->band);
                break;

            default:
                AT_WARN_LOG("AT_RcvMtaNrFreqLockQryCnf : Current req type is out of bounds.");
                return VOS_ERR;
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif

#endif


VOS_UINT32 AT_RcvMtaCommBoosterInd(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_CommBoosterInd *mtaAtCommBooster = VOS_NULL_PTR;
    VOS_UINT32             i                = 0;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    rcvMsg           = (AT_MTA_Msg *)msg;
    mtaAtCommBooster = (MTA_AT_CommBoosterInd *)rcvMsg->content;
    indexNum         = 0;
    length           = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (mtaAtCommBooster->len > MTA_AT_BOOSTER_IND_MAX_NUM) {
        mtaAtCommBooster->len = MTA_AT_BOOSTER_IND_MAX_NUM;
    }

    /* \r\n^BOOSTERNTF */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s", g_atCrLf, g_atStringTab[AT_STRING_BOOSTERNTF].text);

    /* module indexNum, len */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d, %d,\"", mtaAtCommBooster->pidIndex, mtaAtCommBooster->len);

    for (i = 0; i < mtaAtCommBooster->len; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mtaAtCommBooster->boosterIndData[i]);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCommBoosterSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg                 = VOS_NULL_PTR;
    MTA_AT_CommBoosterSetCnf *mtaAtSetCommBoosterCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg                 = (AT_MTA_Msg *)msg;
    mtaAtSetCommBoosterCnf = (MTA_AT_CommBoosterSetCnf *)rcvMsg->content;
    result                 = AT_OK;
    indexNum               = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COMM_BOOSTER_SET) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterSetCnf : Current Option is not AT_CMD_COMM_BOOSTER_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if ((mtaAtSetCommBoosterCnf->internalResultFlag == VOS_FALSE) &&
        (mtaAtSetCommBoosterCnf->externalResultFlag == VOS_FALSE)) {
        result = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, 0);
    } else if (mtaAtSetCommBoosterCnf->internalResultFlag == VOS_TRUE) {
        result = AT_ERROR;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaAtSetCommBoosterCnf->result);
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCommBoosterQueryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg                   = VOS_NULL_PTR;
    MTA_AT_CommBoosterQueryCnf *mtaAtQueryCommBoosterCnf = VOS_NULL_PTR;
    VOS_UINT32                  result;
    VOS_UINT8                   indexNum;
    VOS_UINT16                  length;
    VOS_UINT32                  i;

    /* 初始化 */
    rcvMsg                   = (AT_MTA_Msg *)msg;
    mtaAtQueryCommBoosterCnf = (MTA_AT_CommBoosterQueryCnf *)rcvMsg->content;
    result                   = AT_OK;
    indexNum                 = 0;
    length                   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterQueryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterQueryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_COMM_BOOSTER_QRY) {
        AT_WARN_LOG("AT_RcvMtaCommBoosterQueryCnf : Current Option is not AT_CMD_COMM_BOOSTER_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (mtaAtQueryCommBoosterCnf->result == VOS_FALSE) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        /* \r\n^COMMBOOSTER */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: \"", g_parseContext[indexNum].cmdElement->cmdName);

        if (mtaAtQueryCommBoosterCnf->len > MTA_AT_BOOSTER_QRY_CNF_MAX_NUM) {
            mtaAtQueryCommBoosterCnf->len = MTA_AT_BOOSTER_QRY_CNF_MAX_NUM;
        }

        for (i = 0; (i < mtaAtQueryCommBoosterCnf->len); i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", mtaAtQueryCommBoosterCnf->date[i]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
        g_atSendDataBuff.bufLen = length;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvGameModeSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_GameModeSetCfn *setCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_GameModeSetCfn *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvGameModeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvGameModeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GAME_MODE_SET) {
        AT_WARN_LOG("AT_RcvGameModeSetCnf : Current Option is not AT_CMD_SAMPLE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_RcvCdmaModemSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CdmaModemCapSetCnf *setCnf = VOS_NULL_PTR;
    AT_MTA_ModemCapUpdateReq   atMtaModemCapUpdate;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;
    VOS_UINT8                  isCdmaModemSwitchNotResetFlg;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_CdmaModemCapSetCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvCdmaModemSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvCdmaModemSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CDMAMODEMSWITCH_SET) {
        AT_WARN_LOG("AT_RcvCdmaModemSetCnf : Current Option is not AT_CMD_CDMAMODEMSWITCH_SET.");
        return VOS_ERR;
    }

    AT_NORM_LOG1("AT_RcvCdmaModemSetCnf : return Result ", setCnf->result);

    if (setCnf->result == MTA_AT_RESULT_ERROR) {
        AT_WARN_LOG1("AT_RcvCdmaModemSetCnf : MTA Return Error. ucResult", setCnf->result);
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, AT_CME_WRITE_NV_TimeOut);
        return VOS_OK;
    } else if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        AT_WARN_LOG1("AT_RcvCdmaModemSetCnf : MTA Return Error. ucResult", setCnf->result);
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    } else {
        result = AT_OK;
    }
    AT_ReadPlatformNV();

    /*  at 命令入口处已经检查，到了此处，必定时modem0 */
    isCdmaModemSwitchNotResetFlg = AT_GetModemCdmaModemSwitchCtxAddrFromModemId(MODEM_ID_0)->enableFlg;

    if (isCdmaModemSwitchNotResetFlg == VOS_FALSE) {
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, result);

        return VOS_OK;
    }

    (VOS_VOID)memset_s(&atMtaModemCapUpdate, (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq), 0x00,
                       (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq));

    /* 更新了平台能力，通知AT->MTA->RRM，进行底层平台能力更新 */
    /* AT发送至MTA的消息结构赋值 */
    atMtaModemCapUpdate.modemCapUpdateType = AT_MTA_MODEM_CAP_UPDATE_TYPE_CDMA_MODEM_SWITCH;

    /* 发送消息给C核处理 */
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_MODEM_CAP_UPDATE_REQ,
                               (VOS_UINT8 *)&atMtaModemCapUpdate, (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq),
                               I0_UEPS_PID_MTA) != AT_SUCCESS) {
        AT_WARN_LOG("AT_RcvCdmaModemSetCnf: AT_FillAndSndAppReqMsg Fail!");
        /* 复位AT状态 */
        AT_STOP_TIMER_CMD_READY(indexNum);
        g_atSendDataBuff.bufLen = 0;

        At_FormatResultData(indexNum, result);
        return VOS_OK;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MODEM_CAP_UPDATE_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvCdmaCapResumeSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CdmaCapResumeSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_CdmaCapResumeSetCnf *)rcvMsg->content;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvCdmaCapResumeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvCdmaCapResumeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CDMACAPRESUME_SET) {
        AT_WARN_LOG("AT_RcvCdmaCapResumeSetCnf : Current Option is not AT_CMD_CDMACAPRESUME_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    AT_NORM_LOG1("AT_RcvCdmaCapResumeSetCnf : return Result ", setCnf->result);

    /*  平台能力恢复失败，后续流行不再处理，at命令返回失败 */
    if (setCnf->result != MTA_AT_RESULT_NO_ERROR) {
        AT_WARN_LOG1("AT_RcvCdmaCapResumeSetCnf : MTA Return Error. ucResult", setCnf->result);

        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#endif


VOS_UINT32 AT_RcvMtaNvLoadSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetNvloadCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetNvloadCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNvLoadSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNvLoadSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVLOAD_SET) {
        AT_WARN_LOG("AT_RcvMtaNvLoadSetCnf : Current Option is not AT_CMD_NVLOAD_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, setCnf->result);
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvMtaPseucellInfoSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *rcvMsg             = VOS_NULL_PTR;
    MTA_AT_PseucellInfoSetCnf *pseucellInfoSetCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum;
    VOS_UINT32                 result;

    /* 初始化 */
    rcvMsg             = (AT_MTA_Msg *)msg;
    pseucellInfoSetCnf = (MTA_AT_PseucellInfoSetCnf *)rcvMsg->content;
    indexNum           = 0;
    result             = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPseucellInfoSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaPseucellInfoSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PSEUCELL_SET) {
        AT_WARN_LOG("AT_RcvMtaPseucellInfoSetCnf : Current Option is not AT_CMD_PSEUCELL_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (pseucellInfoSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaChrAlarmRlatCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                   *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_ChrAlarmRlatCfgSetCnf *chrAlarmSetCnf = VOS_NULL_PTR;
    VOS_UINT32                    result;
    VOS_UINT16                    length;
    VOS_UINT8                     indexNum;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)msg;
    chrAlarmSetCnf = (MTA_AT_ChrAlarmRlatCfgSetCnf *)rcvMsg->content;
    indexNum       = 0;
    result         = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaChrAlarmRlatCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaChrAlarmRlatCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CHRALARMRLATCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaChrAlarmRlatCfgSetCnf : Current Option is not AT_CMD_CHRALARMRLATCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (chrAlarmSetCnf->result == MTA_AT_RESULT_NO_ERROR) {
        if (chrAlarmSetCnf->alarmOp == AT_MTA_CHR_ALARM_RLAT_OP_READ) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                chrAlarmSetCnf->alarmDetail);

            g_atSendDataBuff.bufLen = length;
        }

        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaClearHistoryFreqCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8         indexNum;
    VOS_UINT32        result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaClearHistoryFreqCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaClearHistoryFreqCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLEAR_HISTORY_FREQ) {
        AT_WARN_LOG("AT_RcvMtaClearHistoryFreqCnf : Current Option is not AT_CMD_CLEAR_HISTORY_FREQ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



LOCAL AT_COPS_RatUint8 At_ConvertRatParaToCopsRat(TAF_MMA_RatTypeUint8 rat)
{
    AT_COPS_RatUint8    rslt;
    switch (rat) {
        case TAF_MMA_RAT_GSM: {
            rslt = AT_COPS_RAT_GSM;
            break;
        }
        case TAF_MMA_RAT_WCDMA: {
            rslt = AT_COPS_RAT_WCDMA;
            break;
        }
        case TAF_MMA_RAT_LTE: {
            rslt = AT_COPS_RAT_LTE;
            break;
        }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case TAF_MMA_RAT_NR: {
            rslt = AT_COPS_RAT_NR;
            break;
        }
#endif
        default: {
            rslt = AT_COPS_RAT_BUTT;
            break;
        }
    }

    return rslt;
}


LOCAL VOS_VOID AT_ConvertPlmnId2Bcd(TAF_PLMN_Id *plmnId)
{
    VOS_UINT32 i;
    VOS_UINT32 tmp;
    VOS_UINT8  tmpLocal[4];

    tmp          = plmnId->mcc;
    plmnId->mcc = 0;
    for (i = 0; i < 3; i++) {
        tmpLocal[i] = tmp & 0x0f;
        tmp >>= 8;
    }
    plmnId->mcc = ((VOS_UINT32)tmpLocal[0] << 8) | ((VOS_UINT32)tmpLocal[1] << 4) | tmpLocal[2];

    tmp = plmnId->mnc;
    plmnId->mnc = 0;
    for (i = 0; i < 3; i++) {
        tmpLocal[i] = tmp & 0x0f;
        tmp >>= 8;
    }

    if (tmpLocal[2] == 0xf) {
        plmnId->mnc = ((VOS_UINT32)tmpLocal[0] << 4) | ((VOS_UINT32)tmpLocal[1]) | 0xf00;
    } else {
        plmnId->mnc = ((VOS_UINT32)tmpLocal[0] << 8) | ((VOS_UINT32)tmpLocal[1] << 4) | tmpLocal[2];
    }
    return;
}


VOS_UINT32 AT_RcvMmaSrchedPlmnInfoInd(struct MsgCB *msg)
{
    TAF_MMA_SrchedPlmnInfoInd *srchedPlmnInfo = VOS_NULL_PTR;
    VOS_UINT16                 length;
    VOS_UINT8                  indexNum = 0;
    VOS_UINT32                 i;
    ModemIdUint16              modemId;
    VOS_UINT32                 rslt;
    VOS_UINT32                 plmnNum;
    AT_COPS_RatUint8           rat;

    length         = 0;
    modemId        = MODEM_ID_0;
    srchedPlmnInfo = (TAF_MMA_SrchedPlmnInfoInd *)msg;

    if (At_ClientIdToUserId(srchedPlmnInfo->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSrchedPlmnInfoInd At_ClientIdToUserId FAILURE");
        return VOS_ERR;
    }

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaSrchedPlmnInfoInd: Get modem id fail.");
        return VOS_ERR;
    }

    plmnNum = AT_MIN(srchedPlmnInfo->plmnNum, TAF_MMA_MAX_SRCHED_LAI_NUM);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                 (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s ", g_atCrLf,
                                 g_atStringTab[AT_STRING_SRCHEDPLMNLIST].text);

    for (i = 0; i < plmnNum; i++) {
        srchedPlmnInfo->plmnName[i].operatorNameLong[TAF_PH_OPER_NAME_LONG - 1] = '\0';
        srchedPlmnInfo->plmnName[i].operatorNameShort[TAF_PH_OPER_NAME_SHORT - 1] = '\0';

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                         (TAF_CHAR *)g_atSndCodeAddress + length, "(%u,\"%s\",\"%s\"",
                                         srchedPlmnInfo->plmnStatus[i], srchedPlmnInfo->plmnName[i].operatorNameLong,
                                         srchedPlmnInfo->plmnName[i].operatorNameShort);

        /* 将PLMN ID转为BCD格式 */
        AT_ConvertPlmnId2Bcd(&(srchedPlmnInfo->plmnName[i].plmnId));

        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                         (TAF_CHAR *)g_atSndCodeAddress + length, ",\"%X%X%X",
                                         (0x0f00 & srchedPlmnInfo->plmnName[i].plmnId.mcc) >> 8,
                                         (0x00f0 & srchedPlmnInfo->plmnName[i].plmnId.mcc) >> 4,
                                         (0x000f & srchedPlmnInfo->plmnName[i].plmnId.mcc));

        if ((srchedPlmnInfo->plmnName[i].plmnId.mnc & 0x0f00) == 0x0f00) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                             (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x\",",
                                             (srchedPlmnInfo->plmnName[i].plmnId.mnc & 0xf0) >> 4,
                                             (srchedPlmnInfo->plmnName[i].plmnId.mnc & 0x0f));
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                             (TAF_CHAR *)g_atSndCodeAddress + length, "%x%x%x\",",
                                             (srchedPlmnInfo->plmnName[i].plmnId.mnc & 0x0f00) >> 8,
                                             (srchedPlmnInfo->plmnName[i].plmnId.mnc & 0xf0) >> 4,
                                             (srchedPlmnInfo->plmnName[i].plmnId.mnc & 0x0f));
        }

        rat = At_ConvertRatParaToCopsRat(srchedPlmnInfo->laiWithRscp[i].rat);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, "%u,%u,%d)", rat,
                                             srchedPlmnInfo->laiWithRscp[i].lac, srchedPlmnInfo->laiWithRscp[i].rscp);

        if (i < plmnNum - 1) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                             (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                                     (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaTransModeQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg               = VOS_NULL_PTR;
    MTA_AT_TransmodeQryCnf *mtaAtQryTransModeCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化 */
    rcvMsg               = (AT_MTA_Msg *)msg;
    mtaAtQryTransModeCnf = (MTA_AT_TransmodeQryCnf *)rcvMsg->content;
    result               = AT_OK;
    indexNum             = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaTransModeQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaTransModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TRANSMODE_READ) {
        AT_WARN_LOG("AT_RcvMtaTransModeQryCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (mtaAtQryTransModeCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaAtQryTransModeCnf->transMode);
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaUECenterQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg              = VOS_NULL_PTR;
    MTA_AT_QryUeCenterCnf *mtaAtQryUECenterCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg              = (AT_MTA_Msg *)msg;
    mtaAtQryUECenterCnf = (MTA_AT_QryUeCenterCnf *)rcvMsg->content;
    result              = AT_OK;
    indexNum            = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_UE_CENTER_QRY) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (mtaAtQryUECenterCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaAtQryUECenterCnf->ueCenter);
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaUECenterSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg              = VOS_NULL_PTR;
    MTA_AT_SetUeCenterCnf *mtaAtSetUECenterCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg              = (AT_MTA_Msg *)msg;
    mtaAtSetUECenterCnf = (MTA_AT_SetUeCenterCnf *)rcvMsg->content;
    result              = AT_OK;
    indexNum            = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_UE_CENTER_SET) {
        AT_WARN_LOG("AT_RcvMtaUECenterQryCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (mtaAtSetUECenterCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQuickCardSwitchSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                   *rcvMsg             = VOS_NULL_PTR;
    MTA_AT_SetQuickCardSwitchCnf *quickCardSwitchCnf = VOS_NULL_PTR;
    VOS_UINT32                    result;
    VOS_UINT8                     indexNum;

    /* 初始化 */
    rcvMsg             = (AT_MTA_Msg *)msg;
    quickCardSwitchCnf = (MTA_AT_SetQuickCardSwitchCnf *)rcvMsg->content;
    result             = AT_OK;
    indexNum           = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQuickCardSwitchSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQuickCardSwitchSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_QUICK_CARD_SWITCH_SET) {
        AT_WARN_LOG("AT_RcvMtaQuickCardSwitchSetCnf : Current Option is not AT_CMD_RRC_VERSION_QUERY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (quickCardSwitchCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}



VOS_UINT32 AT_RcvMmaInitLocInfoInd(struct MsgCB *msg)
{
    VOS_UINT8               indexNum;
    TAF_MMA_InitLocInfoInd *initLocInfoInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum       = 0;
    initLocInfoInd = (TAF_MMA_InitLocInfoInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(initLocInfoInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaInitLocInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^INITLOCINFO: %x%x%x,%d%s", g_atCrLf, (initLocInfoInd->mcc & 0x0f00) >> 8,
        (initLocInfoInd->mcc & 0x00f0) >> 4, (initLocInfoInd->mcc & 0x000f), initLocInfoInd->sid, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEflociInfoSetCnf(struct MsgCB *msg)
{
    TAF_MMA_EflociinfoSetCnf *eflociInfoSetCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum         = 0;
    VOS_UINT32                result;

    eflociInfoSetCnf = (TAF_MMA_EflociinfoSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(eflociInfoSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EFLOCIINFO_SET) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoSetCnf : Current Option is not AT_CMD_EFLOCIINFO_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (eflociInfoSetCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaEflociInfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_EflociinfoQryCnf *efloiinfoQryCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum        = 0;
    VOS_UINT32                result;
    VOS_UINT16                length;

    efloiinfoQryCnf = (TAF_MMA_EflociinfoQryCnf *)msg;
    length          = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(efloiinfoQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EFLOCIINFO_QRY) {
        AT_WARN_LOG("AT_RcvMmaEflociInfoQryCnf : Current Option is not AT_CMD_EFLOCIINFO_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 查询失败直接返回ERROR */
    if (efloiinfoQryCnf->rslt != TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_ERROR;

        At_FormatResultData(indexNum, result);
        return VOS_OK;
    }

    /* <tmsi> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: 0x%08x", g_parseContext[indexNum].cmdElement->cmdName,
        (VOS_UINT32)efloiinfoQryCnf->eflociInfo.tmsi);

    /* <plmn> */
    if (efloiinfoQryCnf->eflociInfo.plmnId.mcc == 0xfff && efloiinfoQryCnf->eflociInfo.plmnId.mnc == 0xfff) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"ffffff\"");
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%x%x%x",
            (0x0f00 & efloiinfoQryCnf->eflociInfo.plmnId.mcc) >> 8,
            (0x00f0 & efloiinfoQryCnf->eflociInfo.plmnId.mcc) >> 4, (0x000f & efloiinfoQryCnf->eflociInfo.plmnId.mcc));

        if (((0x0f00 & efloiinfoQryCnf->eflociInfo.plmnId.mnc) >> 8) != 0x0F) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%x", (0x0f00 & efloiinfoQryCnf->eflociInfo.plmnId.mnc) >> 8);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%x%x\"", (0x00f0 & efloiinfoQryCnf->eflociInfo.plmnId.mnc) >> 4,
            (0x000f & efloiinfoQryCnf->eflociInfo.plmnId.mnc));
    }
    /* <lac> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%04x", efloiinfoQryCnf->eflociInfo.lac);

    /* <location_update_status> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", efloiinfoQryCnf->eflociInfo.locationUpdateStatus);

    /* <rfu> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%02x", efloiinfoQryCnf->eflociInfo.rfu);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPsEflociInfoSetCnf(struct MsgCB *msg)
{
    TAF_MMA_EfpslociinfoSetCnf *psEflociInfoSetCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum           = 0;
    VOS_UINT32                  result;

    psEflociInfoSetCnf = (TAF_MMA_EfpslociinfoSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(psEflociInfoSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EFPSLOCIINFO_SET) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoSetCnf : Current Option is not AT_CMD_EFPSLOCIINFO_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (psEflociInfoSetCnf->rslt == TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

#if (FEATURE_LTEV == FEATURE_ON)

VOS_UINT32 AT_RcvMtaVModeSetCnf(struct MsgCB *msg)
{
    TAF_MTA_VMODE_SetCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;
    rcvMsg                         = (TAF_MTA_VMODE_SetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaVModeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaVModeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VMODE_SET) {
        AT_WARN_LOG("AT_RcvMtaVModeSetCnf : Current Option is not AT_CMD_VMODE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VMODE设置命令返回 */
    if (rcvMsg->result != TAF_MTA_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvMtaVModeQryCnf(struct MsgCB *msg)
{
    TAF_MTA_VMODE_QryCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    /* 初始化 */
    rcvMsg = (TAF_MTA_VMODE_QryCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaVModeQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaVModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VMODE_QUERY) {
        AT_WARN_LOG("AT_RcvMtaVModeQryCnf : Current Option is not AT_CMD_VMODE_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^VMODE查询命令返回 */
    g_atSendDataBuff.bufLen = 0;
    if (rcvMsg->result != TAF_MTA_OK) {
        result = AT_ERROR;
    } else {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "^VMODE: %d", rcvMsg->mode);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_VOID AT_ProcMsgFromVnas(struct MsgCB *msg)
{
    VNAS_ReqMsg *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32   i;
    VOS_UINT32   msgCnt;
    VOS_UINT32   msgName;
    VOS_UINT32   rst;

    /* 从g_atProcMsgFromVnasTbl中获取消息个数 */
    msgCnt = sizeof(g_atProcMsgFromVnasTbl) / sizeof(AT_MSG_Proc);
    rcvMsg = (VNAS_ReqMsg *)msg;

    /* 从消息包中获取MSG Name */
    msgName = rcvMsg->msgName;

    /* g_astAtProcMsgFromVnaspTab查表，进行消息分发 */
    for (i = 0; i < msgCnt; i++) {
        if (g_atProcMsgFromVnasTbl[i].msgType == msgName) {
            rst = g_atProcMsgFromVnasTbl[i].procMsgFunc((struct MsgCB *)rcvMsg);

            if (rst == VOS_ERR) {
                AT_ERR_LOG("AT_ProcMsgFromVnas: Msg Proc Err!");
            }

            return;
        }
    }

    /* 没有找到匹配的消息 */
    if (msgCnt == i) {
        AT_ERR_LOG("AT_ProcMsgFromVnas: Msg Id is invalid!");
    }

    return;
}


VOS_UINT32 AT_RcvVnasSrcidSetCnf(struct MsgCB *msg)
{
    VNAS_ReqMsg             *rcvMsg         = VOS_NULL_PTR;
    VNAS_LtevSourceIdSetCnf *setSourceIdCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                tblIndex = 0;

    /* 初始化 */
    rcvMsg         = (VNAS_ReqMsg *)msg;
    setSourceIdCnf = (VNAS_LtevSourceIdSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->clientId, &tblIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVnasSrcidSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(tblIndex)) {
        AT_WARN_LOG("AT_RcvVnasSrcidSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[tblIndex].cmdCurrentOpt != AT_CMD_SRCID_SET) {
        AT_WARN_LOG("AT_RcvVnasSrcidSetCnf : Current Option is not AT_CMD_VMODE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(tblIndex);

    /* 格式化AT^SRCID设置命令返回 */
    if (setSourceIdCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(tblIndex, result);

    return VOS_OK;
}

VOS_UINT32 AT_RcvVnasSrcidQryCnf(struct MsgCB *msg)
{
    VNAS_ReqMsg             *rcvMsg         = VOS_NULL_PTR;
    VNAS_LtevSourceIdQryCnf *qrySourceIdCnf = VOS_NULL_PTR;
    VOS_UINT8                tblIndex       = 0;

    /* 初始化 */
    rcvMsg         = (VNAS_ReqMsg *)msg;
    qrySourceIdCnf = (VNAS_LtevSourceIdQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->clientId, &tblIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVnasSrcidQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(tblIndex)) {
        AT_WARN_LOG("AT_RcvVnasSrcidQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[tblIndex].cmdCurrentOpt != AT_CMD_SRCID_QRY) {
        AT_WARN_LOG("AT_RcvVnasSrcidQryCnf : Current Option is not AT_CMD_SRCID_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(tblIndex);

    /* 格式化AT^SRCID查询命令返回 */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "^SRCID: %d,%d", qrySourceIdCnf->sourceid, qrySourceIdCnf->sourceIdOrign);

    /* 输出结果 */
    At_FormatResultData(tblIndex, AT_OK);

    return VOS_OK;
}
#endif

#endif

VOS_UINT32 AT_RcvMmaPsEflociInfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_EfpslociinfoQryCnf *psefloiinfoQryCnf = VOS_NULL_PTR;
    VOS_UINT8                   indexNum          = 0;
    VOS_UINT32                  result;
    VOS_UINT16                  length;

    psefloiinfoQryCnf = (TAF_MMA_EfpslociinfoQryCnf *)msg;
    length            = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(psefloiinfoQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EFPSLOCIINFO_QRY) {
        AT_WARN_LOG("AT_RcvMmaPsEflociInfoQryCnf : Current Option is not AT_CMD_EFPSLOCIINFO_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 查询失败直接返回ERROR */
    if (psefloiinfoQryCnf->rslt != TAF_MMA_APP_OPER_RESULT_SUCCESS) {
        result = AT_ERROR;

        At_FormatResultData(indexNum, result);
        return VOS_OK;
    }

    /* <ptmsi> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: 0x%08x", g_parseContext[indexNum].cmdElement->cmdName,
        psefloiinfoQryCnf->psEflociInfo.pTmsi);

    /* <ptmsi_signature> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%6x", psefloiinfoQryCnf->psEflociInfo.pTmsiSignature);

    /* <plmn> */
    if (psefloiinfoQryCnf->psEflociInfo.plmnId.mcc == 0xfff && psefloiinfoQryCnf->psEflociInfo.plmnId.mnc == 0xfff) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"ffffff\"");
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%x%x%x",
            (0x0f00 & psefloiinfoQryCnf->psEflociInfo.plmnId.mcc) >> 8,
            (0x00f0 & psefloiinfoQryCnf->psEflociInfo.plmnId.mcc) >> 4,
            (0x000f & psefloiinfoQryCnf->psEflociInfo.plmnId.mcc));

        if (((0x0f00 & psefloiinfoQryCnf->psEflociInfo.plmnId.mnc) >> 8) != 0x0F) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%x",
                (0x0f00 & psefloiinfoQryCnf->psEflociInfo.plmnId.mnc) >> 8);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%x%x\"",
            (0x00f0 & psefloiinfoQryCnf->psEflociInfo.plmnId.mnc) >> 4,
            (0x000f & psefloiinfoQryCnf->psEflociInfo.plmnId.mnc));
    }
    /* <lac> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%04x", psefloiinfoQryCnf->psEflociInfo.lac);

    /* <rac> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%02x", psefloiinfoQryCnf->psEflociInfo.rac);

    /* <location_update_status> */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", psefloiinfoQryCnf->psEflociInfo.psLocationUpdateStatus);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}

#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)

VOS_UINT32 AT_RcvMtaSetM2MFreqLockCnf(struct MsgCB *pmsg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetM2MFreqlockCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)pmsg;
    setCnf   = (MTA_AT_SetM2MFreqlockCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        return AT_ERROR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        return AT_ERROR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_M2M_FREQLOCK_SET) {
        return AT_ERROR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryM2MFreqLockCnf(struct MsgCB *pmsg)
{
    AT_MTA_Msg               *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_QryM2MFreqlockCnf *qryFreqLockCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;
    VOS_UINT16                length;

    /* 初始化 */
    rcvMsg         = (AT_MTA_Msg *)pmsg;
    qryFreqLockCnf = (MTA_AT_QryM2MFreqlockCnf *)rcvMsg->content;
    result         = AT_OK;
    indexNum       = 0;
    length         = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryM2MFreqLockCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryM2MFreqLockCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_M2M_FREQLOCK_QRY) {
        AT_WARN_LOG("AT_RcvMtaQryFreqLockCnf : Current Option is not AT_CMD_M2M_FREQLOCK_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^FREQLOCK查询命令返回 */
    g_atSendDataBuff.bufLen = 0;

    /* 锁频查询结果按G-W-T-L顺序依次输出  */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 输出GSM制式锁频状态信息 */
    if (qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_GSM - 1] == AT_MTA_M2M_FREQLOCK_FLAG_TYPE_OFF) {
        /* <enable>,<mode> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\"",
            qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_GSM - 1],
            g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_GSM - 1].strMode);
    } else {
        /* <enable>,<freq>,<mode>,<band>,, */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,\"%s\",,",
            qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_GSM - 1],
            g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_GSM - 1].strMode, qryFreqLockCnf->gFreqLockInfo.freq,
            g_freqLockGsmBandTbl[qryFreqLockCnf->gFreqLockInfo.band].strGsmBand);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* 输出WCDMA制式锁频状态信息 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_WCDMA - 1] == AT_MTA_M2M_FREQLOCK_FLAG_TYPE_OFF) {
        /* <enable>,<mode> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\"",
            qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_WCDMA - 1],
            g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_WCDMA - 1].strMode);
    } else {
        /* 判断WCDMA锁频的类型:区分锁频、锁频+扰码组合 */
        if (qryFreqLockCnf->wFreqLockInfo.freqType == AT_MTA_M2M_FREQLOCK_TYPE_ENUM_FREQ_ONLY) {
            /* <enable>,<mode>,<freq>,,, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,,",
                qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_WCDMA - 1],
                g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_WCDMA - 1].strMode, qryFreqLockCnf->wFreqLockInfo.freq);
        } else {
            /* <enable>,<mode>,<freq>,,<psc>, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,%d,",
                qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_WCDMA - 1],
                g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_WCDMA - 1].strMode, qryFreqLockCnf->wFreqLockInfo.freq,
                qryFreqLockCnf->wFreqLockInfo.psc);
        }
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* 输出TDS-CDMA制式锁频状态信息 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1] == AT_MTA_M2M_FREQLOCK_FLAG_TYPE_OFF) {
        /* <enable>,<mode> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\"",
            qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1],
            g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1].strMode);
    } else {
        /* 判断WCDMA锁频的类型:区分锁频、锁频+扰码组合 */
        if (qryFreqLockCnf->tFreqLockInfo.freqType == AT_MTA_M2M_FREQLOCK_TYPE_ENUM_FREQ_ONLY) {
            /* <enable>,<mode>,<freq>,,, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,,",
                qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1],
                g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1].strMode, qryFreqLockCnf->tFreqLockInfo.freq);
        } else {
            /* <enable>,<mode>,<freq>,,<psc>, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,%d,",
                qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1],
                g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_TDSCDMA - 1].strMode, qryFreqLockCnf->tFreqLockInfo.freq,
                qryFreqLockCnf->tFreqLockInfo.sc);
        }
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    /* 输出LTE制式锁频状态信息 */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    if (qryFreqLockCnf->freqState[AT_MTA_M2M_FREQLOCK_MODE_LTE - 1] == AT_MTA_M2M_FREQLOCK_FLAG_TYPE_OFF) {
        /* <enable>,<mode> */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\"",
            qryFreqLockCnf->freqState[AT_MTA_M2M_FREQLOCK_MODE_LTE - 1],
            g_freqLockModeTbl[AT_MTA_M2M_FREQLOCK_MODE_LTE - 1].strMode);
    } else {
        /* 判断LTE锁频的类型:区分锁频、锁频+扰码组合 */
        if (qryFreqLockCnf->lFreqLockInfo.freqType == AT_MTA_M2M_FREQLOCK_TYPE_ENUM_FREQ_ONLY) {
            /* <enable>,<mode>,<freq>,,, */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,,",
                qryFreqLockCnf->freqState[AT_MTA_M2M_FREQLOCK_MODE_LTE - 1],
                g_freqLockModeTbl[AT_MTA_M2M_FREQLOCK_MODE_LTE - 1].strMode, qryFreqLockCnf->lFreqLockInfo.freq);
        } else {
            /* <enable>,<mode>,<freq>,,,<pci> */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%d,\"%s\",%d,,,%d",
                qryFreqLockCnf->freqState[AT_FREQLOCK_MODE_TYPE_LTE - 1],
                g_freqLockModeTbl[AT_FREQLOCK_MODE_TYPE_LTE - 1].strMode, qryFreqLockCnf->lFreqLockInfo.freq,
                qryFreqLockCnf->lFreqLockInfo.pci);
        }
    }

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaSetXCposrCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg       *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_ResultCnf *setXcposrCnf = VOS_NULL_PTR;
    VOS_UINT8         indexNum     = 0;
    VOS_UINT32        result;

    /* 初始化消息变量 */
    mtaMsg       = (AT_MTA_Msg *)msg;
    setXcposrCnf = (MTA_AT_ResultCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetXCposrCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetXCposrCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_XCPOSR_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_XCPOSR_SET) {
        AT_WARN_LOG("AT_RcvMtaSetXCposrCnf: WARNING:Not AT_CMD_XCPOSR_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (setXcposrCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;
    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryXcposrCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg          *mtaMsg       = VOS_NULL_PTR;
    MTA_AT_QryXcposrCnf *qryXcposrCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum     = 0;
    VOS_UINT32           result;

    /* 初始化消息变量 */
    mtaMsg       = (AT_MTA_Msg *)msg;
    qryXcposrCnf = (MTA_AT_QryXcposrCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_XCPOSR_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_XCPOSR_QRY) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:Not AT_CMD_XCPOSR_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (qryXcposrCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            qryXcposrCnf->xcposrEnableCfg);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetXcposrRptCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg       *mtaMsg          = VOS_NULL_PTR;
    MTA_AT_ResultCnf *setXcposrRptCnf = VOS_NULL_PTR;
    VOS_UINT8         indexNum        = 0;
    VOS_UINT32        result;

    /* 初始化消息变量 */
    mtaMsg          = (AT_MTA_Msg *)msg;
    setXcposrRptCnf = (MTA_AT_ResultCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetXcposrRptCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetXcposrRptCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_XCPOSR_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_XCPOSRRPT_SET) {
        AT_WARN_LOG("AT_RcvMtaSetXcposrRptCnf: WARNING:Not AT_CMD_XCPOSRRPT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (setXcposrRptCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryXcposrRptCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *mtaMsg          = VOS_NULL_PTR;
    MTA_AT_QryXcposrrptCnf *qryXcposrRptCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum        = 0;
    VOS_UINT32              result;

    /* 初始化消息变量 */
    mtaMsg          = (AT_MTA_Msg *)msg;
    qryXcposrRptCnf = (MTA_AT_QryXcposrrptCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_XCPOSR_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_XCPOSRRPT_QRY) {
        AT_WARN_LOG("AT_RcvMtaQryXcposrCnf: WARNING:Not AT_CMD_XCPOSR_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (qryXcposrRptCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            qryXcposrRptCnf->xcposrRptFlg);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvFratIgnitionQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *mtaMsg      = VOS_NULL_PTR;
    MTA_AT_FratIgnitionQryCnf *ignitionCnf = VOS_NULL_PTR;
    VOS_UINT32                 ret;
    VOS_UINT8                  indexNum;

    /* 初始化消息变量 */
    mtaMsg      = (AT_MTA_Msg *)msg;
    ignitionCnf = (MTA_AT_FratIgnitionQryCnf *)mtaMsg->content;
    indexNum    = 0;
    ret         = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvFratIgnitionQryCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvFratIgnitionQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FRATIGNITION_QRY) {
        AT_WARN_LOG("AT_RcvFratIgnitionQryCnf : Current Option is not AT_CMD_FRATIGNITION_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (ignitionCnf->result != MTA_AT_RESULT_NO_ERROR) {
        ret = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %u", g_parseContext[indexNum].cmdElement->cmdName,
            ignitionCnf->fratIgnitionState);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_RcvFratIgnitionSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg                *mtaMsg     = VOS_NULL_PTR;
    MTA_AT_FratIgnitionSetCnf *cnfContent = VOS_NULL_PTR;
    VOS_UINT8                  indexNum   = 0;
    VOS_UINT32                 result;

    /* 初始化消息变量 */
    mtaMsg = (AT_MTA_Msg *)msg;
    cnfContent = (MTA_AT_FratIgnitionSetCnf *)mtaMsg->content;
    result = AT_ERROR;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvFratIgnitionSetCnf: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvFratIgnitionSetCnf: AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_FRATIGNITION_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FRATIGNITION_SET) {
        AT_WARN_LOG("AT_RcvFratIgnitionSetCnf: Not AT_CMD_FRATIGNITION_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (cnfContent->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = 0;

    /* 调用AT_FormatResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetSensorCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetSensorCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetSensorCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetSensorCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetSensorCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SENSOR_SET) {
        AT_WARN_LOG("AT_RcvMtaSetSensorCnf : Current Option is not AT_CMD_SENSOR_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSetScreenCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetScreenCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8            indexNum;
    VOS_UINT32           result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetScreenCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetScreenCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetScreenCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SCREEN_SET) {
        AT_WARN_LOG("AT_RcvMtaSetScreenCnf : Current Option is not AT_CMD_SCREEN_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMtaUartTestCnf(struct MsgCB *msg)
{
    AT_MTA_Msg         *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_UartTestCnf *uartTestCnf = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT8           indexId;

    /* 初始化 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    uartTestCnf = (MTA_AT_UartTestCnf *)rcvMsg->content;
    indexId     = 0;
    result      = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexId) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaUartTestCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexId)) {
        AT_WARN_LOG("AT_RcvMtaUartTestCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexId].cmdCurrentOpt != AT_CMD_UART_TEST_SET) {
        AT_WARN_LOG("AT_RcvMtaUartTestCnf : Current Option is not AT_CMD_SCREEN_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexId);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (uartTestCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexId, result);
    return VOS_OK;
}


AT_UartTestStateUint8 AT_ConvertUartTestResult(MTA_AT_UartTestRsltUint32 uartTestRslt)
{
    AT_UartTestStateUint8 returnCode;

    switch (uartTestRslt) {
        case MTA_AT_UART_TEST_RSLT_SUCCESS:
            returnCode = AT_UART_TEST_PASS;
            break;
        case MTA_AT_UART_TEST_RSLT_DATA_ERROR:
            returnCode = AT_UART_TEST_DATA_ERROR;
            break;
        case MTA_AT_UART_TEST_RSLT_ICC_NOT_OPEN:
            returnCode = AT_UART_TEST_ICC_NOT_OPEN;
            break;
        case MTA_AT_UART_TEST_RSLT_TIME_OUT:
            returnCode = AT_UART_TEST_TIMEOUT;
            break;
        default:
            returnCode = AT_UART_TEST_DATA_ERROR;
            break;
    }

    return returnCode;
}


VOS_UINT32 AT_RcvMtaUartTestRsltInd(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_UartTestRsltInd *uartTestRsltInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    rcvMsg          = (AT_MTA_Msg *)msg;
    uartTestRsltInd = (MTA_AT_UartTestRsltInd *)rcvMsg->content;

    AT_SetUartTestState(AT_ConvertUartTestResult(uartTestRsltInd->uartTestRslt));

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaSetModemTimeCnf(struct MsgCB *msg)
{
    AT_MTA_Msg        *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetTimeCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8          indexNum;
    VOS_UINT32         result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetTimeCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetModemTimeCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetModemTimeCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MODEM_TIME_SET) {
        AT_WARN_LOG("AT_RcvMtaSetModemTimeCnf : Current Option is not AT_CMD_MODEM_TIME_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaAfcClkUnlockCauseInd(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg               = VOS_NULL_PTR;
    MTA_AT_AfcClkUnlockInd *mtaAtAfcClkUnlockInd = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT16              length;

    /* 初始化 */
    rcvMsg               = (AT_MTA_Msg *)msg;
    mtaAtAfcClkUnlockInd = (MTA_AT_AfcClkUnlockInd *)rcvMsg->content;
    indexNum             = 0;
    length               = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaAfcClkUnlockCauseInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 打印^AFCCLKUNLOCK: */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_AFCCLKUNLOCK].text,
        mtaAtAfcClkUnlockInd->cause, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


/*lint -save -e838 -specific(-e838)*/
/*lint -save -e826 -specific(-e826)*/
VOS_UINT32 AT_RcvDrvAgentQryCcpuMemInfoCnf(struct MsgCB *data)
{
    VOS_UINT8                    indexNum          = 0;
    DRV_AGENT_QryCcpuMemInfoCnf *qryCcpuMemInfoCnf = VOS_NULL_PTR;
    DRV_AGENT_Msg               *rcvMsg            = VOS_NULL_PTR;

    rcvMsg            = (DRV_AGENT_Msg *)data;
    qryCcpuMemInfoCnf = (DRV_AGENT_QryCcpuMemInfoCnf *)rcvMsg->content;

    /* 根据ClientID获取通道索引 */
    if (At_ClientIdToUserId(qryCcpuMemInfoCnf->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentQryCcpuMemInfoCnf: Get Index Fail!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentQryCcpuMemInfoCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* AT模块在等待^CCPUMEMINFO命令的操作结果事件上报 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCPUMEMINFO_QRY) {
        AT_WARN_LOG("AT_RcvDrvAgentQryCcpuMemInfoCnf: Error Option!");
        return VOS_ERR;
    }

    /* 使用AT_STOP_TIMER_CMD_READY恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            qryCcpuMemInfoCnf->chkPoolLeakRslt, qryCcpuMemInfoCnf->chkNodeLeakRslt,
            qryCcpuMemInfoCnf->chkRrmNodeLeakRslt);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}
/*lint -restore*/
/*lint -restore*/


VOS_UINT32 AT_RcvMtaSetBestFreqCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32        rslt;
    VOS_UINT8         indexNum;

    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    indexNum = 0;
    rslt     = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetBestFreqCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetBestFreqCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BESTFREQ_SET) {
        AT_WARN_LOG("AT_RcvMtaSetBestFreqCnf : Current Option is not AT_CMD_RXTESTMODE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        rslt = AT_OK;
    }

    At_FormatResultData(indexNum, rslt);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaBestFreqInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg             *mtaMsg          = VOS_NULL_PTR;
    MTA_AT_BestFreqCaseInd *bestFreqCaseInd = VOS_NULL_PTR;
    VOS_UINT32              loop;
    VOS_UINT16              length;
    VOS_UINT8               indexNum;

    indexNum        = 0;
    length          = 0;
    mtaMsg          = (AT_MTA_Msg *)msg;
    bestFreqCaseInd = (MTA_AT_BestFreqCaseInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaBestFreqInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    bestFreqCaseInd->rptDeviceNum = AT_MIN(bestFreqCaseInd->rptDeviceNum, MTA_AT_MAX_BESTFREQ_GROUPNUM);
    for (loop = 0; loop < bestFreqCaseInd->rptDeviceNum; loop++) {
        length = 0;

        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^BESTFREQ: %d,%d,%d%s", g_atCrLf,
            bestFreqCaseInd->deviceFreqList[loop].groupId, bestFreqCaseInd->deviceFreqList[loop].deviceId,
            bestFreqCaseInd->deviceFreqList[loop].caseId, g_atCrLf);

        g_atSendDataBuff.bufLen = length;

        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaQryBestFreqCnf(struct MsgCB *msg)

{
    AT_MTA_Msg            *mtaMsg      = VOS_NULL_PTR;
    MTA_AT_BestfreqQryCnf *bestFreqCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT32             loop;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum    = 0;
    length      = 0;
    mtaMsg      = (AT_MTA_Msg *)msg;
    bestFreqCnf = (MTA_AT_BestfreqQryCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaBestFreqInfoCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaBestFreqInfoCnf: WARNING:AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BESTFREQ_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (bestFreqCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        bestFreqCnf->activeDeviceNum = AT_MIN(bestFreqCnf->activeDeviceNum, MTA_AT_MAX_BESTFREQ_GROUPNUM);
        for (loop = 0; loop < bestFreqCnf->activeDeviceNum; loop++) {
            if (loop != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, bestFreqCnf->deviceInfoList[loop].groupId,
                bestFreqCnf->deviceInfoList[loop].deviceId, bestFreqCnf->deviceInfoList[loop].caseId,
                bestFreqCnf->deviceInfoList[loop].mode);
        }
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvMtaModemCapUpdateCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ModemCapUpdateCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;
    VOS_UINT32                result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_ModemCapUpdateCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaModemCapUpdateCnf : WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaModemCapUpdateCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MODEM_CAP_UPDATE_SET) {
        AT_WARN_LOG("AT_RcvMtaModemCapUpdateCnf : Current Option is not AT_CMD_MODEM_CAP_UPDATE_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->rslt == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}




VOS_UINT32 AT_RcvMmaPacspQryCnf(struct MsgCB *msg)
{
    TAF_MMA_PacspQryCnf *pacspQryCnf = VOS_NULL_PTR;
    ModemIdUint16        modemId;
    VOS_UINT16           length;
    VOS_UINT8            indexNum = 0;

    pacspQryCnf = (TAF_MMA_PacspQryCnf *)msg;
    length      = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(pacspQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPacspQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaDplmnQryCnf: Get modem id fail.");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPacspQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PACSP_QRY) {
        AT_WARN_LOG("AT_RcvMmaPacspQryCnf : Current Option is not AT_CMD_PACSP_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "+PACSP%d", pacspQryCnf->plmnMode);

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


#if (FEATURE_LTE == FEATURE_ON)

VOS_UINT32 AT_RcvMtaLteCategoryInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg                *mtaMsg             = VOS_NULL_PTR;
    MTA_AT_LteCategoryInfoInd *lteCategoryInfoInd = VOS_NULL_PTR;

    mtaMsg             = (AT_MTA_Msg *)msg;
    lteCategoryInfoInd = (MTA_AT_LteCategoryInfoInd *)mtaMsg->content;

    if ((lteCategoryInfoInd->dlCategery >= AT_UE_LTE_CATEGORY_NUM_MAX) ||
        (lteCategoryInfoInd->ulCategery >= AT_UE_LTE_CATEGORY_NUM_MAX)) {
        AT_WARN_LOG2("AT_RcvMtaLteCategoryInfoInd: WARNING: illegal Category value <DlCategery, UlCategery>!",
                     lteCategoryInfoInd->dlCategery, lteCategoryInfoInd->ulCategery);
        return VOS_ERR;
    } else {
        g_atDlRateCategory.lteUeDlCategory = lteCategoryInfoInd->dlCategery;
        g_atDlRateCategory.lteUeUlCategory = lteCategoryInfoInd->ulCategery;
    }

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaPhyCommAckInd(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_PhyCommAckInd *mtaAtCommAckInd = VOS_NULL_PTR;
    VOS_UINT16            length;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg          = (AT_MTA_Msg *)msg;                       /*lint !e838 */
    mtaAtCommAckInd = (MTA_AT_PhyCommAckInd *)rcvMsg->content; /*lint !e838 */
    indexNum        = 0;
    length          = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPhyCommAckInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 打印^PHYCOMACK: */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%d,%d%s", g_atCrLf,
        g_atStringTab[AT_STRING_PHYCOMACK].text, mtaAtCommAckInd->ackType, mtaAtCommAckInd->ackValue1,
        mtaAtCommAckInd->ackValue2, mtaAtCommAckInd->ackValue3, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)

VOS_UINT32 AT_RcvMtaRcmDcxoSampleQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg                = VOS_NULL_PTR;
    MTA_AT_DcxoSampeQryCnf *mtaAtDcxoSampleQryCnf = VOS_NULL_PTR;
    VOS_UINT16              dataLen;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;
    VOS_UINT32              i;

    /* 初始化 */
    rcvMsg                = (AT_MTA_Msg *)msg;
    mtaAtDcxoSampleQryCnf = (MTA_AT_DcxoSampeQryCnf *)rcvMsg->content;
    indexNum              = 0;
    dataLen               = 0;
    i                     = 0;
    result                = AT_OK;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRcnDcxoSampleQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaRcnDcxoSampleQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_DCXOQRY_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DCXOQRY_QRY) {
        AT_WARN_LOG("AT_RcvMtaRcnDcxoSampleQryCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 打印^PHYCOMACK: */
    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,", mtaAtDcxoSampleQryCnf->dcxoSampleType);

    if (mtaAtDcxoSampleQryCnf->result == MTA_AT_DCXO_SAMPLE_RESULT_NO_ERROR) {
        switch (mtaAtDcxoSampleQryCnf->dcxoSampleType) {
            case AT_MTA_DCXO_SAMPLE_H:
            case AT_MTA_DCXO_SAMPLE_M:
            case AT_MTA_DCXO_SAMPLE_L:
                dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,", mtaAtDcxoSampleQryCnf->result);
                for (i = 0; i < UCOM_NV_DCXO_SAMPLE_H_MAX_NUM; i++) {
                    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,",
                        mtaAtDcxoSampleQryCnf->sctionData.sample[i].swTime);

                    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,",
                        mtaAtDcxoSampleQryCnf->sctionData.sample[i].swMode);

                    dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                        (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,",
                        mtaAtDcxoSampleQryCnf->sctionData.sample[i].swTemp);
                    if (i == (UCOM_NV_DCXO_SAMPLE_H_MAX_NUM - 1)) {
                        dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d",
                            mtaAtDcxoSampleQryCnf->sctionData.sample[i].swPpm);
                    } else {
                        dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                            (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,",
                            mtaAtDcxoSampleQryCnf->sctionData.sample[i].swPpm);
                    }
                }
                break;
            case AT_MTA_DCXO_SAMPLE_INITFREQ:
                dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,", mtaAtDcxoSampleQryCnf->result);
                dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d,",
                    mtaAtDcxoSampleQryCnf->sctionData.dcxoInitPpm.swInitFrequency);

                dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d",
                    mtaAtDcxoSampleQryCnf->sctionData.dcxoInitPpm.swInitTemperature);
                break;
            default:
                mtaAtDcxoSampleQryCnf->result = MTA_AT_DCXO_SAMPLE_RESULT_ERROR;
                dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d", mtaAtDcxoSampleQryCnf->result);
                break;
        }
    } else {
        dataLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + dataLen, "%d", mtaAtDcxoSampleQryCnf->result);
    }

    g_atSendDataBuff.bufLen = dataLen;

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMmaMtReattachInd(struct MsgCB *msg)
{
    TAF_MMA_MtreattachInd *mtReattachInd = VOS_NULL_PTR;
    ModemIdUint16          modemId;
    VOS_UINT8              indexNum;
    VOS_UINT16             length;

    /* 初始化消息变量 */
    indexNum      = 0;
    length        = 0;
    mtReattachInd = (TAF_MMA_MtreattachInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtReattachInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaMtReattachInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaMtReattachInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_MTREATTACH].text,
        g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCemodeSetCnf(struct MsgCB *msg)
{
    TAF_MMA_CemodeSetCnf *cemodeSetCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    cemodeSetCnf = (TAF_MMA_CemodeSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cemodeSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCemodeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCemodeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CEMODE_SET) {
        AT_WARN_LOG("AT_RcvMmaCemodeSetCnf : Current Option is not AT_CMD_CEMODE_SET.");
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (cemodeSetCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, cemodeSetCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaCemodeQryCnf(struct MsgCB *msg)
{
    TAF_MMA_CemodeQryCnf *cemodeQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化消息变量 */
    indexNum     = 0;
    result       = 0;
    cemodeQryCnf = (TAF_MMA_CemodeQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cemodeQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaCemodeQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaCemodeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CEMODE_READ) {
        AT_WARN_LOG("AT_RcvMmaCemodeQryCnf : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cemodeQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d%s", g_atCrLf, g_parseContext[indexNum].cmdElement->cmdName,
            cemodeQryCnf->ceMode, g_atCrLf);
        result                  = AT_OK;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaPseudBtsSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_PseudBtsSetCnf *setPseudBtsCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;
    VOS_UINT8              indexNum = 0;

    length         = 0;
    rcvMsg         = (AT_MTA_Msg *)msg;
    setPseudBtsCnf = (MTA_AT_PseudBtsSetCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPseudBtsSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaPseudBtsSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PSEUDBTS_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setPseudBtsCnf->result == MTA_AT_RESULT_NO_ERROR) {
        if (setPseudBtsCnf->pseudBtsIdentType == AT_PSEUD_BTS_PARAM_TIMES) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                setPseudBtsCnf->u.pseudBtsIdentTimes);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                setPseudBtsCnf->u.pseudBtsIdentCap);
        }

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else if (setPseudBtsCnf->result == MTA_AT_RESULT_INCORRECT_PARAMETERS) {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_CME_INCORRECT_PARAMETERS;
    } else if (setPseudBtsCnf->result == MTA_AT_RESULT_FUNC_DISABLE) {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_CME_FUNC_DISABLE;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSubClfSparamSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                *rcvMsg             = VOS_NULL_PTR;
    MTA_AT_SubclfsparamSetCnf *setSubClfSparamCnf = VOS_NULL_PTR;
    VOS_UINT8                  indexNum           = 0;
    VOS_UINT32                 result;

    rcvMsg             = (AT_MTA_Msg *)msg;
    setSubClfSparamCnf = (MTA_AT_SubclfsparamSetCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSubClfSparamSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSubClfSparamSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SUBCLFSPARAM_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setSubClfSparamCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }
    g_atSendDataBuff.bufLen = 0;

    /* 发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSubClfSparamQryCnf(struct MsgCB *msg)
{
    MTA_AT_SubclfsparamQryCnf *subClfSparamQryCnf = VOS_NULL_PTR;
    AT_MTA_Msg                *rcvMsg             = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT16                 length;
    VOS_UINT8                  indexNum = 0;

    length             = 0;
    rcvMsg             = (AT_MTA_Msg *)msg;
    subClfSparamQryCnf = (MTA_AT_SubclfsparamQryCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSubClfSparamQryCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSubClfSparamQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SUBCLFSPARAM_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (subClfSparamQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName,
            subClfSparamQryCnf->versionId);

        g_atSendDataBuff.bufLen = length;
        result                  = AT_OK;
    } else if (subClfSparamQryCnf->result == MTA_AT_RESULT_INCORRECT_PARAMETERS) {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_CME_INCORRECT_PARAMETERS;
    } else if (subClfSparamQryCnf->result == MTA_AT_RESULT_FUNC_DISABLE) {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_CME_FUNC_DISABLE;
    } else {
        g_atSendDataBuff.bufLen = 0;
        result                  = AT_ERROR;
    }

    /* 发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRejinfoQryCnf(struct MsgCB *msg)
{
    TAF_MMA_RejinfoQryCnf *rejinfoQryCnf = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum      = 0;
    length        = 0;
    rejinfoQryCnf = (TAF_MMA_RejinfoQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rejinfoQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRejinfoQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRejinfoQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_REJINFO_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_REJINFO_QRY) {
        return VOS_ERR;
    }

    /* 停止保护定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 打印rejinfo AT命令参数 */
    length = AT_PrintRejinfo(length, &(rejinfoQryCnf->phoneRejInfo));

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, AT_OK);
    return VOS_OK;
}


VOS_UINT16 AT_PrintRejinfo(VOS_UINT16 length, TAF_PH_RegRejInfo *rejinfo)
{
    VOS_UINT16 len;

    len = length;

    len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + len, "%x%x%x", (rejinfo->plmnId.mcc & 0x0f00) >> 8,
        (rejinfo->plmnId.mcc & 0xf0) >> 4, (rejinfo->plmnId.mcc & 0x0f));

    if ((rejinfo->plmnId.mnc & 0x0f00) == 0x0f00) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%x%x,", (rejinfo->plmnId.mnc & 0xf0) >> 4,
            (rejinfo->plmnId.mnc & 0x0f));
    } else {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%x%x%x,", (rejinfo->plmnId.mnc & 0x0f00) >> 8,
            (rejinfo->plmnId.mnc & 0xf0) >> 4, (rejinfo->plmnId.mnc & 0x0f));
    }

    len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + len, "%d,%d,%d,%d,%d,", rejinfo->srvDomain, rejinfo->rejCause, rejinfo->rat,
        rejinfo->rejType, rejinfo->originalRejCause);

    /* Lac */
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    if (rejinfo->rat == TAF_MMA_REJ_RAT_NR) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "\"%X%X%X%X%X%X\",", 0x000f & (rejinfo->lac >> 20),
            0x000f & (rejinfo->lac >> 16), 0x000f & (rejinfo->lac >> 12), 0x000f & (rejinfo->lac >> 8),
            0x000f & (rejinfo->lac >> 4), 0x000f & (rejinfo->lac >> 0));
    } else
#endif
    {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "\"%X%X%X%X\",", 0x000f & (rejinfo->lac >> 12),
            0x000f & (rejinfo->lac >> 8), 0x000f & (rejinfo->lac >> 4), 0x000f & (rejinfo->lac >> 0));
    }

    /* Rac */
    len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + len, "\"%X%X\",", 0x000f & (rejinfo->rac >> 4), 0x000f & (rejinfo->rac >> 0));

    /* <CI>域以8字节方式上报 */
    len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + len, "\"%08X%08X\"", rejinfo->cellId.cellIdHighBit,
        rejinfo->cellId.cellIdLowBit);

    /* ESM cause */
    if (rejinfo->pdnRejCause != 0) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, ",%d", rejinfo->pdnRejCause);
    }

    return len;
}


VOS_UINT32 AT_RcvMmaElevatorStateInd(struct MsgCB *msg)
{
    TAF_MMA_ElevatorStateInd *elevatorState = VOS_NULL_PTR;
    ModemIdUint16             modemId;
    VOS_UINT8                 indexNum;
    VOS_UINT16                length;

    /* 初始化消息变量 */
    indexNum      = 0;
    length        = 0;
    elevatorState = (TAF_MMA_ElevatorStateInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(elevatorState->clientId, &indexNum) == AT_FAILURE) {
        AT_ERR_LOG("AT_RcvMmaElevatorStateInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaElevatorStateInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_ELEVATOR].text,
        elevatorState->sensorPara.sensorState, elevatorState->sensorPara.serviceState,
        elevatorState->sensorPara.sensorScene, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

VOS_UINT32 AT_RcvTafCcmCSChannelInfoQryCnf(struct MsgCB *msg)
{
    TAF_CCM_QryChannelInfoCnf *channelInfo = VOS_NULL_PTR;
    VOS_UINT32                 result;
    VOS_UINT8                  indexNum;

    channelInfo = (TAF_CCM_QryChannelInfoCnf *)msg;
    indexNum    = 0;

    /* 根据clientId获取通道索引 */
    if (At_ClientIdToUserId(channelInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmCSChannelInfoQryCnf: Get Index Fail!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCcmCSChannelInfoQryCnf: WARNING:AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSCHANNELINFO_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (channelInfo->para.result != VOS_OK) {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    } else {
        result = AT_OK;

        /* 输出查询结果 */
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            channelInfo->para.channelType, channelInfo->para.voiceDomain);
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmChannelInfoInd(struct MsgCB *msg)
{
    TAF_CCM_CallChannelInfoInd *channelInfoInd = VOS_NULL_PTR;
    VOS_UINT16                  length;
    VOS_UINT8                   indexNum;
    TAF_CALL_ChannelTypeUint8   channelType;

    length         = 0;
    indexNum       = 0;
    channelInfoInd = (TAF_CCM_CallChannelInfoInd *)msg;

    if ((channelInfoInd->codecType == MN_CALL_CODEC_TYPE_BUTT) && (channelInfoInd->isLocalAlertingFlag == VOS_FALSE)) {
        AT_WARN_LOG("AT_RcvTafCcmChannelInfoInd: WARNING: CodecType BUTT!");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(channelInfoInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmChannelInfoInd:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    channelType = AT_ConvertCodecTypeToChannelType(channelInfoInd->isLocalAlertingFlag, channelInfoInd->codecType);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d,%d%s", g_atCrLf,
        g_atStringTab[AT_STRING_CS_CHANNEL_INFO].text, channelType, channelInfoInd->voiceDomain, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 At_RcvTafCcmCallModifyCnf(struct MsgCB *msg)
{
    TAF_CCM_CallAnswerRemoteModifyCnf *modifyCnf = VOS_NULL_PTR;
    VOS_UINT8                          indexNum;

    modifyCnf = (TAF_CCM_CallAnswerRemoteModifyCnf *)msg;
    indexNum  = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(modifyCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvTafCallModifyCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvTafCallModifyCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CALL_MODIFY_INIT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CALL_MODIFY_INIT_SET) {
        AT_WARN_LOG("At_RcvTafCallModifyCnf: WARNING:Not AT_CMD_CALL_MODIFY_INIT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断操作是否成功 */
    if (modifyCnf->cause != TAF_CS_CAUSE_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 At_RcvTafCcmCallAnswerRemoteModifyCnf(struct MsgCB *msg)
{
    TAF_CCM_CallAnswerRemoteModifyCnf *modifyCnf = VOS_NULL_PTR;
    VOS_UINT8                          indexNum;

    modifyCnf = (TAF_CCM_CallAnswerRemoteModifyCnf *)msg;
    indexNum  = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(modifyCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvTafCcmCallAnswerRemoteModifyCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("At_RcvTafCcmCallAnswerRemoteModifyCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CALL_MODIFY_ANS_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CALL_MODIFY_ANS_SET) {
        AT_WARN_LOG("At_RcvTafCcmCallAnswerRemoteModifyCnf: WARNING:Not AT_CMD_CALL_MODIFY_CNF_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断操作是否成功 */
    if (modifyCnf->cause != TAF_CS_CAUSE_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmQryEconfCalledInfoCnf(struct MsgCB *msg)
{
    TAF_CCM_QryEconfCalledInfoCnf *econfInfo = VOS_NULL_PTR;
    VOS_UINT32                     ret;
    VOS_UINT8                      indexNum;

    /* 初始化 */
    econfInfo = (TAF_CCM_QryEconfCalledInfoCnf *)msg;
    indexNum  = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(econfInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmQryEconfCalledInfoCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLCCECONF_QRY) {
        AT_WARN_LOG("AT_RcvTafCcmQryEconfCalledInfoCnf: WARNING: CmdCurrentOpt != AT_CMD_CLCCECONF_QRY!");
        return VOS_ERR;
    }

    ret = At_ProcQryClccEconfResult(econfInfo, indexNum);

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMta5gNssaiSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_5GNssaiSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_5GNssaiSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMta5gNssaiSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMta5gNssaiSetCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GPNSSAI_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GDFTCFGNSSAI_SET)) {
        AT_WARN_LOG("AT_RcvMta5gNssaiSetCnf : Current Option is not NSSAI_SET.");

        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_Print5gPreferNssai(VOS_UINT8 indexNum, MTA_AT_5GNssaiQryCnf *pst5gNssaiQryCnf)
{
    VOS_UINT16 length;
    VOS_UINT32 nssaiLen;
    VOS_CHAR   acStrPreferNssai[AT_EVT_MULTI_S_NSSAI_LEN];

    length = 0;

    /* +C5GPNSSAI:
     * <Preferred_NSSAI_3gpp_length>,<Preferred_NSSAI_3gpp>,<Preferred_NSSAI_non3gpp_length>,<Preferred_NSSAI_non3gpp>
     */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 组装切片消息格式 */
    nssaiLen = 0;
    (VOS_VOID)memset_s(acStrPreferNssai, sizeof(acStrPreferNssai), 0, sizeof(acStrPreferNssai));

    AT_ConvertMultiSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gPreferNssai.snssaiNum, PS_MAX_CFG_S_NSSAI_NUM),
                                  &pst5gNssaiQryCnf->st5gPreferNssai.snssai[0], acStrPreferNssai,
                                  sizeof(acStrPreferNssai), &nssaiLen);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", nssaiLen);

    if (nssaiLen != 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrPreferNssai);
    }

    g_atSendDataBuff.bufLen = length;

    return;
}


VOS_VOID AT_Print5gDefCfgNssai(VOS_UINT8 indexNum, MTA_AT_5GNssaiQryCnf *pst5gNssaiQryCnf)
{
    VOS_UINT16 length;
    VOS_UINT32 nssaiLen;
    VOS_CHAR   acStrDefCfgNssai[AT_EVT_MULTI_S_NSSAI_LEN];

    length = 0;

    /* +C5GNSSAI: [<default_configured_nssai_length>,<default_configured_nssai>] */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 组装切片消息格式 */
    nssaiLen = 0;
    (VOS_VOID)memset_s(acStrDefCfgNssai, sizeof(acStrDefCfgNssai), 0, sizeof(acStrDefCfgNssai));

    AT_ConvertMultiSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gDefCfgNssai.snssaiNum, PS_MAX_CFG_S_NSSAI_NUM),
                                  &pst5gNssaiQryCnf->st5gDefCfgNssai.snssai[0], acStrDefCfgNssai,
                                  sizeof(acStrDefCfgNssai), &nssaiLen);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", nssaiLen);

    if (nssaiLen != 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrDefCfgNssai);
    }

    g_atSendDataBuff.bufLen = length;

    return;
}


VOS_VOID AT_Print5gNssaiRdpInfo(VOS_UINT8 indexNum, MTA_AT_5GNssaiQryCnf *pst5gNssaiQryCnf)
{
    VOS_UINT16 length = 0;
    VOS_UINT32 nssaiLen;
    VOS_CHAR   acStrNssai[AT_EVT_MULTI_S_NSSAI_LEN];
    VOS_UINT32 i;

    /*
     * [+C5GNSSAIRDP:
     * [<default_configured_nssai_length>,<default_configured_nssai>[,<rejected_nssai_3gpp_length>,<rejected_nssai_3gpp>[,<rejected_nssai_non3gpp_length>,<rejected_nssai_non3gpp>]]]
     * [<CR><LF>+C5GNSSAIRDP:
     * <plmn_id>[,<configured_nssai_length>,<configured_nssai>[,<allowed_nssai_3gpp_length>,<allowed_nssai_3gpp>,<allowed_nssai_non3gpp_length>,<allowed_nssai_non3gpp>]]
     * [<CR><LF>+C5GNSSAIRDP:
     * <plmn_id>[,<configured_nssai_length>,<configured_nssai>[,<allowed_nssai_3gpp_length>,<allowed_nssai_3gpp>,<allowed_nssai_non3gpp_length>,<allowed_nssai_non3gpp>]]
     * [...]]]]
     */

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* 组装Default切片消息格式 */
    nssaiLen = 0;
    (VOS_VOID)memset_s(acStrNssai, sizeof(acStrNssai), 0, sizeof(acStrNssai));

    AT_ConvertMultiSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gDefCfgNssai.snssaiNum, PS_MAX_CFG_S_NSSAI_NUM),
        &pst5gNssaiQryCnf->st5gDefCfgNssai.snssai[0], acStrNssai, sizeof(acStrNssai), &nssaiLen);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", nssaiLen);

    if (nssaiLen != 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrNssai);
    }

    if (pst5gNssaiQryCnf->sNssaiQryType > AT_MTA_NSSAI_QRY_DEF_CFG_NSSAI_ONLY) {
        /* 组装reject 3gpp切片消息格式 */
        nssaiLen = 0;
        (VOS_VOID)memset_s(acStrNssai, sizeof(acStrNssai), 0, sizeof(acStrNssai));

        AT_ConvertMultiRejectSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gRejNssai.snssaiNum, PS_MAX_REJECT_S_NSSAI_NUM),
            &pst5gNssaiQryCnf->st5gRejNssai.snssai[0], acStrNssai, sizeof(acStrNssai), &nssaiLen);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,", nssaiLen);

        if (nssaiLen != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrNssai);
        }
    }

    if (pst5gNssaiQryCnf->sNssaiQryType <= AT_MTA_NSSAI_QRY_DEF_CFG_AND_REJ_NSSAI) {
        /* 如果查询type是default config nssai only或者default config nssai and reject nssai，查询完毕，则直接提前return */
        g_atSendDataBuff.bufLen = length;
        return;
    }

    pst5gNssaiQryCnf->st5gAllowCfgNssai.plmnNum = AT_MIN(pst5gNssaiQryCnf->st5gAllowCfgNssai.plmnNum,
                                                         MTA_AT_MAX_ALLOWED_AND_CFG_NSSAI_PLMN_NUM);

    for (i = 0; i < pst5gNssaiQryCnf->st5gAllowCfgNssai.plmnNum; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

        /* 打印输出PLMN ID */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%X%X%X",
            (0x0f & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_PLMNID]),
            (0xf0 & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_PLMNID]) >> 4,
            (0x0f & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_PLMNSTATUS]));

        /* MNC为两位 */
        if ((0xf0 & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_PLMNSTATUS]) == 0xf0) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X\"",
                (0x0f & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_ACCESS_MODE]),
                (0xf0 & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_ACCESS_MODE]) >> 4);
        }
        /* MNC为三位 */
        else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                    (TAF_CHAR *)g_atSndCodeAddress + length, "%X%X%X\"",
                    (0x0f & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_ACCESS_MODE]),
                    (0xf0 & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_ACCESS_MODE]) >> 4,
                    (0xf0 & pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].plmnId.plmnId[AT_PLMN_PLMNSTATUS]) >> 4);
        }

        /* 组装config切片消息格式 */
        nssaiLen = 0;
        (VOS_VOID)memset_s(acStrNssai, sizeof(acStrNssai), 0, sizeof(acStrNssai));

        AT_ConvertMultiSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].cfgNssai.snssaiNum,
            PS_MAX_CFG_S_NSSAI_NUM), &pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].cfgNssai.snssai[0], acStrNssai,
            sizeof(acStrNssai), &nssaiLen);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,", nssaiLen);

        if (nssaiLen != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrNssai);
        }

        if (pst5gNssaiQryCnf->sNssaiQryType == AT_MTA_NSSAI_QRY_DEF_CFG_AND_REJ_AND_CFG_NSSAI) {
            continue;
        }

        /* 组装Allowed切片消息格式 */
        nssaiLen = 0;
        (VOS_VOID)memset_s(acStrNssai, sizeof(acStrNssai), 0, sizeof(acStrNssai));

        AT_ConvertMultiSNssaiToString(AT_MIN(pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].allowNssai.snssaiNum,
            PS_MAX_ALLOWED_S_NSSAI_NUM), &pst5gNssaiQryCnf->st5gAllowCfgNssai.nssai[i].allowNssai.snssai[0], acStrNssai,
            sizeof(acStrNssai), &nssaiLen);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,", nssaiLen);

        if (nssaiLen != 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", acStrNssai);
        }
    }

    g_atSendDataBuff.bufLen = length;
    return;
}


VOS_UINT32 AT_RcvMta5gNssaiQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_5GNssaiQryCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    qryCnf   = (MTA_AT_5GNssaiQryCnf *)rcvMsg->content;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMta5gNssaiSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMta5gNssaiSetCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GPNSSAI_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GNSSAI_QRY) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_C5GNSSAIRDP_SET)) {
        AT_WARN_LOG("AT_RcvMta5gNssaiQryCnf : Current Option is not qry nssai.");

        return VOS_ERR;
    }

    /* 判断查询结果是否失败,如果失败则返回ERROR */
    if (qryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_C5GPNSSAI_QRY) {
            AT_Print5gPreferNssai(indexNum, qryCnf);
        }

        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_C5GNSSAI_QRY) {
            AT_Print5gDefCfgNssai(indexNum, qryCnf);
        }

        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_C5GNSSAIRDP_SET) {
            AT_Print5gNssaiRdpInfo(indexNum, qryCnf);
        }

        At_FormatResultData(indexNum, AT_OK);
    }

    /* 恢复AT命令实体状态为READY状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLadnRptSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetLadnRptCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             idx;

    rcvMsg = (AT_MTA_Msg *)msg;
    setCnf = (MTA_AT_SetLadnRptCnf *)rcvMsg->content;
    result = AT_OK;
    idx    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &idx) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLadnRptSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(idx)) {
        AT_WARN_LOG("AT_RcvMtaLadnRptSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[idx].cmdCurrentOpt != AT_CMD_CLADN_SET) {
        AT_WARN_LOG("AT_RcvMtaLadnRptSetCnf : Current Option is not CLADN_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(idx);
    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(idx, result);
    return VOS_OK;
}


VOS_VOID AT_ConvertNwDnnToString(VOS_UINT8 *inputDnn, VOS_UINT8 inputLen, VOS_UINT8 *outputDnn, VOS_UINT8 outputLen)
{
    VOS_UINT32 i = 0;
    VOS_UINT32 j = 0;
    VOS_UINT8 labelLen;

    if ((inputLen < PS_MAX_APN_LEN) || (outputLen < PS_MAX_APN_LEN)) {
        return;
    }

    labelLen = inputDnn[1]; /* 第1个label的长度 */
    for (i = 1; (i < inputDnn[0]) && (i < PS_MAX_APN_LEN); i++) {
        /* 从第1个有效字符开始检查 */
        if (j < labelLen) {
            /* 拷贝labelLen个字符 */
            outputDnn[i] = inputDnn[i + 1];
            j++; /* 有效字符数增1 */
        } else {
            outputDnn[i] = '.';             /* 拷贝一个Label完毕后，追加一个'.'号 */
            j            = 0;               /* 开始下一个Label的长度累计 */
            labelLen     = inputDnn[i + 1]; /* 取下一个label的长度 */
        }
    }
    outputDnn[0] = (VOS_UINT8)(i - 1); /* 字符串的长度 */
}


VOS_UINT32 AT_RcvMtaLadnInfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_QryLadnInfoCnf *qryCnf           = VOS_NULL_PTR;
    VOS_UINT8              idx              = 0;
    VOS_UINT16             length           = 0;
    VOS_UINT8              allowedDnnNum    = 0;
    VOS_UINT8              nonAllowedDnnNum = 0;
    VOS_UINT8              allowedDnnList[MTA_AT_MAX_LADN_DNN_NUM][PS_MAX_APN_LEN];
    VOS_UINT8              nonAllowedDnnList[MTA_AT_MAX_LADN_DNN_NUM][PS_MAX_APN_LEN];
    VOS_UINT32             i;
    errno_t                memResult;
    VOS_UINT8              strLen;
    VOS_UINT8              srcStr[PS_MAX_APN_LEN + 1]; /* 网络格式DNN */
    VOS_UINT8              destStr[PS_MAX_APN_LEN];    /* 字符串格式DNN */

    rcvMsg = (AT_MTA_Msg *)msg;
    qryCnf = (MTA_AT_QryLadnInfoCnf *)rcvMsg->content;

    (VOS_VOID)memset_s(srcStr, sizeof(srcStr), 0x00, sizeof(srcStr));
    (VOS_VOID)memset_s(destStr, sizeof(destStr), 0x00, sizeof(destStr));

    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &idx) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLadnInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(idx)) {
        AT_WARN_LOG("AT_RcvMtaLadnInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    if ((g_atClientTab[idx].cmdCurrentOpt != AT_CMD_CLADN_QRY)) {
        AT_WARN_LOG("AT_RcvMtaLadnInfoQryCnf : Current Option is not qry ladn info.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(idx);

    /* 如果查询结果失败，直接返回ERROR */
    if (qryCnf->result != MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(idx, AT_ERROR);
    } else {
        for (i = 0; i < MTA_AT_MAX_LADN_DNN_NUM; i++) {
            (VOS_VOID)memset_s(allowedDnnList[i], sizeof(allowedDnnList[i]), 0x00, sizeof(allowedDnnList[i]));
            (VOS_VOID)memset_s(nonAllowedDnnList[i], sizeof(nonAllowedDnnList[i]), 0x00, sizeof(nonAllowedDnnList[i]));
        }

        for (i = 0; i < AT_MIN(qryCnf->ladnDnnNum, MTA_AT_MAX_LADN_DNN_NUM); i++) {
            /* 转换网络格式DNN为字符串 */
            srcStr[0] = qryCnf->ladnList[i].dnn.length;
            strLen    = AT_MIN(PS_MAX_APN_LEN, srcStr[0]);
            if (strLen == 0) {
                continue;
            }
            memResult = memcpy_s(&srcStr[1], sizeof(srcStr) - 1, qryCnf->ladnList[i].dnn.value, strLen);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(srcStr) - 1, strLen);
            AT_ConvertNwDnnToString(srcStr, PS_MAX_APN_LEN, destStr, PS_MAX_APN_LEN);

            /* 根据DNN是否可用，将DNN分别拷贝至可用和不可用DNN的数组，并统计数量 */
            if (qryCnf->ladnList[i].availFlg == VOS_TRUE) {
                memResult = memcpy_s(allowedDnnList[allowedDnnNum], sizeof(allowedDnnList[allowedDnnNum]), &destStr[1],
                                     PS_MAX_APN_LEN - 1);
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(allowedDnnList[allowedDnnNum]), PS_MAX_APN_LEN - 1);
                allowedDnnNum++;
            } else {
                memResult = memcpy_s(nonAllowedDnnList[nonAllowedDnnNum], sizeof(nonAllowedDnnList[nonAllowedDnnNum]),
                                     &destStr[1], PS_MAX_APN_LEN - 1);
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nonAllowedDnnList[nonAllowedDnnNum]), PS_MAX_APN_LEN - 1);
                nonAllowedDnnNum++;
            }
            (VOS_VOID)memset_s(destStr, sizeof(destStr), 0x00, sizeof(destStr));
        }
        /* ^CLADN : */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[idx].cmdElement->cmdName);
        /* <n>, */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", qryCnf->rptFlg);
        /* <allowed_ladn_dnn_num>,<allowed_dnn_list> */
        length = AT_PrintLadnDnn(allowedDnnNum, allowedDnnList, length);

        /* , */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");

        /* <nonallowed_ladn_dnn_num>,<noallowed_dnn_list> */
        length = AT_PrintLadnDnn(nonAllowedDnnNum, nonAllowedDnnList, length);

        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(idx, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLadnInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg         *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_LadnInfoInd *ladnInfoInd      = VOS_NULL_PTR;
    VOS_UINT8           idx              = 0;
    VOS_UINT16          length           = 0;
    VOS_UINT8           allowedDnnNum    = 0;
    VOS_UINT8           nonAllowedDnnNum = 0;
    VOS_UINT8           allowedDnnList[MTA_AT_MAX_LADN_DNN_NUM][PS_MAX_APN_LEN];
    VOS_UINT8           nonAllowedDnnList[MTA_AT_MAX_LADN_DNN_NUM][PS_MAX_APN_LEN];
    VOS_UINT32          i;
    errno_t             memResult;
    VOS_UINT8           strLen;
    VOS_UINT8           srcStr[PS_MAX_APN_LEN + 1]; /* 网络格式DNN */
    VOS_UINT8           destStr[PS_MAX_APN_LEN];    /* 字符串格式DNN */

    rcvMsg      = (AT_MTA_Msg *)msg;
    ladnInfoInd = (MTA_AT_LadnInfoInd *)rcvMsg->content;

    (VOS_VOID)memset_s(srcStr, sizeof(srcStr), 0x00, sizeof(srcStr));
    (VOS_VOID)memset_s(destStr, sizeof(destStr), 0x00, sizeof(destStr));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &idx) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLadnInfoInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    for (i = 0; i < MTA_AT_MAX_LADN_DNN_NUM; i++) {
        (VOS_VOID)memset_s(allowedDnnList[i], sizeof(allowedDnnList[i]), 0x00, sizeof(allowedDnnList[i]));
        (VOS_VOID)memset_s(nonAllowedDnnList[i], sizeof(nonAllowedDnnList[i]), 0x00, sizeof(nonAllowedDnnList[i]));
    }

    for (i = 0; i < AT_MIN(ladnInfoInd->ladnDnnNum, MTA_AT_MAX_LADN_DNN_NUM); i++) {
        /* 转换网络格式DNN为字符串 */
        srcStr[0] = ladnInfoInd->ladnList[i].dnn.length;
        strLen    = AT_MIN(PS_MAX_APN_LEN, srcStr[0]);
        if (strLen == 0) {
            continue;
        }
        memResult = memcpy_s(&srcStr[1], sizeof(srcStr) - 1, ladnInfoInd->ladnList[i].dnn.value, strLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(srcStr) - 1, strLen);
        AT_ConvertNwDnnToString(srcStr, PS_MAX_APN_LEN, destStr, PS_MAX_APN_LEN);

        /* 根据DNN是否可用，将DNN分别拷贝至可用和不可用DNN的数组，并统计数量 */
        if (ladnInfoInd->ladnList[i].availFlg == VOS_TRUE) {
            memResult = memcpy_s(allowedDnnList[allowedDnnNum], sizeof(allowedDnnList[allowedDnnNum]), &destStr[1],
                                 PS_MAX_APN_LEN - 1);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(allowedDnnList[allowedDnnNum]), PS_MAX_APN_LEN - 1);
            allowedDnnNum++;
        } else {
            memResult = memcpy_s(nonAllowedDnnList[nonAllowedDnnNum], sizeof(nonAllowedDnnList[nonAllowedDnnNum]),
                                 &destStr[1], PS_MAX_APN_LEN - 1);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nonAllowedDnnList[nonAllowedDnnNum]), PS_MAX_APN_LEN - 1);
            nonAllowedDnnNum++;
        }
        (VOS_VOID)memset_s(destStr, sizeof(destStr), 0x00, sizeof(destStr));
    }
    /* ^CLADNU : */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^CLADNU: ", g_atCrLf);

    /* <allowed_ladn_dnn_num>,<allowed_dnn_list> */
    length = AT_PrintLadnDnn(allowedDnnNum, allowedDnnList, length);

    /* , */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",");

    /* <nonallowed_ladn_dnn_num>,<noallowed_dnn_list> */
    length = AT_PrintLadnDnn(nonAllowedDnnNum, nonAllowedDnnList, length);

    /* /r/n */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(idx, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT16 AT_PrintLadnDnn(VOS_UINT8 dnnNum, VOS_UINT8 dnnList[][PS_MAX_APN_LEN], VOS_UINT16 length)
{
    VOS_UINT32 i;
    VOS_UINT16 len;

    len = length;

    if (dnnNum == 0) {
        /* ,<ladn_dnn_num> */
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%d,", dnnNum);
        /* ,<dnn_list> */
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "\"\"");
    } else {
        /* ,<ladn_dnn_num> */
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "%d,", dnnNum);
        /* ,<dnn_list> */
        len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + len, "\"");

        for (i = 0; i < dnnNum; i++) {
            if (i != dnnNum - 1) {
                len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + len, "%s;", dnnList[i]);
            } else if (i == dnnNum - 1) {
                len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + len, "%s\"", dnnList[i]);
            }
        }
    }
    return len;
}


VOS_UINT32 AT_RcvMtaPendingNssaiInd(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg = VOS_NULL_PTR;
    MTA_AT_PendingNssaiInd *pendingNssaiInd = VOS_NULL_PTR;
    VOS_UINT32              strLen = 0;
    VOS_CHAR                strNssai[AT_EVT_MULTI_S_NSSAI_LEN] = {0};
    VOS_UINT16              length = 0;
    VOS_UINT8               idx = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    pendingNssaiInd = (MTA_AT_PendingNssaiInd *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &idx) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPendingNssaiInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_ConvertMultiSNssaiToString(AT_MIN((VOS_UINT8)pendingNssaiInd->pendingNssaiNum, MTA_AT_MAX_PENDING_NSSAI_NUM),
                                  &pendingNssaiInd->sNssaiList[0], strNssai, sizeof(strNssai), &strLen);

    /* ^CPENDINGNSSAI : */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d", g_atCrLf, g_atStringTab[AT_STRING_CPENDINGNSSAI].text,
        strLen);

    if (strLen != 0) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",\"%s\"", strNssai);
    }

    /* /r/n */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(idx, g_atSndCodeAddress, length);

    return VOS_OK;
}

#endif

#if ((FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON))

VOS_UINT32 AT_RcvMtaHsrcellInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg            *mtaMsg         = VOS_NULL_PTR;
    MTA_AT_HsrcellInfoInd *hsrcellInfoInd = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT8              atIndex;

    length         = 0;
    atIndex        = 0;
    mtaMsg         = (AT_MTA_Msg *)msg;
    hsrcellInfoInd = (MTA_AT_HsrcellInfoInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaHsrcellInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (hsrcellInfoInd->rat == MTA_AT_HSRCELLINFO_RAT_NR) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^HSRCELLINFO: %d,%d,%d,%d,%d%s", g_atCrLf, hsrcellInfoInd->rat,
            hsrcellInfoInd->highSpeedFlg, hsrcellInfoInd->rsrp, hsrcellInfoInd->cellIdLowBit,
            hsrcellInfoInd->cellIdHighBit, g_atCrLf);
    } else {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^HSRCELLINFO: %d,%d,%d,%d%s", g_atCrLf, hsrcellInfoInd->rat,
            hsrcellInfoInd->highSpeedFlg, hsrcellInfoInd->rsrp, hsrcellInfoInd->cellIdLowBit, g_atCrLf);
    }

    At_SendResultData(atIndex, g_atSndCodeAddress, length);
    return VOS_OK;
}
#endif /* (FEATURE_EDA_SUPPORT == FEATURE_ON) && (FEATURE_MBB_HSRCELLINFO == FEATURE_ON) */


VOS_UINT32 AT_RcvDmsLowWaterMarkEvent(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify *subscripEvent = VOS_NULL_PTR;
    AT_UartCtx                          *uartCtx         = VOS_NULL_PTR;

    subscripEvent = (struct DMS_PORT_SubscripEventNotify *)msg;
    uartCtx         = AT_GetUartCtxAddr();

    /* 端口索引检查 */
    if (subscripEvent->clientId >= AT_CLIENT_ID_BUTT) {
        AT_ERR_LOG1("AT_RcvDmsLowWaterMarkEvent : AT INDEX NOT FOUND!", subscripEvent->clientId);
        return VOS_ERR;
    }

    /* 只处理UART端口的低水线消息 */
    if (AT_CheckHsUartUser(subscripEvent->clientId) != VOS_TRUE) {
        AT_ERR_LOG1("AT_RcvDmsLowWaterMarkEvent : not hsuart user!", subscripEvent->clientId);
        return VOS_ERR;
    }

    /* TX低水线处理 */
    if (uartCtx->wmLowFunc != VOS_NULL_PTR) {
        uartCtx->wmLowFunc(subscripEvent->clientId);
    }

    return VOS_OK;
}

PS_IFACE_IdUint8 AT_GetNdisIfaceIdByDmsPortId(DMS_PortIdUint16 portId)
{
    switch (portId) {
        case DMS_PORT_NCM_DATA:
            return PS_IFACE_ID_NDIS0;
#if (FEATURE_MULTI_NCM == FEATURE_ON)
        case DMS_PORT_NCM_DATA1:
            return PS_IFACE_ID_NDIS1;
        case DMS_PORT_NCM_DATA2:
            return PS_IFACE_ID_NDIS2;
        case DMS_PORT_NCM_DATA3:
            return PS_IFACE_ID_NDIS3;
#endif
        default:
            return PS_IFACE_ID_NDIS0;
    }
}


VOS_UINT32 AT_RcvDmsNcmConnBreakEvent(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify *subscripEvent = VOS_NULL_PTR;
    struct DMS_NcmConnBreakInfo *ncmConnBreakInfo = VOS_NULL_PTR;
    VOS_UINT32 ifaceId;
    AT_ClientIdUint16 clientId;

    subscripEvent = (struct DMS_PORT_SubscripEventNotify *)msg;
    ncmConnBreakInfo = (struct DMS_NcmConnBreakInfo *)subscripEvent->data;
    clientId = subscripEvent->clientId;

    /* NDIS ctrl口发送网卡断开事件 */
    if (clientId != AT_CLIENT_ID_NDIS) {
        return VOS_ERR;
    }

    ifaceId = AT_GetNdisIfaceIdByDmsPortId(ncmConnBreakInfo->ncmDataPortId);

    AT_PR_LOGH("iface id %d", ifaceId);
    AT_ProcNdisDisconnectInd(ifaceId, VOS_TRUE);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDmsUsbDisconnectEvent(struct MsgCB *msg)
{
    AT_ProcNdisDisconnectInd(PS_IFACE_ID_BUTT, VOS_FALSE);

    /* modem端口处理 */
    AT_ModemEnableCB((VOS_UINT8)AT_CLIENT_ID_MODEM, VOS_FALSE);
    AT_DeRegModemPsDataFCPoint((VOS_UINT8)AT_CLIENT_ID_MODEM,
                        AT_GET_RABID_FROM_EXRABID(g_atClientTab[AT_CLIENT_ID_MODEM].exPsRabId));

    /* 从记录表中删除USB相关的client ID */
    AT_RmUsedClientIdFromTab(AT_CLIENT_ID_PCUI);
    AT_RmUsedClientIdFromTab(AT_CLIENT_ID_CTRL);
    AT_RmUsedClientIdFromTab(AT_CLIENT_ID_NDIS);
    AT_RmUsedClientIdFromTab(AT_CLIENT_ID_MODEM);
    AT_RmUsedClientIdFromTab(AT_CLIENT_ID_PCUI2);

    return VOS_OK;
}


VOS_UINT32 AT_RcvDmsEscapeSequenceEvent(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify *subscripEvent = VOS_NULL_PTR;
    DMS_PortIdUint16                     portId;
    DMS_PortModeUint8                    mode;
    DMS_PortDataModeUint8                dataMode;

    subscripEvent = (struct DMS_PORT_SubscripEventNotify *)msg;

    if (subscripEvent->clientId >= AT_CLIENT_ID_BUTT) {
        AT_ERR_LOG1("AT_RcvDmsEscapeSequenceEvent : AT INDEX abnormal!", subscripEvent->clientId);
        return VOS_ERR;
    }

    portId = AT_GetDmsPortIdByClientId(subscripEvent->clientId);

    mode     = DMS_PORT_GetMode(portId);
    dataMode = DMS_PORT_GetDataMode(portId);

    if (mode != DMS_PORT_MODE_ONLINE_DATA)
    {
        AT_WARN_LOG2("AT_RcvDmsEscapeSequenceEvent abnormal mode!", subscripEvent->clientId, mode);
        return VOS_ERR;
    }

    switch (dataMode)
    {
#if (FEATURE_AT_HSUART == FEATURE_ON)
        case DMS_PORT_DATA_PPP_RAW:
        case DMS_PORT_DATA_PPP:
            AT_ProcDmsEscSeqInPppDataMode(subscripEvent->clientId);
            break;
#endif

        case DMS_PORT_DATA_OM:
            DMS_PORT_ResumeCmdMode(portId);
            break;

        default:
            AT_ERR_LOG1("AT_RcvDmsEscapeSequenceEvent : abnormal data mode !", dataMode);
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvDmsDtrDeassertedEvent(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify *subscripEvent = VOS_NULL_PTR;
    DMS_PortModeUint8                    mode;
    DMS_PortDataModeUint8                dataMode;
    DMS_PortIdUint16                     portId;

    subscripEvent = (struct DMS_PORT_SubscripEventNotify *)msg;

    /* 端口索引检查 */
    if (subscripEvent->clientId >= AT_CLIENT_ID_BUTT) {
        AT_ERR_LOG1("AT_RcvDmsDtrDeassertedEvent : AT INDEX NOT FOUND!", subscripEvent->clientId);
        return VOS_ERR;
    }

    portId   = AT_GetDmsPortIdByClientId(subscripEvent->clientId);
    mode     = DMS_PORT_GetMode(portId);
    dataMode = DMS_PORT_GetDataMode(portId);

    /* 数传模式响应dtr 变低处理 */
    if ((mode == DMS_PORT_MODE_ONLINE_DATA) || (mode == DMS_PORT_MODE_ONLINE_CMD)) {
        switch (dataMode) {
            case DMS_PORT_DATA_PPP:
                AT_ProcDtrDeassertedIndPppDataMode(subscripEvent->clientId);
                break;

            case DMS_PORT_DATA_PPP_RAW:
                AT_ProcDtrDeassertedIndRawDataMode(subscripEvent->clientId);
                break;

            default:
                AT_WARN_LOG("AT_RcvDmsDtrDeassertedEvent: DataMode Wrong!");
                return VOS_ERR;
        }
    }

    return VOS_OK;
}

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)

VOS_UINT32 AT_RcvDmsUartInitEvent(struct MsgCB *msg)
{

    VOS_UINT32 i = 0;
    VOS_UINT8 indexNum;

    for (i = 0; i < MAX_CMUX_PORT_NUM; i++) {
        indexNum = (VOS_UINT8)AT_CLIENT_ID_CMUXAT + i;
        /*修改DLC0 DISC无法断开拨号的问题*/
        g_atClientTab[indexNum].dataId = indexNum;
        g_atClientTab[indexNum].ctrlId = indexNum;
    }
    /*恢复AT与UART的通道*/
    AT_UartInitPort();

    g_cmuxOpen = VOS_FALSE;

    AT_PR_LOGI("CMUX init uart success");

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaNvRefreshNtf(struct MsgCB *msg)
{
    /* 更新Phone随卡NV */
    AT_ReadIpv6AddrTestModeCfgNV();
    AT_ReadPrivacyFilterCfgNv();
    AT_InitStk();
    AT_ReadWasCapabilityNV();
    AT_ReadAgpsNv();

    /* 更新MBB随卡NV */
    AT_ReadSmsNV();
    AT_ReadRoamCapaNV();
    AT_ReadIpv6CapabilityNV();
    AT_ReadGasCapabilityNV();
    AT_ReadApnCustomFormatCfgNV();

    return VOS_OK;
}
