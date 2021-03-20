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
#ifndef _TAF_MTC_API_H_
#define _TAF_MTC_API_H_

#include "vos.h"
#include "taf_type_def.h"
#include "PsTypeDef.h"
#include "taf_api.h"
#if (VOS_WIN32 == VOS_OS_VER)
#ifndef DMT
#include "UT_Stub.h"
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* MTC �ṩ����ϢID����, ��0x1001��ʼ����ҪΪ�˱�����������Ϣ�غ� */
#define TAF_MTC_MSG_ID_BASE (0x1000)

#define MTC_NONE_CS_VALUE (0x00)                          /* �޵绰 */
#define MTC_GU_CS_VALUE (MTC_SET_BIT(MTC_CS_TYPE_GU))     /* GU�绰 */
#define MTC_IMS_CS_VALUE (MTC_SET_BIT(MTC_CS_TYPE_IMS))   /* IMS�绰 */
#define MTC_CDMA_CS_VALUE (MTC_SET_BIT(MTC_CS_TYPE_CDMA)) /* CDMA�绰 */

#define MTC_CS_TYPE_ALL_VALUE (MTC_GU_CS_VALUE | MTC_IMS_CS_VALUE | MTC_CDMA_CS_VALUE)

#define MTC_NONE_PS_VALUE (0x00)
#define MTC_GU_PS_VALUE (MTC_SET_BIT(MTC_PS_TYPE_GU))
#define MTC_LTE_PS_VALUE (MTC_SET_BIT(MTC_PS_TYPE_LTE))
#define MTC_CDMA_PS_VALUE (MTC_SET_BIT(MTC_PS_TYPE_CDMA))
#define MTC_NR_PS_VALUE (MTC_SET_BIT(MTC_PS_TYPE_NR))

#define MTC_PS_TYPE_ALL_VALUE (MTC_GU_PS_VALUE | MTC_LTE_PS_VALUE | MTC_CDMA_PS_VALUE | MTC_NR_PS_VALUE)


enum TAF_MTC_MsgId {
    /* _H2ASN_MsgChoice  TAF_MTC_CdmaStateInd */
    ID_MSG_MTC_CDMA_CONN_STATE_IND = TAF_MTC_MSG_ID_BASE + 0x0001,
    /* _H2ASN_MsgChoice  TAF_MTC_ModemConnStatusInd */
    ID_MSG_MTC_MODEM_SERVICE_CONN_STATE_IND,
    /* _H2ASN_MsgChoice  TAF_MTC_UsimmStatusInd */
    ID_MSG_MTC_USIMM_STATE_IND,
    /* _H2ASN_MsgChoice  MTC_BEGIN_SessionInd */
    ID_MSG_MTC_BEGIN_SESSION_IND,
    /* _H2ASN_MsgChoice  MTC_END_SessionInd */
    ID_MSG_MTC_END_SESSION_IND,
    /* _H2ASN_MsgChoice  MTC_POWER_SaveInd */
    ID_MSG_MTC_POWER_SAVE_IND,
    /* _H2ASN_MsgChoice  MTC_RAT_ModeInd */
    ID_MSG_MTC_RAT_MODE_IND,
    /* _H2ASN_MsgChoice  MTC_1X_ActiveFlgInd */
    ID_MSG_MTC_1X_ACTIVE_IND,

    ID_MSG_MTC_BUTT
};
typedef VOS_UINT32 TAF_MTC_MsgIdUint32;


enum TAF_MTC_SrvConnState {
    TAF_MTC_SRV_NO_EXIST = 0, /* ������ */
    TAF_MTC_SRV_EXIST,        /* ������ */

    TAF_MTC_SRV_CONN_STATE_BUTT
};
typedef VOS_UINT8 TAF_MTC_SrvConnStateUint8;


enum TAF_MTC_PowerState {
    TAF_MTC_POWER_OFF = 0, /* �ػ� */
    TAF_MTC_POWER_ON,      /* ���� */

    TAF_MTC_POWER_STATE_BUTT
};
typedef VOS_UINT8 TAF_MTC_PowerStateUint8;



enum TAF_MTC_UsimmCardServic {
    TAF_MTC_USIMM_CARD_SERVIC_ABSENT      = 0, /* �޿� */
    TAF_MTC_USIMM_CARD_SERVIC_UNAVAILABLE = 1, /* �п�,���񲻿��� */
    TAF_MTC_USIMM_CARD_SERVIC_SIM_PIN     = 2, /* SIM����������PIN��ԭ�򲻿��� */
    TAF_MTC_USIMM_CARD_SERVIC_SIM_PUK     = 3, /* SIM����������PUK��ԭ�򲻿��� */
    TAF_MTC_USIMM_CARD_SERVIC_NET_LCOK    = 4, /* SIM������������������ԭ�򲻿��� */
    TAF_MTC_USIMM_CARD_SERVIC_IMSI_LCOK   = 5, /* SIM����������IMSI����ԭ�򲻿��� */
    TAF_MTC_USIMM_CARD_SERVIC_AVAILABLE   = 6, /* ������� */

    TAF_MTC_USIMM_CARD_SERVIC_BUTT
};
typedef VOS_UINT16 TAF_MTC_UsimmCardServicUint16;



enum TAF_MTC_CdmaUsimmCard {
    TAF_MTC_CDMA_USIMM_CARD_UNVALID = 0, /* �޿� */
    TAF_MTC_CDMA_USIMM_CARD_VALID,       /* ������� */

    TAF_MTC_CDMA_USIMM_CARD_BUTT
};
typedef VOS_UINT8 TAF_MTC_CdmaUsimmCardUint8;


enum MTC_SESSION_Type {
    MTC_SESSION_TYPE_CS_MO_NORMAL_CALL        = 0,
    MTC_SESSION_TYPE_CS_MO_EMERGENCY_CALL     = 1,
    MTC_SESSION_TYPE_CS_MO_SS                 = 2,
    MTC_SESSION_TYPE_CS_MO_SMS                = 3,
    MTC_SESSION_TYPE_CS_MT_NORMAL_CALL        = 4,
    MTC_SESSION_TYPE_CS_MT_EMERGENCY_CALLBACK = 5, /* ������eCall�Ļغ����� */
    MTC_SESSION_TYPE_CS_MT_SS                 = 6,
    MTC_SESSION_TYPE_CS_MT_SMS                = 7,
    MTC_SESSION_TYPE_CS_LAU                   = 8,
    MTC_SESSION_TYPE_CS_DETACH                = 9,
    MTC_SESSION_TYPE_CS_MO_NORMAL_CSFB        = 10,
    MTC_SESSION_TYPE_CS_MO_EMERGENCY_CSFB     = 11,
    MTC_SESSION_TYPE_CS_MT_CSFB               = 12,
    MTC_SESSION_TYPE_CS_LOOP_BACK             = 13, /* ����ģʽ */
    MTC_SESSION_TYPE_TAU_COMBINED             = 14, /* ����TAU */
    MTC_SESSION_TYPE_TAU_ONLY_EPS             = 15, /* PS ONLY TAU */
    MTC_SESSION_TYPE_TAU_PERIODIC             = 16, /* ������TAU */
    MTC_SESSION_TYPE_COMBINED_ATTACH          = 17, /* GU�µ�����ATTACH */
    MTC_SESSION_TYPE_COMBINED_RAU             = 18, /* GU�µ�����RAU */
    MTC_SESSION_TYPE_CS_MT_PAGING             = 19,

    MTC_SESSION_TYPE_CS_BUTT = 20, /* ���ö��֮ǰΪCS������� */

    MTC_SESSION_TYPE_PS_CONVERSAT_CALL        = 21,
    MTC_SESSION_TYPE_PS_STREAM_CALL           = 22,
    MTC_SESSION_TYPE_PS_INTERACT_CALL         = 23,
    MTC_SESSION_TYPE_PS_BACKGROUND_CALL       = 24,
    MTC_SESSION_TYPE_PS_SUBSCRIB_TRAFFIC_CALL = 25,
    MTC_SESSION_TYPE_PS_MO_SMS                = 26,
    MTC_SESSION_TYPE_PS_MT_SMS                = 27,
    MTC_SESSION_TYPE_PS_ATTACH                = 28, /* GU�µ�ATTACH */
    MTC_SESSION_TYPE_PS_RAU                   = 29, /* GU�µ�RAU */
    MTC_SESSION_TYPE_PS_DETACH                = 30, /* GU�µ�DETACH */
    MTC_SESSION_TYPE_1X_PS_CALL               = 31, /* 1X�ϱ���PSҵ�� */

    MTC_SESSION_TYPE_NR_INITIAL_REG  = 32, /* NR�µĳ�ʼע�� */
    MTC_SESSION_TYPE_NR_MOBILITY_REG = 33, /* NR�µ��ƶ���ע�� */
    MTC_SESSION_TYPE_NR_EMC_REG      = 34, /* NR�µĽ�������ע�� */
    MTC_SESSION_TYPE_NR_NORMAL_DEREG = 35, /* NR�µ���ͨȥע�� */
    MTC_SESSION_TYPE_NR_SM           = 36, /* NR�µ�SMҵ�� */

    MTC_SESSION_TYPE_LTE_ATTACH      = 37, /* LTE�µ�ATTACH */
    MTC_SESSION_TYPE_IMSA_NORMAL_REG = 38, /* IMSA�µ���ͨע�� */

    /* ��MTC_SESSION_TYPE_CS_BUTT�����ö��ΪPS������� */
    MTC_SESSION_TYPE_BUTT = 39
};
typedef VOS_UINT8 MTC_SESSION_TypeUint8;


enum MTC_CS_Type {
    MTC_CS_TYPE_GU = 0x00, /* GU�绰 */
    MTC_CS_TYPE_IMS,       /* IMS�绰 */
    MTC_CS_TYPE_CDMA,      /* CDMA�绰 */

    MTC_CS_TYPE_BUTT
};
typedef VOS_UINT8 MTC_CS_TypeUint8;


enum MTC_PS_Type {
    MTC_PS_TYPE_GU   = 0x00, /* PS */
    MTC_PS_TYPE_LTE  = 0x01, /* EPS */
    MTC_PS_TYPE_CDMA = 0x02, /* CDMA */
    MTC_PS_TYPE_NR   = 0x03, /* NR */
    MTC_PS_TYPE_BUTT
};
typedef VOS_UINT8 MTC_PS_TypeUint8;


enum TAF_MTC_PowerSave {
    TAF_MTC_POWER_SAVE = 0,  /* ����power save */
    TAF_MTC_POWER_SAVE_EXIT, /* �˳�power save */

    TAF_MTC_POWER_SAVE_BUTT
};
typedef VOS_UINT8 TAF_MTC_PowerSaveUint8;


enum MTC_RATMODE {
    MTC_RATMODE_GSM   = 0x00,
    MTC_RATMODE_WCDMA = 0x01,
    MTC_RATMODE_LTE   = 0x02,
    MTC_RATMODE_TDS   = 0x03,
    MTC_RATMODE_1X    = 0x04,
    MTC_RATMODE_HRPD  = 0x05,
    MTC_RATMODE_NR    = 0X06,
    MTC_RATMODE_BUTT
};
typedef VOS_UINT8 MTC_RATMODE_Uint8;


typedef struct {
    TAF_MTC_SrvConnStateUint8     csConnSt;   /* CS����״̬ */
    TAF_MTC_SrvConnStateUint8     psConnSt;   /* PS����״̬ */
    TAF_MTC_PowerStateUint8       powerState; /* ���ػ�״̬ */
    VOS_UINT8                     reserved1[1];
    TAF_MTC_UsimmCardServicUint16 usimmState; /* ��״̬ */
    VOS_UINT8                     reserved2[2];
} TAF_MTC_CdmaStateInfo;


typedef struct {
    TAF_Ctrl              ctrl;
    TAF_MTC_CdmaStateInfo cdmaState;
} TAF_MTC_CdmaStateInd;


typedef struct {
    VOS_UINT32                opPsSrv : 1;
    VOS_UINT32                opCsSrv : 1;
    VOS_UINT32                opEpsSrv : 1;
    VOS_UINT32                opNrpsSrv : 1;
    VOS_UINT32                opImsSrv : 1;
    VOS_UINT32                spare : 27;
    TAF_MTC_SrvConnStateUint8 psSrvConnState;
    TAF_MTC_SrvConnStateUint8 csSrvConnState;
    TAF_MTC_SrvConnStateUint8 epsSrvConnState;
    TAF_MTC_SrvConnStateUint8 nrpsSrvConnState;
    TAF_MTC_SrvConnStateUint8 imsSrvConnState;
    VOS_UINT8                 reserved[3];
} TAF_MTC_SrvConnStateInfo;


typedef struct {
    TAF_Ctrl                 ctrl;
    TAF_MTC_SrvConnStateInfo modemConnStateInfo;
} TAF_MTC_ModemConnStatusInd;


typedef struct {
    TAF_Ctrl                      ctrl;
    TAF_MTC_UsimmCardServicUint16 usimState; /* Usim��״̬ */
    TAF_MTC_UsimmCardServicUint16 csimState; /* Csim��״̬ */
} TAF_MTC_UsimmStatusInd;


typedef struct {
    TAF_Ctrl          ctrl;
    MTC_RATMODE_Uint8 ratMode;
    VOS_UINT8         reserved[3];
} MTC_RAT_ModeInd;


typedef struct {
    TAF_Ctrl  ctrl;
    VOS_UINT8 activeFlg;
    VOS_UINT8 isSwitchOnScene;
    VOS_UINT8 reserved[2];
} MTC_1X_ActiveFlgInd;


typedef struct {
    TAF_Ctrl              ctrl;
    MTC_SESSION_TypeUint8 sessionType;
    VOS_UINT8             reserved[3];
} MTC_BEGIN_SessionInd;


typedef struct {
    TAF_Ctrl              ctrl;
    PS_BOOL_ENUM_UINT8    csRelAll;
    PS_BOOL_ENUM_UINT8    psRelAll;
    MTC_SESSION_TypeUint8 sessionType;
    VOS_UINT8             reserved;
} MTC_END_SessionInd;


typedef struct {
    TAF_Ctrl               ctrl;
    TAF_MTC_PowerSaveUint8 powerSaveStatus;
    VOS_UINT8              reserved[3];
} MTC_POWER_SaveInd;

/*
 * H2ASN������Ϣ�ṹ����
 */
typedef struct {
    /* _H2ASN_MsgChoice_Export TAF_MTC_MSG_ID_ENUM_UINT32 */
    TAF_MTC_MsgIdUint32 msgId;
    VOS_UINT8           msgBlock[4];
    /*
     * _H2ASN_MsgChoice_When_Comment          TAF_MTC_MsgIdUint32
     */
} TAF_MTC_MsgReq;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    TAF_MTC_MsgReq msgReq;
} TAF_MTC_ApiMsg;

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

VOS_UINT32 MTC_SndMsg(VOS_UINT32 taskId, VOS_UINT32 msgId, const VOS_VOID *data, VOS_UINT32 length);
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_UINT32 MTC_SetCdmaServiceConnStateInfo(TAF_Ctrl *ctrl, TAF_MTC_CdmaStateInfo *cdmsState);
#endif

VOS_UINT32 MTC_SetModemServiceConnState(TAF_Ctrl *ctrl, TAF_MTC_SrvConnStateInfo *modemConnSt);


VOS_UINT32 MTC_SetModemUsimmState(TAF_Ctrl *ctrl, TAF_MTC_UsimmCardServicUint16 usimState,
                                  TAF_MTC_UsimmCardServicUint16 csimState);

VOS_VOID MTC_SetBeginSessionInfo(TAF_Ctrl *ctrl, MTC_SESSION_TypeUint8 sessionType);

VOS_VOID MTC_SetEndSessionInfo(TAF_Ctrl *ctrl, MTC_SESSION_TypeUint8 sessionType);

VOS_VOID MTC_SetPowerSaveInfo(VOS_UINT32 sndpid, TAF_MTC_PowerSaveUint8 powerSaveStatus);

VOS_VOID MTC_SetRatModeInfo(TAF_Ctrl *ctrl, MTC_RATMODE_Uint8 ratMode);

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
VOS_VOID MTC_Set1xActiveFlg(VOS_UINT8 uc1xActiveFlg, VOS_UINT8 isSwitchOnScene);

#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
