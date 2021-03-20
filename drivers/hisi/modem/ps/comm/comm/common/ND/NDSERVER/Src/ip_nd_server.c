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

/*
 * 1 Include HeadFile
 */
#include "ip_nd_server.h"
#include "mdrv.h"
#include "ps_tag.h"
#include "securec.h"
#include "ps_ndis.h"
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#include "ads_dev_i.h"
#include "ads_device_interface.h"
#endif


#define THIS_MODU ps_nd
/*lint -e767*/
#define THIS_FILE_ID PS_FILE_ID_IPNDSERVER_C
/*lint +e767*/

/*
 * 2 Declare the Global Variable
 */
/* ND SERVER������Ϣ */
IP_NdserverAddrInfo     g_ndServerAddrInfo[IP_NDSERVER_ADDRINFO_MAX_NUM];
IP_NdserverTeDetectBuf g_ndServerTeDetectBuf;

/* ND SERVER���ر�������ò��� */
VOS_UINT8  g_ndServerMFlag;          /* M��ʶ */
VOS_UINT8  g_ndServerOFlag;          /* O��ʶ */
VOS_UINT16 g_routerLifetime; /* Router Lifetime */
VOS_UINT32 g_reachableTime;  /* Reachable Time */
VOS_UINT32 g_retransTimer;   /* Retrans Timer */

VOS_UINT32 g_nsTimerLen;         /* ��������NS��ʱ��ʱ�� */
VOS_UINT32 g_nsTimerMaxExpNum;   /* ��������NS���ʱ���� */
VOS_UINT32 g_periodicNsTimerLen; /* ������NS��ʱ��ʱ�� */
VOS_UINT32 g_periodicRaTimerLen; /* ������RA��ʱ��ʱ������ֹRouter���� */
VOS_UINT32 g_firstNsTimerLen;    /* �յ��ظ���ַ����ȴ��Ķ�ʱ��ʱ�� */
VOS_UINT32 g_raTimerLen;         /* �յ��ظ���ַ���ǰ���ڷ���RA��ʱ��ʱ�� */

/* �����Է���RAʱ������� */
VOS_UINT32 g_periodicRaTimeCnt[IP_NDSERVER_ADDRINFO_MAX_NUM];

/* ���ڴ洢�������ND�������� */
IP_ND_Msg g_ndMsgData;

/* ND SERVERʹ�õķ��������� */
VOS_UINT8 g_ndserverSendMsgBuffer[IP_IPM_MTU];

/* ND SERVER����ͳ����Ϣ */
IP_NdserverPacketStatisticsInfo g_ndServerPktStatInfo[IP_NDSERVER_ADDRINFO_MAX_NUM];

/*
 * ��������: ��ʱ����Te IP��ַ����TE�����ظ���ַ���������������ȡTE MAC��
 *             ַ
 * �޸���ʷ:
 *  1.��    ��: 2011��5��17��
 *    �޸�����: �����ɺ���
 */
STATIC VOS_VOID IP_NDSERVER_SaveTeDetectIp(const VOS_UINT8 *teGlobalAddr)
{
    errno_t    rlt;
    VOS_UINT32 bufIndex = g_ndServerTeDetectBuf.head;

    if ((IP_IPV6_IS_LINKLOCAL_ADDR(teGlobalAddr)) || (IP_IPV6_64BITPREFIX_EQUAL_ZERO(teGlobalAddr)) ||
        (IP_IPV6_IS_MULTICAST_ADDR(teGlobalAddr))) {
        /* ����Global IP */
        return;
    }

    /* ��ͬPrefix�ĵ�ַ�Ƿ��Ѿ����� */
    while (bufIndex != g_ndServerTeDetectBuf.tail) {
        /*lint -e960*/
        if ((g_ndServerTeDetectBuf.teIpBuf[bufIndex].valid == IP_TRUE) &&
            (IP_MEM_CMP(teGlobalAddr, g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr,
                        ND_IP_IPV6_PREFIX_LENGTH) == 0)) {
            /*lint +e960*/
            /* ��ͬPrefixֻ����һ����ַ */
            rlt = memcpy_s(g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr, IP_IPV6_ADDR_LEN,
                           teGlobalAddr, IP_IPV6_ADDR_LEN);
            IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
            return;
        }
        bufIndex = TTF_MOD_ADD(bufIndex, 1, g_ndServerTeDetectBuf.maxNum);
    }

    if (g_ndServerTeDetectBuf.head ==
        TTF_MOD_ADD(g_ndServerTeDetectBuf.tail, 1, g_ndServerTeDetectBuf.maxNum)) {
        /* BUF��ʱ�������ϵĵ�ַ */
        g_ndServerTeDetectBuf.head = TTF_MOD_ADD(g_ndServerTeDetectBuf.head, 1,
                                                 g_ndServerTeDetectBuf.maxNum);
    }

    /* ����IP��ַ */
    rlt = memcpy_s(g_ndServerTeDetectBuf.teIpBuf[g_ndServerTeDetectBuf.tail].teGlobalAddr,
                   IP_IPV6_ADDR_LEN, teGlobalAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    g_ndServerTeDetectBuf.teIpBuf[g_ndServerTeDetectBuf.tail].valid = IP_TRUE;

    g_ndServerTeDetectBuf.tail = TTF_MOD_ADD(g_ndServerTeDetectBuf.tail, 1,
                                             g_ndServerTeDetectBuf.maxNum);
}

/*
 * ��������: ��ȡ��ʱ�����TE IP��ַ
 * �޸���ʷ:
 *  1.��    ��: 2011��5��17��
 *    �޸�����: �����ɺ���
 */
STATIC VOS_UINT8 *IP_NDSERVER_GetTeDetectIp(const VOS_UINT8 *prefixAddr)
{
    VOS_UINT32 bufIndex = g_ndServerTeDetectBuf.head;
    while (bufIndex != g_ndServerTeDetectBuf.tail) {
        /*lint -e960*/
        if ((g_ndServerTeDetectBuf.teIpBuf[bufIndex].valid == IP_TRUE) &&
            (IP_MEM_CMP(prefixAddr, g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr,
                        ND_IP_IPV6_PREFIX_LENGTH) == 0)) {
            /*lint +e960*/
            g_ndServerTeDetectBuf.teIpBuf[bufIndex].valid = IP_FALSE;

            /* Bufͷ����϶���� */
            while ((g_ndServerTeDetectBuf.head != g_ndServerTeDetectBuf.tail) &&
                   (g_ndServerTeDetectBuf.teIpBuf[g_ndServerTeDetectBuf.head].valid == IP_FALSE)) {
                g_ndServerTeDetectBuf.head = TTF_MOD_ADD(g_ndServerTeDetectBuf.head, 1,
                                                         g_ndServerTeDetectBuf.maxNum);
            }

            return g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr;
        }
        bufIndex = TTF_MOD_ADD(bufIndex, 1, g_ndServerTeDetectBuf.maxNum);
    }

    return IP_NULL_PTR;
}

/*
 * Description: ���ñ����������
 * History:
 *    1.      2011-04-01  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_SetLocalParam(VOS_VOID)
{
    /* M��ʶ��0����������ͨ��DHCPv6��ȡIPv6��ַ */
    g_ndServerMFlag = 0;

    /* O��ʶ��1����������ͨ����״̬DHCPv6��ȡ��������������DNS��������SIP�������� */
    g_ndServerOFlag = 1;

    /* Router Lifetime(��) */
    g_routerLifetime = 9000; /* ·��������9000s */

    /* 3600000:Reachable Time(����) */
    g_reachableTime = 3600000;

    /* Retrans Timer(����)��0����ʾδ֪ */
    g_retransTimer = 0;

    /* 4000:��������NS��ʱ��ʱ��(����) */
    g_nsTimerLen = 4000;

    /* 3:��������NS���ʱ���� */
    g_nsTimerMaxExpNum = 3;

    /* 60000:������NS��ʱ��ʱ��(����) */
    g_periodicNsTimerLen = 60000;

    /* 3600000:������RA����ֹRouter����(����) */
    g_periodicRaTimerLen = 3600000;

    /* 2000:�յ��ظ���ַ����ȴ��Ķ�ʱ��ʱ��(����) */
    g_firstNsTimerLen = 2000;

    /* 15000:�յ��ظ���ַ���ǰ���ڷ���RA�Ķ�ʱ��ʱ��(����) */
    g_raTimerLen = 15000;

    return;
}

/*
 * Description: ���ͳ����Ϣ
 * History: VOS_VOID
 *    1.      2011-04-11  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_DebugInit(VOS_VOID)
{
    VOS_UINT32 addrIndex;
    for (addrIndex = 0; addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM; addrIndex++) {
        (VOS_VOID)memset_s(IP_NDSERVER_GET_STATINFO_ADDR(addrIndex), sizeof(IP_NdserverPacketStatisticsInfo),
                           IP_NULL, sizeof(IP_NdserverPacketStatisticsInfo));
    }

    return;
}

/*
 * Description: �ж϶�ʱ�����Ƿ�Ϸ�
 * Input: ulIndex     --- ND SERVER������������
 *                   enTimerType --- ��ʱ������
 * History: VOS_UINT32
 *    1.      2011-04-02  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_IsTimerNameValid(VOS_UINT32 addrIndex, NDSER_TimerUint32 timerType)
{
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
        case IP_ND_SERVER_TIMER_FIRST_NS:
        case IP_ND_SERVER_TIMER_RA: {
            if (addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM) {
                return IP_TRUE;
            }

            break;
        }
        default:
            break;
    }

    return IP_FALSE;
}

/*
 * Description: ��ȡ��ʱ��
 * Input: enTimerType------------------��ʱ������
 *                   ulIndex----------------------��ʱ��������
 * History:
 *    1.      2011-04-02  Draft Enact
 */
IP_TimerInfo *IP_NDSERVER_GetTimer(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    IP_TimerInfo *timerInfo = VOS_NULL_PTR;

    /* ���ݶ�ʱ����ͬ���ͣ���ȡ��ʱ�� */
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
        case IP_ND_SERVER_TIMER_FIRST_NS:
        case IP_ND_SERVER_TIMER_RA:
            timerInfo = IP_NDSERVER_ADDRINFO_GET_TIMER(timerIndex);
            break;
        default:
            break;
    }

    return timerInfo;
}

/*
 * Description: ��ȡ��ʱ��ʱ��
 * Input: enTimerType------------------��ʱ������
 * History:
 *    1.      2011-04-02  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_GetTimerLen(NDSER_TimerUint32 timerType)
{
    VOS_UINT32 timerLen;

    /* ���ݶ�ʱ����ͬ���ͣ���ʱ��ʱ����ͬ */
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
            timerLen = g_nsTimerLen;
            break;
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
            timerLen = g_periodicNsTimerLen;
            break;
        case IP_ND_SERVER_TIMER_FIRST_NS:
            timerLen = g_firstNsTimerLen;
            break;
        case IP_ND_SERVER_TIMER_RA:
            timerLen = g_raTimerLen;
            break;
        default:
            timerLen = IP_NULL;
            break;
    }

    return timerLen;
}
/*lint -e550*/
/*
 * Description: ��ӡ��ʱ��������Ϣ
 * Input: enTimerType------------------��ʱ������
 *                   ulIndex----------------------��ʱ��������
 * History:
 *    1.      2010-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_PrintTimeStartInfo(VOS_UINT32 addrIndex, NDSER_TimerUint32 timerType)
{
    /* ���ݶ�ʱ����ͬ���ͣ���ӡ��Ӧ��Ϣ */
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStartInfo:NORM:IP TimerStart(ulIndex): IP_ND_SERVER_TIMER_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStartInfo:NORM:IP TimerStart(ulIndex): IP_ND_SERVER_TIMER_PERIODIC_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_FIRST_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStartInfo:NORM:IP TimerStart(ulIndex): IP_ND_SERVER_TIMER_FIRST_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_RA:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStartInfo:NORM:IP TimerStart(ulIndex): IP_ND_SERVER_TIMER_RA",
                           addrIndex);
            break;
        default:
            break;
    }

    return;
}

/*
 * Description: ��ӡ��ʱ���ر���Ϣ
 * Input: enTimerType------------------��ʱ������
 *                   ulIndex----------------------��ʱ��������
 * History:
 *    1.      2010-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_PrintTimeStopInfo(VOS_UINT32 addrIndex, NDSER_TimerUint32 timerType)
{
    /* ���ݶ�ʱ����ͬ���ͣ���ӡ��Ӧ��Ϣ */
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStopInfo:NORM:IP TimerStop(ulIndex): IP_ND_SERVER_TIMER_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStopInfo:NORM:IP TimerStop(ulIndex): IP_ND_SERVER_TIMER_PERIODIC_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_FIRST_NS:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStopInfo:NORM:IP TimerStop(ulIndex): IP_ND_SERVER_TIMER_FIRST_NS",
                           addrIndex);
            break;
        case IP_ND_SERVER_TIMER_RA:
            IPND_INFO_LOG1(NDIS_NDSERVER_PID,
                           "IP_NDSERVER_PrintTimeStopInfo:NORM:IP TimerStop(ulIndex): IP_ND_SERVER_TIMER_RA",
                           addrIndex);
            break;
        default:
            break;
    }

    return;
}
/*lint +e550*/
/*
 * Discription: ����ĳһ���ص�ĳ�����͵Ķ�ʱ��
 * History:
 *      1.      2011-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_TimerStart(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    VOS_UINT32     timerLen;
    IP_TimerInfo  *timerInfo = VOS_NULL_PTR;

    /* ��ulIndex�Ϸ����ж� */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart: WARN: Input Para(ulIndex) err !", timerIndex,
                          timerType);
        return;
    }

    /* ������Ϣ��Ӧ�������źͶ�ʱ������,��ȡ������Ķ�ʱ�� */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* ��ӡ�쳣��Ϣ */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:ERROR: Get Timer failed.");
        return;
    }

    /* �ж϶�ʱ���Ƿ�򿪣��Ѵ���ر� */
    if (timerInfo->hTm != VOS_NULL_PTR) {
        /* �ر�ʧ�ܣ��򱨾����� */
        if (PS_STOP_REL_TIMER(&(timerInfo->hTm)) != VOS_OK) {
            /* ��ӡ�쳣��Ϣ */
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:WARN: stop reltimer error!");
            return;
        }

        /* ��ӡ�쳣��Ϣ */
        IPND_INFO_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:(TimerType) Timer not close!", timerInfo->timerName,
                       timerIndex);
    }

    /* ���ݶ�ʱ����ͬ���ͣ���ʱ����Ϣ��ͬ */
    timerLen = IP_NDSERVER_GetTimerLen(timerType);
    if (timerLen == IP_NULL) {
        /* ��ӡ�쳣��Ϣ */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:ERROR: start unreasonable reltimer.");
        return;
    }

    /* �趨��ʱ��NAME,enTimerType�趨��ʱ��Para����ʧ���򱨾����� */
    if (PS_START_REL_TIMER(&(timerInfo->hTm), NDIS_NDSERVER_PID, timerLen, (VOS_UINT32)timerType,
                           timerIndex, VOS_RELTIMER_NOLOOP) != VOS_OK) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:WARN: start reltimer error!");
        return;
    }

    /* ���¶�ʱ����� */
    timerInfo->timerName = timerType;

    /* ��ӡ��ʱ��������Ϣ */
    IP_NDSERVER_PrintTimeStartInfo(timerIndex, timerType);

    return;
}

/*
 * Discription: ֹͣĳһ����ĳ�����Ͷ�ʱ��
 * Return: None
 *      1.      2011-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_TimerStop(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    IP_TimerInfo *timerInfo = VOS_NULL_PTR;

    /* ��ulIndex�Ϸ����ж� */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop: WARN: Input Para(ulIndex) err !", timerIndex,
                          timerType);
        return;
    }

    /* ������Ϣ��Ӧ�������źͶ�ʱ������,��ȡ������Ķ�ʱ�� */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* ��ӡ�쳣��Ϣ */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop:ERROR:Get Timer failed.");
        return;
    }

    /* ��ʱ�����ڴ�״̬����رգ����򣬺��� */
    if (timerInfo->hTm != VOS_NULL_PTR) {
        if (timerType != timerInfo->timerName) {
            /* ��ӡ�쳣��Ϣ */
            IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop: TimerType not match:", timerType,
                              timerInfo->timerName);
        }
        /* �ر�ʧ�ܣ��򱨾����� */
        if (PS_STOP_REL_TIMER(&(timerInfo->hTm)) != VOS_OK) {
            /* ��ӡ�쳣��Ϣ */
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop:WARN: stop reltimer error!");
            return;
        }

        /* �����ʱ���� */
        timerInfo->loopTimes = 0;

        /* ��ӡ��ʱ���ر���Ϣ */
        IP_NDSERVER_PrintTimeStopInfo(timerIndex, timerType);
    }

    return;
}

/*
 * Description: ND SERVERģ��ĳ�ʼ��
 * History: VOS_VOID
 *    1.        2011-04-01  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_Init(VOS_VOID)
{
    VOS_UINT32 addrIndex;
    VOS_UINT16 payLoad = IP_IPV6_PAYLOAD;
    errno_t    rlt;

    IP_NDSERVER_SetLocalParam();
    IP_NDSERVER_DebugInit();

    for (addrIndex = 0; addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM; addrIndex++) {
        (VOS_VOID)memset_s(IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex), sizeof(IP_NdserverAddrInfo), IP_NULL,
                           sizeof(IP_NdserverAddrInfo));

        rlt = memcpy_s(IP_NDSERVER_ADDRINFO_GET_MACADDR(addrIndex), IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(),
                       IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        rlt = memcpy_s((IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex) + IP_MAC_ADDR_LEN),
                       (IP_ETH_MAC_HEADER_LEN - IP_MAC_ADDR_LEN), (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        /* payload����Ϊ2���ֽ� */
        rlt = memcpy_s((IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex) + 2 * IP_MAC_ADDR_LEN),
                       (IP_ETH_MAC_HEADER_LEN - 2 * IP_MAC_ADDR_LEN), (VOS_UINT8 *)(&payLoad), 2); /* 2:byte */
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        /* ��������IP��������� */
        rlt = LUP_CreateQue(NDIS_NDSERVER_PID, &(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex)),
                            ND_IPV6_WAIT_ADDR_RSLT_Q_LEN);
        if (rlt != PS_SUCC) {
            PS_PRINTF_ERR("IP_NDSERVER_Init, LUP_CreateQue DlPktQue fail.\n");
            return;
        }
#endif
    }

    (VOS_VOID)memset_s(&g_ndServerTeDetectBuf, sizeof(IP_NdserverTeDetectBuf), IP_NULL,
                       sizeof(IP_NdserverTeDetectBuf));

    g_ndServerTeDetectBuf.head   = 0;
    g_ndServerTeDetectBuf.tail   = 0;
    g_ndServerTeDetectBuf.maxNum = 2 * IP_NDSERVER_ADDRINFO_MAX_NUM; /* 2:buf */

    return;
}

/*
 * ��������: ND SERVER PID��ʼ������
 * �޸���ʷ:
 *  1.��    ��: 2011��12��7��
 *    �޸�����: �����ɺ���
 */
VOS_UINT32 APP_NdServer_Pid_InitFunc(enum VOS_InitPhaseDefine phase)
{
    switch (phase) {
        case VOS_IP_LOAD_CONFIG:
            IP_NDSERVER_Init();
            break;
        case VOS_IP_FARMALLOC:
        case VOS_IP_INITIAL:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
        case VOS_IP_BUTT:
            break;
        default:
            break;
    }
    return 0;
}

/*
 * Description: ���ND SERVER�����Ϣ
 * Input: ulIndex --- ND SERVER�ṹ������
 * History:
 *    1.     2011-04-01  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_Stop(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo *infoAddr = IP_NULL_PTR;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    infoAddr->validFlag = IP_FALSE;

    infoAddr->epsbId = 0;

    (VOS_VOID)memset_s(&infoAddr->ipv6NwPara, sizeof(ESM_IP_Ipv6NWPara), IP_NULL,
                       sizeof(ESM_IP_Ipv6NWPara));

    if (infoAddr->teAddrInfo.teAddrState != IP_NDSERVER_TE_ADDR_INEXISTENT) {
        IP_NDSERVER_SaveTeDetectIp(infoAddr->teAddrInfo.teGlobalAddr);
    }

    /* ���TE��ַ��¼ */
    (VOS_VOID)memset_s(&infoAddr->teAddrInfo, sizeof(IP_NdserverTeAddrInfo), IP_NULL,
                       sizeof(IP_NdserverTeAddrInfo));

    IP_NDSERVER_TimerStop(addrIndex, infoAddr->timerInfo.timerName);

    (VOS_VOID)memset_s(&infoAddr->ipSndNwMsg, sizeof(IP_SndMsg), IP_NULL, sizeof(IP_SndMsg));

    g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;

    return;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * Description: �ͷ����л�������е�PKT
 * History: VOS_VOID
 *    1.       2011-12-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ClearDlPktQue(VOS_UINT32 addrIndex)
{
    IMM_Zc      *immZc = VOS_NULL_PTR;
    VOS_INT32    lockKey;

    while (LUP_IsQueEmpty(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex)) != PS_TRUE) {
        lockKey = VOS_SplIMP();
        if (LUP_DeQue(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), (VOS_VOID **)(&immZc)) != PS_SUCC) {
            VOS_Splx(lockKey);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ClearDlPktQue, LUP_DeQue return fail!");
            return;
        }
        VOS_Splx(lockKey);
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
    }

    return;
}
#endif

/*
 * Description: ����NDIS������Ipv6PdnRel
 * History: VOS_VOID
 *    1.       2011-12-09  Draft Enact
 *    2.       2013-1-16  DSDA
 */
VOS_VOID NdSer_Ipv6PdnRel(VOS_UINT8 exRabId)
{
    VOS_UINT32 addrIndex;

    IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnRel is entered.");

    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM) {
        IP_NDSERVER_Stop(addrIndex);
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        IP_NDSERVER_ClearDlPktQue(addrIndex); /* �ͷ�����PKT������� */
#endif
    } else {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnRel: Invalid Input:", addrIndex, exRabId);
    }

    return;
}

/*
 * Description: ��NDIS���ã���ѯ��Ӧ���ص�ND SERVERʵ���Ƿ���Ч
 * History: VOS_UINT32
 *    1.       2011-12-06  Draft Enact
 */
VOS_UINT32 NdSer_Ipv6PdnValid(VOS_UINT8 rabId)
{
    VOS_UINT32 addrIndex;

    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(rabId);
    if (addrIndex >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        return PS_FAIL;
    }

    return PS_SUCC;
}

/*
 * Description: ��NDIS���ã���ȡND SERVER��Ӧʵ�������MAC֡ͷ
 * History: VOS_UINT8
 *    1.       2011-12-06  Draft Enact
 */
VOS_UINT8 *NdSer_GetMacFrm(VOS_UINT8 addrIndex, VOS_UINT8 *teAddrState)
{
    IP_NdserverAddrInfo *infoAddr = IP_NULL_PTR;

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "pstInfoAddr ucValidFlag is not TRUE!", addrIndex);
        return VOS_NULL_PTR;
    }

    *teAddrState = infoAddr->teAddrInfo.teAddrState;

    return IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex);
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * Description: ��NDIS���ã���δ���PC MAC��ַʱ����������IP��
 * History: VOS_UINT8
 *    1.      2011-12-06  Draft Enact
 */
VOS_VOID NdSer_MacAddrInvalidProc(IMM_Zc *immZc, VOS_UINT8 addrIndex)
{
    IMM_Zc      *queHead = VOS_NULL_PTR;
    VOS_INT32    lockKey;

    IP_NDSERVER_AddMacInvalidPktNum(addrIndex);

    /* **********************����IPV6���ݰ�����******************************** */
    if (LUP_IsQueFull(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex)) == PS_TRUE) {
        lockKey = VOS_SplIMP();
        if (LUP_DeQue(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), (VOS_VOID **)(&queHead)) != PS_SUCC) {
            VOS_Splx(lockKey);
            /*lint -e522*/
            IMM_ZcFree(immZc);
            /*lint +e522*/
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_MacAddrInvalidProc, LUP_DeQue return NULL");
            return;
        }
        VOS_Splx(lockKey);
        /*lint -e522*/
        IMM_ZcFree(queHead); /* �ͷ������IP�� */
        /*lint +e522*/
    }

    lockKey = VOS_SplIMP();
    if (LUP_EnQue(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), immZc) != PS_SUCC) { /* �������µ�IP�� */
        VOS_Splx(lockKey);
        /*lint -e522*/
        IMM_ZcFree(immZc);
        /*lint +e522*/
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_MacAddrInvalidProc, LUP_EnQue return NULL");
        return;
    }
    VOS_Splx(lockKey);

    IP_NDSERVER_AddEnquePktNum(addrIndex);

    return;
}
#endif
/*
 * Description: ���ҵ�һ�����õ�IPv6ǰ׺
 * Input: pstConfigParaInd ---- ESM_IP_NW_PARA_IND_STRU��Ϣָ��
 * Output: pulPrefixIndex   ---- ǰ׺����ָ��
 * History:
 *      1.     2011-04-05  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_FindIpv6EffectivePrefix(const AT_NDIS_Ipv6PdnInfo *configParaInd,
                                                      VOS_UINT32 *prefixIndex)
{
    VOS_UINT32              loopIndex;
    ND_IP_Ipv6Prefix       *ipv6Prefix = IP_NULL_PTR;

    /* ����ǰ׺�б�����A��ʶΪ1��ǰ׺����Ϊ64��ǰ׺,prefixLen��λ��bit */
    for (loopIndex = IP_NULL; loopIndex < configParaInd->prefixNum; loopIndex++) {
        ipv6Prefix = (ND_IP_Ipv6Prefix *)&configParaInd->prefixList[loopIndex];
        if ((ipv6Prefix->prefixAValue == IP_IPV6_OP_TRUE) &&
            (ipv6Prefix->prefixLen == (ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH) * 8) && /* 8:bit */
            (!IP_IPV6_64BITPREFIX_EQUAL_ZERO(ipv6Prefix->prefix)) &&
            (!IP_IPV6_IS_LINKLOCAL_ADDR(ipv6Prefix->prefix)) &&
            (!IP_IPV6_IS_MULTICAST_ADDR(ipv6Prefix->prefix))) {
            *prefixIndex = loopIndex;
            return IP_SUCC;
        }
    }

    return IP_FAIL;
}

/*
 * Function Name: IP_NDSERVER_GetNwPara
 * Description: ��ID_ESM_IP_NW_PARA_IND�л�ȡ�������
 * Input: pstNwParaInd--------------��Ϣָ��
 * Output: pstNwParamTmp-------------�������ָ��
 * History: VOS_UINT32
 *    1.      2011-04-05  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_GetNwPara(ESM_IP_Ipv6NWPara *nwParamTmp,
                                        const AT_NDIS_Ipv6PdnInfo *nwParaInd)
{
    VOS_UINT32 prefixIndex;
    VOS_UINT32 rslt;
    errno_t    rlt;
    (VOS_VOID)memset_s(nwParamTmp, sizeof(ESM_IP_Ipv6NWPara), IP_NULL, sizeof(ESM_IP_Ipv6NWPara));

    /* ��ȡHOP LIMIT */
    nwParamTmp->curHopLimit = nwParaInd->curHopLimit;

    /* ��ȡMTU */
    if (nwParaInd->OpMtu == IP_IPV6_OP_TRUE) {
        nwParamTmp->opMtu = nwParaInd->OpMtu;
        nwParamTmp->mtuValue      = nwParaInd->Mtu;
    }

    /* ��ȡ�ӿڱ�ʶ�� */
    rlt = memcpy_s(nwParamTmp->interfaceId, ND_IP_IPV6_IFID_LENGTH, nwParaInd->interfaceId,
                   ND_IP_IPV6_IFID_LENGTH);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* �����ַǰ׺�б�Ŀǰֻ�����ʹ�õ�һ�����õ�IPv6ǰ׺ */
    rslt = IP_NDSERVER_FindIpv6EffectivePrefix(nwParaInd, &prefixIndex);
    if (rslt != IP_SUCC) {
        return IP_FAIL;
    }

    rlt = memcpy_s(&nwParamTmp->prefixList[IP_NULL], sizeof(ND_IP_Ipv6Prefix),
                   &nwParaInd->prefixList[prefixIndex], sizeof(ND_IP_Ipv6Prefix));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    nwParamTmp->prefixNum = 1;

    /* ��ȡDNS�������б� */
    rlt = memcpy_s(nwParamTmp->dnsSer.priDnsServer, IP_IPV6_ADDR_LEN, nwParaInd->dnsSer.priServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(nwParamTmp->dnsSer.secDnsServer, IP_IPV6_ADDR_LEN, nwParaInd->dnsSer.secServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    nwParamTmp->dnsSer.dnsSerNum = nwParaInd->dnsSer.serNum;

    /* ��ȡSIP�������б� */
    rlt = memcpy_s(nwParamTmp->sipSer.priSipServer, IP_IPV6_ADDR_LEN, nwParaInd->pcscfSer.priServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(nwParamTmp->sipSer.secSipServer, IP_IPV6_ADDR_LEN, nwParaInd->pcscfSer.secServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    nwParamTmp->sipSer.sipSerNum = nwParaInd->pcscfSer.serNum;

    return IP_SUCC;
}

/*
 * Description: RA����ͷ�̶���������
 * Input: ulIndex --- ND SERVERʵ������
 * Output: pucData --- ���Ļ���ָ��
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormRaHeaderMsg(VOS_UINT32 addrIndex, VOS_UINT8 *data)
{
    ESM_IP_Ipv6NWPara *nwPara = IP_NULL_PTR;

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    nwPara = IP_NDSERVER_ADDRINFO_GET_NWPARA(addrIndex);

    /* ���� */
    *data = IP_ICMPV6_TYPE_RA;
    data++;

    /* ���� */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:����У��� */
    data += 2;

    /* ��ǰ������ */
    *data = nwPara->curHopLimit;
    data++;

    /* 7 6:�����ַ���ñ�־��������״̬���ñ�־ */
    *data = (VOS_UINT8)(((g_ndServerMFlag & 0x1) << 7) | ((g_ndServerOFlag & 0x1) << 6));
    data++;

    /* ·�������� */
    IP_SetUint16Data(data, g_routerLifetime);
    data += 2; /* ·������������Ҫռ2���ֽ� */

    /* �ɴ�ʱ�� */
    IP_SetUint32Data(data, g_reachableTime);
    data += 4; /* �ɴ�ʱ����Ҫռ4���ֽ� */

    /* �ط���ʱ�� */
    IP_SetUint32Data(data, g_retransTimer);

    return;
}

/*
 * Function Name: IP_NDSERVER_FormRaOptMsg
 * Description: RA����ѡ�������
 * Input: ulIndex --- ND SERVERʵ������
 *                   pucMacAddr- Դ��·���ַ
 * Output: pucData --- ���Ļ���ָ��
 *                   pulLen ---- ���ĳ���ָ��
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormRaOptMsg(VOS_UINT32 addrIndex, const ND_MacAddrInfo macAddr, VOS_UINT8 *data,
                                         VOS_UINT32 *len)
{
    ESM_IP_Ipv6NWPara        *nwPara = IP_NULL_PTR;
    VOS_UINT32                count;
    VOS_UINT32                tmpMtu;
    VOS_INT32                 rlt;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    nwPara = IP_NDSERVER_ADDRINFO_GET_NWPARA(addrIndex);

    /* ���� */
    *data = IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR;
    data++;

    /* ���� */
    *data = 1;
    data++;

    /* ��·���ַ */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    (*len) += IP_ND_OPT_UNIT_LEN;

    if (nwPara->opMtu == IP_IPV6_OP_TRUE) {
        /* ���� */
        *data = IP_ICMPV6_OPT_MTU;
        data++;

        /* ���� */
        *data = 1;
        data++;

        /* 2:���� */
        data += 2;

        /* MTU: ȡNV�е�MTU��RA�е�MTU����Сֵ��Ϊ����PC��MTU */
        tmpMtu = PS_MIN(g_nvMtu, nwPara->mtuValue);
        IP_SetUint32Data(data, tmpMtu);
        data += 4; /* MTU����ռ4���ֽ� */

        (*len) += IP_ND_OPT_UNIT_LEN;
    }

    for (count = 0; count < nwPara->prefixNum; count++) {
        /* ���� */
        *data = IP_ICMPV6_OPT_PREFIX_INFO;
        data++;

        /* 4:���� */
        *data = 4;
        data++;

        /* ǰ׺���� */
        *data = (VOS_UINT8)(nwPara->prefixList[count].prefixLen);
        data++;

        /* ��·�ϱ�־�����α�־ */
        *data = (VOS_UINT8)(((nwPara->prefixList[count].prefixLValue & 0x1) << 7) | /* 7:bit */
                            ((nwPara->prefixList[count].prefixAValue & 0x1) << 6)); /* 6:bit */
        data++;

        /* ��Ч������ */
        IP_SetUint32Data(data, nwPara->prefixList[count].validLifeTime);
        data += 4; /* ��Ч������ռ4���ֽ� */

        /* ѡ�������� */
        IP_SetUint32Data(data, nwPara->prefixList[count].preferredLifeTime);
        data += 4; /* ѡ��������ռ4���ֽ� */

        /* 4:�����ֶ� */
        data += 4;

        /* ǰ׺ */
        rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, nwPara->prefixList[count].prefix, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
        data += IP_IPV6_ADDR_LEN;

        (*len) += (4 * IP_ND_OPT_UNIT_LEN); /* 4:len */
    }

    return;
}

/*
 * Function Name: IP_NDSERVER_FormEtherHeaderMsg
 * Description: IPv6��ͷ����
 * Input: aucSrcMacAddr --- ԴMAC��ַ
 *                   aucDstMacAddr --- Ŀ��MAC��ַ
 * Output: pucData --- ���Ļ���ָ��
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormEtherHeaderMsg(const ND_MacAddrInfo srcMacAddr, const ND_MacAddrInfo dstMacAddr,
                                               VOS_UINT8 *data)
{
    VOS_INT32 rlt;
    /* Ŀ��MAC */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, dstMacAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    /* ԴMAC */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, srcMacAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    /* ���� */
    IP_SetUint16Data(data, (VOS_UINT16)IP_GMAC_PAYLOAD_TYPE_IPV6);

    return;
}
VOS_VOID IP_NDSERVER_InitAddrInfo(ND_Ipv6AddrInfo *srcIPAddr, ND_MacAddrInfo *srcMacAddr, ND_Ipv6AddrInfo *dstIPAddr,
                                  ND_MacAddrInfo  *dstMacAddr)
{
    (VOS_VOID)memset_s(srcIPAddr, sizeof(ND_Ipv6AddrInfo), IP_NULL, sizeof(ND_Ipv6AddrInfo));
    (VOS_VOID)memset_s(dstIPAddr, sizeof(ND_Ipv6AddrInfo), IP_NULL, sizeof(ND_Ipv6AddrInfo));
    (VOS_VOID)memset_s(srcMacAddr, sizeof(ND_MacAddrInfo), IP_NULL, sizeof(ND_MacAddrInfo));
    (VOS_VOID)memset_s(dstMacAddr, sizeof(ND_MacAddrInfo), IP_NULL, sizeof(ND_MacAddrInfo));
}

/*
 * Function Name: IP_NDSERVER_FormRaMsg
 * Description: ����·�ɹ�����Ϣ
 * Input: ulIndex ------- ����RA����ʵ������
 *                   pstNdMsgData -- Ŀ����Ϣ����
 * Output: pucSendBuff --- ����RA���Ļ���
 *                   pulSendLen ---- ���ͱ��ĳ���
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormRaMsg(VOS_UINT32 addrIndex, const IP_ND_Msg *ndMsgData, VOS_UINT8 *sendBuff,
                                      VOS_UINT32 *sendLen)
{
    VOS_UINT32                  packetLen                    = IP_NULL;
    VOS_UINT8                  *data                        = sendBuff;
    ND_Ipv6AddrInfo             srcIPAddr;
    ND_MacAddrInfo              srcMacAddr;
    ND_Ipv6AddrInfo             dstIPAddr;
    ND_MacAddrInfo              dstMacAddr;
    IP_NdserverAddrInfo *infoAddr                    = IP_NULL_PTR;
    VOS_INT32                   rlt;
    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg is entered.");
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);
    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    data += IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ����RA��ͷ */
    IP_NDSERVER_FormRaHeaderMsg(addrIndex, data);

    packetLen += IP_ICMPV6_RA_HEADER_LEN;

    /* ��������MAC��ַ����link-local��ַ */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);
    rlt = memcpy_s(infoAddr->ueLinkLocalAddr, IP_IPV6_ADDR_LEN, srcIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* ����RA����ѡ�� */
    IP_NDSERVER_FormRaOptMsg(addrIndex, srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    /* ȷ���������鲥��ʽ */
    if ((ndMsgData != VOS_NULL_PTR) && (IP_IPV6_IS_LINKLOCAL_ADDR(ndMsgData->srcIp)) &&
        (ndMsgData->ndMsgStru.rsInfo.opSrcLinkLayerAddr == 1)) {
        rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ndMsgData->srcIp, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        rlt = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, ndMsgData->ndMsgStru.rsInfo.srcLinkLayerAddr,
                       IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    } else {
        rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, g_ndAllNodesMulticaseAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        rlt = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, g_ndAllNodesMacAddr, IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    }

    data -= IP_IPV6_HEAD_LEN;

    /* ����IPv6��ͷ */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* ����ICMPv6��ͷУ��� */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* ������̫��ͷ */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* ���ر��ĳ��� */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendRaMsg
 * Description: ����·�ɹ�����Ϣ
 * Input: ulIndex ------- ����RA����ʵ������
 *                   pstNdMsgData -- Ŀ����Ϣ����
 * History: VOS_UINT32
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_SendRaMsg(VOS_UINT32 addrIndex, const IP_ND_Msg *ndMsgData)
{
    VOS_UINT8 *sendBuff = VOS_NULL_PTR;
    VOS_UINT32 sendLen   = IP_NULL;
    VOS_UINT32 epsbId;

    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, PS_FAIL);
    epsbId = IP_NDSERVER_GET_EPSBID(addrIndex);

    sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);

    /* �����γ�RA��Ϣ���� */
    IP_NDSERVER_FormRaMsg(addrIndex, ndMsgData, sendBuff, &sendLen);

    IP_NDSERVER_AddTransRaPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    /* ��RA��Ϣ���͵�PC */
    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * Function Name: IP_NDSERVER_FormNaHeaderMsg
 * Description: NA����ͷ�̶���������
 * Input: pucTargetIPAddr --- Ŀ��IP��ַ
 *                   ucSolicitFlag   --- �����־
 * Output: pucData --- ���Ļ���ָ��
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNaHeaderMsg(const ND_Ipv6AddrInfo targetIPAddr, VOS_UINT8 solicitFlag, VOS_UINT8 *data)
{
    VOS_INT32 rlt;
    /* ���� */
    *data = IP_ICMPV6_TYPE_NA;
    data++;

    /* ���� */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:����У��� */
    data += 2;

    /* 6:·������־�������־�����Ǳ�־ */
    *data = (VOS_UINT8)(0xa0 | ((solicitFlag & 0x1) << 6));
    data++;

    /* 3:���� */
    data += 3;

    /* Ŀ���ַ */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, targetIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    return;
}

/*
 * Description: NA����ѡ�������
 * Input: pucMacAddr- Դ��·���ַ
 * Output: pucData --- ���Ļ���ָ��
 *                   pulLen ---- ���ĳ���ָ��
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNaOptMsg(const ND_MacAddrInfo macAddr, VOS_UINT8 *data, VOS_UINT32 *len)
{
    VOS_INT32 rlt;
    /* ���� */
    *data = IP_ICMPV6_OPT_TGT_LINK_LAYER_ADDR;
    data++;

    /* ���� */
    *data = 1;
    data++;

    /* ��·���ַ */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    (*len) += IP_ND_OPT_UNIT_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNaMsg
 * Description: �����ھӹ�����Ϣ
 * Input: ulIndex ------- ����NA����ʵ������
 *                   pstNdMsgData -- Ŀ����Ϣ����
 * Output: pucSendBuff --- ����NA���Ļ���
 *                   pulSendLen ---- ���ͱ��ĳ���
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNaMsg(const IP_ND_Msg *ndMsgData, VOS_UINT8 *sendBuff, VOS_UINT32 *sendLen)
{
    VOS_UINT32 packetLen                   = IP_NULL;
    VOS_UINT8 *data                        = sendBuff;
    ND_Ipv6AddrInfo  srcIPAddr;
    ND_MacAddrInfo   srcMacAddr;
    ND_Ipv6AddrInfo  dstIPAddr;
    ND_MacAddrInfo   dstMacAddr;
    VOS_UINT8  solicitFlag                  = IP_NULL;
    VOS_INT32  rlt;
    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNaMsg is entered.");
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);

    /* ��������MAC��ַ��ΪԴMAC */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    if (ndMsgData == VOS_NULL_PTR) {
        /* ��������MAC��ַ����link-local��ַ */
        IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
        IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);
    } else {
        /* ʹ��Ŀ���ַ��ΪԴIP */
        rlt = memcpy_s(srcIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ndMsgData->ndMsgStru.nsInfo.targetAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    }

    /* ȷ���������鲥��ʽ */
    if ((ndMsgData != VOS_NULL_PTR) && (!IP_IPV6_EQUAL_ALL_ZERO(ndMsgData->srcIp)) &&
        (!IP_IPV6_IS_MULTICAST_ADDR(ndMsgData->srcIp)) &&
        (ndMsgData->ndMsgStru.nsInfo.opSrcLinkLayerAddr == 1)) {
        rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ndMsgData->srcIp, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        rlt = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, ndMsgData->ndMsgStru.nsInfo.srcLinkLayerAddr,
                       IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        solicitFlag = 1;
    } else {
        rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, g_ndAllNodesMulticaseAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        rlt = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, g_ndAllNodesMacAddr, IP_MAC_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    }

    data += IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ����NA��ͷ */
    IP_NDSERVER_FormNaHeaderMsg(srcIPAddr, solicitFlag, data);

    packetLen += IP_ICMPV6_NA_HEADER_LEN;

    /* ����NA��ѡ�� */
    IP_NDSERVER_FormNaOptMsg(srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    data -= IP_IPV6_HEAD_LEN;

    /* ����IPv6��ͷ */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* ����ICMPv6У��� */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNaMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* ������̫��ͷ */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* ���ر��ĳ��� */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendNaMsg
 * Description: �����ھӹ�����Ϣ
 * Input: ulIndex ------- ����NA����ʵ������
 *                   pstNdMsgData -- Ŀ����Ϣ����
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_SendNaMsg(VOS_UINT32 addrIndex, const IP_ND_Msg *ndMsgData)
{
    VOS_UINT8 *sendBuff = VOS_NULL_PTR;
    VOS_UINT32 sendLen   = IP_NULL;
    VOS_UINT32 epsbId;
    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, PS_FAIL);
    epsbId = IP_NDSERVER_GET_EPSBID(addrIndex);

    sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);

    /* �����γ�NA��Ϣ���� */
    IP_NDSERVER_FormNaMsg(ndMsgData, sendBuff, &sendLen);

    IP_NDSERVER_AddTransNaPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * Function Name: IP_NDSERVER_FormNsHeaderMsg
 * Description: NS����ͷ�̶���������
 * Input: ulIndex --- ND SERVERʵ������
 * Output: pucData --- ���Ļ���ָ��
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNsHeaderMsg(VOS_UINT32 addrIndex, VOS_UINT8 *data, const VOS_UINT8 *dstAddr)
{
    VOS_INT32 rlt;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    /* ���� */
    *data = IP_ICMPV6_TYPE_NS;
    data++;

    /* ���� */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:����У��� */
    data += 2;

    /* 4:���� */
    data += 4;

    /* Ŀ���ַ */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, dstAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNsOptMsg
 * Description: NS����ѡ�������
 * Input: pucMacAddr- Դ��·���ַ
 * Output: pucData --- ���Ļ���ָ��
 *                   pulLen ---- ���ĳ���ָ��
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNsOptMsg(const ND_MacAddrInfo macAddr, VOS_UINT8 *data, VOS_UINT32 *len)
{
    VOS_INT32 rlt;
    /* ���� */
    *data = IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR;
    data++;

    /* ���� */
    *data = 1;
    data++;

    /* ��·���ַ */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    (*len) += IP_ND_OPT_UNIT_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNsMsg
 * Description: ���ɵ�ַ�����ھ�������Ϣ
 * Input: ulIndex ------- ����NS����ʵ������
 * Output: pucSendBuff --- ����NS���Ļ���
 *                   pulSendLen ---- ���ͱ��ĳ���
 * History: VOS_VOID
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNsMsg(VOS_UINT32 addrIndex, VOS_UINT8 *sendBuff, VOS_UINT32 *sendLen,
                                      const VOS_UINT8 *dstAddr)
{
    VOS_UINT32 packetLen                    = IP_NULL;
    VOS_UINT8 *data                        = sendBuff;
    ND_Ipv6AddrInfo  srcIPAddr;
    ND_MacAddrInfo   srcMacAddr;
    ND_Ipv6AddrInfo  dstIPAddr;
    ND_MacAddrInfo   dstMacAddr;
    VOS_INT32  rlt;
    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNsMsg is entered.");

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);

    /* ��������MAC��ַ����link-local��ַ */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* ����Ŀ��IP��������ڵ��鲥��ַ */
    IP_ProduceSolicitedNodeMulticastIPAddr(dstIPAddr.ipv6Addr, dstAddr);
    IP_ProduceSolicitedNodeMulticastMacAddr(dstMacAddr.macAddr, dstAddr);

    data += IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ����NS��ͷ */
    IP_NDSERVER_FormNsHeaderMsg(addrIndex, data, dstAddr);

    packetLen += IP_ICMPV6_NS_HEADER_LEN;

    /* ����NS��ѡ�� */
    IP_NDSERVER_FormNsOptMsg(srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    data -= IP_IPV6_HEAD_LEN;

    /* ����IPv6��ͷ */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* ����ICMPv6У��� */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNsMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* ������̫��ͷ */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* ���ر��ĳ��� */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendNsMsg
 * Description: ���͵�ַ�����ھ�������Ϣ
 * Input: ulIndex ------- ����NS����ʵ������
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_SendNsMsg(VOS_UINT32 addrIndex, const VOS_UINT8 *dstAddr)
{
    VOS_UINT8 *sendBuff = VOS_NULL_PTR;
    VOS_UINT32 sendLen   = IP_NULL;
    VOS_UINT32 epsbId;
    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, PS_FAIL);
    epsbId = IP_NDSERVER_GET_EPSBID(addrIndex);

    sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);

    /* �����γ�NS��Ϣ���� */
    IP_NDSERVER_FormNsMsg(addrIndex, sendBuff, &sendLen, dstAddr);

    IP_NDSERVER_AddTransNsPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * ��������: ����ExRabId����NDSERVERʵ�������
 * �޸���ʷ:
 *  1.��    ��: 2013��1��15��
 *    �޸�����: �����ɺ���
 */
VOS_UINT32 NdSer_GetAddrInfoIdx(VOS_UINT8 exRabId)
{
    VOS_UINT32 i = 0;

    /* ��ѯ�Ƿ��Ѵ�����ӦEntity */
    do {
        if ((PS_TRUE == g_ndServerAddrInfo[i].validFlag) && (exRabId == g_ndServerAddrInfo[i].epsbId)) {
            /* �ҵ���Ӧʵ�� */
            return i;
        }
    } while ((++i) < IP_NDSERVER_ADDRINFO_MAX_NUM);

    return IP_NDSERVER_ADDRINFO_MAX_NUM;
}

/*
 * ��������: ����һ�����е�NDSERVERʵ��
 * �޸���ʷ:
 *  1.��    ��: 2013��1��15��
 *    �޸�����: �����ɺ���
 */
IP_NdserverAddrInfo *NdSer_AllocAddrInfo(VOS_UINT32 *pUlIndex)
{
    VOS_UINT32 i = 0;

    if (pUlIndex == IP_NULL_PTR) {
        return IP_NULL_PTR;
    }

    *pUlIndex = IP_NDSERVER_ADDRINFO_MAX_NUM;

    do {
        if (PS_FALSE == g_ndServerAddrInfo[i].validFlag) {
            /* �ҵ�����ʵ�� */
            *pUlIndex = i;
            return &g_ndServerAddrInfo[i];
        }
    } while ((++i) < IP_NDSERVER_ADDRINFO_MAX_NUM);

    return IP_NULL_PTR;
}

/*
 * ��������: PDN IPV6��ַ��Ϣ���
 * �޸���ʷ:
 *  1.��    ��: 2011��12��11��
 *    �޸�����: �����ɺ���
 *  2.��    ��: 2013��1��15��
 *    �޸�����: DSDA������ʹ��ExRabId��ӳ��NdSerʵ��
 */
VOS_UINT32 NdSer_CheckIpv6PdnInfo(const AT_NDIS_Ipv6PdnInfo *ipv6PdnInfo)
{
    VOS_UINT32        prefixIndex;
    ND_IP_Ipv6Prefix *ipv6Prefix = IP_NULL_PTR;

    if (ipv6PdnInfo->prefixNum >= AT_NDIS_MAX_PREFIX_NUM_IN_RA) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_CheckIpv6PdnInfo: Invalid IPV6 PrefixNum!",
                        ipv6PdnInfo->prefixNum);
        return IP_FAIL;
    }

    /* ����ǰ׺�б�����A��ʶΪ1��ǰ׺����Ϊ64��ǰ׺��prefixLen��λΪbit */
    for (prefixIndex = IP_NULL; prefixIndex < ipv6PdnInfo->prefixNum; prefixIndex++) {
        ipv6Prefix = (ND_IP_Ipv6Prefix *)&ipv6PdnInfo->prefixList[prefixIndex];
        if ((ipv6Prefix->prefixAValue == PS_TRUE) &&
            ((ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH) * 8 == ipv6Prefix->prefixLen) && /* 8:bit */
            (!IP_IPV6_64BITPREFIX_EQUAL_ZERO(ipv6Prefix->prefix)) &&
            (!IP_IPV6_IS_LINKLOCAL_ADDR(ipv6Prefix->prefix)) &&
            (!IP_IPV6_IS_MULTICAST_ADDR(ipv6Prefix->prefix))) {
            return PS_SUCC;
        }
    }

    return PS_FAIL;
}
STATIC VOS_UINT32 NdSer_RoutePrefixConfirm(ESM_IP_Ipv6NWPara *nwParamTmp, IP_NdserverAddrInfo *infoAddr,
                                           VOS_UINT32 timerIndex)
{
    VOS_UINT8 *ipV6;
    VOS_INT32  rlt;
    ipV6 = IP_NDSERVER_GetTeDetectIp(nwParamTmp->prefixList[0].prefix);
    /* MT�ػ��ٿ�����ND SERVER�յ�����RAǰ���������յ�TE���ظ���ַ���NS */
    if (ipV6 != IP_NULL_PTR) {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Using saved TeAddr to get MAC.");
        rlt = memcpy_s(infoAddr->teAddrInfo.teGlobalAddr, IP_IPV6_ADDR_LEN, ipV6,
                       IP_IPV6_ADDR_LEN);
        if (rlt != EOK) {
            return IP_FAIL;
        }
        infoAddr->teAddrInfo.teAddrState = IP_NDSERVER_TE_ADDR_INCOMPLETE;
        /* �����յ��ظ���ַ����ĵȴ���ʱ�� */
        IP_NDSERVER_TimerStart(timerIndex, IP_ND_SERVER_TIMER_FIRST_NS);
    } else {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: None saved TeAddr.");

        /* ����·�ɹ��涨ʱ�� */
        IP_NDSERVER_TimerStart(timerIndex, IP_ND_SERVER_TIMER_RA);
    }
    return IP_SUCC;
}
/*
 * Description: ����NDIS������Ipv6PdnInfoCfg
 */
VOS_VOID NdSer_Ipv6PdnInfoCfg(VOS_UINT8 exRabId, const AT_NDIS_Ipv6PdnInfo *ipv6PdnInfo)
{
    VOS_UINT32                  addrIndex;
    IP_NdserverAddrInfo        *infoAddr  = IP_NULL_PTR;
    ESM_IP_Ipv6NWPara           nwParamTmp;
    VOS_INT32                   rlt;
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcEsmIpNwParaInd is entered.");

    /* ͨ����չRabId����NDSERVERʵ�壬������Ҳ����������һ�����е� */
    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM) {
        infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    } else {
        /* ����һ��AddrInfo�ṹ�壬���һ��Index */
        infoAddr = NdSer_AllocAddrInfo(&addrIndex);
    }

    if (infoAddr == IP_NULL_PTR) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Get ND Server Entity failed.", exRabId);
        return;
    }

    /* ��ȡ������� */
    if (IP_NDSERVER_GetNwPara(&nwParamTmp, ipv6PdnInfo) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Get Nw Param fail.");
        return;
    }

    /* �ж��Ƿ�δ�����·��ǰ׺ */
    if (infoAddr->validFlag != IP_TRUE) {
        if (NdSer_RoutePrefixConfirm(&nwParamTmp, infoAddr, addrIndex) == IP_FAIL) {
            return;
        }
    } else if (IP_MEM_CMP(infoAddr->ipv6NwPara.prefixList[0].prefix,
                          nwParamTmp.prefixList[0].prefix,
                          (ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH)) != 0) {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Valid ND Server get new Nw Param.");

        /* ���TE��ַ��¼ */
        (VOS_VOID)memset_s(&infoAddr->teAddrInfo, sizeof(IP_NdserverTeAddrInfo), IP_NULL,
                           sizeof(IP_NdserverTeAddrInfo));

        /* ����·�ɹ��涨ʱ�� */
        IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);
    }

    /* ��stNwParamTmp�е����ݿ�����pstInfoAddr->stIpv6NwPara�� */
    rlt = memcpy_s(&infoAddr->ipv6NwPara, sizeof(ESM_IP_Ipv6NWPara), &nwParamTmp,
                   sizeof(ESM_IP_Ipv6NWPara));
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* ���ó��غ� */
    infoAddr->epsbId = exRabId;

    /* ��λND SERVER�ṹ����Ч��־ */
    infoAddr->validFlag = IP_TRUE;

    /* ����RA��Ϣ��PC */
    if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg:Send RA Msg failed.");
    }

    return;
}
STATIC VOS_UINT32 IP_NDSERVER_SetDestRsDataInfo(IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset, VOS_UINT32 payLoad,
                                                const VOS_UINT8 *ipMsg, VOS_INT32 remainLen)
{
    VOS_INT32          rlt;
    if (remainLen < 0) {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeRsData: Invalid IPV6 PayLoad::!", payLoad,
                        icmpv6HeadOffset);
        return IP_FAIL;
    }

    rlt = memcpy_s(destData->srcIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(destData->desIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    return IP_SUCC;
}
STATIC VOS_UINT32 IP_ProcRsOptByOptType(VOS_UINT32 optType, VOS_UINT32 optLen, IP_ND_MsgRs *rs, const VOS_UINT8 *ipMsg)
{
    VOS_INT32          rlt;
    switch (optType) {
        case IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR: {
            if (optLen == 1) {
                if (rs->opSrcLinkLayerAddr == 0) {
                    rlt = memcpy_s(rs->srcLinkLayerAddr, IP_MAC_ADDR_LEN, ipMsg + 2, IP_MAC_ADDR_LEN); /* 2:len */
                    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
                    rs->opSrcLinkLayerAddr = 1;
                } else {
                    IPND_ERROR_LOG(NDIS_NDSERVER_PID,
                                   "IP_ProcRsOptByOptType: Redundant Source Link-Layer Addr!");
                }
            } else {
                IPND_ERROR_LOG1(NDIS_NDSERVER_PID,
                                "IP_ProcRsOptByOptType: Invalid Source Link-Layer Addr Length:!", optLen);
            }
        } break;
        default:
            break;
    }
    return IP_SUCC;
}
/*
 * Description: ��RS�����и�ʽת��
 */
STATIC VOS_UINT32 IP_NDSERVER_DecodeRsData(const VOS_UINT8 *srcData, IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset)
{
    VOS_UINT32         payLoad;
    VOS_INT32          remainLen;
    VOS_UINT32         optType;
    VOS_UINT32         optLen;
    const VOS_UINT8         *ipMsg = srcData;
    IP_ND_MsgRs       *rs    = &destData->ndMsgStru.rsInfo;
    /* ��ȡPAYLOAD */
    IP_GetUint16Data(payLoad, ipMsg + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET);

    remainLen = (VOS_INT32)(payLoad + IP_IPV6_HEAD_LEN - (icmpv6HeadOffset + IP_ICMPV6_RS_HEADER_LEN));
    if (IP_NDSERVER_SetDestRsDataInfo(destData, icmpv6HeadOffset, payLoad, ipMsg, remainLen) == IP_FAIL) {
        return IP_FAIL;
    }

    /*lint -e679 -esym(679,*)*/
    ipMsg += icmpv6HeadOffset + IP_ICMPV6_RS_HEADER_LEN;
    /*lint -e679 +esym(679,*)*/

    while (remainLen >= IP_ND_OPT_UNIT_LEN) {
        optType = *ipMsg;
        optLen  = *(ipMsg + 1);

        if (optLen == 0) {
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeRsData: Invalid ND options length 0!");
            return IP_FAIL;
        }
        if (IP_ProcRsOptByOptType(optType, optLen, rs, ipMsg) == IP_FAIL) {
            return IP_FAIL;
        }

        remainLen -= (VOS_INT32)IP_GetNdOptionLen(optLen);
        /*lint -e679 -esym(679,*)*/
        ipMsg += IP_GetNdOptionLen(optLen);
        /*lint -e679 +esym(679,*)*/
    }

    if (remainLen != 0) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeRsData: Payload not match Package:!", remainLen);
    }

    return IP_SUCC;
}

/*
 * Function Name: IP_NDSERVER_RsMsgProc
 * Description: ����RS��Ϣ
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   pucSrcData ------------ IP���ݱ�
 *                   ulIcmpv6HeadOffset ---- ICMPv6����ͷƫ����
 * History:
 *    1.      2011-04-06  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_RsMsgProc(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT32 icmpv6HeadOffset)
{
    const VOS_UINT8                  *ipMsg     = srcData;
    IP_ND_Msg                  *ndMsgData = VOS_NULL_PTR;
    IP_NdserverAddrInfo        *infoAddr  = IP_NULL_PTR;
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_RsMsgProc is entered.");

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_AddRcvRsPktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IP_NDSERVER_AddErrRsPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_RsMsgProc: ND Server info flag is invalid!");
        return;
    }

    ndMsgData = IP_NDSERVER_GET_NDMSGDATA_ADDR();
    (VOS_VOID)memset_s(ndMsgData, sizeof(IP_ND_Msg), IP_NULL, sizeof(IP_ND_Msg));

    if (IP_NDSERVER_DecodeRsData(ipMsg, ndMsgData, icmpv6HeadOffset) != IP_SUCC) {
        IP_NDSERVER_AddErrRsPktNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcessRsMsg: Invalid IPV6 RS Msg:!");
        return;
    }

    /* ����RA��Ϣ��PC */
    if (IP_NDSERVER_SendRaMsg(addrIndex, ndMsgData) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_RsMsgProc:Send RA Msg failed.");
    }

    return;
}
STATIC VOS_UINT32 IP_NDSERVER_SetDestNsDataInfo(IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset, VOS_UINT32 payLoad,
                                                const VOS_UINT8 *ipMsg, VOS_INT32 remainLen)
{
    VOS_INT32          rlt;
    IP_ND_MsgNs       *ns    = &destData->ndMsgStru.nsInfo;
    if (remainLen < 0) {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNsData: Invalid IPV6 PayLoad::!", payLoad,
                        icmpv6HeadOffset);
        return IP_FAIL;
    }

    rlt = memcpy_s(destData->srcIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(destData->desIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(ns->targetAddr, IP_IPV6_ADDR_LEN,
                   ipMsg + icmpv6HeadOffset + IP_ICMPV6_TARGET_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    return IP_SUCC;
}
STATIC VOS_UINT32 IP_ProcNsOptByOptType(VOS_UINT32 optType, VOS_UINT32 optLen, IP_ND_MsgNs *ns, const VOS_UINT8 *ipMsg)
{
    VOS_INT32          rlt;
    switch (optType) {
        case IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR: {
            if (optLen == 1) {
                if (ns->opSrcLinkLayerAddr == 0) {
                    rlt = memcpy_s(ns->srcLinkLayerAddr, IP_MAC_ADDR_LEN, ipMsg + 2, IP_MAC_ADDR_LEN); /* 2:len */
                    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
                    ns->opSrcLinkLayerAddr = 1;
                } else {
                    IPND_ERROR_LOG(NDIS_NDSERVER_PID,
                                   "IP_ProcNsOptByOptType: Redundant Source Link-Layer Addr!");
                }
            } else {
                IPND_ERROR_LOG1(NDIS_NDSERVER_PID,
                                "IP_ProcNsOptByOptType: Invalid Source Link-Layer Addr Length:!", optLen);
            }
        } break;
        default:
          break;
    }
    return IP_SUCC;
}
/*
 * Description: ��NS�����и�ʽת��
 */
STATIC VOS_UINT32 IP_NDSERVER_DecodeNsData(const VOS_UINT8 *srcData, IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset)
{
    VOS_UINT32         payLoad;
    VOS_INT32          remainLen;
    VOS_UINT32         optType;
    VOS_UINT32         optLen;
    const VOS_UINT8         *ipMsg = srcData;
    IP_ND_MsgNs       *ns    = &destData->ndMsgStru.nsInfo;

    /* ��ȡPAYLOAD */
    IP_GetUint16Data(payLoad, ipMsg + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET);

    remainLen = (VOS_INT32)(payLoad + IP_IPV6_HEAD_LEN - (icmpv6HeadOffset + IP_ICMPV6_NS_HEADER_LEN));
    if (IP_NDSERVER_SetDestNsDataInfo(destData, icmpv6HeadOffset, payLoad, ipMsg, remainLen) == IP_FAIL) {
        return IP_FAIL;
    }

    /*lint -e679 -esym(679,*)*/
    ipMsg += icmpv6HeadOffset + IP_ICMPV6_NS_HEADER_LEN;
    /*lint -e679 +esym(679,*)*/

    while (remainLen >= IP_ND_OPT_UNIT_LEN) {
        optType = *ipMsg;
        optLen  = *(ipMsg + 1);

        if (optLen == 0) {
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNsData: Invalid ND options length 0!");
            return IP_FAIL;
        }
        if (IP_ProcNsOptByOptType(optType, optLen, ns, ipMsg) == IP_FAIL) {
            return IP_FAIL;
        }

        remainLen -= (VOS_INT32)IP_GetNdOptionLen(optLen);
        /*lint -e679 -esym(679,*)*/
        ipMsg += IP_GetNdOptionLen(optLen);
        /*lint -e679 +esym(679,*)*/
    }

    if (remainLen != 0) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNsData: Payload not match Package:!", remainLen);
    }

    return IP_SUCC;
}

/*
 * Function Name: IP_NDSERVER_IsSelfIPAddr
 * Description: �ж��Ƿ����Լ���IP��ַ
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   aucIPAddr ------------  IP��ַָ��
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_IsSelfIPAddr(VOS_UINT32 addrIndex, const VOS_UINT8 *iPAddr)
{
    VOS_UINT8                 linkLocalIPAddr[IP_IPV6_ADDR_LEN] = {IP_NULL};
    VOS_UINT8                 globalIPAddr[IP_IPV6_ADDR_LEN]    = {IP_NULL};
    VOS_UINT8                 macAddr[IP_MAC_ADDR_LEN];
    VOS_UINT8                *macIpAddr                         = &macAddr[0];
    VOS_UINT8                *ipAddr                            = IP_NULL_PTR;
    ESM_IP_Ipv6NWPara        *nwPara                            = IP_NULL_PTR;
    VOS_INT32                 rlt;
    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, IP_FALSE);

    /* ��������MAC��ַ����link-local��ַ */
    rlt = memcpy_s(macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FALSE);
    ipAddr = &linkLocalIPAddr[0];
    IP_ProduceIfaceIdFromMacAddr(ipAddr, macIpAddr);
    IP_SetUint16Data(ipAddr, IP_IPV6_LINK_LOCAL_PREFIX);

    ipAddr = &globalIPAddr[0];
    IP_ProduceIfaceIdFromMacAddr(ipAddr, macIpAddr);
    nwPara = IP_NDSERVER_ADDRINFO_GET_NWPARA(addrIndex);
    rlt = memcpy_s(globalIPAddr, IP_IPV6_ADDR_LEN, nwPara->prefixList[0].prefix,
                   ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FALSE);

    /*lint -e960*/
    if ((IP_MEM_CMP(iPAddr, linkLocalIPAddr, IP_IPV6_ADDR_LEN) == IP_NULL) ||
        (IP_MEM_CMP(iPAddr, globalIPAddr, IP_IPV6_ADDR_LEN) == IP_NULL)) {
        /*lint +e960*/
        return IP_TRUE;
    } else {
        return IP_FALSE;
    }
}

/*
 * Function Name: IP_NDSERVER_EqualAdvertisedPrefix
 * Description: �ж��Ƿ�����ѹ����ȫ��ǰ׺
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   aucIPAddr ------------  IP��ַָ��
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_EqualAdvertisedPrefix(VOS_UINT32 addrIndex, const VOS_UINT8 *iPAddr)
{
    ESM_IP_Ipv6NWPara *nwPara = IP_NULL_PTR;

    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, IP_FALSE);
    nwPara = IP_NDSERVER_ADDRINFO_GET_NWPARA(addrIndex);
    if (IP_MEM_CMP(iPAddr, nwPara->prefixList[0].prefix, ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH)
        == IP_NULL) {
        return IP_TRUE;
    } else {
        return IP_FALSE;
    }
}

/*
 * Function Name: IP_NDSERVER_RcvTeDetectionAddr
 * Description: �յ��ظ���ַ����ַ
 * Input: ulIndex  -------------- ND SERVERʵ������
 *                   aucIPAddr ------------  IP��ַָ��
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_RcvTeDetectionAddr(VOS_UINT32 addrIndex, const VOS_UINT8 *iPAddr)
{
    IP_NdserverTeAddrInfo *teInfo = IP_NULL_PTR;
    VOS_INT32                      rlt;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    /*lint -e960*/
    if ((teInfo->teAddrState == IP_NDSERVER_TE_ADDR_REACHABLE) &&
        (IP_MEM_CMP(teInfo->teGlobalAddr, iPAddr, IP_IPV6_ADDR_LEN) == IP_NULL)) {
        /*lint +e960*/
        return;
    }

    IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_RcvTeDetectionAddr: Old Te Addr:", teInfo->teAddrState);
    rlt = memcpy_s(teInfo->teGlobalAddr, IP_IPV6_ADDR_LEN, iPAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    teInfo->teAddrState = IP_NDSERVER_TE_ADDR_INCOMPLETE;

    IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_RcvTeDetectionAddr: New Te Addr:", teInfo->teAddrState);

    return;
}
STATIC VOS_VOID IP_NDSERVER_NotSelfIPAddr(VOS_UINT32 addrIndex, const IP_ND_Msg *ndMsgData)
{
    /* �ж��Ƿ����ظ���ַ���NS */
    if ((IP_IPV6_EQUAL_ALL_ZERO(ndMsgData->srcIp)) &&
        (ndMsgData->ndMsgStru.nsInfo.opSrcLinkLayerAddr == 0)) {
        if (IP_NDSERVER_EqualAdvertisedPrefix(addrIndex, ndMsgData->ndMsgStru.nsInfo.targetAddr) == IP_TRUE) {
            IP_NDSERVER_RcvTeDetectionAddr(addrIndex, ndMsgData->ndMsgStru.nsInfo.targetAddr);
            /* �����յ��ظ���ַ����ĵȴ���ʱ�� */
            IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_FIRST_NS);

            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc:Receive duplicate addr detection packet.");
            return;
        } else {
            IP_NDSERVER_AddErrNsPktNum(addrIndex);
            IPND_WARNING_LOG(NDIS_NDSERVER_PID,
                             "IP_NDSERVER_NsMsgProc:Duplicate addr detect IP don't equal Advertised Prefix, discard!");
            return;
        }
    } else if ((!IP_IPV6_EQUAL_ALL_ZERO(ndMsgData->srcIp)) &&
               (ndMsgData->ndMsgStru.nsInfo.opSrcLinkLayerAddr != 0)) {
        if (IP_NDSERVER_SendNaMsg(addrIndex, ndMsgData) != PS_SUCC) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc:Send NA Msg failed.");
        }
    } else {
        IP_NDSERVER_AddErrNsPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc:Target IP is not self addr, discard!");
        return;
    }
    return;
}


/*
 * Function Name: IP_NDSERVER_NsMsgProc
 * Description: ����NS��Ϣ
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   pucSrcData ------------ IP���ݱ�
 *                   ulIcmpv6HeadOffset ---- ICMPv6����ͷƫ����
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_NsMsgProc(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT32 icmpv6HeadOffset)
{
    const VOS_UINT8                  *ipMsg     = srcData;
    IP_ND_Msg                  *ndMsgData = VOS_NULL_PTR;
    IP_NdserverAddrInfo        *infoAddr  = IP_NULL_PTR;
    VOS_UINT8                  *ipAddrData = IP_NULL_PTR;

    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc is entered.");
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_AddRcvNsPktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    ndMsgData = IP_NDSERVER_GET_NDMSGDATA_ADDR();
    (VOS_VOID)memset_s(ndMsgData, sizeof(IP_ND_Msg), IP_NULL, sizeof(IP_ND_Msg));

    if (IP_NDSERVER_DecodeNsData(ipMsg, ndMsgData, icmpv6HeadOffset) != IP_SUCC) {
        IP_NDSERVER_AddErrNsPktNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc: Invalid IPV6 NS Msg:!");
        return;
    }

    if (infoAddr->validFlag != IP_TRUE) {
        /* MT�ػ��ٿ�����ND SERVER�յ�����RAǰ���������յ�TE���ظ���ַ���NS */
        if ((IP_IPV6_EQUAL_ALL_ZERO(ndMsgData->srcIp)) &&
            (ndMsgData->ndMsgStru.nsInfo.opSrcLinkLayerAddr == 0)) {
            IP_NDSERVER_SaveTeDetectIp(ndMsgData->ndMsgStru.nsInfo.targetAddr);
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc: receive DAD NS packet before RA.");
        } else {
            IP_NDSERVER_AddErrNsPktNum(addrIndex);
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc: ND Server info flag is invalid!");
        }
        return;
    }

    ipAddrData = &ndMsgData->ndMsgStru.nsInfo.targetAddr[0];
    if (IP_NDSERVER_IsSelfIPAddr(addrIndex, ipAddrData) == IP_TRUE) {
        /* ����NA��Ϣ��PC */
        if (IP_NDSERVER_SendNaMsg(addrIndex, ndMsgData) != PS_SUCC) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NsMsgProc:Send NA Msg failed.");
        }
    } else {
        IP_NDSERVER_NotSelfIPAddr(addrIndex, ndMsgData);
    }

    return;
}
STATIC VOS_UINT32 IP_NDSERVER_FillDestDataInfo(VOS_INT32 *remainLen, const VOS_UINT8 *ipMsg,
                                               VOS_UINT32 icmpv6HeadOffset, IP_ND_Msg *destData, IP_ND_MsgNa *na)
{
    VOS_UINT32         payLoad;
    VOS_INT32          rlt;

    /* ��ȡPAYLOAD */
    IP_GetUint16Data(payLoad, ipMsg + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET);
    *remainLen = (VOS_INT32)(payLoad + IP_IPV6_HEAD_LEN - (icmpv6HeadOffset + IP_ICMPV6_NA_HEADER_LEN));

    if ((*remainLen) < 0) {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNaData: Invalid IPV6 PayLoad::!", payLoad,
                        icmpv6HeadOffset);
        return IP_FAIL;
    }

    rlt = memcpy_s(destData->srcIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(destData->desIp, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    na->naRValue = ((*(ipMsg + icmpv6HeadOffset + IP_ICMPV6_NA_FLAG_OFFSET)) >> 7) & 0x01; /* 7:bit */
    na->naSValue = ((*(ipMsg + icmpv6HeadOffset + IP_ICMPV6_NA_FLAG_OFFSET)) >> 6) & 0x01; /* 6:bit */
    na->naOValue = ((*(ipMsg + icmpv6HeadOffset + IP_ICMPV6_NA_FLAG_OFFSET)) >> 5) & 0x01; /* 5:bit */

    rlt = memcpy_s(na->targetAddr, IP_IPV6_ADDR_LEN,
                   ipMsg + icmpv6HeadOffset + IP_ICMPV6_TARGET_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    return IP_SUCC;
}
STATIC VOS_UINT32 IP_ProcNaOptByOptType(VOS_UINT32 optType, VOS_UINT32 optLen, IP_ND_MsgNa *na, const VOS_UINT8 *ipMsg)
{
    VOS_INT32          rlt;
    switch (optType) {
        case IP_ICMPV6_OPT_TGT_LINK_LAYER_ADDR: {
            if (optLen == 1) {
                if (na->opTargetLinkLayerAddr == 0) {
                    rlt = memcpy_s(na->targetLinkLayerAddr, IP_MAC_ADDR_LEN, ipMsg + 2, IP_MAC_ADDR_LEN); /* 2:len */
                    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
                    na->opTargetLinkLayerAddr = 1;
                } else {
                    IPND_ERROR_LOG(NDIS_NDSERVER_PID,
                                   "IP_GetNaTargetLinkLayerAddr: Redundant Target Link-Layer Addr!");
                }
            } else {
                IPND_ERROR_LOG1(NDIS_NDSERVER_PID,
                                "IP_GetNaTargetLinkLayerAddr: Invalid Target Link-Layer Addr Length:!", optLen);
            }
        } break;
        default:
            break;
    }
    return IP_SUCC;
}
/*
 * Description: ��NA�����и�ʽת��
 */
STATIC VOS_UINT32 IP_NDSERVER_DecodeNaData(const VOS_UINT8 *srcData, IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset)
{
    VOS_INT32          remainLen = 0;
    VOS_UINT32         optType;
    VOS_UINT32         optLen;
    const VOS_UINT8         *ipMsg = srcData;
    IP_ND_MsgNa       *na    = &destData->ndMsgStru.naInfo;

    if (IP_NDSERVER_FillDestDataInfo(&remainLen, ipMsg, icmpv6HeadOffset, destData, na) == IP_FAIL) {
        return IP_FAIL;
    }

    /*lint -e679 -esym(679,*)*/
    ipMsg += icmpv6HeadOffset + IP_ICMPV6_NA_HEADER_LEN;
    /*lint -e679 +esym(679,*)*/

    while (remainLen >= IP_ND_OPT_UNIT_LEN) {
        optType = *ipMsg;
        optLen  = *(ipMsg + 1);

        if (optLen == 0) {
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNaData: Invalid ND options length 0!");
            return IP_FAIL;
        }

        if (IP_ProcNaOptByOptType(optType, optLen, na, ipMsg) == IP_FAIL) {
            return IP_FAIL;
        }

        remainLen -= (VOS_INT32)IP_GetNdOptionLen(optLen);
        /*lint -e679 -esym(679,*)*/
        ipMsg += IP_GetNdOptionLen(optLen);
        /*lint -e679 +esym(679,*)*/
    }

    if (remainLen != 0) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_DecodeNaData: Payload not match Package:!", remainLen);
    }

    return IP_SUCC;
}

/*
 * Function Name: IP_NDSERVER_EqualSavedTeAddr
 * Description: �ж��Ƿ����ѱ����TE��ַ
 * Input: ulIndex  -------------  ND SERVERʵ������
 *                   aucIPAddr ------------  IP��ַָ��
 *                   aucMACAddr -----------  MAC��ַָ��
 * History:
 *    1.      2011-04-09  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_EqualSavedTeAddr(VOS_UINT32 addrIndex, const VOS_UINT8 *iPAddr, const VOS_UINT8 *mACAddr)
{
    IP_NdserverTeAddrInfo *teInfo = IP_NULL_PTR;

    IP_ASSERT_RTN(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM, IP_FALSE);
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    /*lint -e960*/
    if ((teInfo->teAddrState == IP_NDSERVER_TE_ADDR_REACHABLE) &&
        (IP_MEM_CMP(teInfo->teGlobalAddr, iPAddr, IP_IPV6_ADDR_LEN) == IP_NULL) &&
        (IP_MEM_CMP(teInfo->teLinkLayerAddr, mACAddr, IP_MAC_ADDR_LEN) == IP_NULL)) {
        /*lint +e960*/
        return IP_TRUE;
    } else {
        return IP_FALSE;
    }
}
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * Function Name: IP_NDSERVER_SendDlPkt
 * Description: �������л����IPV6���ݰ�
 * Input: ulIndex  -------------  ND SERVERʵ������
 * History:
 *    1.      2011-12-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_SendDlPkt(VOS_UINT32 addrIndex)
{
    IMM_Zc                 *immZc  = VOS_NULL_PTR;
    VOS_UINT8               rabId;
    ADS_PktTypeUint8        pktType;
    VOS_UINT16              app;
    VOS_INT32               lockKey;

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    while (LUP_IsQueEmpty(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex)) != PS_TRUE) {
        lockKey = VOS_SplIMP();
        if (LUP_DeQue(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), (VOS_VOID **)(&immZc)) != PS_SUCC) {
            VOS_Splx(lockKey);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_SendDlPkt, LUP_DeQue return fail!");
            return;
        }
        VOS_Splx(lockKey);

        /* �� ImmZc��ȡ��RabId��enPdpType */
        /*lint -e522*/
        app     = IMM_ZcGetUserApp(immZc);
        rabId   = (VOS_UINT8)(app & 0xFF); /* �����ֵʵ���� ��չRabId */
        pktType = (VOS_UINT8)(app >> 8); /* 8:bit */
        if (Ndis_DlSendNcm(rabId, pktType, immZc) != PS_SUCC) {
            IMM_ZcFree(immZc);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_SendDlPkt, Ndis_DlSendNcm fail!");
        }
        /*lint +e522*/
        IP_NDSERVER_AddSendQuePktNum(addrIndex);
    }

    return;
}
#endif


/*
 * Function Name: IP_NDSERVER_UpdateSpeMac
 * Description: ����Ӳ��MAC��ַ
 */
STATIC VOS_VOID IP_NDSERVER_UpdateSpeMac(VOS_UINT32 addrIndex,  const VOS_UINT8 *macAddr)
{
    IP_NdserverAddrInfo     *infoAddr   = VOS_NULL_PTR;
    VOS_INT32                rlt;

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_UpdateSpeMac: ND Server info flag is invalid!");
        return;
    }

    rlt = memcpy_s(g_speMacHeader.srcAddr, ETH_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(),
                   ETH_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    rlt = memcpy_s(g_speMacHeader.dstAddr, ETH_MAC_ADDR_LEN, macAddr, ETH_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);


#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#if (defined(CONFIG_BALONG_ESPE))
        mdrv_wan_set_ipfmac((VOS_VOID *)&g_speMacHeader);
#endif
#endif

    IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_UpdateSpeMac OK");
    return;
}


/*
 * Function Name: IP_NDSERVER_UpdateTeAddrInfo
 * Description: �յ��ظ���ַ����ַ
 * Input: ulIndex  -------------  ND SERVERʵ������
 *                   aucGlobalIPAddr ------  ȫ��IP��ַָ��
 *                   aucMACAddr -----------  MAC��ַָ��
 *                   aucLinkLocalIPAddr ---  ��·����IP��ַָ��
 * History:
 *    1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_UpdateTeAddrInfo(VOS_UINT32 addrIndex, const VOS_UINT8 *globalIPAddr,
                                             const VOS_UINT8 *macAddr, const VOS_UINT8 *linkLocalIPAddr)
{
    IP_NdserverTeAddrInfo *teInfo = IP_NULL_PTR;
    VOS_INT32                      rlt;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_UpdateTeAddrInfo: Old Te Addr:", teInfo->teAddrState);
    rlt = memcpy_s(teInfo->teGlobalAddr, IP_IPV6_ADDR_LEN, globalIPAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    rlt = memcpy_s(teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN, macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* ��������MAC֡ͷ�е�Ŀ��MAC��ַ */
    rlt = memcpy_s(IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex), IP_ETH_MAC_HEADER_LEN, macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    if (IP_IPV6_IS_LINKLOCAL_ADDR(linkLocalIPAddr)) {
        rlt = memcpy_s(teInfo->teLinkLocalAddr, IP_IPV6_ADDR_LEN, linkLocalIPAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    } else {
        (VOS_VOID)memset_s(teInfo->teLinkLocalAddr, IP_IPV6_ADDR_LEN, IP_NULL, IP_IPV6_ADDR_LEN);
    }

    teInfo->teAddrState = IP_NDSERVER_TE_ADDR_REACHABLE;

    /* ����Ӳ��MAC��ַ */
    IP_NDSERVER_UpdateSpeMac(addrIndex, macAddr);

    IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_UpdateTeAddrInfo: New Te Addr:", teInfo->teAddrState);
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    /* ��������IP��������е����ݰ� */
    IP_NDSERVER_SendDlPkt(addrIndex);
#endif
    return;
}

/*
 * Function Name: IP_NDSERVER_NaMsgProc
 * Description: ����NA��Ϣ
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   pucSrcData ------------ IP���ݱ�
 *                   ulIcmpv6HeadOffset ---- ICMPv6����ͷƫ����
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_NaMsgProc(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT32 icmpv6HeadOffset)
{
    const VOS_UINT8                  *ipMsg     = srcData;
    IP_ND_Msg                  *ndMsgData = VOS_NULL_PTR;
    IP_NdserverAddrInfo        *infoAddr  = IP_NULL_PTR;
    VOS_UINT8                  *ipAddrData = IP_NULL_PTR;

    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NaMsgProc is entered.");
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_AddRcvNaPktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IP_NDSERVER_AddErrNaPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NaMsgProc: ND Server info flag is invalid!");
        return;
    }

    ndMsgData = IP_NDSERVER_GET_NDMSGDATA_ADDR();
    (VOS_VOID)memset_s(ndMsgData, sizeof(IP_ND_Msg), IP_NULL, sizeof(IP_ND_Msg));

    if (IP_NDSERVER_DecodeNaData(ipMsg, ndMsgData, icmpv6HeadOffset) != IP_SUCC) {
        IP_NDSERVER_AddErrNaPktNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NaMsgProc: Invalid IPV6 NS Msg:!");
        return;
    }

    if (ndMsgData->ndMsgStru.naInfo.opTargetLinkLayerAddr == 0) {
        IP_NDSERVER_AddErrNaPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID,
                         "IP_NDSERVER_NaMsgProc:Packet don't contain Target Linklayer Addr, discard!");
        return;
    }

    ipAddrData = &ndMsgData->desIp[0];
    if ((IP_NDSERVER_IsSelfIPAddr(addrIndex, ipAddrData) != IP_TRUE) &&
        (!IP_IPV6_IS_MULTICAST_ADDR(ndMsgData->desIp))) {
        IP_NDSERVER_AddErrNaPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NaMsgProc:Destination IP don't contain self Addr, discard!");
        return;
    }

    if (IP_NDSERVER_EqualAdvertisedPrefix(addrIndex, ndMsgData->ndMsgStru.naInfo.targetAddr) != IP_TRUE) {
        IP_NDSERVER_AddErrNaPktNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_NaMsgProc:Target IP don't equal Advertised Prefix, discard!");
        return;
    }

    if (IP_NDSERVER_EqualSavedTeAddr(addrIndex, ndMsgData->ndMsgStru.naInfo.targetAddr,
                                     ndMsgData->ndMsgStru.naInfo.targetLinkLayerAddr) != IP_TRUE) {
        IP_NDSERVER_UpdateTeAddrInfo(addrIndex, ndMsgData->ndMsgStru.naInfo.targetAddr,
                                     ndMsgData->ndMsgStru.naInfo.targetLinkLayerAddr, ndMsgData->srcIp);
    }

    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_PERIODIC_NS);

    return;
}

/*
 * Function Name: IP_NDSERVER_BuildTooBigICMPPkt
 * Description: �γ���PC�ظ��ĳ�������Ӧ
 * Input: pucSrcData ----------- Դ����ָ��
 *                   pstDestData ---------- Ŀ��ת���ṹָ��
 *                   ulIcmpv6HeadOffset --- ICMPv6��ͷƫ����
 * History:
 *    1.      2011-12-09  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_BuildTooBigICMPPkt(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT8 *destData,
                                                 VOS_UINT32 dataLen)
{
    const VOS_UINT8                     *ipMsg                       = srcData;
    VOS_UINT8                     *data                        = VOS_NULL_PTR;
    ND_Ipv6AddrInfo                srcIPAddr;
    ND_MacAddrInfo                 srcMacAddr;
    ND_Ipv6AddrInfo                dstIPAddr;
    ND_MacAddrInfo                 dstMacAddr;
    IP_NdserverTeAddrInfo         *teInfo                      = IP_NULL_PTR;
    VOS_UINT32                     datatemp; /* linux 4�ֽڶ��� */
    VOS_INT32                      rlt;

    if (dataLen <= (IP_IPV6_HEAD_LEN + IP_ICMPV6_HEAD_LEN)) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_BuildTooBigICMPPkt: ulDataLen is too short.", dataLen);
        return IP_FAIL;
    }
    /* ��������MAC��ַ����link-local��ַ */
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* �õ�Ŀ��IPV6��ַ */
    rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* �õ�Ŀ��MAC��ַ */
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    rlt     = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* ָ��ICMP�ײ� */
    data = destData + IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ICMP type�� */
    *data = IP_ICMPV6_PACKET_TOO_BIG;
    data++;

    /* ICMP code�� */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* ICMP У����� */
    *(VOS_UINT16 *)(VOS_VOID *)data = 0;
    data += 2; /* 2:len */

    /* MTU �� */
    /*lint -e960*/
    datatemp = VOS_HTONL(dataLen); /* linux 4�ֽڶ��� */
    rlt      = memcpy_s(data, sizeof(VOS_UINT32), (VOS_UINT8 *)(&datatemp), sizeof(VOS_UINT32)); /* linux 4�ֽڶ��� */
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    /*lint +e960*/
    data += 4; /* 4:len */

    /* ��дICMP PayLoad���� */
    rlt = memcpy_s(data, (IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - IP_IPV6_HEAD_LEN - IP_ICMPV6_HEAD_LEN), ipMsg,
                   ((dataLen - IP_IPV6_HEAD_LEN) - IP_ICMPV6_HEAD_LEN));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* ָ��IPV6�ײ� */
    destData += IP_ETHERNET_HEAD_LEN;
    /* ��дIPV6ͷ�� */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, (dataLen - IP_IPV6_HEAD_LEN), destData, IP_HEAD_PROTOCOL_ICMPV6);

    /* ����ICMPv6��ͷУ��� */
    if (IP_BuildIcmpv6Checksum(destData, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg: Build ICMPv6 Checksum failed.");
        return IP_FAIL;
    }

    /* ������̫��ͷ */
    destData -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, destData);

    return IP_SUCC;
}

/*
 * ��������: ����PC���͵ĳ�����
 * �޸���ʷ:
 *  1.��    ��: 2011��12��9��
 *    �޸�����: �����ɺ���
 */
VOS_VOID IP_NDSERVER_ProcTooBigPkt(VOS_UINT8 rabId, const VOS_UINT8 *srcData, VOS_UINT32 dataLen)
{
    VOS_UINT32                  addrIndex;
    const VOS_UINT8                  *ipMsg    = srcData;
    VOS_UINT8                  *sendBuff = VOS_NULL_PTR;
    IP_NdserverAddrInfo *infoAddr = IP_NULL_PTR;

    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(rabId);
    if (addrIndex >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTooBigPkt: ND Server info flag is invalid!");
        return;
    }

    IP_NDSERVER_AddRcvBooBigPktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);
    if (IP_NDSERVER_BuildTooBigICMPPkt(addrIndex, ipMsg, sendBuff, dataLen) != IP_SUCC) {
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTooBigPkt: IP_NDSERVER_BuildTooBigICMPPkt FAIL!");
        return;
    }

    /* ����������Ӧ���͵�PC */
    if (Ndis_SendMacFrm(sendBuff, dataLen + IP_ETHERNET_HEAD_LEN, infoAddr->epsbId) == PS_FAIL) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTooBigPkt:Send TooBigPkt failed.");
        return;
    }

    IP_NDSERVER_AddTransTooBigPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return;
}

/*
 * Function Name: IP_NDSERVER_FormEchoReply
 * Description: �γ���PC�ظ���ECHO REPLY
 * Input: pucSrcData ----------- Դ����ָ��
 *                   pstDestData ---------- Ŀ��ת���ṹָ��
 *                   ulIcmpv6HeadOffset --- ICMPv6��ͷƫ����
 * History:
 *    1.      2011-12-09  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_FormEchoReply(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT8 *destData,
                                            VOS_UINT32 dataLen)
{
    const VOS_UINT8                     *ipMsg                       = srcData;
    VOS_UINT8                     *data                        = VOS_NULL_PTR;
    ND_Ipv6AddrInfo                srcIPAddr;
    ND_MacAddrInfo                 srcMacAddr;
    ND_Ipv6AddrInfo                dstIPAddr;
    ND_MacAddrInfo                 dstMacAddr;
    IP_NdserverTeAddrInfo         *teInfo                      = IP_NULL_PTR;
    VOS_INT32                      rlt;

    if (dataLen <= IP_IPV6_HEAD_LEN) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_FormEchoReply: ulDataLen is too short.", dataLen);
        return IP_FAIL;
    }
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);

    /* ��������MAC��ַ����link-local��ַ */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* �õ�Ŀ��IPV6��ַ */
    rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* �õ�Ŀ��MAC��ַ */
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    rlt     = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* ָ��ICMP�ײ� */
    data = destData + IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ��дICMP���� */
    ipMsg += IP_IPV6_HEAD_LEN;
    rlt = memcpy_s(data, (IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - IP_IPV6_HEAD_LEN), ipMsg,
                   (dataLen - IP_IPV6_HEAD_LEN));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* ICMP type�� */
    *data = IP_ICMPV6_TYPE_ECHOREPLY;
    data++;

    /* ICMP code�� */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* ICMP У����� */
    *(VOS_UINT16 *)(VOS_VOID *)data = 0;

    /* ָ��IPV6�ײ� */
    destData += IP_ETHERNET_HEAD_LEN;
    /* ��дIPV6ͷ�� */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, (dataLen - IP_IPV6_HEAD_LEN), destData,
                            IP_HEAD_PROTOCOL_ICMPV6);

    /* ����ICMPv6��ͷУ��� */
    if (IP_BuildIcmpv6Checksum(destData, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormEchoReply: Build ICMPv6 Checksum failed.");
        return IP_FAIL;
    }

    /* ������̫��ͷ */
    destData -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, destData);

    return IP_SUCC;
}

/*
 * Function Name: IP_NDSERVER_EchoRequestMsgProc
 * Description: ����ECHO REQUEST��Ϣ
 * Input: ulIndex  -------------- ������Ϣʵ������
 *                   pucSrcData ------------ IP���ݱ�
 *                   ulIcmpv6HeadOffset ---- ICMPv6����ͷƫ����
 * History:
 *    1.      2011-12-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_EchoRequestMsgProc(VOS_UINT32 addrIndex, const VOS_UINT8 *srcData, VOS_UINT32 dataLen)
{
    const VOS_UINT8                  *ipMsg                     = srcData;
    VOS_UINT8                  *sendBuff                  = VOS_NULL_PTR;
    IP_NdserverAddrInfo        *infoAddr                  = IP_NULL_PTR;
    VOS_UINT8                   pktAddr[IP_IPV6_ADDR_LEN];
    VOS_INT32                   rlt;
    VOS_UINT8                  *ipAddrData = &pktAddr[0];

    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_EchoRequestMsgProc is entered.");

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_AddRcvEchoPktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IP_NDSERVER_AddErrEchoPktNum(addrIndex);
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_EchoRequestMsgProc: ND Server info flag is invalid!");
        return;
    }

    rlt = memcpy_s(pktAddr, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    if (IP_NDSERVER_IsSelfIPAddr(addrIndex, ipAddrData) != IP_TRUE) {
        IP_NDSERVER_AddErrEchoPktNum(addrIndex);
        return;
    }

    sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);

    if (IP_NDSERVER_FormEchoReply(addrIndex, ipMsg, sendBuff, dataLen) != IP_SUCC) {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_EchoRequestMsgProc: IP_NDSERVER_FormEchoReply fail!");
        return;
    }

    /* ��ECHO REPLY���͵�PC */
    if (Ndis_SendMacFrm(sendBuff, dataLen + IP_ETHERNET_HEAD_LEN, infoAddr->epsbId) == PS_FAIL) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_EchoRequestMsgProc:Send ECHO REPLY failed.");
        return;
    }

    IP_NDSERVER_AddTransEchoPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return;
}
STATIC VOS_VOID NdSer_NdProcPktByMsgType(const VOS_UINT8 *data, VOS_UINT32 icmpv6HeadOffset, VOS_UINT32 addrIndex,
                                         VOS_UINT32 dataLen)
{
    IP_Icmpv6TypeUint32         icmpv6MsgType;
    /* ȡICMPV6��Ϣ�е�TYPE�ֶ� */
    icmpv6MsgType = *(data + icmpv6HeadOffset);

    switch (icmpv6MsgType) {
        case IP_ICMPV6_TYPE_RS:
            IP_NDSERVER_RsMsgProc(addrIndex, data, icmpv6HeadOffset);
            break;
        case IP_ICMPV6_TYPE_NS:
            IP_NDSERVER_NsMsgProc(addrIndex, data, icmpv6HeadOffset);
            break;
        case IP_ICMPV6_TYPE_NA:
            IP_NDSERVER_NaMsgProc(addrIndex, data, icmpv6HeadOffset);
            break;
        case IP_ICMPV6_TYPE_ECHOREQUEST:
            IP_NDSERVER_EchoRequestMsgProc(addrIndex, data, dataLen);
            break;

        default:
            IP_NDSERVER_AddDiscPktNum(addrIndex);
            IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "NdSer_NdAndEchoPktProc: Ignored IPV6 ND Msg:!", icmpv6MsgType);
            break;
    }
    return;
}
VOS_UINT32 NdSer_NdGetMsgInfo(const MsgBlock *msgBlock, VOS_UINT8 **data, VOS_UINT32 *dataLen, VOS_UINT8 *exRabId)
{
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
    CDS_NDIS_DataInd       *rcvMsg = (CDS_NDIS_DataInd *)(VOS_VOID *)msgBlock;
    *data      = rcvMsg->data;
    *dataLen   = rcvMsg->len;
    *exRabId   = rcvMsg->ifaceId;
    if (*exRabId >= PS_IFACE_ID_BUTT) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_NdAndEchoPktProc, IMM_ZcGetDataPtr return NULL", exRabId);
        return PS_FAIL;
    }
#else
    ADS_NDIS_DataInd       *rcvMsg = (ADS_NDIS_DataInd *)(VOS_VOID *)msgBlock;

    *data = IMM_ZcGetDataPtr(rcvMsg->data);
    if (*data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NdSer_NdAndEchoPktProc, IMM_ZcGetDataPtr fail!");
        return PS_FAIL;
    }
    *dataLen = IMM_ZcGetUsedLen(rcvMsg->data);
    *exRabId = NDIS_FORM_EXBID(rcvMsg->modemId, rcvMsg->rabId);
#endif
    return PS_SUCC;
}
/*
 * Function Name: NdSer_NdAndEchoPktProc
 */
VOS_VOID NdSer_NdAndEchoPktProc(const MsgBlock *msgBlock)
{
    VOS_UINT32                  icmpv6HeadOffset = IP_NULL;
    VOS_UINT32                  addrIndex;
    VOS_UINT8                  *data             = VOS_NULL_PTR;
    VOS_UINT32                  dataLen;
    VOS_UINT8                   exRabId;
    IP_NdserverAddrInfo *nDSerAddrInfoTmp = VOS_NULL_PTR;
    if (NdSer_NdGetMsgInfo(msgBlock, &data, &dataLen, &exRabId) != PS_SUCC) {
        return;
    }

#if (FEATURE_OFF == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    if (ND_CheckEpsIdValid(exRabId) != IP_SUCC) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_NdAndEchoPktProc, IMM_ZcGetDataPtr return NULL", exRabId);
        return;
    }
#endif
    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (addrIndex >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        /*
         * MT�ػ��ٿ�����ND SERVER�յ�����RAǰ���������յ�TE���ظ���ַ���NS ��Ϊ���ܹ����������Ϣ��
         * �˴���ʱ����һ��ND Ser Addr Info
         */
        IPND_INFO_LOG1(NDIS_NDSERVER_PID, "NdSer_NdAndEchoPktProc: alloc new addr info for NS msg", exRabId);
        nDSerAddrInfoTmp = NdSer_AllocAddrInfo(&addrIndex);
        if (nDSerAddrInfoTmp == IP_NULL_PTR) {
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_AllocAddrInfo: return NULL!");
            return;
        }
        nDSerAddrInfoTmp->validFlag = IP_FALSE;
    }

    IP_NDSERVER_AddRcvPktTotalNum(addrIndex);

    /* �ж���Ϣ�Ƿ�Ϸ���ND��Ϣ */
    if (IP_IsValidNdMsg(data, dataLen, &icmpv6HeadOffset) != IP_TRUE) {
        IP_NDSERVER_AddDiscPktNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_NdAndEchoPktProc: Invalid ND Msg!");
        return;
    }

    NdSer_NdProcPktByMsgType(data, icmpv6HeadOffset, addrIndex, dataLen);

    return;
}
STATIC VOS_UINT32 IP_GetDnsOptLen(const ND_IP_Ipv6DnsSer *dnsSer, const IP_Ipv6PktDhcpOptHdr *dhcpRequestOpt,
                                  VOS_UINT16 *dnsOptLen, VOS_UINT16 *dhcpRequestDnsOptLen,
                                  VOS_UINT16 *replyUdpDataLen)
{
    VOS_UINT32                     dhcpReqOptCodeLoopCnt;

    for (dhcpReqOptCodeLoopCnt = 0; dhcpReqOptCodeLoopCnt < (dhcpRequestOpt->dhcpOptLen);
         dhcpReqOptCodeLoopCnt += 2) { /* 2:len */
        if (VOS_NTOHS(*((VOS_UINT16 *)(&(dhcpRequestOpt->dhcpOptData[dhcpReqOptCodeLoopCnt]))))
            == IP_IPV6_DHCP6_OPT_DNS_SERVERS) {
            (*dnsOptLen) = dnsSer->dnsSerNum;
            if (((*dnsOptLen) == 0) || ((*dnsOptLen) > IP_IPV6_MAX_DNS_NUM)) {
                IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_GetDnsOptLen, DNS Option is err!");
                return PS_FAIL;
            }

            (*dhcpRequestDnsOptLen) = (VOS_UINT16)((*dnsOptLen) * sizeof(NDIS_Ipv6Addr));
            (*replyUdpDataLen) += (*dhcpRequestDnsOptLen + 4); /* 4��ʾ2���ֽڵ�option-code+2�ֽڵ�option-len */
            break;
        }
    }
    return PS_SUCC;
}
STATIC VOS_VOID IP_GetSrcAddr(ND_MacAddrInfo *srcMacAddr, ND_Ipv6AddrInfo *srcIPAddr)
{
    errno_t rlt;
    rlt = memcpy_s(srcMacAddr->macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr->ipv6Addr, srcMacAddr->macAddr);
    IP_SetUint16Data(srcIPAddr->ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);
    return;
}
STATIC VOS_VOID IP_GetDstAddr(ND_Ipv6AddrInfo *dstIPAddr, ND_MacAddrInfo *dstMacAddr, VOS_UINT32 addrIndex,
                              const VOS_UINT8 *dhcpInfoReqData)
{
    IP_NdserverTeAddrInfo         *teInfo                      = IP_NULL_PTR;
    errno_t                        rlt;
    rlt = memcpy_s(dstIPAddr->ipv6Addr, IP_IPV6_ADDR_LEN, dhcpInfoReqData + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));

    /* �õ�Ŀ��MAC��ַ */
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    rlt = memcpy_s(dstMacAddr->macAddr, IP_MAC_ADDR_LEN, teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN);
    NDIS_CHK_SEC_RETURN_NO_VAL((rlt != EOK));
    return;
}
/*lint -e778*/
/*lint -e572*/
STATIC VOS_UINT32 IP_FillDhcpServerIdOption(VOS_UINT8 *dhcpReplyDhcpHdrOffset, ND_MacAddrInfo srcMacAddr)
{
    IP_Ipv6PktDhcpDuidLLOpt        dhcpDuidLLOpt;
    errno_t                        rlt;
    (VOS_VOID)memset_s(&dhcpDuidLLOpt, sizeof(IP_Ipv6PktDhcpDuidLLOpt), 0,
                       sizeof(IP_Ipv6PktDhcpDuidLLOpt));
    dhcpDuidLLOpt.dhcpDuidType   = VOS_HTONS(IP_IPV6_DHCP_DUID_LL_OPT_TYPE);
    dhcpDuidLLOpt.dhcpDuidHWType = VOS_HTONS(IP_IPV6_HW_TYPE);
    rlt = memcpy_s(dhcpDuidLLOpt.linkLayerAddr, IP_IPV6_DHCP_OPT_LINKADDR_SIZE, srcMacAddr.macAddr,
                   IP_IPV6_DHCP_OPT_LINKADDR_SIZE);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);
    (*(VOS_UINT16 *)dhcpReplyDhcpHdrOffset) = VOS_HTONS(IP_IPV6_DHCP_OPT_SERVER_ID);
    dhcpReplyDhcpHdrOffset += sizeof(VOS_UINT16);
    (*(VOS_UINT16 *)dhcpReplyDhcpHdrOffset) = VOS_HTONS(IP_IPV6_DHCP_DUID_LL_OPT_LEN);

    dhcpReplyDhcpHdrOffset += sizeof(VOS_UINT16);
    rlt = memcpy_s(dhcpReplyDhcpHdrOffset, IP_IPV6_DHCP_DUID_LL_OPT_LEN, (VOS_UINT8 *)(&dhcpDuidLLOpt),
                   IP_IPV6_DHCP_DUID_LL_OPT_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);
    return PS_SUCC;
}
/*lint +e778*/
/*lint +e572*/
STATIC VOS_UINT32 IP_FillDhcpClientIdOption(const IP_Ipv6PktDhcpOptHdr *dhcpClientIdOpt,
                                            VOS_UINT8 *dhcpReplyDhcpHdrOffset, VOS_UINT32 *tmpDestLen)
{
    errno_t                        rlt;

    /* 4��ʾ2���ֽڵ�option-code+2�ֽڵ�option-len */
    if (((VOS_UINT32)(VOS_NTOHS(dhcpClientIdOpt->dhcpOptLen) + 4)) > (*tmpDestLen)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_FillDhcpClientIdOption, DHCP CLient ID option is err!");
        return PS_FAIL;
    }
    rlt = memcpy_s(dhcpReplyDhcpHdrOffset, (*tmpDestLen), (VOS_UINT8 *)dhcpClientIdOpt,
                   (VOS_NTOHS(dhcpClientIdOpt->dhcpOptLen) + 4)); /* 4:len */
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);
    *tmpDestLen -= TTF_MIN(*tmpDestLen, (VOS_UINT32)(VOS_NTOHS(dhcpClientIdOpt->dhcpOptLen) + 4)); /* 4:len */
    return PS_SUCC;
}
STATIC VOS_UINT32 IP_FillDhcpDnsOption(VOS_UINT8 *dhcpReplyDhcpHdrOffset, const ND_IP_Ipv6DnsSer *dnsSer,
                                       VOS_UINT16 dnsOptLen, VOS_UINT32 tmpDestLen, VOS_UINT16 dhcpRequestDnsOptLen)
{
    IP_Ipv6DhcpDnsOption           ipv6DhcpDnsOpt;
    errno_t                        rlt;
    if (dhcpRequestDnsOptLen != 0) {
        (VOS_VOID)memset_s(&ipv6DhcpDnsOpt, sizeof(IP_Ipv6DhcpDnsOption), 0, sizeof(IP_Ipv6DhcpDnsOption));
        /*lint -e778*/
        /*lint -e572*/
        ipv6DhcpDnsOpt.optionCode = VOS_HTONS(IP_IPV6_DHCP6_OPT_DNS_SERVERS);
        /*lint +e778*/
        /*lint +e572*/
        rlt = memcpy_s(&ipv6DhcpDnsOpt.ipv6DNS[0], (sizeof(NDIS_Ipv6Addr)), dnsSer->priDnsServer,
                       (sizeof(NDIS_Ipv6Addr)));
        IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);

        if (dnsOptLen == IP_IPV6_MAX_DNS_NUM) {
            rlt = memcpy_s(&ipv6DhcpDnsOpt.ipv6DNS[1], (sizeof(NDIS_Ipv6Addr)), dnsSer->secDnsServer,
                           (sizeof(NDIS_Ipv6Addr)));
            IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);
        }
        ipv6DhcpDnsOpt.optionLen = VOS_HTONS((VOS_UINT16)(IP_IPV6_ADDR_LEN * dnsOptLen));
        rlt = memcpy_s(dhcpReplyDhcpHdrOffset, tmpDestLen, (VOS_UINT8 *)&ipv6DhcpDnsOpt,
                       (4 + (dnsOptLen * sizeof(NDIS_Ipv6Addr)))); /* 4:len */
        IP_CHK_SEC_RETURN_VAL(rlt != EOK, PS_FAIL);
    }
    return PS_SUCC;
}
VOS_VOID IP_NDSERVER_FillIpUdpInfo(NDIS_Ipv6Hdr *dhcpReplyIpv6Hdr, IP_UdpHeadInfo *dhcpReplyUdpHdr,
                                   ND_Ipv6AddrInfo srcIPAddr, ND_Ipv6AddrInfo dstIPAddr, VOS_UINT16 replyUdpDataLen)
{
    /* ��� IP ͷ */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, replyUdpDataLen, (VOS_UINT8 *)(dhcpReplyIpv6Hdr),
                            IP_HEAD_PROTOCOL_UDP);

    /* ��� UDP ͷ */
    (VOS_VOID)TTF_NDIS_InputUDPHead((VOS_UINT8 *)(dhcpReplyUdpHdr), IP_IPV6_DHCP6_UE_PORT, IP_IPV6_DHCP6_PC_PORT,
                                    replyUdpDataLen);
}
VOS_UINT32 IP_FillIdInfo(ND_MacAddrInfo srcMacAddr, VOS_UINT8 **dhcpReplyDhcpHdrOffset, VOS_UINT32 *tmpDestLen,
                         IP_Ipv6PktDhcpOptHdr *dhcpClientIdOpt, VOS_UINT8 *dhcpInfoReqData)
{
    /* ��� DHCP reply ͷ */
    *(*dhcpReplyDhcpHdrOffset) = IP_IPV6_DHCP6_TYPE_REPLY;
    *dhcpReplyDhcpHdrOffset += 1;
    if (memcpy_s(*dhcpReplyDhcpHdrOffset, IP_IPV6_DHCP6_TRANS_ID_LEN,
                 ((dhcpInfoReqData + IP_IPV6_HEAD_LEN) + IP_UDP_HEAD_LEN) + 1, IP_IPV6_DHCP6_TRANS_ID_LEN) != EOK) {
        return PS_FAIL;
    }

    *dhcpReplyDhcpHdrOffset += IP_IPV6_DHCP6_TRANS_ID_LEN;

    /* ��� DHCP ServerID option (DUID ʹ�õ����ַ�ʽ�� DUID-LL) */
    if (IP_FillDhcpServerIdOption(*dhcpReplyDhcpHdrOffset, srcMacAddr) == PS_FAIL) {
        return PS_FAIL;
    }

    *dhcpReplyDhcpHdrOffset += sizeof(VOS_UINT16) + sizeof(VOS_UINT16) + IP_IPV6_DHCP_DUID_LL_OPT_LEN;

    *tmpDestLen = IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - sizeof(NDIS_Ipv6Hdr) - sizeof(IP_UdpHeadInfo) -
                 IP_IPV6_DHCP6_REPLY_HDR_LEN - IP_IPV6_DHCP_DUID_LL_OPT_LEN;

    /* ��� DHCP CLient ID option */
    if (dhcpClientIdOpt != VOS_NULL_PTR) {
        if (IP_FillDhcpClientIdOption(dhcpClientIdOpt, *dhcpReplyDhcpHdrOffset, tmpDestLen) == PS_FAIL) {
            return PS_FAIL;
        }
        *dhcpReplyDhcpHdrOffset += (VOS_NTOHS(dhcpClientIdOpt->dhcpOptLen) + 4); /* 4:lenoffset */
    }
    return PS_SUCC;
}
VOS_VOID IP_NDSERVER_GetOption(VOS_UINT16 reqIp6PktLen, VOS_UINT8 *dhcpInfoReqData,
                               IP_Ipv6PktDhcpOptHdr **dhcpClientIdOpt,
                               IP_Ipv6PktDhcpOptHdr **dhcpRequestOpt,
                               VOS_UINT16 *replyUdpDataLen)
{
    VOS_UINT16                     reqDhcpOptLen;
    IP_Ipv6PktDhcpOptHdr          *reqDhcpOptHdr               = VOS_NULL_PTR;
    VOS_UINT16                     dhcpClientIdOptLen;
    reqDhcpOptLen   = reqIp6PktLen - ((IP_IPV6_HEAD_LEN + IP_UDP_HEAD_LEN) + IP_UDP_DHCP_HDR_SIZE);
    reqDhcpOptHdr  = (IP_Ipv6PktDhcpOptHdr *)(VOS_VOID *)(dhcpInfoReqData + IP_IPV6_HEAD_LEN +
                                                          IP_UDP_HEAD_LEN + IP_UDP_DHCP_HDR_SIZE);
    /* ��ȡ DHCP Client ID Option */
    (VOS_VOID)TTF_NDIS_Ipv6GetDhcpOption(reqDhcpOptHdr, reqDhcpOptLen, IP_IPV6_DHCP_OPT_CLIEND_ID,
                                         dhcpClientIdOpt, 0);
    if (*dhcpClientIdOpt != VOS_NULL_PTR) {
        dhcpClientIdOptLen = VOS_NTOHS((*dhcpClientIdOpt)->dhcpOptLen);
        *replyUdpDataLen += (dhcpClientIdOptLen + 4); /* 4��ʾ2���ֽڵ�option-code+2�ֽڵ�option-len */
    }

    /* ��ȡ DHCP Request Option */
    (VOS_VOID)TTF_NDIS_Ipv6GetDhcpOption(reqDhcpOptHdr, reqDhcpOptLen, IP_IPV6_DHCP_OPT_REQUEST,
                                         dhcpRequestOpt, 0);
}
VOS_VOID IP_NDSERVER_GetFillReplyInfoAddr(VOS_UINT8 **sendBuff, NDIS_Ipv6Hdr **dhcpReplyIpv6Hdr,
                                          IP_UdpHeadInfo **dhcpReplyUdpHdr, VOS_UINT8 **dhcpReplyDhcpHdrOffset)
{
    *sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(*sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);
    /* ָ��IPV6�ײ� */
    *sendBuff += IP_ETHERNET_HEAD_LEN;
    *dhcpReplyIpv6Hdr     = (NDIS_Ipv6Hdr *)(*sendBuff);
    *dhcpReplyUdpHdr      = (IP_UdpHeadInfo *)((*dhcpReplyIpv6Hdr) + 1);
    *dhcpReplyDhcpHdrOffset = (VOS_UINT8 *)((*dhcpReplyUdpHdr) + 1);
}
/*
 * Description: ��װ������DHCPV6����
 */
/*lint -e778*/
/*lint -e572*/
STATIC VOS_UINT32 IP_NDSERVER_SendDhcp6Reply(VOS_UINT32 addrIndex, const ND_IP_Ipv6DnsSer *dnsSer,
                                             VOS_UINT8 *dhcpInfoReqData, VOS_UINT16 reqIp6PktLen)
{
    NDIS_Ipv6Hdr                  *dhcpReplyIpv6Hdr            = VOS_NULL_PTR;
    IP_UdpHeadInfo                *dhcpReplyUdpHdr             = VOS_NULL_PTR;
    IP_Ipv6PktDhcpOptHdr          *dhcpRequestOpt              = VOS_NULL_PTR;
    IP_Ipv6PktDhcpOptHdr          *dhcpClientIdOpt             = VOS_NULL_PTR;
    VOS_UINT32                     tmpDestLen;
    VOS_UINT8                     *dhcpReplyDhcpHdrOffset      = VOS_NULL_PTR; /* �ƶ�ָ�� */
    VOS_UINT16                     replyUdpDataLen;
    VOS_UINT16                     dhcpReplyPktLen;
    VOS_UINT16                     dnsOptLen                   = 0;
    VOS_UINT16                     dhcpRequestDnsOptLen        = 0;
    ND_Ipv6AddrInfo                srcIPAddr;
    ND_MacAddrInfo                 srcMacAddr;
    ND_Ipv6AddrInfo                dstIPAddr;
    ND_MacAddrInfo                 dstMacAddr;
    VOS_UINT8                     *sendBuff                    = VOS_NULL_PTR;

    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);
    replyUdpDataLen = IP_UDP_HEAD_LEN + IP_UDP_DHCP_HDR_SIZE + IP_IPV6_DHCP_OPT_CLIENT_ID_LEN;
    IP_NDSERVER_GetOption(reqIp6PktLen, dhcpInfoReqData, &dhcpClientIdOpt, &dhcpRequestOpt, &replyUdpDataLen);
    if (dhcpRequestOpt != VOS_NULL_PTR) {
        if (IP_GetDnsOptLen(dnsSer, dhcpRequestOpt, &dnsOptLen, &dhcpRequestDnsOptLen, &replyUdpDataLen) == PS_FAIL) {
            return PS_FAIL;
        }
    }
    if (replyUdpDataLen <= (IP_UDP_HEAD_LEN + IP_UDP_DHCP_HDR_SIZE + IP_IPV6_DHCP_OPT_CLIENT_ID_LEN)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_SendDhcp6Reply, No content need to reply!");
        return PS_SUCC;
    }

    /* ��������MAC��ַ����link-local��ַ */
    IP_GetSrcAddr(&srcMacAddr, &srcIPAddr);
    /* �õ�Ŀ��IPV6��ַ */
    IP_GetDstAddr(&dstIPAddr, &dstMacAddr, addrIndex, dhcpInfoReqData);

    IP_NDSERVER_GetFillReplyInfoAddr(&sendBuff, &dhcpReplyIpv6Hdr, &dhcpReplyUdpHdr, &dhcpReplyDhcpHdrOffset);

    IP_NDSERVER_FillIpUdpInfo(dhcpReplyIpv6Hdr, dhcpReplyUdpHdr, srcIPAddr, dstIPAddr, replyUdpDataLen);

    if (IP_FillIdInfo(srcMacAddr, &dhcpReplyDhcpHdrOffset, &tmpDestLen, dhcpClientIdOpt, dhcpInfoReqData) != PS_SUCC) {
        return PS_FAIL;
    }

    /* ��� DHCP DNS OPTION */
    if (IP_FillDhcpDnsOption(dhcpReplyDhcpHdrOffset, dnsSer, dnsOptLen, tmpDestLen, dhcpRequestDnsOptLen) == PS_FAIL) {
        return PS_FAIL;
    }
    dhcpReplyUdpHdr->udpCheckSum = TTF_NDIS_Ipv6_CalcCheckSum((VOS_UINT8 *)dhcpReplyUdpHdr, replyUdpDataLen,
        &dhcpReplyIpv6Hdr->srcAddr, &dhcpReplyIpv6Hdr->dstAddr, IP_HEAD_PROTOCOL_UDP);
    /* ָ����̫���ײ� */
    sendBuff -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, sendBuff);

    IP_NDSERVER_AddTransDhcpv6PktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    /* ����reply��Ϣ���� */
    dhcpReplyPktLen = TTF_MIN(IP_IPV6_HEAD_LEN + replyUdpDataLen + IP_ETHERNET_HEAD_LEN, IP_IPM_MTU);

    /* ��DHCPV6 REPLY��Ϣ���͵�PC */
    return Ndis_SendMacFrm(sendBuff, dhcpReplyPktLen, IP_NDSERVER_ADDRINFO_GET_EPSID(addrIndex));
}

/*lint +e778*/
/*lint +e572*/
STATIC VOS_UINT32 NdSer_DhcpJugePortMsgTypeValid(const VOS_UINT8 *data, VOS_UINT32 addrIndex)
{
    VOS_UINT16                  srcPort;
    VOS_UINT16                  dstPort;
    VOS_UINT8                   msgType;
    const VOS_UINT8                  *udpData    = VOS_NULL_PTR;

    udpData = data + IP_IPV6_HEAD_LEN;
    srcPort  = *(VOS_UINT16 *)(VOS_VOID *)udpData;
    dstPort  = *(VOS_UINT16 *)(VOS_VOID *)(udpData + 2); /* 2:��srcportƫ�������ֽڻ�ȡdstport */

    if ((VOS_NTOHS(srcPort) != IP_IPV6_DHCP6_PC_PORT) || (VOS_NTOHS(dstPort) != IP_IPV6_DHCP6_UE_PORT)) {
        IP_NDSERVER_AddErrDhcpv6PktNum(addrIndex);
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, usSrcPort, usDstPort is err!", srcPort, dstPort);
        return PS_FAIL;
    }

    msgType = *(udpData + IP_UDP_HEAD_LEN);
    if (msgType != IP_IPV6_DHCP6_INFOR_REQ) {
        IP_NDSERVER_AddErrDhcpv6PktNum(addrIndex);
        IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, ucMsgType is err!", msgType);
        return PS_FAIL;
    }
    return PS_SUCC;
}
VOS_UINT32 NdSer_DhcpV6GetMsgInfo(const MsgBlock *msgBlock, VOS_UINT8 **data, VOS_UINT32 *dataLen, VOS_UINT8 *exRabId)
{
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON)
    CDS_NDIS_DataInd       *rcvMsg = (CDS_NDIS_DataInd *)(VOS_VOID *)msgBlock;
    *data      = rcvMsg->data;
    *dataLen   = rcvMsg->len;
    *exRabId   = rcvMsg->ifaceId;

#else
    ADS_NDIS_DataInd       *rcvMsg = (ADS_NDIS_DataInd *)(VOS_VOID *)msgBlock;

    *data = IMM_ZcGetDataPtr(rcvMsg->data);
    if (*data == VOS_NULL_PTR) {
        NDIS_ERROR_LOG(NDIS_TASK_PID, "NdSer_DhcpV6PktProc, IMM_ZcGetDataPtr fail!");
        return PS_FAIL;
    }
    *dataLen = IMM_ZcGetUsedLen(rcvMsg->data);
    *exRabId = NDIS_FORM_EXBID(rcvMsg->modemId, rcvMsg->rabId);
#endif
    return PS_SUCC;
}
/*
 * Description: ����DHCPV6����
 */
VOS_VOID NdSer_DhcpV6PktProc(const MsgBlock *msgBlock)
{
    VOS_UINT32                  addrIndex;
    VOS_UINT8                  *data       = VOS_NULL_PTR;
    VOS_UINT32                  dataLen;
    IP_NdserverAddrInfo        *infoAddr   = IP_NULL_PTR;
    ND_IP_Ipv6DnsSer           *dnsSer     = VOS_NULL_PTR;
    VOS_UINT8                   exRabId;

    if (NdSer_DhcpV6GetMsgInfo(msgBlock, &data, &dataLen, &exRabId) != PS_SUCC) {
        return;
    }
#if (FEATURE_OFF == FEATURE_DATA_SERVICE_NEW_PLATFORM)
    if (ND_CheckEpsIdValid(exRabId) != IP_SUCC) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, ucRabId is Invalid!", exRabId);
        return;
    }
#endif

    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (addrIndex >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc: Invalid Input:", exRabId, addrIndex);
        return;
    }

    IP_NDSERVER_AddRcvDHCPV6PktNum(addrIndex);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    if (NdSer_DhcpJugePortMsgTypeValid(data, addrIndex) == PS_FAIL) {
        return;
    }

    dnsSer = &(infoAddr->ipv6NwPara.dnsSer);

    /* Nd Serverû��DNS������յ�Dhcpv6 information request,�����ñ��� */
    if (dnsSer->dnsSerNum == 0) {
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, No DNS information exists");
        return;
    }
    if (dataLen <= ((IP_IPV6_HEAD_LEN + IP_UDP_HEAD_LEN) + IP_UDP_DHCP_HDR_SIZE)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, No Option Len!");
        return;
    }
    if (IP_NDSERVER_SendDhcp6Reply(addrIndex, dnsSer, data, (VOS_UINT16)dataLen) == PS_FAIL) {
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "NdSer_DhcpV6PktProc, send dhcp6 reply fail");
    }

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgNsExp
 * Description: �����ھ�����󣬵ȴ��ھӹ��泬ʱ
 * Input: pMsg -------- ��Ϣָ��
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp is entered.");

    /* ����Ϣ��ȡ��ND SERVER���� */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp: ND Server state err!");
        return;
    }

    /* �ж���ʱ�����Ƿ�С�ڹ涨��ʱ���� */
    if (infoAddr->timerInfo.loopTimes < g_nsTimerMaxExpNum) {
        /* ����NS��Ϣ��PC */
        if (PS_SUCC != IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr)) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp:Send NS Msg failed.");
        }

        /* ����ʱ����ʱ������1 */
        infoAddr->timerInfo.loopTimes++;

        /* �����ھ�����ʱ�� */
        IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);
    } else {
        /* �����ʱ����Ϣ */
        infoAddr->timerInfo.loopTimes = 0;

        /* ���δ���״̬��TE��ַ����Ӧ��ַ�������������TE��ַ���ط�RA */
        if (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INCOMPLETE) {
            infoAddr->teAddrInfo.teAddrState = IP_NDSERVER_TE_ADDR_INEXISTENT;

            /* ����RA��Ϣ��PC */
            if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
                IP_NDSERVER_AddTransPktFailNum(addrIndex);
                IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp:Send RA Msg failed.");
            }

            /* ����·�ɹ��涨ʱ�� */
            IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
            IP_NDSERVER_ClearDlPktQue(addrIndex); /* ������л�������е�PKT */
#endif
        } else {
            /* �����������ھ�����ʱ�� */
            IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_PERIODIC_NS);
        }
    }

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgPeriodicNsExp
 * Description: �������ھ�����ʱ
 * Input: pMsg -------- ��Ϣָ��
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgPeriodicNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* ��ӡ����ú��� */
    IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp is entered.");

    /* ����Ϣ��ȡ��ND SERVER���� */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp: ND Server state err!");
        return;
    }

    /* ���������Է���RA��ʱ����ֹRouter���� */
    if (g_periodicRaTimeCnt[addrIndex]-- == 0) {
        g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;
        ;

        /* ����RA��Ϣ��PC */
        if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp:Send RA Msg failed.");
        }
    }

    /* ����NS��Ϣ��PC */
    if (IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp:Send NS Msg failed.");
    }

    /* �����ʱ����Ϣ */
    infoAddr->timerInfo.loopTimes = 0;
    /* �����ھ�����ʱ�� */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgPeriodicNsExp
 * Description: �յ��ظ���ַ����ȴ���ʱ����ʱ
 * Input: pMsg -------- ��Ϣָ��
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgFirstNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp is entered.");

    /* ����Ϣ��ȡ��ND SERVER���� */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp: ND Server state err!");
        return;
    }

    /* ����NS��Ϣ��PC */
    if (IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp:Send NS Msg failed.");
    }

    /* �����ʱ����Ϣ */
    infoAddr->timerInfo.loopTimes = 0;
    /* �����ھ�����ʱ�� */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgRaExp
 * Description: �յ��ظ���ַ���ǰ���ڷ���·�ɹ��泬ʱ
 * Input: pMsg -------- ��Ϣָ��
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgRaExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo        *infoAddr                    = IP_NULL_PTR;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (VOS_OS_VER != VOS_WIN32)
    VOS_UINT8                  *data                        = VOS_NULL_PTR;
    VOS_INT32                   lockKey;
#endif
        VOS_INT32                   rlt;
        IMM_Zc                     *immZc                       = VOS_NULL_PTR;
        VOS_UINT8                   dstIPAddr[IP_IPV6_ADDR_LEN];
#endif

    /* ��ӡ����ú��� */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp is entered.");

    /* ����Ϣ��ȡ��ND SERVER���� */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp: ND Server state err!");
        return;
    }

    /* ����RA��Ϣ��PC */
    if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp:Send RA Msg failed.");
    }

    /* �����ʱ����Ϣ */
    infoAddr->timerInfo.loopTimes = 0;
    /* ����·�ɹ��涨ʱ�� */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);

    /* NDIS����C�ˣ��޻������ */
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (VOS_OS_VER != VOS_WIN32)
    /* *********** ����IP����Ŀ�ĵ�ַ��ΪNS����Ŀ���ַ�����е�ַ���� ********* */
    lockKey = VOS_SplIMP();
    if (LUP_PeekQueHead(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), (VOS_VOID **)(&immZc)) != PS_SUCC) {
        VOS_Splx(lockKey);
        return;
    }
    VOS_Splx(lockKey);

    data = IMM_ZcGetDataPtr(immZc);
    if (data == VOS_NULL_PTR) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp, IMM_ZcGetDataPtr return NULL");
        return;
    }

    /* �õ�Ŀ��IPV6��ַ������NS���� */
    rlt = memcpy_s(dstIPAddr, IP_IPV6_ADDR_LEN, data + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    if (IP_NDSERVER_SendNsMsg(addrIndex, dstIPAddr) != PS_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp, IP_NDSERVER_SendNsMsg return NULL");
        return;
    }
#endif
#endif
    return;
}

/*
 * Function Name: IP_NDSERVER_TimerMsgDistr
 * Description: ND SERVER TIMER��Ϣ�ַ�����
 * History:
 *      1.     2011-04-02  Draft Enact
 */
NDSER_TimerUint32 IP_NDSERVER_TimerMsgDistr(const MsgBlock *rcvMsg)
{
    VOS_UINT32              timerIndex;
    NDSER_TimerUint32       timerType;
    IP_TimerInfo           *timerInfo = VOS_NULL_PTR;

    /* ����Ϣ��ȡ��ulIndex��enTimerType */
    timerIndex     = IP_GetTimerPara(rcvMsg);
    timerType = (NDSER_TimerUint32)IP_GetTimerName(rcvMsg);
    /* �жϺϷ��� */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: Invalid Timer Name or Para !");
        return IP_MSG_DISCARD;
    }

    /* ������Ϣ��Ӧ�������źͶ�ʱ������,��ȡ������Ķ�ʱ�� */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: Get Timer failed.");
        return IP_MSG_DISCARD;
    }

    /* �ж���Ϣ�����붨ʱ�������Ƿ�һ�� */
    if (timerType != timerInfo->timerName) {
        /* ��ӡ�쳣��Ϣ */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: TimerType not match:", timerType,
                          timerInfo->timerName);
        return IP_MSG_DISCARD;
    }

    /* ��ʱ����ʱ���� */
    switch (timerType) {
        case IP_ND_SERVER_TIMER_NS:
            IP_NDSERVER_ProcTimerMsgNsExp(timerIndex);
            break;
        case IP_ND_SERVER_TIMER_PERIODIC_NS:
            IP_NDSERVER_ProcTimerMsgPeriodicNsExp(timerIndex);
            break;
        case IP_ND_SERVER_TIMER_FIRST_NS:
            IP_NDSERVER_ProcTimerMsgFirstNsExp(timerIndex);
            break;
        case IP_ND_SERVER_TIMER_RA:
            IP_NDSERVER_ProcTimerMsgRaExp(timerIndex);
            break;
        default:
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: Illegal Timer Type!");
            break;
    }

    return IP_MSG_HANDLED;
}

/*
 * ��������: PP NDIS����PID��Ϣ������
 * �޸���ʷ:
 *  1.��    ��: 2012��12��9��
 *    �޸�����: �����ɺ���
 */
VOS_VOID APP_NdServer_PidMsgProc(MsgBlock *rcvMsg)
{
    if (rcvMsg == VOS_NULL_PTR) {
        PS_PRINTF_INFO("Error:APP_Ndis_DLPidMsgProc Parameter pRcvMsg is NULL!");
        return;
    }
    switch (rcvMsg->ulSenderPid) {
        case DOPRA_PID_TIMER:
            (VOS_VOID)IP_NDSERVER_TimerMsgDistr(rcvMsg);
            break;
        default:
            break;
    }
    return;
}

/*
 * Function Name: IP_NDSERVER_CmdHelp
 * Description: IP NDSERVERģ��������ʾ
 * History:
 *    1.      2011-04-11  Draft Enact
 */
VOS_VOID IP_NDSERVER_CmdHelp(VOS_VOID)
{
    PS_PRINTF_ERR("\r\n");
    PS_PRINTF_ERR("********************** IP NDSERVER CMD LIST*********************\r\n");
    PS_PRINTF_ERR("%-30s : %s\r\n", "IP_NDSERVER_ShowLocalNwParamInfo", "show local net para");
    PS_PRINTF_ERR("%-30s : %s\r\n", "IP_NDSERVER_ShowStatInfo(index)", "show stat info(0)");
    PS_PRINTF_ERR("*******************************************************************\r\n");
    PS_PRINTF_ERR("\r\n");

    return;
}

/*
 * Function Name: IP_NDSERVER_ShowLocalNwParamInfo
 * Description: ��ʾ���ر�������������Ϣ
 * History:
 *    1.      2011-04-11  Draft Enact
 */
VOS_VOID IP_NDSERVER_ShowLocalNwParamInfo(VOS_VOID)
{
    PS_PRINTF_ERR("************************Local Nw Param Info***********************\r\n");
    PS_PRINTF_ERR("manage addr config flag %d\r\n", g_ndServerMFlag);
    PS_PRINTF_ERR("other stated config flag %d\r\n", g_ndServerOFlag);
    PS_PRINTF_ERR("router lifetime(s) %d\r\n", g_routerLifetime);
    PS_PRINTF_ERR("Reachable Time(ms) %d\r\n", g_reachableTime);
    PS_PRINTF_ERR("Retrans Timer(ms) %d\r\n", g_retransTimer);
    PS_PRINTF_ERR("Neighbor solicition TimerLen %d\r\n", g_nsTimerLen);
    PS_PRINTF_ERR("Neighbor solicition Timer Max Timerout Num %d\r\n", g_nsTimerMaxExpNum);
    PS_PRINTF_ERR("Periodic NsTimer Len %d\r\n", g_periodicNsTimerLen);
    PS_PRINTF_ERR("PeriodicRaTimerLen(ms) %d\r\n", g_periodicRaTimerLen);
    PS_PRINTF_ERR("FirstNsTimerLen(ms) %d\r\n", g_firstNsTimerLen);
    PS_PRINTF_ERR("RaTimerLen(ms) %d\r\n", g_raTimerLen);

    return;
}

/*
 * Function Name: IP_NDSERVER_ShowStatInfo
 * Description: ��ʾ����ͳ����Ϣ
 * Input: ulIndex --- ʵ������
 * History:
 *    1.      2011-04-11  Draft Enact
 */
VOS_VOID IP_NDSERVER_ShowStatInfo(VOS_UINT32 addrIndex)
{
    IP_NdserverPacketStatisticsInfo *pktStatInfo = IP_NULL_PTR;
    VOS_UINT32                       tmp;

    if (addrIndex >= IP_NDSERVER_ADDRINFO_MAX_NUM) {
        tmp = IP_NDSERVER_ADDRINFO_MAX_NUM;
        PS_PRINTF_ERR("IP_NDSERVER_ShowStatInfo:input para range:0-%d\r\n", tmp - 1);
        return;
    }

    pktStatInfo = IP_NDSERVER_GET_STATINFO_ADDR(addrIndex);

    PS_PRINTF_ERR("****************************packet stat info(%d)****************************\r\n", addrIndex);
    PS_PRINTF_ERR("Receive Packet Total Num:            %d\r\n", pktStatInfo->rcvPktTotalNum);
    PS_PRINTF_ERR("Discard Packet Num:                  %d\r\n", pktStatInfo->discPktNum);
    PS_PRINTF_ERR("RcvNsPktNum:                         %d\r\n", pktStatInfo->rcvNsPktNum);
    PS_PRINTF_ERR("ulRcvNaPktNum :                      %d\r\n", pktStatInfo->rcvNaPktNum);
    PS_PRINTF_ERR("RcvRsPktNum:                         %d\r\n", pktStatInfo->rcvRsPktNum);
    PS_PRINTF_ERR("RcvEchoPktNum:                       %d\r\n", pktStatInfo->rcvEchoPktNum);
    PS_PRINTF_ERR("RcvTooBigPktNum:                     %d\r\n", pktStatInfo->rcvTooBigPktNum);
    PS_PRINTF_ERR("RcvDhcpv6PktNum:                     %d\r\n", pktStatInfo->rcvDhcpv6PktNum);
    PS_PRINTF_ERR("ErrNsPktNum:                         %d\r\n", pktStatInfo->errNsPktNum);
    PS_PRINTF_ERR("ErrNaPktNum:                         %d\r\n", pktStatInfo->errNaPktNum);
    PS_PRINTF_ERR("ErrRsPktNum :                        %d\r\n", pktStatInfo->errRsPktNum);
    PS_PRINTF_ERR("ErrEchoPktNum:                       %d\r\n", pktStatInfo->errEchoPktNum);
    PS_PRINTF_ERR("ErrTooBigPktNum:                     %d\r\n", pktStatInfo->errTooBigPktNum);
    PS_PRINTF_ERR("ErrDhcpv6PktNum:                     %d\r\n", pktStatInfo->errDhcpv6PktNum);
    PS_PRINTF_ERR("TransPktTotalNum:                    %d\r\n", pktStatInfo->transPktTotalNum);
    PS_PRINTF_ERR("TransPktFailNum:                     %d\r\n", pktStatInfo->transPktFailNum);
    PS_PRINTF_ERR("TransNsPktNum:                       %d\r\n", pktStatInfo->transNsPktNum);
    PS_PRINTF_ERR("TransNaPktNum :                      %d\r\n", pktStatInfo->transNaPktNum);
    PS_PRINTF_ERR("TransRaPktNum :                      %d\r\n", pktStatInfo->transRaPktNum);
    PS_PRINTF_ERR("TransEchoPktNum :                    %d\r\n", pktStatInfo->transEchoPktNum);
    PS_PRINTF_ERR("TransTooBigPktNum :                  %d\r\n", pktStatInfo->transTooBigPktNum);
    PS_PRINTF_ERR("TransDhcpv6PktNum :                  %d\r\n", pktStatInfo->transDhcpv6PktNum);
    PS_PRINTF_ERR("MacInvalidPktNum :                   %d\r\n", pktStatInfo->macInvalidPktNum);
    PS_PRINTF_ERR("EnquePktNum :                        %d\r\n", pktStatInfo->enquePktNum);
    PS_PRINTF_ERR("EnquePktNum :                        %d\r\n", pktStatInfo->sendQuePktNum);

    return;
}

