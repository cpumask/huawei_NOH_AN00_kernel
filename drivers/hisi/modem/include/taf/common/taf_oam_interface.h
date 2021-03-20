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

#ifndef __TAF_OAM_INTERFACE_H__
#define __TAF_OAM_INTERFACE_H__

#include "v_msg.h"
#include "taf_app_mma.h"
#include "mn_msg_api.h"
#include "mn_client.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define SAR_ANTSTATE_IND 0x1111

#define TAF_STK_CURC_RPT_CFG_MAX_SIZE (8)

#define COMM_LOG_PORT_USB CPM_OM_PORT_TYPE_USB
#define COMM_LOG_PORT_VCOM CPM_OM_PORT_TYPE_VCOM


enum TAF_OAM_MsgType {
    /* SPY/DIAG/OM -> MMA */
    /* _H2ASN_MsgChoice MN_APP_PhoneModeSetReq */
    OAM_MMA_PHONE_MODE_SET_REQ = 0x00,
    /* _H2ASN_MsgChoice MN_APP_PhoneLoaddefaultReq */
    OAM_MMA_PHONE_LOADDEFAULT_REQ = 0x01,

    /* taf��stk������Ϣ��������NasStkInterface.h */

    /* TAF  -> OAM(VC) ����OAM�������˿ڣ�����ظ���Ϣ */
    /* _H2ASN_MsgChoice MN_APP_CsSetTransPortMsg */
    TAF_OAM_SET_TRANS_PORT_REQ = 0x0f,

    /* taf��stk������Ϣ��������NasStkInterface.h */

    /* PHONE -> SPY/DIAG/OM OAM_MMA_PHONE_MODE_SET_REQ�Ļظ���Ϣ */
    /* _H2ASN_MsgChoice MN_APP_PhoneEventInfo */
    TAF_OAM_PHONE_EVENT_IND = 0x11,

    /* PHONE -> SPY/DIAG/OM OAM_MMA_PHONE_LOADDEFAULT_REQ�Ļظ���Ϣ */
    /* _H2ASN_MsgChoice MN_APP_PhoneSetCnf */
    TAF_OAM_PHONE_SET_CNF = 0x12,

    /* _H2ASN_MsgChoice SSA_USSD_DCS_DECODE_HOOK_STRU */
    TAF_STK_USSD_DCS_DECODE_HOOK = 0xAAA0,

    MMA_EVT_OM_SDT_CONNECTED_IND = 0xAAAA,

    /* OAM->MMA, TRACE ������Ϣ */
    OAM_MMA_TRACE_CONFIG_REQ = 0xAAAB,

    MMA_OAM_TRACE_CONFIG_CNF,

    /* _H2ASN_MsgChoice TAF_OAM_UnsolicitedRptInfo */
    TAF_OAM_UNSOLICITED_RPT_INFO_IND,

    /* VC  -> OAM ����VCģ����Ҫ�Ҷϵ绰ʱ���쳣��� */
    /* _H2ASN_MsgChoice TAF_OAM_LogEndCallReq */
    TAF_OAM_LOG_END_CALL_REQ,

    /* TAF  -> OAM ��������״̬��Ϣ������ظ���Ϣ */
    /* _H2ASN_MsgChoice TAF_OAM_EmergencyCallStatus */
    TAF_OAM_EMERGENCY_CALL_STATUS_NOTIFY = 0xBBB0,

    TAF_OAM_MSG_TYPE_BUTT
};
typedef VOS_UINT32 TAF_OAM_MsgTypeUint32;


enum TAF_STK_RptCmdIndex {
    TAF_STK_RPT_CMD_MODE = 0,
    TAF_STK_RPT_CMD_RSSI,
    TAF_STK_RPT_CMD_SRVST,
    TAF_STK_RPT_CMD_SIMST,
    TAF_STK_RPT_CMD_TIME,
    TAF_STK_RPT_CMD_SMMEMFULL,
    TAF_STK_RPT_CMD_CTZV,
    TAF_STK_RPT_CMD_DSFLOWRPT,
    TAF_STK_RPT_CMD_ORIG,
    TAF_STK_RPT_CMD_CONF,
    TAF_STK_RPT_CMD_CONN,
    TAF_STK_RPT_CMD_CEND,
    TAF_STK_RPT_CMD_STIN,
    TAF_STK_RPT_CMD_CERSSI,
    TAF_STK_RPT_CMD_ANLEVEL,
    TAF_STK_RPT_CMD_LWCLASH,
    TAF_STK_RPT_CMD_XLEMA,
    TAF_STK_RPT_CMD_ACINFO,
    TAF_STK_RPT_CMD_PLMN,
    TAF_STK_RPT_CMD_CALLSTATE,
    TAF_STK_RPT_CMD_CREG,
    TAF_STK_RPT_CMD_CUSD,
    TAF_STK_RPT_CMD_CSSI,
    TAF_STK_RPT_CMD_CSSU,
    TAF_STK_RPT_CMD_LWURC,
    TAF_STK_RPT_CMD_CUUS1I,
    TAF_STK_RPT_CMD_CUUS1U,
    TAF_STK_RPT_CMD_CGREG,
    TAF_STK_RPT_CMD_CEREG,
    TAF_STK_RPT_CMD_BUTT
};
typedef VOS_UINT8 TAF_STK_RptCmdIndexUint8;


enum TAF_APS_UserConnStatus {
    TAF_APS_USER_CONN_EXIST,
    TAF_APS_USER_CONN_NOT_EXIST,
    TAF_APS_USER_CONN_BUTT
};
typedef VOS_UINT8 TAF_APS_UserConnStatusUint8;


enum TAF_OAM_SdtConnectStatus {
    TAF_OAM_SDT_CONNECT_STATUS_CONNECTED,    /* SDT�͵��������� */
    TAF_OAM_SDT_CONNECT_STATUS_DISCONNECTED, /* SDT�͵���Ͽ����� */
    TAF_OAM_SDT_CONNECT_STATUS_BUTT
};
typedef VOS_UINT8 TAF_OAM_SdtConnectStatusUint8;


enum TAF_OAM_PcRecurCfg {
    TAF_OAM_PC_RECUR_CFG_ENABLE,  /* ʹ�ܷ���PC�ط���Ϣ */
    TAF_OAM_PC_RECUR_CFG_DISABLE, /* ȥʹ�ܷ���PC�ط���Ϣ */
    TAF_OAM_PC_RECUR_CFG_BUTT
};
typedef VOS_UINT8 TAF_OAM_PcRecurCfgUint8;


enum TAF_OAM_EmergencyCallStatus {
    TAF_OAM_EMERGENCY_CALL_START, /* ����������� */
    TAF_OAM_EMERGENCY_CALL_END,   /* �������н��� */
    TAF_OAM_EMERGENCY_CALL_STATUS_BUTT
};
typedef VOS_UINT8 TAF_OAM_EmergencyCallStatusUint8;


enum OM_HSIC_PortStatus {
    OM_HSIC_PORT_STATUS_OFF = 0, /* HSIC��GU��OM��δ������ */
    OM_HSIC_PORT_STATUS_ON,      /* HSIC��GU��OM���ѹ����� */
    OM_HSIC_PORT_STATUS_BUTT
};
typedef VOS_UINT32 OM_HSIC_PortStatusUint32;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16 primId;  /* ��Ϣ�� */
    VOS_UINT16 reserve; /* ���� */
    VOS_UINT32 status;  /* �������� */
    VOS_UINT32 port;    /* �˿ں� */
} MN_APP_CsSetTransPortMsg;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: STK/SPY����MMA�ػ�����
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32    msgId;
    VOS_UINT16    clientId;
    VOS_UINT8     opID;
    VOS_UINT8     reserved1[1];
    TAF_PH_OpMode phOpMode;
} MN_APP_PhoneModeSetReq;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: STK/SPY����MMA�ָ�������������
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT16 clientId;
    VOS_UINT8  opID;
    VOS_UINT8  reserved1[1];
} MN_APP_PhoneLoaddefaultReq;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: PHONE ����STK ҵ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgId;
    TAF_PHONE_EventInfo phoneEvent;
} MN_APP_PhoneEventInfo;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: PHONE ����STK ҵ��
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgId;
    VOS_UINT16          clientId;
    VOS_UINT8           opID;
    TAF_PARA_TYPE       paraType;
    TAF_PARA_SET_RESULT result;
    VOS_UINT8           reserved[3];
} MN_APP_PhoneSetCnf;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: OAM����TAF��ANTEN�¼��ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 antenStatus; /* ����״̬�ϱ���Taf */
} MN_APP_SarAntenstatusMsg;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ����DSP��Reduction�¼��ṹ
 */
typedef struct {
    VOS_UINT32 sarType;      /* �ϱ��������� */
    VOS_UINT32 antenStatus;  /* ����״̬ */
    VOS_UINT32 sarReduction; /* ��sar�ȼ� */
} MN_APP_SarInfo;


typedef struct {
    TAF_OAM_PcRecurCfgUint8 pcRecurCfgFlg;
    VOS_UINT8               rsv3[3];
} TAF_OAM_TraceCfg;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId; /* ԭ��ID */

    TAF_OAM_TraceCfg traceCfg; /* TRACE������Ϣ */
} TAF_OAM_TraceCfgInd;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 rptCmdIndex;
    VOS_UINT8  curcRptCfg[TAF_STK_CURC_RPT_CFG_MAX_SIZE];
    VOS_UINT8  unsolicitedRptCfg[TAF_STK_CURC_RPT_CFG_MAX_SIZE];
} TAF_OAM_UnsolicitedRptInfo;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32                       msgId;
    TAF_OAM_EmergencyCallStatusUint8 emergencyCallStatus;
    VOS_UINT8                        reserved1[3];
} TAF_OAM_EmergencyCallStatusPara;


typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;
    VOS_UINT32 cause;
} TAF_OAM_LogEndCallReq;

typedef VOS_INT (*pSockRecv)(VOS_UINT8 uPortNo, VOS_UINT8 *pData, VOS_UINT16 uslength);

extern TAF_APS_UserConnStatusUint8 TAF_APS_GetUserConnStatus(VOS_VOID);

extern VOS_VOID Spy_SarSendToDsp(ModemIdUint16 modemID, const MN_APP_SarInfo *spyToDsp);

extern VOS_UINT16 Spy_SarGetReduction(VOS_VOID);

/* Deleted SSA_GetDcsMsgCoding */

extern VOS_VOID SSA_UssdDcsHook(VOS_UINT32 receiverPid, VOS_UINT8 dcs, MN_MSG_MsgCodingUint8 msgCoding);

extern VOS_UINT32 PPM_QueryLogPort(VOS_UINT32 *logPort);

extern OM_HSIC_PortStatusUint32 PPM_GetHsicPortStatus(VOS_VOID);

extern VOS_UINT8  TAF_MMA_GetCurrentPhoneModeByModemId(ModemIdUint16 modemId);
extern VOS_UINT32 MN_MSG_Decode_UsimMsg(VOS_UINT8 *data, VOS_UINT32 len, MN_MSG_SubmitLong *longSubmit);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of TafOamInterface.h */
