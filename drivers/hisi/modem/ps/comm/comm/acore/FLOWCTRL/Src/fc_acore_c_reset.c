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

#include "product_config.h"
#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)

#include "fc.h"
#include "fc_interface.h"
#include "fc_intra_msg.h"
#include "fc_acore_c_reset.h"
#include "ps_trace_msg.h"



#define THIS_FILE_ID PS_FILE_ID_ACORE_CRESET_FLOW_CTRL_C

VOS_SEM g_fcaCorecResetDoneSem; /* FcACore完成回调事务信号量 */

/* 底软C核复位过程模块通知FcACore进行复位处理或者发送复位成功的通知 */
VOS_UINT32 FC_AcoreCResetSendNotify(FC_MsgTypeUint16 msgName)
{
    FC_ACORE_CresetInd *msg = VOS_NULL_PTR;

    /* 申请消息内存 */
    msg = (FC_ACORE_CresetInd*)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL_A, (VOS_UINT32)sizeof(FC_ACORE_CresetInd) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreCResetSendNotify, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    /* 填写消息内容 */
    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL_A);
    msg->msgName = msgName;

    /* 发送消息 */
    (VOS_VOID)PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    return VOS_OK;
}

/* FcACore需要注册到底软C核复位接口中的回调函数 */
VOS_INT FC_AcoreCResetCallback(drv_reset_cb_moment_e param, VOS_INT userdata)
{
    VOS_UINT32 result;

    if (param == MDRV_RESET_CB_BEFORE) { /* 复位处理时调用 */
        FC_AcoreCResetSendNotify(ID_FC_ACORE_CRESET_START_IND);

        result = VOS_SmP(g_fcaCorecResetDoneSem, FC_ACORE_CRESET_TIMEOUT_LEN);
        if (result != VOS_OK) {
            FC_LOG1(PS_PRINT_ERROR, "FC_ACORE_CRESET_Callback, wait g_ulFcACoreResetDoneSem timeout! ulResult = <1>\r\n",
                (VOS_INT32)result);

            return VOS_ERR;
        }
    } else if (param == MDRV_RESET_CB_AFTER) { /* 复位成功后调用 */
        FC_AcoreCResetSendNotify(ID_FC_ACORE_CRESET_END_IND);
    }

    /* for lint 715 */
    (VOS_VOID)userdata;

    return VOS_OK;
}

/* FcACore收到底软C核复位过程处理模块发来的复位及复位成功的通知后的处理函数 */
VOS_UINT32 FC_AcoreCResetProc(FC_ACORE_CresetMomentUint8 cResetMoment)
{
    VOS_UINT32          fcPointLoop;
    FC_Point           *fcPoint = VOS_NULL_PTR;
    VOS_UINT32          result;
    FC_MsgTypeUint16    msgName;
    FC_ACORE_CresetRsp *msg = VOS_NULL_PTR;
    VOS_UINT32          rspResult;

    rspResult = 0;

    if (cResetMoment == FC_ACORE_CRESET_BEFORE_RESET) {
        msgName = ID_FC_ACORE_CRESET_START_RSP;

        /* 复位处理时遍历每个流控点，执行流控点上外部模块注册的reset函数 */
        for (fcPointLoop = 0; fcPointLoop < g_fcPointMgr.pointNum; fcPointLoop++) {
            fcPoint = &g_fcPointMgr.fcPoint[fcPointLoop];

            if (fcPoint->rstFunc != VOS_NULL_PTR) {
                result = fcPoint->rstFunc(fcPoint->param1, fcPoint->param2);
                rspResult |= result;
                FC_MNTN_TracePointFcEvent(ID_FC_MNTN_ACORE_CRESET_START_FC, fcPoint, VOS_TRUE, result);
            }
        }
    } else if (cResetMoment == FC_ACORE_CRESET_AFTER_RESET) {
        msgName = ID_FC_ACORE_CRESET_END_RSP;
    } else {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreCResetProc, enCResetMoment invalid !\n");
        return VOS_ERR;
    }

    /* 申请消息内存: */
    msg = (FC_ACORE_CresetRsp*)(VOS_UINT_PTR)VOS_AllocMsg(
        UEPS_PID_FLOWCTRL_A, (VOS_UINT32)sizeof(FC_ACORE_CresetRsp) - VOS_MSG_HEAD_LENGTH);

    if (msg == VOS_NULL_PTR) {
        FC_LOG(PS_PRINT_ERROR, "FC_AcoreCResetProc, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    /* 填写消息内容: */
    TTF_SET_MSG_RECEIVER_ID(msg, UEPS_PID_FLOWCTRL_A);
    msg->msgName = msgName;
    msg->result  = rspResult;

    /* 发送消息: */
    (VOS_VOID)PS_TRACE_AND_SND_MSG(UEPS_PID_FLOWCTRL, msg);

    return VOS_OK;
}

/* 收到ID_FC_ACORE_CRESET_START_RSP消息后的处理 */
VOS_VOID FC_AcoreCResetRcvStartRsp(VOS_VOID)
{
    VOS_SmV(g_fcaCorecResetDoneSem);

    return;
}


#endif
