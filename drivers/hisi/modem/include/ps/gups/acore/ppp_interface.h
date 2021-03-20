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

#ifndef PPP_INTERFACE_H
#define PPP_INTERFACE_H

#include "pppa_pppc_at_type.h"
#include "product_config.h"
#include "v_typdef.h"
#include "PsTypeDef.h"
#if (OSA_CPU_ACPU == VOS_OSA_CPU) || (VOS_WIN32 == VOS_OS_VER) || (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
#include "imm_interface.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ϵͳ�����Ҫ��PPP ID����Ŀ */
#define PPP_MAX_ID_NUM 1

#ifndef IPV6_ADDR_LEN
#define IPV6_ADDR_LEN 16 /* IPV6��ַ���� */
#endif

enum PPP_AtCtrlOperType {
    PPP_AT_CTRL_REL_PPP_REQ     = 0, /* �ͷ�IP����PPP��·���� */
    PPP_AT_CTRL_REL_PPP_RAW_REQ = 1, /* �ͷ�PPP����PPP��·���� */
    PPP_AT_CTRL_HDLC_DISABLE    = 2, /* HDLCӲ��ģ��ȥʹ�ܲ��� */
    PPPA_CCORE_RESET_IND,

    PPP_AT_CTRL_BUTT
};
typedef VOS_UINT32 PPP_AtCtrlOperTypeUint32;

#pragma pack(4)

typedef VOS_UINT16 PPP_Id;

#define AT_PPP_RELEASE_IND_MSG 0x00 /* �ͷ�PDP���ӵ����� */
#define AT_PPP_PROTOCOL_REL_IND_MSG 0x02 /* PPP��·����ͷŵ�ָʾ */

typedef struct {
    MSG_Header      msgHeader;
    VOS_UINT16      clientId;
    VOS_UINT8       rsv[2];
} AT_PPP_ReleaseInd;

/* ���½ṹΪC��TAFʹ�ã�PPPA��ʹ�ã���TAF���ĺ�ɾ�� */
typedef struct {
    MSG_Header      msgHeader;
    VOS_UINT16      pppId;
    VOS_UINT8       rsv[2];
} AT_PPP_ProtocolRelInd;

typedef struct {
    VOS_UINT16 papReqLen;   /* request����: 24.008Ҫ����[3,253]�ֽ� */
    VOS_UINT8  rsv[2]; /* ���뱣�� */
    VOS_UINT8* papReq;       /* request */
} PPP_AuthPapContent;

typedef struct {
    VOS_UINT16 chapChallengeLen; /* challenge����: 24.008Ҫ����[3,253]�ֽ� */
    VOS_UINT16 chapResponseLen;  /* response����: 24.008Ҫ����[3,253]�ֽ� */
    VOS_UINT8* chapChallenge;     /* challenge */
    VOS_UINT8* chapResponse;      /* response */
} PPP_AuthChapContent;

typedef struct {
    PPP_AuthTypeUint8       authType;
    VOS_UINT8               rsv[3];

    union {
        PPP_AuthPapContent  papContent;
        PPP_AuthChapContent chapContent;
    } AuthContent;
} PPP_ReqAuthConfig;

typedef struct {
    VOS_UINT16 ipcpLen;     /* Ipcp֡���� */
    VOS_UINT8  rsv[2]; /* ���뱣�� */
    VOS_UINT8* ipcp;   /* Ipcp֡ */
} PPP_ReqIpcpConfig;

typedef struct {
    PPP_ReqAuthConfig reqAuthCfg;
    PPP_ReqIpcpConfig reqIpcpCfg;
} PPP_ReqConfig;

#pragma pack()

#if (VOS_OSA_CPU == OSA_CPU_ACPU) || (VOS_OS_VER == VOS_WIN32) || (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)

/*
 * �ϲ�ע���PPP�����������ݷ��ͽӿڡ�
 * userData��PPP����ʱЯ����PPP��������ʱ����, PPP����ע����ȡ
 */
typedef VOS_UINT32 (*PPP_SendDataToModem)(VOS_ULONG userData, IMM_Zc *data);

/* PPP�ṩ��AT�Ľӿ� */
extern VOS_UINT32 PPPA_CreatePppReq(PPP_Id* pppId, VOS_ULONG userData, PPP_SendDataToModem sender);
extern VOS_UINT32 PPPA_CreateRawDataPppReq(PPP_Id* pppId, VOS_ULONG userData, PPP_SendDataToModem sender);

extern VOS_UINT32 PPPA_RcvConfigInfoInd(PPP_Id pppId, PPPA_PdpActiveResult* pppIndConfigInfo);
extern VOS_VOID   PPP_UpdateWinsConfig(VOS_UINT8 wins);
extern VOS_VOID   PPP_SetRawDataByPassMode(VOS_UINT32 rawDataByPassMode);

/* IP��ʽ���ṩ���������ݽ��սӿ� */
extern VOS_UINT32 PPP_PullPacketEvent(VOS_UINT16 pppId, IMM_Zc* immZc);

/* PPP��ʽ���ṩ���������ݽ��սӿ� */
extern VOS_UINT32 PPP_PullRawDataEvent(VOS_UINT16 pppId, IMM_Zc* immZc);
#endif

extern VOS_UINT32 PPP_RcvAtCtrlOperEvent(VOS_UINT16 pppId, PPP_AtCtrlOperTypeUint32 ctrlOperType);

extern VOS_UINT32 PPP_RegDlDataCallback(PPP_Id pppId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

