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
#ifndef _AT_SND_MSG_H_
#define _AT_SND_MSG_H_

#include "vos.h"
#include "at_rabm_interface.h"
#include "at_mn_interface.h"
#include "AtCtx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AT_IMS_MSG_CONTEXT_LENGTH 4

#pragma pack(push, 4)


typedef struct {
    VOS_UINT32                            msgLen;                         /* message length */
    VOS_UINT8 TAF_COMM_ATTRIBUTE_ALIGNED4 msgContext[AT_IMS_MSG_CONTEXT_LENGTH]; /* message context */
} AT_IMS_CtrlMsg;

typedef AT_IMS_CtrlMsg AT_MAPCON_CTRL_MSG_STRU;

VOS_UINT32 AT_FillAndSndAppReqMsg(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT16 msgType, VOS_UINT8 *para,
                                  VOS_UINT32 paraLen, VOS_UINT32 rcvPid);

VOS_UINT32 AT_SndSetFastDorm(VOS_UINT16 clientId, VOS_UINT8 opId, AT_RABM_FastdormPara *fastDormPara);
VOS_UINT32 AT_SndQryFastDorm(VOS_UINT16 clientId, VOS_UINT8 opId);

VOS_UINT32 AT_FillAppReqMsgHeader(MN_APP_ReqMsg *msg, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                  VOS_UINT16 msgType, VOS_UINT32 rcvPid);

VOS_UINT32 At_SndReleaseRrcReq(VOS_UINT16 clientId, VOS_UINT8 opId);

#if (FEATURE_HUAWEI_VP == FEATURE_ON)
VOS_UINT32 AT_SndSetVoicePrefer(VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT32 voicePreferApStatus);
VOS_UINT32 AT_SndQryVoicePrefer(VOS_UINT16 clientId, VOS_UINT8 opId);
#endif

VOS_UINT32 AT_FillAndSndCSIMAMsg(VOS_UINT16 clinetID, VOS_UINT32 modemStatus);
#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_SndImsaImsCtrlMsg(VOS_UINT16 clientId, VOS_UINT8 opId, AT_IMS_CtrlMsg *atImsaMsgPara);
VOS_UINT32 AT_SndImsaCallRawDataMsg(VOS_UINT16 clientId, VOS_UINT8 opId,
    VOS_UINT8 dstId, VOS_UINT32 dataLen, VOS_UINT8 *data);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtSndMsg.h */
