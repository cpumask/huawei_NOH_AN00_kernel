/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Create: 2012/10/20
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 and
 *  only version 2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3) Neither the name of Huawei nor the names of its contributors may
 *     be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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

#ifndef __IPNDSERVER_H__
#define __IPNDSERVER_H__

/*
 * 1 Include Headfile
 */
#include "vos.h"
#include "PsTypeDef.h"
#include "LUPQueue.h"
#include "at_ndis_interface.h"
#include "ip_comm.h"
#include "ip_ipm_global.h"
#include "ps_iface_global_def.h"
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
#include "cds_ndis_interface.h"
#else
#include "imm_interface.h"
#include "ads_ndis_interface.h"
#endif

/*
 * 1.1 Cplusplus Announce
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
 * #pragma pack(*)    �����ֽڶ��뷽ʽ
 */
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*
 * 2 Macro
 */
#ifdef _lint
#define NDIS_NDSERVER_PID 420
#else
#define NDIS_NDSERVER_PID UEPS_PID_NDSERVER
#endif

/* ����IP��������г��� */
#define ND_IPV6_WAIT_ADDR_RSLT_Q_LEN 10

/* ND SERVER����ַ��Ϣ�� */
#define IP_NDSERVER_ADDRINFO_MAX_NUM 11

#define IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId) (NdSer_GetAddrInfoIdx(exRabId))

#define IP_NDSERVER_GET_EPSBID(index) (g_ndServerAddrInfo[index].epsbId)

#define IP_NDSERVER_ADDRINFO_GET_ADDR(index) (&g_ndServerAddrInfo[index])

#define IP_NDSERVER_ADDRINFO_GET_EPSID(index) (g_ndServerAddrInfo[index].epsbId)

#define IP_NDSERVER_ADDRINFO_GET_TIMER(index) (&(g_ndServerAddrInfo[index].timerInfo))

#define IP_NDSERVER_ADDRINFO_GET_NWPARA(index) (&(g_ndServerAddrInfo[index].ipv6NwPara))

#define IP_NDSERVER_ADDRINFO_GET_TEINFO(index) (&(g_ndServerAddrInfo[index].teAddrInfo))

#define IP_NDSERVER_ADDRINFO_GET_MACADDR(index) (g_ndServerAddrInfo[index].ueMacAddr)

#define IP_NDSERVER_ADDRINFO_GET_MACFRAME(index) (g_ndServerAddrInfo[index].macFrmHdr)

#define IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(index) (g_ndServerAddrInfo[index].dlPktQue)

#define IP_NDSERVER_GET_NDMSGDATA_ADDR() (&g_ndMsgData)

#define IP_NDSERVER_GET_SENDMSG_BUFFER() (g_ndserverSendMsgBuffer)

#define IP_NDSERVER_GET_STATINFO_ADDR(index) (&g_ndServerPktStatInfo[index])
#define IP_NDSERVER_AddRcvPktTotalNum(index) (g_ndServerPktStatInfo[index].rcvPktTotalNum++)
#define IP_NDSERVER_AddDiscPktNum(index) (g_ndServerPktStatInfo[index].discPktNum++)
#define IP_NDSERVER_AddRcvNsPktNum(index) (g_ndServerPktStatInfo[index].rcvNsPktNum++)
#define IP_NDSERVER_AddRcvNaPktNum(index) (g_ndServerPktStatInfo[index].rcvNaPktNum++)
#define IP_NDSERVER_AddRcvRsPktNum(index) (g_ndServerPktStatInfo[index].rcvRsPktNum++)
#define IP_NDSERVER_AddRcvEchoPktNum(index) (g_ndServerPktStatInfo[index].rcvEchoPktNum++)
#define IP_NDSERVER_AddRcvBooBigPktNum(index) (g_ndServerPktStatInfo[index].rcvTooBigPktNum++)
#define IP_NDSERVER_AddRcvDHCPV6PktNum(index) (g_ndServerPktStatInfo[index].rcvDhcpv6PktNum++)
#define IP_NDSERVER_AddErrNsPktNum(index) (g_ndServerPktStatInfo[index].errNsPktNum++)
#define IP_NDSERVER_AddErrNaPktNum(index) (g_ndServerPktStatInfo[index].errNaPktNum++)
#define IP_NDSERVER_AddErrRsPktNum(index) (g_ndServerPktStatInfo[index].errRsPktNum++)
#define IP_NDSERVER_AddErrEchoPktNum(index) (g_ndServerPktStatInfo[index].errEchoPktNum++)
#define IP_NDSERVER_AddErrTooBigPktNum(index) (g_ndServerPktStatInfo[index].errTooBigPktNum++)
#define IP_NDSERVER_AddErrDhcpv6PktNum(index) (g_ndServerPktStatInfo[index].errDhcpv6PktNum++)
#define IP_NDSERVER_AddTransPktTotalNum(index) (g_ndServerPktStatInfo[index].transPktTotalNum++)
#define IP_NDSERVER_AddTransPktFailNum(index) (g_ndServerPktStatInfo[index].transPktFailNum++)
#define IP_NDSERVER_AddTransNsPktNum(index) (g_ndServerPktStatInfo[index].transNsPktNum++)
#define IP_NDSERVER_AddTransNaPktNum(index) (g_ndServerPktStatInfo[index].transNaPktNum++)
#define IP_NDSERVER_AddTransRaPktNum(index) (g_ndServerPktStatInfo[index].transRaPktNum++)
#define IP_NDSERVER_AddTransEchoPktNum(index) (g_ndServerPktStatInfo[index].transEchoPktNum++)
#define IP_NDSERVER_AddTransTooBigPktNum(index) (g_ndServerPktStatInfo[index].transTooBigPktNum++)
#define IP_NDSERVER_AddTransDhcpv6PktNum(index) (g_ndServerPktStatInfo[index].transDhcpv6PktNum++)
#define IP_NDSERVER_AddMacInvalidPktNum(index) (g_ndServerPktStatInfo[index].macInvalidPktNum++)
#define IP_NDSERVER_AddEnquePktNum(index) (g_ndServerPktStatInfo[index].enquePktNum++)
#define IP_NDSERVER_AddSendQuePktNum(index) (g_ndServerPktStatInfo[index].sendQuePktNum++)

/*
 * 4 Enum
 */
/*
 * Э����:
 * ASN.1����:
 * ö��˵��: NDSERVER��ʱ��ö��
 */
enum NDSER_Timer {
    /* ND SERVER��ʱ�� */
    IP_ND_SERVER_TIMER_NS          = 0,
    IP_ND_SERVER_TIMER_PERIODIC_NS = 1,
    IP_ND_SERVER_TIMER_FIRST_NS    = 2,
    IP_ND_SERVER_TIMER_RA          = 3,

    IP_ND_SERVER_TIMER_BUTT
};
typedef VOS_UINT32 NDSER_TimerUint32;

/*
 * Э����:
 * ASN.1����:
 * ö��˵��: TE��ַ״̬����
 */
enum IP_NdserverTeAddrState {
    IP_NDSERVER_TE_ADDR_INEXISTENT = 0, /* ������ */
    IP_NDSERVER_TE_ADDR_INCOMPLETE = 1, /* δ��� */
    IP_NDSERVER_TE_ADDR_REACHABLE  = 2, /* �ɴ� */
    IP_NDSERVER_TE_ADDR_BUTT
};
typedef VOS_UINT8 IP_NdserverTeAddrStateUint8;

/*
 * 5 STRUCT
 */
/*
 * �ṹ��: IP_NdserverTeAddrInfo
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ND SERVER�����TE��ַ�ṹ��
 */
typedef struct {
    VOS_UINT8                            teLinkLocalAddr[IP_IPV6_ADDR_LEN];
    VOS_UINT8                            teGlobalAddr[IP_IPV6_ADDR_LEN];
    VOS_UINT8                            teLinkLayerAddr[IP_MAC_ADDR_LEN];
    IP_NdserverTeAddrStateUint8 teAddrState;
    VOS_UINT8                            reserved[1];
} IP_NdserverTeAddrInfo;

/*
 * �ṹ��: IP_Ipv6Addr
 * Э����:
 * ASN.1����:
 * �ṹ˵��: IPV6��ַ�ṹ��
 */
typedef struct {
    VOS_UINT32 valid;
    VOS_UINT8  teGlobalAddr[IP_IPV6_ADDR_LEN];
} IP_Ipv6Addr;

/*
 * �ṹ��: IP_NdserverTeDetectBuf
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ND SERVER�����TE��ַ�ṹ�壬����MAC��ַ��ȡ
 */
typedef struct {
    VOS_UINT32        head;
    VOS_UINT32        tail;
    VOS_UINT32        maxNum;
    IP_Ipv6Addr teIpBuf[2 * (IP_NDSERVER_ADDRINFO_MAX_NUM)]; /* 2:buf */
} IP_NdserverTeDetectBuf;

/*
 * �ṹ��: IP_NdserverAddrInfo
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ND SERVER���ݽṹ��
 */
typedef struct {
    VOS_UINT8 epsbId;
    VOS_UINT8 validFlag;
    VOS_UINT8 reserved[14]; /* 14:rsv */
    /* Ϊ��ͬ64λ����ϵͳ���ݣ�����8�ֽڶ���λ�� */
    LUP_QUEUE_STRU *dlPktQue; /* ���а�������� */
    IP_SndMsg ipSndNwMsg;

    ESM_IP_Ipv6NWPara ipv6NwPara;

    IP_NdserverTeAddrInfo teAddrInfo;

    VOS_UINT8 ueLinkLocalAddr[IP_IPV6_ADDR_LEN];

    IP_TimerInfo timerInfo;

    VOS_UINT8 ueMacAddr[IP_MAC_ADDR_LEN];
    VOS_UINT8 macFrmHdr[IP_ETH_MAC_HEADER_LEN];
} IP_NdserverAddrInfo;

/*
 * �ṹ��: IP_NdserverPacketStatisticsInfo
 * Э����:
 * ASN.1����:
 * �ṹ˵��: ND SERVER����ͳ�����ݽṹ��
 */
typedef struct {
    VOS_UINT32 rcvPktTotalNum;    /* �յ����ݰ����� */
    VOS_UINT32 discPktNum;        /* �������ݰ����� */
    VOS_UINT32 rcvNsPktNum;       /* �յ�NS���ݰ����� */
    VOS_UINT32 rcvNaPktNum;       /* �յ�NA���ݰ����� */
    VOS_UINT32 rcvRsPktNum;       /* �յ�RS���ݰ����� */
    VOS_UINT32 rcvEchoPktNum;     /* �յ�PING������ */
    VOS_UINT32 rcvTooBigPktNum;   /* �յ�IPV6���������� */
    VOS_UINT32 rcvDhcpv6PktNum;   /* �յ�DHCPV6������ */
    VOS_UINT32 errNsPktNum;       /* ����NS���ݰ����� */
    VOS_UINT32 errNaPktNum;       /* ����NA���ݰ����� */
    VOS_UINT32 errRsPktNum;       /* ����RS���ݰ����� */
    VOS_UINT32 errEchoPktNum;     /* ����ECHO REQ���ݰ����� */
    VOS_UINT32 errTooBigPktNum;   /* ���󳬳������� */
    VOS_UINT32 errDhcpv6PktNum;   /* ����DHCPV6���ݰ����� */
    VOS_UINT32 transPktTotalNum;  /* �������ݰ����� */
    VOS_UINT32 transPktFailNum;   /* �������ݰ�ʧ������ */
    VOS_UINT32 transNsPktNum;     /* ����NS���ݰ����� */
    VOS_UINT32 transNaPktNum;     /* ����NA���ݰ����� */
    VOS_UINT32 transRaPktNum;     /* ����RA���ݰ����� */
    VOS_UINT32 transEchoPktNum;   /* ����ECHO REPLY���ݰ����� */
    VOS_UINT32 transTooBigPktNum; /* ����IPV6��������Ӧ���� */
    VOS_UINT32 transDhcpv6PktNum; /* ����DHCPV6 REPLY���ݰ����� */
    VOS_UINT32 macInvalidPktNum;  /* ����IP������ʱPC MAC��ַ��Ч��ͳ���� */
    VOS_UINT32 enquePktNum;       /* ���гɹ������IP������ */
    VOS_UINT32 sendQuePktNum;     /* ���гɹ����ͻ����IP������ */
} IP_NdserverPacketStatisticsInfo;

/*
 * 7 Extern Global Variable
 */
extern IP_NdserverAddrInfo g_ndServerAddrInfo[];

/*
 * 8 Fuction Extern
 */
VOS_UINT32 NdSer_CheckIpv6PdnInfo(const AT_NDIS_Ipv6PdnInfo *ipv6PdnInfo);
VOS_VOID   NdSer_Ipv6PdnInfoCfg(VOS_UINT8 rabId, const AT_NDIS_Ipv6PdnInfo *ipv6PdnInfo);
VOS_VOID   NdSer_DhcpV6PktProc(const MsgBlock *msgBlock);
VOS_UINT32 NdSer_GetAddrInfoIdx(VOS_UINT8 exRabId);
VOS_UINT8 *NdSer_GetMacFrm(VOS_UINT8 index, VOS_UINT8 *teAddrState);
VOS_VOID   NdSer_Ipv6PdnRel(VOS_UINT8 rabId);
VOS_VOID   NdSer_NdAndEchoPktProc(const MsgBlock *msgBlock);
VOS_UINT32 APP_NdServer_Pid_InitFunc(enum VOS_InitPhaseDefine ePhase);
VOS_VOID   IP_NDSERVER_ProcTooBigPkt(VOS_UINT8 rabId, const VOS_UINT8 *srcData, VOS_UINT32 dataLen);
VOS_VOID   APP_NdServer_PidMsgProc(MsgBlock *rcvMsg);
IP_NdserverAddrInfo *NdSer_AllocAddrInfo(VOS_UINT32 *pUlIndex);

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_VOID   NdSer_MacAddrInvalidProc(IMM_Zc *immZc, VOS_UINT8 index);
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of ip_nd_server.h */
