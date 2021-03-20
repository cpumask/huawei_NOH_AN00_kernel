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
/*lint -save -e537 -e958 -e718 -e746*/
#include "at_lte_set_proc.h"
#include "osm.h"
#include "gen_msg.h"
#include "at_lte_common.h"
#include "ATCmdProc.h"

/*
 * 功能描述  : 发送TMODE模式到TL C核
 */
VOS_UINT32 atSetTmodePara(VOS_UINT8 clientId, VOS_UINT32 tmode)
{
    SYM_SET_TmodeReq tmodeInfo;
    tmodeInfo.tmodeMode = (SYM_TMODE_ENUM)tmode;

    if (AT_SUCCESS == atSendFtmDataMsg(I0_MSP_SYS_FTM_PID, ID_MSG_SYM_SET_TMODE_REQ, clientId,
                                       (VOS_UINT8 *)(&tmodeInfo), sizeof(SYM_SET_TmodeReq))) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}

/*
 * 功能描述: TL C核模式设置返回处理函数 输入参数  : ucClientId Client ID
 *           pMsgBlock  消息内容
 */
VOS_UINT32 atSetTmodeParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock)
{
    OS_MSG           *event    = NULL;
    SYM_SET_TmodeCnf *tmodeCnf = NULL;

    /* pMsgBlock 在上层调用中已判断是否为空 */
    event    = (OS_MSG *)(((MsgBlock *)msgBlock)->value);
    tmodeCnf = (SYM_SET_TmodeCnf *)event->param1;

    HAL_SDMLOG("\n atSetTmodeParaCnfProc, ulErrCode=0x%x\n", (VOS_INT)tmodeCnf->errCode);

    CmdErrProc(clientId, tmodeCnf->errCode, 0, NULL);
    return AT_FW_CLIENT_STATUS_READY;
}
/*lint -restore*/
