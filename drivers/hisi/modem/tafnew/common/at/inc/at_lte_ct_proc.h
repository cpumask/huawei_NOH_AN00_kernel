/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
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

#ifndef _ATLTECTPROC_H_
#define _ATLTECTPROC_H_

#include "vos.h"
#include "taf_type_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
/* 从tafdrvagent.h中移到这个文件中 */
#define AT_FEATURE_NAME_LEN_MAX 16
#define AT_FEATURE_CONTENT_LEN_MAX 504

typedef struct {
    VOS_UINT8 featureFlag;
    VOS_UINT8 TAF_COMM_ATTRIBUTE_ALIGNED4 featureName[AT_FEATURE_NAME_LEN_MAX];
    VOS_UINT8 TAF_COMM_ATTRIBUTE_ALIGNED4 content[AT_FEATURE_CONTENT_LEN_MAX];
} AT_FEATURE_SUPPORT_ST;

VOS_UINT32 atQryFTXONPara(VOS_UINT8 clientId);
VOS_UINT32 atQryFTXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atQryFPllStatusPara(VOS_UINT8 clientId);
VOS_UINT32 atQryFPllStatusParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atSetFRXONPara(VOS_UINT8 clientId);
VOS_UINT32 atSetFRXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atQryFRXONPara(VOS_UINT8 clientId);
VOS_UINT32 atQryFRXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 atSetFCHANPara(VOS_UINT8 clientId);
VOS_UINT32 atSetFCHANParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);

VOS_UINT32 atQryFCHANPara(VOS_UINT8 clientId);
VOS_UINT32 atQryFCHANParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
#endif

VOS_UINT32 atSetFWAVEParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atSetTselrfPara(VOS_UINT8 clientId);
VOS_UINT32 atQryTselrfPara(VOS_UINT8 clientId);
VOS_UINT32 atSetFLNAPara(VOS_UINT8 clientId);
VOS_UINT32 atSetFLNAParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atQryFLNAPara(VOS_UINT8 clientId);
VOS_UINT32 atQryFRSSIPara(VOS_UINT8 clientId);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_GetLteFeatureInfo(AT_FEATURE_SUPPORT_ST *feATure);
#endif

VOS_UINT32 atSetTBATPara(VOS_UINT8 clientId);
VOS_UINT32 atSetTbatCnf(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atRdTbatCnf(VOS_UINT8 clientId, struct MsgCB *msgBlock);

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
VOS_UINT32 At_ProcLteTxCltInfoReport(struct MsgCB *msgBlock);
#endif

VOS_UINT32 atSetFTXONParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);

#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
VOS_UINT32 atSetFTXONPara(VOS_UINT8 clientId);
#endif

VOS_UINT32 atSetFWAVEPara(VOS_UINT8 clientId);
VOS_UINT32 atSetTselrfParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atQryFLNAParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);
VOS_UINT32 atQryFRSSIParaCnfProc(VOS_UINT8 clientId, struct MsgCB *msgBlock);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _ATLTECTPROC_H_ */
