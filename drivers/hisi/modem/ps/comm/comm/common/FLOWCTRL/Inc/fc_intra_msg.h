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

#ifndef __FCINTRAMSG_H__
#define __FCINTRAMSG_H__

#include "vos.h"
#include "PsTypeDef.h"
#include "fc_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

enum FC_MsgType {
    ID_FC_CPU_A_OVERLOAD_IND         = 0x0001, /* _H2ASN_MsgChoice FC_CpuLoadInd */
    ID_FC_CPU_A_UNDERLOAD_IND        = 0x0002, /* _H2ASN_MsgChoice FC_CpuLoadInd */
    ID_FC_TEMPERATURE_OVERLOAD_IND   = 0x0003, /* _H2ASN_MsgChoice FC_TEMPERATURE_IND_STRU */
    ID_FC_TEMPERATURE_UNDERLOAD_IND  = 0x0004, /* _H2ASN_MsgChoice FC_TEMPERATURE_IND_STRU */
    ID_FC_TEMPERATURE_RECOVER_IND    = 0x0005, /* _H2ASN_MsgChoice FC_TEMPERATURE_IND_STRU */
    ID_FC_SET_FLOWCTRL_IND           = 0x0006, /* _H2ASN_MsgChoice FC_SET_FLOWCTRL_IND_STRU */
    ID_FC_STOP_FLOWCTRL_IND          = 0x0007, /* _H2ASN_MsgChoice FC_STOP_FLOWCTRL_IND_STRU */
    ID_FC_SET_POINT_FLOWCTRL_IND     = 0x0008, /* _H2ASN_MsgChoice FC_SET_POINT_FLOWCTRL_IND_STRU */
    ID_FC_STOP_POINT_FLOWCTRL_IND    = 0x0009, /* _H2ASN_MsgChoice FC_STOP_POINT_FLOWCTRL_IND_STRU */
    ID_FC_REG_POINT_IND              = 0x000A, /* _H2ASN_MsgChoice FC_REG_POINT_IND_STRU */
    ID_FC_DEREG_POINT_IND            = 0x000B, /* _H2ASN_MsgChoice FC_DEREG_POINT_IND_STRU */
    ID_FC_CHANGE_POINT_IND           = 0x000C, /* _H2ASN_MsgChoice FC_CHANGE_POINT_IND_STRU */
    ID_FC_ADD_RAB_FCID_MAP_IND       = 0x000D, /* _H2ASN_MsgChoice FC_ADD_RAB_FCID_MAP_IND_STRU */
    ID_FC_DEL_RAB_FCID_MAP_IND       = 0x000E, /* _H2ASN_MsgChoice FC_DEL_RAB_FCID_MAP_IND_STRU */
    ID_FC_MEM_UP_TO_TARGET_PRI_IND   = 0x000F, /* _H2ASN_MsgChoice FC_MEM_UP_TO_TARGET_PRI_IND_STRU */
    ID_FC_MEM_DOWN_TO_TARGET_PRI_IND = 0x0010, /* _H2ASN_MsgChoice FC_MEM_DOWN_TO_TARGET_PRI_IND_STRU */
    ID_FC_CPU_C_OVERLOAD_IND         = 0x0011, /* _H2ASN_MsgChoice FC_CpuLoadInd */
    ID_FC_CPU_C_UNDERLOAD_IND        = 0x0012, /* _H2ASN_MsgChoice FC_CpuLoadInd */
    ID_FC_C_FLOWCTRL_NOTIFY          = 0x0013, /* _H2ASN_MsgChoice FC_FLOWCTRL_NOTIFY_STRU */
    ID_FC_ACORE_CRESET_START_IND     = 0x0014, /* _H2ASN_MsgChoice FC_ACORE_CRESET_IND_STRU */
    ID_FC_ACORE_CRESET_END_IND       = 0x0015, /* _H2ASN_MsgChoice FC_ACORE_CRESET_IND_STRU */
    ID_FC_ACORE_CRESET_START_RSP     = 0x0016, /* _H2ASN_MsgChoice FC_ACORE_CRESET_RSP_STRU */
    ID_FC_ACORE_CRESET_END_RSP       = 0x0017, /* _H2ASN_MsgChoice FC_ACORE_CRESET_RSP_STRU */

    ID_FC_MSG_TYPE_BUTT = 0xFFFF
};
typedef VOS_UINT16 FC_MsgTypeUint16;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    VOS_UINT32           cpuLoad;
    VOS_UINT32           ulRate;
    VOS_UINT32           dlRate;
} FC_CpuLoadInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT16           temperature;
} FC_TemperatureInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    VOS_RatModeUint32    rateMode;
} FC_SetFlowCtrlInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    VOS_RatModeUint32    rateMode;
} FC_FlowCtrlNotify;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    VOS_RatModeUint32    rateMode;
} FC_StopFlowCtrlInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    FC_IdUint8           fcId;
    VOS_UINT8            rsv[1];
} FC_SetPointFlowCtrlInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    FC_IdUint8           fcId;
    VOS_UINT8            rsv[1];
} FC_StopPointFlowCtrlInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    FC_RegPointPara      fcPoint;
} FC_RegPointInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    ModemIdUint16        modemId; /* _H2ASN_Replace VOS_UINT16  enModemId; */
    FC_IdUint8           fcId;
    VOS_UINT8            rsv[7];
} FC_DeregPointInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    ModemIdUint16        modemId; /* _H2ASN_Replace VOS_UINT16  enModemId; */
    FC_IdUint8           fcId;
    FC_PolicyIdUint8     policyId;
    FC_PriTypeUint8      pri;
    VOS_UINT8            rsv[5];
} FC_ChangePointInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    ModemIdUint16        modemId; /* _H2ASN_Replace VOS_UINT16  enModemId; */
    FC_IdUint8           fcId;
    VOS_UINT8            rabId;
    VOS_UINT8            rsv[6];
} FC_AddRabFcIdMapInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    ModemIdUint16        modemId; /* _H2ASN_Replace VOS_UINT16  enModemId; */
    VOS_UINT8            rabId;
    VOS_UINT8            rsv[7];
} FC_DelRabFcIdMapInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT16           memFreeCnt;
    FC_PriTypeUint8      targetPri;
    VOS_UINT8            rsv[7];
} FC_MemUpToTargetPriInd;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT16           memFreeCnt;
    FC_PriTypeUint8      targetPri;
    VOS_UINT8            rsv[7];
} FC_MemDownToTargetPriInd;

/* C核单独复位特性中，底软通知FcACore的指示结构 */
typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
        FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
} FC_ACORE_CresetInd;

/* C核单独复位特性中，FcACore完成回调事务后的通知消息结构 */
typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    FC_MsgTypeUint16 msgName; /* _H2ASN_Skip */
    VOS_UINT8            rsv[2];
    VOS_UINT32           result;
} FC_ACORE_CresetRsp;

typedef struct {
    FC_MsgTypeUint16 msgId; /* _H2ASN_MsgChoice_Export FC_MSG_TYPE_ENUM_UINT16 */

    VOS_UINT8 msgBlock[2];
} FC_IntraMsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    FC_IntraMsgData msgData;
} FC_IntraMsgMSG;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
