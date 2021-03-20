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

#include "AtCmdMiscProc.h"
#include "securec.h"
#include "AtSndMsg.h"
#include "ATCmdProc.h"
#include "AtDataProc.h"

#include "AtInit.h"

#include "taf_drv_agent.h"
#include "AtSetParaCmd.h"

#include "AtMtCommFun.h"
#include "at_mta_interface.h"

#include "at_mdrv_interface.h" /* AT_CheckFactoryNv */



LOCAL VOS_VOID AT_ProcDlMcsRsp(MTA_AT_McsQryCnf *mtaAtQryMcsCnf, VOS_UINT8 indexNum, VOS_UINT16 *length);

LOCAL VOS_VOID AT_ProcUlMcsRsp(MTA_AT_McsQryCnf *mtaAtQryMcsCnf, VOS_UINT8 indexNum, VOS_UINT16 *length);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
LOCAL VOS_UINT32 AT_CheckNwDeploymentPara(VOS_VOID);
#endif
/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_MISC_PROC_C

#define MCS_RAT_LTE 0
#define MCS_RAT_NR 1
#define HFREQ_INFO_RAT_LTE 6
#define HFREQ_INFO_RAT_NR 7

#define AT_MBMS_PRIORITY_UNICAST 0
#define AT_MBMS_PRIORITY_MBMS 1

#define AT_MBMSINTERESTLIST_PARA_NUM 6
#define AT_MBMSINTERESTLIST_MBMS_PRIORITY 5

#define AT_LCACFG_MAX_PARA_NUM 11
#define AT_LCACFG_MIN_PARA_NUM 1
#define AT_DISABLE_LCACFG_MIN_PARA_NUM 3
#define AT_LCACFG_ENABLE 0
#define AT_LCACFG_CFG_PARA 1
#define AT_LCACFG_BAND_NUM 2

#define AT_FINETIMEREQ_MAX_PARA_NUM 3
#define AT_FINETIMEREQ_MIN_PARA_NUM 1
#define AT_FINETIMEREQ_RAT 2

#define AT_OVERHEATINGCFG_TYPE 0
#define AT_OVERHEATINGCFG_FLAG 1
#define AT_OVERHEATINGCFG_SETPARAMFLAG 2
#define AT_OVERHEATINGCFG_PARA1 3
#define AT_OVERHEATINGCFG_PARA2 4
#define AT_OVERHEATINGCFG_PARA3 5
#define AT_OVERHEATINGCFG_PARA4 6
#define AT_OVERHEATINGCFG_PARA5 7
#define AT_OVERHEATINGCFG_PARA6 8
#define AT_OVERHEATINGCFG_PARA7 9
#define AT_OVERHEATINGCFG_PARA8 10
#define AT_OVERHEATINGCFG_PARA9 11
#define AT_OVERHEATINGCFG_PARA10 12

#define AT_LL2COMCFG_MAX_PARA_NUM 4
#define AT_LL2COMCFG_MIN_PARA_NUM 2
#define AT_LL2COMCFG_CMDTYPE 0
#define AT_LL2COMCFG_PARA1 1
#define AT_LL2COMCFG_PARA2 2
#define AT_LL2COMCFG_PARA3 3

#define AT_NL2COMCFG_CMDTYPE 0
#define AT_NL2COMCFG_PARA1 1
#define AT_NL2COMCFG_PARA2 2
#define AT_NL2COMCFG_PARA3 3

#define AT_TIMESET_PARA_NUM 3
#define AT_TIMESET_DATE 0
#define AT_TIMESET_TIME 1
#define AT_TIMESET_ZONE 2

#define AT_PHYCOMCFG_MAX_PARA_NUM 5
#define AT_PHYCOMCFG_MIN_PARA_NUM 3
#define AT_PHYCOMCFG_PARA1 2
#define AT_PHYCOMCFG_PARA2 3
#define AT_PHYCOMCFG_PARA3 4

#define AT_MIPIWREX_PARA_NUM 6
#define AT_MIPIWREX_SECONDARY_ID 2
#define AT_MIPIWREX_REG_ADDR 3
#define AT_MIPIWREX_BYTE_CNT 4
#define AT_MIPIWREX_VALUE 5

#define AT_MIPIRDEX_SECONDARY_ID 2
#define AT_MIPIRDEX_REG_ADDR 3
#define AT_MIPIRDEX_BYTE_CNT 4
#define AT_MIPIRDEX_SPEED_TYPE 5

#define AT_NRFREQLOCK_PARA_NUM_BAND 3
#define AT_NRFREQLOCK_PARA_NUM_FREQ 4
#define AT_NRFREQLOCK_PARA_NUM_CELL 5
#define AT_NRFREQLOCK_BAND 2
#define AT_NRFREQLOCK_NRARFCN 3
#define AT_NRFREQLOCK_PHYCELL_ID 4

#define AT_CCLK_TIME 0


#define AT_SUBCLFSPARAM_MAX_PARA_NUM_FUNC_OFF 6
#define AT_SUBCLFSPARAM_MAX_PARA_NUM 9
#define AT_SUBCLFSPARAM_MIN_PARA_NUM 6
#define AT_SUBCLFSPARAM_PARA_VERSIONID_MAX_LEN 10
#define AT_SUBCLFSPARAM_SEQ 0
#define AT_SUBCLFSPARAM_VERSIONID 1
#define AT_SUBCLFSPARAM_TYPE 2
#define AT_SUBCLFSPARAM_ACTIVEFLG 3
#define AT_SUBCLFSPARAM_PROBARATE 4
#define AT_SUBCLFSPARAM_CLFSGROUPNUM 5

#define AT_UE_CAP_MAX_PARA_NUM 6
#define AT_UE_CAP_MIN_PARA_NUM 4
#define AT_UE_CAP_PARA1_INDEX 3
#define AT_UE_CAP_PARA_REQ 0
#define AT_UE_CAP_PARA_ENDFLAG 1
#define AT_UE_CAP_CAPPARAMNUM 2
#define AT_UE_CAP_PARA1 3

#define AT_NRRCCAPCFG_MIN_PARA_NUM 1
#define AT_NRRCCAPCFG_MAX_PARA_NUM 11
#define AT_NRRCCAPCFG_NRRCCFGMODE 0
#define AT_NRRCCAPCFG_NRRCCFGNETMODE_VALID_VALUE0 0
#define AT_NRRCCAPCFG_NRRCCFGNETMODE_VALID_VALUE1 1

#define AT_MBMSCMD_MAX_PARA_NUM 3
#define AT_SAMPLE_MAX_PARA_NUM 4
#define AT_PSSCENE_MAX_PARA_NUM 2
#define AT_ACTPDPSTUB_PARA_NUM 2
#define AT_DOSYSEVENT_PARA_NUM 1
#define AT_DOSIGMASK_PARA_NUM 1
#define AT_VTRLQUALRPT_PARA_NUM 2
#define AT_ECCCFG_PARA_NUM 2
#define AT_PSEUDBTS_PARA_NUM 2
#define AT_NRPWRCTRL_PARA_NUM 2

#define AT_PDPSTU_CLIENT_ID 0
#define AT_PCUIPSCALL_FLAG_INDEX 0
#define AT_CTRLPSCALL_FLAG_INDEX 1
#define AT_PCUI2PSCALL_FLAG_INDEX 2

#define AT_DOSYSEVENT_DO_SYS_EVENT 0
#define AT_DOSYSEVENT_DO_SYS_EVENT_VALID 10

#define AT_DOSIGMASK_DO_SIG_MASK 0
#define AT_DOSIGMASK_DO_SIG_MASK_VALID 10

#define AT_ETDFLG_IS_0_REG_ADDR_MAX 31
#define AT_ETDFLG_IS_1_REG_ADDR_MAX 255
#define AT_ETDFLG_IS_0_BYTE_CNT_VALID 1
#define AT_ETDFLG_IS_1_BYTE_CNT_VALID1 1
#define AT_ETDFLG_IS_1_BYTE_CNT_VALID2 2

#define AT_LRRCUECAPPARAMSET_PARA1_INDEX 3
#define AT_NRRCUECAPPARAMSET_PARA1_INDEX 3

#define AT_SIMLOCKDATAWRITEEX_LAYER_FOUR_VALID_VALUE 4
#define AT_SIMLOCKDATAWRITEEX_LAYER_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_ULINDEX_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_ULTOTAL_MAX_VALID_VALUE 255
#define AT_SIMLOCKDATAWRITEEX_STR_MIN_LEN 6
#define AT_SIMLOCKOTADATAWRITE_STR_MIN_LEN 6

#define AT_COMMPOS_LENGTH 4
#define AT_FIRST_COMMPOS 0
#define AT_SECOND_COMMPOS 1
#define AT_THIRD_COMMPOS 2
#define AT_FOURTH_COMMPOS 3

#define AT_PHONEPHYNUM_PARA_NUM 3
#define AT_PHONEPHYNUM_PARA_TYPE_NUM 0
#define AT_PHONEPHYNUM_PARA_RSA_NUM 1
#define AT_PHONEPHYNUM_PARA_HMAC_NUM 2

#define AT_SPECIAL_CMD_MAX_LEN 23

#define AT_SIMLOCKDATAREADEX_VALID_PARA_NUM 2

#define AT_MBMSCMD_PARAM1 1
#define AT_MBMSCMD_PARAM2 2

#define AT_CONST_NUM_9 9
#define AT_CONST_NUM_11 11

#define AT_NRCACELLRPTCFG_PARA_NUM 1
#define AT_NRNWCAPRPTCFG_PARA_NUM 2
#define AT_NRNWCAPQRY_PARA_NUM 1
#define AT_NRNWCAPRPTQRY_PARA_NUM 1

#define AT_NRBANDBLOCKLIST_PARA_MIN_NUM 1
#define AT_NRBANDBLOCKLIST_PARA_MAX_NUM 3
#define AT_NRBANDBLOCKLIST_PARA_OPTION 0    /* NRBANDBLOCKLIST的第一个参数OPTION */
#define AT_NRBANDBLOCKLIST_PARA_BAND_NUM 1  /* NRBANDBLOCKLIST的第二个参数BAND_NUM */
#define AT_NRBANDBLOCKLIST_PARA_BAND_LIST 2 /* NRBANDBLOCKLIST的第三个参数BAND_LIST */

#define AT_SAMPLE_PARA_REQTYPE 0     /* SAMPLE的第一个参数REQTYPE */
#define AT_SAMPLE_PARA_TEMPRANGE 1   /* SAMPLE的第一个参数REQTYPE */
#define AT_SAMPLE_PARA_PPMOFFSET 2   /* SAMPLE的第一个参数REQTYPE */
#define AT_SAMPLE_PARA_TIMEOFFSET 3  /* SAMPLE的第一个参数REQTYPE */

#define AT_MTA_COEXPARA_RESERVED_LEN 2
#define AT_FINETIME_FAIL_NUM 2
#define AT_CONFIG_ENABLE_PARA_NUM 1
#define AT_UE_CAP_PARA_ENDFLAG_VALID_VALUE 1

#define AT_NVCHK_PARA_MAX_NUM 1
#define AT_NVCHK_PARA_PARA0 0       /* NVCHK的第一个参数PARA0 */
#define AT_NVCHK_CRC_MAX_NUM 3

#define AT_LOGENABLE_PARA_MAX_NUM 1
#define AT_LOGENABLE_PARA_ENABLE 0  /* LOGENABLE的第一个参数ENABLE */

#define AT_MTA_PARA_INFO_CLFS_GROUP_NUM 6 /* 结构体AT_MTA_ParaInfo的第一个成员clfsGroupNum*/
#define AT_MTA_PARA_INFO_DATA_LENGTH 7 /* 结构体AT_MTA_ParaInfo的第二个成员dataLength*/
#define AT_MTA_PARA_INFO_CLFS_DATA 8 /* 结构体AT_MTA_ParaInfo的第三个成员clfsData*/
#define AT_MTA_PARA_INFO_CLFS_DATA_LEN 4

#define AT_AFCCLKINFO_PARA_A0_MANTISSA 0
#define AT_AFCCLKINFO_PARA_A0_EXPONENT 1
#define AT_AFCCLKINFO_PARA_A1_MANTISSA 2
#define AT_AFCCLKINFO_PARA_A1_EXPONENT 3
#define AT_AFCCLKINFO_PARA_A2_MANTISSA 0
#define AT_AFCCLKINFO_PARA_A2_EXPONENT 1
#define AT_AFCCLKINFO_PARA_A3_MANTISSA 2
#define AT_AFCCLKINFO_PARA_A3_EXPONENT 3

#define AT_NRBANDBLOCKLIST_PARA_BAND_VALID_VALUE1 1
#define AT_NRBANDBLOCKLIST_PARA_BAND_VALID_VALUE2 2
#define AT_SIMLOCK_DATA_LEN 4
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
#define AT_NRPHY_COMCFG_MAX_PARA_NUM 16
#define AT_NRPHY_COMCFG_MIN_PARA_NUM 2

#define AT_NRPOWERSAVINGCFG_MIN_PARA_NUM 1
#define AT_NRPOWERSAVINGCFG_MAX_PARA_NUM 24
#define AT_NRPOWERSAVINGCFG_CMDTYPE 0
#endif

enum AT_NrPowerSavingParaIndex {
    AT_NRPOWERSAVING_PARA0 = 0,
    AT_NRPOWERSAVING_PARA1,
    AT_NRPOWERSAVING_PARA2,
    AT_NRPOWERSAVING_PARA3,
    AT_NRPOWERSAVING_PARA4,
    AT_NRPOWERSAVING_PARA5,
    AT_NRPOWERSAVING_PARA6,
    AT_NRPOWERSAVING_PARA7,
    AT_NRPOWERSAVING_PARA8,
    AT_NRPOWERSAVING_PARA9,
    AT_NRPOWERSAVING_PARA10,
    AT_NRPOWERSAVING_PARA11,
    AT_NRPOWERSAVING_PARA12,
    AT_NRPOWERSAVING_PARA13,
    AT_NRPOWERSAVING_PARA14,
    AT_NRPOWERSAVING_PARA15,
    AT_NRPOWERSAVING_PARA16,
    AT_NRPOWERSAVING_PARA17,
    AT_NRPOWERSAVING_PARA18,
    AT_NRPOWERSAVING_PARA19,
    AT_NRPOWERSAVING_PARA20,
    AT_NRPOWERSAVING_PARA21,
    AT_NRPOWERSAVING_PARA22,
    AT_NRPOWERSAVING_PARA_BUTT
};
typedef VOS_UINT32 AT_NrPowerSavingParaIndex;

static const AT_SpecialCmdFuncTbl g_ApModemSpecialCmdFucTab[] = {
    /* AT^IDENTIFYEND= 处理函数 */
    { "AT^IDENTIFYEND=", AT_HandleIdentifyEndCmd },
    /* AT^IDENTIFYOTAEND= 处理函数 */
    { "AT^IDENTIFYOTAEND=", AT_HandleIdentifyEndCmd },
    /* AT^PHONEPHYNUM= 处理函数 */
    { "AT^PHONEPHYNUM=", AT_HandlePhonePhynumCmd },
};


VOS_UINT32 AT_SetActiveModem(VOS_UINT8 indexNum)
{
    TAF_NV_DsdsActiveModemMode mode;
    AT_MTA_ModemCapUpdateReq   atMtaModemCapUpdate;

    (VOS_VOID)memset_s(&atMtaModemCapUpdate, (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq), 0x00,
                       (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq));

    /* 参数个数检查 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&mode, sizeof(mode), 0x00, sizeof(mode));
    mode.activeModem = (TAF_NV_ActiveModemModeUint8)g_atParaList[0].paraValue;

    /* 写NV, 返回AT_OK */
    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_DSDS_ACTIVE_MODEM_MODE, (VOS_UINT8 *)&mode, sizeof(mode)) != NV_OK) {
        AT_ERR_LOG("AT_SetActiveModem(): NV_ITEM_DSDS_ACTIVE_MODEM_MODE NV Write Fail!");
        return AT_ERROR;
    }

    /* 更新了NV，通知AT->MTA->RRM，进行底层平台能力更新 */
    /* AT发送至MTA的消息结构赋值 */
    atMtaModemCapUpdate.modemCapUpdateType = AT_MTA_MODEM_CAP_UPDATE_TYPE_ACTIVE_MODEM;

    /* 发送消息给C核处理 */
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_MODEM_CAP_UPDATE_REQ,
                               (VOS_UINT8 *)&atMtaModemCapUpdate, (VOS_UINT32)sizeof(AT_MTA_ModemCapUpdateReq),
                               I0_UEPS_PID_MTA) != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetActiveModem(): Snd MTA Req Failed!");

        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MODEM_CAP_UPDATE_SET;

    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_LTE == FEATURE_ON)
#if (FEATURE_LTE_MBMS == FEATURE_ON)

VOS_UINT32 AT_SetMBMSServiceOptPara(VOS_UINT8 indexNum)
{
    AT_MTA_MbmsServiceOptionSetReq mBMSServiceOption;
    VOS_UINT32                     rst;

    (VOS_VOID)memset_s(&mBMSServiceOption, sizeof(mBMSServiceOption), 0x00, sizeof(mBMSServiceOption));

    /* 参数为空 */
    if (g_atParaList[1].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = atAuc2ul(g_atParaList[1].para, (VOS_UINT16)g_atParaList[1].paraLen, &g_atParaList[1].paraValue);

    if (rst != AT_SUCCESS) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[1].paraValue) {
        case 0:
            mBMSServiceOption.cfg = AT_MTA_CFG_DISABLE;
            break;

        case 1:
            mBMSServiceOption.cfg = AT_MTA_CFG_ENABLE;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送跨核消息到C核, 设置使能或者去使能MBMS服务特性 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_MBMS_SERVICE_OPTION_SET_REQ, &mBMSServiceOption, sizeof(mBMSServiceOption),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMBMSServicePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_SERVICE_OPTION_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetMBMSServiceStatePara(VOS_UINT8 indexNum)
{
    AT_MTA_MbmsServiceStateSetReq mBMSServiceState;
    VOS_UINT32                    rst;

    (VOS_VOID)memset_s(&mBMSServiceState, sizeof(mBMSServiceState), 0x00, sizeof(mBMSServiceState));

    switch (g_atParaList[0].paraValue) {
        case AT_MBMS_ACTIVATE_TYPE:
            mBMSServiceState.stateSet = AT_MTA_MBMS_SERVICE_STATE_SET_ACTIVE;
            break;

        case AT_MBMS_DEACTIVATE_TYPE:
            mBMSServiceState.stateSet = AT_MTA_MBMS_SERVICE_STATE_SET_DEACTIVE;
            break;

        case AT_MBMS_DEACTIVATE_ALL_TYPE:
            mBMSServiceState.stateSet = AT_MTA_MBMS_SERVICE_STATE_SET_DEACTIVE_ALL;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    if (mBMSServiceState.stateSet != AT_MTA_MBMS_SERVICE_STATE_SET_DEACTIVE_ALL) {
        /* 参数为空 */
        if (g_atParaList[AT_MBMSCMD_PARAM1].paraLen == 0 || g_atParaList[AT_MBMSCMD_PARAM2].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        rst = atAuc2ul(g_atParaList[AT_MBMSCMD_PARAM1].para, (VOS_UINT16)g_atParaList[AT_MBMSCMD_PARAM1].paraLen,
                       &g_atParaList[AT_MBMSCMD_PARAM1].paraValue);

        if (rst != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 参数长度不正确 */
        if (((AT_MBMS_TMGI_MAX_LENGTH - 1) != g_atParaList[AT_MBMSCMD_PARAM2].paraLen) &&
            (g_atParaList[AT_MBMSCMD_PARAM2].paraLen != AT_MBMS_TMGI_MAX_LENGTH)) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* <AreaID> */
        mBMSServiceState.areaId = g_atParaList[AT_MBMSCMD_PARAM1].paraValue;

        /* <TMGI>:MBMS Service ID */
        if (At_Auc2ul(g_atParaList[AT_MBMSCMD_PARAM2].para, AT_MBMS_SERVICE_ID_LENGTH,
                      &mBMSServiceState.tmgi.mbmsSerId) == AT_FAILURE) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* <TMGI>:Mcc */
        if (At_String2Hex(&g_atParaList[AT_MBMSCMD_PARAM2].para[AT_MBMS_SERVICE_ID_LENGTH], AT_MBMS_MCC_LENGTH,
                          &mBMSServiceState.tmgi.plmnId.mcc) == AT_FAILURE) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* <TMGI>:Mnc */
        if (At_String2Hex(&g_atParaList[AT_MBMSCMD_PARAM2].para[AT_MBMS_SERVICE_ID_AND_MCC_LENGTH],
                          g_atParaList[AT_MBMSCMD_PARAM2].paraLen - AT_MBMS_SERVICE_ID_AND_MCC_LENGTH,
                          &mBMSServiceState.tmgi.plmnId.mnc) == AT_FAILURE) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if ((AT_MBMS_TMGI_MAX_LENGTH - 1) == g_atParaList[AT_MBMSCMD_PARAM2].paraLen) {
            mBMSServiceState.tmgi.plmnId.mnc |= 0x0F00;
        }
    }

    /* 发送跨核消息到C核, 设置使能或者去使能MBMS服务特性 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_MBMS_SERVICE_STATE_SET_REQ, &mBMSServiceState, sizeof(mBMSServiceState),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMBMSServiceStatePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_SERVICE_STATE_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetMBMSPreferencePara(VOS_UINT8 indexNum)
{
    AT_MTA_MbmsPreferenceSetReq mBMSCastMode;
    VOS_UINT32                  rst;

    (VOS_VOID)memset_s(&mBMSCastMode, sizeof(mBMSCastMode), 0x00, sizeof(mBMSCastMode));

    /* 参数为空 */
    if (g_atParaList[1].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = atAuc2ul(g_atParaList[1].para, (VOS_UINT16)g_atParaList[1].paraLen, &g_atParaList[1].paraValue);

    if (rst != AT_SUCCESS) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[1].paraValue) {
        case 0:
            mBMSCastMode.castMode = AT_MTA_MBMS_CAST_MODE_UNICAST;
            break;

        case 1:
            mBMSCastMode.castMode = AT_MTA_MBMS_CAST_MODE_MULTICAST;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送跨核消息到C核, 设置MBMS广播模式 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_MBMS_PREFERENCE_SET_REQ, &mBMSCastMode, sizeof(mBMSCastMode),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMBMSPreferencePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_PREFERENCE_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetMBMSCMDPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_MBMSCMD_MAX_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<cmd> */
    switch (g_atParaList[0].paraValue) {
        /* <cmd> equal "MBMS_SERVICE_ENABLER" */
        case AT_MBMS_SERVICE_ENABLER_TYPE:
            rst = AT_SetMBMSServiceOptPara(indexNum);
            break;
        /* <cmd> equal "ACTIVATE", "DEACTIVATE" or "DEACTIVATE_ALL" */
        case AT_MBMS_ACTIVATE_TYPE:
        case AT_MBMS_DEACTIVATE_TYPE:
        case AT_MBMS_DEACTIVATE_ALL_TYPE:
            rst = AT_SetMBMSServiceStatePara(indexNum);
            break;
        /* <cmd> equal "MBMS_PREFERENCE" */
        case AT_MBMS_PREFERENCE_TYPE:
            rst = AT_SetMBMSPreferencePara(indexNum);
            break;
        /* <cmd> equal "SIB16_GET_NETWORK_TIME" */
        case AT_MBMS_SIB16_GET_NETWORK_TIME_TYPE:
            rst = AT_QryMBMSSib16NetworkTimePara(indexNum);
            break;
        /* <cmd> equal "BSSI_SIGNAL_LEVEL" */
        case AT_MBMS_BSSI_SIGNAL_LEVEL_TYPE:
            rst = AT_QryMBMSBssiSignalLevelPara(indexNum);
            break;
        /* <cmd> equal "NETWORK_INFORMATION" */
        case AT_MBMS_NETWORK_INFORMATION_TYPE:
            rst = AT_QryMBMSNetworkInfoPara(indexNum);
            break;
        /* <cmd> equal "MODEM_STATUS" */
        case AT_MBMS_MODEM_STATUS_TYPE:
            rst = AT_QryMBMSModemStatusPara(indexNum);
            break;

        default:
            rst = AT_CME_INCORRECT_PARAMETERS;
            break;
    }

    return rst;
}


VOS_UINT32 AT_SetMBMSEVPara(VOS_UINT8 indexNum)
{
    AT_MTA_MbmsUnsolicitedCfgSetReq mBMSUnsolicitedCfg;
    VOS_UINT32                      rst;

    (VOS_VOID)memset_s(&mBMSUnsolicitedCfg, sizeof(mBMSUnsolicitedCfg), 0x00, sizeof(mBMSUnsolicitedCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[0].paraValue) {
        case 0:
            mBMSUnsolicitedCfg.cfg = AT_MTA_CFG_DISABLE;
            break;

        case 1:
            mBMSUnsolicitedCfg.cfg = AT_MTA_CFG_ENABLE;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送跨核消息到C核, 设置MBMS主动上报配置 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_MBMS_UNSOLICITED_CFG_SET_REQ, &mBMSUnsolicitedCfg,
                                 sizeof(mBMSUnsolicitedCfg), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMBMSEVPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_UNSOLICITED_CFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetMBMSInterestListPara(VOS_UINT8 indexNum)
{
    AT_MTA_MbmsInterestlistSetReq mBMSInterestList;
    VOS_UINT32                    rst;
    VOS_UINT8                     interestNum;

    (VOS_VOID)memset_s(&mBMSInterestList, sizeof(mBMSInterestList), 0x00, sizeof(mBMSInterestList));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_MBMSINTERESTLIST_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[AT_MBMSINTERESTLIST_MBMS_PRIORITY].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[AT_MBMSINTERESTLIST_MBMS_PRIORITY].paraValue) {
        case AT_MBMS_PRIORITY_UNICAST:
            mBMSInterestList.mbmsPriority = AT_MTA_MBMS_PRIORITY_UNICAST;
            break;

        case AT_MBMS_PRIORITY_MBMS:
            mBMSInterestList.mbmsPriority = AT_MTA_MBMS_PRIORITY_MBMS;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 频点列表赋值 */
    for (interestNum = 0; interestNum < AT_MTA_INTEREST_FREQ_MAX_NUM; interestNum++) {
        if (g_atParaList[interestNum].paraLen != 0) {
            rst = atAuc2ul(g_atParaList[interestNum].para, (VOS_UINT16)g_atParaList[interestNum].paraLen,
                           &g_atParaList[interestNum].paraValue);

            if (rst != AT_SUCCESS) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            mBMSInterestList.freqList[interestNum] = g_atParaList[interestNum].paraValue;
        } else {
            /* 默认值为无效值0xFFFFFFFF */
            mBMSInterestList.freqList[interestNum] = 0xFFFFFFFF;
        }
    }

    /* 发送跨核消息到C核, 设置MBMS主动上报配置 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_INTEREST_LIST_SET_REQ, &mBMSInterestList, sizeof(mBMSInterestList),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMBMSInterestListPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_INTERESTLIST_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryMBMSSib16NetworkTimePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送跨核消息到C核, 查询SIB16网络时间请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SIB16_NETWORK_TIME_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMBMSSib16NetworkTimePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_SIB16_NETWORK_TIME_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryMBMSBssiSignalLevelPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送跨核消息到C核, 查询BSSI信号强度请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_BSSI_SIGNAL_LEVEL_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMBMSBssiSignalLevelPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_BSSI_SIGNAL_LEVEL_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryMBMSNetworkInfoPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送跨核消息到C核, 查询网络信息请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_NETWORK_INFO_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMBMSNetworkInfoPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_NETWORK_INFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryMBMSModemStatusPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 发送跨核消息到C核, 查询eMBMS功能状态请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_EMBMS_STATUS_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMBMSModemStatusPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EMBMS_STATUS_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryMBMSCmdPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MTA 发送查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_MBMS_AVL_SERVICE_LIST_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryMBMSCmdPara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MBMS_AVL_SERVICE_LIST_QRY;

    return AT_WAIT_ASYNC_RETURN;
}



VOS_UINT32 At_TestMBMSCMDPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress,
        "%s: (\"MBMS_SERVICE_ENABLER\",\"ACTIVATE\",\"DEACTIVATE\",\"DEACTIVATE_ALL\",\"MBMS_PREFERENCE\",\"SIB16_GET_NETWORK_TIME\",\"BSSI_SIGNAL_LEVEL\",\"NETWORK_INFORMATION\",\"MODEM_STATUS\")",
        g_parseContext[indexNum].cmdElement->cmdName);

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}


VOS_UINT32 AT_RcvMtaMBMSServiceOptSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceOptSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceOptSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_SERVICE_OPTION_SET) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceOptSetCnf : Current Option is not AT_CMD_MBMS_SERVICE_OPTION_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSServiceStateSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceStateSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceStateSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_SERVICE_STATE_SET) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceStateSetCnf : Current Option is not AT_CMD_MBMS_SERVICE_STATE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSPreferenceSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSPreferenceSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSPreferenceSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_PREFERENCE_SET) {
        AT_WARN_LOG("AT_RcvMtaMBMSPreferenceSetCnf : Current Option is not AT_CMD_MBMS_PREFERENCE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSSib16NetworkTimeQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                        *rcvMsg = VOS_NULL_PTR;
    MTA_AT_MbmsSib16NetworkTimeQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT64                        *utc    = VOS_NULL_PTR;
    VOS_UINT8                          utcInfo[AT_MBMS_UTC_MAX_LENGTH + 1];
    VOS_UINT32                         result;
    VOS_UINT8                          indexNum;
    VOS_INT32                          bufLen;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_MbmsSib16NetworkTimeQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;
    (VOS_VOID)memset_s(utcInfo, sizeof(utcInfo), 0x00, sizeof(utcInfo));

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSSib16NetworkTimeQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSSib16NetworkTimeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_SIB16_NETWORK_TIME_QRY) {
        AT_WARN_LOG("AT_RcvMtaMBMSSib16NetworkTimeQryCnf : Current Option is not AT_CMD_MBMS_SIB16_NETWORK_TIME_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        utc    = (VOS_UINT64 *)mtaCnf->utc;
        bufLen = snprintf_s((VOS_CHAR *)utcInfo, AT_MBMS_UTC_MAX_LENGTH + 1, AT_MBMS_UTC_MAX_LENGTH, "%llu", *utc);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, AT_MBMS_UTC_MAX_LENGTH + 1, AT_MBMS_UTC_MAX_LENGTH);
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName, utcInfo);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSBssiSignalLevelQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_MbmsBssiSignalLevelQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                        result;
    VOS_UINT8                         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_MbmsBssiSignalLevelQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSBssiSignalLevelQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSBssiSignalLevelQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_BSSI_SIGNAL_LEVEL_QRY) {
        AT_WARN_LOG("AT_RcvMtaMBMSBssiSignalLevelQryCnf : Current Option is not AT_CMD_MBMS_BSSI_SIGNAL_LEVEL_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)mtaCnf->bssiLevel);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSNetworkInfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                   *rcvMsg = VOS_NULL_PTR;
    MTA_AT_MbmsNetworkInfoQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                    result;
    VOS_UINT8                     indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_MbmsNetworkInfoQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSNetworkInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSNetworkInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_NETWORK_INFO_QRY) {
        AT_WARN_LOG("AT_RcvMtaMBMSNetworkInfoQryCnf : Current Option is not AT_CMD_MBMS_NETWORK_INFO_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                (VOS_INT32)mtaCnf->cellId);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSModemStatusQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_EmbmsStatusQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_EmbmsStatusQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSModemStatusQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSModemStatusQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EMBMS_STATUS_QRY) {
        AT_WARN_LOG("AT_RcvMtaMBMSModemStatusQryCnf : Current Option is not AT_CMD_EMBMS_STATUS_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                (VOS_INT32)mtaCnf->status);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSEVSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSEVSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSEVSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_UNSOLICITED_CFG_SET) {
        AT_WARN_LOG("AT_RcvMtaMBMSEVSetCnf : Current Option is not AT_CMD_MBMS_UNSOLICITED_CFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSServiceEventInd(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg   = VOS_NULL_PTR;
    MTA_AT_MbmsServiceEventInd *mtaAtInd = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;
    VOS_UINT16                  length;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaAtInd = (MTA_AT_MbmsServiceEventInd *)rcvMsg->content;
    indexNum = 0;
    length   = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSServiceEventInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s %d%s", g_atCrLf, g_atStringTab[AT_STRING_MBMSEV].text,
        mtaAtInd->event, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMBMSInterestListSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSInterestListSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSInterestListSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_INTERESTLIST_SET) {
        AT_WARN_LOG("AT_RcvMtaMBMSInterestListSetCnf : Current Option is not AT_CMD_MBMS_INTERESTLIST_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_ReportMBMSCmdQryCnf(MTA_AT_MbmsAvlServiceListQryCnf *mtaCnf, VOS_UINT8 indexNum)
{
    VOS_UINT32 listNum;
    VOS_UINT16 length;
    VOS_UINT8  serviceID[AT_MBMS_SERVICE_ID_LENGTH + 1];

    length = 0;

    for (listNum = 0; listNum < TAF_MIN(mtaCnf->avlServiceNum, AT_MTA_MBMS_AVL_SERVICE_MAX_NUM); listNum++) {
        /* 获得MBMS Service ID字符串形式 */
        (VOS_VOID)memset_s(serviceID, sizeof(serviceID), 0x00, sizeof(serviceID));
        At_ul2Auc(mtaCnf->avlServices[listNum].tmgi.mbmsSerId, AT_MBMS_SERVICE_ID_LENGTH, serviceID);

        /* ^MBMSCMD: <AreaID>,<TMGI>:MBMS Service ID */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%s", g_parseContext[indexNum].cmdElement->cmdName,
            (VOS_INT32)mtaCnf->avlServices[listNum].areaId, serviceID);

        /* <TMGI>:Mcc */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%x%x%x",
            (mtaCnf->avlServices[listNum].tmgi.plmnId.mcc & 0x0f00) >> 8,
            (mtaCnf->avlServices[listNum].tmgi.plmnId.mcc & 0xf0) >> 4,
            (mtaCnf->avlServices[listNum].tmgi.plmnId.mcc & 0x0f));
        /* <TMGI>:Mnc */
        if ((mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0x0f00) == 0x0f00) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%x%x",
                (mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0xf0) >> 4,
                (mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0x0f));
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%x%x%x",
                (mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0x0f00) >> 8,
                (mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0xf0) >> 4,
                (mtaCnf->avlServices[listNum].tmgi.plmnId.mnc & 0x0f));
        }

        if (mtaCnf->avlServices[listNum].opSessionId == VOS_TRUE) {
            /* <SessionID> */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", (VOS_INT32)mtaCnf->avlServices[listNum].sessionId);
        }

        if (listNum != (mtaCnf->avlServiceNum - 1)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = length;
    return;
}


VOS_UINT32 AT_RcvMtaMBMSCmdQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                      *rcvMsg = VOS_NULL_PTR;
    MTA_AT_MbmsAvlServiceListQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                       result;
    VOS_UINT8                        indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_MbmsAvlServiceListQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMBMSCmdQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMBMSCmdQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MBMS_AVL_SERVICE_LIST_QRY) {
        AT_WARN_LOG("AT_RcvMtaMBMSCmdQryCnf : Current Option is not AT_CMD_MBMS_AVL_SERVICE_LIST_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        AT_ReportMBMSCmdQryCnf(mtaCnf, indexNum);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_SetLteLowPowerPara(VOS_UINT8 indexNum)
{
    AT_MTA_LowPowerConsumptionSetReq powerConsumption;
    VOS_UINT32                       rst;

    (VOS_VOID)memset_s(&powerConsumption, sizeof(powerConsumption), 0x00, sizeof(powerConsumption));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[0].paraValue) {
        case 0:
            powerConsumption.lteLowPowerFlg = AT_MTA_LTE_LOW_POWER_NORMAL;
            break;

        case 1:
            powerConsumption.lteLowPowerFlg = AT_MTA_LTE_LOW_POWER_LOW;
            break;

        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送跨核消息到C核, 设置低功耗 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_LOW_POWER_SET_REQ, (VOS_UINT8 *)&powerConsumption,
                                 sizeof(powerConsumption), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetLteLowPowerPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTE_LOW_POWER_CONSUMPTION_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_INT32 AT_GetIsmCoexParaValue(VOS_UINT8 *begain, VOS_UINT8 **ppEnd)
{
    VOS_UINT32 total    = 0;
    VOS_INT32  rstTotal = 0;
    VOS_UINT32 rst;
    VOS_UINT32 flag = 0;
    VOS_UINT8 *end  = VOS_NULL_PTR;

    end = begain;

    while ((*end != ' ') && (*end != '\0')) {
        end++;
    }

    if (*begain == '-') {
        flag = 1;
        begain++;
    }

    rst = atAuc2ul(begain, (VOS_UINT16)(end - begain), &total);

    if (rst != AT_SUCCESS) {
        rstTotal = AT_COEX_INVALID;
    } else {
        *ppEnd   = (end + 1);
        rstTotal = (VOS_INT32)(flag ? (0 - total) : total);
    }

    return rstTotal;
}


VOS_UINT32 AT_CheckIsmCoexParaValue(VOS_INT32 val, VOS_UINT32 paraNum)
{
    VOS_UINT32 rst = AT_SUCCESS;

    switch (paraNum) {
        case AT_COEX_PARA_COEX_ENABLE:
            if ((val < AT_COEX_PARA_COEX_ENABLE_MIN) || (val > AT_COEX_PARA_COEX_ENABLE_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        case AT_COEX_PARA_TX_BEGIN:
            if ((val < AT_COEX_PARA_TX_BEGIN_MIN) || (val > AT_COEX_PARA_TX_BEGIN_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        case AT_COEX_PARA_TX_END:
            if ((val < AT_COEX_PARA_TX_END_MIN) || (val > AT_COEX_PARA_TX_END_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        case AT_COEX_PARA_TX_POWER:
            if ((val < AT_COEX_PARA_TX_POWER_MIN) || (val > AT_COEX_PARA_TX_POWER_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        case AT_COEX_PARA_RX_BEGIN:
            if ((val < AT_COEX_PARA_RX_BEGIN_MIN) || (val > AT_COEX_PARA_RX_BEGIN_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        case AT_COEX_PARA_RX_END:
            if ((val < AT_COEX_PARA_RX_END_MIN) || (val > AT_COEX_PARA_RX_END_MAX)) {
                rst = AT_FAILURE;
            }
            break;
        default:
            rst = AT_FAILURE;
            break;
    }

    return rst;
}


VOS_VOID AT_SetL4AIsmCoexParaValue(AT_MTA_LteWifiCoexSetReq ismCoex, L4A_ISMCOEX_Req *reqToL4A, VOS_UINT8 indexNum)
{
    VOS_UINT32 i;

    reqToL4A->ctrl.clientId = g_atClientTab[indexNum].clientId;
    reqToL4A->ctrl.opId = 0;
    reqToL4A->ctrl.pid  = WUEPS_PID_AT;

    for (i = 0; i < AT_MTA_ISMCOEX_BANDWIDTH_NUM; i++) {
        reqToL4A->coex[i].flag    = (VOS_UINT32)ismCoex.coexPara[i].cfg;
        reqToL4A->coex[i].txBegin = (VOS_UINT32)ismCoex.coexPara[i].txBegin;
        reqToL4A->coex[i].txEnd   = (VOS_UINT32)ismCoex.coexPara[i].txEnd;
        reqToL4A->coex[i].txPower = (VOS_INT32)ismCoex.coexPara[i].txPower;
        reqToL4A->coex[i].rxBegin = (VOS_UINT32)ismCoex.coexPara[i].rxBegin;
        reqToL4A->coex[i].rxEnd   = (VOS_UINT32)ismCoex.coexPara[i].rxEnd;
    }

    return;
}


VOS_UINT32 AT_SetIsmCoexPara(VOS_UINT8 indexNum)
{
    AT_MTA_LteWifiCoexSetReq ismCoex;
    L4A_ISMCOEX_Req          reqToL4A = {0};
    VOS_UINT32               rst, retTemp;
    VOS_UINT32               i, j;
    VOS_INT32                ret;
    VOS_UINT16              *val  = VOS_NULL_PTR; /* 将要存储的值指针 */
    VOS_UINT8               *cur  = VOS_NULL_PTR; /* 解析字符串时的当前指针 */
    VOS_UINT8               *para = VOS_NULL_PTR; /* 参数字符串头指针 */

    (VOS_VOID)memset_s(&ismCoex, sizeof(ismCoex), 0x00, sizeof(ismCoex));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数 */
    if (g_atParaIndex != AT_MTA_ISMCOEX_BANDWIDTH_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    for (i = 0; i < AT_MTA_ISMCOEX_BANDWIDTH_NUM; i++) {
        cur                            = g_atParaList[i].para;
        ismCoex.coexPara[i].coexBwType = (AT_MTA_CoexBwTypeUint16)i;
        val                            = &(ismCoex.coexPara[i].cfg);

        for (j = 0; j < sizeof(AT_MTA_CoexPara) / sizeof(VOS_UINT16) - AT_MTA_COEXPARA_RESERVED_LEN; j++) {
            para = cur;
            ret  = AT_GetIsmCoexParaValue(para, &cur);

            if (ret == AT_COEX_INVALID) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            if (AT_CheckIsmCoexParaValue(ret, j) == AT_FAILURE) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            *val = (VOS_UINT16)ret;
            val++;
        }
    }

    ismCoex.coexParaNum  = AT_MTA_ISMCOEX_BANDWIDTH_NUM;
    ismCoex.coexParaSize = sizeof(ismCoex.coexPara);

    AT_SetL4AIsmCoexParaValue(ismCoex, &reqToL4A, indexNum);

    /* 发送消息到L4A */
    retTemp = atSendL4aDataMsg(g_atClientTab[indexNum].clientId, I0_MSP_L4_L4A_PID, ID_MSG_L4A_ISMCOEXSET_REQ,
                               (VOS_UINT8 *)(&reqToL4A), sizeof(reqToL4A));

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_WIFI_COEX_SET_REQ, (VOS_UINT8 *)&ismCoex, sizeof(ismCoex),
                                 I0_UEPS_PID_MTA);
    if (retTemp != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetIsmCoexPara: atSendDataMsg fail.");
    }

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetIsmCoexPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTE_WIFI_COEX_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryIsmCoexPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询ISMCOEX列表请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_WIFI_COEX_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryIsmCoexPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTE_WIFI_COEX_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaLteLowPowerSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         idx;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    idx  = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &idx) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLteLowPowerSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(idx)) {
        AT_WARN_LOG("AT_RcvMtaLteLowPowerSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[idx].cmdCurrentOpt != AT_CMD_LTE_LOW_POWER_CONSUMPTION_SET) {
        AT_WARN_LOG("AT_RcvMtaLteLowPowerSetCnf : Current Option is not AT_CMD_LTE_LOW_POWER_CONSUMPTION_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(idx);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(idx, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaIsmCoexSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTE_WIFI_COEX_SET) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexSetCnf : Current Option is not AT_CMD_LTE_WIFI_COEX_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (cnf->result != VOS_OK) {
        result = AT_ERROR;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaIsmCoexQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_LteWifiCoexQryCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT32                i;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_LteWifiCoexQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTE_WIFI_COEX_QRY) {
        AT_WARN_LOG("AT_RcvMtaIsmCoexQryCnf : Current Option is not AT_CMD_LTE_WIFI_COEX_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:", g_parseContext[indexNum].cmdElement->cmdName);

    for (i = 0; i < AT_MTA_ISMCOEX_BANDWIDTH_NUM; i++) {
        g_atSendDataBuff.bufLen +=
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, " %d %d %d %d %d %d,", cnf->coexPara[i].cfg,
                cnf->coexPara[i].txBegin, cnf->coexPara[i].txEnd, cnf->coexPara[i].txPower, cnf->coexPara[i].rxBegin,
                cnf->coexPara[i].rxEnd);
    }

    g_atSendDataBuff.bufLen--;

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_ProLCaCfgPara(AT_MTA_CaCfgSetReq *caCfgReq)
{
    errno_t    memResult;
    VOS_UINT32 paraIndex;
    VOS_UINT32 i;

    /* 如果配置为使能，只需要一个参数 */
    if (g_atParaList[AT_LCACFG_ENABLE].paraValue == AT_CONFIG_ENABLE_PARA_NUM) {
        caCfgReq->caActFlag = (VOS_UINT8)g_atParaList[AT_LCACFG_ENABLE].paraValue;
        return AT_OK;
    }
    /* 如果配置为去使能，至少需要三个参数参数 */
    if ((g_atParaIndex < AT_DISABLE_LCACFG_MIN_PARA_NUM) || (g_atParaList[AT_LCACFG_CFG_PARA].paraLen == 0) ||
        (g_atParaList[AT_LCACFG_BAND_NUM].paraLen == 0)) {
        AT_WARN_LOG("AT_ProLCaCfgPara: para num is error or para len is 0.");
        return AT_ERROR;
    }

    /* band num值和band数量对不上 */
    if ((g_atParaIndex - AT_MTA_BAND_INFO_OFFSET) != g_atParaList[AT_LCACFG_BAND_NUM].paraValue) {
        AT_WARN_LOG("AT_ProLCaCfgPara: para num is error.");
        return AT_ERROR;
    }

    caCfgReq->caActFlag       = (VOS_UINT8)g_atParaList[AT_LCACFG_ENABLE].paraValue;
    caCfgReq->caInfo.caA2Flg  = ((VOS_UINT8)g_atParaList[AT_LCACFG_CFG_PARA].paraValue) & (0x01);
    /* bit位为1位打开A4功能 */
    caCfgReq->caInfo.caA4Flg  = (((VOS_UINT8)g_atParaList[AT_LCACFG_CFG_PARA].paraValue) & (0x02)) >> 1;
    /* bit位为2位打开CQI=0功能 */
    caCfgReq->caInfo.caCqiFlg = (((VOS_UINT8)g_atParaList[AT_LCACFG_CFG_PARA].paraValue) & (0x04)) >> 2;
    caCfgReq->caInfo.bandNum  = (VOS_UINT16)g_atParaList[AT_LCACFG_BAND_NUM].paraValue;

    for (i = 0; i < (VOS_UINT32)TAF_MIN(caCfgReq->caInfo.bandNum, AT_MTA_MAX_BAND_NUM); i++) {
        paraIndex = AT_MTA_BAND_INFO_OFFSET + i;

        if (At_AsciiNum2HexString(g_atParaList[paraIndex].para, &(g_atParaList[paraIndex].paraLen)) == AT_FAILURE) {
            return AT_ERROR;
        }

        if (sizeof(AT_MTA_BandInfo) != g_atParaList[paraIndex].paraLen) {
            AT_WARN_LOG("AT_ProLCaCfgPara: para len is error.");
            return AT_ERROR;
        }

        memResult = memcpy_s(&(caCfgReq->caInfo.bandInfo[i]), (VOS_SIZE_T)sizeof(caCfgReq->caInfo.bandInfo[i]),
                             g_atParaList[paraIndex].para, g_atParaList[paraIndex].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(caCfgReq->caInfo.bandInfo[i]),
                            g_atParaList[paraIndex].paraLen);
    }

    return AT_OK;
}


VOS_UINT32 AT_SetLCaCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_CaCfgSetReq caCfgReq;
    VOS_UINT32         rst;

    (VOS_VOID)memset_s(&caCfgReq, sizeof(caCfgReq), 0x00, sizeof(caCfgReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if ((g_atParaIndex < AT_LCACFG_MIN_PARA_NUM) || (g_atParaIndex > AT_LCACFG_MAX_PARA_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数有效性检查 */
    if (AT_ProLCaCfgPara(&caCfgReq) == AT_ERROR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送跨核消息到C核, 设置低功耗 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_CA_CFG_SET_REQ, (VOS_UINT8 *)&caCfgReq, sizeof(caCfgReq),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetLCaCfgPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTE_CA_CFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaLteCaCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLteCaCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLteCaCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTE_CA_CFG_SET) {
        AT_WARN_LOG("AT_RcvMtaLteCaCfgSetCnf : Current Option is not AT_CMD_LTE_CA_CFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryLCaCellExPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询LCACELLEX列表请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_CA_CELLEX_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryLCaCellExPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTE_CA_CELLEX_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaLteCaCellExQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CaCellInfoCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT32            i;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_CaCellInfoCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLteCaCellExQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLteCaCellExQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTE_CA_CELLEX_QRY) {
        AT_WARN_LOG("AT_RcvMtaLteCaCellExQryCnf : Current Option is not AT_CMD_LTE_CA_CELLEX_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (cnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        cnf->totalCellNum = (cnf->totalCellNum < MTA_AT_CA_MAX_CELL_NUM) ? cnf->totalCellNum : MTA_AT_CA_MAX_CELL_NUM;

        for (i = 0; i < cnf->totalCellNum; i++) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
                "%s: %d,%d,%d,%d,%d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                cnf->cellInfo[i].cellIndex, cnf->cellInfo[i].ulConfigured, cnf->cellInfo[i].dlConfigured,
                cnf->cellInfo[i].actived, cnf->cellInfo[i].laaScellFlg, cnf->cellInfo[i].bandInd,
                cnf->cellInfo[i].bandWidth, cnf->cellInfo[i].earfcn);
            if (i != cnf->totalCellNum - 1) {
                g_atSendDataBuff.bufLen +=
                    (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s", g_atCrLf);
            }
        }
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLteCaCellExInfoNtf(struct MsgCB *msg)
{
    VOS_UINT8             indexNum;
    AT_MTA_Msg           *mtaMsg        = VOS_NULL_PTR;
    MTA_AT_CaCellInfoNtf *caCellInfoNtf = VOS_NULL_PTR;
    VOS_UINT32            i;

    /* 初始化消息变量 */
    indexNum      = 0;
    mtaMsg        = (AT_MTA_Msg *)msg;
    caCellInfoNtf = (MTA_AT_CaCellInfoNtf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLteCaCellExInfoNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    caCellInfoNtf->totalCellNum = TAF_MIN(caCellInfoNtf->totalCellNum, MTA_AT_CA_MAX_CELL_NUM);
    for (i = 0; i < caCellInfoNtf->totalCellNum; i++) {
        g_atSendDataBuff.bufLen +=
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d,%d,%d,%d,%d,%d,%d,%d,%d", g_atCrLf,
                g_atStringTab[AT_STRING_LCACELLEX].text, caCellInfoNtf->totalCellNum,
                caCellInfoNtf->cellInfo[i].cellIndex, caCellInfoNtf->cellInfo[i].ulConfigured,
                caCellInfoNtf->cellInfo[i].dlConfigured, caCellInfoNtf->cellInfo[i].actived,
                caCellInfoNtf->cellInfo[i].laaScellFlg, caCellInfoNtf->cellInfo[i].bandInd,
                caCellInfoNtf->cellInfo[i].bandWidth, caCellInfoNtf->cellInfo[i].earfcn);

        if (i == caCellInfoNtf->totalCellNum - 1) {
            g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s",
                g_atCrLf);
        }
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetLcaCellRptCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_CaCellSetReq cACellType;
    VOS_UINT32          rst;

    (VOS_VOID)memset_s(&cACellType, sizeof(cACellType), 0x00, sizeof(cACellType));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数及长度检查 */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    cACellType.blEnableType = g_atParaList[0].paraValue;

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_CA_CELL_RPT_CFG_SET_REQ, (VOS_UINT8 *)&cACellType, sizeof(cACellType),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetLcaCellRptCfgPara: AT_SetLcaCellRptCfgPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LCACELLRPTCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaCACellSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CaCellSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_CaCellSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCACellSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCACellSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LCACELLRPTCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaCACellSetCnf : Current Option is not AT_CMD_LCACELLRPTCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QryLcaCellRptCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询LCACELLRPTCFG列表请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_LTE_CA_CELL_RPT_CFG_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryLcaCellRptCfgPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LCACELLRPTCFG_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaCACellQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CaCellQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_CaCellQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCACellQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCACellQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LCACELLRPTCFG_QRY) {
        AT_WARN_LOG("AT_RcvMtaCACellQryCnf : Current Option is not AT_CMD_LCACELLRPTCFG_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen +=
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                mtaCnf->blEnableType);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_QryNrCaCellPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询NRCACELL信息 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_NR_CA_CELL_INFO_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNrCaCellPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRCACELL_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetNrCaCellRptCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrCaCellInfoRptCfgSetReq caCellType;
    VOS_UINT32                      rst;

    (VOS_VOID)memset_s(&caCellType, sizeof(caCellType), 0x00, sizeof(caCellType));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数及长度检查 */
    if ((g_atParaIndex != AT_NRCACELLRPTCFG_PARA_NUM) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    caCellType.caCellRptFlg = g_atParaList[0].paraValue;

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NR_CA_CELL_INFO_RPT_CFG_SET_REQ, (VOS_UINT8 *)&caCellType, sizeof(caCellType), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNrCaCellRptCfgPara: AT_SetNrcaCellRptCfgPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRCACELLRPTCFG_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryNrCaCellRptCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NR_CA_CELL_INFO_RPT_CFG_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);
    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryNrCaCellRptCfgPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRCACELLRPTCFG_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaNrCaCellInfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrCaCellInfoCnf *cnf = VOS_NULL_PTR;
    VOS_UINT32 i;
    VOS_UINT32 length = 0;
    AT_RreturnCodeUint32 result;
    VOS_UINT8 indexNum = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    cnf = (MTA_AT_NrCaCellInfoCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRCACELL_QRY) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoQryCnf : Current Option is not AT_CMD_NRCACELL_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    /* 如果消息携带结果失败，返回ERROR */
    if (cnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;

        cnf->cellNum = TAF_MIN(cnf->cellNum, MTA_AT_CA_MAX_CELL_NUM);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
            (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + length,
            "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, cnf->cellNum);

        for (i = 0; i < cnf->cellNum; i++) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + length,
                ",%d,%d,%d,%d", cnf->cellInfo[i].cellIdx, cnf->cellInfo[i].dlConfigured,
                cnf->cellInfo[i].nulConfigured, cnf->cellInfo[i].sulConfigured);
        }
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrCaCellInfoRptCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32 result = AT_OK;
    VOS_UINT8 indexNum = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_ResultCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRCACELLRPTCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgSetCnf : Current Option is not AT_CMD_NRCACELLRPTCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrCaCellInfoRptCfgQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrCaCellInfoRptCfgQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32 result;
    VOS_UINT8 indexNum = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    mtaCnf = (MTA_AT_NrCaCellInfoRptCfgQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRCACELLRPTCFG_QRY) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoRptCfgQryCnf : Current Option is not AT_CMD_NRCACELLRPTCFG_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            mtaCnf->caCellRptFlg);
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrCaCellInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrCaCellInfoInd *cnf = VOS_NULL_PTR;
    VOS_UINT32 i;
    VOS_UINT32 length = 0;
    VOS_UINT8 indexNum = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    cnf = (MTA_AT_NrCaCellInfoInd *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrCaCellInfoInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    cnf->cellNum = TAF_MIN(cnf->cellNum, MTA_AT_CA_MAX_CELL_NUM);
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d",
        g_atCrLf, g_atStringTab[AT_STRING_NRCACELLRPT].text, cnf->cellNum);
    for (i = 0; i < cnf->cellNum; i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d,%d",
            cnf->cellInfo[i].cellIdx, cnf->cellInfo[i].dlConfigured, cnf->cellInfo[i].nulConfigured,
            cnf->cellInfo[i].sulConfigured);
    }
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
    g_atSendDataBuff.bufLen = (VOS_UINT16)length;
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    return VOS_OK;
}


VOS_UINT32 AT_SetNrNwCapRptCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrNwCapInfoRptCfgSetReq rptCfg;
    VOS_UINT32                   rst;

    (VOS_VOID)memset_s(&rptCfg, sizeof(rptCfg), 0x00, sizeof(rptCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数个数及长度检查 */
    if ((g_atParaIndex != AT_NRNWCAPRPTCFG_PARA_NUM) || (g_atParaList[0].paraLen == 0) ||
        (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rptCfg.type = g_atParaList[0].paraValue;
    rptCfg.rptFlg = g_atParaList[1].paraValue;

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NR_NW_CAP_INFO_RPT_CFG_SET_REQ, (VOS_UINT8 *)&rptCfg, sizeof(rptCfg), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNrNwCapRptCfgPara: AT_SetNrNwCapRptCfgPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRNWCAPRPTCFG_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaNrNwCapInfoReportInd(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrNwCapInfoReportInd *rptContent = VOS_NULL_PTR;
    VOS_UINT32 length = 0;
    VOS_UINT8 indexNum = 0;

    rcvMsg = (AT_MTA_Msg *)msg;
    rptContent = (MTA_AT_NrNwCapInfoReportInd *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNwCaCapInfoReportInd : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_NORM_LOG1("AT_RcvMtaNwCaCapInfoReportInd : type is", rptContent->type);
    /* 添加类型判断，避免底层异常上报触发AT上报后唤醒AP */
    if (rptContent->type == AT_MTA_NR_NW_CAP_INFO_SA) {
        g_atSendDataBuff.bufLen = 0;
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_NRNWCAP].text, rptContent->type,
            rptContent->capInfo.commPara.para1, rptContent->capInfo.commPara.para2,
            rptContent->capInfo.commPara.para3, rptContent->capInfo.commPara.para4,
            rptContent->capInfo.commPara.para5, rptContent->capInfo.commPara.para6,
            rptContent->capInfo.commPara.para7, rptContent->capInfo.commPara.para8,
            rptContent->capInfo.commPara.para9, rptContent->capInfo.commPara.para10,
            g_atCrLf);

        g_atSendDataBuff.bufLen = (VOS_UINT16)length;
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrNwCapInfoRptCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *mtaCnf = VOS_NULL_PTR;
    AT_RreturnCodeUint32 result;
    VOS_UINT8 indexNum = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    mtaCnf = (MTA_AT_ResultCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRNWCAPRPTCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgSetCnf : Current Option is not AT_CMD_NRNWCAPRPTCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    result = AT_ConvertMtaResult(mtaCnf->result);
    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_SetNrNwCapQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrNwCapInfoQryReq qryReq;
    VOS_UINT32 result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&qryReq, sizeof(AT_MTA_NrNwCapInfoQryReq), 0x00, sizeof(AT_MTA_NrNwCapInfoQryReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNrNwCapQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个,且第1个参数长度不应为0，否则返回AT_CME_INCORRECT_PARAMETERS */
    if ((g_atParaIndex != AT_NRNWCAPQRY_PARA_NUM) || (g_atParaList[0].paraLen == 0)) {
        AT_WARN_LOG("AT_SetNrNwCapQryPara: At Cmd para num or len error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体，发送消息到MTA */
    qryReq.type = g_atParaList[0].paraValue;
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NR_NW_CAP_INFO_QRY_REQ, (VOS_UINT8 *)&qryReq, sizeof(AT_MTA_NrNwCapInfoQryReq), I0_UEPS_PID_MTA);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRNWCAPQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_SetNrNwCapRptQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrNwCapInfoRptCfgQryReq rptCfgQry;
    VOS_UINT32 rst;

    (VOS_VOID)memset_s(&rptCfgQry, sizeof(rptCfgQry), 0x00, sizeof(rptCfgQry));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数个数及长度检查 */
    if ((g_atParaIndex != AT_NRNWCAPRPTQRY_PARA_NUM) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体，发送消息到MTA */
    rptCfgQry.type = g_atParaList[0].paraValue;
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NR_NW_CAP_INFO_RPT_CFG_QRY_REQ, (VOS_UINT8 *)&rptCfgQry, sizeof(rptCfgQry), I0_UEPS_PID_MTA);
    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNrNwCapRptQryPara: AT_SetNrNwCapRptQryPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRNWCAPRPTQRY_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaNrNwCapInfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrNwCapInfoQryCnf *mtaCnfContent = VOS_NULL_PTR;
    AT_RreturnCodeUint32 result;
    VOS_UINT8 indexNum = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    mtaCnfContent = (MTA_AT_NrNwCapInfoQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRNWCAPQRY_SET) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoQryCnf : Current Option is not AT_CMD_NRNWCAPQRY_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (mtaCnfContent->result == MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, mtaCnfContent->type,
                mtaCnfContent->capInfo.commPara.para1, mtaCnfContent->capInfo.commPara.para2,
                mtaCnfContent->capInfo.commPara.para3, mtaCnfContent->capInfo.commPara.para4,
                mtaCnfContent->capInfo.commPara.para5, mtaCnfContent->capInfo.commPara.para6,
                mtaCnfContent->capInfo.commPara.para7, mtaCnfContent->capInfo.commPara.para8,
                mtaCnfContent->capInfo.commPara.para9, mtaCnfContent->capInfo.commPara.para10);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(mtaCnfContent->result);
    }
    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrNwCapInfoRptCfgQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrNwCapInfoRptCfgQryCnf *mtaCnf = VOS_NULL_PTR;
    AT_RreturnCodeUint32 result;
    VOS_UINT8 indexNum = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    mtaCnf = (MTA_AT_NrNwCapInfoRptCfgQryCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRNWCAPRPTQRY_SET) {
        AT_WARN_LOG("AT_RcvMtaNrNwCapInfoRptCfgQryCnf : Current Option is not AT_CMD_NRNWCAPRPTCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (mtaCnf->result == MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s: %d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, mtaCnf->type, mtaCnf->rptFlg);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(mtaCnf->result);
    }
    /* 输出结果 */
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#endif


VOS_UINT32 AT_SetFineTimeReqPara(VOS_UINT8 indexNum)
{
    AT_MTA_FineTimeSetReq fineTimeType;
    VOS_UINT32            rst;

    (VOS_VOID)memset_s(&fineTimeType, sizeof(fineTimeType), 0x00, sizeof(fineTimeType));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if ((g_atParaIndex < AT_FINETIMEREQ_MIN_PARA_NUM) || (g_atParaIndex > AT_FINETIMEREQ_MAX_PARA_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fineTimeType.fineTimeType = (VOS_UINT8)g_atParaList[0].paraValue;

    if (g_atParaList[1].paraLen == 0) {
        /* <num>参数不下发取默认值 */
        fineTimeType.num = AT_FINE_TIME_DEFAULT_NUM;
    } else {
        fineTimeType.num = (VOS_UINT16)g_atParaList[1].paraValue;
    }

    if (g_atParaList[AT_FINETIMEREQ_RAT].paraLen == 0) {
        /* <rat>参数不下发取默认值LTE，即当前GPS芯片为老平台 */
        fineTimeType.ratMode = MTA_AT_FINE_TIME_RAT_LTE;
    } else {
        fineTimeType.ratMode = (VOS_UINT8)g_atParaList[AT_FINETIMEREQ_RAT].paraValue;
    }

    if (fineTimeType.fineTimeType == AT_MTA_FINETIME_PARA_0) {
        fineTimeType.forceFlag = VOS_TRUE;
    } else {
        fineTimeType.forceFlag = VOS_FALSE;
    }

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_FINE_TIME_SET_REQ, (VOS_UINT8 *)&fineTimeType, sizeof(fineTimeType),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetFineTimeReqPara: AT_SetFineTimeReqPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FINE_TIME_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaFineTimeSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_FineTimeSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_FineTimeSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaFineTimeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaFineTimeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FINE_TIME_SET) {
        AT_WARN_LOG("AT_RcvMtaFineTimeSetCnf : Current Option is not AT_CMD_LTE_FINE_TIME_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_CheckUeCapPara(VOS_VOID)
{
    VOS_UINT32 i;
    /* 参数类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if ((g_atParaIndex < AT_UE_CAP_MIN_PARA_NUM) || (g_atParaIndex > AT_UE_CAP_MAX_PARA_NUM)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数组数检查,命令总的参数个数减去后面三个参数 */
    if ((g_atParaList[AT_UE_CAP_CAPPARAMNUM].paraValue + 3) != g_atParaIndex) {
        AT_WARN_LOG("AT_CheckUeCapPara: Parameters Group Number Wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 必带参数检查 */
    if (g_atParaList[AT_UE_CAP_PARA1].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数长度不为偶数返回错误 */
    for (i = AT_UE_CAP_PARA1; i < g_atParaIndex; i++) {
        if ((g_atParaList[i].paraLen % 2) != 0) {
            AT_WARN_LOG("AT_CheckUeCapPara: Parameters Length Is Not Even.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    return AT_SUCCESS;
}


VOS_UINT32 AT_CheckOverHeatingPara(VOS_VOID)
{
    /* 类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 必带参数检查 */
    if ((g_atParaList[AT_OVERHEATINGCFG_TYPE].paraLen == 0) || (g_atParaList[AT_OVERHEATINGCFG_FLAG].paraLen == 0)
        || (g_atParaList[AT_OVERHEATINGCFG_SETPARAMFLAG].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_FillUeCapPara(AT_MTA_UeCapParamSet *setUeCap, VOS_UINT32 paraLen)
{
    VOS_UINT32 loop;
    VOS_UINT32 moveLen;
    VOS_UINT32 result;

    setUeCap->seq = g_atParaList[AT_UE_CAP_PARA_REQ].paraValue;

    if (g_atParaList[AT_UE_CAP_PARA_ENDFLAG].paraValue == AT_UE_CAP_PARA_ENDFLAG_VALID_VALUE) {
        setUeCap->endFlag = VOS_TRUE;
    } else {
        setUeCap->endFlag = VOS_FALSE;
    }

    moveLen = 0;
    for (loop = AT_UE_CAP_PARA1_INDEX; loop < (VOS_UINT32)TAF_MIN(g_atParaIndex, AT_MAX_PARA_NUMBER); loop++) {
        if (g_atParaList[loop].paraLen > 0) {
            result = At_AsciiString2HexSimple(setUeCap->msg + moveLen, g_atParaList[loop].para,
                                              g_atParaList[loop].paraLen);
            if (result != AT_SUCCESS) {
                return AT_CME_INCORRECT_PARAMETERS;
            }
            moveLen += (VOS_UINT32)g_atParaList[loop].paraLen / 2;
        }
    }

    setUeCap->msgLen = paraLen;

    return AT_SUCCESS;
}


VOS_VOID AT_FillOverHeatingReqParam(AT_MTA_OverHeatingSetReq *overHeatingReq)
{
    overHeatingReq->overHeatingType = g_atParaList[AT_OVERHEATINGCFG_TYPE].paraValue == 1 ? MTA_AT_RAT_MODE_NR :
                                                                                            MTA_AT_RAT_MODE_LTE;
    overHeatingReq->overHeatingFlag = g_atParaList[AT_OVERHEATINGCFG_FLAG].paraValue == 1 ? AT_MTA_OVERHEATING_QUIT :
                                                                                            AT_MTA_OVERHEATING_SET;
    overHeatingReq->setParamFlag    = g_atParaList[AT_OVERHEATINGCFG_SETPARAMFLAG].paraValue;

    if (overHeatingReq->overHeatingFlag == AT_MTA_OVERHEATING_SET) {
        if (overHeatingReq->overHeatingType == MTA_AT_RAT_MODE_LTE) {
            overHeatingReq->param1 = g_atParaList[AT_OVERHEATINGCFG_PARA1].paraValue;
            overHeatingReq->param2 = g_atParaList[AT_OVERHEATINGCFG_PARA2].paraValue;
            overHeatingReq->param3 = g_atParaList[AT_OVERHEATINGCFG_PARA3].paraValue;
            overHeatingReq->param4 = g_atParaList[AT_OVERHEATINGCFG_PARA4].paraValue;
        }

        if (overHeatingReq->overHeatingType == MTA_AT_RAT_MODE_NR) {
            overHeatingReq->param1  = g_atParaList[AT_OVERHEATINGCFG_PARA1].paraValue;
            overHeatingReq->param2  = g_atParaList[AT_OVERHEATINGCFG_PARA2].paraValue;
            overHeatingReq->param3  = g_atParaList[AT_OVERHEATINGCFG_PARA3].paraValue;
            overHeatingReq->param4  = g_atParaList[AT_OVERHEATINGCFG_PARA4].paraValue;
            overHeatingReq->param5  = g_atParaList[AT_OVERHEATINGCFG_PARA5].paraValue;
            overHeatingReq->param6  = g_atParaList[AT_OVERHEATINGCFG_PARA6].paraValue;
            overHeatingReq->param7  = g_atParaList[AT_OVERHEATINGCFG_PARA7].paraValue;
            overHeatingReq->param8  = g_atParaList[AT_OVERHEATINGCFG_PARA8].paraValue;
            overHeatingReq->param9  = g_atParaList[AT_OVERHEATINGCFG_PARA9].paraValue;
            overHeatingReq->param10 = g_atParaList[AT_OVERHEATINGCFG_PARA10].paraValue;
        }
    }
    return;
}


VOS_BOOL AT_CheckModem0(VOS_UINT8 indexNum)
{
    MODEM_ID_ENUM_UINT16 modemId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("Get modem id fail!");
        return VOS_FALSE;
    }

    if (modemId != MODEM_ID_0) {
        AT_ERR_LOG1("Modem is not modem0!", modemId);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_SetOverHeatingCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_OverHeatingSetReq overHeatingReq;
    VOS_UINT32               result;

    (VOS_VOID)memset_s(&overHeatingReq, sizeof(overHeatingReq), 0x00, sizeof(overHeatingReq));

    /* 参数检查 */
    result = AT_CheckOverHeatingPara();
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetOverHeatingCfgPara: AT_CheckOverHeatingPara Failed");
        return result;
    }

    AT_FillOverHeatingReqParam(&overHeatingReq);

    if (overHeatingReq.overHeatingType == MTA_AT_RAT_MODE_NR) {
        /* 仅支持在Modem0上发起 */
        result = AT_CheckModem0(indexNum);
        if (result != VOS_TRUE) {
            AT_ERR_LOG("AT_SetOverHeatingCfgPara: AT_CheckModem0 Failed");
            return AT_ERROR;
        }
    }

    /* 发送跨核消息到C核 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_OVERHEATING_CFG_SET_REQ, (VOS_UINT8 *)&overHeatingReq,
                                    sizeof(overHeatingReq), I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetOverHeatingCfgPara: AT_FillAndSndAppReqMsg failed.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_OVER_HEATING_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetLrrcUeCapPara(VOS_UINT8 indexNum)
{
    AT_MTA_UeCapParamSet *setUeCap = VOS_NULL_PTR;
    VOS_UINT32            msgLen;
    VOS_UINT32            paraLen = 0;
    VOS_UINT32            result;
    VOS_UINT32            i;

    /* 参数检查 */
    result = AT_CheckUeCapPara();
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetLrrcUeCapPara: AT_CheckUeCapPara Failed");
        return result;
    }

    for (i = AT_LRRCUECAPPARAMSET_PARA1_INDEX; i < (VOS_UINT32)TAF_MIN(g_atParaIndex, AT_MAX_PARA_NUMBER); i++) {
        paraLen +=  (VOS_UINT32)g_atParaList[i].paraLen / 2;
    }

    msgLen   = sizeof(AT_MTA_UeCapParamSet) + paraLen;
    setUeCap = (AT_MTA_UeCapParamSet *)PS_MEM_ALLOC(WUEPS_PID_AT, msgLen);
    /* 内存申请失败，返回AT_ERROR */
    if (setUeCap == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SetLrrcUeCapPara:memory alloc fail.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s((VOS_INT8 *)setUeCap, msgLen, 0x00, msgLen);

    /* 填写消息内容 */
    setUeCap->ratMode = AT_MTA_RAT_MODE_LTE;
    result            = AT_FillUeCapPara(setUeCap, paraLen);
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetLrrcUeCapPara: AT_FillUeCapPara Failed");
        PS_MEM_FREE(WUEPS_PID_AT, setUeCap);
        return result;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_LRRC_UE_CAP_PARAM_SET_REQ, (VOS_UINT8 *)setUeCap,
                                    msgLen, I0_UEPS_PID_MTA);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetLrrcUeCapPara: AT_FillAndSndAppReqMsg failed.");
        PS_MEM_FREE(WUEPS_PID_AT, setUeCap);
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LRRC_UE_CAP_SET;
    PS_MEM_FREE(WUEPS_PID_AT, setUeCap);

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetOverHeatingQryParam(VOS_UINT8 indexNum)
{
    AT_MTA_OverHeatingQryReq overHeatingQry;
    VOS_UINT32               result;

    (VOS_VOID)memset_s(&overHeatingQry, sizeof(overHeatingQry), 0x00, sizeof(overHeatingQry));

    /* 类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 必带参数检查 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    overHeatingQry.overHeatingType = g_atParaList[0].paraValue == 1 ? MTA_AT_RAT_MODE_NR : MTA_AT_RAT_MODE_LTE;

    /* 发送跨核消息到C核 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_OVERHEATING_CFG_QRY_REQ, (VOS_UINT8 *)&overHeatingQry,
                                    sizeof(overHeatingQry), I0_UEPS_PID_MTA);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryOverHeating: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_OVER_HEATING_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaOverHeatingSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_OverHeatingSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_OverHeatingSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_OVER_HEATING_SET) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingSetCnf : Current Option is not AT_CMD_LTE_OVER_HEATING_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLrrcUeCapSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_UeCapParamSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_UeCapParamSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLrrcUeCapSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLrrcUeCapSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LRRC_UE_CAP_SET) {
        AT_WARN_LOG("AT_RcvMtaLrrcUeCapSetCnf : Current Option is not AT_CMD_LRRC_UE_CAP_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaOverHeatingQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_OverHeatingQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_OverHeatingQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_OVER_HEATING_QRY) {
        AT_WARN_LOG("AT_RcvMtaOverHeatingQryCnf : Current Option is not AT_CMD_OVER_HEATING_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen +=
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, mtaCnf->overHeatingSupportFlag,
                mtaCnf->lastReportParamFlag, mtaCnf->param1, mtaCnf->param2, mtaCnf->param3, mtaCnf->param4,
                mtaCnf->param5, mtaCnf->param6, mtaCnf->param7, mtaCnf->param8, mtaCnf->param9, mtaCnf->param10);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaSibFineTimeNtf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_SibFineTimeInd *sibFineTime = VOS_NULL_PTR;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    sibFineTime = (MTA_AT_SibFineTimeInd *)rcvMsg->content;
    indexNum    = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSibFineTimeNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSibFineTimeNtf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    if (sibFineTime->result != VOS_OK) {
        /* "^FINETIMEFAIL: 1 */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_FINETIMEFAIL].text, 1, g_atCrLf);
        /* 输出结果 */
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    } else {
        /* "^FINETIMEINFO: */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d,%d,%d,%d,%s,%s", g_atCrLf,
            g_atStringTab[AT_STRING_FINETIMEINFO].text, sibFineTime->rat, sibFineTime->ta, sibFineTime->sinr,
            sibFineTime->state, sibFineTime->utcTime, sibFineTime->utcTimeOffset);

        if (sibFineTime->leapSecondValid == VOS_TRUE) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d%s",
                sibFineTime->leapSecond, g_atCrLf);
        } else {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s",
                g_atCrLf);
        }

        /* 输出结果 */
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLrrcUeCapNtf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg = VOS_NULL_PTR;
    MTA_AT_LrrcUeCapInfoNotify *ueCap  = VOS_NULL_PTR;
    VOS_UINT32                  loop;
    VOS_UINT32                  i;
    VOS_UINT8                   indexNum;

    /* 初始化消息变量 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    ueCap    = (MTA_AT_LrrcUeCapInfoNotify *)rcvMsg->content;
    loop     = AT_MIN(ueCap->msgLen, MTA_AT_UE_CAP_INFO_IND_MAX_LEN);
    indexNum = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLrrcUeCapNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d,\"", g_atCrLf,
            g_atStringTab[AT_STRING_LRRCUECAPINFONTF].text, (ueCap->msgLen * AT_DOUBLE_LENGTH));

    for (i = 0; i < loop; i++) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%02X", ueCap->msg[i]);
    }

    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "\"%s", g_atCrLf);

    /* 输出结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLppFineTimeNtf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_LppFineTimeInd *lppFineTime = VOS_NULL_PTR;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    lppFineTime = (MTA_AT_LppFineTimeInd *)rcvMsg->content;
    indexNum    = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLppFineTimeNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLppFineTimeNtf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    if (lppFineTime->result != VOS_OK) {
        /* "^FINETIMEFAIL: 2 */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_FINETIMEFAIL].text, AT_FINETIME_FAIL_NUM, g_atCrLf);
        /* 输出结果 */
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    } else {
        /* "^SFN: */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_SFN].text, lppFineTime->sysFn, g_atCrLf);
        /* 输出结果 */
        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckL2ComCfgPara(VOS_VOID)
{
    /* 参数个数最少2个，最多4个，其他范围不正确 */
    if ((g_atParaIndex < AT_LL2COMCFG_MIN_PARA_NUM) || (g_atParaIndex > AT_LL2COMCFG_MAX_PARA_NUM)) {
        AT_WARN_LOG("AT_CheckL2ComCfgPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数设置为空，返回错误 */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_CheckL2ComCfgPara: First para length is 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetLL2ComCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_L2ComCfgSetReq setLmacComCfg;
    VOS_UINT32            result;

    (VOS_VOID)memset_s(&setLmacComCfg, sizeof(setLmacComCfg), 0x00, sizeof(setLmacComCfg));

    /* 不是设置命令返回错误 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetLL2ComCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数有效性检查 */
    result = AT_CheckL2ComCfgPara();

    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充内容 */
    setLmacComCfg.cmdType = g_atParaList[AT_LL2COMCFG_CMDTYPE].paraValue;
    setLmacComCfg.para1   = g_atParaList[AT_LL2COMCFG_PARA1].paraValue;
    setLmacComCfg.para2   = g_atParaList[AT_LL2COMCFG_PARA2].paraValue;
    setLmacComCfg.para3   = g_atParaList[AT_LL2COMCFG_PARA3].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_LL2_COM_CFG_SET_REQ, (VOS_UINT8 *)&setLmacComCfg,
                                    sizeof(AT_MTA_L2ComCfgSetReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LL2COMCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_WARN_LOG("AT_SetLL2ComCfgPara: AT_FillAndSndAppReqMsg Failed!");
    return AT_ERROR;
}


VOS_UINT32 AT_SetLL2ComQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_L2ComCfgQryReq l2ComQryReq;
    VOS_UINT32            result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&l2ComQryReq, sizeof(l2ComQryReq), 0x00, sizeof(l2ComQryReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetLL2ComQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetLL2ComQryPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数长度不能为0，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetLL2ComQryPara: FIrst para length is 0.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    l2ComQryReq.cmdType = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_LL2_COM_CFG_QRY_REQ, (VOS_UINT8 *)&l2ComQryReq,
                                    sizeof(AT_MTA_L2ComCfgQryReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LL2COMCFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_WARN_LOG("AT_SetLL2ComQryPara: AT_FillAndSndAppReqMsg Failed!");
    return AT_ERROR;
}


VOS_UINT32 AT_RcvMtaLL2ComCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_L2ComCfgSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_L2ComCfgSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LL2COMCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgSetCnf : Current Option is not AT_CMD_LL2COMCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    result = AT_ConvertMtaResult(mtaCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaLL2ComCfgQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg       = VOS_NULL_PTR;
    MTA_AT_L2ComCfgQryCnf *ll2ComQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg       = (AT_MTA_Msg *)msg;
    ll2ComQryCnf = (MTA_AT_L2ComCfgQryCnf *)rcvMsg->content;
    indexNum     = 0;
    result       = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待^LL2COMQRY命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LL2COMCFG_QRY) {
        AT_WARN_LOG("AT_RcvMtaLL2ComCfgQryCnf : Current Option is not AT_CMD_LL2COMCFG_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (ll2ComQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* ^LL2COMQRY:<cmd_type>,[<para1>,<para2>,<para3>]  */
        g_atSendDataBuff.bufLen +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s: %u,%u,%u,%u",
                g_parseContext[indexNum].cmdElement->cmdName, ll2ComQryCnf->cmdType, ll2ComQryCnf->para1,
                ll2ComQryCnf->para2, ll2ComQryCnf->para3);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(ll2ComQryCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_SetConnectRecovery(VOS_UINT8 indexNum)
{
    VOS_UINT32                          result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_LTE_CONN_RECOVERY_NTF,
                                    VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetConnectRecovery: snd Fail");
        return AT_ERROR;
    }

    /* 与ap、lrrc接口约束，不需要等待结果应答，lrrc会启动保护定时器，保护ap频繁下发场景 */
    return AT_OK;
}
#endif
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_SetNL2ComCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_L2ComCfgSetReq setNmacComCfg;
    VOS_UINT32            result;

    (VOS_VOID)memset_s(&setNmacComCfg, sizeof(setNmacComCfg), 0x00, sizeof(setNmacComCfg));

    /* 不是设置命令返回错误 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNL2ComCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数有效性检查 */
    result = AT_CheckL2ComCfgPara();

    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充内容 */
    setNmacComCfg.cmdType = g_atParaList[AT_NL2COMCFG_CMDTYPE].paraValue;
    setNmacComCfg.para1   = g_atParaList[AT_NL2COMCFG_PARA1].paraValue;
    setNmacComCfg.para2   = g_atParaList[AT_NL2COMCFG_PARA2].paraValue;
    setNmacComCfg.para3   = g_atParaList[AT_NL2COMCFG_PARA3].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NL2_COM_CFG_SET_REQ, (VOS_UINT8 *)&setNmacComCfg,
                                    sizeof(AT_MTA_L2ComCfgSetReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NL2COMCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_WARN_LOG("AT_SetNL2ComCfgPara: AT_FillAndSndAppReqMsg Failed!");
    return AT_ERROR;
}


VOS_UINT32 AT_RcvMtaNL2ComCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_L2ComCfgSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_L2ComCfgSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NL2COMCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgSetCnf : Current Option is not AT_CMD_NL2COMCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    result = AT_ConvertMtaResult(mtaCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetNL2ComQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_L2ComCfgQryReq l2ComQryReq;
    VOS_UINT32            result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&l2ComQryReq, sizeof(l2ComQryReq), 0x00, sizeof(l2ComQryReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNL2ComQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetNL2ComQryPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数长度不能为0，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetNL2ComQryPara: First para length is 0.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    l2ComQryReq.cmdType = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NL2_COM_CFG_QRY_REQ, (VOS_UINT8 *)&l2ComQryReq,
                                    sizeof(AT_MTA_L2ComCfgQryReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NL2COMCFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_WARN_LOG("AT_SetNL2ComQryPara: AT_FillAndSndAppReqMsg Failed!");
    return AT_ERROR;
}


VOS_UINT32 AT_RcvMtaNL2ComCfgQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg       = VOS_NULL_PTR;
    MTA_AT_L2ComCfgQryCnf *nl2ComQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg       = (AT_MTA_Msg *)msg;
    nl2ComQryCnf = (MTA_AT_L2ComCfgQryCnf *)rcvMsg->content;
    indexNum     = 0;
    result       = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待^NL2COMQRY命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NL2COMCFG_QRY) {
        AT_WARN_LOG("AT_RcvMtaNL2ComCfgQryCnf : Current Option is not AT_CMD_NL2COMCFG_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (nl2ComQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* ^NL2COMQRY:<cmd_type>,[<para1>,<para2>,<para3>]  */
        g_atSendDataBuff.bufLen +=
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s: %u,%u,%u,%u",
                g_parseContext[indexNum].cmdElement->cmdName, nl2ComQryCnf->cmdType, nl2ComQryCnf->para1,
                nl2ComQryCnf->para2, nl2ComQryCnf->para3);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(nl2ComQryCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_CheckNPdcpCfgPara(VOS_VOID)
{
    /* 参数个数最少2个，最多4个，其他范围不正确 */
    if ((g_atParaIndex < 2) || (g_atParaIndex > 4)) {
        AT_WARN_LOG("AT_CheckNPdcpCfgPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数设置为空，返回错误 */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_CheckNPdcpCfgPara: First para length is 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetNPdcpSleepThresCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NPdcpSleepThresCfgReq setNPdcpCfg;
    VOS_UINT32                   result;

    (VOS_VOID)memset_s(&setNPdcpCfg, sizeof(setNPdcpCfg), 0x00, sizeof(setNPdcpCfg));

    /* 不是设置命令返回错误 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNPdcpSleepThresCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数有效性检查 */
    result = AT_CheckNPdcpCfgPara();
    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充内容 */
    setNPdcpCfg.operation  = g_atParaList[0].paraValue;
    setNPdcpCfg.sleepThres = g_atParaList[1].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NPDCP_SLEEP_THRES_CFG_REQ, (VOS_UINT8 *)&setNPdcpCfg,
                                    sizeof(AT_MTA_NPdcpSleepThresCfgReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NPDCP_SLEEPTHRES_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_WARN_LOG("AT_SetNPdcpSleepThresCfgPara: AT_FillAndSndAppReqMsg Failed!");
    return AT_ERROR;
}


VOS_UINT32 AT_RcvMtaNPdcpSleepThresCfgCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                   *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NPdcpSleepThresCfgCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32                    result;
    VOS_UINT8                     indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_NPdcpSleepThresCfgCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNPdcpSleepThresCfgCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNPdcpSleepThresCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NPDCP_SLEEPTHRES_SET) {
        AT_WARN_LOG("AT_RcvMtaNPdcpSleepThresCfgCnf : Current Option is not AT_CMD_NL2COMCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result == MTA_AT_RESULT_NO_ERROR) {
        /* ^NPDCPALIVECFG:<result>,<sleepThres> */
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s: %u,%u",
            g_parseContext[indexNum].cmdElement->cmdName, mtaCnf->result, mtaCnf->curThres);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(mtaCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetNrrcUeCapPara(VOS_UINT8 indexNum)
{
    AT_MTA_UeCapParamSet *setUeCap = VOS_NULL_PTR;
    VOS_UINT32            paraLen  = 0;
    VOS_UINT32            msgLen;
    VOS_UINT32            result;
    VOS_UINT32            i;

    /* 参数检查 */
    result = AT_CheckUeCapPara();
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetNrrcUeCapPara: AT_CheckUeCapPara Failed");
        return result;
    }

    /* 仅支持在Modem0上发起 */
    result = AT_CheckModem0(indexNum);
    if (result != VOS_TRUE) {
        AT_ERR_LOG("AT_SetNrrcUeCapPara: AT_CheckModem0 Failed");
        return AT_ERROR;
    }
    for (i = AT_NRRCUECAPPARAMSET_PARA1_INDEX; i < (VOS_UINT32)TAF_MIN(g_atParaIndex, AT_MAX_PARA_NUMBER); i++) {
        paraLen += (VOS_UINT32)g_atParaList[i].paraLen / 2;
    }

    msgLen   = sizeof(AT_MTA_UeCapParamSet) + paraLen;
    setUeCap = (AT_MTA_UeCapParamSet *)PS_MEM_ALLOC(WUEPS_PID_AT, msgLen);

    /* 内存申请失败，返回AT_ERROR */
    if (setUeCap == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SetNrrcUeCapPara:memory alloc fail.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s((VOS_INT8 *)setUeCap, msgLen, 0x00, msgLen);

    /* 填写消息内容 */
    setUeCap->ratMode = AT_MTA_RAT_MODE_NR;
    result            = AT_FillUeCapPara(setUeCap, paraLen);
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetNrrcUeCapPara: AT_FillUeCapPara Failed");
        PS_MEM_FREE(WUEPS_PID_AT, setUeCap);
        return result;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NRRC_UE_CAP_PARAM_SET_REQ, (VOS_UINT8 *)setUeCap, msgLen,
                                    I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNrrcUeCapPara: AT_FillAndSndAppReqMsg failed.");
        PS_MEM_FREE(WUEPS_PID_AT, setUeCap);
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRRC_UE_CAP_SET;
    PS_MEM_FREE(WUEPS_PID_AT, setUeCap);

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaNrrcUeCapSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_UeCapParamSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_UeCapParamSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrrcUeCapSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrrcUeCapSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRRC_UE_CAP_SET) {
        AT_WARN_LOG("AT_RcvMtaNrrcUeCapSetCnf : Current Option is not AT_CMD_NRRC_UE_CAP_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrrcUeCapNtf(struct MsgCB *msg)
{
    AT_MTA_Msg                 *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NrrcUeCapInfoNotify *ueCap  = VOS_NULL_PTR;
    VOS_UINT32                  i;
    VOS_UINT32                  loop;
    VOS_UINT8                   indexNum;

    /* 初始化消息变量 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    ueCap    = (MTA_AT_NrrcUeCapInfoNotify *)rcvMsg->content;
    loop     = AT_MIN(ueCap->msgLen, MTA_AT_UE_CAP_INFO_IND_MAX_LEN);
    indexNum = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrrcUeCapNtf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen +=
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s%s%d,\"", g_atCrLf,
            g_atStringTab[AT_STRING_NRRCUECAPINFONTF].text, (ueCap->msgLen * AT_DOUBLE_LENGTH));

    for (i = 0; i < loop; i++) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%02X", ueCap->msg[i]);
    }

    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "\"%s", g_atCrLf);

    /* 输出结果 */
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

#endif


VOS_UINT32 AT_SetLogEnablePara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_LOGENABLE_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_LOGENABLE_PARA_ENABLE].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryLogEnable(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}



VOS_VOID AT_StopSimlockDataWriteTimer(VOS_UINT8 indexNum)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT32                          timerName;
    VOS_UINT32                          tempIndex;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    tempIndex = (VOS_UINT32)indexNum;
    timerName = AT_SIMLOCKWRITEEX_TIMER;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    if (simlockWriteExCtx != VOS_NULL_PTR) {
        (VOS_VOID)AT_StopRelTimer(timerName, &(simlockWriteExCtx->hSimLockWriteExProtectTimer));
    }

    return;
}

VOS_UINT32 AT_ProcSimlockWriteExData(VOS_UINT8 *simLockData, VOS_UINT16 paraLen)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT8                          *tempData          = VOS_NULL_PTR;
    errno_t                             memResult;
    VOS_UINT16                          totalLen;

    if ((simLockData == VOS_NULL_PTR) || (paraLen == 0)) {
        AT_ERR_LOG("AT_ProcSimlockWriteExData: NULL Pointer");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    /* 当前是一次新的设置过程，收到的是第一条AT命令 */
    if (simlockWriteExCtx->data == VOS_NULL_PTR) {
        /*lint -save -e830*/
        simlockWriteExCtx->data = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, (VOS_UINT32)paraLen);
        /*lint -restore */
        /* 分配内存失败，直接返回 */
        if (simlockWriteExCtx->data == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_ProcSimlockWriteExData: first data, Alloc mem fail");

            return AT_CME_MEMORY_FAILURE;
        }
        (VOS_VOID)memset_s(simlockWriteExCtx->data, paraLen, 0x00, paraLen);

        memResult = memcpy_s(simlockWriteExCtx->data, paraLen, simLockData, paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, paraLen, paraLen);

        simlockWriteExCtx->simlockDataLen = paraLen;
    } else {
        /* 当前不是收到第一条AT命令，需要拼接码流 */
        totalLen = paraLen + simlockWriteExCtx->simlockDataLen;
        /*lint -save -e516*/
        tempData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, totalLen);
        /*lint -restore */
        /* 分配内存失败，直接返回 */
        if (tempData == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_ProcSimlockWriteExData: Non-first data, Alloc mem fail");

            return AT_CME_MEMORY_FAILURE;
        }
        (VOS_VOID)memset_s(tempData, totalLen, 0x00, totalLen);

        if (simlockWriteExCtx->simlockDataLen > 0) {
            memResult = memcpy_s(tempData, totalLen, simlockWriteExCtx->data, simlockWriteExCtx->simlockDataLen);
            TAF_MEM_CHK_RTN_VAL(memResult, totalLen, simlockWriteExCtx->simlockDataLen);
        }

        memResult = memcpy_s((tempData + simlockWriteExCtx->simlockDataLen),
                             (totalLen - simlockWriteExCtx->simlockDataLen), simLockData, paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, (totalLen - simlockWriteExCtx->simlockDataLen), paraLen);

        /*lint -save -e830*/
        PS_MEM_FREE(WUEPS_PID_AT, simlockWriteExCtx->data);
        simlockWriteExCtx->data = VOS_NULL_PTR;
        /*lint -restore */
        simlockWriteExCtx->simlockDataLen = totalLen;
        simlockWriteExCtx->data           = tempData;
    }

    return AT_SUCCESS;
}

VOS_UINT32 AT_SaveSimlockDataIntoCtx(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                     AT_SimlockTypeUint8 netWorkFlg)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx = VOS_NULL_PTR;
    VOS_UINT8                          *simLockData       = VOS_NULL_PTR;
    VOS_UINT8                          *hmac              = VOS_NULL_PTR;
    VOS_UINT32                          result;
    VOS_UINT32                          layer;
    VOS_UINT32                          total;
    VOS_UINT32                          curIndex;
    errno_t                             memResult;
    VOS_UINT16                          simLockDataLen;
    VOS_UINT16                          hmacLen;
    VOS_UINT8                           paraNum;

    /* 变量初始化 */
    layer          = simlockWriteExPara->layer;
    total          = simlockWriteExPara->total;
    curIndex       = simlockWriteExPara->index;
    simLockDataLen = simlockWriteExPara->simLockDataLen;
    hmacLen        = simlockWriteExPara->hmacLen;
    paraNum        = simlockWriteExPara->paraNum;
    simLockData    = simlockWriteExPara->simLockData;
    hmac           = simlockWriteExPara->hmac;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    /* 当前不再设置过程中，第一次收到此命令 */
    if (simlockWriteExCtx->settingFlag == VOS_FALSE) {
        if (curIndex != 1) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: Invalid ulCurrIndex", simlockWriteExPara->index);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 将字符串参数转换为码流，并保存 */
        result = AT_ProcSimlockWriteExData(simLockData, simLockDataLen);

        if (result != AT_SUCCESS) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: AT_ProcSimlockWriteExData fail %d", result);

            return result;
        }

        simlockWriteExCtx->netWorkFlg  = netWorkFlg;
        simlockWriteExCtx->clientId    = indexNum;
        simlockWriteExCtx->totalNum    = (VOS_UINT8)total;
        simlockWriteExCtx->curIdx      = (VOS_UINT8)curIndex;
        simlockWriteExCtx->layer       = (VOS_UINT8)layer;
        simlockWriteExCtx->settingFlag = VOS_TRUE;
    } else {
        /* 必须同一个用户下发 */
        if (netWorkFlg != simlockWriteExCtx->netWorkFlg) {
            AT_WARN_LOG2("AT_SaveSimlockDataIntoCtx: ucNetWorkFlg error, PreNetWorkFlg %d, CurNetWorkFlg %d",
                         netWorkFlg, simlockWriteExCtx->netWorkFlg);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 必须在同一个通道下发命令 */
        if (indexNum != simlockWriteExCtx->clientId) {
            AT_WARN_LOG2("AT_SaveSimlockDataIntoCtx: port error, ucIndex %d, ucClientId %d", indexNum,
                         simlockWriteExCtx->clientId);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 当前已经在设置中，当前下发的Layer与之前之前下发的Layer不同 */
        if ((VOS_UINT8)layer != simlockWriteExCtx->layer) {
            AT_WARN_LOG2("AT_SaveSimlockDataIntoCtx: Layer %d wrong, %d", layer, simlockWriteExCtx->layer);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 当前已经在设置中，当前下发的total与之前之前下发的total不同 */
        if ((VOS_UINT8)total != simlockWriteExCtx->totalNum) {
            AT_WARN_LOG2("AT_SaveSimlockDataIntoCtx: total %d wrong, %d", total, simlockWriteExCtx->totalNum);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 当前下发的Index不是之前下发Index+1 */
        if ((VOS_UINT8)curIndex != (simlockWriteExCtx->curIdx + 1)) {
            AT_WARN_LOG2("AT_SaveSimlockDataIntoCtx: CurIndex %d wrong, %d", curIndex, simlockWriteExCtx->curIdx);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 将字符串参数转换为码流 */
        result = AT_ProcSimlockWriteExData(simLockData, simLockDataLen);

        if (result != AT_SUCCESS) {
            AT_WARN_LOG1("AT_SaveSimlockDataIntoCtx: AT_ProcSimlockWriteExData fail %d", result);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return result;
        }

        /* 更新CurrIndex */
        simlockWriteExCtx->curIdx = (VOS_UINT8)curIndex;
    }

    /* 如果参数个数为5，将第5个参数copy到全局变量，如果之前有输入HMAC，覆盖之前的输入 */
    if ((paraNum == 5) && (hmacLen == AT_SET_SIMLOCK_DATA_HMAC_LEN) && hmac != VOS_NULL_PTR) {
        memResult = memcpy_s(simlockWriteExCtx->hmac, AT_SET_SIMLOCK_DATA_HMAC_LEN, hmac, hmacLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_SET_SIMLOCK_DATA_HMAC_LEN, hmacLen);
        simlockWriteExCtx->hmacLen = (VOS_UINT8)hmacLen;
    }

    return AT_OK;
}

VOS_UINT32 AT_CheckSimlockDataWriteExPara(AT_SimlockWriteExPara *simlockWriteExPara)
{
    /* 规定的layer取值为0,1,2,3,4,255 */
    if ((simlockWriteExPara->layer > AT_SIMLOCKDATAWRITEEX_LAYER_FOUR_VALID_VALUE) &&
    (simlockWriteExPara->layer != AT_SIMLOCKDATAWRITEEX_LAYER_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid layer value:", simlockWriteExPara->layer);
        return VOS_ERR;
    }

    /* 规定的ulIndex取值为1-255 */
    if ((simlockWriteExPara->index == 0) ||
        (simlockWriteExPara->index > AT_SIMLOCKDATAWRITEEX_ULINDEX_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid ulIndex value:", simlockWriteExPara->index);
        return VOS_ERR;
    }

    /* 规定的ulTotal取值为1-255 */
    if ((simlockWriteExPara->total == 0) ||
        (simlockWriteExPara->total > AT_SIMLOCKDATAWRITEEX_ULTOTAL_MAX_VALID_VALUE)) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: invalid ulTotal value:", simlockWriteExPara->total);
        return VOS_ERR;
    }

    /* Index要小于total */
    if (simlockWriteExPara->index > simlockWriteExPara->total) {
        AT_WARN_LOG2("AT_CheckSimlockDataWriteExPara: Index bigger than total", simlockWriteExPara->index,
                     simlockWriteExPara->total);

        return VOS_ERR;
    }

    /* 规定一次写入的simlockdata数据不大于1400个字符 */
    if (simlockWriteExPara->simLockDataLen > AT_SIMLOCKDATA_PER_WRITE_MAX_LEN) {
        AT_WARN_LOG1("AT_CheckSimlockDataWriteExPara: SimLockData is too long:", simlockWriteExPara->simLockDataLen);
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetSimlockDataWriteExPara(AT_SimlockWriteExPara *simlockWriteExPara, VOS_UINT8 indexNum,
                                        AT_SimlockTypeUint8 netWorkFlg)
{
    AT_SIMLOCKDATAWRITEEX_CMD_PROC_CTX *simlockWriteExCtx    = VOS_NULL_PTR;
    DRV_AGENT_SimlockwriteexSetReq     *simlockWriteExSetReq = VOS_NULL_PTR;
    VOS_UINT32                          timerName;
    VOS_UINT32                          tempIndex;
    VOS_UINT32                          result;
    VOS_UINT32                          length;
    errno_t                             memResult;
    VOS_UINT16                          hmacLen;

    simlockWriteExCtx = AT_GetSimLockWriteExCmdCtxAddr();

    if (AT_CheckSimlockDataWriteExPara(simlockWriteExPara) != VOS_OK) {
        if (simlockWriteExCtx->settingFlag == VOS_FALSE) {
            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);
        }

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (simlockWriteExCtx->totalNum > AT_SIM_LOCK_DATA_WRITEEX_MAX_TOTAL) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将参数保存到全局变量  */
    result = AT_SaveSimlockDataIntoCtx(simlockWriteExPara, indexNum, netWorkFlg);

    if (result != AT_OK) {
        return result;
    }

    /* 如果还未收齐数据，则启动定时器，回复OK */
    if (simlockWriteExCtx->curIdx < simlockWriteExCtx->totalNum) {
        /* 停止上一周期的定时器，重启定时器 */
        AT_StopSimlockDataWriteTimer(indexNum);

        tempIndex = (VOS_UINT32)indexNum;
        timerName = AT_SIMLOCKWRITEEX_TIMER;
        timerName |= AT_INTERNAL_PROCESS_TYPE;
        timerName |= (tempIndex << 12);

        (VOS_VOID)AT_StartRelTimer(&(simlockWriteExCtx->hSimLockWriteExProtectTimer),
                                   AT_SIMLOCK_WRITE_EX_PROTECT_TIMER_LEN, timerName, 0, VOS_RELTIMER_NOLOOP);

        return AT_OK;
    } else {
        /* 已经收齐了数据，将Simlock_Data转换码流 */
        result = At_AsciiNum2HexString(simlockWriteExCtx->data, &(simlockWriteExCtx->simlockDataLen));
        if (result != AT_SUCCESS) {
            AT_WARN_LOG2("AT_SetSimlockDataWriteExPara: At_AsciiNum2HexString fail ulResult: %d ulParaLen: %d", result,
                         simlockWriteExCtx->simlockDataLen);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 已经收齐了数据，将HMAC转换码流 */
        hmacLen = simlockWriteExCtx->hmacLen;
        result  = At_AsciiNum2HexString(simlockWriteExCtx->hmac, &hmacLen);

        simlockWriteExCtx->hmacLen = (VOS_UINT8)hmacLen;

        if ((result != AT_SUCCESS) || (simlockWriteExCtx->hmacLen != DRV_AGENT_HMAC_DATA_LEN)) {
            AT_WARN_LOG2("AT_SetSimlockDataWriteExPara: At_AsciiNum2HexString fail ulResult: %d ulParaLen: %d", result,
                         simlockWriteExCtx->hmacLen);

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_INCORRECT_PARAMETERS;
        }

        length               = sizeof(DRV_AGENT_SimlockwriteexSetReq) + simlockWriteExCtx->simlockDataLen -
                               AT_SIMLOCK_DATA_LEN;
        simlockWriteExSetReq = (DRV_AGENT_SimlockwriteexSetReq *)PS_MEM_ALLOC(WUEPS_PID_AT, length);

        if (simlockWriteExSetReq == VOS_NULL_PTR) {
            AT_WARN_LOG("AT_SetSimlockDataWriteExPara: alloc mem fail.");

            AT_ClearSimLockWriteExCtx();
            AT_StopSimlockDataWriteTimer(indexNum);

            return AT_CME_MEMORY_FAILURE;
        }
        (VOS_VOID)memset_s(simlockWriteExSetReq, length, 0x00, length);

        simlockWriteExSetReq->hmacLen = simlockWriteExCtx->hmacLen;
        if (simlockWriteExCtx->hmacLen > 0) {
            memResult = memcpy_s(simlockWriteExSetReq->hmac, sizeof(simlockWriteExSetReq->hmac),
                                 simlockWriteExCtx->hmac, simlockWriteExCtx->hmacLen);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(simlockWriteExSetReq->hmac), simlockWriteExCtx->hmacLen);
        }

        simlockWriteExSetReq->simlockDataLen = simlockWriteExCtx->simlockDataLen;
        if (simlockWriteExCtx->simlockDataLen > 0) {
            memResult = memcpy_s(simlockWriteExSetReq->simlockData, simlockWriteExCtx->simlockDataLen,
                                 simlockWriteExCtx->data, simlockWriteExCtx->simlockDataLen);
            TAF_MEM_CHK_RTN_VAL(memResult, simlockWriteExCtx->simlockDataLen, simlockWriteExCtx->simlockDataLen);
        }

        /* 记录总共写入的次数，既最后一次写入时的index */
        simlockWriteExSetReq->total = simlockWriteExCtx->totalNum;

        /* 记录是否是网络下发的标识 */
        simlockWriteExSetReq->netWorkFlg = simlockWriteExCtx->netWorkFlg;
        simlockWriteExSetReq->layer      = simlockWriteExCtx->layer;

        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        /* 转换成功, 发送跨核消息到C核, 设置产线公钥 */
        result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                        DRV_AGENT_SIMLOCKWRITEEX_SET_REQ, (VOS_UINT8 *)simlockWriteExSetReq, length,
                                        I0_WUEPS_PID_DRV_AGENT);
        /*lint -save -e516*/
        PS_MEM_FREE(WUEPS_PID_AT, simlockWriteExSetReq);
        /*lint -restore */
        if (result != TAF_SUCCESS) {
            AT_WARN_LOG("AT_SetSimlockDataWriteExPara: AT_FillAndSndAppReqMsg fail.");

            return AT_ERROR;
        }

        /* 由于SIMLOCKDATAWRITEEX特殊处理，需要手动启动定时器 */
        if (At_StartTimer(AT_SET_PARA_TIME, indexNum) != AT_SUCCESS) {
            AT_WARN_LOG("AT_SetSimlockDataWriteExPara: At_StartTimer fail.");

            return AT_ERROR;
        }

        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

        /* 设置AT模块实体的状态为等待异步返回 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIMLOCKDATAWRITEEX_SET;

        return AT_WAIT_ASYNC_RETURN;
    }
}


VOS_UINT32 AT_ParseSimlockDataWriteExParaValue(VOS_UINT8 *data, AT_SimlockWriteExPara *simlockWriteExPara,
                                               VOS_UINT16 pos, VOS_UINT16 len)
{
    VOS_UINT16 commPos[AT_COMMPOS_LENGTH] = {0};
    VOS_UINT32 firstParaVal;
    VOS_UINT32 secParaVal;
    VOS_UINT32 thirdParaVal;
    VOS_UINT16 loop;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT16 thirdParaLen;
    VOS_UINT16 fourthParaLen;
    VOS_UINT16 fifthParaLen;
    VOS_UINT16 commaCnt;
    VOS_UINT8  paraNum;

    commaCnt      = 0;
    fourthParaLen = 0;
    fifthParaLen  = 0;
    firstParaVal  = 0;
    secParaVal    = 0;
    thirdParaVal  = 0;
    paraNum       = 0;

    /* 获取命令中的逗号位置和个数 */
    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            /* 记录下逗号的位置 */
            if (commaCnt < AT_COMMPOS_LENGTH) {
                commPos[commaCnt] = loop + 1;
            }
            commaCnt++;
        }
    }

    /* 若逗号个数大于4，则AT命令结果返回失败 */
    if ((commaCnt != AT_CONST_NUM_4) && (commaCnt != AT_CONST_NUM_3)) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: Num of Para is  Invalid!");
        return VOS_ERR;
    }

    /* 计算参数的长度 */
    firstParaLen  = (commPos[AT_FIRST_COMMPOS] - pos) - (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    secondParaLen = (commPos[AT_SECOND_COMMPOS] - commPos[AT_FIRST_COMMPOS]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    thirdParaLen  = (commPos[AT_THIRD_COMMPOS] - commPos[AT_SECOND_COMMPOS]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);

    /* 如果逗号的个数为3，那么参数的个数为4；如果逗号的个数为4，那么参数的个数为5 */
    if (commaCnt == 3) {
        fourthParaLen = len - commPos[AT_THIRD_COMMPOS];
        paraNum       = 4;
    } else {
        fourthParaLen = (commPos[AT_FOURTH_COMMPOS] - commPos[AT_THIRD_COMMPOS]) -
                        (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
        fifthParaLen  = len - commPos[AT_FOURTH_COMMPOS];
        paraNum       = 5;
    }

    /* 获取第一个参数值 */
    if (atAuc2ul(data + pos, firstParaLen, &firstParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulFirstParaVal value invalid");
        return VOS_ERR;
    }

    /* 获取第二个参数值 */
    if (atAuc2ul(data + commPos[0], secondParaLen, &secParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulSecParaVal value invalid");
        return VOS_ERR;
    }

    /* 获取第三个参数值 */
    if (atAuc2ul(data + commPos[1], thirdParaLen, &thirdParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_ParseSimlockDataWriteExParaValue: ulThirdParaVal value invalid");
        return VOS_ERR;
    }

    simlockWriteExPara->paraNum        = paraNum;
    simlockWriteExPara->layer          = firstParaVal;
    simlockWriteExPara->index          = secParaVal;
    simlockWriteExPara->total          = thirdParaVal;
    simlockWriteExPara->simLockDataLen = fourthParaLen;
    simlockWriteExPara->simLockData    = data + commPos[AT_THIRD_COMMPOS];

    /* 如果参数个数等于5 */
    if (paraNum == 5) {
        simlockWriteExPara->hmacLen = fifthParaLen;
        simlockWriteExPara->hmac    = data + commPos[AT_FOURTH_COMMPOS];
    }

    return VOS_OK;
}
#if (FEATURE_SC_NETWORK_UPDATE == FEATURE_ON)

VOS_UINT32 AT_HandleSimLockNWDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    AT_ParseCmdNameType   atCmdName;
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            cmdlen;
    VOS_UINT16            pos;
    VOS_UINT16            length;
    VOS_INT8              ret;

    /* 只能是APPVCOM端口19下发的命令 */
    if ((g_atClientTab[indexNum].userType != AT_APP_USER) ||
        (g_atClientTab[indexNum].portNo != APP_VCOM_DEV_INDEX_19)) {
        return AT_FAILURE;
    }

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));
    cmdlen = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKNWDATAWRITE=", AT_CONST_NUM_23);

    if (len < cmdlen) {
        return AT_FAILURE;
    }

    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockNWDataWriteCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是"AT^SIMLOCKDATAWRITEEX="直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKNWDATAWRITE=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        dataPara = VOS_NULL_PTR;
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKNWDATAWRITE", AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen("^SIMLOCKNWDATAWRITE", AT_CONST_NUM_23);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.pucSimLockData = VOS_NULL_PTR;
    simlockWriteExPara.pucHmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        dataPara = VOS_NULL_PTR;
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        return AT_SUCCESS;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_NW);

    /* 添加打印 ^SIMLOCKNWDATAWRITE:<index>操作 */
    length = 0;

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_OK) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", "^SIMLOCKNWDATAWRITE", simlockWriteExPara.ulIndex);
        }

        g_atSendDataBuff.usBufLen = length;
        At_FormatResultData(indexNum, result);
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    dataPara = VOS_NULL_PTR;
    return AT_SUCCESS;
}
#endif

VOS_UINT32 AT_HandleSimLockDataWriteExCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    AT_ParseCmdNameType   atCmdName;
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            cmdlen;
    VOS_UINT16            pos;
    VOS_UINT16            length;
    VOS_INT8              ret;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));

    /* 局部变量初始化 */
    cmdlen = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKDATAWRITEEX=", AT_CONST_NUM_23);

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* 长度不满足设置命令的最小长度 AT^SIMLOCKDATAWRITEEX=1,1,1,  等于号后6个字符 */
    if ((cmdlen + AT_SIMLOCKDATAWRITEEX_STR_MIN_LEN) > len) {
        return AT_FAILURE;
    }
    /*lint -save -e516*/
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockDataWriteExCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是"AT^SIMLOCKDATAWRITEEX="直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKDATAWRITEEX=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKDATAWRITEEX", AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen("^SIMLOCKDATAWRITEEX", AT_CONST_NUM_23);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.simLockData = VOS_NULL_PTR;
    simlockWriteExPara.hmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return AT_SUCCESS;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_FAC);

    /* 添加打印 ^SIMLOCKDATAWRITEEX:<index>操作 */
    length = 0;

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_OK) {
            AT_PR_LOGI("return OK");

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", "^SIMLOCKDATAWRITEEX", simlockWriteExPara.index);
        }

        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    return AT_SUCCESS;
}


VOS_UINT32 AT_HandleSimLockOtaDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8            *dataPara = VOS_NULL_PTR;
    AT_ParseCmdNameType   atCmdName;
    AT_SimlockWriteExPara simlockWriteExPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            cmdlen;
    VOS_UINT16            pos;
    VOS_UINT16            length;
    VOS_INT8              ret;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));

    /* 局部变量初始化 */
    cmdlen = (VOS_UINT16)VOS_StrNLen("AT^SIMLOCKOTADATAWRITE=", AT_CONST_NUM_24);

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* 长度不满足设置命令的最小长度 AT^SIMLOCKOTADATAWRITE=1,1,1,  等于号后6个字符 */
    if ((cmdlen + AT_SIMLOCKOTADATAWRITE_STR_MIN_LEN) > len) {
        return AT_FAILURE;
    }
    /*lint -save -e516*/
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockOtaDataWriteCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是"AT^SIMLOCKOTADATAWRITE="直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKOTADATAWRITE=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKOTADATAWRITE", AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen("^SIMLOCKOTADATAWRITE", AT_CONST_NUM_24);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&simlockWriteExPara, sizeof(simlockWriteExPara), 0x00, sizeof(simlockWriteExPara));
    simlockWriteExPara.simLockData = VOS_NULL_PTR;
    simlockWriteExPara.hmac        = VOS_NULL_PTR;

    if (AT_ParseSimlockDataWriteExParaValue(data, &simlockWriteExPara, pos, len) != VOS_OK) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearSimLockWriteExCtx();
        AT_StopSimlockDataWriteTimer(indexNum);

        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return AT_SUCCESS;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetSimlockDataWriteExPara(&simlockWriteExPara, indexNum, AT_SIMLOCK_TYPE_OTA);

    /* 添加打印 ^SIMLOCKDATAWRITEEX:<index>操作 */
    length = 0;

    if (result != AT_WAIT_ASYNC_RETURN) {
        if (result == AT_OK) {
            AT_PR_LOGI("return OK");

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", "^SIMLOCKOTADATAWRITE", simlockWriteExPara.index);
        }

        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, result);
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    return AT_SUCCESS;
}


VOS_VOID AT_HandleIdentifyEndCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len, const VOS_CHAR *specialCmdName)
{
    AT_ParseCmdNameType   atCmdName;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            pos;
    VOS_UINT16            paraLen;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));
    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)specialCmdName, AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen((VOS_CHAR *)specialCmdName, AT_SPECIAL_CMD_MAX_LEN);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    paraLen = len - pos;

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetIdentifyEndPara(indexNum, (data + pos), paraLen, VOS_FALSE);
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }
    return;
}


VOS_UINT32 AT_ProcPhonePhynumCmdCtx(AT_PhonePhynumPara *phonePhynumSetPara){
    AT_PhonePhynumCmdProcCtx *phynumCmdCtx = VOS_NULL_PTR;
    errno_t                   memResult;

    phynumCmdCtx = AT_GetPhonePhynumCmdCtxAddr();

    if ((phonePhynumSetPara->phynumTypeLen == 0) ||
        (phonePhynumSetPara->phonePhynumValueLen == 0) ||
        (phonePhynumSetPara->hmacLen == 0)) {
        AT_ERR_LOG("AT_ProcPhonePhynumCmdCtx: NULL Pointer");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 变量初始化 */
    /* 将字符串参数转换成码流，第一个参数 */
    if (phynumCmdCtx->typeValue == VOS_NULL_PTR) {
        phynumCmdCtx->typeValueLen = phonePhynumSetPara->phynumTypeLen;
        phynumCmdCtx->typeValue    = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT,
            (VOS_UINT32)phonePhynumSetPara->phynumTypeLen);
        if (phynumCmdCtx->typeValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: first data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->typeValue, phynumCmdCtx->typeValueLen, 0x00, phynumCmdCtx->typeValueLen);

        memResult = memcpy_s(phynumCmdCtx->typeValue, phynumCmdCtx->typeValueLen, phonePhynumSetPara->phynumTypeData,
                             phonePhynumSetPara->phynumTypeLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->typeValueLen, phonePhynumSetPara->phynumTypeLen);
    }

    /* 将字符串参数转换成码流，第二个参数 */
    if (phynumCmdCtx->phynumValue == VOS_NULL_PTR) {
        phynumCmdCtx->phynumValueLen = phonePhynumSetPara->phonePhynumValueLen;
        phynumCmdCtx->phynumValue = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT,
            (VOS_UINT32)phonePhynumSetPara->phonePhynumValueLen);
        if (phynumCmdCtx->phynumValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: second data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->phynumValue, phynumCmdCtx->phynumValueLen, 0x00, phynumCmdCtx->phynumValueLen);

        memResult = memcpy_s(phynumCmdCtx->phynumValue, phynumCmdCtx->phynumValueLen,
                             phonePhynumSetPara->phonePhynumValue, phonePhynumSetPara->phonePhynumValueLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->phynumValueLen, phonePhynumSetPara->phonePhynumValueLen);
    }

    /* 将字符串参数转换成码流，第三个参数 */
    if (phynumCmdCtx->hmacValue == VOS_NULL_PTR) {
        phynumCmdCtx->hmacValueLen = phonePhynumSetPara->hmacLen;
        phynumCmdCtx->hmacValue    = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, (VOS_UINT32)phonePhynumSetPara->hmacLen);
        if (phynumCmdCtx->hmacValue == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_PhonePhynumCmdCtx: third data, Alloc mem fail");
            return VOS_ERR;
        }
        (VOS_VOID)memset_s(phynumCmdCtx->hmacValue, phynumCmdCtx->hmacValueLen, 0x00, phynumCmdCtx->hmacValueLen);

        memResult = memcpy_s(phynumCmdCtx->hmacValue, phynumCmdCtx->hmacValueLen,
                             phonePhynumSetPara->hmacData, phonePhynumSetPara->hmacLen);
        TAF_MEM_CHK_RTN_VAL(memResult, phynumCmdCtx->hmacValueLen, phonePhynumSetPara->hmacLen);
    }

    return AT_OK;
}


VOS_UINT32 AT_PhonePhynumParaValue(VOS_UINT8 *data, AT_PhonePhynumPara *phonePhynumPara,
                                   VOS_UINT16 pos, VOS_UINT16 len)
{
    VOS_UINT32 result;
    VOS_UINT32 loop;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT16 thirdParaLen;
    VOS_UINT16 commaCnt                         = 0;
    VOS_UINT16 commPos[AT_PHONEPHYNUM_PARA_NUM] = {0};

    /* 获取命令中的逗号位置和个数 */
    for (loop = pos; loop < (VOS_UINT32)AT_MIN(len, AT_CMD_MAX_LEN); loop++) {
        if (*(data + loop) == ',') {
            /* 记录下逗号的位置 */
            if (commaCnt < AT_PHONEPHYNUM_PARA_NUM) {
                commPos[commaCnt] = (VOS_UINT16)(loop + 1);
            } else {
                break;
            }
            commaCnt++;
        }
    }

    /* 若逗号个数不等于2，则AT命令结果返回失败 */
    if (commaCnt != AT_CONST_NUM_2) {
        AT_WARN_LOG("AT_PhonePhynumParaValue: Num of Para is  Invalid!");
        return VOS_ERR;
    }

    /* 此处commPos为逗号位置，pos为等号位置，确认减法不会发生反转 */
    firstParaLen  = (commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM] - pos) - (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    secondParaLen = (commPos[AT_PHONEPHYNUM_PARA_RSA_NUM] - commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM]) -
                    (VOS_UINT16)VOS_StrNLen(",", AT_CONST_NUM_2);
    thirdParaLen  = (len - commPos[AT_PHONEPHYNUM_PARA_RSA_NUM]);

    phonePhynumPara->phynumTypeLen       = firstParaLen;
    phonePhynumPara->phynumTypeData      = data + pos;
    phonePhynumPara->phonePhynumValueLen = secondParaLen;
    phonePhynumPara->phonePhynumValue    = data + commPos[AT_PHONEPHYNUM_PARA_TYPE_NUM];
    phonePhynumPara->hmacLen             = thirdParaLen;
    phonePhynumPara->hmacData            = data + commPos[AT_PHONEPHYNUM_PARA_RSA_NUM];

    /* 将参数保存到全局变量  */
    result = AT_ProcPhonePhynumCmdCtx(phonePhynumPara);

    if (result != AT_OK) {
        return result;
    }

    return VOS_OK;
}


VOS_VOID AT_HandlePhonePhynumCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len, const VOS_CHAR *specialCmdName)
{
    AT_ParseCmdNameType   atCmdName;
    AT_PhonePhynumPara    phonePhynumPara;
    VOS_UINT32            result;
    errno_t               memResult;
    VOS_UINT16            pos;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));
    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)specialCmdName, AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrNLen("AT", AT_CONST_NUM_3);

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrNLen((VOS_CHAR *)specialCmdName, AT_SPECIAL_CMD_MAX_LEN);
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrNLen("=", AT_CONST_NUM_2);

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&phonePhynumPara, sizeof(phonePhynumPara), 0x00, sizeof(phonePhynumPara));
    phonePhynumPara.phynumTypeData   = VOS_NULL_PTR;
    phonePhynumPara.phonePhynumValue = VOS_NULL_PTR;
    phonePhynumPara.hmacData         = VOS_NULL_PTR;

    if (AT_PhonePhynumParaValue(data, &phonePhynumPara, pos, len) != VOS_OK) {
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearPhonePhynumCtx();
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);

        return;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetPhonePhynumPara(indexNum);
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }

    AT_ClearPhonePhynumCtx();
    return;
}


VOS_UINT32 AT_HandleApModemSpecialCmdProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len,
               const VOS_CHAR *specialCmdName)
{
    VOS_CHAR             *specialCmd     = VOS_NULL_PTR;
    VOS_UINT8            *dataPara       = VOS_NULL_PTR;
    AT_SpecialCmdProcFunc specialCmdFunc = VOS_NULL_PTR;
    errno_t               memResult;
    VOS_INT32             specialCmdLen;
    VOS_UINT32            i;
    VOS_UINT16            cmdlen;
    VOS_INT8              ret;

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        return AT_FAILURE;
    }

    specialCmd = (VOS_CHAR *)PS_MEM_ALLOC(WUEPS_PID_AT, VOS_StrLen(specialCmdName) + 4);
    if (specialCmd == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleApModemSpecialCmdProc: specialCmd Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(specialCmd, VOS_StrLen(specialCmdName) + 4, 0x00, VOS_StrLen(specialCmdName) + 4);

    specialCmdLen = snprintf_s(specialCmd, (VOS_SIZE_T)(VOS_StrLen(specialCmdName) + 4),
        (VOS_SIZE_T)VOS_StrLen(specialCmdName), "AT%s=", specialCmdName);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(specialCmdLen, VOS_StrLen(specialCmdName) + 4, VOS_StrLen(specialCmdName));

    /* 局部变量初始化 */
    cmdlen = (VOS_UINT16)VOS_StrNLen(specialCmd, AT_SPECIAL_CMD_MAX_LEN);

    /* 长度不满足设置命令specialCmd的最小长度 */
    if (cmdlen > len) {
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        return AT_FAILURE;
    }

    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen + 1);
    if (dataPara == VOS_NULL_PTR) {
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        AT_ERR_LOG("AT_HandleApModemSpecialCmdProc: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen + 1, 0x00, cmdlen + 1);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen + 1, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen + 1, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是specialCmd直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, specialCmd, cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
        return AT_FAILURE;
    }

    for (i = 0; i < sizeof(g_ApModemSpecialCmdFucTab) / sizeof(AT_SpecialCmdFuncTbl); i++) {
        if (VOS_StrNiCmp(g_ApModemSpecialCmdFucTab[i].specialCmdName, specialCmd, cmdlen) == VOS_OK) {
            specialCmdFunc = g_ApModemSpecialCmdFucTab[i].specialCmdFunc;
            break;
        }
    }

    if (specialCmdFunc != VOS_NULL_PTR) {
        specialCmdFunc(indexNum, data, len, specialCmdName); /* 特殊处理函数 */
    }

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    PS_MEM_FREE(WUEPS_PID_AT, specialCmd);
    return AT_SUCCESS;
}


VOS_UINT32 AT_RcvDrvAgentSimlockWriteExSetCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                  *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_SimlockwriteexSetCnf *event  = VOS_NULL_PTR;
    VOS_UINT32                      result;
    VOS_UINT16                      length;
    VOS_UINT8                       indexNum;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_SimlockwriteexSetCnf *)rcvMsg->content;
    indexNum = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockWriteExSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockWriteExSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIMLOCKWRITEEX_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKDATAWRITEEX_SET) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockWriteExSetCnf: CmdCurrentOpt ERR.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    length = 0;

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, event->total);
        result = AT_OK;
    } else {
        /* 异常情况, 转换错误码 */
        result = AT_PERSONALIZATION_ERR_BEGIN + event->result;
    }

    g_atSendDataBuff.bufLen = length;

    /* 调用AT_FormATResultDATa发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#if (FEATURE_PHONE_SC == FEATURE_ON)

VOS_UINT32 AT_SimLockDataReadExPara(VOS_UINT8 indexNum)
{
    DRV_AGENT_SimlockdatareadexReadReq simLockDataReadExReq;
    VOS_UINT32                         result;

    AT_PR_LOGI("enter");

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&simLockDataReadExReq, sizeof(simLockDataReadExReq), 0x00, sizeof(simLockDataReadExReq));

    /* 通道检查 */
    if (AT_IsApPort(indexNum) == VOS_FALSE) {
        AT_WARN_LOG("AT_SimLockDataReadExPara: It Is not Ap Port.");
        return AT_ERROR;
    }

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SimLockDataReadExPara: ucCmdOptType is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数错误 */
    if (g_atParaIndex != AT_SIMLOCKDATAREADEX_VALID_PARA_NUM) {
        AT_WARN_LOG("AT_SimLockDataReadExPara: g_atParaIndex ERR.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数的长度不能为0 */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SimLockDataReadExPara: g_atParaList[0].paraLen err.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    simLockDataReadExReq.layer    = (VOS_UINT8)g_atParaList[0].paraValue;
    simLockDataReadExReq.indexNum = (VOS_UINT8)g_atParaList[1].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    DRV_AGENT_SIMLOCKDATAREADEX_READ_REQ, (VOS_UINT8 *)&simLockDataReadExReq,
                                    (VOS_UINT32)sizeof(simLockDataReadExReq), I0_WUEPS_PID_DRV_AGENT);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SimLockDataReadExPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SIMLOCKDATAREADEX_READ_SET;

    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 AT_RcvDrvAgentSimlockDataReadExReadCnf(struct MsgCB *msg)
{
    DRV_AGENT_Msg                      *rcvMsg = VOS_NULL_PTR;
    DRV_AGENT_SimlockdatareadexReadCnf *event  = VOS_NULL_PTR;
    VOS_UINT32                          result;
    VOS_UINT32                          loop;
    VOS_UINT16                          length;
    VOS_UINT8                           indexNum;

    AT_PR_LOGI("enter");

    /* 初始化消息变量 */
    length   = 0;
    indexNum = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    rcvMsg   = (DRV_AGENT_Msg *)msg;
    event    = (DRV_AGENT_SimlockdatareadexReadCnf *)rcvMsg->content;

    result = event->result;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(event->atAppCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataReadExReadCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataReadExReadCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SIMLOCKDATAREADEX_READ_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SIMLOCKDATAREADEX_READ_SET) {
        AT_WARN_LOG("AT_RcvDrvAgentSimlockDataReadExReadCnf: CmdCurrentOpt ERR.");
        return VOS_ERR;
    }

    /* 判断查询操作是否成功 */
    if (event->result == DRV_AGENT_PERSONALIZATION_NO_ERROR) {
        /* 输出设置结果 */
        result = AT_OK;

        /* 添加<layer>,<index>,<total>打印 */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d,%d,%d,", g_parseContext[indexNum].cmdElement->cmdName, event->layer,
            event->indexNum, event->total);

        /* 添加<simlock_data>打印 */
        for (loop = 0; loop < event->dataLen; loop++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02x", event->data[loop]);
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


VOS_UINT32 AT_SetActPdpStubPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 flag;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_ACTPDPSTUB_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 获取设置的标志 */
    flag = (VOS_UINT8)g_atParaList[1].paraValue;

    /* 根据MODEM ID调用不同的桩函数 */
    if (g_atParaList[AT_PDPSTU_CLIENT_ID].paraValue == AT_PCUIPSCALL_FLAG_INDEX) {
        AT_SetPcuiPsCallFlag(flag, AT_CLIENT_ID_APP);
    } else if (g_atParaList[AT_PCUIPSCALL_FLAG_INDEX].paraValue == AT_CTRLPSCALL_FLAG_INDEX) {
        AT_SetCtrlPsCallFlag(flag, AT_CLIENT_ID_APP);
    } else if (g_atParaList[AT_PCUIPSCALL_FLAG_INDEX].paraValue == AT_PCUI2PSCALL_FLAG_INDEX) {
        AT_SetPcui2PsCallFlag(flag, AT_CLIENT_ID_APP);
    } else {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetNVCHKPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 loopIndex;

    AT_PR_LOGI("Rcv Msg");

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_NVCHK_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 未带参数判断 */
    if (g_atParaList[AT_NVCHK_PARA_PARA0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果是全部检查，则循环检查全部CRC */
    if (g_atParaList[AT_NVCHK_PARA_PARA0].paraValue == 0) {
        for (loopIndex = 0; loopIndex < AT_NVCHK_CRC_MAX_NUM; loopIndex++) {
            if (AT_CheckFactoryNv(loopIndex) != 0) {
                AT_PR_LOGI("Call interface success!");
                return AT_ERROR;
            }
        }

        return AT_OK;
    }

    /* 如果返回0，为正常，非0为异常 */
    if (AT_CheckFactoryNv(g_atParaList[AT_NVCHK_PARA_PARA0].paraValue - 1) == 0) {
        AT_PR_LOGI("Call interface success!");
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaAfcClkInfoCnf(struct MsgCB *msg)
{
    AT_MTA_Msg                    *mtaMsg = VOS_NULL_PTR;
    MTA_AT_QryAfcClkFreqXocoefCnf *afcCnf = VOS_NULL_PTR;
    VOS_UINT32                     ret;
    VOS_UINT8                      indexNum;

    mtaMsg = (AT_MTA_Msg *)msg;
    afcCnf = (MTA_AT_QryAfcClkFreqXocoefCnf *)mtaMsg->content;

    /* 初始化消息变量 */
    indexNum = 0;
    ret      = AT_OK;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaAfcClkInfoCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaAfcClkInfoCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_AFCCLKINFO_QRY) {
        AT_WARN_LOG("AT_RcvMtaAfcClkInfoCnf : Current Option is not AT_CMD_AFCCLKINFO_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (afcCnf->result != MTA_AT_RESULT_NO_ERROR) {
        ret = AT_ERROR;
    } else {
        /*
         * ^AFCCLKINFO: <status>,<deviation>,<sTemp>,<eTemp>,<a0_m>,<a0_e>,
         * <a1_m>,<a1_e>,<a2_m>,<a2_e>,<a3_m>,<a3_e>,
         * <rat>,<ModemId>
         */
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %u,%d,%d,%d,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d",
                g_parseContext[indexNum].cmdElement->cmdName, afcCnf->status, afcCnf->deviation,
                afcCnf->coeffStartTemp, afcCnf->coeffEndTemp, afcCnf->coeffMantissa[AT_AFCCLKINFO_PARA_A0_MANTISSA],
                afcCnf->coeffMantissa[AT_AFCCLKINFO_PARA_A0_EXPONENT],
                afcCnf->coeffMantissa[AT_AFCCLKINFO_PARA_A1_MANTISSA],
                afcCnf->coeffMantissa[AT_AFCCLKINFO_PARA_A1_EXPONENT],
                afcCnf->coeffExponent[AT_AFCCLKINFO_PARA_A2_MANTISSA],
                afcCnf->coeffExponent[AT_AFCCLKINFO_PARA_A2_EXPONENT],
                afcCnf->coeffExponent[AT_AFCCLKINFO_PARA_A3_MANTISSA],
                afcCnf->coeffExponent[AT_AFCCLKINFO_PARA_A3_EXPONENT], afcCnf->ratMode, afcCnf->modemId);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_SetCtzuPara(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        g_ctzuFlag = 0;
    } else {
        g_ctzuFlag = g_atParaList[0].paraValue;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCtzuPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, g_ctzuFlag);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_SetEvdoSysEvent(VOS_UINT8 indexNum)
{
    AT_MTA_EvdoSysEventSetReq sysEvent;
    VOS_UINT32                rslt;

    (VOS_VOID)memset_s(&sysEvent, sizeof(sysEvent), 0x00, sizeof(sysEvent));

    /* 命令状态检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不为1或者字符串长度大于10, 4294967295 = 0xffffffff */
    if ((g_atParaIndex != AT_DOSYSEVENT_PARA_NUM) || (g_atParaList[AT_DOSYSEVENT_DO_SYS_EVENT].paraLen >
        AT_DOSYSEVENT_DO_SYS_EVENT_VALID)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    sysEvent.doSysEvent = g_atParaList[AT_DOSYSEVENT_DO_SYS_EVENT].paraValue;

    rslt = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                  ID_AT_MTA_EVDO_SYS_EVENT_SET_REQ, (VOS_UINT8 *)&sysEvent, sizeof(sysEvent),
                                  I0_UEPS_PID_MTA);

    if (rslt == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EVDO_SYS_EVENT_SET;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetEvdoSysEvent: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetDoSigMask(VOS_UINT8 indexNum)
{
    AT_MTA_EvdoSigMaskSetReq sigMask;
    VOS_UINT32               rslt;

    (VOS_VOID)memset_s(&sigMask, sizeof(sigMask), 0x00, sizeof(sigMask));

    /* 命令状态检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不为1或者字符串长度大于10, 4294967295 = 0xffffffff */
    if ((g_atParaIndex != AT_DOSIGMASK_PARA_NUM) || (g_atParaList[AT_DOSIGMASK_DO_SIG_MASK].paraLen >
        AT_DOSIGMASK_DO_SIG_MASK_VALID)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    sigMask.doSigMask = g_atParaList[AT_DOSIGMASK_DO_SIG_MASK].paraValue;

    rslt = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                  ID_AT_MTA_EVDO_SIG_MASK_SET_REQ, (VOS_UINT8 *)&sigMask, sizeof(sigMask),
                                  I0_UEPS_PID_MTA);

    if (rslt == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_EVDO_SIG_MASK_SET;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetDoSigMask: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaEvdoSysEventSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEvdoSysEventSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaEvdoSysEventSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EVDO_SYS_EVENT_SET) {
        AT_WARN_LOG("AT_RcvMtaEvdoSysEventSetCnf : Current Option is not AT_CMD_LTE_WIFI_COEX_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^MEID命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (cnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEvdoSigMaskSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigMaskSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigMaskSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_EVDO_SIG_MASK_SET) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigMaskSetCnf : Current Option is not AT_CMD_LTE_WIFI_COEX_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^MEID命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (cnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEvdoRevARLinkInfoInd(struct MsgCB *msg)
{
    VOS_UINT8                    indexNum;
    VOS_UINT32                   dataLen;
    VOS_UINT32                   rslt;
    AT_MTA_Msg                  *mtaMsg    = VOS_NULL_PTR;
    MTA_AT_EvdoRevaRlinkInfoInd *rlinkInfo = VOS_NULL_PTR;
    VOS_UINT8                   *data      = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum  = 0;
    mtaMsg    = (AT_MTA_Msg *)msg;
    rlinkInfo = (MTA_AT_EvdoRevaRlinkInfoInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEvdoRevARLinkInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 前后两个双引号加\0的长度为3 */
    dataLen = rlinkInfo->paraLen * sizeof(VOS_UINT8) * AT_DOUBLE_LENGTH + (AT_DOUBLE_QUO_MARKS_LENGTH + 1);
    /*lint -save -e516 */
    data = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, dataLen);
    /*lint -restore */
    if (data == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMtaEvdoRevARLinkInfoInd(): mem alloc Fail!");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(data, dataLen, 0x00, dataLen);
    data[dataLen - 1] = '\0';

    /* 前后两个双引号加\0的长度为3 */
    rslt = AT_HexToAsciiString(&data[1], (dataLen - (AT_DOUBLE_QUO_MARKS_LENGTH + 1)),
                               rlinkInfo->content, rlinkInfo->paraLen);

    data[0]                                    = '"';
    data[dataLen - AT_DOUBLE_QUO_MARKS_LENGTH] = '"';

    if (rslt != AT_OK) {
        AT_WARN_LOG("AT_RcvMtaEvdoRevARLinkInfoInd: WARNING: Hex to Ascii trans fail!");

        PS_MEM_FREE(WUEPS_PID_AT, data);
        data = VOS_NULL_PTR;

        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^DOREVARLINK: %d,%s%s",
        g_atCrLf, rlinkInfo->paraLen * AT_DOUBLE_LENGTH, data, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    PS_MEM_FREE(WUEPS_PID_AT, data);
    data = VOS_NULL_PTR;

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaEvdoSigExEventInd(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum;
    AT_MTA_Msg               *mtaMsg     = VOS_NULL_PTR;
    MTA_AT_EvdoSigExeventInd *sigExEvent = VOS_NULL_PTR;
    VOS_UINT8                *data       = VOS_NULL_PTR;
    VOS_UINT32                dataLen;
    VOS_UINT32                rslt;

    /* 初始化消息变量 */
    indexNum   = 0;
    mtaMsg     = (AT_MTA_Msg *)msg;
    sigExEvent = (MTA_AT_EvdoSigExeventInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigExEventInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    /* 前后两个双引号加\0的长度为3 */
    dataLen = sigExEvent->paraLen * sizeof(VOS_UINT8) * AT_DOUBLE_LENGTH + (AT_DOUBLE_QUO_MARKS_LENGTH + 1);
    /*lint -save -e516 */
    data = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, dataLen);
    /*lint -restore */
    if (data == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigExEventInd(): mem alloc Fail!");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(data, dataLen, 0x00, dataLen);
    data[dataLen - 1] = '\0';

    /* 前后两个双引号加\0的长度为3 */
    rslt = AT_HexToAsciiString(&data[1], (dataLen - (AT_DOUBLE_QUO_MARKS_LENGTH + 1)),
                               sigExEvent->content, sigExEvent->paraLen);

    data[0]           = '"';
    data[dataLen - AT_DOUBLE_QUO_MARKS_LENGTH] = '"';

    if (rslt != AT_OK) {
        AT_WARN_LOG("AT_RcvMtaEvdoSigExEventInd: WARNING: Hex to Ascii trans fail!");

        PS_MEM_FREE(WUEPS_PID_AT, data);
        data = VOS_NULL_PTR;

        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^DOSIGEXEVENT: %d,%s%s", g_atCrLf,
        sigExEvent->paraLen * AT_DOUBLE_LENGTH, data, g_atCrLf);

    PS_MEM_FREE(WUEPS_PID_AT, data);
    data = VOS_NULL_PTR;

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNoCardModeSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg = VOS_NULL_PTR;
    MTA_AT_ResultCnf *cnf    = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT8         indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    cnf      = (MTA_AT_ResultCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NOCARDMODE_SET) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeSetCnf : Current Option is not AT_CMD_NOCARDMODE_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化AT^CNOCARDMODE命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (cnf->result != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNoCardModeQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_NoCardModeQryCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_NoCardModeQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NOCARDMODE_QRY) {
        AT_WARN_LOG("AT_RcvMtaNoCardModeQryCnf : Current Option is not AT_CMD_NOCARDMODE_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                mtaCnf->enableFlag);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetCdmaModemCapPara(PLATAFORM_RatCapability *sourceModemPlatform,
                                  PLATAFORM_RatCapability *destinationModemPlatform, ModemIdUint16 sourceModemId,
                                  ModemIdUint16 destinationModemId, VOS_UINT8 indexNum)
{
    AT_MTA_CdmaModemCapSetReq atCmd;
    errno_t                   memResult;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    AT_WARN_LOG2("AT_SetCdmaModemCapPara: Entry enSourceModemId and enDestinationModemId !", destinationModemId,
                 destinationModemId);

    atCmd.sourceModemId      = sourceModemId;
    atCmd.destinationModemId = destinationModemId;
    memResult = memcpy_s(&(atCmd.sourceModemPlatform), sizeof(atCmd.sourceModemPlatform), sourceModemPlatform,
                         sizeof(PLATAFORM_RatCapability));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.sourceModemPlatform), sizeof(PLATAFORM_RatCapability));

    memResult = memcpy_s(&(atCmd.destinationModemPlatform), sizeof(atCmd.destinationModemPlatform),
                         destinationModemPlatform, sizeof(PLATAFORM_RatCapability));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.destinationModemPlatform), sizeof(PLATAFORM_RatCapability));

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_CDMA_MODEM_CAP_SET_REQ,
                               (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_CdmaModemCapSetReq),
                               I0_UEPS_PID_MTA) == TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetCdmaModemCapPara,AT_FillAndSndAppReqMsg return SUCCESS");
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CDMAMODEMSWITCH_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_ERR_LOG("AT_SetCdmaModemCapPara,AT_FillAndSndAppReqMsg return ERROR");
        return AT_ERROR;
    }
}


LOCAL VOS_UINT32 AT_CdmaModemIdSet(VOS_UINT32 modem0SupportCMode, VOS_UINT32 modem1SupportCMode,
                                   AT_MTA_CdmaCapResumeSetReq *atCmd)
{
    if ((modem0SupportCMode == VOS_TRUE) && (modem1SupportCMode == VOS_FALSE)) {
        atCmd->cdmaModemId = MODEM_ID_0;
        return AT_SUCCESS;
    } else if ((modem0SupportCMode == VOS_FALSE) && (modem1SupportCMode == VOS_TRUE)) {
        atCmd->cdmaModemId = MODEM_ID_1;
        return AT_SUCCESS;
    } else if ((modem0SupportCMode == VOS_FALSE) && (modem1SupportCMode == VOS_FALSE)) {
        /*  两个modem都没有cdma能力，不需要恢复 */
        AT_NORM_LOG("At_SetCdmaCapResume:Modem0 and modem1 not have cdma capa!");
        return AT_OK;
    } else { /* 当前不支持双cdma能力，如果配置，启动失败，物理层初始化失败 */
        AT_ERR_LOG("At_SetCdmaCapResume:Modem0 and modem1 both have cdma capa!");
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetCdmaCapResume(VOS_UINT8 indexNum)
{
    PLATAFORM_RatCapability    modem0PlatRat;
    PLATAFORM_RatCapability    modem1PlatRat;
    AT_MTA_CdmaCapResumeSetReq atCmd;
    VOS_UINT32                 modem0Support1XFlg;
    VOS_UINT32                 modem0SupportHrpdFlg;
    VOS_UINT32                 modem1Support1XFlg;
    VOS_UINT32                 modem1SupportHrpdFlg;
    VOS_UINT32                 modem0SupportCMode;
    VOS_UINT32                 modem1SupportCMode;
    VOS_UINT32                 rslt;
    errno_t                    memResult;
    ModemIdUint16              modemId;

    (VOS_VOID)memset_s(&modem0PlatRat, sizeof(modem0PlatRat), 0x00, sizeof(modem0PlatRat));
    (VOS_VOID)memset_s(&modem1PlatRat, sizeof(modem1PlatRat), 0x00, sizeof(modem1PlatRat));
    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    AT_WARN_LOG("At_SetCdmaCapResume Entry: ");

    /* 改命令仅支持在Modem0上发起 */
    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("At_SetCdmaCapResume: Get modem id fail!");

        return AT_ERROR;
    }

    if (modemId != MODEM_ID_0) {
        AT_ERR_LOG1("At_SetCdmaCapResume:  modem is not modem0!", modemId);
        return AT_ERROR;
    }

    /* 读取nv出厂区配置 */
    if (TAF_ACORE_NV_READ_FACTORY(MODEM_ID_0, NV_ITEM_PLATFORM_RAT_CAP, &modem0PlatRat,
                                  sizeof(PLATAFORM_RatCapability)) != NV_OK) {
        AT_ERR_LOG("At_SetCdmaCapResume: Read Nv Fail, NvId PlatForm_Rat_Cap,Modem0!");
        return AT_ERROR;
    } else {
        AT_WARN_LOG("Read PlatForm from Factory success: ");
        AT_ReadPlatFormPrint(MODEM_ID_0, modem0PlatRat);
    }

    /* 读取nv出厂区配置 */
    if (TAF_ACORE_NV_READ_FACTORY(MODEM_ID_1, NV_ITEM_PLATFORM_RAT_CAP, &modem1PlatRat,
                                  sizeof(PLATAFORM_RatCapability)) != NV_OK) {
        AT_ERR_LOG("At_SetCdmaCapResume: Read Nv Fail, NvId PlatForm_Rat_Cap,Modem1!");
        return AT_ERROR;
    } else {
        AT_WARN_LOG("Read PlatForm from Factory success: ");
        AT_ReadPlatFormPrint(MODEM_ID_0, modem1PlatRat);
    }
    if ((modem0PlatRat.ratNum == 0) || (modem1PlatRat.ratNum == 0)) {
        AT_ERR_LOG("At_SetCdmaCapResume: Factory PlatForm RatNum is 0");
        return AT_ERROR;
    }

    memResult = memcpy_s(&(atCmd.modem0Platform), sizeof(atCmd.modem0Platform), &modem0PlatRat,
                         sizeof(PLATAFORM_RatCapability));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.modem0Platform), sizeof(PLATAFORM_RatCapability));

    memResult = memcpy_s(&(atCmd.modem1Platform), sizeof(atCmd.modem1Platform), &modem1PlatRat,
                         sizeof(PLATAFORM_RatCapability));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.modem1Platform), sizeof(PLATAFORM_RatCapability));

    /*
     * 检查cdma能力在哪个modem上，为了防止出现两个modem都存在cdma能力(会复位)，
     * 恢复时，先恢复无cdma能力的modem
     */
    modem0Support1XFlg   = AT_IsPlatformSupport1XMode(&modem0PlatRat);
    modem0SupportHrpdFlg = AT_IsPlatformSupportHrpdMode(&modem0PlatRat);
    modem1Support1XFlg   = AT_IsPlatformSupport1XMode(&modem1PlatRat);
    modem1SupportHrpdFlg = AT_IsPlatformSupportHrpdMode(&modem1PlatRat);

    modem0SupportCMode = (modem0Support1XFlg | modem0SupportHrpdFlg);
    modem1SupportCMode = (modem1Support1XFlg | modem1SupportHrpdFlg);

    rslt = AT_CdmaModemIdSet(modem0SupportCMode, modem1SupportCMode, &atCmd);
    if (rslt != AT_SUCCESS) {
        return rslt;
    }

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_CDMA_CAP_RESUME_SET_REQ,
                               (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_CdmaCapResumeSetReq),
                               I0_UEPS_PID_MTA) == TAF_SUCCESS) {
        AT_ERR_LOG("At_SetCdmaCapResume, AT_FillAndSndAppReqMsg CDMA_CAP_RESUME_SET_REQ return SUCCESS");
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CDMACAPRESUME_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_ERR_LOG("At_SetCdmaCapResume,AT_FillAndSndAppReqMsg return ERROR");
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryFactoryCdmaCap(VOS_UINT8 indexNum)
{
    PLATAFORM_RatCapability modem0PlatRat;
    PLATAFORM_RatCapability modem1PlatRat;
    VOS_UINT32              modem0Support1XFlg;
    VOS_UINT32              modem0SupportHrpdFlg;
    VOS_UINT32              modem1Support1XFlg;
    VOS_UINT32              modem1SupportHrpdFlg;
    VOS_UINT32              modem0SupportCMode;
    VOS_UINT32              modem1SupportCMode;
    VOS_UINT32              checkRlt;
    VOS_UINT32              allModemNotSupportCMode;

    VOS_UINT16 length;

    (VOS_VOID)memset_s(&modem0PlatRat, sizeof(modem0PlatRat), 0x00, sizeof(modem0PlatRat));
    (VOS_VOID)memset_s(&modem1PlatRat, sizeof(modem1PlatRat), 0x00, sizeof(modem1PlatRat));
    checkRlt                = AT_SUCCESS;
    allModemNotSupportCMode = VOS_FALSE;

    AT_NORM_LOG("At_QryFactoryCdmaCap Entry: ");

    /*  查询读取nv，修改为从备份区读取，不从工作区读取 */
    if (TAF_ACORE_NV_READ_FACTORY(MODEM_ID_0, NV_ITEM_PLATFORM_RAT_CAP, &modem0PlatRat,
                                  sizeof(PLATAFORM_RatCapability)) != NV_OK) {
        AT_WARN_LOG("Read PlatForm from Factory fail: MODEM_ID_0");
        return AT_ERROR;
    } else {
        AT_NORM_LOG("Read PlatForm from Factory success: ");
        AT_ReadPlatFormPrint(MODEM_ID_0, modem0PlatRat);
    }

    if (TAF_ACORE_NV_READ_FACTORY(MODEM_ID_1, NV_ITEM_PLATFORM_RAT_CAP, &modem1PlatRat,
                                  sizeof(PLATAFORM_RatCapability)) != NV_OK) {
        AT_WARN_LOG("Read PlatForm from Factory fail: MODEM_ID_1");
        return AT_ERROR;
    } else {
        AT_NORM_LOG("Read PlatForm from Factory success: ");
        AT_ReadPlatFormPrint(MODEM_ID_1, modem1PlatRat);
    }

    modem0Support1XFlg   = AT_IsPlatformSupport1XMode(&modem0PlatRat);
    modem0SupportHrpdFlg = AT_IsPlatformSupportHrpdMode(&modem0PlatRat);
    modem1Support1XFlg   = AT_IsPlatformSupport1XMode(&modem1PlatRat);
    modem1SupportHrpdFlg = AT_IsPlatformSupportHrpdMode(&modem1PlatRat);

    AT_NORM_LOG2("At_QryFactoryCdmaCap modem0 cdma cpa: ", modem0Support1XFlg, modem0SupportHrpdFlg);
    AT_NORM_LOG2("At_QryFactoryCdmaCap modem1 cdma cpa: ", modem1Support1XFlg, modem1SupportHrpdFlg);

    modem0SupportCMode = (modem0Support1XFlg | modem0SupportHrpdFlg);
    modem1SupportCMode = (modem1Support1XFlg | modem1SupportHrpdFlg);

    /* 如果modem0和modem1同时满足支持，则认为设置失败 */
    if ((modem0SupportCMode == VOS_TRUE) && (modem1SupportCMode == VOS_TRUE)) {
        checkRlt = AT_ERROR;
    }

    if ((modem0SupportCMode == VOS_FALSE) && (modem1SupportCMode == VOS_FALSE)) {
        checkRlt                = AT_ERROR;
        allModemNotSupportCMode = VOS_TRUE;
    }

    if (checkRlt == AT_ERROR) {
        if (allModemNotSupportCMode == VOS_TRUE) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: -1", g_parseContext[indexNum].cmdElement->cmdName);

            g_atSendDataBuff.bufLen = length;

            return AT_OK;
        }

        return AT_ERROR;
    }

    /* 1X,DO在Modem0 */
    if (modem0SupportCMode == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 0", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    }

    /* 1X,DO在Modem1 */
    if (modem1SupportCMode == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 1", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;
    }

    return AT_OK;
}

#endif


VOS_VOID AT_ReadPlatFormPrint(ModemIdUint16 modemId, PLATAFORM_RatCapability platRat)
{
    VOS_UINT32 i;

    AT_WARN_LOG1("Read PlatForm ModemId: ", modemId);
    AT_WARN_LOG1("Read PlatForm RatNum: ", platRat.ratNum);

    for (i = 0; (i < platRat.ratNum) && (i < PLATFORM_MAX_RAT_NUM); i++) {
        AT_WARN_LOG1("Read PlatForm RatType: ", platRat.ratList[i]);
    }
}


VOS_UINT32 AT_SetFratIgnitionPara(VOS_UINT8 indexNum)
{
    VOS_UINT32               rst;
    AT_MTA_FratIgnitionUnit8 fratIgnitionSta;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数目检查 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ucFratIgnitionSta 取值错误 */
    if ((g_atParaList[0].paraValue >= AT_MTA_FRAT_IGNITION_STATT_BUTT) || (g_atParaList[0].paraLen == 0)) {
        AT_WARN_LOG1("AT_SetFratIgnitionPara: para err", g_atParaList[0].paraValue);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    fratIgnitionSta = (AT_MTA_FratIgnitionUnit8)g_atParaList[0].paraValue;

    /* 发送消息 ID_AT_MTA_FRAT_IGNITION_SET_REQ 给MTA处理，该消息带参数(VOS_UINT8)g_atParaList[0].ulParaValue */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_FRAT_IGNITION_SET_REQ,
                                 (VOS_UINT8 *)&fratIgnitionSta, sizeof(fratIgnitionSta), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FRATIGNITION_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetFratIgnitionPara: send ReqMsg fail");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TimeParaYTDCheck(AT_MTA_ModemTime *atMtaModemTime)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    /* 参数个数或者VER长度不正确 */
    if (g_atParaList[AT_TIMESET_DATE].paraLen != AT_MODEM_YTD_LEN) {
        AT_ERR_LOG("AT_TimeParaYTDCheck: length of YTD parameter is error.");
        return VOS_ERR;
    }

    /* 按照格式 YYYY/MM/DD 解析年月日，并判断格式、范围 */
    if ((g_atParaList[AT_TIMESET_DATE].para[AT_MODEM_YEAR_LEN] != '/') ||
         /* 1为日期的分隔符宽度 */
        (g_atParaList[AT_TIMESET_DATE].para[AT_MODEM_YEAR_LEN+1+AT_MODEM_MONTH_LEN] != '/')) {
        AT_ERR_LOG("AT_TimeParaYTDCheck: The date formats parameter is error.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), g_atParaList[AT_TIMESET_DATE].para, AT_MODEM_YEAR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_YEAR_LEN);
    atMtaModemTime->year = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->year > AT_MODEM_YEAR_MAX) || (atMtaModemTime->year < AT_MODEM_YEAR_MIN)) {
        AT_ERR_LOG("AT_TimeParaYTDCheck: The parameter of year is out of range");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_TIMESET_DATE].para[AT_MODEM_YEAR_LEN+1],
                         AT_MODEM_MONTH_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_MONTH_LEN);
    atMtaModemTime->month = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->month > AT_MODEM_MONTH_MAX) || (atMtaModemTime->month < AT_MODEM_MONTH_MIN)) {
        AT_ERR_LOG("AT_TimeParaYTDCheck: The parameter of month is out of range");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer),&g_atParaList[AT_TIMESET_DATE].para[AT_MODEM_YEAR_LEN + 1+
                         AT_MODEM_MONTH_LEN +1], AT_MODEM_DATE_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_DATE_LEN);
    atMtaModemTime->day = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->day > AT_MODEM_DAY_MAX) || (atMtaModemTime->day < AT_MODEM_DAY_MIN)) {
        AT_ERR_LOG("AT_TimeParaYTDCheck: The parameter of day is out of range");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_TimeParaTimeCheck(AT_MTA_ModemTime *atMtaModemTime)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    if (g_atParaList[AT_TIMESET_TIME].paraLen != AT_MODEM_TIME_LEN) {
        AT_ERR_LOG("AT_TimeParaTimeCheck: length of time parameter is error.");
        return VOS_ERR;
    }

    /* 按照格式 HH:MM:SS 解析时间，并判断格式、范围 */
    /* 1为日期的分隔符宽度 */
    if ((g_atParaList[AT_TIMESET_TIME].para[AT_MODEM_HOUR_LEN] != ':') ||
        (g_atParaList[AT_TIMESET_TIME].para[AT_MODEM_HOUR_LEN + 1 + AT_MODEM_MIN_LEN] != ':')) {
        AT_ERR_LOG("AT_TimeParaTimeCheck: The time formats parameter is error.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), g_atParaList[AT_TIMESET_TIME].para, AT_MODEM_HOUR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_HOUR_LEN);
    atMtaModemTime->hour = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->hour > AT_MODEM_HOUR_MAX) || (atMtaModemTime->hour < AT_MODEM_HOUR_MIN)) {
        AT_ERR_LOG("AT_TimeParaTimeCheck: The parameter of hour is out of range");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer),
                         &g_atParaList[AT_TIMESET_TIME].para[AT_MODEM_HOUR_LEN + 1], AT_MODEM_MIN_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_MIN_LEN);
    atMtaModemTime->min = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->min > AT_MODEM_MIN_MAX) || (atMtaModemTime->min < AT_MODEM_MIN_MIN)) {
        AT_ERR_LOG("AT_TimeParaTimeCheck: The parameter of min is out of range");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer),&g_atParaList[AT_TIMESET_TIME].para[AT_MODEM_HOUR_LEN + 1+
                         AT_MODEM_MIN_LEN + 1], AT_MODEM_SEC_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_SEC_LEN);
    atMtaModemTime->sec = (VOS_INT32)AT_AtoI((VOS_CHAR *)buffer);

    if ((atMtaModemTime->sec > AT_MODEM_SEC_MAX) || (atMtaModemTime->sec < AT_MODEM_SEC_MIN)) {
        AT_ERR_LOG("AT_TimeParaTimeCheck: The parameter of second is out of range");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_TimeParaZoneCheck(AT_MTA_ModemTime *atMtaModemTime)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    if ((g_atParaList[AT_TIMESET_ZONE].paraLen == 0) ||
        (g_atParaList[AT_TIMESET_ZONE].paraLen >= AT_GET_MODEM_TIME_BUFF_LEN)) {
        AT_ERR_LOG1("AT_TimeParaZoneCheck: length of zone parameter is wrong.", g_atParaList[AT_TIMESET_ZONE].paraLen);
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), g_atParaList[AT_TIMESET_ZONE].para,
                         g_atParaList[AT_TIMESET_ZONE].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), g_atParaList[AT_TIMESET_ZONE].paraLen);

    /* 时区范围是[-12, 12] */
    if (AT_AtoInt((VOS_CHAR *)buffer, &atMtaModemTime->zone) == VOS_ERR) {
        return VOS_ERR;
    }

    if ((atMtaModemTime->zone > AT_MODEM_ZONE_MAX) || (atMtaModemTime->zone < AT_MODEM_ZONE_MIN)) {
        AT_ERR_LOG("AT_TimeParaZoneCheck: The parameter of zone is out of range.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetModemTimePara(VOS_UINT8 indexNum)
{
    AT_MTA_ModemTime atMtaModemTime;
    VOS_UINT32       rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数目检查 */
    if (g_atParaIndex != AT_TIMESET_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数格式检查 */
    (VOS_VOID)memset_s(&atMtaModemTime, (VOS_SIZE_T)sizeof(atMtaModemTime), 0x00, (VOS_SIZE_T)sizeof(atMtaModemTime));

    /* 检查年月日 */
    if (AT_TimeParaYTDCheck(&atMtaModemTime) == VOS_ERR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 检查时间 */
    if (AT_TimeParaTimeCheck(&atMtaModemTime) == VOS_ERR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 检查时区 */
    if (AT_TimeParaZoneCheck(&atMtaModemTime) == VOS_ERR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送消息 ID_AT_MTA_MODEM_TIME_SET_REQ 给MTA处理，该消息带参数 stAtMtaModemTime */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_MODEM_TIME_SET_REQ,
                                 (VOS_UINT8 *)&atMtaModemTime, (VOS_SIZE_T)sizeof(atMtaModemTime), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MODEM_TIME_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetModemTimePara: send ReqMsg fail");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetPhyComCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_PhyComCfgSetReq phyComCfg;
    VOS_UINT32             result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&phyComCfg, (VOS_SIZE_T)sizeof(phyComCfg), 0x00, (VOS_SIZE_T)sizeof(phyComCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetPhyComCfg : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 判断参数数量 */
    if ((g_atParaIndex < AT_PHYCOMCFG_MIN_PARA_NUM) || (g_atParaIndex > AT_PHYCOMCFG_MAX_PARA_NUM)) {
        /* 参数数量错误 */
        AT_WARN_LOG("AT_SetPhyComCfg : Current Number wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 消息赋值 */
    phyComCfg.cmdType   = (VOS_UINT16)g_atParaList[0].paraValue;
    phyComCfg.ratBitmap = (VOS_UINT16)g_atParaList[1].paraValue;
    phyComCfg.para1     = g_atParaList[AT_PHYCOMCFG_PARA1].paraValue;

#if (FEATURE_UE_MODE_CDMA != FEATURE_ON)
    if (((phyComCfg.ratBitmap & AT_MTA_PHY_COM_CFG_RAT_1X) != 0) ||
        ((phyComCfg.ratBitmap & AT_MTA_PHY_COM_CFG_RAT_HRPD) != 0)) {
        AT_WARN_LOG("AT_SetPhyComCfgPara :rat 1x/HRPD not support.");
        return AT_CME_INCORRECT_PARAMETERS;
    }
#endif

    if (g_atParaIndex == AT_CONST_NUM_4) {
        phyComCfg.para2 = g_atParaList[AT_PHYCOMCFG_PARA2].paraValue;
    } else {
        phyComCfg.para2 = g_atParaList[AT_PHYCOMCFG_PARA2].paraValue;
        phyComCfg.para3 = g_atParaList[AT_PHYCOMCFG_PARA3].paraValue;
    }

    /* 发送消息给MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_PHY_COM_CFG_SET_REQ, (VOS_UINT8 *)&phyComCfg,
                                    (VOS_SIZE_T)sizeof(phyComCfg), I0_UEPS_PID_MTA);

    /* 发送失败 */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetPhyComCfg: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 发送成功，设置当前操作模式 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHY_COM_CFG_SET;

    /* 等待异步处理时间返回 */
    return AT_WAIT_ASYNC_RETURN;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_SetNrphyComCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrphyComCfgSetReq nrPhyComCfg;
    VOS_UINT32               result;
    VOS_UINT32               i;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&nrPhyComCfg, sizeof(nrPhyComCfg), 0x00, sizeof(nrPhyComCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNrphyComCfgPara : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 判断参数数量 */
    if ((g_atParaIndex > AT_NRPHY_COMCFG_MAX_PARA_NUM) || (g_atParaIndex < AT_NRPHY_COMCFG_MIN_PARA_NUM)) {
        /* 参数数量错误 */
        AT_WARN_LOG("AT_SetNrphyComCfgPara : Current Number wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 消息赋值 */
    nrPhyComCfg.cmdType = (VOS_UINT16)g_atParaList[0].paraValue;
    nrPhyComCfg.paraNum = (VOS_UINT16)(g_atParaIndex - 1);
    for(i = 0; i < nrPhyComCfg.paraNum; i++){
        nrPhyComCfg.paraList[i] = g_atParaList[i + 1].paraValue;
    }

    /* 发送消息给MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NRPHY_COM_CFG_SET_REQ, (VOS_UINT8 *)&nrPhyComCfg,(VOS_SIZE_T)sizeof(nrPhyComCfg), I0_UEPS_PID_MTA);

    /* 发送失败 */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetNrphyComCfgPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 发送成功，设置当前操作模式 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRPHY_COM_CFG_SET;

    /* 等待异步处理时间返回 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaNrPhyComCfgSetCnf(struct MsgCB *msg)
{
    MTA_AT_NrphyComCfgSetCnf *nrPhyComCfgCnf = VOS_NULL_PTR;
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    VOS_UINT8                 indexNum;

    /* 初始化局部变量 */
    rcvMsg = (AT_MTA_Msg *)msg;
    nrPhyComCfgCnf = (MTA_AT_NrphyComCfgSetCnf *)rcvMsg->content;
    indexNum     = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrPhyComCfgSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrPhyComCfgSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NRPHY_COM_CFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRPHY_COM_CFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNrPhyComCfgSetCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化上报命令 */
    if (nrPhyComCfgCnf->result != MTA_AT_RESULT_NO_ERROR) {
        /* 命令结果 *AT_ERROR */
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        /* 命令结果 *AT_OK */
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 At_SetLowPowerModePara(VOS_UINT8 indexNum)
{
    AT_MTA_LowPwrModeReq lowPwrModeReq;

    /* 初始化 */
    (VOS_VOID)memset_s(&lowPwrModeReq, sizeof(lowPwrModeReq), 0x0, sizeof(lowPwrModeReq));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetLowPowerModePara:Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 判断参数数量 */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        /* 参数数量错误 */
        AT_WARN_LOG("At_SetLowPowerModePara : Para wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    lowPwrModeReq.rat = (AT_MTA_LowPwrModeRatUint8)g_atParaList[0].paraValue;

    /* 给MTA发消息 */
    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               ID_AT_MTA_LOW_PWR_MODE_SET_REQ, (VOS_UINT8 *)&lowPwrModeReq, sizeof(lowPwrModeReq),
                               I0_UEPS_PID_MTA) == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LOWPWRMODE_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}
#endif


VOS_UINT32 AT_RcvMtaPhyComCfgSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg       = VOS_NULL_PTR;
    MTA_AT_PhyComCfgSetCnf *phyComCfgCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;

    /* 初始化局部变量 */
    rcvMsg       = (AT_MTA_Msg *)msg;
    phyComCfgCnf = (MTA_AT_PhyComCfgSetCnf *)rcvMsg->content;
    indexNum     = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPhyComCfgSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaPhyComCfgSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PHY_COM_CFG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PHY_COM_CFG_SET) {
        AT_WARN_LOG("AT_RcvMtaPhyComCfgSetCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    /* 格式化上报命令 */
    if (phyComCfgCnf->result != MTA_AT_RESULT_NO_ERROR) {
        /* 命令结果 *AT_ERROR */
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        /* 命令结果 *AT_OK */
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}



VOS_UINT32 AT_GetValidSamplePara(AT_MTA_SetSampleReq *atCmd)
{
    VOS_INT32 temp;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数大于4个，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex > AT_SAMPLE_MAX_PARA_NUM) {
        AT_WARN_LOG("AT_GetValidNetScanPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数长度为0，返回AT_CME_INCORR ECT_PARAMETERS */
    if (g_atParaList[AT_SAMPLE_PARA_REQTYPE].paraLen == 0) {
        AT_WARN_LOG("AT_GetValidNetScanPara: para0 Length = 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atCmd->reqType = (VOS_UINT16)g_atParaList[AT_SAMPLE_PARA_REQTYPE].paraValue;

    if (g_atParaList[AT_SAMPLE_PARA_TEMPRANGE].paraLen == 0) {
        atCmd->tempRange = AT_TEMP_ZONE_DEFAULT;
    } else {
        atCmd->tempRange = (VOS_UINT16)g_atParaList[AT_SAMPLE_PARA_TEMPRANGE].paraValue;
    }

    if (g_atParaList[AT_SAMPLE_PARA_PPMOFFSET].paraLen == 0) {
        atCmd->ppmOffset = 0;
    } else {
        temp = 0;

        if (AT_AtoInt((VOS_CHAR *)g_atParaList[AT_SAMPLE_PARA_PPMOFFSET].para, &temp) == VOS_ERR) {
            AT_ERR_LOG("AT_GetValidNetScanPara: para2 err");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        atCmd->ppmOffset = (VOS_INT16)temp;
    }

    if (g_atParaList[AT_SAMPLE_PARA_TIMEOFFSET].paraLen == 0) {
        atCmd->timeOffset = 0;
    } else {
        temp = 0;

        if (AT_AtoInt((VOS_CHAR *)g_atParaList[AT_SAMPLE_PARA_TIMEOFFSET].para, &temp) == VOS_ERR) {
            AT_ERR_LOG("AT_GetValidNetScanPara: para3 err");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        atCmd->timeOffset = (VOS_INT16)temp;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetSamplePara(VOS_UINT8 indexNum)
{
    AT_MTA_SetSampleReq atCmd;
    VOS_UINT32          result;
    ModemIdUint16       modemId;
    VOS_UINT32          ret;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    modemId = MODEM_ID_0;
    ret     = AT_GetModemIdFromClient(indexNum, &modemId);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_SetSamplePara: Get modem id fail.");
        return AT_ERROR;
    }

    if (modemId != MODEM_ID_0) {
        AT_ERR_LOG("AT_SetSamplePara: MODEM_ID ERROR");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ret = AT_GetValidSamplePara(&atCmd);

    if (ret != AT_OK) {
        AT_ERR_LOG("AT_SetSamplePara: AT_GetValidNetScanPara Failed");
        return ret;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_SET_SAMPLE_REQ, (VOS_UINT8 *)&atCmd,
                                    sizeof(AT_MTA_SetSampleReq), I0_UEPS_PID_MTA);

    if (result != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetSamplePara: AT_FillAndSndAppReqMsg Failed!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SAMPLE_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSetSampleCnf(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetSampleCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32           result;
    VOS_UINT8            indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetSampleCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetSampleCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetSampleCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SAMPLE_SET) {
        AT_WARN_LOG("AT_RcvMtaSetSampleCnf : Current Option is not AT_CMD_SAMPLE_SET.");
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



VOS_UINT32 AT_SetRxTestModePara(VOS_UINT8 indexNum)
{
    AT_MTA_SetRxtestmodeReq rxTestModeCfg;
    VOS_UINT32              rst;

    (VOS_VOID)memset_s(&rxTestModeCfg, (VOS_SIZE_T)sizeof(rxTestModeCfg), 0x00, (VOS_SIZE_T)sizeof(rxTestModeCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetRxTestModePara : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetRxTestModePara : The number of input parameters is error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetRxTestModePara : The number of input parameters is zero.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rxTestModeCfg.cfg = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 发送跨核消息到C核, 设置侦听测试模式 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_RX_TEST_MODE_SET_REQ, (VOS_UINT8 *)&rxTestModeCfg,
                                 (VOS_SIZE_T)sizeof(rxTestModeCfg), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetRxTestModePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RXTESTMODE_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSetRxTestModeCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetRxtestmodeCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetRxtestmodeCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetRxTestModeCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetRxTestModeCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RXTESTMODE_SET) {
        AT_WARN_LOG("AT_RcvMtaSetRxTestModeCnf : Current Option is not AT_CMD_RXTESTMODE_SET.");
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


VOS_UINT32 AT_SetMipiWrParaEx(VOS_UINT8 indexNum)
{
    AT_MTA_MipiWrexReq mipiWrEx;
    VOS_UINT32         result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&mipiWrEx, (VOS_SIZE_T)sizeof(mipiWrEx), 0x00, (VOS_SIZE_T)sizeof(mipiWrEx));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetMipiWrParaEx : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量不对 */
    if (g_atParaIndex != AT_MIPIWREX_PARA_NUM) {
        AT_WARN_LOG("AT_SetMipiWrParaEx : Current Number wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* extend_flag 为 0 */
    if (g_atParaList[0].paraValue == 0) {
        /* byte_cnt 如果不为1 或者 reg_addr 不在0-31之间,数值大于0XFF,直接返回错误 */
        if ((g_atParaList[AT_MIPIWREX_BYTE_CNT].paraValue != AT_ETDFLG_IS_0_BYTE_CNT_VALID) ||
            (g_atParaList[AT_MIPIWREX_REG_ADDR].paraValue > AT_ETDFLG_IS_0_REG_ADDR_MAX) ||
            (g_atParaList[AT_MIPIWREX_VALUE].paraValue > 0xFF)) {
            AT_WARN_LOG("AT_SetMipiWrParaEx : extend_flag is 0, byte_cnt or reg_addr or value wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        mipiWrEx.extendFlag = g_atParaList[0].paraValue;
        mipiWrEx.mipiId     = g_atParaList[1].paraValue;
        mipiWrEx.secondaryId = g_atParaList[AT_MIPIWREX_SECONDARY_ID].paraValue;
        mipiWrEx.regAddr    = g_atParaList[AT_MIPIWREX_REG_ADDR].paraValue;
        mipiWrEx.byteCnt    = g_atParaList[AT_MIPIWREX_BYTE_CNT].paraValue;
        mipiWrEx.value      = g_atParaList[AT_MIPIWREX_VALUE].paraValue;
    }
    /* extend_flag 为 1 */
    else if (g_atParaList[0].paraValue == 1) {
        /* reg_addr 不在0-255之间, 直接返回错误 */
        if ((g_atParaList[AT_MIPIWREX_BYTE_CNT].paraValue == AT_ETDFLG_IS_1_BYTE_CNT_VALID1) &&
            (g_atParaList[AT_MIPIWREX_VALUE].paraValue > 0xFF)) {
            AT_WARN_LOG("AT_SetMipiWrParaEx : extend_flag is 1, byte_cnt is 1, value wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if ((g_atParaList[AT_MIPIWREX_BYTE_CNT].paraValue == AT_ETDFLG_IS_1_BYTE_CNT_VALID2) &&
            (g_atParaList[AT_MIPIWREX_VALUE].paraValue > 0xFFFF)) {
            AT_WARN_LOG("AT_SetMipiWrParaEx : extend_flag is 1, byte_cnt is 2, value wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if (g_atParaList[AT_MIPIWREX_REG_ADDR].paraValue > AT_ETDFLG_IS_1_REG_ADDR_MAX) {
            AT_WARN_LOG("AT_SetMipiWrParaEx : extend_flag is 1, reg_addr wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        mipiWrEx.extendFlag = g_atParaList[0].paraValue;
        mipiWrEx.mipiId     = g_atParaList[1].paraValue;
        mipiWrEx.secondaryId = g_atParaList[AT_MIPIWREX_SECONDARY_ID].paraValue;
        mipiWrEx.regAddr    = g_atParaList[AT_MIPIWREX_REG_ADDR].paraValue;
        mipiWrEx.byteCnt    = g_atParaList[AT_MIPIWREX_BYTE_CNT].paraValue;
        mipiWrEx.value      = g_atParaList[AT_MIPIWREX_VALUE].paraValue;
    } else {
        AT_WARN_LOG("AT_SetMipiWrParaEx : extend_flag wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 发送消息给MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_MIPI_WREX_REQ, (VOS_UINT8 *)&mipiWrEx, (VOS_SIZE_T)sizeof(mipiWrEx),
                                    I0_UEPS_PID_MTA);

    /* 发送失败 */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMipiWrParaEx: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 发送成功，设置当前操作模式 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPI_WREX;

    /* 等待异步处理时间返回 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaMipiWrEXCnf(struct MsgCB *msg)
{
    AT_MTA_Msg         *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_MipiWrexCnf *mipiWrEXCnf = VOS_NULL_PTR;
    VOS_UINT8           indexNum;

    /* 初始化局部变量 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    mipiWrEXCnf = (MTA_AT_MipiWrexCnf *)rcvMsg->content;
    indexNum    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMiPiWrEXCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMiPiWrEXCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MIPI_WREX */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPI_WREX) {
        AT_WARN_LOG("AT_RcvMtaMiPiWrEXCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化上报命令 */
    if (mipiWrEXCnf->result != MTA_AT_RESULT_NO_ERROR) {
        /* 命令结果 *AT_ERROR */
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        /* 命令结果 *AT_OK */
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetMipiRdParaEx(VOS_UINT8 indexNum)
{
    AT_MTA_MipiRdexReq mipiRdEx;
    VOS_UINT32         result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&mipiRdEx, (VOS_SIZE_T)sizeof(mipiRdEx), 0x00, (VOS_SIZE_T)sizeof(mipiRdEx));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetMipiRdParaEx : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量不对 */
    if ((g_atParaIndex != AT_CONST_NUM_5) && (g_atParaIndex != AT_CONST_NUM_6)) {
        AT_WARN_LOG("AT_SetMipiRdParaEx : Current Numbers Wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* extend_flag 为 0 */
    if (g_atParaList[0].paraValue == 0) {
        /* byte_cnt 如果不为1 或者 reg_addr 不在0-31之间, 直接返回错误 */
        if ((g_atParaList[AT_MIPIRDEX_BYTE_CNT].paraValue != AT_ETDFLG_IS_0_BYTE_CNT_VALID) ||
            (g_atParaList[AT_MIPIRDEX_REG_ADDR].paraValue > AT_ETDFLG_IS_0_REG_ADDR_MAX)) {
            AT_WARN_LOG("AT_SetMipiRdParaEx : extend_flag is 0, byte_cnt or reg_addr wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    /* extend_flag 为 1 */
    else if (g_atParaList[0].paraValue == 1) {
        /* reg_addr 不在0-255之间, 直接返回错误 */
        if (g_atParaList[AT_MIPIRDEX_REG_ADDR].paraValue > AT_ETDFLG_IS_1_REG_ADDR_MAX) {
            AT_WARN_LOG("AT_SetMipiRdParaEx : extend_flag is 1, reg_addr wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
        AT_WARN_LOG("AT_SetMipiRdParaEx : extend_flag wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    mipiRdEx.extendFlag = g_atParaList[0].paraValue;
    mipiRdEx.mipiId     = g_atParaList[1].paraValue;
    mipiRdEx.secondaryId = g_atParaList[AT_MIPIRDEX_SECONDARY_ID].paraValue;
    mipiRdEx.regAddr    = g_atParaList[AT_MIPIRDEX_REG_ADDR].paraValue;
    mipiRdEx.byteCnt    = g_atParaList[AT_MIPIRDEX_BYTE_CNT].paraValue;
    if (g_atParaIndex == AT_CONST_NUM_6) {
        mipiRdEx.speedType = g_atParaList[AT_MIPIRDEX_SPEED_TYPE].paraValue;
    } else {
        /* 为了兼容之前的版本，该参数默认可以不填，按照全速进行READ操作 */
        mipiRdEx.speedType = 1;
    }

    /* 发送消息给MTA */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_MIPI_RDEX_REQ, (VOS_UINT8 *)&mipiRdEx, (VOS_SIZE_T)sizeof(mipiRdEx),
                                    I0_UEPS_PID_MTA);

    /* 发送失败 */
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetMipiRdParaEx: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 发送成功，设置当前操作模式 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MIPI_RDEX;

    /* 等待异步处理时间返回 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaMipiRdEXCnf(struct MsgCB *msg)
{
    AT_MTA_Msg         *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_MipiRdexCnf *miPiRdEXCnf = VOS_NULL_PTR;
    VOS_UINT16          length;
    VOS_UINT8           indexNum;

    /* 初始化局部变量 */
    rcvMsg      = (AT_MTA_Msg *)msg;
    miPiRdEXCnf = (MTA_AT_MipiRdexCnf *)rcvMsg->content;
    indexNum    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMiPiRdEXCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMiPiRdEXCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MIPI_WREX */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MIPI_RDEX) {
        AT_WARN_LOG("AT_RcvMtaMiPiRdEXCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化上报命令 */
    if (miPiRdEXCnf->result != MTA_AT_RESULT_NO_ERROR) {
        /* 命令结果 *AT_ERROR */
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        /* 命令结果 *AT_OK */
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, miPiRdEXCnf->value);
        g_atSendDataBuff.bufLen = length;
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetCrrconnPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetCrrconnReq setCrrconn;
    VOS_UINT32           rst;

    (VOS_VOID)memset_s(&setCrrconn, sizeof(setCrrconn), 0x00, sizeof(setCrrconn));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数赋值 */
    setCrrconn.enable = (AT_MTA_CfgUint8)g_atParaList[0].paraValue;

    /* 发送跨核消息到C核，设置CRRCONN主动上报设置 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_CRRCONN_SET_REQ, (VOS_UINT8 *)&setCrrconn, sizeof(setCrrconn),
                                 I0_UEPS_PID_MTA);
    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetCrrconnPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CRRCONN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryCrrconnPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MTA 发送查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_CRRCONN_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryCrrconnPara: send Msg fail.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CRRCONN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSetCrrconnCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetCrrconnCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetCrrconnCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetCrrconnCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetCrrconnCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRRCONN_SET) {
        AT_WARN_LOG("AT_RcvMtaSetCrrconnCnf : Current Option is not AT_CMD_CRRCONN_SET.");
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


VOS_UINT32 AT_RcvMtaQryCrrconnCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg           *mtaMsg        = VOS_NULL_PTR;
    MTA_AT_QryCrrconnCnf *qryCrrconnCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT8             indexNum = 0;

    /* 初始化消息变量 */
    mtaMsg        = (AT_MTA_Msg *)msg;
    qryCrrconnCnf = (MTA_AT_QryCrrconnCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaQryCrrconnCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaQryCrrconnCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_CRRCONN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRRCONN_QRY) {
        AT_WARN_LOG("AT_RcvMtaQryCrrconnCnf: WARNING:Not AT_CMD_CRRCONN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (qryCrrconnCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            qryCrrconnCnf->enable, qryCrrconnCnf->status0, qryCrrconnCnf->status1, qryCrrconnCnf->status2);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用AT_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaCrrconnStatusInd(struct MsgCB *msg)
{
    AT_MTA_Msg              *mtaMsg           = VOS_NULL_PTR;
    MTA_AT_CrrconnStatusInd *crrconnStatusInd = VOS_NULL_PTR;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum         = 0;
    mtaMsg           = (AT_MTA_Msg *)msg;
    crrconnStatusInd = (MTA_AT_CrrconnStatusInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCrrconnStatusInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CRRCONN: %d,%d,%d%s", g_atCrLf, crrconnStatusInd->status0,
        crrconnStatusInd->status1, crrconnStatusInd->status2, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetVtrlqualrptPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetVtrlqualrptReq setVtrlqualrpt;
    VOS_UINT32               rst;

    (VOS_VOID)memset_s(&setVtrlqualrpt, sizeof(setVtrlqualrpt), 0x00, sizeof(setVtrlqualrpt));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不正确 */
    if (g_atParaIndex != AT_VTRLQUALRPT_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数赋值 */
    setVtrlqualrpt.enable    = g_atParaList[0].paraValue;
    setVtrlqualrpt.threshold = g_atParaList[1].paraValue;

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_VTRLQUALRPT_SET_REQ, (VOS_UINT8 *)&setVtrlqualrpt,
                                 (VOS_SIZE_T)sizeof(setVtrlqualrpt), I0_UEPS_PID_MTA);
    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetVtrlqualrptPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VTRLQUALRPT_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSetVtrlqualrptCnf(struct MsgCB *msg)
{
    AT_MTA_Msg               *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SetVtrlqualrptCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32                result;
    VOS_UINT8                 indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_SetVtrlqualrptCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSetVtrlqualrptCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSetVtrlqualrptCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_VTRLQUALRPT_SET) {
        AT_WARN_LOG("AT_RcvMtaSetVtrlqualrptCnf : Current Option is not AT_CMD_VTRLQUALRPT_SET.");
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


VOS_UINT32 AT_RcvMtaRlQualityInfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg              *mtaMsg           = VOS_NULL_PTR;
    MTA_AT_RlQualityInfoInd *rlQualityInfoInd = VOS_NULL_PTR;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum         = 0;
    mtaMsg           = (AT_MTA_Msg *)msg;
    rlQualityInfoInd = (MTA_AT_RlQualityInfoInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaRlQualityInfoInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^LTERLQUALINFO: %d,%d,%d,%d%s", g_atCrLf, rlQualityInfoInd->rsrp,
        rlQualityInfoInd->rsrq, rlQualityInfoInd->rssi, rlQualityInfoInd->bler, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaVideoDiagInfoRpt(struct MsgCB *msg)
{
    AT_MTA_Msg              *mtaMsg           = VOS_NULL_PTR;
    MTA_AT_VideoDiagInfoRpt *videoDiagInfoRpt = VOS_NULL_PTR;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum         = 0;
    mtaMsg           = (AT_MTA_Msg *)msg;
    videoDiagInfoRpt = (MTA_AT_VideoDiagInfoRpt *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaVideoDiagInfoRpt: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^LPDCPINFORPT: %u,%u,%u,%u%s", g_atCrLf, videoDiagInfoRpt->currBuffTime,
        videoDiagInfoRpt->currBuffPktNum, videoDiagInfoRpt->macUlThrput, videoDiagInfoRpt->maxBuffTime, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetEccCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetEccCfgReq setEccCfgReq;
    VOS_UINT32          result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setEccCfgReq, sizeof(setEccCfgReq), 0x00, sizeof(setEccCfgReq));

    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为2个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != AT_ECCCFG_PARA_NUM) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数长度不为0，否则返回AT_CME_INCORR ECT_PARAMETERS */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_WARN_LOG("AT_CheckErrcCapCfgPara: Length = 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    setEccCfgReq.eccEnable = (PS_BOOL_ENUM_UINT8)g_atParaList[0].paraValue;
    setEccCfgReq.rptPeriod = g_atParaList[1].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_ECC_CFG_SET_REQ, (VOS_UINT8 *)&setEccCfgReq, sizeof(AT_MTA_SetEccCfgReq),
                                    I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECCCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_SetNrFreqLockPara(TAF_UINT8 indexNum)
{
    AT_MTA_SetNrfreqlockReq nrFreqLock;
    VOS_UINT32              rst;

    (VOS_VOID)memset_s(&nrFreqLock, sizeof(nrFreqLock), 0x00, sizeof(nrFreqLock));

    rst = AT_GetNrFreqLockPara(indexNum, &nrFreqLock);

    if (rst != AT_SUCCESS) {
        return rst;
    }

    /* 发送消息 ID_AT_MTA_NR_FREQLOCK_SET_REQ 给C核 AT 代理处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_NR_FREQLOCK_SET_REQ,
                                 (VOS_UINT8 *)&nrFreqLock, sizeof(AT_MTA_SetNrfreqlockReq), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRFREQLOCK_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_GetNrFreqLockPara(VOS_UINT8 clientId, AT_MTA_SetNrfreqlockReq *nrFreqLockInfo)
{
    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("GetNrFreqLockPara: At Cmd Opt Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    nrFreqLockInfo->freqType = (VOS_UINT8)g_atParaList[0].paraValue;

    switch (nrFreqLockInfo->freqType) {
        case MTA_AT_FREQLOCK_TYPE_LOCK_NONE:
            if (g_atParaList[0].paraLen != 1) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            break;
        case MTA_AT_FREQLOCK_TYPE_LOCK_FREQ:
            if (g_atParaIndex != AT_NRFREQLOCK_PARA_NUM_FREQ) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            nrFreqLockInfo->freqType = (VOS_UINT8)g_atParaList[0].paraValue;
            nrFreqLockInfo->scsType  = (VOS_UINT8)g_atParaList[1].paraValue;
            nrFreqLockInfo->band     = (VOS_UINT16)g_atParaList[AT_NRFREQLOCK_BAND].paraValue;
            nrFreqLockInfo->nrArfcn  = (VOS_UINT32)g_atParaList[AT_NRFREQLOCK_NRARFCN].paraValue;

            break;
        case MTA_AT_FREQLOCK_TYPE_LOCK_CELL:
            if (g_atParaIndex != AT_NRFREQLOCK_PARA_NUM_CELL) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            nrFreqLockInfo->freqType  = (VOS_UINT8)g_atParaList[0].paraValue;
            nrFreqLockInfo->scsType   = (VOS_UINT8)g_atParaList[1].paraValue;
            nrFreqLockInfo->band      = (VOS_UINT16)g_atParaList[AT_NRFREQLOCK_BAND].paraValue;
            nrFreqLockInfo->nrArfcn   = (VOS_UINT32)g_atParaList[AT_NRFREQLOCK_NRARFCN].paraValue;
            nrFreqLockInfo->phyCellId = (VOS_UINT16)g_atParaList[AT_NRFREQLOCK_PHYCELL_ID].paraValue;

            break;
        case MTA_AT_FREQLOCK_TYPE_LOCK_BAND:
            if (g_atParaIndex != AT_NRFREQLOCK_PARA_NUM_BAND) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            nrFreqLockInfo->freqType = (VOS_UINT8)g_atParaList[0].paraValue;
            nrFreqLockInfo->scsType  = (VOS_UINT8)g_atParaList[1].paraValue;
            nrFreqLockInfo->band     = (VOS_UINT16)g_atParaList[AT_NRFREQLOCK_BAND].paraValue;
            break;
        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}
#endif


VOS_UINT32 AT_SetLendcPara(VOS_UINT8 indexNum)
{
    AT_MTA_UnsolicitedRptSetReq atCmd;
    VOS_UINT32                  result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atCmd.u.lendcRptFlg = (VOS_UINT8)g_atParaList[0].paraValue;
    atCmd.reqType       = AT_MTA_SET_LENDC_RPT_TYPE;

    /* 给MTA发送^LENDC设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_UNSOLICITED_RPT_SET_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_UnsolicitedRptSetReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}
#endif


VOS_UINT32 AT_ProcCclkTimeParaYMDAuxMode(VOS_UINT8 yearLen, VOS_INT32 *plYear, VOS_INT32 *plMonth, VOS_INT32 *plDay)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    /* 按照格式 "(yy/yyyy)/mm/dd,hh:mm:ss(+/-)zz"，并判断范围 */

    /* 检查(yy/yyyy) */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    if ((yearLen > 0) && (yearLen <= AT_GET_MODEM_TIME_BUFF_LEN)) {
        memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), g_atParaList[AT_CCLK_TIME].para, yearLen);
        TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), yearLen);
    }

    if (AT_AtoInt((VOS_CHAR *)buffer, plYear) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of year is err");
        return VOS_ERR;
    }

    if (yearLen == AT_MODEM_DEFALUT_AUX_MODE_YEAR_LEN) {
        if ((*plYear > AT_MODEM_DEFALUT_AUX_MODE_YEAR_MAX) || (*plYear < AT_MODEM_DEFALUT_AUX_MODE_YEAR_MIN)) {
            AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of year is out of range");
            return VOS_ERR;
        }
    } else {
        if ((*plYear > AT_MODEM_YEAR_MAX) || (*plYear < AT_MODEM_YEAR_MIN)) {
            AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of year is out of range");
            return VOS_ERR;
        }
    }

    /* 检查mm */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer),
                         &g_atParaList[AT_CCLK_TIME].para[yearLen + 1],AT_MODEM_MONTH_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_MONTH_LEN);

    if (AT_AtoInt((VOS_CHAR *)buffer, plMonth) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of month is err");
        return VOS_ERR;
    }

    if ((*plMonth > AT_MODEM_MONTH_MAX) || (*plMonth < AT_MODEM_MONTH_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of month is out of range");
        return VOS_ERR;
    }

    /* 检查dd */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer),
                         &g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1],AT_MODEM_DATE_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_DATE_LEN);

    if (AT_AtoInt((VOS_CHAR *)buffer, plDay) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of day is err");
        return VOS_ERR;
    }

    if ((*plDay > AT_MODEM_DAY_MAX) || (*plDay < AT_MODEM_DAY_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaYMDAuxMode: The parameter of day is out of range");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcCclkTimeParaHMSAuxMode(VOS_UINT8 yearLen, VOS_INT32 *plHour, VOS_INT32 *plMin, VOS_INT32 *plSec)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    /* 按照格式 "(yy/yyyy)/mm/dd,hh:mm:ss(+/-)zz"，并判断范围 */

    /* 检查hh */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_CCLK_TIME].para[yearLen + 1 +
                         AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1],AT_MODEM_HOUR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_HOUR_LEN);

    if (AT_AtoInt((VOS_CHAR *)buffer, plHour) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of hour is err");
        return VOS_ERR;
    }

    if ((*plHour > AT_MODEM_HOUR_MAX) || (*plHour < AT_MODEM_HOUR_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of hour is out of range");
        return VOS_ERR;
    }

    /* 检查mm */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_CCLK_TIME].para[yearLen + 1 +
                         AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 + AT_MODEM_HOUR_LEN + 1],AT_MODEM_MIN_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_MIN_LEN);

    if (AT_AtoInt((VOS_CHAR *)buffer, plMin) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of min is err");
        return VOS_ERR;
    }

    if ((*plMin > AT_MODEM_MIN_MAX) || (*plMin < AT_MODEM_MIN_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of min is out of range");
        return VOS_ERR;
    }

    /* 检查ss */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));
    /* 1为日期的分隔符宽度 */
    memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_CCLK_TIME].para[yearLen+ 1 +
                         AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 + AT_MODEM_HOUR_LEN + 1 + AT_MODEM_MIN_LEN +1],
                         AT_MODEM_SEC_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_SEC_LEN);

    if (AT_AtoInt((VOS_CHAR *)buffer, plSec) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of sec is err");
        return VOS_ERR;
    }

    if ((*plSec > AT_MODEM_SEC_MAX) || (*plSec < AT_MODEM_SEC_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaHMSAuxMode: The parameter of sec is out of range");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcCclkTimeParaZoneAuxMode(VOS_UINT8 yearLen, VOS_INT32 *plZone)
{
    VOS_UINT8 buffer[AT_GET_MODEM_TIME_BUFF_LEN];
    errno_t   memResult;

    /* 按照格式 "(yy/yyyy)/mm/dd,hh:mm:ss(+/-)zz"，并判断范围 */

    /* 检查(+/-)zz */
    (VOS_VOID)memset_s(buffer, (VOS_SIZE_T)sizeof(buffer), 0x00, (VOS_SIZE_T)sizeof(buffer));

    /* 1为日期的分隔符宽度 */
    if (g_atParaList[AT_CCLK_TIME].para[yearLen+ 1 +AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 +
        AT_MODEM_TIME_LEN] == '-') {
        memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_CCLK_TIME].para[yearLen + 1 +
                             AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 + AT_MODEM_TIME_LEN],AT_MODEM_ZONE_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_ZONE_LEN);
    } else {
        memResult = memcpy_s(buffer, (VOS_SIZE_T)sizeof(buffer), &g_atParaList[AT_CCLK_TIME].para[yearLen + 1 +
                             AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 + AT_MODEM_TIME_LEN + 1],
                             AT_MODEM_ZONE_LEN - 1);
        TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(buffer), AT_MODEM_ZONE_LEN - 1);
    }

    if (AT_AtoInt((VOS_CHAR *)buffer, plZone) == VOS_ERR) {
        AT_ERR_LOG("AT_ProcCclkTimeParaZoneAuxMode: The parameter of zone is err");
        return VOS_ERR;
    }

    if ((*plZone > AT_MODEM_TIME_ZONE_MAX) || (*plZone < AT_MODEM_TIME_ZONE_MIN)) {
        AT_ERR_LOG("AT_ProcCclkTimeParaZoneAuxMode: The parameter of zone is out of range");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_ProcCclkTimeParaAuxMode(VOS_UINT8 indexNum, VOS_UINT8 yearLen)
{
    VOS_INT32       sec;
    VOS_INT32       min;
    VOS_INT32       hour;
    VOS_INT32       day;
    VOS_INT32       month;
    VOS_INT32       year;
    VOS_INT32       zone;
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;

    sec   = 0;
    min   = 0;
    hour  = 0;
    day   = 0;
    month = 0;
    year  = 0;
    zone  = 0;

    /* 按照格式 "(yy/yyyy)/mm/dd,hh:mm:ss(+/-)zz"，并判断范围 */

    /* 检查(yy/yyyy)/mm/dd */
    if (AT_ProcCclkTimeParaYMDAuxMode(yearLen, &year, &month, &day) == VOS_ERR) {
        return VOS_ERR;
    }

    /* 检查hh:mm:ss */
    if (AT_ProcCclkTimeParaHMSAuxMode(yearLen, &hour, &min, &sec) == VOS_ERR) {
        return VOS_ERR;
    }

    /* 检查(+/-)zz */
    if (AT_ProcCclkTimeParaZoneAuxMode(yearLen, &zone) == VOS_ERR) {
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (yearLen == AT_MODEM_DEFALUT_AUX_MODE_YEAR_LEN) {
        netCtx->timeInfo.universalTimeandLocalTimeZone.year = (VOS_UINT8)year;
    } else {
        /* '%100'是为了将yyyy的年份表示方式转为yy的方式 */
        netCtx->timeInfo.universalTimeandLocalTimeZone.year = (VOS_UINT8)(year % 100);
    }

    netCtx->timeInfo.universalTimeandLocalTimeZone.month    = (VOS_UINT8)month;
    netCtx->timeInfo.universalTimeandLocalTimeZone.day      = (VOS_UINT8)day;
    netCtx->timeInfo.universalTimeandLocalTimeZone.hour     = (VOS_UINT8)hour;
    netCtx->timeInfo.universalTimeandLocalTimeZone.minute   = (VOS_UINT8)min;
    netCtx->timeInfo.universalTimeandLocalTimeZone.second   = (VOS_UINT8)sec;
    netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone = (VOS_INT8)zone;
    netCtx->timeInfo.localTimeZone                          = (VOS_INT8)zone;

    netCtx->timeInfo.ieFlg = netCtx->timeInfo.ieFlg | NAS_MM_INFO_IE_UTLTZ;

    return VOS_OK;
}


VOS_UINT32 AT_SetCclkPara(VOS_UINT8 indexNum)
{
    AT_ModemNetCtx *netCtx = VOS_NULL_PTR;
    VOS_UINT8       yearLen;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数目检查 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    if (netCtx->csdfCfg.auxMode == 1) {
        /* "yy/mm/dd,hh:mm:ss(+/-)zz" */
        yearLen = AT_MODEM_DEFALUT_AUX_MODE_YEAR_LEN;
    } else {
        /* "yyyy/mm/dd,hh:mm:ss(+/-)zz" */
        yearLen = AT_MODEM_OTHER_AUX_MODE_YEAR_LEN;
    }

    /* 参数长度不正确 */
    if ((AT_MODEM_AUX_MODE_EXCEPT_YEAR_TIME_LEN + yearLen) != g_atParaList[AT_CCLK_TIME].paraLen) {
        AT_ERR_LOG("AT_SetCclkPara: length of parameter is error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 按照格式 "(yy/yyyy)/mm/dd,hh:mm:ss(+/-)zz"，并判断格式 */
    /* 1为日期的分隔符宽度 */
    if ((g_atParaList[AT_CCLK_TIME].para[yearLen] != '/') ||
        (g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN] != '/') ||
        (g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN] != ',') ||
        (g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 +
        AT_MODEM_HOUR_LEN] != ':') ||
        (g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 +
        AT_MODEM_HOUR_LEN + 1 +AT_MODEM_MIN_LEN] != ':') ||
        ((g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 +
        AT_MODEM_TIME_LEN] != '+') &&
        (g_atParaList[AT_CCLK_TIME].para[yearLen + 1 + AT_MODEM_MONTH_LEN + 1 + AT_MODEM_DATE_LEN + 1 +
        AT_MODEM_TIME_LEN] != '-'))) {
        AT_ERR_LOG("AT_SetCclkPara: The date formats parameter is error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_ProcCclkTimeParaAuxMode(indexNum, yearLen) == VOS_ERR) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}

#if (FEATURE_DSDS == FEATURE_ON)

VOS_UINT32 AT_SetDsdsStatePara(VOS_UINT8 indexNum)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_SetDsdsStatePara: Get modem id fail.");
        return AT_ERROR;
    }

    if (modemId != MODEM_ID_0) {
        return AT_ERROR;
    }

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (TAF_MMA_SetDsdsStateReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atParaList[0].paraValue) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DSDS_STATE_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaDsdsStateSetCnf(struct MsgCB *msg)
{
    TAF_MMA_DsdsStateSetCnf *dsdsStateSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum        = 0;
    VOS_UINT32               result;

    dsdsStateSetCnf = (TAF_MMA_DsdsStateSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(dsdsStateSetCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDsdsStateSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaDsdsStateSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_DSDS_STATE_SET) {
        AT_WARN_LOG("AT_RcvMmaDsdsStateSetCnf : Current Option is not AT_CMD_CIND_SET.");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);

    if (dsdsStateSetCnf->rslt == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaDsdsStateNotify(struct MsgCB *msg)
{
    TAF_MMA_DsdsStateNotify *dsdsStateNotify = VOS_NULL_PTR;
    ModemIdUint16            modemId;
    VOS_UINT8                indexNum;
    VOS_UINT16               length;

    /* 初始化消息变量 */
    indexNum        = 0;
    length          = 0;
    dsdsStateNotify = (TAF_MMA_DsdsStateNotify *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(dsdsStateNotify->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaDsdsStateNotify: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvMmaDsdsStateNotify: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_DSDSSTATE].text,
        dsdsStateNotify->supportDsds3, g_atCrLf);

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_SetGameModePara(VOS_UINT8 indexNum)
{
    ModemIdUint16             modemId;
    AT_MTA_CommGameModeSetReq atCmd;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_SetGameModePara: Get modem id fail.");
        return AT_ERROR;
    }

    if (modemId != MODEM_ID_0) {
        AT_ERR_LOG("enModemId isn't MODEM 0");
        return AT_ERROR;
    }

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));
    atCmd.gameMode = (VOS_UINT8)g_atParaList[0].paraValue;

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_GAME_MODE_SET_REQ, (VOS_UINT8 *)&atCmd,
                               sizeof(AT_MTA_CommGameModeSetReq), I0_UEPS_PID_MTA) == TAF_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GAME_MODE_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetSmsDomainPara(VOS_UINT8 indexNum)
{
    AT_MTA_SmsDomainSetReq smsDomain;
    VOS_UINT32             rst;

    (VOS_VOID)memset_s(&smsDomain, sizeof(smsDomain), 0x00, sizeof(smsDomain));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数及长度检查 */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    smsDomain.smsDomain = (AT_MTA_SmsDomainUint8)g_atParaList[0].paraValue;

    /* 发送跨核消息到C核 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SMS_DOMAIN_SET_REQ, (VOS_UINT8 *)&smsDomain, sizeof(smsDomain),
                                 I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetSmsDomainPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SMSDOMAIN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSmsDomainSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg = VOS_NULL_PTR;
    MTA_AT_SmsDomainSetCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_SmsDomainSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SMSDOMAIN_SET) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainSetCnf : Current Option is not AT_CMD_SMSDOMAIN_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QrySmsDomainPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* AT 给MTA 发送查询请求消息 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_SMS_DOMAIN_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_SetSmsDomainPara: AT_QrySmsDomainPara fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SMSDOMAIN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaSmsDomainQryCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *mtaMsg = VOS_NULL_PTR;
    MTA_AT_SmsDomainQryCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum = 0;

    /* 初始化消息变量 */
    mtaMsg = (AT_MTA_Msg *)msg;
    qryCnf = (MTA_AT_SmsDomainQryCnf *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_SMSDOMAIN_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SMSDOMAIN_QRY) {
        AT_WARN_LOG("AT_RcvMtaSmsDomainQryCnf: WARNING:Not AT_CMD_SMSDOMAIN_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 判断查询操作是否成功 */
    if (qryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                qryCnf->smsDomain);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    /* 调用AT_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_ConvertWs46RatOrderPara(VOS_UINT8 clientId, TAF_MMA_SysCfgPara *sysCfgSetPara)
{
    TAF_MMA_MultimodeRatCfg *ratOrder = VOS_NULL_PTR;
    ModemIdUint16            modemId;
    VOS_UINT32               rst;

    modemId  = MODEM_ID_0;
    ratOrder = &(sysCfgSetPara->multiModeRatCfg);

    rst = AT_GetModemIdFromClient(clientId, &modemId);
    if (rst != VOS_OK) {
        AT_ERR_LOG1("AT_ConvertWs46RatOrderPara:Get ModemID From ClientID fail,ClientID:", clientId);
        return AT_ERROR;
    }

    AT_SetDefaultRatPrioList(modemId, ratOrder, VOS_TRUE, VOS_TRUE);

    sysCfgSetPara->userPrio = AT_GetSysCfgPrioRat(sysCfgSetPara);

    return AT_OK;
}


VOS_UINT32 AT_SetWs46Para(VOS_UINT8 indexNum)
{
    TAF_MMA_SysCfgPara sysCfgSetPara;
    VOS_UINT32         rst;
    AT_ModemNetCtx    *netCtx = VOS_NULL_PTR;

    (VOS_VOID)memset_s(&sysCfgSetPara, sizeof(sysCfgSetPara), 0x00, sizeof(sysCfgSetPara));

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    rst = AT_OK;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = AT_ConvertWs46RatOrderPara(indexNum, &sysCfgSetPara);

    if (rst != AT_OK) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* stSysCfgSetPara其他参数赋值 */
    if (netCtx->roamFeature == AT_ROAM_FEATURE_ON) {
        sysCfgSetPara.roam = TAF_MMA_ROAM_UNCHANGE;
    } else {
        sysCfgSetPara.roam = AT_ROAM_FEATURE_OFF_NOCHANGE;
    }

    sysCfgSetPara.srvDomain       = TAF_MMA_SERVICE_DOMAIN_NOCHANGE;
    sysCfgSetPara.guBand.bandLow  = TAF_PH_BAND_NO_CHANGE;
    sysCfgSetPara.guBand.bandHigh = 0;

    (VOS_VOID)memset_s(&(sysCfgSetPara.lBand), sizeof(sysCfgSetPara.lBand), 0x00, sizeof(sysCfgSetPara.lBand));
    sysCfgSetPara.lBand.bandInfo[0] = TAF_PH_BAND_NO_CHANGE;

    /* 执行命令操作 */
    if (TAF_MMA_SetSysCfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &sysCfgSetPara) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_WS46_SET;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryWs46Para(VOS_UINT8 indexNum)
{
    if (TAF_MMA_QrySyscfgReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_WS46_QRY; /* 设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                             /* 等待异步事件返回 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TestWs46Para(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "(12,22,25,28,29,30,31)");

    return AT_OK;
}


VOS_UINT32 AT_SetGpsLocSetPara(VOS_UINT8 indexNum)
{
    AT_MTA_GpslocsetSetReq atCmd;
    VOS_UINT32             result;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不等于1个，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetGpsLocSetPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数长度为0，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetGpsLocSetPara: para0 Length = 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atCmd.locationPermitFlag = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_GPS_LOCSET_SET_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_GpslocsetSetReq), I0_UEPS_PID_MTA);

    if (result != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetGpsLocSetPara: AT_FillAndSndAppReqMsg Failed!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GPSLOCSET_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaGpsLocSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg = VOS_NULL_PTR;
    MTA_AT_GpslocsetSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_GpslocsetSetCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaGpsLocSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaGpsLocSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_GPSLOCSET_SET) {
        AT_WARN_LOG("AT_RcvMtaGpsLocSetCnf : Current Option is not AT_CMD_GPSLOCSET_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    result = AT_ConvertMtaResult(setCnf->result);

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_VOID AT_PrintCclkTime(VOS_UINT8 indexNum, TIME_ZONE_Time *timeZoneTime, ModemIdUint16 modemId)
{
    TIME_ZONE_Time *timeZone = VOS_NULL_PTR;
    AT_ModemNetCtx *netCtx   = VOS_NULL_PTR;
    VOS_UINT32      year;

    timeZone = timeZoneTime;

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if (netCtx->csdfCfg.auxMode == 1) {
        /* "yy/mm/dd,hh:mm:ss(+/-)zz" */
        year = timeZone->year;
    } else {
        /* "yyyy/mm/dd,hh:mm:ss(+/-)zz" */
        if (timeZone->year > AT_GMT_TIME_DEFAULT) {
            year = 1900 + (VOS_UINT32)timeZone->year;
        } else {
            year = 2000 + (VOS_UINT32)timeZone->year;
        }
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: \"%02d/%02d/%02d,%02d:%02d:%02d",
            g_parseContext[indexNum].cmdElement->cmdName, year, timeZone->month, timeZone->day, timeZone->hour,
            timeZone->minute, timeZone->second);

    if (timeZone->timeZone >= 0) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "+%02d\"", timeZone->timeZone);

    } else {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "-%02d\"", -(timeZone->timeZone));
    }

    return;
}


VOS_UINT32 AT_RcvMtaCclkQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg        *rcvMsg = VOS_NULL_PTR;
    MTA_AT_CclkQryCnf *qryCnf = VOS_NULL_PTR;
    AT_ModemNetCtx    *netCtx = VOS_NULL_PTR;
    VOS_UINT32         result;
    VOS_UINT8          indexNum;
    ModemIdUint16      modemId;
    TIME_ZONE_Time     time;

    /* 初始化 */
    (VOS_VOID)memset_s(&time, sizeof(time), 0x00, sizeof(time));
    rcvMsg   = (AT_MTA_Msg *)msg;
    qryCnf   = (MTA_AT_CclkQryCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    result = AT_GetModemIdFromClient(indexNum, &modemId);
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_RcvMtaCclkQryCnf: Get modem id fail.");
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaCclkQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaCclkQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCLK_QRY) {
        AT_WARN_LOG("AT_RcvMtaCclkQryCnf : Current Option is not AT_CMD_CCLK_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    /* 判断查询操作是否成功 */
    if (qryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
        time   = qryCnf->time;

        netCtx->timeInfo.universalTimeandLocalTimeZone.year     = qryCnf->time.year;
        netCtx->timeInfo.universalTimeandLocalTimeZone.month    = qryCnf->time.month;
        netCtx->timeInfo.universalTimeandLocalTimeZone.day      = qryCnf->time.day;
        netCtx->timeInfo.universalTimeandLocalTimeZone.hour     = qryCnf->time.hour;
        netCtx->timeInfo.universalTimeandLocalTimeZone.minute   = qryCnf->time.minute;
        netCtx->timeInfo.universalTimeandLocalTimeZone.second   = qryCnf->time.second;
        netCtx->timeInfo.universalTimeandLocalTimeZone.timeZone = qryCnf->time.timeZone;
        netCtx->timeInfo.ieFlg |= NAS_MM_INFO_IE_UTLTZ;

        AT_PrintCclkTime(indexNum, &time, modemId);
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}



VOS_UINT32 AT_RcvMtaTempProtectInd(struct MsgCB *msg)
{
    AT_MTA_Msg            *mtaMsg   = VOS_NULL_PTR;
    MTA_AT_TempProtectInd *tempInd  = VOS_NULL_PTR;
    VOS_UINT8              indexNum = 0;

    mtaMsg  = (AT_MTA_Msg *)msg;
    tempInd = (MTA_AT_TempProtectInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaTempProtectInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d%s", g_atCrLf, g_atStringTab[AT_STRING_TEMPPROTECT].text,
            tempInd->tempResult, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_SetNvRefreshPara(VOS_UINT8 indexNum)
{
    AT_MTA_NvRefreshSetReq atCmd;
    VOS_UINT32             result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

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

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    atCmd.reason = AT_MTA_NV_REFRESH_USIM_DEPENDENT;

    /* 给MTA发送NV_REFRESH设置请求 */
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_NV_REFRESH_SET_REQ,
                                    (VOS_UINT8 *)&atCmd, sizeof(AT_MTA_NvRefreshSetReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NVREFRESH_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_ERR_LOG("AT_SetNvRefreshPara:  AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaNvRefreshSetCnf(struct MsgCB *msg)
{
    /* 定义局部变量 */
    AT_MTA_Msg             *mtaMsg = VOS_NULL_PTR;
    MTA_AT_NvRefreshSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT8               indexNum;

    /* 初始化消息变量 */
    mtaMsg   = (AT_MTA_Msg *)msg;
    setCnf   = (MTA_AT_NvRefreshSetCnf *)mtaMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNvRefreshSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNvRefreshSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_NVREFRESH_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NVREFRESH_SET) {
        AT_WARN_LOG("AT_RcvMtaNvRefreshSetCnf: WARNING:Not AT_CMD_NVREFRESH_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    /* 判断设置操作是否成功 */
    if (setCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    /* 调用At_FormatResultData发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetPseudBtsPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetPseudbtsReq atCmd;
    VOS_UINT32            ret;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不等于2个，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != AT_PSEUDBTS_PARA_NUM) {
        AT_WARN_LOG("AT_SetPseudBtsPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第1个参数长度为0，返回AT_CME_INCORR ECT_PARAMETERS */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_WARN_LOG("AT_SetPseudBtsPara: para Length = 0");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atCmd.pseudRat        = (VOS_UINT8)g_atParaList[0].paraValue;
    atCmd.pseudBtsQryType = (VOS_UINT8)g_atParaList[1].paraValue;

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_PSEUDBTS_SET_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_SetPseudbtsReq), I0_UEPS_PID_MTA);

    if (ret != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetPseudBtsPara: AT_FillAndSndAppReqMsg Failed!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PSEUDBTS_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_CheckSubClfsParamPara(VOS_VOID)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数大于9个、小于6个，返回AT_CME_INCORRECT_PARAMETERS */
    if ((g_atParaIndex < AT_SUBCLFSPARAM_MIN_PARA_NUM) || (g_atParaIndex > AT_SUBCLFSPARAM_MAX_PARA_NUM)) {
        AT_WARN_LOG("AT_CheckSubClfsParamPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_SUBCLFSPARAM_VERSIONID].paraLen > AT_SUBCLFSPARAM_PARA_VERSIONID_MAX_LEN) {
        AT_WARN_LOG("AT_CheckSubClfsParamPara: At Para1 Length Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果为功能关闭命令，则参数最多为6个 */
    if ((g_atParaList[AT_SUBCLFSPARAM_ACTIVEFLG].paraValue == 0) &&
        (g_atParaIndex > AT_SUBCLFSPARAM_MAX_PARA_NUM_FUNC_OFF)) {
        AT_WARN_LOG("AT_CheckSubClfsParamPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetSubClfsParamPara(VOS_UINT8 indexNum)
{
    AT_MTA_SetSubclfsparamReq *msg = VOS_NULL_PTR;
    VOS_UINT_PTR               clfsDataAddr;
    VOS_UINT32                 bufLen;
    VOS_UINT32                 bsLen;
    VOS_UINT32                 loop;
    VOS_UINT32                 deltaLen;
    VOS_UINT32                 ret;
    errno_t                    memResult;

    /* 参数检查 */
    ret = AT_CheckSubClfsParamPara();
    if (ret != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetSubClfsParamPara: AT_CheckSubClfsParamPara Failed");
        return ret;
    }

    bsLen  = (VOS_UINT32)g_atParaList[AT_MTA_PARA_INFO_CLFS_GROUP_NUM].paraLen +
             (VOS_UINT32)g_atParaList[AT_MTA_PARA_INFO_DATA_LENGTH].paraLen +
             (VOS_UINT32)g_atParaList[AT_MTA_PARA_INFO_CLFS_DATA].paraLen;
    bufLen = sizeof(AT_MTA_SetSubclfsparamReq);

    if (bsLen > AT_MTA_PARA_INFO_CLFS_DATA_LEN) {
        bufLen += bsLen - AT_MTA_PARA_INFO_CLFS_DATA_LEN;
    }

    /*lint -save -e830*/
    msg = (AT_MTA_SetSubclfsparamReq *)PS_MEM_ALLOC(WUEPS_PID_AT, bufLen);
    /*lint -restore */

    /* 内存申请失败，返回AT_ERROR */
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SetSubClfsParamPara:memory alloc fail.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s((VOS_INT8 *)msg, bufLen, 0x00, bufLen);

    /* 填写消息内容 */
    msg->seq                   = (VOS_UINT8)g_atParaList[AT_SUBCLFSPARAM_SEQ].paraValue;
    msg->type                  = (VOS_UINT8)g_atParaList[AT_SUBCLFSPARAM_TYPE].paraValue;
    msg->activeFlg             = (VOS_UINT8)g_atParaList[AT_SUBCLFSPARAM_ACTIVEFLG].paraValue;
    msg->probaRate             = g_atParaList[AT_SUBCLFSPARAM_PROBARATE].paraValue;
    msg->paraInfo.clfsGroupNum = (VOS_UINT16)g_atParaList[AT_SUBCLFSPARAM_CLFSGROUPNUM].paraValue;
    msg->paraInfo.dataLength   = (VOS_UINT16)bsLen;

    if (g_atParaList[AT_SUBCLFSPARAM_VERSIONID].paraLen > 0) {
        memResult = memcpy_s(msg->versionId, sizeof(msg->versionId), g_atParaList[AT_SUBCLFSPARAM_VERSIONID].para,
                             g_atParaList[AT_SUBCLFSPARAM_VERSIONID].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(msg->versionId), g_atParaList[AT_SUBCLFSPARAM_VERSIONID].paraLen);
    }

    deltaLen = 0;
    clfsDataAddr = (VOS_UINT_PTR)msg->paraInfo.clfsData;
    for (loop = AT_SUBCLFSPARAM_MIN_PARA_NUM; loop < AT_SUBCLFSPARAM_MAX_PARA_NUM; loop++) {
        if (g_atParaList[loop].paraLen > 0) {
            memResult = memcpy_s((VOS_UINT8 *)(clfsDataAddr + deltaLen), (bsLen - deltaLen), g_atParaList[loop].para,
                                 g_atParaList[loop].paraLen);
            TAF_MEM_CHK_RTN_VAL(memResult, (bsLen - deltaLen), g_atParaList[loop].paraLen);

            deltaLen = deltaLen + g_atParaList[loop].paraLen;
        } else {
            break;
        }
    }

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SUBCLFSPARAM_SET_REQ, (VOS_UINT8 *)msg, bufLen, I0_UEPS_PID_MTA);

    /*lint -save -e516*/
    PS_MEM_FREE(WUEPS_PID_AT, msg);
    /*lint -restore */

    if (ret != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetPseudBtsPara: AT_FillAndSndAppReqMsg Failed!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SUBCLFSPARAM_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QrySubClfsParamPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询云通信版本请求 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 ID_AT_MTA_SUBCLFSPARAM_QRY_REQ, VOS_NULL_PTR, 0, I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QrySubClfsParamPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SUBCLFSPARAM_QRY;

    return AT_WAIT_ASYNC_RETURN;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_SetClDbDomainStatusPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 mode;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不等于1，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetClDbDomainStatusPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写消息内容 */
    mode = (VOS_UINT8)g_atParaList[0].paraValue;

    if (TAF_MMA_SetClDbDomainStatus(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, mode) != VOS_TRUE) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CLDBDOMAINSTATUS_SET;

    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 AT_SetUlFreqRptPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 mode;

    /* 指令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetUlFreqPara : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数或长度不正确 */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetUlFreqPara : The number of input parameters is error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写消息 */
    mode = (VOS_UINT8)g_atParaList[0].paraValue;

    /* 发送消息给C核处理 */
    if (TAF_MMA_SetUlFreqRptReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, mode) ==
        VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ULFREQRPT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryUlFreqPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rslt;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* 给MMA发送消息查询服务小区上行频率 */
    rslt = TAF_MMA_QryUlFreqRptReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (rslt == VOS_TRUE) {
        /* 返回命令处理挂起状态 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ULFREQRPT_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaUlFreqRptSetCnf(struct MsgCB *msg)
{
    TAF_MMA_UlfreqrptSetCnf *ulfreqSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum;
    VOS_UINT32               result;

    /* 初始化 */
    indexNum     = 0;
    result       = AT_ERROR;
    ulfreqSetCnf = (TAF_MMA_UlfreqrptSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(ulfreqSetCnf->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ULFREQRPT_SET) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptSetCnf : Current Option is not AT_CMD_EFPSLOCIINFO_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 格式化命令返回 */
    g_atSendDataBuff.bufLen = 0;

    if (ulfreqSetCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaUlFreqChangeInd(struct MsgCB *msg)
{
    VOS_UINT8          indexNum;
    TAF_MMA_UlFreqInd *ulFreqInfoInd = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum      = 0;
    ulFreqInfoInd = (TAF_MMA_UlFreqInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(ulFreqInfoInd->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaUlFreqChangeInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_ULFREQRPT].text,
        ulFreqInfoInd->rat, ulFreqInfoInd->freq, ulFreqInfoInd->bandWidth, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaUlFreqRptQryCnf(struct MsgCB *msg)
{
    TAF_MMA_UlfreqrptQryCnf *ulFreqInfoInd = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum      = 0;
    ulFreqInfoInd = (TAF_MMA_UlfreqrptQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(ulFreqInfoInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ULFREQRPT_QRY) {
        AT_WARN_LOG("AT_RcvMmaUlFreqRptQryCnf : Current Option is not AT_CMD_EFPSLOCIINFO_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (ulFreqInfoInd->errorCause == TAF_ERR_NO_ERROR) {
        result                  = AT_OK;
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "^ULFREQRPT: %d,%d,%d,%d", ulFreqInfoInd->mode, ulFreqInfoInd->rat,
            ulFreqInfoInd->ulFreq, ulFreqInfoInd->bandwidth);
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaPseudBtsIdentInd(struct MsgCB *msg)
{
    AT_MTA_Msg              *mtaMsg           = VOS_NULL_PTR;
    MTA_AT_PseudBtsIdentInd *pseudBtsIdentInd = VOS_NULL_PTR;
    VOS_UINT8                indexNum         = 0;

    mtaMsg           = (AT_MTA_Msg *)msg;
    pseudBtsIdentInd = (MTA_AT_PseudBtsIdentInd *)mtaMsg->content;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(mtaMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaPseudBtsIdentInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d%s", g_atCrLf, g_atStringTab[AT_STRING_PSEUDBTS].text,
        pseudBtsIdentInd->pseudBtsType, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 At_SetPsScenePara(TAF_UINT8 indexNum)
{
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    TAF_MMA_PsScenePara psSceneStru;
    VOS_UINT32          rslt;
    ModemIdUint16       modemId;

    modemId = MODEM_ID_BUTT;

    /* 通过clientID获得ModemID */
    rslt = AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId);

    /* 如果ModemID获取失败或不在Modem1，返回失败 */
    if ((rslt != VOS_OK) || (modemId != MODEM_ID_1)) {
        AT_WARN_LOG1("At_SetPsScenePara: AT_GetModemIdFromClient failed or not modem1!modem id is,", modemId);
        return AT_ERROR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (At_CheckCurrRatModeIsCL((VOS_UINT8)(g_atClientTab[indexNum].clientId)) == VOS_TRUE) {
        AT_WARN_LOG("At_SetPsScenePara: operation not allowed in CL mode!");
        return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
    }
#endif

    /* 参数检查 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_PSSCENE_MAX_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* PS业务开始时，ps_detach标记必须为false */
    if ((g_atParaList[0].paraValue == 1) && (g_atParaList[1].paraValue != 0)) {
        AT_WARN_LOG2("At_SetPsScenePara: paravalue is wrong,para0,para1,", g_atParaList[0].paraValue,
                     g_atParaList[1].paraValue);
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&psSceneStru, sizeof(psSceneStru), 0x00, sizeof(psSceneStru));
    psSceneStru.psServiceState = (VOS_UINT8)g_atParaList[0].paraValue;
    psSceneStru.psDetachFlag   = (VOS_UINT8)g_atParaList[1].paraValue;

    if (TAF_MMA_SetPsSceneReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &psSceneStru) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PSSCENE_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}


VOS_UINT32 AT_QryPsScenePara(TAF_UINT8 indexNum)
{
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    VOS_UINT32    rslt;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_BUTT;

    /* 通过clientID获得ModemID */
    rslt = AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId);

    /* 如果ModemID获取失败或不在Modem1，返回失败 */
    if ((rslt != VOS_OK) || (modemId != MODEM_ID_1)) {
        AT_WARN_LOG("AT_QryPsScenePara: AT_GetModemIdFromClient failed or not modem1!");
        return AT_ERROR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (At_CheckCurrRatModeIsCL((VOS_UINT8)(g_atClientTab[indexNum].clientId)) == VOS_TRUE) {
        AT_WARN_LOG("AT_QryPsScenePara: operation not allowed in CL mode!");
        return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
    }
#endif

    if (TAF_MMA_QryPsSceneReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PSSCENE_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
#else
    return AT_ERROR;
#endif
}

VOS_UINT32 AT_SetSmsAntiAttackCapQrypara(TAF_UINT8 indexNum)
{
    VOS_UINT8 antiSmsAttacktype;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

    /* 参数个数不为1或者字符串长度不为1 */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen != 1)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (At_CheckCurrRatModeIsCL((VOS_UINT8)(g_atClientTab[indexNum].clientId)) == VOS_TRUE) {
        AT_WARN_LOG("AT_SetSmsAntiAttackCapQrypara: operation not allowed in CL mode.");
        return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
    }
#endif

    antiSmsAttacktype = (VOS_UINT8)g_atParaList[0].paraValue;

    if (TAF_MMA_QrySmsAntiAttackCapReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                       antiSmsAttacktype) != VOS_TRUE) {
        AT_WARN_LOG("AT_SetSmsAntiAttackCapQrypara: send msg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SMSANTIATTACKCAP_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvSmsAntiAttackCapQryCnf(struct MsgCB *rcvMsg)
{
    TAF_MMA_SmsAntiAttackCapQryCnf *qryCnf = VOS_NULL_PTR;
    VOS_UINT8                       indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    qryCnf   = (TAF_MMA_SmsAntiAttackCapQryCnf *)rcvMsg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(qryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvSmsAntiAttackCapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSmsAntiAttackCapQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SMSANTIATTACKCAP_QRY) {
        AT_WARN_LOG("AT_RcvSmsAntiAttackCapQryCnf : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, qryCnf->result,
        qryCnf->cause);

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_SetSmsAntiAttackState(VOS_UINT8 indexNum)
{
    VOS_UINT32                      result;
    TAF_MMA_SmsAntiAttackStateUint8 state;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_ERROR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (At_CheckCurrRatModeIsCL((VOS_UINT8)(g_atClientTab[indexNum].clientId)) == VOS_TRUE) {
        AT_WARN_LOG("AT_SetSmsAntiAttackState: operation not allowed in CL mode!");
        return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
    }
#endif

    /* 参数个数不为1或者字符串长度不为1 */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen != 1)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为0或者1，当前只支持这两个值 */
    if (g_atParaList[0].paraValue > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    state = (TAF_MMA_SmsAntiAttackStateUint8)g_atParaList[0].paraValue;

    /* 发送跨核消息到C核, 设置防攻击状态 */

    result = TAF_MMA_SetSmsAntiAttackState(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                           state);

    if (result != VOS_TRUE) {
        AT_WARN_LOG("AT_SetSmsAntiAttackState: AT_SetSmsAntiAttackState fail.");
        return AT_ERROR;
    }

    /* 发送成功，设置当前操作模式 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SMSANTIATTACK_SET;

    /* 等待异步处理时间返回 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMmaSmsAntiAttackSetCnf(struct MsgCB *rcvMsg)
{
    TAF_MMA_SmsAntiAttackStateSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32                        result;
    VOS_UINT8                         indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    setCnf   = (TAF_MMA_SmsAntiAttackStateSetCnf *)rcvMsg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(setCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSmsAntiAttackSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSmsAntiAttackSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SMSANTIATTACK_SET) {
        AT_WARN_LOG("AT_RcvMmaSmsAntiAttackSetCnf : Current Option is not AT_CMD_SMSANTIATTACK_SET.");
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, setCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_QrySmsNasCapPara(TAF_UINT8 indexNum)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (At_CheckCurrRatModeIsCL((VOS_UINT8)(g_atClientTab[indexNum].clientId)) == VOS_TRUE) {
        AT_WARN_LOG("AT_QrySmsNasCapPara: operation not allowed in CL mode.");
        return AT_CME_OPERATION_NOT_ALLOWED_IN_CL_MODE;
    }
#endif

    if (TAF_MMA_QrySmsNasCapReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId) !=
        VOS_TRUE) {
        AT_WARN_LOG("AT_QrySmsNasCapPara: send msg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SMSNASCAP_QRY;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMmaSmsNasCapQryCnf(struct MsgCB *rcvMsg)
{
    TAF_MMA_SmsNasCapQryCnf *smsNasCapQryCnf = VOS_NULL_PTR;
    VOS_UINT32               result;
    VOS_UINT8                indexNum;

    /* 初始化消息变量 */
    indexNum        = 0;
    result          = 0;
    smsNasCapQryCnf = (TAF_MMA_SmsNasCapQryCnf *)rcvMsg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(smsNasCapQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaSmsNasCapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaSmsNasCapQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SMSNASCAP_QRY) {
        AT_WARN_LOG("AT_RcvMmaSmsNasCapQryCnf : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (smsNasCapQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                smsNasCapQryCnf->smsNasCap);
        result = AT_OK;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPsSceneSetCnf(struct MsgCB *msg)
{
    TAF_MMA_PsSceneSetCnf *setCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum = 0;
    setCnf   = (TAF_MMA_PsSceneSetCnf *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(setCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPsSceneSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPsSceneSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PSSCENE_SET) {
        AT_WARN_LOG("AT_RcvMmaPsSceneSetCnf : Current Option is not AT_CMD_PSSCENE_SET.");
        return VOS_ERR;
    }
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (setCnf->result == TAF_ERR_NO_ERROR) {
        result = AT_OK;
    } else {
        result = At_ChgTafErrorCode(indexNum, setCnf->result);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPsSceneQryCnf(struct MsgCB *msg)
{
    TAF_MMA_PsSceneQryCnf *psSceneQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化消息变量 */
    indexNum      = 0;
    result        = 0;
    psSceneQryCnf = (TAF_MMA_PsSceneQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(psSceneQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPsSceneQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaPsSceneQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PSSCENE_QRY) {
        AT_WARN_LOG("AT_RcvMmaPsSceneQryCnf : Current Option is not AT_CMD_CEMODE_READ.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (psSceneQryCnf->result == TAF_ERR_NO_ERROR) {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                psSceneQryCnf->psSceneState);
        result = AT_OK;
    } else {
        result                  = AT_ERROR;
        g_atSendDataBuff.bufLen = 0;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_SetForceSyncPara(VOS_UINT8 indexNum)
{
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    AT_DEVICE_CmdCtrl *atDevCmdCtrl = VOS_NULL_PTR;
#else
    AT_MT_Info *atMtInfoCtx = VOS_NULL_PTR;
#endif
    AT_MTA_SetForcesyncReq atCmd;
    VOS_UINT32             ret;

    (VOS_VOID)memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(atCmd));

    /* 该命令需在非信令模式下使用 */
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
    atDevCmdCtrl = AT_GetDevCmdCtrl();
    if (atDevCmdCtrl->currentTMode != AT_TMODE_FTM)
#else
    atMtInfoCtx             = AT_GetMtInfoCtx();
    if (atMtInfoCtx->currentTMode != AT_TMODE_FTM)
#endif
    {
        return AT_ERROR;
    }

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不等于1个，返回AT_CME_INCORRECT_PARAMETERS */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        AT_WARN_LOG("AT_SetForceSyncPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    atCmd.num = (VOS_UINT16)g_atParaList[0].paraValue;

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_FORCESYNC_SET_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_SetForcesyncReq), I0_UEPS_PID_MTA);

    if (ret != AT_SUCCESS) {
        AT_WARN_LOG("AT_SetForceSyncPara: AT_FillAndSndAppReqMsg Failed!");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FORCESYNC_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaForceSyncSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg          = VOS_NULL_PTR;
    MTA_AT_SetForcesyncCnf *setForceSyncCnf = VOS_NULL_PTR;
    VOS_UINT8               indexNum;
    VOS_UINT32              result;

    indexNum        = 0;
    result          = 0;
    rcvMsg          = (AT_MTA_Msg *)msg;
    setForceSyncCnf = (MTA_AT_SetForcesyncCnf *)(rcvMsg->content);

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaForceSyncSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 判断是否为广播 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaForceSyncSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_FORCESYNC_SET) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (setForceSyncCnf->result == MTA_AT_RESULT_NO_ERROR) {
        result = AT_OK;
    } else {
        result = AT_ERROR;
    }
    g_atSendDataBuff.bufLen = 0;

    /* 发送命令结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 At_SetGnssNtyPara(VOS_UINT8 indexNum)
{
    AT_MTA_Gnssnty atMtaGnssnty;
    VOS_UINT32     rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数长度检查 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /* Hi110X芯片在GNSS场景下给MODEM发AT^GNSSNTY=1,退出GNSSNTY场景后发AT^GNSSNTY=0 */
    atMtaGnssnty.value = g_atParaList[0].paraValue;

    /* 发送消息 ID_AT_MTA_GNSS_NTY 给MTA处理，该消息带参数 stAtMtaGnssnty */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_GNSS_NTY, (VOS_UINT8 *)&atMtaGnssnty,
                                 (VOS_SIZE_T)sizeof(atMtaGnssnty), I0_UEPS_PID_MTA);

    if (rst != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetGnssNtyPara: send ReqMsg fail");
        return AT_ERROR;
    }

    return AT_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

LOCAL VOS_UINT32 AT_CheckNrrcCapCfgPara(VOS_VOID)
{
    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_CheckNrrcCapCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 不携带参数，返回AT_CME_INCORR ECT_PARAMETERS */
    if (g_atParaList[AT_NRRCCAPCFG_NRRCCFGMODE].paraLen == 0) {
        AT_WARN_LOG("AT_CheckNrrcCapCfgPara: First Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

     /* 判断参数数量 */
    if ((g_atParaIndex < AT_NRRCCAPCFG_MIN_PARA_NUM)) {
        /* 参数数量错误 */
        AT_WARN_LOG("AT_CheckNrrcCapCfgPara : Current Number wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetNrrcCapCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrrccapCfgSetReq setNrrcCapCfgReq;
    VOS_UINT32              result;
    VOS_UINT32              i;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setNrrcCapCfgReq, sizeof(setNrrcCapCfgReq), 0x00, sizeof(setNrrcCapCfgReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    result = AT_CheckNrrcCapCfgPara();
    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充结构体 */
    setNrrcCapCfgReq.nrrcCfgNetMode = g_atParaList[0].paraValue;
    setNrrcCapCfgReq.paraNum = (VOS_UINT32)(g_atParaIndex - 1);
    for (i = 0; i < AT_MTA_NRRCCAP_PARA_MAX_NUM; i++) {
        setNrrcCapCfgReq.para[i] = (g_atParaList + i + 1)->paraValue;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NRRCCAP_CFG_SET_REQ, (VOS_UINT8 *)&setNrrcCapCfgReq,
                                    sizeof(AT_MTA_NrrccapCfgSetReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRRCCAPCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_SetNrrcCapQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrrccapQryReq setNrrcCapQryReq;
    VOS_UINT32           result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setNrrcCapQryReq, sizeof(setNrrcCapQryReq), 0x00, sizeof(setNrrcCapQryReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNrrcCapQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetNrrcCapQryPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数长度不能为0，否则返回AT_CME_INCORR ECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetNrrcCapQryPara: First Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    setNrrcCapQryReq.nrrcCfgNetMode = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    ID_AT_MTA_NRRCCAP_QRY_REQ, (VOS_UINT8 *)&setNrrcCapQryReq,
                                    sizeof(AT_MTA_NrrccapQryReq), I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRRCCAPQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


LOCAL VOS_UINT32 AT_NrPwrCtrlParaCheck(AT_MTA_NrpwrctrlSetReq *atCmd)
{
    VOS_INT32  val;
    VOS_UINT32 rst;

    val = 0;
    if (AT_AtoInt((VOS_CHAR *)g_atParaList[1].para, &val) == VOS_ERR) {
        AT_WARN_LOG("AT_NrPwrCtrlParaCheck: string -> num error.");
        return VOS_ERR;
    }

    rst = VOS_ERR;
    switch ((AT_NrpwrctrlModeTypeUint32)g_atParaList[0].paraValue) {
        /* 0和2的case范围先别限制，物理层开发的时候再修改 */
        case AT_NRPWRCTRL_MODE_RI_NUM_CTR:
            if ((val == AT_NRPWRCTRL_DISABLE_REDUCE_RI) || (val == AT_NRPWRCTRL_ENABLE_REDUCE_RI)) {
                rst = VOS_OK;
            }
            break;

        default:
            AT_WARN_LOG("AT_NrPwrCtrlParaCheck: unexpected mode type.");
            break;
    }

    if (rst == VOS_ERR) {
        return VOS_ERR;
    }

    atCmd->mode = (VOS_UINT8)g_atParaList[0].paraValue;
    atCmd->para = (VOS_INT16)val;

    return VOS_OK;
}


VOS_UINT32 AT_SetNrPwrCtrlPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrpwrctrlSetReq atCmd;
    VOS_UINT32             rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNrPwrCtrlPara: ucCmdOptType is not SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_NRPWRCTRL_PARA_NUM) {
        AT_WARN_LOG("AT_SetNrPwrCtrlPara: Para num is not correct.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数1和参数2的长度不能为0 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_WARN_LOG("AT_SetNrPwrCtrlPara: Length of para is 0.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数2的输入最大范围为(-8~63),长度不能大于2 */
    if (g_atParaList[1].paraLen > 2) {
        AT_WARN_LOG("AT_SetNrPwrCtrlPara: Length of para 2 is greater than 2.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&atCmd, sizeof(AT_MTA_NrpwrctrlSetReq), 0, sizeof(AT_MTA_NrpwrctrlSetReq));

    rst = AT_NrPwrCtrlParaCheck(&atCmd);

    if (rst == VOS_ERR) {
        AT_WARN_LOG("AT_SetNrPwrCtrlPara: Check para fail.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 下发参数更新请求消息给C核处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_NRPWRCTRL_SET_REQ, (VOS_UINT8 *)&atCmd,
                                 sizeof(AT_MTA_NrpwrctrlSetReq), I0_UEPS_PID_MTA);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRPWRCTRL_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_TestNrPwrCtrlPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: (0-2),(0-1)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}
#endif


VOS_UINT32 AT_RcvMtaTxPowerQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg           *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_TxpowerQryCnf *txpwrQryCnf = VOS_NULL_PTR;
    VOS_UINT32            result;
    VOS_UINT16            length;
    VOS_UINT8             indexNum;

    rcvMsg      = (AT_MTA_Msg *)msg;
    txpwrQryCnf = (MTA_AT_TxpowerQryCnf *)rcvMsg->content;
    length      = 0;
    indexNum    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaTxPowerQryCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaTxPowerQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_TXPOWER_QRY) {
        AT_WARN_LOG("AT_RcvMtaTxPowerQryCnf: WARNING: CmdCurrentOpt != AT_CMD_TXPOWER_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(txpwrQryCnf->result);

    if (txpwrQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, txpwrQryCnf->txpwrInfo.guTxPwr,
            txpwrQryCnf->txpwrInfo.puschPwr, txpwrQryCnf->txpwrInfo.pucchPwr, txpwrQryCnf->txpwrInfo.srsPwr,
            txpwrQryCnf->txpwrInfo.prachPwr);
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMtaNtxPowerQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg      = VOS_NULL_PTR;
    MTA_AT_NtxpowerQryCnf *txpwrQryCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT32             loop;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    rcvMsg      = (AT_MTA_Msg *)msg;
    txpwrQryCnf = (MTA_AT_NtxpowerQryCnf *)rcvMsg->content;
    length      = 0;
    indexNum    = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNtxPowerQryCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNtxPowerQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NTXPOWER_QRY) {
        AT_WARN_LOG("AT_RcvMtaNtxPowerQryCnf: WARNING: CmdCurrentOpt != AT_CMD_NTXPOWER_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(txpwrQryCnf->result);

    if (txpwrQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        for (loop = 0; loop < (VOS_UINT32)TAF_MIN(txpwrQryCnf->nrCellNum, MTA_AT_MAX_CC_NUMBER); loop++) {
            if (txpwrQryCnf->nrTxPwrInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%d,%d,%d,",
                    txpwrQryCnf->nrTxPwrInfo[loop].sulPuschPwr, txpwrQryCnf->nrTxPwrInfo[loop].sulPucchPwr,
                    txpwrQryCnf->nrTxPwrInfo[loop].sulSrsPwr, txpwrQryCnf->nrTxPwrInfo[loop].sulPrachPwr,
                    txpwrQryCnf->nrTxPwrInfo[loop].sulFreq);
            } else {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d,%d,%d,%d,%d,",
                    txpwrQryCnf->nrTxPwrInfo[loop].ulPuschPwr, txpwrQryCnf->nrTxPwrInfo[loop].ulPucchPwr,
                    txpwrQryCnf->nrTxPwrInfo[loop].ulSrsPwr, txpwrQryCnf->nrTxPwrInfo[loop].ulPrachPwr,
                    txpwrQryCnf->nrTxPwrInfo[loop].ulFreq);
            }
        }
        /* 删除循环最后的,字符 */
        g_atSendDataBuff.bufLen = length - 1;
    } else {
        g_atSendDataBuff.bufLen = length;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMtaMcsSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg       *rcvMsg         = VOS_NULL_PTR;
    MTA_AT_McsQryCnf *mtaAtQryMcsCnf = VOS_NULL_PTR;
    AT_ModemNetCtx   *netCtx         = VOS_NULL_PTR;
    VOS_UINT32        result;
    VOS_UINT16        length;
    VOS_UINT8         indexNum;

    rcvMsg         = (AT_MTA_Msg *)msg;
    mtaAtQryMcsCnf = (MTA_AT_McsQryCnf *)rcvMsg->content;

    indexNum = 0;
    length   = 0;

    /* 通过 Clientid 获取 index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaMcsSetCnf: WARNING: AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaMcsSetCnf: WARNING:AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_MCS_SET) {
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromClientId(indexNum);

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(mtaAtQryMcsCnf->result);

    if (mtaAtQryMcsCnf->result == MTA_AT_RESULT_NO_ERROR) {
        if (netCtx->mcsDirection == AT_MCS_DIRECTION_UL) {
            AT_ProcUlMcsRsp(mtaAtQryMcsCnf, indexNum, &length);
        } else {
            AT_ProcDlMcsRsp(mtaAtQryMcsCnf, indexNum, &length);
        }
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaHfreqinfoQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg             *rcvMsg           = VOS_NULL_PTR;
    MTA_AT_HfreqinfoQryCnf *mtaAtQryHfreqCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    VOS_UINT32 loop;
#endif
    VOS_UINT16 length;
    VOS_UINT8  indexNum;

    rcvMsg           = (AT_MTA_Msg *)msg;
    mtaAtQryHfreqCnf = (MTA_AT_HfreqinfoQryCnf *)rcvMsg->content;

    indexNum = 0;
    length   = 0;

    /* 通过 Clientid 获取 index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaHfreqinfoQryCnf: WARNING: AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaHfreqinfoQryCnf: WARNING:AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HFREQINFO_QRY) {
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(mtaAtQryHfreqCnf->result);

    if (mtaAtQryHfreqCnf->result == MTA_AT_RESULT_NO_ERROR) {
        if (mtaAtQryHfreqCnf->resultType != MTA_AT_RESULT_TYPE_NR) {
            /* LTE */
            length +=
                (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d,%d,%d,%d,%d,%d,%d",
                    g_parseContext[indexNum].cmdElement->cmdName, mtaAtQryHfreqCnf->isreportFlg, HFREQ_INFO_RAT_LTE,
                    mtaAtQryHfreqCnf->lteHfreqInfo.band, mtaAtQryHfreqCnf->lteHfreqInfo.dlEarfcn,
                    mtaAtQryHfreqCnf->lteHfreqInfo.dlFreq, mtaAtQryHfreqCnf->lteHfreqInfo.dlBandWidth,
                    mtaAtQryHfreqCnf->lteHfreqInfo.ulEarfcn, mtaAtQryHfreqCnf->lteHfreqInfo.ulFreq,
                    mtaAtQryHfreqCnf->lteHfreqInfo.ulBandWidth);
        }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        if (mtaAtQryHfreqCnf->resultType == MTA_AT_RESULT_TYPE_DC) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }

        if (mtaAtQryHfreqCnf->resultType != MTA_AT_RESULT_TYPE_LTE) {
            /* NR上行暂不支持并发 */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                mtaAtQryHfreqCnf->isreportFlg, HFREQ_INFO_RAT_NR);

            for (loop = 0; loop < (VOS_UINT32)AT_MIN(mtaAtQryHfreqCnf->nrCellNum, MTA_AT_MAX_CC_NUMBER); loop++) {
                if (mtaAtQryHfreqCnf->nrHfreqInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", mtaAtQryHfreqCnf->nrHfreqInfo[loop].sulBand);
                } else {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", mtaAtQryHfreqCnf->nrHfreqInfo[loop].band);
                }

                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d", mtaAtQryHfreqCnf->nrHfreqInfo[loop].dlNarfcn,
                    mtaAtQryHfreqCnf->nrHfreqInfo[loop].dlFreq, mtaAtQryHfreqCnf->nrHfreqInfo[loop].dlBandWidth);

                if (mtaAtQryHfreqCnf->nrHfreqInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d",
                        mtaAtQryHfreqCnf->nrHfreqInfo[loop].sulNarfcn, mtaAtQryHfreqCnf->nrHfreqInfo[loop].sulFreq,
                        mtaAtQryHfreqCnf->nrHfreqInfo[loop].sulBandWidth);
                } else {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d",
                        mtaAtQryHfreqCnf->nrHfreqInfo[loop].ulNarfcn, mtaAtQryHfreqCnf->nrHfreqInfo[loop].ulFreq,
                        mtaAtQryHfreqCnf->nrHfreqInfo[loop].ulBandWidth);
                }
            }
        }
#endif
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaHfreqinfoInd(struct MsgCB *msg)
{
    AT_MTA_Msg          *rcvMsg        = VOS_NULL_PTR;
    MTA_AT_HfreqinfoInd *mtaAtHfreqInd = VOS_NULL_PTR;
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    VOS_UINT32 loop;
#endif
    VOS_UINT16 length;
    VOS_UINT8  indexNum;

    rcvMsg        = (AT_MTA_Msg *)msg;
    mtaAtHfreqInd = (MTA_AT_HfreqinfoInd *)rcvMsg->content;

    indexNum = 0;
    length   = 0;

    /* 通过 Clientid 获取 index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaHfreqinfoInd: WARNING: AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (mtaAtHfreqInd->result == MTA_AT_RESULT_NO_ERROR) {
        if (mtaAtHfreqInd->resultType == MTA_AT_RESULT_TYPE_LTE) {
            /* LTE */
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s^HFREQINFO: %d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
                HFREQ_INFO_RAT_LTE, mtaAtHfreqInd->lteHfreqInfo.band, mtaAtHfreqInd->lteHfreqInfo.dlEarfcn,
                mtaAtHfreqInd->lteHfreqInfo.dlFreq, mtaAtHfreqInd->lteHfreqInfo.dlBandWidth,
                mtaAtHfreqInd->lteHfreqInfo.ulEarfcn, mtaAtHfreqInd->lteHfreqInfo.ulFreq,
                mtaAtHfreqInd->lteHfreqInfo.ulBandWidth, g_atCrLf);
        }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        if (mtaAtHfreqInd->resultType == MTA_AT_RESULT_TYPE_NR) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s^HFREQINFO: %d", g_atCrLf, HFREQ_INFO_RAT_NR);
            for (loop = 0; loop < (VOS_UINT32)AT_MIN(mtaAtHfreqInd->nrCellNum, MTA_AT_MAX_CC_NUMBER); loop++) {
                if (mtaAtHfreqInd->nrHfreqInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", mtaAtHfreqInd->nrHfreqInfo[loop].sulBand);
                } else {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", mtaAtHfreqInd->nrHfreqInfo[loop].band);
                }
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d", mtaAtHfreqInd->nrHfreqInfo[loop].dlNarfcn,
                    mtaAtHfreqInd->nrHfreqInfo[loop].dlFreq, mtaAtHfreqInd->nrHfreqInfo[loop].dlBandWidth);

                if (mtaAtHfreqInd->nrHfreqInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d",
                        mtaAtHfreqInd->nrHfreqInfo[loop].sulNarfcn, mtaAtHfreqInd->nrHfreqInfo[loop].sulFreq,
                        mtaAtHfreqInd->nrHfreqInfo[loop].sulBandWidth);
                } else {
                    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d",
                        mtaAtHfreqInd->nrHfreqInfo[loop].ulNarfcn, mtaAtHfreqInd->nrHfreqInfo[loop].ulFreq,
                        mtaAtHfreqInd->nrHfreqInfo[loop].ulBandWidth);
                }
            }
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
#endif
    }

    g_atSendDataBuff.bufLen = length;

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMtaFastReturn5gEndcInd(struct MsgCB *msg)
{
    AT_MTA_Msg  *rcvMsg   = VOS_NULL_PTR;
    VOS_UINT8    indexNum = 0;

    rcvMsg   = (AT_MTA_Msg *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaFastReturn5gEndcInd: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^CONNECTRECOVERY");
    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    return VOS_OK;
}
#endif


VOS_UINT32 AT_RcvMmaRrcStatQryCnf(struct MsgCB *msg)
{
    TAF_MMA_RrcstatQryCnf *rrcStatQryCnf = VOS_NULL_PTR;
    VOS_UINT16             length;
    VOS_UINT8              indexNum;

    rrcStatQryCnf = (TAF_MMA_RrcstatQryCnf *)msg;
    length        = 0;
    indexNum      = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rrcStatQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRrcStatQryCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMmaRrcStatQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_RRCSTAT_QRY) {
        AT_WARN_LOG("AT_RcvMmaRrcStatQryCnf: WARNING: CmdCurrentOpt != AT_CMD_RRCSTAT_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (rrcStatQryCnf->rslt == TAF_ERR_ERROR) {
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData(indexNum, AT_ERROR);
        return VOS_OK;
    }

    if (rrcStatQryCnf->rrcCampStat.netRat == TAF_MMA_RAT_LTE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            rrcStatQryCnf->reportFlg, rrcStatQryCnf->rrcCampStat.rrcStat,
            rrcStatQryCnf->rrcCampStat.tempCampStat);

        g_atSendDataBuff.bufLen = length;
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            rrcStatQryCnf->reportFlg, rrcStatQryCnf->rrcCampStat.rrcStat);

        g_atSendDataBuff.bufLen = length;
    }

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaRrcStatInd(struct MsgCB *msg)
{
    VOS_UINT8           indexNum;
    TAF_MMA_RrcstatInd *rrcStat = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum = 0;
    rrcStat  = (TAF_MMA_RrcstatInd *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(rrcStat->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaRrcStatInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = 0;

    if (rrcStat->rrcCampStat.netRat == TAF_MMA_RAT_LTE) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^RRCSTAT: %d,%d%s",
            g_atCrLf, rrcStat->rrcCampStat.rrcStat, rrcStat->rrcCampStat.tempCampStat, g_atCrLf);
    }

#if (FEATURE_ON == FEATURE_UE_MODE_NR)
    if (rrcStat->rrcCampStat.netRat == TAF_MMA_RAT_NR) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^RRCSTAT: %d%s", g_atCrLf, rrcStat->rrcCampStat.rrcStat, g_atCrLf);
    }
#endif

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaPseudBtsIdentInd(struct MsgCB *rcvMsg)
{
    VOS_UINT8                 indexNum;
    TAF_MMA_PseudBtsIdentInd *pseudBtsIdentInfo = VOS_NULL_PTR;

    /* 初始化消息变量 */
    indexNum          = 0;
    pseudBtsIdentInfo = (TAF_MMA_PseudBtsIdentInd *)rcvMsg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(pseudBtsIdentInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaPseudBtsIdentInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s: %d%s", g_atCrLf, g_atStringTab[AT_STRING_PSEUDBTS].text,
        pseudBtsIdentInfo->currentRat, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


LOCAL VOS_VOID AT_ProcUlMcsRsp(MTA_AT_McsQryCnf *mtaAtQryMcsCnf, VOS_UINT8 indexNum, VOS_UINT16 *length)
{
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    VOS_UINT32 loop;
#endif

    if (mtaAtQryMcsCnf->resultType != MTA_AT_RESULT_TYPE_NR) {
        /* LTE */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, AT_MCS_DIRECTION_UL, MCS_RAT_LTE, MTA_AT_INDEX_TABLE_ONE,
            mtaAtQryMcsCnf->lteMcsInfo.ulMcs[0], mtaAtQryMcsCnf->lteMcsInfo.ulMcs[1]);
    }

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    if (mtaAtQryMcsCnf->resultType == MTA_AT_RESULT_TYPE_DC) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
    }

    if (mtaAtQryMcsCnf->resultType != MTA_AT_RESULT_TYPE_LTE) {
        /* NR */
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_MCS_DIRECTION_UL, MCS_RAT_NR);
        for (loop = 0; loop < (VOS_UINT32)AT_MIN(mtaAtQryMcsCnf->nrCellNum, MTA_AT_MAX_CC_NUMBER); loop++) {
            /* NR上行暂不支持并发 */
            if (mtaAtQryMcsCnf->nrMcsInfo[loop].ulMode == MTA_AT_UL_MODE_UL_ONLY) {
                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,%d,%d", mtaAtQryMcsCnf->nrMcsInfo[loop].ulMcsTable,
                    mtaAtQryMcsCnf->nrMcsInfo[loop].ulMcs[0], mtaAtQryMcsCnf->nrMcsInfo[loop].ulMcs[1]);
            } else if (mtaAtQryMcsCnf->nrMcsInfo[loop].ulMode == MTA_AT_UL_MODE_SUL_ONLY) {
                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,%d,%d", mtaAtQryMcsCnf->nrMcsInfo[loop].sulMcsTable,
                    mtaAtQryMcsCnf->nrMcsInfo[loop].sulMcs[0], mtaAtQryMcsCnf->nrMcsInfo[loop].sulMcs[1]);
            } else {
                *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,%d,%d", MTA_AT_INDEX_TABLE_BUTT,
                    MTA_AT_INVALID_MCS_VALUE, MTA_AT_INVALID_MCS_VALUE);
            }
        }
    }
#endif
}


LOCAL VOS_VOID AT_ProcDlMcsRsp(MTA_AT_McsQryCnf *mtaAtQryMcsCnf, VOS_UINT8 indexNum, VOS_UINT16 *length)
{
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    VOS_UINT32 loop;
#endif

    if (mtaAtQryMcsCnf->resultType != MTA_AT_RESULT_TYPE_NR) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d,%d,%d,%d",
            g_parseContext[indexNum].cmdElement->cmdName, AT_MCS_DIRECTION_DL, MCS_RAT_LTE, MTA_AT_INDEX_TABLE_ONE,
            mtaAtQryMcsCnf->lteMcsInfo.dlMcs[0], mtaAtQryMcsCnf->lteMcsInfo.dlMcs[1]);
    }
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    if (mtaAtQryMcsCnf->resultType == MTA_AT_RESULT_TYPE_DC) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
    }

    if (mtaAtQryMcsCnf->resultType != MTA_AT_RESULT_TYPE_LTE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_MCS_DIRECTION_DL, MCS_RAT_NR);
        for (loop = 0; loop < (VOS_UINT32)AT_MIN(mtaAtQryMcsCnf->nrCellNum, MTA_AT_MAX_CC_NUMBER); loop++) {
            *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,%d,%d", mtaAtQryMcsCnf->nrMcsInfo[loop].dlMcsTable,
                mtaAtQryMcsCnf->nrMcsInfo[loop].dlMcs[0], mtaAtQryMcsCnf->nrMcsInfo[loop].dlMcs[1]);
        }
    }
#endif
}

#if ((FEATURE_UE_MODE_NR == FEATURE_ON) && defined(FEATURE_PHONE_ENG_AT_CMD) && \
     (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_SetUartTest(VOS_UINT8 indexId)
{
    VOS_UINT32 result;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        AT_WARN_LOG("AT_SetUartTest: Incorrect Cmd Type.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 测试状态为running时不能设置，已出数据校验结果时可以设置 */
    if (AT_GetUartTestState() == AT_UART_TEST_RUNNING) {
        AT_WARN_LOG("AT_SetUartTest: is testing!");
        return AT_ERROR;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexId].clientId, 0, ID_AT_MTA_UART_TEST_REQ, VOS_NULL, 0,
                                    I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexId].cmdCurrentOpt = AT_CMD_UART_TEST_SET;
        AT_SetUartTestState(AT_UART_TEST_RUNNING);
        return AT_WAIT_ASYNC_RETURN;
    } else {
        AT_WARN_LOG("AT_SetUartTest: Send msg fail!");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryUartTest(VOS_UINT8 indexId)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", "^UARTTEST", AT_GetUartTestState());

    /* 不是正在测试，查询后统一设置成初始状态 */
    if (AT_GetUartTestState() != AT_UART_TEST_RUNNING) {
        AT_SetUartTestState(AT_UART_TEST_NOT_START);
    }
    return AT_OK;
}


VOS_UINT32 AT_SetI2sTest(VOS_UINT8 indexId)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 状态错误 */
    if (AT_GetI2sTestState() == AT_I2S_TEST_RUNNING) {
        AT_WARN_LOG("AT_SetI2sTest: is testing!");
        return AT_ERROR;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexId].clientId, g_atClientTab[indexId].opId, APP_VC_MSG_I2S_TEST_REQ,
                                 VOS_NULL_PTR, 0, I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexId].cmdCurrentOpt = AT_CMD_I2S_TEST_SET;
        AT_SetI2sTestState(AT_I2S_TEST_RUNNING);
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryI2sTest(VOS_UINT8 indexId)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", "^I2STEST", AT_GetI2sTestState());

    /* 不是正在测试，查询后统一设置成初始状态 */
    if (AT_GetI2sTestState() != AT_I2S_TEST_RUNNING) {
        AT_SetI2sTestState(AT_I2S_TEST_NOT_START);
    }
    return AT_OK;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_RcvMta5gOptionSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_Set5GOptionCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_Set5GOptionCnf *)rcvMsg->content;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMta5gOptionSetCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMta5gOptionSetCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_5G_OPTION_SET) {
        AT_WARN_LOG("AT_RcvMta5gOptionSetCnf : Current Option is not AT_CMD_5G_OPTION_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->rslt != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMta5gOptionQryCnf(struct MsgCB *msg)
{
    AT_MTA_Msg            *rcvMsg = VOS_NULL_PTR;
    MTA_AT_Qry5GOptionCnf *mtaCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT8              indexNum;

    /* 初始化 */
    rcvMsg   = (AT_MTA_Msg *)msg;
    mtaCnf   = (MTA_AT_Qry5GOptionCnf *)rcvMsg->content;
    result   = AT_OK;
    indexNum = 0;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMta5gOptionQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMta5gOptionQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_5G_OPTION_QRY) {
        AT_WARN_LOG("AT_RcvMta5gOptionQryCnf : Current Option is not AT_CMD_5G_OPTION_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    g_atSendDataBuff.bufLen = 0;

    if (mtaCnf->rslt != MTA_AT_RESULT_NO_ERROR) {
        result = AT_ERROR;
    } else {
        g_atSendDataBuff.bufLen =
            (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                mtaCnf->nrSaSupportFlag, mtaCnf->nrDcMode, mtaCnf->en5gcAccessMode);
    }

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


LOCAL VOS_UINT32 AT_IsNrBandValid(VOS_UINT16 nrBand)
{
    if ((nrBand >= 1) && (nrBand <= TAF_MMA_MAX_NR_BAND_INDEX)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


LOCAL VOS_UINT32 AT_IsNrBandParaCharValid(VOS_UINT8 charactor)
{
    if (isdigit(charactor) || (charactor == ',')) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


LOCAL VOS_UINT32 AT_GetNrBandBlockList(VOS_UINT16 *nrBandBlockList, VOS_UINT32 bandMaxNum)
{
    VOS_UINT8 *tempPtr = VOS_NULL_PTR;
    VOS_UINT32 bandNum = 0;
    VOS_UINT32 nrBand  = 0;
    VOS_UINT16 bandLen = 0;
    VOS_UINT32 i;

    tempPtr = g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].para;

    for (i = 0; i < (VOS_UINT32)g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].paraLen + 1U; i++) {
        if (i < g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].paraLen) {
            if (AT_IsNrBandParaCharValid(g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].para[i]) != VOS_TRUE) {
                return 0;
            }

            if (g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].para[i] != ',') {
                bandLen++;
                if (bandLen > 3) {
                    /* 单个Band值不允许超过3位数 */
                    return 0;
                }
                continue;
            }
        }

        if (bandLen != 0) {
            if (bandNum >= bandMaxNum) {
                /* 超过最大Band个数按照失败返回 */
                return 0;
            }

            if (atAuc2ul(tempPtr, bandLen, &nrBand) != AT_SUCCESS) {
                return 0;
            }

            if (AT_IsNrBandValid((VOS_UINT16)nrBand) != VOS_TRUE) {
                return 0;
            }

            nrBandBlockList[bandNum] = (VOS_UINT16)nrBand;
            bandNum++;
            tempPtr += bandLen + 1;
            bandLen = 0;
        }
    }

    return bandNum;
}


LOCAL VOS_UINT32 AT_HasIdenticalItems(VOS_UINT16 *list, VOS_UINT32 num)
{
    VOS_UINT32 i, j;

    for (i = 0; i < num; i++) {
        for (j = i + 1; j < num; j++) {
            if (list[i] == list[j]) {
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_SetNrBandBlockListWith3Para(TAF_MMA_NrBandBlockListSetPara *nrBandBlockListSetStru)
{
    VOS_UINT32 maxStrlen;

    /* 字符串最长的长度是10个3位数和9个逗号，共39 */
    maxStrlen = 3 * TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM + 9;

    if ((g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue !=
        TAF_MMA_NR_BAND_SET_OPTION_DEL_BAND_FROM_BLOCK_LIST) &&
        (g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue !=
        TAF_MMA_NR_BAND_SET_OPTION_ADD_BAND_TO_BLOCK_LIST)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_NUM].paraLen != AT_NRBANDBLOCKLIST_PARA_BAND_VALID_VALUE1) &&
        (g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_NUM].paraLen != AT_NRBANDBLOCKLIST_PARA_BAND_VALID_VALUE2)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].paraLen == 0) ||
        (g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_LIST].paraLen > maxStrlen)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    nrBandBlockListSetStru->option  = g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue;
    nrBandBlockListSetStru->bandNum = AT_GetNrBandBlockList(nrBandBlockListSetStru->bandList,
                                                            TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM);

    if (nrBandBlockListSetStru->bandNum != g_atParaList[AT_NRBANDBLOCKLIST_PARA_BAND_NUM].paraValue) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_HasIdenticalItems(nrBandBlockListSetStru->bandList, nrBandBlockListSetStru->bandNum) == VOS_TRUE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetNrBandBlockListPara(VOS_UINT8 atIndex)
{
    TAF_MMA_NrBandBlockListSetPara nrBandBlockListSetStru;
    ModemIdUint16                  modemId;
    VOS_UINT32                     rslt;

    rslt = AT_GetModemIdFromClient(atIndex, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG1("AT_SetNrBandBlockListPara:Get ModemID From ClientID fail,ClientID:", atIndex);
        return AT_ERROR;
    }

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果参数长度不符合命令 */
    if (g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果9203不支持NR，直接返回ERR */
    if (AT_IsModemSupportRat(modemId, TAF_MMA_RAT_NR) != VOS_TRUE) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&nrBandBlockListSetStru, sizeof(nrBandBlockListSetStru), 0x00, sizeof(nrBandBlockListSetStru));

    if (g_atParaIndex == AT_NRBANDBLOCKLIST_PARA_MIN_NUM) {
        /* 高频开关 */
        if ((g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue != TAF_MMA_NR_BAND_SET_OPTION_TURN_FR2_ON) &&
            (g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue != TAF_MMA_NR_BAND_SET_OPTION_TURN_FR2_OFF)) {
            return AT_CME_INCORRECT_PARAMETERS;
        } else {
            nrBandBlockListSetStru.option = g_atParaList[AT_NRBANDBLOCKLIST_PARA_OPTION].paraValue;
        }
    } else if (g_atParaIndex == AT_NRBANDBLOCKLIST_PARA_MAX_NUM) {
        /* BAND Block名单设置 */
        if (AT_SetNrBandBlockListWith3Para(&nrBandBlockListSetStru) != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    if (TAF_MMA_SetNrBandBlockListReq(WUEPS_PID_AT, g_atClientTab[atIndex].clientId, 0, &nrBandBlockListSetStru) ==
        VOS_TRUE) {
        /* 设置当前操作类型 */
        g_atClientTab[atIndex].cmdCurrentOpt = AT_CMD_NR_BAND_BLOCKLIST_SET;
        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMmaNrBandBlockListSetCnf(struct MsgCB *msg)
{
    TAF_MMA_NrBandBlockListSetCnf *nrBandBlockListSetCnf = VOS_NULL_PTR;
    VOS_UINT32                     result;
    VOS_UINT8                      atIndex;
    VOS_UINT32                     msgLength;

    nrBandBlockListSetCnf = (TAF_MMA_NrBandBlockListSetCnf *)msg;
    atIndex               = 0;

    msgLength = sizeof(TAF_MMA_NrBandBlockListSetCnf) - VOS_MSG_HEAD_LENGTH;

    if (msgLength != VOS_GET_MSG_LEN(nrBandBlockListSetCnf)) {
        AT_ERR_LOG("AT_RcvMmaNrBandBlockListSetCnf: ERROR: MSG LENGTH ERROR!");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(nrBandBlockListSetCnf->ctrl.clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListSetCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(atIndex)) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[atIndex].cmdCurrentOpt != AT_CMD_NR_BAND_BLOCKLIST_SET) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListSetCnf: WARNING: CmdCurrentOpt != AT_CMD_NR_BAND_BLOCKLIST_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(atIndex);

    g_atSendDataBuff.bufLen = 0;

    if (nrBandBlockListSetCnf->result != TAF_ERR_NO_ERROR) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(atIndex, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMmaNrBandBlockListQryCnf(struct MsgCB *msg)
{
    TAF_MMA_NrBandBlockListQryCnf *rcvMsg = VOS_NULL_PTR;
    VOS_UINT16                     length;
    VOS_UINT8                      atIndex = 0;
    VOS_UINT32                     bandNum;
    VOS_UINT32                     i;
    VOS_UINT32                     msgLength;

    /* 初始化 */
    rcvMsg    = (TAF_MMA_NrBandBlockListQryCnf *)msg;
    msgLength = sizeof(TAF_MMA_NrBandBlockListQryCnf) - VOS_MSG_HEAD_LENGTH;

    if (msgLength != VOS_GET_MSG_LEN(rcvMsg)) {
        AT_ERR_LOG("AT_RcvMmaNrBandBlockListQryCnf: ERROR: MSG LENGTH ERROR!");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->ctrl.clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(atIndex)) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[atIndex].cmdCurrentOpt != AT_CMD_NR_BAND_BLOCKLIST_QRY) {
        AT_WARN_LOG("AT_RcvMmaNrBandBlockListQryCnf : Current Option is not AT_CMD_NR_BAND_BLOCKLIST_QRY.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(atIndex);

    g_atSendDataBuff.bufLen = 0;

    if (rcvMsg->result != TAF_ERR_NO_ERROR) {
        At_FormatResultData(atIndex, AT_ERROR);
        return VOS_OK;
    }

    bandNum = (VOS_UINT32)AT_MIN(rcvMsg->bandNum, TAF_MMA_MAX_NR_BAND_BLOCKLIST_NUM);

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[atIndex].cmdElement->cmdName, rcvMsg->fr2SupportSw,
        bandNum);

    for (i = 0; i < bandNum; i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, ",%d", rcvMsg->bandList[i]);
    }

    g_atSendDataBuff.bufLen = length;

    /* 输出结果 */
    At_FormatResultData(atIndex, AT_OK);

    return VOS_OK;
}


LOCAL VOS_UINT32 AT_CheckNwDeploymentPara(VOS_VOID)
{
    /* 宏观参数个数不正确,4~6个参数 */
    if ((g_atParaIndex > 6) || (g_atParaIndex < 4)) {
        AT_ERR_LOG("AT_CheckNwDeploymentPara: number of parameter error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 输入参数长度不对 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen != AT_CSS_PLMN_DEPLOYMENT_VERSION_LEN) ||
        (g_atParaList[2].paraLen == 0)) {
        AT_ERR_LOG("AT_CheckNwDeploymentPara: para len error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* BS_count一共有3种取值，如果参数个数不对，那么认为设置命令错误返回 */
    if (g_atParaIndex != g_atParaList[2].paraValue + 3) {
        AT_ERR_LOG("AT_CheckNwDeploymentPara: number of BS error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


LOCAL VOS_UINT32 AT_FormatNwDeploymentMsg(VOS_UINT32 byteStrLen, ModemIdUint16 modemId,
    VOS_UINT8 atIndex)
{
    AT_CSS_PlmnDeploymentInfoSetReq *msg = VOS_NULL_PTR;
    VOS_UINT_PTR                     msgDataAddr;
    VOS_UINT32                       bufLen;
    errno_t                          memResult;

    /* 预制频点信息长度 */
    bufLen = sizeof(AT_CSS_PlmnDeploymentInfoSetReq);

    if (byteStrLen > AT_MSG_DEFAULT_VALUE_LEN) {
        bufLen = bufLen + byteStrLen - AT_MSG_DEFAULT_VALUE_LEN;
    }

    /* 申请消息包AT_CSS_PlmnDeploymentInfoSetReq */
    msg = (AT_CSS_PlmnDeploymentInfoSetReq *)AT_ALLOC_MSG_WITH_HDR(bufLen);

    /* 内存申请失败，返回 */
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_SetNwDeploymentPara: memory alloc fail.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s((VOS_INT8 *)msg + VOS_MSG_HEAD_LENGTH, (VOS_SIZE_T)bufLen - VOS_MSG_HEAD_LENGTH, 0x00,
                       (VOS_SIZE_T)bufLen - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头 */
    AT_CFG_MSG_HDR(msg, PS_PID_CSS, ID_AT_CSS_PLMN_DEPLOYMENT_INFO_SET_REQ);

    /* 填写消息内容 */
    msg->modemId = modemId;
    msg->clientId = g_atClientTab[atIndex].clientId;
    msg->seq = (VOS_UINT8)g_atParaList[0].paraValue;
    msg->dataLen = byteStrLen;

    memResult = memcpy_s(msg->versionId, sizeof(msg->versionId), g_atParaList[1].para, g_atParaList[1].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(msg->versionId), g_atParaList[1].paraLen);

    /* 填写data[4]，根据<BS_count>的个数来赋值 */
    msgDataAddr = (VOS_UINT_PTR)msg->data;
    if (g_atParaList[2].paraValue == 1) {
        memResult = memcpy_s((VOS_UINT8*)msgDataAddr, byteStrLen, g_atParaList[3].para, g_atParaList[3].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen, g_atParaList[3].paraLen);
    }else if (g_atParaList[2].paraValue == 2) {
        memResult = memcpy_s((VOS_UINT8*)msgDataAddr, byteStrLen, g_atParaList[3].para, g_atParaList[3].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen, g_atParaList[3].paraLen);

        memResult = memcpy_s(msg->data + g_atParaList[3].paraLen, byteStrLen - g_atParaList[3].paraLen,
                             g_atParaList[4].para, g_atParaList[4].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen - g_atParaList[3].paraLen, g_atParaList[4].paraLen);
    } else {
        memResult = memcpy_s((VOS_UINT8*)msgDataAddr, byteStrLen, g_atParaList[3].para, g_atParaList[3].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen, g_atParaList[3].paraLen);

        memResult = memcpy_s(msg->data + g_atParaList[3].paraLen, byteStrLen - g_atParaList[3].paraLen,
                             g_atParaList[4].para, g_atParaList[4].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen - g_atParaList[3].paraLen, g_atParaList[4].paraLen);

        memResult = memcpy_s(msg->data + g_atParaList[3].paraLen + g_atParaList[4].paraLen,
                             byteStrLen - g_atParaList[3].paraLen - g_atParaList[4].paraLen,
                             g_atParaList[5].para, g_atParaList[5].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, byteStrLen - g_atParaList[3].paraLen - g_atParaList[4].paraLen,
                            g_atParaList[5].paraLen);
    }

    AT_SEND_MSG(msg);

    return AT_SUCCESS;
}



VOS_UINT32 AT_SetNwDeploymentPara(VOS_UINT8 atIndex)
{
    VOS_UINT32 byteStrLen;
    ModemIdUint16 modemId;
    VOS_UINT32 ret;
    VOS_UINT32 result;

    modemId = MODEM_ID_0;

    ret = AT_GetModemIdFromClient(atIndex, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_SetNwDeploymentPara: Get modem id fail.");
        return AT_ERROR;
    }

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetNwDeploymentPara: Cmd Opt Type is wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    result = AT_CheckNwDeploymentPara();
    if (result != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetNwDeploymentPara: check Nw Deployment para error.");
        return result;
    }

    switch (g_atParaList[2].paraValue) {
        case 1: {
            byteStrLen = g_atParaList[3].paraLen;
            break;
        }
        case 2: {
            byteStrLen = g_atParaList[3].paraLen + g_atParaList[4].paraLen;
            break;
        }
        case 3: {
            byteStrLen = g_atParaList[3].paraLen + g_atParaList[4].paraLen + g_atParaList[5].paraLen;
            break;
        }
        default: {
            AT_ERR_LOG("AT_SetNwDeploymentPara: check Nw Deployment para error.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    if (AT_FormatNwDeploymentMsg(byteStrLen, modemId, atIndex) != AT_SUCCESS) {
        AT_ERR_LOG("AT_SetNwDeploymentPara: SB is wrong.");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[atIndex].cmdCurrentOpt = AT_CMD_NWDEPLOYMENT_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvCssNwDeploymentSetCnf(struct MsgCB *msg)
{
    CSS_AT_PlmnDeploymentInfoSetCnf *plmnDeploymentInfoSetCnf = VOS_NULL_PTR;
    VOS_UINT32 result;
    VOS_UINT8 atIndex;
    VOS_UINT32 msgLength;

    plmnDeploymentInfoSetCnf = (CSS_AT_PlmnDeploymentInfoSetCnf *)msg;
    atIndex = 0;

    msgLength = sizeof(CSS_AT_PlmnDeploymentInfoSetCnf) - VOS_MSG_HEAD_LENGTH;

    if (msgLength != VOS_GET_MSG_LEN(plmnDeploymentInfoSetCnf)) {
        AT_ERR_LOG("AT_RcvCssNwDeploymentSetCnf: ERROR: MSG LENGTH ERROR!");
        return VOS_ERR;
    }

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(plmnDeploymentInfoSetCnf->clientId, &atIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvCssNwDeploymentSetCnf: WARNING: AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(atIndex)) {
        AT_WARN_LOG("AT_RcvCssNwDeploymentSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    if (g_atClientTab[atIndex].cmdCurrentOpt != AT_CMD_NWDEPLOYMENT_SET) {
        AT_WARN_LOG("AT_RcvCssNwDeploymentSetCnf: WARNING: CmdCurrentOpt != AT_CMD_NWDEPLOYMENT_SET!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(atIndex);

    g_atSendDataBuff.bufLen = 0;

    if (plmnDeploymentInfoSetCnf->result != VOS_OK) {
        result = AT_ERROR;
    } else {
        result = AT_OK;
    }

    /* 输出结果 */
    At_FormatResultData(atIndex, result);

    return VOS_OK;
}
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_RcvMtaLteSarStubSetCnf(struct MsgCB *msg)
{
    AT_MTA_Msg              *rcvMsg                = VOS_NULL_PTR;
    AT_MTA_LteSarStubSetCnf *mtaAtLteSarStubSetCnf = VOS_NULL_PTR;
    VOS_UINT8                indexNum              = 0;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    mtaAtLteSarStubSetCnf = (AT_MTA_LteSarStubSetCnf *)rcvMsg->content;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaLteSarStubSetCnf:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaLteSarStubSetCnf:AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    /* 当前AT是否在等待该命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTESARSTUB_SET) {
        AT_WARN_LOG("AT_RcvMtaLteSarStubSetCnf:Current Option is not AT_CMD_LTESARSTUB_SET.");
        return VOS_ERR;
    }
    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;

    if (mtaAtLteSarStubSetCnf->result != MTA_AT_RESULT_NO_ERROR) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }
    return VOS_OK;
}
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

LOCAL VOS_UINT32 AT_CheckNrPowerSavingCfgPara(VOS_VOID)
{
    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_CheckNrPowerSavingCfgPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 不携带参数，返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[AT_NRPOWERSAVINGCFG_CMDTYPE].paraLen == 0) {
        AT_WARN_LOG("AT_CheckNrPowerSavingCfgPara: First Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

     /* 判断参数数量 */
    if (g_atParaIndex < AT_NRPOWERSAVINGCFG_MIN_PARA_NUM || g_atParaIndex > AT_NRPOWERSAVINGCFG_MAX_PARA_NUM) {
        /* 参数数量错误 */
        AT_WARN_LOG("AT_CheckNrPowerSavingCfgPara : Wrong Number of Parameters.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetNrPowerSavingCfgPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrPowerSavingCfgSetReq setNrPowerSavingCfgReq;
    VOS_UINT32 result;
    VOS_UINT32 i;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setNrPowerSavingCfgReq, sizeof(setNrPowerSavingCfgReq), 0x00, sizeof(setNrPowerSavingCfgReq));

    /* 参数个数和合法性检查,不合法直接返回失败 */
    result = AT_CheckNrPowerSavingCfgPara();
    if (result != AT_SUCCESS) {
        return result;
    }

    /* 填充结构体 */
    setNrPowerSavingCfgReq.type = g_atParaList[0].paraValue;
    setNrPowerSavingCfgReq.paraNum = (VOS_UINT32)(g_atParaIndex - 1);
    for (i = 0; i < setNrPowerSavingCfgReq.paraNum; i++) {
        setNrPowerSavingCfgReq.para[i] = g_atParaList[i + 1].paraValue;
    }

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NRPOWERSAVING_CFG_SET_REQ, (VOS_UINT8 *)&setNrPowerSavingCfgReq, sizeof(setNrPowerSavingCfgReq),
        I0_UEPS_PID_MTA);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRPOWERSAVINGCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_SetNrPowerSavingQryPara(VOS_UINT8 indexNum)
{
    AT_MTA_NrPowerSavingQryReq setNrPowerSavingQryReq;
    VOS_UINT32                 result;

    /* 局部变量初始化 */
    (VOS_VOID)memset_s(&setNrPowerSavingQryReq, sizeof(setNrPowerSavingQryReq), 0x00, sizeof(setNrPowerSavingQryReq));

    /* 参数有效性检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetNrPowerSavingQryPara: At Cmd Opt Set Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数应为1个，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaIndex != 1) {
        AT_WARN_LOG("AT_SetNrPowerSavingQryPara: At Para Num Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 第一个参数长度不能为0，否则返回AT_CME_INCORRECT_PARAMETERS */
    if (g_atParaList[0].paraLen == 0) {
        AT_WARN_LOG("AT_SetNrPowerSavingQryPara: First Para Error.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充结构体 */
    setNrPowerSavingQryReq.type = g_atParaList[0].paraValue;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        ID_AT_MTA_NRPOWERSAVING_QRY_REQ, (VOS_UINT8 *)&setNrPowerSavingQryReq, sizeof(setNrPowerSavingQryReq),
        I0_UEPS_PID_MTA);

    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NRPOWERSAVINGQRY_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_RcvMtaNrPowerSavingCfgCnf(struct MsgCB *msg)
{
    MTA_AT_NrPowerSavingCfgSetCnf *nrPowerSavingCfgCnf = VOS_NULL_PTR;
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32  result;
    VOS_UINT8   indexNum;

    /* 初始化 */
    rcvMsg = (AT_MTA_Msg *)msg;
    nrPowerSavingCfgCnf = (MTA_AT_NrPowerSavingCfgSetCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingCfgCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingCfgCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待AT^NRPOWERSAVINGCFG命令返回 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRPOWERSAVINGCFG_SET) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingCfgCnf : Current Option is not AT_CMD_NRPOWERSAVINGCFG_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    result = AT_ConvertMtaResult(nrPowerSavingCfgCnf->result);

    /* 输出结果 */
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaNrPowerSavingQryCnf(struct MsgCB *msg)
{
    MTA_AT_NrPowerSavingQryCnf *nrPowerSavingQryCnf = VOS_NULL_PTR;
    AT_MTA_Msg *rcvMsg = VOS_NULL_PTR;
    VOS_UINT32  result;
    VOS_UINT8   indexNum;

    rcvMsg = (AT_MTA_Msg *)msg;
    nrPowerSavingQryCnf = (MTA_AT_NrPowerSavingQryCnf *)rcvMsg->content;
    indexNum = 0;
    result   = AT_ERROR;

    if (At_ClientIdToUserId(rcvMsg->appCtrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingQryCnf : WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingQryCnf : AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NRPOWERSAVINGQRY_SET) {
        AT_WARN_LOG("AT_RcvMtaNrPowerSavingQryCnf : Current Option is not AT_CMD_NRPOWERSAVINGQRY_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = 0;
    if (nrPowerSavingQryCnf->result == MTA_AT_RESULT_NO_ERROR) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
            "^NRPOWERSAVINGQRY: %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
            nrPowerSavingQryCnf->type, nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA0],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA1], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA2],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA3], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA4],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA5], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA6],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA7], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA8],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA9], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA10],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA11], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA12],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA13], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA14],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA15], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA16],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA17], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA18],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA19], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA20],
            nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA21], nrPowerSavingQryCnf->para[AT_NRPOWERSAVING_PARA22]);
        result = AT_OK;
    } else {
        result = AT_ConvertMtaResult(nrPowerSavingQryCnf->result);
    }
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#endif

