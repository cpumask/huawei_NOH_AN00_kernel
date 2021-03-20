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

#ifndef __RNIC_ENTITY_H__
#define __RNIC_ENTITY_H__

#include "vos.h"
#include "imm_interface.h"
#include "RnicCtx.h"
#include "rnic_cds_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

#define RNIC_BUILD_EXRABID(modemid, rabid) ((VOS_UINT8)((((modemid) << 6) & 0xC0) | ((rabid)&0x3F)))

#define RNIC_GET_MODEMID_FROM_EXRABID(ucExRabId) \
    (((VOS_UINT8)(ucExRabId) & (RNIC_RABID_TAKE_MODEM_1_MASK | RNIC_RABID_TAKE_MODEM_2_MASK)) >> 6)

#define RNIC_GET_RABID_FROM_EXRABID(ucExRabId) ((VOS_UINT8)(ucExRabId)&RNIC_RABID_UNTAKE_MODEM_MASK)

#define RNIC_GET_RMNETID_FROM_EXPARAM(ulExParam) ((VOS_UINT8)((ulExParam)&0x000000FF))


enum RNIC_ResultType {
    RNIC_OK         = 0, /* �������� */
    RNIC_NOTSTARTED = 1, /* δ��ʼ */
    RNIC_INPROGRESS = 2, /* ������ */
    RNIC_PERM       = 3,
    RNIC_NOENT      = 4,
    RNIC_IO         = 5,
    RNIC_NXIO       = 6,
    RNIC_NOMEM      = 7,
    /* δ���뵽�ڴ� */ /* δ���뵽�ڴ� */
    RNIC_BUSY        = 8,  /* RNIC�����豸æ */
    RNIC_NODEV       = 9,  /* ���豸 */
    RNIC_INVAL       = 10, /* �Ƿ��豸 */
    RNIC_NOTSUPP     = 11, /* ������֧�� */
    RNIC_TIMEDOUT    = 12, /* ��ʱ */
    RNIC_SUSPENDED   = 13, /* ���� */
    RNIC_UNKNOWN     = 14, /* δ֪���� */
    RNIC_TEST_FAILED = 15, /* ����ʧ�� */
    RNIC_STATE       = 16, /* ״̬���� */
    RNIC_STALLED     = 17, /* ʧ�� */
    RNIC_PARAM       = 18, /* �������� */
    RNIC_ABORTED     = 19, /* ����ȡ�� */
    RNIC_SHORT       = 20, /* ��Դ���� */
    RNIC_EXPIRED     = 21, /* ��� */

    RNIC_ADDR_INVALID = 22, /* �޷������ַ */
    RNIC_OUT_RANGE    = 23, /* ������Ч��Χ�� */
    RNIC_PKT_TYPE_INVAL = 24, /* ��Чip���� */
    RNIC_ADDMAC_FAIL    = 25, /* ���macͷʧ�� */
    RNIC_RX_PKT_FAIL    = 26, /* �����ں˽ӿڽ�������ʧ�� */
    RNIC_ERROR = 0xff, /* RNIC����ʧ�� */
    RNIC_BUTT
};
typedef VOS_INT32 RNIC_ResultTypeInt32;

/*
 * Э����:
 * ASN.1����:
 * �ṹ˵��: IP�̶�ͷ
 */
typedef struct {
    VOS_UINT8 ipHdrLen : 4; /* header length */
    VOS_UINT8 ipVer : 4;    /* version */

    VOS_UINT8  serviceType;    /* type of service */
    VOS_UINT16 totalLen;       /* total length */
    VOS_UINT16 identification; /* identification */
    VOS_UINT16 offset;         /* fragment offset field */
    VOS_UINT8  ttl;            /* time to live */
    VOS_UINT8  protocol;       /* protocol */
    VOS_UINT16 checkSum;       /* checksum */
    VOS_UINT32 srcAddr;        /* source address */
    VOS_UINT32 destAddr;       /* dest address */
} RNIC_IPFIXHDR;

VOS_INT RNIC_V4DataTxProc(IMM_Zc *immZc, VOS_UINT8 rmNetId, VOS_UINT32 flag);
VOS_INT RNIC_V6DataTxProc(IMM_Zc *immZc, VOS_UINT8 rmNetId, VOS_UINT32 flag);
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
VOS_INT RNIC_EthDataTxProc(IMM_Zc *immZc, VOS_UINT8 rmNetId, VOS_UINT32 flag);
VOS_INT RNIC_DataRxProc(VOS_ULONG usrData, IMM_Zc *immZc);
#else
VOS_INT RNIC_DataRxProc(VOS_UINT8 exRabid, IMM_Zc *immZc, ADS_PktTypeUint8 pktType, VOS_UINT32 exParam);
#endif /* FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM */

#if ((FEATURE_ON == FEATURE_IMS) && (FEATURE_ON == FEATURE_ACPU_PHONE_MODE))
VOS_VOID   RNIC_SendVoWifiUlData(RNIC_IFACE_Ctx *ifaceCtx);
VOS_INT    RNIC_VoWifiDataTxProc(IMM_Zc *immZc, RNIC_DEV_ID_ENUM_UINT8 rmNetId, VOS_UINT32 flag);
VOS_UINT32 RNIC_RecvVoWifiDlData(RNIC_DEV_ID_ENUM_UINT8 rmNetId, CDS_RNIC_ImsDataInd *imsDataInd);
#endif /* FEATURE_ON == FEATURE_IMS && FEATURE_ON == FEATURE_ACPU_PHONE_MODE */

VOS_VOID RNIC_TxDropProc(VOS_UINT8 rmNetId);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __RNIC_ENTITY_H__ */
