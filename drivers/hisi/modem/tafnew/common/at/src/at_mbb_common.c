/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
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

#include "at_mbb_common.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "mn_comm_api.h"
#include "taf_type_def.h"
#include "AtDataProc.h"
#include "AtTafAgentInterface.h"
#include "AtCheckFunc.h"


#define THIS_FILE_ID PS_FILE_ID_AT_MBB_COMMON_C

#if (FEATURE_MBB_CUST == FEATURE_ON)
/*
 * 功能描述: +CCWA命令的打断处理
 *
 */
VOS_UINT32 AT_AbortCcwaPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* 特性是否打开 */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->ssAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    clientOperationId.clientId = g_atClientTab[indexNum].clientId;
    clientOperationId.opId = 0;
    clientOperationId.reserve = 0;

    /* +CCWA开启/关闭/查询处理需支持可中断 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CCWA_DISABLE) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CCWA_ENABLE) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CCWA_QUERY)) {
        result = MN_FillAndSndAppReqMsg(&clientOperationId, TAF_MSG_ABORT_SS_MSG, VOS_NULL_PTR, 0, I0_WUEPS_PID_TAF);
        if (result == TAF_SUCCESS) {
            return AT_ABORT;
        }
    }
    return AT_FAILURE;
}

/*
 * 功能描述: +CLIP命令的打断处理
 *
 */
VOS_UINT32 AT_AbortClipPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* 特性是否打开 */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->ssAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    clientOperationId.clientId = g_atClientTab[indexNum].clientId;
    clientOperationId.opId = 0;
    clientOperationId.reserve = 0;

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CLIP_READ) {
        result = MN_FillAndSndAppReqMsg(&clientOperationId, TAF_MSG_ABORT_SS_MSG, VOS_NULL_PTR, 0, I0_WUEPS_PID_TAF);
        if (result == TAF_SUCCESS) {
            return AT_ABORT;
        }
    }
    return AT_FAILURE;
}

/*
 * 功能描述: +CLIR命令的打断处理
 *
 */
VOS_UINT32 AT_AbortClirPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* 特性是否打开 */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->ssAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    clientOperationId.clientId = g_atClientTab[indexNum].clientId;
    clientOperationId.opId = 0;
    clientOperationId.reserve = 0;

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CLIR_READ) {
        result = MN_FillAndSndAppReqMsg(&clientOperationId, TAF_MSG_ABORT_SS_MSG, VOS_NULL_PTR, 0, I0_WUEPS_PID_TAF);
        if (result == TAF_SUCCESS) {
            return AT_ABORT;
        }
    }
    return AT_FAILURE;
}

/*
 * 功能描述: +CMGS命令的打断处理
 *
 */
VOS_UINT32 AT_AbortCmgsPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 特性是否打开 */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->smsAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* +CMGS TEXT/PDU短信发送处理支持可中断 */
    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGS_TEXT_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGS_PDU_SET)) {
        result = MSG_SendAppReq(MN_MSG_MSGTYPE_ABORT_RPDATA_DIRECT, g_atClientTab[indexNum].clientId,
            g_atClientTab[indexNum].opId, VOS_NULL_PTR);
        if (result == MN_ERR_NO_ERROR) {
            return AT_ABORT;
        }
    }

    return AT_FAILURE;
}
#endif

VOS_VOID AT_GetMbbUsrDialPdpType(TAF_IFACE_DialParam *usrDialParam)
{
    VOS_UINT8           ipv6Capability;

    ipv6Capability = AT_GetIpv6Capability();
    if ((ipv6Capability == AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP) ||
        (ipv6Capability == AT_IPV6_CAPABILITY_IPV4V6_OVER_TWO_PDP)) {
        usrDialParam->pdpType = TAF_PDP_IPV4V6;
    } else if (ipv6Capability == AT_IPV6_CAPABILITY_IPV6_ONLY) {
        usrDialParam->pdpType = TAF_PDP_IPV6;
    } else {
        usrDialParam->pdpType = TAF_PDP_IPV4;
    }
}

VOS_VOID AT_ReadMbbFeatureNv(VOS_VOID)
{
    TAF_NVIM_MbbFeatureCfg    readMbbFeatureCfg;
    AT_ModemMbbCustmizeCtx   *featureCfgInfo = VOS_NULL_PTR;
#if (FEATURE_MBB_CUST == FEATURE_ON)
    VOS_UINT32                result;
#endif
    ModemIdUint16             modemId;

    memset_s(&readMbbFeatureCfg, sizeof(TAF_NVIM_MbbFeatureCfg), 0x00, sizeof(TAF_NVIM_MbbFeatureCfg));

#if (FEATURE_MBB_CUST == FEATURE_ON)
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MBB_FEATURE_CFG, &readMbbFeatureCfg, sizeof(TAF_NVIM_MbbFeatureCfg));

    if (result != NV_OK) {
        AT_ERR_LOG("AT_ReadMbbFeatureNv: read NV_ITEM_MBB_FEATURE_CFG failed!");

        return;
    }
#endif

    for (modemId = 0; modemId < MODEM_ID_BUTT; modemId++) {
        featureCfgInfo = AT_GetModemMbbCustCtxAddrFromModemId(modemId);

        featureCfgInfo->voipApnKey = (readMbbFeatureCfg.voipApnKey == VOS_TRUE) ? VOS_TRUE : VOS_FALSE;
        featureCfgInfo->dialStickFlg = (readMbbFeatureCfg.dialStickFlg == VOS_TRUE) ? VOS_TRUE : VOS_FALSE;
        featureCfgInfo->dtmfCustFlg = (readMbbFeatureCfg.dmtfCustFlag == VOS_TRUE) ? VOS_TRUE : VOS_FALSE;
        featureCfgInfo->ssAbortEnable= (readMbbFeatureCfg.ssAbortEnable == VOS_TRUE) ? VOS_TRUE : VOS_FALSE;
        featureCfgInfo->smsAbortEnable= (readMbbFeatureCfg.smsAbortEnable == VOS_TRUE) ? VOS_TRUE : VOS_FALSE;
    }

    return;
}


VOS_VOID AT_ReadMbbCustomizeNv(VOS_VOID)
{
    AT_ReadMbbFeatureNv();
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
/*
 * 功能说明: ^LTEPROFILE命令对PdpType参数检查
 * 输出参数: output: 用于获取pdptype类型
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckPdpType(VOS_UINT8 *output)
{
    VOS_UINT8 pdpType;

    /* 参考cgdcont对PDPTYPE校验的实现 */
    pdpType = (VOS_UINT8)(g_atParaList[LTE_PROFILE_PDP_TYPE].paraValue + 1);

    /* 设置的类型与实际支持的类型进行比较 */
    if ((pdpType == TAF_PDP_IPV6) || (pdpType == TAF_PDP_IPV4V6)) {
#if (FEATURE_IPV6 == FEATURE_ON)
        if (AT_GetIpv6Capability() == AT_IPV6_CAPABILITY_IPV4_ONLY) {
            AT_ERR_LOG("AT_LteProfileParaCheckPdpType UE Capability is not support ipv6!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
#else
        AT_ERR_LOG("AT_LteProfileParaCheckPdpType UE is not support ipv6!");
        return AT_CME_INCORRECT_PARAMETERS;
#endif
    }

    *output = pdpType;
    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE命令对PdpType参数检查
 * 输出参数: nvAttachProfileInfo: 检查成功后需要设置ImsiPreFix的成员参数
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckImsiPreFix(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    /* 参数长度检查,imsiprefix length must in [5-10] */
    if ((g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen > TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT) ||
        (g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen < TAF_MIN_LTE_APN_IMSI_PREFIX_SUPPORT)) {
        AT_ERR_LOG("AT_LteProfileParaCheckImsiPreFix IMSI_PREFIX is len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串数字检测 */
    if (At_CheckNumString(g_atParaList[LTE_PROFILE_IMSIPREFIX].para, g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen) != AT_SUCCESS) {
        AT_ERR_LOG("AT_LteProfileParaCheckImsiPreFix At_CheckNumString return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置 imsiPrefix */
    nvAttachProfileInfo->imsiPrefixLen = (VOS_UINT8)g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen;
    (VOS_VOID)At_AsciiString2HexSimple(nvAttachProfileInfo->imsiPrefixBcd, g_atParaList[LTE_PROFILE_IMSIPREFIX].para,
        nvAttachProfileInfo->imsiPrefixLen);

    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE命令对PdpType参数检查
 * 输入参数: index: AT下发通道
 * 输出参数: nvAttachProfileInfo: 检查成功后需要设置APN的成员参数
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckApn(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo, VOS_UINT8 indexNum)
{
    errno_t memResult;

    /* 检查apn长度，32位以内或者非0 */
    if ((g_atParaList[4].paraLen > TAF_NVIM_MAX_APNRETRY_APN_LEN) || (g_atParaList[4].paraLen == 0)) {
        AT_ERR_LOG("AT_LteProfileParaCheckApn apn len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 检查apn格式 */
    if (AT_CheckApnFormat(g_atParaList[4].para, g_atParaList[4].paraLen,
        g_atClientTab[indexNum].clientId) != VOS_OK) {
        AT_ERR_LOG("AT_LteProfileParaCheckApn AT_CheckApnFormat return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置APN */
    nvAttachProfileInfo->apnLen = (VOS_UINT8)g_atParaList[4].paraLen;
    memResult = memcpy_s(nvAttachProfileInfo->apn, sizeof(nvAttachProfileInfo->apn), g_atParaList[4].para,
        nvAttachProfileInfo->apnLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->apn), nvAttachProfileInfo->apnLen);
    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE命令对PdpType参数检查
 * 输出参数: nvAttachProfileInfo: 检查成功后需要设置用户名、密码和鉴权类型
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckAuthInfo(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    errno_t memResult;

    /* 检查username和userpwd的长度 */
    if ((g_atParaList[5].paraLen > TAF_NVIM_MAX_APNRETRY_USERNAME_LEN) ||
        (g_atParaList[6].paraLen > TAF_NVIM_MAX_APNRETRY_PASSWORD_LEN)) {
        AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo USERNAME_LEN or USERPWD_LEN is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置USERNAME */
    nvAttachProfileInfo->userNameLen = (VOS_UINT8)g_atParaList[5].paraLen; /* param5 user len */
    if (nvAttachProfileInfo->userNameLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->userName, sizeof(nvAttachProfileInfo->userName),
            g_atParaList[5].para, nvAttachProfileInfo->userNameLen); /* param5 user save */
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->userName), nvAttachProfileInfo->userNameLen);
    }

    /* 设置PWD */
    nvAttachProfileInfo->pwdLen = (VOS_UINT8)g_atParaList[6].paraLen; /* param6 pwd len */
    if (nvAttachProfileInfo->pwdLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->pwd, sizeof(nvAttachProfileInfo->pwd),
            g_atParaList[6].para, nvAttachProfileInfo->pwdLen); /* param6 pwd save */
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->pwd), nvAttachProfileInfo->pwdLen);
    }

    /* 检查authType不为空,值在TAF_PDP_AUTH_TYPE_BUTT之内 */
    if ((g_atParaList[7].paraLen == 0) || (g_atParaList[7].paraValue >= TAF_PDP_AUTH_TYPE_BUTT)) {
        AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo AUTH_TYPE is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置 AuthType */
    nvAttachProfileInfo->authType = (VOS_UINT8)g_atParaList[7].paraValue; /* param7 authType */
    /* 如果鉴权类型不为空，但是用户名密码其中有一为空则返回参数错误 */
    if (nvAttachProfileInfo->authType != TAF_PDP_AUTH_TYPE_NONE) {
        if ((nvAttachProfileInfo->userNameLen == 0) || (nvAttachProfileInfo->pwdLen == 0)) {
            AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo AUTH_TYPE is not null but username or pwd len is 0!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE命令对PdpType参数检查
 * 输出参数: nvAttachProfileInfo: 检查成功后需要设置用户名、密码和鉴权类型
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckProfileName(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    errno_t memResult;

    /* 检查profileName不为空 */
    if (g_atParaList[8].paraLen == 0) {
        AT_ERR_LOG("AT_LteProfileParaCheckProfileName profileName len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 完成对应字符串转换 */
    if (At_AsciiNum2HexString(g_atParaList[8].para, &g_atParaList[8].paraLen) != AT_SUCCESS) {
        AT_ERR_LOG("AT_LteProfileParaCheckProfileName At_AsciiNum2HexString return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串大于18字节则截断 */
    if (g_atParaList[8].paraLen >= TAF_NVIM_MAX_APNRETRY_PRONAME_LEN) {
        g_atParaList[8].paraLen = TAF_NVIM_MAX_APNRETRY_PRONAME_LEN - 1;
    }

    /* 设置ProfileName */
    nvAttachProfileInfo->profileNameLen = (VOS_UINT8)g_atParaList[8].paraLen;
    if (nvAttachProfileInfo->profileNameLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->profileName, sizeof(nvAttachProfileInfo->profileName),
            g_atParaList[8].para, nvAttachProfileInfo->profileNameLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->profileName), nvAttachProfileInfo->profileNameLen);
    }
    return AT_SUCCESS;
}

/*
 * 功能说明: APN info字段填充
 * 输入参数: index: AT下发通道
 * 输出参数: apnInfo: 用于获取apn信息
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果:   执行失败
 */
LOCAL VOS_UINT32 AT_LteProfileParaSet(VOS_UINT8 indexNum, TAF_PS_CustomerAttachApnInfo *proInfo)
{
    /* 校验与填充参数3，IpType */
    /* IP:1, IPV6:2, IPV4V6:3, 第三参数实现需要确认 */
    if (AT_LteProfileParaCheckPdpType(&proInfo->nvAttachProfileInfo.pdpType) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckPdpType return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 校验与填充参数4，ImsiPrefix */
    if (AT_LteProfileParaCheckImsiPreFix(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckImsiPreFix return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 校验与填充参数5，Apn 信息 */
    if (AT_LteProfileParaCheckApn(&proInfo->nvAttachProfileInfo, indexNum) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckApn return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 校验与填充参数6、7、8，auth鉴权参数用户名、密码、鉴权类型 */
    if (AT_LteProfileParaCheckAuthInfo(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckAuthInfo return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 校验与填充参数9，ProfileName */
    if (AT_LteProfileParaCheckProfileName(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckProfileName return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }
    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE命令参数检查
 * 输出参数: index: AT通道号
 *           cmdReadFlg: 用于获取读取标签
 *           apnInfo: 用于获取apn信息
 * 返回结果: AT_SUCCESS: 执行成功
 *           其他结果: 执行失败
 */
LOCAL VOS_UINT32 AT_SetLteProfileParaCheckParameter(VOS_UINT8 indexNum, VOS_UINT8 *cmdReadFlg,
    TAF_PS_CustomerAttachApnInfo *proInfo)
{
    /* 端口形态检查，APP端口可以正常读写 */
    if ((g_atDataLocked == VOS_TRUE) && (indexNum != AT_CLIENT_ID_APP) && (indexNum != AT_CLIENT_ID_APP1)) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter the port is not allow!");
        return AT_ERROR;
    }

    /* 类型和index 参数携带校验 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter the indexNum is not support!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    proInfo->exInfo.index = (VOS_UINT16)g_atParaList[1].paraValue;

    /* 读取与设置类型的判断与填充 */
    if (g_atParaList[0].paraValue == 0) {
        *cmdReadFlg = VOS_TRUE;
        /* 读取命令时大于2个参数就返回参数错误 */
        if (g_atParaIndex > 2) {
            AT_ERR_LOG("At_SetLteProfileParaCheckParameter read cmd para is too much!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
        return AT_SUCCESS;
    } else {
        /* 默认仅允许设置序号为0的定制APN */
        if (proInfo->exInfo.index != 0) {
            AT_ERR_LOG("At_SetLteProfileParaCheckParameter usIndex is not 0, this indexNum is not support!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        *cmdReadFlg = VOS_FALSE;
        proInfo->nvAttachProfileInfo.activeFlag = VOS_TRUE;

        /* 只有2个参数的设置命令是用来删除参数的 */
        if (g_atParaIndex == 2) {
            /* 激活状态设置为FALSE */
            proInfo->nvAttachProfileInfo.activeFlag = VOS_FALSE;
            return AT_SUCCESS;
        }

        /* 校验与填充参数3~9 */
        if (AT_LteProfileParaSet(indexNum, proInfo) != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_SUCCESS;
}

/*
 * 功能说明: ^LTEPROFILE设置命令处理
 * 输出参数: index: AT通道号
 * 返回结果: AT_WAIT_ASYNC_RETURN: 异步等待状态
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_SetCustProfilePara(VOS_UINT8 indexNum)
{
    TAF_PS_CustomerAttachApnInfo        setCustomAttachApn;
    VOS_UINT32                          result;
    VOS_UINT8                           cmdReadFlg = VOS_FALSE;

    (VOS_VOID)memset_s(&setCustomAttachApn, sizeof(setCustomAttachApn), 0x00, sizeof(setCustomAttachApn));

    /* 参数检查 */
    result = AT_SetLteProfileParaCheckParameter(indexNum, &cmdReadFlg, &setCustomAttachApn);
    if (result != AT_SUCCESS) {
        return result;
    }

    /* 读取命令参数填充 */
    if (cmdReadFlg == VOS_TRUE) {
        result = TAF_PS_GetCustomAttachProfile(WUEPS_PID_AT,
            AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), g_atClientTab[indexNum].opId,
            &(setCustomAttachApn.exInfo));
    } else {
        /* 设置命令参数填充 */
        result = TAF_PS_SetCustomAttachProfile(WUEPS_PID_AT,
            AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), g_atClientTab[indexNum].opId,
            &setCustomAttachApn);
    }

    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEPROFILE_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

/*
 * 功能说明: ^LTEPROFILE测试命令处理
 * 输出参数: index: AT通道号
 * 返回结果: AT_WAIT_ASYNC_RETURN: 异步等待状态
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_TestCustProfilePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = TAF_PS_GetCustomAttachProfileCount(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId),
        g_atClientTab[indexNum].opId);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEPROFILE_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

/*
 * 功能说明: ^LTEAPNATTACH设置命令处理
 * 输入参数: index: AT通道号
 * 返回结果: AT_WAIT_ASYNC_RETURN: 异步等待
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_SetCustProfileAttachSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32                          result;
    VOS_UINT32                          switchFlag;

    /* 参数只能为0和1 */
    if ((g_atParaIndex > 1) || (g_atParaList[0].paraLen != 1) || (g_atParaList[0].paraValue > 1)) {
        AT_ERR_LOG("AT_SetCustProfileAttachSwitch Para check ERROR !");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switchFlag = g_atParaList[0].paraValue;

    result = TAF_PS_SetAttachProfileSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId),
        g_atClientTab[indexNum].opId, switchFlag);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEAPNATTACH_SWITCH;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

/*
 * 功能说明: ^LTEAPNATTACH查询命令处理
 * 输入参数: index: AT通道号
 * 返回结果: AT_WAIT_ASYNC_RETURN: 异步等待
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_QryCustProfileAttachSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = TAF_PS_GetAttachProfileSwitchStatus(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId),
        g_atClientTab[indexNum].opId);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LTEAPNATTACH_SWITCH;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

/*
 * 功能说明: 增加打印Profile信息
 * 输出参数: curLen: 目前打印字符串的长度
 *           eventCnf: 事件信息
 * 返回结果: 增加Profile信息打印后的字符串长度
 */
LOCAL VOS_UINT16 AT_PrintAttachProfileInfo(VOS_UINT16 curLen, TAF_PS_GetCustAttachProfileInfoCnf *eventCnf)
{
    errno_t                             memResult;
    VOS_UINT16                          length;
    VOS_UINT8                           tempBuffer[TAF_NVIM_MAX_APNRETRY_PRONAME_LEN * 2 + 1] = {0};

    length = curLen;

    /* 填充APN */
    if (eventCnf->custAttachApn.nvAttachProfileInfo.apnLen > 0) {
        memResult = memcpy_s(tempBuffer, sizeof(tempBuffer), eventCnf->custAttachApn.nvAttachProfileInfo.apn,
            TAF_NVIM_MAX_APNRETRY_APN_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempBuffer), TAF_NVIM_MAX_APNRETRY_APN_LEN);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", tempBuffer);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",");
    }

    /* 填充User Name */
    if (eventCnf->custAttachApn.nvAttachProfileInfo.userNameLen > 0) {
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        memResult = memcpy_s(tempBuffer, sizeof(tempBuffer), eventCnf->custAttachApn.nvAttachProfileInfo.userName,
            TAF_NVIM_MAX_APNRETRY_USERNAME_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempBuffer), TAF_NVIM_MAX_APNRETRY_USERNAME_LEN);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", tempBuffer);
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",");
    }

    if (eventCnf->custAttachApn.nvAttachProfileInfo.pwdLen > 0) {
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        memResult = memcpy_s(tempBuffer, sizeof(tempBuffer), eventCnf->custAttachApn.nvAttachProfileInfo.pwd,
            TAF_NVIM_MAX_APNRETRY_PASSWORD_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempBuffer), TAF_NVIM_MAX_APNRETRY_PASSWORD_LEN);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", tempBuffer);

        /* 敏感信息使用完毕后重置 */
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        (VOS_VOID)memset_s(eventCnf->custAttachApn.nvAttachProfileInfo.pwd,
            sizeof(eventCnf->custAttachApn.nvAttachProfileInfo.pwd), 0x00,
            sizeof(eventCnf->custAttachApn.nvAttachProfileInfo.pwd));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",");
    }

    /* 填充Auth Type */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,", eventCnf->custAttachApn.nvAttachProfileInfo.authType);

    /* 填充Profile Name */
    if (eventCnf->custAttachApn.nvAttachProfileInfo.profileNameLen > 0) {
        /* HEX转String */
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        (VOS_VOID)AT_HexToAsciiString(tempBuffer, TAF_NVIM_MAX_APNRETRY_PRONAME_LEN,
            eventCnf->custAttachApn.nvAttachProfileInfo.profileName, eventCnf->custAttachApn.nvAttachProfileInfo.profileNameLen);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", tempBuffer);
    }
    return length;
}

/*
 * 功能说明: 处理^LTEPROFILE=1,x,...设置APN定制信息命令返回
 * 输出参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtSetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetCustAttachProfileInfoCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                          result = AT_ERROR;
    eventCnf = (TAF_PS_SetCustAttachProfileInfoCnf*)evtInfo;

    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEPROFILE_SET) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        result = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * 功能说明: 处理^LTEPROFILE=0,x,...获取APN定制信息命令返回
 * 输出参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetCustAttachProfileInfoCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                          result = AT_ERROR;
    VOS_UINT16                          length = 0;
    VOS_UINT8                           imsiPrefixBufLen = 0;
    VOS_UINT8                           tempBuffer[TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT + 1] = {0};

    eventCnf = (TAF_PS_GetCustAttachProfileInfoCnf *)evtInfo;
    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEPROFILE_SET) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        /* 填充AT命令头 */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName,
            eventCnf->custAttachApn.exInfo.index);

        /* 填充IP类型 */
        if (eventCnf->custAttachApn.nvAttachProfileInfo.pdpType == TAF_PDP_IPV4V6) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", "IPV4V6");
        } else if (eventCnf->custAttachApn.nvAttachProfileInfo.pdpType == TAF_PDP_IPV6) {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", "IPV6");
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", "IP");
        }

        /* 填充IMSI */
        if (eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixLen > 0) {
            eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixLen =
                AT_MIN(eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixLen,
                TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT);

            imsiPrefixBufLen = (eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixLen + 1) >> 1;
            (VOS_VOID)AT_HexToAsciiString(tempBuffer, TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT,
                eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixBcd, imsiPrefixBufLen);
            tempBuffer[eventCnf->custAttachApn.nvAttachProfileInfo.imsiPrefixLen] = '\0';
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"%s\",", tempBuffer);
        } else {
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",");
        }

        /* 填充Profile信息 */
        length = AT_PrintAttachProfileInfo(length, eventCnf);
        result = AT_OK;
    }

    /* 停止AT保护定时器 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* 输出打印内容 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * 功能说明: 处理^LTEPROFILE=?命令异步返回
 * 输出参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCountCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetCustAttachProfileCountCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                           result = AT_ERROR;
    VOS_UINT16                           length = 0;

    eventCnf = (TAF_PS_GetCustAttachProfileCountCnf*)evtInfo;
    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEPROFILE_SET) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: (0,1),(0-%d),10,32,32,32,(0-3)",
            g_parseContext[indexNum].cmdElement->cmdName, eventCnf->maxCount);
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * 功能说明: 处理LTEATTACHAPN主动上报
 * 输入参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtSuccAttachProfileIndexInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SuccAttachProfileIndexInd   *eventInd = VOS_NULL_PTR;
    ModemIdUint16                       modemId = MODEM_ID_0;
    VOS_UINT16                          length = 0;

    eventInd = (TAF_PS_SuccAttachProfileIndexInd*)evtInfo;
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafPsEvtSuccAttachProfileIndexInd: Get modem id fail.");
        return VOS_ERR;
    }

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s^LTEPROFILE: %d%s", g_atCrLf, eventInd->succProIdx, g_atCrLf);
    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    return VOS_OK;
}

/*
 * 功能说明: 处理^LTEAPNATTACH=x,...设置轮训开关命令返回
 * 输入参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtSetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetAttachProfileSwitchStatusCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                              result = AT_ERROR;

    eventCnf = (TAF_PS_SetAttachProfileSwitchStatusCnf*)evtInfo;
    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEAPNATTACH_SWITCH) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        result = AT_OK;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * 功能说明: 处理^LTEAPNATTACH?...查询轮训开关和轮训状态命令返回
 * 输入参数: index: AT通道号
 *           eventInfo: 事件信息
 * 返回结果: AT_OK: 执行成功
 *           AT_ERROR: 执行失败
 */
VOS_UINT32 AT_RcvTafPsEvtGetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetAttachProfileSwitchStatusCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                              result = AT_ERROR;
    VOS_UINT16                              length = 0;

    eventCnf = (TAF_PS_GetAttachProfileSwitchStatusCnf*)evtInfo;
    /* 检查当前命令的操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEAPNATTACH_SWITCH) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
            eventCnf->switchFlag, eventCnf->status);
        result = AT_OK;
    }

    g_atSendDataBuff.bufLen = length;
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}
#endif


