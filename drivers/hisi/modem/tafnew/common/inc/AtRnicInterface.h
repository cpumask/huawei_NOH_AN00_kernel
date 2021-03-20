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

#ifndef __AT_RNIC_INTERFACE_H__
#define __AT_RNIC_INTERFACE_H__

#include "vos.h"
#include "rnic_dev_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

#if (VOS_OS_VER == VOS_WIN32)
/* ����ST���������IP���ݰ����ȣ���λ�ֽ� */
#define RNIC_IP_DATA_LEN 12
#endif

/* APP���ź�Ĭ�Ͽ���ʱ�� */
#define AT_APP_IDLE_TIME 600

#define RNIC_RMNET_NAME_MAX_LEN 16

/*
 * �ṹ˵��: USB Tethering����״̬
 */
enum AT_RNIC_UsbTetherConn {
    AT_RNIC_USB_TETHER_DISCONNECT = 0x00, /* USB tetheringδ���� */
    AT_RNIC_USB_TETHER_CONNECTED  = 0x01, /* USB tethering������ */
    AT_RNIC_USB_TETHER_CONN_BUTT
};
typedef VOS_UINT8 AT_RNIC_UsbTetherConnUint8;

/*
 * �ṹ˵��: RNIC����ҵ��������������ģʽ
 */
enum AT_RNIC_FeatureMode {
    AT_RNIC_SINGLE_RMNET = 0x00, /* �������������� */
    AT_RNIC_ALL_RMNET    = 0x01, /* ��������ҵ�������������� */
    AT_RNIC_FEATURE_MODE_BUTT
};
typedef VOS_UINT8 AT_RNIC_FeatureModeUint8;

/*
 * ö��˵��: AT��RNIC����Ϣ����
 */
enum AT_RNIC_MsgId {
    /* AT����RNIC����Ϣö�� */
    /* 0x0001, 0x0003 ~ 0x0008 ɾ�� */
    ID_AT_RNIC_DIAL_MODE_REQ = 0x0002, /* ����ģʽ��ѯ */
    ID_AT_RNIC_USB_TETHER_INFO_IND = 0x000B, /* USB Tethering��Ϣָʾ */
    ID_AT_RNIC_RMNET_CFG_REQ = 0x000C, /* �������������������� */

    /* RNIC����AT����Ϣö�� */
    /* 0x1001 0x1002ɾ�� */
    ID_RNIC_AT_DIAL_MODE_CNF = 0x1003, /* ����ģʽ��ѯ�ظ� */
    ID_AT_RNIC_RMNET_CFG_CNF = 0x1004, /* ���������������ûظ� */
    ID_RNIC_AT_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32 AT_RNIC_MsgIdUint32;

/*
 * 4 STRUCT&UNION����
 */

/*
 * �ṹ˵��: AT��RNIC����������ѯ����
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;

    VOS_UINT16 clientId; /* Client ID */
    VOS_UINT8  rsv[2];   /* ���� */
} AT_RNIC_DialModeReq;

/*
 * �ṹ˵��: RNIC��AT����ģʽ��ѯ�ظ�
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;

    VOS_UINT16 clientId;        /* Client ID */
    VOS_UINT8  rsv[2];          /* ���� */
    VOS_UINT32 dialMode;        /* Dialģʽ */
    VOS_UINT32 idleTime;        /* ��ʱ������ */
    VOS_UINT32 eventReportFlag; /* �Ƿ��Ӧ���ϱ���ʶ */
} RNIC_AT_DialModeCnf;

/*
 * �ṹ˵��: USB������Ϣָʾ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;

    VOS_UINT8                  rmnetName[RNIC_RMNET_NAME_MAX_LEN]; /* Rmnet������ */
    AT_RNIC_UsbTetherConnUint8 tetherConnStat;                     /* USB Tethering����״̬ */
    VOS_UINT8                  rsv[3];

} AT_RNIC_UsbTetherInfoInd;

/*
 * �ṹ˵��: ����������������ָʾ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;

    VOS_UINT16               clientId;
    AT_RNIC_FeatureModeUint8 featureMode;
    VOS_UINT8                weight;
    VOS_UINT8                rmnetName[RNIC_RMNET_NAME_MAX_LEN];

} AT_RNIC_RmnetCfgReq;

/*
 * �ṹ˵��: ����������������ָʾ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgId;

    VOS_UINT16 clientId;
    VOS_UINT8  rsv[2];
    VOS_INT    result;

} RNIC_AT_RmnetCfgCnf;

extern VOS_UINT32 RNIC_StartFlowCtrl(RNIC_DEV_ID_ENUM_UINT8 rmNetId);
extern VOS_UINT32 RNIC_StopFlowCtrl(RNIC_DEV_ID_ENUM_UINT8 rmNetId);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __AT_RNIC_INTERFACE_H__ */
