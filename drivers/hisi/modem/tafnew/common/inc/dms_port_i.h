/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
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


#ifndef _DMS_PORT_I_H_
#define _DMS_PORT_I_H_

#include <product_config.h>
#include "vos.h"
#include "imm_interface.h"
#include "dms_port_def.h"
#include "cbt_cpm.h"
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
#include "cmux_api_ifc.h"
#include "cmux_tgt_osal.h"
#include "cmux_drv_ifc.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(push, 4)

typedef VOS_UINT32 (*DMS_PORT_RCV_PPP_DATA_FUNC)(VOS_UINT16 pppId, IMM_Zc* immZc);

struct DMS_ClientInfo {
    VOS_UINT16              id;
    DMS_ClientTypeUint8     type;
    VOS_UINT8               reserv1;

    VOS_UINT16              pppId;
    VOS_UINT8               reserv2[2];

    VOS_INT  (*RcvCmdData)(VOS_UINT16 clientId, VOS_UINT8 *data, VOS_UINT32 len);

    union {
        VOS_UINT32  (*RcvPppData)(VOS_UINT16 pppId, IMM_Zc* immZc);
        VOS_UINT32  (*RcvCbtData)(VOS_UINT8 *buffer, VOS_UINT32 len);
    };
};

VOS_UINT32 DMS_PORT_RegClientInfo(DMS_PortIdUint16 portId, struct DMS_ClientInfo *clientInfo);

VOS_VOID DMS_PORT_Init(VOS_VOID);

/* ����ֵ�����ͳɹ���VOS_OK������ʧ�ܣ�VOS_ERR */
VOS_UINT32 DMS_PORT_SendData(DMS_PortIdUint16 portId, VOS_UINT8 *data, VOS_UINT16 len);

/* ncm��uart�ȶ˿ڵ�������������API�ӿ� */
VOS_UINT32  DMS_PORT_SetIpv6Dns(DMS_PortIdUint16 portId, VOS_UINT8 *ipv6DnsInfo, VOS_UINT32 len);
VOS_UINT32 DMS_PORT_ChangeFlowCtrl(DMS_PortIdUint16 portId, VOS_UINT32 enableFlg);
VOS_UINT32 DMS_PORT_ChangeConnSpeed(DMS_PortIdUint16 portId, VOS_UINT32 downBitrate, VOS_UINT32 upBitrate);
VOS_UINT32 DMS_PORT_ChangeConnLinkState(DMS_PortIdUint16 portId, VOS_UINT32 linkStatus);

#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_UINT32 DMS_PORT_ConfigFlowCtrl(DMS_PortIdUint16 portId, VOS_UINT32 rtsFlag, VOS_UINT32 ctsFlag);
VOS_UINT32 DMS_PORT_SetCharFrame(DMS_PortIdUint16 portId, VOS_UINT32 udiDataLen, VOS_UINT32 udiStpLen, VOS_UINT32 udiParity);
VOS_UINT32 DMS_PORT_SetBaud(DMS_PortIdUint16 portId, VOS_UINT32 baudRate);
VOS_UINT32 DMS_PORT_SetACShell(DMS_PortIdUint16 portId, VOS_UINT32 mode);
VOS_UINT32 DMS_PORT_FlushTxData(DMS_PortIdUint16 portId);
VOS_UINT32 DMS_PORT_StartHsuartFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2);
VOS_UINT32 DMS_PORT_StopHsuartFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2);
#endif
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
VOS_INT32 DMS_PORT_SwitchUart2Cmux(cmux_info_type* CmuxInfo, DMS_PortIdUint16 portId);
VOS_UINT8 DMS_PORT_GetCmuxActivePppPstaskDlc(VOS_VOID);
VOS_VOID DMS_PORT_SetCmuxActivePppPstaskDlc(VOS_UINT8 dlc);
#endif

VOS_INT32 DMS_PORT_GetPortHandle(DMS_PortIdUint16 portId);

/* ͨ��ģʽ��� */
VOS_VOID DMS_PORT_SetMode(DMS_PortIdUint16 portId, DMS_PortModeUint8 mode);
VOS_VOID DMS_PORT_SetDataMode(DMS_PortIdUint16 portId, DMS_PortDataModeUint8 dataMode);
VOS_VOID DMS_PORT_SetDataState(DMS_PortIdUint16 portId, DMS_PortDataStateUint8 dataState);

DMS_PortModeUint8 DMS_PORT_GetMode(DMS_PortIdUint16 portId);
DMS_PortDataModeUint8 DMS_PORT_GetDataMode(DMS_PortIdUint16 portId);
DMS_PortDataStateUint8 DMS_PORT_GetDataState(DMS_PortIdUint16 portId);

VOS_BOOL DMS_PORT_IsPppPacketTransMode(DMS_PortIdUint16 portId);

VOS_UINT32 DMS_PORT_StartModemFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2);
VOS_UINT32 DMS_PORT_StopModemFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2);

/* OM CBT���պͷ��ͺ���ע�� */
VOS_BOOL DMS_PORT_IsExistCbtPort(VOS_VOID);

/* PPP���պ���ע�ắ�����ͷ��ͺ��� */
VOS_UINT32 DMS_PORT_SendPppPacket(VOS_ULONG userData, IMM_Zc *dataBuf);

/* �˿�ģʽ������ģʽ�л�����ͬ������ģʽ */
VOS_VOID DMS_PORT_SwitchToOmDataMode(DMS_PortIdUint16 portId, CBTCPM_RCV_FUNC rcvFunc);
VOS_VOID DMS_PORT_SwitchToPppDataMode(DMS_PortIdUint16 portId, DMS_PortDataModeUint8 dataMode,
                                      DMS_PORT_RCV_PPP_DATA_FUNC func, VOS_UINT16 pppId);

/* �˿�ģʽ�ָ���CMDģʽ������ */
VOS_VOID DMS_PORT_ResumeCmdMode(DMS_PortIdUint16 portId);

/* �����¼� ���������ӿ�ֻ����dms fid�µĸ�pidʹ�ã���fidʹ�ÿ��ܴ�������*/
VOS_VOID DMS_PORT_SubscripEvent(DMS_SubscripEventIdUint32 eventId, VOS_UINT32 pid);
VOS_VOID DMS_PORT_DesubscripEvent(DMS_SubscripEventIdUint32 eventId, VOS_UINT32 pid);

/* �ܽ��ź���ؽӿ� */
VOS_VOID DMS_PORT_AssertDcd(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_AssertDsr(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_AssertCts(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_AssertRi(DMS_PortIdUint16 portId);

VOS_VOID DMS_PORT_DeassertDcd(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_DeassertDsr(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_DeassertCts(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_DeassertRi(DMS_PortIdUint16 portId);

VOS_VOID DMS_PORT_SetDcdMode(DMS_PortUartDcdModeUint8  dcdMode);
VOS_VOID DMS_PORT_SetDsrMode(DMS_PortUartDsrModeUint8  dsrMode);
VOS_VOID DMS_PORT_SetDtrMode(DMS_PortUartDtrModeUint8  dtrMode);

VOS_BOOL DMS_PORT_IsCtsDeasserted(DMS_PortIdUint16 portId);
VOS_BOOL DMS_PORT_IsRiAsserted(DMS_PortIdUint16 portId);

VOS_BOOL DMS_PORT_IsDcdModeConnectOn(VOS_VOID);
VOS_BOOL DMS_PORT_IsDsrModeConnectOn(VOS_VOID);

VOS_VOID DMS_PORT_ProcNoCarrierMsc(DMS_PortIdUint16 portId);
VOS_VOID DMS_PORT_ProcConnectMsc(DMS_PortIdUint16 portId);

/* uartˮ�߱�־ */
VOS_UINT32 DMS_PORT_GetUartWaterMarkFlg(VOS_VOID);
VOS_VOID DMS_PORT_SetUartWaterMarkFlg(VOS_UINT32 txWmHighFlg);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
