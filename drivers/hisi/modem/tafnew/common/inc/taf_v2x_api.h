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
#define AT_PSCCH_TRANSPORT_BLOCKS 1                /* PSCCH����� */
#define AT_STCH_PDCP_SDU_PACKETS 1                 /* PDCP���ݰ� */
#define AT_PSSCH_TRANSPORT_BLOCKS 1                /* PSSCH����� */
#define AT_CCUTLE_MONITOR_LIST_MAX_NUM 16          /* �ɼ���LAYER2-ID������� */
#define AT_CCUTLE_MONITOR_LIST_CONTEXT_TO_ID 6     /* ÿ��LAYER2-IDʹ��6���ַ����� */
#define AT_CCUTLE_MONITOR_LIST_PACKETS_TO_ID 8     /* ÿ��LAYER2-ID�����ݰ�����ʹ��8���ַ����� */
#define AT_CCUTLE_MONITOR_LIST_CONTEXT_MAX_NUM 96  /* ��������LAYER2-ID���ռ�õ��ַ� */
#define AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM 128 /* ��������LAYER2-ID�������ݰ����ռ�õ��ַ� */

/* AT��VRRC֮�����Ϣ�ӿ� */
enum AT_VRRC_MsgType {
    VRRC_MSG_RSU_SYNCST_QRY_REQ = 0, /* ��ѯRSUͬ��״̬������Ϣ */
    VRRC_MSG_RSU_SYNCST_QRY_CNF,     /* ��ѯRSUͬ��״̬ȷ����Ϣ */
    VRRC_MSG_GNSS_TEST_START_REQ,    /* ֪ͨPHY����GNSS����������Ϣ */
    VRRC_MSG_GNSS_TEST_START_CNF,    /* ֪ͨPHY����GNSS����ȷ����Ϣ */
    VRRC_MSG_GNSS_INFO_QRY_REQ,      /* ��ѯGNSS�����Ϣ������Ϣ */
    VRRC_MSG_GNSS_INFO_QRY_CNF,      /* ��ѯGNSS�����Ϣȷ����Ϣ */
    VRRC_MSG_RSU_SLINFO_QRY_REQ,
    VRRC_MSG_RSU_SLINFO_QRY_CNF,
    VRRC_MSG_RSU_SLINFO_SET_REQ,
    VRRC_MSG_RSU_SLINFO_SET_CNF,

    VRRC_MSG_RPPCFG_SET_REQ,  /* ����V2X��Դ�ز���������Ϣ */
    VRRC_MSG_RPPCFG_SET_CNF,  /* ����V2X��Դ�ز���ȷ����Ϣ */
    VRRC_MSG_RPPCFG_QRY_REQ,  /* ��ѯV2X��Դ�ز���������Ϣ */
    VRRC_MSG_RPPCFG_QRY_CNF,  /* ��ѯV2X��Դ�ز���ȷ����Ϣ */
    VRRC_MSG_RSU_SLCFGST_RPT, /* V2X��Դ�ز�������״̬�ϱ� */

    VRRC_MSG_RSU_PHYR_SET_REQ,
    VRRC_MSG_RSU_PHYR_SET_CNF,
    VRRC_MSG_RSU_PHYR_QRY_REQ,
    VRRC_MSG_RSU_PHYR_QRY_CNF,

    VRRC_MSG_RSU_GNSSSYNCST_QRY_REQ, /* ��ѯRSU GNSSͬ��״̬������Ϣ */
    VRRC_MSG_RSU_GNSSSYNCST_QRY_CNF, /* ��ѯRSU GNSSͬ��״̬ȷ����Ϣ */

    VRRC_MSG_RSU_VPHYSTAT_QRY_REQ, /* ��ѯ�����շ�ͳ�ƽ��������Ϣ */
    VRRC_MSG_RSU_VPHYSTAT_QRY_CNF, /* ��ѯ�����շ�ͳ�ƽ��ȷ����Ϣ */

    VRRC_MSG_RSU_VPHYSTAT_CLR_SET_REQ, /* ���û����շ�ͳ�ƽ������������Ϣ */
    VRRC_MSG_RSU_VPHYSTAT_CLR_SET_CNF, /* ���û����շ�ͳ�ƽ������ȷ����Ϣ */

    VRRC_MSG_RSU_VSNRRSRP_QRY_REQ, /* ��ѯ����SNR��RSRP��Ϣ������Ϣ */
    VRRC_MSG_RSU_VSNRRSRP_QRY_CNF, /* ��ѯ����SNR��RSRP��Ϣȷ����Ϣ */

    VRRC_MSG_GNSS_INFO_GET_REQ, /* ��ѯ��ǰGNSS Debug��Ϣ������Ϣ */
    VRRC_MSG_GNSS_INFO_GET_CNF, /* ��ѯ��ǰGNSS Debug��Ϣȷ����Ϣ */

    VRRC_MSG_RSU_RSSI_QRY_REQ, /* ��ѯ����RSSI��Ϣ������Ϣ */
    VRRC_MSG_RSU_RSSI_QRY_CNF, /* ��ѯ����RSSI��Ϣȷ����Ϣ */

    VRRC_MSG_PC5SYNC_QRY_REQ, /* ��ѯPC5ͬ����ز���������Ϣ��VRRC���账�� */
    VRRC_MSG_PC5SYNC_QRY_CNF, /* ��ѯPC5ͬ����ز���ȷ����Ϣ��VRRC���账�� */
    VRRC_MSG_PC5SYNC_SET_REQ, /* ����PC5ͬ����ز���������Ϣ ��VRRC���账�� */
    VRRC_MSG_PC5SYNC_SET_CNF, /* ����PC5ͬ����ز���ȷ����Ϣ��VRRC���账�� */

    AT_VRRC_SYNC_SOURCE_QRY_REQ = 35, /* ͬ��Դ��ѯ���� */
    VRRC_AT_SYNC_SOURCE_QRY_CNF,      /* ͬ��Դ��ѯȷ�� */

    AT_VRRC_SYNC_SOURCE_RPT_SET_REQ, /* ͬ��Դ�仯�ϱ��������� */
    VRRC_AT_SYNC_SOURCE_RPT_SET_CNF, /* ͬ��Դ�仯�ϱ�����ȷ�� */
    AT_VRRC_SYNC_SOURCE_RPT_QRY_REQ, /* ͬ��Դ�仯�ϱ���ѯ���� */
    VRRC_AT_SYNC_SOURCE_RPT_QRY_CNF, /* ͬ��Դ�仯�ϱ���ѯȷ�� */
    VRRC_AT_SYNC_SOURCE_RPT_IND,     /* ͬ��Դ�仯�����ϱ� */

    AT_VRRC_SYNC_MODE_SET_REQ, /* ͬ��ģʽ�������� */
    VRRC_AT_SYNC_MODE_SET_CNF, /* ͬ��ģʽ����ȷ�� */

    AT_VRRC_SYNC_MODE_QRY_REQ, /* ͬ��ģʽ��ѯ���� */
    VRRC_AT_SYNC_MODE_QRY_CNF, /* ͬ��ģʽ��ѯȷ�� */

    AT_VRRC_SET_TX_POWER_REQ, /* ��������㷢�书��������Ϣ */
    VRRC_AT_SET_TX_POWER_CNF, /* ��������㷢�书��ȷ����Ϣ */
    VRRC_MSG_BUTT
};
typedef VOS_UINT16 AT_VRRC_MsgTypeUint16;

/* AT��VMAC֮�����Ϣ�ӿ� */
enum AT_VMAC_MsgType {
    VMAC_MSG_RSU_SLPTRST_RPT = 0,
    VMAC_MSG_RSU_VRSSI_QRY_REQ,
    VMAC_MSG_RSU_VRSSI_QRY_CNF,
    VMAC_MSG_RSU_MCG_CFG_SET_REQ,
    VMAC_MSG_RSU_MCG_CFG_SET_CNF,
    VMAC_MSG_RSU_DSTIDTM_SET_REQ,
    VMAC_MSG_RSU_DSTIDTM_SET_CNF,
    VMAC_MSG_PC5_SEND_DATA_REQ, /* ����PC5����������Ϣ */
    VMAC_MSG_PC5_SEND_DATA_CNF, /* ����PC5����ȷ����Ϣ */

    VMAC_AT_RSSI_RPT_IND = 9, /* RSSI�仯�����ϱ� */
    AT_VMAC_CBR_QRY_REQ,      /* CBR��ѯ���� */
    VMAC_AT_CBR_QRY_CNF,      /* CBR��ѯȷ�� */
    VMAC_AT_CBR_RPT_IND,      /* CBR�����ϱ� */
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

/* AT��VTC֮�����Ϣ�ӿ� */
enum AT_VTC_MsgType {
    AT_VTC_TEST_MODE_ACTIVE_STATE_SET_REQ = 0,   /* ����ģʽ����״̬�������� */
    VTC_AT_TEST_MODE_ACTIVE_STATE_SET_CNF,       /* ����ģʽ����״̬������Ӧ */
    AT_VTC_TEST_MODE_ACTIVE_STATE_QRY_REQ,       /* ����ģʽ����״̬�������� */
    VTC_AT_TEST_MODE_ACTIVE_STATE_QRY_CNF,       /* ����ģʽ����״̬������Ӧ */
    AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_SET_REQ, /* ����ģʽE�պ�״̬�������� */
    VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_SET_CNF, /* ����ģʽE�պ�״̬������Ӧ */
    AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_REQ, /* ����ģʽE�պ�״̬��ѯ���� */
    VTC_AT_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_CNF, /* ����ģʽE�պ�״̬��ѯ��Ӧ */
    AT_VTC_SIDELINK_PACKET_COUNTER_QRY_REQ,      /* ���ݰ���ѯ���� */
    VTC_AT_SIDELINK_PACKET_COUNTER_QRY_CNF,      /* ���ݰ���ѯ��Ӧ */
    AT_VTC_RESET_UTC_TIME_SET_REQ,               /* ����UTCʱ���������� */
    VTC_AT_RESET_UTC_TIME_SET_CNF,               /* ����UTCʱ��������Ӧ */
    AT_VTC_CBR_QRY_REQ,                          /* CBR�ŵ�æ�ʲ�ѯ���� */
    VTC_AT_CBR_QRY_CNF,                          /* CBR�ŵ�æ�ʲ�ѯ��Ӧ */
    AT_VTC_SENDING_DATA_ACTION_SET_REQ,          /* ����������Ϊ�������� */
    VTC_AT_SENDING_DATA_ACTION_SET_CNF,          /* ����������Ϊ������Ӧ */
    AT_VTC_SENDING_DATA_ACTION_QRY_REQ,          /* ����������Ϊ�������� */
    VTC_AT_SENDING_DATA_ACTION_QRY_CNF,          /* ����������Ϊ������Ӧ */

    AT_VTC_PC5_SEND_DATA_REQ,
    VTC_AT_PC5_SEND_DATA_CNF,

    AT_VTC_LAYER_TWO_ID_SET_REQ,       /* ����ģʽ����L2ID�������� */
    VTC_AT_LAYER_TWO_ID_SET_CNF,       /* ����ģʽ����L2ID������Ӧ */
    AT_VTC_LAYER_TWO_ID_QRY_REQ,       /* ����ģʽ����L2ID��ѯ���� */
    VTC_AT_LAYER_TWO_ID_QRY_CNF,       /* ����ģʽ����L2ID��ѯ��Ӧ */
};
typedef VOS_UINT32 AT_VTC_MsgTypeUint32;

/* ����ģʽ����״̬ */
enum AT_VTC_TestModeActiveState {
    AT_VTC_TEST_MODE_STATE_DEACTIVATED = 0, /* ȥ���� */
    AT_VTC_TEST_MODE_STATE_ACTIVATED,       /* ���� */

    AT_VTC_TEST_MODE_STATE_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeActiveStateUint8;

/* ����ģʽ���� */
enum AT_VTC_TestModeType {
    AT_VTC_TEST_MODE_TYPE_E = 1, /* ����ģʽE */

    AT_VTC_TEST_MODE_TYPE_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeTypeUint8;

/* ����ģʽ�±ջ�״̬ */
enum AT_VTC_TestModeCloseStatus {
    AT_VTC_TEST_MODE_STATUS_CLOSE = 0, /* �������ģʽ */
    AT_VTC_TEST_MODE_STATUS_OPEN,      /* �˳�����ģʽ */

    AT_VTC_TEST_MODE_STATUS_BUTT
};
typedef VOS_UINT8 AT_VTC_TestModeCloseStatusUint8;

/* ����ģʽ���������� */
enum AT_VTC_CommunicationDirection {
    AT_VTC_COMMUNICATION_DIRECTION_RECEIVE = 0, /* �������� */
    AT_VTC_COMMUNICATION_DIRECTION_TRANSMIT,    /* �������� */

    AT_VTC_COMMUNICATION_DIRECTION_BUTT
};
typedef VOS_UINT8 AT_VTC_CommunicationDirectionUint8;

/* ���Destination Layer-ID�ĸ�ʽ */
enum AT_VTC_MonitorListFormat {
    AT_VTC_MONITOR_LIST_FORMAT_BINARY = 1, /* �����Ƹ�ʽ */

    AT_VTC_MONITOR_LIST_FORMAT_BUTT
};
typedef VOS_UINT8 AT_VTC_MonitorListFormatUint8;

/* ����������Ϊ */
enum AT_VTC_SendingDataAction {
    AT_VTC_SENDING_DATA_ACTION_STOP = 0, /* ֹͣ�������� */
    AT_VTC_SENDING_DATA_ACTION_START,    /* ��ʼ�������� */

    AT_VTC_SENDING_DATA_ACTION_BUTT
};
typedef VOS_UINT8 AT_VTC_SendingDataActionUint8;

/* ���ݰ���ʽ */
enum AT_VTC_PacketCounterFormat {
    AT_VTC_PACKET_COUNTER_FORMAT_BINARY = 1, /* �����Ƹ�ʽ */

    AT_VTC_PACKET_COUNTER_FORMAT_BUTT
};
typedef VOS_UINT8 AT_VTC_PacketCounterFormatUint8;

/* ͬ��Դ���� */
enum VRRC_AT_SyncSourceType {
    VRRC_AT_SYNC_SOURCE_TYPE_GNSS = 0,
    VRRC_AT_SYNC_SOURCE_TYPE_ENODEB,
    VRRC_AT_SYNC_SOURCE_TYPE_REF_UE,
    VRRC_AT_SYNC_SOURCE_TYPE_SELF,

    VRRC_AT_SYNC_SOURCE_TYPE_BUTT
};
typedef VOS_UINT8 VRRC_AT_SyncSourceTypeUint8;

/* ͬ��Դ״̬ */
enum VRRC_AT_SyncSourceStatus {
    VRRC_AT_SYNC_SOURCE_STATUS_IN_SYNC = 0, /* ͬ�� */
    VRRC_AT_SYNC_SOURCE_STATUS_OUT_OF_SYNC, /* ʧ�� */

    VRRC_AT_SYNC_SOURCE_STATUS_BUTT
};
typedef VOS_UINT8 VRRC_AT_SyncSourceStatusUint8;

/* ͬ��ģʽ */
enum AT_VRRC_SyncMode {
    AT_VRRC_SYNC_MODE_GNSS_ONLY = 0,    /* ��GNSS */
    AT_VRRC_SYNC_MODE_GNSS_REF_UE_SELF, /* GNSS�Ӳο�UE����ͬ�� */

    AT_VRRC_SYNC_MODE_BUTT
};
typedef VOS_UINT8 AT_VRRC_SyncModeUint8;

/* ͬ��Դ�ϱ����� */
enum AT_VRRC_SyncSourceRptSwitch {
    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_OFF = 0, /* �ر� */
    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_ON,      /* �� */

    AT_VRRC_SYNC_SOURCE_RPT_SWITCH_BUTT
};
typedef VOS_UINT8 AT_VRRC_SyncSourceRptSwitchUint8;

/* VTC��AT����� */
enum VTC_AT_ResultCode {
    VTC_AT_RESULT_OK                  = 0, /* �ɹ� */
    VTC_AT_RESULT_ERR                 = 1, /* ʧ�� */
    VTC_AT_RESULT_OPERATION_NOT_ALLOW = 2, /* ���������� */
    VTC_AT_RESULT_TIMEOUT             = 3, /* ��ʱ����ʱ */
};
typedef VOS_UINT32 VTC_AT_ResultCodeUint32;

/* VRRC��AT����� */
enum VRRC_AT_ResultCode {
    VRRC_AT_RESULT_OK                  = 0, /* �ɹ� */
    VRRC_AT_RESULT_ERR                 = 1, /* ʧ�� */
    VRRC_AT_RESULT_NO_SYNC_SOURCE      = 2, /* û���ҵ�ͬ��Դ */
};
typedef VOS_UINT32 VRRC_AT_ResultCodeUint32;

/* �����ȥ�������ģʽ */
typedef struct {
    AT_VTC_TestModeActiveStateUint8 state;
    AT_VTC_TestModeTypeUint8        type;
    VOS_UINT8                       reserved[2];
} AT_VTC_TestModeActiveStatePara;

/* VTC�ظ�AT����ģʽ����״̬��ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32        result;
    AT_VTC_TestModeActiveStatePara para;
} VTC_AT_TestModeActiveStateQryCnf;

/* ����ģʽE�պ�״̬ */
typedef struct {
    VOS_UINT32                         length;
    AT_VTC_TestModeCloseStatusUint8    status;
    AT_VTC_CommunicationDirectionUint8 direction;
    AT_VTC_MonitorListFormatUint8      format;
    VOS_CHAR                           monitorList[AT_CCUTLE_MONITOR_LIST_CONTEXT_MAX_NUM + 1];
} AT_VTC_TestModeECloseStatePara;

/* VTC�ظ�AT����ģʽ�պ�״̬��ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32        result;
    AT_VTC_TestModeECloseStatePara para;
} VTC_AT_TestModeECloseStateQryCnf;

/* sidelink���ݰ����� */
typedef struct {
    VOS_UINT32 lenOfPscchTransportBlocks; /* ����PSCCH������������ַ����� */
    VOS_UINT32 lenOfStchPdcpSduPackets;   /* ����STCH PDCP SDU���ݰ��������ַ����� */
    VOS_UINT32 lenOfPsschTransportBlocks; /* ����PSSCH������������ַ����� */
    VOS_CHAR   numOfPscchTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1]; /* PSCCH��������� */
    VOS_CHAR   numOfStchPdcpSduPackets[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1];   /* STCH PDCP SDU������ */
    VOS_CHAR   numOfPsschTransportBlocks[AT_CCUTLE_MONITOR_LIST_PACKETS_MAX_NUM + 1]; /* PSSCH��������� */
    VOS_UINT8  reserved1;
} VTC_AT_SidelinkPacketCounterPara;

/* VTC�ظ�AT���ݰ�������ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32          result;
    VTC_AT_SidelinkPacketCounterPara para;
} VTC_AT_SidelinkPacketCounterQryCnf;

/* VTC���������ŵ�æ�ʲ��� */
typedef struct {
    VOS_UINT32 isPscchCbrValid; /* PSCCH�ŵ�æ�ʲ����Ƿ���Ч */
    VOS_UINT32 psschCbr;        /* PSSCH�ŵ�æ�� */
    VOS_UINT32 pscchCbr;        /* PSCCH�ŵ�æ�� */
} VTC_AT_CbrPara;

/* VTC�ظ�AT�ŵ�æ�ʲ�ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
    VTC_AT_CbrPara          para;
} VTC_AT_CbrQryCnf;

/* PC5�����ݷ������� */
typedef struct {
    AT_VTC_SendingDataActionUint8 action; /* ֹͣ��ʼ���� */
    VOS_UINT8                     reserved[3];
    VOS_UINT32                    dataSize;    /* ���ݰ���С */
    VOS_UINT32                    periodicity; /* �������� */
} AT_VTC_SendingDataActionPara;

/* VTC�ظ�AT PC5�����ݷ������ò�ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32      result;
    AT_VTC_SendingDataActionPara para;
} VTC_AT_SendingDataActionQryCnf;

/* ����ģʽ���޸�L2ID */
typedef struct {
    VOS_UINT32 srcId;
    VOS_UINT32 dstId;
} AT_VTC_TestModeModifyL2IdPara;

/* VTC�ظ�AT����ģʽL2ID��ѯ��� */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
    AT_VTC_TestModeModifyL2IdPara para;
} VTC_AT_TestModeModifyL2IdQryCnf;

/* ͬ��״̬���� */
typedef struct {
    VOS_UINT32                    earfcn;    /* ��ǰͬ��Դ����Ƶ�� */
    VRRC_AT_SyncSourceTypeUint8   type;      /* ͬ��Դ���� */
    VRRC_AT_SyncSourceStatusUint8 status;    /* ͬ��Դ״̬ */
    VOS_UINT16                    slssId;    /* ֵ��Χ0-335 */
    VOS_UINT16                    subSlssId; /* ֵ��Χ0-65535 */
    VOS_UINT8                     reserved[2];
} VRRC_AT_SyncSourceStatePara;

/* VRRC�ظ�ATͬ��״̬��ѯ��� */
typedef struct {
    VOS_UINT32                  result;
    VRRC_AT_SyncSourceStatePara para;
} VRRC_AT_SyncSourceStateQryCnf;

/* ͬ��״̬�ϱ����� */
typedef struct {
    AT_VRRC_SyncSourceRptSwitchUint8 status;
    VOS_UINT8                        reserved[3];
} AT_VRRC_SyncSourceRptSwitchPara;

/* VRRC�ظ�ATͬ��״̬�ϱ����ز�ѯ��� */
typedef struct {
    VOS_UINT32                      result;
    AT_VRRC_SyncSourceRptSwitchPara para;
} VRRC_AT_SyncSourceRptSwitchQryCnf;

/* ͬ��ģʽ */
typedef struct {
    AT_VRRC_SyncModeUint8 mode;
    VOS_UINT8             reserved[3];
} AT_VRRC_SyncModePara;

/* VRRC�ظ�ATͬ��ģʽ��ѯ��� */
typedef struct {
    VOS_UINT32           result;
    AT_VRRC_SyncModePara para;
} VRRC_AT_SyncModeQryCnf;

/* PC5��RSSI���� */
typedef struct {
    VOS_INT16 rssi;
    VOS_INT16 reserved;
} VMAC_AT_Pc5RssiPara;

/* VMAC��ȡ����CBR���� */
typedef struct {
    VOS_UINT32 psschCbr;
} VMAC_AT_CbrPara;

/* VMAC�ظ�AT CBR��ѯ��� */
typedef struct {
    VOS_UINT32      result;
    VMAC_AT_CbrPara para;
} VMAC_AT_CbrQryCnf;

/* VTC�ظ�AT���ý�� */
typedef struct {
    VTC_AT_ResultCodeUint32 result;
} VTC_AT_MsgSetCnf;

/* VRRC�ظ�AT���ý�� */
typedef struct {
    VOS_UINT32 result;
} VRRC_AT_MsgSetCnf;

/* AT������Ϣ��VTC */
typedef struct {
    VOS_MSG_HEADER
    AT_VTC_MsgTypeUint32 msgName;
    MN_CLIENT_ID_T       clientId;
    MN_OPERATION_ID_T    opId;
    VOS_UINT8            reserved;
    VOS_UINT8            content[0];  //lint !e43
} AT_VTC_MsgReq;

/* VTC�ظ���Ϣ��AT */
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
    AT_VRRC_MsgTypeUint16 msgName;     /* ��Ϣ�� */
    VOS_UINT8             reserve1[2]; /* ���� */
    MN_CLIENT_ID_T        clientId;    /* Client ID */
    MN_OPERATION_ID_T     opId;        /* Operation ID */
    VOS_UINT8             reserve2;    /* ���� */
    VOS_UINT8             content[4];  /* ��Ϣ���� */
} AT_VRRC_ReqMsg;


typedef struct {
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 syncSrc;
    VOS_UINT8 syncStat;
    VOS_UINT8 reserved;
} VRRC_AT_SyncstQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 syncStat;
    VOS_UINT8 reserve[2]; /* ���� */
} VRRC_AT_GnsssyncstQryCnf;

/*
 * �ṹ��: TAF_GNSS_SysType
 * �ṹ˵��: ����ϵͳ���͵�ö��
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
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 perPpsStat;
    VOS_UINT8 syncStat;
    VOS_UINT8 satNum;
    VOS_UINT8 sysSatNum[GNSS_SYS_BDS + 1];
    VOS_UINT8 rev[2];
} VRRC_AT_GnssQryCnf;

/*
 * �ṹ��: TAF_GNSS_SysInfo
 * �ṹ˵��: GNSSϵͳ���ͼ���Ӧ�����
 */
typedef struct {
    TAF_GNSS_SysTypeUint16 gnssSystem; /* ����ϵͳ���� */
    VOS_INT16              cno;        /* C/N0������ȣ� */
} TAF_GNSS_SysInfo;

#define MAX_SATNUM 30  // ������Ǹ���
#define MAXLEN 12


typedef struct {
    VOS_UINT8        qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8        satNum;  /* ������Ŀ */
    VOS_UINT8        rev[2];
    VOS_UINT8        longitude[MAXLEN];   /* ���� */
    VOS_UINT8        latitude[MAXLEN];    /* γ�� */
    TAF_GNSS_SysInfo sysInfo[MAX_SATNUM]; /* GNSSϵͳ���ͼ���Ӧ����� */
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
    AT_VMAC_MsgTypeUint16 msgName;     /* ��Ϣ�� */
    VOS_UINT8             reserve1[2]; /* ���� */
    MN_CLIENT_ID_T        clientId;    /* Client ID */
    MN_OPERATION_ID_T     opId;        /* Operation ID */
    VOS_UINT8             reserve2;    /* ���� */
    VOS_UINT8             content[4];  /* ��Ϣ���� */
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
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_INT8  maxTxPwr;
    VOS_UINT8 minMcs;
    VOS_UINT8 maxMcs;
    VOS_UINT8 minSubchn;
    VOS_UINT8 maxSubchn;
    VOS_UINT8 rsv[2];
} VRRC_AT_PhyrQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT32 firstSaDecSucSum; /* ����SA������ȷ���� */
    VOS_UINT32 secSaDecSucSum;   /* �ش�SA������ȷ���� */
    VOS_UINT32 twiSaDecSucSum;   /* ����SA������ȷ���� */
    VOS_UINT32 uplinkPackSum;    /* ���Ͷ��յ�L2��ͳ�� */
    VOS_UINT32 downlinkPackSum;  /* ���ն��հ�ͳ�� */
    VOS_UINT32 phyUplinkPackSum; /* ����ҵ�񷢰��� */
} VRRC_AT_VphystatQryCnf;


typedef struct {
    VOS_UINT8 vphyStatClrFlg;
    VOS_UINT8 rsv[3];
} AT_VRRC_VphystatClrSet;


typedef struct {
    TAF_ERROR_CodeUint32 result;
} VRRC_AT_VphystatClrSetCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT8 msgNum;             /* SA����ɹ����� */
    VOS_INT8  saMainSNR[10];      /* SA����SNR */
    VOS_INT8  saDiversitySNR[10]; /* SA�ּ�SNR */
    VOS_INT8  daMainSNR[10];      /* DA����SNR */
    VOS_INT8  daDiversitySNR[10]; /* DA�ּ�SNR */
    VOS_INT8  daRsrp[10];         /* DA RSRP */
    VOS_UINT8 reserved;

} VRRC_AT_VsnrrsrpQryCnf;


typedef struct {
    VOS_UINT8 qryRslt; /* ��ѯsuccess or failure */
    VOS_UINT8 rsv[3];

    VOS_UINT8 msgNum;             /* RSSI�����Ӵ��ϱ�����ֵ��ʾRSSI��Ч���� */
    VOS_INT8  daMainRSSIMax;      /* �������Ӵ�RSSI���ֵ */
    VOS_INT8  daDiversityRSSIMax; /* �ּ����Ӵ�RSSI���ֵ */
    VOS_UINT8 reserved;

} VRRC_AT_RssiQryCnf;


typedef struct {
    VOS_UINT16 qryRslt; /* ��ѯsuccess or failure */
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

/* ����������� */
typedef struct {
    VOS_INT16 setTxPower; /* ��λΪ0.125dBm(setTxPower=80��ʾ��ǰ���ù���Ϊ10dBm) */
    VOS_UINT8 rsv[2];
} AT_VRRC_Set_Tx_PowerPara;

/* PHY�ظ�����������ʽ�� */
typedef struct {
    VOS_UINT16 result;
    VOS_INT16 txPower;
} VRRC_AT_Set_Tx_PowerQryCnf;

typedef struct {
    VOS_MSG_HEADER                      /* _H2ASN_Skip */
    AT_VPDCP_MsgTypeUint16 msgName;     /* ��Ϣ�� */
    VOS_UINT8              reserve1[2]; /* ���� */
    MN_CLIENT_ID_T         clientId;    /* Client ID */
    MN_OPERATION_ID_T      opId;        /* Operation ID */
    VOS_UINT8              reserve2;    /* ���� */
    VOS_UINT8              content[4];  /* ��Ϣ���� */
} AT_VPDCP_ReqMsg;

typedef struct {
    VOS_UINT32 qryRslt; /* ��ѯsuccess or failure */
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
