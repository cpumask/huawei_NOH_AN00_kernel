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

#include "AtCmdCallProc.h"
#include "securec.h"

#include "AtSndMsg.h"
#include "ATCmdProc.h"
#include "taf_ccm_api.h"
#include "AtTafAgentInterface.h"
#include "app_vc_api.h"
#include "AtEventReport.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_CALL_PROC_C

#define AT_ECLSTART_PARA_MAX_NUM 4
#define AT_ECLSTART_ACTIVATION_TYPE 0
#define AT_ECLSTART_TYPE_OF_CALL 1
#define AT_ECLSTART_DIAL_NUM 2
#define AT_ECLSTART_OPRT_MADE 3

#define AT_ECLCFG_PARA_MAX_NUM 3
#define AT_ECLCFG_MODE 0
#define AT_ECLCFG_VOC_CONFIG 1
#define AT_ECLCFG_REDIAL_CONFIG 2

#define AT_ECLMSD_ECALL_MSD 0

#define AT_CBURSTDTMF_PARA_NUM 4
#define AT_CBURSTDTMF_CALL_ID 0
#define AT_CBURSTDTMF_DTMF_KEY 1
#define AT_CBURSTDTMF_ON_LENGTH 2
#define AT_CBURSTDTMF_OFF_LENGTH 3

#define AT_CCONTDTMF_DTMF_KEY_VALID_LEN 1
#define AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP 0
#define AT_CCONTDTMF_DTMF_KEY 2

#define AT_ECLMODE_ECALLFORCE_MODE 0
#define AT_ECLMODE_ECALLFORCE_MODE_MAX_VALUE 2

#define AT_REJCALL_PARA_NUM 2

#define AT_OUTPUT_MAX_LENGTH 2
#define AT_DIGIT_MAX_LENGTH 2
#define AT_ECLIMSCFG_MAX_PARA_NUM 2
#define AT_ECLIMSCFG_MODE_INDEX 0
#define AT_ECLIMSCFG_CONTENT_INDEX 1

#if (FEATURE_ECALL == FEATURE_ON)



LOCAL VOS_UINT32 AT_SetEclstartParaCheckArgs(VOS_UINT8 indexNum)
{
    /*  �ж�1: ��ѡ�����ĳ��Ȳ���Ϊ0 */
    if ((g_atParaList[AT_ECLSTART_ACTIVATION_TYPE].paraLen == 0) ||
        (g_atParaList[AT_ECLSTART_TYPE_OF_CALL].paraLen == 0)) {
        return AT_ERROR;
    }

    /* �ж�2: ������������, �ж�1�ѱ�֤��������>=2 */
    if (g_atParaIndex > AT_ECLSTART_PARA_MAX_NUM) {
        return AT_ERROR;
    }

    /* �ж�3:  AT^ECLSTART=1,1, ��������Ǵ�� */
    if ((g_atParaIndex == 3) && (g_atParaList[AT_ECLSTART_DIAL_NUM].paraLen == 0)) {
        return AT_ERROR;
    }

    /* �ж�4:  AT^ECLSTART=1,1,, ��������Ǵ�� */
    if ((g_atParaIndex == 4) && (g_atParaList[AT_ECLSTART_OPRT_MADE].paraLen == 0)) {
        return AT_ERROR;
    }

    /* �ж�5:  ��ǰecallͨ���� */
    if (AT_HaveEcallActive(indexNum, VOS_FALSE) == VOS_TRUE) {
        return AT_ERROR;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_SetEclstartPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl                            ctrl = {0};
    MN_CALL_OrigParam                   callOrigPara = {0};
    MN_CALL_CalledNum                   dialNumber = {0};
    APP_VC_SetOprtmodeReq               eclOprtModepara = {0};
    VOS_UINT32                          rst;
    ModemIdUint16                       modemId;
    errno_t                             memResult;

    if (AT_SetEclstartParaCheckArgs(indexNum) == AT_ERROR) {
        return AT_ERROR;
    }

    /* oprt_mode Ĭ��ΪPUSHģʽ */
    if ((g_atParaList[AT_ECLSTART_OPRT_MADE].paraLen == 0) ||
        (g_atParaList[AT_ECLSTART_OPRT_MADE].paraValue == APP_VC_ECALL_OPRT_PUSH)) {
        eclOprtModepara.ecallOpMode = APP_VC_ECALL_OPRT_PUSH;
    } else {
        eclOprtModepara.ecallOpMode = APP_VC_ECALL_OPRT_PULL;
    }

    /* ����VCģ�鱣�浱ǰ����ģʽ */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 APP_VC_MSG_SET_ECALL_OPRTMODE_REQ, (VOS_UINT8 *)&eclOprtModepara,
                                 sizeof(eclOprtModepara), I0_WUEPS_PID_VC);

    if (rst == TAF_FAILURE) {
        return AT_ERROR;
    }

    /* �����ź��� */
    if (g_atParaList[AT_ECLSTART_DIAL_NUM].paraLen > 0) {
        /* ��鲢ת���绰���� */
        if (AT_FillCalledNumPara(g_atParaList[AT_ECLSTART_DIAL_NUM].para,
                                 g_atParaList[AT_ECLSTART_DIAL_NUM].paraLen, &dialNumber) != VOS_OK) {
            AT_UpdateCallErrInfo(indexNum, TAF_CS_CAUSE_INVALID_PARAMETER, VOS_NULL_PTR);
            return AT_ERROR;
        }
    }

    /* ^ECLSTART=x,0  �������call */
    if (g_atParaList[AT_ECLSTART_TYPE_OF_CALL].paraValue == 0) {
        callOrigPara.callType = MN_CALL_TYPE_TEST;
        callOrigPara.serviceType = TAF_CALL_SERVICE_TYPE_TEST_ECALL;
        /* ֻ�в��Ժ��в��·��绰���룬�������в��·��绰���� */
        memResult = memcpy_s(&callOrigPara.dialNumber, sizeof(callOrigPara.dialNumber), &dialNumber, sizeof(dialNumber));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callOrigPara.dialNumber), sizeof(dialNumber));
    } else if (g_atParaList[AT_ECLSTART_TYPE_OF_CALL].paraValue == 1) {
        /* ^ECLSTART=0,1  �û��������call */
        if (g_atParaList[AT_ECLSTART_ACTIVATION_TYPE].paraValue == 0) {
            callOrigPara.callType = MN_CALL_TYPE_MIEC;
            callOrigPara.serviceType = TAF_CALL_SERVICE_TYPE_MIEC;
        }
        /* ^ECLSTART=1,1  �Զ��������call */
        else {
            callOrigPara.callType = MN_CALL_TYPE_AIEC;
            callOrigPara.serviceType = TAF_CALL_SERVICE_TYPE_AIEC;
        }
    } else {
        callOrigPara.callType = MN_CALL_TYPE_RECFGURATION;
        callOrigPara.serviceType = TAF_CALL_SERVICE_TYPE_RECFGURATION_ECALL;
        /* ֻ�в��Ժ��в��·��绰���룬�������в��·��绰���� */
        memResult = memcpy_s(&callOrigPara.dialNumber, sizeof(callOrigPara.dialNumber), &dialNumber, sizeof(dialNumber));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(callOrigPara.dialNumber), sizeof(dialNumber));
    }
    callOrigPara.voiceDomain = TAF_CALL_VOICE_DOMAIN_AUTO;
    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* AT��CCM���ͺ�����Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, &callOrigPara, ID_TAF_CCM_CALL_ORIG_REQ, sizeof(callOrigPara), modemId);
    if (rst != VOS_OK) {
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLSTART_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetEclstopPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    TAF_Ctrl          ctrl;
    MN_CALL_SupsParam callSupsPara;
    ModemIdUint16     modemId;

    /* ������������������ */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_ERROR;
    }

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&callSupsPara, sizeof(callSupsPara), 0x00, sizeof(callSupsPara));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    callSupsPara.callSupsCmd = MN_CALL_SUPS_CMD_REL_ECALL;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* AT��CCM���ͺ�����Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, &callSupsPara, ID_TAF_CCM_CALL_SUPS_CMD_REQ, sizeof(callSupsPara), modemId);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLSTOP_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetEclcfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32               rst;
    APP_VC_MsgSetEcallCfgReq eclcfgSetPara;

    /* �ж�һ: ��ѡ�����ĳ��Ȳ���Ϊ0 */
    if (g_atParaList[AT_ECLCFG_MODE].paraLen == 0) {
        return AT_ERROR;
    }

    /* �ж϶�: ������������, �ж�һ�Ѿ���֤��������>=1 */
    if (g_atParaIndex > AT_ECLCFG_PARA_MAX_NUM) {
        return AT_ERROR;
    }

    /* �ж�3:  AT^ECLCFG=0, ���� AT^ECLCFG=0,1, ��������Ǵ�� */
    if (((g_atParaIndex == AT_ECLCFG_PARA_MAX_NUM - 1) && (g_atParaList[AT_ECLCFG_VOC_CONFIG].paraLen == 0)) ||
        ((g_atParaIndex == AT_ECLCFG_PARA_MAX_NUM) && (g_atParaList[AT_ECLCFG_REDIAL_CONFIG].paraLen == 0))) {
        return AT_ERROR;
    }

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&eclcfgSetPara, sizeof(eclcfgSetPara), 0x00, sizeof(eclcfgSetPara));

    eclcfgSetPara.mode = (APP_VC_EcallMsdModeUint16)g_atParaList[AT_ECLCFG_MODE].paraValue;

    if (g_atParaList[AT_ECLCFG_VOC_CONFIG].paraLen == 0) {
        eclcfgSetPara.vocConfig = VOC_CONFIG_NO_CHANGE;
    } else {
        eclcfgSetPara.vocConfig = (APP_VC_EcallVocConfigUint16)g_atParaList[AT_ECLCFG_VOC_CONFIG].paraValue;
    }

    /* ȡ��ECALL�ز����ܣ���������������, Ŀǰֻ���·��ر����� */
    if ((g_atParaList[AT_ECLCFG_REDIAL_CONFIG].paraLen != 0) &&
        (g_atParaList[AT_ECLCFG_REDIAL_CONFIG].paraValue == VOS_TRUE)) {
        return AT_ERROR;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 APP_VC_MSG_SET_ECALL_CFG_REQ, (VOS_UINT8 *)&eclcfgSetPara, sizeof(eclcfgSetPara),
                                 I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetEclmsdPara(VOS_UINT8 indexNum)
{
    VOS_UINT32          rst;
    APP_VC_MsgSetMsdReq eclmsdPara;

    /*
     * ��������Ϊ1��
     * ��������Ϊ1ʱ���������Ȳ�����Ϊ0��
     * ��"AT+CELMSD="����£�g_atParaIndexΪ0
     */
    if (g_atParaIndex != 1) {
        return AT_ERROR;
    }

    /*
     * �������Ȳ���
     * ʮ�������ı��ַ���������ʮ���������ݱ��뷽ʽ���ַ�������Ϊ280���ֽڣ���ʾ����MSDЭ��Ҫ���140���ֽ�ԭʼ����
     * ���MSDЭ���ԭʼ���ݳ�2
     */
    if (((APP_VC_MSD_DATA_LEN * 2) != g_atParaList[AT_ECLMSD_ECALL_MSD].paraLen)) {
        return AT_ERROR;
    }

    if (At_AsciiString2HexSimple(eclmsdPara.msdData, g_atParaList[AT_ECLMSD_ECALL_MSD].para,
                                 APP_VC_MSD_DATA_LEN * 2) == AT_FAILURE) {
        return AT_ERROR;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, APP_VC_MSG_SET_MSD_REQ,
                                 (VOS_UINT8 *)&eclmsdPara, sizeof(eclmsdPara), I0_WUEPS_PID_VC);
    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLMSD_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}



VOS_UINT32 AT_QryEclcfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 APP_VC_MSG_QRY_ECALL_CFG_REQ, VOS_NULL, 0, I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLCFG_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryEclmsdPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, APP_VC_MSG_QRY_MSD_REQ,
                                 VOS_NULL, 0, I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLMSD_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TestEclstartPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1),(0,1,2),(0,1)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}


VOS_UINT32 AT_TestEclmsdPara(VOS_UINT8 indexNum)
{
    /*
     * ^eclmsd��֧�ֲ������
     * �ú���������eclmsd��������ʱ����ERROR��������"(MSD)"
     */
    return AT_ERROR;
}


VOS_UINT8 AT_IsEcallType(TAFAGENT_CALL_InfoParam callInfo)
{
    /* �����ǰ�ǽ������У���������IMS�ϻ�����CS�Ͼ���Ϊ��eCall���� */
    if ((callInfo.callType == MN_CALL_TYPE_EMERGENCY) ||
        (callInfo.callType == MN_CALL_TYPE_PSAP_ECALL) ||
        (callInfo.serviceType == TAF_CALL_SERVICE_TYPE_PSAP_ECALL) ||
        (callInfo.serviceType == TAF_CALL_SERVICE_TYPE_TEST_ECALL) ||
        (callInfo.serviceType == TAF_CALL_SERVICE_TYPE_RECFGURATION_ECALL) ||
        (callInfo.serviceType == TAF_CALL_SERVICE_TYPE_MIEC) ||
        (callInfo.serviceType == TAF_CALL_SERVICE_TYPE_AIEC)) {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}

VOS_UINT8 AT_IsHaveEcallExsit(TAFAGENT_CALL_InfoParam callInfos, VOS_UINT8 checkFlag)
{
    if (AT_IsEcallType(callInfos) == VOS_TRUE) {
        if (checkFlag == VOS_TRUE) {
            if (callInfos.callState == MN_CALL_S_ACTIVE) {
                return VOS_TRUE;
            }
        } else {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT8 AT_HaveEcallActive(VOS_UINT8 indexNum, VOS_UINT8 checkFlag)
{
    VOS_UINT8  numOfCalls         = 0;
    VOS_UINT32 i                  = 0;
    VOS_UINT8  numOfCallsSmallNum = 0;
    VOS_UINT32 rst;
    TAFAGENT_CALL_InfoParam callInfos[MN_CALL_MAX_NUM];

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG("AT_HaveEcallActive : ucIndex.");

        return VOS_FALSE;
    }
    (VOS_VOID)memset_s(callInfos, sizeof(callInfos), 0x00, sizeof(callInfos));

    /* ��ͬ��API��ȡͨ����Ϣ */
    rst = TAF_AGENT_GetCallInfoReq(g_atClientTab[indexNum].clientId, &numOfCalls, callInfos);

    if (rst != VOS_OK) {
        AT_ERR_LOG("AT_HaveEcallActive : TAF_AGENT_GetCallInfoReq.");

        return VOS_FALSE;
    }

    AT_NORM_LOG1("AT_HaveEcallActive : [ucCheckFlag]", checkFlag);

    numOfCallsSmallNum = TAF_MIN(numOfCalls, MN_CALL_MAX_NUM);
    /* ��ǰ��ECALL ͨ������TRUE */
    for (i = 0; i < numOfCallsSmallNum; i++) {
        if (AT_IsHaveEcallExsit(callInfos[i], checkFlag) == VOS_TRUE) {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 AT_SetEclpushPara(VOS_UINT8 indexNum)
{
    VOS_UINT32          rst;
    APP_VC_MsgSetMsdReq eclmsdPara;

    /* ������������������ */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_ERROR;
    }

    /* ��ǰ��Ecall����״̬����ERROR */
    if (AT_HaveEcallActive(indexNum, VOS_TRUE) == VOS_FALSE) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&eclmsdPara, sizeof(APP_VC_MsgSetMsdReq), 0x00, sizeof(APP_VC_MsgSetMsdReq));
    /* ��VC����APP_VC_MSG_ECALL_PUSH_REQ���� */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 APP_VC_MSG_SET_ECALL_PUSH_REQ, (VOS_UINT8 *)&eclmsdPara, sizeof(eclmsdPara),
                                 I0_WUEPS_PID_VC);

    if (rst == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLPUSH_SET;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetEclAbortPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;
    VOS_UINT32 abortReason = 0;

    /* ������������������ */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_ERROR;
    }

    /* ��ǰ��Ecall����״̬����ERROR */
    if (AT_HaveEcallActive(indexNum, VOS_TRUE) == VOS_FALSE) {
        return AT_ERROR;
    }

    /* ��VC����APP_VC_MSG_ECALL_ABORT_REQ���� */
    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 APP_VC_MSG_SET_ECALL_ABORT_REQ, (VOS_UINT8 *)&abortReason, sizeof(abortReason),
                                 I0_WUEPS_PID_VC);

    if (ret == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ABORT_SET;

        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_SetEclModePara(VOS_UINT8 indexNum)
{
    TAF_NVIM_CustomEcallCfg ecallCfg;
    VOS_UINT32              result;

    (VOS_VOID)memset_s(&ecallCfg, sizeof(ecallCfg), 0x00, sizeof(ecallCfg));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_SetEclModePara: ucCmdOptType Error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != 1) {
        AT_ERR_LOG("AT_SetEclModePara: num Error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* <mode>��ֵֻ֧������Ϊ0-2 */
    if (g_atParaList[AT_ECLMODE_ECALLFORCE_MODE].paraValue > AT_ECLMODE_ECALLFORCE_MODE_MAX_VALUE) {
        AT_ERR_LOG("AT_SetEclModePara: value Error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �ȶ�ȡNV��ֵ */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, &ecallCfg, sizeof(ecallCfg));

    /* NV��ȡʧ�ܻ�NVδ����ʱ������error */
    if (result != NV_OK) {
        AT_ERR_LOG("AT_SetEclModePara: NV read!");
        return AT_ERROR;
    }

    if (ecallCfg.ecallForceMode != g_atParaList[AT_ECLMODE_ECALLFORCE_MODE].paraValue) {
        ecallCfg.ecallForceMode = (VOS_UINT8)g_atParaList[AT_ECLMODE_ECALLFORCE_MODE].paraValue;
        /* д��NV��ֵ */
        result = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, (VOS_UINT8 *)&ecallCfg, sizeof(ecallCfg));

        if (result != NV_OK) {
            return AT_ERROR;
        }

        AT_SetEclModeValue(ecallCfg.ecallForceMode);
    }

    return AT_OK;
}

VOS_UINT32 AT_QryEclModePara(VOS_UINT8 indexNum)
{
    VOS_UINT16              length;
    TAF_NVIM_CustomEcallCfg ecallCfg;
    VOS_UINT32              result;

    (VOS_VOID)memset_s(&ecallCfg, sizeof(ecallCfg), 0x00, sizeof(ecallCfg));
    /* �ȶ�ȡNV��ֵ */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, &ecallCfg, sizeof(ecallCfg));

    /* NV��ȡʧ�ܻ�NVδ����ʱ�����ı�ģʽ */
    if (result != NV_OK) {
        AT_ERR_LOG("AT_QryEclModePara: NV read error!");
    } else {
        AT_SetEclModeValue(ecallCfg.ecallForceMode);
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, AT_GetEclModeValue());
    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}

VOS_UINT32 AT_TestEclModePara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %s", g_parseContext[indexNum].cmdElement->cmdName,
        g_parseContext[indexNum].cmdElement->param);
    return AT_OK;
}


VOS_VOID AT_EcallAlackDisplay(AT_ECALL_AlackValue ecallAlackInfo, VOS_UINT16 *length)
{
    TAF_INT8        timeZone = 0;
    TIME_ZONE_Time *timeInfo = VOS_NULL_PTR;

    timeInfo = &ecallAlackInfo.ecallAlackTimeInfo.universalTimeandLocalTimeZone;
    /* YYYY */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "\"%4d/",
        timeInfo->year + 2000); /* year */

    /* MM */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d%d/",
        timeInfo->month / AT_DECIMAL_BASE_NUM,  /* month high */
        timeInfo->month % AT_DECIMAL_BASE_NUM); /* month low */
    /* dd */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d%d,",
        timeInfo->day / AT_DECIMAL_BASE_NUM,  /* day high */
        timeInfo->day % AT_DECIMAL_BASE_NUM); /* day low */

    /* hh */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d%d:",
        timeInfo->hour / AT_DECIMAL_BASE_NUM,  /* hour high */
        timeInfo->hour % AT_DECIMAL_BASE_NUM); /* hour low */

    /* mm */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length,
        "%d%d:", timeInfo->minute / AT_DECIMAL_BASE_NUM, /* minutes high */
        timeInfo->minute % AT_DECIMAL_BASE_NUM);         /* minutes high */

    /* ss */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, "%d%d",
        timeInfo->second / AT_DECIMAL_BASE_NUM,  /* sec high */
        timeInfo->second % AT_DECIMAL_BASE_NUM); /* sec low */

    /* ���ʱ�� */
    if ((ecallAlackInfo.ecallAlackTimeInfo.ieFlg & NAS_MM_INFO_IE_LTZ) == NAS_MM_INFO_IE_LTZ) {
        timeZone = ecallAlackInfo.ecallAlackTimeInfo.localTimeZone;
    } else {
        timeZone = timeInfo->timeZone;
    }

    if (timeZone == AT_INVALID_TZ_VALUE) {
        timeZone = 0;
    }

    if (timeZone != AT_INVALID_TZ_VALUE) {
        *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s%02d\"",
            (timeZone < 0) ? "-" : "+", (timeZone < 0) ? (-timeZone) : timeZone);
    }

    /* AlackValue */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d", ecallAlackInfo.ecallAlackValue);
    return;
}


VOS_VOID AT_EcallAlAckListDisplay(VOS_VOID)
{
    VOS_UINT32          i                  = 0;
    VOS_UINT16          length             = 0;
    VOS_UINT32          readNum            = 0;
    AT_ECALL_AlackInfo *ecallAlackInfoAddr = VOS_NULL_PTR;

    ecallAlackInfoAddr = AT_EcallAlAckInfoAddr();

    /* ��ʾ����ALACK ֵ�ͽ���ʱ�� */
    for (i = 0; i < (VOS_UINT32)AT_MIN(ecallAlackInfoAddr->ecallAlackNum, AT_ECALL_ALACK_NUM); i++) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "^ECLLIST: ");

        readNum = (ecallAlackInfoAddr->ecallAlackBeginNum + i) % AT_ECALL_ALACK_NUM;
        AT_EcallAlackDisplay(ecallAlackInfoAddr->ecallAlackInfo[readNum], &length);

        if (i + 1 < ecallAlackInfoAddr->ecallAlackNum) {
            /* �س����� */
            length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = length;
    return;
}


VOS_UINT32 AT_QryEclListPara(VOS_UINT8 indexNum)
{
    AT_EcallAlAckListDisplay();

    return AT_OK;
}


VOS_UINT32 AT_RcvVcMsgEcallPushCnfProc(MN_AT_IndEvt *data)
{
    VOS_UINT8         indexNum = 0;
    VOS_UINT32        ret      = VOS_ERR;
    APP_VC_SetMsdCnf *rslt     = VOS_NULL_PTR;

    if (data == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* ͨ��clientid��ȡindex */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvVcMsgEcallPushCnfProc:WARNING:AT INDEX NOT FOUND!");

        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvVcMsgEcallPushCnfProc : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* ��ǰAT�Ƿ��ڵȴ�������� */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ECLPUSH_SET) {
        AT_WARN_LOG("AT_RcvVcMsgEcallPushCnfProc:WARNING:AT ARE WAITING ANOTHER CMD!");

        return VOS_ERR;
    }

    rslt = (APP_VC_SetMsdCnf *)data->content;

    if (rslt->rslt == VOS_OK) {
        ret = AT_OK;
    } else {
        ret = AT_ERROR;
    }

    g_atSendDataBuff.bufLen = 0;

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(indexNum);

    At_FormatResultData(indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_ProcVcEcallAbortCnf(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    VOS_UINT32 rslt = AT_ERROR;

    /* ������Ч���ж� */
    if (vcEvtInfo == VOS_NULL) {
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_ProcVcEcallAbortCnf : AT_BROADCAST_INDEX.");

        return VOS_ERR;
    }

    /* ��ǰAT�Ƿ��ڵȴ�������� */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_ABORT_SET) {
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


VOS_UINT32 AT_ProcVcReportEcallAlackEvent(VOS_UINT8 indexNum, APP_VC_EventInfo *vcEvtInfo)
{
    NAS_MM_InfoInd      localAtTimeInfo = {0};
    NAS_MM_InfoInd      cCLKTimeInfo = {0};
    ModemIdUint16       modemId            = MODEM_ID_0;
    AT_ECALL_AlackInfo *ecallAlackInfoAddr = VOS_NULL_PTR;
    VOS_UINT32          writeNum = 0;
    VOS_UINT16          length   = 0;
    AT_ModemNetCtx     *netCtx   = VOS_NULL_PTR;
    errno_t             memResult;

    if (vcEvtInfo == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_ProcVcReportEcallAlackEvent: Get modem id fail.");
        return VOS_ERR;
    }

    netCtx = AT_GetModemNetCtxAddrFromModemId(modemId);

    if ((netCtx->timeInfo.ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        /* ���ȸ����������CCLK��ʱ��ת��ΪlocalATʱ�� */
        AT_GetLiveTime(&netCtx->timeInfo, &localAtTimeInfo, netCtx->nwSecond);
    } else {
        memResult = memcpy_s(&cCLKTimeInfo, sizeof(cCLKTimeInfo), &netCtx->timeInfo, sizeof(NAS_MM_InfoInd));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cCLKTimeInfo), sizeof(NAS_MM_InfoInd));
        cCLKTimeInfo.ieFlg = NAS_MM_INFO_IE_UTLTZ;
        AT_GetLiveTime(&cCLKTimeInfo, &localAtTimeInfo, 1);
    }

    /* �洢ʱ���ALACK ֵ */
    ecallAlackInfoAddr = AT_EcallAlAckInfoAddr();
    if (ecallAlackInfoAddr->ecallAlackNum < AT_ECALL_ALACK_NUM) {
        ecallAlackInfoAddr->ecallAlackNum++;
        writeNum = (VOS_UINT32)ecallAlackInfoAddr->ecallAlackNum - 1;
    } else {
        ecallAlackInfoAddr->ecallAlackBeginNum += 1;
        ecallAlackInfoAddr->ecallAlackBeginNum %= AT_ECALL_ALACK_NUM;
        /*
         * EcallAlAckList��BeginNum����0ʱ��writeNum��ʾѭ������EcallAlAckList��ǰһ��������
         * Ӧ�õ���AT_ECALL_ALACK_NUM - 1
         */
        if (ecallAlackInfoAddr->ecallAlackBeginNum == 0) {
            writeNum = AT_ECALL_ALACK_NUM - 1;
        } else {
            writeNum = (VOS_UINT32)ecallAlackInfoAddr->ecallAlackBeginNum - 1;
        }
    }

    memResult = memcpy_s(&ecallAlackInfoAddr->ecallAlackInfo[writeNum].ecallAlackTimeInfo, sizeof(NAS_MM_InfoInd),
        &localAtTimeInfo, sizeof(localAtTimeInfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(NAS_MM_InfoInd), sizeof(localAtTimeInfo));
    ecallAlackInfoAddr->ecallAlackInfo[writeNum].ecallAlackValue = vcEvtInfo->ecallReportAlackValue;

    /* ��ʾʱ���ALACK ֵ */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ECLREC: ", g_atCrLf);
    AT_EcallAlackDisplay(ecallAlackInfoAddr->ecallAlackInfo[writeNum], &length);

    /* �س����� */
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);
    At_SendResultData(AT_CLIENT_ID_CTRL, g_atSndCodeAddress, length);
    return VOS_OK;
}


VOS_VOID AT_RcvTafEcallStatusErrorInd(VOS_VOID)
{
    VOS_UINT16 length;

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^ECLSTAT: 2,3%s", g_atCrLf, g_atCrLf);

    At_SendResultData(AT_BROADCAST_CLIENT_INDEX_MODEM_0, g_atSndCodeAddress, length);

    return;
}

VOS_UINT32 AT_ConvertContentTypeModeToEcallDomainMode(VOS_UINT8 mode)
{
    AT_NORM_LOG1("AT_ConvertContentTypeModeToEcallDomainMode: ContentTypeMode", mode);
    if (mode == AT_IMSA_ECALL_CONTENT_TYPE_MODE_CS_PREFER) {
        return TAF_CALL_ECALL_DOMAIN_CS_PREFER;
    } else if (mode == AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_CUSTOM) {
        return TAF_CALL_ECALL_DOMAIN_PS_PREFER;
    } else {
        return TAF_CALL_ECALL_DOMAIN_PS_PREFER;
    }
}

VOS_UINT32 AT_CheckEclImsCfgPara(VOS_VOID)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_ERR_LOG("AT_CheckEclImsCfgPara: cmdOptType Error!");
        return AT_ERROR;
    }

    /* �����������ܳ���2���ҵ�һ����������Ϊ�� */
    if ((g_atParaIndex > AT_ECLIMSCFG_MAX_PARA_NUM) || (g_atParaList[AT_ECLIMSCFG_MODE_INDEX].paraLen == 0)) {
        AT_ERR_LOG("AT_CheckEclImsCfgPara: num Error!");
        return AT_ERROR;
    }

    /* ecallContentTypeMode ��ֵֻ֧������Ϊ0-2 ��cs_prefer �� ps_prefer_custom �� ps_perfer_standard */
    if (g_atParaList[AT_ECLIMSCFG_MODE_INDEX].paraValue >= AT_IMSA_ECALL_CONTENT_TYPE_MODE_BUTT) {
        AT_ERR_LOG("AT_CheckEclImsCfgPara: value Error!");
        return AT_ERROR;
    }

    /* ����1 ecallContentTypeModeΪ1(ps_prefer_custom)ʱ������2 content type ����Ϊ�� */
    if ((g_atParaList[AT_ECLIMSCFG_MODE_INDEX].paraValue == AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_CUSTOM) &&
        (g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].paraLen == 0)) {
        AT_ERR_LOG("AT_SetEclImsCfgPara: value Error!");
        return AT_ERROR;
    }

    return AT_SUCCESS;
}

VOS_UINT32 AT_UpdateEcallDomainMode(VOS_UINT8 mode, ModemIdUint16 modemId, VOS_UINT8 indexNum)
{
    TAF_Ctrl                            ctrl;
    TAF_NVIM_CustomEcallCfg             ecallCfg;
    VOS_UINT32                          result;
    VOS_UINT32                          ecallDomainMode;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&ecallCfg, sizeof(ecallCfg), 0x00, sizeof(ecallCfg));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    /* �ȶ�ȡNV��ֵ */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, &ecallCfg, sizeof(TAF_NVIM_CustomEcallCfg));
    /* NV��ȡʧ�ܻ�NVδ����ʱ������error */
    if (result != NV_OK) {
        AT_ERR_LOG("AT_UpdateEcallDomainMode: NV read!");
        return AT_ERROR;
    }

    ecallDomainMode = AT_ConvertContentTypeModeToEcallDomainMode(mode);
    /* ����NV��ȫ�ֱ��� */
    if (ecallCfg.ecallDomainMode != ecallDomainMode) {
        ecallCfg.ecallDomainMode = (VOS_UINT8)ecallDomainMode;
        /* д��NV��ֵ */
        result = TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, (VOS_UINT8 *)&ecallCfg,
            sizeof(TAF_NVIM_CustomEcallCfg));

        if (result != NV_OK) {
            AT_ERR_LOG("AT_UpdateEcallDomainMode: NV write!");
            return AT_ERROR;
        }

        /* ����Ϣ��C��ˢ��ecall domain mode */
        result = TAF_CCM_CallCommonReq(&ctrl, &ecallDomainMode, ID_TAF_CCM_SET_ECALL_DOMAIN_MODE_REQ,
                                    sizeof(ecallDomainMode), modemId);
        if (result != TAF_SUCCESS) {
            AT_ERR_LOG("AT_UpdateEcallDomainMode: TAF_CCM_CallCommonReq FAIL");
            return AT_ERROR;
        }
    }
    return AT_SUCCESS;
}

VOS_UINT32 AT_FillEcallContentType(AT_IMSA_EcallContentTypeSetReq *ecallContentType)
{
    errno_t                             memValue;

    if (ecallContentType->mode == AT_IMSA_ECALL_CONTENT_TYPE_MODE_PS_PREFER_CUSTOM) {
        if ((g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].paraLen > AT_IMSA_CONTENT_TYPE_MAX_LEN) ||
            (g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].paraLen == 0)) {
            AT_ERR_LOG("AT_SetEclImsCfgPara: value Error!");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        memValue = memcpy_s(ecallContentType->context, sizeof(ecallContentType->context),
                            g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].para,
                            g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].paraLen);
        TAF_MEM_CHK_RTN_VAL(memValue, sizeof(ecallContentType->context),
                            g_atParaList[AT_ECLIMSCFG_CONTENT_INDEX].paraLen);
    }
    return AT_SUCCESS;
}

VOS_UINT32 AT_SetEclImsCfgPara(VOS_UINT8 indexNum)
{
    AT_IMSA_EcallContentTypeSetReq      ecallContentType = {0};
    VOS_UINT32                          rst;
    ModemIdUint16                       modemId;

    /* ������� */
    if (AT_CheckEclImsCfgPara() != AT_SUCCESS) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* �����õ�ֵ�͵�ǰֵ��һ��ʱ��ˢ��NV��ȫ�ֱ��� */
    ecallContentType.mode = (VOS_UINT8)g_atParaList[AT_ECLIMSCFG_MODE_INDEX].paraValue;
    if (AT_UpdateEcallDomainMode(ecallContentType.mode, modemId, indexNum) != AT_SUCCESS) {
        return AT_ERROR;
    }

    /* ��� Ecall contentType */
    if (AT_FillEcallContentType(&ecallContentType) != AT_SUCCESS) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ecallDomainMode ΪCS_PREFERʱ������Ҫ֪ͨIMSA */
    if (ecallContentType.mode == AT_IMSA_ECALL_CONTENT_TYPE_MODE_CS_PREFER) {
        return AT_OK;
    }
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
              ID_AT_IMSA_ECALL_ECONTENT_TYPE_SET_REQ, (VOS_UINT8 *)ecallContentType.context,
             (VOS_UINT32)(sizeof(ecallContentType.context) + sizeof(ecallContentType.mode) +
              sizeof(ecallContentType.reserved)),
              PS_PID_IMSA);
    if (rst != TAF_SUCCESS) {
        AT_ERR_LOG("AT_SetEclImsCfgPara: AT_FillAndSndAppReqMsg FAIL");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLIMSCFG_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryEclImsCfgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32                          result;
    TAF_NVIM_CustomEcallCfg             ecallCfg;

    (VOS_VOID)memset_s(&ecallCfg, sizeof(ecallCfg), 0x00, sizeof(ecallCfg));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* �ȶ�ȡNV��ֵ */
    result = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_CUSTOM_ECALL_CFG, &ecallCfg, sizeof(TAF_NVIM_CustomEcallCfg));
    /* NV��ȡʧ�ܻ�NVδ����ʱ������error */
    if (result != NV_OK) {
        AT_ERR_LOG("AT_QryEclImsCfgPara: NV read!");
        return AT_ERROR;
    }

    /* ecallDomainMode ΪCS_PREFERʱ������ҪȥIMSA��ѯ */
    if (ecallCfg.ecallDomainMode == TAF_CALL_ECALL_DOMAIN_CS_PREFER) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
                AT_IMSA_ECALL_CONTENT_TYPE_MODE_CS_PREFER);
        return AT_OK;
    }
    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                ID_AT_IMSA_ECALL_ECONTENT_TYPE_QRY_REQ, VOS_NULL_PTR,
                0, PS_PID_IMSA);

    if (result != TAF_SUCCESS) {
        AT_ERR_LOG("AT_QryEclImsCfgPara: AT_FillAndSndAppReqMsg FAIL");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ECLIMSCFG_QRY;
    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 At_RcvVcMsgDtmfDecoderIndProc(MN_AT_IndEvt *data)
{
    APP_VC_DtmfDecoderInd *dtmfInd  = VOS_NULL_PTR;
    VOS_UINT8              indexNum = 0;
    VOS_CHAR               output[AT_OUTPUT_MAX_LENGTH];

    /* ͨ��clientid��ȡindex */
    if (At_ClientIdToUserId(data->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("At_RcvVcMsgDtmfDecoderIndProc:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* ��ʼ�� */
    dtmfInd   = (APP_VC_DtmfDecoderInd *)data->content;
    output[0] = dtmfInd->ucDtmfCode;
    output[1] = '\0';

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^DDTMF: %s%s", g_atCrLf, output, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_CheckCfshNumber(VOS_UINT8 *atPara, VOS_UINT16 len)
{
    VOS_UINT32 loop;

    /* ���볤����Ч���ж�:+�ſ�ͷ�Ĺ��ʺ��룬��󳤶Ȳ��ܴ���33�������ܴ���32 */
    if (atPara[0] == '+') {
        if (len > (TAF_CALL_MAX_FLASH_DIGIT_LEN + 1)) {
            return VOS_ERR;
        }

        atPara++;
        len--;
    } else {
        if (len > TAF_CALL_MAX_FLASH_DIGIT_LEN) {
            return VOS_ERR;
        }
    }

    /* �����ַ���Ч���ж�(���������ʺ�������ַ�'+') */
    for (loop = 0; loop < len; loop++) {
        if (((atPara[loop] >= '0') && (atPara[loop] <= '9')) || (atPara[loop] == '*') || (atPara[loop] == '#')) {
            continue;
        } else {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetCfshPara(VOS_UINT8 indexNum)
{
    errno_t            memResult;
    VOS_UINT32         rst;
    TAF_Ctrl           ctrl;
    TAF_CALL_FlashPara flashPara;
    ModemIdUint16      modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&flashPara, sizeof(flashPara), 0x00, sizeof(flashPara));

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��Я���˲���<number>���������Ч�� */
    if (g_atParaIndex == 1) {
        if (AT_CheckCfshNumber(g_atParaList[0].para, g_atParaList[0].paraLen) != VOS_OK) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
        /* ����AT����AT^CFSH= ���ز������� */
        if (g_atParseCmd.cmdOptType == AT_CMD_OPT_SET_PARA_CMD) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    (VOS_VOID)memset_s(&flashPara, sizeof(flashPara), 0x00, sizeof(flashPara));

    flashPara.digitNum = (VOS_UINT8)g_atParaList[0].paraLen;
    if (g_atParaList[0].paraLen > 0) {
        memResult = memcpy_s(flashPara.digit, sizeof(flashPara.digit), g_atParaList[0].para, g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(flashPara.digit), g_atParaList[0].paraLen);
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* ����ID_TAF_CCM_SEND_FLASH_REQ��Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, &flashPara, ID_TAF_CCM_SEND_FLASH_REQ, sizeof(flashPara), modemId);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CFSH_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_RcvTafCcmSndFlashRslt(struct MsgCB *msg)
{
    VOS_UINT8             indexNum     = 0;
    TAF_CCM_SendFlashCnf *sndFlashRslt = VOS_NULL_PTR;

    sndFlashRslt = (TAF_CCM_SendFlashCnf *)msg;

    /* ����ClientID��ȡͨ������ */
    if (At_ClientIdToUserId(sndFlashRslt->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallSndFlashRslt: Get Index Fail!");
        return VOS_ERR;
    }

    /* ATģ���ڵȴ�^CFSH����Ĳ�������¼��ϱ� */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CFSH_SET) {
        AT_WARN_LOG("AT_RcvTafCallSndFlashRslt: Error Option!");
        return VOS_ERR;
    }

    /* ʹ��AT_STOP_TIMER_CMD_READY�ָ�AT����ʵ��״̬ΪREADY״̬ */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* ������ʱ��Ӧ�Ĵ������ӡ����Ľ�� */
    if (sndFlashRslt->result.result == VOS_OK) {
        At_FormatResultData(indexNum, AT_OK);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    return VOS_OK;
}


VOS_UINT32 At_TestCBurstDTMFPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length,
        "^CBURSTDTMF: (1-7),(0-9,*,#),(95,150,200,250,300,350),(60,100,150,200)");
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_SetCBurstDTMFPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;
    errno_t    memResult;
    VOS_UINT32 loop;
    VOS_UINT32 paraInvalidFlg;

    TAF_Ctrl               ctrl;
    TAF_CALL_BurstDtmfPara burstDTMFPara;
    ModemIdUint16          modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&burstDTMFPara, sizeof(burstDTMFPara), 0x00, sizeof(burstDTMFPara));

    /* ������Ч�Լ�� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    paraInvalidFlg = (g_atParaIndex != AT_CBURSTDTMF_PARA_NUM) ||
                     (g_atParaList[AT_CBURSTDTMF_CALL_ID].paraLen == 0) ||
                     (g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen == 0) ||
                     (g_atParaList[AT_CBURSTDTMF_ON_LENGTH].paraLen == 0) ||
                     (g_atParaList[AT_CBURSTDTMF_OFF_LENGTH].paraLen == 0);

    if (paraInvalidFlg == VOS_TRUE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* DTMF Key������Ч���ж� */
    if (g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen > TAF_CALL_MAX_BURST_DTMF_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* DTMF Key��Ч���ж� */
    for (loop = 0; loop < g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen; loop++) {
        if (((g_atParaList[AT_CBURSTDTMF_DTMF_KEY].para[loop] >= '0') &&
            (g_atParaList[AT_CBURSTDTMF_DTMF_KEY].para[loop] <= '9')) ||
            (g_atParaList[AT_CBURSTDTMF_DTMF_KEY].para[loop] == '*') ||
            (g_atParaList[AT_CBURSTDTMF_DTMF_KEY].para[loop] == '#')) {
            continue;
        } else {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    burstDTMFPara.callId   = (VOS_UINT8)g_atParaList[AT_CBURSTDTMF_CALL_ID].paraValue;
    burstDTMFPara.digitNum = (VOS_UINT8)g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen;

    memResult = memcpy_s(burstDTMFPara.digit, sizeof(burstDTMFPara.digit), g_atParaList[AT_CBURSTDTMF_DTMF_KEY].para,
                         g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(burstDTMFPara.digit), g_atParaList[AT_CBURSTDTMF_DTMF_KEY].paraLen);

    burstDTMFPara.onLength  = g_atParaList[AT_CBURSTDTMF_ON_LENGTH].paraValue;
    burstDTMFPara.offLength = g_atParaList[AT_CBURSTDTMF_OFF_LENGTH].paraValue;

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* ����ID_TAF_CCM_SEND_BURST_DTMF_REQ��Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, &burstDTMFPara, ID_TAF_CCM_SEND_BURST_DTMF_REQ, sizeof(burstDTMFPara), modemId);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CBURSTDTMF_SET;

        /* ������������״̬ */
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 AT_RcvTafCcmSndBurstDTMFCnf(struct MsgCB *msg)
{
    VOS_UINT8                 indexNum     = 0;
    TAF_CCM_SendBurstDtmfCnf *burstDtmfCnf = VOS_NULL_PTR;

    burstDtmfCnf = (TAF_CCM_SendBurstDtmfCnf *)msg;

    /* ����ClientID��ȡͨ������ */
    if (At_ClientIdToUserId(burstDtmfCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallSndBurstDTMFCnf: Get Index Fail!");
        return VOS_ERR;
    }

    /* ATģ���ڵȴ�^CBURSTDTMF��������Ĳ�������¼��ϱ� */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CBURSTDTMF_SET) {
        AT_WARN_LOG("AT_RcvTafCallSndBurstDTMFCnf: Error Option!");
        return VOS_ERR;
    }

    /* ʹ��AT_STOP_TIMER_CMD_READY�ָ�AT����ʵ��״̬ΪREADY״̬ */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* ������ʱ��Ӧ�Ĵ������ӡ����Ľ�� */
    if (burstDtmfCnf->burstDtmfCnfPara.result != TAF_CALL_SEND_BURST_DTMF_CNF_RESULT_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCalledNumInfoInd(struct MsgCB *msg)
{
    TAF_CCM_CalledNumInfoInd *calledNum = VOS_NULL_PTR;
    VOS_UINT8                 indexNum  = 0;
    VOS_UINT8                 digit[TAF_CALL_MAX_CALLED_NUMBER_CHARI_OCTET_NUM + 1];
    errno_t                   memResult;

    calledNum = (TAF_CCM_CalledNumInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(calledNum->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCalledNumInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    calledNum->calledNumInfoPara.digitNum = AT_MIN(calledNum->calledNumInfoPara.digitNum,
                                                   TAF_CALL_MAX_CALLED_NUMBER_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (calledNum->calledNumInfoPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), calledNum->calledNumInfoPara.digit,
                             calledNum->calledNumInfoPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), calledNum->calledNumInfoPara.digitNum);
    }

    /* ��pstCalledNum->aucDigit�����һλ��'\0',��ֹ��pstCalledNum->aucDigit�޽�����������AT���ϱ� */
    digit[calledNum->calledNumInfoPara.digitNum] = '\0';

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d,%d,%s%s", g_atCrLf, g_atStringTab[AT_STRING_CCALLEDNUM].text,
        calledNum->calledNumInfoPara.numType, calledNum->calledNumInfoPara.numPlan, digit, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCallingNumInfoInd(struct MsgCB *msg)
{
    TAF_CCM_CallingNumInfoInd *callingNum = VOS_NULL_PTR;
    VOS_UINT8                  indexNum   = 0;
    VOS_UINT8                  digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM + 1];
    errno_t                    memResult;

    callingNum = (TAF_CCM_CallingNumInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(callingNum->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCallingNumInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    callingNum->callIngNumInfoPara.digitNum = AT_MIN(callingNum->callIngNumInfoPara.digitNum,
                                                     TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));

    if (callingNum->callIngNumInfoPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), callingNum->callIngNumInfoPara.digit,
                             callingNum->callIngNumInfoPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), callingNum->callIngNumInfoPara.digitNum);
    }

    /* ��pstCallingNum->aucDigit�����һλ��'\0',��ֹ��pstCallingNum->aucDigit�޽�����������AT���ϱ� */
    digit[callingNum->callIngNumInfoPara.digitNum] = '\0';

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d,%d,%d,%d,%s%s", g_atCrLf,
            g_atStringTab[AT_STRING_CCALLINGNUM].text, callingNum->callIngNumInfoPara.numType,
            callingNum->callIngNumInfoPara.numPlan, callingNum->callIngNumInfoPara.pi,
            callingNum->callIngNumInfoPara.si, digit, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmDispInfoInd(struct MsgCB *msg)
{
    TAF_CCM_DisplayInfoInd *displayInfo = VOS_NULL_PTR;
    VOS_UINT8               indexNum    = 0;
    VOS_UINT8               digit[TAF_CALL_MAX_DISPALY_CHARI_OCTET_NUM + 1];
    errno_t                 memResult;

    displayInfo = (TAF_CCM_DisplayInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(displayInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmDispInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    displayInfo->disPlayInfoIndPara.digitNum = AT_MIN(displayInfo->disPlayInfoIndPara.digitNum,
                                                      TAF_CALL_MAX_DISPALY_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (displayInfo->disPlayInfoIndPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), displayInfo->disPlayInfoIndPara.digit,
                             displayInfo->disPlayInfoIndPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), displayInfo->disPlayInfoIndPara.digitNum);
    }

    /* ��pstDisplayInfo->aucDigit�����һλ��'\0',��ֹ��pstDisplayInfo->aucDigit�޽�����������AT���ϱ� */
    digit[displayInfo->disPlayInfoIndPara.digitNum] = '\0';

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s,,,%s", g_atCrLf, g_atStringTab[AT_STRING_CDISP].text, digit, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmExtDispInfoInd(struct MsgCB *msg)
{
    TAF_CCM_ExtDisplayInfoInd *extDispInfo          = VOS_NULL_PTR;
    VOS_UINT8                  digit[TAF_CALL_MAX_EXTENDED_DISPALY_CHARI_OCTET_NUM + 1];
    VOS_UINT32                 loop;
    VOS_UINT32                 digitNum;
    errno_t                    memResult;
    VOS_UINT8                  indexNum             = 0;
    VOS_UINT32                 infoRecsDataSmallNum = 0;

    extDispInfo = (TAF_CCM_ExtDisplayInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(extDispInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallExtDispInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    infoRecsDataSmallNum =
        (VOS_UINT32)TAF_MIN(extDispInfo->disPlayInfoIndPara.infoRecsDataNum, TAF_CALL_MAX_EXT_DISPLAY_DATA_NUM);
    /* ��ʼ�� */
    for (loop = 0; loop < infoRecsDataSmallNum; loop++) {
        /*
         * ��pstExtDispInfo->aucInfoRecsData[ulLoop].aucDigit�����һλ��'\0',
         * ��ֹ��pstExtDispInfo->aucInfoRecsData[ulLoop].aucDigit�޽�����������AT���ϱ�
         */
        digitNum = AT_MIN(extDispInfo->disPlayInfoIndPara.infoRecsData[loop].digitNum,
                          TAF_CALL_MAX_EXTENDED_DISPALY_CHARI_OCTET_NUM + 1);
        (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
        if (digitNum > 0) {
            memResult = memcpy_s(digit, sizeof(digit), extDispInfo->disPlayInfoIndPara.infoRecsData[loop].digit,
                                 digitNum);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), digitNum);
        }
        digit[digitNum] = '\0';

        /* �����ѯ��� */
        g_atSendDataBuff.bufLen =
            (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s,%d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_CDISP].text,
                digit, extDispInfo->disPlayInfoIndPara.extDispInd, extDispInfo->disPlayInfoIndPara.displayType,
                extDispInfo->disPlayInfoIndPara.infoRecsData[loop].dispalyTag, g_atCrLf);

        At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmConnNumInfoInd(struct MsgCB *msg)
{
    TAF_CCM_ConnNumInfoInd *connNumInfo = VOS_NULL_PTR;
    VOS_UINT8               digit[TAF_CALL_MAX_CONNECTED_NUMBER_CHARI_OCTET_NUM + 1];
    errno_t                 memResult;
    VOS_UINT8               indexNum = 0;

    connNumInfo = (TAF_CCM_ConnNumInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(connNumInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmConnNumInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    connNumInfo->connNumInfoIndPara.digitNum = AT_MIN(connNumInfo->connNumInfoIndPara.digitNum,
                                                      TAF_CALL_MAX_CONNECTED_NUMBER_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (connNumInfo->connNumInfoIndPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), connNumInfo->connNumInfoIndPara.digit,
                             connNumInfo->connNumInfoIndPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), connNumInfo->connNumInfoIndPara.digitNum);
    }

    /* ��pstConnNumInfo->aucDigit�����һλ��'\0',��ֹ��pstConnNumInfo->aucDigit�޽�����������AT���ϱ� */
    digit[connNumInfo->connNumInfoIndPara.digitNum] = '\0';

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d,%d,%d,%d,%s%s", g_atCrLf, g_atStringTab[AT_STRING_CCONNNUM].text,
            connNumInfo->connNumInfoIndPara.numType, connNumInfo->connNumInfoIndPara.numPlan,
            connNumInfo->connNumInfoIndPara.pi, connNumInfo->connNumInfoIndPara.si, digit, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmRedirNumInfoInd(struct MsgCB *msg)
{
    TAF_CCM_RedirNumInfoInd *redirNumInfo = VOS_NULL_PTR;
    VOS_UINT8                digit[TAF_CALL_MAX_REDIRECTING_NUMBER_CHARI_OCTET_NUM + 1];
    errno_t                  memResult;
    VOS_UINT16               length;
    VOS_UINT8                indexNum = 0;

    length       = 0;
    redirNumInfo = (TAF_CCM_RedirNumInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(redirNumInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmRedirNumInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    redirNumInfo->redirNumInfoIndPara.digitNum = AT_MIN(redirNumInfo->redirNumInfoIndPara.digitNum,
                                                        TAF_CALL_MAX_REDIRECTING_NUMBER_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (redirNumInfo->redirNumInfoIndPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), redirNumInfo->redirNumInfoIndPara.digitNumArray,
                             redirNumInfo->redirNumInfoIndPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), redirNumInfo->redirNumInfoIndPara.digitNum);
    }

    /* ��pstRedirNumInfo->aucDigitNum�����һλ��'\0',��ֹ��pstRedirNumInfo->aucDigitNum�޽�����������AT���ϱ� */
    digit[redirNumInfo->redirNumInfoIndPara.digitNum] = '\0';

    /* �����ѯ���������EXTENSIONBIT1��EXTENSIONBIT2�����ѡ�� */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d,%d,%s", g_atCrLf, g_atStringTab[AT_STRING_CREDIRNUM].text,
        redirNumInfo->redirNumInfoIndPara.numType, redirNumInfo->redirNumInfoIndPara.numPlan, digit);

    if (redirNumInfo->redirNumInfoIndPara.opPi == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redirNumInfo->redirNumInfoIndPara.pi);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    if (redirNumInfo->redirNumInfoIndPara.opSi == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redirNumInfo->redirNumInfoIndPara.si);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    if (redirNumInfo->redirNumInfoIndPara.opRedirReason == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", redirNumInfo->redirNumInfoIndPara.redirReason);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmSignalInfoInd(struct MsgCB *msg)
{
    TAF_CCM_SignalInfoInd *pstsignalInfo = VOS_NULL_PTR;
    VOS_UINT8              indexNum      = 0;

    pstsignalInfo = (TAF_CCM_SignalInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(pstsignalInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmSignalInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d,%d,%d%s", g_atCrLf, g_atStringTab[AT_STRING_CSIGTONE].text,
            pstsignalInfo->signalInfoIndPara.signalType, pstsignalInfo->signalInfoIndPara.alertPitch,
            pstsignalInfo->signalInfoIndPara.signal, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmLineCtrlInfoInd(struct MsgCB *msg)
{
    TAF_CCM_LineCtrlInfoInd *lineCtrlInfo = VOS_NULL_PTR;
    VOS_UINT16               length;
    VOS_UINT8                indexNum = 0;

    length       = 0;
    lineCtrlInfo = (TAF_CCM_LineCtrlInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(lineCtrlInfo->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCcmLineCtrlInfoInd: Get Index Fail!");
        return VOS_ERR;
    }

    /* �����ѯ��� */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%d", g_atCrLf, g_atStringTab[AT_STRING_CLCTR].text,
        lineCtrlInfo->lineCtrlInfoIndPara.polarityIncluded);

    if (lineCtrlInfo->lineCtrlInfoIndPara.toggleModePresent == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", lineCtrlInfo->lineCtrlInfoIndPara.toggleMode);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    if (lineCtrlInfo->lineCtrlInfoIndPara.reversePolarityPresent == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", lineCtrlInfo->lineCtrlInfoIndPara.reversePolarity);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d%s", lineCtrlInfo->lineCtrlInfoIndPara.powerDenialTime, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmCCWACInd(struct MsgCB *msg)
{
    TAF_CCM_CcwacInfoInd *ccwac = VOS_NULL_PTR;
    VOS_UINT8             digit[TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM + 1];
    errno_t               memResult;
    VOS_UINT16            length;
    VOS_UINT8             indexNum = 0;

    length = 0;
    ccwac  = (TAF_CCM_CcwacInfoInd *)msg;

    /* ����clientId��ȡͨ������ */
    if (At_ClientIdToUserId(ccwac->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCCWACInd: Get Index Fail!");
        return VOS_ERR;
    }

    ccwac->ccwacInfoPara.digitNum = AT_MIN(ccwac->ccwacInfoPara.digitNum,
                                           TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM);
    /* ��ʼ�� */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (ccwac->ccwacInfoPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), ccwac->ccwacInfoPara.digit, ccwac->ccwacInfoPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), ccwac->ccwacInfoPara.digitNum);
    }

    /* ��pstCCWAC->aucDigit�����һλ��'\0',��ֹ��pstCCWAC->aucDigit�޽�����������AT���ϱ� */
    if (ccwac->ccwacInfoPara.digitNum < (TAF_CALL_MAX_CALLING_NUMBER_CHARI_OCTET_NUM + 1)) {
        digit[ccwac->ccwacInfoPara.digitNum] = '\0';
    }

    /* �����ѯ��� */
    if (ccwac->ccwacInfoPara.signalIsPresent == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%s,%d,%d,%d,%d,%d,%d,%d,%d%s", g_atCrLf,
            g_atStringTab[AT_STRING_CCWAC].text, digit, ccwac->ccwacInfoPara.pi, ccwac->ccwacInfoPara.si,
            ccwac->ccwacInfoPara.numType, ccwac->ccwacInfoPara.numPlan, ccwac->ccwacInfoPara.signalIsPresent,
            ccwac->ccwacInfoPara.signalType, ccwac->ccwacInfoPara.alertPitch, ccwac->ccwacInfoPara.signal, g_atCrLf);

    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%s,%d,%d,%d,%d,%d,,,%s", g_atCrLf,
            g_atStringTab[AT_STRING_CCWAC].text, digit, ccwac->ccwacInfoPara.pi, ccwac->ccwacInfoPara.si,
            ccwac->ccwacInfoPara.numType, ccwac->ccwacInfoPara.numPlan, ccwac->ccwacInfoPara.signalIsPresent,
            g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return VOS_OK;
}


VOS_UINT32 At_TestCContinuousDTMFPara(VOS_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "^CCONTDTMF: (1-7),(0,1),(0-9,*,#)");
    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 AT_SetCContinuousDTMFPara(VOS_UINT8 indexNum)
{
    VOS_UINT32            rst;
    TAF_Ctrl              ctrl;
    TAF_CALL_ContDtmfPara contDTMFPara;
    ModemIdUint16         modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&contDTMFPara, sizeof(contDTMFPara), 0x00, sizeof(contDTMFPara));

    /* Check the validity of parameter */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetCContinuousDTMFPara: Non set command!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  Check the validity of <Call_ID> and <Switch> */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_WARN_LOG("AT_SetCContinuousDTMFPara: Invalid <Call_ID> or <Switch>!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*
     * If the <Switch> is Start and the number of parameter isn't equal to 3.
     * Or if the <Switch> is Stop and the number of parameter isn't equal to 2��both invalid
     */
    if (((g_atParaList[1].paraValue == TAF_CALL_CONT_DTMF_STOP) && (g_atParaIndex != AT_CCONTDTMF_PARA_NUM_MIN)) ||
        ((g_atParaList[1].paraValue == TAF_CALL_CONT_DTMF_START) && (g_atParaIndex != AT_CCONTDTMF_PARA_NUM_MAX))) {
        AT_WARN_LOG("AT_SetCContinuousDTMFPara: The number of parameters mismatch!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* If the <Switch> is Start,the <Dtmf_Key> should be setted and check its validity */
    if (g_atParaList[1].paraValue == TAF_CALL_CONT_DTMF_START) {
        if (AT_CheckCContDtmfKeyPara() == VOS_ERR) {
            AT_WARN_LOG("AT_SetCContinuousDTMFPara: Invalid <Dtmf_Key>!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    contDTMFPara.callId     = (VOS_UINT8)g_atParaList[0].paraValue;
    contDTMFPara.dtmfSwitch = (VOS_UINT8)g_atParaList[1].paraValue;
    contDTMFPara.digit      = (VOS_UINT8)g_atParaList[AT_CCONTDTMF_DTMF_KEY].para[AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP];

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* ����ID_TAF_CCM_SEND_CONT_DTMF_REQ��Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, &contDTMFPara, ID_TAF_CCM_SEND_CONT_DTMF_REQ, sizeof(contDTMFPara), modemId);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCONTDTMF_SET;

        /* Return hang-up state */
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}


VOS_UINT32 AT_CheckCContDtmfKeyPara(VOS_VOID)
{
    if (g_atParaList[AT_CCONTDTMF_DTMF_KEY].paraLen != AT_CCONTDTMF_DTMF_KEY_VALID_LEN) {
        return VOS_ERR;
    }

    if (((g_atParaList[AT_CCONTDTMF_DTMF_KEY].para[AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP] >= '0') &&
         (g_atParaList[AT_CCONTDTMF_DTMF_KEY].para[AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP] <= '9')) ||
         (g_atParaList[AT_CCONTDTMF_DTMF_KEY].para[AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP] == '*') ||
         (g_atParaList[AT_CCONTDTMF_DTMF_KEY].para[AT_CCONTDTMF_DTMF_KEY_SWITCH_STOP] == '#')) {
        return VOS_OK;
    } else {
        return VOS_ERR;
    }
}


VOS_UINT32 AT_RcvTafCcmSndContinuousDTMFCnf(struct MsgCB *msg)
{
    VOS_UINT8                indexNum    = 0;
    TAF_CCM_SendContDtmfCnf *contDtmfCnf = VOS_NULL_PTR;

    contDtmfCnf = (TAF_CCM_SendContDtmfCnf *)msg;

    /* According to ClientID to get the index */
    if (At_ClientIdToUserId(contDtmfCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallSndContinuousDTMFCnf: Get Index Fail!");
        return VOS_ERR;
    }

    /* AT module is waiting for report the result of ^CCONTDTMF command */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCONTDTMF_SET) {
        AT_WARN_LOG("AT_RcvTafCallSndContinuousDTMFCnf: Error Option!");
        return VOS_ERR;
    }

    /* Use AT_STOP_TIMER_CMD_READY to recover the AT command state to READY state */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* According to the error code of temporary respond, printf the result of command */
    if (contDtmfCnf->contDtmfCnfPara.result != TAF_CALL_SEND_CONT_DTMF_CNF_RESULT_SUCCESS) {
        At_FormatResultData(indexNum, AT_ERROR);
    } else {
        At_FormatResultData(indexNum, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmRcvContinuousDtmfInd(struct MsgCB *msg)
{
    TAF_CCM_ContDtmfInd *rcvContDtmf = VOS_NULL_PTR;
    VOS_UINT8            digit[AT_DIGIT_MAX_LENGTH];
    VOS_UINT8            indexNum = 0;

    rcvContDtmf = (TAF_CCM_ContDtmfInd *)msg;

    /* According to ClientID to get the index */
    if (At_ClientIdToUserId(rcvContDtmf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallRcvContinuousDtmfInd: Get Index Fail!");
        return VOS_ERR;
    }

    /*
     * Initialize aucDigit[0] with pstRcvContDtmf->ucDigit and  aucDigit[1] = '\0'
     * Because AT_FormatReportString does not allow to print pstRcvContDtmf->ucDigit with %c
     * Hence, need to convert digit into string and print as string
     */
    digit[0] = rcvContDtmf->contDtmfIndPara.digit;
    digit[1] = '\0';

    /* Output the inquire result */
    if (rcvContDtmf->contDtmfIndPara.dtmfSwitch == TAF_CALL_CONT_DTMF_START) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^CCONTDTMF: %d,%d,\"%s\"%s", g_atCrLf, rcvContDtmf->ctrl.callId,
            rcvContDtmf->contDtmfIndPara.dtmfSwitch, digit, g_atCrLf);
    } else {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^CCONTDTMF: %d,%d%s", g_atCrLf, rcvContDtmf->ctrl.callId,
            rcvContDtmf->contDtmfIndPara.dtmfSwitch, g_atCrLf);
    }

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafCcmRcvBurstDtmfInd(struct MsgCB *msg)
{
    TAF_CCM_BurstDtmfInd *rcvBurstDtmf = VOS_NULL_PTR;
    VOS_UINT8             digit[TAF_CALL_MAX_BURST_DTMF_NUM + 1];
    errno_t               memResult;
    VOS_UINT8             indexNum = 0;

    rcvBurstDtmf = (TAF_CCM_BurstDtmfInd *)msg;

    /* According to ClientID to get the index */
    if (At_ClientIdToUserId(rcvBurstDtmf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallRcvBurstDtmfInd: Get Index Fail!");
        return VOS_ERR;
    }

    rcvBurstDtmf->burstDtmfIndPara.digitNum = (VOS_UINT8)AT_MIN(rcvBurstDtmf->burstDtmfIndPara.digitNum,
                                                     TAF_CALL_MAX_BURST_DTMF_NUM);
    /* initialization */
    (VOS_VOID)memset_s(digit, sizeof(digit), 0x00, sizeof(digit));
    if (rcvBurstDtmf->burstDtmfIndPara.digitNum > 0) {
        memResult = memcpy_s(digit, sizeof(digit), rcvBurstDtmf->burstDtmfIndPara.digit,
                             rcvBurstDtmf->burstDtmfIndPara.digitNum);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(digit), rcvBurstDtmf->burstDtmfIndPara.digitNum);
    }

    /* Add the '\0' to the last byte of pstRcvBurstDtmf->aucDigit */
    digit[rcvBurstDtmf->burstDtmfIndPara.digitNum] = '\0';

    /* Output the inquire result */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CBURSTDTMF: %d,\"%s\",%d,%d%s", g_atCrLf, rcvBurstDtmf->ctrl.callId, digit,
        rcvBurstDtmf->burstDtmfIndPara.onLength, rcvBurstDtmf->burstDtmfIndPara.offLength, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);

    return VOS_OK;
}

VOS_UINT32 AT_TestCclprPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (1-7)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetCclprPara(VOS_UINT8 indexNum)
{
    VOS_UINT32           result;
    TAF_Ctrl             ctrl;
    TAF_CALL_QryClprPara qryClprPara;
    ModemIdUint16        modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&qryClprPara, sizeof(qryClprPara), 0x00, sizeof(qryClprPara));

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    qryClprPara.callId          = (MN_CALL_ID_T)g_atParaList[0].paraValue;
    qryClprPara.qryClprModeType = TAF_CALL_QRY_CLPR_MODE_C;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* ���Ϳ����ϢID_TAF_CCM_QRY_CLPR_REQ��C�� */
    result = TAF_CCM_CallCommonReq(&ctrl, (void *)&qryClprPara, ID_TAF_CCM_QRY_CLPR_REQ, sizeof(qryClprPara), modemId);

    if (result != VOS_OK) {
        AT_WARN_LOG("AT_SetCclprPara: TAF_XCALL_SendCclpr fail.");
        return AT_ERROR;
    }

    /* ����ATģ��ʵ���״̬Ϊ�ȴ��첽���� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCLPR_SET;

    return AT_WAIT_ASYNC_RETURN;
}
#endif

VOS_UINT32 AT_SetRejCallPara(VOS_UINT8 indexNum)
{
    AT_ModemCcCtx *ccCtx = VOS_NULL_PTR;

    TAF_Ctrl          ctrl;
    MN_CALL_SupsParam callSupsPara;
    ModemIdUint16     modemId;

    /* ָ�����ͼ�� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetRejCallPara : Current Option is not AT_CMD_REJCALL!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������� */
    if (g_atParaIndex != AT_REJCALL_PARA_NUM) {
        AT_WARN_LOG("AT_SetRejCallPara : The number of input parameters is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����Ϊ�� */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        AT_WARN_LOG("AT_SetRejCallPara : Input parameters is error!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&callSupsPara, sizeof(callSupsPara), 0x00, sizeof(callSupsPara));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    callSupsPara.callSupsCmd  = MN_CALL_SUPS_CMD_REL_INCOMING_OR_WAITING;
    callSupsPara.callId       = (VOS_UINT8)g_atParaList[0].paraValue;
    callSupsPara.callRejCause = (VOS_UINT8)g_atParaList[1].paraValue;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    if (TAF_CCM_CallCommonReq(&ctrl, &callSupsPara, ID_TAF_CCM_CALL_SUPS_CMD_REQ, sizeof(callSupsPara), modemId) !=
        VOS_OK) {
        AT_WARN_LOG("AT_SetRejCallPara : Send Msg fail!");
        return AT_ERROR;
    }

    /* ֹͣ�Զ����� */
    ccCtx = AT_GetModemCcCtxAddrFromClientId(indexNum);

    if (ccCtx->s0TimeInfo.timerStart == VOS_TRUE) {
        AT_StopRelTimer(ccCtx->s0TimeInfo.timerName, &(ccCtx->s0TimeInfo.s0Timer));
        ccCtx->s0TimeInfo.timerStart = TAF_FALSE;
        ccCtx->s0TimeInfo.timerName  = 0;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_REJCALL_SET;

    return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
}


VOS_UINT32 AT_TestRejCallPara(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_QryCimsErrPara(VOS_UINT8 indexNum)
{
    TAF_CALL_ErrorInfoText *callErrInfo = VOS_NULL_PTR;

    callErrInfo = AT_GetCallErrInfoText(indexNum);

    /* �����ѯ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,\"%s\"", g_parseContext[indexNum].cmdElement->cmdName,
        AT_GetCsCallErrCause(indexNum), callErrInfo->errInfoText);

    return AT_OK;
}
#endif


VOS_UINT32 AT_QryCsChannelInfoPara(VOS_UINT8 indexNum)
{
    TAF_Ctrl      ctrl;
    VOS_UINT32    rst;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* AT��CCM���ͺ�����Ϣ */
    rst = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_QRY_CHANNEL_INFO_REQ, 0, modemId);

    if (rst != VOS_OK) {
        AT_WARN_LOG("AT_QryCsChannelInfoPara: Send Msg fail!");
        return AT_ERROR;
    }

    /* ����ATģ��ʵ���״̬Ϊ�ȴ��첽���� */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CSCHANNELINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

