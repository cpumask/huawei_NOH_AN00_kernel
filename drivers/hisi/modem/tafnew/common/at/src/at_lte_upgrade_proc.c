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
/*
 * PROJECT   : MSP
 * SUBSYSTEM : MSP
 * MODULE    : AT
 * OWNER     :
 */
/*lint --e{537,322,7,958,732,813,718,746,734}*/
#include "at_lte_upgrade_proc.h"
#include <osm.h>
#include "securec.h"
#include "msp_upgrade_proc.h"
#include "at_lte_common.h"
#include "ATCmdProc.h"
#include "at_mdrv_interface.h"
#include "nv_stru_sys.h"
#include "nv_stru_lps.h"
#include "nv_id_msp.h"
/*lint -e767 原因:Log打印*/
#define THIS_FILE_ID MSP_FILE_ID_AT_LTE_UPGRADE_PROC_C
/*lint +e767 */

#define AT_NV_IMEI_LEN 15
#define AT_QRY_AUTHORITY_VER_LEN 5
#define AT_DEC_MAX_NUM 9

VOS_UINT32 g_atNvBackupFlag = 0;

VOS_UINT32 At_GetNvRevertState(VOS_VOID)
{
    VOS_UINT32 ret;
    VOS_UINT16 resumeFlag = 0;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ID_DRV_RESUME_FLAG, &resumeFlag, sizeof(VOS_UINT16)); /* to do */
    if (ret) {
        return ret;
    }
    if (resumeFlag != 0) {
        return ((VOS_UINT32)-1);
    }
    return 0;
}

VOS_UINT32 At_GetNvAuthorityVer(VOS_UINT32 *pdata)
{
    VOS_UINT32 rst;

    if (pdata == NULL) {
        return ((VOS_UINT32)-1);
    }

    rst = TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_AUTHORITY_VER, pdata, sizeof(VOS_UINT32));

    if (rst != 0) {
        *pdata = 0;
    }

    return 0;
}

VOS_UINT32 At_GetImei(VOS_CHAR szimei[NV_ITEM_IMEI_SIZE])
{
    VOS_UINT32 ret;
    VOS_UINT32 uslen;
    VOS_UINT32 subscript                   = 0;
    VOS_CHAR   checkdata                   = 0;
    VOS_CHAR   auctemp[AT_NV_IMEI_LEN + 1] = {0};

    uslen = AT_NV_IMEI_LEN + 1;

    ret = TAF_ACORE_NV_READ(MODEM_ID_0, 0, auctemp, uslen);

    if (ret != 0) {
        return ret;
    } else {
        /* 一次赋值为subscript和subscript + 1, 所以循环加2 */
        for (subscript = 0; subscript < (AT_NV_IMEI_LEN - 1); subscript += 2) {
            checkdata += (VOS_CHAR)(((auctemp[subscript]) +
                ((auctemp[(VOS_UINT32)(subscript + 1)] * AT_DOUBLE_LENGTH) / AT_DECIMAL_BASE_NUM)) +
                ((auctemp[(VOS_UINT32)(subscript + 1)] * AT_DOUBLE_LENGTH) % AT_DECIMAL_BASE_NUM));
        }
        checkdata = (AT_DECIMAL_BASE_NUM - (checkdata % AT_DECIMAL_BASE_NUM)) % AT_DECIMAL_BASE_NUM;

        for (subscript = 0; subscript < uslen; subscript++) {
            *(szimei + subscript) = *(auctemp + subscript) + 0x30; /* 字符转换 */
        }

        szimei[AT_NV_IMEI_LEN - 1] = checkdata + 0x30;
        szimei[AT_NV_IMEI_LEN]     = 0;
    }

    return 0;
}

/* NV备份命令 */
VOS_UINT32 atSetNVBackup(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;
    g_atSendDataBuff.bufLen = 0;

    AT_PR_LOGI("Rcv Msg");

#if (VOS_OS_VER != VOS_WIN32)
    rst = TAF_ACORE_NV_UPGRADE_BACKUP(EN_NVM_BACKUP_FILE);
    AT_PR_LOGI("Call interface success!");
#else
    rst = VOS_OK;
#endif
    if (rst != VOS_OK) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }
    g_atNvBackupFlag++;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%d\r", rst);

    return AT_OK;
}

VOS_UINT32 atQryAuthorityVer(VOS_UINT8 clientId)
{
    errno_t    memResult;
    VOS_UINT32 rst;
    VOS_UINT32 authorityVer = 0;
    VOS_UINT8 ver[AT_QRY_AUTHORITY_VER_LEN] = {0};
    VOS_UINT32 i = 0;

    rst = At_GetNvAuthorityVer(&authorityVer);
    if (rst != ERR_MSP_SUCCESS) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }

    memResult = memcpy_s(ver, sizeof(ver), &authorityVer, sizeof(VOS_UINT32));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ver), sizeof(VOS_UINT32));

    for (i = 0; i < (AT_QRY_AUTHORITY_VER_LEN - 1); i++) {
        ver[i] = (ver[i] > AT_DEC_MAX_NUM ? 0 : ver[i]) + '0';
    }

    g_atSendDataBuff.bufLen = 0;
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%s\r", ver);
    return AT_OK;
}

VOS_UINT32 atQryAuthorityID(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;
    VOS_UINT8  data[NV_ITEM_IMEI_SIZE] = {0};
    g_atSendDataBuff.bufLen            = 0;

    rst = At_GetImei((VOS_CHAR *)data);
    if (rst != ERR_MSP_SUCCESS) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%s\r", data);

    return AT_OK;
}

// 自动恢复状态查询
VOS_UINT32 atSetNVRstSTTS(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;
    g_atSendDataBuff.bufLen = 0;

    rst = At_GetNvRevertState();
    if (rst != ERR_MSP_SUCCESS) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%d\r", rst);

    return AT_OK;
}

VOS_UINT32 atSetNVFactoryRestore(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;

    /* 因为MODEN拨号会下发该命令，所以在moden口不进行NV恢复，直接返回OK */
    if (g_atClientTab[clientId].userType == AT_MODEM_USER) {
        return AT_OK;
    }

    rst = TAF_ACORE_NV_FREVERT_FNV();
    if (rst != VOS_OK) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%d\r", rst);
    return AT_OK;
}

VOS_UINT32 atSetNVFactoryBack(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;

    rst = TAF_ACORE_NV_BACKUP_FNV();
    if (rst != VOS_OK) {
        AT_PR_LOGE("nv Backup faile! %d", rst);

        CmdErrProc(clientId, rst, 0, NULL);

        return AT_ERROR;
    }


    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%d\r", rst);
    return AT_OK;
}

VOS_UINT32 AT_SetLteSdloadPara(VOS_UINT8 clientId)
{
    FTM_RdSdloadReq sdloadSetReq = {0};
    VOS_UINT32      rst;

    rst = atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_FTM_SET_SDLOAD_REQ, clientId, (VOS_UINT8 *)(&sdloadSetReq),
                           sizeof(sdloadSetReq));

    if (rst == AT_SUCCESS) {
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_SDLOAD_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_ERROR;
}

VOS_UINT32 atSetSdloadCnf(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    FTM_SetSdloadCnf *cnf   = NULL;
    OS_MSG           *event = NULL;
    g_atSendData.bufLen     = 0;

    event = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    cnf   = (FTM_SetSdloadCnf *)event->param1;

    if (cnf == NULL) {
        CmdErrProc(clientId, ERR_MSP_FAILURE, 0, NULL);
        return AT_OK;
    }

    CmdErrProc(clientId, cnf->errCode, 0, NULL);

    return AT_OK;
}
/* END: To_Be_Deleted_V7R2 */
