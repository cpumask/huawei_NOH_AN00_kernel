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
 * ��������: +CCWA����Ĵ�ϴ���
 *
 */
VOS_UINT32 AT_AbortCcwaPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* �����Ƿ�� */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->ssAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    clientOperationId.clientId = g_atClientTab[indexNum].clientId;
    clientOperationId.opId = 0;
    clientOperationId.reserve = 0;

    /* +CCWA����/�ر�/��ѯ������֧�ֿ��ж� */
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
 * ��������: +CLIP����Ĵ�ϴ���
 *
 */
VOS_UINT32 AT_AbortClipPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* �����Ƿ�� */
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
 * ��������: +CLIR����Ĵ�ϴ���
 *
 */
VOS_UINT32 AT_AbortClirPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    MN_CLIENT_OperationId clientOperationId;

    /* �����Ƿ�� */
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
 * ��������: +CMGS����Ĵ�ϴ���
 *
 */
VOS_UINT32 AT_AbortCmgsPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* �����Ƿ�� */
    if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->smsAbortEnable == VOS_FALSE) {
        return AT_FAILURE;
    }

    /* +CMGS TEXT/PDU���ŷ��ʹ���֧�ֿ��ж� */
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
 * ����˵��: ^LTEPROFILE�����PdpType�������
 * �������: output: ���ڻ�ȡpdptype����
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckPdpType(VOS_UINT8 *output)
{
    VOS_UINT8 pdpType;

    /* �ο�cgdcont��PDPTYPEУ���ʵ�� */
    pdpType = (VOS_UINT8)(g_atParaList[LTE_PROFILE_PDP_TYPE].paraValue + 1);

    /* ���õ�������ʵ��֧�ֵ����ͽ��бȽ� */
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
 * ����˵��: ^LTEPROFILE�����PdpType�������
 * �������: nvAttachProfileInfo: ���ɹ�����Ҫ����ImsiPreFix�ĳ�Ա����
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckImsiPreFix(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    /* �������ȼ��,imsiprefix length must in [5-10] */
    if ((g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen > TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT) ||
        (g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen < TAF_MIN_LTE_APN_IMSI_PREFIX_SUPPORT)) {
        AT_ERR_LOG("AT_LteProfileParaCheckImsiPreFix IMSI_PREFIX is len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �ַ������ּ�� */
    if (At_CheckNumString(g_atParaList[LTE_PROFILE_IMSIPREFIX].para, g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen) != AT_SUCCESS) {
        AT_ERR_LOG("AT_LteProfileParaCheckImsiPreFix At_CheckNumString return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���� imsiPrefix */
    nvAttachProfileInfo->imsiPrefixLen = (VOS_UINT8)g_atParaList[LTE_PROFILE_IMSIPREFIX].paraLen;
    (VOS_VOID)At_AsciiString2HexSimple(nvAttachProfileInfo->imsiPrefixBcd, g_atParaList[LTE_PROFILE_IMSIPREFIX].para,
        nvAttachProfileInfo->imsiPrefixLen);

    return AT_SUCCESS;
}

/*
 * ����˵��: ^LTEPROFILE�����PdpType�������
 * �������: index: AT�·�ͨ��
 * �������: nvAttachProfileInfo: ���ɹ�����Ҫ����APN�ĳ�Ա����
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckApn(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo, VOS_UINT8 indexNum)
{
    errno_t memResult;

    /* ���apn���ȣ�32λ���ڻ��߷�0 */
    if ((g_atParaList[4].paraLen > TAF_NVIM_MAX_APNRETRY_APN_LEN) || (g_atParaList[4].paraLen == 0)) {
        AT_ERR_LOG("AT_LteProfileParaCheckApn apn len is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���apn��ʽ */
    if (AT_CheckApnFormat(g_atParaList[4].para, g_atParaList[4].paraLen,
        g_atClientTab[indexNum].clientId) != VOS_OK) {
        AT_ERR_LOG("AT_LteProfileParaCheckApn AT_CheckApnFormat return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����APN */
    nvAttachProfileInfo->apnLen = (VOS_UINT8)g_atParaList[4].paraLen;
    memResult = memcpy_s(nvAttachProfileInfo->apn, sizeof(nvAttachProfileInfo->apn), g_atParaList[4].para,
        nvAttachProfileInfo->apnLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->apn), nvAttachProfileInfo->apnLen);
    return AT_SUCCESS;
}

/*
 * ����˵��: ^LTEPROFILE�����PdpType�������
 * �������: nvAttachProfileInfo: ���ɹ�����Ҫ�����û���������ͼ�Ȩ����
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckAuthInfo(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    errno_t memResult;

    /* ���username��userpwd�ĳ��� */
    if ((g_atParaList[5].paraLen > TAF_NVIM_MAX_APNRETRY_USERNAME_LEN) ||
        (g_atParaList[6].paraLen > TAF_NVIM_MAX_APNRETRY_PASSWORD_LEN)) {
        AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo USERNAME_LEN or USERPWD_LEN is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����USERNAME */
    nvAttachProfileInfo->userNameLen = (VOS_UINT8)g_atParaList[5].paraLen; /* param5 user len */
    if (nvAttachProfileInfo->userNameLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->userName, sizeof(nvAttachProfileInfo->userName),
            g_atParaList[5].para, nvAttachProfileInfo->userNameLen); /* param5 user save */
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->userName), nvAttachProfileInfo->userNameLen);
    }

    /* ����PWD */
    nvAttachProfileInfo->pwdLen = (VOS_UINT8)g_atParaList[6].paraLen; /* param6 pwd len */
    if (nvAttachProfileInfo->pwdLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->pwd, sizeof(nvAttachProfileInfo->pwd),
            g_atParaList[6].para, nvAttachProfileInfo->pwdLen); /* param6 pwd save */
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->pwd), nvAttachProfileInfo->pwdLen);
    }

    /* ���authType��Ϊ��,ֵ��TAF_PDP_AUTH_TYPE_BUTT֮�� */
    if ((g_atParaList[7].paraLen == 0) || (g_atParaList[7].paraValue >= TAF_PDP_AUTH_TYPE_BUTT)) {
        AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo AUTH_TYPE is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���� AuthType */
    nvAttachProfileInfo->authType = (VOS_UINT8)g_atParaList[7].paraValue; /* param7 authType */
    /* �����Ȩ���Ͳ�Ϊ�գ������û�������������һΪ���򷵻ز������� */
    if (nvAttachProfileInfo->authType != TAF_PDP_AUTH_TYPE_NONE) {
        if ((nvAttachProfileInfo->userNameLen == 0) || (nvAttachProfileInfo->pwdLen == 0)) {
            AT_ERR_LOG("AT_LteProfileParaCheckAuthInfo AUTH_TYPE is not null but username or pwd len is 0!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    return AT_SUCCESS;
}

/*
 * ����˵��: ^LTEPROFILE�����PdpType�������
 * �������: nvAttachProfileInfo: ���ɹ�����Ҫ�����û���������ͼ�Ȩ����
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaCheckProfileName(TAF_NVIM_LteAttachProfileInfo *nvAttachProfileInfo)
{
    errno_t memResult;

    /* ���profileName��Ϊ�� */
    if (g_atParaList[8].paraLen == 0) {
        AT_ERR_LOG("AT_LteProfileParaCheckProfileName profileName len is 0!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ɶ�Ӧ�ַ���ת�� */
    if (At_AsciiNum2HexString(g_atParaList[8].para, &g_atParaList[8].paraLen) != AT_SUCCESS) {
        AT_ERR_LOG("AT_LteProfileParaCheckProfileName At_AsciiNum2HexString return error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �ַ�������18�ֽ���ض� */
    if (g_atParaList[8].paraLen >= TAF_NVIM_MAX_APNRETRY_PRONAME_LEN) {
        g_atParaList[8].paraLen = TAF_NVIM_MAX_APNRETRY_PRONAME_LEN - 1;
    }

    /* ����ProfileName */
    nvAttachProfileInfo->profileNameLen = (VOS_UINT8)g_atParaList[8].paraLen;
    if (nvAttachProfileInfo->profileNameLen > 0) {
        memResult = memcpy_s(nvAttachProfileInfo->profileName, sizeof(nvAttachProfileInfo->profileName),
            g_atParaList[8].para, nvAttachProfileInfo->profileNameLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(nvAttachProfileInfo->profileName), nvAttachProfileInfo->profileNameLen);
    }
    return AT_SUCCESS;
}

/*
 * ����˵��: APN info�ֶ����
 * �������: index: AT�·�ͨ��
 * �������: apnInfo: ���ڻ�ȡapn��Ϣ
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������:   ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_LteProfileParaSet(VOS_UINT8 indexNum, TAF_PS_CustomerAttachApnInfo *proInfo)
{
    /* У����������3��IpType */
    /* IP:1, IPV6:2, IPV4V6:3, ��������ʵ����Ҫȷ�� */
    if (AT_LteProfileParaCheckPdpType(&proInfo->nvAttachProfileInfo.pdpType) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckPdpType return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* У����������4��ImsiPrefix */
    if (AT_LteProfileParaCheckImsiPreFix(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckImsiPreFix return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* У����������5��Apn ��Ϣ */
    if (AT_LteProfileParaCheckApn(&proInfo->nvAttachProfileInfo, indexNum) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckApn return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* У����������6��7��8��auth��Ȩ�����û��������롢��Ȩ���� */
    if (AT_LteProfileParaCheckAuthInfo(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckAuthInfo return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* У����������9��ProfileName */
    if (AT_LteProfileParaCheckProfileName(&proInfo->nvAttachProfileInfo) != AT_SUCCESS) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter AT_LteProfileParaCheckProfileName return ERROR!");
        return AT_CME_INCORRECT_PARAMETERS;
    }
    return AT_SUCCESS;
}

/*
 * ����˵��: ^LTEPROFILE����������
 * �������: index: ATͨ����
 *           cmdReadFlg: ���ڻ�ȡ��ȡ��ǩ
 *           apnInfo: ���ڻ�ȡapn��Ϣ
 * ���ؽ��: AT_SUCCESS: ִ�гɹ�
 *           �������: ִ��ʧ��
 */
LOCAL VOS_UINT32 AT_SetLteProfileParaCheckParameter(VOS_UINT8 indexNum, VOS_UINT8 *cmdReadFlg,
    TAF_PS_CustomerAttachApnInfo *proInfo)
{
    /* �˿���̬��飬APP�˿ڿ���������д */
    if ((g_atDataLocked == VOS_TRUE) && (indexNum != AT_CLIENT_ID_APP) && (indexNum != AT_CLIENT_ID_APP1)) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter the port is not allow!");
        return AT_ERROR;
    }

    /* ���ͺ�index ����Я��У�� */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_ERR_LOG("At_SetLteProfileParaCheckParameter the indexNum is not support!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    proInfo->exInfo.index = (VOS_UINT16)g_atParaList[1].paraValue;

    /* ��ȡ���������͵��ж������ */
    if (g_atParaList[0].paraValue == 0) {
        *cmdReadFlg = VOS_TRUE;
        /* ��ȡ����ʱ����2�������ͷ��ز������� */
        if (g_atParaIndex > 2) {
            AT_ERR_LOG("At_SetLteProfileParaCheckParameter read cmd para is too much!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
        return AT_SUCCESS;
    } else {
        /* Ĭ�Ͻ������������Ϊ0�Ķ���APN */
        if (proInfo->exInfo.index != 0) {
            AT_ERR_LOG("At_SetLteProfileParaCheckParameter usIndex is not 0, this indexNum is not support!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        *cmdReadFlg = VOS_FALSE;
        proInfo->nvAttachProfileInfo.activeFlag = VOS_TRUE;

        /* ֻ��2����������������������ɾ�������� */
        if (g_atParaIndex == 2) {
            /* ����״̬����ΪFALSE */
            proInfo->nvAttachProfileInfo.activeFlag = VOS_FALSE;
            return AT_SUCCESS;
        }

        /* У����������3~9 */
        if (AT_LteProfileParaSet(indexNum, proInfo) != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_SUCCESS;
}

/*
 * ����˵��: ^LTEPROFILE���������
 * �������: index: ATͨ����
 * ���ؽ��: AT_WAIT_ASYNC_RETURN: �첽�ȴ�״̬
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_SetCustProfilePara(VOS_UINT8 indexNum)
{
    TAF_PS_CustomerAttachApnInfo        setCustomAttachApn;
    VOS_UINT32                          result;
    VOS_UINT8                           cmdReadFlg = VOS_FALSE;

    (VOS_VOID)memset_s(&setCustomAttachApn, sizeof(setCustomAttachApn), 0x00, sizeof(setCustomAttachApn));

    /* ������� */
    result = AT_SetLteProfileParaCheckParameter(indexNum, &cmdReadFlg, &setCustomAttachApn);
    if (result != AT_SUCCESS) {
        return result;
    }

    /* ��ȡ���������� */
    if (cmdReadFlg == VOS_TRUE) {
        result = TAF_PS_GetCustomAttachProfile(WUEPS_PID_AT,
            AT_PS_BuildExClientId(g_atClientTab[indexNum].clientId), g_atClientTab[indexNum].opId,
            &(setCustomAttachApn.exInfo));
    } else {
        /* �������������� */
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
 * ����˵��: ^LTEPROFILE���������
 * �������: index: ATͨ����
 * ���ؽ��: AT_WAIT_ASYNC_RETURN: �첽�ȴ�״̬
 *           AT_ERROR: ִ��ʧ��
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
 * ����˵��: ^LTEAPNATTACH���������
 * �������: index: ATͨ����
 * ���ؽ��: AT_WAIT_ASYNC_RETURN: �첽�ȴ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_SetCustProfileAttachSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32                          result;
    VOS_UINT32                          switchFlag;

    /* ����ֻ��Ϊ0��1 */
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
 * ����˵��: ^LTEAPNATTACH��ѯ�����
 * �������: index: ATͨ����
 * ���ؽ��: AT_WAIT_ASYNC_RETURN: �첽�ȴ�
 *           AT_ERROR: ִ��ʧ��
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
 * ����˵��: ���Ӵ�ӡProfile��Ϣ
 * �������: curLen: Ŀǰ��ӡ�ַ����ĳ���
 *           eventCnf: �¼���Ϣ
 * ���ؽ��: ����Profile��Ϣ��ӡ����ַ�������
 */
LOCAL VOS_UINT16 AT_PrintAttachProfileInfo(VOS_UINT16 curLen, TAF_PS_GetCustAttachProfileInfoCnf *eventCnf)
{
    errno_t                             memResult;
    VOS_UINT16                          length;
    VOS_UINT8                           tempBuffer[TAF_NVIM_MAX_APNRETRY_PRONAME_LEN * 2 + 1] = {0};

    length = curLen;

    /* ���APN */
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

    /* ���User Name */
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

        /* ������Ϣʹ����Ϻ����� */
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        (VOS_VOID)memset_s(eventCnf->custAttachApn.nvAttachProfileInfo.pwd,
            sizeof(eventCnf->custAttachApn.nvAttachProfileInfo.pwd), 0x00,
            sizeof(eventCnf->custAttachApn.nvAttachProfileInfo.pwd));
    } else {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "\"\",");
    }

    /* ���Auth Type */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%d,", eventCnf->custAttachApn.nvAttachProfileInfo.authType);

    /* ���Profile Name */
    if (eventCnf->custAttachApn.nvAttachProfileInfo.profileNameLen > 0) {
        /* HEXתString */
        (VOS_VOID)memset_s(tempBuffer, sizeof(tempBuffer), 0x00, sizeof(tempBuffer));
        (VOS_VOID)AT_HexToAsciiString(tempBuffer, TAF_NVIM_MAX_APNRETRY_PRONAME_LEN,
            eventCnf->custAttachApn.nvAttachProfileInfo.profileName, eventCnf->custAttachApn.nvAttachProfileInfo.profileNameLen);
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s", tempBuffer);
    }
    return length;
}

/*
 * ����˵��: ����^LTEPROFILE=1,x,...����APN������Ϣ�����
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_RcvTafPsEvtSetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetCustAttachProfileInfoCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                          result = AT_ERROR;
    eventCnf = (TAF_PS_SetCustAttachProfileInfoCnf*)evtInfo;

    /* ��鵱ǰ����Ĳ������� */
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
 * ����˵��: ����^LTEPROFILE=0,x,...��ȡAPN������Ϣ�����
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetCustAttachProfileInfoCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                          result = AT_ERROR;
    VOS_UINT16                          length = 0;
    VOS_UINT8                           imsiPrefixBufLen = 0;
    VOS_UINT8                           tempBuffer[TAF_MAX_LTE_APN_IMSI_PREFIX_SUPPORT + 1] = {0};

    eventCnf = (TAF_PS_GetCustAttachProfileInfoCnf *)evtInfo;
    /* ��鵱ǰ����Ĳ������� */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_LTEPROFILE_SET) {
        return VOS_ERR;
    }

    if (eventCnf->result == VOS_OK) {
        /* ���AT����ͷ */
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,", g_parseContext[indexNum].cmdElement->cmdName,
            eventCnf->custAttachApn.exInfo.index);

        /* ���IP���� */
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

        /* ���IMSI */
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

        /* ���Profile��Ϣ */
        length = AT_PrintAttachProfileInfo(length, eventCnf);
        result = AT_OK;
    }

    /* ֹͣAT������ʱ�� */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* �����ӡ���� */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);
    return VOS_OK;
}

/*
 * ����˵��: ����^LTEPROFILE=?�����첽����
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_RcvTafPsEvtGetCustomAttachProfileCountCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetCustAttachProfileCountCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                           result = AT_ERROR;
    VOS_UINT16                           length = 0;

    eventCnf = (TAF_PS_GetCustAttachProfileCountCnf*)evtInfo;
    /* ��鵱ǰ����Ĳ������� */
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
 * ����˵��: ����LTEATTACHAPN�����ϱ�
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
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
 * ����˵��: ����^LTEAPNATTACH=x,...������ѵ���������
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_RcvTafPsEvtSetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_SetAttachProfileSwitchStatusCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                              result = AT_ERROR;

    eventCnf = (TAF_PS_SetAttachProfileSwitchStatusCnf*)evtInfo;
    /* ��鵱ǰ����Ĳ������� */
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
 * ����˵��: ����^LTEAPNATTACH?...��ѯ��ѵ���غ���ѵ״̬�����
 * �������: index: ATͨ����
 *           eventInfo: �¼���Ϣ
 * ���ؽ��: AT_OK: ִ�гɹ�
 *           AT_ERROR: ִ��ʧ��
 */
VOS_UINT32 AT_RcvTafPsEvtGetAttachProfileSwitchStatusCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_GetAttachProfileSwitchStatusCnf *eventCnf = VOS_NULL_PTR;
    VOS_UINT32                              result = AT_ERROR;
    VOS_UINT16                              length = 0;

    eventCnf = (TAF_PS_GetAttachProfileSwitchStatusCnf*)evtInfo;
    /* ��鵱ǰ����Ĳ������� */
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


