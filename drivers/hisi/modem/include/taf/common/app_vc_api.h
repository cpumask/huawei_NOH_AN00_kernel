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
#ifndef APP_VC_API_H
#define APP_VC_API_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "taf_type_def.h"
#include "vos.h"

#pragma pack(push, 4)

#define APP_VC_MSD_DATA_LEN (140) /* 单位:btye */

#define VC_CALL_ECALL_REDIAL_CLEAN (10) /* PSAP要求IVS主动挂断 */
#define VOC_CONFIG_NO_CHANGE (2001)

/*
 * 3类型定义
 */
/* 语音设备：0 手持；1 手持免提；2 车载免提；3 耳机；4 蓝牙；7 超级免提；8 智音通话 */
enum VC_PHY_DeviceMode {
    VC_PHY_DEVICE_MODE_HANDSET,        /* 手持 */
    VC_PHY_DEVICE_MODE_HANDS_FREE,     /* 手持免提 */
    VC_PHY_DEVICE_MODE_CAR_FREE,       /* 车载免提 */
    VC_PHY_DEVICE_MODE_EARPHONE,       /* 耳机 */
    VC_PHY_DEVICE_MODE_BLUETOOTH,      /* 蓝牙 */
    VC_PHY_DEVICE_MODE_PCVOICE,        /* PC语音模式 */
    VC_PHY_DEVICE_MODE_HEADPHONE,      /* 不带耳机MIC的耳机通话 */
    VC_PHY_DEVICE_MODE_SUPER_HANDFREE, /* 超级免提 */
    VC_PHY_DEVICE_MODE_SMART_TALK,     /* 智音通话 */
    VC_PHY_DEVICE_MODEM_USBVOICE, /* USB设备 */
    VC_PHY_DEVICE_MODE_BUTT /* invalid value */
};
typedef VOS_UINT16 VC_PHY_DeviceModeUint16;

/*
 * 结构说明: STICK形态下，用户通过^CVOICE私有命令设置的语音模式枚举
 */
enum APP_VC_VoiceMode {
    APP_VC_VOICE_MODE_PCVOICE    = 0,
    APP_VC_VOICE_MODE_EARPHONE   = 1,
    APP_VC_VOICE_MODE_HANDSET    = 2,
    APP_VC_VOICE_MODE_HANDS_FREE = 3,
    APP_VC_VOICE_MODE_BUTT
};
typedef VOS_UINT16 APP_VC_VoiceModeUint16;

/*
 * 结构说明: 语言输出端口枚举
 */
enum APP_VC_VoicePort {
    APP_VC_VOICE_PORT_MODEM   = 1,
    APP_VC_VOICE_PORT_DIAG    = 2,
    APP_VC_VOICE_PORT_PCUI    = 3,
    APP_VC_VOICE_PORT_PCVOICE = 4,
    APP_VC_VOICE_PORT_PCSC    = 5,
    APP_VC_VOICE_PORT_BUTT
};
typedef VOS_UINT8 APP_VC_VoicePortUint8;

enum APP_VC_Event {
    APP_VC_EVT_PARM_CHANGED, /* service para changed */
    APP_VC_EVT_SET_VOLUME,
    APP_VC_EVT_SET_VOICE_MODE,
    APP_VC_EVT_GET_VOLUME,
    APP_VC_EVT_SET_MUTE_STATUS,
    APP_VC_EVT_GET_MUTE_STATUS,

    APP_VC_EVT_SET_ECALL_CFG,
    APP_VC_EVT_ECALL_TRANS_STATUS,
    APP_VC_EVT_ECALL_TRANS_ALACK,
    APP_VC_EVT_ECALL_ABORT_CNF,
    APP_VC_EVT_BUTT
};
typedef VOS_UINT32 APP_VC_EventUint32;

enum APP_VC_MuteStatus {
    APP_VC_MUTE_STATUS_UNMUTED = 0,
    APP_VC_MUTE_STATUS_MUTED   = 1,
    APP_VC_MUTE_STATUS_BUTT
};
typedef VOS_UINT8 APP_VC_MuteStatusUint8;


enum TAF_VC_Ttymode {
    TAF_VC_TTY_OFF  = 0,
    TAF_VC_TTY_FULL = 1,
    TAF_VC_TTY_VCO  = 2,
    TAF_VC_TTY_HCO  = 3,
    TAF_VC_TTYMODE_BUTT
};
typedef VOS_UINT8 TAF_VC_TtymodeUint8;


enum APP_VC_Msg {
    /* 设置音量请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_SET_VOLUME = 0,
    /* 设置通话模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_SET_MODE,

    /* 查询通话模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_QRY_MODE,
    /* 查询通话模式回复消息 */
    /* _H2ASN_MsgChoice APP_VC_EventInfo */
    APP_VC_MSG_CNF_QRY_MODE,

    /* 设置语音端口请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_SET_PORT,
    /* 设置语音端口回复消息 */
    /* _H2ASN_MsgChoice APP_VC_EventInfo */
    APP_VC_MSG_CNF_SET_PORT,

    /* 查询语音端口请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_QRY_PORT,
    /* 查询语音端口回复消息 */
    /* _H2ASN_MsgChoice APP_VC_EventInfo */
    APP_VC_MSG_CNF_QRY_PORT,

    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_REQ_QRY_VOLUME,
    APP_VC_MSG_CNF_QRY_VOLUME,

    /* 设置静音状态请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_SET_MUTE_STATUS_REQ,
    /* 获取静音状态请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_GET_MUTE_STATUS_REQ,

    /* 设置前台模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_SET_FOREGROUND_REQ,
    /* 设置前台模式回复消息 */
    /* _H2ASN_MsgChoice APP_VC_MsgCnfAsn */
    APP_VC_MSG_SET_FOREGROUND_CNF,

    /* 设置后台模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_SET_BACKGROUND_REQ,
    /* 设置后台模式回复消息 */
    /* _H2ASN_MsgChoice APP_VC_MsgCnfAsn */
    APP_VC_MSG_SET_BACKGROUND_CNF,

    /* 模式查询请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_FOREGROUND_QRY,
    /* 模式查询回复消息 */
    /* _H2ASN_MsgChoice APP_VC_QryGroungRsp */
    APP_VC_MSG_FOREGROUND_RSP,

    /* 设置语音环回模式消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_SET_MODEMLOOP_REQ,

    /* 设置MSD数据请求 */
    /* _H2ASN_MsgChoice APP_VC_MsgSetMsdReq */
    APP_VC_MSG_SET_MSD_REQ,
    /* 设置MSD数据回复 */
    /* _H2ASN_MsgChoice APP_VC_MsgCnfAsn */
    APP_VC_MSG_SET_MSD_CNF,

    /* 查询MSD数据请求 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_QRY_MSD_REQ,
    /* 查询MSD数据回复 */
    /* _H2ASN_MsgChoice APP_VC_MsgQryMsdCnf */
    APP_VC_MSG_QRY_MSD_CNF,

    /* 设置ecall配置请求 */
    /* _H2ASN_MsgChoice APP_VC_MsgSetEcallCfgReq */
    APP_VC_MSG_SET_ECALL_CFG_REQ,

    /* 查询ecall配置信息请求 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_QRY_ECALL_CFG_REQ,
    /* 查询ecall配置信息回复 */
    /* _H2ASN_MsgChoice APP_VC_MsgQryEcallCfgCnf */
    APP_VC_MSG_QRY_ECALL_CFG_CNF,

    /* DTMF Decoder Ind上报消息 */
    /* _H2ASN_MsgChoice APP_VC_DtmfDecoderInd */
    APP_VC_MSG_DTMF_DECODER_IND,

    /* 设置TTY模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_SetTtymodeReq */
    APP_VC_MSG_SET_TTYMODE_REQ,
    /* 设置TTY模式回复消息 */
    /* _H2ASN_MsgChoice APP_VC_MsgCnfAsn */
    APP_VC_MSG_SET_TTYMODE_CNF,
    /* 查询TTY模式请求消息 */
    /* _H2ASN_MsgChoice APP_VC_ReqMsgAsn */
    APP_VC_MSG_QRY_TTYMODE_REQ,
    /* 查询TTY模式回复消息 */
    /* _H2ASN_MsgChoice APP_VC_QryTtymodeCnf */
    APP_VC_MSG_QRY_TTYMODE_CNF,

    APP_VC_MSG_HIFI_RESET_BEGIN_NTF,
    APP_VC_MSG_HIFI_RESET_END_NTF,

    APP_VC_MSG_SET_ECALL_OPRTMODE_REQ,
    APP_VC_MSG_SET_ECALL_PUSH_REQ,
    APP_VC_MSG_SET_ECALL_PUSH_CNF,
    APP_VC_MSG_SET_ECALL_ABORT_REQ,

    APP_VC_MSG_I2S_TEST_REQ,
    APP_VC_MSG_I2S_TEST_CNF,
    APP_VC_MSG_I2S_TEST_RSLT_IND,

    APP_VC_MSG_BUTT
};
typedef VOS_UINT16 APP_VC_MsgUint16;


enum APP_VC_QryGround {
    APP_VC_FOREGROUND     = 0,
    APP_VC_BACKGROUND     = 1,
    APP_VC_GROUND_INVALID = 2,
    APP_VC_GROUND_BUTT
};
typedef VOS_UINT16 APP_VC_QryGroundUint16;


enum APP_VC_StartHifiOrig {
    APP_VC_START_HIFI_ORIG_GUCALL = 0,
    APP_VC_START_HIFI_ORIG_IMSA   = 1,
    APP_VC_START_HIFI_ORIG_XCALL  = 2,
    APP_VC_START_HIFI_ORIG_BUTT
};
typedef VOS_UINT8 APP_VC_StartHifiOrigUint8;


enum APP_VC_EcallTransStatus {
    APP_VC_ECALL_MSD_TRANSMITTING_START = 0,   /* 当前MSD数据已经开始发送 */
    APP_VC_ECALL_MSD_TRANSMITTING_SUCCESS,     /* 当前MSD数据已经发送成功 */
    APP_VC_ECALL_MSD_TRANSMITTING_FAIL,        /* 当前MSD数据已经发送失败 */
    APP_VC_ECALL_PSAP_MSD_REQUIRETRANSMITTING, /* PSAP要求更新MSD数据 */
    APP_VC_ECALL_TRANS_STATUS_BUTT
};
typedef VOS_UINT8 APP_VC_EcallTransStatusUint8;


enum APP_VC_EcallTransFailCause {
    APP_VC_ECALL_CAUSE_WAIT_PSAP_TIME_OUT = 0,    /* 等待PSAP的传输指示超时 */
    APP_VC_ECALL_CAUSE_MSD_TRANSMITTING_TIME_OUT, /* MSD数据传输超时 */
    APP_VC_ECALL_CAUSE_WAIT_AL_ACK_TIME_OUT,      /* 等待应用层的确认超时 */
    APP_VC_ECALL_CAUSE_UNSPECIFIC_ERROR,          /* 其他异常错误 */
    APP_VC_ECALL_CAUSE_NW_ERROR,                  /* 网络错误 */
    APP_VC_ECALL_CAUSE_BUTT
};
typedef VOS_UINT8 APP_VC_EcallTransFailCauseUint8;


enum APP_VC_EcallMsdMode {
    APP_VC_ECALL_MSD_MODE_TRANSPARENT = 0, /* 透传模式 */
    APP_VC_ECALL_MSD_MODE_BUTT
};
typedef VOS_UINT16 APP_VC_EcallMsdModeUint16;


enum APP_VC_EcallVocConfig {
    APP_VC_ECALL_VOC_CONFIG_NOT_ABANDON = 0, /* MSD数据传输过程带语音 */
    APP_VC_ECALL_VOC_CONFIG_ABANDON,         /* MSD数据传输过程不带语音 */
    APP_VC_ECALL_VOC_CONFIG_BUTT
};
typedef VOS_UINT16 APP_VC_EcallVocConfigUint16;


enum APP_VC_EcallOprtMode {
    APP_VC_ECALL_OPRT_PULL = 0,
    APP_VC_ECALL_OPRT_PUSH,
    APP_VC_ECALL_OPRT_BUTT
};
typedef VOS_UINT8 APP_VC_EcallOprtModeUint8;


enum APP_VC_MsdSendMode {
    APP_VC_ECALL_MSD_SEND_SIP = 1,
    APP_VC_ECALL_MSD_SEND_IN_BAND = 2,
    APP_VC_ECALL_MSD_SEND_MODE_BUTT
};
typedef VOS_UINT8 APP_VC_MsdSendModeUint8;


enum APP_VC_I2sTestRslt {
    APP_VC_I2S_TEST_RSLT_SUCCESS  = 0x0000,
    APP_VC_I2S_TEST_RSLT_FAILED   = 0x0001,
    APP_VC_I2S_TEST_RSLT_TIME_OUT = 0x0002,
    APP_VC_I2S_TEST_RSLT_BUTT
};
typedef VOS_UINT32 APP_VC_I2sTestRsltUint32;


enum APP_VC_HifiResetType {
    APP_VC_HIFI_RESET_FROM_AP = 0, /* SOC或拼片AP侧触发HIFI RESET */
    APP_VC_HIFI_RESET_FROM_CP = 1, /* 拼片CP侧触发HIFI RESET */

    APP_VC_HIFI_RESET_TYPE_BUTT
};
typedef VOS_UINT8 APP_VC_HifiResetTypeUint8;


struct APP_VC_HifiResetInfo {
    APP_VC_HifiResetTypeUint8 hifiResetType; /* 设置HIFI RESET类型 */
    VOS_UINT8                 reserved[3];
};

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    APP_VC_MsgUint16  msgName;     /* 消息名 */
    VOS_UINT8         reserve1[2]; /* 保留 */
    MN_CLIENT_ID_T    clientId;    /* Client ID */
    MN_OPERATION_ID_T opId;        /* Operation ID */
    VOS_UINT8         reserve2;    /* 保留 */
    VOS_UINT8         content[4];  /* 消息内容 */
} APP_VC_ReqMsg;


typedef struct {
    VOS_BOOL                success; /* set(get) success or failure */
    VOS_UINT32              errCode;
    MN_CLIENT_ID_T          clientId;
    MN_OPERATION_ID_T       opId;
    APP_VC_VoicePortUint8   voicePort;
    VC_PHY_DeviceModeUint16 devMode;
    VOS_UINT16              volume;
    APP_VC_MuteStatusUint8  muteStatus;
    VOS_UINT8               rsv[2];

    APP_VC_EcallTransStatusUint8 ecallState;
    VOS_UINT32                   ecallDescription;
    VOS_UINT8                    ecallReportAlackValue;
    VOS_UINT8                    reserved[3];
} APP_VC_EventInfo;


typedef struct {
    VOS_UINT8              qryRslt; /* 查询 success or failure */
    VOS_UINT8              reserved;
    APP_VC_QryGroundUint16 ground; /* 查询结果 */
} APP_VC_QryGroungRsp;


typedef struct {
    VOS_UINT8 msdData[APP_VC_MSD_DATA_LEN]; /* MSD数据内容 */
} APP_VC_MsgSetMsdReq;


typedef struct {
    VOS_UINT8 qryRslt; /* 查询结果 */
    VOS_UINT8 reserved[3];
    VOS_UINT8 msdData[APP_VC_MSD_DATA_LEN]; /* MSD数据内容 */
} APP_VC_MsgQryMsdCnf;


typedef struct {
    APP_VC_EcallMsdModeUint16   mode;      /* 数据传送模式 */
    APP_VC_EcallVocConfigUint16 vocConfig; /* 传送数据时是否打开声码器 */
} APP_VC_MsgSetEcallCfgReq;


typedef struct {
    VOS_UINT8                   qryRslt; /* 查询结果 */
    VOS_UINT8                   reserved[3];
    APP_VC_EcallMsdModeUint16   mode;      /* MSD数据传送模式 */
    APP_VC_EcallVocConfigUint16 vocConfig; /* 传送数据时是否打开声码器 */
} APP_VC_MsgQryEcallCfgCnf;


typedef struct {
    VOS_UINT8 rslt; /* 结果 */
    VOS_UINT8 reserved[3];
} APP_VC_SetMsdCnf;


typedef struct {
    VOS_UINT32 result; /* 结果 */
} APP_VC_I2sTestCnf;


typedef struct {
    APP_VC_I2sTestRsltUint32 result; /* 结果 */
} APP_VC_I2sTestRsltInd;


typedef struct {
    APP_VC_EcallOprtModeUint8 ecallOpMode; /* operate mode:pull or push */
    VOS_UINT8                 reserved[3];
} APP_VC_SetOprtmodeReq;


typedef struct {
    VOS_UINT8           qryRslt; /* 查询 success or failure */
    TAF_VC_TtymodeUint8 ttyMode; /* 查询结果 */
    VOS_UINT8           reserved[2];
} APP_VC_QryTtymodeCnf;


typedef struct {
    TAF_VC_TtymodeUint8 ttyMode; /* 设置TTY MODE */
    VOS_UINT8           reserved[3];
} APP_VC_SetTtymodeReq;


typedef struct {
    VOS_UINT8 rslt;
    VOS_UINT8 reserved[3];
} APP_VC_MsgCnfAsn;


typedef struct {
    VOS_UINT8 content[4];
} APP_VC_ReqMsgAsn;


VOS_UINT32 APP_VC_SetVoiceVolume(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT8 voiceVolume);


VOS_UINT32 APP_VC_SetVoiceMode(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, VOS_UINT8 voiceMode, VOS_UINT32 rcvPid);


VOS_UINT32 APP_VC_SetVoicePort(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, APP_VC_VoicePortUint8 voicePort);


VOS_UINT32 APP_VC_GetVoicePort(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);


VOS_UINT32 APP_VC_GetVoiceMode(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);


extern VC_PHY_DeviceModeUint16 APP_VC_AppVcVoiceMode2VcPhyVoiceMode(APP_VC_VoiceModeUint16 voiceMode);


extern APP_VC_VoiceModeUint16 APP_VC_VcPhyVoiceMode2AppVcVoiceMode(VC_PHY_DeviceModeUint16 voiceMode);

extern VOS_UINT32 APP_VC_GetVoiceVolume(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId);

/*
 * 功能描述: 设置静音状态
 */
VOS_UINT32 APP_VC_SetMuteStatus(VOS_UINT16 clientId, VOS_UINT8 opId, APP_VC_MuteStatusUint8 muteStatus);

/*
 * 功能描述: 获取静音状态
 */
VOS_UINT32 APP_VC_GetMuteStatus(VOS_UINT16 clientId, VOS_UINT8 opId);

/*
 * 功能描述: 设置进入或退出语音换回
 */
VOS_UINT32 APP_VC_SetModemLoop(VOS_UINT16 clientId, VOS_UINT8 opId, VOS_UINT8 modemLoop);

VOS_UINT32 APP_VC_SendHifiResetNtf(VOS_UINT32 rcvPid, APP_VC_HifiResetTypeUint8 hifiResetType, APP_VC_MsgUint16 msgName);


typedef struct {
    VOS_CHAR   ucDtmfCode;
    VOS_UINT8  ucReserved;
    VOS_UINT16 usOntime;
} APP_VC_DtmfDecoderInd;



#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* APP_VC_API_H */
