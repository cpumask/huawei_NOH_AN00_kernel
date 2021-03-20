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

/* ***************************************************************************** */

/* PROJECT   : MSP_CPV700R001C01 */

/* SUBSYSTEM : AT */

/* MODULE    : LTE */

/* OWNER     : */

/* ***************************************************************************** */

/*lint -save -e537 -e958 -e713 -e718 -e746 -e740 -e516 -e830 -e533*/
#include "at_lte_common.h"

#include "securec.h"

#include "osm.h"
#include "msp_at.h"
#include "mdrv_rfile.h"

#include "mdrv_om.h"
#include "mn_comm_api.h"
/* msp��ش������ڵ����mdrv_diag_system_common.h�ж��� */
#include "at_mdrv_interface.h"

/*lint -e767 ԭ��:Log��ӡ*/
#define THIS_FILE_ID MSP_FILE_ID_AT_LTE_COMMON_C
/*lint +e767 */
#define AT_SEND_DATA_BUFF_INDEX 2

/* ���ͻ����� */

AT_SEND_DataBuffer g_atSendData = {
    {0},
};
VOS_UINT8 *g_atSndCodeAddr = &g_atSendData.buffer[AT_SEND_DATA_BUFF_INDEX];

static const AT_ERROR_CodeTable g_errCodeTable[] = {
    { APP_SUCCESS, AT_OK },
    { APP_FAILURE, AT_ERROR },
    { APP_ERR_CAPABILITY_ERROR, AT_CME_OPERATION_NOT_SUPPORTED },
    { APP_ERR_USIM_SIM_CARD_NOTEXIST, AT_CME_SIM_NOT_INSERTED },
    { APP_ERR_NEED_PIN1, AT_CME_SIM_PIN_REQUIRED },
    { APP_ERR_NEED_PUK1, AT_CME_SIM_PUK_REQUIRED },
    { APP_ERR_USIM_SIM_INVALIDATION, AT_CME_SIM_FAILURE },
    { APP_ERR_ID_INVALID, AT_CME_INVALID_INDEX },
    { APP_ERR_TIME_OUT, AT_CME_NETWORK_TIMEOUT },
    { APP_ERR_PARA_ERROR, AT_CME_INCORRECT_PARAMETERS },
    { APP_ERR_NULL_PTR, AT_CME_INCORRECT_PARAMETERS },
    { APP_ERR_NUM_VALUE_INVALID, AT_CME_INCORRECT_PARAMETERS },
    { APP_ERR_NUM_LEN_INVALID, AT_CME_INCORRECT_PARAMETERS },

    /* ���Ŵ��� */

    { APP_ERR_SM_APN_LEN_ILLEGAL, AT_CME_APN_LEN_ILLEGAL },
    { APP_ERR_SM_APN_SYNTACTICAL_ERROR, AT_CME_APN_SYNTACTICAL_ERROR },
    { APP_ERR_SM_SET_APN_BEFORE_SET_AUTH, AT_CME_SET_APN_BEFORE_SET_AUTH },
    { APP_ERR_SM_AUTH_TYPE_ILLEGAL, AT_CME_AUTH_TYPE_ILLEGAL },
    { APP_ERR_SM_USER_NAME_TOO_LONG, AT_CME_USER_NAME_TOO_LONG },
    { APP_ERR_SM_USER_PASSWORD_TOO_LONG, AT_CME_USER_PASSWORD_TOO_LONG },
    { APP_ERR_SM_ACCESS_NUM_TOO_LONG, AT_CME_ACCESS_NUM_TOO_LONG },
    { APP_ERR_SM_CALL_CID_IN_OPERATION, AT_CME_CALL_CID_IN_OPERATION },
    { APP_ERR_SM_BEARER_TYPE_NOT_DEFAULT, AT_CME_BEARER_TYPE_NOT_DEFAULT },
    { APP_ERR_SM_CALL_CID_INVALID, AT_CME_CALL_CID_INVALID },
    { APP_ERR_SM_CALL_CID_ACTIVE, AT_CME_CALL_CID_ACTIVE },
    { APP_ERR_SM_BEARER_TYPE_ILLEGAL, AT_CME_BEARER_TYPE_ILLEGAL },
    { APP_ERR_SM_MUST_EXIST_DEFAULT_TYPE_CID, AT_CME_MUST_EXIST_DEFAULT_TYPE_CID },
    { APP_ERR_SM_PDN_TYPE_ILLEGAL, AT_CME_PDN_TYPE_ILLEGAL },
    { APP_ERR_SM_IPV4_ADDR_ALLOC_TYPE_ILLEGAL, AT_CME_IPV4_ADDR_ALLOC_TYPE_ILLEGAL },
    { APP_ERR_SM_LINK_CID_INVALID, AT_CME_LINK_CID_INVALID },

    /* װ�� ERROR CODE */
    { ERR_MSP_SUCCESS, AT_OK },
    { ERR_MSP_FAILURE, AT_ERROR },
    { ERR_MSP_UNKNOWN, AT_DEVICE_ERR_UNKNOWN },
    { ERR_MSP_INVALID_PARAMETER, AT_DEVICE_INVALID_PARAMETERS },
    { ERR_MSP_INVALID_OP, AT_DEVICE_INVALID_OP },
};

#define FTM_DUMP_FILE_NAME_LENGTH 50
#define FTM_DUMP_FILE_MAX_NUM 4
#define FTM_DUMP_SIZE_12K 0x3000

#define FTM_DUMP_LOG_PATH MODEM_LOG_ROOT "/DrvLog/Ftm/"
#define FTM_DUMP_HEAD "ftm_"

/*lint -e959 */
/*lint -e958 */

struct linux_dirent {
    unsigned long  ino;
    unsigned long  off;
    unsigned short reclen;
    char           name[1];
};
/*lint +e959 */
/*lint +e958 */

/* ***************************************************************************** */

/* ��������: atChgErrorCode */

/* ��������: ������ģ�鷵�صĴ�����ת����AT�Ĵ����� */

/*  */

/* ����˵��: */

/*   usTafErrorCode [in] ������ */

/*  */

/* �� �� ֵ: */

/*    ����AT�Ĵ����� */

/*  */

/* ����Ҫ��: TODO: ... */

/* ���þ���: TODO: ... */


/* ***************************************************************************** */

static VOS_UINT32 ChgErrorCode(VOS_UINT32 tafErrorCode)
{
    VOS_UINT32 rtn    = AT_CME_UNKNOWN;
    VOS_UINT32 i      = 0;
    VOS_UINT32 errNum = sizeof(g_errCodeTable) / sizeof(AT_ERROR_CodeTable);

    for (i = 0; i < errNum; i++) {
        if (tafErrorCode == g_errCodeTable[i].srcError) {
            rtn = g_errCodeTable[i].atError;

            return rtn;
        }
    }

    return AT_ERROR;
}

/* ***************************************************************************** */

/* ��������: CmdErrProc */

/* ��������: ������ת����AT�ظ��ϱ� */

/*  */

/* ����˵��: */

/*   ucClientId [in] �ͻ���ID */

/*   ulErrCode [in]  ������ */

/*   usBufLen [in]   �ظ���Ϣ */

/*   pucBuf [in]     �ظ���Ϣ���� */

/*  */

/* �� �� ֵ: */

/*    �� */

/*  */

/* ����Ҫ��: TODO: ... */

/* ���þ���: TODO: ... */


/* ***************************************************************************** */

VOS_VOID CmdErrProc(VOS_UINT8 clientId, VOS_UINT32 errCode, VOS_UINT16 bufLen, VOS_UINT8 *buf)
{
    errno_t    memResult;
    VOS_UINT32 result       = 0;
    g_atSendDataBuff.bufLen = bufLen;
    if ((buf != VOS_NULL_PTR) && (bufLen != 0) &&
        (bufLen <= (AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET))) {
        /* �ĳ��������ƥ�亯�� At_FormatResultData */
        memResult = memcpy_s(g_atSndCodeAddress, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET,
                             buf, bufLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_MAX_LEN + AT_CMD_PADDING_LEN - AT_SEND_DATA_BUFF_OFFSET, bufLen);
    }

    if (errCode != ERR_MSP_SUCCESS) {
        result = ChgErrorCode(errCode);

        At_FormatResultData(clientId, result);
    } else {
        At_FormatResultData(clientId, AT_OK);
    }

    return;
}

/* ***************************************************************************** */

/* ��������: atSendFtmDataMsg */

/* ��������: ATģ���FTM ģ�鷢����Ϣ */

/*  */

/* ����˵��: */

/*   TaskId [in] ����PID */

/*   MsgId  [in] ��ϢID */

/*   ulClientId [in] �˿ں� */

/*   pData  [in] ������ʼ */

/*   uLen   [in] ���ݳ��� */

/* �� �� ֵ: */

/*    ERR_MSP_SUCCESS�ɹ� */

/*    ��ERR_MSP_SUCCESSʧ�� */


/* ***************************************************************************** */

VOS_UINT32 atSendFtmDataMsg(VOS_UINT32 taskId, VOS_UINT32 msgId, VOS_UINT32 clientId, const VOS_UINT8 *data,
                            VOS_UINT32 uLen)
{
    errno_t        memResult;
    VOS_UINT32     u32Ret   = ERR_MSP_UNKNOWN;
    AT_FW_DataMsg *msgBlock = NULL;

    msgBlock = (AT_FW_DataMsg *)VOS_AllocMsg(WUEPS_PID_AT, (sizeof(AT_FW_DataMsg) + uLen - VOS_MSG_HEAD_LENGTH));

    if (msgBlock != VOS_NULL_PTR) {
        TAF_CfgMsgHdr((MsgBlock *)msgBlock, WUEPS_PID_AT, AT_GetDestPid((MN_CLIENT_ID_T)clientId, taskId),
                      sizeof(AT_FW_DataMsg) + uLen - VOS_MSG_HEAD_LENGTH);

        msgBlock->msgId    = msgId;
        msgBlock->clientId = clientId;
        msgBlock->len      = uLen;

        if ((data != VOS_NULL_PTR) && (uLen != 0)) {
            memResult = memcpy_s(msgBlock->context, uLen, data, uLen);
            TAF_MEM_CHK_RTN_VAL(memResult, uLen, uLen);
        }

        u32Ret = TAF_TraceAndSendMsg(VOS_GET_SENDER_ID(msgBlock), msgBlock);

        if (u32Ret == VOS_OK) {
            u32Ret = ERR_MSP_SUCCESS;
        }
    }

    return u32Ret;
}


VOS_UINT32 atSendL4aDataMsg(MN_CLIENT_ID_T clientId, VOS_UINT32 taskId, VOS_UINT32 msgId, const VOS_UINT8 *data,
                            VOS_UINT32 uLen)
{
    errno_t       memResult;
    VOS_UINT32    u32Ret   = 0;
    L4A_COMM_Req *msgBlock = VOS_NULL_PTR;

    if (uLen <= VOS_MSG_HEAD_LENGTH) {
        AT_ERR_LOG("msgBlock cannot malloc space !");
        return AT_ERROR;
    }
    msgBlock = (L4A_COMM_Req *)VOS_AllocMsg(WUEPS_PID_AT, (uLen - VOS_MSG_HEAD_LENGTH));

    if (msgBlock != VOS_NULL_PTR) {
        memResult = memset_s((VOS_CHAR *)msgBlock + VOS_MSG_HEAD_LENGTH, uLen - VOS_MSG_HEAD_LENGTH, 0x00,
                             uLen - VOS_MSG_HEAD_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, uLen - VOS_MSG_HEAD_LENGTH, uLen - VOS_MSG_HEAD_LENGTH);
        memResult = memcpy_s(msgBlock, uLen, data, uLen);
        TAF_MEM_CHK_RTN_VAL(memResult, uLen, uLen);
        VOS_SET_SENDER_ID(msgBlock, WUEPS_PID_AT);
        VOS_SET_RECEIVER_ID(msgBlock, AT_GetDestPid(clientId, taskId));
        VOS_SET_MSG_LEN(msgBlock, (uLen - VOS_MSG_HEAD_LENGTH));
        msgBlock->msgId = msgId;

        u32Ret = TAF_TraceAndSendMsg(VOS_GET_SENDER_ID(msgBlock), msgBlock);
    }

    return u32Ret;
}

/* ������LTE��TDS˽��AT���� */

static const AT_ParCmdElement g_atTlCmdTbl[] = {
    { AT_CMD_LWCLASH,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryLwclashPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
      (VOS_UINT8 *)"^LWCLASH", VOS_NULL_PTR },

    { AT_CMD_RADVER,
      AT_SetRadverPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
      (VOS_UINT8 *)"^RADVER", (VOS_UINT8 *)"(0-3),(4-16)" },

    { AT_CMD_CASCELLINFO,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryCaScellInfoPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
      (VOS_UINT8 *)"^CASCELLINFO", VOS_NULL_PTR },

    { AT_CMD_LCACELL,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryLcacellPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_NO_LIMITED,
      (VOS_UINT8 *)"^LCACELL", VOS_NULL_PTR }
};

/*lint -restore*/


VOS_UINT32 At_RegisterTLCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atTlCmdTbl, sizeof(g_atTlCmdTbl) / sizeof(g_atTlCmdTbl[0]));
}
