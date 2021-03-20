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
#ifndef _AT_MNTN_H_
#define _AT_MNTN_H_

#include "vos.h"
#include "product_config.h"
#include "PsTypeDef.h"
#include "taf_type_def.h"
#include "mdrv.h"
#include "AtInternalMsg.h"
#include "AtPrivate.h"
#include "mn_client.h"
#if (!defined(MODEM_FUSION_VERSION)) || (defined(LLT_OS_VER))
#include "msp_diag_comm.h"
#include "mdrv_om.h"
#else
#include "mdrv_diag.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* Log Print Module Define */
#ifndef THIS_MODU
#define THIS_MODU ps_nas
#endif

/* 1.1: OM_GreenChannel�ĵڶ�������usPrimId�Ķ��� */
#define AT_OM_GREEN_CHANNEL_PS 0xC001 /* PS��� */

/* ��װ��ά�ɲ��¼���Ϣͷ */
#if (VOS_OS_VER == VOS_WIN32)
#define AT_MNTN_CFG_MSG_HDR(pstMsg, enEventId, ulEventLen) \
    (pstMsg)->ulSenderCpuId   = VOS_LOCAL_CPUID;           \
    (pstMsg)->ulSenderPid     = WUEPS_PID_AT;              \
    (pstMsg)->ulReceiverCpuId = VOS_LOCAL_CPUID;           \
    (pstMsg)->ulReceiverPid   = WUEPS_PID_AT;              \
    (pstMsg)->ulLength        = (ulEventLen);              \
    (pstMsg)->msgId           = (enEventId)
#else
#define AT_MNTN_CFG_MSG_HDR(pstMsg, enEventId, ulEventLen) \
    VOS_SET_SENDER_ID(pstMsg, WUEPS_PID_AT);               \
    VOS_SET_RECEIVER_ID(pstMsg, WUEPS_PID_AT);             \
    VOS_SET_MSG_LEN(pstMsg, (ulEventLen));                 \
    (pstMsg)->msgId = (enEventId)
#endif

/* C�˵�����λͳ����Ϣ */
#define AT_DBG_SET_SEM_INIT_FLAG(flag) (g_atStatsInfo.cCpuResetStatsInfo.semInitFlg = (flag))
#define AT_DBG_SAVE_BINARY_SEM_ID(sem_id) (g_atStatsInfo.cCpuResetStatsInfo.binarySemId = (sem_id))
#define AT_DBG_CREATE_BINARY_SEM_FAIL_NUM(n) (g_atStatsInfo.cCpuResetStatsInfo.createBinarySemFailNum += (n))
#define AT_DBG_LOCK_BINARY_SEM_FAIL_NUM(n) (g_atStatsInfo.cCpuResetStatsInfo.lockBinarySemFailNum += (n))
#define AT_DBG_SAVE_LAST_BIN_SEM_ERR_RSLT(rslt) (g_atStatsInfo.cCpuResetStatsInfo.lastBinarySemErrRslt = (rslt))
#define AT_DBG_SAVE_CCPU_RESET_BEFORE_NUM(n) (g_atStatsInfo.cCpuResetStatsInfo.resetBeforeNum += (n))
#define AT_DBG_SAVE_CCPU_RESET_AFTER_NUM(n) (g_atStatsInfo.cCpuResetStatsInfo.resetAfterNum += (n))
#define AT_DBG_SAVE_HIFI_RESET_NUM(n) (g_atStatsInfo.cCpuResetStatsInfo.hifiResetNum += (n))

/* AT��¼Msg�������� */
#define AT_MNTN_MSG_RECORD_MAX_NUM 100

#if (FEATURE_VCOM_EXT == FEATURE_ON)
/* vcom��չ�ڴ�ʱ����ͳ��pcui��ctrl�Լ�appvcom0��appvcom34�˿ڣ���37���˿� */
#define AT_MNTN_MAX_PORT_NUM 37
#define AT_MNTN_MAX_PORT_CLIENT_ID AT_CLIENT_ID_APP35
#else
/* vcom��չ�ڹر�ʱ����ͳ��pcui��ctrl�Լ�appvcom0��appvcom1�˿ڣ���4���˿� */
#define AT_MNTN_MAX_PORT_NUM 4
#define AT_MNTN_MAX_PORT_CLIENT_ID AT_CLIENT_ID_BUTT
#endif

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF) || (defined(LLT_OS_VER))
#define AT_DUMP_FIELD_ID       OM_AP_AT
#else
#define AT_DUMP_FIELD_ID       OM_CP_AT
#endif

#define AT_MNTN_CMD_NAME_LEN 12

#define AT_MNTN_SAVE_EXC_LOG_LENGTH 4096

#define AT_MNTN_DUMP_BEGIN_TAG 0xAA55AA55
#define AT_MNTN_DUMP_END_TAG 0xAA55AA55

#ifndef TAF_LOG
#define TAF_LOG(Mod, SubMod, Level, String)                                                                    \
    (VOS_VOID)mdrv_diag_log_report(MDRV_DIAG_GEN_LOG_MODULE(MODEM_ID_0, DIAG_MODE_UMTS, (Level)), (Mod), NULL, \
                                   __LINE__, (String " \r\n"))

#define TAF_LOG1(Mod, SubMod, Level, String, Para1)                                                            \
    (VOS_VOID)mdrv_diag_log_report(MDRV_DIAG_GEN_LOG_MODULE(MODEM_ID_0, DIAG_MODE_UMTS, (Level)), (Mod), NULL, \
                                   __LINE__, (String " %d \r\n"), (VOS_INT32)(Para1))

#define TAF_LOG2(Mod, SubMod, Level, String, Para1, Para2)                                                     \
    (VOS_VOID)mdrv_diag_log_report(MDRV_DIAG_GEN_LOG_MODULE(MODEM_ID_0, DIAG_MODE_UMTS, (Level)), (Mod), NULL, \
                                   __LINE__, (String " %d, %d \r\n"), (VOS_INT32)(Para1), (VOS_INT32)(Para2))

#define TAF_LOG3(Mod, SubMod, Level, String, Para1, Para2, Para3)                                                     \
    (VOS_VOID)mdrv_diag_log_report(MDRV_DIAG_GEN_LOG_MODULE(MODEM_ID_0, DIAG_MODE_UMTS, (Level)), (Mod), NULL,        \
                                   __LINE__, (String " %d, %d, %d \r\n"), (VOS_INT32)(Para1), (VOS_INT32)(Para2), \
                                   (VOS_INT32)(Para3))

#define TAF_LOG4(Mod, SubMod, Level, String, Para1, Para2, Para3, Para4)                                       \
    (VOS_VOID)mdrv_diag_log_report(MDRV_DIAG_GEN_LOG_MODULE(MODEM_ID_0, DIAG_MODE_UMTS, (Level)), (Mod), NULL, \
                                   __LINE__, (String " %d, %d, %d, %d \r\n"), (VOS_INT32)(Para1),          \
                                   (VOS_INT32)(Para2), (VOS_INT32)(Para3), (VOS_INT32)(Para4))
#endif

/*
 * �ṹ˵��: �����豸����
 */
enum AT_FC_DeviceType {
    AT_FC_DEVICE_TYPE_MODEM  = 0x00, /* MODEM */
    AT_FC_DEVICE_TYPE_NCM    = 0x01, /* NCM */
    AT_FC_DEVICE_TYPE_RMNET  = 0x02, /* RMNET */
    AT_FC_DEVICE_TYPE_HSUART = 0x03, /* HSUART */

    AT_FC_DEVICE_TYPE_BUTT
};
typedef VOS_UINT32 AT_FC_DeviceTypeUint32;

/*
 * �ṹ˵��: ���ص�����
 */
enum AT_FC_PointType {
    AT_FC_POINT_TYPE_MODEM_PS = 0x00, /* MODEM PS */
    AT_FC_POINT_TYPE_MODEM_VP = 0x01, /* MODEM VP */
    AT_FC_POINT_TYPE_NDIS     = 0x02, /* NCM */
    AT_FC_POINT_TYPE_RMNET    = 0x03, /* RMNET */
    AT_FC_POINT_TYPE_HSIC     = 0x04, /* HSIC */

    AT_FC_POINT_TYPE_BUTT
};
typedef VOS_UINT32 AT_FC_PointTypeUint32;


enum AT_OM_GreenchannelErr {
    AT_OM_GREENCHANNEL_PS_CID_NOT_DEFINE = 0,
    AT_OM_GREENCHANNEL_PS_CREATE_PPP_REQ_ERR,
    AT_OM_GREENCHANNEL_PS_CREATE_RAW_DATA_PPP_REQ_ERR,
    AT_OM_GREENCHANNEL_PS_IP_TYPE_DIAL_FAIL,
    AT_OM_GREENCHANNEL_PS_PPP_TYPE_DIAL_FAIL,
    AT_OM_GREENCHANNEL_PS_DEACTIVE_PDP_ERR_EVT,
    AT_OM_GREENCHANNEL_PS_ACTIVE_PDP_REJ,
    AT_OM_GREENCHANNEL_PS_MODIFY_PDP_REJ,
    AT_OM_GREENCHANNEL_PS_NET_ORIG_DEACTIVE_IND,

    AT_OM_GREENCHANNEL_ERR_BUTT
};
typedef VOS_UINT32 AT_OM_GreenchannelErrUint32;



typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    AT_InterMsgIdUint32    msgId;  /* ��Ϣ���� */ /* _H2ASN_Skip */
    VOS_UINT32             portId; /* �˿�ID */
    AT_FC_DeviceTypeUint32 device;
} AT_MntnFlowCtrl;


typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    AT_InterMsgIdUint32   msgId;  /* ��Ϣ���� */ /* _H2ASN_Skip */
    VOS_UINT32            portId; /* �˿�ID */
    AT_FC_PointTypeUint32 point;
} AT_MntnFcPoint;


typedef struct {
    AT_ClientIdUint16 atClientTabIndex;
    /* �ö˿��Ƿ����������ϱ���VOS_TRUEΪ����VOS_FALSEΪ������Ĭ������ */
    VOS_UINT8     reportFlg;
    VOS_UINT8     reserv;
    ModemIdUint16 modemId; /* �ö˿������ĸ�modem */
    VOS_UINT8     reserv1[2];
} AT_RptPort;


typedef struct {
    VOS_MSG_HEADER /* _H2ASN_Skip */
    AT_InterMsgIdUint32 msgId;
    AT_RptPort          atRptPort[AT_MAX_CLIENT_NUM];
} AT_MntnRptport;


typedef struct {
    VOS_UINT8 pcuiCtrlConcurrentFlg; /* PCUI��CTRL�ڲ�����־ */
    VOS_UINT8 pcuiPsCallFlg;         /* PCUI��ģ��NDISDUP���ű�־ */
    VOS_UINT8 pcuiUserId;            /* PCUI��ģ���ĸ��˿ڲ��� */
    VOS_UINT8 ctrlPsCallFlg;         /* CTRL��ģ��NDISDUP���ű�־ */
    VOS_UINT8 ctrlUserId;            /* CTRL��ģ���ĸ��˿ڲ��� */
    VOS_UINT8 pcui2PsCallFlg;        /* PCUI2��ģ��NDISDUP���ű�־ */
    VOS_UINT8 pcui2UserId;           /* PCUI2��ģ���ĸ��˿ڲ��� */
    VOS_UINT8 unCheckApPortFlg;
} AT_DebugInfo;


typedef struct {
    /* ��λ�ź�����Ϣ */
    VOS_SEM    binarySemId; /* �������ź���ID */
    /* ��ʼ����ʶ, VOS_TRUE: �ɹ�; VOS_FALSE: ʧ�� */
    VOS_UINT32 semInitFlg;
    VOS_UINT32 createBinarySemFailNum; /* �����������ź���ʧ�ܴ��� */
    VOS_UINT32 lockBinarySemFailNum;   /* ���������ź���ʧ�ܴ��� */
    VOS_UINT32 lastBinarySemErrRslt;   /* ���һ�����������ź���ʧ�ܽ�� */
    VOS_UINT32 resetBeforeNum;         /* C�˸�λǰ�Ĵ��� */
    VOS_UINT32 resetAfterNum;          /* C�˸�λ��Ĵ��� */
    VOS_UINT32 hifiResetNum;           /* HIFI��λ�Ĵ��� */
    VOS_UINT32 reserved;
} AT_ResetStatsInfo;

/*
 * �ṹ˵��: ��¼ATģ��Ŀ�ά�ɲ���Ϣ
 */
typedef struct {
    AT_ResetStatsInfo cCpuResetStatsInfo;

} AT_MntnStats;


typedef struct {
    VOS_UINT32 sendPid;
    VOS_UINT32 msgName;
    VOS_UINT32 sliceStart;
    VOS_UINT32 sliceEnd;
} AT_MntnMsgRecord;


typedef struct {
    AT_MntnMsgRecord atMntnMsgRecord[AT_MNTN_MSG_RECORD_MAX_NUM];
    VOS_UINT32       currentIndex;
    VOS_UINT32       reserve;
} AT_MntnMsgRecordInfo;


typedef struct {
    VOS_MSG_HEADER                   /* _H2ASN_Skip */
    AT_InterMsgIdUint32 msgId;
    VOS_UINT32          usedFlg;     /* ����ʵ������־ */
    VOS_UINT8           currPdpType; /* ��ǰ�������� */
    VOS_UINT8           portIndex;
    VOS_UINT8           userIndex;
    VOS_UINT8           usrType;
    VOS_UINT8           usrCid;
    VOS_UINT8           userPdpType;
    VOS_UINT8           pdpTypeValidFlag;
    VOS_UINT8           apnLen;
    VOS_UINT16          usernameLen;
    VOS_UINT16          passwordLen;
    VOS_UINT16          authType;
    VOS_UINT8           rsv1[2];
    VOS_UINT32          iPv4ValidFlag;

    VOS_UINT8           ipv4Cid;        /* IPv4 CID */
    VOS_UINT8           ipv4State;      /* IPv4 ״̬ */
    VOS_UINT8           ipv4DendRptFlg; /* IPV4 DEND�Ƿ��Ѿ��ϱ��� */
    VOS_UINT8           rsv2[1];        /* ����λ */

    VOS_UINT8           ipv6Cid;        /* IPv6 CID */
    VOS_UINT8           ipv6State;      /* IPv6 ״̬ */
    VOS_UINT8           ipv6DendRptFlg; /* IPV6 DEND�Ƿ��Ѿ��ϱ��� */
    VOS_UINT8           rsv3[1];        /* ����λ */

} AT_MntnPsCallEntity;


typedef struct {
    VOS_UINT8 cmdName[AT_MNTN_CMD_NAME_LEN];
    VOS_UINT32 clientStatus;
} AT_MntnPortInfoLog;


typedef struct {
    VOS_UINT32 beginTag;
    VOS_UINT32 reserv1;

    AT_MntnMsgRecordInfo msgInfo;

    /* ����AT_MNTN_MAX_PORT_NUM���FEATURE_VCOM_EXT�Ĵ򿪹رն��仯������stPortInfo���ڽṹ����� */
    AT_MntnPortInfoLog portInfo[AT_MNTN_MAX_PORT_NUM];

    VOS_UINT32 reserv2;
    VOS_UINT32 endTag;
} AT_MntnSaveExcLogInfo;

/*
 * H2ASN������Ϣ�ṹ����
 */
typedef struct {
    /* _H2ASN_MsgChoice_Export AT_INTER_MSG_ID_ENUM_UINT32 */
    AT_InterMsgIdUint32 msgId;
    VOS_UINT8           msg[AT_MSG_DEFAULT_VALUE_LEN];
    /*
     * _H2ASN_MsgChoice_When_Comment          AT_InterMsgIdUint32
     */
} AT_MNTN_MSG_DATA;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    AT_MNTN_MSG_DATA msgData;
} AtMntn_MSG;

extern AT_MntnStats g_atStatsInfo;
extern AT_DebugInfo g_atDebugInfo;

VOS_UINT8 AT_GetPcuiCtrlConcurrentFlag(VOS_VOID);
VOS_UINT8 AT_GetPcuiPsCallFlag(VOS_VOID);

VOS_UINT8 AT_GetPcuiUsertId(VOS_VOID);
VOS_UINT8 AT_GetCtrlPsCallFlag(VOS_VOID);
VOS_UINT8 AT_GetCtrlUserId(VOS_VOID);
VOS_UINT8 AT_GetPcui2PsCallFlag(VOS_VOID);
VOS_UINT8 AT_GetPcui2UserId(VOS_VOID);
VOS_VOID  AT_MntnTraceRPTPORT(VOS_VOID);
VOS_VOID  AT_SetUnCheckApPortFlg(VOS_UINT8 flag);
VOS_UINT8 AT_GetUnCheckApPortFlg(VOS_VOID);


VOS_VOID AT_MntnTraceEvent(struct MsgCB *msg);


/*
 * ��������: ע�����ص��ά�ɲ�
 */
VOS_VOID AT_MntnTraceRegFcPoint(VOS_UINT8 index, AT_FC_PointTypeUint32 fcPoint);

/*
 * ��������: ȥע�����ص��ά�ɲ�
 */
VOS_VOID AT_MntnTraceDeregFcPoint(VOS_UINT8 index, AT_FC_PointTypeUint32 fcPoint);

/*
 * ��������: AT����ظ���ά�ɲ�
 */
VOS_VOID AT_MntnTraceCmdResult(VOS_UINT8 index, VOS_UINT8 *data, VOS_UINT16 dataLen);

VOS_VOID AT_MntnTraceAtAtpCmdReq(VOS_UINT8 *data, VOS_UINT32 len);


VOS_VOID AT_SetPcuiCtrlConcurrentFlag(VOS_UINT8 flag);

VOS_VOID AT_ShowClientCtxInfo(VOS_VOID);

VOS_VOID AT_RecordAtMsgInfo(VOS_UINT32 sendPid, VOS_UINT32 msgName, VOS_UINT32 sliceStart, VOS_UINT32 sliceEnd);

VOS_UINT16 AT_MntnTransferClientIdToMntnIndex(VOS_UINT16 index);

VOS_VOID AT_MntnSaveCurCmdName(VOS_UINT16 index);

VOS_VOID AT_MntnSaveExcLog(VOS_VOID);

VOS_VOID AT_RegisterDumpCallBack(VOS_VOID);

VOS_VOID AT_InitMntnCtx(VOS_VOID);

VOS_VOID AT_SetPcuiPsCallFlag(VOS_UINT8 flag, VOS_UINT8 index);
VOS_VOID AT_SetCtrlPsCallFlag(VOS_UINT8 flag, VOS_UINT8 index);
VOS_VOID AT_SetPcui2PsCallFlag(VOS_UINT8 flag, VOS_UINT8 index);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
