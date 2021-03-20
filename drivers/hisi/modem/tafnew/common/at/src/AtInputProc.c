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

#include "AtInputProc.h"
#include "securec.h"

#include "ppp_interface.h"
#include "AtUsbComItf.h"
#include "AtTafAgentInterface.h"
#include "TafAgentInterface.h"
#include "AtCmdMsgProc.h"
#include "AtDataProc.h"
#include "at_mdrv_interface.h"
#include "AtMntn.h"
#include "AcpuReset.h"

#include "AtInternalMsg.h"
#include "mn_comm_api.h"

#include "dms_debug.h"
#include "dms_file_node_i.h"
#include "product_config.h"

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
#include "dms_port_i.h"
#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/* adrv.h����hifireset_regcbfunc���������������ýӿ�ɾ���󣬸�ͷ�ļ�����ɾ�� */
#include "adrv.h"
#endif


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_AT_INPUTPROC_C
#define AT_MSG_CUSTOMIZED_ITEM_LEN 4

VOS_UINT32 g_atUsbDebugFlag = VOS_FALSE;

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
VOS_UINT8 g_cmuxOpen = VOS_FALSE;
#endif

/* AT֡�ṹ��DRV ֵ֮��Ķ�Ӧ��ϵ */
static const AT_UartFormatParam g_atUartFormatTab[] = {
    /* auto detect (not support) */

    { AT_UART_FORMAT_8DATA_2STOP, AT_UART_DATA_LEN_8_BIT,
      AT_UART_STOP_LEN_2_BIT, AT_UART_PARITY_LEN_0_BIT },

    { AT_UART_FORMAT_8DATA_1PARITY_1STOP, AT_UART_DATA_LEN_8_BIT,
      AT_UART_STOP_LEN_1_BIT, AT_UART_PARITY_LEN_1_BIT },

    { AT_UART_FORMAT_8DATA_1STOP, AT_UART_DATA_LEN_8_BIT,
      AT_UART_STOP_LEN_1_BIT, AT_UART_PARITY_LEN_0_BIT },

    { AT_UART_FORMAT_7DATA_2STOP, AT_UART_DATA_LEN_7_BIT,
      AT_UART_STOP_LEN_2_BIT, AT_UART_PARITY_LEN_0_BIT },

    { AT_UART_FORMAT_7DATA_1PARITY_1STOP, AT_UART_DATA_LEN_7_BIT,
      AT_UART_STOP_LEN_1_BIT, AT_UART_PARITY_LEN_1_BIT },

    { AT_UART_FORMAT_7DATA_1STOP, AT_UART_DATA_LEN_7_BIT,
      AT_UART_STOP_LEN_1_BIT, AT_UART_PARITY_LEN_0_BIT }
};


MODULE_EXPORTED VOS_UINT32 AT_GetUsbDebugFlag(VOS_VOID)
{
    return g_atUsbDebugFlag;
}


AT_ClientManage* AT_GetClientManage(AT_ClientIdUint16 clientId)
{
    /* �ڲ��ӿڣ��ɵ��÷���֤client id��Ч */
    return &(g_atClientTab[clientId]);
}


VOS_INT32 AT_RcvCmdData(VOS_UINT16 clientId, VOS_UINT8 *data, VOS_UINT32 length)
{
    AT_ClientManage   *atClientManage = VOS_NULL_PTR;
    VOS_UINT32         ret;

    /* AT����ȴ�����ͳһ�õ�VOS_UINT16������֧�ַ�Χ�ĳ��ȴ�ӡ�쳣���� */
    if (length > AT_RCV_DATA_MAX_LEN) {
        AT_WARN_LOG("AT_RcvCmdData: length is more than 0xffff!");
        return VOS_ERR;
    }

    if (clientId >= AT_CLIENT_ID_BUTT) {
        AT_WARN_LOG1("AT_RcvCmdData: client id more than AT_CLIENT_ID_BUTT!", clientId);
        return VOS_ERR;
    }

    atClientManage = AT_GetClientManage(clientId);

    if (atClientManage->used == AT_CLIENT_NULL) {
        AT_WARN_LOG("AT_RcvCmdData: APP client is unused!");
        return VOS_ERR;
    }

    if (AT_GetUsbDebugFlag() == VOS_TRUE) {
        PS_PRINTF_INFO("AT_RcvCmdData clientId = %d, length = %d, data = %s\n", clientId, length, data);
    }

    ret = At_CmdStreamPreProc((VOS_UINT8)clientId, data, (VOS_UINT16)length);

    if (ret == AT_SUCCESS) {
        return VOS_OK;
    } else {
        return VOS_ERR;
    }
}


VOS_UINT32 AT_GetAtMsgStruMsgLength(VOS_UINT32 infoLength)
{
    VOS_UINT32 msgLength;

    if (infoLength > AT_MSG_CUSTOMIZED_ITEM_LEN) {
        msgLength = (sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH) + (infoLength - AT_MSG_CUSTOMIZED_ITEM_LEN);
    } else {
        msgLength = sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH;
    }

    return msgLength;
}


VOS_UINT8 AT_GetUserTypeFromIndex(VOS_UINT8 indexNum)
{
    VOS_UINT8 userType;

    if (indexNum < AT_MAX_CLIENT_NUM) {
        userType = g_atClientTab[indexNum].userType;
    } else {
        userType = AT_BUTT_USER;
    }

    return userType;
}


VOS_VOID AT_SetAts3Value(VOS_UINT8 value)
{
    if (g_atUsbDebugFlag == VOS_TRUE) {
        g_atS3 = value;
    }

    return;
}



VOS_BOOL AT_IsClientAllowEcho(VOS_UINT8 clientId)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* UART NDIS ͨ��������� */
    if ((clientId == AT_CLIENT_ID_UART) || (clientId == AT_CLIENT_ID_NDIS)) {
        return VOS_FALSE;
    }

    /* HSUARTͨ��������� */
#if (FEATURE_AT_HSUART == FEATURE_ON)
    if (clientId == AT_CLIENT_ID_HSUART) {
        return VOS_FALSE;
    }
#endif

    if (AT_CheckAppUser(clientId) == VOS_TRUE) {
        /* AGPSͨ��������� */
#if (FEATURE_VCOM_EXT == FEATURE_ON)
        if ((clientId == AT_CLIENT_ID_APP9) || (clientId == AT_CLIENT_ID_APP12) || (clientId == AT_CLIENT_ID_APP24)) {
            return VOS_FALSE;
        }
#endif

        /* E5��̬������� */
        if (*systemAppConfig == SYSTEM_APP_WEBUI) {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_VOID AT_CmdStreamEcho(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT32 ret;
    VOS_UINT16 echoLen;

    /* �ж�pData�����Ľ�β�Ƿ�Ϊ<CR><LF>��ʽ��������2Ϊ�س����������ַ����� */
    if ((len > 2) && (g_atS3 == data[len - 2]) && (g_atS4 == data[len - 1])) {
        /* ɾȥ��β��<LF>�ַ� */
        echoLen = len - 1;
    } else {
        echoLen = len;
    }

    if (AT_IsClientAllowEcho(indexNum) == VOS_TRUE) {
        ret = DMS_PORT_SendData(g_atClientTab[indexNum].portNo, data, echoLen);
    } else {
        AT_ERR_LOG1("AT_CmdStreamEcho:WARNING: not allow echo, ucIndex:", indexNum);
        return;
    }

    if (ret != VOS_OK) {
        AT_ERR_LOG1("AT_CmdStreamEcho:WARNING: Abnormal UserType,ucIndex:", indexNum);
    }

    return;
}


VOS_UINT32 At_CmdStreamPreProc(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8 *head = TAF_NULL_PTR;
    errno_t    memResult;
    VOS_UINT16 count = 0;
    VOS_UINT16 total = 0;

    head = data;

    if (g_atUsbDebugFlag == VOS_TRUE) {
        PS_PRINTF_INFO("<At_CmdStreamPreProc> AtEType = %d, UserType = %d, g_atS3 = %d\n", g_atEType,
                       g_atClientTab[indexNum].userType, g_atS3);
    }

    /* ����ͨ������ */
    if (g_atEType == AT_E_ECHO_CMD) {
        AT_CmdStreamEcho(indexNum, data, len);
    }

    /*
     * MACϵͳ�ϵ�MP��̨����:AT+CMGS=**<CR><^z><Z>(��AT+CMGW=**<CR><^z><Z>)
     * Ϊ�˹�ܸ����⣬��Ҫ�ڽ��յ�������ʽ��������
     * ��Ҫ����������Ч�ַ�<^z><Z>ɾȥ
     */
    AT_DiscardInvalidCharForSms(data, &len);

    /* �����������ַ��Ž���������Ϣ��ʽ���͵�AT����Ϣ������: <CR>/<ctrl-z>/<ESC> */
    while (count++ < len) {
        if (At_CheckSplitChar((*((data + count) - 1)))) {
            if (g_atDataBuff[indexNum].buffLen > 0) {
                if ((g_atDataBuff[indexNum].buffLen + count) >= AT_COM_BUFF_LEN) {
                    g_atDataBuff[indexNum].buffLen = 0;
                    AT_WARN_LOG("At_CmdStreamPreProc: buff is full at split char!");
                    return AT_FAILURE;
                }
                memResult = memcpy_s(&g_atDataBuff[indexNum].dataBuff[g_atDataBuff[indexNum].buffLen],
                                     AT_COM_BUFF_LEN - g_atDataBuff[indexNum].buffLen, head, count);
                TAF_MEM_CHK_RTN_VAL(memResult, AT_COM_BUFF_LEN - g_atDataBuff[indexNum].buffLen, count);
                At_SendCmdMsg(indexNum, g_atDataBuff[indexNum].dataBuff,
                              (TAF_UINT16)(g_atDataBuff[indexNum].buffLen + count), AT_NORMAL_TYPE_MSG);
                head                           = data + count;
                total                          = count;
                g_atDataBuff[indexNum].buffLen = 0;
            } else {
                At_SendCmdMsg(indexNum, head, count - total, AT_NORMAL_TYPE_MSG);
                head  = data + count;
                total = count;
            }
        }
    }

    if (total < len) {
        if ((g_atDataBuff[indexNum].buffLen + (len - total)) >= AT_COM_BUFF_LEN) {
            g_atDataBuff[indexNum].buffLen = 0;
            AT_WARN_LOG("At_CmdStreamPreProc: buff is full after split char!");
            return AT_FAILURE;
        }
        memResult = memcpy_s(&g_atDataBuff[indexNum].dataBuff[g_atDataBuff[indexNum].buffLen],
                             AT_COM_BUFF_LEN - g_atDataBuff[indexNum].buffLen, head, (TAF_UINT32)(len - total));
        TAF_MEM_CHK_RTN_VAL(memResult, AT_COM_BUFF_LEN - g_atDataBuff[indexNum].buffLen, (TAF_UINT32)(len - total));
        g_atDataBuff[indexNum].buffLen += (VOS_UINT16)((data - head) + len);
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_ProcDtrDeassertedIndPppDataMode(AT_ClientIdUint16 indexNum)
{
    AT_ClientManage    *clientManage = VOS_NULL_PTR;

    clientManage = AT_GetClientManage(indexNum);

    /*
     * 1.�ж�(g_atClientTab[ucIndex].CmdCurrentOpt != AT_CMD_PS_DATA_CALL_END_SET)
     *  ��Ŀ��:�������Ͽ�����֮ǰ�Ѿ�ִ����PDP DEACTIVE���̣���ʱ��������DTR����
     *  ������ִ�и��쳣����
     * 2.�÷�֧�Ĵ�����:����������״̬��,�û�����PPP�Ͽ�����ʱPPP�����޷�������
     *   ֻ�����������DTR�źŵ�ʱ��ִ��PPP�Ͽ�����
     */
    if ((clientManage->cmdCurrentOpt == AT_CMD_PS_DATA_CALL_END_SET) ||
        (clientManage->cmdCurrentOpt == AT_CMD_WAIT_PPP_PROTOCOL_REL_SET)) {
        return AT_SUCCESS;
    }

    /*
     * ����������״̬�£���PPP�Ͽ���Э�̱������޷�����UE��ģ�
     * UEֻ����DTR�ź����͵�ʱ��,ִ��PPP���ŶϿ�����
     */
    if (DMS_PORT_IsCtsDeasserted(clientManage->portNo) == VOS_TRUE) {
        /* ͨ������cts��ֹͣ���� */
        DMS_PORT_AssertCts(clientManage->portNo);
    }

    AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_REQ);

    /* ֹͣModem�ڵ�AT��ʱ���Լ�AT��·�ĵ�ǰ����ָʾ */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* EVENT - RCV Down DTR to Disconnect PPP in Abnormal procedure(PDP type:IP) ;index */
    AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_DOWN_DTR_RELEASE_PPP_IP_TYPE, (VOS_UINT8 *)&indexNum, sizeof(indexNum));

    if (TAF_PS_PPP_DIAL_DOWN(WUEPS_PID_AT, AT_PS_BuildExClientId(clientManage->clientId), 0,
                       clientManage->cid) == VOS_OK) {
        /* ����ʱ�� */
        if (At_StartTimer(AT_SET_PARA_TIME, (VOS_UINT8)indexNum) != AT_SUCCESS) {
            AT_ERR_LOG("AT_ProcDtrDeassertedIndPppDataMode:ERROR:Start Timer");
            return AT_FAILURE;
        }

        /* ���õ�ǰ�������� */
        clientManage->cmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
    } else {
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_ProcDtrDeassertedIndRawDataMode(AT_ClientIdUint16 indexNum)
{
    AT_ClientManage    *clientManage = VOS_NULL_PTR;

    clientManage = AT_GetClientManage(indexNum);

    /* ��ԭ�ȿ��������أ�����ֹͣ���� */
    if (DMS_PORT_IsCtsDeasserted(clientManage->portNo) == VOS_TRUE) {
        /* ͨ������cts��ֹͣ���� */
        DMS_PORT_AssertCts(clientManage->portNo);
    }

    AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);

    /* ֹͣModem�ڵ�AT��ʱ���Լ�AT��·�ĵ�ǰ����ָʾ */
    AT_STOP_TIMER_CMD_READY(indexNum);

    /* EVENT - RCV Down DTR to Disconnect PPP in Abnormal procedure(PDP type:PPP) ;index */
    AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_DOWN_DTR_RELEASE_PPP_PPP_TYPE, (VOS_UINT8*)&indexNum, sizeof(indexNum));

    if (TAF_PS_PPP_DIAL_DOWN(WUEPS_PID_AT, AT_PS_BuildExClientId(clientManage->clientId), 0,
                       clientManage->cid) == VOS_OK) {
        clientManage->cmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
        return AT_SUCCESS;
    } else {
        return AT_FAILURE;
    }
}

/* ɾ��At_SetModemStatusForFC����, ���ܺ�At_SetModemStatus�ظ� */


VOS_UINT32 AT_ModemEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_MODEM;

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_MODEM, ID_AT_MNTN_RESULT_MODEM);
    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_MODEM;
    g_atClientTab[indexNum].portNo   = DMS_PORT_MODEM;
    g_atClientTab[indexNum].userType = AT_MODEM_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode         = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus   = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_MODEM;
    clientInfo.type       = DMS_CLIENT_TYPE_MODEM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_MODEM, &clientInfo);

    AT_NORM_LOG2("AT_ModemEst ucIndex and reg ret is ", indexNum, ret);

    return ret;
}


MODULE_EXPORTED VOS_VOID AT_SetUsbDebugFlag(VOS_UINT32 flag)
{
    g_atUsbDebugFlag = flag;
}


VOS_UINT32 At_UsbPcuiEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_PCUI;

    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_PCUI, ID_AT_MNTN_RESULT_PCUI);

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_PCUI;
    g_atClientTab[indexNum].portNo   = DMS_PORT_PCUI;
    g_atClientTab[indexNum].userType = AT_USBCOM_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode         = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus   = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_PCUI;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_PCUI, &clientInfo);

    AT_WARN_LOG2("At_UsbPcuiEst ucIndex and reg ret is ", indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 At_UsbCtrEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_CTRL;

    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_CTRL, ID_AT_MNTN_RESULT_CTRL);

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_CTRL;
    g_atClientTab[indexNum].portNo   = DMS_PORT_CTRL;
    g_atClientTab[indexNum].userType = AT_CTR_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode         = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus   = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_CTRL;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_CTRL, &clientInfo);

    AT_WARN_LOG2("At_UsbCtrEst ucIndex and reg ret is ", indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 At_UsbPcui2Est(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_PCUI2;

    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_PCUI2, ID_AT_MNTN_RESULT_PCUI2);

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_PCUI2;
    g_atClientTab[indexNum].portNo   = DMS_PORT_PCUI2;
    g_atClientTab[indexNum].userType = AT_PCUI2_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode         = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus   = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_PCUI2;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_PCUI2, &clientInfo);

    AT_WARN_LOG2("At_UsbPcui2Est ucIndex and reg ret is ", indexNum, ret);

    return VOS_OK;
}


VOS_VOID AT_UartInitPort(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_UART;

    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_UART, ID_AT_MNTN_RESULT_UART);

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_UART;
    g_atClientTab[indexNum].portNo   = DMS_PORT_UART;
    g_atClientTab[indexNum].userType = AT_UART_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode       = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_UART;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_UART, &clientInfo);

    AT_WARN_LOG2("AT_UART_InitPort ucIndex and reg ret is ", indexNum, ret);
}


VOS_UINT32 AT_CheckUartUser(AT_ClientIdUint16 indexNum)
{
#if (FEATURE_UART_BAUDRATE_AUTO_ADAPTION == FEATURE_ON)
    if (g_atClientTab[indexNum].userType == AT_UART_USER) {
        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}


VOS_UINT32 AT_UART_IsBaudRateValid(AT_UartBaudrateUint32 baudRate)
{
    VOS_UINT32 ret = VOS_FALSE;

    switch (baudRate) {
        case AT_UART_BAUDRATE_0:
        case AT_UART_BAUDRATE_9600:
        case AT_UART_BAUDRATE_19200:
        case AT_UART_BAUDRATE_38400:
        case AT_UART_BAUDRATE_57600:
        case AT_UART_BAUDRATE_115200:
            ret = VOS_TRUE;
            break;

        default:
            ret = VOS_FALSE;
            break;
    }

    return ret;
}
#if (FEATURE_AT_HSUART == FEATURE_ON)


VOS_VOID AT_UART_ConfigDefaultPara(VOS_VOID)
{
    AT_UartPhyCfg *phyCfg = VOS_NULL_PTR;
    phyCfg                = AT_GetUartPhyCfgInfo();

    /* ����UART������ */
    AT_HSUART_ConfigBaudRate(AT_CLIENT_ID_UART, phyCfg->baudRate);
}


VOS_UINT32 AT_HSUART_IsBaudRateValid(AT_UartBaudrateUint32 baudRate)
{
    VOS_UINT32 ret = VOS_FALSE;

    /*
     * ARM   --- 0,300,600,1200,2400,4800,9600,19200,38400,57600,115200,
     * 230400,460800,921600,2764800,4000000
     */
    switch (baudRate) {
        case AT_UART_BAUDRATE_0:
        case AT_UART_BAUDRATE_300:
        case AT_UART_BAUDRATE_600:
        case AT_UART_BAUDRATE_1200:
        case AT_UART_BAUDRATE_2400:
        case AT_UART_BAUDRATE_4800:
        case AT_UART_BAUDRATE_9600:
        case AT_UART_BAUDRATE_19200:
        case AT_UART_BAUDRATE_38400:
        case AT_UART_BAUDRATE_57600:
        case AT_UART_BAUDRATE_115200:
        case AT_UART_BAUDRATE_230400:
        case AT_UART_BAUDRATE_460800:
        case AT_UART_BAUDRATE_921600:
        case AT_UART_BAUDRATE_2764800:
        case AT_UART_BAUDRATE_4000000:
            ret = VOS_TRUE;
            break;

        default:
            ret = VOS_FALSE;
            break;
    }

    return ret;
}


VOS_UINT32 AT_HSUART_IsFormatValid(AT_UartFormatUint8 format)
{
    VOS_UINT32 ret = VOS_FALSE;

    switch (format) {
        case AT_UART_FORMAT_8DATA_2STOP:
        case AT_UART_FORMAT_8DATA_1PARITY_1STOP:
        case AT_UART_FORMAT_8DATA_1STOP:
        case AT_UART_FORMAT_7DATA_2STOP:
        case AT_UART_FORMAT_7DATA_1PARITY_1STOP:
        case AT_UART_FORMAT_7DATA_1STOP:
            ret = VOS_TRUE;
            break;

        default:
            ret = VOS_FALSE;
            break;
    }

    return ret;
}


VOS_UINT32 AT_HSUART_IsParityValid(AT_UartParityUint8 parity)
{
    VOS_UINT32 ret = VOS_FALSE;

    /*
     * ע: ��ͬ�汾ѡ�õ�UART IP��ͬ, У�����������
     * V3R3            --- ODD, EVEN, MARK, SPACE
     * V7R11(or later) --- ODD, EVEN
     */
    switch (parity) {
        case AT_UART_PARITY_ODD:
        case AT_UART_PARITY_EVEN:
            ret = VOS_TRUE;
            break;

        default:
            ret = VOS_FALSE;
            break;
    }

    return ret;
}


VOS_UINT32 AT_HSUART_ValidateFlowCtrlParam(AT_UartFcDceByDteUint8 fcDceByDte, AT_UartFcDteByDceUint8 fcDteByDce)
{
    /*
     * ע: ��ͬ�汾ѡ�õ�UART IP��ͬ, ���ع���������
     * V3R3            --- Ӳ������֧�������е���������ر�
     * V7R11(or later) --- Ӳ������֧��������ͬʱ������ر�
     */
    if (fcDceByDte != fcDteByDce) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_HSUART_ValidateCharFrameParam(AT_UartFormatUint8 format, AT_UartParityUint8 parity)
{
    /* ����ʽ�����Ƿ�֧�� */
    if (AT_HSUART_IsFormatValid(format) == VOS_FALSE) {
        return VOS_FALSE;
    }

    /* �����鷽ʽ�Ƿ�֧�� */
    if (AT_HSUART_IsParityValid(parity) == VOS_FALSE) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


const AT_UartFormatParam* AT_HSUART_GetFormatParam(AT_UartFormatUint8 format)
{
    const AT_UartFormatParam   *formatTblPtr = VOS_NULL_PTR;
    const AT_UartFormatParam   *formatParam  = VOS_NULL_PTR;
    VOS_UINT32                  cnt;

    formatTblPtr = AT_UART_GET_FORMAT_TBL_PTR();

    for (cnt = 0; cnt < AT_UART_GET_FORMAT_TBL_SIZE(); cnt++) {
        if (format == formatTblPtr[cnt].format) {
            formatParam = &formatTblPtr[cnt];
        }
    }

    return formatParam;
}


VOS_UINT32 AT_HSUART_GetUdiValueByDataLen(AT_UartDataLenUint8 dataLen, VOS_UINT32 *udiValue)
{
    /* ָ��ǿռ�� */
    if (udiValue == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* ӳ�����UDI VALUE */
    switch (dataLen) {
        case AT_UART_DATA_LEN_5_BIT:
            *udiValue = DMS_WLEN_5_BITS;
            break;

        case AT_UART_DATA_LEN_6_BIT:
            *udiValue = DMS_WLEN_6_BITS;
            break;

        case AT_UART_DATA_LEN_7_BIT:
            *udiValue = DMS_WLEN_7_BITS;
            break;

        case AT_UART_DATA_LEN_8_BIT:
            *udiValue = DMS_WLEN_8_BITS;
            break;

        default:
            return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_HSUART_GetUdiValueByStopLen(AT_UartStopLenUint8 stopLen, VOS_UINT32 *udiValue)
{
    /* ָ��ǿռ�� */
    if (udiValue == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* ӳ�����UDI VALUE */
    switch (stopLen) {
        case AT_UART_STOP_LEN_1_BIT:
            *udiValue = DMS_STP2_OFF;
            break;

        case AT_UART_STOP_LEN_2_BIT:
            *udiValue = DMS_STP2_ON;
            break;

        default:
            return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_HSUART_GetUdiValueByParity(AT_UartParityUint8 parity, VOS_UINT32 *udiValue)
{
    /* ָ��ǿռ�� */
    if (udiValue == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* ӳ�����UDI VALUE */
    switch (parity) {
        case AT_UART_PARITY_ODD:
            *udiValue = DMS_PARITY_CHECK_ODD;
            break;

        case AT_UART_PARITY_EVEN:
            *udiValue = DMS_PARITY_CHECK_EVEN;
            break;

        case AT_UART_PARITY_MARK:
            *udiValue = DMS_PARITY_CHECK_MARK;
            break;

        case AT_UART_PARITY_SPACE:
            *udiValue = DMS_PARITY_CHECK_SPACE;
            break;

        default:
            return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_HSUART_ConfigFlowCtrl(VOS_UINT8 indexNum, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag)
{
    if (DMS_PORT_ConfigFlowCtrl(DMS_PORT_HSUART, rtsFlag, ctsFlag) != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_HSUART_ConfigCharFrame(VOS_UINT8 indexNum, AT_UartFormatUint8 format, AT_UartParityUint8 parity)
{
    const AT_UartFormatParam   *formatParam = VOS_NULL_PTR;
    VOS_UINT32                  udiDataLenth;
    VOS_UINT32                  udiStpLenth;
    VOS_UINT32                  udiParity;
    VOS_UINT32                  result;

    /* ������ʼ�� */
    udiDataLenth = DMS_WLEN_8_BITS;
    udiStpLenth  = DMS_STP2_OFF;
    udiParity    = DMS_PARITY_NO_CHECK;

    /* �����õ�֡��ʽ��У�鷽��ת��ΪDRV�ӿڸ�ʽ */
    formatParam = AT_HSUART_GetFormatParam(format);
    if (formatParam == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HSUART_ConfigCharFrame: Format is invalid!");
        return VOS_ERR;
    }

    result = AT_HSUART_GetUdiValueByDataLen(formatParam->dataBitLength, &udiDataLenth);
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_HSUART_ConfigCharFrame: Data bit length is invalid!");
        return VOS_ERR;
    }

    result = AT_HSUART_GetUdiValueByStopLen(formatParam->stopBitLength, &udiStpLenth);
    if (result != VOS_OK) {
        AT_ERR_LOG("AT_HSUART_ConfigCharFrame: Stop bit length is invalid!");
        return VOS_ERR;
    }

    if (formatParam->parityBitLength == AT_UART_PARITY_LEN_1_BIT) {
        result = AT_HSUART_GetUdiValueByParity(parity, &udiParity);
        if (result != VOS_OK) {
            AT_ERR_LOG("AT_HSUART_ConfigCharFrame: Parity bit length is invalid!");
            return VOS_ERR;
        }
    }

    /* ����DRV��������UART֡��ʽ������������������λ����/ֹͣλ����/У��λ�� */
    if (DMS_PORT_SetCharFrame(DMS_PORT_HSUART, udiDataLenth, udiStpLenth, udiParity) != VOS_OK) {
        AT_ERR_LOG("AT_HSUART_ConfigCharFrame: Set CharFrame failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_HSUART_ConfigBaudRate(VOS_UINT8 indexNum, AT_UartBaudrateUint32 baudRate)
{
    /* ����DRV�������ô��ڵĲ����� */
    DMS_PortIdUint16 portId = DMS_PORT_BUTT;

    if (AT_CheckHsUartUser(indexNum) == VOS_TRUE) {
        portId = DMS_PORT_HSUART;
    } else if (AT_CheckUartUser(indexNum) == VOS_TRUE) {
        portId = DMS_PORT_UART;
    } else {
        AT_ERR_LOG1("AT_HSUART_ConfigBaudRate: indexNum invalid!", indexNum);
        return VOS_ERR;
    }

    if (DMS_PORT_SetBaud(portId, baudRate) != VOS_OK) {
        AT_ERR_LOG("AT_HSUART_ConfigBaudRate: Set Baud failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_HSUART_InitLink(VOS_UINT8 indexNum)
{
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));

    g_atClientTab[indexNum].clientId      = AT_CLIENT_ID_HSUART;
    g_atClientTab[indexNum].used          = AT_CLIENT_USED;
    g_atClientTab[indexNum].userType      = AT_HSUART_USER;
    g_atClientTab[indexNum].portNo        = DMS_PORT_HSUART;
    g_atClientTab[indexNum].indMode       = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;

    return;
}


VOS_UINT32 AT_HSUART_InitPort(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_HSUART;

    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    /* ����HSUART�˿���Ϣӳ�� */
    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_HSUART, ID_AT_MNTN_RESULT_HSUART);

    /* ��ʼ��UART��· */
    AT_HSUART_InitLink(indexNum);

    clientInfo.id         = AT_CLIENT_ID_HSUART;
    clientInfo.type       = DMS_CLIENT_TYPE_HSUART;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_HSUART, &clientInfo);

    AT_WARN_LOG2("AT_HSUART_InitPort ucIndex and reg ret is ", indexNum, ret);

    return ret;
}


VOS_VOID AT_HSUART_ConfigDefaultPara(VOS_VOID)
{
    AT_UartPhyCfg *phyCfg = VOS_NULL_PTR;
    phyCfg                = AT_GetUartPhyCfgInfo();

    /* ����UART������ */
    AT_HSUART_ConfigBaudRate(AT_CLIENT_ID_HSUART, phyCfg->baudRate);

    /* ����UART֡��ʽ */
    AT_HSUART_ConfigCharFrame(AT_CLIENT_ID_HSUART, phyCfg->frame.format, phyCfg->frame.parity);
}

#endif


VOS_INT32 AT_AppComEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;
    VOS_UINT8             loop;

    for (loop = 0; loop < DMS_APP_PORT_SIZE; loop++) {
        indexNum = AT_CLIENT_ID_APP + loop;

        /* ��ն�Ӧ���� */
        (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
        (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

        AT_ConfigTraceMsg(indexNum, (ID_AT_CMD_APP + loop), (ID_AT_MNTN_RESULT_APP + loop));

        g_atClientTab[indexNum].clientId = AT_CLIENT_ID_APP + (VOS_UINT16)loop;

        /* ��д�û����� */
        g_atClientTab[indexNum].portNo        = DMS_PORT_APP + (VOS_UINT16)loop;
        g_atClientTab[indexNum].userType      = AT_APP_USER;
        g_atClientTab[indexNum].used          = AT_CLIENT_USED;
        g_atClientTab[indexNum].indMode       = AT_IND_MODE;
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;

        clientInfo.id         = indexNum;
        clientInfo.type       = DMS_CLIENT_TYPE_COM;
        clientInfo.RcvCmdData = AT_RcvCmdData;

        ret = DMS_PORT_RegClientInfo(g_atClientTab[indexNum].portNo, &clientInfo);
        AT_WARN_LOG2("AT_AppComEst ucIndex and reg ret is ", indexNum, ret);
    }
    return VOS_OK;
}


VOS_UINT32 AT_SockEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_SOCK;

    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_SOCK, ID_AT_MNTN_RESULT_SOCK);

    g_atClientTab[indexNum].clientId      = AT_CLIENT_ID_SOCK;
    g_atClientTab[indexNum].portNo        = DMS_PORT_SOCK;
    g_atClientTab[indexNum].userType      = AT_SOCK_USER;
    g_atClientTab[indexNum].used          = AT_CLIENT_USED;
    g_atClientTab[indexNum].indMode       = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_SOCK;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_SOCK, &clientInfo);

    AT_WARN_LOG2("AT_SockEst ucIndex and reg ret is ", indexNum, ret);

    return ret;
}



VOS_UINT32 AT_UsbNdisEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;

    indexNum = AT_CLIENT_ID_NDIS;

    /* ��ն�Ӧ���� */
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));
    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    AT_ConfigTraceMsg(indexNum, ID_AT_CMD_NDIS, ID_AT_MNTN_RESULT_NDIS);

    /* ��д�û����� */
    g_atClientTab[indexNum].clientId = AT_CLIENT_ID_NDIS;
    g_atClientTab[indexNum].portNo   = DMS_PORT_NCM_CTRL;
    g_atClientTab[indexNum].userType = AT_NDIS_USER;
    g_atClientTab[indexNum].used     = AT_CLIENT_USED;

    /* ���¿��Բ�����д��ǰ��PS_MEMSET�Ѿ���ʼ����ֻΪ�ɿ���� */
    g_atClientTab[indexNum].indMode       = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;

    clientInfo.id         = AT_CLIENT_ID_NDIS;
    clientInfo.type       = DMS_CLIENT_TYPE_COM;
    clientInfo.RcvCmdData = AT_RcvCmdData;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_NCM_CTRL, &clientInfo);

    AT_WARN_LOG2("AT_UsbNdisEst ucIndex and reg ret is ", indexNum, ret);

    return VOS_OK;
}


VOS_UINT32 AT_UsbNdisDataEst(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;

    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    /* NDIS data ͨ��������at���������client idΪ0 */
    clientInfo.id    = 0;
    clientInfo.type  = DMS_CLIENT_TYPE_NDIS_DATA;

    ret = DMS_PORT_RegClientInfo(DMS_PORT_NCM_DATA, &clientInfo);
#if (FEATURE_MULTI_NCM == FEATURE_ON)
    DMS_PORT_RegClientInfo(DMS_PORT_NCM_DATA1, &clientInfo);
    DMS_PORT_RegClientInfo(DMS_PORT_NCM_DATA2, &clientInfo);
    DMS_PORT_RegClientInfo(DMS_PORT_NCM_DATA3, &clientInfo);
#endif

    AT_WARN_LOG1("AT_UsbNdisDataEst reg ret is ", ret);

    return ret;
}


VOS_UINT32 AT_CheckPcuiCtrlConcurrent(VOS_UINT8 indexOne, VOS_UINT8 indexTwo)
{
    VOS_UINT32 userFlg1;
    VOS_UINT32 userFlg2;

    userFlg1 = AT_CheckUserType(indexOne, AT_USBCOM_USER);
    userFlg1 |= AT_CheckUserType(indexOne, AT_CTR_USER);
    userFlg1 |= AT_CheckUserType(indexOne, AT_PCUI2_USER);

    if (userFlg1 != VOS_TRUE) {
        return VOS_FALSE;
    }

    userFlg2 = AT_CheckUserType(indexTwo, AT_USBCOM_USER);
    userFlg2 |= AT_CheckUserType(indexTwo, AT_CTR_USER);
    userFlg2 |= AT_CheckUserType(indexTwo, AT_PCUI2_USER);

    if (userFlg2 != VOS_TRUE) {
        return VOS_FALSE;
    }

    if (g_atClientTab[indexOne].userType != g_atClientTab[indexTwo].userType) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_IsConcurrentPorts(VOS_UINT8 indexOne, VOS_UINT8 indexTwo)
{
    VOS_UINT32 appUserFlg1;
    VOS_UINT32 appUserFlg2;

    /* ͬһ��ͨ����֧�ֲ���������㺯����֤ */
    appUserFlg1 = AT_CheckAppUser(indexOne);
    appUserFlg2 = AT_CheckAppUser(indexTwo);

    /* ͨ��1��APPͨ��,ͨ��2Ҳ��APPͨ�� */
    if (appUserFlg1 == VOS_TRUE) {
        if (appUserFlg2 == VOS_TRUE) {
            return VOS_TRUE;
        }
    }

    /* PCUI��CTRL�ڲ����жϣ����������� */
    if (AT_GetPcuiCtrlConcurrentFlag() == VOS_TRUE) {
        if (AT_CheckPcuiCtrlConcurrent(indexOne, indexTwo) == VOS_TRUE) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_CheckAppUser(VOS_UINT8 indexNum)
{
    if (g_atClientTab[indexNum].userType != AT_APP_USER) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_CheckHsUartUser(AT_ClientIdUint16 indexNum)
{
#if (FEATURE_AT_HSUART == FEATURE_ON)
    if (g_atClientTab[indexNum].userType == AT_HSUART_USER) {
        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}


VOS_UINT32 AT_CheckModemUser(VOS_UINT8 indexNum)
{
    if (g_atClientTab[indexNum].userType != AT_MODEM_USER) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_CheckUserType(VOS_UINT8 indexNum, AT_USER_TYPE userType)
{
    if (userType != g_atClientTab[indexNum].userType) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID AT_InitFcMap(VOS_VOID)
{
    VOS_UINT8 loop;

    /* ��ʼ��g_fcIdMaptoFcPri */
    for (loop = 0; loop < FC_ID_BUTT; loop++) {
        g_fcIdMaptoFcPri[loop].used      = VOS_FALSE;
        g_fcIdMaptoFcPri[loop].fcPri     = FC_PRI_BUTT;
        g_fcIdMaptoFcPri[loop].rabIdMask = 0;
        g_fcIdMaptoFcPri[loop].modemId   = MODEM_ID_BUTT;
    }
}


TAF_UINT32 At_SendCmdMsg(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len, TAF_UINT8 type)
{
    AT_Msg           *msg           = TAF_NULL_PTR;
    AT_CmdMsgNumCtrl *msgNumCtrlCtx = VOS_NULL_PTR;
    VOS_UINT_PTR      tmpAddr;
    VOS_UINT32        length;
    VOS_ULONG         lockLevel;
    errno_t           memResult;
    ModemIdUint16 modemId;
    msgNumCtrlCtx = AT_GetMsgNumCtrlCtxAddr();

    if (data == VOS_NULL_PTR) {
        AT_WARN_LOG("At_SendCmdMsg :pData is null ptr!");
        return AT_FAILURE;
    }

    if (len == 0) {
        AT_WARN_LOG("At_SendCmdMsg ulLength = 0");
        return AT_FAILURE;
    }

    if (len > AT_COM_BUFF_LEN) {
        AT_WARN_LOG("At_SendCmdMsg ulLength > AT_COM_BUFF_LEN");
        return AT_FAILURE;
    }

    /* �����Զ����ITEM����4���ֽ� */
    length = AT_GetAtMsgStruMsgLength(len);
    msg = (AT_Msg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == TAF_NULL_PTR) {
        AT_ERR_LOG("At_SendCmdMsg:ERROR:Alloc Msg");
        return AT_FAILURE;
    }

    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, length);

    if (type == AT_NORMAL_TYPE_MSG) {
        if (msgNumCtrlCtx->msgCount > AT_MAX_MSG_NUM) {
            /*lint -save -e516 */
            /* �ͷŷ�����ڴ�ռ� */
            PS_FREE_MSG(WUEPS_PID_AT, msg);
            /*lint -restore */

            return AT_FAILURE;
        }

        lockLevel = 0;
        /*lint -e571*/
        VOS_SpinLockIntLock(&(msgNumCtrlCtx->spinLock), lockLevel);
        /*lint +e571*/

        msgNumCtrlCtx->msgCount++;

        VOS_SpinUnlockIntUnlock(&(msgNumCtrlCtx->spinLock), lockLevel);
    }

    if (type == AT_COMBIN_BLOCK_MSG) {
        msg->msgId = ID_AT_COMBIN_BLOCK_CMD;
    } else {
        msg->msgId = AT_GetCmdMsgID(indexNum);
    }

    msg->type     = type;     /* ���� */
    msg->indexNum = indexNum; /* ���� */
    msg->len      = len;      /* ���� */

    modemId = MODEM_ID_0;
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        modemId = MODEM_ID_0;
    }

    msg->modemId = (VOS_UINT8)modemId;
    /* �汾��Ϣ */
    msg->versionId    = 0xAA;
    msg->filterAtType = (VOS_UINT8)g_logPrivacyAtCmd;

    (VOS_VOID)memset_s(msg->value, sizeof(msg->value), 0x00, sizeof(msg->value));

    msg->userType = AT_GetUserTypeFromIndex(indexNum);

    /* ��д����Ϣ���� */
    tmpAddr   = (VOS_UINT_PTR)(msg->value);
    memResult = memcpy_s((VOS_UINT8 *)tmpAddr, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    /* ������Ϣ��AT_PID; */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != 0) {
        AT_ERR_LOG("At_SendCmdMsg:ERROR:VOS_SendMsg");

        /* ������Ϣ����ʧ��ʱ���ᴥ��������λ���ʴ˴�����ulMsgCount--���� */

        return AT_FAILURE;
    }
    return AT_SUCCESS;
}


VOS_UINT32 AT_IsApPort(VOS_UINT8 indexNum)
{
    VOS_UINT32 vcomUserFlg;
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    if (g_atDebugInfo.unCheckApPortFlg == 0) {
        /* ��ʼ�� */
        systemAppConfig = AT_GetSystemAppConfigAddr();
        vcomUserFlg     = AT_CheckAppUser(indexNum);

        if (*systemAppConfig == SYSTEM_APP_ANDROID) {
            /* ������ֻ���̬����Ҫ�ж�HSIC�˿ڣ�MUX�˿ڣ�VCOM�˿� */
            if (vcomUserFlg == VOS_FALSE) {
                return VOS_FALSE;
            }
#if (FEATURE_PHONE_SC == FEATURE_ON)
        } else if (*systemAppConfig == SYSTEM_APP_MP) {
            if (vcomUserFlg == VOS_FALSE) {
                return VOS_FALSE;
            }
#endif
        } else {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)

STATIC VOS_INT AT_ProcCCpuResetBefore(VOS_VOID)
{
    AT_Msg *msg = VOS_NULL_PTR;

    AT_PR_LOGI("enter, %u", VOS_GetSlice());

    /* ���ô��ڸ�λǰ�ı�־ */
    AT_SetResetFlag(VOS_TRUE);

    dms_reset_modem_status();

    /* ���TAFAGENT���е��ź��� */
    TAF_AGENT_ClearAllSem();

    /* ������Ϣ */
    /*lint -save -e516 */
    msg = (AT_Msg *)TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, sizeof(AT_Msg));
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_PR_LOGE("alloc msg fail, %u", VOS_GetSlice());
        return VOS_ERROR;
    }

    /* ��д��Ϣͷ */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH);
    msg->type = ID_CCPU_AT_RESET_START_IND;

    msg->msgId = ID_AT_COMM_CCPU_RESET_START;

    /* ����Ϣ */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_PR_LOGE("send msg fail, %u", VOS_GetSlice());
        return VOS_ERROR;
    }

    /* �ȴ��ظ��ź�����ʼΪ��״̬���ȴ���Ϣ��������ź��������� */
    if (VOS_SmP(AT_GetResetSem(), AT_RESET_TIMEOUT_LEN) != VOS_OK) {
        AT_PR_LOGE("VOS_SmP fail, %u", VOS_GetSlice());
        AT_DBG_LOCK_BINARY_SEM_FAIL_NUM(1);

        return VOS_ERROR;
    }

    /* ��¼��λǰ�Ĵ��� */
    AT_DBG_SAVE_CCPU_RESET_BEFORE_NUM(1);

    AT_PR_LOGI("succ, %u", VOS_GetSlice());

    return VOS_OK;
}


STATIC VOS_INT AT_ProcCCpuResetAfter(VOS_VOID)
{
    AT_Msg *msg = VOS_NULL_PTR;

    AT_PR_LOGI("enter, %u", VOS_GetSlice());

    /* ������Ϣ */
    /*lint -save -e516 */
    msg = (AT_Msg *)TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, sizeof(AT_Msg));
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        AT_PR_LOGE("alloc msg fail, %u", VOS_GetSlice());
        return VOS_ERROR;
    }

    /* ��д��Ϣͷ */
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH);
    msg->type = ID_CCPU_AT_RESET_END_IND;

    msg->msgId = ID_AT_COMM_CCPU_RESET_END;

    /* ����Ϣ */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        AT_PR_LOGE("send msg fail, %u", VOS_GetSlice());
        return VOS_ERROR;
    }

    /* ��¼��λ��Ĵ��� */
    AT_DBG_SAVE_CCPU_RESET_AFTER_NUM(1);

    AT_PR_LOGI("succ, %u", VOS_GetSlice());

    return VOS_OK;
}


STATIC VOS_INT AT_CCpuResetCallback(drv_reset_cb_moment_e param, VOS_INT iUserData)
{
    /* ��λǰ */
    if (param == MDRV_RESET_CB_BEFORE) {
        return AT_ProcCCpuResetBefore();
    }
    /* ��λ�� */
    else if (param == MDRV_RESET_CB_AFTER) {
        return AT_ProcCCpuResetAfter();
    } else {
        return VOS_ERROR;
    }
}


STATIC VOS_INT AT_HifiResetCallback(enum DRV_RESET_CALLCBFUN_MOMENT param, VOS_INT iUserData)
{
    AT_Msg *msg = VOS_NULL_PTR;

    /* ����Ϊ0��ʾ��λǰ���� */
    if (param == DRV_RESET_CALLCBFUN_RESET_BEFORE) {
        AT_PR_LOGI("before reset enter, %u", VOS_GetSlice());
        /* ������Ϣ */
        /*lint -save -e516 */
        msg = (AT_Msg *)TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, sizeof(AT_Msg));
        /*lint -restore */
        if (msg == VOS_NULL_PTR) {
            AT_PR_LOGE("before reset alloc msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }

        /* ��д��Ϣͷ */
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH);
        msg->type = ID_HIFI_AT_RESET_BEGIN_IND;

        msg->msgId = ID_AT_COMM_HIFI_RESET_START;

        /* ����Ϣ */
        if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
            AT_PR_LOGE("after reset alloc msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }

        return VOS_OK;
    }
    /* ��λ�� */
    else if (param == DRV_RESET_CALLCBFUN_RESET_AFTER) {
        AT_PR_LOGI("after reset enter, %u", VOS_GetSlice());
        /* ������Ϣ */
        /*lint -save -e516 */
        msg = (AT_Msg *)TAF_AllocMsgWithHeaderLen(WUEPS_PID_AT, sizeof(AT_Msg));
        /*lint -restore */
        if (msg == VOS_NULL_PTR) {
            AT_PR_LOGE("after reset alloc msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }

        /* ��д��Ϣͷ */
        TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, WUEPS_PID_AT, sizeof(AT_Msg) - VOS_MSG_HEAD_LENGTH);
        msg->type = ID_HIFI_AT_RESET_END_IND;

        msg->msgId = ID_AT_COMM_HIFI_RESET_END;

        /* ����Ϣ */
        if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
            AT_PR_LOGE("after reset send msg fail, %u", VOS_GetSlice());
            return VOS_ERROR;
        }
        return VOS_OK;
    } else {
        return VOS_ERROR;
    }
}


VOS_VOID AT_RegResetCallbackFunc(VOS_VOID)
{
    /* ������ע��ص�����������C�˵�����λ�Ĵ��� */
    AT_RegisterSysbootResetNotify(NAS_AT_FUNC_PROC_NAME, AT_CCpuResetCallback, 0, ACPU_RESET_PRIORITY_AT);

    /* ������ע��ص�����������HIFI������λ�Ĵ��� */
    hifireset_regcbfunc(NAS_AT_FUNC_PROC_NAME, AT_HifiResetCallback, 0, ACPU_RESET_PRIORITY_AT);
}
#endif

VOS_VOID AT_ModemEnableCB(VOS_UINT8 indexNum, VOS_UINT32 enable)
{
    AT_ClientManage        *clientManage = VOS_NULL_PTR;
    DMS_PortDataModeUint8   dataMode;

    clientManage = AT_GetClientManage(indexNum);
    dataMode     = DMS_PORT_GetDataMode(clientManage->portNo);

    /*
     * �豸Ĭ�ϴ�����Ч״̬�������ݾ�ͨ�����ص����գ�
     * �� �豸ʧЧʱ�����ݵ�ǰ״̬��֪ͨPPP���紦������̬��
     *    ��֪ͨATȥ����PDP.
     */
    if (enable == PS_FALSE) {
        if (dataMode == DMS_PORT_DATA_PPP) {
            AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_REQ);

            /* ��ԭ�ȿ��������أ�����ֹͣ���� */
            if (DMS_PORT_IsCtsDeasserted(clientManage->portNo) == VOS_TRUE) {
                /* ͨ������cts��ֹͣ���� */
                DMS_PORT_AssertCts(clientManage->portNo);
            }

            /* �Ͽ����� */
            if (TAF_PS_PPP_DIAL_DOWN(WUEPS_PID_AT, AT_PS_BuildExClientId(clientManage->clientId), 0,
                               clientManage->cid) != VOS_OK) {
                AT_ERR_LOG("AT_ModemEnableCB: TAF_PS_CallEnd failed in <AT_PPP_DATA_MODE>!");
                return;
            }
        } else if (dataMode == DMS_PORT_DATA_PPP_RAW) {
            AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);

            /* ��ԭ�ȿ��������أ�����ֹͣ���� */
            if (DMS_PORT_IsCtsDeasserted(clientManage->portNo) == VOS_TRUE) {
                /* ͨ������cts��ֹͣ���� */
                DMS_PORT_AssertCts(clientManage->portNo);
            }

            /* �Ͽ����� */
            if (TAF_PS_PPP_DIAL_DOWN(WUEPS_PID_AT, AT_PS_BuildExClientId(clientManage->clientId), 0,
                               clientManage->cid) != VOS_OK) {
                AT_ERR_LOG("AT_ModemEnableCB: TAF_PS_CallEnd failed in <AT_IP_DATA_MODE>!");
                return;
            }
        } else {
            /* �յ�else��֧������PCLINT���� */
        }

        /* ֹͣ��ʱ�� */
        AT_StopRelTimer(indexNum, &clientManage->hTimer);

        /*
         * �ܽ��ź��޸ĺ�At_ModemRelInd����ֻ������USB���γ���ʱ����ã�
         * Ϊ�˴ﵽModem��always-on��Ŀ�ģ���ʱ��Ҫ����AT��·��״̬Ǩ��
         * ������������״̬:
         */
        DMS_PORT_ResumeCmdMode(clientManage->portNo);
        clientManage->indMode       = AT_IND_MODE;
        clientManage->cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;
    }
}

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)

VOS_VOID AT_CMUX_InitLink(VOS_UINT8 indexNum)
{
    VOS_UINT8 offset = 0;

    if (indexNum < AT_CLIENT_ID_CMUXAT) {
        return;
    }

    offset = indexNum - AT_CLIENT_ID_CMUXAT;
    (VOS_VOID)memset_s(&g_atClientTab[indexNum], sizeof(AT_ClientManage), 0x00, sizeof(AT_ClientManage));

    g_atClientTab[indexNum].clientId      = AT_CLIENT_ID_CMUXAT + offset;
    g_atClientTab[indexNum].used          = AT_CLIENT_USED;
    g_atClientTab[indexNum].userType      = AT_CMUXAT_USER + offset;
    g_atClientTab[indexNum].portNo        = DMS_PORT_CMUXAT + offset;
    g_atClientTab[indexNum].indMode       = AT_IND_MODE;
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;
    /*��ʼ����ʱ��usDataId/usCtrlId��usClientId��ͬ*/
    g_atClientTab[indexNum].dataId = indexNum;
    g_atClientTab[indexNum].ctrlId = indexNum;

    return;
}


VOS_VOID AT_CMUX_InitPort(VOS_VOID)
{
    struct DMS_ClientInfo clientInfo;
    VOS_UINT32            ret;
    VOS_UINT8             indexNum;
    VOS_UINT8             i = 0;

    indexNum = AT_CLIENT_ID_CMUXAT;

    (VOS_VOID)memset_s(&clientInfo, sizeof(struct DMS_ClientInfo), 0x00, sizeof(struct DMS_ClientInfo));

    /* ���ö˿���Ϣӳ��--TO DO */
    for (i = 0; i < MAX_CMUX_PORT_NUM; i++) {
        indexNum = AT_CLIENT_ID_CMUXAT + i;
        /* ����CMUX�˿���Ϣӳ�� */
        AT_ConfigTraceMsg(indexNum, ID_AT_CMD_CMUXAT + i,
                                        ID_AT_MNTN_RESULT_CMUXAT + i);
        /* ��ʼ����· */
        AT_CMUX_InitLink(indexNum);

        clientInfo.id         = AT_CLIENT_ID_CMUXAT + i;
        clientInfo.type       = DMS_CLIENT_TYPE_CMUX;
        clientInfo.RcvCmdData = AT_RcvCmdData;

        ret = DMS_PORT_RegClientInfo(DMS_PORT_CMUXAT + i, &clientInfo);

        AT_WARN_LOG2("AT_CMUX_InitPort ucIndex and reg ret is ", indexNum, ret);
    }
    return;
}


VOS_UINT32 AT_CheckCmuxUser(VOS_UINT8 indexNum)
{
    if (g_cmuxOpen == VOS_FALSE) {
        return VOS_FALSE;
    }

    if ((g_atClientTab[indexNum].userType == AT_CMUXAT_USER) ||
        (g_atClientTab[indexNum].userType == AT_CMUXMDM_USER)||
        (g_atClientTab[indexNum].userType == AT_CMUXEXT_USER)||
        (g_atClientTab[indexNum].userType == AT_CMUXGPS_USER)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 AT_CMUX_CheckUartUser(VOS_UINT8 indexNum)
{
    if (g_atClientTab[indexNum].userType == AT_UART_USER) {
        return VOS_TRUE;
    }
    return VOS_FALSE;
}

#endif

