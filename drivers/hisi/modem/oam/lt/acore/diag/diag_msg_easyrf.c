/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <securec.h>
#include "diag_msg_easyrf.h"
#include <mdrv.h>
#include <mdrv_diag_system.h>
#include "diag_common.h"
#include "diag_api.h"
#include "diag_cfg.h"
#include "diag_debug.h"


#define THIS_FILE_ID MSP_FILE_ID_DIAG_MSGEASYRF_C

/*
 * Function Name: diag_EasyRFMsgProc
 * Description: EasyRF 配置消息透传处理
 */
VOS_UINT32 diag_EasyRfMsgProc(msp_diag_frame_info_s *pData)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;
    DIAG_COMM_CNF_STRU stRttCnf = { 0 };
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = { 0 };
    diag_osa_msg_head_s *pstMsg = NULL;
    diag_osa_msg_head_s *pstVosMsg = NULL;
    errno_t err;

    if (pData->ulMsgLen < sizeof(diag_osa_msg_head_s) + sizeof(msp_diag_data_head_s)) {
        diag_error("rev tools msg len error, len:0x%x\n", pData->ulMsgLen);
        diag_FailedCmdCnf(pData, ERR_MSP_DIAG_ERRPID_CMD);
        return ERR_MSP_FAILURE;
    }

    pstMsg = (diag_osa_msg_head_s *)(pData->aucData + sizeof(msp_diag_data_head_s));

    if (VOS_CheckPidValidity(pstMsg->ulReceiverPid) != VOS_PID_AVAILABLE) {
        diag_error("rev pid error, Pid:0x%x\n", pstMsg->ulReceiverPid);
        ulRet = ERR_MSP_INVALID_ID;
        goto DIAG_ERROR;
    }

    if (pstMsg->ulLength > DIAG_MEM_ALLOC_LEN_MAX) {
        diag_error("Msg len valid,MsgId=0x%x, MsgLen=0x%x\n", pData->ulCmdId, pData->ulMsgLen);
        ulRet = ERR_MSP_NOT_ENOUGH_MEMORY;
        goto DIAG_ERROR;
    }

    pstVosMsg = (diag_osa_msg_head_s *)VOS_AllocMsg(MSP_PID_DIAG_APP_AGENT, pstMsg->ulLength);
    if (pstVosMsg == VOS_NULL) {
        diag_error("VOS_AllocMsg failed!\n");
        ulRet = ERR_MSP_MALLOC_FAILUE;
        goto DIAG_ERROR;
    }

    pstVosMsg->ulReceiverPid = pstMsg->ulReceiverPid;

    err = memcpy_s(&pstVosMsg->ulMsgId, pstMsg->ulLength, &pstMsg->ulMsgId, pstMsg->ulLength);
    if (err != EOK) {
        diag_error("memcpy fail:%x\n", err);
    }

    ulRet = VOS_SendMsg(MSP_PID_DIAG_APP_AGENT, pstVosMsg);
    if (ulRet != VOS_OK) {
        diag_error("VOS_SendMsg failed(0x%x)\n", ulRet);
        ulRet = ERR_MSP_DIAG_SEND_MSG_FAIL;
    }

DIAG_ERROR:
    stRttCnf.ulRet = ulRet;
    DIAG_MSG_COMMON_PROC(stDiagInfo, stRttCnf, pData);

    stDiagInfo.ulMsgType = pData->stID.pri4b;

    /* 组包回复 */
    ulRet = DIAG_MsgReport(&stDiagInfo, &stRttCnf, sizeof(stRttCnf));

    return ulRet;
}
/*
 * Function Name: diag_EasyRfMsgInit
 * Description: MSP EasyRF部分初始化
 */
VOS_VOID diag_EasyRfMsgInit(VOS_VOID)
{
    /* 注册message消息回调 */
#ifdef DIAG_SYSTEM_5G
    DIAG_MsgProcReg(DIAG_MSG_TYPE_EASYRF, diag_EasyRfMsgProc);
#endif

    return;
}

