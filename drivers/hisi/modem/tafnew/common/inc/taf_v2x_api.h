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

#ifndef TAF_V2X_API_H
#define TAF_V2X_API_H

#include "vos.h"
#include "taf_type_def.h"
#include "at_mn_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef WIN32
#pragma warning(disable : 4200) /* zero-sized array in struct/union */
#endif

#pragma pack(push, 4)
#if (FEATURE_LTEV == FEATURE_ON)
#define AT_PSCCH_TRANSPORT_BLOCKS 1                /* PSCCH传输块 */
#define AT_STCH_PDCP_SDU_PACKETS 1                 /* PDCP数据包 */
#define AT_PSSCH_TRANSPORT_BLOCKS 1                /* PSSCH传输块 */
#define AT_CCUTLE_MONITOR_LIST_MAX_NUM 16          /* 可监测的LAYER2-ID最大数量 */
#define AT_CCUTLE_MONITOR_LIST_CONTEXT_TO_ID 6     /* 每个LAYER2-ID使用6个字符描述 */
#define AT_CCUTLE_MONITOR_LIST_PACKETS_TO_ID 8     /* 每个LAYER2-ID下数据包数量使用8个字符描述 */
#define AT_CCUTLE_MONITOR_LIST_CONTEXT_MAX_NUM 96  /* 描述所有LAYER2-ID最多占用的字符 */
#define AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM 128 /* 描述所有LAYER2-ID所有数据包最多占用的字符 */

/* AT和VRRC之间的消息接口 */
enum AT_VRRC_MsgType {
    VRRC_MSG_RSU_SYNCST_QRY_REQ = 0, /* 查询RSU同步状态请求消息 */
    VRRC_MSG_RSU_SYNCST_QRY_CNF,     /* 查询RSU同步状态确认消息 */
    VRRC_MSG_GNSS_TEST_START_REQ,    /* 通知PHY启动GNSS测试请求消息 */
    VRRC_MSG_GNSS_TEST_START_CNF,    /* 通知PHY启动GNSS测试确认消息 */
    VRRC_MSG_GNSS_INFO_QRY_REQ,      /* 查询GNSS相关信息请求消息 */
    VRRC_MSG_GNSS_INFO_QRY_CNF,      /* 查询GNSS相关信息确认消息 */
    VRRC_MSG_RSU_SLINFO_QRY_REQ,
    VRRC_MSG_RSU_SLINFO_QRY_CNF,
    VRRC_MSG_RSU_SLINFO_SET_REQ,
    VRRC_MSG_RSU_SLINFO_SET_CNF,

    VRRC_MSG_RPPCFG_SET_REQ,  /* 设置V2X资源池参数请求消息 */
    VRRC_MSG_RPPCFG_SET_CNF,  /* 设置V2X资源池参数确认消息 */
    VRRC_MSG_RPPCFG_QRY_REQ,  /* 查询V2X资源池参数请求消息 */
    VRRC_MSG_RPPCFG_QRY_CNF,  /* 查询V2X资源池参数确认消息 */
    VRRC_MSG_RSU_SLCFGST_RPT, /* V2X资源池参数配置状态上报 */

    VRRC_MSG_RSU_PHYR_SET_REQ,
    VRRC_MSG_RSU_PHYR_SET_CNF,
    VRRC_MSG_RSU_PHYR_QRY_REQ,
    VRRC_MSG_RSU_PHYR_QRY_CNF,

    VRRC_MSG_RSU_GNSSSYNCST_QRY_REQ, /* 查询RSU GNSS同步状态请求消息 */
    VRRC_MSG_RSU_GNSSSYNCST_QRY_CNF, /* 查询RSU GNSS同步状态确认消息 */

    VRRC_MSG_RSU_VPHYSTAT_QRY_REQ, /* 查询基带收发统计结果请求消息 */
    VRRC_MSG_RSU_VPHYSTAT_QRY_CNF, /* 查询基带收发统计结果确认消息 */

    VRRC_MSG_RSU_VPHYSTAT_CLR_SET_REQ, /* 设置基带收发统计结果清零请求消息 */
    VRRC_MSG_RSU_VPHYSTAT_CLR_SET_CNF, /* 设置基带收发统计结果清零确认消息 */

    VRRC_MSG_RSU_VSNRRSRP_QRY_REQ, /* 查询基带SNR、RSRP信息请求消息 */
    VRRC_MSG_RSU_VSNRRSRP_QRY_CNF, /* 查询基带SNR、RSRP信息确认消息 */

    VRRC_MSG_GNSS_INFO_GET_REQ, /* 查询当前GNSS Debug信息请求消息 */
    VRRC_MSG_GNSS_INFO_GET_CNF, /* 查询当前GNSS Debug信息确认消息 */

    VRRC_MSG_RSU_RSSI_QRY_REQ, /* 查询基带RSSI信息请求消息 */
    VRRC_MSG_RSU_RSSI_QRY_CNF, /* 查询基带RSSI信息确认消息 */

    VRRC_MSG_PC5SYNC_QRY_REQ, /* 查询PC5同步相关参数请求消息，VRRC无需处理 */
    VRRC_MSG_PC5SYNC_QRY_CNF, /* 查询PC5同步相关参数确认消息，VRRC无需处理 */
    VRRC_MSG_PC5SYNC_SET_REQ, /* 设置PC5同步相关参数请求消息 ，VRRC无需处理 */
    VRRC_MSG_PC5SYNC_SET_CNF, /* 设置PC5同步相关参数确认消息，VRRC无需处理 */

    AT_VRRC_SYNC_SOURCE_QRY_REQ = 35, /* 同步源查询请求 */
    VRRC_AT_SYNC_SOURCE_QRY_CNF,      /* 同步源查询确认 */

    AT_VRRC_SYNC_SOURCE_RPT_SET_REQ, /* 同步源变化上报设置请求 */
    VRRC_AT_SYNC_SOURCE_RPT_SET_CNF, /* 同步源变化上报设置确认 */
    AT_VRRC_SYNC_SOURCE_RPT_QRY_REQ, /* 同步源变化上报查询请求 */
    VRRC_AT_SYNC_SOURCE_RPT_QRY_CNF, /* 同步源变化上报查询确认 */
    VRRC_AT_SYNC_SOURCE_RPT_IND,     /* 同步源变化主动上报 */

    AT_VRRC_SYNC_MODE_SET_REQ, /* 同步模式设置请求 */
    VRRC_AT_SYNC_MODE_SET_CNF, /* 同步模式设置确认 */

    AT_VRRC_SYNC_MODE_QRY_REQ, /* 同步模式查询请求 */
    VRRC_AT_SYNC_MODE_QRY_CNF, /* 同步模式查询确认 */

    AT_VRRC_SET_TX_POWER_REQ, /* 设置物理层发射功率请求消息 */
    VRRC_AT_SET_TX_POWER_CNF, /* 设置物理层发射功率确认消息 */
    VRRC_MSG_BUTT
};
typedef VOS_UINT16 AT_VRRC_MsgTypeUint16;

/* AT和VMAC之间的消息接口 */
enum AT_VMAC_MsgType {
    VMAC_MSG_RSU_SLPTRST_RPT = 0,
    VMAC_MSG_RSU_VRSSI_QRY_REQ,
    VMAC_MSG_RSU_VRSSI_QRY_CNF,
    VMAC_MSG_RSU_MCG_CFG_SET_REQ,
    VMAC_MSG_RSU_MCG_CFG_SET_CNF,
    VMAC_MSG_RSU_DSTIDTM_SET_REQ,
    VMAC_MSG_RSU_DSTIDTM_SET_CNF,
    VMAC_MSG_PC5_SEND_DATA_REQ, /* 启动PC5发包请求消息 */
    VMAC_MSG_PC5_SEND_DATA_CNF, /* 启动PC5发包确认消息 */

    VMAC_AT_RSSI_RPT_IND = 9, /* RSSI变化主动上报 */
    AT_VMAC_CBR_QRY_REQ,      /* CBR查询请求 */
    VMAC_AT_CBR_QRY_CNF,      /* CBR查询确认 */
    VMAC_AT_CBR_RPT_IND,      /* CBR主动上报 */
    VMAC_MSG_BUTT
};
typedef VOS_UINT16 AT_VMAC_MsgTypeUint16;


enum AT_VPDCP_MsgType {
    VPDCP_MSG_RSU_PTRRPT_QRY_REQ = 0,
    VPDCP_MSG_RSU_PTRRPT_QRY_CNF,

    VPDCP_MSG_RSU_PTRRPT_SET_REQ,
    VPDCP_MSG_RSU_PTRRPT_SET_CNF,

    VPDCP_MSG_BUTT
};
typedef VOS_UINT16 AT_VPDCP_MsgTypeUint16;

/* AT和VTC之间的消息接口 */
enum AT_VTC_MsgType {
    AT_VTC_TEST_MODE_ACTIVE_STATE_SET_REQ = 0,   /* 测试模式激活状态设置请求 */
    VTC_AT_TEST_MODE_ACTIVE_STATE_SET_CNF,       /* 测试模式激活状态设置响应 */
    AT_VTC_TEST_MODE_ACTIVE_STATE_QRY_REQ,       /* 测试模式激活状态设置请求 */
    VTC_AT_TEST_MODE_ACTIVE_STATE_QRY_CNF,       /* 测试模式激活状态设置响应 */
    AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_SET_REQ, /* 测试模式E闭合状态设置请求 */
    VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_SET_CNF, /* 测试模式E闭合状态设置响应 */
    AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_REQ, /* 测试模式E闭合状态查询请求 */
    VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_CNF, /* 测试模式E闭合状态查询响应 */
    AT_VTC_SIDELINK_PACKET_COUNTER_QRY_REQ,      /* 数据包查询请求 */
    VTC_AT_SIDELINK_PACKET_COUNTER_QRY_CNF,      /* 数据包查询响应 */
    AT_VTC_RESET_UTC_TIME_SET_REQ,               /* 重置UTC时间设置请求 */
    VTC_AT_RESET_UTC_TIME_SET_CNF,               /* 重置UTC时间设置响应 */
    AT_VTC_CBR_QRY_REQ,                          /* CBR信道忙率查询请求 */
    VTC_AT_CBR_QRY_CNF,                          /* CBR信道忙率查询响应 */
    AT_VTC_SENDING_DATA_ACTION_SET_REQ,          /* 发送数据行为设置请求 */
    VTC_AT_SENDING_DATA_ACTION_SET_CNF,          /* 发送数据行为设置响应 */
    AT_VTC_SENDING_DATA_ACTION_QRY_REQ,          /* 发送数据行为设置请求 */
    VTC_AT_SENDING_DATA_ACTION_QRY_CNF,          /* 发送数据行为设置响应 */

    AT_VTC_PC5_SEND_DATA_REQ,
    VTC_AT_PC5_SEND_DATA_CNF,

    AT_VTC_LAYER_TWO_ID_SET_REQ,       /* 测试模式发包L2ID设置请求 */
    VTC_AT_LAYER_TWO_ID_SET_CNF,       /* 测试模式发包L2ID设置响应 */
    AT_VTC_LAYER_TWO_ID_QRY_REQ,       /* 测试模式发包L2ID查询请求 */
    VTC_AT_LAYER_TWO_ID_QRY_CNF,       /* 测试模式发包L2ID查询响应 */
};
typedef VOS_UINT32 AT_VTC_MsgTypeUint32;

/* 测试模式激活状态 */
enum AT_VTC_TestModeActiveState {
    AT_VTC_TEST_MODE_STATE_DEACTIVATED = 0, /* 去激活 */
    AT_VTC_TEST_MODE_STATE_ACTIVATED,       /* 激活 */

    AT_VTC_TEST_MODE_STATE_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeActiveStateUint8;

/* 测试模式类型 */
enum AT_VTC_TestModeType {
    AT_VTC_TEST_MODE_TYPE_E = 1, /* 测试模式E */

    AT_VTC_TEST_MODE_TYPE_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeTypeUint8;

/* 测试模式下闭环状态 */
enum AT_VTC_TestModeCloseStatus {
    AT_VTC_TEST_MODE_STATUS_CLOSE = 0, /* 进入测试模式 */
    AT_VTC_TEST_MODE_STATUS_OPEN,      /* 退出测试模式 */

    AT_VTC_TEST_MODE_STATUS_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeCloseStatusUint8;

/* 测试模式下数据流向 */
enum AT_VTC_CommunicationDirection {
    AT_VTC_COMMUNICATION_DIRECTION_RECEIVE = 0, /* 接收数据 */
    AT_VTC_COMMUNICATION_DIRECTION_TRANSMIT,    /* 发送数据 */

    AT_VTC_COMMUNICATION_DIRECTION_BUTT
};
typedef VOS_UINT8 AT_VTC_CommunicationDirectionUint8;

/* 监测Destination Layer-ID的格式 */
enum AT_VTC_MonitorListFormat {
    AT_VTC_MONITOR_LIST_FORMAT_BINARY = 1, /* 二进制格式 */

    AT_VTC_MONITOR_LIST_FORMAT_BUTT
};
typedef VOS_UINT8 AT_VTC_MonitorListFormatUint8;

/* 发送数据行为 */
enum AT_VTC_SendingDataAction {
    AT_VTC_SENDING_DATA_ACTION_STOP = 0, /* 停止发送数据 */
    AT_VTC_SENDING_DATA_ACTION_START,    /* 开始发送数据 */

    AT_VTC_SENDING_DATA_ACTION_BUTT
};
typedef VOS_UINT8 AT_VTC_SendingDataActionUint8;

/* 数据包格式 */
enum AT_VTC_PacketCounterFormat {
    AT_VTC_PACKET_COUNTER_FORMAT_BINARY = 1, /* 二进制格式 */

    AT_VTC_PACKET_COUNTER_FORMAT_BUTT
};
typedef VOS_UINT8 AT_VTC_PacketCounterFormatUint8;

/* 同步源类型 */
enum VRRC_AT_SyncSourceType {
    VRRC_AT_SYNC_SOURCE_TYPE_GNSS = 0,
    VRRC_AT_SYNC_SOURCE_TYPE_ENODEB,
    VRRC_AT_SYNC_SOURCE_TYPE_REF_UE,
    VRRC_AT_SYNC_SOURCE_TYPE_SELF,

    VRRC_AT_SYNC_SOURCE_TYPE_BUTT
};
typedef VOS_UINT8 VRRC_AT_SyncSourceTypeUint8;

/* 同步源状态 */
enum VRRC_AT_SyncSourceStatus {
    VRRC_AT_SYNC_SOURCE_STATUS_IN_SYNC = 0, /* 同步 */
    VRRC_AT_SYNC_SOURCE_STATUS_OUT_OF_SYNC, /* 失步 */

    VRRC_AT_SYNC_SOURCE_STATUS_BUTT
};
typedef VOS_UINT8 VRRC_AT_SyncSourceStatusUint8;

/* 同步模式 */
enum AT_VRRC_SyncMode {
    AT_VRRC_SYNC_MODE_GNSS_ONLY = 0,    /* 仅GNSS */
    AT_VRRC_SYNC_MODE_GNSS_REF_UE_SELF, /* GNSS加参考UE加自同步 */

    AT_VRRC_SYNC_MODE_BUTT
};
typedef VOS_UINT8 AT_VRRC_SyncModeUint8;

/* 同步源上报开关 */
enum AT_VRRC_SyncSourceRptSwitch {
    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_OFF = 0, /* 关闭 */
    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_ON,      /* 打开 */

    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_BUTT
};
typedef VOS_UINT8 AT_VRRC_SyncSourceRptSwitchUint8;

/* VTC到AT结果码 */
enum VTC_AT_ResultCode {
    VTC_AT_RESULT_OK                  = 0, /* 成功 */
    VTC_AT_RESULT_ERR                 = 1, /* 失败 */
    VTC_AT_RESULT_OPERATION_NOT_ALLOW = 2, /* 操作不允许 */
    VTC_AT_RESULT_TIMEOUT             = 3, /* 定时器超时 */
};
typedef VOS_UINT32 VTC_AT_ResultCodeUint32;

/* VRRC到AT结果码 */
enum VRRC_AT_ResultCode {
    VRRC_AT_RESULT_OK                  = 0, /* 成功 */
    VRRC_AT_RESULT_ERR                 = 1, /* 失败 */
    VRRC_AT_RESULT_NO_SYNC_SOURCE      = 2, /* 没有找到同步源 */
};
typedef VOS_UINT32 VRRC_AT_ResultCodeUint32;

/* 激活或去激活测试模式 */
typedef struct {
    AT_VTC_TestModeActiveStateUint8 state;
    AT_VTC_TestModeTypeUint8        type;
    VOS_UINT8                       reserved[2];
} AT_VTC_TestModeActiveStatePara;

/* VTC回复AT测试模式激活状态查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32        result;
    AT_VTC_TestModeActiveStatePara para;
} VTC_AT_TestModeActiveStateQryCnf;

/* 测试模式E闭合状态 */
typedef struct {
    VOS_UINT32                         length;
    AT_VTC_TestModeCloseStatusUint8    status;
    AT_VTC_CommunicationDirectionUint8 direction;
    AT_VTC_MonitorListFormatUint8      format;
    VOS_CHAR                           monitorList[AT_CCUTLE_MONITOR_LIST_CONTEXT_MAX_NUM + 1];
} AT_VTC_TestModeECloseStatePara;

/* VTC回复AT测试模式闭合状态查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32        result;
    AT_VTC_TestModeECloseStatePara para;
} VTC_AT_TestModeECloseStateQryCnf;

/* sidelink数据包数量 */
typedef struct {
    VOS_UINT32 lenOfPscchTransportBlocks; /* 描述PSCCH传输块数量的字符长度 */
    VOS_UINT32 lenOfStchPdcpSduPackets;   /* 描述STCH PDCP SDU数据包数量的字符长度 */
    VOS_UINT32 lenOfPsschTransportBlocks; /* 描述PSSCH传输块数量的字符长度 */
    VOS_CHAR   numOfPscchTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1]; /* PSCCH传输块数量 */
    VOS_CHAR   numOfStchPdcpSduPackets[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1];   /* STCH PDCP SDU包数量 */
    VOS_CHAR   numOfPsschTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1]; /* PSSCH传输块数量 */
    VOS_UINT8  reserved1;
} VTC_AT_SidelinkPacketCounterPara;

/* VTC回复AT数据包数量查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32          result;
    VTC_AT_SidelinkPacketCounterPara para;
} VTC_AT_SidelinkPacketCounterQryCnf;

/* VTC测量到的信道忙率参数 */
typedef struct {
    VOS_UINT32 isPscchCbrValid; /* PSCCH信道忙率参数是否有效 */
    VOS_UINT32 psschCbr;        /* PSSCH信道忙率 */
    VOS_UINT32 pscchCbr;        /* PSCCH信道忙率 */
} VTC_AT_CbrPara;

/* VTC回复AT信道忙率查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
    VTC_AT_CbrPara          para;
} VTC_AT_CbrQryCnf;

/* PC5上数据发送配置 */
typedef struct {
    AT_VTC_SendingDataActionUint8 action; /* 停止或开始发送 */
    VOS_UINT8                     reserved[3];
    VOS_UINT32                    dataSize;    /* 数据包大小 */
    VOS_UINT32                    periodicity; /* 发送周期 */
} AT_VTC_SendingDataActionPara;

/* VTC回复AT PC5上数据发送配置查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32      result;
    AT_VTC_SendingDataActionPara para;
} VTC_AT_SendingDataActionQryCnf;

/* 测试模式下修改L2ID */
typedef struct {
    VOS_UINT32 srcId;
    VOS_UINT32 dstId;
} AT_VTC_TestModeModifyL2IdPara;

/* VTC回复AT测试模式L2ID查询结果 */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
    AT_VTC_TestModeModifyL2IdPara para;
} VTC_AT_TestModeModifyL2IdQryCnf;

/* 同步状态参数 */
typedef struct {
    VOS_UINT32                    earfcn;    /* 当前同步源工作频点 */
    VRRC_AT_SyncSourceTypeUint8   type;      /* 同步源类型 */
    VRRC_AT_SyncSourceStatusUint8 status;    /* 同步源状态 */
    VOS_UINT16                    slssId;    /* 值范围0-335 */
    VOS_UINT16                    subSlssId; /* 值范围0-65535 */
    VOS_UINT8                     reserved[2];
} VRRC_AT_SyncSourceStatePara;

/* VRRC回复AT同步状态查询结果 */
typedef struct {
    VOS_UINT32                  result;
    VRRC_AT_SyncSourceStatePara para;
} VRRC_AT_SyncSourceStateQryCnf;

/* 同步状态上报开关 */
typedef struct {
    AT_VRRC_SyncSourceRptSwitchUint8 status;
    VOS_UINT8                        reserved[3];
} AT_VRRC_SyncSourceRptSwitchPara;

/* VRRC回复AT同步状态上报开关查询结果 */
typedef struct {
    VOS_UINT32                      result;
    AT_VRRC_SyncSourceRptSwitchPara para;
} VRRC_AT_SyncSourceRptSwitchQryCnf;

/* 同步模式 */
typedef struct {
    AT_VRRC_SyncModeUint8 mode;
    VOS_UINT8             reserved[3];
} AT_VRRC_SyncModePara;

/* VRRC回复AT同步模式查询结果 */
typedef struct {
    VOS_UINT32           result;
    AT_VRRC_SyncModePara para;
} VRRC_AT_SyncModeQryCnf;

/* PC5上RSSI参数 */
typedef struct {
    VOS_INT16 rssi;
    VOS_INT16 reserved;
} VMAC_AT_Pc5RssiPara;

/* VMAC获取到的CBR参数 */
typedef struct {
    VOS_UINT32 psschCbr;
} VMAC_AT_CbrPara;

/* VMAC回复AT CBR查询结果 */
typedef struct {
    VOS_UINT32      result;
    VMAC_AT_CbrPara para;
} VMAC_AT_CbrQryCnf;

/* VTC回复AT设置结果 */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
} VTC_AT_MsgSetCnf;

/* VRRC回复AT设置结果 */
typedef struct {
    VOS_UINT32 result;
} VRRC_AT_MsgSetCnf;

/* AT发送消息到VTC */
typedef struct {
    VOS_MSG_HEADER
    AT_VTC_MsgTypeUint32 msgName;
    MN_CLIENT_ID_T       clientId;
    MN_OPERATION_ID_T    opId;
    VOS_UINT8            reserved;
    VOS_UINT8            content[0];  //lint !e43
} AT_VTC_MsgReq;

/* VTC回复消息给AT */
typedef struct {
    VOS_MSG_HEADER
    AT_VTC_MsgTypeUint32 msgName;
    MN_CLIENT_ID_T       clientId;
    VOS_UINT8            reserved[2];
    VOS_UINT8            content[0];  //lint !e43
} VTC_AT_MsgCnf;

VOS_UINT32 AT_SndVtcMsg(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT32 msgName, const VOS_UINT8 *data,
                        VOS_UINT32 dataSize);


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    AT_VRRC_MsgTypeUint16 msgName;     /* 消息名 */
    VOS_UINT8             reserve1[2]; /* 保留 */
    MN_CLIENT_ID_T        clientId;    /* Client ID */
    MN_OPERATION_ID_T     opId;        /* Operation ID */
    VOS_UINT8             reserve2;    /* 保留 */
    VOS_UINT8             content[4];  /* 消息内容 */
} AT_VRRC_ReqMsg;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 syncSrc;
    VOS_UINT8 syncStat;
    VOS_UINT8 reserved;
} VRRC_AT_SyncstQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 syncStat;
    VOS_UINT8 reserve[2]; /* 保留 */
} VRRC_AT_GnsssyncstQryCnf;

/*
 * 结构名: TAF_GNSS_SysType
 * 结构说明: 卫星系统类型的枚举
 */
enum TAF_GNSS_SysType {
    GNSS_SYS_GPS = 0x0, /* GPS */
    GNSS_SYS_GALILEO,   /* Galileo */
    GNSS_SYS_GLONASS,   /* GLONASS */
    GNSS_SYS_QZSS,      /* QZSS */
    GNSS_SYS_SBAS,      /* SBAS */
    GNSS_SYS_BDS        /* BDS */
};
typedef VOS_UINT16 TAF_GNSS_SysTypeUint16;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 perPpsStat;
    VOS_UINT8 syncStat;
    VOS_UINT8 satNum;
    VOS_UINT8 sysSatNum[GNSS_SYS_BDS + 1];
    VOS_UINT8 rev[2];
} VRRC_AT_GnssQryCnf;

/*
 * 结构名: TAF_GNSS_SysInfo
 * 结构说明: GNSS系统类型及对应信噪比
 */
typedef struct {
    TAF_GNSS_SysTypeUint16 gnssSystem; /* 卫星系统类型 */
    VOS_INT16              cno;        /* C/N0（信噪比） */
} TAF_GNSS_SysInfo;

#define MAX_SATNUM 30  // 最大搜星个数
#define MAXLEN 12


typedef struct {
    VOS_UINT8        qryRslt; /* 查询success or failure */
    VOS_UINT8        satNum;  /* 搜星数目 */
    VOS_UINT8        rev[2];
    VOS_UINT8        longitude[MAXLEN];   /* 经度 */
    VOS_UINT8        latitude[MAXLEN];    /* 纬度 */
    TAF_GNSS_SysInfo sysInfo[MAX_SATNUM]; /* GNSS系统类型及对应信噪比 */
} AT_VRRC_GnssDebugInfo;

typedef struct {
    VOS_UINT32 frequencyBand;
    VOS_UINT32 bandWidth;
    VOS_UINT32 centralFrequency;
} AT_VRRC_Slinfo;

typedef struct {
    TAF_ERROR_CodeUint32 result;
    VOS_UINT32           frequencyBand;
    VOS_UINT32           bandWidth;
    VOS_UINT32           centralFrequency;
} VRRC_AT_SlinfoQryCnf;

typedef struct {
    VOS_UINT32 groupType;
    VOS_UINT32 rtType;
    VOS_UINT32 poolId;
    VOS_UINT32 slOffsetIndicator;
    VOS_UINT8  bitmapLen;
    VOS_UINT8  subChanBitmap[15];
    VOS_UINT32 adjacency;
    VOS_UINT32 subChnSize;
    VOS_UINT32 subChnNum;
    VOS_UINT32 subChnStartRB;
    VOS_UINT32 pscchPoolStartRB;
    VOS_UINT32 poolNum;
} AT_VRRC_RppcfgSet;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_RppcfgSetCnf;

typedef struct {
    VOS_UINT32 groupType;
    VOS_UINT32 rtType;
} AT_VRRC_RppcfgQry;

typedef struct {
    VOS_UINT32 poolId;
    VOS_UINT32 slOffsetIndicator;
    VOS_UINT8  bitmapLen;
    VOS_UINT8  subChanBitmap[15];
    VOS_UINT32 adjacency;
    VOS_UINT32 subChnSize;
    VOS_UINT32 subChnNum;
    VOS_UINT32 subChnStartRB;
    VOS_UINT32 pscchPoolStartRB;
} AT_VRRC_RppcfgRespoolPara;

typedef struct {
    TAF_ERROR_CodeUint32      result;
    VOS_UINT32                groupType;
    VOS_UINT32                rtType;
    VOS_UINT32                poolNum;
    AT_VRRC_RppcfgRespoolPara resPool[16];
} VRRC_AT_RppcfgQryCnf;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_RsuSlcfgstRpt;


typedef struct {
    VOS_MSG_HEADER                     /* _H2ASN_Skip */
    AT_VMAC_MsgTypeUint16 msgName;     /* 消息名 */
    VOS_UINT8             reserve1[2]; /* 保留 */
    MN_CLIENT_ID_T        clientId;    /* Client ID */
    MN_OPERATION_ID_T     opId;        /* Operation ID */
    VOS_UINT8             reserve2;    /* 保留 */
    VOS_UINT8             content[4];  /* 消息内容 */
} AT_VMAC_ReqMsg;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VMAC_AT_RsuSlptrstRpt;


typedef struct {
    VOS_INT8  maxTxPwr;
    VOS_UINT8 minMcs;
    VOS_UINT8 maxMcs;
    VOS_UINT8 minSubChn;
    VOS_UINT8 maxSubChn;
    VOS_UINT8 rsv[3];
} AT_VRRC_PhyrSet;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_PhyrSetCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_INT8  maxTxPwr;
    VOS_UINT8 minMcs;
    VOS_UINT8 maxMcs;
    VOS_UINT8 minSubchn;
    VOS_UINT8 maxSubchn;
    VOS_UINT8 rsv[2];
} VRRC_AT_PhyrQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT32 firstSaDecSucSum; /* 初传SA译码正确个数 */
    VOS_UINT32 secSaDecSucSum;   /* 重传SA译码正确个数 */
    VOS_UINT32 twiSaDecSucSum;   /* 两次SA译码正确个数 */
    VOS_UINT32 uplinkPackSum;    /* 发送端收到L2包统计 */
    VOS_UINT32 downlinkPackSum;  /* 接收端收包统计 */
    VOS_UINT32 phyUplinkPackSum; /* 上行业务发包数 */
} VRRC_AT_VphystatQryCnf;


typedef struct {
    VOS_UINT8 vphyStatClrFlg;
    VOS_UINT8 rsv[3];
} AT_VRRC_VphystatClrSet;


typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_VphystatClrSetCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT8 msgNum;             /* SA译码成功个数 */
    VOS_INT8  saMainSNR[10];      /* SA主集SNR */
    VOS_INT8  saDiversitySNR[10]; /* SA分集SNR */
    VOS_INT8  daMainSNR[10];      /* DA主集SNR */
    VOS_INT8  daDiversitySNR[10]; /* DA分集SNR */
    VOS_INT8  daRsrp[10];         /* DA RSRP */
    VOS_UINT8 reserved;

} VRRC_AT_VsnrrsrpQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询success or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT8 msgNum;             /* RSSI按照子带上报，该值表示RSSI有效个数 */
    VOS_INT8  daMainRSSIMax;      /* 主集各子带RSSI最大值 */
    VOS_INT8  daDiversityRSSIMax; /* 分集各子带RSSI最大值 */
    VOS_UINT8 reserved;

} VRRC_AT_RssiQryCnf;


typedef struct {
    VOS_UINT16 qryRslt; /* 查询success or failure */
    VOS_INT16  vrssi;
} VMAC_AT_VrssiQryCnf;


typedef struct {
    VOS_UINT8  saRbStart;
    VOS_UINT8  daRbNum;
    VOS_UINT8  mcs;
    VOS_UINT8  flag;
    VOS_UINT16 sendFrmMask;
    VOS_UINT16 period;
} AT_VTC_SendVMacDataPara;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VTC_AT_SendVMacDataCnf;

/* 配置输出功率 */
typedef struct {
    VOS_INT16 setTxPower; /* 单位为0.125dBm(setTxPower=80表示当前设置功率为10dBm) */
    VOS_UINT8 rsv[2];
} AT_VRRC_Set_Tx_PowerPara;

/* PHY回复设置输出功率结果 */
typedef struct {
    VOS_UINT16 result;
    VOS_INT16 txPower;
} VRRC_AT_Set_Tx_PowerQryCnf;

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    AT_VPDCP_MsgTypeUint16 msgName;     /* 消息名 */
    VOS_UINT8              reserve1[2]; /* 保留 */
    MN_CLIENT_ID_T         clientId;    /* Client ID */
    MN_OPERATION_ID_T      opId;        /* Operation ID */
    VOS_UINT8              reserve2;    /* 保留 */
    VOS_UINT8              content[4];  /* 消息内容 */
} AT_VPDCP_ReqMsg;

typedef struct {
    VOS_UINT32 qryRslt; /* 查询success or failure */
    VOS_UINT32 pc5TxBytes;
    VOS_UINT32 pc5RxBytes;
    VOS_UINT32 pc5TxPkts;
    VOS_UINT32 pc5RxPkts;
} VPDCP_AT_PtrrptQryCnf;

typedef struct {
    VOS_UINT32 groupId;
    VOS_UINT32 selfRsuId;
    VOS_UINT32 selfRsuType;
    VOS_UINT32 indoorRsuId;
    VOS_UINT32 rsuListNum;
    VOS_UINT32 rsuIdList[10];
} AT_VMAC_McgCfgSet;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VMAC_AT_McgCfgSetCnf;

typedef struct {
    VOS_UINT32 dstId;
    VOS_UINT32 tmType;
} AT_VMAC_DstidtmInfo;

typedef struct {
    VOS_UINT32          dstIdNum;
    AT_VMAC_DstidtmInfo dstIdList[10];
} AT_VMAC_DstidtmSet;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VMAC_AT_DstidtmSetCnf;

typedef struct {
    VOS_UINT8 syncSwitch;
    VOS_UINT8 syncDfnSwitch;
    VOS_UINT8 gnssValidTimerLen;
    VOS_UINT8 syncOffsetInd1;
    VOS_UINT8 syncOffsetInd2;
    VOS_UINT8 syncOffsetInd3;
    VOS_UINT8 syncTxThreshOoc;
    VOS_UINT8 fltCoefficient;
    VOS_UINT8 syncRefMinHyst;
    VOS_UINT8 syncRefDiffHyst;
    VOS_UINT8 reserved[2];
} AT_VRRC_Pc5syncSet;

typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_Pc5syncSetCnf;

typedef struct {
    VOS_UINT8 qryRslt;
    VOS_UINT8 syncSwitch;
    VOS_UINT8 syncDfnSwitch;
    VOS_UINT8 gnssValidTimerLen;
    VOS_UINT8 syncOffsetInd1;
    VOS_UINT8 syncOffsetInd2;
    VOS_UINT8 syncOffsetInd3;
    VOS_UINT8 syncTxThreshOoc;
    VOS_UINT8 fltCoefficient;
    VOS_UINT8 syncRefMinHyst;
    VOS_UINT8 syncRefDiffHyst;
    VOS_UINT8 reserved[1];
} VRRC_AT_Pc5syncQryCnf;

#endif
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
