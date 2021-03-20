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
/* ND SERVER数据信息 */
IP_NdserverAddrInfo     g_ndServerAddrInfo[IP_NDSERVER_ADDRINFO_MAX_NUM];
IP_NdserverTeDetectBuf g_ndServerTeDetectBuf;

/* ND SERVER本地保存的配置参数 */
VOS_UINT8  g_ndServerMFlag;          /* M标识 */
VOS_UINT8  g_ndServerOFlag;          /* O标识 */
VOS_UINT16 g_routerLifetime; /* Router Lifetime */
VOS_UINT32 g_reachableTime;  /* Reachable Time */
VOS_UINT32 g_retransTimer;   /* Retrans Timer */

VOS_UINT32 g_nsTimerLen;         /* 非周期性NS定时器时长 */
VOS_UINT32 g_nsTimerMaxExpNum;   /* 非周期性NS最大超时次数 */
VOS_UINT32 g_periodicNsTimerLen; /* 周期性NS定时器时长 */
VOS_UINT32 g_periodicRaTimerLen; /* 周期性RA定时器时长，防止Router过期 */
VOS_UINT32 g_firstNsTimerLen;    /* 收到重复地址检测后等待的定时器时长 */
VOS_UINT32 g_raTimerLen;         /* 收到重复地址检测前周期发送RA定时器时长 */

/* 周期性发送RA时间计数器 */
VOS_UINT32 g_periodicRaTimeCnt[IP_NDSERVER_ADDRINFO_MAX_NUM];

/* 用于存储解析后的ND报文数据 */
IP_ND_Msg g_ndMsgData;

/* ND SERVER使用的发包缓冲区 */
VOS_UINT8 g_ndserverSendMsgBuffer[IP_IPM_MTU];

/* ND SERVER报文统计信息 */
IP_NdserverPacketStatisticsInfo g_ndServerPktStatInfo[IP_NDSERVER_ADDRINFO_MAX_NUM];

/*
 * 功能描述: 临时保存Te IP地址，在TE不发重复地址检测的情况下用来获取TE MAC地
 *             址
 * 修改历史:
 *  1.日    期: 2011年5月17日
 *    修改内容: 新生成函数
 */
STATIC VOS_VOID IP_NDSERVER_SaveTeDetectIp(const VOS_UINT8 *teGlobalAddr)
{
    errno_t    rlt;
    VOS_UINT32 bufIndex = g_ndServerTeDetectBuf.head;

    if ((IP_IPV6_IS_LINKLOCAL_ADDR(teGlobalAddr)) || (IP_IPV6_64BITPREFIX_EQUAL_ZERO(teGlobalAddr)) ||
        (IP_IPV6_IS_MULTICAST_ADDR(teGlobalAddr))) {
        /* 不是Global IP */
        return;
    }

    /* 相同Prefix的地址是否已经存在 */
    while (bufIndex != g_ndServerTeDetectBuf.tail) {
        /*lint -e960*/
        if ((g_ndServerTeDetectBuf.teIpBuf[bufIndex].valid == IP_TRUE) &&
            (IP_MEM_CMP(teGlobalAddr, g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr,
                        ND_IP_IPV6_PREFIX_LENGTH) == 0)) {
            /*lint +e960*/
            /* 相同Prefix只保留一个地址 */
            rlt = memcpy_s(g_ndServerTeDetectBuf.teIpBuf[bufIndex].teGlobalAddr, IP_IPV6_ADDR_LEN,
                           teGlobalAddr, IP_IPV6_ADDR_LEN);
            IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
            return;
        }
        bufIndex = TTF_MOD_ADD(bufIndex, 1, g_ndServerTeDetectBuf.maxNum);
    }

    if (g_ndServerTeDetectBuf.head ==
        TTF_MOD_ADD(g_ndServerTeDetectBuf.tail, 1, g_ndServerTeDetectBuf.maxNum)) {
        /* BUF满时覆盖最老的地址 */
        g_ndServerTeDetectBuf.head = TTF_MOD_ADD(g_ndServerTeDetectBuf.head, 1,
                                                 g_ndServerTeDetectBuf.maxNum);
    }

    /* 保存IP地址 */
    rlt = memcpy_s(g_ndServerTeDetectBuf.teIpBuf[g_ndServerTeDetectBuf.tail].teGlobalAddr,
                   IP_IPV6_ADDR_LEN, teGlobalAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    g_ndServerTeDetectBuf.teIpBuf[g_ndServerTeDetectBuf.tail].valid = IP_TRUE;

    g_ndServerTeDetectBuf.tail = TTF_MOD_ADD(g_ndServerTeDetectBuf.tail, 1,
                                             g_ndServerTeDetectBuf.maxNum);
}

/*
 * 功能描述: 获取临时保存的TE IP地址
 * 修改历史:
 *  1.日    期: 2011年5月17日
 *    修改内容: 新生成函数
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

            /* Buf头部空隙处理 */
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
 * Description: 设置本地网络参数
 * History:
 *    1.      2011-04-01  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_SetLocalParam(VOS_VOID)
{
    /* M标识，0，主机不能通过DHCPv6获取IPv6地址 */
    g_ndServerMFlag = 0;

    /* O标识，1，主机可以通过无状态DHCPv6获取其他参数，例如DNS服务器，SIP服务器； */
    g_ndServerOFlag = 1;

    /* Router Lifetime(秒) */
    g_routerLifetime = 9000; /* 路由生存期9000s */

    /* 3600000:Reachable Time(毫秒) */
    g_reachableTime = 3600000;

    /* Retrans Timer(毫秒)，0，表示未知 */
    g_retransTimer = 0;

    /* 4000:非周期性NS定时器时长(毫秒) */
    g_nsTimerLen = 4000;

    /* 3:非周期性NS最大超时次数 */
    g_nsTimerMaxExpNum = 3;

    /* 60000:周期性NS定时器时长(毫秒) */
    g_periodicNsTimerLen = 60000;

    /* 3600000:周期性RA，防止Router过期(毫秒) */
    g_periodicRaTimerLen = 3600000;

    /* 2000:收到重复地址检测后等待的定时器时长(毫秒) */
    g_firstNsTimerLen = 2000;

    /* 15000:收到重复地址检测前周期发送RA的定时器时长(毫秒) */
    g_raTimerLen = 15000;

    return;
}

/*
 * Description: 清除统计信息
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
 * Description: 判断定时器名是否合法
 * Input: ulIndex     --- ND SERVER数据体索引号
 *                   enTimerType --- 定时器类型
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
 * Description: 获取定时器
 * Input: enTimerType------------------定时器类型
 *                   ulIndex----------------------定时器索引号
 * History:
 *    1.      2011-04-02  Draft Enact
 */
IP_TimerInfo *IP_NDSERVER_GetTimer(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    IP_TimerInfo *timerInfo = VOS_NULL_PTR;

    /* 根据定时器不同类型，获取定时器 */
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
 * Description: 获取定时器时长
 * Input: enTimerType------------------定时器类型
 * History:
 *    1.      2011-04-02  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_GetTimerLen(NDSER_TimerUint32 timerType)
{
    VOS_UINT32 timerLen;

    /* 根据定时器不同类型，定时器时长不同 */
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
 * Description: 打印定时器启动信息
 * Input: enTimerType------------------定时器类型
 *                   ulIndex----------------------定时器索引号
 * History:
 *    1.      2010-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_PrintTimeStartInfo(VOS_UINT32 addrIndex, NDSER_TimerUint32 timerType)
{
    /* 根据定时器不同类型，打印相应信息 */
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
 * Description: 打印定时器关闭信息
 * Input: enTimerType------------------定时器类型
 *                   ulIndex----------------------定时器索引号
 * History:
 *    1.      2010-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_PrintTimeStopInfo(VOS_UINT32 addrIndex, NDSER_TimerUint32 timerType)
{
    /* 根据定时器不同类型，打印相应信息 */
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
 * Discription: 启动某一承载的某种类型的定时器
 * History:
 *      1.      2011-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_TimerStart(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    VOS_UINT32     timerLen;
    IP_TimerInfo  *timerInfo = VOS_NULL_PTR;

    /* 对ulIndex合法性判断 */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* 打印异常信息 */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart: WARN: Input Para(ulIndex) err !", timerIndex,
                          timerType);
        return;
    }

    /* 根据消息对应的索引号和定时器类型,获取相关联的定时器 */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* 打印异常信息 */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:ERROR: Get Timer failed.");
        return;
    }

    /* 判断定时器是否打开，已打开则关闭 */
    if (timerInfo->hTm != VOS_NULL_PTR) {
        /* 关闭失败，则报警返回 */
        if (PS_STOP_REL_TIMER(&(timerInfo->hTm)) != VOS_OK) {
            /* 打印异常信息 */
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:WARN: stop reltimer error!");
            return;
        }

        /* 打印异常信息 */
        IPND_INFO_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:(TimerType) Timer not close!", timerInfo->timerName,
                       timerIndex);
    }

    /* 根据定时器不同类型，定时器信息不同 */
    timerLen = IP_NDSERVER_GetTimerLen(timerType);
    if (timerLen == IP_NULL) {
        /* 打印异常信息 */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:ERROR: start unreasonable reltimer.");
        return;
    }

    /* 设定定时器NAME,enTimerType设定定时器Para，打开失败则报警返回 */
    if (PS_START_REL_TIMER(&(timerInfo->hTm), NDIS_NDSERVER_PID, timerLen, (VOS_UINT32)timerType,
                           timerIndex, VOS_RELTIMER_NOLOOP) != VOS_OK) {
        /* 打印异常信息 */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStart:WARN: start reltimer error!");
        return;
    }

    /* 更新定时器类别 */
    timerInfo->timerName = timerType;

    /* 打印定时器启动信息 */
    IP_NDSERVER_PrintTimeStartInfo(timerIndex, timerType);

    return;
}

/*
 * Discription: 停止某一承载某种类型定时器
 * Return: None
 *      1.      2011-04-02  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_TimerStop(VOS_UINT32 timerIndex, NDSER_TimerUint32 timerType)
{
    IP_TimerInfo *timerInfo = VOS_NULL_PTR;

    /* 对ulIndex合法性判断 */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* 打印异常信息 */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop: WARN: Input Para(ulIndex) err !", timerIndex,
                          timerType);
        return;
    }

    /* 根据消息对应的索引号和定时器类型,获取相关联的定时器 */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* 打印异常信息 */
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop:ERROR:Get Timer failed.");
        return;
    }

    /* 定时器处于打开状态，则关闭，否则，忽略 */
    if (timerInfo->hTm != VOS_NULL_PTR) {
        if (timerType != timerInfo->timerName) {
            /* 打印异常信息 */
            IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop: TimerType not match:", timerType,
                              timerInfo->timerName);
        }
        /* 关闭失败，则报警返回 */
        if (PS_STOP_REL_TIMER(&(timerInfo->hTm)) != VOS_OK) {
            /* 打印异常信息 */
            IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerStop:WARN: stop reltimer error!");
            return;
        }

        /* 清除超时计数 */
        timerInfo->loopTimes = 0;

        /* 打印定时器关闭信息 */
        IP_NDSERVER_PrintTimeStopInfo(timerIndex, timerType);
    }

    return;
}

/*
 * Description: ND SERVER模块的初始化
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

        /* payload长度为2个字节 */
        rlt = memcpy_s((IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex) + 2 * IP_MAC_ADDR_LEN),
                       (IP_ETH_MAC_HEADER_LEN - 2 * IP_MAC_ADDR_LEN), (VOS_UINT8 *)(&payLoad), 2); /* 2:byte */
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

        g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
        /* 创建下行IP包缓存队列 */
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
 * 功能描述: ND SERVER PID初始化函数
 * 修改历史:
 *  1.日    期: 2011年12月7日
 *    修改内容: 新生成函数
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
 * Description: 清除ND SERVER相关信息
 * Input: ulIndex --- ND SERVER结构索引号
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

    /* 清除TE地址记录 */
    (VOS_VOID)memset_s(&infoAddr->teAddrInfo, sizeof(IP_NdserverTeAddrInfo), IP_NULL,
                       sizeof(IP_NdserverTeAddrInfo));

    IP_NDSERVER_TimerStop(addrIndex, infoAddr->timerInfo.timerName);

    (VOS_VOID)memset_s(&infoAddr->ipSndNwMsg, sizeof(IP_SndMsg), IP_NULL, sizeof(IP_SndMsg));

    g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;

    return;
}

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
/*
 * Description: 释放下行缓存队列中的PKT
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
 * Description: 处理NDIS发来的Ipv6PdnRel
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
        IP_NDSERVER_ClearDlPktQue(addrIndex); /* 释放下行PKT缓存队列 */
#endif
    } else {
        IPND_ERROR_LOG2(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnRel: Invalid Input:", addrIndex, exRabId);
    }

    return;
}

/*
 * Description: 供NDIS调用，查询对应承载的ND SERVER实体是否还有效
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
 * Description: 供NDIS调用，获取ND SERVER对应实体的完整MAC帧头
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
 * Description: 供NDIS调用，当未获得PC MAC地址时，缓存下行IP包
 * History: VOS_UINT8
 *    1.      2011-12-06  Draft Enact
 */
VOS_VOID NdSer_MacAddrInvalidProc(IMM_Zc *immZc, VOS_UINT8 addrIndex)
{
    IMM_Zc      *queHead = VOS_NULL_PTR;
    VOS_INT32    lockKey;

    IP_NDSERVER_AddMacInvalidPktNum(addrIndex);

    /* **********************下行IPV6数据包缓存******************************** */
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
        IMM_ZcFree(queHead); /* 释放最早的IP包 */
        /*lint +e522*/
    }

    lockKey = VOS_SplIMP();
    if (LUP_EnQue(IP_NDSERVER_ADDRINFO_GET_DLPKTQUE(addrIndex), immZc) != PS_SUCC) { /* 插入最新的IP包 */
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
 * Description: 查找第一个可用的IPv6前缀
 * Input: pstConfigParaInd ---- ESM_IP_NW_PARA_IND_STRU消息指针
 * Output: pulPrefixIndex   ---- 前缀索引指针
 * History:
 *      1.     2011-04-05  Draft Enact
 */
STATIC VOS_UINT32 IP_NDSERVER_FindIpv6EffectivePrefix(const AT_NDIS_Ipv6PdnInfo *configParaInd,
                                                      VOS_UINT32 *prefixIndex)
{
    VOS_UINT32              loopIndex;
    ND_IP_Ipv6Prefix       *ipv6Prefix = IP_NULL_PTR;

    /* 遍历前缀列表，查找A标识为1，前缀长度为64的前缀,prefixLen单位是bit */
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
 * Description: 从ID_ESM_IP_NW_PARA_IND中获取网络参数
 * Input: pstNwParaInd--------------消息指针
 * Output: pstNwParamTmp-------------网络参数指针
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

    /* 获取HOP LIMIT */
    nwParamTmp->curHopLimit = nwParaInd->curHopLimit;

    /* 获取MTU */
    if (nwParaInd->OpMtu == IP_IPV6_OP_TRUE) {
        nwParamTmp->opMtu = nwParaInd->OpMtu;
        nwParamTmp->mtuValue      = nwParaInd->Mtu;
    }

    /* 获取接口标识符 */
    rlt = memcpy_s(nwParamTmp->interfaceId, ND_IP_IPV6_IFID_LENGTH, nwParaInd->interfaceId,
                   ND_IP_IPV6_IFID_LENGTH);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 保存地址前缀列表，目前只保存和使用第一个可用的IPv6前缀 */
    rslt = IP_NDSERVER_FindIpv6EffectivePrefix(nwParaInd, &prefixIndex);
    if (rslt != IP_SUCC) {
        return IP_FAIL;
    }

    rlt = memcpy_s(&nwParamTmp->prefixList[IP_NULL], sizeof(ND_IP_Ipv6Prefix),
                   &nwParaInd->prefixList[prefixIndex], sizeof(ND_IP_Ipv6Prefix));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    nwParamTmp->prefixNum = 1;

    /* 获取DNS服务器列表 */
    rlt = memcpy_s(nwParamTmp->dnsSer.priDnsServer, IP_IPV6_ADDR_LEN, nwParaInd->dnsSer.priServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    rlt = memcpy_s(nwParamTmp->dnsSer.secDnsServer, IP_IPV6_ADDR_LEN, nwParaInd->dnsSer.secServer,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    nwParamTmp->dnsSer.dnsSerNum = nwParaInd->dnsSer.serNum;

    /* 获取SIP服务器列表 */
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
 * Description: RA报文头固定部分设置
 * Input: ulIndex --- ND SERVER实体索引
 * Output: pucData --- 报文缓冲指针
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormRaHeaderMsg(VOS_UINT32 addrIndex, VOS_UINT8 *data)
{
    ESM_IP_Ipv6NWPara *nwPara = IP_NULL_PTR;

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    nwPara = IP_NDSERVER_ADDRINFO_GET_NWPARA(addrIndex);

    /* 类型 */
    *data = IP_ICMPV6_TYPE_RA;
    data++;

    /* 代码 */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:跳过校验和 */
    data += 2;

    /* 当前跳限制 */
    *data = nwPara->curHopLimit;
    data++;

    /* 7 6:管理地址配置标志、其他有状态配置标志 */
    *data = (VOS_UINT8)(((g_ndServerMFlag & 0x1) << 7) | ((g_ndServerOFlag & 0x1) << 6));
    data++;

    /* 路由生存期 */
    IP_SetUint16Data(data, g_routerLifetime);
    data += 2; /* 路由生存周期需要占2个字节 */

    /* 可达时间 */
    IP_SetUint32Data(data, g_reachableTime);
    data += 4; /* 可达时间需要占4个字节 */

    /* 重发定时器 */
    IP_SetUint32Data(data, g_retransTimer);

    return;
}

/*
 * Function Name: IP_NDSERVER_FormRaOptMsg
 * Description: RA报文选项部分设置
 * Input: ulIndex --- ND SERVER实体索引
 *                   pucMacAddr- 源链路层地址
 * Output: pucData --- 报文缓冲指针
 *                   pulLen ---- 报文长度指针
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

    /* 类型 */
    *data = IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR;
    data++;

    /* 长度 */
    *data = 1;
    data++;

    /* 链路层地址 */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    (*len) += IP_ND_OPT_UNIT_LEN;

    if (nwPara->opMtu == IP_IPV6_OP_TRUE) {
        /* 类型 */
        *data = IP_ICMPV6_OPT_MTU;
        data++;

        /* 长度 */
        *data = 1;
        data++;

        /* 2:保留 */
        data += 2;

        /* MTU: 取NV中的MTU和RA中的MTU的最小值作为发给PC的MTU */
        tmpMtu = PS_MIN(g_nvMtu, nwPara->mtuValue);
        IP_SetUint32Data(data, tmpMtu);
        data += 4; /* MTU长度占4个字节 */

        (*len) += IP_ND_OPT_UNIT_LEN;
    }

    for (count = 0; count < nwPara->prefixNum; count++) {
        /* 类型 */
        *data = IP_ICMPV6_OPT_PREFIX_INFO;
        data++;

        /* 4:长度 */
        *data = 4;
        data++;

        /* 前缀长度 */
        *data = (VOS_UINT8)(nwPara->prefixList[count].prefixLen);
        data++;

        /* 链路上标志、自治标志 */
        *data = (VOS_UINT8)(((nwPara->prefixList[count].prefixLValue & 0x1) << 7) | /* 7:bit */
                            ((nwPara->prefixList[count].prefixAValue & 0x1) << 6)); /* 6:bit */
        data++;

        /* 有效生存期 */
        IP_SetUint32Data(data, nwPara->prefixList[count].validLifeTime);
        data += 4; /* 有效生存期占4个字节 */

        /* 选用生存期 */
        IP_SetUint32Data(data, nwPara->prefixList[count].preferredLifeTime);
        data += 4; /* 选用生存期占4个字节 */

        /* 4:保留字段 */
        data += 4;

        /* 前缀 */
        rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, nwPara->prefixList[count].prefix, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
        data += IP_IPV6_ADDR_LEN;

        (*len) += (4 * IP_ND_OPT_UNIT_LEN); /* 4:len */
    }

    return;
}

/*
 * Function Name: IP_NDSERVER_FormEtherHeaderMsg
 * Description: IPv6报头设置
 * Input: aucSrcMacAddr --- 源MAC地址
 *                   aucDstMacAddr --- 目的MAC地址
 * Output: pucData --- 报文缓冲指针
 * History:
 *    1.      2011-04-07  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormEtherHeaderMsg(const ND_MacAddrInfo srcMacAddr, const ND_MacAddrInfo dstMacAddr,
                                               VOS_UINT8 *data)
{
    VOS_INT32 rlt;
    /* 目的MAC */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, dstMacAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    /* 源MAC */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, srcMacAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    data += IP_MAC_ADDR_LEN;

    /* 类型 */
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
 * Description: 生成路由公告消息
 * Input: ulIndex ------- 处理RA发包实体索引
 *                   pstNdMsgData -- 目的信息参数
 * Output: pucSendBuff --- 发送RA报文缓冲
 *                   pulSendLen ---- 发送报文长度
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
    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg is entered.");
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);
    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);

    data += IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* 设置RA报头 */
    IP_NDSERVER_FormRaHeaderMsg(addrIndex, data);

    packetLen += IP_ICMPV6_RA_HEADER_LEN;

    /* 根据自身MAC地址生成link-local地址 */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);
    rlt = memcpy_s(infoAddr->ueLinkLocalAddr, IP_IPV6_ADDR_LEN, srcIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* 设置RA报文选项 */
    IP_NDSERVER_FormRaOptMsg(addrIndex, srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    /* 确定单播或组播方式 */
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

    /* 设置IPv6报头 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* 生成ICMPv6报头校验和 */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* 设置以太报头 */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* 返回报文长度 */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendRaMsg
 * Description: 发送路由公告消息
 * Input: ulIndex ------- 处理RA发包实体索引
 *                   pstNdMsgData -- 目的信息参数
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

    /* 调用形成RA消息函数 */
    IP_NDSERVER_FormRaMsg(addrIndex, ndMsgData, sendBuff, &sendLen);

    IP_NDSERVER_AddTransRaPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    /* 将RA消息发送到PC */
    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * Function Name: IP_NDSERVER_FormNaHeaderMsg
 * Description: NA报文头固定部分设置
 * Input: pucTargetIPAddr --- 目的IP地址
 *                   ucSolicitFlag   --- 请求标志
 * Output: pucData --- 报文缓冲指针
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNaHeaderMsg(const ND_Ipv6AddrInfo targetIPAddr, VOS_UINT8 solicitFlag, VOS_UINT8 *data)
{
    VOS_INT32 rlt;
    /* 类型 */
    *data = IP_ICMPV6_TYPE_NA;
    data++;

    /* 代码 */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:跳过校验和 */
    data += 2;

    /* 6:路由器标志、请求标志、覆盖标志 */
    *data = (VOS_UINT8)(0xa0 | ((solicitFlag & 0x1) << 6));
    data++;

    /* 3:保留 */
    data += 3;

    /* 目标地址 */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, targetIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    return;
}

/*
 * Description: NA报文选项部分设置
 * Input: pucMacAddr- 源链路层地址
 * Output: pucData --- 报文缓冲指针
 *                   pulLen ---- 报文长度指针
 * History:
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNaOptMsg(const ND_MacAddrInfo macAddr, VOS_UINT8 *data, VOS_UINT32 *len)
{
    VOS_INT32 rlt;
    /* 类型 */
    *data = IP_ICMPV6_OPT_TGT_LINK_LAYER_ADDR;
    data++;

    /* 长度 */
    *data = 1;
    data++;

    /* 链路层地址 */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    (*len) += IP_ND_OPT_UNIT_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNaMsg
 * Description: 生成邻居公告消息
 * Input: ulIndex ------- 处理NA发包实体索引
 *                   pstNdMsgData -- 目的信息参数
 * Output: pucSendBuff --- 发送NA报文缓冲
 *                   pulSendLen ---- 发送报文长度
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
    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNaMsg is entered.");
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);

    /* 根据自身MAC地址作为源MAC */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    if (ndMsgData == VOS_NULL_PTR) {
        /* 根据自身MAC地址生成link-local地址 */
        IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
        IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);
    } else {
        /* 使用目标地址作为源IP */
        rlt = memcpy_s(srcIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ndMsgData->ndMsgStru.nsInfo.targetAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    }

    /* 确定单播或组播方式 */
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

    /* 设置NA报头 */
    IP_NDSERVER_FormNaHeaderMsg(srcIPAddr, solicitFlag, data);

    packetLen += IP_ICMPV6_NA_HEADER_LEN;

    /* 设置NA可选项 */
    IP_NDSERVER_FormNaOptMsg(srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    data -= IP_IPV6_HEAD_LEN;

    /* 设置IPv6报头 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* 生成ICMPv6校验和 */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNaMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* 设置以太报头 */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* 返回报文长度 */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendNaMsg
 * Description: 发送邻居公告消息
 * Input: ulIndex ------- 处理NA发包实体索引
 *                   pstNdMsgData -- 目的信息参数
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

    /* 调用形成NA消息函数 */
    IP_NDSERVER_FormNaMsg(ndMsgData, sendBuff, &sendLen);

    IP_NDSERVER_AddTransNaPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * Function Name: IP_NDSERVER_FormNsHeaderMsg
 * Description: NS报文头固定部分设置
 * Input: ulIndex --- ND SERVER实体索引
 * Output: pucData --- 报文缓冲指针
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNsHeaderMsg(VOS_UINT32 addrIndex, VOS_UINT8 *data, const VOS_UINT8 *dstAddr)
{
    VOS_INT32 rlt;
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    /* 类型 */
    *data = IP_ICMPV6_TYPE_NS;
    data++;

    /* 代码 */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* 2:跳过校验和 */
    data += 2;

    /* 4:保留 */
    data += 4;

    /* 目标地址 */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, dstAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNsOptMsg
 * Description: NS报文选项部分设置
 * Input: pucMacAddr- 源链路层地址
 * Output: pucData --- 报文缓冲指针
 *                   pulLen ---- 报文长度指针
 *    1.      2011-04-08  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_FormNsOptMsg(const ND_MacAddrInfo macAddr, VOS_UINT8 *data, VOS_UINT32 *len)
{
    VOS_INT32 rlt;
    /* 类型 */
    *data = IP_ICMPV6_OPT_SRC_LINK_LAYER_ADDR;
    data++;

    /* 长度 */
    *data = 1;
    data++;

    /* 链路层地址 */
    rlt = memcpy_s(data, IP_MAC_ADDR_LEN, macAddr.macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    (*len) += IP_ND_OPT_UNIT_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_FormNsMsg
 * Description: 生成地址解析邻居请求消息
 * Input: ulIndex ------- 处理NS发包实体索引
 * Output: pucSendBuff --- 发送NS报文缓冲
 *                   pulSendLen ---- 发送报文长度
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
    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNsMsg is entered.");

    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);

    /* 根据自身MAC地址生成link-local地址 */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* 根据目的IP生成请求节点组播地址 */
    IP_ProduceSolicitedNodeMulticastIPAddr(dstIPAddr.ipv6Addr, dstAddr);
    IP_ProduceSolicitedNodeMulticastMacAddr(dstMacAddr.macAddr, dstAddr);

    data += IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* 设置NS报头 */
    IP_NDSERVER_FormNsHeaderMsg(addrIndex, data, dstAddr);

    packetLen += IP_ICMPV6_NS_HEADER_LEN;

    /* 设置NS可选项 */
    IP_NDSERVER_FormNsOptMsg(srcMacAddr, (VOS_VOID *)(data + packetLen), &packetLen);

    data -= IP_IPV6_HEAD_LEN;

    /* 设置IPv6报头 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, packetLen, data, IP_HEAD_PROTOCOL_ICMPV6);

    /* 生成ICMPv6校验和 */
    if (IP_BuildIcmpv6Checksum(data, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormNsMsg: Build ICMPv6 Checksum failed.");
    }

    data -= IP_ETHERNET_HEAD_LEN;

    /* 设置以太报头 */
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, data);

    /* 返回报文长度 */
    *sendLen = packetLen + IP_IPV6_HEAD_LEN + IP_ETHERNET_HEAD_LEN;

    return;
}

/*
 * Function Name: IP_NDSERVER_SendNsMsg
 * Description: 发送地址解析邻居请求消息
 * Input: ulIndex ------- 处理NS发包实体索引
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

    /* 调用形成NS消息函数 */
    IP_NDSERVER_FormNsMsg(addrIndex, sendBuff, &sendLen, dstAddr);

    IP_NDSERVER_AddTransNsPktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    return Ndis_SendMacFrm(sendBuff, sendLen, (VOS_UINT8)epsbId);
}

/*
 * 功能描述: 根据ExRabId查找NDSERVER实体的索引
 * 修改历史:
 *  1.日    期: 2013年1月15日
 *    修改内容: 新生成函数
 */
VOS_UINT32 NdSer_GetAddrInfoIdx(VOS_UINT8 exRabId)
{
    VOS_UINT32 i = 0;

    /* 查询是否已存在相应Entity */
    do {
        if ((PS_TRUE == g_ndServerAddrInfo[i].validFlag) && (exRabId == g_ndServerAddrInfo[i].epsbId)) {
            /* 找到相应实体 */
            return i;
        }
    } while ((++i) < IP_NDSERVER_ADDRINFO_MAX_NUM);

    return IP_NDSERVER_ADDRINFO_MAX_NUM;
}

/*
 * 功能描述: 分配一个空闲的NDSERVER实体
 * 修改历史:
 *  1.日    期: 2013年1月15日
 *    修改内容: 新生成函数
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
            /* 找到空闲实体 */
            *pUlIndex = i;
            return &g_ndServerAddrInfo[i];
        }
    } while ((++i) < IP_NDSERVER_ADDRINFO_MAX_NUM);

    return IP_NULL_PTR;
}

/*
 * 功能描述: PDN IPV6地址信息检查
 * 修改历史:
 *  1.日    期: 2011年12月11日
 *    修改内容: 新生成函数
 *  2.日    期: 2013年1月15日
 *    修改内容: DSDA开发，使用ExRabId来映射NdSer实体
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

    /* 遍历前缀列表，查找A标识为1，前缀长度为64的前缀，prefixLen单位为bit */
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
    /* MT关机再开机后，ND SERVER收到网侧RA前，可能先收到TE的重复地址检测NS */
    if (ipV6 != IP_NULL_PTR) {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Using saved TeAddr to get MAC.");
        rlt = memcpy_s(infoAddr->teAddrInfo.teGlobalAddr, IP_IPV6_ADDR_LEN, ipV6,
                       IP_IPV6_ADDR_LEN);
        if (rlt != EOK) {
            return IP_FAIL;
        }
        infoAddr->teAddrInfo.teAddrState = IP_NDSERVER_TE_ADDR_INCOMPLETE;
        /* 启动收到重复地址检测后的等待定时器 */
        IP_NDSERVER_TimerStart(timerIndex, IP_ND_SERVER_TIMER_FIRST_NS);
    } else {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: None saved TeAddr.");

        /* 启动路由公告定时器 */
        IP_NDSERVER_TimerStart(timerIndex, IP_ND_SERVER_TIMER_RA);
    }
    return IP_SUCC;
}
/*
 * Description: 处理NDIS发来的Ipv6PdnInfoCfg
 */
VOS_VOID NdSer_Ipv6PdnInfoCfg(VOS_UINT8 exRabId, const AT_NDIS_Ipv6PdnInfo *ipv6PdnInfo)
{
    VOS_UINT32                  addrIndex;
    IP_NdserverAddrInfo        *infoAddr  = IP_NULL_PTR;
    ESM_IP_Ipv6NWPara           nwParamTmp;
    VOS_INT32                   rlt;
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcEsmIpNwParaInd is entered.");

    /* 通过扩展RabId查找NDSERVER实体，如果查找不到，则分配一个空闲的 */
    addrIndex = IP_NDSERVER_GET_ADDR_INFO_INDEX(exRabId);
    if (addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM) {
        infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    } else {
        /* 分配一个AddrInfo结构体，并且获得Index */
        infoAddr = NdSer_AllocAddrInfo(&addrIndex);
    }

    if (infoAddr == IP_NULL_PTR) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Get ND Server Entity failed.", exRabId);
        return;
    }

    /* 获取网络参数 */
    if (IP_NDSERVER_GetNwPara(&nwParamTmp, ipv6PdnInfo) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Get Nw Param fail.");
        return;
    }

    /* 判断是否未分配过路由前缀 */
    if (infoAddr->validFlag != IP_TRUE) {
        if (NdSer_RoutePrefixConfirm(&nwParamTmp, infoAddr, addrIndex) == IP_FAIL) {
            return;
        }
    } else if (IP_MEM_CMP(infoAddr->ipv6NwPara.prefixList[0].prefix,
                          nwParamTmp.prefixList[0].prefix,
                          (ND_IP_IPV6_ADDR_LENGTH - ND_IP_IPV6_IFID_LENGTH)) != 0) {
        IPND_INFO_LOG(NDIS_NDSERVER_PID, "NdSer_Ipv6PdnInfoCfg: Valid ND Server get new Nw Param.");

        /* 清除TE地址记录 */
        (VOS_VOID)memset_s(&infoAddr->teAddrInfo, sizeof(IP_NdserverTeAddrInfo), IP_NULL,
                           sizeof(IP_NdserverTeAddrInfo));

        /* 启动路由公告定时器 */
        IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);
    }

    /* 将stNwParamTmp中的内容拷贝到pstInfoAddr->stIpv6NwPara中 */
    rlt = memcpy_s(&infoAddr->ipv6NwPara, sizeof(ESM_IP_Ipv6NWPara), &nwParamTmp,
                   sizeof(ESM_IP_Ipv6NWPara));
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* 设置承载号 */
    infoAddr->epsbId = exRabId;

    /* 置位ND SERVER结构体有效标志 */
    infoAddr->validFlag = IP_TRUE;

    /* 发送RA消息到PC */
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
 * Description: 对RS包进行格式转换
 */
STATIC VOS_UINT32 IP_NDSERVER_DecodeRsData(const VOS_UINT8 *srcData, IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset)
{
    VOS_UINT32         payLoad;
    VOS_INT32          remainLen;
    VOS_UINT32         optType;
    VOS_UINT32         optLen;
    const VOS_UINT8         *ipMsg = srcData;
    IP_ND_MsgRs       *rs    = &destData->ndMsgStru.rsInfo;
    /* 获取PAYLOAD */
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
 * Description: 处理RS消息
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   pucSrcData ------------ IP数据报
 *                   ulIcmpv6HeadOffset ---- ICMPv6报文头偏移量
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

    /* 发送RA消息到PC */
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
 * Description: 对NS包进行格式转换
 */
STATIC VOS_UINT32 IP_NDSERVER_DecodeNsData(const VOS_UINT8 *srcData, IP_ND_Msg *destData, VOS_UINT32 icmpv6HeadOffset)
{
    VOS_UINT32         payLoad;
    VOS_INT32          remainLen;
    VOS_UINT32         optType;
    VOS_UINT32         optLen;
    const VOS_UINT8         *ipMsg = srcData;
    IP_ND_MsgNs       *ns    = &destData->ndMsgStru.nsInfo;

    /* 获取PAYLOAD */
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
 * Description: 判断是否是自己的IP地址
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   aucIPAddr ------------  IP地址指针
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

    /* 根据自身MAC地址生成link-local地址 */
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
 * Description: 判断是否符合已公告的全球前缀
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   aucIPAddr ------------  IP地址指针
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
 * Description: 收到重复地址检测地址
 * Input: ulIndex  -------------- ND SERVER实体索引
 *                   aucIPAddr ------------  IP地址指针
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
    /* 判断是否是重复地址检测NS */
    if ((IP_IPV6_EQUAL_ALL_ZERO(ndMsgData->srcIp)) &&
        (ndMsgData->ndMsgStru.nsInfo.opSrcLinkLayerAddr == 0)) {
        if (IP_NDSERVER_EqualAdvertisedPrefix(addrIndex, ndMsgData->ndMsgStru.nsInfo.targetAddr) == IP_TRUE) {
            IP_NDSERVER_RcvTeDetectionAddr(addrIndex, ndMsgData->ndMsgStru.nsInfo.targetAddr);
            /* 启动收到重复地址检测后的等待定时器 */
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
 * Description: 处理NS消息
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   pucSrcData ------------ IP数据报
 *                   ulIcmpv6HeadOffset ---- ICMPv6报文头偏移量
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
        /* MT关机再开机后，ND SERVER收到网侧RA前，可能先收到TE的重复地址检测NS */
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
        /* 发送NA消息到PC */
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

    /* 获取PAYLOAD */
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
 * Description: 对NA包进行格式转换
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
 * Description: 判断是否是已保存的TE地址
 * Input: ulIndex  -------------  ND SERVER实体索引
 *                   aucIPAddr ------------  IP地址指针
 *                   aucMACAddr -----------  MAC地址指针
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
 * Description: 发送下行缓存的IPV6数据包
 * Input: ulIndex  -------------  ND SERVER实体索引
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

        /* 从 ImmZc中取出RabId和enPdpType */
        /*lint -e522*/
        app     = IMM_ZcGetUserApp(immZc);
        rabId   = (VOS_UINT8)(app & 0xFF); /* 保存的值实际是 扩展RabId */
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
 * Description: 配置硬件MAC地址
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
 * Description: 收到重复地址检测地址
 * Input: ulIndex  -------------  ND SERVER实体索引
 *                   aucGlobalIPAddr ------  全球IP地址指针
 *                   aucMACAddr -----------  MAC地址指针
 *                   aucLinkLocalIPAddr ---  链路本地IP地址指针
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

    /* 更新完整MAC帧头中的目的MAC地址 */
    rlt = memcpy_s(IP_NDSERVER_ADDRINFO_GET_MACFRAME(addrIndex), IP_ETH_MAC_HEADER_LEN, macAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    if (IP_IPV6_IS_LINKLOCAL_ADDR(linkLocalIPAddr)) {
        rlt = memcpy_s(teInfo->teLinkLocalAddr, IP_IPV6_ADDR_LEN, linkLocalIPAddr, IP_IPV6_ADDR_LEN);
        IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    } else {
        (VOS_VOID)memset_s(teInfo->teLinkLocalAddr, IP_IPV6_ADDR_LEN, IP_NULL, IP_IPV6_ADDR_LEN);
    }

    teInfo->teAddrState = IP_NDSERVER_TE_ADDR_REACHABLE;

    /* 配置硬件MAC地址 */
    IP_NDSERVER_UpdateSpeMac(addrIndex, macAddr);

    IPND_WARNING_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_UpdateTeAddrInfo: New Te Addr:", teInfo->teAddrState);
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
    /* 发送下行IP缓存队列中的数据包 */
    IP_NDSERVER_SendDlPkt(addrIndex);
#endif
    return;
}

/*
 * Function Name: IP_NDSERVER_NaMsgProc
 * Description: 处理NA消息
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   pucSrcData ------------ IP数据报
 *                   ulIcmpv6HeadOffset ---- ICMPv6报文头偏移量
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
 * Description: 形成向PC回复的超长包响应
 * Input: pucSrcData ----------- 源报文指针
 *                   pstDestData ---------- 目的转换结构指针
 *                   ulIcmpv6HeadOffset --- ICMPv6报头偏移量
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
    VOS_UINT32                     datatemp; /* linux 4字节对齐 */
    VOS_INT32                      rlt;

    if (dataLen <= (IP_IPV6_HEAD_LEN + IP_ICMPV6_HEAD_LEN)) {
        IPND_ERROR_LOG1(NDIS_NDSERVER_PID, "IP_NDSERVER_BuildTooBigICMPPkt: ulDataLen is too short.", dataLen);
        return IP_FAIL;
    }
    /* 根据自身MAC地址生成link-local地址 */
    IP_NDSERVER_InitAddrInfo(&srcIPAddr, &srcMacAddr, &dstIPAddr, &dstMacAddr);
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* 得到目的IPV6地址 */
    rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 得到目的MAC地址 */
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    rlt     = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 指向ICMP首部 */
    data = destData + IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* ICMP type域 */
    *data = IP_ICMPV6_PACKET_TOO_BIG;
    data++;

    /* ICMP code域 */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* ICMP 校验和域 */
    *(VOS_UINT16 *)(VOS_VOID *)data = 0;
    data += 2; /* 2:len */

    /* MTU 域 */
    /*lint -e960*/
    datatemp = VOS_HTONL(dataLen); /* linux 4字节对齐 */
    rlt      = memcpy_s(data, sizeof(VOS_UINT32), (VOS_UINT8 *)(&datatemp), sizeof(VOS_UINT32)); /* linux 4字节对齐 */
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    /*lint +e960*/
    data += 4; /* 4:len */

    /* 填写ICMP PayLoad部分 */
    rlt = memcpy_s(data, (IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - IP_IPV6_HEAD_LEN - IP_ICMPV6_HEAD_LEN), ipMsg,
                   ((dataLen - IP_IPV6_HEAD_LEN) - IP_ICMPV6_HEAD_LEN));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 指向IPV6首部 */
    destData += IP_ETHERNET_HEAD_LEN;
    /* 填写IPV6头部 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, (dataLen - IP_IPV6_HEAD_LEN), destData, IP_HEAD_PROTOCOL_ICMPV6);

    /* 生成ICMPv6报头校验和 */
    if (IP_BuildIcmpv6Checksum(destData, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormRaMsg: Build ICMPv6 Checksum failed.");
        return IP_FAIL;
    }

    /* 设置以太报头 */
    destData -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, destData);

    return IP_SUCC;
}

/*
 * 功能描述: 处理PC发送的超长包
 * 修改历史:
 *  1.日    期: 2011年12月9日
 *    修改内容: 新生成函数
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

    /* 将超长包响应发送到PC */
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
 * Description: 形成向PC回复的ECHO REPLY
 * Input: pucSrcData ----------- 源报文指针
 *                   pstDestData ---------- 目的转换结构指针
 *                   ulIcmpv6HeadOffset --- ICMPv6报头偏移量
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

    /* 根据自身MAC地址生成link-local地址 */
    rlt = memcpy_s(srcMacAddr.macAddr, IP_MAC_ADDR_LEN, (VOS_VOID *)Ndis_GetMacAddr(), IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);
    IP_ProduceIfaceIdFromMacAddr(srcIPAddr.ipv6Addr, srcMacAddr.macAddr);
    IP_SetUint16Data(srcIPAddr.ipv6Addr, IP_IPV6_LINK_LOCAL_PREFIX);

    /* 得到目的IPV6地址 */
    rlt = memcpy_s(dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN, ipMsg + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 得到目的MAC地址 */
    teInfo = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);
    rlt     = memcpy_s(dstMacAddr.macAddr, IP_MAC_ADDR_LEN, teInfo->teLinkLayerAddr, IP_MAC_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* 指向ICMP首部 */
    data = destData + IP_ETHERNET_HEAD_LEN + IP_IPV6_HEAD_LEN;

    /* 填写ICMP报文 */
    ipMsg += IP_IPV6_HEAD_LEN;
    rlt = memcpy_s(data, (IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - IP_IPV6_HEAD_LEN), ipMsg,
                   (dataLen - IP_IPV6_HEAD_LEN));
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, IP_FAIL);

    /* ICMP type域 */
    *data = IP_ICMPV6_TYPE_ECHOREPLY;
    data++;

    /* ICMP code域 */
    *data = IP_IPV6_ND_VALID_CODE;
    data++;

    /* ICMP 校验和域 */
    *(VOS_UINT16 *)(VOS_VOID *)data = 0;

    /* 指向IPV6首部 */
    destData += IP_ETHERNET_HEAD_LEN;
    /* 填写IPV6头部 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, (dataLen - IP_IPV6_HEAD_LEN), destData,
                            IP_HEAD_PROTOCOL_ICMPV6);

    /* 生成ICMPv6报头校验和 */
    if (IP_BuildIcmpv6Checksum(destData, IP_IPV6_HEAD_LEN) != IP_SUCC) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_FormEchoReply: Build ICMPv6 Checksum failed.");
        return IP_FAIL;
    }

    /* 设置以太报头 */
    destData -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, destData);

    return IP_SUCC;
}

/*
 * Function Name: IP_NDSERVER_EchoRequestMsgProc
 * Description: 处理ECHO REQUEST消息
 * Input: ulIndex  -------------- 处理消息实体索引
 *                   pucSrcData ------------ IP数据报
 *                   ulIcmpv6HeadOffset ---- ICMPv6报文头偏移量
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

    /* 将ECHO REPLY发送到PC */
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
    /* 取ICMPV6消息中的TYPE字段 */
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
         * MT关机再开机后，ND SERVER收到网侧RA前，可能先收到TE的重复地址检测NS ，为了能够处理此类消息，
         * 此处临时申请一个ND Ser Addr Info
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

    /* 判断消息是否合法的ND消息 */
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
            (*replyUdpDataLen) += (*dhcpRequestDnsOptLen + 4); /* 4表示2个字节的option-code+2字节的option-len */
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

    /* 得到目的MAC地址 */
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

    /* 4表示2个字节的option-code+2字节的option-len */
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
    /* 填充 IP 头 */
    IP_ND_FormIPv6HeaderMsg(srcIPAddr, dstIPAddr, replyUdpDataLen, (VOS_UINT8 *)(dhcpReplyIpv6Hdr),
                            IP_HEAD_PROTOCOL_UDP);

    /* 填充 UDP 头 */
    (VOS_VOID)TTF_NDIS_InputUDPHead((VOS_UINT8 *)(dhcpReplyUdpHdr), IP_IPV6_DHCP6_UE_PORT, IP_IPV6_DHCP6_PC_PORT,
                                    replyUdpDataLen);
}
VOS_UINT32 IP_FillIdInfo(ND_MacAddrInfo srcMacAddr, VOS_UINT8 **dhcpReplyDhcpHdrOffset, VOS_UINT32 *tmpDestLen,
                         IP_Ipv6PktDhcpOptHdr *dhcpClientIdOpt, VOS_UINT8 *dhcpInfoReqData)
{
    /* 填充 DHCP reply 头 */
    *(*dhcpReplyDhcpHdrOffset) = IP_IPV6_DHCP6_TYPE_REPLY;
    *dhcpReplyDhcpHdrOffset += 1;
    if (memcpy_s(*dhcpReplyDhcpHdrOffset, IP_IPV6_DHCP6_TRANS_ID_LEN,
                 ((dhcpInfoReqData + IP_IPV6_HEAD_LEN) + IP_UDP_HEAD_LEN) + 1, IP_IPV6_DHCP6_TRANS_ID_LEN) != EOK) {
        return PS_FAIL;
    }

    *dhcpReplyDhcpHdrOffset += IP_IPV6_DHCP6_TRANS_ID_LEN;

    /* 填充 DHCP ServerID option (DUID 使用第三种方式即 DUID-LL) */
    if (IP_FillDhcpServerIdOption(*dhcpReplyDhcpHdrOffset, srcMacAddr) == PS_FAIL) {
        return PS_FAIL;
    }

    *dhcpReplyDhcpHdrOffset += sizeof(VOS_UINT16) + sizeof(VOS_UINT16) + IP_IPV6_DHCP_DUID_LL_OPT_LEN;

    *tmpDestLen = IP_IPM_MTU - IP_ETHERNET_HEAD_LEN - sizeof(NDIS_Ipv6Hdr) - sizeof(IP_UdpHeadInfo) -
                 IP_IPV6_DHCP6_REPLY_HDR_LEN - IP_IPV6_DHCP_DUID_LL_OPT_LEN;

    /* 填充 DHCP CLient ID option */
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
    /* 获取 DHCP Client ID Option */
    (VOS_VOID)TTF_NDIS_Ipv6GetDhcpOption(reqDhcpOptHdr, reqDhcpOptLen, IP_IPV6_DHCP_OPT_CLIEND_ID,
                                         dhcpClientIdOpt, 0);
    if (*dhcpClientIdOpt != VOS_NULL_PTR) {
        dhcpClientIdOptLen = VOS_NTOHS((*dhcpClientIdOpt)->dhcpOptLen);
        *replyUdpDataLen += (dhcpClientIdOptLen + 4); /* 4表示2个字节的option-code+2字节的option-len */
    }

    /* 获取 DHCP Request Option */
    (VOS_VOID)TTF_NDIS_Ipv6GetDhcpOption(reqDhcpOptHdr, reqDhcpOptLen, IP_IPV6_DHCP_OPT_REQUEST,
                                         dhcpRequestOpt, 0);
}
VOS_VOID IP_NDSERVER_GetFillReplyInfoAddr(VOS_UINT8 **sendBuff, NDIS_Ipv6Hdr **dhcpReplyIpv6Hdr,
                                          IP_UdpHeadInfo **dhcpReplyUdpHdr, VOS_UINT8 **dhcpReplyDhcpHdrOffset)
{
    *sendBuff = IP_NDSERVER_GET_SENDMSG_BUFFER();
    (VOS_VOID)memset_s(*sendBuff, IP_IPM_MTU, IP_NULL, IP_IPM_MTU);
    /* 指向IPV6首部 */
    *sendBuff += IP_ETHERNET_HEAD_LEN;
    *dhcpReplyIpv6Hdr     = (NDIS_Ipv6Hdr *)(*sendBuff);
    *dhcpReplyUdpHdr      = (IP_UdpHeadInfo *)((*dhcpReplyIpv6Hdr) + 1);
    *dhcpReplyDhcpHdrOffset = (VOS_UINT8 *)((*dhcpReplyUdpHdr) + 1);
}
/*
 * Description: 组装并发送DHCPV6报文
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
    VOS_UINT8                     *dhcpReplyDhcpHdrOffset      = VOS_NULL_PTR; /* 移动指针 */
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

    /* 根据自身MAC地址生成link-local地址 */
    IP_GetSrcAddr(&srcMacAddr, &srcIPAddr);
    /* 得到目的IPV6地址 */
    IP_GetDstAddr(&dstIPAddr, &dstMacAddr, addrIndex, dhcpInfoReqData);

    IP_NDSERVER_GetFillReplyInfoAddr(&sendBuff, &dhcpReplyIpv6Hdr, &dhcpReplyUdpHdr, &dhcpReplyDhcpHdrOffset);

    IP_NDSERVER_FillIpUdpInfo(dhcpReplyIpv6Hdr, dhcpReplyUdpHdr, srcIPAddr, dstIPAddr, replyUdpDataLen);

    if (IP_FillIdInfo(srcMacAddr, &dhcpReplyDhcpHdrOffset, &tmpDestLen, dhcpClientIdOpt, dhcpInfoReqData) != PS_SUCC) {
        return PS_FAIL;
    }

    /* 填充 DHCP DNS OPTION */
    if (IP_FillDhcpDnsOption(dhcpReplyDhcpHdrOffset, dnsSer, dnsOptLen, tmpDestLen, dhcpRequestDnsOptLen) == PS_FAIL) {
        return PS_FAIL;
    }
    dhcpReplyUdpHdr->udpCheckSum = TTF_NDIS_Ipv6_CalcCheckSum((VOS_UINT8 *)dhcpReplyUdpHdr, replyUdpDataLen,
        &dhcpReplyIpv6Hdr->srcAddr, &dhcpReplyIpv6Hdr->dstAddr, IP_HEAD_PROTOCOL_UDP);
    /* 指向以太网首部 */
    sendBuff -= IP_ETHERNET_HEAD_LEN;
    IP_NDSERVER_FormEtherHeaderMsg(srcMacAddr, dstMacAddr, sendBuff);

    IP_NDSERVER_AddTransDhcpv6PktNum(addrIndex);
    IP_NDSERVER_AddTransPktTotalNum(addrIndex);

    /* 限制reply消息长度 */
    dhcpReplyPktLen = TTF_MIN(IP_IPV6_HEAD_LEN + replyUdpDataLen + IP_ETHERNET_HEAD_LEN, IP_IPM_MTU);

    /* 将DHCPV6 REPLY消息发送到PC */
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
    dstPort  = *(VOS_UINT16 *)(VOS_VOID *)(udpData + 2); /* 2:从srcport偏移两个字节获取dstport */

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
 * Description: 处理DHCPV6报文
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

    /* Nd Server没有DNS情况下收到Dhcpv6 information request,丢弃该报文 */
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
 * Description: 发送邻居请求后，等待邻居公告超时
 * Input: pMsg -------- 消息指针
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp is entered.");

    /* 从消息中取出ND SERVER索引 */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp: ND Server state err!");
        return;
    }

    /* 判定超时次数是否小于规定超时次数 */
    if (infoAddr->timerInfo.loopTimes < g_nsTimerMaxExpNum) {
        /* 发送NS消息到PC */
        if (PS_SUCC != IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr)) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp:Send NS Msg failed.");
        }

        /* 将定时器超时次数加1 */
        infoAddr->timerInfo.loopTimes++;

        /* 重启邻居请求定时器 */
        IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);
    } else {
        /* 清除定时器信息 */
        infoAddr->timerInfo.loopTimes = 0;

        /* 如果未完成状态的TE地址不响应地址解析请求，则清除TE地址，重发RA */
        if (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INCOMPLETE) {
            infoAddr->teAddrInfo.teAddrState = IP_NDSERVER_TE_ADDR_INEXISTENT;

            /* 发送RA消息到PC */
            if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
                IP_NDSERVER_AddTransPktFailNum(addrIndex);
                IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgNsExp:Send RA Msg failed.");
            }

            /* 启动路由公告定时器 */
            IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
            IP_NDSERVER_ClearDlPktQue(addrIndex); /* 清除下行缓存队列中的PKT */
#endif
        } else {
            /* 启动周期性邻居请求定时器 */
            IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_PERIODIC_NS);
        }
    }

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgPeriodicNsExp
 * Description: 周期性邻居请求超时
 * Input: pMsg -------- 消息指针
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgPeriodicNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* 打印进入该函数 */
    IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp is entered.");

    /* 从消息中取出ND SERVER索引 */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp: ND Server state err!");
        return;
    }

    /* 增加周期性发送RA计时，防止Router过期 */
    if (g_periodicRaTimeCnt[addrIndex]-- == 0) {
        g_periodicRaTimeCnt[addrIndex] = g_periodicRaTimerLen / g_periodicNsTimerLen;
        ;

        /* 发送RA消息到PC */
        if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
            IP_NDSERVER_AddTransPktFailNum(addrIndex);
            IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp:Send RA Msg failed.");
        }
    }

    /* 发送NS消息到PC */
    if (IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgPeriodicNsExp:Send NS Msg failed.");
    }

    /* 清除定时器信息 */
    infoAddr->timerInfo.loopTimes = 0;
    /* 启动邻居请求定时器 */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgPeriodicNsExp
 * Description: 收到重复地址检测后等待定时器超时
 * Input: pMsg -------- 消息指针
 * History:
 *      1.      2011-04-09  Draft Enact
 */
STATIC VOS_VOID IP_NDSERVER_ProcTimerMsgFirstNsExp(VOS_UINT32 addrIndex)
{
    IP_NdserverAddrInfo    *infoAddr = IP_NULL_PTR;
    IP_NdserverTeAddrInfo  *teInfo   = IP_NULL_PTR;

    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp is entered.");

    /* 从消息中取出ND SERVER索引 */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    teInfo   = IP_NDSERVER_ADDRINFO_GET_TEINFO(addrIndex);

    if ((infoAddr->validFlag != IP_TRUE) ||
        (infoAddr->teAddrInfo.teAddrState == IP_NDSERVER_TE_ADDR_INEXISTENT)) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp: ND Server state err!");
        return;
    }

    /* 发送NS消息到PC */
    if (IP_NDSERVER_SendNsMsg(addrIndex, teInfo->teGlobalAddr) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgFirstNsExp:Send NS Msg failed.");
    }

    /* 清除定时器信息 */
    infoAddr->timerInfo.loopTimes = 0;
    /* 启动邻居请求定时器 */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_NS);

    return;
}

/*
 * Function Name: IP_NDSERVER_ProcTimerMsgRaExp
 * Description: 收到重复地址检测前周期发送路由公告超时
 * Input: pMsg -------- 消息指针
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

    /* 打印进入该函数 */
    IPND_INFO_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp is entered.");

    /* 从消息中取出ND SERVER索引 */
    IP_ASSERT(addrIndex < IP_NDSERVER_ADDRINFO_MAX_NUM);

    infoAddr = IP_NDSERVER_ADDRINFO_GET_ADDR(addrIndex);
    if (infoAddr->validFlag != IP_TRUE) {
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp: ND Server state err!");
        return;
    }

    /* 发送RA消息到PC */
    if (IP_NDSERVER_SendRaMsg(addrIndex, VOS_NULL_PTR) != PS_SUCC) {
        IP_NDSERVER_AddTransPktFailNum(addrIndex);
        IPND_ERROR_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_ProcTimerMsgRaExp:Send RA Msg failed.");
    }

    /* 清除定时器信息 */
    infoAddr->timerInfo.loopTimes = 0;
    /* 启动路由公告定时器 */
    IP_NDSERVER_TimerStart(addrIndex, IP_ND_SERVER_TIMER_RA);

    /* NDIS下移C核，无缓存队列 */
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (VOS_OS_VER != VOS_WIN32)
    /* *********** 下行IP包的目的地址作为NS包的目标地址，进行地址解析 ********* */
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

    /* 得到目的IPV6地址，触发NS发送 */
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
 * Description: ND SERVER TIMER消息分发函数
 * History:
 *      1.     2011-04-02  Draft Enact
 */
NDSER_TimerUint32 IP_NDSERVER_TimerMsgDistr(const MsgBlock *rcvMsg)
{
    VOS_UINT32              timerIndex;
    NDSER_TimerUint32       timerType;
    IP_TimerInfo           *timerInfo = VOS_NULL_PTR;

    /* 从消息中取出ulIndex和enTimerType */
    timerIndex     = IP_GetTimerPara(rcvMsg);
    timerType = (NDSER_TimerUint32)IP_GetTimerName(rcvMsg);
    /* 判断合法性 */
    if (IP_NDSERVER_IsTimerNameValid(timerIndex, timerType) == IP_FALSE) {
        /* 打印异常信息 */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: Invalid Timer Name or Para !");
        return IP_MSG_DISCARD;
    }

    /* 根据消息对应的索引号和定时器类型,获取相关联的定时器 */
    timerInfo = IP_NDSERVER_GetTimer(timerIndex, timerType);
    if (timerInfo == VOS_NULL_PTR) {
        /* 打印异常信息 */
        IPND_WARNING_LOG(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: Get Timer failed.");
        return IP_MSG_DISCARD;
    }

    /* 判断消息类型与定时器类型是否一致 */
    if (timerType != timerInfo->timerName) {
        /* 打印异常信息 */
        IPND_WARNING_LOG2(NDIS_NDSERVER_PID, "IP_NDSERVER_TimerMsgDistr: TimerType not match:", timerType,
                          timerInfo->timerName);
        return IP_MSG_DISCARD;
    }

    /* 定时器超时处理 */
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
 * 功能描述: PP NDIS上行PID消息处理函数
 * 修改历史:
 *  1.日    期: 2012年12月9日
 *    修改内容: 新生成函数
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
 * Description: IP NDSERVER模块命令显示
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
 * Description: 显示本地保存的网络参数信息
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
 * Description: 显示报文统计信息
 * Input: ulIndex --- 实体索引
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

