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

#ifndef FC_CDS_INTERFACE_H
#define FC_CDS_INTERFACE_H

#include "vos.h"
#include "om_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

enum CDS_FC_MsgType {
    ID_CDS_FC_START_CHANNEL_IND = 0x0001, /* _H2ASN_MsgChoice CDS_FC_START_CHANNEL_IND_STRU */
    ID_CDS_FC_STOP_CHANNEL_IND  = 0x0002, /* _H2ASN_MsgChoice CDS_FC_STOP_CHANNEL_IND_STRU */
    ID_FC_CDS_DL_THRES_CHG_IND  = 0x0003, /* _H2ASN_MsgChoice FC_CDS_THRES_CHG_IND_STRU */
    ID_CDS_FC_MSG_TYPE_BUTT     = 0xFFFF
};
typedef VOS_UINT32 CDS_FC_MsgTypeUint32;

typedef struct {
    VOS_MSG_HEADER
    CDS_FC_MsgTypeUint32 msgName;
    MODEM_ID_ENUM_UINT16        modemId;
    VOS_UINT8                   rabId;
    VOS_UINT8                   reserved[1];
} CDS_FC_StartChannelInd;

typedef struct {
    VOS_MSG_HEADER
    CDS_FC_MsgTypeUint32 msgName;
    MODEM_ID_ENUM_UINT16        modemId;
    VOS_UINT8                   rabId;
    VOS_UINT8                   reserved[1];
} CDS_FC_StopChannelInd;

typedef struct {
    VOS_MSG_HEADER
    CDS_FC_MsgTypeUint32 msgName;
    VOS_UINT8                   thres;
    VOS_UINT8                   reserved[3];
} FC_CDS_ThresChgInd;

extern SPY_DataDowngradeResultUint32 FC_DownUlGradeProcess(VOS_VOID);
extern SPY_DataUpgradeResultUint32   FC_UpUlGradeProcess(VOS_VOID);
extern VOS_VOID                      FC_RecoverUlGradeProcess(VOS_VOID);
extern unsigned int                  FC_CpuProcess(unsigned int ulCpuLoad);

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
