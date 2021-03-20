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

#ifndef __ATCMDCAGPSPROC_H__
#define __ATCMDCAGPSPROC_H__

#include "AtCtx.h"
#include "AtParse.h"
#include "ATCmdProc.h"

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
#include "at_xpds_interface.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_AGPS == FEATURE_ON) && (FEATURE_XPDS == FEATURE_ON))

#define AT_XPDS_AGPS_DATAUPLEN_MAX 240

#define AT_AGPS_EPH_INFO_MAX_SEG_NUM 5
#define AT_AGPS_EPH_INFO_FIRST_SEG_STR_LEN 8
#define AT_AGPS_EPH_INFO_NOT_FIRST_SEG_STR_LEN 960

#define AT_AGPS_ALM_INFO_MAX_SEG_NUM 3
#define AT_AGPS_ALM_INFO_FIRST_SEG_STR_LEN 8
#define AT_AGPS_ALM_INFO_NOT_FIRST_SEG_STR_LEN 896


typedef struct {
    AT_XPDS_MsgTypeUint32 msgType;
    VOS_UINT32            reserved;
    VOS_CHAR             *pcATCmd;
} AT_CagpsCmdNameTlb;


typedef struct {
    AT_XPDS_MsgTypeUint32 msgType;
    AT_CmdCurrentOpt      cmdOpt;
} AT_CagpsCmdOptTlb;

extern VOS_UINT32 At_SetAgpsDataCallStatus(VOS_UINT8 indexNum);

extern VOS_UINT32 At_SetAgpsUpBindStatus(VOS_UINT8 indexNum);

extern VOS_UINT32 At_SetAgpsForwardData(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_RcvXpdsAgpsDataCallReq(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvXpdsAgpsServerBindReq(struct MsgCB *msg);

extern VOS_UINT32 AT_RcvXpdsAgpsReverseDataInd(struct MsgCB *msg);

extern VOS_UINT32 AT_CagpsSndXpdsReq(VOS_UINT8 indexNum, AT_XPDS_MsgTypeUint32 msgType, VOS_UINT32 msgStructSize);

extern VOS_UINT32 AT_SetCagpsCfgPosMode(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsStart(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsStop(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsCfgMpcAddr(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsCfgPdeAddr(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsQryRefloc(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsQryTime(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsPrmInfo(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCagpsReplyNiReq(VOS_UINT8 indexNum);

extern VOS_UINT32 At_SetCagpsPosInfo(VOS_UINT8 indexNum);

extern VOS_CHAR* AT_SearchCagpsATCmd(AT_XPDS_MsgTypeUint32 msgType);

extern VOS_UINT32 AT_RcvXpdsCagpsCnf(struct MsgCB *msg);

AT_CmdCurrentOpt AT_SearchCagpsATCmdOpt(AT_XPDS_MsgTypeUint32 msgType);

extern VOS_UINT32 AT_RcvXpdsCagpsRlstCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvXpdsEphInfoInd(struct MsgCB *msg);

VOS_UINT32 AT_RcvXpdsAlmInfoInd(struct MsgCB *msg);

extern VOS_UINT32 At_SetCgpsControlStart(VOS_UINT8 indexNum);

extern VOS_UINT32 At_SetCgpsControlStop(VOS_UINT8 indexNum);

#endif
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
