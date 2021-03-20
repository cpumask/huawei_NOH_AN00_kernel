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

#ifndef PPPA_PPPC_AT_TYPE_H
#define PPPA_PPPC_AT_TYPE_H

#include "v_typdef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#pragma pack(4)

#ifndef IPV4_ADDR_LEN
#define   IPV4_ADDR_LEN                     4       /*IPV4��ַ����*/
#endif

#define AUTHLEN                             100    /* Size of authname/authkey(porting from BSD, not alter) */
#define PASSWORDLEN                         100    /* Size of authname/authkey */
#define CHAPCHALLENGELEN                    48     /* Maximum chap challenge(porting from BSD, not alter) */
#define MD5DIGESTSIZE                       16     /* MD5 (Message-Digest) hash size */

#define PPP_PAP_REQ_BUF_MAX_LEN             (PASSWORDLEN + AUTHLEN)

/* code(1B)+id(1B)+length(2B)+challenge_size(1B)+challenge+name */
#define PPP_CHAP_CHALLENGE_BUF_MAX_LEN      (1 + 1 + 2 + 1 + CHAPCHALLENGELEN + AUTHLEN)

/* code(1B)+id(1B)+length(2B)+response_size(1B)+response+name */
#define PPP_CHAP_RESPONSE_BUF_MAX_LEN       (1 + 1 + 2 + 1 + PASSWORDLEN + AUTHLEN)

#define PPP_AUTH_FRAME_BUF_MAX_LEN          256
#define PPP_IPCP_FRAME_BUF_MAX_LEN          256

typedef struct {
    VOS_UINT16  papReqLen;                            /*request����: 24.008Ҫ����[3,253]�ֽ�*/
    VOS_UINT8   reserve[2];                          /* ���뱣�� */
    VOS_UINT8   papReqBuf[PPP_PAP_REQ_BUF_MAX_LEN];  /*request*/
} PPPA_AuthPapContent;

typedef struct {
    VOS_UINT16  chapChallengeLen;                     /*challenge����: 24.008Ҫ����[3,253]�ֽ�*/
    VOS_UINT16  chapResponseLen;                      /*response����: 24.008Ҫ����[3,253]�ֽ�*/
    VOS_UINT8   chapChallengeBuf[PPP_CHAP_CHALLENGE_BUF_MAX_LEN];  /*challenge,153B*/
    VOS_UINT8   chapResponseBuf[PPP_CHAP_RESPONSE_BUF_MAX_LEN];    /*response,205B*/
    VOS_UINT8   reserve[2];
} PPPA_AuthChapContent;

/* ��Ȩ���� */
enum PPP_AuthType {
    PPP_NO_AUTH_TYPE                     = 0,
    PPP_PAP_AUTH_TYPE                    = 1,
    PPP_CHAP_AUTH_TYPE                   = 2,
    PPP_MS_CHAPV2_AUTH_TYPE              = 3,
    PPP_AUTH_TYPE_BUTT
};
typedef VOS_UINT8 PPP_AuthTypeUint8;


typedef struct {
    PPP_AuthTypeUint8  authType;
    VOS_UINT8                 reserve[3];              /* ���뱣�� */

    union {
        PPPA_AuthPapContent  papContent;
        PPPA_AuthChapContent chapContent;
    } authContent;
} PPPA_ReqAuthConfigInfo;

typedef struct { 
    VOS_UINT16  ipcpLen;                              /*Ipcp֡����*/
    VOS_UINT8   reserve[2];                          /* ���뱣�� */
    VOS_UINT8   ipcp[PPP_IPCP_FRAME_BUF_MAX_LEN];    /*Ipcp֡*/
} PPPA_ReqIpcpConfigInfo;

typedef struct{
    PPPA_ReqAuthConfigInfo authConfig;
    PPPA_ReqIpcpConfigInfo ipcpConfig;
} PPPA_PdpActiveConfig;

typedef struct {
    VOS_UINT32                          opPriDns    : 1;             /*Primary DNS server Address*/
    VOS_UINT32                          opSecDns    : 1;             /*Secondary DNS server Address*/
    VOS_UINT32                          opGateWay   : 1;             /*Peer IP address*/
    VOS_UINT32                          opPriNbns   : 1;             /*Primary WINS DNS address*/
    VOS_UINT32                          opSecNbns   : 1;             /*Seocndary WINS DNS address*/
    VOS_UINT32                          opSpare     : 27;            /*Secondary DNS server Address*/

    VOS_UINT8                           priDns[IPV4_ADDR_LEN];
    VOS_UINT8                           secDns[IPV4_ADDR_LEN];
    VOS_UINT8                           gateWay[IPV4_ADDR_LEN];
    VOS_UINT8                           priNbns[IPV4_ADDR_LEN];
    VOS_UINT8                           secNbns[IPV4_ADDR_LEN];
} PPPA_PcoIpv4Item;

/*��ӦPPP_IND_CONFIG_INFO_STRU��������Ϣ��ʽ����*/
typedef struct {
    VOS_UINT8                    ipAddr[IPV4_ADDR_LEN];       /*����IP��ַ*/
    PPPA_PcoIpv4Item             pcoIpv4Item;                  /*PCO��Ϣ*/
} PPPA_PdpActiveResult;

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif